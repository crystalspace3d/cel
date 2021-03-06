/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin
  
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

#ifndef __CEL_TOOLS_REWARD_CHANGEPROPERTY__
#define __CEL_TOOLS_REWARD_CHANGEPROPERTY__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "physicallayer/entity.h"
#include "propclass/prop.h"
#include "tools/rewards.h"
#include "tools/parameters.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that changes the value of a property.
 * This reward type listens to the name 'cel.rewards.changeproperty'.
 */
CEL_DECLARE_REWARDTYPE(ChangeProperty,"cel.rewards.changeproperty")

/**
 * The 'changeproperty' reward factory.
 */
class celChangePropertyRewardFactory : public scfImplementation2<
	celChangePropertyRewardFactory, iRewardFactory,
	iChangePropertyRewardFactory>
{
private:
  csRef<celChangePropertyRewardType> type;
  csString prop_par;
  csString entity_par;
  csString class_par;
  csString pc_par;
  csString tag_par;
  csString string_par;
  csString long_par;
  csString float_par;
  csString bool_par;
  csString diff_par;
  bool do_toggle;

public:
  celChangePropertyRewardFactory (celChangePropertyRewardType* type);
  virtual ~celChangePropertyRewardFactory ();

  virtual csPtr<iReward> CreateReward (iQuest* q, iCelParameterBlock* params);
  virtual iRewardType* GetRewardType () const { return type; }
  virtual bool Save (iDocumentNode* node);
  virtual bool Load (iDocumentNode* node);

  //----------------- iChangePropertyQuestRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity);
  virtual const char* GetEntity () const { return entity_par; }
  virtual void SetClassParameter (const char* ent_class);
  virtual const char* GetClass () const { return class_par; }
  virtual void SetPropertyParameter (const char* prop);
  virtual const char* GetProperty () const { return prop_par; }
  virtual celDataType GetPropertyType () const
  {
    if (string_par) return CEL_DATA_STRING;
    else if (long_par) return CEL_DATA_LONG;
    else if (float_par) return CEL_DATA_FLOAT;
    else if (bool_par) return CEL_DATA_BOOL;
    else return CEL_DATA_NONE;
  }
  virtual void SetPCParameter (const char* pc, const char* tag);
  virtual const char* GetPC () const { return pc_par; }
  virtual const char* GetPCTag () const { return tag_par; }
  virtual void SetStringParameter (const char* pstring);
  virtual const char* GetString () const { return string_par; }
  virtual void SetLongParameter (const char* plong);
  virtual const char* GetLong () const { return long_par; }
  virtual void SetFloatParameter (const char* pfloat);
  virtual const char* GetFloat () const { return float_par; }
  virtual void SetBoolParameter (const char* pbool);
  virtual const char* GetBool () const { return bool_par; }
  virtual void SetDiffParameter (const char* pdiff);
  virtual const char* GetDiff () const { return diff_par; }
  virtual void SetToggle (bool t);
  virtual bool IsToggle () const { return do_toggle; }
};

/**
 * The 'changeproperty' reward.
 */
class celChangePropertyRewardBase : public scfImplementation1<
	celChangePropertyRewardBase, iReward>
{
protected:
  csRef<celChangePropertyRewardType> type;
  csRef<iParameter> prop;
  csRef<iParameter> pc;
  csRef<iParameter> tag;
  csRef<iParameter> pstring;
  csRef<iParameter> plong;
  csRef<iParameter> pfloat;
  csRef<iParameter> pbool;
  csRef<iParameter> pdiff;
  bool do_toggle;

public:
  celChangePropertyRewardBase (celChangePropertyRewardType* type,
  	iCelParameterBlock* params,
	const char* prop_par,
	const char* pc,
	const char* tag,
	const char* string_par,
	const char* long_par,
	const char* float_par,
	const char* bool_par,
	const char* diff_par,
	bool do_toggle);
  virtual ~celChangePropertyRewardBase () {};
  virtual void Reward (iCelParameterBlock* params) = 0;
  
protected:
  void ChangePropertyOnPc (iCelPropertyClass *pclass, iCelParameterBlock* params);
  void ChangePropertyOnPcProp (iPcProperties *properties, iCelParameterBlock* params);

};


class celChangePropertyReward : public celChangePropertyRewardBase
{
private:
  csRef<iParameter> entity;
  csWeakRef<iCelEntity> ent;
  csWeakRef<iPcProperties> properties;
  csWeakRef<iCelPropertyClass> pclass;
  csRef<iParameterManager> pm;

public:
  celChangePropertyReward (celChangePropertyRewardType* type,
  	iCelParameterBlock* params,
	const char* prop_par,
	const char* entity_par,
	const char* pc,
	const char* tag,
	const char* string_par,
	const char* long_par,
	const char* float_par,
	const char* bool_par,
	const char* diff_par,
	bool do_toggle);
  virtual ~celChangePropertyReward () {};

  virtual void Reward (iCelParameterBlock* params);
};
/**
 * The 'changeproperty' reward.
 */
class celClassChangePropertyReward : public celChangePropertyRewardBase
{
private:
  csRef<iCelEntityList> entlist;
  csRef<iParameter> clazz;

public:
  celClassChangePropertyReward (celChangePropertyRewardType* type,
  	iCelParameterBlock* params,
	const char* prop_par,
	const char* class_par,
	const char* pc,
	const char* tag,
	const char* string_par,
	const char* long_par,
	const char* float_par,
	const char* bool_par,
	const char* diff_par,
	bool do_toggle);
  virtual ~celClassChangePropertyReward () {};

  virtual void Reward (iCelParameterBlock* params);

private:
  void PcReward (iCelParameterBlock* params);
  void PcPropReward (iCelParameterBlock* params);
};


#endif // __CEL_TOOLS_REWARD_CHANGEPROPERTY__

