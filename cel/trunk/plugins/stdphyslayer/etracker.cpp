/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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
#include "csutil/util.h"
#include "csutil/debug.h"
#include "plugins/stdphyslayer/etracker.h"
#include "plugins/stdphyslayer/entity.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celEntityTracker)
  SCF_IMPLEMENTS_INTERFACE (iCelEntityTracker)
SCF_IMPLEMENT_IBASE_END

celEntityTracker::celEntityTracker (celPlLayer* pl, const char* name)
{
  SCF_CONSTRUCT_IBASE (0);
  celEntityTracker::pl = pl;
  celEntityTracker::name = csStrNew (name);
}

celEntityTracker::~celEntityTracker ()
{
  SCF_DESTRUCT_IBASE ();
}

bool celEntityTracker::AddEntity (iCelEntity* entity)
{
  entities.Add (entity);
  return true;
}

void celEntityTracker::RemoveEntity (iCelEntity* entity)
{
  entities.Delete (entity);
}

void celEntityTracker::RemoveEntities ()
{
  entities.DeleteAll ();
}

csPtr<iCelEntityList> celEntityTracker::FindNearbyEntities (iSector* sector,
  	const csVector3& pos, float radius)
{
  return 0;
}

//---------------------------------------------------------------------------

