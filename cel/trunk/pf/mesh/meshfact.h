/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PF_MESHFACT__
#define __CEL_PF_MESHFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pf/mesh.h"
#include "pf/meshsel.h"

struct iCelEntity;
struct iMeshWrapper;
struct iMeshFactoryWrapper;
struct iCamera;

/**
 * Factory for mesh.
 */
class celPfMesh : public iCelPropertyClassFactory
{
private:
  iObjectRegistry* object_reg;

public:
  celPfMesh (iBase* parent);
  virtual ~celPfMesh ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pfmesh"; }
  virtual iCelPropertyClass* CreatePropertyClass (const char* type);
  virtual int GetTypeCount () const { return 2; }
  virtual const char* GetTypeName (int idx) const;

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPfMesh);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

/**
 * This is a mesh property class.
 */
class celPcMesh : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iMeshWrapper* mesh;
  iObjectRegistry* object_reg;

  iMeshFactoryWrapper* LoadMeshFactory (const char* fileName);

public:
  celPcMesh (iObjectRegistry* object_reg);
  virtual ~celPcMesh ();
  void SetMesh (const char* factname, const char* filename);
  void MoveMesh (iSector* sector, const csVector3& pos);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pcmesh"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);

  struct PcMesh : public iPcMesh
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMesh);
    virtual void SetMesh (const char* factname, const char* filename)
    {
      scfParent->SetMesh (factname, filename);
    }
    virtual iMeshWrapper* GetMesh () const
    {
      return scfParent->mesh;
    }
    virtual void MoveMesh (iSector* sector, const csVector3& pos)
    {
      scfParent->MoveMesh (sector, pos);
    }
  } scfiPcMesh;
};

/**
 * This is a mesh selection property class.
 */
class celPcMeshSelect : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iObjectRegistry* object_reg;
  iCamera* camera;

public:
  celPcMeshSelect (iObjectRegistry* object_reg);
  virtual ~celPcMeshSelect ();

  bool HandleEvent (iEvent& ev);
  void SetCamera (iCamera* camera);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pcmeshselect"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);

  struct PcMeshSelect : public iPcMeshSelect
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMeshSelect);
    virtual void SetCamera (iCamera* camera)
    {
      scfParent->SetCamera (camera);
    }
  } scfiPcMeshSelect;
  struct EventHandler : public iEventHandler
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMeshSelect);
    virtual bool HandleEvent (iEvent& ev)
    {
      return scfParent->HandleEvent (ev);
    }
  } scfiEventHandler;
};

#endif // __CEL_PF_MESHFACT__

