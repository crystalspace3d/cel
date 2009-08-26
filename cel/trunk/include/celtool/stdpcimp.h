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
#include "iutil/objreg.h"
#include "ivaria/reporter.h"
#include "csutil/csobject.h"
#include "csutil/scf.h"
#include "csutil/scf_implementation.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"

#include "celtool/celtoolextern.h"

#include "physicallayer/propchg.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/messaging.h"

struct iCelEntity;
struct iObjectRegistry;


struct Property
{
  csStringID		id;
  celDataType		datatype;
  bool			readonly;
  const char*		desc;
  Property () : id (csInvalidStringID) { }
};

/**
 * Keep a static instance of this structure in your property
 * class so that your property class can implement properties
 * more easily.
 */
struct PropertyHolder
{
  Property* properties;
  size_t propertycount;
  // Set to true if we have done an action.
  bool actions_done;
  // The mask that we will use to listen for messages.
  csString mask;

  /**
   * This is a special hash that maps ID's to constants
   * so that we can use a switch to see which ID it is.
   */
  csHash<int, csStringID> constants;

  /**
   * This hash is like 'constants' except that it is used for new-style
   * action id's (like 'cel.wire.addinput').
   */
  csHash<int, csStringID> new_constants;

  PropertyHolder () : properties (0), propertycount (0), actions_done (false)
  { }
  ~PropertyHolder () { delete[] properties; }
  void SetCount (int cnt)
  {
    if (properties) return;
    propertycount = cnt;
    properties = new Property[cnt];
  }
};

/**
 * This is a common implementation for a property class
 * from which all other property classes can inherit.
 * This makes it easier to write a property class.
 */
class CEL_CELTOOL_EXPORT celPcCommon
  : public scfImplementation4<celPcCommon, iCelPropertyClass, iCelTimerListener,
    iMessageSender,iMessageReceiver>
{
private:
  csRefArray<iCelPropertyChangeCallback> callbacks;
  // This flag is true if we currently don't know the state of
  // the sibling property classes (property classes in the same
  // entity). It is set to true by PropertyClassesHaveChanged()
  // and cleared by HavePropertyClassesChanged().
  bool propclasses_dirty;
  char* tag;
  // the name of the property class stored in the iObject
  csObject csobj;

  static csStringID id_name;
  static csStringID id_value;

protected:
  iCelEntity* entity;
  iObjectRegistry* object_reg;
  csWeakRef<iCelPlLayer> pl;

protected:
  void FirePropertyChangeCallback (int propertyId);

  /**
   * Setup the mask used for receiving action messages.
   */
  void SetActionMask (const char* mask);

  /**
   * Helper function to setup an action.
   */
  void AddAction (int idx, const char* id)
  {
    propholder->actions_done = true;
    propholder->constants.Put (pl->FetchStringID (id), idx);
    csString newid = propholder->mask;
    CS_ASSERT (newid.Length () > 0);
    newid += id;
    propholder->new_constants.Put (pl->FetchStringID (newid), idx);
  }

  /**
   * Helper function to setup properties.
   * \param idx is a numerical index for the property starting at 0.
   * \param id is the id string ('bla').
   * \param type is the type for the property.
   * \param readonly
   * \param desc is the description.
   * \param pointer is the pointer of the field this
   * property corresponds with or 0 if it requires manual
   * intervention.
   */
  void AddProperty (size_t idx, const char* id,
	celDataType type, bool readonly, const char* desc,
	void* prop)
  {
    // return if an invalid index was specified
    if (idx >= propholder->propertycount)
    {
      csRef<iReporter> rep = csQueryRegistry<iReporter>(object_reg);
      if (rep)
        rep->ReportError("crystalspace.cel.physicallayer",
            "celPcCommon::AddProperty out of bounds %zu >= %zu!",
            idx, propholder->propertycount);
      else
        csPrintf("Error: celPcCommon::AddProperty out of bounds %zu >= %zu!",
            idx, propholder->propertycount);
      return;
    }
    if (propdata == 0)
    {
      propdata = new void* [propholder->propertycount];
    }
    Property& pr = propholder->properties[idx];
    if (pr.id == csInvalidStringID)
    {
      pr.id = pl->FetchStringID (id);
      pr.datatype = type;
      pr.readonly = readonly;
      pr.desc = desc;
      propholder->constants.Put (pr.id, (int)idx);
    }
    propdata[idx] = prop;
  }

  void** propdata;
  PropertyHolder* propholder;

private:
  template <class T>
  bool SetPropertyTemplated (csStringID propertyId, T l,
    celDataType type);
  template <class T>
  T GetPropertyTemplated (csStringID propertyId, celDataType type);
  template <class T>
  bool GetPropertyTemplated (csStringID propertyId, celDataType type, T& v);

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

  virtual const char* GetName () const;
  virtual void SetName (const char* pcname);

  iObject *QueryObject ();

  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
  virtual bool AddPropertyChangeCallback (iCelPropertyChangeCallback* cb);
  virtual bool RemovePropertyChangeCallback (
  	iCelPropertyChangeCallback* cb);

  virtual bool SetPropertyIndexed (int, long) { return false; }
  virtual bool SetPropertyIndexed (int, float) { return false; }
  virtual bool SetPropertyIndexed (int, bool) { return false; }
  virtual bool SetPropertyIndexed (int, const char*) { return false; }
  virtual bool SetPropertyIndexed (int, const csVector2&) { return false; }
  virtual bool SetPropertyIndexed (int, const csVector3&) { return false; }
  virtual bool SetPropertyIndexed (int, const csColor&) { return false; }
  virtual bool SetPropertyIndexed (int, iCelPropertyClass* pclass)
  { return false; }
  virtual bool SetPropertyIndexed (int, iCelEntity* entity) { return false; }
  virtual bool SetPropertyIndexed (int, iBase* ibase) { return false; }

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

  virtual bool GetPropertyIndexed (int, long& l) { return false; }
  virtual bool GetPropertyIndexed (int, float& f) { return false; }
  virtual bool GetPropertyIndexed (int, bool& b) { return false; }
  virtual bool GetPropertyIndexed (int, const char*&) { return false; }
  virtual bool GetPropertyIndexed (int, csVector2&) { return false; }
  virtual bool GetPropertyIndexed (int, csVector3&) { return false; }
  virtual bool GetPropertyIndexed (int, csColor&) { return false; }
  virtual bool GetPropertyIndexed (int, iCelPropertyClass*&) { return false; }
  virtual bool GetPropertyIndexed (int, iCelEntity*&) { return false; }
  virtual bool GetPropertyIndexed (int, iBase*&) { return false; }

  virtual long GetPropertyLongByID (csStringID);
  virtual float GetPropertyFloatByID (csStringID);
  virtual bool GetPropertyBoolByID (csStringID);
  virtual const char* GetPropertyStringByID (csStringID);
  virtual bool GetPropertyVectorByID (csStringID, csVector2&);
  virtual bool GetPropertyVectorByID (csStringID, csVector3&);
  virtual bool GetPropertyColorByID (csStringID, csColor&);
  virtual iCelPropertyClass* GetPropertyPClassByID (csStringID);
  virtual iCelEntity* GetPropertyEntityByID (csStringID);
  virtual iBase* GetPropertyIBaseByID (csStringID);

  virtual bool PerformAction (csStringID, iCelParameterBlock*, celData& ret);
  virtual bool PerformActionIndexed (int, iCelParameterBlock*, celData& ret)
  { return false; }
  virtual const char* GetPropertyOrActionDescription (csStringID);
  virtual size_t GetPropertyAndActionCount ();
  virtual csStringID GetPropertyOrActionID (size_t);
  virtual void PropertyClassesHaveChanged ()
  {
    propclasses_dirty = true;
  }
  virtual celDataType GetPropertyOrActionType (csStringID);
  virtual bool IsPropertyReadOnly (csStringID);

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

  // --- For iMessageSender --------------------------------------------
  virtual void MessageDispatcherRemoved (iMessageDispatcher* dispatcher) { }

  // --- For iMessageReceiver ------------------------------------------
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
};

#endif // __CEL_CELTOOL_STDPC__

