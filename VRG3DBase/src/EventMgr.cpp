
#include "../include/EventMgr.H"

//#include "CommonInc.H"
#include "../include/ConfigVal.H"
#include "../include/StringUtils.H"
#include "SynchedSystem.h"

using namespace G3D;

EventMgr::EventMgr(Log *log)
{
  _log = log;
  _fullDebug        = MinVR::ConfigVal("EventMgr_FullDebug", false, false);
  _printAsQueued    = MinVR::ConfigVal("EventMgr_PrintAsQueued", false, false);
  _printAsProcessed = MinVR::ConfigVal("EventMgr_PrintAsProcessed", false, false);

  // Set aliases for events from the EventAliases ConfigVal
  std::string aliases = MinVR::ConfigVal("EventAliases","",false);
  std::string thisAlias;
  while (popUntilSemicolon(aliases, thisAlias)) {
    std::string eventName;
    if (popNextToken(thisAlias, eventName)) {
      Array<std::string> aliasedTo;
      aliasedTo = MinVR::splitStringIntoArray(thisAlias);
      if (aliasedTo.size()) {
        addEventAliases(eventName, aliasedTo); 
      }
      else {
        alwaysAssertM(false, "Empty list of aliases for event " + eventName);
      }
    }
    else {
      alwaysAssertM(false, "Expected alias description.");
    }
  }
}

EventMgr::~EventMgr()
{
}

int
EventMgr::numFsas()
{
  return _fsas.size();
}

FsaRef
EventMgr::getFsa(int i)
{
  debugAssert(i < _fsas.size());
  return _fsas[i];
}

void
EventMgr::addFsaRef(FsaRef fsa)
{
  _fsas.append(fsa);
  fsa->setDebug(_fullDebug);
}

void
EventMgr::removeFsaRef(FsaRef fsa)
{
  int i = _fsas.findIndex(fsa);
  if (i >= 0)
    _fsas.remove(i);
}

void
EventMgr::addDeviceLevelFsaRef(FsaRef fsa)
{
  _deviceLevelFsas.append(fsa);
  fsa->setDebug(_fullDebug);
}

void
EventMgr::setFullDebug(bool debug)
{
  _fullDebug = debug;
  for (int i=0;i<_fsas.size();i++) {
    _fsas[i]->setDebug(debug);
  }
}

void
EventMgr::queueEvent(MinVR::EventRef event)
{
  // apply any filters to pre-process events
  bool addToQueue = true;
  for (int i=0;i<_filters.size();i++) {
    if (!_filters[i]->filter(event))
      addToQueue = false;
  }
  if (!addToQueue) {
    return;
  }
  queueEventWithoutFilter(event);
}

void
EventMgr::queueEventWithoutFilter(MinVR::EventRef event)
{
  // queue up the event
  if ((_printAsQueued) || (_fullDebug)) {
    cout << "Queueing event: " << event->toString() << endl;
  }
  _eventQueue.append(event);
  
  _recentEventTable.set(event->getName(), event);

  // if this event is aliased to something else, then create events for 
  // each alias and queue them as well
  if (_eventAliases.containsKey(event->getName())) {
    Array<std::string> aliases = _eventAliases[event->getName()];
    for (int i=0;i<aliases.size();i++) {
      MinVR::EventRef newevent = createCopyOfEvent(event);
      newevent->rename(aliases[i]);
      if (_fullDebug) {
        cout << "Generating new event for alias: " << aliases[i] << endl;
      }
      queueEvent(newevent);
    }
  }
}


/*void
EventMgr::queueEventFromNetMsg(EventNetMsg &m)
{
  queueEvent(m.event);
}

void
EventMgr::queueEventsFromNetMsg(EventBufferNetMsg &m)
{
  for (int i=0;i<m.eventBuffer.size();i++) {
    queueEvent(m.eventBuffer[i]);
  }
}*/



void
EventMgr::processEvent(MinVR::EventRef event)
{
  if ((_printAsProcessed) || (_fullDebug))
    cout << "Processing event: " << event->toString() << endl;
  
  // Copy the array so that changes made to the member during a processEvent() 
  // call won't affect which fsa's get the event. All fsa's should be thought
  // of as receiving the event simultaneously.
  Array<FsaRef> fsas = _fsas;
  for ( int i = 0; i < fsas.size(); i++ ) {
    fsas[i]->processEvent(event);
  }
}

void
EventMgr::processEventDeviceLevel(MinVR::EventRef event)
{
  if ((_printAsProcessed) || (_fullDebug))
    cout << "Processing event at device level: " << event->toString() << endl;
  for (int i=0;i<_deviceLevelFsas.size();i++) {
    _deviceLevelFsas[i]->processEvent(event);
  }
}


void
EventMgr::processEventQueue()
{
  // Add any timer events whose time has come to the queue
  double now = MinVR::SynchedSystem::getLocalTime();
  int e=0;
  while (e<_timerEvents.size()) {
    if (_timerEventTimes[e] <= now) {
      queueEvent(_timerEvents[e]);
      _timerEvents.fastRemove(e);
      _timerEventTimes.fastRemove(e);
    }
    else {
      e++;
    }
  }

  // If an event name is listed as an event to compress, then if
  // multiple events of that name are generated in a single frame, we
  // only keep the last one.
  Array<std::string> eventsToCompress = MinVR::splitStringIntoArray(MinVR::ConfigVal("EventMgr_EventsToCompress","",false));
  for (int i=0;i<eventsToCompress.size();i++) {
    int j = _eventQueue.size()-1;
    bool gotFirst = false;
    while (j >= 0) {
      if ((_eventQueue[j]->getName() == eventsToCompress[i]) ||
          (_eventQueue[j]->getName() == eventsToCompress[i] + "_down") ||
          (_eventQueue[j]->getName() == eventsToCompress[i] + "up")) {
        if (!gotFirst) {
          gotFirst = true;
        }
        else {
          _eventQueue.fastRemove(j);
        }
      }
      j--;
    }
  }
  
  // First, device level Fsa's respond to Events.  Usually, these
  // device level Fsa's will generate additional Events and place
  // them on the Queue.
  if (_deviceLevelFsas.size()) {
    int size = _eventQueue.size();
    for (int i=0;i<size;i++) {
      processEventDeviceLevel(_eventQueue[i]);
    }
  }

  // Next, normal Fsa's respond to all events, including those generated
  // by device level Fsa's.
  for (int i=0;i<_eventQueue.size();i++) {
    processEvent(_eventQueue[i]);
  }

  _eventQueue.clear();
}


void
EventMgr::addEventAliases(const std::string &eventName,
                          const Array<std::string> &newEventNames)
{
  if (_log) {
    _log->print("Alias(es) for Event \"" + eventName + "\" are:  ");
    for (int i=0;i<newEventNames.size();i++) {
      _log->print("\"" + newEventNames[i] + "\" ");
    }
    _log->println("");
  }
  _eventAliases.set( eventName, newEventNames );
}


MinVR::EventRef
EventMgr::getMostRecentEvent(const std::string &eventName)
{
  if (!_recentEventTable.containsKey(eventName)) {
    return NULL;
  }
  else {
    return _recentEventTable[eventName];
  }
}

void
EventMgr::queueTimerEvent(MinVR::EventRef event, double queueTime)
{
  _timerEvents.append(event);
  _timerEventTimes.append(MinVR::SynchedSystem::getLocalTime() + queueTime);
}

#ifdef WITH_PHOTON
	MinVR::VRDataQueue & EventMgr::getOutEvents() {
		return _outEvents;
	}

	void EventMgr::setPhotonProperties(int timestamp, std::string user_Identifier) {
		_server_timestamp = timestamp;
		_user_identifier = user_Identifier;
	}

	std::string EventMgr::getUserIdentifer() {
		return _user_identifier;
	}

	int EventMgr::getServerTime() {
		return _server_timestamp;
	}

	void EventMgr::addEvent(std::string name, std::string message) {
		MinVR::VRDataIndex di(name);
		encode_xml(message);
		di.addData("Message", message);
		_outEvents.push(di);
	}
#endif

	void EventMgr::encode_xml(std::string& data) {
		std::string buffer;
		buffer.reserve(data.size());
		for (size_t pos = 0; pos != data.size(); ++pos) {
			switch (data[pos]) {
			case '&':  buffer.append("&amp;");       break;
			case '\"': buffer.append("&quot;");      break;
			case '\'': buffer.append("&apos;");      break;
			case '<':  buffer.append("&lt;");        break;
			case '>':  buffer.append("&gt;");        break;
			default:   buffer.append(&data[pos], 1); break;
			}
		}
		data.swap(buffer);
	}

	void EventMgr::decode_xml(std::string& data, std::string toSearch, std::string replaceStr) {
		size_t pos = data.find(toSearch);
		// Repeat till end is reached
		while (pos != std::string::npos)
		{
			// Replace this occurrence of Sub String
			data.replace(pos, toSearch.size(), replaceStr);
			// Get the next occurrence from the current position
			pos = data.find(toSearch, pos + replaceStr.size());
		}
	}

	void EventMgr::decode_xml(std::string& data) {
		decode_xml(data, "&amp;", "&");
		decode_xml(data, "&quot;", "\"");
		decode_xml(data, "&apos;", "\'");
		decode_xml(data, "&lt;", "<");
		decode_xml(data, "&gt;", "&");
	}

CoordinateFrame 
EventMgr::getCurrentHeadFrame()
{
  // The ConfigVal "HeadTracker" is typically assigned to the event name that comes one of the sensors
  // on a tracking device, for example "Polhemus_Head_Tracker" or "Intersense_Head_Tracker".
  MinVR::EventRef lastHeadMovement = getMostRecentEvent(MinVR::ConfigVal("HeadTracker", "Head_Tracker", false));
  if (lastHeadMovement.notNull()) {
    return lastHeadMovement->getCoordinateFrameData();
  }
  else {
    return CoordinateFrame(Vector3(0,0,1));
  }
}



