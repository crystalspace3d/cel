#ifndef __CEL_ANIMATIONSYSTEM_SWITCHER_NODE_H
#define __CEL_ANIMATIONSYSTEM_SWITCHER_NODE_H

#include "csutil/refarr.h"
#include "csutil/scf_implementation.h"
#include "physicallayer/pl.h"
#include "tools/animsys.h"

struct iVirtualClock;

namespace CEL
{
namespace Animation
{

class SwitcherNode : public scfImplementation1<SwitcherNode, iNode>
{
public:
  SwitcherNode ();

  bool Initialise (iObjectRegistry *objreg, iCelEntity *ent, csRef<Skeleton::iSkeleton> skel);
  void AddChild (csRef<iNode> c);
  void AttachCondition (csRef<iCondition> cond);
  Skeleton::Animation::iMixingNode* GetMixingNode ();
  bool SetParameter (const char* name, const celData &param);
  void Update ();
  void SetName (const char* n);
  const char* GetName ();
private:
  csRefArray<iNode> children;
  csRef<iNode> active_child, deact_child;
  size_t actid, deactid;
  csRefArray<iCondition> conditions;
  csString name;

  csRef<Skeleton::Animation::iBlendNode> blender;

  csRef<iVirtualClock> vc;
  bool setup_for_next;
  size_t lasttime;
  float crosstime, currdur;
  bool in_crossing;
  csString currstate, nextstate;
};

class SwitcherNodeFactory : public scfImplementation1<SwitcherNodeFactory, iNodeFactory>
{
public:
  SwitcherNodeFactory () : scfImplementationType (this) {}
  const char* GetTypeName () const
  {
    return "switcher";
  }
  csPtr<iNode> CreateNode () const
  {
    return csPtr<iNode> (new SwitcherNode);
  }
};

}
}

#endif
