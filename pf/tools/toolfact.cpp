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
#include "pf/tools/toolfact.h"
#include "pl/entity.h"
#include "bl/behave.h"
#include "csutil/util.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPfTools)

SCF_EXPORT_CLASS_TABLE (pftools)
  SCF_EXPORT_CLASS (celPfTools, "cel.pcfactory.tools",
  	"CEL Tools Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfTools)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfTools::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfTools::celPfTools (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfTools::~celPfTools ()
{
}

bool celPfTools::Initialize (iObjectRegistry* /*object_reg*/)
{
  return true;
}

iCelPropertyClass* celPfTools::CreatePropertyClass (const char* type)
{
  if (strcmp (type, "pctooltip")) return NULL;
  return new celPcTooltip ();
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcTooltip)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTooltip)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTooltip::PcTooltip)
  SCF_IMPLEMENTS_INTERFACE (iPcTooltip)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcTooltip::celPcTooltip ()
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTooltip);
  visible = false;
  text = NULL;
}

celPcTooltip::~celPcTooltip ()
{
  delete[] text;
}

void celPcTooltip::SetEntity (iCelEntity* entity)
{
  celPcTooltip::entity = entity;
}

void celPcTooltip::SetText (const char* t)
{
  delete[] t;
  text = csStrNew (t);
}

//---------------------------------------------------------------------------

