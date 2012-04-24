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

#ifndef __CEL_TOOLS_LOOT__
#define __CEL_TOOLS_LOOT__

#include "csutil/util.h"
#include "csutil/set.h"
#include "csutil/hash.h"
#include "csutil/array.h"
#include "iutil/comp.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
#include "physicallayer/propclas.h"

#include "tools/parameters.h"
#include "tools/loot.h"

class celLootManager;
struct iCelEntityTemplate;

struct lpItem
{
  csString templateName;
  iCelEntityTemplate* templatePtr;	// This is 0 in the original item. Only the copy will have this.
  float chance;
  int minamount;
  int maxamount;
  csRef<iParameter> validate;
  int attempts;		// Count how many generation attempts we already had. Used only on copy of item.
  lpItem (const lpItem& copy)
  {
    templateName = copy.templateName;
    templatePtr = copy.templatePtr;
    chance = copy.chance;
    minamount = copy.minamount;
    maxamount = copy.maxamount;
    validate = copy.validate;
    attempts = copy.attempts;
  }
  lpItem (const char* n, float chance, int minamount, int maxamount, iParameter* validate) :
    templateName (n), templatePtr (0), chance (chance), minamount (minamount), maxamount (maxamount),
    validate (validate), attempts (3) { }
};

struct lpLoot
{
  iCelEntityTemplate* templatePtr;
  int amount;
  lpLoot (iCelEntityTemplate* t, int amount) : templatePtr (t), amount (amount) { }
};

class celLootPackage : public scfImplementation2<celLootPackage, iLootPackage, iLootGenerator>
{
private:
  celLootManager* lootmgr;
  csString name;
  int minloot, maxloot;
  bool randomGeneration;
  csArray<lpItem> items;

public:
  celLootPackage (celLootManager* lootmgr, const char* name) :
    scfImplementationType (this), lootmgr (lootmgr),
    name (name), minloot (1), maxloot (10), randomGeneration (false)
  { }
  virtual ~celLootPackage () { }

  virtual const char* GetName () const { return name; }
  const csString& GetCsName () const { return name; }

  virtual void SetMinLoot (int m) { minloot = m; }
  virtual int GetMinLoot () const { return minloot; }
  virtual void SetMaxLoot (int m) { maxloot = m; }
  virtual int GetMaxLoot () const { return maxloot; }

  virtual void SetRandomGeneration (bool r) { randomGeneration = r; }
  virtual bool IsRandomGenerationEnabled () const { return randomGeneration; }

  virtual void AddLootItem (const char* templateName, float chance, int minamount, int maxamount,
      iParameter* validatePar = 0);
  virtual size_t GetLootItemCount () const { return items.GetSize (); }
  virtual const char* GetLootItemTemplate (size_t idx) const { return items[idx].templateName; }
  virtual float GetLootItemChance (size_t idx) const { return items[idx].chance; }
  virtual int GetLootItemMinAmount (size_t idx) const { return items[idx].minamount; }
  virtual int GetLootItemMaxAmount (size_t idx) const { return items[idx].maxamount; }
  virtual iParameter* GetLootItemValidate (size_t idx) const { return items[idx].validate; }

  virtual iLootGenerator* QueryLootGenerator () { return this; }

  virtual bool GenerateLoot (iPcInventory* inventory, iCelParameterBlock* params);
};

struct lsGenerator
{
  csRef<iLootGenerator> generator;
  float chance;
  csRef<iParameter> validate;
  lsGenerator (iLootGenerator* generator, float chance, iParameter* validate) :
    generator (generator), chance (chance), validate (validate) { }
};

class celLootSelector : public scfImplementation2<celLootSelector, iLootSelector, iLootGenerator>
{
private:
  csString name;
  csArray<lsGenerator> generators;

public:
  celLootSelector (const char* name) :
    scfImplementationType (this), name (name)
  { }
  virtual ~celLootSelector () { }

  virtual const char* GetName () const { return name; }
  const csString& GetCsName () const { return name; }

  virtual void AddLootGenerator (iLootGenerator* generator, float chance, iParameter* validatePar = 0);
  virtual size_t GetLootGeneratorCount () const { return generators.GetSize (); }
  virtual iLootGenerator* GetLootGenerator (size_t idx) const { return generators[idx].generator; }
  virtual float GetLootGeneratorChance (size_t idx) const { return generators[idx].chance; }
  virtual iParameter* GetLootGeneratorValidate (size_t idx) const { return generators[idx].validate; }

  virtual iLootGenerator* QueryLootGenerator () { return this; }

  virtual bool GenerateLoot (iPcInventory* inventory, iCelParameterBlock* params);
};

/**
 * The Loot Manager.
 */
class celLootManager : public scfImplementation2<celLootManager, iLootManager, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csWeakRef<iCelPlLayer> pl;

  csRefArray<celLootPackage> packages;
  csRefArray<celLootSelector> selectors;

public:
  celLootManager (iBase* parent);
  virtual ~celLootManager ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  iCelPlLayer* GetPL ();

  virtual iLootPackage* CreateLootPackage (const char* name);
  virtual size_t GetLootPackageCount () const { return packages.GetSize (); }
  virtual iLootPackage* GetLootPackage (size_t idx) const { return packages[idx]; }
  virtual iLootPackage* FindLootPackage (const char* name) const;
  virtual void RemoveLootPackage (iLootPackage* package);

  virtual iLootSelector* CreateLootSelector (const char* name);
  virtual size_t GetLootSelectorCount () const { return selectors.GetSize (); }
  virtual iLootSelector* GetLootSelector (size_t idx) const { return selectors[idx]; }
  virtual iLootSelector* FindLootSelector (const char* name) const;
  virtual void RemoveLootSelector (iLootSelector* selector);

  virtual iLootGenerator* FindLootGenerator (const char* name) const;
};

#endif // __CEL_TOOLS_LOOT__

