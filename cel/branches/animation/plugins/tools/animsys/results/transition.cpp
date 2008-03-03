#include "cssysdef.h"
#include "transition.h"
#include "ivaria/reporter.h"

namespace CEL
{
namespace Animation
{

TransitionResult::TransitionResult () : scfImplementationType (this)
{
}
bool TransitionResult::Initialise (iObjectRegistry *objreg, iCelEntity *ent, iPcAnimation *pcanim)
{
  return true;
}
bool TransitionResult::SetParameter (const char* name, const celData &param)
{
  return false;
}
bool TransitionResult::Execute ()
{
  puts ("booya!");
  return true;
}

}
}
