#include "cssysdef.h"
#include "test.h"
#include "imesh/nskeleton.h"
#include "imesh/skelanim.h"
#include "iutil/objreg.h"
#include "ivaria/reporter.h"

namespace CEL
{
namespace Animation
{

TestNode::TestNode ()
  : scfImplementationType (this)
{
}
bool TestNode::Initialise (iObjectRegistry *objreg, iCelEntity *ent, csRef<Skeleton::iSkeleton> skel)
{
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (objreg);
  if (!pl)
  {
    csReport (objreg, CS_REPORTER_SEVERITY_ERROR,
      "cel.animation.system",
      "Missing Physical Layer!");
    return false;
  }
  // Tick every so often so we can update the state
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  csRef<Skeleton::Animation::iAnimationFactoryLayer> animfactlay = skel->GetFactory ()->GetAnimationFactoryLayer ();
  csRef<Skeleton::Animation::iAnimationLayer> animlay = skel->GetAnimationLayer ();
  return true;
}
void TestNode::AddChild (csRef<iNode> c)
{
  children.Push (c);
}
Skeleton::Animation::iMixingNode* TestNode::GetMixingNode ()
{
  return 0;
}
bool TestNode::SetParameter (const char* name, const celData &param)
{
  return true;
}
void TestNode::TickEveryFrame ()
{
  // do nothing
  //puts ("b");
}
void TestNode::TickOnce ()
{
}

}
}
