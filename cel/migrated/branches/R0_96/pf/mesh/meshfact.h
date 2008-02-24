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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_MESHFACT__
#define __CEL_PF_MESHFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pl/facttmpl.h"
#include "pf/common/stdpcimp.h"
#include "pf/mesh.h"
#include "pf/meshsel.h"

struct iCelEntity;
struct iMeshWrapper;
struct iMeshFactoryWrapper;
struct iPcCamera;
struct iCamera;
class csVector3;

/**
 * Factory for mesh.
 */
CEL_DECLARE_FACTORY (Mesh)
CEL_DECLARE_FACTORY (MeshSelect)   

/**
 * This is a mesh property class.
 */
class celPcMesh : public celPcCommon
{
private:
  csRef<iMeshWrapper> mesh;
  bool visible;
  char* fileName;
  char* factName;
  csRef<iMeshFactoryWrapper> factory_ptr;

  iMeshFactoryWrapper* LoadMeshFactory ();

public:
  celPcMesh (iObjectRegistry* object_reg);
  virtual ~celPcMesh ();
  void Clear ();
  void SetMesh (const char* factname, const char* filename);
  void SetMesh (iMeshWrapper* mesh);
  void CreateEmptyThing ();
  iMeshWrapper* GetMesh () { return mesh; }
  void MoveMesh (iSector* sector, const csVector3& pos);
  void SetAction (const char* actionName, bool reset = false);
  const char* GetAction ();
  void SetReverseAction (bool reverse = true);
  void Hide ();
  void Show ();
  bool IsVisible () const { return visible; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmesh"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcMesh : public iPcMesh
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMesh);
    virtual void SetMesh (const char* factname, const char* filename)
    {
      scfParent->SetMesh (factname, filename);
    }
    virtual void SetMesh (iMeshWrapper* mesh)
    {
      scfParent->SetMesh (mesh);
    }
    virtual void CreateEmptyThing ()
    {
      scfParent->CreateEmptyThing ();
    }
    virtual iMeshWrapper* GetMesh () const
    {
      return scfParent->GetMesh ();
    }
    virtual void MoveMesh (iSector* sector, const csVector3& pos)
    {
      scfParent->MoveMesh (sector, pos);
    }
    virtual void SetAction (const char* actionName, bool reset = false)
    {
      scfParent->SetAction (actionName, reset);
    }
    virtual const char* GetAction ()
    {
      return scfParent->GetAction ();
    }
    virtual void SetReverseAction (bool reverse = true)
    {
      scfParent->SetReverseAction (reverse);
    }
    void Hide ()
    {
      scfParent->Hide ();
    }
    void Show ()
    {
      scfParent->Show ();
    }
    bool IsVisible () const
    {
      return scfParent->IsVisible ();
    }
  } scfiPcMesh;
};

/**
 * This is a mesh selection property class.
 */
class celPcMeshSelect : public celPcCommon
{
private:
  csRef<iPcCamera> pccamera;

  // If the following var is non-NULL then we
  // are busy selecting a mesh and are waiting for a mouse up
  // to arrive.
  iCelEntity* sel_entity;

  // Set to true if we are currently on top of the selected entity
  // (in do_follow mode).
  bool cur_on_top;

  // On which mouse buttons do we act (flag combination of CEL_MOUSE_...).
  int mouse_buttons;

  // If true then selections on ALL objects will arrive
  // at the attached behaviour. Otherwise only selections
  // on the attached entity will come through.
  bool do_global;

  // If true then it is possible to drag the selected object.
  bool do_drag;

  // Normal of the plane that is used to constrain dragging.
  csVector3 drag_normal;
  // If drag_normal is given in camera or world space coordinates.
  bool drag_normal_camera;

  // If true then follow the mouse so that up/down events
  // come through immediatelly when the mouse leaves the object
  // (even if mouse up has not yet happened). Otherwise mouse
  // up event will arrive only when the mouse is released
  // independent of wether or not the mouse is above the
  // object at release time.
  bool do_follow;

  // If true then additionally also send move messages even
  // if the object is not selected (but the mouse is over
  // an entity).
  bool do_follow_always;

  // If true then send mouse-move events.
  bool do_sendmove;

  // If true then send mouse-up events.
  bool do_sendup;

  // If true then send mouse-down events.
  bool do_senddown;

  // Setup the event handler based on settings.
  void SetupEventHandler ();

  // Data for the event handlers in behaviour layer.
  class PcMeshSelectData : public iPcMeshSelectData
  {
  private:
    int mouse_x, mouse_y, mouse_button;
    iCelEntity* entity;

  public:
    PcMeshSelectData () { SCF_CONSTRUCT_IBASE (NULL); }
    virtual ~PcMeshSelectData () { }

    void Select (iCelEntity* entity, int mouse_x, int mouse_y,
      int mouse_button)
    {
      PcMeshSelectData::entity = entity;
      PcMeshSelectData::mouse_x = mouse_x;
      PcMeshSelectData::mouse_y = mouse_y;
      PcMeshSelectData::mouse_button = mouse_button;
    }

    SCF_DECLARE_IBASE;
    virtual int GetMouseButton () const
    {
      return mouse_button;
    }
    virtual void GetMousePosition (int& x, int& y) const
    {
      x = mouse_x;
      y = mouse_y;
    }
    virtual iCelEntity* GetEntity () const { return entity; }
  } mesh_sel_data;
  void SendMessage (const char* name, iCelEntity* ent, int x, int y, int but);

public:
  celPcMeshSelect (iObjectRegistry* object_reg);
  virtual ~celPcMeshSelect ();

  bool HandleEvent (iEvent& ev);
  void SetCamera (iPcCamera* pccamera);

  void SetMouseButtons (int buttons) { mouse_buttons = buttons; }
  int GetMouseButtons () const { return mouse_buttons; }

  void SetGlobalSelection (bool glob) { do_global = glob; }
  bool HasGlobalSelection () const { return do_global; }
  void SetFollowMode (bool follow)
  {
    do_follow = follow;
    SetupEventHandler ();
  }
  bool HasFollowMode () const { return do_follow; }
  void SetFollowAlwaysMode (bool followalways) { do_follow_always = followalways; }
  bool HasFollowAlwaysMode () const { return do_follow_always; }
  void SetDragMode (bool drag)
  {
    do_drag = drag;
    SetupEventHandler ();
  }
  bool HasDragMode () const { return do_drag; }
  void SetDragPlaneNormal (const csVector3& drag_normal, bool camera_space)
  {
    celPcMeshSelect::drag_normal = drag_normal;
    drag_normal_camera = camera_space;
  }
  void GetDragPlaneNormal (csVector3& drag_normal, bool& camera_space) const
  {
    drag_normal = celPcMeshSelect::drag_normal;
    camera_space = drag_normal_camera;
  }
  void SetSendmoveEvent (bool mov)
  {
    do_sendmove = mov;
    SetupEventHandler ();
  }
  bool HasSendmoveEvent () const { return do_sendmove; }
  void SetSendupEvent (bool su) { do_sendup = su; }
  bool HasSendupEvent () const { return do_sendup; }
  void SetSenddownEvent (bool sd) { do_senddown = sd; }
  bool HasSenddownEvent () const { return do_senddown; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmeshselect"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcMeshSelect : public iPcMeshSelect
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMeshSelect);
    virtual void SetCamera (iPcCamera* pccamera)
    {
      scfParent->SetCamera (pccamera);
    }
    virtual void SetMouseButtons (int buttons)
    {
      scfParent->SetMouseButtons (buttons);
    }
    virtual int GetMouseButtons () const
    {
      return scfParent->GetMouseButtons ();
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
    virtual void SetFollowAlwaysMode (bool followalways)
    {
      scfParent->SetFollowAlwaysMode (followalways);
    }
    virtual bool HasFollowAlwaysMode () const
    {
      return scfParent->HasFollowAlwaysMode ();
    }
    virtual void SetDragMode (bool drag)
    {
      scfParent->SetDragMode (drag);
    }
    virtual bool HasDragMode () const
    {
      return scfParent->HasDragMode ();
    }
    virtual void SetDragPlaneNormal (const csVector3& drag_normal,
    	bool camera_space)
    {
      scfParent->SetDragPlaneNormal (drag_normal, camera_space);
    }
    virtual void GetDragPlaneNormal (csVector3& drag_normal,
    	bool& camera_space) const
    {
      scfParent->GetDragPlaneNormal (drag_normal, camera_space);
    }
    virtual void SetSendmoveEvent (bool mov)
    {
      scfParent->SetSendmoveEvent (mov);
    }
    virtual bool HasSendmoveEvent () const
    {
      return scfParent->HasSendmoveEvent ();
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

  // This is not an embedded interface in order to avoid
  // a circular reference between this registered event handler
  // and the parent object.
  class EventHandler : public iEventHandler
  {
  private:
    celPcMeshSelect* parent;
  public:
    EventHandler (celPcMeshSelect* parent)
    {
      SCF_CONSTRUCT_IBASE (NULL);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler () { }
    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
};

#endif // __CEL_PF_MESHFACT__
