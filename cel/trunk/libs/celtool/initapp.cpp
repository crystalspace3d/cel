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

void celInitializer::setup_plugin_dirs(iObjectRegistry* reg,
  char const* detected_dir)
{
  static bool done = false;
  if (!done)
  {
    csPluginPaths cel_paths;
    csString cel_path = getenv("CEL");
    if (!cel_path.IsEmpty())
    {
      cel_paths.AddOnce(cel_path, CEL_PLUGIN_SCAN_RECURSE, "cel");
      cel_path << CS_PATH_SEPARATOR << "lib";
      cel_paths.AddOnce(cel_path, CEL_PLUGIN_SCAN_RECURSE, "cel");
      cel_path << CS_PATH_SEPARATOR << "cel";
      cel_paths.AddOnce(cel_path, CEL_PLUGIN_SCAN_RECURSE, "cel");
    }

    if (detected_dir != 0)
      cel_paths.AddOnce(detected_dir, CEL_PLUGIN_SCAN_RECURSE, "cel");

    scfInitialize(&cel_paths);
    done = true;
  }
}

bool celInitializer::RequestPlugins(iObjectRegistry* r, ...)
{
  va_list args;
  va_start(args, r);
  bool const ok = celInitializer::RequestPlugins(r, args);
  va_end(args);
  return ok;
}

bool celInitializer::RequestPluginsV(iObjectRegistry* r, va_list args)
{
  SetupConfigManager(r, 0);
  SetupCelPluginDirs(r);
  return superclass::RequestPluginsV(r, args);
}

bool celInitializer::RequestPlugins(iObjectRegistry* r,
  csArray<csPluginRequest> const& a)
{
  SetupConfigManager(r, 0);
  SetupCelPluginDirs(r);
  return superclass::RequestPlugins(r, a);
}
