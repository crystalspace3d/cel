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
#include "iengine/sector.h"
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

  movable = 0;
  currstate = DISABLED;
  stime = 2.0f / 3.0f;
  sdist = 2.0;
  sinvel = 6.0;
  RecomputeShimmyAccel ();
  left_hand_rel.Set (-0.2f, 1.4f, -0.4f);

  currsector = 0;
  c_ledge_point_id = 0;
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

bool celPcGrab::FindInterfaces ()
{
  if (HavePropertyClassesChanged ())
  {
    // not the place, but works well enough :)
    entity->QueryMessageChannel ()->Subscribe (this, "cel.move.jump.started");
    linmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
    jump = celQueryPropertyClassEntity<iPcJump> (entity);
    csRef<iPcMesh> mesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    if (mesh)
      movable = mesh->GetMesh ()->GetMovable ();
  }
  // check if the sector has changed since the last update. if it has then rerequest ledgegroup from new sector
  if (movable->GetSectors ()->Get (0) != currsector)
  {
    currsector = movable->GetSectors ()->Get (0);
    ledges = scfQueryInterface<iLedgeGroup> (currsector->QueryObject ()->GetChild ("cel.ledgegroup"));
  }
  return linmove && jump && movable && ledges;
}

void celPcGrab::TickEveryFrame ()
{
  UpdateMovement ();
}

static bool LiesOnSegment (const csVector3 &left, const csVector3 &eddir, const csVector3 &point)
{
  //const csVector3 eddir (right - left);
  float u = (point - left) * eddir / eddir.SquaredNorm ();
  if (u < 0 || u > 1)
    return false;
  return true;
}
static csVector3 ClosestOnSegment (const csVector3 &left, const csVector3 &edgediff, const csVector3 &point)
{
  float u = (point - left) * edgediff / edgediff.SquaredNorm ();
  if (u < 0)
    return left;
  else if (u > 1)
    return left + edgediff;
  return left + u * edgediff;
}
inline static bool ShouldGrabLedge (const csVector3 &start, const csVector3 &edgediff, const csVector3 &hand)
{
  return (ClosestOnSegment (start, edgediff, hand) - hand).SquaredNorm () < 0.1f;
}

void celPcGrab::UpdateMovement ()
{
  if (currstate == DISABLED || !FindInterfaces ())
    return;

  //else
    //linmove->SetBodyVelocity (csVector3 (0));

  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
  iMovable *mov = engine->FindMeshObject ("HandLeft")->GetMovable ();
  iMovable *mov1 = engine->FindMeshObject ("HandRight")->GetMovable ();
  csVector3 lefthand = movable->GetFullTransform ().This2Other (csVector3 (0.2f, 1.4f, -0.4f));
  mov->SetPosition (lefthand);
  csVector3 righthand = movable->GetFullTransform ().This2Other (csVector3 (-0.2f, 1.4f, -0.4f));
  mov1->SetPosition (righthand);
  //static const csVector3 leftcorn (2.0f, 1.3f, -4.9f), rightcorn (0.0f, 1.3f, -4.9f), edgediff (-2.0f, 0.0f, 0.0f);

  csRef<iVirtualClock> vc = csQueryRegistry<iVirtualClock> (object_reg);
  csTicks el = vc->GetElapsedTicks ();

  // moving sideways along an edge
  if (currstate == SHIMMY_RIGHT || currstate == SHIMMY_LEFT)
  {
    // now we just get the 2 current points defining the ledge
    csVector3 leftcorn, rightcorn;
    iLedge* l = ledges->Get (c_ledge_id);
    leftcorn.y = rightcorn.y = l->GetYPosition ();
    {
      csVector2 proxpos (l->GetPoint (c_ledge_point_id - 1));
      leftcorn.x = proxpos.x;
      leftcorn.z = proxpos.y;
      proxpos = l->GetPoint (c_ledge_point_id);
      rightcorn.x = proxpos.x;
      rightcorn.z = proxpos.y;
    }
    // ...and calculate the offset
    const csVector3 edgediff (rightcorn - leftcorn);
    // convert shimmy direction to dir value- interface should actually change
    int dir = -1;
    csVector3 hand = righthand;
    if (currstate == SHIMMY_LEFT)
    {
      hand = lefthand;
      dir = 1;
    }
    // now if we lie on the segment then we can shimmy
    if (LiesOnSegment (leftcorn, edgediff, hand))
    {
      float s = dir * linmove->GetBodyVelocity ().x;
      // deccelerate from initial velocity
      s -= saccel * el / 1000.0f;
      // when speed hits 0 then reset speed- like a ball bouncing without damping
      if (s < 0.0f)
        s = sinvel;
      linmove->SetBodyVelocity (csVector3 (dir * s, 0, 0));
    }
    // but stop as soon as we start to move off of it
    else
      linmove->SetBodyVelocity (csVector3 (0));
  }
  else if (currstate == HANG)
  {
    linmove->SetBodyVelocity (csVector3 (0));
  }
  else if (currstate == SEARCHING)
  {
    // we find the hands position in world space
    csReversibleTransform currtrans = movable->GetFullTransform ();
    csVector3 lefthand = currtrans.This2Other (left_hand_rel);
    // flip x component to get right hand in players local space
    left_hand_rel.x = -left_hand_rel.x;
    csVector3 righthand = currtrans.This2Other (left_hand_rel);
    // and remember to reset it again afterwards
    left_hand_rel.x = -left_hand_rel.x;

    // iterate through all the ledges in this sector and do our test
    for (size_t ledidx = 0; ledidx < ledges->GetCount (); ledidx++)
    {
      iLedge* l = ledges->Get (ledidx);
      csVector3 prevpos, currpos;
      // y position doesn't change
      currpos.y = l->GetYPosition ();
      // we skip checking point index 0 but still start at 0 so we update prevpos
      for (size_t pidx = 0; pidx < l->GetPointCount (); pidx++)
      {
        {
          csVector2 proxpos (l->GetPoint (pidx));
          currpos.x = proxpos.x;
          currpos.z = proxpos.y;
        }
        if (pidx != 0 && TryGrabLedge (prevpos, currpos, lefthand, righthand))
        {
          // so we grabbed the ledge! store these for later
          c_ledge_id = ledidx;
          c_ledge_point_id = pidx;
          return;
        }
        prevpos = currpos;
      }
    }
  }
}
bool celPcGrab::TryGrabLedge (const csVector3 &left, const csVector3 &right, const csVector3 &lhand, const csVector3 &rhand)
{
  // check if we're actually cool to grab the ledge
  const csVector3 edgediff (right - left);
  if (!(ShouldGrabLedge (left, edgediff, lhand) && ShouldGrabLedge (left, edgediff, rhand)))
    return false;

  // so we are... now find closest points on the segment to both hands
  csVector3 closehands[2] = {
    ClosestOnSegment (left, edgediff, lhand),
    ClosestOnSegment (left, edgediff, rhand) };
  // the snap centre point will be the centre point between both those points
  csVector3 snap_centre ((closehands[0] + closehands[1]) / 2), snap_dir = (right - left);
  // find perpendicular vector to get forward direction
  // ... safe to assume 2D vectors since y value of ledges doesn't change among its points
  CS::Swap (snap_dir.x, snap_dir.z);

  // we use a trick to find which direction is ledge forward from player
  // ... see which distance is small when add direction to player offset
  csVector3 centrehand ((lhand + rhand) / 2);
  if ((snap_centre + snap_dir - centrehand).SquaredNorm () > (snap_centre - snap_dir - centrehand).SquaredNorm ())
    snap_dir = -snap_dir;

  // now apply the calculated transform
  csReversibleTransform snap;
  snap.LookAt (snap_dir, csVector3 (0, 1, 0));
  // we minus the local hand offsets to get real position for actual origin
  snap.SetOrigin (snap_centre - snap.This2OtherRelative (csVector3 (0, left_hand_rel.y, left_hand_rel.z)));
  movable->SetTransform (snap);

  // setup states
  jump->Freeze (true);
  currstate = HANG;
  return true;
}

// this is defunct
static csVector3 FindClosestPointOnEdge (const csVector3 &start, const csVector3 &edgediff, const csVector3 &hand)
{
  return start + ((hand - start) >> edgediff);
}
// is this defunct also?
void celPcGrab::AttemptGrab ()
{
  return;
  if (!FindInterfaces ())
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
      ClosestOnSegment (leftcorn, edgediff, currhands[0]),
      ClosestOnSegment (leftcorn, edgediff, currhands[1]) };
    float closest [2] = {
      (handspos[0] - currhands[0]).SquaredNorm (),
      (handspos[1] - currhands[1]).SquaredNorm () };
    printf ("%f %f", sqrt(closest[0]), sqrt(closest[1]));
    // find the hand closest to the edge
    // if it beats our previous record then we keep it
    if (LiesOnSegment (leftcorn, edgediff, closest[0]) && closest[0] < closest[1])  // left hand is closest!
    {
      if (closest[0] < fromedge_sq)
      {
        hand = 0;
        fromedge_sq = closest[0];
        nearposedge = handspos[0];
        printf ("\t 0");
      }
    }
    else if (LiesOnSegment (leftcorn, edgediff, closest[1]))
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
      tar->SetPosition (FindClosestPointOnEdge (leftcorn, edgediff, currhands[0]));
      tarother->SetPosition (FindClosestPointOnEdge (leftcorn, edgediff, currhands[1]));
    }
    else if (hand == 1 && (fromedge_sq - closest[1]) < EPSILON)
    {
      tar->SetPosition (FindClosestPointOnEdge (leftcorn, edgediff, currhands[1]));
      tarother->SetPosition (FindClosestPointOnEdge (leftcorn, edgediff, currhands[0]));
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
  //mov->SetTransform (movable->GetFullTransform ());
  //mov->SetPosition (movable->GetPosition () + csVector3 (0.2, 1.4, 0));
  csVector3 lefthand = movable->GetFullTransform ().This2Other (csVector3 (0.2f, 1.4f, -0.2f));
  mov->SetPosition (lefthand);
  csVector3 righthand = movable->GetFullTransform ().This2Other (csVector3 (-0.2f, 1.4f, -0.2f));
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

  csReversibleTransform blaa (movable->GetFullTransform ());
  cent.y = blaa.GetOrigin ().y;
  csVector3 flatdiff (blaa.GetOrigin () - cent);
  blaa.LookAt (flatdiff, csVector3 (0, 1, 0));
  //if (flatdiff.SquaredNorm () < 4.0f)
    //movable->SetTransform (blaa);
}
