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

void ObtainPolarCoords (csVector2 &polar, const csVector3 &cart)
{
  const float x = cart.x, y = cart.z,
    r = sqrt (x*x + y*y), theta = atan2 (y, x);
  polar.x = r;
  polar.y = theta;
}

void ObtainSpherical (csVector3 &spher, const csVector3 &cart)
{
  const float r = cart.Norm (),
    theta = acos (cart.z / r), phi = atan2 (cart.y, cart.x);
  spher.x = r;
  spher.y = theta;
  spher.z = phi;
}

void InterpolateVecPolar (csVector3 &curr, const csVector3 &next,
    float time, float spring)
{
  if (time > (1.0f / spring))
    time = (1.0f / spring);
  const float i = time * spring;
  // no interpolation
  curr = next;
  // linear interpolation
  //curr = (next - curr) * i + curr;
  // polar coordinate interpolation with linear y interpolation
  /*csVector2 polcurr, polnext;
  ObtainPolarCoords (polcurr, curr);
  ObtainPolarCoords (polnext, next);
  polcurr = (polnext - polcurr) * i + polcurr;
  float inter_y = (next.y - curr.y) * i + curr.y;

  curr.x = polcurr.x * cos (polcurr.y);
  curr.y = inter_y;
  curr.z = polcurr.x * sin (polcurr.y);*/
  // spherical coordinate interpolation
  /*csVector3 sphcurr, sphnext;
  ObtainSpherical (sphcurr, curr);
  ObtainSpherical (sphnext, next);
  sphcurr = (sphnext - sphcurr) * i + sphcurr;
  curr.x = sphcurr.x * sin (sphcurr.y) * cos (sphcurr.z);
  curr.y = sphcurr.x * sin (sphcurr.y) * sin (sphcurr.z);
  curr.z = sphcurr.x * cos (sphcurr.y);*/
}
void celPcNewCamera::CalcElasticVec (csVector3& curr, const csVector3& ideal,
    float deltaTime, float springCoef)
{
  if (deltaTime > (1.0f / springCoef))
    deltaTime = (1.0f / springCoef);
  curr = curr - ((curr - ideal) * deltaTime * springCoef);
/*
  csVector3 deltaVec;

  deltaVec = curr - ideal;
  if (deltaVec.SquaredNorm () <= 0.001f)
  {
    newVec = curr;
    return;
  }

  float len = deltaVec.Norm ();
#if 0
  csVector3 vel = deltaIdeal * deltaTime;
  float force = springCoef * -len + 0.25f * (deltaVec * vel) / len;
  newVec = curr + deltaVec / len * force * deltaTime;
#else
  deltaVec *= springCoef * len * deltaTime;
  if (deltaVec.SquaredNorm() >= len * len)
    newVec = curr;
  else
    newVec = curr - deltaVec;
#endif*/
}

celPcNewCamera::celPcNewCamera (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);

  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_VIEW);

  init_reset = false;

  offsetTarget.Set (0.0f, 0.0f, 0.0f);
  offsetOrigin.Set (0.0f, 0.0f, 0.0f);
  minoffset = 6.0f;

  currMode = (size_t)-1;

  detectCollisions = false;
  collisionSpringCoef = 3.0f;
  collisionOriginRadius = 1.0f;
  collisionTargetRadius = 2.0f;

  inTransition = true;
  transitionSpringCoef = 2.0f;
  transitionCutoffOriginDist = 1.0f;
  transitionCutoffTargetDist = 1.0f;


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

  propinfo.SetCount (14);
  AddProperty (propid_colldet, "cel.property.colldet",
  	CEL_DATA_BOOL, false, "Camera will use collision detection.", 0);
  AddProperty (propid_colldet_spring, "cel.property.colldet_spring",
  	CEL_DATA_FLOAT, false,
  	"Springyness in case of collision.",
  	&collisionSpringCoef);
  AddProperty (propid_colldet_origin_radius,
  	"cel.property.colldet_origin_radius",
  	CEL_DATA_FLOAT, false,
  	"Space between wall and camera origin in case of collision.",
  	&collisionOriginRadius);
  AddProperty (propid_colldet_target_radius,
  	"cel.property.colldet_target_radius",
  	CEL_DATA_FLOAT, false,
  	"Offset for target collision detection.",
  	&collisionTargetRadius);
  AddProperty (propid_offset, "cel.property.offset",
  	CEL_DATA_VECTOR3, false, "Offset from the center of the mesh.",
  	&offsetTarget);
  AddProperty (propid_offset_origin, "cel.property.offset_origin",
  	CEL_DATA_VECTOR3, false,
  	"Offset of the camera origin point from the center of the mesh.",
  	&offsetOrigin);
  AddProperty (propid_offset_target, "cel.property.offset_target",
  	CEL_DATA_VECTOR3, false,
  	"Offset of the target point from the center of the mesh.",
  	&offsetTarget);
  AddProperty (propid_spring, "cel.property.spring",
  	CEL_DATA_FLOAT, false, "Common spring coefficient.", 0);
  AddProperty (propid_spring_origin, "cel.property.spring_origin",
  	CEL_DATA_FLOAT, false, "Spring coefficient for origin.",
  	&originSpringCoef);
  AddProperty (propid_spring_target, "cel.property.spring_target",
  	CEL_DATA_FLOAT, false, "Spring coefficient for target.",
  	&targetSpringCoef);
  AddProperty (propid_spring_up, "cel.property.spring_up",
  	CEL_DATA_FLOAT, false, "Spring coefficient for up vector.",
  	&upSpringCoef);
  AddProperty (propid_trans_spring, "cel.property.transition_spring",
  	CEL_DATA_FLOAT, false,
  	"Springyness of the transition to a new camera mode.",
  	&transitionSpringCoef);
  AddProperty (propid_trans_cutofforigin,
  	"cel.property.transition_cutofforigin",
  	CEL_DATA_FLOAT, false,
  	"Camera transition mode cutoff distance from origin to origin.",
  	&transitionCutoffOriginDist);
  AddProperty (propid_trans_cutofftarget,
  	"cel.property.transition_cutofftarget",
  	CEL_DATA_FLOAT, false,
  	"Camera transition mode cutoff distance from target to target.",
  	&transitionCutoffTargetDist);
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
  return detectCollisions;
}

void celPcNewCamera::SetCollisionDetection (bool detectCollisions)
{
  this->detectCollisions = detectCollisions;
}

bool celPcNewCamera::GetCollisionDetection () const
{
  return detectCollisions;
}

void celPcNewCamera::SetCollisionSpringCoefficient (float springCoef)
{
  collisionSpringCoef = springCoef;
}

float celPcNewCamera::GetCollisionSpringCoefficient () const
{
  return collisionSpringCoef;
}

bool celPcNewCamera::InCameraTransition () const
{
  return inTransition;
}

void celPcNewCamera::SetTransitionSpringCoefficient (float springCoef)
{
  transitionSpringCoef = springCoef;
}

float celPcNewCamera::GetTransitionSpringCoefficient () const
{
  return transitionSpringCoef;
}

void celPcNewCamera::SetTransitionCutoffDistance (float cutOffOriginDist,
	float cutOffTargetDist)
{
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
  return currMode;
}

iCelCameraMode* celPcNewCamera::GetCurrentCameraMode ()
{
  return cameraModes.Top ();
}

iCelCameraMode* celPcNewCamera::GetCameraMode (int idx)
{
  if (idx < 0)
    return cameraModes.GetSize () > 0 ? cameraModes.Top () : 0;
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

  float modeOriginSpringCoef = mode->GetOriginSpringCoefficient ();
  float modeTargetSpringCoef = mode->GetTargetSpringCoefficient ();
  float modeUpSpringCoef = mode->GetUpSpringCoefficient ();

  // if we're in a transition, then use the transition spring settings
  if (inTransition)
  {
    modeOriginSpringCoef = transitionSpringCoef;
    modeTargetSpringCoef = transitionSpringCoef;
    modeUpSpringCoef = transitionSpringCoef;
  }

  csVector3 desiredOrigin = mode->GetOrigin ();
  csVector3 desiredTarget = mode->GetTarget ();
  csVector3 desiredUp = mode->GetUp ();

  // build the desired transform so that we can calculate the desired
  // offset that we want
  csReversibleTransform desired_camtrans;
  desired_camtrans.SetOrigin (desiredOrigin);
  desired_camtrans.LookAt (desiredTarget - desiredOrigin,
  	csVector3 (0.0f, 1.0f, 0.0f));
  // then apply the desired offset to the desired position
  desiredOrigin += desired_camtrans.This2OtherRelative (offsetOrigin);

  // perform collision detection
  if (GetCollisionDetection () && mode->AllowCollisionDetection ())
  {
    csVector3 beamDirection = desiredTarget - desiredOrigin;
    beamDirection.Normalize ();
    csTraceBeamResult beam = csColliderHelper::TraceBeam (cdsys, baseSector,
    	desiredTarget + (beamDirection * collisionTargetRadius), desiredOrigin, true);
    if (beam.closest_mesh)
    {
      csPoly3D tri;
      tri.AddVertex (beam.closest_tri.a);
      tri.AddVertex (beam.closest_tri.b);
      tri.AddVertex (beam.closest_tri.c);
      desiredOrigin = beam.closest_isect - (tri.ComputeNormal () * collisionOriginRadius);
      desiredTarget =  desiredOrigin + beamDirection;
      modeOriginSpringCoef = collisionSpringCoef;
      modeTargetSpringCoef = collisionSpringCoef;
      modeUpSpringCoef = collisionSpringCoef;
    }
  }

  if (inTransition || mode->UseSpringOrigin ())
    CalcElasticVec (camOrigin, desiredOrigin, elapsedSecs, modeOriginSpringCoef);
    //InterpolateVecPolar (camOrigin, desiredOrigin, elapsedSecs, modeOriginSpringCoef);
  else
    camOrigin = desiredOrigin;

  if (inTransition || mode->UseSpringTarget ())
    CalcElasticVec (camTarget, desiredTarget, elapsedSecs,
    	modeTargetSpringCoef);
  else
    camTarget = mode->GetTarget ();

  if (inTransition || mode->UseSpringUp ())
    CalcElasticVec (camUp, desiredUp, elapsedSecs, modeUpSpringCoef);
  else
    camUp = mode->GetUp ();

  // if we're in a transition then see if this latest camera movement
  // has push us into the next camera mode
  if (inTransition
  	&& (camOrigin - desiredOrigin).SquaredNorm ()
  	<= transitionCutoffOriginDist * transitionCutoffOriginDist
  	&& (camTarget - mode->GetTarget ()).SquaredNorm ()
  	<= transitionCutoffTargetDist * transitionCutoffTargetDist)
  {
    UpdateMeshVisibility ();
    inTransition = false;
  }

  // adjust for offset allowed to character
  csVector3 offset (camOrigin - camTarget);
  if (offset.SquaredNorm () < minoffset * minoffset)
  {
    // fix it at the minimum distance now
    offset.Normalize ();
    camOrigin = offset * minoffset + camTarget;
  }

  // Adjust camera transform for relative position and lookat position.
  csReversibleTransform camTrans;
  camTrans.SetOrigin (camOrigin);
  camTrans.LookAt (camTarget - camOrigin, camUp);

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
