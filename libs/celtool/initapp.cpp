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

// Recurse into subdirectories when scanning for plugins? Disabled presently
// because some developers run CEL programs from within the source tree, and it
// can be quite costly to recurse over the entire tree.
#define CEL_PLUGIN_SCAN_RECURSE false

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
      cel_paths.AddUniqueExpanded("/usr/lib/cel/", CEL_PLUGIN_SCAN_RECURSE);
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
  return superclass::RequestPluginsV(r, args);
}

bool celInitializer::RequestPlugins(iObjectRegistry* r,
  csArray<csPluginRequest> const& a)
{
  SetupCelPluginDirs(r);
  return superclass::RequestPlugins(r, a);
}
