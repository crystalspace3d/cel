#ifndef MAINAPP_H
#define MAINAPP_H

#include <crystalspace.h>
#include <celtool/initapp.h>
#include "csutil/algorithms.h"
#include <physicallayer/entity.h>
#include <physicallayer/pl.h>
#include <physicallayer/propclas.h>
#include <propclass/zone.h>
#include <propclass/camera.h>
#include <propclass/mesh.h>
#include <propclass/linmove.h>
#include <propclass/actormove.h>
#include <propclass/input.h>
#include <tools/celgraph.h>
#include <tools/celnavmesh.h>
#include <tools/celhpf.h>
#include "BehaviourLayer.h"

class FramePrinter;

class MainApp : public csApplicationFramework, public csBaseEventHandler
{
private:
  // Plugins
  csRef<iGraphics3D> g3d;
  csRef<iEngine> engine;
  csRef<iVirtualClock> vc;
  csRef<iKeyboardDriver> kbd;
  csRef<iLoader> loader;
  csRef<iVFS> vfs;
  csRef<FramePrinter> printer;
  csRef<iCelPlLayer> physicalLayer;
  csRef<BehaviourLayer> behaviourLayer; // iCelBlLayer
  csRef<iCelHNavStructBuilder> navStructBuilder;

  // Hierarchical pathfinding
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
  bool originSet;
  bool destinationSet;
  
  csRef<iCamera> camera;
  csRef<iPcCamera> pcCamera;
  csRef<iCelZone> zone;
  csRef<iCelRegion> region;
  csRef<iCelMapFile> mapfile;
  csRef<iCelEntity> levelEntity; // iCelEntity
  csRef<iCelEntity> playerEntity; // iCelEntity
  bool renderNavMesh;
  bool renderDestination;
  bool renderPath;

  void Frame ();
  bool LoadLevel ();
  bool LoadPlugins ();
  bool CreatePlayer ();

  // Event handlers
  virtual bool OnKeyboard (iEvent& ev);
  virtual bool OnMouseClick (iEvent& ev);

  // Event handlers helpers
  void MouseClick1Handler (iEvent& ev);
  void MouseClick2Handler (iEvent& ev);
  void MouseClick3Handler (iEvent& ev);

public:
  MainApp ();
  virtual ~MainApp ();

  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
  virtual void OnExit ();
};


#endif
