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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA.
*/

#include "cssysdef.h"
#include "bootstrap.h"
#include "csutil/sysfunc.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
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
#include "imap/loader.h"
#include "isndsys/ss_renderer.h"
#include "isndsys/ss_loader.h"

#include "celtool/initapp.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "celtool/stdparams.h"

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

void Bootstrap::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (0);
}

bool Bootstrap::HandleEvent (iEvent& ev)
{
  if (ev.Name == csevFinalProcess (object_reg))
  {
    bootstrap->FinishFrame ();
    return true;
  }

  if (bootstrap->GetBareMode ()) return false;

  csKeyEventType eventtype = csKeyEventHelper::GetEventType(&ev);
  if (eventtype == csKeyEventTypeDown)
  {
    utf32_char code = csKeyEventHelper::GetCookedCode (&ev);
    if (code == CSKEY_ESC)
    {
      csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
      if (q)
        q->GetEventOutlet()->Broadcast (csevQuit (object_reg));
      return true;
    }
  }
  return false;
}

bool Bootstrap::BootstrapEventHandler (iEvent& ev)
{
  if (bootstrap)
    return bootstrap->HandleEvent (ev);
  else
    return false;
}

bool Bootstrap::BareInitialize ()
{
  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
  // Initialize a very bare bones version of bootstrap which
  // works totally different from normal bootstrap.
  const char* path = cmdline->GetName (0);
  const char* file = cmdline->GetName (1);
  if (path == 0 || file == 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	  "crystalspace.application.bootstrap",
    	  "This tool expects at least two parameters in -bare mode. The first\n"
          "parameter is a VFS path, the second the name of the XML file to\n"
	  "load and the optional third parameter is the name of the config\n"
	  "file to use.");
    return false;
  }
  csString configname = cmdline->GetName (2);
  if (configname.IsEmpty ())
    configname = "/celconfig/bootstrap.cfg";

  if (!csInitializer::SetupConfigManager (object_reg, configname))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
    	"Can't initialize config from '%s'!", (const char*)configname);
    return false;
  }

  if (!celInitializer::RequestPlugins (object_reg,
  	CS_REQUEST_VFS, CS_REQUEST_END))
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

  // Open the main system. This will open all the previously loaded plug-ins.
  if (!csInitializer::OpenApplication (object_reg))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
    	"Error opening system!");
    return false;
  }

  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  csRef<iLoader> loader = csQueryRegistry<iLoader> (object_reg);
  if (!vfs->ChDirAuto (path, 0, 0, file))
    return false;
  if (!loader->LoadMapFile (file, false))
    return false;

  return true;
}

bool Bootstrap::FullInitialize ()
{
  csString configname = "/celconfig/bootstrap.cfg";

  if (!csInitializer::SetupConfigManager (object_reg, configname))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
    	"Can't initialize config from '%s'!", (const char*)configname);
    return false;
  }

  if (!celInitializer::RequestPlugins (object_reg,
  	CS_REQUEST_VFS, CS_REQUEST_END))
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

  const char* extra_method = cmdline->GetName (2);
  if (extra_method)
  {
    const char* extra_arg1 = cmdline->GetName (3);
    const char* extra_arg2 = cmdline->GetName (4);
    const char* extra_arg3 = cmdline->GetName (5);
    celData ret;
    celGenericParameterBlock* params = NULL;
    csStringID id_par1 = pl->FetchStringID ("cel.parameter.parameter1");
    csStringID id_par2 = pl->FetchStringID ("cel.parameter.parameter2");
    csStringID id_par3 = pl->FetchStringID ("cel.parameter.parameter3");

    if (extra_arg3)
    {
      params = new celGenericParameterBlock (3);
      params->SetParameterDef (0, id_par1, "parameter1");
      params->SetParameterDef (1, id_par2, "parameter2");
      params->SetParameterDef (2, id_par3, "parameter3");
      params->GetParameter(0).Set(extra_arg1);
      params->GetParameter(1).Set(extra_arg2);
      params->GetParameter(2).Set(extra_arg3);
    }
    else if (extra_arg2)
    {
      params = new celGenericParameterBlock (2);
      params->SetParameterDef (0, id_par1, "parameter1");
      params->SetParameterDef (1, id_par2, "parameter2");
      params->GetParameter(0).Set(extra_arg1);
      params->GetParameter(1).Set(extra_arg2);
    }
    else if (extra_arg1)
    {
      params = new celGenericParameterBlock (1);
      params->SetParameterDef (0, id_par1, "parameter1");
      params->GetParameter(0).Set(extra_arg1);
    }
    behave->SendMessage (extra_method, 0, ret, params);
  }

  return true;
}

bool Bootstrap::Initialize (int argc, const char* const argv[])
{
  object_reg = csInitializer::CreateEnvironment (argc, argv);
  if (!object_reg) return false;

  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
  const char* bare = cmdline->GetOption ("bare");
  bare_mode = bare != 0;
  if (bare_mode)
    return BareInitialize ();
  else
    return FullInitialize ();
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
  bootstrap = 0;

  csInitializer::DestroyApplication (object_reg);
  return 0;
}

