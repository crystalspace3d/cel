/*
    Copyright (C) 2006 by Jorrit Tyberghein

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
#include "celstart.h"
#include "csutil/sysfunc.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "csutil/event.h"
#include "csutil/cmdhelp.h"
#include "csutil/cfgacc.h"
#include "csutil/physfile.h"
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

// The global pointer to celstart
CelStart* celstart;

CelStart::CelStart ()
{
  do_clearscreen = false;
}

CelStart::~CelStart ()
{
  if (pl)
  {
    pl->CleanCache ();
  }
}

void CelStart::SetupFrame ()
{
  if (do_clearscreen)
    g3d->BeginDraw (CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN);
}

void CelStart::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (0);
}

bool CelStart::HandleEvent (iEvent& ev)
{
  if (ev.Name == csevProcess (object_reg))
  {
    celstart->SetupFrame ();
    return true;
  }
  else if (ev.Name == csevFinalProcess (object_reg))
  {
    celstart->FinishFrame ();
    return true;
  }

  return false;
}

bool CelStart::CelStartEventHandler (iEvent& ev)
{
  if (celstart)
    return celstart->HandleEvent (ev);
  else
    return false;
}

bool CelStart::FindPath (iVFS* vfs, csString& path, csString& configname)
{
  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
  const char* arg = cmdline->GetName (0);

  if (!arg)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	  "crystalspace.application.celstart",
    	  "Please specify the name of the config file or else the path to an\n"
	  "archive or directory containing celstart.cfg");
    return false;
  }

  csPhysicalFile physfile (arg, "rb");
  if (physfile.GetStatus () == 0)
  {
    char buf[4];
    physfile.Read (buf, 4);
    bool is_zip = buf[0] == 'P' && buf[1] == 'K' && buf[2] == 3 && buf[3] == 4;
    if (is_zip)
    {
      vfs->Mount ("/tmp/celstart", arg);
      configname = "/tmp/celstart/celstart.cfg";
      path = "/tmp/celstart";
      return true;
    }
    else
    {
      path = "/this";
      configname = "/this/";
      configname += arg;
      return true;
    }
  }

  bool exists = vfs->Exists (arg);
  if (exists)
  {
    configname = arg;
    path = "/this";
  }
  else if (vfs->ChDirAuto (arg, 0, "/tmp/celstart", "celstart.cfg"))
  {
    configname = "/tmp/celstart/celstart.cfg";
    path = "/tmp/celstart";
  }
  else
  {
    // ChDir failed. Try to see if the path is a config file directly.
    configname = arg;
    path = "/this";
  }
  return true;
}

bool CelStart::Initialize (int argc, const char* const argv[])
{
  object_reg = csInitializer::CreateEnvironment (argc, argv);
  if (!object_reg) return false;

  csRef<iVFS> vfs = csInitializer::SetupVFS (object_reg);
  if (!vfs)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	  "crystalspace.application.celstart",
    	  "Couldn't setup vfs! Is Crystal Space setup correctly?");
    return false;
  }

  csString path;
  csString configname;
  if (!FindPath (vfs, path, configname))
    return false;

  if (!csInitializer::SetupConfigManager (object_reg, configname))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celstart",
    	"Can't initialize config from '%s'!", (const char*)configname);
    return false;
  }

  if (!celInitializer::RequestPlugins (object_reg,
  	CS_REQUEST_VFS, CS_REQUEST_END))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celstart",
    	"Can't initialize plugins!");
    return false;
  }

  if (!csInitializer::SetupEventHandler (object_reg, CelStartEventHandler))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celstart",
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
    	"crystalspace.application.celstart",
    	"No iGraphics3D plugin!");
    return false;
  }

  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celstart",
    	"CEL physical layer missing!");
    return false;
  }

  // Open the main system. This will open all the previously loaded plug-ins.
  if (!csInitializer::OpenApplication (object_reg))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celstart",
    	"Error opening system!");
    return false;
  }

  csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (object_reg);
  csRef<iConfigManager> cfg = csQueryRegistry<iConfigManager> (object_reg);
  csRef<iConfigIterator> it = cfg->Enumerate ("CelStart.BehaviourLayer.");
  while (it && it->Next ())
  {
    const char* bhname = it->GetKey (true);
    const char* id = it->GetStr ();
    csRef<iCelBlLayer> bl = csLoadPlugin<iCelBlLayer> (plugmgr, id);
    if (!bl)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celstart",
    	"Can't load behaviour layer '%s' with id '%s'!", bhname, id);
      return false;
    }
    object_reg->Register (bl, bhname);
    pl->RegisterBehaviourLayer (bl);
  }

  it = cfg->Enumerate ("CelStart.Entity.");
  while (it && it->Next ())
  {
    const char* entityname = it->GetKey (true);
    csRef<iCelEntity> ent = pl->CreateEntity ();
    ent->SetName (entityname);
    csString key = "CelStart.EntityBehaviour.";
    key += entityname;
    const char* behaviourname = cfg->GetStr (key, 0);
    if (behaviourname)
    {
      key = "CelStart.EntityBehaviourLayer.";
      key += entityname;
      const char* behaviourlayer = cfg->GetStr (key, 0);
      csRef<iCelBlLayer> bl;
      if (behaviourlayer)
	bl = pl->FindBehaviourLayer (behaviourlayer);
      else
	bl = csQueryRegistry<iCelBlLayer> (object_reg);
      if (!bl)
      {
        csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	    "crystalspace.application.celstart",
    	    "Error finding behaviour layer for entity '%s'!",
	    entityname);
        return false;
      }
      iCelBehaviour* behave = bl->CreateBehaviour (ent, behaviourname);
      if (!behave)
      {
        csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	    "crystalspace.application.celstart",
    	    "Can't create behaviour '%s' for entity '%s'!",
	    behaviourname, entityname);
        return false;
      }
    }
  }

  csRef<iLoader> loader = csQueryRegistry<iLoader> (object_reg);
  it = cfg->Enumerate ("CelStart.MapFile.");
  while (it && it->Next ())
  {
    const char* file = it->GetStr ();
    if (!vfs->ChDirAuto (path, 0, 0, file))
      return false;
    if (!loader->LoadMapFile (file, false))
      return false;
  }

  do_clearscreen = cfg->GetBool ("CelStart.ClearScreen", false);

  return true;
}


void CelStart::Start ()
{
  csDefaultRunLoop (object_reg);
}

/*---------------------------------------------------------------------*
 * Main function
 *---------------------------------------------------------------------*/
int main (int argc, char* argv[])
{
  celstart = new CelStart ();

  if (celstart->Initialize (argc, argv))
    celstart->Start ();

  iObjectRegistry* object_reg = celstart->object_reg;
  delete celstart;
  celstart = 0;

  csInitializer::DestroyApplication (object_reg);
  return 0;
}

