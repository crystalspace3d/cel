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

#ifndef __CEL_PF_WHEELEDMOVEFACT__
#define __CEL_PF_WHEELEDMOVEFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/wheeledmove.h"
#include "propclass/wheeled.h"
#include "propclass/meshdeform.h"
#include "propclass/mechsys.h"
#include "propclass/defcam.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for wheeled movement class.
 */
CEL_DECLARE_FACTORY (WheeledMove)

/**
 * This is a property class for wheeled movement.
 */
class celPcWheeledMove : public scfImplementationExt1<
	celPcWheeledMove, celPcCommon, iPcWheeledMove>
{
private:
  // For input PC messages
  static csStringID id_input_accelerate_up;
  static csStringID id_input_accelerate_down;
  static csStringID id_input_reverse_up;
  static csStringID id_input_reverse_down;
  static csStringID id_input_steerleft_up;
  static csStringID id_input_steerleft_down;
  static csStringID id_input_steerright_up;
  static csStringID id_input_steerright_down;
  static csStringID id_input_handbrake_up;
  static csStringID id_input_handbrake_down;
  static csStringID id_input_lookup_up;
  static csStringID id_input_lookup_down;
  static csStringID id_input_lookdown_up;
  static csStringID id_input_lookdown_down;
  static csStringID id_input_center_down;

  // For mechanics PC messages
  static csStringID id_mech_collision;
  static csStringID id_mech_par_position;
  static csStringID id_mech_par_normal;
  static csStringID id_mech_par_depth;

  csWeakRef<iPcMechanicsObject> pcmechobj;
  csWeakRef<iPcMeshDeform> pcmeshdeform;
  csWeakRef<iPcWheeled> pcwheeled;
  csWeakRef<iPcDefaultCamera> pcdefcamera;
  void GetPCS ();

public:
  celPcWheeledMove (iObjectRegistry* object_reg);
  virtual ~celPcWheeledMove ();
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

#endif // __CEL_PF_WHEELEDMOVEFACT__

