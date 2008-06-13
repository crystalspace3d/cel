/*
    Copyright (C) 2004 by Jorrit Tyberghein
    Written by John Harger <jharger AT users.sourceforge.net>

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

#ifndef __CELTOOL_CELTOOLEXTERN_H__
#define __CELTOOL_CELTOOLEXTERN_H__

#include "cssysdef.h"
/* Currently, the only thing provided by celconfig.h is CEL_BUILD_SHARED_LIBS.
   On MSVC, this is set in the project options.
   Also, this file is the only user of celconfig.h. So go with the cheap
   solution and conditionally include celconfig.h if not on MSVC.
   If either more files start to use celconfig.h or more settings are stored
   in it, a better solution, such as the one used by CS to wrap csconfig.h,
   should be sought.
 */
#if !defined(CS_COMPILER_MSVC)
#include "celconfig.h"
#endif

#if defined(CS_PLATFORM_WIN32)
  #if defined(CEL_BUILD_SHARED_LIBS)
    #define CEL_EXPORT_SYM CS_EXPORT_SYM_DLL
    #define CEL_IMPORT_SYM CS_IMPORT_SYM_DLL
  #else
    #define CEL_EXPORT_SYM
    #define CEL_IMPORT_SYM
  #endif // CEL_BUILD_SHARED_LIBS
#else
  #if defined(CEL_BUILD_SHARED_LIBS)
    #define CEL_EXPORT_SYM CS_VISIBILITY_DEFAULT
  #else
    #define CEL_EXPORT_SYM
  #endif
  #define CEL_IMPORT_SYM
#endif

#ifdef CEL_CELTOOL_LIB
  #define CEL_CELTOOL_EXPORT CEL_EXPORT_SYM
#else
  #define CEL_CELTOOL_EXPORT CEL_IMPORT_SYM
#endif

#endif // __CELTOOL_CELTOOLEXTERN_H__
