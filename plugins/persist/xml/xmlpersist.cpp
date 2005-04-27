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
#include "plugins/persist/xml/xmlpersist.h"
#include "csutil/csendian.h"
#include "csutil/memfile.h"
#include "csutil/cscolor.h"
#include "csutil/xmltiny.h"
#include "iutil/vfs.h"
#include "iutil/objreg.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

// define this to track save/loading problems
// #define PERSIST_DEBUG

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPersistXML)

SCF_IMPLEMENT_IBASE (celPersistXML)
  SCF_IMPLEMENTS_INTERFACE (iCelPersistence)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPersistXML::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPersistXML::celPersistXML (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  object_reg = 0;
}

celPersistXML::~celPersistXML ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celPersistXML::Initialize (iObjectRegistry* object_reg)
{
  celPersistXML::object_reg = object_reg;

  vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  if (!vfs) return false;
  
  return true;
}

void celPersistXML::Report (const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persist.xml",
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

bool celPersistXML::Write (iDocumentNode* node, iCelDataBuffer* db)
{
  node->SetAttributeAsInt ("serial", db->GetSerialNumber ());
  size_t i;
  for (i = 0 ; i < db->GetDataCount () ; i++)
  {
    if (!Write (node, db->GetData (i)))
      return false;
  }
  return true;
}

bool celPersistXML::Write (iDocumentNode* node, celData* data)
{
  csRef<iDocumentNode> attrnode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
  switch (data->type)
  {
    case CEL_DATA_NONE:
    case CEL_DATA_ACTION:
      CS_ASSERT (false);
      break;
    case CEL_DATA_BOOL:
      attrnode->SetValue ("bool");
      attrnode->SetAttributeAsInt ("v", int (data->value.bo));
      break;
    case CEL_DATA_BYTE:
      attrnode->SetValue ("byte");
      attrnode->SetAttributeAsInt ("v", int (data->value.b));
      break;
    case CEL_DATA_WORD:
      attrnode->SetValue ("word");
      attrnode->SetAttributeAsInt ("v", int (data->value.w));
      break;
    case CEL_DATA_LONG:
      attrnode->SetValue ("long");
      attrnode->SetAttributeAsInt ("v", int (data->value.l));
      break;
    case CEL_DATA_UBYTE:
      attrnode->SetValue ("ubyte");
      attrnode->SetAttributeAsInt ("v", int (data->value.ub));
      break;
    case CEL_DATA_UWORD:
      attrnode->SetValue ("uworld");
      attrnode->SetAttributeAsInt ("v", int (data->value.uw));
      break;
    case CEL_DATA_ULONG:
      attrnode->SetValue ("ulong");
      attrnode->SetAttributeAsInt ("v", int (data->value.ul));
      break;
    case CEL_DATA_FLOAT:
      attrnode->SetValue ("float");
      attrnode->SetAttributeAsFloat ("v", data->value.f);
      break;
    case CEL_DATA_VECTOR2:
      attrnode->SetValue ("vector2");
      attrnode->SetAttributeAsFloat ("x", data->value.v.x);
      attrnode->SetAttributeAsFloat ("y", data->value.v.y);
      break;
    case CEL_DATA_VECTOR3:
      attrnode->SetValue ("vector3");
      attrnode->SetAttributeAsFloat ("x", data->value.v.x);
      attrnode->SetAttributeAsFloat ("y", data->value.v.y);
      attrnode->SetAttributeAsFloat ("z", data->value.v.z);
      break;
    case CEL_DATA_COLOR:
      attrnode->SetValue ("color");
      attrnode->SetAttributeAsFloat ("r", data->value.col.red);
      attrnode->SetAttributeAsFloat ("g", data->value.col.green);
      attrnode->SetAttributeAsFloat ("b", data->value.col.blue);
      break;
    case CEL_DATA_STRING:
      attrnode->SetValue ("string");
      if (!data->value.s->IsEmpty ())
        attrnode->SetAttribute ("v", *data->value.s);
      break;
    case CEL_DATA_PCLASS:
      attrnode->SetValue ("pc");
      if (!Write (attrnode, data->value.pc, false)) return false;
      break;
    case CEL_DATA_ENTITY:
      attrnode->SetValue ("entity");
      if (!Write (attrnode, data->value.ent, false)) return false;
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

bool celPersistXML::Write (iDocumentNode* pcnode,
	iCelPropertyClass* pc, bool savelocal)
{
  if (!pc)
  {
    // 0 pc.
    pcnode->SetAttribute ("null", "true");
    return true;
  }
  if (!set->IsLocal (pc))
  {
    // This is a non-local pc.
    pcnode->SetAttribute ("extref", "true");
    csRef<iCelDataBuffer> db = set->SaveExternalPC (pc);
    return Write (pcnode, db);
  }
  else if (!savelocal)
  {
    // This is a local pc and we're not saving local pc's.
    // In that case we still have to save a reference.
    size_t entity_idx = entities_map.Get (pc->GetEntity (),
    	csArrayItemNotFound);
    CS_ASSERT(entity_idx != csArrayItemNotFound);
    pcnode->SetAttributeAsInt ("locref", entity_idx);
    pcnode->SetAttribute ("name", pc->GetName ());
    if (pc->GetTag ())
      pcnode->SetAttribute ("tag", pc->GetTag ());
    return true;
  }

  if (pc->GetName ())
    pcnode->SetAttribute ("name", pc->GetName ());
  if (pc->GetTag ())
    pcnode->SetAttribute ("tag", pc->GetTag ());

  csRef<iCelDataBuffer> db = pc->Save ();
  if (!db)
  {
    printf ("pc '%s' doesn't support saving!\n", pc->GetName ());
    fflush (stdout);
    return false;
  }
  return Write (pcnode, db);
}

bool celPersistXML::Write (iDocumentNode* entnode,
	iCelEntity* entity, bool savelocal)
{
  if (!entity)
  {
    // 0 entity.
    entnode->SetAttribute ("null", "true");
    return true;
  }
  if (!set->IsLocal (entity))
  {
    // This is a non-local entity.
    entnode->SetAttribute ("extref", "true");
    csRef<iCelDataBuffer> db = set->SaveExternalEntity (entity);
    return Write (entnode, db);
  }
  else if (!savelocal)
  {
    // This is a local entity and we're not saving local entities.
    // In that case we still have to save a reference.
    size_t idx = entities_map.Get (entity, csArrayItemNotFound);
    CS_ASSERT(idx != csArrayItemNotFound);
    entnode->SetAttributeAsInt ("locref", idx);
    return true;
  }

  if (entity->GetName ())
    entnode->SetAttribute ("name", entity->GetName ());

  iCelPropertyClassList* pl = entity->GetPropertyClassList ();
  size_t i;
  for (i = 0 ; i < pl->GetCount () ; i++)
  {
    csRef<iDocumentNode> pcnode = entnode->CreateNodeBefore (
    	CS_NODE_ELEMENT, 0);
    pcnode->SetValue ("pc");
    if (!Write (pcnode, pl->Get (i), true))
    {
      printf ("Error writing pc!\n"); fflush (stdout);
      return false;	// @@@ Report
    }
  }

  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    csRef<iDocumentNode> behnode = entnode->CreateNodeBefore (
    	CS_NODE_ELEMENT, 0);
    behnode->SetValue ("behaviour");
    behnode->SetAttribute ("layer", bh->GetBehaviourLayer ()->GetName ());
    behnode->SetAttribute ("name", bh->GetName ());
  }
  return true;
}

//------------------------------------------------------------------------

bool celPersistXML::ReadMarker (char* marker)
{
  if (file->Read (marker, 4) < 4)
    return false;
  return true;
}

bool celPersistXML::CheckMarker (const char* comp)
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

bool celPersistXML::Read (int8& b)
{
  if (file->Read((char*) &b, 1) < 1)
    return false;
  return true;
}

bool celPersistXML::Read (uint8& ub)
{
  if (file->Read((char*) &ub, 1) < 1)
    return false;
  return true;
}

bool celPersistXML::Read (int16& w)
{
  if (file->Read((char*) &w, 2) < 2)
    return false;
  w = csGetLittleEndianShort (&w);
  return true;
}

bool celPersistXML::Read (uint16& uw)
{
  if (file->Read((char*) &uw, 2) < 2)
    return false;
  uw = csGetLittleEndianShort (&uw);
  return true;
}

bool celPersistXML::Read (int32& l)
{
  if (file->Read((char*) &l, 4) < 4)
    return false;
  l = csGetLittleEndianLong (&l);
  return true;
}

bool celPersistXML::Read (uint32& ul)
{
  if (file->Read((char*) &ul, 4) < 4)
    return false;
  ul = csGetLittleEndianLong (&ul);
  return true;
}

bool celPersistXML::Read (float& f)
{
  if (file->Read((char*) &f, 4) < 4)
    return false;
  f = csConvertEndian (f);
  return true;
}

bool celPersistXML::Read (char*& str)
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

bool celPersistXML::Read (celData* cd)
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

bool celPersistXML::Read (iCelDataBuffer*& db)
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

bool celPersistXML::Read (iCelEntity* entity, iCelPropertyClass*& pc)
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
    bool rc = true;
    rc = rc && Read (entid);
    rc = rc && Read (pcname);
    if (rc)
    {
      iCelEntity* entity = set->GetEntity (entid);
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

bool celPersistXML::Read (iCelEntity*& entity)
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
bool celPersistXML::Load (iCelLocalEntitySet* set, const char* name)
{
  csRef<iDataBuffer> data = vfs->ReadFile (name);
  if (!data)
    return false;

  csMemFile mf ((const char*) data->GetData(), data->GetSize());
  file = &mf;
  celPersistXML::set = set;

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

bool celPersistXML::Save (iCelLocalEntitySet* set, const char* name)
{
  celPersistXML::set = set;
  csRef<iDocumentSystem> xml = CS_QUERY_REGISTRY (object_reg, iDocumentSystem);
  if (!xml)
    xml.AttachNew (new csTinyDocumentSystem ());
  csRef<iDocument> doc = xml->CreateDocument ();
  csRef<iDocumentNode> root = doc->CreateRoot ();
  csRef<iDocumentNode> parent = root->CreateNodeBefore (CS_NODE_ELEMENT, 0);
  parent->SetValue ("celentities");

  size_t i;
  entities_map.DeleteAll ();
  for (i = 0 ; i < set->GetEntityCount () ; i++)
    entities_map.Put (set->GetEntity (i), i);
  for (i = 0 ; i < set->GetEntityCount () ; i++)
  {
    iCelEntity* ent = set->GetEntity (i);
    csRef<iDocumentNode> entnode = parent->CreateNodeBefore (
    	CS_NODE_ELEMENT, 0);
    entnode->SetValue ("entity");
    if (!Write (entnode, ent, true))
    {
      printf ("Error writing entity!\n"); fflush (stdout);
      return false;	// @@@ Report
    }
  }

  if (doc->Write (vfs, name) != 0)
  {
    printf ("Error writing doc!\n"); fflush (stdout);
    return false;	// @@@ Report
  }

  return true;
}

//------------------------------------------------------------------------

