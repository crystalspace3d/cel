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
class celGenericParameterBlock;

CS_PLUGIN_NAMESPACE_BEGIN(pfInput)
{

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
};

struct celButtonMap
{
  celButtonMap *next, *prev;
  csEventID type;
  uint device;
  int numeric;
  uint32 modifiers;
  char *command;
  char *command_end;	// Points to 0 or 1 to indicate positive/negative cmd
};

struct celAxisMap
{
  celAxisMap *next, *prev;
  csEventID type;
  uint device;
  int numeric;
  uint32 modifiers;
  bool recenter;
  char *command;
};

/**
 * This comment is an input property class.
 */
class celPcCommandInput : public scfImplementationExt1<
	celPcCommandInput, celPcCommon, iPcCommandInput>
{
private:
  celKeyMap* keylist;
  celButtonMap* buttonlist;
  celAxisMap* axislist;
  static csStringID id_trigger;
  static csStringID id_command;
  static csStringID action_bind;
  static csStringID id_x;
  static csStringID id_y;
  bool screenspace;
  csRef<iGraphics2D> g2d;
  csRef<iEventNameRegistry> name_reg;
  bool do_cooked;

  celGenericParameterBlock* mouse_params;

  // For properties.
  enum propids
  {
    propid_cooked = 0,
    propid_screenspace
  };
  static Property* properties;
  static size_t propertycount;
  static void UpdateProperties (iObjectRegistry* object_reg);

public:
  celPcCommandInput (iObjectRegistry* object_reg);
  virtual ~celPcCommandInput ();

  virtual const char* GetName () const { return "pccommandinput"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  bool HandleEvent (iEvent& ev);

  virtual void Activate (bool activate = true);
  virtual void SetCookedMode (bool cooked) { do_cooked = cooked; }
  virtual bool GetCookedMode () const { return do_cooked; }
  virtual void ScreenCoordinates (bool screen = true);
  virtual float ScreenToCentered (float screencoord, float axis = 0);
  virtual float CenteredToScreen (float centeredcoord, float axis = 0);
  virtual bool LoadConfig (const char* fname);
  virtual bool Bind (const char* triggername, const char* command);
  virtual const char* GetBind(const char *triggername) const;
  virtual bool RemoveBind (const char* triggername, const char* command);
  virtual void RemoveAllBinds ();

  class EventHandler : public iEventHandler
  {
  private:
    // This is a weak ref so that we can ignore the events
    // that occur when our parent has been deleted.
    csWeakRef<celPcCommandInput> parent;

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
      if (parent)
        return parent->HandleEvent (ev);
      else
	return false;
    }
    CS_EVENTHANDLER_NAMES("cel.propclass.pccommandinput")
    CS_EVENTHANDLER_NIL_CONSTRAINTS
  } *scfiEventHandler;

protected:
  celKeyMap *GetMap (utf32_char key, uint32 modifiers) const;
  celAxisMap *GetAxisMap (csEventID type, uint device, int numeric,
  	uint32 modifiers) const;
  celButtonMap *GetButtonMap (csEventID type, uint device, int numeric,
  	uint32 modifiers) const;
};

}
CS_PLUGIN_NAMESPACE_END(pfInput)

#endif // __CEL_PF_TESTFACT__
