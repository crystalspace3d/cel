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
class celPcWire;

/**
 * Factory for wire.
 */
CEL_DECLARE_FACTORY (Wire)

/**
 * A representation of an output.
 */
class celWireOutput : public csRefCount
{
public:
  csRef<iCelParameterBlock> extra_params;

protected:
  csArray<celParameterMapping> mappings;

  static csRef<iCelParameterBlock> CombineParams (iCelParameterBlock* p1, iCelParameterBlock* p2);
  csRef<iCelParameterBlock> MapParams (iCelEntity* entity, iCelParameterBlock* params);

public:
  virtual ~celWireOutput () { }
  virtual void Do (celPcWire* wire, iCelParameterBlock* params) = 0;
  void AddMapping (const char* source, const char* dest, iCelExpression* expression);
};

class celWireOutputMessage : public celWireOutput
{
private:
  csString msgid;
  csWeakRef<iMessageChannel> channel;

public:
  celWireOutputMessage (csString msgid, iMessageChannel* channel) :
    msgid (msgid), channel (channel) { }
  virtual ~celWireOutputMessage () { }
  virtual void Do (celPcWire* wire, iCelParameterBlock* params);
};

class celWireOutputAction : public celWireOutput
{
private:
  csStringID actionID;
  csWeakRef<iCelPropertyClass> pc;

public:
  celWireOutputAction (csStringID actionID, iCelPropertyClass* pc) :
    actionID (actionID), pc (pc) { }
  virtual ~celWireOutputAction() { }
  virtual void Do (celPcWire* wire, iCelParameterBlock* params);
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
  static csStringID id_id;
  static csStringID id_source;
  static csStringID id_dest;
  static csStringID id_expression;

  // For actions.
  enum actionids
  {
    action_addinput = 0,
    action_addoutput,
    action_addaction,
    action_mapparameter
  };
  static PropertyHolder propinfo;

  csRefArray<celWireOutput> output;

public:
  celPcWire (iObjectRegistry* object_reg);
  virtual ~celPcWire ();

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);

  virtual void AddInput (const char* msg_mask, iMessageChannel* channel = 0);
  virtual size_t AddOutput (const char* msgid, iMessageChannel* channel = 0,
      iCelParameterBlock* extra_params = 0);
  virtual size_t AddOutputAction (csStringID actionID, iCelPropertyClass* pc,
      iCelParameterBlock* extra_params = 0);
  virtual void MapParameter (size_t id, const char* source, const char* dest,
      iCelExpression* expression = 0);

  // For iMessageReceiver.
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
};

#endif // __CEL_PF_WIREFACT__

