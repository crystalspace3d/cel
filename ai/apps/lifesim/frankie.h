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
#ifndef __FRANKIE_H
#define __FRANKIE_H

#include "tools/behaviourtree.h"

#include "behave.h"
#include "lifesim.h"

struct iBTNode;

class FrankieBehaviour : public BehaviourCommon
{
 private:
  csRef<iBTNode> behaviourTree;

 public:
  FrankieBehaviour (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
		    iCelPlLayer* physicalLayer);
  virtual ~FrankieBehaviour () {}

  void CreateBehaviourTree ();

  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);

  void TickEveryFrame ();
  void TickOnce ();
};

#endif // __FRANKIE_H
