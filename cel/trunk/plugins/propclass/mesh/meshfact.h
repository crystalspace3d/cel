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
#include "iutil/csinput.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/mesh.h"
#include "propclass/meshsel.h"

struct iCelEntity;
struct iMeshWrapper;
struct iMeshFactoryWrapper;
struct iPcCamera;
struct iCamera;
struct iEngine;
class csVector3;

/**
 * Factory for mesh.
 */
CEL_DECLARE_FACTORY (Mesh)
CEL_DECLARE_FACTORY (MeshSelect)   

/**
 * This is a mesh property class.
 */
class celPcMesh : public scfImplementationExt1<
	celPcMesh, celPcCommon, iPcMesh>
{
private:
  csRef<iMeshWrapper> mesh;
  csWeakRef<iEngine> engine;
  bool visible;
  csString path;
  csString fileName;
  csString factName;
  csRef<iMeshFactoryWrapper> factory_ptr;

  enum celPcMeshCreationFlag
  {
    CEL_CREATE_NONE = 0,
    CEL_CREATE_FACTORY,
    CEL_CREATE_MESH,
    CEL_CREATE_MESHREMOVE,
    CEL_CREATE_THING,
    CEL_CREATE_GENMESH
  };

  // This flag indicates how the mesh was created.
  celPcMeshCreationFlag creation_flag;

  iMeshFactoryWrapper* LoadMeshFactory ();

  static csStringID id_name;
  static csStringID id_path;
  static csStringID id_filename;
  static csStringID id_factoryname;
  static csStringID id_sector;
  static csStringID id_position;
  static csStringID id_rotation;
  static csStringID id_forward;
  static csStringID id_up;
  static csStringID id_visible;
  static csStringID id_material;
  static csStringID id_value;
  static csStringID id_type;
  static csStringID id_animation;
  static csStringID id_cycle;

  enum actionids
  {
    action_setmesh = 0,
    action_loadmesh,
    action_loadmeshpath,
    action_movemesh,
    action_rotatemesh,
    action_clearrotation,
    action_lookat,
    action_setvisible,
    action_setmaterial,
    action_setshadervar,
    action_setanimation,
    action_createemptything,
    action_createemptygenmesh
  };

  // Remove the mesh from this pcmesh.
  void RemoveMesh ();

  // For properties.
  enum propids
  {
    propid_position = 0,
    propid_fullposition,
    propid_sector,
    propid_path,
    propid_factory,
    propid_filename
  };
  static PropertyHolder propinfo;

public:
  celPcMesh (iObjectRegistry* object_reg);
  virtual ~celPcMesh ();
  /**
   * Unattaches this entity from the physical layer and removes this
   * mesh from the engine.
   */
  void Clear ();
  /// Set path.
  virtual void SetPath (const char* path);
  /**
   * Assign a mesh created from factname and/or loaded from filename
   * to this pcmesh.
   */
  virtual bool SetMesh (const char* factname, const char* filename);
  /**
   * Assign a mesh from a meshwrapper to this pcmesh.
   */
  virtual void SetMesh (iMeshWrapper* mesh, bool do_remove);
  /**
   * Creates an empty thing mesh for this pcmesh-entity.
   */
  virtual void CreateEmptyThing ( const char* factname);
  /**
   * Creates an empty genmesh mesh for this pcmesh-entity.
   */
  virtual void CreateEmptyGenmesh (const char* factname);
  /**
   * Get the mesh of this pcmesh-entity.
   */
  virtual iMeshWrapper* GetMesh () const { return mesh; }
  /**
   * Move this mesh to the according sector and position.
   */
  virtual void MoveMesh (iSector* sector, const csVector3& pos);
  /**
   * Sets an action for this mesh if different from the current action,
   * or resetaction is set.
   */
  virtual void SetAction (const char* actionName, bool reset = false);
  virtual void SetAnimation (const char* actionName, bool cycle,
      float weight=1.0,float fadein=0.1,float fadeout=0.1);

  /**
   * Returns the name for the current action.
   */
  virtual const char* GetAction ();
  /**
   * Reverse the currently set action.
   */
  virtual void SetReverseAction (bool reverse = true);
  /**
   * Hides this entity.
   */
  virtual void Hide ();
  /**
   * Shows this entity.
   */
  virtual void Show ();
  /**
   * Returns visibility status for this entity.
   */
  virtual bool IsVisible () const { return visible; }

  virtual const char* GetName () const { return "pcmesh"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
  virtual bool GetPropertyIndexed (int, csVector3&);
  virtual bool GetPropertyIndexed (int, const char*&);

  // Functions to set ShaderVars in the mesh
  virtual void SetShaderVar(csStringID name, float value);
  virtual void SetShaderVar(csStringID name, int value);
  virtual void SetShaderVar(csStringID name, csVector3 value);
  virtual void SetShaderVar(csStringID name, csVector2 value);
};

class celPcMeshSelect;
typedef csSet<csPtrKey<celPcMeshSelect> > celMeshSelectSet;

/**
 * This is a global event handler for all mesh selectors.
 * This is an optimization because registering many listeners
 * is not very fast in CS.
 */
class celMeshSelectListener : public scfImplementation2<
			      celMeshSelectListener,
			      scfFakeInterface<celMeshSelectListener>,
			      iEventHandler>
{
private:
  celMeshSelectSet listeners;
  celMeshSelectSet listeners_with_move;
  bool is_iterating;
  celMeshSelectSet todo_add_listeners;
  celMeshSelectSet todo_add_listeners_with_move;
  celMeshSelectSet todo_rem_listeners;
  iEventNameRegistry* name_reg;

public:
  SCF_INTERFACE (celMeshSelectListener, 0, 0, 1);

  celMeshSelectListener (iEventNameRegistry* name_reg)
    : scfImplementationType (this), is_iterating (false),
      name_reg (name_reg) { }
  virtual ~celMeshSelectListener () { }
  virtual bool HandleEvent (iEvent& ev);
  void RegisterMeshSelect (celPcMeshSelect* meshsel, bool withmove);
  void UnregisterMeshSelect (celPcMeshSelect* meshsel);

  CS_EVENTHANDLER_NAMES("cel.propclass.pcmeshselect")
  CS_EVENTHANDLER_NIL_CONSTRAINTS
};

/**
 * This is a mesh selection property class.
 */
class celPcMeshSelect : public scfImplementationExt1<
	celPcMeshSelect, celPcCommon, iPcMeshSelect>
{
private:
  csWeakRef<iPcCamera> pccamera;

  csRef<iMouseDriver> mousedrv;
  csRef<iEventNameRegistry> name_reg;

  /// Listeners.
  csRefArray<iPcMeshSelectListener> listeners;

  // If the following var is non-0 then we
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

  // Drag offset: this is the offset between the selection point
  // and the origin of the object.
  csVector3 drag_offset;

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
  
  // The maximum distance to perform mesh selection over.
  float max_distance;

  // Setup the event handler based on settings.
  void SetupEventHandler ();

  static csStringID id_buttons;
  static csStringID id_normal;
  static csStringID id_camera;

  enum actionids
  {
    action_setcamera = 0,
    action_setmousebuttons,
    action_setdragplanenormal
  };

  // For properties.
  enum propids
  {
    propid_global = 0,
    propid_follow,
    propid_followalways,
    propid_drag,
    propid_sendmove,
    propid_sendup,
    propid_senddown,
    propid_maxdistance
  };
  static PropertyHolder propinfo;

  static csStringID id_x, id_y, id_button, id_entity;
  celGenericParameterBlock* params;
#define MSSM_TYPE_DOWN 0
#define MSSM_TYPE_UP 1
#define MSSM_TYPE_MOVE 2
  void SendMessage (int t, iCelEntity* ent, int x, int y, int but);

  csRef<celMeshSelectListener> handler;

public:
  celPcMeshSelect (iObjectRegistry* object_reg);
  virtual ~celPcMeshSelect ();

  bool HandleEvent (iEvent& ev);
  virtual void SetCamera (iPcCamera* pccamera);

  virtual void SetMouseButtons (int buttons) { mouse_buttons = buttons; }
  virtual void SetMouseButtons (const char* buttons);
  virtual int GetMouseButtons () const { return mouse_buttons; }

  virtual void SetGlobalSelection (bool glob) { do_global = glob; }
  virtual bool HasGlobalSelection () const { return do_global; }
  virtual void SetFollowMode (bool follow)
  {
    do_follow = follow;
    SetupEventHandler ();
  }
  virtual bool HasFollowMode () const { return do_follow; }
  virtual void SetFollowAlwaysMode (bool followalways)
  { do_follow_always = followalways; }
  virtual bool HasFollowAlwaysMode () const { return do_follow_always; }
  virtual void SetDragMode (bool drag)
  {
    do_drag = drag;
    SetupEventHandler ();
  }
  virtual bool HasDragMode () const { return do_drag; }
  virtual void SetDragPlaneNormal (const csVector3& drag_normal,
      bool camera_space)
  {
    celPcMeshSelect::drag_normal = drag_normal;
    drag_normal_camera = camera_space;
  }
  virtual void GetDragPlaneNormal (csVector3& drag_normal,
  	bool& camera_space) const
  {
    drag_normal = celPcMeshSelect::drag_normal;
    camera_space = drag_normal_camera;
  }
  virtual void SetSendmoveEvent (bool mov)
  {
    do_sendmove = mov;
    SetupEventHandler ();
  }
  virtual bool HasSendmoveEvent () const { return do_sendmove; }
  virtual void SetSendupEvent (bool su) { do_sendup = su; }
  virtual bool HasSendupEvent () const { return do_sendup; }
  virtual void SetSenddownEvent (bool sd) { do_senddown = sd; }
  virtual bool HasSenddownEvent () const { return do_senddown; }
  virtual void SetMaxSelectionDistance (float distance)
  { max_distance = distance; }
  virtual float GetMaxSelectionDistance () const { return max_distance; }

  virtual void AddMeshSelectListener (iPcMeshSelectListener* listener);
  virtual void RemoveMeshSelectListener (iPcMeshSelectListener* listener);
  void FireListenersDown (int x, int y, int button, iCelEntity* entity);
  void FireListenersUp (int x, int y, int button, iCelEntity* entity);
  void FireListenersMove (int x, int y, int button, iCelEntity* entity);

  virtual const char* GetName () const { return "pcmeshselect"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

  // Override SetProperty from celPcCommon in order to provide support
  // for the 'bool' properties.
  virtual bool SetPropertyIndexed (int, bool);
  virtual bool GetPropertyIndexed (int, bool&);
};

#endif // __CEL_PF_MESHFACT__

