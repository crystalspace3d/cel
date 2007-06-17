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

#include "cssysdef.h"
#include "iutil/objreg.h"
#include "plugins/propclass/moveactor/moveactor.h"
#include "plugins/propclass/moveactor/modes/analog.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (MoveActor, "pcmove.actor")

//---------------------------------------------------------------------------

csStringID celPcMoveActor::id_message = csInvalidStringID;

csHash<int, csStringID> constants;

PropertyHolder celPcMoveActor::propinfo;

celPcMoveActor::celPcMoveActor (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  if (id_message == csInvalidStringID)
    id_message = pl->FetchStringID ("cel.parameter.message");
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_message, "message");

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_print, "cel.action.Print");
  }

  // For properties.
  propinfo.SetCount (2);
  AddProperty (propid_counter, "cel.property.counter",
	CEL_DATA_LONG, false, "Print counter.", &counter);
  AddProperty (propid_max, "cel.property.max",
	CEL_DATA_LONG, false, "Max length.", 0);

  counter = 0;
  max = 0;

  currmode = (size_t)-1;
}

celPcMoveActor::~celPcMoveActor ()
{
  delete params;
}

bool celPcMoveActor::SetPropertyIndexed (int idx, long b)
{
  if (idx == propid_max)
  {
    max = b;
    return true;
  }
  return false;
}

bool celPcMoveActor::GetPropertyIndexed (int idx, long& l)
{
  if (idx == propid_max)
  {
    l = max;
    return true;
  }
  return false;
}

#define TEST_SERIAL 2

csPtr<iCelDataBuffer> celPcMoveActor::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  databuf->Add (int32 (counter));
  databuf->Add (int32 (max));
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMoveActor::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL) return false;

  counter = databuf->GetInt32 ();
  max = databuf->GetInt32 ();

  return true;
}

bool celPcMoveActor::PerformActionIndexed (int idx, iCelParameterBlock* params,
  celData& ret)
{
  switch (idx)
  {
    case action_print:
      {
        CEL_FETCH_STRING_PAR (msg,params,id_message);
        if (!p_msg) return false;
        Print (msg);
        return true;
      }
    default:
      return false;
  }
  return false;
}

void celPcMoveActor::Print (const char* msg)
{
  printf ("Print: %s\n", msg);
  fflush (stdout);
  iCelBehaviour* ble = entity->GetBehaviour ();
  if (ble)
  {
    celData ret;
    params->GetParameter (0).Set (msg);
    ble->SendMessage ("pcmove.actor_print", this, ret, params);
  }

  counter++;
  size_t l = strlen (msg);
  if (l > max) max = l;
}

//---------------------------------------------------------------------------

iPcmMoveActor::General* celPcMoveActor::GetMode (size_t idx)
{
  if ((idx == (size_t)-1) || (idx >= modes.GetSize ()))
    return 0;
  return modes[idx];
}
size_t celPcMoveActor::GetCurrentModeIndex () const
{
  return currmode;
}
size_t celPcMoveActor::AttachMode (iPcmMoveActor::General* mode)
{
  modes.Push (mode);
  mode->DecRef ();
  return (modes.GetSize () - 1);
}
size_t celPcMoveActor::AttachMode (iPcMoveActor::MODE_NAME modetype)
{
  switch (modetype)
  {
    case MODE_ANALOG:
      return AttachMode (new celMoveActorMode::Analog);
    case MODE_COUNT:
    default:
      return (size_t)-1;
  }
}
size_t celPcMoveActor::AttachMode (const char* modename)
{
  if (!strcmp (modename, "analog"))
    return AttachMode (iPcMoveActor::MODE_ANALOG);
  else
    return (size_t)-1;
}
bool celPcMoveActor::SetCurrentMode (size_t idx)
{
  if (idx >= modes.GetSize ())
    return false;
  currmode = idx;
  return true;
}
void celPcMoveActor::NextMode ()
{
  currmode++;
  if (currmode >= modes.GetSize ())
    currmode = 0;
}
void celPcMoveActor::PreviousMode ()
{
  if (currmode <= 0)
    currmode = modes.GetSize () - 1;
  else
    currmode--;
}
