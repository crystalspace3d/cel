/*
  Copyright (C) 2011 Christian Van Brussel, Eutyche Mukuama, Dodzi de Souza
      Communications and Remote
      Sensing Laboratory of the School of Engineering at the 
      Universite catholique de Louvain, Belgium
      http://www.tele.ucl.ac.be

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef __BEHAVE_H
#define __BEHAVE_H

#include "iutil/objreg.h"

#include "physicallayer/pl.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/behave.h"

class BehaviourLayer : public scfImplementation1<BehaviourLayer, iCelBlLayer>
{
private:
  iCelPlLayer* physicalLayer;

public:
  BehaviourLayer (iObjectRegistry* registry, iCelPlLayer* physicalLayer);
  virtual ~BehaviourLayer ();

  virtual const char* GetName () const { return "behaviourlayer"; }
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);

  iObjectRegistry* registry;
};

class BehaviourCommon
: public scfImplementation2<BehaviourCommon,
  iCelBehaviour,
  iCelTimerListener>
{
protected:
  csString name;  
  csRef<iCelEntity> entity;
  csRef<BehaviourLayer> behaviourLayer;
  csRef<iCelPlLayer> physicalLayer;

public:
  BehaviourCommon (const char* name, iCelEntity* entity,
		   BehaviourLayer* behaviourLayer, iCelPlLayer* physicalLayer);
  virtual ~BehaviourCommon ();

  virtual void CreateBehaviourTree () = 0;

  /**
   * This is a specific version of SendMessage() that accepts
   * an integer instead of a csStringID. Subclasses of ccBehaviourBase
   * will only have to override this version. The normal iCelBehaviour
   * versions of SendMessage() are implemented in this class and
   * will redirect to this version.
   */
  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);

  virtual iCelBlLayer* GetBehaviourLayer () const;
  virtual bool SendMessage (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, ...);
  virtual bool SendMessageV (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                             iCelParameterBlock* params, va_list arg);
  virtual void* GetInternalObject ();

  virtual const char* GetName () const { return name.GetData (); }

  //-- iCelTimerListener
  virtual void TickEveryFrame () = 0;
  virtual void TickOnce () = 0;
};

#endif // __BEHAVE_H
