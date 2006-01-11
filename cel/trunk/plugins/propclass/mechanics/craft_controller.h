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

#ifndef __CEL_PF_CRAFTCONTROLLER_FACTORY__
#define __CEL_PF_CRAFTCONTROLLER_FACTORY__

#include "cstypes.h"

#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"

#include "propclass/craft.h"

/**
 * Factory for hover.
 */
CEL_DECLARE_FACTORY (CraftController)

/**
 * Hover stabiliser property class.
 */
class celPcCraftController : public celPcCommon
{
public:
  celPcCraftController (iObjectRegistry* object_reg);
  virtual ~celPcCraftController ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName() const { return "pccraft"; };
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  /**
   * Should Tick every frame and update the objects
   * height
   */
  virtual void UpdateBody();

  /* I have done the functions this way, as opposed to
       TurnLeft(float isturning);
     so that later on they can easily be extended with
     initial reactionaries */
  virtual void StartTurnLeft() { turn_left = true; }
  virtual void StopTurnLeft() { turn_left = false; }
  virtual void StartTurnRight() { turn_right = true; }
  virtual void StopTurnRight() { turn_right = false; }

  virtual void StartTurnUp() { turn_up = true; }
  virtual void StopTurnUp() { turn_up = false; }
  virtual void StartTurnDown() { turn_down = true; }
  virtual void StopTurnDown() { turn_down = false; }

  struct PcCraftController : public iPcCraftController
  {
    SCF_DECLARE_EMBEDDED_IBASE(celPcCraftController);

    virtual void StartTurnLeft()
    {
      scfParent->StartTurnLeft();
    }
    virtual void StopTurnLeft()
    {
      scfParent->StopTurnLeft();
    }
    virtual void StartTurnRight()
    {
      scfParent->StartTurnRight();
    }
    virtual void StopTurnRight()
    {
      scfParent->StopTurnRight();
    }

    virtual void StartTurnUp()
    {
      scfParent->StartTurnUp();
    }
    virtual void StopTurnUp()
    {
      scfParent->StopTurnUp();
    }
    virtual void StartTurnDown()
    {
      scfParent->StartTurnDown();
    }
    virtual void StopTurnDown()
    {
      scfParent->StopTurnDown();
    }
  } scfiPcCraftController;

  // Made independent to avoid circular refs and leaks.
  struct CelTimerListener : public scfImplementation1<
  	CelTimerListener, iCelTimerListener>
  {
    celPcCraftController* parent;
    CelTimerListener (celPcCraftController* parent) :
    	scfImplementationType (this), parent (parent) { }
    virtual ~CelTimerListener () { }
    virtual void TickEveryFrame ()
    {
      parent->UpdateBody ();
    }
    virtual void TickOnce ()
    {
      return;
    }
  } * scfiCelTimerListener;
private:
  void DoTurningCalc (bool isturning, float &turn, float acc, float max);

  bool turn_left, turn_right;
  bool turn_up, turn_down;

  float current_turning_left, current_turning_right;
  float current_up, current_down;

  float turn_acc, updwn_acc;
  float turn_max, updwn_max;
};

#endif
