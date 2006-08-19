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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_CELTOOL_STDPC__
#define __CEL_CELTOOL_STDPC__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "csutil/scf_implementation.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/pl.h"

struct iCelEntity;
struct iObjectRegistry;


struct Property
{
  csStringID		id;
  celDataType		datatype;
  bool			readonly;
  const char*		desc;
};

/**
 * This is a common implementation for a property class
 * from which all other property classes can inherit.
 * This makes it easier to write a property class.
 */
class celPcCommon : public scfImplementation2<
	celPcCommon, iCelPropertyClass, iCelTimerListener>
{
private:
  csRefArray<iCelPropertyChangeCallback> callbacks;
  // This flag is true if we currently don't know the state of
  // the sibling property classes (property classes in the same
  // entity). It is set to true by PropertyClassesHaveChanged()
  // and cleared by HavePropertyClassesChanged().
  bool propclasses_dirty;
  char* tag;

protected:
  iCelEntity* entity;
  iObjectRegistry* object_reg;
  csWeakRef<iCelPlLayer> pl;

protected:
  void FirePropertyChangeCallback (int propertyId);

  void UpdateProperties () { return; }
  /**
   * Helper function to setup properties.
   * \param idx is a numerical index for the property starting at 0.
   * \param id is the id string ('cel.property.bla').
   * \param type is the type for the property.
   * \param readonly
   * \param desc is the description.
   */
  void AddProperty (int idx, const char* id,
	celDataType type, bool readonly, const char* desc)
  {
    props[idx].id = pl->FetchStringID (id);
    props[idx].datatype = type;
    props[idx].readonly = readonly;
    props[idx].desc = desc;
  }

  void** propdata;
  Property* props;
  size_t* propcount;

public:
  celPcCommon (iObjectRegistry* object_reg);
  virtual ~celPcCommon ();

  bool HavePropertyClassesChanged ()
  {
    if (!propclasses_dirty) return false;
    propclasses_dirty = false;
    return true;
  }

  virtual void SetTag (const char* tagname);
  virtual const char* GetTag () const { return tag; }

  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
  virtual bool AddPropertyChangeCallback (iCelPropertyChangeCallback* cb);
  virtual bool RemovePropertyChangeCallback (
  	iCelPropertyChangeCallback* cb);

  virtual bool SetProperty (csStringID, long);
  virtual bool SetProperty (csStringID, float);
  virtual bool SetProperty (csStringID, bool);
  virtual bool SetProperty (csStringID, const char*);
  virtual bool SetProperty (csStringID, const csVector2&);
  virtual bool SetProperty (csStringID, const csVector3&);
  virtual bool SetProperty (csStringID, const csColor&);
  virtual bool SetProperty (csStringID, iCelPropertyClass* pclass);
  virtual bool SetProperty (csStringID, iCelEntity* entity);
  virtual bool SetProperty (csStringID, iBase* ibase);
  virtual celDataType GetPropertyOrActionType (csStringID);
  virtual bool IsPropertyReadOnly (csStringID);
  virtual long GetPropertyLong (csStringID);
  virtual float GetPropertyFloat (csStringID);
  virtual bool GetPropertyBool (csStringID);
  virtual const char* GetPropertyString (csStringID);
  virtual bool GetPropertyVector (csStringID, csVector2&);
  virtual bool GetPropertyVector (csStringID, csVector3&);
  virtual bool GetPropertyColor (csStringID, csColor&);
  virtual iCelPropertyClass* GetPropertyPClass (csStringID);
  virtual iCelEntity* GetPropertyEntity (csStringID);
  virtual iBase* GetPropertyIBase (csStringID);
  virtual bool PerformAction (csStringID, iCelParameterBlock*, celData& ret)
  { return false; }
  virtual const char* GetPropertyOrActionDescription (csStringID);
  virtual size_t GetPropertyAndActionCount ();
  virtual csStringID GetPropertyOrActionID (size_t);
  virtual void PropertyClassesHaveChanged ()
  {
    propclasses_dirty = true;
  }

  virtual csPtr<iCelDataBuffer> SaveFirstPass () { return 0; }
  virtual bool LoadFirstPass (iCelDataBuffer*) { return 0; }

  virtual csPtr<iCelDataBuffer> GetPersistentData (
	celPersistenceType persistence_type)
    { return 0; }
  virtual celPersistenceResult SetPersistentData (csTicks data_time, 
        iCelDataBuffer* data, celPersistenceType persistence_type)
    { return CEL_PERSIST_RESULT_OK; };

  // --- For iCelTimerListener -----------------------------------------
  virtual void TickEveryFrame () { }
  virtual void TickOnce () { }
};

#endif // __CEL_CELTOOL_STDPC__

