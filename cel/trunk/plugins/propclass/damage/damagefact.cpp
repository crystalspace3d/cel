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
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "plugins/propclass/damage/damagefact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY_ALT (Damage, "pclogic.damage", "pcdamage")

//---------------------------------------------------------------------------

csStringID celPcDamage::id_amount = csInvalidStringID;
csStringID celPcDamage::id_source = csInvalidStringID;
csStringID celPcDamage::id_sector = csInvalidStringID;
csStringID celPcDamage::id_position = csInvalidStringID;
csStringID celPcDamage::id_type = csInvalidStringID;
csStringID celPcDamage::id_radius = csInvalidStringID;
csStringID celPcDamage::id_direction = csInvalidStringID;
csStringID celPcDamage::id_maxdist = csInvalidStringID;
csStringID celPcDamage::id_target = csInvalidStringID;

PropertyHolder celPcDamage::propinfo;

celPcDamage::celPcDamage (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  amount = 1.0;
  falloff = FALLOFF_NORMAL;
  position.Set (0, 0, 0);
  sourceset = false;
  if (id_amount == csInvalidStringID)
  {
    id_amount = pl->FetchStringID ("amount");
    id_source = pl->FetchStringID ("source");
    id_sector = pl->FetchStringID ("sector");
    id_position = pl->FetchStringID ("position");
    id_type = pl->FetchStringID ("type");
    id_radius = pl->FetchStringID ("radius");
    id_direction = pl->FetchStringID ("direction");
    id_maxdist = pl->FetchStringID ("maxdist");
    id_target = pl->FetchStringID ("target");
  }

  params = new celVariableParameterBlock ();
  params->SetParameterDef (0, id_amount);
  params->SetParameterDef (1, id_source);
  params->SetParameterDef (2, id_sector);
  params->SetParameterDef (3, id_position);
  params->SetParameterDef (4, id_type);

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_areadamage, "AreaDamage");
    AddAction (action_beamdamage, "BeamDamage");
    AddAction (action_singledamage, "SingleDamage");
  }

  // For properties.
  propinfo.SetCount (5);
  AddProperty (propid_amount, "amount",
  	CEL_DATA_FLOAT, false, "Amount of damage.", &amount);
  AddProperty (propid_type, "type",
  	CEL_DATA_STRING, false, "Type of damage.", 0);
  AddProperty (propid_sector, "sector",
  	CEL_DATA_STRING, false, "Originating sector.", 0);
  AddProperty (propid_position, "position",
  	CEL_DATA_VECTOR3, false, "Originating position.", &position);
  AddProperty (propid_source, "source",
  	CEL_DATA_STRING, false, "Source of damage.", &amount);

  engine = csQueryRegistry<iEngine> (object_reg);
  if (!engine)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pclogic.damage", "No iEngine plugin!");
    return;
  }
}

celPcDamage::~celPcDamage ()
{
  delete params;
}

bool celPcDamage::SetPropertyIndexed (int idx, const char* b)
{
  switch (idx)
  {
    case propid_type:
      type = b;
      return true;
    case propid_sector:
      sector = b;
      return true;
    case propid_source:
      source = b;
      return true;
    default:
      return false;
  }
}

bool celPcDamage::GetPropertyIndexed (int idx, const char*& s)
{
  switch (idx)
  {
    case propid_type:
      s = type;
      return true;
    case propid_sector:
      s = sector;
      return true;
    case propid_source:
      s = source;
      return true;
    default:
      return false;
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

bool celPcDamage::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_areadamage:
      {
        CEL_FETCH_FLOAT_PAR (radius,params,id_radius);
        if (!p_radius) radius = 1000000000.0f;
        AreaDamage (radius);
        return true;
      }
    case action_beamdamage:
      {
        CEL_FETCH_FLOAT_PAR (maxdist,params,id_maxdist);
        if (!p_maxdist) { maxdist = 1000000000.0f; }
        CEL_FETCH_VECTOR3_PAR (direction,params,id_direction);
        if (!p_direction) return false; // @@@ Error!
        BeamDamage (direction, maxdist);
        return true;
      }
    case action_singledamage:
      {
        CEL_FETCH_STRING_PAR (target,params,id_target);
        if (!p_target) return false;	// @@@ Error!
        SingleDamage (target);
        return true;
      }
    default:
      return false;
  }
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
    default:
      new_amount= amount;
      break;
  }
  params->GetParameter (0).Set (new_amount);
  if (behave)
  {
    celData ret;
    behave->SendMessage ("pcdamage_hurt", 0, ret, params);
  }
  if (!dispatcher_hurt)
  {
    dispatcher_hurt = ent->QueryMessageChannel ()->CreateMessageDispatcher (
	  this, "cel.damage.hurt");
    if (!dispatcher_hurt) return;
  }
  dispatcher_hurt->SendMessage (params);
}

void celPcDamage::SetDamageSource (const char* source)
{
    sourceset = true;
    celPcDamage::source = source;
}

void celPcDamage::CheckSource ()
{
  if (!sourceset)
  {
    sourceset = true;
    source = entity->GetName ();
  }
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
  CheckSource ();
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
    default:
      max_radius = radius;
      break;
  }
  iSector* s;
  csVector3 p;
  GetLocation (s, p);
  if (s == 0) return;	// Can't do area damage.

  params->GetParameter (1).Set (source);
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
  CheckSource ();
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
    default:
      max_radius = maxdist;
      break;
  }
  iSector* s;
  csVector3 p;
  GetLocation (s, p);
  if (s == 0) return;	// Can't do beam damage.

  params->GetParameter (1).Set (source);
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
  CheckSource ();
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

  params->GetParameter (1).Set (source);
  params->GetParameter (2).Set (s == 0 ? "" : (s->QueryObject ()->GetName ()));
  params->GetParameter (3).Set (p);
  params->GetParameter (4).Set (type);
  DoDamage (ent, p);
}

//---------------------------------------------------------------------------

