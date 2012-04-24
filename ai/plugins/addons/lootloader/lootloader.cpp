/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein

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
#include "iutil/plugin.h"
#include "iutil/vfs.h"
#include "ivaria/reporter.h"
#include "imap/services.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "tools/parameters.h"

#include "lootloader.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celAddOnLootLoader)

enum
{
  XMLTOKEN_PACKAGE,
  XMLTOKEN_SELECTOR,

  XMLTOKEN_RANDOMGEN,
  XMLTOKEN_ITEM,

  XMLTOKEN_GENERATOR,
};


celAddOnLootLoader::celAddOnLootLoader (iBase* parent) :
  scfImplementationType (this, parent)
{
  object_reg = 0;
}

celAddOnLootLoader::~celAddOnLootLoader ()
{
}

bool celAddOnLootLoader::Initialize (iObjectRegistry* object_reg)
{
  celAddOnLootLoader::object_reg = object_reg;
  synldr = csQueryRegistry<iSyntaxService> (object_reg);
  if (!synldr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cel.addons.lootloader",
        "Can't find syntax services!");
    return false;
  }
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cel.addons.lootloader",
        "Can't find physical layer!");
    return false;
  }
  lootmgr = csLoadPluginCheck<iLootManager> (object_reg, "cel.tools.lootmanager");
  if (!lootmgr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cel.addons.lootloader",
        "Can't find loot manager!");
    return false;
  }
  pm = csLoadPluginCheck<iParameterManager> (object_reg, "cel.parameters.manager");
  if (!pm)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cel.addons.lootloader",
        "Can't find parameter manager!");
    return false;
  }

  xmltokens.Register ("package", XMLTOKEN_PACKAGE);
  xmltokens.Register ("selector", XMLTOKEN_SELECTOR);
  xmltokens.Register ("randomgen", XMLTOKEN_RANDOMGEN);
  xmltokens.Register ("item", XMLTOKEN_ITEM);
  xmltokens.Register ("generator", XMLTOKEN_GENERATOR);

  return true;
}

const char* celAddOnLootLoader::GetAttributeValue (iDocumentNode* child,
        const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
        "cel.addons.lootloader", child,
        "Can't find attribute '%s'!", propname);
    return 0;
  }
  return rc;
}

bool celAddOnLootLoader::ParsePackage (iDocumentNode* node)
{
  csString name = node->GetAttributeValue ("name");
  csRef<iLootPackage> package = lootmgr->CreateLootPackage (name);
  if (node->GetAttribute ("minloot"))
    package->SetMinLoot (node->GetAttributeValueAsInt ("minloot"));
  if (node->GetAttribute ("maxloot"))
    package->SetMaxLoot (node->GetAttributeValueAsInt ("maxloot"));


  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_RANDOMGEN:
	package->SetRandomGeneration (true);
	break;
      case XMLTOKEN_ITEM:
	{
	  csString templateName = child->GetAttributeValue ("template");
	  float chance = child->GetAttributeValueAsFloat ("chance");
	  int minamount = child->GetAttributeValueAsFloat ("minamount");
	  int maxamount = child->GetAttributeValueAsFloat ("maxamount");
	  csRef<iParameter> validate;
	  if (child->GetAttribute ("validate"))
	  {
	    csString validateStr = child->GetAttributeValue ("validate");
	    validate = pm->GetParameter (validateStr, CEL_DATA_BOOL);
	  }
	  package->AddLootItem (templateName, chance, minamount, maxamount, validate);
	}
	break;
      default:
        synldr->ReportBadToken (child);
        break;
    }
  }
  return true;
}

bool celAddOnLootLoader::ParseSelector (iDocumentNode* node)
{
  csString name = node->GetAttributeValue ("name");
  csRef<iLootSelector> selector = lootmgr->CreateLootSelector (name);

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_GENERATOR:
	{
	  csString name = child->GetAttributeValue ("name");
	  float chance = child->GetAttributeValueAsFloat ("chance");
	  csRef<iParameter> validate;
	  if (child->GetAttribute ("validate"))
	  {
	    csString validateStr = child->GetAttributeValue ("validate");
	    validate = pm->GetParameter (validateStr, CEL_DATA_BOOL);
	  }
	  iLootGenerator* gen = lootmgr->FindLootGenerator (name);
	  if (!gen)
	  {
	    synldr->ReportError ("cel.addons.lootloader", child,
		"Can't find loot generator '%s'!", name.GetData ());
	    return false;
	  }
	  selector->AddLootGenerator (gen, chance, validate);
	}
	break;
      default:
        synldr->ReportBadToken (child);
        break;
    }
  }
  return true;
}

csPtr<iBase> celAddOnLootLoader::Parse (iDocumentNode* node,
        iStreamSource*, iLoaderContext* ldr_context, iBase* context)
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
      case XMLTOKEN_PACKAGE:
	if (!ParsePackage (child)) return 0;
	break;
      case XMLTOKEN_SELECTOR:
	if (!ParseSelector (child)) return 0;
	break;
      default:
        synldr->ReportBadToken (child);
        break;
    }
  }


  IncRef ();
  return (iLoaderPlugin*)this;
}

