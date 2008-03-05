#ifndef __CEL_ANIMATIONSYSTEM_ACTIONS_NODE_H
#define __CEL_ANIMATIONSYSTEM_ACTIONS_NODE_H

#include "csutil/refarr.h"
#include "csutil/scf_implementation.h"
#include "physicallayer/pl.h"
#include "tools/animsys.h"

struct iVirtualClock;

namespace CEL
{
namespace Animation
{

class ActionsNode : public scfImplementation1<ActionsNode, iNode>
{
public:
  ActionsNode ();

  bool Initialise (iObjectRegistry *objreg, iCelEntity *ent, csRef<Skeleton::iSkeleton> skel);
  void AddChild (csRef<iNode> c);
  void AttachCondition (csRef<iCondition> cond);
  Skeleton::Animation::iMixingNode* GetMixingNode ();
  bool SetParameter (const char* name, const celData &param);
  void Update ();
  void SetName (const char* n);
  const char* GetName ();
private:
  void SetNodeActivation (const csString &nodename, bool active);

  csRefArray<iNode> children, active_children;
  csRefArray<iCondition> conditions;
  csString name;

  csRef<Skeleton::Animation::iAccumulateNode> accum;
  csArray<csTuple2<size_t, float> > mix_steps;
  // stack of actions to perform
  csArray<csString> activate, deactivate, rampon;//, rampoff;

  csRef<iVirtualClock> vc;
  size_t lasttime;
  float mixtime, currdur;
};

class ActionsNodeFactory : public scfImplementation1<ActionsNodeFactory, iNodeFactory>
{
public:
  ActionsNodeFactory () : scfImplementationType (this) {}
  const char* GetTypeName () const
  {
    return "actions";
  }
  csPtr<iNode> CreateNode () const
  {
    return csPtr<iNode> (new ActionsNode);
  }
};

}
}

#endif
