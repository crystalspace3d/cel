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

#ifndef __XD_BEHAVIOURLAYER__
#define __XD_BEHAVIOURLAYER__

// CS Includes
#include "csutil/leakguard.h"

// CEL Includes
#include "behaviourlayer/bl.h"

class HoverTest;

/**
 * CEL requires a behaviour layer which will provide the entities with
 * the 'scripts' or behaviours. The behaviours basically control the game
 * logic. In some cases you can use real scripting languages for that (i.e.
 * python) and then you could use the standard CEL python behaviour layer.
 * In our case we want to use normal C++ classes to control the game logic
 * so we make our own behaviour layer which basically just creates the
 * required behaviour class depending on what the entity wants.
 */
class htBehaviourLayer : public scfImplementation1<htBehaviourLayer,
  iCelBlLayer>
{
private:
  HoverTest* app;
iCelPlLayer* pl;

public:
  CS_LEAKGUARD_DECLARE (htBehaviourLayer);

  // Constructor.
  htBehaviourLayer (HoverTest* app, iCelPlLayer *pl);
  // Destructor.
  virtual ~htBehaviourLayer ();

  virtual const char* GetName () const { return "behaviour_layer"; }

  /**
   * We implement this function from iCelBlLayer so that we can create
   * our behaviour classes depending on the given behaviour name (script
   * name).
   */
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);

  /**
   * Get the Ecksdee application class.
   */
  HoverTest* GetApplication () const { return app; }
};

#endif

