/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein

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
#include "plugins/propclass/grab/grab.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

// FOR TESTING---- remove this
#include "iutil/virtclk.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "propclass/mesh.h"

// CEL Includes
#include "propclass/jump.h"
#include "propclass/linmove.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Grab, "pcmove.grab")

//---------------------------------------------------------------------------

PropertyHolder celPcGrab::propinfo;

celPcGrab::celPcGrab (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  propholder = &propinfo;
  // For actions.
  if (!propinfo.actions_done)
  {
  }

  // For properties.
  propinfo.SetCount (0);

  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);

  currstate = DISABLED;
  stime = 2.0f / 3.0f;
  sdist = 2.0;
  sinvel = 6.0;
  RecomputeShimmyAccel ();
}

celPcGrab::~celPcGrab ()
{
}

#define TEST_SERIAL 0

csPtr<iCelDataBuffer> celPcGrab::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcGrab::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL) return false;
  return true;
}

bool celPcGrab::PerformActionIndexed (int idx, iCelParameterBlock* params, celData& ret)
{
  /*switch (idx)
  {
    default:
      return false;
  }*/
  return false;
}

//---------------------------------------------------------------------------

bool celPcGrab::ReceiveMessage (csStringID msg_id, iMessageSender *sender, celData &ret, iCelParameterBlock *params)
{
  AttemptGrab ();
  return true;
}

void celPcGrab::SetState (GrabState state)
{
  currstate = state;
}
iPcGrab::GrabState celPcGrab::GetState () const
{
  return currstate;
}

void celPcGrab::SetShimmyTime (float time)
{
  stime = time;
  RecomputeShimmyAccel ();
}
float celPcGrab::GetShimmyTime () const
{
  return stime;
}
void celPcGrab::SetShimmyDistance (float dist)
{
  sdist = dist;
  RecomputeShimmyAccel ();
}
float celPcGrab::GetShimmyDistance () const
{
  return sdist;
}
void celPcGrab::SetShimmyInitialVelocity (float vel)
{
  sinvel = vel;
  RecomputeShimmyAccel ();
}
float celPcGrab::GetShimmyInitialVelocity () const
{
  return sinvel;
}
void celPcGrab::RecomputeShimmyAccel ()
{
  saccel = 2.0 * (sinvel * stime - sdist) / (stime * stime);
}

bool celPcGrab::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    // not the place, but works well enough :)
    entity->QueryMessageChannel ()->Subscribe (this, "cel.move.jump.started");
    linmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
    jump = celQueryPropertyClassEntity<iPcJump> (entity);
  }
  return linmove && jump;
}

void celPcGrab::TickEveryFrame ()
{
  UpdateMovement ();
}

static csVector3 FindClosestPointOnEdge (const csVector3 &start, const csVector3 &hand, const csVector3 &edgediff)
{
  return start + ((hand - start) >> edgediff);
}
// loser function needed for quick code below
static float FindDistanceFromEdge (const csVector3 &start, const csVector3 &hand, const csVector3 &edgediff)
{
  return (start + ((hand - start) >> edgediff) - hand).SquaredNorm ();
}


void celPcGrab::UpdateMovement ()
{
  if (currstate == DISABLED || !FindSiblingPropertyClasses ())
    return;

  //else
    //linmove->SetBodyVelocity (csVector3 (0));

  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
  iMovable *mov = engine->FindMeshObject ("HandLeft")->GetMovable ();
  iMovable *mov1 = engine->FindMeshObject ("HandRight")->GetMovable ();
  csRef<iPcMesh> mesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  csVector3 lefthand = mesh->GetMesh ()->GetMovable ()->GetFullTransform ().This2Other (csVector3 (0.2f, 1.4f, -0.4f));
  mov->SetPosition (lefthand);
  csVector3 righthand = mesh->GetMesh ()->GetMovable ()->GetFullTransform ().This2Other (csVector3 (-0.2f, 1.4f, -0.4f));
  mov1->SetPosition (righthand);

  csRef<iVirtualClock> vc = csQueryRegistry<iVirtualClock> (object_reg);
  csTicks el = vc->GetElapsedTicks ();

  if (currstate == SHIMMY_RIGHT)
  {
    float s = -linmove->GetBodyVelocity ().x;
    s -= saccel * el / 1000.0f;
    if (s < 0.0f)
      s = sinvel;
    linmove->SetBodyVelocity (csVector3 (-s, 0, 0));
  }
  else if (currstate == SHIMMY_LEFT)
  {
    float s = linmove->GetBodyVelocity ().x;
    s -= saccel * el / 1000.0f;
    if (s < 0.0f)
      s = sinvel;
    linmove->SetBodyVelocity (csVector3 (s, 0, 0));
  }
  else if (currstate == HANG)
  {
    linmove->SetBodyVelocity (csVector3 (0));
  }
  else if (currstate == SEARCHING)
  //csRef<iPcMesh> mesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  //if (mesh->GetMesh ()->GetMovable ()->GetFullTransform ().This2Other (csVector3 (0.2, 1.4, -0.2)).y > 1.3f)
  {
    csVector3 lefthand = mesh->GetMesh ()->GetMovable ()->GetFullTransform ().This2Other (csVector3 (0.2f, 1.4f, -0.4f));
    csVector3 righthand = mesh->GetMesh ()->GetMovable ()->GetFullTransform ().This2Other (csVector3 (-0.2f, 1.4f, -0.4f));
    static const csVector3 leftcorn (2.0f, 1.3f, -4.9f), rightcorn (0.0f, 1.3f, -4.9f), edgediff (-2.0f, 0.0f, 0.0f);
    csVector3 u (rightcorn - leftcorn), v (lefthand - leftcorn);
    csVector3 closest;
    closest = leftcorn + (v >> u);

    if (FindDistanceFromEdge (leftcorn, lefthand, edgediff) < 0.1f ||
      FindDistanceFromEdge (leftcorn, righthand, edgediff) < 0.1f)
    {
      //puts ("Grab LEDGE");
      csVector3 other (closest + u * 0.2);

      csVector3 cent = (closest + other) / 2;
      csVector3 dir = u;
      // i dont like its 2d
      float z = dir.x;
      dir.x = dir.z;
      dir.z = z;
      csVector3 dir2 (dir);
      dir2.z = -dir.z;
      csVector3 centrehand ((lefthand + righthand) / 2);
      if ((cent + dir - centrehand).SquaredNorm () < (cent + dir2 - centrehand).SquaredNorm ())
        dir = dir2;
      csReversibleTransform blaa;
      blaa.LookAt (-dir, csVector3 (0, 1, 0));
      blaa.SetOrigin (cent - blaa.This2OtherRelative (csVector3 (0.0f, 1.4f, -0.4f)));
      mesh->GetMesh ()->GetMovable ()->SetTransform (blaa);

      //jump->Enable (false);
      jump->Freeze (true);
      currstate = HANG;
    }
  }
}

void celPcGrab::AttemptGrab ()
{
  if (!FindSiblingPropertyClasses ())
    return;

  // -------------------
  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);

  // how many steps to take along extrapolated jump for testing closeness of hands
  const size_t teststeps = 10;
  // offset of left hand from origin
  csVector3 handoff (0.2f, 1.4f, -0.4f);

  // our hardcoded edge
  static const csVector3 leftcorn (2.0f, 1.3f, -4.9f), rightcorn (0.0f, 1.3f, -4.9f), edgediff (-2.0f, 0.0f, 0.0f);

  // the time it takes to perform the jump (entire arc)
  float jumptime = 2.0f * jump->GetJumpSpeed () / jump->GetGravity ();
  // which hand is closest and the squared length it was from the edge
  size_t hand = 3;
  float fromedge_sq = 99999999.9f;
  // save the position of the closest point on edge to hand during extrapolated jump for later
  csVector3 nearposedge;
  // current transform
  csReversibleTransform playtrans (linmove->GetFullTransform ());
  for (size_t currstep = 1; currstep <= teststeps; currstep++)
  {
    // using laws of motion, get the extrapolated time and use it to calculate where we are in jump
    float currtime = (jumptime * currstep) / teststeps,
      currheight = jump->GetJumpSpeed () * currtime - 0.5 * jump->GetGravity () * currtime * currtime,
      currforward = linmove->GetBodyVelocity ().z * currtime;

    // current position of both hands in space
    csVector3 currhands [2];
    currhands[0].Set (playtrans.This2Other (csVector3 (0, currheight, currforward) + handoff));
    // flip x coordinate to calculate for the other hand
    handoff.x = -handoff.x;
    currhands[1].Set (playtrans.This2Other (csVector3 (0, currheight, currforward) + handoff));
    // flip it back again
    handoff.x = -handoff.x;

    // find closest point on edge to our hands
    csVector3 handspos[2] = {
      FindClosestPointOnEdge (leftcorn, currhands[0], edgediff),
      FindClosestPointOnEdge (leftcorn, currhands[1], edgediff) };
    float closest [2] = {
      (handspos[0] - currhands[0]).SquaredNorm (),
      (handspos[1] - currhands[1]).SquaredNorm () };
    printf ("%f %f", sqrt(closest[0]), sqrt(closest[1]));
    // find the hand closest to the edge
    // if it beats our previous record then we keep it
    if (closest[0] < closest[1])  // left hand is closest!
    {
      if (closest[0] < fromedge_sq)
      {
        hand = 0;
        fromedge_sq = closest[0];
        nearposedge = handspos[0];
        printf ("\t 0");
      }
    }
    else
    {
      if (closest[1] < fromedge_sq)
      {
        hand = 1;
        fromedge_sq = closest[1];
        nearposedge = handspos[1];
        printf ("\t 1");
      }
    }
    puts ("");

    // ------------ debug
    char debugname[3] = { 'J', '0' + currstep - 1, '\0' };
    iMovable *debugmarker = engine->FindMeshObject (debugname)->GetMovable ();
    debugmarker->SetPosition (currhands[0]);
    iMovable *tar = engine->FindMeshObject ("Target")->GetMovable ();
    iMovable *tarother = engine->FindMeshObject ("TargetOther")->GetMovable ();
    if (hand == 0 && (fromedge_sq - closest[0]) < EPSILON)
    {
      tar->SetPosition (FindClosestPointOnEdge (leftcorn, currhands[0], edgediff));
      tarother->SetPosition (FindClosestPointOnEdge (leftcorn, currhands[1], edgediff));
    }
    else if (hand == 1 && (fromedge_sq - closest[1]) < EPSILON)
    {
      tar->SetPosition (FindClosestPointOnEdge (leftcorn, currhands[1], edgediff));
      tarother->SetPosition (FindClosestPointOnEdge (leftcorn, currhands[0], edgediff));
    }
  }

  if (hand == 0)
    puts ("left hand is closesr");
  else
    puts ("right hand is closest");

  // test if at any point we ever come close to any ledge
  if (fromedge_sq > 0.1f)
    return;
  puts ("We are close to a ledge");
  //jump->Enable (false);

  // now find closest current point to either hand so that if our speed is too fast we can slow it down
  //  if we don't reach height of jump before coming in contact with the ledge
  csVector3 handposnow;
  if (hand == 0)  // left hand
  {
    handposnow = playtrans.This2Other (handoff);
  }
  else if (hand == 1) // right hand
  {
    // to get 'other' hand we need to flip x
    handoff.x = -handoff.x;
    handposnow = playtrans.This2Other (handoff);
    // flip it back
    handoff.x = -handoff.x;
  }
  const float handedge_sq = (nearposedge - handposnow).SquaredNorm ();

  float halfprojjump = -linmove->GetBodyVelocity ().z * jump->GetAirTime () / (2 * 1000.0);
  printf ("Projected Jump length / 2 = %f\n", halfprojjump);
  if (-linmove->GetBodyVelocity ().z < EPSILON || handedge_sq < halfprojjump * halfprojjump)
  {
    csVector3 currspeed (linmove->GetBodyVelocity ());
    currspeed.z = -sqrt(handedge_sq) * 2;
    linmove->SetBodyVelocity (currspeed);
  }

  // now face ledge so that we get there with minimal fuss

  // ----------------------------
  iMovable *mov = engine->FindMeshObject ("HandLeft")->GetMovable ();
  iMovable *mov1 = engine->FindMeshObject ("HandRight")->GetMovable ();
  iMovable *corn = engine->FindMeshObject ("Corner0")->GetMovable ();
  iMovable *corn1 = engine->FindMeshObject ("Corner1")->GetMovable ();
  iMovable *tar = engine->FindMeshObject ("Target")->GetMovable ();
  iMovable *tarother = engine->FindMeshObject ("TargetOther")->GetMovable ();
  iMovable *tarfor = engine->FindMeshObject ("TargetForward")->GetMovable ();
  csRef<iPcMesh> mesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  //mov->SetTransform (mesh->GetMesh ()->GetMovable ()->GetFullTransform ());
  //mov->SetPosition (mesh->GetMesh ()->GetMovable ()->GetPosition () + csVector3 (0.2, 1.4, 0));
  csVector3 lefthand = mesh->GetMesh ()->GetMovable ()->GetFullTransform ().This2Other (csVector3 (0.2f, 1.4f, -0.2f));
  mov->SetPosition (lefthand);
  csVector3 righthand = mesh->GetMesh ()->GetMovable ()->GetFullTransform ().This2Other (csVector3 (-0.2f, 1.4f, -0.2f));
  mov1->SetPosition (righthand);
  corn->SetPosition (leftcorn);
  corn1->SetPosition (rightcorn);

  csVector3 u (rightcorn - leftcorn), v (lefthand - leftcorn);
  csVector3 closest;
  closest = leftcorn + (v >> u);
  //closest = (leftcorn + rightcorn) / 2;

  csVector3 other (closest + u * 0.2);

  // see which is smallest distance
  csVector3 cent = (closest + other) / 2;
  if ((closest - cent).SquaredNorm () > (other - cent).SquaredNorm ())
  {
    csVector3 tmp (closest);
    closest = other;
    other = tmp;
  }
  tar->SetPosition (closest);
  tarother->SetPosition (other);

  csVector3 dir = other - cent;
  float z = dir.x;
  dir.x = dir.z;
  dir.z = z;
  tarfor->SetPosition (cent + dir);

  csReversibleTransform blaa (mesh->GetMesh ()->GetMovable ()->GetFullTransform ());
  cent.y = blaa.GetOrigin ().y;
  csVector3 flatdiff (blaa.GetOrigin () - cent);
  blaa.LookAt (flatdiff, csVector3 (0, 1, 0));
  //if (flatdiff.SquaredNorm () < 4.0f)
    //mesh->GetMesh ()->GetMovable ()->SetTransform (blaa);
}
