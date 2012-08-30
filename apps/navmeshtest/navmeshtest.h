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
#ifndef NAVMESHTEST_H
#define NAVMESHTEST_H

#include "cstool/demoapplication.h"
#include <crystalspace.h>
#include <tools/celgraph.h>
#include <tools/celnavmesh.h>
#include <tools/celhpf.h>
#include <iostream>
#include <fstream>

class FramePrinter;

class MainApp : public CS::Utility::DemoApplication
{
private:
  csRef<iCollideSystem> cdsys;
  csRef<iRenderManager> rm;

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

  csArray<csSimpleRenderMesh*>* navStructMeshes;
  csArray<csSimpleRenderMesh*>* pathMeshes;
  csArray<csSimpleRenderMesh*>* pathEndMeshes;
  //csArray<csSimpleRenderMesh*> originMeshes;
  //csArray<csSimpleRenderMesh*> destinationMeshes;
  bool clearMeshes;
  bool updateMeshes;
  bool updatePathMeshes;
  bool updateOriginMeshes;
  bool updateDestinationMeshes;

  csRef<iCelHPath> path;
  csRef<iCelHNavStruct> navStruct;
  csRef<iCelNavMeshParams> params;

  csRef<iMapNode> mapNodeOrigin;
  csRef<iMapNode> mapNodeTarget;
  csRefArray<iMapNode> mapNodes;

  int GetMapNodes ();
  float GetMapNodesDistance ();
  void CycleMapNode (csRef<iMapNode>& mapnode);
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

  void PrintHelp ();
  bool OnInitialize (int argc, char* argv[]);
  void OnExit ();
  bool Application ();

  CS_EVENTHANDLER_NAMES ("application.navmesh")
  CS_EVENTHANDLER_NIL_CONSTRAINTS
};


#endif // NAVMESHTEST_H
