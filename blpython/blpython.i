#define __CEL__
%module blcelc
%include cspace.i
%{
#include "pl/pl.h"
#include "pl/propfact.h"
#include "pl/propclas.h"
#include "pl/entity.h"
#include "pl/persist.h"
#include "bl/bl.h"
#include "bl/behave.h"
#include "pf/region.h"
#include "pf/camera.h"
#include "pf/common/stdpcimp.h"
#include "pf/mesh.h"
#include "pf/meshsel.h"
#include "pf/timer.h"
#include "pf/solid.h"
#include "pf/gravity.h"
#include "pf/move.h"
#include "pf/inv.h"
#include "pf/chars.h"
#include "blpython/blpython.h"
%}


struct iCelPlLayer : public iBase
{
  //virtual csPtr<iCelEntity> CreateEntity ();
  virtual iCelPropertyClass* CreatePropertyClass (iCelEntity *entity, const char* propname);
  virtual const char* FetchString (csStringID id);

  virtual int GetBehaviourLayerCount () const = 0;
  virtual iCelBlLayer* GetBehaviourLayer (int idx) const = 0;
  virtual iCelBlLayer* FindBehaviourLayer (const char* name) const = 0;  
};

%{
iCelPlLayer *csQueryRegistry_iCelPlLayer(iObjectRegistry *object_reg) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  return pl;
}
%}
iCelPlLayer *csQueryRegistry_iCelPlLayer(iObjectRegistry *object_reg);

struct iCelEntity : public iBase
{
  const char* GetName () const;
  void SetName (const char* n);
  
  virtual void SetBehaviour (iCelBehaviour* ent);
  virtual iCelBehaviour* GetBehaviour ();

  virtual CS_ID GetID () const = 0;

  iCelPropertyClassList* GetPropertyClassList ();

  %addmethods {
    iCelBehaviour *LoadBehaviour(iObjectRegistry *object_reg, const char *name) {
      csRef<iCelBlLayer> bl(CS_QUERY_REGISTRY(object_reg, iCelBlLayer));
      if(!bl.IsValid()) return 0;
      csRef<iCelBehaviour> bh(bl->CreateBehaviour(self, name));
      if(!bh.IsValid()) return 0;
      self->SetBehaviour(bh);
      return bh;
    }
  }
};

%{
iCelEntity *celCreateEntity(iObjectRegistry *object_reg, const char *name) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY(object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelEntity> en(pl->CreateEntity());
  if(!en.IsValid()) return 0;
  en->SetName(name);
  en->IncRef();
  return en;
}
%}
iCelEntity *celCreateEntity(iObjectRegistry *object_reg, const char *name);

%{
iCelEntity *scfQueryInterface_iCelEntity(iBase *base) {
  csRef<iCelEntity> ent(SCF_QUERY_INTERFACE (base, iCelEntity));
  return ent;
}
%}
iCelEntity *scfQueryInterface_iCelEntity(iBase *base);

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

%{
iCelEntityList *celFindNearbyEntities(iObjectRegistry *object_reg, iSector *sector, csVector3 pos, float radius) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelEntityList> entlist(pl->FindNearbyEntities(sector, pos, radius));
  entlist->IncRef();
  return entlist;
}
%}
iCelEntityList *celFindNearbyEntities(iObjectRegistry *object_reg, iSector *sector, csVector3 pos, float radius);

struct iCelBlLayer : public iBase
{
  virtual const char* GetName () const = 0;

  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity,
  	const char* name) = 0;
};

%{
iCelBlLayer *csQueryRegistry_iCelBlLayer(iObjectRegistry *object_reg) {
  csRef<iCelBlLayer> bl(CS_QUERY_REGISTRY (object_reg, iCelBlLayer));
  return bl;
}
%}
iCelBlLayer *csQueryRegistry_iCelBlLayer(iObjectRegistry *object_reg);

struct iCelBehaviour : public iBase
{
  virtual const char* GetName () const = 0;

  virtual iCelBlLayer* GetBehaviourLayer () const = 0;

  virtual bool SendMessage(const char* msg_id, iBase *msg_info) = 0;
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

%{
iPcRegion *celQueryPC_iPcRegion(iCelPropertyClassList *pclist) {
  csRef<iPcRegion> pcregion(CEL_QUERY_PROPCLASS(pclist, iPcRegion));
  pcregion->IncRef ();
  return pcregion;
}
%}
iPcRegion *celQueryPC_iPcRegion(iCelPropertyClassList *pclist);

struct iCelPropertyClassList : public iBase
{
  virtual int GetCount ();
  virtual iCelPropertyClass* Get (int n);
  virtual int Find (iCelPropertyClass* obj);
  virtual iCelPropertyClass* FindByName (const char *Name);
  virtual iBase* FindByInterface (scfInterfaceID id, int version);
};

struct iPcRegion : public iBase
{
  virtual void SetWorldFile (const char* vfsdir, const char* name) = 0;
  virtual const char* GetWorldDir () const = 0;
  virtual const char* GetWorldFile () const = 0;
  virtual void SetRegionName (const char* name) = 0;
  virtual const char* GetRegionName () const = 0;
  virtual bool Load () = 0;
  virtual void Unload () = 0;
  virtual iSector* FindSector (const char* sectorname) = 0;
  virtual iSector* GetStartSector (const char* name = 0) = 0;
  virtual csVector3 GetStartPosition (const char* name = 0) = 0;
  virtual void PointCamera (iPcCamera* pccamera, const char* name = 0) = 0;
  
  %addmethods {
    bool LoadWorld(const char *vfsdir, const char *name) {
      self->SetWorldFile(vfsdir, name);
      return self->Load();
    }
  }

};

%{
iPcRegion *celCreateRegion(iObjectRegistry *object_reg, iCelEntity *entity, const char *name) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, "pcregion"));
  if(!pc.IsValid()) return 0;
  csRef<iPcRegion> pcregion(SCF_QUERY_INTERFACE(pc, iPcRegion));
  if(!pcregion.IsValid()) return 0;
  pcregion->SetRegionName(name);
  return pcregion;
}
%}
iPcRegion *celCreateRegion(iObjectRegistry *object_reg, iCelEntity *entity, const char *name);

%{
iPcRegion *scfQuery_iPcRegion(iCelPropertyClass *pc) { 
  csRef<iPcRegion> pcregion(SCF_QUERY_INTERFACE(pc, iPcRegion));
  pcregion->IncRef ();
  return pcregion;
}
%}
iPcRegion *scfQuery_iPcRegion(iCelPropertyClass *pc);

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

  %addmethods {

  }

};

%{
iPcCamera *celCreateCamera(iObjectRegistry *object_reg, iCelEntity *world, iCelEntity *entity, const char *name) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, name));
  if(!pc.IsValid()) return 0;
  csRef<iPcCamera> pccam(SCF_QUERY_INTERFACE(pc, iPcCamera));
  if(!pccam.IsValid()) return 0;
  csRef<iPcRegion> pcregion(CEL_QUERY_PROPCLASS(world->GetPropertyClassList(), iPcRegion));
  if(!pcregion.IsValid()) return 0;
  if(pccam->SetRegion(pcregion)) {
    return pccam;
  } else {
    return 0;
  }
}
%}
iPcCamera *celCreateCamera(iObjectRegistry *object_reg, iCelEntity *world, iCelEntity *entity, const char *name);

%{
iPcCamera *scfQuery_iPcCamera(iCelPropertyClass *pc) {
  csRef<iPcCamera> pccamera(SCF_QUERY_INTERFACE(pc, iPcCamera));
  return pccamera;
}
%}
iPcCamera *scfQuery_iPcCamera(iCelPropertyClass *pc);

%{
iPcMesh *scfQuery_iPcMesh(iCelPropertyClass *pc) {
  csRef<iPcMesh> pcMesh(SCF_QUERY_INTERFACE(pc, iPcMesh));
  return pcMesh;
}
%}
iPcMesh *scfQuery_iPcMesh(iCelPropertyClass *pc);

%{
iPcTimer *scfQuery_iPcTimer(iCelPropertyClass *pc) {
  csRef<iPcTimer> pcTimer(SCF_QUERY_INTERFACE(pc, iPcTimer));
  return pcTimer;
}
%}
iPcTimer *scfQuery_iPcTimer(iCelPropertyClass *pc);

%{
iPcInventory *scfQuery_iPcInventory(iCelPropertyClass *pc) {
  csRef<iPcInventory> pcInventory(SCF_QUERY_INTERFACE(pc, iPcInventory));
  return pcInventory;
}
%}
iPcInventory *scfQuery_iPcInventory(iCelPropertyClass *pc);

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
  virtual void SetDragPlaneNormal (const csVector3& drag_normal, bool camera_space) = 0;
  virtual void GetDragPlaneNormal (csVector3& drag_normal, bool& camera_space) const = 0;
  virtual void SetSendmoveEvent (bool mov) = 0;
  virtual bool HasSendmoveEvent () const = 0;
  virtual void SetSendupEvent (bool su) = 0;
  virtual bool HasSendupEvent () const = 0;
  virtual void SetSenddownEvent (bool sd) = 0;
  virtual bool HasSenddownEvent () const = 0;
};

%{
iPcMeshSelect *celPcMeshSelect(iObjectRegistry *object_reg, iCelEntity *entity) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, "pcmeshselect"));
  if(!pc.IsValid()) return 0;
  csRef<iPcMeshSelect> pcmeshselect(SCF_QUERY_INTERFACE(pc, iPcMeshSelect));
  if(!pcmeshselect.IsValid()) return 0;
  return pcmeshselect;
}
%}
iPcMeshSelect *celPcMeshSelect(iObjectRegistry *object_reg, iCelEntity *entity);

struct iPcMesh : public iBase
{
  %name(LoadMesh) virtual void SetMesh (const char* factname, const char* filename) = 0;
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

%{
iPcMesh *celPcMesh(iObjectRegistry *object_reg, iCelEntity *entity) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, "pcmesh"));
  if(!pc.IsValid()) return 0;
  csRef<iPcMesh> pcmesh(SCF_QUERY_INTERFACE(pc, iPcMesh));
  if(!pcmesh.IsValid()) return 0;
  return pcmesh;
}
%}
iPcMesh *celPcMesh(iObjectRegistry *object_reg, iCelEntity *entity);

struct iPcTimer : public iBase
{
  virtual void WakeUp (csTicks t, bool repeat) = 0;
  virtual void Clear () = 0;
};

%{
iPcTimer *celPcTimer(iObjectRegistry *object_reg, iCelEntity *entity) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, "pctimer"));
  if(!pc.IsValid()) return 0;
  csRef<iPcTimer> pctimer(SCF_QUERY_INTERFACE(pc, iPcTimer));
  if(!pctimer.IsValid()) return 0;
  return pctimer;
}
%}
iPcTimer *celPcTimer(iObjectRegistry *object_reg, iCelEntity *entity);

struct iPcSolid : public iBase
{
  virtual void SetMesh (iPcMesh* mesh) = 0;
  virtual iPcMesh* GetMesh () const = 0;
  virtual iCollider* GetCollider () = 0;
};

%{
iPcSolid *celPcSolid(iObjectRegistry *object_reg, iCelEntity *entity) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, "pcsolid"));
  if(!pc.IsValid()) return 0;
  csRef<iPcSolid> pcsolid(SCF_QUERY_INTERFACE(pc, iPcSolid));
  if(!pcsolid.IsValid()) return 0;
  return pcsolid;
}
%}
iPcSolid *celPcSolid(iObjectRegistry *object_reg, iCelEntity *entity);

struct iPcGravity : public iBase
{
  %name(CreateGravityColliderFromMesh) virtual void CreateGravityCollider (iPcMesh* mesh) = 0;
  virtual void CreateGravityCollider (const csVector3& dim, const csVector3& offs) = 0;
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

%{
iPcGravity *celPcGravity(iObjectRegistry *object_reg, iCelEntity *entity) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, "pcgravity"));
  if(!pc.IsValid()) return 0;
  csRef<iPcGravity> pcgravity(SCF_QUERY_INTERFACE(pc, iPcGravity));
  if(!pcgravity.IsValid()) return 0;
  return pcgravity;
}
%}
iPcGravity *celPcGravity(iObjectRegistry *object_reg, iCelEntity *entity);

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

%{
iPcMovable *celPcMovable(iObjectRegistry *object_reg, iCelEntity *entity) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, "pcmovable"));
  if(!pc.IsValid()) return 0;
  csRef<iPcMovable> pcmovable(SCF_QUERY_INTERFACE(pc, iPcMovable));
  if(!pcmovable.IsValid()) return 0;
  return pcmovable;
}
%}
iPcMovable *celPcMovable(iObjectRegistry *object_reg, iCelEntity *entity);

struct iPcInventory : public iBase
{
  virtual bool AddEntity (iCelEntity* entity) = 0;
  virtual bool RemoveEntity (iCelEntity* entity) = 0;
  virtual bool RemoveAll () = 0;
  virtual int GetEntityCount () const = 0;
  virtual iCelEntity* GetEntity (int idx) const = 0;
  virtual bool SetStrictCharacteristics (const char* charName, bool strict) = 0;
  virtual bool HasStrictCharacteristics (const char* charName) const = 0;
  virtual bool SetConstraints (const char* charName, float minValue, float maxValue, float totalMaxValue) = 0;
  virtual bool GetConstraints (const char* charName, float& minValue, float& maxValue, float& totalMaxValue) const = 0;
  virtual void RemoveConstraints (const char* charName) = 0;
  virtual void RemoveAllConstraints () = 0;
  virtual float GetCurrentCharacteristic (const char* charName) const = 0;
  virtual void MarkDirty (const char* charName) = 0;
  virtual bool TestConstraints (const char* charName) = 0;
  virtual void Dump () = 0;
};

%{
iPcInventory *celPcInventory(iObjectRegistry *object_reg, iCelEntity *entity) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, "pcinventory"));
  if(!pc.IsValid()) return 0;
  csRef<iPcInventory> pcinv(SCF_QUERY_INTERFACE(pc, iPcInventory));
  if(!pcinv.IsValid()) return 0;
  return pcinv;
}
%}
iPcInventory *celPcInventory(iObjectRegistry *object_reg, iCelEntity *entity);

struct iPcCharacteristics : public iBase
{
  virtual bool SetCharacteristic (const char* name, float value) = 0;
  virtual bool SetInheritedCharacteristic (const char* name, float factor, float add) = 0;
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

%{
iPcCharacteristics *celPcCharacteristics(iObjectRegistry *object_reg, iCelEntity *entity) {
  csRef<iCelPlLayer> pl(CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  if(!pl.IsValid()) return 0;
  csRef<iCelPropertyClass> pc(pl->CreatePropertyClass(entity, "pccharacteristics"));
  if(!pc.IsValid()) return 0;
  csRef<iPcCharacteristics> pccharacteristics(SCF_QUERY_INTERFACE(pc, iPcCharacteristics));
  if(!pccharacteristics.IsValid()) return 0;
  return pccharacteristics;
}
%}
iPcCharacteristics *celPcCharacteristics(iObjectRegistry *object_reg, iCelEntity *entity);

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
