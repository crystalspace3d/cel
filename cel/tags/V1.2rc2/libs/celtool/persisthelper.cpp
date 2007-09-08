/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "celtool/persisthelper.h"

//---------------------------------------------------------------------------

celStandardLocalEntitySet::celStandardLocalEntitySet (iCelPlLayer* pl)
  : scfImplementationType (this)
{
  celStandardLocalEntitySet::pl = pl;
}

celStandardLocalEntitySet::~celStandardLocalEntitySet ()
{
}

#define SERIAL_ENTITY 1
#define SERIAL_PC 1

csPtr<iCelDataBuffer> celStandardLocalEntitySet::SaveExternalEntity (
	iCelEntity* entity)
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (SERIAL_ENTITY);
  databuf->Add (entity->GetName ());
  return csPtr<iCelDataBuffer> (databuf);
}

iCelEntity* celStandardLocalEntitySet::FindExternalEntity (
	iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != SERIAL_ENTITY) return 0;
  const char* name = databuf->GetString ()->GetData ();
  if (!name) return 0;
  return pl->FindEntity (name);
}

csPtr<iCelDataBuffer> celStandardLocalEntitySet::SaveExternalPC (
	iCelPropertyClass* pc)
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (SERIAL_PC);
  databuf->Add (pc->GetEntity ()->GetName ());
  databuf->Add (pc->GetName ());
  databuf->Add (pc->GetTag ());
  return csPtr<iCelDataBuffer> (databuf);
}

iCelPropertyClass* celStandardLocalEntitySet::FindExternalPC (
	iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != SERIAL_PC) return 0;
  const char* entityname = databuf->GetString ()->GetData ();
  const char* pcname = databuf->GetString ()->GetData ();
  const char* tagname = databuf->GetString ()->GetData ();
  iCelEntity* entity = pl->FindEntity (entityname);
  if (!entity) return 0;
  iCelPropertyClassList* pclist = entity->GetPropertyClassList ();
  iCelPropertyClass* pc;
  if (tagname)
    pc = pclist->FindByNameAndTag (pcname, tagname);
  else
    pc = pclist->FindByName (pcname);

  return pc;
}

//---------------------------------------------------------------------------

celStandardPersistentDataList::celStandardPersistentDataList ()
  : scfImplementationType (this)
{
}

celStandardPersistentDataList::~celStandardPersistentDataList ()
{
  size_t i, count = data_list.GetSize ();
  for (i = 0; i < count; i++)
    delete data_list[i];
  data_list.DeleteAll ();
}

bool celStandardPersistentDataList::GetPersistentData (size_t idx, 
     csRef<iCelDataBuffer>& databuf, csString& pc_name, csString& pc_tag) const
{
  if (idx >= data_list.GetSize ())
    return false;
  
  celPersistentDataEntry* data_entry = data_list.Get (idx);
  databuf = data_entry->databuf;
  pc_name = data_entry->pc_name;
  pc_tag = data_entry->pc_tag;
  return true;
}

void celStandardPersistentDataList::AddPersistentData (
     csRef<iCelDataBuffer>& databuf, csString& pc_name, csString& pc_tag)
{
  celPersistentDataEntry* data_entry = new celPersistentDataEntry ();
  data_entry->databuf = databuf;
  data_entry->pc_name = pc_name;
  data_entry->pc_tag = pc_tag;
  data_list.Push (data_entry);
}

//---------------------------------------------------------------------------

