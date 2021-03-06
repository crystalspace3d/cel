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

#include "plugins/propclass/hover/hover.h"

#include "propclass/mechsys.h"
#include "propclass/mesh.h"

#include "csgeom/box.h"
#include "csgeom/math3d.h"
#include "iengine/camera.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "imesh/object.h"
#include "imesh/objmodel.h"
#include "ivaria/dynamics.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (Hover, "pcvehicle.hover", "pchover")

// Parameters.
csStringID celPcHover::param_hbeamcutoff = csInvalidStringID;
csStringID celPcHover::param_angoff = csInvalidStringID;
csStringID celPcHover::param_angheight = csInvalidStringID;
csStringID celPcHover::param_angstr = csInvalidStringID;
csStringID celPcHover::param_hover = csInvalidStringID;
csStringID celPcHover::param_p_factor = csInvalidStringID;
csStringID celPcHover::param_i_factor = csInvalidStringID;
csStringID celPcHover::param_d_factor = csInvalidStringID;
csStringID celPcHover::param_hoverheight = csInvalidStringID;

PropertyHolder celPcHover::propinfo;

celPcHover::celPcHover (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg), celPeriodicTimer (pl)
{
  hover_on = true;
  ang_beam_offset = 0.5;
  ang_cutoff_height = 8;
  ang_mult = 1;
  height_beam_cutoff = 200;

  // set default PID values
  pid.p_factor = 8.98f;
  pid.i_factor = 0.0f;
  pid.d_factor = 0.0f;
  pid.hover_height = 4.0f;
  pid.sum_errors = 0.0f;
  pid.last_height = 0.0f;
  pid.dampening = 1.0f;
  pid.clamp = 100.0f;

  if (param_hover == csInvalidStringID)
  {
    // Parameters.
    param_hbeamcutoff = pl->FetchStringID ("heightcutoff");
    param_angoff = pl->FetchStringID ("offset");
    param_angheight = pl->FetchStringID ("angheight");
    param_angstr = pl->FetchStringID ("angstrength");
    param_hover = pl->FetchStringID ("hover");
    param_p_factor = pl->FetchStringID ("pfactor");
    param_i_factor = pl->FetchStringID ("ifactor");
    param_d_factor = pl->FetchStringID ("dfactor");
    param_hoverheight = pl->FetchStringID ("hoverheight");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.hover.action.");
    AddAction (action_sethbeamcutoff, "SetHeightBeamCutoff");
    AddAction (action_setangoff, "SetAngularBeamOffset");
    AddAction (action_setangheight, "SetAngularCutoffHeight");
    AddAction (action_setangstr, "SetAngularCorrectionStrength");
    AddAction (action_hoveron, "HoverOn");
    AddAction (action_setfactors, "SetFactors");
    AddAction (action_sethoverheight, "SetHoverHeight");
  }

  propinfo.SetCount (4);
  AddProperty (propid_p_factor, "p_factor",
        CEL_DATA_FLOAT, false, "Proportional factor.", &pid.p_factor);
  AddProperty (propid_i_factor, "i_factor",
        CEL_DATA_FLOAT, false, "Integral factor.", &pid.i_factor);
  AddProperty (propid_d_factor, "d_factor",
        CEL_DATA_FLOAT, false, "Differential factor.", &pid.d_factor);
  AddProperty (propid_hover_height, "hover_height",
        CEL_DATA_FLOAT, false, "Height for the object to hover at.", &pid.hover_height);
}

celPcHover::~celPcHover ()
{
}

bool celPcHover::PerformActionIndexed (int idx, iCelParameterBlock* params,
    celData& ret)
{
  switch (idx)
  {
    case action_sethbeamcutoff:
      {
	float heightcutoff;
	if (!Fetch (heightcutoff, params, param_hbeamcutoff)) return false;
        SetHeightBeamCutoff (heightcutoff);
        return true;
      }
    case action_setangoff:
      {
	float offset;
	if (!Fetch (offset, params, param_angoff)) return false;
        SetAngularBeamOffset (offset);
	return true;
      }
    case action_setangheight:
      {
	float angheight;
	if (!Fetch (angheight, params, param_angheight)) return false;
        SetAngularCutoffHeight (angheight);
	return true;
      }
    case action_setangstr:
      {
	float angstrength;
	if (!Fetch (angstrength, params, param_angstr)) return false;
        SetAngularCorrectionStrength (angstrength);
	return true;
      }
    case action_hoveron:
      {
        printf ("This action (HoverOn) is temporarily disabled.\n");
        /*bool hover;
	 if (!Fetch (hover, params, param_hover)) return false;
        if (hover)
        {
          //CS_REPORT(ERROR,"Couldn't get 'heightcutoff' parameter for SetAngularCorrectionStrength!");
          printf("Couldn't get 'angstrength' parameter for SetAngularCorrectionStrength!");
          return false;
        }*/
	return true;
      }
    case action_setfactors:
      {
	float p_factor, i_factor, d_factor;
	if (!Fetch (p_factor, params, param_p_factor, true, 0.0f)) return false;
	if (!Fetch (i_factor, params, param_i_factor, true, 0.0f)) return false;
	if (!Fetch (d_factor, params, param_d_factor, true, 0.0f)) return false;
        SetFactors (p_factor, i_factor, d_factor);
        return true;
      }
    case action_sethoverheight:
      {
	float hoverheight;
	if (!Fetch (hoverheight, params, param_hoverheight)) return false;
        SetHoverHeight (hoverheight);
      }
    default:
      return false;
  }
}

void celPcHover::SetFactors (float p, float i, float d)
{
  pid.p_factor = p;
  pid.i_factor = i;
  pid.d_factor = d;
}

void celPcHover::Tick ()
{
  PerformStabilising ();
}

float celPcHover::AngularAlignment (csVector3 offset, float height)
{
  offset *= ang_beam_offset;  // this will convert the (0,0,1) to (0,0,K) vector

  // do first rotation test - simple trigonmetry
  /*
    here we do 2 rotation tests, to see how far the object
    needs to be rotated to be aligned with the terrain.

    You can visualize this as being hd_up, being the height
    in front of the object, and hd_down being the height behind.

    Then using a bit of trig we calculate the angle the
    object has to rotate through to be aligned with the
    terrain.

    Of course this is also done sideways, so just substitue
    hd_up with hd_right in your head.
  */
  float hd_up = Height (pcmechobj->LocalToWorld (offset));
  float r_up = atan ((height - hd_up) / ang_beam_offset);

  float hd_down = Height (pcmechobj->LocalToWorld (-1 * offset));
  float r_down = atan ((height - hd_down) / -ang_beam_offset);

  if (hd_up >= 10000000)
  {
    if(hd_down >= 10000000)
      return 0.0;	// worst case, both heights were infinite

    /* height test 'behind' is the only
        trustable height */
    return r_down;
  }
  else if (hd_down >= 10000000)
  {
    // height test in front is used
    return r_up;
  }

  //! is one value more likely to be better than another?
  return (r_down + r_up) / 2.0f;	// 2 good rotation values - average them
}

float celPcHover::PIDStatus::Force (float curr_height)
{
  float pval, dval, ival;
  float error = hover_height - curr_height;
  //printf ("E: %f\n", error);

  // calculate the proportional term
  pval = p_factor * error;

  // calculate the integral term
  sum_errors += error;
  ival = i_factor * sum_errors;

  // calculate the differential term
  dval = d_factor * (curr_height - last_height);
  last_height = curr_height;

  //printf ("p: %f\ti: %f\td: %f\n", pval, ival, dval);
  return csClamp (pval + ival + dval, clamp, -clamp);
}

void celPcHover::PerformStabilising ()
{
  if (!pcmechobj)
    pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (GetEntity());
  if (!pcmechobj)
    return;

  /* here we get ship info which can be used to calculate
      upthrust force, from functor object */
  float height = Height();

  if (hover_on)
  {
    // do PID calculation here.
    float force = pid.Force (height);
    //printf ("%f %f\n",height,force);

    // apply the force
    pcmechobj->AddForceDuration(csVector3 (0, force, 0), false,
        csVector3 (0,0,0), 0.1f);
  }
  else
  {
    // if hover is turned off we still need to update this since this
    // can be queried outside the property class and its stored here.
    pid.last_height = height;
  }

  // the ships roll and pitch should try to remain level
  if ((ang_mult > 0.0) && (height < ang_cutoff_height))
  {
    float rx = AngularAlignment (csVector3 (0,0,-1), height);
    float rz = AngularAlignment (csVector3 (1,0,0), height);

    // offset hack for tendency of nose to dip into ground when going uphill
    if (rx > 0.0)
      rx *= 3.0;

    // align the ship by getting it to rotate in whatever direction
    pcmechobj->SetAngularVelocity (pcmechobj->GetAngularVelocity() +
        pcmechobj->LocalToWorld (csVector3 (rx,0,rz) * ang_mult));
  }
}

float celPcHover::Height (csVector3 offset, bool accurate)
{
  if (!pcmesh)
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (GetEntity ());
  if (!pcmesh)
    // do something proper here
    return 999999999.9f;
  // get the initial flags so it can be reset afterwards
  uint32 flags = pcmesh->GetMesh()->GetFlags().Get ();
  // set flag so height tests don't hit object itself
  pcmesh->GetMesh ()->GetFlags ().Set (CS_ENTITY_NOHITBEAM);

  /* height is calculated using a hitbeam from objects
      position down along the objects coord system
      through height_beam_cutoff */
  csVector3 start = pcmechobj->GetBody ()->GetPosition () + offset;
  csVector3 end = start + csVector3 (0,-height_beam_cutoff,0);

  iSector* sector = pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0);
  csSectorHitBeamResult bres = sector->HitBeam (start, end, true);
  // beam height * proportion of beam hit
  float height = (bres.isect - start).Norm ();
  if (!CS::IsFinite (height))
  {
    // reset flags to original state
    pcmesh->GetMesh()->GetFlags().SetAll (flags);
    return 999999999.9f;
  }
  // reset flags to original state
  pcmesh->GetMesh()->GetFlags().SetAll (flags);
  return height;
}
