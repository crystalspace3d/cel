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
#include "csutil/databuf.h"
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

bool celPersistClassic::Report (const char* msg, ...)
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
  return false;
}

bool celPersistClassic::ReportTrunc ()
{
  return Report ("File is truncated!");
}

bool celPersistClassic::ReportWrite ()
{
  return Report ("Error while writing to file!");
}

//------------------------------------------------------------------------

bool celPersistClassic::WriteMarker (const char* s)
{
  if (!file->Write (s, 4)) return ReportWrite ();
  return true;
}

bool celPersistClassic::Write (const char* s)
{
  uint16 l = s ? (uint16)strlen (s) : 0;
  if (!Write (l)) return ReportWrite ();
  if (s && !file->Write (s, l)) return ReportWrite ();
  return true;
}

bool celPersistClassic::Write (iCelDataBuffer* db)
{
  if (!Write ((int32) db->GetSerialNumber())) return ReportWrite ();
  if (!Write ((uint16) db->GetDataCount())) return ReportWrite ();
  size_t i;
  for (i = 0 ; i < db->GetDataCount () ; i++)
  {
    if (!Write (db->GetData (i)))
      return ReportWrite ();
  }
  return true;
}

bool celPersistClassic::Write (celData* data)
{
  if (!Write ((uint8) data->type)) return ReportWrite ();
  switch (data->type)
  {
    case CEL_DATA_NONE:
    case CEL_DATA_ACTION:
      CS_ASSERT (false);
      break;
    case CEL_DATA_BOOL:
      if (!Write ((uint8)data->value.bo)) return ReportWrite ();
      break;
    case CEL_DATA_BYTE:
      if (!Write (data->value.b)) return ReportWrite ();
      break;
    case CEL_DATA_WORD:
      if (!Write (data->value.w)) return ReportWrite ();
      break;
    case CEL_DATA_LONG:
      if (!Write (data->value.l)) return ReportWrite ();
      break;
    case CEL_DATA_UBYTE:
      if (!Write (data->value.ub)) return ReportWrite ();
      break;
    case CEL_DATA_UWORD:
      if (!Write (data->value.uw)) return ReportWrite ();
      break;
    case CEL_DATA_ULONG:
      if (!Write (data->value.ul)) return ReportWrite ();
      break;
    case CEL_DATA_FLOAT:
      if (!Write (data->value.f)) return ReportWrite ();
      break;
    case CEL_DATA_VECTOR2:
      {
        if (!Write (data->value.v.x)) return ReportWrite ();
	if (!Write (data->value.v.y)) return ReportWrite ();
      }
      break;
    case CEL_DATA_VECTOR3:
      {
        if (!Write (data->value.v.x)) return ReportWrite ();
	if (!Write (data->value.v.y)) return ReportWrite ();
	if (!Write (data->value.v.z)) return ReportWrite ();
      }
      break;
    case CEL_DATA_COLOR:
      {
        if (!Write (data->value.col.red)) return ReportWrite ();
	if (!Write (data->value.col.green)) return ReportWrite ();
	if (!Write (data->value.col.blue)) return ReportWrite ();
      }
      break;
    case CEL_DATA_STRING:
      if (!Write (*data->value.s)) return ReportWrite ();
      break;
    case CEL_DATA_PCLASS:
      if (!Write (data->value.pc, false)) return ReportWrite ();
      break;
    case CEL_DATA_ENTITY:
      if (!Write (data->value.ent, false)) return ReportWrite ();
      break;
    case CEL_DATA_IBASE:
      Report ("Data type iBase is not allowed for persistence!");
      return ReportWrite ();
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
  return (file->Write ((const char*) &v, 4)? true:false);
}

bool celPersistClassic::Write (int32 v)
{
  v = csConvertEndian(v);
  return (file->Write ((const char*) &v, 4)?true : false);
}

bool celPersistClassic::Write (uint16 v)
{
  v = csConvertEndian(v);
  return (file->Write ((const char*) &v, 2)?true : false);
}

bool celPersistClassic::Write (int16 v)
{
  v = csConvertEndian(v);
  return (file->Write ((const char*) &v, 2)? true : false);
}

bool celPersistClassic::Write (uint8 v)
{
    return (file->Write ((const char*) &v, 1)? true : false);
}

bool celPersistClassic::Write (int8 v)
{
    return (file->Write ((const char*) &v, 1)? true : false);
}

bool celPersistClassic::Write (float f)
{
  f = csConvertEndian (f);
  return (file->Write ((const char*)&f, 4)? true: false);
}

bool celPersistClassic::Write (iCelPropertyClass* pc, bool savelocal)
{
  if (!pc)
  {
    // 0 pc.
    if (!WriteMarker ("PCL0")) return ReportWrite ();
    return true;
  }
  if (!set->IsLocal (pc))
  {
    // This is a non-local pc.
    if (!WriteMarker ("PCLE")) return ReportWrite ();
    csRef<iCelDataBuffer> db = set->SaveExternalPC (pc);
    return Write (db);
  }
  else if (!savelocal)
  {
    // This is a local pc and we're not saving local pc's.
    // In that case we still have to save a reference.
    if (!WriteMarker ("PCLR")) return ReportWrite ();
    size_t entity_idx = entities_map.Get (pc->GetEntity (),
    	csArrayItemNotFound);
    CS_ASSERT(entity_idx != csArrayItemNotFound);
    if (!Write ((uint32)entity_idx)) return ReportWrite ();
    if (!Write (pc->GetName ())) return ReportWrite ();
    if (!Write (pc->GetTag ())) return ReportWrite ();
    return true;
  }

  if (!WriteMarker ("PCLI")) return ReportWrite ();
  // First write entity ID, then property class name.
  if (!Write (pc->GetName ())) return ReportWrite ();
  if (!Write (pc->GetTag ())) return ReportWrite ();
  csRef<iCelDataBuffer> db = pc->Save ();
  if (!db)
    return Report ("Property class '%s' doesn't support writing!",
    	pc->GetName ());
  return Write (db);
}

bool celPersistClassic::Write (iCelEntity* entity, bool savelocal)
{
  if (!entity)
  {
    // 0 entity.
    if (!WriteMarker ("ENT0")) return ReportWrite ();
    return true;
  }
  if (!set->IsLocal (entity))
  {
    // This is a non-local entity.
    if (!WriteMarker ("ENTE")) return ReportWrite ();
    csRef<iCelDataBuffer> db = set->SaveExternalEntity (entity);
    if (!db)
      return Report ("External entity is not properly saved!");
    return Write (db);
  }
  else if (!savelocal)
  {
    // This is a local entity and we're not saving local entities.
    // In that case we still have to save a reference.
    if (!WriteMarker ("ENTR")) return ReportWrite ();
    size_t idx = entities_map.Get (entity, csArrayItemNotFound);
    CS_ASSERT(idx != csArrayItemNotFound);
    if (!Write ((uint32)idx)) return ReportWrite ();
    return true;
  }

  if (!WriteMarker ("ENTI")) return ReportWrite ();
  if (!Write (entity->GetName ())) return ReportWrite ();

  iCelPropertyClassList* pl = entity->GetPropertyClassList ();
  if (!Write ((uint16) pl->GetCount())) return ReportWrite ();
  size_t i;
  for (i = 0 ; i < pl->GetCount () ; i++)
  {
    if (!Write (pl->Get (i), true))
      return false;
  }

  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {  
    if (!Write (bh->GetBehaviourLayer ()->GetName ())) return ReportWrite ();
    if (!Write (bh->GetName ())) return ReportWrite ();
  }
  else
  {
    if (!Write ((char*)0)) return ReportWrite ();
    if (!Write ((char*)0)) return ReportWrite ();
  }  
  return true;
}

bool celPersistClassic::WriteFirstPass (iCelEntity* entity)
{
  iCelPropertyClassList* plist = entity->GetPropertyClassList ();
  if (!WriteMarker ("ENT1")) return ReportWrite ();
  size_t i;
  for (i = 0 ; i < plist->GetCount () ; i++)
  {
    iCelPropertyClass* pc = plist->Get (i);
    csRef<iCelDataBuffer> db = pc->SaveFirstPass ();
    if (db)
    {
      // Yes, we have one.
      if (!Write ((uint8)1))
        return ReportWrite ();
      if (!WriteMarker ("PCL1")) return ReportWrite ();
      // First write entity ID, then property class name.
      if (!Write (pc->GetName ())) return ReportWrite ();
      if (!Write (pc->GetTag ())) return ReportWrite ();
      if (!Write (db))
        return false;
    }
  }
  // Close the list.
  if (!Write ((uint8)0))
    return ReportWrite ();

  return true;
}

//------------------------------------------------------------------------

bool celPersistClassic::ReadMarker (char* marker)
{
  if (file->Read (marker, 4) < 4)
    return ReportTrunc ();
  return true;
}

bool celPersistClassic::CheckMarker (const char* comp)
{
  char marker[5];
  if (!ReadMarker (marker))
    return ReportTrunc ();
  marker[4] = 0;
  if (strncmp (marker, comp, 4))
    return Report ("Expected marker '%s' but got '%s'!", comp, marker);
  return true;
}

bool celPersistClassic::Read (int8& b)
{
  if (file->Read((char*) &b, 1) < 1)
    return ReportTrunc ();
  return true;
}

bool celPersistClassic::Read (uint8& ub)
{
  if (file->Read((char*) &ub, 1) < 1)
    return ReportTrunc ();
  return true;
}

bool celPersistClassic::Read (int16& w)
{
  if (file->Read((char*) &w, 2) < 2)
    return ReportTrunc ();
  w = csGetLittleEndianShort (&w);
  return true;
}

bool celPersistClassic::Read (uint16& uw)
{
  if (file->Read((char*) &uw, 2) < 2)
    return ReportTrunc ();
  uw = csGetLittleEndianShort (&uw);
  return true;
}

bool celPersistClassic::Read (int32& l)
{
  if (file->Read((char*) &l, 4) < 4)
    return ReportTrunc ();
  l = csGetLittleEndianLong (&l);
  return true;
}

bool celPersistClassic::Read (uint32& ul)
{
  if (file->Read((char*) &ul, 4) < 4)
    return ReportTrunc ();
  ul = csGetLittleEndianLong (&ul);
  return true;
}

bool celPersistClassic::Read (float& f)
{
  if (file->Read((char*) &f, 4) < 4)
    return ReportTrunc ();
  f = csConvertEndian (f);
  return true;
}

bool celPersistClassic::Read (csString& str)
{
  uint16 l;
  if (!Read (l)) return false;
  if (l)
  {
    char* newstr = new char[l+1];
    if (file->Read((char*) newstr, l) < l)
    {
      delete [] newstr;
      str.Empty ();
      return ReportTrunc ();
    }
    newstr[l] = 0;
    str = newstr;
    delete[] newstr;
  }
  else str.Empty ();
  return true;
}

bool celPersistClassic::Read (char*& str)
{
  uint16 l;
  if (!Read (l)) return false;
  if (l)
  {
    str = new char[l+1];
    if (file->Read((char*) str, l) < l)
    {
      delete [] str;
      str = 0;
      return ReportTrunc ();
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
      return Report ("Bad type in persisted data!");
    case CEL_DATA_BOOL:
      if (!Read (ub)) return false;
      cd->Set (ub);
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
      return Report ("Data type iBase is not allowed for persistence!");
    default:
      return Report ("Found unknown Data type!");
  }
  return true;
}

bool celPersistClassic::Read (csRef<iCelDataBuffer>& db)
{
  int32 ser;
  db = 0;
  if (!Read (ser)) return false;
  uint16 cnt;
  if (!Read (cnt)) return false;
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  db = pl->CreateDataBuffer (ser);
  int i;
  for (i = 0 ; i < cnt ; i++)
    if (!Read (db->AddData ()))
      return Report ("Error reading data entry %d!", i);

  return true;
}

bool celPersistClassic::Read (iCelEntity* entity, iCelPropertyClass*& pc)
{
  char marker[5];
  pc = 0;
  if (!ReadMarker (marker))
    return false;
  marker[4]=0;
  if (strncmp (marker, "PCL", 3))
    return Report ("Expected property class, got something else: '%s'!",marker);
  if (marker[3] == '0')
  {
    // 0 entity.
    return true;
  }
  if (marker[3] == 'E')
  {
    csRef<iCelDataBuffer> db;
    if (!Read (db))
      return Report ("Error reading external property class reference!");
    pc = set->FindExternalPC (db);
    return true;
  }

  // Either we have a local ref or not. But in any case
  // we first want to try to find if the pc already exists. In that
  // case we reuse it even if it is not a ref.
  if (marker[3] == 'R')
  {
    // A reference.
    uint32 entid;
    if (!Read (entid))
      return false;
    entity = set->GetEntity (entid);
  }
  else if (marker[3] != 'I')
    return Report ("Bad property class marker '%s'!", marker);

  csString pcname;
  if (!Read (pcname))
    return false;
  csString tagname;
  if (!Read (tagname))
    return false;
  if (tagname)
    pc = entity->GetPropertyClassList ()->FindByNameAndTag (pcname, tagname);
  else
    pc = entity->GetPropertyClassList ()->FindByName (pcname);

  bool loadpcdata = true;
  if (marker[3] == 'R')
  {
    // A reference.
    if (pc) return true;

    // We couldn't find the pc. So we create it here by falling through
    // to the next code. Later on that pc will be picked up.
    // We set loadpcdata to false because we don't want to load the
    // PC data here. We only want to create it so that it can be used
    // later.
    loadpcdata = false;
  }

  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pc)
  {
    iCelPropertyClassFactory* pf = pl->FindPropertyClassFactory (pcname);
    if (!pf)
      return Report ("Couldn't create property class '%s'!",
      	(const char*)pcname);
    csRef<iCelPropertyClass> pcref (pf->CreatePropertyClass());
    pc = pcref;
    if (tagname) pc->SetTag (tagname);
    pc->SetEntity (entity);
    printf ("Adding PC '%s' to Entity '%s'\n", (const char*)pcname,
    	entity->GetName());
    entity->GetPropertyClassList ()->Add (pc);
  }
  if (loadpcdata)
  {
    csRef<iCelDataBuffer> db;
    if (!Read (db))
      return Report ("Error loading property class '%s'!", (const char*)pcname);
    if (!pc->Load (db))
      return Report ("Error loading property class '%s'!", (const char*)pcname);
  }

  return true;
}

bool celPersistClassic::Read (iCelEntity*& entity)
{
  char marker[5];
  if (!ReadMarker (marker))
    return false;
  marker[4]='\0';
  if (strncmp (marker, "ENT", 3))
    return Report ("Expected entity, got something else: %s",marker);
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
      return Report ("Expected entity ID, got something else!");
    entity = set->GetEntity (entid);
    return true;
  }
  else if (marker[3] == 'E')
  {
    csRef<iCelDataBuffer> db;
    if (!Read (db))
      return Report ("Error reading external entity reference!");
    entity = set->FindExternalEntity (db);
  }
  else if (marker[3] == 'I')
  {
    // In this case we know the entity is already given to this
    // routine.
    csString entname, bhname, bhlayername;
    if (!Read (entname)) return false;
    uint16 c;
    if (!Read (c)) return false;

    // An entity.
    entity->SetName (entname);
    printf ("  Reading entity %lu ('%s')...\n",
	    (unsigned long)entity->GetID(), (const char*)entname);

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

    if (!Read (bhlayername))
      return false;
    if (!Read (bhname))
      return false;
    if (!bhlayername.IsEmpty () && !bhname.IsEmpty ())
    {
      csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      iCelBlLayer* bl = pl->FindBehaviourLayer (bhlayername);
      if (!bl) return Report ("Couldn't find behaviour layer '%s'!",
      	(const char*)bhlayername);
      iCelBehaviour* bh = bl->CreateBehaviour (entity, bhname);
      if (!bh) return Report ("Couldn't create behaviour '%s'!",
      	(const char*)bhname);
      bh->DecRef ();
    }
  }
  else return Report ("Bad entity marker!");

  return true;
}

bool celPersistClassic::ReadFirstPass (iCelEntity* entity)
{
  if (!CheckMarker ("ENT1"))
    return false;
  uint8 c;
  if (!Read (c)) return false;
  while (c)
  {
    if (!CheckMarker ("PCL1"))
      return false;
    csString pcname;
    if (!Read (pcname)) return false;
    csString tagname;
    if (!Read (tagname)) return false;

    // @@@ Not efficient! Store pl!
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    iCelPropertyClassFactory* pf = pl->FindPropertyClassFactory (pcname);
    if (!pf)
      return Report ("Couldn't create property class '%s'!",
      	(const char*)pcname);
    csRef<iCelPropertyClass> pc = pf->CreatePropertyClass();
    if (tagname) pc->SetTag (tagname);
    pc->SetEntity (entity);
    entity->GetPropertyClassList ()->Add (pc);
    csRef<iCelDataBuffer> db;
    if (!Read (db))
      return Report ("Error loading property class '%s'!", (const char*)pcname);
    if (!pc->LoadFirstPass (db))
      return Report ("Error loading property class '%s'!", (const char*)pcname);

    if (!Read (c)) return false;
  }

  return true;
}

//------------------------------------------------------------------------

#define CEL_MARKER "CEL1"
bool celPersistClassic::Load (iCelLocalEntitySet* set, iFile* file)
{
  this->file = file;
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

  // --- First pass -----------------------------------
  for (i = 0 ; i < cnt ; i++)
  {
    // @@@ Not efficient! Store pl!
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    csRef<iCelEntity> ent = pl->CreateEntity ();
    entities_map.Put ((iCelEntity*)ent, i);
    set->AddEntity (ent);
    if (!ReadFirstPass (ent)) return false;
  }

  // --- Second pass ----------------------------------
  if (!CheckMarker ("SECO")) return false;
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

bool celPersistClassic::Load (iCelLocalEntitySet* set, const char* name)
{
  csRef<iFile> file = vfs->Open (name, VFS_FILE_READ);
  if (!file)
    return Report ("Failed to read file '%s'!", name);
  return Load (set, file);
}

bool celPersistClassic::Save (iCelLocalEntitySet* set, iFile* file)
{
  this->file = file;
  celPersistClassic::set = set;

  if (!WriteMarker (CEL_MARKER)) return false;

  if (!Write ((uint32)(set->GetEntityCount ()))) return false;
  size_t i;
  entities_map.DeleteAll ();
  for (i = 0 ; i < set->GetEntityCount () ; i++)
    entities_map.Put (set->GetEntity (i), i);

  // --- First pass -----------------------------------
  for (i = 0 ; i < set->GetEntityCount () ; i++)
  {
    iCelEntity* ent = set->GetEntity (i);
    if (!WriteFirstPass (ent)) return false;
  }

  // --- Second pass ----------------------------------
  if (!WriteMarker ("SECO")) return false;
  for (i = 0 ; i < set->GetEntityCount () ; i++)
  {
    iCelEntity* ent = set->GetEntity (i);
    if (!Write (ent, true)) return false;
  }

  return true;
}

bool celPersistClassic::Save (iCelLocalEntitySet* set, const char* name)
{
  csMemFile m;
  csRef<iFile> mf = SCF_QUERY_INTERFACE (&m, iFile);

  if (Save (set, mf))
  {
    vfs->WriteFile (name, m.GetData (), m.GetSize ());
    return true;
  }
  else
  {
    return false;
  }
}

//------------------------------------------------------------------------

