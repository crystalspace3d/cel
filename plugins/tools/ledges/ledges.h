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

#ifndef __CEL_TOOLS_LEDGES__
#define __CEL_TOOLS_LEDGES__

#include "csutil/csobject.h"
#include "csutil/util.h"
#include "csutil/hash.h"
#include "csutil/redblacktree.h"
#include "csutil/weakrefarr.h"
#include "csutil/stringarray.h"
#include "csutil/eventhandlers.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "ivaria/conin.h"
#include "ivaria/conout.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
#include "physicallayer/propclas.h"
#include "behaviourlayer/behave.h"
#include "behaviourlayer/bl.h"
#include "tools/ledges.h"
#include "propclass/prop.h"
#include "propclass/quest.h"

class celLedge : public scfImplementation1<celLedge, iLedge>
{
 public:
  celLedge ();
  virtual ~celLedge ();
  void AddPoint (const csVector2 &p);
  void SetYPosition (float y);
  float GetYPosition () const;
  size_t GetPointCount () const;
  const csVector2 &GetPoint (size_t i) const;
 private:
  csArray<csVector2> points;
  float ypos;
};

class celLedgeGroup : public scfImplementationExt1<celLedgeGroup, csObject, iLedgeGroup>
{
 public:
  celLedgeGroup (iBase* parent);
  virtual ~celLedgeGroup ();
  iObject* QueryObject ();
  iLedge* CreateLedge ();
  size_t GetCount () const;
  iLedge* Get (size_t i);
 private:
  iObjectRegistry* object_reg;
  csRefArray<celLedge> ledges;
};

#endif // __CEL_TOOLS_LEDGES__
