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
#include "nodes/test.h"

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
  RegisterNodeFactory (new TestNodeFactory);
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

}
}
