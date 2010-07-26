#ifndef MAINAPP_H
#define MAINAPP_H

#include <crystalspace.h>
#include <tools/celgraph.h>
#include <tools/celnavmesh.h>
#include <tools/celhpf.h>
#include <iostream>
#include <fstream>

class FramePrinter;

class MainApp : public csApplicationFramework, public csBaseEventHandler
{
private:
  csRef<iGraphics3D> g3d;
  csRef<iEngine> engine;
  csRef<iLoader> loader;
  csRef<iVirtualClock> vc;
  csRef<iKeyboardDriver> kbd;
  csRef<iCollideSystem> cdsys;
  csRef<iRenderManager> rm;
  csRef<iView> view;
  csRef<iVFS> vfs;
  csRef<FramePrinter> printer;

  csRef<iSector> currentSector;
  csRef<iMeshWrapper> actor;
  csColliderActor collider_actor;
  csString mapLocation;

  csRef<iCelNavMeshBuilder> navMeshBuilder;
  csRef<iCelHNavStructBuilder> navStructBuilder;

  csVector3 agentStartPosition;
  csVector3 origin;
  csRef<iSector> originSector;
  csVector3 destination;
  csRef<iSector> destinationSector;
  bool originSet;
  bool destinationSet;

  csRef<iCelHPath> path;
  csRef<iCelHNavStruct> navStruct;
  csRef<iCelNavMeshParams> params;
  
  bool LoadMap ();
  bool CreateAgent ();
  void Frame ();

  // Event handlers
  virtual bool OnKeyboard (iEvent& ev);
  virtual bool OnMouseClick (iEvent& ev);

  // Event handlers helpers
  void MouseClick1Handler (iEvent& ev);
  void MouseClick2Handler (iEvent& ev);
  void MouseClick3Handler (iEvent& ev);

  bool SetupModules ();

public:
  MainApp();
  ~MainApp();

  bool OnInitialize (int argc, char* argv[]);
  void OnExit ();
  bool Application ();

  CS_EVENTHANDLER_NAMES ("application.navmesh")
  CS_EVENTHANDLER_NIL_CONSTRAINTS
};


#endif
