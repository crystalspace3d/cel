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
class csVector3;

struct iPcCamera;
struct iCelEntity;
struct iCelPlLayer;
struct iCelBlLayer;
struct iCelPropertyClass;
struct iCelPropertyClassFactory;

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

  iCelPlLayer* pl;
  iCelBlLayer* bl;
  iCelEntity* game;
 
  static bool CelTestEventHandler (iEvent& ev);
  bool HandleEvent (iEvent& ev);
  void SetupFrame ();
  void FinishFrame ();

  bool LoadPcFactory (const char* classname);
  bool LoadTextures ();
  bool LoadTexture (const char* txtName, const char* fileName);
  bool CreateRoom ();
  iCelEntity* CreateBoxEntity (const char* name, const char* factName,
  	iPcCamera* pccamera,
  	float weight, float size,
	float max_indiv_weight, float max_weight,
	float max_indiv_size, float max_size,
  	const csVector3& pos);
  iCelEntity* CreateDummyEntity (const char* name, const char* factName,
  	float weight, float size,
  	const csVector3& pos, const csVector3& force);
  iCelEntity* CreateActor (const char *name, const char* factName,
	const csVector3& pos);
  
public:
  CelTest ();
  ~CelTest ();

  bool Initialize (int argc, const char* const argv[]);
  void Start ();
};

#endif // __CELTEST_H__

