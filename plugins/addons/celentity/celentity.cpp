/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein

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
#include "csutil/scanstr.h"
#include "csutil/xmltiny.h"
#include "iutil/objreg.h"
#include "iutil/document.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "ivaria/reporter.h"
#include "imap/services.h"
#include "iengine/mesh.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "behaviourlayer/bl.h"
#include "propclass/mesh.h"

#include "celentity.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celAddOnCelEntity)

enum
{
  XMLTOKEN_BEHAVIOUR,
  XMLTOKEN_PROPCLASS,
  XMLTOKEN_PROPERTY,
  XMLTOKEN_ACTION,
  XMLTOKEN_PARAMS,
  XMLTOKEN_CALL,
  XMLTOKEN_CLASS,

  XMLTOKEN_FLOAT,
  XMLTOKEN_BOOL,
  XMLTOKEN_STRING,
  XMLTOKEN_LONG,
  XMLTOKEN_VECTOR,
  XMLTOKEN_COLOR
};


celAddOnCelEntity::celAddOnCelEntity (iBase* parent) :
  scfImplementationType (this, parent)
{
  object_reg = 0;
}

celAddOnCelEntity::~celAddOnCelEntity ()
{
}

bool celAddOnCelEntity::Initialize (iObjectRegistry* object_reg)
{
  celAddOnCelEntity::object_reg = object_reg;
  synldr = csQueryRegistry<iSyntaxService> (object_reg);
  if (!synldr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cel.addons.celentity",
        "Can't find syntax services!");
    return false;
  }
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cel.addons.celentity",
        "Can't find physical layer!");
    return false;
  }

  xmltokens.Register ("behaviour", XMLTOKEN_BEHAVIOUR);
  xmltokens.Register ("propclass", XMLTOKEN_PROPCLASS);
  xmltokens.Register ("property", XMLTOKEN_PROPERTY);
  xmltokens.Register ("action", XMLTOKEN_ACTION);
  xmltokens.Register ("params", XMLTOKEN_PARAMS);
  xmltokens.Register ("call", XMLTOKEN_CALL);
  xmltokens.Register ("class", XMLTOKEN_CLASS);

  xmltokens.Register ("float", XMLTOKEN_FLOAT);
  xmltokens.Register ("bool", XMLTOKEN_BOOL);
  xmltokens.Register ("string", XMLTOKEN_STRING);
  xmltokens.Register ("long", XMLTOKEN_LONG);
  xmltokens.Register ("vector", XMLTOKEN_VECTOR);
  xmltokens.Register ("color", XMLTOKEN_COLOR);

  return true;
}

const char* celAddOnCelEntity::GetAttributeValue (iDocumentNode* child,
        const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
        "cel.addons.celentity", child,
        "Can't find attribute '%s'!", propname);
    return 0;
  }
  return rc;
}

csStringID celAddOnCelEntity::GetAttributeID (iDocumentNode* child,
        const char* prefix, const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
        "cel.addons.celentity", child,
        "Can't find attribute '%s'!", propname);
    return csInvalidStringID;
  }
  csString p (prefix);
  p += rc;
  return pl->FetchStringID ((const char*)p);
}

csStringID celAddOnCelEntity::GetAttributeID (iDocumentNode* child,
        const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
        "cel.addons.celentity", child,
        "Can't find attribute '%s'!", propname);
    return csInvalidStringID;
  }
  return pl->FetchStringID (rc);
}

bool celAddOnCelEntity::ParseProperties (iCelPropertyClass* pc,
        iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_PROPERTY:
        {
          csStringID propid = GetAttributeID (child, "name");
          if (propid == csInvalidStringID) return false;

          csRef<iDocumentAttributeIterator> attr_it = child->GetAttributes ();
          while (attr_it->HasNext ())
          {
            csRef<iDocumentAttribute> attr = attr_it->Next ();
            const char* attr_name = attr->GetName ();
            csStringID attr_id = xmltokens.Request (attr_name);
            switch (attr_id)
            {
              case XMLTOKEN_FLOAT:
                pc->SetProperty (propid, attr->GetValueAsFloat ());
                break;
              case XMLTOKEN_STRING:
                pc->SetProperty (propid, attr->GetValue ());
                break;
              case XMLTOKEN_BOOL:
                {
                  const char* v = attr->GetValue ();
                  bool b;
                  csScanStr (v, "%b", &b);
                  pc->SetProperty (propid, b);
                }
                break;
              case XMLTOKEN_LONG:
                pc->SetProperty (propid, (long)attr->GetValueAsInt ());
                break;
              case XMLTOKEN_VECTOR:
                {
                  csVector3 v;
                  int rc = csScanStr (attr->GetValue (), "%f,%f,%f", &v.x,
                            &v.y, &v.z);
                  if (rc == 3)
                    pc->SetProperty (propid, v);
                  else
                  {
                    csVector2 v2;
                    csScanStr (attr->GetValue (), "%f,%f", &v2.x, &v2.y);
                    pc->SetProperty (propid, v2);
                  }
                }
                break;
              case XMLTOKEN_COLOR:
                {
                  csColor v;
                  csScanStr (attr->GetValue (), "%f,%f,%f",
                              &v.red, &v.green, &v.blue);
                  pc->SetProperty (propid, v);
                }
                break;
            }
          }
        }
        break;
      case XMLTOKEN_ACTION:
        {
          csStringID propid = GetAttributeID (child, "name");
          if (propid == csInvalidStringID) return false;
          csRef<iCelParameterBlock> params = celParameterTools::ParseParams (
            object_reg, child, CEL_PARAM_END);
          celData ret;
          pc->PerformAction (propid, params, ret);
        }
        break;
      default:
        synldr->ReportBadToken (child);
        break;
    }
  }
  return true;
}

csPtr<iBase> celAddOnCelEntity::Parse (iDocumentNode* node,
        iStreamSource*, iLoaderContext* ldr_context, iBase* context)
{
  if (pl->IsEntityAddonAllowed ())
  {
    // If the context is not a mesh we will create a standalone entity.
    csRef<iMeshWrapper> mesh = scfQueryInterface<iMeshWrapper> (context);
    iCelEntity* ent = Load (node, mesh);
    csRef<iBase> ent_return = (iBase*)ent;
    return csPtr<iBase> (ent_return);
  }
  else
  {
    // Entity addons are not allowed. Do nothing.
    // IncRef because we return a csPtr. We have to return something
    // that is not 0 here.
    IncRef ();
    return (iLoaderPlugin*)this;
  }
}

iCelEntity* celAddOnCelEntity::Load (const char* path, const char* file,
    iMeshWrapper* mesh)
{
  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  if (path)
  {
    vfs->PushDir ();
    vfs->ChDir (path);
  }

  csRef<iFile> buf = vfs->Open (file, VFS_FILE_READ);
  if (!buf)
  {
    if (path)
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
        "cel.addons.celentity",
        "Can't load file '%s' from '%s'!", file, path);
    else
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
        "cel.addons.celentity",
        "Can't load file '%s'!", file);
    return 0;
  }

  csRef<iDocumentSystem> docsys = csQueryRegistry<iDocumentSystem> (object_reg);
  if (!docsys) docsys = csPtr<iDocumentSystem> (new csTinyDocumentSystem ());
  csRef<iDocument> doc = docsys->CreateDocument ();
  const char* error = doc->Parse (buf, true);
  if (error != 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cell.addons.celentity",
            "Document system error for file '%s': %s!", file, error);
    return 0;
  }
  iCelEntity* entity = Load (doc->GetRoot ()->GetNode ("addon"), mesh);

  if (path)
  {
    vfs->PopDir ();
  }

  return entity;
}

iCelEntity* celAddOnCelEntity::Load (iDocumentNode* node, iMeshWrapper* mesh)
{
  csRef<iCelEntity> ent;

  const char* templatename = node->GetAttributeValue ("template");
  const char* entityname = node->GetAttributeValue ("entityname");
  if (!entityname && mesh)
    entityname = mesh->QueryObject ()->GetName ();

  if (templatename)
  {
    iCelEntityTemplate* templ = pl->FindEntityTemplate (templatename);
    if (!templ)
    {
      synldr->ReportError (
        "cel.addons.celentity",
        node, "Can't find entity template '%s'!", templatename);
      return 0;
    }

    csRef<iDocumentNode> paramsnode = node->GetNode ("params");
    celData thisdata;
    thisdata.Set (entityname);
    csRef<iCelParameterBlock> params = celParameterTools::ParseParams (
            object_reg, paramsnode, "this", &thisdata, CEL_PARAM_END);

    ent = pl->CreateEntity (templ, entityname, params);
  }
  else
  {
    ent = pl->CreateEntity ();
    ent->SetName (entityname);
  }

  iCelPropertyClass* pc;
  if (mesh)
  {
    // If we have a mesh we also create a pcmesh property class.
    pc = pl->CreatePropertyClass (ent, "pcobject.mesh");
    csRef<iPcMesh> pcmesh = scfQueryInterface<iPcMesh> (pc);
    pcmesh->SetMesh (mesh);
  }

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_BEHAVIOUR:
      {
        csRef<iCelBlLayer> bl;
        const char* blname = child->GetAttributeValue ("layer");
        if (blname)
        {
          bl = csQueryRegistryTagInterface<iCelBlLayer> ( object_reg, blname);
          if (!bl) bl = pl->FindBehaviourLayer (blname);
        }
        else
        {
          bl = csQueryRegistry<iCelBlLayer> (object_reg);
        }
        if (!bl)
        {
          synldr->ReportError (
              "cel.addons.celentity",
              child, "Can't find the needed behaviour layer!");
          return 0;
        }
        const char* behavename = child->GetAttributeValue ("name");
        if (!behavename)
        {
          synldr->ReportError (
              "cel.addons.celentity",
              child, "Name of the behaviour is missing!");
          return 0;
        }
        iCelBehaviour* behave = bl->CreateBehaviour (ent, behavename);
        if (!behave)
        {
          synldr->ReportError (
              "cel.addons.celentity",
              child, "Couldn't create behaviour with name '%s'!",
              behavename);
          return 0;
        }
        break;
      }
      case XMLTOKEN_PROPCLASS:
      {
        LoadPropclass (child, ent);
        break;
      }
      case XMLTOKEN_CLASS:
      {
        csStringID cls = pl->FetchStringID (child->GetContentsValue ());
        ent->AddClass (cls);
        break;
      }
      case XMLTOKEN_CALL:
      {
        if (!ent->GetBehaviour ())
        {
          synldr->ReportError (
              "cel.addons.celentity",
              child, "Behaviour is missing for 'call'!");
          return 0;
        }
        csRef<iCelParameterBlock> params = celParameterTools::ParseParams (
            object_reg, child, CEL_PARAM_END);
        if (!params) return 0;
        const char* msgid = child->GetAttributeValue ("event");
        if (!msgid)
        {
          synldr->ReportError (
              "cel.addons.celentity",
              child, "'event' name is missing for call!");
          return 0;
        }
        celData ret;
        ent->GetBehaviour ()->SendMessage (msgid, 0, ret, params);
        ent->QueryMessageChannel ()->SendMessage (pl->FetchStringID (msgid),
            pl->QueryMessageSender (), params);
        break;
      }
      case XMLTOKEN_PARAMS:
        // Handled above.
        break;
      default:
        synldr->ReportBadToken (child);
        break;
    }
  }
  celData msgret;
  csRef<celOneParameterBlock> msgparams;
  msgparams.AttachNew (new celOneParameterBlock ());
  msgparams->SetParameterDef (pl->FetchStringID (entityname));
  if (ent->GetBehaviour ())
  {
    ent->GetBehaviour ()->SendMessage ("cel.entity.loaded",
          0, msgret, msgparams);
  }
  ent->QueryMessageChannel ()->SendMessage (
		  pl->FetchStringID ("cel.entity.loaded"),
		  pl->QueryMessageSender (), msgparams);
  return ent;
}

iCelPropertyClass* celAddOnCelEntity::LoadPropclass (iDocumentNode* node, iCelEntity* ent)
{
  iCelPropertyClass* pc;
  const char* name = node->GetAttributeValue ("name");
  if (!name)
  {
    synldr->ReportError (
      "cel.addons.celentity",
      node, "Name of the property class is missing!");
    return 0;
  }
  const char* tag = node->GetAttributeValue ("tag");
  iCelPropertyClassList* plist = ent->GetPropertyClassList ();
  // First check if the pc is already present.
  pc = plist->FindByNameAndTag (name, tag);
  if (!pc)
    pc = pl->CreatePropertyClass (ent, name);
  if (!pc)
  {
    synldr->ReportError (
      "cel.addons.celentity",
      node, "Couldn't create property class '%s'!", name);
    return 0;
  }
  if (!ParseProperties (pc, node))
    return 0;
  if (tag)
    pc->SetTag (tag);
  // check for children propclasses
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    if (id == XMLTOKEN_PROPCLASS)
    {
      LoadPropclass (child, ent);
    }
  }
  return pc;
}
