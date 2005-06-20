#define __CEL__
%module blcelc

%import "ivaria/cspace.i"

%{
#include <crystalspace.h>
#include "celtool/initapp.h"
#include "physicallayer/pl.h"
#include "physicallayer/propfact.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/behave.h"
#include "propclass/region.h"
#include "propclass/camera.h"
#include "propclass/defcam.h"
#include "propclass/simpcam.h"
#include "propclass/mesh.h"
#include "propclass/meshsel.h"
#include "propclass/timer.h"
#include "propclass/solid.h"
#include "propclass/gravity.h"
#include "propclass/move.h"
#include "propclass/inv.h"
#include "propclass/chars.h"
#include "propclass/linmove.h"
#include "propclass/actormove.h"
#include "propclass/input.h"
#include "propclass/dynmove.h"
#include "propclass/billboard.h"
#include "propclass/mechsys.h"
#include "propclass/mechthruster.h"
#include "propclass/tooltip.h"
#include "plugins/behaviourlayer/python/blpython.h"
#include "tools/billboard.h"
#include "propclass/zone.h"
%}

// @@@ UGLY: Duplicated from CS. Find a better way.
%inline %{

  struct _csPyEventHandler : public iEventHandler
  {
    SCF_DECLARE_IBASE;
    _csPyEventHandler (PyObject * obj) : _pySelf(obj)
    {
      SCF_CONSTRUCT_IBASE(0);
      IncRef();
    }
    virtual ~_csPyEventHandler ()
    {
      SCF_DESTRUCT_IBASE();
      DecRef();
    }
    virtual bool HandleEvent (iEvent & event)
    {
      PyObject * event_obj = SWIG_NewPointerObj(
        (void *) &event, SWIG_TypeQuery("iEvent *"), 0
      );
      PyObject * result = PyObject_CallMethod(_pySelf, "HandleEvent", "(O)",
        event_obj
      );
      Py_DECREF(event_obj);
      if (!result)
      {
        return false;
      }
      bool res = PyInt_AsLong(result);
      Py_DECREF(result);
      return res;
    }
  private:
    PyObject * _pySelf;
  };

%}

//=============================================================================
// Helper macros.
//=============================================================================

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

//=============================================================================
// Published interfaces and functions.
//=============================================================================

%ignore celInitializer::RequestPlugins;
%ignore celInitializer::RequestPluginsV;
%include "celtool/initapp.h"

//-----------------------------------------------------------------------------

%include "physicallayer/pl.h"
%inline %{
iCelPlLayer *csQueryRegistry_iCelPlLayer (iObjectRegistry *object_reg)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  return pl;
}
%}

//-----------------------------------------------------------------------------

%include "physicallayer/entity.h"
%extend iCelEntity {
  iCelBehaviour *CreateBehaviour(iCelBlLayer *bl, const char *name)
  {
    csRef<iCelBehaviour> bh(bl->CreateBehaviour(self, name));
    if (!bh.IsValid()) return 0;
    return bh;
  }
}

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

//-----------------------------------------------------------------------------

%include "behaviourlayer/bl.h"
%inline %{
iCelBlLayer *csQueryRegistry_iCelBlLayer (iObjectRegistry *object_reg)
{
  csRef<iCelBlLayer> bl = CS_QUERY_REGISTRY (object_reg, iCelBlLayer);
  return bl;
}
%}

//-----------------------------------------------------------------------------

%ignore iCelBehaviour::SendMessageV;
%include "behaviourlayer/behave.h"
%extend iCelBehaviour {
  PyObject *GetPythonObject()
  {
    PyObject* obj = (PyObject*)(self->GetInternalObject());
    Py_INCREF (obj);
    return obj;
  }
}

//-----------------------------------------------------------------------------

%ignore iCelPropertyClass::SetProperty;
%include "physicallayer/propclas.h"
%extend iCelPropertyClass {
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

//-----------------------------------------------------------------------------

%include "propclass/dynmove.h"
CEL_PC(iPcDynamicSystem, DynamicSystem, pcdynsys)
CEL_PC_QUERY_CLASSLIST(iPcDynamicSystem)
CEL_PC(iPcDynamicBody, DynamicBody, pcdynbody)
CEL_PC_QUERY_CLASSLIST(iPcDynamicBody)

//-----------------------------------------------------------------------------

%include "propclass/mechsys.h"
CEL_PC(iPcMechanicsSystem, MechanicsSystem, pcmechsys)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsSystem)
CEL_PC(iPcMechanicsObject, MechanicsObject, pcmechobject)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsObject)

%include "propclass/mechthruster.h"
CEL_PC(iPcMechanicsThruster, MechanicsThrusterReactionary,
	pcmechthrustreactionary)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsThruster)
CEL_PC(iPcMechanicsBalancedGroup, MechanicsBalancedGroup, pcmechbalancedgroup)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsBalancedGroup)
CEL_PC(iPcMechanicsThrusterController, MechanicsThrusterController,
	pcmechthrustercontroller)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsThrusterController)

//-----------------------------------------------------------------------------

%include "tools/billboard.h"

%include "propclass/billboard.h"
CEL_PC(iPcBillboard, Billboard, pcbillboard)
CEL_PC_QUERY_CLASSLIST(iPcBillboard)

//-----------------------------------------------------------------------------

%include "propclass/region.h"
%extend iPcRegion {
  bool LoadWorld (const char *vfsdir, const char *name)
  {
    self->SetWorldFile (vfsdir, name);
    return self->Load ();
  }
}

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

//-----------------------------------------------------------------------------

%include "propclass/zone.h"
CEL_PC(iPcZoneManager, ZoneManager, pczonemanager)

//-----------------------------------------------------------------------------

%include "propclass/input.h"
CEL_PC(iPcCommandInput, CommandInput, pccommandinput)

//-----------------------------------------------------------------------------

%include "propclass/linmove.h"
CEL_PC(iPcLinearMovement, LinearMovement, pclinearmovement)

//-----------------------------------------------------------------------------
%include "propclass/actormove.h"
CEL_PC(iPcActorMove, ActorMove, pcactormove)

//-----------------------------------------------------------------------------

%include "propclass/camera.h"
CEL_PC(iPcCamera, Camera, pccamera)

%include "propclass/defcam.h"
CEL_PC(iPcDefaultCamera, DefaultCamera, pcdefaultcamera)

%include "propclass/simpcam.h"
CEL_PC(iPcSimpleCamera, SimpleCamera, pcsimplecamera)

//-----------------------------------------------------------------------------

%include "propclass/meshsel.h"
CEL_PC(iPcMeshSelect, MeshSelect, pcmeshselect)

//-----------------------------------------------------------------------------

%rename(LoadMesh) iPcMesh::SetMesh;
%include "propclass/mesh.h"
CEL_PC(iPcMesh, Mesh, pcmesh)

//-----------------------------------------------------------------------------

%include "propclass/timer.h"
CEL_PC(iPcTimer, Timer, pctimer)

//-----------------------------------------------------------------------------

%include "propclass/solid.h"
CEL_PC(iPcSolid, Solid, pcsolid)

//-----------------------------------------------------------------------------

%rename(CreateGravityColliderFromMesh) iPcGravity::CreateGravityCollider;
%include "propclass/gravity.h"
CEL_PC(iPcGravity, Gravity, pcgravity)

//-----------------------------------------------------------------------------

%rename(SetPos) iPcMovable::Move;
%include "propclass/move.h"
CEL_PC(iPcMovable, Movable, pcmovable)

//-----------------------------------------------------------------------------

%include "propclass/inv.h"
CEL_PC(iPcInventory, Inventory, pcinventory)

//-----------------------------------------------------------------------------

%include "propclass/chars.h"
CEL_PC(iPcCharacteristics, Characteristics, pccharacteristics)

//-----------------------------------------------------------------------------

%include "propclass/tooltip.h"
CEL_PC(iPcTooltip, ToolTip, pctooltip)

//-----------------------------------------------------------------------------

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
  CEL_DATA_ACTION,
  CEL_DATA_COLOR,
  CEL_DATA_IBASE
};
