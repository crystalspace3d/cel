/*
    Copyright (C) 2006 by Jorrit Tyberghein

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

#ifndef __XD_BE_SHIP__
#define __XD_BE_SHIP__

// CS Includes
#include "csutil/weakref.h"
#include "csutil/scf.h"

// CEL Includes
#include "behaviourlayer/behave.h"
#include "propclass/craft.h"
#include "propclass/mechsys.h"
#include "propclass/defcam.h"

struct iCelBlLayer;
struct iCelEntity;

class htBehaviourActor : public scfImplementation1<htBehaviourActor,
  iCelBehaviour>
{
private:
  iCelBlLayer* bl;
  iCelEntity* entity;

  csWeakRef<iPcMechanicsObject> pcmechobj;
  csWeakRef<iPcCraftController> pccraft;
  csWeakRef<iPcDefaultCamera> pcdefcamera;

  iPcMechanicsObject* GetMechObject ();
  iPcCraftController* GetCraft ();
  iPcDefaultCamera* GetCamera ();

public:
  htBehaviourActor(iCelBlLayer *bl, iCelEntity *entity);
  virtual ~htBehaviourActor();

  virtual iCelBlLayer* GetBehaviourLayer () const { return bl; }
  virtual void* GetInternalObject () { return 0; }
  virtual const char* GetName () const { return "hoverobj"; }
  virtual bool SendMessage (const char *msg_id, iCelPropertyClass *pc,
      celData &ret, iCelParameterBlock *params,...);
  virtual bool SendMessageV (const char *msg_id, iCelPropertyClass *pc,
      celData &ret, iCelParameterBlock *params, va_list arg);
};

#endif
