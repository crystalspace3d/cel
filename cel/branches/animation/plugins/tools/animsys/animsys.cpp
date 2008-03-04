/*
    Crystal Space Entity Layer
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

#include "cssysdef.h"
#include "plugins/tools/animsys/animsys.h"

#include "nodes/animation.h"
#include "nodes/switcher.h"
//#include "nodes/test.h"
#include "conds/prop.h"
#include "results/transition.h"

CS_IMPLEMENT_PLUGIN

namespace CEL
{
namespace Animation
{

SCF_IMPLEMENT_FACTORY (AnimationSystem)

AnimationSystem::AnimationSystem (iBase* parent)
  : scfImplementationType (this, parent)
{
}
AnimationSystem::~AnimationSystem ()
{
}

bool AnimationSystem::Initialize (iObjectRegistry *objreg)
{
  RegisterNodeFactory (new AnimationNodeFactory);
  RegisterNodeFactory (new SwitcherNodeFactory);
  //RegisterNodeFactory (new TestNodeFactory);
  RegisterConditionFactory (new PropertyConditionFactory);
  RegisterResultFactory (new TransitionResultFactory);
  return true;
}
csPtr<iNode> AnimationSystem::CreateNode (const char* factname) const
{
  for (csRefArray<iNodeFactory>::ConstIterator it = nodefacts.GetIterator (); it.HasNext (); )
  {
    const iNodeFactory* nf = it.Next ();
    if (!strcmp (nf->GetTypeName (), factname))
    {
      return nf->CreateNode ();
    }
  }
  return 0;
}
void AnimationSystem::RegisterNodeFactory (csRef<iNodeFactory> nodefact)
{
  nodefacts.Push (nodefact);
}
void AnimationSystem::RegisterNodeFactory (iNodeFactory *nodefact)
{
  csRef<iNodeFactory> refnf;
  refnf.AttachNew (nodefact);
  nodefacts.Push (refnf);
}
csPtr<iCondition> AnimationSystem::CreateCondition (const char* factname) const
{
  for (csRefArray<iConditionFactory>::ConstIterator it = condfacts.GetIterator (); it.HasNext (); )
  {
    const iConditionFactory* cf = it.Next ();
    if (!strcmp (cf->GetTypeName (), factname))
    {
      return cf->CreateNode ();
    }
  }
  return 0;
}
void AnimationSystem::RegisterConditionFactory (csRef<iConditionFactory> condfact)
{
  condfacts.Push (condfact);
}
void AnimationSystem::RegisterConditionFactory (iConditionFactory *condfact)
{
  csRef<iConditionFactory> refcf;
  refcf.AttachNew (condfact);
  condfacts.Push (refcf);
}
csPtr<iResult> AnimationSystem::CreateResult (const char* factname) const
{
  for (csRefArray<iResultFactory>::ConstIterator it = resultfacts.GetIterator (); it.HasNext (); )
  {
    const iResultFactory* rf = it.Next ();
    if (!strcmp (rf->GetTypeName (), factname))
    {
      return rf->CreateNode ();
    }
  }
  return 0;
}
void AnimationSystem::RegisterResultFactory (csRef<iResultFactory> resfact)
{
  resultfacts.Push (resfact);
}
void AnimationSystem::RegisterResultFactory (iResultFactory *resfact)
{
  csRef<iResultFactory> refrf;
  refrf.AttachNew (resfact);
  resultfacts.Push (refrf);
}

}
}
