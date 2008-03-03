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

#ifndef __CEL_ANIMATIONALALSIISTER__
#define __CEL_ANIMATIONALALSIISTER__

#include "csutil/scf.h"
#include "imesh/skelanim.h"
#include "physicallayer/datatype.h"
#include "propclass/animation.h"

struct iCelEntity;
struct iObjectRegistry;

namespace CEL
{
namespace Animation
{

struct iCondition : public virtual iBase
{
  virtual bool Initialise (iObjectRegistry *objreg, iCelEntity *ent) = 0;
  virtual bool SetParameter (const char* name, const celData &param) = 0;
  // can contain child conditions remember!!!
  // if passes then will call evaluate on its children
  virtual bool Evaluate () = 0;
};
struct iConditionFactory : public virtual iBase
{
  SCF_INTERFACE (iConditionFactory, 0, 0, 1);
  virtual csPtr<iCondition> CreateNode () const = 0;
  virtual const char* GetTypeName () const = 0;
};

struct iNode : public virtual iBase
{
  SCF_INTERFACE (iNode, 0, 0, 1);
  virtual bool Initialise (iObjectRegistry *objreg, iCelEntity *ent, csRef<Skeleton::iSkeleton> skel) = 0;
  virtual void AddChild (csRef<iNode> c) = 0;
  virtual void AttachCondition (csRef<iCondition> cond) = 0;
  virtual Skeleton::Animation::iMixingNode* GetMixingNode () = 0;
  virtual bool SetParameter (const char* name, const celData &param) = 0;
  virtual void Update () = 0;
  virtual void SetName (const char* n) = 0;
  virtual const char* GetName () = 0;
};
struct iNodeFactory : public virtual iBase
{
  SCF_INTERFACE (iNodeFactory, 0, 0, 1);
  virtual csPtr<iNode> CreateNode () const = 0;
  virtual const char* GetTypeName () const = 0;
};

struct iAnimationSystem : public virtual iBase
{
  SCF_INTERFACE (iAnimationSystem, 0, 0, 1);
  virtual csPtr<iNode> CreateNode (const char* factname) const = 0;
  virtual void RegisterNodeFactory (csRef<iNodeFactory> nodefact) = 0;

  virtual csPtr<iCondition> CreateCondition (const char* factname) const = 0;
  virtual void RegisterConditionFactory (csRef<iConditionFactory> condfact) = 0;
};

}
}

#endif //__CEL_ANIMATIONALALSIISTER__

