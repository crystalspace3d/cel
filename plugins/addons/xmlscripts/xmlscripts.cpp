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
#include "iutil/objreg.h"
#include "iutil/document.h"
#include "iutil/object.h"
#include "ivaria/reporter.h"
#include "imap/services.h"
#include "iengine/mesh.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/blgen.h"
#include "propclass/mesh.h"

#include "xmlscripts.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celAddOnXmlScripts)

SCF_IMPLEMENT_IBASE (celAddOnXmlScripts)
  SCF_IMPLEMENTS_INTERFACE (iLoaderPlugin)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celAddOnXmlScripts::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

enum
{
  XMLTOKEN_SCRIPT,
  XMLTOKEN_LAYER,
  XMLTOKEN_PCFACTORY
};


celAddOnXmlScripts::celAddOnXmlScripts (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  object_reg = 0;
}

celAddOnXmlScripts::~celAddOnXmlScripts ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celAddOnXmlScripts::Initialize (iObjectRegistry* object_reg)
{
  celAddOnXmlScripts::object_reg = object_reg;
  synldr = CS_QUERY_REGISTRY (object_reg, iSyntaxService);
  if (!synldr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.addons.xmlscripts",
	"Can't find syntax services!");
    return false;
  }

  xmltokens.Register ("script", XMLTOKEN_SCRIPT);
  xmltokens.Register ("layer", XMLTOKEN_LAYER);
  xmltokens.Register ("pcfactory", XMLTOKEN_PCFACTORY);

  return true;
}

void celAddOnXmlScripts::GetBlGen (iDocumentNode* child)
{
  if (!bl)
  {
    bl = CS_QUERY_REGISTRY (object_reg, iCelBlLayer);
    if (!bl)
    {
      synldr->ReportError (
	        "cel.addons.xmlscripts",
	        child, "Can't find a default behaviour layer!");
      return;
    }
  }
  if (!blgen)
  {
    blgen = SCF_QUERY_INTERFACE (bl, iCelBlLayerGenerate);
    if (!blgen)
    {
      synldr->ReportError (
	        "cel.addons.xmlscripts",
	        child, "Behaviour layer doesn't allow generation of scripts!");
    }
  }
}

csPtr<iBase> celAddOnXmlScripts::Parse (iDocumentNode* node,
	iLoaderContext* ldr_context, iBase*)
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
      case XMLTOKEN_PCFACTORY:
        {
	  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg,
	  	iCelPlLayer);
	  if (!pl->LoadPropertyClassFactory (child->GetContentsValue ()))
	  {
	    synldr->ReportError (
	        "cel.addons.xmlscripts",
	        child, "Couldn't load property class factory '%s'!",
		child->GetContentsValue ());
	    return 0;
	  }
	}
	break;
      case XMLTOKEN_SCRIPT:
        {
	  const char* scriptname = child->GetAttributeValue ("name");
	  if (!scriptname)
	  {
	    synldr->ReportError (
	        "cel.addons.xmlscripts",
	        child, "'name' attribute is missing for 'script'!");
	    return 0;
	  }
	  GetBlGen (child);
	  if (!blgen) return 0;
	  bool rc = blgen->CreateBehaviourScriptFromDoc (
	  	scriptname, child);
	  if (!rc)
	  {
	    // Already reported.
	    return 0;
	  }
	}
        break;
      case XMLTOKEN_LAYER:
        {
	  bl = CS_QUERY_REGISTRY_TAG_INTERFACE (object_reg,
	  	child->GetContentsValue (), iCelBlLayer);
	  if (!bl)
	  {
	    synldr->ReportError (
	        "cel.addons.xmlscripts",
	        child, "Can't find behaviour layer with id '%s'!",
		child->GetContentsValue ());
	    return 0;
	  }
	  GetBlGen (child);
	  if (!blgen) return 0;
	}
        break;
      default:
        synldr->ReportBadToken (child);
	return 0;
    }
  }
  // Just return something to indicate success.
  IncRef ();
  return csPtr<iBase> (this);
}

