#ifndef APP_H
#define APP_H

#include <physicallayer/entity.h>
#include <physicallayer/pl.h>
#include <tools/celnavmesh.h>
#include <tools/celhpf.h>
#include "behave.h"

class FramePrinter;

class MainApp : public csApplicationFramework,
                public csBaseEventHandler
{
private:
  csRef<iGraphics3D> g3d;
  csRef<iEngine> engine;
  csRef<iLoader> loader;
  csRef<iVFS> vfs;
  csRef<iVirtualClock> vc;
  csRef<iKeyboardDriver> kbd;
  csRef<FramePrinter> printer;

  csRef<iCelPlLayer> pl;
  csRef<BehaviourLayer> bl;
  csRef<iCelEntity> level_entity;
  csRef<iCelEntity> player_entity;
  csRef<iCelEntity> steering_entity;

  // Hierarchical pathfinding
  csRef<iCelHNavStructBuilder> navStructBuilder;  
  csRef<iCelHPath> path;
  csRef<iCelHNavStruct> navStruct;
  csRef<iCelNavMeshParams> params;
  float agentHeight;
  float agentRadius;

  // Path
  csVector3 origin;
  csVector3 destination;
  csRef<iSector> originSector;  
  csRef<iSector> destinationSector;
  bool destinationSet;

  csArray<csSimpleRenderMesh*>* navStructMeshes;
  csArray<csSimpleRenderMesh*>* pathMeshes;
  csArray<csSimpleRenderMesh*>* destinationMeshes;
  bool clearMeshes;
  bool updateMeshes;
  bool updatePathMeshes;
  bool updateDestinationMeshes;
  bool renderNavMesh;
  bool renderDestination;
  bool renderPath;
  bool updateNavmesh;
  csBox3 updateArea;

  bool OnKeyboard (iEvent&);

  csRef<iPcCamera> pccamera;
  void Frame ();
  
  bool LoadLevel ();
  bool CreatePlayer ();
  bool LoadSteering ();

public:
  MainApp ();
  virtual ~MainApp ();

  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
  virtual void OnExit ();
};


#endif
