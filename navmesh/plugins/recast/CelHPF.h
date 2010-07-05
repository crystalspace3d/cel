/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
  
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

#ifndef __CEL_HPF__
#define __CEL_HPF__

#include <cssysdef.h>
#include <csgeom/vector3.h>
#include <cstool/mapnode.h>
#include <csutil/csstring.h>
#include <csutil/hash.h>
#include <csutil/scf_implementation.h>
#include <iengine/sector.h>
#include <iutil/comp.h>
#include <iutil/objreg.h>
#include <ivaria/mapnode.h>
#include <tools/celgraph.h>
#include <tools/celnavmesh.h>
#include <tools/celhpf.h>
#include "CelNavMesh.h"
#include "Recast.h"

CS_PLUGIN_NAMESPACE_BEGIN(celNavMesh)
{


/**
 * Path between two map nodes.
 */
class celHPath : public scfImplementationExt1<celHPath, csObject, iCelPath>
{
private:
  csRef<iCelPath> hlPath; // High level path
  csHash<csRef<iCelNavMesh>, csPtrKey<iSector> >& navMeshes;
  csArray<csRef<iCelNavMeshPath> > llPaths; // Low level paths
  size_t llSize; // Low level paths array size
  size_t currentllPosition; // Current position for low level paths array
  csRef<iMapNode> currentNode;
  csRef<iMapNode> tmpNode;
  csPtrKey<iSector> currentSector;
  csRef<iMapNode> firstFrom; // Optimization for celHPath::GetFirst
  csRef<iMapNode> firstGoal; // Optimization for celHPath::GetFirst
  csRef<iMapNode> lastFrom; // Optimization for celHPath::GetLast
  csRef<iMapNode> lastGoal; // Optimization for celHPath::GetLast

public:
  celHPath (csHash<csRef<iCelNavMesh>, csPtrKey<iSector> >& navMeshes);
  virtual ~celHPath ();

  void Initialize(iCelPath* highLevelPath);

  // iCelPath methods
  virtual iObject* QueryObject ();
  virtual void AddNode (iMapNode* node);
  virtual void InsertNode (size_t pos, iMapNode* node);
  virtual iMapNode* Next ();
  virtual iMapNode* Previous ();
  virtual iMapNode* Current ();
  virtual csVector3 CurrentPosition ();
  virtual iSector* CurrentSector ();
  virtual bool HasNext ();
  virtual bool HasPrevious ();
  virtual void Restart ();
  virtual void Clear ();
  virtual iMapNode* GetFirst ();
  virtual iMapNode* GetLast ();
  virtual void Invert ();
  virtual size_t GetNodeCount();
};



/**
 * Hierarchical navigation mesh representing the navigable areas of a Map.
 */
class celHNavStruct : public scfImplementation1<celHNavStruct, iCelHNavStruct>
{
private:
  csRef<iCelNavMeshParams> parameters;
  csHash<csRef<iCelNavMesh>, csPtrKey<iSector> > navMeshes;
  csRef<iCelGraph> hlGraph; // High level graph

public:
  celHNavStruct (const iCelNavMeshParams* params);
  virtual ~celHNavStruct ();

  void AddNavMesh(iCelNavMesh* navMesh);
  bool BuildHighLevelGraph();

  // API
  virtual iCelPath* ShortestPath (const csVector3& from, iSector* fromSector, const csVector3& goal,
      iSector* goalSector);
  virtual iCelPath* ShortestPath (iMapNode* from, iMapNode* goal);
  virtual bool SaveToFile (const csString& file);
  virtual bool LoadFromFile (const csString& file);
  virtual const iCelNavMeshParams* GetNavMeshParams () const;
  virtual void DebugRender ();
  virtual void DebugRenderAgent(const csVector3& pos, int red, int green, int blue, int alpha) const;
};



/**
 * Hierarchical navigation mesh creator.
 */
class celHNavStructBuilder : public scfImplementation2<celHNavStructBuilder, iCelHNavStructBuilder, iComponent>
{
private:
  csRef<iObjectRegistry> objectRegistry;
  csRef<iCelNavMeshParams> parameters;
  csList<iSector*>* sectors;
  csHash<csRef<iCelNavMeshBuilder>, csPtrKey<iSector> > builders;

  bool InstantiateNavMeshBuilders();

public:
  celHNavStructBuilder (iBase* parent);
  virtual ~celHNavStructBuilder ();
  virtual bool Initialize (iObjectRegistry*);

  // API
  virtual void SetSectors (csList<iSector*> sectorList);
  virtual iCelHNavStruct* BuildHNavStruct ();
  virtual bool UpdateNavMesh (iCelHNavStruct* hNavStruct, const csBox3& boundingBox, iSector* sector = 0);
  virtual bool UpdateNavMesh (iCelHNavStruct* hNavStruct, const csOBB& boundingBox, iSector* sector = 0);
  virtual const iCelNavMeshParams* GetNavMeshParams () const;
  virtual void SetNavMeshParams (const iCelNavMeshParams* parameters);
};

}
CS_PLUGIN_NAMESPACE_END(celNavMesh)

#endif // __CEL_HPF__
