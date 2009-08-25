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

csRef<iCelParameterBlock> celWireOutput::CombineParams (iCelParameterBlock* p1, iCelParameterBlock* p2)
{
  if (!p1) return p2;
  if (!p2) return p1;
  csRef<iCelParameterBlock> b;
  b.AttachNew (new celCombineParameterBlock (p1, p2));
  return b;
}

csRef<iCelParameterBlock> celWireOutput::MapParams (iCelEntity* entity, iCelParameterBlock* params)
{
  if (mappings.GetSize () == 0)
    return params;
  //csRef<celMappedParameterBlock> newparams;
  //newparams.AttachNew (new celMappedParameterBlock (params, mapping));
  //return newparams;
  return 0;
}

void celWireOutput::AddMapping (const char* source, const char* dest, iCelExpression* expression)
{
  celParameterMapping mapping;
  mapping.source = source;
  mapping.dest = dest;
  mapping.expression = expression;
  mappings.Push (mapping);
}

void celWireOutputMessage::Do (celPcWire* wire, iCelParameterBlock* params)
{
  if (channel)
    channel->SendMessage (msgid, wire, MapParams (wire->GetEntity (), CombineParams (extra_params, params)));
}

void celWireOutputAction::Do (celPcWire* wire, iCelParameterBlock* params)
{
  if (pc)
  {
    celData ret;
    pc->PerformAction (actionID, MapParams (wire->GetEntity (), CombineParams (extra_params, params)), ret);
  }
}

//---------------------------------------------------------------------------

csStringID celPcWire::id_mask = csInvalidStringID;
csStringID celPcWire::id_entity = csInvalidStringID;
csStringID celPcWire::id_msgid = csInvalidStringID;
csStringID celPcWire::id_actionid = csInvalidStringID;
csStringID celPcWire::id_pc = csInvalidStringID;
csStringID celPcWire::id_id = csInvalidStringID;
csStringID celPcWire::id_source = csInvalidStringID;
csStringID celPcWire::id_dest = csInvalidStringID;
csStringID celPcWire::id_expression = csInvalidStringID;

PropertyHolder celPcWire::propinfo;

celPcWire::celPcWire (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  if (id_mask == csInvalidStringID)
  {
    id_mask = pl->FetchStringID ("mask");
    id_entity = pl->FetchStringID ("entity");
    id_msgid = pl->FetchStringID ("msgid");
    id_actionid = pl->FetchStringID ("actionid");
    id_pc = pl->FetchStringID ("pc");
    id_id = pl->FetchStringID ("id");
    id_source = pl->FetchStringID ("source");
    id_dest = pl->FetchStringID ("dest");
    id_expression = pl->FetchStringID ("expression");
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_addinput, "AddInput");
    AddAction (action_addoutput, "AddOutput");
    AddAction (action_addaction, "AddAction");
    AddAction (action_mapparameter, "MapParameter");
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
    case action_mapparameter:
      {
        CEL_FETCH_LONG_PAR (id,params,id_id);
        if (!p_id) return false;
        CEL_FETCH_STRING_PAR (source,params,id_source);
        if (!p_source) return false;
        CEL_FETCH_STRING_PAR (dest,params,id_dest);
        if (!p_dest) return false;
        CEL_FETCH_STRING_PAR (expression,params,id_expression);
        //if (!p_expression) return false;
	//@@@ TODO!!!
	MapParameter (id, source, dest);
	return true;
      }
    default:
      return false;
  }
  return false;
}

bool celPcWire::ReceiveMessage (csStringID msgid, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  size_t i;
  for (i = 0 ; i < output.GetSize () ; i++)
    output[i]->Do (this, params);
  return true;
}

void celPcWire::AddInput (const char* msg_mask, iMessageChannel* channel)
{
  if (channel == 0)
    channel = entity->QueryMessageChannel ();
  channel->Subscribe (this, msg_mask);
}

size_t celPcWire::AddOutput (const char* msgid, iMessageChannel* channel,
      iCelParameterBlock* extra_params)
{
  csRef<celWireOutputMessage> out;
  if (channel == 0)
    channel = entity->QueryMessageChannel ();
  out.AttachNew (new celWireOutputMessage (msgid, channel));
  out->extra_params = extra_params;
  return output.Push (out);
}

size_t celPcWire::AddOutputAction (csStringID actionID, iCelPropertyClass* pc,
      iCelParameterBlock* extra_params)
{
  csRef<celWireOutputAction> out;
  out.AttachNew (new celWireOutputAction (actionID, pc));
  out->extra_params = extra_params;
  return output.Push (out);
}

void celPcWire::MapParameter (size_t id, const char* source, const char* dest,
      iCelExpression* expression)
{
  output[id]->AddMapping (source, dest, expression);
}

//---------------------------------------------------------------------------

