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

#ifndef __CEL_PF_ACTORLARAFACT__
#define __CEL_PF_ACTORLARAFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/actoranalog.h"

struct iCelEntity;
struct iObjectRegistry;
struct iPcMesh;
struct iPcLinearMovement;
struct iPcCamera;
struct iVirtualClock;

/**
 * Factory for actor lara.
 */
CEL_DECLARE_FACTORY (ActorAnalog)

/**
 * This is a actor lara property class.
 */
class celPcActorAnalog : public scfImplementationExt1<celPcActorAnalog,
  celPcCommon, iPcActorAnalog>
{
public:
  celPcActorAnalog (iObjectRegistry* object_reg);
  virtual ~celPcActorAnalog ();

  csPtr<iCelDataBuffer> Save ();
  bool Load (iCelDataBuffer* databuf);
  bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);

  // Callback to update the character
  void TickEveryFrame ();

  void SetAxis (size_t axis, float value);
  float GetAxis (size_t axis) const;
  void AddAxis (size_t axis, float value);
  void SetMovementSpeed (float speed);
  float GetMovementSpeed () const;
  void SetMovementAcceleration (float accel);
  float GetMovementAcceleration () const;
  void SetMovementDeceleration (float decel);
  float GetMovementDeceleration () const;
  void SetTurningSpeed (float speed);
  float GetTurningSpeed () const;
private:
  // Called regularly and any movement change
  void UpdateMovement ();

  // Check for any changes and update if necessary
  void FindSiblingPropertyClasses ();

  // Axis as set by client.
  csVector2 target_axis;

  float turnspeed;
  float movespeed, moveaccel, movedecel;

  // references to sibling property classes
  csWeakRef<iPcMesh> pcmesh;
  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcCamera> camera;

  csRef<iVirtualClock> vc;

  // For SendMessage parameters.
  static csStringID id_axis;
  static csStringID id_value;

  // For actions.
  enum actionids
  {
    action_setaxis = 0,
    action_addaxis,
    action_setmovespeed,
    action_setmoveaccel,
    action_setmovedecel,
    action_setturnspeed
  };

  // For properties.
  enum propids
  {
    propid_axisx = 0,
    propid_axisy,
    propid_movespeed,
    propid_moveaccel,
    propid_movedecel,
    propid_turnspeed
  };
  static PropertyHolder propinfo;
};

#endif // __CEL_PF_ACTORLARAFACT__
