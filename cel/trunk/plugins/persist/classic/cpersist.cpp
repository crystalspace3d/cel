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
  SCF_IMPLEMENTS_INTERFACE (iCelPersistance) /* Deprecated: Backward compat. */
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPersistClassic::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPersistClassicContext)
  SCF_IMPLEMENTS_INTERFACE (iCelPersistenceContext)
  SCF_IMPLEMENTS_INTERFACE (iCelPersistanceContext) /* Backward compat. */
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iCelEntityRemoveCallback)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPersistClassicContext::RemoveCallback)
  SCF_IMPLEMENTS_INTERFACE (iCelEntityRemoveCallback)
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

iCelPersistenceContext* celPersistClassic::CreateContext(iBase* data, int mode
    , bool performmapping)
{
  csRef<iFile> file (SCF_QUERY_INTERFACE(data, iFile));
  if (!file)
    return 0;

  celPersistClassicContext* context = new celPersistClassicContext;
  if (!context->Initialize(object_reg, file, mode, performmapping))
  {
    delete context;
    return 0;
  }
  
  return context;
}

bool celPersistClassic::SaveEntity (iCelEntity* entity, const char* name)
{
  celPersistClassicContext* context;
  csMemFile m;
  
  csRef<iFile> mf (SCF_QUERY_INTERFACE (&m, iFile));
  context = new celPersistClassicContext;
  if (!context->Initialize(object_reg, mf, CEL_PERSIST_MODE_WRITE, false))
  {
    context->DecRef();
    return false;
  }

  if (!context->WriteMarker ("CEL0"))
  {
    context->DecRef();
    return false;
  }
  if (!context->Write (entity))
  {
    context->DecRef();
    return false;
  }
  context->DecRef();

  vfs->WriteFile (name, m.GetData (), m.GetSize ());

  return true;
}

csPtr<iCelEntity> celPersistClassic::LoadEntity (const char* name)
{
  celPersistClassicContext* context;
  iCelEntity* ent;
  
  csRef<iDataBuffer> data (vfs->ReadFile (name));
  if (!data)
      return 0;

  csMemFile mf((const char*) data->GetData(), data->GetSize());
  context = new celPersistClassicContext;
  if (!context->Initialize(object_reg, &mf, CEL_PERSIST_MODE_READ, true))
    return 0;

  if (!context->CheckMarker ("CEL0"))
  {
    context->Report ("File is not a CEL file, bad marker '%s'!", "CEL0");
    context->DecRef();
    return 0;
  }
  
  if (!context->Read (ent))
  {
    context->Report ("Failed to load entity!");
    context->DecRef();
    return 0;
  }
  ent->IncRef();

  context->DecRef();

  return ent;
}

// ---------------------------------------------------------------------------

celPersistClassicContext::celPersistClassicContext()
{
  SCF_CONSTRUCT_IBASE(0);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiCelEntityRemoveCallback);
  object_reg = 0;
}

celPersistClassicContext::~celPersistClassicContext()
{
  // free our references to entities
  Clear();
    
  if (performmapping)
    pl->UnregisterRemoveCallback(&scfiCelEntityRemoveCallback);
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiCelEntityRemoveCallback);
  SCF_DESTRUCT_IBASE ();
}

void celPersistClassicContext::Clear()
{
  if (performmapping)
    ClearTempRefs();
  
  read_entities.DeleteAll();
  entities.DeleteAll();
  pclasses.DeleteAll();
}

void celPersistClassicContext::ClearTempRefs()
{
  temprefs.DeleteAll ();
}

void celPersistClassicContext::RemoveEntity (iCelEntity* entity)
{
  // the real entitiy dies, so we have to remove the ref from our mapping
  // tables
  uint serverid = GetMappedID(entity);
  if (serverid == 0)
    return;
  read_entities.DeleteAll (serverid);
  read_ids.DeleteAll (entity->GetID());
}

bool celPersistClassicContext::Initialize(iObjectRegistry* object_reg,
    iFile* file, int mode, bool performmapping)
{
  celPersistClassicContext::object_reg = object_reg;
  celPersistClassicContext::performmapping = performmapping;
  
  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl) return false;

  celPersistClassicContext::file = file;
  (void) mode;

  if (performmapping)
    pl->RegisterRemoveCallback(&scfiCelEntityRemoveCallback);

  return true;
}

void celPersistClassicContext::Report (const char* msg, ...)
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

csPtr<iCelEntity> celPersistClassicContext::LoadEntity()
{
  iCelEntity* ent;
  if (!Read(ent))
    return 0;

  ent->IncRef();
  if (performmapping)
  {
    ClearTempRefs();
  }

  return ent;
}

bool celPersistClassicContext::SaveEntity(iCelEntity* entity)
{ 
  return Write(entity);
}

iCelEntity* celPersistClassicContext::FindEntity (uint id)
{
  return read_entities.Get (id, 0);
}

iCelEntity* celPersistClassicContext::FindOrCreateEntity (uint id)
{
  csRef<iCelEntity> entity;
  
  if (!performmapping)
  {
    entity = pl->GetEntity(id);
    if (!entity)
      Report ("No mapping of entities but entity not found (ID '%u').", id);
  }
  else
  {
    entity = read_entities.Get (id, 0);
    if (!entity)
    {
      entity = pl->CreateEntity ();
      if (entity)
      {
	read_entities.Put (id, entity);
	uint* tmpid = new uint(id);
	read_ids.Put (entity->GetID(), tmpid);
	temprefs.Push (entity);
      }
    }
  }

  return entity;
}

iCelEntity* celPersistClassicContext::GetMappedEntity(uint id)
{
  return FindEntity(id);
}

uint celPersistClassicContext::GetMappedID(iCelEntity* entity)
{
  uint* id = read_ids.Get(entity->GetID(), 0);
  return id ? *id : 0;
}

bool celPersistClassicContext::ReadMarker (char* marker)
{
  if (file->Read(marker, 4) < 4)
    return false;

  return true;
}

bool celPersistClassicContext::CheckMarker (const char* comp)
{
  char marker[5];
  if (!ReadMarker(marker))
    return false;
  marker[4]=0;
  
  if (strncmp (marker, comp, 4)) {
    Report ("Expected marker '%s' but got '%s'!", comp, marker);
    return false;
  }

  return true;
}

#ifdef PERSIST_DEBUG
#define READDEBUG(x)  { if (!CheckMarker(x)) {			    \
  Report ("Expected marker: %s, got something else!\n",x);	    \
  CS_ASSERT(false);						    \
  return false;							    \
  }								    \
}
#else
#define READDEBUG(x)
#endif

bool celPersistClassicContext::Read (int8& b)
{
  READDEBUG("INT8");
  if (file->Read((char*) &b, 1) < 1)
    return false;

  return true;
}

bool celPersistClassicContext::Read (uint8& ub)
{
  READDEBUG("UIT8");
  if (file->Read((char*) &ub, 1) < 1)
    return false;

  return true;
}

bool celPersistClassicContext::Read (int16& w)
{
  READDEBUG("IT16");
  if (file->Read((char*) &w, 2) < 2)
    return false;

  w = csGetLittleEndianShort (&w);
  return true;
}

bool celPersistClassicContext::Read (uint16& uw)
{
  READDEBUG("UI16");
  if (file->Read((char*) &uw, 2) < 2)
    return false;
  
  uw = csGetLittleEndianShort (&uw);
  return true;
}

bool celPersistClassicContext::Read (int32& l)
{
  READDEBUG("IT32");
  if (file->Read((char*) &l, 4) < 4)
    return false;
  
  l = csGetLittleEndianLong (&l);
  return true;
}

bool celPersistClassicContext::Read (uint32& ul)
{
  READDEBUG("UI32");
  if (file->Read((char*) &ul, 4) < 4)
    return false;
  
  ul = csGetLittleEndianLong (&ul);
  return true;
}

bool celPersistClassicContext::Read (float& f)
{
  READDEBUG("FLT");
  if (file->Read((char*) &f, 4) < 4)
    return false;

  f = csConvertEndian (f);
  return true;
}

bool celPersistClassicContext::Read (char*& str)
{
  READDEBUG("STR");
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

bool celPersistClassicContext::Read (celData* cd)
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

  READDEBUG("CLDT");
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
	if (!Read (pc)) return false;
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
    case CEL_DATA_BUFFER:
      {
        iCelDataBuffer* db;
	if (!Read (db)) return false;
	cd->Set (db);
	db->DecRef ();
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

bool celPersistClassicContext::Read (iCelDataBuffer*& db)
{
  READDEBUG("DTBF");
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
  dbref = pl->CreateDataBuffer (ser);
  db = dbref;
  db->SetDataCount (cnt);
  int i;
  for (i = 0 ; i < cnt ; i++)
  {
    if (!Read (db->GetData (i)))
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

bool celPersistClassicContext::Read (iCelPropertyClass*& pc)
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
  if (marker[3] == 'R')
  {
    // A reference.
    uint entid;
    char* pcname = 0;
    bool rc = true;
    rc = rc && Read (entid);
    rc = rc && Read (pcname);
    if (rc)
    {
      iCelEntity* entity = FindOrCreateEntity (entid);
      if (!entity)
      {
	Report ("Cannot find entity for '%s'!", pcname);
	rc = false;
      }
      pc = entity->GetPropertyClassList ()->FindByName (pcname);
      if (!pc)
      {
        Report ("Cannot find property class '%s' for entity '%s'!",
		pcname, entity->GetName());
	rc = false;
      }
    }
    delete[] pcname;
    return rc;
  }
  else if (marker[3] == 'I')
  {
    uint entid;
    char* pcname = 0;
    bool rc = true;
    rc = rc && Read (entid);
    rc = rc && Read (pcname);
    if (rc)
    {
      rc = false;
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
            iCelEntity* entity = FindOrCreateEntity (entid);
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

bool celPersistClassicContext::Read (iCelEntity*& entity)
{
  char marker[5];
  entity = 0;
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
  if (marker[3] == '0') return true;	// 0 entity.
  if (marker[3] == 'R')
  {
    // A reference.
    uint entid;
    if (!Read (entid))
    {
      Report ("Expected entity ID, got something else!");
      return false;
    }
    entity = FindOrCreateEntity (entid);
  }
  else if (marker[3] == 'I')
  {
    uint entid;
    char* entname = 0, * bhname = 0, * bhlayername = 0;
    bool rc = true;
    rc = rc && Read (entid);
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
    entity = FindOrCreateEntity (entid);
    entity->SetName(entname);
    Report ("  Reading entity %d ('%s')...\n",entid,entname);
    delete[] entname;

    int i;
    for (i = 0 ; i < c ; i++)
    {
      iCelPropertyClass* pc;
      if (!Read (pc))
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

#ifdef PERSIST_DEBUG
#define WRITEDEBUG(x) if (!WriteMarker(x)) return false;
#else
#define WRITEDEBUG(x)
#endif

bool celPersistClassicContext::WriteMarker (const char* s)
{
  if (!file->Write (s, 4)) return false;
  return true;
}

bool celPersistClassicContext::Write (const char* s)
{
  WRITEDEBUG("STR");
  uint16 l = s ? strlen (s) : 0;
  if (!Write (l)) return false;
  if (s && !file->Write (s, l)) return false;
  return true;
}

bool celPersistClassicContext::WriteID (iCelEntity* entity)
{
  if (performmapping)
  {
    uint id = GetMappedID(entity);
    return Write(id);
  }
  else
    return Write(entity->GetID());
}

bool celPersistClassicContext::Write (iCelPropertyClass* pc)
{
  if (!pc)
  {
    // 0 pc.
    if (!WriteMarker ("PCL0")) return false;
    return true;
  }
  bool ref = pclasses.In (pc);
  if (ref)
  {
    if (!WriteMarker ("PCLR")) return false;
    iCelEntity* pc_ent = pc->GetEntity ();
    // First write entity ID, then property class name.
    if (!WriteID (pc_ent)) return false;
    if (!Write (pc->GetName ())) return false;
    return true;
  }

  pclasses.Add (pc);
  if (!WriteMarker ("PCLI")) return false;
  iCelEntity* pc_ent = pc->GetEntity ();
#if 0
  printf ("%s - %p\n", pc->GetName(), pc_ent);
  printf (" %s\n", pc_ent->GetName());
#endif
  // First write entity ID, then property class name.
  if (!WriteID (pc_ent)) return false;
  if (!Write (pc->GetName ())) return false;
  csRef<iCelDataBuffer> db (pc->Save ());
  if (!db) return false;
  return Write (db);
}

bool celPersistClassicContext::Write (iCelEntity* entity)
{
  if (!entity)
  {
    // 0 entity.
    if (!WriteMarker ("ENT0")) return false;
    return true;
  }
  bool ref = entities.In (entity);
  if (ref)
  {
    if (!WriteMarker ("ENTR")) return false;
    if (!WriteID (entity)) return false;
    return true;
  }

//  Report (" CEL Persistence writing entity %d ('%s')...\n",entity->GetID(),entity->GetName() );

  entities.Add (entity);
  if (!WriteMarker ("ENTI")) return false;
  if (!WriteID (entity)) return false;
  if (!Write (entity->GetName ())) return false;

  iCelPropertyClassList* pl = entity->GetPropertyClassList ();
  if (!Write ((uint16) pl->GetCount())) return false;
  size_t i;
  for (i = 0 ; i < pl->GetCount () ; i++)
  {
    if (!Write (pl->Get (i)))
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

bool celPersistClassicContext::Write (iCelDataBuffer* db)
{
  WRITEDEBUG("DTBF");
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

bool celPersistClassicContext::Write (celData* data)
{
  WRITEDEBUG("CLDT");
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
      if (!Write (data->value.pc)) return false;
      break;
    case CEL_DATA_ENTITY:
      if (!Write (data->value.ent)) return false;
      break;
    case CEL_DATA_BUFFER:
      if (!Write (data->value.db)) return false;
      break;
    case CEL_DATA_IBASE:
      Report ("Data type iBase is not allowed for persistence!");
      return false;
    default:
      Report("Found a DataType I'm not able to write!");
      CS_ASSERT (false);
      break;
  }
  return true;
}

bool celPersistClassicContext::Write (uint32 v)
{
  WRITEDEBUG("UI32");
  v = csConvertEndian(v);
  return file->Write ((const char*) &v, 4);
}

bool celPersistClassicContext::Write (int32 v)
{
  WRITEDEBUG("IT32");
  v = csConvertEndian(v);
  return file->Write ((const char*) &v, 4);
}

bool celPersistClassicContext::Write (uint16 v)
{
  WRITEDEBUG("UI16");
  v = csConvertEndian(v);
  return file->Write ((const char*) &v, 2);
}

bool celPersistClassicContext::Write (int16 v)
{
  WRITEDEBUG("IT16");
  v = csConvertEndian(v);
  return file->Write ((const char*) &v, 2);
}

bool celPersistClassicContext::Write (uint8 v)
{
  WRITEDEBUG("UIT8");
  return file->Write ((const char*) &v, 1);
}

bool celPersistClassicContext::Write (int8 v)
{
  WRITEDEBUG("INT8");
  return file->Write ((const char*) &v, 1);
}

bool celPersistClassicContext::Write (float f)
{
  WRITEDEBUG("FLT");
  f = csConvertEndian (f);
  return file->Write ((const char*)&f, 4);
}
