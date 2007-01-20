#define __CEL__
%module blcelc
%import "bindings/cspace.i"
%include "celproperties.i" // all property accessors
CS_PROPERTY_HELPERS
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
#include "propclass/meshdeform.h"
#include "propclass/damage.h"
#include "propclass/quest.h"
#include "plugins/behaviourlayer/python/blpython.h"
#include "tools/billboard.h"
#include "tools/celconsole.h"
#include "tools/questmanager.h"
#include "propclass/zone.h"
%}
%inline %{
  extern PyObject *    // from crystalspace_python
  _csRef_to_Python (const csRef<iBase> & ref, void * ptr, const char * name);

%}

//=============================================================================
// Interfaces that need csRef,csPtr,csRefArray
// uses INTERFACE_POST and INTERFACE_PRE from cspace.i
//=============================================================================

%define CEL_APPLY_FOR_EACH_INTERFACE
  INTERFACE_APPLY(iCelPlLayer)
  INTERFACE_APPLY(iCelBlLayer)
  INTERFACE_APPLY(iCelEntity)
  INTERFACE_APPLY(iCelPropertyClass)
  INTERFACE_APPLY(iQuestManager)
  INTERFACE_APPLY(iCelEntityList)
%enddef

//=============================================================================
// Helper macros.
//=============================================================================

%define CEL_PC_CREATE(pcType, funcName, pcname)
%inline %{
pcType *funcName(iCelPlLayer *pl, iCelEntity *entity, const char* tagname = 0 ) 
{
  csRef<iCelPropertyClass> pc;
  if (tagname)
    pc = pl->CreateTaggedPropertyClass(entity, #pcname, tagname);
  else
    pc = pl->CreatePropertyClass(entity, #pcname );
  if (!pc.IsValid()) return 0;
  csRef<pcType> pclm = scfQueryInterface<pcType>(pc);
  if (!pclm.IsValid()) return 0;
  return pclm;
}
%}
%enddef

%define CEL_PC_GETSET(pcType, funcName,pcname)
%inline %{
pcType * funcName (iCelPlLayer *pl, iCelEntity *entity, const char* tagname = 0)
{
  csRef<pcType> pclm;
  if (tagname)
    pclm = CEL_QUERY_PROPCLASS_TAG (
      entity->GetPropertyClassList (), pcType, tagname);
  else
    pclm = CEL_QUERY_PROPCLASS (
      entity->GetPropertyClassList (), pcType);
  if (pclm.IsValid()) return pclm;
  csRef<iCelPropertyClass> pc;
  if (tagname)
    pc = pl->CreateTaggedPropertyClass(entity, #pcname, tagname );
  else
    pc = pl->CreatePropertyClass(entity, #pcname );
  if (!pc.IsValid()) return 0;
  pclm = scfQueryInterface<pcType>(pc);
  if (!pclm.IsValid()) return 0;
  return pclm;
}
%}
%enddef

%define CEL_PC_GET(pcType, funcName)
%inline %{
pcType * funcName (iCelEntity *entity, const char* tagname = 0 )
{
  csRef<pcType> pc;
  if (tagname)
    pc = CEL_QUERY_PROPCLASS_TAG (
      entity->GetPropertyClassList (), pcType, tagname);
  else
    pc = CEL_QUERY_PROPCLASS (
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
  csRef<pcType> iface = scfQueryInterface<pcType>(pc);
  if (iface) iface->IncRef ();
  return iface;
}
%}
%enddef

%define CEL_PC_QUERY_CLASSLIST(pcType)
%inline %{
pcType *scfQueryPC_ ## pcType (iCelPropertyClassList *pclist)
{
  csRef<pcType> iface = scfQueryInterface<pcType>(pclist);
  if (iface) iface->IncRef ();
  return iface;
}
%}
%enddef

%extend iCelPropertyClass {
   %pythoncode %{
   def GetterFallback(self,attr):
        raise AttributeError
   def SetterFallback(self,attr,value):
        raise AttributeError
%}
}

#undef SCF_QUERY_INTERFACE
%define CEL_PC_FIX_INHERITANCE(pcType)
%extend pcType {
   %pythoncode %{
   _PC = None
   def _getBasePc(self):
        pc = cspace.SCF_QUERY_INTERFACE(self,iCelPropertyClass)
        _object.__setattr__(self,"_PC",pc)
   def __getattr__(self,attr):
        if not self._PC: self._getBasePc()
        try:
            return _swig_getattr(self, pcType, attr)
        except:
            pass
        if hasattr(self._PC,attr):
            return getattr(self._PC,attr)
        else:
            return self.GetterFallback(attr)
   def __setattr__(self,attr,value):
        if not self._PC: self._getBasePc()
        if attr in pcType.__swig_setmethods__.keys():
            return _swig_setattr(self,pcType,attr,value)
        elif hasattr(self._PC,attr):
            setattr(self._PC,attr,value)
        else:
            return self.SetterFallback(attr,value)
%}
}
%enddef

%define CEL_PC(pcType, funcBaseName, pcname)
CEL_PC_CREATE(pcType, celCreate ## funcBaseName, pcname)
CEL_PC_GETSET(pcType, celGetSet ## funcBaseName, pcname)
CEL_PC_GET(pcType, celGet ## funcBaseName)
CEL_PC_QUERY(pcType)
CEL_PC_FIX_INHERITANCE(pcType)
%enddef

//-----------------------------------------------------------------------------
%define CELLIST_METHODS(classname,typename)
	PYLIST_BASE_FUNCTIONS(classname,typename*,size_t,GetCount,Get,Add,Remove,Find)
	PYLIST_BYNAME_FUNCTIONS(classname,typename*,FindByName)
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

%apply csStringFast * { const csStringFast<12>& };
%template (csStringFast12) csStringFast<12>;
%template (celEntityTemplateParams) csHash<csStringFast<12>, csStringFast<12> >; 

%include "physicallayer/pl.h"
%inline %{
iCelPlLayer *csQueryRegistry_iCelPlLayer (iObjectRegistry *object_reg)
{
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
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
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
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
  csRef<iCelEntity> ent = scfQueryInterface<iCelEntity>(base);
  return ent;
}
%}

%inline %{
iCelEntityList *celFindNearbyEntities (iObjectRegistry *object_reg,
	iSector *sector, csVector3 pos, float radius, bool do_invisible=false)
{
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
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
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
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
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl.IsValid()) return 0;
  csRef<iCelEntityList> entlist = pl->FindNearbyEntities (sector, box, do_invisible);
  entlist->IncRef();
  return entlist;
}
%}
//-----------------------------------------------------------------------------

%include "physicallayer/entitytpl.h"

//-----------------------------------------------------------------------------

%include "behaviourlayer/bl.h"
%inline %{
iCelBlLayer *csQueryRegistry_iCelBlLayer (iObjectRegistry *object_reg)
{
  csRef<iCelBlLayer> bl = csQueryRegistry<iCelBlLayer> (object_reg);
  return bl;
}
%}

//-----------------------------------------------------------------------------

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

/* shadow Remove method so pcclass specific interfaces can be used */
%feature("shadow") iCelPropertyClassList::Remove
%{
  def Remove(self,propclass):
    if not (isinstance(propclass,int) or isinstance(propclass,
            iCelPropertyClass)):
      propclass = cspace.SCF_QUERY_INTERFACE(propclass,iCelPropertyClass)
    return _blcelc.iCelPropertyClassList_Remove(self,propclass)
%}
%ignore iCelPropertyClass::SetProperty;
%include "physicallayer/propfact.h"

/* Some typemaps so values are appropiately returned */
TYPEMAP_ARGOUT_PTR(csVector3)
TYPEMAP_ARGOUT_PTR(csVector2)
TYPEMAP_ARGOUT_PTR(csColor)
APPLY_TYPEMAP_ARGOUT_PTR(csVector3, csVector3& v)
APPLY_TYPEMAP_ARGOUT_PTR(csVector2, csVector2& v)
APPLY_TYPEMAP_ARGOUT_PTR(csColor, csColor& v)

/* Rename vector getter functions so they dont shadow each other */
%rename(GetPropertyVector3) iCelPropertyClass::GetPropertyVector (csStringID propertyID, csVector3& v);
%rename(GetPropertyVector2) iCelPropertyClass::GetPropertyVector (csStringID propertyID, csVector2& v);

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
  bool SetPropertyVector2 (csStringID id, const csVector2& vec)
  { return self->SetProperty (id, vec); }
  bool SetPropertyVector3 (csStringID id, const csVector3& vec)
  { return self->SetProperty (id, vec); }
  bool SetPropertyColor (csStringID id, const csColor& col)
  { return self->SetProperty (id, col); }
  bool SetPropertyEntity (csStringID id, const iCelEntity* ent)
  { return self->SetProperty (id, ent); }
}
/* Clear Typemaps */
%clear csVector3& v;
%clear csVector2& v;
%clear csColor& v;

CELLIST_METHODS(iCelPropertyClassList,iCelPropertyClass)

//-----------------------------------------------------------------------------

%include "propclass/mechsys.h"
CEL_PC(iPcMechanicsSystem, MechanicsSystem, pcmechsys)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsSystem)

CEL_PC(iPcMechanicsObject, MechanicsObject, pcmechobject)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsObject)

CEL_PC_QUERY_CLASSLIST(iPcMechanicsJoint)
CEL_PC(iPcMechanicsJoint, MechanicsJoint, pcmechjoint)

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
  csRef<iPcRegion> pcregion = scfQueryInterface<iPcRegion>(pc);
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
%include "propclass/zone.h"
CEL_PC(iPcZoneManager, ZoneManager, pczonemanager)

//-----------------------------------------------------------------------------

// TODO this needs dict like interface for binds
%include "propclass/input.h"
CEL_PC(iPcCommandInput, CommandInput, pccommandinput)

//-----------------------------------------------------------------------------
// TODO this class has a horrible interface, must review carefully
//%cel_attribute(iPcLinearMovement,csVector3&,AngularVelocity)
//%cel_attribute(iPcLinearMovement,csVector3&,Velocity)
//%cel_attribute(iPcLinearMovement,float,Speed)
%include "propclass/linmove.h"
CEL_PC(iPcLinearMovement, LinearMovement, pclinearmovement)

//-----------------------------------------------------------------------------
//%cel_attribute(iPcActorMove,float,MouseMoveSpeed)
%include "propclass/actormove.h"
CEL_PC(iPcActorMove, ActorMove, pcactormove)

//-----------------------------------------------------------------------------

// TODO must review distance methods
%include "propclass/camera.h"
CEL_PC(iPcCamera, Camera, pccamera)

//-----------------------------------------------------------------------------

//%cel_attribute(iPcDefaultCamera,float,TurnSpeed) ONLY HAS SETTER
//%cel_attribute(iPcDefaultCamera,float,SwingCoef) ONLY HAS SETTER
%include "propclass/defcam.h"
CEL_PC(iPcDefaultCamera, DefaultCamera, pcdefaultcamera)

// TODO simpcam missing
%include "propclass/simpcam.h"
CEL_PC(iPcSimpleCamera, SimpleCamera, pcsimplecamera)

//-----------------------------------------------------------------------------

// %cel_attribute(iPcMeshSelect,bool,DragPlaneNormal) HAS TWO PARAMETERS
%include "propclass/meshsel.h"
CEL_PC(iPcMeshSelect, MeshSelect, pcmeshselect)

//-----------------------------------------------------------------------------

%rename(LoadMesh) iPcMesh::SetMesh(const char* factname, const char* filename);
%include "propclass/mesh.h"
CEL_PC(iPcMesh, Mesh, pcmesh)

//-----------------------------------------------------------------------------

// no getters/setters
%include "propclass/timer.h"
CEL_PC(iPcTimer, Timer, pctimer)

//-----------------------------------------------------------------------------

%include "propclass/projectile.h"
CEL_PC(iPcProjectile, Projectile, pcprojectile)

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

%include "propclass/tooltip.h"
CEL_PC(iPcTooltip, ToolTip, pctooltip)

//-----------------------------------------------------------------------------

%include "propclass/sound.h"
CEL_PC(iPcSoundSource, SoundSource, pcsoundsource)
CEL_PC(iPcSoundListener, SoundListener, pcsoundlistener)

//-----------------------------------------------------------------------------

// TODO needs dict like interface
%include "propclass/prop.h"
CEL_PC(iPcProperties, Properties, pcproperties)

//-----------------------------------------------------------------------------

%include "propclass/mover.h"
CEL_PC(iPcMover, Mover, pcmover)

//-----------------------------------------------------------------------------

%include "propclass/hover.h"
CEL_PC(iPcHover, Hover, pchover)

//-----------------------------------------------------------------------------

// TODO *very* inconsistent interfaces
%include "propclass/craft.h"
CEL_PC(iPcCraftController, CraftController, pccraft)

//-----------------------------------------------------------------------------

// TODO this deserves some pseudo-dict for wheels
%include "propclass/wheeled.h"
CEL_PC(iPcWheeled, Wheeled, pcwheeled)

//-----------------------------------------------------------------------------

%include "propclass/meshdeform.h"
CEL_PC(iPcMeshDeform, MeshDeform, pcmeshdeform)

//-----------------------------------------------------------------------------

%include "propclass/damage.h"
CEL_PC(iPcDamage, Damage, pcdamage)

//-----------------------------------------------------------------------------

%include "tools/questmanager.h"
%include "propclass/quest.h"
CEL_PC(iPcQuest, Quest, pcquest)

//-----------------------------------------------------------------------------

%include "tools/celconsole.h"
%inline %{
iCelConsole *csQueryRegistry_iCelConsole (iObjectRegistry *object_reg)
{
  csRef<iCelConsole> bl = csQueryRegistry<iCelConsole> (object_reg);
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
