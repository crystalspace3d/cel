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
#include "iutil/objreg.h"
#include "csutil/debug.h"
#include "pf/common/stdpcimp.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/propchg.h"
#include "pl/persist.h"
#include "bl/behave.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcCommon)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
SCF_IMPLEMENT_IBASE_END

celPcCommon::celPcCommon (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  celPcCommon::object_reg = object_reg;
  DG_ADDI (this, "celPcCommon()");
  entity = NULL;
}

celPcCommon::~celPcCommon ()
{
  DG_REM (this);
}

void celPcCommon::SetEntity (iCelEntity* entity)
{
  celPcCommon::entity = entity;
}

void celPcCommon::FirePropertyChangeCallback (int propertyId)
{
  int i;
  for (i = 0 ; i < callbacks.Length () ; i++)
  {
    iCelPropertyChangeCallback* cb = (iCelPropertyChangeCallback*)callbacks[i];
    cb->PropertyChanged (propertyId, this);
  }
}

bool celPcCommon::AddPropertyChangeCallback (iCelPropertyChangeCallback* cb)
{
  int idx = callbacks.Find (cb);
  if (idx != -1) return false;
  callbacks.Push (cb);
  return true;
}

bool celPcCommon::RemovePropertyChangeCallback (
  	iCelPropertyChangeCallback* cb)
{
  int idx = callbacks.Find (cb);
  if (idx == -1) return false;
  callbacks.Delete (idx);
  return true;
}

//---------------------------------------------------------------------------

