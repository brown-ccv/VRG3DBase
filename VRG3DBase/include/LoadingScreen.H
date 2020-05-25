/**
 * /author Dan Keefe (dfk)
 *
 * /file  LoadingScreen.H
 * /brief 
 *
 */ 

#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <CommonInc.H>
#include "ConfigVal.H"
#include "GfxMgr.H"



   class LoadingScreen
{
public:
   static void renderAndSwapBuffers(GfxMgrRef gfxMgr, double percentDone);

   static void render(GfxMgrRef gfxMgr, double percentDone);
};


#endif

