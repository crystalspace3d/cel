#ifndef __CEL_ANIMATION_PLAY_ANIMATION_RESULT_H
#define __CEL_ANIMATION_PLAY_ANIMATION_RESULT_H

#include "csutil/scf_implementation.h"
#include "csutil/weakref.h"
#include "tools/animsys.h"

namespace CEL
{
namespace Animation
{

class PlayAnimationResult : public scfImplementation1<PlayAnimationResult, iResult>
{
public:
  PlayAnimationResult ();
  bool Initialise (iObjectRegistry *objreg, iCelEntity *ent, iPcAnimation *pcanim);
  bool SetParameter (const char* name, const celData &param);
  bool Execute ();
private:
  csString nodename;
  csWeakRef<iPcAnimation> pcanim;
  csWeakRef<iNode> node;

  int playcount;
  float playspeed;
  bool reset;
};

class PlayAnimationResultFactory : public scfImplementation1<PlayAnimationResultFactory, iResultFactory>
{
public:
  PlayAnimationResultFactory () : scfImplementationType (this) {}
  csPtr<iResult> CreateNode () const
  {
    return csPtr<iResult> (new PlayAnimationResult);
  }
  const char* GetTypeName () const
  {
    return "playanimation";
  }
};

}
}

#endif
