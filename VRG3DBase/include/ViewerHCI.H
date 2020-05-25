/**
 * \author Daniel Keefe (dfk)
 *
 * \file  ViewerHCI.H
 * \brief
 *
 */


#ifndef VIEWERHCI_H
#define VIEWERHCI_H

#include <CommonInc.H>
#include "EventMgr.H"
#include "Fsa.H"
#include "GfxMgr.H"

/**
  This viewer assumes the model is centered around (0,0,0).

  Controls:

  Mouse with left btn - trackball rotate
  Arrow keys          - pan left, right, up, and down
  "I" and "O"         - dolly in and out
  PAGEUP and PAGEDOWN - scale model up/down
  SPACEBAR            - start/stop rocking the model back and forth
  "F" and "S"         - make rocking faster or slower
  "W" and "N"         - make rocking angle wider or narrower
*/
class ViewerHCI
{
public:  
   ViewerHCI(EventMgrRef eventMgr, GfxMgrRef gfxMgr);
   virtual ~ViewerHCI();

   void trackballOn(MinVR::EventRef e);
   void trackballMove(MinVR::EventRef e);
   void trackballOff(MinVR::EventRef e);

   void toggleRocking(MinVR::EventRef e);
   void speedUpRocking(MinVR::EventRef e);
   void slowDownRocking(MinVR::EventRef e);
   void incRockingAngle(MinVR::EventRef e);
   void decRockingAngle(MinVR::EventRef e);

   void panLeft(MinVR::EventRef e);
   void panRight(MinVR::EventRef e);
   void panUp(MinVR::EventRef e);
   void panDown(MinVR::EventRef e);
   void dollyIn(MinVR::EventRef e);
   void dollyOut(MinVR::EventRef e);
   void scaleUp(MinVR::EventRef e);
   void scaleDown(MinVR::EventRef e);

   void poseForAnimation(G3D::Array<G3D::SurfaceRef> &posedModels, const G3D::CoordinateFrame &virtualToRoomSpace);

   void setRotCenter(G3D::Vector3 v){
    _boundingSphere.center=v;
  }

protected:
  EventMgrRef     _eventMgr;
  FsaRef          _fsa;
  GfxMgrRef       _gfxMgr;
  double          _lastRotTime;
  double          _rotAngularVel;
  G3D::Sphere          _boundingSphere;
  G3D::Vector3         _lastIntersectionPt;
  int             _animationCallbackID;
  double          _panAmt;
  bool            _rocking;
  double          _rockingDir;
  double          _rockingAngle;
  double          _totalAngle;
};

#endif
