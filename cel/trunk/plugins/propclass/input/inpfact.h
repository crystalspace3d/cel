/*
    Crystal Space Entity Layer
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

#ifndef __CEL_PF_INPFACT__
#define __CEL_PF_INPFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "propclass/input.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;
struct iGraphics2D;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (CommandInput)

struct celKeyMap
{
  celKeyMap *next, *prev;
  utf32_char key;
  uint32 modifiers;
  char *command;
  char *command_end;	// Points to 0 or 1 to indicate positive/negative cmd
  bool is_on;
};

struct celButtonMap
{
  celButtonMap *next, *prev;
  int type, numeric;
  uint32 modifiers;
  char *command;
  char *command_end;	// Points to 0 or 1 to indicate positive/negative cmd
  bool is_on;
};

struct celAxisMap
{
  celAxisMap *next, *prev;
  int type, numeric;
  uint32 modifiers;
  bool recenter;
  char *command;
};

/**
 * This comment is an input property class.
 */
class celPcCommandInput : public celPcCommon
{
private:
  celKeyMap* keylist;
  celButtonMap* buttonlist;
  celAxisMap* axislist;
  static csStringID id_trigger;
  static csStringID id_command;
  static csStringID action_bind;
  bool screenspace;
  csRef<iGraphics2D> g2d;

public:
  celPcCommandInput (iObjectRegistry* object_reg);
  virtual ~celPcCommandInput ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pccommandinput"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  bool HandleEvent (iEvent& ev);

  virtual void Activate (bool activate = true);
  virtual void ScreenCoordinates (bool screen = true);
  virtual float ScreenToCentered (float screencoord, float axis = 0);
  virtual float CenteredToScreen (float centeredcoord, float axis = 0);
  virtual bool LoadConfig (const char* fname);
  virtual bool Bind (const char* triggername, const char* command);
  virtual const char* GetBind(const char *triggername) const;
  virtual bool RemoveBind (const char* triggername, const char* command);
  virtual void RemoveAllBinds ();

  struct PcCommandInput : public iPcCommandInput
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCommandInput);
    virtual void Activate (bool activate = true)
    { scfParent->Activate(activate); }
    virtual void ScreenCoordinates (bool screen = true)
    { scfParent->ScreenCoordinates(screen); }
    virtual float ScreenToCentered (float screencoord, float axis = 0)
    { return scfParent->ScreenToCentered(screencoord, axis); }
    virtual float CenteredToScreen (float centeredcoord, float axis = 0)
    { return scfParent->CenteredToScreen(centeredcoord, axis); }
    virtual bool LoadConfig (const char* fname)
    { return scfParent->LoadConfig(fname); }
    virtual bool Bind (const char* triggername, const char* command)
    { return scfParent->Bind(triggername, command); }
    virtual const char* GetBind (const char* triggername) const
    { return scfParent->GetBind(triggername); }
    virtual bool RemoveBind (const char* triggername, const char* command)
    { return scfParent->RemoveBind(triggername, command); }
    virtual void RemoveAllBinds ()
    { scfParent->RemoveAllBinds(); }
  } scfiPcCommandInput;

  class EventHandler : public iEventHandler
  {
  private:
    celPcCommandInput* parent;

  public:
    EventHandler (celPcCommandInput* parent)
    {
      SCF_CONSTRUCT_IBASE(0);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler()
    {
      SCF_DESTRUCT_IBASE ();
    }

    SCF_DECLARE_IBASE;

    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;

protected:
  celKeyMap *GetMap (utf32_char key, uint32 modifiers) const;
  celAxisMap *GetAxisMap (int type, int numeric, uint32 modifiers) const;
  celButtonMap *GetButtonMap (int type, int numeric, uint32 modifiers) const;
};

#endif // __CEL_PF_TESTFACT__

