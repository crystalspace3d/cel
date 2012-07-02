/*
    Copyright (C) 2012 by Christian Van Brussel

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
#ifndef BEHAVE_H
#define BEHAVE_H

#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iutil/csinput.h"

#include "behaviourlayer/behave.h"
#include "behaviourlayer/bl.h"

class BehaviourLayer : public scfImplementation1<BehaviourLayer, iCelBlLayer>
{
private:
  csRef<iCelPlLayer> physicalLayer;
  csRef<iObjectRegistry> objectRegistry;

public:
  BehaviourLayer (iCelPlLayer* physicalLayer, iObjectRegistry* objectRegistry);
  virtual ~BehaviourLayer ();

  virtual const char* GetName () const;
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);
};

class BehaviourCommon : public scfImplementation1<BehaviourCommon, iCelBehaviour>
{
protected:
  csRef<iCelEntity> entity;
  csRef<BehaviourLayer> behaviourLayer;
  csRef<iCelPlLayer> physicalLayer;

public:
  BehaviourCommon (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl);
  virtual ~BehaviourCommon ();

  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);

  virtual iCelBlLayer* GetBehaviourLayer () const;
  virtual bool SendMessage (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, ...);
  virtual bool SendMessageV (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                             iCelParameterBlock* params, va_list arg);
  virtual void* GetInternalObject ();
};

class BehaviourPlayer : public BehaviourCommon
{
private:
  csStringID id_pccommandinput_forward1;
  csStringID id_pccommandinput_forward0;
  csStringID id_pccommandinput_backward1;
  csStringID id_pccommandinput_backward0;
  csStringID id_pccommandinput_rotateleft1;
  csStringID id_pccommandinput_rotateleft0;
  csStringID id_pccommandinput_rotateright1;
  csStringID id_pccommandinput_rotateright0;
  csStringID id_pccommandinput_cammode1;

  csRef<iObjectRegistry> objectRegistry;
  csRef<iCamera> camera;

public:
  BehaviourPlayer (iCelEntity* entity, BehaviourLayer* behaviourLayer, iCelPlLayer* physicalLayer, 
                   iObjectRegistry* objectRegistry);
  virtual ~BehaviourPlayer () { }

  virtual const char* GetName () const;
  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);
};

#endif
