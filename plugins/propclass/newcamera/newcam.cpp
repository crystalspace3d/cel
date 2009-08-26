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
#include "iengine/collection.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/csview.h"
#include "cstool/collider.h"
#include "csgeom/poly3d.h"
#include "csgeom/transfrm.h"
#include "csgeom/sphere.h"
#include "ivaria/view.h"
#include "ivaria/collider.h"
#include "ivaria/reporter.h"
#include "ivideo/graph3d.h"
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

  if (currMode >= cameraModes.GetSize ())
    return;

  if (!pcmesh->GetMesh ())
    return;

  if (cameraModes[currMode]->DrawAttachedMesh ())
    pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE, 0);
  else
    pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE,
      CS_ENTITY_INVISIBLE);
}

static void CalcElasticVec (csVector3& curr, const csVector3& ideal,
    float deltaTime, float springCoef)
{
  if (deltaTime > (1.0f / springCoef))
    deltaTime = (1.0f / springCoef);
  curr = curr - ((curr - ideal) * deltaTime * springCoef);
}

static csVector3 InterpolateVector (float i, const csVector3& curr, const csVector3& next)
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

  offsetOrigin.Set (0.0f, 0.0f, 0.0f);
  offsetTarget.Set (0.0f, 1.7f, 0.0f);

  currMode = (size_t)-1;

  docolldet = true;
  collyfocusoff = 1.5f;
  corrmult = 1.5f;
  avoidradsq = 4.0f;
  avoidyoff = 2.0f;
  avoidinter = 3.0f;

  originSpringCoef = 2.0;
  targetSpringCoef = 2.0;
  upSpringCoef = 2.0;

  inTransition = true;
  transitionSpringCoef = 2.0f;
  transitionCutoffOriginDist = 1.0f;
  transitionCutoffTargetDist = 1.0f;
  usecrappyspring = false;
  transtime = 0.5f;
  currtrans = 0.0f;

  if (id_name == csInvalidStringID)
  {
    id_name = pl->FetchStringID ("name");
    id_nr = pl->FetchStringID ("nr");
    id_x = pl->FetchStringID ("x");
    id_y = pl->FetchStringID ("y");
    id_w = pl->FetchStringID ("w");
    id_h = pl->FetchStringID ("h");
    id_enable = pl->FetchStringID ("enable");
    id_minfps = pl->FetchStringID ("min_fps");
    id_maxfps = pl->FetchStringID ("max_fps");
    id_mindist = pl->FetchStringID ("min_distance");
    id_dist = pl->FetchStringID ("distance");
  }
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_name);
  params->SetParameterDef (id_nr);

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.camera.new.action.");
    AddAction (action_attachcameramode, "AttachCameraMode");
    AddAction (action_setcameramode, "SetCameraMode");
    AddAction (action_nextcameramode, "NextCameraMode");
    AddAction (action_prevcameramode, "PrevCameraMode");
    AddAction (action_setrectangle, "SetRectangle");
    AddAction (action_setperspcenter, "SetPerspectiveCenter");
    AddAction (action_adaptiveclipping, "AdaptiveDistanceClipping");
    AddAction (action_fixedclipping, "FixedDistanceClipping");
  }

  propinfo.SetCount (22);
  AddProperty (propid_colldet, "colldet",
    CEL_DATA_BOOL, false, "Whether camera will use collision detection.", &docolldet);
  AddProperty (propid_colldet_yfocusoff, "yfocusoff",
    CEL_DATA_FLOAT, false, "Y offset from player position for collision detection focus.", &collyfocusoff);
  AddProperty (propid_colldet_corrmult, "corrmult",
    CEL_DATA_FLOAT, false, "Correction to add to camera after collision.", &corrmult);
  AddProperty (propid_colldet_avoid_radsq, "avoid_radsq",
    CEL_DATA_FLOAT, false, "Player avoidance squared radius.", &avoidradsq);
  AddProperty (propid_colldet_avoid_yoff, "avoid_yoff",
    CEL_DATA_FLOAT, false, "Y offset for avoiding the player.", &avoidyoff);
  AddProperty (propid_colldet_avoid_inter, "avoid_inter",
    CEL_DATA_FLOAT, false, "Interpolation between normal and Y offsetted height.", &avoidinter);
  AddProperty (propid_colldet_spring, "colldet_spring",
    CEL_DATA_FLOAT, false,
    "DEPRECATED: Springiness in case of collision.",
    0);
  AddProperty (propid_colldet_origin_radius,
    "colldet_origin_radius",
    CEL_DATA_FLOAT, false,
    "DEPRECATED: Space between wall and camera origin in case of collision.",
    0);
  AddProperty (propid_colldet_target_radius,
    "colldet_target_radius",
    CEL_DATA_FLOAT, false,
    "DEPRECATED: Offset for target collision detection.",
    0);
  AddProperty (propid_offset, "offset",
    CEL_DATA_VECTOR3, false, "Offset from the center of the mesh.",
    &offsetTarget);
  AddProperty (propid_offset_origin, "offset_origin",
    CEL_DATA_VECTOR3, false,
    "Offset of the camera origin point from the center of the mesh.",
    &offsetOrigin);
  AddProperty (propid_offset_target, "offset_target",
    CEL_DATA_VECTOR3, false,
    "Offset of the target point from the center of the mesh.",
    &offsetTarget);
  AddProperty (propid_offset_min, "minoffset",
    CEL_DATA_FLOAT, false, "Minimum distance between camera and player on X-Z plane.", &minoffset);
  AddProperty (propid_spring, "spring",
    CEL_DATA_FLOAT, false, "Common spring coefficient.", 0);
  AddProperty (propid_spring_origin, "spring_origin",
    CEL_DATA_FLOAT, false, "Spring coefficient for origin.",
    &originSpringCoef);
  AddProperty (propid_spring_target, "spring_target",
    CEL_DATA_FLOAT, false, "Spring coefficient for target.",
    &targetSpringCoef);
  AddProperty (propid_spring_up, "spring_up",
    CEL_DATA_FLOAT, false, "Spring coefficient for up vector.",
    &upSpringCoef);
  AddProperty (propid_trans_spring, "transition_spring",
    CEL_DATA_FLOAT, false,
    "DEPRECATED: Springyness of the transition to a new camera mode.",
    &transitionSpringCoef);
  AddProperty (propid_trans_cutofforigin,
    "transition_cutofforigin",
    CEL_DATA_FLOAT, false,
    "DEPRECATED: Camera transition mode cutoff distance from origin to origin.",
    &transitionCutoffOriginDist);
  AddProperty (propid_trans_cutofftarget,
    "transition_cutofftarget",
    CEL_DATA_FLOAT, false,
    "DEPRECATED: Camera transition mode cutoff distance from target to target.",
    &transitionCutoffTargetDist);
  AddProperty (propid_trans_time, "trans_time",
    CEL_DATA_FLOAT, false, "Time to transition to the new mode.", &transtime);
  AddProperty (propid_trans_on, "trans_on",
    CEL_DATA_BOOL, true, "Whether currently in a transition.", &inTransition);
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
        originSpringCoef = val;
        targetSpringCoef = val;
        upSpringCoef = val;
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
        val = originSpringCoef;
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
        val = originSpringCoef;
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
    camOrigin = movable->GetTransform ().GetOrigin ();
    camTarget = movable->GetTransform ().
      This2OtherRelative (csVector3 (0.0f, 0.0f, -1.0f));
    camUp  = movable->GetTransform ().
      This2OtherRelative (csVector3 (0.0f, 1.0f, 0.0f));
  }

  UpdateMeshVisibility ();
  return true;
}

const csVector3& celPcNewCamera::GetBasePos () const
{
  return baseOrigin;
}

const csVector3& celPcNewCamera::GetBaseOrigin () const
{
  return baseOrigin;
}

const csVector3& celPcNewCamera::GetBaseDir () const
{
  return baseDir;
}

const csVector3& celPcNewCamera::GetBaseUp () const
{
  return baseUp;
}

const csReversibleTransform& celPcNewCamera::GetBaseTrans () const
{
  return baseTrans;
}

const csVector3& celPcNewCamera::GetPos () const
{
  return camOrigin;
}

const csVector3& celPcNewCamera::GetOrigin () const
{
  return camOrigin;
}

const csVector3& celPcNewCamera::GetTarget () const
{
  return camTarget;
}

const csVector3& celPcNewCamera::GetUp () const
{
  return camUp;
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
  offsetTarget = offset;
}

void celPcNewCamera::SetCameraPositionOffset (const csVector3& offset)
{
  offsetOrigin = offset;
}

void celPcNewCamera::SetSpringCoefficient (float springCoef)
{
  originSpringCoef = springCoef;
}

float celPcNewCamera::GetSpringCoefficient () const
{
  return originSpringCoef;
}

void celPcNewCamera::SetOriginSpringCoefficient (float springCoef)
{
  originSpringCoef = springCoef;
}

float celPcNewCamera::GetOriginSpringCoefficient () const
{
  return originSpringCoef;
}

void celPcNewCamera::SetTargetSpringCoefficient (float springCoef)
{
  targetSpringCoef = springCoef;
}

float celPcNewCamera::GetTargetSpringCoefficient () const
{
  return targetSpringCoef;
}

void celPcNewCamera::SetUpSpringCoefficient (float springCoef)
{
  upSpringCoef = springCoef;
}

float celPcNewCamera::GetUpSpringCoefficient () const
{
  return upSpringCoef;
}

bool celPcNewCamera::DetectCollisions () const
{
  return docolldet;
}

void celPcNewCamera::SetCollisionDetection (bool detectCollisions)
{
  docolldet = detectCollisions;
}

bool celPcNewCamera::GetCollisionDetection () const
{
  return docolldet;
}

void celPcNewCamera::SetCollisionSpringCoefficient (float springCoef)
{
}

float celPcNewCamera::GetCollisionSpringCoefficient () const
{
  return 0.0f;
}

void celPcNewCamera::SetCollisionYFocusOffset (float yoff)
{
  collyfocusoff = yoff;
}
float celPcNewCamera::GetCollisionYFocusOffset () const
{
  return collyfocusoff;
}

void celPcNewCamera::SetCollisionCorrection (float corr)
{
  corrmult = corr;
}
float celPcNewCamera::GetCollisionCorrection () const
{
  return corrmult;
}

void celPcNewCamera::SetCollisionAvoidanceRadiusSq (float radsq)
{
  avoidradsq = (radsq < EPSILON) ? EPSILON : radsq;
}
float celPcNewCamera::GetCollisionAvoidanceRadiusSq () const
{
  return avoidradsq;
}

void celPcNewCamera::SetCollisionYAvoidance (float yavoid)
{
  avoidyoff = yavoid;
}
float celPcNewCamera::GetCollisionYAvoidance () const
{
  return avoidyoff;
}

void celPcNewCamera::SetCollisionAvoidanceInterpolation (float aint)
{
  avoidinter = (aint < EPSILON) ? EPSILON : aint;
}
float celPcNewCamera::GetCollisionAvoidanceInterpolation () const
{
  return avoidinter;
}

bool celPcNewCamera::InCameraTransition () const
{
  return inTransition;
}

void celPcNewCamera::SetTransitionSpringCoefficient (float springCoef)
{
  usecrappyspring = true;
  transitionSpringCoef = springCoef;
}

float celPcNewCamera::GetTransitionSpringCoefficient () const
{
  return transitionSpringCoef;
}

void celPcNewCamera::SetTransitionCutoffDistance (float cutOffOriginDist,
  float cutOffTargetDist)
{
  usecrappyspring = true;
  transitionCutoffOriginDist = cutOffOriginDist;
  transitionCutoffTargetDist = cutOffTargetDist;
}

float celPcNewCamera::GetTransitionCutoffPosDistance () const
{
  return transitionCutoffOriginDist;
}

float celPcNewCamera::GetTransitionCutoffOriginDistance () const
{
  return transitionCutoffOriginDist;
}

float celPcNewCamera::GetTransitionCutoffTargetDistance () const
{
  return transitionCutoffTargetDist;
}

void celPcNewCamera::SetTransitionTime (float time)
{
  usecrappyspring = false;
  transtime = time;
}
float celPcNewCamera::GetTransitionTime () const
{
  return transtime;
}

size_t celPcNewCamera::AttachCameraMode (iCelCameraMode* mode)
{
  cameraModes.Push (mode);
  mode->SetParentCamera ((iPcNewCamera*)this);
  mode->DecRef ();

  return (cameraModes.GetSize () - 1);
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
      return AttachCameraMode (new celCameraMode::Tracking (pl));
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
  return currMode;
}

iCelCameraMode* celPcNewCamera::GetCurrentCameraMode ()
{
  if (cameraModes.IsEmpty ())
    return 0;
  if (currMode >= cameraModes.GetSize ())
    return cameraModes.Top ();
  return cameraModes[currMode];
}

iCelCameraMode* celPcNewCamera::GetCameraMode (int idx)
{
  if (idx < 0)
    return GetCurrentCameraMode ();
  if (static_cast<size_t> (idx) >= cameraModes.GetSize ())
    return 0;
  return cameraModes[idx];
}

bool celPcNewCamera::SetCurrentCameraMode (size_t modeIndex)
{
  if (modeIndex >= cameraModes.GetSize ())
    return false;

  inTransition = true;

  // show the actor when in transition
  if (pcmesh && pcmesh->GetMesh ())
    pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE, 0);

  currMode = modeIndex;

  return true;
}

void celPcNewCamera::NextCameraMode ()
{
  if (cameraModes.GetSize () == 0)
    return;

  size_t newMode = currMode + 1;
  if (newMode >= cameraModes.GetSize ())
    newMode = 0;
  SetCurrentCameraMode (newMode);
}

void celPcNewCamera::PrevCameraMode ()
{
  if (cameraModes.GetSize () == 0)
    return;

  size_t newMode = currMode - 1;
  if (newMode == (size_t)-1)
    newMode = cameraModes.GetSize () - 1;
  SetCurrentCameraMode (newMode);
}

void celPcNewCamera::UpdateCamera ()
{
  if (!init_reset)
    init_reset = Reset ();

  csTicks elapsedTime = vc->GetElapsedTicks ();
  float elapsedSecs = elapsedTime / 1000.0f;

  // Try to get position and sector from the mesh.
  if (pcmesh)
  {
    iMeshWrapper* mesh = pcmesh->GetMesh ();
    iMovable* movable = mesh->GetMovable ();
    baseTrans = movable->GetFullTransform ();
    baseSector = movable->GetSectors ()->Get (0);
  }
  else
  {
    baseTrans.SetT2O (csMatrix3 ());
    baseSector = 0;
    return;
  }

  baseOrigin = baseTrans.GetOrigin ()
    + baseTrans.This2OtherRelative (offsetTarget);
  baseDir = baseTrans.This2OtherRelative (csVector3 (0.0f, 0.0f, -1.0f));
  baseUp  = baseTrans.This2OtherRelative (csVector3 (0.0f, 1.0f, 0.0f));

  // basic camera mode in case of no modes attached
  if (currMode >= cameraModes.GetSize ())
  {
    SetCurrentCameraMode (cameraModes.GetSize () - 1);
    if (currMode >= cameraModes.GetSize ())
    {
      if (pcmesh)
      {
        // so we just align the camera's pos and dir with
        // the attached mesh
        csReversibleTransform camTrans;
        camTrans.SetOrigin (baseTrans.GetOrigin ());
        camTrans.LookAt (baseDir, baseUp);
        iCamera* c = view->GetCamera ();
        // needs to be in the right sector
        if (c->GetSector () != baseSector)
          c->SetSector (baseSector);
        c->SetTransform (camTrans);
      }
      return;
    }
  }

  iCelCameraMode* mode = cameraModes[currMode];
  if (!mode->DecideCameraState ())
    return;

  csVector3 desiredOrigin = mode->GetOrigin ();
  csVector3 desiredTarget = mode->GetTarget ();
  csVector3 desiredUp = mode->GetUp ();

  // if we're in a transition, then use the transition spring settings
  if (inTransition)
  {
    if (usecrappyspring)
    {
      CalcElasticVec (camOrigin, desiredOrigin, elapsedSecs, transitionSpringCoef);
      CalcElasticVec (camTarget, desiredTarget, elapsedSecs, transitionSpringCoef);
      CalcElasticVec (camUp, desiredUp, elapsedSecs, transitionSpringCoef);
    }
    else
    {
      currtrans += elapsedSecs / transtime;
      if (currtrans > 1.0f)
      {
        // if going to or from a mode that made player mesh invisible
        UpdateMeshVisibility ();
        // switch transition off
        inTransition = false;
        // and clip this for usage below
        currtrans = 1.0f;
      }

      camOrigin = InterpolateVector (currtrans, lastmode.pos, desiredOrigin);
      camTarget = InterpolateVector (currtrans, lastmode.tar, desiredTarget);
      camUp = InterpolateVector (currtrans, lastmode.up, desiredUp);
    }
  }
  else
  {
    float modeOriginSpringCoef = mode->GetOriginSpringCoefficient ();
    float modeTargetSpringCoef = mode->GetTargetSpringCoefficient ();
    float modeUpSpringCoef = mode->GetUpSpringCoefficient ();

    if (mode->UseSpringOrigin ())
      CalcElasticVec (camOrigin, desiredOrigin, elapsedSecs, modeOriginSpringCoef);
    else
      camOrigin = desiredOrigin;

    if (mode->UseSpringTarget ())
      CalcElasticVec (camTarget, desiredTarget, elapsedSecs, modeTargetSpringCoef);
    else
      camTarget = mode->GetTarget ();

    if (mode->UseSpringUp ())
      CalcElasticVec (camUp, desiredUp, elapsedSecs, modeUpSpringCoef);
    else
      camUp = mode->GetUp ();
  }

  // build the desired transform so that we can calculate the desired
  // offset that we want
  csReversibleTransform desired_camtrans;
  desired_camtrans.SetOrigin (desiredOrigin);
  desired_camtrans.LookAt (desiredTarget - desiredOrigin,
    csVector3 (0.0f, 1.0f, 0.0f));
  // then apply the desired offset to the desired position
  desiredOrigin += desired_camtrans.This2OtherRelative (offsetOrigin);

  csVector3 corrorigin (camOrigin), corrtar (camTarget);
  // perform collision detection
  if (GetCollisionDetection () && mode->AllowCollisionDetection ())
  {
    // project beam from bottom of character
    // offset upwards, so we don't zoom to feet
    csVector3 focalpos (baseOrigin);
    focalpos += baseUp * collyfocusoff;
    // turn off hitbeam for player so we don't hit player mesh!
    bool mesh_hitset = false;
    if (pcmesh)
    {
      csFlags &mflags = pcmesh->GetMesh ()->GetFlags ();
      if (!mflags.Check (CS_ENTITY_NOHITBEAM))
        mesh_hitset = true;
      mflags.Set (CS_ENTITY_NOHITBEAM);
    }
    const csTraceBeamResult beam = csColliderHelper::TraceBeam (cdsys, baseSector,
      camOrigin, focalpos, true);
    // ...and reset the flag again if it wasn't set!
    if (mesh_hitset)
      pcmesh->GetMesh ()->GetFlags ().Set (CS_ENTITY_NOHITBEAM, 0);
    // collision!
    if (beam.sqdistance > 0)
    {
      // compute direction vector of beam
      const csVector3 lookat (focalpos - camOrigin), dir (lookat.Unit ());
      float lookat_len = lookat.Norm ();
      // so we offset a proportional amount down the beam towards the player so as not to be
      // inside the wall.
      corrorigin = beam.closest_isect + 0.1f * (lookat_len - sqrt (beam.sqdistance)) * dir;

      // calculate flat distance to player
      csVector3 flatdiff (baseOrigin - corrorigin);
      flatdiff.y = 0.0f;
      const float flatdistsq = flatdiff.SquaredNorm ();
      // so when camera reaches collision avoidance radius bring camera slowly up above player
      if (flatdistsq < avoidradsq)
      {
        // difference between desired y position and curr y pos, with falloff as you get away from player to avoidradsq
        corrorigin.y += (baseOrigin.y + avoidyoff - corrorigin.y) * (1.0f - flatdistsq / avoidradsq);
      }
      // same as before, except that we exaggerate it a bit, to make it smoother (avoidance interpolation)
      float falloff = flatdistsq / (avoidradsq * avoidinter);
      if (falloff > 1.0f)
        falloff = 1.0f;
      // recorrect the camera's lookat now to reflect our changes
      // ... a bit of interpolation so the camera doesn't jump when this piece of code executes.
      corrtar = corrorigin + falloff * (camTarget - camOrigin) + (1.0f - falloff) * (focalpos - corrorigin);
    }
  }

  // Adjust camera transform for relative position and lookat position.
  csReversibleTransform camTrans;
  camTrans.SetOrigin (corrorigin);
  camTrans.LookAt (corrtar - corrorigin, camUp);

  iCamera* c = view->GetCamera ();
  // First set the camera back on where the sector is.
  // We assume here that normal camera movement is good.
  if (c->GetSector () != baseSector)
    c->SetSector (baseSector);
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
