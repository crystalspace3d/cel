# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _blcelc

def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name) or (name == "thisown"):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

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
        return "<%s.%s; proxy of C++ celInitializer instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def __repr__(self):
        return "<%s.%s; proxy of C++ celData instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    __swig_setmethods__["type"] = _blcelc.celData_type_set
    __swig_getmethods__["type"] = _blcelc.celData_type_get
    if _newclass:type = property(_blcelc.celData_type_get, _blcelc.celData_type_set)
    def __init__(self, *args):
        _swig_setattr(self, celData, 'this', _blcelc.new_celData(*args))
        _swig_setattr(self, celData, 'thisown', 1)
    def assign(*args): return _blcelc.celData_assign(*args)
    def __del__(self, destroy=_blcelc.delete_celData):
        try:
            if self.thisown: destroy(self)
        except: pass

    def Clear(*args): return _blcelc.celData_Clear(*args)
    def Set(*args): return _blcelc.celData_Set(*args)
    def SetAction(*args): return _blcelc.celData_SetAction(*args)
    def SetIBase(*args): return _blcelc.celData_SetIBase(*args)
    def SetParameter(*args): return _blcelc.celData_SetParameter(*args)
    __swig_getmethods__["value"] = _blcelc.celData_value_get
    if _newclass:value = property(_blcelc.celData_value_get)

class celDataPtr(celData):
    def __init__(self, this):
        _swig_setattr(self, celData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, celData, 'thisown', 0)
        _swig_setattr(self, celData,self.__class__,celData)
_blcelc.celData_swigregister(celDataPtr)

class celData_value(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, celData_value, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, celData_value, name)
    def __repr__(self):
        return "<%s.%s; proxy of C++ celData_value instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        _swig_setattr(self, celData_value, 'this', _blcelc.new_celData_value(*args))
        _swig_setattr(self, celData_value, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_celData_value):
        try:
            if self.thisown: destroy(self)
        except: pass


class celData_valuePtr(celData_value):
    def __init__(self, this):
        _swig_setattr(self, celData_value, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, celData_value, 'thisown', 0)
        _swig_setattr(self, celData_value,self.__class__,celData_value)
_blcelc.celData_value_swigregister(celData_valuePtr)

class celData_value_par(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, celData_value_par, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, celData_value_par, name)
    def __repr__(self):
        return "<%s.%s; proxy of C++ celData_value_par instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    __swig_setmethods__["parname"] = _blcelc.celData_value_par_parname_set
    __swig_getmethods__["parname"] = _blcelc.celData_value_par_parname_get
    if _newclass:parname = property(_blcelc.celData_value_par_parname_get, _blcelc.celData_value_par_parname_set)
    __swig_setmethods__["partype"] = _blcelc.celData_value_par_partype_set
    __swig_getmethods__["partype"] = _blcelc.celData_value_par_partype_get
    if _newclass:partype = property(_blcelc.celData_value_par_partype_get, _blcelc.celData_value_par_partype_set)
    def __init__(self, *args):
        _swig_setattr(self, celData_value_par, 'this', _blcelc.new_celData_value_par(*args))
        _swig_setattr(self, celData_value_par, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_celData_value_par):
        try:
            if self.thisown: destroy(self)
        except: pass


class celData_value_parPtr(celData_value_par):
    def __init__(self, this):
        _swig_setattr(self, celData_value_par, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, celData_value_par, 'thisown', 0)
        _swig_setattr(self, celData_value_par,self.__class__,celData_value_par)
_blcelc.celData_value_par_swigregister(celData_value_parPtr)

class celData_value_col(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, celData_value_col, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, celData_value_col, name)
    def __repr__(self):
        return "<%s.%s; proxy of C++ celData_value_col instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        _swig_setattr(self, celData_value_col, 'this', _blcelc.new_celData_value_col(*args))
        _swig_setattr(self, celData_value_col, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_celData_value_col):
        try:
            if self.thisown: destroy(self)
        except: pass


class celData_value_colPtr(celData_value_col):
    def __init__(self, this):
        _swig_setattr(self, celData_value_col, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, celData_value_col, 'thisown', 0)
        _swig_setattr(self, celData_value_col,self.__class__,celData_value_col)
_blcelc.celData_value_col_swigregister(celData_value_colPtr)

class celData_value_v(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, celData_value_v, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, celData_value_v, name)
    def __repr__(self):
        return "<%s.%s; proxy of C++ celData_value_v instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        _swig_setattr(self, celData_value_v, 'this', _blcelc.new_celData_value_v(*args))
        _swig_setattr(self, celData_value_v, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_celData_value_v):
        try:
            if self.thisown: destroy(self)
        except: pass


class celData_value_vPtr(celData_value_v):
    def __init__(self, this):
        _swig_setattr(self, celData_value_v, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, celData_value_v, 'thisown', 0)
        _swig_setattr(self, celData_value_v,self.__class__,celData_value_v)
_blcelc.celData_value_v_swigregister(celData_value_vPtr)

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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelNewEntityCallback instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iCelEntityRemoveCallback instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iCelTimerListener instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iCelPlLayer instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_getmethods__["entitytpls"] = lambda self: self.GetEntityTemplates()
    __swig_getmethods__["pcfactories"] = lambda self: self.GetPcFactories()
    __swig_getmethods__["behaviourlayers"] = lambda self: self.GetBehaviourLayers()
    __swig_getmethods__["entities"] = lambda self: self.GetEntities() 
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
        return "<%s.%s; proxy of C++ iCelEntityTracker instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetName(*args): return _blcelc.iCelEntityTracker_GetName(*args)
    def AddEntity(*args): return _blcelc.iCelEntityTracker_AddEntity(*args)
    def RemoveEntity(*args): return _blcelc.iCelEntityTracker_RemoveEntity(*args)
    def RemoveEntities(*args): return _blcelc.iCelEntityTracker_RemoveEntities(*args)
    def FindNearbyEntities(*args): return _blcelc.iCelEntityTracker_FindNearbyEntities(*args)
    __swig_getmethods__["name"] = _blcelc.iCelEntityTracker_GetName  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelEntity instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def CreateBehaviour(*args): return _blcelc.iCelEntity_CreateBehaviour(*args)
    __swig_setmethods__["name"] = _blcelc.iCelEntity_SetName  
    __swig_getmethods__["name"] = _blcelc.iCelEntity_GetName  
    __swig_setmethods__["id"] = _blcelc.iCelEntity_SetID  
    __swig_getmethods__["id"] = _blcelc.iCelEntity_GetID  
    __swig_setmethods__["behaviour"] = _blcelc.iCelEntity_SetBehaviour  
    __swig_getmethods__["behaviour"] = _blcelc.iCelEntity_GetBehaviour  
    __swig_getmethods__["pcclasses"] = _blcelc.iCelEntity_GetPropertyClassList  
    __swig_getmethods__["classes"] = _blcelc.iCelEntity_GetClasses  
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
        return "<%s.%s; proxy of C++ iCelEntityList instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetCount(*args): return _blcelc.iCelEntityList_GetCount(*args)
    def Get(*args): return _blcelc.iCelEntityList_Get(*args)
    def Add(*args): return _blcelc.iCelEntityList_Add(*args)
    def Remove(*args): return _blcelc.iCelEntityList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iCelEntityList_RemoveAll(*args)
    def Find(*args): return _blcelc.iCelEntityList_Find(*args)
    def FindByName(*args): return _blcelc.iCelEntityList_FindByName(*args)
    def __delitem__(*args): return _blcelc.iCelEntityList___delitem__(*args)
    def __len__(*args): return _blcelc.iCelEntityList___len__(*args)
    def append(*args): return _blcelc.iCelEntityList_append(*args)
    def content_iterator(self):
            for idx in xrange(len(self)):
                    yield self.Get(idx)
    def __iter__(self): return self.content_iterator()  
    def __getitem__(*args): return _blcelc.iCelEntityList___getitem__(*args)
    def __contains__(*args): return _blcelc.iCelEntityList___contains__(*args)
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
class iCelEntityTemplate(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityTemplate, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntityTemplate, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelEntityTemplate instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_setmethods__["name"] = _blcelc.iCelEntityTemplate_SetName  
    __swig_getmethods__["name"] = _blcelc.iCelEntityTemplate_GetName  
    __swig_setmethods__["behaviour"] = _blcelc.iCelEntityTemplate_SetBehaviour  
    __swig_getmethods__["behaviour"] = _blcelc.iCelEntityTemplate_GetBehaviour  
    __swig_getmethods__["behaviourlayer"] = _blcelc.iCelEntityTemplate_GetBehaviourLayer  
    def __del__(self, destroy=_blcelc.delete_iCelEntityTemplate):
        try:
            if self.thisown: destroy(self)
        except: pass


class iCelEntityTemplatePtr(iCelEntityTemplate):
    def __init__(self, this):
        _swig_setattr(self, iCelEntityTemplate, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelEntityTemplate, 'thisown', 0)
        _swig_setattr(self, iCelEntityTemplate,self.__class__,iCelEntityTemplate)
_blcelc.iCelEntityTemplate_swigregister(iCelEntityTemplatePtr)

celFindNearbyEntities = _blcelc.celFindNearbyEntities

class iCelPropertyClassTemplate(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClassTemplate, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClassTemplate, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelPropertyClassTemplate instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def SetName(*args): return _blcelc.iCelPropertyClassTemplate_SetName(*args)
    def GetName(*args): return _blcelc.iCelPropertyClassTemplate_GetName(*args)
    def SetTag(*args): return _blcelc.iCelPropertyClassTemplate_SetTag(*args)
    def GetTag(*args): return _blcelc.iCelPropertyClassTemplate_GetTag(*args)
    def SetPropertyVariable(*args): return _blcelc.iCelPropertyClassTemplate_SetPropertyVariable(*args)
    def SetProperty(*args): return _blcelc.iCelPropertyClassTemplate_SetProperty(*args)
    def PerformAction(*args): return _blcelc.iCelPropertyClassTemplate_PerformAction(*args)
    def __del__(self, destroy=_blcelc.delete_iCelPropertyClassTemplate):
        try:
            if self.thisown: destroy(self)
        except: pass


class iCelPropertyClassTemplatePtr(iCelPropertyClassTemplate):
    def __init__(self, this):
        _swig_setattr(self, iCelPropertyClassTemplate, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelPropertyClassTemplate, 'thisown', 0)
        _swig_setattr(self, iCelPropertyClassTemplate,self.__class__,iCelPropertyClassTemplate)
_blcelc.iCelPropertyClassTemplate_swigregister(iCelPropertyClassTemplatePtr)

class iCelBlLayer(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelBlLayer, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelBlLayer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelBlLayer instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iCelParameterBlock instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetParameterCount(*args): return _blcelc.iCelParameterBlock_GetParameterCount(*args)
    def GetParameter(*args): return _blcelc.iCelParameterBlock_GetParameter(*args)
    def GetParameterByIndex(*args): return _blcelc.iCelParameterBlock_GetParameterByIndex(*args)
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
        return "<%s.%s; proxy of C++ iCelBehaviour instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetName(*args): return _blcelc.iCelBehaviour_GetName(*args)
    def GetBehaviourLayer(*args): return _blcelc.iCelBehaviour_GetBehaviourLayer(*args)
    def GetInternalObject(*args): return _blcelc.iCelBehaviour_GetInternalObject(*args)
    def GetPythonObject(*args): return _blcelc.iCelBehaviour_GetPythonObject(*args)
    def SendMessage(*args): return _blcelc.iCelBehaviour_SendMessage(*args)
    __swig_getmethods__["name"] = _blcelc.iCelBehaviour_GetName  
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

class celGenericParameterBlock(iCelParameterBlock):
    __swig_setmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, celGenericParameterBlock, name, value)
    __swig_getmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, celGenericParameterBlock, name)
    def __repr__(self):
        return "<%s.%s; proxy of C++ celGenericParameterBlock instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args):
        _swig_setattr(self, celGenericParameterBlock, 'this', _blcelc.new_celGenericParameterBlock(*args))
        _swig_setattr(self, celGenericParameterBlock, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_celGenericParameterBlock):
        try:
            if self.thisown: destroy(self)
        except: pass

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

class celGenericParameterBlockPtr(celGenericParameterBlock):
    def __init__(self, this):
        _swig_setattr(self, celGenericParameterBlock, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, celGenericParameterBlock, 'thisown', 0)
        _swig_setattr(self, celGenericParameterBlock,self.__class__,celGenericParameterBlock)
_blcelc.celGenericParameterBlock_swigregister(celGenericParameterBlockPtr)

class celVariableParameterBlock(iCelParameterBlock):
    __swig_setmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, celVariableParameterBlock, name, value)
    __swig_getmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, celVariableParameterBlock, name)
    def __repr__(self):
        return "<%s.%s; proxy of C++ celVariableParameterBlock instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args):
        _swig_setattr(self, celVariableParameterBlock, 'this', _blcelc.new_celVariableParameterBlock(*args))
        _swig_setattr(self, celVariableParameterBlock, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_celVariableParameterBlock):
        try:
            if self.thisown: destroy(self)
        except: pass

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

class celVariableParameterBlockPtr(celVariableParameterBlock):
    def __init__(self, this):
        _swig_setattr(self, celVariableParameterBlock, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, celVariableParameterBlock, 'thisown', 0)
        _swig_setattr(self, celVariableParameterBlock,self.__class__,celVariableParameterBlock)
_blcelc.celVariableParameterBlock_swigregister(celVariableParameterBlockPtr)

class celOneParameterBlock(iCelParameterBlock):
    __swig_setmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, celOneParameterBlock, name, value)
    __swig_getmethods__ = {}
    for _s in [iCelParameterBlock]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, celOneParameterBlock, name)
    def __repr__(self):
        return "<%s.%s; proxy of C++ celOneParameterBlock instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args):
        _swig_setattr(self, celOneParameterBlock, 'this', _blcelc.new_celOneParameterBlock(*args))
        _swig_setattr(self, celOneParameterBlock, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_celOneParameterBlock):
        try:
            if self.thisown: destroy(self)
        except: pass

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

class celOneParameterBlockPtr(celOneParameterBlock):
    def __init__(self, this):
        _swig_setattr(self, celOneParameterBlock, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, celOneParameterBlock, 'thisown', 0)
        _swig_setattr(self, celOneParameterBlock,self.__class__,celOneParameterBlock)
_blcelc.celOneParameterBlock_swigregister(celOneParameterBlockPtr)

class iCelPropertyClassFactory(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClassFactory, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClassFactory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelPropertyClassFactory instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetName(*args): return _blcelc.iCelPropertyClassFactory_GetName(*args)
    def CreatePropertyClass(*args): return _blcelc.iCelPropertyClassFactory_CreatePropertyClass(*args)
    __swig_getmethods__["name"] = _blcelc.iCelPropertyClassFactory_GetName  
    def __del__(self, destroy=_blcelc.delete_iCelPropertyClassFactory):
        try:
            if self.thisown: destroy(self)
        except: pass


class iCelPropertyClassFactoryPtr(iCelPropertyClassFactory):
    def __init__(self, this):
        _swig_setattr(self, iCelPropertyClassFactory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelPropertyClassFactory, 'thisown', 0)
        _swig_setattr(self, iCelPropertyClassFactory,self.__class__,iCelPropertyClassFactory)
_blcelc.iCelPropertyClassFactory_swigregister(iCelPropertyClassFactoryPtr)

class iCelPropertyClass(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClass, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClass, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelPropertyClass instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def SetPropertyLong(*args): return _blcelc.iCelPropertyClass_SetPropertyLong(*args)
    def SetPropertyFloat(*args): return _blcelc.iCelPropertyClass_SetPropertyFloat(*args)
    def SetPropertyBool(*args): return _blcelc.iCelPropertyClass_SetPropertyBool(*args)
    def SetPropertyString(*args): return _blcelc.iCelPropertyClass_SetPropertyString(*args)
    def SetPropertyVector3(*args): return _blcelc.iCelPropertyClass_SetPropertyVector3(*args)
    __swig_getmethods__["name"] = _blcelc.iCelPropertyClass_GetName  
    __swig_setmethods__["tag"] = _blcelc.iCelPropertyClass_SetTag  
    __swig_getmethods__["tag"] = _blcelc.iCelPropertyClass_GetTag  
    __swig_setmethods__["entity"] = _blcelc.iCelPropertyClass_SetEntity  
    __swig_getmethods__["entity"] = _blcelc.iCelPropertyClass_GetEntity  
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
        return "<%s.%s; proxy of C++ iCelPropertyClassList instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def __delitem__(*args): return _blcelc.iCelPropertyClassList___delitem__(*args)
    def __len__(*args): return _blcelc.iCelPropertyClassList___len__(*args)
    def append(*args): return _blcelc.iCelPropertyClassList_append(*args)
    def content_iterator(self):
            for idx in xrange(len(self)):
                    yield self.Get(idx)
    def __iter__(self): return self.content_iterator()  
    def __getitem__(*args): return _blcelc.iCelPropertyClassList___getitem__(*args)
    def __contains__(*args): return _blcelc.iCelPropertyClassList___contains__(*args)
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

class iPcMechanicsSystem(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsSystem, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcMechanicsSystem instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_setmethods__["dynsys"] = _blcelc.iPcMechanicsSystem_SetDynamicSystem  
    __swig_getmethods__["dynsys"] = _blcelc.iPcMechanicsSystem_GetDynamicSystem  
    __swig_setmethods__["steptime"] = _blcelc.iPcMechanicsSystem_SetStepTime  
    __swig_getmethods__["steptime"] = _blcelc.iPcMechanicsSystem_GetStepTime  
    __swig_setmethods__["gravity"] = _blcelc.iPcMechanicsSystem_SetGravity  
    __swig_getmethods__["gravity"] = _blcelc.iPcMechanicsSystem_GetGravity  
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
        return "<%s.%s; proxy of C++ iPcMechanicsObject instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_setmethods__["mesh"] = _blcelc.iPcMechanicsObject_SetMesh  
    __swig_getmethods__["mesh"] = _blcelc.iPcMechanicsObject_GetMesh  
    __swig_setmethods__["light"] = _blcelc.iPcMechanicsObject_SetLight  
    __swig_getmethods__["light"] = _blcelc.iPcMechanicsObject_GetLight  
    __swig_setmethods__["camera"] = _blcelc.iPcMechanicsObject_SetCamera  
    __swig_getmethods__["camera"] = _blcelc.iPcMechanicsObject_GetCamera  
    __swig_setmethods__["mechsys"] = _blcelc.iPcMechanicsObject_SetMechanicsSystem  
    __swig_getmethods__["mechsys"] = _blcelc.iPcMechanicsObject_GetMechanicsSystem  
    __swig_getmethods__["body"] = _blcelc.iPcMechanicsObject_GetBody  
    __swig_setmethods__["friction"] = _blcelc.iPcMechanicsObject_SetFriction  
    __swig_getmethods__["friction"] = _blcelc.iPcMechanicsObject_GetFriction  
    __swig_setmethods__["mass"] = _blcelc.iPcMechanicsObject_SetMass  
    __swig_getmethods__["mass"] = _blcelc.iPcMechanicsObject_GetMass  
    __swig_setmethods__["elasticity"] = _blcelc.iPcMechanicsObject_SetElasticity  
    __swig_getmethods__["elasticity"] = _blcelc.iPcMechanicsObject_GetElasticity  
    __swig_setmethods__["softness"] = _blcelc.iPcMechanicsObject_SetSoftness  
    __swig_getmethods__["softness"] = _blcelc.iPcMechanicsObject_GetSoftness  
    __swig_setmethods__["drag"] = _blcelc.iPcMechanicsObject_SetDrag  
    __swig_getmethods__["drag"] = _blcelc.iPcMechanicsObject_GetDrag  
    __swig_setmethods__["lift"] = _blcelc.iPcMechanicsObject_SetLift  
    __swig_getmethods__["lift"] = _blcelc.iPcMechanicsObject_GetLift  
    __swig_setmethods__["linearvelocity"] = _blcelc.iPcMechanicsObject_SetLinearVelocity  
    __swig_getmethods__["linearvelocity"] = _blcelc.iPcMechanicsObject_GetLinearVelocity  
    __swig_setmethods__["angularvelocity"] = _blcelc.iPcMechanicsObject_SetAngularVelocity  
    __swig_getmethods__["angularvelocity"] = _blcelc.iPcMechanicsObject_GetAngularVelocity  
    __swig_setmethods__["static"] = _blcelc.iPcMechanicsObject_MakeStatic  
    __swig_getmethods__["static"] = _blcelc.iPcMechanicsObject_IsStatic  
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

class iPcMechanicsJoint(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsJoint, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsJoint, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcMechanicsJoint instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetJoint(*args): return _blcelc.iPcMechanicsJoint_GetJoint(*args)
    __swig_getmethods__["joint"] = _blcelc.iPcMechanicsJoint_GetJoint  
    def __del__(self, destroy=_blcelc.delete_iPcMechanicsJoint):
        try:
            if self.thisown: destroy(self)
        except: pass


class iPcMechanicsJointPtr(iPcMechanicsJoint):
    def __init__(self, this):
        _swig_setattr(self, iPcMechanicsJoint, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMechanicsJoint, 'thisown', 0)
        _swig_setattr(self, iPcMechanicsJoint,self.__class__,iPcMechanicsJoint)
_blcelc.iPcMechanicsJoint_swigregister(iPcMechanicsJointPtr)


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

celCreateMechanicsJoint = _blcelc.celCreateMechanicsJoint

celGetSetMechanicsJoint = _blcelc.celGetSetMechanicsJoint

celGetMechanicsJoint = _blcelc.celGetMechanicsJoint

scfQuery_iPcMechanicsJoint = _blcelc.scfQuery_iPcMechanicsJoint

scfQueryPC_iPcMechanicsJoint = _blcelc.scfQueryPC_iPcMechanicsJoint
class iPcMechanicsThruster(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMechanicsThruster, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMechanicsThruster, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcMechanicsThruster instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iPcMechanicsBalancedGroup instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iPcMechanicsThrusterController instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iBillboardLayer instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iBillboardEventHandler instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def Select(*args): return _blcelc.iBillboardEventHandler_Select(*args)
    def MouseMove(*args): return _blcelc.iBillboardEventHandler_MouseMove(*args)
    def MouseMoveAway(*args): return _blcelc.iBillboardEventHandler_MouseMoveAway(*args)
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
CEL_BILLBOARD_SENDMOVE = _blcelc.CEL_BILLBOARD_SENDMOVE
class iBillboard(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboard, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboard, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iBillboard instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iBillboardManager instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iPcBillboard instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def SetBillboardName(*args): return _blcelc.iPcBillboard_SetBillboardName(*args)
    def GetBillboardName(*args): return _blcelc.iPcBillboard_GetBillboardName(*args)
    def GetBillboard(*args): return _blcelc.iPcBillboard_GetBillboard(*args)
    def EnableEvents(*args): return _blcelc.iPcBillboard_EnableEvents(*args)
    def AreEventsEnabled(*args): return _blcelc.iPcBillboard_AreEventsEnabled(*args)
    __swig_setmethods__["name"] = _blcelc.iPcBillboard_SetBillboardName  
    __swig_getmethods__["name"] = _blcelc.iPcBillboard_GetBillboardName  
    __swig_setmethods__["events"] = _blcelc.iPcBillboard_EnableEvents  
    __swig_getmethods__["events"] = _blcelc.iPcBillboard_AreEventsEnabled  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcRegion instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_setmethods__["name"] = _blcelc.iPcRegion_SetRegionName  
    __swig_getmethods__["name"] = _blcelc.iPcRegion_GetRegionName  
    __swig_getmethods__["region"] = _blcelc.iPcRegion_GetRegion  
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
        return "<%s.%s; proxy of C++ iCelMapFile instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def SetPath(*args): return _blcelc.iCelMapFile_SetPath(*args)
    def SetFile(*args): return _blcelc.iCelMapFile_SetFile(*args)
    def GetPath(*args): return _blcelc.iCelMapFile_GetPath(*args)
    def GetFile(*args): return _blcelc.iCelMapFile_GetFile(*args)
    def SetSectorName(*args): return _blcelc.iCelMapFile_SetSectorName(*args)
    def GetSectorName(*args): return _blcelc.iCelMapFile_GetSectorName(*args)
    __swig_setmethods__["path"] = _blcelc.iCelMapFile_SetPath  
    __swig_getmethods__["path"] = _blcelc.iCelMapFile_GetPath  
    __swig_setmethods__["file"] = _blcelc.iCelMapFile_SetFile  
    __swig_getmethods__["file"] = _blcelc.iCelMapFile_GetFile  
    __swig_setmethods__["sectorname"] = _blcelc.iCelMapFile_SetSectorName  
    __swig_getmethods__["sectorname"] = _blcelc.iCelMapFile_GetSectorName  
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
        return "<%s.%s; proxy of C++ iCelRegion instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetName(*args): return _blcelc.iCelRegion_GetName(*args)
    def SetCachePath(*args): return _blcelc.iCelRegion_SetCachePath(*args)
    def GetCachePath(*args): return _blcelc.iCelRegion_GetCachePath(*args)
    def CreateMapFile(*args): return _blcelc.iCelRegion_CreateMapFile(*args)
    def GetMapFileCount(*args): return _blcelc.iCelRegion_GetMapFileCount(*args)
    def GetMapFile(*args): return _blcelc.iCelRegion_GetMapFile(*args)
    def RemoveMapFile(*args): return _blcelc.iCelRegion_RemoveMapFile(*args)
    def RemoveAllMapFiles(*args): return _blcelc.iCelRegion_RemoveAllMapFiles(*args)
    def AssociateEntity(*args): return _blcelc.iCelRegion_AssociateEntity(*args)
    def DissociateEntity(*args): return _blcelc.iCelRegion_DissociateEntity(*args)
    __swig_getmethods__["name"] = _blcelc.iCelRegion_GetName  
    __swig_setmethods__["cachepath"] = _blcelc.iCelRegion_SetCachePath  
    __swig_getmethods__["cachepath"] = _blcelc.iCelRegion_GetCachePath  
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
        return "<%s.%s; proxy of C++ iCelZone instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetName(*args): return _blcelc.iCelZone_GetName(*args)
    def LinkRegion(*args): return _blcelc.iCelZone_LinkRegion(*args)
    def GetRegionCount(*args): return _blcelc.iCelZone_GetRegionCount(*args)
    def GetRegion(*args): return _blcelc.iCelZone_GetRegion(*args)
    def FindRegion(*args): return _blcelc.iCelZone_FindRegion(*args)
    def UnlinkRegion(*args): return _blcelc.iCelZone_UnlinkRegion(*args)
    def UnlinkAllRegions(*args): return _blcelc.iCelZone_UnlinkAllRegions(*args)
    __swig_getmethods__["name"] = _blcelc.iCelZone_GetName  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcZoneManager instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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

celGetSetZoneManager = _blcelc.celGetSetZoneManager

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
        return "<%s.%s; proxy of C++ iPcCommandInput instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def Activate(*args): return _blcelc.iPcCommandInput_Activate(*args)
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
    __swig_setmethods__["cookedmode"] = _blcelc.iPcCommandInput_SetCookedMode  
    __swig_getmethods__["cookedmode"] = _blcelc.iPcCommandInput_GetCookedMode  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcGravityCallback instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iPcLinearMovement instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_setmethods__["anchor"] = _blcelc.iPcLinearMovement_SetAnchor  
    __swig_getmethods__["anchor"] = _blcelc.iPcLinearMovement_GetAnchor  
    __swig_setmethods__["angularvelocity"] = _blcelc.iPcLinearMovement_SetAngularVelocity  
    __swig_getmethods__["angularvelocity"] = _blcelc.iPcLinearMovement_GetAngularVelocity  
    __swig_setmethods__["velocity"] = _blcelc.iPcLinearMovement_SetVelocity  
    __swig_getmethods__["velocity"] = _blcelc.iPcLinearMovement_GetVelocity  
    __swig_setmethods__["path"] = _blcelc.iPcLinearMovement_SetPath  
    __swig_getmethods__["path"] = _blcelc.iPcLinearMovement_IsPath  
    __swig_getmethods__["sector"] = _blcelc.iPcLinearMovement_GetSector  
    __swig_setmethods__["onground"] = _blcelc.iPcLinearMovement_SetOnGround  
    __swig_getmethods__["onground"] = _blcelc.iPcLinearMovement_IsOnGround  
    __swig_setmethods__["gravity"] = _blcelc.iPcLinearMovement_SetGravity  
    __swig_getmethods__["gravity"] = _blcelc.iPcLinearMovement_GetGravity  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcActorMove instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def IsMoveMoveEnabled(*args): return _blcelc.iPcActorMove_IsMoveMoveEnabled(*args)
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
    __swig_setmethods__["forward"] = _blcelc.iPcActorMove_Forward  
    __swig_getmethods__["forward"] = _blcelc.iPcActorMove_IsMovingForward  
    __swig_setmethods__["backward"] = _blcelc.iPcActorMove_Backward  
    __swig_getmethods__["backward"] = _blcelc.iPcActorMove_IsMovingBackward  
    __swig_setmethods__["strafeleft"] = _blcelc.iPcActorMove_StrafeLeft  
    __swig_getmethods__["strafeleft"] = _blcelc.iPcActorMove_IsStrafingLeft  
    __swig_setmethods__["straferight"] = _blcelc.iPcActorMove_StrafeRight  
    __swig_getmethods__["straferight"] = _blcelc.iPcActorMove_IsStrafingRight  
    __swig_setmethods__["rotateleft"] = _blcelc.iPcActorMove_RotateLeft  
    __swig_getmethods__["rotateleft"] = _blcelc.iPcActorMove_IsRotatingLeft  
    __swig_setmethods__["rotateright"] = _blcelc.iPcActorMove_RotateRight  
    __swig_getmethods__["rotateright"] = _blcelc.iPcActorMove_IsRotatingRight  
    __swig_setmethods__["run"] = _blcelc.iPcActorMove_Run  
    __swig_getmethods__["run"] = _blcelc.iPcActorMove_IsRunning  
    __swig_setmethods__["autorun"] = _blcelc.iPcActorMove_AutoRun  
    __swig_getmethods__["autorun"] = _blcelc.iPcActorMove_IsAutoRunning  
    __swig_setmethods__["movementspeed"] = _blcelc.iPcActorMove_SetMovementSpeed  
    __swig_getmethods__["movementspeed"] = _blcelc.iPcActorMove_GetMovementSpeed  
    __swig_setmethods__["runningspeed"] = _blcelc.iPcActorMove_SetRunningSpeed  
    __swig_getmethods__["runningspeed"] = _blcelc.iPcActorMove_GetRunningSpeed  
    __swig_setmethods__["rotationspeed"] = _blcelc.iPcActorMove_SetRotationSpeed  
    __swig_getmethods__["rotationspeed"] = _blcelc.iPcActorMove_GetRotationSpeed  
    __swig_setmethods__["jumpingvelocity"] = _blcelc.iPcActorMove_SetJumpingVelocity  
    __swig_getmethods__["jumpingvelocity"] = _blcelc.iPcActorMove_GetJumpingVelocity  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcCamera instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_getmethods__["camera"] = _blcelc.iPcCamera_GetCamera  
    __swig_getmethods__["view"] = _blcelc.iPcCamera_GetView  
    __swig_setmethods__["clearzbuffer"] = _blcelc.iPcCamera_SetClearZBuffer  
    __swig_getmethods__["clearzbuffer"] = _blcelc.iPcCamera_GetClearZBuffer  
    __swig_setmethods__["clearscreen"] = _blcelc.iPcCamera_SetClearScreen  
    __swig_getmethods__["clearscreen"] = _blcelc.iPcCamera_GetClearScreen  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcDefaultCamera instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_setmethods__["yaw"] = _blcelc.iPcDefaultCamera_SetYaw  
    __swig_getmethods__["yaw"] = _blcelc.iPcDefaultCamera_GetYaw  
    __swig_setmethods__["pitch"] = _blcelc.iPcDefaultCamera_SetPitch  
    __swig_getmethods__["pitch"] = _blcelc.iPcDefaultCamera_GetPitch  
    __swig_setmethods__["yaw_vel"] = _blcelc.iPcDefaultCamera_SetYawVelocity  
    __swig_getmethods__["yaw_vel"] = _blcelc.iPcDefaultCamera_GetYawVelocity  
    __swig_setmethods__["pitch_vel"] = _blcelc.iPcDefaultCamera_SetPitchVelocity  
    __swig_getmethods__["pitch_vel"] = _blcelc.iPcDefaultCamera_GetPitchVelocity  
    __swig_setmethods__["mode"] = _blcelc.iPcDefaultCamera_SetModeName  
    __swig_getmethods__["mode"] = _blcelc.iPcDefaultCamera_GetModeName  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcSimpleCamera instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def SetDrawMesh(*args): return _blcelc.iPcSimpleCamera_SetDrawMesh(*args)
    def SetCameraOffset(*args): return _blcelc.iPcSimpleCamera_SetCameraOffset(*args)
    def SetLookAtOffset(*args): return _blcelc.iPcSimpleCamera_SetLookAtOffset(*args)
    def SetMesh(*args): return _blcelc.iPcSimpleCamera_SetMesh(*args)
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcMeshSelectListener instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
        return "<%s.%s; proxy of C++ iPcMeshSelect instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_setmethods__["mousebuttons"] = _blcelc.iPcMeshSelect_SetMouseButtons  
    __swig_getmethods__["mousebuttons"] = _blcelc.iPcMeshSelect_GetMouseButtons  
    __swig_setmethods__["global"] = _blcelc.iPcMeshSelect_SetGlobalSelection  
    __swig_getmethods__["global"] = _blcelc.iPcMeshSelect_HasGlobalSelection  
    __swig_setmethods__["follow"] = _blcelc.iPcMeshSelect_SetFollowMode  
    __swig_getmethods__["follow"] = _blcelc.iPcMeshSelect_HasFollowMode  
    __swig_setmethods__["followalways"] = _blcelc.iPcMeshSelect_SetFollowAlwaysMode  
    __swig_getmethods__["followalways"] = _blcelc.iPcMeshSelect_HasFollowAlwaysMode  
    __swig_setmethods__["drag"] = _blcelc.iPcMeshSelect_SetDragMode  
    __swig_getmethods__["drag"] = _blcelc.iPcMeshSelect_HasDragMode  
    __swig_setmethods__["dragnormal"] = _blcelc.iPcMeshSelect_SetDragPlaneNormal  
    __swig_getmethods__["dragnormal"] = _blcelc.iPcMeshSelect_GetDragPlaneNormal  
    __swig_setmethods__["moveevent"] = _blcelc.iPcMeshSelect_SetSendmoveEvent  
    __swig_getmethods__["moveevent"] = _blcelc.iPcMeshSelect_HasSendmoveEvent  
    __swig_setmethods__["moveup"] = _blcelc.iPcMeshSelect_SetSendupEvent  
    __swig_getmethods__["moveup"] = _blcelc.iPcMeshSelect_HasSendupEvent  
    __swig_setmethods__["movedown"] = _blcelc.iPcMeshSelect_SetSenddownEvent  
    __swig_getmethods__["movedown"] = _blcelc.iPcMeshSelect_HasSenddownEvent  
    __swig_setmethods__["distance"] = _blcelc.iPcMeshSelect_SetMaxSelectionDistance  
    __swig_getmethods__["distance"] = _blcelc.iPcMeshSelect_GetMaxSelectionDistance  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcMesh instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def SetPath(*args): return _blcelc.iPcMesh_SetPath(*args)
    def LoadMesh(*args): return _blcelc.iPcMesh_LoadMesh(*args)
    def CreateEmptyThing(*args): return _blcelc.iPcMesh_CreateEmptyThing(*args)
    def GetMesh(*args): return _blcelc.iPcMesh_GetMesh(*args)
    def MoveMesh(*args): return _blcelc.iPcMesh_MoveMesh(*args)
    def SetAction(*args): return _blcelc.iPcMesh_SetAction(*args)
    def GetAction(*args): return _blcelc.iPcMesh_GetAction(*args)
    def SetReverseAction(*args): return _blcelc.iPcMesh_SetReverseAction(*args)
    def SetAnimation(*args): return _blcelc.iPcMesh_SetAnimation(*args)
    def Hide(*args): return _blcelc.iPcMesh_Hide(*args)
    def Show(*args): return _blcelc.iPcMesh_Show(*args)
    def IsVisible(*args): return _blcelc.iPcMesh_IsVisible(*args)
    __swig_getmethods__["mesh"] = _blcelc.iPcMesh_GetMesh  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcTimer instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcProjectile instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def Start(*args): return _blcelc.iPcProjectile_Start(*args)
    def Interrupt(*args): return _blcelc.iPcProjectile_Interrupt(*args)
    def IsMoving(*args): return _blcelc.iPcProjectile_IsMoving(*args)
    __swig_getmethods__["moving"] = _blcelc.iPcProjectile_IsMoving  
    def __del__(self, destroy=_blcelc.delete_iPcProjectile):
        try:
            if self.thisown: destroy(self)
        except: pass


class iPcProjectilePtr(iPcProjectile):
    def __init__(self, this):
        _swig_setattr(self, iPcProjectile, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcProjectile, 'thisown', 0)
        _swig_setattr(self, iPcProjectile,self.__class__,iPcProjectile)
_blcelc.iPcProjectile_swigregister(iPcProjectilePtr)


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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcSolid instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def SetMesh(*args): return _blcelc.iPcSolid_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcSolid_GetMesh(*args)
    def GetCollider(*args): return _blcelc.iPcSolid_GetCollider(*args)
    __swig_getmethods__["collider"] = _blcelc.iPcSolid_GetCollider  
    __swig_setmethods__["mesh"] = _blcelc.iPcSolid_SetMesh  
    __swig_getmethods__["mesh"] = _blcelc.iPcSolid_GetMesh  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcGravity instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    __swig_getmethods__["gravitycollider"] = _blcelc.iPcGravity_GetGravityCollider  
    __swig_setmethods__["solid"] = _blcelc.iPcGravity_SetSolid  
    __swig_getmethods__["solid"] = _blcelc.iPcGravity_GetSolid  
    __swig_setmethods__["weight"] = _blcelc.iPcGravity_SetWeight  
    __swig_getmethods__["weight"] = _blcelc.iPcGravity_GetWeight  
    __swig_setmethods__["active"] = _blcelc.iPcGravity_SetActive  
    __swig_getmethods__["active"] = _blcelc.iPcGravity_IsActive  
    __swig_setmethods__["movable"] = _blcelc.iPcGravity_SetMovable  
    __swig_getmethods__["movable"] = _blcelc.iPcGravity_GetMovable  
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcMovable instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def SetMesh(*args): return _blcelc.iPcMovable_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcMovable_GetMesh(*args)
    def SetPos(*args): return _blcelc.iPcMovable_SetPos(*args)
    def AddConstraint(*args): return _blcelc.iPcMovable_AddConstraint(*args)
    def RemoveConstraint(*args): return _blcelc.iPcMovable_RemoveConstraint(*args)
    def RemoveAllConstraints(*args): return _blcelc.iPcMovable_RemoveAllConstraints(*args)
    __swig_setmethods__["mesh"] = _blcelc.iPcMovable_SetMesh  
    __swig_getmethods__["mesh"] = _blcelc.iPcMovable_GetMesh  
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
        return "<%s.%s; proxy of C++ iPcMovableConstraint instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcInventoryListener instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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

class iCelInventorySpaceSlot(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelInventorySpaceSlot, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelInventorySpaceSlot, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelInventorySpaceSlot instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetEntity(*args): return _blcelc.iCelInventorySpaceSlot_GetEntity(*args)
    def AddEntity(*args): return _blcelc.iCelInventorySpaceSlot_AddEntity(*args)
    def RemoveEntity(*args): return _blcelc.iCelInventorySpaceSlot_RemoveEntity(*args)
    def GetCount(*args): return _blcelc.iCelInventorySpaceSlot_GetCount(*args)
    def RemoveAll(*args): return _blcelc.iCelInventorySpaceSlot_RemoveAll(*args)
    def IsEmpty(*args): return _blcelc.iCelInventorySpaceSlot_IsEmpty(*args)
    def __del__(self, destroy=_blcelc.delete_iCelInventorySpaceSlot):
        try:
            if self.thisown: destroy(self)
        except: pass


class iCelInventorySpaceSlotPtr(iCelInventorySpaceSlot):
    def __init__(self, this):
        _swig_setattr(self, iCelInventorySpaceSlot, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelInventorySpaceSlot, 'thisown', 0)
        _swig_setattr(self, iCelInventorySpaceSlot,self.__class__,iCelInventorySpaceSlot)
_blcelc.iCelInventorySpaceSlot_swigregister(iCelInventorySpaceSlotPtr)

class iCelInventorySpace(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelInventorySpace, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelInventorySpace, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelInventorySpace instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetSpaceTotal(*args): return _blcelc.iCelInventorySpace_GetSpaceTotal(*args)
    def GetSpaceLeft(*args): return _blcelc.iCelInventorySpace_GetSpaceLeft(*args)
    def GetSpaceTaken(*args): return _blcelc.iCelInventorySpace_GetSpaceTaken(*args)
    def AddEntity(*args): return _blcelc.iCelInventorySpace_AddEntity(*args)
    def RemoveEntity(*args): return _blcelc.iCelInventorySpace_RemoveEntity(*args)
    def RemoveAll(*args): return _blcelc.iCelInventorySpace_RemoveAll(*args)
    def GetSlot(*args): return _blcelc.iCelInventorySpace_GetSlot(*args)
    def GetEntity(*args): return _blcelc.iCelInventorySpace_GetEntity(*args)
    def __del__(self, destroy=_blcelc.delete_iCelInventorySpace):
        try:
            if self.thisown: destroy(self)
        except: pass


class iCelInventorySpacePtr(iCelInventorySpace):
    def __init__(self, this):
        _swig_setattr(self, iCelInventorySpace, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelInventorySpace, 'thisown', 0)
        _swig_setattr(self, iCelInventorySpace,self.__class__,iCelInventorySpace)
_blcelc.iCelInventorySpace_swigregister(iCelInventorySpacePtr)

class iPcInventory(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcInventory, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcInventory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcInventory instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def GetEntities(self): return iCelEntityInvFakeArray(self)
    __swig_getmethods__["entities"] = lambda self: self.GetEntities() 
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcCharacteristics instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcTooltip instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcSoundListener instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetSoundListener(*args): return _blcelc.iPcSoundListener_GetSoundListener(*args)
    def __del__(self, destroy=_blcelc.delete_iPcSoundListener):
        try:
            if self.thisown: destroy(self)
        except: pass


class iPcSoundListenerPtr(iPcSoundListener):
    def __init__(self, this):
        _swig_setattr(self, iPcSoundListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcSoundListener, 'thisown', 0)
        _swig_setattr(self, iPcSoundListener,self.__class__,iPcSoundListener)
_blcelc.iPcSoundListener_swigregister(iPcSoundListenerPtr)

class iPcSoundSource(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcSoundSource, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcSoundSource, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcSoundSource instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetSoundSource(*args): return _blcelc.iPcSoundSource_GetSoundSource(*args)
    def SetSoundName(*args): return _blcelc.iPcSoundSource_SetSoundName(*args)
    def GetSoundName(*args): return _blcelc.iPcSoundSource_GetSoundName(*args)
    def __del__(self, destroy=_blcelc.delete_iPcSoundSource):
        try:
            if self.thisown: destroy(self)
        except: pass


class iPcSoundSourcePtr(iPcSoundSource):
    def __init__(self, this):
        _swig_setattr(self, iPcSoundSource, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcSoundSource, 'thisown', 0)
        _swig_setattr(self, iPcSoundSource,self.__class__,iPcSoundSource)
_blcelc.iPcSoundSource_swigregister(iPcSoundSourcePtr)


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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcPropertyListener instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def PropertyChanged(*args): return _blcelc.iPcPropertyListener_PropertyChanged(*args)
    def __del__(self, destroy=_blcelc.delete_iPcPropertyListener):
        try:
            if self.thisown: destroy(self)
        except: pass


class iPcPropertyListenerPtr(iPcPropertyListener):
    def __init__(self, this):
        _swig_setattr(self, iPcPropertyListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcPropertyListener, 'thisown', 0)
        _swig_setattr(self, iPcPropertyListener,self.__class__,iPcPropertyListener)
_blcelc.iPcPropertyListener_swigregister(iPcPropertyListenerPtr)

class iPcProperties(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcProperties, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcProperties, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcProperties instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def __del__(self, destroy=_blcelc.delete_iPcProperties):
        try:
            if self.thisown: destroy(self)
        except: pass


class iPcPropertiesPtr(iPcProperties):
    def __init__(self, this):
        _swig_setattr(self, iPcProperties, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcProperties, 'thisown', 0)
        _swig_setattr(self, iPcProperties,self.__class__,iPcProperties)
_blcelc.iPcProperties_swigregister(iPcPropertiesPtr)


celCreateProperties = _blcelc.celCreateProperties

celGetSetProperties = _blcelc.celGetSetProperties

celGetProperties = _blcelc.celGetProperties

scfQuery_iPcProperties = _blcelc.scfQuery_iPcProperties
class iPcHover(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcHover, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcHover, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcHover instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def SetWorldMesh(*args): return _blcelc.iPcHover_SetWorldMesh(*args)
    def SetWorld(*args): return _blcelc.iPcHover_SetWorld(*args)
    def SetHeightBeamCutoff(*args): return _blcelc.iPcHover_SetHeightBeamCutoff(*args)
    def SetAngularBeamOffset(*args): return _blcelc.iPcHover_SetAngularBeamOffset(*args)
    def SetAngularCutoffHeight(*args): return _blcelc.iPcHover_SetAngularCutoffHeight(*args)
    def SetAngularCorrectionStrength(*args): return _blcelc.iPcHover_SetAngularCorrectionStrength(*args)
    def HoverOn(*args): return _blcelc.iPcHover_HoverOn(*args)
    def HoverOff(*args): return _blcelc.iPcHover_HoverOff(*args)
    def SetStabiliserFunction(*args): return _blcelc.iPcHover_SetStabiliserFunction(*args)
    def UseDefaultFunction(*args): return _blcelc.iPcHover_UseDefaultFunction(*args)
    def GetHeight(*args): return _blcelc.iPcHover_GetHeight(*args)
    def __del__(self, destroy=_blcelc.delete_iPcHover):
        try:
            if self.thisown: destroy(self)
        except: pass


class iPcHoverPtr(iPcHover):
    def __init__(self, this):
        _swig_setattr(self, iPcHover, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcHover, 'thisown', 0)
        _swig_setattr(self, iPcHover,self.__class__,iPcHover)
_blcelc.iPcHover_swigregister(iPcHoverPtr)


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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcCraftController instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def __del__(self, destroy=_blcelc.delete_iPcCraftController):
        try:
            if self.thisown: destroy(self)
        except: pass


class iPcCraftControllerPtr(iPcCraftController):
    def __init__(self, this):
        _swig_setattr(self, iPcCraftController, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcCraftController, 'thisown', 0)
        _swig_setattr(self, iPcCraftController,self.__class__,iPcCraftController)
_blcelc.iPcCraftController_swigregister(iPcCraftControllerPtr)


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
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iPcWheeled instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
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
    def SetWheelSuspensionSoftness(*args): return _blcelc.iPcWheeled_SetWheelSuspensionSoftness(*args)
    def SetWheelSuspensionDamping(*args): return _blcelc.iPcWheeled_SetWheelSuspensionDamping(*args)
    def SetWheelLeftSteerSensitivity(*args): return _blcelc.iPcWheeled_SetWheelLeftSteerSensitivity(*args)
    def SetWheelRightSteerSensitivity(*args): return _blcelc.iPcWheeled_SetWheelRightSteerSensitivity(*args)
    def SetWheelTurnSpeed(*args): return _blcelc.iPcWheeled_SetWheelTurnSpeed(*args)
    def SetWheelReturnSpeed(*args): return _blcelc.iPcWheeled_SetWheelReturnSpeed(*args)
    def SetWheelEnginePower(*args): return _blcelc.iPcWheeled_SetWheelEnginePower(*args)
    def SetWheelBrakePower(*args): return _blcelc.iPcWheeled_SetWheelBrakePower(*args)
    def SetWheelSteerInverted(*args): return _blcelc.iPcWheeled_SetWheelSteerInverted(*args)
    def SetWheelHandbrakeAffected(*args): return _blcelc.iPcWheeled_SetWheelHandbrakeAffected(*args)
    def GetWheelPosition(*args): return _blcelc.iPcWheeled_GetWheelPosition(*args)
    def GetWheelSuspensionSoftness(*args): return _blcelc.iPcWheeled_GetWheelSuspensionSoftness(*args)
    def GetWheelSuspensionDamping(*args): return _blcelc.iPcWheeled_GetWheelSuspensionDamping(*args)
    def GetWheelLeftSteerSensitivity(*args): return _blcelc.iPcWheeled_GetWheelLeftSteerSensitivity(*args)
    def GetWheelRightSteerSensitivity(*args): return _blcelc.iPcWheeled_GetWheelRightSteerSensitivity(*args)
    def GetWheelTurnSpeed(*args): return _blcelc.iPcWheeled_GetWheelTurnSpeed(*args)
    def GetWheelReturnSpeed(*args): return _blcelc.iPcWheeled_GetWheelReturnSpeed(*args)
    def GetWheelEnginePower(*args): return _blcelc.iPcWheeled_GetWheelEnginePower(*args)
    def GetWheelBrakePower(*args): return _blcelc.iPcWheeled_GetWheelBrakePower(*args)
    def GetWheelSteerInverted(*args): return _blcelc.iPcWheeled_GetWheelSteerInverted(*args)
    def GetWheelHandbrakeAffected(*args): return _blcelc.iPcWheeled_GetWheelHandbrakeAffected(*args)
    def __del__(self, destroy=_blcelc.delete_iPcWheeled):
        try:
            if self.thisown: destroy(self)
        except: pass


class iPcWheeledPtr(iPcWheeled):
    def __init__(self, this):
        _swig_setattr(self, iPcWheeled, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcWheeled, 'thisown', 0)
        _swig_setattr(self, iPcWheeled,self.__class__,iPcWheeled)
_blcelc.iPcWheeled_swigregister(iPcWheeledPtr)


celCreateWheeled = _blcelc.celCreateWheeled

celGetSetWheeled = _blcelc.celGetSetWheeled

celGetWheeled = _blcelc.celGetWheeled

scfQuery_iPcWheeled = _blcelc.scfQuery_iPcWheeled
class iCelConsoleCommand(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelConsoleCommand, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelConsoleCommand, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelConsoleCommand instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetCommand(*args): return _blcelc.iCelConsoleCommand_GetCommand(*args)
    def GetDescription(*args): return _blcelc.iCelConsoleCommand_GetDescription(*args)
    def Help(*args): return _blcelc.iCelConsoleCommand_Help(*args)
    def Execute(*args): return _blcelc.iCelConsoleCommand_Execute(*args)
    def __del__(self, destroy=_blcelc.delete_iCelConsoleCommand):
        try:
            if self.thisown: destroy(self)
        except: pass


class iCelConsoleCommandPtr(iCelConsoleCommand):
    def __init__(self, this):
        _swig_setattr(self, iCelConsoleCommand, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelConsoleCommand, 'thisown', 0)
        _swig_setattr(self, iCelConsoleCommand,self.__class__,iCelConsoleCommand)
_blcelc.iCelConsoleCommand_swigregister(iCelConsoleCommandPtr)

class iCelConsole(cspace.iBase):
    __swig_setmethods__ = {}
    for _s in [cspace.iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelConsole, name, value)
    __swig_getmethods__ = {}
    for _s in [cspace.iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelConsole, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ iCelConsole instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def GetInputConsole(*args): return _blcelc.iCelConsole_GetInputConsole(*args)
    def GetOutputConsole(*args): return _blcelc.iCelConsole_GetOutputConsole(*args)
    def RegisterCommand(*args): return _blcelc.iCelConsole_RegisterCommand(*args)
    def __del__(self, destroy=_blcelc.delete_iCelConsole):
        try:
            if self.thisown: destroy(self)
        except: pass


class iCelConsolePtr(iCelConsole):
    def __init__(self, this):
        _swig_setattr(self, iCelConsole, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCelConsole, 'thisown', 0)
        _swig_setattr(self, iCelConsole,self.__class__,iCelConsole)
_blcelc.iCelConsole_swigregister(iCelConsolePtr)


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


