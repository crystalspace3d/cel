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


class csWrapPtr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csWrapPtr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csWrapPtr, name)
    def __repr__(self):
        return "<C csWrapPtr instance at %s>" % (self.this,)
    __swig_setmethods__["Ref"] = _blcelc.csWrapPtr_Ref_set
    __swig_getmethods__["Ref"] = _blcelc.csWrapPtr_Ref_get
    if _newclass:Ref = property(_blcelc.csWrapPtr_Ref_get, _blcelc.csWrapPtr_Ref_set)
    __swig_setmethods__["VoidPtr"] = _blcelc.csWrapPtr_VoidPtr_set
    __swig_getmethods__["VoidPtr"] = _blcelc.csWrapPtr_VoidPtr_get
    if _newclass:VoidPtr = property(_blcelc.csWrapPtr_VoidPtr_get, _blcelc.csWrapPtr_VoidPtr_set)
    __swig_getmethods__["Type"] = _blcelc.csWrapPtr_Type_get
    if _newclass:Type = property(_blcelc.csWrapPtr_Type_get)
    def __init__(self, *args):
        _swig_setattr(self, csWrapPtr, 'this', _blcelc.new_csWrapPtr(*args))
        _swig_setattr(self, csWrapPtr, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csWrapPtr):
        try:
            if self.thisown: destroy(self)
        except: pass

class csWrapPtrPtr(csWrapPtr):
    def __init__(self, this):
        _swig_setattr(self, csWrapPtr, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csWrapPtr, 'thisown', 0)
        _swig_setattr(self, csWrapPtr,self.__class__,csWrapPtr)
_blcelc.csWrapPtr_swigregister(csWrapPtrPtr)

CS_VOIDED_PTR = _blcelc.CS_VOIDED_PTR
class iBase(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBase, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, iBase, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBase instance at %s>" % (self.this,)
    def IncRef(*args): return _blcelc.iBase_IncRef(*args)
    def DecRef(*args): return _blcelc.iBase_DecRef(*args)
    def GetRefCount(*args): return _blcelc.iBase_GetRefCount(*args)
    def QueryInterface(*args): return _blcelc.iBase_QueryInterface(*args)
    __swig_getmethods__["QueryInterfaceSafe"] = lambda x: _blcelc.iBase_QueryInterfaceSafe
    if _newclass:QueryInterfaceSafe = staticmethod(_blcelc.iBase_QueryInterfaceSafe)
    def AddRefOwner(*args): return _blcelc.iBase_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.iBase_RemoveRefOwner(*args)
    def __del__(self, destroy=_blcelc.delete_iBase):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iBase_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iBase_scfGetVersion)
    def _DynamicCast(*args): return _blcelc.iBase__DynamicCast(*args)

class iBasePtr(iBase):
    def __init__(self, this):
        _swig_setattr(self, iBase, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBase, 'thisown', 0)
        _swig_setattr(self, iBase,self.__class__,iBase)
_blcelc.iBase_swigregister(iBasePtr)
cvar = _blcelc.cvar
csArrayItemNotFound = cvar.csArrayItemNotFound

iBase_QueryInterfaceSafe = _blcelc.iBase_QueryInterfaceSafe

iBase_scfGetVersion = _blcelc.iBase_scfGetVersion

SCF_STATIC_CLASS_CONTEXT = _blcelc.SCF_STATIC_CLASS_CONTEXT
class iFactory(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iFactory, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iFactory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFactory instance at %s>" % (self.this,)
    def CreateInstance(*args): return _blcelc.iFactory_CreateInstance(*args)
    def TryUnload(*args): return _blcelc.iFactory_TryUnload(*args)
    def QueryDescription(*args): return _blcelc.iFactory_QueryDescription(*args)
    def QueryDependencies(*args): return _blcelc.iFactory_QueryDependencies(*args)
    def QueryClassID(*args): return _blcelc.iFactory_QueryClassID(*args)
    def QueryModuleName(*args): return _blcelc.iFactory_QueryModuleName(*args)
    def __del__(self, destroy=_blcelc.delete_iFactory):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFactory_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFactory_scfGetVersion)

class iFactoryPtr(iFactory):
    def __init__(self, this):
        _swig_setattr(self, iFactory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iFactory, 'thisown', 0)
        _swig_setattr(self, iFactory,self.__class__,iFactory)
_blcelc.iFactory_swigregister(iFactoryPtr)

iFactory_scfGetVersion = _blcelc.iFactory_scfGetVersion


scfCompatibleVersion = _blcelc.scfCompatibleVersion
class iSCF(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSCF, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSCF, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSCF instance at %s>" % (self.this,)
    def RegisterClasses(*args): return _blcelc.iSCF_RegisterClasses(*args)
    def ClassRegistered(*args): return _blcelc.iSCF_ClassRegistered(*args)
    def CreateInstance(*args): return _blcelc.iSCF_CreateInstance(*args)
    def GetClassDescription(*args): return _blcelc.iSCF_GetClassDescription(*args)
    def GetClassDependencies(*args): return _blcelc.iSCF_GetClassDependencies(*args)
    def GetPluginMetadata(*args): return _blcelc.iSCF_GetPluginMetadata(*args)
    def UnloadUnusedModules(*args): return _blcelc.iSCF_UnloadUnusedModules(*args)
    def RegisterClass(*args): return _blcelc.iSCF_RegisterClass(*args)
    def RegisterFactoryFunc(*args): return _blcelc.iSCF_RegisterFactoryFunc(*args)
    def UnregisterClass(*args): return _blcelc.iSCF_UnregisterClass(*args)
    def GetInterfaceName(*args): return _blcelc.iSCF_GetInterfaceName(*args)
    def GetInterfaceID(*args): return _blcelc.iSCF_GetInterfaceID(*args)
    def Finish(*args): return _blcelc.iSCF_Finish(*args)
    def QueryClassList(*args): return _blcelc.iSCF_QueryClassList(*args)
    def ScanPluginsPath(*args): return _blcelc.iSCF_ScanPluginsPath(*args)
    def RegisterPlugin(*args): return _blcelc.iSCF_RegisterPlugin(*args)
    def __del__(self, destroy=_blcelc.delete_iSCF):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSCF_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSCF_scfGetVersion)

class iSCFPtr(iSCF):
    def __init__(self, this):
        _swig_setattr(self, iSCF, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSCF, 'thisown', 0)
        _swig_setattr(self, iSCF,self.__class__,iSCF)
_blcelc.iSCF_swigregister(iSCFPtr)

iSCF_scfGetVersion = _blcelc.iSCF_scfGetVersion

CS_DBGHELP_UNITTEST = _blcelc.CS_DBGHELP_UNITTEST
CS_DBGHELP_BENCHMARK = _blcelc.CS_DBGHELP_BENCHMARK
CS_DBGHELP_TXTDUMP = _blcelc.CS_DBGHELP_TXTDUMP
CS_DBGHELP_GFXDUMP = _blcelc.CS_DBGHELP_GFXDUMP
CS_DBGHELP_STATETEST = _blcelc.CS_DBGHELP_STATETEST
class iDebugHelper(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDebugHelper, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDebugHelper, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDebugHelper instance at %s>" % (self.this,)
    def GetSupportedTests(*args): return _blcelc.iDebugHelper_GetSupportedTests(*args)
    def UnitTest(*args): return _blcelc.iDebugHelper_UnitTest(*args)
    def StateTest(*args): return _blcelc.iDebugHelper_StateTest(*args)
    def Benchmark(*args): return _blcelc.iDebugHelper_Benchmark(*args)
    def Dump(*args): return _blcelc.iDebugHelper_Dump(*args)
    def DebugCommand(*args): return _blcelc.iDebugHelper_DebugCommand(*args)
    def __del__(self, destroy=_blcelc.delete_iDebugHelper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDebugHelper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDebugHelper_scfGetVersion)

class iDebugHelperPtr(iDebugHelper):
    def __init__(self, this):
        _swig_setattr(self, iDebugHelper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDebugHelper, 'thisown', 0)
        _swig_setattr(self, iDebugHelper,self.__class__,iDebugHelper)
_blcelc.iDebugHelper_swigregister(iDebugHelperPtr)

iDebugHelper_scfGetVersion = _blcelc.iDebugHelper_scfGetVersion

class csColor(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csColor, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csColor, name)
    def __repr__(self):
        return "<C csColor instance at %s>" % (self.this,)
    __swig_setmethods__["red"] = _blcelc.csColor_red_set
    __swig_getmethods__["red"] = _blcelc.csColor_red_get
    if _newclass:red = property(_blcelc.csColor_red_get, _blcelc.csColor_red_set)
    __swig_setmethods__["green"] = _blcelc.csColor_green_set
    __swig_getmethods__["green"] = _blcelc.csColor_green_get
    if _newclass:green = property(_blcelc.csColor_green_get, _blcelc.csColor_green_set)
    __swig_setmethods__["blue"] = _blcelc.csColor_blue_set
    __swig_getmethods__["blue"] = _blcelc.csColor_blue_get
    if _newclass:blue = property(_blcelc.csColor_blue_get, _blcelc.csColor_blue_set)
    def __init__(self, *args):
        _swig_setattr(self, csColor, 'this', _blcelc.new_csColor(*args))
        _swig_setattr(self, csColor, 'thisown', 1)
    def Set(*args): return _blcelc.csColor_Set(*args)
    def Clamp(*args): return _blcelc.csColor_Clamp(*args)
    def ClampDown(*args): return _blcelc.csColor_ClampDown(*args)
    def assign(*args): return _blcelc.csColor_assign(*args)
    def __imul__(*args): return _blcelc.csColor___imul__(*args)
    def __iadd__(*args): return _blcelc.csColor___iadd__(*args)
    def __isub__(*args): return _blcelc.csColor___isub__(*args)
    def __eq__(*args): return _blcelc.csColor___eq__(*args)
    def __ne__(*args): return _blcelc.csColor___ne__(*args)
    def Add(*args): return _blcelc.csColor_Add(*args)
    def Subtract(*args): return _blcelc.csColor_Subtract(*args)
    def __add__(*args): return _blcelc.csColor___add__(*args)
    def __sub__(*args): return _blcelc.csColor___sub__(*args)
    def __mul__(*args): return _blcelc.csColor___mul__(*args)
    def __rmul__(*args): return _blcelc.csColor___rmul__(*args)
    def __del__(self, destroy=_blcelc.delete_csColor):
        try:
            if self.thisown: destroy(self)
        except: pass

class csColorPtr(csColor):
    def __init__(self, this):
        _swig_setattr(self, csColor, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csColor, 'thisown', 0)
        _swig_setattr(self, csColor,self.__class__,csColor)
_blcelc.csColor_swigregister(csColorPtr)

class csColor4(csColor):
    __swig_setmethods__ = {}
    for _s in [csColor]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csColor4, name, value)
    __swig_getmethods__ = {}
    for _s in [csColor]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csColor4, name)
    def __repr__(self):
        return "<C csColor4 instance at %s>" % (self.this,)
    __swig_setmethods__["alpha"] = _blcelc.csColor4_alpha_set
    __swig_getmethods__["alpha"] = _blcelc.csColor4_alpha_get
    if _newclass:alpha = property(_blcelc.csColor4_alpha_get, _blcelc.csColor4_alpha_set)
    def __init__(self, *args):
        _swig_setattr(self, csColor4, 'this', _blcelc.new_csColor4(*args))
        _swig_setattr(self, csColor4, 'thisown', 1)
    def Set(*args): return _blcelc.csColor4_Set(*args)
    def assign(*args): return _blcelc.csColor4_assign(*args)
    def __imul__(*args): return _blcelc.csColor4___imul__(*args)
    def __iadd__(*args): return _blcelc.csColor4___iadd__(*args)
    def __isub__(*args): return _blcelc.csColor4___isub__(*args)
    def __eq__(*args): return _blcelc.csColor4___eq__(*args)
    def __ne__(*args): return _blcelc.csColor4___ne__(*args)
    def __del__(self, destroy=_blcelc.delete_csColor4):
        try:
            if self.thisown: destroy(self)
        except: pass

class csColor4Ptr(csColor4):
    def __init__(self, this):
        _swig_setattr(self, csColor4, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csColor4, 'thisown', 0)
        _swig_setattr(self, csColor4,self.__class__,csColor4)
_blcelc.csColor4_swigregister(csColor4Ptr)

class csCommandLineHelper(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csCommandLineHelper, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csCommandLineHelper, name)
    def __repr__(self):
        return "<C csCommandLineHelper instance at %s>" % (self.this,)
    __swig_getmethods__["Help"] = lambda x: _blcelc.csCommandLineHelper_Help
    if _newclass:Help = staticmethod(_blcelc.csCommandLineHelper_Help)
    __swig_getmethods__["CheckHelp"] = lambda x: _blcelc.csCommandLineHelper_CheckHelp
    if _newclass:CheckHelp = staticmethod(_blcelc.csCommandLineHelper_CheckHelp)
    def __init__(self, *args):
        _swig_setattr(self, csCommandLineHelper, 'this', _blcelc.new_csCommandLineHelper(*args))
        _swig_setattr(self, csCommandLineHelper, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csCommandLineHelper):
        try:
            if self.thisown: destroy(self)
        except: pass

class csCommandLineHelperPtr(csCommandLineHelper):
    def __init__(self, this):
        _swig_setattr(self, csCommandLineHelper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csCommandLineHelper, 'thisown', 0)
        _swig_setattr(self, csCommandLineHelper,self.__class__,csCommandLineHelper)
_blcelc.csCommandLineHelper_swigregister(csCommandLineHelperPtr)

csCommandLineHelper_Help = _blcelc.csCommandLineHelper_Help

csCommandLineHelper_CheckHelp = _blcelc.csCommandLineHelper_CheckHelp

class csStringSetIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csStringSetIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csStringSetIterator, name)
    def __repr__(self):
        return "<C csStringSetIterator instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csStringSetIterator, 'this', _blcelc.new_csStringSetIterator(*args))
        _swig_setattr(self, csStringSetIterator, 'thisown', 1)
    def HasNext(*args): return _blcelc.csStringSetIterator_HasNext(*args)
    def Next(*args): return _blcelc.csStringSetIterator_Next(*args)
    def __del__(self, destroy=_blcelc.delete_csStringSetIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class csStringSetIteratorPtr(csStringSetIterator):
    def __init__(self, this):
        _swig_setattr(self, csStringSetIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csStringSetIterator, 'thisown', 0)
        _swig_setattr(self, csStringSetIterator,self.__class__,csStringSetIterator)
_blcelc.csStringSetIterator_swigregister(csStringSetIteratorPtr)

class csStringSet(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csStringSet, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csStringSet, name)
    def __repr__(self):
        return "<C csStringSet instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csStringSet, 'this', _blcelc.new_csStringSet(*args))
        _swig_setattr(self, csStringSet, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csStringSet):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Request(*args): return _blcelc.csStringSet_Request(*args)
    def Contains(*args): return _blcelc.csStringSet_Contains(*args)
    def Clear(*args): return _blcelc.csStringSet_Clear(*args)

class csStringSetPtr(csStringSet):
    def __init__(self, this):
        _swig_setattr(self, csStringSet, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csStringSet, 'thisown', 0)
        _swig_setattr(self, csStringSet,self.__class__,csStringSet)
_blcelc.csStringSet_swigregister(csStringSetPtr)

class iString(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iString, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iString, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iString instance at %s>" % (self.this,)
    def GetData(*args): return _blcelc.iString_GetData(*args)
    def __ne__(*args): return _blcelc.iString___ne__(*args)
    def __del__(self, destroy=_blcelc.delete_iString):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iString_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iString_scfGetVersion)
    def __getitem__(*args): return _blcelc.iString___getitem__(*args)
    def __setitem__(*args): return _blcelc.iString___setitem__(*args)

class iStringPtr(iString):
    def __init__(self, this):
        _swig_setattr(self, iString, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iString, 'thisown', 0)
        _swig_setattr(self, iString,self.__class__,iString)
_blcelc.iString_swigregister(iStringPtr)

iString_scfGetVersion = _blcelc.iString_scfGetVersion

class csString(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csString, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csString, name)
    def __repr__(self):
        return "<C csString instance at %s>" % (self.this,)
    def GetData(*args): return _blcelc.csString_GetData(*args)
    def GetDataSafe(*args): return _blcelc.csString_GetDataSafe(*args)
    def Length(*args): return _blcelc.csString_Length(*args)
    def IsEmpty(*args): return _blcelc.csString_IsEmpty(*args)
    def FindStr(*args): return _blcelc.csString_FindStr(*args)
    def FindReplace(*args): return _blcelc.csString_FindReplace(*args)
    def StartsWith(*args): return _blcelc.csString_StartsWith(*args)
    def __init__(self, *args):
        _swig_setattr(self, csString, 'this', _blcelc.new_csString(*args))
        _swig_setattr(self, csString, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csString):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __lt__(*args): return _blcelc.csString___lt__(*args)
    def __gt__(*args): return _blcelc.csString___gt__(*args)
    def __getitem__(*args): return _blcelc.csString___getitem__(*args)
    def __setitem__(*args): return _blcelc.csString___setitem__(*args)
    def __delitem__(*args): return _blcelc.csString___delitem__(*args)

class csStringPtr(csString):
    def __init__(self, this):
        _swig_setattr(self, csString, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csString, 'thisown', 0)
        _swig_setattr(self, csString,self.__class__,csString)
_blcelc.csString_swigregister(csStringPtr)

class csVector2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csVector2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csVector2, name)
    def __repr__(self):
        return "<C csVector2 instance at %s>" % (self.this,)
    __swig_setmethods__["x"] = _blcelc.csVector2_x_set
    __swig_getmethods__["x"] = _blcelc.csVector2_x_get
    if _newclass:x = property(_blcelc.csVector2_x_get, _blcelc.csVector2_x_set)
    __swig_setmethods__["y"] = _blcelc.csVector2_y_set
    __swig_getmethods__["y"] = _blcelc.csVector2_y_get
    if _newclass:y = property(_blcelc.csVector2_y_get, _blcelc.csVector2_y_set)
    def __init__(self, *args):
        _swig_setattr(self, csVector2, 'this', _blcelc.new_csVector2(*args))
        _swig_setattr(self, csVector2, 'thisown', 1)
    def Set(*args): return _blcelc.csVector2_Set(*args)
    def Get(*args): return _blcelc.csVector2_Get(*args)
    def Norm(*args): return _blcelc.csVector2_Norm(*args)
    def SquaredNorm(*args): return _blcelc.csVector2_SquaredNorm(*args)
    def Rotate(*args): return _blcelc.csVector2_Rotate(*args)
    def IsLeft(*args): return _blcelc.csVector2_IsLeft(*args)
    def __iadd__(*args): return _blcelc.csVector2___iadd__(*args)
    def __isub__(*args): return _blcelc.csVector2___isub__(*args)
    def __imul__(*args): return _blcelc.csVector2___imul__(*args)
    def __idiv__(*args): return _blcelc.csVector2___idiv__(*args)
    def __pos__(*args): return _blcelc.csVector2___pos__(*args)
    def __neg__(*args): return _blcelc.csVector2___neg__(*args)
    def __add__(*args): return _blcelc.csVector2___add__(*args)
    def __sub__(*args): return _blcelc.csVector2___sub__(*args)
    def __mul__(*args): return _blcelc.csVector2___mul__(*args)
    def __div__(*args): return _blcelc.csVector2___div__(*args)
    def __eq__(*args): return _blcelc.csVector2___eq__(*args)
    def __ne__(*args): return _blcelc.csVector2___ne__(*args)
    def __lt__(*args): return _blcelc.csVector2___lt__(*args)
    def __gt__(*args): return _blcelc.csVector2___gt__(*args)
    def __rmul__(*args): return _blcelc.csVector2___rmul__(*args)
    def __abs__(*args): return _blcelc.csVector2___abs__(*args)
    def __getitem__(*args): return _blcelc.csVector2___getitem__(*args)
    def __setitem__(*args): return _blcelc.csVector2___setitem__(*args)
    def __del__(self, destroy=_blcelc.delete_csVector2):
        try:
            if self.thisown: destroy(self)
        except: pass

class csVector2Ptr(csVector2):
    def __init__(self, this):
        _swig_setattr(self, csVector2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csVector2, 'thisown', 0)
        _swig_setattr(self, csVector2,self.__class__,csVector2)
_blcelc.csVector2_swigregister(csVector2Ptr)

class csVector3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csVector3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csVector3, name)
    def __repr__(self):
        return "<C csVector3 instance at %s>" % (self.this,)
    __swig_setmethods__["x"] = _blcelc.csVector3_x_set
    __swig_getmethods__["x"] = _blcelc.csVector3_x_get
    if _newclass:x = property(_blcelc.csVector3_x_get, _blcelc.csVector3_x_set)
    __swig_setmethods__["y"] = _blcelc.csVector3_y_set
    __swig_getmethods__["y"] = _blcelc.csVector3_y_get
    if _newclass:y = property(_blcelc.csVector3_y_get, _blcelc.csVector3_y_set)
    __swig_setmethods__["z"] = _blcelc.csVector3_z_set
    __swig_getmethods__["z"] = _blcelc.csVector3_z_get
    if _newclass:z = property(_blcelc.csVector3_z_get, _blcelc.csVector3_z_set)
    def __init__(self, *args):
        _swig_setattr(self, csVector3, 'this', _blcelc.new_csVector3(*args))
        _swig_setattr(self, csVector3, 'thisown', 1)
    def Cross(*args): return _blcelc.csVector3_Cross(*args)
    def __iadd__(*args): return _blcelc.csVector3___iadd__(*args)
    def __isub__(*args): return _blcelc.csVector3___isub__(*args)
    def __pos__(*args): return _blcelc.csVector3___pos__(*args)
    def __neg__(*args): return _blcelc.csVector3___neg__(*args)
    def Set(*args): return _blcelc.csVector3_Set(*args)
    def Get(*args): return _blcelc.csVector3_Get(*args)
    def Norm(*args): return _blcelc.csVector3_Norm(*args)
    def SquaredNorm(*args): return _blcelc.csVector3_SquaredNorm(*args)
    def Unit(*args): return _blcelc.csVector3_Unit(*args)
    def Normalize(*args): return _blcelc.csVector3_Normalize(*args)
    def IsZero(*args): return _blcelc.csVector3_IsZero(*args)
    def __add__(*args): return _blcelc.csVector3___add__(*args)
    def __sub__(*args): return _blcelc.csVector3___sub__(*args)
    def __mul__(*args): return _blcelc.csVector3___mul__(*args)
    def __eq__(*args): return _blcelc.csVector3___eq__(*args)
    def __ne__(*args): return _blcelc.csVector3___ne__(*args)
    def __lt__(*args): return _blcelc.csVector3___lt__(*args)
    def __gt__(*args): return _blcelc.csVector3___gt__(*args)
    def __imul__(*args): return _blcelc.csVector3___imul__(*args)
    def __idiv__(*args): return _blcelc.csVector3___idiv__(*args)
    def __div__(*args): return _blcelc.csVector3___div__(*args)
    def project(*args): return _blcelc.csVector3_project(*args)
    def __rmul__(*args): return _blcelc.csVector3___rmul__(*args)
    def __abs__(*args): return _blcelc.csVector3___abs__(*args)
    def __getitem__(*args): return _blcelc.csVector3___getitem__(*args)
    def __setitem__(*args): return _blcelc.csVector3___setitem__(*args)
    def __nonzero__(*args): return _blcelc.csVector3___nonzero__(*args)
    def __del__(self, destroy=_blcelc.delete_csVector3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csVector3Ptr(csVector3):
    def __init__(self, this):
        _swig_setattr(self, csVector3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csVector3, 'thisown', 0)
        _swig_setattr(self, csVector3,self.__class__,csVector3)
_blcelc.csVector3_swigregister(csVector3Ptr)

class csMatrix2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csMatrix2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csMatrix2, name)
    def __repr__(self):
        return "<C csMatrix2 instance at %s>" % (self.this,)
    __swig_setmethods__["m11"] = _blcelc.csMatrix2_m11_set
    __swig_getmethods__["m11"] = _blcelc.csMatrix2_m11_get
    if _newclass:m11 = property(_blcelc.csMatrix2_m11_get, _blcelc.csMatrix2_m11_set)
    __swig_setmethods__["m12"] = _blcelc.csMatrix2_m12_set
    __swig_getmethods__["m12"] = _blcelc.csMatrix2_m12_get
    if _newclass:m12 = property(_blcelc.csMatrix2_m12_get, _blcelc.csMatrix2_m12_set)
    __swig_setmethods__["m21"] = _blcelc.csMatrix2_m21_set
    __swig_getmethods__["m21"] = _blcelc.csMatrix2_m21_get
    if _newclass:m21 = property(_blcelc.csMatrix2_m21_get, _blcelc.csMatrix2_m21_set)
    __swig_setmethods__["m22"] = _blcelc.csMatrix2_m22_set
    __swig_getmethods__["m22"] = _blcelc.csMatrix2_m22_get
    if _newclass:m22 = property(_blcelc.csMatrix2_m22_get, _blcelc.csMatrix2_m22_set)
    def __init__(self, *args):
        _swig_setattr(self, csMatrix2, 'this', _blcelc.new_csMatrix2(*args))
        _swig_setattr(self, csMatrix2, 'thisown', 1)
    def Row1(*args): return _blcelc.csMatrix2_Row1(*args)
    def Row2(*args): return _blcelc.csMatrix2_Row2(*args)
    def Col1(*args): return _blcelc.csMatrix2_Col1(*args)
    def Col2(*args): return _blcelc.csMatrix2_Col2(*args)
    def Set(*args): return _blcelc.csMatrix2_Set(*args)
    def __iadd__(*args): return _blcelc.csMatrix2___iadd__(*args)
    def __isub__(*args): return _blcelc.csMatrix2___isub__(*args)
    def __imul__(*args): return _blcelc.csMatrix2___imul__(*args)
    def __idiv__(*args): return _blcelc.csMatrix2___idiv__(*args)
    def __pos__(*args): return _blcelc.csMatrix2___pos__(*args)
    def __neg__(*args): return _blcelc.csMatrix2___neg__(*args)
    def Transpose(*args): return _blcelc.csMatrix2_Transpose(*args)
    def GetTranspose(*args): return _blcelc.csMatrix2_GetTranspose(*args)
    def GetInverse(*args): return _blcelc.csMatrix2_GetInverse(*args)
    def Invert(*args): return _blcelc.csMatrix2_Invert(*args)
    def Determinant(*args): return _blcelc.csMatrix2_Determinant(*args)
    def Identity(*args): return _blcelc.csMatrix2_Identity(*args)
    def __del__(self, destroy=_blcelc.delete_csMatrix2):
        try:
            if self.thisown: destroy(self)
        except: pass

class csMatrix2Ptr(csMatrix2):
    def __init__(self, this):
        _swig_setattr(self, csMatrix2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csMatrix2, 'thisown', 0)
        _swig_setattr(self, csMatrix2,self.__class__,csMatrix2)
_blcelc.csMatrix2_swigregister(csMatrix2Ptr)

class csMatrix3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csMatrix3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csMatrix3, name)
    def __repr__(self):
        return "<C csMatrix3 instance at %s>" % (self.this,)
    __swig_setmethods__["m11"] = _blcelc.csMatrix3_m11_set
    __swig_getmethods__["m11"] = _blcelc.csMatrix3_m11_get
    if _newclass:m11 = property(_blcelc.csMatrix3_m11_get, _blcelc.csMatrix3_m11_set)
    __swig_setmethods__["m12"] = _blcelc.csMatrix3_m12_set
    __swig_getmethods__["m12"] = _blcelc.csMatrix3_m12_get
    if _newclass:m12 = property(_blcelc.csMatrix3_m12_get, _blcelc.csMatrix3_m12_set)
    __swig_setmethods__["m13"] = _blcelc.csMatrix3_m13_set
    __swig_getmethods__["m13"] = _blcelc.csMatrix3_m13_get
    if _newclass:m13 = property(_blcelc.csMatrix3_m13_get, _blcelc.csMatrix3_m13_set)
    __swig_setmethods__["m21"] = _blcelc.csMatrix3_m21_set
    __swig_getmethods__["m21"] = _blcelc.csMatrix3_m21_get
    if _newclass:m21 = property(_blcelc.csMatrix3_m21_get, _blcelc.csMatrix3_m21_set)
    __swig_setmethods__["m22"] = _blcelc.csMatrix3_m22_set
    __swig_getmethods__["m22"] = _blcelc.csMatrix3_m22_get
    if _newclass:m22 = property(_blcelc.csMatrix3_m22_get, _blcelc.csMatrix3_m22_set)
    __swig_setmethods__["m23"] = _blcelc.csMatrix3_m23_set
    __swig_getmethods__["m23"] = _blcelc.csMatrix3_m23_get
    if _newclass:m23 = property(_blcelc.csMatrix3_m23_get, _blcelc.csMatrix3_m23_set)
    __swig_setmethods__["m31"] = _blcelc.csMatrix3_m31_set
    __swig_getmethods__["m31"] = _blcelc.csMatrix3_m31_get
    if _newclass:m31 = property(_blcelc.csMatrix3_m31_get, _blcelc.csMatrix3_m31_set)
    __swig_setmethods__["m32"] = _blcelc.csMatrix3_m32_set
    __swig_getmethods__["m32"] = _blcelc.csMatrix3_m32_get
    if _newclass:m32 = property(_blcelc.csMatrix3_m32_get, _blcelc.csMatrix3_m32_set)
    __swig_setmethods__["m33"] = _blcelc.csMatrix3_m33_set
    __swig_getmethods__["m33"] = _blcelc.csMatrix3_m33_get
    if _newclass:m33 = property(_blcelc.csMatrix3_m33_get, _blcelc.csMatrix3_m33_set)
    def __init__(self, *args):
        _swig_setattr(self, csMatrix3, 'this', _blcelc.new_csMatrix3(*args))
        _swig_setattr(self, csMatrix3, 'thisown', 1)
    def Row1(*args): return _blcelc.csMatrix3_Row1(*args)
    def Row2(*args): return _blcelc.csMatrix3_Row2(*args)
    def Row3(*args): return _blcelc.csMatrix3_Row3(*args)
    def Col1(*args): return _blcelc.csMatrix3_Col1(*args)
    def Col2(*args): return _blcelc.csMatrix3_Col2(*args)
    def Col3(*args): return _blcelc.csMatrix3_Col3(*args)
    def Set(*args): return _blcelc.csMatrix3_Set(*args)
    def assign(*args): return _blcelc.csMatrix3_assign(*args)
    def __iadd__(*args): return _blcelc.csMatrix3___iadd__(*args)
    def __isub__(*args): return _blcelc.csMatrix3___isub__(*args)
    def __idiv__(*args): return _blcelc.csMatrix3___idiv__(*args)
    def __pos__(*args): return _blcelc.csMatrix3___pos__(*args)
    def __neg__(*args): return _blcelc.csMatrix3___neg__(*args)
    def Transpose(*args): return _blcelc.csMatrix3_Transpose(*args)
    def GetTranspose(*args): return _blcelc.csMatrix3_GetTranspose(*args)
    def GetInverse(*args): return _blcelc.csMatrix3_GetInverse(*args)
    def Invert(*args): return _blcelc.csMatrix3_Invert(*args)
    def Determinant(*args): return _blcelc.csMatrix3_Determinant(*args)
    def Identity(*args): return _blcelc.csMatrix3_Identity(*args)
    def IsIdentity(*args): return _blcelc.csMatrix3_IsIdentity(*args)
    def __add__(*args): return _blcelc.csMatrix3___add__(*args)
    def __sub__(*args): return _blcelc.csMatrix3___sub__(*args)
    def __div__(*args): return _blcelc.csMatrix3___div__(*args)
    def __eq__(*args): return _blcelc.csMatrix3___eq__(*args)
    def __ne__(*args): return _blcelc.csMatrix3___ne__(*args)
    def __lt__(*args): return _blcelc.csMatrix3___lt__(*args)
    def __mul__(*args): return _blcelc.csMatrix3___mul__(*args)
    def __imul__(*args): return _blcelc.csMatrix3___imul__(*args)
    def __rmul__(*args): return _blcelc.csMatrix3___rmul__(*args)
    def __del__(self, destroy=_blcelc.delete_csMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csMatrix3Ptr(csMatrix3):
    def __init__(self, this):
        _swig_setattr(self, csMatrix3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csMatrix3, 'thisown', 0)
        _swig_setattr(self, csMatrix3,self.__class__,csMatrix3)
_blcelc.csMatrix3_swigregister(csMatrix3Ptr)

class csXRotMatrix3(csMatrix3):
    __swig_setmethods__ = {}
    for _s in [csMatrix3]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csXRotMatrix3, name, value)
    __swig_getmethods__ = {}
    for _s in [csMatrix3]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csXRotMatrix3, name)
    def __repr__(self):
        return "<C csXRotMatrix3 instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csXRotMatrix3, 'this', _blcelc.new_csXRotMatrix3(*args))
        _swig_setattr(self, csXRotMatrix3, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csXRotMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csXRotMatrix3Ptr(csXRotMatrix3):
    def __init__(self, this):
        _swig_setattr(self, csXRotMatrix3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csXRotMatrix3, 'thisown', 0)
        _swig_setattr(self, csXRotMatrix3,self.__class__,csXRotMatrix3)
_blcelc.csXRotMatrix3_swigregister(csXRotMatrix3Ptr)

class csYRotMatrix3(csMatrix3):
    __swig_setmethods__ = {}
    for _s in [csMatrix3]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csYRotMatrix3, name, value)
    __swig_getmethods__ = {}
    for _s in [csMatrix3]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csYRotMatrix3, name)
    def __repr__(self):
        return "<C csYRotMatrix3 instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csYRotMatrix3, 'this', _blcelc.new_csYRotMatrix3(*args))
        _swig_setattr(self, csYRotMatrix3, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csYRotMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csYRotMatrix3Ptr(csYRotMatrix3):
    def __init__(self, this):
        _swig_setattr(self, csYRotMatrix3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csYRotMatrix3, 'thisown', 0)
        _swig_setattr(self, csYRotMatrix3,self.__class__,csYRotMatrix3)
_blcelc.csYRotMatrix3_swigregister(csYRotMatrix3Ptr)

class csZRotMatrix3(csMatrix3):
    __swig_setmethods__ = {}
    for _s in [csMatrix3]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csZRotMatrix3, name, value)
    __swig_getmethods__ = {}
    for _s in [csMatrix3]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csZRotMatrix3, name)
    def __repr__(self):
        return "<C csZRotMatrix3 instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csZRotMatrix3, 'this', _blcelc.new_csZRotMatrix3(*args))
        _swig_setattr(self, csZRotMatrix3, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csZRotMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csZRotMatrix3Ptr(csZRotMatrix3):
    def __init__(self, this):
        _swig_setattr(self, csZRotMatrix3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csZRotMatrix3, 'thisown', 0)
        _swig_setattr(self, csZRotMatrix3,self.__class__,csZRotMatrix3)
_blcelc.csZRotMatrix3_swigregister(csZRotMatrix3Ptr)

class csXScaleMatrix3(csMatrix3):
    __swig_setmethods__ = {}
    for _s in [csMatrix3]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csXScaleMatrix3, name, value)
    __swig_getmethods__ = {}
    for _s in [csMatrix3]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csXScaleMatrix3, name)
    def __repr__(self):
        return "<C csXScaleMatrix3 instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csXScaleMatrix3, 'this', _blcelc.new_csXScaleMatrix3(*args))
        _swig_setattr(self, csXScaleMatrix3, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csXScaleMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csXScaleMatrix3Ptr(csXScaleMatrix3):
    def __init__(self, this):
        _swig_setattr(self, csXScaleMatrix3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csXScaleMatrix3, 'thisown', 0)
        _swig_setattr(self, csXScaleMatrix3,self.__class__,csXScaleMatrix3)
_blcelc.csXScaleMatrix3_swigregister(csXScaleMatrix3Ptr)

class csYScaleMatrix3(csMatrix3):
    __swig_setmethods__ = {}
    for _s in [csMatrix3]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csYScaleMatrix3, name, value)
    __swig_getmethods__ = {}
    for _s in [csMatrix3]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csYScaleMatrix3, name)
    def __repr__(self):
        return "<C csYScaleMatrix3 instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csYScaleMatrix3, 'this', _blcelc.new_csYScaleMatrix3(*args))
        _swig_setattr(self, csYScaleMatrix3, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csYScaleMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csYScaleMatrix3Ptr(csYScaleMatrix3):
    def __init__(self, this):
        _swig_setattr(self, csYScaleMatrix3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csYScaleMatrix3, 'thisown', 0)
        _swig_setattr(self, csYScaleMatrix3,self.__class__,csYScaleMatrix3)
_blcelc.csYScaleMatrix3_swigregister(csYScaleMatrix3Ptr)

class csZScaleMatrix3(csMatrix3):
    __swig_setmethods__ = {}
    for _s in [csMatrix3]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csZScaleMatrix3, name, value)
    __swig_getmethods__ = {}
    for _s in [csMatrix3]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csZScaleMatrix3, name)
    def __repr__(self):
        return "<C csZScaleMatrix3 instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csZScaleMatrix3, 'this', _blcelc.new_csZScaleMatrix3(*args))
        _swig_setattr(self, csZScaleMatrix3, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csZScaleMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csZScaleMatrix3Ptr(csZScaleMatrix3):
    def __init__(self, this):
        _swig_setattr(self, csZScaleMatrix3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csZScaleMatrix3, 'thisown', 0)
        _swig_setattr(self, csZScaleMatrix3,self.__class__,csZScaleMatrix3)
_blcelc.csZScaleMatrix3_swigregister(csZScaleMatrix3Ptr)

class csTransform(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csTransform, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csTransform, name)
    def __repr__(self):
        return "<C csTransform instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csTransform, 'this', _blcelc.new_csTransform(*args))
        _swig_setattr(self, csTransform, 'thisown', 1)
    def Identity(*args): return _blcelc.csTransform_Identity(*args)
    def IsIdentity(*args): return _blcelc.csTransform_IsIdentity(*args)
    def GetO2T(*args): return _blcelc.csTransform_GetO2T(*args)
    def GetO2TTranslation(*args): return _blcelc.csTransform_GetO2TTranslation(*args)
    def GetOrigin(*args): return _blcelc.csTransform_GetOrigin(*args)
    def SetO2T(*args): return _blcelc.csTransform_SetO2T(*args)
    def SetO2TTranslation(*args): return _blcelc.csTransform_SetO2TTranslation(*args)
    def SetOrigin(*args): return _blcelc.csTransform_SetOrigin(*args)
    def Translate(*args): return _blcelc.csTransform_Translate(*args)
    def Other2ThisRelative(*args): return _blcelc.csTransform_Other2ThisRelative(*args)
    def Other2This(*args): return _blcelc.csTransform_Other2This(*args)
    __swig_getmethods__["GetReflect"] = lambda x: _blcelc.csTransform_GetReflect
    if _newclass:GetReflect = staticmethod(_blcelc.csTransform_GetReflect)
    def __mul__(*args): return _blcelc.csTransform___mul__(*args)
    def __rmul__(*args): return _blcelc.csTransform___rmul__(*args)
    def __del__(self, destroy=_blcelc.delete_csTransform):
        try:
            if self.thisown: destroy(self)
        except: pass

class csTransformPtr(csTransform):
    def __init__(self, this):
        _swig_setattr(self, csTransform, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csTransform, 'thisown', 0)
        _swig_setattr(self, csTransform,self.__class__,csTransform)
_blcelc.csTransform_swigregister(csTransformPtr)

csTransform_GetReflect = _blcelc.csTransform_GetReflect

class csReversibleTransform(csTransform):
    __swig_setmethods__ = {}
    for _s in [csTransform]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csReversibleTransform, name, value)
    __swig_getmethods__ = {}
    for _s in [csTransform]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csReversibleTransform, name)
    def __repr__(self):
        return "<C csReversibleTransform instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csReversibleTransform, 'this', _blcelc.new_csReversibleTransform(*args))
        _swig_setattr(self, csReversibleTransform, 'thisown', 1)
    def GetT2O(*args): return _blcelc.csReversibleTransform_GetT2O(*args)
    def GetT2OTranslation(*args): return _blcelc.csReversibleTransform_GetT2OTranslation(*args)
    def GetInverse(*args): return _blcelc.csReversibleTransform_GetInverse(*args)
    def SetO2T(*args): return _blcelc.csReversibleTransform_SetO2T(*args)
    def SetT2O(*args): return _blcelc.csReversibleTransform_SetT2O(*args)
    def This2OtherRelative(*args): return _blcelc.csReversibleTransform_This2OtherRelative(*args)
    def This2Other(*args): return _blcelc.csReversibleTransform_This2Other(*args)
    def RotateOther(*args): return _blcelc.csReversibleTransform_RotateOther(*args)
    def RotateThis(*args): return _blcelc.csReversibleTransform_RotateThis(*args)
    def LookAt(*args): return _blcelc.csReversibleTransform_LookAt(*args)
    def __imul__(*args): return _blcelc.csReversibleTransform___imul__(*args)
    def __mul__(*args): return _blcelc.csReversibleTransform___mul__(*args)
    def __idiv__(*args): return _blcelc.csReversibleTransform___idiv__(*args)
    def __div__(*args): return _blcelc.csReversibleTransform___div__(*args)
    def __del__(self, destroy=_blcelc.delete_csReversibleTransform):
        try:
            if self.thisown: destroy(self)
        except: pass

class csReversibleTransformPtr(csReversibleTransform):
    def __init__(self, this):
        _swig_setattr(self, csReversibleTransform, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csReversibleTransform, 'thisown', 0)
        _swig_setattr(self, csReversibleTransform,self.__class__,csReversibleTransform)
_blcelc.csReversibleTransform_swigregister(csReversibleTransformPtr)

class csOrthoTransform(csReversibleTransform):
    __swig_setmethods__ = {}
    for _s in [csReversibleTransform]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csOrthoTransform, name, value)
    __swig_getmethods__ = {}
    for _s in [csReversibleTransform]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csOrthoTransform, name)
    def __repr__(self):
        return "<C csOrthoTransform instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csOrthoTransform, 'this', _blcelc.new_csOrthoTransform(*args))
        _swig_setattr(self, csOrthoTransform, 'thisown', 1)
    def SetO2T(*args): return _blcelc.csOrthoTransform_SetO2T(*args)
    def SetT2O(*args): return _blcelc.csOrthoTransform_SetT2O(*args)
    def __del__(self, destroy=_blcelc.delete_csOrthoTransform):
        try:
            if self.thisown: destroy(self)
        except: pass

class csOrthoTransformPtr(csOrthoTransform):
    def __init__(self, this):
        _swig_setattr(self, csOrthoTransform, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csOrthoTransform, 'thisown', 0)
        _swig_setattr(self, csOrthoTransform,self.__class__,csOrthoTransform)
_blcelc.csOrthoTransform_swigregister(csOrthoTransformPtr)

class csSphere(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSphere, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSphere, name)
    def __repr__(self):
        return "<C csSphere instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csSphere, 'this', _blcelc.new_csSphere(*args))
        _swig_setattr(self, csSphere, 'thisown', 1)
    def GetCenter(*args): return _blcelc.csSphere_GetCenter(*args)
    def SetCenter(*args): return _blcelc.csSphere_SetCenter(*args)
    def GetRadius(*args): return _blcelc.csSphere_GetRadius(*args)
    def SetRadius(*args): return _blcelc.csSphere_SetRadius(*args)
    def Union(*args): return _blcelc.csSphere_Union(*args)
    def __iadd__(*args): return _blcelc.csSphere___iadd__(*args)
    def __imul__(*args): return _blcelc.csSphere___imul__(*args)
    def __div__(*args): return _blcelc.csSphere___div__(*args)
    def __del__(self, destroy=_blcelc.delete_csSphere):
        try:
            if self.thisown: destroy(self)
        except: pass

class csSpherePtr(csSphere):
    def __init__(self, this):
        _swig_setattr(self, csSphere, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csSphere, 'thisown', 0)
        _swig_setattr(self, csSphere,self.__class__,csSphere)
_blcelc.csSphere_swigregister(csSpherePtr)

CS_POLY_IN = _blcelc.CS_POLY_IN
CS_POLY_ON = _blcelc.CS_POLY_ON
CS_POLY_OUT = _blcelc.CS_POLY_OUT
class csPlane2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPlane2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPlane2, name)
    def __repr__(self):
        return "<C csPlane2 instance at %s>" % (self.this,)
    __swig_setmethods__["norm"] = _blcelc.csPlane2_norm_set
    __swig_getmethods__["norm"] = _blcelc.csPlane2_norm_get
    if _newclass:norm = property(_blcelc.csPlane2_norm_get, _blcelc.csPlane2_norm_set)
    __swig_setmethods__["CC"] = _blcelc.csPlane2_CC_set
    __swig_getmethods__["CC"] = _blcelc.csPlane2_CC_get
    if _newclass:CC = property(_blcelc.csPlane2_CC_get, _blcelc.csPlane2_CC_set)
    def __init__(self, *args):
        _swig_setattr(self, csPlane2, 'this', _blcelc.new_csPlane2(*args))
        _swig_setattr(self, csPlane2, 'thisown', 1)
    def Normal(*args): return _blcelc.csPlane2_Normal(*args)
    def GetNormal(*args): return _blcelc.csPlane2_GetNormal(*args)
    def A(*args): return _blcelc.csPlane2_A(*args)
    def B(*args): return _blcelc.csPlane2_B(*args)
    def C(*args): return _blcelc.csPlane2_C(*args)
    def Set(*args): return _blcelc.csPlane2_Set(*args)
    __swig_getmethods__["Classify"] = lambda x: _blcelc.csPlane2_Classify
    if _newclass:Classify = staticmethod(_blcelc.csPlane2_Classify)
    def Distance(*args): return _blcelc.csPlane2_Distance(*args)
    def SquaredDistance(*args): return _blcelc.csPlane2_SquaredDistance(*args)
    def Invert(*args): return _blcelc.csPlane2_Invert(*args)
    def Normalize(*args): return _blcelc.csPlane2_Normalize(*args)
    def __del__(self, destroy=_blcelc.delete_csPlane2):
        try:
            if self.thisown: destroy(self)
        except: pass

class csPlane2Ptr(csPlane2):
    def __init__(self, this):
        _swig_setattr(self, csPlane2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPlane2, 'thisown', 0)
        _swig_setattr(self, csPlane2,self.__class__,csPlane2)
_blcelc.csPlane2_swigregister(csPlane2Ptr)

csPlane2_Classify = _blcelc.csPlane2_Classify

class csPlane3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPlane3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPlane3, name)
    def __repr__(self):
        return "<C csPlane3 instance at %s>" % (self.this,)
    __swig_setmethods__["norm"] = _blcelc.csPlane3_norm_set
    __swig_getmethods__["norm"] = _blcelc.csPlane3_norm_get
    if _newclass:norm = property(_blcelc.csPlane3_norm_get, _blcelc.csPlane3_norm_set)
    __swig_setmethods__["DD"] = _blcelc.csPlane3_DD_set
    __swig_getmethods__["DD"] = _blcelc.csPlane3_DD_get
    if _newclass:DD = property(_blcelc.csPlane3_DD_get, _blcelc.csPlane3_DD_set)
    def __init__(self, *args):
        _swig_setattr(self, csPlane3, 'this', _blcelc.new_csPlane3(*args))
        _swig_setattr(self, csPlane3, 'thisown', 1)
    def Normal(*args): return _blcelc.csPlane3_Normal(*args)
    def A(*args): return _blcelc.csPlane3_A(*args)
    def B(*args): return _blcelc.csPlane3_B(*args)
    def C(*args): return _blcelc.csPlane3_C(*args)
    def D(*args): return _blcelc.csPlane3_D(*args)
    def Set(*args): return _blcelc.csPlane3_Set(*args)
    __swig_getmethods__["Classify"] = lambda x: _blcelc.csPlane3_Classify
    if _newclass:Classify = staticmethod(_blcelc.csPlane3_Classify)
    def Distance(*args): return _blcelc.csPlane3_Distance(*args)
    def Invert(*args): return _blcelc.csPlane3_Invert(*args)
    def Normalize(*args): return _blcelc.csPlane3_Normalize(*args)
    def ClipPolygon(*args): return _blcelc.csPlane3_ClipPolygon(*args)
    def __imul__(*args): return _blcelc.csPlane3___imul__(*args)
    def __idiv__(*args): return _blcelc.csPlane3___idiv__(*args)
    def __div__(*args): return _blcelc.csPlane3___div__(*args)
    def __del__(self, destroy=_blcelc.delete_csPlane3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csPlane3Ptr(csPlane3):
    def __init__(self, this):
        _swig_setattr(self, csPlane3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPlane3, 'thisown', 0)
        _swig_setattr(self, csPlane3,self.__class__,csPlane3)
_blcelc.csPlane3_swigregister(csPlane3Ptr)

csPlane3_Classify = _blcelc.csPlane3_Classify

class csMath2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csMath2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csMath2, name)
    def __repr__(self):
        return "<C csMath2 instance at %s>" % (self.this,)
    __swig_getmethods__["WhichSide2D"] = lambda x: _blcelc.csMath2_WhichSide2D
    if _newclass:WhichSide2D = staticmethod(_blcelc.csMath2_WhichSide2D)
    __swig_getmethods__["WhichSide2D"] = lambda x: _blcelc.csMath2_WhichSide2D
    if _newclass:WhichSide2D = staticmethod(_blcelc.csMath2_WhichSide2D)
    __swig_getmethods__["InPoly2D"] = lambda x: _blcelc.csMath2_InPoly2D
    if _newclass:InPoly2D = staticmethod(_blcelc.csMath2_InPoly2D)
    __swig_getmethods__["Area2"] = lambda x: _blcelc.csMath2_Area2
    if _newclass:Area2 = staticmethod(_blcelc.csMath2_Area2)
    __swig_getmethods__["Right"] = lambda x: _blcelc.csMath2_Right
    if _newclass:Right = staticmethod(_blcelc.csMath2_Right)
    __swig_getmethods__["Left"] = lambda x: _blcelc.csMath2_Left
    if _newclass:Left = staticmethod(_blcelc.csMath2_Left)
    __swig_getmethods__["Visible"] = lambda x: _blcelc.csMath2_Visible
    if _newclass:Visible = staticmethod(_blcelc.csMath2_Visible)
    __swig_getmethods__["PlanesEqual"] = lambda x: _blcelc.csMath2_PlanesEqual
    if _newclass:PlanesEqual = staticmethod(_blcelc.csMath2_PlanesEqual)
    __swig_getmethods__["PlanesClose"] = lambda x: _blcelc.csMath2_PlanesClose
    if _newclass:PlanesClose = staticmethod(_blcelc.csMath2_PlanesClose)
    def __init__(self, *args):
        _swig_setattr(self, csMath2, 'this', _blcelc.new_csMath2(*args))
        _swig_setattr(self, csMath2, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csMath2):
        try:
            if self.thisown: destroy(self)
        except: pass

class csMath2Ptr(csMath2):
    def __init__(self, this):
        _swig_setattr(self, csMath2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csMath2, 'thisown', 0)
        _swig_setattr(self, csMath2,self.__class__,csMath2)
_blcelc.csMath2_swigregister(csMath2Ptr)

csMath2_WhichSide2D = _blcelc.csMath2_WhichSide2D

csMath2_InPoly2D = _blcelc.csMath2_InPoly2D

csMath2_Area2 = _blcelc.csMath2_Area2

csMath2_Right = _blcelc.csMath2_Right

csMath2_Left = _blcelc.csMath2_Left

csMath2_Visible = _blcelc.csMath2_Visible

csMath2_PlanesEqual = _blcelc.csMath2_PlanesEqual

csMath2_PlanesClose = _blcelc.csMath2_PlanesClose

class csIntersect2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csIntersect2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csIntersect2, name)
    def __repr__(self):
        return "<C csIntersect2 instance at %s>" % (self.this,)
    __swig_getmethods__["IntersectPolygon"] = lambda x: _blcelc.csIntersect2_IntersectPolygon
    if _newclass:IntersectPolygon = staticmethod(_blcelc.csIntersect2_IntersectPolygon)
    __swig_getmethods__["Segments"] = lambda x: _blcelc.csIntersect2_Segments
    if _newclass:Segments = staticmethod(_blcelc.csIntersect2_Segments)
    __swig_getmethods__["SegmentLine"] = lambda x: _blcelc.csIntersect2_SegmentLine
    if _newclass:SegmentLine = staticmethod(_blcelc.csIntersect2_SegmentLine)
    __swig_getmethods__["Lines"] = lambda x: _blcelc.csIntersect2_Lines
    if _newclass:Lines = staticmethod(_blcelc.csIntersect2_Lines)
    __swig_getmethods__["Plane"] = lambda x: _blcelc.csIntersect2_Plane
    if _newclass:Plane = staticmethod(_blcelc.csIntersect2_Plane)
    __swig_getmethods__["Plane"] = lambda x: _blcelc.csIntersect2_Plane
    if _newclass:Plane = staticmethod(_blcelc.csIntersect2_Plane)
    __swig_getmethods__["PlaneNoTest"] = lambda x: _blcelc.csIntersect2_PlaneNoTest
    if _newclass:PlaneNoTest = staticmethod(_blcelc.csIntersect2_PlaneNoTest)
    __swig_getmethods__["PlaneNoTest"] = lambda x: _blcelc.csIntersect2_PlaneNoTest
    if _newclass:PlaneNoTest = staticmethod(_blcelc.csIntersect2_PlaneNoTest)
    __swig_getmethods__["Planes"] = lambda x: _blcelc.csIntersect2_Planes
    if _newclass:Planes = staticmethod(_blcelc.csIntersect2_Planes)
    def __init__(self, *args):
        _swig_setattr(self, csIntersect2, 'this', _blcelc.new_csIntersect2(*args))
        _swig_setattr(self, csIntersect2, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csIntersect2):
        try:
            if self.thisown: destroy(self)
        except: pass

class csIntersect2Ptr(csIntersect2):
    def __init__(self, this):
        _swig_setattr(self, csIntersect2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csIntersect2, 'thisown', 0)
        _swig_setattr(self, csIntersect2,self.__class__,csIntersect2)
_blcelc.csIntersect2_swigregister(csIntersect2Ptr)

csIntersect2_IntersectPolygon = _blcelc.csIntersect2_IntersectPolygon

csIntersect2_Segments = _blcelc.csIntersect2_Segments

csIntersect2_SegmentLine = _blcelc.csIntersect2_SegmentLine

csIntersect2_Lines = _blcelc.csIntersect2_Lines

csIntersect2_Plane = _blcelc.csIntersect2_Plane

csIntersect2_PlaneNoTest = _blcelc.csIntersect2_PlaneNoTest

csIntersect2_Planes = _blcelc.csIntersect2_Planes

class csPoly2D(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPoly2D, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPoly2D, name)
    def __repr__(self):
        return "<C csPoly2D instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csPoly2D, 'this', _blcelc.new_csPoly2D(*args))
        _swig_setattr(self, csPoly2D, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csPoly2D):
        try:
            if self.thisown: destroy(self)
        except: pass
    def assign(*args): return _blcelc.csPoly2D_assign(*args)
    def MakeEmpty(*args): return _blcelc.csPoly2D_MakeEmpty(*args)
    def GetVertexCount(*args): return _blcelc.csPoly2D_GetVertexCount(*args)
    def GetVertices(*args): return _blcelc.csPoly2D_GetVertices(*args)
    def GetVertex(*args): return _blcelc.csPoly2D_GetVertex(*args)
    def GetFirst(*args): return _blcelc.csPoly2D_GetFirst(*args)
    def GetLast(*args): return _blcelc.csPoly2D_GetLast(*args)
    __swig_getmethods__["In"] = lambda x: _blcelc.csPoly2D_In
    if _newclass:In = staticmethod(_blcelc.csPoly2D_In)
    def MakeRoom(*args): return _blcelc.csPoly2D_MakeRoom(*args)
    def SetVertexCount(*args): return _blcelc.csPoly2D_SetVertexCount(*args)
    def AddVertex(*args): return _blcelc.csPoly2D_AddVertex(*args)
    def SetVertices(*args): return _blcelc.csPoly2D_SetVertices(*args)
    def ClipAgainst(*args): return _blcelc.csPoly2D_ClipAgainst(*args)
    def Intersect(*args): return _blcelc.csPoly2D_Intersect(*args)
    def ClipPlane(*args): return _blcelc.csPoly2D_ClipPlane(*args)
    def ExtendConvex(*args): return _blcelc.csPoly2D_ExtendConvex(*args)
    def GetSignedArea(*args): return _blcelc.csPoly2D_GetSignedArea(*args)
    def Random(*args): return _blcelc.csPoly2D_Random(*args)
    def __getitem__(*args): return _blcelc.csPoly2D___getitem__(*args)
    def __setitem__ (self, i, v):
      own_v = self.__getitem__(i)
      for i in range(2):
        own_v[i] = v[i]


class csPoly2DPtr(csPoly2D):
    def __init__(self, this):
        _swig_setattr(self, csPoly2D, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPoly2D, 'thisown', 0)
        _swig_setattr(self, csPoly2D,self.__class__,csPoly2D)
_blcelc.csPoly2D_swigregister(csPoly2DPtr)

csPoly2D_In = _blcelc.csPoly2D_In

class csPoly2DFactory(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPoly2DFactory, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPoly2DFactory, name)
    def __repr__(self):
        return "<C csPoly2DFactory instance at %s>" % (self.this,)
    def Create(*args): return _blcelc.csPoly2DFactory_Create(*args)
    def __init__(self, *args):
        _swig_setattr(self, csPoly2DFactory, 'this', _blcelc.new_csPoly2DFactory(*args))
        _swig_setattr(self, csPoly2DFactory, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csPoly2DFactory):
        try:
            if self.thisown: destroy(self)
        except: pass

class csPoly2DFactoryPtr(csPoly2DFactory):
    def __init__(self, this):
        _swig_setattr(self, csPoly2DFactory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPoly2DFactory, 'thisown', 0)
        _swig_setattr(self, csPoly2DFactory,self.__class__,csPoly2DFactory)
_blcelc.csPoly2DFactory_swigregister(csPoly2DFactoryPtr)


fSqr = _blcelc.fSqr
class csMath3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csMath3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csMath3, name)
    def __repr__(self):
        return "<C csMath3 instance at %s>" % (self.this,)
    __swig_getmethods__["WhichSide3D"] = lambda x: _blcelc.csMath3_WhichSide3D
    if _newclass:WhichSide3D = staticmethod(_blcelc.csMath3_WhichSide3D)
    __swig_getmethods__["Visible"] = lambda x: _blcelc.csMath3_Visible
    if _newclass:Visible = staticmethod(_blcelc.csMath3_Visible)
    __swig_getmethods__["Visible"] = lambda x: _blcelc.csMath3_Visible
    if _newclass:Visible = staticmethod(_blcelc.csMath3_Visible)
    __swig_getmethods__["FindIntersection"] = lambda x: _blcelc.csMath3_FindIntersection
    if _newclass:FindIntersection = staticmethod(_blcelc.csMath3_FindIntersection)
    __swig_getmethods__["Between"] = lambda x: _blcelc.csMath3_Between
    if _newclass:Between = staticmethod(_blcelc.csMath3_Between)
    __swig_getmethods__["SetMinMax"] = lambda x: _blcelc.csMath3_SetMinMax
    if _newclass:SetMinMax = staticmethod(_blcelc.csMath3_SetMinMax)
    __swig_getmethods__["DoubleArea3"] = lambda x: _blcelc.csMath3_DoubleArea3
    if _newclass:DoubleArea3 = staticmethod(_blcelc.csMath3_DoubleArea3)
    __swig_getmethods__["Direction3"] = lambda x: _blcelc.csMath3_Direction3
    if _newclass:Direction3 = staticmethod(_blcelc.csMath3_Direction3)
    __swig_getmethods__["CalcNormal"] = lambda x: _blcelc.csMath3_CalcNormal
    if _newclass:CalcNormal = staticmethod(_blcelc.csMath3_CalcNormal)
    __swig_getmethods__["CalcNormal"] = lambda x: _blcelc.csMath3_CalcNormal
    if _newclass:CalcNormal = staticmethod(_blcelc.csMath3_CalcNormal)
    __swig_getmethods__["CalcPlane"] = lambda x: _blcelc.csMath3_CalcPlane
    if _newclass:CalcPlane = staticmethod(_blcelc.csMath3_CalcPlane)
    __swig_getmethods__["PlanesEqual"] = lambda x: _blcelc.csMath3_PlanesEqual
    if _newclass:PlanesEqual = staticmethod(_blcelc.csMath3_PlanesEqual)
    __swig_getmethods__["PlanesClose"] = lambda x: _blcelc.csMath3_PlanesClose
    if _newclass:PlanesClose = staticmethod(_blcelc.csMath3_PlanesClose)
    __swig_getmethods__["OuterPlanes"] = lambda x: _blcelc.csMath3_OuterPlanes
    if _newclass:OuterPlanes = staticmethod(_blcelc.csMath3_OuterPlanes)
    __swig_getmethods__["FindObserverSides"] = lambda x: _blcelc.csMath3_FindObserverSides
    if _newclass:FindObserverSides = staticmethod(_blcelc.csMath3_FindObserverSides)
    __swig_getmethods__["SpherePosition"] = lambda x: _blcelc.csMath3_SpherePosition
    if _newclass:SpherePosition = staticmethod(_blcelc.csMath3_SpherePosition)
    def __init__(self, *args):
        _swig_setattr(self, csMath3, 'this', _blcelc.new_csMath3(*args))
        _swig_setattr(self, csMath3, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csMath3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csMath3Ptr(csMath3):
    def __init__(self, this):
        _swig_setattr(self, csMath3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csMath3, 'thisown', 0)
        _swig_setattr(self, csMath3,self.__class__,csMath3)
_blcelc.csMath3_swigregister(csMath3Ptr)

csMath3_WhichSide3D = _blcelc.csMath3_WhichSide3D

csMath3_Visible = _blcelc.csMath3_Visible

csMath3_FindIntersection = _blcelc.csMath3_FindIntersection

csMath3_Between = _blcelc.csMath3_Between

csMath3_SetMinMax = _blcelc.csMath3_SetMinMax

csMath3_DoubleArea3 = _blcelc.csMath3_DoubleArea3

csMath3_Direction3 = _blcelc.csMath3_Direction3

csMath3_CalcNormal = _blcelc.csMath3_CalcNormal

csMath3_CalcPlane = _blcelc.csMath3_CalcPlane

csMath3_PlanesEqual = _blcelc.csMath3_PlanesEqual

csMath3_PlanesClose = _blcelc.csMath3_PlanesClose

csMath3_OuterPlanes = _blcelc.csMath3_OuterPlanes

csMath3_FindObserverSides = _blcelc.csMath3_FindObserverSides

csMath3_SpherePosition = _blcelc.csMath3_SpherePosition

class csSquaredDist(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSquaredDist, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSquaredDist, name)
    def __repr__(self):
        return "<C csSquaredDist instance at %s>" % (self.this,)
    __swig_getmethods__["PointPoint"] = lambda x: _blcelc.csSquaredDist_PointPoint
    if _newclass:PointPoint = staticmethod(_blcelc.csSquaredDist_PointPoint)
    __swig_getmethods__["PointLine"] = lambda x: _blcelc.csSquaredDist_PointLine
    if _newclass:PointLine = staticmethod(_blcelc.csSquaredDist_PointLine)
    __swig_getmethods__["PointPlane"] = lambda x: _blcelc.csSquaredDist_PointPlane
    if _newclass:PointPlane = staticmethod(_blcelc.csSquaredDist_PointPlane)
    __swig_getmethods__["PointPoly"] = lambda x: _blcelc.csSquaredDist_PointPoly
    if _newclass:PointPoly = staticmethod(_blcelc.csSquaredDist_PointPoly)
    def __init__(self, *args):
        _swig_setattr(self, csSquaredDist, 'this', _blcelc.new_csSquaredDist(*args))
        _swig_setattr(self, csSquaredDist, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csSquaredDist):
        try:
            if self.thisown: destroy(self)
        except: pass

class csSquaredDistPtr(csSquaredDist):
    def __init__(self, this):
        _swig_setattr(self, csSquaredDist, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csSquaredDist, 'thisown', 0)
        _swig_setattr(self, csSquaredDist,self.__class__,csSquaredDist)
_blcelc.csSquaredDist_swigregister(csSquaredDistPtr)

csSquaredDist_PointPoint = _blcelc.csSquaredDist_PointPoint

csSquaredDist_PointLine = _blcelc.csSquaredDist_PointLine

csSquaredDist_PointPlane = _blcelc.csSquaredDist_PointPlane

csSquaredDist_PointPoly = _blcelc.csSquaredDist_PointPoly

class csIntersect3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csIntersect3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csIntersect3, name)
    def __repr__(self):
        return "<C csIntersect3 instance at %s>" % (self.this,)
    __swig_getmethods__["IntersectPolygon"] = lambda x: _blcelc.csIntersect3_IntersectPolygon
    if _newclass:IntersectPolygon = staticmethod(_blcelc.csIntersect3_IntersectPolygon)
    __swig_getmethods__["IntersectSegment"] = lambda x: _blcelc.csIntersect3_IntersectSegment
    if _newclass:IntersectSegment = staticmethod(_blcelc.csIntersect3_IntersectSegment)
    __swig_getmethods__["IntersectTriangle"] = lambda x: _blcelc.csIntersect3_IntersectTriangle
    if _newclass:IntersectTriangle = staticmethod(_blcelc.csIntersect3_IntersectTriangle)
    __swig_getmethods__["IntersectPolygon"] = lambda x: _blcelc.csIntersect3_IntersectPolygon
    if _newclass:IntersectPolygon = staticmethod(_blcelc.csIntersect3_IntersectPolygon)
    __swig_getmethods__["Planes"] = lambda x: _blcelc.csIntersect3_Planes
    if _newclass:Planes = staticmethod(_blcelc.csIntersect3_Planes)
    __swig_getmethods__["Plane"] = lambda x: _blcelc.csIntersect3_Plane
    if _newclass:Plane = staticmethod(_blcelc.csIntersect3_Plane)
    __swig_getmethods__["Plane"] = lambda x: _blcelc.csIntersect3_Plane
    if _newclass:Plane = staticmethod(_blcelc.csIntersect3_Plane)
    __swig_getmethods__["Planes"] = lambda x: _blcelc.csIntersect3_Planes
    if _newclass:Planes = staticmethod(_blcelc.csIntersect3_Planes)
    __swig_getmethods__["PlaneXPlane"] = lambda x: _blcelc.csIntersect3_PlaneXPlane
    if _newclass:PlaneXPlane = staticmethod(_blcelc.csIntersect3_PlaneXPlane)
    __swig_getmethods__["PlaneYPlane"] = lambda x: _blcelc.csIntersect3_PlaneYPlane
    if _newclass:PlaneYPlane = staticmethod(_blcelc.csIntersect3_PlaneYPlane)
    __swig_getmethods__["PlaneZPlane"] = lambda x: _blcelc.csIntersect3_PlaneZPlane
    if _newclass:PlaneZPlane = staticmethod(_blcelc.csIntersect3_PlaneZPlane)
    __swig_getmethods__["PlaneAxisPlane"] = lambda x: _blcelc.csIntersect3_PlaneAxisPlane
    if _newclass:PlaneAxisPlane = staticmethod(_blcelc.csIntersect3_PlaneAxisPlane)
    __swig_getmethods__["Z0Plane"] = lambda x: _blcelc.csIntersect3_Z0Plane
    if _newclass:Z0Plane = staticmethod(_blcelc.csIntersect3_Z0Plane)
    __swig_getmethods__["Z0Plane"] = lambda x: _blcelc.csIntersect3_Z0Plane
    if _newclass:Z0Plane = staticmethod(_blcelc.csIntersect3_Z0Plane)
    __swig_getmethods__["XPlane"] = lambda x: _blcelc.csIntersect3_XPlane
    if _newclass:XPlane = staticmethod(_blcelc.csIntersect3_XPlane)
    __swig_getmethods__["XPlane"] = lambda x: _blcelc.csIntersect3_XPlane
    if _newclass:XPlane = staticmethod(_blcelc.csIntersect3_XPlane)
    __swig_getmethods__["YPlane"] = lambda x: _blcelc.csIntersect3_YPlane
    if _newclass:YPlane = staticmethod(_blcelc.csIntersect3_YPlane)
    __swig_getmethods__["YPlane"] = lambda x: _blcelc.csIntersect3_YPlane
    if _newclass:YPlane = staticmethod(_blcelc.csIntersect3_YPlane)
    __swig_getmethods__["ZPlane"] = lambda x: _blcelc.csIntersect3_ZPlane
    if _newclass:ZPlane = staticmethod(_blcelc.csIntersect3_ZPlane)
    __swig_getmethods__["ZPlane"] = lambda x: _blcelc.csIntersect3_ZPlane
    if _newclass:ZPlane = staticmethod(_blcelc.csIntersect3_ZPlane)
    __swig_getmethods__["AxisPlane"] = lambda x: _blcelc.csIntersect3_AxisPlane
    if _newclass:AxisPlane = staticmethod(_blcelc.csIntersect3_AxisPlane)
    __swig_getmethods__["XFrustum"] = lambda x: _blcelc.csIntersect3_XFrustum
    if _newclass:XFrustum = staticmethod(_blcelc.csIntersect3_XFrustum)
    __swig_getmethods__["XFrustum"] = lambda x: _blcelc.csIntersect3_XFrustum
    if _newclass:XFrustum = staticmethod(_blcelc.csIntersect3_XFrustum)
    __swig_getmethods__["YFrustum"] = lambda x: _blcelc.csIntersect3_YFrustum
    if _newclass:YFrustum = staticmethod(_blcelc.csIntersect3_YFrustum)
    __swig_getmethods__["YFrustum"] = lambda x: _blcelc.csIntersect3_YFrustum
    if _newclass:YFrustum = staticmethod(_blcelc.csIntersect3_YFrustum)
    __swig_getmethods__["BoxSegment"] = lambda x: _blcelc.csIntersect3_BoxSegment
    if _newclass:BoxSegment = staticmethod(_blcelc.csIntersect3_BoxSegment)
    __swig_getmethods__["BoxFrustum"] = lambda x: _blcelc.csIntersect3_BoxFrustum
    if _newclass:BoxFrustum = staticmethod(_blcelc.csIntersect3_BoxFrustum)
    __swig_getmethods__["BoxSphere"] = lambda x: _blcelc.csIntersect3_BoxSphere
    if _newclass:BoxSphere = staticmethod(_blcelc.csIntersect3_BoxSphere)
    def __init__(self, *args):
        _swig_setattr(self, csIntersect3, 'this', _blcelc.new_csIntersect3(*args))
        _swig_setattr(self, csIntersect3, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csIntersect3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csIntersect3Ptr(csIntersect3):
    def __init__(self, this):
        _swig_setattr(self, csIntersect3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csIntersect3, 'thisown', 0)
        _swig_setattr(self, csIntersect3,self.__class__,csIntersect3)
_blcelc.csIntersect3_swigregister(csIntersect3Ptr)

csIntersect3_IntersectSegment = _blcelc.csIntersect3_IntersectSegment

csIntersect3_IntersectTriangle = _blcelc.csIntersect3_IntersectTriangle

csIntersect3_IntersectPolygon = _blcelc.csIntersect3_IntersectPolygon

csIntersect3_Plane = _blcelc.csIntersect3_Plane

csIntersect3_Planes = _blcelc.csIntersect3_Planes

csIntersect3_PlaneXPlane = _blcelc.csIntersect3_PlaneXPlane

csIntersect3_PlaneYPlane = _blcelc.csIntersect3_PlaneYPlane

csIntersect3_PlaneZPlane = _blcelc.csIntersect3_PlaneZPlane

csIntersect3_PlaneAxisPlane = _blcelc.csIntersect3_PlaneAxisPlane

csIntersect3_Z0Plane = _blcelc.csIntersect3_Z0Plane

csIntersect3_XPlane = _blcelc.csIntersect3_XPlane

csIntersect3_YPlane = _blcelc.csIntersect3_YPlane

csIntersect3_ZPlane = _blcelc.csIntersect3_ZPlane

csIntersect3_AxisPlane = _blcelc.csIntersect3_AxisPlane

csIntersect3_XFrustum = _blcelc.csIntersect3_XFrustum

csIntersect3_YFrustum = _blcelc.csIntersect3_YFrustum

csIntersect3_BoxSegment = _blcelc.csIntersect3_BoxSegment

csIntersect3_BoxFrustum = _blcelc.csIntersect3_BoxFrustum

csIntersect3_BoxSphere = _blcelc.csIntersect3_BoxSphere

class csGeomDebugHelper(iDebugHelper):
    __swig_setmethods__ = {}
    for _s in [iDebugHelper]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csGeomDebugHelper, name, value)
    __swig_getmethods__ = {}
    for _s in [iDebugHelper]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csGeomDebugHelper, name)
    def __repr__(self):
        return "<C csGeomDebugHelper instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csGeomDebugHelper, 'this', _blcelc.new_csGeomDebugHelper(*args))
        _swig_setattr(self, csGeomDebugHelper, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csGeomDebugHelper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_setmethods__["scfRefCount"] = _blcelc.csGeomDebugHelper_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csGeomDebugHelper_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csGeomDebugHelper_scfRefCount_get, _blcelc.csGeomDebugHelper_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csGeomDebugHelper_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csGeomDebugHelper_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csGeomDebugHelper_scfWeakRefOwners_get, _blcelc.csGeomDebugHelper_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc.csGeomDebugHelper_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc.csGeomDebugHelper_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csGeomDebugHelper_scfParent_get
    if _newclass:scfParent = property(_blcelc.csGeomDebugHelper_scfParent_get, _blcelc.csGeomDebugHelper_scfParent_set)
    def IncRef(*args): return _blcelc.csGeomDebugHelper_IncRef(*args)
    def DecRef(*args): return _blcelc.csGeomDebugHelper_DecRef(*args)
    def GetRefCount(*args): return _blcelc.csGeomDebugHelper_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.csGeomDebugHelper_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.csGeomDebugHelper_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.csGeomDebugHelper_QueryInterface(*args)
    def GetSupportedTests(*args): return _blcelc.csGeomDebugHelper_GetSupportedTests(*args)
    def UnitTest(*args): return _blcelc.csGeomDebugHelper_UnitTest(*args)
    def StateTest(*args): return _blcelc.csGeomDebugHelper_StateTest(*args)
    def Benchmark(*args): return _blcelc.csGeomDebugHelper_Benchmark(*args)
    def Dump(*args): return _blcelc.csGeomDebugHelper_Dump(*args)
    def DebugCommand(*args): return _blcelc.csGeomDebugHelper_DebugCommand(*args)

class csGeomDebugHelperPtr(csGeomDebugHelper):
    def __init__(self, this):
        _swig_setattr(self, csGeomDebugHelper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csGeomDebugHelper, 'thisown', 0)
        _swig_setattr(self, csGeomDebugHelper,self.__class__,csGeomDebugHelper)
_blcelc.csGeomDebugHelper_swigregister(csGeomDebugHelperPtr)

CS_POL_SAME_PLANE = _blcelc.CS_POL_SAME_PLANE
CS_POL_FRONT = _blcelc.CS_POL_FRONT
CS_POL_BACK = _blcelc.CS_POL_BACK
CS_POL_SPLIT_NEEDED = _blcelc.CS_POL_SPLIT_NEEDED
class csPoly3D(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPoly3D, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPoly3D, name)
    def __repr__(self):
        return "<C csPoly3D instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csPoly3D, 'this', _blcelc.new_csPoly3D(*args))
        _swig_setattr(self, csPoly3D, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csPoly3D):
        try:
            if self.thisown: destroy(self)
        except: pass
    def MakeEmpty(*args): return _blcelc.csPoly3D_MakeEmpty(*args)
    def GetVertexCount(*args): return _blcelc.csPoly3D_GetVertexCount(*args)
    def GetVertices(*args): return _blcelc.csPoly3D_GetVertices(*args)
    def GetVertex(*args): return _blcelc.csPoly3D_GetVertex(*args)
    def GetFirst(*args): return _blcelc.csPoly3D_GetFirst(*args)
    def GetLast(*args): return _blcelc.csPoly3D_GetLast(*args)
    __swig_getmethods__["In"] = lambda x: _blcelc.csPoly3D_In
    if _newclass:In = staticmethod(_blcelc.csPoly3D_In)
    def MakeRoom(*args): return _blcelc.csPoly3D_MakeRoom(*args)
    def SetVertexCount(*args): return _blcelc.csPoly3D_SetVertexCount(*args)
    def AddVertex(*args): return _blcelc.csPoly3D_AddVertex(*args)
    def SetVertices(*args): return _blcelc.csPoly3D_SetVertices(*args)
    def ProjectXPlane(*args): return _blcelc.csPoly3D_ProjectXPlane(*args)
    def ProjectYPlane(*args): return _blcelc.csPoly3D_ProjectYPlane(*args)
    def ProjectZPlane(*args): return _blcelc.csPoly3D_ProjectZPlane(*args)
    def ProjectAxisPlane(*args): return _blcelc.csPoly3D_ProjectAxisPlane(*args)
    __swig_getmethods__["Classify"] = lambda x: _blcelc.csPoly3D_Classify
    if _newclass:Classify = staticmethod(_blcelc.csPoly3D_Classify)
    def Classify(*args): return _blcelc.csPoly3D_Classify(*args)
    def ClassifyX(*args): return _blcelc.csPoly3D_ClassifyX(*args)
    def ClassifyY(*args): return _blcelc.csPoly3D_ClassifyY(*args)
    def ClassifyZ(*args): return _blcelc.csPoly3D_ClassifyZ(*args)
    def CutToPlane(*args): return _blcelc.csPoly3D_CutToPlane(*args)
    def SplitWithPlane(*args): return _blcelc.csPoly3D_SplitWithPlane(*args)
    def SplitWithPlaneX(*args): return _blcelc.csPoly3D_SplitWithPlaneX(*args)
    def SplitWithPlaneY(*args): return _blcelc.csPoly3D_SplitWithPlaneY(*args)
    def SplitWithPlaneZ(*args): return _blcelc.csPoly3D_SplitWithPlaneZ(*args)
    __swig_getmethods__["ComputeNormal"] = lambda x: _blcelc.csPoly3D_ComputeNormal
    if _newclass:ComputeNormal = staticmethod(_blcelc.csPoly3D_ComputeNormal)
    __swig_getmethods__["ComputeNormal"] = lambda x: _blcelc.csPoly3D_ComputeNormal
    if _newclass:ComputeNormal = staticmethod(_blcelc.csPoly3D_ComputeNormal)
    __swig_getmethods__["ComputeNormal"] = lambda x: _blcelc.csPoly3D_ComputeNormal
    if _newclass:ComputeNormal = staticmethod(_blcelc.csPoly3D_ComputeNormal)
    def ComputeNormal(*args): return _blcelc.csPoly3D_ComputeNormal(*args)
    __swig_getmethods__["ComputePlane"] = lambda x: _blcelc.csPoly3D_ComputePlane
    if _newclass:ComputePlane = staticmethod(_blcelc.csPoly3D_ComputePlane)
    __swig_getmethods__["ComputePlane"] = lambda x: _blcelc.csPoly3D_ComputePlane
    if _newclass:ComputePlane = staticmethod(_blcelc.csPoly3D_ComputePlane)
    __swig_getmethods__["ComputePlane"] = lambda x: _blcelc.csPoly3D_ComputePlane
    if _newclass:ComputePlane = staticmethod(_blcelc.csPoly3D_ComputePlane)
    def ComputePlane(*args): return _blcelc.csPoly3D_ComputePlane(*args)
    def GetArea(*args): return _blcelc.csPoly3D_GetArea(*args)
    def GetCenter(*args): return _blcelc.csPoly3D_GetCenter(*args)
    def __getitem__(*args): return _blcelc.csPoly3D___getitem__(*args)
    def __setitem__ (self, i, v):
      own_v = self.__getitem__(i)
      for i in range(3):
        own_v[i] = v[i]


class csPoly3DPtr(csPoly3D):
    def __init__(self, this):
        _swig_setattr(self, csPoly3D, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPoly3D, 'thisown', 0)
        _swig_setattr(self, csPoly3D,self.__class__,csPoly3D)
_blcelc.csPoly3D_swigregister(csPoly3DPtr)

csPoly3D_In = _blcelc.csPoly3D_In

class csCompressVertex(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csCompressVertex, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csCompressVertex, name)
    def __repr__(self):
        return "<C csCompressVertex instance at %s>" % (self.this,)
    __swig_setmethods__["orig_idx"] = _blcelc.csCompressVertex_orig_idx_set
    __swig_getmethods__["orig_idx"] = _blcelc.csCompressVertex_orig_idx_get
    if _newclass:orig_idx = property(_blcelc.csCompressVertex_orig_idx_get, _blcelc.csCompressVertex_orig_idx_set)
    __swig_setmethods__["x"] = _blcelc.csCompressVertex_x_set
    __swig_getmethods__["x"] = _blcelc.csCompressVertex_x_get
    if _newclass:x = property(_blcelc.csCompressVertex_x_get, _blcelc.csCompressVertex_x_set)
    __swig_setmethods__["y"] = _blcelc.csCompressVertex_y_set
    __swig_getmethods__["y"] = _blcelc.csCompressVertex_y_get
    if _newclass:y = property(_blcelc.csCompressVertex_y_get, _blcelc.csCompressVertex_y_set)
    __swig_setmethods__["z"] = _blcelc.csCompressVertex_z_set
    __swig_getmethods__["z"] = _blcelc.csCompressVertex_z_get
    if _newclass:z = property(_blcelc.csCompressVertex_z_get, _blcelc.csCompressVertex_z_set)
    __swig_setmethods__["new_idx"] = _blcelc.csCompressVertex_new_idx_set
    __swig_getmethods__["new_idx"] = _blcelc.csCompressVertex_new_idx_get
    if _newclass:new_idx = property(_blcelc.csCompressVertex_new_idx_get, _blcelc.csCompressVertex_new_idx_set)
    __swig_setmethods__["used"] = _blcelc.csCompressVertex_used_set
    __swig_getmethods__["used"] = _blcelc.csCompressVertex_used_get
    if _newclass:used = property(_blcelc.csCompressVertex_used_get, _blcelc.csCompressVertex_used_set)
    def __init__(self, *args):
        _swig_setattr(self, csCompressVertex, 'this', _blcelc.new_csCompressVertex(*args))
        _swig_setattr(self, csCompressVertex, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csCompressVertex):
        try:
            if self.thisown: destroy(self)
        except: pass

class csCompressVertexPtr(csCompressVertex):
    def __init__(self, this):
        _swig_setattr(self, csCompressVertex, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csCompressVertex, 'thisown', 0)
        _swig_setattr(self, csCompressVertex,self.__class__,csCompressVertex)
_blcelc.csCompressVertex_swigregister(csCompressVertexPtr)

class csVector3Array(csPoly3D):
    __swig_setmethods__ = {}
    for _s in [csPoly3D]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csVector3Array, name, value)
    __swig_getmethods__ = {}
    for _s in [csPoly3D]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csVector3Array, name)
    def __repr__(self):
        return "<C csVector3Array instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csVector3Array, 'this', _blcelc.new_csVector3Array(*args))
        _swig_setattr(self, csVector3Array, 'thisown', 1)
    def AddVertexSmart(*args): return _blcelc.csVector3Array_AddVertexSmart(*args)
    __swig_getmethods__["CompressVertices"] = lambda x: _blcelc.csVector3Array_CompressVertices
    if _newclass:CompressVertices = staticmethod(_blcelc.csVector3Array_CompressVertices)
    __swig_getmethods__["CompressVertices"] = lambda x: _blcelc.csVector3Array_CompressVertices
    if _newclass:CompressVertices = staticmethod(_blcelc.csVector3Array_CompressVertices)
    def __del__(self, destroy=_blcelc.delete_csVector3Array):
        try:
            if self.thisown: destroy(self)
        except: pass

class csVector3ArrayPtr(csVector3Array):
    def __init__(self, this):
        _swig_setattr(self, csVector3Array, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csVector3Array, 'thisown', 0)
        _swig_setattr(self, csVector3Array,self.__class__,csVector3Array)
_blcelc.csVector3Array_swigregister(csVector3ArrayPtr)

csVector3Array_CompressVertices = _blcelc.csVector3Array_CompressVertices

class csTriangle(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csTriangle, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csTriangle, name)
    def __repr__(self):
        return "<C csTriangle instance at %s>" % (self.this,)
    __swig_setmethods__["a"] = _blcelc.csTriangle_a_set
    __swig_getmethods__["a"] = _blcelc.csTriangle_a_get
    if _newclass:a = property(_blcelc.csTriangle_a_get, _blcelc.csTriangle_a_set)
    __swig_setmethods__["b"] = _blcelc.csTriangle_b_set
    __swig_getmethods__["b"] = _blcelc.csTriangle_b_get
    if _newclass:b = property(_blcelc.csTriangle_b_get, _blcelc.csTriangle_b_set)
    __swig_setmethods__["c"] = _blcelc.csTriangle_c_set
    __swig_getmethods__["c"] = _blcelc.csTriangle_c_get
    if _newclass:c = property(_blcelc.csTriangle_c_get, _blcelc.csTriangle_c_set)
    def __init__(self, *args):
        _swig_setattr(self, csTriangle, 'this', _blcelc.new_csTriangle(*args))
        _swig_setattr(self, csTriangle, 'thisown', 1)
    def assign(*args): return _blcelc.csTriangle_assign(*args)
    def Set(*args): return _blcelc.csTriangle_Set(*args)
    def __del__(self, destroy=_blcelc.delete_csTriangle):
        try:
            if self.thisown: destroy(self)
        except: pass

class csTrianglePtr(csTriangle):
    def __init__(self, this):
        _swig_setattr(self, csTriangle, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csTriangle, 'thisown', 0)
        _swig_setattr(self, csTriangle,self.__class__,csTriangle)
_blcelc.csTriangle_swigregister(csTrianglePtr)

class csRect(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csRect, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csRect, name)
    def __repr__(self):
        return "<C csRect instance at %s>" % (self.this,)
    __swig_setmethods__["xmin"] = _blcelc.csRect_xmin_set
    __swig_getmethods__["xmin"] = _blcelc.csRect_xmin_get
    if _newclass:xmin = property(_blcelc.csRect_xmin_get, _blcelc.csRect_xmin_set)
    __swig_setmethods__["ymin"] = _blcelc.csRect_ymin_set
    __swig_getmethods__["ymin"] = _blcelc.csRect_ymin_get
    if _newclass:ymin = property(_blcelc.csRect_ymin_get, _blcelc.csRect_ymin_set)
    __swig_setmethods__["xmax"] = _blcelc.csRect_xmax_set
    __swig_getmethods__["xmax"] = _blcelc.csRect_xmax_get
    if _newclass:xmax = property(_blcelc.csRect_xmax_get, _blcelc.csRect_xmax_set)
    __swig_setmethods__["ymax"] = _blcelc.csRect_ymax_set
    __swig_getmethods__["ymax"] = _blcelc.csRect_ymax_get
    if _newclass:ymax = property(_blcelc.csRect_ymax_get, _blcelc.csRect_ymax_set)
    def __init__(self, *args):
        _swig_setattr(self, csRect, 'this', _blcelc.new_csRect(*args))
        _swig_setattr(self, csRect, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csRect):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Intersect(*args): return _blcelc.csRect_Intersect(*args)
    def Intersects(*args): return _blcelc.csRect_Intersects(*args)
    def Union(*args): return _blcelc.csRect_Union(*args)
    def Exclude(*args): return _blcelc.csRect_Exclude(*args)
    def Subtract(*args): return _blcelc.csRect_Subtract(*args)
    def IsEmpty(*args): return _blcelc.csRect_IsEmpty(*args)
    def MakeEmpty(*args): return _blcelc.csRect_MakeEmpty(*args)
    def Set(*args): return _blcelc.csRect_Set(*args)
    def SetPos(*args): return _blcelc.csRect_SetPos(*args)
    def SetSize(*args): return _blcelc.csRect_SetSize(*args)
    def Move(*args): return _blcelc.csRect_Move(*args)
    def Width(*args): return _blcelc.csRect_Width(*args)
    def Height(*args): return _blcelc.csRect_Height(*args)
    def Contains(*args): return _blcelc.csRect_Contains(*args)
    def ContainsRel(*args): return _blcelc.csRect_ContainsRel(*args)
    def Equal(*args): return _blcelc.csRect_Equal(*args)
    def Normalize(*args): return _blcelc.csRect_Normalize(*args)
    def Area(*args): return _blcelc.csRect_Area(*args)
    def AddAdjanced(*args): return _blcelc.csRect_AddAdjanced(*args)
    def __eq__(*args): return _blcelc.csRect___eq__(*args)
    def __ne__(*args): return _blcelc.csRect___ne__(*args)
    def Extend(*args): return _blcelc.csRect_Extend(*args)
    def Join(*args): return _blcelc.csRect_Join(*args)
    def Outset(*args): return _blcelc.csRect_Outset(*args)
    def Inset(*args): return _blcelc.csRect_Inset(*args)
    def ClipLineGeneral(*args): return _blcelc.csRect_ClipLineGeneral(*args)
    def ClipLine(*args): return _blcelc.csRect_ClipLine(*args)
    def ClipLineSafe(*args): return _blcelc.csRect_ClipLineSafe(*args)

class csRectPtr(csRect):
    def __init__(self, this):
        _swig_setattr(self, csRect, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csRect, 'thisown', 0)
        _swig_setattr(self, csRect,self.__class__,csRect)
_blcelc.csRect_swigregister(csRectPtr)

class csRectRegion(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csRectRegion, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csRectRegion, name)
    def __repr__(self):
        return "<C csRectRegion instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csRectRegion, 'this', _blcelc.new_csRectRegion(*args))
        _swig_setattr(self, csRectRegion, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csRectRegion):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Include(*args): return _blcelc.csRectRegion_Include(*args)
    def Exclude(*args): return _blcelc.csRectRegion_Exclude(*args)
    def ClipTo(*args): return _blcelc.csRectRegion_ClipTo(*args)
    def Count(*args): return _blcelc.csRectRegion_Count(*args)
    def RectAt(*args): return _blcelc.csRectRegion_RectAt(*args)
    def makeEmpty(*args): return _blcelc.csRectRegion_makeEmpty(*args)

class csRectRegionPtr(csRectRegion):
    def __init__(self, this):
        _swig_setattr(self, csRectRegion, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csRectRegion, 'thisown', 0)
        _swig_setattr(self, csRectRegion,self.__class__,csRectRegion)
_blcelc.csRectRegion_swigregister(csRectRegionPtr)
FRAGMENT_BUFFER_SIZE = cvar.FRAGMENT_BUFFER_SIZE

class csQuaternion(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csQuaternion, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csQuaternion, name)
    def __repr__(self):
        return "<C csQuaternion instance at %s>" % (self.this,)
    def Init(*args): return _blcelc.csQuaternion_Init(*args)
    def __init__(self, *args):
        _swig_setattr(self, csQuaternion, 'this', _blcelc.new_csQuaternion(*args))
        _swig_setattr(self, csQuaternion, 'thisown', 1)
    def __imul__(*args): return _blcelc.csQuaternion___imul__(*args)
    def Conjugate(*args): return _blcelc.csQuaternion_Conjugate(*args)
    def Negate(*args): return _blcelc.csQuaternion_Negate(*args)
    def Invert(*args): return _blcelc.csQuaternion_Invert(*args)
    def GetAxisAngle(*args): return _blcelc.csQuaternion_GetAxisAngle(*args)
    def SetWithAxisAngle(*args): return _blcelc.csQuaternion_SetWithAxisAngle(*args)
    def PrepRotation(*args): return _blcelc.csQuaternion_PrepRotation(*args)
    def Rotate(*args): return _blcelc.csQuaternion_Rotate(*args)
    def Normalize(*args): return _blcelc.csQuaternion_Normalize(*args)
    def SetWithEuler(*args): return _blcelc.csQuaternion_SetWithEuler(*args)
    def GetEulerAngles(*args): return _blcelc.csQuaternion_GetEulerAngles(*args)
    def ToAxisAngle(*args): return _blcelc.csQuaternion_ToAxisAngle(*args)
    def Slerp(*args): return _blcelc.csQuaternion_Slerp(*args)
    __swig_setmethods__["r"] = _blcelc.csQuaternion_r_set
    __swig_getmethods__["r"] = _blcelc.csQuaternion_r_get
    if _newclass:r = property(_blcelc.csQuaternion_r_get, _blcelc.csQuaternion_r_set)
    __swig_setmethods__["x"] = _blcelc.csQuaternion_x_set
    __swig_getmethods__["x"] = _blcelc.csQuaternion_x_get
    if _newclass:x = property(_blcelc.csQuaternion_x_get, _blcelc.csQuaternion_x_set)
    __swig_setmethods__["y"] = _blcelc.csQuaternion_y_set
    __swig_getmethods__["y"] = _blcelc.csQuaternion_y_get
    if _newclass:y = property(_blcelc.csQuaternion_y_get, _blcelc.csQuaternion_y_set)
    __swig_setmethods__["z"] = _blcelc.csQuaternion_z_set
    __swig_getmethods__["z"] = _blcelc.csQuaternion_z_get
    if _newclass:z = property(_blcelc.csQuaternion_z_get, _blcelc.csQuaternion_z_set)
    def __add__(*args): return _blcelc.csQuaternion___add__(*args)
    def __sub__(*args): return _blcelc.csQuaternion___sub__(*args)
    def __mul__(*args): return _blcelc.csQuaternion___mul__(*args)
    def __del__(self, destroy=_blcelc.delete_csQuaternion):
        try:
            if self.thisown: destroy(self)
        except: pass

class csQuaternionPtr(csQuaternion):
    def __init__(self, this):
        _swig_setattr(self, csQuaternion, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csQuaternion, 'thisown', 0)
        _swig_setattr(self, csQuaternion,self.__class__,csQuaternion)
_blcelc.csQuaternion_swigregister(csQuaternionPtr)

class csSpline(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSpline, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSpline, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C csSpline instance at %s>" % (self.this,)
    def __del__(self, destroy=_blcelc.delete_csSpline):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetDimensionCount(*args): return _blcelc.csSpline_GetDimensionCount(*args)
    def GetPointCount(*args): return _blcelc.csSpline_GetPointCount(*args)
    def InsertPoint(*args): return _blcelc.csSpline_InsertPoint(*args)
    def RemovePoint(*args): return _blcelc.csSpline_RemovePoint(*args)
    def SetTimeValues(*args): return _blcelc.csSpline_SetTimeValues(*args)
    def SetTimeValue(*args): return _blcelc.csSpline_SetTimeValue(*args)
    def GetTimeValues(*args): return _blcelc.csSpline_GetTimeValues(*args)
    def GetTimeValue(*args): return _blcelc.csSpline_GetTimeValue(*args)
    def SetDimensionValues(*args): return _blcelc.csSpline_SetDimensionValues(*args)
    def SetDimensionValue(*args): return _blcelc.csSpline_SetDimensionValue(*args)
    def GetDimensionValues(*args): return _blcelc.csSpline_GetDimensionValues(*args)
    def GetDimensionValue(*args): return _blcelc.csSpline_GetDimensionValue(*args)
    def SetIndexValues(*args): return _blcelc.csSpline_SetIndexValues(*args)
    def GetIndexValues(*args): return _blcelc.csSpline_GetIndexValues(*args)
    def Calculate(*args): return _blcelc.csSpline_Calculate(*args)
    def GetCurrentIndex(*args): return _blcelc.csSpline_GetCurrentIndex(*args)
    def GetInterpolatedDimension(*args): return _blcelc.csSpline_GetInterpolatedDimension(*args)

class csSplinePtr(csSpline):
    def __init__(self, this):
        _swig_setattr(self, csSpline, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csSpline, 'thisown', 0)
        _swig_setattr(self, csSpline,self.__class__,csSpline)
_blcelc.csSpline_swigregister(csSplinePtr)

class csCubicSpline(csSpline):
    __swig_setmethods__ = {}
    for _s in [csSpline]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csCubicSpline, name, value)
    __swig_getmethods__ = {}
    for _s in [csSpline]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csCubicSpline, name)
    def __repr__(self):
        return "<C csCubicSpline instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csCubicSpline, 'this', _blcelc.new_csCubicSpline(*args))
        _swig_setattr(self, csCubicSpline, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csCubicSpline):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Calculate(*args): return _blcelc.csCubicSpline_Calculate(*args)
    def GetInterpolatedDimension(*args): return _blcelc.csCubicSpline_GetInterpolatedDimension(*args)

class csCubicSplinePtr(csCubicSpline):
    def __init__(self, this):
        _swig_setattr(self, csCubicSpline, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csCubicSpline, 'thisown', 0)
        _swig_setattr(self, csCubicSpline,self.__class__,csCubicSpline)
_blcelc.csCubicSpline_swigregister(csCubicSplinePtr)

class csBSpline(csSpline):
    __swig_setmethods__ = {}
    for _s in [csSpline]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csBSpline, name, value)
    __swig_getmethods__ = {}
    for _s in [csSpline]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csBSpline, name)
    def __repr__(self):
        return "<C csBSpline instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csBSpline, 'this', _blcelc.new_csBSpline(*args))
        _swig_setattr(self, csBSpline, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csBSpline):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Calculate(*args): return _blcelc.csBSpline_Calculate(*args)
    def GetInterpolatedDimension(*args): return _blcelc.csBSpline_GetInterpolatedDimension(*args)

class csBSplinePtr(csBSpline):
    def __init__(self, this):
        _swig_setattr(self, csBSpline, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csBSpline, 'thisown', 0)
        _swig_setattr(self, csBSpline,self.__class__,csBSpline)
_blcelc.csBSpline_swigregister(csBSplinePtr)

class csCatmullRomSpline(csBSpline):
    __swig_setmethods__ = {}
    for _s in [csBSpline]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csCatmullRomSpline, name, value)
    __swig_getmethods__ = {}
    for _s in [csBSpline]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csCatmullRomSpline, name)
    def __repr__(self):
        return "<C csCatmullRomSpline instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csCatmullRomSpline, 'this', _blcelc.new_csCatmullRomSpline(*args))
        _swig_setattr(self, csCatmullRomSpline, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csCatmullRomSpline):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Clone(*args): return _blcelc.csCatmullRomSpline_Clone(*args)

class csCatmullRomSplinePtr(csCatmullRomSpline):
    def __init__(self, this):
        _swig_setattr(self, csCatmullRomSpline, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csCatmullRomSpline, 'thisown', 0)
        _swig_setattr(self, csCatmullRomSpline,self.__class__,csCatmullRomSpline)
_blcelc.csCatmullRomSpline_swigregister(csCatmullRomSplinePtr)

class csPoint(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPoint, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPoint, name)
    def __repr__(self):
        return "<C csPoint instance at %s>" % (self.this,)
    __swig_setmethods__["x"] = _blcelc.csPoint_x_set
    __swig_getmethods__["x"] = _blcelc.csPoint_x_get
    if _newclass:x = property(_blcelc.csPoint_x_get, _blcelc.csPoint_x_set)
    __swig_setmethods__["y"] = _blcelc.csPoint_y_set
    __swig_getmethods__["y"] = _blcelc.csPoint_y_get
    if _newclass:y = property(_blcelc.csPoint_y_get, _blcelc.csPoint_y_set)
    def __init__(self, *args):
        _swig_setattr(self, csPoint, 'this', _blcelc.new_csPoint(*args))
        _swig_setattr(self, csPoint, 'thisown', 1)
    def Set(*args): return _blcelc.csPoint_Set(*args)
    def __del__(self, destroy=_blcelc.delete_csPoint):
        try:
            if self.thisown: destroy(self)
        except: pass

class csPointPtr(csPoint):
    def __init__(self, this):
        _swig_setattr(self, csPoint, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPoint, 'thisown', 0)
        _swig_setattr(self, csPoint,self.__class__,csPoint)
_blcelc.csPoint_swigregister(csPointPtr)

CS_BOUNDINGBOX_MAXVALUE = _blcelc.CS_BOUNDINGBOX_MAXVALUE
CS_BOX_CORNER_xy = _blcelc.CS_BOX_CORNER_xy
CS_BOX_CORNER_xY = _blcelc.CS_BOX_CORNER_xY
CS_BOX_CORNER_Xy = _blcelc.CS_BOX_CORNER_Xy
CS_BOX_CORNER_XY = _blcelc.CS_BOX_CORNER_XY
CS_BOX_CENTER2 = _blcelc.CS_BOX_CENTER2
CS_BOX_EDGE_xy_Xy = _blcelc.CS_BOX_EDGE_xy_Xy
CS_BOX_EDGE_Xy_xy = _blcelc.CS_BOX_EDGE_Xy_xy
CS_BOX_EDGE_Xy_XY = _blcelc.CS_BOX_EDGE_Xy_XY
CS_BOX_EDGE_XY_Xy = _blcelc.CS_BOX_EDGE_XY_Xy
CS_BOX_EDGE_XY_xY = _blcelc.CS_BOX_EDGE_XY_xY
CS_BOX_EDGE_xY_XY = _blcelc.CS_BOX_EDGE_xY_XY
CS_BOX_EDGE_xY_xy = _blcelc.CS_BOX_EDGE_xY_xy
CS_BOX_EDGE_xy_xY = _blcelc.CS_BOX_EDGE_xy_xY
class csBox2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csBox2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csBox2, name)
    def __repr__(self):
        return "<C csBox2 instance at %s>" % (self.this,)
    def MinX(*args): return _blcelc.csBox2_MinX(*args)
    def MinY(*args): return _blcelc.csBox2_MinY(*args)
    def MaxX(*args): return _blcelc.csBox2_MaxX(*args)
    def MaxY(*args): return _blcelc.csBox2_MaxY(*args)
    def Min(*args): return _blcelc.csBox2_Min(*args)
    def Max(*args): return _blcelc.csBox2_Max(*args)
    def GetCorner(*args): return _blcelc.csBox2_GetCorner(*args)
    def GetCenter(*args): return _blcelc.csBox2_GetCenter(*args)
    def SetCenter(*args): return _blcelc.csBox2_SetCenter(*args)
    def SetSize(*args): return _blcelc.csBox2_SetSize(*args)
    def GetEdgeInfo(*args): return _blcelc.csBox2_GetEdgeInfo(*args)
    def GetEdge(*args): return _blcelc.csBox2_GetEdge(*args)
    __swig_getmethods__["Intersect"] = lambda x: _blcelc.csBox2_Intersect
    if _newclass:Intersect = staticmethod(_blcelc.csBox2_Intersect)
    __swig_getmethods__["Intersect"] = lambda x: _blcelc.csBox2_Intersect
    if _newclass:Intersect = staticmethod(_blcelc.csBox2_Intersect)
    def Intersect(*args): return _blcelc.csBox2_Intersect(*args)
    def In(*args): return _blcelc.csBox2_In(*args)
    def Overlap(*args): return _blcelc.csBox2_Overlap(*args)
    def Contains(*args): return _blcelc.csBox2_Contains(*args)
    def Empty(*args): return _blcelc.csBox2_Empty(*args)
    def SquaredOriginDist(*args): return _blcelc.csBox2_SquaredOriginDist(*args)
    def SquaredOriginMaxDist(*args): return _blcelc.csBox2_SquaredOriginMaxDist(*args)
    def StartBoundingBox(*args): return _blcelc.csBox2_StartBoundingBox(*args)
    def AddBoundingVertex(*args): return _blcelc.csBox2_AddBoundingVertex(*args)
    def AddBoundingVertexSmart(*args): return _blcelc.csBox2_AddBoundingVertexSmart(*args)
    def AddBoundingVertexTest(*args): return _blcelc.csBox2_AddBoundingVertexTest(*args)
    def AddBoundingVertexSmartTest(*args): return _blcelc.csBox2_AddBoundingVertexSmartTest(*args)
    def __init__(self, *args):
        _swig_setattr(self, csBox2, 'this', _blcelc.new_csBox2(*args))
        _swig_setattr(self, csBox2, 'thisown', 1)
    def Set(*args): return _blcelc.csBox2_Set(*args)
    def SetMin(*args): return _blcelc.csBox2_SetMin(*args)
    def SetMax(*args): return _blcelc.csBox2_SetMax(*args)
    def __iadd__(*args): return _blcelc.csBox2___iadd__(*args)
    def __imul__(*args): return _blcelc.csBox2___imul__(*args)
    def TestIntersect(*args): return _blcelc.csBox2_TestIntersect(*args)
    def __mul__(*args): return _blcelc.csBox2___mul__(*args)
    def __ne__(*args): return _blcelc.csBox2___ne__(*args)
    def __gt__(*args): return _blcelc.csBox2___gt__(*args)
    def __add__(*args): return _blcelc.csBox2___add__(*args)
    def __lt__(*args): return _blcelc.csBox2___lt__(*args)
    def __del__(self, destroy=_blcelc.delete_csBox2):
        try:
            if self.thisown: destroy(self)
        except: pass

class csBox2Ptr(csBox2):
    def __init__(self, this):
        _swig_setattr(self, csBox2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csBox2, 'thisown', 0)
        _swig_setattr(self, csBox2,self.__class__,csBox2)
_blcelc.csBox2_swigregister(csBox2Ptr)

CS_BOX_CORNER_xyz = _blcelc.CS_BOX_CORNER_xyz
CS_BOX_CORNER_xyZ = _blcelc.CS_BOX_CORNER_xyZ
CS_BOX_CORNER_xYz = _blcelc.CS_BOX_CORNER_xYz
CS_BOX_CORNER_xYZ = _blcelc.CS_BOX_CORNER_xYZ
CS_BOX_CORNER_Xyz = _blcelc.CS_BOX_CORNER_Xyz
CS_BOX_CORNER_XyZ = _blcelc.CS_BOX_CORNER_XyZ
CS_BOX_CORNER_XYz = _blcelc.CS_BOX_CORNER_XYz
CS_BOX_CORNER_XYZ = _blcelc.CS_BOX_CORNER_XYZ
CS_BOX_CENTER3 = _blcelc.CS_BOX_CENTER3
CS_BOX_SIDE_x = _blcelc.CS_BOX_SIDE_x
CS_BOX_SIDE_X = _blcelc.CS_BOX_SIDE_X
CS_BOX_SIDE_y = _blcelc.CS_BOX_SIDE_y
CS_BOX_SIDE_Y = _blcelc.CS_BOX_SIDE_Y
CS_BOX_SIDE_z = _blcelc.CS_BOX_SIDE_z
CS_BOX_SIDE_Z = _blcelc.CS_BOX_SIDE_Z
CS_BOX_INSIDE = _blcelc.CS_BOX_INSIDE
CS_BOX_EDGE_Xyz_xyz = _blcelc.CS_BOX_EDGE_Xyz_xyz
CS_BOX_EDGE_xyz_Xyz = _blcelc.CS_BOX_EDGE_xyz_Xyz
CS_BOX_EDGE_xyz_xYz = _blcelc.CS_BOX_EDGE_xyz_xYz
CS_BOX_EDGE_xYz_xyz = _blcelc.CS_BOX_EDGE_xYz_xyz
CS_BOX_EDGE_xYz_XYz = _blcelc.CS_BOX_EDGE_xYz_XYz
CS_BOX_EDGE_XYz_xYz = _blcelc.CS_BOX_EDGE_XYz_xYz
CS_BOX_EDGE_XYz_Xyz = _blcelc.CS_BOX_EDGE_XYz_Xyz
CS_BOX_EDGE_Xyz_XYz = _blcelc.CS_BOX_EDGE_Xyz_XYz
CS_BOX_EDGE_Xyz_XyZ = _blcelc.CS_BOX_EDGE_Xyz_XyZ
CS_BOX_EDGE_XyZ_Xyz = _blcelc.CS_BOX_EDGE_XyZ_Xyz
CS_BOX_EDGE_XyZ_XYZ = _blcelc.CS_BOX_EDGE_XyZ_XYZ
CS_BOX_EDGE_XYZ_XyZ = _blcelc.CS_BOX_EDGE_XYZ_XyZ
CS_BOX_EDGE_XYZ_XYz = _blcelc.CS_BOX_EDGE_XYZ_XYz
CS_BOX_EDGE_XYz_XYZ = _blcelc.CS_BOX_EDGE_XYz_XYZ
CS_BOX_EDGE_XYZ_xYZ = _blcelc.CS_BOX_EDGE_XYZ_xYZ
CS_BOX_EDGE_xYZ_XYZ = _blcelc.CS_BOX_EDGE_xYZ_XYZ
CS_BOX_EDGE_xYZ_xYz = _blcelc.CS_BOX_EDGE_xYZ_xYz
CS_BOX_EDGE_xYz_xYZ = _blcelc.CS_BOX_EDGE_xYz_xYZ
CS_BOX_EDGE_xYZ_xyZ = _blcelc.CS_BOX_EDGE_xYZ_xyZ
CS_BOX_EDGE_xyZ_xYZ = _blcelc.CS_BOX_EDGE_xyZ_xYZ
CS_BOX_EDGE_xyZ_xyz = _blcelc.CS_BOX_EDGE_xyZ_xyz
CS_BOX_EDGE_xyz_xyZ = _blcelc.CS_BOX_EDGE_xyz_xyZ
CS_BOX_EDGE_xyZ_XyZ = _blcelc.CS_BOX_EDGE_xyZ_XyZ
CS_BOX_EDGE_XyZ_xyZ = _blcelc.CS_BOX_EDGE_XyZ_xyZ
class csBox3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csBox3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csBox3, name)
    def __repr__(self):
        return "<C csBox3 instance at %s>" % (self.this,)
    def MinX(*args): return _blcelc.csBox3_MinX(*args)
    def MinY(*args): return _blcelc.csBox3_MinY(*args)
    def MinZ(*args): return _blcelc.csBox3_MinZ(*args)
    def MaxX(*args): return _blcelc.csBox3_MaxX(*args)
    def MaxY(*args): return _blcelc.csBox3_MaxY(*args)
    def MaxZ(*args): return _blcelc.csBox3_MaxZ(*args)
    def Min(*args): return _blcelc.csBox3_Min(*args)
    def Max(*args): return _blcelc.csBox3_Max(*args)
    def GetCorner(*args): return _blcelc.csBox3_GetCorner(*args)
    def GetEdgeInfo(*args): return _blcelc.csBox3_GetEdgeInfo(*args)
    def GetFaceEdges(*args): return _blcelc.csBox3_GetFaceEdges(*args)
    def GetCenter(*args): return _blcelc.csBox3_GetCenter(*args)
    def SetCenter(*args): return _blcelc.csBox3_SetCenter(*args)
    def SetSize(*args): return _blcelc.csBox3_SetSize(*args)
    def GetSide(*args): return _blcelc.csBox3_GetSide(*args)
    def GetVisibleSides(*args): return _blcelc.csBox3_GetVisibleSides(*args)
    __swig_getmethods__["OtherSide"] = lambda x: _blcelc.csBox3_OtherSide
    if _newclass:OtherSide = staticmethod(_blcelc.csBox3_OtherSide)
    def GetEdge(*args): return _blcelc.csBox3_GetEdge(*args)
    def In(*args): return _blcelc.csBox3_In(*args)
    def Overlap(*args): return _blcelc.csBox3_Overlap(*args)
    def Contains(*args): return _blcelc.csBox3_Contains(*args)
    def Empty(*args): return _blcelc.csBox3_Empty(*args)
    def StartBoundingBox(*args): return _blcelc.csBox3_StartBoundingBox(*args)
    def AddBoundingVertex(*args): return _blcelc.csBox3_AddBoundingVertex(*args)
    def AddBoundingVertexSmart(*args): return _blcelc.csBox3_AddBoundingVertexSmart(*args)
    def AddBoundingVertexTest(*args): return _blcelc.csBox3_AddBoundingVertexTest(*args)
    def AddBoundingVertexSmartTest(*args): return _blcelc.csBox3_AddBoundingVertexSmartTest(*args)
    def __init__(self, *args):
        _swig_setattr(self, csBox3, 'this', _blcelc.new_csBox3(*args))
        _swig_setattr(self, csBox3, 'thisown', 1)
    def Set(*args): return _blcelc.csBox3_Set(*args)
    def SetMin(*args): return _blcelc.csBox3_SetMin(*args)
    def SetMax(*args): return _blcelc.csBox3_SetMax(*args)
    def Split(*args): return _blcelc.csBox3_Split(*args)
    def TestSplit(*args): return _blcelc.csBox3_TestSplit(*args)
    def AdjacentX(*args): return _blcelc.csBox3_AdjacentX(*args)
    def AdjacentY(*args): return _blcelc.csBox3_AdjacentY(*args)
    def AdjacentZ(*args): return _blcelc.csBox3_AdjacentZ(*args)
    def Adjacent(*args): return _blcelc.csBox3_Adjacent(*args)
    def CalculatePointSegment(*args): return _blcelc.csBox3_CalculatePointSegment(*args)
    def GetConvexOutline(*args): return _blcelc.csBox3_GetConvexOutline(*args)
    def Between(*args): return _blcelc.csBox3_Between(*args)
    def ManhattanDistance(*args): return _blcelc.csBox3_ManhattanDistance(*args)
    def SquaredOriginDist(*args): return _blcelc.csBox3_SquaredOriginDist(*args)
    def SquaredOriginMaxDist(*args): return _blcelc.csBox3_SquaredOriginMaxDist(*args)
    def ProjectBox(*args): return _blcelc.csBox3_ProjectBox(*args)
    def ProjectOutline(*args): return _blcelc.csBox3_ProjectOutline(*args)
    def ProjectBoxAndOutline(*args): return _blcelc.csBox3_ProjectBoxAndOutline(*args)
    def __iadd__(*args): return _blcelc.csBox3___iadd__(*args)
    def __imul__(*args): return _blcelc.csBox3___imul__(*args)
    def TestIntersect(*args): return _blcelc.csBox3_TestIntersect(*args)
    def __mul__(*args): return _blcelc.csBox3___mul__(*args)
    def __ne__(*args): return _blcelc.csBox3___ne__(*args)
    def __gt__(*args): return _blcelc.csBox3___gt__(*args)
    def __add__(*args): return _blcelc.csBox3___add__(*args)
    def __lt__(*args): return _blcelc.csBox3___lt__(*args)
    def __del__(self, destroy=_blcelc.delete_csBox3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csBox3Ptr(csBox3):
    def __init__(self, this):
        _swig_setattr(self, csBox3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csBox3, 'thisown', 0)
        _swig_setattr(self, csBox3,self.__class__,csBox3)
_blcelc.csBox3_swigregister(csBox3Ptr)

csBox3_OtherSide = _blcelc.csBox3_OtherSide

class csSegment2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSegment2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSegment2, name)
    def __repr__(self):
        return "<C csSegment2 instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csSegment2, 'this', _blcelc.new_csSegment2(*args))
        _swig_setattr(self, csSegment2, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csSegment2):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Set(*args): return _blcelc.csSegment2_Set(*args)
    def SetStart(*args): return _blcelc.csSegment2_SetStart(*args)
    def SetEnd(*args): return _blcelc.csSegment2_SetEnd(*args)
    def Start(*args): return _blcelc.csSegment2_Start(*args)
    def End(*args): return _blcelc.csSegment2_End(*args)

class csSegment2Ptr(csSegment2):
    def __init__(self, this):
        _swig_setattr(self, csSegment2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csSegment2, 'thisown', 0)
        _swig_setattr(self, csSegment2,self.__class__,csSegment2)
_blcelc.csSegment2_swigregister(csSegment2Ptr)

class csSegment3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSegment3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSegment3, name)
    def __repr__(self):
        return "<C csSegment3 instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csSegment3, 'this', _blcelc.new_csSegment3(*args))
        _swig_setattr(self, csSegment3, 'thisown', 1)
    def Set(*args): return _blcelc.csSegment3_Set(*args)
    def SetStart(*args): return _blcelc.csSegment3_SetStart(*args)
    def SetEnd(*args): return _blcelc.csSegment3_SetEnd(*args)
    def Start(*args): return _blcelc.csSegment3_Start(*args)
    def End(*args): return _blcelc.csSegment3_End(*args)
    def __del__(self, destroy=_blcelc.delete_csSegment3):
        try:
            if self.thisown: destroy(self)
        except: pass

class csSegment3Ptr(csSegment3):
    def __init__(self, this):
        _swig_setattr(self, csSegment3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csSegment3, 'thisown', 0)
        _swig_setattr(self, csSegment3,self.__class__,csSegment3)
_blcelc.csSegment3_swigregister(csSegment3Ptr)

class csRGBcolor(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csRGBcolor, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csRGBcolor, name)
    def __repr__(self):
        return "<C csRGBcolor instance at %s>" % (self.this,)
    __swig_setmethods__["red"] = _blcelc.csRGBcolor_red_set
    __swig_getmethods__["red"] = _blcelc.csRGBcolor_red_get
    if _newclass:red = property(_blcelc.csRGBcolor_red_get, _blcelc.csRGBcolor_red_set)
    __swig_setmethods__["green"] = _blcelc.csRGBcolor_green_set
    __swig_getmethods__["green"] = _blcelc.csRGBcolor_green_get
    if _newclass:green = property(_blcelc.csRGBcolor_green_get, _blcelc.csRGBcolor_green_set)
    __swig_setmethods__["blue"] = _blcelc.csRGBcolor_blue_set
    __swig_getmethods__["blue"] = _blcelc.csRGBcolor_blue_get
    if _newclass:blue = property(_blcelc.csRGBcolor_blue_get, _blcelc.csRGBcolor_blue_set)
    def __init__(self, *args):
        _swig_setattr(self, csRGBcolor, 'this', _blcelc.new_csRGBcolor(*args))
        _swig_setattr(self, csRGBcolor, 'thisown', 1)
    def Set(*args): return _blcelc.csRGBcolor_Set(*args)
    def __eq__(*args): return _blcelc.csRGBcolor___eq__(*args)
    def __ne__(*args): return _blcelc.csRGBcolor___ne__(*args)
    def __add__(*args): return _blcelc.csRGBcolor___add__(*args)
    def __del__(self, destroy=_blcelc.delete_csRGBcolor):
        try:
            if self.thisown: destroy(self)
        except: pass

class csRGBcolorPtr(csRGBcolor):
    def __init__(self, this):
        _swig_setattr(self, csRGBcolor, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csRGBcolor, 'thisown', 0)
        _swig_setattr(self, csRGBcolor,self.__class__,csRGBcolor)
_blcelc.csRGBcolor_swigregister(csRGBcolorPtr)

class csRGBpixel(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csRGBpixel, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csRGBpixel, name)
    def __repr__(self):
        return "<C csRGBpixel instance at %s>" % (self.this,)
    __swig_setmethods__["red"] = _blcelc.csRGBpixel_red_set
    __swig_getmethods__["red"] = _blcelc.csRGBpixel_red_get
    if _newclass:red = property(_blcelc.csRGBpixel_red_get, _blcelc.csRGBpixel_red_set)
    __swig_setmethods__["green"] = _blcelc.csRGBpixel_green_set
    __swig_getmethods__["green"] = _blcelc.csRGBpixel_green_get
    if _newclass:green = property(_blcelc.csRGBpixel_green_get, _blcelc.csRGBpixel_green_set)
    __swig_setmethods__["blue"] = _blcelc.csRGBpixel_blue_set
    __swig_getmethods__["blue"] = _blcelc.csRGBpixel_blue_get
    if _newclass:blue = property(_blcelc.csRGBpixel_blue_get, _blcelc.csRGBpixel_blue_set)
    __swig_setmethods__["alpha"] = _blcelc.csRGBpixel_alpha_set
    __swig_getmethods__["alpha"] = _blcelc.csRGBpixel_alpha_get
    if _newclass:alpha = property(_blcelc.csRGBpixel_alpha_get, _blcelc.csRGBpixel_alpha_set)
    def __init__(self, *args):
        _swig_setattr(self, csRGBpixel, 'this', _blcelc.new_csRGBpixel(*args))
        _swig_setattr(self, csRGBpixel, 'thisown', 1)
    def __eq__(*args): return _blcelc.csRGBpixel___eq__(*args)
    def __ne__(*args): return _blcelc.csRGBpixel___ne__(*args)
    def asRGBcolor(*args): return _blcelc.csRGBpixel_asRGBcolor(*args)
    def eq(*args): return _blcelc.csRGBpixel_eq(*args)
    def Intensity(*args): return _blcelc.csRGBpixel_Intensity(*args)
    def Luminance(*args): return _blcelc.csRGBpixel_Luminance(*args)
    def Set(*args): return _blcelc.csRGBpixel_Set(*args)
    def __iadd__(*args): return _blcelc.csRGBpixel___iadd__(*args)
    def UnsafeAdd(*args): return _blcelc.csRGBpixel_UnsafeAdd(*args)
    def SafeAdd(*args): return _blcelc.csRGBpixel_SafeAdd(*args)
    def __del__(self, destroy=_blcelc.delete_csRGBpixel):
        try:
            if self.thisown: destroy(self)
        except: pass

class csRGBpixelPtr(csRGBpixel):
    def __init__(self, this):
        _swig_setattr(self, csRGBpixel, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csRGBpixel, 'thisown', 0)
        _swig_setattr(self, csRGBpixel,self.__class__,csRGBpixel)
_blcelc.csRGBpixel_swigregister(csRGBpixelPtr)

R_COEF = _blcelc.R_COEF
G_COEF = _blcelc.G_COEF
B_COEF = _blcelc.B_COEF
R_COEF_SQ = _blcelc.R_COEF_SQ
G_COEF_SQ = _blcelc.G_COEF_SQ
B_COEF_SQ = _blcelc.B_COEF_SQ

csDefaultRunLoop = _blcelc.csDefaultRunLoop

csPlatformStartup = _blcelc.csPlatformStartup

csPlatformShutdown = _blcelc.csPlatformShutdown

csPrintf = _blcelc.csPrintf

csFPutErr = _blcelc.csFPutErr

csPrintfErr = _blcelc.csPrintfErr

csGetTicks = _blcelc.csGetTicks

csSleep = _blcelc.csSleep

csGetUsername = _blcelc.csGetUsername

csGetPlatformConfigPath = _blcelc.csGetPlatformConfigPath
class csPluginRequest(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPluginRequest, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPluginRequest, name)
    def __repr__(self):
        return "<C csPluginRequest instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csPluginRequest, 'this', _blcelc.new_csPluginRequest(*args))
        _swig_setattr(self, csPluginRequest, 'thisown', 1)
    def assign(*args): return _blcelc.csPluginRequest_assign(*args)
    def __eq__(*args): return _blcelc.csPluginRequest___eq__(*args)
    def __ne__(*args): return _blcelc.csPluginRequest___ne__(*args)
    def GetClassName(*args): return _blcelc.csPluginRequest_GetClassName(*args)
    def GetInterfaceName(*args): return _blcelc.csPluginRequest_GetInterfaceName(*args)
    def GetInterfaceID(*args): return _blcelc.csPluginRequest_GetInterfaceID(*args)
    def GetInterfaceVersion(*args): return _blcelc.csPluginRequest_GetInterfaceVersion(*args)
    def __del__(self, destroy=_blcelc.delete_csPluginRequest):
        try:
            if self.thisown: destroy(self)
        except: pass

class csPluginRequestPtr(csPluginRequest):
    def __init__(self, this):
        _swig_setattr(self, csPluginRequest, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPluginRequest, 'thisown', 0)
        _swig_setattr(self, csPluginRequest,self.__class__,csPluginRequest)
_blcelc.csPluginRequest_swigregister(csPluginRequestPtr)

class csInitializer(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csInitializer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csInitializer, name)
    def __repr__(self):
        return "<C csInitializer instance at %s>" % (self.this,)
    __swig_getmethods__["CreateEnvironment"] = lambda x: _blcelc.csInitializer_CreateEnvironment
    if _newclass:CreateEnvironment = staticmethod(_blcelc.csInitializer_CreateEnvironment)
    __swig_getmethods__["InitializeSCF"] = lambda x: _blcelc.csInitializer_InitializeSCF
    if _newclass:InitializeSCF = staticmethod(_blcelc.csInitializer_InitializeSCF)
    __swig_getmethods__["CreateObjectRegistry"] = lambda x: _blcelc.csInitializer_CreateObjectRegistry
    if _newclass:CreateObjectRegistry = staticmethod(_blcelc.csInitializer_CreateObjectRegistry)
    __swig_getmethods__["CreatePluginManager"] = lambda x: _blcelc.csInitializer_CreatePluginManager
    if _newclass:CreatePluginManager = staticmethod(_blcelc.csInitializer_CreatePluginManager)
    __swig_getmethods__["CreateEventQueue"] = lambda x: _blcelc.csInitializer_CreateEventQueue
    if _newclass:CreateEventQueue = staticmethod(_blcelc.csInitializer_CreateEventQueue)
    __swig_getmethods__["CreateVirtualClock"] = lambda x: _blcelc.csInitializer_CreateVirtualClock
    if _newclass:CreateVirtualClock = staticmethod(_blcelc.csInitializer_CreateVirtualClock)
    __swig_getmethods__["CreateCommandLineParser"] = lambda x: _blcelc.csInitializer_CreateCommandLineParser
    if _newclass:CreateCommandLineParser = staticmethod(_blcelc.csInitializer_CreateCommandLineParser)
    __swig_getmethods__["CreateVerbosityManager"] = lambda x: _blcelc.csInitializer_CreateVerbosityManager
    if _newclass:CreateVerbosityManager = staticmethod(_blcelc.csInitializer_CreateVerbosityManager)
    __swig_getmethods__["CreateConfigManager"] = lambda x: _blcelc.csInitializer_CreateConfigManager
    if _newclass:CreateConfigManager = staticmethod(_blcelc.csInitializer_CreateConfigManager)
    __swig_getmethods__["CreateInputDrivers"] = lambda x: _blcelc.csInitializer_CreateInputDrivers
    if _newclass:CreateInputDrivers = staticmethod(_blcelc.csInitializer_CreateInputDrivers)
    __swig_getmethods__["CreateStringSet"] = lambda x: _blcelc.csInitializer_CreateStringSet
    if _newclass:CreateStringSet = staticmethod(_blcelc.csInitializer_CreateStringSet)
    __swig_getmethods__["SetupConfigManager"] = lambda x: _blcelc.csInitializer_SetupConfigManager
    if _newclass:SetupConfigManager = staticmethod(_blcelc.csInitializer_SetupConfigManager)
    __swig_getmethods__["SetupVFS"] = lambda x: _blcelc.csInitializer_SetupVFS
    if _newclass:SetupVFS = staticmethod(_blcelc.csInitializer_SetupVFS)
    __swig_getmethods__["_RequestPlugins"] = lambda x: _blcelc.csInitializer__RequestPlugins
    if _newclass:_RequestPlugins = staticmethod(_blcelc.csInitializer__RequestPlugins)
    __swig_getmethods__["OpenApplication"] = lambda x: _blcelc.csInitializer_OpenApplication
    if _newclass:OpenApplication = staticmethod(_blcelc.csInitializer_OpenApplication)
    __swig_getmethods__["CloseApplication"] = lambda x: _blcelc.csInitializer_CloseApplication
    if _newclass:CloseApplication = staticmethod(_blcelc.csInitializer_CloseApplication)
    __swig_getmethods__["_SetupEventHandler"] = lambda x: _blcelc.csInitializer__SetupEventHandler
    if _newclass:_SetupEventHandler = staticmethod(_blcelc.csInitializer__SetupEventHandler)
    __swig_getmethods__["DestroyApplication"] = lambda x: _blcelc.csInitializer_DestroyApplication
    if _newclass:DestroyApplication = staticmethod(_blcelc.csInitializer_DestroyApplication)
    def __init__(self, *args):
        _swig_setattr(self, csInitializer, 'this', _blcelc.new_csInitializer(*args))
        _swig_setattr(self, csInitializer, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csInitializer):
        try:
            if self.thisown: destroy(self)
        except: pass

class csInitializerPtr(csInitializer):
    def __init__(self, this):
        _swig_setattr(self, csInitializer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csInitializer, 'thisown', 0)
        _swig_setattr(self, csInitializer,self.__class__,csInitializer)
_blcelc.csInitializer_swigregister(csInitializerPtr)

csInitializer_CreateEnvironment = _blcelc.csInitializer_CreateEnvironment

csInitializer_InitializeSCF = _blcelc.csInitializer_InitializeSCF

csInitializer_CreateObjectRegistry = _blcelc.csInitializer_CreateObjectRegistry

csInitializer_CreatePluginManager = _blcelc.csInitializer_CreatePluginManager

csInitializer_CreateEventQueue = _blcelc.csInitializer_CreateEventQueue

csInitializer_CreateVirtualClock = _blcelc.csInitializer_CreateVirtualClock

csInitializer_CreateCommandLineParser = _blcelc.csInitializer_CreateCommandLineParser

csInitializer_CreateVerbosityManager = _blcelc.csInitializer_CreateVerbosityManager

csInitializer_CreateConfigManager = _blcelc.csInitializer_CreateConfigManager

csInitializer_CreateInputDrivers = _blcelc.csInitializer_CreateInputDrivers

csInitializer_CreateStringSet = _blcelc.csInitializer_CreateStringSet

csInitializer_SetupConfigManager = _blcelc.csInitializer_SetupConfigManager

csInitializer_SetupVFS = _blcelc.csInitializer_SetupVFS

csInitializer__RequestPlugins = _blcelc.csInitializer__RequestPlugins

csInitializer_OpenApplication = _blcelc.csInitializer_OpenApplication

csInitializer_CloseApplication = _blcelc.csInitializer_CloseApplication

csInitializer__SetupEventHandler = _blcelc.csInitializer__SetupEventHandler

csInitializer_DestroyApplication = _blcelc.csInitializer_DestroyApplication

class csPluginRequestArray(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPluginRequestArray, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPluginRequestArray, name)
    def __repr__(self):
        return "<C csArray<(csPluginRequest)> instance at %s>" % (self.this,)
    def __del__(self, destroy=_blcelc.delete_csPluginRequestArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self, *args):
        _swig_setattr(self, csPluginRequestArray, 'this', _blcelc.new_csPluginRequestArray(*args))
        _swig_setattr(self, csPluginRequestArray, 'thisown', 1)
    def Length(*args): return _blcelc.csPluginRequestArray_Length(*args)
    def Get(*args): return _blcelc.csPluginRequestArray_Get(*args)
    def Push(*args): return _blcelc.csPluginRequestArray_Push(*args)
    def Pop(*args): return _blcelc.csPluginRequestArray_Pop(*args)
    def Top(*args): return _blcelc.csPluginRequestArray_Top(*args)
    def Insert(*args): return _blcelc.csPluginRequestArray_Insert(*args)
    def Truncate(*args): return _blcelc.csPluginRequestArray_Truncate(*args)
    def Empty(*args): return _blcelc.csPluginRequestArray_Empty(*args)
    def DeleteIndex(*args): return _blcelc.csPluginRequestArray_DeleteIndex(*args)
    def DeleteIndexFast(*args): return _blcelc.csPluginRequestArray_DeleteIndexFast(*args)
    def DeleteRange(*args): return _blcelc.csPluginRequestArray_DeleteRange(*args)
    def DeleteFast(*args): return _blcelc.csPluginRequestArray_DeleteFast(*args)

class csPluginRequestArrayPtr(csPluginRequestArray):
    def __init__(self, this):
        _swig_setattr(self, csPluginRequestArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPluginRequestArray, 'thisown', 0)
        _swig_setattr(self, csPluginRequestArray,self.__class__,csPluginRequestArray)
_blcelc.csPluginRequestArray_swigregister(csPluginRequestArrayPtr)

class iAwsKey(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsKey, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsKey, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsKey instance at %s>" % (self.this,)
    def Type(*args): return _blcelc.iAwsKey_Type(*args)
    def Name(*args): return _blcelc.iAwsKey_Name(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iAwsKey_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iAwsKey_scfGetVersion)

class iAwsKeyPtr(iAwsKey):
    def __init__(self, this):
        _swig_setattr(self, iAwsKey, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsKey, 'thisown', 0)
        _swig_setattr(self, iAwsKey,self.__class__,iAwsKey)
_blcelc.iAwsKey_swigregister(iAwsKeyPtr)
aws_debug = cvar.aws_debug
AWSF_AlwaysEraseWindows = cvar.AWSF_AlwaysEraseWindows
AWSF_AlwaysRedrawWindows = cvar.AWSF_AlwaysRedrawWindows
AWSF_RaiseOnMouseOver = cvar.AWSF_RaiseOnMouseOver
AWSF_KeyboardControl = cvar.AWSF_KeyboardControl

iAwsKey_scfGetVersion = _blcelc.iAwsKey_scfGetVersion

class iAwsIntKey(iAwsKey):
    __swig_setmethods__ = {}
    for _s in [iAwsKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsIntKey, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsIntKey, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsIntKey instance at %s>" % (self.this,)
    def Value(*args): return _blcelc.iAwsIntKey_Value(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsIntKey):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsIntKeyPtr(iAwsIntKey):
    def __init__(self, this):
        _swig_setattr(self, iAwsIntKey, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsIntKey, 'thisown', 0)
        _swig_setattr(self, iAwsIntKey,self.__class__,iAwsIntKey)
_blcelc.iAwsIntKey_swigregister(iAwsIntKeyPtr)

class iAwsFloatKey(iAwsKey):
    __swig_setmethods__ = {}
    for _s in [iAwsKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsFloatKey, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsFloatKey, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsFloatKey instance at %s>" % (self.this,)
    def Value(*args): return _blcelc.iAwsFloatKey_Value(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsFloatKey):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsFloatKeyPtr(iAwsFloatKey):
    def __init__(self, this):
        _swig_setattr(self, iAwsFloatKey, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsFloatKey, 'thisown', 0)
        _swig_setattr(self, iAwsFloatKey,self.__class__,iAwsFloatKey)
_blcelc.iAwsFloatKey_swigregister(iAwsFloatKeyPtr)

class iAwsStringKey(iAwsKey):
    __swig_setmethods__ = {}
    for _s in [iAwsKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsStringKey, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsStringKey, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsStringKey instance at %s>" % (self.this,)
    def Value(*args): return _blcelc.iAwsStringKey_Value(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsStringKey):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsStringKeyPtr(iAwsStringKey):
    def __init__(self, this):
        _swig_setattr(self, iAwsStringKey, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsStringKey, 'thisown', 0)
        _swig_setattr(self, iAwsStringKey,self.__class__,iAwsStringKey)
_blcelc.iAwsStringKey_swigregister(iAwsStringKeyPtr)

class iAwsRectKey(iAwsKey):
    __swig_setmethods__ = {}
    for _s in [iAwsKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsRectKey, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsRectKey, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsRectKey instance at %s>" % (self.this,)
    def Value(*args): return _blcelc.iAwsRectKey_Value(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsRectKey):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsRectKeyPtr(iAwsRectKey):
    def __init__(self, this):
        _swig_setattr(self, iAwsRectKey, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsRectKey, 'thisown', 0)
        _swig_setattr(self, iAwsRectKey,self.__class__,iAwsRectKey)
_blcelc.iAwsRectKey_swigregister(iAwsRectKeyPtr)

class iAwsRGBKey(iAwsKey):
    __swig_setmethods__ = {}
    for _s in [iAwsKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsRGBKey, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsRGBKey, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsRGBKey instance at %s>" % (self.this,)
    def Value(*args): return _blcelc.iAwsRGBKey_Value(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsRGBKey):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsRGBKeyPtr(iAwsRGBKey):
    def __init__(self, this):
        _swig_setattr(self, iAwsRGBKey, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsRGBKey, 'thisown', 0)
        _swig_setattr(self, iAwsRGBKey,self.__class__,iAwsRGBKey)
_blcelc.iAwsRGBKey_swigregister(iAwsRGBKeyPtr)

class iAwsPointKey(iAwsKey):
    __swig_setmethods__ = {}
    for _s in [iAwsKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsPointKey, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsPointKey, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsPointKey instance at %s>" % (self.this,)
    def Value(*args): return _blcelc.iAwsPointKey_Value(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsPointKey):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsPointKeyPtr(iAwsPointKey):
    def __init__(self, this):
        _swig_setattr(self, iAwsPointKey, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsPointKey, 'thisown', 0)
        _swig_setattr(self, iAwsPointKey,self.__class__,iAwsPointKey)
_blcelc.iAwsPointKey_swigregister(iAwsPointKeyPtr)

class iAwsConnectionKey(iAwsKey):
    __swig_setmethods__ = {}
    for _s in [iAwsKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsConnectionKey, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsConnectionKey, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsConnectionKey instance at %s>" % (self.this,)
    def Sink(*args): return _blcelc.iAwsConnectionKey_Sink(*args)
    def Trigger(*args): return _blcelc.iAwsConnectionKey_Trigger(*args)
    def Signal(*args): return _blcelc.iAwsConnectionKey_Signal(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsConnectionKey):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsConnectionKeyPtr(iAwsConnectionKey):
    def __init__(self, this):
        _swig_setattr(self, iAwsConnectionKey, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsConnectionKey, 'thisown', 0)
        _swig_setattr(self, iAwsConnectionKey,self.__class__,iAwsConnectionKey)
_blcelc.iAwsConnectionKey_swigregister(iAwsConnectionKeyPtr)

class iAwsKeyContainer(iAwsKey):
    __swig_setmethods__ = {}
    for _s in [iAwsKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsKeyContainer, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsKeyContainer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsKeyContainer instance at %s>" % (self.this,)
    def Find(*args): return _blcelc.iAwsKeyContainer_Find(*args)
    def Children(*args): return _blcelc.iAwsKeyContainer_Children(*args)
    def Add(*args): return _blcelc.iAwsKeyContainer_Add(*args)
    def GetAt(*args): return _blcelc.iAwsKeyContainer_GetAt(*args)
    def Length(*args): return _blcelc.iAwsKeyContainer_Length(*args)
    def Remove(*args): return _blcelc.iAwsKeyContainer_Remove(*args)
    def RemoveAll(*args): return _blcelc.iAwsKeyContainer_RemoveAll(*args)
    def Consume(*args): return _blcelc.iAwsKeyContainer_Consume(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsKeyContainer):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsKeyContainerPtr(iAwsKeyContainer):
    def __init__(self, this):
        _swig_setattr(self, iAwsKeyContainer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsKeyContainer, 'thisown', 0)
        _swig_setattr(self, iAwsKeyContainer,self.__class__,iAwsKeyContainer)
_blcelc.iAwsKeyContainer_swigregister(iAwsKeyContainerPtr)

class iAwsComponentNode(iAwsKeyContainer):
    __swig_setmethods__ = {}
    for _s in [iAwsKeyContainer]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsComponentNode, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsKeyContainer]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsComponentNode, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsComponentNode instance at %s>" % (self.this,)
    def ComponentTypeName(*args): return _blcelc.iAwsComponentNode_ComponentTypeName(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsComponentNode):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsComponentNodePtr(iAwsComponentNode):
    def __init__(self, this):
        _swig_setattr(self, iAwsComponentNode, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsComponentNode, 'thisown', 0)
        _swig_setattr(self, iAwsComponentNode,self.__class__,iAwsComponentNode)
_blcelc.iAwsComponentNode_swigregister(iAwsComponentNodePtr)

class iAws(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAws, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAws, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAws instance at %s>" % (self.this,)
    def GetPrefMgr(*args): return _blcelc.iAws_GetPrefMgr(*args)
    def GetSinkMgr(*args): return _blcelc.iAws_GetSinkMgr(*args)
    def SetPrefMgr(*args): return _blcelc.iAws_SetPrefMgr(*args)
    def RegisterComponentFactory(*args): return _blcelc.iAws_RegisterComponentFactory(*args)
    def FindComponentFactory(*args): return _blcelc.iAws_FindComponentFactory(*args)
    def GetTopComponent(*args): return _blcelc.iAws_GetTopComponent(*args)
    def SetTopComponent(*args): return _blcelc.iAws_SetTopComponent(*args)
    def GetFocusedComponent(*args): return _blcelc.iAws_GetFocusedComponent(*args)
    def SetFocusedComponent(*args): return _blcelc.iAws_SetFocusedComponent(*args)
    def GetKeyboardFocusedComponent(*args): return _blcelc.iAws_GetKeyboardFocusedComponent(*args)
    def ComponentAt(*args): return _blcelc.iAws_ComponentAt(*args)
    def MouseInComponent(*args): return _blcelc.iAws_MouseInComponent(*args)
    def Print(*args): return _blcelc.iAws_Print(*args)
    def Redraw(*args): return _blcelc.iAws_Redraw(*args)
    def Mark(*args): return _blcelc.iAws_Mark(*args)
    def Unmark(*args): return _blcelc.iAws_Unmark(*args)
    def Erase(*args): return _blcelc.iAws_Erase(*args)
    def MaskEraser(*args): return _blcelc.iAws_MaskEraser(*args)
    def InvalidateUpdateStore(*args): return _blcelc.iAws_InvalidateUpdateStore(*args)
    def CaptureMouse(*args): return _blcelc.iAws_CaptureMouse(*args)
    def ReleaseMouse(*args): return _blcelc.iAws_ReleaseMouse(*args)
    def SetModal(*args): return _blcelc.iAws_SetModal(*args)
    def UnSetModal(*args): return _blcelc.iAws_UnSetModal(*args)
    def HandleEvent(*args): return _blcelc.iAws_HandleEvent(*args)
    def GetCanvas(*args): return _blcelc.iAws_GetCanvas(*args)
    def G2D(*args): return _blcelc.iAws_G2D(*args)
    def G3D(*args): return _blcelc.iAws_G3D(*args)
    def CreateWindowFrom(*args): return _blcelc.iAws_CreateWindowFrom(*args)
    def CreateEmbeddableComponent(*args): return _blcelc.iAws_CreateEmbeddableComponent(*args)
    def CreateParmList(*args): return _blcelc.iAws_CreateParmList(*args)
    def CreateTransition(*args): return _blcelc.iAws_CreateTransition(*args)
    def CreateTransitionEx(*args): return _blcelc.iAws_CreateTransitionEx(*args)
    def SetFlag(*args): return _blcelc.iAws_SetFlag(*args)
    def ClearFlag(*args): return _blcelc.iAws_ClearFlag(*args)
    def GetFlags(*args): return _blcelc.iAws_GetFlags(*args)
    def GetObjectRegistry(*args): return _blcelc.iAws_GetObjectRegistry(*args)
    def AllWindowsHidden(*args): return _blcelc.iAws_AllWindowsHidden(*args)
    def ComponentIsInTransition(*args): return _blcelc.iAws_ComponentIsInTransition(*args)
    def ComponentDestroyed(*args): return _blcelc.iAws_ComponentDestroyed(*args)
    def __del__(self, destroy=_blcelc.delete_iAws):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iAws_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iAws_scfGetVersion)
    def SetupCanvas(*args): return _blcelc.iAws_SetupCanvas(*args)

class iAwsPtr(iAws):
    def __init__(self, this):
        _swig_setattr(self, iAws, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAws, 'thisown', 0)
        _swig_setattr(self, iAws,self.__class__,iAws)
_blcelc.iAws_swigregister(iAwsPtr)

iAws_scfGetVersion = _blcelc.iAws_scfGetVersion

class iAwsPrefManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsPrefManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsPrefManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsPrefManager instance at %s>" % (self.this,)
    def Setup(*args): return _blcelc.iAwsPrefManager_Setup(*args)
    def Load(*args): return _blcelc.iAwsPrefManager_Load(*args)
    def NameToId(*args): return _blcelc.iAwsPrefManager_NameToId(*args)
    def SelectDefaultSkin(*args): return _blcelc.iAwsPrefManager_SelectDefaultSkin(*args)
    def LookupIntKey(*args): return _blcelc.iAwsPrefManager_LookupIntKey(*args)
    def LookupStringKey(*args): return _blcelc.iAwsPrefManager_LookupStringKey(*args)
    def LookupRectKey(*args): return _blcelc.iAwsPrefManager_LookupRectKey(*args)
    def LookupRGBKey(*args): return _blcelc.iAwsPrefManager_LookupRGBKey(*args)
    def LookupPointKey(*args): return _blcelc.iAwsPrefManager_LookupPointKey(*args)
    def GetInt(*args): return _blcelc.iAwsPrefManager_GetInt(*args)
    def GetFloat(*args): return _blcelc.iAwsPrefManager_GetFloat(*args)
    def GetRect(*args): return _blcelc.iAwsPrefManager_GetRect(*args)
    def GetString(*args): return _blcelc.iAwsPrefManager_GetString(*args)
    def GetRGB(*args): return _blcelc.iAwsPrefManager_GetRGB(*args)
    def FindWindowDef(*args): return _blcelc.iAwsPrefManager_FindWindowDef(*args)
    def FindSkinDef(*args): return _blcelc.iAwsPrefManager_FindSkinDef(*args)
    def RemoveWindowDef(*args): return _blcelc.iAwsPrefManager_RemoveWindowDef(*args)
    def RemoveAllWindowDefs(*args): return _blcelc.iAwsPrefManager_RemoveAllWindowDefs(*args)
    def RemoveSkinDef(*args): return _blcelc.iAwsPrefManager_RemoveSkinDef(*args)
    def RemoveAllSkinDefs(*args): return _blcelc.iAwsPrefManager_RemoveAllSkinDefs(*args)
    def SetColor(*args): return _blcelc.iAwsPrefManager_SetColor(*args)
    def GetColor(*args): return _blcelc.iAwsPrefManager_GetColor(*args)
    def FindColor(*args): return _blcelc.iAwsPrefManager_FindColor(*args)
    def GetDefaultFont(*args): return _blcelc.iAwsPrefManager_GetDefaultFont(*args)
    def GetFont(*args): return _blcelc.iAwsPrefManager_GetFont(*args)
    def GetTexture(*args): return _blcelc.iAwsPrefManager_GetTexture(*args)
    def SetTextureManager(*args): return _blcelc.iAwsPrefManager_SetTextureManager(*args)
    def SetFontServer(*args): return _blcelc.iAwsPrefManager_SetFontServer(*args)
    def SetDefaultFont(*args): return _blcelc.iAwsPrefManager_SetDefaultFont(*args)
    def SetWindowMgr(*args): return _blcelc.iAwsPrefManager_SetWindowMgr(*args)
    def SetupPalette(*args): return _blcelc.iAwsPrefManager_SetupPalette(*args)
    def RegisterConstant(*args): return _blcelc.iAwsPrefManager_RegisterConstant(*args)
    def ConstantExists(*args): return _blcelc.iAwsPrefManager_ConstantExists(*args)
    def GetConstantValue(*args): return _blcelc.iAwsPrefManager_GetConstantValue(*args)
    def CreateKeyFactory(*args): return _blcelc.iAwsPrefManager_CreateKeyFactory(*args)
    def CreateConnectionNodeFactory(*args): return _blcelc.iAwsPrefManager_CreateConnectionNodeFactory(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsPrefManager):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsPrefManagerPtr(iAwsPrefManager):
    def __init__(self, this):
        _swig_setattr(self, iAwsPrefManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsPrefManager, 'thisown', 0)
        _swig_setattr(self, iAwsPrefManager,self.__class__,iAwsPrefManager)
_blcelc.iAwsPrefManager_swigregister(iAwsPrefManagerPtr)

class iAwsSinkManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsSinkManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsSinkManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsSinkManager instance at %s>" % (self.this,)
    def RegisterSink(*args): return _blcelc.iAwsSinkManager_RegisterSink(*args)
    def RemoveSink(*args): return _blcelc.iAwsSinkManager_RemoveSink(*args)
    def FindSink(*args): return _blcelc.iAwsSinkManager_FindSink(*args)
    def CreateSink(*args): return _blcelc.iAwsSinkManager_CreateSink(*args)
    def CreateSlot(*args): return _blcelc.iAwsSinkManager_CreateSlot(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsSinkManager):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsSinkManagerPtr(iAwsSinkManager):
    def __init__(self, this):
        _swig_setattr(self, iAwsSinkManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsSinkManager, 'thisown', 0)
        _swig_setattr(self, iAwsSinkManager,self.__class__,iAwsSinkManager)
_blcelc.iAwsSinkManager_swigregister(iAwsSinkManagerPtr)

class iAwsSink(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsSink, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsSink, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsSink instance at %s>" % (self.this,)
    def GetTriggerID(*args): return _blcelc.iAwsSink_GetTriggerID(*args)
    def HandleTrigger(*args): return _blcelc.iAwsSink_HandleTrigger(*args)
    def RegisterTrigger(*args): return _blcelc.iAwsSink_RegisterTrigger(*args)
    def GetError(*args): return _blcelc.iAwsSink_GetError(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsSink):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsSinkPtr(iAwsSink):
    def __init__(self, this):
        _swig_setattr(self, iAwsSink, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsSink, 'thisown', 0)
        _swig_setattr(self, iAwsSink,self.__class__,iAwsSink)
_blcelc.iAwsSink_swigregister(iAwsSinkPtr)

class iAwsSource(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsSource, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsSource, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsSource instance at %s>" % (self.this,)
    def GetComponent(*args): return _blcelc.iAwsSource_GetComponent(*args)
    def RegisterSlot(*args): return _blcelc.iAwsSource_RegisterSlot(*args)
    def UnregisterSlot(*args): return _blcelc.iAwsSource_UnregisterSlot(*args)
    def Broadcast(*args): return _blcelc.iAwsSource_Broadcast(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsSource):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsSourcePtr(iAwsSource):
    def __init__(self, this):
        _swig_setattr(self, iAwsSource, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsSource, 'thisown', 0)
        _swig_setattr(self, iAwsSource,self.__class__,iAwsSource)
_blcelc.iAwsSource_swigregister(iAwsSourcePtr)

class iAwsSlot(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsSlot, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsSlot, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsSlot instance at %s>" % (self.this,)
    def Connect(*args): return _blcelc.iAwsSlot_Connect(*args)
    def Disconnect(*args): return _blcelc.iAwsSlot_Disconnect(*args)
    def Emit(*args): return _blcelc.iAwsSlot_Emit(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsSlot):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsSlotPtr(iAwsSlot):
    def __init__(self, this):
        _swig_setattr(self, iAwsSlot, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsSlot, 'thisown', 0)
        _swig_setattr(self, iAwsSlot,self.__class__,iAwsSlot)
_blcelc.iAwsSlot_swigregister(iAwsSlotPtr)

class iAwsLayoutManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsLayoutManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsLayoutManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsLayoutManager instance at %s>" % (self.this,)
    def SetOwner(*args): return _blcelc.iAwsLayoutManager_SetOwner(*args)
    def AddComponent(*args): return _blcelc.iAwsLayoutManager_AddComponent(*args)
    def RemoveComponent(*args): return _blcelc.iAwsLayoutManager_RemoveComponent(*args)
    def LayoutComponents(*args): return _blcelc.iAwsLayoutManager_LayoutComponents(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsLayoutManager):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsLayoutManagerPtr(iAwsLayoutManager):
    def __init__(self, this):
        _swig_setattr(self, iAwsLayoutManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsLayoutManager, 'thisown', 0)
        _swig_setattr(self, iAwsLayoutManager,self.__class__,iAwsLayoutManager)
_blcelc.iAwsLayoutManager_swigregister(iAwsLayoutManagerPtr)

class iAwsComponent(iAwsSource):
    __swig_setmethods__ = {}
    for _s in [iAwsSource]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsComponent, name, value)
    __swig_getmethods__ = {}
    for _s in [iAwsSource]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsComponent, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsComponent instance at %s>" % (self.this,)
    def Create(*args): return _blcelc.iAwsComponent_Create(*args)
    def Setup(*args): return _blcelc.iAwsComponent_Setup(*args)
    def HandleEvent(*args): return _blcelc.iAwsComponent_HandleEvent(*args)
    def GetProperty(*args): return _blcelc.iAwsComponent_GetProperty(*args)
    def SetProperty(*args): return _blcelc.iAwsComponent_SetProperty(*args)
    def Execute(*args): return _blcelc.iAwsComponent_Execute(*args)
    def Invalidate(*args): return _blcelc.iAwsComponent_Invalidate(*args)
    def Frame(*args): return _blcelc.iAwsComponent_Frame(*args)
    def ClientFrame(*args): return _blcelc.iAwsComponent_ClientFrame(*args)
    def Type(*args): return _blcelc.iAwsComponent_Type(*args)
    def SetFlag(*args): return _blcelc.iAwsComponent_SetFlag(*args)
    def ClearFlag(*args): return _blcelc.iAwsComponent_ClearFlag(*args)
    def Flags(*args): return _blcelc.iAwsComponent_Flags(*args)
    def WindowManager(*args): return _blcelc.iAwsComponent_WindowManager(*args)
    def Parent(*args): return _blcelc.iAwsComponent_Parent(*args)
    def Window(*args): return _blcelc.iAwsComponent_Window(*args)
    def Layout(*args): return _blcelc.iAwsComponent_Layout(*args)
    def SetParent(*args): return _blcelc.iAwsComponent_SetParent(*args)
    def SetLayout(*args): return _blcelc.iAwsComponent_SetLayout(*args)
    def AddToLayout(*args): return _blcelc.iAwsComponent_AddToLayout(*args)
    def getPreferredSize(*args): return _blcelc.iAwsComponent_getPreferredSize(*args)
    def setPreferredSize(*args): return _blcelc.iAwsComponent_setPreferredSize(*args)
    def clearPreferredSize(*args): return _blcelc.iAwsComponent_clearPreferredSize(*args)
    def getMinimumSize(*args): return _blcelc.iAwsComponent_getMinimumSize(*args)
    def getInsets(*args): return _blcelc.iAwsComponent_getInsets(*args)
    def Overlaps(*args): return _blcelc.iAwsComponent_Overlaps(*args)
    def isHidden(*args): return _blcelc.iAwsComponent_isHidden(*args)
    def SetFocusable(*args): return _blcelc.iAwsComponent_SetFocusable(*args)
    def Focusable(*args): return _blcelc.iAwsComponent_Focusable(*args)
    def isFocused(*args): return _blcelc.iAwsComponent_isFocused(*args)
    def IsMaximized(*args): return _blcelc.iAwsComponent_IsMaximized(*args)
    def Hide(*args): return _blcelc.iAwsComponent_Hide(*args)
    def Show(*args): return _blcelc.iAwsComponent_Show(*args)
    def SetFocus(*args): return _blcelc.iAwsComponent_SetFocus(*args)
    def UnsetFocus(*args): return _blcelc.iAwsComponent_UnsetFocus(*args)
    def Move(*args): return _blcelc.iAwsComponent_Move(*args)
    def MoveTo(*args): return _blcelc.iAwsComponent_MoveTo(*args)
    def Resize(*args): return _blcelc.iAwsComponent_Resize(*args)
    def ResizeTo(*args): return _blcelc.iAwsComponent_ResizeTo(*args)
    def Maximize(*args): return _blcelc.iAwsComponent_Maximize(*args)
    def UnMaximize(*args): return _blcelc.iAwsComponent_UnMaximize(*args)
    def LayoutChildren(*args): return _blcelc.iAwsComponent_LayoutChildren(*args)
    def isDeaf(*args): return _blcelc.iAwsComponent_isDeaf(*args)
    def SetDeaf(*args): return _blcelc.iAwsComponent_SetDeaf(*args)
    def GetID(*args): return _blcelc.iAwsComponent_GetID(*args)
    def SetID(*args): return _blcelc.iAwsComponent_SetID(*args)
    def FindChild(*args): return _blcelc.iAwsComponent_FindChild(*args)
    def DoFindChild(*args): return _blcelc.iAwsComponent_DoFindChild(*args)
    def ChildAt(*args): return _blcelc.iAwsComponent_ChildAt(*args)
    def AddChild(*args): return _blcelc.iAwsComponent_AddChild(*args)
    def RemoveChild(*args): return _blcelc.iAwsComponent_RemoveChild(*args)
    def GetChildCount(*args): return _blcelc.iAwsComponent_GetChildCount(*args)
    def GetTopChild(*args): return _blcelc.iAwsComponent_GetTopChild(*args)
    def ComponentAbove(*args): return _blcelc.iAwsComponent_ComponentAbove(*args)
    def ComponentBelow(*args): return _blcelc.iAwsComponent_ComponentBelow(*args)
    def SetComponentAbove(*args): return _blcelc.iAwsComponent_SetComponentAbove(*args)
    def SetComponentBelow(*args): return _blcelc.iAwsComponent_SetComponentBelow(*args)
    def AddToTabOrder(*args): return _blcelc.iAwsComponent_AddToTabOrder(*args)
    def TabNext(*args): return _blcelc.iAwsComponent_TabNext(*args)
    def TabPrev(*args): return _blcelc.iAwsComponent_TabPrev(*args)
    def GetTabLength(*args): return _blcelc.iAwsComponent_GetTabLength(*args)
    def GetTabComponent(*args): return _blcelc.iAwsComponent_GetTabComponent(*args)
    def GetFirstFocusableChild(*args): return _blcelc.iAwsComponent_GetFirstFocusableChild(*args)
    def Raise(*args): return _blcelc.iAwsComponent_Raise(*args)
    def Lower(*args): return _blcelc.iAwsComponent_Lower(*args)
    def HasChildren(*args): return _blcelc.iAwsComponent_HasChildren(*args)
    def SetRedrawTag(*args): return _blcelc.iAwsComponent_SetRedrawTag(*args)
    def RedrawTag(*args): return _blcelc.iAwsComponent_RedrawTag(*args)
    def OnDraw(*args): return _blcelc.iAwsComponent_OnDraw(*args)
    def OnMouseDown(*args): return _blcelc.iAwsComponent_OnMouseDown(*args)
    def OnMouseUp(*args): return _blcelc.iAwsComponent_OnMouseUp(*args)
    def OnMouseMove(*args): return _blcelc.iAwsComponent_OnMouseMove(*args)
    def OnMouseClick(*args): return _blcelc.iAwsComponent_OnMouseClick(*args)
    def OnMouseDoubleClick(*args): return _blcelc.iAwsComponent_OnMouseDoubleClick(*args)
    def OnMouseExit(*args): return _blcelc.iAwsComponent_OnMouseExit(*args)
    def OnMouseEnter(*args): return _blcelc.iAwsComponent_OnMouseEnter(*args)
    def OnKeyboard(*args): return _blcelc.iAwsComponent_OnKeyboard(*args)
    def OnLostFocus(*args): return _blcelc.iAwsComponent_OnLostFocus(*args)
    def OnGainFocus(*args): return _blcelc.iAwsComponent_OnGainFocus(*args)
    def OnFrame(*args): return _blcelc.iAwsComponent_OnFrame(*args)
    def OnAdded(*args): return _blcelc.iAwsComponent_OnAdded(*args)
    def OnResized(*args): return _blcelc.iAwsComponent_OnResized(*args)
    def OnChildMoved(*args): return _blcelc.iAwsComponent_OnChildMoved(*args)
    def OnRaise(*args): return _blcelc.iAwsComponent_OnRaise(*args)
    def OnLower(*args): return _blcelc.iAwsComponent_OnLower(*args)
    def OnChildHide(*args): return _blcelc.iAwsComponent_OnChildHide(*args)
    def OnChildShow(*args): return _blcelc.iAwsComponent_OnChildShow(*args)
    def OnSetFocus(*args): return _blcelc.iAwsComponent_OnSetFocus(*args)
    def OnUnsetFocus(*args): return _blcelc.iAwsComponent_OnUnsetFocus(*args)
    def Unlink(*args): return _blcelc.iAwsComponent_Unlink(*args)
    def LinkAbove(*args): return _blcelc.iAwsComponent_LinkAbove(*args)
    def LinkBelow(*args): return _blcelc.iAwsComponent_LinkBelow(*args)
    def SetTopChild(*args): return _blcelc.iAwsComponent_SetTopChild(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsComponent):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsComponentPtr(iAwsComponent):
    def __init__(self, this):
        _swig_setattr(self, iAwsComponent, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsComponent, 'thisown', 0)
        _swig_setattr(self, iAwsComponent,self.__class__,iAwsComponent)
_blcelc.iAwsComponent_swigregister(iAwsComponentPtr)

class iAwsComponentFactory(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsComponentFactory, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsComponentFactory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsComponentFactory instance at %s>" % (self.this,)
    def Create(*args): return _blcelc.iAwsComponentFactory_Create(*args)
    def Register(*args): return _blcelc.iAwsComponentFactory_Register(*args)
    def RegisterConstant(*args): return _blcelc.iAwsComponentFactory_RegisterConstant(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsComponentFactory):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsComponentFactoryPtr(iAwsComponentFactory):
    def __init__(self, this):
        _swig_setattr(self, iAwsComponentFactory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsComponentFactory, 'thisown', 0)
        _swig_setattr(self, iAwsComponentFactory,self.__class__,iAwsComponentFactory)
_blcelc.iAwsComponentFactory_swigregister(iAwsComponentFactoryPtr)

class iAwsKeyFactory(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsKeyFactory, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsKeyFactory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsKeyFactory instance at %s>" % (self.this,)
    def Initialize(*args): return _blcelc.iAwsKeyFactory_Initialize(*args)
    def AddToWindowList(*args): return _blcelc.iAwsKeyFactory_AddToWindowList(*args)
    def AddFactory(*args): return _blcelc.iAwsKeyFactory_AddFactory(*args)
    def AddIntKey(*args): return _blcelc.iAwsKeyFactory_AddIntKey(*args)
    def AddStringKey(*args): return _blcelc.iAwsKeyFactory_AddStringKey(*args)
    def AddRectKey(*args): return _blcelc.iAwsKeyFactory_AddRectKey(*args)
    def AddRGBKey(*args): return _blcelc.iAwsKeyFactory_AddRGBKey(*args)
    def AddPointKey(*args): return _blcelc.iAwsKeyFactory_AddPointKey(*args)
    def AddConnectionKey(*args): return _blcelc.iAwsKeyFactory_AddConnectionKey(*args)
    def AddConnectionNode(*args): return _blcelc.iAwsKeyFactory_AddConnectionNode(*args)
    def GetThisNode(*args): return _blcelc.iAwsKeyFactory_GetThisNode(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsKeyFactory):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsKeyFactoryPtr(iAwsKeyFactory):
    def __init__(self, this):
        _swig_setattr(self, iAwsKeyFactory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsKeyFactory, 'thisown', 0)
        _swig_setattr(self, iAwsKeyFactory,self.__class__,iAwsKeyFactory)
_blcelc.iAwsKeyFactory_swigregister(iAwsKeyFactoryPtr)

class iAwsConnectionNodeFactory(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsConnectionNodeFactory, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsConnectionNodeFactory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsConnectionNodeFactory instance at %s>" % (self.this,)
    def Initialize(*args): return _blcelc.iAwsConnectionNodeFactory_Initialize(*args)
    def AddConnectionKey(*args): return _blcelc.iAwsConnectionNodeFactory_AddConnectionKey(*args)
    def GetThisNode(*args): return _blcelc.iAwsConnectionNodeFactory_GetThisNode(*args)
    def __del__(self, destroy=_blcelc.delete_iAwsConnectionNodeFactory):
        try:
            if self.thisown: destroy(self)
        except: pass

class iAwsConnectionNodeFactoryPtr(iAwsConnectionNodeFactory):
    def __init__(self, this):
        _swig_setattr(self, iAwsConnectionNodeFactory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsConnectionNodeFactory, 'thisown', 0)
        _swig_setattr(self, iAwsConnectionNodeFactory,self.__class__,iAwsConnectionNodeFactory)
_blcelc.iAwsConnectionNodeFactory_swigregister(iAwsConnectionNodeFactoryPtr)

MAX_OUTPUT_VERTICES = _blcelc.MAX_OUTPUT_VERTICES
CS_CLIP_OUTSIDE = _blcelc.CS_CLIP_OUTSIDE
CS_CLIP_INSIDE = _blcelc.CS_CLIP_INSIDE
CS_CLIP_CLIPPED = _blcelc.CS_CLIP_CLIPPED
class csVertexStatus(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csVertexStatus, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csVertexStatus, name)
    def __repr__(self):
        return "<C csVertexStatus instance at %s>" % (self.this,)
    __swig_setmethods__["Type"] = _blcelc.csVertexStatus_Type_set
    __swig_getmethods__["Type"] = _blcelc.csVertexStatus_Type_get
    if _newclass:Type = property(_blcelc.csVertexStatus_Type_get, _blcelc.csVertexStatus_Type_set)
    __swig_setmethods__["Vertex"] = _blcelc.csVertexStatus_Vertex_set
    __swig_getmethods__["Vertex"] = _blcelc.csVertexStatus_Vertex_get
    if _newclass:Vertex = property(_blcelc.csVertexStatus_Vertex_get, _blcelc.csVertexStatus_Vertex_set)
    __swig_setmethods__["Pos"] = _blcelc.csVertexStatus_Pos_set
    __swig_getmethods__["Pos"] = _blcelc.csVertexStatus_Pos_get
    if _newclass:Pos = property(_blcelc.csVertexStatus_Pos_get, _blcelc.csVertexStatus_Pos_set)
    def __init__(self, *args):
        _swig_setattr(self, csVertexStatus, 'this', _blcelc.new_csVertexStatus(*args))
        _swig_setattr(self, csVertexStatus, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csVertexStatus):
        try:
            if self.thisown: destroy(self)
        except: pass

class csVertexStatusPtr(csVertexStatus):
    def __init__(self, this):
        _swig_setattr(self, csVertexStatus, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csVertexStatus, 'thisown', 0)
        _swig_setattr(self, csVertexStatus,self.__class__,csVertexStatus)
_blcelc.csVertexStatus_swigregister(csVertexStatusPtr)

CS_VERTEX_ORIGINAL = _blcelc.CS_VERTEX_ORIGINAL
CS_VERTEX_ONEDGE = _blcelc.CS_VERTEX_ONEDGE
CS_VERTEX_INSIDE = _blcelc.CS_VERTEX_INSIDE
class iClipper2D(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iClipper2D, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iClipper2D, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iClipper2D instance at %s>" % (self.this,)
    def Clip(*args): return _blcelc.iClipper2D_Clip(*args)
    def ClipInPlace(*args): return _blcelc.iClipper2D_ClipInPlace(*args)
    def ClassifyBox(*args): return _blcelc.iClipper2D_ClassifyBox(*args)
    def IsInside(*args): return _blcelc.iClipper2D_IsInside(*args)
    def GetVertexCount(*args): return _blcelc.iClipper2D_GetVertexCount(*args)
    def GetClipPoly(*args): return _blcelc.iClipper2D_GetClipPoly(*args)
    clipperPoly = _blcelc.iClipper2D_clipperPoly
    clipperBox = _blcelc.iClipper2D_clipperBox
    def GetClipperType(*args): return _blcelc.iClipper2D_GetClipperType(*args)
    def __del__(self, destroy=_blcelc.delete_iClipper2D):
        try:
            if self.thisown: destroy(self)
        except: pass

class iClipper2DPtr(iClipper2D):
    def __init__(self, this):
        _swig_setattr(self, iClipper2D, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iClipper2D, 'thisown', 0)
        _swig_setattr(self, iClipper2D,self.__class__,iClipper2D)
_blcelc.iClipper2D_swigregister(iClipper2DPtr)

class iObjectModelListener(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iObjectModelListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iObjectModelListener, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectModelListener instance at %s>" % (self.this,)
    def ObjectModelChanged(*args): return _blcelc.iObjectModelListener_ObjectModelChanged(*args)
    def __del__(self, destroy=_blcelc.delete_iObjectModelListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iObjectModelListener_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iObjectModelListener_scfGetVersion)

class iObjectModelListenerPtr(iObjectModelListener):
    def __init__(self, this):
        _swig_setattr(self, iObjectModelListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iObjectModelListener, 'thisown', 0)
        _swig_setattr(self, iObjectModelListener,self.__class__,iObjectModelListener)
_blcelc.iObjectModelListener_swigregister(iObjectModelListenerPtr)

iObjectModelListener_scfGetVersion = _blcelc.iObjectModelListener_scfGetVersion

class iObjectModel(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iObjectModel, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iObjectModel, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectModel instance at %s>" % (self.this,)
    def GetShapeNumber(*args): return _blcelc.iObjectModel_GetShapeNumber(*args)
    def GetPolygonMeshBase(*args): return _blcelc.iObjectModel_GetPolygonMeshBase(*args)
    def GetPolygonMeshColldet(*args): return _blcelc.iObjectModel_GetPolygonMeshColldet(*args)
    def SetPolygonMeshColldet(*args): return _blcelc.iObjectModel_SetPolygonMeshColldet(*args)
    def GetPolygonMeshViscull(*args): return _blcelc.iObjectModel_GetPolygonMeshViscull(*args)
    def SetPolygonMeshViscull(*args): return _blcelc.iObjectModel_SetPolygonMeshViscull(*args)
    def GetPolygonMeshShadows(*args): return _blcelc.iObjectModel_GetPolygonMeshShadows(*args)
    def SetPolygonMeshShadows(*args): return _blcelc.iObjectModel_SetPolygonMeshShadows(*args)
    def CreateLowerDetailPolygonMesh(*args): return _blcelc.iObjectModel_CreateLowerDetailPolygonMesh(*args)
    def GetObjectBoundingBox(*args): return _blcelc.iObjectModel_GetObjectBoundingBox(*args)
    def SetObjectBoundingBox(*args): return _blcelc.iObjectModel_SetObjectBoundingBox(*args)
    def GetRadius(*args): return _blcelc.iObjectModel_GetRadius(*args)
    def AddListener(*args): return _blcelc.iObjectModel_AddListener(*args)
    def RemoveListener(*args): return _blcelc.iObjectModel_RemoveListener(*args)
    def __del__(self, destroy=_blcelc.delete_iObjectModel):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iObjectModel_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iObjectModel_scfGetVersion)

class iObjectModelPtr(iObjectModel):
    def __init__(self, this):
        _swig_setattr(self, iObjectModel, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iObjectModel, 'thisown', 0)
        _swig_setattr(self, iObjectModel,self.__class__,iObjectModel)
_blcelc.iObjectModel_swigregister(iObjectModelPtr)

iObjectModel_scfGetVersion = _blcelc.iObjectModel_scfGetVersion

CS_POLYMESH_CLOSED = _blcelc.CS_POLYMESH_CLOSED
CS_POLYMESH_NOTCLOSED = _blcelc.CS_POLYMESH_NOTCLOSED
CS_POLYMESH_CONVEX = _blcelc.CS_POLYMESH_CONVEX
CS_POLYMESH_NOTCONVEX = _blcelc.CS_POLYMESH_NOTCONVEX
CS_POLYMESH_DEFORMABLE = _blcelc.CS_POLYMESH_DEFORMABLE
CS_POLYMESH_TRIANGLEMESH = _blcelc.CS_POLYMESH_TRIANGLEMESH
class csMeshedPolygon(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csMeshedPolygon, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csMeshedPolygon, name)
    def __repr__(self):
        return "<C csMeshedPolygon instance at %s>" % (self.this,)
    __swig_setmethods__["num_vertices"] = _blcelc.csMeshedPolygon_num_vertices_set
    __swig_getmethods__["num_vertices"] = _blcelc.csMeshedPolygon_num_vertices_get
    if _newclass:num_vertices = property(_blcelc.csMeshedPolygon_num_vertices_get, _blcelc.csMeshedPolygon_num_vertices_set)
    __swig_setmethods__["vertices"] = _blcelc.csMeshedPolygon_vertices_set
    __swig_getmethods__["vertices"] = _blcelc.csMeshedPolygon_vertices_get
    if _newclass:vertices = property(_blcelc.csMeshedPolygon_vertices_get, _blcelc.csMeshedPolygon_vertices_set)
    def __init__(self, *args):
        _swig_setattr(self, csMeshedPolygon, 'this', _blcelc.new_csMeshedPolygon(*args))
        _swig_setattr(self, csMeshedPolygon, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csMeshedPolygon):
        try:
            if self.thisown: destroy(self)
        except: pass

class csMeshedPolygonPtr(csMeshedPolygon):
    def __init__(self, this):
        _swig_setattr(self, csMeshedPolygon, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csMeshedPolygon, 'thisown', 0)
        _swig_setattr(self, csMeshedPolygon,self.__class__,csMeshedPolygon)
_blcelc.csMeshedPolygon_swigregister(csMeshedPolygonPtr)

class iPolygonMesh(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPolygonMesh, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPolygonMesh, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPolygonMesh instance at %s>" % (self.this,)
    def GetVertexCount(*args): return _blcelc.iPolygonMesh_GetVertexCount(*args)
    def GetVertices(*args): return _blcelc.iPolygonMesh_GetVertices(*args)
    def GetPolygonCount(*args): return _blcelc.iPolygonMesh_GetPolygonCount(*args)
    def GetPolygons(*args): return _blcelc.iPolygonMesh_GetPolygons(*args)
    def GetTriangleCount(*args): return _blcelc.iPolygonMesh_GetTriangleCount(*args)
    def GetTriangles(*args): return _blcelc.iPolygonMesh_GetTriangles(*args)
    def Lock(*args): return _blcelc.iPolygonMesh_Lock(*args)
    def Unlock(*args): return _blcelc.iPolygonMesh_Unlock(*args)
    def GetFlags(*args): return _blcelc.iPolygonMesh_GetFlags(*args)
    def GetChangeNumber(*args): return _blcelc.iPolygonMesh_GetChangeNumber(*args)
    def __del__(self, destroy=_blcelc.delete_iPolygonMesh):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iPolygonMesh_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iPolygonMesh_scfGetVersion)

class iPolygonMeshPtr(iPolygonMesh):
    def __init__(self, this):
        _swig_setattr(self, iPolygonMesh, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPolygonMesh, 'thisown', 0)
        _swig_setattr(self, iPolygonMesh,self.__class__,iPolygonMesh)
_blcelc.iPolygonMesh_swigregister(iPolygonMeshPtr)

iPolygonMesh_scfGetVersion = _blcelc.iPolygonMesh_scfGetVersion

class csPolygonMesh(iPolygonMesh):
    __swig_setmethods__ = {}
    for _s in [iPolygonMesh]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPolygonMesh, name, value)
    __swig_getmethods__ = {}
    for _s in [iPolygonMesh]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csPolygonMesh, name)
    def __repr__(self):
        return "<C csPolygonMesh instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csPolygonMesh, 'this', _blcelc.new_csPolygonMesh(*args))
        _swig_setattr(self, csPolygonMesh, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csPolygonMesh):
        try:
            if self.thisown: destroy(self)
        except: pass
    def SetVertices(*args): return _blcelc.csPolygonMesh_SetVertices(*args)
    def SetPolygons(*args): return _blcelc.csPolygonMesh_SetPolygons(*args)
    def SetPolygonIndices(*args): return _blcelc.csPolygonMesh_SetPolygonIndices(*args)
    def SetPolygonIndexCount(*args): return _blcelc.csPolygonMesh_SetPolygonIndexCount(*args)
    def GetPolygonIndices(*args): return _blcelc.csPolygonMesh_GetPolygonIndices(*args)
    def SetVertexCount(*args): return _blcelc.csPolygonMesh_SetVertexCount(*args)
    def SetPolygonCount(*args): return _blcelc.csPolygonMesh_SetPolygonCount(*args)
    def ShapeChanged(*args): return _blcelc.csPolygonMesh_ShapeChanged(*args)
    __swig_setmethods__["scfRefCount"] = _blcelc.csPolygonMesh_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csPolygonMesh_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csPolygonMesh_scfRefCount_get, _blcelc.csPolygonMesh_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csPolygonMesh_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csPolygonMesh_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csPolygonMesh_scfWeakRefOwners_get, _blcelc.csPolygonMesh_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc.csPolygonMesh_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc.csPolygonMesh_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csPolygonMesh_scfParent_get
    if _newclass:scfParent = property(_blcelc.csPolygonMesh_scfParent_get, _blcelc.csPolygonMesh_scfParent_set)
    def IncRef(*args): return _blcelc.csPolygonMesh_IncRef(*args)
    def DecRef(*args): return _blcelc.csPolygonMesh_DecRef(*args)
    def GetRefCount(*args): return _blcelc.csPolygonMesh_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.csPolygonMesh_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.csPolygonMesh_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.csPolygonMesh_QueryInterface(*args)
    def GetVertexCount(*args): return _blcelc.csPolygonMesh_GetVertexCount(*args)
    def GetVertices(*args): return _blcelc.csPolygonMesh_GetVertices(*args)
    def GetPolygonCount(*args): return _blcelc.csPolygonMesh_GetPolygonCount(*args)
    def GetPolygons(*args): return _blcelc.csPolygonMesh_GetPolygons(*args)
    def GetTriangleCount(*args): return _blcelc.csPolygonMesh_GetTriangleCount(*args)
    def GetTriangles(*args): return _blcelc.csPolygonMesh_GetTriangles(*args)
    def Lock(*args): return _blcelc.csPolygonMesh_Lock(*args)
    def Unlock(*args): return _blcelc.csPolygonMesh_Unlock(*args)
    def GetFlags(*args): return _blcelc.csPolygonMesh_GetFlags(*args)
    def GetChangeNumber(*args): return _blcelc.csPolygonMesh_GetChangeNumber(*args)

class csPolygonMeshPtr(csPolygonMesh):
    def __init__(self, this):
        _swig_setattr(self, csPolygonMesh, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPolygonMesh, 'thisown', 0)
        _swig_setattr(self, csPolygonMesh,self.__class__,csPolygonMesh)
_blcelc.csPolygonMesh_swigregister(csPolygonMeshPtr)

class csPolygonMeshBox(iPolygonMesh):
    __swig_setmethods__ = {}
    for _s in [iPolygonMesh]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPolygonMeshBox, name, value)
    __swig_getmethods__ = {}
    for _s in [iPolygonMesh]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csPolygonMeshBox, name)
    def __repr__(self):
        return "<C csPolygonMeshBox instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csPolygonMeshBox, 'this', _blcelc.new_csPolygonMeshBox(*args))
        _swig_setattr(self, csPolygonMeshBox, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csPolygonMeshBox):
        try:
            if self.thisown: destroy(self)
        except: pass
    def SetBox(*args): return _blcelc.csPolygonMeshBox_SetBox(*args)
    __swig_setmethods__["scfRefCount"] = _blcelc.csPolygonMeshBox_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csPolygonMeshBox_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csPolygonMeshBox_scfRefCount_get, _blcelc.csPolygonMeshBox_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csPolygonMeshBox_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csPolygonMeshBox_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csPolygonMeshBox_scfWeakRefOwners_get, _blcelc.csPolygonMeshBox_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc.csPolygonMeshBox_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc.csPolygonMeshBox_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csPolygonMeshBox_scfParent_get
    if _newclass:scfParent = property(_blcelc.csPolygonMeshBox_scfParent_get, _blcelc.csPolygonMeshBox_scfParent_set)
    def IncRef(*args): return _blcelc.csPolygonMeshBox_IncRef(*args)
    def DecRef(*args): return _blcelc.csPolygonMeshBox_DecRef(*args)
    def GetRefCount(*args): return _blcelc.csPolygonMeshBox_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.csPolygonMeshBox_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.csPolygonMeshBox_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.csPolygonMeshBox_QueryInterface(*args)
    def GetVertexCount(*args): return _blcelc.csPolygonMeshBox_GetVertexCount(*args)
    def GetVertices(*args): return _blcelc.csPolygonMeshBox_GetVertices(*args)
    def GetPolygonCount(*args): return _blcelc.csPolygonMeshBox_GetPolygonCount(*args)
    def GetPolygons(*args): return _blcelc.csPolygonMeshBox_GetPolygons(*args)
    def GetTriangleCount(*args): return _blcelc.csPolygonMeshBox_GetTriangleCount(*args)
    def GetTriangles(*args): return _blcelc.csPolygonMeshBox_GetTriangles(*args)
    def Lock(*args): return _blcelc.csPolygonMeshBox_Lock(*args)
    def Unlock(*args): return _blcelc.csPolygonMeshBox_Unlock(*args)
    def GetFlags(*args): return _blcelc.csPolygonMeshBox_GetFlags(*args)
    def GetChangeNumber(*args): return _blcelc.csPolygonMeshBox_GetChangeNumber(*args)

class csPolygonMeshBoxPtr(csPolygonMeshBox):
    def __init__(self, this):
        _swig_setattr(self, csPolygonMeshBox, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPolygonMeshBox, 'thisown', 0)
        _swig_setattr(self, csPolygonMeshBox,self.__class__,csPolygonMeshBox)
_blcelc.csPolygonMeshBox_swigregister(csPolygonMeshBoxPtr)

class iFrustumViewUserdata(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iFrustumViewUserdata, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iFrustumViewUserdata, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFrustumViewUserdata instance at %s>" % (self.this,)
    def __del__(self, destroy=_blcelc.delete_iFrustumViewUserdata):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFrustumViewUserdata_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFrustumViewUserdata_scfGetVersion)

class iFrustumViewUserdataPtr(iFrustumViewUserdata):
    def __init__(self, this):
        _swig_setattr(self, iFrustumViewUserdata, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iFrustumViewUserdata, 'thisown', 0)
        _swig_setattr(self, iFrustumViewUserdata,self.__class__,iFrustumViewUserdata)
_blcelc.iFrustumViewUserdata_swigregister(iFrustumViewUserdataPtr)

iFrustumViewUserdata_scfGetVersion = _blcelc.iFrustumViewUserdata_scfGetVersion

class csFrustumContext(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csFrustumContext, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csFrustumContext, name)
    def __repr__(self):
        return "<C csFrustumContext instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csFrustumContext, 'this', _blcelc.new_csFrustumContext(*args))
        _swig_setattr(self, csFrustumContext, 'thisown', 1)
    def assign(*args): return _blcelc.csFrustumContext_assign(*args)
    def GetShadows(*args): return _blcelc.csFrustumContext_GetShadows(*args)
    def SetShadows(*args): return _blcelc.csFrustumContext_SetShadows(*args)
    def SetNewShadows(*args): return _blcelc.csFrustumContext_SetNewShadows(*args)
    def IsShared(*args): return _blcelc.csFrustumContext_IsShared(*args)
    def SetLightFrustum(*args): return _blcelc.csFrustumContext_SetLightFrustum(*args)
    def SetNewLightFrustum(*args): return _blcelc.csFrustumContext_SetNewLightFrustum(*args)
    def GetLightFrustum(*args): return _blcelc.csFrustumContext_GetLightFrustum(*args)
    def SetMirrored(*args): return _blcelc.csFrustumContext_SetMirrored(*args)
    def IsMirrored(*args): return _blcelc.csFrustumContext_IsMirrored(*args)
    def __del__(self, destroy=_blcelc.delete_csFrustumContext):
        try:
            if self.thisown: destroy(self)
        except: pass

class csFrustumContextPtr(csFrustumContext):
    def __init__(self, this):
        _swig_setattr(self, csFrustumContext, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csFrustumContext, 'thisown', 0)
        _swig_setattr(self, csFrustumContext,self.__class__,csFrustumContext)
_blcelc.csFrustumContext_swigregister(csFrustumContextPtr)

class iFrustumView(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iFrustumView, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iFrustumView, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFrustumView instance at %s>" % (self.this,)
    def GetFrustumContext(*args): return _blcelc.iFrustumView_GetFrustumContext(*args)
    def CreateFrustumContext(*args): return _blcelc.iFrustumView_CreateFrustumContext(*args)
    def CopyFrustumContext(*args): return _blcelc.iFrustumView_CopyFrustumContext(*args)
    def SetFrustumContext(*args): return _blcelc.iFrustumView_SetFrustumContext(*args)
    def RestoreFrustumContext(*args): return _blcelc.iFrustumView_RestoreFrustumContext(*args)
    def SetObjectFunction(*args): return _blcelc.iFrustumView_SetObjectFunction(*args)
    def CallObjectFunction(*args): return _blcelc.iFrustumView_CallObjectFunction(*args)
    def GetRadius(*args): return _blcelc.iFrustumView_GetRadius(*args)
    def GetSquaredRadius(*args): return _blcelc.iFrustumView_GetSquaredRadius(*args)
    def ThingShadowsEnabled(*args): return _blcelc.iFrustumView_ThingShadowsEnabled(*args)
    def CheckShadowMask(*args): return _blcelc.iFrustumView_CheckShadowMask(*args)
    def CheckProcessMask(*args): return _blcelc.iFrustumView_CheckProcessMask(*args)
    def StartNewShadowBlock(*args): return _blcelc.iFrustumView_StartNewShadowBlock(*args)
    def SetUserdata(*args): return _blcelc.iFrustumView_SetUserdata(*args)
    def GetUserdata(*args): return _blcelc.iFrustumView_GetUserdata(*args)
    def CreateShadowBlock(*args): return _blcelc.iFrustumView_CreateShadowBlock(*args)
    def __del__(self, destroy=_blcelc.delete_iFrustumView):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFrustumView_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFrustumView_scfGetVersion)

class iFrustumViewPtr(iFrustumView):
    def __init__(self, this):
        _swig_setattr(self, iFrustumView, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iFrustumView, 'thisown', 0)
        _swig_setattr(self, iFrustumView,self.__class__,iFrustumView)
_blcelc.iFrustumView_swigregister(iFrustumViewPtr)

iFrustumView_scfGetVersion = _blcelc.iFrustumView_scfGetVersion

CS_LIGHT_THINGSHADOWS = _blcelc.CS_LIGHT_THINGSHADOWS
CS_LIGHT_ACTIVEHALO = _blcelc.CS_LIGHT_ACTIVEHALO
CS_LIGHT_DYNAMICTYPE_STATIC = _blcelc.CS_LIGHT_DYNAMICTYPE_STATIC
CS_LIGHT_DYNAMICTYPE_PSEUDO = _blcelc.CS_LIGHT_DYNAMICTYPE_PSEUDO
CS_LIGHT_DYNAMICTYPE_DYNAMIC = _blcelc.CS_LIGHT_DYNAMICTYPE_DYNAMIC
CS_DEFAULT_LIGHT_LEVEL = _blcelc.CS_DEFAULT_LIGHT_LEVEL
CS_NORMAL_LIGHT_LEVEL = _blcelc.CS_NORMAL_LIGHT_LEVEL
CS_ATTN_NONE = _blcelc.CS_ATTN_NONE
CS_ATTN_LINEAR = _blcelc.CS_ATTN_LINEAR
CS_ATTN_INVERSE = _blcelc.CS_ATTN_INVERSE
CS_ATTN_REALISTIC = _blcelc.CS_ATTN_REALISTIC
CS_ATTN_CLQ = _blcelc.CS_ATTN_CLQ
class iLightCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLightCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLightCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightCallback instance at %s>" % (self.this,)
    def OnColorChange(*args): return _blcelc.iLightCallback_OnColorChange(*args)
    def OnPositionChange(*args): return _blcelc.iLightCallback_OnPositionChange(*args)
    def OnSectorChange(*args): return _blcelc.iLightCallback_OnSectorChange(*args)
    def OnRadiusChange(*args): return _blcelc.iLightCallback_OnRadiusChange(*args)
    def OnDestroy(*args): return _blcelc.iLightCallback_OnDestroy(*args)
    def OnAttenuationChange(*args): return _blcelc.iLightCallback_OnAttenuationChange(*args)
    def __del__(self, destroy=_blcelc.delete_iLightCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iLightCallbackPtr(iLightCallback):
    def __init__(self, this):
        _swig_setattr(self, iLightCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLightCallback, 'thisown', 0)
        _swig_setattr(self, iLightCallback,self.__class__,iLightCallback)
_blcelc.iLightCallback_swigregister(iLightCallbackPtr)

class iLight(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLight, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLight, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLight instance at %s>" % (self.this,)
    def GetLightID(*args): return _blcelc.iLight_GetLightID(*args)
    def QueryObject(*args): return _blcelc.iLight_QueryObject(*args)
    def GetDynamicType(*args): return _blcelc.iLight_GetDynamicType(*args)
    def GetCenter(*args): return _blcelc.iLight_GetCenter(*args)
    def SetCenter(*args): return _blcelc.iLight_SetCenter(*args)
    def GetSector(*args): return _blcelc.iLight_GetSector(*args)
    def GetColor(*args): return _blcelc.iLight_GetColor(*args)
    def SetColor(*args): return _blcelc.iLight_SetColor(*args)
    def GetInfluenceRadius(*args): return _blcelc.iLight_GetInfluenceRadius(*args)
    def GetInfluenceRadiusSq(*args): return _blcelc.iLight_GetInfluenceRadiusSq(*args)
    def SetInfluenceRadius(*args): return _blcelc.iLight_SetInfluenceRadius(*args)
    def GetAttenuation(*args): return _blcelc.iLight_GetAttenuation(*args)
    def SetAttenuation(*args): return _blcelc.iLight_SetAttenuation(*args)
    def SetAttenuationVector(*args): return _blcelc.iLight_SetAttenuationVector(*args)
    def GetAttenuationVector(*args): return _blcelc.iLight_GetAttenuationVector(*args)
    def CalculateAttenuationVector(*args): return _blcelc.iLight_CalculateAttenuationVector(*args)
    def GetDistanceForBrightness(*args): return _blcelc.iLight_GetDistanceForBrightness(*args)
    def CreateCrossHalo(*args): return _blcelc.iLight_CreateCrossHalo(*args)
    def CreateNovaHalo(*args): return _blcelc.iLight_CreateNovaHalo(*args)
    def CreateFlareHalo(*args): return _blcelc.iLight_CreateFlareHalo(*args)
    def GetHalo(*args): return _blcelc.iLight_GetHalo(*args)
    def GetBrightnessAtDistance(*args): return _blcelc.iLight_GetBrightnessAtDistance(*args)
    def GetFlags(*args): return _blcelc.iLight_GetFlags(*args)
    def SetLightCallback(*args): return _blcelc.iLight_SetLightCallback(*args)
    def RemoveLightCallback(*args): return _blcelc.iLight_RemoveLightCallback(*args)
    def GetLightCallbackCount(*args): return _blcelc.iLight_GetLightCallbackCount(*args)
    def GetLightCallback(*args): return _blcelc.iLight_GetLightCallback(*args)
    def GetLightNumber(*args): return _blcelc.iLight_GetLightNumber(*args)
    def AddAffectedLightingInfo(*args): return _blcelc.iLight_AddAffectedLightingInfo(*args)
    def RemoveAffectedLightingInfo(*args): return _blcelc.iLight_RemoveAffectedLightingInfo(*args)
    def Setup(*args): return _blcelc.iLight_Setup(*args)
    def __del__(self, destroy=_blcelc.delete_iLight):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iLight_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iLight_scfGetVersion)

class iLightPtr(iLight):
    def __init__(self, this):
        _swig_setattr(self, iLight, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLight, 'thisown', 0)
        _swig_setattr(self, iLight,self.__class__,iLight)
_blcelc.iLight_swigregister(iLightPtr)

iLight_scfGetVersion = _blcelc.iLight_scfGetVersion

class iLightList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLightList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLightList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightList instance at %s>" % (self.this,)
    def GetCount(*args): return _blcelc.iLightList_GetCount(*args)
    def Get(*args): return _blcelc.iLightList_Get(*args)
    def Add(*args): return _blcelc.iLightList_Add(*args)
    def Remove(*args): return _blcelc.iLightList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iLightList_RemoveAll(*args)
    def Find(*args): return _blcelc.iLightList_Find(*args)
    def FindByName(*args): return _blcelc.iLightList_FindByName(*args)
    def FindByID(*args): return _blcelc.iLightList_FindByID(*args)
    def __del__(self, destroy=_blcelc.delete_iLightList):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iLightList_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iLightList_scfGetVersion)

class iLightListPtr(iLightList):
    def __init__(self, this):
        _swig_setattr(self, iLightList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLightList, 'thisown', 0)
        _swig_setattr(self, iLightList,self.__class__,iLightList)
_blcelc.iLightList_swigregister(iLightListPtr)

iLightList_scfGetVersion = _blcelc.iLightList_scfGetVersion

class iLightingProcessData(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLightingProcessData, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLightingProcessData, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightingProcessData instance at %s>" % (self.this,)
    def FinalizeLighting(*args): return _blcelc.iLightingProcessData_FinalizeLighting(*args)
    def __del__(self, destroy=_blcelc.delete_iLightingProcessData):
        try:
            if self.thisown: destroy(self)
        except: pass

class iLightingProcessDataPtr(iLightingProcessData):
    def __init__(self, this):
        _swig_setattr(self, iLightingProcessData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLightingProcessData, 'thisown', 0)
        _swig_setattr(self, iLightingProcessData,self.__class__,iLightingProcessData)
_blcelc.iLightingProcessData_swigregister(iLightingProcessDataPtr)

class iLightingProcessInfo(iFrustumViewUserdata):
    __swig_setmethods__ = {}
    for _s in [iFrustumViewUserdata]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLightingProcessInfo, name, value)
    __swig_getmethods__ = {}
    for _s in [iFrustumViewUserdata]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLightingProcessInfo, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightingProcessInfo instance at %s>" % (self.this,)
    def GetLight(*args): return _blcelc.iLightingProcessInfo_GetLight(*args)
    def IsDynamic(*args): return _blcelc.iLightingProcessInfo_IsDynamic(*args)
    def SetColor(*args): return _blcelc.iLightingProcessInfo_SetColor(*args)
    def GetColor(*args): return _blcelc.iLightingProcessInfo_GetColor(*args)
    def AttachUserdata(*args): return _blcelc.iLightingProcessInfo_AttachUserdata(*args)
    def QueryUserdata(*args): return _blcelc.iLightingProcessInfo_QueryUserdata(*args)
    def FinalizeLighting(*args): return _blcelc.iLightingProcessInfo_FinalizeLighting(*args)
    def __del__(self, destroy=_blcelc.delete_iLightingProcessInfo):
        try:
            if self.thisown: destroy(self)
        except: pass

class iLightingProcessInfoPtr(iLightingProcessInfo):
    def __init__(self, this):
        _swig_setattr(self, iLightingProcessInfo, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLightingProcessInfo, 'thisown', 0)
        _swig_setattr(self, iLightingProcessInfo,self.__class__,iLightingProcessInfo)
_blcelc.iLightingProcessInfo_swigregister(iLightingProcessInfoPtr)

class iLightIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLightIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLightIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightIterator instance at %s>" % (self.this,)
    def HasNext(*args): return _blcelc.iLightIterator_HasNext(*args)
    def Next(*args): return _blcelc.iLightIterator_Next(*args)
    def GetLastSector(*args): return _blcelc.iLightIterator_GetLastSector(*args)
    def Reset(*args): return _blcelc.iLightIterator_Reset(*args)
    def __del__(self, destroy=_blcelc.delete_iLightIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class iLightIteratorPtr(iLightIterator):
    def __init__(self, this):
        _swig_setattr(self, iLightIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLightIterator, 'thisown', 0)
        _swig_setattr(self, iLightIterator,self.__class__,iLightIterator)
_blcelc.iLightIterator_swigregister(iLightIteratorPtr)

class iSectorCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSectorCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSectorCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSectorCallback instance at %s>" % (self.this,)
    def Traverse(*args): return _blcelc.iSectorCallback_Traverse(*args)
    def __del__(self, destroy=_blcelc.delete_iSectorCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSectorCallbackPtr(iSectorCallback):
    def __init__(self, this):
        _swig_setattr(self, iSectorCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSectorCallback, 'thisown', 0)
        _swig_setattr(self, iSectorCallback,self.__class__,iSectorCallback)
_blcelc.iSectorCallback_swigregister(iSectorCallbackPtr)

class iSectorMeshCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSectorMeshCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSectorMeshCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSectorMeshCallback instance at %s>" % (self.this,)
    def NewMesh(*args): return _blcelc.iSectorMeshCallback_NewMesh(*args)
    def RemoveMesh(*args): return _blcelc.iSectorMeshCallback_RemoveMesh(*args)
    def __del__(self, destroy=_blcelc.delete_iSectorMeshCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSectorMeshCallbackPtr(iSectorMeshCallback):
    def __init__(self, this):
        _swig_setattr(self, iSectorMeshCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSectorMeshCallback, 'thisown', 0)
        _swig_setattr(self, iSectorMeshCallback,self.__class__,iSectorMeshCallback)
_blcelc.iSectorMeshCallback_swigregister(iSectorMeshCallbackPtr)

class iSector(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSector, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSector, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSector instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iSector_QueryObject(*args)
    def SetRenderLoop(*args): return _blcelc.iSector_SetRenderLoop(*args)
    def GetRenderLoop(*args): return _blcelc.iSector_GetRenderLoop(*args)
    def HasFog(*args): return _blcelc.iSector_HasFog(*args)
    def GetFog(*args): return _blcelc.iSector_GetFog(*args)
    def SetFog(*args): return _blcelc.iSector_SetFog(*args)
    def DisableFog(*args): return _blcelc.iSector_DisableFog(*args)
    def GetMeshes(*args): return _blcelc.iSector_GetMeshes(*args)
    def GetLights(*args): return _blcelc.iSector_GetLights(*args)
    def ShineLights(*args): return _blcelc.iSector_ShineLights(*args)
    def SetDynamicAmbientLight(*args): return _blcelc.iSector_SetDynamicAmbientLight(*args)
    def GetDynamicAmbientLight(*args): return _blcelc.iSector_GetDynamicAmbientLight(*args)
    def CalculateSectorBBox(*args): return _blcelc.iSector_CalculateSectorBBox(*args)
    def SetVisibilityCullerPlugin(*args): return _blcelc.iSector_SetVisibilityCullerPlugin(*args)
    def GetVisibilityCuller(*args): return _blcelc.iSector_GetVisibilityCuller(*args)
    def GetRecLevel(*args): return _blcelc.iSector_GetRecLevel(*args)
    def IncRecLevel(*args): return _blcelc.iSector_IncRecLevel(*args)
    def DecRecLevel(*args): return _blcelc.iSector_DecRecLevel(*args)
    def HitBeamPortals(*args): return _blcelc.iSector_HitBeamPortals(*args)
    def HitBeam(*args): return _blcelc.iSector_HitBeam(*args)
    def FollowSegment(*args): return _blcelc.iSector_FollowSegment(*args)
    def Draw(*args): return _blcelc.iSector_Draw(*args)
    def PrepareDraw(*args): return _blcelc.iSector_PrepareDraw(*args)
    def GetVisibleMeshes(*args): return _blcelc.iSector_GetVisibleMeshes(*args)
    def SetSectorCallback(*args): return _blcelc.iSector_SetSectorCallback(*args)
    def RemoveSectorCallback(*args): return _blcelc.iSector_RemoveSectorCallback(*args)
    def GetSectorCallbackCount(*args): return _blcelc.iSector_GetSectorCallbackCount(*args)
    def GetSectorCallback(*args): return _blcelc.iSector_GetSectorCallback(*args)
    def AddSectorMeshCallback(*args): return _blcelc.iSector_AddSectorMeshCallback(*args)
    def RemoveSectorMeshCallback(*args): return _blcelc.iSector_RemoveSectorMeshCallback(*args)
    def CheckFrustum(*args): return _blcelc.iSector_CheckFrustum(*args)
    def GetPortalMeshes(*args): return _blcelc.iSector_GetPortalMeshes(*args)
    def RegisterPortalMesh(*args): return _blcelc.iSector_RegisterPortalMesh(*args)
    def UnregisterPortalMesh(*args): return _blcelc.iSector_UnregisterPortalMesh(*args)
    def UnlinkObjects(*args): return _blcelc.iSector_UnlinkObjects(*args)
    def __del__(self, destroy=_blcelc.delete_iSector):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSector_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSector_scfGetVersion)

class iSectorPtr(iSector):
    def __init__(self, this):
        _swig_setattr(self, iSector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSector, 'thisown', 0)
        _swig_setattr(self, iSector,self.__class__,iSector)
_blcelc.iSector_swigregister(iSectorPtr)

iSector_scfGetVersion = _blcelc.iSector_scfGetVersion

class iSectorList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSectorList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSectorList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSectorList instance at %s>" % (self.this,)
    def GetCount(*args): return _blcelc.iSectorList_GetCount(*args)
    def Get(*args): return _blcelc.iSectorList_Get(*args)
    def Add(*args): return _blcelc.iSectorList_Add(*args)
    def Remove(*args): return _blcelc.iSectorList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iSectorList_RemoveAll(*args)
    def Find(*args): return _blcelc.iSectorList_Find(*args)
    def FindByName(*args): return _blcelc.iSectorList_FindByName(*args)
    def __del__(self, destroy=_blcelc.delete_iSectorList):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSectorList_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSectorList_scfGetVersion)

class iSectorListPtr(iSectorList):
    def __init__(self, this):
        _swig_setattr(self, iSectorList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSectorList, 'thisown', 0)
        _swig_setattr(self, iSectorList,self.__class__,iSectorList)
_blcelc.iSectorList_swigregister(iSectorListPtr)

iSectorList_scfGetVersion = _blcelc.iSectorList_scfGetVersion

class iSectorIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSectorIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSectorIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSectorIterator instance at %s>" % (self.this,)
    def HasNext(*args): return _blcelc.iSectorIterator_HasNext(*args)
    def Next(*args): return _blcelc.iSectorIterator_Next(*args)
    def GetLastPosition(*args): return _blcelc.iSectorIterator_GetLastPosition(*args)
    def Reset(*args): return _blcelc.iSectorIterator_Reset(*args)
    def __del__(self, destroy=_blcelc.delete_iSectorIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSectorIteratorPtr(iSectorIterator):
    def __init__(self, this):
        _swig_setattr(self, iSectorIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSectorIterator, 'thisown', 0)
        _swig_setattr(self, iSectorIterator,self.__class__,iSectorIterator)
_blcelc.iSectorIterator_swigregister(iSectorIteratorPtr)

CS_ENGINE_CACHE_READ = _blcelc.CS_ENGINE_CACHE_READ
CS_ENGINE_CACHE_WRITE = _blcelc.CS_ENGINE_CACHE_WRITE
CS_ENGINE_CACHE_NOUPDATE = _blcelc.CS_ENGINE_CACHE_NOUPDATE
CS_RENDPRI_NONE = _blcelc.CS_RENDPRI_NONE
CS_RENDPRI_BACK2FRONT = _blcelc.CS_RENDPRI_BACK2FRONT
CS_RENDPRI_FRONT2BACK = _blcelc.CS_RENDPRI_FRONT2BACK
class iEngineSectorCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEngineSectorCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEngineSectorCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEngineSectorCallback instance at %s>" % (self.this,)
    def NewSector(*args): return _blcelc.iEngineSectorCallback_NewSector(*args)
    def RemoveSector(*args): return _blcelc.iEngineSectorCallback_RemoveSector(*args)
    def __del__(self, destroy=_blcelc.delete_iEngineSectorCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iEngineSectorCallbackPtr(iEngineSectorCallback):
    def __init__(self, this):
        _swig_setattr(self, iEngineSectorCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEngineSectorCallback, 'thisown', 0)
        _swig_setattr(self, iEngineSectorCallback,self.__class__,iEngineSectorCallback)
_blcelc.iEngineSectorCallback_swigregister(iEngineSectorCallbackPtr)

class iEngine(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEngine, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEngine, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEngine instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iEngine_QueryObject(*args)
    def Prepare(*args): return _blcelc.iEngine_Prepare(*args)
    def ForceRelight(*args): return _blcelc.iEngine_ForceRelight(*args)
    def RemoveLight(*args): return _blcelc.iEngine_RemoveLight(*args)
    def PrepareTextures(*args): return _blcelc.iEngine_PrepareTextures(*args)
    def PrepareMeshes(*args): return _blcelc.iEngine_PrepareMeshes(*args)
    def ShineLights(*args): return _blcelc.iEngine_ShineLights(*args)
    def GetTextureFormat(*args): return _blcelc.iEngine_GetTextureFormat(*args)
    def DeleteAll(*args): return _blcelc.iEngine_DeleteAll(*args)
    def RegisterRenderPriority(*args): return _blcelc.iEngine_RegisterRenderPriority(*args)
    def GetRenderPriority(*args): return _blcelc.iEngine_GetRenderPriority(*args)
    def GetRenderPrioritySorting(*args): return _blcelc.iEngine_GetRenderPrioritySorting(*args)
    def GetSkyRenderPriority(*args): return _blcelc.iEngine_GetSkyRenderPriority(*args)
    def GetPortalRenderPriority(*args): return _blcelc.iEngine_GetPortalRenderPriority(*args)
    def GetWallRenderPriority(*args): return _blcelc.iEngine_GetWallRenderPriority(*args)
    def GetObjectRenderPriority(*args): return _blcelc.iEngine_GetObjectRenderPriority(*args)
    def GetAlphaRenderPriority(*args): return _blcelc.iEngine_GetAlphaRenderPriority(*args)
    def ClearRenderPriorities(*args): return _blcelc.iEngine_ClearRenderPriorities(*args)
    def GetRenderPriorityCount(*args): return _blcelc.iEngine_GetRenderPriorityCount(*args)
    def GetRenderPriorityName(*args): return _blcelc.iEngine_GetRenderPriorityName(*args)
    def CreateBaseMaterial(*args): return _blcelc.iEngine_CreateBaseMaterial(*args)
    def CreateTexture(*args): return _blcelc.iEngine_CreateTexture(*args)
    def CreateBlackTexture(*args): return _blcelc.iEngine_CreateBlackTexture(*args)
    def CreateMaterial(*args): return _blcelc.iEngine_CreateMaterial(*args)
    def CreateSector(*args): return _blcelc.iEngine_CreateSector(*args)
    def AddEngineSectorCallback(*args): return _blcelc.iEngine_AddEngineSectorCallback(*args)
    def RemoveEngineSectorCallback(*args): return _blcelc.iEngine_RemoveEngineSectorCallback(*args)
    def CreateSectorWallsMesh(*args): return _blcelc.iEngine_CreateSectorWallsMesh(*args)
    def CreateThingMesh(*args): return _blcelc.iEngine_CreateThingMesh(*args)
    def GetSectors(*args): return _blcelc.iEngine_GetSectors(*args)
    def GetMeshFactories(*args): return _blcelc.iEngine_GetMeshFactories(*args)
    def GetMeshes(*args): return _blcelc.iEngine_GetMeshes(*args)
    def GetCollections(*args): return _blcelc.iEngine_GetCollections(*args)
    def GetCameraPositions(*args): return _blcelc.iEngine_GetCameraPositions(*args)
    def GetTextureList(*args): return _blcelc.iEngine_GetTextureList(*args)
    def GetMaterialList(*args): return _blcelc.iEngine_GetMaterialList(*args)
    def GetVariableList(*args): return _blcelc.iEngine_GetVariableList(*args)
    def AddMeshAndChildren(*args): return _blcelc.iEngine_AddMeshAndChildren(*args)
    def CreateRegion(*args): return _blcelc.iEngine_CreateRegion(*args)
    def GetRegions(*args): return _blcelc.iEngine_GetRegions(*args)
    def FindMaterial(*args): return _blcelc.iEngine_FindMaterial(*args)
    def FindTexture(*args): return _blcelc.iEngine_FindTexture(*args)
    def FindSector(*args): return _blcelc.iEngine_FindSector(*args)
    def FindMeshObject(*args): return _blcelc.iEngine_FindMeshObject(*args)
    def FindMeshFactory(*args): return _blcelc.iEngine_FindMeshFactory(*args)
    def FindCameraPosition(*args): return _blcelc.iEngine_FindCameraPosition(*args)
    def FindCollection(*args): return _blcelc.iEngine_FindCollection(*args)
    def SetLightingCacheMode(*args): return _blcelc.iEngine_SetLightingCacheMode(*args)
    def GetLightingCacheMode(*args): return _blcelc.iEngine_GetLightingCacheMode(*args)
    def SetClearZBuf(*args): return _blcelc.iEngine_SetClearZBuf(*args)
    def GetClearZBuf(*args): return _blcelc.iEngine_GetClearZBuf(*args)
    def GetDefaultClearZBuf(*args): return _blcelc.iEngine_GetDefaultClearZBuf(*args)
    def SetClearScreen(*args): return _blcelc.iEngine_SetClearScreen(*args)
    def GetClearScreen(*args): return _blcelc.iEngine_GetClearScreen(*args)
    def GetDefaultClearScreen(*args): return _blcelc.iEngine_GetDefaultClearScreen(*args)
    def SetMaxLightmapSize(*args): return _blcelc.iEngine_SetMaxLightmapSize(*args)
    def GetMaxLightmapSize(*args): return _blcelc.iEngine_GetMaxLightmapSize(*args)
    def GetDefaultMaxLightmapSize(*args): return _blcelc.iEngine_GetDefaultMaxLightmapSize(*args)
    def GetMaxLightmapAspectRatio(*args): return _blcelc.iEngine_GetMaxLightmapAspectRatio(*args)
    def ResetWorldSpecificSettings(*args): return _blcelc.iEngine_ResetWorldSpecificSettings(*args)
    def CreateCamera(*args): return _blcelc.iEngine_CreateCamera(*args)
    def CreateLight(*args): return _blcelc.iEngine_CreateLight(*args)
    def FindLight(*args): return _blcelc.iEngine_FindLight(*args)
    def FindLightID(*args): return _blcelc.iEngine_FindLightID(*args)
    def GetLightIterator(*args): return _blcelc.iEngine_GetLightIterator(*args)
    def GetBeginDrawFlags(*args): return _blcelc.iEngine_GetBeginDrawFlags(*args)
    def GetTopLevelClipper(*args): return _blcelc.iEngine_GetTopLevelClipper(*args)
    def CreateMeshFactory(*args): return _blcelc.iEngine_CreateMeshFactory(*args)
    def CreateLoaderContext(*args): return _blcelc.iEngine_CreateLoaderContext(*args)
    def LoadMeshFactory(*args): return _blcelc.iEngine_LoadMeshFactory(*args)
    def CreateMeshWrapper(*args): return _blcelc.iEngine_CreateMeshWrapper(*args)
    def LoadMeshWrapper(*args): return _blcelc.iEngine_LoadMeshWrapper(*args)
    def CreatePortalContainer(*args): return _blcelc.iEngine_CreatePortalContainer(*args)
    def CreatePortal(*args): return _blcelc.iEngine_CreatePortal(*args)
    def PrecacheDraw(*args): return _blcelc.iEngine_PrecacheDraw(*args)
    def Draw(*args): return _blcelc.iEngine_Draw(*args)
    def SetContext(*args): return _blcelc.iEngine_SetContext(*args)
    def GetContext(*args): return _blcelc.iEngine_GetContext(*args)
    def SetAmbientLight(*args): return _blcelc.iEngine_SetAmbientLight(*args)
    def GetAmbientLight(*args): return _blcelc.iEngine_GetAmbientLight(*args)
    def GetNearbyLights(*args): return _blcelc.iEngine_GetNearbyLights(*args)
    def GetNearbySectors(*args): return _blcelc.iEngine_GetNearbySectors(*args)
    def GetNearbyObjects(*args): return _blcelc.iEngine_GetNearbyObjects(*args)
    def GetNearbyMeshes(*args): return _blcelc.iEngine_GetNearbyMeshes(*args)
    def GetVisibleObjects(*args): return _blcelc.iEngine_GetVisibleObjects(*args)
    def GetVisibleMeshes(*args): return _blcelc.iEngine_GetVisibleMeshes(*args)
    def RemoveObject(*args): return _blcelc.iEngine_RemoveObject(*args)
    def SetCacheManager(*args): return _blcelc.iEngine_SetCacheManager(*args)
    def GetCacheManager(*args): return _blcelc.iEngine_GetCacheManager(*args)
    def GetDefaultAmbientLight(*args): return _blcelc.iEngine_GetDefaultAmbientLight(*args)
    def CreateFrustumView(*args): return _blcelc.iEngine_CreateFrustumView(*args)
    def CreateObjectWatcher(*args): return _blcelc.iEngine_CreateObjectWatcher(*args)
    def WantToDie(*args): return _blcelc.iEngine_WantToDie(*args)
    def GetRenderLoopManager(*args): return _blcelc.iEngine_GetRenderLoopManager(*args)
    def GetCurrentDefaultRenderloop(*args): return _blcelc.iEngine_GetCurrentDefaultRenderloop(*args)
    def SetCurrentDefaultRenderloop(*args): return _blcelc.iEngine_SetCurrentDefaultRenderloop(*args)
    def GetCurrentFrameNumber(*args): return _blcelc.iEngine_GetCurrentFrameNumber(*args)
    def SetSaveableFlag(*args): return _blcelc.iEngine_SetSaveableFlag(*args)
    def GetSaveableFlag(*args): return _blcelc.iEngine_GetSaveableFlag(*args)
    def __del__(self, destroy=_blcelc.delete_iEngine):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iEngine_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iEngine_scfGetVersion)

class iEnginePtr(iEngine):
    def __init__(self, this):
        _swig_setattr(self, iEngine, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEngine, 'thisown', 0)
        _swig_setattr(self, iEngine,self.__class__,iEngine)
_blcelc.iEngine_swigregister(iEnginePtr)

iEngine_scfGetVersion = _blcelc.iEngine_scfGetVersion

class iCameraSectorListener(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCameraSectorListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCameraSectorListener, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCameraSectorListener instance at %s>" % (self.this,)
    def NewSector(*args): return _blcelc.iCameraSectorListener_NewSector(*args)
    def __del__(self, destroy=_blcelc.delete_iCameraSectorListener):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCameraSectorListenerPtr(iCameraSectorListener):
    def __init__(self, this):
        _swig_setattr(self, iCameraSectorListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCameraSectorListener, 'thisown', 0)
        _swig_setattr(self, iCameraSectorListener,self.__class__,iCameraSectorListener)
_blcelc.iCameraSectorListener_swigregister(iCameraSectorListenerPtr)

class iCamera(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCamera, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCamera instance at %s>" % (self.this,)
    def Clone(*args): return _blcelc.iCamera_Clone(*args)
    def GetFOV(*args): return _blcelc.iCamera_GetFOV(*args)
    def GetInvFOV(*args): return _blcelc.iCamera_GetInvFOV(*args)
    def GetFOVAngle(*args): return _blcelc.iCamera_GetFOVAngle(*args)
    def SetFOV(*args): return _blcelc.iCamera_SetFOV(*args)
    def SetFOVAngle(*args): return _blcelc.iCamera_SetFOVAngle(*args)
    def GetShiftX(*args): return _blcelc.iCamera_GetShiftX(*args)
    def GetShiftY(*args): return _blcelc.iCamera_GetShiftY(*args)
    def SetPerspectiveCenter(*args): return _blcelc.iCamera_SetPerspectiveCenter(*args)
    def GetTransform(*args): return _blcelc.iCamera_GetTransform(*args)
    def SetTransform(*args): return _blcelc.iCamera_SetTransform(*args)
    def MoveWorld(*args): return _blcelc.iCamera_MoveWorld(*args)
    def Move(*args): return _blcelc.iCamera_Move(*args)
    def MoveWorldUnrestricted(*args): return _blcelc.iCamera_MoveWorldUnrestricted(*args)
    def MoveUnrestricted(*args): return _blcelc.iCamera_MoveUnrestricted(*args)
    def GetSector(*args): return _blcelc.iCamera_GetSector(*args)
    def SetSector(*args): return _blcelc.iCamera_SetSector(*args)
    def Correct(*args): return _blcelc.iCamera_Correct(*args)
    def IsMirrored(*args): return _blcelc.iCamera_IsMirrored(*args)
    def SetMirrored(*args): return _blcelc.iCamera_SetMirrored(*args)
    def GetFarPlane(*args): return _blcelc.iCamera_GetFarPlane(*args)
    def SetFarPlane(*args): return _blcelc.iCamera_SetFarPlane(*args)
    def GetCameraNumber(*args): return _blcelc.iCamera_GetCameraNumber(*args)
    def Perspective(*args): return _blcelc.iCamera_Perspective(*args)
    def InvPerspective(*args): return _blcelc.iCamera_InvPerspective(*args)
    def OnlyPortals(*args): return _blcelc.iCamera_OnlyPortals(*args)
    def GetOnlyPortals(*args): return _blcelc.iCamera_GetOnlyPortals(*args)
    def AddCameraSectorListener(*args): return _blcelc.iCamera_AddCameraSectorListener(*args)
    def RemoveCameraSectorListener(*args): return _blcelc.iCamera_RemoveCameraSectorListener(*args)
    def __del__(self, destroy=_blcelc.delete_iCamera):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCamera_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCamera_scfGetVersion)

class iCameraPtr(iCamera):
    def __init__(self, this):
        _swig_setattr(self, iCamera, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCamera, 'thisown', 0)
        _swig_setattr(self, iCamera,self.__class__,iCamera)
_blcelc.iCamera_swigregister(iCameraPtr)

iCamera_scfGetVersion = _blcelc.iCamera_scfGetVersion

class iCameraPosition(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCameraPosition, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCameraPosition, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCameraPosition instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iCameraPosition_QueryObject(*args)
    def Clone(*args): return _blcelc.iCameraPosition_Clone(*args)
    def GetSector(*args): return _blcelc.iCameraPosition_GetSector(*args)
    def SetSector(*args): return _blcelc.iCameraPosition_SetSector(*args)
    def GetPosition(*args): return _blcelc.iCameraPosition_GetPosition(*args)
    def SetPosition(*args): return _blcelc.iCameraPosition_SetPosition(*args)
    def GetUpwardVector(*args): return _blcelc.iCameraPosition_GetUpwardVector(*args)
    def SetUpwardVector(*args): return _blcelc.iCameraPosition_SetUpwardVector(*args)
    def GetForwardVector(*args): return _blcelc.iCameraPosition_GetForwardVector(*args)
    def SetForwardVector(*args): return _blcelc.iCameraPosition_SetForwardVector(*args)
    def Set(*args): return _blcelc.iCameraPosition_Set(*args)
    def Load(*args): return _blcelc.iCameraPosition_Load(*args)
    def SetFarPlane(*args): return _blcelc.iCameraPosition_SetFarPlane(*args)
    def ClearFarPlane(*args): return _blcelc.iCameraPosition_ClearFarPlane(*args)
    def GetFarPlane(*args): return _blcelc.iCameraPosition_GetFarPlane(*args)
    def __del__(self, destroy=_blcelc.delete_iCameraPosition):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCameraPosition_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCameraPosition_scfGetVersion)

class iCameraPositionPtr(iCameraPosition):
    def __init__(self, this):
        _swig_setattr(self, iCameraPosition, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCameraPosition, 'thisown', 0)
        _swig_setattr(self, iCameraPosition,self.__class__,iCameraPosition)
_blcelc.iCameraPosition_swigregister(iCameraPositionPtr)

iCameraPosition_scfGetVersion = _blcelc.iCameraPosition_scfGetVersion

class iCameraPositionList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCameraPositionList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCameraPositionList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCameraPositionList instance at %s>" % (self.this,)
    def NewCameraPosition(*args): return _blcelc.iCameraPositionList_NewCameraPosition(*args)
    def GetCount(*args): return _blcelc.iCameraPositionList_GetCount(*args)
    def Get(*args): return _blcelc.iCameraPositionList_Get(*args)
    def Add(*args): return _blcelc.iCameraPositionList_Add(*args)
    def Remove(*args): return _blcelc.iCameraPositionList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iCameraPositionList_RemoveAll(*args)
    def Find(*args): return _blcelc.iCameraPositionList_Find(*args)
    def FindByName(*args): return _blcelc.iCameraPositionList_FindByName(*args)
    def __del__(self, destroy=_blcelc.delete_iCameraPositionList):
        try:
            if self.thisown: destroy(self)
        except: pass

class iCameraPositionListPtr(iCameraPositionList):
    def __init__(self, this):
        _swig_setattr(self, iCameraPositionList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCameraPositionList, 'thisown', 0)
        _swig_setattr(self, iCameraPositionList,self.__class__,iCameraPositionList)
_blcelc.iCameraPositionList_swigregister(iCameraPositionListPtr)

class iTextureCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iTextureCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iTextureCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureCallback instance at %s>" % (self.this,)
    def UseTexture(*args): return _blcelc.iTextureCallback_UseTexture(*args)
    def __del__(self, destroy=_blcelc.delete_iTextureCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iTextureCallbackPtr(iTextureCallback):
    def __init__(self, this):
        _swig_setattr(self, iTextureCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iTextureCallback, 'thisown', 0)
        _swig_setattr(self, iTextureCallback,self.__class__,iTextureCallback)
_blcelc.iTextureCallback_swigregister(iTextureCallbackPtr)

class iTextureWrapper(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iTextureWrapper, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iTextureWrapper, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureWrapper instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iTextureWrapper_QueryObject(*args)
    def Clone(*args): return _blcelc.iTextureWrapper_Clone(*args)
    def SetImageFile(*args): return _blcelc.iTextureWrapper_SetImageFile(*args)
    def GetImageFile(*args): return _blcelc.iTextureWrapper_GetImageFile(*args)
    def SetTextureHandle(*args): return _blcelc.iTextureWrapper_SetTextureHandle(*args)
    def GetTextureHandle(*args): return _blcelc.iTextureWrapper_GetTextureHandle(*args)
    def SetKeyColor(*args): return _blcelc.iTextureWrapper_SetKeyColor(*args)
    def GetKeyColor(*args): return _blcelc.iTextureWrapper_GetKeyColor(*args)
    def SetFlags(*args): return _blcelc.iTextureWrapper_SetFlags(*args)
    def GetFlags(*args): return _blcelc.iTextureWrapper_GetFlags(*args)
    def Register(*args): return _blcelc.iTextureWrapper_Register(*args)
    def SetUseCallback(*args): return _blcelc.iTextureWrapper_SetUseCallback(*args)
    def GetUseCallback(*args): return _blcelc.iTextureWrapper_GetUseCallback(*args)
    def Visit(*args): return _blcelc.iTextureWrapper_Visit(*args)
    def IsVisitRequired(*args): return _blcelc.iTextureWrapper_IsVisitRequired(*args)
    def SetKeepImage(*args): return _blcelc.iTextureWrapper_SetKeepImage(*args)
    def KeepImage(*args): return _blcelc.iTextureWrapper_KeepImage(*args)
    def SetTextureClass(*args): return _blcelc.iTextureWrapper_SetTextureClass(*args)
    def GetTextureClass(*args): return _blcelc.iTextureWrapper_GetTextureClass(*args)
    def __del__(self, destroy=_blcelc.delete_iTextureWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iTextureWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iTextureWrapper_scfGetVersion)

class iTextureWrapperPtr(iTextureWrapper):
    def __init__(self, this):
        _swig_setattr(self, iTextureWrapper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iTextureWrapper, 'thisown', 0)
        _swig_setattr(self, iTextureWrapper,self.__class__,iTextureWrapper)
_blcelc.iTextureWrapper_swigregister(iTextureWrapperPtr)

iTextureWrapper_scfGetVersion = _blcelc.iTextureWrapper_scfGetVersion

class iTextureList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iTextureList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iTextureList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureList instance at %s>" % (self.this,)
    def NewTexture(*args): return _blcelc.iTextureList_NewTexture(*args)
    def GetCount(*args): return _blcelc.iTextureList_GetCount(*args)
    def Get(*args): return _blcelc.iTextureList_Get(*args)
    def Add(*args): return _blcelc.iTextureList_Add(*args)
    def Remove(*args): return _blcelc.iTextureList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iTextureList_RemoveAll(*args)
    def Find(*args): return _blcelc.iTextureList_Find(*args)
    def FindByName(*args): return _blcelc.iTextureList_FindByName(*args)
    def __del__(self, destroy=_blcelc.delete_iTextureList):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iTextureList_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iTextureList_scfGetVersion)

class iTextureListPtr(iTextureList):
    def __init__(self, this):
        _swig_setattr(self, iTextureList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iTextureList, 'thisown', 0)
        _swig_setattr(self, iTextureList,self.__class__,iTextureList)
_blcelc.iTextureList_swigregister(iTextureListPtr)

iTextureList_scfGetVersion = _blcelc.iTextureList_scfGetVersion

class iMaterialWrapper(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMaterialWrapper, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMaterialWrapper, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialWrapper instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iMaterialWrapper_QueryObject(*args)
    def Clone(*args): return _blcelc.iMaterialWrapper_Clone(*args)
    def SetMaterialHandle(*args): return _blcelc.iMaterialWrapper_SetMaterialHandle(*args)
    def GetMaterialHandle(*args): return _blcelc.iMaterialWrapper_GetMaterialHandle(*args)
    def SetMaterial(*args): return _blcelc.iMaterialWrapper_SetMaterial(*args)
    def GetMaterial(*args): return _blcelc.iMaterialWrapper_GetMaterial(*args)
    def Register(*args): return _blcelc.iMaterialWrapper_Register(*args)
    def Visit(*args): return _blcelc.iMaterialWrapper_Visit(*args)
    def IsVisitRequired(*args): return _blcelc.iMaterialWrapper_IsVisitRequired(*args)
    def __del__(self, destroy=_blcelc.delete_iMaterialWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMaterialWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMaterialWrapper_scfGetVersion)

class iMaterialWrapperPtr(iMaterialWrapper):
    def __init__(self, this):
        _swig_setattr(self, iMaterialWrapper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMaterialWrapper, 'thisown', 0)
        _swig_setattr(self, iMaterialWrapper,self.__class__,iMaterialWrapper)
_blcelc.iMaterialWrapper_swigregister(iMaterialWrapperPtr)

iMaterialWrapper_scfGetVersion = _blcelc.iMaterialWrapper_scfGetVersion

class iMaterialEngine(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMaterialEngine, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMaterialEngine, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialEngine instance at %s>" % (self.this,)
    def GetTextureWrapper(*args): return _blcelc.iMaterialEngine_GetTextureWrapper(*args)
    def Visit(*args): return _blcelc.iMaterialEngine_Visit(*args)
    def IsVisitRequired(*args): return _blcelc.iMaterialEngine_IsVisitRequired(*args)
    def __del__(self, destroy=_blcelc.delete_iMaterialEngine):
        try:
            if self.thisown: destroy(self)
        except: pass

class iMaterialEnginePtr(iMaterialEngine):
    def __init__(self, this):
        _swig_setattr(self, iMaterialEngine, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMaterialEngine, 'thisown', 0)
        _swig_setattr(self, iMaterialEngine,self.__class__,iMaterialEngine)
_blcelc.iMaterialEngine_swigregister(iMaterialEnginePtr)

class iMaterialList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMaterialList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMaterialList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialList instance at %s>" % (self.this,)
    def NewMaterial(*args): return _blcelc.iMaterialList_NewMaterial(*args)
    def GetCount(*args): return _blcelc.iMaterialList_GetCount(*args)
    def Get(*args): return _blcelc.iMaterialList_Get(*args)
    def Add(*args): return _blcelc.iMaterialList_Add(*args)
    def Remove(*args): return _blcelc.iMaterialList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iMaterialList_RemoveAll(*args)
    def Find(*args): return _blcelc.iMaterialList_Find(*args)
    def FindByName(*args): return _blcelc.iMaterialList_FindByName(*args)
    def __del__(self, destroy=_blcelc.delete_iMaterialList):
        try:
            if self.thisown: destroy(self)
        except: pass

class iMaterialListPtr(iMaterialList):
    def __init__(self, this):
        _swig_setattr(self, iMaterialList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMaterialList, 'thisown', 0)
        _swig_setattr(self, iMaterialList,self.__class__,iMaterialList)
_blcelc.iMaterialList_swigregister(iMaterialListPtr)

CS_ENTITY_DETAIL = _blcelc.CS_ENTITY_DETAIL
CS_ENTITY_CAMERA = _blcelc.CS_ENTITY_CAMERA
CS_ENTITY_INVISIBLEMESH = _blcelc.CS_ENTITY_INVISIBLEMESH
CS_ENTITY_NOSHADOWS = _blcelc.CS_ENTITY_NOSHADOWS
CS_ENTITY_NOLIGHTING = _blcelc.CS_ENTITY_NOLIGHTING
CS_ENTITY_NOHITBEAM = _blcelc.CS_ENTITY_NOHITBEAM
CS_LIGHTINGUPDATE_SORTRELEVANCE = _blcelc.CS_LIGHTINGUPDATE_SORTRELEVANCE
CS_LIGHTINGUPDATE_ALWAYSUPDATE = _blcelc.CS_LIGHTINGUPDATE_ALWAYSUPDATE
class iMeshDrawCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshDrawCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshDrawCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshDrawCallback instance at %s>" % (self.this,)
    def BeforeDrawing(*args): return _blcelc.iMeshDrawCallback_BeforeDrawing(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshDrawCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iMeshDrawCallbackPtr(iMeshDrawCallback):
    def __init__(self, this):
        _swig_setattr(self, iMeshDrawCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshDrawCallback, 'thisown', 0)
        _swig_setattr(self, iMeshDrawCallback,self.__class__,iMeshDrawCallback)
_blcelc.iMeshDrawCallback_swigregister(iMeshDrawCallbackPtr)

class iMeshWrapper(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshWrapper, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshWrapper, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshWrapper instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iMeshWrapper_QueryObject(*args)
    def GetMeshObject(*args): return _blcelc.iMeshWrapper_GetMeshObject(*args)
    def SetMeshObject(*args): return _blcelc.iMeshWrapper_SetMeshObject(*args)
    def GetPortalContainer(*args): return _blcelc.iMeshWrapper_GetPortalContainer(*args)
    def GetLightingInfo(*args): return _blcelc.iMeshWrapper_GetLightingInfo(*args)
    def GetShadowReceiver(*args): return _blcelc.iMeshWrapper_GetShadowReceiver(*args)
    def GetShadowCaster(*args): return _blcelc.iMeshWrapper_GetShadowCaster(*args)
    def GetVisibilityNumber(*args): return _blcelc.iMeshWrapper_GetVisibilityNumber(*args)
    def GetFactory(*args): return _blcelc.iMeshWrapper_GetFactory(*args)
    def SetFactory(*args): return _blcelc.iMeshWrapper_SetFactory(*args)
    def SetLightingUpdate(*args): return _blcelc.iMeshWrapper_SetLightingUpdate(*args)
    def GetMovable(*args): return _blcelc.iMeshWrapper_GetMovable(*args)
    def PlaceMesh(*args): return _blcelc.iMeshWrapper_PlaceMesh(*args)
    def HitBeamBBox(*args): return _blcelc.iMeshWrapper_HitBeamBBox(*args)
    def HitBeamOutline(*args): return _blcelc.iMeshWrapper_HitBeamOutline(*args)
    def HitBeamObject(*args): return _blcelc.iMeshWrapper_HitBeamObject(*args)
    def HitBeam(*args): return _blcelc.iMeshWrapper_HitBeam(*args)
    def SetDrawCallback(*args): return _blcelc.iMeshWrapper_SetDrawCallback(*args)
    def RemoveDrawCallback(*args): return _blcelc.iMeshWrapper_RemoveDrawCallback(*args)
    def GetDrawCallbackCount(*args): return _blcelc.iMeshWrapper_GetDrawCallbackCount(*args)
    def GetDrawCallback(*args): return _blcelc.iMeshWrapper_GetDrawCallback(*args)
    def SetRenderPriority(*args): return _blcelc.iMeshWrapper_SetRenderPriority(*args)
    def GetRenderPriority(*args): return _blcelc.iMeshWrapper_GetRenderPriority(*args)
    def SetRenderPriorityRecursive(*args): return _blcelc.iMeshWrapper_SetRenderPriorityRecursive(*args)
    def GetFlags(*args): return _blcelc.iMeshWrapper_GetFlags(*args)
    def SetFlagsRecursive(*args): return _blcelc.iMeshWrapper_SetFlagsRecursive(*args)
    def SetZBufMode(*args): return _blcelc.iMeshWrapper_SetZBufMode(*args)
    def GetZBufMode(*args): return _blcelc.iMeshWrapper_GetZBufMode(*args)
    def SetZBufModeRecursive(*args): return _blcelc.iMeshWrapper_SetZBufModeRecursive(*args)
    def HardTransform(*args): return _blcelc.iMeshWrapper_HardTransform(*args)
    def GetWorldBoundingBox(*args): return _blcelc.iMeshWrapper_GetWorldBoundingBox(*args)
    def GetTransformedBoundingBox(*args): return _blcelc.iMeshWrapper_GetTransformedBoundingBox(*args)
    def GetScreenBoundingBox(*args): return _blcelc.iMeshWrapper_GetScreenBoundingBox(*args)
    def GetChildren(*args): return _blcelc.iMeshWrapper_GetChildren(*args)
    def GetParentContainer(*args): return _blcelc.iMeshWrapper_GetParentContainer(*args)
    def SetParentContainer(*args): return _blcelc.iMeshWrapper_SetParentContainer(*args)
    def GetRadius(*args): return _blcelc.iMeshWrapper_GetRadius(*args)
    def CreateStaticLOD(*args): return _blcelc.iMeshWrapper_CreateStaticLOD(*args)
    def DestroyStaticLOD(*args): return _blcelc.iMeshWrapper_DestroyStaticLOD(*args)
    def GetStaticLOD(*args): return _blcelc.iMeshWrapper_GetStaticLOD(*args)
    def AddMeshToStaticLOD(*args): return _blcelc.iMeshWrapper_AddMeshToStaticLOD(*args)
    def RemoveMeshFromStaticLOD(*args): return _blcelc.iMeshWrapper_RemoveMeshFromStaticLOD(*args)
    def DrawShadow(*args): return _blcelc.iMeshWrapper_DrawShadow(*args)
    def DrawLight(*args): return _blcelc.iMeshWrapper_DrawLight(*args)
    def CastHardwareShadow(*args): return _blcelc.iMeshWrapper_CastHardwareShadow(*args)
    def SetDrawAfterShadow(*args): return _blcelc.iMeshWrapper_SetDrawAfterShadow(*args)
    def GetDrawAfterShadow(*args): return _blcelc.iMeshWrapper_GetDrawAfterShadow(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshWrapper_scfGetVersion)

class iMeshWrapperPtr(iMeshWrapper):
    def __init__(self, this):
        _swig_setattr(self, iMeshWrapper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshWrapper, 'thisown', 0)
        _swig_setattr(self, iMeshWrapper,self.__class__,iMeshWrapper)
_blcelc.iMeshWrapper_swigregister(iMeshWrapperPtr)

iMeshWrapper_scfGetVersion = _blcelc.iMeshWrapper_scfGetVersion

class iMeshFactoryWrapper(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshFactoryWrapper, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshFactoryWrapper, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshFactoryWrapper instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iMeshFactoryWrapper_QueryObject(*args)
    def GetMeshObjectFactory(*args): return _blcelc.iMeshFactoryWrapper_GetMeshObjectFactory(*args)
    def SetMeshObjectFactory(*args): return _blcelc.iMeshFactoryWrapper_SetMeshObjectFactory(*args)
    def HardTransform(*args): return _blcelc.iMeshFactoryWrapper_HardTransform(*args)
    def CreateMeshWrapper(*args): return _blcelc.iMeshFactoryWrapper_CreateMeshWrapper(*args)
    def GetParentContainer(*args): return _blcelc.iMeshFactoryWrapper_GetParentContainer(*args)
    def SetParentContainer(*args): return _blcelc.iMeshFactoryWrapper_SetParentContainer(*args)
    def GetChildren(*args): return _blcelc.iMeshFactoryWrapper_GetChildren(*args)
    def GetTransform(*args): return _blcelc.iMeshFactoryWrapper_GetTransform(*args)
    def SetTransform(*args): return _blcelc.iMeshFactoryWrapper_SetTransform(*args)
    def CreateStaticLOD(*args): return _blcelc.iMeshFactoryWrapper_CreateStaticLOD(*args)
    def DestroyStaticLOD(*args): return _blcelc.iMeshFactoryWrapper_DestroyStaticLOD(*args)
    def SetStaticLOD(*args): return _blcelc.iMeshFactoryWrapper_SetStaticLOD(*args)
    def GetStaticLOD(*args): return _blcelc.iMeshFactoryWrapper_GetStaticLOD(*args)
    def AddFactoryToStaticLOD(*args): return _blcelc.iMeshFactoryWrapper_AddFactoryToStaticLOD(*args)
    def RemoveFactoryFromStaticLOD(*args): return _blcelc.iMeshFactoryWrapper_RemoveFactoryFromStaticLOD(*args)
    def SetZBufMode(*args): return _blcelc.iMeshFactoryWrapper_SetZBufMode(*args)
    def GetZBufMode(*args): return _blcelc.iMeshFactoryWrapper_GetZBufMode(*args)
    def SetZBufModeRecursive(*args): return _blcelc.iMeshFactoryWrapper_SetZBufModeRecursive(*args)
    def SetRenderPriority(*args): return _blcelc.iMeshFactoryWrapper_SetRenderPriority(*args)
    def GetRenderPriority(*args): return _blcelc.iMeshFactoryWrapper_GetRenderPriority(*args)
    def SetRenderPriorityRecursive(*args): return _blcelc.iMeshFactoryWrapper_SetRenderPriorityRecursive(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshFactoryWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshFactoryWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshFactoryWrapper_scfGetVersion)

class iMeshFactoryWrapperPtr(iMeshFactoryWrapper):
    def __init__(self, this):
        _swig_setattr(self, iMeshFactoryWrapper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshFactoryWrapper, 'thisown', 0)
        _swig_setattr(self, iMeshFactoryWrapper,self.__class__,iMeshFactoryWrapper)
_blcelc.iMeshFactoryWrapper_swigregister(iMeshFactoryWrapperPtr)

iMeshFactoryWrapper_scfGetVersion = _blcelc.iMeshFactoryWrapper_scfGetVersion

class iMeshList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshList instance at %s>" % (self.this,)
    def GetCount(*args): return _blcelc.iMeshList_GetCount(*args)
    def Get(*args): return _blcelc.iMeshList_Get(*args)
    def Add(*args): return _blcelc.iMeshList_Add(*args)
    def Remove(*args): return _blcelc.iMeshList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iMeshList_RemoveAll(*args)
    def Find(*args): return _blcelc.iMeshList_Find(*args)
    def FindByName(*args): return _blcelc.iMeshList_FindByName(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshList):
        try:
            if self.thisown: destroy(self)
        except: pass

class iMeshListPtr(iMeshList):
    def __init__(self, this):
        _swig_setattr(self, iMeshList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshList, 'thisown', 0)
        _swig_setattr(self, iMeshList,self.__class__,iMeshList)
_blcelc.iMeshList_swigregister(iMeshListPtr)

class iMeshFactoryList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshFactoryList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshFactoryList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshFactoryList instance at %s>" % (self.this,)
    def GetCount(*args): return _blcelc.iMeshFactoryList_GetCount(*args)
    def Get(*args): return _blcelc.iMeshFactoryList_Get(*args)
    def Add(*args): return _blcelc.iMeshFactoryList_Add(*args)
    def Remove(*args): return _blcelc.iMeshFactoryList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iMeshFactoryList_RemoveAll(*args)
    def Find(*args): return _blcelc.iMeshFactoryList_Find(*args)
    def FindByName(*args): return _blcelc.iMeshFactoryList_FindByName(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshFactoryList):
        try:
            if self.thisown: destroy(self)
        except: pass

class iMeshFactoryListPtr(iMeshFactoryList):
    def __init__(self, this):
        _swig_setattr(self, iMeshFactoryList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshFactoryList, 'thisown', 0)
        _swig_setattr(self, iMeshFactoryList,self.__class__,iMeshFactoryList)
_blcelc.iMeshFactoryList_swigregister(iMeshFactoryListPtr)

class iMeshWrapperIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshWrapperIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshWrapperIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshWrapperIterator instance at %s>" % (self.this,)
    def Next(*args): return _blcelc.iMeshWrapperIterator_Next(*args)
    def Reset(*args): return _blcelc.iMeshWrapperIterator_Reset(*args)
    def HasNext(*args): return _blcelc.iMeshWrapperIterator_HasNext(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshWrapperIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshWrapperIterator_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshWrapperIterator_scfGetVersion)

class iMeshWrapperIteratorPtr(iMeshWrapperIterator):
    def __init__(self, this):
        _swig_setattr(self, iMeshWrapperIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshWrapperIterator, 'thisown', 0)
        _swig_setattr(self, iMeshWrapperIterator,self.__class__,iMeshWrapperIterator)
_blcelc.iMeshWrapperIterator_swigregister(iMeshWrapperIteratorPtr)

iMeshWrapperIterator_scfGetVersion = _blcelc.iMeshWrapperIterator_scfGetVersion

class iMovableListener(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMovableListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMovableListener, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMovableListener instance at %s>" % (self.this,)
    def MovableChanged(*args): return _blcelc.iMovableListener_MovableChanged(*args)
    def MovableDestroyed(*args): return _blcelc.iMovableListener_MovableDestroyed(*args)
    def __del__(self, destroy=_blcelc.delete_iMovableListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMovableListener_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMovableListener_scfGetVersion)

class iMovableListenerPtr(iMovableListener):
    def __init__(self, this):
        _swig_setattr(self, iMovableListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMovableListener, 'thisown', 0)
        _swig_setattr(self, iMovableListener,self.__class__,iMovableListener)
_blcelc.iMovableListener_swigregister(iMovableListenerPtr)

iMovableListener_scfGetVersion = _blcelc.iMovableListener_scfGetVersion

class iMovable(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMovable, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMovable, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMovable instance at %s>" % (self.this,)
    def GetParent(*args): return _blcelc.iMovable_GetParent(*args)
    def SetParent(*args): return _blcelc.iMovable_SetParent(*args)
    def SetSector(*args): return _blcelc.iMovable_SetSector(*args)
    def ClearSectors(*args): return _blcelc.iMovable_ClearSectors(*args)
    def GetSectors(*args): return _blcelc.iMovable_GetSectors(*args)
    def InSector(*args): return _blcelc.iMovable_InSector(*args)
    def SetPosition(*args): return _blcelc.iMovable_SetPosition(*args)
    def GetPosition(*args): return _blcelc.iMovable_GetPosition(*args)
    def GetFullPosition(*args): return _blcelc.iMovable_GetFullPosition(*args)
    def SetTransform(*args): return _blcelc.iMovable_SetTransform(*args)
    def GetTransform(*args): return _blcelc.iMovable_GetTransform(*args)
    def GetFullTransform(*args): return _blcelc.iMovable_GetFullTransform(*args)
    def MovePosition(*args): return _blcelc.iMovable_MovePosition(*args)
    def Transform(*args): return _blcelc.iMovable_Transform(*args)
    def AddListener(*args): return _blcelc.iMovable_AddListener(*args)
    def RemoveListener(*args): return _blcelc.iMovable_RemoveListener(*args)
    def UpdateMove(*args): return _blcelc.iMovable_UpdateMove(*args)
    def GetUpdateNumber(*args): return _blcelc.iMovable_GetUpdateNumber(*args)
    def IsTransformIdentity(*args): return _blcelc.iMovable_IsTransformIdentity(*args)
    def IsFullTransformIdentity(*args): return _blcelc.iMovable_IsFullTransformIdentity(*args)
    def TransformIdentity(*args): return _blcelc.iMovable_TransformIdentity(*args)
    def __del__(self, destroy=_blcelc.delete_iMovable):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMovable_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMovable_scfGetVersion)

class iMovablePtr(iMovable):
    def __init__(self, this):
        _swig_setattr(self, iMovable, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMovable, 'thisown', 0)
        _swig_setattr(self, iMovable,self.__class__,iMovable)
_blcelc.iMovable_swigregister(iMovablePtr)

iMovable_scfGetVersion = _blcelc.iMovable_scfGetVersion

class iRegion(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iRegion, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iRegion, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iRegion instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iRegion_QueryObject(*args)
    def Add(*args): return _blcelc.iRegion_Add(*args)
    def Remove(*args): return _blcelc.iRegion_Remove(*args)
    def Clear(*args): return _blcelc.iRegion_Clear(*args)
    def DeleteAll(*args): return _blcelc.iRegion_DeleteAll(*args)
    def PrepareTextures(*args): return _blcelc.iRegion_PrepareTextures(*args)
    def ShineLights(*args): return _blcelc.iRegion_ShineLights(*args)
    def Prepare(*args): return _blcelc.iRegion_Prepare(*args)
    def FindSector(*args): return _blcelc.iRegion_FindSector(*args)
    def FindMeshObject(*args): return _blcelc.iRegion_FindMeshObject(*args)
    def FindMeshFactory(*args): return _blcelc.iRegion_FindMeshFactory(*args)
    def FindTexture(*args): return _blcelc.iRegion_FindTexture(*args)
    def FindMaterial(*args): return _blcelc.iRegion_FindMaterial(*args)
    def FindCameraPosition(*args): return _blcelc.iRegion_FindCameraPosition(*args)
    def FindCollection(*args): return _blcelc.iRegion_FindCollection(*args)
    def IsInRegion(*args): return _blcelc.iRegion_IsInRegion(*args)
    def __del__(self, destroy=_blcelc.delete_iRegion):
        try:
            if self.thisown: destroy(self)
        except: pass

class iRegionPtr(iRegion):
    def __init__(self, this):
        _swig_setattr(self, iRegion, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iRegion, 'thisown', 0)
        _swig_setattr(self, iRegion,self.__class__,iRegion)
_blcelc.iRegion_swigregister(iRegionPtr)

class iRegionList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iRegionList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iRegionList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iRegionList instance at %s>" % (self.this,)
    def GetCount(*args): return _blcelc.iRegionList_GetCount(*args)
    def Get(*args): return _blcelc.iRegionList_Get(*args)
    def Add(*args): return _blcelc.iRegionList_Add(*args)
    def Remove(*args): return _blcelc.iRegionList_Remove(*args)
    def RemoveAll(*args): return _blcelc.iRegionList_RemoveAll(*args)
    def Find(*args): return _blcelc.iRegionList_Find(*args)
    def FindByName(*args): return _blcelc.iRegionList_FindByName(*args)
    def __del__(self, destroy=_blcelc.delete_iRegionList):
        try:
            if self.thisown: destroy(self)
        except: pass

class iRegionListPtr(iRegionList):
    def __init__(self, this):
        _swig_setattr(self, iRegionList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iRegionList, 'thisown', 0)
        _swig_setattr(self, iRegionList,self.__class__,iRegionList)
_blcelc.iRegionList_swigregister(iRegionListPtr)

class iVisibilityObjectIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVisibilityObjectIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVisibilityObjectIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVisibilityObjectIterator instance at %s>" % (self.this,)
    def HasNext(*args): return _blcelc.iVisibilityObjectIterator_HasNext(*args)
    def Next(*args): return _blcelc.iVisibilityObjectIterator_Next(*args)
    def Reset(*args): return _blcelc.iVisibilityObjectIterator_Reset(*args)
    def __del__(self, destroy=_blcelc.delete_iVisibilityObjectIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class iVisibilityObjectIteratorPtr(iVisibilityObjectIterator):
    def __init__(self, this):
        _swig_setattr(self, iVisibilityObjectIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVisibilityObjectIterator, 'thisown', 0)
        _swig_setattr(self, iVisibilityObjectIterator,self.__class__,iVisibilityObjectIterator)
_blcelc.iVisibilityObjectIterator_swigregister(iVisibilityObjectIteratorPtr)

class iVisibilityCullerListener(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVisibilityCullerListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVisibilityCullerListener, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVisibilityCullerListener instance at %s>" % (self.this,)
    def ObjectVisible(*args): return _blcelc.iVisibilityCullerListener_ObjectVisible(*args)
    def __del__(self, destroy=_blcelc.delete_iVisibilityCullerListener):
        try:
            if self.thisown: destroy(self)
        except: pass

class iVisibilityCullerListenerPtr(iVisibilityCullerListener):
    def __init__(self, this):
        _swig_setattr(self, iVisibilityCullerListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVisibilityCullerListener, 'thisown', 0)
        _swig_setattr(self, iVisibilityCullerListener,self.__class__,iVisibilityCullerListener)
_blcelc.iVisibilityCullerListener_swigregister(iVisibilityCullerListenerPtr)

class iVisibilityCuller(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVisibilityCuller, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVisibilityCuller, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVisibilityCuller instance at %s>" % (self.this,)
    def Setup(*args): return _blcelc.iVisibilityCuller_Setup(*args)
    def RegisterVisObject(*args): return _blcelc.iVisibilityCuller_RegisterVisObject(*args)
    def UnregisterVisObject(*args): return _blcelc.iVisibilityCuller_UnregisterVisObject(*args)
    def PrecacheCulling(*args): return _blcelc.iVisibilityCuller_PrecacheCulling(*args)
    def VisTest(*args): return _blcelc.iVisibilityCuller_VisTest(*args)
    def IntersectSegmentSloppy(*args): return _blcelc.iVisibilityCuller_IntersectSegmentSloppy(*args)
    def IntersectSegment(*args): return _blcelc.iVisibilityCuller_IntersectSegment(*args)
    def CastShadows(*args): return _blcelc.iVisibilityCuller_CastShadows(*args)
    def ParseCullerParameters(*args): return _blcelc.iVisibilityCuller_ParseCullerParameters(*args)
    def __del__(self, destroy=_blcelc.delete_iVisibilityCuller):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iVisibilityCuller_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iVisibilityCuller_scfGetVersion)

class iVisibilityCullerPtr(iVisibilityCuller):
    def __init__(self, this):
        _swig_setattr(self, iVisibilityCuller, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVisibilityCuller, 'thisown', 0)
        _swig_setattr(self, iVisibilityCuller,self.__class__,iVisibilityCuller)
_blcelc.iVisibilityCuller_swigregister(iVisibilityCullerPtr)

iVisibilityCuller_scfGetVersion = _blcelc.iVisibilityCuller_scfGetVersion

CS_CULLER_HINT_GOODOCCLUDER = _blcelc.CS_CULLER_HINT_GOODOCCLUDER
CS_CULLER_HINT_BADOCCLUDER = _blcelc.CS_CULLER_HINT_BADOCCLUDER
class iVisibilityObject(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVisibilityObject, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVisibilityObject, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVisibilityObject instance at %s>" % (self.this,)
    def GetMovable(*args): return _blcelc.iVisibilityObject_GetMovable(*args)
    def GetMeshWrapper(*args): return _blcelc.iVisibilityObject_GetMeshWrapper(*args)
    def SetVisibilityNumber(*args): return _blcelc.iVisibilityObject_SetVisibilityNumber(*args)
    def GetVisibilityNumber(*args): return _blcelc.iVisibilityObject_GetVisibilityNumber(*args)
    def GetObjectModel(*args): return _blcelc.iVisibilityObject_GetObjectModel(*args)
    def GetCullerFlags(*args): return _blcelc.iVisibilityObject_GetCullerFlags(*args)
    def __del__(self, destroy=_blcelc.delete_iVisibilityObject):
        try:
            if self.thisown: destroy(self)
        except: pass

class iVisibilityObjectPtr(iVisibilityObject):
    def __init__(self, this):
        _swig_setattr(self, iVisibilityObject, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVisibilityObject, 'thisown', 0)
        _swig_setattr(self, iVisibilityObject,self.__class__,iVisibilityObject)
_blcelc.iVisibilityObject_swigregister(iVisibilityObjectPtr)

CS_PORTAL_CLIPDEST = _blcelc.CS_PORTAL_CLIPDEST
CS_PORTAL_CLIPSTRADDLING = _blcelc.CS_PORTAL_CLIPSTRADDLING
CS_PORTAL_ZFILL = _blcelc.CS_PORTAL_ZFILL
CS_PORTAL_WARP = _blcelc.CS_PORTAL_WARP
CS_PORTAL_MIRROR = _blcelc.CS_PORTAL_MIRROR
CS_PORTAL_STATICDEST = _blcelc.CS_PORTAL_STATICDEST
CS_PORTAL_FLOAT = _blcelc.CS_PORTAL_FLOAT
CS_PORTAL_COLLDET = _blcelc.CS_PORTAL_COLLDET
CS_PORTAL_VISCULL = _blcelc.CS_PORTAL_VISCULL
class iPortalCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPortalCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPortalCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPortalCallback instance at %s>" % (self.this,)
    def Traverse(*args): return _blcelc.iPortalCallback_Traverse(*args)
    def __del__(self, destroy=_blcelc.delete_iPortalCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPortalCallbackPtr(iPortalCallback):
    def __init__(self, this):
        _swig_setattr(self, iPortalCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPortalCallback, 'thisown', 0)
        _swig_setattr(self, iPortalCallback,self.__class__,iPortalCallback)
_blcelc.iPortalCallback_swigregister(iPortalCallbackPtr)

class iPortal(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPortal, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPortal, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPortal instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iPortal_QueryObject(*args)
    def SetName(*args): return _blcelc.iPortal_SetName(*args)
    def GetName(*args): return _blcelc.iPortal_GetName(*args)
    def GetSector(*args): return _blcelc.iPortal_GetSector(*args)
    def GetVertices(*args): return _blcelc.iPortal_GetVertices(*args)
    def GetWorldVertices(*args): return _blcelc.iPortal_GetWorldVertices(*args)
    def GetVertexIndices(*args): return _blcelc.iPortal_GetVertexIndices(*args)
    def GetVertexIndicesCount(*args): return _blcelc.iPortal_GetVertexIndicesCount(*args)
    def GetObjectPlane(*args): return _blcelc.iPortal_GetObjectPlane(*args)
    def GetWorldPlane(*args): return _blcelc.iPortal_GetWorldPlane(*args)
    def ComputeCameraPlane(*args): return _blcelc.iPortal_ComputeCameraPlane(*args)
    def PointOnPolygon(*args): return _blcelc.iPortal_PointOnPolygon(*args)
    def SetSector(*args): return _blcelc.iPortal_SetSector(*args)
    def GetFlags(*args): return _blcelc.iPortal_GetFlags(*args)
    def SetMaximumSectorVisit(*args): return _blcelc.iPortal_SetMaximumSectorVisit(*args)
    def GetMaximumSectorVisit(*args): return _blcelc.iPortal_GetMaximumSectorVisit(*args)
    def SetPortalCallback(*args): return _blcelc.iPortal_SetPortalCallback(*args)
    def RemovePortalCallback(*args): return _blcelc.iPortal_RemovePortalCallback(*args)
    def GetPortalCallbackCount(*args): return _blcelc.iPortal_GetPortalCallbackCount(*args)
    def GetPortalCallback(*args): return _blcelc.iPortal_GetPortalCallback(*args)
    def SetMissingSectorCallback(*args): return _blcelc.iPortal_SetMissingSectorCallback(*args)
    def RemoveMissingSectorCallback(*args): return _blcelc.iPortal_RemoveMissingSectorCallback(*args)
    def GetMissingSectorCallbackCount(*args): return _blcelc.iPortal_GetMissingSectorCallbackCount(*args)
    def GetMissingSectorCallback(*args): return _blcelc.iPortal_GetMissingSectorCallback(*args)
    def GetTextureFilter(*args): return _blcelc.iPortal_GetTextureFilter(*args)
    def SetFilter(*args): return _blcelc.iPortal_SetFilter(*args)
    def GetColorFilter(*args): return _blcelc.iPortal_GetColorFilter(*args)
    def SetWarp(*args): return _blcelc.iPortal_SetWarp(*args)
    def SetMirror(*args): return _blcelc.iPortal_SetMirror(*args)
    def GetWarp(*args): return _blcelc.iPortal_GetWarp(*args)
    def HardTransform(*args): return _blcelc.iPortal_HardTransform(*args)
    def ObjectToWorld(*args): return _blcelc.iPortal_ObjectToWorld(*args)
    def Warp(*args): return _blcelc.iPortal_Warp(*args)
    def WarpSpace(*args): return _blcelc.iPortal_WarpSpace(*args)
    def CompleteSector(*args): return _blcelc.iPortal_CompleteSector(*args)
    def CheckFrustum(*args): return _blcelc.iPortal_CheckFrustum(*args)
    def HitBeamPortals(*args): return _blcelc.iPortal_HitBeamPortals(*args)
    def __del__(self, destroy=_blcelc.delete_iPortal):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iPortal_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iPortal_scfGetVersion)

class iPortalPtr(iPortal):
    def __init__(self, this):
        _swig_setattr(self, iPortal, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPortal, 'thisown', 0)
        _swig_setattr(self, iPortal,self.__class__,iPortal)
_blcelc.iPortal_swigregister(iPortalPtr)

iPortal_scfGetVersion = _blcelc.iPortal_scfGetVersion

class iPortalContainer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPortalContainer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPortalContainer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPortalContainer instance at %s>" % (self.this,)
    def GetPortalCount(*args): return _blcelc.iPortalContainer_GetPortalCount(*args)
    def GetPortal(*args): return _blcelc.iPortalContainer_GetPortal(*args)
    def CreatePortal(*args): return _blcelc.iPortalContainer_CreatePortal(*args)
    def RemovePortal(*args): return _blcelc.iPortalContainer_RemovePortal(*args)
    def Draw(*args): return _blcelc.iPortalContainer_Draw(*args)
    def __del__(self, destroy=_blcelc.delete_iPortalContainer):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iPortalContainer_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iPortalContainer_scfGetVersion)

class iPortalContainerPtr(iPortalContainer):
    def __init__(self, this):
        _swig_setattr(self, iPortalContainer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPortalContainer, 'thisown', 0)
        _swig_setattr(self, iPortalContainer,self.__class__,iPortalContainer)
_blcelc.iPortalContainer_swigregister(iPortalContainerPtr)

iPortalContainer_scfGetVersion = _blcelc.iPortalContainer_scfGetVersion

class iGeneralMeshCommonState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGeneralMeshCommonState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGeneralMeshCommonState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGeneralMeshCommonState instance at %s>" % (self.this,)
    def SetMaterialWrapper(*args): return _blcelc.iGeneralMeshCommonState_SetMaterialWrapper(*args)
    def GetMaterialWrapper(*args): return _blcelc.iGeneralMeshCommonState_GetMaterialWrapper(*args)
    def SetMixMode(*args): return _blcelc.iGeneralMeshCommonState_SetMixMode(*args)
    def GetMixMode(*args): return _blcelc.iGeneralMeshCommonState_GetMixMode(*args)
    def SetLighting(*args): return _blcelc.iGeneralMeshCommonState_SetLighting(*args)
    def IsLighting(*args): return _blcelc.iGeneralMeshCommonState_IsLighting(*args)
    def SetColor(*args): return _blcelc.iGeneralMeshCommonState_SetColor(*args)
    def GetColor(*args): return _blcelc.iGeneralMeshCommonState_GetColor(*args)
    def SetManualColors(*args): return _blcelc.iGeneralMeshCommonState_SetManualColors(*args)
    def IsManualColors(*args): return _blcelc.iGeneralMeshCommonState_IsManualColors(*args)
    def SetShadowCasting(*args): return _blcelc.iGeneralMeshCommonState_SetShadowCasting(*args)
    def IsShadowCasting(*args): return _blcelc.iGeneralMeshCommonState_IsShadowCasting(*args)
    def SetShadowReceiving(*args): return _blcelc.iGeneralMeshCommonState_SetShadowReceiving(*args)
    def IsShadowReceiving(*args): return _blcelc.iGeneralMeshCommonState_IsShadowReceiving(*args)
    def __del__(self, destroy=_blcelc.delete_iGeneralMeshCommonState):
        try:
            if self.thisown: destroy(self)
        except: pass

class iGeneralMeshCommonStatePtr(iGeneralMeshCommonState):
    def __init__(self, this):
        _swig_setattr(self, iGeneralMeshCommonState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGeneralMeshCommonState, 'thisown', 0)
        _swig_setattr(self, iGeneralMeshCommonState,self.__class__,iGeneralMeshCommonState)
_blcelc.iGeneralMeshCommonState_swigregister(iGeneralMeshCommonStatePtr)

class iGeneralMeshState(iGeneralMeshCommonState):
    __swig_setmethods__ = {}
    for _s in [iGeneralMeshCommonState]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGeneralMeshState, name, value)
    __swig_getmethods__ = {}
    for _s in [iGeneralMeshCommonState]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGeneralMeshState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGeneralMeshState instance at %s>" % (self.this,)
    def SetAnimationControl(*args): return _blcelc.iGeneralMeshState_SetAnimationControl(*args)
    def GetAnimationControl(*args): return _blcelc.iGeneralMeshState_GetAnimationControl(*args)
    def __del__(self, destroy=_blcelc.delete_iGeneralMeshState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iGeneralMeshState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iGeneralMeshState_scfGetVersion)

class iGeneralMeshStatePtr(iGeneralMeshState):
    def __init__(self, this):
        _swig_setattr(self, iGeneralMeshState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGeneralMeshState, 'thisown', 0)
        _swig_setattr(self, iGeneralMeshState,self.__class__,iGeneralMeshState)
_blcelc.iGeneralMeshState_swigregister(iGeneralMeshStatePtr)

iGeneralMeshState_scfGetVersion = _blcelc.iGeneralMeshState_scfGetVersion

class iGeneralFactoryState(iGeneralMeshCommonState):
    __swig_setmethods__ = {}
    for _s in [iGeneralMeshCommonState]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGeneralFactoryState, name, value)
    __swig_getmethods__ = {}
    for _s in [iGeneralMeshCommonState]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGeneralFactoryState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGeneralFactoryState instance at %s>" % (self.this,)
    def SetVertexCount(*args): return _blcelc.iGeneralFactoryState_SetVertexCount(*args)
    def GetVertexCount(*args): return _blcelc.iGeneralFactoryState_GetVertexCount(*args)
    def GetVertices(self):
      return CSMutableArrayHelper(self.GetVertexByIndex, self.GetVertexCount)


    def GetTexels(self):
      return CSMutableArrayHelper(self.GetTexelByIndex, self.GetVertexCount)


    def GetNormals(self):
      # iGeneralFactoryState::GetVertices()
      return CSMutableArrayHelper(self.GetNormalByIndex, self.GetVertexCount)


    def SetTriangleCount(*args): return _blcelc.iGeneralFactoryState_SetTriangleCount(*args)
    def GetTriangleCount(*args): return _blcelc.iGeneralFactoryState_GetTriangleCount(*args)
    def GetTriangles(self):
      return CSMutableArrayHelper(self.GetTriangleByIndex, self.GetTriangleCount)


    def GetColors(self):
      return CSMutableArrayHelper(self.GetNormalByIndex, self.GetVertexCount)


    def Invalidate(*args): return _blcelc.iGeneralFactoryState_Invalidate(*args)
    def CalculateNormals(*args): return _blcelc.iGeneralFactoryState_CalculateNormals(*args)
    def GenerateBox(*args): return _blcelc.iGeneralFactoryState_GenerateBox(*args)
    def SetBack2Front(*args): return _blcelc.iGeneralFactoryState_SetBack2Front(*args)
    def IsAutoNormals(*args): return _blcelc.iGeneralFactoryState_IsAutoNormals(*args)
    def IsBack2Front(*args): return _blcelc.iGeneralFactoryState_IsBack2Front(*args)
    def SetAnimationControlFactory(*args): return _blcelc.iGeneralFactoryState_SetAnimationControlFactory(*args)
    def GetAnimationControlFactory(*args): return _blcelc.iGeneralFactoryState_GetAnimationControlFactory(*args)
    def AddRenderBuffer(*args): return _blcelc.iGeneralFactoryState_AddRenderBuffer(*args)
    def SetRenderBufferComponent(*args): return _blcelc.iGeneralFactoryState_SetRenderBufferComponent(*args)
    def SetRenderBuffer(*args): return _blcelc.iGeneralFactoryState_SetRenderBuffer(*args)
    def __del__(self, destroy=_blcelc.delete_iGeneralFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iGeneralFactoryState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iGeneralFactoryState_scfGetVersion)
    def GetVertexByIndex(*args): return _blcelc.iGeneralFactoryState_GetVertexByIndex(*args)
    def GetTexelByIndex(*args): return _blcelc.iGeneralFactoryState_GetTexelByIndex(*args)
    def GetNormalByIndex(*args): return _blcelc.iGeneralFactoryState_GetNormalByIndex(*args)
    def GetTriangleByIndex(*args): return _blcelc.iGeneralFactoryState_GetTriangleByIndex(*args)
    def GetColorByIndex(*args): return _blcelc.iGeneralFactoryState_GetColorByIndex(*args)

class iGeneralFactoryStatePtr(iGeneralFactoryState):
    def __init__(self, this):
        _swig_setattr(self, iGeneralFactoryState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGeneralFactoryState, 'thisown', 0)
        _swig_setattr(self, iGeneralFactoryState,self.__class__,iGeneralFactoryState)
_blcelc.iGeneralFactoryState_swigregister(iGeneralFactoryStatePtr)

iGeneralFactoryState_scfGetVersion = _blcelc.iGeneralFactoryState_scfGetVersion

class iGenMeshAnimationControl(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGenMeshAnimationControl, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGenMeshAnimationControl, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGenMeshAnimationControl instance at %s>" % (self.this,)
    def AnimatesVertices(*args): return _blcelc.iGenMeshAnimationControl_AnimatesVertices(*args)
    def AnimatesTexels(*args): return _blcelc.iGenMeshAnimationControl_AnimatesTexels(*args)
    def AnimatesNormals(*args): return _blcelc.iGenMeshAnimationControl_AnimatesNormals(*args)
    def AnimatesColors(*args): return _blcelc.iGenMeshAnimationControl_AnimatesColors(*args)
    def UpdateVertices(*args): return _blcelc.iGenMeshAnimationControl_UpdateVertices(*args)
    def UpdateTexels(*args): return _blcelc.iGenMeshAnimationControl_UpdateTexels(*args)
    def UpdateNormals(*args): return _blcelc.iGenMeshAnimationControl_UpdateNormals(*args)
    def UpdateColors(*args): return _blcelc.iGenMeshAnimationControl_UpdateColors(*args)
    def __del__(self, destroy=_blcelc.delete_iGenMeshAnimationControl):
        try:
            if self.thisown: destroy(self)
        except: pass

class iGenMeshAnimationControlPtr(iGenMeshAnimationControl):
    def __init__(self, this):
        _swig_setattr(self, iGenMeshAnimationControl, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGenMeshAnimationControl, 'thisown', 0)
        _swig_setattr(self, iGenMeshAnimationControl,self.__class__,iGenMeshAnimationControl)
_blcelc.iGenMeshAnimationControl_swigregister(iGenMeshAnimationControlPtr)

class iGenMeshAnimationControlFactory(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGenMeshAnimationControlFactory, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGenMeshAnimationControlFactory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGenMeshAnimationControlFactory instance at %s>" % (self.this,)
    def CreateAnimationControl(*args): return _blcelc.iGenMeshAnimationControlFactory_CreateAnimationControl(*args)
    def Load(*args): return _blcelc.iGenMeshAnimationControlFactory_Load(*args)
    def Save(*args): return _blcelc.iGenMeshAnimationControlFactory_Save(*args)
    def __del__(self, destroy=_blcelc.delete_iGenMeshAnimationControlFactory):
        try:
            if self.thisown: destroy(self)
        except: pass

class iGenMeshAnimationControlFactoryPtr(iGenMeshAnimationControlFactory):
    def __init__(self, this):
        _swig_setattr(self, iGenMeshAnimationControlFactory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGenMeshAnimationControlFactory, 'thisown', 0)
        _swig_setattr(self, iGenMeshAnimationControlFactory,self.__class__,iGenMeshAnimationControlFactory)
_blcelc.iGenMeshAnimationControlFactory_swigregister(iGenMeshAnimationControlFactoryPtr)

class iGenMeshAnimationControlType(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGenMeshAnimationControlType, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGenMeshAnimationControlType, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGenMeshAnimationControlType instance at %s>" % (self.this,)
    def CreateAnimationControlFactory(*args): return _blcelc.iGenMeshAnimationControlType_CreateAnimationControlFactory(*args)
    def __del__(self, destroy=_blcelc.delete_iGenMeshAnimationControlType):
        try:
            if self.thisown: destroy(self)
        except: pass

class iGenMeshAnimationControlTypePtr(iGenMeshAnimationControlType):
    def __init__(self, this):
        _swig_setattr(self, iGenMeshAnimationControlType, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGenMeshAnimationControlType, 'thisown', 0)
        _swig_setattr(self, iGenMeshAnimationControlType,self.__class__,iGenMeshAnimationControlType)
_blcelc.iGenMeshAnimationControlType_swigregister(iGenMeshAnimationControlTypePtr)

class csSprite2DVertex(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSprite2DVertex, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSprite2DVertex, name)
    def __repr__(self):
        return "<C csSprite2DVertex instance at %s>" % (self.this,)
    __swig_setmethods__["pos"] = _blcelc.csSprite2DVertex_pos_set
    __swig_getmethods__["pos"] = _blcelc.csSprite2DVertex_pos_get
    if _newclass:pos = property(_blcelc.csSprite2DVertex_pos_get, _blcelc.csSprite2DVertex_pos_set)
    __swig_setmethods__["color_init"] = _blcelc.csSprite2DVertex_color_init_set
    __swig_getmethods__["color_init"] = _blcelc.csSprite2DVertex_color_init_get
    if _newclass:color_init = property(_blcelc.csSprite2DVertex_color_init_get, _blcelc.csSprite2DVertex_color_init_set)
    __swig_setmethods__["color"] = _blcelc.csSprite2DVertex_color_set
    __swig_getmethods__["color"] = _blcelc.csSprite2DVertex_color_get
    if _newclass:color = property(_blcelc.csSprite2DVertex_color_get, _blcelc.csSprite2DVertex_color_set)
    __swig_setmethods__["u"] = _blcelc.csSprite2DVertex_u_set
    __swig_getmethods__["u"] = _blcelc.csSprite2DVertex_u_get
    if _newclass:u = property(_blcelc.csSprite2DVertex_u_get, _blcelc.csSprite2DVertex_u_set)
    __swig_setmethods__["v"] = _blcelc.csSprite2DVertex_v_set
    __swig_getmethods__["v"] = _blcelc.csSprite2DVertex_v_get
    if _newclass:v = property(_blcelc.csSprite2DVertex_v_get, _blcelc.csSprite2DVertex_v_set)
    def __init__(self, *args):
        _swig_setattr(self, csSprite2DVertex, 'this', _blcelc.new_csSprite2DVertex(*args))
        _swig_setattr(self, csSprite2DVertex, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csSprite2DVertex):
        try:
            if self.thisown: destroy(self)
        except: pass

class csSprite2DVertexPtr(csSprite2DVertex):
    def __init__(self, this):
        _swig_setattr(self, csSprite2DVertex, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csSprite2DVertex, 'thisown', 0)
        _swig_setattr(self, csSprite2DVertex,self.__class__,csSprite2DVertex)
_blcelc.csSprite2DVertex_swigregister(csSprite2DVertexPtr)

class iSprite2DUVAnimationFrame(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSprite2DUVAnimationFrame, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSprite2DUVAnimationFrame, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite2DUVAnimationFrame instance at %s>" % (self.this,)
    def SetName(*args): return _blcelc.iSprite2DUVAnimationFrame_SetName(*args)
    def GetName(*args): return _blcelc.iSprite2DUVAnimationFrame_GetName(*args)
    def GetUVCoo(*args): return _blcelc.iSprite2DUVAnimationFrame_GetUVCoo(*args)
    def GetUVCount(*args): return _blcelc.iSprite2DUVAnimationFrame_GetUVCount(*args)
    def SetUV(*args): return _blcelc.iSprite2DUVAnimationFrame_SetUV(*args)
    def SetFrameData(*args): return _blcelc.iSprite2DUVAnimationFrame_SetFrameData(*args)
    def RemoveUV(*args): return _blcelc.iSprite2DUVAnimationFrame_RemoveUV(*args)
    def GetDuration(*args): return _blcelc.iSprite2DUVAnimationFrame_GetDuration(*args)
    def SetDuration(*args): return _blcelc.iSprite2DUVAnimationFrame_SetDuration(*args)
    def __del__(self, destroy=_blcelc.delete_iSprite2DUVAnimationFrame):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSprite2DUVAnimationFramePtr(iSprite2DUVAnimationFrame):
    def __init__(self, this):
        _swig_setattr(self, iSprite2DUVAnimationFrame, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSprite2DUVAnimationFrame, 'thisown', 0)
        _swig_setattr(self, iSprite2DUVAnimationFrame,self.__class__,iSprite2DUVAnimationFrame)
_blcelc.iSprite2DUVAnimationFrame_swigregister(iSprite2DUVAnimationFramePtr)

class iSprite2DUVAnimation(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSprite2DUVAnimation, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSprite2DUVAnimation, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite2DUVAnimation instance at %s>" % (self.this,)
    def SetName(*args): return _blcelc.iSprite2DUVAnimation_SetName(*args)
    def GetName(*args): return _blcelc.iSprite2DUVAnimation_GetName(*args)
    def GetFrameCount(*args): return _blcelc.iSprite2DUVAnimation_GetFrameCount(*args)
    def GetFrame(*args): return _blcelc.iSprite2DUVAnimation_GetFrame(*args)
    def CreateFrame(*args): return _blcelc.iSprite2DUVAnimation_CreateFrame(*args)
    def MoveFrame(*args): return _blcelc.iSprite2DUVAnimation_MoveFrame(*args)
    def RemoveFrame(*args): return _blcelc.iSprite2DUVAnimation_RemoveFrame(*args)
    def __del__(self, destroy=_blcelc.delete_iSprite2DUVAnimation):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSprite2DUVAnimationPtr(iSprite2DUVAnimation):
    def __init__(self, this):
        _swig_setattr(self, iSprite2DUVAnimation, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSprite2DUVAnimation, 'thisown', 0)
        _swig_setattr(self, iSprite2DUVAnimation,self.__class__,iSprite2DUVAnimation)
_blcelc.iSprite2DUVAnimation_swigregister(iSprite2DUVAnimationPtr)

class iSprite2DFactoryState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSprite2DFactoryState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSprite2DFactoryState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite2DFactoryState instance at %s>" % (self.this,)
    def SetMaterialWrapper(*args): return _blcelc.iSprite2DFactoryState_SetMaterialWrapper(*args)
    def GetMaterialWrapper(*args): return _blcelc.iSprite2DFactoryState_GetMaterialWrapper(*args)
    def SetMixMode(*args): return _blcelc.iSprite2DFactoryState_SetMixMode(*args)
    def GetMixMode(*args): return _blcelc.iSprite2DFactoryState_GetMixMode(*args)
    def SetLighting(*args): return _blcelc.iSprite2DFactoryState_SetLighting(*args)
    def HasLighting(*args): return _blcelc.iSprite2DFactoryState_HasLighting(*args)
    def GetUVAnimationCount(*args): return _blcelc.iSprite2DFactoryState_GetUVAnimationCount(*args)
    def CreateUVAnimation(*args): return _blcelc.iSprite2DFactoryState_CreateUVAnimation(*args)
    def RemoveUVAnimation(*args): return _blcelc.iSprite2DFactoryState_RemoveUVAnimation(*args)
    def GetUVAnimation(*args): return _blcelc.iSprite2DFactoryState_GetUVAnimation(*args)
    def __del__(self, destroy=_blcelc.delete_iSprite2DFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSprite2DFactoryStatePtr(iSprite2DFactoryState):
    def __init__(self, this):
        _swig_setattr(self, iSprite2DFactoryState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSprite2DFactoryState, 'thisown', 0)
        _swig_setattr(self, iSprite2DFactoryState,self.__class__,iSprite2DFactoryState)
_blcelc.iSprite2DFactoryState_swigregister(iSprite2DFactoryStatePtr)

class iSprite2DState(iSprite2DFactoryState):
    __swig_setmethods__ = {}
    for _s in [iSprite2DFactoryState]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSprite2DState, name, value)
    __swig_getmethods__ = {}
    for _s in [iSprite2DFactoryState]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSprite2DState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite2DState instance at %s>" % (self.this,)
    def CreateRegularVertices(*args): return _blcelc.iSprite2DState_CreateRegularVertices(*args)
    def SetUVAnimation(*args): return _blcelc.iSprite2DState_SetUVAnimation(*args)
    def GetUVAnimation(*args): return _blcelc.iSprite2DState_GetUVAnimation(*args)
    def StopUVAnimation(*args): return _blcelc.iSprite2DState_StopUVAnimation(*args)
    def PlayUVAnimation(*args): return _blcelc.iSprite2DState_PlayUVAnimation(*args)
    def __del__(self, destroy=_blcelc.delete_iSprite2DState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSprite2DState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSprite2DState_scfGetVersion)
    def GetVertexByIndex(*args): return _blcelc.iSprite2DState_GetVertexByIndex(*args)
    def GetVertexCount(*args): return _blcelc.iSprite2DState_GetVertexCount(*args)

class iSprite2DStatePtr(iSprite2DState):
    def __init__(self, this):
        _swig_setattr(self, iSprite2DState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSprite2DState, 'thisown', 0)
        _swig_setattr(self, iSprite2DState,self.__class__,iSprite2DState)
_blcelc.iSprite2DState_swigregister(iSprite2DStatePtr)

iSprite2DState_scfGetVersion = _blcelc.iSprite2DState_scfGetVersion

CS_SPR_LIGHTING_HQ = _blcelc.CS_SPR_LIGHTING_HQ
CS_SPR_LIGHTING_LQ = _blcelc.CS_SPR_LIGHTING_LQ
CS_SPR_LIGHTING_FAST = _blcelc.CS_SPR_LIGHTING_FAST
CS_SPR_LIGHTING_RANDOM = _blcelc.CS_SPR_LIGHTING_RANDOM
CS_SPR_LIGHT_GLOBAL = _blcelc.CS_SPR_LIGHT_GLOBAL
CS_SPR_LIGHT_TEMPLATE = _blcelc.CS_SPR_LIGHT_TEMPLATE
CS_SPR_LIGHT_LOCAL = _blcelc.CS_SPR_LIGHT_LOCAL
CS_SPR_LOD_GLOBAL = _blcelc.CS_SPR_LOD_GLOBAL
CS_SPR_LOD_TEMPLATE = _blcelc.CS_SPR_LOD_TEMPLATE
CS_SPR_LOD_LOCAL = _blcelc.CS_SPR_LOD_LOCAL
class iSpriteFrame(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSpriteFrame, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSpriteFrame, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteFrame instance at %s>" % (self.this,)
    def SetName(*args): return _blcelc.iSpriteFrame_SetName(*args)
    def GetName(*args): return _blcelc.iSpriteFrame_GetName(*args)
    def GetAnmIndex(*args): return _blcelc.iSpriteFrame_GetAnmIndex(*args)
    def GetTexIndex(*args): return _blcelc.iSpriteFrame_GetTexIndex(*args)
    def __del__(self, destroy=_blcelc.delete_iSpriteFrame):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSpriteFramePtr(iSpriteFrame):
    def __init__(self, this):
        _swig_setattr(self, iSpriteFrame, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSpriteFrame, 'thisown', 0)
        _swig_setattr(self, iSpriteFrame,self.__class__,iSpriteFrame)
_blcelc.iSpriteFrame_swigregister(iSpriteFramePtr)

class iSpriteAction(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSpriteAction, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSpriteAction, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteAction instance at %s>" % (self.this,)
    def SetName(*args): return _blcelc.iSpriteAction_SetName(*args)
    def GetName(*args): return _blcelc.iSpriteAction_GetName(*args)
    def GetFrameCount(*args): return _blcelc.iSpriteAction_GetFrameCount(*args)
    def GetFrame(*args): return _blcelc.iSpriteAction_GetFrame(*args)
    def GetNextFrame(*args): return _blcelc.iSpriteAction_GetNextFrame(*args)
    def GetFrameDelay(*args): return _blcelc.iSpriteAction_GetFrameDelay(*args)
    def GetFrameDisplacement(*args): return _blcelc.iSpriteAction_GetFrameDisplacement(*args)
    def AddFrame(*args): return _blcelc.iSpriteAction_AddFrame(*args)
    def __del__(self, destroy=_blcelc.delete_iSpriteAction):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSpriteActionPtr(iSpriteAction):
    def __init__(self, this):
        _swig_setattr(self, iSpriteAction, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSpriteAction, 'thisown', 0)
        _swig_setattr(self, iSpriteAction,self.__class__,iSpriteAction)
_blcelc.iSpriteAction_swigregister(iSpriteActionPtr)

class iSpriteSocket(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSpriteSocket, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSpriteSocket, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteSocket instance at %s>" % (self.this,)
    def SetName(*args): return _blcelc.iSpriteSocket_SetName(*args)
    def GetName(*args): return _blcelc.iSpriteSocket_GetName(*args)
    def SetMeshWrapper(*args): return _blcelc.iSpriteSocket_SetMeshWrapper(*args)
    def GetMeshWrapper(*args): return _blcelc.iSpriteSocket_GetMeshWrapper(*args)
    def SetTriangleIndex(*args): return _blcelc.iSpriteSocket_SetTriangleIndex(*args)
    def GetTriangleIndex(*args): return _blcelc.iSpriteSocket_GetTriangleIndex(*args)
    def __del__(self, destroy=_blcelc.delete_iSpriteSocket):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSpriteSocketPtr(iSpriteSocket):
    def __init__(self, this):
        _swig_setattr(self, iSpriteSocket, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSpriteSocket, 'thisown', 0)
        _swig_setattr(self, iSpriteSocket,self.__class__,iSpriteSocket)
_blcelc.iSpriteSocket_swigregister(iSpriteSocketPtr)

class iSprite3DFactoryState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSprite3DFactoryState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSprite3DFactoryState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite3DFactoryState instance at %s>" % (self.this,)
    def SetMaterialWrapper(*args): return _blcelc.iSprite3DFactoryState_SetMaterialWrapper(*args)
    def GetMaterialWrapper(*args): return _blcelc.iSprite3DFactoryState_GetMaterialWrapper(*args)
    def AddVertices(*args): return _blcelc.iSprite3DFactoryState_AddVertices(*args)
    def GetVertexCount(*args): return _blcelc.iSprite3DFactoryState_GetVertexCount(*args)
    def GetVertex(*args): return _blcelc.iSprite3DFactoryState_GetVertex(*args)
    def SetVertex(*args): return _blcelc.iSprite3DFactoryState_SetVertex(*args)
    def GetVertices(*args): return _blcelc.iSprite3DFactoryState_GetVertices(*args)
    def SetVertices(*args): return _blcelc.iSprite3DFactoryState_SetVertices(*args)
    def GetTexel(*args): return _blcelc.iSprite3DFactoryState_GetTexel(*args)
    def SetTexel(*args): return _blcelc.iSprite3DFactoryState_SetTexel(*args)
    def GetTexels(*args): return _blcelc.iSprite3DFactoryState_GetTexels(*args)
    def SetTexels(*args): return _blcelc.iSprite3DFactoryState_SetTexels(*args)
    def GetNormal(*args): return _blcelc.iSprite3DFactoryState_GetNormal(*args)
    def SetNormal(*args): return _blcelc.iSprite3DFactoryState_SetNormal(*args)
    def GetNormals(*args): return _blcelc.iSprite3DFactoryState_GetNormals(*args)
    def SetNormals(*args): return _blcelc.iSprite3DFactoryState_SetNormals(*args)
    def AddTriangle(*args): return _blcelc.iSprite3DFactoryState_AddTriangle(*args)
    def GetTriangle(*args): return _blcelc.iSprite3DFactoryState_GetTriangle(*args)
    def GetTriangles(*args): return _blcelc.iSprite3DFactoryState_GetTriangles(*args)
    def GetTriangleCount(*args): return _blcelc.iSprite3DFactoryState_GetTriangleCount(*args)
    def SetTriangleCount(*args): return _blcelc.iSprite3DFactoryState_SetTriangleCount(*args)
    def SetTriangles(*args): return _blcelc.iSprite3DFactoryState_SetTriangles(*args)
    def AddFrame(*args): return _blcelc.iSprite3DFactoryState_AddFrame(*args)
    def FindFrame(*args): return _blcelc.iSprite3DFactoryState_FindFrame(*args)
    def GetFrameCount(*args): return _blcelc.iSprite3DFactoryState_GetFrameCount(*args)
    def GetFrame(*args): return _blcelc.iSprite3DFactoryState_GetFrame(*args)
    def AddAction(*args): return _blcelc.iSprite3DFactoryState_AddAction(*args)
    def FindAction(*args): return _blcelc.iSprite3DFactoryState_FindAction(*args)
    def GetFirstAction(*args): return _blcelc.iSprite3DFactoryState_GetFirstAction(*args)
    def GetActionCount(*args): return _blcelc.iSprite3DFactoryState_GetActionCount(*args)
    def GetAction(*args): return _blcelc.iSprite3DFactoryState_GetAction(*args)
    def AddSocket(*args): return _blcelc.iSprite3DFactoryState_AddSocket(*args)
    def FindSocket(*args): return _blcelc.iSprite3DFactoryState_FindSocket(*args)
    def GetSocketCount(*args): return _blcelc.iSprite3DFactoryState_GetSocketCount(*args)
    def GetSocket(*args): return _blcelc.iSprite3DFactoryState_GetSocket(*args)
    def EnableTweening(*args): return _blcelc.iSprite3DFactoryState_EnableTweening(*args)
    def IsTweeningEnabled(*args): return _blcelc.iSprite3DFactoryState_IsTweeningEnabled(*args)
    def SetLightingQuality(*args): return _blcelc.iSprite3DFactoryState_SetLightingQuality(*args)
    def GetLightingQuality(*args): return _blcelc.iSprite3DFactoryState_GetLightingQuality(*args)
    def SetLightingQualityConfig(*args): return _blcelc.iSprite3DFactoryState_SetLightingQualityConfig(*args)
    def GetLightingQualityConfig(*args): return _blcelc.iSprite3DFactoryState_GetLightingQualityConfig(*args)
    def SetLodLevelConfig(*args): return _blcelc.iSprite3DFactoryState_SetLodLevelConfig(*args)
    def GetLodLevelConfig(*args): return _blcelc.iSprite3DFactoryState_GetLodLevelConfig(*args)
    def MergeNormals(*args): return _blcelc.iSprite3DFactoryState_MergeNormals(*args)
    def SetMixMode(*args): return _blcelc.iSprite3DFactoryState_SetMixMode(*args)
    def GetMixMode(*args): return _blcelc.iSprite3DFactoryState_GetMixMode(*args)
    def __del__(self, destroy=_blcelc.delete_iSprite3DFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSprite3DFactoryStatePtr(iSprite3DFactoryState):
    def __init__(self, this):
        _swig_setattr(self, iSprite3DFactoryState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSprite3DFactoryState, 'thisown', 0)
        _swig_setattr(self, iSprite3DFactoryState,self.__class__,iSprite3DFactoryState)
_blcelc.iSprite3DFactoryState_swigregister(iSprite3DFactoryStatePtr)

class iSprite3DState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSprite3DState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSprite3DState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite3DState instance at %s>" % (self.this,)
    def SetMaterialWrapper(*args): return _blcelc.iSprite3DState_SetMaterialWrapper(*args)
    def GetMaterialWrapper(*args): return _blcelc.iSprite3DState_GetMaterialWrapper(*args)
    def SetMixMode(*args): return _blcelc.iSprite3DState_SetMixMode(*args)
    def GetMixMode(*args): return _blcelc.iSprite3DState_GetMixMode(*args)
    def SetLighting(*args): return _blcelc.iSprite3DState_SetLighting(*args)
    def IsLighting(*args): return _blcelc.iSprite3DState_IsLighting(*args)
    def SetFrame(*args): return _blcelc.iSprite3DState_SetFrame(*args)
    def GetCurFrame(*args): return _blcelc.iSprite3DState_GetCurFrame(*args)
    def GetFrameCount(*args): return _blcelc.iSprite3DState_GetFrameCount(*args)
    def SetAction(*args): return _blcelc.iSprite3DState_SetAction(*args)
    def SetReverseAction(*args): return _blcelc.iSprite3DState_SetReverseAction(*args)
    def SetSingleStepAction(*args): return _blcelc.iSprite3DState_SetSingleStepAction(*args)
    def SetOverrideAction(*args): return _blcelc.iSprite3DState_SetOverrideAction(*args)
    def PropagateAction(*args): return _blcelc.iSprite3DState_PropagateAction(*args)
    def GetCurAction(*args): return _blcelc.iSprite3DState_GetCurAction(*args)
    def GetReverseAction(*args): return _blcelc.iSprite3DState_GetReverseAction(*args)
    def EnableTweening(*args): return _blcelc.iSprite3DState_EnableTweening(*args)
    def IsTweeningEnabled(*args): return _blcelc.iSprite3DState_IsTweeningEnabled(*args)
    def UnsetTexture(*args): return _blcelc.iSprite3DState_UnsetTexture(*args)
    def GetLightingQuality(*args): return _blcelc.iSprite3DState_GetLightingQuality(*args)
    def SetLocalLightingQuality(*args): return _blcelc.iSprite3DState_SetLocalLightingQuality(*args)
    def SetLightingQualityConfig(*args): return _blcelc.iSprite3DState_SetLightingQualityConfig(*args)
    def GetLightingQualityConfig(*args): return _blcelc.iSprite3DState_GetLightingQualityConfig(*args)
    def SetLodLevelConfig(*args): return _blcelc.iSprite3DState_SetLodLevelConfig(*args)
    def GetLodLevelConfig(*args): return _blcelc.iSprite3DState_GetLodLevelConfig(*args)
    def IsLodEnabled(*args): return _blcelc.iSprite3DState_IsLodEnabled(*args)
    def SetBaseColor(*args): return _blcelc.iSprite3DState_SetBaseColor(*args)
    def GetBaseColor(*args): return _blcelc.iSprite3DState_GetBaseColor(*args)
    def FindSocket(*args): return _blcelc.iSprite3DState_FindSocket(*args)
    def __del__(self, destroy=_blcelc.delete_iSprite3DState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSprite3DState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSprite3DState_scfGetVersion)

class iSprite3DStatePtr(iSprite3DState):
    def __init__(self, this):
        _swig_setattr(self, iSprite3DState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSprite3DState, 'thisown', 0)
        _swig_setattr(self, iSprite3DState,self.__class__,iSprite3DState)
_blcelc.iSprite3DState_swigregister(iSprite3DStatePtr)

iSprite3DState_scfGetVersion = _blcelc.iSprite3DState_scfGetVersion

class iSpriteCal3DSocket(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSpriteCal3DSocket, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSpriteCal3DSocket, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteCal3DSocket instance at %s>" % (self.this,)
    def SetName(*args): return _blcelc.iSpriteCal3DSocket_SetName(*args)
    def GetName(*args): return _blcelc.iSpriteCal3DSocket_GetName(*args)
    def SetMeshWrapper(*args): return _blcelc.iSpriteCal3DSocket_SetMeshWrapper(*args)
    def GetMeshWrapper(*args): return _blcelc.iSpriteCal3DSocket_GetMeshWrapper(*args)
    def SetTriangleIndex(*args): return _blcelc.iSpriteCal3DSocket_SetTriangleIndex(*args)
    def GetTriangleIndex(*args): return _blcelc.iSpriteCal3DSocket_GetTriangleIndex(*args)
    def SetSubmeshIndex(*args): return _blcelc.iSpriteCal3DSocket_SetSubmeshIndex(*args)
    def GetSubmeshIndex(*args): return _blcelc.iSpriteCal3DSocket_GetSubmeshIndex(*args)
    def SetMeshIndex(*args): return _blcelc.iSpriteCal3DSocket_SetMeshIndex(*args)
    def GetMeshIndex(*args): return _blcelc.iSpriteCal3DSocket_GetMeshIndex(*args)
    def SetTransform(*args): return _blcelc.iSpriteCal3DSocket_SetTransform(*args)
    def GetTransform(*args): return _blcelc.iSpriteCal3DSocket_GetTransform(*args)
    def GetSecondaryCount(*args): return _blcelc.iSpriteCal3DSocket_GetSecondaryCount(*args)
    def GetSecondaryMesh(*args): return _blcelc.iSpriteCal3DSocket_GetSecondaryMesh(*args)
    def GetSecondaryTransform(*args): return _blcelc.iSpriteCal3DSocket_GetSecondaryTransform(*args)
    def SetSecondaryTransform(*args): return _blcelc.iSpriteCal3DSocket_SetSecondaryTransform(*args)
    def AttachSecondary(*args): return _blcelc.iSpriteCal3DSocket_AttachSecondary(*args)
    def DetachSecondary(*args): return _blcelc.iSpriteCal3DSocket_DetachSecondary(*args)
    def FindSecondary(*args): return _blcelc.iSpriteCal3DSocket_FindSecondary(*args)
    def __del__(self, destroy=_blcelc.delete_iSpriteCal3DSocket):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSpriteCal3DSocketPtr(iSpriteCal3DSocket):
    def __init__(self, this):
        _swig_setattr(self, iSpriteCal3DSocket, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSpriteCal3DSocket, 'thisown', 0)
        _swig_setattr(self, iSpriteCal3DSocket,self.__class__,iSpriteCal3DSocket)
_blcelc.iSpriteCal3DSocket_swigregister(iSpriteCal3DSocketPtr)

class iSpriteCal3DFactoryState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSpriteCal3DFactoryState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSpriteCal3DFactoryState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteCal3DFactoryState instance at %s>" % (self.this,)
    def Create(*args): return _blcelc.iSpriteCal3DFactoryState_Create(*args)
    def ReportLastError(*args): return _blcelc.iSpriteCal3DFactoryState_ReportLastError(*args)
    def SetLoadFlags(*args): return _blcelc.iSpriteCal3DFactoryState_SetLoadFlags(*args)
    def SetBasePath(*args): return _blcelc.iSpriteCal3DFactoryState_SetBasePath(*args)
    def LoadCoreSkeleton(*args): return _blcelc.iSpriteCal3DFactoryState_LoadCoreSkeleton(*args)
    def RescaleFactory(*args): return _blcelc.iSpriteCal3DFactoryState_RescaleFactory(*args)
    def LoadCoreAnimation(*args): return _blcelc.iSpriteCal3DFactoryState_LoadCoreAnimation(*args)
    def LoadCoreMesh(*args): return _blcelc.iSpriteCal3DFactoryState_LoadCoreMesh(*args)
    def LoadCoreMorphTarget(*args): return _blcelc.iSpriteCal3DFactoryState_LoadCoreMorphTarget(*args)
    def AddMorphAnimation(*args): return _blcelc.iSpriteCal3DFactoryState_AddMorphAnimation(*args)
    def AddMorphTarget(*args): return _blcelc.iSpriteCal3DFactoryState_AddMorphTarget(*args)
    def AddCoreMaterial(*args): return _blcelc.iSpriteCal3DFactoryState_AddCoreMaterial(*args)
    def CalculateAllBoneBoundingBoxes(*args): return _blcelc.iSpriteCal3DFactoryState_CalculateAllBoneBoundingBoxes(*args)
    def BindMaterials(*args): return _blcelc.iSpriteCal3DFactoryState_BindMaterials(*args)
    def GetMeshCount(*args): return _blcelc.iSpriteCal3DFactoryState_GetMeshCount(*args)
    def GetMorphAnimationCount(*args): return _blcelc.iSpriteCal3DFactoryState_GetMorphAnimationCount(*args)
    def GetMorphTargetCount(*args): return _blcelc.iSpriteCal3DFactoryState_GetMorphTargetCount(*args)
    def GetMeshName(*args): return _blcelc.iSpriteCal3DFactoryState_GetMeshName(*args)
    def FindMeshName(*args): return _blcelc.iSpriteCal3DFactoryState_FindMeshName(*args)
    def GetDefaultMaterial(*args): return _blcelc.iSpriteCal3DFactoryState_GetDefaultMaterial(*args)
    def GetMorphAnimationName(*args): return _blcelc.iSpriteCal3DFactoryState_GetMorphAnimationName(*args)
    def FindMorphAnimationName(*args): return _blcelc.iSpriteCal3DFactoryState_FindMorphAnimationName(*args)
    def IsMeshDefault(*args): return _blcelc.iSpriteCal3DFactoryState_IsMeshDefault(*args)
    def AddSocket(*args): return _blcelc.iSpriteCal3DFactoryState_AddSocket(*args)
    def FindSocket(*args): return _blcelc.iSpriteCal3DFactoryState_FindSocket(*args)
    def GetSocketCount(*args): return _blcelc.iSpriteCal3DFactoryState_GetSocketCount(*args)
    def GetSocket(*args): return _blcelc.iSpriteCal3DFactoryState_GetSocket(*args)
    def GetCal3DCoreModel(*args): return _blcelc.iSpriteCal3DFactoryState_GetCal3DCoreModel(*args)
    def RegisterAnimCallback(*args): return _blcelc.iSpriteCal3DFactoryState_RegisterAnimCallback(*args)
    def RemoveAnimCallback(*args): return _blcelc.iSpriteCal3DFactoryState_RemoveAnimCallback(*args)
    def __del__(self, destroy=_blcelc.delete_iSpriteCal3DFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSpriteCal3DFactoryStatePtr(iSpriteCal3DFactoryState):
    def __init__(self, this):
        _swig_setattr(self, iSpriteCal3DFactoryState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSpriteCal3DFactoryState, 'thisown', 0)
        _swig_setattr(self, iSpriteCal3DFactoryState,self.__class__,iSpriteCal3DFactoryState)
_blcelc.iSpriteCal3DFactoryState_swigregister(iSpriteCal3DFactoryStatePtr)

class iSpriteCal3DState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSpriteCal3DState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSpriteCal3DState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteCal3DState instance at %s>" % (self.this,)
    C3D_ANIM_TYPE_NONE = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_NONE
    C3D_ANIM_TYPE_IDLE = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_IDLE
    C3D_ANIM_TYPE_TRAVEL = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_TRAVEL
    C3D_ANIM_TYPE_CYCLE = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_CYCLE
    C3D_ANIM_TYPE_STYLE_CYCLE = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_STYLE_CYCLE
    C3D_ANIM_TYPE_ACTION = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_ACTION
    def GetAnimCount(*args): return _blcelc.iSpriteCal3DState_GetAnimCount(*args)
    def GetAnimName(*args): return _blcelc.iSpriteCal3DState_GetAnimName(*args)
    def GetAnimType(*args): return _blcelc.iSpriteCal3DState_GetAnimType(*args)
    def FindAnim(*args): return _blcelc.iSpriteCal3DState_FindAnim(*args)
    def ClearAllAnims(*args): return _blcelc.iSpriteCal3DState_ClearAllAnims(*args)
    def SetAnimCycle(*args): return _blcelc.iSpriteCal3DState_SetAnimCycle(*args)
    def AddAnimCycle(*args): return _blcelc.iSpriteCal3DState_AddAnimCycle(*args)
    def ClearAnimCycle(*args): return _blcelc.iSpriteCal3DState_ClearAnimCycle(*args)
    def GetActiveAnimCount(*args): return _blcelc.iSpriteCal3DState_GetActiveAnimCount(*args)
    def GetActiveAnims(*args): return _blcelc.iSpriteCal3DState_GetActiveAnims(*args)
    def SetActiveAnims(*args): return _blcelc.iSpriteCal3DState_SetActiveAnims(*args)
    def SetAnimAction(*args): return _blcelc.iSpriteCal3DState_SetAnimAction(*args)
    def SetVelocity(*args): return _blcelc.iSpriteCal3DState_SetVelocity(*args)
    def SetDefaultIdleAnim(*args): return _blcelc.iSpriteCal3DState_SetDefaultIdleAnim(*args)
    def SetLOD(*args): return _blcelc.iSpriteCal3DState_SetLOD(*args)
    def AttachCoreMesh(*args): return _blcelc.iSpriteCal3DState_AttachCoreMesh(*args)
    def DetachCoreMesh(*args): return _blcelc.iSpriteCal3DState_DetachCoreMesh(*args)
    def BlendMorphTarget(*args): return _blcelc.iSpriteCal3DState_BlendMorphTarget(*args)
    def ClearMorphTarget(*args): return _blcelc.iSpriteCal3DState_ClearMorphTarget(*args)
    def FindSocket(*args): return _blcelc.iSpriteCal3DState_FindSocket(*args)
    def SetMaterial(*args): return _blcelc.iSpriteCal3DState_SetMaterial(*args)
    def SetTimeFactor(*args): return _blcelc.iSpriteCal3DState_SetTimeFactor(*args)
    def GetTimeFactor(*args): return _blcelc.iSpriteCal3DState_GetTimeFactor(*args)
    def GetAnimationTime(*args): return _blcelc.iSpriteCal3DState_GetAnimationTime(*args)
    def GetAnimationDuration(*args): return _blcelc.iSpriteCal3DState_GetAnimationDuration(*args)
    def SetAnimationTime(*args): return _blcelc.iSpriteCal3DState_SetAnimationTime(*args)
    def GetCal3DModel(*args): return _blcelc.iSpriteCal3DState_GetCal3DModel(*args)
    def SetUserData(*args): return _blcelc.iSpriteCal3DState_SetUserData(*args)
    def __del__(self, destroy=_blcelc.delete_iSpriteCal3DState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSpriteCal3DState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSpriteCal3DState_scfGetVersion)

class iSpriteCal3DStatePtr(iSpriteCal3DState):
    def __init__(self, this):
        _swig_setattr(self, iSpriteCal3DState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSpriteCal3DState, 'thisown', 0)
        _swig_setattr(self, iSpriteCal3DState,self.__class__,iSpriteCal3DState)
_blcelc.iSpriteCal3DState_swigregister(iSpriteCal3DStatePtr)

iSpriteCal3DState_scfGetVersion = _blcelc.iSpriteCal3DState_scfGetVersion

class csModelConverterFormat(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csModelConverterFormat, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csModelConverterFormat, name)
    def __repr__(self):
        return "<C csModelConverterFormat instance at %s>" % (self.this,)
    __swig_setmethods__["Name"] = _blcelc.csModelConverterFormat_Name_set
    __swig_getmethods__["Name"] = _blcelc.csModelConverterFormat_Name_get
    if _newclass:Name = property(_blcelc.csModelConverterFormat_Name_get, _blcelc.csModelConverterFormat_Name_set)
    __swig_setmethods__["CanLoad"] = _blcelc.csModelConverterFormat_CanLoad_set
    __swig_getmethods__["CanLoad"] = _blcelc.csModelConverterFormat_CanLoad_get
    if _newclass:CanLoad = property(_blcelc.csModelConverterFormat_CanLoad_get, _blcelc.csModelConverterFormat_CanLoad_set)
    __swig_setmethods__["CanSave"] = _blcelc.csModelConverterFormat_CanSave_set
    __swig_getmethods__["CanSave"] = _blcelc.csModelConverterFormat_CanSave_get
    if _newclass:CanSave = property(_blcelc.csModelConverterFormat_CanSave_get, _blcelc.csModelConverterFormat_CanSave_set)
    def __init__(self, *args):
        _swig_setattr(self, csModelConverterFormat, 'this', _blcelc.new_csModelConverterFormat(*args))
        _swig_setattr(self, csModelConverterFormat, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csModelConverterFormat):
        try:
            if self.thisown: destroy(self)
        except: pass

class csModelConverterFormatPtr(csModelConverterFormat):
    def __init__(self, this):
        _swig_setattr(self, csModelConverterFormat, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csModelConverterFormat, 'thisown', 0)
        _swig_setattr(self, csModelConverterFormat,self.__class__,csModelConverterFormat)
_blcelc.csModelConverterFormat_swigregister(csModelConverterFormatPtr)

class iModelConverter(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iModelConverter, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iModelConverter, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iModelConverter instance at %s>" % (self.this,)
    def GetFormatCount(*args): return _blcelc.iModelConverter_GetFormatCount(*args)
    def GetFormat(*args): return _blcelc.iModelConverter_GetFormat(*args)
    def Load(*args): return _blcelc.iModelConverter_Load(*args)
    def Save(*args): return _blcelc.iModelConverter_Save(*args)
    def __del__(self, destroy=_blcelc.delete_iModelConverter):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iModelConverter_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iModelConverter_scfGetVersion)

class iModelConverterPtr(iModelConverter):
    def __init__(self, this):
        _swig_setattr(self, iModelConverter, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iModelConverter, 'thisown', 0)
        _swig_setattr(self, iModelConverter,self.__class__,iModelConverter)
_blcelc.iModelConverter_swigregister(iModelConverterPtr)

iModelConverter_scfGetVersion = _blcelc.iModelConverter_scfGetVersion

CS_MESH_STATICPOS = _blcelc.CS_MESH_STATICPOS
CS_MESH_STATICSHAPE = _blcelc.CS_MESH_STATICSHAPE
CS_FACTORY_STATICSHAPE = _blcelc.CS_FACTORY_STATICSHAPE
class iMeshObjectDrawCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshObjectDrawCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshObjectDrawCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshObjectDrawCallback instance at %s>" % (self.this,)
    def BeforeDrawing(*args): return _blcelc.iMeshObjectDrawCallback_BeforeDrawing(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshObjectDrawCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iMeshObjectDrawCallbackPtr(iMeshObjectDrawCallback):
    def __init__(self, this):
        _swig_setattr(self, iMeshObjectDrawCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshObjectDrawCallback, 'thisown', 0)
        _swig_setattr(self, iMeshObjectDrawCallback,self.__class__,iMeshObjectDrawCallback)
_blcelc.iMeshObjectDrawCallback_swigregister(iMeshObjectDrawCallbackPtr)

class iMeshObject(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshObject, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshObject, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshObject instance at %s>" % (self.this,)
    def GetFactory(*args): return _blcelc.iMeshObject_GetFactory(*args)
    def GetFlags(*args): return _blcelc.iMeshObject_GetFlags(*args)
    def Clone(*args): return _blcelc.iMeshObject_Clone(*args)
    def GetRenderMeshes(*args): return _blcelc.iMeshObject_GetRenderMeshes(*args)
    def SetVisibleCallback(*args): return _blcelc.iMeshObject_SetVisibleCallback(*args)
    def GetVisibleCallback(*args): return _blcelc.iMeshObject_GetVisibleCallback(*args)
    def NextFrame(*args): return _blcelc.iMeshObject_NextFrame(*args)
    def HardTransform(*args): return _blcelc.iMeshObject_HardTransform(*args)
    def SupportsHardTransform(*args): return _blcelc.iMeshObject_SupportsHardTransform(*args)
    def HitBeamOutline(*args): return _blcelc.iMeshObject_HitBeamOutline(*args)
    def HitBeamObject(*args): return _blcelc.iMeshObject_HitBeamObject(*args)
    def SetLogicalParent(*args): return _blcelc.iMeshObject_SetLogicalParent(*args)
    def GetLogicalParent(*args): return _blcelc.iMeshObject_GetLogicalParent(*args)
    def GetObjectModel(*args): return _blcelc.iMeshObject_GetObjectModel(*args)
    def SetColor(*args): return _blcelc.iMeshObject_SetColor(*args)
    def GetColor(*args): return _blcelc.iMeshObject_GetColor(*args)
    def SetMaterialWrapper(*args): return _blcelc.iMeshObject_SetMaterialWrapper(*args)
    def GetMaterialWrapper(*args): return _blcelc.iMeshObject_GetMaterialWrapper(*args)
    def InvalidateMaterialHandles(*args): return _blcelc.iMeshObject_InvalidateMaterialHandles(*args)
    def PositionChild(*args): return _blcelc.iMeshObject_PositionChild(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshObject_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshObject_scfGetVersion)

class iMeshObjectPtr(iMeshObject):
    def __init__(self, this):
        _swig_setattr(self, iMeshObject, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshObject, 'thisown', 0)
        _swig_setattr(self, iMeshObject,self.__class__,iMeshObject)
_blcelc.iMeshObject_swigregister(iMeshObjectPtr)

iMeshObject_scfGetVersion = _blcelc.iMeshObject_scfGetVersion

class iMeshObjectFactory(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshObjectFactory, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshObjectFactory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshObjectFactory instance at %s>" % (self.this,)
    def GetFlags(*args): return _blcelc.iMeshObjectFactory_GetFlags(*args)
    def NewInstance(*args): return _blcelc.iMeshObjectFactory_NewInstance(*args)
    def Clone(*args): return _blcelc.iMeshObjectFactory_Clone(*args)
    def HardTransform(*args): return _blcelc.iMeshObjectFactory_HardTransform(*args)
    def SupportsHardTransform(*args): return _blcelc.iMeshObjectFactory_SupportsHardTransform(*args)
    def SetLogicalParent(*args): return _blcelc.iMeshObjectFactory_SetLogicalParent(*args)
    def GetLogicalParent(*args): return _blcelc.iMeshObjectFactory_GetLogicalParent(*args)
    def GetMeshObjectType(*args): return _blcelc.iMeshObjectFactory_GetMeshObjectType(*args)
    def GetObjectModel(*args): return _blcelc.iMeshObjectFactory_GetObjectModel(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshObjectFactory):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshObjectFactory_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshObjectFactory_scfGetVersion)

class iMeshObjectFactoryPtr(iMeshObjectFactory):
    def __init__(self, this):
        _swig_setattr(self, iMeshObjectFactory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshObjectFactory, 'thisown', 0)
        _swig_setattr(self, iMeshObjectFactory,self.__class__,iMeshObjectFactory)
_blcelc.iMeshObjectFactory_swigregister(iMeshObjectFactoryPtr)

iMeshObjectFactory_scfGetVersion = _blcelc.iMeshObjectFactory_scfGetVersion

class iMeshObjectType(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshObjectType, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshObjectType, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshObjectType instance at %s>" % (self.this,)
    def NewFactory(*args): return _blcelc.iMeshObjectType_NewFactory(*args)
    def __del__(self, destroy=_blcelc.delete_iMeshObjectType):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshObjectType_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshObjectType_scfGetVersion)

class iMeshObjectTypePtr(iMeshObjectType):
    def __init__(self, this):
        _swig_setattr(self, iMeshObjectType, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshObjectType, 'thisown', 0)
        _swig_setattr(self, iMeshObjectType,self.__class__,iMeshObjectType)
_blcelc.iMeshObjectType_swigregister(iMeshObjectTypePtr)

iMeshObjectType_scfGetVersion = _blcelc.iMeshObjectType_scfGetVersion

class iBallState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBallState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBallState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBallState instance at %s>" % (self.this,)
    def SetRadius(*args): return _blcelc.iBallState_SetRadius(*args)
    def GetRadius(*args): return _blcelc.iBallState_GetRadius(*args)
    def SetShift(*args): return _blcelc.iBallState_SetShift(*args)
    def GetShift(*args): return _blcelc.iBallState_GetShift(*args)
    def SetRimVertices(*args): return _blcelc.iBallState_SetRimVertices(*args)
    def GetRimVertices(*args): return _blcelc.iBallState_GetRimVertices(*args)
    def SetMaterialWrapper(*args): return _blcelc.iBallState_SetMaterialWrapper(*args)
    def GetMaterialWrapper(*args): return _blcelc.iBallState_GetMaterialWrapper(*args)
    def SetMixMode(*args): return _blcelc.iBallState_SetMixMode(*args)
    def GetMixMode(*args): return _blcelc.iBallState_GetMixMode(*args)
    def SetReversed(*args): return _blcelc.iBallState_SetReversed(*args)
    def IsReversed(*args): return _blcelc.iBallState_IsReversed(*args)
    def SetTopOnly(*args): return _blcelc.iBallState_SetTopOnly(*args)
    def IsTopOnly(*args): return _blcelc.iBallState_IsTopOnly(*args)
    def SetLighting(*args): return _blcelc.iBallState_SetLighting(*args)
    def IsLighting(*args): return _blcelc.iBallState_IsLighting(*args)
    def SetColor(*args): return _blcelc.iBallState_SetColor(*args)
    def GetColor(*args): return _blcelc.iBallState_GetColor(*args)
    def SetCylindricalMapping(*args): return _blcelc.iBallState_SetCylindricalMapping(*args)
    def IsCylindricalMapping(*args): return _blcelc.iBallState_IsCylindricalMapping(*args)
    def ApplyVertGradient(*args): return _blcelc.iBallState_ApplyVertGradient(*args)
    def ApplyLightSpot(*args): return _blcelc.iBallState_ApplyLightSpot(*args)
    def PaintSky(*args): return _blcelc.iBallState_PaintSky(*args)
    def __del__(self, destroy=_blcelc.delete_iBallState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iBallState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iBallState_scfGetVersion)

class iBallStatePtr(iBallState):
    def __init__(self, this):
        _swig_setattr(self, iBallState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBallState, 'thisown', 0)
        _swig_setattr(self, iBallState,self.__class__,iBallState)
_blcelc.iBallState_swigregister(iBallStatePtr)

iBallState_scfGetVersion = _blcelc.iBallState_scfGetVersion

class csPolygonRange(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPolygonRange, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPolygonRange, name)
    def __repr__(self):
        return "<C csPolygonRange instance at %s>" % (self.this,)
    __swig_setmethods__["start"] = _blcelc.csPolygonRange_start_set
    __swig_getmethods__["start"] = _blcelc.csPolygonRange_start_get
    if _newclass:start = property(_blcelc.csPolygonRange_start_get, _blcelc.csPolygonRange_start_set)
    __swig_setmethods__["end"] = _blcelc.csPolygonRange_end_set
    __swig_getmethods__["end"] = _blcelc.csPolygonRange_end_get
    if _newclass:end = property(_blcelc.csPolygonRange_end_get, _blcelc.csPolygonRange_end_set)
    def __init__(self, *args):
        _swig_setattr(self, csPolygonRange, 'this', _blcelc.new_csPolygonRange(*args))
        _swig_setattr(self, csPolygonRange, 'thisown', 1)
    def Set(*args): return _blcelc.csPolygonRange_Set(*args)
    def __del__(self, destroy=_blcelc.delete_csPolygonRange):
        try:
            if self.thisown: destroy(self)
        except: pass

class csPolygonRangePtr(csPolygonRange):
    def __init__(self, this):
        _swig_setattr(self, csPolygonRange, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPolygonRange, 'thisown', 0)
        _swig_setattr(self, csPolygonRange,self.__class__,csPolygonRange)
_blcelc.csPolygonRange_swigregister(csPolygonRangePtr)

CS_POLY_LIGHTING = _blcelc.CS_POLY_LIGHTING
CS_POLY_COLLDET = _blcelc.CS_POLY_COLLDET
CS_POLY_VISCULL = _blcelc.CS_POLY_VISCULL
CS_POLYINDEX_LAST = _blcelc.CS_POLYINDEX_LAST
CS_THING_NOCOMPRESS = _blcelc.CS_THING_NOCOMPRESS
CS_THING_MOVE_NEVER = _blcelc.CS_THING_MOVE_NEVER
CS_THING_MOVE_OCCASIONAL = _blcelc.CS_THING_MOVE_OCCASIONAL
class iPolygonHandle(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPolygonHandle, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPolygonHandle, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPolygonHandle instance at %s>" % (self.this,)
    def GetThingFactoryState(*args): return _blcelc.iPolygonHandle_GetThingFactoryState(*args)
    def GetMeshObjectFactory(*args): return _blcelc.iPolygonHandle_GetMeshObjectFactory(*args)
    def GetThingState(*args): return _blcelc.iPolygonHandle_GetThingState(*args)
    def GetMeshObject(*args): return _blcelc.iPolygonHandle_GetMeshObject(*args)
    def GetIndex(*args): return _blcelc.iPolygonHandle_GetIndex(*args)
    def __del__(self, destroy=_blcelc.delete_iPolygonHandle):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPolygonHandlePtr(iPolygonHandle):
    def __init__(self, this):
        _swig_setattr(self, iPolygonHandle, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPolygonHandle, 'thisown', 0)
        _swig_setattr(self, iPolygonHandle,self.__class__,iPolygonHandle)
_blcelc.iPolygonHandle_swigregister(iPolygonHandlePtr)

class iThingFactoryState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iThingFactoryState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iThingFactoryState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iThingFactoryState instance at %s>" % (self.this,)
    def CompressVertices(*args): return _blcelc.iThingFactoryState_CompressVertices(*args)
    def GetPolygonCount(*args): return _blcelc.iThingFactoryState_GetPolygonCount(*args)
    def RemovePolygon(*args): return _blcelc.iThingFactoryState_RemovePolygon(*args)
    def RemovePolygons(*args): return _blcelc.iThingFactoryState_RemovePolygons(*args)
    def FindPolygonByName(*args): return _blcelc.iThingFactoryState_FindPolygonByName(*args)
    def AddEmptyPolygon(*args): return _blcelc.iThingFactoryState_AddEmptyPolygon(*args)
    def AddTriangle(*args): return _blcelc.iThingFactoryState_AddTriangle(*args)
    def AddQuad(*args): return _blcelc.iThingFactoryState_AddQuad(*args)
    def AddPolygon(*args): return _blcelc.iThingFactoryState_AddPolygon(*args)
    def AddOutsideBox(*args): return _blcelc.iThingFactoryState_AddOutsideBox(*args)
    def AddInsideBox(*args): return _blcelc.iThingFactoryState_AddInsideBox(*args)
    def SetPolygonName(*args): return _blcelc.iThingFactoryState_SetPolygonName(*args)
    def GetPolygonName(*args): return _blcelc.iThingFactoryState_GetPolygonName(*args)
    def CreatePolygonHandle(*args): return _blcelc.iThingFactoryState_CreatePolygonHandle(*args)
    def SetPolygonMaterial(*args): return _blcelc.iThingFactoryState_SetPolygonMaterial(*args)
    def GetPolygonMaterial(*args): return _blcelc.iThingFactoryState_GetPolygonMaterial(*args)
    def AddPolygonVertex(*args): return _blcelc.iThingFactoryState_AddPolygonVertex(*args)
    def SetPolygonVertexIndices(*args): return _blcelc.iThingFactoryState_SetPolygonVertexIndices(*args)
    def GetPolygonVertexCount(*args): return _blcelc.iThingFactoryState_GetPolygonVertexCount(*args)
    def GetPolygonVertex(*args): return _blcelc.iThingFactoryState_GetPolygonVertex(*args)
    def GetPolygonVertexIndices(*args): return _blcelc.iThingFactoryState_GetPolygonVertexIndices(*args)
    def SetPolygonTextureMapping(*args): return _blcelc.iThingFactoryState_SetPolygonTextureMapping(*args)
    def GetPolygonTextureMapping(*args): return _blcelc.iThingFactoryState_GetPolygonTextureMapping(*args)
    def SetPolygonTextureMappingEnabled(*args): return _blcelc.iThingFactoryState_SetPolygonTextureMappingEnabled(*args)
    def IsPolygonTextureMappingEnabled(*args): return _blcelc.iThingFactoryState_IsPolygonTextureMappingEnabled(*args)
    def SetPolygonFlags(*args): return _blcelc.iThingFactoryState_SetPolygonFlags(*args)
    def ResetPolygonFlags(*args): return _blcelc.iThingFactoryState_ResetPolygonFlags(*args)
    def GetPolygonFlags(*args): return _blcelc.iThingFactoryState_GetPolygonFlags(*args)
    def GetPolygonObjectPlane(*args): return _blcelc.iThingFactoryState_GetPolygonObjectPlane(*args)
    def IsPolygonTransparent(*args): return _blcelc.iThingFactoryState_IsPolygonTransparent(*args)
    def PointOnPolygon(*args): return _blcelc.iThingFactoryState_PointOnPolygon(*args)
    def GetVertexCount(*args): return _blcelc.iThingFactoryState_GetVertexCount(*args)
    def GetVertex(*args): return _blcelc.iThingFactoryState_GetVertex(*args)
    def GetVertices(*args): return _blcelc.iThingFactoryState_GetVertices(*args)
    def CreateVertex(*args): return _blcelc.iThingFactoryState_CreateVertex(*args)
    def SetVertex(*args): return _blcelc.iThingFactoryState_SetVertex(*args)
    def DeleteVertex(*args): return _blcelc.iThingFactoryState_DeleteVertex(*args)
    def DeleteVertices(*args): return _blcelc.iThingFactoryState_DeleteVertices(*args)
    def SetSmoothingFlag(*args): return _blcelc.iThingFactoryState_SetSmoothingFlag(*args)
    def GetSmoothingFlag(*args): return _blcelc.iThingFactoryState_GetSmoothingFlag(*args)
    def GetNormals(*args): return _blcelc.iThingFactoryState_GetNormals(*args)
    def GetCosinusFactor(*args): return _blcelc.iThingFactoryState_GetCosinusFactor(*args)
    def SetCosinusFactor(*args): return _blcelc.iThingFactoryState_SetCosinusFactor(*args)
    def __del__(self, destroy=_blcelc.delete_iThingFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass

class iThingFactoryStatePtr(iThingFactoryState):
    def __init__(self, this):
        _swig_setattr(self, iThingFactoryState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iThingFactoryState, 'thisown', 0)
        _swig_setattr(self, iThingFactoryState,self.__class__,iThingFactoryState)
_blcelc.iThingFactoryState_swigregister(iThingFactoryStatePtr)

class iThingState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iThingState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iThingState, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iThingState instance at %s>" % (self.this,)
    def GetFactory(*args): return _blcelc.iThingState_GetFactory(*args)
    def GetVertexW(*args): return _blcelc.iThingState_GetVertexW(*args)
    def GetVerticesW(*args): return _blcelc.iThingState_GetVerticesW(*args)
    def GetMovingOption(*args): return _blcelc.iThingState_GetMovingOption(*args)
    def SetMovingOption(*args): return _blcelc.iThingState_SetMovingOption(*args)
    def Prepare(*args): return _blcelc.iThingState_Prepare(*args)
    def Unprepare(*args): return _blcelc.iThingState_Unprepare(*args)
    def ReplaceMaterial(*args): return _blcelc.iThingState_ReplaceMaterial(*args)
    def ClearReplacedMaterials(*args): return _blcelc.iThingState_ClearReplacedMaterials(*args)
    def SetMixMode(*args): return _blcelc.iThingState_SetMixMode(*args)
    def GetMixMode(*args): return _blcelc.iThingState_GetMixMode(*args)
    def CreatePolygonHandle(*args): return _blcelc.iThingState_CreatePolygonHandle(*args)
    def GetPolygonWorldPlane(*args): return _blcelc.iThingState_GetPolygonWorldPlane(*args)
    def __del__(self, destroy=_blcelc.delete_iThingState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iThingState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iThingState_scfGetVersion)

class iThingStatePtr(iThingState):
    def __init__(self, this):
        _swig_setattr(self, iThingState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iThingState, 'thisown', 0)
        _swig_setattr(self, iThingState,self.__class__,iThingState)
_blcelc.iThingState_swigregister(iThingStatePtr)

iThingState_scfGetVersion = _blcelc.iThingState_scfGetVersion

class iThingEnvironment(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iThingEnvironment, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iThingEnvironment, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iThingEnvironment instance at %s>" % (self.this,)
    def Clear(*args): return _blcelc.iThingEnvironment_Clear(*args)
    def GetLightmapCellSize(*args): return _blcelc.iThingEnvironment_GetLightmapCellSize(*args)
    def SetLightmapCellSize(*args): return _blcelc.iThingEnvironment_SetLightmapCellSize(*args)
    def GetDefaultLightmapCellSize(*args): return _blcelc.iThingEnvironment_GetDefaultLightmapCellSize(*args)
    def __del__(self, destroy=_blcelc.delete_iThingEnvironment):
        try:
            if self.thisown: destroy(self)
        except: pass

class iThingEnvironmentPtr(iThingEnvironment):
    def __init__(self, this):
        _swig_setattr(self, iThingEnvironment, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iThingEnvironment, 'thisown', 0)
        _swig_setattr(self, iThingEnvironment,self.__class__,iThingEnvironment)
_blcelc.iThingEnvironment_swigregister(iThingEnvironmentPtr)

class iLoaderStatus(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLoaderStatus, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLoaderStatus, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLoaderStatus instance at %s>" % (self.this,)
    def IsReady(*args): return _blcelc.iLoaderStatus_IsReady(*args)
    def IsError(*args): return _blcelc.iLoaderStatus_IsError(*args)
    def __del__(self, destroy=_blcelc.delete_iLoaderStatus):
        try:
            if self.thisown: destroy(self)
        except: pass

class iLoaderStatusPtr(iLoaderStatus):
    def __init__(self, this):
        _swig_setattr(self, iLoaderStatus, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLoaderStatus, 'thisown', 0)
        _swig_setattr(self, iLoaderStatus,self.__class__,iLoaderStatus)
_blcelc.iLoaderStatus_swigregister(iLoaderStatusPtr)

class iLoader(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLoader, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLoader, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLoader instance at %s>" % (self.this,)
    def LoadImage(*args): return _blcelc.iLoader_LoadImage(*args)
    def LoadTexture(*args): return _blcelc.iLoader_LoadTexture(*args)
    def LoadSoundData(*args): return _blcelc.iLoader_LoadSoundData(*args)
    def LoadSound(*args): return _blcelc.iLoader_LoadSound(*args)
    def ThreadedLoadMapFile(*args): return _blcelc.iLoader_ThreadedLoadMapFile(*args)
    def LoadMapFile(*args): return _blcelc.iLoader_LoadMapFile(*args)
    def LoadLibraryFile(*args): return _blcelc.iLoader_LoadLibraryFile(*args)
    def LoadMeshObjectFactory(*args): return _blcelc.iLoader_LoadMeshObjectFactory(*args)
    def LoadMeshObject(*args): return _blcelc.iLoader_LoadMeshObject(*args)
    def Load(*args): return _blcelc.iLoader_Load(*args)
    def __del__(self, destroy=_blcelc.delete_iLoader):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iLoader_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iLoader_scfGetVersion)

class iLoaderPtr(iLoader):
    def __init__(self, this):
        _swig_setattr(self, iLoader, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLoader, 'thisown', 0)
        _swig_setattr(self, iLoader,self.__class__,iLoader)
_blcelc.iLoader_swigregister(iLoaderPtr)

iLoader_scfGetVersion = _blcelc.iLoader_scfGetVersion

class iLoaderPlugin(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLoaderPlugin, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLoaderPlugin, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLoaderPlugin instance at %s>" % (self.this,)
    def Parse(*args): return _blcelc.iLoaderPlugin_Parse(*args)
    def __del__(self, destroy=_blcelc.delete_iLoaderPlugin):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iLoaderPlugin_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iLoaderPlugin_scfGetVersion)

class iLoaderPluginPtr(iLoaderPlugin):
    def __init__(self, this):
        _swig_setattr(self, iLoaderPlugin, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLoaderPlugin, 'thisown', 0)
        _swig_setattr(self, iLoaderPlugin,self.__class__,iLoaderPlugin)
_blcelc.iLoaderPlugin_swigregister(iLoaderPluginPtr)

iLoaderPlugin_scfGetVersion = _blcelc.iLoaderPlugin_scfGetVersion

class iBinaryLoaderPlugin(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBinaryLoaderPlugin, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBinaryLoaderPlugin, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBinaryLoaderPlugin instance at %s>" % (self.this,)
    def Parse(*args): return _blcelc.iBinaryLoaderPlugin_Parse(*args)
    def __del__(self, destroy=_blcelc.delete_iBinaryLoaderPlugin):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iBinaryLoaderPlugin_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iBinaryLoaderPlugin_scfGetVersion)

class iBinaryLoaderPluginPtr(iBinaryLoaderPlugin):
    def __init__(self, this):
        _swig_setattr(self, iBinaryLoaderPlugin, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBinaryLoaderPlugin, 'thisown', 0)
        _swig_setattr(self, iBinaryLoaderPlugin,self.__class__,iBinaryLoaderPlugin)
_blcelc.iBinaryLoaderPlugin_swigregister(iBinaryLoaderPluginPtr)

iBinaryLoaderPlugin_scfGetVersion = _blcelc.iBinaryLoaderPlugin_scfGetVersion

class iSaver(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSaver, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSaver, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSaver instance at %s>" % (self.this,)
    def SaveMapFile(*args): return _blcelc.iSaver_SaveMapFile(*args)
    def __del__(self, destroy=_blcelc.delete_iSaver):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSaverPtr(iSaver):
    def __init__(self, this):
        _swig_setattr(self, iSaver, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSaver, 'thisown', 0)
        _swig_setattr(self, iSaver,self.__class__,iSaver)
_blcelc.iSaver_swigregister(iSaverPtr)

class iSoundHandle(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSoundHandle, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSoundHandle, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundHandle instance at %s>" % (self.this,)
    def IsStatic(*args): return _blcelc.iSoundHandle_IsStatic(*args)
    def Play(*args): return _blcelc.iSoundHandle_Play(*args)
    def CreateSource(*args): return _blcelc.iSoundHandle_CreateSource(*args)
    def StartStream(*args): return _blcelc.iSoundHandle_StartStream(*args)
    def StopStream(*args): return _blcelc.iSoundHandle_StopStream(*args)
    def ResetStream(*args): return _blcelc.iSoundHandle_ResetStream(*args)
    def __del__(self, destroy=_blcelc.delete_iSoundHandle):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundHandle_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundHandle_scfGetVersion)

class iSoundHandlePtr(iSoundHandle):
    def __init__(self, this):
        _swig_setattr(self, iSoundHandle, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSoundHandle, 'thisown', 0)
        _swig_setattr(self, iSoundHandle,self.__class__,iSoundHandle)
_blcelc.iSoundHandle_swigregister(iSoundHandlePtr)

iSoundHandle_scfGetVersion = _blcelc.iSoundHandle_scfGetVersion

class iSoundLoader(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSoundLoader, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSoundLoader, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundLoader instance at %s>" % (self.this,)
    def LoadSound(*args): return _blcelc.iSoundLoader_LoadSound(*args)
    def __del__(self, destroy=_blcelc.delete_iSoundLoader):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundLoader_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundLoader_scfGetVersion)

class iSoundLoaderPtr(iSoundLoader):
    def __init__(self, this):
        _swig_setattr(self, iSoundLoader, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSoundLoader, 'thisown', 0)
        _swig_setattr(self, iSoundLoader,self.__class__,iSoundLoader)
_blcelc.iSoundLoader_swigregister(iSoundLoaderPtr)

iSoundLoader_scfGetVersion = _blcelc.iSoundLoader_scfGetVersion

class iSoundRender(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSoundRender, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSoundRender, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundRender instance at %s>" % (self.this,)
    def SetVolume(*args): return _blcelc.iSoundRender_SetVolume(*args)
    def GetVolume(*args): return _blcelc.iSoundRender_GetVolume(*args)
    def RegisterSound(*args): return _blcelc.iSoundRender_RegisterSound(*args)
    def UnregisterSound(*args): return _blcelc.iSoundRender_UnregisterSound(*args)
    def GetListener(*args): return _blcelc.iSoundRender_GetListener(*args)
    def MixingFunction(*args): return _blcelc.iSoundRender_MixingFunction(*args)
    def __del__(self, destroy=_blcelc.delete_iSoundRender):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundRender_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundRender_scfGetVersion)

class iSoundRenderPtr(iSoundRender):
    def __init__(self, this):
        _swig_setattr(self, iSoundRender, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSoundRender, 'thisown', 0)
        _swig_setattr(self, iSoundRender,self.__class__,iSoundRender)
_blcelc.iSoundRender_swigregister(iSoundRenderPtr)

iSoundRender_scfGetVersion = _blcelc.iSoundRender_scfGetVersion

class iSoundWrapper(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSoundWrapper, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSoundWrapper, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundWrapper instance at %s>" % (self.this,)
    def GetSound(*args): return _blcelc.iSoundWrapper_GetSound(*args)
    def QueryObject(*args): return _blcelc.iSoundWrapper_QueryObject(*args)
    def __del__(self, destroy=_blcelc.delete_iSoundWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundWrapper_scfGetVersion)

class iSoundWrapperPtr(iSoundWrapper):
    def __init__(self, this):
        _swig_setattr(self, iSoundWrapper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSoundWrapper, 'thisown', 0)
        _swig_setattr(self, iSoundWrapper,self.__class__,iSoundWrapper)
_blcelc.iSoundWrapper_swigregister(iSoundWrapperPtr)

iSoundWrapper_scfGetVersion = _blcelc.iSoundWrapper_scfGetVersion

class iSoundDriver(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSoundDriver, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSoundDriver, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundDriver instance at %s>" % (self.this,)
    def Open(*args): return _blcelc.iSoundDriver_Open(*args)
    def Close(*args): return _blcelc.iSoundDriver_Close(*args)
    def LockMemory(*args): return _blcelc.iSoundDriver_LockMemory(*args)
    def UnlockMemory(*args): return _blcelc.iSoundDriver_UnlockMemory(*args)
    def IsBackground(*args): return _blcelc.iSoundDriver_IsBackground(*args)
    def Is16Bits(*args): return _blcelc.iSoundDriver_Is16Bits(*args)
    def IsStereo(*args): return _blcelc.iSoundDriver_IsStereo(*args)
    def GetFrequency(*args): return _blcelc.iSoundDriver_GetFrequency(*args)
    def IsHandleVoidSound(*args): return _blcelc.iSoundDriver_IsHandleVoidSound(*args)
    def ThreadAware(*args): return _blcelc.iSoundDriver_ThreadAware(*args)
    def __del__(self, destroy=_blcelc.delete_iSoundDriver):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundDriver_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundDriver_scfGetVersion)

class iSoundDriverPtr(iSoundDriver):
    def __init__(self, this):
        _swig_setattr(self, iSoundDriver, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSoundDriver, 'thisown', 0)
        _swig_setattr(self, iSoundDriver,self.__class__,iSoundDriver)
_blcelc.iSoundDriver_swigregister(iSoundDriverPtr)

iSoundDriver_scfGetVersion = _blcelc.iSoundDriver_scfGetVersion

SOUND_RESTART = _blcelc.SOUND_RESTART
SOUND_LOOP = _blcelc.SOUND_LOOP
SOUND3D_DISABLE = _blcelc.SOUND3D_DISABLE
SOUND3D_RELATIVE = _blcelc.SOUND3D_RELATIVE
SOUND3D_ABSOLUTE = _blcelc.SOUND3D_ABSOLUTE
SOUND_DISTANCE_INFINITE = _blcelc.SOUND_DISTANCE_INFINITE
class iSoundSource(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSoundSource, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSoundSource, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundSource instance at %s>" % (self.this,)
    def Play(*args): return _blcelc.iSoundSource_Play(*args)
    def Stop(*args): return _blcelc.iSoundSource_Stop(*args)
    def SetVolume(*args): return _blcelc.iSoundSource_SetVolume(*args)
    def GetVolume(*args): return _blcelc.iSoundSource_GetVolume(*args)
    def SetFrequencyFactor(*args): return _blcelc.iSoundSource_SetFrequencyFactor(*args)
    def GetFrequencyFactor(*args): return _blcelc.iSoundSource_GetFrequencyFactor(*args)
    def GetMode3D(*args): return _blcelc.iSoundSource_GetMode3D(*args)
    def SetMode3D(*args): return _blcelc.iSoundSource_SetMode3D(*args)
    def SetPosition(*args): return _blcelc.iSoundSource_SetPosition(*args)
    def GetPosition(*args): return _blcelc.iSoundSource_GetPosition(*args)
    def SetVelocity(*args): return _blcelc.iSoundSource_SetVelocity(*args)
    def GetVelocity(*args): return _blcelc.iSoundSource_GetVelocity(*args)
    def SetMinimumDistance(*args): return _blcelc.iSoundSource_SetMinimumDistance(*args)
    def SetMaximumDistance(*args): return _blcelc.iSoundSource_SetMaximumDistance(*args)
    def GetMinimumDistance(*args): return _blcelc.iSoundSource_GetMinimumDistance(*args)
    def GetMaximumDistance(*args): return _blcelc.iSoundSource_GetMaximumDistance(*args)
    def __del__(self, destroy=_blcelc.delete_iSoundSource):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundSource_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundSource_scfGetVersion)

class iSoundSourcePtr(iSoundSource):
    def __init__(self, this):
        _swig_setattr(self, iSoundSource, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSoundSource, 'thisown', 0)
        _swig_setattr(self, iSoundSource,self.__class__,iSoundSource)
_blcelc.iSoundSource_swigregister(iSoundSourcePtr)

iSoundSource_scfGetVersion = _blcelc.iSoundSource_scfGetVersion

class iComponent(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iComponent, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iComponent, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iComponent instance at %s>" % (self.this,)
    def Initialize(*args): return _blcelc.iComponent_Initialize(*args)
    def __del__(self, destroy=_blcelc.delete_iComponent):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iComponent_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iComponent_scfGetVersion)

class iComponentPtr(iComponent):
    def __init__(self, this):
        _swig_setattr(self, iComponent, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iComponent, 'thisown', 0)
        _swig_setattr(self, iComponent,self.__class__,iComponent)
_blcelc.iComponent_swigregister(iComponentPtr)

iComponent_scfGetVersion = _blcelc.iComponent_scfGetVersion

class iCacheManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCacheManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCacheManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCacheManager instance at %s>" % (self.this,)
    def SetCurrentType(*args): return _blcelc.iCacheManager_SetCurrentType(*args)
    def GetCurrentType(*args): return _blcelc.iCacheManager_GetCurrentType(*args)
    def SetCurrentScope(*args): return _blcelc.iCacheManager_SetCurrentScope(*args)
    def GetCurrentScope(*args): return _blcelc.iCacheManager_GetCurrentScope(*args)
    def CacheData(*args): return _blcelc.iCacheManager_CacheData(*args)
    def ReadCache(*args): return _blcelc.iCacheManager_ReadCache(*args)
    def ClearCache(*args): return _blcelc.iCacheManager_ClearCache(*args)
    def Flush(*args): return _blcelc.iCacheManager_Flush(*args)
    def __del__(self, destroy=_blcelc.delete_iCacheManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCacheManager_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCacheManager_scfGetVersion)

class iCacheManagerPtr(iCacheManager):
    def __init__(self, this):
        _swig_setattr(self, iCacheManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCacheManager, 'thisown', 0)
        _swig_setattr(self, iCacheManager,self.__class__,iCacheManager)
_blcelc.iCacheManager_swigregister(iCacheManagerPtr)

iCacheManager_scfGetVersion = _blcelc.iCacheManager_scfGetVersion

class csFileTime(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csFileTime, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csFileTime, name)
    def __repr__(self):
        return "<C csFileTime instance at %s>" % (self.this,)
    __swig_setmethods__["sec"] = _blcelc.csFileTime_sec_set
    __swig_getmethods__["sec"] = _blcelc.csFileTime_sec_get
    if _newclass:sec = property(_blcelc.csFileTime_sec_get, _blcelc.csFileTime_sec_set)
    __swig_setmethods__["min"] = _blcelc.csFileTime_min_set
    __swig_getmethods__["min"] = _blcelc.csFileTime_min_get
    if _newclass:min = property(_blcelc.csFileTime_min_get, _blcelc.csFileTime_min_set)
    __swig_setmethods__["hour"] = _blcelc.csFileTime_hour_set
    __swig_getmethods__["hour"] = _blcelc.csFileTime_hour_get
    if _newclass:hour = property(_blcelc.csFileTime_hour_get, _blcelc.csFileTime_hour_set)
    __swig_setmethods__["day"] = _blcelc.csFileTime_day_set
    __swig_getmethods__["day"] = _blcelc.csFileTime_day_get
    if _newclass:day = property(_blcelc.csFileTime_day_get, _blcelc.csFileTime_day_set)
    __swig_setmethods__["mon"] = _blcelc.csFileTime_mon_set
    __swig_getmethods__["mon"] = _blcelc.csFileTime_mon_get
    if _newclass:mon = property(_blcelc.csFileTime_mon_get, _blcelc.csFileTime_mon_set)
    __swig_setmethods__["year"] = _blcelc.csFileTime_year_set
    __swig_getmethods__["year"] = _blcelc.csFileTime_year_get
    if _newclass:year = property(_blcelc.csFileTime_year_get, _blcelc.csFileTime_year_set)
    def __init__(self, *args):
        _swig_setattr(self, csFileTime, 'this', _blcelc.new_csFileTime(*args))
        _swig_setattr(self, csFileTime, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csFileTime):
        try:
            if self.thisown: destroy(self)
        except: pass

class csFileTimePtr(csFileTime):
    def __init__(self, this):
        _swig_setattr(self, csFileTime, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csFileTime, 'thisown', 0)
        _swig_setattr(self, csFileTime,self.__class__,csFileTime)
_blcelc.csFileTime_swigregister(csFileTimePtr)

VFS_PATH_DIVIDER = _blcelc.VFS_PATH_DIVIDER
VFS_PATH_SEPARATOR = _blcelc.VFS_PATH_SEPARATOR
VFS_MAX_PATH_LEN = _blcelc.VFS_MAX_PATH_LEN
VFS_FILE_MODE = _blcelc.VFS_FILE_MODE
VFS_FILE_READ = _blcelc.VFS_FILE_READ
VFS_FILE_WRITE = _blcelc.VFS_FILE_WRITE
VFS_FILE_APPEND = _blcelc.VFS_FILE_APPEND
VFS_FILE_UNCOMPRESSED = _blcelc.VFS_FILE_UNCOMPRESSED
VFS_STATUS_OK = _blcelc.VFS_STATUS_OK
VFS_STATUS_OTHER = _blcelc.VFS_STATUS_OTHER
VFS_STATUS_NOSPACE = _blcelc.VFS_STATUS_NOSPACE
VFS_STATUS_RESOURCES = _blcelc.VFS_STATUS_RESOURCES
VFS_STATUS_ACCESSDENIED = _blcelc.VFS_STATUS_ACCESSDENIED
VFS_STATUS_IOERROR = _blcelc.VFS_STATUS_IOERROR
class iFile(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iFile, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iFile, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFile instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iFile_GetName(*args)
    def GetSize(*args): return _blcelc.iFile_GetSize(*args)
    def GetStatus(*args): return _blcelc.iFile_GetStatus(*args)
    def Read(*args): return _blcelc.iFile_Read(*args)
    def Write(*args): return _blcelc.iFile_Write(*args)
    def Flush(*args): return _blcelc.iFile_Flush(*args)
    def AtEOF(*args): return _blcelc.iFile_AtEOF(*args)
    def GetPos(*args): return _blcelc.iFile_GetPos(*args)
    def SetPos(*args): return _blcelc.iFile_SetPos(*args)
    def GetAllData(*args): return _blcelc.iFile_GetAllData(*args)
    def __del__(self, destroy=_blcelc.delete_iFile):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFile_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFile_scfGetVersion)

class iFilePtr(iFile):
    def __init__(self, this):
        _swig_setattr(self, iFile, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iFile, 'thisown', 0)
        _swig_setattr(self, iFile,self.__class__,iFile)
_blcelc.iFile_swigregister(iFilePtr)

iFile_scfGetVersion = _blcelc.iFile_scfGetVersion

class iVFS(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVFS, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVFS, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVFS instance at %s>" % (self.this,)
    def ChDir(*args): return _blcelc.iVFS_ChDir(*args)
    def GetCwd(*args): return _blcelc.iVFS_GetCwd(*args)
    def PushDir(*args): return _blcelc.iVFS_PushDir(*args)
    def PopDir(*args): return _blcelc.iVFS_PopDir(*args)
    def ExpandPath(*args): return _blcelc.iVFS_ExpandPath(*args)
    def Exists(*args): return _blcelc.iVFS_Exists(*args)
    def FindFiles(*args): return _blcelc.iVFS_FindFiles(*args)
    def Open(*args): return _blcelc.iVFS_Open(*args)
    def ReadFile(*args): return _blcelc.iVFS_ReadFile(*args)
    def WriteFile(*args): return _blcelc.iVFS_WriteFile(*args)
    def DeleteFile(*args): return _blcelc.iVFS_DeleteFile(*args)
    def Sync(*args): return _blcelc.iVFS_Sync(*args)
    def Mount(*args): return _blcelc.iVFS_Mount(*args)
    def Unmount(*args): return _blcelc.iVFS_Unmount(*args)
    def MountRoot(*args): return _blcelc.iVFS_MountRoot(*args)
    def SaveMounts(*args): return _blcelc.iVFS_SaveMounts(*args)
    def LoadMountsFromFile(*args): return _blcelc.iVFS_LoadMountsFromFile(*args)
    def ChDirAuto(*args): return _blcelc.iVFS_ChDirAuto(*args)
    def GetFileTime(*args): return _blcelc.iVFS_GetFileTime(*args)
    def SetFileTime(*args): return _blcelc.iVFS_SetFileTime(*args)
    def GetFileSize(*args): return _blcelc.iVFS_GetFileSize(*args)
    def GetRealPath(*args): return _blcelc.iVFS_GetRealPath(*args)
    def GetMounts(*args): return _blcelc.iVFS_GetMounts(*args)
    def GetRealMountPaths(*args): return _blcelc.iVFS_GetRealMountPaths(*args)
    def __del__(self, destroy=_blcelc.delete_iVFS):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iVFS_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iVFS_scfGetVersion)

class iVFSPtr(iVFS):
    def __init__(self, this):
        _swig_setattr(self, iVFS, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVFS, 'thisown', 0)
        _swig_setattr(self, iVFS,self.__class__,iVFS)
_blcelc.iVFS_swigregister(iVFSPtr)

iVFS_scfGetVersion = _blcelc.iVFS_scfGetVersion

class iObject(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iObject, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iObject, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObject instance at %s>" % (self.this,)
    def SetName(*args): return _blcelc.iObject_SetName(*args)
    def GetName(*args): return _blcelc.iObject_GetName(*args)
    def GetID(*args): return _blcelc.iObject_GetID(*args)
    def SetObjectParent(*args): return _blcelc.iObject_SetObjectParent(*args)
    def GetObjectParent(*args): return _blcelc.iObject_GetObjectParent(*args)
    def ObjAdd(*args): return _blcelc.iObject_ObjAdd(*args)
    def ObjRemove(*args): return _blcelc.iObject_ObjRemove(*args)
    def ObjRemoveAll(*args): return _blcelc.iObject_ObjRemoveAll(*args)
    def ObjAddChildren(*args): return _blcelc.iObject_ObjAddChildren(*args)
    def GetChild(*args): return _blcelc.iObject_GetChild(*args)
    def GetIterator(*args): return _blcelc.iObject_GetIterator(*args)
    def ObjReleaseOld(*args): return _blcelc.iObject_ObjReleaseOld(*args)
    def __del__(self, destroy=_blcelc.delete_iObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iObject_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iObject_scfGetVersion)

class iObjectPtr(iObject):
    def __init__(self, this):
        _swig_setattr(self, iObject, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iObject, 'thisown', 0)
        _swig_setattr(self, iObject,self.__class__,iObject)
_blcelc.iObject_swigregister(iObjectPtr)

iObject_scfGetVersion = _blcelc.iObject_scfGetVersion

class iObjectIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iObjectIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iObjectIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectIterator instance at %s>" % (self.this,)
    def Next(*args): return _blcelc.iObjectIterator_Next(*args)
    def Reset(*args): return _blcelc.iObjectIterator_Reset(*args)
    def GetParentObj(*args): return _blcelc.iObjectIterator_GetParentObj(*args)
    def HasNext(*args): return _blcelc.iObjectIterator_HasNext(*args)
    def FindName(*args): return _blcelc.iObjectIterator_FindName(*args)
    def __del__(self, destroy=_blcelc.delete_iObjectIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class iObjectIteratorPtr(iObjectIterator):
    def __init__(self, this):
        _swig_setattr(self, iObjectIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iObjectIterator, 'thisown', 0)
        _swig_setattr(self, iObjectIterator,self.__class__,iObjectIterator)
_blcelc.iObjectIterator_swigregister(iObjectIteratorPtr)

class iObjectRegistry(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iObjectRegistry, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iObjectRegistry, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectRegistry instance at %s>" % (self.this,)
    def Clear(*args): return _blcelc.iObjectRegistry_Clear(*args)
    def Register(*args): return _blcelc.iObjectRegistry_Register(*args)
    def Unregister(*args): return _blcelc.iObjectRegistry_Unregister(*args)
    def Get(*args): return _blcelc.iObjectRegistry_Get(*args)
    def __del__(self, destroy=_blcelc.delete_iObjectRegistry):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iObjectRegistry_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iObjectRegistry_scfGetVersion)

class iObjectRegistryPtr(iObjectRegistry):
    def __init__(self, this):
        _swig_setattr(self, iObjectRegistry, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iObjectRegistry, 'thisown', 0)
        _swig_setattr(self, iObjectRegistry,self.__class__,iObjectRegistry)
_blcelc.iObjectRegistry_swigregister(iObjectRegistryPtr)

iObjectRegistry_scfGetVersion = _blcelc.iObjectRegistry_scfGetVersion

class iObjectRegistryIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iObjectRegistryIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iObjectRegistryIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectRegistryIterator instance at %s>" % (self.this,)
    def Reset(*args): return _blcelc.iObjectRegistryIterator_Reset(*args)
    def GetCurrentTag(*args): return _blcelc.iObjectRegistryIterator_GetCurrentTag(*args)
    def HasNext(*args): return _blcelc.iObjectRegistryIterator_HasNext(*args)
    def Next(*args): return _blcelc.iObjectRegistryIterator_Next(*args)
    def __del__(self, destroy=_blcelc.delete_iObjectRegistryIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class iObjectRegistryIteratorPtr(iObjectRegistryIterator):
    def __init__(self, this):
        _swig_setattr(self, iObjectRegistryIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iObjectRegistryIterator, 'thisown', 0)
        _swig_setattr(self, iObjectRegistryIterator,self.__class__,iObjectRegistryIterator)
_blcelc.iObjectRegistryIterator_swigregister(iObjectRegistryIteratorPtr)

class iVirtualClock(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVirtualClock, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVirtualClock, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVirtualClock instance at %s>" % (self.this,)
    def Advance(*args): return _blcelc.iVirtualClock_Advance(*args)
    def Suspend(*args): return _blcelc.iVirtualClock_Suspend(*args)
    def Resume(*args): return _blcelc.iVirtualClock_Resume(*args)
    def GetElapsedTicks(*args): return _blcelc.iVirtualClock_GetElapsedTicks(*args)
    def GetCurrentTicks(*args): return _blcelc.iVirtualClock_GetCurrentTicks(*args)
    def __del__(self, destroy=_blcelc.delete_iVirtualClock):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iVirtualClock_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iVirtualClock_scfGetVersion)

class iVirtualClockPtr(iVirtualClock):
    def __init__(self, this):
        _swig_setattr(self, iVirtualClock, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVirtualClock, 'thisown', 0)
        _swig_setattr(self, iVirtualClock,self.__class__,iVirtualClock)
_blcelc.iVirtualClock_swigregister(iVirtualClockPtr)

iVirtualClock_scfGetVersion = _blcelc.iVirtualClock_scfGetVersion

class iEventAttributeIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEventAttributeIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEventAttributeIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventAttributeIterator instance at %s>" % (self.this,)
    def HasNext(*args): return _blcelc.iEventAttributeIterator_HasNext(*args)
    def Next(*args): return _blcelc.iEventAttributeIterator_Next(*args)
    def Reset(*args): return _blcelc.iEventAttributeIterator_Reset(*args)
    def __del__(self, destroy=_blcelc.delete_iEventAttributeIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class iEventAttributeIteratorPtr(iEventAttributeIterator):
    def __init__(self, this):
        _swig_setattr(self, iEventAttributeIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEventAttributeIterator, 'thisown', 0)
        _swig_setattr(self, iEventAttributeIterator,self.__class__,iEventAttributeIterator)
_blcelc.iEventAttributeIterator_swigregister(iEventAttributeIteratorPtr)

class csKeyEventData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csKeyEventData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csKeyEventData, name)
    def __repr__(self):
        return "<C csKeyEventData instance at %s>" % (self.this,)
    __swig_setmethods__["eventType"] = _blcelc.csKeyEventData_eventType_set
    __swig_getmethods__["eventType"] = _blcelc.csKeyEventData_eventType_get
    if _newclass:eventType = property(_blcelc.csKeyEventData_eventType_get, _blcelc.csKeyEventData_eventType_set)
    __swig_setmethods__["codeRaw"] = _blcelc.csKeyEventData_codeRaw_set
    __swig_getmethods__["codeRaw"] = _blcelc.csKeyEventData_codeRaw_get
    if _newclass:codeRaw = property(_blcelc.csKeyEventData_codeRaw_get, _blcelc.csKeyEventData_codeRaw_set)
    __swig_setmethods__["codeCooked"] = _blcelc.csKeyEventData_codeCooked_set
    __swig_getmethods__["codeCooked"] = _blcelc.csKeyEventData_codeCooked_get
    if _newclass:codeCooked = property(_blcelc.csKeyEventData_codeCooked_get, _blcelc.csKeyEventData_codeCooked_set)
    __swig_setmethods__["modifiers"] = _blcelc.csKeyEventData_modifiers_set
    __swig_getmethods__["modifiers"] = _blcelc.csKeyEventData_modifiers_get
    if _newclass:modifiers = property(_blcelc.csKeyEventData_modifiers_get, _blcelc.csKeyEventData_modifiers_set)
    __swig_setmethods__["autoRepeat"] = _blcelc.csKeyEventData_autoRepeat_set
    __swig_getmethods__["autoRepeat"] = _blcelc.csKeyEventData_autoRepeat_get
    if _newclass:autoRepeat = property(_blcelc.csKeyEventData_autoRepeat_get, _blcelc.csKeyEventData_autoRepeat_set)
    __swig_setmethods__["charType"] = _blcelc.csKeyEventData_charType_set
    __swig_getmethods__["charType"] = _blcelc.csKeyEventData_charType_get
    if _newclass:charType = property(_blcelc.csKeyEventData_charType_get, _blcelc.csKeyEventData_charType_set)
    def __init__(self, *args):
        _swig_setattr(self, csKeyEventData, 'this', _blcelc.new_csKeyEventData(*args))
        _swig_setattr(self, csKeyEventData, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csKeyEventData):
        try:
            if self.thisown: destroy(self)
        except: pass

class csKeyEventDataPtr(csKeyEventData):
    def __init__(self, this):
        _swig_setattr(self, csKeyEventData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csKeyEventData, 'thisown', 0)
        _swig_setattr(self, csKeyEventData,self.__class__,csKeyEventData)
_blcelc.csKeyEventData_swigregister(csKeyEventDataPtr)

csmbLeft = _blcelc.csmbLeft
csmbRight = _blcelc.csmbRight
csmbMiddle = _blcelc.csmbMiddle
csmbWheelUp = _blcelc.csmbWheelUp
csmbWheelDown = _blcelc.csmbWheelDown
csmbExtra1 = _blcelc.csmbExtra1
csmbExtra2 = _blcelc.csmbExtra2
class csEventMouseData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csEventMouseData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csEventMouseData, name)
    def __repr__(self):
        return "<C csEventMouseData instance at %s>" % (self.this,)
    __swig_setmethods__["x"] = _blcelc.csEventMouseData_x_set
    __swig_getmethods__["x"] = _blcelc.csEventMouseData_x_get
    if _newclass:x = property(_blcelc.csEventMouseData_x_get, _blcelc.csEventMouseData_x_set)
    __swig_setmethods__["y"] = _blcelc.csEventMouseData_y_set
    __swig_getmethods__["y"] = _blcelc.csEventMouseData_y_get
    if _newclass:y = property(_blcelc.csEventMouseData_y_get, _blcelc.csEventMouseData_y_set)
    __swig_setmethods__["Button"] = _blcelc.csEventMouseData_Button_set
    __swig_getmethods__["Button"] = _blcelc.csEventMouseData_Button_get
    if _newclass:Button = property(_blcelc.csEventMouseData_Button_get, _blcelc.csEventMouseData_Button_set)
    __swig_setmethods__["Modifiers"] = _blcelc.csEventMouseData_Modifiers_set
    __swig_getmethods__["Modifiers"] = _blcelc.csEventMouseData_Modifiers_get
    if _newclass:Modifiers = property(_blcelc.csEventMouseData_Modifiers_get, _blcelc.csEventMouseData_Modifiers_set)
    def __init__(self, *args):
        _swig_setattr(self, csEventMouseData, 'this', _blcelc.new_csEventMouseData(*args))
        _swig_setattr(self, csEventMouseData, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csEventMouseData):
        try:
            if self.thisown: destroy(self)
        except: pass

class csEventMouseDataPtr(csEventMouseData):
    def __init__(self, this):
        _swig_setattr(self, csEventMouseData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csEventMouseData, 'thisown', 0)
        _swig_setattr(self, csEventMouseData,self.__class__,csEventMouseData)
_blcelc.csEventMouseData_swigregister(csEventMouseDataPtr)

class csEventJoystickData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csEventJoystickData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csEventJoystickData, name)
    def __repr__(self):
        return "<C csEventJoystickData instance at %s>" % (self.this,)
    __swig_setmethods__["number"] = _blcelc.csEventJoystickData_number_set
    __swig_getmethods__["number"] = _blcelc.csEventJoystickData_number_get
    if _newclass:number = property(_blcelc.csEventJoystickData_number_get, _blcelc.csEventJoystickData_number_set)
    __swig_setmethods__["x"] = _blcelc.csEventJoystickData_x_set
    __swig_getmethods__["x"] = _blcelc.csEventJoystickData_x_get
    if _newclass:x = property(_blcelc.csEventJoystickData_x_get, _blcelc.csEventJoystickData_x_set)
    __swig_setmethods__["y"] = _blcelc.csEventJoystickData_y_set
    __swig_getmethods__["y"] = _blcelc.csEventJoystickData_y_get
    if _newclass:y = property(_blcelc.csEventJoystickData_y_get, _blcelc.csEventJoystickData_y_set)
    __swig_setmethods__["Button"] = _blcelc.csEventJoystickData_Button_set
    __swig_getmethods__["Button"] = _blcelc.csEventJoystickData_Button_get
    if _newclass:Button = property(_blcelc.csEventJoystickData_Button_get, _blcelc.csEventJoystickData_Button_set)
    __swig_setmethods__["Modifiers"] = _blcelc.csEventJoystickData_Modifiers_set
    __swig_getmethods__["Modifiers"] = _blcelc.csEventJoystickData_Modifiers_get
    if _newclass:Modifiers = property(_blcelc.csEventJoystickData_Modifiers_get, _blcelc.csEventJoystickData_Modifiers_set)
    def __init__(self, *args):
        _swig_setattr(self, csEventJoystickData, 'this', _blcelc.new_csEventJoystickData(*args))
        _swig_setattr(self, csEventJoystickData, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csEventJoystickData):
        try:
            if self.thisown: destroy(self)
        except: pass

class csEventJoystickDataPtr(csEventJoystickData):
    def __init__(self, this):
        _swig_setattr(self, csEventJoystickData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csEventJoystickData, 'thisown', 0)
        _swig_setattr(self, csEventJoystickData,self.__class__,csEventJoystickData)
_blcelc.csEventJoystickData_swigregister(csEventJoystickDataPtr)

class csEventCommandData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csEventCommandData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csEventCommandData, name)
    def __repr__(self):
        return "<C csEventCommandData instance at %s>" % (self.this,)
    __swig_setmethods__["Code"] = _blcelc.csEventCommandData_Code_set
    __swig_getmethods__["Code"] = _blcelc.csEventCommandData_Code_get
    if _newclass:Code = property(_blcelc.csEventCommandData_Code_get, _blcelc.csEventCommandData_Code_set)
    __swig_setmethods__["Info"] = _blcelc.csEventCommandData_Info_set
    __swig_getmethods__["Info"] = _blcelc.csEventCommandData_Info_get
    if _newclass:Info = property(_blcelc.csEventCommandData_Info_get, _blcelc.csEventCommandData_Info_set)
    def __init__(self, *args):
        _swig_setattr(self, csEventCommandData, 'this', _blcelc.new_csEventCommandData(*args))
        _swig_setattr(self, csEventCommandData, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csEventCommandData):
        try:
            if self.thisown: destroy(self)
        except: pass

class csEventCommandDataPtr(csEventCommandData):
    def __init__(self, this):
        _swig_setattr(self, csEventCommandData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csEventCommandData, 'thisown', 0)
        _swig_setattr(self, csEventCommandData,self.__class__,csEventCommandData)
_blcelc.csEventCommandData_swigregister(csEventCommandDataPtr)

csEventErrNone = _blcelc.csEventErrNone
csEventErrLossy = _blcelc.csEventErrLossy
csEventErrNotFound = _blcelc.csEventErrNotFound
csEventErrMismatchInt = _blcelc.csEventErrMismatchInt
csEventErrMismatchUInt = _blcelc.csEventErrMismatchUInt
csEventErrMismatchFloat = _blcelc.csEventErrMismatchFloat
csEventErrMismatchBuffer = _blcelc.csEventErrMismatchBuffer
csEventErrMismatchEvent = _blcelc.csEventErrMismatchEvent
csEventErrMismatchIBase = _blcelc.csEventErrMismatchIBase
csEventErrUhOhUnknown = _blcelc.csEventErrUhOhUnknown
csEventAttrUnknown = _blcelc.csEventAttrUnknown
csEventAttrInt = _blcelc.csEventAttrInt
csEventAttrUInt = _blcelc.csEventAttrUInt
csEventAttrFloat = _blcelc.csEventAttrFloat
csEventAttrDatabuffer = _blcelc.csEventAttrDatabuffer
csEventAttrEvent = _blcelc.csEventAttrEvent
csEventAttriBase = _blcelc.csEventAttriBase
class iEvent(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEvent, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEvent, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEvent instance at %s>" % (self.this,)
    __swig_setmethods__["Type"] = _blcelc.iEvent_Type_set
    __swig_getmethods__["Type"] = _blcelc.iEvent_Type_get
    if _newclass:Type = property(_blcelc.iEvent_Type_get, _blcelc.iEvent_Type_set)
    __swig_setmethods__["Category"] = _blcelc.iEvent_Category_set
    __swig_getmethods__["Category"] = _blcelc.iEvent_Category_get
    if _newclass:Category = property(_blcelc.iEvent_Category_get, _blcelc.iEvent_Category_set)
    __swig_setmethods__["SubCategory"] = _blcelc.iEvent_SubCategory_set
    __swig_getmethods__["SubCategory"] = _blcelc.iEvent_SubCategory_get
    if _newclass:SubCategory = property(_blcelc.iEvent_SubCategory_get, _blcelc.iEvent_SubCategory_set)
    __swig_setmethods__["Flags"] = _blcelc.iEvent_Flags_set
    __swig_getmethods__["Flags"] = _blcelc.iEvent_Flags_get
    if _newclass:Flags = property(_blcelc.iEvent_Flags_get, _blcelc.iEvent_Flags_set)
    __swig_setmethods__["Time"] = _blcelc.iEvent_Time_set
    __swig_getmethods__["Time"] = _blcelc.iEvent_Time_get
    if _newclass:Time = property(_blcelc.iEvent_Time_get, _blcelc.iEvent_Time_set)
    def AddInt8(*args): return _blcelc.iEvent_AddInt8(*args)
    def AddUInt8(*args): return _blcelc.iEvent_AddUInt8(*args)
    def AddInt16(*args): return _blcelc.iEvent_AddInt16(*args)
    def AddUInt16(*args): return _blcelc.iEvent_AddUInt16(*args)
    def AddInt32(*args): return _blcelc.iEvent_AddInt32(*args)
    def AddUInt32(*args): return _blcelc.iEvent_AddUInt32(*args)
    def AddFloat(*args): return _blcelc.iEvent_AddFloat(*args)
    def AddDouble(*args): return _blcelc.iEvent_AddDouble(*args)
    def AddBool(*args): return _blcelc.iEvent_AddBool(*args)
    def Add(*args): return _blcelc.iEvent_Add(*args)
    def RetrieveInt8(*args): return _blcelc.iEvent_RetrieveInt8(*args)
    def RetrieveUInt8(*args): return _blcelc.iEvent_RetrieveUInt8(*args)
    def RetrieveInt16(*args): return _blcelc.iEvent_RetrieveInt16(*args)
    def RetrieveUInt16(*args): return _blcelc.iEvent_RetrieveUInt16(*args)
    def RetrieveUInt32(*args): return _blcelc.iEvent_RetrieveUInt32(*args)
    def RetrieveFloat(*args): return _blcelc.iEvent_RetrieveFloat(*args)
    def RetrieveDouble(*args): return _blcelc.iEvent_RetrieveDouble(*args)
    def RetrieveBool(*args): return _blcelc.iEvent_RetrieveBool(*args)
    def Retrieve(*args): return _blcelc.iEvent_Retrieve(*args)
    def AttributeExists(*args): return _blcelc.iEvent_AttributeExists(*args)
    def GetAttributeType(*args): return _blcelc.iEvent_GetAttributeType(*args)
    def Remove(*args): return _blcelc.iEvent_Remove(*args)
    def RemoveAll(*args): return _blcelc.iEvent_RemoveAll(*args)
    def GetAttributeIterator(*args): return _blcelc.iEvent_GetAttributeIterator(*args)
    def __del__(self, destroy=_blcelc.delete_iEvent):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iEvent_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iEvent_scfGetVersion)
    __swig_getmethods__["Mouse"] = _blcelc.iEvent_Mouse_get
    if _newclass:Mouse = property(_blcelc.iEvent_Mouse_get)
    __swig_getmethods__["Joystick"] = _blcelc.iEvent_Joystick_get
    if _newclass:Joystick = property(_blcelc.iEvent_Joystick_get)
    __swig_getmethods__["Command"] = _blcelc.iEvent_Command_get
    if _newclass:Command = property(_blcelc.iEvent_Command_get)

class iEventPtr(iEvent):
    def __init__(self, this):
        _swig_setattr(self, iEvent, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEvent, 'thisown', 0)
        _swig_setattr(self, iEvent,self.__class__,iEvent)
_blcelc.iEvent_swigregister(iEventPtr)

iEvent_scfGetVersion = _blcelc.iEvent_scfGetVersion

class iEventPlug(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEventPlug, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEventPlug, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventPlug instance at %s>" % (self.this,)
    def GetPotentiallyConflictingEvents(*args): return _blcelc.iEventPlug_GetPotentiallyConflictingEvents(*args)
    def QueryEventPriority(*args): return _blcelc.iEventPlug_QueryEventPriority(*args)
    def EnableEvents(*args): return _blcelc.iEventPlug_EnableEvents(*args)
    def __del__(self, destroy=_blcelc.delete_iEventPlug):
        try:
            if self.thisown: destroy(self)
        except: pass

class iEventPlugPtr(iEventPlug):
    def __init__(self, this):
        _swig_setattr(self, iEventPlug, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEventPlug, 'thisown', 0)
        _swig_setattr(self, iEventPlug,self.__class__,iEventPlug)
_blcelc.iEventPlug_swigregister(iEventPlugPtr)

class iEventOutlet(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEventOutlet, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEventOutlet, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventOutlet instance at %s>" % (self.this,)
    def CreateEvent(*args): return _blcelc.iEventOutlet_CreateEvent(*args)
    def Post(*args): return _blcelc.iEventOutlet_Post(*args)
    def Key(*args): return _blcelc.iEventOutlet_Key(*args)
    def Mouse(*args): return _blcelc.iEventOutlet_Mouse(*args)
    def Joystick(*args): return _blcelc.iEventOutlet_Joystick(*args)
    def Broadcast(*args): return _blcelc.iEventOutlet_Broadcast(*args)
    def ImmediateBroadcast(*args): return _blcelc.iEventOutlet_ImmediateBroadcast(*args)
    def __del__(self, destroy=_blcelc.delete_iEventOutlet):
        try:
            if self.thisown: destroy(self)
        except: pass

class iEventOutletPtr(iEventOutlet):
    def __init__(self, this):
        _swig_setattr(self, iEventOutlet, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEventOutlet, 'thisown', 0)
        _swig_setattr(self, iEventOutlet,self.__class__,iEventOutlet)
_blcelc.iEventOutlet_swigregister(iEventOutletPtr)

class iEventCord(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEventCord, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEventCord, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventCord instance at %s>" % (self.this,)
    def Insert(*args): return _blcelc.iEventCord_Insert(*args)
    def Remove(*args): return _blcelc.iEventCord_Remove(*args)
    def GetPass(*args): return _blcelc.iEventCord_GetPass(*args)
    def SetPass(*args): return _blcelc.iEventCord_SetPass(*args)
    def GetCategory(*args): return _blcelc.iEventCord_GetCategory(*args)
    def GetSubcategory(*args): return _blcelc.iEventCord_GetSubcategory(*args)
    def __del__(self, destroy=_blcelc.delete_iEventCord):
        try:
            if self.thisown: destroy(self)
        except: pass

class iEventCordPtr(iEventCord):
    def __init__(self, this):
        _swig_setattr(self, iEventCord, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEventCord, 'thisown', 0)
        _swig_setattr(self, iEventCord,self.__class__,iEventCord)
_blcelc.iEventCord_swigregister(iEventCordPtr)

class csKeyEventHelper(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csKeyEventHelper, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csKeyEventHelper, name)
    def __repr__(self):
        return "<C csKeyEventHelper instance at %s>" % (self.this,)
    __swig_getmethods__["GetRawCode"] = lambda x: _blcelc.csKeyEventHelper_GetRawCode
    if _newclass:GetRawCode = staticmethod(_blcelc.csKeyEventHelper_GetRawCode)
    __swig_getmethods__["GetCookedCode"] = lambda x: _blcelc.csKeyEventHelper_GetCookedCode
    if _newclass:GetCookedCode = staticmethod(_blcelc.csKeyEventHelper_GetCookedCode)
    __swig_getmethods__["GetModifiers"] = lambda x: _blcelc.csKeyEventHelper_GetModifiers
    if _newclass:GetModifiers = staticmethod(_blcelc.csKeyEventHelper_GetModifiers)
    __swig_getmethods__["GetEventType"] = lambda x: _blcelc.csKeyEventHelper_GetEventType
    if _newclass:GetEventType = staticmethod(_blcelc.csKeyEventHelper_GetEventType)
    __swig_getmethods__["GetAutoRepeat"] = lambda x: _blcelc.csKeyEventHelper_GetAutoRepeat
    if _newclass:GetAutoRepeat = staticmethod(_blcelc.csKeyEventHelper_GetAutoRepeat)
    __swig_getmethods__["GetCharacterType"] = lambda x: _blcelc.csKeyEventHelper_GetCharacterType
    if _newclass:GetCharacterType = staticmethod(_blcelc.csKeyEventHelper_GetCharacterType)
    __swig_getmethods__["GetEventData"] = lambda x: _blcelc.csKeyEventHelper_GetEventData
    if _newclass:GetEventData = staticmethod(_blcelc.csKeyEventHelper_GetEventData)
    __swig_getmethods__["GetModifiersBits"] = lambda x: _blcelc.csKeyEventHelper_GetModifiersBits
    if _newclass:GetModifiersBits = staticmethod(_blcelc.csKeyEventHelper_GetModifiersBits)
    __swig_getmethods__["GetModifiersBits"] = lambda x: _blcelc.csKeyEventHelper_GetModifiersBits
    if _newclass:GetModifiersBits = staticmethod(_blcelc.csKeyEventHelper_GetModifiersBits)
    __swig_getmethods__["GetModifiers"] = lambda x: _blcelc.csKeyEventHelper_GetModifiers
    if _newclass:GetModifiers = staticmethod(_blcelc.csKeyEventHelper_GetModifiers)
    def __init__(self, *args):
        _swig_setattr(self, csKeyEventHelper, 'this', _blcelc.new_csKeyEventHelper(*args))
        _swig_setattr(self, csKeyEventHelper, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csKeyEventHelper):
        try:
            if self.thisown: destroy(self)
        except: pass

class csKeyEventHelperPtr(csKeyEventHelper):
    def __init__(self, this):
        _swig_setattr(self, csKeyEventHelper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csKeyEventHelper, 'thisown', 0)
        _swig_setattr(self, csKeyEventHelper,self.__class__,csKeyEventHelper)
_blcelc.csKeyEventHelper_swigregister(csKeyEventHelperPtr)

csKeyEventHelper_GetRawCode = _blcelc.csKeyEventHelper_GetRawCode

csKeyEventHelper_GetCookedCode = _blcelc.csKeyEventHelper_GetCookedCode

csKeyEventHelper_GetEventType = _blcelc.csKeyEventHelper_GetEventType

csKeyEventHelper_GetAutoRepeat = _blcelc.csKeyEventHelper_GetAutoRepeat

csKeyEventHelper_GetCharacterType = _blcelc.csKeyEventHelper_GetCharacterType

csKeyEventHelper_GetEventData = _blcelc.csKeyEventHelper_GetEventData

csKeyEventHelper_GetModifiersBits = _blcelc.csKeyEventHelper_GetModifiersBits

csKeyEventHelper_GetModifiers = _blcelc.csKeyEventHelper_GetModifiers

csevNothing = _blcelc.csevNothing
csevKeyboard = _blcelc.csevKeyboard
csevMouseMove = _blcelc.csevMouseMove
csevMouseDown = _blcelc.csevMouseDown
csevMouseUp = _blcelc.csevMouseUp
csevMouseClick = _blcelc.csevMouseClick
csevMouseDoubleClick = _blcelc.csevMouseDoubleClick
csevJoystickMove = _blcelc.csevJoystickMove
csevJoystickDown = _blcelc.csevJoystickDown
csevJoystickUp = _blcelc.csevJoystickUp
csevCommand = _blcelc.csevCommand
csevBroadcast = _blcelc.csevBroadcast
csevMouseEnter = _blcelc.csevMouseEnter
csevMouseExit = _blcelc.csevMouseExit
csevLostFocus = _blcelc.csevLostFocus
csevGainFocus = _blcelc.csevGainFocus
csevGroupOff = _blcelc.csevGroupOff
csevFrameStart = _blcelc.csevFrameStart
csKeyEventTypeUp = _blcelc.csKeyEventTypeUp
csKeyEventTypeDown = _blcelc.csKeyEventTypeDown
CSEF_BROADCAST = _blcelc.CSEF_BROADCAST
csKeyModifierTypeShift = _blcelc.csKeyModifierTypeShift
csKeyModifierTypeCtrl = _blcelc.csKeyModifierTypeCtrl
csKeyModifierTypeAlt = _blcelc.csKeyModifierTypeAlt
csKeyModifierTypeCapsLock = _blcelc.csKeyModifierTypeCapsLock
csKeyModifierTypeNumLock = _blcelc.csKeyModifierTypeNumLock
csKeyModifierTypeScrollLock = _blcelc.csKeyModifierTypeScrollLock
csKeyModifierTypeLast = _blcelc.csKeyModifierTypeLast
csKeyModifierNumLeft = _blcelc.csKeyModifierNumLeft
csKeyModifierNumRight = _blcelc.csKeyModifierNumRight
csKeyModifierNumAny = _blcelc.csKeyModifierNumAny
class csKeyModifiers(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csKeyModifiers, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csKeyModifiers, name)
    def __repr__(self):
        return "<C csKeyModifiers instance at %s>" % (self.this,)
    __swig_setmethods__["modifiers"] = _blcelc.csKeyModifiers_modifiers_set
    __swig_getmethods__["modifiers"] = _blcelc.csKeyModifiers_modifiers_get
    if _newclass:modifiers = property(_blcelc.csKeyModifiers_modifiers_get, _blcelc.csKeyModifiers_modifiers_set)
    def __init__(self, *args):
        _swig_setattr(self, csKeyModifiers, 'this', _blcelc.new_csKeyModifiers(*args))
        _swig_setattr(self, csKeyModifiers, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csKeyModifiers):
        try:
            if self.thisown: destroy(self)
        except: pass

class csKeyModifiersPtr(csKeyModifiers):
    def __init__(self, this):
        _swig_setattr(self, csKeyModifiers, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csKeyModifiers, 'thisown', 0)
        _swig_setattr(self, csKeyModifiers,self.__class__,csKeyModifiers)
_blcelc.csKeyModifiers_swigregister(csKeyModifiersPtr)

CSKEY_ESC = _blcelc.CSKEY_ESC
CSKEY_ENTER = _blcelc.CSKEY_ENTER
CSKEY_TAB = _blcelc.CSKEY_TAB
CSKEY_BACKSPACE = _blcelc.CSKEY_BACKSPACE
CSKEY_SPACE = _blcelc.CSKEY_SPACE
CSKEY_SPECIAL_FIRST = _blcelc.CSKEY_SPECIAL_FIRST
CSKEY_SPECIAL_LAST = _blcelc.CSKEY_SPECIAL_LAST
CSKEY_UP = _blcelc.CSKEY_UP
CSKEY_DOWN = _blcelc.CSKEY_DOWN
CSKEY_LEFT = _blcelc.CSKEY_LEFT
CSKEY_RIGHT = _blcelc.CSKEY_RIGHT
CSKEY_PGUP = _blcelc.CSKEY_PGUP
CSKEY_PGDN = _blcelc.CSKEY_PGDN
CSKEY_HOME = _blcelc.CSKEY_HOME
CSKEY_END = _blcelc.CSKEY_END
CSKEY_INS = _blcelc.CSKEY_INS
CSKEY_DEL = _blcelc.CSKEY_DEL
CSKEY_CONTEXT = _blcelc.CSKEY_CONTEXT
CSKEY_PRINTSCREEN = _blcelc.CSKEY_PRINTSCREEN
CSKEY_PAUSE = _blcelc.CSKEY_PAUSE
CSKEY_F1 = _blcelc.CSKEY_F1
CSKEY_F2 = _blcelc.CSKEY_F2
CSKEY_F3 = _blcelc.CSKEY_F3
CSKEY_F4 = _blcelc.CSKEY_F4
CSKEY_F5 = _blcelc.CSKEY_F5
CSKEY_F6 = _blcelc.CSKEY_F6
CSKEY_F7 = _blcelc.CSKEY_F7
CSKEY_F8 = _blcelc.CSKEY_F8
CSKEY_F9 = _blcelc.CSKEY_F9
CSKEY_F10 = _blcelc.CSKEY_F10
CSKEY_F11 = _blcelc.CSKEY_F11
CSKEY_F12 = _blcelc.CSKEY_F12
CSKEY_MODIFIER_FIRST = _blcelc.CSKEY_MODIFIER_FIRST
CSKEY_MODIFIER_LAST = _blcelc.CSKEY_MODIFIER_LAST
CSKEY_MODIFIERTYPE_SHIFT = _blcelc.CSKEY_MODIFIERTYPE_SHIFT
CSKEY_PAD_FLAG = _blcelc.CSKEY_PAD_FLAG
csKeyCharTypeNormal = _blcelc.csKeyCharTypeNormal
csKeyCharTypeDead = _blcelc.csKeyCharTypeDead
CSEVTYPE_Keyboard = _blcelc.CSEVTYPE_Keyboard
CSEVTYPE_Mouse = _blcelc.CSEVTYPE_Mouse
CSEVTYPE_Joystick = _blcelc.CSEVTYPE_Joystick
cscmdNothing = _blcelc.cscmdNothing
cscmdQuit = _blcelc.cscmdQuit
cscmdFocusChanged = _blcelc.cscmdFocusChanged
cscmdSystemOpen = _blcelc.cscmdSystemOpen
cscmdSystemClose = _blcelc.cscmdSystemClose
cscmdContextResize = _blcelc.cscmdContextResize
cscmdContextClose = _blcelc.cscmdContextClose
cscmdCommandLineHelp = _blcelc.cscmdCommandLineHelp
cscmdPreProcess = _blcelc.cscmdPreProcess
cscmdProcess = _blcelc.cscmdProcess
cscmdPostProcess = _blcelc.cscmdPostProcess
cscmdFinalProcess = _blcelc.cscmdFinalProcess
cscmdCanvasHidden = _blcelc.cscmdCanvasHidden
cscmdCanvasExposed = _blcelc.cscmdCanvasExposed
class iEventQueue(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEventQueue, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEventQueue, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventQueue instance at %s>" % (self.this,)
    def Process(*args): return _blcelc.iEventQueue_Process(*args)
    def Dispatch(*args): return _blcelc.iEventQueue_Dispatch(*args)
    def RegisterListener(*args): return _blcelc.iEventQueue_RegisterListener(*args)
    def RemoveListener(*args): return _blcelc.iEventQueue_RemoveListener(*args)
    def ChangeListenerTrigger(*args): return _blcelc.iEventQueue_ChangeListenerTrigger(*args)
    def CreateEventOutlet(*args): return _blcelc.iEventQueue_CreateEventOutlet(*args)
    def GetEventOutlet(*args): return _blcelc.iEventQueue_GetEventOutlet(*args)
    def GetEventCord(*args): return _blcelc.iEventQueue_GetEventCord(*args)
    def CreateEvent(*args): return _blcelc.iEventQueue_CreateEvent(*args)
    def Post(*args): return _blcelc.iEventQueue_Post(*args)
    def Get(*args): return _blcelc.iEventQueue_Get(*args)
    def Clear(*args): return _blcelc.iEventQueue_Clear(*args)
    def IsEmpty(*args): return _blcelc.iEventQueue_IsEmpty(*args)
    def RemoveAllListeners(*args): return _blcelc.iEventQueue_RemoveAllListeners(*args)
    def __del__(self, destroy=_blcelc.delete_iEventQueue):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iEventQueue_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iEventQueue_scfGetVersion)

class iEventQueuePtr(iEventQueue):
    def __init__(self, this):
        _swig_setattr(self, iEventQueue, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEventQueue, 'thisown', 0)
        _swig_setattr(self, iEventQueue,self.__class__,iEventQueue)
_blcelc.iEventQueue_swigregister(iEventQueuePtr)

iEventQueue_scfGetVersion = _blcelc.iEventQueue_scfGetVersion

class iEventHandler(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEventHandler, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEventHandler, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventHandler instance at %s>" % (self.this,)
    def HandleEvent(*args): return _blcelc.iEventHandler_HandleEvent(*args)
    def __del__(self, destroy=_blcelc.delete_iEventHandler):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iEventHandler_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iEventHandler_scfGetVersion)

class iEventHandlerPtr(iEventHandler):
    def __init__(self, this):
        _swig_setattr(self, iEventHandler, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEventHandler, 'thisown', 0)
        _swig_setattr(self, iEventHandler,self.__class__,iEventHandler)
_blcelc.iEventHandler_swigregister(iEventHandlerPtr)

iEventHandler_scfGetVersion = _blcelc.iEventHandler_scfGetVersion

class iPluginIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPluginIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPluginIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPluginIterator instance at %s>" % (self.this,)
    def HasNext(*args): return _blcelc.iPluginIterator_HasNext(*args)
    def Next(*args): return _blcelc.iPluginIterator_Next(*args)
    def __del__(self, destroy=_blcelc.delete_iPluginIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class iPluginIteratorPtr(iPluginIterator):
    def __init__(self, this):
        _swig_setattr(self, iPluginIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPluginIterator, 'thisown', 0)
        _swig_setattr(self, iPluginIterator,self.__class__,iPluginIterator)
_blcelc.iPluginIterator_swigregister(iPluginIteratorPtr)

class iPluginManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPluginManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPluginManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPluginManager instance at %s>" % (self.this,)
    def LoadPlugin(*args): return _blcelc.iPluginManager_LoadPlugin(*args)
    def QueryPlugin(*args): return _blcelc.iPluginManager_QueryPlugin(*args)
    def UnloadPlugin(*args): return _blcelc.iPluginManager_UnloadPlugin(*args)
    def RegisterPlugin(*args): return _blcelc.iPluginManager_RegisterPlugin(*args)
    def GetPlugins(*args): return _blcelc.iPluginManager_GetPlugins(*args)
    def Clear(*args): return _blcelc.iPluginManager_Clear(*args)
    def QueryOptions(*args): return _blcelc.iPluginManager_QueryOptions(*args)
    def __del__(self, destroy=_blcelc.delete_iPluginManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iPluginManager_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iPluginManager_scfGetVersion)

class iPluginManagerPtr(iPluginManager):
    def __init__(self, this):
        _swig_setattr(self, iPluginManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPluginManager, 'thisown', 0)
        _swig_setattr(self, iPluginManager,self.__class__,iPluginManager)
_blcelc.iPluginManager_swigregister(iPluginManagerPtr)

iPluginManager_scfGetVersion = _blcelc.iPluginManager_scfGetVersion

CS_MAX_MOUSE_BUTTONS = _blcelc.CS_MAX_MOUSE_BUTTONS
CS_MAX_JOYSTICK_COUNT = _blcelc.CS_MAX_JOYSTICK_COUNT
CS_MAX_JOYSTICK_BUTTONS = _blcelc.CS_MAX_JOYSTICK_BUTTONS
csComposeNoChar = _blcelc.csComposeNoChar
csComposeNormalChar = _blcelc.csComposeNormalChar
csComposeComposedChar = _blcelc.csComposeComposedChar
csComposeUncomposeable = _blcelc.csComposeUncomposeable
class iKeyComposer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iKeyComposer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iKeyComposer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iKeyComposer instance at %s>" % (self.this,)
    def HandleKey(*args): return _blcelc.iKeyComposer_HandleKey(*args)
    def ResetState(*args): return _blcelc.iKeyComposer_ResetState(*args)
    def __del__(self, destroy=_blcelc.delete_iKeyComposer):
        try:
            if self.thisown: destroy(self)
        except: pass

class iKeyComposerPtr(iKeyComposer):
    def __init__(self, this):
        _swig_setattr(self, iKeyComposer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iKeyComposer, 'thisown', 0)
        _swig_setattr(self, iKeyComposer,self.__class__,iKeyComposer)
_blcelc.iKeyComposer_swigregister(iKeyComposerPtr)

class iKeyboardDriver(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iKeyboardDriver, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iKeyboardDriver, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iKeyboardDriver instance at %s>" % (self.this,)
    def Reset(*args): return _blcelc.iKeyboardDriver_Reset(*args)
    def DoKey(*args): return _blcelc.iKeyboardDriver_DoKey(*args)
    def GetModifierState(*args): return _blcelc.iKeyboardDriver_GetModifierState(*args)
    def CreateKeyComposer(*args): return _blcelc.iKeyboardDriver_CreateKeyComposer(*args)
    def SynthesizeCooked(*args): return _blcelc.iKeyboardDriver_SynthesizeCooked(*args)
    def __del__(self, destroy=_blcelc.delete_iKeyboardDriver):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iKeyboardDriver_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iKeyboardDriver_scfGetVersion)
    def GetKeyState(*args): return _blcelc.iKeyboardDriver_GetKeyState(*args)

class iKeyboardDriverPtr(iKeyboardDriver):
    def __init__(self, this):
        _swig_setattr(self, iKeyboardDriver, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iKeyboardDriver, 'thisown', 0)
        _swig_setattr(self, iKeyboardDriver,self.__class__,iKeyboardDriver)
_blcelc.iKeyboardDriver_swigregister(iKeyboardDriverPtr)

iKeyboardDriver_scfGetVersion = _blcelc.iKeyboardDriver_scfGetVersion

class iMouseDriver(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMouseDriver, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMouseDriver, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMouseDriver instance at %s>" % (self.this,)
    def SetDoubleClickTime(*args): return _blcelc.iMouseDriver_SetDoubleClickTime(*args)
    def Reset(*args): return _blcelc.iMouseDriver_Reset(*args)
    def GetLastX(*args): return _blcelc.iMouseDriver_GetLastX(*args)
    def GetLastY(*args): return _blcelc.iMouseDriver_GetLastY(*args)
    def GetLastButton(*args): return _blcelc.iMouseDriver_GetLastButton(*args)
    def DoButton(*args): return _blcelc.iMouseDriver_DoButton(*args)
    def DoMotion(*args): return _blcelc.iMouseDriver_DoMotion(*args)
    def __del__(self, destroy=_blcelc.delete_iMouseDriver):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMouseDriver_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMouseDriver_scfGetVersion)

class iMouseDriverPtr(iMouseDriver):
    def __init__(self, this):
        _swig_setattr(self, iMouseDriver, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMouseDriver, 'thisown', 0)
        _swig_setattr(self, iMouseDriver,self.__class__,iMouseDriver)
_blcelc.iMouseDriver_swigregister(iMouseDriverPtr)

iMouseDriver_scfGetVersion = _blcelc.iMouseDriver_scfGetVersion

class iJoystickDriver(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iJoystickDriver, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iJoystickDriver, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iJoystickDriver instance at %s>" % (self.this,)
    def Reset(*args): return _blcelc.iJoystickDriver_Reset(*args)
    def GetLastX(*args): return _blcelc.iJoystickDriver_GetLastX(*args)
    def GetLastY(*args): return _blcelc.iJoystickDriver_GetLastY(*args)
    def GetLastButton(*args): return _blcelc.iJoystickDriver_GetLastButton(*args)
    def DoButton(*args): return _blcelc.iJoystickDriver_DoButton(*args)
    def DoMotion(*args): return _blcelc.iJoystickDriver_DoMotion(*args)
    def __del__(self, destroy=_blcelc.delete_iJoystickDriver):
        try:
            if self.thisown: destroy(self)
        except: pass

class iJoystickDriverPtr(iJoystickDriver):
    def __init__(self, this):
        _swig_setattr(self, iJoystickDriver, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iJoystickDriver, 'thisown', 0)
        _swig_setattr(self, iJoystickDriver,self.__class__,iJoystickDriver)
_blcelc.iJoystickDriver_swigregister(iJoystickDriverPtr)

class iConfigFile(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iConfigFile, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iConfigFile, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConfigFile instance at %s>" % (self.this,)
    def GetFileName(*args): return _blcelc.iConfigFile_GetFileName(*args)
    def GetVFS(*args): return _blcelc.iConfigFile_GetVFS(*args)
    def SetFileName(*args): return _blcelc.iConfigFile_SetFileName(*args)
    def Load(*args): return _blcelc.iConfigFile_Load(*args)
    def Save(*args): return _blcelc.iConfigFile_Save(*args)
    def Clear(*args): return _blcelc.iConfigFile_Clear(*args)
    def Enumerate(*args): return _blcelc.iConfigFile_Enumerate(*args)
    def KeyExists(*args): return _blcelc.iConfigFile_KeyExists(*args)
    def SubsectionExists(*args): return _blcelc.iConfigFile_SubsectionExists(*args)
    def GetInt(*args): return _blcelc.iConfigFile_GetInt(*args)
    def GetFloat(*args): return _blcelc.iConfigFile_GetFloat(*args)
    def GetStr(*args): return _blcelc.iConfigFile_GetStr(*args)
    def GetBool(*args): return _blcelc.iConfigFile_GetBool(*args)
    def GetComment(*args): return _blcelc.iConfigFile_GetComment(*args)
    def SetStr(*args): return _blcelc.iConfigFile_SetStr(*args)
    def SetInt(*args): return _blcelc.iConfigFile_SetInt(*args)
    def SetFloat(*args): return _blcelc.iConfigFile_SetFloat(*args)
    def SetBool(*args): return _blcelc.iConfigFile_SetBool(*args)
    def SetComment(*args): return _blcelc.iConfigFile_SetComment(*args)
    def DeleteKey(*args): return _blcelc.iConfigFile_DeleteKey(*args)
    def GetEOFComment(*args): return _blcelc.iConfigFile_GetEOFComment(*args)
    def SetEOFComment(*args): return _blcelc.iConfigFile_SetEOFComment(*args)
    def __del__(self, destroy=_blcelc.delete_iConfigFile):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iConfigFile_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iConfigFile_scfGetVersion)

class iConfigFilePtr(iConfigFile):
    def __init__(self, this):
        _swig_setattr(self, iConfigFile, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iConfigFile, 'thisown', 0)
        _swig_setattr(self, iConfigFile,self.__class__,iConfigFile)
_blcelc.iConfigFile_swigregister(iConfigFilePtr)

iConfigFile_scfGetVersion = _blcelc.iConfigFile_scfGetVersion

class iConfigIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iConfigIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iConfigIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConfigIterator instance at %s>" % (self.this,)
    def GetConfigFile(*args): return _blcelc.iConfigIterator_GetConfigFile(*args)
    def GetSubsection(*args): return _blcelc.iConfigIterator_GetSubsection(*args)
    def Rewind(*args): return _blcelc.iConfigIterator_Rewind(*args)
    def Next(*args): return _blcelc.iConfigIterator_Next(*args)
    def GetKey(*args): return _blcelc.iConfigIterator_GetKey(*args)
    def GetInt(*args): return _blcelc.iConfigIterator_GetInt(*args)
    def GetFloat(*args): return _blcelc.iConfigIterator_GetFloat(*args)
    def GetStr(*args): return _blcelc.iConfigIterator_GetStr(*args)
    def GetBool(*args): return _blcelc.iConfigIterator_GetBool(*args)
    def GetComment(*args): return _blcelc.iConfigIterator_GetComment(*args)
    def __del__(self, destroy=_blcelc.delete_iConfigIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iConfigIterator_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iConfigIterator_scfGetVersion)

class iConfigIteratorPtr(iConfigIterator):
    def __init__(self, this):
        _swig_setattr(self, iConfigIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iConfigIterator, 'thisown', 0)
        _swig_setattr(self, iConfigIterator,self.__class__,iConfigIterator)
_blcelc.iConfigIterator_swigregister(iConfigIteratorPtr)

iConfigIterator_scfGetVersion = _blcelc.iConfigIterator_scfGetVersion

class iConfigManager(iConfigFile):
    __swig_setmethods__ = {}
    for _s in [iConfigFile]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iConfigManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iConfigFile]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iConfigManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConfigManager instance at %s>" % (self.this,)
    PriorityMin = _blcelc.iConfigManager_PriorityMin
    PriorityVeryLow = _blcelc.iConfigManager_PriorityVeryLow
    PriorityLow = _blcelc.iConfigManager_PriorityLow
    PriorityMedium = _blcelc.iConfigManager_PriorityMedium
    PriorityHigh = _blcelc.iConfigManager_PriorityHigh
    PriorityVeryHigh = _blcelc.iConfigManager_PriorityVeryHigh
    PriorityMax = _blcelc.iConfigManager_PriorityMax
    ConfigPriorityPlugin = _blcelc.iConfigManager_ConfigPriorityPlugin
    ConfigPriorityApplication = _blcelc.iConfigManager_ConfigPriorityApplication
    ConfigPriorityUserGlobal = _blcelc.iConfigManager_ConfigPriorityUserGlobal
    ConfigPriorityUserApp = _blcelc.iConfigManager_ConfigPriorityUserApp
    ConfigPriorityCmdLine = _blcelc.iConfigManager_ConfigPriorityCmdLine
    def AddDomain(*args): return _blcelc.iConfigManager_AddDomain(*args)
    def RemoveDomain(*args): return _blcelc.iConfigManager_RemoveDomain(*args)
    def LookupDomain(*args): return _blcelc.iConfigManager_LookupDomain(*args)
    def SetDomainPriority(*args): return _blcelc.iConfigManager_SetDomainPriority(*args)
    def GetDomainPriority(*args): return _blcelc.iConfigManager_GetDomainPriority(*args)
    def SetDynamicDomain(*args): return _blcelc.iConfigManager_SetDynamicDomain(*args)
    def GetDynamicDomain(*args): return _blcelc.iConfigManager_GetDynamicDomain(*args)
    def SetDynamicDomainPriority(*args): return _blcelc.iConfigManager_SetDynamicDomainPriority(*args)
    def GetDynamicDomainPriority(*args): return _blcelc.iConfigManager_GetDynamicDomainPriority(*args)
    def FlushRemoved(*args): return _blcelc.iConfigManager_FlushRemoved(*args)
    def __del__(self, destroy=_blcelc.delete_iConfigManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iConfigManager_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iConfigManager_scfGetVersion)

class iConfigManagerPtr(iConfigManager):
    def __init__(self, this):
        _swig_setattr(self, iConfigManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iConfigManager, 'thisown', 0)
        _swig_setattr(self, iConfigManager,self.__class__,iConfigManager)
_blcelc.iConfigManager_swigregister(iConfigManagerPtr)

iConfigManager_scfGetVersion = _blcelc.iConfigManager_scfGetVersion

class iStringArray(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iStringArray, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iStringArray, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStringArray instance at %s>" % (self.this,)
    def Length(*args): return _blcelc.iStringArray_Length(*args)
    def Push(*args): return _blcelc.iStringArray_Push(*args)
    def Pop(*args): return _blcelc.iStringArray_Pop(*args)
    def Get(*args): return _blcelc.iStringArray_Get(*args)
    def Find(*args): return _blcelc.iStringArray_Find(*args)
    def FindCaseInsensitive(*args): return _blcelc.iStringArray_FindCaseInsensitive(*args)
    def FindSortedKey(*args): return _blcelc.iStringArray_FindSortedKey(*args)
    def Sort(*args): return _blcelc.iStringArray_Sort(*args)
    def DeleteIndex(*args): return _blcelc.iStringArray_DeleteIndex(*args)
    def Insert(*args): return _blcelc.iStringArray_Insert(*args)
    def DeleteAll(*args): return _blcelc.iStringArray_DeleteAll(*args)
    def __del__(self, destroy=_blcelc.delete_iStringArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStringArray_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStringArray_scfGetVersion)

class iStringArrayPtr(iStringArray):
    def __init__(self, this):
        _swig_setattr(self, iStringArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iStringArray, 'thisown', 0)
        _swig_setattr(self, iStringArray,self.__class__,iStringArray)
_blcelc.iStringArray_swigregister(iStringArrayPtr)

iStringArray_scfGetVersion = _blcelc.iStringArray_scfGetVersion

CS_NODE_DOCUMENT = _blcelc.CS_NODE_DOCUMENT
CS_NODE_ELEMENT = _blcelc.CS_NODE_ELEMENT
CS_NODE_COMMENT = _blcelc.CS_NODE_COMMENT
CS_NODE_UNKNOWN = _blcelc.CS_NODE_UNKNOWN
CS_NODE_TEXT = _blcelc.CS_NODE_TEXT
CS_NODE_DECLARATION = _blcelc.CS_NODE_DECLARATION
CS_CHANGEABLE_NEVER = _blcelc.CS_CHANGEABLE_NEVER
CS_CHANGEABLE_NEWROOT = _blcelc.CS_CHANGEABLE_NEWROOT
CS_CHANGEABLE_YES = _blcelc.CS_CHANGEABLE_YES
class iDocumentAttributeIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDocumentAttributeIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDocumentAttributeIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentAttributeIterator instance at %s>" % (self.this,)
    def HasNext(*args): return _blcelc.iDocumentAttributeIterator_HasNext(*args)
    def Next(*args): return _blcelc.iDocumentAttributeIterator_Next(*args)
    def __del__(self, destroy=_blcelc.delete_iDocumentAttributeIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class iDocumentAttributeIteratorPtr(iDocumentAttributeIterator):
    def __init__(self, this):
        _swig_setattr(self, iDocumentAttributeIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDocumentAttributeIterator, 'thisown', 0)
        _swig_setattr(self, iDocumentAttributeIterator,self.__class__,iDocumentAttributeIterator)
_blcelc.iDocumentAttributeIterator_swigregister(iDocumentAttributeIteratorPtr)

class iDocumentAttribute(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDocumentAttribute, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDocumentAttribute, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentAttribute instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iDocumentAttribute_GetName(*args)
    def GetValue(*args): return _blcelc.iDocumentAttribute_GetValue(*args)
    def GetValueAsInt(*args): return _blcelc.iDocumentAttribute_GetValueAsInt(*args)
    def GetValueAsFloat(*args): return _blcelc.iDocumentAttribute_GetValueAsFloat(*args)
    def GetValueAsBool(*args): return _blcelc.iDocumentAttribute_GetValueAsBool(*args)
    def SetName(*args): return _blcelc.iDocumentAttribute_SetName(*args)
    def SetValue(*args): return _blcelc.iDocumentAttribute_SetValue(*args)
    def SetValueAsInt(*args): return _blcelc.iDocumentAttribute_SetValueAsInt(*args)
    def SetValueAsFloat(*args): return _blcelc.iDocumentAttribute_SetValueAsFloat(*args)
    def __del__(self, destroy=_blcelc.delete_iDocumentAttribute):
        try:
            if self.thisown: destroy(self)
        except: pass

class iDocumentAttributePtr(iDocumentAttribute):
    def __init__(self, this):
        _swig_setattr(self, iDocumentAttribute, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDocumentAttribute, 'thisown', 0)
        _swig_setattr(self, iDocumentAttribute,self.__class__,iDocumentAttribute)
_blcelc.iDocumentAttribute_swigregister(iDocumentAttributePtr)

class iDocumentNodeIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDocumentNodeIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDocumentNodeIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentNodeIterator instance at %s>" % (self.this,)
    def HasNext(*args): return _blcelc.iDocumentNodeIterator_HasNext(*args)
    def Next(*args): return _blcelc.iDocumentNodeIterator_Next(*args)
    def __del__(self, destroy=_blcelc.delete_iDocumentNodeIterator):
        try:
            if self.thisown: destroy(self)
        except: pass

class iDocumentNodeIteratorPtr(iDocumentNodeIterator):
    def __init__(self, this):
        _swig_setattr(self, iDocumentNodeIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDocumentNodeIterator, 'thisown', 0)
        _swig_setattr(self, iDocumentNodeIterator,self.__class__,iDocumentNodeIterator)
_blcelc.iDocumentNodeIterator_swigregister(iDocumentNodeIteratorPtr)

class iDocumentNode(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDocumentNode, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDocumentNode, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentNode instance at %s>" % (self.this,)
    def GetType(*args): return _blcelc.iDocumentNode_GetType(*args)
    def Equals(*args): return _blcelc.iDocumentNode_Equals(*args)
    def GetValue(*args): return _blcelc.iDocumentNode_GetValue(*args)
    def SetValue(*args): return _blcelc.iDocumentNode_SetValue(*args)
    def SetValueAsInt(*args): return _blcelc.iDocumentNode_SetValueAsInt(*args)
    def SetValueAsFloat(*args): return _blcelc.iDocumentNode_SetValueAsFloat(*args)
    def GetParent(*args): return _blcelc.iDocumentNode_GetParent(*args)
    def GetNodes(*args): return _blcelc.iDocumentNode_GetNodes(*args)
    def GetNode(*args): return _blcelc.iDocumentNode_GetNode(*args)
    def RemoveNode(*args): return _blcelc.iDocumentNode_RemoveNode(*args)
    def RemoveNodes(*args): return _blcelc.iDocumentNode_RemoveNodes(*args)
    def CreateNodeBefore(*args): return _blcelc.iDocumentNode_CreateNodeBefore(*args)
    def GetContentsValue(*args): return _blcelc.iDocumentNode_GetContentsValue(*args)
    def GetContentsValueAsInt(*args): return _blcelc.iDocumentNode_GetContentsValueAsInt(*args)
    def GetContentsValueAsFloat(*args): return _blcelc.iDocumentNode_GetContentsValueAsFloat(*args)
    def GetAttributes(*args): return _blcelc.iDocumentNode_GetAttributes(*args)
    def GetAttribute(*args): return _blcelc.iDocumentNode_GetAttribute(*args)
    def GetAttributeValue(*args): return _blcelc.iDocumentNode_GetAttributeValue(*args)
    def GetAttributeValueAsInt(*args): return _blcelc.iDocumentNode_GetAttributeValueAsInt(*args)
    def GetAttributeValueAsFloat(*args): return _blcelc.iDocumentNode_GetAttributeValueAsFloat(*args)
    def GetAttributeValueAsBool(*args): return _blcelc.iDocumentNode_GetAttributeValueAsBool(*args)
    def RemoveAttribute(*args): return _blcelc.iDocumentNode_RemoveAttribute(*args)
    def RemoveAttributes(*args): return _blcelc.iDocumentNode_RemoveAttributes(*args)
    def SetAttribute(*args): return _blcelc.iDocumentNode_SetAttribute(*args)
    def SetAttributeAsInt(*args): return _blcelc.iDocumentNode_SetAttributeAsInt(*args)
    def SetAttributeAsFloat(*args): return _blcelc.iDocumentNode_SetAttributeAsFloat(*args)
    def __del__(self, destroy=_blcelc.delete_iDocumentNode):
        try:
            if self.thisown: destroy(self)
        except: pass

class iDocumentNodePtr(iDocumentNode):
    def __init__(self, this):
        _swig_setattr(self, iDocumentNode, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDocumentNode, 'thisown', 0)
        _swig_setattr(self, iDocumentNode,self.__class__,iDocumentNode)
_blcelc.iDocumentNode_swigregister(iDocumentNodePtr)

class iDocument(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDocument, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDocument, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocument instance at %s>" % (self.this,)
    def Clear(*args): return _blcelc.iDocument_Clear(*args)
    def CreateRoot(*args): return _blcelc.iDocument_CreateRoot(*args)
    def GetRoot(*args): return _blcelc.iDocument_GetRoot(*args)
    def Parse(*args): return _blcelc.iDocument_Parse(*args)
    def Write(*args): return _blcelc.iDocument_Write(*args)
    def Changeable(*args): return _blcelc.iDocument_Changeable(*args)
    def __del__(self, destroy=_blcelc.delete_iDocument):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDocument_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDocument_scfGetVersion)

class iDocumentPtr(iDocument):
    def __init__(self, this):
        _swig_setattr(self, iDocument, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDocument, 'thisown', 0)
        _swig_setattr(self, iDocument,self.__class__,iDocument)
_blcelc.iDocument_swigregister(iDocumentPtr)

iDocument_scfGetVersion = _blcelc.iDocument_scfGetVersion

class iDocumentSystem(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDocumentSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDocumentSystem, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentSystem instance at %s>" % (self.this,)
    def CreateDocument(*args): return _blcelc.iDocumentSystem_CreateDocument(*args)
    def __del__(self, destroy=_blcelc.delete_iDocumentSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDocumentSystem_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDocumentSystem_scfGetVersion)

class iDocumentSystemPtr(iDocumentSystem):
    def __init__(self, this):
        _swig_setattr(self, iDocumentSystem, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDocumentSystem, 'thisown', 0)
        _swig_setattr(self, iDocumentSystem,self.__class__,iDocumentSystem)
_blcelc.iDocumentSystem_swigregister(iDocumentSystemPtr)

iDocumentSystem_scfGetVersion = _blcelc.iDocumentSystem_scfGetVersion

class csTinyDocumentSystem(iDocumentSystem):
    __swig_setmethods__ = {}
    for _s in [iDocumentSystem]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csTinyDocumentSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [iDocumentSystem]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csTinyDocumentSystem, name)
    def __repr__(self):
        return "<C csTinyDocumentSystem instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csTinyDocumentSystem, 'this', _blcelc.new_csTinyDocumentSystem(*args))
        _swig_setattr(self, csTinyDocumentSystem, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csTinyDocumentSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_setmethods__["scfRefCount"] = _blcelc.csTinyDocumentSystem_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csTinyDocumentSystem_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csTinyDocumentSystem_scfRefCount_get, _blcelc.csTinyDocumentSystem_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csTinyDocumentSystem_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csTinyDocumentSystem_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csTinyDocumentSystem_scfWeakRefOwners_get, _blcelc.csTinyDocumentSystem_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc.csTinyDocumentSystem_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc.csTinyDocumentSystem_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csTinyDocumentSystem_scfParent_get
    if _newclass:scfParent = property(_blcelc.csTinyDocumentSystem_scfParent_get, _blcelc.csTinyDocumentSystem_scfParent_set)
    def IncRef(*args): return _blcelc.csTinyDocumentSystem_IncRef(*args)
    def DecRef(*args): return _blcelc.csTinyDocumentSystem_DecRef(*args)
    def GetRefCount(*args): return _blcelc.csTinyDocumentSystem_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.csTinyDocumentSystem_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.csTinyDocumentSystem_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.csTinyDocumentSystem_QueryInterface(*args)
    def CreateDocument(*args): return _blcelc.csTinyDocumentSystem_CreateDocument(*args)

class csTinyDocumentSystemPtr(csTinyDocumentSystem):
    def __init__(self, this):
        _swig_setattr(self, csTinyDocumentSystem, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csTinyDocumentSystem, 'thisown', 0)
        _swig_setattr(self, csTinyDocumentSystem,self.__class__,csTinyDocumentSystem)
_blcelc.csTinyDocumentSystem_swigregister(csTinyDocumentSystemPtr)

class iDataBuffer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDataBuffer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDataBuffer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDataBuffer instance at %s>" % (self.this,)
    def GetSize(*args): return _blcelc.iDataBuffer_GetSize(*args)
    def GetData(*args): return _blcelc.iDataBuffer_GetData(*args)
    def asString(*args): return _blcelc.iDataBuffer_asString(*args)
    def GetUint8(*args): return _blcelc.iDataBuffer_GetUint8(*args)
    def __del__(self, destroy=_blcelc.delete_iDataBuffer):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDataBuffer_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDataBuffer_scfGetVersion)

class iDataBufferPtr(iDataBuffer):
    def __init__(self, this):
        _swig_setattr(self, iDataBuffer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDataBuffer, 'thisown', 0)
        _swig_setattr(self, iDataBuffer,self.__class__,iDataBuffer)
_blcelc.iDataBuffer_swigregister(iDataBufferPtr)

iDataBuffer_scfGetVersion = _blcelc.iDataBuffer_scfGetVersion

CS_WRITE_BASELINE = _blcelc.CS_WRITE_BASELINE
CS_WRITE_NOANTIALIAS = _blcelc.CS_WRITE_NOANTIALIAS
class csPixelCoord(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPixelCoord, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPixelCoord, name)
    def __repr__(self):
        return "<C csPixelCoord instance at %s>" % (self.this,)
    __swig_setmethods__["x"] = _blcelc.csPixelCoord_x_set
    __swig_getmethods__["x"] = _blcelc.csPixelCoord_x_get
    if _newclass:x = property(_blcelc.csPixelCoord_x_get, _blcelc.csPixelCoord_x_set)
    __swig_setmethods__["y"] = _blcelc.csPixelCoord_y_set
    __swig_getmethods__["y"] = _blcelc.csPixelCoord_y_get
    if _newclass:y = property(_blcelc.csPixelCoord_y_get, _blcelc.csPixelCoord_y_set)
    def __init__(self, *args):
        _swig_setattr(self, csPixelCoord, 'this', _blcelc.new_csPixelCoord(*args))
        _swig_setattr(self, csPixelCoord, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csPixelCoord):
        try:
            if self.thisown: destroy(self)
        except: pass

class csPixelCoordPtr(csPixelCoord):
    def __init__(self, this):
        _swig_setattr(self, csPixelCoord, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPixelCoord, 'thisown', 0)
        _swig_setattr(self, csPixelCoord,self.__class__,csPixelCoord)
_blcelc.csPixelCoord_swigregister(csPixelCoordPtr)

class csPixelFormat(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPixelFormat, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPixelFormat, name)
    def __repr__(self):
        return "<C csPixelFormat instance at %s>" % (self.this,)
    __swig_setmethods__["RedMask"] = _blcelc.csPixelFormat_RedMask_set
    __swig_getmethods__["RedMask"] = _blcelc.csPixelFormat_RedMask_get
    if _newclass:RedMask = property(_blcelc.csPixelFormat_RedMask_get, _blcelc.csPixelFormat_RedMask_set)
    __swig_setmethods__["GreenMask"] = _blcelc.csPixelFormat_GreenMask_set
    __swig_getmethods__["GreenMask"] = _blcelc.csPixelFormat_GreenMask_get
    if _newclass:GreenMask = property(_blcelc.csPixelFormat_GreenMask_get, _blcelc.csPixelFormat_GreenMask_set)
    __swig_setmethods__["BlueMask"] = _blcelc.csPixelFormat_BlueMask_set
    __swig_getmethods__["BlueMask"] = _blcelc.csPixelFormat_BlueMask_get
    if _newclass:BlueMask = property(_blcelc.csPixelFormat_BlueMask_get, _blcelc.csPixelFormat_BlueMask_set)
    __swig_setmethods__["AlphaMask"] = _blcelc.csPixelFormat_AlphaMask_set
    __swig_getmethods__["AlphaMask"] = _blcelc.csPixelFormat_AlphaMask_get
    if _newclass:AlphaMask = property(_blcelc.csPixelFormat_AlphaMask_get, _blcelc.csPixelFormat_AlphaMask_set)
    __swig_setmethods__["RedShift"] = _blcelc.csPixelFormat_RedShift_set
    __swig_getmethods__["RedShift"] = _blcelc.csPixelFormat_RedShift_get
    if _newclass:RedShift = property(_blcelc.csPixelFormat_RedShift_get, _blcelc.csPixelFormat_RedShift_set)
    __swig_setmethods__["GreenShift"] = _blcelc.csPixelFormat_GreenShift_set
    __swig_getmethods__["GreenShift"] = _blcelc.csPixelFormat_GreenShift_get
    if _newclass:GreenShift = property(_blcelc.csPixelFormat_GreenShift_get, _blcelc.csPixelFormat_GreenShift_set)
    __swig_setmethods__["BlueShift"] = _blcelc.csPixelFormat_BlueShift_set
    __swig_getmethods__["BlueShift"] = _blcelc.csPixelFormat_BlueShift_get
    if _newclass:BlueShift = property(_blcelc.csPixelFormat_BlueShift_get, _blcelc.csPixelFormat_BlueShift_set)
    __swig_setmethods__["AlphaShift"] = _blcelc.csPixelFormat_AlphaShift_set
    __swig_getmethods__["AlphaShift"] = _blcelc.csPixelFormat_AlphaShift_get
    if _newclass:AlphaShift = property(_blcelc.csPixelFormat_AlphaShift_get, _blcelc.csPixelFormat_AlphaShift_set)
    __swig_setmethods__["RedBits"] = _blcelc.csPixelFormat_RedBits_set
    __swig_getmethods__["RedBits"] = _blcelc.csPixelFormat_RedBits_get
    if _newclass:RedBits = property(_blcelc.csPixelFormat_RedBits_get, _blcelc.csPixelFormat_RedBits_set)
    __swig_setmethods__["GreenBits"] = _blcelc.csPixelFormat_GreenBits_set
    __swig_getmethods__["GreenBits"] = _blcelc.csPixelFormat_GreenBits_get
    if _newclass:GreenBits = property(_blcelc.csPixelFormat_GreenBits_get, _blcelc.csPixelFormat_GreenBits_set)
    __swig_setmethods__["BlueBits"] = _blcelc.csPixelFormat_BlueBits_set
    __swig_getmethods__["BlueBits"] = _blcelc.csPixelFormat_BlueBits_get
    if _newclass:BlueBits = property(_blcelc.csPixelFormat_BlueBits_get, _blcelc.csPixelFormat_BlueBits_set)
    __swig_setmethods__["AlphaBits"] = _blcelc.csPixelFormat_AlphaBits_set
    __swig_getmethods__["AlphaBits"] = _blcelc.csPixelFormat_AlphaBits_get
    if _newclass:AlphaBits = property(_blcelc.csPixelFormat_AlphaBits_get, _blcelc.csPixelFormat_AlphaBits_set)
    __swig_setmethods__["PalEntries"] = _blcelc.csPixelFormat_PalEntries_set
    __swig_getmethods__["PalEntries"] = _blcelc.csPixelFormat_PalEntries_get
    if _newclass:PalEntries = property(_blcelc.csPixelFormat_PalEntries_get, _blcelc.csPixelFormat_PalEntries_set)
    __swig_setmethods__["PixelBytes"] = _blcelc.csPixelFormat_PixelBytes_set
    __swig_getmethods__["PixelBytes"] = _blcelc.csPixelFormat_PixelBytes_get
    if _newclass:PixelBytes = property(_blcelc.csPixelFormat_PixelBytes_get, _blcelc.csPixelFormat_PixelBytes_set)
    def complete(*args): return _blcelc.csPixelFormat_complete(*args)
    def __init__(self, *args):
        _swig_setattr(self, csPixelFormat, 'this', _blcelc.new_csPixelFormat(*args))
        _swig_setattr(self, csPixelFormat, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csPixelFormat):
        try:
            if self.thisown: destroy(self)
        except: pass

class csPixelFormatPtr(csPixelFormat):
    def __init__(self, this):
        _swig_setattr(self, csPixelFormat, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPixelFormat, 'thisown', 0)
        _swig_setattr(self, csPixelFormat,self.__class__,csPixelFormat)
_blcelc.csPixelFormat_swigregister(csPixelFormatPtr)

class csImageArea(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csImageArea, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csImageArea, name)
    def __repr__(self):
        return "<C csImageArea instance at %s>" % (self.this,)
    __swig_setmethods__["x"] = _blcelc.csImageArea_x_set
    __swig_getmethods__["x"] = _blcelc.csImageArea_x_get
    if _newclass:x = property(_blcelc.csImageArea_x_get, _blcelc.csImageArea_x_set)
    __swig_setmethods__["y"] = _blcelc.csImageArea_y_set
    __swig_getmethods__["y"] = _blcelc.csImageArea_y_get
    if _newclass:y = property(_blcelc.csImageArea_y_get, _blcelc.csImageArea_y_set)
    __swig_setmethods__["w"] = _blcelc.csImageArea_w_set
    __swig_getmethods__["w"] = _blcelc.csImageArea_w_get
    if _newclass:w = property(_blcelc.csImageArea_w_get, _blcelc.csImageArea_w_set)
    __swig_setmethods__["h"] = _blcelc.csImageArea_h_set
    __swig_getmethods__["h"] = _blcelc.csImageArea_h_get
    if _newclass:h = property(_blcelc.csImageArea_h_get, _blcelc.csImageArea_h_set)
    __swig_setmethods__["data"] = _blcelc.csImageArea_data_set
    __swig_getmethods__["data"] = _blcelc.csImageArea_data_get
    if _newclass:data = property(_blcelc.csImageArea_data_get, _blcelc.csImageArea_data_set)
    def __init__(self, *args):
        _swig_setattr(self, csImageArea, 'this', _blcelc.new_csImageArea(*args))
        _swig_setattr(self, csImageArea, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csImageArea):
        try:
            if self.thisown: destroy(self)
        except: pass

class csImageAreaPtr(csImageArea):
    def __init__(self, this):
        _swig_setattr(self, csImageArea, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csImageArea, 'thisown', 0)
        _swig_setattr(self, csImageArea,self.__class__,csImageArea)
_blcelc.csImageArea_swigregister(csImageAreaPtr)

class iOffscreenCanvasCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iOffscreenCanvasCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iOffscreenCanvasCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iOffscreenCanvasCallback instance at %s>" % (self.this,)
    def FinishDraw(*args): return _blcelc.iOffscreenCanvasCallback_FinishDraw(*args)
    def SetRGB(*args): return _blcelc.iOffscreenCanvasCallback_SetRGB(*args)
    def __del__(self, destroy=_blcelc.delete_iOffscreenCanvasCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iOffscreenCanvasCallbackPtr(iOffscreenCanvasCallback):
    def __init__(self, this):
        _swig_setattr(self, iOffscreenCanvasCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iOffscreenCanvasCallback, 'thisown', 0)
        _swig_setattr(self, iOffscreenCanvasCallback,self.__class__,iOffscreenCanvasCallback)
_blcelc.iOffscreenCanvasCallback_swigregister(iOffscreenCanvasCallbackPtr)

class iGraphics2D(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGraphics2D, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGraphics2D, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGraphics2D instance at %s>" % (self.this,)
    def Open(*args): return _blcelc.iGraphics2D_Open(*args)
    def Close(*args): return _blcelc.iGraphics2D_Close(*args)
    def GetWidth(*args): return _blcelc.iGraphics2D_GetWidth(*args)
    def GetHeight(*args): return _blcelc.iGraphics2D_GetHeight(*args)
    def GetPage(*args): return _blcelc.iGraphics2D_GetPage(*args)
    def DoubleBuffer(*args): return _blcelc.iGraphics2D_DoubleBuffer(*args)
    def GetDoubleBufferState(*args): return _blcelc.iGraphics2D_GetDoubleBufferState(*args)
    def GetPixelFormat(*args): return _blcelc.iGraphics2D_GetPixelFormat(*args)
    def GetPixelBytes(*args): return _blcelc.iGraphics2D_GetPixelBytes(*args)
    def GetPalEntryCount(*args): return _blcelc.iGraphics2D_GetPalEntryCount(*args)
    def GetPalette(*args): return _blcelc.iGraphics2D_GetPalette(*args)
    def SetRGB(*args): return _blcelc.iGraphics2D_SetRGB(*args)
    def FindRGB(*args): return _blcelc.iGraphics2D_FindRGB(*args)
    def GetRGB(*args): return _blcelc.iGraphics2D_GetRGB(*args)
    def GetRGBA(*args): return _blcelc.iGraphics2D_GetRGBA(*args)
    def SetClipRect(*args): return _blcelc.iGraphics2D_SetClipRect(*args)
    def GetClipRect(*args): return _blcelc.iGraphics2D_GetClipRect(*args)
    def BeginDraw(*args): return _blcelc.iGraphics2D_BeginDraw(*args)
    def FinishDraw(*args): return _blcelc.iGraphics2D_FinishDraw(*args)
    def Print(*args): return _blcelc.iGraphics2D_Print(*args)
    def Clear(*args): return _blcelc.iGraphics2D_Clear(*args)
    def ClearAll(*args): return _blcelc.iGraphics2D_ClearAll(*args)
    def DrawLine(*args): return _blcelc.iGraphics2D_DrawLine(*args)
    def DrawBox(*args): return _blcelc.iGraphics2D_DrawBox(*args)
    def ClipLine(*args): return _blcelc.iGraphics2D_ClipLine(*args)
    def DrawPixel(*args): return _blcelc.iGraphics2D_DrawPixel(*args)
    def DrawPixels(*args): return _blcelc.iGraphics2D_DrawPixels(*args)
    def Blit(*args): return _blcelc.iGraphics2D_Blit(*args)
    def GetPixelAt(*args): return _blcelc.iGraphics2D_GetPixelAt(*args)
    def GetPixel(*args): return _blcelc.iGraphics2D_GetPixel(*args)
    def SaveArea(*args): return _blcelc.iGraphics2D_SaveArea(*args)
    def RestoreArea(*args): return _blcelc.iGraphics2D_RestoreArea(*args)
    def FreeArea(*args): return _blcelc.iGraphics2D_FreeArea(*args)
    def Write(*args): return _blcelc.iGraphics2D_Write(*args)
    def WriteBaseline(*args): return _blcelc.iGraphics2D_WriteBaseline(*args)
    def AllowResize(*args): return _blcelc.iGraphics2D_AllowResize(*args)
    def Resize(*args): return _blcelc.iGraphics2D_Resize(*args)
    def GetFontServer(*args): return _blcelc.iGraphics2D_GetFontServer(*args)
    def PerformExtension(*args): return _blcelc.iGraphics2D_PerformExtension(*args)
    def ScreenShot(*args): return _blcelc.iGraphics2D_ScreenShot(*args)
    def GetNativeWindow(*args): return _blcelc.iGraphics2D_GetNativeWindow(*args)
    def GetFullScreen(*args): return _blcelc.iGraphics2D_GetFullScreen(*args)
    def SetFullScreen(*args): return _blcelc.iGraphics2D_SetFullScreen(*args)
    def SetMousePosition(*args): return _blcelc.iGraphics2D_SetMousePosition(*args)
    def SetMouseCursor(*args): return _blcelc.iGraphics2D_SetMouseCursor(*args)
    def SetGamma(*args): return _blcelc.iGraphics2D_SetGamma(*args)
    def GetGamma(*args): return _blcelc.iGraphics2D_GetGamma(*args)
    def CreateOffscreenCanvas(*args): return _blcelc.iGraphics2D_CreateOffscreenCanvas(*args)
    def __del__(self, destroy=_blcelc.delete_iGraphics2D):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iGraphics2D_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iGraphics2D_scfGetVersion)
    def _PerformExtension(*args): return _blcelc.iGraphics2D__PerformExtension(*args)
    def PerformExtension (self, command, *args):
      self._PerformExtension(self.__class__.__name__, command, args);


class iGraphics2DPtr(iGraphics2D):
    def __init__(self, this):
        _swig_setattr(self, iGraphics2D, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGraphics2D, 'thisown', 0)
        _swig_setattr(self, iGraphics2D,self.__class__,iGraphics2D)
_blcelc.iGraphics2D_swigregister(iGraphics2DPtr)

iGraphics2D_scfGetVersion = _blcelc.iGraphics2D_scfGetVersion

CSDRAW_2DGRAPHICS = _blcelc.CSDRAW_2DGRAPHICS
CSDRAW_3DGRAPHICS = _blcelc.CSDRAW_3DGRAPHICS
CSDRAW_CLEARZBUFFER = _blcelc.CSDRAW_CLEARZBUFFER
CSDRAW_CLEARSCREEN = _blcelc.CSDRAW_CLEARSCREEN
CS_CLIPPER_NONE = _blcelc.CS_CLIPPER_NONE
CS_CLIPPER_OPTIONAL = _blcelc.CS_CLIPPER_OPTIONAL
CS_CLIPPER_TOPLEVEL = _blcelc.CS_CLIPPER_TOPLEVEL
CS_CLIPPER_REQUIRED = _blcelc.CS_CLIPPER_REQUIRED
CS_CLIP_NOT = _blcelc.CS_CLIP_NOT
CS_CLIP_NEEDED = _blcelc.CS_CLIP_NEEDED
class csFog(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csFog, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csFog, name)
    def __repr__(self):
        return "<C csFog instance at %s>" % (self.this,)
    __swig_setmethods__["enabled"] = _blcelc.csFog_enabled_set
    __swig_getmethods__["enabled"] = _blcelc.csFog_enabled_get
    if _newclass:enabled = property(_blcelc.csFog_enabled_get, _blcelc.csFog_enabled_set)
    __swig_setmethods__["density"] = _blcelc.csFog_density_set
    __swig_getmethods__["density"] = _blcelc.csFog_density_get
    if _newclass:density = property(_blcelc.csFog_density_get, _blcelc.csFog_density_set)
    __swig_setmethods__["red"] = _blcelc.csFog_red_set
    __swig_getmethods__["red"] = _blcelc.csFog_red_get
    if _newclass:red = property(_blcelc.csFog_red_get, _blcelc.csFog_red_set)
    __swig_setmethods__["green"] = _blcelc.csFog_green_set
    __swig_getmethods__["green"] = _blcelc.csFog_green_get
    if _newclass:green = property(_blcelc.csFog_green_get, _blcelc.csFog_green_set)
    __swig_setmethods__["blue"] = _blcelc.csFog_blue_set
    __swig_getmethods__["blue"] = _blcelc.csFog_blue_get
    if _newclass:blue = property(_blcelc.csFog_blue_get, _blcelc.csFog_blue_set)
    def __init__(self, *args):
        _swig_setattr(self, csFog, 'this', _blcelc.new_csFog(*args))
        _swig_setattr(self, csFog, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csFog):
        try:
            if self.thisown: destroy(self)
        except: pass

class csFogPtr(csFog):
    def __init__(self, this):
        _swig_setattr(self, csFog, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csFog, 'thisown', 0)
        _swig_setattr(self, csFog,self.__class__,csFog)
_blcelc.csFog_swigregister(csFogPtr)

CS_ZBUF_NONE = _blcelc.CS_ZBUF_NONE
CS_ZBUF_FILL = _blcelc.CS_ZBUF_FILL
CS_ZBUF_TEST = _blcelc.CS_ZBUF_TEST
CS_ZBUF_USE = _blcelc.CS_ZBUF_USE
CS_ZBUF_FILLONLY = _blcelc.CS_ZBUF_FILLONLY
CS_ZBUF_EQUAL = _blcelc.CS_ZBUF_EQUAL
CS_ZBUF_INVERT = _blcelc.CS_ZBUF_INVERT
CS_ZBUF_MESH = _blcelc.CS_ZBUF_MESH
CS_ZBUF_MESH2 = _blcelc.CS_ZBUF_MESH2
CS_VATTRIB_SPECIFIC_FIRST = _blcelc.CS_VATTRIB_SPECIFIC_FIRST
CS_VATTRIB_SPECIFIC_LAST = _blcelc.CS_VATTRIB_SPECIFIC_LAST
CS_VATTRIB_GENERIC_FIRST = _blcelc.CS_VATTRIB_GENERIC_FIRST
CS_VATTRIB_GENERIC_LAST = _blcelc.CS_VATTRIB_GENERIC_LAST
CS_VATTRIB_POSITION = _blcelc.CS_VATTRIB_POSITION
CS_VATTRIB_WEIGHT = _blcelc.CS_VATTRIB_WEIGHT
CS_VATTRIB_NORMAL = _blcelc.CS_VATTRIB_NORMAL
CS_VATTRIB_COLOR = _blcelc.CS_VATTRIB_COLOR
CS_VATTRIB_PRIMARY_COLOR = _blcelc.CS_VATTRIB_PRIMARY_COLOR
CS_VATTRIB_SECONDARY_COLOR = _blcelc.CS_VATTRIB_SECONDARY_COLOR
CS_VATTRIB_FOGCOORD = _blcelc.CS_VATTRIB_FOGCOORD
CS_VATTRIB_TEXCOORD = _blcelc.CS_VATTRIB_TEXCOORD
CS_VATTRIB_TEXCOORD0 = _blcelc.CS_VATTRIB_TEXCOORD0
CS_VATTRIB_TEXCOORD1 = _blcelc.CS_VATTRIB_TEXCOORD1
CS_VATTRIB_TEXCOORD2 = _blcelc.CS_VATTRIB_TEXCOORD2
CS_VATTRIB_TEXCOORD3 = _blcelc.CS_VATTRIB_TEXCOORD3
CS_VATTRIB_TEXCOORD4 = _blcelc.CS_VATTRIB_TEXCOORD4
CS_VATTRIB_TEXCOORD5 = _blcelc.CS_VATTRIB_TEXCOORD5
CS_VATTRIB_TEXCOORD6 = _blcelc.CS_VATTRIB_TEXCOORD6
CS_VATTRIB_TEXCOORD7 = _blcelc.CS_VATTRIB_TEXCOORD7
CS_VATTRIB_0 = _blcelc.CS_VATTRIB_0
CS_VATTRIB_1 = _blcelc.CS_VATTRIB_1
CS_VATTRIB_2 = _blcelc.CS_VATTRIB_2
CS_VATTRIB_3 = _blcelc.CS_VATTRIB_3
CS_VATTRIB_4 = _blcelc.CS_VATTRIB_4
CS_VATTRIB_5 = _blcelc.CS_VATTRIB_5
CS_VATTRIB_6 = _blcelc.CS_VATTRIB_6
CS_VATTRIB_7 = _blcelc.CS_VATTRIB_7
CS_VATTRIB_8 = _blcelc.CS_VATTRIB_8
CS_VATTRIB_9 = _blcelc.CS_VATTRIB_9
CS_VATTRIB_10 = _blcelc.CS_VATTRIB_10
CS_VATTRIB_11 = _blcelc.CS_VATTRIB_11
CS_VATTRIB_12 = _blcelc.CS_VATTRIB_12
CS_VATTRIB_13 = _blcelc.CS_VATTRIB_13
CS_VATTRIB_14 = _blcelc.CS_VATTRIB_14
CS_VATTRIB_15 = _blcelc.CS_VATTRIB_15
G3DFOGMETHOD_NONE = _blcelc.G3DFOGMETHOD_NONE
G3DFOGMETHOD_ZBUFFER = _blcelc.G3DFOGMETHOD_ZBUFFER
G3DFOGMETHOD_VERTEX = _blcelc.G3DFOGMETHOD_VERTEX
CS_FX_MASK_MIXMODE = _blcelc.CS_FX_MASK_MIXMODE
CS_FX_COPY = _blcelc.CS_FX_COPY
CS_FX_MULTIPLY = _blcelc.CS_FX_MULTIPLY
CS_FX_MULTIPLY2 = _blcelc.CS_FX_MULTIPLY2
CS_FX_ADD = _blcelc.CS_FX_ADD
CS_FX_ALPHA = _blcelc.CS_FX_ALPHA
CS_FX_TRANSPARENT = _blcelc.CS_FX_TRANSPARENT
CS_FX_DESTALPHAADD = _blcelc.CS_FX_DESTALPHAADD
CS_FX_SRCALPHAADD = _blcelc.CS_FX_SRCALPHAADD
CS_FX_PREMULTALPHA = _blcelc.CS_FX_PREMULTALPHA
CS_FX_MESH = _blcelc.CS_FX_MESH
CS_FX_KEYCOLOR = _blcelc.CS_FX_KEYCOLOR
CS_FX_FLAT = _blcelc.CS_FX_FLAT
CS_FX_TILING = _blcelc.CS_FX_TILING
CS_FX_MASK_ALPHA = _blcelc.CS_FX_MASK_ALPHA
class csAlphaMode(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csAlphaMode, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csAlphaMode, name)
    def __repr__(self):
        return "<C csAlphaMode instance at %s>" % (self.this,)
    alphaNone = _blcelc.csAlphaMode_alphaNone
    alphaBinary = _blcelc.csAlphaMode_alphaBinary
    alphaSmooth = _blcelc.csAlphaMode_alphaSmooth
    __swig_setmethods__["autoAlphaMode"] = _blcelc.csAlphaMode_autoAlphaMode_set
    __swig_getmethods__["autoAlphaMode"] = _blcelc.csAlphaMode_autoAlphaMode_get
    if _newclass:autoAlphaMode = property(_blcelc.csAlphaMode_autoAlphaMode_get, _blcelc.csAlphaMode_autoAlphaMode_set)
    def __init__(self, *args):
        _swig_setattr(self, csAlphaMode, 'this', _blcelc.new_csAlphaMode(*args))
        _swig_setattr(self, csAlphaMode, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csAlphaMode):
        try:
            if self.thisown: destroy(self)
        except: pass

class csAlphaModePtr(csAlphaMode):
    def __init__(self, this):
        _swig_setattr(self, csAlphaMode, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csAlphaMode, 'thisown', 0)
        _swig_setattr(self, csAlphaMode,self.__class__,csAlphaMode)
_blcelc.csAlphaMode_swigregister(csAlphaModePtr)

CS_LIGHTPARAM_POSITION = _blcelc.CS_LIGHTPARAM_POSITION
CS_LIGHTPARAM_DIFFUSE = _blcelc.CS_LIGHTPARAM_DIFFUSE
CS_LIGHTPARAM_SPECULAR = _blcelc.CS_LIGHTPARAM_SPECULAR
CS_LIGHTPARAM_ATTENUATION = _blcelc.CS_LIGHTPARAM_ATTENUATION
CS_SHADOW_VOLUME_BEGIN = _blcelc.CS_SHADOW_VOLUME_BEGIN
CS_SHADOW_VOLUME_PASS1 = _blcelc.CS_SHADOW_VOLUME_PASS1
CS_SHADOW_VOLUME_PASS2 = _blcelc.CS_SHADOW_VOLUME_PASS2
CS_SHADOW_VOLUME_FAIL1 = _blcelc.CS_SHADOW_VOLUME_FAIL1
CS_SHADOW_VOLUME_FAIL2 = _blcelc.CS_SHADOW_VOLUME_FAIL2
CS_SHADOW_VOLUME_USE = _blcelc.CS_SHADOW_VOLUME_USE
CS_SHADOW_VOLUME_FINISH = _blcelc.CS_SHADOW_VOLUME_FINISH
G3DRENDERSTATE_ZBUFFERMODE = _blcelc.G3DRENDERSTATE_ZBUFFERMODE
G3DRENDERSTATE_DITHERENABLE = _blcelc.G3DRENDERSTATE_DITHERENABLE
G3DRENDERSTATE_BILINEARMAPPINGENABLE = _blcelc.G3DRENDERSTATE_BILINEARMAPPINGENABLE
G3DRENDERSTATE_TRILINEARMAPPINGENABLE = _blcelc.G3DRENDERSTATE_TRILINEARMAPPINGENABLE
G3DRENDERSTATE_TRANSPARENCYENABLE = _blcelc.G3DRENDERSTATE_TRANSPARENCYENABLE
G3DRENDERSTATE_MIPMAPENABLE = _blcelc.G3DRENDERSTATE_MIPMAPENABLE
G3DRENDERSTATE_TEXTUREMAPPINGENABLE = _blcelc.G3DRENDERSTATE_TEXTUREMAPPINGENABLE
G3DRENDERSTATE_LIGHTINGENABLE = _blcelc.G3DRENDERSTATE_LIGHTINGENABLE
G3DRENDERSTATE_INTERLACINGENABLE = _blcelc.G3DRENDERSTATE_INTERLACINGENABLE
G3DRENDERSTATE_MMXENABLE = _blcelc.G3DRENDERSTATE_MMXENABLE
G3DRENDERSTATE_INTERPOLATIONSTEP = _blcelc.G3DRENDERSTATE_INTERPOLATIONSTEP
G3DRENDERSTATE_MAXPOLYGONSTODRAW = _blcelc.G3DRENDERSTATE_MAXPOLYGONSTODRAW
G3DRENDERSTATE_GOURAUDENABLE = _blcelc.G3DRENDERSTATE_GOURAUDENABLE
G3DRENDERSTATE_EDGES = _blcelc.G3DRENDERSTATE_EDGES
class csGraphics3DCaps(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csGraphics3DCaps, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csGraphics3DCaps, name)
    def __repr__(self):
        return "<C csGraphics3DCaps instance at %s>" % (self.this,)
    __swig_setmethods__["CanClip"] = _blcelc.csGraphics3DCaps_CanClip_set
    __swig_getmethods__["CanClip"] = _blcelc.csGraphics3DCaps_CanClip_get
    if _newclass:CanClip = property(_blcelc.csGraphics3DCaps_CanClip_get, _blcelc.csGraphics3DCaps_CanClip_set)
    __swig_setmethods__["minTexHeight"] = _blcelc.csGraphics3DCaps_minTexHeight_set
    __swig_getmethods__["minTexHeight"] = _blcelc.csGraphics3DCaps_minTexHeight_get
    if _newclass:minTexHeight = property(_blcelc.csGraphics3DCaps_minTexHeight_get, _blcelc.csGraphics3DCaps_minTexHeight_set)
    __swig_setmethods__["minTexWidth"] = _blcelc.csGraphics3DCaps_minTexWidth_set
    __swig_getmethods__["minTexWidth"] = _blcelc.csGraphics3DCaps_minTexWidth_get
    if _newclass:minTexWidth = property(_blcelc.csGraphics3DCaps_minTexWidth_get, _blcelc.csGraphics3DCaps_minTexWidth_set)
    __swig_setmethods__["maxTexHeight"] = _blcelc.csGraphics3DCaps_maxTexHeight_set
    __swig_getmethods__["maxTexHeight"] = _blcelc.csGraphics3DCaps_maxTexHeight_get
    if _newclass:maxTexHeight = property(_blcelc.csGraphics3DCaps_maxTexHeight_get, _blcelc.csGraphics3DCaps_maxTexHeight_set)
    __swig_setmethods__["maxTexWidth"] = _blcelc.csGraphics3DCaps_maxTexWidth_set
    __swig_getmethods__["maxTexWidth"] = _blcelc.csGraphics3DCaps_maxTexWidth_get
    if _newclass:maxTexWidth = property(_blcelc.csGraphics3DCaps_maxTexWidth_get, _blcelc.csGraphics3DCaps_maxTexWidth_set)
    __swig_setmethods__["fog"] = _blcelc.csGraphics3DCaps_fog_set
    __swig_getmethods__["fog"] = _blcelc.csGraphics3DCaps_fog_get
    if _newclass:fog = property(_blcelc.csGraphics3DCaps_fog_get, _blcelc.csGraphics3DCaps_fog_set)
    __swig_setmethods__["NeedsPO2Maps"] = _blcelc.csGraphics3DCaps_NeedsPO2Maps_set
    __swig_getmethods__["NeedsPO2Maps"] = _blcelc.csGraphics3DCaps_NeedsPO2Maps_get
    if _newclass:NeedsPO2Maps = property(_blcelc.csGraphics3DCaps_NeedsPO2Maps_get, _blcelc.csGraphics3DCaps_NeedsPO2Maps_set)
    __swig_setmethods__["MaxAspectRatio"] = _blcelc.csGraphics3DCaps_MaxAspectRatio_set
    __swig_getmethods__["MaxAspectRatio"] = _blcelc.csGraphics3DCaps_MaxAspectRatio_get
    if _newclass:MaxAspectRatio = property(_blcelc.csGraphics3DCaps_MaxAspectRatio_get, _blcelc.csGraphics3DCaps_MaxAspectRatio_set)
    __swig_setmethods__["SupportsPointSprites"] = _blcelc.csGraphics3DCaps_SupportsPointSprites_set
    __swig_getmethods__["SupportsPointSprites"] = _blcelc.csGraphics3DCaps_SupportsPointSprites_get
    if _newclass:SupportsPointSprites = property(_blcelc.csGraphics3DCaps_SupportsPointSprites_get, _blcelc.csGraphics3DCaps_SupportsPointSprites_set)
    __swig_setmethods__["DestinationAlpha"] = _blcelc.csGraphics3DCaps_DestinationAlpha_set
    __swig_getmethods__["DestinationAlpha"] = _blcelc.csGraphics3DCaps_DestinationAlpha_get
    if _newclass:DestinationAlpha = property(_blcelc.csGraphics3DCaps_DestinationAlpha_get, _blcelc.csGraphics3DCaps_DestinationAlpha_set)
    __swig_setmethods__["StencilShadows"] = _blcelc.csGraphics3DCaps_StencilShadows_set
    __swig_getmethods__["StencilShadows"] = _blcelc.csGraphics3DCaps_StencilShadows_get
    if _newclass:StencilShadows = property(_blcelc.csGraphics3DCaps_StencilShadows_get, _blcelc.csGraphics3DCaps_StencilShadows_set)
    def __init__(self, *args):
        _swig_setattr(self, csGraphics3DCaps, 'this', _blcelc.new_csGraphics3DCaps(*args))
        _swig_setattr(self, csGraphics3DCaps, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csGraphics3DCaps):
        try:
            if self.thisown: destroy(self)
        except: pass

class csGraphics3DCapsPtr(csGraphics3DCaps):
    def __init__(self, this):
        _swig_setattr(self, csGraphics3DCaps, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csGraphics3DCaps, 'thisown', 0)
        _swig_setattr(self, csGraphics3DCaps,self.__class__,csGraphics3DCaps)
_blcelc.csGraphics3DCaps_swigregister(csGraphics3DCapsPtr)

CS_MESHTYPE_TRIANGLES = _blcelc.CS_MESHTYPE_TRIANGLES
CS_MESHTYPE_QUADS = _blcelc.CS_MESHTYPE_QUADS
CS_MESHTYPE_TRIANGLESTRIP = _blcelc.CS_MESHTYPE_TRIANGLESTRIP
CS_MESHTYPE_TRIANGLEFAN = _blcelc.CS_MESHTYPE_TRIANGLEFAN
CS_MESHTYPE_POINTS = _blcelc.CS_MESHTYPE_POINTS
CS_MESHTYPE_POINT_SPRITES = _blcelc.CS_MESHTYPE_POINT_SPRITES
CS_MESHTYPE_LINES = _blcelc.CS_MESHTYPE_LINES
CS_MESHTYPE_LINESTRIP = _blcelc.CS_MESHTYPE_LINESTRIP
CS_MESHTYPE_POLYGON = _blcelc.CS_MESHTYPE_POLYGON
csSimpleMeshScreenspace = _blcelc.csSimpleMeshScreenspace
class csSimpleRenderMesh(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSimpleRenderMesh, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSimpleRenderMesh, name)
    def __repr__(self):
        return "<C csSimpleRenderMesh instance at %s>" % (self.this,)
    __swig_setmethods__["meshtype"] = _blcelc.csSimpleRenderMesh_meshtype_set
    __swig_getmethods__["meshtype"] = _blcelc.csSimpleRenderMesh_meshtype_get
    if _newclass:meshtype = property(_blcelc.csSimpleRenderMesh_meshtype_get, _blcelc.csSimpleRenderMesh_meshtype_set)
    __swig_setmethods__["indexCount"] = _blcelc.csSimpleRenderMesh_indexCount_set
    __swig_getmethods__["indexCount"] = _blcelc.csSimpleRenderMesh_indexCount_get
    if _newclass:indexCount = property(_blcelc.csSimpleRenderMesh_indexCount_get, _blcelc.csSimpleRenderMesh_indexCount_set)
    __swig_setmethods__["indices"] = _blcelc.csSimpleRenderMesh_indices_set
    __swig_getmethods__["indices"] = _blcelc.csSimpleRenderMesh_indices_get
    if _newclass:indices = property(_blcelc.csSimpleRenderMesh_indices_get, _blcelc.csSimpleRenderMesh_indices_set)
    __swig_setmethods__["vertexCount"] = _blcelc.csSimpleRenderMesh_vertexCount_set
    __swig_getmethods__["vertexCount"] = _blcelc.csSimpleRenderMesh_vertexCount_get
    if _newclass:vertexCount = property(_blcelc.csSimpleRenderMesh_vertexCount_get, _blcelc.csSimpleRenderMesh_vertexCount_set)
    __swig_setmethods__["vertices"] = _blcelc.csSimpleRenderMesh_vertices_set
    __swig_getmethods__["vertices"] = _blcelc.csSimpleRenderMesh_vertices_get
    if _newclass:vertices = property(_blcelc.csSimpleRenderMesh_vertices_get, _blcelc.csSimpleRenderMesh_vertices_set)
    __swig_setmethods__["texcoords"] = _blcelc.csSimpleRenderMesh_texcoords_set
    __swig_getmethods__["texcoords"] = _blcelc.csSimpleRenderMesh_texcoords_get
    if _newclass:texcoords = property(_blcelc.csSimpleRenderMesh_texcoords_get, _blcelc.csSimpleRenderMesh_texcoords_set)
    __swig_setmethods__["colors"] = _blcelc.csSimpleRenderMesh_colors_set
    __swig_getmethods__["colors"] = _blcelc.csSimpleRenderMesh_colors_get
    if _newclass:colors = property(_blcelc.csSimpleRenderMesh_colors_get, _blcelc.csSimpleRenderMesh_colors_set)
    __swig_setmethods__["texture"] = _blcelc.csSimpleRenderMesh_texture_set
    __swig_getmethods__["texture"] = _blcelc.csSimpleRenderMesh_texture_get
    if _newclass:texture = property(_blcelc.csSimpleRenderMesh_texture_get, _blcelc.csSimpleRenderMesh_texture_set)
    __swig_setmethods__["shader"] = _blcelc.csSimpleRenderMesh_shader_set
    __swig_getmethods__["shader"] = _blcelc.csSimpleRenderMesh_shader_get
    if _newclass:shader = property(_blcelc.csSimpleRenderMesh_shader_get, _blcelc.csSimpleRenderMesh_shader_set)
    __swig_setmethods__["dynDomain"] = _blcelc.csSimpleRenderMesh_dynDomain_set
    __swig_getmethods__["dynDomain"] = _blcelc.csSimpleRenderMesh_dynDomain_get
    if _newclass:dynDomain = property(_blcelc.csSimpleRenderMesh_dynDomain_get, _blcelc.csSimpleRenderMesh_dynDomain_set)
    __swig_setmethods__["alphaType"] = _blcelc.csSimpleRenderMesh_alphaType_set
    __swig_getmethods__["alphaType"] = _blcelc.csSimpleRenderMesh_alphaType_get
    if _newclass:alphaType = property(_blcelc.csSimpleRenderMesh_alphaType_get, _blcelc.csSimpleRenderMesh_alphaType_set)
    __swig_setmethods__["z_buf_mode"] = _blcelc.csSimpleRenderMesh_z_buf_mode_set
    __swig_getmethods__["z_buf_mode"] = _blcelc.csSimpleRenderMesh_z_buf_mode_get
    if _newclass:z_buf_mode = property(_blcelc.csSimpleRenderMesh_z_buf_mode_get, _blcelc.csSimpleRenderMesh_z_buf_mode_set)
    __swig_setmethods__["mixmode"] = _blcelc.csSimpleRenderMesh_mixmode_set
    __swig_getmethods__["mixmode"] = _blcelc.csSimpleRenderMesh_mixmode_get
    if _newclass:mixmode = property(_blcelc.csSimpleRenderMesh_mixmode_get, _blcelc.csSimpleRenderMesh_mixmode_set)
    __swig_setmethods__["object2camera"] = _blcelc.csSimpleRenderMesh_object2camera_set
    __swig_getmethods__["object2camera"] = _blcelc.csSimpleRenderMesh_object2camera_get
    if _newclass:object2camera = property(_blcelc.csSimpleRenderMesh_object2camera_get, _blcelc.csSimpleRenderMesh_object2camera_set)
    def __init__(self, *args):
        _swig_setattr(self, csSimpleRenderMesh, 'this', _blcelc.new_csSimpleRenderMesh(*args))
        _swig_setattr(self, csSimpleRenderMesh, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csSimpleRenderMesh):
        try:
            if self.thisown: destroy(self)
        except: pass

class csSimpleRenderMeshPtr(csSimpleRenderMesh):
    def __init__(self, this):
        _swig_setattr(self, csSimpleRenderMesh, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csSimpleRenderMesh, 'thisown', 0)
        _swig_setattr(self, csSimpleRenderMesh,self.__class__,csSimpleRenderMesh)
_blcelc.csSimpleRenderMesh_swigregister(csSimpleRenderMeshPtr)

class iGraphics3D(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGraphics3D, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGraphics3D, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGraphics3D instance at %s>" % (self.this,)
    def Open(*args): return _blcelc.iGraphics3D_Open(*args)
    def Close(*args): return _blcelc.iGraphics3D_Close(*args)
    def GetDriver2D(*args): return _blcelc.iGraphics3D_GetDriver2D(*args)
    def GetTextureManager(*args): return _blcelc.iGraphics3D_GetTextureManager(*args)
    def SetDimensions(*args): return _blcelc.iGraphics3D_SetDimensions(*args)
    def GetWidth(*args): return _blcelc.iGraphics3D_GetWidth(*args)
    def GetHeight(*args): return _blcelc.iGraphics3D_GetHeight(*args)
    def GetCaps(*args): return _blcelc.iGraphics3D_GetCaps(*args)
    def SetPerspectiveCenter(*args): return _blcelc.iGraphics3D_SetPerspectiveCenter(*args)
    def GetPerspectiveCenter(*args): return _blcelc.iGraphics3D_GetPerspectiveCenter(*args)
    def SetPerspectiveAspect(*args): return _blcelc.iGraphics3D_SetPerspectiveAspect(*args)
    def GetPerspectiveAspect(*args): return _blcelc.iGraphics3D_GetPerspectiveAspect(*args)
    def SetObjectToCamera(*args): return _blcelc.iGraphics3D_SetObjectToCamera(*args)
    def GetObjectToCamera(*args): return _blcelc.iGraphics3D_GetObjectToCamera(*args)
    def SetRenderTarget(*args): return _blcelc.iGraphics3D_SetRenderTarget(*args)
    def GetRenderTarget(*args): return _blcelc.iGraphics3D_GetRenderTarget(*args)
    def BeginDraw(*args): return _blcelc.iGraphics3D_BeginDraw(*args)
    def FinishDraw(*args): return _blcelc.iGraphics3D_FinishDraw(*args)
    def Print(*args): return _blcelc.iGraphics3D_Print(*args)
    def DrawPixmap(*args): return _blcelc.iGraphics3D_DrawPixmap(*args)
    def DrawLine(*args): return _blcelc.iGraphics3D_DrawLine(*args)
    def SetClipper(*args): return _blcelc.iGraphics3D_SetClipper(*args)
    def GetClipper(*args): return _blcelc.iGraphics3D_GetClipper(*args)
    def GetClipType(*args): return _blcelc.iGraphics3D_GetClipType(*args)
    def SetNearPlane(*args): return _blcelc.iGraphics3D_SetNearPlane(*args)
    def ResetNearPlane(*args): return _blcelc.iGraphics3D_ResetNearPlane(*args)
    def GetNearPlane(*args): return _blcelc.iGraphics3D_GetNearPlane(*args)
    def HasNearPlane(*args): return _blcelc.iGraphics3D_HasNearPlane(*args)
    def SetRenderState(*args): return _blcelc.iGraphics3D_SetRenderState(*args)
    def GetRenderState(*args): return _blcelc.iGraphics3D_GetRenderState(*args)
    def SetOption(*args): return _blcelc.iGraphics3D_SetOption(*args)
    def CreateRenderBuffer(*args): return _blcelc.iGraphics3D_CreateRenderBuffer(*args)
    def CreateIndexRenderBuffer(*args): return _blcelc.iGraphics3D_CreateIndexRenderBuffer(*args)
    def CreateInterleavedRenderBuffers(*args): return _blcelc.iGraphics3D_CreateInterleavedRenderBuffers(*args)
    def ActivateBuffers(*args): return _blcelc.iGraphics3D_ActivateBuffers(*args)
    def DeactivateBuffers(*args): return _blcelc.iGraphics3D_DeactivateBuffers(*args)
    def SetTextureState(*args): return _blcelc.iGraphics3D_SetTextureState(*args)
    def DrawMesh(*args): return _blcelc.iGraphics3D_DrawMesh(*args)
    def SetWriteMask(*args): return _blcelc.iGraphics3D_SetWriteMask(*args)
    def GetWriteMask(*args): return _blcelc.iGraphics3D_GetWriteMask(*args)
    def SetZMode(*args): return _blcelc.iGraphics3D_SetZMode(*args)
    def EnableZOffset(*args): return _blcelc.iGraphics3D_EnableZOffset(*args)
    def DisableZOffset(*args): return _blcelc.iGraphics3D_DisableZOffset(*args)
    def SetShadowState(*args): return _blcelc.iGraphics3D_SetShadowState(*args)
    def GetZBuffValue(*args): return _blcelc.iGraphics3D_GetZBuffValue(*args)
    def OpenPortal(*args): return _blcelc.iGraphics3D_OpenPortal(*args)
    def ClosePortal(*args): return _blcelc.iGraphics3D_ClosePortal(*args)
    def CreateHalo(*args): return _blcelc.iGraphics3D_CreateHalo(*args)
    def RemoveFromCache(*args): return _blcelc.iGraphics3D_RemoveFromCache(*args)
    def IsLightmapOK(*args): return _blcelc.iGraphics3D_IsLightmapOK(*args)
    def CreatePolygonRenderer(*args): return _blcelc.iGraphics3D_CreatePolygonRenderer(*args)
    def SetWorldToCamera(*args): return _blcelc.iGraphics3D_SetWorldToCamera(*args)
    def DrawSimpleMesh(*args): return _blcelc.iGraphics3D_DrawSimpleMesh(*args)
    def GetZMode(*args): return _blcelc.iGraphics3D_GetZMode(*args)
    def __del__(self, destroy=_blcelc.delete_iGraphics3D):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iGraphics3D_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iGraphics3D_scfGetVersion)

class iGraphics3DPtr(iGraphics3D):
    def __init__(self, this):
        _swig_setattr(self, iGraphics3D, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGraphics3D, 'thisown', 0)
        _swig_setattr(self, iGraphics3D,self.__class__,iGraphics3D)
_blcelc.iGraphics3D_swigregister(iGraphics3DPtr)

iGraphics3D_scfGetVersion = _blcelc.iGraphics3D_scfGetVersion

csmcNone = _blcelc.csmcNone
csmcArrow = _blcelc.csmcArrow
csmcLens = _blcelc.csmcLens
csmcCross = _blcelc.csmcCross
csmcPen = _blcelc.csmcPen
csmcMove = _blcelc.csmcMove
csmcSizeNWSE = _blcelc.csmcSizeNWSE
csmcSizeNESW = _blcelc.csmcSizeNESW
csmcSizeNS = _blcelc.csmcSizeNS
csmcSizeEW = _blcelc.csmcSizeEW
csmcStop = _blcelc.csmcStop
csmcWait = _blcelc.csmcWait
CS_ALERT_ERROR = _blcelc.CS_ALERT_ERROR
CS_ALERT_WARNING = _blcelc.CS_ALERT_WARNING
CS_ALERT_NOTE = _blcelc.CS_ALERT_NOTE
class iNativeWindowManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNativeWindowManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNativeWindowManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNativeWindowManager instance at %s>" % (self.this,)
    def Alert(*args): return _blcelc.iNativeWindowManager_Alert(*args)
    def __del__(self, destroy=_blcelc.delete_iNativeWindowManager):
        try:
            if self.thisown: destroy(self)
        except: pass

class iNativeWindowManagerPtr(iNativeWindowManager):
    def __init__(self, this):
        _swig_setattr(self, iNativeWindowManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNativeWindowManager, 'thisown', 0)
        _swig_setattr(self, iNativeWindowManager,self.__class__,iNativeWindowManager)
_blcelc.iNativeWindowManager_swigregister(iNativeWindowManagerPtr)

class iNativeWindow(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNativeWindow, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNativeWindow, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNativeWindow instance at %s>" % (self.this,)
    def SetTitle(*args): return _blcelc.iNativeWindow_SetTitle(*args)
    def __del__(self, destroy=_blcelc.delete_iNativeWindow):
        try:
            if self.thisown: destroy(self)
        except: pass

class iNativeWindowPtr(iNativeWindow):
    def __init__(self, this):
        _swig_setattr(self, iNativeWindow, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNativeWindow, 'thisown', 0)
        _swig_setattr(self, iNativeWindow,self.__class__,iNativeWindow)
_blcelc.iNativeWindow_swigregister(iNativeWindowPtr)

CSFONT_LARGE = _blcelc.CSFONT_LARGE
CSFONT_ITALIC = _blcelc.CSFONT_ITALIC
CSFONT_COURIER = _blcelc.CSFONT_COURIER
CSFONT_SMALL = _blcelc.CSFONT_SMALL
CS_FONT_DEFAULT_GLYPH = _blcelc.CS_FONT_DEFAULT_GLYPH
class iFontDeleteNotify(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iFontDeleteNotify, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iFontDeleteNotify, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFontDeleteNotify instance at %s>" % (self.this,)
    def BeforeDelete(*args): return _blcelc.iFontDeleteNotify_BeforeDelete(*args)
    def __del__(self, destroy=_blcelc.delete_iFontDeleteNotify):
        try:
            if self.thisown: destroy(self)
        except: pass

class iFontDeleteNotifyPtr(iFontDeleteNotify):
    def __init__(self, this):
        _swig_setattr(self, iFontDeleteNotify, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iFontDeleteNotify, 'thisown', 0)
        _swig_setattr(self, iFontDeleteNotify,self.__class__,iFontDeleteNotify)
_blcelc.iFontDeleteNotify_swigregister(iFontDeleteNotifyPtr)

class csBitmapMetrics(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csBitmapMetrics, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csBitmapMetrics, name)
    def __repr__(self):
        return "<C csBitmapMetrics instance at %s>" % (self.this,)
    __swig_setmethods__["width"] = _blcelc.csBitmapMetrics_width_set
    __swig_getmethods__["width"] = _blcelc.csBitmapMetrics_width_get
    if _newclass:width = property(_blcelc.csBitmapMetrics_width_get, _blcelc.csBitmapMetrics_width_set)
    __swig_setmethods__["height"] = _blcelc.csBitmapMetrics_height_set
    __swig_getmethods__["height"] = _blcelc.csBitmapMetrics_height_get
    if _newclass:height = property(_blcelc.csBitmapMetrics_height_get, _blcelc.csBitmapMetrics_height_set)
    __swig_setmethods__["left"] = _blcelc.csBitmapMetrics_left_set
    __swig_getmethods__["left"] = _blcelc.csBitmapMetrics_left_get
    if _newclass:left = property(_blcelc.csBitmapMetrics_left_get, _blcelc.csBitmapMetrics_left_set)
    __swig_setmethods__["top"] = _blcelc.csBitmapMetrics_top_set
    __swig_getmethods__["top"] = _blcelc.csBitmapMetrics_top_get
    if _newclass:top = property(_blcelc.csBitmapMetrics_top_get, _blcelc.csBitmapMetrics_top_set)
    def __init__(self, *args):
        _swig_setattr(self, csBitmapMetrics, 'this', _blcelc.new_csBitmapMetrics(*args))
        _swig_setattr(self, csBitmapMetrics, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csBitmapMetrics):
        try:
            if self.thisown: destroy(self)
        except: pass

class csBitmapMetricsPtr(csBitmapMetrics):
    def __init__(self, this):
        _swig_setattr(self, csBitmapMetrics, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csBitmapMetrics, 'thisown', 0)
        _swig_setattr(self, csBitmapMetrics,self.__class__,csBitmapMetrics)
_blcelc.csBitmapMetrics_swigregister(csBitmapMetricsPtr)

class csGlyphMetrics(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csGlyphMetrics, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csGlyphMetrics, name)
    def __repr__(self):
        return "<C csGlyphMetrics instance at %s>" % (self.this,)
    __swig_setmethods__["advance"] = _blcelc.csGlyphMetrics_advance_set
    __swig_getmethods__["advance"] = _blcelc.csGlyphMetrics_advance_get
    if _newclass:advance = property(_blcelc.csGlyphMetrics_advance_get, _blcelc.csGlyphMetrics_advance_set)
    def __init__(self, *args):
        _swig_setattr(self, csGlyphMetrics, 'this', _blcelc.new_csGlyphMetrics(*args))
        _swig_setattr(self, csGlyphMetrics, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csGlyphMetrics):
        try:
            if self.thisown: destroy(self)
        except: pass

class csGlyphMetricsPtr(csGlyphMetrics):
    def __init__(self, this):
        _swig_setattr(self, csGlyphMetrics, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csGlyphMetrics, 'thisown', 0)
        _swig_setattr(self, csGlyphMetrics,self.__class__,csGlyphMetrics)
_blcelc.csGlyphMetrics_swigregister(csGlyphMetricsPtr)

class iFont(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iFont, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iFont, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFont instance at %s>" % (self.this,)
    def AddDeleteCallback(*args): return _blcelc.iFont_AddDeleteCallback(*args)
    def RemoveDeleteCallback(*args): return _blcelc.iFont_RemoveDeleteCallback(*args)
    def GetSize(*args): return _blcelc.iFont_GetSize(*args)
    def GetMaxSize(*args): return _blcelc.iFont_GetMaxSize(*args)
    def GetGlyphMetrics(*args): return _blcelc.iFont_GetGlyphMetrics(*args)
    def GetGlyphBitmap(*args): return _blcelc.iFont_GetGlyphBitmap(*args)
    def GetGlyphAlphaBitmap(*args): return _blcelc.iFont_GetGlyphAlphaBitmap(*args)
    def GetDimensions(*args): return _blcelc.iFont_GetDimensions(*args)
    def GetLength(*args): return _blcelc.iFont_GetLength(*args)
    def GetDescent(*args): return _blcelc.iFont_GetDescent(*args)
    def GetAscent(*args): return _blcelc.iFont_GetAscent(*args)
    def HasGlyph(*args): return _blcelc.iFont_HasGlyph(*args)
    def GetTextHeight(*args): return _blcelc.iFont_GetTextHeight(*args)
    def GetUnderlinePosition(*args): return _blcelc.iFont_GetUnderlinePosition(*args)
    def GetUnderlineThickness(*args): return _blcelc.iFont_GetUnderlineThickness(*args)
    def __del__(self, destroy=_blcelc.delete_iFont):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFont_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFont_scfGetVersion)

class iFontPtr(iFont):
    def __init__(self, this):
        _swig_setattr(self, iFont, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iFont, 'thisown', 0)
        _swig_setattr(self, iFont,self.__class__,iFont)
_blcelc.iFont_swigregister(iFontPtr)

iFont_scfGetVersion = _blcelc.iFont_scfGetVersion

class iFontServer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iFontServer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iFontServer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFontServer instance at %s>" % (self.this,)
    def LoadFont(*args): return _blcelc.iFontServer_LoadFont(*args)
    def __del__(self, destroy=_blcelc.delete_iFontServer):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFontServer_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFontServer_scfGetVersion)

class iFontServerPtr(iFontServer):
    def __init__(self, this):
        _swig_setattr(self, iFontServer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iFontServer, 'thisown', 0)
        _swig_setattr(self, iFontServer,self.__class__,iFontServer)
_blcelc.iFontServer_swigregister(iFontServerPtr)

iFontServer_scfGetVersion = _blcelc.iFontServer_scfGetVersion

class iHalo(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iHalo, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iHalo, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iHalo instance at %s>" % (self.this,)
    def GetWidth(*args): return _blcelc.iHalo_GetWidth(*args)
    def GetHeight(*args): return _blcelc.iHalo_GetHeight(*args)
    def SetColor(*args): return _blcelc.iHalo_SetColor(*args)
    def GetColor(*args): return _blcelc.iHalo_GetColor(*args)
    def Draw(*args): return _blcelc.iHalo_Draw(*args)
    def __del__(self, destroy=_blcelc.delete_iHalo):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iHalo_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iHalo_scfGetVersion)

class iHaloPtr(iHalo):
    def __init__(self, this):
        _swig_setattr(self, iHalo, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iHalo, 'thisown', 0)
        _swig_setattr(self, iHalo,self.__class__,iHalo)
_blcelc.iHalo_swigregister(iHaloPtr)

iHalo_scfGetVersion = _blcelc.iHalo_scfGetVersion

class iShaderVariableContext(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderVariableContext, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderVariableContext, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderVariableContext instance at %s>" % (self.this,)
    def AddVariable(*args): return _blcelc.iShaderVariableContext_AddVariable(*args)
    def GetVariable(*args): return _blcelc.iShaderVariableContext_GetVariable(*args)
    def GetVariableAdd(*args): return _blcelc.iShaderVariableContext_GetVariableAdd(*args)
    def GetShaderVariables(*args): return _blcelc.iShaderVariableContext_GetShaderVariables(*args)
    def PushVariables(*args): return _blcelc.iShaderVariableContext_PushVariables(*args)
    def PopVariables(*args): return _blcelc.iShaderVariableContext_PopVariables(*args)
    def __del__(self, destroy=_blcelc.delete_iShaderVariableContext):
        try:
            if self.thisown: destroy(self)
        except: pass

class iShaderVariableContextPtr(iShaderVariableContext):
    def __init__(self, this):
        _swig_setattr(self, iShaderVariableContext, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderVariableContext, 'thisown', 0)
        _swig_setattr(self, iShaderVariableContext,self.__class__,iShaderVariableContext)
_blcelc.iShaderVariableContext_swigregister(iShaderVariableContextPtr)

TagNeutral = _blcelc.TagNeutral
TagForbidden = _blcelc.TagForbidden
TagRequired = _blcelc.TagRequired
class iShaderManager(iShaderVariableContext):
    __swig_setmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderManager instance at %s>" % (self.this,)
    def RegisterShader(*args): return _blcelc.iShaderManager_RegisterShader(*args)
    def UnregisterShader(*args): return _blcelc.iShaderManager_UnregisterShader(*args)
    def GetShader(*args): return _blcelc.iShaderManager_GetShader(*args)
    def GetShaders(*args): return _blcelc.iShaderManager_GetShaders(*args)
    def RegisterCompiler(*args): return _blcelc.iShaderManager_RegisterCompiler(*args)
    def GetCompiler(*args): return _blcelc.iShaderManager_GetCompiler(*args)
    def GetShaderVariableStack(*args): return _blcelc.iShaderManager_GetShaderVariableStack(*args)
    def SetTagOptions(*args): return _blcelc.iShaderManager_SetTagOptions(*args)
    def GetTagOptions(*args): return _blcelc.iShaderManager_GetTagOptions(*args)
    def GetTags(*args): return _blcelc.iShaderManager_GetTags(*args)
    def __del__(self, destroy=_blcelc.delete_iShaderManager):
        try:
            if self.thisown: destroy(self)
        except: pass

class iShaderManagerPtr(iShaderManager):
    def __init__(self, this):
        _swig_setattr(self, iShaderManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderManager, 'thisown', 0)
        _swig_setattr(self, iShaderManager,self.__class__,iShaderManager)
_blcelc.iShaderManager_swigregister(iShaderManagerPtr)

class iShaderRenderInterface(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderRenderInterface, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderRenderInterface, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderRenderInterface instance at %s>" % (self.this,)
    def GetPrivateObject(*args): return _blcelc.iShaderRenderInterface_GetPrivateObject(*args)
    def __del__(self, destroy=_blcelc.delete_iShaderRenderInterface):
        try:
            if self.thisown: destroy(self)
        except: pass

class iShaderRenderInterfacePtr(iShaderRenderInterface):
    def __init__(self, this):
        _swig_setattr(self, iShaderRenderInterface, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderRenderInterface, 'thisown', 0)
        _swig_setattr(self, iShaderRenderInterface,self.__class__,iShaderRenderInterface)
_blcelc.iShaderRenderInterface_swigregister(iShaderRenderInterfacePtr)

class iShader(iShaderVariableContext):
    __swig_setmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShader, name, value)
    __swig_getmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShader, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShader instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iShader_QueryObject(*args)
    def GetFileName(*args): return _blcelc.iShader_GetFileName(*args)
    def SetFileName(*args): return _blcelc.iShader_SetFileName(*args)
    def GetTicket(*args): return _blcelc.iShader_GetTicket(*args)
    def GetNumberOfPasses(*args): return _blcelc.iShader_GetNumberOfPasses(*args)
    def ActivatePass(*args): return _blcelc.iShader_ActivatePass(*args)
    def SetupPass(*args): return _blcelc.iShader_SetupPass(*args)
    def TeardownPass(*args): return _blcelc.iShader_TeardownPass(*args)
    def DeactivatePass(*args): return _blcelc.iShader_DeactivatePass(*args)
    def __del__(self, destroy=_blcelc.delete_iShader):
        try:
            if self.thisown: destroy(self)
        except: pass

class iShaderPtr(iShader):
    def __init__(self, this):
        _swig_setattr(self, iShader, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShader, 'thisown', 0)
        _swig_setattr(self, iShader,self.__class__,iShader)
_blcelc.iShader_swigregister(iShaderPtr)

class iShaderPriorityList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderPriorityList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderPriorityList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderPriorityList instance at %s>" % (self.this,)
    def GetCount(*args): return _blcelc.iShaderPriorityList_GetCount(*args)
    def GetPriority(*args): return _blcelc.iShaderPriorityList_GetPriority(*args)
    def __del__(self, destroy=_blcelc.delete_iShaderPriorityList):
        try:
            if self.thisown: destroy(self)
        except: pass

class iShaderPriorityListPtr(iShaderPriorityList):
    def __init__(self, this):
        _swig_setattr(self, iShaderPriorityList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderPriorityList, 'thisown', 0)
        _swig_setattr(self, iShaderPriorityList,self.__class__,iShaderPriorityList)
_blcelc.iShaderPriorityList_swigregister(iShaderPriorityListPtr)

class iShaderCompiler(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderCompiler, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderCompiler, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderCompiler instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iShaderCompiler_GetName(*args)
    def CompileShader(*args): return _blcelc.iShaderCompiler_CompileShader(*args)
    def ValidateTemplate(*args): return _blcelc.iShaderCompiler_ValidateTemplate(*args)
    def IsTemplateToCompiler(*args): return _blcelc.iShaderCompiler_IsTemplateToCompiler(*args)
    def GetPriorities(*args): return _blcelc.iShaderCompiler_GetPriorities(*args)
    def __del__(self, destroy=_blcelc.delete_iShaderCompiler):
        try:
            if self.thisown: destroy(self)
        except: pass

class iShaderCompilerPtr(iShaderCompiler):
    def __init__(self, this):
        _swig_setattr(self, iShaderCompiler, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderCompiler, 'thisown', 0)
        _swig_setattr(self, iShaderCompiler,self.__class__,iShaderCompiler)
_blcelc.iShaderCompiler_swigregister(iShaderCompilerPtr)

class iTextureHandle(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iTextureHandle, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iTextureHandle, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureHandle instance at %s>" % (self.this,)
    def GetFlags(*args): return _blcelc.iTextureHandle_GetFlags(*args)
    def SetKeyColor(*args): return _blcelc.iTextureHandle_SetKeyColor(*args)
    def GetKeyColorStatus(*args): return _blcelc.iTextureHandle_GetKeyColorStatus(*args)
    def GetKeyColor(*args): return _blcelc.iTextureHandle_GetKeyColor(*args)
    CS_TEX_IMG_1D = _blcelc.iTextureHandle_CS_TEX_IMG_1D
    CS_TEX_IMG_2D = _blcelc.iTextureHandle_CS_TEX_IMG_2D
    CS_TEX_IMG_3D = _blcelc.iTextureHandle_CS_TEX_IMG_3D
    CS_TEX_IMG_CUBEMAP = _blcelc.iTextureHandle_CS_TEX_IMG_CUBEMAP
    CS_TEXTURE_CUBE_POS_X = _blcelc.iTextureHandle_CS_TEXTURE_CUBE_POS_X
    CS_TEXTURE_CUBE_NEG_X = _blcelc.iTextureHandle_CS_TEXTURE_CUBE_NEG_X
    CS_TEXTURE_CUBE_POS_Y = _blcelc.iTextureHandle_CS_TEXTURE_CUBE_POS_Y
    CS_TEXTURE_CUBE_NEG_Y = _blcelc.iTextureHandle_CS_TEXTURE_CUBE_NEG_Y
    CS_TEXTURE_CUBE_POS_Z = _blcelc.iTextureHandle_CS_TEXTURE_CUBE_POS_Z
    CS_TEXTURE_CUBE_NEG_Z = _blcelc.iTextureHandle_CS_TEXTURE_CUBE_NEG_Z
    def GetRendererDimensions(*args): return _blcelc.iTextureHandle_GetRendererDimensions(*args)
    def GetOriginalDimensions(*args): return _blcelc.iTextureHandle_GetOriginalDimensions(*args)
    def GetTextureTarget(*args): return _blcelc.iTextureHandle_GetTextureTarget(*args)
    def Blit(*args): return _blcelc.iTextureHandle_Blit(*args)
    def GetImageName(*args): return _blcelc.iTextureHandle_GetImageName(*args)
    def GetMeanColor(*args): return _blcelc.iTextureHandle_GetMeanColor(*args)
    def GetCacheData(*args): return _blcelc.iTextureHandle_GetCacheData(*args)
    def SetCacheData(*args): return _blcelc.iTextureHandle_SetCacheData(*args)
    def GetPrivateObject(*args): return _blcelc.iTextureHandle_GetPrivateObject(*args)
    def GetAlphaMap(*args): return _blcelc.iTextureHandle_GetAlphaMap(*args)
    def GetAlphaType(*args): return _blcelc.iTextureHandle_GetAlphaType(*args)
    def Precache(*args): return _blcelc.iTextureHandle_Precache(*args)
    def SetTextureClass(*args): return _blcelc.iTextureHandle_SetTextureClass(*args)
    def GetTextureClass(*args): return _blcelc.iTextureHandle_GetTextureClass(*args)
    def SetAlphaType(*args): return _blcelc.iTextureHandle_SetAlphaType(*args)
    def __del__(self, destroy=_blcelc.delete_iTextureHandle):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iTextureHandle_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iTextureHandle_scfGetVersion)

class iTextureHandlePtr(iTextureHandle):
    def __init__(self, this):
        _swig_setattr(self, iTextureHandle, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iTextureHandle, 'thisown', 0)
        _swig_setattr(self, iTextureHandle,self.__class__,iTextureHandle)
_blcelc.iTextureHandle_swigregister(iTextureHandlePtr)

iTextureHandle_scfGetVersion = _blcelc.iTextureHandle_scfGetVersion

CS_TEXTURE_2D = _blcelc.CS_TEXTURE_2D
CS_TEXTURE_3D = _blcelc.CS_TEXTURE_3D
CS_TEXTURE_DITHER = _blcelc.CS_TEXTURE_DITHER
CS_TEXTURE_NOMIPMAPS = _blcelc.CS_TEXTURE_NOMIPMAPS
CS_TEXTURE_CLAMP = _blcelc.CS_TEXTURE_CLAMP
CS_TEXTURE_NOFILTER = _blcelc.CS_TEXTURE_NOFILTER
class iRendererLightmap(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iRendererLightmap, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iRendererLightmap, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iRendererLightmap instance at %s>" % (self.this,)
    def GetSLMCoords(*args): return _blcelc.iRendererLightmap_GetSLMCoords(*args)
    def SetData(*args): return _blcelc.iRendererLightmap_SetData(*args)
    def SetLightCellSize(*args): return _blcelc.iRendererLightmap_SetLightCellSize(*args)
    def __del__(self, destroy=_blcelc.delete_iRendererLightmap):
        try:
            if self.thisown: destroy(self)
        except: pass

class iRendererLightmapPtr(iRendererLightmap):
    def __init__(self, this):
        _swig_setattr(self, iRendererLightmap, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iRendererLightmap, 'thisown', 0)
        _swig_setattr(self, iRendererLightmap,self.__class__,iRendererLightmap)
_blcelc.iRendererLightmap_swigregister(iRendererLightmapPtr)

class iSuperLightmap(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSuperLightmap, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSuperLightmap, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSuperLightmap instance at %s>" % (self.this,)
    def RegisterLightmap(*args): return _blcelc.iSuperLightmap_RegisterLightmap(*args)
    def Dump(*args): return _blcelc.iSuperLightmap_Dump(*args)
    def GetTexture(*args): return _blcelc.iSuperLightmap_GetTexture(*args)
    def __del__(self, destroy=_blcelc.delete_iSuperLightmap):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSuperLightmapPtr(iSuperLightmap):
    def __init__(self, this):
        _swig_setattr(self, iSuperLightmap, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSuperLightmap, 'thisown', 0)
        _swig_setattr(self, iSuperLightmap,self.__class__,iSuperLightmap)
_blcelc.iSuperLightmap_swigregister(iSuperLightmapPtr)

class iTextureManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iTextureManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iTextureManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureManager instance at %s>" % (self.this,)
    def RegisterTexture(*args): return _blcelc.iTextureManager_RegisterTexture(*args)
    def RegisterMaterial(*args): return _blcelc.iTextureManager_RegisterMaterial(*args)
    def FreeMaterials(*args): return _blcelc.iTextureManager_FreeMaterials(*args)
    def GetTextureFormat(*args): return _blcelc.iTextureManager_GetTextureFormat(*args)
    def CreateSuperLightmap(*args): return _blcelc.iTextureManager_CreateSuperLightmap(*args)
    def GetMaxTextureSize(*args): return _blcelc.iTextureManager_GetMaxTextureSize(*args)
    def GetLightmapRendererCoords(*args): return _blcelc.iTextureManager_GetLightmapRendererCoords(*args)
    def __del__(self, destroy=_blcelc.delete_iTextureManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iTextureManager_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iTextureManager_scfGetVersion)

class iTextureManagerPtr(iTextureManager):
    def __init__(self, this):
        _swig_setattr(self, iTextureManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iTextureManager, 'thisown', 0)
        _swig_setattr(self, iTextureManager,self.__class__,iTextureManager)
_blcelc.iTextureManager_swigregister(iTextureManagerPtr)

iTextureManager_scfGetVersion = _blcelc.iTextureManager_scfGetVersion

CS_DEFMAT_DIFFUSE = _blcelc.CS_DEFMAT_DIFFUSE
CS_DEFMAT_AMBIENT = _blcelc.CS_DEFMAT_AMBIENT
CS_DEFMAT_REFLECTION = _blcelc.CS_DEFMAT_REFLECTION
CS_MATERIAL_VARNAME_DIFFUSE = _blcelc.CS_MATERIAL_VARNAME_DIFFUSE
CS_MATERIAL_VARNAME_AMBIENT = _blcelc.CS_MATERIAL_VARNAME_AMBIENT
CS_MATERIAL_VARNAME_REFLECTION = _blcelc.CS_MATERIAL_VARNAME_REFLECTION
CS_MATERIAL_VARNAME_FLATCOLOR = _blcelc.CS_MATERIAL_VARNAME_FLATCOLOR
CS_MATERIAL_TEXTURE_DIFFUSE = _blcelc.CS_MATERIAL_TEXTURE_DIFFUSE
CS_MATERIAL_TEXTURE_LAYER1 = _blcelc.CS_MATERIAL_TEXTURE_LAYER1
CS_MATERIAL_TEXTURE_LAYER2 = _blcelc.CS_MATERIAL_TEXTURE_LAYER2
CS_MATERIAL_TEXTURE_LAYER3 = _blcelc.CS_MATERIAL_TEXTURE_LAYER3
CS_MATERIAL_TEXTURE_LAYER4 = _blcelc.CS_MATERIAL_TEXTURE_LAYER4
class csTextureLayer(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csTextureLayer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csTextureLayer, name)
    def __repr__(self):
        return "<C csTextureLayer instance at %s>" % (self.this,)
    __swig_setmethods__["txt_handle"] = _blcelc.csTextureLayer_txt_handle_set
    __swig_getmethods__["txt_handle"] = _blcelc.csTextureLayer_txt_handle_get
    if _newclass:txt_handle = property(_blcelc.csTextureLayer_txt_handle_get, _blcelc.csTextureLayer_txt_handle_set)
    __swig_setmethods__["mode"] = _blcelc.csTextureLayer_mode_set
    __swig_getmethods__["mode"] = _blcelc.csTextureLayer_mode_get
    if _newclass:mode = property(_blcelc.csTextureLayer_mode_get, _blcelc.csTextureLayer_mode_set)
    __swig_setmethods__["uscale"] = _blcelc.csTextureLayer_uscale_set
    __swig_getmethods__["uscale"] = _blcelc.csTextureLayer_uscale_get
    if _newclass:uscale = property(_blcelc.csTextureLayer_uscale_get, _blcelc.csTextureLayer_uscale_set)
    __swig_setmethods__["vscale"] = _blcelc.csTextureLayer_vscale_set
    __swig_getmethods__["vscale"] = _blcelc.csTextureLayer_vscale_get
    if _newclass:vscale = property(_blcelc.csTextureLayer_vscale_get, _blcelc.csTextureLayer_vscale_set)
    __swig_setmethods__["ushift"] = _blcelc.csTextureLayer_ushift_set
    __swig_getmethods__["ushift"] = _blcelc.csTextureLayer_ushift_get
    if _newclass:ushift = property(_blcelc.csTextureLayer_ushift_get, _blcelc.csTextureLayer_ushift_set)
    __swig_setmethods__["vshift"] = _blcelc.csTextureLayer_vshift_set
    __swig_getmethods__["vshift"] = _blcelc.csTextureLayer_vshift_get
    if _newclass:vshift = property(_blcelc.csTextureLayer_vshift_get, _blcelc.csTextureLayer_vshift_set)
    def __init__(self, *args):
        _swig_setattr(self, csTextureLayer, 'this', _blcelc.new_csTextureLayer(*args))
        _swig_setattr(self, csTextureLayer, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csTextureLayer):
        try:
            if self.thisown: destroy(self)
        except: pass

class csTextureLayerPtr(csTextureLayer):
    def __init__(self, this):
        _swig_setattr(self, csTextureLayer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csTextureLayer, 'thisown', 0)
        _swig_setattr(self, csTextureLayer,self.__class__,csTextureLayer)
_blcelc.csTextureLayer_swigregister(csTextureLayerPtr)

class iMaterial(iShaderVariableContext):
    __swig_setmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMaterial, name, value)
    __swig_getmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMaterial, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterial instance at %s>" % (self.this,)
    def SetShader(*args): return _blcelc.iMaterial_SetShader(*args)
    def GetShader(*args): return _blcelc.iMaterial_GetShader(*args)
    def GetShaders(*args): return _blcelc.iMaterial_GetShaders(*args)
    def GetTexture(*args): return _blcelc.iMaterial_GetTexture(*args)
    def GetTextureLayerCount(*args): return _blcelc.iMaterial_GetTextureLayerCount(*args)
    def GetTextureLayer(*args): return _blcelc.iMaterial_GetTextureLayer(*args)
    def GetFlatColor(*args): return _blcelc.iMaterial_GetFlatColor(*args)
    def SetFlatColor(*args): return _blcelc.iMaterial_SetFlatColor(*args)
    def GetReflection(*args): return _blcelc.iMaterial_GetReflection(*args)
    def SetReflection(*args): return _blcelc.iMaterial_SetReflection(*args)
    def __del__(self, destroy=_blcelc.delete_iMaterial):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMaterial_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMaterial_scfGetVersion)

class iMaterialPtr(iMaterial):
    def __init__(self, this):
        _swig_setattr(self, iMaterial, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMaterial, 'thisown', 0)
        _swig_setattr(self, iMaterial,self.__class__,iMaterial)
_blcelc.iMaterial_swigregister(iMaterialPtr)

iMaterial_scfGetVersion = _blcelc.iMaterial_scfGetVersion

class iMaterialHandle(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMaterialHandle, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMaterialHandle, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialHandle instance at %s>" % (self.this,)
    def GetShader(*args): return _blcelc.iMaterialHandle_GetShader(*args)
    def GetTexture(*args): return _blcelc.iMaterialHandle_GetTexture(*args)
    def GetFlatColor(*args): return _blcelc.iMaterialHandle_GetFlatColor(*args)
    def GetReflection(*args): return _blcelc.iMaterialHandle_GetReflection(*args)
    def __del__(self, destroy=_blcelc.delete_iMaterialHandle):
        try:
            if self.thisown: destroy(self)
        except: pass

class iMaterialHandlePtr(iMaterialHandle):
    def __init__(self, this):
        _swig_setattr(self, iMaterialHandle, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMaterialHandle, 'thisown', 0)
        _swig_setattr(self, iMaterialHandle,self.__class__,iMaterialHandle)
_blcelc.iMaterialHandle_swigregister(iMaterialHandlePtr)

CS_POS_BY_FRAME = _blcelc.CS_POS_BY_FRAME
CS_POS_BY_TIME = _blcelc.CS_POS_BY_TIME
CS_DECODE_SPAN = _blcelc.CS_DECODE_SPAN
CS_DYNAMIC_FRAMESIZE = _blcelc.CS_DYNAMIC_FRAMESIZE
CS_STREAMTYPE_AUDIO = _blcelc.CS_STREAMTYPE_AUDIO
CS_STREAMTYPE_VIDEO = _blcelc.CS_STREAMTYPE_VIDEO
CS_STREAMTYPE_MIDI = _blcelc.CS_STREAMTYPE_MIDI
CS_STREAMTYPE_TEXT = _blcelc.CS_STREAMTYPE_TEXT
class csStreamDescription(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csStreamDescription, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csStreamDescription, name)
    def __repr__(self):
        return "<C csStreamDescription instance at %s>" % (self.this,)
    __swig_setmethods__["type"] = _blcelc.csStreamDescription_type_set
    __swig_getmethods__["type"] = _blcelc.csStreamDescription_type_get
    if _newclass:type = property(_blcelc.csStreamDescription_type_get, _blcelc.csStreamDescription_type_set)
    __swig_setmethods__["codec"] = _blcelc.csStreamDescription_codec_set
    __swig_getmethods__["codec"] = _blcelc.csStreamDescription_codec_get
    if _newclass:codec = property(_blcelc.csStreamDescription_codec_get, _blcelc.csStreamDescription_codec_set)
    __swig_getmethods__["name"] = _blcelc.csStreamDescription_name_get
    if _newclass:name = property(_blcelc.csStreamDescription_name_get)
    def __init__(self, *args):
        _swig_setattr(self, csStreamDescription, 'this', _blcelc.new_csStreamDescription(*args))
        _swig_setattr(self, csStreamDescription, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csStreamDescription):
        try:
            if self.thisown: destroy(self)
        except: pass

class csStreamDescriptionPtr(csStreamDescription):
    def __init__(self, this):
        _swig_setattr(self, csStreamDescription, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csStreamDescription, 'thisown', 0)
        _swig_setattr(self, csStreamDescription,self.__class__,csStreamDescription)
_blcelc.csStreamDescription_swigregister(csStreamDescriptionPtr)

class csVideoStreamDescription(csStreamDescription):
    __swig_setmethods__ = {}
    for _s in [csStreamDescription]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csVideoStreamDescription, name, value)
    __swig_getmethods__ = {}
    for _s in [csStreamDescription]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csVideoStreamDescription, name)
    def __repr__(self):
        return "<C csVideoStreamDescription instance at %s>" % (self.this,)
    __swig_setmethods__["colordepth"] = _blcelc.csVideoStreamDescription_colordepth_set
    __swig_getmethods__["colordepth"] = _blcelc.csVideoStreamDescription_colordepth_get
    if _newclass:colordepth = property(_blcelc.csVideoStreamDescription_colordepth_get, _blcelc.csVideoStreamDescription_colordepth_set)
    __swig_setmethods__["framecount"] = _blcelc.csVideoStreamDescription_framecount_set
    __swig_getmethods__["framecount"] = _blcelc.csVideoStreamDescription_framecount_get
    if _newclass:framecount = property(_blcelc.csVideoStreamDescription_framecount_get, _blcelc.csVideoStreamDescription_framecount_set)
    __swig_setmethods__["width"] = _blcelc.csVideoStreamDescription_width_set
    __swig_getmethods__["width"] = _blcelc.csVideoStreamDescription_width_get
    if _newclass:width = property(_blcelc.csVideoStreamDescription_width_get, _blcelc.csVideoStreamDescription_width_set)
    __swig_setmethods__["height"] = _blcelc.csVideoStreamDescription_height_set
    __swig_getmethods__["height"] = _blcelc.csVideoStreamDescription_height_get
    if _newclass:height = property(_blcelc.csVideoStreamDescription_height_get, _blcelc.csVideoStreamDescription_height_set)
    __swig_setmethods__["framerate"] = _blcelc.csVideoStreamDescription_framerate_set
    __swig_getmethods__["framerate"] = _blcelc.csVideoStreamDescription_framerate_get
    if _newclass:framerate = property(_blcelc.csVideoStreamDescription_framerate_get, _blcelc.csVideoStreamDescription_framerate_set)
    __swig_setmethods__["duration"] = _blcelc.csVideoStreamDescription_duration_set
    __swig_getmethods__["duration"] = _blcelc.csVideoStreamDescription_duration_get
    if _newclass:duration = property(_blcelc.csVideoStreamDescription_duration_get, _blcelc.csVideoStreamDescription_duration_set)
    def __init__(self, *args):
        _swig_setattr(self, csVideoStreamDescription, 'this', _blcelc.new_csVideoStreamDescription(*args))
        _swig_setattr(self, csVideoStreamDescription, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csVideoStreamDescription):
        try:
            if self.thisown: destroy(self)
        except: pass

class csVideoStreamDescriptionPtr(csVideoStreamDescription):
    def __init__(self, this):
        _swig_setattr(self, csVideoStreamDescription, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csVideoStreamDescription, 'thisown', 0)
        _swig_setattr(self, csVideoStreamDescription,self.__class__,csVideoStreamDescription)
_blcelc.csVideoStreamDescription_swigregister(csVideoStreamDescriptionPtr)

class csAudioStreamDescription(csStreamDescription):
    __swig_setmethods__ = {}
    for _s in [csStreamDescription]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csAudioStreamDescription, name, value)
    __swig_getmethods__ = {}
    for _s in [csStreamDescription]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csAudioStreamDescription, name)
    def __repr__(self):
        return "<C csAudioStreamDescription instance at %s>" % (self.this,)
    __swig_setmethods__["formattag"] = _blcelc.csAudioStreamDescription_formattag_set
    __swig_getmethods__["formattag"] = _blcelc.csAudioStreamDescription_formattag_get
    if _newclass:formattag = property(_blcelc.csAudioStreamDescription_formattag_get, _blcelc.csAudioStreamDescription_formattag_set)
    __swig_setmethods__["channels"] = _blcelc.csAudioStreamDescription_channels_set
    __swig_getmethods__["channels"] = _blcelc.csAudioStreamDescription_channels_get
    if _newclass:channels = property(_blcelc.csAudioStreamDescription_channels_get, _blcelc.csAudioStreamDescription_channels_set)
    __swig_setmethods__["samplespersecond"] = _blcelc.csAudioStreamDescription_samplespersecond_set
    __swig_getmethods__["samplespersecond"] = _blcelc.csAudioStreamDescription_samplespersecond_get
    if _newclass:samplespersecond = property(_blcelc.csAudioStreamDescription_samplespersecond_get, _blcelc.csAudioStreamDescription_samplespersecond_set)
    __swig_setmethods__["bitspersample"] = _blcelc.csAudioStreamDescription_bitspersample_set
    __swig_getmethods__["bitspersample"] = _blcelc.csAudioStreamDescription_bitspersample_get
    if _newclass:bitspersample = property(_blcelc.csAudioStreamDescription_bitspersample_get, _blcelc.csAudioStreamDescription_bitspersample_set)
    __swig_setmethods__["duration"] = _blcelc.csAudioStreamDescription_duration_set
    __swig_getmethods__["duration"] = _blcelc.csAudioStreamDescription_duration_get
    if _newclass:duration = property(_blcelc.csAudioStreamDescription_duration_get, _blcelc.csAudioStreamDescription_duration_set)
    def __init__(self, *args):
        _swig_setattr(self, csAudioStreamDescription, 'this', _blcelc.new_csAudioStreamDescription(*args))
        _swig_setattr(self, csAudioStreamDescription, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csAudioStreamDescription):
        try:
            if self.thisown: destroy(self)
        except: pass

class csAudioStreamDescriptionPtr(csAudioStreamDescription):
    def __init__(self, this):
        _swig_setattr(self, csAudioStreamDescription, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csAudioStreamDescription, 'thisown', 0)
        _swig_setattr(self, csAudioStreamDescription,self.__class__,csAudioStreamDescription)
_blcelc.csAudioStreamDescription_swigregister(csAudioStreamDescriptionPtr)

class iStreamIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iStreamIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iStreamIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStreamIterator instance at %s>" % (self.this,)
    def HasNext(*args): return _blcelc.iStreamIterator_HasNext(*args)
    def Next(*args): return _blcelc.iStreamIterator_Next(*args)
    def __del__(self, destroy=_blcelc.delete_iStreamIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStreamIterator_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStreamIterator_scfGetVersion)

class iStreamIteratorPtr(iStreamIterator):
    def __init__(self, this):
        _swig_setattr(self, iStreamIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iStreamIterator, 'thisown', 0)
        _swig_setattr(self, iStreamIterator,self.__class__,iStreamIterator)
_blcelc.iStreamIterator_swigregister(iStreamIteratorPtr)

iStreamIterator_scfGetVersion = _blcelc.iStreamIterator_scfGetVersion

class iStreamFormat(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iStreamFormat, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iStreamFormat, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStreamFormat instance at %s>" % (self.this,)
    def GetCaps(*args): return _blcelc.iStreamFormat_GetCaps(*args)
    def GetStreamIterator(*args): return _blcelc.iStreamFormat_GetStreamIterator(*args)
    def Select(*args): return _blcelc.iStreamFormat_Select(*args)
    def NextFrame(*args): return _blcelc.iStreamFormat_NextFrame(*args)
    def Load(*args): return _blcelc.iStreamFormat_Load(*args)
    def Unload(*args): return _blcelc.iStreamFormat_Unload(*args)
    def __del__(self, destroy=_blcelc.delete_iStreamFormat):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStreamFormat_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStreamFormat_scfGetVersion)

class iStreamFormatPtr(iStreamFormat):
    def __init__(self, this):
        _swig_setattr(self, iStreamFormat, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iStreamFormat, 'thisown', 0)
        _swig_setattr(self, iStreamFormat,self.__class__,iStreamFormat)
_blcelc.iStreamFormat_swigregister(iStreamFormatPtr)

iStreamFormat_scfGetVersion = _blcelc.iStreamFormat_scfGetVersion

class iStream(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iStream, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iStream, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStream instance at %s>" % (self.this,)
    def GetStreamDescription(*args): return _blcelc.iStream_GetStreamDescription(*args)
    def GotoFrame(*args): return _blcelc.iStream_GotoFrame(*args)
    def GotoTime(*args): return _blcelc.iStream_GotoTime(*args)
    def SetPlayMethod(*args): return _blcelc.iStream_SetPlayMethod(*args)
    def NextFrame(*args): return _blcelc.iStream_NextFrame(*args)
    def __del__(self, destroy=_blcelc.delete_iStream):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStream_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStream_scfGetVersion)

class iStreamPtr(iStream):
    def __init__(self, this):
        _swig_setattr(self, iStream, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iStream, 'thisown', 0)
        _swig_setattr(self, iStream,self.__class__,iStream)
_blcelc.iStream_swigregister(iStreamPtr)

iStream_scfGetVersion = _blcelc.iStream_scfGetVersion

class iVideoStream(iStream):
    __swig_setmethods__ = {}
    for _s in [iStream]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVideoStream, name, value)
    __swig_getmethods__ = {}
    for _s in [iStream]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVideoStream, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVideoStream instance at %s>" % (self.this,)
    def GetStreamDescription(*args): return _blcelc.iVideoStream_GetStreamDescription(*args)
    def SetRect(*args): return _blcelc.iVideoStream_SetRect(*args)
    def SetFXMode(*args): return _blcelc.iVideoStream_SetFXMode(*args)
    def NextFrameGetMaterial(*args): return _blcelc.iVideoStream_NextFrameGetMaterial(*args)
    def __del__(self, destroy=_blcelc.delete_iVideoStream):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iVideoStream_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iVideoStream_scfGetVersion)

class iVideoStreamPtr(iVideoStream):
    def __init__(self, this):
        _swig_setattr(self, iVideoStream, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVideoStream, 'thisown', 0)
        _swig_setattr(self, iVideoStream,self.__class__,iVideoStream)
_blcelc.iVideoStream_swigregister(iVideoStreamPtr)

iVideoStream_scfGetVersion = _blcelc.iVideoStream_scfGetVersion

class iAudioStream(iStream):
    __swig_setmethods__ = {}
    for _s in [iStream]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAudioStream, name, value)
    __swig_getmethods__ = {}
    for _s in [iStream]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAudioStream, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAudioStream instance at %s>" % (self.this,)
    def GetStreamDescription(*args): return _blcelc.iAudioStream_GetStreamDescription(*args)
    def __del__(self, destroy=_blcelc.delete_iAudioStream):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iAudioStream_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iAudioStream_scfGetVersion)

class iAudioStreamPtr(iAudioStream):
    def __init__(self, this):
        _swig_setattr(self, iAudioStream, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAudioStream, 'thisown', 0)
        _swig_setattr(self, iAudioStream,self.__class__,iAudioStream)
_blcelc.iAudioStream_swigregister(iAudioStreamPtr)

iAudioStream_scfGetVersion = _blcelc.iAudioStream_scfGetVersion

CS_CODECFORMAT_RGB_CHANNEL = _blcelc.CS_CODECFORMAT_RGB_CHANNEL
CS_CODECFORMAT_RGBA_CHANNEL = _blcelc.CS_CODECFORMAT_RGBA_CHANNEL
CS_CODECFORMAT_YUV_CHANNEL = _blcelc.CS_CODECFORMAT_YUV_CHANNEL
CS_CODECFORMAT_RGB_INTERLEAVED = _blcelc.CS_CODECFORMAT_RGB_INTERLEAVED
CS_CODECFORMAT_RGBA_INTERLEAVED = _blcelc.CS_CODECFORMAT_RGBA_INTERLEAVED
CS_CODECFORMAT_YUV_INTERLEAVED = _blcelc.CS_CODECFORMAT_YUV_INTERLEAVED
class csCodecDescription(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csCodecDescription, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csCodecDescription, name)
    def __repr__(self):
        return "<C csCodecDescription instance at %s>" % (self.this,)
    __swig_setmethods__["codec"] = _blcelc.csCodecDescription_codec_set
    __swig_getmethods__["codec"] = _blcelc.csCodecDescription_codec_get
    if _newclass:codec = property(_blcelc.csCodecDescription_codec_get, _blcelc.csCodecDescription_codec_set)
    __swig_setmethods__["bEncode"] = _blcelc.csCodecDescription_bEncode_set
    __swig_getmethods__["bEncode"] = _blcelc.csCodecDescription_bEncode_get
    if _newclass:bEncode = property(_blcelc.csCodecDescription_bEncode_get, _blcelc.csCodecDescription_bEncode_set)
    __swig_setmethods__["bDecode"] = _blcelc.csCodecDescription_bDecode_set
    __swig_getmethods__["bDecode"] = _blcelc.csCodecDescription_bDecode_get
    if _newclass:bDecode = property(_blcelc.csCodecDescription_bDecode_get, _blcelc.csCodecDescription_bDecode_set)
    __swig_setmethods__["decodeoutput"] = _blcelc.csCodecDescription_decodeoutput_set
    __swig_getmethods__["decodeoutput"] = _blcelc.csCodecDescription_decodeoutput_get
    if _newclass:decodeoutput = property(_blcelc.csCodecDescription_decodeoutput_get, _blcelc.csCodecDescription_decodeoutput_set)
    __swig_setmethods__["encodeinput"] = _blcelc.csCodecDescription_encodeinput_set
    __swig_getmethods__["encodeinput"] = _blcelc.csCodecDescription_encodeinput_get
    if _newclass:encodeinput = property(_blcelc.csCodecDescription_encodeinput_get, _blcelc.csCodecDescription_encodeinput_set)
    def __init__(self, *args):
        _swig_setattr(self, csCodecDescription, 'this', _blcelc.new_csCodecDescription(*args))
        _swig_setattr(self, csCodecDescription, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csCodecDescription):
        try:
            if self.thisown: destroy(self)
        except: pass

class csCodecDescriptionPtr(csCodecDescription):
    def __init__(self, this):
        _swig_setattr(self, csCodecDescription, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csCodecDescription, 'thisown', 0)
        _swig_setattr(self, csCodecDescription,self.__class__,csCodecDescription)
_blcelc.csCodecDescription_swigregister(csCodecDescriptionPtr)

CS_IMGFMT_MASK = _blcelc.CS_IMGFMT_MASK
CS_IMGFMT_NONE = _blcelc.CS_IMGFMT_NONE
CS_IMGFMT_TRUECOLOR = _blcelc.CS_IMGFMT_TRUECOLOR
CS_IMGFMT_PALETTED8 = _blcelc.CS_IMGFMT_PALETTED8
CS_IMGFMT_ANY = _blcelc.CS_IMGFMT_ANY
CS_IMGFMT_ALPHA = _blcelc.CS_IMGFMT_ALPHA
CS_IMGFMT_INVALID = _blcelc.CS_IMGFMT_INVALID
csimg2D = _blcelc.csimg2D
csimg3D = _blcelc.csimg3D
csimgCube = _blcelc.csimgCube
class iImage(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iImage, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iImage, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iImage instance at %s>" % (self.this,)
    def GetImageData(*args): return _blcelc.iImage_GetImageData(*args)
    def GetWidth(*args): return _blcelc.iImage_GetWidth(*args)
    def GetHeight(*args): return _blcelc.iImage_GetHeight(*args)
    def GetDepth(*args): return _blcelc.iImage_GetDepth(*args)
    def SetName(*args): return _blcelc.iImage_SetName(*args)
    def GetName(*args): return _blcelc.iImage_GetName(*args)
    def GetFormat(*args): return _blcelc.iImage_GetFormat(*args)
    def GetPalette(*args): return _blcelc.iImage_GetPalette(*args)
    def GetAlpha(*args): return _blcelc.iImage_GetAlpha(*args)
    def HasKeyColor(*args): return _blcelc.iImage_HasKeyColor(*args)
    def HasKeycolor(*args): return _blcelc.iImage_HasKeycolor(*args)
    def GetKeyColor(*args): return _blcelc.iImage_GetKeyColor(*args)
    def GetKeycolor(*args): return _blcelc.iImage_GetKeycolor(*args)
    def HasMipmaps(*args): return _blcelc.iImage_HasMipmaps(*args)
    def GetMipmap(*args): return _blcelc.iImage_GetMipmap(*args)
    def GetRawFormat(*args): return _blcelc.iImage_GetRawFormat(*args)
    def GetRawData(*args): return _blcelc.iImage_GetRawData(*args)
    def GetImageType(*args): return _blcelc.iImage_GetImageType(*args)
    def HasSubImages(*args): return _blcelc.iImage_HasSubImages(*args)
    def GetSubImage(*args): return _blcelc.iImage_GetSubImage(*args)
    def __del__(self, destroy=_blcelc.delete_iImage):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iImage_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iImage_scfGetVersion)

class iImagePtr(iImage):
    def __init__(self, this):
        _swig_setattr(self, iImage, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iImage, 'thisown', 0)
        _swig_setattr(self, iImage,self.__class__,iImage)
_blcelc.iImage_swigregister(iImagePtr)

iImage_scfGetVersion = _blcelc.iImage_scfGetVersion

CS_IMAGEIO_LOAD = _blcelc.CS_IMAGEIO_LOAD
CS_IMAGEIO_SAVE = _blcelc.CS_IMAGEIO_SAVE
class csImageIOFileFormatDescription(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csImageIOFileFormatDescription, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csImageIOFileFormatDescription, name)
    def __repr__(self):
        return "<C csImageIOFileFormatDescription instance at %s>" % (self.this,)
    __swig_getmethods__["mime"] = _blcelc.csImageIOFileFormatDescription_mime_get
    if _newclass:mime = property(_blcelc.csImageIOFileFormatDescription_mime_get)
    __swig_getmethods__["subtype"] = _blcelc.csImageIOFileFormatDescription_subtype_get
    if _newclass:subtype = property(_blcelc.csImageIOFileFormatDescription_subtype_get)
    __swig_setmethods__["cap"] = _blcelc.csImageIOFileFormatDescription_cap_set
    __swig_getmethods__["cap"] = _blcelc.csImageIOFileFormatDescription_cap_get
    if _newclass:cap = property(_blcelc.csImageIOFileFormatDescription_cap_get, _blcelc.csImageIOFileFormatDescription_cap_set)
    def __init__(self, *args):
        _swig_setattr(self, csImageIOFileFormatDescription, 'this', _blcelc.new_csImageIOFileFormatDescription(*args))
        _swig_setattr(self, csImageIOFileFormatDescription, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csImageIOFileFormatDescription):
        try:
            if self.thisown: destroy(self)
        except: pass

class csImageIOFileFormatDescriptionPtr(csImageIOFileFormatDescription):
    def __init__(self, this):
        _swig_setattr(self, csImageIOFileFormatDescription, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csImageIOFileFormatDescription, 'thisown', 0)
        _swig_setattr(self, csImageIOFileFormatDescription,self.__class__,csImageIOFileFormatDescription)
_blcelc.csImageIOFileFormatDescription_swigregister(csImageIOFileFormatDescriptionPtr)

class iImageIO(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iImageIO, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iImageIO, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iImageIO instance at %s>" % (self.this,)
    def GetDescription(*args): return _blcelc.iImageIO_GetDescription(*args)
    def Load(*args): return _blcelc.iImageIO_Load(*args)
    def SetDithering(*args): return _blcelc.iImageIO_SetDithering(*args)
    def Save(*args): return _blcelc.iImageIO_Save(*args)
    def __del__(self, destroy=_blcelc.delete_iImageIO):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iImageIO_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iImageIO_scfGetVersion)

class iImageIOPtr(iImageIO):
    def __init__(self, this):
        _swig_setattr(self, iImageIO, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iImageIO, 'thisown', 0)
        _swig_setattr(self, iImageIO,self.__class__,iImageIO)
_blcelc.iImageIO_swigregister(iImageIOPtr)

iImageIO_scfGetVersion = _blcelc.iImageIO_scfGetVersion

CS_REPORTER_SEVERITY_BUG = _blcelc.CS_REPORTER_SEVERITY_BUG
CS_REPORTER_SEVERITY_ERROR = _blcelc.CS_REPORTER_SEVERITY_ERROR
CS_REPORTER_SEVERITY_WARNING = _blcelc.CS_REPORTER_SEVERITY_WARNING
CS_REPORTER_SEVERITY_NOTIFY = _blcelc.CS_REPORTER_SEVERITY_NOTIFY
CS_REPORTER_SEVERITY_DEBUG = _blcelc.CS_REPORTER_SEVERITY_DEBUG
class iReporterListener(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iReporterListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iReporterListener, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iReporterListener instance at %s>" % (self.this,)
    def Report(*args): return _blcelc.iReporterListener_Report(*args)
    def __del__(self, destroy=_blcelc.delete_iReporterListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iReporterListener_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iReporterListener_scfGetVersion)

class iReporterListenerPtr(iReporterListener):
    def __init__(self, this):
        _swig_setattr(self, iReporterListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iReporterListener, 'thisown', 0)
        _swig_setattr(self, iReporterListener,self.__class__,iReporterListener)
_blcelc.iReporterListener_swigregister(iReporterListenerPtr)

iReporterListener_scfGetVersion = _blcelc.iReporterListener_scfGetVersion

class iReporterIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iReporterIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iReporterIterator, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iReporterIterator instance at %s>" % (self.this,)
    def HasNext(*args): return _blcelc.iReporterIterator_HasNext(*args)
    def Next(*args): return _blcelc.iReporterIterator_Next(*args)
    def GetMessageSeverity(*args): return _blcelc.iReporterIterator_GetMessageSeverity(*args)
    def GetMessageId(*args): return _blcelc.iReporterIterator_GetMessageId(*args)
    def GetMessageDescription(*args): return _blcelc.iReporterIterator_GetMessageDescription(*args)
    def __del__(self, destroy=_blcelc.delete_iReporterIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iReporterIterator_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iReporterIterator_scfGetVersion)

class iReporterIteratorPtr(iReporterIterator):
    def __init__(self, this):
        _swig_setattr(self, iReporterIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iReporterIterator, 'thisown', 0)
        _swig_setattr(self, iReporterIterator,self.__class__,iReporterIterator)
_blcelc.iReporterIterator_swigregister(iReporterIteratorPtr)

iReporterIterator_scfGetVersion = _blcelc.iReporterIterator_scfGetVersion

class iReporter(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iReporter, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iReporter, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iReporter instance at %s>" % (self.this,)
    def Report(*args): return _blcelc.iReporter_Report(*args)
    def Clear(*args): return _blcelc.iReporter_Clear(*args)
    def GetMessageIterator(*args): return _blcelc.iReporter_GetMessageIterator(*args)
    def AddReporterListener(*args): return _blcelc.iReporter_AddReporterListener(*args)
    def RemoveReporterListener(*args): return _blcelc.iReporter_RemoveReporterListener(*args)
    def FindReporterListener(*args): return _blcelc.iReporter_FindReporterListener(*args)
    def ReportError(*args): return _blcelc.iReporter_ReportError(*args)
    def ReportWarning(*args): return _blcelc.iReporter_ReportWarning(*args)
    def ReportNotify(*args): return _blcelc.iReporter_ReportNotify(*args)
    def ReportBug(*args): return _blcelc.iReporter_ReportBug(*args)
    def ReportDebug(*args): return _blcelc.iReporter_ReportDebug(*args)
    def __del__(self, destroy=_blcelc.delete_iReporter):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iReporter_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iReporter_scfGetVersion)

class iReporterPtr(iReporter):
    def __init__(self, this):
        _swig_setattr(self, iReporter, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iReporter, 'thisown', 0)
        _swig_setattr(self, iReporter,self.__class__,iReporter)
_blcelc.iReporter_swigregister(iReporterPtr)

iReporter_scfGetVersion = _blcelc.iReporter_scfGetVersion

class csReporterHelper(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csReporterHelper, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csReporterHelper, name)
    def __repr__(self):
        return "<C csReporterHelper instance at %s>" % (self.this,)
    __swig_getmethods__["Report"] = lambda x: _blcelc.csReporterHelper_Report
    if _newclass:Report = staticmethod(_blcelc.csReporterHelper_Report)
    def __init__(self, *args):
        _swig_setattr(self, csReporterHelper, 'this', _blcelc.new_csReporterHelper(*args))
        _swig_setattr(self, csReporterHelper, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csReporterHelper):
        try:
            if self.thisown: destroy(self)
        except: pass

class csReporterHelperPtr(csReporterHelper):
    def __init__(self, this):
        _swig_setattr(self, csReporterHelper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csReporterHelper, 'thisown', 0)
        _swig_setattr(self, csReporterHelper,self.__class__,csReporterHelper)
_blcelc.csReporterHelper_swigregister(csReporterHelperPtr)

csReporterHelper_Report = _blcelc.csReporterHelper_Report

csConPageUp = _blcelc.csConPageUp
csConPageDown = _blcelc.csConPageDown
csConVeryTop = _blcelc.csConVeryTop
csConVeryBottom = _blcelc.csConVeryBottom
csConNoCursor = _blcelc.csConNoCursor
csConNormalCursor = _blcelc.csConNormalCursor
csConInsertCursor = _blcelc.csConInsertCursor
class iConsoleWatcher(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iConsoleWatcher, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iConsoleWatcher, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConsoleWatcher instance at %s>" % (self.this,)
    def ConsoleVisibilityChanged(*args): return _blcelc.iConsoleWatcher_ConsoleVisibilityChanged(*args)
    def __del__(self, destroy=_blcelc.delete_iConsoleWatcher):
        try:
            if self.thisown: destroy(self)
        except: pass

class iConsoleWatcherPtr(iConsoleWatcher):
    def __init__(self, this):
        _swig_setattr(self, iConsoleWatcher, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iConsoleWatcher, 'thisown', 0)
        _swig_setattr(self, iConsoleWatcher,self.__class__,iConsoleWatcher)
_blcelc.iConsoleWatcher_swigregister(iConsoleWatcherPtr)

class iConsoleOutput(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iConsoleOutput, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iConsoleOutput, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConsoleOutput instance at %s>" % (self.this,)
    def PutText(*args): return _blcelc.iConsoleOutput_PutText(*args)
    def GetLine(*args): return _blcelc.iConsoleOutput_GetLine(*args)
    def Draw2D(*args): return _blcelc.iConsoleOutput_Draw2D(*args)
    def Draw3D(*args): return _blcelc.iConsoleOutput_Draw3D(*args)
    def Clear(*args): return _blcelc.iConsoleOutput_Clear(*args)
    def SetBufferSize(*args): return _blcelc.iConsoleOutput_SetBufferSize(*args)
    def GetTransparency(*args): return _blcelc.iConsoleOutput_GetTransparency(*args)
    def SetTransparency(*args): return _blcelc.iConsoleOutput_SetTransparency(*args)
    def GetFont(*args): return _blcelc.iConsoleOutput_GetFont(*args)
    def SetFont(*args): return _blcelc.iConsoleOutput_SetFont(*args)
    def GetTopLine(*args): return _blcelc.iConsoleOutput_GetTopLine(*args)
    def ScrollTo(*args): return _blcelc.iConsoleOutput_ScrollTo(*args)
    def GetCursorStyle(*args): return _blcelc.iConsoleOutput_GetCursorStyle(*args)
    def SetCursorStyle(*args): return _blcelc.iConsoleOutput_SetCursorStyle(*args)
    def SetVisible(*args): return _blcelc.iConsoleOutput_SetVisible(*args)
    def GetVisible(*args): return _blcelc.iConsoleOutput_GetVisible(*args)
    def AutoUpdate(*args): return _blcelc.iConsoleOutput_AutoUpdate(*args)
    def SetCursorPos(*args): return _blcelc.iConsoleOutput_SetCursorPos(*args)
    def GetMaxLineWidth(*args): return _blcelc.iConsoleOutput_GetMaxLineWidth(*args)
    def RegisterWatcher(*args): return _blcelc.iConsoleOutput_RegisterWatcher(*args)
    def PerformExtension(*args): return _blcelc.iConsoleOutput_PerformExtension(*args)
    def __del__(self, destroy=_blcelc.delete_iConsoleOutput):
        try:
            if self.thisown: destroy(self)
        except: pass

class iConsoleOutputPtr(iConsoleOutput):
    def __init__(self, this):
        _swig_setattr(self, iConsoleOutput, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iConsoleOutput, 'thisown', 0)
        _swig_setattr(self, iConsoleOutput,self.__class__,iConsoleOutput)
_blcelc.iConsoleOutput_swigregister(iConsoleOutputPtr)

class iStandardReporterListener(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iStandardReporterListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iStandardReporterListener, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStandardReporterListener instance at %s>" % (self.this,)
    def SetOutputConsole(*args): return _blcelc.iStandardReporterListener_SetOutputConsole(*args)
    def SetNativeWindowManager(*args): return _blcelc.iStandardReporterListener_SetNativeWindowManager(*args)
    def SetReporter(*args): return _blcelc.iStandardReporterListener_SetReporter(*args)
    def SetDebugFile(*args): return _blcelc.iStandardReporterListener_SetDebugFile(*args)
    def SetDefaults(*args): return _blcelc.iStandardReporterListener_SetDefaults(*args)
    def SetMessageDestination(*args): return _blcelc.iStandardReporterListener_SetMessageDestination(*args)
    def RemoveMessages(*args): return _blcelc.iStandardReporterListener_RemoveMessages(*args)
    def ShowMessageID(*args): return _blcelc.iStandardReporterListener_ShowMessageID(*args)
    def GetDebugFile(*args): return _blcelc.iStandardReporterListener_GetDebugFile(*args)
    def __del__(self, destroy=_blcelc.delete_iStandardReporterListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStandardReporterListener_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStandardReporterListener_scfGetVersion)

class iStandardReporterListenerPtr(iStandardReporterListener):
    def __init__(self, this):
        _swig_setattr(self, iStandardReporterListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iStandardReporterListener, 'thisown', 0)
        _swig_setattr(self, iStandardReporterListener,self.__class__,iStandardReporterListener)
_blcelc.iStandardReporterListener_swigregister(iStandardReporterListenerPtr)

iStandardReporterListener_scfGetVersion = _blcelc.iStandardReporterListener_scfGetVersion

class iView(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iView, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iView, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iView instance at %s>" % (self.this,)
    def GetEngine(*args): return _blcelc.iView_GetEngine(*args)
    def SetEngine(*args): return _blcelc.iView_SetEngine(*args)
    def GetCamera(*args): return _blcelc.iView_GetCamera(*args)
    def SetCamera(*args): return _blcelc.iView_SetCamera(*args)
    def GetContext(*args): return _blcelc.iView_GetContext(*args)
    def SetContext(*args): return _blcelc.iView_SetContext(*args)
    def SetRectangle(*args): return _blcelc.iView_SetRectangle(*args)
    def ClearView(*args): return _blcelc.iView_ClearView(*args)
    def AddViewVertex(*args): return _blcelc.iView_AddViewVertex(*args)
    def RestrictClipperToScreen(*args): return _blcelc.iView_RestrictClipperToScreen(*args)
    def UpdateClipper(*args): return _blcelc.iView_UpdateClipper(*args)
    def GetClipper(*args): return _blcelc.iView_GetClipper(*args)
    def Draw(*args): return _blcelc.iView_Draw(*args)
    def SetAutoResize(*args): return _blcelc.iView_SetAutoResize(*args)
    def __del__(self, destroy=_blcelc.delete_iView):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iView_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iView_scfGetVersion)

class iViewPtr(iView):
    def __init__(self, this):
        _swig_setattr(self, iView, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iView, 'thisown', 0)
        _swig_setattr(self, iView,self.__class__,iView)
_blcelc.iView_swigregister(iViewPtr)

iView_scfGetVersion = _blcelc.iView_scfGetVersion

class csCollisionPair(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csCollisionPair, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csCollisionPair, name)
    def __repr__(self):
        return "<C csCollisionPair instance at %s>" % (self.this,)
    __swig_setmethods__["a1"] = _blcelc.csCollisionPair_a1_set
    __swig_getmethods__["a1"] = _blcelc.csCollisionPair_a1_get
    if _newclass:a1 = property(_blcelc.csCollisionPair_a1_get, _blcelc.csCollisionPair_a1_set)
    __swig_setmethods__["b1"] = _blcelc.csCollisionPair_b1_set
    __swig_getmethods__["b1"] = _blcelc.csCollisionPair_b1_get
    if _newclass:b1 = property(_blcelc.csCollisionPair_b1_get, _blcelc.csCollisionPair_b1_set)
    __swig_setmethods__["c1"] = _blcelc.csCollisionPair_c1_set
    __swig_getmethods__["c1"] = _blcelc.csCollisionPair_c1_get
    if _newclass:c1 = property(_blcelc.csCollisionPair_c1_get, _blcelc.csCollisionPair_c1_set)
    __swig_setmethods__["a2"] = _blcelc.csCollisionPair_a2_set
    __swig_getmethods__["a2"] = _blcelc.csCollisionPair_a2_get
    if _newclass:a2 = property(_blcelc.csCollisionPair_a2_get, _blcelc.csCollisionPair_a2_set)
    __swig_setmethods__["b2"] = _blcelc.csCollisionPair_b2_set
    __swig_getmethods__["b2"] = _blcelc.csCollisionPair_b2_get
    if _newclass:b2 = property(_blcelc.csCollisionPair_b2_get, _blcelc.csCollisionPair_b2_set)
    __swig_setmethods__["c2"] = _blcelc.csCollisionPair_c2_set
    __swig_getmethods__["c2"] = _blcelc.csCollisionPair_c2_get
    if _newclass:c2 = property(_blcelc.csCollisionPair_c2_get, _blcelc.csCollisionPair_c2_set)
    def __init__(self, *args):
        _swig_setattr(self, csCollisionPair, 'this', _blcelc.new_csCollisionPair(*args))
        _swig_setattr(self, csCollisionPair, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csCollisionPair):
        try:
            if self.thisown: destroy(self)
        except: pass

class csCollisionPairPtr(csCollisionPair):
    def __init__(self, this):
        _swig_setattr(self, csCollisionPair, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csCollisionPair, 'thisown', 0)
        _swig_setattr(self, csCollisionPair,self.__class__,csCollisionPair)
_blcelc.csCollisionPair_swigregister(csCollisionPairPtr)

class csIntersectingTriangle(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csIntersectingTriangle, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csIntersectingTriangle, name)
    def __repr__(self):
        return "<C csIntersectingTriangle instance at %s>" % (self.this,)
    __swig_setmethods__["a"] = _blcelc.csIntersectingTriangle_a_set
    __swig_getmethods__["a"] = _blcelc.csIntersectingTriangle_a_get
    if _newclass:a = property(_blcelc.csIntersectingTriangle_a_get, _blcelc.csIntersectingTriangle_a_set)
    __swig_setmethods__["b"] = _blcelc.csIntersectingTriangle_b_set
    __swig_getmethods__["b"] = _blcelc.csIntersectingTriangle_b_get
    if _newclass:b = property(_blcelc.csIntersectingTriangle_b_get, _blcelc.csIntersectingTriangle_b_set)
    __swig_setmethods__["c"] = _blcelc.csIntersectingTriangle_c_set
    __swig_getmethods__["c"] = _blcelc.csIntersectingTriangle_c_get
    if _newclass:c = property(_blcelc.csIntersectingTriangle_c_get, _blcelc.csIntersectingTriangle_c_set)
    def __init__(self, *args):
        _swig_setattr(self, csIntersectingTriangle, 'this', _blcelc.new_csIntersectingTriangle(*args))
        _swig_setattr(self, csIntersectingTriangle, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csIntersectingTriangle):
        try:
            if self.thisown: destroy(self)
        except: pass

class csIntersectingTrianglePtr(csIntersectingTriangle):
    def __init__(self, this):
        _swig_setattr(self, csIntersectingTriangle, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csIntersectingTriangle, 'thisown', 0)
        _swig_setattr(self, csIntersectingTriangle,self.__class__,csIntersectingTriangle)
_blcelc.csIntersectingTriangle_swigregister(csIntersectingTrianglePtr)

class iCollider(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCollider, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCollider, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCollider instance at %s>" % (self.this,)
    def __del__(self, destroy=_blcelc.delete_iCollider):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCollider_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCollider_scfGetVersion)

class iColliderPtr(iCollider):
    def __init__(self, this):
        _swig_setattr(self, iCollider, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCollider, 'thisown', 0)
        _swig_setattr(self, iCollider,self.__class__,iCollider)
_blcelc.iCollider_swigregister(iColliderPtr)

iCollider_scfGetVersion = _blcelc.iCollider_scfGetVersion

class iCollideSystem(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCollideSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCollideSystem, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCollideSystem instance at %s>" % (self.this,)
    def CreateCollider(*args): return _blcelc.iCollideSystem_CreateCollider(*args)
    def Collide(*args): return _blcelc.iCollideSystem_Collide(*args)
    def GetCollisionPairs(*args): return _blcelc.iCollideSystem_GetCollisionPairs(*args)
    def GetCollisionPairCount(*args): return _blcelc.iCollideSystem_GetCollisionPairCount(*args)
    def ResetCollisionPairs(*args): return _blcelc.iCollideSystem_ResetCollisionPairs(*args)
    def CollideRay(*args): return _blcelc.iCollideSystem_CollideRay(*args)
    def GetIntersectingTriangles(*args): return _blcelc.iCollideSystem_GetIntersectingTriangles(*args)
    def SetOneHitOnly(*args): return _blcelc.iCollideSystem_SetOneHitOnly(*args)
    def GetOneHitOnly(*args): return _blcelc.iCollideSystem_GetOneHitOnly(*args)
    def __del__(self, destroy=_blcelc.delete_iCollideSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCollideSystem_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCollideSystem_scfGetVersion)
    def GetCollisionPairByIndex(*args): return _blcelc.iCollideSystem_GetCollisionPairByIndex(*args)
    def GetCollisionPairs (self):
      num = self.GetCollisionPairCount()
      pairs = []
      for i in range(num):
        pairs.append(self.GetCollisionPairByIndex(i))
      return pairs


class iCollideSystemPtr(iCollideSystem):
    def __init__(self, this):
        _swig_setattr(self, iCollideSystem, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iCollideSystem, 'thisown', 0)
        _swig_setattr(self, iCollideSystem,self.__class__,iCollideSystem)
_blcelc.iCollideSystem_swigregister(iCollideSystemPtr)

iCollideSystem_scfGetVersion = _blcelc.iCollideSystem_scfGetVersion

class iDynamics(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDynamics, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDynamics, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDynamics instance at %s>" % (self.this,)
    def CreateSystem(*args): return _blcelc.iDynamics_CreateSystem(*args)
    def RemoveSystem(*args): return _blcelc.iDynamics_RemoveSystem(*args)
    def FindSystem(*args): return _blcelc.iDynamics_FindSystem(*args)
    def Step(*args): return _blcelc.iDynamics_Step(*args)
    def __del__(self, destroy=_blcelc.delete_iDynamics):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDynamics_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDynamics_scfGetVersion)

class iDynamicsPtr(iDynamics):
    def __init__(self, this):
        _swig_setattr(self, iDynamics, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDynamics, 'thisown', 0)
        _swig_setattr(self, iDynamics,self.__class__,iDynamics)
_blcelc.iDynamics_swigregister(iDynamicsPtr)

iDynamics_scfGetVersion = _blcelc.iDynamics_scfGetVersion

class iDynamicSystem(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDynamicSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDynamicSystem, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDynamicSystem instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iDynamicSystem_QueryObject(*args)
    def SetGravity(*args): return _blcelc.iDynamicSystem_SetGravity(*args)
    def GetGravity(*args): return _blcelc.iDynamicSystem_GetGravity(*args)
    def SetLinearDampener(*args): return _blcelc.iDynamicSystem_SetLinearDampener(*args)
    def GetLinearDampener(*args): return _blcelc.iDynamicSystem_GetLinearDampener(*args)
    def SetRollingDampener(*args): return _blcelc.iDynamicSystem_SetRollingDampener(*args)
    def GetRollingDampener(*args): return _blcelc.iDynamicSystem_GetRollingDampener(*args)
    def EnableAutoDisable(*args): return _blcelc.iDynamicSystem_EnableAutoDisable(*args)
    def AutoDisableEnabled(*args): return _blcelc.iDynamicSystem_AutoDisableEnabled(*args)
    def SetAutoDisableParams(*args): return _blcelc.iDynamicSystem_SetAutoDisableParams(*args)
    def Step(*args): return _blcelc.iDynamicSystem_Step(*args)
    def CreateBody(*args): return _blcelc.iDynamicSystem_CreateBody(*args)
    def RemoveBody(*args): return _blcelc.iDynamicSystem_RemoveBody(*args)
    def FindBody(*args): return _blcelc.iDynamicSystem_FindBody(*args)
    def CreateGroup(*args): return _blcelc.iDynamicSystem_CreateGroup(*args)
    def RemoveGroup(*args): return _blcelc.iDynamicSystem_RemoveGroup(*args)
    def CreateJoint(*args): return _blcelc.iDynamicSystem_CreateJoint(*args)
    def RemoveJoint(*args): return _blcelc.iDynamicSystem_RemoveJoint(*args)
    def GetDefaultMoveCallback(*args): return _blcelc.iDynamicSystem_GetDefaultMoveCallback(*args)
    def AttachColliderMesh(*args): return _blcelc.iDynamicSystem_AttachColliderMesh(*args)
    def AttachColliderCylinder(*args): return _blcelc.iDynamicSystem_AttachColliderCylinder(*args)
    def AttachColliderBox(*args): return _blcelc.iDynamicSystem_AttachColliderBox(*args)
    def AttachColliderSphere(*args): return _blcelc.iDynamicSystem_AttachColliderSphere(*args)
    def AttachColliderPlane(*args): return _blcelc.iDynamicSystem_AttachColliderPlane(*args)
    def __del__(self, destroy=_blcelc.delete_iDynamicSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDynamicSystem_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDynamicSystem_scfGetVersion)

class iDynamicSystemPtr(iDynamicSystem):
    def __init__(self, this):
        _swig_setattr(self, iDynamicSystem, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDynamicSystem, 'thisown', 0)
        _swig_setattr(self, iDynamicSystem,self.__class__,iDynamicSystem)
_blcelc.iDynamicSystem_swigregister(iDynamicSystemPtr)

iDynamicSystem_scfGetVersion = _blcelc.iDynamicSystem_scfGetVersion

class iDynamicsMoveCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDynamicsMoveCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDynamicsMoveCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDynamicsMoveCallback instance at %s>" % (self.this,)
    def Execute(*args): return _blcelc.iDynamicsMoveCallback_Execute(*args)
    def __del__(self, destroy=_blcelc.delete_iDynamicsMoveCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iDynamicsMoveCallbackPtr(iDynamicsMoveCallback):
    def __init__(self, this):
        _swig_setattr(self, iDynamicsMoveCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDynamicsMoveCallback, 'thisown', 0)
        _swig_setattr(self, iDynamicsMoveCallback,self.__class__,iDynamicsMoveCallback)
_blcelc.iDynamicsMoveCallback_swigregister(iDynamicsMoveCallbackPtr)

class iDynamicsCollisionCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDynamicsCollisionCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDynamicsCollisionCallback, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDynamicsCollisionCallback instance at %s>" % (self.this,)
    def Execute(*args): return _blcelc.iDynamicsCollisionCallback_Execute(*args)
    def __del__(self, destroy=_blcelc.delete_iDynamicsCollisionCallback):
        try:
            if self.thisown: destroy(self)
        except: pass

class iDynamicsCollisionCallbackPtr(iDynamicsCollisionCallback):
    def __init__(self, this):
        _swig_setattr(self, iDynamicsCollisionCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDynamicsCollisionCallback, 'thisown', 0)
        _swig_setattr(self, iDynamicsCollisionCallback,self.__class__,iDynamicsCollisionCallback)
_blcelc.iDynamicsCollisionCallback_swigregister(iDynamicsCollisionCallbackPtr)

class iBodyGroup(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBodyGroup, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBodyGroup, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBodyGroup instance at %s>" % (self.this,)
    def AddBody(*args): return _blcelc.iBodyGroup_AddBody(*args)
    def RemoveBody(*args): return _blcelc.iBodyGroup_RemoveBody(*args)
    def BodyInGroup(*args): return _blcelc.iBodyGroup_BodyInGroup(*args)
    def __del__(self, destroy=_blcelc.delete_iBodyGroup):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iBodyGroup_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iBodyGroup_scfGetVersion)

class iBodyGroupPtr(iBodyGroup):
    def __init__(self, this):
        _swig_setattr(self, iBodyGroup, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBodyGroup, 'thisown', 0)
        _swig_setattr(self, iBodyGroup,self.__class__,iBodyGroup)
_blcelc.iBodyGroup_swigregister(iBodyGroupPtr)

iBodyGroup_scfGetVersion = _blcelc.iBodyGroup_scfGetVersion

class iRigidBody(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iRigidBody, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iRigidBody, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iRigidBody instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iRigidBody_QueryObject(*args)
    def MakeStatic(*args): return _blcelc.iRigidBody_MakeStatic(*args)
    def MakeDynamic(*args): return _blcelc.iRigidBody_MakeDynamic(*args)
    def IsStatic(*args): return _blcelc.iRigidBody_IsStatic(*args)
    def Disable(*args): return _blcelc.iRigidBody_Disable(*args)
    def Enable(*args): return _blcelc.iRigidBody_Enable(*args)
    def IsEnabled(*args): return _blcelc.iRigidBody_IsEnabled(*args)
    def GetGroup(*args): return _blcelc.iRigidBody_GetGroup(*args)
    def AttachColliderMesh(*args): return _blcelc.iRigidBody_AttachColliderMesh(*args)
    def AttachColliderCylinder(*args): return _blcelc.iRigidBody_AttachColliderCylinder(*args)
    def AttachColliderBox(*args): return _blcelc.iRigidBody_AttachColliderBox(*args)
    def AttachColliderSphere(*args): return _blcelc.iRigidBody_AttachColliderSphere(*args)
    def AttachColliderPlane(*args): return _blcelc.iRigidBody_AttachColliderPlane(*args)
    def SetPosition(*args): return _blcelc.iRigidBody_SetPosition(*args)
    def GetPosition(*args): return _blcelc.iRigidBody_GetPosition(*args)
    def SetOrientation(*args): return _blcelc.iRigidBody_SetOrientation(*args)
    def GetOrientation(*args): return _blcelc.iRigidBody_GetOrientation(*args)
    def SetTransform(*args): return _blcelc.iRigidBody_SetTransform(*args)
    def GetTransform(*args): return _blcelc.iRigidBody_GetTransform(*args)
    def SetLinearVelocity(*args): return _blcelc.iRigidBody_SetLinearVelocity(*args)
    def GetLinearVelocity(*args): return _blcelc.iRigidBody_GetLinearVelocity(*args)
    def SetAngularVelocity(*args): return _blcelc.iRigidBody_SetAngularVelocity(*args)
    def GetAngularVelocity(*args): return _blcelc.iRigidBody_GetAngularVelocity(*args)
    def SetProperties(*args): return _blcelc.iRigidBody_SetProperties(*args)
    def GetProperties(*args): return _blcelc.iRigidBody_GetProperties(*args)
    def AdjustTotalMass(*args): return _blcelc.iRigidBody_AdjustTotalMass(*args)
    def AddForce(*args): return _blcelc.iRigidBody_AddForce(*args)
    def AddTorque(*args): return _blcelc.iRigidBody_AddTorque(*args)
    def AddRelForce(*args): return _blcelc.iRigidBody_AddRelForce(*args)
    def AddRelTorque(*args): return _blcelc.iRigidBody_AddRelTorque(*args)
    def AddForceAtPos(*args): return _blcelc.iRigidBody_AddForceAtPos(*args)
    def AddForceAtRelPos(*args): return _blcelc.iRigidBody_AddForceAtRelPos(*args)
    def AddRelForceAtPos(*args): return _blcelc.iRigidBody_AddRelForceAtPos(*args)
    def AddRelForceAtRelPos(*args): return _blcelc.iRigidBody_AddRelForceAtRelPos(*args)
    def GetForce(*args): return _blcelc.iRigidBody_GetForce(*args)
    def GetTorque(*args): return _blcelc.iRigidBody_GetTorque(*args)
    def AttachMesh(*args): return _blcelc.iRigidBody_AttachMesh(*args)
    def GetAttachedMesh(*args): return _blcelc.iRigidBody_GetAttachedMesh(*args)
    def SetMoveCallback(*args): return _blcelc.iRigidBody_SetMoveCallback(*args)
    def SetCollisionCallback(*args): return _blcelc.iRigidBody_SetCollisionCallback(*args)
    def Collision(*args): return _blcelc.iRigidBody_Collision(*args)
    def Update(*args): return _blcelc.iRigidBody_Update(*args)
    def __del__(self, destroy=_blcelc.delete_iRigidBody):
        try:
            if self.thisown: destroy(self)
        except: pass

class iRigidBodyPtr(iRigidBody):
    def __init__(self, this):
        _swig_setattr(self, iRigidBody, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iRigidBody, 'thisown', 0)
        _swig_setattr(self, iRigidBody,self.__class__,iRigidBody)
_blcelc.iRigidBody_swigregister(iRigidBodyPtr)

class iJoint(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iJoint, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iJoint, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iJoint instance at %s>" % (self.this,)
    def Attach(*args): return _blcelc.iJoint_Attach(*args)
    def GetAttachedBody(*args): return _blcelc.iJoint_GetAttachedBody(*args)
    def SetTransform(*args): return _blcelc.iJoint_SetTransform(*args)
    def GetTransform(*args): return _blcelc.iJoint_GetTransform(*args)
    def SetTransConstraints(*args): return _blcelc.iJoint_SetTransConstraints(*args)
    def IsXTransConstrained(*args): return _blcelc.iJoint_IsXTransConstrained(*args)
    def IsYTransConstrained(*args): return _blcelc.iJoint_IsYTransConstrained(*args)
    def IsZTransConstrained(*args): return _blcelc.iJoint_IsZTransConstrained(*args)
    def SetMinimumDistance(*args): return _blcelc.iJoint_SetMinimumDistance(*args)
    def GetMinimumDistance(*args): return _blcelc.iJoint_GetMinimumDistance(*args)
    def SetMaximumDistance(*args): return _blcelc.iJoint_SetMaximumDistance(*args)
    def GetMaximumDistance(*args): return _blcelc.iJoint_GetMaximumDistance(*args)
    def SetRotConstraints(*args): return _blcelc.iJoint_SetRotConstraints(*args)
    def IsXRotConstrained(*args): return _blcelc.iJoint_IsXRotConstrained(*args)
    def IsYRotConstrained(*args): return _blcelc.iJoint_IsYRotConstrained(*args)
    def IsZRotConstrained(*args): return _blcelc.iJoint_IsZRotConstrained(*args)
    def SetMinimumAngle(*args): return _blcelc.iJoint_SetMinimumAngle(*args)
    def GetMinimumAngle(*args): return _blcelc.iJoint_GetMinimumAngle(*args)
    def SetMaximumAngle(*args): return _blcelc.iJoint_SetMaximumAngle(*args)
    def GetMaximumAngle(*args): return _blcelc.iJoint_GetMaximumAngle(*args)
    def SetBounce(*args): return _blcelc.iJoint_SetBounce(*args)
    def GetBounce(*args): return _blcelc.iJoint_GetBounce(*args)
    def SetDesiredVelocity(*args): return _blcelc.iJoint_SetDesiredVelocity(*args)
    def GetDesiredVelocity(*args): return _blcelc.iJoint_GetDesiredVelocity(*args)
    def SetMaxForce(*args): return _blcelc.iJoint_SetMaxForce(*args)
    def GetMaxForce(*args): return _blcelc.iJoint_GetMaxForce(*args)
    def __del__(self, destroy=_blcelc.delete_iJoint):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iJoint_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iJoint_scfGetVersion)

class iJointPtr(iJoint):
    def __init__(self, this):
        _swig_setattr(self, iJoint, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iJoint, 'thisown', 0)
        _swig_setattr(self, iJoint,self.__class__,iJoint)
_blcelc.iJoint_swigregister(iJointPtr)

iJoint_scfGetVersion = _blcelc.iJoint_scfGetVersion

CS_SEQUENCE_LIGHTCHANGE_NONE = _blcelc.CS_SEQUENCE_LIGHTCHANGE_NONE
CS_SEQUENCE_LIGHTCHANGE_LESS = _blcelc.CS_SEQUENCE_LIGHTCHANGE_LESS
CS_SEQUENCE_LIGHTCHANGE_GREATER = _blcelc.CS_SEQUENCE_LIGHTCHANGE_GREATER
class iParameterESM(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iParameterESM, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iParameterESM, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iParameterESM instance at %s>" % (self.this,)
    def GetValue(*args): return _blcelc.iParameterESM_GetValue(*args)
    def IsConstant(*args): return _blcelc.iParameterESM_IsConstant(*args)
    def __del__(self, destroy=_blcelc.delete_iParameterESM):
        try:
            if self.thisown: destroy(self)
        except: pass

class iParameterESMPtr(iParameterESM):
    def __init__(self, this):
        _swig_setattr(self, iParameterESM, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iParameterESM, 'thisown', 0)
        _swig_setattr(self, iParameterESM,self.__class__,iParameterESM)
_blcelc.iParameterESM_swigregister(iParameterESMPtr)

class iEngineSequenceParameters(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEngineSequenceParameters, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEngineSequenceParameters, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEngineSequenceParameters instance at %s>" % (self.this,)
    def GetParameterCount(*args): return _blcelc.iEngineSequenceParameters_GetParameterCount(*args)
    def GetParameter(*args): return _blcelc.iEngineSequenceParameters_GetParameter(*args)
    def GetParameterIdx(*args): return _blcelc.iEngineSequenceParameters_GetParameterIdx(*args)
    def GetParameterName(*args): return _blcelc.iEngineSequenceParameters_GetParameterName(*args)
    def AddParameter(*args): return _blcelc.iEngineSequenceParameters_AddParameter(*args)
    def SetParameter(*args): return _blcelc.iEngineSequenceParameters_SetParameter(*args)
    def CreateParameterESM(*args): return _blcelc.iEngineSequenceParameters_CreateParameterESM(*args)
    def __del__(self, destroy=_blcelc.delete_iEngineSequenceParameters):
        try:
            if self.thisown: destroy(self)
        except: pass

class iEngineSequenceParametersPtr(iEngineSequenceParameters):
    def __init__(self, this):
        _swig_setattr(self, iEngineSequenceParameters, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEngineSequenceParameters, 'thisown', 0)
        _swig_setattr(self, iEngineSequenceParameters,self.__class__,iEngineSequenceParameters)
_blcelc.iEngineSequenceParameters_swigregister(iEngineSequenceParametersPtr)

class iSequenceWrapper(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSequenceWrapper, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSequenceWrapper, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSequenceWrapper instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iSequenceWrapper_QueryObject(*args)
    def GetSequence(*args): return _blcelc.iSequenceWrapper_GetSequence(*args)
    def CreateBaseParameterBlock(*args): return _blcelc.iSequenceWrapper_CreateBaseParameterBlock(*args)
    def GetBaseParameterBlock(*args): return _blcelc.iSequenceWrapper_GetBaseParameterBlock(*args)
    def CreateParameterBlock(*args): return _blcelc.iSequenceWrapper_CreateParameterBlock(*args)
    def AddOperationSetVariable(*args): return _blcelc.iSequenceWrapper_AddOperationSetVariable(*args)
    def AddOperationSetMaterial(*args): return _blcelc.iSequenceWrapper_AddOperationSetMaterial(*args)
    def AddOperationSetPolygonMaterial(*args): return _blcelc.iSequenceWrapper_AddOperationSetPolygonMaterial(*args)
    def AddOperationSetLight(*args): return _blcelc.iSequenceWrapper_AddOperationSetLight(*args)
    def AddOperationFadeLight(*args): return _blcelc.iSequenceWrapper_AddOperationFadeLight(*args)
    def AddOperationSetAmbient(*args): return _blcelc.iSequenceWrapper_AddOperationSetAmbient(*args)
    def AddOperationFadeAmbient(*args): return _blcelc.iSequenceWrapper_AddOperationFadeAmbient(*args)
    def AddOperationRandomDelay(*args): return _blcelc.iSequenceWrapper_AddOperationRandomDelay(*args)
    def AddOperationSetMeshColor(*args): return _blcelc.iSequenceWrapper_AddOperationSetMeshColor(*args)
    def AddOperationFadeMeshColor(*args): return _blcelc.iSequenceWrapper_AddOperationFadeMeshColor(*args)
    def AddOperationSetFog(*args): return _blcelc.iSequenceWrapper_AddOperationSetFog(*args)
    def AddOperationFadeFog(*args): return _blcelc.iSequenceWrapper_AddOperationFadeFog(*args)
    def AddOperationRotateDuration(*args): return _blcelc.iSequenceWrapper_AddOperationRotateDuration(*args)
    def AddOperationMoveDuration(*args): return _blcelc.iSequenceWrapper_AddOperationMoveDuration(*args)
    def AddOperationTriggerState(*args): return _blcelc.iSequenceWrapper_AddOperationTriggerState(*args)
    def AddOperationCheckTrigger(*args): return _blcelc.iSequenceWrapper_AddOperationCheckTrigger(*args)
    def AddOperationTestTrigger(*args): return _blcelc.iSequenceWrapper_AddOperationTestTrigger(*args)
    def __del__(self, destroy=_blcelc.delete_iSequenceWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSequenceWrapperPtr(iSequenceWrapper):
    def __init__(self, this):
        _swig_setattr(self, iSequenceWrapper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSequenceWrapper, 'thisown', 0)
        _swig_setattr(self, iSequenceWrapper,self.__class__,iSequenceWrapper)
_blcelc.iSequenceWrapper_swigregister(iSequenceWrapperPtr)

class iSequenceTrigger(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSequenceTrigger, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSequenceTrigger, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSequenceTrigger instance at %s>" % (self.this,)
    def QueryObject(*args): return _blcelc.iSequenceTrigger_QueryObject(*args)
    def AddConditionInSector(*args): return _blcelc.iSequenceTrigger_AddConditionInSector(*args)
    def AddConditionSectorVisible(*args): return _blcelc.iSequenceTrigger_AddConditionSectorVisible(*args)
    def AddConditionMeshClick(*args): return _blcelc.iSequenceTrigger_AddConditionMeshClick(*args)
    def AddConditionLightChange(*args): return _blcelc.iSequenceTrigger_AddConditionLightChange(*args)
    def AddConditionManual(*args): return _blcelc.iSequenceTrigger_AddConditionManual(*args)
    def SetEnabled(*args): return _blcelc.iSequenceTrigger_SetEnabled(*args)
    def IsEnabled(*args): return _blcelc.iSequenceTrigger_IsEnabled(*args)
    def ClearConditions(*args): return _blcelc.iSequenceTrigger_ClearConditions(*args)
    def Trigger(*args): return _blcelc.iSequenceTrigger_Trigger(*args)
    def SetParameters(*args): return _blcelc.iSequenceTrigger_SetParameters(*args)
    def GetParameters(*args): return _blcelc.iSequenceTrigger_GetParameters(*args)
    def FireSequence(*args): return _blcelc.iSequenceTrigger_FireSequence(*args)
    def GetFiredSequence(*args): return _blcelc.iSequenceTrigger_GetFiredSequence(*args)
    def TestConditions(*args): return _blcelc.iSequenceTrigger_TestConditions(*args)
    def CheckState(*args): return _blcelc.iSequenceTrigger_CheckState(*args)
    def ForceFire(*args): return _blcelc.iSequenceTrigger_ForceFire(*args)
    def __del__(self, destroy=_blcelc.delete_iSequenceTrigger):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSequenceTriggerPtr(iSequenceTrigger):
    def __init__(self, this):
        _swig_setattr(self, iSequenceTrigger, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSequenceTrigger, 'thisown', 0)
        _swig_setattr(self, iSequenceTrigger,self.__class__,iSequenceTrigger)
_blcelc.iSequenceTrigger_swigregister(iSequenceTriggerPtr)

class iSequenceTimedOperation(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSequenceTimedOperation, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSequenceTimedOperation, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSequenceTimedOperation instance at %s>" % (self.this,)
    def Do(*args): return _blcelc.iSequenceTimedOperation_Do(*args)
    def __del__(self, destroy=_blcelc.delete_iSequenceTimedOperation):
        try:
            if self.thisown: destroy(self)
        except: pass

class iSequenceTimedOperationPtr(iSequenceTimedOperation):
    def __init__(self, this):
        _swig_setattr(self, iSequenceTimedOperation, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSequenceTimedOperation, 'thisown', 0)
        _swig_setattr(self, iSequenceTimedOperation,self.__class__,iSequenceTimedOperation)
_blcelc.iSequenceTimedOperation_swigregister(iSequenceTimedOperationPtr)

class iEngineSequenceManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEngineSequenceManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEngineSequenceManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEngineSequenceManager instance at %s>" % (self.this,)
    def GetSequenceManager(*args): return _blcelc.iEngineSequenceManager_GetSequenceManager(*args)
    def SetCamera(*args): return _blcelc.iEngineSequenceManager_SetCamera(*args)
    def GetCamera(*args): return _blcelc.iEngineSequenceManager_GetCamera(*args)
    def CreateParameterESM(*args): return _blcelc.iEngineSequenceManager_CreateParameterESM(*args)
    def CreateTrigger(*args): return _blcelc.iEngineSequenceManager_CreateTrigger(*args)
    def RemoveTrigger(*args): return _blcelc.iEngineSequenceManager_RemoveTrigger(*args)
    def RemoveTriggers(*args): return _blcelc.iEngineSequenceManager_RemoveTriggers(*args)
    def GetTriggerCount(*args): return _blcelc.iEngineSequenceManager_GetTriggerCount(*args)
    def GetTrigger(*args): return _blcelc.iEngineSequenceManager_GetTrigger(*args)
    def FindTriggerByName(*args): return _blcelc.iEngineSequenceManager_FindTriggerByName(*args)
    def FireTriggerByName(*args): return _blcelc.iEngineSequenceManager_FireTriggerByName(*args)
    def CreateSequence(*args): return _blcelc.iEngineSequenceManager_CreateSequence(*args)
    def RemoveSequence(*args): return _blcelc.iEngineSequenceManager_RemoveSequence(*args)
    def RemoveSequences(*args): return _blcelc.iEngineSequenceManager_RemoveSequences(*args)
    def GetSequenceCount(*args): return _blcelc.iEngineSequenceManager_GetSequenceCount(*args)
    def GetSequence(*args): return _blcelc.iEngineSequenceManager_GetSequence(*args)
    def FindSequenceByName(*args): return _blcelc.iEngineSequenceManager_FindSequenceByName(*args)
    def RunSequenceByName(*args): return _blcelc.iEngineSequenceManager_RunSequenceByName(*args)
    def FireTimedOperation(*args): return _blcelc.iEngineSequenceManager_FireTimedOperation(*args)
    def __del__(self, destroy=_blcelc.delete_iEngineSequenceManager):
        try:
            if self.thisown: destroy(self)
        except: pass

class iEngineSequenceManagerPtr(iEngineSequenceManager):
    def __init__(self, this):
        _swig_setattr(self, iEngineSequenceManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEngineSequenceManager, 'thisown', 0)
        _swig_setattr(self, iEngineSequenceManager,self.__class__,iEngineSequenceManager)
_blcelc.iEngineSequenceManager_swigregister(iEngineSequenceManagerPtr)

class iScriptObject(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iScriptObject, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iScriptObject, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iScriptObject instance at %s>" % (self.this,)
    def IsType(*args): return _blcelc.iScriptObject_IsType(*args)
    def GetPointer(*args): return _blcelc.iScriptObject_GetPointer(*args)
    def SetPointer(*args): return _blcelc.iScriptObject_SetPointer(*args)
    def IntCall(*args): return _blcelc.iScriptObject_IntCall(*args)
    def FloatCall(*args): return _blcelc.iScriptObject_FloatCall(*args)
    def DoubleCall(*args): return _blcelc.iScriptObject_DoubleCall(*args)
    def Call(*args): return _blcelc.iScriptObject_Call(*args)
    def ObjectCall(*args): return _blcelc.iScriptObject_ObjectCall(*args)
    def SetInt(*args): return _blcelc.iScriptObject_SetInt(*args)
    def SetFloat(*args): return _blcelc.iScriptObject_SetFloat(*args)
    def SetDouble(*args): return _blcelc.iScriptObject_SetDouble(*args)
    def SetString(*args): return _blcelc.iScriptObject_SetString(*args)
    def Set(*args): return _blcelc.iScriptObject_Set(*args)
    def SetTruth(*args): return _blcelc.iScriptObject_SetTruth(*args)
    def GetFloat(*args): return _blcelc.iScriptObject_GetFloat(*args)
    def Get(*args): return _blcelc.iScriptObject_Get(*args)
    def GetTruth(*args): return _blcelc.iScriptObject_GetTruth(*args)
    def __del__(self, destroy=_blcelc.delete_iScriptObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iScriptObject_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iScriptObject_scfGetVersion)

class iScriptObjectPtr(iScriptObject):
    def __init__(self, this):
        _swig_setattr(self, iScriptObject, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iScriptObject, 'thisown', 0)
        _swig_setattr(self, iScriptObject,self.__class__,iScriptObject)
_blcelc.iScriptObject_swigregister(iScriptObjectPtr)

iScriptObject_scfGetVersion = _blcelc.iScriptObject_scfGetVersion

class iScript(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iScript, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iScript, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iScript instance at %s>" % (self.this,)
    def Initialize(*args): return _blcelc.iScript_Initialize(*args)
    def RunText(*args): return _blcelc.iScript_RunText(*args)
    def LoadModule(*args): return _blcelc.iScript_LoadModule(*args)
    def IntCall(*args): return _blcelc.iScript_IntCall(*args)
    def FloatCall(*args): return _blcelc.iScript_FloatCall(*args)
    def DoubleCall(*args): return _blcelc.iScript_DoubleCall(*args)
    def Call(*args): return _blcelc.iScript_Call(*args)
    def ObjectCall(*args): return _blcelc.iScript_ObjectCall(*args)
    def NewObject(*args): return _blcelc.iScript_NewObject(*args)
    def StoreInt(*args): return _blcelc.iScript_StoreInt(*args)
    def StoreFloat(*args): return _blcelc.iScript_StoreFloat(*args)
    def StoreDouble(*args): return _blcelc.iScript_StoreDouble(*args)
    def StoreString(*args): return _blcelc.iScript_StoreString(*args)
    def Store(*args): return _blcelc.iScript_Store(*args)
    def SetTruth(*args): return _blcelc.iScript_SetTruth(*args)
    def RetrieveFloat(*args): return _blcelc.iScript_RetrieveFloat(*args)
    def Retrieve(*args): return _blcelc.iScript_Retrieve(*args)
    def GetTruth(*args): return _blcelc.iScript_GetTruth(*args)
    def Remove(*args): return _blcelc.iScript_Remove(*args)
    def __del__(self, destroy=_blcelc.delete_iScript):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iScript_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iScript_scfGetVersion)

class iScriptPtr(iScript):
    def __init__(self, this):
        _swig_setattr(self, iScript, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iScript, 'thisown', 0)
        _swig_setattr(self, iScript,self.__class__,iScript)
_blcelc.iScript_swigregister(iScriptPtr)

iScript_scfGetVersion = _blcelc.iScript_scfGetVersion

class csObject(iObject):
    __swig_setmethods__ = {}
    for _s in [iObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csObject, name, value)
    __swig_getmethods__ = {}
    for _s in [iObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csObject, name)
    def __repr__(self):
        return "<C csObject instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csObject, 'this', _blcelc.new_csObject(*args))
        _swig_setattr(self, csObject, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    def SetName(*args): return _blcelc.csObject_SetName(*args)
    def GetName(*args): return _blcelc.csObject_GetName(*args)
    def GetID(*args): return _blcelc.csObject_GetID(*args)
    def SetObjectParent(*args): return _blcelc.csObject_SetObjectParent(*args)
    def GetObjectParent(*args): return _blcelc.csObject_GetObjectParent(*args)
    def ObjAdd(*args): return _blcelc.csObject_ObjAdd(*args)
    def ObjRemove(*args): return _blcelc.csObject_ObjRemove(*args)
    def ObjRemoveAll(*args): return _blcelc.csObject_ObjRemoveAll(*args)
    def ObjAddChildren(*args): return _blcelc.csObject_ObjAddChildren(*args)
    def GetChild(*args): return _blcelc.csObject_GetChild(*args)
    def GetIterator(*args): return _blcelc.csObject_GetIterator(*args)
    __swig_setmethods__["scfRefCount"] = _blcelc.csObject_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csObject_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csObject_scfRefCount_get, _blcelc.csObject_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csObject_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csObject_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csObject_scfWeakRefOwners_get, _blcelc.csObject_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc.csObject_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc.csObject_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csObject_scfParent_get
    if _newclass:scfParent = property(_blcelc.csObject_scfParent_get, _blcelc.csObject_scfParent_set)
    def IncRef(*args): return _blcelc.csObject_IncRef(*args)
    def DecRef(*args): return _blcelc.csObject_DecRef(*args)
    def GetRefCount(*args): return _blcelc.csObject_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.csObject_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.csObject_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.csObject_QueryInterface(*args)
    def ObjReleaseOld(*args): return _blcelc.csObject_ObjReleaseOld(*args)

class csObjectPtr(csObject):
    def __init__(self, this):
        _swig_setattr(self, csObject, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csObject, 'thisown', 0)
        _swig_setattr(self, csObject,self.__class__,csObject)
_blcelc.csObject_swigregister(csObjectPtr)

class csView(iView):
    __swig_setmethods__ = {}
    for _s in [iView]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csView, name, value)
    __swig_getmethods__ = {}
    for _s in [iView]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csView, name)
    def __repr__(self):
        return "<C csView instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csView, 'this', _blcelc.new_csView(*args))
        _swig_setattr(self, csView, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csView):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetEngine(*args): return _blcelc.csView_GetEngine(*args)
    def SetEngine(*args): return _blcelc.csView_SetEngine(*args)
    def GetCamera(*args): return _blcelc.csView_GetCamera(*args)
    def SetCamera(*args): return _blcelc.csView_SetCamera(*args)
    def GetContext(*args): return _blcelc.csView_GetContext(*args)
    def SetContext(*args): return _blcelc.csView_SetContext(*args)
    def SetRectangle(*args): return _blcelc.csView_SetRectangle(*args)
    def ClearView(*args): return _blcelc.csView_ClearView(*args)
    def AddViewVertex(*args): return _blcelc.csView_AddViewVertex(*args)
    def RestrictClipperToScreen(*args): return _blcelc.csView_RestrictClipperToScreen(*args)
    def SetAutoResize(*args): return _blcelc.csView_SetAutoResize(*args)
    def UpdateClipper(*args): return _blcelc.csView_UpdateClipper(*args)
    def GetClipper(*args): return _blcelc.csView_GetClipper(*args)
    def Draw(*args): return _blcelc.csView_Draw(*args)
    __swig_setmethods__["scfRefCount"] = _blcelc.csView_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csView_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csView_scfRefCount_get, _blcelc.csView_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csView_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csView_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csView_scfWeakRefOwners_get, _blcelc.csView_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc.csView_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc.csView_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csView_scfParent_get
    if _newclass:scfParent = property(_blcelc.csView_scfParent_get, _blcelc.csView_scfParent_set)
    def IncRef(*args): return _blcelc.csView_IncRef(*args)
    def DecRef(*args): return _blcelc.csView_DecRef(*args)
    def GetRefCount(*args): return _blcelc.csView_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.csView_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.csView_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.csView_QueryInterface(*args)

class csViewPtr(csView):
    def __init__(self, this):
        _swig_setattr(self, csView, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csView, 'thisown', 0)
        _swig_setattr(self, csView,self.__class__,csView)
_blcelc.csView_swigregister(csViewPtr)

class csColliderWrapper(csObject):
    __swig_setmethods__ = {}
    for _s in [csObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csColliderWrapper, name, value)
    __swig_getmethods__ = {}
    for _s in [csObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csColliderWrapper, name)
    def __repr__(self):
        return "<C csColliderWrapper instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csColliderWrapper, 'this', _blcelc.new_csColliderWrapper(*args))
        _swig_setattr(self, csColliderWrapper, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csColliderWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetCollider(*args): return _blcelc.csColliderWrapper_GetCollider(*args)
    def GetCollideSystem(*args): return _blcelc.csColliderWrapper_GetCollideSystem(*args)
    def Collide(*args): return _blcelc.csColliderWrapper_Collide(*args)
    __swig_getmethods__["GetColliderWrapper"] = lambda x: _blcelc.csColliderWrapper_GetColliderWrapper
    if _newclass:GetColliderWrapper = staticmethod(_blcelc.csColliderWrapper_GetColliderWrapper)
    __swig_getmethods__["GetColliderWrapper"] = lambda x: _blcelc.csColliderWrapper_GetColliderWrapper
    if _newclass:GetColliderWrapper = staticmethod(_blcelc.csColliderWrapper_GetColliderWrapper)
    def IncRef(*args): return _blcelc.csColliderWrapper_IncRef(*args)
    def DecRef(*args): return _blcelc.csColliderWrapper_DecRef(*args)
    def GetRefCount(*args): return _blcelc.csColliderWrapper_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc.csColliderWrapper_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc.csColliderWrapper_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc.csColliderWrapper_QueryInterface(*args)

class csColliderWrapperPtr(csColliderWrapper):
    def __init__(self, this):
        _swig_setattr(self, csColliderWrapper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csColliderWrapper, 'thisown', 0)
        _swig_setattr(self, csColliderWrapper,self.__class__,csColliderWrapper)
_blcelc.csColliderWrapper_swigregister(csColliderWrapperPtr)

csColliderWrapper_GetColliderWrapper = _blcelc.csColliderWrapper_GetColliderWrapper

class csColliderHelper(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csColliderHelper, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csColliderHelper, name)
    def __repr__(self):
        return "<C csColliderHelper instance at %s>" % (self.this,)
    __swig_getmethods__["InitializeCollisionWrapper"] = lambda x: _blcelc.csColliderHelper_InitializeCollisionWrapper
    if _newclass:InitializeCollisionWrapper = staticmethod(_blcelc.csColliderHelper_InitializeCollisionWrapper)
    __swig_getmethods__["InitializeCollisionWrappers"] = lambda x: _blcelc.csColliderHelper_InitializeCollisionWrappers
    if _newclass:InitializeCollisionWrappers = staticmethod(_blcelc.csColliderHelper_InitializeCollisionWrappers)
    __swig_getmethods__["CollideArray"] = lambda x: _blcelc.csColliderHelper_CollideArray
    if _newclass:CollideArray = staticmethod(_blcelc.csColliderHelper_CollideArray)
    __swig_getmethods__["CollidePath"] = lambda x: _blcelc.csColliderHelper_CollidePath
    if _newclass:CollidePath = staticmethod(_blcelc.csColliderHelper_CollidePath)
    __swig_getmethods__["TraceBeam"] = lambda x: _blcelc.csColliderHelper_TraceBeam
    if _newclass:TraceBeam = staticmethod(_blcelc.csColliderHelper_TraceBeam)
    def __init__(self, *args):
        _swig_setattr(self, csColliderHelper, 'this', _blcelc.new_csColliderHelper(*args))
        _swig_setattr(self, csColliderHelper, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csColliderHelper):
        try:
            if self.thisown: destroy(self)
        except: pass

class csColliderHelperPtr(csColliderHelper):
    def __init__(self, this):
        _swig_setattr(self, csColliderHelper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csColliderHelper, 'thisown', 0)
        _swig_setattr(self, csColliderHelper,self.__class__,csColliderHelper)
_blcelc.csColliderHelper_swigregister(csColliderHelperPtr)

csColliderHelper_InitializeCollisionWrapper = _blcelc.csColliderHelper_InitializeCollisionWrapper

csColliderHelper_InitializeCollisionWrappers = _blcelc.csColliderHelper_InitializeCollisionWrappers

csColliderHelper_CollideArray = _blcelc.csColliderHelper_CollideArray

csColliderHelper_CollidePath = _blcelc.csColliderHelper_CollidePath

csColliderHelper_TraceBeam = _blcelc.csColliderHelper_TraceBeam


csfxInterference = _blcelc.csfxInterference

csfxFadeOut = _blcelc.csfxFadeOut

csfxFadeTo = _blcelc.csfxFadeTo

csfxFadeToColor = _blcelc.csfxFadeToColor

csfxGreenScreen = _blcelc.csfxGreenScreen

csfxRedScreen = _blcelc.csfxRedScreen

csfxBlueScreen = _blcelc.csfxBlueScreen

csfxWhiteOut = _blcelc.csfxWhiteOut

csfxShadeVert = _blcelc.csfxShadeVert

csfxScreenDPFX = _blcelc.csfxScreenDPFX

csfxScreenDPFXPartial = _blcelc.csfxScreenDPFXPartial
PIXMAP_TOP = _blcelc.PIXMAP_TOP
PIXMAP_LEFT = _blcelc.PIXMAP_LEFT
PIXMAP_CENTER = _blcelc.PIXMAP_CENTER
PIXMAP_BOTTOM = _blcelc.PIXMAP_BOTTOM
PIXMAP_RIGHT = _blcelc.PIXMAP_RIGHT
class csPixmap(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPixmap, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPixmap, name)
    def __repr__(self):
        return "<C csSimplePixmap instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, csPixmap, 'this', _blcelc.new_csPixmap(*args))
        _swig_setattr(self, csPixmap, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete_csPixmap):
        try:
            if self.thisown: destroy(self)
        except: pass
    def SetTextureHandle(*args): return _blcelc.csPixmap_SetTextureHandle(*args)
    def SetTextureRectangle(*args): return _blcelc.csPixmap_SetTextureRectangle(*args)
    def DrawScaled(*args): return _blcelc.csPixmap_DrawScaled(*args)
    def DrawTiled(*args): return _blcelc.csPixmap_DrawTiled(*args)
    def Width(*args): return _blcelc.csPixmap_Width(*args)
    def Height(*args): return _blcelc.csPixmap_Height(*args)
    def Advance(*args): return _blcelc.csPixmap_Advance(*args)
    def GetTextureHandle(*args): return _blcelc.csPixmap_GetTextureHandle(*args)

class csPixmapPtr(csPixmap):
    def __init__(self, this):
        _swig_setattr(self, csPixmap, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPixmap, 'thisown', 0)
        _swig_setattr(self, csPixmap,self.__class__,csPixmap)
_blcelc.csPixmap_swigregister(csPixmapPtr)


CS_IS_KEYBOARD_EVENT = _blcelc.CS_IS_KEYBOARD_EVENT

CS_IS_MOUSE_EVENT = _blcelc.CS_IS_MOUSE_EVENT

CS_IS_JOYSTICK_EVENT = _blcelc.CS_IS_JOYSTICK_EVENT

CS_IS_INPUT_EVENT = _blcelc.CS_IS_INPUT_EVENT

CS_QUERY_REGISTRY_TAG = _blcelc.CS_QUERY_REGISTRY_TAG

CS_LOAD_PLUGIN_ALWAYS = _blcelc.CS_LOAD_PLUGIN_ALWAYS

CS_FX_SETALPHA = _blcelc.CS_FX_SETALPHA

CS_FX_SETALPHA_INT = _blcelc.CS_FX_SETALPHA_INT
CSMASK_Nothing = (1 << csevNothing)
CSMASK_FrameProcess = CSMASK_Nothing
CSMASK_Keyboard = (1 << csevKeyboard)
CSMASK_MouseMove = (1 << csevMouseMove)
CSMASK_MouseDown = (1 << csevMouseDown)
CSMASK_MouseUp = (1 << csevMouseUp)
CSMASK_MouseClick = (1 << csevMouseClick)
CSMASK_MouseDoubleClick = (1 << csevMouseDoubleClick)
CSMASK_JoystickMove = (1 << csevJoystickMove)
CSMASK_JoystickDown = (1 << csevJoystickDown)
CSMASK_JoystickUp = (1 << csevJoystickUp)
CSMASK_Command = (1 << csevCommand)
CSMASK_Broadcast = (1 << csevBroadcast)

CSMASK_Mouse = (CSMASK_MouseMove | CSMASK_MouseDown | CSMASK_MouseUp
  | CSMASK_MouseClick | CSMASK_MouseDoubleClick)
CSMASK_Joystick = (CSMASK_JoystickMove | CSMASK_JoystickDown
  | CSMASK_JoystickUp)
CSMASK_Input = (CSMASK_Keyboard | CSMASK_Mouse | CSMASK_Joystick)


class _csPyEventHandler(iEventHandler):
    __swig_setmethods__ = {}
    for _s in [iEventHandler]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, _csPyEventHandler, name, value)
    __swig_getmethods__ = {}
    for _s in [iEventHandler]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, _csPyEventHandler, name)
    def __repr__(self):
        return "<C _csPyEventHandler instance at %s>" % (self.this,)
    __swig_setmethods__["scfRefCount"] = _blcelc._csPyEventHandler_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc._csPyEventHandler_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc._csPyEventHandler_scfRefCount_get, _blcelc._csPyEventHandler_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc._csPyEventHandler_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc._csPyEventHandler_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc._csPyEventHandler_scfWeakRefOwners_get, _blcelc._csPyEventHandler_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return _blcelc._csPyEventHandler_scfRemoveRefOwners(*args)
    __swig_setmethods__["scfParent"] = _blcelc._csPyEventHandler_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc._csPyEventHandler_scfParent_get
    if _newclass:scfParent = property(_blcelc._csPyEventHandler_scfParent_get, _blcelc._csPyEventHandler_scfParent_set)
    def IncRef(*args): return _blcelc._csPyEventHandler_IncRef(*args)
    def DecRef(*args): return _blcelc._csPyEventHandler_DecRef(*args)
    def GetRefCount(*args): return _blcelc._csPyEventHandler_GetRefCount(*args)
    def AddRefOwner(*args): return _blcelc._csPyEventHandler_AddRefOwner(*args)
    def RemoveRefOwner(*args): return _blcelc._csPyEventHandler_RemoveRefOwner(*args)
    def QueryInterface(*args): return _blcelc._csPyEventHandler_QueryInterface(*args)
    def __init__(self, *args):
        _swig_setattr(self, _csPyEventHandler, 'this', _blcelc.new__csPyEventHandler(*args))
        _swig_setattr(self, _csPyEventHandler, 'thisown', 1)
    def __del__(self, destroy=_blcelc.delete__csPyEventHandler):
        try:
            if self.thisown: destroy(self)
        except: pass
    def HandleEvent(*args): return _blcelc._csPyEventHandler_HandleEvent(*args)

class _csPyEventHandlerPtr(_csPyEventHandler):
    def __init__(self, this):
        _swig_setattr(self, _csPyEventHandler, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, _csPyEventHandler, 'thisown', 0)
        _swig_setattr(self, _csPyEventHandler,self.__class__,_csPyEventHandler)
_blcelc._csPyEventHandler_swigregister(_csPyEventHandlerPtr)

class csPyEventHandler (_csPyEventHandler):
  """Python version of iEventHandler implementation.
     This class can be used as base class for event handlers in Python.
     Call csPyEventHandler.__init__(self) in __init__ of derived class.
  """
  def __init__ (self):
    _csPyEventHandler.__init__(self, self)

class _EventHandlerFuncWrapper (csPyEventHandler):
  def __init__ (self, func):
    csPyEventHandler.__init__(self)
    self._func = func
    # Make sure a reference keeps to this wrapper instance.
    self._func._cs_event_handler_wrapper = self
  def HandleEvent (self, event):
    return self._func(event)

def _csInitializer_SetupEventHandler (reg, obj,
    mask=(CSMASK_FrameProcess|CSMASK_Input|CSMASK_Broadcast)):
  """Replacement of C++ versions."""
  if callable(obj):
    # obj is a function
    hdlr = _EventHandlerFuncWrapper(obj)
    hdlr.thisown = 1
  else:
    # assume it is a iEventHandler
    hdlr = obj
  return csInitializer._SetupEventHandler(reg, hdlr, mask)

csInitializer.SetupEventHandler = \
  staticmethod(_csInitializer_SetupEventHandler)


def _csInitializer_RequestPlugins (reg, plugins):
  """Replacement of C++ version."""
  def _get_tuple (x):
    if callable(x):
      return tuple(x())
    else:
      return tuple(x)
  requests = csPluginRequestArray()
  for cls, intf, ident, ver in map(
      lambda x: _get_tuple(x), plugins):
    requests.Push(csPluginRequest(
      csString(cls), csString(intf), ident, ver))
  return csInitializer._RequestPlugins(reg, requests)

csInitializer.RequestPlugins = staticmethod(_csInitializer_RequestPlugins)



_CS_QUERY_REGISTRY = _blcelc._CS_QUERY_REGISTRY

_CS_QUERY_REGISTRY_TAG_INTERFACE = _blcelc._CS_QUERY_REGISTRY_TAG_INTERFACE

_SCF_QUERY_INTERFACE = _blcelc._SCF_QUERY_INTERFACE

_SCF_QUERY_INTERFACE_SAFE = _blcelc._SCF_QUERY_INTERFACE_SAFE

_CS_QUERY_PLUGIN_CLASS = _blcelc._CS_QUERY_PLUGIN_CLASS

_CS_LOAD_PLUGIN = _blcelc._CS_LOAD_PLUGIN

_CS_GET_CHILD_OBJECT = _blcelc._CS_GET_CHILD_OBJECT

_CS_GET_NAMED_CHILD_OBJECT = _blcelc._CS_GET_NAMED_CHILD_OBJECT

_CS_GET_FIRST_NAMED_CHILD_OBJECT = _blcelc._CS_GET_FIRST_NAMED_CHILD_OBJECT
csReport = csReporterHelper.Report

def _GetIntfId (intf):
  return cvar.iSCF_SCF.GetInterfaceID(intf.__name__)
def _GetIntfVersion (intf):
  return eval('%s_scfGetVersion()' % intf.__name__, locals(), globals())

def CS_QUERY_REGISTRY (reg, intf):
  return _CS_QUERY_REGISTRY (reg, intf.__name__, _GetIntfVersion(intf))

def CS_QUERY_REGISTRY_TAG_INTERFACE (reg, tag, intf):
  return _CS_QUERY_REGISTRY_TAG_INTERFACE (reg, tag, intf.__name__,
    _GetIntfVersion(intf))

def SCF_QUERY_INTERFACE (obj, intf):
  return _SCF_QUERY_INTERFACE (obj, intf.__name__, _GetIntfVersion(intf))

def SCF_QUERY_INTERFACE_SAFE (obj, intf):
  return _SCF_QUERY_INTERFACE_SAFE(obj, intf.__name__,
    _GetIntfVersion(intf))

def CS_GET_CHILD_OBJECT (obj, intf):
  return _CS_GET_CHILD_OBJECT(obj, intf.__name__, _GetIntfVersion(intf))

def CS_GET_NAMED_CHILD_OBJECT (obj, intf, name):
  return _CS_GET_NAMED_CHILD_OBJECT(obj, intf.__name__,
    _GetIntfVersion(intf), name)

def CS_GET_FIRST_NAMED_CHILD_OBJECT (obj, intf, name):
  return CS_GET_FIRST_NAMED_CHILD_OBJECT (obj, intf.__name__,
    _GetIntfVersion(intf), name)

def CS_QUERY_PLUGIN_CLASS (obj, class_id, intf):
  return _CS_QUERY_PLUGIN_CLASS(obj, class_id, intf.__name__,
    _GetIntfVersion(intf))

def CS_LOAD_PLUGIN (obj, class_id, intf):
  return _CS_LOAD_PLUGIN(obj, class_id, intf.__name__,
    _GetIntfVersion(intf))

def CS_REQUEST_PLUGIN (name, intf):
  return (name, intf.__name__, cvar.iSCF_SCF.GetInterfaceID(intf.__name__),
    eval('%s_scfGetVersion()' % intf.__name__, locals(), globals()))

def CS_REQUEST_VFS ():
  return CS_REQUEST_PLUGIN("crystalspace.kernel.vfs", iVFS)

def CS_REQUEST_FONTSERVER ():
  return CS_REQUEST_PLUGIN("crystalspace.font.server.default", iFontServer)

def CS_REQUEST_IMAGELOADER ():
  return CS_REQUEST_PLUGIN("crystalspace.graphic.image.io.multiplex",
    iImageIO)

def CS_REQUEST_NULL3D ():
  return CS_REQUEST_PLUGIN("crystalspace.graphics3d.null", iGraphics3D)

def CS_REQUEST_SOFTWARE3D ():
  return CS_REQUEST_PLUGIN("crystalspace.graphics3d.software", iGraphics3D)

def CS_REQUEST_OPENGL3D ():
  return CS_REQUEST_PLUGIN("crystalspace.graphics3d.opengl", iGraphics3D)

def CS_REQUEST_ENGINE ():
  return CS_REQUEST_PLUGIN("crystalspace.engine.3d", iEngine)

def CS_REQUEST_LEVELLOADER ():
  return CS_REQUEST_PLUGIN("crystalspace.level.loader", iLoader)

def CS_REQUEST_LEVELSAVER ():
  return CS_REQUEST_PLUGIN("crystalspace.level.saver", iSaver)

def CS_REQUEST_REPORTER ():
  return CS_REQUEST_PLUGIN("crystalspace.utilities.reporter", iReporter)

def CS_REQUEST_REPORTERLISTENER ():
  return CS_REQUEST_PLUGIN("crystalspace.utilities.stdrep",
    iStandardReporterListener)

def CS_REQUEST_CONSOLEOUT ():
  return CS_REQUEST_PLUGIN("crystalspace.console.output.simple",
    iConsoleOutput)


class CSMutableArrayHelper:
  def __init__(self, getFunc, lenFunc):
    self.getFunc = getFunc
    self.lenFunc = lenFunc

  def __len__(self):
    return self.lenFunc()

  def __getitem__(self, key):
    if type(key) != type(0):
      raise TypeError()
    arrlen = self.lenFunc()
    if key < 0 or key >= arrlen:
      raise IndexError('Length is ' + str(arrlen) + ', you asked for ' +
        str(key))
    return self.getFunc(key)

  # We do not implement __setitem__ because the only legal action is to
  #  overwrite the object at the given location.  (The contents of the
  #  array are mutable, but the array is a single allocation of a single
  #  type.)  Callers should be using the contained objects' own
  #  __setitem__ or mutation methods.

  # We do not implement __delitem__ because we cannot delete items.

CS_VEC_FORWARD = csVector3(0,0,1)
CS_VEC_BACKWARD = csVector3(0,0,-1)
CS_VEC_RIGHT = csVector3(1,0,0)
CS_VEC_LEFT = csVector3(-1,0,0)
CS_VEC_UP = csVector3(0,1,0)
CS_VEC_DOWN = csVector3(0,-1,0)
CS_VEC_ROT_RIGHT = csVector3(0,1,0)
CS_VEC_ROT_LEFT = csVector3(0,-1,0)
CS_VEC_TILT_RIGHT = -csVector3(0,0,1)
CS_VEC_TILT_LEFT = -csVector3(0,0,-1) 
CS_VEC_TILT_UP = -csVector3(1,0,0)
CS_VEC_TILT_DOWN = -csVector3(-1,0,0)


CS_POLYRANGE_LAST = csPolygonRange (-1, -1)

class celInitializer(csInitializer):
    __swig_setmethods__ = {}
    for _s in [csInitializer]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, celInitializer, name, value)
    __swig_getmethods__ = {}
    for _s in [csInitializer]: __swig_getmethods__.update(_s.__swig_getmethods__)
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

class iCelPlLayer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPlLayer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPlLayer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPlLayer instance at %s>" % (self.this,)
    def FindEntity(*args): return _blcelc.iCelPlLayer_FindEntity(*args)
    def RemoveEntity(*args): return _blcelc.iCelPlLayer_RemoveEntity(*args)
    def LoadPropertyClassFactory(*args): return _blcelc.iCelPlLayer_LoadPropertyClassFactory(*args)
    def CreatePropertyClass(*args): return _blcelc.iCelPlLayer_CreatePropertyClass(*args)
    def FetchString(*args): return _blcelc.iCelPlLayer_FetchString(*args)
    def GetBehaviourLayerCount(*args): return _blcelc.iCelPlLayer_GetBehaviourLayerCount(*args)
    def GetBehaviourLayer(*args): return _blcelc.iCelPlLayer_GetBehaviourLayer(*args)
    def FindBehaviourLayer(*args): return _blcelc.iCelPlLayer_FindBehaviourLayer(*args)
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


csQueryRegistry_iCelPlLayer = _blcelc.csQueryRegistry_iCelPlLayer
class iCelEntity(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntity, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelEntity, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelEntity instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iCelEntity_GetName(*args)
    def SetName(*args): return _blcelc.iCelEntity_SetName(*args)
    def SetBehaviour(*args): return _blcelc.iCelEntity_SetBehaviour(*args)
    def GetBehaviour(*args): return _blcelc.iCelEntity_GetBehaviour(*args)
    def GetID(*args): return _blcelc.iCelEntity_GetID(*args)
    def GetPropertyClassList(*args): return _blcelc.iCelEntity_GetPropertyClassList(*args)
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


celRegisterPCFactory = _blcelc.celRegisterPCFactory

celCreateEntity = _blcelc.celCreateEntity

scfQueryInterface_iCelEntity = _blcelc.scfQueryInterface_iCelEntity
class iCelEntityList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelEntityList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
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


celFindNearbyEntities = _blcelc.celFindNearbyEntities
class iCelBlLayer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelBlLayer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
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
class iCelParameterBlock(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelParameterBlock, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
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

class iCelBehaviour(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelBehaviour, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelBehaviour, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelBehaviour instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iCelBehaviour_GetName(*args)
    def GetBehaviourLayer(*args): return _blcelc.iCelBehaviour_GetBehaviourLayer(*args)
    def SendMessage(*args): return _blcelc.iCelBehaviour_SendMessage(*args)
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

class iCelPropertyClass(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClass, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClass, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPropertyClass instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iCelPropertyClass_GetName(*args)
    def GetEntity(*args): return _blcelc.iCelPropertyClass_GetEntity(*args)
    def SetEntity(*args): return _blcelc.iCelPropertyClass_SetEntity(*args)
    def GetPropertyOrActionType(*args): return _blcelc.iCelPropertyClass_GetPropertyOrActionType(*args)
    def IsPropertyReadOnly(*args): return _blcelc.iCelPropertyClass_IsPropertyReadOnly(*args)
    def GetPropertyLong(*args): return _blcelc.iCelPropertyClass_GetPropertyLong(*args)
    def GetPropertyFloat(*args): return _blcelc.iCelPropertyClass_GetPropertyFloat(*args)
    def GetPropertyBool(*args): return _blcelc.iCelPropertyClass_GetPropertyBool(*args)
    def GetPropertyString(*args): return _blcelc.iCelPropertyClass_GetPropertyString(*args)
    def GetPropertyVector(*args): return _blcelc.iCelPropertyClass_GetPropertyVector(*args)
    def GetPropertyAndActionCount(*args): return _blcelc.iCelPropertyClass_GetPropertyAndActionCount(*args)
    def GetPropertyOrActionID(*args): return _blcelc.iCelPropertyClass_GetPropertyOrActionID(*args)
    def GetPropertyOrActionDescription(*args): return _blcelc.iCelPropertyClass_GetPropertyOrActionDescription(*args)
    def PerformAction(*args): return _blcelc.iCelPropertyClass_PerformAction(*args)
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

class iCelPropertyClassList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPropertyClassList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPropertyClassList, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPropertyClassList instance at %s>" % (self.this,)
    def GetCount(*args): return _blcelc.iCelPropertyClassList_GetCount(*args)
    def Get(*args): return _blcelc.iCelPropertyClassList_Get(*args)
    def Find(*args): return _blcelc.iCelPropertyClassList_Find(*args)
    def FindByName(*args): return _blcelc.iCelPropertyClassList_FindByName(*args)
    def FindByInterface(*args): return _blcelc.iCelPropertyClassList_FindByInterface(*args)
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

class iPcDynamicSystem(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcDynamicSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcDynamicSystem, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcDynamicSystem instance at %s>" % (self.this,)
    def GetDynamicSystem(*args): return _blcelc.iPcDynamicSystem_GetDynamicSystem(*args)
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


celCreateDynamicSystem = _blcelc.celCreateDynamicSystem

celGetDynamicSystem = _blcelc.celGetDynamicSystem

scfQuery_iPcDynamicSystem = _blcelc.scfQuery_iPcDynamicSystem

scfQueryPC_iPcDynamicSystem = _blcelc.scfQueryPC_iPcDynamicSystem
class iPcDynamicBody(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcDynamicBody, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
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


celCreateDynamicBody = _blcelc.celCreateDynamicBody

celGetDynamicBody = _blcelc.celGetDynamicBody

scfQuery_iPcDynamicBody = _blcelc.scfQuery_iPcDynamicBody

scfQueryPC_iPcDynamicBody = _blcelc.scfQueryPC_iPcDynamicBody
class iBillboard(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboard, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboard, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBillboard instance at %s>" % (self.this,)
    def GetName(*args): return _blcelc.iBillboard_GetName(*args)
    def GetFlags(*args): return _blcelc.iBillboard_GetFlags(*args)
    def SetMaterialName(*args): return _blcelc.iBillboard_SetMaterialName(*args)
    def GetMaterialName(*args): return _blcelc.iBillboard_GetMaterialName(*args)
    def SetSize(*args): return _blcelc.iBillboard_SetSize(*args)
    def GetImageSize(*args): return _blcelc.iBillboard_GetImageSize(*args)
    def GetSize(*args): return _blcelc.iBillboard_GetSize(*args)
    def SetPosition(*args): return _blcelc.iBillboard_SetPosition(*args)
    def GetPosition(*args): return _blcelc.iBillboard_GetPosition(*args)
    def Move(*args): return _blcelc.iBillboard_Move(*args)
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

class iBillboardManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboardManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboardManager, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBillboardManager instance at %s>" % (self.this,)
    def CreateBillboard(*args): return _blcelc.iBillboardManager_CreateBillboard(*args)
    def FindBillboard(*args): return _blcelc.iBillboardManager_FindBillboard(*args)
    def RemoveBillboard(*args): return _blcelc.iBillboardManager_RemoveBillboard(*args)
    def GetBillboardCount(*args): return _blcelc.iBillboardManager_GetBillboardCount(*args)
    def GetBillboard(*args): return _blcelc.iBillboardManager_GetBillboard(*args)
    def RemoveAll(*args): return _blcelc.iBillboardManager_RemoveAll(*args)
    def SetFlags(*args): return _blcelc.iBillboardManager_SetFlags(*args)
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

class iPcBillboard(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcBillboard, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcBillboard, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcBillboard instance at %s>" % (self.this,)
    def SetBillboardName(*args): return _blcelc.iPcBillboard_SetBillboardName(*args)
    def GetBillboardName(*args): return _blcelc.iPcBillboard_GetBillboardName(*args)
    def GetBillboard(*args): return _blcelc.iPcBillboard_GetBillboard(*args)
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
class iPcRegion(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcRegion, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcRegion, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcRegion instance at %s>" % (self.this,)
    def SetWorldFile(*args): return _blcelc.iPcRegion_SetWorldFile(*args)
    def GetWorldDir(*args): return _blcelc.iPcRegion_GetWorldDir(*args)
    def GetWorldFile(*args): return _blcelc.iPcRegion_GetWorldFile(*args)
    def CreateEmptySector(*args): return _blcelc.iPcRegion_CreateEmptySector(*args)
    def SetRegionName(*args): return _blcelc.iPcRegion_SetRegionName(*args)
    def GetRegionName(*args): return _blcelc.iPcRegion_GetRegionName(*args)
    def Load(*args): return _blcelc.iPcRegion_Load(*args)
    def Unload(*args): return _blcelc.iPcRegion_Unload(*args)
    def FindSector(*args): return _blcelc.iPcRegion_FindSector(*args)
    def GetStartSector(*args): return _blcelc.iPcRegion_GetStartSector(*args)
    def GetStartPosition(*args): return _blcelc.iPcRegion_GetStartPosition(*args)
    def PointCamera(*args): return _blcelc.iPcRegion_PointCamera(*args)
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
class iPcCommandInput(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCommandInput, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCommandInput, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCommandInput instance at %s>" % (self.this,)
    def Activate(*args): return _blcelc.iPcCommandInput_Activate(*args)
    def LoadConfig(*args): return _blcelc.iPcCommandInput_LoadConfig(*args)
    def Bind(*args): return _blcelc.iPcCommandInput_Bind(*args)
    def GetBind(*args): return _blcelc.iPcCommandInput_GetBind(*args)
    def RemoveBind(*args): return _blcelc.iPcCommandInput_RemoveBind(*args)
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
class iPcLinearMovement(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcLinearMovement, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcLinearMovement, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcLinearMovement instance at %s>" % (self.this,)
    def SetAngularVelocity(*args): return _blcelc.iPcLinearMovement_SetAngularVelocity(*args)
    def SetSpeed(*args): return _blcelc.iPcLinearMovement_SetSpeed(*args)
    def SetVelocity(*args): return _blcelc.iPcLinearMovement_SetVelocity(*args)
    def GetVelocity(*args): return _blcelc.iPcLinearMovement_GetVelocity(*args)
    def GetAngularVelocity(*args): return _blcelc.iPcLinearMovement_GetAngularVelocity(*args)
    def InitCD(*args): return _blcelc.iPcLinearMovement_InitCD(*args)
    def GetDRData(*args): return _blcelc.iPcLinearMovement_GetDRData(*args)
    def SetDRData(*args): return _blcelc.iPcLinearMovement_SetDRData(*args)
    def SetPosition(*args): return _blcelc.iPcLinearMovement_SetPosition(*args)
    def GetLastPosition(*args): return _blcelc.iPcLinearMovement_GetLastPosition(*args)
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
    def SetDeltaLimit(*args): return _blcelc.iPcLinearMovement_SetDeltaLimit(*args)
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
class iPcCamera(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCamera, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCamera instance at %s>" % (self.this,)
    freelook = _blcelc.iPcCamera_freelook
    firstperson = _blcelc.iPcCamera_firstperson
    thirdperson = _blcelc.iPcCamera_thirdperson
    m64_thirdperson = _blcelc.iPcCamera_m64_thirdperson
    lara_thirdperson = _blcelc.iPcCamera_lara_thirdperson
    def SetRegion(*args): return _blcelc.iPcCamera_SetRegion(*args)
    def SetZoneManager(*args): return _blcelc.iPcCamera_SetZoneManager(*args)
    def SetMode(*args): return _blcelc.iPcCamera_SetMode(*args)
    def GetMode(*args): return _blcelc.iPcCamera_GetMode(*args)
    def SetModeName(*args): return _blcelc.iPcCamera_SetModeName(*args)
    def GetModeName(*args): return _blcelc.iPcCamera_GetModeName(*args)
    def GetNextMode(*args): return _blcelc.iPcCamera_GetNextMode(*args)
    def SetSpringParameters(*args): return _blcelc.iPcCamera_SetSpringParameters(*args)
    def SetMinMaxCameraDistance(*args): return _blcelc.iPcCamera_SetMinMaxCameraDistance(*args)
    def SetTurnSpeed(*args): return _blcelc.iPcCamera_SetTurnSpeed(*args)
    def SetSwingCoef(*args): return _blcelc.iPcCamera_SetSwingCoef(*args)
    def SetFirstPersonOffset(*args): return _blcelc.iPcCamera_SetFirstPersonOffset(*args)
    def SetThirdPersonOffset(*args): return _blcelc.iPcCamera_SetThirdPersonOffset(*args)
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
CEL_MOUSE_BUTTON1 = _blcelc.CEL_MOUSE_BUTTON1
CEL_MOUSE_BUTTON2 = _blcelc.CEL_MOUSE_BUTTON2
CEL_MOUSE_BUTTON3 = _blcelc.CEL_MOUSE_BUTTON3
class iPcMeshSelect(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMeshSelect, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMeshSelect, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMeshSelect instance at %s>" % (self.this,)
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
class iPcMesh(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMesh, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMesh, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMesh instance at %s>" % (self.this,)
    def LoadMesh(*args): return _blcelc.iPcMesh_LoadMesh(*args)
    def SetMesh(*args): return _blcelc.iPcMesh_SetMesh(*args)
    def CreateEmptyThing(*args): return _blcelc.iPcMesh_CreateEmptyThing(*args)
    def GetMesh(*args): return _blcelc.iPcMesh_GetMesh(*args)
    def MoveMesh(*args): return _blcelc.iPcMesh_MoveMesh(*args)
    def SetAction(*args): return _blcelc.iPcMesh_SetAction(*args)
    def GetAction(*args): return _blcelc.iPcMesh_GetAction(*args)
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
class iPcTimer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcTimer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcTimer, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcTimer instance at %s>" % (self.this,)
    def WakeUp(*args): return _blcelc.iPcTimer_WakeUp(*args)
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
class iPcSolid(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcSolid, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
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
class iPcGravity(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcGravity, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcGravity, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcGravity instance at %s>" % (self.this,)
    def CreateGravityColliderFromMesh(*args): return _blcelc.iPcGravity_CreateGravityColliderFromMesh(*args)
    def CreateGravityCollider(*args): return _blcelc.iPcGravity_CreateGravityCollider(*args)
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
class iPcMovable(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMovable, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMovable, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMovable instance at %s>" % (self.this,)
    def SetMesh(*args): return _blcelc.iPcMovable_SetMesh(*args)
    def GetMesh(*args): return _blcelc.iPcMovable_GetMesh(*args)
    def SetPos(*args): return _blcelc.iPcMovable_SetPos(*args)
    def Move(*args): return _blcelc.iPcMovable_Move(*args)
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


celCreateMovable = _blcelc.celCreateMovable

celGetMovable = _blcelc.celGetMovable

scfQuery_iPcMovable = _blcelc.scfQuery_iPcMovable
class iPcInventory(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcInventory, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcInventory, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcInventory instance at %s>" % (self.this,)
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
class iPcCharacteristics(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCharacteristics, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
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
CEL_DATA_BUFFER = _blcelc.CEL_DATA_BUFFER
CEL_DATA_ACTION = _blcelc.CEL_DATA_ACTION
CEL_DATA_COLOR = _blcelc.CEL_DATA_COLOR
CEL_DATA_IBASE = _blcelc.CEL_DATA_IBASE

