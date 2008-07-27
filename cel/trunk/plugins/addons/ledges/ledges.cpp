/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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
#include "cstool/initapp.h"
#include "iutil/objreg.h"
#include "iutil/document.h"
#include "iutil/object.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"
#include "imap/services.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "behaviourlayer/bl.h"
#include "propclass/mesh.h"
#include "celtool/stdparams.h"

#include "ledges.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celAddOnLedges)

celAddOnLedges::celAddOnLedges (iBase* parent) :
  scfImplementationType (this, parent)
{
  object_reg = 0;
}

celAddOnLedges::~celAddOnLedges ()
{
}

bool celAddOnLedges::Initialize (iObjectRegistry* object_reg)
{
  celAddOnLedges::object_reg = object_reg;
  synldr = csQueryRegistry<iSyntaxService> (object_reg);
  if (!synldr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.addons.ledges",
      "Can't find syntax services!");
    return false;
  }
  return true;
}

csPtr<iBase> celAddOnLedges::Parse (iDocumentNode* node, iStreamSource*, iLoaderContext* ldr_context, iBase* context)
{
  csRef<iSector> sector = scfQueryInterface<iSector>(context);
  if (!sector)
  {
    synldr->ReportError (
    "cel.addons.celgraph", node,
    "Ledges addon must be placed inside sectors!");
    return 0;
  }
  csRef<iLedgeGroup> ledges;
  ledges = Load (node);
  csRef<iBase> ledges_return = scfQueryInterface<iBase> (ledges);
  sector->QueryObject()->ObjAdd (ledges->QueryObject());
  return csPtr<iBase> (ledges_return);
}

csPtr<iLedgeGroup> celAddOnLedges::Load (iDocumentNode* node)
{
  csRef<iLedgeGroup> ledges = scfCreateInstance<iLedgeGroup> ("cel.ledgegroup");
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (!strcmp (value, "ledge"))
    {
      LoadLedge (child, ledges);
    }
  }
  return csPtr<iLedgeGroup> (ledges);
}

void celAddOnLedges::LoadLedge (iDocumentNode* node, iLedgeGroup* ledges)
{
  iLedge* ledge = ledges->CreateLedge ();
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (!strcmp (value, "p"))
    {
      csVector3 point;
      point.x = child->GetAttributeValueAsFloat ("x");
      point.y = child->GetAttributeValueAsFloat ("y");
      point.z = child->GetAttributeValueAsFloat ("z");
      puts (point.Description ().GetData ());
      ledge->AddPoint (point);
    }
  }
}
