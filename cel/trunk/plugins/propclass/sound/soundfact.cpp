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
#include "cstool/initapp.h"
#include "iengine/engine.h"
#include "plugins/propclass/sound/soundfact.h"
#include "physicallayer/pl.h"
#include "isndsys/ss_stream.h"
#include "isndsys/ss_manager.h"

#include "iengine/camera.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "csgeom/transfrm.h"
#include "propclass/mesh.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (SoundListener, "pcsound.listener", "pcsoundlistener")
CEL_IMPLEMENT_FACTORY_ALT (SoundSource, "pcsound.source", "pcsoundsource")

//---------------------------------------------------------------------------

class celSoundSourceMovableListener : public scfImplementation1<celSoundSourceMovableListener,
	iMovableListener>
{
private:
  csWeakRef<iSndSysSource3D> soundsource;

public:
  celSoundSourceMovableListener (iSndSysSource3D* soundsource)
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
      csRef<iSndSysSource3DDirectionalSimple> ds =
            scfQueryInterface<iSndSysSource3DDirectionalSimple> (soundsource);
      ds->SetDirection (tr.GetFront ());
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

void celPcSoundSource::Activate ()
{
  // @@@ TODO
}

void celPcSoundSource::Deactivate ()
{
  // @@@ TODO
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
  if (follow && source3d)
  {
    csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    if (pcmesh)
    {
      movlistener.AttachNew (new celSoundSourceMovableListener (
        source3d));
      movable_for_listener = pcmesh->GetMesh ()->GetMovable ();
      movable_for_listener->AddListener (movlistener);
    }
  }
}

//---------------------------------------------------------------------------

csStringID celPcSoundListener::id_front = csInvalidStringID;
csStringID celPcSoundListener::id_top = csInvalidStringID;

PropertyHolder celPcSoundListener::propinfo;

celPcSoundListener::celPcSoundListener (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For actions.
  if (id_front == csInvalidStringID)
  {
    id_front = pl->FetchStringID ("front");
    id_top = pl->FetchStringID ("top");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.sound.listener.action.");
    AddAction (action_setdirection, "SetDirection");
  }

  // For properties.
  propinfo.SetCount (5);
  AddProperty (propid_front, "front",
  	CEL_DATA_VECTOR3, false, "Front direction vector.", 0);
  AddProperty (propid_top, "top",
  	CEL_DATA_VECTOR3, false, "Top direction vector.", 0);
  AddProperty (propid_position, "position",
  	CEL_DATA_VECTOR3, false, "Position vector.", 0);
  AddProperty (propid_distancefactor, "distancefactor",
  	CEL_DATA_FLOAT, false, "Distance factor.", 0);
  AddProperty (propid_rollofffactor, "rollofffactor",
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

void celPcSoundListener::PropertyClassesHaveChanged ()
{
  // no listener means there is no sound system, so not much to do.
  if (!listener)
    return;
  // look for pccamera and place it in separate var, so we can see if it
  // really changed.
  csWeakRef<iPcCamera> found_pccamera = 
	celQueryPropertyClassEntity<iPcCamera> (entity);
  if (found_pccamera == pccamera)
    return;
  // we found a new camera, so assign it to our weakref, and setup the
  // timer listener.
  pccamera = found_pccamera;
  if (pccamera)
  {
    pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  }
  else
  {
    pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  }
}

void celPcSoundListener::TickEveryFrame ()
{
  csReversibleTransform tr = pccamera->GetCamera()->GetTransform ();
  listener->SetPosition ( tr.GetOrigin() );
  listener->SetDirection( tr.GetFront(), tr.GetUp() );
}

bool celPcSoundListener::SetPropertyIndexed (int idx, const csVector3& b)
{
  if (!listener) return false;
  switch (idx)
  {
    case propid_front:
      {
        csVector3 f, t;
        listener->GetDirection (f, t);
        listener->SetDirection (b, t);
        return true;
      }
    case propid_top:
      {
        csVector3 f, t;
        listener->GetDirection (f, t);
        listener->SetDirection (f, b);
        return true;
      }
    case propid_position:
      listener->SetPosition (b);
      return true;
    default:
      return false;
  }
}

bool celPcSoundListener::GetPropertyIndexed (int idx, csVector3& b)
{
  if (!listener) return false;
  switch (idx)
  {
    case propid_front:
      {
        csVector3 t;
        listener->GetDirection (b, t);
        return true;
      }
    case propid_top:
      {
        csVector3 f;
        listener->GetDirection (f, b);
        return true;
      }
    case propid_position:
      b = listener->GetPosition ();
      return true;
    default:
      return false;
  }
}

bool celPcSoundListener::SetPropertyIndexed (int idx, float b)
{
  if (!listener) return false;
  switch (idx)
  {
    case propid_distancefactor:
      listener->SetDistanceFactor (b);
      return true;
    case propid_rollofffactor:
      listener->SetRollOffFactor (b);
      return true;
    default:
      return false;
  }
}

bool celPcSoundListener::GetPropertyIndexed (int idx, float& b)
{
  if (!listener) return false;
  switch (idx)
  {
    case propid_distancefactor:
      b = listener->GetDistanceFactor ();
      return true;
    case propid_rollofffactor:
      b = listener->GetRollOffFactor ();
      return true;
    default:
      return false;
  }
}

bool celPcSoundListener::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  if (!listener) return false;
  if (idx == action_setdirection)
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

PropertyHolder celPcSoundSource::propinfo;

celPcSoundSource::celPcSoundSource (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  mode = CS_SND3D_DISABLE;

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.sound.source.action.");
    AddAction (action_pause, "Pause");
    AddAction (action_unpause, "Unpause");
    AddAction (action_play, "Play");
    AddAction (action_stop, "Stop");
  }

  // For properties.
  propinfo.SetCount (9);
  AddProperty (propid_soundname, "soundname",
	CEL_DATA_STRING, false, "Name of the sound.", 0);
  AddProperty (propid_volume, "volume",
	CEL_DATA_FLOAT, false, "Volume.", 0);
  AddProperty (propid_directionalradiation, "directionalradiation",
	CEL_DATA_FLOAT, false, "Directional radiation.", 0);
  AddProperty (propid_position, "position",
	CEL_DATA_VECTOR3, false, "Position vector.", 0);
  AddProperty (propid_minimumdistance, "minimumdistance",
	CEL_DATA_FLOAT, false, "Minimum distance.", 0);
  AddProperty (propid_maximumdistance, "maximumdistance",
	CEL_DATA_FLOAT, false, "Maximum distance.", 0);
  AddProperty (propid_loop, "loop",
	CEL_DATA_BOOL, false, "Loop.", 0);
  AddProperty (propid_follow, "follow",
	CEL_DATA_BOOL, false, "Whether to follow own entity pcmesh.", 0);
  AddProperty (propid_mode, "mode",
	CEL_DATA_STRING, false, "'absolute', 'relative', or 'disable'.", 0);

  follow = 0;
}

const char* celPcSoundSource::GetMode () const
{
  switch (mode)
  {
    case CS_SND3D_RELATIVE: return "relative";
    case CS_SND3D_ABSOLUTE: return "absolute";
    default: return "disable";
  }
}

void celPcSoundSource::SetMode (const char* modename)
{
  int m;
  if (!strcasecmp ("absolute", modename)) m = CS_SND3D_ABSOLUTE;
  else if (!strcasecmp ("relative", modename)) m = CS_SND3D_RELATIVE;
  else m = CS_SND3D_DISABLE;
  if (mode == m) return;
  mode = m;
  stream = 0;
  source = 0;
  source3d = 0;
}

celPcSoundSource::~celPcSoundSource ()
{
}

bool celPcSoundSource::SetPropertyIndexed (int idx, const csVector3& b)
{
  if (!GetSource ()) return false;
  if (idx == propid_position)
  {
    if (source3d) source3d->SetPosition (b);
    return true;
  }
  return false;
}

bool celPcSoundSource::GetPropertyIndexed (int idx, csVector3& b)
{
  if (!GetSource ()) return false;
  if (idx == propid_position)
  {
    if (source3d) b = source3d->GetPosition ();
    else b.Set (0, 0, 0);
    return true;
  }
  return false;
}

bool celPcSoundSource::SetPropertyIndexed (int idx, float b)
{
  if (!GetSource ()) return false;
  switch (idx)
  {
    case propid_volume:
      source->SetVolume (b);
      return true;
    case propid_directionalradiation:
      if (source3d)
      {
        csRef<iSndSysSource3DDirectionalSimple> ds =
          scfQueryInterface<iSndSysSource3DDirectionalSimple> (source3d);
        ds->SetDirectionalRadiation (b);
      }
      return true;
    case propid_minimumdistance:
      if (source3d) source3d->SetMinimumDistance (b);
      return true;
    case propid_maximumdistance:
      if (source3d) 
      {
        source3d->SetMaximumDistance (b);
      }
      return true;
    default:
      return false;
  }
}

bool celPcSoundSource::GetPropertyIndexed (int idx, float& b)
{
  if (!GetSource ()) return false;
  switch (idx)
  {
    case propid_volume:
      b = source->GetVolume ();
      return true;
    case propid_directionalradiation:
      if (source3d)
      {
        csRef<iSndSysSource3DDirectionalSimple> ds =
          scfQueryInterface<iSndSysSource3DDirectionalSimple> (source3d);
        b = ds->GetDirectionalRadiation ();
      }
      else
	b = 0.0f;
      return true;
    case propid_minimumdistance:
      if (source3d)
        b = source3d->GetMinimumDistance ();
      else
	b = 0.0f;
      return true;
    case propid_maximumdistance:
      if (source3d)
        b = source3d->GetMaximumDistance ();
      else
	b = 0.0f;
      return true;
    default:
      return false;
  }
}

bool celPcSoundSource::SetPropertyIndexed (int idx, bool b)
{
  if (!GetSource ()) return false;
  switch (idx)
  {
    case propid_loop:
      stream->SetLoopState (b ? CS_SNDSYS_STREAM_LOOP :
    	  CS_SNDSYS_STREAM_DONTLOOP);
      return true;
    case propid_follow:
      follow = b;
      UpdateListener();
      return true;
    default:
      return false;
  }
}

bool celPcSoundSource::GetPropertyIndexed (int idx, bool& b)
{
  if (!GetSource ()) return false;
  switch (idx)
  {
    case propid_loop:
      b = stream->GetLoopState () == CS_SNDSYS_STREAM_LOOP;
      return true;
    case propid_follow:
      b = follow;
      return true;
    default:
      return false;
  }
}

bool celPcSoundSource::SetPropertyIndexed (int idx, const char* b)
{
  if (idx == propid_soundname)
  {
    SetSoundName (b);
    return true;
  }
  else if (idx == propid_mode)
  {
    SetMode (b);
    return true;
  }
  return false;
}

bool celPcSoundSource::GetPropertyIndexed (int idx, const char*& b)
{
  if (idx == propid_soundname)
  {
    b = soundname;
    return true;
  }
  else if (idx == propid_mode)
  {
    b = GetMode ();
    return true;
  }
  return false;
}

bool celPcSoundSource::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  if (!GetSource ()) return false;
  switch (idx)
  {
    case action_unpause:
      stream->Unpause ();
      return true;
    case action_stop:
      stream->Pause ();
      stream->ResetPosition ();
      return true;
    case action_play:
      stream->ResetPosition ();
      stream->Unpause ();
      return true;
    case action_pause:
      stream->Pause ();
      return true;
    default:
      return false;
  }
}

void celPcSoundSource::Play ()
{
  if (GetSource ())
  {
    stream->ResetPosition ();
    stream->Unpause ();
  }
}

void celPcSoundSource::Stop ()
{
  if (GetSource ())
  {
    stream->Pause ();
    stream->ResetPosition ();
  }
}

void celPcSoundSource::Pause ()
{
  if (GetSource ())
    stream->Pause ();
}

void celPcSoundSource::Unpause ()
{
  if (GetSource ())
    stream->Unpause ();
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
  if (source && (mode == CS_SND3D_DISABLE ? true : source3d.IsValid ())) return true;
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

  stream = renderer->CreateStream (soundwrap->GetData (), mode);
  csRef<iSndSysSource> src = renderer->CreateSource (stream);
  if (src)
  {
    source = scfQueryInterface<iSndSysSource> (src);
    source3d = scfQueryInterface<iSndSysSource3D> (src);
  }
  return source != 0;
}

void celPcSoundSource::SetSoundName (const char* name)
{
  soundname = name;
  soundwrap = 0;
  source = 0;
  source3d = 0;
  stream = 0;
}

//---------------------------------------------------------------------------

