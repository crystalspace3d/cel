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
   * Get the effective maximum thrust of this thruster, taking into account the
   * current environment.
   */
  virtual float GetEffectiveMaxThrust () = 0;


  ////
  //Applying thrust

  /**
   * Request that the thruster apply a percentage of its total thrust.
   * \param group the group that is requesting this thrust.
   * \param thrust the amount of thrust to request.
   */
  virtual void ThrustRequest (iPcMechanicsThrusterGroup* group,
	percentage thrust) = 0;

  /**
   * Cancel a previous thrust request. (returns false if the thrust request has
   * already been cancelled or does not exist.)
   * \param group the group that requested this thrust.
   */
  virtual void CancelThrustRequest (iPcMechanicsThrusterGroup* group) = 0;

  /**
   * Is this thruster currently taking orders? (of fries)
   */
  virtual bool IsAvailable () = 0;
};


SCF_VERSION (iPcMechanicsThrusterGroup, 0, 0, 1);

enum celAxisType {
  CEL_TGT_ROTATION,
  CEL_TGT_TRANSLATION
};

/**
 * Property class representing a group of thrusters that can translate or
 * rotate the object in a certain way.
 */
struct iPcMechanicsThrusterGroup : public iBase
{
  /**
   * Set this thruster group's type.
   * \param type one of celAxisType, the type of thruster this is.
   */
  virtual void SetType (celAxisType type) = 0;

  /**
   * Get this thruster group's type.
   */
  virtual celAxisType GetType () = 0;


  /**
   * Add a thruster to this thruster group. At least one is required.
   * \param thruster the thruster to add to this group.
   * \param multiplier the percentage of the maximum thrust of this thruster to
   * use.
   */
  virtual void AddThruster (iPcMechanicsThruster* thruster, percentage multiplier) = 0;

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
   * Get the strength of the thruster group. This is equal to the magnitude of
   * the maximum net force minus the magnitude of the maximum net moment that
   * this group can produce.
   */
  virtual float GetStrength () = 0;

  /**
   * Are all the thrusters in this thruster group taking orders?
   */
  virtual bool IsAvailable () = 0;

  /**
   * Apply the given percentage of total thrust.
   */
  virtual void ApplyThrust (percentage thrust) = 0;

  /**
   * A thruster calls this when its effective thrust changes.
   */
  virtual void UpdateThrust (const char* thrusterid, float thrust) = 0;
};


SCF_VERSION (iPcMechanicsThrusterController, 0, 0, 1);

/**
 * Property class that controls the thrusters attached to an object.
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
   * \param type one of celAxisType, the type of groups that can be
   * added to this group.
   * \param axis the vector representing either the direction of movement for
   * this axis (for translational axes) or the direction of the axis around
   * which rotation on this axis occurs. (for rotational axes)
   */
  virtual void AddAxis (const char* name, celAxisType type, const
	csVector3 axis) = 0;

  /**
   * Get the axis of rotation or translation.
   * \param name the name of the axis to query.
   */
  virtual const csVector3 GetAxis (const char* name) = 0;

  /**
   * Get the strength of the best thruster group in the given axis.
   * \param name the name of the axis to check.
   */
  virtual float GetAxisStrength (const char* name) = 0;

  /**
   * Add a thruster group to the specified axis.
   * \param thrustergroup the group to add.
   * \param axisname the name of the axis to add to.
   */
  virtual void AddThrusterGroup (iPcMechanicsThrusterGroup* thrustergroup,
	const char* axisname) = 0;

  /**
   * Remove a thruster group from the specified axis.
   * \param thrustergrouptag the tag of the group to remove.
   * \param axisname the name of the axis to remove from.
   */
  virtual void RemoveThrusterGroup (const char* thrustergrouptag, const char*
	axisname) = 0;

  /**
   * Apply the given percentage of total thrust for a given axis.
   * \param thrust the percentage of the total thrust for this axis to apply.
   * \param axisname the name of the axis to apply thrust along.
   */
  virtual void ApplyThrust (percentage thrust, const char* axisname) = 0;
};


struct celThrusterRequestData
{
public:
  float thrust;
  csRef<iPcMechanicsThrusterGroup> group;

  celThrusterRequestData ()
  {
    celThrusterRequestData::group = 0;
    celThrusterRequestData::thrust = 0.0f;
  }
  ~celThrusterRequestData ()
  {
  };
};

#endif //__CEL_PF_MECHANICS_THRUSTER__
