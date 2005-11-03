/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_MECHANICS_SYSTEM_FACTORY__
#define __CEL_PF_MECHANICS_SYSTEM_FACTORY__

#include "cstypes.h"
#include "csgeom/transfrm.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/refarr.h"
#include "csutil/parray.h"

#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/mechsys.h"
#include "propclass/mesh.h"
#include "propclass/light.h"
#include "propclass/camera.h"
#include "ivaria/dynamics.h"

struct iVirtualClock;
struct iCelPlLayer;
struct iCelEntity;
struct iCelEntityList;
class csVector3;
class csReversibleTransform;
class csColliderWrapper;

/**
 * Factory for mechsys.
 */
CEL_DECLARE_FACTORY (MechanicsSystem)
CEL_DECLARE_FACTORY (MechanicsObject)

struct celForce
{
  iPcMechanicsObject* body;
  float seconds;		// Remaining time. Not used if for entire frame.
  bool frame;			// True if processing is for entire frame.
  uint32 forceid;		// The string ID for the tag of this force.
  bool relative;		// True if force and position are relative to the object.
  csVector3 force;		// Defines the magnitude and direction of the force.
  csVector3 position;		// The position of the force.
};

/**
 * The mechanics system. This takes care of coordinating all the objects in the
 * system, applying forces and handling collisions.
 */
class celPcMechanicsSystem : public scfImplementationExt1<
	celPcMechanicsSystem, celPcCommon, iPcMechanicsSystem>
{
private:
  // Actions
  static csStringID action_setsystem;
  static csStringID action_setgravity;
  static csStringID action_quickstep;
  static csStringID action_enablestepfast;
  static csStringID action_disablestepfast;
  static csStringID action_setsteptime;

  // Parameters for action_setsystem (mechsystem)
  static csStringID param_dynsys;

  // Parameters for action_setgravity
  static csStringID param_gravity;

  // Parameters for action_setsteptime
  static csStringID param_time;
 
  csRef<iDynamics> dynamics;
  csRef<iDynamicSystem> dynsystem;
  bool dynsystem_error_reported;
  csRef<iVirtualClock> vc;
  csArray<celForce> forces;
  csHash<iBodyGroup*, const char*> groups;
  float delta;
  float remaining_delta;

  void ProcessForces (float dt);
  iDynamics *GetDynamics ();

public:
  celPcMechanicsSystem (iObjectRegistry* object_reg);
  virtual ~celPcMechanicsSystem ();

  virtual void SetDynamicSystem (iDynamicSystem* dynsys);
  virtual void SetDynamicSystem (const char *dynsys);
  virtual iDynamicSystem* GetDynamicSystem ();
  virtual void SetStepTime (float delta)
  {
    celPcMechanicsSystem::delta = delta;
  }
  virtual float GetStepTime () const { return delta; }

  virtual void SetGravity (const csVector3& grav);
  virtual const csVector3 GetGravity ();

  virtual void AddForceTagged (iPcMechanicsObject* body, const csVector3& force,
	bool relative, const csVector3& position, uint32 forceid);
  virtual void RemoveForceTagged (iPcMechanicsObject* body, uint32 forceid);
  virtual void AddForceDuration (iPcMechanicsObject* body, const csVector3& force,
	bool relative, const csVector3& position, float seconds);
  virtual void AddForceFrame (iPcMechanicsObject* body, const csVector3& force,
	bool relative, const csVector3& position);
  virtual void ClearForces (iPcMechanicsObject* body);
  virtual void ClearAllForces ();
  void ApplyForce (celForce& f);

  virtual csPtr<iRigidBody> CreateBody ();
  virtual void RemoveBody (iRigidBody* body);
  virtual void AddBodyToGroup (iRigidBody* body, const char* group);
  virtual void RemoveBodyFromGroup (iRigidBody* body, const char* group);
  virtual iJoint* CreateJoint (iRigidBody* body1, iRigidBody* body2);
  virtual void RemoveJoint (iJoint* joint);

  virtual void DisableStepFast ();
  virtual void EnableStepFast ();
  virtual void EnableQuickStep ();

  virtual const char* GetName () const { return "pcmechsys"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual void TickEveryFrame ();
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  // Made independent to avoid circular refs and leaks.
  struct CelTimerListener : public scfImplementation1<
  	CelTimerListener, iCelTimerListener>
  {
    celPcMechanicsSystem* parent;
    CelTimerListener (celPcMechanicsSystem* parent) :
    	scfImplementationType (this), parent (parent) { }
    virtual ~CelTimerListener () { }
    virtual void TickEveryFrame ()
    {
      parent->TickEveryFrame ();
    }
    virtual void TickOnce ()
    {
      return;
    }
  } * scfiCelTimerListener;
};

#define CEL_BODY_INVALID 0
#define CEL_BODY_SPHERE 1
#define CEL_BODY_BOX 2
#define CEL_BODY_CYLINDER 3
#define CEL_BODY_PLANE 4
#define CEL_BODY_MESH 5

struct body_data
{
  virtual ~body_data () { }
};

struct sphere_data : public body_data
{
  sphere_data (float radius, const csVector3& offset)
  {
    sphere_data::radius = radius;
    sphere_data::offset = offset;
  }
  float radius;
  csVector3 offset;
};

struct box_data : public body_data
{
  box_data (const csVector3& size, const csOrthoTransform& transform)
  {

    box_data::size = size;
    box_data::transform = transform;
  }
  csVector3 size;
  csOrthoTransform transform;
};

struct cylinder_data : public body_data
{
  cylinder_data (float length, float radius, const csOrthoTransform& transform)
  {
    cylinder_data::length = length;
    cylinder_data::radius = radius;
    cylinder_data::transform = transform;
  }
  float length;
  float radius;
  csOrthoTransform transform;
};

struct plane_data : public body_data
{
  plane_data (const csPlane3& plane)
  {
    plane_data::plane = plane;
  }
  csPlane3 plane;
};

/**
 * This is a dynamic body property class.
 */
class celPcMechanicsObject : public scfImplementationExt1<
	celPcMechanicsObject, celPcCommon, iPcMechanicsObject>
{
private:
  // Actions
  static csStringID action_initphys;
  static csStringID action_makestatic;
  static csStringID action_setsystem;
  static csStringID action_setmesh;
  static csStringID action_setcollidersphere;
  static csStringID action_setcollidercylinder;
  static csStringID action_setcolliderbox;
  static csStringID action_setcolliderplane;
  static csStringID action_setcollidermesh;
  static csStringID action_setlinearvelocity;
  static csStringID action_setangularvelocity;
  static csStringID action_addforceonce;
  static csStringID action_addforceduration;
  static csStringID action_addforceframe;
  static csStringID action_clearforces;

  // Parameters for action_initphys
  static csStringID param_friction;
  static csStringID param_mass;
  static csStringID param_elasticity;
  static csStringID param_density;
  static csStringID param_softness;
  static csStringID param_lift;
  static csStringID param_drag;

  // Parameters.
  static csStringID param_static;
  static csStringID param_systempcent;
  static csStringID param_systempctag;
  static csStringID param_meshpctag;
  static csStringID param_radius;
  static csStringID param_offset;
  static csStringID param_length;
  static csStringID param_axis;
  static csStringID param_angle;
  static csStringID param_size;
  static csStringID param_normal;
  static csStringID param_otherbody;
  static csStringID param_force;
  static csStringID param_relative;
  static csStringID param_position;
  static csStringID param_seconds;
  static csStringID param_velocity;

  celOneParameterBlock* params;

  // Last assigned force id for this object
  uint32 forceidseed;

  csWeakRef<iPcMechanicsSystem> mechsystem;
  csWeakRef<iPcMesh> pcmesh;
  csWeakRef<iPcLight> pclight;
  csWeakRef<iPcCamera> pccamera;
  csWeakRef<iRigidBody> body;
  int btype;
  body_data* bdata;

  bool is_static;

  float friction;
  float mass;
  float elasticity;
  float density;
  float softness;
  csVector3 lift;
  float drag;

  void GetMechSystem ();

  void FindMeshLightCamera ();
  csReversibleTransform GetFullTransform ();
  void AttachObject ();

public:
  celPcMechanicsObject (iObjectRegistry* object_reg);
  virtual ~celPcMechanicsObject ();

  virtual void SetMesh (iPcMesh* mesh)
  {
    pcmesh = mesh;
    pclight = 0;
    pccamera = 0;
  }
  virtual iPcMesh* GetMesh ();
  virtual void SetLight (iPcLight* light)
  {
    pcmesh = 0;
    pclight = light;
    pccamera = 0;
  }
  virtual iPcLight* GetLight ();
  virtual void SetCamera (iPcCamera* camera)
  {
    pcmesh = 0;
    pclight = 0;
    pccamera = camera;
  }
  virtual iPcCamera* GetCamera ();

  virtual void SetMechanicsSystem (iPcMechanicsSystem* dynsys)
  {
    mechsystem = dynsys;
  }

  virtual iPcMechanicsSystem* GetMechanicsSystem ()
  {
    return mechsystem;
  }

  virtual iRigidBody* GetBody ();

  virtual void SetFriction (float friction);

  virtual void SetMass (float mass);

  virtual void SetElasticity (float elasticity);

  virtual void SetDensity (float density);

  virtual void SetSoftness (float softness);

  virtual void SetLift (const csVector3& lift);

  virtual void SetDrag (float drag);

  virtual float GetFriction () { return friction; }
  virtual float GetMass () { return mass; } 
  virtual float GetElasticity () { return elasticity; }
  virtual float GetDensity () { return density; }
  virtual float GetSoftness () { return softness; }
  virtual const csVector3& GetLift () { return lift; }
  virtual float GetDrag () { return drag; }

  virtual void SetLinearVelocity (const csVector3& vel);
  virtual const csVector3 GetLinearVelocity ();
  virtual void SetAngularVelocity (const csVector3& vel);
  virtual const csVector3 GetAngularVelocity ();

  virtual void MakeStatic (bool stat);
  virtual bool IsStatic () const { return is_static; }

  virtual void AttachColliderSphere (float radius, const csVector3& offset);
  virtual void AttachColliderCylinder (float length, float radius,
  	const csOrthoTransform& trans);
  virtual void AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans);
  virtual void AttachColliderPlane (const csPlane3& plane);
  virtual void AttachColliderMesh ();

  virtual uint32 AddForceTagged (const csVector3& force, bool relative, 
	const csVector3& position);
  virtual void RemoveForceTagged (uint32 forceid);
  virtual void AddForceOnce (const csVector3& force, bool relative,
	const csVector3& position);
  virtual void AddForceDuration (const csVector3& force, bool relative,
	const csVector3& position, float seconds);
  virtual void AddForceFrame (const csVector3& force, bool relative,
	const csVector3& position);
  virtual void ClearForces ();

  virtual void AddToGroup (const char* group);
  virtual void RemoveFromGroup (const char* group);
  virtual iJoint* CreateJoint (iPcMechanicsObject* other);

  void Collision (iRigidBody *thisbody, iRigidBody *otherbody);

  virtual const char* GetName () const { return "pcmechobject"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  struct DynamicsCollisionCallback : public iDynamicsCollisionCallback
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMechanicsObject);
    virtual void Execute (iRigidBody *thisbody, iRigidBody *otherbody)
    {
      scfParent->Collision (thisbody, otherbody);
    }
  } scfiDynamicsCollisionCallback;
};

#endif // __CEL_PF_MECHANICS_SYSTEM_FACTORY__

