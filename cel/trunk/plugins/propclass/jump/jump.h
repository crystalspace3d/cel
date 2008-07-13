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

#ifndef __CEL_PF_JUMPOVERFACTORIES__
#define __CEL_PF_JUMPOVERFACTORIES__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/jump.h"

struct iCelEntity;
struct iObjectRegistry;
struct iPcLinearMovement;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (Jump)

/**
 * This is a test property class.
 */
class celPcJump : public scfImplementationExt2<celPcJump, celPcCommon, iPcJump, iMessageReceiver>
{
public:
  celPcJump (iObjectRegistry* object_reg);
  virtual ~celPcJump ();

  csPtr<iCelDataBuffer> Save ();
  bool Load (iCelDataBuffer* databuf);
  bool PerformActionIndexed (int idx, iCelParameterBlock* params, celData& ret);

  // Callback to update the character
  void TickEveryFrame ();

  void Jump ();
  void Freeze (bool frozen);
  State GetState () const;

  void GlideTurn (GlideTurnDirection gtur);
  void GlidePitch (GlidePitchDirection gpit);
  void SetGlideGravity (float glidegrav);
  float GetGlideGravity () const;
  void SetGlidePitchLimit (float gptlim);
  float GetGlidePitchLimit () const;
  void SetGlidePitchSpeed (float gptspd);
  float GetGlidePitchSpeed () const;
  void SetGlideTurnSpeed (float gtrspd);
  float GetGlideTurnSpeed () const;

  void SetJumpSpeed (float spd);
  float GetJumpSpeed () const;
  void SetJumpHeight (float height);
  float GetJumpHeight () const;
  csTicks GetAirTime () const;
  void SetDoubleJumpSpeed (float spd);
  float GetDoubleJumpSpeed () const;
  void SetGravity (float grav);
  float GetGravity () const;
  void SetFixedJump (bool fixjump);
  bool GetFixedJump () const;

  // only used for getting move.impossible events
  bool ReceiveMessage (csStringID msg_id, iMessageSender *sender, celData &ret, iCelParameterBlock *params);

private:
  // Called regularly and any movement change
  void UpdateMovement ();
  // Check for any changes and update if necessary
  bool FindSiblingPropertyClasses ();
  void DoJump ();
  // control for when gliding
  void GlideControl ();

  State currstate, startact;
  bool falling;
  float jumpspeed, doublejumpspeed;
  float gravity;
  bool fixedjump;

  // glide variables
  GlideTurnDirection g_turn;
  GlidePitchDirection g_pitch;
  float glide_gravity, glide_pitch_limit, glide_pitch_speed, glide_turn_speed;
  // forward speed at the beginning of the glide
  float glide_startspeed;

  csWeakRef<iPcLinearMovement> linmove;

  // For states.
  enum actionids
  {
    action_jump = 0
  };

  // For properties.
  enum propids
  {
    propid_isjumping = 0,
    propid_jumpspeed
  };
  static PropertyHolder propinfo;

  struct
  {
    // messages saying that player started jump, and landed
    csRef<iMessageDispatcher> started, landed;
  } dispatcher;
};

#endif // __CEL_PF_JUMPOVERFACTORIES__

