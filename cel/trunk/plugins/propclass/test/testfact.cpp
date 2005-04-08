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
#include "csutil/debug.h"
#include "plugins/propclass/test/testfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Test, "pctest")

//---------------------------------------------------------------------------

csStringID celPcTest::id_message = csInvalidStringID;
csStringID celPcTest::action_print = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcTest)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTest)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTest::PcTest)
  SCF_IMPLEMENTS_INTERFACE (iPcTest)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcTest::celPcTest (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTest);

  // For SendMessage parameters.
  if (id_message == csInvalidStringID)
    id_message = pl->FetchStringID ("cel.parameter.message");
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_message, "message");

  // For PerformAction.
  if (action_print == csInvalidStringID)
    action_print = pl->FetchStringID ("cel.action.Print");

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_counter] = &counter;	// Automatically handled.
  propdata[propid_max] = 0;		// Handled in this class.
}

celPcTest::~celPcTest ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcTest);
}

Property* celPcTest::properties = 0;
int celPcTest::propertycount = 0;

void celPcTest::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT (pl != 0);

    propertycount = 2;
    properties = new Property[propertycount];

    properties[propid_counter].id = pl->FetchStringID (
    	"cel.property.counter");
    properties[propid_counter].datatype = CEL_DATA_LONG;
    properties[propid_counter].readonly = false;
    properties[propid_counter].desc = "Print counter.";

    properties[propid_counter].id = pl->FetchStringID (
    	"cel.property.max");
    properties[propid_counter].datatype = CEL_DATA_LONG;
    properties[propid_counter].readonly = false;
    properties[propid_counter].desc = "Max length.";
  }
}

bool celPcTest::SetProperty (csStringID propertyId, long b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_max].id)
  {
    max = b;
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

long celPcTest::GetPropertyLong (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_max].id)
  {
    return max;
  }
  else
  {
    return celPcCommon::GetPropertyLong (propertyId);
  }
}

#define TEST_SERIAL 2

csPtr<iCelDataBuffer> celPcTest::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  databuf->SetDataCount (2);
  databuf->GetData (0)->Set (CS_STATIC_CAST(int32,counter));
  databuf->GetData (1)->Set (CS_STATIC_CAST(int32,max));
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTest::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL) return false;
  if (databuf->GetDataCount () != 2) return false;

  celData* cd;
  cd = databuf->GetData (0); if (!cd) return false;
  counter = cd->value.l;
  cd = databuf->GetData (1); if (!cd) return false;
  max = cd->value.l;

  return true;
}

bool celPcTest::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_print)
  {
    CEL_FETCH_STRING_PAR (msg,params,id_message);
    if (!p_msg) return false;
    Print (msg);
    return true;
  }
  return false;
}

void celPcTest::Print (const char* msg)
{
  printf ("Print: %s\n", msg);
  fflush (stdout);
  iCelBehaviour* ble = entity->GetBehaviour ();
  CS_ASSERT (ble != 0);
  celData ret;
  params->GetParameter (0).Set (msg);
  ble->SendMessage ("pctest_print", this, ret, params);

  counter++;
  size_t l = strlen (msg);
  if (l > max) max = l;
}

//---------------------------------------------------------------------------

