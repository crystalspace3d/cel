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
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pl/facttmpl.h"
#include "pf/common/stdpcimp.h"
#include "pf/input.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (CommandInput)

struct celKeyMap
{
  celKeyMap* next, *prev;
  int key;
  char* command;
  char* command_end;	// Points to 0 or 1 to indicate positive/negative cmd
  bool is_on;
};

/**
 * This is an input property class.
 */
class celPcCommandInput : public celPcCommon
{
private:
  celKeyMap* maplist;

public:
  celPcCommandInput (iObjectRegistry* object_reg);
  virtual ~celPcCommandInput ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pckeyinput"; }
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  bool HandleEvent (iEvent& ev);

  virtual void Activate (bool activate = true);
  virtual bool LoadConfig (const char* fname);
  virtual bool Bind (const char* triggername, const char* command);
  virtual const char* GetBind(const char *triggername) const;
  virtual bool RemoveBind (const char* triggername, const char* command);

  struct PcCommandInput : public iPcCommandInput
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCommandInput);
    virtual void Activate (bool activate = true)
    { scfParent->Activate(activate); }
    virtual bool LoadConfig (const char* fname)
    { return scfParent->LoadConfig(fname); }
    virtual bool Bind (const char* triggername, const char* command)
    { return scfParent->Bind(triggername, command); }
    virtual const char* GetBind (const char* triggername) const
    { return scfParent->GetBind(triggername); }
    virtual bool RemoveBind (const char* triggername, const char* command)
    { return scfParent->RemoveBind(triggername, command); }
  } scfiPcCommandInput;

  class EventHandler : public iEventHandler
  {
  private:
    celPcCommandInput* parent;

  public:
    EventHandler (celPcCommandInput* parent)
    {
      SCF_CONSTRUCT_IBASE(NULL);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler() {}

    SCF_DECLARE_IBASE;

    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;

protected:
  celKeyMap *GetMap(int key) const;
};

#endif // __CEL_PF_TESTFACT__

