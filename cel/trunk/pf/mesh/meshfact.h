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
  void ClearMesh () { mesh = NULL; }
  void MoveMesh (iSector* sector, const csVector3& pos);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pcmesh"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual iMeshWrapper* GetMesh () { return mesh; }
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
      return scfParent->GetMesh ();
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

  // If the following var is non-NULL then we
  // are busy selecting a mesh and are waiting for a mouse up
  // to arrive.
  iCelEntity* sel_entity;

  // Set to true if we are currently on top of the selected entity
  // (in do_follow mode).
  bool cur_on_top;

  // If true then selections on ALL objects will arrive
  // at the attached behaviour. Otherwise only selections
  // on the attached entity will come through.
  bool do_global;

  // If true then it is possible to drag the selected object.
  bool do_drag;

  // If true then follow the mouse so that up/down events
  // come through immediatelly when the mouse leaves the object
  // (even if mouse up has not yet happened). Otherwise mouse
  // up event will arrive only when the mouse is released
  // independent of wether or not the mouse is above the
  // object at release time.
  bool do_follow;

  // If true then send mouse-up events.
  bool do_sendup;

  // If true then send mouse-down events.
  bool do_senddown;

public:
  celPcMeshSelect (iObjectRegistry* object_reg);
  virtual ~celPcMeshSelect ();

  bool HandleEvent (iEvent& ev);
  void SetCamera (iCamera* camera);

  void SetGlobalSelection (bool glob) { do_global = glob; }
  bool HasGlobalSelection () const { return do_global; }
  void SetFollowMode (bool follow) { do_follow = follow; };
  bool HasFollowMode () const { return do_follow; }
  void SetDragMode (bool drag) { do_drag = drag; }
  bool HasDragMode () const { return do_drag; }
  void SetSendupEvent (bool su) { do_sendup = su; }
  bool HasSendupEvent () const { return do_sendup; }
  void SetSenddownEvent (bool sd) { do_senddown = sd; }
  bool HasSenddownEvent () const { return do_senddown; }

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
    virtual void SetGlobalSelection (bool glob)
    {
      scfParent->SetGlobalSelection (glob);
    }
    virtual bool HasGlobalSelection () const
    {
      return scfParent->HasGlobalSelection ();
    }
    virtual void SetFollowMode (bool follow)
    {
      scfParent->SetFollowMode (follow);
    }
    virtual bool HasFollowMode () const
    {
      return scfParent->HasFollowMode ();
    }
    virtual void SetDragMode (bool drag)
    {
      scfParent->SetDragMode (drag);
    }
    virtual bool HasDragMode () const
    {
      return scfParent->HasDragMode ();
    }
    virtual void SetSendupEvent (bool su)
    {
      scfParent->SetSendupEvent (su);
    }
    virtual bool HasSendupEvent () const
    {
      return scfParent->HasSendupEvent ();
    }
    virtual void SetSenddownEvent (bool sd)
    {
      scfParent->SetSenddownEvent (sd);
    }
    virtual bool HasSenddownEvent () const
    {
      return scfParent->HasSenddownEvent ();
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

