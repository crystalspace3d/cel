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
#include "propclass/defcam.h"

#include "csgeom/box.h"
#include "csgeom/math3d.h"
#include "iengine/camera.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"
#include "imesh/object.h"
#include "imesh/objmodel.h"
#include "ivaria/dynamics.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY(Hover, "pchover")

SCF_IMPLEMENT_IBASE_EXT (celPcHover)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcHover)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcHover::PcHover)
  SCF_IMPLEMENTS_INTERFACE (iPcHover)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

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
	: celPcCommon (object_reg), celPeriodicTimer (pl)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcHover);

  hover_on = true;
  ang_beam_offset = 0.5;
  ang_cutoff_height = 8;
  ang_mult = 1;
  height_beam_cutoff = 200;

  // set default PID values
  pid.p_factor = 8.98;
  pid.i_factor = 0.0;
  pid.d_factor = 0.0;
  pid.hover_height = 4.0;
  pid.sum_errors = 0.0;
  pid.last_height = 0.0;
  pid.dampening = 1.0;
  pid.clamp = 100.0;

  if (param_hover == csInvalidStringID)
  {
    // Parameters.
    param_hbeamcutoff = pl->FetchStringID ("cel.parameter.heightcutoff");
    param_angoff = pl->FetchStringID ("cel.parameter.offset");
    param_angheight = pl->FetchStringID ("cel.parameter.angheight");
    param_angstr = pl->FetchStringID ("cel.parameter.angstrength");
    param_hover = pl->FetchStringID ("cel.parameter.hover");
    param_p_factor = pl->FetchStringID ("cel.parameter.pfactor");
    param_i_factor = pl->FetchStringID ("cel.parameter.ifactor");
    param_d_factor = pl->FetchStringID ("cel.parameter.dfactor");
    param_hoverheight = pl->FetchStringID ("cel.parameter.hoverheight");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_sethbeamcutoff, "cel.action.SetHeightBeamCutoff");
    AddAction (action_setangoff, "cel.action.SetAngularBeamOffset");
    AddAction (action_setangheight, "cel.action.SetAngularCutoffHeight");
    AddAction (action_setangstr, "cel.action.SetAngularCorrectionStrength");
    AddAction (action_hoveron, "cel.action.HoverOn");
    AddAction (action_setfactors, "cel.action.SetFactors");
    AddAction (action_sethoverheight, "cel.action.SetHoverHeight");
  }

  propinfo.SetCount (4);
  AddProperty (propid_p_factor, "cel.property.p_factor",
        CEL_DATA_FLOAT, false, "Proportional factor.", &pid.p_factor);
  AddProperty (propid_i_factor, "cel.property.i_factor",
        CEL_DATA_FLOAT, false, "Integral factor.", &pid.i_factor);
  AddProperty (propid_d_factor, "cel.property.d_factor",
        CEL_DATA_FLOAT, false, "Differential factor.", &pid.d_factor);
  AddProperty (propid_hover_height, "cel.property.hover_height",
        CEL_DATA_FLOAT, false, "Height for the object to hover at.", &pid.hover_height);
}

celPcHover::~celPcHover ()
{
}

csPtr<iCelDataBuffer> celPcHover::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (1);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcHover::Load (iCelDataBuffer* databuf)
{
  csRef<iPcMechanicsObject> ship_mech = CEL_QUERY_PROPCLASS_ENT (GetEntity(),
  	iPcMechanicsObject);
  return true;
}

bool celPcHover::PerformActionIndexed (int idx, iCelParameterBlock* params,
    celData& ret)
{
  switch (idx)
  {
    case action_sethbeamcutoff:
      {
        CEL_FETCH_FLOAT_PAR (heightcutoff, params, param_hbeamcutoff);
        if (!heightcutoff)
        {
          //CS_REPORT(ERROR,"Couldn't get 'heightcutoff' parameter for SetHeightBeamCutoff!");
          printf("Couldn't get 'heightcutoff' parameter for SetHeightBeamCutoff!");
          return false;
        }
        SetHeightBeamCutoff (heightcutoff);
        return true;
      }
    case action_setangoff:
      {
        CEL_FETCH_FLOAT_PAR (offset, params, param_angoff);
        if (!offset)
        {
          //CS_REPORT(ERROR,"Couldn't get 'offset' parameter for SetAngularBeamOffset!");
          printf("Couldn't get 'offset' parameter for SetAngularBeamOffset!");
          return false;
        }
        SetAngularBeamOffset (offset);
	return true;
      }
    case action_setangheight:
      {
        CEL_FETCH_FLOAT_PAR (angheight, params, param_angheight);
        if (!angheight)
        {
          //CS_REPORT(ERROR,"Couldn't get 'angheight' parameter for SetAngularCutoffHeight!");
          printf("Couldn't get 'angheight' parameter for SetAngularCutoffHeight!");
          return false;
        }
        SetAngularCutoffHeight (angheight);
	return true;
      }
    case action_setangstr:
      {
        CEL_FETCH_FLOAT_PAR (angstrength, params, param_angstr);
        if (!angstrength)
        {
          //CS_REPORT(ERROR,"Couldn't get 'heightcutoff' parameter for SetAngularCorrectionStrength!");
          printf("Couldn't get 'angstrength' parameter for SetAngularCorrectionStrength!");
          return false;
        }
        SetAngularCorrectionStrength (angstrength);
	return true;
      }
    case action_hoveron:
      {
        printf ("This action (HoverOn) is temporarily disabled.\n");
        /*CEL_FETCH_BOOL_PAR (hover, params, param_hover);
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
        CEL_FETCH_FLOAT_PAR (p_factor, params, param_p_factor);
        CEL_FETCH_FLOAT_PAR (i_factor, params, param_i_factor);
        CEL_FETCH_FLOAT_PAR (d_factor, params, param_d_factor);
        SetFactors (p_factor, i_factor, d_factor);
        return true;
      }
    case action_sethoverheight:
      {
        CEL_FETCH_FLOAT_PAR (hoverheight, params, param_hoverheight);
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
    csClamp (pval, -clamp, clamp);

    // calculate the integral term
    sum_errors += error;
    ival = i_factor * sum_errors;
    csClamp (ival, -clamp, clamp);

    // calculate the differential term
    dval = d_factor * (curr_height - last_height);
    last_height = curr_height;
    csClamp (dval, -clamp, clamp);

    //printf ("p: %f\ti: %f\td: %f\n", pval, ival, dval);
    return pval + ival + dval;
}

void celPcHover::PerformStabilising ()
{
  if (!pcmechobj)
    pcmechobj = CEL_QUERY_PROPCLASS_ENT (GetEntity(), iPcMechanicsObject);
  if (!pccamera)
    pccamera = CEL_QUERY_PROPCLASS_ENT (GetEntity(), iPcDefaultCamera);
  if (!pcmechobj || !pccamera)
    return;

  /* here we get ship info which can be used to calculate
      upthrust force, from functor object */
  float height = Height();

  if (hover_on)
  {
    // do PID calculation here.
    float force = pid.Force (height);
    //printf ("%f %f\n",obj_info.height,force);

    // apply the force
    pcmechobj->AddForceDuration(csVector3 (0, force, 0), false,
        csVector3 (0,0,0), 0.1f);
    //pcmechobj->AddForceOnce (csVector3 (0,force,0), false, csVector3 (0,0,0));
    //pcmechobj->SetLinearVelocity (pcmechobj->GetLinearVelocity () + csVector3 (0,force,0));
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
    pcmesh = CEL_QUERY_PROPCLASS_ENT (GetEntity (), iPcMesh);
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

  iSector *sector = pccamera->GetCamera ()->GetSector ();
  csSectorHitBeamResult bres = sector->HitBeam (start, end, true);
  //if(bres.hit)
    // beam height * proportion of beam hit
    float height = (bres.isect - start).Norm ();
    if (!csFinite (height))
    {
      // reset flags to original state
      pcmesh->GetMesh()->GetFlags().SetAll (flags);
      return 999999999.9f;
    }
    // reset flags to original state
    pcmesh->GetMesh()->GetFlags().SetAll (flags);
    return height;
  //else
    /* beam didn't hit so we try going upwards
        from object */
    //return ReverseHeight(start);
}

float celPcHover::ReverseHeight (csVector3 &start, iSector *sector)
{
  // instead of downwards the beam goes upwards
  csVector3 end = start + csVector3 (0,height_beam_cutoff,0);

  csSectorHitBeamResult bres = sector->HitBeam(start, end, false);
  if(false)
    return (start - bres.isect).Norm ();
  else
    return 999999999.0f;
}
