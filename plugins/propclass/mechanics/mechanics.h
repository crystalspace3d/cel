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
CEL_DECLARE_FACTORY (MechanicsJoint)

struct celForce
{
  iPcMechanicsObject* body;
  float seconds;	// Remaining time. Not used if for entire frame.
  bool frame;		// True if processing is for entire frame.
  uint32 forceid;	// The string ID for the tag of this force.
  bool relative;	// True if force and position are relative to object.
  csVector3 force;	// Defines the magnitude and direction of the force.
  csVector3 position;	// The position of the force.
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
  enum actionids
  {
    action_setsystem = 0,
    action_setgravity,
    action_quickstep,
    action_enablestepfast,
    action_disablestepfast,
    action_setsteptime,
    action_setsimulationspeed,
    action_setplugin
  };
  static PropertyHolder propinfo;

  // Parameters.
  static csStringID param_dynsys;
  static csStringID param_gravity;
  static csStringID param_time;
  static csStringID param_simulationspeed;
  static csStringID param_plugin;
 
  csString pluginName;
  csRef<iDynamics> dynamics;
  csRef<iDynamicSystem> dynsystem;
  csRef<CS::Physics::Bullet::iDynamicSystem> bullet_dynSys;
  bool dynsystem_error_reported;
  csRef<iVirtualClock> vc;
  csArray<celForce> forces;
  csHash<iBodyGroup*, const char*> groups;
  float delta;
  float remaining_delta;
  float simulationspeed;

  void ProcessForces (float dt);
  iDynamics *GetDynamics ();

  iRigidBody* FindBody (const char* entityname);

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
  virtual void SetSimulationSpeed (float simulationspeed)
  {
    celPcMechanicsSystem::simulationspeed = simulationspeed;
  }
  virtual float GetSimulationSpeed () {return simulationspeed; }
  virtual void SetGravity (const csVector3& grav);
  virtual const csVector3 GetGravity ();

  virtual void AddForceTagged (iPcMechanicsObject* body, const csVector3& force,
	bool relative, const csVector3& position, uint32 forceid);
  virtual void RemoveForceTagged (iPcMechanicsObject* body, uint32 forceid);
  virtual void AddForceDuration (iPcMechanicsObject* body,
  	const csVector3& force,
	bool relative, const csVector3& position, float seconds);
  virtual void AddForceFrame (iPcMechanicsObject* body, const csVector3& force,
	bool relative, const csVector3& position);
  virtual void ClearForces (iPcMechanicsObject* body);
  virtual void ClearAllForces ();
  void ApplyForce (celForce& f);

  // override SetEntity to register and unregister from registry there.
  virtual void SetEntity(iCelEntity* entity);

  virtual csPtr<iRigidBody> CreateBody ();
  virtual void RemoveBody (iRigidBody* body);
  virtual void AddBodyToGroup (iRigidBody* body, const char* group);
  virtual void RemoveBodyFromGroup (iRigidBody* body, const char* group);
  virtual iJoint* CreateJoint (iRigidBody* body1, iRigidBody* body2);
  virtual void RemoveJoint (iJoint* joint);

  virtual void DisableStepFast ();
  virtual void EnableStepFast ();
  virtual void EnableQuickStep ();

  virtual void TickEveryFrame ();
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

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
#define CEL_BODY_CONVEXMESH 6

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
  enum actionids
  {
    action_initphys = 0,
    action_makestatic,
    action_setsystem,
    action_setmesh,
    action_setcolliderboundingsphere,
    action_setcollidersphere,
    action_setcollidercylinder,
    action_setcolliderbox,
    action_setcolliderboundingbox,
    action_setcolliderplane,
    action_setcollidermesh,
    action_setcolliderconvexmesh,
    action_setlinearvelocity,
    action_setangularvelocity,
    action_addforceonce,
    action_addforceduration,
    action_addforceframe,
    action_addforcetagged,
    action_removeforcetagged,
    action_clearforces,
    action_setposition,
    action_rotate,
    action_clearrotation,
    action_lookat,
    action_addtogroup
  };

  // Parameters.
  static csStringID param_friction;
  static csStringID param_mass;
  static csStringID param_elasticity;
  static csStringID param_density;
  static csStringID param_softness;
  static csStringID param_lift;
  static csStringID param_drag;
  static csStringID param_static;
  static csStringID param_systempcent;
  static csStringID param_systempctag;
  static csStringID param_meshpctag;
  static csStringID param_offset;
  static csStringID param_length;
  static csStringID param_axis;
  static csStringID param_angle;
  static csStringID param_size;
  static csStringID param_sizeadjustment;
  static csStringID param_radius;
  static csStringID param_radiusadjustment;
  static csStringID param_normal;
  static csStringID param_otherbody;
  static csStringID param_force;
  static csStringID param_relative;
  static csStringID param_position;
  static csStringID param_seconds;
  static csStringID param_velocity;
  static csStringID param_tag;
  static csStringID param_forward;
  static csStringID param_up;
  static csStringID param_rotation;
  static csStringID param_depth;
  static csStringID param_group;

  csRef<celVariableParameterBlock> params;

  csRef<iMessageDispatcher> dispatcher_cd;

  // Last assigned force id for this object
  uint32 forceidseed;
  // Last used tag for AddForceTagged() (only set through
  // PerformActionIndexed).
  uint32 last_tag;

  csWeakRef<iPcMechanicsSystem> mechsystem;
  csWeakRef<iPcMesh> pcmesh;
  csWeakRef<iPcLight> pclight;
  csWeakRef<iPcCamera> pccamera;
  csWeakRef<iRigidBody> body;
  int btype;
  body_data* bdata;

  bool is_static;
  bool cd_enabled;

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
  void SetTransform (const csReversibleTransform& tr);
  void AttachObject ();

  // For properties.
  enum propids
  {
    propid_lasttag = 0,
    propid_linearvelocity,
    propid_angularvelocity,
    propid_static,
    propid_cdcallback,
    propid_enabled
  };
  static PropertyHolder propinfo;

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
  virtual void SetBody (iRigidBody* body);

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
  virtual void SetCollisionCallbackEnabled (bool en) { cd_enabled = en; }
  virtual bool IsCollisionCallbackEnabled () const { return cd_enabled; }

  virtual void AttachColliderSphere (float radius, const csVector3& offset);
  virtual void AttachColliderBoundingSphere(float radiusadjusment = 0.0f);
  virtual void AttachColliderCylinder (float length, float radius,
  	const csOrthoTransform& trans);
  virtual void AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans);
  virtual void AttachColliderBoundingBox(const csVector3& sizeadjustment = csVector3(0));
  virtual void AttachColliderPlane (const csPlane3& plane);
  virtual void AttachColliderConvexMesh ();
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

  void Collision (iRigidBody *thisbody, iRigidBody *otherbody,
	const csVector3& pos, const csVector3& normal, float depth);

  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
  virtual bool SetPropertyIndexed (int, bool v);
  virtual bool GetPropertyIndexed (int, bool& v);
  virtual bool SetPropertyIndexed (int, const csVector3& vec);
  virtual bool GetPropertyIndexed (int, csVector3& vec);

  virtual csVector3 LocalToWorld(csVector3 local);
  virtual csVector3 WorldToLocal(csVector3 world);

  struct DynamicsCollisionCallback : public scfImplementation1<
	DynamicsCollisionCallback, iDynamicsCollisionCallback>
  {
    celPcMechanicsObject* parent;
    DynamicsCollisionCallback (celPcMechanicsObject* parent) :
      scfImplementationType (this)
    {
      DynamicsCollisionCallback::parent = parent;
    }
    virtual ~DynamicsCollisionCallback ()
    {
    }

    virtual void Execute (iRigidBody *thisbody, iRigidBody *otherbody,
	const csVector3& pos, const csVector3& normal, float depth)
    {
      parent->Collision (thisbody, otherbody, pos, normal, depth);
    }
  } *scfiDynamicsCollisionCallback;
};

/**
 * This is a joint property class.
 */
class celPcMechanicsJoint : public scfImplementationExt1<
	celPcMechanicsJoint, celPcCommon, iPcMechanicsJoint>
{
private:
  // Actions
  enum actionids
  {
    action_setparentbody = 0,
    action_setposition,
    action_setconstraindist,
    action_setdistances,
    action_setconstrainangle,
    action_setangles
  };
  static PropertyHolder propinfo;

  // Parameters.
  static csStringID param_body;
  static csStringID param_position;
  static csStringID param_min;
  static csStringID param_max;
  static csStringID param_x;
  static csStringID param_y;
  static csStringID param_z;

  celOneParameterBlock* params;

  csWeakRef<iCelEntity> parent_body;
  csWeakRef<iJoint> joint;

  void CreateJoint ();

public:
  celPcMechanicsJoint (iObjectRegistry* object_reg);
  virtual ~celPcMechanicsJoint ();

  virtual iJoint* GetJoint () { return joint; }

  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
};

#endif // __CEL_PF_MECHANICS_SYSTEM_FACTORY__

