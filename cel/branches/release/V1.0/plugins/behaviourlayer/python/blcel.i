#define __CEL__
%module blcelc
%import "bindings/cspace.i"
%include "cel_attributes.i"
%{
#include <crystalspace.h>
#include "celtool/initapp.h"
#include "celtool/stdparams.h"
#include "physicallayer/datatype.h"
#include "physicallayer/pl.h"
#include "physicallayer/propfact.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
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
#include "propclass/projectile.h"
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
#include "propclass/mover.h"
#include "propclass/tooltip.h"
#include "propclass/prop.h"
#include "propclass/hover.h"
#include "propclass/craft.h"
#include "propclass/wheeled.h"
#include "propclass/damage.h"
#include "plugins/behaviourlayer/python/blpython.h"
#include "tools/billboard.h"
#include "tools/celconsole.h"
#include "propclass/zone.h"
%}
%inline %{
  struct _csPyEventHandler;
  extern PyObject *    // from crystalspace_python
  _csRef_to_Python (const csRef<iBase> & ref, void * ptr, const char * name);

%}

//=============================================================================
// Interfaces that need csRef,csPtr,csRefArray
// uses INTERFACE_POST and INTERFACE_PRE from cspace.i
//=============================================================================

%define CEL_APPLY_FOR_EACH_INTERFACE
  INTERFACE_APPLY(iCelEntity)
  INTERFACE_APPLY(iCelEntityList)
%enddef

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

//-----------------------------------------------------------------------------
%define CELLIST_METHODS(classname,typename)
	PYLIST_BASE_FUNCTIONS(classname,typename,size_t,GetCount,Get,Add,Remove,Find)
	PYLIST_BYNAME_FUNCTIONS(classname,typename,FindByName)
%enddef

//-----------------------------------------------------------------------------

// this serves to wrap list methods in classes as a fake array.
%define CEL_FAKE_ARRAY(pref,contenttype,countmethod,getmethod,findmethod,delmethod,addmethod)
%pythoncode %{
class contenttype ## pref ## FakeArray:
	def __init__(self,parent): self.parent = parent
	def __contains__(self,obj):
		if self.parent.findmethod(obj): return True
		else: return False
	def __repr__(self): return "List of "+str(#contenttype)
	def __len__(self): return self.parent.countmethod()
	def __delitem__(self,val):
		if type(val) == type(""):
			obj = self.parent.findmethod(val)
			if obj: return self.parent.delmethod(obj)
			else: raise IndexError(val+" not in list")
		else: return self.parent.delmethod(val)
	def __noappend__(self,obj):
		print "Append not supported by this list"
	def content_iterator(self):
		for idx in xrange(len(self)):
			yield self.parent.getmethod(idx)
	def __iter__(self): return self.content_iterator()
	def __getitem__(self,val):
		if type(val) == type(""): return self.parent.findmethod(val)
		else: return self.parent.getmethod(val)
	def append(self,obj): return self.parent. ## addmethod(obj)
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

#undef INTERFACE_APPLY
#define INTERFACE_APPLY(x) INTERFACE_PRE(x)
CEL_APPLY_FOR_EACH_INTERFACE

//-----------------------------------------------------------------------------

%cel_attribute(iCelEntityTracker,const char*,Name,GetName)

%include "physicallayer/pl.h"
%inline %{
iCelPlLayer *csQueryRegistry_iCelPlLayer (iObjectRegistry *object_reg)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  return pl;
}
%}

// extension to create a parameter block from a dict, list or tuple
%extend iCelPlLayer {
	%pythoncode %{
	def CreateParameterBlock(self,valdict):
		"""Create a celGenericParameterBlock from a dict, list or
		tuple"""
		parblock = celGenericParameterBlock(len(valdict))
		for idx,valkey in enumerate(valdict):
			keyid = self.FetchStringID("cel.parameter."+valkey)
			parblock.SetParameterDef (idx,keyid,valkey)
			if type(valdict) == type({}):
				parblock[keyid] = valdict[valkey]
		return parblock	%}
}
// fake arrays to handle stuff managed by the physical layer.
// iCelEntityTemplatePlFakeArray
CEL_FAKE_ARRAY(Pl,iCelEntityTemplate,GetEntityTemplateCount,
	GetEntityTemplateByIndex,FindEntityTemplate,RemoveEntityTemplate,__noappend__)

// iCelEntityPlFakeArray
CEL_FAKE_ARRAY(Pl,iCelEntity,GetEntityCount,GetEntityByIndex,FindEntity,RemoveEntity,__noappend__)

// iCelBlLayerPlFakeArray
CEL_FAKE_ARRAY(Pl,iCelBlLayer,GetBehaviourLayerCount,GetBehaviourLayer,
	       FindBehaviourLayer,UnregisterBehaviourLayer,
	       RegisterBehaviourLayer)

// iCelPropertyClassFactoryPlFakeArray
CEL_FAKE_ARRAY(Pl,iCelPropertyClassFactory,GetPropertyClassFactoryCount,
	       GetPropertyClassFactory,FindPropertyClassFactory,
	       UnregisterPropertyClassFactory,LoadPropertyClassFactory)

%extend iCelPlLayer {
	%pythoncode %{
	def GetEntities(self): return iCelEntityPlFakeArray(self)
	def GetEntityTemplates(self): return iCelEntityTemplatePlFakeArray(self)
	def GetPcFactories(self): return iCelPropertyClassFactoryPlFakeArray(self)
	def GetBehaviourLayers(self): return iCelBlLayerPlFakeArray(self)
	__swig_getmethods__["EntityTemplates"] = lambda self: self.GetEntityTemplates()
	__swig_getmethods__["PcFactories"] = lambda self: self.GetPcFactories()
	__swig_getmethods__["BehaviourLayers"] = lambda self: self.GetBehaviourLayers()
	__swig_getmethods__["Entities"] = lambda self: self.GetEntities() %}
}

//-----------------------------------------------------------------------------

%cel_attribute(iCelEntity,const char*,Name)
%cel_attribute(iCelEntity,uint,ID)
%cel_attribute(iCelEntity,iCelBehaviour*,Behaviour)
%cel_attribute(iCelEntity,iCelPropertyClassList*,PropertyClassList,GetPropertyClassList)
%cel_attribute(iCelEntity,csSet<csStringID>&,Classes,GetClasses)

%include "physicallayer/entity.h"
%extend iCelEntity {
  iCelBehaviour *CreateBehaviour(iCelBlLayer *bl, const char *name)
  {
    csRef<iCelBehaviour> bh(bl->CreateBehaviour(self, name));
    if (!bh.IsValid()) return 0;
    return bh;
  }
}
CELLIST_METHODS(iCelEntityList,iCelEntity)

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

%cel_attribute(iCelEntityTemplate,const char*,Name)
%cel_attribute(iCelEntityTemplate,const char*,Behaviour,GetBehaviour)
%cel_attribute(iCelEntityTemplate,const char*,BehaviourLayer,GetBehaviourLayer)
%cel_attribute(iCelEntityTemplate,csSet<csStringID>&,Classes,GetClasses)
%include "physicallayer/entitytpl.h"

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

%cel_attribute(iCelBehaviour,const char*,Name,GetName)
%cel_attribute(iCelBehaviour,iCelBlLayer*,BehaviourLayer,GetBehaviourLayer)
%ignore iCelBehaviour::SendMessageV;
%ignore iCelBehaviour::GetInternalObject;
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
  PyObject *SendMessage(const char* msg_id, iCelPropertyClass* prop, iCelParameterBlock* params)
  {
       celData ret;
       if(self->SendMessage (msg_id,prop,ret,params))
       {
         CELDATA_RETURN((&ret));
	 return obj;
       }
       else
         Py_INCREF(Py_None);
	 return Py_None;
  }
}

//-----------------------------------------------------------------------------

%ignore celVariableParameterBlock::GetParameter (size_t idx);
%ignore celGenericParameterBlock::GetParameter (size_t idx);
%include "celtool/stdparams.h"

//-----------------------------------------------------------------------------

%cel_attribute(iCelPropertyClass,const char*,Name,GetName)
%cel_attribute(iCelPropertyClass,const char*,Tag)
%cel_attribute(iCelPropertyClass,iCelEntity*,Entity)
%cel_attribute(iCelPropertyClassFactory,const char*,Name,GetName)
%ignore iCelPropertyClass::SetProperty;
%include "physicallayer/propfact.h"
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
CELLIST_METHODS(iCelPropertyClassList,iCelPropertyClass)

//-----------------------------------------------------------------------------

%cel_attribute(iPcMechanicsSystem,iDynamicSystem*,DynamicSystem)
%cel_attribute(iPcMechanicsSystem,float,StepTime)
%cel_attribute(iPcMechanicsSystem,csVector3,Gravity)
%cel_attribute(iPcMechanicsSystem,float,SimulationSpeed)
%include "propclass/mechsys.h"
CEL_PC(iPcMechanicsSystem, MechanicsSystem, pcmechsys)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsSystem)

%cel_attribute(iPcMechanicsObject,iPcMesh*,Mesh)
%cel_attribute(iPcMechanicsObject,iPcLight*,Light)
%cel_attribute(iPcMechanicsObject,iPcCamera*,Camera)
%cel_attribute(iPcMechanicsObject,iPcMechanicsSystem*,MechanicsSystem)
%cel_attribute(iPcMechanicsObject,iRigidBody*,Body,GetBody)
%cel_attribute(iPcMechanicsObject,float,Friction)
%cel_attribute(iPcMechanicsObject,float,Mass)
%cel_attribute(iPcMechanicsObject,float,Elasticity)
%cel_attribute(iPcMechanicsObject,float,Density)
%cel_attribute(iPcMechanicsObject,float,Softness)
%cel_attribute(iPcMechanicsObject,csVector3&,Lift)
%cel_attribute(iPcMechanicsObject,float,Drag)
%cel_attribute(iPcMechanicsObject,csVector3,LinearVelocity)
%cel_attribute(iPcMechanicsObject,csVector3,AngularVelocity)
%cel_attribute(iPcMechanicsObject,csVector3&,Lift)
%cel_attribute(iPcMechanicsObject,bool,Static,IsStatic,MakeStatic)
%cel_attribute(iPcMechanicsObject,bool,CollisionCallbackEnabled,IsCollisionCallbackEnabled,SetCollisionCallbackEnabled)
CEL_PC(iPcMechanicsObject, MechanicsObject, pcmechobject)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsObject)

%cel_attribute(iPcMechanicsJoint,iJoint*,Joint,GetJoint)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsJoint)
CEL_PC(iPcMechanicsJoint, MechanicsJoint, pcmechjoint)

%cel_attribute(iPcMechanicsThruster,iPcMechanicsObject*,MechanicsObject)
%cel_attribute(iPcMechanicsThruster,csVector3&,Position)
%cel_attribute(iPcMechanicsThruster,csVector3&,Orientation)
%cel_attribute(iPcMechanicsThruster,float,MaxThrust,GetMaxThrust)
%cel_attribute(iPcMechanicsThrusterController,iPcMechanicsObject*,MechanicsObject)
%cel_attribute(iPcMechanicsBalancedGroup,celAxisType,Type)
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

%cel_attribute(iPcBillboard,const char*,Name,GetBillboardName,SetBillboardName)
%cel_attribute(iPcBillboard,iBillboard*,Billboard,GetBillboard)
%cel_attribute(iPcBillboard,bool,EventsEnabled,AreEventsEnabled,EnableEvents)
%include "propclass/billboard.h"
CEL_PC(iPcBillboard, Billboard, pcbillboard)
CEL_PC_QUERY_CLASSLIST(iPcBillboard)

//-----------------------------------------------------------------------------

%cel_attribute(iPcRegion,const char*,Name,GetRegionName,SetRegionName)
%cel_attribute(iPcRegion,iRegion*,Region,GetRegion)
%cel_attribute(iPcRegion,const char*,StartSector,GetStartSector)
%cel_attribute(iPcRegion,csVector3,StartPosition,GetStartPosition)
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

// TODO these need pseudo-dict like interfaces for some stuff
%cel_attribute(iCelMapFile,const char*,Path)
%cel_attribute(iCelMapFile,const char*,File)
%cel_attribute(iCelMapFile,const char*,SectorName)
%cel_attribute(iCelRegion,const char*,Name,GetName)
%cel_attribute(iCelRegion,const char*,CachePath)
%cel_attribute(iCelRegion,size_t,MapFileCount,GetMapFileCount)
%cel_attribute(iCelZone,const char*,Name,GetName)
%cel_attribute(iCelZone,size_t,RegionCount,GetRegionCount)
%cel_attribute(iPcZoneManager,bool,ColliderWrappersEnabled,IsColliderWrappers,EnableColliderWrappers)
%cel_attribute(iPcZoneManager,int,LoadingMode)
%cel_attribute(iPcZoneManager,size_t,ZoneCount,GetZoneCount)
%cel_attribute(iPcZoneManager,size_t,RegionCount,GetRegionCount)
%cel_attribute(iPcZoneManager,const char*,LastStartRegionName,GetLastStartRegionName)
%cel_attribute(iPcZoneManager,const char*,LastStartName,GetLastStartName)
%include "propclass/zone.h"
CEL_PC(iPcZoneManager, ZoneManager, pczonemanager)

//-----------------------------------------------------------------------------

// TODO this needs dict like interface for binds
%cel_attribute(iPcCommandInput,bool,CookedModeEnabled,GetCookedMode,SetCookedMode)
%include "propclass/input.h"
CEL_PC(iPcCommandInput, CommandInput, pccommandinput)

//-----------------------------------------------------------------------------
// TODO this class has a horrible interface, must review carefully
%cel_attribute(iPcLinearMovement,iPcMesh*,Anchor)
//%cel_attribute(iPcLinearMovement,csVector3&,AngularVelocity)
//%cel_attribute(iPcLinearMovement,csVector3&,Velocity)
//%cel_attribute(iPcLinearMovement,float,Speed)
%cel_attribute(iPcLinearMovement,iSector*,Sector,GetSector)
%cel_attribute(iPcLinearMovement,bool,OnGround,IsOnGround,SetOnGround)
%cel_attribute(iPcLinearMovement,float,Gravity)
%cel_attribute(iPcLinearMovement,csVector3,PortalDisplacement,GetPortalDisplacement)
%include "propclass/linmove.h"
CEL_PC(iPcLinearMovement, LinearMovement, pclinearmovement)

//-----------------------------------------------------------------------------
%cel_attribute(iPcActorMove,bool,MovingForward,IsMovingForward,Forward)
%cel_attribute(iPcActorMove,bool,MovingBackward,IsMovingBackward,Backward)
%cel_attribute(iPcActorMove,bool,StrafingLeft,IsStrafingLeft,StrafeLeft)
%cel_attribute(iPcActorMove,bool,StrafingRight,IsStrafingRight,StrafeRight)
%cel_attribute(iPcActorMove,bool,RotatingLeft,IsRotatingLeft,RotateLeft)
%cel_attribute(iPcActorMove,bool,RotatingRight,IsRotatingRight,RotateRight)
%cel_attribute(iPcActorMove,bool,Running,IsRunning,Run)
%cel_attribute(iPcActorMove,bool,AutoRunEnabled,IsAutoRunning,AutoRun)
%cel_attribute(iPcActorMove,float,RunningSpeed)
%cel_attribute(iPcActorMove,float,RotationSpeed)
%cel_attribute(iPcActorMove,float,MovementSpeed)
%cel_attribute(iPcActorMove,bool,MouseMoveEnabled,IsMouseMoveEnabled,EnableMouseMove)
%cel_attribute(iPcActorMove,bool,MouseMoveInverted,IsMouseMoveInverted,SetMouseMoveInverted)
//%cel_attribute(iPcActorMove,float,MouseMoveSpeed)
%include "propclass/actormove.h"
CEL_PC(iPcActorMove, ActorMove, pcactormove)

//-----------------------------------------------------------------------------

// TODO must review distance methods
%cel_attribute(iPcCamera,iCamera*,Camera,GetCamera)
%cel_attribute(iPcCamera,iView*,View,GetView)
%cel_attribute(iPcCamera,bool,ClearZBuffer)
%cel_attribute(iPcCamera,bool,ClearScreen)
%cel_attribute(iPcCamera,int,DrawFlags,GetDrawFlags)
%cel_attribute(iPcCamera,float,FixedDistance,GetFixedDistance)
%cel_attribute(iPcCamera,float,AdaptiveMaxFPS,GetAdaptiveMaxFPS)
%cel_attribute(iPcCamera,float,AdaptiveMinFPS,GetAdaptiveMinFPS)
%cel_attribute(iPcCamera,float,AdaptiveMinDistance,GetAdaptiveMinDistance)
%include "propclass/camera.h"
CEL_PC(iPcCamera, Camera, pccamera)

//-----------------------------------------------------------------------------

%cel_attribute(iPcDefaultCamera,iPcDefaultCamera::CameraMode,Mode)
%cel_attribute(iPcDefaultCamera,const char*,ModeName)
//%cel_attribute(iPcDefaultCamera,float,TurnSpeed) ONLY HAS SETTER
//%cel_attribute(iPcDefaultCamera,float,SwingCoef) ONLY HAS SETTER
%cel_attribute(iPcDefaultCamera,float,Pitch)
%cel_attribute(iPcDefaultCamera,float,PitchVelocity)
%cel_attribute(iPcDefaultCamera,float,Yaw)
%cel_attribute(iPcDefaultCamera,float,YawVelocity)
%cel_attribute(iPcDefaultCamera,float,Distance)
%cel_attribute(iPcDefaultCamera,float,DistanceVelocity)
%include "propclass/defcam.h"
CEL_PC(iPcDefaultCamera, DefaultCamera, pcdefaultcamera)

// TODO simpcam missing
%include "propclass/simpcam.h"
CEL_PC(iPcSimpleCamera, SimpleCamera, pcsimplecamera)

//-----------------------------------------------------------------------------

%cel_attribute(iPcMeshSelect,int,MouseButtons) // has another way to set
%cel_attribute(iPcMeshSelect,bool,GlobalSelection,HasGlobalSelection,SetGlobalSelection)
%cel_attribute(iPcMeshSelect,bool,FollowMode,HasFollowMode,SetFollowMode)
%cel_attribute(iPcMeshSelect,bool,FollowAlwaysMode,HasFollowAlwaysMode,SetFollowAlwaysMode)
%cel_attribute(iPcMeshSelect,bool,DragMode,HasDragMode,SetDragMode)
// %cel_attribute(iPcMeshSelect,bool,DragPlaneNormal) HAS TWO PARAMETERS
%cel_attribute(iPcMeshSelect,bool,SendmoveEvent,HasSendmoveEvent,SetSendmoveEvent)
%cel_attribute(iPcMeshSelect,bool,SendupEvent,HasSendupEvent,SetSendupEvent)
%cel_attribute(iPcMeshSelect,bool,SenddownEvent,HasSenddownEvent,SetSenddownEvent)
%cel_attribute(iPcMeshSelect,float,MaxSelectionDistance)
%include "propclass/meshsel.h"
CEL_PC(iPcMeshSelect, MeshSelect, pcmeshselect)

//-----------------------------------------------------------------------------

// TODO Visible method should be a getter/setter
%cel_attribute(iPcMesh,iMeshWrapper*,Mesh)
%rename(LoadMesh) iPcMesh::SetMesh(const char* factname, const char* filename);
%include "propclass/mesh.h"
CEL_PC(iPcMesh, Mesh, pcmesh)

//-----------------------------------------------------------------------------

// no getters/setters
%include "propclass/timer.h"
CEL_PC(iPcTimer, Timer, pctimer)

//-----------------------------------------------------------------------------

%cel_attribute(iPcProjectile,bool,Moving,IsMoving)
%include "propclass/projectile.h"
CEL_PC(iPcProjectile, Projectile, pcprojectile)

//-----------------------------------------------------------------------------

%cel_attribute(iPcSolid,iPcMesh*,Mesh)
%cel_attribute(iPcSolid,iCollider*,Collider,GetCollider)
%include "propclass/solid.h"
CEL_PC(iPcSolid, Solid, pcsolid)

//-----------------------------------------------------------------------------

%cel_attribute(iPcGravity,iCollider*,GravityCollider,GetGravityCollider)
%cel_attribute(iPcGravity,iPcMovable*,Movable)
%cel_attribute(iPcGravity,iPcSolid*,Solid)
%cel_attribute(iPcGravity,float,Weight)
%cel_attribute(iPcGravity,bool,Active,IsActive,SetActive)
%cel_attribute(iPcGravity,bool,Resting,IsResting)
%rename(CreateGravityColliderFromMesh) iPcGravity::CreateGravityCollider;
%include "propclass/gravity.h"
CEL_PC(iPcGravity, Gravity, pcgravity)

//-----------------------------------------------------------------------------

%cel_attribute(iPcMovable,iPcMesh*,Mesh)
%rename(SetPos) iPcMovable::Move;
%include "propclass/move.h"
CEL_PC(iPcMovable, Movable, pcmovable)

//-----------------------------------------------------------------------------

// TODO more things to do here
%include "propclass/inv.h"
CEL_PC(iPcInventory, Inventory, pcinventory)

// iCelEntityInvFakeArray
CEL_FAKE_ARRAY(Inv,iCelEntity,GetEntityCount,GetEntity,In,RemoveEntity,AddEntity)

%extend iPcInventory {
	%pythoncode %{
	def GetEntities(self): return iCelEntityInvFakeArray(self)
	__swig_getmethods__["Entities"] = lambda self: self.GetEntities() %}
}


//-----------------------------------------------------------------------------

// TODO add dict like interface for characteristics
%include "propclass/chars.h"
CEL_PC(iPcCharacteristics, Characteristics, pccharacteristics)

//-----------------------------------------------------------------------------

// TODO visible interface is inconsistent
%cel_attribute(iPcTooltip,bool,Visible,IsVisible)
%include "propclass/tooltip.h"
CEL_PC(iPcTooltip, ToolTip, pctooltip)

SETTER_METHOD(iPcTooltip,"Text", SetText)
//-----------------------------------------------------------------------------

%cel_attribute(iPcSoundSource,iSndSysSource*,SoundSource,GetSoundSource)
%cel_attribute(iPcSoundSource,const char*,SoundName)
%cel_attribute(iPcSoundListener,iSndSysListener*,SoundListener,GetSoundListener)
%include "propclass/sound.h"
CEL_PC(iPcSoundSource, SoundSource, pcsoundsource)
CEL_PC(iPcSoundListener, SoundListener, pcsoundlistener)

//-----------------------------------------------------------------------------

// TODO needs dict like interface
%include "propclass/prop.h"
CEL_PC(iPcProperties, Properties, pcproperties)

//-----------------------------------------------------------------------------

%cel_attribute(iPcMover,iSector*,Sector,GetSector)
%cel_attribute(iPcMover,csVector3&,Position,GetPosition)
%cel_attribute(iPcMover,csVector3&,Up,GetUp)
%cel_attribute(iPcMover,float,SqRadius,GetSqRadius)
%cel_attribute(iPcMover,bool,Moving,IsMoving)
%include "propclass/mover.h"
CEL_PC(iPcMover, Mover, pcmover)

//-----------------------------------------------------------------------------

%cel_attribute(iPcHover,float,Height, GetHeight)
%include "propclass/hover.h"
CEL_PC(iPcHover, Hover, pchover)

SETTER_METHOD(iPcHover, "HeightBeamCutoff", SetHeightBeamCutoff)
SETTER_METHOD(iPcHover, "AngularBeamOffset", SetAngularBeamOffset)
SETTER_METHOD(iPcHover, "AngularCutoffHeight", SetAngularCutoffHeight)
SETTER_METHOD(iPcHover, "AngularCorrectionStrength", SetAngularCorrectionStrength)


//-----------------------------------------------------------------------------

// TODO *very* inconsistent interfaces
%cel_attribute(iPcCraftController,bool,ThrusterOn,IsThrusterOn)
%include "propclass/craft.h"
CEL_PC(iPcCraftController, CraftController, pccraft)

SETTER_METHOD(iPcCraftController, "AccTurn", SetAccTurn)
SETTER_METHOD(iPcCraftController, "AccPitch", SetAccPitch)
SETTER_METHOD(iPcCraftController, "MaxTurn", SetMaxTurn)
SETTER_METHOD(iPcCraftController, "MaxPitch", SetMaxPitch)
SETTER_METHOD(iPcCraftController, "ThrustForce", SetThrustForce)
SETTER_METHOD(iPcCraftController, "TopSpeed", SetTopSpeed)
SETTER_METHOD(iPcCraftController, "RedirectVelocityRatio", SetRedirectVelocityRatio)
SETTER_METHOD(iPcCraftController, "DecelerationRate", SetDecelerationRate)
SETTER_METHOD(iPcCraftController, "BrakingSpeed", SetBrakingSpeed)
SETTER_METHOD(iPcCraftController, "AfterBurnerTopSpeed", SetAfterBurnerTopSpeed)

//-----------------------------------------------------------------------------

// TODO this deserves some pseudo-dict for wheels
%cel_attribute(iPcWheeled,bool,TankMode)
%cel_attribute(iPcWheeled,bool,Accelerating,IsAccelerating,Accelerate)
%cel_attribute(iPcWheeled,bool,Braking,IsBraking,Brake)
%cel_attribute(iPcWheeled,bool,Handbraking,IsHandbraking,Handbrake)
%cel_attribute(iPcWheeled,float,SteerAmount)
%cel_attribute(iPcWheeled,int,Gear)
%cel_attribute(iPcWheeled,int,TopGear,GetTopGear)
%cel_attribute(iPcWheeled,iBodyGroup*,BodyGroup,GetBodyGroup)
%include "propclass/wheeled.h"
CEL_PC(iPcWheeled, Wheeled, pcwheeled)

SETTER_METHOD(iPcWheeled,"BrakeForce", SetBrakeForce)
SETTER_METHOD(iPcWheeled,"AutoReverse", SetAutoReverse)
GETTER_METHOD(iPcWheeled,"WheelCount", GetWheelCount)

//-----------------------------------------------------------------------------

%cel_attribute(iPcDamage,float,Damage)
%cel_attribute(iPcDamage,const char*,DamageType)
%cel_attribute(iPcDamage,const char*,FallOff)
%cel_attribute(iPcDamage,const char*,DamageSector,GetDamageSector)
%cel_attribute(iPcDamage,csVector3&,DamagePosition,GetDamagePosition)
%include "propclass/damage.h"
CEL_PC(iPcDamage, Damage, pcdamage)

//-----------------------------------------------------------------------------

%cel_attribute(iCelConsole,iConsoleInput*,InputConsole,GetInputConsole)
%cel_attribute(iCelConsole,iConsoleOutput*,OutputConsole,GetOutputConsole)
%include "tools/celconsole.h"
%inline %{
iCelConsole *csQueryRegistry_iCelConsole (iObjectRegistry *object_reg)
{
  csRef<iCelConsole> bl = CS_QUERY_REGISTRY (object_reg, iCelConsole);
  return bl;
}
%}

//-----------------------------------------------------------------------------

#undef INTERFACE_APPLY
#define INTERFACE_APPLY(x) INTERFACE_POST(x)
CEL_APPLY_FOR_EACH_INTERFACE

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// helpers to redirect output to cel console

%pythoncode %{
class CelConsoleOut:
	"""Class that can be assigned to sys.stdout or sys.stderr"""
	def __init__(self,oreg):
		self.oreg = oreg
	def write(self,s):
		csQueryRegistry_iCelConsole(self.oreg).GetOutputConsole().PutText(str(s))

class CelConsoleOutOverride:
	"""Class that redirects stdout and stderr to celconsole"""
	def __init__(self,oreg):
		import sys
		self.oldstdout = sys.stdout
		self.oldstderr = sys.stderr
		sys.stdout = CelConsoleOut(oreg)
		sys.stderr = CelConsoleOut(oreg)
	def __del__(self):
		import sys
		sys.stdout = self.oldstdout
		sys.stderr = self.oldstderr
%}
