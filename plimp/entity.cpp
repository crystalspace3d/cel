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
#include "csutil/util.h"
#include "plimp/entity.h"
#include "plimp/propclas.h"
#include "bl/behave.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celEntity)
  SCF_IMPLEMENTS_INTERFACE (iCelEntity)
SCF_IMPLEMENT_IBASE_END

celEntity::celEntity ()
{
  SCF_CONSTRUCT_IBASE (NULL);
  name = NULL;
  plist = new celPropertyClassList (this);
  behaviour = NULL;
}

celEntity::~celEntity ()
{
  delete[] name;
  delete plist;
  if (behaviour) behaviour->DecRef ();
}

void celEntity::SetName (const char* n)
{
  delete[] name;
  if (n)
    name = csStrNew (n);
  else
    name = NULL;
}

void celEntity::SetBehaviour (iCelBehaviour* ent)
{
  if (ent) ent->IncRef ();
  if (behaviour) behaviour->DecRef ();
  behaviour = ent;
}

iCelPropertyClassList* celEntity::GetPropertyClassList ()
{
  return (iCelPropertyClassList*)plist;
}

