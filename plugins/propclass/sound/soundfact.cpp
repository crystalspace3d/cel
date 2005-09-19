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
#include "iutil/objreg.h"
#include "iutil/plugin.h"
#include "iutil/object.h"
#include "csutil/debug.h"
#include "iengine/engine.h"
#include "plugins/propclass/sound/soundfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "isound/handle.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (SoundListener, "pcsoundlistener")
CEL_IMPLEMENT_FACTORY (SoundSource, "pcsoundsource")

//---------------------------------------------------------------------------

csStringID celPcSoundListener::action_setdirection = csInvalidStringID;
csStringID celPcSoundListener::id_front = csInvalidStringID;
csStringID celPcSoundListener::id_top = csInvalidStringID;

celPcSoundListener::celPcSoundListener (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For PerformAction.
  if (action_setdirection == csInvalidStringID)
  {
    action_setdirection = pl->FetchStringID ("cel.action.SetDirection");
    id_front = pl->FetchStringID ("cel.parameter.front");
    id_top = pl->FetchStringID ("cel.parameter.top");
  }

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_front] = 0;
  propdata[propid_top] = 0;
  propdata[propid_position] = 0;
  propdata[propid_velocity] = 0;
  propdata[propid_distancefactor] = 0;
  propdata[propid_rollofffactor] = 0;
  propdata[propid_dopplerfactor] = 0;
  propdata[propid_headsize] = 0;
  propdata[propid_environment] = 0;

  renderer = CS_QUERY_REGISTRY (object_reg, iSoundRender);
  if (!renderer)
  {
    // @@@ Report error?
    printf ("Can't find renderer!\n");
    fflush (stdout);
    return;
  }
  listener = renderer->GetListener ();
}

celPcSoundListener::~celPcSoundListener ()
{
}

Property* celPcSoundListener::properties = 0;
size_t celPcSoundListener::propertycount = 0;

void celPcSoundListener::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    propertycount = 9;
    properties = new Property[propertycount];

    properties[propid_front].id = pl->FetchStringID ("cel.property.front");
    properties[propid_front].datatype = CEL_DATA_VECTOR3;
    properties[propid_front].readonly = false;
    properties[propid_front].desc = "Front direction vector.";

    properties[propid_top].id = pl->FetchStringID ("cel.property.top");
    properties[propid_top].datatype = CEL_DATA_VECTOR3;
    properties[propid_top].readonly = false;
    properties[propid_top].desc = "Top direction vector.";

    properties[propid_position].id = pl->FetchStringID ("cel.property.position");
    properties[propid_position].datatype = CEL_DATA_VECTOR3;
    properties[propid_position].readonly = false;
    properties[propid_position].desc = "Position vector.";

    properties[propid_velocity].id = pl->FetchStringID ("cel.property.velocity");
    properties[propid_velocity].datatype = CEL_DATA_VECTOR3;
    properties[propid_velocity].readonly = false;
    properties[propid_velocity].desc = "Velocity direction vector.";

    properties[propid_distancefactor].id = pl->FetchStringID (
    	"cel.property.distancefactor");
    properties[propid_distancefactor].datatype = CEL_DATA_FLOAT;
    properties[propid_distancefactor].readonly = false;
    properties[propid_distancefactor].desc = "Distance factor.";

    properties[propid_rollofffactor].id = pl->FetchStringID (
    	"cel.property.rollofffactor");
    properties[propid_rollofffactor].datatype = CEL_DATA_FLOAT;
    properties[propid_rollofffactor].readonly = false;
    properties[propid_rollofffactor].desc = "Rolloff factor.";

    properties[propid_dopplerfactor].id = pl->FetchStringID (
    	"cel.property.dopplerfactor");
    properties[propid_dopplerfactor].datatype = CEL_DATA_FLOAT;
    properties[propid_dopplerfactor].readonly = false;
    properties[propid_dopplerfactor].desc = "Doppler factor.";

    properties[propid_headsize].id = pl->FetchStringID ("cel.property.headsize");
    properties[propid_headsize].datatype = CEL_DATA_FLOAT;
    properties[propid_headsize].readonly = false;
    properties[propid_headsize].desc = "Head size.";

    properties[propid_environment].id = pl->FetchStringID (
    	"cel.property.environment");
    properties[propid_environment].datatype = CEL_DATA_STRING;
    properties[propid_environment].readonly = false;
    properties[propid_environment].desc = "Environment specification.";
  }
}

bool celPcSoundListener::SetProperty (csStringID propertyId, const csVector3& b)
{
  if (!listener) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_front].id)
  {
    csVector3 f, t;
    listener->GetDirection (f, t);
    listener->SetDirection (b, t);
    return true;
  }
  else if (propertyId == properties[propid_top].id)
  {
    csVector3 f, t;
    listener->GetDirection (f, t);
    listener->SetDirection (f, b);
    return true;
  }
  else if (propertyId == properties[propid_position].id)
  {
    listener->SetPosition (b);
    return true;
  }
  else if (propertyId == properties[propid_velocity].id)
  {
    listener->SetVelocity (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

bool celPcSoundListener::GetPropertyVector (csStringID propertyId, csVector3& b)
{
  if (!listener) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_front].id)
  {
    csVector3 t;
    listener->GetDirection (b, t);
    return true;
  }
  else if (propertyId == properties[propid_top].id)
  {
    csVector3 f;
    listener->GetDirection (f, b);
    return true;
  }
  else if (propertyId == properties[propid_position].id)
  {
    b = listener->GetPosition ();
    return true;
  }
  else if (propertyId == properties[propid_velocity].id)
  {
    b = listener->GetVelocity ();
    return true;
  }
  else
  {
    return celPcCommon::GetPropertyVector (propertyId, b);
  }
}

bool celPcSoundListener::SetProperty (csStringID propertyId, float b)
{
  if (!listener) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_distancefactor].id)
  {
    listener->SetDistanceFactor (b);
    return true;
  }
  else if (propertyId == properties[propid_rollofffactor].id)
  {
    listener->SetRollOffFactor (b);
    return true;
  }
  else if (propertyId == properties[propid_dopplerfactor].id)
  {
    listener->SetDopplerFactor (b);
    return true;
  }
  else if (propertyId == properties[propid_headsize].id)
  {
    listener->SetHeadSize (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

float celPcSoundListener::GetPropertyFloat (csStringID propertyId)
{
  if (!listener) return 0.0f;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_distancefactor].id)
  {
    return listener->GetDistanceFactor ();
  }
  else if (propertyId == properties[propid_rollofffactor].id)
  {
    return listener->GetRollOffFactor ();
  }
  else if (propertyId == properties[propid_dopplerfactor].id)
  {
    return listener->GetDopplerFactor ();
  }
  else if (propertyId == properties[propid_headsize].id)
  {
    return listener->GetHeadSize ();
  }
  else
  {
    return celPcCommon::GetPropertyFloat (propertyId);
  }
}

bool celPcSoundListener::SetProperty (csStringID propertyId, const char* b)
{
  if (!listener) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_environment].id)
  {
    SetEnvironment (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

const char* celPcSoundListener::GetPropertyString (csStringID propertyId)
{
  if (!listener) return 0;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_environment].id)
  {
    return GetEnvironment ();
  }
  else
  {
    return celPcCommon::GetPropertyString (propertyId);
  }
}

#define SOUNDLISTENER_SERIAL 2

csPtr<iCelDataBuffer> celPcSoundListener::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (SOUNDLISTENER_SERIAL);
  //databuf->Add (int32 (counter));
  //databuf->Add (int32 (max));
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcSoundListener::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != SOUNDLISTENER_SERIAL) return false;

  //counter = databuf->GetInt32 ();
  //max = databuf->GetInt32 ();

  return true;
}

bool celPcSoundListener::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (!listener) return false;
  if (actionId == action_setdirection)
  {
    CEL_FETCH_VECTOR3_PAR (front,params,id_front);
    if (!p_front) return false;
    CEL_FETCH_VECTOR3_PAR (top,params,id_top);
    if (!p_top) return false;
    listener->SetDirection (front, top);
    return true;
  }
  return false;
}

const char* env_table[] =
{
  "generic",
  "paddedcell",
  "room",
  "bathroom",
  "livingroom",
  "stoneroom",
  "auditorium",
  "concerthall",
  "cave",
  "arena",
  "carpetedhallway",
  "hallway",
  "stonecorridor",
  "alley",
  "forest",
  "city",
  "mountains",
  "quarry",
  "plain",
  "parkinglot",
  "sewerpipe",
  "underwater",
  "drugged",
  "dizzy",
  "psychotic"
};

void celPcSoundListener::SetEnvironment (const char* env)
{
  int i;
  for (i = 0 ; i <= ENVIRONMENT_PSYCHOTIC ; i++)
    if (!strcmp (env, env_table[i]))
    {
      listener->SetEnvironment ((csSoundEnvironment)i);
      break;
    }
}

const char* celPcSoundListener::GetEnvironment ()
{
  int e = listener->GetEnvironment ();
  if (e < 0 || e > ENVIRONMENT_PSYCHOTIC) return "<unknown>";
  return env_table[e];
}

//---------------------------------------------------------------------------

csStringID celPcSoundSource::action_play = csInvalidStringID;
csStringID celPcSoundSource::id_method = csInvalidStringID;
csStringID celPcSoundSource::action_stop = csInvalidStringID;

celPcSoundSource::celPcSoundSource (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For PerformAction.
  if (action_play == csInvalidStringID)
  {
    action_play = pl->FetchStringID ("cel.action.Play");
    id_method = pl->FetchStringID ("cel.parameter.method");
    action_stop = pl->FetchStringID ("cel.action.Stop");
  }

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_soundname] = 0;
  propdata[propid_volume] = 0;
  propdata[propid_frequencyfactor] = 0;
  propdata[propid_mode3d] = 0;
  propdata[propid_position] = 0;
  propdata[propid_velocity] = 0;
  propdata[propid_minimumdistance] = 0;
  propdata[propid_maximumdistance] = 0;
}

celPcSoundSource::~celPcSoundSource ()
{
}

Property* celPcSoundSource::properties = 0;
size_t celPcSoundSource::propertycount = 0;

void celPcSoundSource::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    propertycount = 8;
    properties = new Property[propertycount];

    properties[propid_soundname].id = pl->FetchStringID ("cel.property.soundname");
    properties[propid_soundname].datatype = CEL_DATA_STRING;
    properties[propid_soundname].readonly = false;
    properties[propid_soundname].desc = "Name of the sound.";

    properties[propid_volume].id = pl->FetchStringID ("cel.property.volume");
    properties[propid_volume].datatype = CEL_DATA_FLOAT;
    properties[propid_volume].readonly = false;
    properties[propid_volume].desc = "Volume.";

    properties[propid_frequencyfactor].id = pl->FetchStringID (
    	"cel.property.frequencyfactor");
    properties[propid_frequencyfactor].datatype = CEL_DATA_FLOAT;
    properties[propid_frequencyfactor].readonly = false;
    properties[propid_frequencyfactor].desc = "Frequency factor.";

    properties[propid_mode3d].id = pl->FetchStringID ("cel.property.mode3d");
    properties[propid_mode3d].datatype = CEL_DATA_LONG;
    properties[propid_mode3d].readonly = false;
    properties[propid_mode3d].desc = "3D Mode.";

    properties[propid_position].id = pl->FetchStringID ("cel.property.position");
    properties[propid_position].datatype = CEL_DATA_VECTOR3;
    properties[propid_position].readonly = false;
    properties[propid_position].desc = "Position vector.";

    properties[propid_velocity].id = pl->FetchStringID ("cel.property.velocity");
    properties[propid_velocity].datatype = CEL_DATA_VECTOR3;
    properties[propid_velocity].readonly = false;
    properties[propid_velocity].desc = "Velocity direction vector.";

    properties[propid_minimumdistance].id = pl->FetchStringID (
    	"cel.property.minimumdistance");
    properties[propid_minimumdistance].datatype = CEL_DATA_FLOAT;
    properties[propid_minimumdistance].readonly = false;
    properties[propid_minimumdistance].desc = "Minimum distance.";

    properties[propid_maximumdistance].id = pl->FetchStringID (
    	"cel.property.maximumdistance");
    properties[propid_maximumdistance].datatype = CEL_DATA_FLOAT;
    properties[propid_maximumdistance].readonly = false;
    properties[propid_maximumdistance].desc = "Maximum distance.";
  }
}

bool celPcSoundSource::SetProperty (csStringID propertyId, const csVector3& b)
{
  if (!GetSource ()) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_position].id)
  {
    source->SetPosition (b);
    return true;
  }
  else if (propertyId == properties[propid_velocity].id)
  {
    source->SetVelocity (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

bool celPcSoundSource::GetPropertyVector (csStringID propertyId, csVector3& b)
{
  if (!GetSource ()) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_position].id)
  {
    b = source->GetPosition ();
    return true;
  }
  else if (propertyId == properties[propid_velocity].id)
  {
    b = source->GetVelocity ();
    return true;
  }
  else
  {
    return celPcCommon::GetPropertyVector (propertyId, b);
  }
}

bool celPcSoundSource::SetProperty (csStringID propertyId, float b)
{
  if (!GetSource ()) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_volume].id)
  {
    source->SetVolume (b);
    return true;
  }
  else if (propertyId == properties[propid_frequencyfactor].id)
  {
    source->SetFrequencyFactor (b);
    return true;
  }
  else if (propertyId == properties[propid_minimumdistance].id)
  {
    source->SetMinimumDistance (b);
    return true;
  }
  else if (propertyId == properties[propid_maximumdistance].id)
  {
    source->SetMaximumDistance (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

float celPcSoundSource::GetPropertyFloat (csStringID propertyId)
{
  if (!GetSource ()) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_volume].id)
  {
    return source->GetVolume ();
  }
  else if (propertyId == properties[propid_frequencyfactor].id)
  {
    return source->GetFrequencyFactor ();
  }
  else if (propertyId == properties[propid_minimumdistance].id)
  {
    return source->GetMinimumDistance ();
  }
  else if (propertyId == properties[propid_maximumdistance].id)
  {
    return source->GetMaximumDistance ();
  }
  else
  {
    return celPcCommon::GetPropertyFloat (propertyId);
  }
}

bool celPcSoundSource::SetProperty (csStringID propertyId, long b)
{
  if (!GetSource ()) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_mode3d].id)
  {
    source->SetMode3D (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

long celPcSoundSource::GetPropertyLong (csStringID propertyId)
{
  if (!GetSource ()) return 0;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_mode3d].id)
  {
    return source->GetMode3D ();
  }
  else
  {
    return celPcCommon::GetPropertyLong (propertyId);
  }
}

bool celPcSoundSource::SetProperty (csStringID propertyId, const char* b)
{
  if (!GetSource ()) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_soundname].id)
  {
    SetSoundName (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

const char* celPcSoundSource::GetPropertyString (csStringID propertyId)
{
  if (!GetSource ()) return false;
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_soundname].id)
  {
    return soundname;
  }
  else
  {
    return celPcCommon::GetPropertyString (propertyId);
  }
}

#define SOUNDSOURCE_SERIAL 2

csPtr<iCelDataBuffer> celPcSoundSource::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (SOUNDSOURCE_SERIAL);
  //databuf->Add (int32 (counter));
  //databuf->Add (int32 (max));
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcSoundSource::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != SOUNDSOURCE_SERIAL) return false;

  //counter = databuf->GetInt32 ();
  //max = databuf->GetInt32 ();

  return true;
}

bool celPcSoundSource::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (!GetSource ()) return false;
  if (actionId == action_play)
  {
    CEL_FETCH_LONG_PAR (method,params,id_method);
    if (!p_method) return false;
    source->Play (method);
    return true;
  }
  else if (actionId == action_stop)
  {
    source->Stop ();
    return true;
  }
  return false;
}

void celPcSoundSource::GetSoundWrap ()
{
  if (!soundwrap)
  {
    csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
    soundwrap = CS_GET_NAMED_CHILD_OBJECT (engine->QueryObject (),
  	iSoundWrapper, soundname);
    if (!soundwrap)
    {
      // @@@ Report error?
      printf ("Can't find sound '%s'!\n", (const char*)soundname);
      fflush (stdout);
    }
  }
}

bool celPcSoundSource::GetSource ()
{
  GetSoundWrap ();
  if (!soundwrap) return false;
  source = soundwrap->GetSound ()->CreateSource (SOUND3D_DISABLE);
  return source != 0;
}

void celPcSoundSource::SetSoundName (const char* name)
{
  soundname = name;
  soundwrap = 0;
  source = 0;
}

//---------------------------------------------------------------------------

