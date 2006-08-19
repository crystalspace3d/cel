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

PropertyHolder celPcSoundListener::propinfo;

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
  propholder = &propinfo;
  propinfo.SetCount (5);
  AddProperty (propid_front, "cel.property.front",
	CEL_DATA_VECTOR3, false, "Front direction vector.", 0);
  AddProperty (propid_top, "cel.property.top",
	CEL_DATA_VECTOR3, false, "Top direction vector.", 0);
  AddProperty (propid_position, "cel.property.position",
	CEL_DATA_VECTOR3, false, "Position vector.", 0);
  AddProperty (propid_distancefactor, "cel.property.distancefactor",
	CEL_DATA_FLOAT, false, "Distance factor.", 0);
  AddProperty (propid_rollofffactor, "cel.property.rollofffactor",
	CEL_DATA_FLOAT, false, "Rolloff factor.", 0);

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

bool celPcSoundListener::SetProperty (csStringID propertyId, const csVector3& b)
{
  if (!listener) return false;
  if (propinfo.TestID (propid_front, propertyId))
  {
    csVector3 f, t;
    listener->GetDirection (f, t);
    listener->SetDirection (b, t);
    return true;
  }
  else if (propinfo.TestID (propid_top, propertyId))
  {
    csVector3 f, t;
    listener->GetDirection (f, t);
    listener->SetDirection (f, b);
    return true;
  }
  else if (propinfo.TestID (propid_position, propertyId))
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
  if (propinfo.TestID (propid_front, propertyId))
  {
    csVector3 t;
    listener->GetDirection (b, t);
    return true;
  }
  else if (propinfo.TestID (propid_top, propertyId))
  {
    csVector3 f;
    listener->GetDirection (f, b);
    return true;
  }
  else if (propinfo.TestID (propid_position, propertyId))
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
  if (propinfo.TestID (propid_distancefactor, propertyId))
  {
    listener->SetDistanceFactor (b);
    return true;
  }
  else if (propinfo.TestID (propid_rollofffactor, propertyId))
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
  if (propinfo.TestID (propid_distancefactor, propertyId))
  {
    return listener->GetDistanceFactor ();
  }
  else if (propinfo.TestID (propid_rollofffactor, propertyId))
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

PropertyHolder celPcSoundSource::propinfo;

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
  propholder = &propinfo;
  propinfo.SetCount (8);
  AddProperty (propid_soundname, "cel.property.soundname",
	CEL_DATA_STRING, false, "Name of the sound.", 0);
  AddProperty (propid_volume, "cel.property.volume",
	CEL_DATA_FLOAT, false, "Volume.", 0);
  AddProperty (propid_directionalradiation, "cel.property.directionalradiation",
	CEL_DATA_FLOAT, false, "Directional radiation.", 0);
  AddProperty (propid_position, "cel.property.position",
	CEL_DATA_VECTOR3, false, "Position vector.", 0);
  AddProperty (propid_minimumdistance, "cel.property.minimumdistance",
	CEL_DATA_FLOAT, false, "Minimum distance.", 0);
  AddProperty (propid_maximumdistance, "cel.property.maximumdistance",
	CEL_DATA_FLOAT, false, "Maximum distance.", 0);
  AddProperty (propid_loop, "cel.property.loop",
	CEL_DATA_BOOL, false, "Loop.", 0);
  AddProperty (propid_follow, "cel.property.follow",
	CEL_DATA_BOOL, false, "Whether to follow own entity pcmesh.", 0);

  follow = 0;
}

celPcSoundSource::~celPcSoundSource ()
{
}

bool celPcSoundSource::SetProperty (csStringID propertyId, const csVector3& b)
{
  if (!GetSource ()) return false;
  if (propinfo.TestID (propid_position, propertyId))
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
  if (propinfo.TestID (propid_position, propertyId))
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
  if (propinfo.TestID (propid_volume, propertyId))
  {
    source->SetVolume (b);
    return true;
  }
  else if (propinfo.TestID (propid_directionalradiation, propertyId))
  {
    source->SetDirectionalRadiation (b);
    return true;
  }
  else if (propinfo.TestID (propid_minimumdistance, propertyId))
  {
    source->SetMinimumDistance (b);
    return true;
  }
  else if (propinfo.TestID (propid_maximumdistance, propertyId))
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
  if (propinfo.TestID (propid_volume, propertyId))
  {
    return source->GetVolume ();
  }
  else if (propinfo.TestID (propid_directionalradiation, propertyId))
  {
    return source->GetDirectionalRadiation ();
  }
  else if (propinfo.TestID (propid_minimumdistance, propertyId))
  {
    return source->GetMinimumDistance ();
  }
  else if (propinfo.TestID (propid_maximumdistance, propertyId))
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
  if (propinfo.TestID (propid_loop, propertyId))
  {
    source->GetStream ()->SetLoopState (b ? CS_SNDSYS_STREAM_LOOP :
    	CS_SNDSYS_STREAM_DONTLOOP);
    return true;
  }
  else if (propinfo.TestID (propid_follow, propertyId))
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
  if (propinfo.TestID (propid_loop, propertyId))
  {
    return source->GetStream ()->GetLoopState () == CS_SNDSYS_STREAM_LOOP;
  }
  else if (propinfo.TestID (propid_follow, propertyId))
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
  if (propinfo.TestID (propid_soundname, propertyId))
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
  if (propinfo.TestID (propid_soundname, propertyId))
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

