#ifndef __RAM_H
#define __RAM_H

#include "tools/behaviourtree.h"

#include "behave.h"
#include "lifesim.h"

struct iBTNode;

class RamBehaviour : public BehaviourCommon
{
 private:
  csRef<iBTNode> behaviourTree;
  csStringID className;
  csRef<iCelEntityList> entities;
  
  bool dead;
  
  csStringID id_pctrigger_entityenters;
  csStringID id_pctrigger_entityleaves;
  csStringID id_pctrigger_entertrigger;
  csStringID id_pctrigger_leavetrigger;
  csStringID id_pcmover_arrived;
  
 public:
  RamBehaviour (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
		    iCelPlLayer* physicalLayer);
  virtual ~RamBehaviour () {}

  void CreateBehaviourTree ();

  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);

  void TickEveryFrame ();
  void TickOnce ();
};

#endif // __RAM_H
