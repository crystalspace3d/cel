/*
    Copyright (C) 2004 by Eric Sunshine

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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "celtool/initapp.h"
#include <csutil/syspath.h>
#include "iutil/cfgmgr.h"

#include "ivaria/reporter.h"
#include "csutil/verbosity.h"
#include "csutil/cfgacc.h"
#include "csutil/cfgfile.h"
#include "csutil/cmdline.h"
#include "csutil/csshlib.h"

// Recurse into subdirectories when scanning for plugins? Disabled presently
// because some developers run CEL programs from within the source tree, and it
// can be quite costly to recurse over the entire tree.
#define CEL_PLUGIN_SCAN_RECURSE false

// These defines should be set by the configure script
#ifndef CEL_CONFIGDIR
#ifdef CS_COMPILER_GCC
#warning CEL_CONFIGDIR not set
#endif
#define CEL_CONFIGDIR "/usr/local/" CS_PACKAGE_NAME
#endif
#ifndef CEL_PLUGINDIR
#ifdef CEL_COMPILER_GCC
#warning CEL_PLUGINDIR not set
#endif
#define CEL_PLUGINDIR "/usr/local/" CS_PACKAGE_NAME "/lib"
#endif


static bool cel_config_done = false;

void celInitializer::setup_plugin_dirs(iObjectRegistry* r, char const* dir0)
{
  static bool done = false;
  if (!done)
  {
    csPathsList cel_paths;
    csString cel_env (getenv ("CEL"));
    if (!cel_env.IsEmpty())
    {
      static const char* const celDirs[] = {
	"", 
	"lib/", 
	"cel/",
	0};
      csPathsList cel_env_paths (cel_env);
      // Assign "cel" type to all paths
      for (size_t i = 0; i < cel_env_paths.GetSize(); i++)
        cel_env_paths[i].type = "cel";
      // Add CEL paths to try
      cel_paths.AddUniqueExpanded(cel_env_paths * csPathsList (celDirs), 
        CEL_PLUGIN_SCAN_RECURSE);
    }
    else
    {
      cel_paths.AddUniqueExpanded(CEL_PLUGINDIR, CEL_PLUGIN_SCAN_RECURSE);
    }

    if (dir0 != 0)
      cel_paths.AddUniqueExpanded(dir0, CEL_PLUGIN_SCAN_RECURSE, "cel");

    scfInitialize(&cel_paths);
    done = true;
  }
}

bool celInitializer::RequestPlugins(iObjectRegistry* r, ...)
{
  va_list args;
  va_start(args, r);
  bool const ok = celInitializer::RequestPluginsV(r, args);
  va_end(args);
  return ok;
}

bool celInitializer::RequestPluginsV(iObjectRegistry* r, va_list args)
{
  SetupCelPluginDirs(r);
  bool const ok = superclass::RequestPluginsV(r, args);
  LoadCelVFS(r);
  return ok;
}

bool celInitializer::RequestPlugins(iObjectRegistry* r,
  csArray<csPluginRequest> const& a)
{
  SetupCelPluginDirs(r);
  bool const ok = superclass::RequestPlugins(r, a);
  // find and add cel vfs file if its not done already
  LoadCelVFS(r);
  return ok;
}

bool celInitializer::LoadMountsFromFile(iObjectRegistry* r, char const* configPath)
{
  csRef<iConfigManager> cfg_mgr = csQueryRegistry<iConfigManager> (r);
  csRef<iVFS> vfs = csQueryRegistry<iVFS> (r);
  csRef<iConfigFile> vfs_file = cfg_mgr->AddDomain(configPath, NULL, 0);
  return vfs->LoadMountsFromFile(vfs_file);
}

bool celInitializer::LoadCelVFS(iObjectRegistry* r)
{
  bool ok = false;
  if (cel_config_done) return true;
  // find and add cel vfs file
  csPathsList cel_env_path;
  csString cel_env (getenv ("CEL"));
  // find dir
  if (cel_env.IsEmpty())
    cel_env_path = csPathsList(CEL_CONFIGDIR);
  else
  {
    cel_env_path = csPathsList(cel_env);
  }
  // check vfs.cfg file is actually there.
  csPathsList vfs_file_path = 
	    csPathsUtilities::LocateFile(cel_env_path,"vfs.cfg");
  if (vfs_file_path.Length())
  {
    csString cel_vfs_file = 
      vfs_file_path[0].path+csString(CS_PATH_SEPARATOR)+csString("vfs.cfg");
    ok = LoadMountsFromFile(r,cel_vfs_file);
    cel_config_done = true;
  }
  else
  {
    csRef<iCommandLineParser> cmdline = csQueryRegistry<iCommandLineParser> (r);
    cel_env_path = csPathsList(cmdline->GetAppDir ());
    vfs_file_path = csPathsUtilities::LocateFile(cel_env_path,"vfs.cfg");
    if (vfs_file_path.Length())
      ok = true;
    else
      csReport(r,CS_REPORTER_SEVERITY_WARNING,"cel.initializer",
	     "Couldn't find vfs.cfg!");
  }
  return ok;
}

iVFS* celInitializer::SetupVFS(iObjectRegistry* r, const char* pluginID)
{
  csRef<iVFS> vfs = superclass::SetupVFS(r, pluginID);
  LoadCelVFS(r);
  return vfs;
}

bool celInitializer::SetupConfigManager (
		  iObjectRegistry* r, char const* configName, char const* AppID)
{
  SetupVFS(r);
  return superclass::SetupConfigManager(r,configName,AppID);  
}
