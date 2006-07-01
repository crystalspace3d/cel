#define __CEL__
%module blcelc

%import "ivaria/cspace.i"

%{
#include <crystalspace.h>
#include "celtool/initapp.h"
#include "celtool/stdparams.h"
#include "physicallayer/datatype.h"
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
#include "propclass/billboard.h"
#include "propclass/mechsys.h"
#include "propclass/sound.h"
#include "propclass/mechthruster.h"
#include "propclass/tooltip.h"
#include "propclass/prop.h"
#include "propclass/hover.h"
#include "propclass/craft.h"
#include "plugins/behaviourlayer/python/blpython.h"
#include "tools/billboard.h"
#include "propclass/zone.h"
%}
%inline %{
  struct _csPyEventHandler;
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

%define CEL_PC_GETSET(pcType, funcName,pcname)
%inline %{
pcType * funcName (iCelPlLayer *pl, iCelEntity *entity)
{
  csRef<pcType> pclm = CEL_QUERY_PROPCLASS (
    entity->GetPropertyClassList (), pcType);
  if (pclm.IsValid()) return pclm;
  csRef<iCelPropertyClass> pc = pl->CreatePropertyClass(entity, #pcname );
  if (!pc.IsValid()) return 0;
  pclm = SCF_QUERY_INTERFACE(pc, pcType);
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
CEL_PC_GETSET(pcType, celGetSet ## funcBaseName, pcname)
CEL_PC_GET(pcType, celGet ## funcBaseName)
CEL_PC_QUERY(pcType)
%enddef

%define GETTER_METHOD(classname,varname,getfunc)
%extend classname {
 %pythoncode %{
  __swig_getmethods__[varname] = _blcelc. ## classname ## _ ## getfunc
  %}
}
%enddef

%define SETTER_METHOD(classname,varname,setfunc)
%extend classname {
 %pythoncode %{
  __swig_setmethods__[varname] = _blcelc. ## classname ## _ ## setfunc
  %}
}
%enddef

%define GETSET_METHODS(classname, varname, getfunc, setfunc)
SETTER_METHOD(classname, varname, setfunc)
GETTER_METHOD(classname, varname, getfunc)
%enddef

%define CELLIST_METHODS(typename)
	size_t __len__ () { return self->GetCount(); }
	typename *__getitem__(size_t n) {return self->Get(n);}
	void __delitem__(size_t n) { self->Remove(n); }
	void append( typename *e) { self->Add(e); }
	%pythoncode %{
	def content_iterator(self):
		for idx in xrange(len(self)):
			yield self.Get(idx)
	def __iter__(self):
		return self.content_iterator()
	%}
%enddef


//=============================================================================
// Published interfaces and functions.
//=============================================================================

%ignore celInitializer::RequestPlugins;
%ignore celInitializer::RequestPluginsV;
%include "celtool/initapp.h"

//-----------------------------------------------------------------------------

%ignore celData::GetDebugInfo;
%include "datatype.i"
%include "physicallayer/datatype.h"

//-----------------------------------------------------------------------------

%include "physicallayer/pl.h"
%inline %{
iCelPlLayer *csQueryRegistry_iCelPlLayer (iObjectRegistry *object_reg)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  return pl;
}
%}

GETTER_METHOD(iCelPlLayer,"entitytemplatecount",GetEntityTemplateCount)
GETTER_METHOD(iCelPlLayer,"entitycount",GetEntityCount)
GETTER_METHOD(iCelPlLayer,"propertyclassfactorycount",GetPropertyClassFactoryCount)
GETTER_METHOD(iCelPlLayer,"behaviourlayercount",GetBehaviourLayerCount)

GETTER_METHOD(iCelEntityTracker,"name",GetName)

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
GETSET_METHODS(iCelEntity,"name",GetName,SetName)
GETSET_METHODS(iCelEntity,"id",GetID,SetID)
GETSET_METHODS(iCelEntity,"behaviour",GetBehaviour,SetBehaviour)

GETTER_METHOD(iCelEntity,"propertyclasslist",GetPropertyClassList)
GETTER_METHOD(iCelEntity,"classes",GetClasses)

%extend iCelEntityList {
	CELLIST_METHODS(iCelEntity)
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
	iSector *sector, csVector3 pos, float radius, bool do_invisible=false)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl.IsValid()) return 0;
  csRef<iCelEntityList> entlist = pl->FindNearbyEntities (sector, pos, radius, do_invisible);
  entlist->IncRef();
  return entlist;
}
%}

%inline %{
iCelEntityList *celFindNearbyEntities (iObjectRegistry *object_reg,
       iSector *sector, csVector3 pos, csVector3 dest, bool do_invisible=false)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl.IsValid()) return 0;
  csRef<iCelEntityList> entlist = pl->FindNearbyEntities (sector, pos, dest, do_invisible);
  entlist->IncRef();
  return entlist;
}
%}

%inline %{
iCelEntityList *celFindNearbyEntities (iObjectRegistry *object_reg,
       iSector *sector, csBox3 box, bool do_invisible=false)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl.IsValid()) return 0;
  csRef<iCelEntityList> entlist = pl->FindNearbyEntities (sector, box, do_invisible);
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


//iParameterBlock Extensions
%include "parblock.i"
%extend iCelBehaviour {
  PyObject *GetPythonObject()
  {
    PyObject* obj = (PyObject*)(self->GetInternalObject());
    Py_INCREF (obj);
    return obj;
  }
  PyObject *SendMessage(const char* msg_id, iCelParameterBlock* params)
  {
       celData ret;
       if(self->SendMessage (msg_id,0,ret,params))
       {
         CELDATA_RETURN((&ret));
	 return obj;
       }
       else
	 return Py_None;
  }
}
GETTER_METHOD(iCelBehaviour,"name",GetName)

//-----------------------------------------------------------------------------

%ignore celVariableParameterBlock::GetParameter (size_t idx);
%ignore celGenericParameterBlock::GetParameter (size_t idx);
%include "celtool/stdparams.h"

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
GETTER_METHOD(iCelPropertyClass,"name",GetName)
GETSET_METHODS(iCelPropertyClass,"tag",GetTag,SetTag)
GETSET_METHODS(iCelPropertyClass,"entity",GetEntity,SetEntity)
%extend iCelPropertyClassList {
	CELLIST_METHODS(iCelPropertyClass)
}
//-----------------------------------------------------------------------------

%include "propclass/mechsys.h"
CEL_PC(iPcMechanicsSystem, MechanicsSystem, pcmechsys)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsSystem)
CEL_PC(iPcMechanicsObject, MechanicsObject, pcmechobject)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsObject)
CEL_PC(iPcMechanicsJoint, MechanicsJoint, pcmechjoint)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsJoint)

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
GETSET_METHODS(iPcActorMove,"forward",IsMovingForward,Forward)
GETSET_METHODS(iPcActorMove,"backward",IsMovingBackward,Backward)
GETSET_METHODS(iPcActorMove,"strafeleft",IsStrafingLeft,StrafeLeft)
GETSET_METHODS(iPcActorMove,"straferight",IsStrafingRight,StrafeRight)
GETSET_METHODS(iPcActorMove,"rotateleft",IsRotatingLeft,RotateLeft)
GETSET_METHODS(iPcActorMove,"rotateright",IsRotatingRight,RotateRight)
GETSET_METHODS(iPcActorMove,"run",IsRunning,Run)
GETSET_METHODS(iPcActorMove,"autorun",IsAutoRunning,AutoRun)
GETSET_METHODS(iPcActorMove,"movementspeed",GetMovementSpeed,SetMovementSpeed)
GETSET_METHODS(iPcActorMove,"runningspeed",GetRunningSpeed,SetRunningSpeed)
GETSET_METHODS(iPcActorMove,"rotationspeed",GetRotationSpeed,SetRotationSpeed)
GETSET_METHODS(iPcActorMove,"jumpingvelocity",GetJumpingVelocity,SetJumpingVelocity)

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

%include "propclass/sound.h"
CEL_PC(iPcSoundSource, SoundSource, pcsoundsource)
CEL_PC(iPcSoundListener, SoundListener, pcsoundlistener)

//-----------------------------------------------------------------------------

%include "propclass/prop.h"
CEL_PC(iPcProperties, Properties, pcproperties)

//-----------------------------------------------------------------------------

%include "propclass/hover.h"
CEL_PC(iPcHover, Hover, pchover)

//-----------------------------------------------------------------------------

%include "propclass/craft.h"
CEL_PC(iPcCraftController, CraftController, pccraft)

//-----------------------------------------------------------------------------



