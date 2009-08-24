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

#include "cssysdef.h"
#include "iutil/objreg.h"
#include "plugins/propclass/wire/wirefact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Wire, "pcmisc.wire")

//---------------------------------------------------------------------------

csStringID celPcWire::id_mask = csInvalidStringID;
csStringID celPcWire::id_entity = csInvalidStringID;
csStringID celPcWire::id_msgid = csInvalidStringID;
csStringID celPcWire::id_actionid = csInvalidStringID;
csStringID celPcWire::id_pc = csInvalidStringID;

PropertyHolder celPcWire::propinfo;

celPcWire::celPcWire (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  if (id_mask == csInvalidStringID)
  {
    id_mask = pl->FetchStringID ("cel.parameter.mask");
    id_entity = pl->FetchStringID ("cel.parameter.entity");
    id_msgid = pl->FetchStringID ("cel.parameter.msgid");
    id_actionid = pl->FetchStringID ("cel.parameter.actionid");
    id_pc = pl->FetchStringID ("cel.parameter.pc");
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_addinput, "cel.action.AddInput");
    AddAction (action_addoutput, "cel.action.AddOutput");
    AddAction (action_addaction, "cel.action.AddAction");
  }
}

celPcWire::~celPcWire ()
{
}

#define WIRE_SERIAL 1

csPtr<iCelDataBuffer> celPcWire::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (WIRE_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcWire::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != WIRE_SERIAL) return false;

  return true;
}

bool celPcWire::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_addinput:
      {
        CEL_FETCH_STRING_PAR (mask,params,id_mask);
        if (!p_mask) return false;
        CEL_FETCH_STRING_PAR (entity,params,id_entity);
	iCelEntity* ent = this->entity;
        if (p_entity)
	  ent = pl->FindEntity (entity);
	// @@@ Error check on ent!
	AddInput (mask, ent->QueryMessageChannel ());
        return true;
      }
    case action_addoutput:
      {
        CEL_FETCH_STRING_PAR (msgid,params,id_msgid);
        if (!p_msgid) return false;
        CEL_FETCH_STRING_PAR (entity,params,id_entity);
	iCelEntity* ent = this->entity;
        if (p_entity)
	  ent = pl->FindEntity (entity);
	// @@@ Error check on ent!
	AddOutput (msgid, ent->QueryMessageChannel (), params);
        return true;
      }
    case action_addaction:
      {
        CEL_FETCH_STRING_PAR (actionid,params,id_actionid);
        if (!p_actionid) return false;
        CEL_FETCH_STRING_PAR (entity,params,id_entity);
	iCelEntity* ent = this->entity;
        if (p_entity)
	  ent = pl->FindEntity (entity);
	// @@@ Error check on ent!
        CEL_FETCH_STRING_PAR (pcname,params,id_pc);
        if (!p_pcname) return false;
	iCelPropertyClassList* pclist = ent->GetPropertyClassList ();
	iCelPropertyClass* pc = pclist->FindByName (pcname);
	// @@@ Produce error?
	if (!pc) return false;
	csStringID actionID = pl->FetchStringID (actionid);
	AddOutputAction (actionID, pc, params);
        return true;
      }
    default:
      return false;
  }
  return false;
}

static csRef<iCelParameterBlock> CombineParams (iCelParameterBlock* p1, iCelParameterBlock* p2)
{
  if (!p1) return p2;
  if (!p2) return p1;
  csRef<iCelParameterBlock> b;
  b.AttachNew (new celCombineParameterBlock (p1, p2));
  return b;
}

bool celPcWire::ReceiveMessage (csStringID msgid, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  size_t i;
  for (i = 0 ; i < output_messages.GetSize () ; i++)
  {
    const celWireOutputMessage& out = output_messages[i];
    if (out.channel)
      out.channel->SendMessage (out.msgid, this, CombineParams (out.extra_params, params));
  }
  for (i = 0 ; i < output_actions.GetSize () ; i++)
  {
    const celWireOutputAction& out = output_actions[i];
    if (out.pc)
    {
      celData ret;
      out.pc->PerformAction (out.actionID, CombineParams (out.extra_params, params), ret);
    }
  }
  return true;
}

void celPcWire::AddInput (const char* msg_mask, iMessageChannel* channel)
{
  if (channel == 0)
    channel = entity->QueryMessageChannel ();
  channel->Subscribe (this, msg_mask);
}

void celPcWire::AddOutput (const char* msgid, iMessageChannel* channel,
      iCelParameterBlock* extra_params)
{
  celWireOutputMessage out;
  out.msgid = msgid;
  if (channel == 0)
    out.channel = entity->QueryMessageChannel ();
  else
    out.channel = channel;
  out.extra_params = extra_params;
  output_messages.Push (out);
}

void celPcWire::AddOutputAction (csStringID actionID, iCelPropertyClass* pc,
      iCelParameterBlock* extra_params)
{
  celWireOutputAction out;
  out.actionID = actionID;
  out.pc = pc;
  out.extra_params = extra_params;
  output_actions.Push (out);
}

//---------------------------------------------------------------------------

