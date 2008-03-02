#include "cssysdef.h"
#include "test.h"
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
bool TestNode::Initialise (iObjectRegistry *objreg, iCelEntity *ent)
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
