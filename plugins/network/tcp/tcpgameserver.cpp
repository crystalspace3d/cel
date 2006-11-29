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

//CS includes
#include "cssysdef.h"
#include "iutil/virtclk.h"

//CEL includes
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "include/celtool/persisthelper.h"
#include "plugins/network/tcp/tcputils.h"
#include "plugins/network/tcp/tcpgame.h"
#include "plugins/network/tcp/tcpgamefactory.h"
#include "plugins/network/tcp/tcpgameserver.h"

CS_PLUGIN_NAMESPACE_BEGIN(celTCPNetwork)
{

SCF_IMPLEMENT_IBASE_EXT (celTCPGameServer)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iCelGameServer)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celTCPGameServer::TCPGameServer)
  SCF_IMPLEMENTS_INTERFACE (iCelGameServer)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

/// default port number of a server
#define DEFAULT_SERVER_PORT 35000

/// maximum time for an incoming connection to send initialization data
#define CONNECTING_TIMEOUT 10000

/// default value for client timeout to be set with SetClientTimeOut
#define DEFAULT_CLIENT_TIMEOUT 60000

/// default value for network update period to be set with SetNetworkPeriod
#define DEFAULT_UPDATE_PERIOD 40

celTCPGameServer::celTCPGameServer (iObjectRegistry* object_reg, 
				    celTCPGameFactory* factory)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiCelGameServer);
  celTCPGameServer::object_reg = object_reg;
  celTCPGameServer::factory = factory;
  manager = 0;
  client_timeout = DEFAULT_CLIENT_TIMEOUT;
  is_game_admin = false;
  network_update_period = DEFAULT_UPDATE_PERIOD;
  last_update = 0;

  // TODO: use virtual clock to send time
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  if (!vc) ReportError (object_reg, "iVirtualClock Missing!");
}

celTCPGameServer::~celTCPGameServer ()
{
  nlEnable (NL_BLOCKING_IO);
  nlClose (server_socket);
  nlDisable (NL_BLOCKING_IO);

  size_t i;
  for (i = 0; i < incoming_players.Length (); i++)
    delete incoming_players[i];
  incoming_players.DeleteAll ();

  csHash<celPlayerData*, uint32>::GlobalIterator player_it = 
    players_data.GetIterator ();
  while (player_it.HasNext ())
    delete player_it.Next ();
  players_data.DeleteAll ();

  players_kicked.DeleteAll ();  

  SCF_DESTRUCT_EMBEDDED_IBASE (scfiCelGameServer);
}

bool celTCPGameServer::InitializeServer (celTCPGame* game, 
					 celPlayer* game_admin)
{
  // copy game data
  celTCPGameServer::game = game;
  if (game_admin)
  {
    is_game_admin = true;
    celTCPGameServer::game_admin = *game_admin;    
  }
  else is_game_admin = false;

  // TODO: check if we need to read the port number from the IPv6 address
  // TODO: read the ipaddress of the game admin

  // find port number of server
  unsigned short port_nb = game->game_info.port_nb;
  if (port_nb == 0) port_nb = DEFAULT_SERVER_PORT;

  // open socket
  server_socket = nlOpen (port_nb, NL_RELIABLE);
  if (server_socket == NL_INVALID)
  {
    csString txt = "Failed to open a socket on port number ";
    txt += port_nb;
    return ReportNetworkError (object_reg, txt);
  }

  // read server ip address
  NLaddress server_address;
  if (!nlGetLocalAddr (server_socket, &server_address))
    ReportNetworkError (object_reg, "Error reading IP address of server");
  else
  {
    CopyAddress (server_address, game->game_info.ip_address);
    if (!ReadAddressData (server_address, game->game_info.hostname, 
			  game->game_info.port_nb))
      ReportNetworkError (object_reg, "Error reading IP hostname of server");
  }

  // let the socket accept new connections
  if (!nlListen (server_socket))
  {
    nlClose (server_socket);
    return ReportNetworkError (object_reg, "Could not listen on socket");
  }

  // init the manager of the game factory
  if (!factory->manager->InitServer (&game->scfiCelGame))
  {
    nlClose (server_socket);
    return ReportError (object_reg, "The server manager failed its initialization");
  }

  // TODO: update game info

  return true;
}

void celTCPGameServer::UpdateNetwork ()
{
  // get time of this snapshot
  // use time from virtual clock
  csTicks snapshot_time = csGetTicks ();

  // TODO: do not active waiting
  // check if update period is past
  if (snapshot_time - last_update > network_update_period)
  {
    last_update = snapshot_time;

    // kick players
    KickPlayers (snapshot_time);

    // look for new incoming players
    UpdateConnectingPlayers (snapshot_time);

    // update all client sockets
    UpdatePlayerSockets (snapshot_time);

    // read data from all players
    ReadPlayerSockets (snapshot_time);

    // send data to all players
    WritePlayerSockets (snapshot_time);
  }
}

void celTCPGameServer::UpdateConnectingPlayers (csTicks snapshot_time)
{
  // look for new incoming players
  NLsocket new_socket = nlAcceptConnection (server_socket);
  if (new_socket != NL_INVALID)
  {
    celIncomingPlayer* new_player = new celIncomingPlayer (new_socket);
    new_player->connection_start = snapshot_time;
    incoming_players.Put (0, new_player);
  }

  size_t incoming_index = incoming_players.Length ();
  while (incoming_index > 0)
  {
    celIncomingPlayer* incoming_player = incoming_players[incoming_index - 1];

    // check if connection not timedout
    if (snapshot_time - incoming_player->connection_start > CONNECTING_TIMEOUT)
    {
      delete incoming_player;
      incoming_players.DeleteIndex (incoming_index - 1);
      incoming_index--;
      continue;
    }

    // update socket
    if (incoming_player->socket_cache->UpdateSocket () <= 0)
    {
      // connection is closed or broken
      delete incoming_player;
      incoming_players.DeleteIndex (incoming_index - 1);
      incoming_index--;
      continue;
    }

    // check if client has been rejected and can be deleted
    if (incoming_player->to_delete)
    {
      if (!incoming_player->socket_cache->IsRemainingSentPacket ())
      {
	delete incoming_player;
	incoming_players.DeleteIndex (incoming_index - 1);
      }
      incoming_index--;
      continue;
    }

    // TODO: check validity of packets: if a client send wrong data then it can crash the server.
    // Such check is not really needed in this plugin as it is not designed to work on the Internet and
    // we can assume the other players on the network are trusted.
    // In the same order of idea, there is no protection against buffer overflow attacks.

    // check if first packet ready
    if (incoming_player->socket_cache->IsReceivedPacket ())
    {
      celNetworkBuffer* packet = 
	incoming_player->socket_cache->GetReceivedPacket ();

      // read game name and check it
      csString client_game_name;
      packet->Read (client_game_name);
      if (!factory->game_name.Compare (client_game_name))
      {
	// create answer packet
	celNetworkBuffer* answer_packet = new celNetworkBuffer ();
	uint8 answer_type = CEL_NET_SERVER_REJECTED_BAD_GAME;
	answer_packet->Write (answer_type);
	answer_packet->Write (factory->game_name);

	// send packet
	delete packet;
	incoming_player->to_delete = true;
	incoming_player->socket_cache->SendPacket (answer_packet);
	incoming_player->socket_cache->UpdateSocket ();
	incoming_index--;
	continue;
      }

      // read protocol version and check it
      csString client_protocol_version;
      packet->Read (client_protocol_version);
      if (!factory->protocol_version.Compare (client_protocol_version))
      {
	// create answer packet
	celNetworkBuffer* answer_packet = new celNetworkBuffer ();
	uint8 answer_type = CEL_NET_SERVER_REJECTED_BAD_PROTOCOL;
	answer_packet->Write (answer_type);
	answer_packet->Write (factory->protocol_version);

	// send packet
	delete packet;
	incoming_player->to_delete = true;
	incoming_player->socket_cache->SendPacket (answer_packet);
	incoming_player->socket_cache->UpdateSocket ();
	incoming_index--;
	continue;
      }

      // read end of initialization packet
      csString client_password;
      packet->Read (client_password);
      celPlayer player;
      packet->Read (player.player_id);
      packet->Read (player.player_name);
      // TODO: read custom data of player
      delete packet;

      // read IP address and port number of client
      NLaddress client_address;
      if (!nlGetRemoteAddr (incoming_player->socket_cache->socket, 
			    &client_address))
	ReportNetworkError (object_reg, "Error reading IP address of client");
      else
      {
	CopyAddress (client_address, player.ip_address);
	if (!ReadAddressData (client_address, player.hostname, player.port_nb))
	  ReportNetworkError (object_reg, "Error reading IP hostname of client");
      }

      // check if the game type is not single player
      bool rejected_singleplayer = false;
      if (factory->game->game_type == CEL_NET_SINGLEPLAYER)
      {
	if (player_list.GetCount () >= 1)
	  rejected_singleplayer = true;
	else
	{
	  csString player_hostname;
	  uint16 player_port_nb;
	  if (!factory->game->client->GetClientIPAddress (player_hostname, 
							  player_port_nb))
	    rejected_singleplayer = true;
	  else if (!player_hostname.Compare(player.hostname)
		   || player_port_nb != player.port_nb)
	    rejected_singleplayer = true;
	}
      }

      if (rejected_singleplayer)
      {
	// create answer packet
	celNetworkBuffer* answer_packet = new celNetworkBuffer ();
	uint8 answer_type = CEL_NET_SERVER_REJECTED_SINGLEPLAYER;
	answer_packet->Write (answer_type);

	// send packet
	incoming_player->to_delete = true;
	incoming_player->socket_cache->SendPacket (answer_packet);
	incoming_player->socket_cache->UpdateSocket ();
	incoming_index--;
	continue;
      }

      // check password
      if (!factory->game->game_info.password.IsEmpty ()
	  && !factory->game->game_info.password.Compare (client_password))
      {
	// create answer packet
	celNetworkBuffer* answer_packet = new celNetworkBuffer ();
	uint8 answer_type = CEL_NET_SERVER_REJECTED_BAD_PASSWORD;
	answer_packet->Write (answer_type);

	// send packet
	incoming_player->to_delete = true;
	incoming_player->socket_cache->SendPacket (answer_packet);
	incoming_player->socket_cache->UpdateSocket ();
	incoming_index--;
	continue;
      }

      // TODO: close game if the administrator is not authorized
      // ask for authorization from the server manager
      csString unauthorized_reason = "";
      if (!manager->AuthorizePlayer (&player, unauthorized_reason))
      {
	// create answer packet
	celNetworkBuffer* answer_packet = new celNetworkBuffer ();
	uint8 answer_type = CEL_NET_SERVER_REJECTED_UNAUTHORIZED;
	answer_packet->Write (answer_type);
	answer_packet->Write (unauthorized_reason);

	// send packet
	incoming_player->to_delete = true;
	incoming_player->socket_cache->SendPacket (answer_packet);
	incoming_player->socket_cache->UpdateSocket ();
	incoming_index--;
	continue;
      }

      // check if the new incoming player is not a player already connected but timed out
      // TODO: does not work
      bool player_was_timedout = false;
      celPlayerData* timeout_player_data;
      csHash<celPlayerData*, uint32>::GlobalIterator timeout_player_it = 
	players_data.GetIterator ();
      while (timeout_player_it.HasNext ())
      {
	timeout_player_data = timeout_player_it.Next ();
	// TODO: find a better way to recognize the player
	if (timeout_player_data->unreachable_start
	    && timeout_player_data->player.hostname.Compare (player.hostname)
	    && timeout_player_data->player.player_name.Compare (player.player_name))
	{
	  player_was_timedout = true;
	  break;
	}
      }

      // check max number of players not reached
      if (!player_was_timedout
	  && game->game_info.max_players
	  && players_data.GetSize () >= game->game_info.max_players)
      {
	// create answer packet
	celNetworkBuffer* answer_packet = new celNetworkBuffer ();
	uint8 answer_type = CEL_NET_SERVER_REJECTED_MAX_PLAYERS;
	answer_packet->Write (answer_type);

	// send packet
	incoming_player->to_delete = true;
	incoming_player->socket_cache->SendPacket (answer_packet);
	incoming_player->socket_cache->UpdateSocket ();
	incoming_index--;
	continue;
      }

      // check no player ID clash
      if (!player_was_timedout)
      {
	bool id_clash = false;
	uint32 new_id = 0;
	csHash<celPlayerData*, uint32>::GlobalIterator id_it = 
	  players_data.GetIterator ();
	while (id_it.HasNext ())
	{
	  celPlayerData* id_player_data = id_it.Next ();
	  // check if a player has already this ID
	  if (id_player_data->player.player_id == player.player_id)
	    id_clash = true;

	  // find an ID not already allocated
	  if (id_player_data->player.player_id == new_id)
	    new_id++;
	}
	// allocate a new ID
	if (id_clash) player.player_id = new_id;
      }

      // Player accepted!!
      // create answer packet
      celNetworkBuffer* answer_packet = new celNetworkBuffer ();
      uint8 answer_type = CEL_NET_SERVER_LOADING_DATA;
      answer_packet->Write (answer_type);
      csTicks time = csGetTicks ();
      //csTicks time = vc->GetElapsedTicks ();
      answer_packet->Write (time);
      answer_packet->Write (player.player_id);
      answer_packet->Write (player.player_name);
      answer_packet->Write (player.hostname);
      int i = 0;
      for ( ; i < 32; i++)
	answer_packet->Write (player.ip_address[i]);
      answer_packet->Write (player.port_nb);
      // TODO: send custom data of player

      // send game info
      answer_packet->Write (game->game_info.game_id);
      answer_packet->Write (game->game_info.game_name);
      answer_packet->Write (game->game_info.hostname);
      answer_packet->Write (game->game_info.port_nb);
      answer_packet->Write (game->game_info.max_players);
      answer_packet->Write (game->game_info.custom_data);
 
      // send packet
      incoming_player->socket_cache->SendPacket (answer_packet);
      incoming_player->socket_cache->UpdateSocket ();

      // put new player in connected players
      celPlayerData* player_data;
      celPlayerNetworkState previous_state;
      if (!player_was_timedout)
      {
	// first time we see this player
	player_data = new celPlayerData ();
	player_data->player = player;
	player_data->socket_cache = incoming_player->socket_cache;
	incoming_player->socket_cache = 0;
	previous_state = CEL_NET_PLAYER_NOT_CONNECTED;
	player_data->network_state = CEL_NET_PLAYER_CONNECTING;
	players_data.Put (player.player_id, player_data);
	player_list.Add (&player_data->player);
      }
      else
      {
	// player was timed out
	player_data = timeout_player_data;
	uint32 previous_id = player_data->player.player_id;
	player_data->player = player;
	player_data->player.player_id = previous_id;
	delete player_data->socket_cache;
	player_data->socket_cache = incoming_player->socket_cache;
	incoming_player->socket_cache = 0;
	player_data->unreachable_start = 0;

	size_t i;
	for (i = 0; i < player_data->server_events.Length (); i++)
	  delete player_data->server_events[i];
	player_data->server_events.DeleteAll ();

	csHash<NetworkLinkServerData*, uint>::GlobalIterator link_it = 
	  player_data->network_links.GetIterator ();
	while (link_it.HasNext ())
	  delete link_it.Next ();
	player_data->network_links.DeleteAll ();

	previous_state = CEL_NET_PLAYER_UNREACHABLE;
	player_data->network_state = CEL_NET_PLAYER_CONNECTING;
      }

      // remove from incoming players
      delete incoming_player;
      incoming_players.DeleteIndex (incoming_index - 1);
      incoming_index--;

      // call server manager
      if (manager)
	manager->PlayerNetworkStateChanged (&player_data->player, 
					    CEL_NET_PLAYER_CONNECTING, 
					    previous_state);
      continue;
    }

    incoming_index--;
  }
}

void celTCPGameServer::UpdatePlayerSockets (csTicks snapshot_time)
{
  csHash<celPlayerData*, uint32>::GlobalIterator player_it = players_data.GetIterator ();
  while (player_it.HasNext ())
  {
    celPlayerData* player_data = player_it.Next ();
    int update_socket_result = player_data->socket_cache->UpdateSocket ();

    // if connection is still valid
    if (update_socket_result == 1)
      continue;

    // if connection is closed
    if (update_socket_result == 0)
    {
      // call server manager
      celPlayerNetworkState previous_state = player_data->network_state;
      player_data->network_state = CEL_NET_PLAYER_DISCONNECTED;
      if (manager)
	manager->PlayerNetworkStateChanged (&player_data->player, 
					    CEL_NET_PLAYER_DISCONNECTED, 
					    previous_state);

      // TODO: if the player is the admin, we have to close the server

      // remove player
      player_list.Remove (&player_data->player);
      // TODO: is this valid? Doesn't it corrupt the iterator?
      players_data.DeleteAll (player_data->player.player_id);
      delete player_data;
      continue;
    }

    // if connection is broken for the first time
    if (update_socket_result == -1
	&& player_data->network_state != CEL_NET_PLAYER_UNREACHABLE)
    {
      player_data->unreachable_start = snapshot_time;

      // call server manager
      celPlayerNetworkState previous_state = player_data->network_state;
      player_data->network_state = CEL_NET_PLAYER_UNREACHABLE;
      if (manager)
	manager->PlayerNetworkStateChanged (&player_data->player, 
					    CEL_NET_PLAYER_UNREACHABLE, 
					    previous_state);
      continue;
    }

    // if connection is broken and timedout
    if (update_socket_result == -1
	&& snapshot_time - player_data->unreachable_start > client_timeout)
    {
      // call server manager
      celPlayerNetworkState previous_state = player_data->network_state;
      player_data->network_state = CEL_NET_PLAYER_LOST;
      if (manager)
	manager->PlayerNetworkStateChanged (&player_data->player, 
					    CEL_NET_PLAYER_LOST, 
					    previous_state);

      // TODO: if the player is the admin, we have to close the server

      // remove player
      player_list.Remove (&player_data->player);
      // TODO: is this valid? Doesn't it corrupt the iterator?
      players_data.DeleteAll (player_data->player.player_id);
      delete player_data;
      continue;
    }
  }
}

void celTCPGameServer::ReadPlayerSockets (csTicks snapshot_time)
{
  csHash<celPlayerData*, uint32>::GlobalIterator player_it = 
    players_data.GetIterator ();
  while (player_it.HasNext ())
  {
    celPlayerData* player_data = player_it.Next ();

    // if player is still connecting
    if (player_data->network_state == CEL_NET_PLAYER_CONNECTING)
    {
      if (!player_data->socket_cache->IsReceivedPacket ())
	continue;

      celNetworkBuffer* new_packet = player_data->socket_cache->GetReceivedPacket ();
      uint8 answer_type;
      new_packet->Read (answer_type);
      delete new_packet;

      // check if client is really ready
      if (answer_type != CEL_NET_PLAYER_PLAYING)
      {
	// another answer is not authorized
	// call server manager
	celPlayerNetworkState previous_state = player_data->network_state;
	player_data->network_state = CEL_NET_PLAYER_LOST;
	if (manager)
	  manager->PlayerNetworkStateChanged (&player_data->player, 
					      CEL_NET_PLAYER_LOST, 
					      previous_state);

	// TODO: if the player is the admin, we have to close the server
	
	// remove player
	player_list.Remove (&player_data->player);
	// TODO: is this valid? Doesn't it corrupt the iterator?
	players_data.DeleteAll (player_data->player.player_id);
	delete player_data;
	continue;
      }

      // client is now playing
      // call server manager
      celPlayerNetworkState previous_state = player_data->network_state;
      player_data->network_state = CEL_NET_PLAYER_PLAYING;
      if (manager)
	manager->PlayerNetworkStateChanged (&player_data->player, 
					    CEL_NET_PLAYER_PLAYING, 
					    previous_state);

    }

    // if player is unreachable
    if (player_data->network_state != CEL_NET_PLAYER_PLAYING)
      continue;

    // normal reception
    while (player_data->socket_cache->IsReceivedPacket ())
    {
      celNetworkBuffer* new_packet = 
	player_data->socket_cache->GetReceivedPacket ();

      while (new_packet->IsReadDataRemaining ())
      {
	uint8 data_type;
	new_packet->Read (data_type);

	int i = 0;
	celClientEventType event_type;
	csTicks event_time;
	csRef<iCelDataBuffer> data_buffer;
	celStandardPersistentDataList* data_list;

	switch (data_type)
	{
	case CLIENT_DATA_EVENT:
	  uint8 events_nb;
	  new_packet->Read (events_nb);

	  for ( ; i < events_nb; i++)
	  {
	    // read event
	    new_packet->Read (event_type);
	    new_packet->Read (event_time);
	    new_packet->Read (data_buffer);

	    // call manager
	    if (manager) manager->HandleClientEvent (&player_data->player, 
						     event_type, event_time, 
						     data_buffer);
	  }
	  break;

	case CLIENT_DATA_CONTROLLED_LINK:
	  uint8 links_nb;
	  new_packet->Read (links_nb);

	  for ( ; i < links_nb; i++)
	  {
	    // read link data
	    uint entity_id;
	    new_packet->Read (entity_id);
	    data_list = new celStandardPersistentDataList ();
	    new_packet->Read (data_list);

	    // update entity
	    NetworkLinkServerData* link_data = 
	      player_data->network_links.Get (entity_id, 0);
	    CS_ASSERT (link_data->player_controlled);
	    CS_ASSERT (link_data->link_data.linked_entity);
	    // TODO: check if the player is not cheating and can really control this entity

	    // set persistent data of the entity
	    csRef<iCelDataBuffer> databuf;
	    csString pc_name, pc_tag;
	    iCelPropertyClass* pc;
	    size_t i, count = data_list->GetCount ();
	    for (i = 0; i < count; i++)
	    {
	      data_list->GetPersistentData (i, databuf, pc_name, pc_tag);
	      if (pc_tag)
		pc = link_data->link_data.linked_entity->
		  GetPropertyClassList ()->FindByNameAndTag (pc_name, pc_tag);
	      else
		pc = link_data->link_data.linked_entity->
		  GetPropertyClassList ()->FindByName (pc_name);
	      
	      if (pc)
	      {
		// TODO: use real clock value
		celPersistenceResult persist_code = 
		  pc->SetPersistentData (0, databuf, CEL_PERSIST_TYPE_CLIENT_CONTROL);
		if (persist_code != CEL_PERSIST_RESULT_OK && manager)
		  manager->PersistenceProblem (&player_data->player, 
					       link_data->link_data.linked_entity, 
					       pc, persist_code);
	      }
	      else if (manager)
		  manager->PersistenceProblem (&player_data->player, 
					       link_data->link_data.linked_entity, 0, 
					       CEL_PERSIST_RESULT_UNKNOWN_PC);
	    }
	    delete data_list;
	  }
	  break;

	default:
	  ReportError (object_reg, "Invalid data type received from player");
	  return;
	}
      }

      delete new_packet;
    }
  }
}

void celTCPGameServer::WritePlayerSockets (csTicks snapshot_time)
{
  csHash<celPlayerData*, uint32>::GlobalIterator player_it = 
    players_data.GetIterator ();
  while (player_it.HasNext ())
  {
    celPlayerData* player_data = player_it.Next ();

    if (player_data->network_state != CEL_NET_PLAYER_PLAYING)
      continue;

    // TODO: check frequency of the player
    // TODO: do not allocate a new buffer each time, re-use another one
    celNetworkBuffer* packet = new celNetworkBuffer ();

    // write snapshot time
    packet->Write (snapshot_time);

    // write server events
    uint8 list_size = player_data->server_events.GetSize ();
    if (list_size > 0)
    {
      // TODO: write time of events
      packet->Write ((uint8) SERVER_DATA_EVENT);
      packet->Write (list_size);

      csArray<celServerEventData*>::Iterator events_it = 
	player_data->server_events.GetIterator ();
      while (events_it.HasNext ())
      {
	celServerEventData* event = events_it.Next ();
	packet->Write (event->event_type);
	packet->Write (event->event_time);
	packet->Write (event->event_data);
	delete event;
      }
    }
    player_data->server_events.DeleteAll ();

    // get the set of network links to send
    csArray<NetworkLinkServerData*> new_links;
    csArray<NetworkLinkServerData*> pending_links;
    csArray<NetworkLinkServerData*> controlled_links;
    csArray<NetworkLinkServerData*> removed_links;
    player_data->GetNetworkLinks (snapshot_time, new_links, pending_links, 
				  controlled_links, removed_links);

    // write new network links
    uint8 links_size = new_links.GetSize ();
    if (links_size > 0)
    {
      packet->Write ((uint8) SERVER_DATA_LINK_INIT);
      packet->Write (links_size);

      csArray<NetworkLinkServerData*>::Iterator links_it = 
	new_links.GetIterator ();
      while (links_it.HasNext ())
      {
	NetworkLinkServerData* link = links_it.Next ();

	// if player_controlled then send network link params
	packet->Write (link->player_controlled);
	if (link->player_controlled)
	  packet->Write (link->link_data.period);

	// write link data
	packet->Write ((uint8) link->link_data.link_type);
	packet->Write (link->link_data.linked_entity->GetID ());
	csRef<iCelPersistentDataList> data_list = 
	  GetEntityPersistentData (link->link_data.linked_entity, 
				   &link->link_data.persistence_mask, 
				   CEL_PERSIST_TYPE_SERVER_CONTROL);
	packet->Write (data_list);

	// mark link as pending link
	link->link_state = 1;
	link->last_update = snapshot_time;
      }
    }

    // write pending network links
    links_size = pending_links.GetSize ();
    if (links_size > 0)
    {
      packet->Write ((uint8) SERVER_DATA_LINK_UPDATE);
      packet->Write (links_size);

      csArray<NetworkLinkServerData*>::Iterator links_it = 
	pending_links.GetIterator ();
      while (links_it.HasNext ())
      {
	NetworkLinkServerData* link = links_it.Next ();

	// write link data
	packet->Write (link->link_data.linked_entity->GetID ());
	csRef<iCelPersistentDataList> data_list = 
	  GetEntityPersistentData (link->link_data.linked_entity, 
				   &link->link_data.persistence_mask, 
				   CEL_PERSIST_TYPE_SERVER_CONTROL);
	packet->Write (data_list);

	// mark time of last update
	link->last_update = snapshot_time;
      }
    }

    // write network links controlled by the player
    links_size = controlled_links.GetSize ();
    if (links_size > 0)
    {
      packet->Write ((uint8) SERVER_DATA_LINK_CONTROL);
      packet->Write (links_size);

      csArray<NetworkLinkServerData*>::Iterator links_it = 
	controlled_links.GetIterator ();
      while (links_it.HasNext ())
      {
	NetworkLinkServerData* link = links_it.Next ();

	// write link data
	packet->Write (link->link_data.linked_entity->GetID ());
	csRef<iCelPersistentDataList> data_list = 
	  GetEntityPersistentData (link->link_data.linked_entity, 
				   &link->link_data.persistence_mask, 
				   CEL_PERSIST_TYPE_SERVER_FORCING);
	packet->Write (data_list);

	// mark time of last update
	link->last_update = snapshot_time;
      }
    }

    // write network links to be removed
    links_size = removed_links.GetSize ();
    if (links_size > 0)
    {
      packet->Write ((uint8) SERVER_DATA_LINK_REMOVE);
      packet->Write (links_size);

      csArray<NetworkLinkServerData*>::Iterator links_it = 
	removed_links.GetIterator ();
      while (links_it.HasNext ())
      {
	NetworkLinkServerData* link = links_it.Next ();

	// write link data
	packet->Write (link->link_data.linked_entity->GetID ());

	// remove network link
	player_data->network_links.
	  DeleteAll (link->link_data.linked_entity->GetID ());
	delete link;
      }
    }

    // send packet
    if (packet->GetSize () > 0)
      player_data->socket_cache->SendPacket (packet);
    else delete packet;
    player_data->socket_cache->UpdateSocket ();
  }
}

void celTCPGameServer::KickPlayers (csTicks snapshot_time)
{
  csArray<KickedPlayer>::Iterator player_it = players_kicked.GetIterator ();
  while (player_it.HasNext ())
  {
    // TODO: close game if player is admin
    KickedPlayer kicked_player = player_it.Next ();

    if (!player_list.Remove (kicked_player.player))
    {
      ReportError (object_reg, "Kicking an invalid player");
      continue;
    }

    // create an incoming player marked to delete
    celPlayerData* player_data = 
      players_data.Get (kicked_player.player->player_id, 0);
    celIncomingPlayer* incoming_player = new celIncomingPlayer ();
    incoming_player->socket_cache = player_data->socket_cache;
    player_data->socket_cache = 0;
    players_data.DeleteAll (kicked_player.player->player_id);
    delete player_data;
    incoming_player->to_delete = true;
    incoming_player->connection_start = snapshot_time;
    incoming_players.Put (0, incoming_player);

    // create kicked packet
    celNetworkBuffer* answer_packet = new celNetworkBuffer ();
    answer_packet->Write (snapshot_time);
    uint8 answer_type = SERVER_DATA_KICKED;
    answer_packet->Write (answer_type);
    answer_packet->Write (kicked_player.reason);
  
    // send packet
    // TODO: clear all previous messages to be sent
    incoming_player->socket_cache->SendPacket (answer_packet);  
  }
  players_kicked.DeleteAll ();
}

void celTCPGameServer::SetAdministrator (celPlayer* new_admin)
{
  if (new_admin)
  {
    is_game_admin = true;
    game_admin = *new_admin;    
  }
  else is_game_admin = false;
}

celPlayerNetworkState celTCPGameServer::GetPlayerState (celPlayer* player) 
{
  celPlayerData* player_data = players_data.Get (player->player_id, 0);
  CS_ASSERT (player_data);
  return player_data->network_state;
}

void celTCPGameServer::KickPlayer (celPlayer* player, csString reason)
{
  KickedPlayer kicked_player;
  kicked_player.player = player;
  kicked_player.reason = reason;
  players_kicked.Push (kicked_player);
}

iCelPlayerList* celTCPGameServer::CreateChannel (csStringID channel_id)
{
  // @@@ TODO
  return 0;
}

void celTCPGameServer::RemoveChannel (csStringID channel_id)
{
  // @@@ TODO
}

void celTCPGameServer::LaunchServerEvent (celPlayer* player, 
        celServerEventData &event_data)
{
  celPlayerData* player_data = players_data.Get (player->player_id, 0);
  CS_ASSERT (player_data);
  celServerEventData* new_event = new celServerEventData (event_data);
  player_data->server_events.Push (new_event);
}

void celTCPGameServer::LaunchServerEvent (csStringID channel_id, 
        celServerEventData &event_data)
{
  // @@@ TODO
}

void celTCPGameServer::SetNetworkLink (celPlayer* player, 
        celNetworkLinkData &link_data, bool player_controlled)
{
  // TODO: check if link was already present

  celPlayerData* player_data = players_data.Get (player->player_id, 0);
  CS_ASSERT (player_data);
  CS_ASSERT (link_data.linked_entity);
  NetworkLinkServerData* server_link_data = new NetworkLinkServerData ();
  server_link_data->link_state = 0;
  server_link_data->player_controlled = player_controlled;
  server_link_data->link_data = link_data;
  player_data->network_links.Put (link_data.linked_entity->GetID (), 
				  server_link_data);
}

void celTCPGameServer::ChangeLinkControl (iCelEntity* entity, celPlayer* player, 
        bool player_controlled)
{
  // @@@ TODO
}

void celTCPGameServer::SetNetworkLink (csStringID channel_id, 
        celNetworkLinkData &link_data)
{
  // @@@ TODO
}

void celTCPGameServer::RemoveNetworkLink (iCelEntity* entity, celPlayer* player)
{
  celPlayerData* player_data = players_data.Get (player->player_id, 0);
  CS_ASSERT (player_data);
  NetworkLinkServerData* server_link_data = 
    player_data->network_links.Get (entity->GetID (), 0);
  if (!server_link_data) return;
  server_link_data->link_state = 2;
}

void celTCPGameServer::RemoveNetworkLink (iCelEntity* entity, csStringID channel_id)
{
  // @@@ TODO
}

void celTCPGameServer::ChangeLevel (celGameInfo* new_game)
{
  // @@@ TODO
}

void celTCPGameServer::SetMaximumBandwidth (size_t width)
{
  // @@@ TODO
}

void celTCPGameServer::GetNetworkPlayerStats (celPlayer* player, 
					      celNetworkPlayerStats& stats) const
{
  // @@@ TODO
}

void celTCPGameServer::GetNetworkTotalStats (celNetworkPlayerTotalStats& stats) const
{
  // @@@ TODO
}

//---------------------------------------------------------------------------

celPlayerData::~celPlayerData ()
{
  delete socket_cache;
    
  size_t i;
  for (i = 0; i < server_events.Length (); i++)
    delete server_events[i];
  server_events.DeleteAll ();

  csHash<NetworkLinkServerData*, uint>::GlobalIterator link_it = 
    network_links.GetIterator ();
  while (link_it.HasNext ())
    delete link_it.Next ();
  network_links.DeleteAll ();
}

void celPlayerData::GetNetworkLinks (csTicks snapshot_time, 
				     csArray<NetworkLinkServerData*>& new_links, 
				     csArray<NetworkLinkServerData*>& pending_links, 
				     csArray<NetworkLinkServerData*>& controlled_links, 
				     csArray<NetworkLinkServerData*>& removed_links)
{
  csHash<NetworkLinkServerData*, uint>::GlobalIterator links_it = 
    network_links.GetIterator ();
  while (links_it.HasNext ())
  {
    NetworkLinkServerData* link_data = links_it.Next ();

    // if it is a new link
    if (link_data->link_state == 0)
    {
      new_links.Push (link_data);
      continue;
    }

    // if it is a link that need to be removed
    if (link_data->link_state == 2)
    {
      removed_links.Push (link_data);
      continue;
    }

    // if it is a pending link
    if (snapshot_time - link_data->last_update >= link_data->link_data.period)
    {
      // controlled by the client
      if (link_data->player_controlled)
	controlled_links.Push (link_data);
      // controlled by the server
      else
	pending_links.Push (link_data);
    }
  }

  // TODO: find a set of network links that will not be bigger than the maximum bandwidth allowed
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPlayerList)
  SCF_IMPLEMENTS_INTERFACE (iCelPlayerList)
SCF_IMPLEMENT_IBASE_END

celPlayerList::celPlayerList ()
{
  SCF_CONSTRUCT_IBASE (0);
}

celPlayerList::~celPlayerList ()
{
  RemoveAll ();
  SCF_DESTRUCT_IBASE ();
}

size_t celPlayerList::GetCount () const
{
  return players.Length ();
}

celPlayer* celPlayerList::Get (size_t index) const
{
  CS_ASSERT ((index != csArrayItemNotFound) && index < players.Length ());
  return players[index];
}

size_t celPlayerList::Add (celPlayer* player)
{
  return players.Push (player);
}

bool celPlayerList::Remove (celPlayer* player)
{
  size_t idx = players.Find (player);
  if (idx != csArrayItemNotFound)
  {
    players.DeleteIndex (idx);
    return true;
  }
  return false;
}

bool celPlayerList::Remove (size_t n)
{
  players.DeleteIndex (n);
  return true;
}

void celPlayerList::RemoveAll ()
{
  while (players.Length () > 0)
    Remove ((size_t) 0);
}

size_t celPlayerList::Find (celPlayer* player) const
{
  return players.Find (player);
}

}
CS_PLUGIN_NAMESPACE_END(celTCPNetwork)

