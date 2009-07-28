/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/camera.h"

#include "plugins/tools/triggers/trig_operation.h"

//TEMPORARY
#include "tools/questmanager.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celOperationTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE_NEW(Operation)

//---------------------------------------------------------------------------

celOperationTriggerFactory::celOperationTriggerFactory (
	celOperationTriggerType* type) : scfImplementationType (this)
{
  celOperationTriggerFactory::type = type;
}

celOperationTriggerFactory::~celOperationTriggerFactory ()
{
}

csPtr<iTrigger> celOperationTriggerFactory::CreateTrigger (
    const celParams& params)
{
  csRef<iPluginManager> plugin_mgr = 
    csQueryRegistry<iPluginManager> (type->object_reg);

  csRef<iParameterManager> pm = csLoadPlugin<iParameterManager> 
    (plugin_mgr, "cel.parameters.manager");

  const char* op = pm->ResolveParameter (params, operation_par);

  celOperationTrigger* trig;
  switch (op[0])
  {
     case 'a':  // and
  	trig = new celAndOperationTrigger (type,
  	  params, operation_par, triggers);
        break;
     case 'o':  // or
  	trig = new celOrOperationTrigger (type,
  	  params, operation_par, triggers);
        break;
     case 'x':  // xor
  	trig = new celXorOperationTrigger (type,
  	  params, operation_par, triggers);
        break;
     default:
        csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
          "cel.triggers.operation",
          "'operation' must be one of 'and', 'or' or 'xor'!");
          return 0;
  }
  return trig;
}

bool celOperationTriggerFactory::Load (iDocumentNode* node)
{
  triggers.DeleteAll();
  operation_par = node->GetAttributeValue ("operation");
  if (!operation_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.operation",
      "'operation' attribute is missing for the operation trigger!");
    return false;
  }

  csRef<iDocumentNodeIterator> t_nodes = node->GetNodes("trigger");
  while (t_nodes->HasNext())
  {
    csRef<iDocumentNode> node = t_nodes->Next();

	csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (type->object_reg);
    csRef<iTriggerType> ttype = csLoadPlugin<iTriggerType> (plugin_mgr,
      csString("cel.triggers.")+csString(node->GetAttributeValue("type"))); 

    if (ttype)
    {
      csRef<iTriggerFactory> newfact = ttype->CreateTriggerFactory();
      newfact->Load(node->GetNode("fireon"));
      triggers.Push(newfact);
    }
    else
    {
      csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
          "cel.triggers.operation",
          "%s trigger type does not exist.", node->GetAttributeValue ("type"));
      return false;
    }
  }
  return true;
}

void celOperationTriggerFactory::SetOperationParameter (
	const char* operation)
{
  operation_par = operation;
}

//---------------------------------------------------------------------------

celOperationTrigger::celOperationTrigger (
	celOperationTriggerType* type,
  	const celParams& params,
	const char* operation_par,
	csRefArray<iTriggerFactory> &trigger_factories) 
	: scfImplementationType (this)
{
  checking = false;
  celOperationTrigger::type = type;
  csRef<iPluginManager> plugin_mgr = 
    csQueryRegistry<iPluginManager> (type->object_reg);

  csRef<iParameterManager> pm = csLoadPlugin<iParameterManager> 
    (plugin_mgr, "cel.parameters.manager");

  operation = pm->ResolveParameter (params, operation_par);
  csRefArray<iTriggerFactory>::Iterator iter = trigger_factories.GetIterator();
  while (iter.HasNext())
  {
    csRef<iTrigger> newtrigger = iter.Next()->CreateTrigger(params);
    triggers.Push(newtrigger);
  }
}

celOperationTrigger::~celOperationTrigger ()
{
  DeactivateTrigger ();
}

void celOperationTrigger::RegisterCallback (iTriggerCallback* callback)
{
  celOperationTrigger::callback = callback;
  csRefArray<iTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iter.Next()->RegisterCallback(this);
  }
}

void celOperationTrigger::ClearCallback ()
{
  callback = 0;
  csRefArray<iTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iter.Next()->ClearCallback();
  }
}

void celOperationTrigger::ActivateTrigger ()
{
  csRefArray<iTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iter.Next()->ActivateTrigger();
  }
}

void celOperationTrigger::DeactivateTrigger ()
{
  csRefArray<iTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iter.Next()->DeactivateTrigger();
  }
}

bool celOperationTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celOperationTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

//---------------------------------------------------------------------------
// AND operation

bool celAndOperationTrigger::Check ()
{
  checking = true;
  csRefArray<iTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iTrigger *trig = iter.Next();
    if (!trig->Check())
    {
      checking = false;
      return false;
    }
  }
  DeactivateTrigger ();
  if (callback) callback->TriggerFired ((iTrigger*)this, 0);
  checking = false;
  return true;
}

void celAndOperationTrigger::TriggerFired (iTrigger* trigger, iCelParameterBlock* params)
{
  if (checking) return;
  checking = true;
  csRefArray<iTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iTrigger *trig = iter.Next();
    if (trigger != trig)
    {
      if (!trig->Check())  // exit as soon as one is false
      {
	checking = false;
        return;
      }
    }
  }
  DeactivateTrigger ();
  if (callback) callback->TriggerFired ((iTrigger*)this, 0);
  checking = false;
}

//---------------------------------------------------------------------------
// OR operation

bool celOrOperationTrigger::Check ()
{
  checking = true;
  csRefArray<iTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iTrigger *trig = iter.Next();
    if (trig->Check())  // true once a trigger is true
    {
      DeactivateTrigger ();
      if (callback) callback->TriggerFired ((iTrigger*)this, 0);
      checking = false;
      return true;
    }
  }
  checking = false;
  return false;
}

void celOrOperationTrigger::TriggerFired (iTrigger* trigger, iCelParameterBlock* params)
{
  if (checking) return;
  // one true trigger is enough, so just do the callback immediately
  if (callback)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iTrigger*)this, 0);
  }
}

//---------------------------------------------------------------------------
// XOR operation

bool celXorOperationTrigger::Check ()
{
  checking = true;
  int ntrue = 0;
  csRefArray<iTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iTrigger *trig = iter.Next();
    if (trig->Check())
    {
      ntrue++;
      if (ntrue > 1) // xor defines only one must be true.
      {
	checking = false;
        return false;
      }
    }
  }
  DeactivateTrigger ();
  if (callback) callback->TriggerFired ((iTrigger*)this, 0);
  checking = false;
  return ntrue != 0;  // must be 0 or 1 as we're exiting above for more than 2.
}

void celXorOperationTrigger::TriggerFired (iTrigger* trigger, iCelParameterBlock* params)
{
  if (checking) return;
  checking = true;
  csRefArray<iTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iTrigger *trig = iter.Next();
    if (trigger != trig)
    {
      if (trig->Check()) // exit as soon as one is true
      {
	checking = false;
        return;
      }
    }
  }
  if (callback) callback->TriggerFired ((iTrigger*)this, params);
  checking = false;
}

