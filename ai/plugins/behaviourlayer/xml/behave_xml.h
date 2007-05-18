/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein

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

#ifndef __CEL_BLXML_BEHAVE__
#define __CEL_BLXML_BEHAVE__

#include "cstypes.h"
#include "csutil/randomgen.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/util.h"
#include "iutil/csinput.h"
#include "behaviourlayer/behave.h"
#include "tools/billboard.h"

struct iCelEntity;
struct iPcProperties;
struct iPcRules;
struct iPcBillboard;
struct iObjectRegistry;
class celXmlScript;

/**
 * General behaviour class.
 */
class celBehaviourXml : public iCelBehaviour
{
protected:
  iCelEntity* entity;
  iCelBlLayer* bl;
  csWeakRef<iPcProperties> props;	// Optimization.
  csWeakRef<iPcRules> rules;		// Optimization.
  csWeakRef<iPcBillboard> billboard;	// Optimization.
  iObjectRegistry* object_reg;
  celXmlScript* script;
  char* name;
  static csRandomGen rng;

public:
  celBehaviourXml (iCelEntity* entity, iObjectRegistry* object_reg);
  virtual ~celBehaviourXml ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  void SetBehaviourLayer (iCelBlLayer* bl)
  {
    celBehaviourXml::bl = bl;
  }
  void SetScript (celXmlScript* script)
  {
    celBehaviourXml::script = script;
  }
  iPcProperties* GetProperties ();
  iPcRules* GetRules ();
  iPcBillboard* GetBillboard ();
  iObjectRegistry* GetObjectRegistry () { return object_reg; }
  void Randomize () { rng.Initialize (csGetTicks ()); }
  float GetRandFloat (float f) { return rng.Get () * f; }
  int32 GetRandInt32 (int32 i) { return rng.Get (i); }
  uint32 GetRandUInt32 (uint32 u) { return rng.Get (u); }
  void Quit ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual iCelBlLayer* GetBehaviourLayer () const { return bl; }
  virtual bool SendMessage (const char* msg_id,
  	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, ...);
  virtual bool SendMessageV (const char* msg_id,
  	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg);
  virtual void* GetInternalObject () { return 0; }
};

/**
 * Special bootstrap behaviour.
 */
class celBehaviourBootstrap : public iCelBehaviour
{
protected:
  iCelEntity* entity;
  iCelBlLayer* bl;
  iObjectRegistry* object_reg;
  char* name;

public:
  celBehaviourBootstrap (iCelEntity* entity, iObjectRegistry* object_reg);
  virtual ~celBehaviourBootstrap ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  void SetBehaviourLayer (iCelBlLayer* bl)
  {
    celBehaviourBootstrap::bl = bl;
  }

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual iCelBlLayer* GetBehaviourLayer () const { return bl; }
  virtual bool SendMessage (const char* msg_id,
  	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, ...);
  virtual bool SendMessageV (const char* msg_id,
  	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg);
  virtual void* GetInternalObject () { return 0; }
};


#endif // __CEL_BLXML_BEHAVE__

