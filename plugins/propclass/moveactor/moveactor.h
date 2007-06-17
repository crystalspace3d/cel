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

#ifndef __CEL_PF_MOVEACTORFACT__
#define __CEL_PF_MOVEACTORFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "csutil/refarr.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/moveactor.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for move.actor.
 */
CEL_DECLARE_FACTORY (MoveActor)

/**
 * This is a test property class.
 */
class celPcMoveActor : public scfImplementationExt1<
  celPcMoveActor, celPcCommon, iPcMoveActor>
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

  csRefArray<iPcmMoveActor::General> modes;
  size_t currmode;

  // Other fields.
  int counter;
  size_t max;

public:
  celPcMoveActor (iObjectRegistry* object_reg);
  virtual ~celPcMoveActor ();

  virtual void Print (const char* msg);

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);

  // Override SetProperty from celPcCommon in order to provide support
  // for the 'max' property.
  virtual bool SetPropertyIndexed (int idx, long b);
  virtual bool GetPropertyIndexed (int, long&);

  // mode related
  virtual iPcmMoveActor::General* GetMode (size_t idx = (size_t)-1);
  virtual size_t GetCurrentModeIndex () const;
  virtual size_t AttachMode (iPcmMoveActor::General* mode);
  virtual size_t AttachMode (iPcMoveActor::MODE_NAME modetype);
  virtual size_t AttachMode (const char* modename);
  virtual bool SetCurrentMode (size_t idx);
  virtual void NextMode ();
  virtual void PreviousMode ();
};

#endif // __CEL_PF_MOVEACTORFACT__
