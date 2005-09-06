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
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/object.h"
#include "ivaria/reporter.h"
#include "iengine/movable.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/persist.h"
#include "propclass/camera.h"

#include "plugins/tools/quests/seqop_movepath.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_SEQOPTYPE(MovePath)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.quests.seqop.movepath", msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celMovePathSeqOpFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestSeqOpFactory)
  SCF_IMPLEMENTS_INTERFACE (iMovePathQuestSeqOpFactory)
SCF_IMPLEMENT_IBASE_END

celMovePathSeqOpFactory::celMovePathSeqOpFactory (
	celMovePathSeqOpType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celMovePathSeqOpFactory::type = type;
}

celMovePathSeqOpFactory::~celMovePathSeqOpFactory ()
{
  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestSeqOp> celMovePathSeqOpFactory::CreateSeqOp (
    const csHash<csStrKey,csStrKey>& params)
{
  celMovePathSeqOp* seqop = new celMovePathSeqOp (type,
  	params, entity_par, tag_par, sectors, nodes, times);
  return seqop;
}

bool celMovePathSeqOpFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  if (entity_par.IsEmpty ())
    return Report (type->object_reg,
      "'entity' attribute is missing for the movepath seqop!");
  tag_par = node->GetAttributeValue ("entity_tag");

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (!strcmp ("pathnode", value))
    {
      AddPathNode (
      	child->GetAttributeValue ("sector"),
      	child->GetAttributeValue ("node"),
      	child->GetAttributeValue ("time"));
    }
  }

  return true;
}

void celMovePathSeqOpFactory::SetEntityParameter (const char* entity,
	const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celMovePathSeqOpFactory::AddPathNode (const char* sector,
	const char* node, const char* time)
{
  sectors.Push (sector);
  nodes.Push (node);
  times.Push (time);
}

//---------------------------------------------------------------------------

static float ToFloat (const char* s)
{
  if (!s) return 0.0f;
  float f;
  sscanf (s, "%f", &f);
  return f;
}

SCF_IMPLEMENT_IBASE (celMovePathSeqOp)
  SCF_IMPLEMENTS_INTERFACE (iQuestSeqOp)
SCF_IMPLEMENT_IBASE_END

celMovePathSeqOp::celMovePathSeqOp (
	celMovePathSeqOpType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par, const char* tag_par,
	const csStringArray& sectors, const csStringArray& nodes,
	const csStringArray& times)
{
  SCF_CONSTRUCT_IBASE (0);
  celMovePathSeqOp::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = qm->ResolveParameter (params, entity_par);
  tag = qm->ResolveParameter (params, tag_par);

  csString sectorname;
  path = new csPath ((int)nodes.Length ());

  sector = 0;
  maxtime = 0.0f;
  size_t i;
  for (i = 0 ; i < nodes.Length () ; i++)
  {
    float t = ToFloat (qm->ResolveParameter (params, times[i]));
    path->SetTime ((int)i, t);
    if (t >= maxtime) maxtime = t;

    if (!sector)
    {
      csRef<iEngine> engine = CS_QUERY_REGISTRY (type->object_reg, iEngine);
      sectorname = qm->ResolveParameter (params, sectors[i]);
      sector = engine->FindSector (sectorname);
      if (!sector)
      {
        delete path;
	path = 0;
	Report (type->object_reg, "Can't find sector '%s' in movepath seqop!",
		(const char*)sectorname);
	return;
      }
    }

    csRef<iMapNode> mapnode = CS_GET_NAMED_CHILD_OBJECT (
  	sector->QueryObject (), iMapNode, nodes[i]);
    if (!mapnode)
    {
      delete path;
      path = 0;
      Report (type->object_reg,
      	"Can't find node '%s' in sector '%s' (movepath seqop)!",
	(const char*)nodes[i], (const char*)sectorname);
      return;
    }

    const csVector3& pos = mapnode->GetPosition ();
    path->SetPositionVector ((int)i, pos);
    path->SetForwardVector ((int)i, mapnode->GetZVector ());
    path->SetUpVector ((int)i, mapnode->GetYVector ());
  }
}

celMovePathSeqOp::~celMovePathSeqOp ()
{
  delete path;
  SCF_DESTRUCT_IBASE ();
}

void celMovePathSeqOp::FindMesh ()
{
  if (mesh) return;

  // @@@ Too many queries for efficiency?
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  iCelEntity* ent = pl->FindEntity (entity);
  if (ent)
  {
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcMesh, tag);
    if (pcmesh)
      mesh = pcmesh->GetMesh ();
  }
}

bool celMovePathSeqOp::Load (iCelDataBuffer* databuf)
{
  mesh = 0;
  return true;
}

void celMovePathSeqOp::Save (iCelDataBuffer* databuf)
{
}

void celMovePathSeqOp::Init ()
{
  mesh = 0;
  if (path) FindMesh ();
}

void celMovePathSeqOp::Do (float time)
{
  if (mesh)
  {
    path->CalculateAtTime (time * maxtime);
    csVector3 pos, up, forward;
    path->GetInterpolatedPosition (pos);
    path->GetInterpolatedUp (up);
    path->GetInterpolatedForward (forward);
    mesh->GetMovable ()->GetTransform ().SetOrigin (pos);
    mesh->GetMovable ()->GetTransform ().LookAt (forward, up);
    mesh->GetMovable ()->UpdateMove ();
  }
}

//---------------------------------------------------------------------------

