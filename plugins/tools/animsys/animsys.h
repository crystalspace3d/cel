/*    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
  
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_TOOLS_ANIMSYS__
#define __CEL_TOOLS_ANIMSYS__

#include "tools/animsys.h"
#include "csutil/refarr.h"
#include "csutil/scf_implementation.h"
#include "iutil/comp.h"

namespace CEL
{
namespace Animation
{

class AnimationSystem : public scfImplementation2<AnimationSystem, iAnimationSystem, iComponent>
{
public:
  AnimationSystem (iBase* parent);
  virtual ~AnimationSystem ();

  bool Initialize (iObjectRegistry *objreg);
  csPtr<iNode> CreateNode (const char* factname) const;
  void RegisterNodeFactory (csRef<iNodeFactory> nodefact);
  csPtr<iCondition> CreateCondition (const char* factname) const;
  void RegisterConditionFactory (csRef<iConditionFactory> condfact);
private:
  void RegisterNodeFactory (iNodeFactory *nodefact);
  void RegisterConditionFactory (iConditionFactory *condfact);
  csRefArray<iNodeFactory> nodefacts;
  csRefArray<iConditionFactory> condfacts;
  //csArray<iResultFactory> resultfacts;
};

}
}

#endif // __CEL_TOOLS_ANIMSYS__
