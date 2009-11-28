#define __CEL__
%module(directors="1") blcelc
%import "bindings/cspace.i"
%include "celproperties.i" // all property accessors
CS_PROPERTY_HELPERS

%{
#include <crystalspace.h>
#include "cel.h"
%}

/* Funtions to set the modules global SCF pointer, this is needed
   when working on a pure scripting environment, as then this code
   lives in a non-cs dll, thus the pointer isnt initialized
   by cs itself, and scf stuff wont work unless the pointer is
   initialized manually. Use it after CreateEnvironment call. */
#ifndef SWIGIMPORTED
INLINE_FUNCTIONS
#endif

/* Ignored Macros */
#undef CEL_CELTOOL_EXPORT
#define CEL_CELTOOL_EXPORT

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
  INTERFACE_APPLY(iMessageDispatcher)
  INTERFACE_APPLY(iBillboardManager)
  INTERFACE_APPLY(iCelGraph)
%enddef

//=============================================================================
// Common macros (undef and redefine in language specific files)
//=============================================================================

#define CELLIST_METHODS(classname,typename)
#define CEL_FAKE_ARRAY(pref,contenttype,countmethod,getmethod,findmethod,delmethod,addmethod)
#define CEL_PC_FIX_INHERITANCE(pcType)

CS_WRAP_PTR_IMPLEMENT(celWrapPtr)

//=============================================================================
// Language specific part
//=============================================================================

#if defined(SWIGPYTHON)
  %include "bindings/python/pre.i"
#elif defined(SWIGPERL5)
  %include "bindings/perl/pre.i"
#elif defined(SWIGRUBY)
  %include "bindings/ruby/pre.i"
#elif defined(SWIGTCL8)
  %include "bindings/tcl/pre.i"
#elif defined(SWIGJAVA)
  %include "bindings/java/pre.i"
#elif defined(SWIGLUA)
  %include "bindings/lua/pre.i"
#endif 


//=============================================================================
// Helper macros.
//=============================================================================

%define CEL_PC_CREATE(pcType, funcName, pcname)
%inline %{
pcType *funcName(iCelPlLayer *pl, iCelEntity *entity, const char* tagname = 0 ) 
{
  CS_ASSERT (entity != 0);
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
  CS_ASSERT (entity != 0);
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
  CS_ASSERT (entity != 0);
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

#undef SCF_QUERY_INTERFACE

%define CEL_PC(pcType, funcBaseName, pcname)
CEL_PC_CREATE(pcType, celCreate ## funcBaseName, pcname)
CEL_PC_GETSET(pcType, celGetSet ## funcBaseName, pcname)
CEL_PC_GET(pcType, celGet ## funcBaseName)
CEL_PC_QUERY(pcType)
CEL_PC_FIX_INHERITANCE(pcType)
INTERFACE_POST(pcType)
%enddef

//=============================================================================
// RefCounted Objects
//=============================================================================

%feature("ref")   celGenericParameterBlock "$this->IncRef();"
%feature("unref") celGenericParameterBlock "$this->DecRef();"

//=============================================================================
// Published interfaces and functions.
//=============================================================================

%ignore celInitializer::RequestPlugins;
%ignore celInitializer::RequestPluginsV;
%include "celtool/initapp.h"

//-----------------------------------------------------------------------------

%ignore celData::GetDebugInfo;
ARRAY_CHANGE_ALL_TEMPLATE(celData)
%include "physicallayer/datatype.h"


//-----------------------------------------------------------------------------

#undef INTERFACE_APPLY
#define INTERFACE_APPLY(x) INTERFACE_PRE(x)
CEL_APPLY_FOR_EACH_INTERFACE

//-----------------------------------------------------------------------------

%include "physicallayer/messaging.h"

//-----------------------------------------------------------------------------

/* the following declarations are a bit awkward, but seems swig (1.3.31)
obligues to set all parameters in template declarations, even default ones */
// celEntityTemplateParams
%apply csStringFast * { const csStringFast<12>& };
%template (csStringFast12) csStringFast<12>;
typedef csHash<csStringFast<12>, csStringFast<12>, CS::Memory::AllocatorMalloc, csArrayElementHandler<CS::Container::HashElement<csStringFast<12>, csStringFast<12> > > > celEntityTemplateParams;
%template (celEntityTemplateParams) csHash<csStringFast<12>, csStringFast<12>, CS::Memory::AllocatorMalloc, csArrayElementHandler<CS::Container::HashElement<csStringFast<12>, csStringFast<12> > > >;

// celQuestParams
typedef csHash<csStringBase, csStringBase, CS::Memory::AllocatorMalloc, csArrayElementHandler<CS::Container::HashElement<csStringBase, csStringBase > > > celQuestParams;
%template (celQuestParams) csHash<csStringBase, csStringBase, CS::Memory::AllocatorMalloc, csArrayElementHandler<CS::Container::HashElement<csStringBase, csStringBase > > >;

%ignore iCelPlLayer::SendMessageV;
%ignore iCelPlLayer::CreateEntity (iCelEntityTemplate* factory,const char* name, ...);
%include "physicallayer/pl.h"
%inline %{
iCelPlLayer *csQueryRegistry_iCelPlLayer (iObjectRegistry *object_reg)
{
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  return pl;
}
%}
// fake arrays to handle stuff managed by the physical layer.
// iCelEntityTemplatePlFakeArray
CEL_FAKE_ARRAY(Pl,iCelEntityTemplate,GetEntityTemplateCount,
	GetEntityTemplate,FindEntityTemplate,RemoveEntityTemplate,__noappend__)

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
csPtr<iCelEntity> celCreateEntity(iCelPlLayer *pl, const char *name)
{
  csPrintf("celCreateEntity is deprecated, please use CreateEntity\n");
  return pl->CreateEntity(name,0,0,CEL_PROPCLASS_END);
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

//-----------------------------------------------------------------------------

%ignore celVariableParameterBlock::GetParameter (size_t idx);
%ignore celGenericParameterBlock::GetParameter (size_t idx);
%template (scfGenericParameterBlock) scfImplementation1<celGenericParameterBlock, iCelParameterBlock >;
%template (scfVariableParameterBlock) scfImplementation1<celVariableParameterBlock,iCelParameterBlock >;
%template (scfCombineParameterBlock) scfImplementation1<celCombineParameterBlock,iCelParameterBlock >;
%template (scfOneParameterBlock) scfImplementation1<celOneParameterBlock,iCelParameterBlock >;
%include "celtool/stdparams.h"

//-----------------------------------------------------------------------------

//%ignore iCelPropertyClass::SetProperty;
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
%rename(GetPropertyVector3ByID) iCelPropertyClass::GetPropertyVectorByID (csStringID propertyID, csVector3& v);
%rename(GetPropertyVector2ByID) iCelPropertyClass::GetPropertyVectorByID (csStringID propertyID, csVector2& v);

%ignore iCelPropertyClassList::FindByInterface(scfInterfaceID id, int version) const;
%ignore iCelPropertyClassList::FindByInterfaceAndTag(scfInterfaceID id, int version, const char* tag) const;
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
%extend iCelPropertyClassList {
  celWrapPtr FindByInterface (const char *iface, int iface_ver)
  {
    return celWrapPtr (iface, iface_ver, csRef<iBase> (
      self->FindByInterface(iSCF::SCF->GetInterfaceID (iface), iface_ver)));
  }
  celWrapPtr FindByInterfaceAndTag (const char *iface, int iface_ver, const char* tag)
  {
    return celWrapPtr (iface, iface_ver, csRef<iBase> (
      self->FindByInterfaceAndTag(iSCF::SCF->GetInterfaceID (iface), iface_ver, tag)));
  }
}
/* Clear Typemaps */
%clear csVector3& v;
%clear csVector2& v;
%clear csColor& v;

CELLIST_METHODS(iCelPropertyClassList,iCelPropertyClass)

//-----------------------------------------------------------------------------

%include "propclass/mechsys.h"
CEL_PC(iPcMechanicsSystem, MechanicsSystem, pcphysics.system)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsSystem)

CEL_PC(iPcMechanicsObject, MechanicsObject, pcphysics.object)
CEL_PC_QUERY_CLASSLIST(iPcMechanicsObject)

CEL_PC_QUERY_CLASSLIST(iPcMechanicsJoint)
CEL_PC(iPcMechanicsJoint, MechanicsJoint, pcphysics.joint)

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
CEL_PC(iPcBillboard, Billboard, pc2d.billboard)
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

CEL_PC(iPcRegion, Region, pcworld.region)
//-----------------------------------------------------------------------------

// TODO these need pseudo-dict like interfaces for some stuff
%include "propclass/zone.h"
CEL_PC(iPcZoneManager, ZoneManager, pcworld.zonemanager)

//-----------------------------------------------------------------------------

// TODO this needs dict like interface for binds
%include "propclass/input.h"
CEL_PC(iPcCommandInput, CommandInput, pcinput.standard)

//-----------------------------------------------------------------------------
// TODO this class has a horrible interface, must review carefully
//%cel_attribute(iPcLinearMovement,csVector3&,AngularVelocity)
//%cel_attribute(iPcLinearMovement,csVector3&,Velocity)
//%cel_attribute(iPcLinearMovement,float,Speed)
%include "propclass/linmove.h"
CEL_PC(iPcLinearMovement, LinearMovement, pcmove.linear)

//-----------------------------------------------------------------------------
//%cel_attribute(iPcActorMove,float,MouseMoveSpeed)
%include "propclass/actormove.h"
CEL_PC(iPcActorMove, ActorMove, pcmove.actor.standard)

//-----------------------------------------------------------------------------

%include "propclass/actoranalog.h"
CEL_PC(iPcActorAnalog, ActorAnalog, pcmove.actor.analog)

//-----------------------------------------------------------------------------

// TODO must review distance methods
%include "propclass/camera.h"
//CEL_PC(iPcCamera, Camera, pccamera)

//-----------------------------------------------------------------------------

//%cel_attribute(iPcDefaultCamera,float,TurnSpeed) ONLY HAS SETTER
//%cel_attribute(iPcDefaultCamera,float,SwingCoef) ONLY HAS SETTER
%include "propclass/defcam.h"
CEL_PC(iPcDefaultCamera, DefaultCamera, pccamera.old)

// TODO simpcam missing
%include "propclass/simpcam.h"
CEL_PC(iPcSimpleCamera, SimpleCamera, pccamera.simple)

%include "propclass/newcamera.h"
CEL_PC(iPcNewCamera, NewCamera, pccamera.standard)

%include "propclass/delegcam.h"
CEL_PC(iPcDelegateCamera, DelegateCamera, pccamera.delegate)

//-----------------------------------------------------------------
// modes for delegate camera

%include "propclass/cameras/tracking.h"
CEL_PC(iPcTrackingCamera, TrackingCamera, pccamera.mode.tracking)

//-----------------------------------------------------------------

//-----------------------------------------------------------------------------

// %cel_attribute(iPcMeshSelect,bool,DragPlaneNormal) HAS TWO PARAMETERS
%include "propclass/meshsel.h"
CEL_PC(iPcMeshSelect, MeshSelect, pcobject.mesh.select)

//-----------------------------------------------------------------------------

%include "propclass/mesh.h"
%extend iPcMesh {
  void LoadMesh (const char* factname, const char* filename)
  {
    puts ("Warning! Warning! Deprecated method detected. Please use SetMesh!");
    self->SetMesh (factname, filename);
  }
}
CEL_PC(iPcMesh, Mesh, pcobject.mesh)

//-----------------------------------------------------------------------------

%include "propclass/timer.h"
CEL_PC(iPcTimer, Timer, pctools.timer)

//-----------------------------------------------------------------------------

%include "propclass/trigger.h"
CEL_PC(iPcTrigger, Trigger, pclogic.trigger)

//-----------------------------------------------------------------------------

%include "propclass/steer.h"
CEL_PC(iPcSteer, Steer, pcmove.steer)

//-----------------------------------------------------------------------------

%include "propclass/pathfinder.h"
CEL_PC(iPcPathFinder, PathFinder, pcmove.pathfinder)

//-----------------------------------------------------------------------------

%include "propclass/spawn.h"
CEL_PC(iPcSpawn, Spawn, pclogic.spawn)

//-----------------------------------------------------------------------------
%include "propclass/projectile.h"
CEL_PC(iPcProjectile, Projectile, pcmove.projectile)

//-----------------------------------------------------------------------------

%include "propclass/solid.h"
CEL_PC(iPcSolid, Solid, pcmove.solid)

//-----------------------------------------------------------------------------

%rename(CreateGravityColliderFromMesh) iPcGravity::CreateGravityCollider;
%include "propclass/gravity.h"
CEL_PC(iPcGravity, Gravity, pcmove.gravity)

//-----------------------------------------------------------------------------

%rename(SetPos) iPcMovable::Move;
%include "propclass/move.h"
CEL_PC(iPcMovable, Movable, pcmove.movable)

//-----------------------------------------------------------------------------

// TODO more things to do here
%include "propclass/inv.h"
CEL_PC(iPcInventory, Inventory, pctools.inventory)

// iCelEntityInvFakeArray
CEL_FAKE_ARRAY(Inv,iCelEntity,GetEntityCount,GetEntity,In,RemoveEntity,AddEntity)
//-----------------------------------------------------------------------------

// TODO add dict like interface for characteristics
%include "propclass/chars.h"
CEL_PC(iPcCharacteristics, Characteristics, pctools.inventory.characteristics)

//-----------------------------------------------------------------------------

%include "propclass/tooltip.h"
CEL_PC(iPcTooltip, ToolTip, pc2d.tooltip)

//-----------------------------------------------------------------------------

%include "propclass/sound.h"
CEL_PC(iPcSoundSource, SoundSource, pcsound.source)
CEL_PC(iPcSoundListener, SoundListener, pcsound.listener)

//-----------------------------------------------------------------------------

%include "propclass/prop.h"
CEL_PC(iPcProperties, Properties, pctools.properties)

//-----------------------------------------------------------------------------

%include "propclass/mover.h"
CEL_PC(iPcMover, Mover, pcmove.mover)

//-----------------------------------------------------------------------------

%include "propclass/hover.h"
CEL_PC(iPcHover, Hover, pcvehicle.hover)

//-----------------------------------------------------------------------------

// TODO *very* inconsistent interfaces
%include "propclass/craft.h"
CEL_PC(iPcCraftController, CraftController, pcvehicle.craft)

//-----------------------------------------------------------------------------

// TODO this deserves some pseudo-dict for wheels
%include "propclass/wheeled.h"
CEL_PC(iPcWheeled, Wheeled, pcvehicle.wheeled)

//-----------------------------------------------------------------------------

%include "propclass/meshdeform.h"
CEL_PC(iPcMeshDeform, MeshDeform, pcobject.mesh.deform)

//-----------------------------------------------------------------------------

%include "propclass/damage.h"
CEL_PC(iPcDamage, Damage, pclogic.damage)

//-----------------------------------------------------------------------------

%include "tools/questmanager.h"
%include "propclass/quest.h"
CEL_PC(iPcQuest, Quest, pclogic.quest)

//-----------------------------------------------------------------------------

%include "tools/celgraph.h"

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

#ifndef SWIGIMPORTED
%include "bindings/pfdirector.i"
#endif

#undef INTERFACE_APPLY
#define INTERFACE_APPLY(x) INTERFACE_POST(x)
CEL_APPLY_FOR_EACH_INTERFACE

#if defined(SWIGPYTHON)
  %include "bindings/python/post.i"
#elif defined(SWIGPERL5)
  %include "bindings/perl/post.i"
#elif defined(SWIGRUBY)
  %include "bindings/ruby/post.i"
#elif defined(SWIGTCL8)
  %include "bindings/tcl/post.i"
#elif defined(SWIGJAVA)
  %include "bindings/java/post.i"
#elif defined(SWIGLUA)
  %include "bindings/lua/post.i"
#endif


