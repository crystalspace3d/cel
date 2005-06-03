/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_QUESTS_TRIG_PROPERTYCHANGE__
#define __CEL_TOOLS_QUESTS_TRIG_PROPERTYCHANGE__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"
#include "propclass/prop.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever the camera
 * enters a specific sector.
 * This trigger type listens to the name 'cel.questtrigger.propertychange'.
 */
CEL_DECLARE_TRIGGERTYPE(PropertyChange,"cel.questtrigger.propertychange")

/**
 * The 'propertychange' trigger factory.
 */
class celPropertyChangeTriggerFactory :
	public iQuestTriggerFactory,
	public iPropertyChangeQuestTriggerFactory
{
private:
  celPropertyChangeTriggerType* type;
  char* entity_par;
  char* tag_par;
  char* prop_par;
  char* value_par;

public:
  celPropertyChangeTriggerFactory (celPropertyChangeTriggerType* type);
  virtual ~celPropertyChangeTriggerFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestTrigger> CreateTrigger (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iPropertyChangeQuestTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetPropertyParameter (const char* prop);
  virtual void SetValueParameter (const char* value);
};

/**
 * The 'propertychange' trigger.
 */
class celPropertyChangeTrigger :
	public iQuestTrigger,
	public iPcPropertyListener
{
private:
  celPropertyChangeTriggerType* type;
  csRef<iQuestTriggerCallback> callback;
  csWeakRef<iPcProperties> properties;
  char* entity;
  char* tag;
  char* prop;
  char* value;

  void FindProperties ();
  bool TestProperty (size_t idx);

public:
  celPropertyChangeTrigger (celPropertyChangeTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* prop_par, const char* value_par);
  virtual ~celPropertyChangeTrigger ();

  SCF_DECLARE_IBASE;

  virtual void RegisterCallback (iQuestTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  //----------------------- iPcPropertyListener ----------------------------
  virtual void PropertyChanged (iPcProperties* pcprop, size_t idx);
};

#endif // __CEL_TOOLS_QUESTS_TRIG_PROPERTYCHANGE__

