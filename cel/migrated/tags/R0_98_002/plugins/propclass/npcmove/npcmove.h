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

#ifndef __CEL_PF_NPCMOVEFACT__
#define __CEL_PF_NPCMOVEFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/parray.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "csutil/hashhandlers.h"
#include "imesh/spritecal3d.h"
#include "imesh/sprite3d.h"

#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/npcmove.h"
#include "propclass/linmove.h"
#include "propclass/mesh.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;

/**
 * Factory for npcmove.
 */
CEL_DECLARE_FACTORY(NpcMove)

/**
 * This is a property class that helps with NPC movement.
 * It knows about pclinmove and can also control
 * animation in sprcal3d and spr3d mesh objects.
 */
class celPcNpcMove : public celPcCommon
{
private:
  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcMesh> pcmesh;

  bool checked_spritestate;
  csWeakRef<iSpriteCal3DState> sprcal3d;
  csWeakRef<iSprite3DState> spr3d;

  void FindSiblingPropertyClasses ();
  void GetSpriteStates ();

public:
  celPcNpcMove (iObjectRegistry* object_reg);
  virtual ~celPcNpcMove ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcnpcmove"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual void TickOnce ();

  struct PcNpcMove : public iPcNpcMove
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcNpcMove);
  } scfiPcNpcMove;
  friend struct PcNpcMove;
};

#endif // __CEL_PF_NPCMOVEFACT__

