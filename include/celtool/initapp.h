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

#ifndef __CELTOOL_INITAPP_H__
#define __CELTOOL_INITAPP_H__

#include <cssysdef.h>
#include <cstool/initapp.h>

#ifndef CEL_DETECTED_PLUGIN_DIR
#  ifdef CEL_PLUGIN_DIR
#    define CEL_DETECTED_PLUGIN_DIR CEL_PLUGIN_DIR
#  else
#    define CEL_DETECTED_PLUGIN_DIR ((char const*)0)
#  endif
#endif

/**
 * This class contains static member functions which can help set up an
 * application to use the Crystal Entity Layter. It is possible to do all the
 * setup on your own but using the functions below will considerably simplify
 * the task.
 */
class celInitializer : public csInitializer
{
private:
  typedef csInitializer superclass;

protected:
  static void setup_plugin_dirs(iObjectRegistry*, char const* detected_dir);

public:
  /**
   * Instructs SCF to also scan the CEL plugin directory or directories This
   * method is invoked automatically by celInitializer::RequestPlugins(), so
   * you do not normally need to call it. You may, however, want to call it
   * if you do not utilize celInitializer::RequestPlugins(). If you do call it
   * manually, it usually is wise to do so before invoking the underlying
   * csInitializer::RequestPlugins() or #CS_REQUEST_PLUGIN().
   * <p>
   * The following are queried in an attempt to locate the CEL plugin
   * directory:
   * <ul>
   * <li>The #CEL environment variable (as well as several likely
   *     subdirectories of the mentioned path).</li>
   * <li>The #CEL_PLUGIN_DIR compile-time constant, if available. The typical
   *     way to define this property is to have the configure.ac script invoke
   *     AC_PATH_CEL or CS_PATH_CEL (defined in cel.m4) and to then
   *     "substitute" the discovered value of the plugin directory into the
   *     generated config.h file as follows:
   *       \code
   *       CS_PATH_CEL([someversion], [], [AC_MSG_ERROR([Failed to find CEL])])
   *       AC_DEFINE_UNQUOTED([CEL_PLUGIN_DIR], ["$CEL_PLUGIN_DIR"],
   *         [Define with path to CEL plugins.])
   *       \endcode
   * </ul>
   */
  static void SetupCelPluginDirs(iObjectRegistry* r)
  { setup_plugin_dirs(r, CEL_DETECTED_PLUGIN_DIR); }

  /**
   * A convenience wrapper for csInitializer::RequestPlugins() which first
   * invokes celInitializer::SetupCelPluginDirs().
   * <p>
   * The variable arguments should contain four entries for every plugin you
   * wish to load: SCF class name, SCF interface name, inteface ID, and
   * interface version. To make this easier it is recommended that you use one
   * of the #CS_REQUEST_xxx macros (see <cstool/initapp.h>). <b>WARNING</b> Be
   * sure to terminate the list with #CS_REQUEST_END!
   */
  static bool RequestPlugins(iObjectRegistry*, ...);

  /**
   * This is just like RequestPlugins(...), which accepts a variable list of
   * arguments at compile-time, except that arguments are passed as a
   * `va_list'.
   */
  static bool RequestPluginsV (iObjectRegistry*, va_list);

  /**
   * A convenience wrapper for csInitializer::RequestPlugins() which first
   * invokes celInitializer::SetupCelPluginDirs().
   * <p>
   * Unlike the variable-argument RequestPlugins(...) method which expects you
   * to know the list of requested plugins at compile-time, this overload
   * allows you to construct an array of plugins at run-time.  You do this by
   * constructing a csArray<> of csPluginRequest records (see
   * <cstool/initapp.h>).  For example:
   * \code
   * csArray<csPluginRequest> a;
   * a.Push(csPluginRequest(CS_REQUEST_VFS));
   * a.Push(csPluginRequest(CS_REQUEST_ENGINE));
   * a.Push(csPluginRequest(CS_REQUEST_PLUGIN("myproj.foobar",iFoobar)));
   * celInitializer::RequestPlugins(registry,a);
   * \endcode
   * <b>WARNING</b> csArray<> already knows its own size, so do <b>not</b>
   * terminate the list with #CS_REQUEST_END.
   */
  static bool RequestPlugins(iObjectRegistry*,csArray<csPluginRequest> const&);
};

/** @} */

#endif // __CELTOOL_INITAPP_H__
