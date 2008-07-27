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
#include "cstool/initapp.h"
#include "csutil/objreg.h"
#include "csutil/event.h"
#include "csutil/cfgacc.h"
#include "csutil/inputdef.h"
#include "csutil/array.h"
#include "csutil/priorityqueue.h"
#include "cstool/mapnode.h"
#include "ivaria/mapnode.h"
#include "csgeom/math3d.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "iutil/objreg.h"
#include "ivideo/graph3d.h"
#include "ivaria/reporter.h"
#include "ivaria/conin.h"
#include "ivaria/script.h"
#include "csutil/randomgen.h"
#include "csutil/hash.h"

#include "tools/expression.h"
#include "plugins/tools/ledges/ledges.h"

CS_IMPLEMENT_PLUGIN

//--------------------------------------------------------------------------

celLedge::celLedge () : scfImplementationType (this)
{
}
celLedge::~celLedge ()
{
}
void celLedge::AddPoint (const csVector3 &p)
{
  points.Push (p);
}

SCF_IMPLEMENT_FACTORY (celLedgeGroup)

celLedgeGroup::celLedgeGroup (iBase* parent) : scfImplementationType (this, parent)
{
}
celLedgeGroup::~celLedgeGroup ()
{
}
iObject* celLedgeGroup::QueryObject ()
{
  return this;
}
iLedge* celLedgeGroup::CreateLedge ()
{
  csRef<celLedge> l;
  l.AttachNew (new celLedge ());
  ledges.Push (l);
  return l;
}
