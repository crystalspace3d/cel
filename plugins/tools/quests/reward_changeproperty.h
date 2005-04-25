/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_QUESTS_REWARD_CHANGEPROPERTY__
#define __CEL_TOOLS_QUESTS_REWARD_CHANGEPROPERTY__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"
#include "physicallayer/entity.h"
#include "propclass/prop.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This reward type listens to the name 'cel.questreward.changeproperty'.
 */
CEL_DECLARE_REWARDTYPE(ChangeProperty,"cel.questreward.changeproperty")

/**
 * The 'changeproperty' reward factory.
 */
class celChangePropertyRewardFactory :
	public iQuestRewardFactory,
	public iChangePropertyQuestRewardFactory
{
private:
  celChangePropertyRewardType* type;
  char* prop_par;
  char* entity_name_par;
  char* string_par;
  char* long_par;
  char* float_par;
  char* bool_par;
  char* diff_par;
  bool do_toggle;

public:
  celChangePropertyRewardFactory (celChangePropertyRewardType* type);
  virtual ~celChangePropertyRewardFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestReward> CreateReward (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iChangePropertyQuestRewardFactory -----------------------
  virtual void SetEntityNameParameter (const char* entity_name);
  virtual void SetPropertyNameParameter (const char* prop_name);
  virtual void SetStringParameter (const char* pstring);
  virtual void SetLongParameter (const char* plong);
  virtual void SetFloatParameter (const char* pfloat);
  virtual void SetBoolParameter (const char* pbool);
  virtual void SetDiffParameter (const char* pdiff);
  virtual void SetToggle ();
};

/**
 * The 'changeproperty' reward.
 */
class celChangePropertyReward :
	public iQuestReward
{
private:
  celChangePropertyRewardType* type;
  char* prop;
  char* entity_name;
  char* pstring;
  char* plong;
  char* pfloat;
  char* pbool;
  char* pdiff;
  bool do_toggle;
  csWeakRef<iCelEntity> entity;
  csWeakRef<iPcProperties> properties;

public:
  celChangePropertyReward (celChangePropertyRewardType* type,
  	const celQuestParams& params,
	const char* prop_par,
	const char* entity_name_par,
	const char* string_par,
	const char* long_par,
	const char* float_par,
	const char* bool_par,
	const char* diff_par,
	bool do_toggle);
  virtual ~celChangePropertyReward ();

  SCF_DECLARE_IBASE;

  virtual void Reward ();
};

#endif // __CEL_TOOLS_QUESTS_REWARD_CHANGEPROPERTY__

