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
#include "plugins/propclass/test/testfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (Test, "pcmisc.test")

//---------------------------------------------------------------------------

csStringID celPcTest::id_message = csInvalidStringID;

PropertyHolder celPcTest::propinfo;

celPcTest::celPcTest (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  if (id_message == csInvalidStringID)
    id_message = pl->FetchStringID ("message");
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_message);

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.test.action.");
    AddAction (action_print, "Print");
  }

  // For properties.
  propinfo.SetCount (2);
  AddProperty (propid_counter, "counter",
	CEL_DATA_LONG, false, "Print counter.", &counter);
  AddProperty (propid_max, "max",
	CEL_DATA_LONG, false, "Max length.", 0);

  counter = 0;
  max = 0;
}

celPcTest::~celPcTest ()
{
  delete params;
}

bool celPcTest::SetPropertyIndexed (int idx, long b)
{
  if (idx == propid_max)
  {
    max = b;
    return true;
  }
  return false;
}

bool celPcTest::GetPropertyIndexed (int idx, long& l)
{
  if (idx == propid_max)
  {
    l = max;
    return true;
  }
  return false;
}

bool celPcTest::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_print:
      {
	csString msg;
	if (!Fetch (msg, params, id_message)) return false;
        Print (msg);
        return true;
      }
    default:
      return false;
  }
  return false;
}

void celPcTest::Print (const char* msg)
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
      CreateMessageDispatcher (this, pl->FetchStringID ("cel.test.print"));
    if (!dispatcher_print) return;
  }
  dispatcher_print->SendMessage (params);

  counter++;
  size_t l = strlen (msg);
  if (l > max) max = l;
}

//---------------------------------------------------------------------------

