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
#include "csutil/cfgacc.h"
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
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/texture.h"
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
  if (files.Length () > 0)
  {
    g3d->BeginDraw (CSDRAW_2DGRAPHICS | CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN);
    size_t i;
    int my = mouse->GetLastY ();
    for (i = top_file ; i < files.Length () ; i++)
    {
      iGraphics2D* g2d = g3d->GetDriver2D ();
      int y = 10 + (i-top_file)*70;
      if (y+70 > g2d->GetHeight ()) break;
      bool sel = my >= y && my <= y+63;
      g2d->DrawBox (7, y+2, g2d->GetWidth ()-14, 70-4, box_color3);
      g2d->DrawBox (8, y+3, g2d->GetWidth ()-16, 70-6, box_color2);
      g2d->DrawBox (9, y+4, g2d->GetWidth ()-18, 70-8, sel ? sel_box_color1 : box_color1);
      if (icons[i])
	icons[i]->DrawScaled (g3d, 10, y+5, 80, 60);
      csString w = names[i];
      w += " (";
      w += files[i];
      w += ")";
      g2d->Write (font, 100, y+10, sel ? sel_font_fg : font_fg,
	  sel ? sel_font_bg : font_bg, w);
      if (descriptions[i])
        g2d->Write (font, 100, y+20, sel ? sel_font_fg : font_fg,
	    sel ? sel_font_bg : font_bg, descriptions[i]);
    }
  }
  else
  {
    if (do_clearscreen)
      g3d->BeginDraw (CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN);
  }
}

void CelStart::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (0);
  // @@@
  //csSleep (5);
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

  if (files.Length () > 0)
  {
    if (CS_IS_KEYBOARD_EVENT (object_reg, ev))
    {
      csKeyEventType eventtype = csKeyEventHelper::GetEventType(&ev);
      if (eventtype == csKeyEventTypeDown)
      {
        utf32_char code = csKeyEventHelper::GetCookedCode (&ev);
        if (code == CSKEY_ESC)
        {
	  startme = "";
          csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
          if (q)
            q->GetEventOutlet()->Broadcast (csevQuit (object_reg));
          return true;
        }
	else if (code == CSKEY_UP)
	{
	  if (top_file > 0) top_file--;
	}
	else if (code == CSKEY_DOWN)
	{
	  if (top_file < (int)files.Length ()-1) top_file++;
	}
      }
    }
    else if (CS_IS_MOUSE_EVENT (object_reg, ev))
    {
      if (csMouseEventHelper::GetEventType (&ev) == csMouseEventTypeUp)
      {
        int my = csMouseEventHelper::GetY (&ev);
        int i = (my-10)/70+top_file;
        if (i >= (int)0 && i < (int)files.Length ())
        {
	  printf ("Start %d\n", i); fflush (stdout);
	  startme = files[i];
          csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
          if (q)
            q->GetEventOutlet()->Broadcast (csevQuit (object_reg));
	}
      }
    }
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

void CelStart::FindCelStartArchives ()
{
  top_file = 0;
  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  vfs->Mount ("/tmp/celstart_app", "$^");
  csRef<iStringArray> filelist = vfs->FindFiles ("/tmp/celstart_app/*");
  size_t i;
  for (i = 0 ; i < filelist->Length () ; i++)
  {
    const char* file = filelist->Get (i);
    csRef<iDataBuffer> realpath_db = vfs->GetRealPath (file);
    char* realpath = (char*)(realpath_db->GetData ());
    char* testpath = new char [strlen (realpath)+3];
    strcpy (testpath, realpath);
    if (testpath[strlen (testpath)-1] == '/')
    {
      // We have a directory.
      size_t l = strlen (testpath);
      testpath[l-1] = '$';
      testpath[l] = '/';
      testpath[l+1] = 0;
    }
    else if (strstr (testpath, ".zip") == 0)
    {
      delete[] testpath;
      continue;
    }
    else
    {
      // We have a zip.
    }

    vfs->Mount ("/tmp/celstart_test", testpath);
    if (vfs->Exists ("/tmp/celstart_test/celstart.cfg"))
    {
      files.Push (testpath);
      csConfigAccess acc (object_reg, "/tmp/celstart_test/celstart.cfg");
      const char* name = acc->GetStr ("CelStart.Name", 0);
      
      if (name) names.Push (name);
      else
      {
	const char* s = strrchr (testpath, '/');
	if (!s)
	{
	  s = strrchr (testpath, '\\');
	  if (!s) s = testpath;
	  else s++;
	}
	else s++;
	if (*s) s = testpath;
	names.Push (s);
      }
      const char* description = acc->GetStr ("CelStart.Description", 0);
      if (description) descriptions.Push (description);
      else descriptions.Push ("");
      const char* icon = acc->GetStr ("CelStart.Icon", 0);
      if (!icon)
        icons.Push (0);
      else
      {
	csRef<iLoader> loader = csQueryRegistry<iLoader> (object_reg);
	csString iconpath = "/tmp/celstart_test/";
	iconpath += icon;
	csRef<iTextureHandle> txt = loader->LoadTexture (iconpath, CS_TEXTURE_2D,
	    g3d->GetTextureManager ());
	csSimplePixmap* pm = new csSimplePixmap (txt);
	icons.Push (pm);
      }
    }
    csRef<iConfigManager> cfg = csQueryRegistry<iConfigManager> (object_reg);
    cfg->FlushRemoved ();
    vfs->Unmount ("/tmp/celstart_test", testpath);

    delete[] testpath;
  }
  vfs->Unmount ("/tmp/celstart_app", 0);
}

bool CelStart::FindPath (iVFS* vfs, csString& realpath,
    csString& path, csString& configname)
{
  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
  const char* arg = cmdline->GetName (0);

  csPhysicalFile physfile (arg, "rb");
  if (physfile.GetStatus () == 0)
  {
    char buf[4];
    physfile.Read (buf, 4);
    bool is_zip = buf[0] == 'P' && buf[1] == 'K' && buf[2] == 3 && buf[3] == 4;
    if (is_zip)
    {
printf ("1\n"); fflush (stdout);
      vfs->Mount ("/tmp/celstart", arg);
      configname = "/tmp/celstart/celstart.cfg";
      path = "/tmp/celstart";
      realpath = arg;
      return true;
    }
    else
    {
printf ("2\n"); fflush (stdout);
      path = "/this";
      configname = "/this/";
      configname += arg;
      realpath = "";
      return true;
    }
  }

  bool exists = vfs->Exists (arg);
  if (exists)
  {
printf ("3\n"); fflush (stdout);
    configname = arg;
    path = "/this";
    realpath = "";
  }
  else if (vfs->ChDirAuto (arg, 0, "/tmp/celstart", "celstart.cfg"))
  {
printf ("4\n"); fflush (stdout);
    configname = "/tmp/celstart/celstart.cfg";
    path = "/tmp/celstart";
    realpath = arg;
  }
  else
  {
printf ("5\n"); fflush (stdout);
    // ChDir failed. Try to see if the path is a config file directly.
    configname = arg;
    path = "/this";
    realpath = "";
  }
  return true;
}

bool CelStart::StartDemo (int argc, const char* const argv[],
    const char* realpath, const char* path, const char* configname)
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
  if (realpath && *realpath)
    vfs->Mount (path, realpath);

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

bool CelStart::StartDemoSelector (int argc, const char* const argv[])
{
  CelStart::argc = argc;
  CelStart::argv = (const char**)argv;

  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
  cmdline->AddOption ("silent", "");
  cmdline->AddOption ("mode", "800x600");

  if (!celInitializer::RequestPlugins (object_reg,
        CS_REQUEST_VFS,
        CS_REQUEST_OPENGL3D,
        CS_REQUEST_ENGINE,
        CS_REQUEST_FONTSERVER,
        CS_REQUEST_IMAGELOADER,
        CS_REQUEST_LEVELLOADER,
        CS_REQUEST_REPORTER,
        CS_REQUEST_REPORTERLISTENER,
        CS_REQUEST_END))
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

  mouse = csQueryRegistry<iMouseDriver> (object_reg);

  // Open the main system. This will open all the previously loaded plug-ins.
  if (!csInitializer::OpenApplication (object_reg))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celstart",
    	"Error opening system!");
    return false;
  }

  font = g3d->GetDriver2D ()->GetFontServer ()->LoadFont (CSFONT_COURIER);
  font_fg = g3d->GetDriver2D ()->FindRGB (0, 0, 0);
  font_bg = g3d->GetDriver2D ()->FindRGB (200, 200, 200);
  sel_font_fg = g3d->GetDriver2D ()->FindRGB (0, 0, 0);
  sel_font_bg = g3d->GetDriver2D ()->FindRGB (40, 210, 170);
  box_color3 = g3d->GetDriver2D ()->FindRGB (60, 60, 60);
  box_color2 = g3d->GetDriver2D ()->FindRGB (120, 120, 120);
  box_color1 = g3d->GetDriver2D ()->FindRGB (200, 200, 200);
  sel_box_color1 = sel_font_bg;

  FindCelStartArchives ();
  if (files.Length () == 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	  "crystalspace.application.celstart",
    	  "Couldn't find any celstart compatible game files!\n"
	  "You can download such demos from http://www.crystalspace3d.org");
    return false;
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

  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
  const char* arg = cmdline->GetName (0);
  if (!arg)
  {
    return StartDemoSelector (argc, argv);
  }

  csString realpath;
  csString path;
  csString configname;
  if (!FindPath (vfs, realpath, path, configname))
    return false;

  cmdline = 0;
  vfs = 0;
  csInitializer::DestroyApplication (object_reg);
  printf ("Start realpath='%s' path='%s' configname='%s'\n", (const char*)realpath, (const char*)path, (const char*)configname); fflush (stdout);
  return StartDemo (argc, argv, realpath, path, configname);
}


void CelStart::Start ()
{
  csDefaultRunLoop (object_reg);
  if (!startme.IsEmpty ())
  {
    printf ("Start '%s'!\n", (const char*)startme);
    fflush (stdout);
    pl = 0;
    g3d = 0;
    font = 0;
    mouse = 0;
    files.DeleteAll ();
    names.DeleteAll ();
    descriptions.DeleteAll ();
    icons.DeleteAll ();
    csInitializer::DestroyApplication (object_reg);
    if (!StartDemo (argc, argv, startme, "/tmp/celstart", "/tmp/celstart/celstart.cfg"))
      return;
    csDefaultRunLoop (object_reg);
  }
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

