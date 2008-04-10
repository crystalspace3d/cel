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
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/camera.h"

#include "plugins/tools/quests/trig_operation.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(Operation)

//---------------------------------------------------------------------------

celOperationTriggerFactory::celOperationTriggerFactory (
	celOperationTriggerType* type) : scfImplementationType (this)
{
  celOperationTriggerFactory::type = type;
  operation_par = 0;
}

celOperationTriggerFactory::~celOperationTriggerFactory ()
{
  delete[] operation_par;
}

csPtr<iQuestTrigger> celOperationTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  const char* op = qm->ResolveParameter (params, operation_par);

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
          "cel.questtrigger.operation",
          "'operation' must be one of 'and', 'or' or 'xor'!");
          return 0;
  }
  return trig;
}

bool celOperationTriggerFactory::Load (iDocumentNode* node)
{
  delete[] operation_par; operation_par = 0;
  triggers.DeleteAll();

  operation_par = csStrNew (node->GetAttributeValue ("operation"));
  if (!operation_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.operation",
      "'operation' attribute is missing for the operation trigger!");
    return false;
  }

  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);

  csRef<iDocumentNodeIterator> t_nodes = node->GetNodes("trigger");
  while (t_nodes->HasNext())
  {
    csRef<iDocumentNode> node = t_nodes->Next();
    iQuestTriggerType *ttype = qm->GetTriggerType(csString("cel.questtrigger.")+csString(node->GetAttributeValue("type")));
    if (ttype)
    {
      csRef<iQuestTriggerFactory> newfact = ttype->CreateTriggerFactory();
      newfact->Load(node->GetNode("fireon"));
      triggers.Push(newfact);
    }
    else
    {
      csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
        "cel.questtrigger.operation",
        csString(node->GetAttributeValue("type")+csString(" trigger type does not exist.")));
      return false;
    }
  }
  return true;
}

void celOperationTriggerFactory::SetOperationParameter (
	const char* operation)
{
  if (operation_par == operation) 
    return;

  delete[] operation_par;
  operation_par = csStrNew (operation);
}

//---------------------------------------------------------------------------

celOperationTrigger::celOperationTrigger (
	celOperationTriggerType* type,
  	const celQuestParams& params,
	const char* operation_par,
	csRefArray<iQuestTriggerFactory> &trigger_factories) 
	: scfImplementationType (this)
{
  celOperationTrigger::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  operation = csStrNew (qm->ResolveParameter (params, operation_par));
  csRefArray<iQuestTriggerFactory>::Iterator iter = trigger_factories.GetIterator();
  while (iter.HasNext())
  {
    csRef<iQuestTrigger> newtrigger = iter.Next()->CreateTrigger(0,params);
    triggers.Push(newtrigger);
  }
}

celOperationTrigger::~celOperationTrigger ()
{
  DeactivateTrigger ();
  delete[] operation;
}

void celOperationTrigger::RegisterCallback (iQuestTriggerCallback* callback)
{
  celOperationTrigger::callback = callback;
  csRefArray<iQuestTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iter.Next()->RegisterCallback(this);
  }
}

void celOperationTrigger::ClearCallback ()
{
  callback = 0;
  csRefArray<iQuestTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iter.Next()->ClearCallback();
  }
}

void celOperationTrigger::ActivateTrigger ()
{
  csRefArray<iQuestTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iter.Next()->ActivateTrigger();
  }
}



void celOperationTrigger::DeactivateTrigger ()
{
  csRefArray<iQuestTrigger>::Iterator iter = triggers.GetIterator();
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
  csRefArray<iQuestTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iQuestTrigger *trig = iter.Next();
    if (!trig->Check())
      return false;
  }
  DeactivateTrigger ();
  if (callback) callback->TriggerFired ((iQuestTrigger*)this, 0);
  return true;
}

void celAndOperationTrigger::TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params)
{
  csRefArray<iQuestTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iQuestTrigger *trig = iter.Next();
    if (trigger != trig)
    {
      if (!trig->Check())  // exit as soon as one is false
        return;
    }
  }
  if (callback) callback->TriggerFired ((iQuestTrigger*)this, 0);
}

//---------------------------------------------------------------------------
// OR operation

bool celOrOperationTrigger::Check ()
{
  csRefArray<iQuestTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iQuestTrigger *trig = iter.Next();
    if (trig->Check())  // true once a trigger is true
    {
      DeactivateTrigger ();
      if (callback) callback->TriggerFired ((iQuestTrigger*)this, 0);
      return true;
    }
  }
  return false;
}

void celOrOperationTrigger::TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params)
{
  // one true trigger is enough, so just do the callback immediately
  if (callback) callback->TriggerFired ((iQuestTrigger*)this, 0);
}

//---------------------------------------------------------------------------
// XOR operation

bool celXorOperationTrigger::Check ()
{
  int ntrue = 0;
  csRefArray<iQuestTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iQuestTrigger *trig = iter.Next();
    if (trig->Check())
    {
      ntrue++;
      if (ntrue > 1) // xor defines only one must be true.
        return false;
    }
  }
  DeactivateTrigger ();
  if (callback) callback->TriggerFired ((iQuestTrigger*)this, 0);
  return ntrue;  // must be 0 or 1 as we're exiting above for more than 2.
}

void celXorOperationTrigger::TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params)
{
  csRefArray<iQuestTrigger>::Iterator iter = triggers.GetIterator();
  while (iter.HasNext())
  {
    iQuestTrigger *trig = iter.Next();
    if (trigger != trig)
    {
      if (trig->Check()) // exit as soon as one is true
        return;
    }
  }
  if (callback) callback->TriggerFired ((iQuestTrigger*)this, params);
}

