/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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

#include <math.h>

#include "cssysdef.h"
#include "plugins/propclass/npcmove/npcmove.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "csutil/debug.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivaria/reporter.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"
#include "iengine/mesh.h"
#include "imesh/object.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (NpcMove, "pcnpcmove")

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcNpcMove)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcNpcMove)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcNpcMove::PcNpcMove)
  SCF_IMPLEMENTS_INTERFACE (iPcNpcMove)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcNpcMove::celPcNpcMove (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcNpcMove);
  checked_spritestate = false;
  pl->CallbackPCOnce (this, 50, cscmdPreProcess);
}

celPcNpcMove::~celPcNpcMove ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcNpcMove);
}

void celPcNpcMove::TickOnce ()
{
  FindSiblingPropertyClasses ();
  GetSpriteStates ();

  if (sprcal3d)
    sprcal3d->SetAnimCycle ("stand", 1.0);
  else if (spr3d)
    spr3d->SetAction ("stand");
}

#define NPCMOVE_SERIAL 1

csPtr<iCelDataBuffer> celPcNpcMove::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (NPCMOVE_SERIAL);
  databuf->SetDataCount (0);
  return csPtr<iCelDataBuffer> (databuf);
}


bool celPcNpcMove::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != NPCMOVE_SERIAL) return false;
  if (databuf->GetDataCount () != 0) return false;
  return true;
}

void celPcNpcMove::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    checked_spritestate = false;
  }
}

void celPcNpcMove::GetSpriteStates ()
{
  if (checked_spritestate) return;
  if (!pcmesh) return;
  iMeshWrapper* m = pcmesh->GetMesh ();
  if (!m) return;
  iMeshObject* o = m->GetMeshObject ();
  if (!o) return;
  checked_spritestate = true;
  sprcal3d = SCF_QUERY_INTERFACE (o, iSpriteCal3DState);
  if (sprcal3d) return;
  spr3d = SCF_QUERY_INTERFACE (o, iSprite3DState);
}

//---------------------------------------------------------------------------

