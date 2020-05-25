/**
 * \author Daniel Keefe (dfk)
 *
 * \file  EventMgr.H
 *
 */

#ifndef EVENTMGR_H
#define EVENTMGR_H



//#include <SDL.h>
//#include <G3D/G3D.h>
//using namespace G3D;

#include "Fsa.H"
#include "EventFilter.H"
//#include "EventNet.h"


typedef G3D::ReferenceCountedPointer<class EventMgr> EventMgrRef;
/**
*/
class EventMgr : public G3D::ReferenceCountedObject
{
public:
   EventMgr(G3D::Log *log);
   virtual ~EventMgr();

  /// Adds a Fsa to the list of Fsa's that should respond to events
  /// as they are generated
   void   addFsaRef(FsaRef fsa);

  /// Removes a Fsa from the list that responds to incoming events
   void   removeFsaRef(FsaRef fsa);

  /// Device level Fsa's are processed before the normal Fsa's.  Add
  /// a Fsa as a device level fsa if it responds to events in order to
  /// generate new Events.  It just means that these Fsa's are processed
  /// first and the Events that they generate are only processed by normal
  /// Fsa's, not other DeviceLevel Fsa's.
    void   addDeviceLevelFsaRef(FsaRef fsa);

  /// Adds an event to the queue.  FSA's respond to the event the next
  /// time processEventQueue gets called.
   void   queueEvent(MinVR::EventRef event);
    void   queueEventWithoutFilter(MinVR::EventRef event);

  //void   queueEventFromNetMsg(EventNetMsg &m);
  //void   queueEventsFromNetMsg(EventBufferNetMsg &m);

   int    numFsas();
   FsaRef getFsa(int i);

  /// Sets up aliases for eventName.  Whenever an Event named eventName
  /// is queued, a copy of the Event renamed to the alias name, is also 
  /// queued.  You can add several aliases for each Event.
   void   addEventAliases(const std::string &eventName, const G3D::Array<std::string> &newEventNames);

  /// Filters provide a mechanism for pre-processing events.  You can
  /// intercept events and change them or block them, etc..  These are
  /// for very advanced use only, and in most cases you can do whatever
  /// you were thinking of doing with a filter with a Device-Level Fsa, 
  /// which is the preferred way of doing it.
   void   addEventFilter(EventFilterRef f) { _filters.append(f); }

   void   removeEventFilter(EventFilterRef f) {
      _filters.remove(_filters.findIndex(f)); 
  }

  /// Outputs event debugging printouts
   void   setFullDebug(bool debug);
  /// Outputs events as they are added to the event queue
   void   setPrintAsQueued(bool debug)    { _printAsQueued = debug; }
  /// Outputs event names just before the event is processed
   void   setPrintAsProcessed(bool debug) { _printAsProcessed = debug; }

  /// Usually called by the system mainloop to process all input
   void   processEventQueue();

  /// Processes this event immediately, event is not placed on the Queue
  /// and no queued events are processed.
   void   processEvent(MinVR::EventRef event);
   void   processEventDeviceLevel(MinVR::EventRef event);

  /// Tells the system to generate this event at queueTime seconds from now
   void   queueTimerEvent(MinVR::EventRef event, double queueTime);

  /// Returns a ref to the most recent event of the given name to be queued, or NULL
  /// if no events were found.
   MinVR::EventRef getMostRecentEvent(const std::string &eventName);



  /***  Shortcuts for Accessing Really Common System-Style Events  ***/

  /// Shortcuts for getting the data from the most recent head motion event.  In the
  /// case where there is no head tracking, the head/camera position is reported as
  /// the point (0,0,1).  All coordinates are in RoomSpace.  Assumes head tracking is
  /// setup so that head movement events are named "Head_Tracker".
   G3D::CoordinateFrame  getCurrentHeadFrame();

private:
  G3D::Log                   *_log;
  G3D::Array<MinVR::EventRef>       _eventQueue;
  G3D::Array<FsaRef>         _fsas;
  G3D::Array<FsaRef>         _deviceLevelFsas;
  G3D::Array<EventFilterRef> _filters;
  bool                  _fullDebug;
  bool                  _printAsQueued;
  bool                  _printAsProcessed;
  G3D::Table< std::string, G3D::Array<std::string> >  _eventAliases;
  G3D::Array<MinVR::EventRef>      _timerEvents;
  G3D::Array<double>        _timerEventTimes;

  /// For each Event name, maintains a copy of the most recent Event queued
  G3D::Table< std::string, MinVR::EventRef > _recentEventTable;
};

#endif

