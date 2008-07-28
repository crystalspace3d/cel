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

#ifndef __CEL_PF_GRABBINSTUFF__
#define __CEL_PF_GRABBINSTUFF__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/grab.h"
#include "tools/ledges.h"

struct iCelEntity;
struct iObjectRegistry;
struct iPcLinearMovement;
struct iPcJump;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (Grab)

/**
 * This is a test property class.
 */
class celPcGrab : public scfImplementationExt2<celPcGrab, celPcCommon, iPcGrab, iMessageReceiver>
{
public:
  celPcGrab (iObjectRegistry* object_reg);
  virtual ~celPcGrab ();

  csPtr<iCelDataBuffer> Save ();
  bool Load (iCelDataBuffer* databuf);
  bool PerformActionIndexed (int idx, iCelParameterBlock* params, celData& ret);

  // Callback to update the character
  void TickEveryFrame ();
  // only used for getting jump.started events
  bool ReceiveMessage (csStringID msg_id, iMessageSender *sender, celData &ret, iCelParameterBlock *params);

  void SetState (GrabState state);
  GrabState GetState () const;

  void SetShimmyTime (float time);
  float GetShimmyTime () const;
  void SetShimmyDistance (float dist);
  float GetShimmyDistance () const;
  void SetShimmyInitialVelocity (float vel);
  float GetShimmyInitialVelocity () const;

private:
  // Called regularly and any movement change
  void UpdateMovement ();
  // try to perform a grab if possible
  void AttemptGrab ();
  // Check for any changes and update if necessary
  bool FindSiblingPropertyClasses ();
  // Needed when changing any of the shimmy variables using
  // the public Set functions
  void RecomputeShimmyAccel ();

  // private impl functions
  GrabState currstate;
  // shimmy variables: time, distance, initial velocity
  float stime, sdist, sinvel, saccel;

  csWeakRef<iPcLinearMovement> linmove;
  csWeakRef<iPcJump> jump;
  csWeakRef<iLedgeGroup> ledges;

  // For actions.
  enum actionids
  {
  };

  // For properties.
  enum propids
  {
  };
  static PropertyHolder propinfo;
};

#endif // __CEL_PF_GRABBINSTUFF__

