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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "pf/test/testfact.h"
#include "pl/entity.h"
#include "bl/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPfTest)

SCF_EXPORT_CLASS_TABLE (pftest)
  SCF_EXPORT_CLASS (celPfTest, "cel.pcfactory.test",
  	"CEL Test Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfTest)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfTest::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfTest::celPfTest (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfTest::~celPfTest ()
{
}

bool celPfTest::Initialize (iObjectRegistry* /*object_reg*/)
{
  return true;
}

iCelPropertyClass* celPfTest::CreatePropertyClass (const char* type)
{
  if (strcmp (type, "pctest")) return NULL;
  return new celPcTest ();
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcTest)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTest)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTest::PcTest)
  SCF_IMPLEMENTS_INTERFACE (iPcTest)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcTest::celPcTest ()
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTest);
}

celPcTest::~celPcTest ()
{
}

void celPcTest::SetEntity (iCelEntity* entity)
{
  celPcTest::entity = entity;
}

void celPcTest::PcTest::Print (const char* msg)
{
  printf ("Print: %s\n", msg);
  fflush (stdout);
  CS_ASSERT (scfParent->GetEntity () != NULL);
  iCelBehaviour* ble = scfParent->GetEntity ()->GetBehaviour ();
  CS_ASSERT (ble != NULL);
  ble->SendMessage ("print");
}

//---------------------------------------------------------------------------

