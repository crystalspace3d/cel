#define __CEL__
%module blcelc
%include "ivaria/cspace.i"
%{
#include "physicallayer/pl.h"
#include "physicallayer/propfact.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/behave.h"
#include "propclass/region.h"
#include "propclass/camera.h"
#include "propclass/mesh.h"
#include "propclass/meshsel.h"
#include "propclass/timer.h"
#include "propclass/solid.h"
#include "propclass/gravity.h"
#include "propclass/move.h"
#include "propclass/inv.h"
#include "propclass/chars.h"
#include "propclass/linmove.h"
#include "propclass/input.h"
#include "propclass/dynmove.h"
#include "propclass/billboard.h"
#include "plugins/behaviourlayer/python/blpython.h"
#include "tools/billboard.h"
%}

//=======================================================================

// some macros

%define CEL_PC_CREATE(pcType, funcName, pcname)
%inline %{
pcType *funcName(iCelPlLayer *pl, iCelEntity *entity) {
  csRef<iCelPropertyClass> pc = pl->CreatePropertyClass(entity, #pcname );
  if (!pc.IsValid()) return 0;
  csRef<pcType> pclm = SCF_QUERY_INTERFACE(pc, pcType);
  if (!pclm.IsValid()) return 0;
  return pclm;
}
%}
%enddef

%define CEL_PC_GET(pcType, funcName)
%inline %{
pcType * funcName (iCelEntity *entity)
{
  csRef<pcType> pc = CEL_QUERY_PROPCLASS (
    entity->GetPropertyClassList (), pcType);
  if (!pc.IsValid()) return 0;
  return pc;
}
%}
%enddef

%define CEL_PC_QUERY(pcType)
%inline %{
pcType *scfQuery_ ## pcType (iCelPropertyClass *pc)
{
  csRef<pcType> iface = SCF_QUERY_INTERFACE(pc, pcType);
  if (iface) iface->IncRef ();
  return iface;
}
%}
%enddef

%define CEL_PC_QUERY_CLASSLIST(pcType)
%inline %{
pcType *scfQueryPC_ ## pcType (iCelPropertyClassList *pclist)
{
  csRef<pcType> iface = SCF_QUERY_INTERFACE(pclist, pcType);
  if (iface) iface->IncRef ();
  return iface;
}
%}
%enddef

%define CEL_PC(pcType, funcBaseName, pcname)
CEL_PC_CREATE(pcType, celCreate ## funcBaseName, pcname)
CEL_PC_GET(pcType, celGet ## funcBaseName)
CEL_PC_QUERY(pcType)
%enddef

// ======================================================================
struct iCelPlLayer : public iBase
{
  //virtual csPtr<iCelEntity> CreateEntity ();
  virtual iCelEntity* FindEntity (const char* name) = 0;
  virtual void RemoveEntity (iCelEntity* entity) = 0;
  virtual bool LoadPropertyClassFactory (const char* plugin_id) = 0;
  virtual iCelPropertyClass* CreatePropertyClass (iCelEntity *entity,
  	const char* propname);
  virtual const char* FetchString (csStringID id);

  virtual int GetBehaviourLayerCount () const = 0;
  virtual iCelBlLayer* GetBehaviourLayer (int idx) const = 0;
  virtual iCelBlLayer* FindBehaviourLayer (const char* name) const = 0;  
};

%inline %{
iCelPlLayer *csQueryRegistry_iCelPlLayer (iObjectRegistry *object_reg)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  return pl;
}
%}

struct iCelEntity : public iBase
{
  const char* GetName () const;
  void SetName (const char* n);
  
  virtual void SetBehaviour (iCelBehaviour* ent);
  virtual iCelBehaviour* GetBehaviour ();

  virtual CS_ID GetID () const = 0;

  iCelPropertyClassList* GetPropertyClassList ();

  %extend {
    iCelBehaviour *CreateBehaviour(iCelBlLayer *bl, const char *name)
    {
      csRef<iCelBehaviour> bh(bl->CreateBehaviour(self, name));
      if (!bh.IsValid()) return 0;
      return bh;
    }
  }
};

%inline %{
bool celRegisterPCFactory (iObjectRegistry* object_reg, const char* pcfactname)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg,
  	iCelPlLayer);
  bool rc = pl->LoadPropertyClassFactory (pcfactname);
  return rc;
}
%}

%inline %{
iCelEntity *celCreateEntity(iCelPlLayer *pl, const char *name)
{
  csRef<iCelEntity> en = pl->CreateEntity();
  if (!en.IsValid()) return 0;
  en->SetName (name);
  en->IncRef ();
  return en;
}
%}

%inline %{
iCelEntity *scfQueryInterface_iCelEntity (iBase *base)
{
  csRef<iCelEntity> ent = SCF_QUERY_INTERFACE (base, iCelEntity);
  return ent;
}
%}

struct iCelEntityList : public iBase
{
  virtual int GetCount () const = 0;
  virtual iCelEntity* Get (int n) const = 0;
  virtual int Add (iCelEntity* obj) = 0;
  virtual bool Remove (iCelEntity* obj) = 0;
  virtual bool Remove (int n) = 0;
  virtual void RemoveAll () = 0;
  virtual int Find (iCelEntity* obj) const = 0;
  virtual iCelEntity* FindByName (const char *Name) const = 0;
};

%inline %{
iCelEntityList *celFindNearbyEntities (iObjectRegistry *object_reg,
	iSector *sector, csVector3 pos, float radius)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl.IsValid()) return 0;
  csRef<iCelEntityList> entlist = pl->FindNearbyEntities (sector, pos, radius);
  entlist->IncRef();
  return entlist;
}
%}

struct iCelBlLayer : public iBase
{
  virtual const char* GetName () const = 0;
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity,
  	const char* name) = 0;
};

%inline %{
iCelBlLayer *csQueryRegistry_iCelBlLayer (iObjectRegistry *object_reg)
{
  csRef<iCelBlLayer> bl = CS_QUERY_REGISTRY (object_reg, iCelBlLayer);
  return bl;
}
%}

struct iCelParameterBlock : public iBase
{
  virtual int GetParameterCount () const = 0;
  virtual const char* GetParameter (int idx, csStringID& id,
  	celDataType& t) const = 0;
  virtual const celData* GetParameter (csStringID id) const = 0;
};

struct iCelBehaviour : public iBase
{
  virtual const char* GetName () const = 0;
  virtual iCelBlLayer* GetBehaviourLayer () const = 0;
  virtual bool SendMessage(const char* msg_id, celData& ret,
	iCelParameterBlock *params) = 0;

  %extend {
    PyObject *GetPythonObject()
    {
      PyObject* obj = (PyObject*)(self->GetInternalObject());
      Py_INCREF (obj);
      return obj;
    }
  }
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

  virtual bool PerformAction (csStringID actionID, 
  	iCelParameterBlock* params) = 0;

  %extend
  {
    bool SetPropertyLong (csStringID id, long l )
    { return self->SetProperty (id, l); }
    bool SetPropertyFloat (csStringID id, float f)
    { return self->SetProperty (id, f); }
    bool SetPropertyBool (csStringID id, bool b)
    { return self->SetProperty (id, b); }
    bool SetPropertyString (csStringID id, const char* s)
    { return self->SetProperty (id, s); }
    bool SetPropertyVector3 (csStringID id, const csVector3& v)
    { return self->SetProperty (id, v); }
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

//=======================================================================

struct iPcDynamicSystem : public iBase
{
  virtual iDynamicSystem* GetDynamicSystem () = 0;
  virtual void AddForceDuration (iPcDynamicBody* pcbody,
  	const csVector3& force, float seconds) = 0;
  virtual void AddForceFrame (iPcDynamicBody* pcbody,
  	const csVector3& force) = 0;
  virtual void ClearForces (iPcDynamicBody* pcbody) = 0;
  virtual void ClearAllForces () = 0;
};

CEL_PC(iPcDynamicSystem, DynamicSystem, pcdynsys)
CEL_PC_QUERY_CLASSLIST(iPcDynamicSystem)

//=======================================================================

struct iPcDynamicBody : public iBase
{
  virtual void SetMesh (iPcMesh* mesh) = 0;
  virtual iPcMesh* GetMesh () = 0;
  virtual void SetDynamicSystem (iPcDynamicSystem* dynsys) = 0;
  virtual iPcDynamicSystem* GetDynamicSystem () = 0;
  virtual iRigidBody* GetBody () = 0;
  virtual void SetParameters (float friction, float density,
  	float elasticity, float softness, float mass) = 0;
  virtual void MakeStatic (bool stat) = 0;
  virtual bool IsStatic () const = 0;
  virtual void AttachColliderSphere (float radius, const csVector3& offset) = 0;
  virtual void AttachColliderCylinder (float length, float radius,
  	const csOrthoTransform& trans) = 0;
  virtual void AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans) = 0;
  virtual void AttachColliderPlane (const csPlane3& plane) = 0;
  virtual void AttachColliderMesh () = 0;
  virtual void AddForceOnce (const csVector3& force) = 0;
  virtual void AddForceDuration (const csVector3& force, float seconds) = 0;
  virtual void AddForceFrame (const csVector3& force) = 0;
  virtual void ClearForces () = 0;
};

CEL_PC(iPcDynamicBody, DynamicBody, pcdynbody)
CEL_PC_QUERY_CLASSLIST(iPcDynamicBody)

//=======================================================================

struct iBillboard : public iBase
{
  virtual const char* GetName () const = 0;
  virtual csFlags& GetFlags () = 0;
  virtual bool SetMaterialName (const char* matname) = 0;
  virtual const char* GetMaterialName () = 0;
  virtual void SetSize (int w, int h) = 0;
  virtual void GetImageSize (int& w, int& h) = 0;
  virtual void GetSize (int& w, int& h) = 0;
  virtual void SetPosition (int x, int y) = 0;
  virtual void GetPosition (int& x, int& y) const = 0;
  virtual void Move (int dx, int dy) = 0;
};

struct iBillboardManager : public iBase
{
  virtual iBillboard* CreateBillboard (const char* name) = 0;
  virtual iBillboard* FindBillboard (const char* name) const = 0;
  virtual void RemoveBillboard (iBillboard* billboard) = 0;
  virtual int GetBillboardCount () const = 0;
  virtual iBillboard* GetBillboard (int idx) const = 0;
  virtual void RemoveAll () = 0;
  virtual void SetFlags (uint32 flags, uint32 mask) = 0;
};

struct iPcBillboard : public iBase
{
  virtual void SetBillboardName (const char* name) = 0;
  virtual const char* GetBillboardName () = 0;
  virtual iBillboard* GetBillboard () = 0;
};

CEL_PC(iPcBillboard, Billboard, pcbillboard)
CEL_PC_QUERY_CLASSLIST(iPcBillboard)

//=======================================================================

struct iPcRegion : public iBase
{
  virtual void SetWorldFile (const char* vfsdir, const char* name) = 0;
  virtual const char* GetWorldDir () const = 0;
  virtual const char* GetWorldFile () const = 0;
  virtual void CreateEmptySector (const char* name) = 0;
  virtual void SetRegionName (const char* name) = 0;
  virtual const char* GetRegionName () const = 0;
  virtual bool Load () = 0;
  virtual void Unload () = 0;
  virtual iSector* FindSector (const char* sectorname) = 0;
  virtual iSector* GetStartSector (const char* name = 0) = 0;
  virtual csVector3 GetStartPosition (const char* name = 0) = 0;
  virtual void PointCamera (iPcCamera* pccamera, const char* name = 0) = 0;
  
  %extend {
    bool LoadWorld (const char *vfsdir, const char *name)
    {
      self->SetWorldFile (vfsdir, name);
      return self->Load ();
    }
  }

};

%inline %{
iPcRegion *celCreateRegion (iCelPlLayer *pl, iCelEntity *entity,
	const char *name)
{
  csRef<iCelPropertyClass> pc = pl->CreatePropertyClass(entity, "pcregion");
  if (!pc.IsValid()) return 0;
  csRef<iPcRegion> pcregion = SCF_QUERY_INTERFACE(pc, iPcRegion);
  if (!pcregion.IsValid()) return 0;
  pcregion->SetRegionName (name);
  return pcregion;
}
%}

CEL_PC_GET(iPcRegion, Region)
CEL_PC_QUERY(iPcRegion)
CEL_PC_QUERY_CLASSLIST(iPcRegion)

//=======================================================================

struct iPcCommandInput : public iBase
{
  virtual void Activate (bool activate=true) = 0;
  virtual bool LoadConfig (const char* fname) = 0;
  virtual bool Bind (const char* triggername, const char* command) = 0;
  virtual const char* GetBind (const char* triggername) const = 0;
  virtual bool RemoveBind (const char* triggername, const char* command) = 0;
};

CEL_PC(iPcCommandInput, CommandInput, pckeyinput)

//=======================================================================

struct iPcLinearMovement : public iBase
{
  virtual void SetRotation (const csVector3& angle) = 0;
  virtual void SetSpeed (float speedZ) = 0;
  virtual void SetCameraPitchSpeed (float angle) = 0;
  virtual void SetVelocity (const csVector3& vel) = 0;
  virtual void GetVelocity (csVector3& v) = 0;
  virtual bool InitCD (const csVector3& body, const csVector3& shift,
  	const csVector3& shift)=0;
  virtual bool InitCD () = 0;
  virtual csPtr<iDataBuffer> GetDRData() = 0;
  virtual bool SetDRData (iDataBuffer* data, bool detectcheat) = 0;
  virtual bool NeedDRData (uint8& priority) = 0;
  virtual void SetPosition (const csVector3& pos, float yrot,
  	const iSector* sector) = 0;
  virtual void GetLastPosition (csVector3& pos, float& yrot,
  	iSector*& sector) = 0;
  virtual iSector* GetSector () = 0;
  virtual void SetReady (bool flag) = 0;    
  virtual bool IsReady() const = 0;    
  virtual bool IsOnGround () const = 0;
  virtual void ExtrapolatePosition (float delta) = 0;
};

CEL_PC(iPcLinearMovement, LinearMovement, pclinearmovement)

//=======================================================================

struct iPcCamera : public iBase
{
  enum CameraMode
  {
    freelook = 0,
    follow,
    rotational,
    firstperson
  };
  virtual bool SetRegion (iPcRegion* region, bool point = true,
      const char* name = 0) = 0;
  virtual bool SetMode (CameraMode m, bool use_cd = true) = 0;
  virtual CameraMode GetMode () const = 0;
  virtual void SetFollowPos (const csVector3& fp, const csVector3& at) = 0;
  virtual void GetFollowPos (csVector3& fp, csVector3& at) const = 0;
  virtual void SetPitch(float angle) = 0;
  virtual float GetPitch() = 0;
  virtual void SetRoll(float angle) = 0;
  virtual float GetRoll() = 0;
  virtual void SetRectangle (int x, int y, int w, int h) = 0;
  virtual iCamera* GetCamera () const = 0;
  virtual iView* GetView () const = 0;
  virtual void SetClearZBuffer (bool flag) = 0;
  virtual bool GetClearZBuffer () const = 0;
  virtual void SetClearScreen (bool flag) = 0;
  virtual bool GetClearScreen () const = 0;
};

CEL_PC(iPcCamera, Camera, pccamera)

//=======================================================================

#define CEL_MOUSE_BUTTON1 1
#define CEL_MOUSE_BUTTON2 2
#define CEL_MOUSE_BUTTON3 4

struct iPcMeshSelect : public iBase
{
  virtual void SetCamera (iPcCamera* camera) = 0;
  virtual void SetMouseButtons (int buttons) = 0;
  virtual int GetMouseButtons () const = 0;
  virtual void SetGlobalSelection (bool glob) = 0;
  virtual bool HasGlobalSelection () const = 0;
  virtual void SetFollowMode (bool follow) = 0;
  virtual bool HasFollowMode () const = 0;
  virtual void SetFollowAlwaysMode (bool followalways) = 0;
  virtual bool HasFollowAlwaysMode () const = 0;
  virtual void SetDragMode (bool drag) = 0;
  virtual bool HasDragMode () const = 0;
  virtual void SetDragPlaneNormal (const csVector3& drag_normal,
  	bool camera_space) = 0;
  virtual void GetDragPlaneNormal (csVector3& drag_normal,
  	bool& camera_space) const = 0;
  virtual void SetSendmoveEvent (bool mov) = 0;
  virtual bool HasSendmoveEvent () const = 0;
  virtual void SetSendupEvent (bool su) = 0;
  virtual bool HasSendupEvent () const = 0;
  virtual void SetSenddownEvent (bool sd) = 0;
  virtual bool HasSenddownEvent () const = 0;
};

CEL_PC(iPcMeshSelect, MeshSelect, pcmeshselect)

//=======================================================================

struct iPcMesh : public iBase
{
  %name(LoadMesh) virtual bool SetMesh (const char* factname,
  	const char* filename) = 0;
  virtual void SetMesh (iMeshWrapper* mesh) = 0;
  virtual void CreateEmptyThing () = 0;
  virtual iMeshWrapper* GetMesh () const = 0;
  virtual void MoveMesh (iSector* sector, const csVector3& pos) = 0;
  virtual void SetAction (const char* actionName, bool reset = false) = 0;
  virtual const char* GetAction () = 0;
  virtual void Hide () = 0;
  virtual void Show () = 0;
  virtual bool IsVisible () const = 0;
};

CEL_PC(iPcMesh, Mesh, pcmesh)

//=======================================================================

struct iPcTimer : public iBase
{
  virtual void WakeUp (csTicks t, bool repeat) = 0;
  virtual void Clear () = 0;
};

CEL_PC(iPcTimer, Timer, pctimer)

//=======================================================================

struct iPcSolid : public iBase
{
  virtual void SetMesh (iPcMesh* mesh) = 0;
  virtual iPcMesh* GetMesh () const = 0;
  virtual iCollider* GetCollider () = 0;
};

CEL_PC(iPcSolid, Solid, pcsolid)

//=======================================================================

struct iPcGravity : public iBase
{
  %name(CreateGravityColliderFromMesh) virtual void CreateGravityCollider (
  	iPcMesh* mesh) = 0;
  virtual void CreateGravityCollider (const csVector3& dim,
  	const csVector3& offs) = 0;
  virtual iCollider* GetGravityCollider () = 0;
  virtual void SetMovable (iPcMovable* movable) = 0;
  virtual iPcMovable* GetMovable () = 0;
  virtual void SetSolid (iPcSolid* solid) = 0;
  virtual iPcSolid* GetSolid () = 0;
  virtual void SetWeight (float w) = 0;
  virtual float GetWeight () const = 0;
  virtual void ClearForces () = 0;
  virtual void ClearPermanentForces () = 0;
  virtual void ResetSpeed () = 0;
  virtual void ApplyForce (const csVector3& force, float time) = 0;
  virtual void ApplyPermanentForce (const csVector3& force) = 0;
  virtual bool IsResting () const = 0;
  virtual void SetActive (bool activate) = 0;
  virtual bool IsActive () const = 0;
};

CEL_PC(iPcGravity, Gravity, pcgravity)

//=======================================================================

struct iPcMovable : public iBase
{
  virtual void SetMesh (iPcMesh* mesh) = 0;
  virtual iPcMesh* GetMesh () = 0;
  %name(SetPos) virtual int Move (iSector* sector, const csVector3& pos) = 0;
  virtual int Move (const csVector3& relpos) = 0;
  virtual void AddConstraint (iPcMovableConstraint* constraint) = 0;
  virtual void RemoveConstraint (iPcMovableConstraint* constraint) = 0;
  virtual void RemoveAllConstraints () = 0;
};

CEL_PC(iPcMovable, Movable, pcmovable)

//=======================================================================

struct iPcInventory : public iBase
{
  virtual bool AddEntity (iCelEntity* entity) = 0;
  virtual bool RemoveEntity (iCelEntity* entity) = 0;
  virtual bool RemoveAll () = 0;
  virtual int GetEntityCount () const = 0;
  virtual iCelEntity* GetEntity (int idx) const = 0;
  virtual bool SetStrictCharacteristics (const char* charName, bool strict) = 0;
  virtual bool HasStrictCharacteristics (const char* charName) const = 0;
  virtual bool SetConstraints (const char* charName, float minValue,
  	float maxValue, float totalMaxValue) = 0;
  virtual bool GetConstraints (const char* charName, float& minValue,
  	float& maxValue, float& totalMaxValue) const = 0;
  virtual void RemoveConstraints (const char* charName) = 0;
  virtual void RemoveAllConstraints () = 0;
  virtual float GetCurrentCharacteristic (const char* charName) const = 0;
  virtual void MarkDirty (const char* charName) = 0;
  virtual bool TestConstraints (const char* charName) = 0;
  virtual void Dump () = 0;
};

CEL_PC(iPcInventory, Inventory, pcinventory)

//=======================================================================

struct iPcCharacteristics : public iBase
{
  virtual bool SetCharacteristic (const char* name, float value) = 0;
  virtual bool SetInheritedCharacteristic (const char* name, float factor,
  	float add) = 0;
  virtual float GetCharacteristic (const char* name) const = 0;
  virtual float GetLocalCharacteristic (const char* name) const = 0;
  virtual float GetInheritedCharacteristic (const char* name) const = 0;
  virtual bool ClearCharacteristic (const char* name) = 0;
  virtual bool HasCharacteristic (const char* name) const = 0;
  virtual bool ClearAll () = 0;
  virtual void AddToInventory (iPcInventory* inv) = 0;
  virtual void RemoveFromInventory (iPcInventory* inv) = 0;
  virtual void MarkDirty (const char* charName) = 0;
  virtual bool TestConstraints (const char* charName) = 0;
  virtual void Dump () = 0;
};

CEL_PC(iPcCharacteristics, Characteristics, pccharacteristics)

//=======================================================================

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
  CEL_DATA_VECTOR2,
  CEL_DATA_VECTOR3,
  CEL_DATA_STRING,
  CEL_DATA_PCLASS,
  CEL_DATA_ENTITY,
  CEL_DATA_BUFFER,
  CEL_DATA_ACTION,
  CEL_DATA_COLOR
};

