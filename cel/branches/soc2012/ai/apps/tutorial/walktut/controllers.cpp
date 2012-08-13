#include <crystalspace.h>

#include <physicallayer/entity.h>
#include <physicallayer/propclas.h>
#include <propclass/linmove.h>
#include <propclass/prop.h>

#include "controllers.h"

CEL_IMPLEMENT_FACTORY (PlayerController, "tutorial.playercontrol")
CEL_IMPLEMENT_FACTORY (BadOneController, "tutorial.badonecontrol")

//----------------------------------------------------------------------

PropertyHolder celPcPlayerController::propinfo;

csStringID celPcPlayerController::id_child = csInvalidStringID;

celPcPlayerController::celPcPlayerController (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  if (id_child == csInvalidStringID)
  {
    id_child = pl->FetchStringID ("child");
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("tutorial.player.action.");
    AddAction (action_drop, "Drop");
    AddAction (action_add, "Add");
    AddAction (action_delete, "Delete");
    AddAction (action_pickup, "PickUp");
  }

  // For properties.
  propinfo.SetCount (0);
}

celPcPlayerController::~celPcPlayerController ()
{
}

bool celPcPlayerController::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_drop:
      Drop ();
      return true;
    case action_pickup:
      {
	csString childName;
	if (!Fetch (childName, params, id_child)) return false;
        PickUp (childName);
      }
      return true;
    case action_add:
      GetInventory ();
      csPrintf ("Got a new object! Objects in inventory:\n");
      ShowInventory ();
      return true;
    case action_delete:
      GetInventory ();
      csPrintf ("Object removed from inventory! Objects in inventory:\n");
      ShowInventory ();
      return true;
    default:
      return false;
  }
  return false;
}

void celPcPlayerController::GetActorMove ()
{
  if (!pcactormove)
  {
    pcactormove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  }
}

void celPcPlayerController::GetMesh ()
{
  if (!pcmesh)
  {
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  }
}

void celPcPlayerController::GetInventory ()
{
  if (!pcinventory)
  {
    pcinventory = celQueryPropertyClassEntity<iPcInventory> (entity);
  }
}

void celPcPlayerController::ShowInventory ()
{
  size_t count = pcinventory->GetEntityCount ();
  size_t i;
  for (i = 0 ; i < count ; i++)
  {
    iCelEntity* child = pcinventory->GetEntity (i);
    csPrintf ("  child %zu is '%s'\n", i, child->GetName ());
  }
}

void celPcPlayerController::PickUp (const char* childName)
{
  GetInventory ();
  iCelEntity* child = pl->FindEntity (childName);
  if (!child)
  {
    printf ("Error! Can't find entity with name '%s'!\n", childName);
    fflush (stdout);
    return;
  }
  pcinventory->AddEntity (child);
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (child);
  if (pcmesh) pcmesh->Hide ();
}

void celPcPlayerController::Drop ()
{
  GetInventory ();
  size_t count = pcinventory->GetEntityCount ();
  if (count <= 0)
  {
    csPrintf ("Inventory is empty!\n");
    return;
  }
  iCelEntity* child = pcinventory->GetEntity (0);
  pcinventory->RemoveEntity (child);
  csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (child);
  if (pclinmove)
  {
    GetMesh ();
    // Now we get current position and orientation from player mesh and from
    // that we calculate a spot in front of the player where we will drop down
    // the item.
    csVector3 pos = pcmesh->GetMesh ()->GetMovable ()->GetTransform ()
      .This2Other (csVector3 (0, 2, -2));
    iSector* sector = pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0);
    pclinmove->SetPosition (pos, 0, sector);
    pclinmove->SetBodyVelocity (csVector3 (0, .1f, 0));
    csRef<iPcMesh> pcmesh_child = celQueryPropertyClassEntity<iPcMesh> (child);
    if (pcmesh_child) pcmesh_child->Show ();
  }
}

//----------------------------------------------------------------------

static bool GetPropVector (iPcProperties* pcprop, const char* prefix, int i, csVector3& v)
{
  csString propname = prefix;
  propname += i;
  size_t idx = pcprop->GetPropertyIndex (propname);
  if (idx == csArrayItemNotFound) return false;
  if (!pcprop->GetPropertyVector (idx, v)) return false;
  return true;
}

static bool GetPropLong (iPcProperties* pcprop, const char* prefix, int i, long& l)
{
  csString propname = prefix;
  propname += i;
  size_t idx = pcprop->GetPropertyIndex (propname);
  if (idx == csArrayItemNotFound) return false;
  l = pcprop->GetPropertyLong (idx);
  return true;
}

PropertyHolder celPcBadOneController::propinfo;

celPcBadOneController::celPcBadOneController (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("tutorial.badone.action.");
    AddAction (action_restart, "Restart");
  }

  // For properties.
  propinfo.SetCount (0);
}

celPcBadOneController::~celPcBadOneController ()
{
}

bool celPcBadOneController::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_restart:
      Restart ();
      return true;
    default:
      return false;
  }
  return false;
}

void celPcBadOneController::ReadPath ()
{
  if (path) return;
  csRef<iPcProperties> pcprop = celQueryPropertyClassEntity<iPcProperties> (entity);

  // Count the number of points we have.
  int count = 0;
  for (;;)
  {
    csVector3 v;
    if (!GetPropVector (pcprop, "pos", count+1, v)) break;
    count++;
  }

  path.AttachNew (new csPath(count));

  int i = 0;
  long totaltime = 0;
  for (i = 0 ; i < count ; i++)
  {
    csVector3 pos, forward, up;
    GetPropVector (pcprop, "pos", i+1, pos);
    if (!GetPropVector (pcprop, "forward", i+1, forward)) forward.Set (1, 0, 0);
    if (!GetPropVector (pcprop, "up", i+1, up)) up.Set (0, 1, 0);
    long time = 0;
    if (!GetPropLong (pcprop, "time", i+1, time)) time = 1000;
    path->SetPositionVector (i, pos);
    path->SetUpVector (i, up);
    path->SetForwardVector (i, forward);
    path->SetTime (i, float (totaltime) / 1000.0);
    totaltime += time;
  }

  csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  if (pclinmove)
  {
    for (i = 0 ; i < count ; i++)
    {
      pclinmove->SetPathAction (i, "default");
    }
  }
}

void celPcBadOneController::Restart ()
{
  printf ("Restart!\n"); fflush (stdout);
  ReadPath ();
  csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  if (pclinmove)
  {
    pclinmove->SetPath (path);
    pclinmove->SetPathTime (0);
  }
}

//----------------------------------------------------------------------

