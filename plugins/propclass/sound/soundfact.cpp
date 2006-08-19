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
#include "cstool/initapp.h"
#include "iengine/engine.h"
#include "plugins/propclass/sound/soundfact.h"
#include "physicallayer/pl.h"
#include "isndsys/ss_stream.h"
#include "isndsys/ss_manager.h"

#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "csgeom/transfrm.h"
#include "propclass/mesh.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (SoundListener, "pcsoundlistener")
CEL_IMPLEMENT_FACTORY (SoundSource, "pcsoundsource")

//---------------------------------------------------------------------------

class celSoundSourceMovableListener : public scfImplementation1<celSoundSourceMovableListener,
	iMovableListener>
{
private:
  csWeakRef<iSndSysSourceSoftware3D> soundsource;

public:
  celSoundSourceMovableListener (iSndSysSourceSoftware3D* soundsource)
  	: scfImplementationType (this), soundsource (soundsource)
  {
  }
  virtual ~celSoundSourceMovableListener () { }
  virtual void MovableChanged (iMovable* movable)
  {
    if (soundsource)
    {
      csReversibleTransform tr = movable->GetFullTransform ();
      soundsource->SetPosition (tr.GetOrigin ());
      soundsource->SetDirection (tr.GetFront ());
    }
  }
  virtual void MovableDestroyed (iMovable*) { }
};

void celPcSoundSource::CheckPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
	  UpdateListener ();
}

void celPcSoundSource::TickEveryFrame ()
{
  if (follow)
    CheckPropertyClasses ();
}

void celPcSoundSource::UpdateListener ()
{
  // Remove listener if present
  if (movlistener)
  {
    if (movable_for_listener)
      movable_for_listener->RemoveListener (movlistener);
    movlistener = 0;
  }
  // Create a new listener if possible and requested
  if (!GetSource ()) return;
  if (follow)
  {
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    if (pcmesh)
    {
      movlistener.AttachNew (new celSoundSourceMovableListener (
        source));
      movable_for_listener = pcmesh->GetMesh ()->GetMovable ();
      movable_for_listener->AddListener (movlistener);
    }
  }
}

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
  UpdateProperties ();
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_front] = 0;
  propdata[propid_top] = 0;
  propdata[propid_position] = 0;
  propdata[propid_distancefactor] = 0;
  propdata[propid_rollofffactor] = 0;

  renderer = csQueryRegistryOrLoad<iSndSysRenderer> (object_reg,
  	"crystalspace.sndsys.renderer.software");
  if (!renderer)
  {
    // @@@ Error report.
    printf ("Error! No sound renderer!\n"); fflush (stdout);
    return;
  }
  listener = renderer->GetListener ();
}

celPcSoundListener::~celPcSoundListener ()
{
}

Property* celPcSoundListener::properties = 0;
size_t celPcSoundListener::propertycount = 0;

void celPcSoundListener::UpdateProperties ()
{
  if (propertycount == 0)
  {
    propertycount = 5;
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
  }
}

bool celPcSoundListener::SetProperty (csStringID propertyId, const csVector3& b)
{
  if (!listener) return false;
  UpdateProperties ();
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
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

bool celPcSoundListener::GetPropertyVector (csStringID propertyId, csVector3& b)
{
  if (!listener) return false;
  UpdateProperties ();
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
  else
  {
    return celPcCommon::GetPropertyVector (propertyId, b);
  }
}

bool celPcSoundListener::SetProperty (csStringID propertyId, float b)
{
  if (!listener) return false;
  UpdateProperties ();
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
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

float celPcSoundListener::GetPropertyFloat (csStringID propertyId)
{
  if (!listener) return 0.0f;
  UpdateProperties ();
  if (propertyId == properties[propid_distancefactor].id)
  {
    return listener->GetDistanceFactor ();
  }
  else if (propertyId == properties[propid_rollofffactor].id)
  {
    return listener->GetRollOffFactor ();
  }
  else
  {
    return celPcCommon::GetPropertyFloat (propertyId);
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
	iCelParameterBlock* params,
	celData& ret)
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

//---------------------------------------------------------------------------

csStringID celPcSoundSource::action_pause = csInvalidStringID;
csStringID celPcSoundSource::action_unpause = csInvalidStringID;

celPcSoundSource::celPcSoundSource (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For PerformAction.
  if (action_pause == csInvalidStringID)
  {
    action_pause = pl->FetchStringID ("cel.action.Pause");
    action_unpause = pl->FetchStringID ("cel.action.Unpause");
  }

  // For properties.
  UpdateProperties ();
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_soundname] = 0;
  propdata[propid_volume] = 0;
  propdata[propid_directionalradiation] = 0;
  propdata[propid_position] = 0;
  propdata[propid_minimumdistance] = 0;
  propdata[propid_maximumdistance] = 0;
  propdata[propid_loop] = 0;
  propdata[propid_follow] = 0;
  follow=0;
}

celPcSoundSource::~celPcSoundSource ()
{
}

Property* celPcSoundSource::properties = 0;
size_t celPcSoundSource::propertycount = 0;

void celPcSoundSource::UpdateProperties ()
{
  if (propertycount == 0)
  {
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

    properties[propid_directionalradiation].id = pl->FetchStringID (
    	"cel.property.directionalradiation");
    properties[propid_directionalradiation].datatype = CEL_DATA_FLOAT;
    properties[propid_directionalradiation].readonly = false;
    properties[propid_directionalradiation].desc = "Directional radiation.";

    properties[propid_position].id = pl->FetchStringID ("cel.property.position");
    properties[propid_position].datatype = CEL_DATA_VECTOR3;
    properties[propid_position].readonly = false;
    properties[propid_position].desc = "Position vector.";

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

    properties[propid_loop].id = pl->FetchStringID ("cel.property.loop");
    properties[propid_loop].datatype = CEL_DATA_BOOL;
    properties[propid_loop].readonly = false;
    properties[propid_loop].desc = "Loop.";

    properties[propid_follow].id = pl->FetchStringID ("cel.property.follow");
    properties[propid_follow].datatype = CEL_DATA_BOOL;
    properties[propid_follow].readonly = false;
    properties[propid_follow].desc = "Whether to follow own entity pcmesh.";
  }
}

bool celPcSoundSource::SetProperty (csStringID propertyId, const csVector3& b)
{
  if (!GetSource ()) return false;
  UpdateProperties ();
  if (propertyId == properties[propid_position].id)
  {
    source->SetPosition (b);
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
  UpdateProperties ();
  if (propertyId == properties[propid_position].id)
  {
    b = source->GetPosition ();
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
  UpdateProperties ();
  if (propertyId == properties[propid_volume].id)
  {
    source->SetVolume (b);
    return true;
  }
  else if (propertyId == properties[propid_directionalradiation].id)
  {
    source->SetDirectionalRadiation (b);
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
  UpdateProperties ();
  if (propertyId == properties[propid_volume].id)
  {
    return source->GetVolume ();
  }
  else if (propertyId == properties[propid_directionalradiation].id)
  {
    return source->GetDirectionalRadiation ();
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

bool celPcSoundSource::SetProperty (csStringID propertyId, bool b)
{
  if (!GetSource ()) return false;
  UpdateProperties ();
  if (propertyId == properties[propid_loop].id)
  {
    source->GetStream ()->SetLoopState (b ? CS_SNDSYS_STREAM_LOOP :
    	CS_SNDSYS_STREAM_DONTLOOP);
    return true;
  }
  else if (propertyId == properties[propid_follow].id)
  {
    follow = b;
    UpdateListener();
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

bool celPcSoundSource::GetPropertyBool (csStringID propertyId)
{
  if (!GetSource ()) return false;
  UpdateProperties ();
  if (propertyId == properties[propid_loop].id)
  {
    return source->GetStream ()->GetLoopState () == CS_SNDSYS_STREAM_LOOP;
  }
  else if (propertyId == properties[propid_follow].id)
  {
    return follow;
  }
  else
  {
    return celPcCommon::GetPropertyBool (propertyId);
  }
}

bool celPcSoundSource::SetProperty (csStringID propertyId, const char* b)
{
  UpdateProperties ();
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
  UpdateProperties ();
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
	iCelParameterBlock* params,
	celData& ret)
{
  if (!GetSource ()) return false;
  if (actionId == action_unpause)
  {
    source->GetStream ()->Unpause ();
    return true;
  }
  else if (actionId == action_pause)
  {
    source->GetStream ()->Pause ();
    return true;
  }
  return false;
}

void celPcSoundSource::GetSoundWrap ()
{
  if (!soundwrap)
  {
    csRef<iSndSysManager> mgr = csQueryRegistryOrLoad<iSndSysManager> (
    	object_reg, "crystalspace.sndsys.manager");
    if (!mgr)
    {
      // @@@ Error report.
      printf ("Error! No sound manager!\n"); fflush (stdout);
      return;
    }

    soundwrap = mgr->FindSoundByName (soundname);
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
  if (source) return true;
  GetSoundWrap ();
  if (!soundwrap) return false;
  csRef<iSndSysRenderer> renderer = csQueryRegistryOrLoad<iSndSysRenderer> (
  	object_reg, "crystalspace.sndsys.renderer.software");
  if (!renderer)
  {
    // @@@ Error report.
    printf ("Error! No sound renderer!\n"); fflush (stdout);
    return false;
  }
  csRef<iSndSysSource> src = renderer->CreateSource (soundwrap->GetStream ());
  if (src)
    source = SCF_QUERY_INTERFACE (src, iSndSysSourceSoftware3D);
  return source != 0;
}

void celPcSoundSource::SetSoundName (const char* name)
{
  soundname = name;
  soundwrap = 0;
  source = 0;
}

//---------------------------------------------------------------------------

