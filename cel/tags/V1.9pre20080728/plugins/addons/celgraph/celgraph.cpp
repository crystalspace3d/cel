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
#include "imap/ldrctxt.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "behaviourlayer/bl.h"
#include "propclass/mesh.h"


#include "celgraph.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celAddOnCelGraph)

enum
{
  XMLTOKEN_NODE,
  XMLTOKEN_EDGE,
  
  XMLTOKEN_FLOAT,
  XMLTOKEN_BOOL,
  XMLTOKEN_STRING,
  XMLTOKEN_LONG,
  XMLTOKEN_VECTOR,
  XMLTOKEN_COLOR
};


celAddOnCelGraph::celAddOnCelGraph (iBase* parent)
  : scfImplementationType (this, parent)
{
  object_reg = 0;
}

celAddOnCelGraph::~celAddOnCelGraph ()
{
}

bool celAddOnCelGraph::Initialize (iObjectRegistry* object_reg)
{
  celAddOnCelGraph::object_reg = object_reg;
  synldr = csQueryRegistry<iSyntaxService> (object_reg);
  if (!synldr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.addons.celgraph",
	"Can't find syntax services!");
    return false;
  }
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.addons.celgraph",
	"Can't find physical layer!");
    return false;
  }

  xmltokens.Register ("node", XMLTOKEN_NODE);
  xmltokens.Register ("edge", XMLTOKEN_EDGE);

  return true;
}

const char* celAddOnCelGraph::GetAttributeValue (iDocumentNode* child,
	const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
	"cel.addons.celgraph", child,
	"Can't find attribute '%s'!", propname);
    return 0;
  }
  return rc;
}

csStringID celAddOnCelGraph::GetAttributeID (iDocumentNode* child,
	const char* prefix, const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
	"cel.addons.celgraph", child,
	"Can't find attribute '%s'!", propname);
    return csInvalidStringID;
  }
  csString p (prefix);
  p += rc;
  return pl->FetchStringID ((const char*)p);
}

csPtr<iBase> celAddOnCelGraph::Parse (iDocumentNode* node,
	iStreamSource*, iLoaderContext* ldr_context, iBase* context)
{
  csRef<iSector> sector = scfQueryInterface<iSector>(context);
  if (!sector)
  {
    synldr->ReportError (
      "cel.addons.celgraph", node,
      "Graph addons must be placed inside sectors!");
    return 0;
  }
  csRef<iCelGraph> graph;
  graph = Load (node);
  csRef<iBase> graph_return = scfQueryInterface<iBase>(graph);
  sector->QueryObject()->ObjAdd(graph->QueryObject());
  return csPtr<iBase> (graph_return);
}

csPtr<iCelGraph> celAddOnCelGraph::Load (const char* path, const char* file)
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
	"cel.addons.celgraph",
	"Can't load file '%s' from '%s'!", file, path);
    else
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"cel.addons.celgraph",
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
	    "cel.addons.celgraph",
	    "Document system error for file '%s': %s!", file, error);
    return 0;
  }
  csRef<iCelGraph> graph = Load (doc->GetRoot ()->GetNode ("addon"));

  if (path)
  {
    vfs->PopDir ();
  }

  return csPtr<iCelGraph>(graph);
}

csPtr<iCelGraph> celAddOnCelGraph::Load (iDocumentNode* node)
{
  const char* graphname = node->GetAttributeValue ("name");
  csRef<iCelGraph> graph = scfCreateInstance<iCelGraph> ("cel.celgraph");

  graph->QueryObject()->SetName(graphname);

  if(!graph)
    fprintf(stderr, "Error Loading CelGraph!\n");

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
    case XMLTOKEN_NODE:
      {
	const char* vec_value = child->GetAttributeValue("position");
	const char* name = child->GetAttributeValue("name");
	
	if (!vec_value)
	  {
	    synldr->ReportError ("cel.addons.celgraph",
				   child, "Incomplete information for Node!");
	    return 0;
	  }
	
	csVector3 v;
	int rc = csScanStr (vec_value, "%f,%f,%f", &v.x, &v.y, &v.z);	
	graph->CreateNode(name,v);
	break;
      }
    case XMLTOKEN_EDGE:
      {	
	const char* from = child->GetAttributeValue("from");
	const char* to = child->GetAttributeValue("to");
	const char* state = child->GetAttributeValue("state");

	if(!from || !to)
	  {
	    synldr->ReportError ("cel.addons.celgraph", 
				 child, "Incomplete information for Edge!");
	    return 0;
	  }
	
	if(state)
	  graph->AddEdgeByNames(from, to, false);
	else
	  graph->AddEdgeByNames(from, to, true);
	break;
      }
    default:
      synldr->ReportBadToken (child);
      break;
    }
  }
  return csPtr<iCelGraph>(graph);
}

