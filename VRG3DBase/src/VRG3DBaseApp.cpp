#include "../include/VRG3DBaseApp.h"
#include "../include/ConfigVal.H"
#include <VRG3DEvent.h>

	

  VRG3DBaseApp::VRG3DBaseApp(int argc, char *argv[])
    :VRG3DApp(argc,argv)
  {
    _log = new G3D::Log("log.txt");

    std::string vrSetupStr = "desktop";
    if (argc >= 2) {
      std::string argStr = std::string(argv[1]);
      if (argStr[0] != '-') {
        vrSetupStr = argStr;
      }
    }

    // initialize the VRApp
    //init(vrSetupStr, _log);

    // Startup ConfigVals
    MinVR::ConfigValMap::map = new MinVR::ConfigMap(argc, argv, _log, false);

    G3D::GLCaps::init();
    // Startup the gfx mgr
    _gfxMgr = new GfxMgr(myRenderDevice, _cameras[0]);
    G3D::Color3 defaultLtCol = G3D::Color3(0.35f, 0.35f, 0.35f);
    _gfxMgr->getLighting()->ambientTop = MinVR::ConfigVal("AmbientTop", defaultLtCol, false);
    _gfxMgr->getLighting()->ambientBottom = MinVR::ConfigVal("AmbientBottom", defaultLtCol, false);
    _gfxMgr->getLighting()->lightArray.append(G3D::GLight::directional(G3D::Vector3(0, 1, 1).unit(), G3D::Color3(0.5, 0.5, 0.5)));
    _gfxMgr->loadTexturesFromConfigVal("LoadTextures", _log);

    // Startup the event mgr
    _eventMgr = new EventMgr(_log);

    // startup a mouse-based standin for 6DOF trackers if we're on a desktop system
    if (G3D::beginsWith(vrSetupStr, "desk")) {
      _mouseToTracker = new MinVR::MouseToTracker(_gfxMgr->getCamera(), MinVR::ConfigVal("MouseToTracker_NumTrackers", 2, false));
    }

#ifdef WITH_PHOTON
	std::vector<MinVR::VRInputDevice*> devices = getInputDevices();
	for (int i = 0; i < devices.size(); i++)
	{
		photon = dynamic_cast<MinVR::VRPhotonDevice*>(devices[i]);
		if (photon != nullptr)
			break;
	}
#endif
  }

  VRG3DBaseApp::~VRG3DBaseApp()
  {

  }

  void VRG3DBaseApp::onRenderGraphicsContext(const MinVR::VRGraphicsState& state)
  {
    if (_eventMgr!=nullptr)
    {
#ifdef WITH_PHOTON
		if (photon)
			_eventMgr->setPhotonProperties(photon->getServerTime(), photon->getUsername());
#endif
		_eventMgr->processEventQueue();
#ifdef WITH_PHOTON
		if (photon) {
			photon->addEvents(&_eventMgr->getOutEvents());
		}
#endif
    }
    VRG3DApp::onRenderGraphicsContext(state);
  }

  void VRG3DBaseApp::onRenderGraphicsScene(const MinVR::VRGraphicsState& state)
  {
     G3D::Matrix4 g3dViewMatrix = state.getViewMatrix();
     G3D::Matrix4 g3dProjectionMatrix = state.getProjectionMatrix();

     // _gfxMgr->getCamera() and _cameras[0] should be the same camera
     _gfxMgr->getCamera()->setViewMatrixCoordinate(g3dViewMatrix);
     _gfxMgr->getCamera()->setProjectionMatrix(g3dProjectionMatrix);
     _cameras[0]->applyProjection(myRenderDevice, state);
     //_gfxMgr->drawFrame();
    //_gfxMgr->drawStats();
    //vrg3dSleepMsecs(ConfigVal("VRBaseApp_SleepTime", 0.0, false));
  }

   void VRG3DBaseApp::onCursorMove(const MinVR::VRCursorEvent &event)
   {
     
     if (_mouseToTracker.notNull()) {
       G3D::Array<MinVR::EventRef> events;
       if (event.getName() == "Mouse_Pointer")
       {
         const float* pos = event.getNormalizedPos();
         MinVR::EventRef vrg3dEvent = new MinVR::VRG3DEvent("Mouse_Pointer", Vector2(pos[0], pos[1]));
         events.append(vrg3dEvent);
       }
      
       
       G3D::Array<MinVR::EventRef> newEvents;
       _mouseToTracker->doUserInput(events, newEvents);
       events.append(newEvents);
       for (int i = 0; i < events.size(); i++) {
         
         _eventMgr->queueEvent(events[i]);
       }
       
       _gfxMgr->poseFrame();
     }
   }

   void VRG3DBaseApp::onAnalogChange(const MinVR::VRAnalogEvent &event)
   {
     _eventMgr->queueEvent(new MinVR::VRG3DEvent(event.getName(), event.getValue()));
     _gfxMgr->poseFrame();
   }


   void VRG3DBaseApp::onButtonUp(const MinVR::VRButtonEvent &state)
   {
     MinVR::EventRef vrg3dEvent = new MinVR::VRG3DEvent(state.getName());
     _eventMgr->queueEvent(vrg3dEvent);
   }

   void VRG3DBaseApp::onTrackerMove(const MinVR::VRTrackerEvent &event)
   {   
       const float * transformM = event.getTransform();
       Matrix4 g3dTransforMatrix(transformM);
       g3dTransforMatrix = g3dTransforMatrix.transpose();
       MinVR::EventRef vrg3dEvent = new MinVR::VRG3DEvent(event.getName(), g3dTransforMatrix.approxCoordinateFrame());
       _eventMgr->queueEvent(vrg3dEvent);
   }

   void VRG3DBaseApp::onGenericEvent(const MinVR::VRDataIndex &index) {
	   if (index.exists("Message")) {
		   std::string message = index.getValue("Message");
		   _eventMgr->decode_xml(message);
		   MinVR::EventRef vrg3dEvent = new MinVR::VRG3DEvent(index.getName(), message);
		   _eventMgr->queueEvent(vrg3dEvent);
	   }
   }

   void VRG3DBaseApp::onButtonDown(const MinVR::VRButtonEvent &state)
   {
     MinVR::EventRef vrg3dEvent = new MinVR::VRG3DEvent(state.getName());
     _eventMgr->queueEvent(vrg3dEvent);
   }


   


