/*
    Copyright (C) 2003 by Jorrit Tyberghein

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

#include "cssys/sysfunc.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "bootstrap.h"
#include "csutil/event.h"
#include "csutil/cmdhelp.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/csinput.h"
#include "iutil/plugin.h"
#include "iutil/vfs.h"
#include "iutil/object.h"
#include "iutil/cmdline.h"
#include "iengine/engine.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "igraphic/imageio.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "ivaria/collider.h"
#include "imap/parser.h"
#include "isound/renderer.h"
#include "isound/loader.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/behave.h"

CS_IMPLEMENT_APPLICATION

//-----------------------------------------------------------------------------

// The global pointer to bootstrap
Bootstrap* bootstrap;

Bootstrap::Bootstrap ()
{
}

Bootstrap::~Bootstrap ()
{
  if (pl)
  {
    pl->CleanCache ();
    if (bootstrap_entity)
      pl->RemoveEntity (bootstrap_entity);
  }
}

void Bootstrap::SetupFrame ()
{
  // We let the entity system do this so there is nothing here.
}

void Bootstrap::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (0);
}

bool Bootstrap::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdProcess)
  {
    bootstrap->SetupFrame ();
    return true;
  }
  else if (ev.Type == csevBroadcast && ev.Command.Code == cscmdFinalProcess)
  {
    bootstrap->FinishFrame ();
    return true;
  }
  else if (ev.Type == csevKeyboard
	&& csKeyEventHelper::GetEventType (&ev) == csKeyEventTypeDown)
  {
    utf32_char code = csKeyEventHelper::GetCookedCode (&ev);
    if (code == CSKEY_ESC)
    {
      csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
      if (q)
        q->GetEventOutlet()->Broadcast (cscmdQuit);
      return true;
    }
  }
  return false;
}

bool Bootstrap::BootstrapEventHandler (iEvent& ev)
{
  return bootstrap->HandleEvent (ev);
}

bool Bootstrap::Initialize (int argc, const char* const argv[])
{
  object_reg = csInitializer::CreateEnvironment (argc, argv);
  if (!object_reg) return false;

  if (!csInitializer::RequestPlugins (object_reg,
  	CS_REQUEST_VFS,
	CS_REQUEST_OPENGL3D,
	CS_REQUEST_ENGINE,
	CS_REQUEST_FONTSERVER,
	CS_REQUEST_IMAGELOADER,
	CS_REQUEST_LEVELLOADER,
	CS_REQUEST_REPORTER,
	CS_REQUEST_REPORTERLISTENER,
	CS_REQUEST_PLUGIN ("cel.physicallayer", iCelPlLayer),
	CS_REQUEST_PLUGIN ("cel.persistance.classic", iCelPersistance),
	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.rapid",
		iCollideSystem),
	CS_REQUEST_PLUGIN ("crystalspace.sound.render.software", iSoundRender),
	CS_REQUEST_PLUGIN ("crystalspace.sound.loader.multiplexer",
		iSoundLoader),
	CS_REQUEST_END))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
	"Can't initialize plugins!");
    return false;
  }

  if (!csInitializer::SetupEventHandler (object_reg, BootstrapEventHandler))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
	"Can't initialize event handler!");
    return false;
  }

  // Check for commandline help.
  if (csCommandLineHelper::CheckHelp (object_reg))
  {
    csCommandLineHelper::Help (object_reg);
    return false;
  }

  // Find the pointer to engine plugin
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (!engine)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
	"No iEngine plugin!");
    return false;
  }

  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  if (!kbd)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
    	"No iKeyboardDriver plugin!");
    return false;
  }

  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  if (!g3d)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
    	"No iGraphics3D plugin!");
    return false;
  }

  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
    	"CEL physical layer missing!");
    return false;
  }
 
  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
  const char* plugin = cmdline->GetName (0);
  const char* behaviour = cmdline->GetName (1);
  if (plugin == 0 || behaviour == 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
	"This tool expects two parameters. The first parameter is the name\n"
	"of a behaviour layer plugin (like 'cel.behaviourlayer.python').\n"
	"The second parameter is the name of a behaviour for that layer.");
    return false;
  }

  csReport (object_reg, CS_REPORTER_SEVERITY_NOTIFY,
    	"crystalspace.application.bootstrap",
  	"Using behaviour layer plugin '%s' to fire up behaviour '%s'.",
  	plugin, behaviour);
  fflush (stdout);

  csRef<iPluginManager> plugin_mgr = CS_QUERY_REGISTRY (object_reg,
  	iPluginManager);
  bl = CS_LOAD_PLUGIN (plugin_mgr, plugin, iCelBlLayer);
  if (!bl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
	"Couldn't load behaviour layer '%s'!",
	plugin);
    return false;
  }
  object_reg->Register (bl, "iCelBlLayer");
  pl->RegisterBehaviourLayer (bl);

  // Open the main system. This will open all the previously loaded plug-ins.
  if (!csInitializer::OpenApplication (object_reg))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
    	"Error opening system!");
    return false;
  }

  bootstrap_entity = pl->CreateEntity ();
  bootstrap_entity->SetName ("bootstrap");
  iCelBehaviour* behave = bl->CreateBehaviour (bootstrap_entity, behaviour);
  bootstrap_entity->SetBehaviour (behave);

  const char* extra_method = cmdline->GetName (2);
  if (extra_method)
  {
    const char* extra_arg1 = cmdline->GetName (3);
    const char* extra_arg2 = cmdline->GetName (4);
    const char* extra_arg3 = cmdline->GetName (5);
    // @@@ TODO:  Support iCelParameterBlock system!
    if (extra_arg3)
      behave->SendMessage (extra_method, 0, extra_arg1, extra_arg2, extra_arg3);
    else if (extra_arg2)
      behave->SendMessage (extra_method, 0, extra_arg1, extra_arg2);
    else if (extra_arg1)
      behave->SendMessage (extra_method, 0, extra_arg1);
    else
      behave->SendMessage (extra_method, 0);
  }

  return true;
}

void Bootstrap::Start ()
{
  csDefaultRunLoop (object_reg);
}

/*---------------------------------------------------------------------*
 * Main function
 *---------------------------------------------------------------------*/
int main (int argc, char* argv[])
{
  bootstrap = new Bootstrap ();

  if (bootstrap->Initialize (argc, argv))
    bootstrap->Start ();

  iObjectRegistry* object_reg = bootstrap->object_reg;
  delete bootstrap;

  csInitializer::DestroyApplication (object_reg);
  return 0;
}

