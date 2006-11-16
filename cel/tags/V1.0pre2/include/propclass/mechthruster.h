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

#ifndef __CEL_PF_MECHANICS_THRUSTER__
#define __CEL_PF_MECHANICS_THRUSTER__

#include "cstypes.h"
#include "csutil/scf.h"

#include "propclass/mechcommon.h"

class csVector3;
struct iPcMechanicsObject;
struct iPcMechanicsThrusterGroup;

SCF_VERSION (iPcMechanicsThruster, 0, 0, 1);

/**
 * Property class representing a thruster that affect forces on the attached
 * object.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - InitThruster: parameters 'objectpctag' (string), 'position' (vector3).
 */
struct iPcMechanicsThruster : public iBase
{
  /**
   * Set the mechanics object to affect. This is required.
   * \param mechsys the object the thruster is attached to.
   */
  virtual void SetMechanicsObject (iPcMechanicsObject* mechsys) = 0;

  /**
   * Get the associated mechanics object.
   */
  virtual iPcMechanicsObject* GetMechanicsObject () = 0;


  ////
  //Set thruster properties

  /**
   * Set the thruster's position relative to the object.
   * \param pos the offset from the center of the object to the thruster.
   */
  virtual void SetPosition (const csVector3& pos) = 0;

  /**
   * Get the thruster's position relative to the object.
   */
  virtual const csVector3& GetPosition () = 0;

  /**
   * Set the orientation of the thruster.
   * \param orientation the direction in which the thruster points.
   */
  virtual void SetOrientation (const csVector3& orientation) = 0;

  /**
   * Get the orientation of the thruster.
   */
  virtual const csVector3& GetOrientation () = 0;

  /**
   * Set the maximum thrust.
   * \param maxthrust the maximum amount of thrust this thruster can
   * accomodate.
   */
  virtual void SetMaxThrust (float maxthrust) = 0;

  /**
   * Get the maximum thrust.
   */
  virtual float GetMaxThrust () = 0;

  /**
   * Get the amount of force a given thrust will produce, taking into account
   * the current environment.
   * \param thrust the amount of thrust to convert.
   */
  virtual float GetThrustForce (float thrust) = 0;

  /**
   * How much thrust is available on this thruster?
   */
  virtual float AvailableThrust () = 0;


  ////
  //Applying thrust

  /**
   * Applies a delta to the thruster's current thrust.
   * \param deltathrust the change in thrust.
   */
  virtual void ThrustChange (float deltathrust) = 0;
};


SCF_VERSION (iPcMechanicsBalancedGroup, 0, 0, 1);

enum celAxisType
{
  CEL_AT_NONE,
  CEL_AT_ROTATION,
  CEL_AT_TRANSLATION
};

/**
 * Property class representing a balanced group of thrusters that can translate
 * or rotate the object in a certain axis. 
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - SetType: parameters 'type' (string, "rotation" or "translation").
 * - AddThruster: parameters 'thrusterpctag' (string), 'multiplier' (float).
 */
struct iPcMechanicsBalancedGroup : public iBase
{
  /**
   * Set this group's type.
   * \param type one of celAxisType, the type of thruster this is.
   */
  virtual void SetType (celAxisType type) = 0;

  /**
   * Get this group's type.
   */
  virtual celAxisType GetType () = 0;


  /**
   * Add a thruster to this thruster group. At least one is required.
   * \param thruster the thruster to add to this group.
   * \param multiplier the percentage of the maximum thrust of this thruster to
   * use when activated through this balanced group. (must be >0 and <=1)
   */
  virtual void AddThruster (iPcMechanicsThruster* thruster,
      float multiplier) = 0;

  /**
   * Remove a thruster.
   * \param tag the tag of the thruster to add to the group.
   */
  virtual void RemoveThruster (const char* tag) = 0;

  /**
   * Get the associated thruster.
   * \param tag the tag of the thruster to retrieve from the group.
   */
  virtual iPcMechanicsThruster* GetThruster (const char* tag) = 0;

  /**
   * What is the maximum amount of thrust that can be applied to the thrusters
   * in this group?
   */
  virtual float AvailableThrust () = 0;

  /**
   * Get the amount of force that the maximum thrust for this group would
   * produce.
   */
  virtual float AvailableThrustForce () = 0;

  /**
   * Apply the given thrust to all thrusters in this balanced group.
   * \param deltathrust the amount of thrust to apply. (must be > 0)
   */
  virtual void ChangeThrust (float deltathrust) = 0;
};


SCF_VERSION (iPcMechanicsThrusterController, 0, 0, 1);

/**
 * Property class that controls the thrusters attached to an object through the
 * use of balanced thruster groups.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - InitThrusterController: parameters 'objectpctag' (string).
 * - AddAxis: parameters 'axisname' (string), 'axistype' (string, "rotation"
 *   or "translation"), 'axisdir' (vector3).
 * - ApplyThrust: parameters 'axisname' (string), 'thrust' (float).
 * - AddBalancedGroup: parameters 'axisname' (string), 'balancedgroup' (string).
 */
struct iPcMechanicsThrusterController : public iBase
{
  /**
   * Set the mechanics object to affect. This is required.
   * \param mechobj the object this controller will affect.
   */
  virtual void SetMechanicsObject (iPcMechanicsObject* mechobj) = 0;

  /**
   * Get the associated mechanics object.
   */
  virtual iPcMechanicsObject* GetMechanicsObject () = 0;

  /**
   * Add a named axis to the controller.
   * \param name the name for this axis.
   * \param type one of celAxisType, the type of groups that can be added to
   * this group.
   * \param axis the vector representing either the direction of movement for
   * this axis (for translational axes) or the direction of the axis around
   * which rotation occurs. (for rotational axes)
   */
  virtual void AddAxis (const char* name, celAxisType type, const
	csVector3 axis) = 0;

  /**
   * Get the axis of rotation or translation.
   * \param name the name of the axis to query.
   */
  virtual const csVector3 GetAxis (const char* name) = 0;

  /**
   * Get the type of the given axis
   * \param name the name of the axis to query.
   */
  virtual const celAxisType GetAxisType (const char* name) = 0;

  /**
   * Get the current velocity in the given axis.
   * \param name the name of the axis to query.
   */
  virtual const float GetAxisVelocity (const char* name) = 0;

  /**
   * Get the strength of the best thruster group in the given axis.
   * \param name the name of the axis to check.
   */
  virtual float GetAxisMaxForce (const char* name) = 0;

  /**
   * Get the amount of thrust needed to produce the maximum force in the given
   * axis.
   * \param name the name of the axis to check.
   */
  virtual float GetAxisMaxThrust (const char* name) = 0;

  /**
   * Add a balanced thruster group to the specified axis.
   * \param group the group to add.
   * \param axisname the name of the axis to add to.
   */
  virtual void AddBalancedGroup (iPcMechanicsBalancedGroup* group,
	const char* axisname) = 0;

  /**
   * Remove a thruster group from the specified axis.
   * \param grouptag the tag of the group to remove.
   * \param axisname the name of the axis to remove from.
   */
  virtual void RemoveBalancedGroup (const char* grouptag, const char*
	axisname) = 0;

  /**
   * Apply the given percentage of total thrust for a given axis.
   * \param thrust the percentage of the total thrust for this axis to apply.
   * \param axisname the name of the axis to apply thrust along.
   * \param id is set to the id of the thrust request being applied.
   */
  virtual void ApplyThrust (float thrust, const char* axisname,
	uint32& id) = 0;

  /**
   * Cancels specified thrust request.
   * \param id the id of the thrust request to be cancelled.
   */
  virtual void CancelThrust (uint32 id) = 0;
};

#endif //__CEL_PF_MECHANICS_THRUSTER__
