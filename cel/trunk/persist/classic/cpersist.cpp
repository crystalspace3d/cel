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
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/propclas.h"
#include "bl/behave.h"
#include "persist/classic/cpersist.h"
#include "cssys/csendian.h"
#include "csutil/memfile.h"
#include "iutil/vfs.h"
#include "iutil/objreg.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPersistClassic)

SCF_EXPORT_CLASS_TABLE (cpersist)
  SCF_EXPORT_CLASS (celPersistClassic, "cel.persistance.classic",
  	"CEL Classic Persistance Module")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPersistClassic)
  SCF_IMPLEMENTS_INTERFACE (iCelPersistance)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPersistClassic::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPersistClassic::celPersistClassic (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPersistClassic::~celPersistClassic ()
{
}

bool celPersistClassic::Initialize (iObjectRegistry* object_reg)
{
  celPersistClassic::object_reg = object_reg;
  return true;
}

iCelEntity* celPersistClassic::LoadEntity (const char* name)
{
  return NULL;
}

bool celPersistClassic::WriteMarker (iFile* f, const char* s)
{
  if (!f->Write (s, 4)) return false;
  return true;
}

bool celPersistClassic::WriteString (iFile* f, const char* s)
{
  int ll = s ? strlen (s) : 0;
  uint16 l = convert_endian (ll);
  if (!f->Write ((const char*)&l, 2)) return false;
  if (s && !f->Write (s, ll)) return false;
  return true;
}

bool celPersistClassic::Write (iFile* f, iCelPropertyClass* pc)
{
  if (!pc)
  {
    // NULL pc.
    if (!WriteMarker (f, "PCL0")) return false;
    return true;
  }
  if (!WriteMarker (f, "PCLI")) return false;

  if (!WriteString (f, pc->GetName ())) return false;
  if (!WriteString (f, pc->GetFactoryName ())) return false;
  iCelDataBuffer* db = pc->Save ();
  if (!db) return false;
  if (!Write (f, db))
  {
    db->DecRef ();
    return false;
  }
  db->DecRef ();
  return true;
}

bool celPersistClassic::Write (iFile* f, iCelEntity* entity)
{
  if (!entity)
  {
    // NULL entity.
    if (!WriteMarker (f, "ENT0")) return false;
    return true;
  }
  if (!WriteMarker (f, "ENTI")) return false;

  if (!WriteString (f, entity->GetName ())) return false;
  if (!WriteString (f, entity->GetBehaviour () ?
  	entity->GetBehaviour ()->GetName () : NULL)) return false;

  iCelPropertyClassList* pl = entity->GetPropertyClassList ();
  uint16 c = convert_endian (pl->GetCount ());
  if (!f->Write ((const char*)&c, 2)) return false;
  int i;
  for (i = 0 ; i < pl->GetCount () ; i++)
  {
    if (!Write (f, pl->Get (i)))
      return false;
  }
  return true;
}

bool celPersistClassic::Write (iFile* f, iCelDataBuffer* db)
{
  long ser = convert_endian (db->GetSerialNumber ());
  if (!f->Write ((const char*)&ser, 4)) return false;
  uint16 cnt = convert_endian (db->GetDataCount ());
  if (!f->Write ((const char*)&cnt, 2)) return false;
  int i;
  for (i = 0 ; i < db->GetDataCount () ; i++)
  {
    if (!Write (f, db->GetData (i)))
      return false;
  }
  return true;
}

bool celPersistClassic::Write (iFile* f, celData* data)
{
  switch (data->type)
  {
    case CEL_DATA_NONE:
      CS_ASSERT (false);
      break;
    case CEL_DATA_BOOL:
      {
        uint8 v = (uint8)data->value.bo;
        if (!f->Write ((const char*)&v, 1)) return false;
      }
      break;
    case CEL_DATA_BYTE:
      if (!f->Write (&(data->value.b), 1)) return false;
      break;
    case CEL_DATA_WORD:
      {
        int16 v = convert_endian (data->value.w);
        if (!f->Write ((const char*)&v, 2)) return false;
      }
      break;
    case CEL_DATA_LONG:
      {
        int32 v = convert_endian (data->value.l);
        if (!f->Write ((const char*)&v, 4)) return false;
      }
      break;
    case CEL_DATA_UBYTE:
      if (!f->Write ((const char*)&(data->value.ub), 1)) return false;
      break;
    case CEL_DATA_UWORD:
      {
        uint16 v = convert_endian (data->value.uw);
        if (!f->Write ((const char*)&v, 2)) return false;
      }
      break;
    case CEL_DATA_ULONG:
      {
        uint32 v = convert_endian ((unsigned long)data->value.ul);
        if (!f->Write ((const char*)&v, 4)) return false;
      }
      break;
    case CEL_DATA_FLOAT:
      {
        float v = convert_endian (data->value.f);
        if (!f->Write ((const char*)&v, 4)) return false;
      }
      break;
    case CEL_DATA_STRING:
      if (!WriteString (f, data->value.s)) return false;
      break;
    case CEL_DATA_PCLASS:
      if (!Write (f, data->value.pc)) return false;
      break;
    case CEL_DATA_ENTITY:
      if (!Write (f, data->value.ent)) return false;
      break;
    case CEL_DATA_BUFFER:
      if (!Write (f, data->value.db)) return false;
      break;
    default:
      CS_ASSERT (false);
      break;
  }
  return true;
}

bool celPersistClassic::SaveEntity (iCelEntity* entity, const char* name)
{
  csMemFile m;
  iFile* mf = SCF_QUERY_INTERFACE (&m, iFile);
  if (!Write (mf, entity))
  {
    mf->DecRef ();
    return false;
  }

  iVFS* vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  CS_ASSERT (vfs != NULL);
  vfs->WriteFile (name, m.GetData (), m.GetSize ());
  mf->DecRef ();
  vfs->DecRef ();
  return true;
}


