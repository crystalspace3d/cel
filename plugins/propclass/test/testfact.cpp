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
}

celPcTest::~celPcTest ()
{
}

#define TEST_SERIAL 1

csPtr<iCelDataBuffer> celPcTest::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  databuf->SetDataCount (0);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTest::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL) return false;
  if (databuf->GetDataCount () != 0) return false;
  return true;
}

void celPcTest::PcTest::Print (const char* msg)
{
  printf ("Print: %s\n", msg);
  fflush (stdout);
  CS_ASSERT (scfParent->GetEntity () != 0);
  iCelBehaviour* ble = scfParent->GetEntity ()->GetBehaviour ();
  CS_ASSERT (ble != 0);
  celData ret;
  ble->SendMessage ("pctest_print", ret, 0);
}

//---------------------------------------------------------------------------

