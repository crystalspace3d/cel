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
#include "behaviourlayer/bl.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"

struct iGraphics3D;
struct iObjectRegistry;
struct iEvent;
struct iVFS;

class CelStart
{
public:
  iObjectRegistry* object_reg;

private:
  csRef<iCelPlLayer> pl;
  csRef<iCelBlLayer> bl;
  csRef<iGraphics3D> g3d;
 
  static bool CelStartEventHandler (iEvent& ev);
  bool HandleEvent (iEvent& ev);
  void SetupFrame ();
  void FinishFrame ();
  bool FindPath (iVFS* vfs, csString& path, csString& configname);

  bool do_clearscreen;

public:
  CelStart ();
  ~CelStart ();

  bool Initialize (int argc, const char* const argv[]);
  void Start ();
};

#endif // __CELSTART_H__

