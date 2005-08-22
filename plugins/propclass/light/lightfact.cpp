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
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Light, "pclight")

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
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
}

//---------------------------------------------------------------------------

csStringID celPcLight::action_setlight = csInvalidStringID;
csStringID celPcLight::id_name = csInvalidStringID;

celPcLight::celPcLight (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);

  if (action_setlight == csInvalidStringID)
  {
    action_setlight = pl->FetchStringID ("cel.action.SetLight");
    id_name = pl->FetchStringID ("cel.parameter.name");
  }
}

celPcLight::~celPcLight ()
{
}

bool celPcLight::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_setlight)
  {
    CEL_FETCH_STRING_PAR (name,params,id_name);
    if (!name) return false;
    SetLight (name);
    return true;
  }
  return false;
}

#define LIGHT_SERIAL 2

csPtr<iCelDataBuffer> celPcLight::Save ()
{
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
  {
    Report (object_reg, "Serialnr != LIGHT_SERIAL.  Cannot load.");
    return false;
  }

  const char* lightn = databuf->GetString ()->GetData ();
  if (lightn)
  {
    if (!SetLight (lightn))
    {
      Report (object_reg, "Light '%s' could not be found!", lightn);
      return false;
    }
    csVector3 center;
    databuf->GetVector3 (center);
    light->SetCenter (center);
    const char* sectorn = databuf->GetString ()->GetData ();
    if (!sectorn)
    {
      Report (object_reg, "Sector name missing for light '%s'!", lightn);
      return false;
    }
    iSector* sector = engine->FindSector (sectorn);
    if (!sector)
    {
      Report (object_reg, "Could not find sector '%s' missing for light '%s'!",
      	sectorn, lightn);
      return false;
    }
    light->GetMovable ()->SetSector (sector);
    csColor color;
    databuf->GetColor (color);
    light->SetColor (color);
  }

  return true;
}

bool celPcLight::SetLight (const char* lightname)
{
  light = engine->FindLight (lightname);
  if (light) return true;
  else return false;
}

void celPcLight::SetLight (iLight* l)
{
  light = l;
}

//---------------------------------------------------------------------------

