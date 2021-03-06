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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CELTEST_H__
#define __CELTEST_H__

#include <stdarg.h>

// CS Includes
#include "csutil/csbaseeventh.h"
#include "cstool/csapplicationframework.h"

// CEL Includes
#include "physicallayer/messaging.h"
#include "actorsettings.h"

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
class FramePrinter;

struct iPcCamera;
struct iCelEntity;
struct iCelPlLayer;
struct iCelBlLayer;
struct iCelPropertyClass;
struct iCelPropertyClassFactory;
struct iPcWire;

class CelTest;

/**
 * Main application class of CelTest.
 */
class CelTest : public csApplicationFramework,
		public csBaseEventHandler
{
private:
  csRef<iEngine> engine;
  csRef<iLoader> loader;
  csRef<iGraphics3D> g3d;
  csRef<iKeyboardDriver> kbd;
  csRef<iVirtualClock> vc;
  csRef<FramePrinter> printer;

  csRef<iCelPlLayer> pl;
  csRef<iCelEntity> game;
  csRef<iCelEntity> entity_cam;

  ActorSettings actorsettings;

  /**
   * Setup everything that needs to be rendered on screen. This routine
   * is called from the event handler in response to a csevFrame
   * broadcast message.
   */
  virtual void Frame ();

  /**
   * Handle keyboard events - ie key presses and releases.
   * This routine is called from the event handler in response to a 
   * csevKeyboard event.
   */
  virtual bool OnKeyboard (iEvent &event);

  bool CreateRoom ();
  void CreateActor ();
  void CreateActionIcon ();
  void CreateSettingBar ();
  void ConnectWires ();

  size_t WireAddOutput (iPcWire* wire, const char* msgid,
		  iCelParameterBlock* params = 0);

public:
  CelTest ();
  virtual ~CelTest ();

  /**
   * Final cleanup.
   */
  virtual void OnExit ();

  /**
   * Main initialization routine. This routine will set up some basic stuff
   * (like load all needed plugins, setup the event handler, ...).
   * In case of failure this routine will return false. You can assume
   * that the error message has been reported to the user.
   */
  virtual bool OnInitialize (int argc, char* argv[]);

  /**
   * Run the application.
   * First, there are some more initialization (everything that is needed 
   * by CrystalCore to use Crystal Space and CEL), Then this routine fires up 
   * the main event loop. This is where everything starts. This loop will 
   * basically start firing events which actually causes Crystal Space to 
   * function. Only when the program exits this function will return.
   */
  virtual bool Application ();

  CS_EVENTHANDLER_NAMES("cel.apps.celtest")
  CS_EVENTHANDLER_NIL_CONSTRAINTS
};

#endif // __CELTEST_H__

