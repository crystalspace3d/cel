#ifndef __SHEEP_H
#define __SHEEP_H

#include "tools/behaviourtree.h"

#include "behave.h"
#include "lifesim.h"

struct iBTNode;

class SheepBehaviour : public BehaviourCommon
{
 private:
  csRef<iBTNode> behaviourTree;
  
  csRef<iTrigger> trigger;
  
  //list of sheep entities
  csRef<iCelEntityList> frankie_entities;
  //list of ram entities
  csRef<iCelEntityList> ram_entities; 
  
  csStringID className;
  csStringID id_pclinearmovement_stuck;
  csStringID id_pctrigger_entityenters;
  csStringID id_pctrigger_entityleaves;
  csStringID id_pctrigger_entertrigger;
  csStringID id_pctrigger_leavetrigger;
  csStringID id_pcsteer_arrived;
  
  //position this entity wants to head to or to escape from
  const char* position;
  const char* resetTrigger;

 public:
  SheepBehaviour (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
		    iCelPlLayer* physicalLayer);
  virtual ~SheepBehaviour () {}

  void CreateBehaviourTree ();

  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);

  void TickEveryFrame ();
  void TickOnce ();
};

#endif // __SHEEP_H
