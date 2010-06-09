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
struct iMovable;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (Grab)

/**
 * This is a test property class.
 */
class celPcGrab : public scfImplementationExt1<celPcGrab, celPcCommon, iPcGrab>
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
  bool ReceiveMessage (csStringID msg_id, iMessageSender *sender, celData &ret,
      iCelParameterBlock *params);

  void SetState (GrabState state);
  GrabState GetState () const;

  void SetShimmyDirection (float sdir);
  float GetShimmyDirection () const;
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
  bool FindInterfaces ();
  // Needed when changing any of the shimmy variables using
  // the public Set functions
  void RecomputeShimmyAccel ();
  // if near edge is detected then freeze player! :)
  bool TryGrabLedge (const csVector3 &left, const csVector3 &right, const csVector3 &lhand, const csVector3 &rhand);

  // private impl functions
  GrabState currstate;
  // shimmy variables: time, distance, initial velocity
  float sdir, stime, sdist, sinvel, saccel;
  // left hand in player local space
  csVector3 left_hand_rel;

  csWeakRef<iPcLinearMovement> linmove;
  csWeakRef<iPcJump> jump;
  csWeakRef<iLedgeGroup> ledges;
  iMovable* movable;
  // only to keep track of when ledgegroup should be changed (when sector changes)
  iSector* currsector;
  // when grabbed then the ledge idx and point idx are set, else point idx = 0
  // it can never be validly 0 since only the right point is referenced (i.e 0->1 stored as 1)
  size_t c_ledge_id, c_ledge_point_id;

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

