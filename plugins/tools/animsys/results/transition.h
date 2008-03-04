#ifndef __CEL_ANIMATION_TRANVSTITEION_RESULT_H
#define __CEL_ANIMATION_TRANVSTITEION_RESULT_H

#include "csutil/scf_implementation.h"
#include "csutil/weakref.h"
#include "tools/animsys.h"

namespace CEL
{
namespace Animation
{

class TransitionResult : public scfImplementation1<TransitionResult, iResult>
{
public:
  TransitionResult ();
  bool Initialise (iObjectRegistry *objreg, iCelEntity *ent, iPcAnimation *pcanim);
  bool SetParameter (const char* name, const celData &param);
  bool Execute ();
private:
  csString nodename;
  csString state;
  csWeakRef<iPcAnimation> pcanim;
  csWeakRef<iNode> node;
};

class TransitionResultFactory : public scfImplementation1<TransitionResultFactory, iResultFactory>
{
public:
  TransitionResultFactory () : scfImplementationType (this) {}
  csPtr<iResult> CreateNode () const
  {
    return csPtr<iResult> (new TransitionResult);
  }
  const char* GetTypeName () const
  {
    return "transition";
  }
};

}
}

#endif
