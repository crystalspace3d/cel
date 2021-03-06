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

#ifndef __CEL_PF_CRAFTCONTROLLER_FACTORY__
#define __CEL_PF_CRAFTCONTROLLER_FACTORY__

#include "cstypes.h"

#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "celtool/ticktimer.h"

#include "propclass/craft.h"

/**
 * Factory for hover.
 */
CEL_DECLARE_FACTORY (CraftController)

/**
 * Hover stabiliser property class.
 */
class celPcCraftController : public scfImplementationExt1<
	celPcCraftController, celPcCommon , iPcCraftController>,
	public celPeriodicTimer
{
public:
  celPcCraftController (iObjectRegistry* object_reg);
  virtual ~celPcCraftController ();

  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
  virtual void Tick ();

  /**
   * Should Tick every frame and update the objects
   * height
   */
  virtual void UpdateBody();

  /* I have done the functions this way, as opposed to
       TurnLeft(float isturning);
     so that later on they can easily be extended with
     initial reactionaries */
  virtual void StartTurnLeft () { turn_left = true; }
  virtual void StopTurnLeft () { turn_left = false; }
  virtual void StartTurnRight () { turn_right = true; }
  virtual void StopTurnRight () { turn_right = false; }

  virtual void StartTurnUp () { turn_up = true; }
  virtual void StopTurnUp () { turn_up = false; }
  virtual void StartTurnDown () { turn_down = true; }
  virtual void StopTurnDown () { turn_down = false; }

  virtual void SetAccTurn (float tacc) { turn_acc = tacc; }
  virtual void SetMaxTurn (float turn) { turn_max = turn; }
  virtual void SetAccPitch (float uacc) { pitch_acc = uacc; }
  virtual void SetMaxPitch (float mud) { pitch_max = mud; }
  virtual void SetRoll (float rol) { roll = rol; }
  virtual void SetThrustForce (float tf) { thrust = tf; }
  virtual void SetTopSpeed (float tspeed) { topspeed = tspeed; }
  virtual void SetAfterBurnerTopSpeed (float tspeed)
  { topburnerspeed = tspeed; }
  virtual void SetRedirectVelocityRatio (float rdvr)
  { redirect_vel_ratio = rdvr; }
  virtual void SetDecelerationRate (float decr)
  { deceleration_rate = 1.0 - decr; }
  virtual void SetBrakingSpeed (float bspd)
  { braking_speed = 1.0 - bspd; }

  virtual void ThrustOn () { thrust_on = true; }
  virtual void ThrustOff () { thrust_on = false; }
  virtual bool IsThrusterOn () { return thrust_on; }

  virtual void AfterBurnerOn () { after_burner = true; }
  virtual void AfterBurnerOff () { after_burner = false; }

  virtual void BrakesOn () { brakes_on = true; }
  virtual void BrakesOff () { brakes_on = false; }

  virtual void SlideOn () { slide_on = true; }
  virtual void SlideOff () { slide_on = false; }

  virtual bool GetPropertyIndexed (int, float& val);
  virtual bool SetPropertyIndexed (int, float val);

private:
  static PropertyHolder propinfo;

  // Actions
  enum actionids
  {
    action_sliding = 0,
    action_braking,
    action_thruster,
    action_aburner
  };

  // For properties.
  enum propids
  {
    propid_turnmax = 0,
    propid_turnacc,
    propid_pitchmax,
    propid_pitchacc,
    propid_roll,
    propid_thrust,
    propid_topspeed,
    propid_atopspeed,
    propid_brakingspeed,
    propid_decelrate,
    propid_rvelratio
  };

  // Parameters.
  static csStringID id_enabled;

  void DoTurningCalc (bool isturning, float &turn, float acc, float max);

  // turning variables
  bool turn_left, turn_right;
  bool turn_up, turn_down;

  float current_turning_left, current_turning_right;
  float current_up, current_down;

  float turn_acc, pitch_acc;
  float turn_max, pitch_max;

  // how much roll when turning left and right
  float roll;

  // thruster variables
  bool thrust_on;
  float thrust;
  ///< speed at which the thruster turns off
  float topspeed;

  bool after_burner;
  float topburnerspeed;

  // airbrake variables
  float speed_ratio_change, dangle;

  bool slide_on;
  float redirect_vel_ratio;
  float deceleration_rate;

  bool brakes_on;
  float braking_speed;
};

#endif
