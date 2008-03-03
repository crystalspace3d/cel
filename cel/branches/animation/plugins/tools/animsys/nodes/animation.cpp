#include "cssysdef.h"
#include "animation.h"
#include "imesh/nskeleton.h"
#include "imesh/skelanim.h"
#include "iutil/objreg.h"
#include "ivaria/reporter.h"

namespace CEL
{
namespace Animation
{

AnimationNode::AnimationNode ()
  : scfImplementationType (this), playcount (1), playspeed (1.0f)
{
}
bool AnimationNode::Initialise (iObjectRegistry *objreg, iCelEntity *ent, csRef<Skeleton::iSkeleton> skel)
{
  csRef<Skeleton::Animation::iAnimationFactoryLayer> animfactlay = skel->GetFactory ()->GetAnimationFactoryLayer ();
  csRef<Skeleton::Animation::iAnimationLayer> animlay = skel->GetAnimationLayer ();

  if (!animname)
  {
    csReport (objreg, CS_REPORTER_SEVERITY_WARNING, "cel.animation.system",
      "Problem finding animation name parameter!");
    return false;
  }
  csRef<Skeleton::Animation::iAnimationFactory> animfact = animfactlay->FindAnimationFactoryByName (animname);
  if (!animfact)
  {
    csReport (objreg, CS_REPORTER_SEVERITY_WARNING, "cel.animation.system",
      "Non existant animation '%s'!", animname.GetData ());
    return false;
  }
  anim = animfact->CreateAnimation ();
  if (!anim)
  {
    csReport (objreg, CS_REPORTER_SEVERITY_WARNING, "cel.animation.system",
      "Internal error creating animation from factory '%s'!", animname.GetData ());
    return false;
  }
  anim->SetPlayCount (playcount);
  anim->SetPlaySpeed (playspeed);
  return true;
}
void AnimationNode::AddChild (csRef<iNode> c)
{
  children.Push (c);
}
void AnimationNode::AttachCondition (csRef<iCondition> cond)
{
  conditions.Push (cond);
}
Skeleton::Animation::iMixingNode* AnimationNode::GetMixingNode ()
{
  return anim;
}
bool AnimationNode::SetParameter (const char* name, const celData &param)
{
  if (!strcmp (name, "animation") && param.type == CEL_DATA_STRING)
  {
    animname = param.value.s->GetData ();
  }
  else if (!strcmp (name, "loop") && param.type == CEL_DATA_BOOL)
  {
    if (param.value.bo == true)
      playcount = -1;
  }
  else if (!strcmp (name, "playcount") && param.type == CEL_DATA_LONG)
  {
    playcount = param.value.l;
  }
  else if (!strcmp (name, "playspeed") && param.type == CEL_DATA_FLOAT)
  {
    playspeed = param.value.f;
  }
  return false;
}
void AnimationNode::Update ()
{
  for (csRefArray<iNode>::Iterator it = children.GetIterator (); it.HasNext (); )
  {
    iNode* c = it.Next ();
    c->Update ();
  }
  for (csRefArray<iCondition>::Iterator it = conditions.GetIterator (); it.HasNext (); )
  {
    iCondition* c = it.Next ();
    c->Evaluate ();
  }
}
void AnimationNode::SetName (const char* n)
{
  name = n;
}
const char* AnimationNode::GetName ()
{
  return name;
}

}
}
