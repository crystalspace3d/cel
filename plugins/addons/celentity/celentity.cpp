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
#include "propclass/mesh.h"

#include "celentity.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celAddOnCelEntity)

SCF_IMPLEMENT_IBASE (celAddOnCelEntity)
  SCF_IMPLEMENTS_INTERFACE (iLoaderPlugin)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celAddOnCelEntity::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

enum
{
  XMLTOKEN_BEHAVIOUR,
  XMLTOKEN_PROPCLASS
};


celAddOnCelEntity::celAddOnCelEntity (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  object_reg = 0;
}

celAddOnCelEntity::~celAddOnCelEntity ()
{
}

bool celAddOnCelEntity::Initialize (iObjectRegistry* object_reg)
{
  celAddOnCelEntity::object_reg = object_reg;
  synldr = CS_QUERY_REGISTRY (object_reg, iSyntaxService);
  if (!synldr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.addons.celentity",
	"Can't find syntax services!");
    return false;
  }
  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.addons.celentity",
	"Can't find physical layer!");
    return false;
  }

  xmltokens.Register ("behaviour", XMLTOKEN_BEHAVIOUR);
  xmltokens.Register ("propclass", XMLTOKEN_PROPCLASS);

  return true;
}

csPtr<iBase> celAddOnCelEntity::Parse (iDocumentNode* node,
	iLoaderContext* ldr_context, iBase* context)
{
  // If the context is not a mesh we will create a standalone entity.
  csRef<iMeshWrapper> mesh = SCF_QUERY_INTERFACE (context, iMeshWrapper);

  csRef<iCelEntity> ent = pl->CreateEntity ();
  const char* entityname = node->GetAttributeValue ("entityname");
  if (entityname)
  {
    ent->SetName (entityname);
  }
  else if (mesh && mesh->QueryObject()->GetName ())
  {
    ent->SetName (mesh->QueryObject ()->GetName ());
  }

  iCelPropertyClass* pc;
  if (mesh)
  {
    // If we have a mesh we also create a pcmesh property class.
    pc = pl->CreatePropertyClass (ent, "pcmesh");
    csRef<iPcMesh> pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);
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
	    bl = CS_QUERY_REGISTRY_TAG_INTERFACE (object_reg, blname,
	    	iCelBlLayer);
	  else
	    bl = CS_QUERY_REGISTRY (object_reg, iCelBlLayer);
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
	  ent->SetBehaviour (behave);
	}
        break;
      case XMLTOKEN_PROPCLASS:
        {
	  iCelPropertyClass* pc;
	  const char* name = child->GetAttributeValue ("name");
	  if (!name)
	  {
	    synldr->ReportError (
	        "cel.addons.celentity",
	        child, "Name of the property class is missing!");
	    return 0;
	  }
	  pc = pl->CreatePropertyClass (ent, name);
	  if (!pc)
	  {
	    synldr->ReportError (
	        "cel.addons.celentity",
	        child, "Couldn't create property class '%s'!", name);
	    return 0;
	  }
	}
        break;
    }
  }
  csRef<iBase> ent_return = (iBase*)ent;
  return csPtr<iBase> (ent_return);
}

