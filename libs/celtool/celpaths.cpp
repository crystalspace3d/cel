/*
    Copyright (C) 2004 by Eric Sunshine
              (C) 2008 by Frank Richter

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

#include <cssysdef.h>
#include "celtool/celpaths.h"
#include "packagedefs.h"

// These defines should be set by the configure script
#ifndef CEL_CONFIGDIR
#ifdef CS_COMPILER_GCC
#warning CEL_CONFIGDIR not set
#endif
#define CEL_CONFIGDIR "/usr/local/" CEL_PACKAGE_NAME_VER
#endif

namespace CEL
{
  csPathsList* GetPlatformInstallationPaths ()
  {
    csPathsList cel_paths;
    csString cel_env (getenv ("CEL_" CEL_VERSION_STR));
    if (cel_env.IsEmpty())
      cel_env = getenv ("CEL");
    if (!cel_env.IsEmpty())
    {
      cel_paths = csPathsList (cel_env);
      // Assign "cel" type to all paths
      for (size_t i = 0; i < cel_paths.GetSize(); i++)
        cel_paths[i].type = "cel";
    }
    else
    {
      cel_paths.AddUniqueExpanded (CEL_CONFIGDIR);
    }
    return new csPathsList (cel_paths);
  }
} // namespace CEL

