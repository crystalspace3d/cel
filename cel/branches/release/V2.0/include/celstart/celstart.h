/*
    Copyright (C) 2008 by Frank Richter

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

#ifndef __CEL_CELSTART_CELSTART_H__
#define __CEL_CELSTART_CELSTART_H__

#include <cssysdef.h>
#include <csutil/dirtyaccessarray.h>
#include <csutil/mempool.h>
#include "celtool/celtoolextern.h"

#ifdef CEL_CELSTARTCORE_LIB
  #define CEL_CELSTARTCORE_EXPORT CEL_EXPORT_SYM
#else
  #define CEL_CELSTARTCORE_EXPORT CEL_IMPORT_SYM
#endif

namespace CEL
{
  /**
   * Main function for running CELstart.
   */
  CEL_CELSTARTCORE_EXPORT int CelStartMain (int argc, const char* const argv[]);
  
  /**
   * Class for running CELstart, simplifying manipulation of the passed command
   * line arguments.
   *
   * Example:
   * \code
   * CelStartWrapper celstart (argc, argv);
   * // Always start with custom game data
   * celstart.AddArgumentBeforeUserArgs ("mydata.celzip");
   * return celstart.Main();
   * \endcode
   */
  class CEL_CELSTARTCORE_EXPORT CelStartWrapper
  {
    csMemoryPool extraArgsPool;
    csDirtyAccessArray<const char*> args;
    size_t userArgsFirst;
    size_t userArgsCount;
  public:
    CelStartWrapper (int argc, const char* const argv[]);
  
    /**
     * Add the given argument before the user-given command line arguments.
     *
     * Example: if the user has given <tt>-one -two -three</tt> and 
     * AddArgumentBeforeUserArgs() was called with <tt>"-a"</tt>,
     * <tt>"-b"</tt>, <tt>"-c"</tt> the final command line as seen by the
     * CELstart main function is <tt>-a -b -c -one -two -three</tt>.
     */
    void AddArgumentBeforeUserArgs (const char* arg);
    /// Add the given argument after the user-given command line arguments
    void AddArgumentAfterUserArgs (const char* arg);
  
    /// Run.
    int Main ();
  };
} // namespace CEL

#endif // __CEL_CELSTART_CELSTART_H__
