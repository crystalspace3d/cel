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
#include "csutil/common_handlers.h"
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

bool Bootstrap::HandleEvent (iEvent& ev)
{
  csKeyEventType eventtype = csKeyEventHelper::GetEventType(&ev);
  if (eventtype == csKeyEventTypeDown)
  {
    utf32_char code = csKeyEventHelper::GetCookedCode (&ev);
    if (code == CSKEY_ESC)
    {
      csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
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

bool Bootstrap::Initialize (int argc, const char* const argv[])
{
  object_reg = csInitializer::CreateEnvironment (argc, argv);
  if (!object_reg) return false;

  csString configname = "/celconfig/bootstrap.cfg";

  if (!celInitializer::SetupConfigManager (object_reg, configname))
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

  g3d = csQueryRegistry<iGraphics3D> (object_reg);
  if (!g3d)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
    	"No iGraphics3D plugin!");
    return false;
  }

  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.bootstrap",
    	"CEL physical layer missing!");
    return false;
  }

  csRef<iCommandLineParser> cmdline = 
  	csQueryRegistry<iCommandLineParser> (object_reg);
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

  csRef<iPluginManager> plugin_mgr = 
  	csQueryRegistry<iPluginManager> (object_reg);
  bl = csLoadPlugin<iCelBlLayer> (plugin_mgr, plugin);
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
    csRef<celVariableParameterBlock> params;
    csStringID id_par1 = pl->FetchStringID ("parameter1");
    csStringID id_par2 = pl->FetchStringID ("parameter2");
    csStringID id_par3 = pl->FetchStringID ("parameter3");

    if (extra_arg3)
    {
      params.AttachNew (new celVariableParameterBlock (3));
      params->AddParameter (id_par1).Set (extra_arg1);
      params->AddParameter (id_par2).Set (extra_arg2);
      params->AddParameter (id_par3).Set (extra_arg3);
    }
    else if (extra_arg2)
    {
      params.AttachNew (new celVariableParameterBlock (2));
      params->AddParameter (id_par1).Set (extra_arg1);
      params->AddParameter (id_par2).Set (extra_arg2);
    }
    else if (extra_arg1)
    {
      params.AttachNew (new celVariableParameterBlock (1));
      params->AddParameter (id_par1).Set (extra_arg1);
    }
    behave->SendMessage (extra_method, 0, ret, params);
  }

  printer.AttachNew (new FramePrinter (object_reg));

  return true;
}


void Bootstrap::Start ()
{
  csDefaultRunLoop (object_reg);
}

void Bootstrap::Stop ()
{
  printer.Invalidate ();
}

/*---------------------------------------------------------------------*
 * Main function
 *---------------------------------------------------------------------*/
int main (int argc, char* argv[])
{
  bootstrap = new Bootstrap ();

  if (bootstrap->Initialize (argc, argv))
    bootstrap->Start ();

  bootstrap->Stop ();

  iObjectRegistry* object_reg = bootstrap->object_reg;
  delete bootstrap;
  bootstrap = 0;

  csInitializer::DestroyApplication (object_reg);
  return 0;
}

