/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Christian Van Brussel

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

#include "physicallayer/pl.h"
#include "physicallayer/persist.h"
#include "physicallayer/entity.h"
#include "include/celtool/persisthelper.h"
#include "propclass/actormove.h"
#include "netmanagers.h"
#include "networktypes.h"

#define LINK_PERIOD 40

celPersistenceResult SetEntityPersistentData (iCelEntity* entity, 
        csTicks data_time, iCelPersistentDataList* data_list, 
        celPersistenceType persistence_type)
{
  celPersistenceResult persist_code = CEL_PERSIST_RESULT_OK;

  csRef<iCelDataBuffer> databuf;
  csString pc_name, pc_tag;
  iCelPropertyClass* pc;
  size_t i, count = data_list->GetCount ();
  for (i = 0; i < count; i++)
  {
    data_list->GetPersistentData (i, databuf, pc_name, pc_tag);
    if (pc_tag)
      pc = entity->GetPropertyClassList ()->FindByNameAndTag (pc_name, pc_tag);
    else
      pc = entity->GetPropertyClassList ()->FindByName (pc_name);

    if (pc)
    {
      celPersistenceResult pc_persist_code = pc->SetPersistentData (data_time, databuf, persistence_type);
      if (pc_persist_code > persist_code)
	persist_code = pc_persist_code;
    }
  }

  return persist_code;
}

/*****************************
 * GameFactoryManager
 ****************************/

GameFactoryManager::GameFactoryManager (NetTest* nettest, iCelGameFactory* factory, csString level_path, csString level_file)
{
  GameFactoryManager::nettest = nettest;
  GameFactoryManager::factory = factory;
  GameFactoryManager::level_path = level_path;
  GameFactoryManager::level_file = level_file;
  pl = CS_QUERY_REGISTRY (nettest->GetObjectRegistry (), iCelPlLayer);
  connection_state = "No connection";
  server_manager = 0;
  client_manager = 0;
}

GameFactoryManager::~GameFactoryManager ()
{
  delete server_manager;
  delete client_manager;
}

void GameFactoryManager::ServerNetworkStateChanged (celServerNetworkState new_state, 
        celServerNetworkState previous_state, csString reason)
{
  printf ("The state of the connection has changed from %d to %d, reason %s\n", previous_state, new_state, reason.GetData ());

  switch (new_state)
  {
  case CEL_NET_SERVER_UNDEF:
    connection_state = "Invalid connection state";
    break;
  case CEL_NET_SERVER_NOT_CONNECTED:
    connection_state = "Not connected";
    break;
  case CEL_NET_SERVER_INVALID_HOSTNAME:
    connection_state.Format ("Invalid server hostname: ""%s""", reason.GetData ());
    break;
  case CEL_NET_SERVER_TRYING_CONNECTION:
    connection_state = "Trying to connect";
    break;
  case CEL_NET_SERVER_CONNECTING:
    connection_state = "Connected, sending initialization data";
    break;
  case CEL_NET_SERVER_REJECTED_BAD_GAME:
    connection_state.Format ("Rejected: the client is playing game ""%s"" while the server is playing game ""%s""", factory->GetGameName ().GetData (), reason.GetData ());
    break;
  case CEL_NET_SERVER_REJECTED_BAD_PROTOCOL:
    connection_state.Format ("Rejected: the client has protocol version ""%s"" while the server has version ""%s""", factory->GetProtocolVersion ().GetData (), reason.GetData ());
    break;
  case CEL_NET_SERVER_REJECTED_BAD_PASSWORD:
    connection_state = "Rejected: invalid password";
    break;
  case CEL_NET_SERVER_REJECTED_SINGLEPLAYER:
    connection_state = "Rejected: the server is playing a single player game";
    break;
  case CEL_NET_SERVER_REJECTED_UNAUTHORIZED:
    connection_state.Format ("Rejected: the player is not authorized to join the game. Reason: ""%s""", reason.GetData ());
    break;
  case CEL_NET_SERVER_REJECTED_MAX_PLAYERS:
    connection_state = "Rejected: server full";
    break;
  case CEL_NET_SERVER_LOADING_DATA:
    connection_state = "The client is connected and is loading the level";
    break;
  case CEL_NET_SERVER_PLAYING:
    connection_state = "Playing";
    break;
  case CEL_NET_SERVER_DISCONNECTED:
    connection_state = "Disconnected";
    break;
  case CEL_NET_SERVER_KICKED:
    connection_state.Format ("Player kicked from the game. Reason: ""%s""", reason.GetData ());
    break;
  case CEL_NET_SERVER_UNREACHABLE:
    connection_state = "Server unreachable";
    break;
  case CEL_NET_SERVER_LOST:
    connection_state = "Server lost";
    break;
  default: 
    connection_state = "Invalid connection state";
  }

  printf ("New state: %s\n", connection_state.GetData ());
  fflush (stdout); 
}

bool GameFactoryManager::InitClient (iCelGame* game)
{
  GameFactoryManager::game = game;

  // load level
  if (!game->IsServerAvailable ())
  {
    // get the path and file to the level to be loaded
    celGameInfo* game_info = game->GetGameInfo ();
    csString path = csStrNew (game_info->custom_data->GetString ()->GetData ());
    csString file = csStrNew (game_info->custom_data->GetString ()->GetData ());

    // TODO: load the level in another tread
    if (!nettest->CreateRoom (path, file))
      return false;

    // add a camera so that the level is really loaded and ready to be displayed
    csRef<iCelEntity> default_camera = nettest->CreateDefaultCamera ("defaultcamera");
    if (!default_camera)
      return false;
  }

  // register client manager
  client_manager = new GameClientManager (this);
  game->GetGameClient ()->RegisterClientManager (client_manager);

  // we are ready to play
  game->GetGameClient ()->SetReady ();

  return true;
}

bool GameFactoryManager::InitServer (iCelGame* game)
{
  GameFactoryManager::game = game;

  // load level
  if (!nettest->CreateRoom (level_path, level_file)) return false;

  // register server manager
  server_manager = new GameServerManager (this);
  game->GetGameServer ()->RegisterServerManager (server_manager);

  // Set client timeout at 10 seconds
  game->GetGameServer ()->SetClientTimeOut (10000);

  // add a npc character
  server_manager->npc_entity = nettest->CreateActorNPC ("npcchar", "", csVector3 (0,0,50));
  if (!server_manager->npc_entity)
    return false;

  csRef<iPcActorMove> pcactormove = CEL_QUERY_PROPCLASS_ENT (server_manager->npc_entity, 
  	iPcActorMove);
  pcactormove->Forward(true);

  return true;
}

//---------------------------------------------------------------------------

/*****************************
 * GameServerManager
 ****************************/

GameServerManager::GameServerManager (GameFactoryManager* factory)
{
  GameServerManager:: factory = factory;
  pl = CS_QUERY_REGISTRY (factory->nettest->GetObjectRegistry (), iCelPlLayer);
}

GameServerManager::~GameServerManager ()
{

}

bool GameServerManager::AuthorizePlayer (celPlayer* player, csString &reason)
{
  // check if the player name is already used
  iCelPlayerList* player_list = factory->GetServer ()->GetPlayerList ();
  csString temp_name = player->player_name;
  int index = 2;
  while (true)
  {
    bool name_found = false;
    size_t i = 0;
    for ( ; i < player_list->GetCount (); i++)
      if (player_list->Get (i)->player_name.Compare (temp_name))
      {
	name_found = true;
	break;
      }

    if (name_found)
      temp_name.Format ("%s%d", player->player_name.GetData (), index++);
    else break;
  }
  player->player_name = temp_name;

  //reason = "Banned from the game";
  //return false;
  return true;
}

celPlayer* GameServerManager::ValidatePlayerUpdate (celPlayer* previous_player_data)
{
  return previous_player_data;
}

void GameServerManager::PlayerNetworkStateChanged (celPlayer* player, 
        celPlayerNetworkState new_state, 
        celPlayerNetworkState previous_state)
{
  printf ("The state of the connection has changed for player %d from %d to %d\n", 
	  player->player_id, previous_state, new_state);
  fflush (stdout);

  // create a message to send to other players
  csString comment;
  switch (new_state)
  {
  case CEL_NET_PLAYER_CONNECTING:
    comment.Format ("Player %s is connecting", player->player_name.GetData ());
    break;
  case CEL_NET_PLAYER_PLAYING:
    comment.Format ("Player %s is connected", player->player_name.GetData ());
    break;
  case CEL_NET_PLAYER_DISCONNECTED:
    comment.Format ("Player %s has left the game", player->player_name.GetData ());
    break;
  case CEL_NET_PLAYER_UNREACHABLE:
    comment.Format ("Player %s is unreachable", player->player_name.GetData ());
    break;
  case CEL_NET_PLAYER_LOST:
    comment.Format ("The connection to player %s is lost", player->player_name.GetData ());
    break;
  default:
    comment = "Invalid new connection state";
    break;
  }

  // send message to all other players
  celServerEventData event;
  event.event_type = SERVER_EVENT_MESSAGE;
  event.event_time = 1000;
  event.event_data = pl->CreateDataBuffer (0);
  event.event_data->Add (comment);
  iCelPlayerList* player_list = factory->GetServer ()->GetPlayerList ();
  size_t i = 0;
  for ( ; i < player_list->GetCount (); i++)
  {
    celPlayer* other_player = player_list->Get (i);
    if (other_player->player_id != player->player_id)
      factory->GetServer ()->LaunchServerEvent (other_player, event);
  }


  // if it is a new player
  if (new_state == CEL_NET_PLAYER_PLAYING)
  {
    csString entity_name;
    csBitArray bit_array;

    // create an avatar for the new player
    bool is_camera_actor = false;
    if (factory->game->IsClientAvailable () && *factory->GetClient()->GetPlayer () == *player)
      is_camera_actor = true;

    csRef<iCelEntity> entity;
    entity_name = "player" + player->player_id;
    if (is_camera_actor)
      entity = factory->nettest->CreateActor (entity_name, "", csVector3 (0,0,50));
    else entity = factory->nettest->CreateActorNPC (entity_name, "", csVector3 (0,0,50));

    celNetworkLinkData player_link_data (NETWORK_LINK_ACTOR, entity, bit_array, LINK_PERIOD);
    if (!is_camera_actor)
      factory->GetServer ()->SetNetworkLink (player, player_link_data, true);

    // add a network link to the npc character
    celNetworkLinkData npc_link_data (NETWORK_LINK_ACTOR, npc_entity, bit_array, LINK_PERIOD);
    factory->GetServer()->SetNetworkLink (player, npc_link_data, false);

    // add a network link to the avatars of the other players
    pl = CS_QUERY_REGISTRY (factory->nettest->GetObjectRegistry (), iCelPlLayer);
    iCelPlayerList* player_list = factory->GetServer ()->GetPlayerList ();
    size_t i = 0;
    for ( ; i < player_list->GetCount (); i++)
    {
      celPlayer* other_player = player_list->Get (i);
      if (factory->GetServer ()->GetPlayerState (other_player) == CEL_NET_PLAYER_PLAYING
	  && other_player->player_id != player->player_id)
      {
	entity_name = "player" + other_player->player_id;
	iCelEntity* entity = pl->FindEntity (entity_name);
	CS_ASSERT (entity);
	celNetworkLinkData link_data (NETWORK_LINK_ACTOR, entity, bit_array, LINK_PERIOD);
	factory->GetServer ()->SetNetworkLink (player, link_data, false);
	factory->GetServer ()->SetNetworkLink (other_player, player_link_data, false);
      }
    }
  }

  // if it is player leaving
  if (new_state == CEL_NET_PLAYER_DISCONNECTED || new_state == CEL_NET_PLAYER_LOST)
  {
    // remove network links to this player
    csString entity_name = "player" + player->player_id;
    iCelEntity* entity = pl->FindEntity (entity_name);
    iCelPlayerList* player_list = factory->GetServer ()->GetPlayerList ();
    size_t i = 0;
    for ( ; i < player_list->GetCount (); i++)
    {
      celPlayer* other_player = player_list->Get (i);
      if (other_player->player_id != player->player_id)
	factory->GetServer ()->RemoveNetworkLink (entity, other_player);
    }

    // remove entity
    pl->RemoveEntity (entity);
  }
}

void GameServerManager::HandleClientEvent (celPlayer* player, 
        celClientEventType event_type, csTicks event_time, 
        iCelDataBuffer* event_data)
{
  printf ("Client event received from player %d: type: %d time: %d\n", player->player_id, event_type, event_time);
  printf ("%s", event_data->GetDebugInfo ().GetData ());
  fflush (stdout);

  if (event_type == CLIENT_EVENT_FORWARD_TEST)
  {
    // forward to all players
    celServerEventData event;
    event.event_type = SERVER_EVENT_FORWARDED_TEST;
    event.event_time = 1000;
    event.event_data = pl->CreateDataBuffer (0);
    event_data->Reset ();
    size_t i = 0;
    for ( ; i < event_data->GetDataCount (); i++)
      *event.event_data->AddData () = *event_data->GetData ();
    iCelPlayerList* player_list = factory->GetServer ()->GetPlayerList ();
    for (i = 0; i < player_list->GetCount (); i++)
      factory->GetServer ()->LaunchServerEvent (player_list->Get (i), event);
  }

  if (event_type == CLIENT_EVENT_KICKING_TEST)
  {
    // the player is asking to be kicked
    factory->GetServer ()->KickPlayer (player, "You just asked for it");

    // send a message to all players
    celServerEventData event;
    event.event_type = SERVER_EVENT_MESSAGE;
    event.event_time = 1000;
    event.event_data = pl->CreateDataBuffer (0);
    csString message;
    message.Format ("Player %s has been kicked from the game", player->player_name.GetData ());
    event.event_data->Add (message);
    iCelPlayerList* player_list = factory->GetServer ()->GetPlayerList ();
    size_t i = 0;
    for ( ; i < player_list->GetCount (); i++)
    {
      celPlayer* other_player = player_list->Get (i);
      if (other_player->player_id != player->player_id)
	factory->GetServer ()->LaunchServerEvent (other_player, event);
    }
  }

  if (event_type == CLIENT_EVENT_KICKING_ALL_TEST)
  {
    // kicking all players
    csString reason;
    reason.Format ("Player %s asked for it", player->player_name.GetData ());
    iCelPlayerList* player_list = factory->GetServer ()->GetPlayerList ();
    size_t i = 0;
    for ( ; i < player_list->GetCount (); i++)
      factory->GetServer ()->KickPlayer(player_list->Get (i), reason);
  }
}

void GameServerManager::PersistenceProblem (celPlayer* player, iCelEntity* entity,
	iCelPropertyClass* pc, celPersistenceResult persist_code)
{
  printf ("Persistence problem encountered while updating data from player %s\n", player->player_name.GetData ());
  printf ("Problem was for entity %d, pc %s:%s. Persistence result: %d\n", entity->GetID (), pc->GetName (), pc->GetTag (), persist_code);
  fflush (stdout);
}

void GameServerManager::ServerEnd ()
{

}

//---------------------------------------------------------------------------

/*****************************
 * GameClientManager
 ****************************/

GameClientManager::GameClientManager (GameFactoryManager* factory)
{
  GameClientManager::factory = factory;
  pl = CS_QUERY_REGISTRY (factory->nettest->GetObjectRegistry (), iCelPlLayer);
  entity_counter = 0;
}

GameClientManager::~GameClientManager ()
{
}

void GameClientManager::PlayerValidated (celPlayer* player)

{
  printf ("Player data validated:\n");
  player->PrintDebugInfo ();
  fflush (stdout); 
}

void GameClientManager::HandleServerEvent (celServerEventType event_type, 
        csTicks event_time, iCelDataBuffer* event_data)

{
  if (event_type == SERVER_EVENT_FORWARDED_TEST)
  {
    // read forwarded message
    event_data->Reset ();
    const char* txt1 = *event_data->GetString ();
    const char* txt2 = *event_data->GetString ();
    uint16 val3 = event_data->GetUInt16 ();

    // format it in a string
    csString comment;
    comment.Format ("%s (%s:%d)", txt1, txt2, val3);

    // add it to list of comments
    comments.Push (comment);
    if (comments.Length () > 5)
      comments.DeleteIndex (0);
    return;
  }

  if (event_type == SERVER_EVENT_MESSAGE)
  {
    //event_data->Reset ();
    const char* message = *event_data->GetString ();

    // add it to list of comments
    comments.Push (message);
    if (comments.Length () > 5)
      comments.DeleteIndex (0);
    return;
  }
}

iCelEntity* GameClientManager::CreateNetworkLinkTarget (celNetworkLinkType link_type, 
        csTicks creation_time, iCelPersistentDataList* creation_data, 
        bool player_controlled)

{
  // create dummy entity if this player is not the actor
  if (link_type == NETWORK_LINK_ACTOR && !factory->IsServerAvailable () && !player_controlled)
  {
    csString entity_name = "player" + entity_counter++;
    csRef<iCelEntity> entity = factory->nettest->CreateActorNPC (entity_name, "", csVector3 (0, 0, 0));
    SetEntityPersistentData (entity, creation_time, creation_data, CEL_PERSIST_TYPE_SERVER_CONTROL);
    return entity;
  }

  // if the entity is the player avatar
  if (link_type == NETWORK_LINK_ACTOR && !factory->IsServerAvailable () && player_controlled)
  {
    // remove the default camera
    csRef<iCelEntity> default_camera = pl->FindEntity ("defaultcamera");
    pl->RemoveEntity (default_camera);

    // add the actor
    csRef<iCelEntity> entity = factory->nettest->CreateActor ("camera", "", csVector3 (0, 100, 0));
    SetEntityPersistentData (entity, creation_time, creation_data, CEL_PERSIST_TYPE_SERVER_FORCING);
    return entity;
  }

  return 0;
}

void GameClientManager::NetworkLinkControlChanged (iCelEntity* entity, 
        bool player_controlled)
{
}

void GameClientManager::NetworkLinkRemoved (csTicks deletion_time, iCelEntity* entity)
{
  pl->RemoveEntity (entity);
}

void GameClientManager::LevelChanged (celGameInfo* new_game)
{
}

void GameClientManager::ClientEnd ()
{
}
