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
#include "ivaria/reporter.h"
#include "csutil/eventnames.h"
#include "csutil/cseventq.h"
#include "csutil/event.h"

#include "nl.h"

#include "physicallayer/pl.h"
#include "plugins/network/tcp/tcputils.h"
#include "plugins/network/tcp/tcpgamefactory.h"

CS_IMPLEMENT_PLUGIN

CS_PLUGIN_NAMESPACE_BEGIN(celTCPNetwork)
{

SCF_IMPLEMENT_IBASE (celTCPGameFactory)
  SCF_IMPLEMENTS_INTERFACE (iCelGameFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celTCPGameFactory::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celTCPGameFactory::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_FACTORY (celTCPGameFactory)

celTCPGameFactory::celTCPGameFactory (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);

  object_reg = 0;
  scfiEventHandler = 0;
  manager = 0;
  game = 0;
  game_name = "";
  protocol_version = "";
}

celTCPGameFactory::~celTCPGameFactory ()
{
  // close current game
  CloseGame ();

  // close networking
  nlShutdown ();

  // remove the callback listening to each frame
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
    if (q != 0)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }

  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celTCPGameFactory::Initialize (iObjectRegistry* object_reg)
{
  celTCPGameFactory::object_reg = object_reg;
  BufferReporter::SetRegistry (object_reg);

  CS_INITIALIZE_EVENT_SHORTCUTS (object_reg);

  // register callback to every frame
  scfiEventHandler = new EventHandler (this);
  csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
  csEventID events[] = { PreProcess, PostProcess, CS_EVENTLIST_END };
  q->RegisterListener (scfiEventHandler, events);

  // init network library
  if (!nlInit ())
    return ReportNetworkError (object_reg, "Failed to initialize network library");
  if (!nlSelectNetwork (NL_IP))
    return ReportNetworkError (object_reg, "Failed to select IP protocol");
  
  return true;
}

bool celTCPGameFactory::HandleEvent (iEvent& ev)
{
  if (!ev.Broadcast) return false;

  // TODO: listen for game info received
 
  // update client and server
  if (game) {
    if (game->IsServerAvailable ()) game->server->UpdateNetwork ();
    if (game->IsClientAvailable ()) game->client->UpdateNetwork ();
  }

  return true;
}

void celTCPGameFactory::StartSearchForGameList (celNetworkGameType game_type, 
        celGameInfo* filter)
{
  // @@@ TODO
}

void celTCPGameFactory::StopSearchForGameList ()
{
  // @@@ TODO
}

iCelGame* celTCPGameFactory::GetCurrentGame () const
{
  if (!game) return 0;
  else return &game->scfiCelGame;
}

bool celTCPGameFactory::CreateNewGame (celNetworkGameType game_type, 
	celGameInfo* game_info, celPlayer* game_admin)
{
  // check if the data are valid
  if (game_name.IsEmpty ())
    return ReportError (object_reg, "No game name defined");
  if (protocol_version.IsEmpty ())
    return ReportError (object_reg, "No game protocol version defined");
  if (game_type != CEL_NET_SINGLEPLAYER
      && game_type != CEL_NET_LOCAL
      && game_type != CEL_NET_PUBLIC)
    return ReportError (object_reg, "Invalid game type");
  if (!game_info)
    return ReportError (object_reg, "No game info defined");
  if (!manager)
    return ReportError (object_reg, "No game factory manager defined");

  // close any existing game
  CloseGame ();

  // create game
  game = new celTCPGame (game_type, game_info);

  // create a server if needed
  if (game_type == CEL_NET_SINGLEPLAYER
      || game_type == CEL_NET_LOCAL
      || game_type == CEL_NET_PUBLIC)
  {
    game->server = new celTCPGameServer (object_reg, this);
    if (!game->server->InitializeServer (game, game_admin))
    {
      delete game;
      game = 0;
      return ReportError (object_reg, "Failed to initialize server");
    }
  }

  // create a client if needed
  if (game_type == CEL_NET_SINGLEPLAYER ||
      ( (game_type == CEL_NET_LOCAL
	 || game_type == CEL_NET_PUBLIC)
	&& game_admin))
  {
    game->client = new celTCPGameClient (object_reg, this);
    if (!game->client->InitializeClient (game, game_admin))
    {
      delete game;
      game = 0;
      return ReportError (object_reg, "Failed to initialize client");
    }
  }

  return true;
}

bool celTCPGameFactory::ConnectGame (celGameInfo* game_info, 
        celPlayer* player)
{
  // check if the data are valid
  if (game_name.IsEmpty ())
    return ReportError (object_reg, "No game name defined");
  if (protocol_version.IsEmpty ())
    return ReportError (object_reg, "No game protocol version defined");
  if (!game_info)
    return ReportError (object_reg, "No game info defined");
  if (!player)
    return ReportError(object_reg, "No player defined");
  if (!manager)
    return ReportError (object_reg, "No game factory manager defined");

  // TODO: check if the game info have to be read from the known public games

  // close any existing game
  CloseGame();

  // create game
  game = new celTCPGame (CEL_NET_UNDEF, game_info);

  // create client
  game->client = new celTCPGameClient (object_reg, this);
  if (!game->client->InitializeClient (game, player))
  {
    delete game;
    game = 0;
    return ReportError (object_reg, "Failed to initialize client");
  }

  return true;
}

bool celTCPGameFactory::CloseGame ()
{
  if (!game)
    return false;

  delete game;
  game = 0;
  return true;
}

void celTCPGameFactory::StartRecordingGame (csString filename)
{
  // @@@ TODO
}

void celTCPGameFactory::StopRecordingGame ()
{
  // @@@ TODO
}

void celTCPGameFactory::StartPlayBackGame (csString filename, 
				  csTicks start_time, csTicks stop_time)
{
  // @@@ TODO
}

}
CS_PLUGIN_NAMESPACE_END(celTCPNetwork)

