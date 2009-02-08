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
#include "celplatform.h"

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
