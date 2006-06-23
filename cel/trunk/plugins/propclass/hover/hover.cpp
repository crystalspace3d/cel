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
#include "iutil/evdefs.h"

#include "plugins/propclass/hover/hover.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

#include "propclass/mechsys.h"
#include "propclass/mesh.h"

#include "iengine/mesh.h"
#include "ivaria/dynamics.h"
#include "ivaria/reporter.h"
#include "iutil/virtclk.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY(Hover, "pchover")

SCF_IMPLEMENT_IBASE_EXT (celPcHover)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcHover)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcHover::PcHover)
  SCF_IMPLEMENTS_INTERFACE (iPcHover)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

// Actions
csStringID celPcHover::action_setworld = csInvalidStringID;
csStringID celPcHover::action_sethbeamcutoff = csInvalidStringID;
csStringID celPcHover::action_setangoff = csInvalidStringID;
csStringID celPcHover::action_setangheight = csInvalidStringID;
csStringID celPcHover::action_setangstr = csInvalidStringID;
csStringID celPcHover::action_usedeffunc = csInvalidStringID;

// Parameters.
csStringID celPcHover::param_world = csInvalidStringID;
csStringID celPcHover::param_hbeamcutoff = csInvalidStringID;
csStringID celPcHover::param_angoff = csInvalidStringID;
csStringID celPcHover::param_angheight = csInvalidStringID;
csStringID celPcHover::param_angstr = csInvalidStringID;

celPcHover::celPcHover (iObjectRegistry* object_reg)
	: celPcCommon (object_reg), celPeriodicTimer (pl)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcHover);

  ang_beam_offset = 2;
  ang_cutoff_height = 5;
  ang_mult = 1;
  height_beam_cutoff = 200;

  // init a default upthruster func
  UseDefaultFunction (1.5f);

  if (action_setworld == csInvalidStringID)
  {
     // Actions
     action_setworld = pl->FetchStringID ("cel.action.SetWorld");
     action_sethbeamcutoff = pl->FetchStringID ("cel.action.SetHeightBeamCutoff");
     action_setangoff = pl->FetchStringID ("cel.action.SetAngularBeamOffset");
     action_setangheight = pl->FetchStringID ("cel.action.SetAngularCutoffHeight");
     action_setangstr = pl->FetchStringID ("cel.action.SetAngularCorrectionStrength");
     action_usedeffunc = pl->FetchStringID ("cel.action.UseDefaultStabiliserFunction");

     // Parameters.
     param_world = pl->FetchStringID ("cel.parameter.world");
     param_hbeamcutoff = pl->FetchStringID ("cel.parameter.heightcutoff");
     param_angoff = pl->FetchStringID ("cel.parameter.offset");
     param_angheight = pl->FetchStringID ("cel.parameter.angheight");
     param_angstr = pl->FetchStringID ("cel.parameter.angstrength");
  }
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

bool celPcHover::PerformAction (csStringID actionId, iCelParameterBlock* params)
{
  if (actionId == action_setworld)
  {
    CEL_FETCH_STRING_PAR (world, params, param_world);
    if (!world)
    {
      // CS_REPORT(ERROR,"Couldn't get 'world' parameter for SetWorld!");
      printf("Couldn't get 'world' parameter for SetWorld!\n");
      return false;
    }
    SetWorld (world);
  }
  else if (actionId == action_sethbeamcutoff)
  {
    CEL_FETCH_FLOAT_PAR (heightcutoff, params, param_hbeamcutoff);
    if (!heightcutoff)
    {
      //CS_REPORT(ERROR,"Couldn't get 'heightcutoff' parameter for SetHeightBeamCutoff!");
      printf("Couldn't get 'heightcutoff' parameter for SetHeightBeamCutoff!");
      return false;
    }
    SetHeightBeamCutoff (heightcutoff);
  }
  else if (actionId == action_setangoff)
  {
    CEL_FETCH_FLOAT_PAR (offset, params, param_angoff);
    if (!offset)
    {
      //CS_REPORT(ERROR,"Couldn't get 'offset' parameter for SetAngularBeamOffset!");
      printf("Couldn't get 'offset' parameter for SetAngularBeamOffset!");
      return false;
    }
    SetAngularBeamOffset (offset);
  }
  else if (actionId == action_setangheight)
  {
    CEL_FETCH_FLOAT_PAR (angheight, params, param_angheight);
    if (!angheight)
    {
      //CS_REPORT(ERROR,"Couldn't get 'angheight' parameter for SetAngularCutoffHeight!");
      printf("Couldn't get 'angheight' parameter for SetAngularCutoffHeight!");
      return false;
    }
    SetAngularCutoffHeight (angheight);
  }
  else if (actionId == action_setangstr)
  {
    CEL_FETCH_FLOAT_PAR (angstrength, params, param_angstr);
    if (!angstrength)
    {
      //CS_REPORT(ERROR,"Couldn't get 'heightcutoff' parameter for SetAngularCorrectionStrength!");
      printf("Couldn't get 'angstrength' parameter for SetAngularCorrectionStrength!");
      return false;
    }
    SetAngularCorrectionStrength (angstrength);
  }
  else if (actionId == action_usedeffunc)
  {
    UseDefaultFunction (1.5f);
  }
  else return false;

  return true;
}

void celPcHover::Tick ()
{
  PerformStabilising ();
}


void celPcHover::SetWorld (const char *name)
{
  world_mesh_name = name;
}
void celPcHover::LookUpWorldMesh ()
{
  /* at the moment this doesn't seem to work */
  iCelEntity *went = pl->FindEntity (world_mesh_name);
  if(!went)  return;
  world_mesh = CEL_QUERY_PROPCLASS_ENT (went , iPcMesh);
  world_mesh_name = 0;
}

void celPcHover::UseDefaultFunction (float dampening)
{
  func.AttachNew(new celDefaultHoverUpthruster (dampening));
}

float celPcHover::AngularAlignment (csVector3 offset, float height)
{
  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (GetEntity(),
  	iPcMechanicsObject);
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

void celPcHover::PerformStabilising ()
{
  // if this hasn't been init, return safely
  if (!world_mesh)
  {
    if (world_mesh_name)
      LookUpWorldMesh ();
    else
      return;
  }

  if (!ship_mech)
    ship_mech = CEL_QUERY_PROPCLASS_ENT (GetEntity(), iPcMechanicsObject);

  /* here we get ship info which can be used to calculate
      upthrust force, from functor object */
  celHoverObjectInfo obj_info;
  // ships height
  object_height = obj_info.height = Height();
  // ships local vertical velocity
  obj_info.yvel = ship_mech->WorldToLocal (ship_mech->GetLinearVelocity ()).y;

  /* get functor object to calculate upthrust force
      from ships info */
  float force = func->Force (obj_info);
  //printf ("%f %f\n",obj_info.height,force);

  // apply the force
  ship_mech->AddForceDuration(csVector3 (0, force, 0), false,
      csVector3 (0,0,0), 0.1f);
  //pcmechobj->AddForceOnce (csVector3 (0,force,0), false, csVector3 (0,0,0));
  //pcmechobj->SetLinearVelocity (pcmechobj->GetLinearVelocity () + csVector3 (0,force,0));

  // the ships roll and pitch should try to remain level
  if ((ang_mult > 0.0) && (obj_info.height < ang_cutoff_height))
  {
    float rx = AngularAlignment (csVector3 (0,0,-1), obj_info.height);
    float rz = AngularAlignment (csVector3 (1,0,0), obj_info.height);

    // align the ship by getting it to rotate in whatever direction
    ship_mech->SetAngularVelocity (ship_mech->GetAngularVelocity() +
        ship_mech->LocalToWorld (csVector3 (rx,0,rz) * ang_mult));
  }
}

float celPcHover::Height (csVector3 offset)
{
  /* height is calculated using a hitbeam from objects
      position down along the objects coord system
      through height_beam_cutoff */
  csVector3 start = ship_mech->GetBody()->GetPosition() + offset;
  csVector3 end = start + csVector3 (0,-height_beam_cutoff,0);

  csHitBeamResult bres = world_mesh->GetMesh()->HitBeam(start , end);
  if(bres.hit)
    // beam height * proportion of beam hit
    return height_beam_cutoff * bres.r;
  else
    /* beam didn't hit so we try going upwards
        from object */
    return ReverseHeight(start);
}

float celPcHover::ReverseHeight (csVector3 &start)
{
  // instead of downwards the beam goes upwards
  csVector3 end = start + csVector3 (0,height_beam_cutoff,0);

  csHitBeamResult bres = world_mesh->GetMesh()->HitBeam(start , end);
  if(bres.hit)
    return -height_beam_cutoff * bres.r;
  else
    return 999999999.0f;
}
