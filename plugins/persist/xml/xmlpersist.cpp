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

bool celPersistXML::Read (iDocumentNode* node, celData* cd)
{
  // @@@ Using a tokenizer would be better here.

  const char* value = node->GetValue ();
  if (!strcmp ("bool", value))
  {
    cd->Set ((bool)(node->GetAttributeValueAsInt ("v")));
  }
  else if (!strcmp ("byte", value))
  {
    cd->Set ((int8)(node->GetAttributeValueAsInt ("v")));
  }
  else if (!strcmp ("ubyte", value))
  {
    cd->Set ((uint8)(node->GetAttributeValueAsInt ("v")));
  }
  else if (!strcmp ("world", value))
  {
    cd->Set ((int16)(node->GetAttributeValueAsInt ("v")));
  }
  else if (!strcmp ("uworld", value))
  {
    cd->Set ((uint16)(node->GetAttributeValueAsInt ("v")));
  }
  else if (!strcmp ("long", value))
  {
    cd->Set ((int32)(node->GetAttributeValueAsInt ("v")));
  }
  else if (!strcmp ("ulong", value))
  {
    cd->Set ((uint32)(node->GetAttributeValueAsInt ("v")));
  }
  else if (!strcmp ("float", value))
  {
    cd->Set (node->GetAttributeValueAsFloat ("v"));
  }
  else if (!strcmp ("string", value))
  {
    cd->Set (node->GetAttributeValue ("v"));
  }
  else if (!strcmp ("color", value))
  {
    csColor v;
    v.red = node->GetAttributeValueAsFloat ("r");
    v.green = node->GetAttributeValueAsFloat ("g");
    v.blue = node->GetAttributeValueAsFloat ("b");
    cd->Set (v);
  }
  else if (!strcmp ("vector2", value))
  {
    csVector2 v;
    v.x = node->GetAttributeValueAsFloat ("x");
    v.y = node->GetAttributeValueAsFloat ("y");
    cd->Set (v);
  }
  else if (!strcmp ("vector3", value))
  {
    csVector3 v;
    v.x = node->GetAttributeValueAsFloat ("x");
    v.y = node->GetAttributeValueAsFloat ("y");
    v.z = node->GetAttributeValueAsFloat ("z");
    cd->Set (v);
  }
  else if (!strcmp ("pc", value))
  {
    iCelPropertyClass* pc;
    if (!Read (node, 0, pc)) return false;
    cd->Set (pc);
  }
  else if (!strcmp ("entity", value))
  {
    iCelEntity* ent;
    if (!Read (node, ent)) return false;
    cd->Set (ent);
  }
  else
  {
    Report ("Found unknown Data type '%s'!", value);
    return false;
  }
  return true;
}

bool celPersistXML::Read (iDocumentNode* node, iCelDataBuffer*& db)
{
  int32 ser = (int32)node->GetAttributeValueAsInt ("serial");
  // @@@ Not efficient.
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  csRef<iCelDataBuffer> dbref = pl->CreateDataBuffer (ser);
  db = dbref;

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    if (!Read (child, db->AddData ()))
    {
      Report ("Error reading data entry %s!", child->GetValue ());
      return false;
    }
  }

  if (dbref) dbref->IncRef (); // Avoid smart pointer release.
  return true;
}

bool celPersistXML::Read (iDocumentNode* pcnode,
	iCelEntity* entity, iCelPropertyClass*& pc)
{
  csRef<iDocumentAttribute> attr;
  attr = pcnode->GetAttribute ("null");
  if (attr != 0)
  {
    pc = 0;
    return true;
  }
  attr = pcnode->GetAttribute ("extref");
  if (attr != 0)
  {
    iCelDataBuffer* db;
    if (!Read (pcnode, db))
    {
      Report ("Error reading external property class reference!");
      return false;
    }
    pc = set->FindExternalPC (db); // @@@ Check?
    db->DecRef ();
    return true;
  }
  attr = pcnode->GetAttribute ("locref");
  if (attr != 0)
  {
    // A reference.
    const char* pcname = pcnode->GetAttributeValue ("name");
    uint32 entid = (uint32)attr->GetValueAsInt ();
    entity = set->GetEntity (entid);
    if (!entity)
    {
      Report ("Cannot find entity for '%s'!", pcname);
      return false;
    }
    const char* tag = pcnode->GetAttributeValue ("tag");
    if (tag)
      pc = entity->GetPropertyClassList ()->FindByNameAndTag (pcname, tag);
    else
      pc = entity->GetPropertyClassList ()->FindByName (pcname);
    if (!pc)
    {
      Report ("Cannot find property class '%s' for entity '%s'!",
		pcname, entity->GetName());
      return false;
    }
    return true;
  }

  const char* pcname = pcnode->GetAttributeValue ("name");
  const char* tagname = pcnode->GetAttributeValue ("tag");

  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelPropertyClassFactory* pf = pl->FindPropertyClassFactory (pcname);
  if (!pf)
  {
    Report ("Couldn't create property class '%s'!", pcname);
    return false;
  }
  csRef<iCelPropertyClass> pcref = pf->CreatePropertyClass();
  pc = pcref;
  if (tagname) pc->SetTag (tagname);
  iCelDataBuffer* db;
  if (!Read (pcnode, db))
  {
    Report ("Error loading property class '%s'!", pcname);
    return false;
  }
  pc->SetEntity (entity);
  if (!pc->Load (db))
  {
    Report ("Error loading property class '%s'!", pcname);
    return false;
  }
  else
  {
    Report ("Adding PC '%s' to Entity '%s'", pcname, entity->GetName());
    entity->GetPropertyClassList ()->Add (pc);
  }
  db->DecRef ();

  return true;
}

bool celPersistXML::Read (iDocumentNode* entnode, iCelEntity*& entity)
{
  csRef<iDocumentAttribute> attr;
  attr = entnode->GetAttribute ("null");
  if (attr != 0)
  {
    entity = 0;
    return true;
  }
  attr = entnode->GetAttribute ("extref");
  if (attr != 0)
  {
    iCelDataBuffer* db;
    if (!Read (entnode, db))
    {
      Report ("Error reading external entity reference!");
      return false;
    }
    entity = set->FindExternalEntity (db);
    db->DecRef ();
    return true;
  }
  attr = entnode->GetAttribute ("locref");
  if (attr != 0)
  {
    // A reference.
    uint32 entid = (uint32)attr->GetValueAsInt ();
    entity = set->GetEntity (entid);
    return true;
  }

  // In this case we know the entity is already given to this
  // routine.
  const char* entname = entnode->GetAttributeValue ("name");
  if (entname) entity->SetName (entname);

  csRef<iDocumentNodeIterator> it = entnode->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> pcnode = it->Next ();
    if (pcnode->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = pcnode->GetValue ();
    if (!strcmp ("pc", value))
    {
      iCelPropertyClass* pc;
      if (!Read (pcnode, entity, pc))
      {
	entity = 0;
	return false;
      }
    }
    else if (!strcmp ("behaviour", value))
    {
      const char* layer = pcnode->GetAttributeValue ("layer");
      const char* bname = pcnode->GetAttributeValue ("name");
      csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      iCelBlLayer* bl = pl->FindBehaviourLayer (layer);
      if (!bl) return false;
      iCelBehaviour* bh = bl->CreateBehaviour (entity, bname);
      if (!bh) return false;
      bh->DecRef ();
    }
    else
    {
      Report ("File doesn't seem to be valid: expected 'pc' in entity '%s'!",
      	entname);
      return false;
    }
  }

  return true;
}

//------------------------------------------------------------------------

bool celPersistXML::Load (iCelLocalEntitySet* set, const char* name)
{
  csRef<iDataBuffer> data = vfs->ReadFile (name);
  if (!data)
  {
    Report ("Failed to read file '%s'!", name);
    return false;
  }

  csRef<iDocumentSystem> xml = CS_QUERY_REGISTRY (object_reg, iDocumentSystem);
  if (!xml)
    xml.AttachNew (new csTinyDocumentSystem ());
  csRef<iDocument> doc = xml->CreateDocument ();
  const char* error = doc->Parse (data, true);
  if (error != 0)
  {
    Report ("Error parsing document '%s': '%s'!", name, error);
    return false;
  }

  celPersistXML::set = set;
  iDocumentNode* root = doc->GetRoot ();
  csRef<iDocumentNode> parent = root->GetNode ("celentities");
  if (!parent)
  {
    Report ("File '%s' doesn't seem to be a valid XML save file!", name);
    return false;
  }

  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);

  size_t idx = 0;
  entities_map.DeleteAll ();
  csRef<iDocumentNodeIterator> it = parent->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> entnode = it->Next ();
    if (entnode->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = entnode->GetValue ();
    if (!strcmp ("entity", value))
    {
      csRef<iCelEntity> ent = pl->CreateEntity ();
      entities_map.Put (ent, idx);
      set->AddEntity (ent);
      idx++;
    }
    else
    {
      Report ("File '%s' doesn't seem to be valid: expected 'entity'!", name);
      return false;
    }
  }

  // Loop again to actually load the entities.
  idx = 0;
  it = parent->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> entnode = it->Next ();
    if (entnode->GetType () != CS_NODE_ELEMENT) continue;
    // No need to check for entity anymore. Previous loop does that.
    iCelEntity* ent = set->GetEntity (idx);
    idx++;
    if (!Read (entnode, ent))
      return false;
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

