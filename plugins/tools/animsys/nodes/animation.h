#ifndef __CEL_ANIMATIONSYSTEM_ANIMATION_NODE__H
#define __CEL_ANIMATIONSYSTEM_ANIMATION_NODE__H

#include "csutil/refarr.h"
#include "csutil/scf_implementation.h"
#include "physicallayer/pl.h"
#include "tools/animsys.h"

namespace CEL
{
namespace Animation
{

class AnimationNode : public scfImplementation1<AnimationNode, iNode>
{
public:
  AnimationNode ();

  bool Initialise (iObjectRegistry *objreg, iCelEntity *ent, csRef<Skeleton::iSkeleton> skel);
  void AddChild (csRef<iNode> c);
  void AttachCondition (csRef<iCondition> cond);
  Skeleton::Animation::iMixingNode* GetMixingNode ();
  bool SetParameter (const char* name, const celData &param);
  void Update ();
  void SetName (const char* n);
  const char* GetName ();
private:
  csRefArray<iCondition> conditions;
  csString animname;
  int playcount;
  float playspeed;
  csRef<Skeleton::Animation::iAnimation> anim;
  csString name;
  //bool resetqud;
  bool force_update;
};

class AnimationNodeFactory : public scfImplementation1<AnimationNodeFactory, iNodeFactory>
{
public:
  AnimationNodeFactory () : scfImplementationType (this) {}
  const char* GetTypeName () const
  {
    return "animation";
  }
  csPtr<iNode> CreateNode () const
  {
    return csPtr<iNode> (new AnimationNode);
  }
};

}
}

#endif
