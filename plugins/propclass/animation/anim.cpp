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
#include "plugins/propclass/animation/anim.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Animation, "pcobject.mesh.animation")

//---------------------------------------------------------------------------

csStringID celPcAnimation::id_message = csInvalidStringID;

csHash<int, csStringID> constants;

PropertyHolder celPcAnimation::propinfo;

celPcAnimation::celPcAnimation (iObjectRegistry* object_reg)
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
}

celPcAnimation::~celPcAnimation ()
{
  delete params;
}

bool celPcAnimation::SetPropertyIndexed (int idx, long b)
{
  if (idx == propid_max)
  {
    max = b;
    return true;
  }
  return false;
}

bool celPcAnimation::GetPropertyIndexed (int idx, long& l)
{
  if (idx == propid_max)
  {
    l = max;
    return true;
  }
  return false;
}

#define TEST_SERIAL 2

csPtr<iCelDataBuffer> celPcAnimation::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  databuf->Add (int32 (counter));
  databuf->Add (int32 (max));
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcAnimation::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL) return false;

  counter = databuf->GetInt32 ();
  max = databuf->GetInt32 ();

  return true;
}

bool celPcAnimation::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
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

void celPcAnimation::Print (const char* msg)
{
  printf ("Print: %s\n", msg);
  fflush (stdout);
  params->GetParameter (0).Set (msg);
  iCelBehaviour* ble = entity->GetBehaviour ();
  if (ble)
  {
    celData ret;
    ble->SendMessage ("pcmisc.test_print", this, ret, params);
  }

  if (!dispatcher_print)
  {
    dispatcher_print = entity->QueryMessageChannel ()->
      CreateMessageDispatcher (this, "cel.test.print");
    if (!dispatcher_print) return;
  }
  dispatcher_print->SendMessage (params);

  counter++;
  size_t l = strlen (msg);
  if (l > max) max = l;
}

bool celPcAnimation::Setup ()
{
  csRef<Skeleton::iGraveyard> skelgrave = csQueryRegistry<Skeleton::iGraveyard> (object_reg);
  if (!skelgrave)
  {
    puts ("Can't find the graveyard!");
    return false;
  }
  skel = skelgrave->CreateSkeleton ("taki", "amir");
  skelgrave->Debug ();
  return true;
}

//---------------------------------------------------------------------------

