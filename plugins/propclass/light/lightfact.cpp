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
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "plugins/propclass/light/lightfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "celtool/stdparams.h"
#include "csutil/util.h"
#include "iutil/object.h"
#include "iengine/sector.h"
#include "iengine/light.h"
#include "iengine/movable.h"
#include "iengine/engine.h"
#include "iengine/scenenode.h"
#include "iengine/mesh.h"
#include "ivaria/reporter.h"
#include "propclass/mesh.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Light, "pclight")

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistence",
    	msg, arg);
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

csStringID celPcLight::id_name = csInvalidStringID;
csStringID celPcLight::id_pos = csInvalidStringID;
csStringID celPcLight::id_sector = csInvalidStringID;
csStringID celPcLight::id_radius = csInvalidStringID;
csStringID celPcLight::id_color = csInvalidStringID;
csStringID celPcLight::id_entity = csInvalidStringID;
csStringID celPcLight::id_tag = csInvalidStringID;

PropertyHolder celPcLight::propinfo;

celPcLight::celPcLight (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  created = false;

  if (id_name == csInvalidStringID)
  {
    id_name = pl->FetchStringID ("cel.parameter.name");
    id_pos = pl->FetchStringID ("cel.parameter.pos");
    id_sector = pl->FetchStringID ("cel.parameter.sector");
    id_radius = pl->FetchStringID ("cel.parameter.radius");
    id_color = pl->FetchStringID ("cel.parameter.color");
    id_entity = pl->FetchStringID ("cel.parameter.entity");
    id_tag = pl->FetchStringID ("cel.parameter.tag");
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_setlight, "cel.action.SetLight");
    AddAction (action_movelight, "cel.action.MoveLight");
    AddAction (action_createlight, "cel.action.CreateLight");
    AddAction (action_changecolor, "cel.action.ChangeColor");
    AddAction (action_parentmesh, "cel.action.ParentMesh");
    AddAction (action_clearparent, "cel.action.ClearParent");
  }
}

celPcLight::~celPcLight ()
{
  if (light && created)
  {
    engine->RemoveLight (light);
  }
}

iLight* celPcLight::CreateLight (const char* lightname,
      iSector* sector, const csVector3& pos,
      float radius, const csColor& color)
{
  if (light && created)
  {
    engine->RemoveLight (light);
  }
  created = true;
  light = engine->CreateLight (lightname, pos, radius, color,
      CS_LIGHT_DYNAMICTYPE_DYNAMIC);
  if (sector)
  {
    sector->GetLights ()->Add (light);
    light->Setup ();
  }
  return light;
}

bool celPcLight::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setlight:
      {
        CEL_FETCH_STRING_PAR (name,params,id_name);
        if (!name)
          return Report (object_reg, "'name' parameter missing for the light!");
        SetLight (name);
        return true;
      }
    case action_createlight:
      {
        CEL_FETCH_STRING_PAR (name,params,id_name);
        if (!name)
          return Report (object_reg, "'name' parameter missing for the light!");
	iSector* sectorptr;
        CEL_FETCH_STRING_PAR (sector,params,id_sector);
        if (!sector) sectorptr = 0;
	else
	{
	  sectorptr = engine->FindSector (sector);
          if (!sectorptr)
            return Report (object_reg, "Could not find sector '%s''!", sector);
	}
        CEL_FETCH_VECTOR3_PAR (pos,params,id_pos);
        if (!p_pos)
          return Report (object_reg, "'pos' parameter missing!");
        CEL_FETCH_FLOAT_PAR (radius,params,id_radius);
        if (!p_radius)
          return Report (object_reg, "'radius' parameter missing!");
        CEL_FETCH_COLOR_PAR (color,params,id_color);
        if (!p_color)
          return Report (object_reg, "'color' parameter missing!");
        CreateLight (name, sectorptr, pos, radius, color);
        return true;
      }
    case action_changecolor:
      {
        CEL_FETCH_COLOR_PAR (color,params,id_color);
        if (!p_color)
          return Report (object_reg, "'color' parameter missing!");
	if (light)
	  light->SetColor (color);
        return true;
      }
    case action_movelight:
      {
        CEL_FETCH_VECTOR3_PAR (pos,params,id_pos);
        if (!p_pos)
          return Report (object_reg,
	      "'pos' parameter missing for moving the light!");
        if (light)
          light->SetCenter (pos);
        return true;
      }
    case action_parentmesh:
      {
	if (!light) return true;
        CEL_FETCH_STRING_PAR (par_entity,params,id_entity);
	iCelEntity* ent;
        if (!p_par_entity) ent = entity;
	else
	{
	  ent = pl->FindEntity (par_entity);
	  if (!ent)
	    return Report (object_reg, "Can't find entity '%s'!",
		par_entity);
	}
        CEL_FETCH_STRING_PAR (par_tag,params,id_tag);
	csRef<iPcMesh> parent_mesh;
	if (!p_par_tag)
	  parent_mesh = celQueryPropertyClassEntity<iPcMesh> (ent);
	else
	  parent_mesh = celQueryPropertyClassTag<iPcMesh> (
	      ent->GetPropertyClassList (), par_tag);
	if (!parent_mesh)
	  return Report (object_reg, "Can't find a mesh!");
	light->QuerySceneNode ()->SetParent (parent_mesh->GetMesh ()
	    ->QuerySceneNode ());
	light->GetMovable ()->UpdateMove ();
	light->Setup ();

	return true;
      }
    case action_clearparent:
      {
	if (light)
	{
	  light->QuerySceneNode ()->SetParent (0);
	  light->GetMovable ()->UpdateMove ();
	}
	return true;
      }
    default:
      return false;
  }
}

#define LIGHT_SERIAL 2

csPtr<iCelDataBuffer> celPcLight::Save ()
{
  // @@@ TODO: support with created.
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (LIGHT_SERIAL);
  if (light)
  {
    databuf->Add (light->QueryObject ()->GetName ());
    const csVector3& center = light->GetCenter ();
    databuf->Add (center);
    iSector* sector = light->GetSector ();
    databuf->Add (sector->QueryObject ()->GetName ());
    const csColor& color = light->GetColor ();
    databuf->Add (color);
  }
  else
  {
    databuf->Add ((const char*)0);
  }

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcLight::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != LIGHT_SERIAL)
    return Report (object_reg, "Serialnr != LIGHT_SERIAL.  Cannot load.");

  const char* lightn = databuf->GetString ()->GetData ();
  if (lightn)
  {
    if (!SetLight (lightn))
      return Report (object_reg, "Light '%s' could not be found!", lightn);
    csVector3 center;
    databuf->GetVector3 (center);
    light->SetCenter (center);
    const char* sectorn = databuf->GetString ()->GetData ();
    if (!sectorn)
      return Report (object_reg, "Sector name missing for light '%s'!", lightn);
    iSector* sector = engine->FindSector (sectorn);
    if (!sector)
      return Report (object_reg,
	  "Could not find sector '%s' missing for light '%s'!",
      	  sectorn, lightn);
    light->GetMovable ()->SetSector (sector);
    csColor color;
    databuf->GetColor (color);
    light->SetColor (color);
  }

  return true;
}

bool celPcLight::SetLight (const char* lightname)
{
  if (light && created)
  {
    engine->RemoveLight (light);
  }
  created = false;
  light = engine->FindLight (lightname);
  if (light) return true;
  else return false;
}

void celPcLight::SetLight (iLight* l)
{
  if (light && created)
  {
    engine->RemoveLight (light);
  }
  created = false;
  light = l;
}

//---------------------------------------------------------------------------

