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
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/behave.h"
#include "plugins/persist/classic/cpersist.h"
#include "csutil/csendian.h"
#include "csutil/memfile.h"
#include "csutil/cscolor.h"
#include "iutil/vfs.h"
#include "iutil/objreg.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

// define this to track save/loading problems
// #define PERSIST_DEBUG

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPersistClassic)

SCF_IMPLEMENT_IBASE (celPersistClassic)
  SCF_IMPLEMENTS_INTERFACE (iCelPersistence)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPersistClassic::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPersistClassic::celPersistClassic (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  object_reg = 0;
}

celPersistClassic::~celPersistClassic ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celPersistClassic::Initialize (iObjectRegistry* object_reg)
{
  celPersistClassic::object_reg = object_reg;

  vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  if (!vfs) return false;
  
  return true;
}

void celPersistClassic::Report (const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persist.classic",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//------------------------------------------------------------------------

bool celPersistClassic::WriteMarker (const char* s)
{
  if (!file->Write (s, 4)) return false;
  return true;
}

bool celPersistClassic::Write (const char* s)
{
  uint16 l = s ? strlen (s) : 0;
  if (!Write (l)) return false;
  if (s && !file->Write (s, l)) return false;
  return true;
}

bool celPersistClassic::Write (iCelDataBuffer* db)
{
  if (!Write ((int32) db->GetSerialNumber())) return false;
  if (!Write ((uint16) db->GetDataCount())) return false;
  size_t i;
  for (i = 0 ; i < db->GetDataCount () ; i++)
  {
    if (!Write (db->GetData (i)))
      return false;
  }
  return true;
}

bool celPersistClassic::Write (celData* data)
{
  if (!Write ((uint8) data->type)) return false;
  switch (data->type)
  {
    case CEL_DATA_NONE:
    case CEL_DATA_ACTION:
      CS_ASSERT (false);
      break;
    case CEL_DATA_BOOL:
      if (!Write ((uint8)data->value.bo)) return false;
      break;
    case CEL_DATA_BYTE:
      if (!Write (data->value.b)) return false;
      break;
    case CEL_DATA_WORD:
      if (!Write (data->value.w)) return false;
      break;
    case CEL_DATA_LONG:
      if (!Write (data->value.l)) return false;
      break;
    case CEL_DATA_UBYTE:
      if (!Write (data->value.ub)) return false;
      break;
    case CEL_DATA_UWORD:
      if (!Write (data->value.uw)) return false;
      break;
    case CEL_DATA_ULONG:
      if (!Write (data->value.ul)) return false;
      break;
    case CEL_DATA_FLOAT:
      if (!Write (data->value.f)) return false;
      break;
    case CEL_DATA_VECTOR2:
      {
        if (!Write (data->value.v.x)) return false;
	if (!Write (data->value.v.y)) return false;
      }
      break;
    case CEL_DATA_VECTOR3:
      {
        if (!Write (data->value.v.x)) return false;
	if (!Write (data->value.v.y)) return false;
	if (!Write (data->value.v.z)) return false;
      }
      break;
    case CEL_DATA_COLOR:
      {
        if (!Write (data->value.col.red)) return false;
	if (!Write (data->value.col.green)) return false;
	if (!Write (data->value.col.blue)) return false;
      }
      break;
    case CEL_DATA_STRING:
      if (!Write (*data->value.s)) return false;
      break;
    case CEL_DATA_PCLASS:
      if (!Write (data->value.pc, false)) return false;
      break;
    case CEL_DATA_ENTITY:
      if (!Write (data->value.ent, false)) return false;
      break;
    case CEL_DATA_IBASE:
      Report ("Data type iBase is not allowed for persistence!");
      return false;
    default:
      Report ("Found a DataType I'm not able to write!");
      CS_ASSERT (false);
      break;
  }
  return true;
}

bool celPersistClassic::Write (uint32 v)
{
  v = csConvertEndian(v);
  return file->Write ((const char*) &v, 4);
}

bool celPersistClassic::Write (int32 v)
{
  v = csConvertEndian(v);
  return file->Write ((const char*) &v, 4);
}

bool celPersistClassic::Write (uint16 v)
{
  v = csConvertEndian(v);
  return file->Write ((const char*) &v, 2);
}

bool celPersistClassic::Write (int16 v)
{
  v = csConvertEndian(v);
  return file->Write ((const char*) &v, 2);
}

bool celPersistClassic::Write (uint8 v)
{
  return file->Write ((const char*) &v, 1);
}

bool celPersistClassic::Write (int8 v)
{
  return file->Write ((const char*) &v, 1);
}

bool celPersistClassic::Write (float f)
{
  f = csConvertEndian (f);
  return file->Write ((const char*)&f, 4);
}

bool celPersistClassic::Write (iCelPropertyClass* pc, bool savelocal)
{
  if (!pc)
  {
    // 0 pc.
    if (!WriteMarker ("PCL0")) return false;
    return true;
  }
  if (!set->IsLocal (pc))
  {
    // This is a non-local pc.
    if (!WriteMarker ("PCLE")) return false;
    csRef<iCelDataBuffer> db = set->SaveExternalPC (pc);
    return Write (db);
  }
  else if (!savelocal)
  {
    // This is a local pc and we're not saving local pc's.
    // In that case we still have to save a reference.
    if (!WriteMarker ("PCLR")) return false;
    size_t entity_idx = entities_map.Get (pc->GetEntity (),
    	csArrayItemNotFound);
    CS_ASSERT(entity_idx != csArrayItemNotFound);
    if (!Write ((uint32)entity_idx)) return false;
    if (!Write (pc->GetName ())) return false;
    if (!Write (pc->GetTag ())) return false;
    return true;
  }

  if (!WriteMarker ("PCLI")) return false;
  // First write entity ID, then property class name.
  if (!Write (pc->GetName ())) return false;
  if (!Write (pc->GetTag ())) return false;
  csRef<iCelDataBuffer> db = pc->Save ();
  if (!db) return false;
  return Write (db);
}

bool celPersistClassic::Write (iCelEntity* entity, bool savelocal)
{
  if (!entity)
  {
    // 0 entity.
    if (!WriteMarker ("ENT0")) return false;
    return true;
  }
  if (!set->IsLocal (entity))
  {
    // This is a non-local entity.
    if (!WriteMarker ("ENTE")) return false;
    csRef<iCelDataBuffer> db = set->SaveExternalEntity (entity);
    return Write (db);
  }
  else if (!savelocal)
  {
    // This is a local entity and we're not saving local entities.
    // In that case we still have to save a reference.
    if (!WriteMarker ("ENTR")) return false;
    size_t idx = entities_map.Get (entity, csArrayItemNotFound);
    CS_ASSERT(idx != csArrayItemNotFound);
    if (!Write ((uint32)idx)) return false;
    return true;
  }

  if (!WriteMarker ("ENTI")) return false;
  if (!Write (entity->GetName ())) return false;

  iCelPropertyClassList* pl = entity->GetPropertyClassList ();
  if (!Write ((uint16) pl->GetCount())) return false;
  size_t i;
  for (i = 0 ; i < pl->GetCount () ; i++)
  {
    if (!Write (pl->Get (i), true))
      return false;
  }

  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {  
    if (!Write (bh->GetBehaviourLayer ()->GetName ())) return false;
    if (!Write (bh->GetName ())) return false;
  }
  else
  {
    if (!Write ((char*)0)) return false;
  }  
  return true;
}

//------------------------------------------------------------------------

bool celPersistClassic::ReadMarker (char* marker)
{
  if (file->Read (marker, 4) < 4)
    return false;
  return true;
}

bool celPersistClassic::CheckMarker (const char* comp)
{
  char marker[5];
  if (!ReadMarker (marker))
    return false;
  marker[4] = 0;
  
  if (strncmp (marker, comp, 4))
  {
    Report ("Expected marker '%s' but got '%s'!", comp, marker);
    return false;
  }

  return true;
}

bool celPersistClassic::Read (int8& b)
{
  if (file->Read((char*) &b, 1) < 1)
    return false;
  return true;
}

bool celPersistClassic::Read (uint8& ub)
{
  if (file->Read((char*) &ub, 1) < 1)
    return false;
  return true;
}

bool celPersistClassic::Read (int16& w)
{
  if (file->Read((char*) &w, 2) < 2)
    return false;
  w = csGetLittleEndianShort (&w);
  return true;
}

bool celPersistClassic::Read (uint16& uw)
{
  if (file->Read((char*) &uw, 2) < 2)
    return false;
  uw = csGetLittleEndianShort (&uw);
  return true;
}

bool celPersistClassic::Read (int32& l)
{
  if (file->Read((char*) &l, 4) < 4)
    return false;
  l = csGetLittleEndianLong (&l);
  return true;
}

bool celPersistClassic::Read (uint32& ul)
{
  if (file->Read((char*) &ul, 4) < 4)
    return false;
  ul = csGetLittleEndianLong (&ul);
  return true;
}

bool celPersistClassic::Read (float& f)
{
  if (file->Read((char*) &f, 4) < 4)
    return false;
  f = csConvertEndian (f);
  return true;
}

bool celPersistClassic::Read (char*& str)
{
  uint16 l;
  if (!Read (l)) return false;
  if (l)
  {
    str = new char[l+1];
    if (file->Read((char*) str, l) < l) {
      delete [] str;
      str = 0;
      return false;
    }
    str[l] = 0;
  }
  else str = 0;
  return true;
}

bool celPersistClassic::Read (celData* cd)
{
  uint8 t;
  uint8 ub;
  int8 b;
  uint16 uw;
  int16 w;
  uint32 ul;
  int32 l;
  float f;
  char* s;

  if (!Read (t)) return false;
  switch (t)
  {
    case CEL_DATA_ACTION:
    case CEL_DATA_NONE:
      return false;
    case CEL_DATA_BOOL:
      if (!Read (ub)) return false;
      cd->Set ((bool)ub);
      break;
    case CEL_DATA_BYTE:
      if (!Read (b)) return false;
      cd->Set (b);
      break;
    case CEL_DATA_WORD:
      if (!Read (w)) return false;
      cd->Set (w);
      break;
    case CEL_DATA_LONG:
      if (!Read (l)) return false;
      cd->Set (l);
      break;
    case CEL_DATA_UBYTE:
      if (!Read (ub)) return false;
      cd->Set (ub);
      break;
    case CEL_DATA_UWORD:
      if (!Read (uw)) return false;
      cd->Set (uw);
      break;
    case CEL_DATA_ULONG:
      if (!Read (ul)) return false;
      cd->Set (ul);
      break;
    case CEL_DATA_FLOAT:
      if (!Read (f)) return false;
      cd->Set (f);
      break;
    case CEL_DATA_STRING:
      if (!Read (s)) return false;
      cd->Set (s);
      delete[] s;
      break;
    case CEL_DATA_COLOR:
      {
        csColor v;
        if (!Read (v.red)) return false;
        if (!Read (v.green)) return false;
        if (!Read (v.blue)) return false;
        cd->Set (v);
      }
      break;
    case CEL_DATA_VECTOR2:
      {
        csVector2 v;
        if (!Read (v.x)) return false;
        if (!Read (v.y)) return false;
        cd->Set (v);
      }
      break;
    case CEL_DATA_VECTOR3:
      {
        csVector3 v;
        if (!Read (v.x)) return false;
        if (!Read (v.y)) return false;
        if (!Read (v.z)) return false;
        cd->Set (v);
      }
      break;
    case CEL_DATA_PCLASS:
      {
        iCelPropertyClass* pc;
	if (!Read (0, pc)) return false;
	cd->Set (pc);
      }
      break;
    case CEL_DATA_ENTITY:
      {
        iCelEntity* ent;
	if (!Read (ent)) return false;
	cd->Set (ent);
      }
      break;
    case CEL_DATA_IBASE:
      Report ("Data type iBase is not allowed for persistence!");
      return false;
    default:
      Report ("Found unknown Data type!");
      return false;
  }
  return true;
}

bool celPersistClassic::Read (iCelDataBuffer*& db)
{
  int32 ser;
  db = 0;
  csRef<iCelDataBuffer> dbref;
  if (!Read (ser))
  {
    Report ("File truncated while reading data buffer serial number!");
    return false;
  }
  uint16 cnt;
  if (!Read (cnt))
  {
    Report ("File truncated while reading number of data entries!");
    return false;
  }
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  dbref = pl->CreateDataBuffer (ser);
  db = dbref;
  int i;
  for (i = 0 ; i < cnt ; i++)
  {
    if (!Read (db->AddData ()))
    {
      Report ("Error reading data entry %d!", i);
      db = 0;
      dbref = 0;
      return false;
    }
  }

  if (dbref) dbref->IncRef (); // Avoid smart pointer release.
  return true;
}

bool celPersistClassic::Read (iCelEntity* entity, iCelPropertyClass*& pc)
{
  char marker[5];
  pc = 0;
  if (!ReadMarker (marker))
  {
    Report ("File truncated while reading property class marker!");
    return false;
  }
  marker[4]=0;
  if (strncmp (marker, "PCL", 3))
  {
    Report ("Expected property class, got something else: '%s'!",marker);
    return false;
  }
  if (marker[3] == '0')
  {
    // 0 entity.
    Report ("Read 0 Propclass!");
    return true;
  }
  if (marker[3] == 'E')
  {
    iCelDataBuffer* db;
    if (!Read (db))
    {
      Report ("Error reading external property class reference!");
      return false;
    }
    pc = set->FindExternalPC (db);
    db->DecRef ();
    return true;
  }
  if (marker[3] == 'R')
  {
    // A reference.
    uint entid;
    char* pcname = 0;
    char* tag = 0;
    bool rc = true;
    rc = rc && Read (entid);
    rc = rc && Read (pcname);
    rc = rc && Read (tag);
    if (rc)
    {
      iCelEntity* entity = set->GetEntity (entid);
      if (!entity)
      {
	Report ("Cannot find entity for '%s'!", pcname);
	rc = false;
      }
      if (tag)
        pc = entity->GetPropertyClassList ()->FindByNameAndTag (pcname, tag);
      else
        pc = entity->GetPropertyClassList ()->FindByName (pcname);
      if (!pc)
      {
        Report ("Cannot find property class '%s' for entity '%s'!",
		pcname, entity->GetName());
	rc = false;
      }
    }
    delete[] pcname;
    delete[] tag;
    return rc;
  }
  else if (marker[3] == 'I')
  {
    uint entid;
    char* pcname = 0;
    bool rc = true;
    rc = rc && Read (pcname);
    if (rc)
    {
      rc = false;
      csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      iCelPropertyClassFactory* pf = pl->FindPropertyClassFactory (pcname);
      if (pf)
      {
        csRef<iCelPropertyClass> pcref (pf->CreatePropertyClass());
        pc = pcref;
	pcref->IncRef ();	// Avoid smart pointer release.
	if (pc)
	{
          iCelDataBuffer* db;
          if (Read (db))
          {
	    pc->SetEntity (entity);
	    if (pc->Load (db))
	    {
	      Report ("Adding PC '%s' to Entity '%s'",
		  pcname, entity->GetName());
	      entity->GetPropertyClassList ()->Add (pc);
	      rc = true;
	    }
	    else
              Report ("Property class '%s' for entity with id '%u' failed to load!",
	      	pcname, entid);
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
    delete[] pcname;
    if (!rc) pc = 0;
    return rc;
  }
  else
  {
    Report ("Bad property class marker!");
    return false;
  }

  return true;
}

bool celPersistClassic::Read (iCelEntity*& entity)
{
  char marker[5];
  if (!ReadMarker (marker))
  {
    Report ("File truncated while reading entity marker!");
    return false;
  }
  marker[4]='\0';
  if (strncmp (marker, "ENT", 3))
  {
    Report ("Expected entity, got something else: %s",marker);
    return false;
  }
  if (marker[3] == '0')
  {
    entity = 0;
    return true;	// 0 entity.
  }
  if (marker[3] == 'R')
  {
    // A reference.
    uint32 entid;
    if (!Read (entid))
    {
      Report ("Expected entity ID, got something else!");
      return false;
    }
    entity = set->GetEntity (entid);
    return true;
  }
  else if (marker[3] == 'E')
  {
    iCelDataBuffer* db;
    if (!Read (db))
    {
      Report ("Error reading external entity reference!");
      return false;
    }
    entity = set->FindExternalEntity (db);
    db->DecRef ();
  }
  else if (marker[3] == 'I')
  {
    // In this case we know the entity is already given to this
    // routine.
    uint entid;
    char* entname = 0, * bhname = 0, * bhlayername = 0;
    bool rc = true;
    rc = rc && Read (entname);
    uint16 c;
    rc = rc && Read (c);
    if (!rc)
    {
      Report ("Missing entity information!");
      delete[] entname;
      return false;
    }

    // An entity.
    entity->SetName (entname);
    Report ("  Reading entity %d ('%s')...\n",entid,entname);
    delete[] entname;

    int i;
    for (i = 0 ; i < c ; i++)
    {
      iCelPropertyClass* pc;
      if (!Read (entity, pc))
      {
	entity = 0;
	return false;
      }
    }

    rc = rc && Read (bhlayername);   
    if (rc && bhlayername)
      rc = rc && Read (bhname);
    if (!rc)
    {
      Report ("Missing behaviour information!");
      delete[] bhlayername;
      delete[] bhname;
      return false;
    }
    if (bhlayername && bhname)
    {
      csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      iCelBlLayer* bl = pl->FindBehaviourLayer (bhlayername);
      if (!bl) return false;
      iCelBehaviour* bh = bl->CreateBehaviour (entity, bhname);
      if (!bh) return false;
      bh->DecRef ();
    }
    delete[] bhname;
    delete[] bhlayername;
  }
  else
  {
    Report ("Bad entity marker!");
    return false;
  }

  return true;
}

//------------------------------------------------------------------------

#define CEL_MARKER "CEL1"
bool celPersistClassic::Load (iCelLocalEntitySet* set, const char* name)
{
  csRef<iDataBuffer> data = vfs->ReadFile (name);
  if (!data)
    return false;

  csMemFile mf ((const char*) data->GetData(), data->GetSize());
  file = &mf;
  celPersistClassic::set = set;

  if (!CheckMarker (CEL_MARKER))
  {
    Report ("File is not a CEL file, bad marker '%s'!", CEL_MARKER);
    return false;
  }

  uint32 cnt;
  if (!Read (cnt))
  {
    Report ("Failed to load entity!");
    return false;
  }
  size_t i;
  entities_map.DeleteAll ();
  for (i = 0 ; i < cnt ; i++)
  {
    // @@@ Not efficient! Store pl!
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    csRef<iCelEntity> ent = pl->CreateEntity ();
    entities_map.Put (ent, i);
    set->AddEntity (ent);
  }

  for (i = 0 ; i < cnt ; i++)
  {
    iCelEntity* ent = set->GetEntity (i);
    if (!Read (ent))
    {
      Report ("Failed to load entity!");
      return false;
    }
  }

  return true;
}

bool celPersistClassic::Save (iCelLocalEntitySet* set, const char* name)
{
  csMemFile m;
  csRef<iFile> mf = SCF_QUERY_INTERFACE (&m, iFile);
  file = mf;
  celPersistClassic::set = set;

  if (!WriteMarker (CEL_MARKER)) return false;

  if (!Write ((uint32)(set->GetEntityCount ()))) return false;
  size_t i;
  entities_map.DeleteAll ();
  for (i = 0 ; i < set->GetEntityCount () ; i++)
    entities_map.Put (set->GetEntity (i), i);
  for (i = 0 ; i < set->GetEntityCount () ; i++)
  {
    iCelEntity* ent = set->GetEntity (i);
    if (!Write (ent, true)) return false;
  }

  vfs->WriteFile (name, m.GetData (), m.GetSize ());

  return true;
}

//------------------------------------------------------------------------

