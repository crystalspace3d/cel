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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CELSTART_H__
#define __CELSTART_H__

#include <stdarg.h>
#include "csutil/stringarray.h"
#include "csutil/parray.h"
#include "iutil/stringarray.h"
#include "cstool/cspixmap.h"
#include "behaviourlayer/bl.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"

struct iGraphics2D;
struct iGraphics3D;
struct iMouseDriver;
struct iObjectRegistry;
struct iEvent;
struct iVFS;
struct iFont;

// This is a version number for celstart.
#define CELSTART_VERSION 6
// This is the minimum version for demo files that we still support.
#define CELSTART_MINIMUMVERSION -1

class CelStart
{
public:
  iObjectRegistry* object_reg;

private:
  csRef<iCelPlLayer> pl;
  csRef<iGraphics3D> g3d;
  bool do_clearscreen;
  bool do_real_demo;

  // Everything for the demo selection screen.
  csRef<iFont> font;
  csRef<iMouseDriver> mouse;
  int font_fg, font_bg;
  int font_error_fg;
  int sel_font_fg, sel_font_bg;
  int box_color1, box_color2, box_color3;
  int sel_box_color1;
  int message_color;
  csString startme;
  int argc;
  const char** argv;
  csStringArray files;
  csStringArray names;
  csStringArray descriptions;
  csStringArray warnings;
  csPDelArray<csSimplePixmap> icons;
  int top_file;
  csString realAppResDir;
  
  // Some metrics for the selector box
  static const int boxMarginX = 7;
  static const int boxMarginY = 2;
  static const int boxTopY = 7;
  static const int boxH = 70;
  static const int boxInnerBorder = 3;
  static const int iconW = 80;
  static const int iconH = 60;
  static const int iconMarginRight = 10;
  static const int textY = 5;
  
  int GetHitBox (int mx, int my);
  void DrawSelectorBox (iGraphics2D* g2d, int x, int y, int w, int h, 
    bool highlight);
 
  static bool CelStartEventHandler (iEvent& ev);
  bool HandleEvent (iEvent& ev);
  void SetupFrame ();
  void FinishFrame ();
  bool FindPath (iVFS* vfs, csString& realpath, csString& path,
      csString& configname);
  void FindCelStartArchives ();
  bool StartDemo (int argc, const char* const argv[],
    const char* realpath, const char* path, const char* configname);
  bool StartDemoSelector (int argc, const char* const argv[]);

public:
  CelStart ();
  ~CelStart ();

  bool Initialize (int argc, const char* const argv[]);
  void Start ();
};

#endif // __CELSTART_H__

