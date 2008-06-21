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
class celPcJump : public scfImplementationExt1<celPcJump, celPcCommon, iPcJump>
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
  void DoubleJump ();
  void Freeze (bool frozen);
  void Glide ();

  bool IsJumping () const;
  bool IsDoubleJumping () const;
  bool IsFrozen ();

  void SetJumpSpeed (float spd);
  float GetJumpSpeed () const;
  void SetDoubleJumpSpeed (float spd);
  float GetDoubleJumpSpeed () const;
  void SetDoubleJumpSensitivity (float sens);
  float GetDoubleJumpSensitivity () const;
  void SetGlideSpeed (float spd);
  float GetGlideSpeed () const;
  void SetGlideSensitivity (float sens);
  float GetGlideSensitivity () const;

private:
  // Called regularly and any movement change
  void UpdateMovement ();

  // Check for any changes and update if necessary
  bool FindSiblingPropertyClasses ();

  bool jumping, doublejumping;
  float jumpspeed, doublejumpspeed;
  csWeakRef<iPcLinearMovement> linmove;

  // For actions.
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

  // message saying that player landed
  csRef<iMessageDispatcher> dispatcher_landed;
};

#endif // __CEL_PF_JUMPOVERFACTORIES__

