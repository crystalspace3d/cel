/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein

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
#include "csgeom/math3d.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "csutil/debug.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "plugins/propclass/damage/damagefact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Damage, "pcdamage")

//---------------------------------------------------------------------------

csStringID celPcDamage::id_amount = csInvalidStringID;
csStringID celPcDamage::id_source = csInvalidStringID;
csStringID celPcDamage::id_sector = csInvalidStringID;
csStringID celPcDamage::id_position = csInvalidStringID;
csStringID celPcDamage::id_type = csInvalidStringID;
csStringID celPcDamage::action_areadamage = csInvalidStringID;
csStringID celPcDamage::id_radius = csInvalidStringID;
csStringID celPcDamage::action_beamdamage = csInvalidStringID;
csStringID celPcDamage::id_direction = csInvalidStringID;
csStringID celPcDamage::id_maxdist = csInvalidStringID;
csStringID celPcDamage::action_singledamage = csInvalidStringID;
csStringID celPcDamage::id_target = csInvalidStringID;

celPcDamage::celPcDamage (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  amount = 1.0;
  falloff = FALLOFF_NORMAL;
  position.Set (0, 0, 0);

  if (id_amount == csInvalidStringID)
  {
    id_amount = pl->FetchStringID ("cel.parameter.amount");
    id_source = pl->FetchStringID ("cel.parameter.source");
    id_sector = pl->FetchStringID ("cel.parameter.sector");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_type = pl->FetchStringID ("cel.parameter.type");
    action_areadamage = pl->FetchStringID ("cel.action.AreaDamage");
    id_radius = pl->FetchStringID ("cel.parameter.radius");
    action_beamdamage = pl->FetchStringID ("cel.action.BeamDamage");
    id_direction = pl->FetchStringID ("cel.parameter.direction");
    id_maxdist = pl->FetchStringID ("cel.parameter.maxdist");
    action_singledamage = pl->FetchStringID ("cel.action.SingleDamage");
    id_target = pl->FetchStringID ("cel.parameter.target");
  }

  params = new celVariableParameterBlock ();
  params->SetParameterDef (0, id_amount, "amount");
  params->SetParameterDef (1, id_source, "source");
  params->SetParameterDef (2, id_sector, "sector");
  params->SetParameterDef (3, id_position, "position");
  params->SetParameterDef (4, id_type, "type");

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_amount] = &amount;
  propdata[propid_type] = 0;
  propdata[propid_sector] = 0;
  propdata[propid_position] = &position;

  engine = csQueryRegistry<iEngine> (object_reg);
}

celPcDamage::~celPcDamage ()
{
  delete params;
}

Property* celPcDamage::properties = 0;
size_t celPcDamage::propertycount = 0;

void celPcDamage::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    propertycount = 4;
    properties = new Property[propertycount];

    properties[propid_amount].id = pl->FetchStringID (
    	"cel.property.amount");
    properties[propid_amount].datatype = CEL_DATA_FLOAT;
    properties[propid_amount].readonly = false;
    properties[propid_amount].desc = "Amount of damage.";

    properties[propid_type].id = pl->FetchStringID (
    	"cel.property.type");
    properties[propid_type].datatype = CEL_DATA_STRING;
    properties[propid_type].readonly = false;
    properties[propid_type].desc = "Type of damage.";

    properties[propid_sector].id = pl->FetchStringID (
    	"cel.property.sector");
    properties[propid_sector].datatype = CEL_DATA_STRING;
    properties[propid_sector].readonly = false;
    properties[propid_sector].desc = "Originating sector.";

    properties[propid_position].id = pl->FetchStringID (
    	"cel.property.position");
    properties[propid_position].datatype = CEL_DATA_VECTOR3;
    properties[propid_position].readonly = false;
    properties[propid_position].desc = "Originating position.";
  }
}

bool celPcDamage::SetProperty (csStringID propertyId, const char* b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_type].id)
  {
    type = b;
    return true;
  }
  else if (propertyId == properties[propid_sector].id)
  {
    sector = b;
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

const char* celPcDamage::GetPropertyString (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_type].id)
  {
    return type;
  }
  else if (propertyId == properties[propid_sector].id)
  {
    return sector;
  }
  else
  {
    return celPcCommon::GetPropertyString (propertyId);
  }
}

#define DAMAGE_SERIAL 2

csPtr<iCelDataBuffer> celPcDamage::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (DAMAGE_SERIAL);
  // @@@ TODO
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcDamage::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != DAMAGE_SERIAL) return false;

  // @@@ TODO

  return true;
}

bool celPcDamage::PerformAction (csStringID actionId,
	iCelParameterBlock* params,
	celData& ret)
{
  if (actionId == action_areadamage)
  {
    CEL_FETCH_FLOAT_PAR (radius,params,id_radius);
    if (!p_radius) radius = 1000000000.0f;
    AreaDamage (radius);
    return true;
  }
  else if (actionId == action_beamdamage)
  {
    CEL_FETCH_FLOAT_PAR (maxdist,params,id_maxdist);
    if (!p_maxdist) { maxdist = 1000000000.0f; }
    CEL_FETCH_VECTOR3_PAR (direction,params,id_direction);
    if (!p_direction) return false; // @@@ Error!
    BeamDamage (direction, maxdist);
    return true;
  }
  else if (actionId == action_singledamage)
  {
    CEL_FETCH_STRING_PAR (target,params,id_target);
    if (!p_target) return false;	// @@@ Error!
    SingleDamage (target);
    return true;
  }
  return false;
}

void celPcDamage::SetDamageLocation (const char* sectorname,
      const csVector3& pos)
{
  sector = sectorname;
  sector_ref = 0;
  position = pos;
}

void celPcDamage::GetLocation (iSector*& s, csVector3& p)
{
  if (sector_ref)
  {
    s = sector_ref;
    p = position;
    return;
  }
  else if (!sector.IsEmpty ())
  {
    sector_ref = engine->FindSector (sector);
    s = sector_ref;
    p = position;
    return;
  }
  if (!pcmesh) pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  if (!pcmesh)
  {
    s = 0;
    return;
  }
  iMovable* mov = pcmesh->GetMesh ()->GetMovable ();
  if (mov->GetSectors ()->GetCount () <= 0)
  {
    s = 0;
    return;
  }
  s = mov->GetSectors ()->Get (0);
  p = mov->GetFullPosition ();
}

void celPcDamage::SetFallOff (const char* f)
{
  if (!strcmp ("linear", f))
    falloff = FALLOFF_LINEAR;
  else if (!strcmp ("constant", f))
    falloff = FALLOFF_CONSTANT;
  else
    falloff = FALLOFF_NORMAL;
}

const char* celPcDamage::GetFallOff () const
{
  switch (falloff)
  {
    case FALLOFF_LINEAR: return "linear";
    case FALLOFF_CONSTANT: return "constant";
    default: return "normal";
  }
}

void celPcDamage::DoDamage (iCelEntity* ent, const csVector3& p)
{
  iCelBehaviour* behave = ent->GetBehaviour ();
  if (!behave) return;
  if (entity == ent)
    return;	// Ignore source of explosion.

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (ent, iPcMesh);
  if (!pcmesh) return;
  float new_amount;
  switch (falloff)
  {
    case FALLOFF_CONSTANT:
      new_amount = amount;
      break;
    case FALLOFF_LINEAR:
      {
        csVector3 pos = pcmesh->GetMesh ()->GetMovable ()
		->GetFullPosition ();
        float sqdist = csSquaredDist::PointPoint (pos, p);
        if (sqdist < 1) sqdist = 1;
	new_amount = float (amount) / sqrt (sqdist);
      }
      break;
    case FALLOFF_NORMAL:
      {
        csVector3 pos = pcmesh->GetMesh ()->GetMovable ()
		->GetFullPosition ();
        float sqdist = csSquaredDist::PointPoint (pos, p);
        if (sqdist < 1) sqdist = 1;
	new_amount = float (amount) / sqdist;
      }
      break;
  }
  params->GetParameter (0).Set (new_amount);
  celData ret;
  behave->SendMessage ("pcdamage_hurt", 0, ret, params);
}

void celPcDamage::DoDamage (iCelEntityList* list, const csVector3& p)
{
  size_t i;
  for (i = 0 ; i < list->GetCount () ; i++)
  {
    iCelEntity* ent = list->Get (i);
    DoDamage (ent, p);
  }
}

void celPcDamage::AreaDamage (float radius)
{
  // @@@ Should be more configurable!!!
#define MIN_FORCE 0.5f
  float max_radius;
  switch (falloff)
  {
    case FALLOFF_CONSTANT:
    case FALLOFF_LINEAR:
      max_radius = radius;
      break;
    case FALLOFF_NORMAL:
      max_radius = sqrt (float (amount) / MIN_FORCE);
      break;
  }
  iSector* s;
  csVector3 p;
  GetLocation (s, p);
  if (s == 0) return;	// Can't do area damage.

  params->GetParameter (1).Set (entity->GetName ());
  params->GetParameter (2).Set (s->QueryObject ()->GetName ());
  params->GetParameter (3).Set (p);
  params->GetParameter (4).Set (type);

  // @@@ Make 'true' for do_invisible configurable?
  csRef<iCelEntityList> list = pl->FindNearbyEntities (s, p,
  	max_radius, true);
  DoDamage (list, p);
}

void celPcDamage::BeamDamage (const csVector3& direction, float maxdist)
{
  // @@@ Should be more configurable!!!
  float max_radius;
  switch (falloff)
  {
    case FALLOFF_CONSTANT:
    case FALLOFF_LINEAR:
      max_radius = maxdist;
      break;
    case FALLOFF_NORMAL:
      max_radius = sqrt (float (amount) / MIN_FORCE);
      break;
  }
  iSector* s;
  csVector3 p;
  GetLocation (s, p);
  if (s == 0) return;	// Can't do beam damage.

  params->GetParameter (1).Set (entity->GetName ());
  params->GetParameter (2).Set (s->QueryObject ()->GetName ());
  params->GetParameter (3).Set (p);
  params->GetParameter (4).Set (type);

  // @@@ Make 'true' for do_invisible configurable?
  csVector3 end = p + max_radius * direction.Unit ();
  csRef<iCelEntityList> list = pl->FindNearbyEntities (s, p,
  	end, true);
  DoDamage (list, p);
}

void celPcDamage::SingleDamage (const char* target)
{
  iCelEntity* ent = pl->FindEntity (target);
  if (!ent) return;

  float max_radius;
  switch (falloff)
  {
    case FALLOFF_CONSTANT:
      max_radius = 1000000000.0f;
      break;
    case FALLOFF_LINEAR:
      max_radius = amount;
      break;
    case FALLOFF_NORMAL:
      max_radius = sqrt (float (amount) / MIN_FORCE);
      break;
  }
  iSector* s;
  csVector3 p;
  GetLocation (s, p);

  params->GetParameter (1).Set (entity->GetName ());
  params->GetParameter (2).Set (s == 0 ? "" : (s->QueryObject ()->GetName ()));
  params->GetParameter (3).Set (p);
  params->GetParameter (4).Set (type);
  DoDamage (ent, p);
}

//---------------------------------------------------------------------------

