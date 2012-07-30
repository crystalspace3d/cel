#ifndef __RAT_H
#define __RAT_H

#include "tools/behaviourtree.h"

#include "behave.h"
#include "lifesim.h"

struct iBTNode;

class RatBehaviour : public BehaviourCommon
{
 private:
  csRef<iBTNode> behaviourTree;
  csStringID className;

 public:
  RatBehaviour (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
		    iCelPlLayer* physicalLayer);
  virtual ~RatBehaviour () {}

  void CreateBehaviourTree ();

  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);

  void TickEveryFrame ();
  void TickOnce ();
};

#endif // __RAT_H
