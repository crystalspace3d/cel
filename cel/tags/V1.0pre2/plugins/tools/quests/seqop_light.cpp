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
#include "ivaria/reporter.h"
#include "iengine/movable.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/persist.h"

#include "plugins/tools/quests/seqop_light.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_SEQOPTYPE(Light)

//---------------------------------------------------------------------------

celLightSeqOpFactory::celLightSeqOpFactory (
	celLightSeqOpType* type) : scfImplementationType (this)
{
  celLightSeqOpFactory::type = type;
}

celLightSeqOpFactory::~celLightSeqOpFactory ()
{
}

csPtr<iQuestSeqOp> celLightSeqOpFactory::CreateSeqOp (
    const csHash<csStrKey,csStrKey>& params)
{
  celLightSeqOp* seqop = new celLightSeqOp (type,
  	params, entity_par, tag_par,
	rel_red_par, rel_green_par, rel_blue_par,
	abs_red_par, abs_green_par, abs_blue_par);
  return seqop;
}

bool celLightSeqOpFactory::Load (iDocumentNode* node)
{
  rel_red_par.Empty ();
  rel_green_par.Empty ();
  rel_blue_par.Empty ();
  abs_red_par.Empty ();
  abs_green_par.Empty ();
  abs_blue_par.Empty ();

  entity_par = node->GetAttributeValue ("entity");
  if (entity_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.transform",
      "'entity' attribute is missing for the transform seqop!");
    return false;
  }
  tag_par = node->GetAttributeValue ("entity_tag");

  csRef<iDocumentNode> relcolor_node = node->GetNode ("relcolor");
  if (relcolor_node)
  {
    rel_red_par = relcolor_node->GetAttributeValue ("red");
    rel_green_par = relcolor_node->GetAttributeValue ("green");
    rel_blue_par = relcolor_node->GetAttributeValue ("blue");
  }
  csRef<iDocumentNode> abscolor_node = node->GetNode ("abscolor");
  if (abscolor_node)
  {
    abs_red_par = abscolor_node->GetAttributeValue ("red");
    abs_green_par = abscolor_node->GetAttributeValue ("green");
    abs_blue_par = abscolor_node->GetAttributeValue ("blue");
  }
  
  return true;
}

void celLightSeqOpFactory::SetEntityParameter (const char* entity,
	const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celLightSeqOpFactory::SetRelColorParameter (const char* red,
	const char* green, const char* blue)
{
  rel_red_par = red;
  rel_green_par = green;
  rel_blue_par = blue;
}

void celLightSeqOpFactory::SetAbsColorParameter (const char* red,
	const char* green, const char* blue)
{
  abs_red_par = red;
  abs_green_par = green;
  abs_blue_par = blue;
}

//---------------------------------------------------------------------------

static float ToFloat (const char* s)
{
  if (!s) return 0.0f;
  float f = 0.0f;
  sscanf (s, "%f", &f);
  return f;
}

celLightSeqOp::celLightSeqOp (
	celLightSeqOpType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par, const char* tag_par,
	const char* rel_red_par, const char* rel_green_par,
		const char* rel_blue_par,
	const char* abs_red_par, const char* abs_green_par,
		const char* abs_blue_par) : scfImplementationType (this)
{
  celLightSeqOp::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = qm->ResolveParameter (params, entity_par);
  tag = qm->ResolveParameter (params, tag_par);
  rel.red = ToFloat (qm->ResolveParameter (params, rel_red_par));
  rel.green = ToFloat (qm->ResolveParameter (params, rel_green_par));
  rel.blue = ToFloat (qm->ResolveParameter (params, rel_blue_par));
  abs.red = ToFloat (qm->ResolveParameter (params, abs_red_par));
  abs.green = ToFloat (qm->ResolveParameter (params, abs_green_par));
  abs.blue = ToFloat (qm->ResolveParameter (params, abs_blue_par));

  do_abs = abs_red_par != 0 && *abs_red_par != 0;
  do_rel = rel_red_par != 0 && *rel_red_par != 0;
}

celLightSeqOp::~celLightSeqOp ()
{
}

void celLightSeqOp::FindLight ()
{
  if (light) return;

  // @@@ To many queries for efficiency?
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent = pl->FindEntity (entity);
  if (ent)
  {
    csRef<iPcLight> pclight = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcLight, tag);
    if (pclight)
    {
      light = pclight->GetLight ();
      start = light->GetColor ();
    }
  }
}

bool celLightSeqOp::Load (iCelDataBuffer* databuf)
{
  light = 0;
  databuf->GetColor (start);
  return true;
}

void celLightSeqOp::Save (iCelDataBuffer* databuf)
{
  databuf->Add (start);
}

void celLightSeqOp::Init ()
{
  light = 0;
  FindLight ();
}

void celLightSeqOp::Do (float time)
{
  if (light)
  {
    csColor col;
    if (do_abs) col = abs;
    else col = start;
    if (do_rel)
      col += time * rel;
    light->SetColor (col);
  }
}

//---------------------------------------------------------------------------

