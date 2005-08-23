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

#ifndef __CEL_PL_NETWORK__
#define __CEL_PL_NETWORK__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/strset.h"

struct iCelEntity;
struct iCelDataBuffer;
enum celNetworkGameType;
enum celServerNetworkState;
enum celPlayerNetworkState;
struct celGame;
struct celGameInfo;
struct celPlayer;
struct iCelPlayerList;
struct celServerEventData;
struct celClientEventData;
struct celNetworkLinkData;
struct celGameFactoryManager;
struct celGameClientManager;
struct celGameServerManager;
struct celNetworkServerStats;
struct celNetworkPlayerStats;
struct celNetworkServerTotalStats;

SCF_VERSION (iCelGame, 0, 0, 1);

/**
 * A networked game. It maintains the main data of a game: game type, game 
 * info and pointers to the local client and server.
 */
struct iCelGame : public iBase
{
  /**
   * Return the type of the game.
   */
  virtual celNetworkGameType GetGameType () const = 0;

  /**
   * Return the general info of the game.
   */
  virtual celGameInfo* GetGameInfo () = 0;

  /**
   * Return true if a local server is available in this process,
   * false otherwise.
   */
  virtual bool IsServerAvailable () const = 0;

  /**
   * Return the local server if available, 0 otherwise.
   */
  virtual iCelGameServer* GetGameServer () const = 0;

  /**
   * Return true if a local client is available in this process,
   * false otherwise.
   */
  virtual bool IsClientAvailable () const = 0;

  /**
   * Return the local client if available, 0 otherwise.
   */
  virtual iCelGameClient* GetGameClient () const = 0;
};

SCF_VERSION (iCelGameFactory, 0, 0, 1);

/**
 * A factory to manage game sessions. You can use it to:
 * <UL>
 * <LI>set and get the name of the game and of the network protocol 
 * version.</LI>
 * <LI>search for available games, connect to them or create a new game.</LI>
 * <LI>record the events of the game and playback them.</LI>
 * </UL>
 * You must define and register a iCelGameFactoryManager which will catch 
 * the events from this factory.
 * <P>There should be only one game factory in an application.
 */
struct iCelGameFactory : public iBase
{
  /**
   * Set the name of the game. Client and server instances can talk 
   * together only if they have the same game name and protocol version.
   * <P>A game name must be set before any operation on the factory.
   * \param game_name an arbitrary game name, like "Crystal Core".
   */
  virtual void SetGameName (csString game_name) = 0;

  /**
   * Return the name of the game. Client and server instances can talk 
   * together only if they have the same game name and protocol version.
   */
  virtual csString GetGameName () const = 0;

  /**
   * Set the name of the network protocol version used by this application.
   * Client and server instances can talk together only if they have the
   * same game name and protocol version.
   * <P>A protocol version must be set before any operation on the factory.
   * <P>The protocol version should be changed each time something has been 
   * changed in the network protocol of the game, ie in the meaning of the
   * network events, like changes in the implementation of the network
   * plugin, the selection of property classes of the entities, the client
   * and server events, the network links and the baselines.
   * \param protocol_version an arbitrary protocol version name, like "v0.3".
   */
  virtual void SetProtocolVersion (const csString protocol_version) = 0;

  /**
   * Return the network protocol version used by this application. Client
   * and server instances can talk together only if they have the same game
   * name and protocol version. 
   */
  virtual csString GetProtocolVersion () const = 0;

  /**
   * Register a manager for this game factory. The manager will be notified
   * when events are thrown from the game factory. Only one manager can be
   * registered at a time.
   * <P>A manager must be registered before any operation on the factory.
   */
  virtual void RegisterGameFactoryManager (
        celGameFactoryManager* manager) = 0;

  /**
   * Start to search for the list of available games with the same game name, 
   * protocol version and game type. The registered celGameFactoryManager 
   * will be notified through celGameFactoryManager::GameInfoReceived each 
   * time a game has been found. A celGameInfo object can be provided to 
   * filter the games.
   * \param filter a new game info will be received only if 
   * celGameInfo::MatchFilter returns true with this filter.
   */
  virtual void StartSearchForGameList (celNetworkGameType game_type, 
        celGameInfo* filter = 0) = 0;

  /**
   * Stop searching for the list of available games. No more 
   * celGameFactoryManager::GameInfoReceived will be raised.
   */
  virtual void StopSearchForGameList () = 0;

  /**
   * Return the current game or 0 if no game is available.
   */
  virtual iCelGame* GetCurrentGame () const = 0;

  /**
   * Create a new game, ie at least a server, but also a client if a player is 
   * provided. Only one game can be created at a time, so if a game is already 
   * running, it will be closed before the new game is launched. If the game 
   * factory was searching for games, it will stop its search.
   * \param game_type the type of the game. It can be CEL_NET_SINGLEPLAYER, 
   * CEL_NET_LOCAL or CEL_NET_PUBLIC.
   * \param game_info the informations on the game.
   * \param player the player launching the game. He can be given special 
   * access rights to modify the game. If player is different from 0, then 
   * a client will also be created.
   * \return true if the game can be created.
   */
  virtual bool CreateNewGame (celNetworkGameType game_type, 
	celGameInfo* game_info, celPlayer* player = 0) = 0;

  /**
   * Connect the player to the game specified in game_info. If a game is
   * already running, it will be closed before the new game is launched. The
   * game_info field must contain the necessary data to find the server, ie a
   * game_id field previously returned by
   * celGameFactoryManager::GameInfoReceived, or the ip_address and port_nb
   * fields. If the game factory was searching for games, it will stop its
   * search.
   * \param game_info the informations on the game we want to connect to.
   * \param player the player which is connecting to the game.
   * \return true if the game can be connected. 
   */
  virtual bool ConnectGame (celGameInfo* game_info, 
        celPlayer* player) = 0;

  /**
   * The player leaves the current game or playback. If a server was also set, 
   * it will be closed.
   * \return true if the game can be closed, false otherwise.
   */
  virtual bool CloseGame () = 0;

  /**
   * Incoming events are recorded in a file. A game must previously be running.
   */
  virtual void StartRecordingGame (csString filename) = 0;

  /**
   * Incoming events are no more recorded in a file.
   */
  virtual void StopRecordingGame () = 0;

  /**
   * Start a game where incoming events are read from a file from start_time to 
   * stop_time. CloseGame can be used to stop the playback. If the game
   * factory was searching for available games or if a game was running, they
   * will be stopped.
   */
  virtual void StartPlayBackGame (csString filename, 
        csTicks start_time, csTicks stop_time) = 0;
};

/**
 * This is an interface you should implement in order to manage the events from 
 * the factory of game sessions. You can use it to:
 * <UL>
 * <LI>receive the information on new games accessible.</LI>
 * <LI>catch the changes of the connection state from the client to the
 *     server.</LI> 
 * <LI>initialize the client and the server.</LI>
 * <LI>provide the baselines of the client and server events, and of the
 *     network links.</LI>
 * </UL>
 */
class celGameFactoryManager
{
 public:
  /**
   * The game factory is searching for available games (it has been started
   * with iCelGameFactory::StartSearchForGameList), and a game has been found.
   * \param game_info the info of the game.
   * \param server_latency an estimation of the latency of the network
   *        connection 
   * from this client to the server.
   */
  virtual void GameInfoReceived (celGameInfo* game_info, 
        csTicks server_latency) {}

  /**
   * The state of the network connection from the client to the server has 
   * changed. An extra comment can be specified when new_state is:
   * <UL>
   * <LI>CEL_NET_SERVER_INVALID_HOSTNAME: reason is the value of the
   *     hostname.</LI>
   * <LI>CEL_NET_SERVER_REJECTED_BAD_GAME: reason is the name of the game
   *     played by the server.</LI>
   * <LI>CEL_NET_SERVER_REJECTED_BAD_PROTOCOL: reason is the name of the
   *     protocol version used by the server.</LI>
   * <LI>CEL_NET_SERVER_REJECTED_UNAUTHORIZED: reason is the text given by
   *     the server to explain why this player has not been authorized to
   *     enter the game.</LI>
   * <LI>CEL_NET_SERVER_KICKED: reason is the text given by the server to
   *     explain why this player has been kicked from the game.</LI>
   * </UL>
   */
  virtual void ServerNetworkStateChanged (celServerNetworkState new_state, 
        celServerNetworkState previous_state, csString reason = "") = 0;

  /**
   * A client is created and has to be initialized. The iCelGameClient object
   * is accessible from the iCelGame object.
   * <P>You should create here a physical layer which will be the copy of the 
   * physical layer of the server. You should wait for network links from the 
   * server before adding most entities, but you can start loading the level,
   * ie the static entities. If the type of the game is CEL_NET_SINGLEPLAYER,
   * you do not need to create a physical layer and you can access directly
   * the one from the server.
   * <P>You should register here the client manager.
   */
  virtual bool InitClient (iCelGame* game) { return true; }

  /**
   * A server is created and has to be initialized. The iCelGameServer object
   * is accessible from the celGame object.
   * <P>You should create here the physical layer, the initial entities and
   * their behaviors.
   * <P>You should register here the server manager.
   */
  virtual bool InitServer (iCelGame* game) { return true; }

  /**
   * Return the baseline of a client event. Baselines are the default types and 
   * values of the fields of an event. They are used to limit the network
   * traffic by delta-compression and by allowing the system to not be forced
   * to send the types of the data.
   * <P>It is not obligatory to provide the baselines and this 
   * function can return 0, but this can result in some network overhead.
   */
  virtual csPtr<iCelDataBuffer> GetClientEventBaseline (
        celClientEventType event_type) { return 0; }

  /**
   * Return the baseline of a server event. It is not obligatory to provide the 
   * baselines and this function can return 0, but this can result in some 
   * network overhead.
   */
  virtual csPtr<iCelDataBuffer> GetServerEventBaseline (
        celServerEventType event_type) { return 0; }

  /**
   * Return the baseline of a network link. It is not obligatory to provide the 
   * baselines and this function can return 0, but this can result in some 
   * network overhead.
   */
  virtual csPtr<iCelDataBuffer> GetNetworkLinkBaseline (
        celNetworkLinkType link_type) { return 0; }

};

SCF_VERSION (iCelGameServer, 0, 0, 1);

/**
 * The main object to manage the server game logic. You can use it to:
 * <UL>
 * <LI>get and set the administrator of the game, get the list of players of 
 * the game, kick a player from the game.</LI>
 * <LI>create and remove channels, ie list of players.</LI>
 * <LI>send server events, create and remove network links.</LI>
 * <LI>re-initialize a game for example when a new level is started.</LI>
 * <LI>set some options on the server.</LI>
 * <LI>get some network statistics.</LI>
 * </UL>
 * You should define and register a celGameServerManager which will catch 
 * the events from this server.
 */
struct iCelGameServer : public iBase
{
  /**
   * Register the manager of this server. Only one manager can be registered
   * at a time.
   */
  virtual void RegisterServerManager (celGameServerManager* manager) = 0;

  /**
   * Set the administrator of the game. It doesn't need to be a player already 
   * connected. 
   */
  virtual void SetAdministrator (celPlayer* new_admin) = 0;

  /**
   * Return the administrator of the game. The return value can be 0 if no 
   * administrator was defined in iCelGameFactory::CreateGame.
   */
  virtual celPlayer* GetAdministrator () = 0;

  /**
   * Return the list of players connected to the server.
   */
  virtual iCelPlayerList* GetPlayerList () = 0;

  /**
   * Return the state of the network connection to the player.
   */
  virtual celPlayerNetworkState GetPlayerState (celPlayer* player) = 0;

  /**
   * Kick the player from the game, ie the player will be forced to 
   * disconnect from the game. A text explaining the reason can be 
   * specified.
   */
  virtual void KickPlayer (celPlayer* player, csString reason) = 0;

  /**
   * Create a channel, ie a list of players. The list will be managed by the 
   * server which will listen to players disconnecting and will so remove them 
   * from the list. Channels are useful to send the same server events or
   * create the same network links to a list of players.
   * \param channel_id the ID of the channel
   */
  virtual iCelPlayerList* CreateChannel (csStringID channel_id) = 0;

  /**
   * Stop managing the list of players and delete it. Any existing network
   * links to this channel will also be removed.
   */
  virtual void RemoveChannel (csStringID channel_id) = 0;

  /**
   * A server event has to be sent to a player.
   */
  virtual void LaunchServerEvent (celPlayer* player, 
        celServerEventData &event_data) = 0;

  /**
   * A server event has to be sent to the players in the channel.
   */
  virtual void LaunchServerEvent (csStringID channel_id, 
        celServerEventData &event_data) = 0;

  /**
   * Establish a link between an entity and a player. When a link is set up, 
   * a copy entity will be created on the client side and the server manager 
   * will send periodically a part of the state of the entity to the copy 
   * entity.
   * <P>If the entity is controlled on the client side, the network link will 
   * be in the other direction and the state of the entity will be sent 
   * periodically from the client side to the server side.
   * <P>If a link between the entity and the player is already present, the 
   * previous link will be removed.
   * \param player the player linked to the entity
   * \param link_data the data of the network link
   * \param player_controlled false if the entity is controlled by the server,
   * true if it is by the client (ie this is the client that will update the
   * entity and send its state to the server, instead of the contrary). 
   */
  virtual void SetNetworkLink (celPlayer* player, 
        celNetworkLinkData &link_data, bool player_controlled) = 0;

  /**
   * Define if it is the client or the server that will update the entity. A
   * network link between the player and the entity must be present.
   * \param entity the entity network linked to the player
   * \param player the player network linked to the entity
   * \param player_controlled false if the entity is controlled by the server,
   * true if it is by the client (ie this is the client that will update the
   * entity and send its state to the server, instead of the contrary). 
   */
  virtual void ChangeLinkControl (iCelEntity* entity, celPlayer* player, 
        bool player_controlled) = 0;

  /**
   * Establish a link between a source entity and the players in a list. When a 
   * link is set up, a copy entity will be created on each client side and the 
   * server manager will send periodically a part of the state of the entity
   * to the copy entity.
   * <P>If a link between the entity and the channel is already present, the 
   * previous link will be removed.
   * \param channel_id the ID of the channel
   * \param link_data the data of the network link
    */
  virtual void SetNetworkLink (csStringID channel_id, 
        celNetworkLinkData &link_data) = 0;

  /**
   * Remove the existing link between the entity and the player, ie the state
   * of the entity will no more be sent to the player.
   */
  virtual void RemoveNetworkLink (iCelEntity* entity, celPlayer* player) = 0;

  /**
   * Remove the existing link between the entity and the channel, ie the state 
   * of the entity will no more be sent to the players in the list.
   */
  virtual void RemoveNetworkLink (iCelEntity* entity,
  	csStringID channel_id) = 0;

  /**
   * Level change, the game is re-initialized. Clear all pending network links 
   * and server events. The iCelGameClientManager of all players will be called 
   * with celGameClientManager::LevelChanged. All the players are marked as 
   * CEL_NET_PLAYER_CONNECTING, they will need to do iCelGameClient::SetReady.
   * \param level_name a significant name for the new level.
   */
  virtual void ChangeLevel (celGameInfo* new_game) = 0;

  /**
   * Specify the maximum frequency at which the server can send data to clients.
   * \param period the number of ticks between two updates from the 
   * server. A period of 0 means "as soon as possible".
   */
  virtual void SetNetworkPeriod (csTicks period) = 0;

  /**
   * Specify the maximum bandwidth of the network data transmission in bytes per 
   * second. A bandwidth of 0 means that it is unlimited.
   */
  virtual void SetMaximumBandwidth (size_t width) = 0;

  /**
   * The server will consider a client connected until the connection is 
   * broken (network state CEL_NET_PLAYER_UNREACHABLE) during the specified
   * time.
   */
  virtual void SetClientTimeOut (csTicks timeout) = 0;

  /**
   * Return the network stats of the specified player.
   */
  virtual void GetNetworkPlayerStats (celPlayer* player, 
        celNetworkPlayerStats& stats) const = 0;

  /**
   * Return the network stats of this server.
   */
  virtual void GetNetworkTotalStats (
  	celNetworkPlayerTotalStats& stats) const = 0;
};

SCF_VERSION (iCelGameClient, 0, 0, 1);

/**
 * The main object to manage the client game logic. You can use it to:
 * <UL>
 * <LI>update the data of the player.</LI>
 * <LI>convert a server clock or a server entity ID in client values.</LI>
 * <LI>tell the system when the player is ready to play.</LI>
 * <LI>send client events.</LI>
 * <LI>set some options on the client.</LI>
 * <LI>get some network statistics.</LI>
 * </UL>
 * You should define and register a celGameClientManager which will catch 
 * the events from this client.
 */
struct iCelGameClient : public iBase
{
  /**
   * Register the manager of this client. Only one manager can be registered at 
   * a time.
   */
  virtual void RegisterClientManager (celGameClientManager* manager) = 0;

  /**
   * Return the current player associated with this client.
   */
  virtual celPlayer* GetPlayer () = 0;

  /**
   * Update the data of the current player. The server will check if the new
   * data are valid and a subsequent celGameClientManager::PlayerUpdated will
   * be called with the validated data.
   */
  virtual void UpdatePlayer (celPlayer* player) = 0;

  /**
   * Convert a time from the server clock to the client clock.
   */
  virtual csTicks ConvertServerClock (csTicks server_time) const = 0;

  /**
   * Return the ID of the entity on the client side that is network linked with 
   * an entity with ID entity_id on the server side.
   */
  virtual uint GetMappedID (uint entity_id) const = 0;

  /**
   * Return the entity on the client side that is network linked with an entity 
   * with ID entity_id on the server side.
   */
  virtual iCelEntity* GetMappedEntity (uint entity_id) const = 0;

  /**
   * Indicate that the client has loaded everything and is ready to play.
   */
  virtual void SetReady () = 0;

  /**
   * Ask the game client to send an event to the server.
   */
  virtual void LaunchClientEvent (celClientEventData &event_data) = 0;

  /**
   * Specify the maximum frequency at which the client can send data.
   */
  virtual void SetNetworkPeriod (csTicks period) = 0;

  /**
   * The maximum bandwidth of the network data transmission in bytes per 
   * second. A bandwidth of 0 means that it is unlimited.
   */
  virtual void SetMaximumBandwidth (size_t width) = 0;

  /**
   * The client will consider itself connected to the server until the 
   * connection is broken during specified time.
   */
  virtual void SetServerTimeOut (csTicks timeout) = 0;

  /**
   * Return the network stats of this client.
   */
  virtual void GetNetworkStats (celNetworkServerStats& stats) const = 0;
};

/**
 * This is an interface you should implement in order to manage the events from 
 * the server. You can use it to:
 * <UL>
 * <LI>authorize players to join the game, check the validity of the data of
 * the players.</LI>
 * <LI>listen for changes in the state of the network connections to the 
 * clients.</LI>
 * <LI>catch the client events.</LI>
 * <LI>handle the detection of cheats.</LI>
 * <LI>close the game.</LI>
 * </UL>
 */
class celGameServerManager
{
 public:
  /**
   * A player is asking to join the game. Return true if the new player is 
   * accepted, false otherwise (for example, because the player has been 
   * banned). A text explaining the reason of a negative answer can be 
   * specified.
   */
  virtual bool AuthorizePlayer (celPlayer* player, csString &reason) = 0;

  /**
   * Check if the new data of a player are valid. It happens when
   * iCelGameClient::UpdatePlayer has been called on the client side.
   * \return the validated data of the player.
   */
  virtual celPlayer* ValidatePlayerUpdate (celPlayer* previous_player_data) = 0;

  /**
   * The state of the network connection to the player has changed.
   */
  virtual void PlayerNetworkStateChanged (celPlayer* player, 
        celPlayerNetworkState new_state, 
        celPlayerNetworkState previous_state) = 0;

  /**
   * A client event has been catched.
   */
  virtual void HandleClientEvent (celPlayer* player, 
        celClientEventType event_type, csTicks event_time, 
        iCelDataBuffer* event_data) = 0;

  /**
   * An entity is controlled by a player and a problem was encountered while
   * updating the data of this entity.
   * \param player The player controlling the entity.
   * \param entity The entity controlled by the player.
   * \param pc The property class updated while the problem was encountered.
   * \param persist_code The code of the problem.
   */
  virtual void PersistenceProblem (celPlayer* player, iCelEntity* entity,
	iCelPropertyClass* pc, celPersistenceResult persist_code) = 0;

  /**
   * The game is finished and the server will be closed. You should delete
   * here all entities and the physical layer.
   */
  virtual void ServerEnd () = 0;
};

/**
 * This is an interface you should implement in order to manage the events from 
 * the client. You can use it to:
 * <UL>
 * <LI>listen for changes in the state of the network connection to the 
 * server.</LI>
 * <LI>get the data of the player validated by the server.</LI>
 * <LI>catch the server events.</LI>
 * <LI>handle the creation and removing of the network links.</LI>
 * <LI>close the client.</LI>
 * </UL>
 */
class celGameClientManager
{
 public:
  /**
   * The data of the player has been validated by the server.
   */
  virtual void PlayerValidated (celPlayer* player) = 0;

  /**
   * A server event has been catched.
   */
  virtual void HandleServerEvent (celServerEventType event_type, 
        csTicks event_time, iCelDataBuffer* event_data) = 0;

  /**
   * A network link has been set up on the server side between a server entity 
   * and this client. A copy entity must be created on this client side. This
   * copy entity will be the adressee of the updates from the network link.
   * <P>If the server is available in the same process, you can simply use the 
   * entity of the server and then return 0 to this function.
   * \param link_type the type of the network link. You should use it to know
   * what type of entity has to be created.
   * \param creation_time the time at which the network link is created.
   * \param creation_data the persistent data of the entity at the creation of
   * the network link.
   * \param player_controlled true if the entity is controlled on the client
   * side, false it is controlled on the server side.
   * \return the copy entity.
   */
  virtual iCelEntity* CreateNetworkLinkTarget (celNetworkLinkType link_type, 
        csTicks creation_time, iCelPersistentDataList* creation_data, 
        bool player_controlled) = 0;

  /**
   * There is a change in the way the entity is controlled, ie if it is 
   * controlled by the client or by the server side.
   * <P>If the entity is now controlled on the client side, you should then set 
   * a behavior to it. Most presumably, the behavior will catch the actions of 
   * the player and update accordingly the entity. 
   * <P>If the entity is now controlled on the server side, you should then
   * remove any behavior from the entity and let the client update it from the
   * server.
   * \param entity the entity which is networked linked.
   * \param player_controlled true if the entity is controlled by the client 
   * side, false if it is controlled by the server side.
   */
  virtual void NetworkLinkControlChanged (iCelEntity* entity, 
        bool player_controlled) = 0;

  /**
   * The network link to the specified entity is closing. The client can remove 
   * the entity from the physical layer.
   */
  virtual void NetworkLinkRemoved (csTicks deletion_time,
  	iCelEntity* entity) = 0;

  /**
   * The level has changed. The client need to do iCelGameClient::SetReady when 
   * the new level is loaded.
   */
  virtual void LevelChanged (celGameInfo* new_game) = 0;

  /**
   * The client will be closed. You should delete here the physical layer
   * of the client.
   */
  virtual void ClientEnd () = 0;
};

#endif // __CEL_PL_NETWORK__
