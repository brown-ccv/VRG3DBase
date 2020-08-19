

#ifndef VRG3DBASEVRAPP_H
#define VRG3DBASEVRAPP_H

#include <api/MinVR.h>
#include <VRG3DApp.h>
#include <CommonInc.H>
#include <MouseToTracker.h>
#include "GfxMgr.H"
#include "EventMgr.H"



class VRG3DBaseApp : public MinVR::VRG3DApp
{
  public:
     VRG3DBaseApp(int argc, char *argv[]);
     virtual ~VRG3DBaseApp();
     
    //PLUGIN_API virtual void onRenderGraphicsContext(const VRGraphicsState& state);
     virtual void onRenderGraphicsScene(const MinVR::VRGraphicsState& state);
     virtual void onCursorMove(const MinVR::VRCursorEvent &event);
     /*virtual void onAnalogChange(const MinVR::VRAnalogEvent &event);*/
     virtual void onButtonDown(const MinVR::VRButtonEvent &event);
     virtual void onButtonUp(const MinVR::VRButtonEvent &event);
     virtual void onTrackerMove(const MinVR::VRTrackerEvent &event);


  protected:
    
    GfxMgrRef         _gfxMgr;
    EventMgrRef       _eventMgr;
    MinVR::MouseToTrackerRef _mouseToTracker;


    
    
};





#endif // !VRG3DBASEAPP




