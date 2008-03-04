/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein

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
#include "plugins/propclass/animation/anim.h"
#include "iutil/objreg.h"
#include "ivideo/graph3d.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY_ALT (Animation, "pcobject.mesh.animation", "pcanimation")

//---------------------------------------------------------------------------

csStringID celPcAnimation::id_message = csInvalidStringID;

csHash<int, csStringID> constants;

PropertyHolder celPcAnimation::propinfo;

celPcAnimation::celPcAnimation (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  InitTokenTable (xmltokens);
  // For SendMessage parameters.
  if (id_message == csInvalidStringID)
    id_message = pl->FetchStringID ("cel.parameter.message");
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_message, "message");

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    //AddAction (action_print, "cel.action.Print");
  }

  // For properties.
  propinfo.SetCount (2);
  AddProperty (propid_counter, "cel.property.counter",
    CEL_DATA_LONG, false, "Print counter.", &counter);
  AddProperty (propid_max, "cel.property.max",
    CEL_DATA_LONG, false, "Max length.", 0);

  // Tick every so often so we can update the state
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);

  counter = 0;
  max = 0;
}

celPcAnimation::~celPcAnimation ()
{
  delete params;
}

#define TEST_SERIAL 0

csPtr<iCelDataBuffer> celPcAnimation::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcAnimation::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL)
    return false;
  return true;
}

bool celPcAnimation::PerformActionIndexed (int idx,
  iCelParameterBlock* params,
  celData& ret)
{
  switch (idx)
  {
    default:
      return false;
  }
  return false;
}
void celPcAnimation::TickEveryFrame ()
{
  if (rootnode)
    rootnode->Update ();
}

bool celPcAnimation::Setup ()
{
  csRef<Skeleton::iGraveyard> skelgrave = csQueryRegistry<Skeleton::iGraveyard> (object_reg);
  if (!skelgrave)
  {
    puts ("Can't find the graveyard!");
    return false;
  }
  skel = skelgrave->CreateSkeleton ("amir", "human");
  skelgrave->Debug ();
  return true;
}
void celPcAnimation::DrawSkeleton (iGraphics3D* g3d)
{
  if (!g3d)
    return;
  skel->DrawDebugBones (g3d);
}

CEL::Animation::iNode* celPcAnimation::FindNodeByName (const char* name)
{
  for (csRefArray<CEL::Animation::iNode>::Iterator it = allnodes.GetIterator (); it.HasNext (); )
  {
    CEL::Animation::iNode* node = it.Next ();
    if (node->GetName () && !strcmp (node->GetName (), name))
      return node;
  }
  return 0;
}
