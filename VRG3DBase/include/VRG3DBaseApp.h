

#ifndef VRG3DBASEVRAPP_H
#define VRG3DBASEVRAPP_H

#include <api/MinVR.h>
#include <VRG3DApp.h>
#include <CommonInc.H>
#include "GfxMgr.H"
#include "EventMgr.H"



class VRG3DBaseApp : public MinVR::VRG3DApp
{
  public:
     VRG3DBaseApp(int argc, char *argv[]);
     virtual ~VRG3DBaseApp();
    //PLUGIN_API virtual void onRenderGraphicsContext(const VRGraphicsState& state);
     virtual void onRenderGraphicsScene(const MinVR::VRGraphicsState& state);

  protected:
    
    GfxMgrRef         _gfxMgr;
    G3D::Color4                _clearColor;
    EventMgrRef       _eventMgr;
    //MouseToTrackerRef _mouseToTracker;


    
    
};





#endif // !VRG3DBASEAPP




