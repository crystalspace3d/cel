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
  return true;
}
void TestNode::AddChild (csRef<iNode> c)
{
  children.Push (c);
}
void TestNode::AttachCondition (csRef<iCondition> cond)
{
  conditions.Push (cond);
}
Skeleton::Animation::iMixingNode* TestNode::GetMixingNode ()
{
  return 0;
}
bool TestNode::SetParameter (const char* name, const celData &param)
{
  return false;
}
void TestNode::Update ()
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
void TestNode::SetName (const char* n)
{
  name = n;
}
const char* TestNode::GetName ()
{
  return name;
}

}
}
