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
class celPcMechanicsSystem : public celPcCommon
{
private:
  // Actions
  static csStringID action_setsystem;
  static csStringID action_setgravity;

  // Parameters for action_setsystem (mechsystem)
  static csStringID param_dynsys;

  // Parameters for action_setgravity
  static csStringID param_gravity;
 
  csRef<iDynamics> dynamics;
  csRef<iDynamicSystem> dynsystem;
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

  void SetDynamicSystem (iDynamicSystem* dynsys);
  void SetDynamicSystem (const char *dynsys);
  iDynamicSystem* GetDynamicSystem ();
  void SetStepTime (float delta)
  {
    celPcMechanicsSystem::delta = delta;
  }
  float GetStepTime () const { return delta; }

  void SetGravity (const csVector3& grav);
  const csVector3 GetGravity ();

  void AddForceTagged (iPcMechanicsObject* body, const csVector3& force,
	bool relative, const csVector3& position, uint32 forceid);
  void RemoveForceTagged (iPcMechanicsObject* body, uint32 forceid);
  void AddForceDuration (iPcMechanicsObject* body, const csVector3& force,
	bool relative, const csVector3& position, float seconds);
  void AddForceFrame (iPcMechanicsObject* body, const csVector3& force,
	bool relative, const csVector3& position);
  void ClearForces (iPcMechanicsObject* body);
  void ClearAllForces ();
  void ApplyForce (celForce& f);

  csPtr<iRigidBody> CreateBody ();
  void RemoveBody (iRigidBody* body);
  void AddBodyToGroup (iRigidBody* body, const char* group);
  void RemoveBodyFromGroup (iRigidBody* body, const char* group);
  iJoint* CreateJoint (iRigidBody* body1, iRigidBody* body2);
  void RemoveJoint (iJoint* joint);

  void DisableStepFast ();
  void EnableStepFast ();
  void EnableQuickStep ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmechsys"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual void TickEveryFrame ();
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  struct PcMechanicsSystem : public iPcMechanicsSystem
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMechanicsSystem);
    virtual void SetDynamicSystem (iDynamicSystem* dynsys)
    {
      scfParent->SetDynamicSystem (dynsys);
    }
    virtual void SetDynamicSystem (const char *dynsysname)
    {
      scfParent->SetDynamicSystem (dynsysname);
    }
    virtual iDynamicSystem* GetDynamicSystem ()
    {
      return scfParent->GetDynamicSystem ();
    }
    virtual void SetStepTime (float delta)
    {
      scfParent->SetStepTime (delta);
    }
    virtual float GetStepTime () const
    {
      return scfParent->GetStepTime ();
    }
    virtual void SetGravity (const csVector3& grav)
    {
      scfParent->SetGravity (grav);
    }
    virtual const csVector3 GetGravity ()
    {
      return scfParent->GetGravity ();
    }
    virtual void AddForceTagged (iPcMechanicsObject* pcobject,
	const csVector3& force, bool relative, const csVector3& position,
	uint32 forceid)
    {
      scfParent->AddForceTagged (pcobject, force, relative, position, forceid);
    }
    virtual void RemoveForceTagged (iPcMechanicsObject* pcobject,
	uint32 forceid)
    {
      scfParent->RemoveForceTagged (pcobject, forceid);
    }
    virtual void AddForceDuration (iPcMechanicsObject* body,
  	const csVector3& force, bool relative, const csVector3& position,
	float seconds)
    {
      scfParent->AddForceDuration (body, force, relative, position, seconds);
    }
    virtual void AddForceFrame (iPcMechanicsObject* body,
	const csVector3& force, bool relative, const csVector3& position)
    {
      scfParent->AddForceFrame (body, force, relative, position);
    }
    virtual void ClearForces (iPcMechanicsObject* body)
    {
      scfParent->ClearForces (body);
    }
    virtual void ClearAllForces ()
    {
      scfParent->ClearAllForces ();
    }
    virtual csPtr<iRigidBody> CreateBody ()
    {
      return scfParent->CreateBody ();
    }
    virtual void RemoveBody (iRigidBody* body)
    {
      scfParent->RemoveBody (body);
    }
    virtual void AddBodyToGroup (iRigidBody* body, const char* group)
    {
      scfParent->AddBodyToGroup (body, group);
    }
    virtual void RemoveBodyFromGroup (iRigidBody* body, const char* group)
    {
      scfParent->RemoveBodyFromGroup (body, group);
    }
    virtual iJoint* CreateJoint (iRigidBody* body1, iRigidBody* body2)
    {
      return scfParent->CreateJoint (body1, body2);
    }
    virtual void RemoveJoint (iJoint* joint)
    {
      scfParent->RemoveJoint (joint);
    }
    virtual void DisableStepFast ()
    {
      scfParent->DisableStepFast ();
    }
    virtual void EnableStepFast ()
    {
      scfParent->EnableStepFast ();
    }
    virtual void EnableQuickStep ()
    {
      scfParent->EnableQuickStep ();
    }
  } scfiPcMechanicsSystem;

  struct CelTimerListener : public iCelTimerListener
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMechanicsSystem);
    virtual void TickEveryFrame ()
    {
      scfParent->TickEveryFrame ();
    }
    virtual void TickOnce ()
    {
      return;
    }
  } scfiCelTimerListener;
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
class celPcMechanicsObject : public celPcCommon
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

  // Parameters for action_initphys
  static csStringID param_friction;
  static csStringID param_mass;
  static csStringID param_elasticity;
  static csStringID param_density;
  static csStringID param_softness;
  static csStringID param_lift;
  static csStringID param_drag;

  // Parameters for action_makestatic
  static csStringID param_static;

  // Parameters for action_setsystem
  static csStringID param_systempcent;
  static csStringID param_systempctag;

  // Parameters for action_setmesh
  static csStringID param_meshpctag;

  // Parameters for action_setcollidersphere
  static csStringID param_radius;
  static csStringID param_offset;

  // Parameters for action_setcollidercylinder
  static csStringID param_length;
  //param_radius shared
  static csStringID param_axis;
  static csStringID param_angle;
  //param_offset shared

  // Parameters for action_setcolliderbox
  static csStringID param_size;
  //param_axis shared
  //param_angle shared
  //param_offset shared

  // Parameters for action_setcolliderplane
  static csStringID param_normal;
  //param_offset shared

  // Parameters for message pcdynamicbody_collision
  static csStringID param_otherbody;
  celOneParameterBlock* params;

  // Last assigned force id for this object
  uint32 forceidseed;

  csWeakRef<iPcMechanicsSystem> mechsystem;
  csWeakRef<iPcMesh> pcmesh;
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

public:
  celPcMechanicsObject (iObjectRegistry* object_reg);
  virtual ~celPcMechanicsObject ();

  void SetMesh (iPcMesh* mesh)
  {
    pcmesh = mesh;
  }
  iPcMesh* GetMesh ();

  void SetMechanicsSystem (iPcMechanicsSystem* dynsys)
  {
    mechsystem = dynsys;
  }

  iPcMechanicsSystem* GetMechanicsSystem ()
  {
    return mechsystem;
  }

  iRigidBody* GetBody ();

  void SetFriction (float friction);

  void SetMass (float mass);

  void SetElasticity (float elasticity);

  void SetDensity (float density);

  void SetSoftness (float softness);

  void SetLift (const csVector3& lift);

  void SetDrag (float drag);

  virtual float GetFriction () {
    return celPcMechanicsObject::friction;
  }

  virtual float GetMass () {
    return celPcMechanicsObject::mass;
  }

  virtual float GetElasticity () {
    return celPcMechanicsObject::elasticity;
  }

  virtual float GetDensity () {
    return celPcMechanicsObject::density;
  }

  virtual float GetSoftness () {
    return celPcMechanicsObject::softness;
  }

  virtual const csVector3& GetLift () {
    return celPcMechanicsObject::lift;
  }

  virtual float GetDrag () {
    return celPcMechanicsObject::drag;
  }

  void SetLinearVelocity (const csVector3& vel);
  const csVector3 GetLinearVelocity ();
  void SetAngularVelocity (const csVector3& vel);
  const csVector3 GetAngularVelocity ();

  void MakeStatic (bool stat);
  bool IsStatic () const { return is_static; }

  void AttachColliderSphere (float radius, const csVector3& offset);
  void AttachColliderCylinder (float length, float radius,
  	const csOrthoTransform& trans);
  void AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans);
  void AttachColliderPlane (const csPlane3& plane);
  void AttachColliderMesh ();

  void AddForceTagged (const csVector3& force, bool relative,
	const csVector3& position, uint32& forceid);
  void RemoveForceTagged (uint32 forceid);
  void AddForceOnce (const csVector3& force, bool relative,
	const csVector3& position);
  void AddForceDuration (const csVector3& force, bool relative,
	const csVector3& position, float seconds);
  void AddForceFrame (const csVector3& force, bool relative,
	const csVector3& position);
  void ClearForces ();

  void AddToGroup (const char* group);
  void RemoveFromGroup (const char* group);
  iJoint* CreateJoint (iPcMechanicsObject* other);

  void Collision (iRigidBody *thisbody, iRigidBody *otherbody);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmechobject"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  struct PcMechanicsObject : public iPcMechanicsObject
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMechanicsObject);
    virtual void SetMesh (iPcMesh* mesh)
    {
      scfParent->SetMesh (mesh);
    }
    virtual iPcMesh* GetMesh ()
    {
      return scfParent->GetMesh ();
    }
    virtual void SetMechanicsSystem (iPcMechanicsSystem* dynsys)
    {
      scfParent->SetMechanicsSystem (dynsys);
    }
    virtual iPcMechanicsSystem* GetMechanicsSystem ()
    {
      return scfParent->GetMechanicsSystem ();
    }
    virtual iRigidBody* GetBody ()
    {
      return scfParent->GetBody ();
    }
    virtual void SetFriction (float friction) {
      scfParent->SetFriction (friction);
    }
    virtual void SetMass (float mass) {
      scfParent->SetMass (mass);
    }
    virtual void SetElasticity (float elasticity) {
      scfParent->SetElasticity (elasticity);
    }
    virtual void SetDensity (float density) {
      scfParent->SetDensity (density);
    }
    virtual void SetSoftness (float softness) {
      scfParent->SetSoftness (softness);
    }
    virtual void SetLift (const csVector3& lift) {
      scfParent->SetLift (lift);
    }
    virtual void SetDrag (float drag) {
      scfParent->SetDrag (drag);
    }
    virtual float GetFriction () {
      return scfParent->GetFriction();
    }
    virtual float GetMass () {
      return scfParent->GetMass();
    }
    virtual float GetElasticity () {
      return scfParent->GetElasticity();
    }
    virtual float GetDensity () {
      return scfParent->GetDensity();
    }
    virtual float GetSoftness () {
      return scfParent->GetSoftness();
    }
    virtual const csVector3& GetLift () {
      return scfParent->GetLift();
    }
    virtual float GetDrag () {
      return scfParent->GetDrag();
    }
    virtual void SetLinearVelocity (const csVector3& vel) {
      scfParent->SetLinearVelocity (vel);
    }
    virtual void SetAngularVelocity (const csVector3& vel) {
      scfParent->SetAngularVelocity (vel);
    }
    virtual const csVector3 GetLinearVelocity () {
      return scfParent->GetLinearVelocity ();
    }
    virtual const csVector3 GetAngularVelocity () {
      return scfParent->GetAngularVelocity ();
    }
    virtual void MakeStatic (bool stat)
    {
      scfParent->MakeStatic (stat);
    }
    virtual bool IsStatic () const
    {
      return scfParent->IsStatic ();
    }
    virtual void AttachColliderSphere (float radius, const csVector3& offset)
    {
      scfParent->AttachColliderSphere (radius, offset);
    }
    virtual void AttachColliderCylinder (float length, float radius,
    	const csOrthoTransform& trans)
    {
      scfParent->AttachColliderCylinder (length, radius, trans);
    }
    virtual void AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans)
    {
      scfParent->AttachColliderBox (size, trans);
    }
    virtual void AttachColliderPlane (const csPlane3& plane)
    {
      scfParent->AttachColliderPlane (plane);
    }
    virtual void AttachColliderMesh ()
    {
      scfParent->AttachColliderMesh ();
    }
    virtual void AddForceTagged (const csVector3& force, bool relative,
	const csVector3& position, uint32& forceid)
    {
      scfParent->AddForceTagged (force, relative, position, forceid);
    }
    virtual void RemoveForceTagged (uint32 forceid)
    {
      scfParent->RemoveForceTagged (forceid);
    }
    virtual void AddForceOnce (const csVector3& force, bool relative,
	const csVector3& position)
    {
      scfParent->AddForceOnce (force, relative, position);
    }
    virtual void AddForceDuration (const csVector3& force, bool relative,
	const csVector3& position, float seconds)
    {
      scfParent->AddForceDuration (force, relative, position, seconds);
    }
    virtual void AddForceFrame (const csVector3& force, bool relative,
	const csVector3& position)
    {
      scfParent->AddForceFrame (force, relative, position);
    }
    virtual void ClearForces ()
    {
      scfParent->ClearForces ();
    }
    virtual void AddToGroup (const char* group)
    {
      scfParent->AddToGroup (group);
    }
    virtual void RemoveFromGroup (const char* group)
    {
      scfParent->RemoveFromGroup (group);
    }
    virtual iJoint* CreateJoint (iPcMechanicsObject* other)
    {
      return scfParent->CreateJoint (other);
    }
  } scfiPcMechanicsObject;

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

