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

#ifndef __NETMANAGERS_H__
#define __NETMANAGERS_H__

#include "physicallayer/nettypes.h"
#include "physicallayer/network.h"
#include "nettest.h"

struct iCelGame;
class NetTest;
class GameServerManager;
class GameClientManager;
struct iCelEntity;

class GameFactoryManager : public celGameFactoryManager
{
 public:
  NetTest* nettest;
  csWeakRef<iCelGame> game;
  csRef<iCelGameFactory> factory;
  csWeakRef<iCelPlLayer> pl;

  csString connection_state;
  csString level_path;
  csString level_file;

 public:
  GameServerManager* server_manager;
  GameClientManager* client_manager;

  GameFactoryManager (NetTest* nettest, iCelGameFactory* factory,
  	csString level_path, csString level_file);
  virtual ~GameFactoryManager ();

  virtual void ServerNetworkStateChanged (celServerNetworkState new_state, 
        celServerNetworkState previous_state, csString reason);
  virtual bool InitClient (iCelGame* game);
  virtual bool InitServer (iCelGame* game);
  csString GetConnectionState () { return connection_state; }
  bool IsClientAvailable ()
  { return (game.IsValid() && game->IsClientAvailable ()); }
  bool IsServerAvailable ()
  { return (game.IsValid() && game->IsServerAvailable ()); }
  iCelGameClient* GetClient()
  { return game.IsValid() ? game->GetGameClient() : 0; }
  iCelGameServer* GetServer()
  { return game.IsValid() ? game->GetGameServer() : 0; }
};

class GameServerManager : public celGameServerManager
{
  GameFactoryManager* factory;
  csRef<iCelPlLayer> pl;
 public:
  csRef<iCelEntity> npc_entity;

 public:
  GameServerManager (GameFactoryManager* factory);
  virtual ~GameServerManager ();

  virtual bool AuthorizePlayer (celPlayer* player, csString &reason);
  virtual celPlayer* ValidatePlayerUpdate (celPlayer* previous_player_data);
  virtual void PlayerNetworkStateChanged (celPlayer* player, 
        celPlayerNetworkState new_state, 
        celPlayerNetworkState previous_state);
  virtual void HandleClientEvent (celPlayer* player, 
        celClientEventType event_type, csTicks event_time, 
        iCelDataBuffer* event_data);
  virtual void PersistenceProblem (celPlayer* player, iCelEntity* entity,
	iCelPropertyClass* pc, celPersistenceResult persist_code);
  virtual void ServerEnd ();
};

class GameClientManager : public celGameClientManager
{
  GameFactoryManager* factory;
  csRef<iCelPlLayer> pl;

  uint32 entity_counter;

 public:
  csArray<csString> comments;

  GameClientManager (GameFactoryManager* factory);
  virtual ~GameClientManager ();

  virtual void PlayerValidated (celPlayer* player);
  virtual void HandleServerEvent (celServerEventType event_type, 
        csTicks event_time, iCelDataBuffer* event_data);
  virtual iCelEntity* CreateNetworkLinkTarget (celNetworkLinkType link_type, 
        csTicks creation_time, iCelPersistentDataList* creation_data, 
        bool player_controlled);
  virtual void NetworkLinkControlChanged (iCelEntity* entity, 
        bool player_controlled);
  virtual void NetworkLinkRemoved (csTicks deletion_time, iCelEntity* entity);
  virtual void LevelChanged (celGameInfo* new_game);
  virtual void ClientEnd ();
};

#endif // __NETMANAGERS_H__
