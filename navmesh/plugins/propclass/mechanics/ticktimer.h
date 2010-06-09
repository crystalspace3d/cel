/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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

#ifndef __CEL_OBJ_TICK_TIMER__
#define __CEL_OBJ_TICK_TIMER__

#include <csutil/scf_implementation.h>
#include <csutil/weakref.h>
#include <physicallayer/pl.h>

class celPeriodicTimer
{
public:
  celPeriodicTimer (csWeakRef<iCelPlLayer> pl) : tick (100),
  	wref_physical_layer (pl)
  {
    scfiCelTimerListener = new CelTimerListener (this);
    pl->CallbackOnce (scfiCelTimerListener, tick, CEL_EVENT_PRE);
  }
  virtual ~celPeriodicTimer () { scfiCelTimerListener->DecRef (); }

  virtual void Tick () = 0;
  virtual void SetTickTime (csTicks tck) { tick = tck; }

private:
  // Made independent to avoid circular refs and leaks.
  struct CelTimerListener : public scfImplementation1<
  	CelTimerListener, iCelTimerListener>
  {
    celPeriodicTimer* parent;
    CelTimerListener (celPeriodicTimer* parent) :
    	scfImplementationType (this), parent (parent) { }
    virtual ~CelTimerListener () { }
    virtual void TickEveryFrame ()
    {
      return;
    }
    virtual void TickOnce ()
    {
      parent->wref_physical_layer->CallbackOnce (this, parent->tick,
      	CEL_EVENT_PRE);
      parent->Tick ();
    }
  } * scfiCelTimerListener;

  csTicks tick;
  csWeakRef<iCelPlLayer> wref_physical_layer;
};

#endif
