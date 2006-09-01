#define __CEL__
%module blcelc
%import "bindings/cspace.i"
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
#include "propclass/tooltip.h"
#include "propclass/prop.h"
#include "propclass/hover.h"
#include "propclass/craft.h"
#include "propclass/wheeled.h"
#include "plugins/behaviourlayer/python/blpython.h"
#include "tools/billboard.h"
#include "tools/celconsole.h"
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

//-----------------------------------------------------------------------------

%define GETTER_METHOD(classname,varname,getfunc)
%extend classname {
 %pythoncode %{
  __swig_getmethods__[varname] = _blcelc. ## classname ## _ ## getfunc  %}
}
%enddef

%define SETTER_METHOD(classname,varname,setfunc)
%extend classname {
 %pythoncode %{
  __swig_setmethods__[varname] = _blcelc. ## classname ## _ ## setfunc  %}
}
%enddef

%define GETSET_METHODS2(classname, varname, getfunc, setfunc)
SETTER_METHOD(classname, varname, setfunc)
GETTER_METHOD(classname, varname, getfunc)
%enddef

%define GETSET_METHODS(classname, varname, parname)
SETTER_METHOD(classname, varname, Set ## parname)
GETTER_METHOD(classname, varname, Get ## parname)
%enddef

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
	__swig_getmethods__["entitytpls"] = lambda self: self.GetEntityTemplates()
	__swig_getmethods__["pcfactories"] = lambda self: self.GetPcFactories()
	__swig_getmethods__["behaviourlayers"] = lambda self: self.GetBehaviourLayers()
	__swig_getmethods__["entities"] = lambda self: self.GetEntities() %}
}

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
GETSET_METHODS(iCelEntity,"name",Name)
GETSET_METHODS(iCelEntity,"id",ID)
GETSET_METHODS(iCelEntity,"behaviour",Behaviour)

GETTER_METHOD(iCelEntity,"pcclasses",GetPropertyClassList)
GETTER_METHOD(iCelEntity,"classes",GetClasses)

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

%include "physicallayer/entitytpl.h"

GETSET_METHODS(iCelEntityTemplate,"name",Name)
GETSET_METHODS(iCelEntityTemplate,"behaviour",Behaviour)
GETTER_METHOD(iCelEntityTemplate,"behaviourlayer",GetBehaviourLayer)

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
  PyObject *SendMessage(const char* msg_id, iCelPropertyClass* prop, iCelParameterBlock* params)
  {
       celData ret;
       if(self->SendMessage (msg_id,prop,ret,params))
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
%include "physicallayer/propfact.h"
GETTER_METHOD(iCelPropertyClassFactory,"name",GetName)
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
GETSET_METHODS(iCelPropertyClass,"tag",Tag)
GETSET_METHODS(iCelPropertyClass,"entity",Entity)
CELLIST_METHODS(iCelPropertyClassList,iCelPropertyClass)

//-----------------------------------------------------------------------------

%include "propclass/mechsys.h"
CEL_PC(iPcMechanicsSystem, MechanicsSystem, pcmechsys)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsSystem)
GETSET_METHODS(iPcMechanicsSystem,"dynsys",DynamicSystem)
GETSET_METHODS(iPcMechanicsSystem,"steptime",StepTime)
GETSET_METHODS(iPcMechanicsSystem,"gravity",Gravity)

CEL_PC(iPcMechanicsObject, MechanicsObject, pcmechobject)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsObject)
GETSET_METHODS(iPcMechanicsObject,"mesh",Mesh)
GETSET_METHODS(iPcMechanicsObject,"light",Light)
GETSET_METHODS(iPcMechanicsObject,"camera",Camera)
GETSET_METHODS(iPcMechanicsObject,"mechsys",MechanicsSystem)
GETTER_METHOD(iPcMechanicsObject,"body",GetBody)
GETSET_METHODS(iPcMechanicsObject,"friction",Friction)
GETSET_METHODS(iPcMechanicsObject,"mass",Mass)
GETSET_METHODS(iPcMechanicsObject,"elasticity",Elasticity)
GETSET_METHODS(iPcMechanicsObject,"softness",Softness)
GETSET_METHODS(iPcMechanicsObject,"drag",Drag)
GETSET_METHODS(iPcMechanicsObject,"lift",Lift)
GETSET_METHODS(iPcMechanicsObject,"linearvelocity",LinearVelocity)
GETSET_METHODS(iPcMechanicsObject,"angularvelocity",AngularVelocity)
GETSET_METHODS2(iPcMechanicsObject,"static",IsStatic,MakeStatic)

CEL_PC(iPcMechanicsJoint, MechanicsJoint, pcmechjoint)
GETTER_METHOD(iPcMechanicsJoint,"joint",GetJoint)
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
GETSET_METHODS(iPcBillboard,"name",BillboardName)
GETSET_METHODS2(iPcBillboard,"events",AreEventsEnabled,EnableEvents)

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
GETSET_METHODS(iPcRegion,"name",RegionName)
GETTER_METHOD(iPcRegion,"region",GetRegion)

//-----------------------------------------------------------------------------

%include "propclass/zone.h"
CEL_PC(iPcZoneManager, ZoneManager, pczonemanager)
GETSET_METHODS(iCelMapFile,"path",Path)
GETSET_METHODS(iCelMapFile,"file",File)
GETSET_METHODS(iCelMapFile,"sectorname",SectorName)
GETTER_METHOD(iCelRegion,"name",GetName)
GETSET_METHODS(iCelRegion,"cachepath",CachePath)
GETTER_METHOD(iCelZone,"name",GetName)

//-----------------------------------------------------------------------------

%include "propclass/input.h"
CEL_PC(iPcCommandInput, CommandInput, pccommandinput)
GETSET_METHODS(iPcCommandInput,"cookedmode",CookedMode)

//-----------------------------------------------------------------------------

%include "propclass/linmove.h"
CEL_PC(iPcLinearMovement, LinearMovement, pclinearmovement)
GETSET_METHODS(iPcLinearMovement,"anchor",Anchor)
GETSET_METHODS(iPcLinearMovement,"angularvelocity",AngularVelocity)
//GETSET_METHODS(iPcLinearMovement,"speed",Speed)
GETSET_METHODS(iPcLinearMovement,"velocity",Velocity)
//GETSET_METHODS(iPcLinearMovement,"position",Position)
//GETSET_METHODS(iPcLinearMovement,"fullposition",FullPosition)
GETSET_METHODS2(iPcLinearMovement,"path",IsPath,SetPath)
GETTER_METHOD(iPcLinearMovement,"sector",GetSector)
GETSET_METHODS2(iPcLinearMovement,"onground",IsOnGround,SetOnGround)
GETSET_METHODS(iPcLinearMovement,"gravity",Gravity)

//-----------------------------------------------------------------------------
%include "propclass/actormove.h"
CEL_PC(iPcActorMove, ActorMove, pcactormove)
GETSET_METHODS2(iPcActorMove,"forward",IsMovingForward,Forward)
GETSET_METHODS2(iPcActorMove,"backward",IsMovingBackward,Backward)
GETSET_METHODS2(iPcActorMove,"strafeleft",IsStrafingLeft,StrafeLeft)
GETSET_METHODS2(iPcActorMove,"straferight",IsStrafingRight,StrafeRight)
GETSET_METHODS2(iPcActorMove,"rotateleft",IsRotatingLeft,RotateLeft)
GETSET_METHODS2(iPcActorMove,"rotateright",IsRotatingRight,RotateRight)
GETSET_METHODS2(iPcActorMove,"run",IsRunning,Run)
GETSET_METHODS2(iPcActorMove,"autorun",IsAutoRunning,AutoRun)
GETSET_METHODS(iPcActorMove,"movementspeed",MovementSpeed)
GETSET_METHODS(iPcActorMove,"runningspeed",RunningSpeed)
GETSET_METHODS(iPcActorMove,"rotationspeed",RotationSpeed)
GETSET_METHODS(iPcActorMove,"jumpingvelocity",JumpingVelocity)

//-----------------------------------------------------------------------------

%include "propclass/camera.h"
CEL_PC(iPcCamera, Camera, pccamera)
GETTER_METHOD(iPcCamera,"camera",GetCamera)
GETTER_METHOD(iPcCamera,"view",GetView)
GETSET_METHODS(iPcCamera,"clearzbuffer",ClearZBuffer)
GETSET_METHODS(iPcCamera,"clearscreen",ClearScreen)

%include "propclass/defcam.h"
CEL_PC(iPcDefaultCamera, DefaultCamera, pcdefaultcamera)
GETSET_METHODS(iPcDefaultCamera,"yaw",Yaw)
GETSET_METHODS(iPcDefaultCamera,"pitch",Pitch)
GETSET_METHODS(iPcDefaultCamera,"yaw_vel",YawVelocity)
GETSET_METHODS(iPcDefaultCamera,"pitch_vel",PitchVelocity)
GETSET_METHODS(iPcDefaultCamera,"mode",ModeName)

%include "propclass/simpcam.h"
CEL_PC(iPcSimpleCamera, SimpleCamera, pcsimplecamera)

//-----------------------------------------------------------------------------

%include "propclass/meshsel.h"
CEL_PC(iPcMeshSelect, MeshSelect, pcmeshselect)
GETSET_METHODS(iPcMeshSelect,"mousebuttons",MouseButtons)
GETSET_METHODS2(iPcMeshSelect,"global",HasGlobalSelection,SetGlobalSelection)
GETSET_METHODS2(iPcMeshSelect,"follow",HasFollowMode,SetFollowMode)
GETSET_METHODS2(iPcMeshSelect,"followalways",HasFollowAlwaysMode,SetFollowAlwaysMode)
GETSET_METHODS2(iPcMeshSelect,"drag",HasDragMode,SetDragMode)
GETSET_METHODS(iPcMeshSelect,"dragnormal",DragPlaneNormal)
GETSET_METHODS2(iPcMeshSelect,"moveevent",HasSendmoveEvent,SetSendmoveEvent)
GETSET_METHODS2(iPcMeshSelect,"moveup",HasSendupEvent,SetSendupEvent)
GETSET_METHODS2(iPcMeshSelect,"movedown",HasSenddownEvent,SetSenddownEvent)
GETSET_METHODS(iPcMeshSelect,"distance",MaxSelectionDistance)

//-----------------------------------------------------------------------------

%rename(LoadMesh) iPcMesh::SetMesh;
%include "propclass/mesh.h"
CEL_PC(iPcMesh, Mesh, pcmesh)
GETTER_METHOD(iPcMesh,"mesh",GetMesh)

//-----------------------------------------------------------------------------

%include "propclass/timer.h"
CEL_PC(iPcTimer, Timer, pctimer)

//-----------------------------------------------------------------------------

%include "propclass/projectile.h"
CEL_PC(iPcProjectile, Projectile, pcprojectile)
GETTER_METHOD(iPcProjectile,"moving",IsMoving)

//-----------------------------------------------------------------------------

%include "propclass/solid.h"
CEL_PC(iPcSolid, Solid, pcsolid)

GETTER_METHOD(iPcSolid,"collider", GetCollider)
GETSET_METHODS(iPcSolid,"mesh", Mesh)

//-----------------------------------------------------------------------------

%rename(CreateGravityColliderFromMesh) iPcGravity::CreateGravityCollider;
%include "propclass/gravity.h"
CEL_PC(iPcGravity, Gravity, pcgravity)

GETTER_METHOD(iPcGravity,"gravitycollider", GetGravityCollider)
GETSET_METHODS(iPcGravity,"solid", Solid)
GETSET_METHODS(iPcGravity,"weight", Weight)
GETSET_METHODS2(iPcGravity,"active", IsActive, SetActive)
GETSET_METHODS(iPcGravity,"movable", Movable)

//-----------------------------------------------------------------------------

%rename(SetPos) iPcMovable::Move;
%include "propclass/move.h"
CEL_PC(iPcMovable, Movable, pcmovable)

GETSET_METHODS(iPcMovable,"mesh", Mesh)

//-----------------------------------------------------------------------------

%include "propclass/inv.h"
CEL_PC(iPcInventory, Inventory, pcinventory)

// iCelEntityInvFakeArray
CEL_FAKE_ARRAY(Inv,iCelEntity,GetEntityCount,GetEntity,In,RemoveEntity,AddEntity)

%extend iPcInventory {
	%pythoncode %{
	def GetEntities(self): return iCelEntityInvFakeArray(self)
	__swig_getmethods__["entities"] = lambda self: self.GetEntities() %}
}


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

%include "propclass/wheeled.h"
CEL_PC(iPcWheeled, Wheeled, pcwheeled)

//-----------------------------------------------------------------------------

%include "tools/celconsole.h"
%inline %{
iCelConsole *csQueryRegistry_iCelConsole (iObjectRegistry *object_reg)
{
  csRef<iCelConsole> bl = CS_QUERY_REGISTRY (object_reg, iCelConsole);
  return bl;
}
%}

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
