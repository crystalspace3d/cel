# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _blcelc

def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


import cspace
class celInitializer(cspace.csInitializer):
    __swig_setmethods__ = {}
    for _s in [cspace.csInitializer]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, celInitializer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.csInitializer]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, celInitializer, name)
    def __repr__(self):
        return "<C celInitializer instance at %s>" % (self.this,)
    __swig_getmethods__["SetupCelPluginDirs"] = lambda x: _blcelc.celInitializer_SetupCelPluginDirs
    if _newclass:SetupCelPluginDirs = staticmethod(_blcelc.celInitializer_SetupCelPluginDirs)
    __swig_getmethods__["_RequestPlugins"] = lambda x: _blcelc.celInitializer__RequestPlugins
    if _newclass:_RequestPlugins = staticmethod(_blcelc.celInitializer__RequestPlugins)
    def __init__(self, *args):
        _swig_setattr(self, celInitializer, 'this', _blcelc.new_celInitializer(*args))
        _swig_setattr(self, celInitializer, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_celInitializer):
        try:
            if self.thisown: destroy(self)
        except: pass

class celInitializerPtr(celInitializer):
    def __init__(self, this):
        _swig_setattr(self, celInitializer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, celInitializer, 'thisown', 0)
        _swig_setattr(self, celInitializer,self.__class__,celInitializer)
_blcelc.celInitializer_swigregister(celInitializerPtr)

celInitializer_SetupCelPluginDirs = _blcelc.celInitializer_SetupCelPluginDirs

celInitializer__RequestPlugins = _blcelc.celInitializer__RequestPlugins

class iCelNewEntityCallback(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelNewEntityCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelNewEntityCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelNewEntityCallback instance at %s>" % (self.this,)
    def NewEntity(*args): return _blcelc.iCelNewEntityCallback_NewEntity(*args)
    def __del__(self, destroy=_blcelc.delete_iCelNewEntityCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelNewEntityCallbackPtr(iCelNewEntityCallback):
    def __init__(self, this):
        _swig_setattr(self, iCelNewEntityCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelNewEntityCallback, 'thisown', 0)
        _swig_setattr(self, iCelNewEntityCallback,self.__class__,iCelNewEntityCallback)
_blcelc.iCelNewEntityCallback_swigregister(iCelNewEntityCallbackPtr)

class iCelEntityRemoveCallback(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityRemoveCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntityRemoveCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelEntityRemoveCallback instance at %s>" % (self.this,)
    def RemoveEntity(*args): return _blcelc.iCelEntityRemoveCallback_RemoveEntity(*args)
    def __del__(self, destroy=_blcelc.delete_iCelEntityRemoveCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelEntityRemoveCallbackPtr(iCelEntityRemoveCallback):
    def __init__(self, this):
        _swig_setattr(self, iCelEntityRemoveCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelEntityRemoveCallback, 'thisown', 0)
        _swig_setattr(self, iCelEntityRemoveCallback,self.__class__,iCelEntityRemoveCallback)
_blcelc.iCelEntityRemoveCallback_swigregister(iCelEntityRemoveCallbackPtr)

class iCelTimerListener(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelTimerListener, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelTimerListener, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelTimerListener instance at %s>" % (self.this,)
    def TickEveryFrame(*args): return _blcelc.iCelTimerListener_TickEveryFrame(*args)
    def TickOnce(*args): return _blcelc.iCelTimerListener_TickOnce(*args)
    def __del__(self, destroy=_blcelc.delete_iCelTimerListener):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelTimerListenerPtr(iCelTimerListener):
    def __init__(self, this):
        _swig_setattr(self, iCelTimerListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelTimerListener, 'thisown', 0)
        _swig_setattr(self, iCelTimerListener,self.__class__,iCelTimerListener)
_blcelc.iCelTimerListener_swigregister(iCelTimerListenerPtr)

class iCelPlLayer(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPlLayer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPlLayer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPlLayer instance at %s>" % (self.this,)
    def CreateEntityInScope(*args): return _blcelc.iCelPlLayer_CreateEntityInScope(*args)
    def RemoveEntity(*args): return _blcelc.iCelPlLayer_RemoveEntity(*args)
    def RemoveEntities(*args): return _blcelc.iCelPlLayer_RemoveEntities(*args)
    def CreateEntityTemplate(*args): return _blcelc.iCelPlLayer_CreateEntityTemplate(*args)
    def RemoveEntityTemplate(*args): return _blcelc.iCelPlLayer_RemoveEntityTemplate(*args)
    def RemoveEntityTemplates(*args): return _blcelc.iCelPlLayer_RemoveEntityTemplates(*args)
    def FindEntityTemplate(*args): return _blcelc.iCelPlLayer_FindEntityTemplate(*args)
    def CreateEntity(*args): return _blcelc.iCelPlLayer_CreateEntity(*args)
    def CreateMessage(*args): return _blcelc.iCelPlLayer_CreateMessage(*args)
    def CreateDataBuffer(*args): return _blcelc.iCelPlLayer_CreateDataBuffer(*args)
    def AttachEntity(*args): return _blcelc.iCelPlLayer_AttachEntity(*args)
    def UnattachEntity(*args): return _blcelc.iCelPlLayer_UnattachEntity(*args)
    def GetEntity(*args): return _blcelc.iCelPlLayer_GetEntity(*args)
    def GetEntityCount(*args): return _blcelc.iCelPlLayer_GetEntityCount(*args)
    def GetEntityByIndex(*args): return _blcelc.iCelPlLayer_GetEntityByIndex(*args)
    def FindEntity(*args): return _blcelc.iCelPlLayer_FindEntity(*args)
    def GetBehaviour(*args): return _blcelc.iCelPlLayer_GetBehaviour(*args)
    def FindAttachedEntity(*args): return _blcelc.iCelPlLayer_FindAttachedEntity(*args)
    def FindNearbyEntities(*args): return _blcelc.iCelPlLayer_FindNearbyEntities(*args)
    def GetHitEntity(*args): return _blcelc.iCelPlLayer_GetHitEntity(*args)
    def CreateEmptyEntityList(*args): return _blcelc.iCelPlLayer_CreateEmptyEntityList(*args)
    def CreateEntityTracker(*args): return _blcelc.iCelPlLayer_CreateEntityTracker(*args)
    def FindEntityTracker(*args): return _blcelc.iCelPlLayer_FindEntityTracker(*args)
    def RemoveEntityTracker(*args): return _blcelc.iCelPlLayer_RemoveEntityTracker(*args)
    def SetEntityAddonAllowed(*args): return _blcelc.iCelPlLayer_SetEntityAddonAllowed(*args)
    def IsEntityAddonAllowed(*args): return _blcelc.iCelPlLayer_IsEntityAddonAllowed(*args)
    def LoadPropertyClassFactory(*args): return _blcelc.iCelPlLayer_LoadPropertyClassFactory(*args)
    def RegisterPropertyClassFactory(*args): return _blcelc.iCelPlLayer_RegisterPropertyClassFactory(*args)
    def UnregisterPropertyClassFactory(*args): return _blcelc.iCelPlLayer_UnregisterPropertyClassFactory(*args)
    def GetPropertyClassFactoryCount(*args): return _blcelc.iCelPlLayer_GetPropertyClassFactoryCount(*args)
    def GetPropertyClassFactory(*args): return _blcelc.iCelPlLayer_GetPropertyClassFactory(*args)
    def FindPropertyClassFactory(*args): return _blcelc.iCelPlLayer_FindPropertyClassFactory(*args)
    def CreatePropertyClass(*args): return _blcelc.iCelPlLayer_CreatePropertyClass(*args)
    def CreateTaggedPropertyClass(*args): return _blcelc.iCelPlLayer_CreateTaggedPropertyClass(*args)
    def RegisterBehaviourLayer(*args): return _blcelc.iCelPlLayer_RegisterBehaviourLayer(*args)
    def UnregisterBehaviourLayer(*args): return _blcelc.iCelPlLayer_UnregisterBehaviourLayer(*args)
    def GetBehaviourLayerCount(*args): return _blcelc.iCelPlLayer_GetBehaviourLayerCount(*args)
    def GetBehaviourLayer(*args): return _blcelc.iCelPlLayer_GetBehaviourLayer(*args)
    def FindBehaviourLayer(*args): return _blcelc.iCelPlLayer_FindBehaviourLayer(*args)
    def Cache(*args): return _blcelc.iCelPlLayer_Cache(*args)
    def Uncache(*args): return _blcelc.iCelPlLayer_Uncache(*args)
    def CleanCache(*args): return _blcelc.iCelPlLayer_CleanCache(*args)
    def FetchStringID(*args): return _blcelc.iCelPlLayer_FetchStringID(*args)
    def FetchString(*args): return _blcelc.iCelPlLayer_FetchString(*args)
    def AddEntityRemoveCallback(*args): return _blcelc.iCelPlLayer_AddEntityRemoveCallback(*args)
    def RemoveEntityRemoveCallback(*args): return _blcelc.iCelPlLayer_RemoveEntityRemoveCallback(*args)
    def AddNewEntityCallback(*args): return _blcelc.iCelPlLayer_AddNewEntityCallback(*args)
    def RemoveNewEntityCallback(*args): return _blcelc.iCelPlLayer_RemoveNewEntityCallback(*args)
    def CallbackEveryFrame(*args): return _blcelc.iCelPlLayer_CallbackEveryFrame(*args)
    def CallbackOnce(*args): return _blcelc.iCelPlLayer_CallbackOnce(*args)
    def RemoveCallbackEveryFrame(*args): return _blcelc.iCelPlLayer_RemoveCallbackEveryFrame(*args)
    def RemoveCallbackOnce(*args): return _blcelc.iCelPlLayer_RemoveCallbackOnce(*args)
    def AddScope(*args): return _blcelc.iCelPlLayer_AddScope(*args)
    def __del__(self, destroy=_blcelc.delete_iCelPlLayer):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelPlLayerPtr(iCelPlLayer):
    def __init__(self, this):
        _swig_setattr(self, iCelPlLayer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelPlLayer, 'thisown', 0)
        _swig_setattr(self, iCelPlLayer,self.__class__,iCelPlLayer)
_blcelc.iCelPlLayer_swigregister(iCelPlLayerPtr)

class iCelEntityTracker(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityTracker, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntityTracker, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelEntityTracker instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iCelEntityTracker_GetName(*args)
    def AddEntity(*args): return _blcelc.iCelEntityTracker_AddEntity(*args)
    def RemoveEntity(*args): return _blcelc.iCelEntityTracker_RemoveEntity(*args)
    def RemoveEntities(*args): return _blcelc.iCelEntityTracker_RemoveEntities(*args)
    def FindNearbyEntities(*args): return _blcelc.iCelEntityTracker_FindNearbyEntities(*args)
    def __del__(self, destroy=_blcelc.delete_iCelEntityTracker):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelEntityTrackerPtr(iCelEntityTracker):
    def __init__(self, this):
        _swig_setattr(self, iCelEntityTracker, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelEntityTracker, 'thisown', 0)
        _swig_setattr(self, iCelEntityTracker,self.__class__,iCelEntityTracker)
_blcelc.iCelEntityTracker_swigregister(iCelEntityTrackerPtr)


csQueryRegistry_iCelPlLayer = _blcelc.csQueryRegistry_iCelPlLayer
class iCelEntity(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntity, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntity, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelEntity instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iCelEntity_QueryObject(*args)
    def GetName(*args): return _blcelc.iCelEntity_GetName(*args)
    def SetName(*args): return _blcelc.iCelEntity_SetName(*args)
    def GetID(*args): return _blcelc.iCelEntity_GetID(*args)
    def SetID(*args): return _blcelc.iCelEntity_SetID(*args)
    def GetPropertyClassList(*args): return _blcelc.iCelEntity_GetPropertyClassList(*args)
    def SetBehaviour(*args): return _blcelc.iCelEntity_SetBehaviour(*args)
    def GetBehaviour(*args): return _blcelc.iCelEntity_GetBehaviour(*args)
    def CreateBehaviour(*args): return _blcelc.iCelEntity_CreateBehaviour(*args)
    def __del__(self, destroy=_blcelc.delete_iCelEntity):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelEntityPtr(iCelEntity):
    def __init__(self, this):
        _swig_setattr(self, iCelEntity, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelEntity, 'thisown', 0)
        _swig_setattr(self, iCelEntity,self.__class__,iCelEntity)
_blcelc.iCelEntity_swigregister(iCelEntityPtr)

class iCelEntityList(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityList, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntityList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelEntityList instance at %s>" % (self.this,)
    def GetCount(*args): return _blcelc.iCelEntityList_GetCount(*args)
    def Get(*args): return _blcelc.iCelEntityList_Get(*args)
    def Add(*args): return _blcelc.iCelEntityList_Add(*args)
    def Remove(*args): return _blcelc.iCelEntityList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iCelEntityList_RemoveAll(*args)
    def Find(*args): return _blcelc.iCelEntityList_Find(*args)
    def FindByName(*args): return _blcelc.iCelEntityList_FindByName(*args)
    def __del__(self, destroy=_blcelc.delete_iCelEntityList):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelEntityListPtr(iCelEntityList):
    def __init__(self, this):
        _swig_setattr(self, iCelEntityList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelEntityList, 'thisown', 0)
        _swig_setattr(self, iCelEntityList,self.__class__,iCelEntityList)
_blcelc.iCelEntityList_swigregister(iCelEntityListPtr)


celRegisterPCFactory = _blcelc.celRegisterPCFactory

celCreateEntity = _blcelc.celCreateEntity

scfQueryInterface_iCelEntity = _blcelc.scfQueryInterface_iCelEntity

celFindNearbyEntities = _blcelc.celFindNearbyEntities
class iCelBlLayer(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelBlLayer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelBlLayer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelBlLayer instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iCelBlLayer_GetName(*args)
    def CreateBehaviour(*args): return _blcelc.iCelBlLayer_CreateBehaviour(*args)
    def __del__(self, destroy=_blcelc.delete_iCelBlLayer):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelBlLayerPtr(iCelBlLayer):
    def __init__(self, this):
        _swig_setattr(self, iCelBlLayer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelBlLayer, 'thisown', 0)
        _swig_setattr(self, iCelBlLayer,self.__class__,iCelBlLayer)
_blcelc.iCelBlLayer_swigregister(iCelBlLayerPtr)


csQueryRegistry_iCelBlLayer = _blcelc.csQueryRegistry_iCelBlLayer
class iCelParameterBlock(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelParameterBlock, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelParameterBlock, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelParameterBlock instance at %s>" % (self.this,)
    def GetParameterCount(*args): return _blcelc.iCelParameterBlock_GetParameterCount(*args)
    def GetParameter(*args): return _blcelc.iCelParameterBlock_GetParameter(*args)
    def __del__(self, destroy=_blcelc.delete_iCelParameterBlock):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelParameterBlockPtr(iCelParameterBlock):
    def __init__(self, this):
        _swig_setattr(self, iCelParameterBlock, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelParameterBlock, 'thisown', 0)
        _swig_setattr(self, iCelParameterBlock,self.__class__,iCelParameterBlock)
_blcelc.iCelParameterBlock_swigregister(iCelParameterBlockPtr)

class iCelBehaviour(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelBehaviour, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelBehaviour, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelBehaviour instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iCelBehaviour_GetName(*args)
    def GetBehaviourLayer(*args): return _blcelc.iCelBehaviour_GetBehaviourLayer(*args)
    def SendMessage(*args): return _blcelc.iCelBehaviour_SendMessage(*args)
    def GetInternalObject(*args): return _blcelc.iCelBehaviour_GetInternalObject(*args)
    def GetPythonObject(*args): return _blcelc.iCelBehaviour_GetPythonObject(*args)
    def __del__(self, destroy=_blcelc.delete_iCelBehaviour):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelBehaviourPtr(iCelBehaviour):
    def __init__(self, this):
        _swig_setattr(self, iCelBehaviour, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelBehaviour, 'thisown', 0)
        _swig_setattr(self, iCelBehaviour,self.__class__,iCelBehaviour)
_blcelc.iCelBehaviour_swigregister(iCelBehaviourPtr)

class iCelPropertyClass(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClass, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClass, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPropertyClass instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iCelPropertyClass_GetName(*args)
    def SetTag(*args): return _blcelc.iCelPropertyClass_SetTag(*args)
    def GetTag(*args): return _blcelc.iCelPropertyClass_GetTag(*args)
    def GetEntity(*args): return _blcelc.iCelPropertyClass_GetEntity(*args)
    def SetEntity(*args): return _blcelc.iCelPropertyClass_SetEntity(*args)
    def SaveFirstPass(*args): return _blcelc.iCelPropertyClass_SaveFirstPass(*args)
    def LoadFirstPass(*args): return _blcelc.iCelPropertyClass_LoadFirstPass(*args)
    def Save(*args): return _blcelc.iCelPropertyClass_Save(*args)
    def Load(*args): return _blcelc.iCelPropertyClass_Load(*args)
    def AddPropertyChangeCallback(*args): return _blcelc.iCelPropertyClass_AddPropertyChangeCallback(*args)
    def RemovePropertyChangeCallback(*args): return _blcelc.iCelPropertyClass_RemovePropertyChangeCallback(*args)
    def GetPropertyOrActionType(*args): return _blcelc.iCelPropertyClass_GetPropertyOrActionType(*args)
    def IsPropertyReadOnly(*args): return _blcelc.iCelPropertyClass_IsPropertyReadOnly(*args)
    def GetPropertyLong(*args): return _blcelc.iCelPropertyClass_GetPropertyLong(*args)
    def GetPropertyFloat(*args): return _blcelc.iCelPropertyClass_GetPropertyFloat(*args)
    def GetPropertyBool(*args): return _blcelc.iCelPropertyClass_GetPropertyBool(*args)
    def GetPropertyString(*args): return _blcelc.iCelPropertyClass_GetPropertyString(*args)
    def GetPropertyVector(*args): return _blcelc.iCelPropertyClass_GetPropertyVector(*args)
    def GetPropertyColor(*args): return _blcelc.iCelPropertyClass_GetPropertyColor(*args)
    def GetPropertyPClass(*args): return _blcelc.iCelPropertyClass_GetPropertyPClass(*args)
    def GetPropertyEntity(*args): return _blcelc.iCelPropertyClass_GetPropertyEntity(*args)
    def PerformAction(*args): return _blcelc.iCelPropertyClass_PerformAction(*args)
    def GetPropertyAndActionCount(*args): return _blcelc.iCelPropertyClass_GetPropertyAndActionCount(*args)
    def GetPropertyOrActionID(*args): return _blcelc.iCelPropertyClass_GetPropertyOrActionID(*args)
    def GetPropertyOrActionDescription(*args): return _blcelc.iCelPropertyClass_GetPropertyOrActionDescription(*args)
    def PropertyClassesHaveChanged(*args): return _blcelc.iCelPropertyClass_PropertyClassesHaveChanged(*args)
    def TickEveryFrame(*args): return _blcelc.iCelPropertyClass_TickEveryFrame(*args)
    def TickOnce(*args): return _blcelc.iCelPropertyClass_TickOnce(*args)
    def SetPropertyLong(*args): return _blcelc.iCelPropertyClass_SetPropertyLong(*args)
    def SetPropertyFloat(*args): return _blcelc.iCelPropertyClass_SetPropertyFloat(*args)
    def SetPropertyBool(*args): return _blcelc.iCelPropertyClass_SetPropertyBool(*args)
    def SetPropertyString(*args): return _blcelc.iCelPropertyClass_SetPropertyString(*args)
    def SetPropertyVector3(*args): return _blcelc.iCelPropertyClass_SetPropertyVector3(*args)
    def __del__(self, destroy=_blcelc.delete_iCelPropertyClass):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelPropertyClassPtr(iCelPropertyClass):
    def __init__(self, this):
        _swig_setattr(self, iCelPropertyClass, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelPropertyClass, 'thisown', 0)
        _swig_setattr(self, iCelPropertyClass,self.__class__,iCelPropertyClass)
_blcelc.iCelPropertyClass_swigregister(iCelPropertyClassPtr)

class iCelPropertyClassList(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClassList, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClassList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPropertyClassList instance at %s>" % (self.this,)
    def GetCount(*args): return _blcelc.iCelPropertyClassList_GetCount(*args)
    def Get(*args): return _blcelc.iCelPropertyClassList_Get(*args)
    def Add(*args): return _blcelc.iCelPropertyClassList_Add(*args)
    def Remove(*args): return _blcelc.iCelPropertyClassList_Remove(*args)
    def RemoveByInterface(*args): return _blcelc.iCelPropertyClassList_RemoveByInterface(*args)
    def RemoveByInterfaceAndTag(*args): return _blcelc.iCelPropertyClassList_RemoveByInterfaceAndTag(*args)
    def RemoveAll(*args): return _blcelc.iCelPropertyClassList_RemoveAll(*args)
    def Find(*args): return _blcelc.iCelPropertyClassList_Find(*args)
    def FindByName(*args): return _blcelc.iCelPropertyClassList_FindByName(*args)
    def FindByNameAndTag(*args): return _blcelc.iCelPropertyClassList_FindByNameAndTag(*args)
    def FindByInterface(*args): return _blcelc.iCelPropertyClassList_FindByInterface(*args)
    def FindByInterfaceAndTag(*args): return _blcelc.iCelPropertyClassList_FindByInterfaceAndTag(*args)
    def __del__(self, destroy=_blcelc.delete_iCelPropertyClassList):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelPropertyClassListPtr(iCelPropertyClassList):
    def __init__(self, this):
        _swig_setattr(self, iCelPropertyClassList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelPropertyClassList, 'thisown', 0)
        _swig_setattr(self, iCelPropertyClassList,self.__class__,iCelPropertyClassList)
_blcelc.iCelPropertyClassList_swigregister(iCelPropertyClassListPtr)

class iPcDynamicSystem(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcDynamicSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcDynamicSystem, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcDynamicSystem instance at %s>" % (self.this,)
    def GetDynamicSystem(*args): return _blcelc.iPcDynamicSystem_GetDynamicSystem(*args)
    def SetStepTime(*args): return _blcelc.iPcDynamicSystem_SetStepTime(*args)
    def GetStepTime(*args): return _blcelc.iPcDynamicSystem_GetStepTime(*args)
    def AddForceDuration(*args): return _blcelc.iPcDynamicSystem_AddForceDuration(*args)
    def AddForceFrame(*args): return _blcelc.iPcDynamicSystem_AddForceFrame(*args)
    def ClearForces(*args): return _blcelc.iPcDynamicSystem_ClearForces(*args)
    def ClearAllForces(*args): return _blcelc.iPcDynamicSystem_ClearAllForces(*args)
    def __del__(self, destroy=_blcelc.delete_iPcDynamicSystem):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcDynamicSystemPtr(iPcDynamicSystem):
    def __init__(self, this):
        _swig_setattr(self, iPcDynamicSystem, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcDynamicSystem, 'thisown', 0)
        _swig_setattr(self, iPcDynamicSystem,self.__class__,iPcDynamicSystem)
_blcelc.iPcDynamicSystem_swigregister(iPcDynamicSystemPtr)

class iPcDynamicBody(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcDynamicBody, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcDynamicBody, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcDynamicBody instance at %s>" % (self.this,)
    def SetMesh(*args): return _blcelc.iPcDynamicBody_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcDynamicBody_GetMesh(*args)
    def SetDynamicSystem(*args): return _blcelc.iPcDynamicBody_SetDynamicSystem(*args)
    def GetDynamicSystem(*args): return _blcelc.iPcDynamicBody_GetDynamicSystem(*args)
    def GetBody(*args): return _blcelc.iPcDynamicBody_GetBody(*args)
    def SetParameters(*args): return _blcelc.iPcDynamicBody_SetParameters(*args)
    def MakeStatic(*args): return _blcelc.iPcDynamicBody_MakeStatic(*args)
    def IsStatic(*args): return _blcelc.iPcDynamicBody_IsStatic(*args)
    def AttachColliderSphere(*args): return _blcelc.iPcDynamicBody_AttachColliderSphere(*args)
    def AttachColliderCylinder(*args): return _blcelc.iPcDynamicBody_AttachColliderCylinder(*args)
    def AttachColliderBox(*args): return _blcelc.iPcDynamicBody_AttachColliderBox(*args)
    def AttachColliderPlane(*args): return _blcelc.iPcDynamicBody_AttachColliderPlane(*args)
    def AttachColliderMesh(*args): return _blcelc.iPcDynamicBody_AttachColliderMesh(*args)
    def AddForceOnce(*args): return _blcelc.iPcDynamicBody_AddForceOnce(*args)
    def AddForceDuration(*args): return _blcelc.iPcDynamicBody_AddForceDuration(*args)
    def AddForceFrame(*args): return _blcelc.iPcDynamicBody_AddForceFrame(*args)
    def ClearForces(*args): return _blcelc.iPcDynamicBody_ClearForces(*args)
    def __del__(self, destroy=_blcelc.delete_iPcDynamicBody):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcDynamicBodyPtr(iPcDynamicBody):
    def __init__(self, this):
        _swig_setattr(self, iPcDynamicBody, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcDynamicBody, 'thisown', 0)
        _swig_setattr(self, iPcDynamicBody,self.__class__,iPcDynamicBody)
_blcelc.iPcDynamicBody_swigregister(iPcDynamicBodyPtr)


celCreateDynamicSystem = _blcelc.celCreateDynamicSystem

celGetDynamicSystem = _blcelc.celGetDynamicSystem

scfQuery_iPcDynamicSystem = _blcelc.scfQuery_iPcDynamicSystem

scfQueryPC_iPcDynamicSystem = _blcelc.scfQueryPC_iPcDynamicSystem

celCreateDynamicBody = _blcelc.celCreateDynamicBody

celGetDynamicBody = _blcelc.celGetDynamicBody

scfQuery_iPcDynamicBody = _blcelc.scfQuery_iPcDynamicBody

scfQueryPC_iPcDynamicBody = _blcelc.scfQueryPC_iPcDynamicBody
class iPcMechanicsSystem(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsSystem, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMechanicsSystem instance at %s>" % (self.this,)
    def SetDynamicSystem(*args): return _blcelc.iPcMechanicsSystem_SetDynamicSystem(*args)
    def GetDynamicSystem(*args): return _blcelc.iPcMechanicsSystem_GetDynamicSystem(*args)
    def SetStepTime(*args): return _blcelc.iPcMechanicsSystem_SetStepTime(*args)
    def GetStepTime(*args): return _blcelc.iPcMechanicsSystem_GetStepTime(*args)
    def SetGravity(*args): return _blcelc.iPcMechanicsSystem_SetGravity(*args)
    def GetGravity(*args): return _blcelc.iPcMechanicsSystem_GetGravity(*args)
    def AddForceDuration(*args): return _blcelc.iPcMechanicsSystem_AddForceDuration(*args)
    def AddForceFrame(*args): return _blcelc.iPcMechanicsSystem_AddForceFrame(*args)
    def AddForceTagged(*args): return _blcelc.iPcMechanicsSystem_AddForceTagged(*args)
    def RemoveForceTagged(*args): return _blcelc.iPcMechanicsSystem_RemoveForceTagged(*args)
    def ClearForces(*args): return _blcelc.iPcMechanicsSystem_ClearForces(*args)
    def ClearAllForces(*args): return _blcelc.iPcMechanicsSystem_ClearAllForces(*args)
    def CreateBody(*args): return _blcelc.iPcMechanicsSystem_CreateBody(*args)
    def RemoveBody(*args): return _blcelc.iPcMechanicsSystem_RemoveBody(*args)
    def AddBodyToGroup(*args): return _blcelc.iPcMechanicsSystem_AddBodyToGroup(*args)
    def RemoveBodyFromGroup(*args): return _blcelc.iPcMechanicsSystem_RemoveBodyFromGroup(*args)
    def CreateJoint(*args): return _blcelc.iPcMechanicsSystem_CreateJoint(*args)
    def RemoveJoint(*args): return _blcelc.iPcMechanicsSystem_RemoveJoint(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMechanicsSystem):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMechanicsSystemPtr(iPcMechanicsSystem):
    def __init__(self, this):
        _swig_setattr(self, iPcMechanicsSystem, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMechanicsSystem, 'thisown', 0)
        _swig_setattr(self, iPcMechanicsSystem,self.__class__,iPcMechanicsSystem)
_blcelc.iPcMechanicsSystem_swigregister(iPcMechanicsSystemPtr)

class iPcMechanicsObject(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsObject, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsObject, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMechanicsObject instance at %s>" % (self.this,)
    def SetMesh(*args): return _blcelc.iPcMechanicsObject_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcMechanicsObject_GetMesh(*args)
    def SetMechanicsSystem(*args): return _blcelc.iPcMechanicsObject_SetMechanicsSystem(*args)
    def GetMechanicsSystem(*args): return _blcelc.iPcMechanicsObject_GetMechanicsSystem(*args)
    def GetBody(*args): return _blcelc.iPcMechanicsObject_GetBody(*args)
    def SetFriction(*args): return _blcelc.iPcMechanicsObject_SetFriction(*args)
    def SetMass(*args): return _blcelc.iPcMechanicsObject_SetMass(*args)
    def SetElasticity(*args): return _blcelc.iPcMechanicsObject_SetElasticity(*args)
    def SetDensity(*args): return _blcelc.iPcMechanicsObject_SetDensity(*args)
    def SetSoftness(*args): return _blcelc.iPcMechanicsObject_SetSoftness(*args)
    def SetLift(*args): return _blcelc.iPcMechanicsObject_SetLift(*args)
    def SetDrag(*args): return _blcelc.iPcMechanicsObject_SetDrag(*args)
    def GetFriction(*args): return _blcelc.iPcMechanicsObject_GetFriction(*args)
    def GetMass(*args): return _blcelc.iPcMechanicsObject_GetMass(*args)
    def GetElasticity(*args): return _blcelc.iPcMechanicsObject_GetElasticity(*args)
    def GetDensity(*args): return _blcelc.iPcMechanicsObject_GetDensity(*args)
    def GetSoftness(*args): return _blcelc.iPcMechanicsObject_GetSoftness(*args)
    def GetLift(*args): return _blcelc.iPcMechanicsObject_GetLift(*args)
    def GetDrag(*args): return _blcelc.iPcMechanicsObject_GetDrag(*args)
    def SetLinearVelocity(*args): return _blcelc.iPcMechanicsObject_SetLinearVelocity(*args)
    def SetAngularVelocity(*args): return _blcelc.iPcMechanicsObject_SetAngularVelocity(*args)
    def GetLinearVelocity(*args): return _blcelc.iPcMechanicsObject_GetLinearVelocity(*args)
    def GetAngularVelocity(*args): return _blcelc.iPcMechanicsObject_GetAngularVelocity(*args)
    def MakeStatic(*args): return _blcelc.iPcMechanicsObject_MakeStatic(*args)
    def IsStatic(*args): return _blcelc.iPcMechanicsObject_IsStatic(*args)
    def AttachColliderSphere(*args): return _blcelc.iPcMechanicsObject_AttachColliderSphere(*args)
    def AttachColliderCylinder(*args): return _blcelc.iPcMechanicsObject_AttachColliderCylinder(*args)
    def AttachColliderBox(*args): return _blcelc.iPcMechanicsObject_AttachColliderBox(*args)
    def AttachColliderPlane(*args): return _blcelc.iPcMechanicsObject_AttachColliderPlane(*args)
    def AttachColliderMesh(*args): return _blcelc.iPcMechanicsObject_AttachColliderMesh(*args)
    def AddForceOnce(*args): return _blcelc.iPcMechanicsObject_AddForceOnce(*args)
    def AddForceDuration(*args): return _blcelc.iPcMechanicsObject_AddForceDuration(*args)
    def AddForceFrame(*args): return _blcelc.iPcMechanicsObject_AddForceFrame(*args)
    def AddForceTagged(*args): return _blcelc.iPcMechanicsObject_AddForceTagged(*args)
    def RemoveForceTagged(*args): return _blcelc.iPcMechanicsObject_RemoveForceTagged(*args)
    def ClearForces(*args): return _blcelc.iPcMechanicsObject_ClearForces(*args)
    def AddToGroup(*args): return _blcelc.iPcMechanicsObject_AddToGroup(*args)
    def RemoveFromGroup(*args): return _blcelc.iPcMechanicsObject_RemoveFromGroup(*args)
    def CreateJoint(*args): return _blcelc.iPcMechanicsObject_CreateJoint(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMechanicsObject):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMechanicsObjectPtr(iPcMechanicsObject):
    def __init__(self, this):
        _swig_setattr(self, iPcMechanicsObject, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMechanicsObject, 'thisown', 0)
        _swig_setattr(self, iPcMechanicsObject,self.__class__,iPcMechanicsObject)
_blcelc.iPcMechanicsObject_swigregister(iPcMechanicsObjectPtr)


celCreateMechanicsSystem = _blcelc.celCreateMechanicsSystem

celGetMechanicsSystem = _blcelc.celGetMechanicsSystem

scfQuery_iPcMechanicsSystem = _blcelc.scfQuery_iPcMechanicsSystem

scfQueryPC_iPcMechanicsSystem = _blcelc.scfQueryPC_iPcMechanicsSystem

celCreateMechanicsObject = _blcelc.celCreateMechanicsObject

celGetMechanicsObject = _blcelc.celGetMechanicsObject

scfQuery_iPcMechanicsObject = _blcelc.scfQuery_iPcMechanicsObject

scfQueryPC_iPcMechanicsObject = _blcelc.scfQueryPC_iPcMechanicsObject
class iPcMechanicsThruster(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsThruster, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsThruster, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMechanicsThruster instance at %s>" % (self.this,)
    def SetMechanicsObject(*args): return _blcelc.iPcMechanicsThruster_SetMechanicsObject(*args)
    def GetMechanicsObject(*args): return _blcelc.iPcMechanicsThruster_GetMechanicsObject(*args)
    def SetPosition(*args): return _blcelc.iPcMechanicsThruster_SetPosition(*args)
    def GetPosition(*args): return _blcelc.iPcMechanicsThruster_GetPosition(*args)
    def SetOrientation(*args): return _blcelc.iPcMechanicsThruster_SetOrientation(*args)
    def GetOrientation(*args): return _blcelc.iPcMechanicsThruster_GetOrientation(*args)
    def SetMaxThrust(*args): return _blcelc.iPcMechanicsThruster_SetMaxThrust(*args)
    def GetMaxThrust(*args): return _blcelc.iPcMechanicsThruster_GetMaxThrust(*args)
    def GetThrustForce(*args): return _blcelc.iPcMechanicsThruster_GetThrustForce(*args)
    def AvailableThrust(*args): return _blcelc.iPcMechanicsThruster_AvailableThrust(*args)
    def ThrustChange(*args): return _blcelc.iPcMechanicsThruster_ThrustChange(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMechanicsThruster):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMechanicsThrusterPtr(iPcMechanicsThruster):
    def __init__(self, this):
        _swig_setattr(self, iPcMechanicsThruster, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMechanicsThruster, 'thisown', 0)
        _swig_setattr(self, iPcMechanicsThruster,self.__class__,iPcMechanicsThruster)
_blcelc.iPcMechanicsThruster_swigregister(iPcMechanicsThrusterPtr)

CEL_AT_NONE = _blcelc.CEL_AT_NONE
CEL_AT_ROTATION = _blcelc.CEL_AT_ROTATION
CEL_AT_TRANSLATION = _blcelc.CEL_AT_TRANSLATION
class iPcMechanicsBalancedGroup(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsBalancedGroup, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsBalancedGroup, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMechanicsBalancedGroup instance at %s>" % (self.this,)
    def SetType(*args): return _blcelc.iPcMechanicsBalancedGroup_SetType(*args)
    def GetType(*args): return _blcelc.iPcMechanicsBalancedGroup_GetType(*args)
    def AddThruster(*args): return _blcelc.iPcMechanicsBalancedGroup_AddThruster(*args)
    def RemoveThruster(*args): return _blcelc.iPcMechanicsBalancedGroup_RemoveThruster(*args)
    def GetThruster(*args): return _blcelc.iPcMechanicsBalancedGroup_GetThruster(*args)
    def AvailableThrust(*args): return _blcelc.iPcMechanicsBalancedGroup_AvailableThrust(*args)
    def AvailableThrustForce(*args): return _blcelc.iPcMechanicsBalancedGroup_AvailableThrustForce(*args)
    def ChangeThrust(*args): return _blcelc.iPcMechanicsBalancedGroup_ChangeThrust(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMechanicsBalancedGroup):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMechanicsBalancedGroupPtr(iPcMechanicsBalancedGroup):
    def __init__(self, this):
        _swig_setattr(self, iPcMechanicsBalancedGroup, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMechanicsBalancedGroup, 'thisown', 0)
        _swig_setattr(self, iPcMechanicsBalancedGroup,self.__class__,iPcMechanicsBalancedGroup)
_blcelc.iPcMechanicsBalancedGroup_swigregister(iPcMechanicsBalancedGroupPtr)

class iPcMechanicsThrusterController(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsThrusterController, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsThrusterController, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMechanicsThrusterController instance at %s>" % (self.this,)
    def SetMechanicsObject(*args): return _blcelc.iPcMechanicsThrusterController_SetMechanicsObject(*args)
    def GetMechanicsObject(*args): return _blcelc.iPcMechanicsThrusterController_GetMechanicsObject(*args)
    def AddAxis(*args): return _blcelc.iPcMechanicsThrusterController_AddAxis(*args)
    def GetAxis(*args): return _blcelc.iPcMechanicsThrusterController_GetAxis(*args)
    def GetAxisType(*args): return _blcelc.iPcMechanicsThrusterController_GetAxisType(*args)
    def GetAxisVelocity(*args): return _blcelc.iPcMechanicsThrusterController_GetAxisVelocity(*args)
    def GetAxisMaxForce(*args): return _blcelc.iPcMechanicsThrusterController_GetAxisMaxForce(*args)
    def GetAxisMaxThrust(*args): return _blcelc.iPcMechanicsThrusterController_GetAxisMaxThrust(*args)
    def AddBalancedGroup(*args): return _blcelc.iPcMechanicsThrusterController_AddBalancedGroup(*args)
    def RemoveBalancedGroup(*args): return _blcelc.iPcMechanicsThrusterController_RemoveBalancedGroup(*args)
    def ApplyThrust(*args): return _blcelc.iPcMechanicsThrusterController_ApplyThrust(*args)
    def CancelThrust(*args): return _blcelc.iPcMechanicsThrusterController_CancelThrust(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMechanicsThrusterController):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMechanicsThrusterControllerPtr(iPcMechanicsThrusterController):
    def __init__(self, this):
        _swig_setattr(self, iPcMechanicsThrusterController, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMechanicsThrusterController, 'thisown', 0)
        _swig_setattr(self, iPcMechanicsThrusterController,self.__class__,iPcMechanicsThrusterController)
_blcelc.iPcMechanicsThrusterController_swigregister(iPcMechanicsThrusterControllerPtr)


celCreateMechanicsThrusterReactionary = _blcelc.celCreateMechanicsThrusterReactionary

celGetMechanicsThrusterReactionary = _blcelc.celGetMechanicsThrusterReactionary

scfQuery_iPcMechanicsThruster = _blcelc.scfQuery_iPcMechanicsThruster

scfQueryPC_iPcMechanicsThruster = _blcelc.scfQueryPC_iPcMechanicsThruster

celCreateMechanicsBalancedGroup = _blcelc.celCreateMechanicsBalancedGroup

celGetMechanicsBalancedGroup = _blcelc.celGetMechanicsBalancedGroup

scfQuery_iPcMechanicsBalancedGroup = _blcelc.scfQuery_iPcMechanicsBalancedGroup

scfQueryPC_iPcMechanicsBalancedGroup = _blcelc.scfQueryPC_iPcMechanicsBalancedGroup

celCreateMechanicsThrusterController = _blcelc.celCreateMechanicsThrusterController

celGetMechanicsThrusterController = _blcelc.celGetMechanicsThrusterController

scfQuery_iPcMechanicsThrusterController = _blcelc.scfQuery_iPcMechanicsThrusterController

scfQueryPC_iPcMechanicsThrusterController = _blcelc.scfQueryPC_iPcMechanicsThrusterController
class iBillboardLayer(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboardLayer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboardLayer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBillboardLayer instance at %s>" % (self.this,)
    def GetOffset(*args): return _blcelc.iBillboardLayer_GetOffset(*args)
    def SetOffset(*args): return _blcelc.iBillboardLayer_SetOffset(*args)
    def Move(*args): return _blcelc.iBillboardLayer_Move(*args)
    def GetName(*args): return _blcelc.iBillboardLayer_GetName(*args)
    def __del__(self, destroy=_blcelc.delete_iBillboardLayer):
        try:
            if self.thisown: destroy(self)
        except: pass

class iBillboardLayerPtr(iBillboardLayer):
    def __init__(self, this):
        _swig_setattr(self, iBillboardLayer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBillboardLayer, 'thisown', 0)
        _swig_setattr(self, iBillboardLayer,self.__class__,iBillboardLayer)
_blcelc.iBillboardLayer_swigregister(iBillboardLayerPtr)

class iBillboardEventHandler(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboardEventHandler, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboardEventHandler, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBillboardEventHandler instance at %s>" % (self.this,)
    def Select(*args): return _blcelc.iBillboardEventHandler_Select(*args)
    def MouseMove(*args): return _blcelc.iBillboardEventHandler_MouseMove(*args)
    def Unselect(*args): return _blcelc.iBillboardEventHandler_Unselect(*args)
    def DoubleClick(*args): return _blcelc.iBillboardEventHandler_DoubleClick(*args)
    def __del__(self, destroy=_blcelc.delete_iBillboardEventHandler):
        try:
            if self.thisown: destroy(self)
        except: pass

class iBillboardEventHandlerPtr(iBillboardEventHandler):
    def __init__(self, this):
        _swig_setattr(self, iBillboardEventHandler, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBillboardEventHandler, 'thisown', 0)
        _swig_setattr(self, iBillboardEventHandler,self.__class__,iBillboardEventHandler)
_blcelc.iBillboardEventHandler_swigregister(iBillboardEventHandlerPtr)

CEL_BILLBOARD_VISIBLE = _blcelc.CEL_BILLBOARD_VISIBLE
CEL_BILLBOARD_MOVABLE = _blcelc.CEL_BILLBOARD_MOVABLE
CEL_BILLBOARD_CLICKABLE = _blcelc.CEL_BILLBOARD_CLICKABLE
CEL_BILLBOARD_RESTACK = _blcelc.CEL_BILLBOARD_RESTACK
class iBillboard(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboard, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboard, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBillboard instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iBillboard_GetName(*args)
    def GetFlags(*args): return _blcelc.iBillboard_GetFlags(*args)
    def SetMaterialName(*args): return _blcelc.iBillboard_SetMaterialName(*args)
    def SetMaterialNameFast(*args): return _blcelc.iBillboard_SetMaterialNameFast(*args)
    def GetMaterialName(*args): return _blcelc.iBillboard_GetMaterialName(*args)
    def SetSize(*args): return _blcelc.iBillboard_SetSize(*args)
    def GetSize(*args): return _blcelc.iBillboard_GetSize(*args)
    def GetImageSize(*args): return _blcelc.iBillboard_GetImageSize(*args)
    def SetPosition(*args): return _blcelc.iBillboard_SetPosition(*args)
    def MoveToPosition(*args): return _blcelc.iBillboard_MoveToPosition(*args)
    def GetPosition(*args): return _blcelc.iBillboard_GetPosition(*args)
    def Move(*args): return _blcelc.iBillboard_Move(*args)
    def StackTop(*args): return _blcelc.iBillboard_StackTop(*args)
    def StackBottom(*args): return _blcelc.iBillboard_StackBottom(*args)
    def StackUp(*args): return _blcelc.iBillboard_StackUp(*args)
    def StackDown(*args): return _blcelc.iBillboard_StackDown(*args)
    def SetUVTopLeft(*args): return _blcelc.iBillboard_SetUVTopLeft(*args)
    def GetUVTopLeft(*args): return _blcelc.iBillboard_GetUVTopLeft(*args)
    def SetUVBottomRight(*args): return _blcelc.iBillboard_SetUVBottomRight(*args)
    def GetUVBottomRight(*args): return _blcelc.iBillboard_GetUVBottomRight(*args)
    def SetColor(*args): return _blcelc.iBillboard_SetColor(*args)
    def GetColor(*args): return _blcelc.iBillboard_GetColor(*args)
    def SetLayer(*args): return _blcelc.iBillboard_SetLayer(*args)
    def GetLayer(*args): return _blcelc.iBillboard_GetLayer(*args)
    def AddEventHandler(*args): return _blcelc.iBillboard_AddEventHandler(*args)
    def RemoveEventHandler(*args): return _blcelc.iBillboard_RemoveEventHandler(*args)
    def __del__(self, destroy=_blcelc.delete_iBillboard):
        try:
            if self.thisown: destroy(self)
        except: pass

class iBillboardPtr(iBillboard):
    def __init__(self, this):
        _swig_setattr(self, iBillboard, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBillboard, 'thisown', 0)
        _swig_setattr(self, iBillboard,self.__class__,iBillboard)
_blcelc.iBillboard_swigregister(iBillboardPtr)

class iBillboardManager(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboardManager, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboardManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBillboardManager instance at %s>" % (self.this,)
    def CreateBillboard(*args): return _blcelc.iBillboardManager_CreateBillboard(*args)
    def FindBillboard(*args): return _blcelc.iBillboardManager_FindBillboard(*args)
    def RemoveBillboard(*args): return _blcelc.iBillboardManager_RemoveBillboard(*args)
    def GetBillboardCount(*args): return _blcelc.iBillboardManager_GetBillboardCount(*args)
    def GetBillboard(*args): return _blcelc.iBillboardManager_GetBillboard(*args)
    def CreateBillboardLayer(*args): return _blcelc.iBillboardManager_CreateBillboardLayer(*args)
    def FindBillboardLayer(*args): return _blcelc.iBillboardManager_FindBillboardLayer(*args)
    def RemoveBillboardLayer(*args): return _blcelc.iBillboardManager_RemoveBillboardLayer(*args)
    def GetBillboardLayerCount(*args): return _blcelc.iBillboardManager_GetBillboardLayerCount(*args)
    def GetBillboardLayer(*args): return _blcelc.iBillboardManager_GetBillboardLayer(*args)
    def RemoveAll(*args): return _blcelc.iBillboardManager_RemoveAll(*args)
    def SetFlags(*args): return _blcelc.iBillboardManager_SetFlags(*args)
    def StackTop(*args): return _blcelc.iBillboardManager_StackTop(*args)
    def StackBottom(*args): return _blcelc.iBillboardManager_StackBottom(*args)
    def StackUp(*args): return _blcelc.iBillboardManager_StackUp(*args)
    def StackDown(*args): return _blcelc.iBillboardManager_StackDown(*args)
    def StackBefore(*args): return _blcelc.iBillboardManager_StackBefore(*args)
    def StackAfter(*args): return _blcelc.iBillboardManager_StackAfter(*args)
    def TestCollision(*args): return _blcelc.iBillboardManager_TestCollision(*args)
    def __del__(self, destroy=_blcelc.delete_iBillboardManager):
        try:
            if self.thisown: destroy(self)
        except: pass

class iBillboardManagerPtr(iBillboardManager):
    def __init__(self, this):
        _swig_setattr(self, iBillboardManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBillboardManager, 'thisown', 0)
        _swig_setattr(self, iBillboardManager,self.__class__,iBillboardManager)
_blcelc.iBillboardManager_swigregister(iBillboardManagerPtr)

class iPcBillboard(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcBillboard, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcBillboard, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcBillboard instance at %s>" % (self.this,)
    def SetBillboardName(*args): return _blcelc.iPcBillboard_SetBillboardName(*args)
    def GetBillboardName(*args): return _blcelc.iPcBillboard_GetBillboardName(*args)
    def GetBillboard(*args): return _blcelc.iPcBillboard_GetBillboard(*args)
    def EnableEvents(*args): return _blcelc.iPcBillboard_EnableEvents(*args)
    def AreEventsEnabled(*args): return _blcelc.iPcBillboard_AreEventsEnabled(*args)
    def __del__(self, destroy=_blcelc.delete_iPcBillboard):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcBillboardPtr(iPcBillboard):
    def __init__(self, this):
        _swig_setattr(self, iPcBillboard, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcBillboard, 'thisown', 0)
        _swig_setattr(self, iPcBillboard,self.__class__,iPcBillboard)
_blcelc.iPcBillboard_swigregister(iPcBillboardPtr)


celCreateBillboard = _blcelc.celCreateBillboard

celGetBillboard = _blcelc.celGetBillboard

scfQuery_iPcBillboard = _blcelc.scfQuery_iPcBillboard

scfQueryPC_iPcBillboard = _blcelc.scfQueryPC_iPcBillboard
class iPcRegion(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcRegion, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcRegion, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcRegion instance at %s>" % (self.this,)
    def SetWorldFile(*args): return _blcelc.iPcRegion_SetWorldFile(*args)
    def GetWorldDir(*args): return _blcelc.iPcRegion_GetWorldDir(*args)
    def GetWorldFile(*args): return _blcelc.iPcRegion_GetWorldFile(*args)
    def SetRegionName(*args): return _blcelc.iPcRegion_SetRegionName(*args)
    def GetRegionName(*args): return _blcelc.iPcRegion_GetRegionName(*args)
    def CreateEmptySector(*args): return _blcelc.iPcRegion_CreateEmptySector(*args)
    def Load(*args): return _blcelc.iPcRegion_Load(*args)
    def Unload(*args): return _blcelc.iPcRegion_Unload(*args)
    def FindSector(*args): return _blcelc.iPcRegion_FindSector(*args)
    def GetStartSector(*args): return _blcelc.iPcRegion_GetStartSector(*args)
    def GetStartPosition(*args): return _blcelc.iPcRegion_GetStartPosition(*args)
    def PointCamera(*args): return _blcelc.iPcRegion_PointCamera(*args)
    def GetRegion(*args): return _blcelc.iPcRegion_GetRegion(*args)
    def LoadWorld(*args): return _blcelc.iPcRegion_LoadWorld(*args)
    def __del__(self, destroy=_blcelc.delete_iPcRegion):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcRegionPtr(iPcRegion):
    def __init__(self, this):
        _swig_setattr(self, iPcRegion, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcRegion, 'thisown', 0)
        _swig_setattr(self, iPcRegion,self.__class__,iPcRegion)
_blcelc.iPcRegion_swigregister(iPcRegionPtr)


celCreateRegion = _blcelc.celCreateRegion

Region = _blcelc.Region

scfQuery_iPcRegion = _blcelc.scfQuery_iPcRegion

scfQueryPC_iPcRegion = _blcelc.scfQueryPC_iPcRegion
class iCelMapFile(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelMapFile, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelMapFile, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelMapFile instance at %s>" % (self.this,)
    def SetPath(*args): return _blcelc.iCelMapFile_SetPath(*args)
    def SetFile(*args): return _blcelc.iCelMapFile_SetFile(*args)
    def GetPath(*args): return _blcelc.iCelMapFile_GetPath(*args)
    def GetFile(*args): return _blcelc.iCelMapFile_GetFile(*args)
    def SetSectorName(*args): return _blcelc.iCelMapFile_SetSectorName(*args)
    def GetSectorName(*args): return _blcelc.iCelMapFile_GetSectorName(*args)
    def __del__(self, destroy=_blcelc.delete_iCelMapFile):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelMapFilePtr(iCelMapFile):
    def __init__(self, this):
        _swig_setattr(self, iCelMapFile, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelMapFile, 'thisown', 0)
        _swig_setattr(self, iCelMapFile,self.__class__,iCelMapFile)
_blcelc.iCelMapFile_swigregister(iCelMapFilePtr)

class iCelRegion(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelRegion, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelRegion, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelRegion instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iCelRegion_GetName(*args)
    def SetCachePath(*args): return _blcelc.iCelRegion_SetCachePath(*args)
    def GetCachePath(*args): return _blcelc.iCelRegion_GetCachePath(*args)
    def CreateMapFile(*args): return _blcelc.iCelRegion_CreateMapFile(*args)
    def GetMapFileCount(*args): return _blcelc.iCelRegion_GetMapFileCount(*args)
    def GetMapFile(*args): return _blcelc.iCelRegion_GetMapFile(*args)
    def RemoveMapFile(*args): return _blcelc.iCelRegion_RemoveMapFile(*args)
    def RemoveAllMapFiles(*args): return _blcelc.iCelRegion_RemoveAllMapFiles(*args)
    def AssociateEntity(*args): return _blcelc.iCelRegion_AssociateEntity(*args)
    def __del__(self, destroy=_blcelc.delete_iCelRegion):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelRegionPtr(iCelRegion):
    def __init__(self, this):
        _swig_setattr(self, iCelRegion, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelRegion, 'thisown', 0)
        _swig_setattr(self, iCelRegion,self.__class__,iCelRegion)
_blcelc.iCelRegion_swigregister(iCelRegionPtr)

class iCelZone(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelZone, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelZone, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelZone instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iCelZone_GetName(*args)
    def LinkRegion(*args): return _blcelc.iCelZone_LinkRegion(*args)
    def GetRegionCount(*args): return _blcelc.iCelZone_GetRegionCount(*args)
    def GetRegion(*args): return _blcelc.iCelZone_GetRegion(*args)
    def FindRegion(*args): return _blcelc.iCelZone_FindRegion(*args)
    def UnlinkRegion(*args): return _blcelc.iCelZone_UnlinkRegion(*args)
    def UnlinkAllRegions(*args): return _blcelc.iCelZone_UnlinkAllRegions(*args)
    def __del__(self, destroy=_blcelc.delete_iCelZone):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCelZonePtr(iCelZone):
    def __init__(self, this):
        _swig_setattr(self, iCelZone, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelZone, 'thisown', 0)
        _swig_setattr(self, iCelZone,self.__class__,iCelZone)
_blcelc.iCelZone_swigregister(iCelZonePtr)

CEL_ZONEERROR_OK = _blcelc.CEL_ZONEERROR_OK
CEL_ZONEERROR_BADREGION = _blcelc.CEL_ZONEERROR_BADREGION
CEL_ZONEERROR_BADSTART = _blcelc.CEL_ZONEERROR_BADSTART
CEL_ZONEERROR_LOAD = _blcelc.CEL_ZONEERROR_LOAD
class iPcZoneManager(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcZoneManager, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcZoneManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcZoneManager instance at %s>" % (self.this,)
    def Load(*args): return _blcelc.iPcZoneManager_Load(*args)
    def CreateZone(*args): return _blcelc.iPcZoneManager_CreateZone(*args)
    def GetZoneCount(*args): return _blcelc.iPcZoneManager_GetZoneCount(*args)
    def GetZone(*args): return _blcelc.iPcZoneManager_GetZone(*args)
    def FindZone(*args): return _blcelc.iPcZoneManager_FindZone(*args)
    def RemoveZone(*args): return _blcelc.iPcZoneManager_RemoveZone(*args)
    def RemoveAllZones(*args): return _blcelc.iPcZoneManager_RemoveAllZones(*args)
    def CreateRegion(*args): return _blcelc.iPcZoneManager_CreateRegion(*args)
    def GetRegionCount(*args): return _blcelc.iPcZoneManager_GetRegionCount(*args)
    def GetRegion(*args): return _blcelc.iPcZoneManager_GetRegion(*args)
    def FindRegion(*args): return _blcelc.iPcZoneManager_FindRegion(*args)
    def RemoveRegion(*args): return _blcelc.iPcZoneManager_RemoveRegion(*args)
    def RemoveAllRegions(*args): return _blcelc.iPcZoneManager_RemoveAllRegions(*args)
    def FindStartLocations(*args): return _blcelc.iPcZoneManager_FindStartLocations(*args)
    def GetLastStartLocation(*args): return _blcelc.iPcZoneManager_GetLastStartLocation(*args)
    def PointCamera(*args): return _blcelc.iPcZoneManager_PointCamera(*args)
    def PointMesh(*args): return _blcelc.iPcZoneManager_PointMesh(*args)
    def __del__(self, destroy=_blcelc.delete_iPcZoneManager):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcZoneManagerPtr(iPcZoneManager):
    def __init__(self, this):
        _swig_setattr(self, iPcZoneManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcZoneManager, 'thisown', 0)
        _swig_setattr(self, iPcZoneManager,self.__class__,iPcZoneManager)
_blcelc.iPcZoneManager_swigregister(iPcZoneManagerPtr)


celCreateZoneManager = _blcelc.celCreateZoneManager

celGetZoneManager = _blcelc.celGetZoneManager

scfQuery_iPcZoneManager = _blcelc.scfQuery_iPcZoneManager
class iPcCommandInput(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCommandInput, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCommandInput, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCommandInput instance at %s>" % (self.this,)
    def Activate(*args): return _blcelc.iPcCommandInput_Activate(*args)
    def ScreenCoordinates(*args): return _blcelc.iPcCommandInput_ScreenCoordinates(*args)
    def ScreenToCentered(*args): return _blcelc.iPcCommandInput_ScreenToCentered(*args)
    def CenteredToScreen(*args): return _blcelc.iPcCommandInput_CenteredToScreen(*args)
    def LoadConfig(*args): return _blcelc.iPcCommandInput_LoadConfig(*args)
    def Bind(*args): return _blcelc.iPcCommandInput_Bind(*args)
    def GetBind(*args): return _blcelc.iPcCommandInput_GetBind(*args)
    def RemoveBind(*args): return _blcelc.iPcCommandInput_RemoveBind(*args)
    def RemoveAllBinds(*args): return _blcelc.iPcCommandInput_RemoveAllBinds(*args)
    def __del__(self, destroy=_blcelc.delete_iPcCommandInput):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcCommandInputPtr(iPcCommandInput):
    def __init__(self, this):
        _swig_setattr(self, iPcCommandInput, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcCommandInput, 'thisown', 0)
        _swig_setattr(self, iPcCommandInput,self.__class__,iPcCommandInput)
_blcelc.iPcCommandInput_swigregister(iPcCommandInputPtr)


celCreateCommandInput = _blcelc.celCreateCommandInput

celGetCommandInput = _blcelc.celGetCommandInput

scfQuery_iPcCommandInput = _blcelc.scfQuery_iPcCommandInput
class iPcGravityCallback(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcGravityCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcGravityCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcGravityCallback instance at %s>" % (self.this,)
    def Callback(*args): return _blcelc.iPcGravityCallback_Callback(*args)
    def __del__(self, destroy=_blcelc.delete_iPcGravityCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcGravityCallbackPtr(iPcGravityCallback):
    def __init__(self, this):
        _swig_setattr(self, iPcGravityCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcGravityCallback, 'thisown', 0)
        _swig_setattr(self, iPcGravityCallback,self.__class__,iPcGravityCallback)
_blcelc.iPcGravityCallback_swigregister(iPcGravityCallbackPtr)

class iPcLinearMovement(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcLinearMovement, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcLinearMovement, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcLinearMovement instance at %s>" % (self.this,)
    def SetAnchor(*args): return _blcelc.iPcLinearMovement_SetAnchor(*args)
    def GetAnchor(*args): return _blcelc.iPcLinearMovement_GetAnchor(*args)
    def SetAngularVelocity(*args): return _blcelc.iPcLinearMovement_SetAngularVelocity(*args)
    def SetSpeed(*args): return _blcelc.iPcLinearMovement_SetSpeed(*args)
    def SetVelocity(*args): return _blcelc.iPcLinearMovement_SetVelocity(*args)
    def AddVelocity(*args): return _blcelc.iPcLinearMovement_AddVelocity(*args)
    def ClearWorldVelocity(*args): return _blcelc.iPcLinearMovement_ClearWorldVelocity(*args)
    def GetVelocity(*args): return _blcelc.iPcLinearMovement_GetVelocity(*args)
    def GetAngularVelocity(*args): return _blcelc.iPcLinearMovement_GetAngularVelocity(*args)
    def GetCDDimensions(*args): return _blcelc.iPcLinearMovement_GetCDDimensions(*args)
    def InitCD(*args): return _blcelc.iPcLinearMovement_InitCD(*args)
    def TimeDiff(*args): return _blcelc.iPcLinearMovement_TimeDiff(*args)
    def GetDRData(*args): return _blcelc.iPcLinearMovement_GetDRData(*args)
    def SetDRData(*args): return _blcelc.iPcLinearMovement_SetDRData(*args)
    def SetSoftDRData(*args): return _blcelc.iPcLinearMovement_SetSoftDRData(*args)
    def SetFullPosition(*args): return _blcelc.iPcLinearMovement_SetFullPosition(*args)
    def SetPosition(*args): return _blcelc.iPcLinearMovement_SetPosition(*args)
    def GetLastPosition(*args): return _blcelc.iPcLinearMovement_GetLastPosition(*args)
    def GetLastFullPosition(*args): return _blcelc.iPcLinearMovement_GetLastFullPosition(*args)
    def IsPath(*args): return _blcelc.iPcLinearMovement_IsPath(*args)
    def GetSector(*args): return _blcelc.iPcLinearMovement_GetSector(*args)
    def ExtrapolatePosition(*args): return _blcelc.iPcLinearMovement_ExtrapolatePosition(*args)
    def UpdateDRDelta(*args): return _blcelc.iPcLinearMovement_UpdateDRDelta(*args)
    def UpdateDR(*args): return _blcelc.iPcLinearMovement_UpdateDR(*args)
    def SetPath(*args): return _blcelc.iPcLinearMovement_SetPath(*args)
    def SetPathTime(*args): return _blcelc.iPcLinearMovement_SetPathTime(*args)
    def SetPathSpeed(*args): return _blcelc.iPcLinearMovement_SetPathSpeed(*args)
    def SetPathAction(*args): return _blcelc.iPcLinearMovement_SetPathAction(*args)
    def SetPathSector(*args): return _blcelc.iPcLinearMovement_SetPathSector(*args)
    def IsOnGround(*args): return _blcelc.iPcLinearMovement_IsOnGround(*args)
    def SetOnGround(*args): return _blcelc.iPcLinearMovement_SetOnGround(*args)
    def SetDeltaLimit(*args): return _blcelc.iPcLinearMovement_SetDeltaLimit(*args)
    def RotateV(*args): return _blcelc.iPcLinearMovement_RotateV(*args)
    def SetGravity(*args): return _blcelc.iPcLinearMovement_SetGravity(*args)
    def GetGravity(*args): return _blcelc.iPcLinearMovement_GetGravity(*args)
    def ResetGravity(*args): return _blcelc.iPcLinearMovement_ResetGravity(*args)
    def AddGravityCallback(*args): return _blcelc.iPcLinearMovement_AddGravityCallback(*args)
    def RemoveGravityCallback(*args): return _blcelc.iPcLinearMovement_RemoveGravityCallback(*args)
    def GetPortalDisplacement(*args): return _blcelc.iPcLinearMovement_GetPortalDisplacement(*args)
    def ClearPortalDisplacement(*args): return _blcelc.iPcLinearMovement_ClearPortalDisplacement(*args)
    def __del__(self, destroy=_blcelc.delete_iPcLinearMovement):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcLinearMovementPtr(iPcLinearMovement):
    def __init__(self, this):
        _swig_setattr(self, iPcLinearMovement, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcLinearMovement, 'thisown', 0)
        _swig_setattr(self, iPcLinearMovement,self.__class__,iPcLinearMovement)
_blcelc.iPcLinearMovement_swigregister(iPcLinearMovementPtr)


celCreateLinearMovement = _blcelc.celCreateLinearMovement

celGetLinearMovement = _blcelc.celGetLinearMovement

scfQuery_iPcLinearMovement = _blcelc.scfQuery_iPcLinearMovement
class iPcActorMove(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcActorMove, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcActorMove, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcActorMove instance at %s>" % (self.this,)
    def Forward(*args): return _blcelc.iPcActorMove_Forward(*args)
    def IsMovingForward(*args): return _blcelc.iPcActorMove_IsMovingForward(*args)
    def Backward(*args): return _blcelc.iPcActorMove_Backward(*args)
    def IsMovingBackward(*args): return _blcelc.iPcActorMove_IsMovingBackward(*args)
    def StrafeLeft(*args): return _blcelc.iPcActorMove_StrafeLeft(*args)
    def IsStrafingLeft(*args): return _blcelc.iPcActorMove_IsStrafingLeft(*args)
    def StrafeRight(*args): return _blcelc.iPcActorMove_StrafeRight(*args)
    def IsStrafingRight(*args): return _blcelc.iPcActorMove_IsStrafingRight(*args)
    def RotateLeft(*args): return _blcelc.iPcActorMove_RotateLeft(*args)
    def IsRotatingLeft(*args): return _blcelc.iPcActorMove_IsRotatingLeft(*args)
    def RotateRight(*args): return _blcelc.iPcActorMove_RotateRight(*args)
    def IsRotatingRight(*args): return _blcelc.iPcActorMove_IsRotatingRight(*args)
    def RotateTo(*args): return _blcelc.iPcActorMove_RotateTo(*args)
    def Run(*args): return _blcelc.iPcActorMove_Run(*args)
    def IsRunning(*args): return _blcelc.iPcActorMove_IsRunning(*args)
    def AutoRun(*args): return _blcelc.iPcActorMove_AutoRun(*args)
    def IsAutoRunning(*args): return _blcelc.iPcActorMove_IsAutoRunning(*args)
    def Jump(*args): return _blcelc.iPcActorMove_Jump(*args)
    def ToggleCameraMode(*args): return _blcelc.iPcActorMove_ToggleCameraMode(*args)
    def SetMovementSpeed(*args): return _blcelc.iPcActorMove_SetMovementSpeed(*args)
    def GetMovementSpeed(*args): return _blcelc.iPcActorMove_GetMovementSpeed(*args)
    def SetRunningSpeed(*args): return _blcelc.iPcActorMove_SetRunningSpeed(*args)
    def GetRunningSpeed(*args): return _blcelc.iPcActorMove_GetRunningSpeed(*args)
    def SetRotationSpeed(*args): return _blcelc.iPcActorMove_SetRotationSpeed(*args)
    def GetRotationSpeed(*args): return _blcelc.iPcActorMove_GetRotationSpeed(*args)
    def SetJumpingVelocity(*args): return _blcelc.iPcActorMove_SetJumpingVelocity(*args)
    def GetJumpingVelocity(*args): return _blcelc.iPcActorMove_GetJumpingVelocity(*args)
    def __del__(self, destroy=_blcelc.delete_iPcActorMove):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcActorMovePtr(iPcActorMove):
    def __init__(self, this):
        _swig_setattr(self, iPcActorMove, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcActorMove, 'thisown', 0)
        _swig_setattr(self, iPcActorMove,self.__class__,iPcActorMove)
_blcelc.iPcActorMove_swigregister(iPcActorMovePtr)


celCreateActorMove = _blcelc.celCreateActorMove

celGetActorMove = _blcelc.celGetActorMove

scfQuery_iPcActorMove = _blcelc.scfQuery_iPcActorMove
class iPcCamera(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCamera, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCamera instance at %s>" % (self.this,)
    def SetRegion(*args): return _blcelc.iPcCamera_SetRegion(*args)
    def SetZoneManager(*args): return _blcelc.iPcCamera_SetZoneManager(*args)
    def SetRectangle(*args): return _blcelc.iPcCamera_SetRectangle(*args)
    def GetCamera(*args): return _blcelc.iPcCamera_GetCamera(*args)
    def GetView(*args): return _blcelc.iPcCamera_GetView(*args)
    def SetClearZBuffer(*args): return _blcelc.iPcCamera_SetClearZBuffer(*args)
    def GetClearZBuffer(*args): return _blcelc.iPcCamera_GetClearZBuffer(*args)
    def SetClearScreen(*args): return _blcelc.iPcCamera_SetClearScreen(*args)
    def GetClearScreen(*args): return _blcelc.iPcCamera_GetClearScreen(*args)
    def DisableDistanceClipping(*args): return _blcelc.iPcCamera_DisableDistanceClipping(*args)
    def EnableFixedDistanceClipping(*args): return _blcelc.iPcCamera_EnableFixedDistanceClipping(*args)
    def EnableAdaptiveDistanceClipping(*args): return _blcelc.iPcCamera_EnableAdaptiveDistanceClipping(*args)
    def UseDistanceClipping(*args): return _blcelc.iPcCamera_UseDistanceClipping(*args)
    def UseFixedDistanceClipping(*args): return _blcelc.iPcCamera_UseFixedDistanceClipping(*args)
    def GetFixedDistance(*args): return _blcelc.iPcCamera_GetFixedDistance(*args)
    def GetAdaptiveMinFPS(*args): return _blcelc.iPcCamera_GetAdaptiveMinFPS(*args)
    def GetAdaptiveMaxFPS(*args): return _blcelc.iPcCamera_GetAdaptiveMaxFPS(*args)
    def GetAdaptiveMinDistance(*args): return _blcelc.iPcCamera_GetAdaptiveMinDistance(*args)
    def SetAutoDraw(*args): return _blcelc.iPcCamera_SetAutoDraw(*args)
    def Draw(*args): return _blcelc.iPcCamera_Draw(*args)
    def __del__(self, destroy=_blcelc.delete_iPcCamera):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcCameraPtr(iPcCamera):
    def __init__(self, this):
        _swig_setattr(self, iPcCamera, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcCamera, 'thisown', 0)
        _swig_setattr(self, iPcCamera,self.__class__,iPcCamera)
_blcelc.iPcCamera_swigregister(iPcCameraPtr)


celCreateCamera = _blcelc.celCreateCamera

celGetCamera = _blcelc.celGetCamera

scfQuery_iPcCamera = _blcelc.scfQuery_iPcCamera
class iPcDefaultCamera(iPcCamera):
    __swig_setmethods__ = {}
    for _s in [iPcCamera]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcDefaultCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [iPcCamera]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcDefaultCamera, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcDefaultCamera instance at %s>" % (self.this,)
    freelook = _blcelc.iPcDefaultCamera_freelook
    firstperson = _blcelc.iPcDefaultCamera_firstperson
    thirdperson = _blcelc.iPcDefaultCamera_thirdperson
    m64_thirdperson = _blcelc.iPcDefaultCamera_m64_thirdperson
    lara_thirdperson = _blcelc.iPcDefaultCamera_lara_thirdperson
    actual_data = _blcelc.iPcDefaultCamera_actual_data
    last_actual = _blcelc.iPcDefaultCamera_last_actual
    transition = _blcelc.iPcDefaultCamera_transition
    camerror = _blcelc.iPcDefaultCamera_camerror
    CameraMode_count = _blcelc.iPcDefaultCamera_CameraMode_count
    def SetMode(*args): return _blcelc.iPcDefaultCamera_SetMode(*args)
    def GetMode(*args): return _blcelc.iPcDefaultCamera_GetMode(*args)
    def SetModeName(*args): return _blcelc.iPcDefaultCamera_SetModeName(*args)
    def GetModeName(*args): return _blcelc.iPcDefaultCamera_GetModeName(*args)
    def GetNextMode(*args): return _blcelc.iPcDefaultCamera_GetNextMode(*args)
    def SetSpringParameters(*args): return _blcelc.iPcDefaultCamera_SetSpringParameters(*args)
    def SetMinMaxCameraDistance(*args): return _blcelc.iPcDefaultCamera_SetMinMaxCameraDistance(*args)
    def SetTurnSpeed(*args): return _blcelc.iPcDefaultCamera_SetTurnSpeed(*args)
    def SetSwingCoef(*args): return _blcelc.iPcDefaultCamera_SetSwingCoef(*args)
    def SetFirstPersonOffset(*args): return _blcelc.iPcDefaultCamera_SetFirstPersonOffset(*args)
    def SetThirdPersonOffset(*args): return _blcelc.iPcDefaultCamera_SetThirdPersonOffset(*args)
    def CenterCamera(*args): return _blcelc.iPcDefaultCamera_CenterCamera(*args)
    def SetPitch(*args): return _blcelc.iPcDefaultCamera_SetPitch(*args)
    def GetPitch(*args): return _blcelc.iPcDefaultCamera_GetPitch(*args)
    def SetPitchVelocity(*args): return _blcelc.iPcDefaultCamera_SetPitchVelocity(*args)
    def GetPitchVelocity(*args): return _blcelc.iPcDefaultCamera_GetPitchVelocity(*args)
    def MovePitch(*args): return _blcelc.iPcDefaultCamera_MovePitch(*args)
    def SetYaw(*args): return _blcelc.iPcDefaultCamera_SetYaw(*args)
    def MoveYaw(*args): return _blcelc.iPcDefaultCamera_MoveYaw(*args)
    def GetYaw(*args): return _blcelc.iPcDefaultCamera_GetYaw(*args)
    def SetYawVelocity(*args): return _blcelc.iPcDefaultCamera_SetYawVelocity(*args)
    def GetYawVelocity(*args): return _blcelc.iPcDefaultCamera_GetYawVelocity(*args)
    def SetDistance(*args): return _blcelc.iPcDefaultCamera_SetDistance(*args)
    def __del__(self, destroy=_blcelc.delete_iPcDefaultCamera):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcDefaultCameraPtr(iPcDefaultCamera):
    def __init__(self, this):
        _swig_setattr(self, iPcDefaultCamera, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcDefaultCamera, 'thisown', 0)
        _swig_setattr(self, iPcDefaultCamera,self.__class__,iPcDefaultCamera)
_blcelc.iPcDefaultCamera_swigregister(iPcDefaultCameraPtr)


celCreateDefaultCamera = _blcelc.celCreateDefaultCamera

celGetDefaultCamera = _blcelc.celGetDefaultCamera

scfQuery_iPcDefaultCamera = _blcelc.scfQuery_iPcDefaultCamera
class iPcSimpleCamera(iPcCamera):
    __swig_setmethods__ = {}
    for _s in [iPcCamera]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcSimpleCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [iPcCamera]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcSimpleCamera, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcSimpleCamera instance at %s>" % (self.this,)
    def SetDrawMesh(*args): return _blcelc.iPcSimpleCamera_SetDrawMesh(*args)
    def SetCameraOffset(*args): return _blcelc.iPcSimpleCamera_SetCameraOffset(*args)
    def SetLookAtOffset(*args): return _blcelc.iPcSimpleCamera_SetLookAtOffset(*args)
    def Draw(*args): return _blcelc.iPcSimpleCamera_Draw(*args)
    def __del__(self, destroy=_blcelc.delete_iPcSimpleCamera):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcSimpleCameraPtr(iPcSimpleCamera):
    def __init__(self, this):
        _swig_setattr(self, iPcSimpleCamera, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcSimpleCamera, 'thisown', 0)
        _swig_setattr(self, iPcSimpleCamera,self.__class__,iPcSimpleCamera)
_blcelc.iPcSimpleCamera_swigregister(iPcSimpleCameraPtr)


celCreateSimpleCamera = _blcelc.celCreateSimpleCamera

celGetSimpleCamera = _blcelc.celGetSimpleCamera

scfQuery_iPcSimpleCamera = _blcelc.scfQuery_iPcSimpleCamera
CEL_MOUSE_BUTTON1 = _blcelc.CEL_MOUSE_BUTTON1
CEL_MOUSE_BUTTON2 = _blcelc.CEL_MOUSE_BUTTON2
CEL_MOUSE_BUTTON3 = _blcelc.CEL_MOUSE_BUTTON3
class iPcMeshSelectListener(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMeshSelectListener, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMeshSelectListener, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMeshSelectListener instance at %s>" % (self.this,)
    def MouseDown(*args): return _blcelc.iPcMeshSelectListener_MouseDown(*args)
    def MouseUp(*args): return _blcelc.iPcMeshSelectListener_MouseUp(*args)
    def MouseMove(*args): return _blcelc.iPcMeshSelectListener_MouseMove(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMeshSelectListener):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMeshSelectListenerPtr(iPcMeshSelectListener):
    def __init__(self, this):
        _swig_setattr(self, iPcMeshSelectListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMeshSelectListener, 'thisown', 0)
        _swig_setattr(self, iPcMeshSelectListener,self.__class__,iPcMeshSelectListener)
_blcelc.iPcMeshSelectListener_swigregister(iPcMeshSelectListenerPtr)

class iPcMeshSelect(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMeshSelect, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMeshSelect, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMeshSelect instance at %s>" % (self.this,)
    def AddMeshSelectListener(*args): return _blcelc.iPcMeshSelect_AddMeshSelectListener(*args)
    def RemoveMeshSelectListener(*args): return _blcelc.iPcMeshSelect_RemoveMeshSelectListener(*args)
    def SetCamera(*args): return _blcelc.iPcMeshSelect_SetCamera(*args)
    def SetMouseButtons(*args): return _blcelc.iPcMeshSelect_SetMouseButtons(*args)
    def GetMouseButtons(*args): return _blcelc.iPcMeshSelect_GetMouseButtons(*args)
    def SetGlobalSelection(*args): return _blcelc.iPcMeshSelect_SetGlobalSelection(*args)
    def HasGlobalSelection(*args): return _blcelc.iPcMeshSelect_HasGlobalSelection(*args)
    def SetFollowMode(*args): return _blcelc.iPcMeshSelect_SetFollowMode(*args)
    def HasFollowMode(*args): return _blcelc.iPcMeshSelect_HasFollowMode(*args)
    def SetFollowAlwaysMode(*args): return _blcelc.iPcMeshSelect_SetFollowAlwaysMode(*args)
    def HasFollowAlwaysMode(*args): return _blcelc.iPcMeshSelect_HasFollowAlwaysMode(*args)
    def SetDragMode(*args): return _blcelc.iPcMeshSelect_SetDragMode(*args)
    def HasDragMode(*args): return _blcelc.iPcMeshSelect_HasDragMode(*args)
    def SetDragPlaneNormal(*args): return _blcelc.iPcMeshSelect_SetDragPlaneNormal(*args)
    def GetDragPlaneNormal(*args): return _blcelc.iPcMeshSelect_GetDragPlaneNormal(*args)
    def SetSendmoveEvent(*args): return _blcelc.iPcMeshSelect_SetSendmoveEvent(*args)
    def HasSendmoveEvent(*args): return _blcelc.iPcMeshSelect_HasSendmoveEvent(*args)
    def SetSendupEvent(*args): return _blcelc.iPcMeshSelect_SetSendupEvent(*args)
    def HasSendupEvent(*args): return _blcelc.iPcMeshSelect_HasSendupEvent(*args)
    def SetSenddownEvent(*args): return _blcelc.iPcMeshSelect_SetSenddownEvent(*args)
    def HasSenddownEvent(*args): return _blcelc.iPcMeshSelect_HasSenddownEvent(*args)
    def SetMaxSelectionDistance(*args): return _blcelc.iPcMeshSelect_SetMaxSelectionDistance(*args)
    def GetMaxSelectionDistance(*args): return _blcelc.iPcMeshSelect_GetMaxSelectionDistance(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMeshSelect):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMeshSelectPtr(iPcMeshSelect):
    def __init__(self, this):
        _swig_setattr(self, iPcMeshSelect, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMeshSelect, 'thisown', 0)
        _swig_setattr(self, iPcMeshSelect,self.__class__,iPcMeshSelect)
_blcelc.iPcMeshSelect_swigregister(iPcMeshSelectPtr)


celCreateMeshSelect = _blcelc.celCreateMeshSelect

celGetMeshSelect = _blcelc.celGetMeshSelect

scfQuery_iPcMeshSelect = _blcelc.scfQuery_iPcMeshSelect
CEL_PCMESH_PROPERTY_MESH = _blcelc.CEL_PCMESH_PROPERTY_MESH
class iPcMesh(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMesh, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMesh, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMesh instance at %s>" % (self.this,)
    def SetPath(*args): return _blcelc.iPcMesh_SetPath(*args)
    def LoadMesh(*args): return _blcelc.iPcMesh_LoadMesh(*args)
    def CreateEmptyThing(*args): return _blcelc.iPcMesh_CreateEmptyThing(*args)
    def GetMesh(*args): return _blcelc.iPcMesh_GetMesh(*args)
    def MoveMesh(*args): return _blcelc.iPcMesh_MoveMesh(*args)
    def SetAction(*args): return _blcelc.iPcMesh_SetAction(*args)
    def GetAction(*args): return _blcelc.iPcMesh_GetAction(*args)
    def SetReverseAction(*args): return _blcelc.iPcMesh_SetReverseAction(*args)
    def Hide(*args): return _blcelc.iPcMesh_Hide(*args)
    def Show(*args): return _blcelc.iPcMesh_Show(*args)
    def IsVisible(*args): return _blcelc.iPcMesh_IsVisible(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMesh):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMeshPtr(iPcMesh):
    def __init__(self, this):
        _swig_setattr(self, iPcMesh, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMesh, 'thisown', 0)
        _swig_setattr(self, iPcMesh,self.__class__,iPcMesh)
_blcelc.iPcMesh_swigregister(iPcMeshPtr)


celCreateMesh = _blcelc.celCreateMesh

celGetMesh = _blcelc.celGetMesh

scfQuery_iPcMesh = _blcelc.scfQuery_iPcMesh
class iPcTimer(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcTimer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcTimer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcTimer instance at %s>" % (self.this,)
    def WakeUp(*args): return _blcelc.iPcTimer_WakeUp(*args)
    def WakeUpFrame(*args): return _blcelc.iPcTimer_WakeUpFrame(*args)
    def Clear(*args): return _blcelc.iPcTimer_Clear(*args)
    def __del__(self, destroy=_blcelc.delete_iPcTimer):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcTimerPtr(iPcTimer):
    def __init__(self, this):
        _swig_setattr(self, iPcTimer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcTimer, 'thisown', 0)
        _swig_setattr(self, iPcTimer,self.__class__,iPcTimer)
_blcelc.iPcTimer_swigregister(iPcTimerPtr)


celCreateTimer = _blcelc.celCreateTimer

celGetTimer = _blcelc.celGetTimer

scfQuery_iPcTimer = _blcelc.scfQuery_iPcTimer
class iPcSolid(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcSolid, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcSolid, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcSolid instance at %s>" % (self.this,)
    def SetMesh(*args): return _blcelc.iPcSolid_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcSolid_GetMesh(*args)
    def GetCollider(*args): return _blcelc.iPcSolid_GetCollider(*args)
    def __del__(self, destroy=_blcelc.delete_iPcSolid):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcSolidPtr(iPcSolid):
    def __init__(self, this):
        _swig_setattr(self, iPcSolid, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcSolid, 'thisown', 0)
        _swig_setattr(self, iPcSolid,self.__class__,iPcSolid)
_blcelc.iPcSolid_swigregister(iPcSolidPtr)


celCreateSolid = _blcelc.celCreateSolid

celGetSolid = _blcelc.celGetSolid

scfQuery_iPcSolid = _blcelc.scfQuery_iPcSolid
class iPcGravity(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcGravity, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcGravity, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcGravity instance at %s>" % (self.this,)
    def CreateGravityColliderFromMesh(*args): return _blcelc.iPcGravity_CreateGravityColliderFromMesh(*args)
    def GetGravityCollider(*args): return _blcelc.iPcGravity_GetGravityCollider(*args)
    def SetMovable(*args): return _blcelc.iPcGravity_SetMovable(*args)
    def GetMovable(*args): return _blcelc.iPcGravity_GetMovable(*args)
    def SetSolid(*args): return _blcelc.iPcGravity_SetSolid(*args)
    def GetSolid(*args): return _blcelc.iPcGravity_GetSolid(*args)
    def SetWeight(*args): return _blcelc.iPcGravity_SetWeight(*args)
    def GetWeight(*args): return _blcelc.iPcGravity_GetWeight(*args)
    def ClearForces(*args): return _blcelc.iPcGravity_ClearForces(*args)
    def ClearPermanentForces(*args): return _blcelc.iPcGravity_ClearPermanentForces(*args)
    def ResetSpeed(*args): return _blcelc.iPcGravity_ResetSpeed(*args)
    def ApplyForce(*args): return _blcelc.iPcGravity_ApplyForce(*args)
    def ApplyPermanentForce(*args): return _blcelc.iPcGravity_ApplyPermanentForce(*args)
    def IsResting(*args): return _blcelc.iPcGravity_IsResting(*args)
    def SetActive(*args): return _blcelc.iPcGravity_SetActive(*args)
    def IsActive(*args): return _blcelc.iPcGravity_IsActive(*args)
    def __del__(self, destroy=_blcelc.delete_iPcGravity):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcGravityPtr(iPcGravity):
    def __init__(self, this):
        _swig_setattr(self, iPcGravity, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcGravity, 'thisown', 0)
        _swig_setattr(self, iPcGravity,self.__class__,iPcGravity)
_blcelc.iPcGravity_swigregister(iPcGravityPtr)


celCreateGravity = _blcelc.celCreateGravity

celGetGravity = _blcelc.celGetGravity

scfQuery_iPcGravity = _blcelc.scfQuery_iPcGravity
CEL_PCMOVABLE_PROPERTY_POSITION = _blcelc.CEL_PCMOVABLE_PROPERTY_POSITION
CEL_MOVE_FAIL = _blcelc.CEL_MOVE_FAIL
CEL_MOVE_SUCCEED = _blcelc.CEL_MOVE_SUCCEED
CEL_MOVE_PARTIAL = _blcelc.CEL_MOVE_PARTIAL
class iPcMovable(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMovable, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMovable, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMovable instance at %s>" % (self.this,)
    def SetMesh(*args): return _blcelc.iPcMovable_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcMovable_GetMesh(*args)
    def SetPos(*args): return _blcelc.iPcMovable_SetPos(*args)
    def AddConstraint(*args): return _blcelc.iPcMovable_AddConstraint(*args)
    def RemoveConstraint(*args): return _blcelc.iPcMovable_RemoveConstraint(*args)
    def RemoveAllConstraints(*args): return _blcelc.iPcMovable_RemoveAllConstraints(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMovable):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMovablePtr(iPcMovable):
    def __init__(self, this):
        _swig_setattr(self, iPcMovable, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMovable, 'thisown', 0)
        _swig_setattr(self, iPcMovable,self.__class__,iPcMovable)
_blcelc.iPcMovable_swigregister(iPcMovablePtr)

class iPcMovableConstraint(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMovableConstraint, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMovableConstraint, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMovableConstraint instance at %s>" % (self.this,)
    def CheckMove(*args): return _blcelc.iPcMovableConstraint_CheckMove(*args)
    def __del__(self, destroy=_blcelc.delete_iPcMovableConstraint):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcMovableConstraintPtr(iPcMovableConstraint):
    def __init__(self, this):
        _swig_setattr(self, iPcMovableConstraint, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMovableConstraint, 'thisown', 0)
        _swig_setattr(self, iPcMovableConstraint,self.__class__,iPcMovableConstraint)
_blcelc.iPcMovableConstraint_swigregister(iPcMovableConstraintPtr)


celCreateMovable = _blcelc.celCreateMovable

celGetMovable = _blcelc.celGetMovable

scfQuery_iPcMovable = _blcelc.scfQuery_iPcMovable
class iPcInventoryListener(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcInventoryListener, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcInventoryListener, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcInventoryListener instance at %s>" % (self.this,)
    def AddChild(*args): return _blcelc.iPcInventoryListener_AddChild(*args)
    def RemoveChild(*args): return _blcelc.iPcInventoryListener_RemoveChild(*args)
    def __del__(self, destroy=_blcelc.delete_iPcInventoryListener):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcInventoryListenerPtr(iPcInventoryListener):
    def __init__(self, this):
        _swig_setattr(self, iPcInventoryListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcInventoryListener, 'thisown', 0)
        _swig_setattr(self, iPcInventoryListener,self.__class__,iPcInventoryListener)
_blcelc.iPcInventoryListener_swigregister(iPcInventoryListenerPtr)

class iPcInventory(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcInventory, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcInventory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcInventory instance at %s>" % (self.this,)
    def AddInventoryListener(*args): return _blcelc.iPcInventory_AddInventoryListener(*args)
    def RemoveInventoryListener(*args): return _blcelc.iPcInventory_RemoveInventoryListener(*args)
    def AddEntity(*args): return _blcelc.iPcInventory_AddEntity(*args)
    def RemoveEntity(*args): return _blcelc.iPcInventory_RemoveEntity(*args)
    def RemoveAll(*args): return _blcelc.iPcInventory_RemoveAll(*args)
    def GetEntityCount(*args): return _blcelc.iPcInventory_GetEntityCount(*args)
    def GetEntity(*args): return _blcelc.iPcInventory_GetEntity(*args)
    def SetStrictCharacteristics(*args): return _blcelc.iPcInventory_SetStrictCharacteristics(*args)
    def HasStrictCharacteristics(*args): return _blcelc.iPcInventory_HasStrictCharacteristics(*args)
    def SetConstraints(*args): return _blcelc.iPcInventory_SetConstraints(*args)
    def GetConstraints(*args): return _blcelc.iPcInventory_GetConstraints(*args)
    def RemoveConstraints(*args): return _blcelc.iPcInventory_RemoveConstraints(*args)
    def RemoveAllConstraints(*args): return _blcelc.iPcInventory_RemoveAllConstraints(*args)
    def GetCurrentCharacteristic(*args): return _blcelc.iPcInventory_GetCurrentCharacteristic(*args)
    def MarkDirty(*args): return _blcelc.iPcInventory_MarkDirty(*args)
    def TestConstraints(*args): return _blcelc.iPcInventory_TestConstraints(*args)
    def Dump(*args): return _blcelc.iPcInventory_Dump(*args)
    def __del__(self, destroy=_blcelc.delete_iPcInventory):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcInventoryPtr(iPcInventory):
    def __init__(self, this):
        _swig_setattr(self, iPcInventory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcInventory, 'thisown', 0)
        _swig_setattr(self, iPcInventory,self.__class__,iPcInventory)
_blcelc.iPcInventory_swigregister(iPcInventoryPtr)


celCreateInventory = _blcelc.celCreateInventory

celGetInventory = _blcelc.celGetInventory

scfQuery_iPcInventory = _blcelc.scfQuery_iPcInventory
class iPcCharacteristics(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCharacteristics, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCharacteristics, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCharacteristics instance at %s>" % (self.this,)
    def SetCharacteristic(*args): return _blcelc.iPcCharacteristics_SetCharacteristic(*args)
    def SetInheritedCharacteristic(*args): return _blcelc.iPcCharacteristics_SetInheritedCharacteristic(*args)
    def GetCharacteristic(*args): return _blcelc.iPcCharacteristics_GetCharacteristic(*args)
    def GetLocalCharacteristic(*args): return _blcelc.iPcCharacteristics_GetLocalCharacteristic(*args)
    def GetInheritedCharacteristic(*args): return _blcelc.iPcCharacteristics_GetInheritedCharacteristic(*args)
    def ClearCharacteristic(*args): return _blcelc.iPcCharacteristics_ClearCharacteristic(*args)
    def HasCharacteristic(*args): return _blcelc.iPcCharacteristics_HasCharacteristic(*args)
    def ClearAll(*args): return _blcelc.iPcCharacteristics_ClearAll(*args)
    def AddToInventory(*args): return _blcelc.iPcCharacteristics_AddToInventory(*args)
    def RemoveFromInventory(*args): return _blcelc.iPcCharacteristics_RemoveFromInventory(*args)
    def MarkDirty(*args): return _blcelc.iPcCharacteristics_MarkDirty(*args)
    def TestConstraints(*args): return _blcelc.iPcCharacteristics_TestConstraints(*args)
    def Dump(*args): return _blcelc.iPcCharacteristics_Dump(*args)
    def __del__(self, destroy=_blcelc.delete_iPcCharacteristics):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcCharacteristicsPtr(iPcCharacteristics):
    def __init__(self, this):
        _swig_setattr(self, iPcCharacteristics, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcCharacteristics, 'thisown', 0)
        _swig_setattr(self, iPcCharacteristics,self.__class__,iPcCharacteristics)
_blcelc.iPcCharacteristics_swigregister(iPcCharacteristicsPtr)


celCreateCharacteristics = _blcelc.celCreateCharacteristics

celGetCharacteristics = _blcelc.celGetCharacteristics

scfQuery_iPcCharacteristics = _blcelc.scfQuery_iPcCharacteristics
CEL_TOOLTIP_LEFT = _blcelc.CEL_TOOLTIP_LEFT
CEL_TOOLTIP_CENTER = _blcelc.CEL_TOOLTIP_CENTER
CEL_TOOLTIP_RIGHT = _blcelc.CEL_TOOLTIP_RIGHT
class iPcTooltip(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcTooltip, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcTooltip, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcTooltip instance at %s>" % (self.this,)
    def SetText(*args): return _blcelc.iPcTooltip_SetText(*args)
    def Show(*args): return _blcelc.iPcTooltip_Show(*args)
    def Hide(*args): return _blcelc.iPcTooltip_Hide(*args)
    def IsVisible(*args): return _blcelc.iPcTooltip_IsVisible(*args)
    def SetTextColor(*args): return _blcelc.iPcTooltip_SetTextColor(*args)
    def SetBackgroundColor(*args): return _blcelc.iPcTooltip_SetBackgroundColor(*args)
    def SetFont(*args): return _blcelc.iPcTooltip_SetFont(*args)
    def SetJustify(*args): return _blcelc.iPcTooltip_SetJustify(*args)
    def __del__(self, destroy=_blcelc.delete_iPcTooltip):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPcTooltipPtr(iPcTooltip):
    def __init__(self, this):
        _swig_setattr(self, iPcTooltip, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcTooltip, 'thisown', 0)
        _swig_setattr(self, iPcTooltip,self.__class__,iPcTooltip)
_blcelc.iPcTooltip_swigregister(iPcTooltipPtr)


celCreateToolTip = _blcelc.celCreateToolTip

celGetToolTip = _blcelc.celGetToolTip

scfQuery_iPcTooltip = _blcelc.scfQuery_iPcTooltip
CEL_DATA_NONE = _blcelc.CEL_DATA_NONE
CEL_DATA_BOOL = _blcelc.CEL_DATA_BOOL
CEL_DATA_BYTE = _blcelc.CEL_DATA_BYTE
CEL_DATA_WORD = _blcelc.CEL_DATA_WORD
CEL_DATA_LONG = _blcelc.CEL_DATA_LONG
CEL_DATA_UBYTE = _blcelc.CEL_DATA_UBYTE
CEL_DATA_UWORD = _blcelc.CEL_DATA_UWORD
CEL_DATA_ULONG = _blcelc.CEL_DATA_ULONG
CEL_DATA_FLOAT = _blcelc.CEL_DATA_FLOAT
CEL_DATA_VECTOR2 = _blcelc.CEL_DATA_VECTOR2
CEL_DATA_VECTOR3 = _blcelc.CEL_DATA_VECTOR3
CEL_DATA_STRING = _blcelc.CEL_DATA_STRING
CEL_DATA_PCLASS = _blcelc.CEL_DATA_PCLASS
CEL_DATA_ENTITY = _blcelc.CEL_DATA_ENTITY
CEL_DATA_ACTION = _blcelc.CEL_DATA_ACTION
CEL_DATA_COLOR = _blcelc.CEL_DATA_COLOR
CEL_DATA_IBASE = _blcelc.CEL_DATA_IBASE

