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
#include "iutil/plugin.h"
#include "plugins/propclass/wire/wirefact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (Wire, "pclogic.wire")

//---------------------------------------------------------------------------

csRef<iCelParameterBlock> celWireOutput::CombineParams (iCelParameterBlock* p1, iCelParameterBlock* p2)
{
  if (!p1) return p2;
  if (!p2) return p1;
  csRef<iCelParameterBlock> b;
  b.AttachNew (new celCombineParameterBlock (p1, p2));
  return b;
}

static iCelPlLayer* global_pl;

csRef<iCelParameterBlock> celWireOutput::MapParams (iCelEntity* entity, iCelParameterBlock* params)
{
  if (mappings.GetSize () == 0)
    return params;
  csRef<celMappedParameterBlock> newparams;
  newparams.AttachNew (new celMappedParameterBlock (entity, params, mappings));
  //for (size_t i = 0 ; i < newparams->GetParameterCount () ; i++)
  //{
    //celDataType t;
    //csStringID id = newparams->GetParameterDef (i, t);
    //const celData* data = newparams->GetParameterByIndex (i);
    //printf ("%d %d %s = '%s'\n", i, (size_t)id, global_pl->FetchString (id), data->value.s);
  //}
  return newparams;
}

void celWireOutput::AddMapping (csStringID source, csStringID dest,
    iCelExpression* expression)
{
  celParameterMapping mapping;
  mapping.source = source;
  mapping.dest = dest;
  mapping.expression = expression;
  mappings.Push (mapping);
}

void celWireOutput::Do (celPcWire* wire, iCelParameterBlock* params)
{
  if (channel)
  {
    csRef<iCelParameterBlock> block = CombineParams (extra_params, params);
#if 0
  printf ("    msgid=%s\n", (const char*)msgid);
  if (block)
  for (size_t i = 0 ; i < block->GetParameterCount () ; i++)
  {
    celDataType t;
    csStringID id = block->GetParameterDef (i, t);
    const celData* data = block->GetParameterByIndex (i);
    if (!data)
      printf ("    %d: null\n", i);
    else if (data->type == CEL_DATA_STRING)
      printf ("    %d %d %s = '%s'\n", i, (size_t)id, global_pl->FetchString (id), data->value.s ? data->value.s->GetData () : 0);
    else
      printf ("    %d %d %s\n", i, (size_t)id, global_pl->FetchString (id));
  }
#endif
    channel->SendMessage (msgid, wire, MapParams (wire->GetEntity (), block));
  }
}

//---------------------------------------------------------------------------

csStringID celPcWire::id_mask = csInvalidStringID;
csStringID celPcWire::id_entity = csInvalidStringID;
csStringID celPcWire::id_msgid = csInvalidStringID;
csStringID celPcWire::id_id = csInvalidStringID;
csStringID celPcWire::id_source = csInvalidStringID;
csStringID celPcWire::id_dest = csInvalidStringID;
csStringID celPcWire::id_expression = csInvalidStringID;

PropertyHolder celPcWire::propinfo;

celPcWire::celPcWire (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  global_pl = pl;
  // For SendMessage parameters.
  if (id_mask == csInvalidStringID)
  {
    id_mask = pl->FetchStringID ("mask");
    id_entity = pl->FetchStringID ("entity");
    id_msgid = pl->FetchStringID ("msgid");
    id_id = pl->FetchStringID ("id");
    id_source = pl->FetchStringID ("source");
    id_dest = pl->FetchStringID ("dest");
    id_expression = pl->FetchStringID ("expression");
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.wire.action.");
    AddAction (action_addinput, "AddInput");
    AddAction (action_addoutput, "AddOutput");
    AddAction (action_mapparameter, "MapParameter");
  }
}

celPcWire::~celPcWire ()
{
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
	{
	  ent = pl->FindEntity (entity);
	  if (!ent)
	  {
	    printf ("Can't find entity '%s'!\n", entity);
	    return false;
	  }
	}
	AddOutput (pl->FetchStringID (msgid), ent->QueryMessageChannel (), params);
        return true;
      }
    case action_mapparameter:
      {
        CEL_FETCH_LONG_PAR (id,params,id_id);
        if (!p_id) return false;
        CEL_FETCH_STRING_PAR (dest,params,id_dest);
        if (!p_dest) return false;
        CEL_FETCH_STRING_PAR (expression,params,id_expression);
        if (p_expression)
	{
	  MapParameterExpression (id, dest, expression);
	}
	else
	{
          CEL_FETCH_STRING_PAR (source,params,id_source);
          if (!p_source) return false;
	  MapParameter (id, source, dest);
	}
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
  if (celPcCommon::ReceiveMessage (msgid, sender, ret, params))
    return true;
  for (size_t i = 0 ; i < output.GetSize () ; i++)
    output[i]->Do (this, params);
  return true;
}

void celPcWire::AddInput (const char* msg_mask, iMessageChannel* channel)
{
  if (channel == 0)
    channel = entity->QueryMessageChannel ();
  channel->Subscribe (this, msg_mask);
}

size_t celPcWire::AddOutput (csStringID msgid, iMessageChannel* channel,
      iCelParameterBlock* extra_params)
{
  csRef<celWireOutput> out;
  if (channel == 0)
    channel = entity->QueryMessageChannel ();
  out.AttachNew (new celWireOutput (msgid, channel));
  out->extra_params = extra_params;
  return output.Push (out);
}

void celPcWire::MapParameter (size_t id, const char* source, const char* dest,
      iCelExpression* expression)
{
  output[id]->AddMapping (pl->FetchStringID (source), pl->FetchStringID (dest), expression);
}

static iCelExpressionParser* GetParser (iObjectRegistry* object_reg)
{
  csRef<iObjectRegistryIterator> it = object_reg->Get (
      scfInterfaceTraits<iCelExpressionParser>::GetID (),
      scfInterfaceTraits<iCelExpressionParser>::GetVersion ());
  iBase* b = it->Next ();
  csRef<iCelExpressionParser> parser;
  if (b)
  {
    parser = scfQueryInterface<iCelExpressionParser> (b);
  }
  if (!parser)
  {
    csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (
    	object_reg);
    parser = csLoadPlugin<iCelExpressionParser> (plugmgr,
      "cel.behaviourlayer.xml");
    if (!parser)
    {
      // @@@ Error report.
      return 0;
    }
    object_reg->Register (parser, "iCelExpressionParser");
  }
  return parser;
}

void celPcWire::MapParameterExpression (size_t id, const char* dest, const char* expression)
{
  iCelExpressionParser* parser = GetParser (object_reg);
  // @@@ Error?
  if (parser)
  {
    csRef<iCelExpression> exp = parser->Parse (expression);
    // @@@ Error?
    if (exp)
      output[id]->AddMapping (csInvalidStringID, pl->FetchStringID (dest), exp);
  }
}

//---------------------------------------------------------------------------

