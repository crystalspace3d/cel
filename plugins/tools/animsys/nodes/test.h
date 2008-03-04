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

class TestNode : public scfImplementation1<TestNode, iNode>
{
public:
  TestNode ();

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
  csRefArray<iCondition> conditions;
  csString name;
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
