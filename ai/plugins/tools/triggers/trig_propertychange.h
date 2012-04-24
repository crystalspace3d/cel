/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin
  
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

#ifndef __CEL_TOOLS_TRIG_PROPERTYCHANGE__
#define __CEL_TOOLS_TRIG_PROPERTYCHANGE__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/triggers.h"
#include "propclass/prop.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers when a property changes
 * This trigger type listens to the name 'cel.triggers.propertychange'.
 */
CEL_DECLARE_TRIGGERTYPE(PropertyChange,"cel.triggers.propertychange")

/**
 * The 'propertychange' trigger factory.
 */
class celPropertyChangeTriggerFactory : public scfImplementation2<
	celPropertyChangeTriggerFactory, iTriggerFactory,
	iPropertyChangeTriggerFactory>
{
private:
  csRef<celPropertyChangeTriggerType> type;
  csString entity_par;
  csString tag_par;
  csString prop_par;
  const char* value_par;   // Do not change this to csString!
  csString op_par;
  bool onchange_par;

public:
  celPropertyChangeTriggerFactory (celPropertyChangeTriggerType* type);
  virtual ~celPropertyChangeTriggerFactory ();

  virtual csPtr<iTrigger> CreateTrigger (iQuest* q, iCelParameterBlock* params);
  virtual iTriggerType* GetTriggerType () const { return type; }
  virtual bool Load (iDocumentNode* node);
  virtual bool Save (iDocumentNode* node);

  //----------------- iPropertyChangeQuestTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual const char* GetEntity () const { return entity_par; }
  virtual const char* GetTag () const { return tag_par; }
  virtual void SetPropertyParameter (const char* prop);
  virtual const char* GetProperty () const { return prop_par; }
  virtual void SetValueParameter (const char* value);
  virtual const char* GetValue () const { return value_par; }
  virtual void SetOperationParameter (const char* op);
  virtual const char* GetOperation () const { return op_par; }
  virtual void SetOnChangeOnly (bool on_change) { onchange_par = on_change; }
  virtual bool IsOnChangeOnly () const { return onchange_par; }
};

/**
 * The 'propertychange' trigger.
 */
class celPropertyChangeTrigger : public scfImplementation2<
	celPropertyChangeTrigger, iTrigger,
	iPcPropertyListener>
{
private:
  csRef<iTriggerCallback> callback;
  csWeakRef<iPcProperties> properties;
  csString entity;
  uint entityID;
  csString tag;
  csString prop;
  const char* value; // Do not change this to csString!
  bool on_change;
  bool is_true;
  bool on_condition;

  void FindProperties ();
  bool TestProperty (size_t idx);
  bool CheckValue ();
protected:
  celPropertyChangeTriggerType* type;
  virtual bool DoTest(long test_value, long actual_value)
  { return (test_value == actual_value); }
  virtual bool DoTest(float test_value, float actual_value)
  { return (test_value == actual_value); }
  virtual bool DoTest(bool test_value, bool actual_value)
  { return (test_value == actual_value); }
  virtual bool DoTest(const char *test_value, const char *actual_value)
  { return (!strcmp (test_value, actual_value)); }
public:
  celPropertyChangeTrigger (celPropertyChangeTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par, const char* prop_par,
	const char* value_par, bool on_change);
  virtual ~celPropertyChangeTrigger ();

  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();

  virtual void Activate () { ActivateTrigger (); }
  virtual void Deactivate () { DeactivateTrigger (); }

  //----------------------- iPcPropertyListener ----------------------------
  virtual void PropertyChanged (iPcProperties* pcprop, size_t idx);
};

class celPropertyChangeTriggerLt : public celPropertyChangeTrigger
{
public:
  celPropertyChangeTriggerLt (celPropertyChangeTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par, const char* prop_par,
	const char* value_par, bool on_change)
  : celPropertyChangeTrigger(type, params, entity_par, tag_par, prop_par,
				value_par, on_change)
  {} ;
  // tests
  virtual bool DoTest(long test_value, long actual_value)
  { return (actual_value<test_value); }
  virtual bool DoTest(float test_value, float actual_value)
  { return (actual_value<test_value); }
  virtual bool DoTest(bool test_value, bool actual_value)
  { printf("Lt test not for bool properties\n"); return false; }
  virtual bool DoTest(const char *test_value, const char *actual_value)
  { printf("Lt test not for string properties\n"); return false; }

};

class celPropertyChangeTriggerGt : public celPropertyChangeTrigger
{
public:
  celPropertyChangeTriggerGt (celPropertyChangeTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par, const char* prop_par,
	const char* value_par, bool on_change)
  : celPropertyChangeTrigger(type, params, entity_par, tag_par, prop_par,
				value_par, on_change)
  {} ;
  // tests
  virtual bool DoTest(long test_value, long actual_value)
  { return (actual_value>test_value); }
  virtual bool DoTest(float test_value, float actual_value)
  { return (actual_value>test_value); }
  virtual bool DoTest(bool test_value, bool actual_value)
  { printf("Gt test not for bool properties\n"); return false; }
  virtual bool DoTest(const char *test_value, const char *actual_value)
  { printf("Gt test not for string properties\n"); return false; }

};

class celPropertyChangeTriggerNe : public celPropertyChangeTrigger
{
public:
  celPropertyChangeTriggerNe (celPropertyChangeTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par, const char* prop_par,
	const char* value_par, bool on_change)
  : celPropertyChangeTrigger(type, params, entity_par, tag_par, prop_par,
				value_par, on_change)
  {} ;
  // tests
  virtual bool DoTest(long test_value, long actual_value)
  { return (actual_value != test_value); }
  virtual bool DoTest(float test_value, float actual_value)
  { return (actual_value != test_value); }
  virtual bool DoTest(bool test_value, bool actual_value)
  { return (test_value != actual_value); }
  virtual bool DoTest(const char *test_value, const char *actual_value)
  { return (strcmp (test_value, actual_value) != 0); }
};

class celPropertyChangeTriggerLe : public celPropertyChangeTrigger
{
public:
  celPropertyChangeTriggerLe (celPropertyChangeTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par, const char* prop_par,
	const char* value_par, bool on_change)
  : celPropertyChangeTrigger(type, params, entity_par, tag_par, prop_par,
				value_par, on_change)
  {} ;
  // tests
  virtual bool DoTest(long test_value, long actual_value)
  { return (actual_value<=test_value); }
  virtual bool DoTest(float test_value, float actual_value)
  { return (actual_value<=test_value); }
  virtual bool DoTest(bool test_value, bool actual_value)
  { printf("Le test not for bool properties\n"); return false; }
  virtual bool DoTest(const char *test_value, const char *actual_value)
  { printf("Le test not for string properties\n"); return false; }
};

class celPropertyChangeTriggerGe : public celPropertyChangeTrigger
{
public:
  celPropertyChangeTriggerGe (celPropertyChangeTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par, const char* prop_par,
	const char* value_par, bool on_change)
  : celPropertyChangeTrigger(type, params, entity_par, tag_par, prop_par,
				value_par, on_change)
  {} ;
  // tests
  virtual bool DoTest(long test_value, long actual_value)
  { return (actual_value>=test_value); }
  virtual bool DoTest(float test_value, float actual_value)
  { return (actual_value>=test_value); }
  virtual bool DoTest(bool test_value, bool actual_value)
  { printf("Ge test not for bool properties\n"); return false; }
  virtual bool DoTest(const char *test_value, const char *actual_value)
  { printf("Ge test not for string properties\n"); return false; }
};

#endif // __CEL_TOOLS_TRIG_PROPERTYCHANGE__

