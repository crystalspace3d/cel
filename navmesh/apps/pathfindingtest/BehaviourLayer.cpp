#include <crystalspace.h>
#include <physicallayer/entity.h>
#include <physicallayer/propclas.h>
#include <propclass/linmove.h>
#include <propclass/prop.h>
#include "BehaviourLayer.h"



/*
 * BehaviourLayer
 */

BehaviourLayer::BehaviourLayer (iCelPlLayer* physicalLayer, iObjectRegistry* objectRegistry) 
    : scfImplementationType (this)
{
  this->physicalLayer = physicalLayer;
  this->objectRegistry = objectRegistry;
}

BehaviourLayer::~BehaviourLayer ()
{
}

iCelBehaviour* BehaviourLayer::CreateBehaviour (iCelEntity* entity, const char* name)
{
  csRef<iCelBehaviour> behaviour;
  if (!strcmp(name, "levelBehaviour"))
  {
    behaviour = new BehaviourLevel(entity, this, physicalLayer);
  }
  else if (!strcmp(name, "playerBehaviour"))
  {
    behaviour = new BehaviourPlayer(entity, this, physicalLayer, objectRegistry);
  }
  else if (!strcmp(name, "boxBehaviour"))
  {
    behaviour = new BehaviourBox(entity, this, physicalLayer, objectRegistry);
  }  

  if (behaviour)
  {
    entity->SetBehaviour(behaviour);
  }
  return behaviour;
}

const char* BehaviourLayer::GetName () const
{
  return "behaviourlayer";
}

void BehaviourLayer::SetNavStruct(iCelHNavStruct* navStruct)
{
  this->navStruct = navStruct;
}

iCelHNavStruct* BehaviourLayer::GetNavStruct() const
{ 
  return navStruct;
}

void BehaviourLayer::SetPath (iCelHPath* path)
{
  this->path = path;
}

iCelHPath* BehaviourLayer::GetPath () const
{
  return path;
}



/*
 * BehaviourCommon
 */

BehaviourCommon::BehaviourCommon (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
                                  iCelPlLayer* physicalLayer) 
    : scfImplementationType (this)
{
  this->entity = entity;
  this->behaviourLayer = behaviourLayer;
  this->physicalLayer = physicalLayer;
}

BehaviourCommon::~BehaviourCommon ()
{
}

bool BehaviourCommon::SendMessage (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                                   iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start(arg, params);
  bool rc = SendMessageV(msg_id, pc, ret, params, arg);
  va_end(arg);
  return rc;
}

bool BehaviourCommon::SendMessageV (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                                    iCelParameterBlock* params, va_list arg)
{
  csStringID id = physicalLayer->FetchStringID(msg_id);
  return SendMessage(id, pc, ret, params, arg);
}

bool BehaviourCommon::SendMessage (csStringID, iCelPropertyClass*, celData&, iCelParameterBlock*, va_list)
{
  return false;
}

void* BehaviourCommon::GetInternalObject ()
{ 
  return 0;
}

iCelBlLayer* BehaviourCommon::GetBehaviourLayer () const
{
  return behaviourLayer;
}



/*
 * BehaviourLevel
 */

bool BehaviourLevel::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                                  iCelParameterBlock* params, va_list arg)
{
  return BehaviourCommon::SendMessage(msg_id, pc, ret, params, arg);
}

const char* BehaviourLevel::GetName () const
{ 
  return "levelBehaviour"; 
}



/*
 * BehaviourPlayer
 */

BehaviourPlayer::BehaviourPlayer (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
                                  iCelPlLayer* physicalLayer, iObjectRegistry* objectRegistry)
    : BehaviourCommon (entity, behaviourLayer, physicalLayer)
{
  this->objectRegistry = objectRegistry;

  id_pccommandinput_forward1 = physicalLayer->FetchStringID("pccommandinput_forward1");
  id_pccommandinput_forward0 = physicalLayer->FetchStringID("pccommandinput_forward0");
  id_pccommandinput_backward1 = physicalLayer->FetchStringID("pccommandinput_backward1");
  id_pccommandinput_backward0 = physicalLayer->FetchStringID("pccommandinput_backward0");
  id_pccommandinput_rotateleft1 = physicalLayer->FetchStringID("pccommandinput_rotateleft1");
  id_pccommandinput_rotateleft0 = physicalLayer->FetchStringID("pccommandinput_rotateleft0");
  id_pccommandinput_rotateright1 = physicalLayer->FetchStringID("pccommandinput_rotateright1");
  id_pccommandinput_rotateright0 = physicalLayer->FetchStringID("pccommandinput_rotateright0");
  id_pccommandinput_cammode1 = physicalLayer->FetchStringID("pccommandinput_cammode1");
  id_pccommandinput_drop1 = physicalLayer->FetchStringID("pccommandinput_drop1");
  id_pccommandinput_setposition = physicalLayer->FetchStringID("pccommandinput_setposition0");

  id_pcinventory_addchild = physicalLayer->FetchStringID("pcinventory_addchild");
  id_pcinventory_removechild = physicalLayer->FetchStringID("pcinventory_removechild");

  id_pcmover_arrived = physicalLayer->FetchStringID("pcmover_arrived");
}

void BehaviourPlayer::GetLinearMovement ()
{
  if (!pcLinearMovement)
  {
    pcLinearMovement = CEL_QUERY_PROPCLASS_ENT(entity, iPcLinearMovement);
  }
}

void BehaviourPlayer::GetActorMove ()
{
  if (!pcActorMove)
  {
    pcActorMove = CEL_QUERY_PROPCLASS_ENT(entity, iPcActorMove);
  }
}

void BehaviourPlayer::GetMover ()
{
  if (!pcMover)
  {
    pcMover = CEL_QUERY_PROPCLASS_ENT(entity, iPcMover);
  }
}

void BehaviourPlayer::GetInventory ()
{
  if (!pcInventory)
  {
    pcInventory = CEL_QUERY_PROPCLASS_ENT(entity, iPcInventory);
  }
}

void BehaviourPlayer::GetMesh ()
{
  if (!pcMesh)
  {
    pcMesh = CEL_QUERY_PROPCLASS_ENT(entity, iPcMesh);
  }
}

void BehaviourPlayer::ShowInventory ()
{
  size_t count = pcInventory->GetEntityCount();
  size_t i;
  for (i = 0 ; i < count ; i++)
  {
    iCelEntity* child = pcInventory->GetEntity(i);
    printf("  child %zu is '%s'\n", i, child->GetName());
  }
}

void BehaviourPlayer::Drop ()
{
  GetInventory();
  size_t count = pcInventory->GetEntityCount();
  if (count <= 0)
  {
    printf("Inventory is empty!\n");
    return;
  }
  csRef<iCelEntity> child = pcInventory->GetEntity(0);
  pcInventory->RemoveEntity(child);
  csRef<iPcLinearMovement> pcLinMove = CEL_QUERY_PROPCLASS_ENT(child, iPcLinearMovement);
  if (pcLinMove)
  {
    GetMesh ();
    // Now we get current position and orientation from player mesh and from
    // that we calculate a spot in front of the player where we will drop down
    // the item.
    csVector3 pos = pcMesh->GetMesh()->GetMovable()->GetTransform().This2Other(csVector3(0, 2, -2));
    csRef<iSector> sector = pcMesh->GetMesh()->GetMovable()->GetSectors()->Get(0);
    pcLinMove->SetPosition(pos, 0, sector);
    pcLinMove->SetBodyVelocity(csVector3 (0, .1f, 0));
    csRef<iPcMesh> pcMeshChild = CEL_QUERY_PROPCLASS_ENT(child, iPcMesh);
    if (pcMeshChild)
    {
      pcMeshChild->Show();
    }
  }
}

bool BehaviourPlayer::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                                   iCelParameterBlock* params, va_list arg)
{
  GetActorMove ();

  if (msg_id == id_pccommandinput_forward1)
  {
    pcActorMove->Forward(true);
  }
  else if (msg_id == id_pccommandinput_forward0)
  {
    pcActorMove->Forward(false);
  }
  else if (msg_id == id_pccommandinput_backward1)
  {
    pcActorMove->Backward(true);
  }
  else if (msg_id == id_pccommandinput_backward0)
  {
    pcActorMove->Backward(false);
  }
  else if (msg_id == id_pccommandinput_rotateleft1)
  {
    pcActorMove->RotateLeft(true);
  }
  else if (msg_id == id_pccommandinput_rotateleft0)
  {
    pcActorMove->RotateLeft(false);
  }
  else if (msg_id == id_pccommandinput_rotateright1)
  {
    pcActorMove->RotateRight(true);
  }
  else if (msg_id == id_pccommandinput_rotateright0)
  {
    pcActorMove->RotateRight(false);
  }
  else if (msg_id == id_pccommandinput_cammode1)
  {
    pcActorMove->ToggleCameraMode();
  }
  else if (msg_id == id_pccommandinput_drop1)
  {
    Drop();
  }
  else if (msg_id == id_pccommandinput_setposition)
  {
    csRef<iCelHNavStruct> navStruct = behaviourLayer->GetNavStruct();
    if (navStruct)
    {
      GetLinearMovement();
      csVector3 origin = pcLinearMovement->GetPosition();
      csRef<iSector> originSector = pcLinearMovement->GetSector();

      csRef<iMouseDriver> mouse = csQueryRegistry<iMouseDriver>(objectRegistry);
      if (!mouse)
      {
        return false;
      }
      csVector2 screenPoint;
      screenPoint.x = mouse->GetLastX();
      screenPoint.y = mouse->GetLastY();

      
      csRef<iPcCamera> pcCamera = CEL_QUERY_PROPCLASS_ENT(entity, iPcCamera);
      csRef<iCamera> camera = pcCamera->GetCamera();
      csScreenTargetResult st = csEngineTools::FindScreenTarget(screenPoint, 10000.0f, camera);
      csRef<iSectorList> sectorList = st.mesh->GetMovable()->GetSectors();
      if (sectorList->GetCount() != 1)
      {
        return false;
      }
      csVector3 destination = st.isect;
      csRef<iSector> destinationSector = sectorList->Get(0);

      path = navStruct->ShortestPath(origin, originSector, destination, destinationSector);
      behaviourLayer->SetPath(path);
      GetMover();
      csRef<iMapNode> node = path->Next();
      pcMover->MoveTo(node->GetSector(), node->GetPosition(), 0.005f);
    }
  }
  else if (msg_id == id_pcinventory_addchild)
  {
    GetInventory();
    printf ("Got a new object! Objects in inventory:\n");
    ShowInventory();
  }
  else if (msg_id == id_pcinventory_removechild)
  {
    GetInventory();
    printf ("Object removed from inventory! Objects in inventory:\n");
    ShowInventory();
  }
  else if (msg_id == id_pcmover_arrived)
  {
    if (path)
    {
      if (path->HasNext())
      {
        GetMover();        
        csRef<iMapNode> node = path->Next();
        pcMover->MoveTo(node->GetSector(), node->GetPosition(), 0.005f);
      }
    }
  }
  else
  {
    return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);;
  }

  return true;
}

const char* BehaviourPlayer::GetName () const
{
  return "playerBehaviour";
}



/*
 * BehaviourBox
 */

BehaviourBox::BehaviourBox (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
                                  iCelPlLayer* physicalLayer, iObjectRegistry* objectRegistry)
    : BehaviourCommon (entity, behaviourLayer, physicalLayer), point1(24.8f, 1.0f, 13.7f), 
      point2(24.8f, 1.0f, 21.3f)
{
  id_box_arrived = physicalLayer->FetchStringID("pcmover_arrived");
  oneIsCurrent = true;

  csRef<iEngine> engine = csLoadPluginCheck<iEngine>(objectRegistry, "crystalspace.engine.3d");
  point1Sector = engine->FindSector("interior");
  point2Sector = point1Sector;
}

void BehaviourBox::GetLinearMovement ()
{
  if (!pcLinearMovement)
  {
    pcLinearMovement = CEL_QUERY_PROPCLASS_ENT(entity, iPcLinearMovement);
  }
}

void BehaviourBox::GetActorMove ()
{
  if (!pcActorMove)
  {
    pcActorMove = CEL_QUERY_PROPCLASS_ENT(entity, iPcActorMove);
  }
}

void BehaviourBox::GetMover ()
{
  if (!pcMover)
  {
    pcMover = CEL_QUERY_PROPCLASS_ENT(entity, iPcMover);
  }
}

void BehaviourBox::GetMesh ()
{
  if (!pcMesh)
  {
    pcMesh = CEL_QUERY_PROPCLASS_ENT(entity, iPcMesh);
  }
}

bool BehaviourBox::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                                   iCelParameterBlock* params, va_list arg)
{
 if (msg_id == id_box_arrived)
  {
    GetMover();
    if (oneIsCurrent)
    {
      pcMover->MoveTo(point2Sector, point2, 0.005f);
    }
    else
    {
      pcMover->MoveTo(point1Sector, point1, 0.005f);
    }
    oneIsCurrent = !oneIsCurrent;
  }
  else
  {
    return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);;
  }

  return true;
}

const char* BehaviourBox::GetName () const
{
  return "boxBehaviour";
}
