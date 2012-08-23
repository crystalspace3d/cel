#ifndef __BUTTERFLY_H
#define __BUTTERFLY_H

#include "tools/behaviourtree.h"

#include "behave.h"
#include "lifesim.h"

struct iBTNode;

class ButterFlyBehaviour : public BehaviourCommon
{
 private:
  csRef<iBTNode> behaviourTree;

  csStringID className;
  csStringID id_pctrigger_entityenters;
  csStringID id_pctrigger_entityleaves;
  csStringID id_pctrigger_entertrigger;
  csStringID id_pctrigger_leavetrigger;
  csStringID id_pcmover_arrived;
  
  bool stopBT;

 public:
  ButterFlyBehaviour (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
		    iCelPlLayer* physicalLayer);
  virtual ~ButterFlyBehaviour () {}

  void CreateBehaviourTree ();

  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);

  void TickEveryFrame ();
  void TickOnce ();
};

#endif // __BUTTERFLY_H
