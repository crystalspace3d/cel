%module blcel
%include cs.i
%{
#include "pl/pl.h"
#include "pl/propfact.h"
#include "pl/propclas.h"
#include "pl/entity.h"
#include "pl/persist.h"
#include "bl/bl.h"
#include "bl/behave.h"
#include "../pf/common/stdpcimp.h"
%}

struct iCelPlLayer : public iBase
{
  iCelEntity* CreateEntity ();
  virtual iCelPropertyClass* CreatePropertyClass (iCelEntity *entity, const char* propname);
  virtual const char* FetchString (csStringID id);

  virtual int GetBehaviourLayerCount () const = 0;
  virtual iCelBlLayer* GetBehaviourLayer (int idx) const = 0;
  virtual iCelBlLayer* FindBehaviourLayer (const char* name) const = 0;  
};

struct iCelEntity : public iBase
{
  const char* GetName () const;
  void SetName (const char* n);
  
  virtual void SetBehaviour (iCelBehaviour* ent);
  virtual iCelBehaviour* GetBehaviour ();

  virtual CS_ID GetID () const = 0;

  iCelPropertyClassList* GetPropertyClassList ();
};


struct iCelBlLayer : public iBase
{
  virtual const char* GetName () const = 0;

  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity,
  	const char* name) = 0;
};

struct iCelBehaviour : public iBase
{
  virtual const char* GetName () const = 0;

  virtual iCelBlLayer* GetBehaviourLayer () const = 0;

  virtual bool SendMessageV (const char* msg_id, iBase* msg_info, va_list arg) = 0;
};

struct iCelPropertyClass : public iBase
{
  virtual const char* GetName () const = 0;

  virtual iCelEntity* GetEntity () = 0;
  virtual void SetEntity (iCelEntity* entity) = 0;

  virtual celDataType GetPropertyOrActionType (
  	csStringID propertyID) = 0;

  virtual bool IsPropertyReadOnly (csStringID propertyID) = 0;
  virtual long GetPropertyLong (csStringID propertyID) = 0;
  virtual float GetPropertyFloat (csStringID propertyID) = 0;
  virtual bool GetPropertyBool (csStringID propertyID) = 0;
  virtual const char* GetPropertyString (csStringID propertyID) = 0;
  virtual bool GetPropertyVector (csStringID propertyID, csVector3& v) = 0;
  virtual int GetPropertyAndActionCount () const = 0;
  virtual csStringID GetPropertyOrActionID (int i) = 0;
  virtual const char* GetPropertyOrActionDescription (csStringID propertyID) = 0;

  virtual bool PerformAction (csStringID actionID, const char* params) = 0;

  %addmethods
  {
    bool SetPropertyLong (csStringID id, long l ) { return self->SetProperty (id, l); }
    bool SetPropertyFloat (csStringID id, float f) { return self->SetProperty (id, f); }
    bool SetPropertyBool (csStringID id, bool b) { return self->SetProperty (id, b); }
    bool SetPropertyString (csStringID id, const char* s) { return self->SetProperty (id, s); }
    bool SetPropertyVector3 (csStringID id, const csVector3& v) { return self->SetProperty (id, v); }
  }

};

struct iCelPropertyClassList : public iBase
{
  virtual int GetCount ();
  virtual iCelPropertyClass* Get (int n);
  virtual int Find (iCelPropertyClass* obj);
  virtual iCelPropertyClass* FindByName (const char *Name);
  virtual iBase* FindByInterface (scfInterfaceID id, int version);
};


enum celDataType
{
  CEL_DATA_NONE = 0,
  CEL_DATA_BOOL,
  CEL_DATA_BYTE,
  CEL_DATA_WORD,
  CEL_DATA_LONG,
  CEL_DATA_UBYTE,
  CEL_DATA_UWORD,
  CEL_DATA_ULONG,
  CEL_DATA_FLOAT,
  CEL_DATA_VECTOR3,
  CEL_DATA_STRING,
  CEL_DATA_PCLASS,
  CEL_DATA_ENTITY,
  CEL_DATA_BUFFER,
  CEL_DATA_ACTION
};
