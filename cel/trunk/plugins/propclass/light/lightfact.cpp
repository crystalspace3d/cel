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

CEL_IMPLEMENT_FACTORY_ALT (Light, "pcobject.light", "pclight")

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.pcobject.light",
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
  engine = csQueryRegistry<iEngine> (object_reg);
  if (!engine)
  {
    Report (object_reg, "No iEngine plugin!");
    return;
  }
  created = false;

  if (id_name == csInvalidStringID)
  {
    id_name = pl->FetchStringID ("name");
    id_pos = pl->FetchStringID ("pos");
    id_sector = pl->FetchStringID ("sector");
    id_radius = pl->FetchStringID ("radius");
    id_color = pl->FetchStringID ("color");
    id_entity = pl->FetchStringID ("entity");
    id_tag = pl->FetchStringID ("tag");
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.light.action.");
    AddAction (action_setlight, "SetLight");
    AddAction (action_movelight, "MoveLight");
    AddAction (action_createlight, "CreateLight");
    AddAction (action_changecolor, "ChangeColor");
    AddAction (action_parentmesh, "ParentMesh");
    AddAction (action_clearparent, "ClearParent");
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
    sector->GetLights ()->Add (light);
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
	csString name;
	if (!Fetch (name, params, id_name)) return false;
        SetLight (name);
        return true;
      }
    case action_createlight:
      {
	csString name, sector;
	if (!Fetch (name, params, id_name)) return false;
	if (!Fetch (sector, params, id_sector, true, "")) return false;
	iSector* sectorptr;
        if (sector.IsEmpty ()) sectorptr = 0;
	else
	{
	  sectorptr = engine->FindSector (sector);
          if (!sectorptr)
            return Report (object_reg, "Could not find sector '%s''!",
		sector.GetData ());
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
	if (!light) return true;
	csString sector;
	if (!Fetch (sector, params, id_sector, true, "")) return false;
	if (!sector.IsEmpty ())
	{
	  // Sector is specified. If empty then we clear the sector.
	  if (*sector)
	  {
	    iSector* sectorptr = engine->FindSector (sector);
            if (!sectorptr)
              return Report (object_reg,
		  "Could not find sector '%s''!", sector.GetData ());
	    iSector* current = light->GetMovable ()->GetSectors ()->Get (0);
	    if (current)
	      current->GetLights ()->Remove (light);
	    sectorptr->GetLights ()->Add (light);
	    light->GetMovable ()->SetSector (sectorptr);
	    light->GetMovable ()->UpdateMove ();
	  }
	  else
	  {
	    iSector* current = light->GetMovable ()->GetSectors ()->Get (0);
	    if (current)
	      current->GetLights ()->Remove (light);
	    light->GetMovable ()->ClearSectors ();
	    light->GetMovable ()->UpdateMove ();
	  }
	}
        CEL_FETCH_VECTOR3_PAR (pos,params,id_pos);
        if (!p_pos)
          return Report (object_reg,
	      "'pos' parameter missing for moving the light!");
        light->GetMovable ()->SetPosition (pos);
        return true;
      }
    case action_parentmesh:
      {
	if (!light) return true;
	csString par_entity, par_tag;
	if (!Fetch (par_entity, params, id_entity, true, "")) return false;
	iCelEntity* ent;
        if (par_entity.IsEmpty ()) ent = entity;
	else
	{
	  ent = pl->FindEntity (par_entity);
	  if (!ent)
	    return Report (object_reg, "Can't find entity '%s'!",
		par_entity.GetData ());
	}
	if (!Fetch (par_tag, params, id_tag, true, "")) return false;
	csRef<iPcMesh> parent_mesh;
	if (par_tag.IsEmpty ())
	  parent_mesh = celQueryPropertyClassEntity<iPcMesh> (ent);
	else
	  parent_mesh = celQueryPropertyClassTag<iPcMesh> (
	      ent->GetPropertyClassList (), par_tag);
	if (!parent_mesh)
	  return Report (object_reg, "Can't find a mesh!");
	light->QuerySceneNode ()->SetParent (parent_mesh->GetMesh ()
	    ->QuerySceneNode ());
	light->GetMovable ()->UpdateMove ();

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

