/*
    Copyright (C) 2001 by Jorrit Tyberghein

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

#ifndef __CELTEST_H__
#define __CELTEST_H__

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

struct iCelEntity;
struct iCelPlLayer;
struct iCelBlLayer;
struct iCelPropertyClass;
struct iCelPropertyClassFactory;

#define OBJ_SPARKBOX "sparkbox"
#define OBJ_MARBLEBOX "marblebox"

class CelTest
{
private:
  iObjectRegistry* object_reg;
  iEngine* engine;
  iLoader* loader;
  iGraphics3D* g3d;
  iKeyboardDriver* kbd;
  iVirtualClock* vc;
  iSector* room;
  iView* view;

  iCelPlLayer* pl;
  iCelBlLayer* bl;
  iCelEntity* game;
 
  static bool CelTestEventHandler (iEvent& ev);
  bool HandleEvent (iEvent& ev);
  void SetupFrame ();
  void FinishFrame ();

  bool LoadTexture (const char* txtName, const char* fileName);
  bool LoadMeshFactory (const char* fileName);
  iCelPropertyClassFactory* LoadPcFactory (const char* pcfactname);
  iCelPropertyClass* CreatePropertyClass (iCelEntity* entity, iCelPropertyClassFactory* pf,
		const char* name);
  void CreateObject (int x, int y, int z, const char* obj);
  bool CreateRoom ();

public:
  CelTest ();
  ~CelTest ();

  bool Initialize (int argc, const char* const argv[]);
  void Start ();
};

#endif // __CELTEST_H__

