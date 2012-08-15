#ifndef APP_H
#define APP_H

#include "cstool/demoapplication.h" 
#include <physicallayer/entity.h>
#include <physicallayer/pl.h>
#include <tools/celnavmesh.h>
#include <tools/celhpf.h>
#include "behave.h"

class FramePrinter;

class MainApp : public CS::Utility::DemoApplication 
{
private:
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

  csArray<csSimpleRenderMesh*>* navStructMeshes;
  csArray<csSimpleRenderMesh*>* pathMeshes;
  bool renderNavMesh;
  bool renderPath;

  bool OnKeyboard (iEvent&);

  csRef<iPcCamera> pccamera;
  void Frame ();
  
  bool LoadLevel ();
  bool CreatePlayer ();
  bool LoadSteering ();

public:
  MainApp ();
  virtual ~MainApp ();

  void PrintHelp ();
  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
  virtual void OnExit ();
};


#endif
