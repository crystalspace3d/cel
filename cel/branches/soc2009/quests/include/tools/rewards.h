#ifndef __CEL_REWARDS__
#define __CEL_REWARDS__

#include "behaviourlayer/behave.h"

//-------------------------------------------------------------------------
// Rewards
//-------------------------------------------------------------------------

/**
 * This is a reward. You can implement your own rewards in addition 
 * to the standard rewards already implemented. You also need to implement
 * a reward factory then (see iRewardFactory).
 */
struct iReward : public virtual iBase
{
  SCF_INTERFACE (iReward, 0, 0, 1);
  /**
   * Perform this reward.
   */
  virtual void Reward (iCelParameterBlock* params) = 0;
};

/**
 * A factory to create rewards. Reward factories are created
 * by an iRewardType instance.
 */
struct iRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iRewardFactory, 0, 0, 1);

  /**
   * Create a reward.
   * \param quest is the quest for which we are creating this reward.
   * \param params are the parameters with which this reward is
   * instantiated.
   */
  virtual csPtr<iReward> CreateReward () = 0;
	//iQuest* quest,
  	//const celQuestParams& params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<reward\> node in a trigger description.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;
};

/**
 * The reward type is responsible for the creation of reward factories.
 * If you want to define a new type of reward then you must implement
 * iQuestRewardType, iQuestRewardFactory, and iQuestReward.
 * You register reward types with the quest manager so that they can
 * be used by quest factories.
 */
struct iRewardType : public virtual iBase
{
  SCF_INTERFACE (iRewardType, 0, 0, 1);

  /**
   * Return the name for this reward type.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Create a reward factory.
   */
  virtual csPtr<iRewardFactory> CreateRewardFactory () = 0;
};


//-------------------------------------------------------------------------
// Specific reward implementations.
//-------------------------------------------------------------------------

/**
 * This interface is implemented by the reward that prints
 * debug messages on standard output. You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this reward type is 'cel.rewards.debugprint'.
 *
 * In XML, factories recognize the following attributes on the 'reward' node:
 * - <em>message</em>: the message to print.
 */
struct iDebugPrintRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iDebugPrintRewardFactory, 0, 0, 1);

  /**
   * Set the message parameter to print (either a message string
   * or a parameter if it starts with '$').
   */
  virtual void SetMessageParameter (const char* msg) = 0;
};

//-------------------------------------------------------------------------

/**
 * Convenience to declare a new reward type class.
 */
#define CEL_DECLARE_REWARDTYPE_NEW(name,id)					\
class cel##name##RewardType : public scfImplementation2<		\
		cel##name##RewardType,iRewardType, iComponent>			\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  csWeakRef<iCelPlLayer> pl;						\
  cel##name##RewardType (iBase* parent);			\
  virtual ~cel##name##RewardType () { }					\
  virtual bool Initialize (iObjectRegistry*);			\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iRewardFactory> CreateRewardFactory ();		\
};

/**
 * Convenience to implement a new reward type class.
 */
#define CEL_IMPLEMENT_REWARDTYPE_NEW(name)					\
cel##name##RewardType::cel##name##RewardType (				\
	iBase* parent) : scfImplementationType (this, parent),	\
	object_reg(0)											\
{															\
}															\
bool cel##name##RewardType::Initialize (					\
	iObjectRegistry* object_reg)							\
{									\
  cel##name##RewardType::object_reg = object_reg;			\
  pl = csQueryRegistry<iCelPlLayer> (object_reg);			\
  return true;												\
}									\
csPtr<iRewardFactory> cel##name##RewardType::CreateRewardFactory ()\
{									\
  cel##name##RewardFactory* fact = new					\
  	cel##name##RewardFactory (this);				\
  return fact;								\
}

#endif // __CEL_REWARDS__