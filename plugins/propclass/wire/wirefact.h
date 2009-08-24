/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_WIREFACT__
#define __CEL_PF_WIREFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/wire.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for wire.
 */
CEL_DECLARE_FACTORY (Wire)

struct celWireOutputMessage
{
  csString msgid;
  csWeakRef<iMessageChannel> channel;
  csRef<iCelParameterBlock> extra_params;
};

struct celWireOutputAction
{
  csStringID actionID;
  csWeakRef<iCelPropertyClass> pc;
  csRef<iCelParameterBlock> extra_params;
};

/**
 * This is a test property class.
 */
class celPcWire : public scfImplementationExt2<
	celPcWire, celPcCommon, iPcWire, iMessageReceiver>
{
private:
  static csStringID id_mask;
  static csStringID id_entity;
  static csStringID id_msgid;
  static csStringID id_actionid;
  static csStringID id_pc;

  // For actions.
  enum actionids
  {
    action_addinput = 0,
    action_addoutput,
    action_addaction
  };
  static PropertyHolder propinfo;

  csSafeCopyArray<celWireOutputMessage> output_messages;
  csSafeCopyArray<celWireOutputAction> output_actions;

public:
  celPcWire (iObjectRegistry* object_reg);
  virtual ~celPcWire ();

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);

  virtual void AddInput (const char* msg_mask, iMessageChannel* channel = 0);
  virtual void AddOutput (const char* msgid, iMessageChannel* channel = 0,
      iCelParameterBlock* extra_params = 0);
  virtual void AddOutputAction (csStringID actionID, iCelPropertyClass* pc,
      iCelParameterBlock* extra_params = 0);

  // For iMessageReceiver.
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
};

#endif // __CEL_PF_WIREFACT__

