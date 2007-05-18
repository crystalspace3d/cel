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

#ifndef __CEL_PL_NETTYPES__
#define __CEL_PL_NETTYPES__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/csstring.h"
#include "csutil/bitarray.h"

#include "physicallayer/persist.h"
#include "physicallayer/entity.h"
#include "celtool/persisthelper.h"

struct iCelEntity;
class celGameInfoList;
struct celGameInfoListIterator;
struct iCelGameServer;
struct iCelGameClient;
class csBitArray;
struct celPlayerListIterator;

/**
 * The different game types. The values can be:
 * <UL>
 * <LI>CEL_NET_UNDEF: Undefined, should never be encountered.</LI>
 * <LI>CEL_NET_SINGLEPLAYER: Only one player.</LI>
 * <LI>CEL_NET_LOCAL: Open to other players from the local network</LI>
 * <LI>CEL_NET_PUBLIC: Open to other players from Internet</LI>
 * <LI>CEL_NET_PLAYBACK: Game played back from a file </LI>
 * </UL>
 */
enum celNetworkGameType
{
  CEL_NET_UNDEF = 0,
  CEL_NET_SINGLEPLAYER,
  CEL_NET_LOCAL,
  CEL_NET_PUBLIC,
  CEL_NET_PLAYBACK
};

/// network event types
// TODO: these types must be changed if there are more than 256 values
typedef uint8 celClientEventType;
typedef uint8 celServerEventType;
typedef uint8 celNetworkLinkType;

/**
 * The server view of the state of the network connection to a player. 
 * The values can be:
 * <UL>
 * <LI>CEL_NET_PLAYER_UNDEF: Undefined, should never be encountered.</LI>
 * <LI>CEL_NET_PLAYER_NOT_CONNECTED: The player is not connected to the game.</LI>
 * <LI>CEL_NET_PLAYER_CONNECTING: The player is connected to the game and 
 * is now loading needed data.</LI>
 * <LI>CEL_NET_PLAYER_PLAYING: The player has loaded everything and is 
 * ready to play.</LI>
 * <LI>CEL_NET_PLAYER_DISCONNECTED: The player has cleanly left the game.</LI>
 * <LI>CEL_NET_PLAYER_UNREACHABLE: The player is no more reachable, ie the 
 * connection has been broken or the client has crashed.</LI>
 * <LI>CEL_NET_PLAYER_LOST: The player was unreachable, and the connection
 * to the player is now definitively lost.</LI>
 * </UL>
 */
enum celPlayerNetworkState
{
  CEL_NET_PLAYER_UNDEF = 0,
  CEL_NET_PLAYER_NOT_CONNECTED,
  CEL_NET_PLAYER_CONNECTING,
  CEL_NET_PLAYER_PLAYING,
  CEL_NET_PLAYER_DISCONNECTED,
  CEL_NET_PLAYER_UNREACHABLE,
  CEL_NET_PLAYER_LOST
};

/**
 * The client view of the state of the network connection to the server. 
 * The values can be:
 * <UL>
 * <LI>CEL_NET_SERVER_UNDEF: Undefined, should never be encountered.</LI>
 * <LI>CEL_NET_SERVER_NOT_CONNECTED: The player is not connected to the game.</LI>
 * <LI>CEL_NET_SERVER_INVALID_HOSTNAME: The hostname of the server is not 
 * valid.</LI>
 * <LI>CEL_NET_SERVER_TRYING_CONNECTION: The system is trying to connect to the 
 * server.</LI>
 * <LI>CEL_NET_SERVER_CONNECTING: The player is connected to the game and is 
 * waiting for an acknowledgment from the server.</LI>
 * <LI>CEL_NET_SERVER_REJECTED_BAD_GAME: The client has a game name different 
 * of the one of the server.</LI>
 * <LI>CEL_NET_SERVER_REJECTED_BAD_PROTOCOL: The client has a protocol version 
 * different of the one of the server.</LI>
 * <LI>CEL_NET_SERVER_REJECTED_BAD_PASSWORD: The client do not know the needed 
 * password.</LI>
 * <LI>CEL_NET_SERVER_REJECTED_SINGLEPLAYER: The server is playing a single 
 * player game.</LI>
 * <LI>CEL_NET_SERVER_REJECTED_UNAUTHORIZED: The server manager has not 
 * authorized the player to connect to the game.</LI>
 * <LI>CEL_NET_SERVER_REJECTED_MAX_PLAYERS: The maximum number of players is 
 * already reached.</LI>
 * <LI>CEL_NET_SERVER_LOADING_DATA: The player is connected to the server and 
 * is now loading the level.</LI>
 * <LI>CEL_NET_SERVER_PLAYING: The player is now playing.</LI>
 * <LI>CEL_NET_SERVER_DISCONNECTED: The server has cleanly closed the session.</LI>
 * <LI>CEL_NET_SERVER_UNREACHABLE: The server is no more reachable, ie the 
 * connection has been broken or the server has crashed.</LI>
 * <LI>CEL_NET_SERVER_LOST: The server was unreachable, and the connection 
 * to the server is now definitively lost.</LI>
 * <LI>CEL_NET_SERVER_KICKED: The server has kicked the player.</LI>
 * </UL>
 */
enum celServerNetworkState
{
  CEL_NET_SERVER_UNDEF = 0,
  CEL_NET_SERVER_NOT_CONNECTED,
  CEL_NET_SERVER_INVALID_HOSTNAME,
  CEL_NET_SERVER_TRYING_CONNECTION,
  CEL_NET_SERVER_CONNECTING,
  CEL_NET_SERVER_REJECTED_BAD_GAME,
  CEL_NET_SERVER_REJECTED_BAD_PROTOCOL,
  CEL_NET_SERVER_REJECTED_BAD_PASSWORD,
  CEL_NET_SERVER_REJECTED_SINGLEPLAYER,
  CEL_NET_SERVER_REJECTED_UNAUTHORIZED,
  CEL_NET_SERVER_REJECTED_MAX_PLAYERS,
  CEL_NET_SERVER_LOADING_DATA,
  CEL_NET_SERVER_PLAYING,
  CEL_NET_SERVER_DISCONNECTED,
  CEL_NET_SERVER_UNREACHABLE,
  CEL_NET_SERVER_LOST,
  CEL_NET_SERVER_KICKED
};

/**
 * General information on a game.
 */
class celGameInfo
{
 public:
  /// The ID of the game.
  uint32 game_id;

  /// The name of the game.
  csString game_name;

  /// The hostname of the server if available.
  csString hostname;

  /// The IP address of the game/server, large enough to hold IPv6 address.
  uint8 ip_address[32];

  /// The port number of the server.
  uint16 port_nb;

  /// The maximum number of players for this game.
  size_t max_players;

  /// The current number of players.
  size_t current_num_players;

  /// The password needed to connect to the game.
  csString password;

  /**
   * Some custom data specific to the application. The interpretation of this 
   * field is up to the application.
   */
  csRef<iCelDataBuffer> custom_data;

  celGameInfo ()
  {
    game_id = 0;
    memset (ip_address, 0, 32);
    port_nb = 0;
    max_players = 0;
    current_num_players = 0;
    custom_data = 0;
  }

  /**
   * Return true if this game info matches the filter, false otherwise.
   */
  bool MatchFilter (celGameInfo* filter);

  /**
   * Return 0 if this game info is equal to the other, a negative value if it 
   * is smaller, and a positive value if it is bigger. This can be used to sort 
   * a list of celGameInfo and display it to the user according to his
   * criterion.
   */
  int Compare (celGameInfo* other, celGameInfoList* filters);
};

/**
 * A list of general info of games.
 */
class celGameInfoList
{
 public:
  virtual ~celGameInfoList () = 0;
  virtual size_t GetCount () const = 0;
  virtual celGameInfo* Get (size_t index) const = 0;
  virtual size_t Add (celGameInfo* player) = 0;
  virtual bool Remove (celGameInfo* player) = 0;
  virtual bool Remove (size_t n) = 0;
  virtual void RemoveAll () = 0;
  virtual size_t Find (celGameInfo* player) const = 0;

  /**
   * Filter the list of games.
   * <P>Note: this interface should be improved
   */
  void Filter (celGameInfo* game_info);

  /**
   * Sort the list of games.
   * <P>Note: this interface should be improved
   */
  void Sort (celGameInfoList* filters);
};

/**
 * General information on a player.
 */
class celPlayer
{
 public:
  virtual ~celPlayer () {}

  /// The ID of the player.
  uint32 player_id;

  /// The name of the player.
  csString player_name;

  /// The hostname of the player if available.
  csString hostname;

  /// The IP address of the game/player, large enough to hold IPv6 address.
  uint8 ip_address[32];

  /// The port number of the player.
  uint16 port_nb;

  celPlayer ()
  {
    player_id = 0;
    memset (ip_address, 0, 32);
    port_nb = 0;
  }

  bool operator == (const celPlayer& other) const
  {
    return hostname.Compare(other.hostname) && port_nb == other.port_nb;
  }

  void PrintDebugInfo ()
  {
    printf("Player data:\n");
    printf("\tID: %d\n", player_id);
    printf("\tname: %s\n", player_name.GetData());
    printf("\thostname: %s\n", hostname.GetData());
    printf("\taddress: ");
    int i = 0;
    for ( ; i < 32; i++)
      printf("%d", ip_address[i]);
    printf("\n");
    printf("\tport: %d\n", port_nb);
  }
};

SCF_VERSION (iCelPlayerList, 0, 0, 1);

/**
 * A list of players.
 */
struct iCelPlayerList
{
 public:
  virtual ~iCelPlayerList () = 0;
  virtual size_t GetCount () const = 0;
  virtual celPlayer* Get (size_t index) const = 0;
  virtual size_t Add (celPlayer* player) = 0;
  virtual bool Remove (celPlayer* player) = 0;
  virtual bool Remove (size_t n) = 0;
  virtual void RemoveAll () = 0;
  virtual size_t Find (celPlayer* player) const = 0;
  //virtual bool Contains (celPlayer* player) = 0;
};

/**
 * The data about a server event.
 */
class celServerEventData
{
 public:
  /**
   * The type of the event.
   */
  celServerEventType event_type;

  /**
   * The time at which the event occured.
   */
  csTicks event_time;

  /**
   * The persistent data of the event.
   */
  csRef<iCelDataBuffer> event_data;

  /**
   * True if we need to be sure that the message has been received by the
   * client.
   */
  bool reliable;
};

/**
 * The data about a client event.
 */
class celClientEventData
{
 public:
  /**
   * The type of the event.
   */
  celClientEventType event_type;

  /**
   * The time at which the event occured.
   */
  csTicks event_time;

  /**
   * The persistent data of the event.
   */
  csRef<iCelDataBuffer> event_data;

  /**
   * True if we need to be sure that the message has been received by the server.
   */
  bool reliable;

  celClientEventData () :
    event_type (0),
    event_time (0),
    event_data (0),
    reliable (true)
    {}

  celClientEventData (celClientEventData &event) :
    event_type (event.event_type),
    event_time (event.event_time),
    event_data (event.event_data),
    reliable (event.reliable)
    {}
};

/**
 * The data about a network link.
 */
class celNetworkLinkData
{
 public:
  /**
   * The type of the source of the network link.
   */
  celNetworkLinkType link_type;

  /**
   * The source of the network link.
   */
  csRef<iCelEntity> linked_entity;

  /**
   * A mask to select needed fields of the persistent data that will be sent.
   */
  csBitArray persistence_mask;

  /**
   * The time period between the sending of updates. A period of 0 means that 
   * the link should be updated only once. A period of 1 means that it should be 
   * updated it as soon as possible.
   */
  csTicks period;

  celNetworkLinkData () {}

  celNetworkLinkData (celNetworkLinkType _link_type, iCelEntity* _linked_entity,
		      csBitArray _persistence_mask, csTicks _period) :
    link_type (_link_type),
    linked_entity (_linked_entity),
    persistence_mask (_persistence_mask),
    period (_period)
    {}

  ~celNetworkLinkData () {}
};

/**
 * The network stats of the connection viewed by the client
 * <P>Note: this interface should be improved
 */
struct celNetworkServerStats
{
  // The latency of the network transmission to the player
  csTicks latency;
  // The state of the connection
  celServerNetworkState network_state;
  // Instant flow of the data sent by the client, in bytes per second
  size_t incoming_bandwidth;
  // Instant flow of the data sent by the server to this client, in bytes
  // per second
  size_t outgoing_bandwidth;
};

/**
 * The network stats of the connection viewed by the server
 * <P>Note: this interface should be improved
 */
struct celNetworkPlayerStats
{
  // The latency of the network transmission to the player
  csTicks latency;
  // Instant flow of the data sent by the client, in bytes per second
  size_t incoming_bandwidth;
  // Instant flow of the data sent by the server to this client, in bytes
  // per second
  size_t outgoing_bandwidth;
};

/**
 * The network stats of the connection of all the players viewed by the server
 * <P>Note: this interface should be improved
 */
struct celNetworkPlayerTotalStats
{
  // Instant flow of all the data sent by the clients, in bytes per second
  size_t total_incoming_bandwidth;
  // Instant flow of all the data sent by the server to the clients, in bytes
  // per second
  size_t total_outgoing_bandwidth;
};

#endif // __CEL_PL_NETTYPES__
