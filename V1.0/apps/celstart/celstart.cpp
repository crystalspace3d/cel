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
#include "csutil/cfgacc.h"
#include "csutil/cmdhelp.h"
#include "csutil/event.h"
#include "csutil/physfile.h"
#include "csutil/sysfunc.h"
#include "csutil/util.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
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
#include "ivideo/natwin.h"
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

CelStart::CelStart () : do_real_demo (false)
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

int CelStart::GetHitBox (int /*mx*/, int my)
{
  return (my-boxTopY)/boxH+top_file;
}

void CelStart::DrawSelectorBox (iGraphics2D* g2d, int x, int y, int w, int h,
                                bool highlight)
{
  g2d->DrawBox (x,   y,   w,   h,   box_color3);
  g2d->DrawBox (x+1, y+1, w-2, h-2, box_color2);
  g2d->DrawBox (x+2, y+2, w-4, h-4, 
    highlight ? sel_box_color1 : box_color1);
}

void CelStart::SetupFrame ()
{
  iGraphics2D* g2d = g3d->GetDriver2D ();
  if (files.Length () > 0)
  {
    const int fontH = font->GetTextHeight();
    g3d->BeginDraw (CSDRAW_2DGRAPHICS | CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN);
    size_t i;
    for (i = top_file ; i < files.Length () ; i++)
    {
      int y = boxTopY + (i-top_file)*boxH;
      if (y+boxH > g2d->GetHeight ()) break;
      bool sel = int (i) == GetHitBox (mouse->GetLastX(), mouse->GetLastY());
      DrawSelectorBox (g2d, boxMarginX, y+boxMarginY, 
        g2d->GetWidth()-2*boxMarginX, boxH-2*boxMarginY, sel); 
      const int iconLeft = boxMarginX+boxInnerBorder;
      if (icons[i])
	icons[i]->DrawScaled (g3d, iconLeft, y+boxMarginY+boxInnerBorder, 
	  iconW, iconH);
      csString w = names[i];
      w += " (";
      w += files[i];
      w += ")";
      const int textLeft = iconLeft+iconW+iconMarginRight;
      const int textTop = y+boxMarginY+boxInnerBorder+textY;
      g2d->Write (font, textLeft, textTop, sel ? sel_font_fg : font_fg,
	  sel ? sel_font_bg : font_bg, w);
      if (descriptions[i])
        g2d->Write (font, textLeft, textTop+fontH, sel ? sel_font_fg : font_fg,
	    sel ? sel_font_bg : font_bg, descriptions[i]);
      if (warnings[i])
        g2d->Write (font, textLeft, textTop+fontH*2,
	    font_error_fg,
	    sel ? sel_font_bg : font_bg, warnings[i]);
    }
  }
  else if (!do_real_demo)
  {
    const int fontH = font->GetTextHeight();
    g3d->BeginDraw (CSDRAW_2DGRAPHICS | CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN);
    int x = boxMarginX;
    int y = boxTopY;
    g2d->Write (font, x, y, message_color, -1, 
      "Couldn't find any celstart compatible game packages!");
    y += fontH;
    g2d->Write (font, x, y, message_color, -1, 
      "You can download such packages from http://www.crystalspace3d.org/downloads/celstart");
    y += fontH;
    csString msg;
    msg.Format ("Do not extract the package files, instead place them in the %s directory,",
      realAppResDir.GetData());
    g2d->Write (font, x, y, message_color, -1, msg);
#define LSQUOT  "\342\200\230"
#define RSQUOT  "\342\200\231"
    y += fontH;
    g2d->Write (font, x, y, message_color, -1, 
      "alternatively you can also start " LSQUOT "celstart" RSQUOT " from a directory with such package files");
    y += fontH;
    g2d->Write (font, x, y, message_color, -1, 
      "or pass the filename of a package as a command line argument.");
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

  if (files.Length () > 0 || !do_real_demo)
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
      }
    }

    if (CS_IS_KEYBOARD_EVENT (object_reg, ev))
    {
      csKeyEventType eventtype = csKeyEventHelper::GetEventType(&ev);
      if (eventtype == csKeyEventTypeDown)
      {
        utf32_char code = csKeyEventHelper::GetCookedCode (&ev);
	if (code == CSKEY_UP)
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
        int i = GetHitBox (csMouseEventHelper::GetX (&ev), 
	  csMouseEventHelper::GetY (&ev));
        if (i >= (int)0 && i < (int)files.Length ())
        {
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

static bool IsCelPackage (const char* file)
{
  return (strstr (file, ".zip") != 0) || (strstr (file, ".celzip") != 0);
}

void CelStart::FindCelStartArchives ()
{
  top_file = 0;
  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  vfs->Mount ("/tmp/celstart_app", "$.$/, $^");
  csRef<iStringArray> realMounts = vfs->GetRealMountPaths ("/tmp/celstart_app");
  char* realMountExpanded = csExpandName (realMounts->Get (1));
  realAppResDir = realMountExpanded;
  delete[] realMountExpanded;
  csRef<iStringArray> filelist = vfs->FindFiles ("/tmp/celstart_app/*");
  size_t i;
  // To work around VFS bug
  csSet<csString> testedFiles;
  for (i = 0 ; i < filelist->Length () ; i++)
  {
    const char* file = filelist->Get (i);
    if (testedFiles.Contains (file)) continue;
    testedFiles.AddNoTest (file); 
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
    else if (!IsCelPackage (testpath))
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
      int minimum_version = acc->GetInt ("CelStart.MinimumVersion",
	  CELSTART_VERSION);
      int maximum_version = acc->GetInt ("CelStart.MaximumVersion",
	  CELSTART_VERSION);
      if (minimum_version > CELSTART_VERSION)
      {
	csString warn;
	warn.Format ("WARNING: Requires at least version %d of CelStart (current is %d)!",
	    minimum_version, CELSTART_VERSION);
	warnings.Push (warn);
      }
      else if (maximum_version < CELSTART_VERSION)
      {
	csString warn;
	warn.Format ("WARNING: Prefers at most version %d of CelStart (current is %d)!",
	    maximum_version, CELSTART_VERSION);
	warnings.Push (warn);
      }
      else if (minimum_version < CELSTART_MINIMUMVERSION)
      {
	csString warn;
	warn.Format ("WARNING: CelStart (version %d) may not be compatible with this demo!",
	    CELSTART_VERSION);
	warnings.Push (warn);
      }
      else warnings.Push ("");

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
	csRef<iTextureHandle> txt = loader->LoadTexture (iconpath,
	    CS_TEXTURE_2D, g3d->GetTextureManager ());
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
      vfs->Mount ("/tmp/celstart", arg);
      configname = "/tmp/celstart/celstart.cfg";
      path = "/tmp/celstart";
      realpath = arg;
      return true;
    }
    else
    {
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
    configname = arg;
    configname += "/celstart.cfg";
    path = arg;
    realpath = "";
  }
  else if (vfs->ChDirAuto (arg, 0, "/tmp/celstart", "celstart.cfg"))
  {
    configname = "/tmp/celstart/celstart.cfg";
    path = "/tmp/celstart";
    realpath = arg;
    realpath += "$/";
  }
  else
  {
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
  do_real_demo = true;

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
  {
    vfs->Mount (path, realpath);
    csString old = getenv ("PYTHONPATH");
    if (!old.IsEmpty ())
    {
#if defined(CS_PLATFORM_WIN32)
      old += ";";
#elif defined(CS_PLATFORM_UNIX) || defined(CS_PLATFORM_MACOSX)
      old += ":";
#else
#error "What a strange platform you have!"
#endif
    }
    old += realpath;
    setenv ("PYTHONPATH",old.GetData(),1);
    printf ("PYTHONPATH: %s\n", old.GetData ()); fflush (stdout);
  }

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

  // Load all config dirs specified in config file.
  csRef<iConfigIterator> cfgdir_it = cfg->Enumerate ("CelStart.ConfigDir.");
  vfs->PushDir(path);
  while (cfgdir_it && cfgdir_it->Next ())
  {
    csString config_dir = cfgdir_it->GetStr();
    config_dir.Append("/");   // otherwise vfs wont treat as a dir
    if (vfs->Exists(config_dir))
    {
      size_t i;
      csRef<iStringArray> cfg_files = vfs->FindFiles(config_dir);
      for (i=0;i<cfg_files->GetSize();i++)
      {
        cfg->Load(cfg_files->Get(i),vfs,true);
      }
    }
    else
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
    	"crystalspace.application.celstart",
    	"Specified directory for '%s' cfg files '%s' does not exist in vfs!", 
	cfgdir_it->GetKey(true),cfgdir_it->GetStr());
    }
  }
  vfs->PopDir();

  // Load behaviour layers
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

  // Load initial entities specified in config file
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

  // Load map files
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

  iNativeWindow* nw = g3d->GetDriver2D ()->GetNativeWindow ();
  if (nw) nw->SetTitle (
      cfg->GetStr ("CelStart.WindowTitle","CelStart Application"));

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
        CS_REQUEST_PLUGIN("crystalspace.font.server.multiplexer", iFontServer),
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

  font = g3d->GetDriver2D ()->GetFontServer ()->LoadFont (CSFONT_LARGE);
  font_fg = g3d->GetDriver2D ()->FindRGB (0, 0, 0);
  font_bg = g3d->GetDriver2D ()->FindRGB (200, 200, 200);
  font_error_fg = g3d->GetDriver2D ()->FindRGB (255, 0, 0);
  sel_font_fg = g3d->GetDriver2D ()->FindRGB (0, 0, 0);
  sel_font_bg = g3d->GetDriver2D ()->FindRGB (40, 210, 170);
  box_color3 = g3d->GetDriver2D ()->FindRGB (60, 60, 60);
  box_color2 = g3d->GetDriver2D ()->FindRGB (120, 120, 120);
  box_color1 = g3d->GetDriver2D ()->FindRGB (200, 200, 200);
  sel_box_color1 = sel_font_bg;
  message_color = g3d->GetDriver2D ()->FindRGB (255, 255, 255);

  FindCelStartArchives ();
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
    warnings.DeleteAll ();
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

