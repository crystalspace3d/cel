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
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"

#include "plugins/tools/quests/trig_entersector.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celEnterSectorTriggerType)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerType)
SCF_IMPLEMENT_IBASE_END

celEnterSectorTriggerType::celEnterSectorTriggerType (
	iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (0);
  celEnterSectorTriggerType::object_reg = object_reg;
}

celEnterSectorTriggerType::~celEnterSectorTriggerType ()
{
  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestTriggerFactory> celEnterSectorTriggerType::CreateTriggerFactory ()
{
  celEnterSectorTriggerFactory* fact = new
  	celEnterSectorTriggerFactory ();
  return fact;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celEnterSectorTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iEnterSectorQuestTriggerFactory)
SCF_IMPLEMENT_IBASE_END

celEnterSectorTriggerFactory::celEnterSectorTriggerFactory ()
{
  SCF_CONSTRUCT_IBASE (0);
  entity_name = 0;
  sector_name = 0;
}

celEnterSectorTriggerFactory::~celEnterSectorTriggerFactory ()
{
  delete[] entity_name;
  delete[] sector_name;
  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestTrigger> celEnterSectorTriggerFactory::CreateTrigger (
    const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params)
{
  celEnterSectorTrigger* trig = new celEnterSectorTrigger ();
  return trig;
}

bool celEnterSectorTriggerFactory::Load (iDocumentNode* node)
{
  return false;
}

void celEnterSectorTriggerFactory::SetEntityName (const char* entity_name)
{
  if (celEnterSectorTriggerFactory::entity_name == entity_name) return;
  delete[] celEnterSectorTriggerFactory::entity_name;
  celEnterSectorTriggerFactory::entity_name = csStrNew (entity_name);
}

void celEnterSectorTriggerFactory::SetSectorName (const char* sector_name)
{
  if (celEnterSectorTriggerFactory::sector_name == sector_name) return;
  delete[] celEnterSectorTriggerFactory::sector_name;
  celEnterSectorTriggerFactory::sector_name = csStrNew (sector_name);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celEnterSectorTrigger)
  SCF_IMPLEMENTS_INTERFACE (iQuestTrigger)
SCF_IMPLEMENT_IBASE_END

celEnterSectorTrigger::celEnterSectorTrigger ()
{
  SCF_CONSTRUCT_IBASE (0);
}

celEnterSectorTrigger::~celEnterSectorTrigger ()
{
  SCF_DESTRUCT_IBASE ();
}

void celEnterSectorTrigger::RegisterCallback (iQuestTriggerCallback* callback)
{
  celEnterSectorTrigger::callback = callback;
}

void celEnterSectorTrigger::ClearCallback ()
{
  callback = 0;
}

void celEnterSectorTrigger::NewSector (iCamera* camera, iSector* sector)
{
  callback->TriggerFired ((iQuestTrigger*)this);
}

//---------------------------------------------------------------------------

