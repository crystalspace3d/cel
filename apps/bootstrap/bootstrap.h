/*
    Copyright (C) 2003 by Jorrit Tyberghein

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

#ifndef __BOOTSTRAP_H__
#define __BOOTSTRAP_H__

#include <stdarg.h>

struct iEngine;
struct iLoader;
struct iGraphics3D;
struct iKeyboardDriver;
struct iVirtualClock;
struct iObjectRegistry;
struct iEvent;
struct iSector;
struct iView;
class csVector3;

struct iPcCamera;
struct iCelEntity;
struct iCelPlLayer;
struct iCelBlLayer;
struct iCelPropertyClass;
struct iCelPropertyClassFactory;

class Bootstrap
{
public:
  iObjectRegistry* object_reg;

private:
  csRef<iCelPlLayer> pl;
  csRef<iCelBlLayer> bl;
  csRef<iEngine> engine;
  csRef<iKeyboardDriver> kbd;
  csRef<iGraphics3D> g3d;
  csRef<iCelEntity> bootstrap_entity;
 
  static bool BootstrapEventHandler (iEvent& ev);
  bool HandleEvent (iEvent& ev);
  void SetupFrame ();
  void FinishFrame ();

public:
  Bootstrap ();
  ~Bootstrap ();

  bool Initialize (int argc, const char* const argv[]);
  void Start ();
};

#endif // __BOOTSTRAP_H__

