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
#include "csutil/cmdhelp.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/csinput.h"
#include "iutil/plugin.h"
#include "iutil/vfs.h"
#include "iutil/object.h"
#include "iengine/engine.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "igraphic/imageio.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "ivaria/collider.h"
#include "imap/parser.h"

#include "physicallayer/pl.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/bl.h"

CS_IMPLEMENT_APPLICATION

//-----------------------------------------------------------------------------

// The global pointer to celpython
CelPython* celpython;

CelPython::CelPython ()
{
}

CelPython::~CelPython ()
{
  if (pl)
    pl->CleanCache ();
}

void CelPython::SetupFrame ()
{
  // We let the entity system do this so there is nothing here.
}

void CelPython::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (0);
}

bool CelPython::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdProcess)
  {
    celpython->SetupFrame ();
    return true;
  }
  else if (ev.Type == csevBroadcast && ev.Command.Code == cscmdFinalProcess)
  {
    celpython->FinishFrame ();
    return true;
  }
  else if (ev.Type == csevKeyDown)
  {
    if (ev.Key.Code == CSKEY_ESC)
    {
      csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
      if (q)
        q->GetEventOutlet()->Broadcast (cscmdQuit);
      return true;
    }
  }
  return false;
}

bool CelPython::CelPythonEventHandler (iEvent& ev)
{
  return celpython->HandleEvent (ev);
}

bool CelPython::Initialize (int argc, const char* const argv[])
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
	CS_REQUEST_PLUGIN ("cel.behaviourlayer.python:iCelBlLayer.Python",
		iCelBlLayer),
	CS_REQUEST_PLUGIN ("cel.persistance.classic", iCelPersistance),
	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.rapid",
		iCollideSystem),
	CS_REQUEST_END))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celpython",
	"Can't initialize plugins!");
    return false;
  }

  if (!csInitializer::SetupEventHandler (object_reg, CelPythonEventHandler))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celpython",
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
    	"crystalspace.application.celpython",
	"No iEngine plugin!");
    return false;
  }

  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  if (!kbd)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celpython",
    	"No iKeyboardDriver plugin!");
    return false;
  }

  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  if (!g3d)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celpython",
    	"No iGraphics3D plugin!");
    return false;
  }

  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celpython",
    	"CEL physical layer missing!");
    return false;
  }
  
  blpython = CS_QUERY_REGISTRY_TAG_INTERFACE (object_reg,
  	"iCelBlLayer.Python", iCelBlLayer);
  if (!blpython)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celpython",
    	"CEL python behaviour layer missing.");
    return false;
  }
  pl->RegisterBehaviourLayer (blpython);

  // Open the main system. This will open all the previously loaded plug-ins.
  if (!csInitializer::OpenApplication (object_reg))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celpython",
    	"Error opening system!");
    return false;
  }

#if 0
  // XXX: This should be in a config file...
  if (!LoadPcFactory ("cel.pcfactory.test"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.movable"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.solid"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.movableconst_cd"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.gravity"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.region"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.camera"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.tooltip"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.timer"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.inventory"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.characteristics"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.mesh"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.meshselect"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.pckeyinput"))
    return false;
#endif

  return true;
}

void CelPython::Start ()
{
  csDefaultRunLoop (object_reg);
}

/*---------------------------------------------------------------------*
 * Main function
 *---------------------------------------------------------------------*/
int main (int argc, char* argv[])
{
  celpython = new CelPython ();

  if (celpython->Initialize (argc, argv))
    celpython->Start ();

  iObjectRegistry* object_reg = celpython->object_reg;
  delete celpython;

  csInitializer::DestroyApplication (object_reg);
  return 0;
}

