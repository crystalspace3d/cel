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

#ifndef __CEL_PF_MECHANICS_SYSTEM__
#define __CEL_PF_MECHANICS_SYSTEM__

#include "cstypes.h"
#include "csutil/scf.h"

#include "propclass/mechcommon.h"

class csVector3;
class csBox3;
class csOrthoTransform;
class csPlane3;
struct iDynamicSystem;
struct iRigidBody;
struct iSector;
struct iPcMesh;
struct iPcLight;
struct iPcCamera;
struct iPcMechanicsObject;
struct iJoint;

/**
 * Property class representing a dynamic system.
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>SetSystem: parameters 'dynsys' (string).
 * <li>SetGravity: parameters 'gravity' (float).
 * <li>SetStepTime: parameters 'time' (float).
 * <li>QuickStep: no parameters.
 * <li>EnableStepFast: no parameters.
 * <li>DisableStepFast: no parameters.
 * </ul>
 */
struct iPcMechanicsSystem : public virtual iBase
{
  SCF_INTERFACE (iPcMechanicsSystem, 0, 0, 1);

  /**
   * Set the dynamic system.
   * \param dynsys a pointer to the iDynamicSystem that this iPcMechanicsSystem
   * represents.
   */
  virtual void SetDynamicSystem (iDynamicSystem* dynsys) = 0;

  /**
   * Set the dynamic system.
   * \param dynsysname the name of the iDynamicSystem that this
   * iPcMechanicsSystem represents.
   */
  virtual void SetDynamicSystem (const char *dynsysname) = 0;


  /**
   * Get the dynamic system.
   */
  virtual iDynamicSystem* GetDynamicSystem () = 0;

  /**
   * Define the time we want for one step. By default this is 0.01 milliseconds.
   * \param delta the time (in milliseconds) that one step represents.
   */
  virtual void SetStepTime (float delta) = 0;

  /**
   * Get the current step time.
   */
  virtual float GetStepTime () const = 0;

  /**
   * Define the gravity for the system. By default this is (0,-9.8,0).
   * \param grav a vector representing the force of gravity for this system.
   */
  virtual void SetGravity (const csVector3& grav) = 0;

  /**
   * Get the current gravity.
   */
  virtual const csVector3 GetGravity () = 0;

  /**
   * During the specified time (in milliseconds) add the force every
   * step to the given body. This function is called by
   * iPcMechanicsObject::AddForceDuration().
   * \param pcobject a pointer to the object that will receive this force every
   * step until the end of the given time.
   * \param force a vector representing the force to add to this object.
   * \param relative if set to true, the given force and position are both in
   * object space; otherwise they are in world space.
   * \param position the position of the force.
   * \param seconds the number of seconds that this force should last.
   */
  virtual void AddForceDuration (iPcMechanicsObject* pcobject,
  	const csVector3& force, bool relative, const csVector3& position,
	float seconds) = 0;

  /**
   * During the next frame add the force every step. This function is called
   * by iPcMechanicsObject::AddForceFrame().
   * \param pcobject a pointer to the object that will receive this force every
   * step until the end of the frame.
   * \param force a vector representing the force to add to this object.
   * \param relative if set to true, the given force and position are both in
   * object space; otherwise they are in world space.
   * \param position the position of the force.
   */
  virtual void AddForceFrame (iPcMechanicsObject* pcobject,
  	const csVector3& force, bool relative, const csVector3& position) = 0;

  /**
   * Add a force with the given tag, to be manually removed later. This
   * function is called by iPcMechanicsObject::AddForceTagged().
   * \param pcobject a pointer to the object that will receive this force every
   * step until it is removed.
   * \param force a vector representing the force to add to this object.
   * \param relative if set to true, the given force and position are both in
   * object space; otherwise they are in world space.
   * \param position the position of the force.
   * \param forceid the tag for this force. (use
   * iCelPlLayer::FetchStringID() to get this.)
   */
  virtual void AddForceTagged (iPcMechanicsObject* pcobject,
  	const csVector3& force, bool relative, const csVector3& position,
	uint32 forceid) = 0;

  /**
   * Remove the force with the given tag. This function is called by
   * iPcMechanicsObject::RemoveForceTagged().
   * \param pcobject a pointer to the object that is currently being affected
   * by the given force.
   * \param forceid the tag of the desired force.
   */
  virtual void RemoveForceTagged (iPcMechanicsObject* pcobject,
	uint32 forceid) = 0;

  /**
   * Remove the given body from the force queues (filled with AddForceFrame()
   * and AddForceDuration()).
   * \param pcobject a pointer to the iPcMechanicsObject to remove all forces
   * from.
   */
  virtual void ClearForces (iPcMechanicsObject* pcobject) = 0;

  /**
   * Clear all forces.
   */
  virtual void ClearAllForces () = 0;

  /**
   * Create an iRigidBody and add it to the system.
   */
  virtual csPtr<iRigidBody> CreateBody () = 0;

  /**
   * Remove a given iRigidBody from the system.
   * \param body the body to remove from the system.
   */
  virtual void RemoveBody (iRigidBody* body) = 0;

  /**
   * Add a body to the specified group.
   * \param body the body to add to the group.
   * \param group the name of the group to add the body to. Creates the group if
   * it does not exist.
   */
  virtual void AddBodyToGroup (iRigidBody* body, const char* group) = 0;

  /**
   * Remove a body from the specified group.
   * \param body the body to remove from the group.
   * \param group the name of the group to remove the body from.
   */
  virtual void RemoveBodyFromGroup (iRigidBody* body, const char* group) = 0;

  /**
   * Create a joint between the two given bodies.
   */
  virtual iJoint* CreateJoint (iRigidBody* body1, iRigidBody* body2) = 0;

  /**
   * Remove a given joint from the system.
   */
  virtual void RemoveJoint (iJoint* joint) = 0;

  /**
   * Disable fast stepping. This is very accurate but can quickly
   * break with lots of objects.
   */
  virtual void DisableStepFast () = 0;
  /**
   * Enable fast stepping. This is a lot faster then DisableStepFast().
   * Default.
   */
  virtual void EnableStepFast () = 0;
  /**
   * Very fast but not accurate.
   */
  virtual void EnableQuickStep () = 0;
};

/**
 * A dynamic body.
 * <p>
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * <ul>
 * <li>pcdynamicbody_collision: an entity has collided with this entity.
 * </ul>
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>InitPhys: parameters 'friction' (float), 'elasticity' (float),
 * 	'density' (float), 'softness' (float), 'lift' (vector3), and
 * 	'drag' (float).
 * <li>MakeStatic: parameters 'static' (bool).
 * <li>SetSystem: parameters 'syspcent' (string) and 'syspctag' (string).
 * <li>SetMesh: parameters 'mechpctag' (string).
 * <li>SetColliderSphere: parameters 'radius' (float) and 'offset' (vector3).
 * <li>SetColliderCylinder: parameters 'length' (float), 'radius' (float)
 *     'axis' (vector3), 'offset' (vector3), and 'angle' (float).
 * <li>SetColliderBox: parameters 'size' (vector3), 'axis' (vector3),
 *     'angle' (float), and 'offset' (vector3).
 * <li>SetColliderPlane: parameters 'normal' (vector3) and 'offset' (float).
 * <li>SetColliderMesh: no parameters.
 * <li>SetLinearVelocity: parameters 'velocity' (vector3).
 * <li>SetAngularVelocity: parameters 'velocity' (vector3).
 * <li>AddForceOnce: parameters 'force' (vector3), 'relative' (bool),
 *     and 'position' (vector3).
 * <li>AddForceDuration: parameters 'force' (vector3), 'relative' (bool),
 *     'position' (vector3), and 'seconds' (float).
 * <li>AddForceFrame: parameters 'force' (vector3), 'relative' (bool),
 *     and 'position' (vector3).
 * <li>AddForceTagged: parameters 'force' (vector3), 'relative' (bool),
 *     and 'position' (vector3). The tag will be set in the property
 *     'cel.property.lasttag'.
 * <li>RemoveForceTagged: parameters 'tag' (long).
 * <li>ClearForces: no parameters.
 * <li>SetPosition: parameters 'position' (vector3).
 * <li>ClearRotation: no parameters.
 * <li>LookAt: parameters 'forward' (vector3) and 'up' (vector3).
 * </ul>
 * <p>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * <ul>
 * <li>lasttag (long, read): last tag returned by AddForceTagged action.
 * </ul>
 */
struct iPcMechanicsObject : public virtual iBase
{
  SCF_INTERFACE (iPcMechanicsObject, 0, 0, 1);

  /**
   * Set the mesh which this body will control.
   * If this is used then this overrides the default behaviour of
   * trying to find 'pcmesh', 'pclight', and 'pccamera' in that order.
   * \param mesh a pointer to the iPcMesh to control.
   */
  virtual void SetMesh (iPcMesh* mesh) = 0;

  /**
   * Get the associated mesh property class.
   */
  virtual iPcMesh* GetMesh () = 0;

  /**
   * Set the light which this body will control.
   * If this is used then this overrides the default behaviour of
   * trying to find 'pcmesh', 'pclight', and 'pccamera' in that order.
   * \param light a pointer to the iPcLight to control.
   */
  virtual void SetLight (iPcLight* light) = 0;

  /**
   * Get the associated light property class.
   */
  virtual iPcLight* GetLight () = 0;

  /**
   * Set the camera which this body will control.
   * If this is used then this overrides the default behaviour of
   * trying to find 'pcmesh', 'pclight', and 'pccamera' in that order.
   * \param camera a pointer to the iPcCamera to control.
   */
  virtual void SetCamera (iPcCamera* camera) = 0;

  /**
   * Get the associated camera property class.
   */
  virtual iPcCamera* GetCamera () = 0;

  /**
   * Set the mechanics system to use. If you don't do this then the
   * object will look in the object registry for a suitable mechanics
   * system.
   * \param mechsys a pointer to the iPcMechanicsSystem to use.
   */
  virtual void SetMechanicsSystem (iPcMechanicsSystem* mechsys) = 0;

  /**
   * Get the associated mechanics system.
   */
  virtual iPcMechanicsSystem* GetMechanicsSystem () = 0;

  /**
   * Get the associated rigid body.
   */
  virtual iRigidBody* GetBody () = 0;


  ////
  //Object properties.

  /**
   * Set the object's friction. Must be called before Attach..., otherwise
   * defaults to 1.
   * \param friction the friction coefficient for this object.
   */
  virtual void SetFriction (float friction) = 0;

  /**
   * Set the object's mass. Must be called before Attach..., otherwise
   * defaults to 1.
   * \param mass the mass of this object.
   */
  virtual void SetMass (float mass) = 0;

  /**
   * Set the object's elasticity. Must be called before Attach..., otherwise
   * defaults to 0.
   * \param elasticity the elasticity of this object.
   */
  virtual void SetElasticity (float elasticity) = 0;

  /**
   * Set the object's density. Must be called before Attach..., otherwise
   * defaults to 1.
   * \param density the density of this object.
   */
  virtual void SetDensity (float density) = 0;

  /**
   * Set the object's softness. Must be called before Attach..., otherwise
   * defaults to 0.
   * \param softness the softness of this object.
   */
  virtual void SetSoftness (float softness) = 0;

  /**
   * Set the object's lift. Must be called before Attach..., otherwise
   * defaults to 0.
   * \param lift the lift generated by this object in a fluid.
   */
  virtual void SetLift (const csVector3& lift) = 0;

  /**
   * Set the object's drag. Must be called before Attach..., otherwise
   * defaults to 0.
   * \param drag the drag generated by this object in a fluid.
   */
  virtual void SetDrag (float drag) = 0;

  /**
   * Get the object's friction. Must be called before Attach..., otherwise
   * defaults to 1.
   */
  virtual float GetFriction () = 0;

  /**
   * Get the object's mass. Must be called before Attach..., otherwise
   * defaults to 1.
   */
  virtual float GetMass () = 0;

  /**
   * Get the object's elasticity. Must be called before Attach..., otherwise
   * defaults to 0.
   */
  virtual float GetElasticity () = 0;

  /**
   * Get the object's density. Must be called before Attach..., otherwise
   * defaults to 1.
   */
  virtual float GetDensity () = 0;

  /**
   * Get the object's softness. Must be called before Attach..., otherwise
   * defaults to 0.
   */
  virtual float GetSoftness () = 0;

  /**
   * Get the object's lift. Must be called before Attach..., otherwise
   * defaults to 0.
   */
  virtual const csVector3& GetLift () = 0;

  /**
   * Get the object's drag. Must be called before Attach..., otherwise
   * defaults to 0.
   */
  virtual float GetDrag () = 0;


  ////
  //Initial velocities

  /**
   * Set the initial linear velocity (movement) of the object.
   * \param vel the linear velocity to apply to the object.
   */
  virtual void SetLinearVelocity (const csVector3& vel) = 0;

  /**
   * Set the initial angular velocity (rotation) of the object.
   * \param vel the angular velocity to apply to the object
   */
  virtual void SetAngularVelocity (const csVector3& vel) = 0;


  ////
  //Reading velocities

  /**
   * Get the current linear velocity (movement) of the object.
   */
  virtual const csVector3 GetLinearVelocity () = 0;

  /**
   * Get the current angular velocity (rotation) of the object.
   */
  virtual const csVector3 GetAngularVelocity () = 0;


  ////
  //Static objects

  /**
   * Make static.
   * \param stat if true, the object is static; otherwise it is affected
   * by forces.
   */
  virtual void MakeStatic (bool stat) = 0;
  
  /**
   * Return true if static.
   */
  virtual bool IsStatic () const = 0;


  ////
  //Colliders

  /**
   * Create a sphere collider for this object.
   * \param radius the radius of the sphere.
   * \param offset the offset from the center of the object to the center of
   * the sphere.
   */
  virtual void AttachColliderSphere (float radius, const csVector3& offset) = 0;

  /**
   * Create a cylinder collider for this object.
   * \param length the length of the cylinder.
   * \param radius the radius of the cylinder.
   * \param trans the transformation to apply to the cylinder.
   */
  virtual void AttachColliderCylinder (float length, float radius,
  	const csOrthoTransform& trans) = 0;

  /**
   * Create a box collider for this object.
   * \param size the size of the box in each direction.
   * \param trans the transformation to apply to the box.
   */
  virtual void AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans) = 0;

  /**
   * Create a plane collider for this object.
   * \param plane the definition of the plane to use.
   */
  virtual void AttachColliderPlane (const csPlane3& plane) = 0;

  /**
   * Create a mesh collider for this object from the attached iPcMesh.
   */
  virtual void AttachColliderMesh () = 0;


  ////
  //Forces

  /**
   * Add a force once to the given object.
   * \param force a vector representing the force to add to this object.
   * \param relative if set to true, the given force and position are both in
   * object space; otherwise they are in world space.
   * \param position the position of the force.
   */
  virtual void AddForceOnce (const csVector3& force, bool relative,
	const csVector3& position) = 0;

  /**
   * During the specified time (in seconds) add the force every step.
   * \param force a vector representing the force to add to this object.
   * \param relative if set to true, the given force and position are both in
   * object space; otherwise they are in world space.
   * \param position the position of the force.
   * \param seconds the number of seconds that this force should last.
   */
  virtual void AddForceDuration (const csVector3& force, bool relative,
	const csVector3& position, float seconds) = 0;

  /**
   * During the next frame add the force every step.
   * \param force a vector representing the force to add to this object.
   * \param relative if set to true, the given force and position are both in
   * object space; otherwise they are in world space.
   * \param position the position of the force.
   */
  virtual void AddForceFrame (const csVector3& force, bool relative,
	const csVector3& position) = 0;

  /**
   * Add a force with the given tag, to be manually removed later.
   * \param force a vector representing the force to add to this object.
   * \param relative if set to true, the given force and position are both in
   * object space; otherwise they are in world space.
   * \param position the position of the force.
   * return value is the force id tag (use it to remove the force later).
   */

  virtual uint32 AddForceTagged (const csVector3& force, bool relative, 
	const csVector3& position) = 0;

  /**
   * Remove the force with the given tag.
   * \param forceid the tag of the desired force.
   */
  virtual void RemoveForceTagged (uint32 forceid) = 0;

  /**
   * Clear the permanent forces on this body.
   */
  virtual void ClearForces () = 0;


  ////
  //Groups and Joints

  /**
   * Add this body to the specified group.
   * \param group the name of the group to add the body to. Creates the group if
   * it does not exist.
   */
  virtual void AddToGroup (const char* group) = 0;

  /**
   * Remove this body from the specified group.
   * \param group the name of the group to remove the body from.
   */
  virtual void RemoveFromGroup (const char* group) = 0;

  /**
   * Create a joint between this object and another.
   */
  virtual iJoint* CreateJoint (iPcMechanicsObject* other) = 0;

  /**
   * Convert a vector from this objects local system to world coord system
   */
  virtual csVector3 LocalToWorld(csVector3 local) = 0;
  /**
   * Convert a vector from world coord system to this objects local system
   */
  virtual csVector3 WorldToLocal(csVector3 world) = 0;
};

/**
 * A joint.
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>SetParentBody: parameters 'body' (string).
 * <li>SetPosition: parameters 'position' (vector3).
 * <li>SetConstrainDist: parameters 'x' (bool), 'y' (bool), and 'z' (bool).
 * <li>SetDistances: parameters 'min' (vector3) and 'max' (vector3).
 * <li>SetConstrainAngle: parameters 'x' (bool), 'y' (bool), and 'z' (bool).
 * <li>SetAngles: parameters 'min' (vector3) and 'max' (vector3).
 * </ul>
 */
struct iPcMechanicsJoint : public virtual iBase
{
  SCF_INTERFACE (iPcMechanicsJoint, 0, 0, 1);

  /**
   * Get the joint corresponding with this.
   */
  virtual iJoint* GetJoint () = 0;
};

#endif // __CEL_PF_MECHANICS_SYSTEM__

