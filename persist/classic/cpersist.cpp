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
#include "pl/propfact.h"
#include "bl/bl.h"
#include "bl/behave.h"
#include "persist/classic/cpersist.h"
#include "cssys/csendian.h"
#include "csutil/memfile.h"
#include "iutil/vfs.h"
#include "iutil/objreg.h"
#include "ivaria/reporter.h"

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
  pl = NULL;
}

celPersistClassic::~celPersistClassic ()
{
  if (pl) pl->DecRef ();
}

bool celPersistClassic::Initialize (iObjectRegistry* object_reg)
{
  celPersistClassic::object_reg = object_reg;
  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  CS_ASSERT (pl != NULL);
  return true;
}

void celPersistClassic::Report (const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  iReporter* rep = CS_QUERY_REGISTRY (object_reg, iReporter);
  if (rep)
  {
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persist.classic",
    	msg, arg);
    rep->DecRef ();
  }
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

iCelEntity* celPersistClassic::FindOrCreateEntity (const char* name)
{
  iCelEntity* entity = (iCelEntity*)read_entities.Get (
  	csHashCompute (name));
  if (!entity)
  {
    entity = pl->CreateEntity ();
    entity->SetName (name);
    read_entities.Put (csHashCompute (name), entity);
  }
  return entity;
}

bool celPersistClassic::ReadMarker (char*& data, size_t& remaining,
	char* marker)
{
  if (remaining < 4) return false;
  memcpy (marker, data, 4);
  data += 4; remaining -= 4;
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining, int8& b)
{
  if (remaining < 1) return false;
  b = (int8)*data;
  data += 1; remaining -= 1;
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining, uint8& ub)
{
  if (remaining < 1) return false;
  ub = (uint8)*data;
  data += 1; remaining -= 1;
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining, int16& w)
{
  if (remaining < 2) return false;
  w = get_le_short (data);
  data += 2; remaining -= 2;
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining, uint16& uw)
{
  if (remaining < 2) return false;
  uw = get_le_short (data);
  data += 2; remaining -= 2;
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining, int32& l)
{
  if (remaining < 4) return false;
  l = get_le_long (data);
  data += 4; remaining -= 4;
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining, uint32& ul)
{
  if (remaining < 4) return false;
  ul = get_le_long (data);
  data += 4; remaining -= 4;
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining, float& f)
{
  if (remaining < 4) return false;
  memcpy ((void*)&f, data, 4);
  f = convert_endian (f);
  data += 4; remaining -= 4;
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining, char*& str)
{
  uint16 l;
  if (!Read (data, remaining, l)) return false;
  if (l)
  {
    if (l > remaining) return false;
    str = new char[l+1];
    memcpy (str, data, l);
    str[l] = 0;
    data += l; remaining -= l;
  }
  else str = NULL;
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining, celData* cd)
{
  uint8 t;
  if (!Read (data, remaining, t)) return false;
  uint8 ub;
  int8 b;
  uint16 uw;
  int16 w;
  uint32 ul;
  int32 l;
  float f;
  char* s;
  switch (t)
  {
    case CEL_DATA_ACTION:
    case CEL_DATA_NONE:
      return false;
    case CEL_DATA_BOOL:
      if (!Read (data, remaining, ub)) return false;
      cd->Set ((bool)ub);
      break;
    case CEL_DATA_BYTE:
      if (!Read (data, remaining, b)) return false;
      cd->Set (b);
      break;
    case CEL_DATA_WORD:
      if (!Read (data, remaining, w)) return false;
      cd->Set (w);
      break;
    case CEL_DATA_LONG:
      if (!Read (data, remaining, l)) return false;
      cd->Set (l);
      break;
    case CEL_DATA_UBYTE:
      if (!Read (data, remaining, ub)) return false;
      cd->Set (ub);
      break;
    case CEL_DATA_UWORD:
      if (!Read (data, remaining, uw)) return false;
      cd->Set (uw);
      break;
    case CEL_DATA_ULONG:
      if (!Read (data, remaining, ul)) return false;
      cd->Set (ul);
      break;
    case CEL_DATA_FLOAT:
      if (!Read (data, remaining, f)) return false;
      cd->Set (f);
      break;
    case CEL_DATA_STRING:
      if (!Read (data, remaining, s)) return false;
      cd->Set (s);
      delete[] s;
      break;
    case CEL_DATA_VECTOR3:
      {
        csVector3 v;
        if (!Read (data, remaining, v.x)) return false;
        if (!Read (data, remaining, v.y)) return false;
        if (!Read (data, remaining, v.z)) return false;
        cd->Set (v);
      }
      break;
    case CEL_DATA_PCLASS:
      {
        iCelPropertyClass* pc;
	if (!Read (data, remaining, pc)) return false;
	cd->Set (pc);
      }
      break;
    case CEL_DATA_ENTITY:
      {
        iCelEntity* ent;
	if (!Read (data, remaining, ent)) return false;
	cd->Set (ent);
      }
      break;
    case CEL_DATA_BUFFER:
      {
        iCelDataBuffer* db;
	if (!Read (data, remaining, db)) return false;
	cd->Set (db);
	db->DecRef ();
      }
      break;
    default:
      return false;
  }
  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining,
	iCelDataBuffer*& db)
{
  int32 ser;
  db = NULL;
  if (!Read (data, remaining, ser))
  {
    Report ("File truncated while reading data buffer serial number!");
    return false;
  }
  uint16 cnt;
  if (!Read (data, remaining, cnt))
  {
    Report ("File truncated while reading number of data entries!");
    return false;
  }
  db = pl->CreateDataBuffer (ser);
  db->SetDataCount (cnt);
  int i;
  for (i = 0 ; i < cnt ; i++)
  {
    if (!Read (data, remaining, db->GetData (i)))
    {
      Report ("Error reading data entry %d!", i);
      db->DecRef ();
      db = NULL;
      return false;
    }
  }

  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining,
	iCelPropertyClass*& pc)
{
  char marker[4];
  pc = NULL;
  if (!ReadMarker (data, remaining, marker))
  {
    Report ("File truncated while reading property class marker!");
    return false;
  }
  if (strncmp (marker, "PCL", 3))
  {
    Report ("Expected property class, got something else!");
    return false;
  }
  if (marker[3] == '0') return true;	// NULL entity.
  if (marker[3] == 'R')
  {
    // A reference.
    char* entname = NULL, * pcname = NULL;
    bool rc = true;
    rc = rc && Read (data, remaining, entname);
    rc = rc && Read (data, remaining, pcname);
    if (rc)
    {
      iCelEntity* entity = FindOrCreateEntity (entname);
      pc = entity->GetPropertyClassList ()->FindByName (pcname);
      if (!pc)
      {
        Report ("Cannot find property class '%s' for entity '%s'!",
		pcname, entname);
	rc = false;
      }
    }
    delete[] entname;
    delete[] pcname;
    return rc;
  }
  else if (marker[3] == 'I')
  {
    char* entname = NULL, * pcname = NULL;
    bool rc = true;
    rc = rc && Read (data, remaining, entname);
    rc = rc && Read (data, remaining, pcname);
    if (rc)
    {
      rc = false;
      iCelPropertyClassFactory* pf = pl->FindPropertyClassFactory (pcname);
      if (pf)
      {
        pc = pf->CreatePropertyClass();
	if (pc)
	{
          iCelDataBuffer* db;
          if (Read (data, remaining, db))
          {
            iCelEntity* entity = FindOrCreateEntity (entname);
	    pc->SetEntity (entity);
	    if (pc->Load (db))
	    {
	      entity->GetPropertyClassList ()->Add (pc);
	      rc = true;
	    }
	    else
              Report ("Property class '%s' for entity '%s' failed to load!",
	      	pcname, entname);
	    pc->DecRef ();
	    db->DecRef ();
          }
        }
	else
          Report ("Cannot create property class '%s'!",	pcname);
      }
      else
        Report ("Cannot find property class factory for '%s'!", pcname);
    }
    delete[] entname;
    delete[] pcname;
    if (!rc) pc = NULL;
    return rc;
  }
  else
  {
    Report ("Bad property class marker!");
    return false;
  }

  return true;
}

bool celPersistClassic::Read (char*& data, size_t& remaining,
	iCelEntity*& entity)
{
  char marker[4];
  entity = NULL;
  if (!ReadMarker (data, remaining, marker))
  {
    Report ("File truncated while reading entity marker!");
    return false;
  }
  if (strncmp (marker, "ENT", 3))
  {
    Report ("Expected entity, got something else!");
    return false;
  }
  if (marker[3] == '0') return true;	// NULL entity.
  if (marker[3] == 'R')
  {
    // A reference.
    char* entname;
    if (!Read (data, remaining, entname))
    {
      Report ("Expected entity name, got something else!");
      return false;
    }
    entity = FindOrCreateEntity (entname);
    delete[] entname;
  }
  else if (marker[3] == 'I')
  {
    char* entname = NULL, * bhname = NULL, * bhlayername = NULL;
    bool rc = true;
    rc = rc && Read (data, remaining, entname);
    rc = rc && Read (data, remaining, bhlayername);
    if (rc && bhlayername)
      rc = rc && Read (data, remaining, bhname);
    uint16 c;
    rc = rc && Read (data, remaining, c);
    if (!rc)
    {
      Report ("Missing entity information!");
      delete[] bhlayername;
      delete[] bhname;
      delete[] entname;
      return false;
    }

    // An entity.
    entity = FindOrCreateEntity (entname);
    delete[] entname;
    if (bhlayername && bhname)
    {
      iCelBlLayer* bl = pl->FindBehaviourLayer (bhlayername);
      iCelBehaviour* bh = bl->CreateBehaviour (entity, bhname);
      entity->SetBehaviour (bh);
      bh->DecRef ();
    }
    delete[] bhname;
    delete[] bhlayername;

    int i;
    for (i = 0 ; i < c ; i++)
    {
      iCelPropertyClass* pc;
      if (!Read (data, remaining, pc))
      {
        entity->DecRef ();
	entity = NULL;
	return false;
      }
    }
  }
  else
  {
    Report ("Bad entity marker!");
    return false;
  }

  return true;
}

iCelEntity* celPersistClassic::LoadEntity (const char* name)
{
  read_entities.DeleteAll ();

  iVFS* vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  CS_ASSERT (vfs != NULL);
  iDataBuffer* data = vfs->ReadFile (name);
  vfs->DecRef ();

  char* d = (char*)data->GetData ();
  size_t remaining = data->GetSize ();

  char marker[5];
  if (!ReadMarker (d, remaining, marker))
  {
    Report ("File is not a CEL file, too short!");
    data->DecRef ();
    return NULL;
  }
  if (strncmp (marker, "CEL0", 4))
  {
    marker[4] = 0;
    Report ("File is not a CEL file, bad marker '%s'!", marker);
    data->DecRef ();
    return NULL;
  }
  iCelEntity* ent;
  bool rc = Read (d, remaining, ent);

  data->DecRef ();

  read_entities.DeleteAll ();
  return rc ? ent : NULL;
}

bool celPersistClassic::WriteMarker (iFile* f, const char* s)
{
  if (!f->Write (s, 4)) return false;
  return true;
}

bool celPersistClassic::Write (iFile* f, const char* s)
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
  bool ref = pclasses.In (pc);
  if (ref)
  {
    if (!WriteMarker (f, "PCLR")) return false;
    iCelEntity* pc_ent = pc->GetEntity ();
    // First write entity name, then property class name.
    if (!Write (f, pc_ent->GetName ())) return false;
    if (!Write (f, pc->GetName ())) return false;
    return true;
  }

  pclasses.Add (pc);
  if (!WriteMarker (f, "PCLI")) return false;
  iCelEntity* pc_ent = pc->GetEntity ();
  // First write entity name, then property class name.
  if (!Write (f, pc_ent->GetName ())) return false;
  if (!Write (f, pc->GetName ())) return false;
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
  bool ref = entities.In (entity);
  if (ref)
  {
    if (!WriteMarker (f, "ENTR")) return false;
    // Unique entity name! @@@
    if (!Write (f, entity->GetName ())) return false;
    return true;
  }

  entities.Add (entity);
  if (!WriteMarker (f, "ENTI")) return false;

  if (!Write (f, entity->GetName ())) return false;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    if (!Write (f, bh->GetBehaviourLayer ()->GetName ())) return false;
    if (!Write (f, bh->GetName ())) return false;
  }
  else
  {
    if (!Write (f, (char*)NULL)) return false;
  }

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
  uint8 t = (uint8)(data->type);
  if (!f->Write ((const char*)&t, 1)) return false;
  switch (data->type)
  {
    case CEL_DATA_NONE:
    case CEL_DATA_ACTION:
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
    case CEL_DATA_VECTOR3:
      {
        float v = convert_endian (data->value.v.x);
        if (!f->Write ((const char*)&v, 4)) return false;
        v = convert_endian (data->value.v.y);
        if (!f->Write ((const char*)&v, 4)) return false;
        v = convert_endian (data->value.v.z);
        if (!f->Write ((const char*)&v, 4)) return false;
      }
      break;
    case CEL_DATA_STRING:
      if (!Write (f, data->value.s)) return false;
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
  entities.DeleteAll ();
  pclasses.DeleteAll ();

  csMemFile m;
  iFile* mf = SCF_QUERY_INTERFACE (&m, iFile);
  if (!WriteMarker (mf, "CEL0"))
  {
    mf->DecRef ();
    return false;
  }
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

  entities.DeleteAll ();
  pclasses.DeleteAll ();
  return true;
}


