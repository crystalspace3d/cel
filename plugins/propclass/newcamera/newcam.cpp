/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
    Copyright (C) 2007 by Dariusz Dawidowski

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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include <math.h>
#include "propclass/mesh.h"
#include "propclass/solid.h"
#include "propclass/zone.h"
#include "plugins/propclass/newcamera/newcam.h"
#include "plugins/propclass/newcamera/modes/firstperson.h"
#include "plugins/propclass/newcamera/modes/thirdperson.h"
#include "plugins/propclass/newcamera/modes/tracking.h"
#include "plugins/propclass/newcamera/modes/horizontal.h"
#include "plugins/propclass/newcamera/modes/isometric.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/flags.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "iutil/virtclk.h"
#include "iutil/csinput.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "imap/loader.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/camera.h"
#include "iengine/region.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "ivideo/graph2d.h"
#include "cstool/csview.h"
#include "cstool/collider.h"
#include "csgeom/poly3d.h"
#include "csgeom/transfrm.h"
#include "csgeom/sphere.h"
#include "ivaria/view.h"
#include "ivaria/collider.h"
#include "ivaria/reporter.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "csqsqrt.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY_ALT (NewCamera, "pccamera.standard", "pcnewcamera")

//---------------------------------------------------------------------------

csStringID celPcNewCamera::id_name = csInvalidStringID;
csStringID celPcNewCamera::id_nr = csInvalidStringID;
csStringID celPcNewCamera::id_x = csInvalidStringID;
csStringID celPcNewCamera::id_y = csInvalidStringID;
csStringID celPcNewCamera::id_w = csInvalidStringID;
csStringID celPcNewCamera::id_h = csInvalidStringID;
csStringID celPcNewCamera::id_enable = csInvalidStringID;
csStringID celPcNewCamera::id_minfps = csInvalidStringID;
csStringID celPcNewCamera::id_maxfps = csInvalidStringID;
csStringID celPcNewCamera::id_mindist = csInvalidStringID;
csStringID celPcNewCamera::id_dist = csInvalidStringID;

PropertyHolder celPcNewCamera::propinfo;

void celPcNewCamera::UpdateMeshVisibility ()
{
  if (!pcmesh)
    return;

  if (currmode >= cameramodes.GetSize ())
    return;

  if (!pcmesh->GetMesh ())
    return;

  if (cameramodes[currmode]->DrawAttachedMesh ())
    pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE, 0);
  else
    pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE,
      CS_ENTITY_INVISIBLE);
}

csVector3 celPcNewCamera::InterpolateVector (float i, const csVector3& curr, const csVector3& next)
{
  // a bit of a clamp [0, 1]
  i = (i > 1.0f) ? 1.0f : ((i < 0.0f) ? 0.0f : i);
  // a classic
  return i * (next - curr) + curr;
}

celPcNewCamera::celPcNewCamera (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);

  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_VIEW);

  init_reset = false;

  minoffset = 6.0f;

  currmode = (size_t)-1;

  docolldet = true;
  collyfocusoff = 2.0f;

  in_transition = false;
  transtime = 0.5f;
  currtrans = 0.0f;


  if (id_name == csInvalidStringID)
  {
    id_name = pl->FetchStringID ("cel.parameter.name");
    id_nr = pl->FetchStringID ("cel.parameter.nr");
    id_x = pl->FetchStringID ("cel.parameter.x");
    id_y = pl->FetchStringID ("cel.parameter.y");
    id_w = pl->FetchStringID ("cel.parameter.w");
    id_h = pl->FetchStringID ("cel.parameter.h");
    id_enable = pl->FetchStringID ("cel.parameter.enable");
    id_minfps = pl->FetchStringID ("cel.parameter.min_fps");
    id_maxfps = pl->FetchStringID ("cel.parameter.max_fps");
    id_mindist = pl->FetchStringID ("cel.parameter.min_distance");
    id_dist = pl->FetchStringID ("cel.parameter.distance");
  }
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_name, "name");
  params->SetParameterDef (id_nr, "nr");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_attachcameramode, "cel.action.AttachCameraMode");
    AddAction (action_setcameramode, "cel.action.SetCameraMode");
    AddAction (action_nextcameramode, "cel.action.NextCameraMode");
    AddAction (action_prevcameramode, "cel.action.PrevCameraMode");
    AddAction (action_setrectangle, "cel.action.SetRectangle");
    AddAction (action_setperspcenter, "cel.action.SetPerspectiveCenter");
    AddAction (action_adaptiveclipping, "cel.action.AdaptiveDistanceClipping");
    AddAction (action_fixedclipping, "cel.action.FixedDistanceClipping");
  }

  propinfo.SetCount (5);
  AddProperty (propid_colldet, "cel.property.colldet",
    CEL_DATA_BOOL, false, "Camera will use collision detection.", 0);
  AddProperty (propid_spring, "cel.property.spring",
    CEL_DATA_FLOAT, false, "Common spring coefficient.", 0);
  AddProperty (propid_spring_origin, "cel.property.spring_origin",
    CEL_DATA_FLOAT, false, "Spring coefficient for origin.",
    &originspring);
  AddProperty (propid_spring_target, "cel.property.spring_target",
    CEL_DATA_FLOAT, false, "Spring coefficient for target.",
    &targetspring);
  AddProperty (propid_spring_up, "cel.property.spring_up",
    CEL_DATA_FLOAT, false, "Spring coefficient for up vector.",
    &upspring);
}

celPcNewCamera::~celPcNewCamera ()
{
}

bool celPcNewCamera::PerformActionIndexed (int idx,
  iCelParameterBlock* params,
  celData& ret)
{
  switch (idx)
  {
    case action_attachcameramode:
    {
      CEL_FETCH_STRING_PAR (name,params,id_name);
      if (!p_name) return true;
      if (!strcmp (name, "camera_firstperson"))
      {
        AttachCameraMode (iPcNewCamera::CCM_FIRST_PERSON);
        return true;
      }
      if (!strcmp (name, "camera_thirdperson"))
      {
        AttachCameraMode (iPcNewCamera::CCM_THIRD_PERSON);
        return true;
      }
      if (!strcmp (name, "camera_tracking"))
      {
        AttachCameraMode (iPcNewCamera::CCM_TRACKING);
        return true;
      }
      if (!strcmp (name, "camera_horizontal"))
      {
        AttachCameraMode (iPcNewCamera::CCM_HORIZONTAL);
        return true;
      }
      if (!strcmp (name, "camera_isometric"))
      {
        AttachCameraMode (iPcNewCamera::CCM_ISOMETRIC);
        return true;
      }
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
        "cel.camera.standard",
        "Unknown camera mode");
      return true;
    }
    case action_setcameramode:
    {
      CEL_FETCH_LONG_PAR (nr,params,id_nr);
      if (!p_nr) return false;
      return SetCurrentCameraMode (nr);
    }
    case action_nextcameramode:
    {
      NextCameraMode ();
      return true;
    }
    case action_prevcameramode:
    {
      PrevCameraMode ();
      return true;
    }
    case action_setrectangle:
    {
      CEL_FETCH_LONG_PAR (x,params,id_x);
      if (!p_x) return false;
      CEL_FETCH_LONG_PAR (y,params,id_y);
      if (!p_y) return false;
      CEL_FETCH_LONG_PAR (w,params,id_w);
      if (!p_w) return false;
      CEL_FETCH_LONG_PAR (h,params,id_h);
      if (!p_h) return false;
      SetRectangle (x, y, w, h);
      return true;
    }
    case action_setperspcenter:
    {
      CEL_FETCH_FLOAT_PAR (x,params,id_x);
      if (!p_x) return false;
      CEL_FETCH_FLOAT_PAR (y,params,id_y);
      if (!p_y) return false;
      SetPerspectiveCenter (x, y);
      return true;
    }
    case action_adaptiveclipping:
    {
      CEL_FETCH_BOOL_PAR (enable,params,id_enable);
      if (!p_enable) return false;
      if (enable == true)
      {
        CEL_FETCH_FLOAT_PAR (minfps,params,id_minfps);
        if (!p_minfps) return false;
        CEL_FETCH_FLOAT_PAR (maxfps,params,id_maxfps);
        if (!p_maxfps) return false;
        CEL_FETCH_FLOAT_PAR (mindist,params,id_mindist);
        if (!p_mindist) return false;
        EnableAdaptiveDistanceClipping (minfps, maxfps, mindist);
      }
      else
        DisableDistanceClipping ();
      return true;
    }
    case action_fixedclipping:
    {
      CEL_FETCH_BOOL_PAR (enable,params,id_enable);
      if (!p_enable) return false;
      if (enable == true)
      {
        CEL_FETCH_FLOAT_PAR (dist,params,id_dist);
        if (!p_dist) return false;
        EnableFixedDistanceClipping (dist);
      }
      else
        DisableDistanceClipping ();
      return true;
    }
    default:
      return false;
  }
}

bool celPcNewCamera::SetPropertyIndexed (int idx, bool val)
{
  switch (idx)
  {
    case propid_colldet:
    {
      SetCollisionDetection (val);
      return true;
    }
    default:
      return false;
  }
}

bool celPcNewCamera::SetPropertyIndexed (int idx, float val)
{
  switch (idx)
  {
    case propid_spring:
    {
      originspring = val;   // seriously, wtf?
      targetspring = val;
      upspring = val;
      return true;
    }
    default:
      return false;
  }
}

bool celPcNewCamera::GetPropertyIndexed (int idx, bool& val)
{
  switch (idx)
  {
    case propid_colldet:
    {
      val = GetCollisionDetection ();
      return true;
    }
    case propid_spring:
    {
      val = originspring;
      return true;
    }
    default:
      return false;
  }
}

bool celPcNewCamera::GetPropertyIndexed (int idx, float& val)
{
  switch (idx)
  {
    case propid_spring:
    {
      val = originspring;
      return true;
    }
    default:
      return false;
  }
}

bool celPcNewCamera::Reset ()
{
  pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  if (pcmesh && pcmesh->GetMesh ())
  {
    iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
    currcam.pos = movable->GetTransform ().GetOrigin ();
    currcam.dir = movable->GetTransform ().
      This2OtherRelative (csVector3 (0.0f, 0.0f, -1.0f));
    currcam.up  = movable->GetTransform ().
      This2OtherRelative (csVector3 (0.0f, 1.0f, 0.0f));
  }

  UpdateMeshVisibility ();
  return true;
}

const csVector3& celPcNewCamera::GetBasePos () const
{
  return base.pos;
}

const csVector3& celPcNewCamera::GetBaseOrigin () const
{
  return base.pos;
}

const csVector3& celPcNewCamera::GetBaseDir () const
{
  return base.dir;
}

const csVector3& celPcNewCamera::GetBaseUp () const
{
  return base.up;
}

const csReversibleTransform& celPcNewCamera::GetBaseTrans () const
{
  return basetrans;
}

const csVector3& celPcNewCamera::GetPos () const
{
  return currcam.pos;
}

const csVector3& celPcNewCamera::GetOrigin () const
{
  return currcam.pos;
}

const csVector3& celPcNewCamera::GetTarget () const
{
  return currcam.dir;
}

const csVector3& celPcNewCamera::GetUp () const
{
  return currcam.up;
}

void celPcNewCamera::SetPositionOffset (const csVector3& offset)
{
  SetTargetPositionOffset (offset);
}

void celPcNewCamera::SetTargetMinimumOffset (float minoff)
{
  minoffset = minoff;
}

void celPcNewCamera::SetTargetPositionOffset (const csVector3& offset)
{
}
void celPcNewCamera::SetCameraPositionOffset (const csVector3& offset)
{
}

void celPcNewCamera::SetSpringCoefficient (float springCoef)
{
  originspring = springCoef;
}

float celPcNewCamera::GetSpringCoefficient () const
{
  return originspring;
}

void celPcNewCamera::SetOriginSpringCoefficient (float springCoef)
{
  originspring = springCoef;
}

float celPcNewCamera::GetOriginSpringCoefficient () const
{
  return originspring;
}

void celPcNewCamera::SetTargetSpringCoefficient (float springCoef)
{
  targetspring = springCoef;
}

float celPcNewCamera::GetTargetSpringCoefficient () const
{
  return targetspring;
}

void celPcNewCamera::SetUpSpringCoefficient (float springCoef)
{
  upspring = springCoef;
}

float celPcNewCamera::GetUpSpringCoefficient () const
{
  return upspring;
}

bool celPcNewCamera::DetectCollisions () const
{
  return docolldet;
}

void celPcNewCamera::SetCollisionDetection (bool docolldet)
{
  this->docolldet = docolldet;
}

bool celPcNewCamera::GetCollisionDetection () const
{
  return docolldet;
}

void celPcNewCamera::SetCollisionYFocusOffset (float yoff)
{
  collyfocusoff = yoff;
}
float celPcNewCamera::GetCollisionYFocusOffset () const
{
  return collyfocusoff;
}

bool celPcNewCamera::InCameraTransition () const
{
  return in_transition;
}

void celPcNewCamera::SetTransitionSpringCoefficient (float springCoef)
{
}
float celPcNewCamera::GetTransitionSpringCoefficient () const
{
  return 0.0f;
}
void celPcNewCamera::SetTransitionCutoffDistance (float cutOffOriginDist,
  float cutOffTargetDist)
{
}
float celPcNewCamera::GetTransitionCutoffPosDistance () const
{
  return 0.0f;
}
float celPcNewCamera::GetTransitionCutoffOriginDistance () const
{
  return 0.0f;
}
float celPcNewCamera::GetTransitionCutoffTargetDistance () const
{
  return 0.0f;
}

void celPcNewCamera::SetTransitionTime (float time)
{
  transtime = time;
}
float celPcNewCamera::GetTransitionTime () const
{
  return transtime;
}

size_t celPcNewCamera::AttachCameraMode (iCelCameraMode* mode)
{
  cameramodes.Push (mode);
  mode->SetParentCamera ((iPcNewCamera*)this);
  mode->DecRef ();
  size_t modeindex = cameramodes.GetSize () - 1;
  // if this is first camera mode to be attached, then update
  // the camera mode!
  if (currmode == (size_t)-1)
    currmode = modeindex;
  return modeindex;
}

size_t celPcNewCamera::AttachCameraMode (iPcNewCamera::CEL_CAMERA_MODE modetype)
{
  switch (modetype)
  {
    case iPcNewCamera::CCM_FIRST_PERSON:
      return AttachCameraMode (new celCameraMode::FirstPerson ());
    case iPcNewCamera::CCM_THIRD_PERSON:
      return AttachCameraMode (new celCameraMode::ThirdPerson ());
    case iPcNewCamera::CCM_TRACKING:
      return AttachCameraMode (new celCameraMode::Tracking (pl, vc));
    case iPcNewCamera::CCM_HORIZONTAL:
      return AttachCameraMode (new celCameraMode::Horizontal ());
    case iPcNewCamera::CCM_ISOMETRIC:
      return AttachCameraMode (new celCameraMode::Isometric ());
    default:
      return (size_t)-1;
  }
}

size_t celPcNewCamera::GetCurrentCameraModeIndex () const
{
  return currmode;
}

iCelCameraMode* celPcNewCamera::GetCurrentCameraMode ()
{
  return GetCameraMode ();
}

iCelCameraMode* celPcNewCamera::GetCameraMode (int idx)
{
  // get current available camera mode
  if (idx < 0)
  {
    // now camera modes attached
    if (cameramodes.IsEmpty ())
      return 0;
    else
    {
      // no mode is set OR mode is set fucked
      if (currmode >= cameramodes.GetSize ())
        return cameramodes.Top ();
      // return current camera mode
      return cameramodes[currmode];
    }
  }
  // when we cast, is it still valid? we didn't select invalid mode?
  if (static_cast<size_t> (idx) >= cameramodes.GetSize ())
    return 0;
  // just go...
  return cameramodes[idx];
}

bool celPcNewCamera::SetCurrentCameraMode (size_t modeindex)
{
  if (modeindex >= cameramodes.GetSize ())
    return false;

  in_transition = true;
  lastmode = currcam;
  currtrans = 0.0f;

  // show the actor when in transition
  if (pcmesh && pcmesh->GetMesh ())
    pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE, 0);

  currmode = modeindex;

  return true;
}

void celPcNewCamera::NextCameraMode ()
{
  if (cameramodes.GetSize () == 0)
    return;

  size_t newmode = currmode + 1;
  // loop round to first mode
  if (newmode >= cameramodes.GetSize ())
    newmode = 0;
  SetCurrentCameraMode (newmode);
}

void celPcNewCamera::PrevCameraMode ()
{
  if (cameramodes.GetSize () == 0)
    return;

  size_t newmode = currmode - 1;
  // loop back round to top mode
  if (newmode == (size_t)-1)
    newmode = cameramodes.GetSize () - 1;
  SetCurrentCameraMode (newmode);
}

void celPcNewCamera::UpdateCamera ()
{
  if (!init_reset)
    init_reset = Reset ();

  float elapsedsecs = vc->GetElapsedTicks () / 1000.0f;

  // Try to get position and sector from the mesh.
  if (pcmesh)
  {
    iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
    basetrans = movable->GetFullTransform ();
    basesector = movable->GetSectors ()->Get (0);
  }
  else
  {
    basetrans.SetT2O (csMatrix3 ());
    basesector = 0;
    return;
  }

  base.pos = basetrans.GetOrigin ();
  base.dir = basetrans.This2OtherRelative (csVector3 (0.0f, 0.0f, -1.0f));
  base.up  = basetrans.This2OtherRelative (csVector3 (0.0f, 1.0f, 0.0f));

  iCelCameraMode* mode = GetCameraMode ();
  // basic camera mode in case of no modes attached
  if (!mode)
  {
    if (pcmesh)
    {
      // so we just align the camera's pos and dir with
      // the attached mesh
      csReversibleTransform camTrans;
      camTrans.SetOrigin (basetrans.GetOrigin ());
      camTrans.LookAt (base.dir, base.up);
      iCamera* c = view->GetCamera ();
      // needs to be in the right sector
      if (c->GetSector () != basesector)
        c->SetSector (basesector);
      c->SetTransform (camTrans);
    }
    return;
  }

  if (!mode->DecideCameraState ())
    return;

  float springpos = mode->GetOriginSpringCoefficient (),
    springdir = mode->GetTargetSpringCoefficient (),
    springup = mode->GetUpSpringCoefficient ();

  // find the desired position that the mode would like us to reach
  CameraState desired;
  desired.pos = mode->GetOrigin ();
  desired.dir = mode->GetTarget () - mode->GetOrigin ();
  desired.up = mode->GetUp ();
  // make sure that what we interpolate is actually sane
  currcam.dir.Normalize ();
  desired.dir.Normalize ();

  // perform a transition to new mode from old position if need be
  if (in_transition)
  {
    // update counter
    currtrans += elapsedsecs / transtime;
    if (currtrans > 1.0f)
    {
      // if going to or from a mode that made player mesh invisible
      UpdateMeshVisibility ();
      // switch transition off
      in_transition = false;
      // and clip this for usage below
      currtrans = 1.0f;
    }
    // actually do the interpolation
    currcam.pos = InterpolateVector (currtrans, lastmode.pos, desired.pos);
    currcam.dir = InterpolateVector (currtrans, lastmode.dir, desired.dir);
    currcam.up =  InterpolateVector (currtrans, lastmode.up, desired.up);
  }
  else
  {
    // interpolate the various vectors if the mode tells us to
    // ... otherwise just directly use the values
    if (mode->UseSpringOrigin ())
      currcam.pos = InterpolateVector (springpos * elapsedsecs, currcam.pos, desired.pos);
    else
      currcam.pos = desired.pos;

    if (mode->UseSpringTarget ())
      currcam.dir = InterpolateVector (springdir * elapsedsecs, currcam.dir, desired.dir);
    else
      currcam.dir = desired.dir;

    if (mode->UseSpringUp ())
      currcam.up =  InterpolateVector (springup * elapsedsecs, currcam.up, desired.up);
    else
      currcam.up = desired.up;
  }

  // adjust for offset allowed to character
  if (currcam.dir.SquaredNorm () < minoffset * minoffset)
  {
    // fix it at the minimum distance now
    // project from player position to camera beam from camera position
    csVector3 projpos = ((base.pos - currcam.pos) >> currcam.dir) + currcam.pos;
    // now move backwards from projected position along camera direction
    currcam.pos = projpos - currcam.dir * minoffset;
  }

  // find corrected origin for collision detection
  csVector3 corrorigin (currcam.pos);
  if (GetCollisionDetection () && mode->AllowCollisionDetection ())
  {
    // project beam from bottom of character
    // offset upwards, so we don't zoom to feet
    base.pos += base.up * collyfocusoff;
    csTraceBeamResult beam = csColliderHelper::TraceBeam (cdsys, basesector,
      currcam.pos, base.pos, true);
    // collision!
    if (beam.sqdistance > 0)
    {
      // compute unit direction vector of beam
      csVector3 dir (base.pos - currcam.pos);
      dir.Normalize ();
      // now compute the corrected origin lying on the beam
      corrorigin = dir * sqrt(beam.sqdistance) + currcam.pos;
    }
  }

  // Adjust camera transform for relative position and lookat position.
  csReversibleTransform camTrans;
  camTrans.SetOrigin (corrorigin);
  camTrans.LookAt (currcam.dir, currcam.up);

  iCamera* c = view->GetCamera ();
  // First set the camera back on where the sector is.
  // We assume here that normal camera movement is good.
  if (c->GetSector () != basesector)
    c->SetSector (basesector);
  c->SetTransform (camTrans);
  c->OnlyPortals (true);
}

int celPcNewCamera::GetDrawFlags ()
{
  return engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS
    | CSDRAW_CLEARZBUFFER;
}

void celPcNewCamera::Draw ()
{
  UpdateCamera ();
  // Tell 3D driver we're going to display 3D things.
  if (g3d->BeginDraw (GetDrawFlags ()))
    view->Draw ();
}

void celPcNewCamera::TickEveryFrame ()
{
  Draw ();
}

const csOrthoTransform& celPcNewCamera::GetTransform ()
{
  return view->GetCamera ()->GetTransform ();
}
