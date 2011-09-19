/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein

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
#include "csutil/weakref.h"
#include "csutil/event.h"
#include "csutil/cfgacc.h"
#include "csutil/randomgen.h"
#include "csgeom/math3d.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "iutil/object.h"
#include "iutil/virtclk.h"
#include "ivaria/reporter.h"

#include "lootmgr.h"
#include "propclass/inv.h"

SCF_IMPLEMENT_FACTORY (celLootManager)

csRandomGen rnd;

//---------------------------------------------------------------------------------------

void celLootPackage::AddLootItem (const char* templateName, float chance, int minamount, int maxamount,
      iParameter* validatePar)
{
  items.Push (lpItem (templateName, chance, minamount, maxamount, validatePar));
}

bool celLootPackage::GenerateLoot (iPcInventory* inventory, iCelParameterBlock* params)
{
  csArray<lpLoot> loot;
  // By making a copy we also copy the 'attempts' field which is set to a fixed number in
  // the base array. In the copy we also fill in the templatePtr from the templateName.
  csArray<lpItem> itemsCopy;
  for (size_t i = 0 ; i < items.GetSize () ; i++)
  {
    iCelEntityTemplate* tpl = lootmgr->GetPL ()->FindEntityTemplate (items[i].templateName);
    if (tpl)
    {
      lpItem item = items[i];
      item.templatePtr = tpl;
      item.attempts = 3;
      itemsCopy.Push (item);
    }
  }

  size_t itemsIdx = 0;

  size_t desiredAmount = rnd.Get (maxloot-minloot+1) + minloot;
  while (loot.GetSize () < desiredAmount && itemsCopy.GetSize () > 0)
  {
    if (randomGeneration) itemsIdx = rnd.Get (itemsCopy.GetSize ());
    lpItem& item = itemsCopy[itemsIdx];
    if (item.chance > rnd.Get () && ((!item.validate) || item.validate->GetBool (params)))
    {
      loot.Push (lpLoot (item.templatePtr, rnd.Get (item.maxamount-item.minamount+1) + item.minamount));
      itemsCopy.DeleteIndex (itemsIdx);
    }
    else if (item.attempts <= 1)
    {
      itemsCopy.DeleteIndex (itemsIdx);
    }
    else
    {
      item.attempts--;
      itemsIdx++;
    }
    if (itemsIdx >= itemsCopy.GetSize ()) itemsIdx = 0;	// Wrap
  }

  for (size_t i = 0 ; i < loot.GetSize () ; i++)
  {
    if (!inventory->AddEntityTemplate (loot[i].templatePtr, loot[i].amount))
      return false;
  }
  return true;
}

//---------------------------------------------------------------------------------------

void celLootSelector::AddLootGenerator (iLootGenerator* generator, float chance, iParameter* validatePar)
{
  generators.Push (lsGenerator (generator, chance, validatePar));
}

bool celLootSelector::GenerateLoot (iPcInventory* inventory, iCelParameterBlock* params)
{
  for (size_t i = 0 ; i < generators.GetSize () ; i++)
  {
    if (generators[i].chance > rnd.Get ())
    {
      if ((!generators[i].validate) || generators[i].validate->GetBool (params))
      {
	return generators[i].generator->GenerateLoot (inventory, params);
      }
    }
  }
  return true;
}

//---------------------------------------------------------------------------------------

celLootManager::celLootManager (iBase* parent)
  : scfImplementationType (this, parent)
{
}

celLootManager::~celLootManager ()
{
}

bool celLootManager::Initialize (iObjectRegistry* object_reg)
{
  celLootManager::object_reg = object_reg;
  return true;
}

iCelPlLayer* celLootManager::GetPL ()
{
  if (!pl)
  {
    pl = csQueryRegistry<iCelPlLayer> (object_reg);
    if (!pl) return 0;
  }
  return pl;
}

iLootPackage* celLootManager::CreateLootPackage (const char* name)
{
  csRef<celLootPackage> lootpackage;
  lootpackage.AttachNew (new celLootPackage (this, name));
  packages.Push (lootpackage);
  return lootpackage;
}

iLootPackage* celLootManager::FindLootPackage (const char* name) const
{
  // @@@ More optimal? Use a hash or something?
  for (size_t i = 0 ; i < packages.GetSize () ; i++)
    if (packages[i]->GetCsName () == name) return packages[i];
  return 0;
}

void celLootManager::RemoveLootPackage (iLootPackage* package)
{
  // @@@ More optimal? Use a hash or something?
  packages.Delete (static_cast<celLootPackage*> (package));
}

iLootSelector* celLootManager::CreateLootSelector (const char* name)
{
  csRef<celLootSelector> lootselector;
  lootselector.AttachNew (new celLootSelector (name));
  selectors.Push (lootselector);
  return lootselector;
}

iLootSelector* celLootManager::FindLootSelector (const char* name) const
{
  // @@@ More optimal? Use a hash or something?
  for (size_t i = 0 ; i < selectors.GetSize () ; i++)
    if (selectors[i]->GetCsName () == name) return selectors[i];
  return 0;
}

void celLootManager::RemoveLootSelector (iLootSelector* selector)
{
  // @@@ More optimal? Use a hash or something?
  selectors.Delete (static_cast<celLootSelector*> (selector));
}

iLootGenerator* celLootManager::FindLootGenerator (const char* name) const
{
  iLootPackage* package = FindLootPackage (name);
  if (package) return package->QueryLootGenerator ();
  iLootSelector* selector = FindLootSelector (name);
  if (selector) return selector->QueryLootGenerator ();
  return 0;
}

//---------------------------------------------------------------------------

