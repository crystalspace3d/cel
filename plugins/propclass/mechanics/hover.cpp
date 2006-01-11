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

#include "plugins/propclass/mechanics/hover.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

#include <propclass/mechsys.h>
#include <propclass/mesh.h>

#include <iengine/mesh.h>
#include <ivaria/dynamics.h>
#include <iutil/virtclk.h>

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY(Hover, "pchover")

SCF_IMPLEMENT_IBASE_EXT (celPcHover)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcHover)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcHover::PcHover)
  SCF_IMPLEMENTS_INTERFACE (iPcHover)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcHover::celPcHover (iObjectRegistry* object_reg)
	: celPcCommon (object_reg), celPeriodicTimer (pl)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcHover);

  ang_beam_offset = 2;
  ang_cutoff_height = 5;
  ang_mult = 1;
  height_beam_cutoff = 200;
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
  csRef<iPcMechanicsObject> ship_mech = CEL_QUERY_PROPCLASS_ENT (GetEntity(), iPcMechanicsObject);
  return true;
}
bool celPcHover::PerformAction (csStringID actionId, iCelParameterBlock* params)
{
  return true;
}

void celPcHover::Tick ()
{
  PerformStabilising ();
}


void celPcHover::SetWorld (const char *name)
{
  iCelEntity *went = pl->FindEntity (name);
  if(!went)  return;
  world_mesh = CEL_QUERY_PROPCLASS_ENT (went , iPcMesh);
}

void celPcHover::DefaultHeightFunction ()
{
  csRef<celIntervalMetaDistribution> i(new celIntervalMetaDistribution());
  csRef<celReturnConstantValue> a(new celReturnConstantValue(70.0));
  csRef<celSquareDistribution>  b(new celSquareDistribution(1.0001f , 30.0f , 3.0f , 20.0f));
  csRef<celReturnConstantValue> c(new celReturnConstantValue(0.0));

  i->Add(a, -9999999999.0f , 0.0001f);
  i->Add(b, 0.0001f , 3.0f);
  i->Add(c, 3.0f , 9999999999.0f);

  csRef<celIntervalMetaDistribution> e(new celIntervalMetaDistribution());
  csRef<celReturnConstantValue> m(new celReturnConstantValue(30.0));
  csRef<celSquareDistribution>  n(new celSquareDistribution(1.0001f , 9.8f , 3.0f , 9.0f));
  csRef<celReturnConstantValue> o(new celReturnConstantValue(0.0));

  e->Add(m, -9999999999.0f , 0.0001f);
  e->Add(n, 0.0001f , 3.0f);
  e->Add(o, 3.0f , 9999999999.0f);

  csRef<iPcMechanicsObject> ship_mech = CEL_QUERY_PROPCLASS_ENT (GetEntity(), iPcMechanicsObject);
  func = new celIfFallingDistribution(ship_mech , i , e , 0.0);
}

float celPcHover::AngularAlignment (csVector3 offset, float height)
{
  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (GetEntity() , iPcMechanicsObject);
  offset *= ang_beam_offset;

  // do first rotation test - simple trigonmetry
  float hd_up = Height (pcmechobj->LocalToWorld (offset));
  float r_up = atan ((height - hd_up) / ang_beam_offset);

  float hd_down = Height (pcmechobj->LocalToWorld (-1 * offset));
  float r_down = atan ((height - hd_down) / -ang_beam_offset);

  if (hd_up >= 10000000)
  {
    if(hd_down >= 10000000)
      return 0.0;	// worst case, both heights were infinite

    return r_down;
  }
  else if (hd_down >= 10000000)
  {
    return r_up;
  }

  //! is one value more likely to be better than another?
  return (r_down + r_up) / 2;	// 2 good rotation values - average them
}

void celPcHover::PerformStabilising ()
{
  float height = Height();

  float force = func->Force (height);

  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (GetEntity(), iPcMechanicsObject);
  pcmechobj->AddForceDuration (csVector3 (0,force,0), false, csVector3 (0,0,0), 0.01f);

  // the ships roll should try to remain level (levels faster when closer to ground)
  if(height < ang_cutoff_height) {
    float rx = AngularAlignment (csVector3 (0,0,-1), height);
    float rz = AngularAlignment (csVector3 (1,0,0), height);
    pcmechobj->SetAngularVelocity (pcmechobj->GetAngularVelocity() + pcmechobj->LocalToWorld(csVector3(rx,0,rz) * ang_mult));
  }
}

float celPcHover::Height (csVector3 offset)
{
  csRef<iPcMechanicsObject> ship_mech = CEL_QUERY_PROPCLASS_ENT (GetEntity(), iPcMechanicsObject);
  csVector3 start = ship_mech->GetBody()->GetPosition() + offset;
  csVector3 end = start + csVector3(0,-height_beam_cutoff,0);
#ifdef NEW_HEIGHTCALC
  // having this verified would be nice
  // move vector down along ships coord sys (not world coords)
  end = ship_mech->LocalToWorld(end);
#endif

  csHitBeamResult bres = world_mesh->GetMesh()->HitBeam(start , end);
  if(bres.hit)
    return height_beam_cutoff * bres.r;
  else
    return ReverseHeight(start);
}
float celPcHover::ReverseHeight (csVector3 &start)
{
  csVector3 end = start + csVector3(0,height_beam_cutoff,0);
#ifdef NEW_HEIGHTCALC
  end = ship_mech->LocalToWorld(end);
#endif

  csHitBeamResult bres = world_mesh->GetMesh()->HitBeam(start , end);
  if(bres.hit)
    return -height_beam_cutoff * bres.r;
  else
    return 999999999.0f;
}
