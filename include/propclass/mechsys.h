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
struct iPcMechanicsObject;

SCF_VERSION (iPcMechanicsSystem, 0, 0, 1);

/**
 * Property class representing a dynamic system.
 */
struct iPcMechanicsSystem : public iBase
{
  /**
   * Set the dynamic system.
   * \param dynsys a pointer to the iDynamicSystem that this iPcMechanicsSystem
   * represents.
   */
  virtual void SetDynamicSystem (iDynamicSystem* dynsys) = 0;

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
   * \param forcetagid the tag for this force. (use
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
   * \param forcetagid the tag of the desired force.
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
};

SCF_VERSION (iPcMechanicsObject, 0, 0, 1);

/**
 * A dynamic body.
 * <p>
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * <ul>
 * <li>pcdynamicbody_collision: an entity has collided with this entity.
 * </ul>
 */
struct iPcMechanicsObject : public iBase
{
  /**
   * Set the mesh which this body will control.
   * If not called then this property class will automatically
   * look for 'pcmesh'.
   * \param mesh a pointer to the iPcMesh to control.
   */
  virtual void SetMesh (iPcMesh* mesh) = 0;

  /**
   * Get the associated mesh property class.
   */
  virtual iPcMesh* GetMesh () = 0;

  /**
   * Set the mechanics system to use. This is required.
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
   * \param forceid is set to the ID of this force.
   */
  virtual void AddForceTagged (const csVector3& force, bool relative,
	const csVector3& position, uint32& forceid) = 0;

  /**
   * Remove the force with the given tag.
   * \param forcetagid the tag of the desired force.
   */
  virtual void RemoveForceTagged (uint32 forceid) = 0;

  /**
   * Clear the permanent forces on this body.
   */
  virtual void ClearForces () = 0;
};

#endif // __CEL_PF_MECHANICS_SYSTEM__

