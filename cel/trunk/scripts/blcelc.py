# This file was created automatically by SWIG 1.3.29.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _blcelc
import new
new_instancemethod = new.instancemethod
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


import cspace
def fix_args(funct):
    def _inner(self, args):
        if type(args) == tuple:
            args = (self,) + args
        else:
            args = (self, args)
        return funct(*args)
    return _inner

_csRef_to_Python = _blcelc._csRef_to_Python
class celInitializer(cspace.csInitializer):
    __swig_setmethods__ = {}
    for _s in [cspace.csInitializer]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, celInitializer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.csInitializer]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, celInitializer, name)
    __repr__ = _swig_repr
    __swig_getmethods__["SetupCelPluginDirs"] = lambda x: _blcelc.celInitializer_SetupCelPluginDirs
    if _newclass:SetupCelPluginDirs = staticmethod(_blcelc.celInitializer_SetupCelPluginDirs)
    __swig_getmethods__["_RequestPlugins"] = lambda x: _blcelc.celInitializer__RequestPlugins
    if _newclass:_RequestPlugins = staticmethod(_blcelc.celInitializer__RequestPlugins)
    __swig_getmethods__["SetupVFS"] = lambda x: _blcelc.celInitializer_SetupVFS
    if _newclass:SetupVFS = staticmethod(_blcelc.celInitializer_SetupVFS)
    __swig_getmethods__["SetupConfigManager"] = lambda x: _blcelc.celInitializer_SetupConfigManager
    if _newclass:SetupConfigManager = staticmethod(_blcelc.celInitializer_SetupConfigManager)
    def __init__(self, *args): 
        this = _blcelc.new_celInitializer(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blcelc.delete_celInitializer
    __del__ = lambda self : None;
celInitializer_swigregister = _blcelc.celInitializer_swigregister
celInitializer_swigregister(celInitializer)
celInitializer_SetupCelPluginDirs = _blcelc.celInitializer_SetupCelPluginDirs
celInitializer__RequestPlugins = _blcelc.celInitializer__RequestPlugins
celInitializer_SetupVFS = _blcelc.celInitializer_SetupVFS
celInitializer_SetupConfigManager = _blcelc.celInitializer_SetupConfigManager

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
CEL_DATA_PARAMETER = _blcelc.CEL_DATA_PARAMETER
CEL_DATA_LAST = _blcelc.CEL_DATA_LAST
class celData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, celData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, celData, name)
    __repr__ = _swig_repr
    __swig_setmethods__["type"] = _blcelc.celData_type_set
    __swig_getmethods__["type"] = _blcelc.celData_type_get
    if _newclass:type = property(_blcelc.celData_type_get, _blcelc.celData_type_set)
    def __init__(self, *args): 
        this = _blcelc.new_celData(*args)
        try: self.this.append(this)
        except: self.this = this
    def assign(*args): return _blcelc.celData_assign(*args)
    __swig_destroy__ = _blcelc.delete_celData
    __del__ = lambda self : None;
    def Clear(*args): return _blcelc.celData_Clear(*args)
    def Set(*args): return _blcelc.celData_Set(*args)
    def SetAction(*args): return _blcelc.celData_SetAction(*args)
    def SetIBase(*args): return _blcelc.celData_SetIBase(*args)
    def SetParameter(*args): return _blcelc.celData_SetParameter(*args)
    __swig_getmethods__["value"] = _blcelc.celData_value_get
    if _newclass:value = property(_blcelc.celData_value_get)
celData_swigregister = _blcelc.celData_swigregister
celData_swigregister(celData)

class celData_value(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, celData_value, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, celData_value, name)
    __repr__ = _swig_repr
    __swig_setmethods__["bo"] = _blcelc.celData_value_bo_set
    __swig_getmethods__["bo"] = _blcelc.celData_value_bo_get
    if _newclass:bo = property(_blcelc.celData_value_bo_get, _blcelc.celData_value_bo_set)
    __swig_setmethods__["b"] = _blcelc.celData_value_b_set
    __swig_getmethods__["b"] = _blcelc.celData_value_b_get
    if _newclass:b = property(_blcelc.celData_value_b_get, _blcelc.celData_value_b_set)
    __swig_setmethods__["ub"] = _blcelc.celData_value_ub_set
    __swig_getmethods__["ub"] = _blcelc.celData_value_ub_get
    if _newclass:ub = property(_blcelc.celData_value_ub_get, _blcelc.celData_value_ub_set)
    __swig_setmethods__["w"] = _blcelc.celData_value_w_set
    __swig_getmethods__["w"] = _blcelc.celData_value_w_get
    if _newclass:w = property(_blcelc.celData_value_w_get, _blcelc.celData_value_w_set)
    __swig_setmethods__["uw"] = _blcelc.celData_value_uw_set
    __swig_getmethods__["uw"] = _blcelc.celData_value_uw_get
    if _newclass:uw = property(_blcelc.celData_value_uw_get, _blcelc.celData_value_uw_set)
    __swig_setmethods__["l"] = _blcelc.celData_value_l_set
    __swig_getmethods__["l"] = _blcelc.celData_value_l_get
    if _newclass:l = property(_blcelc.celData_value_l_get, _blcelc.celData_value_l_set)
    __swig_setmethods__["ul"] = _blcelc.celData_value_ul_set
    __swig_getmethods__["ul"] = _blcelc.celData_value_ul_get
    if _newclass:ul = property(_blcelc.celData_value_ul_get, _blcelc.celData_value_ul_set)
    __swig_setmethods__["f"] = _blcelc.celData_value_f_set
    __swig_getmethods__["f"] = _blcelc.celData_value_f_get
    if _newclass:f = property(_blcelc.celData_value_f_get, _blcelc.celData_value_f_set)
    __swig_setmethods__["s"] = _blcelc.celData_value_s_set
    __swig_getmethods__["s"] = _blcelc.celData_value_s_get
    if _newclass:s = property(_blcelc.celData_value_s_get, _blcelc.celData_value_s_set)
    __swig_setmethods__["pc"] = _blcelc.celData_value_pc_set
    __swig_getmethods__["pc"] = _blcelc.celData_value_pc_get
    if _newclass:pc = property(_blcelc.celData_value_pc_get, _blcelc.celData_value_pc_set)
    __swig_setmethods__["ent"] = _blcelc.celData_value_ent_set
    __swig_getmethods__["ent"] = _blcelc.celData_value_ent_get
    if _newclass:ent = property(_blcelc.celData_value_ent_get, _blcelc.celData_value_ent_set)
    __swig_setmethods__["ibase"] = _blcelc.celData_value_ibase_set
    __swig_getmethods__["ibase"] = _blcelc.celData_value_ibase_get
    if _newclass:ibase = property(_blcelc.celData_value_ibase_get, _blcelc.celData_value_ibase_set)
    __swig_getmethods__["par"] = _blcelc.celData_value_par_get
    if _newclass:par = property(_blcelc.celData_value_par_get)
    __swig_getmethods__["col"] = _blcelc.celData_value_col_get
    if _newclass:col = property(_blcelc.celData_value_col_get)
    __swig_getmethods__["v"] = _blcelc.celData_value_v_get
    if _newclass:v = property(_blcelc.celData_value_v_get)
    def __init__(self, *args): 
        this = _blcelc.new_celData_value(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blcelc.delete_celData_value
    __del__ = lambda self : None;
celData_value_swigregister = _blcelc.celData_value_swigregister
celData_value_swigregister(celData_value)

class celData_value_par(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, celData_value_par, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, celData_value_par, name)
    __repr__ = _swig_repr
    __swig_setmethods__["parname"] = _blcelc.celData_value_par_parname_set
    __swig_getmethods__["parname"] = _blcelc.celData_value_par_parname_get
    if _newclass:parname = property(_blcelc.celData_value_par_parname_get, _blcelc.celData_value_par_parname_set)
    __swig_setmethods__["partype"] = _blcelc.celData_value_par_partype_set
    __swig_getmethods__["partype"] = _blcelc.celData_value_par_partype_get
    if _newclass:partype = property(_blcelc.celData_value_par_partype_get, _blcelc.celData_value_par_partype_set)
    def __init__(self, *args): 
        this = _blcelc.new_celData_value_par(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blcelc.delete_celData_value_par
    __del__ = lambda self : None;
celData_value_par_swigregister = _blcelc.celData_value_par_swigregister
celData_value_par_swigregister(celData_value_par)

class celData_value_col(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, celData_value_col, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, celData_value_col, name)
    __repr__ = _swig_repr
    __swig_setmethods__["red"] = _blcelc.celData_value_col_red_set
    __swig_getmethods__["red"] = _blcelc.celData_value_col_red_get
    if _newclass:red = property(_blcelc.celData_value_col_red_get, _blcelc.celData_value_col_red_set)
    __swig_setmethods__["green"] = _blcelc.celData_value_col_green_set
    __swig_getmethods__["green"] = _blcelc.celData_value_col_green_get
    if _newclass:green = property(_blcelc.celData_value_col_green_get, _blcelc.celData_value_col_green_set)
    __swig_setmethods__["blue"] = _blcelc.celData_value_col_blue_set
    __swig_getmethods__["blue"] = _blcelc.celData_value_col_blue_get
    if _newclass:blue = property(_blcelc.celData_value_col_blue_get, _blcelc.celData_value_col_blue_set)
    def __init__(self, *args): 
        this = _blcelc.new_celData_value_col(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blcelc.delete_celData_value_col
    __del__ = lambda self : None;
celData_value_col_swigregister = _blcelc.celData_value_col_swigregister
celData_value_col_swigregister(celData_value_col)

class celData_value_v(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, celData_value_v, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, celData_value_v, name)
    __repr__ = _swig_repr
    __swig_setmethods__["x"] = _blcelc.celData_value_v_x_set
    __swig_getmethods__["x"] = _blcelc.celData_value_v_x_get
    if _newclass:x = property(_blcelc.celData_value_v_x_get, _blcelc.celData_value_v_x_set)
    __swig_setmethods__["y"] = _blcelc.celData_value_v_y_set
    __swig_getmethods__["y"] = _blcelc.celData_value_v_y_get
    if _newclass:y = property(_blcelc.celData_value_v_y_get, _blcelc.celData_value_v_y_set)
    __swig_setmethods__["z"] = _blcelc.celData_value_v_z_set
    __swig_getmethods__["z"] = _blcelc.celData_value_v_z_get
    if _newclass:z = property(_blcelc.celData_value_v_z_get, _blcelc.celData_value_v_z_set)
    def __init__(self, *args): 
        this = _blcelc.new_celData_value_v(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blcelc.delete_celData_value_v
    __del__ = lambda self : None;
celData_value_v_swigregister = _blcelc.celData_value_v_swigregister
celData_value_v_swigregister(celData_value_v)

CEL_EVENT_PRE = _blcelc.CEL_EVENT_PRE
CEL_EVENT_VIEW = _blcelc.CEL_EVENT_VIEW
CEL_EVENT_POST = _blcelc.CEL_EVENT_POST
class iCelNewEntityCallback(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelNewEntityCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelNewEntityCallback, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def NewEntity(*args): return _blcelc.iCelNewEntityCallback_NewEntity(*args)
    __swig_destroy__ = _blcelc.delete_iCelNewEntityCallback
    __del__ = lambda self : None;
iCelNewEntityCallback_swigregister = _blcelc.iCelNewEntityCallback_swigregister
iCelNewEntityCallback_swigregister(iCelNewEntityCallback)

class iCelEntityRemoveCallback(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityRemoveCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntityRemoveCallback, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def RemoveEntity(*args): return _blcelc.iCelEntityRemoveCallback_RemoveEntity(*args)
    __swig_destroy__ = _blcelc.delete_iCelEntityRemoveCallback
    __del__ = lambda self : None;
iCelEntityRemoveCallback_swigregister = _blcelc.iCelEntityRemoveCallback_swigregister
iCelEntityRemoveCallback_swigregister(iCelEntityRemoveCallback)

class iCelTimerListener(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelTimerListener, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelTimerListener, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def TickEveryFrame(*args): return _blcelc.iCelTimerListener_TickEveryFrame(*args)
    def TickOnce(*args): return _blcelc.iCelTimerListener_TickOnce(*args)
    __swig_destroy__ = _blcelc.delete_iCelTimerListener
    __del__ = lambda self : None;
iCelTimerListener_swigregister = _blcelc.iCelTimerListener_swigregister
iCelTimerListener_swigregister(iCelTimerListener)

class iCelPlLayer(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPlLayer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPlLayer, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def CreateEntityInScope(*args): return _blcelc.iCelPlLayer_CreateEntityInScope(*args)
    def RemoveEntity(*args): return _blcelc.iCelPlLayer_RemoveEntity(*args)
    def RemoveEntities(*args): return _blcelc.iCelPlLayer_RemoveEntities(*args)
    def CreateEntityTemplate(*args): return _blcelc.iCelPlLayer_CreateEntityTemplate(*args)
    def RemoveEntityTemplate(*args): return _blcelc.iCelPlLayer_RemoveEntityTemplate(*args)
    def RemoveEntityTemplates(*args): return _blcelc.iCelPlLayer_RemoveEntityTemplates(*args)
    def FindEntityTemplate(*args): return _blcelc.iCelPlLayer_FindEntityTemplate(*args)
    def GetEntityTemplateCount(*args): return _blcelc.iCelPlLayer_GetEntityTemplateCount(*args)
    def GetEntityTemplate(*args): return _blcelc.iCelPlLayer_GetEntityTemplate(*args)
    def CreateEntity(*args): return _blcelc.iCelPlLayer_CreateEntity(*args)
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
    EntityTemplateCount = property(_blcelc.iCelPlLayer_EntityTemplateCount_get, None, None,
                    "iCelPlLayer.EntityTemplateCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelPlLayer::GetEntityTemplateCount()")

    EntityCount = property(_blcelc.iCelPlLayer_EntityCount_get, None, None,
                    "iCelPlLayer.EntityCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelPlLayer::GetEntityCount()")

    __swig_setmethods__["EntityAddonAllowed"] = _blcelc.iCelPlLayer_EntityAddonAllowed_set
    EntityAddonAllowed = property(_blcelc.iCelPlLayer_EntityAddonAllowed_get, _blcelc.iCelPlLayer_EntityAddonAllowed_set, None,
                    "iCelPlLayer.EntityAddonAllowed -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iCelPlLayer::IsEntityAddonAllowed()\n\tset: void iCelPlLayer::SetEntityAddonAllowed(bool)")


    PropertyClassFactoryCount = property(_blcelc.iCelPlLayer_PropertyClassFactoryCount_get, None, None,
                    "iCelPlLayer.PropertyClassFactoryCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelPlLayer::GetPropertyClassFactoryCount()")

    BehaviourLayerCount = property(_blcelc.iCelPlLayer_BehaviourLayerCount_get, None, None,
                    "iCelPlLayer.BehaviourLayerCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelPlLayer::GetBehaviourLayerCount()")

    def CreateParameterBlock(self,valdict):
    	"""Create a celGenericParameterBlock from a dict, list or
    	tuple"""
    	parblock = celGenericParameterBlock(len(valdict))
    	for idx,valkey in enumerate(valdict):
    		keyid = self.FetchStringID("cel.parameter."+valkey)
    		parblock.SetParameterDef (idx,keyid,valkey)
    		if type(valdict) == type({}):
    			parblock[keyid] = valdict[valkey]
    	return parblock	
    def GetEntities(self): return iCelEntityPlFakeArray(self)
    def GetEntityTemplates(self): return iCelEntityTemplatePlFakeArray(self)
    def GetPcFactories(self): return iCelPropertyClassFactoryPlFakeArray(self)
    def GetBehaviourLayers(self): return iCelBlLayerPlFakeArray(self)
    __swig_getmethods__["EntityTemplates"] = lambda self: self.GetEntityTemplates()
    __swig_getmethods__["PcFactories"] = lambda self: self.GetPcFactories()
    __swig_getmethods__["BehaviourLayers"] = lambda self: self.GetBehaviourLayers()
    __swig_getmethods__["Entities"] = lambda self: self.GetEntities() 
    __swig_destroy__ = _blcelc.delete_iCelPlLayer
    __del__ = lambda self : None;
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCelPlLayer_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCelPlLayer_scfGetVersion)
iCelPlLayer_swigregister = _blcelc.iCelPlLayer_swigregister
iCelPlLayer_swigregister(iCelPlLayer)
iCelPlLayer_scfGetVersion = _blcelc.iCelPlLayer_scfGetVersion

class iCelEntityTracker(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityTracker, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntityTracker, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetName(*args): return _blcelc.iCelEntityTracker_GetName(*args)
    def AddEntity(*args): return _blcelc.iCelEntityTracker_AddEntity(*args)
    def RemoveEntity(*args): return _blcelc.iCelEntityTracker_RemoveEntity(*args)
    def FindNearbyEntities(*args): return _blcelc.iCelEntityTracker_FindNearbyEntities(*args)
    def GetIterator(*args): return _blcelc.iCelEntityTracker_GetIterator(*args)
    def AddEntities(*args): return _blcelc.iCelEntityTracker_AddEntities(*args)
    def RemoveEntities(*args): return _blcelc.iCelEntityTracker_RemoveEntities(*args)
    Name = property(_blcelc.iCelEntityTracker_Name_get, None, None,
                    "iCelEntityTracker.Name -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelEntityTracker::GetName()")

    Iterator = property(_blcelc.iCelEntityTracker_Iterator_get, None, None,
                    "iCelEntityTracker.Iterator -> csPtr<iCelEntityIterator>  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: csPtr<iCelEntityIterator> iCelEntityTracker::GetIterator()")

    __swig_destroy__ = _blcelc.delete_iCelEntityTracker
    __del__ = lambda self : None;
iCelEntityTracker_swigregister = _blcelc.iCelEntityTracker_swigregister
iCelEntityTracker_swigregister(iCelEntityTracker)

csQueryRegistry_iCelPlLayer = _blcelc.csQueryRegistry_iCelPlLayer
class iCelEntityTemplatePlFakeArray:
	def __init__(self,parent): self.parent = parent
	def __contains__(self,obj):
		if self.parent.FindEntityTemplate(obj): return True
		else: return False
	def __repr__(self): return "List of "+str("iCelEntityTemplate")
	def __len__(self): return self.parent.GetEntityTemplateCount()
	def __delitem__(self,val):
		if type(val) == type(""):
			obj = self.parent.FindEntityTemplate(val)
			if obj: return self.parent.RemoveEntityTemplate(obj)
			else: raise IndexError(val+" not in list")
		else: return self.parent.RemoveEntityTemplate(val)
	def __noappend__(self,obj):
		print "Append not supported by this list"
	def content_iterator(self):
		for idx in xrange(len(self)):
			yield self.parent.GetEntityTemplateByIndex(idx)
	def __iter__(self): return self.content_iterator()
	def __getitem__(self,val):
		if type(val) == type(""): return self.parent.FindEntityTemplate(val)
		else: return self.parent.GetEntityTemplateByIndex(val)
	def append(self,obj): return self.parent.__noappend__(obj)

class iCelEntityPlFakeArray:
	def __init__(self,parent): self.parent = parent
	def __contains__(self,obj):
		if self.parent.FindEntity(obj): return True
		else: return False
	def __repr__(self): return "List of "+str("iCelEntity")
	def __len__(self): return self.parent.GetEntityCount()
	def __delitem__(self,val):
		if type(val) == type(""):
			obj = self.parent.FindEntity(val)
			if obj: return self.parent.RemoveEntity(obj)
			else: raise IndexError(val+" not in list")
		else: return self.parent.RemoveEntity(val)
	def __noappend__(self,obj):
		print "Append not supported by this list"
	def content_iterator(self):
		for idx in xrange(len(self)):
			yield self.parent.GetEntityByIndex(idx)
	def __iter__(self): return self.content_iterator()
	def __getitem__(self,val):
		if type(val) == type(""): return self.parent.FindEntity(val)
		else: return self.parent.GetEntityByIndex(val)
	def append(self,obj): return self.parent.__noappend__(obj)

class iCelBlLayerPlFakeArray:
	def __init__(self,parent): self.parent = parent
	def __contains__(self,obj):
		if self.parent.FindBehaviourLayer(obj): return True
		else: return False
	def __repr__(self): return "List of "+str("iCelBlLayer")
	def __len__(self): return self.parent.GetBehaviourLayerCount()
	def __delitem__(self,val):
		if type(val) == type(""):
			obj = self.parent.FindBehaviourLayer(val)
			if obj: return self.parent.UnregisterBehaviourLayer(obj)
			else: raise IndexError(val+" not in list")
		else: return self.parent.UnregisterBehaviourLayer(val)
	def __noappend__(self,obj):
		print "Append not supported by this list"
	def content_iterator(self):
		for idx in xrange(len(self)):
			yield self.parent.GetBehaviourLayer(idx)
	def __iter__(self): return self.content_iterator()
	def __getitem__(self,val):
		if type(val) == type(""): return self.parent.FindBehaviourLayer(val)
		else: return self.parent.GetBehaviourLayer(val)
	def append(self,obj): return self.parent.RegisterBehaviourLayer(obj)

class iCelPropertyClassFactoryPlFakeArray:
	def __init__(self,parent): self.parent = parent
	def __contains__(self,obj):
		if self.parent.FindPropertyClassFactory(obj): return True
		else: return False
	def __repr__(self): return "List of "+str("iCelPropertyClassFactory")
	def __len__(self): return self.parent.GetPropertyClassFactoryCount()
	def __delitem__(self,val):
		if type(val) == type(""):
			obj = self.parent.FindPropertyClassFactory(val)
			if obj: return self.parent.UnregisterPropertyClassFactory(obj)
			else: raise IndexError(val+" not in list")
		else: return self.parent.UnregisterPropertyClassFactory(val)
	def __noappend__(self,obj):
		print "Append not supported by this list"
	def content_iterator(self):
		for idx in xrange(len(self)):
			yield self.parent.GetPropertyClassFactory(idx)
	def __iter__(self): return self.content_iterator()
	def __getitem__(self,val):
		if type(val) == type(""): return self.parent.FindPropertyClassFactory(val)
		else: return self.parent.GetPropertyClassFactory(val)
	def append(self,obj): return self.parent.LoadPropertyClassFactory(obj)

class iCelEntity(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntity, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntity, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def QueryObject(*args): return _blcelc.iCelEntity_QueryObject(*args)
    def GetName(*args): return _blcelc.iCelEntity_GetName(*args)
    def SetName(*args): return _blcelc.iCelEntity_SetName(*args)
    def GetID(*args): return _blcelc.iCelEntity_GetID(*args)
    def SetID(*args): return _blcelc.iCelEntity_SetID(*args)
    def GetPropertyClassList(*args): return _blcelc.iCelEntity_GetPropertyClassList(*args)
    def SetBehaviour(*args): return _blcelc.iCelEntity_SetBehaviour(*args)
    def GetBehaviour(*args): return _blcelc.iCelEntity_GetBehaviour(*args)
    def AddClass(*args): return _blcelc.iCelEntity_AddClass(*args)
    def RemoveClass(*args): return _blcelc.iCelEntity_RemoveClass(*args)
    def HasClass(*args): return _blcelc.iCelEntity_HasClass(*args)
    def GetClasses(*args): return _blcelc.iCelEntity_GetClasses(*args)
    Object = property(_blcelc.iCelEntity_Object_get, None, None,
                    "iCelEntity.Object -> iObject*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iObject* iCelEntity::QueryObject()")

    __swig_setmethods__["Name"] = _blcelc.iCelEntity_Name_set
    Name = property(_blcelc.iCelEntity_Name_get, _blcelc.iCelEntity_Name_set, None,
                    "iCelEntity.Name -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iCelEntity::GetName()\n\tset: void iCelEntity::SetName(const char*)")


    __swig_setmethods__["ID"] = _blcelc.iCelEntity_ID_set
    ID = property(_blcelc.iCelEntity_ID_get, _blcelc.iCelEntity_ID_set, None,
                    "iCelEntity.ID -> uint\n\nThis is equivalent to calling the C++ cs methods:\n\tget: uint iCelEntity::GetID()\n\tset: void iCelEntity::SetID(uint)")


    PropertyClassList = property(_blcelc.iCelEntity_PropertyClassList_get, None, None,
                    "iCelEntity.PropertyClassList -> iCelPropertyClassList*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iCelPropertyClassList* iCelEntity::GetPropertyClassList()")

    __swig_setmethods__["Behaviour"] = _blcelc.iCelEntity_Behaviour_set
    Behaviour = property(_blcelc.iCelEntity_Behaviour_get, _blcelc.iCelEntity_Behaviour_set, None,
                    "iCelEntity.Behaviour -> iCelBehaviour*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iCelBehaviour* iCelEntity::GetBehaviour()\n\tset: void iCelEntity::SetBehaviour(iCelBehaviour*)")


    Classes = property(_blcelc.iCelEntity_Classes_get, None, None,
                    "iCelEntity.Classes -> const csSet<csStringID>&  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const csSet<csStringID>& iCelEntity::GetClasses()")

    def CreateBehaviour(*args): return _blcelc.iCelEntity_CreateBehaviour(*args)
    __swig_destroy__ = _blcelc.delete_iCelEntity
    __del__ = lambda self : None;
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCelEntity_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCelEntity_scfGetVersion)
iCelEntity_swigregister = _blcelc.iCelEntity_swigregister
iCelEntity_swigregister(iCelEntity)
iCelEntity_scfGetVersion = _blcelc.iCelEntity_scfGetVersion

class iCelEntityList(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityList, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntityList, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetCount(*args): return _blcelc.iCelEntityList_GetCount(*args)
    def Get(*args): return _blcelc.iCelEntityList_Get(*args)
    def Add(*args): return _blcelc.iCelEntityList_Add(*args)
    def Remove(*args): return _blcelc.iCelEntityList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iCelEntityList_RemoveAll(*args)
    def Find(*args): return _blcelc.iCelEntityList_Find(*args)
    def FindByName(*args): return _blcelc.iCelEntityList_FindByName(*args)
    def GetIterator(*args): return _blcelc.iCelEntityList_GetIterator(*args)
    Count = property(_blcelc.iCelEntityList_Count_get, None, None,
                    "iCelEntityList.Count -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelEntityList::GetCount()")

    Iterator = property(_blcelc.iCelEntityList_Iterator_get, None, None,
                    "iCelEntityList.Iterator -> csPtr<iCelEntityIterator>  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: csPtr<iCelEntityIterator> iCelEntityList::GetIterator()")

    def __delitem__(*args): return _blcelc.iCelEntityList___delitem__(*args)
    def __len__(*args): return _blcelc.iCelEntityList___len__(*args)
    def append(*args): return _blcelc.iCelEntityList_append(*args)
    def content_iterator(self):
            for idx in xrange(len(self)):
                    yield self.Get(idx)
    def __iter__(self): return self.content_iterator()  
    def __getitem__(*args): return _blcelc.iCelEntityList___getitem__(*args)
    def __contains__(*args): return _blcelc.iCelEntityList___contains__(*args)
    __swig_destroy__ = _blcelc.delete_iCelEntityList
    __del__ = lambda self : None;
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCelEntityList_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCelEntityList_scfGetVersion)
iCelEntityList_swigregister = _blcelc.iCelEntityList_swigregister
iCelEntityList_swigregister(iCelEntityList)
iCelEntityList_scfGetVersion = _blcelc.iCelEntityList_scfGetVersion

class iCelEntityIterator(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntityIterator, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def Next(*args): return _blcelc.iCelEntityIterator_Next(*args)
    def HasNext(*args): return _blcelc.iCelEntityIterator_HasNext(*args)
    Next = property(_blcelc.iCelEntityIterator_Next_get, None, None,
                    "iCelEntityIterator.Next -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iCelEntityIterator::HasNext()")

    __swig_destroy__ = _blcelc.delete_iCelEntityIterator
    __del__ = lambda self : None;
iCelEntityIterator_swigregister = _blcelc.iCelEntityIterator_swigregister
iCelEntityIterator_swigregister(iCelEntityIterator)

celRegisterPCFactory = _blcelc.celRegisterPCFactory
celCreateEntity = _blcelc.celCreateEntity
scfQueryInterface_iCelEntity = _blcelc.scfQueryInterface_iCelEntity
class iCelEntityTemplate(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityTemplate, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntityTemplate, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def QueryObject(*args): return _blcelc.iCelEntityTemplate_QueryObject(*args)
    def GetName(*args): return _blcelc.iCelEntityTemplate_GetName(*args)
    def SetName(*args): return _blcelc.iCelEntityTemplate_SetName(*args)
    def CreatePropertyClassTemplate(*args): return _blcelc.iCelEntityTemplate_CreatePropertyClassTemplate(*args)
    def SetBehaviour(*args): return _blcelc.iCelEntityTemplate_SetBehaviour(*args)
    def GetBehaviourLayer(*args): return _blcelc.iCelEntityTemplate_GetBehaviourLayer(*args)
    def GetBehaviour(*args): return _blcelc.iCelEntityTemplate_GetBehaviour(*args)
    def AddMessage(*args): return _blcelc.iCelEntityTemplate_AddMessage(*args)
    def AddClass(*args): return _blcelc.iCelEntityTemplate_AddClass(*args)
    def RemoveClass(*args): return _blcelc.iCelEntityTemplate_RemoveClass(*args)
    def HasClass(*args): return _blcelc.iCelEntityTemplate_HasClass(*args)
    def GetClasses(*args): return _blcelc.iCelEntityTemplate_GetClasses(*args)
    Object = property(_blcelc.iCelEntityTemplate_Object_get, None, None,
                    "iCelEntityTemplate.Object -> iObject*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iObject* iCelEntityTemplate::QueryObject()")

    __swig_setmethods__["Name"] = _blcelc.iCelEntityTemplate_Name_set
    Name = property(_blcelc.iCelEntityTemplate_Name_get, _blcelc.iCelEntityTemplate_Name_set, None,
                    "iCelEntityTemplate.Name -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iCelEntityTemplate::GetName()\n\tset: void iCelEntityTemplate::SetName(const char*)")


    __swig_setmethods__["Behaviour"] = fix_args(_blcelc.iCelEntityTemplate_SetBehaviour)
    Behaviour = property(_blcelc.iCelEntityTemplate_GetBehaviour, fix_args(_blcelc.iCelEntityTemplate_SetBehaviour), None,
                    "iCelEntityTemplate.Behaviour -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iCelEntityTemplate::GetBehaviour()\n\tset: void iCelEntityTemplate::SetBehaviour(...)")

    BehaviourLayer = property(_blcelc.iCelEntityTemplate_BehaviourLayer_get, None, None,
                    "iCelEntityTemplate.BehaviourLayer -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelEntityTemplate::GetBehaviourLayer()")

    Classes = property(_blcelc.iCelEntityTemplate_Classes_get, None, None,
                    "iCelEntityTemplate.Classes -> const csSet<csStringID>&  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const csSet<csStringID>& iCelEntityTemplate::GetClasses()")

    __swig_destroy__ = _blcelc.delete_iCelEntityTemplate
    __del__ = lambda self : None;
iCelEntityTemplate_swigregister = _blcelc.iCelEntityTemplate_swigregister
iCelEntityTemplate_swigregister(iCelEntityTemplate)
celFindNearbyEntities = _blcelc.celFindNearbyEntities

class iCelPropertyClassTemplate(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClassTemplate, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClassTemplate, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetName(*args): return _blcelc.iCelPropertyClassTemplate_SetName(*args)
    def GetName(*args): return _blcelc.iCelPropertyClassTemplate_GetName(*args)
    def SetTag(*args): return _blcelc.iCelPropertyClassTemplate_SetTag(*args)
    def GetTag(*args): return _blcelc.iCelPropertyClassTemplate_GetTag(*args)
    def SetPropertyVariable(*args): return _blcelc.iCelPropertyClassTemplate_SetPropertyVariable(*args)
    def SetProperty(*args): return _blcelc.iCelPropertyClassTemplate_SetProperty(*args)
    def PerformAction(*args): return _blcelc.iCelPropertyClassTemplate_PerformAction(*args)
    __swig_setmethods__["Name"] = _blcelc.iCelPropertyClassTemplate_Name_set
    Name = property(_blcelc.iCelPropertyClassTemplate_Name_get, _blcelc.iCelPropertyClassTemplate_Name_set, None,
                    "iCelPropertyClassTemplate.Name -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iCelPropertyClassTemplate::GetName()\n\tset: void iCelPropertyClassTemplate::SetName(const char*)")


    __swig_setmethods__["Tag"] = _blcelc.iCelPropertyClassTemplate_Tag_set
    Tag = property(_blcelc.iCelPropertyClassTemplate_Tag_get, _blcelc.iCelPropertyClassTemplate_Tag_set, None,
                    "iCelPropertyClassTemplate.Tag -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iCelPropertyClassTemplate::GetTag()\n\tset: void iCelPropertyClassTemplate::SetTag(const char*)")


    __swig_setmethods__["PropertyVariable"] = fix_args(_blcelc.iCelPropertyClassTemplate_SetPropertyVariable)
    PropertyVariable = property(None, fix_args(_blcelc.iCelPropertyClassTemplate_SetPropertyVariable), None,
                    "iCelPropertyClassTemplate.PropertyVariable -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iCelPropertyClassTemplate::getmethod()\n\tset: void iCelPropertyClassTemplate::SetPropertyVariable(...)")

    __swig_setmethods__["Property"] = fix_args(_blcelc.iCelPropertyClassTemplate_SetProperty)
    Property = property(None, fix_args(_blcelc.iCelPropertyClassTemplate_SetProperty), None,
                    "iCelPropertyClassTemplate.Property -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iCelPropertyClassTemplate::getmethod()\n\tset: void iCelPropertyClassTemplate::SetProperty(...)")

    __swig_destroy__ = _blcelc.delete_iCelPropertyClassTemplate
    __del__ = lambda self : None;
iCelPropertyClassTemplate_swigregister = _blcelc.iCelPropertyClassTemplate_swigregister
iCelPropertyClassTemplate_swigregister(iCelPropertyClassTemplate)

class iCelBlLayer(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelBlLayer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelBlLayer, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetName(*args): return _blcelc.iCelBlLayer_GetName(*args)
    def CreateBehaviour(*args): return _blcelc.iCelBlLayer_CreateBehaviour(*args)
    Name = property(_blcelc.iCelBlLayer_Name_get, None, None,
                    "iCelBlLayer.Name -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelBlLayer::GetName()")

    __swig_destroy__ = _blcelc.delete_iCelBlLayer
    __del__ = lambda self : None;
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCelBlLayer_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCelBlLayer_scfGetVersion)
iCelBlLayer_swigregister = _blcelc.iCelBlLayer_swigregister
iCelBlLayer_swigregister(iCelBlLayer)
iCelBlLayer_scfGetVersion = _blcelc.iCelBlLayer_scfGetVersion

csQueryRegistry_iCelBlLayer = _blcelc.csQueryRegistry_iCelBlLayer
class iCelParameterBlock(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelParameterBlock, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelParameterBlock, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetParameterCount(*args): return _blcelc.iCelParameterBlock_GetParameterCount(*args)
    def GetParameter(*args): return _blcelc.iCelParameterBlock_GetParameter(*args)
    def GetParameterByIndex(*args): return _blcelc.iCelParameterBlock_GetParameterByIndex(*args)
    ParameterCount = property(_blcelc.iCelParameterBlock_ParameterCount_get, None, None,
                    "iCelParameterBlock.ParameterCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelParameterBlock::GetParameterCount()")

    def GetParameterIDByIndex(*args): return _blcelc.iCelParameterBlock_GetParameterIDByIndex(*args)
    def GetParameterNameByIndex(*args): return _blcelc.iCelParameterBlock_GetParameterNameByIndex(*args)
    def __contains__(*args): return _blcelc.iCelParameterBlock___contains__(*args)
    def value_iterator(parblock):
    	for idx in xrange(len(parblock)):
    		yield parblock.GetParameterByIndex(idx)
    def key_iterator(parblock):
    	for idx in xrange(len(parblock)):
    		yield parblock.GetParameterIDByIndex(idx)
    def name_iterator(parblock):
    	for idx in xrange(len(parblock)):
    		yield parblock.GetParameterNameByIndex(idx)
    def GetParameterValue(self,id):
    	return self.GetParameter(id)
    def __iter__(self):
    	return self.value_iterator()
    def iterkeys(self):
    	return self.key_iterator()
    def iternames(self):
    	return self.name_iterator()
    def __getitem__(self,i):
    	return self.GetParameter(i)
    def keys(self):
    	keys = []
    	for idx in xrange(len(self)):
    		keys.append(self.GetParameterIDByIndex(idx))
    	return keys
    def names(self):
    	keys = []
    	for idx in xrange(len(self)):
    		keys.append(self.GetParameterNameByIndex(idx))
    	return keys
    def has_key(self,id):
    	return self.__contains__(id)

    def __setitem__(*args): return _blcelc.iCelParameterBlock___setitem__(*args)
    def __len__(*args): return _blcelc.iCelParameterBlock___len__(*args)
    __swig_destroy__ = _blcelc.delete_iCelParameterBlock
    __del__ = lambda self : None;
iCelParameterBlock_swigregister = _blcelc.iCelParameterBlock_swigregister
iCelParameterBlock_swigregister(iCelParameterBlock)

class iCelBehaviour(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelBehaviour, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelBehaviour, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetName(*args): return _blcelc.iCelBehaviour_GetName(*args)
    def GetBehaviourLayer(*args): return _blcelc.iCelBehaviour_GetBehaviourLayer(*args)
    Name = property(_blcelc.iCelBehaviour_Name_get, None, None,
                    "iCelBehaviour.Name -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelBehaviour::GetName()")

    BehaviourLayer = property(_blcelc.iCelBehaviour_BehaviourLayer_get, None, None,
                    "iCelBehaviour.BehaviourLayer -> iCelBlLayer*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iCelBlLayer* iCelBehaviour::GetBehaviourLayer()")

    def GetPythonObject(*args): return _blcelc.iCelBehaviour_GetPythonObject(*args)
    def SendMessage(*args): return _blcelc.iCelBehaviour_SendMessage(*args)
    __swig_destroy__ = _blcelc.delete_iCelBehaviour
    __del__ = lambda self : None;
iCelBehaviour_swigregister = _blcelc.iCelBehaviour_swigregister
iCelBehaviour_swigregister(iCelBehaviour)

class celGenericParameterBlock(iCelParameterBlock):
    __swig_setmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, celGenericParameterBlock, name, value)
    __swig_getmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, celGenericParameterBlock, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _blcelc.new_celGenericParameterBlock(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blcelc.delete_celGenericParameterBlock
    __del__ = lambda self : None;
    def SetParameterDef(*args): return _blcelc.celGenericParameterBlock_SetParameterDef(*args)
    __swig_setmethods__["scfRefCount"] = _blcelc.celGenericParameterBlock_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.celGenericParameterBlock_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.celGenericParameterBlock_scfRefCount_get, _blcelc.celGenericParameterBlock_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.celGenericParameterBlock_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.celGenericParameterBlock_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.celGenericParameterBlock_scfWeakRefOwners_get, _blcelc.celGenericParameterBlock_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc.celGenericParameterBlock_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc.celGenericParameterBlock_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.celGenericParameterBlock_scfParent_get
    if _newclass:scfParent = property(_blcelc.celGenericParameterBlock_scfParent_get, _blcelc.celGenericParameterBlock_scfParent_set)
    def IncRef(*args): return _blcelc.celGenericParameterBlock_IncRef(*args)
    def DecRef(*args): return _blcelc.celGenericParameterBlock_DecRef(*args)
    def GetRefCount(*args): return _blcelc.celGenericParameterBlock_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.celGenericParameterBlock_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.celGenericParameterBlock_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.celGenericParameterBlock_QueryInterface(*args)
    def GetParameterCount(*args): return _blcelc.celGenericParameterBlock_GetParameterCount(*args)
    def GetParameter(*args): return _blcelc.celGenericParameterBlock_GetParameter(*args)
    def GetParameterByIndex(*args): return _blcelc.celGenericParameterBlock_GetParameterByIndex(*args)
    __swig_setmethods__["ParameterDef"] = fix_args(_blcelc.celGenericParameterBlock_SetParameterDef)
    ParameterDef = property(None, fix_args(_blcelc.celGenericParameterBlock_SetParameterDef), None,
                    "celGenericParameterBlock.ParameterDef -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: celGenericParameterBlock::getmethod()\n\tset: void celGenericParameterBlock::SetParameterDef(...)")

    ParameterCount = property(_blcelc.celGenericParameterBlock_ParameterCount_get, None, None,
                    "celGenericParameterBlock.ParameterCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t celGenericParameterBlock::GetParameterCount()")

celGenericParameterBlock_swigregister = _blcelc.celGenericParameterBlock_swigregister
celGenericParameterBlock_swigregister(celGenericParameterBlock)

class celVariableParameterBlock(iCelParameterBlock):
    __swig_setmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, celVariableParameterBlock, name, value)
    __swig_getmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, celVariableParameterBlock, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _blcelc.new_celVariableParameterBlock(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blcelc.delete_celVariableParameterBlock
    __del__ = lambda self : None;
    def SetParameterDef(*args): return _blcelc.celVariableParameterBlock_SetParameterDef(*args)
    __swig_setmethods__["scfRefCount"] = _blcelc.celVariableParameterBlock_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.celVariableParameterBlock_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.celVariableParameterBlock_scfRefCount_get, _blcelc.celVariableParameterBlock_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.celVariableParameterBlock_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.celVariableParameterBlock_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.celVariableParameterBlock_scfWeakRefOwners_get, _blcelc.celVariableParameterBlock_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc.celVariableParameterBlock_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc.celVariableParameterBlock_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.celVariableParameterBlock_scfParent_get
    if _newclass:scfParent = property(_blcelc.celVariableParameterBlock_scfParent_get, _blcelc.celVariableParameterBlock_scfParent_set)
    def IncRef(*args): return _blcelc.celVariableParameterBlock_IncRef(*args)
    def DecRef(*args): return _blcelc.celVariableParameterBlock_DecRef(*args)
    def GetRefCount(*args): return _blcelc.celVariableParameterBlock_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.celVariableParameterBlock_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.celVariableParameterBlock_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.celVariableParameterBlock_QueryInterface(*args)
    def GetParameterCount(*args): return _blcelc.celVariableParameterBlock_GetParameterCount(*args)
    def GetParameter(*args): return _blcelc.celVariableParameterBlock_GetParameter(*args)
    def GetParameterByIndex(*args): return _blcelc.celVariableParameterBlock_GetParameterByIndex(*args)
    __swig_setmethods__["ParameterDef"] = fix_args(_blcelc.celVariableParameterBlock_SetParameterDef)
    ParameterDef = property(None, fix_args(_blcelc.celVariableParameterBlock_SetParameterDef), None,
                    "celVariableParameterBlock.ParameterDef -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: celVariableParameterBlock::getmethod()\n\tset: void celVariableParameterBlock::SetParameterDef(...)")

    ParameterCount = property(_blcelc.celVariableParameterBlock_ParameterCount_get, None, None,
                    "celVariableParameterBlock.ParameterCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t celVariableParameterBlock::GetParameterCount()")

celVariableParameterBlock_swigregister = _blcelc.celVariableParameterBlock_swigregister
celVariableParameterBlock_swigregister(celVariableParameterBlock)

class celOneParameterBlock(iCelParameterBlock):
    __swig_setmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, celOneParameterBlock, name, value)
    __swig_getmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, celOneParameterBlock, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _blcelc.new_celOneParameterBlock(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blcelc.delete_celOneParameterBlock
    __del__ = lambda self : None;
    def SetParameterDef(*args): return _blcelc.celOneParameterBlock_SetParameterDef(*args)
    __swig_setmethods__["scfRefCount"] = _blcelc.celOneParameterBlock_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.celOneParameterBlock_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.celOneParameterBlock_scfRefCount_get, _blcelc.celOneParameterBlock_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.celOneParameterBlock_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.celOneParameterBlock_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.celOneParameterBlock_scfWeakRefOwners_get, _blcelc.celOneParameterBlock_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc.celOneParameterBlock_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc.celOneParameterBlock_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.celOneParameterBlock_scfParent_get
    if _newclass:scfParent = property(_blcelc.celOneParameterBlock_scfParent_get, _blcelc.celOneParameterBlock_scfParent_set)
    def IncRef(*args): return _blcelc.celOneParameterBlock_IncRef(*args)
    def DecRef(*args): return _blcelc.celOneParameterBlock_DecRef(*args)
    def GetRefCount(*args): return _blcelc.celOneParameterBlock_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.celOneParameterBlock_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.celOneParameterBlock_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.celOneParameterBlock_QueryInterface(*args)
    def GetParameterCount(*args): return _blcelc.celOneParameterBlock_GetParameterCount(*args)
    def GetParameter(*args): return _blcelc.celOneParameterBlock_GetParameter(*args)
    def GetParameterByIndex(*args): return _blcelc.celOneParameterBlock_GetParameterByIndex(*args)
    __swig_setmethods__["ParameterDef"] = fix_args(_blcelc.celOneParameterBlock_SetParameterDef)
    ParameterDef = property(None, fix_args(_blcelc.celOneParameterBlock_SetParameterDef), None,
                    "celOneParameterBlock.ParameterDef -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: celOneParameterBlock::getmethod()\n\tset: void celOneParameterBlock::SetParameterDef(...)")

    ParameterCount = property(_blcelc.celOneParameterBlock_ParameterCount_get, None, None,
                    "celOneParameterBlock.ParameterCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t celOneParameterBlock::GetParameterCount()")

celOneParameterBlock_swigregister = _blcelc.celOneParameterBlock_swigregister
celOneParameterBlock_swigregister(celOneParameterBlock)

class iCelPropertyClassFactory(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClassFactory, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClassFactory, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetName(*args): return _blcelc.iCelPropertyClassFactory_GetName(*args)
    def CreatePropertyClass(*args): return _blcelc.iCelPropertyClassFactory_CreatePropertyClass(*args)
    Name = property(_blcelc.iCelPropertyClassFactory_Name_get, None, None,
                    "iCelPropertyClassFactory.Name -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelPropertyClassFactory::GetName()")

    __swig_destroy__ = _blcelc.delete_iCelPropertyClassFactory
    __del__ = lambda self : None;
iCelPropertyClassFactory_swigregister = _blcelc.iCelPropertyClassFactory_swigregister
iCelPropertyClassFactory_swigregister(iCelPropertyClassFactory)

class iCelPropertyClass(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClass, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClass, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    def GetPropertyIBase(*args): return _blcelc.iCelPropertyClass_GetPropertyIBase(*args)
    def PerformAction(*args): return _blcelc.iCelPropertyClass_PerformAction(*args)
    def GetPropertyAndActionCount(*args): return _blcelc.iCelPropertyClass_GetPropertyAndActionCount(*args)
    def GetPropertyOrActionID(*args): return _blcelc.iCelPropertyClass_GetPropertyOrActionID(*args)
    def GetPropertyOrActionDescription(*args): return _blcelc.iCelPropertyClass_GetPropertyOrActionDescription(*args)
    def PropertyClassesHaveChanged(*args): return _blcelc.iCelPropertyClass_PropertyClassesHaveChanged(*args)
    def GetPersistentData(*args): return _blcelc.iCelPropertyClass_GetPersistentData(*args)
    def SetPersistentData(*args): return _blcelc.iCelPropertyClass_SetPersistentData(*args)
    Name = property(_blcelc.iCelPropertyClass_Name_get, None, None,
                    "iCelPropertyClass.Name -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelPropertyClass::GetName()")

    __swig_setmethods__["Tag"] = _blcelc.iCelPropertyClass_Tag_set
    Tag = property(_blcelc.iCelPropertyClass_Tag_get, _blcelc.iCelPropertyClass_Tag_set, None,
                    "iCelPropertyClass.Tag -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iCelPropertyClass::GetTag()\n\tset: void iCelPropertyClass::SetTag(const char*)")


    __swig_setmethods__["Entity"] = _blcelc.iCelPropertyClass_Entity_set
    Entity = property(_blcelc.iCelPropertyClass_Entity_get, _blcelc.iCelPropertyClass_Entity_set, None,
                    "iCelPropertyClass.Entity -> iCelEntity*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iCelEntity* iCelPropertyClass::GetEntity()\n\tset: void iCelPropertyClass::SetEntity(iCelEntity*)")


    PropertyAndActionCount = property(_blcelc.iCelPropertyClass_PropertyAndActionCount_get, None, None,
                    "iCelPropertyClass.PropertyAndActionCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelPropertyClass::GetPropertyAndActionCount()")

    def SetPropertyLong(*args): return _blcelc.iCelPropertyClass_SetPropertyLong(*args)
    def SetPropertyFloat(*args): return _blcelc.iCelPropertyClass_SetPropertyFloat(*args)
    def SetPropertyBool(*args): return _blcelc.iCelPropertyClass_SetPropertyBool(*args)
    def SetPropertyString(*args): return _blcelc.iCelPropertyClass_SetPropertyString(*args)
    def SetPropertyVector3(*args): return _blcelc.iCelPropertyClass_SetPropertyVector3(*args)
    __swig_destroy__ = _blcelc.delete_iCelPropertyClass
    __del__ = lambda self : None;
iCelPropertyClass_swigregister = _blcelc.iCelPropertyClass_swigregister
iCelPropertyClass_swigregister(iCelPropertyClass)

class iCelPropertyClassList(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClassList, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClassList, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    Count = property(_blcelc.iCelPropertyClassList_Count_get, None, None,
                    "iCelPropertyClassList.Count -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelPropertyClassList::GetCount()")

    def __delitem__(*args): return _blcelc.iCelPropertyClassList___delitem__(*args)
    def __len__(*args): return _blcelc.iCelPropertyClassList___len__(*args)
    def append(*args): return _blcelc.iCelPropertyClassList_append(*args)
    def content_iterator(self):
            for idx in xrange(len(self)):
                    yield self.Get(idx)
    def __iter__(self): return self.content_iterator()  
    def __getitem__(*args): return _blcelc.iCelPropertyClassList___getitem__(*args)
    def __contains__(*args): return _blcelc.iCelPropertyClassList___contains__(*args)
    __swig_destroy__ = _blcelc.delete_iCelPropertyClassList
    __del__ = lambda self : None;
iCelPropertyClassList_swigregister = _blcelc.iCelPropertyClassList_swigregister
iCelPropertyClassList_swigregister(iCelPropertyClassList)

class iPcMechanicsSystem(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsSystem, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetDynamicSystem(*args): return _blcelc.iPcMechanicsSystem_SetDynamicSystem(*args)
    def GetDynamicSystem(*args): return _blcelc.iPcMechanicsSystem_GetDynamicSystem(*args)
    def SetStepTime(*args): return _blcelc.iPcMechanicsSystem_SetStepTime(*args)
    def GetStepTime(*args): return _blcelc.iPcMechanicsSystem_GetStepTime(*args)
    def SetSimulationSpeed(*args): return _blcelc.iPcMechanicsSystem_SetSimulationSpeed(*args)
    def GetSimulationSpeed(*args): return _blcelc.iPcMechanicsSystem_GetSimulationSpeed(*args)
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
    def DisableStepFast(*args): return _blcelc.iPcMechanicsSystem_DisableStepFast(*args)
    def EnableStepFast(*args): return _blcelc.iPcMechanicsSystem_EnableStepFast(*args)
    def EnableQuickStep(*args): return _blcelc.iPcMechanicsSystem_EnableQuickStep(*args)
    __swig_setmethods__["DynamicSystem"] = fix_args(_blcelc.iPcMechanicsSystem_SetDynamicSystem)
    DynamicSystem = property(_blcelc.iPcMechanicsSystem_GetDynamicSystem, fix_args(_blcelc.iPcMechanicsSystem_SetDynamicSystem), None,
                    "iPcMechanicsSystem.DynamicSystem -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMechanicsSystem::GetDynamicSystem()\n\tset: void iPcMechanicsSystem::SetDynamicSystem(...)")

    __swig_setmethods__["StepTime"] = _blcelc.iPcMechanicsSystem_StepTime_set
    StepTime = property(_blcelc.iPcMechanicsSystem_StepTime_get, _blcelc.iPcMechanicsSystem_StepTime_set, None,
                    "iPcMechanicsSystem.StepTime -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMechanicsSystem::GetStepTime()\n\tset: void iPcMechanicsSystem::SetStepTime(float)")


    __swig_setmethods__["SimulationSpeed"] = _blcelc.iPcMechanicsSystem_SimulationSpeed_set
    SimulationSpeed = property(_blcelc.iPcMechanicsSystem_SimulationSpeed_get, _blcelc.iPcMechanicsSystem_SimulationSpeed_set, None,
                    "iPcMechanicsSystem.SimulationSpeed -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMechanicsSystem::GetSimulationSpeed()\n\tset: void iPcMechanicsSystem::SetSimulationSpeed(float)")


    __swig_setmethods__["Gravity"] = _blcelc.iPcMechanicsSystem_Gravity_set
    Gravity = property(_blcelc.iPcMechanicsSystem_Gravity_get, _blcelc.iPcMechanicsSystem_Gravity_set, None,
                    "iPcMechanicsSystem.Gravity -> const csVector3\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csVector3 iPcMechanicsSystem::GetGravity()\n\tset: void iPcMechanicsSystem::SetGravity(const csVector3)")


    __swig_destroy__ = _blcelc.delete_iPcMechanicsSystem
    __del__ = lambda self : None;
iPcMechanicsSystem_swigregister = _blcelc.iPcMechanicsSystem_swigregister
iPcMechanicsSystem_swigregister(iPcMechanicsSystem)

class iPcMechanicsObject(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsObject, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsObject, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetMesh(*args): return _blcelc.iPcMechanicsObject_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcMechanicsObject_GetMesh(*args)
    def SetLight(*args): return _blcelc.iPcMechanicsObject_SetLight(*args)
    def GetLight(*args): return _blcelc.iPcMechanicsObject_GetLight(*args)
    def SetCamera(*args): return _blcelc.iPcMechanicsObject_SetCamera(*args)
    def GetCamera(*args): return _blcelc.iPcMechanicsObject_GetCamera(*args)
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
    def SetCollisionCallbackEnabled(*args): return _blcelc.iPcMechanicsObject_SetCollisionCallbackEnabled(*args)
    def IsCollisionCallbackEnabled(*args): return _blcelc.iPcMechanicsObject_IsCollisionCallbackEnabled(*args)
    def AttachColliderBoundingSphere(*args): return _blcelc.iPcMechanicsObject_AttachColliderBoundingSphere(*args)
    def AttachColliderSphere(*args): return _blcelc.iPcMechanicsObject_AttachColliderSphere(*args)
    def AttachColliderCylinder(*args): return _blcelc.iPcMechanicsObject_AttachColliderCylinder(*args)
    def AttachColliderBoundingBox(*args): return _blcelc.iPcMechanicsObject_AttachColliderBoundingBox(*args)
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
    def LocalToWorld(*args): return _blcelc.iPcMechanicsObject_LocalToWorld(*args)
    def WorldToLocal(*args): return _blcelc.iPcMechanicsObject_WorldToLocal(*args)
    __swig_setmethods__["Mesh"] = _blcelc.iPcMechanicsObject_Mesh_set
    Mesh = property(_blcelc.iPcMechanicsObject_Mesh_get, _blcelc.iPcMechanicsObject_Mesh_set, None,
                    "iPcMechanicsObject.Mesh -> iPcMesh*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMesh* iPcMechanicsObject::GetMesh()\n\tset: void iPcMechanicsObject::SetMesh(iPcMesh*)")


    __swig_setmethods__["Light"] = _blcelc.iPcMechanicsObject_Light_set
    Light = property(_blcelc.iPcMechanicsObject_Light_get, _blcelc.iPcMechanicsObject_Light_set, None,
                    "iPcMechanicsObject.Light -> iPcLight*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcLight* iPcMechanicsObject::GetLight()\n\tset: void iPcMechanicsObject::SetLight(iPcLight*)")


    __swig_setmethods__["Camera"] = _blcelc.iPcMechanicsObject_Camera_set
    Camera = property(_blcelc.iPcMechanicsObject_Camera_get, _blcelc.iPcMechanicsObject_Camera_set, None,
                    "iPcMechanicsObject.Camera -> iPcCamera*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcCamera* iPcMechanicsObject::GetCamera()\n\tset: void iPcMechanicsObject::SetCamera(iPcCamera*)")


    __swig_setmethods__["MechanicsSystem"] = _blcelc.iPcMechanicsObject_MechanicsSystem_set
    MechanicsSystem = property(_blcelc.iPcMechanicsObject_MechanicsSystem_get, _blcelc.iPcMechanicsObject_MechanicsSystem_set, None,
                    "iPcMechanicsObject.MechanicsSystem -> iPcMechanicsSystem*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMechanicsSystem* iPcMechanicsObject::GetMechanicsSystem()\n\tset: void iPcMechanicsObject::SetMechanicsSystem(iPcMechanicsSystem*)")


    Body = property(_blcelc.iPcMechanicsObject_Body_get, None, None,
                    "iPcMechanicsObject.Body -> iRigidBody*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iRigidBody* iPcMechanicsObject::GetBody()")

    __swig_setmethods__["Friction"] = _blcelc.iPcMechanicsObject_Friction_set
    Friction = property(_blcelc.iPcMechanicsObject_Friction_get, _blcelc.iPcMechanicsObject_Friction_set, None,
                    "iPcMechanicsObject.Friction -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMechanicsObject::GetFriction()\n\tset: void iPcMechanicsObject::SetFriction(float)")


    __swig_setmethods__["Mass"] = _blcelc.iPcMechanicsObject_Mass_set
    Mass = property(_blcelc.iPcMechanicsObject_Mass_get, _blcelc.iPcMechanicsObject_Mass_set, None,
                    "iPcMechanicsObject.Mass -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMechanicsObject::GetMass()\n\tset: void iPcMechanicsObject::SetMass(float)")


    __swig_setmethods__["Elasticity"] = _blcelc.iPcMechanicsObject_Elasticity_set
    Elasticity = property(_blcelc.iPcMechanicsObject_Elasticity_get, _blcelc.iPcMechanicsObject_Elasticity_set, None,
                    "iPcMechanicsObject.Elasticity -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMechanicsObject::GetElasticity()\n\tset: void iPcMechanicsObject::SetElasticity(float)")


    __swig_setmethods__["Density"] = _blcelc.iPcMechanicsObject_Density_set
    Density = property(_blcelc.iPcMechanicsObject_Density_get, _blcelc.iPcMechanicsObject_Density_set, None,
                    "iPcMechanicsObject.Density -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMechanicsObject::GetDensity()\n\tset: void iPcMechanicsObject::SetDensity(float)")


    __swig_setmethods__["Softness"] = _blcelc.iPcMechanicsObject_Softness_set
    Softness = property(_blcelc.iPcMechanicsObject_Softness_get, _blcelc.iPcMechanicsObject_Softness_set, None,
                    "iPcMechanicsObject.Softness -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMechanicsObject::GetSoftness()\n\tset: void iPcMechanicsObject::SetSoftness(float)")


    __swig_setmethods__["Lift"] = _blcelc.iPcMechanicsObject_Lift_set
    Lift = property(_blcelc.iPcMechanicsObject_Lift_get, _blcelc.iPcMechanicsObject_Lift_set, None,
                    "iPcMechanicsObject.Lift -> const csVector3&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csVector3& iPcMechanicsObject::GetLift()\n\tset: void iPcMechanicsObject::SetLift(const csVector3&)")


    __swig_setmethods__["Drag"] = _blcelc.iPcMechanicsObject_Drag_set
    Drag = property(_blcelc.iPcMechanicsObject_Drag_get, _blcelc.iPcMechanicsObject_Drag_set, None,
                    "iPcMechanicsObject.Drag -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMechanicsObject::GetDrag()\n\tset: void iPcMechanicsObject::SetDrag(float)")


    __swig_setmethods__["LinearVelocity"] = _blcelc.iPcMechanicsObject_LinearVelocity_set
    LinearVelocity = property(_blcelc.iPcMechanicsObject_LinearVelocity_get, _blcelc.iPcMechanicsObject_LinearVelocity_set, None,
                    "iPcMechanicsObject.LinearVelocity -> const csVector3\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csVector3 iPcMechanicsObject::GetLinearVelocity()\n\tset: void iPcMechanicsObject::SetLinearVelocity(const csVector3)")


    __swig_setmethods__["AngularVelocity"] = _blcelc.iPcMechanicsObject_AngularVelocity_set
    AngularVelocity = property(_blcelc.iPcMechanicsObject_AngularVelocity_get, _blcelc.iPcMechanicsObject_AngularVelocity_set, None,
                    "iPcMechanicsObject.AngularVelocity -> const csVector3\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csVector3 iPcMechanicsObject::GetAngularVelocity()\n\tset: void iPcMechanicsObject::SetAngularVelocity(const csVector3)")


    Static = property(_blcelc.iPcMechanicsObject_Static_get, None, None,
                    "iPcMechanicsObject.Static -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcMechanicsObject::IsStatic()")

    __swig_setmethods__["CollisionCallbackEnabled"] = _blcelc.iPcMechanicsObject_CollisionCallbackEnabled_set
    CollisionCallbackEnabled = property(_blcelc.iPcMechanicsObject_CollisionCallbackEnabled_get, _blcelc.iPcMechanicsObject_CollisionCallbackEnabled_set, None,
                    "iPcMechanicsObject.CollisionCallbackEnabled -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcMechanicsObject::IsCollisionCallbackEnabled()\n\tset: void iPcMechanicsObject::SetCollisionCallbackEnabled(bool)")


    __swig_destroy__ = _blcelc.delete_iPcMechanicsObject
    __del__ = lambda self : None;
iPcMechanicsObject_swigregister = _blcelc.iPcMechanicsObject_swigregister
iPcMechanicsObject_swigregister(iPcMechanicsObject)

class iPcMechanicsJoint(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsJoint, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsJoint, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetJoint(*args): return _blcelc.iPcMechanicsJoint_GetJoint(*args)
    Joint = property(_blcelc.iPcMechanicsJoint_Joint_get, None, None,
                    "iPcMechanicsJoint.Joint -> iJoint*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iJoint* iPcMechanicsJoint::GetJoint()")

    __swig_destroy__ = _blcelc.delete_iPcMechanicsJoint
    __del__ = lambda self : None;
iPcMechanicsJoint_swigregister = _blcelc.iPcMechanicsJoint_swigregister
iPcMechanicsJoint_swigregister(iPcMechanicsJoint)

celCreateMechanicsSystem = _blcelc.celCreateMechanicsSystem
celGetSetMechanicsSystem = _blcelc.celGetSetMechanicsSystem
celGetMechanicsSystem = _blcelc.celGetMechanicsSystem
scfQuery_iPcMechanicsSystem = _blcelc.scfQuery_iPcMechanicsSystem
scfQueryPC_iPcMechanicsSystem = _blcelc.scfQueryPC_iPcMechanicsSystem
celCreateMechanicsObject = _blcelc.celCreateMechanicsObject
celGetSetMechanicsObject = _blcelc.celGetSetMechanicsObject
celGetMechanicsObject = _blcelc.celGetMechanicsObject
scfQuery_iPcMechanicsObject = _blcelc.scfQuery_iPcMechanicsObject
scfQueryPC_iPcMechanicsObject = _blcelc.scfQueryPC_iPcMechanicsObject
scfQueryPC_iPcMechanicsJoint = _blcelc.scfQueryPC_iPcMechanicsJoint
celCreateMechanicsJoint = _blcelc.celCreateMechanicsJoint
celGetSetMechanicsJoint = _blcelc.celGetSetMechanicsJoint
celGetMechanicsJoint = _blcelc.celGetMechanicsJoint
scfQuery_iPcMechanicsJoint = _blcelc.scfQuery_iPcMechanicsJoint
class iPcMechanicsThruster(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsThruster, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsThruster, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    __swig_setmethods__["MechanicsObject"] = _blcelc.iPcMechanicsThruster_MechanicsObject_set
    MechanicsObject = property(_blcelc.iPcMechanicsThruster_MechanicsObject_get, _blcelc.iPcMechanicsThruster_MechanicsObject_set, None,
                    "iPcMechanicsThruster.MechanicsObject -> iPcMechanicsObject*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMechanicsObject* iPcMechanicsThruster::GetMechanicsObject()\n\tset: void iPcMechanicsThruster::SetMechanicsObject(iPcMechanicsObject*)")


    __swig_setmethods__["Position"] = _blcelc.iPcMechanicsThruster_Position_set
    Position = property(_blcelc.iPcMechanicsThruster_Position_get, _blcelc.iPcMechanicsThruster_Position_set, None,
                    "iPcMechanicsThruster.Position -> const csVector3&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csVector3& iPcMechanicsThruster::GetPosition()\n\tset: void iPcMechanicsThruster::SetPosition(const csVector3&)")


    __swig_setmethods__["Orientation"] = _blcelc.iPcMechanicsThruster_Orientation_set
    Orientation = property(_blcelc.iPcMechanicsThruster_Orientation_get, _blcelc.iPcMechanicsThruster_Orientation_set, None,
                    "iPcMechanicsThruster.Orientation -> const csVector3&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csVector3& iPcMechanicsThruster::GetOrientation()\n\tset: void iPcMechanicsThruster::SetOrientation(const csVector3&)")


    __swig_setmethods__["MaxThrust"] = _blcelc.iPcMechanicsThruster_MaxThrust_set
    MaxThrust = property(_blcelc.iPcMechanicsThruster_MaxThrust_get, _blcelc.iPcMechanicsThruster_MaxThrust_set, None,
                    "iPcMechanicsThruster.MaxThrust -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMechanicsThruster::GetMaxThrust()\n\tset: void iPcMechanicsThruster::SetMaxThrust(float)")


    __swig_destroy__ = _blcelc.delete_iPcMechanicsThruster
    __del__ = lambda self : None;
iPcMechanicsThruster_swigregister = _blcelc.iPcMechanicsThruster_swigregister
iPcMechanicsThruster_swigregister(iPcMechanicsThruster)

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
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetType(*args): return _blcelc.iPcMechanicsBalancedGroup_SetType(*args)
    def GetType(*args): return _blcelc.iPcMechanicsBalancedGroup_GetType(*args)
    def AddThruster(*args): return _blcelc.iPcMechanicsBalancedGroup_AddThruster(*args)
    def RemoveThruster(*args): return _blcelc.iPcMechanicsBalancedGroup_RemoveThruster(*args)
    def GetThruster(*args): return _blcelc.iPcMechanicsBalancedGroup_GetThruster(*args)
    def AvailableThrust(*args): return _blcelc.iPcMechanicsBalancedGroup_AvailableThrust(*args)
    def AvailableThrustForce(*args): return _blcelc.iPcMechanicsBalancedGroup_AvailableThrustForce(*args)
    def ChangeThrust(*args): return _blcelc.iPcMechanicsBalancedGroup_ChangeThrust(*args)
    __swig_setmethods__["Type"] = _blcelc.iPcMechanicsBalancedGroup_Type_set
    Type = property(_blcelc.iPcMechanicsBalancedGroup_Type_get, _blcelc.iPcMechanicsBalancedGroup_Type_set, None,
                    "iPcMechanicsBalancedGroup.Type -> celAxisType\n\nThis is equivalent to calling the C++ cs methods:\n\tget: celAxisType iPcMechanicsBalancedGroup::GetType()\n\tset: void iPcMechanicsBalancedGroup::SetType(celAxisType)")


    __swig_destroy__ = _blcelc.delete_iPcMechanicsBalancedGroup
    __del__ = lambda self : None;
iPcMechanicsBalancedGroup_swigregister = _blcelc.iPcMechanicsBalancedGroup_swigregister
iPcMechanicsBalancedGroup_swigregister(iPcMechanicsBalancedGroup)

class iPcMechanicsThrusterController(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsThrusterController, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsThrusterController, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    __swig_setmethods__["MechanicsObject"] = _blcelc.iPcMechanicsThrusterController_MechanicsObject_set
    MechanicsObject = property(_blcelc.iPcMechanicsThrusterController_MechanicsObject_get, _blcelc.iPcMechanicsThrusterController_MechanicsObject_set, None,
                    "iPcMechanicsThrusterController.MechanicsObject -> iPcMechanicsObject*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMechanicsObject* iPcMechanicsThrusterController::GetMechanicsObject()\n\tset: void iPcMechanicsThrusterController::SetMechanicsObject(iPcMechanicsObject*)")


    __swig_destroy__ = _blcelc.delete_iPcMechanicsThrusterController
    __del__ = lambda self : None;
iPcMechanicsThrusterController_swigregister = _blcelc.iPcMechanicsThrusterController_swigregister
iPcMechanicsThrusterController_swigregister(iPcMechanicsThrusterController)

celCreateMechanicsThrusterReactionary = _blcelc.celCreateMechanicsThrusterReactionary
celGetSetMechanicsThrusterReactionary = _blcelc.celGetSetMechanicsThrusterReactionary
celGetMechanicsThrusterReactionary = _blcelc.celGetMechanicsThrusterReactionary
scfQuery_iPcMechanicsThruster = _blcelc.scfQuery_iPcMechanicsThruster
scfQueryPC_iPcMechanicsThruster = _blcelc.scfQueryPC_iPcMechanicsThruster
celCreateMechanicsBalancedGroup = _blcelc.celCreateMechanicsBalancedGroup
celGetSetMechanicsBalancedGroup = _blcelc.celGetSetMechanicsBalancedGroup
celGetMechanicsBalancedGroup = _blcelc.celGetMechanicsBalancedGroup
scfQuery_iPcMechanicsBalancedGroup = _blcelc.scfQuery_iPcMechanicsBalancedGroup
scfQueryPC_iPcMechanicsBalancedGroup = _blcelc.scfQueryPC_iPcMechanicsBalancedGroup
celCreateMechanicsThrusterController = _blcelc.celCreateMechanicsThrusterController
celGetSetMechanicsThrusterController = _blcelc.celGetSetMechanicsThrusterController
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
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetOffset(*args): return _blcelc.iBillboardLayer_GetOffset(*args)
    def SetOffset(*args): return _blcelc.iBillboardLayer_SetOffset(*args)
    def Move(*args): return _blcelc.iBillboardLayer_Move(*args)
    def GetName(*args): return _blcelc.iBillboardLayer_GetName(*args)
    Name = property(_blcelc.iBillboardLayer_Name_get, None, None,
                    "iBillboardLayer.Name -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iBillboardLayer::GetName()")

    __swig_destroy__ = _blcelc.delete_iBillboardLayer
    __del__ = lambda self : None;
iBillboardLayer_swigregister = _blcelc.iBillboardLayer_swigregister
iBillboardLayer_swigregister(iBillboardLayer)

class iBillboardEventHandler(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboardEventHandler, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboardEventHandler, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def Select(*args): return _blcelc.iBillboardEventHandler_Select(*args)
    def MouseMove(*args): return _blcelc.iBillboardEventHandler_MouseMove(*args)
    def MouseMoveAway(*args): return _blcelc.iBillboardEventHandler_MouseMoveAway(*args)
    def Unselect(*args): return _blcelc.iBillboardEventHandler_Unselect(*args)
    def DoubleClick(*args): return _blcelc.iBillboardEventHandler_DoubleClick(*args)
    __swig_destroy__ = _blcelc.delete_iBillboardEventHandler
    __del__ = lambda self : None;
iBillboardEventHandler_swigregister = _blcelc.iBillboardEventHandler_swigregister
iBillboardEventHandler_swigregister(iBillboardEventHandler)

CEL_BILLBOARD_VISIBLE = _blcelc.CEL_BILLBOARD_VISIBLE
CEL_BILLBOARD_MOVABLE = _blcelc.CEL_BILLBOARD_MOVABLE
CEL_BILLBOARD_CLICKABLE = _blcelc.CEL_BILLBOARD_CLICKABLE
CEL_BILLBOARD_RESTACK = _blcelc.CEL_BILLBOARD_RESTACK
CEL_BILLBOARD_SENDMOVE = _blcelc.CEL_BILLBOARD_SENDMOVE
class iBillboard(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboard, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboard, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetName(*args): return _blcelc.iBillboard_GetName(*args)
    def GetFlags(*args): return _blcelc.iBillboard_GetFlags(*args)
    def SetMaterialName(*args): return _blcelc.iBillboard_SetMaterialName(*args)
    def SetMaterialNameFast(*args): return _blcelc.iBillboard_SetMaterialNameFast(*args)
    def GetMaterialName(*args): return _blcelc.iBillboard_GetMaterialName(*args)
    def DrawMesh(*args): return _blcelc.iBillboard_DrawMesh(*args)
    def SetSize(*args): return _blcelc.iBillboard_SetSize(*args)
    def GetSize(*args): return _blcelc.iBillboard_GetSize(*args)
    def GetImageSize(*args): return _blcelc.iBillboard_GetImageSize(*args)
    def SetPosition(*args): return _blcelc.iBillboard_SetPosition(*args)
    def MoveToPosition(*args): return _blcelc.iBillboard_MoveToPosition(*args)
    def GetPosition(*args): return _blcelc.iBillboard_GetPosition(*args)
    def Move(*args): return _blcelc.iBillboard_Move(*args)
    def SetRotation(*args): return _blcelc.iBillboard_SetRotation(*args)
    def GetRotation(*args): return _blcelc.iBillboard_GetRotation(*args)
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
    def SetText(*args): return _blcelc.iBillboard_SetText(*args)
    def GetText(*args): return _blcelc.iBillboard_GetText(*args)
    def GetTextDimensions(*args): return _blcelc.iBillboard_GetTextDimensions(*args)
    def SetTextOffset(*args): return _blcelc.iBillboard_SetTextOffset(*args)
    def GetTextOffsetDX(*args): return _blcelc.iBillboard_GetTextOffsetDX(*args)
    def GetTextOffsetDY(*args): return _blcelc.iBillboard_GetTextOffsetDY(*args)
    def SetTextFont(*args): return _blcelc.iBillboard_SetTextFont(*args)
    def GetTextFont(*args): return _blcelc.iBillboard_GetTextFont(*args)
    def GetTextFontSize(*args): return _blcelc.iBillboard_GetTextFontSize(*args)
    def SetTextFgColor(*args): return _blcelc.iBillboard_SetTextFgColor(*args)
    def GetTextFgColor(*args): return _blcelc.iBillboard_GetTextFgColor(*args)
    def ClearTextFgColor(*args): return _blcelc.iBillboard_ClearTextFgColor(*args)
    def SetTextBgColor(*args): return _blcelc.iBillboard_SetTextBgColor(*args)
    def GetTextBgColor(*args): return _blcelc.iBillboard_GetTextBgColor(*args)
    def SetTextBgTransparent(*args): return _blcelc.iBillboard_SetTextBgTransparent(*args)
    def IsTextBgTransparent(*args): return _blcelc.iBillboard_IsTextBgTransparent(*args)
    def ClearTextBgColor(*args): return _blcelc.iBillboard_ClearTextBgColor(*args)
    Name = property(_blcelc.iBillboard_Name_get, None, None,
                    "iBillboard.Name -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iBillboard::GetName()")

    Flags = property(_blcelc.iBillboard_Flags_get, None, None,
                    "iBillboard.Flags -> csFlags&  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: csFlags& iBillboard::GetFlags()")

    __swig_setmethods__["MaterialName"] = _blcelc.iBillboard_MaterialName_set
    MaterialName = property(_blcelc.iBillboard_MaterialName_get, _blcelc.iBillboard_MaterialName_set, None,
                    "iBillboard.MaterialName -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iBillboard::GetMaterialName()\n\tset: void iBillboard::SetMaterialName(const char*)")


    __swig_setmethods__["MaterialNameFast"] = _blcelc.iBillboard_MaterialNameFast_set
    MaterialNameFast = property(None, _blcelc.iBillboard_MaterialNameFast_set, None,
                    "iBillboard.MaterialNameFast (write only) -> const char*\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iBillboard::SetMaterialNameFast(const char*)")

    __swig_setmethods__["Rotation"] = _blcelc.iBillboard_Rotation_set
    Rotation = property(_blcelc.iBillboard_Rotation_get, _blcelc.iBillboard_Rotation_set, None,
                    "iBillboard.Rotation -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iBillboard::GetRotation()\n\tset: void iBillboard::SetRotation(float)")


    __swig_setmethods__["UVTopLeft"] = _blcelc.iBillboard_UVTopLeft_set
    UVTopLeft = property(_blcelc.iBillboard_UVTopLeft_get, _blcelc.iBillboard_UVTopLeft_set, None,
                    "iBillboard.UVTopLeft -> const csVector2&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csVector2& iBillboard::GetUVTopLeft()\n\tset: void iBillboard::SetUVTopLeft(const csVector2&)")


    __swig_setmethods__["UVBottomRight"] = _blcelc.iBillboard_UVBottomRight_set
    UVBottomRight = property(_blcelc.iBillboard_UVBottomRight_get, _blcelc.iBillboard_UVBottomRight_set, None,
                    "iBillboard.UVBottomRight -> const csVector2&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csVector2& iBillboard::GetUVBottomRight()\n\tset: void iBillboard::SetUVBottomRight(const csVector2&)")


    __swig_setmethods__["Color"] = _blcelc.iBillboard_Color_set
    Color = property(_blcelc.iBillboard_Color_get, _blcelc.iBillboard_Color_set, None,
                    "iBillboard.Color -> const csColor&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csColor& iBillboard::GetColor()\n\tset: void iBillboard::SetColor(const csColor&)")


    __swig_setmethods__["Layer"] = _blcelc.iBillboard_Layer_set
    Layer = property(_blcelc.iBillboard_Layer_get, _blcelc.iBillboard_Layer_set, None,
                    "iBillboard.Layer -> iBillboardLayer*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iBillboardLayer* iBillboard::GetLayer()\n\tset: void iBillboard::SetLayer(iBillboardLayer*)")


    __swig_setmethods__["Text"] = _blcelc.iBillboard_Text_set
    Text = property(_blcelc.iBillboard_Text_get, _blcelc.iBillboard_Text_set, None,
                    "iBillboard.Text -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iBillboard::GetText()\n\tset: void iBillboard::SetText(const char*)")


    __swig_setmethods__["TextOffset"] = fix_args(_blcelc.iBillboard_SetTextOffset)
    TextOffset = property(None, fix_args(_blcelc.iBillboard_SetTextOffset), None,
                    "iBillboard.TextOffset -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iBillboard::getmethod()\n\tset: void iBillboard::SetTextOffset(...)")

    TextOffsetDX = property(_blcelc.iBillboard_TextOffsetDX_get, None, None,
                    "iBillboard.TextOffsetDX -> int  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: int iBillboard::GetTextOffsetDX()")

    TextOffsetDY = property(_blcelc.iBillboard_TextOffsetDY_get, None, None,
                    "iBillboard.TextOffsetDY -> int  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: int iBillboard::GetTextOffsetDY()")

    __swig_setmethods__["TextFont"] = fix_args(_blcelc.iBillboard_SetTextFont)
    TextFont = property(_blcelc.iBillboard_GetTextFont, fix_args(_blcelc.iBillboard_SetTextFont), None,
                    "iBillboard.TextFont -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iBillboard::GetTextFont()\n\tset: void iBillboard::SetTextFont(...)")

    TextFontSize = property(_blcelc.iBillboard_TextFontSize_get, None, None,
                    "iBillboard.TextFontSize -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iBillboard::GetTextFontSize()")

    __swig_setmethods__["TextFgColor"] = _blcelc.iBillboard_TextFgColor_set
    TextFgColor = property(_blcelc.iBillboard_TextFgColor_get, _blcelc.iBillboard_TextFgColor_set, None,
                    "iBillboard.TextFgColor -> const csColor&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csColor& iBillboard::GetTextFgColor()\n\tset: void iBillboard::SetTextFgColor(const csColor&)")


    __swig_setmethods__["TextBgColor"] = _blcelc.iBillboard_TextBgColor_set
    TextBgColor = property(_blcelc.iBillboard_TextBgColor_get, _blcelc.iBillboard_TextBgColor_set, None,
                    "iBillboard.TextBgColor -> const csColor&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csColor& iBillboard::GetTextBgColor()\n\tset: void iBillboard::SetTextBgColor(const csColor&)")


    TextBgTransparent = property(_blcelc.iBillboard_TextBgTransparent_get, None, None,
                    "iBillboard.TextBgTransparent -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iBillboard::IsTextBgTransparent()")

    __swig_destroy__ = _blcelc.delete_iBillboard
    __del__ = lambda self : None;
iBillboard_swigregister = _blcelc.iBillboard_swigregister
iBillboard_swigregister(iBillboard)

class iBillboardManager(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboardManager, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboardManager, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def ScreenToBillboardX(*args): return _blcelc.iBillboardManager_ScreenToBillboardX(*args)
    def ScreenToBillboardY(*args): return _blcelc.iBillboardManager_ScreenToBillboardY(*args)
    def BillboardToScreenX(*args): return _blcelc.iBillboardManager_BillboardToScreenX(*args)
    def BillboardToScreenY(*args): return _blcelc.iBillboardManager_BillboardToScreenY(*args)
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
    def SetDefaultTextFont(*args): return _blcelc.iBillboardManager_SetDefaultTextFont(*args)
    def GetDefaultTextFont(*args): return _blcelc.iBillboardManager_GetDefaultTextFont(*args)
    def GetDefaultTextFontSize(*args): return _blcelc.iBillboardManager_GetDefaultTextFontSize(*args)
    def SetDefaultTextFgColor(*args): return _blcelc.iBillboardManager_SetDefaultTextFgColor(*args)
    def GetDefaultTextFgColor(*args): return _blcelc.iBillboardManager_GetDefaultTextFgColor(*args)
    def SetDefaultTextBgColor(*args): return _blcelc.iBillboardManager_SetDefaultTextBgColor(*args)
    def GetDefaultTextBgColor(*args): return _blcelc.iBillboardManager_GetDefaultTextBgColor(*args)
    def SetDefaultTextBgTransparent(*args): return _blcelc.iBillboardManager_SetDefaultTextBgTransparent(*args)
    def GetShowroom(*args): return _blcelc.iBillboardManager_GetShowroom(*args)
    BillboardCount = property(_blcelc.iBillboardManager_BillboardCount_get, None, None,
                    "iBillboardManager.BillboardCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iBillboardManager::GetBillboardCount()")

    BillboardLayerCount = property(_blcelc.iBillboardManager_BillboardLayerCount_get, None, None,
                    "iBillboardManager.BillboardLayerCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iBillboardManager::GetBillboardLayerCount()")

    __swig_setmethods__["Flags"] = fix_args(_blcelc.iBillboardManager_SetFlags)
    Flags = property(None, fix_args(_blcelc.iBillboardManager_SetFlags), None,
                    "iBillboardManager.Flags -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iBillboardManager::getmethod()\n\tset: void iBillboardManager::SetFlags(...)")

    __swig_setmethods__["DefaultTextFont"] = fix_args(_blcelc.iBillboardManager_SetDefaultTextFont)
    DefaultTextFont = property(_blcelc.iBillboardManager_GetDefaultTextFont, fix_args(_blcelc.iBillboardManager_SetDefaultTextFont), None,
                    "iBillboardManager.DefaultTextFont -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iBillboardManager::GetDefaultTextFont()\n\tset: void iBillboardManager::SetDefaultTextFont(...)")

    DefaultTextFontSize = property(_blcelc.iBillboardManager_DefaultTextFontSize_get, None, None,
                    "iBillboardManager.DefaultTextFontSize -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iBillboardManager::GetDefaultTextFontSize()")

    __swig_setmethods__["DefaultTextFgColor"] = _blcelc.iBillboardManager_DefaultTextFgColor_set
    DefaultTextFgColor = property(_blcelc.iBillboardManager_DefaultTextFgColor_get, _blcelc.iBillboardManager_DefaultTextFgColor_set, None,
                    "iBillboardManager.DefaultTextFgColor -> const csColor&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csColor& iBillboardManager::GetDefaultTextFgColor()\n\tset: void iBillboardManager::SetDefaultTextFgColor(const csColor&)")


    __swig_setmethods__["DefaultTextBgColor"] = _blcelc.iBillboardManager_DefaultTextBgColor_set
    DefaultTextBgColor = property(_blcelc.iBillboardManager_DefaultTextBgColor_get, _blcelc.iBillboardManager_DefaultTextBgColor_set, None,
                    "iBillboardManager.DefaultTextBgColor -> const csColor&\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const csColor& iBillboardManager::GetDefaultTextBgColor()\n\tset: void iBillboardManager::SetDefaultTextBgColor(const csColor&)")


    __swig_setmethods__["DefaultTextBgTransparent"] = fix_args(_blcelc.iBillboardManager_SetDefaultTextBgTransparent)
    DefaultTextBgTransparent = property(None, fix_args(_blcelc.iBillboardManager_SetDefaultTextBgTransparent), None,
                    "iBillboardManager.DefaultTextBgTransparent -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iBillboardManager::getmethod()\n\tset: void iBillboardManager::SetDefaultTextBgTransparent(...)")

    Showroom = property(_blcelc.iBillboardManager_Showroom_get, None, None,
                    "iBillboardManager.Showroom -> iSector*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iSector* iBillboardManager::GetShowroom()")

    __swig_destroy__ = _blcelc.delete_iBillboardManager
    __del__ = lambda self : None;
iBillboardManager_swigregister = _blcelc.iBillboardManager_swigregister
iBillboardManager_swigregister(iBillboardManager)

class iPcBillboard(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcBillboard, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcBillboard, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetBillboardName(*args): return _blcelc.iPcBillboard_SetBillboardName(*args)
    def GetBillboardName(*args): return _blcelc.iPcBillboard_GetBillboardName(*args)
    def GetBillboard(*args): return _blcelc.iPcBillboard_GetBillboard(*args)
    def EnableEvents(*args): return _blcelc.iPcBillboard_EnableEvents(*args)
    def AreEventsEnabled(*args): return _blcelc.iPcBillboard_AreEventsEnabled(*args)
    __swig_setmethods__["BillboardName"] = _blcelc.iPcBillboard_BillboardName_set
    BillboardName = property(_blcelc.iPcBillboard_BillboardName_get, _blcelc.iPcBillboard_BillboardName_set, None,
                    "iPcBillboard.BillboardName -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iPcBillboard::GetBillboardName()\n\tset: void iPcBillboard::SetBillboardName(const char*)")


    Billboard = property(_blcelc.iPcBillboard_Billboard_get, None, None,
                    "iPcBillboard.Billboard -> iBillboard*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iBillboard* iPcBillboard::GetBillboard()")

    __swig_destroy__ = _blcelc.delete_iPcBillboard
    __del__ = lambda self : None;
iPcBillboard_swigregister = _blcelc.iPcBillboard_swigregister
iPcBillboard_swigregister(iPcBillboard)

celCreateBillboard = _blcelc.celCreateBillboard
celGetSetBillboard = _blcelc.celGetSetBillboard
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
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    __swig_setmethods__["WorldFile"] = fix_args(_blcelc.iPcRegion_SetWorldFile)
    WorldFile = property(_blcelc.iPcRegion_GetWorldFile, fix_args(_blcelc.iPcRegion_SetWorldFile), None,
                    "iPcRegion.WorldFile -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcRegion::GetWorldFile()\n\tset: void iPcRegion::SetWorldFile(...)")

    WorldDir = property(_blcelc.iPcRegion_WorldDir_get, None, None,
                    "iPcRegion.WorldDir -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iPcRegion::GetWorldDir()")

    __swig_setmethods__["RegionName"] = _blcelc.iPcRegion_RegionName_set
    RegionName = property(_blcelc.iPcRegion_RegionName_get, _blcelc.iPcRegion_RegionName_set, None,
                    "iPcRegion.RegionName -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iPcRegion::GetRegionName()\n\tset: void iPcRegion::SetRegionName(const char*)")


    Region = property(_blcelc.iPcRegion_Region_get, None, None,
                    "iPcRegion.Region -> iRegion*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iRegion* iPcRegion::GetRegion()")

    def LoadWorld(*args): return _blcelc.iPcRegion_LoadWorld(*args)
    __swig_destroy__ = _blcelc.delete_iPcRegion
    __del__ = lambda self : None;
iPcRegion_swigregister = _blcelc.iPcRegion_swigregister
iPcRegion_swigregister(iPcRegion)

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
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetPath(*args): return _blcelc.iCelMapFile_SetPath(*args)
    def SetFile(*args): return _blcelc.iCelMapFile_SetFile(*args)
    def GetPath(*args): return _blcelc.iCelMapFile_GetPath(*args)
    def GetFile(*args): return _blcelc.iCelMapFile_GetFile(*args)
    def SetSectorName(*args): return _blcelc.iCelMapFile_SetSectorName(*args)
    def GetSectorName(*args): return _blcelc.iCelMapFile_GetSectorName(*args)
    __swig_setmethods__["Path"] = _blcelc.iCelMapFile_Path_set
    Path = property(_blcelc.iCelMapFile_Path_get, _blcelc.iCelMapFile_Path_set, None,
                    "iCelMapFile.Path -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iCelMapFile::GetPath()\n\tset: void iCelMapFile::SetPath(const char*)")


    __swig_setmethods__["File"] = _blcelc.iCelMapFile_File_set
    File = property(_blcelc.iCelMapFile_File_get, _blcelc.iCelMapFile_File_set, None,
                    "iCelMapFile.File -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iCelMapFile::GetFile()\n\tset: void iCelMapFile::SetFile(const char*)")


    __swig_setmethods__["SectorName"] = _blcelc.iCelMapFile_SectorName_set
    SectorName = property(_blcelc.iCelMapFile_SectorName_get, _blcelc.iCelMapFile_SectorName_set, None,
                    "iCelMapFile.SectorName -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iCelMapFile::GetSectorName()\n\tset: void iCelMapFile::SetSectorName(const char*)")


    __swig_destroy__ = _blcelc.delete_iCelMapFile
    __del__ = lambda self : None;
iCelMapFile_swigregister = _blcelc.iCelMapFile_swigregister
iCelMapFile_swigregister(iCelMapFile)

class iCelRegion(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelRegion, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelRegion, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetName(*args): return _blcelc.iCelRegion_GetName(*args)
    def GetCsRegionName(*args): return _blcelc.iCelRegion_GetCsRegionName(*args)
    def SetCachePath(*args): return _blcelc.iCelRegion_SetCachePath(*args)
    def GetCachePath(*args): return _blcelc.iCelRegion_GetCachePath(*args)
    def CreateMapFile(*args): return _blcelc.iCelRegion_CreateMapFile(*args)
    def GetMapFileCount(*args): return _blcelc.iCelRegion_GetMapFileCount(*args)
    def GetMapFile(*args): return _blcelc.iCelRegion_GetMapFile(*args)
    def RemoveMapFile(*args): return _blcelc.iCelRegion_RemoveMapFile(*args)
    def RemoveAllMapFiles(*args): return _blcelc.iCelRegion_RemoveAllMapFiles(*args)
    def AssociateEntity(*args): return _blcelc.iCelRegion_AssociateEntity(*args)
    def DissociateEntity(*args): return _blcelc.iCelRegion_DissociateEntity(*args)
    def ContainsEntity(*args): return _blcelc.iCelRegion_ContainsEntity(*args)
    def GetCsRegion(*args): return _blcelc.iCelRegion_GetCsRegion(*args)
    Name = property(_blcelc.iCelRegion_Name_get, None, None,
                    "iCelRegion.Name -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelRegion::GetName()")

    CsRegionName = property(_blcelc.iCelRegion_CsRegionName_get, None, None,
                    "iCelRegion.CsRegionName -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelRegion::GetCsRegionName()")

    __swig_setmethods__["CachePath"] = _blcelc.iCelRegion_CachePath_set
    CachePath = property(_blcelc.iCelRegion_CachePath_get, _blcelc.iCelRegion_CachePath_set, None,
                    "iCelRegion.CachePath -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iCelRegion::GetCachePath()\n\tset: void iCelRegion::SetCachePath(const char*)")


    MapFileCount = property(_blcelc.iCelRegion_MapFileCount_get, None, None,
                    "iCelRegion.MapFileCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelRegion::GetMapFileCount()")

    __swig_destroy__ = _blcelc.delete_iCelRegion
    __del__ = lambda self : None;
iCelRegion_swigregister = _blcelc.iCelRegion_swigregister
iCelRegion_swigregister(iCelRegion)

class iCelZone(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelZone, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelZone, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetName(*args): return _blcelc.iCelZone_GetName(*args)
    def LinkRegion(*args): return _blcelc.iCelZone_LinkRegion(*args)
    def GetRegionCount(*args): return _blcelc.iCelZone_GetRegionCount(*args)
    def GetRegion(*args): return _blcelc.iCelZone_GetRegion(*args)
    def FindRegion(*args): return _blcelc.iCelZone_FindRegion(*args)
    def UnlinkRegion(*args): return _blcelc.iCelZone_UnlinkRegion(*args)
    def UnlinkAllRegions(*args): return _blcelc.iCelZone_UnlinkAllRegions(*args)
    Name = property(_blcelc.iCelZone_Name_get, None, None,
                    "iCelZone.Name -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelZone::GetName()")

    RegionCount = property(_blcelc.iCelZone_RegionCount_get, None, None,
                    "iCelZone.RegionCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iCelZone::GetRegionCount()")

    __swig_destroy__ = _blcelc.delete_iCelZone
    __del__ = lambda self : None;
iCelZone_swigregister = _blcelc.iCelZone_swigregister
iCelZone_swigregister(iCelZone)

CEL_ZONEERROR_OK = _blcelc.CEL_ZONEERROR_OK
CEL_ZONEERROR_BADREGION = _blcelc.CEL_ZONEERROR_BADREGION
CEL_ZONEERROR_BADSTART = _blcelc.CEL_ZONEERROR_BADSTART
CEL_ZONEERROR_LOAD = _blcelc.CEL_ZONEERROR_LOAD
CEL_ZONE_NORMAL = _blcelc.CEL_ZONE_NORMAL
CEL_ZONE_KEEP = _blcelc.CEL_ZONE_KEEP
CEL_ZONE_LOADALL = _blcelc.CEL_ZONE_LOADALL
class iPcZoneManager(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcZoneManager, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcZoneManager, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def EnableColliderWrappers(*args): return _blcelc.iPcZoneManager_EnableColliderWrappers(*args)
    def IsColliderWrappers(*args): return _blcelc.iPcZoneManager_IsColliderWrappers(*args)
    def SetLoadingMode(*args): return _blcelc.iPcZoneManager_SetLoadingMode(*args)
    def GetLoadingMode(*args): return _blcelc.iPcZoneManager_GetLoadingMode(*args)
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
    def GetLastStartRegionName(*args): return _blcelc.iPcZoneManager_GetLastStartRegionName(*args)
    def GetLastStartName(*args): return _blcelc.iPcZoneManager_GetLastStartName(*args)
    def PointCamera(*args): return _blcelc.iPcZoneManager_PointCamera(*args)
    def PointMesh(*args): return _blcelc.iPcZoneManager_PointMesh(*args)
    def ActivateRegion(*args): return _blcelc.iPcZoneManager_ActivateRegion(*args)
    def FindRegionContaining(*args): return _blcelc.iPcZoneManager_FindRegionContaining(*args)
    ColliderWrappers = property(_blcelc.iPcZoneManager_ColliderWrappers_get, None, None,
                    "iPcZoneManager.ColliderWrappers -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcZoneManager::IsColliderWrappers()")

    __swig_setmethods__["LoadingMode"] = _blcelc.iPcZoneManager_LoadingMode_set
    LoadingMode = property(_blcelc.iPcZoneManager_LoadingMode_get, _blcelc.iPcZoneManager_LoadingMode_set, None,
                    "iPcZoneManager.LoadingMode -> int\n\nThis is equivalent to calling the C++ cs methods:\n\tget: int iPcZoneManager::GetLoadingMode()\n\tset: void iPcZoneManager::SetLoadingMode(int)")


    ZoneCount = property(_blcelc.iPcZoneManager_ZoneCount_get, None, None,
                    "iPcZoneManager.ZoneCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iPcZoneManager::GetZoneCount()")

    RegionCount = property(_blcelc.iPcZoneManager_RegionCount_get, None, None,
                    "iPcZoneManager.RegionCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iPcZoneManager::GetRegionCount()")

    LastStartRegionName = property(_blcelc.iPcZoneManager_LastStartRegionName_get, None, None,
                    "iPcZoneManager.LastStartRegionName -> const char *  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char * iPcZoneManager::GetLastStartRegionName()")

    LastStartName = property(_blcelc.iPcZoneManager_LastStartName_get, None, None,
                    "iPcZoneManager.LastStartName -> const char *  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char * iPcZoneManager::GetLastStartName()")

    __swig_destroy__ = _blcelc.delete_iPcZoneManager
    __del__ = lambda self : None;
iPcZoneManager_swigregister = _blcelc.iPcZoneManager_swigregister
iPcZoneManager_swigregister(iPcZoneManager)

celCreateZoneManager = _blcelc.celCreateZoneManager
celGetSetZoneManager = _blcelc.celGetSetZoneManager
celGetZoneManager = _blcelc.celGetZoneManager
scfQuery_iPcZoneManager = _blcelc.scfQuery_iPcZoneManager
CEL_KEY_STATE_UNUSED = _blcelc.CEL_KEY_STATE_UNUSED
CEL_KEY_STATE_UP = _blcelc.CEL_KEY_STATE_UP
CEL_KEY_STATE_DOWN = _blcelc.CEL_KEY_STATE_DOWN
CEL_KEY_STATE_REPEAT = _blcelc.CEL_KEY_STATE_REPEAT
class iPcCommandInput(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCommandInput, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCommandInput, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def Activate(*args): return _blcelc.iPcCommandInput_Activate(*args)
    def SetSendTrigger(*args): return _blcelc.iPcCommandInput_SetSendTrigger(*args)
    def IsSendTriggerEnabled(*args): return _blcelc.iPcCommandInput_IsSendTriggerEnabled(*args)
    def SetCookedMode(*args): return _blcelc.iPcCommandInput_SetCookedMode(*args)
    def GetCookedMode(*args): return _blcelc.iPcCommandInput_GetCookedMode(*args)
    def ScreenCoordinates(*args): return _blcelc.iPcCommandInput_ScreenCoordinates(*args)
    def ScreenToCentered(*args): return _blcelc.iPcCommandInput_ScreenToCentered(*args)
    def CenteredToScreen(*args): return _blcelc.iPcCommandInput_CenteredToScreen(*args)
    def LoadConfig(*args): return _blcelc.iPcCommandInput_LoadConfig(*args)
    def SaveConfig(*args): return _blcelc.iPcCommandInput_SaveConfig(*args)
    def Bind(*args): return _blcelc.iPcCommandInput_Bind(*args)
    def GetBind(*args): return _blcelc.iPcCommandInput_GetBind(*args)
    def RemoveBind(*args): return _blcelc.iPcCommandInput_RemoveBind(*args)
    def RemoveAllBinds(*args): return _blcelc.iPcCommandInput_RemoveAllBinds(*args)
    __swig_setmethods__["SendTrigger"] = _blcelc.iPcCommandInput_SendTrigger_set
    SendTrigger = property(None, _blcelc.iPcCommandInput_SendTrigger_set, None,
                    "iPcCommandInput.SendTrigger (write only) -> bool\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCommandInput::SetSendTrigger(bool)")

    SendTriggerEnabled = property(_blcelc.iPcCommandInput_SendTriggerEnabled_get, None, None,
                    "iPcCommandInput.SendTriggerEnabled -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcCommandInput::IsSendTriggerEnabled()")

    __swig_setmethods__["CookedMode"] = _blcelc.iPcCommandInput_CookedMode_set
    CookedMode = property(_blcelc.iPcCommandInput_CookedMode_get, _blcelc.iPcCommandInput_CookedMode_set, None,
                    "iPcCommandInput.CookedMode -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcCommandInput::GetCookedMode()\n\tset: void iPcCommandInput::SetCookedMode(bool)")


    __swig_destroy__ = _blcelc.delete_iPcCommandInput
    __del__ = lambda self : None;
iPcCommandInput_swigregister = _blcelc.iPcCommandInput_swigregister
iPcCommandInput_swigregister(iPcCommandInput)

celCreateCommandInput = _blcelc.celCreateCommandInput
celGetSetCommandInput = _blcelc.celGetSetCommandInput
celGetCommandInput = _blcelc.celGetCommandInput
scfQuery_iPcCommandInput = _blcelc.scfQuery_iPcCommandInput
class iPcGravityCallback(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcGravityCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcGravityCallback, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def Callback(*args): return _blcelc.iPcGravityCallback_Callback(*args)
    __swig_destroy__ = _blcelc.delete_iPcGravityCallback
    __del__ = lambda self : None;
iPcGravityCallback_swigregister = _blcelc.iPcGravityCallback_swigregister
iPcGravityCallback_swigregister(iPcGravityCallback)

class iPcLinearMovement(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcLinearMovement, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcLinearMovement, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    def SetHugGround(*args): return _blcelc.iPcLinearMovement_SetHugGround(*args)
    def SetDeltaLimit(*args): return _blcelc.iPcLinearMovement_SetDeltaLimit(*args)
    def RotateV(*args): return _blcelc.iPcLinearMovement_RotateV(*args)
    def SetGravity(*args): return _blcelc.iPcLinearMovement_SetGravity(*args)
    def GetGravity(*args): return _blcelc.iPcLinearMovement_GetGravity(*args)
    def ResetGravity(*args): return _blcelc.iPcLinearMovement_ResetGravity(*args)
    def AddGravityCallback(*args): return _blcelc.iPcLinearMovement_AddGravityCallback(*args)
    def RemoveGravityCallback(*args): return _blcelc.iPcLinearMovement_RemoveGravityCallback(*args)
    def GetPortalDisplacement(*args): return _blcelc.iPcLinearMovement_GetPortalDisplacement(*args)
    def ClearPortalDisplacement(*args): return _blcelc.iPcLinearMovement_ClearPortalDisplacement(*args)
    def SetFullPosition(*args): return _blcelc.iPcLinearMovement_SetFullPosition(*args)
    def SetPosition(*args): return _blcelc.iPcLinearMovement_SetPosition(*args)
    __swig_setmethods__["Anchor"] = _blcelc.iPcLinearMovement_Anchor_set
    Anchor = property(_blcelc.iPcLinearMovement_Anchor_get, _blcelc.iPcLinearMovement_Anchor_set, None,
                    "iPcLinearMovement.Anchor -> iPcMesh*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMesh* iPcLinearMovement::GetAnchor()\n\tset: void iPcLinearMovement::SetAnchor(iPcMesh*)")


    __swig_setmethods__["AngularVelocity"] = fix_args(_blcelc.iPcLinearMovement_SetAngularVelocity)
    AngularVelocity = property(_blcelc.iPcLinearMovement_GetAngularVelocity, fix_args(_blcelc.iPcLinearMovement_SetAngularVelocity), None,
                    "iPcLinearMovement.AngularVelocity -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcLinearMovement::GetAngularVelocity()\n\tset: void iPcLinearMovement::SetAngularVelocity(...)")

    __swig_setmethods__["Speed"] = _blcelc.iPcLinearMovement_Speed_set
    Speed = property(None, _blcelc.iPcLinearMovement_Speed_set, None,
                    "iPcLinearMovement.Speed (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcLinearMovement::SetSpeed(float)")

    __swig_setmethods__["SoftDRData"] = fix_args(_blcelc.iPcLinearMovement_SetSoftDRData)
    SoftDRData = property(None, fix_args(_blcelc.iPcLinearMovement_SetSoftDRData), None,
                    "iPcLinearMovement.SoftDRData -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcLinearMovement::getmethod()\n\tset: void iPcLinearMovement::SetSoftDRData(...)")

    __swig_setmethods__["FullPosition"] = fix_args(_blcelc.iPcLinearMovement_SetFullPosition)
    FullPosition = property(None, fix_args(_blcelc.iPcLinearMovement_SetFullPosition), None,
                    "iPcLinearMovement.FullPosition -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcLinearMovement::getmethod()\n\tset: void iPcLinearMovement::SetFullPosition(...)")

    __swig_setmethods__["Position"] = fix_args(_blcelc.iPcLinearMovement_SetPosition)
    Position = property(None, fix_args(_blcelc.iPcLinearMovement_SetPosition), None,
                    "iPcLinearMovement.Position -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcLinearMovement::getmethod()\n\tset: void iPcLinearMovement::SetPosition(...)")

    __swig_setmethods__["Path"] = fix_args(_blcelc.iPcLinearMovement_SetPath)
    Path = property(_blcelc.iPcLinearMovement_IsPath, fix_args(_blcelc.iPcLinearMovement_SetPath), None,
                    "iPcLinearMovement.Path -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcLinearMovement::IsPath()\n\tset: void iPcLinearMovement::SetPath(...)")

    Sector = property(_blcelc.iPcLinearMovement_Sector_get, None, None,
                    "iPcLinearMovement.Sector -> iSector*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iSector* iPcLinearMovement::GetSector()")

    __swig_setmethods__["PathTime"] = _blcelc.iPcLinearMovement_PathTime_set
    PathTime = property(None, _blcelc.iPcLinearMovement_PathTime_set, None,
                    "iPcLinearMovement.PathTime (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcLinearMovement::SetPathTime(float)")

    __swig_setmethods__["PathSpeed"] = _blcelc.iPcLinearMovement_PathSpeed_set
    PathSpeed = property(None, _blcelc.iPcLinearMovement_PathSpeed_set, None,
                    "iPcLinearMovement.PathSpeed (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcLinearMovement::SetPathSpeed(float)")

    __swig_setmethods__["PathAction"] = fix_args(_blcelc.iPcLinearMovement_SetPathAction)
    PathAction = property(None, fix_args(_blcelc.iPcLinearMovement_SetPathAction), None,
                    "iPcLinearMovement.PathAction -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcLinearMovement::getmethod()\n\tset: void iPcLinearMovement::SetPathAction(...)")

    __swig_setmethods__["PathSector"] = _blcelc.iPcLinearMovement_PathSector_set
    PathSector = property(None, _blcelc.iPcLinearMovement_PathSector_set, None,
                    "iPcLinearMovement.PathSector (write only) -> const char *\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcLinearMovement::SetPathSector(const char *)")

    __swig_setmethods__["OnGround"] = _blcelc.iPcLinearMovement_OnGround_set
    OnGround = property(_blcelc.iPcLinearMovement_OnGround_get, _blcelc.iPcLinearMovement_OnGround_set, None,
                    "iPcLinearMovement.OnGround -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcLinearMovement::IsOnGround()\n\tset: void iPcLinearMovement::SetOnGround(bool)")


    __swig_setmethods__["HugGround"] = _blcelc.iPcLinearMovement_HugGround_set
    HugGround = property(None, _blcelc.iPcLinearMovement_HugGround_set, None,
                    "iPcLinearMovement.HugGround (write only) -> bool\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcLinearMovement::SetHugGround(bool)")

    __swig_setmethods__["DeltaLimit"] = _blcelc.iPcLinearMovement_DeltaLimit_set
    DeltaLimit = property(None, _blcelc.iPcLinearMovement_DeltaLimit_set, None,
                    "iPcLinearMovement.DeltaLimit (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcLinearMovement::SetDeltaLimit(float)")

    __swig_setmethods__["Gravity"] = _blcelc.iPcLinearMovement_Gravity_set
    Gravity = property(_blcelc.iPcLinearMovement_Gravity_get, _blcelc.iPcLinearMovement_Gravity_set, None,
                    "iPcLinearMovement.Gravity -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcLinearMovement::GetGravity()\n\tset: void iPcLinearMovement::SetGravity(float)")


    PortalDisplacement = property(_blcelc.iPcLinearMovement_PortalDisplacement_get, None, None,
                    "iPcLinearMovement.PortalDisplacement -> csVector3  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: csVector3 iPcLinearMovement::GetPortalDisplacement()")

    __swig_destroy__ = _blcelc.delete_iPcLinearMovement
    __del__ = lambda self : None;
iPcLinearMovement_swigregister = _blcelc.iPcLinearMovement_swigregister
iPcLinearMovement_swigregister(iPcLinearMovement)

celCreateLinearMovement = _blcelc.celCreateLinearMovement
celGetSetLinearMovement = _blcelc.celGetSetLinearMovement
celGetLinearMovement = _blcelc.celGetLinearMovement
scfQuery_iPcLinearMovement = _blcelc.scfQuery_iPcLinearMovement
class iPcActorMove(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcActorMove, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcActorMove, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    def MouseMove(*args): return _blcelc.iPcActorMove_MouseMove(*args)
    def Jump(*args): return _blcelc.iPcActorMove_Jump(*args)
    def ToggleCameraMode(*args): return _blcelc.iPcActorMove_ToggleCameraMode(*args)
    def EnableMouseMove(*args): return _blcelc.iPcActorMove_EnableMouseMove(*args)
    def IsMouseMoveEnabled(*args): return _blcelc.iPcActorMove_IsMouseMoveEnabled(*args)
    def SetMouseMoveAccelerated(*args): return _blcelc.iPcActorMove_SetMouseMoveAccelerated(*args)
    def IsMouseMoveAccelerated(*args): return _blcelc.iPcActorMove_IsMouseMoveAccelerated(*args)
    def SetMouseMoveInverted(*args): return _blcelc.iPcActorMove_SetMouseMoveInverted(*args)
    def IsMouseMoveInverted(*args): return _blcelc.iPcActorMove_IsMouseMoveInverted(*args)
    def SetMouseMoveSpeed(*args): return _blcelc.iPcActorMove_SetMouseMoveSpeed(*args)
    def GetMouseMoveSpeedX(*args): return _blcelc.iPcActorMove_GetMouseMoveSpeedX(*args)
    def GetMouseMoveSpeedY(*args): return _blcelc.iPcActorMove_GetMouseMoveSpeedY(*args)
    def SetMovementSpeed(*args): return _blcelc.iPcActorMove_SetMovementSpeed(*args)
    def GetMovementSpeed(*args): return _blcelc.iPcActorMove_GetMovementSpeed(*args)
    def SetRunningSpeed(*args): return _blcelc.iPcActorMove_SetRunningSpeed(*args)
    def GetRunningSpeed(*args): return _blcelc.iPcActorMove_GetRunningSpeed(*args)
    def SetRotationSpeed(*args): return _blcelc.iPcActorMove_SetRotationSpeed(*args)
    def GetRotationSpeed(*args): return _blcelc.iPcActorMove_GetRotationSpeed(*args)
    def SetJumpingVelocity(*args): return _blcelc.iPcActorMove_SetJumpingVelocity(*args)
    def GetJumpingVelocity(*args): return _blcelc.iPcActorMove_GetJumpingVelocity(*args)
    def SetAnimation(*args): return _blcelc.iPcActorMove_SetAnimation(*args)
    __swig_setmethods__["MovingForward"] = _blcelc.iPcActorMove_MovingForward_set
    MovingForward = property(_blcelc.iPcActorMove_MovingForward_get, _blcelc.iPcActorMove_MovingForward_set, None,
                    "iPcActorMove.MovingForward -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcActorMove::IsMovingForward()\n\tset: void iPcActorMove::Forward(bool)")


    __swig_setmethods__["MovingBackward"] = _blcelc.iPcActorMove_MovingBackward_set
    MovingBackward = property(_blcelc.iPcActorMove_MovingBackward_get, _blcelc.iPcActorMove_MovingBackward_set, None,
                    "iPcActorMove.MovingBackward -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcActorMove::IsMovingBackward()\n\tset: void iPcActorMove::Backward(bool)")


    __swig_setmethods__["StrafingLeft"] = _blcelc.iPcActorMove_StrafingLeft_set
    StrafingLeft = property(_blcelc.iPcActorMove_StrafingLeft_get, _blcelc.iPcActorMove_StrafingLeft_set, None,
                    "iPcActorMove.StrafingLeft -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcActorMove::IsStrafingLeft()\n\tset: void iPcActorMove::StrafeLeft(bool)")


    __swig_setmethods__["StrafingRight"] = _blcelc.iPcActorMove_StrafingRight_set
    StrafingRight = property(_blcelc.iPcActorMove_StrafingRight_get, _blcelc.iPcActorMove_StrafingRight_set, None,
                    "iPcActorMove.StrafingRight -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcActorMove::IsStrafingRight()\n\tset: void iPcActorMove::StrafeRight(bool)")


    __swig_setmethods__["RotatingLeft"] = _blcelc.iPcActorMove_RotatingLeft_set
    RotatingLeft = property(_blcelc.iPcActorMove_RotatingLeft_get, _blcelc.iPcActorMove_RotatingLeft_set, None,
                    "iPcActorMove.RotatingLeft -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcActorMove::IsRotatingLeft()\n\tset: void iPcActorMove::RotateLeft(bool)")


    __swig_setmethods__["RotatingRight"] = _blcelc.iPcActorMove_RotatingRight_set
    RotatingRight = property(_blcelc.iPcActorMove_RotatingRight_get, _blcelc.iPcActorMove_RotatingRight_set, None,
                    "iPcActorMove.RotatingRight -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcActorMove::IsRotatingRight()\n\tset: void iPcActorMove::RotateRight(bool)")


    __swig_setmethods__["Running"] = _blcelc.iPcActorMove_Running_set
    Running = property(_blcelc.iPcActorMove_Running_get, _blcelc.iPcActorMove_Running_set, None,
                    "iPcActorMove.Running -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcActorMove::IsRunning()\n\tset: void iPcActorMove::Run(bool)")


    __swig_setmethods__["AutoRunning"] = _blcelc.iPcActorMove_AutoRunning_set
    AutoRunning = property(_blcelc.iPcActorMove_AutoRunning_get, _blcelc.iPcActorMove_AutoRunning_set, None,
                    "iPcActorMove.AutoRunning -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcActorMove::IsAutoRunning()\n\tset: void iPcActorMove::AutoRun(bool)")


    MouseMoveEnabled = property(_blcelc.iPcActorMove_MouseMoveEnabled_get, None, None,
                    "iPcActorMove.MouseMoveEnabled -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcActorMove::IsMouseMoveEnabled()")

    __swig_setmethods__["MouseMoveInverted"] = _blcelc.iPcActorMove_MouseMoveInverted_set
    MouseMoveInverted = property(_blcelc.iPcActorMove_MouseMoveInverted_get, _blcelc.iPcActorMove_MouseMoveInverted_set, None,
                    "iPcActorMove.MouseMoveInverted -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcActorMove::IsMouseMoveInverted()\n\tset: void iPcActorMove::SetMouseMoveInverted(bool)")


    __swig_setmethods__["MouseMoveSpeed"] = fix_args(_blcelc.iPcActorMove_SetMouseMoveSpeed)
    MouseMoveSpeed = property(None, fix_args(_blcelc.iPcActorMove_SetMouseMoveSpeed), None,
                    "iPcActorMove.MouseMoveSpeed -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcActorMove::getmethod()\n\tset: void iPcActorMove::SetMouseMoveSpeed(...)")

    MouseMoveSpeedX = property(_blcelc.iPcActorMove_MouseMoveSpeedX_get, None, None,
                    "iPcActorMove.MouseMoveSpeedX -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iPcActorMove::GetMouseMoveSpeedX()")

    MouseMoveSpeedY = property(_blcelc.iPcActorMove_MouseMoveSpeedY_get, None, None,
                    "iPcActorMove.MouseMoveSpeedY -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iPcActorMove::GetMouseMoveSpeedY()")

    __swig_setmethods__["MovementSpeed"] = _blcelc.iPcActorMove_MovementSpeed_set
    MovementSpeed = property(_blcelc.iPcActorMove_MovementSpeed_get, _blcelc.iPcActorMove_MovementSpeed_set, None,
                    "iPcActorMove.MovementSpeed -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcActorMove::GetMovementSpeed()\n\tset: void iPcActorMove::SetMovementSpeed(float)")


    __swig_setmethods__["RunningSpeed"] = _blcelc.iPcActorMove_RunningSpeed_set
    RunningSpeed = property(_blcelc.iPcActorMove_RunningSpeed_get, _blcelc.iPcActorMove_RunningSpeed_set, None,
                    "iPcActorMove.RunningSpeed -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcActorMove::GetRunningSpeed()\n\tset: void iPcActorMove::SetRunningSpeed(float)")


    __swig_setmethods__["RotationSpeed"] = _blcelc.iPcActorMove_RotationSpeed_set
    RotationSpeed = property(_blcelc.iPcActorMove_RotationSpeed_get, _blcelc.iPcActorMove_RotationSpeed_set, None,
                    "iPcActorMove.RotationSpeed -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcActorMove::GetRotationSpeed()\n\tset: void iPcActorMove::SetRotationSpeed(float)")


    __swig_setmethods__["JumpingVelocity"] = _blcelc.iPcActorMove_JumpingVelocity_set
    JumpingVelocity = property(_blcelc.iPcActorMove_JumpingVelocity_get, _blcelc.iPcActorMove_JumpingVelocity_set, None,
                    "iPcActorMove.JumpingVelocity -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcActorMove::GetJumpingVelocity()\n\tset: void iPcActorMove::SetJumpingVelocity(float)")


    __swig_setmethods__["Animation"] = fix_args(_blcelc.iPcActorMove_SetAnimation)
    Animation = property(None, fix_args(_blcelc.iPcActorMove_SetAnimation), None,
                    "iPcActorMove.Animation -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcActorMove::getmethod()\n\tset: void iPcActorMove::SetAnimation(...)")

    __swig_destroy__ = _blcelc.delete_iPcActorMove
    __del__ = lambda self : None;
iPcActorMove_swigregister = _blcelc.iPcActorMove_swigregister
iPcActorMove_swigregister(iPcActorMove)

celCreateActorMove = _blcelc.celCreateActorMove
celGetSetActorMove = _blcelc.celGetSetActorMove
celGetActorMove = _blcelc.celGetActorMove
scfQuery_iPcActorMove = _blcelc.scfQuery_iPcActorMove
class iPcCamera(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCamera, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    def UpdateCamera(*args): return _blcelc.iPcCamera_UpdateCamera(*args)
    def GetDrawFlags(*args): return _blcelc.iPcCamera_GetDrawFlags(*args)
    __swig_setmethods__["Region"] = fix_args(_blcelc.iPcCamera_SetRegion)
    Region = property(None, fix_args(_blcelc.iPcCamera_SetRegion), None,
                    "iPcCamera.Region -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcCamera::getmethod()\n\tset: void iPcCamera::SetRegion(...)")

    __swig_setmethods__["ZoneManager"] = fix_args(_blcelc.iPcCamera_SetZoneManager)
    ZoneManager = property(None, fix_args(_blcelc.iPcCamera_SetZoneManager), None,
                    "iPcCamera.ZoneManager -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcCamera::getmethod()\n\tset: void iPcCamera::SetZoneManager(...)")

    __swig_setmethods__["Rectangle"] = fix_args(_blcelc.iPcCamera_SetRectangle)
    Rectangle = property(None, fix_args(_blcelc.iPcCamera_SetRectangle), None,
                    "iPcCamera.Rectangle -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcCamera::getmethod()\n\tset: void iPcCamera::SetRectangle(...)")

    Camera = property(_blcelc.iPcCamera_Camera_get, None, None,
                    "iPcCamera.Camera -> iCamera*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iCamera* iPcCamera::GetCamera()")

    View = property(_blcelc.iPcCamera_View_get, None, None,
                    "iPcCamera.View -> iView*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iView* iPcCamera::GetView()")

    __swig_setmethods__["ClearZBuffer"] = _blcelc.iPcCamera_ClearZBuffer_set
    ClearZBuffer = property(_blcelc.iPcCamera_ClearZBuffer_get, _blcelc.iPcCamera_ClearZBuffer_set, None,
                    "iPcCamera.ClearZBuffer -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcCamera::GetClearZBuffer()\n\tset: void iPcCamera::SetClearZBuffer(bool)")


    __swig_setmethods__["ClearScreen"] = _blcelc.iPcCamera_ClearScreen_set
    ClearScreen = property(_blcelc.iPcCamera_ClearScreen_get, _blcelc.iPcCamera_ClearScreen_set, None,
                    "iPcCamera.ClearScreen -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcCamera::GetClearScreen()\n\tset: void iPcCamera::SetClearScreen(bool)")


    FixedDistance = property(_blcelc.iPcCamera_FixedDistance_get, None, None,
                    "iPcCamera.FixedDistance -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iPcCamera::GetFixedDistance()")

    AdaptiveMinFPS = property(_blcelc.iPcCamera_AdaptiveMinFPS_get, None, None,
                    "iPcCamera.AdaptiveMinFPS -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iPcCamera::GetAdaptiveMinFPS()")

    AdaptiveMaxFPS = property(_blcelc.iPcCamera_AdaptiveMaxFPS_get, None, None,
                    "iPcCamera.AdaptiveMaxFPS -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iPcCamera::GetAdaptiveMaxFPS()")

    AdaptiveMinDistance = property(_blcelc.iPcCamera_AdaptiveMinDistance_get, None, None,
                    "iPcCamera.AdaptiveMinDistance -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iPcCamera::GetAdaptiveMinDistance()")

    __swig_setmethods__["AutoDraw"] = _blcelc.iPcCamera_AutoDraw_set
    AutoDraw = property(None, _blcelc.iPcCamera_AutoDraw_set, None,
                    "iPcCamera.AutoDraw (write only) -> bool\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCamera::SetAutoDraw(bool)")

    DrawFlags = property(_blcelc.iPcCamera_DrawFlags_get, None, None,
                    "iPcCamera.DrawFlags -> int  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: int iPcCamera::GetDrawFlags()")

    __swig_destroy__ = _blcelc.delete_iPcCamera
    __del__ = lambda self : None;
iPcCamera_swigregister = _blcelc.iPcCamera_swigregister
iPcCamera_swigregister(iPcCamera)

celCreateCamera = _blcelc.celCreateCamera
celGetSetCamera = _blcelc.celGetSetCamera
celGetCamera = _blcelc.celGetCamera
scfQuery_iPcCamera = _blcelc.scfQuery_iPcCamera
class iPcDefaultCamera(iPcCamera):
    __swig_setmethods__ = {}
    for _s in [iPcCamera]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcDefaultCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [iPcCamera]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcDefaultCamera, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    def SetFollowEntity(*args): return _blcelc.iPcDefaultCamera_SetFollowEntity(*args)
    def SetMode(*args): return _blcelc.iPcDefaultCamera_SetMode(*args)
    def GetMode(*args): return _blcelc.iPcDefaultCamera_GetMode(*args)
    def SetModeName(*args): return _blcelc.iPcDefaultCamera_SetModeName(*args)
    def GetModeName(*args): return _blcelc.iPcDefaultCamera_GetModeName(*args)
    def GetNextMode(*args): return _blcelc.iPcDefaultCamera_GetNextMode(*args)
    def PointCamera(*args): return _blcelc.iPcDefaultCamera_PointCamera(*args)
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
    def GetDistance(*args): return _blcelc.iPcDefaultCamera_GetDistance(*args)
    def SetDistanceVelocity(*args): return _blcelc.iPcDefaultCamera_SetDistanceVelocity(*args)
    def GetDistanceVelocity(*args): return _blcelc.iPcDefaultCamera_GetDistanceVelocity(*args)
    __swig_setmethods__["FollowEntity"] = _blcelc.iPcDefaultCamera_FollowEntity_set
    FollowEntity = property(None, _blcelc.iPcDefaultCamera_FollowEntity_set, None,
                    "iPcDefaultCamera.FollowEntity (write only) -> iCelEntity*\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcDefaultCamera::SetFollowEntity(iCelEntity*)")

    __swig_setmethods__["Mode"] = _blcelc.iPcDefaultCamera_Mode_set
    Mode = property(_blcelc.iPcDefaultCamera_Mode_get, _blcelc.iPcDefaultCamera_Mode_set, None,
                    "iPcDefaultCamera.Mode -> iPcDefaultCamera::CameraMode\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcDefaultCamera::CameraMode iPcDefaultCamera::GetMode()\n\tset: void iPcDefaultCamera::SetMode(iPcDefaultCamera::CameraMode)")


    __swig_setmethods__["ModeName"] = _blcelc.iPcDefaultCamera_ModeName_set
    ModeName = property(_blcelc.iPcDefaultCamera_ModeName_get, _blcelc.iPcDefaultCamera_ModeName_set, None,
                    "iPcDefaultCamera.ModeName -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iPcDefaultCamera::GetModeName()\n\tset: void iPcDefaultCamera::SetModeName(const char*)")


    NextMode = property(_blcelc.iPcDefaultCamera_NextMode_get, None, None,
                    "iPcDefaultCamera.NextMode -> iPcDefaultCamera::CameraMode  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iPcDefaultCamera::CameraMode iPcDefaultCamera::GetNextMode()")

    __swig_setmethods__["SpringParameters"] = fix_args(_blcelc.iPcDefaultCamera_SetSpringParameters)
    SpringParameters = property(None, fix_args(_blcelc.iPcDefaultCamera_SetSpringParameters), None,
                    "iPcDefaultCamera.SpringParameters -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcDefaultCamera::getmethod()\n\tset: void iPcDefaultCamera::SetSpringParameters(...)")

    __swig_setmethods__["MinMaxCameraDistance"] = fix_args(_blcelc.iPcDefaultCamera_SetMinMaxCameraDistance)
    MinMaxCameraDistance = property(None, fix_args(_blcelc.iPcDefaultCamera_SetMinMaxCameraDistance), None,
                    "iPcDefaultCamera.MinMaxCameraDistance -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcDefaultCamera::getmethod()\n\tset: void iPcDefaultCamera::SetMinMaxCameraDistance(...)")

    __swig_setmethods__["TurnSpeed"] = _blcelc.iPcDefaultCamera_TurnSpeed_set
    TurnSpeed = property(None, _blcelc.iPcDefaultCamera_TurnSpeed_set, None,
                    "iPcDefaultCamera.TurnSpeed (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcDefaultCamera::SetTurnSpeed(float)")

    __swig_setmethods__["SwingCoef"] = _blcelc.iPcDefaultCamera_SwingCoef_set
    SwingCoef = property(None, _blcelc.iPcDefaultCamera_SwingCoef_set, None,
                    "iPcDefaultCamera.SwingCoef (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcDefaultCamera::SetSwingCoef(float)")

    __swig_setmethods__["FirstPersonOffset"] = _blcelc.iPcDefaultCamera_FirstPersonOffset_set
    FirstPersonOffset = property(None, _blcelc.iPcDefaultCamera_FirstPersonOffset_set, None,
                    "iPcDefaultCamera.FirstPersonOffset (write only) -> const csVector3&\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcDefaultCamera::SetFirstPersonOffset(const csVector3&)")

    __swig_setmethods__["ThirdPersonOffset"] = _blcelc.iPcDefaultCamera_ThirdPersonOffset_set
    ThirdPersonOffset = property(None, _blcelc.iPcDefaultCamera_ThirdPersonOffset_set, None,
                    "iPcDefaultCamera.ThirdPersonOffset (write only) -> const csVector3&\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcDefaultCamera::SetThirdPersonOffset(const csVector3&)")

    __swig_setmethods__["Pitch"] = _blcelc.iPcDefaultCamera_Pitch_set
    Pitch = property(_blcelc.iPcDefaultCamera_Pitch_get, _blcelc.iPcDefaultCamera_Pitch_set, None,
                    "iPcDefaultCamera.Pitch -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcDefaultCamera::GetPitch()\n\tset: void iPcDefaultCamera::SetPitch(float)")


    __swig_setmethods__["PitchVelocity"] = _blcelc.iPcDefaultCamera_PitchVelocity_set
    PitchVelocity = property(_blcelc.iPcDefaultCamera_PitchVelocity_get, _blcelc.iPcDefaultCamera_PitchVelocity_set, None,
                    "iPcDefaultCamera.PitchVelocity -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcDefaultCamera::GetPitchVelocity()\n\tset: void iPcDefaultCamera::SetPitchVelocity(float)")


    __swig_setmethods__["YawVelocity"] = _blcelc.iPcDefaultCamera_YawVelocity_set
    YawVelocity = property(_blcelc.iPcDefaultCamera_YawVelocity_get, _blcelc.iPcDefaultCamera_YawVelocity_set, None,
                    "iPcDefaultCamera.YawVelocity -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcDefaultCamera::GetYawVelocity()\n\tset: void iPcDefaultCamera::SetYawVelocity(float)")


    __swig_setmethods__["DistanceVelocity"] = _blcelc.iPcDefaultCamera_DistanceVelocity_set
    DistanceVelocity = property(_blcelc.iPcDefaultCamera_DistanceVelocity_get, _blcelc.iPcDefaultCamera_DistanceVelocity_set, None,
                    "iPcDefaultCamera.DistanceVelocity -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcDefaultCamera::GetDistanceVelocity()\n\tset: void iPcDefaultCamera::SetDistanceVelocity(float)")


    __swig_destroy__ = _blcelc.delete_iPcDefaultCamera
    __del__ = lambda self : None;
iPcDefaultCamera_swigregister = _blcelc.iPcDefaultCamera_swigregister
iPcDefaultCamera_swigregister(iPcDefaultCamera)

celCreateDefaultCamera = _blcelc.celCreateDefaultCamera
celGetSetDefaultCamera = _blcelc.celGetSetDefaultCamera
celGetDefaultCamera = _blcelc.celGetDefaultCamera
scfQuery_iPcDefaultCamera = _blcelc.scfQuery_iPcDefaultCamera
class iPcSimpleCamera(iPcCamera):
    __swig_setmethods__ = {}
    for _s in [iPcCamera]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcSimpleCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [iPcCamera]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcSimpleCamera, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetDrawMesh(*args): return _blcelc.iPcSimpleCamera_SetDrawMesh(*args)
    def SetCameraOffset(*args): return _blcelc.iPcSimpleCamera_SetCameraOffset(*args)
    def SetLookAtOffset(*args): return _blcelc.iPcSimpleCamera_SetLookAtOffset(*args)
    def SetMesh(*args): return _blcelc.iPcSimpleCamera_SetMesh(*args)
    def Draw(*args): return _blcelc.iPcSimpleCamera_Draw(*args)
    __swig_setmethods__["DrawMesh"] = _blcelc.iPcSimpleCamera_DrawMesh_set
    DrawMesh = property(None, _blcelc.iPcSimpleCamera_DrawMesh_set, None,
                    "iPcSimpleCamera.DrawMesh (write only) -> bool\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcSimpleCamera::SetDrawMesh(bool)")

    __swig_setmethods__["CameraOffset"] = fix_args(_blcelc.iPcSimpleCamera_SetCameraOffset)
    CameraOffset = property(None, fix_args(_blcelc.iPcSimpleCamera_SetCameraOffset), None,
                    "iPcSimpleCamera.CameraOffset -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcSimpleCamera::getmethod()\n\tset: void iPcSimpleCamera::SetCameraOffset(...)")

    __swig_setmethods__["LookAtOffset"] = _blcelc.iPcSimpleCamera_LookAtOffset_set
    LookAtOffset = property(None, _blcelc.iPcSimpleCamera_LookAtOffset_set, None,
                    "iPcSimpleCamera.LookAtOffset (write only) -> const csVector3&\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcSimpleCamera::SetLookAtOffset(const csVector3&)")

    __swig_setmethods__["Mesh"] = _blcelc.iPcSimpleCamera_Mesh_set
    Mesh = property(None, _blcelc.iPcSimpleCamera_Mesh_set, None,
                    "iPcSimpleCamera.Mesh (write only) -> iPcMesh*\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcSimpleCamera::SetMesh(iPcMesh*)")

    __swig_destroy__ = _blcelc.delete_iPcSimpleCamera
    __del__ = lambda self : None;
iPcSimpleCamera_swigregister = _blcelc.iPcSimpleCamera_swigregister
iPcSimpleCamera_swigregister(iPcSimpleCamera)

celCreateSimpleCamera = _blcelc.celCreateSimpleCamera
celGetSetSimpleCamera = _blcelc.celGetSetSimpleCamera
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
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def MouseDown(*args): return _blcelc.iPcMeshSelectListener_MouseDown(*args)
    def MouseUp(*args): return _blcelc.iPcMeshSelectListener_MouseUp(*args)
    def MouseMove(*args): return _blcelc.iPcMeshSelectListener_MouseMove(*args)
    __swig_destroy__ = _blcelc.delete_iPcMeshSelectListener
    __del__ = lambda self : None;
iPcMeshSelectListener_swigregister = _blcelc.iPcMeshSelectListener_swigregister
iPcMeshSelectListener_swigregister(iPcMeshSelectListener)

class iPcMeshSelect(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMeshSelect, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMeshSelect, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    __swig_setmethods__["Camera"] = _blcelc.iPcMeshSelect_Camera_set
    Camera = property(None, _blcelc.iPcMeshSelect_Camera_set, None,
                    "iPcMeshSelect.Camera (write only) -> iPcCamera*\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcMeshSelect::SetCamera(iPcCamera*)")

    __swig_setmethods__["MouseButtons"] = fix_args(_blcelc.iPcMeshSelect_SetMouseButtons)
    MouseButtons = property(_blcelc.iPcMeshSelect_GetMouseButtons, fix_args(_blcelc.iPcMeshSelect_SetMouseButtons), None,
                    "iPcMeshSelect.MouseButtons -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMeshSelect::GetMouseButtons()\n\tset: void iPcMeshSelect::SetMouseButtons(...)")

    __swig_setmethods__["GlobalSelection"] = _blcelc.iPcMeshSelect_GlobalSelection_set
    GlobalSelection = property(_blcelc.iPcMeshSelect_GlobalSelection_get, _blcelc.iPcMeshSelect_GlobalSelection_set, None,
                    "iPcMeshSelect.GlobalSelection -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcMeshSelect::HasGlobalSelection()\n\tset: void iPcMeshSelect::SetGlobalSelection(bool)")


    __swig_setmethods__["FollowMode"] = _blcelc.iPcMeshSelect_FollowMode_set
    FollowMode = property(_blcelc.iPcMeshSelect_FollowMode_get, _blcelc.iPcMeshSelect_FollowMode_set, None,
                    "iPcMeshSelect.FollowMode -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcMeshSelect::HasFollowMode()\n\tset: void iPcMeshSelect::SetFollowMode(bool)")


    __swig_setmethods__["FollowAlwaysMode"] = _blcelc.iPcMeshSelect_FollowAlwaysMode_set
    FollowAlwaysMode = property(_blcelc.iPcMeshSelect_FollowAlwaysMode_get, _blcelc.iPcMeshSelect_FollowAlwaysMode_set, None,
                    "iPcMeshSelect.FollowAlwaysMode -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcMeshSelect::HasFollowAlwaysMode()\n\tset: void iPcMeshSelect::SetFollowAlwaysMode(bool)")


    __swig_setmethods__["DragMode"] = _blcelc.iPcMeshSelect_DragMode_set
    DragMode = property(_blcelc.iPcMeshSelect_DragMode_get, _blcelc.iPcMeshSelect_DragMode_set, None,
                    "iPcMeshSelect.DragMode -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcMeshSelect::HasDragMode()\n\tset: void iPcMeshSelect::SetDragMode(bool)")


    __swig_setmethods__["SendmoveEvent"] = _blcelc.iPcMeshSelect_SendmoveEvent_set
    SendmoveEvent = property(_blcelc.iPcMeshSelect_SendmoveEvent_get, _blcelc.iPcMeshSelect_SendmoveEvent_set, None,
                    "iPcMeshSelect.SendmoveEvent -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcMeshSelect::HasSendmoveEvent()\n\tset: void iPcMeshSelect::SetSendmoveEvent(bool)")


    __swig_setmethods__["SendupEvent"] = _blcelc.iPcMeshSelect_SendupEvent_set
    SendupEvent = property(_blcelc.iPcMeshSelect_SendupEvent_get, _blcelc.iPcMeshSelect_SendupEvent_set, None,
                    "iPcMeshSelect.SendupEvent -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcMeshSelect::HasSendupEvent()\n\tset: void iPcMeshSelect::SetSendupEvent(bool)")


    __swig_setmethods__["SenddownEvent"] = _blcelc.iPcMeshSelect_SenddownEvent_set
    SenddownEvent = property(_blcelc.iPcMeshSelect_SenddownEvent_get, _blcelc.iPcMeshSelect_SenddownEvent_set, None,
                    "iPcMeshSelect.SenddownEvent -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcMeshSelect::HasSenddownEvent()\n\tset: void iPcMeshSelect::SetSenddownEvent(bool)")


    __swig_setmethods__["MaxSelectionDistance"] = _blcelc.iPcMeshSelect_MaxSelectionDistance_set
    MaxSelectionDistance = property(_blcelc.iPcMeshSelect_MaxSelectionDistance_get, _blcelc.iPcMeshSelect_MaxSelectionDistance_set, None,
                    "iPcMeshSelect.MaxSelectionDistance -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcMeshSelect::GetMaxSelectionDistance()\n\tset: void iPcMeshSelect::SetMaxSelectionDistance(float)")


    __swig_destroy__ = _blcelc.delete_iPcMeshSelect
    __del__ = lambda self : None;
iPcMeshSelect_swigregister = _blcelc.iPcMeshSelect_swigregister
iPcMeshSelect_swigregister(iPcMeshSelect)

celCreateMeshSelect = _blcelc.celCreateMeshSelect
celGetSetMeshSelect = _blcelc.celGetSetMeshSelect
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
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetPath(*args): return _blcelc.iPcMesh_SetPath(*args)
    def LoadMesh(*args): return _blcelc.iPcMesh_LoadMesh(*args)
    def SetMesh(*args): return _blcelc.iPcMesh_SetMesh(*args)
    def CreateEmptyThing(*args): return _blcelc.iPcMesh_CreateEmptyThing(*args)
    def CreateEmptyGenmesh(*args): return _blcelc.iPcMesh_CreateEmptyGenmesh(*args)
    def CreateNullMesh(*args): return _blcelc.iPcMesh_CreateNullMesh(*args)
    def GetMesh(*args): return _blcelc.iPcMesh_GetMesh(*args)
    def MoveMesh(*args): return _blcelc.iPcMesh_MoveMesh(*args)
    def SetAction(*args): return _blcelc.iPcMesh_SetAction(*args)
    def GetAction(*args): return _blcelc.iPcMesh_GetAction(*args)
    def SetReverseAction(*args): return _blcelc.iPcMesh_SetReverseAction(*args)
    def SetAnimation(*args): return _blcelc.iPcMesh_SetAnimation(*args)
    def Hide(*args): return _blcelc.iPcMesh_Hide(*args)
    def Show(*args): return _blcelc.iPcMesh_Show(*args)
    def IsVisible(*args): return _blcelc.iPcMesh_IsVisible(*args)
    __swig_setmethods__["Path"] = _blcelc.iPcMesh_Path_set
    Path = property(None, _blcelc.iPcMesh_Path_set, None,
                    "iPcMesh.Path (write only) -> const char*\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcMesh::SetPath(const char*)")

    __swig_setmethods__["Mesh"] = fix_args(_blcelc.iPcMesh_SetMesh)
    Mesh = property(_blcelc.iPcMesh_GetMesh, fix_args(_blcelc.iPcMesh_SetMesh), None,
                    "iPcMesh.Mesh -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMesh::GetMesh()\n\tset: void iPcMesh::SetMesh(...)")

    __swig_setmethods__["Action"] = _blcelc.iPcMesh_Action_set
    Action = property(_blcelc.iPcMesh_Action_get, _blcelc.iPcMesh_Action_set, None,
                    "iPcMesh.Action -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iPcMesh::GetAction()\n\tset: void iPcMesh::SetAction(const char*)")


    __swig_setmethods__["ReverseAction"] = _blcelc.iPcMesh_ReverseAction_set
    ReverseAction = property(None, _blcelc.iPcMesh_ReverseAction_set, None,
                    "iPcMesh.ReverseAction (write only) -> bool\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcMesh::SetReverseAction(bool)")

    __swig_setmethods__["Animation"] = fix_args(_blcelc.iPcMesh_SetAnimation)
    Animation = property(None, fix_args(_blcelc.iPcMesh_SetAnimation), None,
                    "iPcMesh.Animation -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMesh::getmethod()\n\tset: void iPcMesh::SetAnimation(...)")

    Visible = property(_blcelc.iPcMesh_Visible_get, None, None,
                    "iPcMesh.Visible -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcMesh::IsVisible()")

    __swig_destroy__ = _blcelc.delete_iPcMesh
    __del__ = lambda self : None;
iPcMesh_swigregister = _blcelc.iPcMesh_swigregister
iPcMesh_swigregister(iPcMesh)

celCreateMesh = _blcelc.celCreateMesh
celGetSetMesh = _blcelc.celGetSetMesh
celGetMesh = _blcelc.celGetMesh
scfQuery_iPcMesh = _blcelc.scfQuery_iPcMesh
class iPcTimer(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcTimer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcTimer, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def WakeUp(*args): return _blcelc.iPcTimer_WakeUp(*args)
    def WakeUpFrame(*args): return _blcelc.iPcTimer_WakeUpFrame(*args)
    def Clear(*args): return _blcelc.iPcTimer_Clear(*args)
    __swig_destroy__ = _blcelc.delete_iPcTimer
    __del__ = lambda self : None;
iPcTimer_swigregister = _blcelc.iPcTimer_swigregister
iPcTimer_swigregister(iPcTimer)

celCreateTimer = _blcelc.celCreateTimer
celGetSetTimer = _blcelc.celGetSetTimer
celGetTimer = _blcelc.celGetTimer
scfQuery_iPcTimer = _blcelc.scfQuery_iPcTimer
class iPcProjectile(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcProjectile, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcProjectile, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def Start(*args): return _blcelc.iPcProjectile_Start(*args)
    def Interrupt(*args): return _blcelc.iPcProjectile_Interrupt(*args)
    def IsMoving(*args): return _blcelc.iPcProjectile_IsMoving(*args)
    Moving = property(_blcelc.iPcProjectile_Moving_get, None, None,
                    "iPcProjectile.Moving -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcProjectile::IsMoving()")

    __swig_destroy__ = _blcelc.delete_iPcProjectile
    __del__ = lambda self : None;
iPcProjectile_swigregister = _blcelc.iPcProjectile_swigregister
iPcProjectile_swigregister(iPcProjectile)

celCreateProjectile = _blcelc.celCreateProjectile
celGetSetProjectile = _blcelc.celGetSetProjectile
celGetProjectile = _blcelc.celGetProjectile
scfQuery_iPcProjectile = _blcelc.scfQuery_iPcProjectile
class iPcSolid(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcSolid, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcSolid, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetMesh(*args): return _blcelc.iPcSolid_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcSolid_GetMesh(*args)
    def GetCollider(*args): return _blcelc.iPcSolid_GetCollider(*args)
    def Setup(*args): return _blcelc.iPcSolid_Setup(*args)
    def SetupBox(*args): return _blcelc.iPcSolid_SetupBox(*args)
    __swig_setmethods__["Mesh"] = _blcelc.iPcSolid_Mesh_set
    Mesh = property(_blcelc.iPcSolid_Mesh_get, _blcelc.iPcSolid_Mesh_set, None,
                    "iPcSolid.Mesh -> iPcMesh*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMesh* iPcSolid::GetMesh()\n\tset: void iPcSolid::SetMesh(iPcMesh*)")


    Collider = property(_blcelc.iPcSolid_Collider_get, None, None,
                    "iPcSolid.Collider -> iCollider*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iCollider* iPcSolid::GetCollider()")

    __swig_destroy__ = _blcelc.delete_iPcSolid
    __del__ = lambda self : None;
iPcSolid_swigregister = _blcelc.iPcSolid_swigregister
iPcSolid_swigregister(iPcSolid)

celCreateSolid = _blcelc.celCreateSolid
celGetSetSolid = _blcelc.celGetSetSolid
celGetSolid = _blcelc.celGetSolid
scfQuery_iPcSolid = _blcelc.scfQuery_iPcSolid
class iPcGravity(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcGravity, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcGravity, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    GravityCollider = property(_blcelc.iPcGravity_GravityCollider_get, None, None,
                    "iPcGravity.GravityCollider -> iCollider*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iCollider* iPcGravity::GetGravityCollider()")

    __swig_setmethods__["Movable"] = _blcelc.iPcGravity_Movable_set
    Movable = property(_blcelc.iPcGravity_Movable_get, _blcelc.iPcGravity_Movable_set, None,
                    "iPcGravity.Movable -> iPcMovable*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMovable* iPcGravity::GetMovable()\n\tset: void iPcGravity::SetMovable(iPcMovable*)")


    __swig_setmethods__["Solid"] = _blcelc.iPcGravity_Solid_set
    Solid = property(_blcelc.iPcGravity_Solid_get, _blcelc.iPcGravity_Solid_set, None,
                    "iPcGravity.Solid -> iPcSolid*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcSolid* iPcGravity::GetSolid()\n\tset: void iPcGravity::SetSolid(iPcSolid*)")


    __swig_setmethods__["Weight"] = _blcelc.iPcGravity_Weight_set
    Weight = property(_blcelc.iPcGravity_Weight_get, _blcelc.iPcGravity_Weight_set, None,
                    "iPcGravity.Weight -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcGravity::GetWeight()\n\tset: void iPcGravity::SetWeight(float)")


    Resting = property(_blcelc.iPcGravity_Resting_get, None, None,
                    "iPcGravity.Resting -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcGravity::IsResting()")

    __swig_setmethods__["Active"] = _blcelc.iPcGravity_Active_set
    Active = property(_blcelc.iPcGravity_Active_get, _blcelc.iPcGravity_Active_set, None,
                    "iPcGravity.Active -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcGravity::IsActive()\n\tset: void iPcGravity::SetActive(bool)")


    __swig_destroy__ = _blcelc.delete_iPcGravity
    __del__ = lambda self : None;
iPcGravity_swigregister = _blcelc.iPcGravity_swigregister
iPcGravity_swigregister(iPcGravity)

celCreateGravity = _blcelc.celCreateGravity
celGetSetGravity = _blcelc.celGetSetGravity
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
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetMesh(*args): return _blcelc.iPcMovable_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcMovable_GetMesh(*args)
    def SetPos(*args): return _blcelc.iPcMovable_SetPos(*args)
    def AddConstraint(*args): return _blcelc.iPcMovable_AddConstraint(*args)
    def RemoveConstraint(*args): return _blcelc.iPcMovable_RemoveConstraint(*args)
    def RemoveAllConstraints(*args): return _blcelc.iPcMovable_RemoveAllConstraints(*args)
    __swig_setmethods__["Mesh"] = _blcelc.iPcMovable_Mesh_set
    Mesh = property(_blcelc.iPcMovable_Mesh_get, _blcelc.iPcMovable_Mesh_set, None,
                    "iPcMovable.Mesh -> iPcMesh*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcMesh* iPcMovable::GetMesh()\n\tset: void iPcMovable::SetMesh(iPcMesh*)")


    __swig_destroy__ = _blcelc.delete_iPcMovable
    __del__ = lambda self : None;
iPcMovable_swigregister = _blcelc.iPcMovable_swigregister
iPcMovable_swigregister(iPcMovable)

class iPcMovableConstraint(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMovableConstraint, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMovableConstraint, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def CheckMove(*args): return _blcelc.iPcMovableConstraint_CheckMove(*args)
    __swig_destroy__ = _blcelc.delete_iPcMovableConstraint
    __del__ = lambda self : None;
iPcMovableConstraint_swigregister = _blcelc.iPcMovableConstraint_swigregister
iPcMovableConstraint_swigregister(iPcMovableConstraint)

celCreateMovable = _blcelc.celCreateMovable
celGetSetMovable = _blcelc.celGetSetMovable
celGetMovable = _blcelc.celGetMovable
scfQuery_iPcMovable = _blcelc.scfQuery_iPcMovable
class iPcInventoryListener(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcInventoryListener, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcInventoryListener, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def AddChild(*args): return _blcelc.iPcInventoryListener_AddChild(*args)
    def RemoveChild(*args): return _blcelc.iPcInventoryListener_RemoveChild(*args)
    __swig_destroy__ = _blcelc.delete_iPcInventoryListener
    __del__ = lambda self : None;
iPcInventoryListener_swigregister = _blcelc.iPcInventoryListener_swigregister
iPcInventoryListener_swigregister(iPcInventoryListener)

class iCelInventorySpaceSlot(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelInventorySpaceSlot, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelInventorySpaceSlot, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetEntity(*args): return _blcelc.iCelInventorySpaceSlot_GetEntity(*args)
    def AddEntity(*args): return _blcelc.iCelInventorySpaceSlot_AddEntity(*args)
    def RemoveEntity(*args): return _blcelc.iCelInventorySpaceSlot_RemoveEntity(*args)
    def GetCount(*args): return _blcelc.iCelInventorySpaceSlot_GetCount(*args)
    def RemoveAll(*args): return _blcelc.iCelInventorySpaceSlot_RemoveAll(*args)
    def IsEmpty(*args): return _blcelc.iCelInventorySpaceSlot_IsEmpty(*args)
    Count = property(_blcelc.iCelInventorySpaceSlot_Count_get, None, None,
                    "iCelInventorySpaceSlot.Count -> int  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: int iCelInventorySpaceSlot::GetCount()")

    Empty = property(_blcelc.iCelInventorySpaceSlot_Empty_get, None, None,
                    "iCelInventorySpaceSlot.Empty -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iCelInventorySpaceSlot::IsEmpty()")

    __swig_destroy__ = _blcelc.delete_iCelInventorySpaceSlot
    __del__ = lambda self : None;
iCelInventorySpaceSlot_swigregister = _blcelc.iCelInventorySpaceSlot_swigregister
iCelInventorySpaceSlot_swigregister(iCelInventorySpaceSlot)

class iCelInventorySpace(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelInventorySpace, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelInventorySpace, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetSpaceTotal(*args): return _blcelc.iCelInventorySpace_GetSpaceTotal(*args)
    def GetSpaceLeft(*args): return _blcelc.iCelInventorySpace_GetSpaceLeft(*args)
    def GetSpaceTaken(*args): return _blcelc.iCelInventorySpace_GetSpaceTaken(*args)
    def AddEntity(*args): return _blcelc.iCelInventorySpace_AddEntity(*args)
    def RemoveEntity(*args): return _blcelc.iCelInventorySpace_RemoveEntity(*args)
    def RemoveAll(*args): return _blcelc.iCelInventorySpace_RemoveAll(*args)
    def GetSlot(*args): return _blcelc.iCelInventorySpace_GetSlot(*args)
    def GetEntity(*args): return _blcelc.iCelInventorySpace_GetEntity(*args)
    SpaceTotal = property(_blcelc.iCelInventorySpace_SpaceTotal_get, None, None,
                    "iCelInventorySpace.SpaceTotal -> int  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: int iCelInventorySpace::GetSpaceTotal()")

    SpaceLeft = property(_blcelc.iCelInventorySpace_SpaceLeft_get, None, None,
                    "iCelInventorySpace.SpaceLeft -> int  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: int iCelInventorySpace::GetSpaceLeft()")

    SpaceTaken = property(_blcelc.iCelInventorySpace_SpaceTaken_get, None, None,
                    "iCelInventorySpace.SpaceTaken -> int  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: int iCelInventorySpace::GetSpaceTaken()")

    __swig_destroy__ = _blcelc.delete_iCelInventorySpace
    __del__ = lambda self : None;
iCelInventorySpace_swigregister = _blcelc.iCelInventorySpace_swigregister
iCelInventorySpace_swigregister(iCelInventorySpace)

class iPcInventory(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcInventory, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcInventory, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def AddInventoryListener(*args): return _blcelc.iPcInventory_AddInventoryListener(*args)
    def RemoveInventoryListener(*args): return _blcelc.iPcInventory_RemoveInventoryListener(*args)
    def AddEntity(*args): return _blcelc.iPcInventory_AddEntity(*args)
    def RemoveEntity(*args): return _blcelc.iPcInventory_RemoveEntity(*args)
    def RemoveAll(*args): return _blcelc.iPcInventory_RemoveAll(*args)
    def GetEntityCount(*args): return _blcelc.iPcInventory_GetEntityCount(*args)
    def GetEntity(*args): return _blcelc.iPcInventory_GetEntity(*args)
    def In(*args): return _blcelc.iPcInventory_In(*args)
    def FindEntity(*args): return _blcelc.iPcInventory_FindEntity(*args)
    def GetEntitySlot(*args): return _blcelc.iPcInventory_GetEntitySlot(*args)
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
    def SetSpace(*args): return _blcelc.iPcInventory_SetSpace(*args)
    def GetSpace(*args): return _blcelc.iPcInventory_GetSpace(*args)
    EntityCount = property(_blcelc.iPcInventory_EntityCount_get, None, None,
                    "iPcInventory.EntityCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iPcInventory::GetEntityCount()")

    __swig_setmethods__["Space"] = _blcelc.iPcInventory_Space_set
    Space = property(_blcelc.iPcInventory_Space_get, _blcelc.iPcInventory_Space_set, None,
                    "iPcInventory.Space -> iCelInventorySpace*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iCelInventorySpace* iPcInventory::GetSpace()\n\tset: void iPcInventory::SetSpace(iCelInventorySpace*)")


    def GetEntities(self): return iCelEntityInvFakeArray(self)
    __swig_getmethods__["Entities"] = lambda self: self.GetEntities() 
    __swig_destroy__ = _blcelc.delete_iPcInventory
    __del__ = lambda self : None;
iPcInventory_swigregister = _blcelc.iPcInventory_swigregister
iPcInventory_swigregister(iPcInventory)

celCreateInventory = _blcelc.celCreateInventory
celGetSetInventory = _blcelc.celGetSetInventory
celGetInventory = _blcelc.celGetInventory
scfQuery_iPcInventory = _blcelc.scfQuery_iPcInventory
class iCelEntityInvFakeArray:
	def __init__(self,parent): self.parent = parent
	def __contains__(self,obj):
		if self.parent.In(obj): return True
		else: return False
	def __repr__(self): return "List of "+str("iCelEntity")
	def __len__(self): return self.parent.GetEntityCount()
	def __delitem__(self,val):
		if type(val) == type(""):
			obj = self.parent.In(val)
			if obj: return self.parent.RemoveEntity(obj)
			else: raise IndexError(val+" not in list")
		else: return self.parent.RemoveEntity(val)
	def __noappend__(self,obj):
		print "Append not supported by this list"
	def content_iterator(self):
		for idx in xrange(len(self)):
			yield self.parent.GetEntity(idx)
	def __iter__(self): return self.content_iterator()
	def __getitem__(self,val):
		if type(val) == type(""): return self.parent.In(val)
		else: return self.parent.GetEntity(val)
	def append(self,obj): return self.parent.AddEntity(obj)

class iPcCharacteristics(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCharacteristics, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCharacteristics, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
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
    __swig_destroy__ = _blcelc.delete_iPcCharacteristics
    __del__ = lambda self : None;
iPcCharacteristics_swigregister = _blcelc.iPcCharacteristics_swigregister
iPcCharacteristics_swigregister(iPcCharacteristics)

celCreateCharacteristics = _blcelc.celCreateCharacteristics
celGetSetCharacteristics = _blcelc.celGetSetCharacteristics
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
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetText(*args): return _blcelc.iPcTooltip_SetText(*args)
    def Show(*args): return _blcelc.iPcTooltip_Show(*args)
    def Hide(*args): return _blcelc.iPcTooltip_Hide(*args)
    def IsVisible(*args): return _blcelc.iPcTooltip_IsVisible(*args)
    def SetTextColor(*args): return _blcelc.iPcTooltip_SetTextColor(*args)
    def SetBackgroundColor(*args): return _blcelc.iPcTooltip_SetBackgroundColor(*args)
    def SetFont(*args): return _blcelc.iPcTooltip_SetFont(*args)
    def SetJustify(*args): return _blcelc.iPcTooltip_SetJustify(*args)
    __swig_setmethods__["Text"] = _blcelc.iPcTooltip_Text_set
    Text = property(None, _blcelc.iPcTooltip_Text_set, None,
                    "iPcTooltip.Text (write only) -> const char*\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcTooltip::SetText(const char*)")

    Visible = property(_blcelc.iPcTooltip_Visible_get, None, None,
                    "iPcTooltip.Visible -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcTooltip::IsVisible()")

    __swig_setmethods__["TextColor"] = fix_args(_blcelc.iPcTooltip_SetTextColor)
    TextColor = property(None, fix_args(_blcelc.iPcTooltip_SetTextColor), None,
                    "iPcTooltip.TextColor -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcTooltip::getmethod()\n\tset: void iPcTooltip::SetTextColor(...)")

    __swig_setmethods__["BackgroundColor"] = fix_args(_blcelc.iPcTooltip_SetBackgroundColor)
    BackgroundColor = property(None, fix_args(_blcelc.iPcTooltip_SetBackgroundColor), None,
                    "iPcTooltip.BackgroundColor -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcTooltip::getmethod()\n\tset: void iPcTooltip::SetBackgroundColor(...)")

    __swig_setmethods__["Font"] = _blcelc.iPcTooltip_Font_set
    Font = property(None, _blcelc.iPcTooltip_Font_set, None,
                    "iPcTooltip.Font (write only) -> iFont*\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcTooltip::SetFont(iFont*)")

    __swig_setmethods__["Justify"] = _blcelc.iPcTooltip_Justify_set
    Justify = property(None, _blcelc.iPcTooltip_Justify_set, None,
                    "iPcTooltip.Justify (write only) -> celTooltipJustify\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcTooltip::SetJustify(celTooltipJustify)")

    __swig_destroy__ = _blcelc.delete_iPcTooltip
    __del__ = lambda self : None;
iPcTooltip_swigregister = _blcelc.iPcTooltip_swigregister
iPcTooltip_swigregister(iPcTooltip)

celCreateToolTip = _blcelc.celCreateToolTip
celGetSetToolTip = _blcelc.celGetSetToolTip
celGetToolTip = _blcelc.celGetToolTip
scfQuery_iPcTooltip = _blcelc.scfQuery_iPcTooltip
class iPcSoundListener(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcSoundListener, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcSoundListener, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetSoundListener(*args): return _blcelc.iPcSoundListener_GetSoundListener(*args)
    SoundListener = property(_blcelc.iPcSoundListener_SoundListener_get, None, None,
                    "iPcSoundListener.SoundListener -> iSndSysListener*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iSndSysListener* iPcSoundListener::GetSoundListener()")

    __swig_destroy__ = _blcelc.delete_iPcSoundListener
    __del__ = lambda self : None;
iPcSoundListener_swigregister = _blcelc.iPcSoundListener_swigregister
iPcSoundListener_swigregister(iPcSoundListener)

class iPcSoundSource(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcSoundSource, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcSoundSource, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetSoundSource(*args): return _blcelc.iPcSoundSource_GetSoundSource(*args)
    def SetSoundName(*args): return _blcelc.iPcSoundSource_SetSoundName(*args)
    def GetSoundName(*args): return _blcelc.iPcSoundSource_GetSoundName(*args)
    def SetMode(*args): return _blcelc.iPcSoundSource_SetMode(*args)
    def GetMode(*args): return _blcelc.iPcSoundSource_GetMode(*args)
    SoundSource = property(_blcelc.iPcSoundSource_SoundSource_get, None, None,
                    "iPcSoundSource.SoundSource -> iSndSysSource*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iSndSysSource* iPcSoundSource::GetSoundSource()")

    __swig_setmethods__["SoundName"] = _blcelc.iPcSoundSource_SoundName_set
    SoundName = property(_blcelc.iPcSoundSource_SoundName_get, _blcelc.iPcSoundSource_SoundName_set, None,
                    "iPcSoundSource.SoundName -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iPcSoundSource::GetSoundName()\n\tset: void iPcSoundSource::SetSoundName(const char*)")


    __swig_destroy__ = _blcelc.delete_iPcSoundSource
    __del__ = lambda self : None;
iPcSoundSource_swigregister = _blcelc.iPcSoundSource_swigregister
iPcSoundSource_swigregister(iPcSoundSource)

celCreateSoundSource = _blcelc.celCreateSoundSource
celGetSetSoundSource = _blcelc.celGetSetSoundSource
celGetSoundSource = _blcelc.celGetSoundSource
scfQuery_iPcSoundSource = _blcelc.scfQuery_iPcSoundSource
celCreateSoundListener = _blcelc.celCreateSoundListener
celGetSetSoundListener = _blcelc.celGetSetSoundListener
celGetSoundListener = _blcelc.celGetSoundListener
scfQuery_iPcSoundListener = _blcelc.scfQuery_iPcSoundListener
class iPcPropertyListener(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcPropertyListener, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcPropertyListener, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def PropertyChanged(*args): return _blcelc.iPcPropertyListener_PropertyChanged(*args)
    __swig_destroy__ = _blcelc.delete_iPcPropertyListener
    __del__ = lambda self : None;
iPcPropertyListener_swigregister = _blcelc.iPcPropertyListener_swigregister
iPcPropertyListener_swigregister(iPcPropertyListener)

class iPcProperties(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcProperties, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcProperties, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def AddPropertyListener(*args): return _blcelc.iPcProperties_AddPropertyListener(*args)
    def RemovePropertyListener(*args): return _blcelc.iPcProperties_RemovePropertyListener(*args)
    def SetProperty(*args): return _blcelc.iPcProperties_SetProperty(*args)
    def GetPropertyIndex(*args): return _blcelc.iPcProperties_GetPropertyIndex(*args)
    def SetPropertyIndex(*args): return _blcelc.iPcProperties_SetPropertyIndex(*args)
    def GetPropertyType(*args): return _blcelc.iPcProperties_GetPropertyType(*args)
    def GetPropertyFloat(*args): return _blcelc.iPcProperties_GetPropertyFloat(*args)
    def GetPropertyLong(*args): return _blcelc.iPcProperties_GetPropertyLong(*args)
    def GetPropertyBool(*args): return _blcelc.iPcProperties_GetPropertyBool(*args)
    def GetPropertyVector(*args): return _blcelc.iPcProperties_GetPropertyVector(*args)
    def GetPropertyColor(*args): return _blcelc.iPcProperties_GetPropertyColor(*args)
    def GetPropertyString(*args): return _blcelc.iPcProperties_GetPropertyString(*args)
    def GetPropertyPClass(*args): return _blcelc.iPcProperties_GetPropertyPClass(*args)
    def GetPropertyEntity(*args): return _blcelc.iPcProperties_GetPropertyEntity(*args)
    def GetPropertyIBase(*args): return _blcelc.iPcProperties_GetPropertyIBase(*args)
    def ClearProperty(*args): return _blcelc.iPcProperties_ClearProperty(*args)
    def Clear(*args): return _blcelc.iPcProperties_Clear(*args)
    def GetPropertyCount(*args): return _blcelc.iPcProperties_GetPropertyCount(*args)
    def GetPropertyName(*args): return _blcelc.iPcProperties_GetPropertyName(*args)
    def Dump(*args): return _blcelc.iPcProperties_Dump(*args)
    __swig_setmethods__["Property"] = fix_args(_blcelc.iPcProperties_SetProperty)
    Property = property(None, fix_args(_blcelc.iPcProperties_SetProperty), None,
                    "iPcProperties.Property -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcProperties::getmethod()\n\tset: void iPcProperties::SetProperty(...)")

    __swig_setmethods__["PropertyIndex"] = fix_args(_blcelc.iPcProperties_SetPropertyIndex)
    PropertyIndex = property(_blcelc.iPcProperties_GetPropertyIndex, fix_args(_blcelc.iPcProperties_SetPropertyIndex), None,
                    "iPcProperties.PropertyIndex -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcProperties::GetPropertyIndex()\n\tset: void iPcProperties::SetPropertyIndex(...)")

    PropertyCount = property(_blcelc.iPcProperties_PropertyCount_get, None, None,
                    "iPcProperties.PropertyCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: size_t iPcProperties::GetPropertyCount()")

    __swig_destroy__ = _blcelc.delete_iPcProperties
    __del__ = lambda self : None;
iPcProperties_swigregister = _blcelc.iPcProperties_swigregister
iPcProperties_swigregister(iPcProperties)

celCreateProperties = _blcelc.celCreateProperties
celGetSetProperties = _blcelc.celGetSetProperties
celGetProperties = _blcelc.celGetProperties
scfQuery_iPcProperties = _blcelc.scfQuery_iPcProperties
class iPcMover(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMover, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMover, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def Start(*args): return _blcelc.iPcMover_Start(*args)
    def Interrupt(*args): return _blcelc.iPcMover_Interrupt(*args)
    def GetSector(*args): return _blcelc.iPcMover_GetSector(*args)
    def GetPosition(*args): return _blcelc.iPcMover_GetPosition(*args)
    def GetUp(*args): return _blcelc.iPcMover_GetUp(*args)
    def GetSqRadius(*args): return _blcelc.iPcMover_GetSqRadius(*args)
    def IsMoving(*args): return _blcelc.iPcMover_IsMoving(*args)
    Sector = property(_blcelc.iPcMover_Sector_get, None, None,
                    "iPcMover.Sector -> iSector*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iSector* iPcMover::GetSector()")

    Position = property(_blcelc.iPcMover_Position_get, None, None,
                    "iPcMover.Position -> const csVector3&  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const csVector3& iPcMover::GetPosition()")

    Up = property(_blcelc.iPcMover_Up_get, None, None,
                    "iPcMover.Up -> const csVector3&  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const csVector3& iPcMover::GetUp()")

    SqRadius = property(_blcelc.iPcMover_SqRadius_get, None, None,
                    "iPcMover.SqRadius -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iPcMover::GetSqRadius()")

    Moving = property(_blcelc.iPcMover_Moving_get, None, None,
                    "iPcMover.Moving -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcMover::IsMoving()")

    __swig_destroy__ = _blcelc.delete_iPcMover
    __del__ = lambda self : None;
iPcMover_swigregister = _blcelc.iPcMover_swigregister
iPcMover_swigregister(iPcMover)

celCreateMover = _blcelc.celCreateMover
celGetSetMover = _blcelc.celGetSetMover
celGetMover = _blcelc.celGetMover
scfQuery_iPcMover = _blcelc.scfQuery_iPcMover
class iPcHover(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcHover, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcHover, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetHeightBeamCutoff(*args): return _blcelc.iPcHover_SetHeightBeamCutoff(*args)
    def SetAngularBeamOffset(*args): return _blcelc.iPcHover_SetAngularBeamOffset(*args)
    def SetAngularCutoffHeight(*args): return _blcelc.iPcHover_SetAngularCutoffHeight(*args)
    def SetAngularCorrectionStrength(*args): return _blcelc.iPcHover_SetAngularCorrectionStrength(*args)
    def SetFactors(*args): return _blcelc.iPcHover_SetFactors(*args)
    def SetHoverHeight(*args): return _blcelc.iPcHover_SetHoverHeight(*args)
    def HoverOn(*args): return _blcelc.iPcHover_HoverOn(*args)
    def HoverOff(*args): return _blcelc.iPcHover_HoverOff(*args)
    def GetHeight(*args): return _blcelc.iPcHover_GetHeight(*args)
    __swig_setmethods__["HeightBeamCutoff"] = _blcelc.iPcHover_HeightBeamCutoff_set
    HeightBeamCutoff = property(None, _blcelc.iPcHover_HeightBeamCutoff_set, None,
                    "iPcHover.HeightBeamCutoff (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcHover::SetHeightBeamCutoff(float)")

    __swig_setmethods__["AngularBeamOffset"] = _blcelc.iPcHover_AngularBeamOffset_set
    AngularBeamOffset = property(None, _blcelc.iPcHover_AngularBeamOffset_set, None,
                    "iPcHover.AngularBeamOffset (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcHover::SetAngularBeamOffset(float)")

    __swig_setmethods__["AngularCutoffHeight"] = _blcelc.iPcHover_AngularCutoffHeight_set
    AngularCutoffHeight = property(None, _blcelc.iPcHover_AngularCutoffHeight_set, None,
                    "iPcHover.AngularCutoffHeight (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcHover::SetAngularCutoffHeight(float)")

    __swig_setmethods__["AngularCorrectionStrength"] = _blcelc.iPcHover_AngularCorrectionStrength_set
    AngularCorrectionStrength = property(None, _blcelc.iPcHover_AngularCorrectionStrength_set, None,
                    "iPcHover.AngularCorrectionStrength (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcHover::SetAngularCorrectionStrength(float)")

    __swig_setmethods__["Factors"] = fix_args(_blcelc.iPcHover_SetFactors)
    Factors = property(None, fix_args(_blcelc.iPcHover_SetFactors), None,
                    "iPcHover.Factors -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcHover::getmethod()\n\tset: void iPcHover::SetFactors(...)")

    __swig_setmethods__["HoverHeight"] = _blcelc.iPcHover_HoverHeight_set
    HoverHeight = property(None, _blcelc.iPcHover_HoverHeight_set, None,
                    "iPcHover.HoverHeight (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcHover::SetHoverHeight(float)")

    Height = property(_blcelc.iPcHover_Height_get, None, None,
                    "iPcHover.Height -> float  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: float iPcHover::GetHeight()")

    __swig_destroy__ = _blcelc.delete_iPcHover
    __del__ = lambda self : None;
iPcHover_swigregister = _blcelc.iPcHover_swigregister
iPcHover_swigregister(iPcHover)

celCreateHover = _blcelc.celCreateHover
celGetSetHover = _blcelc.celGetSetHover
celGetHover = _blcelc.celGetHover
scfQuery_iPcHover = _blcelc.scfQuery_iPcHover
class iPcCraftController(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCraftController, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCraftController, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def StartTurnLeft(*args): return _blcelc.iPcCraftController_StartTurnLeft(*args)
    def StopTurnLeft(*args): return _blcelc.iPcCraftController_StopTurnLeft(*args)
    def StartTurnRight(*args): return _blcelc.iPcCraftController_StartTurnRight(*args)
    def StopTurnRight(*args): return _blcelc.iPcCraftController_StopTurnRight(*args)
    def StartTurnUp(*args): return _blcelc.iPcCraftController_StartTurnUp(*args)
    def StopTurnUp(*args): return _blcelc.iPcCraftController_StopTurnUp(*args)
    def StartTurnDown(*args): return _blcelc.iPcCraftController_StartTurnDown(*args)
    def StopTurnDown(*args): return _blcelc.iPcCraftController_StopTurnDown(*args)
    def SetAccTurn(*args): return _blcelc.iPcCraftController_SetAccTurn(*args)
    def SetAccPitch(*args): return _blcelc.iPcCraftController_SetAccPitch(*args)
    def SetMaxTurn(*args): return _blcelc.iPcCraftController_SetMaxTurn(*args)
    def SetMaxPitch(*args): return _blcelc.iPcCraftController_SetMaxPitch(*args)
    def SetRoll(*args): return _blcelc.iPcCraftController_SetRoll(*args)
    def SetThrustForce(*args): return _blcelc.iPcCraftController_SetThrustForce(*args)
    def SetTopSpeed(*args): return _blcelc.iPcCraftController_SetTopSpeed(*args)
    def SetRedirectVelocityRatio(*args): return _blcelc.iPcCraftController_SetRedirectVelocityRatio(*args)
    def SetDecelerationRate(*args): return _blcelc.iPcCraftController_SetDecelerationRate(*args)
    def SetBrakingSpeed(*args): return _blcelc.iPcCraftController_SetBrakingSpeed(*args)
    def ThrustOn(*args): return _blcelc.iPcCraftController_ThrustOn(*args)
    def ThrustOff(*args): return _blcelc.iPcCraftController_ThrustOff(*args)
    def IsThrusterOn(*args): return _blcelc.iPcCraftController_IsThrusterOn(*args)
    def SetAfterBurnerTopSpeed(*args): return _blcelc.iPcCraftController_SetAfterBurnerTopSpeed(*args)
    def AfterBurnerOn(*args): return _blcelc.iPcCraftController_AfterBurnerOn(*args)
    def AfterBurnerOff(*args): return _blcelc.iPcCraftController_AfterBurnerOff(*args)
    def BrakesOn(*args): return _blcelc.iPcCraftController_BrakesOn(*args)
    def BrakesOff(*args): return _blcelc.iPcCraftController_BrakesOff(*args)
    def SlideOn(*args): return _blcelc.iPcCraftController_SlideOn(*args)
    def SlideOff(*args): return _blcelc.iPcCraftController_SlideOff(*args)
    __swig_setmethods__["AccTurn"] = _blcelc.iPcCraftController_AccTurn_set
    AccTurn = property(None, _blcelc.iPcCraftController_AccTurn_set, None,
                    "iPcCraftController.AccTurn (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetAccTurn(float)")

    __swig_setmethods__["AccPitch"] = _blcelc.iPcCraftController_AccPitch_set
    AccPitch = property(None, _blcelc.iPcCraftController_AccPitch_set, None,
                    "iPcCraftController.AccPitch (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetAccPitch(float)")

    __swig_setmethods__["MaxTurn"] = _blcelc.iPcCraftController_MaxTurn_set
    MaxTurn = property(None, _blcelc.iPcCraftController_MaxTurn_set, None,
                    "iPcCraftController.MaxTurn (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetMaxTurn(float)")

    __swig_setmethods__["MaxPitch"] = _blcelc.iPcCraftController_MaxPitch_set
    MaxPitch = property(None, _blcelc.iPcCraftController_MaxPitch_set, None,
                    "iPcCraftController.MaxPitch (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetMaxPitch(float)")

    __swig_setmethods__["ThrustForce"] = _blcelc.iPcCraftController_ThrustForce_set
    ThrustForce = property(None, _blcelc.iPcCraftController_ThrustForce_set, None,
                    "iPcCraftController.ThrustForce (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetThrustForce(float)")

    __swig_setmethods__["TopSpeed"] = _blcelc.iPcCraftController_TopSpeed_set
    TopSpeed = property(None, _blcelc.iPcCraftController_TopSpeed_set, None,
                    "iPcCraftController.TopSpeed (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetTopSpeed(float)")

    __swig_setmethods__["RedirectVelocityRatio"] = _blcelc.iPcCraftController_RedirectVelocityRatio_set
    RedirectVelocityRatio = property(None, _blcelc.iPcCraftController_RedirectVelocityRatio_set, None,
                    "iPcCraftController.RedirectVelocityRatio (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetRedirectVelocityRatio(float)")

    __swig_setmethods__["DecelerationRate"] = _blcelc.iPcCraftController_DecelerationRate_set
    DecelerationRate = property(None, _blcelc.iPcCraftController_DecelerationRate_set, None,
                    "iPcCraftController.DecelerationRate (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetDecelerationRate(float)")

    __swig_setmethods__["BrakingSpeed"] = _blcelc.iPcCraftController_BrakingSpeed_set
    BrakingSpeed = property(None, _blcelc.iPcCraftController_BrakingSpeed_set, None,
                    "iPcCraftController.BrakingSpeed (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetBrakingSpeed(float)")

    ThrusterOn = property(_blcelc.iPcCraftController_ThrusterOn_get, None, None,
                    "iPcCraftController.ThrusterOn -> bool  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: bool iPcCraftController::IsThrusterOn()")

    __swig_setmethods__["AfterBurnerTopSpeed"] = _blcelc.iPcCraftController_AfterBurnerTopSpeed_set
    AfterBurnerTopSpeed = property(None, _blcelc.iPcCraftController_AfterBurnerTopSpeed_set, None,
                    "iPcCraftController.AfterBurnerTopSpeed (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcCraftController::SetAfterBurnerTopSpeed(float)")

    __swig_destroy__ = _blcelc.delete_iPcCraftController
    __del__ = lambda self : None;
iPcCraftController_swigregister = _blcelc.iPcCraftController_swigregister
iPcCraftController_swigregister(iPcCraftController)

celCreateCraftController = _blcelc.celCreateCraftController
celGetSetCraftController = _blcelc.celGetSetCraftController
celGetCraftController = _blcelc.celGetCraftController
scfQuery_iPcCraftController = _blcelc.scfQuery_iPcCraftController
class iPcWheeled(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcWheeled, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcWheeled, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetWheelMesh(*args): return _blcelc.iPcWheeled_SetWheelMesh(*args)
    def SetTankMode(*args): return _blcelc.iPcWheeled_SetTankMode(*args)
    def GetTankMode(*args): return _blcelc.iPcWheeled_GetTankMode(*args)
    def AddWheelAuto(*args): return _blcelc.iPcWheeled_AddWheelAuto(*args)
    def AddWheel(*args): return _blcelc.iPcWheeled_AddWheel(*args)
    def DeleteWheel(*args): return _blcelc.iPcWheeled_DeleteWheel(*args)
    def DeleteAllWheels(*args): return _blcelc.iPcWheeled_DeleteAllWheels(*args)
    def DestroyWheel(*args): return _blcelc.iPcWheeled_DestroyWheel(*args)
    def DestroyAllWheels(*args): return _blcelc.iPcWheeled_DestroyAllWheels(*args)
    def RestoreWheel(*args): return _blcelc.iPcWheeled_RestoreWheel(*args)
    def RestoreAllWheels(*args): return _blcelc.iPcWheeled_RestoreAllWheels(*args)
    def SetOuterWheelSteerPreset(*args): return _blcelc.iPcWheeled_SetOuterWheelSteerPreset(*args)
    def SetFrontWheelPreset(*args): return _blcelc.iPcWheeled_SetFrontWheelPreset(*args)
    def SetRearWheelPreset(*args): return _blcelc.iPcWheeled_SetRearWheelPreset(*args)
    def Accelerate(*args): return _blcelc.iPcWheeled_Accelerate(*args)
    def IsAccelerating(*args): return _blcelc.iPcWheeled_IsAccelerating(*args)
    def Brake(*args): return _blcelc.iPcWheeled_Brake(*args)
    def IsBraking(*args): return _blcelc.iPcWheeled_IsBraking(*args)
    def Handbrake(*args): return _blcelc.iPcWheeled_Handbrake(*args)
    def IsHandbraking(*args): return _blcelc.iPcWheeled_IsHandbraking(*args)
    def SetSteerAmount(*args): return _blcelc.iPcWheeled_SetSteerAmount(*args)
    def SetBrakeForce(*args): return _blcelc.iPcWheeled_SetBrakeForce(*args)
    def GetSteerAmount(*args): return _blcelc.iPcWheeled_GetSteerAmount(*args)
    def SteerLeft(*args): return _blcelc.iPcWheeled_SteerLeft(*args)
    def SteerRight(*args): return _blcelc.iPcWheeled_SteerRight(*args)
    def SteerStraight(*args): return _blcelc.iPcWheeled_SteerStraight(*args)
    def Reverse(*args): return _blcelc.iPcWheeled_Reverse(*args)
    def Neutral(*args): return _blcelc.iPcWheeled_Neutral(*args)
    def SetAutoTransmission(*args): return _blcelc.iPcWheeled_SetAutoTransmission(*args)
    def SetGear(*args): return _blcelc.iPcWheeled_SetGear(*args)
    def GetGear(*args): return _blcelc.iPcWheeled_GetGear(*args)
    def SetGearSettings(*args): return _blcelc.iPcWheeled_SetGearSettings(*args)
    def GetTopGear(*args): return _blcelc.iPcWheeled_GetTopGear(*args)
    def SetAutoReverse(*args): return _blcelc.iPcWheeled_SetAutoReverse(*args)
    def GetBodyGroup(*args): return _blcelc.iPcWheeled_GetBodyGroup(*args)
    def SetWheelPosition(*args): return _blcelc.iPcWheeled_SetWheelPosition(*args)
    def SetWheelRotation(*args): return _blcelc.iPcWheeled_SetWheelRotation(*args)
    def SetWheelSuspensionSoftness(*args): return _blcelc.iPcWheeled_SetWheelSuspensionSoftness(*args)
    def SetWheelSuspensionDamping(*args): return _blcelc.iPcWheeled_SetWheelSuspensionDamping(*args)
    def SetWheelLeftSteerSensitivity(*args): return _blcelc.iPcWheeled_SetWheelLeftSteerSensitivity(*args)
    def SetWheelRightSteerSensitivity(*args): return _blcelc.iPcWheeled_SetWheelRightSteerSensitivity(*args)
    def SetWheelFriction(*args): return _blcelc.iPcWheeled_SetWheelFriction(*args)
    def SetWheelMass(*args): return _blcelc.iPcWheeled_SetWheelMass(*args)
    def SetWheelTurnSpeed(*args): return _blcelc.iPcWheeled_SetWheelTurnSpeed(*args)
    def SetWheelReturnSpeed(*args): return _blcelc.iPcWheeled_SetWheelReturnSpeed(*args)
    def SetWheelEnginePower(*args): return _blcelc.iPcWheeled_SetWheelEnginePower(*args)
    def SetWheelBrakePower(*args): return _blcelc.iPcWheeled_SetWheelBrakePower(*args)
    def SetWheelSteerInverted(*args): return _blcelc.iPcWheeled_SetWheelSteerInverted(*args)
    def SetWheelHandbrakeAffected(*args): return _blcelc.iPcWheeled_SetWheelHandbrakeAffected(*args)
    def GetWheelPosition(*args): return _blcelc.iPcWheeled_GetWheelPosition(*args)
    def GetWheelRotation(*args): return _blcelc.iPcWheeled_GetWheelRotation(*args)
    def GetWheelSuspensionSoftness(*args): return _blcelc.iPcWheeled_GetWheelSuspensionSoftness(*args)
    def GetWheelSuspensionDamping(*args): return _blcelc.iPcWheeled_GetWheelSuspensionDamping(*args)
    def GetWheelLeftSteerSensitivity(*args): return _blcelc.iPcWheeled_GetWheelLeftSteerSensitivity(*args)
    def GetWheelRightSteerSensitivity(*args): return _blcelc.iPcWheeled_GetWheelRightSteerSensitivity(*args)
    def GetWheelFriction(*args): return _blcelc.iPcWheeled_GetWheelFriction(*args)
    def GetWheelMass(*args): return _blcelc.iPcWheeled_GetWheelMass(*args)
    def GetWheelTurnSpeed(*args): return _blcelc.iPcWheeled_GetWheelTurnSpeed(*args)
    def GetWheelReturnSpeed(*args): return _blcelc.iPcWheeled_GetWheelReturnSpeed(*args)
    def GetWheelEnginePower(*args): return _blcelc.iPcWheeled_GetWheelEnginePower(*args)
    def GetWheelBrakePower(*args): return _blcelc.iPcWheeled_GetWheelBrakePower(*args)
    def GetWheelSteerInverted(*args): return _blcelc.iPcWheeled_GetWheelSteerInverted(*args)
    def GetWheelHandbrakeAffected(*args): return _blcelc.iPcWheeled_GetWheelHandbrakeAffected(*args)
    def GetWheelCount(*args): return _blcelc.iPcWheeled_GetWheelCount(*args)
    def GetWheelBody(*args): return _blcelc.iPcWheeled_GetWheelBody(*args)
    def GetWheelJoint(*args): return _blcelc.iPcWheeled_GetWheelJoint(*args)
    def SetCollisionCallbackEnabled(*args): return _blcelc.iPcWheeled_SetCollisionCallbackEnabled(*args)
    def IsCollisionCallbackEnabled(*args): return _blcelc.iPcWheeled_IsCollisionCallbackEnabled(*args)
    __swig_setmethods__["WheelMesh"] = fix_args(_blcelc.iPcWheeled_SetWheelMesh)
    WheelMesh = property(None, fix_args(_blcelc.iPcWheeled_SetWheelMesh), None,
                    "iPcWheeled.WheelMesh -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcWheeled::getmethod()\n\tset: void iPcWheeled::SetWheelMesh(...)")

    __swig_setmethods__["TankMode"] = _blcelc.iPcWheeled_TankMode_set
    TankMode = property(_blcelc.iPcWheeled_TankMode_get, _blcelc.iPcWheeled_TankMode_set, None,
                    "iPcWheeled.TankMode -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcWheeled::GetTankMode()\n\tset: void iPcWheeled::SetTankMode(bool)")


    __swig_setmethods__["OuterWheelSteerPreset"] = _blcelc.iPcWheeled_OuterWheelSteerPreset_set
    OuterWheelSteerPreset = property(None, _blcelc.iPcWheeled_OuterWheelSteerPreset_set, None,
                    "iPcWheeled.OuterWheelSteerPreset (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcWheeled::SetOuterWheelSteerPreset(float)")

    __swig_setmethods__["FrontWheelPreset"] = fix_args(_blcelc.iPcWheeled_SetFrontWheelPreset)
    FrontWheelPreset = property(None, fix_args(_blcelc.iPcWheeled_SetFrontWheelPreset), None,
                    "iPcWheeled.FrontWheelPreset -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcWheeled::getmethod()\n\tset: void iPcWheeled::SetFrontWheelPreset(...)")

    __swig_setmethods__["RearWheelPreset"] = fix_args(_blcelc.iPcWheeled_SetRearWheelPreset)
    RearWheelPreset = property(None, fix_args(_blcelc.iPcWheeled_SetRearWheelPreset), None,
                    "iPcWheeled.RearWheelPreset -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcWheeled::getmethod()\n\tset: void iPcWheeled::SetRearWheelPreset(...)")

    __swig_setmethods__["Accelerating"] = _blcelc.iPcWheeled_Accelerating_set
    Accelerating = property(_blcelc.iPcWheeled_Accelerating_get, _blcelc.iPcWheeled_Accelerating_set, None,
                    "iPcWheeled.Accelerating -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcWheeled::IsAccelerating()\n\tset: void iPcWheeled::Accelerate(bool)")


    __swig_setmethods__["Braking"] = _blcelc.iPcWheeled_Braking_set
    Braking = property(_blcelc.iPcWheeled_Braking_get, _blcelc.iPcWheeled_Braking_set, None,
                    "iPcWheeled.Braking -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcWheeled::IsBraking()\n\tset: void iPcWheeled::Brake(bool)")


    __swig_setmethods__["Handbraking"] = _blcelc.iPcWheeled_Handbraking_set
    Handbraking = property(_blcelc.iPcWheeled_Handbraking_get, _blcelc.iPcWheeled_Handbraking_set, None,
                    "iPcWheeled.Handbraking -> bool\n\nThis is equivalent to calling the C++ cs methods:\n\tget: bool iPcWheeled::IsHandbraking()\n\tset: void iPcWheeled::Handbrake(bool)")


    __swig_setmethods__["SteerAmount"] = _blcelc.iPcWheeled_SteerAmount_set
    SteerAmount = property(_blcelc.iPcWheeled_SteerAmount_get, _blcelc.iPcWheeled_SteerAmount_set, None,
                    "iPcWheeled.SteerAmount -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcWheeled::GetSteerAmount()\n\tset: void iPcWheeled::SetSteerAmount(float)")


    __swig_setmethods__["BrakeForce"] = _blcelc.iPcWheeled_BrakeForce_set
    BrakeForce = property(None, _blcelc.iPcWheeled_BrakeForce_set, None,
                    "iPcWheeled.BrakeForce (write only) -> float\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcWheeled::SetBrakeForce(float)")

    __swig_setmethods__["AutoTransmission"] = _blcelc.iPcWheeled_AutoTransmission_set
    AutoTransmission = property(None, _blcelc.iPcWheeled_AutoTransmission_set, None,
                    "iPcWheeled.AutoTransmission (write only) -> bool\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcWheeled::SetAutoTransmission(bool)")

    __swig_setmethods__["Gear"] = fix_args(_blcelc.iPcWheeled_SetGear)
    Gear = property(_blcelc.iPcWheeled_GetGear, fix_args(_blcelc.iPcWheeled_SetGear), None,
                    "iPcWheeled.Gear -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcWheeled::GetGear()\n\tset: void iPcWheeled::SetGear(...)")

    TopGear = property(_blcelc.iPcWheeled_TopGear_get, None, None,
                    "iPcWheeled.TopGear -> int  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: int iPcWheeled::GetTopGear()")

    __swig_setmethods__["AutoReverse"] = _blcelc.iPcWheeled_AutoReverse_set
    AutoReverse = property(None, _blcelc.iPcWheeled_AutoReverse_set, None,
                    "iPcWheeled.AutoReverse (write only) -> bool\n\nWriting to this is equivalent to calling the C++ cel method:\n\tvoid iPcWheeled::SetAutoReverse(bool)")

    BodyGroup = property(_blcelc.iPcWheeled_BodyGroup_get, None, None,
                    "iPcWheeled.BodyGroup -> iBodyGroup*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iBodyGroup* iPcWheeled::GetBodyGroup()")

    WheelCount = property(_blcelc.iPcWheeled_WheelCount_get, None, None,
                    "iPcWheeled.WheelCount -> int  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: int iPcWheeled::GetWheelCount()")

    __swig_destroy__ = _blcelc.delete_iPcWheeled
    __del__ = lambda self : None;
iPcWheeled_swigregister = _blcelc.iPcWheeled_swigregister
iPcWheeled_swigregister(iPcWheeled)

celCreateWheeled = _blcelc.celCreateWheeled
celGetSetWheeled = _blcelc.celGetSetWheeled
celGetWheeled = _blcelc.celGetWheeled
scfQuery_iPcWheeled = _blcelc.scfQuery_iPcWheeled
class iPcDamage(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcDamage, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcDamage, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def SetDamage(*args): return _blcelc.iPcDamage_SetDamage(*args)
    def GetDamage(*args): return _blcelc.iPcDamage_GetDamage(*args)
    def SetDamageType(*args): return _blcelc.iPcDamage_SetDamageType(*args)
    def GetDamageType(*args): return _blcelc.iPcDamage_GetDamageType(*args)
    def SetFallOff(*args): return _blcelc.iPcDamage_SetFallOff(*args)
    def GetFallOff(*args): return _blcelc.iPcDamage_GetFallOff(*args)
    def SetDamageLocation(*args): return _blcelc.iPcDamage_SetDamageLocation(*args)
    def GetDamageSector(*args): return _blcelc.iPcDamage_GetDamageSector(*args)
    def GetDamagePosition(*args): return _blcelc.iPcDamage_GetDamagePosition(*args)
    def SetDamageSource(*args): return _blcelc.iPcDamage_SetDamageSource(*args)
    def GetDamageSource(*args): return _blcelc.iPcDamage_GetDamageSource(*args)
    def AreaDamage(*args): return _blcelc.iPcDamage_AreaDamage(*args)
    def BeamDamage(*args): return _blcelc.iPcDamage_BeamDamage(*args)
    def SingleDamage(*args): return _blcelc.iPcDamage_SingleDamage(*args)
    __swig_setmethods__["Damage"] = _blcelc.iPcDamage_Damage_set
    Damage = property(_blcelc.iPcDamage_Damage_get, _blcelc.iPcDamage_Damage_set, None,
                    "iPcDamage.Damage -> float\n\nThis is equivalent to calling the C++ cs methods:\n\tget: float iPcDamage::GetDamage()\n\tset: void iPcDamage::SetDamage(float)")


    __swig_setmethods__["DamageType"] = _blcelc.iPcDamage_DamageType_set
    DamageType = property(_blcelc.iPcDamage_DamageType_get, _blcelc.iPcDamage_DamageType_set, None,
                    "iPcDamage.DamageType -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iPcDamage::GetDamageType()\n\tset: void iPcDamage::SetDamageType(const char*)")


    __swig_setmethods__["FallOff"] = _blcelc.iPcDamage_FallOff_set
    FallOff = property(_blcelc.iPcDamage_FallOff_get, _blcelc.iPcDamage_FallOff_set, None,
                    "iPcDamage.FallOff -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iPcDamage::GetFallOff()\n\tset: void iPcDamage::SetFallOff(const char*)")


    __swig_setmethods__["DamageLocation"] = fix_args(_blcelc.iPcDamage_SetDamageLocation)
    DamageLocation = property(None, fix_args(_blcelc.iPcDamage_SetDamageLocation), None,
                    "iPcDamage.DamageLocation -> type\n\nThis is equivalent to calling the C++ cs methods:\n\tget: iPcDamage::getmethod()\n\tset: void iPcDamage::SetDamageLocation(...)")

    DamageSector = property(_blcelc.iPcDamage_DamageSector_get, None, None,
                    "iPcDamage.DamageSector -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iPcDamage::GetDamageSector()")

    DamagePosition = property(_blcelc.iPcDamage_DamagePosition_get, None, None,
                    "iPcDamage.DamagePosition -> const csVector3&  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const csVector3& iPcDamage::GetDamagePosition()")

    __swig_setmethods__["DamageSource"] = _blcelc.iPcDamage_DamageSource_set
    DamageSource = property(_blcelc.iPcDamage_DamageSource_get, _blcelc.iPcDamage_DamageSource_set, None,
                    "iPcDamage.DamageSource -> const char*\n\nThis is equivalent to calling the C++ cs methods:\n\tget: const char* iPcDamage::GetDamageSource()\n\tset: void iPcDamage::SetDamageSource(const char*)")


    __swig_destroy__ = _blcelc.delete_iPcDamage
    __del__ = lambda self : None;
iPcDamage_swigregister = _blcelc.iPcDamage_swigregister
iPcDamage_swigregister(iPcDamage)

celCreateDamage = _blcelc.celCreateDamage
celGetSetDamage = _blcelc.celGetSetDamage
celGetDamage = _blcelc.celGetDamage
scfQuery_iPcDamage = _blcelc.scfQuery_iPcDamage
class iCelConsoleCommand(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelConsoleCommand, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelConsoleCommand, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetCommand(*args): return _blcelc.iCelConsoleCommand_GetCommand(*args)
    def GetDescription(*args): return _blcelc.iCelConsoleCommand_GetDescription(*args)
    def Help(*args): return _blcelc.iCelConsoleCommand_Help(*args)
    def Execute(*args): return _blcelc.iCelConsoleCommand_Execute(*args)
    Command = property(_blcelc.iCelConsoleCommand_Command_get, None, None,
                    "iCelConsoleCommand.Command -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelConsoleCommand::GetCommand()")

    Description = property(_blcelc.iCelConsoleCommand_Description_get, None, None,
                    "iCelConsoleCommand.Description -> const char*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: const char* iCelConsoleCommand::GetDescription()")

    __swig_destroy__ = _blcelc.delete_iCelConsoleCommand
    __del__ = lambda self : None;
iCelConsoleCommand_swigregister = _blcelc.iCelConsoleCommand_swigregister
iCelConsoleCommand_swigregister(iCelConsoleCommand)

class iCelConsole(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelConsole, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelConsole, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def GetInputConsole(*args): return _blcelc.iCelConsole_GetInputConsole(*args)
    def GetOutputConsole(*args): return _blcelc.iCelConsole_GetOutputConsole(*args)
    def RegisterCommand(*args): return _blcelc.iCelConsole_RegisterCommand(*args)
    InputConsole = property(_blcelc.iCelConsole_InputConsole_get, None, None,
                    "iCelConsole.InputConsole -> iConsoleInput*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iConsoleInput* iCelConsole::GetInputConsole()")

    OutputConsole = property(_blcelc.iCelConsole_OutputConsole_get, None, None,
                    "iCelConsole.OutputConsole -> iConsoleOutput*  (read-only)\n\nThis is equivalent to calling the C++ cs method:\n\tget: iConsoleOutput* iCelConsole::GetOutputConsole()")

    __swig_destroy__ = _blcelc.delete_iCelConsole
    __del__ = lambda self : None;
iCelConsole_swigregister = _blcelc.iCelConsole_swigregister
iCelConsole_swigregister(iCelConsole)

csQueryRegistry_iCelConsole = _blcelc.csQueryRegistry_iCelConsole
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



