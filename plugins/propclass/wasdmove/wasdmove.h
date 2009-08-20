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

#ifndef __CEL_PF_WASDMOVEFACT__
#define __CEL_PF_WASDMOVEFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/wasdmove.h"
#include "propclass/analogmotion.h"
#include "propclass/jump.h"
#include "propclass/grab.h"
#include "propclass/linmove.h"
#include "propclass/timer.h"
#include "propclass/cameras/tracking.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for wasd movement class.
 */
CEL_DECLARE_FACTORY (WasdMove)

/**
 * This is a property class for wasd movement.
 */
class celPcWasdMove : public scfImplementationExt2<
	celPcWasdMove, celPcCommon, iPcWasdMove, iMessageReceiver>
{
private:
  // For SendMessage parameters.
  static csStringID id_message;
  celOneParameterBlock* params;

  // For actions.
  enum actionids
  {
    action_print = 0
  };

  // For properties.
  enum propids
  {
    propid_counter = 0,
    propid_max
  };
  static PropertyHolder propinfo;

  // For input messages
  static csStringID id_input_mouseaxis0;
  static csStringID id_input_joyaxis0;
  static csStringID id_input_joyaxis1;
  static csStringID id_input_left_down;
  static csStringID id_input_left_up;
  static csStringID id_input_right_down;
  static csStringID id_input_right_up;
  static csStringID id_input_up_up;
  static csStringID id_input_up_down;
  static csStringID id_input_down_up;
  static csStringID id_input_down_down;
  static csStringID id_input_jump_up;
  static csStringID id_input_jump_down;
  static csStringID id_input_freeze_down;
  static csStringID id_input_roll_down;
  static csStringID id_input_roll_up;
  static csStringID id_input_showstates_up;
  static csStringID id_input_camleft_down;
  static csStringID id_input_camleft_up;
  static csStringID id_input_camright_down;
  static csStringID id_input_camright_up;
  static csStringID id_input_camdown_down;
  static csStringID id_input_camdown_up;
  static csStringID id_input_camup_up;
  static csStringID id_input_camup_down;
  static csStringID id_input_ready_down;
  static csStringID id_input_ready_up;
  static csStringID id_input_lockon_down;
  static csStringID id_input_lockon_up;
  static csStringID id_input_resetcam_down;
  static csStringID id_input_tiltcam;
  static csStringID id_input_pancam;

  // For move messages
  static csStringID id_move_jump_landed;
  static csStringID id_move_jump_started;

  // For timer messages
  static csStringID id_timer_wakeup;

  // For parameters
  static csStringID id_param_x;
  static csStringID id_param_y;
  static csStringID id_param_value;

  // Other fields.
  int counter;
  size_t max;

  csRef<iMessageDispatcher> dispatcher_print;

  csWeakRef<iPcTrackingCamera> trackcam;
  csWeakRef<iPcAnalogMotion> pcactor;
  csWeakRef<iPcJump> jump;
  csWeakRef<iPcGrab> grab;
  csWeakRef<iPcLinearMovement> linmove;
  csWeakRef<iPcTimer> timer;

  bool try_grab_jump;

  void GetPCS ();

public:
  celPcWasdMove (iObjectRegistry* object_reg);
  virtual ~celPcWasdMove ();
  virtual void SetEntity (iCelEntity* entity);

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);

  virtual bool SetPropertyIndexed (int idx, long b);
  virtual bool GetPropertyIndexed (int, long&);

  // For iMessageReceiver.
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
};

#endif // __CEL_PF_WASDMOVEFACT__

