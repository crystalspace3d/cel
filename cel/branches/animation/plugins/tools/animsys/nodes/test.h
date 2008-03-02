#ifndef __CEL_ANIMATIONSYSTEM_TEST_NODE_REALLY_LONG_HEADER_GUEARDS_BY_AMIR_H
#define __CEL_ANIMATIONSYSTEM_TEST_NODE_REALLY_LONG_HEADER_GUEARDS_BY_AMIR_H

#include "csutil/refarr.h"
#include "csutil/scf_implementation.h"
#include "physicallayer/pl.h"
#include "tools/animsys.h"

namespace CEL
{
namespace Animation
{

class TestNode : public scfImplementation2<TestNode, iNode, iCelTimerListener>
{
public:
  TestNode ();

  bool Initialise (iObjectRegistry *objreg, iCelEntity *ent);
  void AddChild (csRef<iNode> c);
  Skeleton::Animation::iMixingNode* GetMixingNode ();
  void TickEveryFrame ();
  void TickOnce ();
private:
  csRefArray<iNode> children;
};

class TestNodeFactory : public scfImplementation1<TestNodeFactory, iNodeFactory>
{
public:
  TestNodeFactory () : scfImplementationType (this) {}
  const char* GetTypeName () const
  {
    return "test";
  }
  csPtr<iNode> CreateNode () const
  {
    return csPtr<iNode> (new TestNode);
  }
};

}
}

#endif
