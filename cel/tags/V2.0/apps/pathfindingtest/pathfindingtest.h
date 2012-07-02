/*
    Copyright (C) 2010 by Leonardo Rodrigo Domingues

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef PATHFINDINGTEST_H
#define PATHFINDINGTEST_H

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
#include "behave.h"

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
  bool destinationSet;

  csList<csSimpleRenderMesh>* navStructMeshes;
  csList<csSimpleRenderMesh>* pathMeshes;
  csList<csSimpleRenderMesh>* destinationMeshes;
  bool clearMeshes;
  bool updateMeshes;
  bool updatePathMeshes;
  bool updateDestinationMeshes;
  
  csRef<iCamera> camera;
  csRef<iPcCamera> pcCamera;
  csRef<iCelZone> zone;
  csRef<iCelRegion> region;
  csRef<iCelMapFile> mapfile;
  csRef<iCelEntity> levelEntity; // iCelEntity
  csRef<iCelEntity> playerEntity; // iCelEntity
  csRef<iCelEntity> boxEntity; // iCelEntity
  bool renderNavMesh;
  bool renderDestination;
  bool renderPath;
  bool renderBox;
  bool updateNavmesh;
  csBox3 updateArea;

  void Frame ();
  bool LoadLevel ();
  bool LoadPlugins ();
  bool CreatePlayer ();
  bool CreateBox ();
  bool TestPath ();

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


#endif // PATHFINDINGTEST_H
