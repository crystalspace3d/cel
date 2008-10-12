/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2005 by Jorrit Tyberghein

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

#ifndef __CEL_PF_PROPFACT__
#define __CEL_PF_PROPFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/parray.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "csutil/csstring.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/prop.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;
struct iVirtualClock;
struct iGraphics2D;
struct iGraphics3D;
struct iFont;
class csStringArray;

/**
 * Factory for tools.
 */
CEL_DECLARE_FACTORY(Properties)

/**
 * This is a property property class.
 */
class celPcProperties :
  public scfImplementationExt1<celPcProperties,
						  celPcCommon,
						  iPcProperties>
{
private:
  struct property
  {
    char* propName;
    csStringID id;
    celDataType type;
    union
    {
      float f;
      long l;
      bool b;
      char* s;
      struct { float x, y, z; } vec;
      struct { float red, green, blue; } col;
    } v;
    csRef<iBase> ref;
    csWeakRef<iCelEntity> entity;
    csWeakRef<iCelPropertyClass> pclass;
  };
  csPDelArray<property> properties;
  csHash<size_t, csStringBase> properties_hash;
  bool properties_hash_dirty;
  csRefArray<iPcPropertyListener> listeners;

  size_t NewProperty (const char* name);
  size_t FindProperty (csStringID id);
  size_t FindOrNewProperty (const char* name);
  size_t FindOrNewProperty (csStringID id);
  void ClearPropertyValue (property* p);

  celOneParameterBlock* params;

  csRef<iMessageDispatcher> dispatcher_set;
  csRef<iMessageDispatcher> dispatcher_clear;

  static csStringID id_index;
  static csStringID id_name;
  static csStringID id_value;

  enum actionids
  {
    action_setproperty = 0
  };

  static PropertyHolder propinfo;

  void HandlePropertyChange (size_t idx);

public:
  celPcProperties (iObjectRegistry* object_reg);
  virtual ~celPcProperties ();

  virtual bool SetProperty (csStringID, const char*);
  virtual bool SetProperty (csStringID, bool);
  virtual bool SetProperty (csStringID, float);
  virtual bool SetProperty (csStringID, long);
  virtual bool SetProperty (csStringID, const csVector2&);
  virtual bool SetProperty (csStringID, const csVector3&);
  virtual bool SetProperty (csStringID, const csColor&);
  virtual bool SetProperty (csStringID, iCelPropertyClass*);
  virtual bool SetProperty (csStringID, iCelEntity*);
  virtual bool SetProperty (csStringID, iBase*);
  virtual const char* GetPropertyStringByID (csStringID);
  virtual bool GetPropertyBoolByID (csStringID);
  virtual long GetPropertyLongByID (csStringID);
  virtual float GetPropertyFloatByID (csStringID);
  virtual bool GetPropertyVectorByID (csStringID, csVector2&);
  virtual bool GetPropertyVectorByID (csStringID, csVector3&);
  virtual bool GetPropertyColorByID (csStringID, csColor&);
  virtual iCelPropertyClass* GetPropertyPClassByID (csStringID);
  virtual iCelEntity* GetPropertyEntityByID (csStringID);
  virtual iBase* GetPropertyIBaseByID (csStringID);

  virtual celDataType GetPropertyOrActionType (csStringID propertyID);
  virtual bool IsPropertyReadOnly (csStringID) { return false; }
  virtual size_t GetPropertyAndActionCount ()
  { return properties.GetSize (); }
  virtual csStringID GetPropertyOrActionID (size_t i);
  virtual const char* GetPropertyOrActionDescription (csStringID);

  void SetProperty (const char* name, float value);
  void SetProperty (const char* name, long value);
  void SetProperty (const char* name, bool value);
  void SetProperty (const char* name, const char* value);
  void SetProperty (const char* name, const csVector2& value);
  void SetProperty (const char* name, const csVector3& value);
  void SetProperty (const char* name, const csColor& value);
  void SetProperty (const char* name, iCelPropertyClass* pclass);
  void SetProperty (const char* name, iCelEntity* entity);
  void SetProperty (const char* name, iBase* value);
  size_t GetPropertyIndex (const char* name);
  void SetPropertyIndex (size_t index, float value);
  void SetPropertyIndex (size_t index, long value);
  void SetPropertyIndex (size_t index, bool value);
  void SetPropertyIndex (size_t index, const char* value);
  void SetPropertyIndex (size_t index, const csVector2& value);
  void SetPropertyIndex (size_t index, const csVector3& value);
  void SetPropertyIndex (size_t index, const csColor& value);
  void SetPropertyIndex (size_t index, iCelPropertyClass* pclass);
  void SetPropertyIndex (size_t index, iCelEntity* entity);
  void SetPropertyIndex (size_t index, iBase* value);
  celDataType GetPropertyType (size_t index) const;
  float GetPropertyFloatIndex (size_t index) const;
  long GetPropertyLongIndex (size_t index) const;
  bool GetPropertyBoolIndex (size_t index) const;
  bool GetPropertyVectorIndex (size_t index, csVector2& value) const;
  bool GetPropertyVectorIndex (size_t index, csVector3& value) const;
  bool GetPropertyColorIndex (size_t index, csColor& value) const;
  const char* GetPropertyStringIndex (size_t index) const;
  iCelPropertyClass* GetPropertyPClassIndex (size_t index) const;
  iCelEntity* GetPropertyEntityIndex (size_t index) const;
  iBase* GetPropertyIBaseIndex (size_t index) const;
  void ClearProperty (size_t index);
  void Clear ();
  size_t GetPropertyCount () const;
  const char* GetPropertyName (size_t idx) const;
  void Dump ();
  void AddPropertyListener (iPcPropertyListener* listener);
  void RemovePropertyListener (iPcPropertyListener* listener);
  void FirePropertyListeners (size_t idx);

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
  	celData& ret);

  /**\name iPcProperties implementation
   * @{ */
  float GetPropertyFloat (size_t index) const
  { return GetPropertyFloatIndex (index); }
  long GetPropertyLong (size_t index) const
  { return GetPropertyLongIndex (index); }
  bool GetPropertyBool (size_t index) const
  { return GetPropertyBoolIndex (index); }
  bool GetPropertyVector (size_t index, csVector2& v) const
  { return GetPropertyVectorIndex (index, v); }
  bool GetPropertyVector (size_t index, csVector3& v) const
  { return GetPropertyVectorIndex (index, v); }
  bool GetPropertyColor (size_t index, csColor& v) const
  { return GetPropertyColorIndex (index, v); }
  const char* GetPropertyString (size_t index) const
  { return GetPropertyStringIndex (index); }
  iCelPropertyClass* GetPropertyPClass (size_t index) const
  { return GetPropertyPClassIndex (index); }
  iCelEntity* GetPropertyEntity (size_t index) const
  { return GetPropertyEntityIndex (index); }
  iBase* GetPropertyIBase (size_t index) const
  { return GetPropertyIBaseIndex (index); }
  /** @} */
};

#endif // __CEL_PF_PROPFACT__

