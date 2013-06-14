/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_DYNMOVEFACT__
#define __CEL_PF_DYNMOVEFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/dynmove.h"
#include "propclass/mechsys.h"
#include "ivaria/bullet.h"

struct iCelEntity;
struct iObjectRegistry;
struct iMouseDriver;
struct iGraphics2D;
struct iPcDefaultCamera;

/**
 * Factory for dyn movement class.
 */
CEL_DECLARE_FACTORY (DynamicMove)

/**
 * This is a property class for dynamic movement.
 */
class celPcDynamicMove : public scfImplementationExt1<
	celPcDynamicMove, celPcCommon, iPcDynamicMove>
{
private:
  // For input messages
  static csStringID id_input_forward_down;
  static csStringID id_input_backward_down;
  static csStringID id_input_forward_up;
  static csStringID id_input_backward_up;
  static csStringID id_input_strafeleft_down;
  static csStringID id_input_straferight_down;
  static csStringID id_input_strafeleft_up;
  static csStringID id_input_straferight_up;
  static csStringID id_input_jump_down;
  static csStringID id_input_lookup_down;
  static csStringID id_input_lookup_up;
  static csStringID id_input_lookdown_down;
  static csStringID id_input_lookdown_up;
  static csStringID id_input_center_down;
  static csStringID id_input_rotateleft_down;
  static csStringID id_input_rotateleft_up;
  static csStringID id_input_rotateright_down;
  static csStringID id_input_rotateright_up;
  static csStringID id_input_mouselook;

#if NEW_PHYSICS
  csRef<CS::Physics::iRigidBody> body;
  csWeakRef<iPcMesh> pcmesh;
  void CreateBody ();
#else
  csWeakRef<iPcMechanicsObject> pcmechobj;
  csWeakRef<CS::Physics::Bullet::iRigidBody> bulletBody;
#endif

  csWeakRef<iPcDefaultCamera> pcdefcamera;
  void GetPCS ();
  void GetCam ();

  csRef<iGraphics2D> g2d;
  csRef<iMouseDriver> mouse;
  bool mouselookEnabled;

  // For properties.
  enum propids
  {
    propid_speed = 0,
    propid_jumpspeed,
    propid_rotspeed,
    propid_correctup,
  };
  static PropertyHolder propinfo;

  float speed;
  float jumpspeed;
  float rotspeed;
  bool correctup;

  csVector3 curspeed;

public:
  celPcDynamicMove (iObjectRegistry* object_reg);
  virtual ~celPcDynamicMove ();
  virtual void SetEntity (iCelEntity* entity);

#if NEW_PHYSICS
  virtual void Move (CS::Physics::iPhysicalSector* sector, const csReversibleTransform& trans);
#else
  virtual void Move (const csReversibleTransform& trans);
#endif

  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);

  // For iMessageReceiver.
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);

  virtual void TickEveryFrame ();

  // Override SetProperty from celPcCommon in order to provide support
  // for the 'correctup' property.
  virtual bool SetPropertyIndexed (int idx, bool b);
  virtual bool GetPropertyIndexed (int, bool& b);

  virtual void EnableMouselook (bool enable);
  virtual bool IsMouselookEnabled () const { return mouselookEnabled; }
};

#endif // __CEL_PF_DYNMOVEFACT__

