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

#ifndef __CEL_BLTEST_BEHAVE__
#define __CEL_BLTEST_BEHAVE__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/util.h"
#include "csutil/weakref.h"
#include "behaviourlayer/behave.h"
#include "physicallayer/messaging.h"
#include "physicallayer/pl.h"
#include "propclass/meshdeform.h"
#include "propclass/analogmotion.h"
#include "propclass/jump.h"
#include "propclass/grab.h"
#include "propclass/linmove.h"
#include "propclass/cameras/tracking.h"

struct iCelEntity;
struct iObjectRegistry;
struct iCelPlLayer;
struct iPcMeshDeform;

/**
 * General behaviour class.
 */
class celBehaviourGeneral : public scfImplementation2<
	celBehaviourGeneral, iCelBehaviour, iMessageReceiver>
{
protected:
  iCelEntity* entity;
  iCelBlLayer* bl;
  iObjectRegistry* object_reg;
  csWeakRef<iCelPlLayer> pl;
  char* name;

public:
  celBehaviourGeneral (iCelEntity* entity, iObjectRegistry* object_reg);
  virtual ~celBehaviourGeneral ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  void SetBehaviourLayer (iCelBlLayer* bl)
  {
    celBehaviourGeneral::bl = bl;
  }

  virtual const char* GetName () const { return name; }
  virtual iCelBlLayer* GetBehaviourLayer () const { return bl; }
  virtual bool SendMessage (const char* msg_id,
  	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* msg_info, ...) { return false; }
  virtual bool SendMessageV (const char* msg_id,
  	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* msg_info, va_list arg) { return false; }
  virtual void* GetInternalObject () { return 0; }

  virtual bool ReceiveMessage (csStringID /*msg_id*/, iMessageSender* /*sender*/,
      celData& /*ret*/, iCelParameterBlock* /*params*/) { return false; }
};

/**
 * 'printer' behaviour.
 */
class celBehaviourPrinter : public celBehaviourGeneral
{
public:
  celBehaviourPrinter (iCelEntity* entity, iObjectRegistry* object_reg);
};

/**
 * 'box' behaviour.
 */
class celBehaviourBox : public celBehaviourGeneral
{
private:
  csStringID msgid_meshsel_down;
  csStringID msgid_pctimer_wakeup;
  csStringID id_param_entity;

public:
  celBehaviourBox (iCelEntity* entity, iObjectRegistry* object_reg);
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
};

/**
 * 'room' behaviour.
 */
class celBehaviourRoom : public celBehaviourGeneral
{
public:
  celBehaviourRoom (iCelEntity* entity, iObjectRegistry* object_reg);
};

/**
 * 'quest' behaviour.
 */
class celBehaviourQuest : public celBehaviourGeneral
{
public:
  celBehaviourQuest (iCelEntity* entity, iObjectRegistry* object_reg);
};

#endif // __CEL_BLTEST_BEHAVE__

