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


class csWrapPtr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csWrapPtr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csWrapPtr, name)
    __swig_setmethods__["Ref"] = _blcelc.csWrapPtr_Ref_set
    __swig_getmethods__["Ref"] = _blcelc.csWrapPtr_Ref_get
    if _newclass:Ref = property(_blcelc.csWrapPtr_Ref_get,_blcelc.csWrapPtr_Ref_set)
    __swig_setmethods__["VoidPtr"] = _blcelc.csWrapPtr_VoidPtr_set
    __swig_getmethods__["VoidPtr"] = _blcelc.csWrapPtr_VoidPtr_get
    if _newclass:VoidPtr = property(_blcelc.csWrapPtr_VoidPtr_get,_blcelc.csWrapPtr_VoidPtr_set)
    __swig_getmethods__["Type"] = _blcelc.csWrapPtr_Type_get
    if _newclass:Type = property(_blcelc.csWrapPtr_Type_get)
    def __init__(self,*args):
        _swig_setattr(self, csWrapPtr, 'this', apply(_blcelc.new_csWrapPtr,args))
        _swig_setattr(self, csWrapPtr, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csWrapPtr):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csWrapPtr instance at %s>" % (self.this,)

class csWrapPtrPtr(csWrapPtr):
    def __init__(self,this):
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
    def IncRef(*args): return apply(_blcelc.iBase_IncRef,args)
    def DecRef(*args): return apply(_blcelc.iBase_DecRef,args)
    def GetRefCount(*args): return apply(_blcelc.iBase_GetRefCount,args)
    def QueryInterface(*args): return apply(_blcelc.iBase_QueryInterface,args)
    __swig_getmethods__["QueryInterfaceSafe"] = lambda x: _blcelc.iBase_QueryInterfaceSafe
    if _newclass:QueryInterfaceSafe = staticmethod(_blcelc.iBase_QueryInterfaceSafe)
    def AddRefOwner(*args): return apply(_blcelc.iBase_AddRefOwner,args)
    def RemoveRefOwner(*args): return apply(_blcelc.iBase_RemoveRefOwner,args)
    def __del__(self, destroy= _blcelc.delete_iBase):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iBase_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iBase_scfGetVersion)
    def _DynamicCast(*args): return apply(_blcelc.iBase__DynamicCast,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBase instance at %s>" % (self.this,)

class iBasePtr(iBase):
    def __init__(self,this):
        _swig_setattr(self, iBase, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBase, 'thisown', 0)
        _swig_setattr(self, iBase,self.__class__,iBase)
_blcelc.iBase_swigregister(iBasePtr)
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
    def CreateInstance(*args): return apply(_blcelc.iFactory_CreateInstance,args)
    def TryUnload(*args): return apply(_blcelc.iFactory_TryUnload,args)
    def QueryDescription(*args): return apply(_blcelc.iFactory_QueryDescription,args)
    def QueryDependencies(*args): return apply(_blcelc.iFactory_QueryDependencies,args)
    def QueryClassID(*args): return apply(_blcelc.iFactory_QueryClassID,args)
    def __del__(self, destroy= _blcelc.delete_iFactory):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFactory_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFactory_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFactory instance at %s>" % (self.this,)

class iFactoryPtr(iFactory):
    def __init__(self,this):
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
    def RegisterClasses(*args): return apply(_blcelc.iSCF_RegisterClasses,args)
    def ClassRegistered(*args): return apply(_blcelc.iSCF_ClassRegistered,args)
    def CreateInstance(*args): return apply(_blcelc.iSCF_CreateInstance,args)
    def GetClassDescription(*args): return apply(_blcelc.iSCF_GetClassDescription,args)
    def GetClassDependencies(*args): return apply(_blcelc.iSCF_GetClassDependencies,args)
    def UnloadUnusedModules(*args): return apply(_blcelc.iSCF_UnloadUnusedModules,args)
    def RegisterClass(*args): return apply(_blcelc.iSCF_RegisterClass,args)
    def RegisterFactoryFunc(*args): return apply(_blcelc.iSCF_RegisterFactoryFunc,args)
    def UnregisterClass(*args): return apply(_blcelc.iSCF_UnregisterClass,args)
    def GetInterfaceID(*args): return apply(_blcelc.iSCF_GetInterfaceID,args)
    def Finish(*args): return apply(_blcelc.iSCF_Finish,args)
    def QueryClassList(*args): return apply(_blcelc.iSCF_QueryClassList,args)
    def ScanPluginsPath(*args): return apply(_blcelc.iSCF_ScanPluginsPath,args)
    def RegisterPlugin(*args): return apply(_blcelc.iSCF_RegisterPlugin,args)
    def __del__(self, destroy= _blcelc.delete_iSCF):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSCF_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSCF_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSCF instance at %s>" % (self.this,)

class iSCFPtr(iSCF):
    def __init__(self,this):
        _swig_setattr(self, iSCF, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSCF, 'thisown', 0)
        _swig_setattr(self, iSCF,self.__class__,iSCF)
_blcelc.iSCF_swigregister(iSCFPtr)
scfInitialize = _blcelc.scfInitialize

cvar = _blcelc.cvar
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
    def GetSupportedTests(*args): return apply(_blcelc.iDebugHelper_GetSupportedTests,args)
    def UnitTest(*args): return apply(_blcelc.iDebugHelper_UnitTest,args)
    def StateTest(*args): return apply(_blcelc.iDebugHelper_StateTest,args)
    def Benchmark(*args): return apply(_blcelc.iDebugHelper_Benchmark,args)
    def Dump(*args): return apply(_blcelc.iDebugHelper_Dump,args)
    def DebugCommand(*args): return apply(_blcelc.iDebugHelper_DebugCommand,args)
    def __del__(self, destroy= _blcelc.delete_iDebugHelper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDebugHelper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDebugHelper_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDebugHelper instance at %s>" % (self.this,)

class iDebugHelperPtr(iDebugHelper):
    def __init__(self,this):
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
    __swig_setmethods__["red"] = _blcelc.csColor_red_set
    __swig_getmethods__["red"] = _blcelc.csColor_red_get
    if _newclass:red = property(_blcelc.csColor_red_get,_blcelc.csColor_red_set)
    __swig_setmethods__["green"] = _blcelc.csColor_green_set
    __swig_getmethods__["green"] = _blcelc.csColor_green_get
    if _newclass:green = property(_blcelc.csColor_green_get,_blcelc.csColor_green_set)
    __swig_setmethods__["blue"] = _blcelc.csColor_blue_set
    __swig_getmethods__["blue"] = _blcelc.csColor_blue_get
    if _newclass:blue = property(_blcelc.csColor_blue_get,_blcelc.csColor_blue_set)
    def __init__(self,*args):
        _swig_setattr(self, csColor, 'this', apply(_blcelc.new_csColor,args))
        _swig_setattr(self, csColor, 'thisown', 1)
    def Set(*args): return apply(_blcelc.csColor_Set,args)
    def Clamp(*args): return apply(_blcelc.csColor_Clamp,args)
    def ClampDown(*args): return apply(_blcelc.csColor_ClampDown,args)
    def assign(*args): return apply(_blcelc.csColor_assign,args)
    def __imul__(*args): return apply(_blcelc.csColor___imul__,args)
    def __iadd__(*args): return apply(_blcelc.csColor___iadd__,args)
    def __isub__(*args): return apply(_blcelc.csColor___isub__,args)
    def Add(*args): return apply(_blcelc.csColor_Add,args)
    def Subtract(*args): return apply(_blcelc.csColor_Subtract,args)
    def __add__(*args): return apply(_blcelc.csColor___add__,args)
    def __sub__(*args): return apply(_blcelc.csColor___sub__,args)
    def __mul__(*args): return apply(_blcelc.csColor___mul__,args)
    def __rmul__(*args): return apply(_blcelc.csColor___rmul__,args)
    def __del__(self, destroy= _blcelc.delete_csColor):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csColor instance at %s>" % (self.this,)

class csColorPtr(csColor):
    def __init__(self,this):
        _swig_setattr(self, csColor, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csColor, 'thisown', 0)
        _swig_setattr(self, csColor,self.__class__,csColor)
_blcelc.csColor_swigregister(csColorPtr)

class csCommandLineHelper(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csCommandLineHelper, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csCommandLineHelper, name)
    __swig_getmethods__["Help"] = lambda x: _blcelc.csCommandLineHelper_Help
    if _newclass:Help = staticmethod(_blcelc.csCommandLineHelper_Help)
    __swig_getmethods__["CheckHelp"] = lambda x: _blcelc.csCommandLineHelper_CheckHelp
    if _newclass:CheckHelp = staticmethod(_blcelc.csCommandLineHelper_CheckHelp)
    def __init__(self,*args):
        _swig_setattr(self, csCommandLineHelper, 'this', apply(_blcelc.new_csCommandLineHelper,args))
        _swig_setattr(self, csCommandLineHelper, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csCommandLineHelper):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csCommandLineHelper instance at %s>" % (self.this,)

class csCommandLineHelperPtr(csCommandLineHelper):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csStringSetIterator, 'this', apply(_blcelc.new_csStringSetIterator,args))
        _swig_setattr(self, csStringSetIterator, 'thisown', 1)
    def HasNext(*args): return apply(_blcelc.csStringSetIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.csStringSetIterator_Next,args)
    def __del__(self, destroy= _blcelc.delete_csStringSetIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csStringSetIterator instance at %s>" % (self.this,)

class csStringSetIteratorPtr(csStringSetIterator):
    def __init__(self,this):
        _swig_setattr(self, csStringSetIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csStringSetIterator, 'thisown', 0)
        _swig_setattr(self, csStringSetIterator,self.__class__,csStringSetIterator)
_blcelc.csStringSetIterator_swigregister(csStringSetIteratorPtr)

class csStringSet(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csStringSet, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csStringSet, name)
    def __init__(self,*args):
        _swig_setattr(self, csStringSet, 'this', apply(_blcelc.new_csStringSet,args))
        _swig_setattr(self, csStringSet, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csStringSet):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Request(*args): return apply(_blcelc.csStringSet_Request,args)
    def Clear(*args): return apply(_blcelc.csStringSet_Clear,args)
    def __repr__(self):
        return "<C csStringSet instance at %s>" % (self.this,)

class csStringSetPtr(csStringSet):
    def __init__(self,this):
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
    def SetCapacity(*args): return apply(_blcelc.iString_SetCapacity,args)
    def GetCapacity(*args): return apply(_blcelc.iString_GetCapacity,args)
    def SetGrowsBy(*args): return apply(_blcelc.iString_SetGrowsBy,args)
    def GetGrowsBy(*args): return apply(_blcelc.iString_GetGrowsBy,args)
    def SetGrowsExponentially(*args): return apply(_blcelc.iString_SetGrowsExponentially,args)
    def GetGrowsExponentially(*args): return apply(_blcelc.iString_GetGrowsExponentially,args)
    def Truncate(*args): return apply(_blcelc.iString_Truncate,args)
    def Reclaim(*args): return apply(_blcelc.iString_Reclaim,args)
    def Clear(*args): return apply(_blcelc.iString_Clear,args)
    def Clone(*args): return apply(_blcelc.iString_Clone,args)
    def GetData(*args): return apply(_blcelc.iString_GetData,args)
    def Length(*args): return apply(_blcelc.iString_Length,args)
    def IsEmpty(*args): return apply(_blcelc.iString_IsEmpty,args)
    def SetAt(*args): return apply(_blcelc.iString_SetAt,args)
    def GetAt(*args): return apply(_blcelc.iString_GetAt,args)
    def Insert(*args): return apply(_blcelc.iString_Insert,args)
    def Overwrite(*args): return apply(_blcelc.iString_Overwrite,args)
    def Append(*args): return apply(_blcelc.iString_Append,args)
    def Slice(*args): return apply(_blcelc.iString_Slice,args)
    def SubString(*args): return apply(_blcelc.iString_SubString,args)
    def FindFirst(*args): return apply(_blcelc.iString_FindFirst,args)
    def FindLast(*args): return apply(_blcelc.iString_FindLast,args)
    def Format(*args): return apply(_blcelc.iString_Format,args)
    def FormatV(*args): return apply(_blcelc.iString_FormatV,args)
    def Replace(*args): return apply(_blcelc.iString_Replace,args)
    def Compare(*args): return apply(_blcelc.iString_Compare,args)
    def CompareNoCase(*args): return apply(_blcelc.iString_CompareNoCase,args)
    def __iadd__(*args): return apply(_blcelc.iString___iadd__,args)
    def __add__(*args): return apply(_blcelc.iString___add__,args)
    def __eq__(*args): return apply(_blcelc.iString___eq__,args)
    def Downcase(*args): return apply(_blcelc.iString_Downcase,args)
    def Upcase(*args): return apply(_blcelc.iString_Upcase,args)
    def __del__(self, destroy= _blcelc.delete_iString):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iString_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iString_scfGetVersion)
    def __getitem__(*args): return apply(_blcelc.iString___getitem__,args)
    def __setitem__(*args): return apply(_blcelc.iString___setitem__,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iString instance at %s>" % (self.this,)

class iStringPtr(iString):
    def __init__(self,this):
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
    def SetCapacity(*args): return apply(_blcelc.csString_SetCapacity,args)
    def GetCapacity(*args): return apply(_blcelc.csString_GetCapacity,args)
    def SetGrowsBy(*args): return apply(_blcelc.csString_SetGrowsBy,args)
    def GetGrowsBy(*args): return apply(_blcelc.csString_GetGrowsBy,args)
    def SetGrowsExponentially(*args): return apply(_blcelc.csString_SetGrowsExponentially,args)
    def GetGrowsExponentially(*args): return apply(_blcelc.csString_GetGrowsExponentially,args)
    def Free(*args): return apply(_blcelc.csString_Free,args)
    def Truncate(*args): return apply(_blcelc.csString_Truncate,args)
    def Reclaim(*args): return apply(_blcelc.csString_Reclaim,args)
    def Clear(*args): return apply(_blcelc.csString_Clear,args)
    def GetData(*args): return apply(_blcelc.csString_GetData,args)
    def Length(*args): return apply(_blcelc.csString_Length,args)
    def IsEmpty(*args): return apply(_blcelc.csString_IsEmpty,args)
    def SetAt(*args): return apply(_blcelc.csString_SetAt,args)
    def GetAt(*args): return apply(_blcelc.csString_GetAt,args)
    def DeleteAt(*args): return apply(_blcelc.csString_DeleteAt,args)
    def Insert(*args): return apply(_blcelc.csString_Insert,args)
    def Overwrite(*args): return apply(_blcelc.csString_Overwrite,args)
    def Append(*args): return apply(_blcelc.csString_Append,args)
    def Slice(*args): return apply(_blcelc.csString_Slice,args)
    def SubString(*args): return apply(_blcelc.csString_SubString,args)
    def FindFirst(*args): return apply(_blcelc.csString_FindFirst,args)
    def FindLast(*args): return apply(_blcelc.csString_FindLast,args)
    def Replace(*args): return apply(_blcelc.csString_Replace,args)
    def Compare(*args): return apply(_blcelc.csString_Compare,args)
    def CompareNoCase(*args): return apply(_blcelc.csString_CompareNoCase,args)
    def __init__(self,*args):
        _swig_setattr(self, csString, 'this', apply(_blcelc.new_csString,args))
        _swig_setattr(self, csString, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csString):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Clone(*args): return apply(_blcelc.csString_Clone,args)
    def LTrim(*args): return apply(_blcelc.csString_LTrim,args)
    def RTrim(*args): return apply(_blcelc.csString_RTrim,args)
    def Trim(*args): return apply(_blcelc.csString_Trim,args)
    def Collapse(*args): return apply(_blcelc.csString_Collapse,args)
    def Format(*args): return apply(_blcelc.csString_Format,args)
    def FormatV(*args): return apply(_blcelc.csString_FormatV,args)
    def AsPadLeft(*args): return apply(_blcelc.csString_AsPadLeft,args)
    __swig_getmethods__["PadLeft"] = lambda x: _blcelc.csString_PadLeft
    if _newclass:PadLeft = staticmethod(_blcelc.csString_PadLeft)
    def AsPadRight(*args): return apply(_blcelc.csString_AsPadRight,args)
    __swig_getmethods__["PadRight"] = lambda x: _blcelc.csString_PadRight
    if _newclass:PadRight = staticmethod(_blcelc.csString_PadRight)
    def AsPadCenter(*args): return apply(_blcelc.csString_AsPadCenter,args)
    __swig_getmethods__["PadCenter"] = lambda x: _blcelc.csString_PadCenter
    if _newclass:PadCenter = staticmethod(_blcelc.csString_PadCenter)
    def assign(*args): return apply(_blcelc.csString_assign,args)
    def __iadd__(*args): return apply(_blcelc.csString___iadd__,args)
    def __add__(*args): return apply(_blcelc.csString___add__,args)
    def __eq__(*args): return apply(_blcelc.csString___eq__,args)
    def __ne__(*args): return apply(_blcelc.csString___ne__,args)
    def Downcase(*args): return apply(_blcelc.csString_Downcase,args)
    def Upcase(*args): return apply(_blcelc.csString_Upcase,args)
    def Detach(*args): return apply(_blcelc.csString_Detach,args)
    def __getitem__(*args): return apply(_blcelc.csString___getitem__,args)
    def __setitem__(*args): return apply(_blcelc.csString___setitem__,args)
    def __delitem__(*args): return apply(_blcelc.csString___delitem__,args)
    def __repr__(self):
        return "<C csString instance at %s>" % (self.this,)

class csStringPtr(csString):
    def __init__(self,this):
        _swig_setattr(self, csString, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csString, 'thisown', 0)
        _swig_setattr(self, csString,self.__class__,csString)
_blcelc.csString_swigregister(csStringPtr)
csString_PadLeft = _blcelc.csString_PadLeft

csString_PadRight = _blcelc.csString_PadRight

csString_PadCenter = _blcelc.csString_PadCenter


class csVector2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csVector2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csVector2, name)
    __swig_setmethods__["x"] = _blcelc.csVector2_x_set
    __swig_getmethods__["x"] = _blcelc.csVector2_x_get
    if _newclass:x = property(_blcelc.csVector2_x_get,_blcelc.csVector2_x_set)
    __swig_setmethods__["y"] = _blcelc.csVector2_y_set
    __swig_getmethods__["y"] = _blcelc.csVector2_y_get
    if _newclass:y = property(_blcelc.csVector2_y_get,_blcelc.csVector2_y_set)
    def __init__(self,*args):
        _swig_setattr(self, csVector2, 'this', apply(_blcelc.new_csVector2,args))
        _swig_setattr(self, csVector2, 'thisown', 1)
    def Set(*args): return apply(_blcelc.csVector2_Set,args)
    def Norm(*args): return apply(_blcelc.csVector2_Norm,args)
    def SquaredNorm(*args): return apply(_blcelc.csVector2_SquaredNorm,args)
    def Rotate(*args): return apply(_blcelc.csVector2_Rotate,args)
    def __iadd__(*args): return apply(_blcelc.csVector2___iadd__,args)
    def __isub__(*args): return apply(_blcelc.csVector2___isub__,args)
    def __imul__(*args): return apply(_blcelc.csVector2___imul__,args)
    def __idiv__(*args): return apply(_blcelc.csVector2___idiv__,args)
    def __pos__(*args): return apply(_blcelc.csVector2___pos__,args)
    def __neg__(*args): return apply(_blcelc.csVector2___neg__,args)
    def __add__(*args): return apply(_blcelc.csVector2___add__,args)
    def __sub__(*args): return apply(_blcelc.csVector2___sub__,args)
    def __mul__(*args): return apply(_blcelc.csVector2___mul__,args)
    def __div__(*args): return apply(_blcelc.csVector2___div__,args)
    def __eq__(*args): return apply(_blcelc.csVector2___eq__,args)
    def __ne__(*args): return apply(_blcelc.csVector2___ne__,args)
    def __lt__(*args): return apply(_blcelc.csVector2___lt__,args)
    def __gt__(*args): return apply(_blcelc.csVector2___gt__,args)
    def __rmul__(*args): return apply(_blcelc.csVector2___rmul__,args)
    def __abs__(*args): return apply(_blcelc.csVector2___abs__,args)
    def __getitem__(*args): return apply(_blcelc.csVector2___getitem__,args)
    def __setitem__(*args): return apply(_blcelc.csVector2___setitem__,args)
    def __del__(self, destroy= _blcelc.delete_csVector2):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csVector2 instance at %s>" % (self.this,)

class csVector2Ptr(csVector2):
    def __init__(self,this):
        _swig_setattr(self, csVector2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csVector2, 'thisown', 0)
        _swig_setattr(self, csVector2,self.__class__,csVector2)
_blcelc.csVector2_swigregister(csVector2Ptr)

class csVector3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csVector3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csVector3, name)
    __swig_setmethods__["x"] = _blcelc.csVector3_x_set
    __swig_getmethods__["x"] = _blcelc.csVector3_x_get
    if _newclass:x = property(_blcelc.csVector3_x_get,_blcelc.csVector3_x_set)
    __swig_setmethods__["y"] = _blcelc.csVector3_y_set
    __swig_getmethods__["y"] = _blcelc.csVector3_y_get
    if _newclass:y = property(_blcelc.csVector3_y_get,_blcelc.csVector3_y_set)
    __swig_setmethods__["z"] = _blcelc.csVector3_z_set
    __swig_getmethods__["z"] = _blcelc.csVector3_z_get
    if _newclass:z = property(_blcelc.csVector3_z_get,_blcelc.csVector3_z_set)
    def __init__(self,*args):
        _swig_setattr(self, csVector3, 'this', apply(_blcelc.new_csVector3,args))
        _swig_setattr(self, csVector3, 'thisown', 1)
    def Cross(*args): return apply(_blcelc.csVector3_Cross,args)
    def __iadd__(*args): return apply(_blcelc.csVector3___iadd__,args)
    def __isub__(*args): return apply(_blcelc.csVector3___isub__,args)
    def __pos__(*args): return apply(_blcelc.csVector3___pos__,args)
    def __neg__(*args): return apply(_blcelc.csVector3___neg__,args)
    def Set(*args): return apply(_blcelc.csVector3_Set,args)
    def Norm(*args): return apply(_blcelc.csVector3_Norm,args)
    def SquaredNorm(*args): return apply(_blcelc.csVector3_SquaredNorm,args)
    def Unit(*args): return apply(_blcelc.csVector3_Unit,args)
    def Normalize(*args): return apply(_blcelc.csVector3_Normalize,args)
    def IsZero(*args): return apply(_blcelc.csVector3_IsZero,args)
    def __add__(*args): return apply(_blcelc.csVector3___add__,args)
    def __sub__(*args): return apply(_blcelc.csVector3___sub__,args)
    def __mul__(*args): return apply(_blcelc.csVector3___mul__,args)
    def __eq__(*args): return apply(_blcelc.csVector3___eq__,args)
    def __ne__(*args): return apply(_blcelc.csVector3___ne__,args)
    def __lt__(*args): return apply(_blcelc.csVector3___lt__,args)
    def __gt__(*args): return apply(_blcelc.csVector3___gt__,args)
    def __imul__(*args): return apply(_blcelc.csVector3___imul__,args)
    def __idiv__(*args): return apply(_blcelc.csVector3___idiv__,args)
    def __div__(*args): return apply(_blcelc.csVector3___div__,args)
    def project(*args): return apply(_blcelc.csVector3_project,args)
    def __rmul__(*args): return apply(_blcelc.csVector3___rmul__,args)
    def __abs__(*args): return apply(_blcelc.csVector3___abs__,args)
    def __getitem__(*args): return apply(_blcelc.csVector3___getitem__,args)
    def __setitem__(*args): return apply(_blcelc.csVector3___setitem__,args)
    def __nonzero__(*args): return apply(_blcelc.csVector3___nonzero__,args)
    def __del__(self, destroy= _blcelc.delete_csVector3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csVector3 instance at %s>" % (self.this,)

class csVector3Ptr(csVector3):
    def __init__(self,this):
        _swig_setattr(self, csVector3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csVector3, 'thisown', 0)
        _swig_setattr(self, csVector3,self.__class__,csVector3)
_blcelc.csVector3_swigregister(csVector3Ptr)

class csMatrix2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csMatrix2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csMatrix2, name)
    __swig_setmethods__["m11"] = _blcelc.csMatrix2_m11_set
    __swig_getmethods__["m11"] = _blcelc.csMatrix2_m11_get
    if _newclass:m11 = property(_blcelc.csMatrix2_m11_get,_blcelc.csMatrix2_m11_set)
    __swig_setmethods__["m12"] = _blcelc.csMatrix2_m12_set
    __swig_getmethods__["m12"] = _blcelc.csMatrix2_m12_get
    if _newclass:m12 = property(_blcelc.csMatrix2_m12_get,_blcelc.csMatrix2_m12_set)
    __swig_setmethods__["m21"] = _blcelc.csMatrix2_m21_set
    __swig_getmethods__["m21"] = _blcelc.csMatrix2_m21_get
    if _newclass:m21 = property(_blcelc.csMatrix2_m21_get,_blcelc.csMatrix2_m21_set)
    __swig_setmethods__["m22"] = _blcelc.csMatrix2_m22_set
    __swig_getmethods__["m22"] = _blcelc.csMatrix2_m22_get
    if _newclass:m22 = property(_blcelc.csMatrix2_m22_get,_blcelc.csMatrix2_m22_set)
    def __init__(self,*args):
        _swig_setattr(self, csMatrix2, 'this', apply(_blcelc.new_csMatrix2,args))
        _swig_setattr(self, csMatrix2, 'thisown', 1)
    def Row1(*args): return apply(_blcelc.csMatrix2_Row1,args)
    def Row2(*args): return apply(_blcelc.csMatrix2_Row2,args)
    def Col1(*args): return apply(_blcelc.csMatrix2_Col1,args)
    def Col2(*args): return apply(_blcelc.csMatrix2_Col2,args)
    def Set(*args): return apply(_blcelc.csMatrix2_Set,args)
    def __iadd__(*args): return apply(_blcelc.csMatrix2___iadd__,args)
    def __isub__(*args): return apply(_blcelc.csMatrix2___isub__,args)
    def __imul__(*args): return apply(_blcelc.csMatrix2___imul__,args)
    def __idiv__(*args): return apply(_blcelc.csMatrix2___idiv__,args)
    def __pos__(*args): return apply(_blcelc.csMatrix2___pos__,args)
    def __neg__(*args): return apply(_blcelc.csMatrix2___neg__,args)
    def Transpose(*args): return apply(_blcelc.csMatrix2_Transpose,args)
    def GetTranspose(*args): return apply(_blcelc.csMatrix2_GetTranspose,args)
    def GetInverse(*args): return apply(_blcelc.csMatrix2_GetInverse,args)
    def Invert(*args): return apply(_blcelc.csMatrix2_Invert,args)
    def Determinant(*args): return apply(_blcelc.csMatrix2_Determinant,args)
    def Identity(*args): return apply(_blcelc.csMatrix2_Identity,args)
    def __del__(self, destroy= _blcelc.delete_csMatrix2):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csMatrix2 instance at %s>" % (self.this,)

class csMatrix2Ptr(csMatrix2):
    def __init__(self,this):
        _swig_setattr(self, csMatrix2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csMatrix2, 'thisown', 0)
        _swig_setattr(self, csMatrix2,self.__class__,csMatrix2)
_blcelc.csMatrix2_swigregister(csMatrix2Ptr)

class csMatrix3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csMatrix3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csMatrix3, name)
    __swig_setmethods__["m11"] = _blcelc.csMatrix3_m11_set
    __swig_getmethods__["m11"] = _blcelc.csMatrix3_m11_get
    if _newclass:m11 = property(_blcelc.csMatrix3_m11_get,_blcelc.csMatrix3_m11_set)
    __swig_setmethods__["m12"] = _blcelc.csMatrix3_m12_set
    __swig_getmethods__["m12"] = _blcelc.csMatrix3_m12_get
    if _newclass:m12 = property(_blcelc.csMatrix3_m12_get,_blcelc.csMatrix3_m12_set)
    __swig_setmethods__["m13"] = _blcelc.csMatrix3_m13_set
    __swig_getmethods__["m13"] = _blcelc.csMatrix3_m13_get
    if _newclass:m13 = property(_blcelc.csMatrix3_m13_get,_blcelc.csMatrix3_m13_set)
    __swig_setmethods__["m21"] = _blcelc.csMatrix3_m21_set
    __swig_getmethods__["m21"] = _blcelc.csMatrix3_m21_get
    if _newclass:m21 = property(_blcelc.csMatrix3_m21_get,_blcelc.csMatrix3_m21_set)
    __swig_setmethods__["m22"] = _blcelc.csMatrix3_m22_set
    __swig_getmethods__["m22"] = _blcelc.csMatrix3_m22_get
    if _newclass:m22 = property(_blcelc.csMatrix3_m22_get,_blcelc.csMatrix3_m22_set)
    __swig_setmethods__["m23"] = _blcelc.csMatrix3_m23_set
    __swig_getmethods__["m23"] = _blcelc.csMatrix3_m23_get
    if _newclass:m23 = property(_blcelc.csMatrix3_m23_get,_blcelc.csMatrix3_m23_set)
    __swig_setmethods__["m31"] = _blcelc.csMatrix3_m31_set
    __swig_getmethods__["m31"] = _blcelc.csMatrix3_m31_get
    if _newclass:m31 = property(_blcelc.csMatrix3_m31_get,_blcelc.csMatrix3_m31_set)
    __swig_setmethods__["m32"] = _blcelc.csMatrix3_m32_set
    __swig_getmethods__["m32"] = _blcelc.csMatrix3_m32_get
    if _newclass:m32 = property(_blcelc.csMatrix3_m32_get,_blcelc.csMatrix3_m32_set)
    __swig_setmethods__["m33"] = _blcelc.csMatrix3_m33_set
    __swig_getmethods__["m33"] = _blcelc.csMatrix3_m33_get
    if _newclass:m33 = property(_blcelc.csMatrix3_m33_get,_blcelc.csMatrix3_m33_set)
    def __init__(self,*args):
        _swig_setattr(self, csMatrix3, 'this', apply(_blcelc.new_csMatrix3,args))
        _swig_setattr(self, csMatrix3, 'thisown', 1)
    def Row1(*args): return apply(_blcelc.csMatrix3_Row1,args)
    def Row2(*args): return apply(_blcelc.csMatrix3_Row2,args)
    def Row3(*args): return apply(_blcelc.csMatrix3_Row3,args)
    def Col1(*args): return apply(_blcelc.csMatrix3_Col1,args)
    def Col2(*args): return apply(_blcelc.csMatrix3_Col2,args)
    def Col3(*args): return apply(_blcelc.csMatrix3_Col3,args)
    def Set(*args): return apply(_blcelc.csMatrix3_Set,args)
    def __iadd__(*args): return apply(_blcelc.csMatrix3___iadd__,args)
    def __isub__(*args): return apply(_blcelc.csMatrix3___isub__,args)
    def __idiv__(*args): return apply(_blcelc.csMatrix3___idiv__,args)
    def __pos__(*args): return apply(_blcelc.csMatrix3___pos__,args)
    def __neg__(*args): return apply(_blcelc.csMatrix3___neg__,args)
    def Transpose(*args): return apply(_blcelc.csMatrix3_Transpose,args)
    def GetTranspose(*args): return apply(_blcelc.csMatrix3_GetTranspose,args)
    def GetInverse(*args): return apply(_blcelc.csMatrix3_GetInverse,args)
    def Invert(*args): return apply(_blcelc.csMatrix3_Invert,args)
    def Determinant(*args): return apply(_blcelc.csMatrix3_Determinant,args)
    def Identity(*args): return apply(_blcelc.csMatrix3_Identity,args)
    def IsIdentity(*args): return apply(_blcelc.csMatrix3_IsIdentity,args)
    def __add__(*args): return apply(_blcelc.csMatrix3___add__,args)
    def __sub__(*args): return apply(_blcelc.csMatrix3___sub__,args)
    def __div__(*args): return apply(_blcelc.csMatrix3___div__,args)
    def __eq__(*args): return apply(_blcelc.csMatrix3___eq__,args)
    def __ne__(*args): return apply(_blcelc.csMatrix3___ne__,args)
    def __lt__(*args): return apply(_blcelc.csMatrix3___lt__,args)
    def __mul__(*args): return apply(_blcelc.csMatrix3___mul__,args)
    def __imul__(*args): return apply(_blcelc.csMatrix3___imul__,args)
    def __rmul__(*args): return apply(_blcelc.csMatrix3___rmul__,args)
    def __del__(self, destroy= _blcelc.delete_csMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csMatrix3 instance at %s>" % (self.this,)

class csMatrix3Ptr(csMatrix3):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csXRotMatrix3, 'this', apply(_blcelc.new_csXRotMatrix3,args))
        _swig_setattr(self, csXRotMatrix3, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csXRotMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csXRotMatrix3 instance at %s>" % (self.this,)

class csXRotMatrix3Ptr(csXRotMatrix3):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csYRotMatrix3, 'this', apply(_blcelc.new_csYRotMatrix3,args))
        _swig_setattr(self, csYRotMatrix3, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csYRotMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csYRotMatrix3 instance at %s>" % (self.this,)

class csYRotMatrix3Ptr(csYRotMatrix3):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csZRotMatrix3, 'this', apply(_blcelc.new_csZRotMatrix3,args))
        _swig_setattr(self, csZRotMatrix3, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csZRotMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csZRotMatrix3 instance at %s>" % (self.this,)

class csZRotMatrix3Ptr(csZRotMatrix3):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csXScaleMatrix3, 'this', apply(_blcelc.new_csXScaleMatrix3,args))
        _swig_setattr(self, csXScaleMatrix3, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csXScaleMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csXScaleMatrix3 instance at %s>" % (self.this,)

class csXScaleMatrix3Ptr(csXScaleMatrix3):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csYScaleMatrix3, 'this', apply(_blcelc.new_csYScaleMatrix3,args))
        _swig_setattr(self, csYScaleMatrix3, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csYScaleMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csYScaleMatrix3 instance at %s>" % (self.this,)

class csYScaleMatrix3Ptr(csYScaleMatrix3):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csZScaleMatrix3, 'this', apply(_blcelc.new_csZScaleMatrix3,args))
        _swig_setattr(self, csZScaleMatrix3, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csZScaleMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csZScaleMatrix3 instance at %s>" % (self.this,)

class csZScaleMatrix3Ptr(csZScaleMatrix3):
    def __init__(self,this):
        _swig_setattr(self, csZScaleMatrix3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csZScaleMatrix3, 'thisown', 0)
        _swig_setattr(self, csZScaleMatrix3,self.__class__,csZScaleMatrix3)
_blcelc.csZScaleMatrix3_swigregister(csZScaleMatrix3Ptr)

class csTransform(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csTransform, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csTransform, name)
    def __init__(self,*args):
        _swig_setattr(self, csTransform, 'this', apply(_blcelc.new_csTransform,args))
        _swig_setattr(self, csTransform, 'thisown', 1)
    def Identity(*args): return apply(_blcelc.csTransform_Identity,args)
    def IsIdentity(*args): return apply(_blcelc.csTransform_IsIdentity,args)
    def GetO2T(*args): return apply(_blcelc.csTransform_GetO2T,args)
    def GetO2TTranslation(*args): return apply(_blcelc.csTransform_GetO2TTranslation,args)
    def GetOrigin(*args): return apply(_blcelc.csTransform_GetOrigin,args)
    def SetO2T(*args): return apply(_blcelc.csTransform_SetO2T,args)
    def SetO2TTranslation(*args): return apply(_blcelc.csTransform_SetO2TTranslation,args)
    def SetOrigin(*args): return apply(_blcelc.csTransform_SetOrigin,args)
    def Translate(*args): return apply(_blcelc.csTransform_Translate,args)
    def Other2ThisRelative(*args): return apply(_blcelc.csTransform_Other2ThisRelative,args)
    def Other2This(*args): return apply(_blcelc.csTransform_Other2This,args)
    __swig_getmethods__["GetReflect"] = lambda x: _blcelc.csTransform_GetReflect
    if _newclass:GetReflect = staticmethod(_blcelc.csTransform_GetReflect)
    def __mul__(*args): return apply(_blcelc.csTransform___mul__,args)
    def __rmul__(*args): return apply(_blcelc.csTransform___rmul__,args)
    def __del__(self, destroy= _blcelc.delete_csTransform):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csTransform instance at %s>" % (self.this,)

class csTransformPtr(csTransform):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csReversibleTransform, 'this', apply(_blcelc.new_csReversibleTransform,args))
        _swig_setattr(self, csReversibleTransform, 'thisown', 1)
    def GetT2O(*args): return apply(_blcelc.csReversibleTransform_GetT2O,args)
    def GetT2OTranslation(*args): return apply(_blcelc.csReversibleTransform_GetT2OTranslation,args)
    def GetInverse(*args): return apply(_blcelc.csReversibleTransform_GetInverse,args)
    def SetO2T(*args): return apply(_blcelc.csReversibleTransform_SetO2T,args)
    def SetT2O(*args): return apply(_blcelc.csReversibleTransform_SetT2O,args)
    def This2OtherRelative(*args): return apply(_blcelc.csReversibleTransform_This2OtherRelative,args)
    def This2Other(*args): return apply(_blcelc.csReversibleTransform_This2Other,args)
    def RotateOther(*args): return apply(_blcelc.csReversibleTransform_RotateOther,args)
    def RotateThis(*args): return apply(_blcelc.csReversibleTransform_RotateThis,args)
    def LookAt(*args): return apply(_blcelc.csReversibleTransform_LookAt,args)
    def __imul__(*args): return apply(_blcelc.csReversibleTransform___imul__,args)
    def __mul__(*args): return apply(_blcelc.csReversibleTransform___mul__,args)
    def __idiv__(*args): return apply(_blcelc.csReversibleTransform___idiv__,args)
    def __div__(*args): return apply(_blcelc.csReversibleTransform___div__,args)
    def __del__(self, destroy= _blcelc.delete_csReversibleTransform):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csReversibleTransform instance at %s>" % (self.this,)

class csReversibleTransformPtr(csReversibleTransform):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csOrthoTransform, 'this', apply(_blcelc.new_csOrthoTransform,args))
        _swig_setattr(self, csOrthoTransform, 'thisown', 1)
    def SetO2T(*args): return apply(_blcelc.csOrthoTransform_SetO2T,args)
    def SetT2O(*args): return apply(_blcelc.csOrthoTransform_SetT2O,args)
    def __del__(self, destroy= _blcelc.delete_csOrthoTransform):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csOrthoTransform instance at %s>" % (self.this,)

class csOrthoTransformPtr(csOrthoTransform):
    def __init__(self,this):
        _swig_setattr(self, csOrthoTransform, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csOrthoTransform, 'thisown', 0)
        _swig_setattr(self, csOrthoTransform,self.__class__,csOrthoTransform)
_blcelc.csOrthoTransform_swigregister(csOrthoTransformPtr)

class csSphere(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSphere, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSphere, name)
    def __init__(self,*args):
        _swig_setattr(self, csSphere, 'this', apply(_blcelc.new_csSphere,args))
        _swig_setattr(self, csSphere, 'thisown', 1)
    def GetCenter(*args): return apply(_blcelc.csSphere_GetCenter,args)
    def SetCenter(*args): return apply(_blcelc.csSphere_SetCenter,args)
    def GetRadius(*args): return apply(_blcelc.csSphere_GetRadius,args)
    def SetRadius(*args): return apply(_blcelc.csSphere_SetRadius,args)
    def Union(*args): return apply(_blcelc.csSphere_Union,args)
    def __iadd__(*args): return apply(_blcelc.csSphere___iadd__,args)
    def __imul__(*args): return apply(_blcelc.csSphere___imul__,args)
    def __div__(*args): return apply(_blcelc.csSphere___div__,args)
    def __del__(self, destroy= _blcelc.delete_csSphere):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csSphere instance at %s>" % (self.this,)

class csSpherePtr(csSphere):
    def __init__(self,this):
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
    __swig_setmethods__["norm"] = _blcelc.csPlane2_norm_set
    __swig_getmethods__["norm"] = _blcelc.csPlane2_norm_get
    if _newclass:norm = property(_blcelc.csPlane2_norm_get,_blcelc.csPlane2_norm_set)
    __swig_setmethods__["CC"] = _blcelc.csPlane2_CC_set
    __swig_getmethods__["CC"] = _blcelc.csPlane2_CC_get
    if _newclass:CC = property(_blcelc.csPlane2_CC_get,_blcelc.csPlane2_CC_set)
    def __init__(self,*args):
        _swig_setattr(self, csPlane2, 'this', apply(_blcelc.new_csPlane2,args))
        _swig_setattr(self, csPlane2, 'thisown', 1)
    def Normal(*args): return apply(_blcelc.csPlane2_Normal,args)
    def GetNormal(*args): return apply(_blcelc.csPlane2_GetNormal,args)
    def A(*args): return apply(_blcelc.csPlane2_A,args)
    def B(*args): return apply(_blcelc.csPlane2_B,args)
    def C(*args): return apply(_blcelc.csPlane2_C,args)
    def Set(*args): return apply(_blcelc.csPlane2_Set,args)
    __swig_getmethods__["Classify"] = lambda x: _blcelc.csPlane2_Classify
    if _newclass:Classify = staticmethod(_blcelc.csPlane2_Classify)
    def Distance(*args): return apply(_blcelc.csPlane2_Distance,args)
    def SquaredDistance(*args): return apply(_blcelc.csPlane2_SquaredDistance,args)
    def Invert(*args): return apply(_blcelc.csPlane2_Invert,args)
    def Normalize(*args): return apply(_blcelc.csPlane2_Normalize,args)
    def __del__(self, destroy= _blcelc.delete_csPlane2):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csPlane2 instance at %s>" % (self.this,)

class csPlane2Ptr(csPlane2):
    def __init__(self,this):
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
    __swig_setmethods__["norm"] = _blcelc.csPlane3_norm_set
    __swig_getmethods__["norm"] = _blcelc.csPlane3_norm_get
    if _newclass:norm = property(_blcelc.csPlane3_norm_get,_blcelc.csPlane3_norm_set)
    __swig_setmethods__["DD"] = _blcelc.csPlane3_DD_set
    __swig_getmethods__["DD"] = _blcelc.csPlane3_DD_get
    if _newclass:DD = property(_blcelc.csPlane3_DD_get,_blcelc.csPlane3_DD_set)
    def __init__(self,*args):
        _swig_setattr(self, csPlane3, 'this', apply(_blcelc.new_csPlane3,args))
        _swig_setattr(self, csPlane3, 'thisown', 1)
    def Normal(*args): return apply(_blcelc.csPlane3_Normal,args)
    def A(*args): return apply(_blcelc.csPlane3_A,args)
    def B(*args): return apply(_blcelc.csPlane3_B,args)
    def C(*args): return apply(_blcelc.csPlane3_C,args)
    def D(*args): return apply(_blcelc.csPlane3_D,args)
    def Set(*args): return apply(_blcelc.csPlane3_Set,args)
    __swig_getmethods__["Classify"] = lambda x: _blcelc.csPlane3_Classify
    if _newclass:Classify = staticmethod(_blcelc.csPlane3_Classify)
    def Distance(*args): return apply(_blcelc.csPlane3_Distance,args)
    def Invert(*args): return apply(_blcelc.csPlane3_Invert,args)
    def Normalize(*args): return apply(_blcelc.csPlane3_Normalize,args)
    def ClipPolygon(*args): return apply(_blcelc.csPlane3_ClipPolygon,args)
    def __imul__(*args): return apply(_blcelc.csPlane3___imul__,args)
    def __idiv__(*args): return apply(_blcelc.csPlane3___idiv__,args)
    def __div__(*args): return apply(_blcelc.csPlane3___div__,args)
    def __del__(self, destroy= _blcelc.delete_csPlane3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csPlane3 instance at %s>" % (self.this,)

class csPlane3Ptr(csPlane3):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csMath2, 'this', apply(_blcelc.new_csMath2,args))
        _swig_setattr(self, csMath2, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csMath2):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csMath2 instance at %s>" % (self.this,)

class csMath2Ptr(csMath2):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csIntersect2, 'this', apply(_blcelc.new_csIntersect2,args))
        _swig_setattr(self, csIntersect2, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csIntersect2):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csIntersect2 instance at %s>" % (self.this,)

class csIntersect2Ptr(csIntersect2):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csPoly2D, 'this', apply(_blcelc.new_csPoly2D,args))
        _swig_setattr(self, csPoly2D, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csPoly2D):
        try:
            if self.thisown: destroy(self)
        except: pass
    def assign(*args): return apply(_blcelc.csPoly2D_assign,args)
    def MakeEmpty(*args): return apply(_blcelc.csPoly2D_MakeEmpty,args)
    def GetVertexCount(*args): return apply(_blcelc.csPoly2D_GetVertexCount,args)
    def GetVertices(*args): return apply(_blcelc.csPoly2D_GetVertices,args)
    def GetVertex(*args): return apply(_blcelc.csPoly2D_GetVertex,args)
    def GetFirst(*args): return apply(_blcelc.csPoly2D_GetFirst,args)
    def GetLast(*args): return apply(_blcelc.csPoly2D_GetLast,args)
    __swig_getmethods__["In"] = lambda x: _blcelc.csPoly2D_In
    if _newclass:In = staticmethod(_blcelc.csPoly2D_In)
    def MakeRoom(*args): return apply(_blcelc.csPoly2D_MakeRoom,args)
    def SetVertexCount(*args): return apply(_blcelc.csPoly2D_SetVertexCount,args)
    def AddVertex(*args): return apply(_blcelc.csPoly2D_AddVertex,args)
    def SetVertices(*args): return apply(_blcelc.csPoly2D_SetVertices,args)
    def UpdateBoundingBox(*args): return apply(_blcelc.csPoly2D_UpdateBoundingBox,args)
    def GetBoundingBox(*args): return apply(_blcelc.csPoly2D_GetBoundingBox,args)
    def ClipAgainst(*args): return apply(_blcelc.csPoly2D_ClipAgainst,args)
    def Intersect(*args): return apply(_blcelc.csPoly2D_Intersect,args)
    def ClipPlane(*args): return apply(_blcelc.csPoly2D_ClipPlane,args)
    def ExtendConvex(*args): return apply(_blcelc.csPoly2D_ExtendConvex,args)
    def GetSignedArea(*args): return apply(_blcelc.csPoly2D_GetSignedArea,args)
    def Random(*args): return apply(_blcelc.csPoly2D_Random,args)
    def __getitem__(*args): return apply(_blcelc.csPoly2D___getitem__,args)
    def __setitem__ (self, i, v):
    	own_v = self.__getitem__(i)
    	for i in range(2):
    		own_v[i] = v[i]


    def __repr__(self):
        return "<C csPoly2D instance at %s>" % (self.this,)

class csPoly2DPtr(csPoly2D):
    def __init__(self,this):
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
    def Create(*args): return apply(_blcelc.csPoly2DFactory_Create,args)
    def __init__(self,*args):
        _swig_setattr(self, csPoly2DFactory, 'this', apply(_blcelc.new_csPoly2DFactory,args))
        _swig_setattr(self, csPoly2DFactory, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csPoly2DFactory):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csPoly2DFactory instance at %s>" % (self.this,)

class csPoly2DFactoryPtr(csPoly2DFactory):
    def __init__(self,this):
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
    __swig_getmethods__["Area3"] = lambda x: _blcelc.csMath3_Area3
    if _newclass:Area3 = staticmethod(_blcelc.csMath3_Area3)
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
    def __init__(self,*args):
        _swig_setattr(self, csMath3, 'this', apply(_blcelc.new_csMath3,args))
        _swig_setattr(self, csMath3, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csMath3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csMath3 instance at %s>" % (self.this,)

class csMath3Ptr(csMath3):
    def __init__(self,this):
        _swig_setattr(self, csMath3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csMath3, 'thisown', 0)
        _swig_setattr(self, csMath3,self.__class__,csMath3)
_blcelc.csMath3_swigregister(csMath3Ptr)
csMath3_WhichSide3D = _blcelc.csMath3_WhichSide3D

csMath3_Visible = _blcelc.csMath3_Visible

csMath3_FindIntersection = _blcelc.csMath3_FindIntersection

csMath3_Between = _blcelc.csMath3_Between

csMath3_SetMinMax = _blcelc.csMath3_SetMinMax

csMath3_Area3 = _blcelc.csMath3_Area3

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
    __swig_getmethods__["PointPoint"] = lambda x: _blcelc.csSquaredDist_PointPoint
    if _newclass:PointPoint = staticmethod(_blcelc.csSquaredDist_PointPoint)
    __swig_getmethods__["PointLine"] = lambda x: _blcelc.csSquaredDist_PointLine
    if _newclass:PointLine = staticmethod(_blcelc.csSquaredDist_PointLine)
    __swig_getmethods__["PointPlane"] = lambda x: _blcelc.csSquaredDist_PointPlane
    if _newclass:PointPlane = staticmethod(_blcelc.csSquaredDist_PointPlane)
    __swig_getmethods__["PointPoly"] = lambda x: _blcelc.csSquaredDist_PointPoly
    if _newclass:PointPoly = staticmethod(_blcelc.csSquaredDist_PointPoly)
    def __init__(self,*args):
        _swig_setattr(self, csSquaredDist, 'this', apply(_blcelc.new_csSquaredDist,args))
        _swig_setattr(self, csSquaredDist, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csSquaredDist):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csSquaredDist instance at %s>" % (self.this,)

class csSquaredDistPtr(csSquaredDist):
    def __init__(self,this):
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
    __swig_getmethods__["IntersectPolygon"] = lambda x: _blcelc.csIntersect3_IntersectPolygon
    if _newclass:IntersectPolygon = staticmethod(_blcelc.csIntersect3_IntersectPolygon)
    __swig_getmethods__["IntersectSegment"] = lambda x: _blcelc.csIntersect3_IntersectSegment
    if _newclass:IntersectSegment = staticmethod(_blcelc.csIntersect3_IntersectSegment)
    __swig_getmethods__["IntersectTriangle"] = lambda x: _blcelc.csIntersect3_IntersectTriangle
    if _newclass:IntersectTriangle = staticmethod(_blcelc.csIntersect3_IntersectTriangle)
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
    __swig_getmethods__["ZPlane"] = lambda x: _blcelc.csIntersect3_ZPlane
    if _newclass:ZPlane = staticmethod(_blcelc.csIntersect3_ZPlane)
    __swig_getmethods__["ZPlane"] = lambda x: _blcelc.csIntersect3_ZPlane
    if _newclass:ZPlane = staticmethod(_blcelc.csIntersect3_ZPlane)
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
    def __init__(self,*args):
        _swig_setattr(self, csIntersect3, 'this', apply(_blcelc.new_csIntersect3,args))
        _swig_setattr(self, csIntersect3, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csIntersect3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csIntersect3 instance at %s>" % (self.this,)

class csIntersect3Ptr(csIntersect3):
    def __init__(self,this):
        _swig_setattr(self, csIntersect3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csIntersect3, 'thisown', 0)
        _swig_setattr(self, csIntersect3,self.__class__,csIntersect3)
_blcelc.csIntersect3_swigregister(csIntersect3Ptr)
csIntersect3_IntersectPolygon = _blcelc.csIntersect3_IntersectPolygon

csIntersect3_IntersectSegment = _blcelc.csIntersect3_IntersectSegment

csIntersect3_IntersectTriangle = _blcelc.csIntersect3_IntersectTriangle

csIntersect3_Plane = _blcelc.csIntersect3_Plane

csIntersect3_Planes = _blcelc.csIntersect3_Planes

csIntersect3_PlaneXPlane = _blcelc.csIntersect3_PlaneXPlane

csIntersect3_PlaneYPlane = _blcelc.csIntersect3_PlaneYPlane

csIntersect3_PlaneZPlane = _blcelc.csIntersect3_PlaneZPlane

csIntersect3_PlaneAxisPlane = _blcelc.csIntersect3_PlaneAxisPlane

csIntersect3_Z0Plane = _blcelc.csIntersect3_Z0Plane

csIntersect3_ZPlane = _blcelc.csIntersect3_ZPlane

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
    def __init__(self,*args):
        _swig_setattr(self, csGeomDebugHelper, 'this', apply(_blcelc.new_csGeomDebugHelper,args))
        _swig_setattr(self, csGeomDebugHelper, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csGeomDebugHelper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_setmethods__["scfRefCount"] = _blcelc.csGeomDebugHelper_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csGeomDebugHelper_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csGeomDebugHelper_scfRefCount_get,_blcelc.csGeomDebugHelper_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csGeomDebugHelper_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csGeomDebugHelper_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csGeomDebugHelper_scfWeakRefOwners_get,_blcelc.csGeomDebugHelper_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return apply(_blcelc.csGeomDebugHelper_scfRemoveRefOwners,args)
    __swig_setmethods__["scfParent"] = _blcelc.csGeomDebugHelper_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csGeomDebugHelper_scfParent_get
    if _newclass:scfParent = property(_blcelc.csGeomDebugHelper_scfParent_get,_blcelc.csGeomDebugHelper_scfParent_set)
    def IncRef(*args): return apply(_blcelc.csGeomDebugHelper_IncRef,args)
    def DecRef(*args): return apply(_blcelc.csGeomDebugHelper_DecRef,args)
    def GetRefCount(*args): return apply(_blcelc.csGeomDebugHelper_GetRefCount,args)
    def AddRefOwner(*args): return apply(_blcelc.csGeomDebugHelper_AddRefOwner,args)
    def RemoveRefOwner(*args): return apply(_blcelc.csGeomDebugHelper_RemoveRefOwner,args)
    def QueryInterface(*args): return apply(_blcelc.csGeomDebugHelper_QueryInterface,args)
    def GetSupportedTests(*args): return apply(_blcelc.csGeomDebugHelper_GetSupportedTests,args)
    def UnitTest(*args): return apply(_blcelc.csGeomDebugHelper_UnitTest,args)
    def StateTest(*args): return apply(_blcelc.csGeomDebugHelper_StateTest,args)
    def Benchmark(*args): return apply(_blcelc.csGeomDebugHelper_Benchmark,args)
    def Dump(*args): return apply(_blcelc.csGeomDebugHelper_Dump,args)
    def DebugCommand(*args): return apply(_blcelc.csGeomDebugHelper_DebugCommand,args)
    def __repr__(self):
        return "<C csGeomDebugHelper instance at %s>" % (self.this,)

class csGeomDebugHelperPtr(csGeomDebugHelper):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csPoly3D, 'this', apply(_blcelc.new_csPoly3D,args))
        _swig_setattr(self, csPoly3D, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csPoly3D):
        try:
            if self.thisown: destroy(self)
        except: pass
    def MakeEmpty(*args): return apply(_blcelc.csPoly3D_MakeEmpty,args)
    def GetVertexCount(*args): return apply(_blcelc.csPoly3D_GetVertexCount,args)
    def GetVertices(*args): return apply(_blcelc.csPoly3D_GetVertices,args)
    def GetVertex(*args): return apply(_blcelc.csPoly3D_GetVertex,args)
    def GetFirst(*args): return apply(_blcelc.csPoly3D_GetFirst,args)
    def GetLast(*args): return apply(_blcelc.csPoly3D_GetLast,args)
    __swig_getmethods__["In"] = lambda x: _blcelc.csPoly3D_In
    if _newclass:In = staticmethod(_blcelc.csPoly3D_In)
    def MakeRoom(*args): return apply(_blcelc.csPoly3D_MakeRoom,args)
    def SetVertexCount(*args): return apply(_blcelc.csPoly3D_SetVertexCount,args)
    def AddVertex(*args): return apply(_blcelc.csPoly3D_AddVertex,args)
    def SetVertices(*args): return apply(_blcelc.csPoly3D_SetVertices,args)
    def ProjectXPlane(*args): return apply(_blcelc.csPoly3D_ProjectXPlane,args)
    def ProjectYPlane(*args): return apply(_blcelc.csPoly3D_ProjectYPlane,args)
    def ProjectZPlane(*args): return apply(_blcelc.csPoly3D_ProjectZPlane,args)
    def ProjectAxisPlane(*args): return apply(_blcelc.csPoly3D_ProjectAxisPlane,args)
    __swig_getmethods__["Classify"] = lambda x: _blcelc.csPoly3D_Classify
    if _newclass:Classify = staticmethod(_blcelc.csPoly3D_Classify)
    def Classify(*args): return apply(_blcelc.csPoly3D_Classify,args)
    def ClassifyX(*args): return apply(_blcelc.csPoly3D_ClassifyX,args)
    def ClassifyY(*args): return apply(_blcelc.csPoly3D_ClassifyY,args)
    def ClassifyZ(*args): return apply(_blcelc.csPoly3D_ClassifyZ,args)
    def CutToPlane(*args): return apply(_blcelc.csPoly3D_CutToPlane,args)
    def SplitWithPlane(*args): return apply(_blcelc.csPoly3D_SplitWithPlane,args)
    def SplitWithPlaneX(*args): return apply(_blcelc.csPoly3D_SplitWithPlaneX,args)
    def SplitWithPlaneY(*args): return apply(_blcelc.csPoly3D_SplitWithPlaneY,args)
    def SplitWithPlaneZ(*args): return apply(_blcelc.csPoly3D_SplitWithPlaneZ,args)
    __swig_getmethods__["ComputeNormal"] = lambda x: _blcelc.csPoly3D_ComputeNormal
    if _newclass:ComputeNormal = staticmethod(_blcelc.csPoly3D_ComputeNormal)
    def ComputeNormal(*args): return apply(_blcelc.csPoly3D_ComputeNormal,args)
    __swig_getmethods__["ComputePlane"] = lambda x: _blcelc.csPoly3D_ComputePlane
    if _newclass:ComputePlane = staticmethod(_blcelc.csPoly3D_ComputePlane)
    def ComputePlane(*args): return apply(_blcelc.csPoly3D_ComputePlane,args)
    def GetSignedArea(*args): return apply(_blcelc.csPoly3D_GetSignedArea,args)
    def GetCenter(*args): return apply(_blcelc.csPoly3D_GetCenter,args)
    def __getitem__(*args): return apply(_blcelc.csPoly3D___getitem__,args)
    def __setitem__ (self, i, v):
    	own_v = self.__getitem__(i)
    	for i in range(3):
    		own_v[i] = v[i]


    def __repr__(self):
        return "<C csPoly3D instance at %s>" % (self.this,)

class csPoly3DPtr(csPoly3D):
    def __init__(self,this):
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
    __swig_setmethods__["orig_idx"] = _blcelc.csCompressVertex_orig_idx_set
    __swig_getmethods__["orig_idx"] = _blcelc.csCompressVertex_orig_idx_get
    if _newclass:orig_idx = property(_blcelc.csCompressVertex_orig_idx_get,_blcelc.csCompressVertex_orig_idx_set)
    __swig_setmethods__["x"] = _blcelc.csCompressVertex_x_set
    __swig_getmethods__["x"] = _blcelc.csCompressVertex_x_get
    if _newclass:x = property(_blcelc.csCompressVertex_x_get,_blcelc.csCompressVertex_x_set)
    __swig_setmethods__["y"] = _blcelc.csCompressVertex_y_set
    __swig_getmethods__["y"] = _blcelc.csCompressVertex_y_get
    if _newclass:y = property(_blcelc.csCompressVertex_y_get,_blcelc.csCompressVertex_y_set)
    __swig_setmethods__["z"] = _blcelc.csCompressVertex_z_set
    __swig_getmethods__["z"] = _blcelc.csCompressVertex_z_get
    if _newclass:z = property(_blcelc.csCompressVertex_z_get,_blcelc.csCompressVertex_z_set)
    __swig_setmethods__["new_idx"] = _blcelc.csCompressVertex_new_idx_set
    __swig_getmethods__["new_idx"] = _blcelc.csCompressVertex_new_idx_get
    if _newclass:new_idx = property(_blcelc.csCompressVertex_new_idx_get,_blcelc.csCompressVertex_new_idx_set)
    __swig_setmethods__["used"] = _blcelc.csCompressVertex_used_set
    __swig_getmethods__["used"] = _blcelc.csCompressVertex_used_get
    if _newclass:used = property(_blcelc.csCompressVertex_used_get,_blcelc.csCompressVertex_used_set)
    def __init__(self,*args):
        _swig_setattr(self, csCompressVertex, 'this', apply(_blcelc.new_csCompressVertex,args))
        _swig_setattr(self, csCompressVertex, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csCompressVertex):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csCompressVertex instance at %s>" % (self.this,)

class csCompressVertexPtr(csCompressVertex):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csVector3Array, 'this', apply(_blcelc.new_csVector3Array,args))
        _swig_setattr(self, csVector3Array, 'thisown', 1)
    def AddVertexSmart(*args): return apply(_blcelc.csVector3Array_AddVertexSmart,args)
    __swig_getmethods__["CompressVertices"] = lambda x: _blcelc.csVector3Array_CompressVertices
    if _newclass:CompressVertices = staticmethod(_blcelc.csVector3Array_CompressVertices)
    __swig_getmethods__["CompressVertices"] = lambda x: _blcelc.csVector3Array_CompressVertices
    if _newclass:CompressVertices = staticmethod(_blcelc.csVector3Array_CompressVertices)
    def __del__(self, destroy= _blcelc.delete_csVector3Array):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csVector3Array instance at %s>" % (self.this,)

class csVector3ArrayPtr(csVector3Array):
    def __init__(self,this):
        _swig_setattr(self, csVector3Array, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csVector3Array, 'thisown', 0)
        _swig_setattr(self, csVector3Array,self.__class__,csVector3Array)
_blcelc.csVector3Array_swigregister(csVector3ArrayPtr)
csVector3Array_CompressVertices = _blcelc.csVector3Array_CompressVertices


class csRect(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csRect, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csRect, name)
    __swig_setmethods__["xmin"] = _blcelc.csRect_xmin_set
    __swig_getmethods__["xmin"] = _blcelc.csRect_xmin_get
    if _newclass:xmin = property(_blcelc.csRect_xmin_get,_blcelc.csRect_xmin_set)
    __swig_setmethods__["ymin"] = _blcelc.csRect_ymin_set
    __swig_getmethods__["ymin"] = _blcelc.csRect_ymin_get
    if _newclass:ymin = property(_blcelc.csRect_ymin_get,_blcelc.csRect_ymin_set)
    __swig_setmethods__["xmax"] = _blcelc.csRect_xmax_set
    __swig_getmethods__["xmax"] = _blcelc.csRect_xmax_get
    if _newclass:xmax = property(_blcelc.csRect_xmax_get,_blcelc.csRect_xmax_set)
    __swig_setmethods__["ymax"] = _blcelc.csRect_ymax_set
    __swig_getmethods__["ymax"] = _blcelc.csRect_ymax_get
    if _newclass:ymax = property(_blcelc.csRect_ymax_get,_blcelc.csRect_ymax_set)
    def __init__(self,*args):
        _swig_setattr(self, csRect, 'this', apply(_blcelc.new_csRect,args))
        _swig_setattr(self, csRect, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csRect):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Intersect(*args): return apply(_blcelc.csRect_Intersect,args)
    def Intersects(*args): return apply(_blcelc.csRect_Intersects,args)
    def Union(*args): return apply(_blcelc.csRect_Union,args)
    def Exclude(*args): return apply(_blcelc.csRect_Exclude,args)
    def Subtract(*args): return apply(_blcelc.csRect_Subtract,args)
    def IsEmpty(*args): return apply(_blcelc.csRect_IsEmpty,args)
    def MakeEmpty(*args): return apply(_blcelc.csRect_MakeEmpty,args)
    def Set(*args): return apply(_blcelc.csRect_Set,args)
    def SetPos(*args): return apply(_blcelc.csRect_SetPos,args)
    def SetSize(*args): return apply(_blcelc.csRect_SetSize,args)
    def Move(*args): return apply(_blcelc.csRect_Move,args)
    def Width(*args): return apply(_blcelc.csRect_Width,args)
    def Height(*args): return apply(_blcelc.csRect_Height,args)
    def Contains(*args): return apply(_blcelc.csRect_Contains,args)
    def ContainsRel(*args): return apply(_blcelc.csRect_ContainsRel,args)
    def Equal(*args): return apply(_blcelc.csRect_Equal,args)
    def Normalize(*args): return apply(_blcelc.csRect_Normalize,args)
    def Area(*args): return apply(_blcelc.csRect_Area,args)
    def AddAdjanced(*args): return apply(_blcelc.csRect_AddAdjanced,args)
    def __eq__(*args): return apply(_blcelc.csRect___eq__,args)
    def __ne__(*args): return apply(_blcelc.csRect___ne__,args)
    def Extend(*args): return apply(_blcelc.csRect_Extend,args)
    def Join(*args): return apply(_blcelc.csRect_Join,args)
    def Outset(*args): return apply(_blcelc.csRect_Outset,args)
    def Inset(*args): return apply(_blcelc.csRect_Inset,args)
    def ClipLineGeneral(*args): return apply(_blcelc.csRect_ClipLineGeneral,args)
    def ClipLine(*args): return apply(_blcelc.csRect_ClipLine,args)
    def ClipLineSafe(*args): return apply(_blcelc.csRect_ClipLineSafe,args)
    def __repr__(self):
        return "<C csRect instance at %s>" % (self.this,)

class csRectPtr(csRect):
    def __init__(self,this):
        _swig_setattr(self, csRect, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csRect, 'thisown', 0)
        _swig_setattr(self, csRect,self.__class__,csRect)
_blcelc.csRect_swigregister(csRectPtr)

class csRectRegion(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csRectRegion, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csRectRegion, name)
    def __init__(self,*args):
        _swig_setattr(self, csRectRegion, 'this', apply(_blcelc.new_csRectRegion,args))
        _swig_setattr(self, csRectRegion, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csRectRegion):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Include(*args): return apply(_blcelc.csRectRegion_Include,args)
    def Exclude(*args): return apply(_blcelc.csRectRegion_Exclude,args)
    def ClipTo(*args): return apply(_blcelc.csRectRegion_ClipTo,args)
    def Count(*args): return apply(_blcelc.csRectRegion_Count,args)
    def RectAt(*args): return apply(_blcelc.csRectRegion_RectAt,args)
    def makeEmpty(*args): return apply(_blcelc.csRectRegion_makeEmpty,args)
    def __repr__(self):
        return "<C csRectRegion instance at %s>" % (self.this,)

class csRectRegionPtr(csRectRegion):
    def __init__(self,this):
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
    def Init(*args): return apply(_blcelc.csQuaternion_Init,args)
    def __init__(self,*args):
        _swig_setattr(self, csQuaternion, 'this', apply(_blcelc.new_csQuaternion,args))
        _swig_setattr(self, csQuaternion, 'thisown', 1)
    def __imul__(*args): return apply(_blcelc.csQuaternion___imul__,args)
    def Conjugate(*args): return apply(_blcelc.csQuaternion_Conjugate,args)
    def Negate(*args): return apply(_blcelc.csQuaternion_Negate,args)
    def Invert(*args): return apply(_blcelc.csQuaternion_Invert,args)
    def GetAxisAngle(*args): return apply(_blcelc.csQuaternion_GetAxisAngle,args)
    def SetWithAxisAngle(*args): return apply(_blcelc.csQuaternion_SetWithAxisAngle,args)
    def PrepRotation(*args): return apply(_blcelc.csQuaternion_PrepRotation,args)
    def Rotate(*args): return apply(_blcelc.csQuaternion_Rotate,args)
    def Normalize(*args): return apply(_blcelc.csQuaternion_Normalize,args)
    def SetWithEuler(*args): return apply(_blcelc.csQuaternion_SetWithEuler,args)
    def GetEulerAngles(*args): return apply(_blcelc.csQuaternion_GetEulerAngles,args)
    def ToAxisAngle(*args): return apply(_blcelc.csQuaternion_ToAxisAngle,args)
    def Slerp(*args): return apply(_blcelc.csQuaternion_Slerp,args)
    __swig_setmethods__["r"] = _blcelc.csQuaternion_r_set
    __swig_getmethods__["r"] = _blcelc.csQuaternion_r_get
    if _newclass:r = property(_blcelc.csQuaternion_r_get,_blcelc.csQuaternion_r_set)
    __swig_setmethods__["x"] = _blcelc.csQuaternion_x_set
    __swig_getmethods__["x"] = _blcelc.csQuaternion_x_get
    if _newclass:x = property(_blcelc.csQuaternion_x_get,_blcelc.csQuaternion_x_set)
    __swig_setmethods__["y"] = _blcelc.csQuaternion_y_set
    __swig_getmethods__["y"] = _blcelc.csQuaternion_y_get
    if _newclass:y = property(_blcelc.csQuaternion_y_get,_blcelc.csQuaternion_y_set)
    __swig_setmethods__["z"] = _blcelc.csQuaternion_z_set
    __swig_getmethods__["z"] = _blcelc.csQuaternion_z_get
    if _newclass:z = property(_blcelc.csQuaternion_z_get,_blcelc.csQuaternion_z_set)
    def __add__(*args): return apply(_blcelc.csQuaternion___add__,args)
    def __sub__(*args): return apply(_blcelc.csQuaternion___sub__,args)
    def __mul__(*args): return apply(_blcelc.csQuaternion___mul__,args)
    def __del__(self, destroy= _blcelc.delete_csQuaternion):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csQuaternion instance at %s>" % (self.this,)

class csQuaternionPtr(csQuaternion):
    def __init__(self,this):
        _swig_setattr(self, csQuaternion, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csQuaternion, 'thisown', 0)
        _swig_setattr(self, csQuaternion,self.__class__,csQuaternion)
_blcelc.csQuaternion_swigregister(csQuaternionPtr)

class csSpline(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSpline, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSpline, name)
    def __del__(self, destroy= _blcelc.delete_csSpline):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetDimensionCount(*args): return apply(_blcelc.csSpline_GetDimensionCount,args)
    def GetPointCount(*args): return apply(_blcelc.csSpline_GetPointCount,args)
    def InsertPoint(*args): return apply(_blcelc.csSpline_InsertPoint,args)
    def RemovePoint(*args): return apply(_blcelc.csSpline_RemovePoint,args)
    def SetTimeValues(*args): return apply(_blcelc.csSpline_SetTimeValues,args)
    def SetTimeValue(*args): return apply(_blcelc.csSpline_SetTimeValue,args)
    def GetTimeValues(*args): return apply(_blcelc.csSpline_GetTimeValues,args)
    def GetTimeValue(*args): return apply(_blcelc.csSpline_GetTimeValue,args)
    def SetDimensionValues(*args): return apply(_blcelc.csSpline_SetDimensionValues,args)
    def SetDimensionValue(*args): return apply(_blcelc.csSpline_SetDimensionValue,args)
    def GetDimensionValues(*args): return apply(_blcelc.csSpline_GetDimensionValues,args)
    def GetDimensionValue(*args): return apply(_blcelc.csSpline_GetDimensionValue,args)
    def Calculate(*args): return apply(_blcelc.csSpline_Calculate,args)
    def GetCurrentIndex(*args): return apply(_blcelc.csSpline_GetCurrentIndex,args)
    def GetInterpolatedDimension(*args): return apply(_blcelc.csSpline_GetInterpolatedDimension,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C csSpline instance at %s>" % (self.this,)

class csSplinePtr(csSpline):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csCubicSpline, 'this', apply(_blcelc.new_csCubicSpline,args))
        _swig_setattr(self, csCubicSpline, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csCubicSpline):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Calculate(*args): return apply(_blcelc.csCubicSpline_Calculate,args)
    def GetInterpolatedDimension(*args): return apply(_blcelc.csCubicSpline_GetInterpolatedDimension,args)
    def __repr__(self):
        return "<C csCubicSpline instance at %s>" % (self.this,)

class csCubicSplinePtr(csCubicSpline):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csBSpline, 'this', apply(_blcelc.new_csBSpline,args))
        _swig_setattr(self, csBSpline, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csBSpline):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Calculate(*args): return apply(_blcelc.csBSpline_Calculate,args)
    def GetInterpolatedDimension(*args): return apply(_blcelc.csBSpline_GetInterpolatedDimension,args)
    def __repr__(self):
        return "<C csBSpline instance at %s>" % (self.this,)

class csBSplinePtr(csBSpline):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csCatmullRomSpline, 'this', apply(_blcelc.new_csCatmullRomSpline,args))
        _swig_setattr(self, csCatmullRomSpline, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csCatmullRomSpline):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csCatmullRomSpline instance at %s>" % (self.this,)

class csCatmullRomSplinePtr(csCatmullRomSpline):
    def __init__(self,this):
        _swig_setattr(self, csCatmullRomSpline, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csCatmullRomSpline, 'thisown', 0)
        _swig_setattr(self, csCatmullRomSpline,self.__class__,csCatmullRomSpline)
_blcelc.csCatmullRomSpline_swigregister(csCatmullRomSplinePtr)

class csPoint(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPoint, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPoint, name)
    __swig_setmethods__["x"] = _blcelc.csPoint_x_set
    __swig_getmethods__["x"] = _blcelc.csPoint_x_get
    if _newclass:x = property(_blcelc.csPoint_x_get,_blcelc.csPoint_x_set)
    __swig_setmethods__["y"] = _blcelc.csPoint_y_set
    __swig_getmethods__["y"] = _blcelc.csPoint_y_get
    if _newclass:y = property(_blcelc.csPoint_y_get,_blcelc.csPoint_y_set)
    def __init__(self,*args):
        _swig_setattr(self, csPoint, 'this', apply(_blcelc.new_csPoint,args))
        _swig_setattr(self, csPoint, 'thisown', 1)
    def Set(*args): return apply(_blcelc.csPoint_Set,args)
    def __del__(self, destroy= _blcelc.delete_csPoint):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csPoint instance at %s>" % (self.this,)

class csPointPtr(csPoint):
    def __init__(self,this):
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
    def MinX(*args): return apply(_blcelc.csBox2_MinX,args)
    def MinY(*args): return apply(_blcelc.csBox2_MinY,args)
    def MaxX(*args): return apply(_blcelc.csBox2_MaxX,args)
    def MaxY(*args): return apply(_blcelc.csBox2_MaxY,args)
    def Min(*args): return apply(_blcelc.csBox2_Min,args)
    def Max(*args): return apply(_blcelc.csBox2_Max,args)
    def GetCorner(*args): return apply(_blcelc.csBox2_GetCorner,args)
    def GetCenter(*args): return apply(_blcelc.csBox2_GetCenter,args)
    def SetCenter(*args): return apply(_blcelc.csBox2_SetCenter,args)
    def SetSize(*args): return apply(_blcelc.csBox2_SetSize,args)
    def GetEdgeInfo(*args): return apply(_blcelc.csBox2_GetEdgeInfo,args)
    def GetEdge(*args): return apply(_blcelc.csBox2_GetEdge,args)
    __swig_getmethods__["Intersect"] = lambda x: _blcelc.csBox2_Intersect
    if _newclass:Intersect = staticmethod(_blcelc.csBox2_Intersect)
    __swig_getmethods__["Intersect"] = lambda x: _blcelc.csBox2_Intersect
    if _newclass:Intersect = staticmethod(_blcelc.csBox2_Intersect)
    def Intersect(*args): return apply(_blcelc.csBox2_Intersect,args)
    def In(*args): return apply(_blcelc.csBox2_In,args)
    def Overlap(*args): return apply(_blcelc.csBox2_Overlap,args)
    def Contains(*args): return apply(_blcelc.csBox2_Contains,args)
    def Empty(*args): return apply(_blcelc.csBox2_Empty,args)
    def SquaredOriginDist(*args): return apply(_blcelc.csBox2_SquaredOriginDist,args)
    def SquaredOriginMaxDist(*args): return apply(_blcelc.csBox2_SquaredOriginMaxDist,args)
    def StartBoundingBox(*args): return apply(_blcelc.csBox2_StartBoundingBox,args)
    def AddBoundingVertex(*args): return apply(_blcelc.csBox2_AddBoundingVertex,args)
    def AddBoundingVertexSmart(*args): return apply(_blcelc.csBox2_AddBoundingVertexSmart,args)
    def __init__(self,*args):
        _swig_setattr(self, csBox2, 'this', apply(_blcelc.new_csBox2,args))
        _swig_setattr(self, csBox2, 'thisown', 1)
    def Set(*args): return apply(_blcelc.csBox2_Set,args)
    def SetMin(*args): return apply(_blcelc.csBox2_SetMin,args)
    def SetMax(*args): return apply(_blcelc.csBox2_SetMax,args)
    def __iadd__(*args): return apply(_blcelc.csBox2___iadd__,args)
    def __imul__(*args): return apply(_blcelc.csBox2___imul__,args)
    def TestIntersect(*args): return apply(_blcelc.csBox2_TestIntersect,args)
    def __del__(self, destroy= _blcelc.delete_csBox2):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csBox2 instance at %s>" % (self.this,)

class csBox2Ptr(csBox2):
    def __init__(self,this):
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
    def MinX(*args): return apply(_blcelc.csBox3_MinX,args)
    def MinY(*args): return apply(_blcelc.csBox3_MinY,args)
    def MinZ(*args): return apply(_blcelc.csBox3_MinZ,args)
    def MaxX(*args): return apply(_blcelc.csBox3_MaxX,args)
    def MaxY(*args): return apply(_blcelc.csBox3_MaxY,args)
    def MaxZ(*args): return apply(_blcelc.csBox3_MaxZ,args)
    def Min(*args): return apply(_blcelc.csBox3_Min,args)
    def Max(*args): return apply(_blcelc.csBox3_Max,args)
    def GetCorner(*args): return apply(_blcelc.csBox3_GetCorner,args)
    def GetEdgeInfo(*args): return apply(_blcelc.csBox3_GetEdgeInfo,args)
    def GetFaceEdges(*args): return apply(_blcelc.csBox3_GetFaceEdges,args)
    def GetCenter(*args): return apply(_blcelc.csBox3_GetCenter,args)
    def SetCenter(*args): return apply(_blcelc.csBox3_SetCenter,args)
    def SetSize(*args): return apply(_blcelc.csBox3_SetSize,args)
    def GetSide(*args): return apply(_blcelc.csBox3_GetSide,args)
    def GetVisibleSides(*args): return apply(_blcelc.csBox3_GetVisibleSides,args)
    __swig_getmethods__["OtherSide"] = lambda x: _blcelc.csBox3_OtherSide
    if _newclass:OtherSide = staticmethod(_blcelc.csBox3_OtherSide)
    def GetEdge(*args): return apply(_blcelc.csBox3_GetEdge,args)
    def In(*args): return apply(_blcelc.csBox3_In,args)
    def Overlap(*args): return apply(_blcelc.csBox3_Overlap,args)
    def Contains(*args): return apply(_blcelc.csBox3_Contains,args)
    def Empty(*args): return apply(_blcelc.csBox3_Empty,args)
    def StartBoundingBox(*args): return apply(_blcelc.csBox3_StartBoundingBox,args)
    def AddBoundingVertex(*args): return apply(_blcelc.csBox3_AddBoundingVertex,args)
    def AddBoundingVertexSmart(*args): return apply(_blcelc.csBox3_AddBoundingVertexSmart,args)
    def __init__(self,*args):
        _swig_setattr(self, csBox3, 'this', apply(_blcelc.new_csBox3,args))
        _swig_setattr(self, csBox3, 'thisown', 1)
    def Set(*args): return apply(_blcelc.csBox3_Set,args)
    def SetMin(*args): return apply(_blcelc.csBox3_SetMin,args)
    def SetMax(*args): return apply(_blcelc.csBox3_SetMax,args)
    def AdjacentX(*args): return apply(_blcelc.csBox3_AdjacentX,args)
    def AdjacentY(*args): return apply(_blcelc.csBox3_AdjacentY,args)
    def AdjacentZ(*args): return apply(_blcelc.csBox3_AdjacentZ,args)
    def Adjacent(*args): return apply(_blcelc.csBox3_Adjacent,args)
    def CalculatePointSegment(*args): return apply(_blcelc.csBox3_CalculatePointSegment,args)
    def GetConvexOutline(*args): return apply(_blcelc.csBox3_GetConvexOutline,args)
    def Between(*args): return apply(_blcelc.csBox3_Between,args)
    def ManhattanDistance(*args): return apply(_blcelc.csBox3_ManhattanDistance,args)
    def SquaredOriginDist(*args): return apply(_blcelc.csBox3_SquaredOriginDist,args)
    def SquaredOriginMaxDist(*args): return apply(_blcelc.csBox3_SquaredOriginMaxDist,args)
    def ProjectBox(*args): return apply(_blcelc.csBox3_ProjectBox,args)
    def ProjectOutline(*args): return apply(_blcelc.csBox3_ProjectOutline,args)
    def ProjectBoxAndOutline(*args): return apply(_blcelc.csBox3_ProjectBoxAndOutline,args)
    def __iadd__(*args): return apply(_blcelc.csBox3___iadd__,args)
    def __imul__(*args): return apply(_blcelc.csBox3___imul__,args)
    def TestIntersect(*args): return apply(_blcelc.csBox3_TestIntersect,args)
    def __del__(self, destroy= _blcelc.delete_csBox3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csBox3 instance at %s>" % (self.this,)

class csBox3Ptr(csBox3):
    def __init__(self,this):
        _swig_setattr(self, csBox3, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csBox3, 'thisown', 0)
        _swig_setattr(self, csBox3,self.__class__,csBox3)
_blcelc.csBox3_swigregister(csBox3Ptr)
csBox3_OtherSide = _blcelc.csBox3_OtherSide


class csRGBcolor(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csRGBcolor, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csRGBcolor, name)
    __swig_setmethods__["red"] = _blcelc.csRGBcolor_red_set
    __swig_getmethods__["red"] = _blcelc.csRGBcolor_red_get
    if _newclass:red = property(_blcelc.csRGBcolor_red_get,_blcelc.csRGBcolor_red_set)
    __swig_setmethods__["green"] = _blcelc.csRGBcolor_green_set
    __swig_getmethods__["green"] = _blcelc.csRGBcolor_green_get
    if _newclass:green = property(_blcelc.csRGBcolor_green_get,_blcelc.csRGBcolor_green_set)
    __swig_setmethods__["blue"] = _blcelc.csRGBcolor_blue_set
    __swig_getmethods__["blue"] = _blcelc.csRGBcolor_blue_get
    if _newclass:blue = property(_blcelc.csRGBcolor_blue_get,_blcelc.csRGBcolor_blue_set)
    def __init__(self,*args):
        _swig_setattr(self, csRGBcolor, 'this', apply(_blcelc.new_csRGBcolor,args))
        _swig_setattr(self, csRGBcolor, 'thisown', 1)
    def Set(*args): return apply(_blcelc.csRGBcolor_Set,args)
    def __eq__(*args): return apply(_blcelc.csRGBcolor___eq__,args)
    def __ne__(*args): return apply(_blcelc.csRGBcolor___ne__,args)
    def __add__(*args): return apply(_blcelc.csRGBcolor___add__,args)
    def __del__(self, destroy= _blcelc.delete_csRGBcolor):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csRGBcolor instance at %s>" % (self.this,)

class csRGBcolorPtr(csRGBcolor):
    def __init__(self,this):
        _swig_setattr(self, csRGBcolor, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csRGBcolor, 'thisown', 0)
        _swig_setattr(self, csRGBcolor,self.__class__,csRGBcolor)
_blcelc.csRGBcolor_swigregister(csRGBcolorPtr)

class csRGBpixel(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csRGBpixel, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csRGBpixel, name)
    __swig_setmethods__["red"] = _blcelc.csRGBpixel_red_set
    __swig_getmethods__["red"] = _blcelc.csRGBpixel_red_get
    if _newclass:red = property(_blcelc.csRGBpixel_red_get,_blcelc.csRGBpixel_red_set)
    __swig_setmethods__["green"] = _blcelc.csRGBpixel_green_set
    __swig_getmethods__["green"] = _blcelc.csRGBpixel_green_get
    if _newclass:green = property(_blcelc.csRGBpixel_green_get,_blcelc.csRGBpixel_green_set)
    __swig_setmethods__["blue"] = _blcelc.csRGBpixel_blue_set
    __swig_getmethods__["blue"] = _blcelc.csRGBpixel_blue_get
    if _newclass:blue = property(_blcelc.csRGBpixel_blue_get,_blcelc.csRGBpixel_blue_set)
    __swig_setmethods__["alpha"] = _blcelc.csRGBpixel_alpha_set
    __swig_getmethods__["alpha"] = _blcelc.csRGBpixel_alpha_get
    if _newclass:alpha = property(_blcelc.csRGBpixel_alpha_get,_blcelc.csRGBpixel_alpha_set)
    def __init__(self,*args):
        _swig_setattr(self, csRGBpixel, 'this', apply(_blcelc.new_csRGBpixel,args))
        _swig_setattr(self, csRGBpixel, 'thisown', 1)
    def __eq__(*args): return apply(_blcelc.csRGBpixel___eq__,args)
    def __ne__(*args): return apply(_blcelc.csRGBpixel___ne__,args)
    def asRGBcolor(*args): return apply(_blcelc.csRGBpixel_asRGBcolor,args)
    def eq(*args): return apply(_blcelc.csRGBpixel_eq,args)
    def Intensity(*args): return apply(_blcelc.csRGBpixel_Intensity,args)
    def Luminance(*args): return apply(_blcelc.csRGBpixel_Luminance,args)
    def Set(*args): return apply(_blcelc.csRGBpixel_Set,args)
    def __iadd__(*args): return apply(_blcelc.csRGBpixel___iadd__,args)
    def UnsafeAdd(*args): return apply(_blcelc.csRGBpixel_UnsafeAdd,args)
    def SafeAdd(*args): return apply(_blcelc.csRGBpixel_SafeAdd,args)
    def __del__(self, destroy= _blcelc.delete_csRGBpixel):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csRGBpixel instance at %s>" % (self.this,)

class csRGBpixelPtr(csRGBpixel):
    def __init__(self,this):
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

csPrintfV = _blcelc.csPrintfV

csFPutErr = _blcelc.csFPutErr

csGetTicks = _blcelc.csGetTicks

csSleep = _blcelc.csSleep

csGetUsername = _blcelc.csGetUsername

CS_REQUEST_END = _blcelc.CS_REQUEST_END
class csInitializer(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csInitializer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csInitializer, name)
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
    __swig_getmethods__["CreateConfigManager"] = lambda x: _blcelc.csInitializer_CreateConfigManager
    if _newclass:CreateConfigManager = staticmethod(_blcelc.csInitializer_CreateConfigManager)
    __swig_getmethods__["CreateInputDrivers"] = lambda x: _blcelc.csInitializer_CreateInputDrivers
    if _newclass:CreateInputDrivers = staticmethod(_blcelc.csInitializer_CreateInputDrivers)
    __swig_getmethods__["SetupConfigManager"] = lambda x: _blcelc.csInitializer_SetupConfigManager
    if _newclass:SetupConfigManager = staticmethod(_blcelc.csInitializer_SetupConfigManager)
    __swig_getmethods__["SetupVFS"] = lambda x: _blcelc.csInitializer_SetupVFS
    if _newclass:SetupVFS = staticmethod(_blcelc.csInitializer_SetupVFS)
    __swig_getmethods__["OpenApplication"] = lambda x: _blcelc.csInitializer_OpenApplication
    if _newclass:OpenApplication = staticmethod(_blcelc.csInitializer_OpenApplication)
    __swig_getmethods__["CloseApplication"] = lambda x: _blcelc.csInitializer_CloseApplication
    if _newclass:CloseApplication = staticmethod(_blcelc.csInitializer_CloseApplication)
    __swig_getmethods__["_SetupEventHandler"] = lambda x: _blcelc.csInitializer__SetupEventHandler
    if _newclass:_SetupEventHandler = staticmethod(_blcelc.csInitializer__SetupEventHandler)
    __swig_getmethods__["SetupEventHandler"] = lambda x: _blcelc.csInitializer_SetupEventHandler
    if _newclass:SetupEventHandler = staticmethod(_blcelc.csInitializer_SetupEventHandler)
    __swig_getmethods__["DestroyApplication"] = lambda x: _blcelc.csInitializer_DestroyApplication
    if _newclass:DestroyApplication = staticmethod(_blcelc.csInitializer_DestroyApplication)
    __swig_getmethods__["_RequestPlugin"] = lambda x: _blcelc.csInitializer__RequestPlugin
    if _newclass:_RequestPlugin = staticmethod(_blcelc.csInitializer__RequestPlugin)
    def __init__(self,*args):
        _swig_setattr(self, csInitializer, 'this', apply(_blcelc.new_csInitializer,args))
        _swig_setattr(self, csInitializer, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csInitializer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csInitializer instance at %s>" % (self.this,)

class csInitializerPtr(csInitializer):
    def __init__(self,this):
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

csInitializer_CreateConfigManager = _blcelc.csInitializer_CreateConfigManager

csInitializer_CreateInputDrivers = _blcelc.csInitializer_CreateInputDrivers

csInitializer_SetupConfigManager = _blcelc.csInitializer_SetupConfigManager

csInitializer_SetupVFS = _blcelc.csInitializer_SetupVFS

csInitializer_OpenApplication = _blcelc.csInitializer_OpenApplication

csInitializer_CloseApplication = _blcelc.csInitializer_CloseApplication

csInitializer__SetupEventHandler = _blcelc.csInitializer__SetupEventHandler

csInitializer_SetupEventHandler = _blcelc.csInitializer_SetupEventHandler

csInitializer_DestroyApplication = _blcelc.csInitializer_DestroyApplication

csInitializer__RequestPlugin = _blcelc.csInitializer__RequestPlugin


class iAwsKey(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iAwsKey, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iAwsKey, name)
    def Type(*args): return apply(_blcelc.iAwsKey_Type,args)
    def Name(*args): return apply(_blcelc.iAwsKey_Name,args)
    def __del__(self, destroy= _blcelc.delete_iAwsKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iAwsKey_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iAwsKey_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsKey instance at %s>" % (self.this,)

class iAwsKeyPtr(iAwsKey):
    def __init__(self,this):
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
    def Value(*args): return apply(_blcelc.iAwsIntKey_Value,args)
    def __del__(self, destroy= _blcelc.delete_iAwsIntKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsIntKey instance at %s>" % (self.this,)

class iAwsIntKeyPtr(iAwsIntKey):
    def __init__(self,this):
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
    def Value(*args): return apply(_blcelc.iAwsFloatKey_Value,args)
    def __del__(self, destroy= _blcelc.delete_iAwsFloatKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsFloatKey instance at %s>" % (self.this,)

class iAwsFloatKeyPtr(iAwsFloatKey):
    def __init__(self,this):
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
    def Value(*args): return apply(_blcelc.iAwsStringKey_Value,args)
    def __del__(self, destroy= _blcelc.delete_iAwsStringKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsStringKey instance at %s>" % (self.this,)

class iAwsStringKeyPtr(iAwsStringKey):
    def __init__(self,this):
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
    def Value(*args): return apply(_blcelc.iAwsRectKey_Value,args)
    def __del__(self, destroy= _blcelc.delete_iAwsRectKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsRectKey instance at %s>" % (self.this,)

class iAwsRectKeyPtr(iAwsRectKey):
    def __init__(self,this):
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
    def Value(*args): return apply(_blcelc.iAwsRGBKey_Value,args)
    def __del__(self, destroy= _blcelc.delete_iAwsRGBKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsRGBKey instance at %s>" % (self.this,)

class iAwsRGBKeyPtr(iAwsRGBKey):
    def __init__(self,this):
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
    def Value(*args): return apply(_blcelc.iAwsPointKey_Value,args)
    def __del__(self, destroy= _blcelc.delete_iAwsPointKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsPointKey instance at %s>" % (self.this,)

class iAwsPointKeyPtr(iAwsPointKey):
    def __init__(self,this):
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
    def Sink(*args): return apply(_blcelc.iAwsConnectionKey_Sink,args)
    def Trigger(*args): return apply(_blcelc.iAwsConnectionKey_Trigger,args)
    def Signal(*args): return apply(_blcelc.iAwsConnectionKey_Signal,args)
    def __del__(self, destroy= _blcelc.delete_iAwsConnectionKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsConnectionKey instance at %s>" % (self.this,)

class iAwsConnectionKeyPtr(iAwsConnectionKey):
    def __init__(self,this):
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
    def Find(*args): return apply(_blcelc.iAwsKeyContainer_Find,args)
    def Children(*args): return apply(_blcelc.iAwsKeyContainer_Children,args)
    def Add(*args): return apply(_blcelc.iAwsKeyContainer_Add,args)
    def GetAt(*args): return apply(_blcelc.iAwsKeyContainer_GetAt,args)
    def Length(*args): return apply(_blcelc.iAwsKeyContainer_Length,args)
    def Remove(*args): return apply(_blcelc.iAwsKeyContainer_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iAwsKeyContainer_RemoveAll,args)
    def Consume(*args): return apply(_blcelc.iAwsKeyContainer_Consume,args)
    def __del__(self, destroy= _blcelc.delete_iAwsKeyContainer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsKeyContainer instance at %s>" % (self.this,)

class iAwsKeyContainerPtr(iAwsKeyContainer):
    def __init__(self,this):
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
    def ComponentTypeName(*args): return apply(_blcelc.iAwsComponentNode_ComponentTypeName,args)
    def __del__(self, destroy= _blcelc.delete_iAwsComponentNode):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsComponentNode instance at %s>" % (self.this,)

class iAwsComponentNodePtr(iAwsComponentNode):
    def __init__(self,this):
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
    def GetPrefMgr(*args): return apply(_blcelc.iAws_GetPrefMgr,args)
    def GetSinkMgr(*args): return apply(_blcelc.iAws_GetSinkMgr,args)
    def SetPrefMgr(*args): return apply(_blcelc.iAws_SetPrefMgr,args)
    def RegisterComponentFactory(*args): return apply(_blcelc.iAws_RegisterComponentFactory,args)
    def FindComponentFactory(*args): return apply(_blcelc.iAws_FindComponentFactory,args)
    def GetTopComponent(*args): return apply(_blcelc.iAws_GetTopComponent,args)
    def SetTopComponent(*args): return apply(_blcelc.iAws_SetTopComponent,args)
    def GetFocusedComponent(*args): return apply(_blcelc.iAws_GetFocusedComponent,args)
    def SetFocusedComponent(*args): return apply(_blcelc.iAws_SetFocusedComponent,args)
    def GetKeyboardFocusedComponent(*args): return apply(_blcelc.iAws_GetKeyboardFocusedComponent,args)
    def ComponentAt(*args): return apply(_blcelc.iAws_ComponentAt,args)
    def MouseInComponent(*args): return apply(_blcelc.iAws_MouseInComponent,args)
    def Print(*args): return apply(_blcelc.iAws_Print,args)
    def Redraw(*args): return apply(_blcelc.iAws_Redraw,args)
    def Mark(*args): return apply(_blcelc.iAws_Mark,args)
    def Unmark(*args): return apply(_blcelc.iAws_Unmark,args)
    def Erase(*args): return apply(_blcelc.iAws_Erase,args)
    def MaskEraser(*args): return apply(_blcelc.iAws_MaskEraser,args)
    def InvalidateUpdateStore(*args): return apply(_blcelc.iAws_InvalidateUpdateStore,args)
    def CaptureMouse(*args): return apply(_blcelc.iAws_CaptureMouse,args)
    def ReleaseMouse(*args): return apply(_blcelc.iAws_ReleaseMouse,args)
    def SetModal(*args): return apply(_blcelc.iAws_SetModal,args)
    def UnSetModal(*args): return apply(_blcelc.iAws_UnSetModal,args)
    def HandleEvent(*args): return apply(_blcelc.iAws_HandleEvent,args)
    def SetupCanvas(*args): return apply(_blcelc.iAws_SetupCanvas,args)
    def GetCanvas(*args): return apply(_blcelc.iAws_GetCanvas,args)
    def G2D(*args): return apply(_blcelc.iAws_G2D,args)
    def G3D(*args): return apply(_blcelc.iAws_G3D,args)
    def CreateWindowFrom(*args): return apply(_blcelc.iAws_CreateWindowFrom,args)
    def CreateEmbeddableComponent(*args): return apply(_blcelc.iAws_CreateEmbeddableComponent,args)
    def CreateParmList(*args): return apply(_blcelc.iAws_CreateParmList,args)
    def CreateTransition(*args): return apply(_blcelc.iAws_CreateTransition,args)
    def CreateTransitionEx(*args): return apply(_blcelc.iAws_CreateTransitionEx,args)
    def SetFlag(*args): return apply(_blcelc.iAws_SetFlag,args)
    def ClearFlag(*args): return apply(_blcelc.iAws_ClearFlag,args)
    def GetFlags(*args): return apply(_blcelc.iAws_GetFlags,args)
    def GetObjectRegistry(*args): return apply(_blcelc.iAws_GetObjectRegistry,args)
    def AllWindowsHidden(*args): return apply(_blcelc.iAws_AllWindowsHidden,args)
    def ComponentIsInTransition(*args): return apply(_blcelc.iAws_ComponentIsInTransition,args)
    def __del__(self, destroy= _blcelc.delete_iAws):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iAws_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iAws_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAws instance at %s>" % (self.this,)

class iAwsPtr(iAws):
    def __init__(self,this):
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
    def Setup(*args): return apply(_blcelc.iAwsPrefManager_Setup,args)
    def Load(*args): return apply(_blcelc.iAwsPrefManager_Load,args)
    def NameToId(*args): return apply(_blcelc.iAwsPrefManager_NameToId,args)
    def SelectDefaultSkin(*args): return apply(_blcelc.iAwsPrefManager_SelectDefaultSkin,args)
    def LookupIntKey(*args): return apply(_blcelc.iAwsPrefManager_LookupIntKey,args)
    def LookupStringKey(*args): return apply(_blcelc.iAwsPrefManager_LookupStringKey,args)
    def LookupRectKey(*args): return apply(_blcelc.iAwsPrefManager_LookupRectKey,args)
    def LookupRGBKey(*args): return apply(_blcelc.iAwsPrefManager_LookupRGBKey,args)
    def LookupPointKey(*args): return apply(_blcelc.iAwsPrefManager_LookupPointKey,args)
    def GetInt(*args): return apply(_blcelc.iAwsPrefManager_GetInt,args)
    def GetFloat(*args): return apply(_blcelc.iAwsPrefManager_GetFloat,args)
    def GetRect(*args): return apply(_blcelc.iAwsPrefManager_GetRect,args)
    def GetString(*args): return apply(_blcelc.iAwsPrefManager_GetString,args)
    def GetRGB(*args): return apply(_blcelc.iAwsPrefManager_GetRGB,args)
    def FindWindowDef(*args): return apply(_blcelc.iAwsPrefManager_FindWindowDef,args)
    def FindSkinDef(*args): return apply(_blcelc.iAwsPrefManager_FindSkinDef,args)
    def RemoveWindowDef(*args): return apply(_blcelc.iAwsPrefManager_RemoveWindowDef,args)
    def RemoveAllWindowDefs(*args): return apply(_blcelc.iAwsPrefManager_RemoveAllWindowDefs,args)
    def RemoveSkinDef(*args): return apply(_blcelc.iAwsPrefManager_RemoveSkinDef,args)
    def RemoveAllSkinDefs(*args): return apply(_blcelc.iAwsPrefManager_RemoveAllSkinDefs,args)
    def SetColor(*args): return apply(_blcelc.iAwsPrefManager_SetColor,args)
    def GetColor(*args): return apply(_blcelc.iAwsPrefManager_GetColor,args)
    def FindColor(*args): return apply(_blcelc.iAwsPrefManager_FindColor,args)
    def GetDefaultFont(*args): return apply(_blcelc.iAwsPrefManager_GetDefaultFont,args)
    def GetFont(*args): return apply(_blcelc.iAwsPrefManager_GetFont,args)
    def GetTexture(*args): return apply(_blcelc.iAwsPrefManager_GetTexture,args)
    def SetTextureManager(*args): return apply(_blcelc.iAwsPrefManager_SetTextureManager,args)
    def SetFontServer(*args): return apply(_blcelc.iAwsPrefManager_SetFontServer,args)
    def SetDefaultFont(*args): return apply(_blcelc.iAwsPrefManager_SetDefaultFont,args)
    def SetWindowMgr(*args): return apply(_blcelc.iAwsPrefManager_SetWindowMgr,args)
    def SetupPalette(*args): return apply(_blcelc.iAwsPrefManager_SetupPalette,args)
    def RegisterConstant(*args): return apply(_blcelc.iAwsPrefManager_RegisterConstant,args)
    def ConstantExists(*args): return apply(_blcelc.iAwsPrefManager_ConstantExists,args)
    def GetConstantValue(*args): return apply(_blcelc.iAwsPrefManager_GetConstantValue,args)
    def CreateKeyFactory(*args): return apply(_blcelc.iAwsPrefManager_CreateKeyFactory,args)
    def CreateConnectionNodeFactory(*args): return apply(_blcelc.iAwsPrefManager_CreateConnectionNodeFactory,args)
    def __del__(self, destroy= _blcelc.delete_iAwsPrefManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsPrefManager instance at %s>" % (self.this,)

class iAwsPrefManagerPtr(iAwsPrefManager):
    def __init__(self,this):
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
    def RegisterSink(*args): return apply(_blcelc.iAwsSinkManager_RegisterSink,args)
    def RemoveSink(*args): return apply(_blcelc.iAwsSinkManager_RemoveSink,args)
    def FindSink(*args): return apply(_blcelc.iAwsSinkManager_FindSink,args)
    def CreateSink(*args): return apply(_blcelc.iAwsSinkManager_CreateSink,args)
    def CreateSlot(*args): return apply(_blcelc.iAwsSinkManager_CreateSlot,args)
    def __del__(self, destroy= _blcelc.delete_iAwsSinkManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsSinkManager instance at %s>" % (self.this,)

class iAwsSinkManagerPtr(iAwsSinkManager):
    def __init__(self,this):
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
    def GetTriggerID(*args): return apply(_blcelc.iAwsSink_GetTriggerID,args)
    def HandleTrigger(*args): return apply(_blcelc.iAwsSink_HandleTrigger,args)
    def RegisterTrigger(*args): return apply(_blcelc.iAwsSink_RegisterTrigger,args)
    def GetError(*args): return apply(_blcelc.iAwsSink_GetError,args)
    def __del__(self, destroy= _blcelc.delete_iAwsSink):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsSink instance at %s>" % (self.this,)

class iAwsSinkPtr(iAwsSink):
    def __init__(self,this):
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
    def GetComponent(*args): return apply(_blcelc.iAwsSource_GetComponent,args)
    def RegisterSlot(*args): return apply(_blcelc.iAwsSource_RegisterSlot,args)
    def UnregisterSlot(*args): return apply(_blcelc.iAwsSource_UnregisterSlot,args)
    def Broadcast(*args): return apply(_blcelc.iAwsSource_Broadcast,args)
    def __del__(self, destroy= _blcelc.delete_iAwsSource):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsSource instance at %s>" % (self.this,)

class iAwsSourcePtr(iAwsSource):
    def __init__(self,this):
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
    def Connect(*args): return apply(_blcelc.iAwsSlot_Connect,args)
    def Disconnect(*args): return apply(_blcelc.iAwsSlot_Disconnect,args)
    def Emit(*args): return apply(_blcelc.iAwsSlot_Emit,args)
    def __del__(self, destroy= _blcelc.delete_iAwsSlot):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsSlot instance at %s>" % (self.this,)

class iAwsSlotPtr(iAwsSlot):
    def __init__(self,this):
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
    def SetOwner(*args): return apply(_blcelc.iAwsLayoutManager_SetOwner,args)
    def AddComponent(*args): return apply(_blcelc.iAwsLayoutManager_AddComponent,args)
    def RemoveComponent(*args): return apply(_blcelc.iAwsLayoutManager_RemoveComponent,args)
    def LayoutComponents(*args): return apply(_blcelc.iAwsLayoutManager_LayoutComponents,args)
    def __del__(self, destroy= _blcelc.delete_iAwsLayoutManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsLayoutManager instance at %s>" % (self.this,)

class iAwsLayoutManagerPtr(iAwsLayoutManager):
    def __init__(self,this):
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
    def Create(*args): return apply(_blcelc.iAwsComponent_Create,args)
    def Setup(*args): return apply(_blcelc.iAwsComponent_Setup,args)
    def HandleEvent(*args): return apply(_blcelc.iAwsComponent_HandleEvent,args)
    def GetProperty(*args): return apply(_blcelc.iAwsComponent_GetProperty,args)
    def SetProperty(*args): return apply(_blcelc.iAwsComponent_SetProperty,args)
    def Execute(*args): return apply(_blcelc.iAwsComponent_Execute,args)
    def Invalidate(*args): return apply(_blcelc.iAwsComponent_Invalidate,args)
    def Frame(*args): return apply(_blcelc.iAwsComponent_Frame,args)
    def ClientFrame(*args): return apply(_blcelc.iAwsComponent_ClientFrame,args)
    def Type(*args): return apply(_blcelc.iAwsComponent_Type,args)
    def SetFlag(*args): return apply(_blcelc.iAwsComponent_SetFlag,args)
    def ClearFlag(*args): return apply(_blcelc.iAwsComponent_ClearFlag,args)
    def Flags(*args): return apply(_blcelc.iAwsComponent_Flags,args)
    def WindowManager(*args): return apply(_blcelc.iAwsComponent_WindowManager,args)
    def Parent(*args): return apply(_blcelc.iAwsComponent_Parent,args)
    def Window(*args): return apply(_blcelc.iAwsComponent_Window,args)
    def Layout(*args): return apply(_blcelc.iAwsComponent_Layout,args)
    def SetParent(*args): return apply(_blcelc.iAwsComponent_SetParent,args)
    def SetLayout(*args): return apply(_blcelc.iAwsComponent_SetLayout,args)
    def AddToLayout(*args): return apply(_blcelc.iAwsComponent_AddToLayout,args)
    def getPreferredSize(*args): return apply(_blcelc.iAwsComponent_getPreferredSize,args)
    def getMinimumSize(*args): return apply(_blcelc.iAwsComponent_getMinimumSize,args)
    def getInsets(*args): return apply(_blcelc.iAwsComponent_getInsets,args)
    def Overlaps(*args): return apply(_blcelc.iAwsComponent_Overlaps,args)
    def isHidden(*args): return apply(_blcelc.iAwsComponent_isHidden,args)
    def SetFocusable(*args): return apply(_blcelc.iAwsComponent_SetFocusable,args)
    def Focusable(*args): return apply(_blcelc.iAwsComponent_Focusable,args)
    def isFocused(*args): return apply(_blcelc.iAwsComponent_isFocused,args)
    def IsMaximized(*args): return apply(_blcelc.iAwsComponent_IsMaximized,args)
    def Hide(*args): return apply(_blcelc.iAwsComponent_Hide,args)
    def Show(*args): return apply(_blcelc.iAwsComponent_Show,args)
    def SetFocus(*args): return apply(_blcelc.iAwsComponent_SetFocus,args)
    def UnsetFocus(*args): return apply(_blcelc.iAwsComponent_UnsetFocus,args)
    def Move(*args): return apply(_blcelc.iAwsComponent_Move,args)
    def MoveTo(*args): return apply(_blcelc.iAwsComponent_MoveTo,args)
    def Resize(*args): return apply(_blcelc.iAwsComponent_Resize,args)
    def ResizeTo(*args): return apply(_blcelc.iAwsComponent_ResizeTo,args)
    def Maximize(*args): return apply(_blcelc.iAwsComponent_Maximize,args)
    def UnMaximize(*args): return apply(_blcelc.iAwsComponent_UnMaximize,args)
    def LayoutChildren(*args): return apply(_blcelc.iAwsComponent_LayoutChildren,args)
    def isDeaf(*args): return apply(_blcelc.iAwsComponent_isDeaf,args)
    def SetDeaf(*args): return apply(_blcelc.iAwsComponent_SetDeaf,args)
    def GetID(*args): return apply(_blcelc.iAwsComponent_GetID,args)
    def SetID(*args): return apply(_blcelc.iAwsComponent_SetID,args)
    def FindChild(*args): return apply(_blcelc.iAwsComponent_FindChild,args)
    def DoFindChild(*args): return apply(_blcelc.iAwsComponent_DoFindChild,args)
    def ChildAt(*args): return apply(_blcelc.iAwsComponent_ChildAt,args)
    def AddChild(*args): return apply(_blcelc.iAwsComponent_AddChild,args)
    def RemoveChild(*args): return apply(_blcelc.iAwsComponent_RemoveChild,args)
    def GetChildCount(*args): return apply(_blcelc.iAwsComponent_GetChildCount,args)
    def GetTopChild(*args): return apply(_blcelc.iAwsComponent_GetTopChild,args)
    def ComponentAbove(*args): return apply(_blcelc.iAwsComponent_ComponentAbove,args)
    def ComponentBelow(*args): return apply(_blcelc.iAwsComponent_ComponentBelow,args)
    def SetComponentAbove(*args): return apply(_blcelc.iAwsComponent_SetComponentAbove,args)
    def SetComponentBelow(*args): return apply(_blcelc.iAwsComponent_SetComponentBelow,args)
    def AddToTabOrder(*args): return apply(_blcelc.iAwsComponent_AddToTabOrder,args)
    def TabNext(*args): return apply(_blcelc.iAwsComponent_TabNext,args)
    def TabPrev(*args): return apply(_blcelc.iAwsComponent_TabPrev,args)
    def GetTabLength(*args): return apply(_blcelc.iAwsComponent_GetTabLength,args)
    def GetTabComponent(*args): return apply(_blcelc.iAwsComponent_GetTabComponent,args)
    def GetFirstFocusableChild(*args): return apply(_blcelc.iAwsComponent_GetFirstFocusableChild,args)
    def Raise(*args): return apply(_blcelc.iAwsComponent_Raise,args)
    def Lower(*args): return apply(_blcelc.iAwsComponent_Lower,args)
    def HasChildren(*args): return apply(_blcelc.iAwsComponent_HasChildren,args)
    def SetRedrawTag(*args): return apply(_blcelc.iAwsComponent_SetRedrawTag,args)
    def RedrawTag(*args): return apply(_blcelc.iAwsComponent_RedrawTag,args)
    def OnDraw(*args): return apply(_blcelc.iAwsComponent_OnDraw,args)
    def OnMouseDown(*args): return apply(_blcelc.iAwsComponent_OnMouseDown,args)
    def OnMouseUp(*args): return apply(_blcelc.iAwsComponent_OnMouseUp,args)
    def OnMouseMove(*args): return apply(_blcelc.iAwsComponent_OnMouseMove,args)
    def OnMouseClick(*args): return apply(_blcelc.iAwsComponent_OnMouseClick,args)
    def OnMouseDoubleClick(*args): return apply(_blcelc.iAwsComponent_OnMouseDoubleClick,args)
    def OnMouseExit(*args): return apply(_blcelc.iAwsComponent_OnMouseExit,args)
    def OnMouseEnter(*args): return apply(_blcelc.iAwsComponent_OnMouseEnter,args)
    def OnKeyboard(*args): return apply(_blcelc.iAwsComponent_OnKeyboard,args)
    def OnLostFocus(*args): return apply(_blcelc.iAwsComponent_OnLostFocus,args)
    def OnGainFocus(*args): return apply(_blcelc.iAwsComponent_OnGainFocus,args)
    def OnFrame(*args): return apply(_blcelc.iAwsComponent_OnFrame,args)
    def OnAdded(*args): return apply(_blcelc.iAwsComponent_OnAdded,args)
    def OnResized(*args): return apply(_blcelc.iAwsComponent_OnResized,args)
    def OnChildMoved(*args): return apply(_blcelc.iAwsComponent_OnChildMoved,args)
    def OnRaise(*args): return apply(_blcelc.iAwsComponent_OnRaise,args)
    def OnLower(*args): return apply(_blcelc.iAwsComponent_OnLower,args)
    def OnChildHide(*args): return apply(_blcelc.iAwsComponent_OnChildHide,args)
    def OnChildShow(*args): return apply(_blcelc.iAwsComponent_OnChildShow,args)
    def OnSetFocus(*args): return apply(_blcelc.iAwsComponent_OnSetFocus,args)
    def OnUnsetFocus(*args): return apply(_blcelc.iAwsComponent_OnUnsetFocus,args)
    def Unlink(*args): return apply(_blcelc.iAwsComponent_Unlink,args)
    def LinkAbove(*args): return apply(_blcelc.iAwsComponent_LinkAbove,args)
    def LinkBelow(*args): return apply(_blcelc.iAwsComponent_LinkBelow,args)
    def SetTopChild(*args): return apply(_blcelc.iAwsComponent_SetTopChild,args)
    def __del__(self, destroy= _blcelc.delete_iAwsComponent):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsComponent instance at %s>" % (self.this,)

class iAwsComponentPtr(iAwsComponent):
    def __init__(self,this):
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
    def Create(*args): return apply(_blcelc.iAwsComponentFactory_Create,args)
    def Register(*args): return apply(_blcelc.iAwsComponentFactory_Register,args)
    def RegisterConstant(*args): return apply(_blcelc.iAwsComponentFactory_RegisterConstant,args)
    def __del__(self, destroy= _blcelc.delete_iAwsComponentFactory):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsComponentFactory instance at %s>" % (self.this,)

class iAwsComponentFactoryPtr(iAwsComponentFactory):
    def __init__(self,this):
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
    def Initialize(*args): return apply(_blcelc.iAwsKeyFactory_Initialize,args)
    def AddToWindowList(*args): return apply(_blcelc.iAwsKeyFactory_AddToWindowList,args)
    def AddFactory(*args): return apply(_blcelc.iAwsKeyFactory_AddFactory,args)
    def AddIntKey(*args): return apply(_blcelc.iAwsKeyFactory_AddIntKey,args)
    def AddStringKey(*args): return apply(_blcelc.iAwsKeyFactory_AddStringKey,args)
    def AddRectKey(*args): return apply(_blcelc.iAwsKeyFactory_AddRectKey,args)
    def AddRGBKey(*args): return apply(_blcelc.iAwsKeyFactory_AddRGBKey,args)
    def AddPointKey(*args): return apply(_blcelc.iAwsKeyFactory_AddPointKey,args)
    def AddConnectionKey(*args): return apply(_blcelc.iAwsKeyFactory_AddConnectionKey,args)
    def AddConnectionNode(*args): return apply(_blcelc.iAwsKeyFactory_AddConnectionNode,args)
    def GetThisNode(*args): return apply(_blcelc.iAwsKeyFactory_GetThisNode,args)
    def __del__(self, destroy= _blcelc.delete_iAwsKeyFactory):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsKeyFactory instance at %s>" % (self.this,)

class iAwsKeyFactoryPtr(iAwsKeyFactory):
    def __init__(self,this):
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
    def Initialize(*args): return apply(_blcelc.iAwsConnectionNodeFactory_Initialize,args)
    def AddConnectionKey(*args): return apply(_blcelc.iAwsConnectionNodeFactory_AddConnectionKey,args)
    def GetThisNode(*args): return apply(_blcelc.iAwsConnectionNodeFactory_GetThisNode,args)
    def __del__(self, destroy= _blcelc.delete_iAwsConnectionNodeFactory):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAwsConnectionNodeFactory instance at %s>" % (self.this,)

class iAwsConnectionNodeFactoryPtr(iAwsConnectionNodeFactory):
    def __init__(self,this):
        _swig_setattr(self, iAwsConnectionNodeFactory, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iAwsConnectionNodeFactory, 'thisown', 0)
        _swig_setattr(self, iAwsConnectionNodeFactory,self.__class__,iAwsConnectionNodeFactory)
_blcelc.iAwsConnectionNodeFactory_swigregister(iAwsConnectionNodeFactoryPtr)

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
    __swig_setmethods__["num_vertices"] = _blcelc.csMeshedPolygon_num_vertices_set
    __swig_getmethods__["num_vertices"] = _blcelc.csMeshedPolygon_num_vertices_get
    if _newclass:num_vertices = property(_blcelc.csMeshedPolygon_num_vertices_get,_blcelc.csMeshedPolygon_num_vertices_set)
    __swig_setmethods__["vertices"] = _blcelc.csMeshedPolygon_vertices_set
    __swig_getmethods__["vertices"] = _blcelc.csMeshedPolygon_vertices_get
    if _newclass:vertices = property(_blcelc.csMeshedPolygon_vertices_get,_blcelc.csMeshedPolygon_vertices_set)
    def __init__(self,*args):
        _swig_setattr(self, csMeshedPolygon, 'this', apply(_blcelc.new_csMeshedPolygon,args))
        _swig_setattr(self, csMeshedPolygon, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csMeshedPolygon):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csMeshedPolygon instance at %s>" % (self.this,)

class csMeshedPolygonPtr(csMeshedPolygon):
    def __init__(self,this):
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
    def GetVertexCount(*args): return apply(_blcelc.iPolygonMesh_GetVertexCount,args)
    def GetVertices(*args): return apply(_blcelc.iPolygonMesh_GetVertices,args)
    def GetPolygonCount(*args): return apply(_blcelc.iPolygonMesh_GetPolygonCount,args)
    def GetPolygons(*args): return apply(_blcelc.iPolygonMesh_GetPolygons,args)
    def GetTriangleCount(*args): return apply(_blcelc.iPolygonMesh_GetTriangleCount,args)
    def GetTriangles(*args): return apply(_blcelc.iPolygonMesh_GetTriangles,args)
    def Lock(*args): return apply(_blcelc.iPolygonMesh_Lock,args)
    def Unlock(*args): return apply(_blcelc.iPolygonMesh_Unlock,args)
    def GetFlags(*args): return apply(_blcelc.iPolygonMesh_GetFlags,args)
    def GetChangeNumber(*args): return apply(_blcelc.iPolygonMesh_GetChangeNumber,args)
    def __del__(self, destroy= _blcelc.delete_iPolygonMesh):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iPolygonMesh_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iPolygonMesh_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPolygonMesh instance at %s>" % (self.this,)

class iPolygonMeshPtr(iPolygonMesh):
    def __init__(self,this):
        _swig_setattr(self, iPolygonMesh, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPolygonMesh, 'thisown', 0)
        _swig_setattr(self, iPolygonMesh,self.__class__,iPolygonMesh)
_blcelc.iPolygonMesh_swigregister(iPolygonMeshPtr)
iPolygonMesh_scfGetVersion = _blcelc.iPolygonMesh_scfGetVersion


MAX_OUTPUT_VERTICES = _blcelc.MAX_OUTPUT_VERTICES
CS_CLIP_OUTSIDE = _blcelc.CS_CLIP_OUTSIDE
CS_CLIP_INSIDE = _blcelc.CS_CLIP_INSIDE
CS_CLIP_CLIPPED = _blcelc.CS_CLIP_CLIPPED
class csVertexStatus(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csVertexStatus, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csVertexStatus, name)
    __swig_setmethods__["Type"] = _blcelc.csVertexStatus_Type_set
    __swig_getmethods__["Type"] = _blcelc.csVertexStatus_Type_get
    if _newclass:Type = property(_blcelc.csVertexStatus_Type_get,_blcelc.csVertexStatus_Type_set)
    __swig_setmethods__["Vertex"] = _blcelc.csVertexStatus_Vertex_set
    __swig_getmethods__["Vertex"] = _blcelc.csVertexStatus_Vertex_get
    if _newclass:Vertex = property(_blcelc.csVertexStatus_Vertex_get,_blcelc.csVertexStatus_Vertex_set)
    __swig_setmethods__["Pos"] = _blcelc.csVertexStatus_Pos_set
    __swig_getmethods__["Pos"] = _blcelc.csVertexStatus_Pos_get
    if _newclass:Pos = property(_blcelc.csVertexStatus_Pos_get,_blcelc.csVertexStatus_Pos_set)
    def __init__(self,*args):
        _swig_setattr(self, csVertexStatus, 'this', apply(_blcelc.new_csVertexStatus,args))
        _swig_setattr(self, csVertexStatus, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csVertexStatus):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csVertexStatus instance at %s>" % (self.this,)

class csVertexStatusPtr(csVertexStatus):
    def __init__(self,this):
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
    def Clip(*args): return apply(_blcelc.iClipper2D_Clip,args)
    def ClipInPlace(*args): return apply(_blcelc.iClipper2D_ClipInPlace,args)
    def ClassifyBox(*args): return apply(_blcelc.iClipper2D_ClassifyBox,args)
    def IsInside(*args): return apply(_blcelc.iClipper2D_IsInside,args)
    def GetVertexCount(*args): return apply(_blcelc.iClipper2D_GetVertexCount,args)
    def GetClipPoly(*args): return apply(_blcelc.iClipper2D_GetClipPoly,args)
    def __del__(self, destroy= _blcelc.delete_iClipper2D):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iClipper2D instance at %s>" % (self.this,)

class iClipper2DPtr(iClipper2D):
    def __init__(self,this):
        _swig_setattr(self, iClipper2D, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iClipper2D, 'thisown', 0)
        _swig_setattr(self, iClipper2D,self.__class__,iClipper2D)
_blcelc.iClipper2D_swigregister(iClipper2DPtr)

CS_BBOX_NORMAL = _blcelc.CS_BBOX_NORMAL
CS_BBOX_ACCURATE = _blcelc.CS_BBOX_ACCURATE
CS_BBOX_MAX = _blcelc.CS_BBOX_MAX
class iObjectModelListener(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iObjectModelListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iObjectModelListener, name)
    def ObjectModelChanged(*args): return apply(_blcelc.iObjectModelListener_ObjectModelChanged,args)
    def __del__(self, destroy= _blcelc.delete_iObjectModelListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iObjectModelListener_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iObjectModelListener_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectModelListener instance at %s>" % (self.this,)

class iObjectModelListenerPtr(iObjectModelListener):
    def __init__(self,this):
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
    def GetShapeNumber(*args): return apply(_blcelc.iObjectModel_GetShapeNumber,args)
    def GetPolygonMeshBase(*args): return apply(_blcelc.iObjectModel_GetPolygonMeshBase,args)
    def GetPolygonMeshColldet(*args): return apply(_blcelc.iObjectModel_GetPolygonMeshColldet,args)
    def SetPolygonMeshColldet(*args): return apply(_blcelc.iObjectModel_SetPolygonMeshColldet,args)
    def GetPolygonMeshViscull(*args): return apply(_blcelc.iObjectModel_GetPolygonMeshViscull,args)
    def SetPolygonMeshViscull(*args): return apply(_blcelc.iObjectModel_SetPolygonMeshViscull,args)
    def GetPolygonMeshShadows(*args): return apply(_blcelc.iObjectModel_GetPolygonMeshShadows,args)
    def SetPolygonMeshShadows(*args): return apply(_blcelc.iObjectModel_SetPolygonMeshShadows,args)
    def CreateLowerDetailPolygonMesh(*args): return apply(_blcelc.iObjectModel_CreateLowerDetailPolygonMesh,args)
    def GetObjectBoundingBox(*args): return apply(_blcelc.iObjectModel_GetObjectBoundingBox,args)
    def GetRadius(*args): return apply(_blcelc.iObjectModel_GetRadius,args)
    def AddListener(*args): return apply(_blcelc.iObjectModel_AddListener,args)
    def RemoveListener(*args): return apply(_blcelc.iObjectModel_RemoveListener,args)
    def __del__(self, destroy= _blcelc.delete_iObjectModel):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iObjectModel_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iObjectModel_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectModel instance at %s>" % (self.this,)

class iObjectModelPtr(iObjectModel):
    def __init__(self,this):
        _swig_setattr(self, iObjectModel, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iObjectModel, 'thisown', 0)
        _swig_setattr(self, iObjectModel,self.__class__,iObjectModel)
_blcelc.iObjectModel_swigregister(iObjectModelPtr)
iObjectModel_scfGetVersion = _blcelc.iObjectModel_scfGetVersion


class iFrustumViewUserdata(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iFrustumViewUserdata, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iFrustumViewUserdata, name)
    def __del__(self, destroy= _blcelc.delete_iFrustumViewUserdata):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFrustumViewUserdata_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFrustumViewUserdata_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFrustumViewUserdata instance at %s>" % (self.this,)

class iFrustumViewUserdataPtr(iFrustumViewUserdata):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csFrustumContext, 'this', apply(_blcelc.new_csFrustumContext,args))
        _swig_setattr(self, csFrustumContext, 'thisown', 1)
    def assign(*args): return apply(_blcelc.csFrustumContext_assign,args)
    def GetShadows(*args): return apply(_blcelc.csFrustumContext_GetShadows,args)
    def SetShadows(*args): return apply(_blcelc.csFrustumContext_SetShadows,args)
    def SetNewShadows(*args): return apply(_blcelc.csFrustumContext_SetNewShadows,args)
    def IsShared(*args): return apply(_blcelc.csFrustumContext_IsShared,args)
    def SetLightFrustum(*args): return apply(_blcelc.csFrustumContext_SetLightFrustum,args)
    def SetNewLightFrustum(*args): return apply(_blcelc.csFrustumContext_SetNewLightFrustum,args)
    def GetLightFrustum(*args): return apply(_blcelc.csFrustumContext_GetLightFrustum,args)
    def SetMirrored(*args): return apply(_blcelc.csFrustumContext_SetMirrored,args)
    def IsMirrored(*args): return apply(_blcelc.csFrustumContext_IsMirrored,args)
    def __del__(self, destroy= _blcelc.delete_csFrustumContext):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csFrustumContext instance at %s>" % (self.this,)

class csFrustumContextPtr(csFrustumContext):
    def __init__(self,this):
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
    def GetFrustumContext(*args): return apply(_blcelc.iFrustumView_GetFrustumContext,args)
    def CreateFrustumContext(*args): return apply(_blcelc.iFrustumView_CreateFrustumContext,args)
    def CopyFrustumContext(*args): return apply(_blcelc.iFrustumView_CopyFrustumContext,args)
    def SetFrustumContext(*args): return apply(_blcelc.iFrustumView_SetFrustumContext,args)
    def RestoreFrustumContext(*args): return apply(_blcelc.iFrustumView_RestoreFrustumContext,args)
    def SetObjectFunction(*args): return apply(_blcelc.iFrustumView_SetObjectFunction,args)
    def CallObjectFunction(*args): return apply(_blcelc.iFrustumView_CallObjectFunction,args)
    def GetRadius(*args): return apply(_blcelc.iFrustumView_GetRadius,args)
    def GetSquaredRadius(*args): return apply(_blcelc.iFrustumView_GetSquaredRadius,args)
    def ThingShadowsEnabled(*args): return apply(_blcelc.iFrustumView_ThingShadowsEnabled,args)
    def CheckShadowMask(*args): return apply(_blcelc.iFrustumView_CheckShadowMask,args)
    def CheckProcessMask(*args): return apply(_blcelc.iFrustumView_CheckProcessMask,args)
    def StartNewShadowBlock(*args): return apply(_blcelc.iFrustumView_StartNewShadowBlock,args)
    def SetUserdata(*args): return apply(_blcelc.iFrustumView_SetUserdata,args)
    def GetUserdata(*args): return apply(_blcelc.iFrustumView_GetUserdata,args)
    def CreateShadowBlock(*args): return apply(_blcelc.iFrustumView_CreateShadowBlock,args)
    def __del__(self, destroy= _blcelc.delete_iFrustumView):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFrustumView_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFrustumView_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFrustumView instance at %s>" % (self.this,)

class iFrustumViewPtr(iFrustumView):
    def __init__(self,this):
        _swig_setattr(self, iFrustumView, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iFrustumView, 'thisown', 0)
        _swig_setattr(self, iFrustumView,self.__class__,iFrustumView)
_blcelc.iFrustumView_swigregister(iFrustumViewPtr)
iFrustumView_scfGetVersion = _blcelc.iFrustumView_scfGetVersion


CS_LIGHT_THINGSHADOWS = _blcelc.CS_LIGHT_THINGSHADOWS
CS_LIGHT_ACTIVEHALO = _blcelc.CS_LIGHT_ACTIVEHALO
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
    def OnColorChange(*args): return apply(_blcelc.iLightCallback_OnColorChange,args)
    def OnPositionChange(*args): return apply(_blcelc.iLightCallback_OnPositionChange,args)
    def OnSectorChange(*args): return apply(_blcelc.iLightCallback_OnSectorChange,args)
    def OnRadiusChange(*args): return apply(_blcelc.iLightCallback_OnRadiusChange,args)
    def OnDestroy(*args): return apply(_blcelc.iLightCallback_OnDestroy,args)
    def __del__(self, destroy= _blcelc.delete_iLightCallback):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightCallback instance at %s>" % (self.this,)

class iLightCallbackPtr(iLightCallback):
    def __init__(self,this):
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
    def GetPrivateObject(*args): return apply(_blcelc.iLight_GetPrivateObject,args)
    def GetLightID(*args): return apply(_blcelc.iLight_GetLightID,args)
    def QueryObject(*args): return apply(_blcelc.iLight_QueryObject,args)
    def GetCenter(*args): return apply(_blcelc.iLight_GetCenter,args)
    def SetCenter(*args): return apply(_blcelc.iLight_SetCenter,args)
    def GetSector(*args): return apply(_blcelc.iLight_GetSector,args)
    def SetSector(*args): return apply(_blcelc.iLight_SetSector,args)
    def GetColor(*args): return apply(_blcelc.iLight_GetColor,args)
    def SetColor(*args): return apply(_blcelc.iLight_SetColor,args)
    def IsDynamic(*args): return apply(_blcelc.iLight_IsDynamic,args)
    def GetInfluenceRadius(*args): return apply(_blcelc.iLight_GetInfluenceRadius,args)
    def GetInfluenceRadiusSq(*args): return apply(_blcelc.iLight_GetInfluenceRadiusSq,args)
    def SetInfluenceRadius(*args): return apply(_blcelc.iLight_SetInfluenceRadius,args)
    def GetAttenuation(*args): return apply(_blcelc.iLight_GetAttenuation,args)
    def SetAttenuation(*args): return apply(_blcelc.iLight_SetAttenuation,args)
    def SetAttenuationVector(*args): return apply(_blcelc.iLight_SetAttenuationVector,args)
    def GetAttenuationVector(*args): return apply(_blcelc.iLight_GetAttenuationVector,args)
    def CalculateAttenuationVector(*args): return apply(_blcelc.iLight_CalculateAttenuationVector,args)
    def GetDistanceForBrightness(*args): return apply(_blcelc.iLight_GetDistanceForBrightness,args)
    def CreateCrossHalo(*args): return apply(_blcelc.iLight_CreateCrossHalo,args)
    def CreateNovaHalo(*args): return apply(_blcelc.iLight_CreateNovaHalo,args)
    def CreateFlareHalo(*args): return apply(_blcelc.iLight_CreateFlareHalo,args)
    def GetBrightnessAtDistance(*args): return apply(_blcelc.iLight_GetBrightnessAtDistance,args)
    def GetFlags(*args): return apply(_blcelc.iLight_GetFlags,args)
    def SetLightCallback(*args): return apply(_blcelc.iLight_SetLightCallback,args)
    def RemoveLightCallback(*args): return apply(_blcelc.iLight_RemoveLightCallback,args)
    def GetLightCallbackCount(*args): return apply(_blcelc.iLight_GetLightCallbackCount,args)
    def GetLightCallback(*args): return apply(_blcelc.iLight_GetLightCallback,args)
    def GetLightNumber(*args): return apply(_blcelc.iLight_GetLightNumber,args)
    def __del__(self, destroy= _blcelc.delete_iLight):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLight instance at %s>" % (self.this,)

class iLightPtr(iLight):
    def __init__(self,this):
        _swig_setattr(self, iLight, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLight, 'thisown', 0)
        _swig_setattr(self, iLight,self.__class__,iLight)
_blcelc.iLight_swigregister(iLightPtr)

class iLightList(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLightList, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLightList, name)
    def GetCount(*args): return apply(_blcelc.iLightList_GetCount,args)
    def Get(*args): return apply(_blcelc.iLightList_Get,args)
    def Add(*args): return apply(_blcelc.iLightList_Add,args)
    def Remove(*args): return apply(_blcelc.iLightList_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iLightList_RemoveAll,args)
    def Find(*args): return apply(_blcelc.iLightList_Find,args)
    def FindByName(*args): return apply(_blcelc.iLightList_FindByName,args)
    def FindByID(*args): return apply(_blcelc.iLightList_FindByID,args)
    def __del__(self, destroy= _blcelc.delete_iLightList):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iLightList_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iLightList_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightList instance at %s>" % (self.this,)

class iLightListPtr(iLightList):
    def __init__(self,this):
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
    def FinalizeLighting(*args): return apply(_blcelc.iLightingProcessData_FinalizeLighting,args)
    def __del__(self, destroy= _blcelc.delete_iLightingProcessData):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightingProcessData instance at %s>" % (self.this,)

class iLightingProcessDataPtr(iLightingProcessData):
    def __init__(self,this):
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
    def GetLight(*args): return apply(_blcelc.iLightingProcessInfo_GetLight,args)
    def IsDynamic(*args): return apply(_blcelc.iLightingProcessInfo_IsDynamic,args)
    def SetColor(*args): return apply(_blcelc.iLightingProcessInfo_SetColor,args)
    def GetColor(*args): return apply(_blcelc.iLightingProcessInfo_GetColor,args)
    def AttachUserdata(*args): return apply(_blcelc.iLightingProcessInfo_AttachUserdata,args)
    def QueryUserdata(*args): return apply(_blcelc.iLightingProcessInfo_QueryUserdata,args)
    def FinalizeLighting(*args): return apply(_blcelc.iLightingProcessInfo_FinalizeLighting,args)
    def __del__(self, destroy= _blcelc.delete_iLightingProcessInfo):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightingProcessInfo instance at %s>" % (self.this,)

class iLightingProcessInfoPtr(iLightingProcessInfo):
    def __init__(self,this):
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
    def HasNext(*args): return apply(_blcelc.iLightIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.iLightIterator_Next,args)
    def GetLastSector(*args): return apply(_blcelc.iLightIterator_GetLastSector,args)
    def Reset(*args): return apply(_blcelc.iLightIterator_Reset,args)
    def __del__(self, destroy= _blcelc.delete_iLightIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightIterator instance at %s>" % (self.this,)

class iLightIteratorPtr(iLightIterator):
    def __init__(self,this):
        _swig_setattr(self, iLightIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iLightIterator, 'thisown', 0)
        _swig_setattr(self, iLightIterator,self.__class__,iLightIterator)
_blcelc.iLightIterator_swigregister(iLightIteratorPtr)

class iStatLight(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iStatLight, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iStatLight, name)
    def GetPrivateObject(*args): return apply(_blcelc.iStatLight_GetPrivateObject,args)
    def QueryObject(*args): return apply(_blcelc.iStatLight_QueryObject,args)
    def QueryLight(*args): return apply(_blcelc.iStatLight_QueryLight,args)
    def AddAffectedLightingInfo(*args): return apply(_blcelc.iStatLight_AddAffectedLightingInfo,args)
    def __del__(self, destroy= _blcelc.delete_iStatLight):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStatLight_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStatLight_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStatLight instance at %s>" % (self.this,)

class iStatLightPtr(iStatLight):
    def __init__(self,this):
        _swig_setattr(self, iStatLight, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iStatLight, 'thisown', 0)
        _swig_setattr(self, iStatLight,self.__class__,iStatLight)
_blcelc.iStatLight_swigregister(iStatLightPtr)
iStatLight_scfGetVersion = _blcelc.iStatLight_scfGetVersion


class iSectorCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSectorCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSectorCallback, name)
    def Traverse(*args): return apply(_blcelc.iSectorCallback_Traverse,args)
    def __del__(self, destroy= _blcelc.delete_iSectorCallback):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSectorCallback instance at %s>" % (self.this,)

class iSectorCallbackPtr(iSectorCallback):
    def __init__(self,this):
        _swig_setattr(self, iSectorCallback, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSectorCallback, 'thisown', 0)
        _swig_setattr(self, iSectorCallback,self.__class__,iSectorCallback)
_blcelc.iSectorCallback_swigregister(iSectorCallbackPtr)

class iSector(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSector, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSector, name)
    def GetPrivateObject(*args): return apply(_blcelc.iSector_GetPrivateObject,args)
    def QueryObject(*args): return apply(_blcelc.iSector_QueryObject,args)
    def HasFog(*args): return apply(_blcelc.iSector_HasFog,args)
    def GetFog(*args): return apply(_blcelc.iSector_GetFog,args)
    def SetFog(*args): return apply(_blcelc.iSector_SetFog,args)
    def DisableFog(*args): return apply(_blcelc.iSector_DisableFog,args)
    def GetMeshes(*args): return apply(_blcelc.iSector_GetMeshes,args)
    def GetLights(*args): return apply(_blcelc.iSector_GetLights,args)
    def ShineLights(*args): return apply(_blcelc.iSector_ShineLights,args)
    def SetDynamicAmbientLight(*args): return apply(_blcelc.iSector_SetDynamicAmbientLight,args)
    def GetDynamicAmbientLight(*args): return apply(_blcelc.iSector_GetDynamicAmbientLight,args)
    def CalculateSectorBBox(*args): return apply(_blcelc.iSector_CalculateSectorBBox,args)
    def SetVisibilityCullerPlugin(*args): return apply(_blcelc.iSector_SetVisibilityCullerPlugin,args)
    def GetVisibilityCuller(*args): return apply(_blcelc.iSector_GetVisibilityCuller,args)
    def GetRecLevel(*args): return apply(_blcelc.iSector_GetRecLevel,args)
    def HitBeamPortals(*args): return apply(_blcelc.iSector_HitBeamPortals,args)
    def HitBeam(*args): return apply(_blcelc.iSector_HitBeam,args)
    def FollowSegment(*args): return apply(_blcelc.iSector_FollowSegment,args)
    def Draw(*args): return apply(_blcelc.iSector_Draw,args)
    def PrepareDraw(*args): return apply(_blcelc.iSector_PrepareDraw,args)
    def GetVisibleMeshes(*args): return apply(_blcelc.iSector_GetVisibleMeshes,args)
    def SetSectorCallback(*args): return apply(_blcelc.iSector_SetSectorCallback,args)
    def RemoveSectorCallback(*args): return apply(_blcelc.iSector_RemoveSectorCallback,args)
    def GetSectorCallbackCount(*args): return apply(_blcelc.iSector_GetSectorCallbackCount,args)
    def GetSectorCallback(*args): return apply(_blcelc.iSector_GetSectorCallback,args)
    def CheckFrustum(*args): return apply(_blcelc.iSector_CheckFrustum,args)
    def GetPortalMeshes(*args): return apply(_blcelc.iSector_GetPortalMeshes,args)
    def RegisterPortalMesh(*args): return apply(_blcelc.iSector_RegisterPortalMesh,args)
    def UnregisterPortalMesh(*args): return apply(_blcelc.iSector_UnregisterPortalMesh,args)
    def __del__(self, destroy= _blcelc.delete_iSector):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSector_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSector_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSector instance at %s>" % (self.this,)

class iSectorPtr(iSector):
    def __init__(self,this):
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
    def GetCount(*args): return apply(_blcelc.iSectorList_GetCount,args)
    def Get(*args): return apply(_blcelc.iSectorList_Get,args)
    def Add(*args): return apply(_blcelc.iSectorList_Add,args)
    def Remove(*args): return apply(_blcelc.iSectorList_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iSectorList_RemoveAll,args)
    def Find(*args): return apply(_blcelc.iSectorList_Find,args)
    def FindByName(*args): return apply(_blcelc.iSectorList_FindByName,args)
    def __del__(self, destroy= _blcelc.delete_iSectorList):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSectorList_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSectorList_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSectorList instance at %s>" % (self.this,)

class iSectorListPtr(iSectorList):
    def __init__(self,this):
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
    def HasNext(*args): return apply(_blcelc.iSectorIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.iSectorIterator_Next,args)
    def GetLastPosition(*args): return apply(_blcelc.iSectorIterator_GetLastPosition,args)
    def Reset(*args): return apply(_blcelc.iSectorIterator_Reset,args)
    def __del__(self, destroy= _blcelc.delete_iSectorIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSectorIterator instance at %s>" % (self.this,)

class iSectorIteratorPtr(iSectorIterator):
    def __init__(self,this):
        _swig_setattr(self, iSectorIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSectorIterator, 'thisown', 0)
        _swig_setattr(self, iSectorIterator,self.__class__,iSectorIterator)
_blcelc.iSectorIterator_swigregister(iSectorIteratorPtr)

CS_NLIGHT_SHADOWS = _blcelc.CS_NLIGHT_SHADOWS
CS_NLIGHT_STATIC = _blcelc.CS_NLIGHT_STATIC
CS_NLIGHT_DYNAMIC = _blcelc.CS_NLIGHT_DYNAMIC
CS_NLIGHT_NEARBYSECTORS = _blcelc.CS_NLIGHT_NEARBYSECTORS
CS_ENGINE_CACHE_READ = _blcelc.CS_ENGINE_CACHE_READ
CS_ENGINE_CACHE_WRITE = _blcelc.CS_ENGINE_CACHE_WRITE
CS_ENGINE_CACHE_NOUPDATE = _blcelc.CS_ENGINE_CACHE_NOUPDATE
CS_RENDPRI_NONE = _blcelc.CS_RENDPRI_NONE
CS_RENDPRI_BACK2FRONT = _blcelc.CS_RENDPRI_BACK2FRONT
CS_RENDPRI_FRONT2BACK = _blcelc.CS_RENDPRI_FRONT2BACK
class iEngine(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEngine, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEngine, name)
    def QueryObject(*args): return apply(_blcelc.iEngine_QueryObject,args)
    def Prepare(*args): return apply(_blcelc.iEngine_Prepare,args)
    def ForceRelight(*args): return apply(_blcelc.iEngine_ForceRelight,args)
    def RemoveLight(*args): return apply(_blcelc.iEngine_RemoveLight,args)
    def PrepareTextures(*args): return apply(_blcelc.iEngine_PrepareTextures,args)
    def PrepareMeshes(*args): return apply(_blcelc.iEngine_PrepareMeshes,args)
    def ShineLights(*args): return apply(_blcelc.iEngine_ShineLights,args)
    def GetTextureFormat(*args): return apply(_blcelc.iEngine_GetTextureFormat,args)
    def DeleteAll(*args): return apply(_blcelc.iEngine_DeleteAll,args)
    def RegisterRenderPriority(*args): return apply(_blcelc.iEngine_RegisterRenderPriority,args)
    def GetRenderPriority(*args): return apply(_blcelc.iEngine_GetRenderPriority,args)
    def SetRenderPriorityCamera(*args): return apply(_blcelc.iEngine_SetRenderPriorityCamera,args)
    def GetRenderPriorityCamera(*args): return apply(_blcelc.iEngine_GetRenderPriorityCamera,args)
    def GetRenderPrioritySorting(*args): return apply(_blcelc.iEngine_GetRenderPrioritySorting,args)
    def GetSkyRenderPriority(*args): return apply(_blcelc.iEngine_GetSkyRenderPriority,args)
    def GetPortalRenderPriority(*args): return apply(_blcelc.iEngine_GetPortalRenderPriority,args)
    def GetWallRenderPriority(*args): return apply(_blcelc.iEngine_GetWallRenderPriority,args)
    def GetObjectRenderPriority(*args): return apply(_blcelc.iEngine_GetObjectRenderPriority,args)
    def GetAlphaRenderPriority(*args): return apply(_blcelc.iEngine_GetAlphaRenderPriority,args)
    def ClearRenderPriorities(*args): return apply(_blcelc.iEngine_ClearRenderPriorities,args)
    def GetRenderPriorityCount(*args): return apply(_blcelc.iEngine_GetRenderPriorityCount,args)
    def GetRenderPriorityName(*args): return apply(_blcelc.iEngine_GetRenderPriorityName,args)
    def CreateBaseMaterial(*args): return apply(_blcelc.iEngine_CreateBaseMaterial,args)
    def CreateTexture(*args): return apply(_blcelc.iEngine_CreateTexture,args)
    def CreateBlackTexture(*args): return apply(_blcelc.iEngine_CreateBlackTexture,args)
    def CreateMaterial(*args): return apply(_blcelc.iEngine_CreateMaterial,args)
    def CreateSector(*args): return apply(_blcelc.iEngine_CreateSector,args)
    def CreateSectorWallsMesh(*args): return apply(_blcelc.iEngine_CreateSectorWallsMesh,args)
    def CreateThingMesh(*args): return apply(_blcelc.iEngine_CreateThingMesh,args)
    def GetSectors(*args): return apply(_blcelc.iEngine_GetSectors,args)
    def GetMeshFactories(*args): return apply(_blcelc.iEngine_GetMeshFactories,args)
    def GetMeshes(*args): return apply(_blcelc.iEngine_GetMeshes,args)
    def GetCollections(*args): return apply(_blcelc.iEngine_GetCollections,args)
    def GetCameraPositions(*args): return apply(_blcelc.iEngine_GetCameraPositions,args)
    def GetTextureList(*args): return apply(_blcelc.iEngine_GetTextureList,args)
    def GetMaterialList(*args): return apply(_blcelc.iEngine_GetMaterialList,args)
    def GetVariableList(*args): return apply(_blcelc.iEngine_GetVariableList,args)
    def CreateRegion(*args): return apply(_blcelc.iEngine_CreateRegion,args)
    def GetRegions(*args): return apply(_blcelc.iEngine_GetRegions,args)
    def FindMaterial(*args): return apply(_blcelc.iEngine_FindMaterial,args)
    def FindTexture(*args): return apply(_blcelc.iEngine_FindTexture,args)
    def FindSector(*args): return apply(_blcelc.iEngine_FindSector,args)
    def FindMeshObject(*args): return apply(_blcelc.iEngine_FindMeshObject,args)
    def FindMeshFactory(*args): return apply(_blcelc.iEngine_FindMeshFactory,args)
    def FindCameraPosition(*args): return apply(_blcelc.iEngine_FindCameraPosition,args)
    def FindCollection(*args): return apply(_blcelc.iEngine_FindCollection,args)
    def SetLightingCacheMode(*args): return apply(_blcelc.iEngine_SetLightingCacheMode,args)
    def GetLightingCacheMode(*args): return apply(_blcelc.iEngine_GetLightingCacheMode,args)
    def SetClearZBuf(*args): return apply(_blcelc.iEngine_SetClearZBuf,args)
    def GetClearZBuf(*args): return apply(_blcelc.iEngine_GetClearZBuf,args)
    def GetDefaultClearZBuf(*args): return apply(_blcelc.iEngine_GetDefaultClearZBuf,args)
    def SetClearScreen(*args): return apply(_blcelc.iEngine_SetClearScreen,args)
    def GetClearScreen(*args): return apply(_blcelc.iEngine_GetClearScreen,args)
    def GetDefaultClearScreen(*args): return apply(_blcelc.iEngine_GetDefaultClearScreen,args)
    def SetMaxLightmapSize(*args): return apply(_blcelc.iEngine_SetMaxLightmapSize,args)
    def GetMaxLightmapSize(*args): return apply(_blcelc.iEngine_GetMaxLightmapSize,args)
    def GetDefaultMaxLightmapSize(*args): return apply(_blcelc.iEngine_GetDefaultMaxLightmapSize,args)
    def GetMaxLightmapAspectRatio(*args): return apply(_blcelc.iEngine_GetMaxLightmapAspectRatio,args)
    def ResetWorldSpecificSettings(*args): return apply(_blcelc.iEngine_ResetWorldSpecificSettings,args)
    def CreateCamera(*args): return apply(_blcelc.iEngine_CreateCamera,args)
    def CreateLight(*args): return apply(_blcelc.iEngine_CreateLight,args)
    def FindLight(*args): return apply(_blcelc.iEngine_FindLight,args)
    def FindLightID(*args): return apply(_blcelc.iEngine_FindLightID,args)
    def GetLightIterator(*args): return apply(_blcelc.iEngine_GetLightIterator,args)
    def CreateDynLight(*args): return apply(_blcelc.iEngine_CreateDynLight,args)
    def RemoveDynLight(*args): return apply(_blcelc.iEngine_RemoveDynLight,args)
    def GetFirstDynLight(*args): return apply(_blcelc.iEngine_GetFirstDynLight,args)
    def GetBeginDrawFlags(*args): return apply(_blcelc.iEngine_GetBeginDrawFlags,args)
    def GetTopLevelClipper(*args): return apply(_blcelc.iEngine_GetTopLevelClipper,args)
    def CreateMeshFactory(*args): return apply(_blcelc.iEngine_CreateMeshFactory,args)
    def CreateLoaderContext(*args): return apply(_blcelc.iEngine_CreateLoaderContext,args)
    def LoadMeshFactory(*args): return apply(_blcelc.iEngine_LoadMeshFactory,args)
    def CreateMeshWrapper(*args): return apply(_blcelc.iEngine_CreateMeshWrapper,args)
    def LoadMeshWrapper(*args): return apply(_blcelc.iEngine_LoadMeshWrapper,args)
    def CreatePortalContainer(*args): return apply(_blcelc.iEngine_CreatePortalContainer,args)
    def CreatePortal(*args): return apply(_blcelc.iEngine_CreatePortal,args)
    def Draw(*args): return apply(_blcelc.iEngine_Draw,args)
    def SetContext(*args): return apply(_blcelc.iEngine_SetContext,args)
    def GetContext(*args): return apply(_blcelc.iEngine_GetContext,args)
    def SetAmbientLight(*args): return apply(_blcelc.iEngine_SetAmbientLight,args)
    def GetAmbientLight(*args): return apply(_blcelc.iEngine_GetAmbientLight,args)
    def GetNearbyLights(*args): return apply(_blcelc.iEngine_GetNearbyLights,args)
    def GetNearbySectors(*args): return apply(_blcelc.iEngine_GetNearbySectors,args)
    def GetNearbyObjects(*args): return apply(_blcelc.iEngine_GetNearbyObjects,args)
    def GetNearbyMeshes(*args): return apply(_blcelc.iEngine_GetNearbyMeshes,args)
    def GetVisibleObjects(*args): return apply(_blcelc.iEngine_GetVisibleObjects,args)
    def GetVisibleMeshes(*args): return apply(_blcelc.iEngine_GetVisibleMeshes,args)
    def RemoveObject(*args): return apply(_blcelc.iEngine_RemoveObject,args)
    def SetCacheManager(*args): return apply(_blcelc.iEngine_SetCacheManager,args)
    def GetCacheManager(*args): return apply(_blcelc.iEngine_GetCacheManager,args)
    def GetDefaultAmbientLight(*args): return apply(_blcelc.iEngine_GetDefaultAmbientLight,args)
    def CreateFrustumView(*args): return apply(_blcelc.iEngine_CreateFrustumView,args)
    def CreateObjectWatcher(*args): return apply(_blcelc.iEngine_CreateObjectWatcher,args)
    def WantToDie(*args): return apply(_blcelc.iEngine_WantToDie,args)
    def GetRenderLoopManager(*args): return apply(_blcelc.iEngine_GetRenderLoopManager,args)
    def GetCurrentDefaultRenderloop(*args): return apply(_blcelc.iEngine_GetCurrentDefaultRenderloop,args)
    def SetCurrentDefaultRenderloop(*args): return apply(_blcelc.iEngine_SetCurrentDefaultRenderloop,args)
    def GetCurrentFrameNumber(*args): return apply(_blcelc.iEngine_GetCurrentFrameNumber,args)
    def __del__(self, destroy= _blcelc.delete_iEngine):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iEngine_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iEngine_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEngine instance at %s>" % (self.this,)

class iEnginePtr(iEngine):
    def __init__(self,this):
        _swig_setattr(self, iEngine, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEngine, 'thisown', 0)
        _swig_setattr(self, iEngine,self.__class__,iEngine)
_blcelc.iEngine_swigregister(iEnginePtr)
iEngine_scfGetVersion = _blcelc.iEngine_scfGetVersion


class iCamera(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCamera, name)
    def Clone(*args): return apply(_blcelc.iCamera_Clone,args)
    def GetFOV(*args): return apply(_blcelc.iCamera_GetFOV,args)
    def GetInvFOV(*args): return apply(_blcelc.iCamera_GetInvFOV,args)
    def GetFOVAngle(*args): return apply(_blcelc.iCamera_GetFOVAngle,args)
    def SetFOV(*args): return apply(_blcelc.iCamera_SetFOV,args)
    def SetFOVAngle(*args): return apply(_blcelc.iCamera_SetFOVAngle,args)
    def GetShiftX(*args): return apply(_blcelc.iCamera_GetShiftX,args)
    def GetShiftY(*args): return apply(_blcelc.iCamera_GetShiftY,args)
    def SetPerspectiveCenter(*args): return apply(_blcelc.iCamera_SetPerspectiveCenter,args)
    def GetTransform(*args): return apply(_blcelc.iCamera_GetTransform,args)
    def SetTransform(*args): return apply(_blcelc.iCamera_SetTransform,args)
    def MoveWorld(*args): return apply(_blcelc.iCamera_MoveWorld,args)
    def Move(*args): return apply(_blcelc.iCamera_Move,args)
    def MoveWorldUnrestricted(*args): return apply(_blcelc.iCamera_MoveWorldUnrestricted,args)
    def MoveUnrestricted(*args): return apply(_blcelc.iCamera_MoveUnrestricted,args)
    def GetSector(*args): return apply(_blcelc.iCamera_GetSector,args)
    def SetSector(*args): return apply(_blcelc.iCamera_SetSector,args)
    def Correct(*args): return apply(_blcelc.iCamera_Correct,args)
    def IsMirrored(*args): return apply(_blcelc.iCamera_IsMirrored,args)
    def SetMirrored(*args): return apply(_blcelc.iCamera_SetMirrored,args)
    def GetFarPlane(*args): return apply(_blcelc.iCamera_GetFarPlane,args)
    def SetFarPlane(*args): return apply(_blcelc.iCamera_SetFarPlane,args)
    def GetCameraNumber(*args): return apply(_blcelc.iCamera_GetCameraNumber,args)
    def Perspective(*args): return apply(_blcelc.iCamera_Perspective,args)
    def InvPerspective(*args): return apply(_blcelc.iCamera_InvPerspective,args)
    def OnlyPortals(*args): return apply(_blcelc.iCamera_OnlyPortals,args)
    def GetOnlyPortals(*args): return apply(_blcelc.iCamera_GetOnlyPortals,args)
    def __del__(self, destroy= _blcelc.delete_iCamera):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCamera_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCamera_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCamera instance at %s>" % (self.this,)

class iCameraPtr(iCamera):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iCameraPosition_QueryObject,args)
    def Clone(*args): return apply(_blcelc.iCameraPosition_Clone,args)
    def GetSector(*args): return apply(_blcelc.iCameraPosition_GetSector,args)
    def SetSector(*args): return apply(_blcelc.iCameraPosition_SetSector,args)
    def GetPosition(*args): return apply(_blcelc.iCameraPosition_GetPosition,args)
    def SetPosition(*args): return apply(_blcelc.iCameraPosition_SetPosition,args)
    def GetUpwardVector(*args): return apply(_blcelc.iCameraPosition_GetUpwardVector,args)
    def SetUpwardVector(*args): return apply(_blcelc.iCameraPosition_SetUpwardVector,args)
    def GetForwardVector(*args): return apply(_blcelc.iCameraPosition_GetForwardVector,args)
    def SetForwardVector(*args): return apply(_blcelc.iCameraPosition_SetForwardVector,args)
    def Set(*args): return apply(_blcelc.iCameraPosition_Set,args)
    def Load(*args): return apply(_blcelc.iCameraPosition_Load,args)
    def SetFarPlane(*args): return apply(_blcelc.iCameraPosition_SetFarPlane,args)
    def ClearFarPlane(*args): return apply(_blcelc.iCameraPosition_ClearFarPlane,args)
    def GetFarPlane(*args): return apply(_blcelc.iCameraPosition_GetFarPlane,args)
    def __del__(self, destroy= _blcelc.delete_iCameraPosition):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCameraPosition_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCameraPosition_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCameraPosition instance at %s>" % (self.this,)

class iCameraPositionPtr(iCameraPosition):
    def __init__(self,this):
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
    def NewCameraPosition(*args): return apply(_blcelc.iCameraPositionList_NewCameraPosition,args)
    def GetCount(*args): return apply(_blcelc.iCameraPositionList_GetCount,args)
    def Get(*args): return apply(_blcelc.iCameraPositionList_Get,args)
    def Add(*args): return apply(_blcelc.iCameraPositionList_Add,args)
    def Remove(*args): return apply(_blcelc.iCameraPositionList_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iCameraPositionList_RemoveAll,args)
    def Find(*args): return apply(_blcelc.iCameraPositionList_Find,args)
    def FindByName(*args): return apply(_blcelc.iCameraPositionList_FindByName,args)
    def __del__(self, destroy= _blcelc.delete_iCameraPositionList):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCameraPositionList instance at %s>" % (self.this,)

class iCameraPositionListPtr(iCameraPositionList):
    def __init__(self,this):
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
    def UseTexture(*args): return apply(_blcelc.iTextureCallback_UseTexture,args)
    def __del__(self, destroy= _blcelc.delete_iTextureCallback):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureCallback instance at %s>" % (self.this,)

class iTextureCallbackPtr(iTextureCallback):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iTextureWrapper_QueryObject,args)
    def Clone(*args): return apply(_blcelc.iTextureWrapper_Clone,args)
    def SetImageFile(*args): return apply(_blcelc.iTextureWrapper_SetImageFile,args)
    def GetImageFile(*args): return apply(_blcelc.iTextureWrapper_GetImageFile,args)
    def SetTextureHandle(*args): return apply(_blcelc.iTextureWrapper_SetTextureHandle,args)
    def GetTextureHandle(*args): return apply(_blcelc.iTextureWrapper_GetTextureHandle,args)
    def SetKeyColor(*args): return apply(_blcelc.iTextureWrapper_SetKeyColor,args)
    def GetKeyColor(*args): return apply(_blcelc.iTextureWrapper_GetKeyColor,args)
    def SetFlags(*args): return apply(_blcelc.iTextureWrapper_SetFlags,args)
    def GetFlags(*args): return apply(_blcelc.iTextureWrapper_GetFlags,args)
    def Register(*args): return apply(_blcelc.iTextureWrapper_Register,args)
    def SetUseCallback(*args): return apply(_blcelc.iTextureWrapper_SetUseCallback,args)
    def GetUseCallback(*args): return apply(_blcelc.iTextureWrapper_GetUseCallback,args)
    def Visit(*args): return apply(_blcelc.iTextureWrapper_Visit,args)
    def IsVisitRequired(*args): return apply(_blcelc.iTextureWrapper_IsVisitRequired,args)
    def SetKeepImage(*args): return apply(_blcelc.iTextureWrapper_SetKeepImage,args)
    def KeepImage(*args): return apply(_blcelc.iTextureWrapper_KeepImage,args)
    def __del__(self, destroy= _blcelc.delete_iTextureWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iTextureWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iTextureWrapper_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureWrapper instance at %s>" % (self.this,)

class iTextureWrapperPtr(iTextureWrapper):
    def __init__(self,this):
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
    def NewTexture(*args): return apply(_blcelc.iTextureList_NewTexture,args)
    def GetCount(*args): return apply(_blcelc.iTextureList_GetCount,args)
    def Get(*args): return apply(_blcelc.iTextureList_Get,args)
    def Add(*args): return apply(_blcelc.iTextureList_Add,args)
    def Remove(*args): return apply(_blcelc.iTextureList_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iTextureList_RemoveAll,args)
    def Find(*args): return apply(_blcelc.iTextureList_Find,args)
    def FindByName(*args): return apply(_blcelc.iTextureList_FindByName,args)
    def __del__(self, destroy= _blcelc.delete_iTextureList):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iTextureList_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iTextureList_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureList instance at %s>" % (self.this,)

class iTextureListPtr(iTextureList):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iMaterialWrapper_QueryObject,args)
    def Clone(*args): return apply(_blcelc.iMaterialWrapper_Clone,args)
    def SetMaterialHandle(*args): return apply(_blcelc.iMaterialWrapper_SetMaterialHandle,args)
    def GetMaterialHandle(*args): return apply(_blcelc.iMaterialWrapper_GetMaterialHandle,args)
    def SetMaterial(*args): return apply(_blcelc.iMaterialWrapper_SetMaterial,args)
    def GetMaterial(*args): return apply(_blcelc.iMaterialWrapper_GetMaterial,args)
    def Register(*args): return apply(_blcelc.iMaterialWrapper_Register,args)
    def Visit(*args): return apply(_blcelc.iMaterialWrapper_Visit,args)
    def IsVisitRequired(*args): return apply(_blcelc.iMaterialWrapper_IsVisitRequired,args)
    def __del__(self, destroy= _blcelc.delete_iMaterialWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMaterialWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMaterialWrapper_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialWrapper instance at %s>" % (self.this,)

class iMaterialWrapperPtr(iMaterialWrapper):
    def __init__(self,this):
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
    def GetTextureWrapper(*args): return apply(_blcelc.iMaterialEngine_GetTextureWrapper,args)
    def Visit(*args): return apply(_blcelc.iMaterialEngine_Visit,args)
    def IsVisitRequired(*args): return apply(_blcelc.iMaterialEngine_IsVisitRequired,args)
    def __del__(self, destroy= _blcelc.delete_iMaterialEngine):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialEngine instance at %s>" % (self.this,)

class iMaterialEnginePtr(iMaterialEngine):
    def __init__(self,this):
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
    def NewMaterial(*args): return apply(_blcelc.iMaterialList_NewMaterial,args)
    def GetCount(*args): return apply(_blcelc.iMaterialList_GetCount,args)
    def Get(*args): return apply(_blcelc.iMaterialList_Get,args)
    def Add(*args): return apply(_blcelc.iMaterialList_Add,args)
    def Remove(*args): return apply(_blcelc.iMaterialList_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iMaterialList_RemoveAll,args)
    def Find(*args): return apply(_blcelc.iMaterialList_Find,args)
    def FindByName(*args): return apply(_blcelc.iMaterialList_FindByName,args)
    def __del__(self, destroy= _blcelc.delete_iMaterialList):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialList instance at %s>" % (self.this,)

class iMaterialListPtr(iMaterialList):
    def __init__(self,this):
        _swig_setattr(self, iMaterialList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMaterialList, 'thisown', 0)
        _swig_setattr(self, iMaterialList,self.__class__,iMaterialList)
_blcelc.iMaterialList_swigregister(iMaterialListPtr)

CS_ENTITY_DETAIL = _blcelc.CS_ENTITY_DETAIL
CS_ENTITY_CAMERA = _blcelc.CS_ENTITY_CAMERA
CS_ENTITY_INVISIBLE = _blcelc.CS_ENTITY_INVISIBLE
CS_ENTITY_NOSHADOWS = _blcelc.CS_ENTITY_NOSHADOWS
CS_ENTITY_NOLIGHTING = _blcelc.CS_ENTITY_NOLIGHTING
class iMeshDrawCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshDrawCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshDrawCallback, name)
    def BeforeDrawing(*args): return apply(_blcelc.iMeshDrawCallback_BeforeDrawing,args)
    def __del__(self, destroy= _blcelc.delete_iMeshDrawCallback):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshDrawCallback instance at %s>" % (self.this,)

class iMeshDrawCallbackPtr(iMeshDrawCallback):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iMeshWrapper_QueryObject,args)
    def GetMeshObject(*args): return apply(_blcelc.iMeshWrapper_GetMeshObject,args)
    def SetMeshObject(*args): return apply(_blcelc.iMeshWrapper_SetMeshObject,args)
    def GetPortalContainer(*args): return apply(_blcelc.iMeshWrapper_GetPortalContainer,args)
    def GetLightingInfo(*args): return apply(_blcelc.iMeshWrapper_GetLightingInfo,args)
    def GetShadowReceiver(*args): return apply(_blcelc.iMeshWrapper_GetShadowReceiver,args)
    def GetVisibilityNumber(*args): return apply(_blcelc.iMeshWrapper_GetVisibilityNumber,args)
    def GetFactory(*args): return apply(_blcelc.iMeshWrapper_GetFactory,args)
    def SetFactory(*args): return apply(_blcelc.iMeshWrapper_SetFactory,args)
    def DeferUpdateLighting(*args): return apply(_blcelc.iMeshWrapper_DeferUpdateLighting,args)
    def UpdateLighting(*args): return apply(_blcelc.iMeshWrapper_UpdateLighting,args)
    def GetMovable(*args): return apply(_blcelc.iMeshWrapper_GetMovable,args)
    def PlaceMesh(*args): return apply(_blcelc.iMeshWrapper_PlaceMesh,args)
    def HitBeamBBox(*args): return apply(_blcelc.iMeshWrapper_HitBeamBBox,args)
    def HitBeamOutline(*args): return apply(_blcelc.iMeshWrapper_HitBeamOutline,args)
    def HitBeamObject(*args): return apply(_blcelc.iMeshWrapper_HitBeamObject,args)
    def HitBeam(*args): return apply(_blcelc.iMeshWrapper_HitBeam,args)
    def SetDrawCallback(*args): return apply(_blcelc.iMeshWrapper_SetDrawCallback,args)
    def RemoveDrawCallback(*args): return apply(_blcelc.iMeshWrapper_RemoveDrawCallback,args)
    def GetDrawCallbackCount(*args): return apply(_blcelc.iMeshWrapper_GetDrawCallbackCount,args)
    def GetDrawCallback(*args): return apply(_blcelc.iMeshWrapper_GetDrawCallback,args)
    def SetRenderPriority(*args): return apply(_blcelc.iMeshWrapper_SetRenderPriority,args)
    def GetRenderPriority(*args): return apply(_blcelc.iMeshWrapper_GetRenderPriority,args)
    def GetFlags(*args): return apply(_blcelc.iMeshWrapper_GetFlags,args)
    def SetZBufMode(*args): return apply(_blcelc.iMeshWrapper_SetZBufMode,args)
    def GetZBufMode(*args): return apply(_blcelc.iMeshWrapper_GetZBufMode,args)
    def HardTransform(*args): return apply(_blcelc.iMeshWrapper_HardTransform,args)
    def GetWorldBoundingBox(*args): return apply(_blcelc.iMeshWrapper_GetWorldBoundingBox,args)
    def GetTransformedBoundingBox(*args): return apply(_blcelc.iMeshWrapper_GetTransformedBoundingBox,args)
    def GetScreenBoundingBox(*args): return apply(_blcelc.iMeshWrapper_GetScreenBoundingBox,args)
    def GetChildren(*args): return apply(_blcelc.iMeshWrapper_GetChildren,args)
    def GetParentContainer(*args): return apply(_blcelc.iMeshWrapper_GetParentContainer,args)
    def SetParentContainer(*args): return apply(_blcelc.iMeshWrapper_SetParentContainer,args)
    def GetRadius(*args): return apply(_blcelc.iMeshWrapper_GetRadius,args)
    def Draw(*args): return apply(_blcelc.iMeshWrapper_Draw,args)
    def CreateStaticLOD(*args): return apply(_blcelc.iMeshWrapper_CreateStaticLOD,args)
    def DestroyStaticLOD(*args): return apply(_blcelc.iMeshWrapper_DestroyStaticLOD,args)
    def GetStaticLOD(*args): return apply(_blcelc.iMeshWrapper_GetStaticLOD,args)
    def AddMeshToStaticLOD(*args): return apply(_blcelc.iMeshWrapper_AddMeshToStaticLOD,args)
    def RemoveMeshFromStaticLOD(*args): return apply(_blcelc.iMeshWrapper_RemoveMeshFromStaticLOD,args)
    def GetRenderMeshes(*args): return apply(_blcelc.iMeshWrapper_GetRenderMeshes,args)
    def DrawShadow(*args): return apply(_blcelc.iMeshWrapper_DrawShadow,args)
    def DrawLight(*args): return apply(_blcelc.iMeshWrapper_DrawLight,args)
    def CastHardwareShadow(*args): return apply(_blcelc.iMeshWrapper_CastHardwareShadow,args)
    def SetDrawAfterShadow(*args): return apply(_blcelc.iMeshWrapper_SetDrawAfterShadow,args)
    def GetDrawAfterShadow(*args): return apply(_blcelc.iMeshWrapper_GetDrawAfterShadow,args)
    def __del__(self, destroy= _blcelc.delete_iMeshWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshWrapper_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshWrapper instance at %s>" % (self.this,)

class iMeshWrapperPtr(iMeshWrapper):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iMeshFactoryWrapper_QueryObject,args)
    def GetMeshObjectFactory(*args): return apply(_blcelc.iMeshFactoryWrapper_GetMeshObjectFactory,args)
    def SetMeshObjectFactory(*args): return apply(_blcelc.iMeshFactoryWrapper_SetMeshObjectFactory,args)
    def HardTransform(*args): return apply(_blcelc.iMeshFactoryWrapper_HardTransform,args)
    def CreateMeshWrapper(*args): return apply(_blcelc.iMeshFactoryWrapper_CreateMeshWrapper,args)
    def GetParentContainer(*args): return apply(_blcelc.iMeshFactoryWrapper_GetParentContainer,args)
    def SetParentContainer(*args): return apply(_blcelc.iMeshFactoryWrapper_SetParentContainer,args)
    def GetChildren(*args): return apply(_blcelc.iMeshFactoryWrapper_GetChildren,args)
    def GetTransform(*args): return apply(_blcelc.iMeshFactoryWrapper_GetTransform,args)
    def SetTransform(*args): return apply(_blcelc.iMeshFactoryWrapper_SetTransform,args)
    def CreateStaticLOD(*args): return apply(_blcelc.iMeshFactoryWrapper_CreateStaticLOD,args)
    def DestroyStaticLOD(*args): return apply(_blcelc.iMeshFactoryWrapper_DestroyStaticLOD,args)
    def SetStaticLOD(*args): return apply(_blcelc.iMeshFactoryWrapper_SetStaticLOD,args)
    def GetStaticLOD(*args): return apply(_blcelc.iMeshFactoryWrapper_GetStaticLOD,args)
    def AddFactoryToStaticLOD(*args): return apply(_blcelc.iMeshFactoryWrapper_AddFactoryToStaticLOD,args)
    def RemoveFactoryFromStaticLOD(*args): return apply(_blcelc.iMeshFactoryWrapper_RemoveFactoryFromStaticLOD,args)
    def __del__(self, destroy= _blcelc.delete_iMeshFactoryWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshFactoryWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshFactoryWrapper_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshFactoryWrapper instance at %s>" % (self.this,)

class iMeshFactoryWrapperPtr(iMeshFactoryWrapper):
    def __init__(self,this):
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
    def GetCount(*args): return apply(_blcelc.iMeshList_GetCount,args)
    def Get(*args): return apply(_blcelc.iMeshList_Get,args)
    def Add(*args): return apply(_blcelc.iMeshList_Add,args)
    def Remove(*args): return apply(_blcelc.iMeshList_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iMeshList_RemoveAll,args)
    def Find(*args): return apply(_blcelc.iMeshList_Find,args)
    def FindByName(*args): return apply(_blcelc.iMeshList_FindByName,args)
    def __del__(self, destroy= _blcelc.delete_iMeshList):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshList instance at %s>" % (self.this,)

class iMeshListPtr(iMeshList):
    def __init__(self,this):
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
    def GetCount(*args): return apply(_blcelc.iMeshFactoryList_GetCount,args)
    def Get(*args): return apply(_blcelc.iMeshFactoryList_Get,args)
    def Add(*args): return apply(_blcelc.iMeshFactoryList_Add,args)
    def Remove(*args): return apply(_blcelc.iMeshFactoryList_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iMeshFactoryList_RemoveAll,args)
    def Find(*args): return apply(_blcelc.iMeshFactoryList_Find,args)
    def FindByName(*args): return apply(_blcelc.iMeshFactoryList_FindByName,args)
    def __del__(self, destroy= _blcelc.delete_iMeshFactoryList):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshFactoryList instance at %s>" % (self.this,)

class iMeshFactoryListPtr(iMeshFactoryList):
    def __init__(self,this):
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
    def Next(*args): return apply(_blcelc.iMeshWrapperIterator_Next,args)
    def Reset(*args): return apply(_blcelc.iMeshWrapperIterator_Reset,args)
    def HasNext(*args): return apply(_blcelc.iMeshWrapperIterator_HasNext,args)
    def __del__(self, destroy= _blcelc.delete_iMeshWrapperIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshWrapperIterator instance at %s>" % (self.this,)

class iMeshWrapperIteratorPtr(iMeshWrapperIterator):
    def __init__(self,this):
        _swig_setattr(self, iMeshWrapperIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshWrapperIterator, 'thisown', 0)
        _swig_setattr(self, iMeshWrapperIterator,self.__class__,iMeshWrapperIterator)
_blcelc.iMeshWrapperIterator_swigregister(iMeshWrapperIteratorPtr)

class iMovableListener(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMovableListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMovableListener, name)
    def MovableChanged(*args): return apply(_blcelc.iMovableListener_MovableChanged,args)
    def MovableDestroyed(*args): return apply(_blcelc.iMovableListener_MovableDestroyed,args)
    def __del__(self, destroy= _blcelc.delete_iMovableListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMovableListener_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMovableListener_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMovableListener instance at %s>" % (self.this,)

class iMovableListenerPtr(iMovableListener):
    def __init__(self,this):
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
    def GetParent(*args): return apply(_blcelc.iMovable_GetParent,args)
    def SetParent(*args): return apply(_blcelc.iMovable_SetParent,args)
    def SetSector(*args): return apply(_blcelc.iMovable_SetSector,args)
    def ClearSectors(*args): return apply(_blcelc.iMovable_ClearSectors,args)
    def GetSectors(*args): return apply(_blcelc.iMovable_GetSectors,args)
    def InSector(*args): return apply(_blcelc.iMovable_InSector,args)
    def SetPosition(*args): return apply(_blcelc.iMovable_SetPosition,args)
    def GetPosition(*args): return apply(_blcelc.iMovable_GetPosition,args)
    def GetFullPosition(*args): return apply(_blcelc.iMovable_GetFullPosition,args)
    def SetTransform(*args): return apply(_blcelc.iMovable_SetTransform,args)
    def GetTransform(*args): return apply(_blcelc.iMovable_GetTransform,args)
    def GetFullTransform(*args): return apply(_blcelc.iMovable_GetFullTransform,args)
    def MovePosition(*args): return apply(_blcelc.iMovable_MovePosition,args)
    def Transform(*args): return apply(_blcelc.iMovable_Transform,args)
    def AddListener(*args): return apply(_blcelc.iMovable_AddListener,args)
    def RemoveListener(*args): return apply(_blcelc.iMovable_RemoveListener,args)
    def UpdateMove(*args): return apply(_blcelc.iMovable_UpdateMove,args)
    def GetUpdateNumber(*args): return apply(_blcelc.iMovable_GetUpdateNumber,args)
    def IsTransformIdentity(*args): return apply(_blcelc.iMovable_IsTransformIdentity,args)
    def IsFullTransformIdentity(*args): return apply(_blcelc.iMovable_IsFullTransformIdentity,args)
    def TransformIdentity(*args): return apply(_blcelc.iMovable_TransformIdentity,args)
    def __del__(self, destroy= _blcelc.delete_iMovable):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMovable_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMovable_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMovable instance at %s>" % (self.this,)

class iMovablePtr(iMovable):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iRegion_QueryObject,args)
    def Add(*args): return apply(_blcelc.iRegion_Add,args)
    def Clear(*args): return apply(_blcelc.iRegion_Clear,args)
    def DeleteAll(*args): return apply(_blcelc.iRegion_DeleteAll,args)
    def PrepareTextures(*args): return apply(_blcelc.iRegion_PrepareTextures,args)
    def ShineLights(*args): return apply(_blcelc.iRegion_ShineLights,args)
    def Prepare(*args): return apply(_blcelc.iRegion_Prepare,args)
    def FindSector(*args): return apply(_blcelc.iRegion_FindSector,args)
    def FindMeshObject(*args): return apply(_blcelc.iRegion_FindMeshObject,args)
    def FindMeshFactory(*args): return apply(_blcelc.iRegion_FindMeshFactory,args)
    def FindTexture(*args): return apply(_blcelc.iRegion_FindTexture,args)
    def FindMaterial(*args): return apply(_blcelc.iRegion_FindMaterial,args)
    def FindCameraPosition(*args): return apply(_blcelc.iRegion_FindCameraPosition,args)
    def FindCollection(*args): return apply(_blcelc.iRegion_FindCollection,args)
    def IsInRegion(*args): return apply(_blcelc.iRegion_IsInRegion,args)
    def __del__(self, destroy= _blcelc.delete_iRegion):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iRegion instance at %s>" % (self.this,)

class iRegionPtr(iRegion):
    def __init__(self,this):
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
    def GetCount(*args): return apply(_blcelc.iRegionList_GetCount,args)
    def Get(*args): return apply(_blcelc.iRegionList_Get,args)
    def Add(*args): return apply(_blcelc.iRegionList_Add,args)
    def Remove(*args): return apply(_blcelc.iRegionList_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iRegionList_RemoveAll,args)
    def Find(*args): return apply(_blcelc.iRegionList_Find,args)
    def FindByName(*args): return apply(_blcelc.iRegionList_FindByName,args)
    def __del__(self, destroy= _blcelc.delete_iRegionList):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iRegionList instance at %s>" % (self.this,)

class iRegionListPtr(iRegionList):
    def __init__(self,this):
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
    def HasNext(*args): return apply(_blcelc.iVisibilityObjectIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.iVisibilityObjectIterator_Next,args)
    def Reset(*args): return apply(_blcelc.iVisibilityObjectIterator_Reset,args)
    def __del__(self, destroy= _blcelc.delete_iVisibilityObjectIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVisibilityObjectIterator instance at %s>" % (self.this,)

class iVisibilityObjectIteratorPtr(iVisibilityObjectIterator):
    def __init__(self,this):
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
    def ObjectVisible(*args): return apply(_blcelc.iVisibilityCullerListener_ObjectVisible,args)
    def __del__(self, destroy= _blcelc.delete_iVisibilityCullerListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVisibilityCullerListener instance at %s>" % (self.this,)

class iVisibilityCullerListenerPtr(iVisibilityCullerListener):
    def __init__(self,this):
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
    def Setup(*args): return apply(_blcelc.iVisibilityCuller_Setup,args)
    def RegisterVisObject(*args): return apply(_blcelc.iVisibilityCuller_RegisterVisObject,args)
    def UnregisterVisObject(*args): return apply(_blcelc.iVisibilityCuller_UnregisterVisObject,args)
    def VisTest(*args): return apply(_blcelc.iVisibilityCuller_VisTest,args)
    def IntersectSegment(*args): return apply(_blcelc.iVisibilityCuller_IntersectSegment,args)
    def CastShadows(*args): return apply(_blcelc.iVisibilityCuller_CastShadows,args)
    def __del__(self, destroy= _blcelc.delete_iVisibilityCuller):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iVisibilityCuller_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iVisibilityCuller_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVisibilityCuller instance at %s>" % (self.this,)

class iVisibilityCullerPtr(iVisibilityCuller):
    def __init__(self,this):
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
    def GetMovable(*args): return apply(_blcelc.iVisibilityObject_GetMovable,args)
    def GetMeshWrapper(*args): return apply(_blcelc.iVisibilityObject_GetMeshWrapper,args)
    def SetVisibilityNumber(*args): return apply(_blcelc.iVisibilityObject_SetVisibilityNumber,args)
    def GetVisibilityNumber(*args): return apply(_blcelc.iVisibilityObject_GetVisibilityNumber,args)
    def GetObjectModel(*args): return apply(_blcelc.iVisibilityObject_GetObjectModel,args)
    def GetCullerFlags(*args): return apply(_blcelc.iVisibilityObject_GetCullerFlags,args)
    def __del__(self, destroy= _blcelc.delete_iVisibilityObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVisibilityObject instance at %s>" % (self.this,)

class iVisibilityObjectPtr(iVisibilityObject):
    def __init__(self,this):
        _swig_setattr(self, iVisibilityObject, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVisibilityObject, 'thisown', 0)
        _swig_setattr(self, iVisibilityObject,self.__class__,iVisibilityObject)
_blcelc.iVisibilityObject_swigregister(iVisibilityObjectPtr)

class csModelConverterFormat(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csModelConverterFormat, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csModelConverterFormat, name)
    __swig_setmethods__["Name"] = _blcelc.csModelConverterFormat_Name_set
    __swig_getmethods__["Name"] = _blcelc.csModelConverterFormat_Name_get
    if _newclass:Name = property(_blcelc.csModelConverterFormat_Name_get,_blcelc.csModelConverterFormat_Name_set)
    __swig_setmethods__["CanLoad"] = _blcelc.csModelConverterFormat_CanLoad_set
    __swig_getmethods__["CanLoad"] = _blcelc.csModelConverterFormat_CanLoad_get
    if _newclass:CanLoad = property(_blcelc.csModelConverterFormat_CanLoad_get,_blcelc.csModelConverterFormat_CanLoad_set)
    __swig_setmethods__["CanSave"] = _blcelc.csModelConverterFormat_CanSave_set
    __swig_getmethods__["CanSave"] = _blcelc.csModelConverterFormat_CanSave_get
    if _newclass:CanSave = property(_blcelc.csModelConverterFormat_CanSave_get,_blcelc.csModelConverterFormat_CanSave_set)
    def __init__(self,*args):
        _swig_setattr(self, csModelConverterFormat, 'this', apply(_blcelc.new_csModelConverterFormat,args))
        _swig_setattr(self, csModelConverterFormat, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csModelConverterFormat):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csModelConverterFormat instance at %s>" % (self.this,)

class csModelConverterFormatPtr(csModelConverterFormat):
    def __init__(self,this):
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
    def GetFormatCount(*args): return apply(_blcelc.iModelConverter_GetFormatCount,args)
    def GetFormat(*args): return apply(_blcelc.iModelConverter_GetFormat,args)
    def Load(*args): return apply(_blcelc.iModelConverter_Load,args)
    def Save(*args): return apply(_blcelc.iModelConverter_Save,args)
    def __del__(self, destroy= _blcelc.delete_iModelConverter):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iModelConverter_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iModelConverter_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iModelConverter instance at %s>" % (self.this,)

class iModelConverterPtr(iModelConverter):
    def __init__(self,this):
        _swig_setattr(self, iModelConverter, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iModelConverter, 'thisown', 0)
        _swig_setattr(self, iModelConverter,self.__class__,iModelConverter)
_blcelc.iModelConverter_swigregister(iModelConverterPtr)
iModelConverter_scfGetVersion = _blcelc.iModelConverter_scfGetVersion


class iMeshObjectDrawCallback(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iMeshObjectDrawCallback, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iMeshObjectDrawCallback, name)
    def BeforeDrawing(*args): return apply(_blcelc.iMeshObjectDrawCallback_BeforeDrawing,args)
    def __del__(self, destroy= _blcelc.delete_iMeshObjectDrawCallback):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshObjectDrawCallback instance at %s>" % (self.this,)

class iMeshObjectDrawCallbackPtr(iMeshObjectDrawCallback):
    def __init__(self,this):
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
    def GetFactory(*args): return apply(_blcelc.iMeshObject_GetFactory,args)
    def DrawTest(*args): return apply(_blcelc.iMeshObject_DrawTest,args)
    def UpdateLighting(*args): return apply(_blcelc.iMeshObject_UpdateLighting,args)
    def Draw(*args): return apply(_blcelc.iMeshObject_Draw,args)
    def GetRenderMeshes(*args): return apply(_blcelc.iMeshObject_GetRenderMeshes,args)
    def SetVisibleCallback(*args): return apply(_blcelc.iMeshObject_SetVisibleCallback,args)
    def GetVisibleCallback(*args): return apply(_blcelc.iMeshObject_GetVisibleCallback,args)
    def NextFrame(*args): return apply(_blcelc.iMeshObject_NextFrame,args)
    def HardTransform(*args): return apply(_blcelc.iMeshObject_HardTransform,args)
    def SupportsHardTransform(*args): return apply(_blcelc.iMeshObject_SupportsHardTransform,args)
    def HitBeamOutline(*args): return apply(_blcelc.iMeshObject_HitBeamOutline,args)
    def HitBeamObject(*args): return apply(_blcelc.iMeshObject_HitBeamObject,args)
    def SetLogicalParent(*args): return apply(_blcelc.iMeshObject_SetLogicalParent,args)
    def GetLogicalParent(*args): return apply(_blcelc.iMeshObject_GetLogicalParent,args)
    def GetObjectModel(*args): return apply(_blcelc.iMeshObject_GetObjectModel,args)
    def SetColor(*args): return apply(_blcelc.iMeshObject_SetColor,args)
    def GetColor(*args): return apply(_blcelc.iMeshObject_GetColor,args)
    def SetMaterialWrapper(*args): return apply(_blcelc.iMeshObject_SetMaterialWrapper,args)
    def GetMaterialWrapper(*args): return apply(_blcelc.iMeshObject_GetMaterialWrapper,args)
    def InvalidateMaterialHandles(*args): return apply(_blcelc.iMeshObject_InvalidateMaterialHandles,args)
    def __del__(self, destroy= _blcelc.delete_iMeshObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshObject_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshObject_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshObject instance at %s>" % (self.this,)

class iMeshObjectPtr(iMeshObject):
    def __init__(self,this):
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
    def NewInstance(*args): return apply(_blcelc.iMeshObjectFactory_NewInstance,args)
    def HardTransform(*args): return apply(_blcelc.iMeshObjectFactory_HardTransform,args)
    def SupportsHardTransform(*args): return apply(_blcelc.iMeshObjectFactory_SupportsHardTransform,args)
    def SetLogicalParent(*args): return apply(_blcelc.iMeshObjectFactory_SetLogicalParent,args)
    def GetLogicalParent(*args): return apply(_blcelc.iMeshObjectFactory_GetLogicalParent,args)
    def GetObjectModel(*args): return apply(_blcelc.iMeshObjectFactory_GetObjectModel,args)
    def __del__(self, destroy= _blcelc.delete_iMeshObjectFactory):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshObjectFactory_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshObjectFactory_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshObjectFactory instance at %s>" % (self.this,)

class iMeshObjectFactoryPtr(iMeshObjectFactory):
    def __init__(self,this):
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
    def NewFactory(*args): return apply(_blcelc.iMeshObjectType_NewFactory,args)
    def __del__(self, destroy= _blcelc.delete_iMeshObjectType):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMeshObjectType_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMeshObjectType_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMeshObjectType instance at %s>" % (self.this,)

class iMeshObjectTypePtr(iMeshObjectType):
    def __init__(self,this):
        _swig_setattr(self, iMeshObjectType, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMeshObjectType, 'thisown', 0)
        _swig_setattr(self, iMeshObjectType,self.__class__,iMeshObjectType)
_blcelc.iMeshObjectType_swigregister(iMeshObjectTypePtr)
iMeshObjectType_scfGetVersion = _blcelc.iMeshObjectType_scfGetVersion


class csSprite2DVertex(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csSprite2DVertex, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csSprite2DVertex, name)
    __swig_setmethods__["pos"] = _blcelc.csSprite2DVertex_pos_set
    __swig_getmethods__["pos"] = _blcelc.csSprite2DVertex_pos_get
    if _newclass:pos = property(_blcelc.csSprite2DVertex_pos_get,_blcelc.csSprite2DVertex_pos_set)
    __swig_setmethods__["color_init"] = _blcelc.csSprite2DVertex_color_init_set
    __swig_getmethods__["color_init"] = _blcelc.csSprite2DVertex_color_init_get
    if _newclass:color_init = property(_blcelc.csSprite2DVertex_color_init_get,_blcelc.csSprite2DVertex_color_init_set)
    __swig_setmethods__["color"] = _blcelc.csSprite2DVertex_color_set
    __swig_getmethods__["color"] = _blcelc.csSprite2DVertex_color_get
    if _newclass:color = property(_blcelc.csSprite2DVertex_color_get,_blcelc.csSprite2DVertex_color_set)
    __swig_setmethods__["u"] = _blcelc.csSprite2DVertex_u_set
    __swig_getmethods__["u"] = _blcelc.csSprite2DVertex_u_get
    if _newclass:u = property(_blcelc.csSprite2DVertex_u_get,_blcelc.csSprite2DVertex_u_set)
    __swig_setmethods__["v"] = _blcelc.csSprite2DVertex_v_set
    __swig_getmethods__["v"] = _blcelc.csSprite2DVertex_v_get
    if _newclass:v = property(_blcelc.csSprite2DVertex_v_get,_blcelc.csSprite2DVertex_v_set)
    def __init__(self,*args):
        _swig_setattr(self, csSprite2DVertex, 'this', apply(_blcelc.new_csSprite2DVertex,args))
        _swig_setattr(self, csSprite2DVertex, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csSprite2DVertex):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csSprite2DVertex instance at %s>" % (self.this,)

class csSprite2DVertexPtr(csSprite2DVertex):
    def __init__(self,this):
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
    def SetName(*args): return apply(_blcelc.iSprite2DUVAnimationFrame_SetName,args)
    def GetName(*args): return apply(_blcelc.iSprite2DUVAnimationFrame_GetName,args)
    def GetUVCoo(*args): return apply(_blcelc.iSprite2DUVAnimationFrame_GetUVCoo,args)
    def GetUVCount(*args): return apply(_blcelc.iSprite2DUVAnimationFrame_GetUVCount,args)
    def SetUV(*args): return apply(_blcelc.iSprite2DUVAnimationFrame_SetUV,args)
    def SetFrameData(*args): return apply(_blcelc.iSprite2DUVAnimationFrame_SetFrameData,args)
    def RemoveUV(*args): return apply(_blcelc.iSprite2DUVAnimationFrame_RemoveUV,args)
    def GetDuration(*args): return apply(_blcelc.iSprite2DUVAnimationFrame_GetDuration,args)
    def SetDuration(*args): return apply(_blcelc.iSprite2DUVAnimationFrame_SetDuration,args)
    def __del__(self, destroy= _blcelc.delete_iSprite2DUVAnimationFrame):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite2DUVAnimationFrame instance at %s>" % (self.this,)

class iSprite2DUVAnimationFramePtr(iSprite2DUVAnimationFrame):
    def __init__(self,this):
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
    def SetName(*args): return apply(_blcelc.iSprite2DUVAnimation_SetName,args)
    def GetName(*args): return apply(_blcelc.iSprite2DUVAnimation_GetName,args)
    def GetFrameCount(*args): return apply(_blcelc.iSprite2DUVAnimation_GetFrameCount,args)
    def GetFrame(*args): return apply(_blcelc.iSprite2DUVAnimation_GetFrame,args)
    def CreateFrame(*args): return apply(_blcelc.iSprite2DUVAnimation_CreateFrame,args)
    def MoveFrame(*args): return apply(_blcelc.iSprite2DUVAnimation_MoveFrame,args)
    def RemoveFrame(*args): return apply(_blcelc.iSprite2DUVAnimation_RemoveFrame,args)
    def __del__(self, destroy= _blcelc.delete_iSprite2DUVAnimation):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite2DUVAnimation instance at %s>" % (self.this,)

class iSprite2DUVAnimationPtr(iSprite2DUVAnimation):
    def __init__(self,this):
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
    def SetMaterialWrapper(*args): return apply(_blcelc.iSprite2DFactoryState_SetMaterialWrapper,args)
    def GetMaterialWrapper(*args): return apply(_blcelc.iSprite2DFactoryState_GetMaterialWrapper,args)
    def SetMixMode(*args): return apply(_blcelc.iSprite2DFactoryState_SetMixMode,args)
    def GetMixMode(*args): return apply(_blcelc.iSprite2DFactoryState_GetMixMode,args)
    def SetLighting(*args): return apply(_blcelc.iSprite2DFactoryState_SetLighting,args)
    def HasLighting(*args): return apply(_blcelc.iSprite2DFactoryState_HasLighting,args)
    def GetUVAnimationCount(*args): return apply(_blcelc.iSprite2DFactoryState_GetUVAnimationCount,args)
    def CreateUVAnimation(*args): return apply(_blcelc.iSprite2DFactoryState_CreateUVAnimation,args)
    def RemoveUVAnimation(*args): return apply(_blcelc.iSprite2DFactoryState_RemoveUVAnimation,args)
    def GetUVAnimation(*args): return apply(_blcelc.iSprite2DFactoryState_GetUVAnimation,args)
    def __del__(self, destroy= _blcelc.delete_iSprite2DFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite2DFactoryState instance at %s>" % (self.this,)

class iSprite2DFactoryStatePtr(iSprite2DFactoryState):
    def __init__(self,this):
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
    def GetVertices(*args): return apply(_blcelc.iSprite2DState_GetVertices,args)
    def CreateRegularVertices(*args): return apply(_blcelc.iSprite2DState_CreateRegularVertices,args)
    def SetUVAnimation(*args): return apply(_blcelc.iSprite2DState_SetUVAnimation,args)
    def StopUVAnimation(*args): return apply(_blcelc.iSprite2DState_StopUVAnimation,args)
    def PlayUVAnimation(*args): return apply(_blcelc.iSprite2DState_PlayUVAnimation,args)
    def GetVertexByIndex(*args): return apply(_blcelc.iSprite2DState_GetVertexByIndex,args)
    def __del__(self, destroy= _blcelc.delete_iSprite2DState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSprite2DState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSprite2DState_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite2DState instance at %s>" % (self.this,)

class iSprite2DStatePtr(iSprite2DState):
    def __init__(self,this):
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
    def SetName(*args): return apply(_blcelc.iSpriteFrame_SetName,args)
    def GetName(*args): return apply(_blcelc.iSpriteFrame_GetName,args)
    def GetAnmIndex(*args): return apply(_blcelc.iSpriteFrame_GetAnmIndex,args)
    def GetTexIndex(*args): return apply(_blcelc.iSpriteFrame_GetTexIndex,args)
    def __del__(self, destroy= _blcelc.delete_iSpriteFrame):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteFrame instance at %s>" % (self.this,)

class iSpriteFramePtr(iSpriteFrame):
    def __init__(self,this):
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
    def SetName(*args): return apply(_blcelc.iSpriteAction_SetName,args)
    def GetName(*args): return apply(_blcelc.iSpriteAction_GetName,args)
    def GetFrameCount(*args): return apply(_blcelc.iSpriteAction_GetFrameCount,args)
    def GetFrame(*args): return apply(_blcelc.iSpriteAction_GetFrame,args)
    def GetNextFrame(*args): return apply(_blcelc.iSpriteAction_GetNextFrame,args)
    def GetFrameDelay(*args): return apply(_blcelc.iSpriteAction_GetFrameDelay,args)
    def GetFrameDisplacement(*args): return apply(_blcelc.iSpriteAction_GetFrameDisplacement,args)
    def AddFrame(*args): return apply(_blcelc.iSpriteAction_AddFrame,args)
    def __del__(self, destroy= _blcelc.delete_iSpriteAction):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteAction instance at %s>" % (self.this,)

class iSpriteActionPtr(iSpriteAction):
    def __init__(self,this):
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
    def SetName(*args): return apply(_blcelc.iSpriteSocket_SetName,args)
    def GetName(*args): return apply(_blcelc.iSpriteSocket_GetName,args)
    def SetMeshWrapper(*args): return apply(_blcelc.iSpriteSocket_SetMeshWrapper,args)
    def GetMeshWrapper(*args): return apply(_blcelc.iSpriteSocket_GetMeshWrapper,args)
    def SetTriangleIndex(*args): return apply(_blcelc.iSpriteSocket_SetTriangleIndex,args)
    def GetTriangleIndex(*args): return apply(_blcelc.iSpriteSocket_GetTriangleIndex,args)
    def __del__(self, destroy= _blcelc.delete_iSpriteSocket):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteSocket instance at %s>" % (self.this,)

class iSpriteSocketPtr(iSpriteSocket):
    def __init__(self,this):
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
    def SetMaterialWrapper(*args): return apply(_blcelc.iSprite3DFactoryState_SetMaterialWrapper,args)
    def GetMaterialWrapper(*args): return apply(_blcelc.iSprite3DFactoryState_GetMaterialWrapper,args)
    def AddVertices(*args): return apply(_blcelc.iSprite3DFactoryState_AddVertices,args)
    def GetVertexCount(*args): return apply(_blcelc.iSprite3DFactoryState_GetVertexCount,args)
    def GetVertex(*args): return apply(_blcelc.iSprite3DFactoryState_GetVertex,args)
    def SetVertex(*args): return apply(_blcelc.iSprite3DFactoryState_SetVertex,args)
    def GetVertices(*args): return apply(_blcelc.iSprite3DFactoryState_GetVertices,args)
    def SetVertices(*args): return apply(_blcelc.iSprite3DFactoryState_SetVertices,args)
    def GetTexel(*args): return apply(_blcelc.iSprite3DFactoryState_GetTexel,args)
    def SetTexel(*args): return apply(_blcelc.iSprite3DFactoryState_SetTexel,args)
    def GetTexels(*args): return apply(_blcelc.iSprite3DFactoryState_GetTexels,args)
    def SetTexels(*args): return apply(_blcelc.iSprite3DFactoryState_SetTexels,args)
    def GetNormal(*args): return apply(_blcelc.iSprite3DFactoryState_GetNormal,args)
    def SetNormal(*args): return apply(_blcelc.iSprite3DFactoryState_SetNormal,args)
    def GetNormals(*args): return apply(_blcelc.iSprite3DFactoryState_GetNormals,args)
    def SetNormals(*args): return apply(_blcelc.iSprite3DFactoryState_SetNormals,args)
    def AddTriangle(*args): return apply(_blcelc.iSprite3DFactoryState_AddTriangle,args)
    def GetTriangle(*args): return apply(_blcelc.iSprite3DFactoryState_GetTriangle,args)
    def GetTriangles(*args): return apply(_blcelc.iSprite3DFactoryState_GetTriangles,args)
    def GetTriangleCount(*args): return apply(_blcelc.iSprite3DFactoryState_GetTriangleCount,args)
    def SetTriangleCount(*args): return apply(_blcelc.iSprite3DFactoryState_SetTriangleCount,args)
    def SetTriangles(*args): return apply(_blcelc.iSprite3DFactoryState_SetTriangles,args)
    def AddFrame(*args): return apply(_blcelc.iSprite3DFactoryState_AddFrame,args)
    def FindFrame(*args): return apply(_blcelc.iSprite3DFactoryState_FindFrame,args)
    def GetFrameCount(*args): return apply(_blcelc.iSprite3DFactoryState_GetFrameCount,args)
    def GetFrame(*args): return apply(_blcelc.iSprite3DFactoryState_GetFrame,args)
    def AddAction(*args): return apply(_blcelc.iSprite3DFactoryState_AddAction,args)
    def FindAction(*args): return apply(_blcelc.iSprite3DFactoryState_FindAction,args)
    def GetFirstAction(*args): return apply(_blcelc.iSprite3DFactoryState_GetFirstAction,args)
    def GetActionCount(*args): return apply(_blcelc.iSprite3DFactoryState_GetActionCount,args)
    def GetAction(*args): return apply(_blcelc.iSprite3DFactoryState_GetAction,args)
    def AddSocket(*args): return apply(_blcelc.iSprite3DFactoryState_AddSocket,args)
    def FindSocket(*args): return apply(_blcelc.iSprite3DFactoryState_FindSocket,args)
    def GetSocketCount(*args): return apply(_blcelc.iSprite3DFactoryState_GetSocketCount,args)
    def GetSocket(*args): return apply(_blcelc.iSprite3DFactoryState_GetSocket,args)
    def EnableSkeletalAnimation(*args): return apply(_blcelc.iSprite3DFactoryState_EnableSkeletalAnimation,args)
    def GetSkeleton(*args): return apply(_blcelc.iSprite3DFactoryState_GetSkeleton,args)
    def EnableTweening(*args): return apply(_blcelc.iSprite3DFactoryState_EnableTweening,args)
    def IsTweeningEnabled(*args): return apply(_blcelc.iSprite3DFactoryState_IsTweeningEnabled,args)
    def SetLightingQuality(*args): return apply(_blcelc.iSprite3DFactoryState_SetLightingQuality,args)
    def GetLightingQuality(*args): return apply(_blcelc.iSprite3DFactoryState_GetLightingQuality,args)
    def SetLightingQualityConfig(*args): return apply(_blcelc.iSprite3DFactoryState_SetLightingQualityConfig,args)
    def GetLightingQualityConfig(*args): return apply(_blcelc.iSprite3DFactoryState_GetLightingQualityConfig,args)
    def SetLodLevelConfig(*args): return apply(_blcelc.iSprite3DFactoryState_SetLodLevelConfig,args)
    def GetLodLevelConfig(*args): return apply(_blcelc.iSprite3DFactoryState_GetLodLevelConfig,args)
    def MergeNormals(*args): return apply(_blcelc.iSprite3DFactoryState_MergeNormals,args)
    def SetMixMode(*args): return apply(_blcelc.iSprite3DFactoryState_SetMixMode,args)
    def GetMixMode(*args): return apply(_blcelc.iSprite3DFactoryState_GetMixMode,args)
    def __del__(self, destroy= _blcelc.delete_iSprite3DFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite3DFactoryState instance at %s>" % (self.this,)

class iSprite3DFactoryStatePtr(iSprite3DFactoryState):
    def __init__(self,this):
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
    def SetMaterialWrapper(*args): return apply(_blcelc.iSprite3DState_SetMaterialWrapper,args)
    def GetMaterialWrapper(*args): return apply(_blcelc.iSprite3DState_GetMaterialWrapper,args)
    def SetMixMode(*args): return apply(_blcelc.iSprite3DState_SetMixMode,args)
    def GetMixMode(*args): return apply(_blcelc.iSprite3DState_GetMixMode,args)
    def SetLighting(*args): return apply(_blcelc.iSprite3DState_SetLighting,args)
    def IsLighting(*args): return apply(_blcelc.iSprite3DState_IsLighting,args)
    def GetSkeletonState(*args): return apply(_blcelc.iSprite3DState_GetSkeletonState,args)
    def SetFrame(*args): return apply(_blcelc.iSprite3DState_SetFrame,args)
    def GetCurFrame(*args): return apply(_blcelc.iSprite3DState_GetCurFrame,args)
    def GetFrameCount(*args): return apply(_blcelc.iSprite3DState_GetFrameCount,args)
    def SetAction(*args): return apply(_blcelc.iSprite3DState_SetAction,args)
    def SetReverseAction(*args): return apply(_blcelc.iSprite3DState_SetReverseAction,args)
    def SetSingleStepAction(*args): return apply(_blcelc.iSprite3DState_SetSingleStepAction,args)
    def SetOverrideAction(*args): return apply(_blcelc.iSprite3DState_SetOverrideAction,args)
    def PropagateAction(*args): return apply(_blcelc.iSprite3DState_PropagateAction,args)
    def GetCurAction(*args): return apply(_blcelc.iSprite3DState_GetCurAction,args)
    def GetReverseAction(*args): return apply(_blcelc.iSprite3DState_GetReverseAction,args)
    def EnableTweening(*args): return apply(_blcelc.iSprite3DState_EnableTweening,args)
    def IsTweeningEnabled(*args): return apply(_blcelc.iSprite3DState_IsTweeningEnabled,args)
    def UnsetTexture(*args): return apply(_blcelc.iSprite3DState_UnsetTexture,args)
    def GetLightingQuality(*args): return apply(_blcelc.iSprite3DState_GetLightingQuality,args)
    def SetLocalLightingQuality(*args): return apply(_blcelc.iSprite3DState_SetLocalLightingQuality,args)
    def SetLightingQualityConfig(*args): return apply(_blcelc.iSprite3DState_SetLightingQualityConfig,args)
    def GetLightingQualityConfig(*args): return apply(_blcelc.iSprite3DState_GetLightingQualityConfig,args)
    def SetLodLevelConfig(*args): return apply(_blcelc.iSprite3DState_SetLodLevelConfig,args)
    def GetLodLevelConfig(*args): return apply(_blcelc.iSprite3DState_GetLodLevelConfig,args)
    def IsLodEnabled(*args): return apply(_blcelc.iSprite3DState_IsLodEnabled,args)
    def SetBaseColor(*args): return apply(_blcelc.iSprite3DState_SetBaseColor,args)
    def GetBaseColor(*args): return apply(_blcelc.iSprite3DState_GetBaseColor,args)
    def FindSocket(*args): return apply(_blcelc.iSprite3DState_FindSocket,args)
    def __del__(self, destroy= _blcelc.delete_iSprite3DState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSprite3DState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSprite3DState_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSprite3DState instance at %s>" % (self.this,)

class iSprite3DStatePtr(iSprite3DState):
    def __init__(self,this):
        _swig_setattr(self, iSprite3DState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSprite3DState, 'thisown', 0)
        _swig_setattr(self, iSprite3DState,self.__class__,iSprite3DState)
_blcelc.iSprite3DState_swigregister(iSprite3DStatePtr)
iSprite3DState_scfGetVersion = _blcelc.iSprite3DState_scfGetVersion


class iSpriteCal3DFactoryState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSpriteCal3DFactoryState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSpriteCal3DFactoryState, name)
    def Create(*args): return apply(_blcelc.iSpriteCal3DFactoryState_Create,args)
    def ReportLastError(*args): return apply(_blcelc.iSpriteCal3DFactoryState_ReportLastError,args)
    def SetLoadFlags(*args): return apply(_blcelc.iSpriteCal3DFactoryState_SetLoadFlags,args)
    def SetBasePath(*args): return apply(_blcelc.iSpriteCal3DFactoryState_SetBasePath,args)
    def LoadCoreSkeleton(*args): return apply(_blcelc.iSpriteCal3DFactoryState_LoadCoreSkeleton,args)
    def SetRenderScale(*args): return apply(_blcelc.iSpriteCal3DFactoryState_SetRenderScale,args)
    def GetRenderScale(*args): return apply(_blcelc.iSpriteCal3DFactoryState_GetRenderScale,args)
    def LoadCoreAnimation(*args): return apply(_blcelc.iSpriteCal3DFactoryState_LoadCoreAnimation,args)
    def LoadCoreMesh(*args): return apply(_blcelc.iSpriteCal3DFactoryState_LoadCoreMesh,args)
    def LoadCoreMorphTarget(*args): return apply(_blcelc.iSpriteCal3DFactoryState_LoadCoreMorphTarget,args)
    def AddMorphAnimation(*args): return apply(_blcelc.iSpriteCal3DFactoryState_AddMorphAnimation,args)
    def AddMorphTarget(*args): return apply(_blcelc.iSpriteCal3DFactoryState_AddMorphTarget,args)
    def AddCoreMaterial(*args): return apply(_blcelc.iSpriteCal3DFactoryState_AddCoreMaterial,args)
    def BindMaterials(*args): return apply(_blcelc.iSpriteCal3DFactoryState_BindMaterials,args)
    def GetMeshCount(*args): return apply(_blcelc.iSpriteCal3DFactoryState_GetMeshCount,args)
    def GetMorphAnimationCount(*args): return apply(_blcelc.iSpriteCal3DFactoryState_GetMorphAnimationCount,args)
    def GetMorphTargetCount(*args): return apply(_blcelc.iSpriteCal3DFactoryState_GetMorphTargetCount,args)
    def GetMeshName(*args): return apply(_blcelc.iSpriteCal3DFactoryState_GetMeshName,args)
    def FindMeshName(*args): return apply(_blcelc.iSpriteCal3DFactoryState_FindMeshName,args)
    def GetMorphAnimationName(*args): return apply(_blcelc.iSpriteCal3DFactoryState_GetMorphAnimationName,args)
    def FindMorphAnimationName(*args): return apply(_blcelc.iSpriteCal3DFactoryState_FindMorphAnimationName,args)
    def IsMeshDefault(*args): return apply(_blcelc.iSpriteCal3DFactoryState_IsMeshDefault,args)
    def __del__(self, destroy= _blcelc.delete_iSpriteCal3DFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteCal3DFactoryState instance at %s>" % (self.this,)

class iSpriteCal3DFactoryStatePtr(iSpriteCal3DFactoryState):
    def __init__(self,this):
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
    C3D_ANIM_TYPE_NONE = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_NONE
    C3D_ANIM_TYPE_TRAVEL = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_TRAVEL
    C3D_ANIM_TYPE_CYCLE = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_CYCLE
    C3D_ANIM_TYPE_STYLE_CYCLE = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_STYLE_CYCLE
    C3D_ANIM_TYPE_ACTION = _blcelc.iSpriteCal3DState_C3D_ANIM_TYPE_ACTION
    def GetAnimCount(*args): return apply(_blcelc.iSpriteCal3DState_GetAnimCount,args)
    def GetAnimName(*args): return apply(_blcelc.iSpriteCal3DState_GetAnimName,args)
    def GetAnimType(*args): return apply(_blcelc.iSpriteCal3DState_GetAnimType,args)
    def ClearAllAnims(*args): return apply(_blcelc.iSpriteCal3DState_ClearAllAnims,args)
    def SetAnimCycle(*args): return apply(_blcelc.iSpriteCal3DState_SetAnimCycle,args)
    def AddAnimCycle(*args): return apply(_blcelc.iSpriteCal3DState_AddAnimCycle,args)
    def ClearAnimCycle(*args): return apply(_blcelc.iSpriteCal3DState_ClearAnimCycle,args)
    def SetAnimAction(*args): return apply(_blcelc.iSpriteCal3DState_SetAnimAction,args)
    def SetVelocity(*args): return apply(_blcelc.iSpriteCal3DState_SetVelocity,args)
    def SetLOD(*args): return apply(_blcelc.iSpriteCal3DState_SetLOD,args)
    def AttachCoreMesh(*args): return apply(_blcelc.iSpriteCal3DState_AttachCoreMesh,args)
    def DetachCoreMesh(*args): return apply(_blcelc.iSpriteCal3DState_DetachCoreMesh,args)
    def BlendMorphTarget(*args): return apply(_blcelc.iSpriteCal3DState_BlendMorphTarget,args)
    def ClearMorphTarget(*args): return apply(_blcelc.iSpriteCal3DState_ClearMorphTarget,args)
    def __del__(self, destroy= _blcelc.delete_iSpriteCal3DState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSpriteCal3DState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSpriteCal3DState_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSpriteCal3DState instance at %s>" % (self.this,)

class iSpriteCal3DStatePtr(iSpriteCal3DState):
    def __init__(self,this):
        _swig_setattr(self, iSpriteCal3DState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iSpriteCal3DState, 'thisown', 0)
        _swig_setattr(self, iSpriteCal3DState,self.__class__,iSpriteCal3DState)
_blcelc.iSpriteCal3DState_swigregister(iSpriteCal3DStatePtr)
iSpriteCal3DState_scfGetVersion = _blcelc.iSpriteCal3DState_scfGetVersion


class iBallState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBallState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBallState, name)
    def SetRadius(*args): return apply(_blcelc.iBallState_SetRadius,args)
    def GetRadius(*args): return apply(_blcelc.iBallState_GetRadius,args)
    def SetShift(*args): return apply(_blcelc.iBallState_SetShift,args)
    def GetShift(*args): return apply(_blcelc.iBallState_GetShift,args)
    def SetRimVertices(*args): return apply(_blcelc.iBallState_SetRimVertices,args)
    def GetRimVertices(*args): return apply(_blcelc.iBallState_GetRimVertices,args)
    def SetMaterialWrapper(*args): return apply(_blcelc.iBallState_SetMaterialWrapper,args)
    def GetMaterialWrapper(*args): return apply(_blcelc.iBallState_GetMaterialWrapper,args)
    def SetMixMode(*args): return apply(_blcelc.iBallState_SetMixMode,args)
    def GetMixMode(*args): return apply(_blcelc.iBallState_GetMixMode,args)
    def SetReversed(*args): return apply(_blcelc.iBallState_SetReversed,args)
    def IsReversed(*args): return apply(_blcelc.iBallState_IsReversed,args)
    def SetTopOnly(*args): return apply(_blcelc.iBallState_SetTopOnly,args)
    def IsTopOnly(*args): return apply(_blcelc.iBallState_IsTopOnly,args)
    def SetLighting(*args): return apply(_blcelc.iBallState_SetLighting,args)
    def IsLighting(*args): return apply(_blcelc.iBallState_IsLighting,args)
    def SetColor(*args): return apply(_blcelc.iBallState_SetColor,args)
    def GetColor(*args): return apply(_blcelc.iBallState_GetColor,args)
    def SetCylindricalMapping(*args): return apply(_blcelc.iBallState_SetCylindricalMapping,args)
    def IsCylindricalMapping(*args): return apply(_blcelc.iBallState_IsCylindricalMapping,args)
    def ApplyVertGradient(*args): return apply(_blcelc.iBallState_ApplyVertGradient,args)
    def ApplyLightSpot(*args): return apply(_blcelc.iBallState_ApplyLightSpot,args)
    def PaintSky(*args): return apply(_blcelc.iBallState_PaintSky,args)
    def __del__(self, destroy= _blcelc.delete_iBallState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iBallState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iBallState_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBallState instance at %s>" % (self.this,)

class iBallStatePtr(iBallState):
    def __init__(self,this):
        _swig_setattr(self, iBallState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iBallState, 'thisown', 0)
        _swig_setattr(self, iBallState,self.__class__,iBallState)
_blcelc.iBallState_swigregister(iBallStatePtr)
iBallState_scfGetVersion = _blcelc.iBallState_scfGetVersion


class iGeneralMeshState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGeneralMeshState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGeneralMeshState, name)
    def SetMaterialWrapper(*args): return apply(_blcelc.iGeneralMeshState_SetMaterialWrapper,args)
    def GetMaterialWrapper(*args): return apply(_blcelc.iGeneralMeshState_GetMaterialWrapper,args)
    def SetMixMode(*args): return apply(_blcelc.iGeneralMeshState_SetMixMode,args)
    def GetMixMode(*args): return apply(_blcelc.iGeneralMeshState_GetMixMode,args)
    def SetLighting(*args): return apply(_blcelc.iGeneralMeshState_SetLighting,args)
    def IsLighting(*args): return apply(_blcelc.iGeneralMeshState_IsLighting,args)
    def SetColor(*args): return apply(_blcelc.iGeneralMeshState_SetColor,args)
    def GetColor(*args): return apply(_blcelc.iGeneralMeshState_GetColor,args)
    def SetManualColors(*args): return apply(_blcelc.iGeneralMeshState_SetManualColors,args)
    def IsManualColors(*args): return apply(_blcelc.iGeneralMeshState_IsManualColors,args)
    def SetShadowCasting(*args): return apply(_blcelc.iGeneralMeshState_SetShadowCasting,args)
    def IsShadowCasting(*args): return apply(_blcelc.iGeneralMeshState_IsShadowCasting,args)
    def SetShadowReceiving(*args): return apply(_blcelc.iGeneralMeshState_SetShadowReceiving,args)
    def IsShadowReceiving(*args): return apply(_blcelc.iGeneralMeshState_IsShadowReceiving,args)
    def __del__(self, destroy= _blcelc.delete_iGeneralMeshState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iGeneralMeshState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iGeneralMeshState_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGeneralMeshState instance at %s>" % (self.this,)

class iGeneralMeshStatePtr(iGeneralMeshState):
    def __init__(self,this):
        _swig_setattr(self, iGeneralMeshState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGeneralMeshState, 'thisown', 0)
        _swig_setattr(self, iGeneralMeshState,self.__class__,iGeneralMeshState)
_blcelc.iGeneralMeshState_swigregister(iGeneralMeshStatePtr)
iGeneralMeshState_scfGetVersion = _blcelc.iGeneralMeshState_scfGetVersion


class iGeneralFactoryState(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGeneralFactoryState, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGeneralFactoryState, name)
    def SetMaterialWrapper(*args): return apply(_blcelc.iGeneralFactoryState_SetMaterialWrapper,args)
    def GetMaterialWrapper(*args): return apply(_blcelc.iGeneralFactoryState_GetMaterialWrapper,args)
    def SetVertexCount(*args): return apply(_blcelc.iGeneralFactoryState_SetVertexCount,args)
    def GetVertexCount(*args): return apply(_blcelc.iGeneralFactoryState_GetVertexCount,args)
    def GetVertices(*args): return apply(_blcelc.iGeneralFactoryState_GetVertices,args)
    def GetTexels(*args): return apply(_blcelc.iGeneralFactoryState_GetTexels,args)
    def GetNormals(*args): return apply(_blcelc.iGeneralFactoryState_GetNormals,args)
    def SetTriangleCount(*args): return apply(_blcelc.iGeneralFactoryState_SetTriangleCount,args)
    def GetTriangleCount(*args): return apply(_blcelc.iGeneralFactoryState_GetTriangleCount,args)
    def GetTriangles(*args): return apply(_blcelc.iGeneralFactoryState_GetTriangles,args)
    def GetColors(*args): return apply(_blcelc.iGeneralFactoryState_GetColors,args)
    def Invalidate(*args): return apply(_blcelc.iGeneralFactoryState_Invalidate,args)
    def CalculateNormals(*args): return apply(_blcelc.iGeneralFactoryState_CalculateNormals,args)
    def GenerateBox(*args): return apply(_blcelc.iGeneralFactoryState_GenerateBox,args)
    def AddRenderBuffer(*args): return apply(_blcelc.iGeneralFactoryState_AddRenderBuffer,args)
    def SetRenderBufferComponent(*args): return apply(_blcelc.iGeneralFactoryState_SetRenderBufferComponent,args)
    def SetRenderBuffer(*args): return apply(_blcelc.iGeneralFactoryState_SetRenderBuffer,args)
    def __del__(self, destroy= _blcelc.delete_iGeneralFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iGeneralFactoryState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iGeneralFactoryState_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGeneralFactoryState instance at %s>" % (self.this,)

class iGeneralFactoryStatePtr(iGeneralFactoryState):
    def __init__(self,this):
        _swig_setattr(self, iGeneralFactoryState, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGeneralFactoryState, 'thisown', 0)
        _swig_setattr(self, iGeneralFactoryState,self.__class__,iGeneralFactoryState)
_blcelc.iGeneralFactoryState_swigregister(iGeneralFactoryStatePtr)
iGeneralFactoryState_scfGetVersion = _blcelc.iGeneralFactoryState_scfGetVersion


class csPolygonRange(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPolygonRange, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPolygonRange, name)
    __swig_setmethods__["start"] = _blcelc.csPolygonRange_start_set
    __swig_getmethods__["start"] = _blcelc.csPolygonRange_start_get
    if _newclass:start = property(_blcelc.csPolygonRange_start_get,_blcelc.csPolygonRange_start_set)
    __swig_setmethods__["end"] = _blcelc.csPolygonRange_end_set
    __swig_getmethods__["end"] = _blcelc.csPolygonRange_end_get
    if _newclass:end = property(_blcelc.csPolygonRange_end_get,_blcelc.csPolygonRange_end_set)
    def __init__(self,*args):
        _swig_setattr(self, csPolygonRange, 'this', apply(_blcelc.new_csPolygonRange,args))
        _swig_setattr(self, csPolygonRange, 'thisown', 1)
    def Set(*args): return apply(_blcelc.csPolygonRange_Set,args)
    def __del__(self, destroy= _blcelc.delete_csPolygonRange):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csPolygonRange instance at %s>" % (self.this,)

class csPolygonRangePtr(csPolygonRange):
    def __init__(self,this):
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
    def GetThingFactoryState(*args): return apply(_blcelc.iPolygonHandle_GetThingFactoryState,args)
    def GetMeshObjectFactory(*args): return apply(_blcelc.iPolygonHandle_GetMeshObjectFactory,args)
    def GetThingState(*args): return apply(_blcelc.iPolygonHandle_GetThingState,args)
    def GetMeshObject(*args): return apply(_blcelc.iPolygonHandle_GetMeshObject,args)
    def GetIndex(*args): return apply(_blcelc.iPolygonHandle_GetIndex,args)
    def __del__(self, destroy= _blcelc.delete_iPolygonHandle):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPolygonHandle instance at %s>" % (self.this,)

class iPolygonHandlePtr(iPolygonHandle):
    def __init__(self,this):
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
    def GetPrivateObject(*args): return apply(_blcelc.iThingFactoryState_GetPrivateObject,args)
    def CompressVertices(*args): return apply(_blcelc.iThingFactoryState_CompressVertices,args)
    def GetPolygonCount(*args): return apply(_blcelc.iThingFactoryState_GetPolygonCount,args)
    def RemovePolygon(*args): return apply(_blcelc.iThingFactoryState_RemovePolygon,args)
    def RemovePolygons(*args): return apply(_blcelc.iThingFactoryState_RemovePolygons,args)
    def FindPolygonByName(*args): return apply(_blcelc.iThingFactoryState_FindPolygonByName,args)
    def AddEmptyPolygon(*args): return apply(_blcelc.iThingFactoryState_AddEmptyPolygon,args)
    def AddTriangle(*args): return apply(_blcelc.iThingFactoryState_AddTriangle,args)
    def AddQuad(*args): return apply(_blcelc.iThingFactoryState_AddQuad,args)
    def AddPolygon(*args): return apply(_blcelc.iThingFactoryState_AddPolygon,args)
    def AddOutsideBox(*args): return apply(_blcelc.iThingFactoryState_AddOutsideBox,args)
    def AddInsideBox(*args): return apply(_blcelc.iThingFactoryState_AddInsideBox,args)
    def SetPolygonName(*args): return apply(_blcelc.iThingFactoryState_SetPolygonName,args)
    def GetPolygonName(*args): return apply(_blcelc.iThingFactoryState_GetPolygonName,args)
    def CreatePolygonHandle(*args): return apply(_blcelc.iThingFactoryState_CreatePolygonHandle,args)
    def SetPolygonMaterial(*args): return apply(_blcelc.iThingFactoryState_SetPolygonMaterial,args)
    def GetPolygonMaterial(*args): return apply(_blcelc.iThingFactoryState_GetPolygonMaterial,args)
    def AddPolygonVertex(*args): return apply(_blcelc.iThingFactoryState_AddPolygonVertex,args)
    def SetPolygonVertexIndices(*args): return apply(_blcelc.iThingFactoryState_SetPolygonVertexIndices,args)
    def GetPolygonVertexCount(*args): return apply(_blcelc.iThingFactoryState_GetPolygonVertexCount,args)
    def GetPolygonVertex(*args): return apply(_blcelc.iThingFactoryState_GetPolygonVertex,args)
    def GetPolygonVertexIndices(*args): return apply(_blcelc.iThingFactoryState_GetPolygonVertexIndices,args)
    def SetPolygonTextureMapping(*args): return apply(_blcelc.iThingFactoryState_SetPolygonTextureMapping,args)
    def GetPolygonTextureMapping(*args): return apply(_blcelc.iThingFactoryState_GetPolygonTextureMapping,args)
    def SetPolygonTextureMappingEnabled(*args): return apply(_blcelc.iThingFactoryState_SetPolygonTextureMappingEnabled,args)
    def IsPolygonTextureMappingEnabled(*args): return apply(_blcelc.iThingFactoryState_IsPolygonTextureMappingEnabled,args)
    def SetPolygonFlags(*args): return apply(_blcelc.iThingFactoryState_SetPolygonFlags,args)
    def ResetPolygonFlags(*args): return apply(_blcelc.iThingFactoryState_ResetPolygonFlags,args)
    def GetPolygonFlags(*args): return apply(_blcelc.iThingFactoryState_GetPolygonFlags,args)
    def GetPolygonObjectPlane(*args): return apply(_blcelc.iThingFactoryState_GetPolygonObjectPlane,args)
    def IsPolygonTransparent(*args): return apply(_blcelc.iThingFactoryState_IsPolygonTransparent,args)
    def GetVertexCount(*args): return apply(_blcelc.iThingFactoryState_GetVertexCount,args)
    def GetVertex(*args): return apply(_blcelc.iThingFactoryState_GetVertex,args)
    def GetVertices(*args): return apply(_blcelc.iThingFactoryState_GetVertices,args)
    def CreateVertex(*args): return apply(_blcelc.iThingFactoryState_CreateVertex,args)
    def SetVertex(*args): return apply(_blcelc.iThingFactoryState_SetVertex,args)
    def DeleteVertex(*args): return apply(_blcelc.iThingFactoryState_DeleteVertex,args)
    def DeleteVertices(*args): return apply(_blcelc.iThingFactoryState_DeleteVertices,args)
    def GetFlags(*args): return apply(_blcelc.iThingFactoryState_GetFlags,args)
    def SetSmoothingFlag(*args): return apply(_blcelc.iThingFactoryState_SetSmoothingFlag,args)
    def GetSmoothingFlag(*args): return apply(_blcelc.iThingFactoryState_GetSmoothingFlag,args)
    def GetNormals(*args): return apply(_blcelc.iThingFactoryState_GetNormals,args)
    def GetCosinusFactor(*args): return apply(_blcelc.iThingFactoryState_GetCosinusFactor,args)
    def SetCosinusFactor(*args): return apply(_blcelc.iThingFactoryState_SetCosinusFactor,args)
    def __del__(self, destroy= _blcelc.delete_iThingFactoryState):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iThingFactoryState instance at %s>" % (self.this,)

class iThingFactoryStatePtr(iThingFactoryState):
    def __init__(self,this):
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
    def GetPrivateObject(*args): return apply(_blcelc.iThingState_GetPrivateObject,args)
    def GetFactory(*args): return apply(_blcelc.iThingState_GetFactory,args)
    def GetVertexW(*args): return apply(_blcelc.iThingState_GetVertexW,args)
    def GetVerticesW(*args): return apply(_blcelc.iThingState_GetVerticesW,args)
    def GetMovingOption(*args): return apply(_blcelc.iThingState_GetMovingOption,args)
    def SetMovingOption(*args): return apply(_blcelc.iThingState_SetMovingOption,args)
    def Prepare(*args): return apply(_blcelc.iThingState_Prepare,args)
    def Unprepare(*args): return apply(_blcelc.iThingState_Unprepare,args)
    def ReplaceMaterial(*args): return apply(_blcelc.iThingState_ReplaceMaterial,args)
    def ClearReplacedMaterials(*args): return apply(_blcelc.iThingState_ClearReplacedMaterials,args)
    def SetMixMode(*args): return apply(_blcelc.iThingState_SetMixMode,args)
    def GetMixMode(*args): return apply(_blcelc.iThingState_GetMixMode,args)
    def CreatePolygonHandle(*args): return apply(_blcelc.iThingState_CreatePolygonHandle,args)
    def GetPolygonWorldPlane(*args): return apply(_blcelc.iThingState_GetPolygonWorldPlane,args)
    def GetPolygonMaterial(*args): return apply(_blcelc.iThingState_GetPolygonMaterial,args)
    def SetPolygonMaterial(*args): return apply(_blcelc.iThingState_SetPolygonMaterial,args)
    def __del__(self, destroy= _blcelc.delete_iThingState):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iThingState_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iThingState_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iThingState instance at %s>" % (self.this,)

class iThingStatePtr(iThingState):
    def __init__(self,this):
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
    def Clear(*args): return apply(_blcelc.iThingEnvironment_Clear,args)
    def GetLightmapCellSize(*args): return apply(_blcelc.iThingEnvironment_GetLightmapCellSize,args)
    def SetLightmapCellSize(*args): return apply(_blcelc.iThingEnvironment_SetLightmapCellSize,args)
    def GetDefaultLightmapCellSize(*args): return apply(_blcelc.iThingEnvironment_GetDefaultLightmapCellSize,args)
    def __del__(self, destroy= _blcelc.delete_iThingEnvironment):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iThingEnvironment instance at %s>" % (self.this,)

class iThingEnvironmentPtr(iThingEnvironment):
    def __init__(self,this):
        _swig_setattr(self, iThingEnvironment, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iThingEnvironment, 'thisown', 0)
        _swig_setattr(self, iThingEnvironment,self.__class__,iThingEnvironment)
_blcelc.iThingEnvironment_swigregister(iThingEnvironmentPtr)

class csPolyTextureMapping(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPolyTextureMapping, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPolyTextureMapping, name)
    __swig_setmethods__["m_obj2tex"] = _blcelc.csPolyTextureMapping_m_obj2tex_set
    __swig_getmethods__["m_obj2tex"] = _blcelc.csPolyTextureMapping_m_obj2tex_get
    if _newclass:m_obj2tex = property(_blcelc.csPolyTextureMapping_m_obj2tex_get,_blcelc.csPolyTextureMapping_m_obj2tex_set)
    __swig_setmethods__["v_obj2tex"] = _blcelc.csPolyTextureMapping_v_obj2tex_set
    __swig_getmethods__["v_obj2tex"] = _blcelc.csPolyTextureMapping_v_obj2tex_get
    if _newclass:v_obj2tex = property(_blcelc.csPolyTextureMapping_v_obj2tex_get,_blcelc.csPolyTextureMapping_v_obj2tex_set)
    __swig_setmethods__["fdu"] = _blcelc.csPolyTextureMapping_fdu_set
    __swig_getmethods__["fdu"] = _blcelc.csPolyTextureMapping_fdu_get
    if _newclass:fdu = property(_blcelc.csPolyTextureMapping_fdu_get,_blcelc.csPolyTextureMapping_fdu_set)
    __swig_setmethods__["fdv"] = _blcelc.csPolyTextureMapping_fdv_set
    __swig_getmethods__["fdv"] = _blcelc.csPolyTextureMapping_fdv_get
    if _newclass:fdv = property(_blcelc.csPolyTextureMapping_fdv_get,_blcelc.csPolyTextureMapping_fdv_set)
    __swig_setmethods__["Imin_u"] = _blcelc.csPolyTextureMapping_Imin_u_set
    __swig_getmethods__["Imin_u"] = _blcelc.csPolyTextureMapping_Imin_u_get
    if _newclass:Imin_u = property(_blcelc.csPolyTextureMapping_Imin_u_get,_blcelc.csPolyTextureMapping_Imin_u_set)
    __swig_setmethods__["Imin_v"] = _blcelc.csPolyTextureMapping_Imin_v_set
    __swig_getmethods__["Imin_v"] = _blcelc.csPolyTextureMapping_Imin_v_get
    if _newclass:Imin_v = property(_blcelc.csPolyTextureMapping_Imin_v_get,_blcelc.csPolyTextureMapping_Imin_v_set)
    __swig_setmethods__["Fmin_u"] = _blcelc.csPolyTextureMapping_Fmin_u_set
    __swig_getmethods__["Fmin_u"] = _blcelc.csPolyTextureMapping_Fmin_u_get
    if _newclass:Fmin_u = property(_blcelc.csPolyTextureMapping_Fmin_u_get,_blcelc.csPolyTextureMapping_Fmin_u_set)
    __swig_setmethods__["Fmin_v"] = _blcelc.csPolyTextureMapping_Fmin_v_set
    __swig_getmethods__["Fmin_v"] = _blcelc.csPolyTextureMapping_Fmin_v_get
    if _newclass:Fmin_v = property(_blcelc.csPolyTextureMapping_Fmin_v_get,_blcelc.csPolyTextureMapping_Fmin_v_set)
    __swig_setmethods__["Fmax_u"] = _blcelc.csPolyTextureMapping_Fmax_u_set
    __swig_getmethods__["Fmax_u"] = _blcelc.csPolyTextureMapping_Fmax_u_get
    if _newclass:Fmax_u = property(_blcelc.csPolyTextureMapping_Fmax_u_get,_blcelc.csPolyTextureMapping_Fmax_u_set)
    __swig_setmethods__["Fmax_v"] = _blcelc.csPolyTextureMapping_Fmax_v_set
    __swig_getmethods__["Fmax_v"] = _blcelc.csPolyTextureMapping_Fmax_v_get
    if _newclass:Fmax_v = property(_blcelc.csPolyTextureMapping_Fmax_v_get,_blcelc.csPolyTextureMapping_Fmax_v_set)
    __swig_setmethods__["shf_u"] = _blcelc.csPolyTextureMapping_shf_u_set
    __swig_getmethods__["shf_u"] = _blcelc.csPolyTextureMapping_shf_u_get
    if _newclass:shf_u = property(_blcelc.csPolyTextureMapping_shf_u_get,_blcelc.csPolyTextureMapping_shf_u_set)
    def GetShiftU(*args): return apply(_blcelc.csPolyTextureMapping_GetShiftU,args)
    def GetIMinU(*args): return apply(_blcelc.csPolyTextureMapping_GetIMinU,args)
    def GetIMinV(*args): return apply(_blcelc.csPolyTextureMapping_GetIMinV,args)
    def GetTextureBox(*args): return apply(_blcelc.csPolyTextureMapping_GetTextureBox,args)
    def GetFDU(*args): return apply(_blcelc.csPolyTextureMapping_GetFDU,args)
    def GetFDV(*args): return apply(_blcelc.csPolyTextureMapping_GetFDV,args)
    __swig_setmethods__["w"] = _blcelc.csPolyTextureMapping_w_set
    __swig_getmethods__["w"] = _blcelc.csPolyTextureMapping_w_get
    if _newclass:w = property(_blcelc.csPolyTextureMapping_w_get,_blcelc.csPolyTextureMapping_w_set)
    __swig_setmethods__["h"] = _blcelc.csPolyTextureMapping_h_set
    __swig_getmethods__["h"] = _blcelc.csPolyTextureMapping_h_get
    if _newclass:h = property(_blcelc.csPolyTextureMapping_h_get,_blcelc.csPolyTextureMapping_h_set)
    __swig_setmethods__["w_orig"] = _blcelc.csPolyTextureMapping_w_orig_set
    __swig_getmethods__["w_orig"] = _blcelc.csPolyTextureMapping_w_orig_get
    if _newclass:w_orig = property(_blcelc.csPolyTextureMapping_w_orig_get,_blcelc.csPolyTextureMapping_w_orig_set)
    def GetWidth(*args): return apply(_blcelc.csPolyTextureMapping_GetWidth,args)
    def GetHeight(*args): return apply(_blcelc.csPolyTextureMapping_GetHeight,args)
    def GetOriginalWidth(*args): return apply(_blcelc.csPolyTextureMapping_GetOriginalWidth,args)
    __swig_setmethods__["lmu1"] = _blcelc.csPolyTextureMapping_lmu1_set
    __swig_getmethods__["lmu1"] = _blcelc.csPolyTextureMapping_lmu1_get
    if _newclass:lmu1 = property(_blcelc.csPolyTextureMapping_lmu1_get,_blcelc.csPolyTextureMapping_lmu1_set)
    __swig_setmethods__["lmv1"] = _blcelc.csPolyTextureMapping_lmv1_set
    __swig_getmethods__["lmv1"] = _blcelc.csPolyTextureMapping_lmv1_get
    if _newclass:lmv1 = property(_blcelc.csPolyTextureMapping_lmv1_get,_blcelc.csPolyTextureMapping_lmv1_set)
    __swig_setmethods__["lmu2"] = _blcelc.csPolyTextureMapping_lmu2_set
    __swig_getmethods__["lmu2"] = _blcelc.csPolyTextureMapping_lmu2_get
    if _newclass:lmu2 = property(_blcelc.csPolyTextureMapping_lmu2_get,_blcelc.csPolyTextureMapping_lmu2_set)
    __swig_setmethods__["lmv2"] = _blcelc.csPolyTextureMapping_lmv2_set
    __swig_getmethods__["lmv2"] = _blcelc.csPolyTextureMapping_lmv2_get
    if _newclass:lmv2 = property(_blcelc.csPolyTextureMapping_lmv2_get,_blcelc.csPolyTextureMapping_lmv2_set)
    def __init__(self,*args):
        _swig_setattr(self, csPolyTextureMapping, 'this', apply(_blcelc.new_csPolyTextureMapping,args))
        _swig_setattr(self, csPolyTextureMapping, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csPolyTextureMapping):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csPolyTextureMapping instance at %s>" % (self.this,)

class csPolyTextureMappingPtr(csPolyTextureMapping):
    def __init__(self,this):
        _swig_setattr(self, csPolyTextureMapping, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPolyTextureMapping, 'thisown', 0)
        _swig_setattr(self, csPolyTextureMapping,self.__class__,csPolyTextureMapping)
_blcelc.csPolyTextureMapping_swigregister(csPolyTextureMappingPtr)

class iLoaderStatus(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iLoaderStatus, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iLoaderStatus, name)
    def IsReady(*args): return apply(_blcelc.iLoaderStatus_IsReady,args)
    def IsError(*args): return apply(_blcelc.iLoaderStatus_IsError,args)
    def __del__(self, destroy= _blcelc.delete_iLoaderStatus):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLoaderStatus instance at %s>" % (self.this,)

class iLoaderStatusPtr(iLoaderStatus):
    def __init__(self,this):
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
    def LoadImage(*args): return apply(_blcelc.iLoader_LoadImage,args)
    def LoadTexture(*args): return apply(_blcelc.iLoader_LoadTexture,args)
    def LoadSoundData(*args): return apply(_blcelc.iLoader_LoadSoundData,args)
    def LoadSound(*args): return apply(_blcelc.iLoader_LoadSound,args)
    def ThreadedLoadMapFile(*args): return apply(_blcelc.iLoader_ThreadedLoadMapFile,args)
    def LoadMapFile(*args): return apply(_blcelc.iLoader_LoadMapFile,args)
    def LoadLibraryFile(*args): return apply(_blcelc.iLoader_LoadLibraryFile,args)
    def LoadMeshObjectFactory(*args): return apply(_blcelc.iLoader_LoadMeshObjectFactory,args)
    def LoadMeshObject(*args): return apply(_blcelc.iLoader_LoadMeshObject,args)
    def LoadEffectFile(*args): return apply(_blcelc.iLoader_LoadEffectFile,args)
    def __del__(self, destroy= _blcelc.delete_iLoader):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iLoader_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iLoader_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLoader instance at %s>" % (self.this,)

class iLoaderPtr(iLoader):
    def __init__(self,this):
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
    def Parse(*args): return apply(_blcelc.iLoaderPlugin_Parse,args)
    def __del__(self, destroy= _blcelc.delete_iLoaderPlugin):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iLoaderPlugin_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iLoaderPlugin_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLoaderPlugin instance at %s>" % (self.this,)

class iLoaderPluginPtr(iLoaderPlugin):
    def __init__(self,this):
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
    def Parse(*args): return apply(_blcelc.iBinaryLoaderPlugin_Parse,args)
    def __del__(self, destroy= _blcelc.delete_iBinaryLoaderPlugin):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iBinaryLoaderPlugin_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iBinaryLoaderPlugin_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBinaryLoaderPlugin instance at %s>" % (self.this,)

class iBinaryLoaderPluginPtr(iBinaryLoaderPlugin):
    def __init__(self,this):
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
    def SaveMapFile(*args): return apply(_blcelc.iSaver_SaveMapFile,args)
    def __del__(self, destroy= _blcelc.delete_iSaver):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSaver instance at %s>" % (self.this,)

class iSaverPtr(iSaver):
    def __init__(self,this):
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
    def IsStatic(*args): return apply(_blcelc.iSoundHandle_IsStatic,args)
    def Play(*args): return apply(_blcelc.iSoundHandle_Play,args)
    def CreateSource(*args): return apply(_blcelc.iSoundHandle_CreateSource,args)
    def StartStream(*args): return apply(_blcelc.iSoundHandle_StartStream,args)
    def StopStream(*args): return apply(_blcelc.iSoundHandle_StopStream,args)
    def ResetStream(*args): return apply(_blcelc.iSoundHandle_ResetStream,args)
    def __del__(self, destroy= _blcelc.delete_iSoundHandle):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundHandle_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundHandle_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundHandle instance at %s>" % (self.this,)

class iSoundHandlePtr(iSoundHandle):
    def __init__(self,this):
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
    def LoadSound(*args): return apply(_blcelc.iSoundLoader_LoadSound,args)
    def __del__(self, destroy= _blcelc.delete_iSoundLoader):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundLoader_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundLoader_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundLoader instance at %s>" % (self.this,)

class iSoundLoaderPtr(iSoundLoader):
    def __init__(self,this):
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
    def SetVolume(*args): return apply(_blcelc.iSoundRender_SetVolume,args)
    def GetVolume(*args): return apply(_blcelc.iSoundRender_GetVolume,args)
    def RegisterSound(*args): return apply(_blcelc.iSoundRender_RegisterSound,args)
    def UnregisterSound(*args): return apply(_blcelc.iSoundRender_UnregisterSound,args)
    def GetListener(*args): return apply(_blcelc.iSoundRender_GetListener,args)
    def MixingFunction(*args): return apply(_blcelc.iSoundRender_MixingFunction,args)
    def __del__(self, destroy= _blcelc.delete_iSoundRender):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundRender_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundRender_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundRender instance at %s>" % (self.this,)

class iSoundRenderPtr(iSoundRender):
    def __init__(self,this):
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
    def GetSound(*args): return apply(_blcelc.iSoundWrapper_GetSound,args)
    def QueryObject(*args): return apply(_blcelc.iSoundWrapper_QueryObject,args)
    def __del__(self, destroy= _blcelc.delete_iSoundWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundWrapper_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundWrapper_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundWrapper instance at %s>" % (self.this,)

class iSoundWrapperPtr(iSoundWrapper):
    def __init__(self,this):
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
    def Open(*args): return apply(_blcelc.iSoundDriver_Open,args)
    def Close(*args): return apply(_blcelc.iSoundDriver_Close,args)
    def LockMemory(*args): return apply(_blcelc.iSoundDriver_LockMemory,args)
    def UnlockMemory(*args): return apply(_blcelc.iSoundDriver_UnlockMemory,args)
    def IsBackground(*args): return apply(_blcelc.iSoundDriver_IsBackground,args)
    def Is16Bits(*args): return apply(_blcelc.iSoundDriver_Is16Bits,args)
    def IsStereo(*args): return apply(_blcelc.iSoundDriver_IsStereo,args)
    def GetFrequency(*args): return apply(_blcelc.iSoundDriver_GetFrequency,args)
    def IsHandleVoidSound(*args): return apply(_blcelc.iSoundDriver_IsHandleVoidSound,args)
    def ThreadAware(*args): return apply(_blcelc.iSoundDriver_ThreadAware,args)
    def __del__(self, destroy= _blcelc.delete_iSoundDriver):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundDriver_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundDriver_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundDriver instance at %s>" % (self.this,)

class iSoundDriverPtr(iSoundDriver):
    def __init__(self,this):
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
class iSoundSource(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iSoundSource, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iSoundSource, name)
    def Play(*args): return apply(_blcelc.iSoundSource_Play,args)
    def Stop(*args): return apply(_blcelc.iSoundSource_Stop,args)
    def SetVolume(*args): return apply(_blcelc.iSoundSource_SetVolume,args)
    def GetVolume(*args): return apply(_blcelc.iSoundSource_GetVolume,args)
    def SetFrequencyFactor(*args): return apply(_blcelc.iSoundSource_SetFrequencyFactor,args)
    def GetFrequencyFactor(*args): return apply(_blcelc.iSoundSource_GetFrequencyFactor,args)
    def GetMode3D(*args): return apply(_blcelc.iSoundSource_GetMode3D,args)
    def SetMode3D(*args): return apply(_blcelc.iSoundSource_SetMode3D,args)
    def SetPosition(*args): return apply(_blcelc.iSoundSource_SetPosition,args)
    def GetPosition(*args): return apply(_blcelc.iSoundSource_GetPosition,args)
    def SetVelocity(*args): return apply(_blcelc.iSoundSource_SetVelocity,args)
    def GetVelocity(*args): return apply(_blcelc.iSoundSource_GetVelocity,args)
    def __del__(self, destroy= _blcelc.delete_iSoundSource):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iSoundSource_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iSoundSource_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSoundSource instance at %s>" % (self.this,)

class iSoundSourcePtr(iSoundSource):
    def __init__(self,this):
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
    def Initialize(*args): return apply(_blcelc.iComponent_Initialize,args)
    def __del__(self, destroy= _blcelc.delete_iComponent):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iComponent_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iComponent_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iComponent instance at %s>" % (self.this,)

class iComponentPtr(iComponent):
    def __init__(self,this):
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
    def SetCurrentType(*args): return apply(_blcelc.iCacheManager_SetCurrentType,args)
    def GetCurrentType(*args): return apply(_blcelc.iCacheManager_GetCurrentType,args)
    def SetCurrentScope(*args): return apply(_blcelc.iCacheManager_SetCurrentScope,args)
    def GetCurrentScope(*args): return apply(_blcelc.iCacheManager_GetCurrentScope,args)
    def CacheData(*args): return apply(_blcelc.iCacheManager_CacheData,args)
    def ReadCache(*args): return apply(_blcelc.iCacheManager_ReadCache,args)
    def ClearCache(*args): return apply(_blcelc.iCacheManager_ClearCache,args)
    def __del__(self, destroy= _blcelc.delete_iCacheManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCacheManager_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCacheManager_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCacheManager instance at %s>" % (self.this,)

class iCacheManagerPtr(iCacheManager):
    def __init__(self,this):
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
    __swig_setmethods__["sec"] = _blcelc.csFileTime_sec_set
    __swig_getmethods__["sec"] = _blcelc.csFileTime_sec_get
    if _newclass:sec = property(_blcelc.csFileTime_sec_get,_blcelc.csFileTime_sec_set)
    __swig_setmethods__["min"] = _blcelc.csFileTime_min_set
    __swig_getmethods__["min"] = _blcelc.csFileTime_min_get
    if _newclass:min = property(_blcelc.csFileTime_min_get,_blcelc.csFileTime_min_set)
    __swig_setmethods__["hour"] = _blcelc.csFileTime_hour_set
    __swig_getmethods__["hour"] = _blcelc.csFileTime_hour_get
    if _newclass:hour = property(_blcelc.csFileTime_hour_get,_blcelc.csFileTime_hour_set)
    __swig_setmethods__["day"] = _blcelc.csFileTime_day_set
    __swig_getmethods__["day"] = _blcelc.csFileTime_day_get
    if _newclass:day = property(_blcelc.csFileTime_day_get,_blcelc.csFileTime_day_set)
    __swig_setmethods__["mon"] = _blcelc.csFileTime_mon_set
    __swig_getmethods__["mon"] = _blcelc.csFileTime_mon_get
    if _newclass:mon = property(_blcelc.csFileTime_mon_get,_blcelc.csFileTime_mon_set)
    __swig_setmethods__["year"] = _blcelc.csFileTime_year_set
    __swig_getmethods__["year"] = _blcelc.csFileTime_year_get
    if _newclass:year = property(_blcelc.csFileTime_year_get,_blcelc.csFileTime_year_set)
    def __init__(self,*args):
        _swig_setattr(self, csFileTime, 'this', apply(_blcelc.new_csFileTime,args))
        _swig_setattr(self, csFileTime, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csFileTime):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csFileTime instance at %s>" % (self.this,)

class csFileTimePtr(csFileTime):
    def __init__(self,this):
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
    def GetName(*args): return apply(_blcelc.iFile_GetName,args)
    def GetSize(*args): return apply(_blcelc.iFile_GetSize,args)
    def GetStatus(*args): return apply(_blcelc.iFile_GetStatus,args)
    def Read(*args): return apply(_blcelc.iFile_Read,args)
    def Write(*args): return apply(_blcelc.iFile_Write,args)
    def Flush(*args): return apply(_blcelc.iFile_Flush,args)
    def AtEOF(*args): return apply(_blcelc.iFile_AtEOF,args)
    def GetPos(*args): return apply(_blcelc.iFile_GetPos,args)
    def GetAllData(*args): return apply(_blcelc.iFile_GetAllData,args)
    def SetPos(*args): return apply(_blcelc.iFile_SetPos,args)
    def __del__(self, destroy= _blcelc.delete_iFile):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFile_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFile_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFile instance at %s>" % (self.this,)

class iFilePtr(iFile):
    def __init__(self,this):
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
    def ChDir(*args): return apply(_blcelc.iVFS_ChDir,args)
    def GetCwd(*args): return apply(_blcelc.iVFS_GetCwd,args)
    def PushDir(*args): return apply(_blcelc.iVFS_PushDir,args)
    def PopDir(*args): return apply(_blcelc.iVFS_PopDir,args)
    def ExpandPath(*args): return apply(_blcelc.iVFS_ExpandPath,args)
    def Exists(*args): return apply(_blcelc.iVFS_Exists,args)
    def FindFiles(*args): return apply(_blcelc.iVFS_FindFiles,args)
    def Open(*args): return apply(_blcelc.iVFS_Open,args)
    def ReadFile(*args): return apply(_blcelc.iVFS_ReadFile,args)
    def WriteFile(*args): return apply(_blcelc.iVFS_WriteFile,args)
    def DeleteFile(*args): return apply(_blcelc.iVFS_DeleteFile,args)
    def Sync(*args): return apply(_blcelc.iVFS_Sync,args)
    def Mount(*args): return apply(_blcelc.iVFS_Mount,args)
    def Unmount(*args): return apply(_blcelc.iVFS_Unmount,args)
    def MountRoot(*args): return apply(_blcelc.iVFS_MountRoot,args)
    def SaveMounts(*args): return apply(_blcelc.iVFS_SaveMounts,args)
    def GetFileTime(*args): return apply(_blcelc.iVFS_GetFileTime,args)
    def SetFileTime(*args): return apply(_blcelc.iVFS_SetFileTime,args)
    def GetFileSize(*args): return apply(_blcelc.iVFS_GetFileSize,args)
    def GetRealPath(*args): return apply(_blcelc.iVFS_GetRealPath,args)
    def __del__(self, destroy= _blcelc.delete_iVFS):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iVFS_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iVFS_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVFS instance at %s>" % (self.this,)

class iVFSPtr(iVFS):
    def __init__(self,this):
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
    def SetName(*args): return apply(_blcelc.iObject_SetName,args)
    def GetName(*args): return apply(_blcelc.iObject_GetName,args)
    def GetID(*args): return apply(_blcelc.iObject_GetID,args)
    def SetObjectParent(*args): return apply(_blcelc.iObject_SetObjectParent,args)
    def GetObjectParent(*args): return apply(_blcelc.iObject_GetObjectParent,args)
    def ObjAdd(*args): return apply(_blcelc.iObject_ObjAdd,args)
    def ObjRemove(*args): return apply(_blcelc.iObject_ObjRemove,args)
    def ObjRemoveAll(*args): return apply(_blcelc.iObject_ObjRemoveAll,args)
    def ObjAddChildren(*args): return apply(_blcelc.iObject_ObjAddChildren,args)
    def GetChild(*args): return apply(_blcelc.iObject_GetChild,args)
    def GetIterator(*args): return apply(_blcelc.iObject_GetIterator,args)
    def ObjReleaseOld(*args): return apply(_blcelc.iObject_ObjReleaseOld,args)
    def __del__(self, destroy= _blcelc.delete_iObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iObject_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iObject_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObject instance at %s>" % (self.this,)

class iObjectPtr(iObject):
    def __init__(self,this):
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
    def Next(*args): return apply(_blcelc.iObjectIterator_Next,args)
    def Reset(*args): return apply(_blcelc.iObjectIterator_Reset,args)
    def GetParentObj(*args): return apply(_blcelc.iObjectIterator_GetParentObj,args)
    def HasNext(*args): return apply(_blcelc.iObjectIterator_HasNext,args)
    def FindName(*args): return apply(_blcelc.iObjectIterator_FindName,args)
    def __del__(self, destroy= _blcelc.delete_iObjectIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectIterator instance at %s>" % (self.this,)

class iObjectIteratorPtr(iObjectIterator):
    def __init__(self,this):
        _swig_setattr(self, iObjectIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iObjectIterator, 'thisown', 0)
        _swig_setattr(self, iObjectIterator,self.__class__,iObjectIterator)
_blcelc.iObjectIterator_swigregister(iObjectIteratorPtr)

class iDataObject(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iDataObject, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iDataObject, name)
    def QueryObject(*args): return apply(_blcelc.iDataObject_QueryObject,args)
    def GetData(*args): return apply(_blcelc.iDataObject_GetData,args)
    def __del__(self, destroy= _blcelc.delete_iDataObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDataObject instance at %s>" % (self.this,)

class iDataObjectPtr(iDataObject):
    def __init__(self,this):
        _swig_setattr(self, iDataObject, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDataObject, 'thisown', 0)
        _swig_setattr(self, iDataObject,self.__class__,iDataObject)
_blcelc.iDataObject_swigregister(iDataObjectPtr)

class iObjectRegistry(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iObjectRegistry, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iObjectRegistry, name)
    def Clear(*args): return apply(_blcelc.iObjectRegistry_Clear,args)
    def Register(*args): return apply(_blcelc.iObjectRegistry_Register,args)
    def Unregister(*args): return apply(_blcelc.iObjectRegistry_Unregister,args)
    def Get(*args): return apply(_blcelc.iObjectRegistry_Get,args)
    def __del__(self, destroy= _blcelc.delete_iObjectRegistry):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iObjectRegistry_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iObjectRegistry_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectRegistry instance at %s>" % (self.this,)

class iObjectRegistryPtr(iObjectRegistry):
    def __init__(self,this):
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
    def Reset(*args): return apply(_blcelc.iObjectRegistryIterator_Reset,args)
    def GetCurrentTag(*args): return apply(_blcelc.iObjectRegistryIterator_GetCurrentTag,args)
    def HasNext(*args): return apply(_blcelc.iObjectRegistryIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.iObjectRegistryIterator_Next,args)
    def __del__(self, destroy= _blcelc.delete_iObjectRegistryIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iObjectRegistryIterator instance at %s>" % (self.this,)

class iObjectRegistryIteratorPtr(iObjectRegistryIterator):
    def __init__(self,this):
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
    def Advance(*args): return apply(_blcelc.iVirtualClock_Advance,args)
    def Suspend(*args): return apply(_blcelc.iVirtualClock_Suspend,args)
    def Resume(*args): return apply(_blcelc.iVirtualClock_Resume,args)
    def GetElapsedTicks(*args): return apply(_blcelc.iVirtualClock_GetElapsedTicks,args)
    def GetCurrentTicks(*args): return apply(_blcelc.iVirtualClock_GetCurrentTicks,args)
    def __del__(self, destroy= _blcelc.delete_iVirtualClock):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iVirtualClock_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iVirtualClock_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVirtualClock instance at %s>" % (self.this,)

class iVirtualClockPtr(iVirtualClock):
    def __init__(self,this):
        _swig_setattr(self, iVirtualClock, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVirtualClock, 'thisown', 0)
        _swig_setattr(self, iVirtualClock,self.__class__,iVirtualClock)
_blcelc.iVirtualClock_swigregister(iVirtualClockPtr)
iVirtualClock_scfGetVersion = _blcelc.iVirtualClock_scfGetVersion


CS_CRYSTAL_PROTOCOL = _blcelc.CS_CRYSTAL_PROTOCOL
CS_MUSCLE_PROTOCOL = _blcelc.CS_MUSCLE_PROTOCOL
CS_XML_PROTOCOL = _blcelc.CS_XML_PROTOCOL
class csKeyEventData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csKeyEventData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csKeyEventData, name)
    __swig_setmethods__["eventType"] = _blcelc.csKeyEventData_eventType_set
    __swig_getmethods__["eventType"] = _blcelc.csKeyEventData_eventType_get
    if _newclass:eventType = property(_blcelc.csKeyEventData_eventType_get,_blcelc.csKeyEventData_eventType_set)
    __swig_setmethods__["codeRaw"] = _blcelc.csKeyEventData_codeRaw_set
    __swig_getmethods__["codeRaw"] = _blcelc.csKeyEventData_codeRaw_get
    if _newclass:codeRaw = property(_blcelc.csKeyEventData_codeRaw_get,_blcelc.csKeyEventData_codeRaw_set)
    __swig_setmethods__["codeCooked"] = _blcelc.csKeyEventData_codeCooked_set
    __swig_getmethods__["codeCooked"] = _blcelc.csKeyEventData_codeCooked_get
    if _newclass:codeCooked = property(_blcelc.csKeyEventData_codeCooked_get,_blcelc.csKeyEventData_codeCooked_set)
    __swig_setmethods__["modifiers"] = _blcelc.csKeyEventData_modifiers_set
    __swig_getmethods__["modifiers"] = _blcelc.csKeyEventData_modifiers_get
    if _newclass:modifiers = property(_blcelc.csKeyEventData_modifiers_get,_blcelc.csKeyEventData_modifiers_set)
    __swig_setmethods__["autoRepeat"] = _blcelc.csKeyEventData_autoRepeat_set
    __swig_getmethods__["autoRepeat"] = _blcelc.csKeyEventData_autoRepeat_get
    if _newclass:autoRepeat = property(_blcelc.csKeyEventData_autoRepeat_get,_blcelc.csKeyEventData_autoRepeat_set)
    __swig_setmethods__["charType"] = _blcelc.csKeyEventData_charType_set
    __swig_getmethods__["charType"] = _blcelc.csKeyEventData_charType_get
    if _newclass:charType = property(_blcelc.csKeyEventData_charType_get,_blcelc.csKeyEventData_charType_set)
    def __init__(self,*args):
        _swig_setattr(self, csKeyEventData, 'this', apply(_blcelc.new_csKeyEventData,args))
        _swig_setattr(self, csKeyEventData, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csKeyEventData):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csKeyEventData instance at %s>" % (self.this,)

class csKeyEventDataPtr(csKeyEventData):
    def __init__(self,this):
        _swig_setattr(self, csKeyEventData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csKeyEventData, 'thisown', 0)
        _swig_setattr(self, csKeyEventData,self.__class__,csKeyEventData)
_blcelc.csKeyEventData_swigregister(csKeyEventDataPtr)

class csEventMouseData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csEventMouseData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csEventMouseData, name)
    __swig_setmethods__["x"] = _blcelc.csEventMouseData_x_set
    __swig_getmethods__["x"] = _blcelc.csEventMouseData_x_get
    if _newclass:x = property(_blcelc.csEventMouseData_x_get,_blcelc.csEventMouseData_x_set)
    __swig_setmethods__["y"] = _blcelc.csEventMouseData_y_set
    __swig_getmethods__["y"] = _blcelc.csEventMouseData_y_get
    if _newclass:y = property(_blcelc.csEventMouseData_y_get,_blcelc.csEventMouseData_y_set)
    __swig_setmethods__["Button"] = _blcelc.csEventMouseData_Button_set
    __swig_getmethods__["Button"] = _blcelc.csEventMouseData_Button_get
    if _newclass:Button = property(_blcelc.csEventMouseData_Button_get,_blcelc.csEventMouseData_Button_set)
    __swig_setmethods__["Modifiers"] = _blcelc.csEventMouseData_Modifiers_set
    __swig_getmethods__["Modifiers"] = _blcelc.csEventMouseData_Modifiers_get
    if _newclass:Modifiers = property(_blcelc.csEventMouseData_Modifiers_get,_blcelc.csEventMouseData_Modifiers_set)
    def __init__(self,*args):
        _swig_setattr(self, csEventMouseData, 'this', apply(_blcelc.new_csEventMouseData,args))
        _swig_setattr(self, csEventMouseData, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csEventMouseData):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csEventMouseData instance at %s>" % (self.this,)

class csEventMouseDataPtr(csEventMouseData):
    def __init__(self,this):
        _swig_setattr(self, csEventMouseData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csEventMouseData, 'thisown', 0)
        _swig_setattr(self, csEventMouseData,self.__class__,csEventMouseData)
_blcelc.csEventMouseData_swigregister(csEventMouseDataPtr)

class csEventJoystickData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csEventJoystickData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csEventJoystickData, name)
    __swig_setmethods__["number"] = _blcelc.csEventJoystickData_number_set
    __swig_getmethods__["number"] = _blcelc.csEventJoystickData_number_get
    if _newclass:number = property(_blcelc.csEventJoystickData_number_get,_blcelc.csEventJoystickData_number_set)
    __swig_setmethods__["x"] = _blcelc.csEventJoystickData_x_set
    __swig_getmethods__["x"] = _blcelc.csEventJoystickData_x_get
    if _newclass:x = property(_blcelc.csEventJoystickData_x_get,_blcelc.csEventJoystickData_x_set)
    __swig_setmethods__["y"] = _blcelc.csEventJoystickData_y_set
    __swig_getmethods__["y"] = _blcelc.csEventJoystickData_y_get
    if _newclass:y = property(_blcelc.csEventJoystickData_y_get,_blcelc.csEventJoystickData_y_set)
    __swig_setmethods__["Button"] = _blcelc.csEventJoystickData_Button_set
    __swig_getmethods__["Button"] = _blcelc.csEventJoystickData_Button_get
    if _newclass:Button = property(_blcelc.csEventJoystickData_Button_get,_blcelc.csEventJoystickData_Button_set)
    __swig_setmethods__["Modifiers"] = _blcelc.csEventJoystickData_Modifiers_set
    __swig_getmethods__["Modifiers"] = _blcelc.csEventJoystickData_Modifiers_get
    if _newclass:Modifiers = property(_blcelc.csEventJoystickData_Modifiers_get,_blcelc.csEventJoystickData_Modifiers_set)
    def __init__(self,*args):
        _swig_setattr(self, csEventJoystickData, 'this', apply(_blcelc.new_csEventJoystickData,args))
        _swig_setattr(self, csEventJoystickData, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csEventJoystickData):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csEventJoystickData instance at %s>" % (self.this,)

class csEventJoystickDataPtr(csEventJoystickData):
    def __init__(self,this):
        _swig_setattr(self, csEventJoystickData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csEventJoystickData, 'thisown', 0)
        _swig_setattr(self, csEventJoystickData,self.__class__,csEventJoystickData)
_blcelc.csEventJoystickData_swigregister(csEventJoystickDataPtr)

class csEventCommandData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csEventCommandData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csEventCommandData, name)
    __swig_setmethods__["Code"] = _blcelc.csEventCommandData_Code_set
    __swig_getmethods__["Code"] = _blcelc.csEventCommandData_Code_get
    if _newclass:Code = property(_blcelc.csEventCommandData_Code_get,_blcelc.csEventCommandData_Code_set)
    __swig_setmethods__["Info"] = _blcelc.csEventCommandData_Info_set
    __swig_getmethods__["Info"] = _blcelc.csEventCommandData_Info_get
    if _newclass:Info = property(_blcelc.csEventCommandData_Info_get,_blcelc.csEventCommandData_Info_set)
    def __init__(self,*args):
        _swig_setattr(self, csEventCommandData, 'this', apply(_blcelc.new_csEventCommandData,args))
        _swig_setattr(self, csEventCommandData, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csEventCommandData):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csEventCommandData instance at %s>" % (self.this,)

class csEventCommandDataPtr(csEventCommandData):
    def __init__(self,this):
        _swig_setattr(self, csEventCommandData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csEventCommandData, 'thisown', 0)
        _swig_setattr(self, csEventCommandData,self.__class__,csEventCommandData)
_blcelc.csEventCommandData_swigregister(csEventCommandDataPtr)

class csEventNetworkData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csEventNetworkData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csEventNetworkData, name)
    __swig_getmethods__["From"] = _blcelc.csEventNetworkData_From_get
    if _newclass:From = property(_blcelc.csEventNetworkData_From_get)
    __swig_getmethods__["From2"] = _blcelc.csEventNetworkData_From2_get
    if _newclass:From2 = property(_blcelc.csEventNetworkData_From2_get)
    __swig_getmethods__["Data"] = _blcelc.csEventNetworkData_Data_get
    if _newclass:Data = property(_blcelc.csEventNetworkData_Data_get)
    __swig_getmethods__["Data2"] = _blcelc.csEventNetworkData_Data2_get
    if _newclass:Data2 = property(_blcelc.csEventNetworkData_Data2_get)
    def __init__(self,*args):
        _swig_setattr(self, csEventNetworkData, 'this', apply(_blcelc.new_csEventNetworkData,args))
        _swig_setattr(self, csEventNetworkData, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csEventNetworkData):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csEventNetworkData instance at %s>" % (self.this,)

class csEventNetworkDataPtr(csEventNetworkData):
    def __init__(self,this):
        _swig_setattr(self, csEventNetworkData, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csEventNetworkData, 'thisown', 0)
        _swig_setattr(self, csEventNetworkData,self.__class__,csEventNetworkData)
_blcelc.csEventNetworkData_swigregister(csEventNetworkDataPtr)

class iEvent(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iEvent, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iEvent, name)
    __swig_setmethods__["Type"] = _blcelc.iEvent_Type_set
    __swig_getmethods__["Type"] = _blcelc.iEvent_Type_get
    if _newclass:Type = property(_blcelc.iEvent_Type_get,_blcelc.iEvent_Type_set)
    __swig_setmethods__["Category"] = _blcelc.iEvent_Category_set
    __swig_getmethods__["Category"] = _blcelc.iEvent_Category_get
    if _newclass:Category = property(_blcelc.iEvent_Category_get,_blcelc.iEvent_Category_set)
    __swig_setmethods__["SubCategory"] = _blcelc.iEvent_SubCategory_set
    __swig_getmethods__["SubCategory"] = _blcelc.iEvent_SubCategory_get
    if _newclass:SubCategory = property(_blcelc.iEvent_SubCategory_get,_blcelc.iEvent_SubCategory_set)
    __swig_setmethods__["Flags"] = _blcelc.iEvent_Flags_set
    __swig_getmethods__["Flags"] = _blcelc.iEvent_Flags_get
    if _newclass:Flags = property(_blcelc.iEvent_Flags_get,_blcelc.iEvent_Flags_set)
    __swig_setmethods__["Time"] = _blcelc.iEvent_Time_set
    __swig_getmethods__["Time"] = _blcelc.iEvent_Time_get
    if _newclass:Time = property(_blcelc.iEvent_Time_get,_blcelc.iEvent_Time_set)
    def AddInt8(*args): return apply(_blcelc.iEvent_AddInt8,args)
    def AddUInt8(*args): return apply(_blcelc.iEvent_AddUInt8,args)
    def AddInt16(*args): return apply(_blcelc.iEvent_AddInt16,args)
    def AddUInt16(*args): return apply(_blcelc.iEvent_AddUInt16,args)
    def AddInt32(*args): return apply(_blcelc.iEvent_AddInt32,args)
    def AddUInt32(*args): return apply(_blcelc.iEvent_AddUInt32,args)
    def AddFloat(*args): return apply(_blcelc.iEvent_AddFloat,args)
    def AddDouble(*args): return apply(_blcelc.iEvent_AddDouble,args)
    def AddString(*args): return apply(_blcelc.iEvent_AddString,args)
    def AddVoidPtr(*args): return apply(_blcelc.iEvent_AddVoidPtr,args)
    def AddBool(*args): return apply(_blcelc.iEvent_AddBool,args)
    def Add(*args): return apply(_blcelc.iEvent_Add,args)
    def FindInt8(*args): return apply(_blcelc.iEvent_FindInt8,args)
    def FindUInt8(*args): return apply(_blcelc.iEvent_FindUInt8,args)
    def FindInt16(*args): return apply(_blcelc.iEvent_FindInt16,args)
    def FindUInt16(*args): return apply(_blcelc.iEvent_FindUInt16,args)
    def FindUInt32(*args): return apply(_blcelc.iEvent_FindUInt32,args)
    def FindFloat(*args): return apply(_blcelc.iEvent_FindFloat,args)
    def FindDouble(*args): return apply(_blcelc.iEvent_FindDouble,args)
    def FindString(*args): return apply(_blcelc.iEvent_FindString,args)
    def FindVoidPtr(*args): return apply(_blcelc.iEvent_FindVoidPtr,args)
    def FindBool(*args): return apply(_blcelc.iEvent_FindBool,args)
    def Find(*args): return apply(_blcelc.iEvent_Find,args)
    def Remove(*args): return apply(_blcelc.iEvent_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iEvent_RemoveAll,args)
    def Print(*args): return apply(_blcelc.iEvent_Print,args)
    def FlattenSize(*args): return apply(_blcelc.iEvent_FlattenSize,args)
    def Flatten(*args): return apply(_blcelc.iEvent_Flatten,args)
    def Unflatten(*args): return apply(_blcelc.iEvent_Unflatten,args)
    def __del__(self, destroy= _blcelc.delete_iEvent):
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
    __swig_getmethods__["Network"] = _blcelc.iEvent_Network_get
    if _newclass:Network = property(_blcelc.iEvent_Network_get)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEvent instance at %s>" % (self.this,)

class iEventPtr(iEvent):
    def __init__(self,this):
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
    def GetPotentiallyConflictingEvents(*args): return apply(_blcelc.iEventPlug_GetPotentiallyConflictingEvents,args)
    def QueryEventPriority(*args): return apply(_blcelc.iEventPlug_QueryEventPriority,args)
    def EnableEvents(*args): return apply(_blcelc.iEventPlug_EnableEvents,args)
    def __del__(self, destroy= _blcelc.delete_iEventPlug):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventPlug instance at %s>" % (self.this,)

class iEventPlugPtr(iEventPlug):
    def __init__(self,this):
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
    def CreateEvent(*args): return apply(_blcelc.iEventOutlet_CreateEvent,args)
    def Post(*args): return apply(_blcelc.iEventOutlet_Post,args)
    def Key(*args): return apply(_blcelc.iEventOutlet_Key,args)
    def Mouse(*args): return apply(_blcelc.iEventOutlet_Mouse,args)
    def Joystick(*args): return apply(_blcelc.iEventOutlet_Joystick,args)
    def Broadcast(*args): return apply(_blcelc.iEventOutlet_Broadcast,args)
    def ImmediateBroadcast(*args): return apply(_blcelc.iEventOutlet_ImmediateBroadcast,args)
    def __del__(self, destroy= _blcelc.delete_iEventOutlet):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventOutlet instance at %s>" % (self.this,)

class iEventOutletPtr(iEventOutlet):
    def __init__(self,this):
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
    def Insert(*args): return apply(_blcelc.iEventCord_Insert,args)
    def Remove(*args): return apply(_blcelc.iEventCord_Remove,args)
    def GetPass(*args): return apply(_blcelc.iEventCord_GetPass,args)
    def SetPass(*args): return apply(_blcelc.iEventCord_SetPass,args)
    def GetCategory(*args): return apply(_blcelc.iEventCord_GetCategory,args)
    def GetSubcategory(*args): return apply(_blcelc.iEventCord_GetSubcategory,args)
    def __del__(self, destroy= _blcelc.delete_iEventCord):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventCord instance at %s>" % (self.this,)

class iEventCordPtr(iEventCord):
    def __init__(self,this):
        _swig_setattr(self, iEventCord, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iEventCord, 'thisown', 0)
        _swig_setattr(self, iEventCord,self.__class__,iEventCord)
_blcelc.iEventCord_swigregister(iEventCordPtr)

class csKeyEventHelper(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csKeyEventHelper, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csKeyEventHelper, name)
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
    def __init__(self,*args):
        _swig_setattr(self, csKeyEventHelper, 'this', apply(_blcelc.new_csKeyEventHelper,args))
        _swig_setattr(self, csKeyEventHelper, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csKeyEventHelper):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csKeyEventHelper instance at %s>" % (self.this,)

class csKeyEventHelperPtr(csKeyEventHelper):
    def __init__(self,this):
        _swig_setattr(self, csKeyEventHelper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csKeyEventHelper, 'thisown', 0)
        _swig_setattr(self, csKeyEventHelper,self.__class__,csKeyEventHelper)
_blcelc.csKeyEventHelper_swigregister(csKeyEventHelperPtr)
csKeyEventHelper_GetRawCode = _blcelc.csKeyEventHelper_GetRawCode

csKeyEventHelper_GetCookedCode = _blcelc.csKeyEventHelper_GetCookedCode

csKeyEventHelper_GetModifiers = _blcelc.csKeyEventHelper_GetModifiers

csKeyEventHelper_GetEventType = _blcelc.csKeyEventHelper_GetEventType

csKeyEventHelper_GetAutoRepeat = _blcelc.csKeyEventHelper_GetAutoRepeat

csKeyEventHelper_GetCharacterType = _blcelc.csKeyEventHelper_GetCharacterType

csKeyEventHelper_GetEventData = _blcelc.csKeyEventHelper_GetEventData

csKeyEventHelper_GetModifiersBits = _blcelc.csKeyEventHelper_GetModifiersBits


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
csevNetwork = _blcelc.csevNetwork
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
    __swig_setmethods__["modifiers"] = _blcelc.csKeyModifiers_modifiers_set
    __swig_getmethods__["modifiers"] = _blcelc.csKeyModifiers_modifiers_get
    if _newclass:modifiers = property(_blcelc.csKeyModifiers_modifiers_get,_blcelc.csKeyModifiers_modifiers_set)
    def __init__(self,*args):
        _swig_setattr(self, csKeyModifiers, 'this', apply(_blcelc.new_csKeyModifiers,args))
        _swig_setattr(self, csKeyModifiers, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csKeyModifiers):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csKeyModifiers instance at %s>" % (self.this,)

class csKeyModifiersPtr(csKeyModifiers):
    def __init__(self,this):
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
    def Process(*args): return apply(_blcelc.iEventQueue_Process,args)
    def Dispatch(*args): return apply(_blcelc.iEventQueue_Dispatch,args)
    def RegisterListener(*args): return apply(_blcelc.iEventQueue_RegisterListener,args)
    def RemoveListener(*args): return apply(_blcelc.iEventQueue_RemoveListener,args)
    def ChangeListenerTrigger(*args): return apply(_blcelc.iEventQueue_ChangeListenerTrigger,args)
    def CreateEventOutlet(*args): return apply(_blcelc.iEventQueue_CreateEventOutlet,args)
    def GetEventOutlet(*args): return apply(_blcelc.iEventQueue_GetEventOutlet,args)
    def GetEventCord(*args): return apply(_blcelc.iEventQueue_GetEventCord,args)
    def CreateEvent(*args): return apply(_blcelc.iEventQueue_CreateEvent,args)
    def Post(*args): return apply(_blcelc.iEventQueue_Post,args)
    def Get(*args): return apply(_blcelc.iEventQueue_Get,args)
    def Clear(*args): return apply(_blcelc.iEventQueue_Clear,args)
    def IsEmpty(*args): return apply(_blcelc.iEventQueue_IsEmpty,args)
    def RemoveAllListeners(*args): return apply(_blcelc.iEventQueue_RemoveAllListeners,args)
    def __del__(self, destroy= _blcelc.delete_iEventQueue):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iEventQueue_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iEventQueue_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventQueue instance at %s>" % (self.this,)

class iEventQueuePtr(iEventQueue):
    def __init__(self,this):
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
    def HandleEvent(*args): return apply(_blcelc.iEventHandler_HandleEvent,args)
    def __del__(self, destroy= _blcelc.delete_iEventHandler):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iEventHandler_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iEventHandler_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEventHandler instance at %s>" % (self.this,)

class iEventHandlerPtr(iEventHandler):
    def __init__(self,this):
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
    def HasNext(*args): return apply(_blcelc.iPluginIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.iPluginIterator_Next,args)
    def __del__(self, destroy= _blcelc.delete_iPluginIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPluginIterator instance at %s>" % (self.this,)

class iPluginIteratorPtr(iPluginIterator):
    def __init__(self,this):
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
    def LoadPlugin(*args): return apply(_blcelc.iPluginManager_LoadPlugin,args)
    def QueryPlugin(*args): return apply(_blcelc.iPluginManager_QueryPlugin,args)
    def UnloadPlugin(*args): return apply(_blcelc.iPluginManager_UnloadPlugin,args)
    def RegisterPlugin(*args): return apply(_blcelc.iPluginManager_RegisterPlugin,args)
    def GetPlugins(*args): return apply(_blcelc.iPluginManager_GetPlugins,args)
    def Clear(*args): return apply(_blcelc.iPluginManager_Clear,args)
    def QueryOptions(*args): return apply(_blcelc.iPluginManager_QueryOptions,args)
    def __del__(self, destroy= _blcelc.delete_iPluginManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iPluginManager_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iPluginManager_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPluginManager instance at %s>" % (self.this,)

class iPluginManagerPtr(iPluginManager):
    def __init__(self,this):
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
    def HandleKey(*args): return apply(_blcelc.iKeyComposer_HandleKey,args)
    def ResetState(*args): return apply(_blcelc.iKeyComposer_ResetState,args)
    def __del__(self, destroy= _blcelc.delete_iKeyComposer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iKeyComposer instance at %s>" % (self.this,)

class iKeyComposerPtr(iKeyComposer):
    def __init__(self,this):
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
    def Reset(*args): return apply(_blcelc.iKeyboardDriver_Reset,args)
    def DoKey(*args): return apply(_blcelc.iKeyboardDriver_DoKey,args)
    def GetModifierState(*args): return apply(_blcelc.iKeyboardDriver_GetModifierState,args)
    def CreateKeyComposer(*args): return apply(_blcelc.iKeyboardDriver_CreateKeyComposer,args)
    def __del__(self, destroy= _blcelc.delete_iKeyboardDriver):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iKeyboardDriver_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iKeyboardDriver_scfGetVersion)
    def GetKeyState(*args): return apply(_blcelc.iKeyboardDriver_GetKeyState,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iKeyboardDriver instance at %s>" % (self.this,)

class iKeyboardDriverPtr(iKeyboardDriver):
    def __init__(self,this):
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
    def SetDoubleClickTime(*args): return apply(_blcelc.iMouseDriver_SetDoubleClickTime,args)
    def Reset(*args): return apply(_blcelc.iMouseDriver_Reset,args)
    def GetLastX(*args): return apply(_blcelc.iMouseDriver_GetLastX,args)
    def GetLastY(*args): return apply(_blcelc.iMouseDriver_GetLastY,args)
    def GetLastButton(*args): return apply(_blcelc.iMouseDriver_GetLastButton,args)
    def DoButton(*args): return apply(_blcelc.iMouseDriver_DoButton,args)
    def DoMotion(*args): return apply(_blcelc.iMouseDriver_DoMotion,args)
    def __del__(self, destroy= _blcelc.delete_iMouseDriver):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMouseDriver instance at %s>" % (self.this,)

class iMouseDriverPtr(iMouseDriver):
    def __init__(self,this):
        _swig_setattr(self, iMouseDriver, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMouseDriver, 'thisown', 0)
        _swig_setattr(self, iMouseDriver,self.__class__,iMouseDriver)
_blcelc.iMouseDriver_swigregister(iMouseDriverPtr)

class iJoystickDriver(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iJoystickDriver, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iJoystickDriver, name)
    def Reset(*args): return apply(_blcelc.iJoystickDriver_Reset,args)
    def GetLastX(*args): return apply(_blcelc.iJoystickDriver_GetLastX,args)
    def GetLastY(*args): return apply(_blcelc.iJoystickDriver_GetLastY,args)
    def GetLastButton(*args): return apply(_blcelc.iJoystickDriver_GetLastButton,args)
    def DoButton(*args): return apply(_blcelc.iJoystickDriver_DoButton,args)
    def DoMotion(*args): return apply(_blcelc.iJoystickDriver_DoMotion,args)
    def __del__(self, destroy= _blcelc.delete_iJoystickDriver):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iJoystickDriver instance at %s>" % (self.this,)

class iJoystickDriverPtr(iJoystickDriver):
    def __init__(self,this):
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
    def GetFileName(*args): return apply(_blcelc.iConfigFile_GetFileName,args)
    def GetVFS(*args): return apply(_blcelc.iConfigFile_GetVFS,args)
    def SetFileName(*args): return apply(_blcelc.iConfigFile_SetFileName,args)
    def Load(*args): return apply(_blcelc.iConfigFile_Load,args)
    def Save(*args): return apply(_blcelc.iConfigFile_Save,args)
    def Clear(*args): return apply(_blcelc.iConfigFile_Clear,args)
    def Enumerate(*args): return apply(_blcelc.iConfigFile_Enumerate,args)
    def KeyExists(*args): return apply(_blcelc.iConfigFile_KeyExists,args)
    def SubsectionExists(*args): return apply(_blcelc.iConfigFile_SubsectionExists,args)
    def GetInt(*args): return apply(_blcelc.iConfigFile_GetInt,args)
    def GetFloat(*args): return apply(_blcelc.iConfigFile_GetFloat,args)
    def GetStr(*args): return apply(_blcelc.iConfigFile_GetStr,args)
    def GetBool(*args): return apply(_blcelc.iConfigFile_GetBool,args)
    def GetComment(*args): return apply(_blcelc.iConfigFile_GetComment,args)
    def SetStr(*args): return apply(_blcelc.iConfigFile_SetStr,args)
    def SetInt(*args): return apply(_blcelc.iConfigFile_SetInt,args)
    def SetFloat(*args): return apply(_blcelc.iConfigFile_SetFloat,args)
    def SetBool(*args): return apply(_blcelc.iConfigFile_SetBool,args)
    def SetComment(*args): return apply(_blcelc.iConfigFile_SetComment,args)
    def DeleteKey(*args): return apply(_blcelc.iConfigFile_DeleteKey,args)
    def GetEOFComment(*args): return apply(_blcelc.iConfigFile_GetEOFComment,args)
    def SetEOFComment(*args): return apply(_blcelc.iConfigFile_SetEOFComment,args)
    def __del__(self, destroy= _blcelc.delete_iConfigFile):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iConfigFile_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iConfigFile_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConfigFile instance at %s>" % (self.this,)

class iConfigFilePtr(iConfigFile):
    def __init__(self,this):
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
    def GetConfigFile(*args): return apply(_blcelc.iConfigIterator_GetConfigFile,args)
    def GetSubsection(*args): return apply(_blcelc.iConfigIterator_GetSubsection,args)
    def Rewind(*args): return apply(_blcelc.iConfigIterator_Rewind,args)
    def Next(*args): return apply(_blcelc.iConfigIterator_Next,args)
    def GetKey(*args): return apply(_blcelc.iConfigIterator_GetKey,args)
    def GetInt(*args): return apply(_blcelc.iConfigIterator_GetInt,args)
    def GetFloat(*args): return apply(_blcelc.iConfigIterator_GetFloat,args)
    def GetStr(*args): return apply(_blcelc.iConfigIterator_GetStr,args)
    def GetBool(*args): return apply(_blcelc.iConfigIterator_GetBool,args)
    def GetComment(*args): return apply(_blcelc.iConfigIterator_GetComment,args)
    def __del__(self, destroy= _blcelc.delete_iConfigIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iConfigIterator_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iConfigIterator_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConfigIterator instance at %s>" % (self.this,)

class iConfigIteratorPtr(iConfigIterator):
    def __init__(self,this):
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
    def AddDomain(*args): return apply(_blcelc.iConfigManager_AddDomain,args)
    def RemoveDomain(*args): return apply(_blcelc.iConfigManager_RemoveDomain,args)
    def LookupDomain(*args): return apply(_blcelc.iConfigManager_LookupDomain,args)
    def SetDomainPriority(*args): return apply(_blcelc.iConfigManager_SetDomainPriority,args)
    def GetDomainPriority(*args): return apply(_blcelc.iConfigManager_GetDomainPriority,args)
    def SetDynamicDomain(*args): return apply(_blcelc.iConfigManager_SetDynamicDomain,args)
    def GetDynamicDomain(*args): return apply(_blcelc.iConfigManager_GetDynamicDomain,args)
    def SetDynamicDomainPriority(*args): return apply(_blcelc.iConfigManager_SetDynamicDomainPriority,args)
    def GetDynamicDomainPriority(*args): return apply(_blcelc.iConfigManager_GetDynamicDomainPriority,args)
    def FlushRemoved(*args): return apply(_blcelc.iConfigManager_FlushRemoved,args)
    def __del__(self, destroy= _blcelc.delete_iConfigManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iConfigManager_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iConfigManager_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConfigManager instance at %s>" % (self.this,)

class iConfigManagerPtr(iConfigManager):
    def __init__(self,this):
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
    def Length(*args): return apply(_blcelc.iStringArray_Length,args)
    def Push(*args): return apply(_blcelc.iStringArray_Push,args)
    def Pop(*args): return apply(_blcelc.iStringArray_Pop,args)
    def Get(*args): return apply(_blcelc.iStringArray_Get,args)
    def Find(*args): return apply(_blcelc.iStringArray_Find,args)
    def FindCaseInsensitive(*args): return apply(_blcelc.iStringArray_FindCaseInsensitive,args)
    def FindSortedKey(*args): return apply(_blcelc.iStringArray_FindSortedKey,args)
    def Sort(*args): return apply(_blcelc.iStringArray_Sort,args)
    def DeleteIndex(*args): return apply(_blcelc.iStringArray_DeleteIndex,args)
    def Insert(*args): return apply(_blcelc.iStringArray_Insert,args)
    def DeleteAll(*args): return apply(_blcelc.iStringArray_DeleteAll,args)
    def __del__(self, destroy= _blcelc.delete_iStringArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStringArray_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStringArray_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStringArray instance at %s>" % (self.this,)

class iStringArrayPtr(iStringArray):
    def __init__(self,this):
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
    def HasNext(*args): return apply(_blcelc.iDocumentAttributeIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.iDocumentAttributeIterator_Next,args)
    def __del__(self, destroy= _blcelc.delete_iDocumentAttributeIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentAttributeIterator instance at %s>" % (self.this,)

class iDocumentAttributeIteratorPtr(iDocumentAttributeIterator):
    def __init__(self,this):
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
    def GetName(*args): return apply(_blcelc.iDocumentAttribute_GetName,args)
    def GetValue(*args): return apply(_blcelc.iDocumentAttribute_GetValue,args)
    def GetValueAsInt(*args): return apply(_blcelc.iDocumentAttribute_GetValueAsInt,args)
    def GetValueAsFloat(*args): return apply(_blcelc.iDocumentAttribute_GetValueAsFloat,args)
    def GetValueAsBool(*args): return apply(_blcelc.iDocumentAttribute_GetValueAsBool,args)
    def SetName(*args): return apply(_blcelc.iDocumentAttribute_SetName,args)
    def SetValue(*args): return apply(_blcelc.iDocumentAttribute_SetValue,args)
    def SetValueAsInt(*args): return apply(_blcelc.iDocumentAttribute_SetValueAsInt,args)
    def SetValueAsFloat(*args): return apply(_blcelc.iDocumentAttribute_SetValueAsFloat,args)
    def __del__(self, destroy= _blcelc.delete_iDocumentAttribute):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentAttribute instance at %s>" % (self.this,)

class iDocumentAttributePtr(iDocumentAttribute):
    def __init__(self,this):
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
    def HasNext(*args): return apply(_blcelc.iDocumentNodeIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.iDocumentNodeIterator_Next,args)
    def __del__(self, destroy= _blcelc.delete_iDocumentNodeIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentNodeIterator instance at %s>" % (self.this,)

class iDocumentNodeIteratorPtr(iDocumentNodeIterator):
    def __init__(self,this):
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
    def GetType(*args): return apply(_blcelc.iDocumentNode_GetType,args)
    def Equals(*args): return apply(_blcelc.iDocumentNode_Equals,args)
    def GetValue(*args): return apply(_blcelc.iDocumentNode_GetValue,args)
    def SetValue(*args): return apply(_blcelc.iDocumentNode_SetValue,args)
    def SetValueAsInt(*args): return apply(_blcelc.iDocumentNode_SetValueAsInt,args)
    def SetValueAsFloat(*args): return apply(_blcelc.iDocumentNode_SetValueAsFloat,args)
    def GetParent(*args): return apply(_blcelc.iDocumentNode_GetParent,args)
    def GetNodes(*args): return apply(_blcelc.iDocumentNode_GetNodes,args)
    def GetNode(*args): return apply(_blcelc.iDocumentNode_GetNode,args)
    def RemoveNode(*args): return apply(_blcelc.iDocumentNode_RemoveNode,args)
    def RemoveNodes(*args): return apply(_blcelc.iDocumentNode_RemoveNodes,args)
    def CreateNodeBefore(*args): return apply(_blcelc.iDocumentNode_CreateNodeBefore,args)
    def GetContentsValue(*args): return apply(_blcelc.iDocumentNode_GetContentsValue,args)
    def GetContentsValueAsInt(*args): return apply(_blcelc.iDocumentNode_GetContentsValueAsInt,args)
    def GetContentsValueAsFloat(*args): return apply(_blcelc.iDocumentNode_GetContentsValueAsFloat,args)
    def GetAttributes(*args): return apply(_blcelc.iDocumentNode_GetAttributes,args)
    def GetAttribute(*args): return apply(_blcelc.iDocumentNode_GetAttribute,args)
    def GetAttributeValue(*args): return apply(_blcelc.iDocumentNode_GetAttributeValue,args)
    def GetAttributeValueAsInt(*args): return apply(_blcelc.iDocumentNode_GetAttributeValueAsInt,args)
    def GetAttributeValueAsFloat(*args): return apply(_blcelc.iDocumentNode_GetAttributeValueAsFloat,args)
    def GetAttributeValueAsBool(*args): return apply(_blcelc.iDocumentNode_GetAttributeValueAsBool,args)
    def RemoveAttribute(*args): return apply(_blcelc.iDocumentNode_RemoveAttribute,args)
    def RemoveAttributes(*args): return apply(_blcelc.iDocumentNode_RemoveAttributes,args)
    def SetAttribute(*args): return apply(_blcelc.iDocumentNode_SetAttribute,args)
    def SetAttributeAsInt(*args): return apply(_blcelc.iDocumentNode_SetAttributeAsInt,args)
    def SetAttributeAsFloat(*args): return apply(_blcelc.iDocumentNode_SetAttributeAsFloat,args)
    def __del__(self, destroy= _blcelc.delete_iDocumentNode):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentNode instance at %s>" % (self.this,)

class iDocumentNodePtr(iDocumentNode):
    def __init__(self,this):
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
    def Clear(*args): return apply(_blcelc.iDocument_Clear,args)
    def CreateRoot(*args): return apply(_blcelc.iDocument_CreateRoot,args)
    def GetRoot(*args): return apply(_blcelc.iDocument_GetRoot,args)
    def Parse(*args): return apply(_blcelc.iDocument_Parse,args)
    def Write(*args): return apply(_blcelc.iDocument_Write,args)
    def Changeable(*args): return apply(_blcelc.iDocument_Changeable,args)
    def __del__(self, destroy= _blcelc.delete_iDocument):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDocument_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDocument_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocument instance at %s>" % (self.this,)

class iDocumentPtr(iDocument):
    def __init__(self,this):
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
    def CreateDocument(*args): return apply(_blcelc.iDocumentSystem_CreateDocument,args)
    def __del__(self, destroy= _blcelc.delete_iDocumentSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDocumentSystem_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDocumentSystem_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDocumentSystem instance at %s>" % (self.this,)

class iDocumentSystemPtr(iDocumentSystem):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csTinyDocumentSystem, 'this', apply(_blcelc.new_csTinyDocumentSystem,args))
        _swig_setattr(self, csTinyDocumentSystem, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csTinyDocumentSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_setmethods__["scfRefCount"] = _blcelc.csTinyDocumentSystem_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csTinyDocumentSystem_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csTinyDocumentSystem_scfRefCount_get,_blcelc.csTinyDocumentSystem_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csTinyDocumentSystem_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csTinyDocumentSystem_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csTinyDocumentSystem_scfWeakRefOwners_get,_blcelc.csTinyDocumentSystem_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return apply(_blcelc.csTinyDocumentSystem_scfRemoveRefOwners,args)
    __swig_setmethods__["scfParent"] = _blcelc.csTinyDocumentSystem_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csTinyDocumentSystem_scfParent_get
    if _newclass:scfParent = property(_blcelc.csTinyDocumentSystem_scfParent_get,_blcelc.csTinyDocumentSystem_scfParent_set)
    def IncRef(*args): return apply(_blcelc.csTinyDocumentSystem_IncRef,args)
    def DecRef(*args): return apply(_blcelc.csTinyDocumentSystem_DecRef,args)
    def GetRefCount(*args): return apply(_blcelc.csTinyDocumentSystem_GetRefCount,args)
    def AddRefOwner(*args): return apply(_blcelc.csTinyDocumentSystem_AddRefOwner,args)
    def RemoveRefOwner(*args): return apply(_blcelc.csTinyDocumentSystem_RemoveRefOwner,args)
    def QueryInterface(*args): return apply(_blcelc.csTinyDocumentSystem_QueryInterface,args)
    def CreateDocument(*args): return apply(_blcelc.csTinyDocumentSystem_CreateDocument,args)
    def __repr__(self):
        return "<C csTinyDocumentSystem instance at %s>" % (self.this,)

class csTinyDocumentSystemPtr(csTinyDocumentSystem):
    def __init__(self,this):
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
    def GetSize(*args): return apply(_blcelc.iDataBuffer_GetSize,args)
    def GetData(*args): return apply(_blcelc.iDataBuffer_GetData,args)
    def asString(*args): return apply(_blcelc.iDataBuffer_asString,args)
    def GetUint8(*args): return apply(_blcelc.iDataBuffer_GetUint8,args)
    def __del__(self, destroy= _blcelc.delete_iDataBuffer):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDataBuffer_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDataBuffer_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDataBuffer instance at %s>" % (self.this,)

class iDataBufferPtr(iDataBuffer):
    def __init__(self,this):
        _swig_setattr(self, iDataBuffer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iDataBuffer, 'thisown', 0)
        _swig_setattr(self, iDataBuffer,self.__class__,iDataBuffer)
_blcelc.iDataBuffer_swigregister(iDataBufferPtr)
iDataBuffer_scfGetVersion = _blcelc.iDataBuffer_scfGetVersion


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
CS_CLIP_TOPLEVEL = _blcelc.CS_CLIP_TOPLEVEL
class csFog(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csFog, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csFog, name)
    __swig_setmethods__["enabled"] = _blcelc.csFog_enabled_set
    __swig_getmethods__["enabled"] = _blcelc.csFog_enabled_get
    if _newclass:enabled = property(_blcelc.csFog_enabled_get,_blcelc.csFog_enabled_set)
    __swig_setmethods__["density"] = _blcelc.csFog_density_set
    __swig_getmethods__["density"] = _blcelc.csFog_density_get
    if _newclass:density = property(_blcelc.csFog_density_get,_blcelc.csFog_density_set)
    __swig_setmethods__["red"] = _blcelc.csFog_red_set
    __swig_getmethods__["red"] = _blcelc.csFog_red_get
    if _newclass:red = property(_blcelc.csFog_red_get,_blcelc.csFog_red_set)
    __swig_setmethods__["green"] = _blcelc.csFog_green_set
    __swig_getmethods__["green"] = _blcelc.csFog_green_get
    if _newclass:green = property(_blcelc.csFog_green_get,_blcelc.csFog_green_set)
    __swig_setmethods__["blue"] = _blcelc.csFog_blue_set
    __swig_getmethods__["blue"] = _blcelc.csFog_blue_get
    if _newclass:blue = property(_blcelc.csFog_blue_get,_blcelc.csFog_blue_set)
    def __init__(self,*args):
        _swig_setattr(self, csFog, 'this', apply(_blcelc.new_csFog,args))
        _swig_setattr(self, csFog, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csFog):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csFog instance at %s>" % (self.this,)

class csFogPtr(csFog):
    def __init__(self,this):
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
CS_VATTRIB_GENERIC_FIRST = _blcelc.CS_VATTRIB_GENERIC_FIRST
CS_VATTRIB_GENERIC_LAST = _blcelc.CS_VATTRIB_GENERIC_LAST
CS_VATTRIB_SPECIFIC_FIRST = _blcelc.CS_VATTRIB_SPECIFIC_FIRST
CS_VATTRIB_SPECIFIC_LAST = _blcelc.CS_VATTRIB_SPECIFIC_LAST
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
CS_FX_KEYCOLOR = _blcelc.CS_FX_KEYCOLOR
CS_FX_GOURAUD = _blcelc.CS_FX_GOURAUD
CS_FX_TILING = _blcelc.CS_FX_TILING
CS_FX_MASK_ALPHA = _blcelc.CS_FX_MASK_ALPHA
class csAlphaMode(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csAlphaMode, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csAlphaMode, name)
    alphaNone = _blcelc.csAlphaMode_alphaNone
    alphaBinary = _blcelc.csAlphaMode_alphaBinary
    alphaSmooth = _blcelc.csAlphaMode_alphaSmooth
    __swig_setmethods__["autoAlphaMode"] = _blcelc.csAlphaMode_autoAlphaMode_set
    __swig_getmethods__["autoAlphaMode"] = _blcelc.csAlphaMode_autoAlphaMode_get
    if _newclass:autoAlphaMode = property(_blcelc.csAlphaMode_autoAlphaMode_get,_blcelc.csAlphaMode_autoAlphaMode_set)
    def __init__(self,*args):
        _swig_setattr(self, csAlphaMode, 'this', apply(_blcelc.new_csAlphaMode,args))
        _swig_setattr(self, csAlphaMode, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csAlphaMode):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csAlphaMode instance at %s>" % (self.this,)

class csAlphaModePtr(csAlphaMode):
    def __init__(self,this):
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
    __swig_setmethods__["CanClip"] = _blcelc.csGraphics3DCaps_CanClip_set
    __swig_getmethods__["CanClip"] = _blcelc.csGraphics3DCaps_CanClip_get
    if _newclass:CanClip = property(_blcelc.csGraphics3DCaps_CanClip_get,_blcelc.csGraphics3DCaps_CanClip_set)
    __swig_setmethods__["minTexHeight"] = _blcelc.csGraphics3DCaps_minTexHeight_set
    __swig_getmethods__["minTexHeight"] = _blcelc.csGraphics3DCaps_minTexHeight_get
    if _newclass:minTexHeight = property(_blcelc.csGraphics3DCaps_minTexHeight_get,_blcelc.csGraphics3DCaps_minTexHeight_set)
    __swig_setmethods__["minTexWidth"] = _blcelc.csGraphics3DCaps_minTexWidth_set
    __swig_getmethods__["minTexWidth"] = _blcelc.csGraphics3DCaps_minTexWidth_get
    if _newclass:minTexWidth = property(_blcelc.csGraphics3DCaps_minTexWidth_get,_blcelc.csGraphics3DCaps_minTexWidth_set)
    __swig_setmethods__["maxTexHeight"] = _blcelc.csGraphics3DCaps_maxTexHeight_set
    __swig_getmethods__["maxTexHeight"] = _blcelc.csGraphics3DCaps_maxTexHeight_get
    if _newclass:maxTexHeight = property(_blcelc.csGraphics3DCaps_maxTexHeight_get,_blcelc.csGraphics3DCaps_maxTexHeight_set)
    __swig_setmethods__["maxTexWidth"] = _blcelc.csGraphics3DCaps_maxTexWidth_set
    __swig_getmethods__["maxTexWidth"] = _blcelc.csGraphics3DCaps_maxTexWidth_get
    if _newclass:maxTexWidth = property(_blcelc.csGraphics3DCaps_maxTexWidth_get,_blcelc.csGraphics3DCaps_maxTexWidth_set)
    __swig_setmethods__["fog"] = _blcelc.csGraphics3DCaps_fog_set
    __swig_getmethods__["fog"] = _blcelc.csGraphics3DCaps_fog_get
    if _newclass:fog = property(_blcelc.csGraphics3DCaps_fog_get,_blcelc.csGraphics3DCaps_fog_set)
    __swig_setmethods__["NeedsPO2Maps"] = _blcelc.csGraphics3DCaps_NeedsPO2Maps_set
    __swig_getmethods__["NeedsPO2Maps"] = _blcelc.csGraphics3DCaps_NeedsPO2Maps_get
    if _newclass:NeedsPO2Maps = property(_blcelc.csGraphics3DCaps_NeedsPO2Maps_get,_blcelc.csGraphics3DCaps_NeedsPO2Maps_set)
    __swig_setmethods__["MaxAspectRatio"] = _blcelc.csGraphics3DCaps_MaxAspectRatio_set
    __swig_getmethods__["MaxAspectRatio"] = _blcelc.csGraphics3DCaps_MaxAspectRatio_get
    if _newclass:MaxAspectRatio = property(_blcelc.csGraphics3DCaps_MaxAspectRatio_get,_blcelc.csGraphics3DCaps_MaxAspectRatio_set)
    def __init__(self,*args):
        _swig_setattr(self, csGraphics3DCaps, 'this', apply(_blcelc.new_csGraphics3DCaps,args))
        _swig_setattr(self, csGraphics3DCaps, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csGraphics3DCaps):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csGraphics3DCaps instance at %s>" % (self.this,)

class csGraphics3DCapsPtr(csGraphics3DCaps):
    def __init__(self,this):
        _swig_setattr(self, csGraphics3DCaps, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csGraphics3DCaps, 'thisown', 0)
        _swig_setattr(self, csGraphics3DCaps,self.__class__,csGraphics3DCaps)
_blcelc.csGraphics3DCaps_swigregister(csGraphics3DCapsPtr)

CS_FOG_FRONT = _blcelc.CS_FOG_FRONT
CS_FOG_BACK = _blcelc.CS_FOG_BACK
CS_FOG_VIEW = _blcelc.CS_FOG_VIEW
CS_FOGTABLE_SIZE = _blcelc.CS_FOGTABLE_SIZE
CS_FOGTABLE_MEDIANDISTANCE = _blcelc.CS_FOGTABLE_MEDIANDISTANCE
CS_FOGTABLE_MAXDISTANCE = _blcelc.CS_FOGTABLE_MAXDISTANCE
CS_FOGTABLE_DISTANCESCALE = _blcelc.CS_FOGTABLE_DISTANCESCALE
CS_FOGTABLE_CLAMPVALUE = _blcelc.CS_FOGTABLE_CLAMPVALUE
CS_FOG_MAXVALUE = _blcelc.CS_FOG_MAXVALUE
class G3DFogInfo(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, G3DFogInfo, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, G3DFogInfo, name)
    __swig_setmethods__["r"] = _blcelc.G3DFogInfo_r_set
    __swig_getmethods__["r"] = _blcelc.G3DFogInfo_r_get
    if _newclass:r = property(_blcelc.G3DFogInfo_r_get,_blcelc.G3DFogInfo_r_set)
    __swig_setmethods__["g"] = _blcelc.G3DFogInfo_g_set
    __swig_getmethods__["g"] = _blcelc.G3DFogInfo_g_get
    if _newclass:g = property(_blcelc.G3DFogInfo_g_get,_blcelc.G3DFogInfo_g_set)
    __swig_setmethods__["b"] = _blcelc.G3DFogInfo_b_set
    __swig_getmethods__["b"] = _blcelc.G3DFogInfo_b_get
    if _newclass:b = property(_blcelc.G3DFogInfo_b_get,_blcelc.G3DFogInfo_b_set)
    __swig_setmethods__["intensity"] = _blcelc.G3DFogInfo_intensity_set
    __swig_getmethods__["intensity"] = _blcelc.G3DFogInfo_intensity_get
    if _newclass:intensity = property(_blcelc.G3DFogInfo_intensity_get,_blcelc.G3DFogInfo_intensity_set)
    __swig_setmethods__["intensity2"] = _blcelc.G3DFogInfo_intensity2_set
    __swig_getmethods__["intensity2"] = _blcelc.G3DFogInfo_intensity2_get
    if _newclass:intensity2 = property(_blcelc.G3DFogInfo_intensity2_get,_blcelc.G3DFogInfo_intensity2_set)
    def __init__(self,*args):
        _swig_setattr(self, G3DFogInfo, 'this', apply(_blcelc.new_G3DFogInfo,args))
        _swig_setattr(self, G3DFogInfo, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_G3DFogInfo):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C G3DFogInfo instance at %s>" % (self.this,)

class G3DFogInfoPtr(G3DFogInfo):
    def __init__(self,this):
        _swig_setattr(self, G3DFogInfo, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, G3DFogInfo, 'thisown', 0)
        _swig_setattr(self, G3DFogInfo,self.__class__,G3DFogInfo)
_blcelc.G3DFogInfo_swigregister(G3DFogInfoPtr)

class G3DCam2TextureTransform(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, G3DCam2TextureTransform, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, G3DCam2TextureTransform, name)
    __swig_setmethods__["m_cam2tex"] = _blcelc.G3DCam2TextureTransform_m_cam2tex_set
    __swig_getmethods__["m_cam2tex"] = _blcelc.G3DCam2TextureTransform_m_cam2tex_get
    if _newclass:m_cam2tex = property(_blcelc.G3DCam2TextureTransform_m_cam2tex_get,_blcelc.G3DCam2TextureTransform_m_cam2tex_set)
    __swig_setmethods__["v_cam2tex"] = _blcelc.G3DCam2TextureTransform_v_cam2tex_set
    __swig_getmethods__["v_cam2tex"] = _blcelc.G3DCam2TextureTransform_v_cam2tex_get
    if _newclass:v_cam2tex = property(_blcelc.G3DCam2TextureTransform_v_cam2tex_get,_blcelc.G3DCam2TextureTransform_v_cam2tex_set)
    def __init__(self,*args):
        _swig_setattr(self, G3DCam2TextureTransform, 'this', apply(_blcelc.new_G3DCam2TextureTransform,args))
        _swig_setattr(self, G3DCam2TextureTransform, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_G3DCam2TextureTransform):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C G3DCam2TextureTransform instance at %s>" % (self.this,)

class G3DCam2TextureTransformPtr(G3DCam2TextureTransform):
    def __init__(self,this):
        _swig_setattr(self, G3DCam2TextureTransform, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, G3DCam2TextureTransform, 'thisown', 0)
        _swig_setattr(self, G3DCam2TextureTransform,self.__class__,G3DCam2TextureTransform)
_blcelc.G3DCam2TextureTransform_swigregister(G3DCam2TextureTransformPtr)

class G3DPolygonDPFX(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, G3DPolygonDPFX, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, G3DPolygonDPFX, name)
    __swig_setmethods__["num"] = _blcelc.G3DPolygonDPFX_num_set
    __swig_getmethods__["num"] = _blcelc.G3DPolygonDPFX_num_get
    if _newclass:num = property(_blcelc.G3DPolygonDPFX_num_get,_blcelc.G3DPolygonDPFX_num_set)
    __swig_setmethods__["vertices"] = _blcelc.G3DPolygonDPFX_vertices_set
    __swig_getmethods__["vertices"] = _blcelc.G3DPolygonDPFX_vertices_get
    if _newclass:vertices = property(_blcelc.G3DPolygonDPFX_vertices_get,_blcelc.G3DPolygonDPFX_vertices_set)
    __swig_setmethods__["z"] = _blcelc.G3DPolygonDPFX_z_set
    __swig_getmethods__["z"] = _blcelc.G3DPolygonDPFX_z_get
    if _newclass:z = property(_blcelc.G3DPolygonDPFX_z_get,_blcelc.G3DPolygonDPFX_z_set)
    __swig_setmethods__["texels"] = _blcelc.G3DPolygonDPFX_texels_set
    __swig_getmethods__["texels"] = _blcelc.G3DPolygonDPFX_texels_get
    if _newclass:texels = property(_blcelc.G3DPolygonDPFX_texels_get,_blcelc.G3DPolygonDPFX_texels_set)
    __swig_setmethods__["colors"] = _blcelc.G3DPolygonDPFX_colors_set
    __swig_getmethods__["colors"] = _blcelc.G3DPolygonDPFX_colors_get
    if _newclass:colors = property(_blcelc.G3DPolygonDPFX_colors_get,_blcelc.G3DPolygonDPFX_colors_set)
    __swig_setmethods__["fog_info"] = _blcelc.G3DPolygonDPFX_fog_info_set
    __swig_getmethods__["fog_info"] = _blcelc.G3DPolygonDPFX_fog_info_get
    if _newclass:fog_info = property(_blcelc.G3DPolygonDPFX_fog_info_get,_blcelc.G3DPolygonDPFX_fog_info_set)
    __swig_setmethods__["use_fog"] = _blcelc.G3DPolygonDPFX_use_fog_set
    __swig_getmethods__["use_fog"] = _blcelc.G3DPolygonDPFX_use_fog_get
    if _newclass:use_fog = property(_blcelc.G3DPolygonDPFX_use_fog_get,_blcelc.G3DPolygonDPFX_use_fog_set)
    __swig_setmethods__["mat_handle"] = _blcelc.G3DPolygonDPFX_mat_handle_set
    __swig_getmethods__["mat_handle"] = _blcelc.G3DPolygonDPFX_mat_handle_get
    if _newclass:mat_handle = property(_blcelc.G3DPolygonDPFX_mat_handle_get,_blcelc.G3DPolygonDPFX_mat_handle_set)
    __swig_setmethods__["mixmode"] = _blcelc.G3DPolygonDPFX_mixmode_set
    __swig_getmethods__["mixmode"] = _blcelc.G3DPolygonDPFX_mixmode_get
    if _newclass:mixmode = property(_blcelc.G3DPolygonDPFX_mixmode_get,_blcelc.G3DPolygonDPFX_mixmode_set)
    __swig_setmethods__["flat_color_r"] = _blcelc.G3DPolygonDPFX_flat_color_r_set
    __swig_getmethods__["flat_color_r"] = _blcelc.G3DPolygonDPFX_flat_color_r_get
    if _newclass:flat_color_r = property(_blcelc.G3DPolygonDPFX_flat_color_r_get,_blcelc.G3DPolygonDPFX_flat_color_r_set)
    __swig_setmethods__["flat_color_g"] = _blcelc.G3DPolygonDPFX_flat_color_g_set
    __swig_getmethods__["flat_color_g"] = _blcelc.G3DPolygonDPFX_flat_color_g_get
    if _newclass:flat_color_g = property(_blcelc.G3DPolygonDPFX_flat_color_g_get,_blcelc.G3DPolygonDPFX_flat_color_g_set)
    __swig_setmethods__["flat_color_b"] = _blcelc.G3DPolygonDPFX_flat_color_b_set
    __swig_getmethods__["flat_color_b"] = _blcelc.G3DPolygonDPFX_flat_color_b_get
    if _newclass:flat_color_b = property(_blcelc.G3DPolygonDPFX_flat_color_b_get,_blcelc.G3DPolygonDPFX_flat_color_b_set)
    def __init__(self,*args):
        _swig_setattr(self, G3DPolygonDPFX, 'this', apply(_blcelc.new_G3DPolygonDPFX,args))
        _swig_setattr(self, G3DPolygonDPFX, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_G3DPolygonDPFX):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C G3DPolygonDPFX instance at %s>" % (self.this,)

class G3DPolygonDPFXPtr(G3DPolygonDPFX):
    def __init__(self,this):
        _swig_setattr(self, G3DPolygonDPFX, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, G3DPolygonDPFX, 'thisown', 0)
        _swig_setattr(self, G3DPolygonDPFX,self.__class__,G3DPolygonDPFX)
_blcelc.G3DPolygonDPFX_swigregister(G3DPolygonDPFXPtr)

class G3DPolygonDFP(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, G3DPolygonDFP, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, G3DPolygonDFP, name)
    __swig_setmethods__["num"] = _blcelc.G3DPolygonDFP_num_set
    __swig_getmethods__["num"] = _blcelc.G3DPolygonDFP_num_get
    if _newclass:num = property(_blcelc.G3DPolygonDFP_num_get,_blcelc.G3DPolygonDFP_num_set)
    __swig_setmethods__["vertices"] = _blcelc.G3DPolygonDFP_vertices_set
    __swig_getmethods__["vertices"] = _blcelc.G3DPolygonDFP_vertices_get
    if _newclass:vertices = property(_blcelc.G3DPolygonDFP_vertices_get,_blcelc.G3DPolygonDFP_vertices_set)
    __swig_setmethods__["normal"] = _blcelc.G3DPolygonDFP_normal_set
    __swig_getmethods__["normal"] = _blcelc.G3DPolygonDFP_normal_get
    if _newclass:normal = property(_blcelc.G3DPolygonDFP_normal_get,_blcelc.G3DPolygonDFP_normal_set)
    def __init__(self,*args):
        _swig_setattr(self, G3DPolygonDFP, 'this', apply(_blcelc.new_G3DPolygonDFP,args))
        _swig_setattr(self, G3DPolygonDFP, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_G3DPolygonDFP):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C G3DPolygonDFP instance at %s>" % (self.this,)

class G3DPolygonDFPPtr(G3DPolygonDFP):
    def __init__(self,this):
        _swig_setattr(self, G3DPolygonDFP, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, G3DPolygonDFP, 'thisown', 0)
        _swig_setattr(self, G3DPolygonDFP,self.__class__,G3DPolygonDFP)
_blcelc.G3DPolygonDFP_swigregister(G3DPolygonDFPPtr)

class G3DPolygonDP(G3DPolygonDFP):
    __swig_setmethods__ = {}
    for _s in [G3DPolygonDFP]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, G3DPolygonDP, name, value)
    __swig_getmethods__ = {}
    for _s in [G3DPolygonDFP]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, G3DPolygonDP, name)
    __swig_setmethods__["fog_info"] = _blcelc.G3DPolygonDP_fog_info_set
    __swig_getmethods__["fog_info"] = _blcelc.G3DPolygonDP_fog_info_get
    if _newclass:fog_info = property(_blcelc.G3DPolygonDP_fog_info_get,_blcelc.G3DPolygonDP_fog_info_set)
    __swig_setmethods__["use_fog"] = _blcelc.G3DPolygonDP_use_fog_set
    __swig_getmethods__["use_fog"] = _blcelc.G3DPolygonDP_use_fog_get
    if _newclass:use_fog = property(_blcelc.G3DPolygonDP_use_fog_get,_blcelc.G3DPolygonDP_use_fog_set)
    __swig_setmethods__["mat_handle"] = _blcelc.G3DPolygonDP_mat_handle_set
    __swig_getmethods__["mat_handle"] = _blcelc.G3DPolygonDP_mat_handle_get
    if _newclass:mat_handle = property(_blcelc.G3DPolygonDP_mat_handle_get,_blcelc.G3DPolygonDP_mat_handle_set)
    __swig_setmethods__["cam2tex"] = _blcelc.G3DPolygonDP_cam2tex_set
    __swig_getmethods__["cam2tex"] = _blcelc.G3DPolygonDP_cam2tex_get
    if _newclass:cam2tex = property(_blcelc.G3DPolygonDP_cam2tex_get,_blcelc.G3DPolygonDP_cam2tex_set)
    __swig_setmethods__["texmap"] = _blcelc.G3DPolygonDP_texmap_set
    __swig_getmethods__["texmap"] = _blcelc.G3DPolygonDP_texmap_get
    if _newclass:texmap = property(_blcelc.G3DPolygonDP_texmap_get,_blcelc.G3DPolygonDP_texmap_set)
    __swig_setmethods__["rlm"] = _blcelc.G3DPolygonDP_rlm_set
    __swig_getmethods__["rlm"] = _blcelc.G3DPolygonDP_rlm_get
    if _newclass:rlm = property(_blcelc.G3DPolygonDP_rlm_get,_blcelc.G3DPolygonDP_rlm_set)
    __swig_setmethods__["do_fullbright"] = _blcelc.G3DPolygonDP_do_fullbright_set
    __swig_getmethods__["do_fullbright"] = _blcelc.G3DPolygonDP_do_fullbright_get
    if _newclass:do_fullbright = property(_blcelc.G3DPolygonDP_do_fullbright_get,_blcelc.G3DPolygonDP_do_fullbright_set)
    __swig_setmethods__["mixmode"] = _blcelc.G3DPolygonDP_mixmode_set
    __swig_getmethods__["mixmode"] = _blcelc.G3DPolygonDP_mixmode_get
    if _newclass:mixmode = property(_blcelc.G3DPolygonDP_mixmode_get,_blcelc.G3DPolygonDP_mixmode_set)
    __swig_setmethods__["z_value"] = _blcelc.G3DPolygonDP_z_value_set
    __swig_getmethods__["z_value"] = _blcelc.G3DPolygonDP_z_value_get
    if _newclass:z_value = property(_blcelc.G3DPolygonDP_z_value_get,_blcelc.G3DPolygonDP_z_value_set)
    def __init__(self,*args):
        _swig_setattr(self, G3DPolygonDP, 'this', apply(_blcelc.new_G3DPolygonDP,args))
        _swig_setattr(self, G3DPolygonDP, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_G3DPolygonDP):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C G3DPolygonDP instance at %s>" % (self.this,)

class G3DPolygonDPPtr(G3DPolygonDP):
    def __init__(self,this):
        _swig_setattr(self, G3DPolygonDP, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, G3DPolygonDP, 'thisown', 0)
        _swig_setattr(self, G3DPolygonDP,self.__class__,G3DPolygonDP)
_blcelc.G3DPolygonDP_swigregister(G3DPolygonDPPtr)

class G3DTriangleMesh(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, G3DTriangleMesh, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, G3DTriangleMesh, name)
    MAX_VERTEXPOOL = _blcelc.G3DTriangleMesh_MAX_VERTEXPOOL
    __swig_setmethods__["num_vertices_pool"] = _blcelc.G3DTriangleMesh_num_vertices_pool_set
    __swig_getmethods__["num_vertices_pool"] = _blcelc.G3DTriangleMesh_num_vertices_pool_get
    if _newclass:num_vertices_pool = property(_blcelc.G3DTriangleMesh_num_vertices_pool_get,_blcelc.G3DTriangleMesh_num_vertices_pool_set)
    __swig_setmethods__["num_triangles"] = _blcelc.G3DTriangleMesh_num_triangles_set
    __swig_getmethods__["num_triangles"] = _blcelc.G3DTriangleMesh_num_triangles_get
    if _newclass:num_triangles = property(_blcelc.G3DTriangleMesh_num_triangles_get,_blcelc.G3DTriangleMesh_num_triangles_set)
    __swig_setmethods__["triangles"] = _blcelc.G3DTriangleMesh_triangles_set
    __swig_getmethods__["triangles"] = _blcelc.G3DTriangleMesh_triangles_get
    if _newclass:triangles = property(_blcelc.G3DTriangleMesh_triangles_get,_blcelc.G3DTriangleMesh_triangles_set)
    __swig_setmethods__["clip_portal"] = _blcelc.G3DTriangleMesh_clip_portal_set
    __swig_getmethods__["clip_portal"] = _blcelc.G3DTriangleMesh_clip_portal_get
    if _newclass:clip_portal = property(_blcelc.G3DTriangleMesh_clip_portal_get,_blcelc.G3DTriangleMesh_clip_portal_set)
    __swig_setmethods__["clip_plane"] = _blcelc.G3DTriangleMesh_clip_plane_set
    __swig_getmethods__["clip_plane"] = _blcelc.G3DTriangleMesh_clip_plane_get
    if _newclass:clip_plane = property(_blcelc.G3DTriangleMesh_clip_plane_get,_blcelc.G3DTriangleMesh_clip_plane_set)
    __swig_setmethods__["clip_z_plane"] = _blcelc.G3DTriangleMesh_clip_z_plane_set
    __swig_getmethods__["clip_z_plane"] = _blcelc.G3DTriangleMesh_clip_z_plane_get
    if _newclass:clip_z_plane = property(_blcelc.G3DTriangleMesh_clip_z_plane_get,_blcelc.G3DTriangleMesh_clip_z_plane_set)
    __swig_setmethods__["use_vertex_color"] = _blcelc.G3DTriangleMesh_use_vertex_color_set
    __swig_getmethods__["use_vertex_color"] = _blcelc.G3DTriangleMesh_use_vertex_color_get
    if _newclass:use_vertex_color = property(_blcelc.G3DTriangleMesh_use_vertex_color_get,_blcelc.G3DTriangleMesh_use_vertex_color_set)
    __swig_setmethods__["do_fog"] = _blcelc.G3DTriangleMesh_do_fog_set
    __swig_getmethods__["do_fog"] = _blcelc.G3DTriangleMesh_do_fog_get
    if _newclass:do_fog = property(_blcelc.G3DTriangleMesh_do_fog_get,_blcelc.G3DTriangleMesh_do_fog_set)
    __swig_setmethods__["do_mirror"] = _blcelc.G3DTriangleMesh_do_mirror_set
    __swig_getmethods__["do_mirror"] = _blcelc.G3DTriangleMesh_do_mirror_get
    if _newclass:do_mirror = property(_blcelc.G3DTriangleMesh_do_mirror_get,_blcelc.G3DTriangleMesh_do_mirror_set)
    __swig_setmethods__["do_morph_texels"] = _blcelc.G3DTriangleMesh_do_morph_texels_set
    __swig_getmethods__["do_morph_texels"] = _blcelc.G3DTriangleMesh_do_morph_texels_get
    if _newclass:do_morph_texels = property(_blcelc.G3DTriangleMesh_do_morph_texels_get,_blcelc.G3DTriangleMesh_do_morph_texels_set)
    __swig_setmethods__["do_morph_colors"] = _blcelc.G3DTriangleMesh_do_morph_colors_set
    __swig_getmethods__["do_morph_colors"] = _blcelc.G3DTriangleMesh_do_morph_colors_get
    if _newclass:do_morph_colors = property(_blcelc.G3DTriangleMesh_do_morph_colors_get,_blcelc.G3DTriangleMesh_do_morph_colors_set)
    VM_WORLDSPACE = _blcelc.G3DTriangleMesh_VM_WORLDSPACE
    VM_VIEWSPACE = _blcelc.G3DTriangleMesh_VM_VIEWSPACE
    __swig_setmethods__["vertex_mode"] = _blcelc.G3DTriangleMesh_vertex_mode_set
    __swig_getmethods__["vertex_mode"] = _blcelc.G3DTriangleMesh_vertex_mode_get
    if _newclass:vertex_mode = property(_blcelc.G3DTriangleMesh_vertex_mode_get,_blcelc.G3DTriangleMesh_vertex_mode_set)
    __swig_setmethods__["mixmode"] = _blcelc.G3DTriangleMesh_mixmode_set
    __swig_getmethods__["mixmode"] = _blcelc.G3DTriangleMesh_mixmode_get
    if _newclass:mixmode = property(_blcelc.G3DTriangleMesh_mixmode_get,_blcelc.G3DTriangleMesh_mixmode_set)
    __swig_setmethods__["morph_factor"] = _blcelc.G3DTriangleMesh_morph_factor_set
    __swig_getmethods__["morph_factor"] = _blcelc.G3DTriangleMesh_morph_factor_get
    if _newclass:morph_factor = property(_blcelc.G3DTriangleMesh_morph_factor_get,_blcelc.G3DTriangleMesh_morph_factor_set)
    __swig_setmethods__["buffers"] = _blcelc.G3DTriangleMesh_buffers_set
    __swig_getmethods__["buffers"] = _blcelc.G3DTriangleMesh_buffers_get
    if _newclass:buffers = property(_blcelc.G3DTriangleMesh_buffers_get,_blcelc.G3DTriangleMesh_buffers_set)
    __swig_setmethods__["mat_handle"] = _blcelc.G3DTriangleMesh_mat_handle_set
    __swig_getmethods__["mat_handle"] = _blcelc.G3DTriangleMesh_mat_handle_get
    if _newclass:mat_handle = property(_blcelc.G3DTriangleMesh_mat_handle_get,_blcelc.G3DTriangleMesh_mat_handle_set)
    __swig_setmethods__["vertex_fog"] = _blcelc.G3DTriangleMesh_vertex_fog_set
    __swig_getmethods__["vertex_fog"] = _blcelc.G3DTriangleMesh_vertex_fog_get
    if _newclass:vertex_fog = property(_blcelc.G3DTriangleMesh_vertex_fog_get,_blcelc.G3DTriangleMesh_vertex_fog_set)
    def __init__(self,*args):
        _swig_setattr(self, G3DTriangleMesh, 'this', apply(_blcelc.new_G3DTriangleMesh,args))
        _swig_setattr(self, G3DTriangleMesh, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_G3DTriangleMesh):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C G3DTriangleMesh instance at %s>" % (self.this,)

class G3DTriangleMeshPtr(G3DTriangleMesh):
    def __init__(self,this):
        _swig_setattr(self, G3DTriangleMesh, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, G3DTriangleMesh, 'thisown', 0)
        _swig_setattr(self, G3DTriangleMesh,self.__class__,G3DTriangleMesh)
_blcelc.G3DTriangleMesh_swigregister(G3DTriangleMeshPtr)

class G3DPolygonMesh(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, G3DPolygonMesh, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, G3DPolygonMesh, name)
    __swig_setmethods__["polybuf"] = _blcelc.G3DPolygonMesh_polybuf_set
    __swig_getmethods__["polybuf"] = _blcelc.G3DPolygonMesh_polybuf_get
    if _newclass:polybuf = property(_blcelc.G3DPolygonMesh_polybuf_get,_blcelc.G3DPolygonMesh_polybuf_set)
    __swig_setmethods__["do_fog"] = _blcelc.G3DPolygonMesh_do_fog_set
    __swig_getmethods__["do_fog"] = _blcelc.G3DPolygonMesh_do_fog_get
    if _newclass:do_fog = property(_blcelc.G3DPolygonMesh_do_fog_get,_blcelc.G3DPolygonMesh_do_fog_set)
    __swig_setmethods__["mixmode"] = _blcelc.G3DPolygonMesh_mixmode_set
    __swig_getmethods__["mixmode"] = _blcelc.G3DPolygonMesh_mixmode_get
    if _newclass:mixmode = property(_blcelc.G3DPolygonMesh_mixmode_get,_blcelc.G3DPolygonMesh_mixmode_set)
    __swig_setmethods__["clip_portal"] = _blcelc.G3DPolygonMesh_clip_portal_set
    __swig_getmethods__["clip_portal"] = _blcelc.G3DPolygonMesh_clip_portal_get
    if _newclass:clip_portal = property(_blcelc.G3DPolygonMesh_clip_portal_get,_blcelc.G3DPolygonMesh_clip_portal_set)
    __swig_setmethods__["clip_plane"] = _blcelc.G3DPolygonMesh_clip_plane_set
    __swig_getmethods__["clip_plane"] = _blcelc.G3DPolygonMesh_clip_plane_get
    if _newclass:clip_plane = property(_blcelc.G3DPolygonMesh_clip_plane_get,_blcelc.G3DPolygonMesh_clip_plane_set)
    __swig_setmethods__["clip_z_plane"] = _blcelc.G3DPolygonMesh_clip_z_plane_set
    __swig_getmethods__["clip_z_plane"] = _blcelc.G3DPolygonMesh_clip_z_plane_get
    if _newclass:clip_z_plane = property(_blcelc.G3DPolygonMesh_clip_z_plane_get,_blcelc.G3DPolygonMesh_clip_z_plane_set)
    __swig_setmethods__["do_mirror"] = _blcelc.G3DPolygonMesh_do_mirror_set
    __swig_getmethods__["do_mirror"] = _blcelc.G3DPolygonMesh_do_mirror_get
    if _newclass:do_mirror = property(_blcelc.G3DPolygonMesh_do_mirror_get,_blcelc.G3DPolygonMesh_do_mirror_set)
    VM_WORLDSPACE = _blcelc.G3DPolygonMesh_VM_WORLDSPACE
    VM_VIEWSPACE = _blcelc.G3DPolygonMesh_VM_VIEWSPACE
    __swig_setmethods__["vertex_mode"] = _blcelc.G3DPolygonMesh_vertex_mode_set
    __swig_getmethods__["vertex_mode"] = _blcelc.G3DPolygonMesh_vertex_mode_get
    if _newclass:vertex_mode = property(_blcelc.G3DPolygonMesh_vertex_mode_get,_blcelc.G3DPolygonMesh_vertex_mode_set)
    __swig_setmethods__["vertex_fog"] = _blcelc.G3DPolygonMesh_vertex_fog_set
    __swig_getmethods__["vertex_fog"] = _blcelc.G3DPolygonMesh_vertex_fog_get
    if _newclass:vertex_fog = property(_blcelc.G3DPolygonMesh_vertex_fog_get,_blcelc.G3DPolygonMesh_vertex_fog_set)
    def __init__(self,*args):
        _swig_setattr(self, G3DPolygonMesh, 'this', apply(_blcelc.new_G3DPolygonMesh,args))
        _swig_setattr(self, G3DPolygonMesh, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_G3DPolygonMesh):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C G3DPolygonMesh instance at %s>" % (self.this,)

class G3DPolygonMeshPtr(G3DPolygonMesh):
    def __init__(self,this):
        _swig_setattr(self, G3DPolygonMesh, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, G3DPolygonMesh, 'thisown', 0)
        _swig_setattr(self, G3DPolygonMesh,self.__class__,G3DPolygonMesh)
_blcelc.G3DPolygonMesh_swigregister(G3DPolygonMeshPtr)

class iGraphics3D(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iGraphics3D, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iGraphics3D, name)
    def Open(*args): return apply(_blcelc.iGraphics3D_Open,args)
    def Close(*args): return apply(_blcelc.iGraphics3D_Close,args)
    def GetDriver2D(*args): return apply(_blcelc.iGraphics3D_GetDriver2D,args)
    def GetTextureManager(*args): return apply(_blcelc.iGraphics3D_GetTextureManager,args)
    def SetDimensions(*args): return apply(_blcelc.iGraphics3D_SetDimensions,args)
    def GetWidth(*args): return apply(_blcelc.iGraphics3D_GetWidth,args)
    def GetHeight(*args): return apply(_blcelc.iGraphics3D_GetHeight,args)
    def GetCaps(*args): return apply(_blcelc.iGraphics3D_GetCaps,args)
    def SetPerspectiveCenter(*args): return apply(_blcelc.iGraphics3D_SetPerspectiveCenter,args)
    def GetPerspectiveCenter(*args): return apply(_blcelc.iGraphics3D_GetPerspectiveCenter,args)
    def SetPerspectiveAspect(*args): return apply(_blcelc.iGraphics3D_SetPerspectiveAspect,args)
    def GetPerspectiveAspect(*args): return apply(_blcelc.iGraphics3D_GetPerspectiveAspect,args)
    def SetObjectToCamera(*args): return apply(_blcelc.iGraphics3D_SetObjectToCamera,args)
    def GetObjectToCamera(*args): return apply(_blcelc.iGraphics3D_GetObjectToCamera,args)
    def SetRenderTarget(*args): return apply(_blcelc.iGraphics3D_SetRenderTarget,args)
    def GetRenderTarget(*args): return apply(_blcelc.iGraphics3D_GetRenderTarget,args)
    def BeginDraw(*args): return apply(_blcelc.iGraphics3D_BeginDraw,args)
    def FinishDraw(*args): return apply(_blcelc.iGraphics3D_FinishDraw,args)
    def Print(*args): return apply(_blcelc.iGraphics3D_Print,args)
    def DrawPixmap(*args): return apply(_blcelc.iGraphics3D_DrawPixmap,args)
    def DrawLine(*args): return apply(_blcelc.iGraphics3D_DrawLine,args)
    def SetClipper(*args): return apply(_blcelc.iGraphics3D_SetClipper,args)
    def GetClipper(*args): return apply(_blcelc.iGraphics3D_GetClipper,args)
    def GetClipType(*args): return apply(_blcelc.iGraphics3D_GetClipType,args)
    def SetNearPlane(*args): return apply(_blcelc.iGraphics3D_SetNearPlane,args)
    def ResetNearPlane(*args): return apply(_blcelc.iGraphics3D_ResetNearPlane,args)
    def GetNearPlane(*args): return apply(_blcelc.iGraphics3D_GetNearPlane,args)
    def HasNearPlane(*args): return apply(_blcelc.iGraphics3D_HasNearPlane,args)
    def SetRenderState(*args): return apply(_blcelc.iGraphics3D_SetRenderState,args)
    def GetRenderState(*args): return apply(_blcelc.iGraphics3D_GetRenderState,args)
    def CreateRenderBuffer(*args): return apply(_blcelc.iGraphics3D_CreateRenderBuffer,args)
    def SetBufferState(*args): return apply(_blcelc.iGraphics3D_SetBufferState,args)
    def SetTextureState(*args): return apply(_blcelc.iGraphics3D_SetTextureState,args)
    def DrawMesh(*args): return apply(_blcelc.iGraphics3D_DrawMesh,args)
    def SetWriteMask(*args): return apply(_blcelc.iGraphics3D_SetWriteMask,args)
    def GetWriteMask(*args): return apply(_blcelc.iGraphics3D_GetWriteMask,args)
    def SetZMode(*args): return apply(_blcelc.iGraphics3D_SetZMode,args)
    def EnableZOffset(*args): return apply(_blcelc.iGraphics3D_EnableZOffset,args)
    def DisableZOffset(*args): return apply(_blcelc.iGraphics3D_DisableZOffset,args)
    def SetShadowState(*args): return apply(_blcelc.iGraphics3D_SetShadowState,args)
    def GetZBuffAt(*args): return apply(_blcelc.iGraphics3D_GetZBuffAt,args)
    def GetZBuffValue(*args): return apply(_blcelc.iGraphics3D_GetZBuffValue,args)
    def DrawPolygon(*args): return apply(_blcelc.iGraphics3D_DrawPolygon,args)
    def DrawPolygonDebug(*args): return apply(_blcelc.iGraphics3D_DrawPolygonDebug,args)
    def DrawPolygonFX(*args): return apply(_blcelc.iGraphics3D_DrawPolygonFX,args)
    def DrawTriangleMesh(*args): return apply(_blcelc.iGraphics3D_DrawTriangleMesh,args)
    def DrawPolygonMesh(*args): return apply(_blcelc.iGraphics3D_DrawPolygonMesh,args)
    def OpenFogObject(*args): return apply(_blcelc.iGraphics3D_OpenFogObject,args)
    def DrawFogPolygon(*args): return apply(_blcelc.iGraphics3D_DrawFogPolygon,args)
    def CloseFogObject(*args): return apply(_blcelc.iGraphics3D_CloseFogObject,args)
    def OpenPortal(*args): return apply(_blcelc.iGraphics3D_OpenPortal,args)
    def ClosePortal(*args): return apply(_blcelc.iGraphics3D_ClosePortal,args)
    def CreateHalo(*args): return apply(_blcelc.iGraphics3D_CreateHalo,args)
    def DumpCache(*args): return apply(_blcelc.iGraphics3D_DumpCache,args)
    def ClearCache(*args): return apply(_blcelc.iGraphics3D_ClearCache,args)
    def RemoveFromCache(*args): return apply(_blcelc.iGraphics3D_RemoveFromCache,args)
    def GetVertexBufferManager(*args): return apply(_blcelc.iGraphics3D_GetVertexBufferManager,args)
    def IsLightmapOK(*args): return apply(_blcelc.iGraphics3D_IsLightmapOK,args)
    def CreatePolygonRenderer(*args): return apply(_blcelc.iGraphics3D_CreatePolygonRenderer,args)
    def SetWorldToCamera(*args): return apply(_blcelc.iGraphics3D_SetWorldToCamera,args)
    def __del__(self, destroy= _blcelc.delete_iGraphics3D):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iGraphics3D_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iGraphics3D_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGraphics3D instance at %s>" % (self.this,)

class iGraphics3DPtr(iGraphics3D):
    def __init__(self,this):
        _swig_setattr(self, iGraphics3D, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGraphics3D, 'thisown', 0)
        _swig_setattr(self, iGraphics3D,self.__class__,iGraphics3D)
_blcelc.iGraphics3D_swigregister(iGraphics3DPtr)
iGraphics3D_scfGetVersion = _blcelc.iGraphics3D_scfGetVersion


CS_WRITE_BASELINE = _blcelc.CS_WRITE_BASELINE
CS_WRITE_NOANTIALIAS = _blcelc.CS_WRITE_NOANTIALIAS
class csPixelCoord(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPixelCoord, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPixelCoord, name)
    __swig_setmethods__["x"] = _blcelc.csPixelCoord_x_set
    __swig_getmethods__["x"] = _blcelc.csPixelCoord_x_get
    if _newclass:x = property(_blcelc.csPixelCoord_x_get,_blcelc.csPixelCoord_x_set)
    __swig_setmethods__["y"] = _blcelc.csPixelCoord_y_set
    __swig_getmethods__["y"] = _blcelc.csPixelCoord_y_get
    if _newclass:y = property(_blcelc.csPixelCoord_y_get,_blcelc.csPixelCoord_y_set)
    def __init__(self,*args):
        _swig_setattr(self, csPixelCoord, 'this', apply(_blcelc.new_csPixelCoord,args))
        _swig_setattr(self, csPixelCoord, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csPixelCoord):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csPixelCoord instance at %s>" % (self.this,)

class csPixelCoordPtr(csPixelCoord):
    def __init__(self,this):
        _swig_setattr(self, csPixelCoord, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPixelCoord, 'thisown', 0)
        _swig_setattr(self, csPixelCoord,self.__class__,csPixelCoord)
_blcelc.csPixelCoord_swigregister(csPixelCoordPtr)

class csPixelFormat(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csPixelFormat, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csPixelFormat, name)
    __swig_setmethods__["RedMask"] = _blcelc.csPixelFormat_RedMask_set
    __swig_getmethods__["RedMask"] = _blcelc.csPixelFormat_RedMask_get
    if _newclass:RedMask = property(_blcelc.csPixelFormat_RedMask_get,_blcelc.csPixelFormat_RedMask_set)
    __swig_setmethods__["GreenMask"] = _blcelc.csPixelFormat_GreenMask_set
    __swig_getmethods__["GreenMask"] = _blcelc.csPixelFormat_GreenMask_get
    if _newclass:GreenMask = property(_blcelc.csPixelFormat_GreenMask_get,_blcelc.csPixelFormat_GreenMask_set)
    __swig_setmethods__["BlueMask"] = _blcelc.csPixelFormat_BlueMask_set
    __swig_getmethods__["BlueMask"] = _blcelc.csPixelFormat_BlueMask_get
    if _newclass:BlueMask = property(_blcelc.csPixelFormat_BlueMask_get,_blcelc.csPixelFormat_BlueMask_set)
    __swig_setmethods__["RedShift"] = _blcelc.csPixelFormat_RedShift_set
    __swig_getmethods__["RedShift"] = _blcelc.csPixelFormat_RedShift_get
    if _newclass:RedShift = property(_blcelc.csPixelFormat_RedShift_get,_blcelc.csPixelFormat_RedShift_set)
    __swig_setmethods__["GreenShift"] = _blcelc.csPixelFormat_GreenShift_set
    __swig_getmethods__["GreenShift"] = _blcelc.csPixelFormat_GreenShift_get
    if _newclass:GreenShift = property(_blcelc.csPixelFormat_GreenShift_get,_blcelc.csPixelFormat_GreenShift_set)
    __swig_setmethods__["BlueShift"] = _blcelc.csPixelFormat_BlueShift_set
    __swig_getmethods__["BlueShift"] = _blcelc.csPixelFormat_BlueShift_get
    if _newclass:BlueShift = property(_blcelc.csPixelFormat_BlueShift_get,_blcelc.csPixelFormat_BlueShift_set)
    __swig_setmethods__["RedBits"] = _blcelc.csPixelFormat_RedBits_set
    __swig_getmethods__["RedBits"] = _blcelc.csPixelFormat_RedBits_get
    if _newclass:RedBits = property(_blcelc.csPixelFormat_RedBits_get,_blcelc.csPixelFormat_RedBits_set)
    __swig_setmethods__["GreenBits"] = _blcelc.csPixelFormat_GreenBits_set
    __swig_getmethods__["GreenBits"] = _blcelc.csPixelFormat_GreenBits_get
    if _newclass:GreenBits = property(_blcelc.csPixelFormat_GreenBits_get,_blcelc.csPixelFormat_GreenBits_set)
    __swig_setmethods__["BlueBits"] = _blcelc.csPixelFormat_BlueBits_set
    __swig_getmethods__["BlueBits"] = _blcelc.csPixelFormat_BlueBits_get
    if _newclass:BlueBits = property(_blcelc.csPixelFormat_BlueBits_get,_blcelc.csPixelFormat_BlueBits_set)
    __swig_setmethods__["PalEntries"] = _blcelc.csPixelFormat_PalEntries_set
    __swig_getmethods__["PalEntries"] = _blcelc.csPixelFormat_PalEntries_get
    if _newclass:PalEntries = property(_blcelc.csPixelFormat_PalEntries_get,_blcelc.csPixelFormat_PalEntries_set)
    __swig_setmethods__["PixelBytes"] = _blcelc.csPixelFormat_PixelBytes_set
    __swig_getmethods__["PixelBytes"] = _blcelc.csPixelFormat_PixelBytes_get
    if _newclass:PixelBytes = property(_blcelc.csPixelFormat_PixelBytes_get,_blcelc.csPixelFormat_PixelBytes_set)
    def complete(*args): return apply(_blcelc.csPixelFormat_complete,args)
    def __init__(self,*args):
        _swig_setattr(self, csPixelFormat, 'this', apply(_blcelc.new_csPixelFormat,args))
        _swig_setattr(self, csPixelFormat, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csPixelFormat):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csPixelFormat instance at %s>" % (self.this,)

class csPixelFormatPtr(csPixelFormat):
    def __init__(self,this):
        _swig_setattr(self, csPixelFormat, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csPixelFormat, 'thisown', 0)
        _swig_setattr(self, csPixelFormat,self.__class__,csPixelFormat)
_blcelc.csPixelFormat_swigregister(csPixelFormatPtr)

class csImageArea(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csImageArea, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csImageArea, name)
    __swig_setmethods__["x"] = _blcelc.csImageArea_x_set
    __swig_getmethods__["x"] = _blcelc.csImageArea_x_get
    if _newclass:x = property(_blcelc.csImageArea_x_get,_blcelc.csImageArea_x_set)
    __swig_setmethods__["y"] = _blcelc.csImageArea_y_set
    __swig_getmethods__["y"] = _blcelc.csImageArea_y_get
    if _newclass:y = property(_blcelc.csImageArea_y_get,_blcelc.csImageArea_y_set)
    __swig_setmethods__["w"] = _blcelc.csImageArea_w_set
    __swig_getmethods__["w"] = _blcelc.csImageArea_w_get
    if _newclass:w = property(_blcelc.csImageArea_w_get,_blcelc.csImageArea_w_set)
    __swig_setmethods__["h"] = _blcelc.csImageArea_h_set
    __swig_getmethods__["h"] = _blcelc.csImageArea_h_get
    if _newclass:h = property(_blcelc.csImageArea_h_get,_blcelc.csImageArea_h_set)
    __swig_setmethods__["data"] = _blcelc.csImageArea_data_set
    __swig_getmethods__["data"] = _blcelc.csImageArea_data_get
    if _newclass:data = property(_blcelc.csImageArea_data_get,_blcelc.csImageArea_data_set)
    def __init__(self,*args):
        _swig_setattr(self, csImageArea, 'this', apply(_blcelc.new_csImageArea,args))
        _swig_setattr(self, csImageArea, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csImageArea):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csImageArea instance at %s>" % (self.this,)

class csImageAreaPtr(csImageArea):
    def __init__(self,this):
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
    def FinishDraw(*args): return apply(_blcelc.iOffscreenCanvasCallback_FinishDraw,args)
    def SetRGB(*args): return apply(_blcelc.iOffscreenCanvasCallback_SetRGB,args)
    def __del__(self, destroy= _blcelc.delete_iOffscreenCanvasCallback):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iOffscreenCanvasCallback instance at %s>" % (self.this,)

class iOffscreenCanvasCallbackPtr(iOffscreenCanvasCallback):
    def __init__(self,this):
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
    def Open(*args): return apply(_blcelc.iGraphics2D_Open,args)
    def Close(*args): return apply(_blcelc.iGraphics2D_Close,args)
    def GetWidth(*args): return apply(_blcelc.iGraphics2D_GetWidth,args)
    def GetHeight(*args): return apply(_blcelc.iGraphics2D_GetHeight,args)
    def GetPage(*args): return apply(_blcelc.iGraphics2D_GetPage,args)
    def DoubleBuffer(*args): return apply(_blcelc.iGraphics2D_DoubleBuffer,args)
    def GetDoubleBufferState(*args): return apply(_blcelc.iGraphics2D_GetDoubleBufferState,args)
    def GetPixelFormat(*args): return apply(_blcelc.iGraphics2D_GetPixelFormat,args)
    def GetPixelBytes(*args): return apply(_blcelc.iGraphics2D_GetPixelBytes,args)
    def GetPalEntryCount(*args): return apply(_blcelc.iGraphics2D_GetPalEntryCount,args)
    def GetPalette(*args): return apply(_blcelc.iGraphics2D_GetPalette,args)
    def SetRGB(*args): return apply(_blcelc.iGraphics2D_SetRGB,args)
    def FindRGB(*args): return apply(_blcelc.iGraphics2D_FindRGB,args)
    def SetClipRect(*args): return apply(_blcelc.iGraphics2D_SetClipRect,args)
    def GetClipRect(*args): return apply(_blcelc.iGraphics2D_GetClipRect,args)
    def BeginDraw(*args): return apply(_blcelc.iGraphics2D_BeginDraw,args)
    def FinishDraw(*args): return apply(_blcelc.iGraphics2D_FinishDraw,args)
    def Print(*args): return apply(_blcelc.iGraphics2D_Print,args)
    def Clear(*args): return apply(_blcelc.iGraphics2D_Clear,args)
    def ClearAll(*args): return apply(_blcelc.iGraphics2D_ClearAll,args)
    def DrawLine(*args): return apply(_blcelc.iGraphics2D_DrawLine,args)
    def DrawBox(*args): return apply(_blcelc.iGraphics2D_DrawBox,args)
    def ClipLine(*args): return apply(_blcelc.iGraphics2D_ClipLine,args)
    def DrawPixel(*args): return apply(_blcelc.iGraphics2D_DrawPixel,args)
    def DrawPixels(*args): return apply(_blcelc.iGraphics2D_DrawPixels,args)
    def Blit(*args): return apply(_blcelc.iGraphics2D_Blit,args)
    def GetPixelAt(*args): return apply(_blcelc.iGraphics2D_GetPixelAt,args)
    def GetPixel(*args): return apply(_blcelc.iGraphics2D_GetPixel,args)
    def SaveArea(*args): return apply(_blcelc.iGraphics2D_SaveArea,args)
    def RestoreArea(*args): return apply(_blcelc.iGraphics2D_RestoreArea,args)
    def FreeArea(*args): return apply(_blcelc.iGraphics2D_FreeArea,args)
    def Write(*args): return apply(_blcelc.iGraphics2D_Write,args)
    def WriteBaseline(*args): return apply(_blcelc.iGraphics2D_WriteBaseline,args)
    def AllowResize(*args): return apply(_blcelc.iGraphics2D_AllowResize,args)
    def Resize(*args): return apply(_blcelc.iGraphics2D_Resize,args)
    def GetFontServer(*args): return apply(_blcelc.iGraphics2D_GetFontServer,args)
    def PerformExtension(*args): return apply(_blcelc.iGraphics2D_PerformExtension,args)
    def PerformExtensionV(*args): return apply(_blcelc.iGraphics2D_PerformExtensionV,args)
    def ScreenShot(*args): return apply(_blcelc.iGraphics2D_ScreenShot,args)
    def GetNativeWindow(*args): return apply(_blcelc.iGraphics2D_GetNativeWindow,args)
    def GetFullScreen(*args): return apply(_blcelc.iGraphics2D_GetFullScreen,args)
    def SetFullScreen(*args): return apply(_blcelc.iGraphics2D_SetFullScreen,args)
    def SetMousePosition(*args): return apply(_blcelc.iGraphics2D_SetMousePosition,args)
    def SetMouseCursor(*args): return apply(_blcelc.iGraphics2D_SetMouseCursor,args)
    def SetGamma(*args): return apply(_blcelc.iGraphics2D_SetGamma,args)
    def GetGamma(*args): return apply(_blcelc.iGraphics2D_GetGamma,args)
    def CreateOffscreenCanvas(*args): return apply(_blcelc.iGraphics2D_CreateOffscreenCanvas,args)
    def __del__(self, destroy= _blcelc.delete_iGraphics2D):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iGraphics2D_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iGraphics2D_scfGetVersion)
    def _PerformExtension(*args): return apply(_blcelc.iGraphics2D__PerformExtension,args)
    def PerformExtension (self, command, *args):
              self._PerformExtension(self.__class__.__name__, command, args);


    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGraphics2D instance at %s>" % (self.this,)

class iGraphics2DPtr(iGraphics2D):
    def __init__(self,this):
        _swig_setattr(self, iGraphics2D, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iGraphics2D, 'thisown', 0)
        _swig_setattr(self, iGraphics2D,self.__class__,iGraphics2D)
_blcelc.iGraphics2D_swigregister(iGraphics2DPtr)
iGraphics2D_scfGetVersion = _blcelc.iGraphics2D_scfGetVersion


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
    def BeforeDelete(*args): return apply(_blcelc.iFontDeleteNotify_BeforeDelete,args)
    def __del__(self, destroy= _blcelc.delete_iFontDeleteNotify):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFontDeleteNotify instance at %s>" % (self.this,)

class iFontDeleteNotifyPtr(iFontDeleteNotify):
    def __init__(self,this):
        _swig_setattr(self, iFontDeleteNotify, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iFontDeleteNotify, 'thisown', 0)
        _swig_setattr(self, iFontDeleteNotify,self.__class__,iFontDeleteNotify)
_blcelc.iFontDeleteNotify_swigregister(iFontDeleteNotifyPtr)

class csBitmapMetrics(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csBitmapMetrics, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csBitmapMetrics, name)
    __swig_setmethods__["width"] = _blcelc.csBitmapMetrics_width_set
    __swig_getmethods__["width"] = _blcelc.csBitmapMetrics_width_get
    if _newclass:width = property(_blcelc.csBitmapMetrics_width_get,_blcelc.csBitmapMetrics_width_set)
    __swig_setmethods__["height"] = _blcelc.csBitmapMetrics_height_set
    __swig_getmethods__["height"] = _blcelc.csBitmapMetrics_height_get
    if _newclass:height = property(_blcelc.csBitmapMetrics_height_get,_blcelc.csBitmapMetrics_height_set)
    __swig_setmethods__["left"] = _blcelc.csBitmapMetrics_left_set
    __swig_getmethods__["left"] = _blcelc.csBitmapMetrics_left_get
    if _newclass:left = property(_blcelc.csBitmapMetrics_left_get,_blcelc.csBitmapMetrics_left_set)
    __swig_setmethods__["top"] = _blcelc.csBitmapMetrics_top_set
    __swig_getmethods__["top"] = _blcelc.csBitmapMetrics_top_get
    if _newclass:top = property(_blcelc.csBitmapMetrics_top_get,_blcelc.csBitmapMetrics_top_set)
    def __init__(self,*args):
        _swig_setattr(self, csBitmapMetrics, 'this', apply(_blcelc.new_csBitmapMetrics,args))
        _swig_setattr(self, csBitmapMetrics, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csBitmapMetrics):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csBitmapMetrics instance at %s>" % (self.this,)

class csBitmapMetricsPtr(csBitmapMetrics):
    def __init__(self,this):
        _swig_setattr(self, csBitmapMetrics, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csBitmapMetrics, 'thisown', 0)
        _swig_setattr(self, csBitmapMetrics,self.__class__,csBitmapMetrics)
_blcelc.csBitmapMetrics_swigregister(csBitmapMetricsPtr)

class csGlyphMetrics(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csGlyphMetrics, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csGlyphMetrics, name)
    __swig_setmethods__["advance"] = _blcelc.csGlyphMetrics_advance_set
    __swig_getmethods__["advance"] = _blcelc.csGlyphMetrics_advance_get
    if _newclass:advance = property(_blcelc.csGlyphMetrics_advance_get,_blcelc.csGlyphMetrics_advance_set)
    def __init__(self,*args):
        _swig_setattr(self, csGlyphMetrics, 'this', apply(_blcelc.new_csGlyphMetrics,args))
        _swig_setattr(self, csGlyphMetrics, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csGlyphMetrics):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csGlyphMetrics instance at %s>" % (self.this,)

class csGlyphMetricsPtr(csGlyphMetrics):
    def __init__(self,this):
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
    def AddDeleteCallback(*args): return apply(_blcelc.iFont_AddDeleteCallback,args)
    def RemoveDeleteCallback(*args): return apply(_blcelc.iFont_RemoveDeleteCallback,args)
    def SetSize(*args): return apply(_blcelc.iFont_SetSize,args)
    def GetSize(*args): return apply(_blcelc.iFont_GetSize,args)
    def GetMaxSize(*args): return apply(_blcelc.iFont_GetMaxSize,args)
    def GetGlyphMetrics(*args): return apply(_blcelc.iFont_GetGlyphMetrics,args)
    def GetGlyphBitmap(*args): return apply(_blcelc.iFont_GetGlyphBitmap,args)
    def GetGlyphAlphaBitmap(*args): return apply(_blcelc.iFont_GetGlyphAlphaBitmap,args)
    def GetDimensions(*args): return apply(_blcelc.iFont_GetDimensions,args)
    def GetLength(*args): return apply(_blcelc.iFont_GetLength,args)
    def GetDescent(*args): return apply(_blcelc.iFont_GetDescent,args)
    def GetAscent(*args): return apply(_blcelc.iFont_GetAscent,args)
    def HasGlyph(*args): return apply(_blcelc.iFont_HasGlyph,args)
    def __del__(self, destroy= _blcelc.delete_iFont):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFont_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFont_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFont instance at %s>" % (self.this,)

class iFontPtr(iFont):
    def __init__(self,this):
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
    def LoadFont(*args): return apply(_blcelc.iFontServer_LoadFont,args)
    def GetFontCount(*args): return apply(_blcelc.iFontServer_GetFontCount,args)
    def GetFont(*args): return apply(_blcelc.iFontServer_GetFont,args)
    def __del__(self, destroy= _blcelc.delete_iFontServer):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iFontServer_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iFontServer_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFontServer instance at %s>" % (self.this,)

class iFontServerPtr(iFontServer):
    def __init__(self,this):
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
    def GetWidth(*args): return apply(_blcelc.iHalo_GetWidth,args)
    def GetHeight(*args): return apply(_blcelc.iHalo_GetHeight,args)
    def SetColor(*args): return apply(_blcelc.iHalo_SetColor,args)
    def GetColor(*args): return apply(_blcelc.iHalo_GetColor,args)
    def Draw(*args): return apply(_blcelc.iHalo_Draw,args)
    def __del__(self, destroy= _blcelc.delete_iHalo):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iHalo_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iHalo_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iHalo instance at %s>" % (self.this,)

class iHaloPtr(iHalo):
    def __init__(self,this):
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
    def AddVariable(*args): return apply(_blcelc.iShaderVariableContext_AddVariable,args)
    def GetVariable(*args): return apply(_blcelc.iShaderVariableContext_GetVariable,args)
    def GetVariableRecursive(*args): return apply(_blcelc.iShaderVariableContext_GetVariableRecursive,args)
    def FillVariableList(*args): return apply(_blcelc.iShaderVariableContext_FillVariableList,args)
    def __del__(self, destroy= _blcelc.delete_iShaderVariableContext):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderVariableContext instance at %s>" % (self.this,)

class iShaderVariableContextPtr(iShaderVariableContext):
    def __init__(self,this):
        _swig_setattr(self, iShaderVariableContext, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderVariableContext, 'thisown', 0)
        _swig_setattr(self, iShaderVariableContext,self.__class__,iShaderVariableContext)
_blcelc.iShaderVariableContext_swigregister(iShaderVariableContextPtr)

class iShaderManager(iShaderVariableContext):
    __swig_setmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderManager, name)
    def CreateShader(*args): return apply(_blcelc.iShaderManager_CreateShader,args)
    def GetShader(*args): return apply(_blcelc.iShaderManager_GetShader,args)
    def GetShaders(*args): return apply(_blcelc.iShaderManager_GetShaders,args)
    def CreateVariable(*args): return apply(_blcelc.iShaderManager_CreateVariable,args)
    def CreateShaderProgram(*args): return apply(_blcelc.iShaderManager_CreateShaderProgram,args)
    def PrepareShaders(*args): return apply(_blcelc.iShaderManager_PrepareShaders,args)
    def __del__(self, destroy= _blcelc.delete_iShaderManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderManager instance at %s>" % (self.this,)

class iShaderManagerPtr(iShaderManager):
    def __init__(self,this):
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
    def GetPrivateObject(*args): return apply(_blcelc.iShaderRenderInterface_GetPrivateObject,args)
    def __del__(self, destroy= _blcelc.delete_iShaderRenderInterface):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderRenderInterface instance at %s>" % (self.this,)

class iShaderRenderInterfacePtr(iShaderRenderInterface):
    def __init__(self,this):
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
    def SetName(*args): return apply(_blcelc.iShader_SetName,args)
    def GetName(*args): return apply(_blcelc.iShader_GetName,args)
    def CreateTechnique(*args): return apply(_blcelc.iShader_CreateTechnique,args)
    def GetTechniqueCount(*args): return apply(_blcelc.iShader_GetTechniqueCount,args)
    def GetTechnique(*args): return apply(_blcelc.iShader_GetTechnique,args)
    def GetBestTechnique(*args): return apply(_blcelc.iShader_GetBestTechnique,args)
    def IsValid(*args): return apply(_blcelc.iShader_IsValid,args)
    def Load(*args): return apply(_blcelc.iShader_Load,args)
    def Prepare(*args): return apply(_blcelc.iShader_Prepare,args)
    def __del__(self, destroy= _blcelc.delete_iShader):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShader instance at %s>" % (self.this,)

class iShaderPtr(iShader):
    def __init__(self,this):
        _swig_setattr(self, iShader, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShader, 'thisown', 0)
        _swig_setattr(self, iShader,self.__class__,iShader)
_blcelc.iShader_swigregister(iShaderPtr)

class iShaderTechnique(iShaderVariableContext):
    __swig_setmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderTechnique, name, value)
    __swig_getmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderTechnique, name)
    def GetPriority(*args): return apply(_blcelc.iShaderTechnique_GetPriority,args)
    def SetPriority(*args): return apply(_blcelc.iShaderTechnique_SetPriority,args)
    def CreatePass(*args): return apply(_blcelc.iShaderTechnique_CreatePass,args)
    def GetPassCount(*args): return apply(_blcelc.iShaderTechnique_GetPassCount,args)
    def GetPass(*args): return apply(_blcelc.iShaderTechnique_GetPass,args)
    def IsValid(*args): return apply(_blcelc.iShaderTechnique_IsValid,args)
    def Load(*args): return apply(_blcelc.iShaderTechnique_Load,args)
    def Prepare(*args): return apply(_blcelc.iShaderTechnique_Prepare,args)
    def __del__(self, destroy= _blcelc.delete_iShaderTechnique):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderTechnique instance at %s>" % (self.this,)

class iShaderTechniquePtr(iShaderTechnique):
    def __init__(self,this):
        _swig_setattr(self, iShaderTechnique, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderTechnique, 'thisown', 0)
        _swig_setattr(self, iShaderTechnique,self.__class__,iShaderTechnique)
_blcelc.iShaderTechnique_swigregister(iShaderTechniquePtr)

class iShaderPass(iShaderVariableContext):
    __swig_setmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderPass, name, value)
    __swig_getmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderPass, name)
    def AddStreamMapping(*args): return apply(_blcelc.iShaderPass_AddStreamMapping,args)
    def GetStreamMapping(*args): return apply(_blcelc.iShaderPass_GetStreamMapping,args)
    def AddTextureMapping(*args): return apply(_blcelc.iShaderPass_AddTextureMapping,args)
    def GetTextureMapping(*args): return apply(_blcelc.iShaderPass_GetTextureMapping,args)
    def GetMixmodeOverride(*args): return apply(_blcelc.iShaderPass_GetMixmodeOverride,args)
    def GetVertexProgram(*args): return apply(_blcelc.iShaderPass_GetVertexProgram,args)
    def SetVertexProgram(*args): return apply(_blcelc.iShaderPass_SetVertexProgram,args)
    def GetFragmentProgram(*args): return apply(_blcelc.iShaderPass_GetFragmentProgram,args)
    def SetFragmentProgram(*args): return apply(_blcelc.iShaderPass_SetFragmentProgram,args)
    def IsValid(*args): return apply(_blcelc.iShaderPass_IsValid,args)
    def Activate(*args): return apply(_blcelc.iShaderPass_Activate,args)
    def Deactivate(*args): return apply(_blcelc.iShaderPass_Deactivate,args)
    def SetupState(*args): return apply(_blcelc.iShaderPass_SetupState,args)
    def ResetState(*args): return apply(_blcelc.iShaderPass_ResetState,args)
    def Load(*args): return apply(_blcelc.iShaderPass_Load,args)
    def Prepare(*args): return apply(_blcelc.iShaderPass_Prepare,args)
    def GetAlphaMode(*args): return apply(_blcelc.iShaderPass_GetAlphaMode,args)
    def __del__(self, destroy= _blcelc.delete_iShaderPass):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderPass instance at %s>" % (self.this,)

class iShaderPassPtr(iShaderPass):
    def __init__(self,this):
        _swig_setattr(self, iShaderPass, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderPass, 'thisown', 0)
        _swig_setattr(self, iShaderPass,self.__class__,iShaderPass)
_blcelc.iShaderPass_swigregister(iShaderPassPtr)

class iShaderProgram(iShaderVariableContext):
    __swig_setmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderProgram, name, value)
    __swig_getmethods__ = {}
    for _s in [iShaderVariableContext]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderProgram, name)
    def Activate(*args): return apply(_blcelc.iShaderProgram_Activate,args)
    def Deactivate(*args): return apply(_blcelc.iShaderProgram_Deactivate,args)
    def SetupState(*args): return apply(_blcelc.iShaderProgram_SetupState,args)
    def ResetState(*args): return apply(_blcelc.iShaderProgram_ResetState,args)
    def IsValid(*args): return apply(_blcelc.iShaderProgram_IsValid,args)
    def Load(*args): return apply(_blcelc.iShaderProgram_Load,args)
    def Prepare(*args): return apply(_blcelc.iShaderProgram_Prepare,args)
    def __del__(self, destroy= _blcelc.delete_iShaderProgram):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderProgram instance at %s>" % (self.this,)

class iShaderProgramPtr(iShaderProgram):
    def __init__(self,this):
        _swig_setattr(self, iShaderProgram, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderProgram, 'thisown', 0)
        _swig_setattr(self, iShaderProgram,self.__class__,iShaderProgram)
_blcelc.iShaderProgram_swigregister(iShaderProgramPtr)

class iShaderProgramPlugin(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iShaderProgramPlugin, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iShaderProgramPlugin, name)
    def CreateProgram(*args): return apply(_blcelc.iShaderProgramPlugin_CreateProgram,args)
    def SupportType(*args): return apply(_blcelc.iShaderProgramPlugin_SupportType,args)
    def Open(*args): return apply(_blcelc.iShaderProgramPlugin_Open,args)
    def __del__(self, destroy= _blcelc.delete_iShaderProgramPlugin):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iShaderProgramPlugin instance at %s>" % (self.this,)

class iShaderProgramPluginPtr(iShaderProgramPlugin):
    def __init__(self,this):
        _swig_setattr(self, iShaderProgramPlugin, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iShaderProgramPlugin, 'thisown', 0)
        _swig_setattr(self, iShaderProgramPlugin,self.__class__,iShaderProgramPlugin)
_blcelc.iShaderProgramPlugin_swigregister(iShaderProgramPluginPtr)

class iTextureHandle(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iTextureHandle, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iTextureHandle, name)
    def GetFlags(*args): return apply(_blcelc.iTextureHandle_GetFlags,args)
    def SetKeyColor(*args): return apply(_blcelc.iTextureHandle_SetKeyColor,args)
    def GetKeyColorStatus(*args): return apply(_blcelc.iTextureHandle_GetKeyColorStatus,args)
    def GetKeyColor(*args): return apply(_blcelc.iTextureHandle_GetKeyColor,args)
    def GetMipMapDimensions(*args): return apply(_blcelc.iTextureHandle_GetMipMapDimensions,args)
    def GetOriginalDimensions(*args): return apply(_blcelc.iTextureHandle_GetOriginalDimensions,args)
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
    def GetMeanColor(*args): return apply(_blcelc.iTextureHandle_GetMeanColor,args)
    def GetCacheData(*args): return apply(_blcelc.iTextureHandle_GetCacheData,args)
    def SetCacheData(*args): return apply(_blcelc.iTextureHandle_SetCacheData,args)
    def GetPrivateObject(*args): return apply(_blcelc.iTextureHandle_GetPrivateObject,args)
    def GetAlphaMap(*args): return apply(_blcelc.iTextureHandle_GetAlphaMap,args)
    def Prepare(*args): return apply(_blcelc.iTextureHandle_Prepare,args)
    def GetCanvas(*args): return apply(_blcelc.iTextureHandle_GetCanvas,args)
    def GetAlphaType(*args): return apply(_blcelc.iTextureHandle_GetAlphaType,args)
    def __del__(self, destroy= _blcelc.delete_iTextureHandle):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iTextureHandle_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iTextureHandle_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureHandle instance at %s>" % (self.this,)

class iTextureHandlePtr(iTextureHandle):
    def __init__(self,this):
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
    def GetSLMCoords(*args): return apply(_blcelc.iRendererLightmap_GetSLMCoords,args)
    def SetData(*args): return apply(_blcelc.iRendererLightmap_SetData,args)
    def SetLightCellSize(*args): return apply(_blcelc.iRendererLightmap_SetLightCellSize,args)
    def __del__(self, destroy= _blcelc.delete_iRendererLightmap):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iRendererLightmap instance at %s>" % (self.this,)

class iRendererLightmapPtr(iRendererLightmap):
    def __init__(self,this):
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
    def RegisterLightmap(*args): return apply(_blcelc.iSuperLightmap_RegisterLightmap,args)
    def Dump(*args): return apply(_blcelc.iSuperLightmap_Dump,args)
    def GetTexture(*args): return apply(_blcelc.iSuperLightmap_GetTexture,args)
    def __del__(self, destroy= _blcelc.delete_iSuperLightmap):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSuperLightmap instance at %s>" % (self.this,)

class iSuperLightmapPtr(iSuperLightmap):
    def __init__(self,this):
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
    def RegisterTexture(*args): return apply(_blcelc.iTextureManager_RegisterTexture,args)
    def PrepareTextures(*args): return apply(_blcelc.iTextureManager_PrepareTextures,args)
    def FreeImages(*args): return apply(_blcelc.iTextureManager_FreeImages,args)
    def RegisterMaterial(*args): return apply(_blcelc.iTextureManager_RegisterMaterial,args)
    def PrepareMaterials(*args): return apply(_blcelc.iTextureManager_PrepareMaterials,args)
    def FreeMaterials(*args): return apply(_blcelc.iTextureManager_FreeMaterials,args)
    def SetVerbose(*args): return apply(_blcelc.iTextureManager_SetVerbose,args)
    def GetTextureFormat(*args): return apply(_blcelc.iTextureManager_GetTextureFormat,args)
    def CreateSuperLightmap(*args): return apply(_blcelc.iTextureManager_CreateSuperLightmap,args)
    def GetMaxTextureSize(*args): return apply(_blcelc.iTextureManager_GetMaxTextureSize,args)
    def GetLightmapRendererCoords(*args): return apply(_blcelc.iTextureManager_GetLightmapRendererCoords,args)
    def __del__(self, destroy= _blcelc.delete_iTextureManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iTextureManager_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iTextureManager_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureManager instance at %s>" % (self.this,)

class iTextureManagerPtr(iTextureManager):
    def __init__(self,this):
        _swig_setattr(self, iTextureManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iTextureManager, 'thisown', 0)
        _swig_setattr(self, iTextureManager,self.__class__,iTextureManager)
_blcelc.iTextureManager_swigregister(iTextureManagerPtr)
iTextureManager_scfGetVersion = _blcelc.iTextureManager_scfGetVersion


class iVertexBuffer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVertexBuffer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVertexBuffer, name)
    def GetPriority(*args): return apply(_blcelc.iVertexBuffer_GetPriority,args)
    def IsLocked(*args): return apply(_blcelc.iVertexBuffer_IsLocked,args)
    def GetVertices(*args): return apply(_blcelc.iVertexBuffer_GetVertices,args)
    def GetTexels(*args): return apply(_blcelc.iVertexBuffer_GetTexels,args)
    def GetColors(*args): return apply(_blcelc.iVertexBuffer_GetColors,args)
    def GetUserArray(*args): return apply(_blcelc.iVertexBuffer_GetUserArray,args)
    def GetUserArrayComponentCount(*args): return apply(_blcelc.iVertexBuffer_GetUserArrayComponentCount,args)
    def GetVertexCount(*args): return apply(_blcelc.iVertexBuffer_GetVertexCount,args)
    def GetBoundingBox(*args): return apply(_blcelc.iVertexBuffer_GetBoundingBox,args)
    def __del__(self, destroy= _blcelc.delete_iVertexBuffer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVertexBuffer instance at %s>" % (self.this,)

class iVertexBufferPtr(iVertexBuffer):
    def __init__(self,this):
        _swig_setattr(self, iVertexBuffer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVertexBuffer, 'thisown', 0)
        _swig_setattr(self, iVertexBuffer,self.__class__,iVertexBuffer)
_blcelc.iVertexBuffer_swigregister(iVertexBufferPtr)

class iPolygonBuffer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPolygonBuffer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPolygonBuffer, name)
    def SetVertexArray(*args): return apply(_blcelc.iPolygonBuffer_SetVertexArray,args)
    def GetVertexCount(*args): return apply(_blcelc.iPolygonBuffer_GetVertexCount,args)
    def GetVertices(*args): return apply(_blcelc.iPolygonBuffer_GetVertices,args)
    def AddMaterial(*args): return apply(_blcelc.iPolygonBuffer_AddMaterial,args)
    def GetMaterialCount(*args): return apply(_blcelc.iPolygonBuffer_GetMaterialCount,args)
    def GetMaterial(*args): return apply(_blcelc.iPolygonBuffer_GetMaterial,args)
    def SetMaterial(*args): return apply(_blcelc.iPolygonBuffer_SetMaterial,args)
    def Clear(*args): return apply(_blcelc.iPolygonBuffer_Clear,args)
    def Prepare(*args): return apply(_blcelc.iPolygonBuffer_Prepare,args)
    def MarkLightmapsDirty(*args): return apply(_blcelc.iPolygonBuffer_MarkLightmapsDirty,args)
    def GetBoundingBox(*args): return apply(_blcelc.iPolygonBuffer_GetBoundingBox,args)
    def AddPolygon(*args): return apply(_blcelc.iPolygonBuffer_AddPolygon,args)
    def __del__(self, destroy= _blcelc.delete_iPolygonBuffer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPolygonBuffer instance at %s>" % (self.this,)

class iPolygonBufferPtr(iPolygonBuffer):
    def __init__(self,this):
        _swig_setattr(self, iPolygonBuffer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPolygonBuffer, 'thisown', 0)
        _swig_setattr(self, iPolygonBuffer,self.__class__,iPolygonBuffer)
_blcelc.iPolygonBuffer_swigregister(iPolygonBufferPtr)

class iVertexBufferManagerClient(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVertexBufferManagerClient, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVertexBufferManagerClient, name)
    def ManagerClosing(*args): return apply(_blcelc.iVertexBufferManagerClient_ManagerClosing,args)
    def __del__(self, destroy= _blcelc.delete_iVertexBufferManagerClient):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVertexBufferManagerClient instance at %s>" % (self.this,)

class iVertexBufferManagerClientPtr(iVertexBufferManagerClient):
    def __init__(self,this):
        _swig_setattr(self, iVertexBufferManagerClient, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVertexBufferManagerClient, 'thisown', 0)
        _swig_setattr(self, iVertexBufferManagerClient,self.__class__,iVertexBufferManagerClient)
_blcelc.iVertexBufferManagerClient_swigregister(iVertexBufferManagerClientPtr)

class iVertexBufferManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iVertexBufferManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iVertexBufferManager, name)
    def CreateBuffer(*args): return apply(_blcelc.iVertexBufferManager_CreateBuffer,args)
    def ChangePriority(*args): return apply(_blcelc.iVertexBufferManager_ChangePriority,args)
    def LockBuffer(*args): return apply(_blcelc.iVertexBufferManager_LockBuffer,args)
    def LockUserArray(*args): return apply(_blcelc.iVertexBufferManager_LockUserArray,args)
    def UnlockBuffer(*args): return apply(_blcelc.iVertexBufferManager_UnlockBuffer,args)
    def CreatePolygonBuffer(*args): return apply(_blcelc.iVertexBufferManager_CreatePolygonBuffer,args)
    def AddClient(*args): return apply(_blcelc.iVertexBufferManager_AddClient,args)
    def RemoveClient(*args): return apply(_blcelc.iVertexBufferManager_RemoveClient,args)
    def __del__(self, destroy= _blcelc.delete_iVertexBufferManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVertexBufferManager instance at %s>" % (self.this,)

class iVertexBufferManagerPtr(iVertexBufferManager):
    def __init__(self,this):
        _swig_setattr(self, iVertexBufferManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iVertexBufferManager, 'thisown', 0)
        _swig_setattr(self, iVertexBufferManager,self.__class__,iVertexBufferManager)
_blcelc.iVertexBufferManager_swigregister(iVertexBufferManagerPtr)

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
    __swig_setmethods__["txt_handle"] = _blcelc.csTextureLayer_txt_handle_set
    __swig_getmethods__["txt_handle"] = _blcelc.csTextureLayer_txt_handle_get
    if _newclass:txt_handle = property(_blcelc.csTextureLayer_txt_handle_get,_blcelc.csTextureLayer_txt_handle_set)
    __swig_setmethods__["mode"] = _blcelc.csTextureLayer_mode_set
    __swig_getmethods__["mode"] = _blcelc.csTextureLayer_mode_get
    if _newclass:mode = property(_blcelc.csTextureLayer_mode_get,_blcelc.csTextureLayer_mode_set)
    __swig_setmethods__["uscale"] = _blcelc.csTextureLayer_uscale_set
    __swig_getmethods__["uscale"] = _blcelc.csTextureLayer_uscale_get
    if _newclass:uscale = property(_blcelc.csTextureLayer_uscale_get,_blcelc.csTextureLayer_uscale_set)
    __swig_setmethods__["vscale"] = _blcelc.csTextureLayer_vscale_set
    __swig_getmethods__["vscale"] = _blcelc.csTextureLayer_vscale_get
    if _newclass:vscale = property(_blcelc.csTextureLayer_vscale_get,_blcelc.csTextureLayer_vscale_set)
    __swig_setmethods__["ushift"] = _blcelc.csTextureLayer_ushift_set
    __swig_getmethods__["ushift"] = _blcelc.csTextureLayer_ushift_get
    if _newclass:ushift = property(_blcelc.csTextureLayer_ushift_get,_blcelc.csTextureLayer_ushift_set)
    __swig_setmethods__["vshift"] = _blcelc.csTextureLayer_vshift_set
    __swig_getmethods__["vshift"] = _blcelc.csTextureLayer_vshift_get
    if _newclass:vshift = property(_blcelc.csTextureLayer_vshift_get,_blcelc.csTextureLayer_vshift_set)
    def __init__(self,*args):
        _swig_setattr(self, csTextureLayer, 'this', apply(_blcelc.new_csTextureLayer,args))
        _swig_setattr(self, csTextureLayer, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csTextureLayer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csTextureLayer instance at %s>" % (self.this,)

class csTextureLayerPtr(csTextureLayer):
    def __init__(self,this):
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
    def SetShader(*args): return apply(_blcelc.iMaterial_SetShader,args)
    def GetShader(*args): return apply(_blcelc.iMaterial_GetShader,args)
    def SetEffect(*args): return apply(_blcelc.iMaterial_SetEffect,args)
    def GetEffect(*args): return apply(_blcelc.iMaterial_GetEffect,args)
    def GetTexture(*args): return apply(_blcelc.iMaterial_GetTexture,args)
    def GetTextureLayerCount(*args): return apply(_blcelc.iMaterial_GetTextureLayerCount,args)
    def GetTextureLayer(*args): return apply(_blcelc.iMaterial_GetTextureLayer,args)
    def GetFlatColor(*args): return apply(_blcelc.iMaterial_GetFlatColor,args)
    def SetFlatColor(*args): return apply(_blcelc.iMaterial_SetFlatColor,args)
    def GetReflection(*args): return apply(_blcelc.iMaterial_GetReflection,args)
    def SetReflection(*args): return apply(_blcelc.iMaterial_SetReflection,args)
    def __del__(self, destroy= _blcelc.delete_iMaterial):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iMaterial_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iMaterial_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterial instance at %s>" % (self.this,)

class iMaterialPtr(iMaterial):
    def __init__(self,this):
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
    def GetShader(*args): return apply(_blcelc.iMaterialHandle_GetShader,args)
    def GetTexture(*args): return apply(_blcelc.iMaterialHandle_GetTexture,args)
    def GetFlatColor(*args): return apply(_blcelc.iMaterialHandle_GetFlatColor,args)
    def GetReflection(*args): return apply(_blcelc.iMaterialHandle_GetReflection,args)
    def Prepare(*args): return apply(_blcelc.iMaterialHandle_Prepare,args)
    def __del__(self, destroy= _blcelc.delete_iMaterialHandle):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialHandle instance at %s>" % (self.this,)

class iMaterialHandlePtr(iMaterialHandle):
    def __init__(self,this):
        _swig_setattr(self, iMaterialHandle, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iMaterialHandle, 'thisown', 0)
        _swig_setattr(self, iMaterialHandle,self.__class__,iMaterialHandle)
_blcelc.iMaterialHandle_swigregister(iMaterialHandlePtr)

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
    def Alert(*args): return apply(_blcelc.iNativeWindowManager_Alert,args)
    def AlertV(*args): return apply(_blcelc.iNativeWindowManager_AlertV,args)
    def __del__(self, destroy= _blcelc.delete_iNativeWindowManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNativeWindowManager instance at %s>" % (self.this,)

class iNativeWindowManagerPtr(iNativeWindowManager):
    def __init__(self,this):
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
    def SetTitle(*args): return apply(_blcelc.iNativeWindow_SetTitle,args)
    def __del__(self, destroy= _blcelc.delete_iNativeWindow):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNativeWindow instance at %s>" % (self.this,)

class iNativeWindowPtr(iNativeWindow):
    def __init__(self,this):
        _swig_setattr(self, iNativeWindow, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNativeWindow, 'thisown', 0)
        _swig_setattr(self, iNativeWindow,self.__class__,iNativeWindow)
_blcelc.iNativeWindow_swigregister(iNativeWindowPtr)

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
    __swig_setmethods__["type"] = _blcelc.csStreamDescription_type_set
    __swig_getmethods__["type"] = _blcelc.csStreamDescription_type_get
    if _newclass:type = property(_blcelc.csStreamDescription_type_get,_blcelc.csStreamDescription_type_set)
    __swig_setmethods__["codec"] = _blcelc.csStreamDescription_codec_set
    __swig_getmethods__["codec"] = _blcelc.csStreamDescription_codec_get
    if _newclass:codec = property(_blcelc.csStreamDescription_codec_get,_blcelc.csStreamDescription_codec_set)
    __swig_getmethods__["name"] = _blcelc.csStreamDescription_name_get
    if _newclass:name = property(_blcelc.csStreamDescription_name_get)
    def __init__(self,*args):
        _swig_setattr(self, csStreamDescription, 'this', apply(_blcelc.new_csStreamDescription,args))
        _swig_setattr(self, csStreamDescription, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csStreamDescription):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csStreamDescription instance at %s>" % (self.this,)

class csStreamDescriptionPtr(csStreamDescription):
    def __init__(self,this):
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
    __swig_setmethods__["colordepth"] = _blcelc.csVideoStreamDescription_colordepth_set
    __swig_getmethods__["colordepth"] = _blcelc.csVideoStreamDescription_colordepth_get
    if _newclass:colordepth = property(_blcelc.csVideoStreamDescription_colordepth_get,_blcelc.csVideoStreamDescription_colordepth_set)
    __swig_setmethods__["framecount"] = _blcelc.csVideoStreamDescription_framecount_set
    __swig_getmethods__["framecount"] = _blcelc.csVideoStreamDescription_framecount_get
    if _newclass:framecount = property(_blcelc.csVideoStreamDescription_framecount_get,_blcelc.csVideoStreamDescription_framecount_set)
    __swig_setmethods__["width"] = _blcelc.csVideoStreamDescription_width_set
    __swig_getmethods__["width"] = _blcelc.csVideoStreamDescription_width_get
    if _newclass:width = property(_blcelc.csVideoStreamDescription_width_get,_blcelc.csVideoStreamDescription_width_set)
    __swig_setmethods__["height"] = _blcelc.csVideoStreamDescription_height_set
    __swig_getmethods__["height"] = _blcelc.csVideoStreamDescription_height_get
    if _newclass:height = property(_blcelc.csVideoStreamDescription_height_get,_blcelc.csVideoStreamDescription_height_set)
    __swig_setmethods__["framerate"] = _blcelc.csVideoStreamDescription_framerate_set
    __swig_getmethods__["framerate"] = _blcelc.csVideoStreamDescription_framerate_get
    if _newclass:framerate = property(_blcelc.csVideoStreamDescription_framerate_get,_blcelc.csVideoStreamDescription_framerate_set)
    __swig_setmethods__["duration"] = _blcelc.csVideoStreamDescription_duration_set
    __swig_getmethods__["duration"] = _blcelc.csVideoStreamDescription_duration_get
    if _newclass:duration = property(_blcelc.csVideoStreamDescription_duration_get,_blcelc.csVideoStreamDescription_duration_set)
    def __init__(self,*args):
        _swig_setattr(self, csVideoStreamDescription, 'this', apply(_blcelc.new_csVideoStreamDescription,args))
        _swig_setattr(self, csVideoStreamDescription, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csVideoStreamDescription):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csVideoStreamDescription instance at %s>" % (self.this,)

class csVideoStreamDescriptionPtr(csVideoStreamDescription):
    def __init__(self,this):
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
    __swig_setmethods__["formattag"] = _blcelc.csAudioStreamDescription_formattag_set
    __swig_getmethods__["formattag"] = _blcelc.csAudioStreamDescription_formattag_get
    if _newclass:formattag = property(_blcelc.csAudioStreamDescription_formattag_get,_blcelc.csAudioStreamDescription_formattag_set)
    __swig_setmethods__["channels"] = _blcelc.csAudioStreamDescription_channels_set
    __swig_getmethods__["channels"] = _blcelc.csAudioStreamDescription_channels_get
    if _newclass:channels = property(_blcelc.csAudioStreamDescription_channels_get,_blcelc.csAudioStreamDescription_channels_set)
    __swig_setmethods__["samplespersecond"] = _blcelc.csAudioStreamDescription_samplespersecond_set
    __swig_getmethods__["samplespersecond"] = _blcelc.csAudioStreamDescription_samplespersecond_get
    if _newclass:samplespersecond = property(_blcelc.csAudioStreamDescription_samplespersecond_get,_blcelc.csAudioStreamDescription_samplespersecond_set)
    __swig_setmethods__["bitspersample"] = _blcelc.csAudioStreamDescription_bitspersample_set
    __swig_getmethods__["bitspersample"] = _blcelc.csAudioStreamDescription_bitspersample_get
    if _newclass:bitspersample = property(_blcelc.csAudioStreamDescription_bitspersample_get,_blcelc.csAudioStreamDescription_bitspersample_set)
    __swig_setmethods__["duration"] = _blcelc.csAudioStreamDescription_duration_set
    __swig_getmethods__["duration"] = _blcelc.csAudioStreamDescription_duration_get
    if _newclass:duration = property(_blcelc.csAudioStreamDescription_duration_get,_blcelc.csAudioStreamDescription_duration_set)
    def __init__(self,*args):
        _swig_setattr(self, csAudioStreamDescription, 'this', apply(_blcelc.new_csAudioStreamDescription,args))
        _swig_setattr(self, csAudioStreamDescription, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csAudioStreamDescription):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csAudioStreamDescription instance at %s>" % (self.this,)

class csAudioStreamDescriptionPtr(csAudioStreamDescription):
    def __init__(self,this):
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
    def HasNext(*args): return apply(_blcelc.iStreamIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.iStreamIterator_Next,args)
    def __del__(self, destroy= _blcelc.delete_iStreamIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStreamIterator_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStreamIterator_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStreamIterator instance at %s>" % (self.this,)

class iStreamIteratorPtr(iStreamIterator):
    def __init__(self,this):
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
    def GetCaps(*args): return apply(_blcelc.iStreamFormat_GetCaps,args)
    def GetStreamIterator(*args): return apply(_blcelc.iStreamFormat_GetStreamIterator,args)
    def Select(*args): return apply(_blcelc.iStreamFormat_Select,args)
    def NextFrame(*args): return apply(_blcelc.iStreamFormat_NextFrame,args)
    def Load(*args): return apply(_blcelc.iStreamFormat_Load,args)
    def Unload(*args): return apply(_blcelc.iStreamFormat_Unload,args)
    def __del__(self, destroy= _blcelc.delete_iStreamFormat):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStreamFormat_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStreamFormat_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStreamFormat instance at %s>" % (self.this,)

class iStreamFormatPtr(iStreamFormat):
    def __init__(self,this):
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
    def GetStreamDescription(*args): return apply(_blcelc.iStream_GetStreamDescription,args)
    def GotoFrame(*args): return apply(_blcelc.iStream_GotoFrame,args)
    def GotoTime(*args): return apply(_blcelc.iStream_GotoTime,args)
    def SetPlayMethod(*args): return apply(_blcelc.iStream_SetPlayMethod,args)
    def NextFrame(*args): return apply(_blcelc.iStream_NextFrame,args)
    def __del__(self, destroy= _blcelc.delete_iStream):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iStream_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iStream_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStream instance at %s>" % (self.this,)

class iStreamPtr(iStream):
    def __init__(self,this):
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
    def GetStreamDescription(*args): return apply(_blcelc.iVideoStream_GetStreamDescription,args)
    def SetRect(*args): return apply(_blcelc.iVideoStream_SetRect,args)
    def SetFXMode(*args): return apply(_blcelc.iVideoStream_SetFXMode,args)
    def NextFrameGetMaterial(*args): return apply(_blcelc.iVideoStream_NextFrameGetMaterial,args)
    def __del__(self, destroy= _blcelc.delete_iVideoStream):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iVideoStream_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iVideoStream_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iVideoStream instance at %s>" % (self.this,)

class iVideoStreamPtr(iVideoStream):
    def __init__(self,this):
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
    def GetStreamDescription(*args): return apply(_blcelc.iAudioStream_GetStreamDescription,args)
    def __del__(self, destroy= _blcelc.delete_iAudioStream):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iAudioStream_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iAudioStream_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iAudioStream instance at %s>" % (self.this,)

class iAudioStreamPtr(iAudioStream):
    def __init__(self,this):
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
    __swig_setmethods__["codec"] = _blcelc.csCodecDescription_codec_set
    __swig_getmethods__["codec"] = _blcelc.csCodecDescription_codec_get
    if _newclass:codec = property(_blcelc.csCodecDescription_codec_get,_blcelc.csCodecDescription_codec_set)
    __swig_setmethods__["bEncode"] = _blcelc.csCodecDescription_bEncode_set
    __swig_getmethods__["bEncode"] = _blcelc.csCodecDescription_bEncode_get
    if _newclass:bEncode = property(_blcelc.csCodecDescription_bEncode_get,_blcelc.csCodecDescription_bEncode_set)
    __swig_setmethods__["bDecode"] = _blcelc.csCodecDescription_bDecode_set
    __swig_getmethods__["bDecode"] = _blcelc.csCodecDescription_bDecode_get
    if _newclass:bDecode = property(_blcelc.csCodecDescription_bDecode_get,_blcelc.csCodecDescription_bDecode_set)
    __swig_setmethods__["decodeoutput"] = _blcelc.csCodecDescription_decodeoutput_set
    __swig_getmethods__["decodeoutput"] = _blcelc.csCodecDescription_decodeoutput_get
    if _newclass:decodeoutput = property(_blcelc.csCodecDescription_decodeoutput_get,_blcelc.csCodecDescription_decodeoutput_set)
    __swig_setmethods__["encodeinput"] = _blcelc.csCodecDescription_encodeinput_set
    __swig_getmethods__["encodeinput"] = _blcelc.csCodecDescription_encodeinput_get
    if _newclass:encodeinput = property(_blcelc.csCodecDescription_encodeinput_get,_blcelc.csCodecDescription_encodeinput_set)
    def __init__(self,*args):
        _swig_setattr(self, csCodecDescription, 'this', apply(_blcelc.new_csCodecDescription,args))
        _swig_setattr(self, csCodecDescription, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csCodecDescription):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csCodecDescription instance at %s>" % (self.this,)

class csCodecDescriptionPtr(csCodecDescription):
    def __init__(self,this):
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
class iImage(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iImage, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iImage, name)
    def GetImageData(*args): return apply(_blcelc.iImage_GetImageData,args)
    def GetWidth(*args): return apply(_blcelc.iImage_GetWidth,args)
    def GetHeight(*args): return apply(_blcelc.iImage_GetHeight,args)
    def GetSize(*args): return apply(_blcelc.iImage_GetSize,args)
    def Rescale(*args): return apply(_blcelc.iImage_Rescale,args)
    def MipMap(*args): return apply(_blcelc.iImage_MipMap,args)
    def SetName(*args): return apply(_blcelc.iImage_SetName,args)
    def GetName(*args): return apply(_blcelc.iImage_GetName,args)
    def GetFormat(*args): return apply(_blcelc.iImage_GetFormat,args)
    def GetPalette(*args): return apply(_blcelc.iImage_GetPalette,args)
    def GetAlpha(*args): return apply(_blcelc.iImage_GetAlpha,args)
    def SetFormat(*args): return apply(_blcelc.iImage_SetFormat,args)
    def Clone(*args): return apply(_blcelc.iImage_Clone,args)
    def Crop(*args): return apply(_blcelc.iImage_Crop,args)
    def CheckAlpha(*args): return apply(_blcelc.iImage_CheckAlpha,args)
    def HasKeycolor(*args): return apply(_blcelc.iImage_HasKeycolor,args)
    def GetKeycolor(*args): return apply(_blcelc.iImage_GetKeycolor,args)
    def Sharpen(*args): return apply(_blcelc.iImage_Sharpen,args)
    def HasMipmaps(*args): return apply(_blcelc.iImage_HasMipmaps,args)
    def __del__(self, destroy= _blcelc.delete_iImage):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iImage_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iImage_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iImage instance at %s>" % (self.this,)

class iImagePtr(iImage):
    def __init__(self,this):
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
    __swig_getmethods__["mime"] = _blcelc.csImageIOFileFormatDescription_mime_get
    if _newclass:mime = property(_blcelc.csImageIOFileFormatDescription_mime_get)
    __swig_getmethods__["subtype"] = _blcelc.csImageIOFileFormatDescription_subtype_get
    if _newclass:subtype = property(_blcelc.csImageIOFileFormatDescription_subtype_get)
    __swig_setmethods__["cap"] = _blcelc.csImageIOFileFormatDescription_cap_set
    __swig_getmethods__["cap"] = _blcelc.csImageIOFileFormatDescription_cap_get
    if _newclass:cap = property(_blcelc.csImageIOFileFormatDescription_cap_get,_blcelc.csImageIOFileFormatDescription_cap_set)
    def __init__(self,*args):
        _swig_setattr(self, csImageIOFileFormatDescription, 'this', apply(_blcelc.new_csImageIOFileFormatDescription,args))
        _swig_setattr(self, csImageIOFileFormatDescription, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csImageIOFileFormatDescription):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csImageIOFileFormatDescription instance at %s>" % (self.this,)

class csImageIOFileFormatDescriptionPtr(csImageIOFileFormatDescription):
    def __init__(self,this):
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
    def GetDescription(*args): return apply(_blcelc.iImageIO_GetDescription,args)
    def Load(*args): return apply(_blcelc.iImageIO_Load,args)
    def SetDithering(*args): return apply(_blcelc.iImageIO_SetDithering,args)
    def Save(*args): return apply(_blcelc.iImageIO_Save,args)
    def __del__(self, destroy= _blcelc.delete_iImageIO):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iImageIO_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iImageIO_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iImageIO instance at %s>" % (self.this,)

class iImageIOPtr(iImageIO):
    def __init__(self,this):
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
    def Report(*args): return apply(_blcelc.iReporterListener_Report,args)
    def __del__(self, destroy= _blcelc.delete_iReporterListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iReporterListener instance at %s>" % (self.this,)

class iReporterListenerPtr(iReporterListener):
    def __init__(self,this):
        _swig_setattr(self, iReporterListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iReporterListener, 'thisown', 0)
        _swig_setattr(self, iReporterListener,self.__class__,iReporterListener)
_blcelc.iReporterListener_swigregister(iReporterListenerPtr)

class iReporterIterator(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iReporterIterator, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iReporterIterator, name)
    def HasNext(*args): return apply(_blcelc.iReporterIterator_HasNext,args)
    def Next(*args): return apply(_blcelc.iReporterIterator_Next,args)
    def GetMessageSeverity(*args): return apply(_blcelc.iReporterIterator_GetMessageSeverity,args)
    def GetMessageId(*args): return apply(_blcelc.iReporterIterator_GetMessageId,args)
    def GetMessageDescription(*args): return apply(_blcelc.iReporterIterator_GetMessageDescription,args)
    def __del__(self, destroy= _blcelc.delete_iReporterIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iReporterIterator instance at %s>" % (self.this,)

class iReporterIteratorPtr(iReporterIterator):
    def __init__(self,this):
        _swig_setattr(self, iReporterIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iReporterIterator, 'thisown', 0)
        _swig_setattr(self, iReporterIterator,self.__class__,iReporterIterator)
_blcelc.iReporterIterator_swigregister(iReporterIteratorPtr)

class iReporter(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iReporter, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iReporter, name)
    def Report(*args): return apply(_blcelc.iReporter_Report,args)
    def Clear(*args): return apply(_blcelc.iReporter_Clear,args)
    def GetMessageIterator(*args): return apply(_blcelc.iReporter_GetMessageIterator,args)
    def AddReporterListener(*args): return apply(_blcelc.iReporter_AddReporterListener,args)
    def RemoveReporterListener(*args): return apply(_blcelc.iReporter_RemoveReporterListener,args)
    def FindReporterListener(*args): return apply(_blcelc.iReporter_FindReporterListener,args)
    def ReportError(*args): return apply(_blcelc.iReporter_ReportError,args)
    def ReportWarning(*args): return apply(_blcelc.iReporter_ReportWarning,args)
    def ReportNotify(*args): return apply(_blcelc.iReporter_ReportNotify,args)
    def ReportBug(*args): return apply(_blcelc.iReporter_ReportBug,args)
    def ReportDebug(*args): return apply(_blcelc.iReporter_ReportDebug,args)
    def __del__(self, destroy= _blcelc.delete_iReporter):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iReporter instance at %s>" % (self.this,)

class iReporterPtr(iReporter):
    def __init__(self,this):
        _swig_setattr(self, iReporter, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iReporter, 'thisown', 0)
        _swig_setattr(self, iReporter,self.__class__,iReporter)
_blcelc.iReporter_swigregister(iReporterPtr)

class csReporterHelper(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csReporterHelper, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csReporterHelper, name)
    __swig_getmethods__["Report"] = lambda x: _blcelc.csReporterHelper_Report
    if _newclass:Report = staticmethod(_blcelc.csReporterHelper_Report)
    def __init__(self,*args):
        _swig_setattr(self, csReporterHelper, 'this', apply(_blcelc.new_csReporterHelper,args))
        _swig_setattr(self, csReporterHelper, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csReporterHelper):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csReporterHelper instance at %s>" % (self.this,)

class csReporterHelperPtr(csReporterHelper):
    def __init__(self,this):
        _swig_setattr(self, csReporterHelper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csReporterHelper, 'thisown', 0)
        _swig_setattr(self, csReporterHelper,self.__class__,csReporterHelper)
_blcelc.csReporterHelper_swigregister(csReporterHelperPtr)
csReporterHelper_Report = _blcelc.csReporterHelper_Report


class iStandardReporterListener(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iStandardReporterListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iStandardReporterListener, name)
    def SetOutputConsole(*args): return apply(_blcelc.iStandardReporterListener_SetOutputConsole,args)
    def SetNativeWindowManager(*args): return apply(_blcelc.iStandardReporterListener_SetNativeWindowManager,args)
    def SetReporter(*args): return apply(_blcelc.iStandardReporterListener_SetReporter,args)
    def SetDebugFile(*args): return apply(_blcelc.iStandardReporterListener_SetDebugFile,args)
    def SetDefaults(*args): return apply(_blcelc.iStandardReporterListener_SetDefaults,args)
    def SetMessageDestination(*args): return apply(_blcelc.iStandardReporterListener_SetMessageDestination,args)
    def RemoveMessages(*args): return apply(_blcelc.iStandardReporterListener_RemoveMessages,args)
    def ShowMessageID(*args): return apply(_blcelc.iStandardReporterListener_ShowMessageID,args)
    def __del__(self, destroy= _blcelc.delete_iStandardReporterListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iStandardReporterListener instance at %s>" % (self.this,)

class iStandardReporterListenerPtr(iStandardReporterListener):
    def __init__(self,this):
        _swig_setattr(self, iStandardReporterListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iStandardReporterListener, 'thisown', 0)
        _swig_setattr(self, iStandardReporterListener,self.__class__,iStandardReporterListener)
_blcelc.iStandardReporterListener_swigregister(iStandardReporterListenerPtr)

class iView(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iView, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iView, name)
    def GetEngine(*args): return apply(_blcelc.iView_GetEngine,args)
    def SetEngine(*args): return apply(_blcelc.iView_SetEngine,args)
    def GetCamera(*args): return apply(_blcelc.iView_GetCamera,args)
    def SetCamera(*args): return apply(_blcelc.iView_SetCamera,args)
    def GetContext(*args): return apply(_blcelc.iView_GetContext,args)
    def SetContext(*args): return apply(_blcelc.iView_SetContext,args)
    def SetRectangle(*args): return apply(_blcelc.iView_SetRectangle,args)
    def ClearView(*args): return apply(_blcelc.iView_ClearView,args)
    def AddViewVertex(*args): return apply(_blcelc.iView_AddViewVertex,args)
    def RestrictClipperToScreen(*args): return apply(_blcelc.iView_RestrictClipperToScreen,args)
    def UpdateClipper(*args): return apply(_blcelc.iView_UpdateClipper,args)
    def GetClipper(*args): return apply(_blcelc.iView_GetClipper,args)
    def Draw(*args): return apply(_blcelc.iView_Draw,args)
    def __del__(self, destroy= _blcelc.delete_iView):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iView_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iView_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iView instance at %s>" % (self.this,)

class iViewPtr(iView):
    def __init__(self,this):
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
    __swig_setmethods__["a1"] = _blcelc.csCollisionPair_a1_set
    __swig_getmethods__["a1"] = _blcelc.csCollisionPair_a1_get
    if _newclass:a1 = property(_blcelc.csCollisionPair_a1_get,_blcelc.csCollisionPair_a1_set)
    __swig_setmethods__["b1"] = _blcelc.csCollisionPair_b1_set
    __swig_getmethods__["b1"] = _blcelc.csCollisionPair_b1_get
    if _newclass:b1 = property(_blcelc.csCollisionPair_b1_get,_blcelc.csCollisionPair_b1_set)
    __swig_setmethods__["c1"] = _blcelc.csCollisionPair_c1_set
    __swig_getmethods__["c1"] = _blcelc.csCollisionPair_c1_get
    if _newclass:c1 = property(_blcelc.csCollisionPair_c1_get,_blcelc.csCollisionPair_c1_set)
    __swig_setmethods__["a2"] = _blcelc.csCollisionPair_a2_set
    __swig_getmethods__["a2"] = _blcelc.csCollisionPair_a2_get
    if _newclass:a2 = property(_blcelc.csCollisionPair_a2_get,_blcelc.csCollisionPair_a2_set)
    __swig_setmethods__["b2"] = _blcelc.csCollisionPair_b2_set
    __swig_getmethods__["b2"] = _blcelc.csCollisionPair_b2_get
    if _newclass:b2 = property(_blcelc.csCollisionPair_b2_get,_blcelc.csCollisionPair_b2_set)
    __swig_setmethods__["c2"] = _blcelc.csCollisionPair_c2_set
    __swig_getmethods__["c2"] = _blcelc.csCollisionPair_c2_get
    if _newclass:c2 = property(_blcelc.csCollisionPair_c2_get,_blcelc.csCollisionPair_c2_set)
    def __init__(self,*args):
        _swig_setattr(self, csCollisionPair, 'this', apply(_blcelc.new_csCollisionPair,args))
        _swig_setattr(self, csCollisionPair, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csCollisionPair):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csCollisionPair instance at %s>" % (self.this,)

class csCollisionPairPtr(csCollisionPair):
    def __init__(self,this):
        _swig_setattr(self, csCollisionPair, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csCollisionPair, 'thisown', 0)
        _swig_setattr(self, csCollisionPair,self.__class__,csCollisionPair)
_blcelc.csCollisionPair_swigregister(csCollisionPairPtr)

class iCollider(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCollider, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCollider, name)
    def __del__(self, destroy= _blcelc.delete_iCollider):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCollider_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCollider_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCollider instance at %s>" % (self.this,)

class iColliderPtr(iCollider):
    def __init__(self,this):
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
    def CreateCollider(*args): return apply(_blcelc.iCollideSystem_CreateCollider,args)
    def CreateSphereCollider(*args): return apply(_blcelc.iCollideSystem_CreateSphereCollider,args)
    def CreateBoxCollider(*args): return apply(_blcelc.iCollideSystem_CreateBoxCollider,args)
    def Collide(*args): return apply(_blcelc.iCollideSystem_Collide,args)
    def GetCollisionPairs(*args): return apply(_blcelc.iCollideSystem_GetCollisionPairs,args)
    def GetCollisionPairCount(*args): return apply(_blcelc.iCollideSystem_GetCollisionPairCount,args)
    def ResetCollisionPairs(*args): return apply(_blcelc.iCollideSystem_ResetCollisionPairs,args)
    def SetOneHitOnly(*args): return apply(_blcelc.iCollideSystem_SetOneHitOnly,args)
    def GetOneHitOnly(*args): return apply(_blcelc.iCollideSystem_GetOneHitOnly,args)
    def CollidePath(*args): return apply(_blcelc.iCollideSystem_CollidePath,args)
    def __del__(self, destroy= _blcelc.delete_iCollideSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iCollideSystem_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iCollideSystem_scfGetVersion)
    def GetCollisionPairByIndex(*args): return apply(_blcelc.iCollideSystem_GetCollisionPairByIndex,args)
    def GetCollisionPairs (self):
    	num = self.GetCollisionPairCount()
    	pairs = []
    	for i in range(num):
    		pairs.append(self.GetCollisionPairByIndex(i))
    	return pairs


    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCollideSystem instance at %s>" % (self.this,)

class iCollideSystemPtr(iCollideSystem):
    def __init__(self,this):
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
    def CreateSystem(*args): return apply(_blcelc.iDynamics_CreateSystem,args)
    def RemoveSystem(*args): return apply(_blcelc.iDynamics_RemoveSystem,args)
    def FindSystem(*args): return apply(_blcelc.iDynamics_FindSystem,args)
    def Step(*args): return apply(_blcelc.iDynamics_Step,args)
    def __del__(self, destroy= _blcelc.delete_iDynamics):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDynamics_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDynamics_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDynamics instance at %s>" % (self.this,)

class iDynamicsPtr(iDynamics):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iDynamicSystem_QueryObject,args)
    def SetGravity(*args): return apply(_blcelc.iDynamicSystem_SetGravity,args)
    def GetGravity(*args): return apply(_blcelc.iDynamicSystem_GetGravity,args)
    def SetLinearDampener(*args): return apply(_blcelc.iDynamicSystem_SetLinearDampener,args)
    def GetLinearDampener(*args): return apply(_blcelc.iDynamicSystem_GetLinearDampener,args)
    def SetRollingDampener(*args): return apply(_blcelc.iDynamicSystem_SetRollingDampener,args)
    def GetRollingDampener(*args): return apply(_blcelc.iDynamicSystem_GetRollingDampener,args)
    def Step(*args): return apply(_blcelc.iDynamicSystem_Step,args)
    def CreateBody(*args): return apply(_blcelc.iDynamicSystem_CreateBody,args)
    def RemoveBody(*args): return apply(_blcelc.iDynamicSystem_RemoveBody,args)
    def FindBody(*args): return apply(_blcelc.iDynamicSystem_FindBody,args)
    def CreateGroup(*args): return apply(_blcelc.iDynamicSystem_CreateGroup,args)
    def RemoveGroup(*args): return apply(_blcelc.iDynamicSystem_RemoveGroup,args)
    def CreateJoint(*args): return apply(_blcelc.iDynamicSystem_CreateJoint,args)
    def RemoveJoint(*args): return apply(_blcelc.iDynamicSystem_RemoveJoint,args)
    def GetDefaultMoveCallback(*args): return apply(_blcelc.iDynamicSystem_GetDefaultMoveCallback,args)
    def AttachColliderMesh(*args): return apply(_blcelc.iDynamicSystem_AttachColliderMesh,args)
    def AttachColliderCylinder(*args): return apply(_blcelc.iDynamicSystem_AttachColliderCylinder,args)
    def AttachColliderBox(*args): return apply(_blcelc.iDynamicSystem_AttachColliderBox,args)
    def AttachColliderSphere(*args): return apply(_blcelc.iDynamicSystem_AttachColliderSphere,args)
    def AttachColliderPlane(*args): return apply(_blcelc.iDynamicSystem_AttachColliderPlane,args)
    def __del__(self, destroy= _blcelc.delete_iDynamicSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iDynamicSystem_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iDynamicSystem_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDynamicSystem instance at %s>" % (self.this,)

class iDynamicSystemPtr(iDynamicSystem):
    def __init__(self,this):
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
    def Execute(*args): return apply(_blcelc.iDynamicsMoveCallback_Execute,args)
    def __del__(self, destroy= _blcelc.delete_iDynamicsMoveCallback):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDynamicsMoveCallback instance at %s>" % (self.this,)

class iDynamicsMoveCallbackPtr(iDynamicsMoveCallback):
    def __init__(self,this):
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
    def Execute(*args): return apply(_blcelc.iDynamicsCollisionCallback_Execute,args)
    def __del__(self, destroy= _blcelc.delete_iDynamicsCollisionCallback):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iDynamicsCollisionCallback instance at %s>" % (self.this,)

class iDynamicsCollisionCallbackPtr(iDynamicsCollisionCallback):
    def __init__(self,this):
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
    def AddBody(*args): return apply(_blcelc.iBodyGroup_AddBody,args)
    def RemoveBody(*args): return apply(_blcelc.iBodyGroup_RemoveBody,args)
    def BodyInGroup(*args): return apply(_blcelc.iBodyGroup_BodyInGroup,args)
    def __del__(self, destroy= _blcelc.delete_iBodyGroup):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iBodyGroup_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iBodyGroup_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBodyGroup instance at %s>" % (self.this,)

class iBodyGroupPtr(iBodyGroup):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iRigidBody_QueryObject,args)
    def MakeStatic(*args): return apply(_blcelc.iRigidBody_MakeStatic,args)
    def MakeDynamic(*args): return apply(_blcelc.iRigidBody_MakeDynamic,args)
    def IsStatic(*args): return apply(_blcelc.iRigidBody_IsStatic,args)
    def GetGroup(*args): return apply(_blcelc.iRigidBody_GetGroup,args)
    def AttachColliderMesh(*args): return apply(_blcelc.iRigidBody_AttachColliderMesh,args)
    def AttachColliderCylinder(*args): return apply(_blcelc.iRigidBody_AttachColliderCylinder,args)
    def AttachColliderBox(*args): return apply(_blcelc.iRigidBody_AttachColliderBox,args)
    def AttachColliderSphere(*args): return apply(_blcelc.iRigidBody_AttachColliderSphere,args)
    def AttachColliderPlane(*args): return apply(_blcelc.iRigidBody_AttachColliderPlane,args)
    def SetPosition(*args): return apply(_blcelc.iRigidBody_SetPosition,args)
    def GetPosition(*args): return apply(_blcelc.iRigidBody_GetPosition,args)
    def SetOrientation(*args): return apply(_blcelc.iRigidBody_SetOrientation,args)
    def GetOrientation(*args): return apply(_blcelc.iRigidBody_GetOrientation,args)
    def SetTransform(*args): return apply(_blcelc.iRigidBody_SetTransform,args)
    def GetTransform(*args): return apply(_blcelc.iRigidBody_GetTransform,args)
    def SetLinearVelocity(*args): return apply(_blcelc.iRigidBody_SetLinearVelocity,args)
    def GetLinearVelocity(*args): return apply(_blcelc.iRigidBody_GetLinearVelocity,args)
    def SetAngularVelocity(*args): return apply(_blcelc.iRigidBody_SetAngularVelocity,args)
    def GetAngularVelocity(*args): return apply(_blcelc.iRigidBody_GetAngularVelocity,args)
    def SetProperties(*args): return apply(_blcelc.iRigidBody_SetProperties,args)
    def GetProperties(*args): return apply(_blcelc.iRigidBody_GetProperties,args)
    def AdjustTotalMass(*args): return apply(_blcelc.iRigidBody_AdjustTotalMass,args)
    def AddForce(*args): return apply(_blcelc.iRigidBody_AddForce,args)
    def AddTorque(*args): return apply(_blcelc.iRigidBody_AddTorque,args)
    def AddRelForce(*args): return apply(_blcelc.iRigidBody_AddRelForce,args)
    def AddRelTorque(*args): return apply(_blcelc.iRigidBody_AddRelTorque,args)
    def AddForceAtPos(*args): return apply(_blcelc.iRigidBody_AddForceAtPos,args)
    def AddForceAtRelPos(*args): return apply(_blcelc.iRigidBody_AddForceAtRelPos,args)
    def AddRelForceAtPos(*args): return apply(_blcelc.iRigidBody_AddRelForceAtPos,args)
    def AddRelForceAtRelPos(*args): return apply(_blcelc.iRigidBody_AddRelForceAtRelPos,args)
    def GetForce(*args): return apply(_blcelc.iRigidBody_GetForce,args)
    def GetTorque(*args): return apply(_blcelc.iRigidBody_GetTorque,args)
    def AttachMesh(*args): return apply(_blcelc.iRigidBody_AttachMesh,args)
    def GetAttachedMesh(*args): return apply(_blcelc.iRigidBody_GetAttachedMesh,args)
    def AttachBone(*args): return apply(_blcelc.iRigidBody_AttachBone,args)
    def GetAttachedBone(*args): return apply(_blcelc.iRigidBody_GetAttachedBone,args)
    def SetMoveCallback(*args): return apply(_blcelc.iRigidBody_SetMoveCallback,args)
    def SetCollisionCallback(*args): return apply(_blcelc.iRigidBody_SetCollisionCallback,args)
    def Collision(*args): return apply(_blcelc.iRigidBody_Collision,args)
    def Update(*args): return apply(_blcelc.iRigidBody_Update,args)
    def __del__(self, destroy= _blcelc.delete_iRigidBody):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iRigidBody instance at %s>" % (self.this,)

class iRigidBodyPtr(iRigidBody):
    def __init__(self,this):
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
    def Attach(*args): return apply(_blcelc.iJoint_Attach,args)
    def GetAttachedBody(*args): return apply(_blcelc.iJoint_GetAttachedBody,args)
    def SetTransform(*args): return apply(_blcelc.iJoint_SetTransform,args)
    def GetTransform(*args): return apply(_blcelc.iJoint_GetTransform,args)
    def SetTransConstraints(*args): return apply(_blcelc.iJoint_SetTransConstraints,args)
    def IsXTransConstrained(*args): return apply(_blcelc.iJoint_IsXTransConstrained,args)
    def IsYTransConstrained(*args): return apply(_blcelc.iJoint_IsYTransConstrained,args)
    def IsZTransConstrained(*args): return apply(_blcelc.iJoint_IsZTransConstrained,args)
    def SetMinimumDistance(*args): return apply(_blcelc.iJoint_SetMinimumDistance,args)
    def GetMinimumDistance(*args): return apply(_blcelc.iJoint_GetMinimumDistance,args)
    def SetMaximumDistance(*args): return apply(_blcelc.iJoint_SetMaximumDistance,args)
    def GetMaximumDistance(*args): return apply(_blcelc.iJoint_GetMaximumDistance,args)
    def SetRotConstraints(*args): return apply(_blcelc.iJoint_SetRotConstraints,args)
    def IsXRotConstrained(*args): return apply(_blcelc.iJoint_IsXRotConstrained,args)
    def IsYRotConstrained(*args): return apply(_blcelc.iJoint_IsYRotConstrained,args)
    def IsZRotConstrained(*args): return apply(_blcelc.iJoint_IsZRotConstrained,args)
    def SetMinimumAngle(*args): return apply(_blcelc.iJoint_SetMinimumAngle,args)
    def GetMinimumAngle(*args): return apply(_blcelc.iJoint_GetMinimumAngle,args)
    def SetMaximumAngle(*args): return apply(_blcelc.iJoint_SetMaximumAngle,args)
    def GetMaximumAngle(*args): return apply(_blcelc.iJoint_GetMaximumAngle,args)
    def __del__(self, destroy= _blcelc.delete_iJoint):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iJoint_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iJoint_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iJoint instance at %s>" % (self.this,)

class iJointPtr(iJoint):
    def __init__(self,this):
        _swig_setattr(self, iJoint, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iJoint, 'thisown', 0)
        _swig_setattr(self, iJoint,self.__class__,iJoint)
_blcelc.iJoint_swigregister(iJointPtr)
iJoint_scfGetVersion = _blcelc.iJoint_scfGetVersion


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
    def ConsoleVisibilityChanged(*args): return apply(_blcelc.iConsoleWatcher_ConsoleVisibilityChanged,args)
    def __del__(self, destroy= _blcelc.delete_iConsoleWatcher):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConsoleWatcher instance at %s>" % (self.this,)

class iConsoleWatcherPtr(iConsoleWatcher):
    def __init__(self,this):
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
    def PutText(*args): return apply(_blcelc.iConsoleOutput_PutText,args)
    def PutTextV(*args): return apply(_blcelc.iConsoleOutput_PutTextV,args)
    def GetLine(*args): return apply(_blcelc.iConsoleOutput_GetLine,args)
    def Draw2D(*args): return apply(_blcelc.iConsoleOutput_Draw2D,args)
    def Draw3D(*args): return apply(_blcelc.iConsoleOutput_Draw3D,args)
    def Clear(*args): return apply(_blcelc.iConsoleOutput_Clear,args)
    def SetBufferSize(*args): return apply(_blcelc.iConsoleOutput_SetBufferSize,args)
    def GetTransparency(*args): return apply(_blcelc.iConsoleOutput_GetTransparency,args)
    def SetTransparency(*args): return apply(_blcelc.iConsoleOutput_SetTransparency,args)
    def GetFont(*args): return apply(_blcelc.iConsoleOutput_GetFont,args)
    def SetFont(*args): return apply(_blcelc.iConsoleOutput_SetFont,args)
    def GetTopLine(*args): return apply(_blcelc.iConsoleOutput_GetTopLine,args)
    def ScrollTo(*args): return apply(_blcelc.iConsoleOutput_ScrollTo,args)
    def GetCursorStyle(*args): return apply(_blcelc.iConsoleOutput_GetCursorStyle,args)
    def SetCursorStyle(*args): return apply(_blcelc.iConsoleOutput_SetCursorStyle,args)
    def SetVisible(*args): return apply(_blcelc.iConsoleOutput_SetVisible,args)
    def GetVisible(*args): return apply(_blcelc.iConsoleOutput_GetVisible,args)
    def AutoUpdate(*args): return apply(_blcelc.iConsoleOutput_AutoUpdate,args)
    def SetCursorPos(*args): return apply(_blcelc.iConsoleOutput_SetCursorPos,args)
    def GetMaxLineWidth(*args): return apply(_blcelc.iConsoleOutput_GetMaxLineWidth,args)
    def RegisterWatcher(*args): return apply(_blcelc.iConsoleOutput_RegisterWatcher,args)
    def PerformExtension(*args): return apply(_blcelc.iConsoleOutput_PerformExtension,args)
    def PerformExtensionV(*args): return apply(_blcelc.iConsoleOutput_PerformExtensionV,args)
    def __del__(self, destroy= _blcelc.delete_iConsoleOutput):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iConsoleOutput instance at %s>" % (self.this,)

class iConsoleOutputPtr(iConsoleOutput):
    def __init__(self,this):
        _swig_setattr(self, iConsoleOutput, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iConsoleOutput, 'thisown', 0)
        _swig_setattr(self, iConsoleOutput,self.__class__,iConsoleOutput)
_blcelc.iConsoleOutput_swigregister(iConsoleOutputPtr)

class iParameterESM(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iParameterESM, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iParameterESM, name)
    def GetValue(*args): return apply(_blcelc.iParameterESM_GetValue,args)
    def IsConstant(*args): return apply(_blcelc.iParameterESM_IsConstant,args)
    def __del__(self, destroy= _blcelc.delete_iParameterESM):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iParameterESM instance at %s>" % (self.this,)

class iParameterESMPtr(iParameterESM):
    def __init__(self,this):
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
    def GetParameterCount(*args): return apply(_blcelc.iEngineSequenceParameters_GetParameterCount,args)
    def GetParameter(*args): return apply(_blcelc.iEngineSequenceParameters_GetParameter,args)
    def GetParameterIdx(*args): return apply(_blcelc.iEngineSequenceParameters_GetParameterIdx,args)
    def GetParameterName(*args): return apply(_blcelc.iEngineSequenceParameters_GetParameterName,args)
    def AddParameter(*args): return apply(_blcelc.iEngineSequenceParameters_AddParameter,args)
    def SetParameter(*args): return apply(_blcelc.iEngineSequenceParameters_SetParameter,args)
    def CreateParameterESM(*args): return apply(_blcelc.iEngineSequenceParameters_CreateParameterESM,args)
    def __del__(self, destroy= _blcelc.delete_iEngineSequenceParameters):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEngineSequenceParameters instance at %s>" % (self.this,)

class iEngineSequenceParametersPtr(iEngineSequenceParameters):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iSequenceWrapper_QueryObject,args)
    def GetSequence(*args): return apply(_blcelc.iSequenceWrapper_GetSequence,args)
    def CreateBaseParameterBlock(*args): return apply(_blcelc.iSequenceWrapper_CreateBaseParameterBlock,args)
    def GetBaseParameterBlock(*args): return apply(_blcelc.iSequenceWrapper_GetBaseParameterBlock,args)
    def CreateParameterBlock(*args): return apply(_blcelc.iSequenceWrapper_CreateParameterBlock,args)
    def AddOperationSetVariable(*args): return apply(_blcelc.iSequenceWrapper_AddOperationSetVariable,args)
    def AddOperationSetMaterial(*args): return apply(_blcelc.iSequenceWrapper_AddOperationSetMaterial,args)
    def AddOperationSetPolygonMaterial(*args): return apply(_blcelc.iSequenceWrapper_AddOperationSetPolygonMaterial,args)
    def AddOperationSetLight(*args): return apply(_blcelc.iSequenceWrapper_AddOperationSetLight,args)
    def AddOperationFadeLight(*args): return apply(_blcelc.iSequenceWrapper_AddOperationFadeLight,args)
    def AddOperationSetAmbient(*args): return apply(_blcelc.iSequenceWrapper_AddOperationSetAmbient,args)
    def AddOperationFadeAmbient(*args): return apply(_blcelc.iSequenceWrapper_AddOperationFadeAmbient,args)
    def AddOperationRandomDelay(*args): return apply(_blcelc.iSequenceWrapper_AddOperationRandomDelay,args)
    def AddOperationSetMeshColor(*args): return apply(_blcelc.iSequenceWrapper_AddOperationSetMeshColor,args)
    def AddOperationFadeMeshColor(*args): return apply(_blcelc.iSequenceWrapper_AddOperationFadeMeshColor,args)
    def AddOperationSetFog(*args): return apply(_blcelc.iSequenceWrapper_AddOperationSetFog,args)
    def AddOperationFadeFog(*args): return apply(_blcelc.iSequenceWrapper_AddOperationFadeFog,args)
    def AddOperationRotateDuration(*args): return apply(_blcelc.iSequenceWrapper_AddOperationRotateDuration,args)
    def AddOperationMoveDuration(*args): return apply(_blcelc.iSequenceWrapper_AddOperationMoveDuration,args)
    def AddOperationTriggerState(*args): return apply(_blcelc.iSequenceWrapper_AddOperationTriggerState,args)
    def AddOperationCheckTrigger(*args): return apply(_blcelc.iSequenceWrapper_AddOperationCheckTrigger,args)
    def AddOperationTestTrigger(*args): return apply(_blcelc.iSequenceWrapper_AddOperationTestTrigger,args)
    def __del__(self, destroy= _blcelc.delete_iSequenceWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSequenceWrapper instance at %s>" % (self.this,)

class iSequenceWrapperPtr(iSequenceWrapper):
    def __init__(self,this):
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
    def QueryObject(*args): return apply(_blcelc.iSequenceTrigger_QueryObject,args)
    def AddConditionInSector(*args): return apply(_blcelc.iSequenceTrigger_AddConditionInSector,args)
    def AddConditionSectorVisible(*args): return apply(_blcelc.iSequenceTrigger_AddConditionSectorVisible,args)
    def AddConditionMeshClick(*args): return apply(_blcelc.iSequenceTrigger_AddConditionMeshClick,args)
    def AddConditionLightChange(*args): return apply(_blcelc.iSequenceTrigger_AddConditionLightChange,args)
    def AddConditionManual(*args): return apply(_blcelc.iSequenceTrigger_AddConditionManual,args)
    def SetEnabled(*args): return apply(_blcelc.iSequenceTrigger_SetEnabled,args)
    def IsEnabled(*args): return apply(_blcelc.iSequenceTrigger_IsEnabled,args)
    def ClearConditions(*args): return apply(_blcelc.iSequenceTrigger_ClearConditions,args)
    def Trigger(*args): return apply(_blcelc.iSequenceTrigger_Trigger,args)
    def SetParameters(*args): return apply(_blcelc.iSequenceTrigger_SetParameters,args)
    def GetParameters(*args): return apply(_blcelc.iSequenceTrigger_GetParameters,args)
    def FireSequence(*args): return apply(_blcelc.iSequenceTrigger_FireSequence,args)
    def GetFiredSequence(*args): return apply(_blcelc.iSequenceTrigger_GetFiredSequence,args)
    def TestConditions(*args): return apply(_blcelc.iSequenceTrigger_TestConditions,args)
    def CheckState(*args): return apply(_blcelc.iSequenceTrigger_CheckState,args)
    def ForceFire(*args): return apply(_blcelc.iSequenceTrigger_ForceFire,args)
    def __del__(self, destroy= _blcelc.delete_iSequenceTrigger):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSequenceTrigger instance at %s>" % (self.this,)

class iSequenceTriggerPtr(iSequenceTrigger):
    def __init__(self,this):
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
    def Do(*args): return apply(_blcelc.iSequenceTimedOperation_Do,args)
    def __del__(self, destroy= _blcelc.delete_iSequenceTimedOperation):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSequenceTimedOperation instance at %s>" % (self.this,)

class iSequenceTimedOperationPtr(iSequenceTimedOperation):
    def __init__(self,this):
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
    def GetSequenceManager(*args): return apply(_blcelc.iEngineSequenceManager_GetSequenceManager,args)
    def SetCamera(*args): return apply(_blcelc.iEngineSequenceManager_SetCamera,args)
    def GetCamera(*args): return apply(_blcelc.iEngineSequenceManager_GetCamera,args)
    def CreateParameterESM(*args): return apply(_blcelc.iEngineSequenceManager_CreateParameterESM,args)
    def CreateTrigger(*args): return apply(_blcelc.iEngineSequenceManager_CreateTrigger,args)
    def RemoveTrigger(*args): return apply(_blcelc.iEngineSequenceManager_RemoveTrigger,args)
    def RemoveTriggers(*args): return apply(_blcelc.iEngineSequenceManager_RemoveTriggers,args)
    def GetTriggerCount(*args): return apply(_blcelc.iEngineSequenceManager_GetTriggerCount,args)
    def GetTrigger(*args): return apply(_blcelc.iEngineSequenceManager_GetTrigger,args)
    def FindTriggerByName(*args): return apply(_blcelc.iEngineSequenceManager_FindTriggerByName,args)
    def FireTriggerByName(*args): return apply(_blcelc.iEngineSequenceManager_FireTriggerByName,args)
    def CreateSequence(*args): return apply(_blcelc.iEngineSequenceManager_CreateSequence,args)
    def RemoveSequence(*args): return apply(_blcelc.iEngineSequenceManager_RemoveSequence,args)
    def RemoveSequences(*args): return apply(_blcelc.iEngineSequenceManager_RemoveSequences,args)
    def GetSequenceCount(*args): return apply(_blcelc.iEngineSequenceManager_GetSequenceCount,args)
    def GetSequence(*args): return apply(_blcelc.iEngineSequenceManager_GetSequence,args)
    def FindSequenceByName(*args): return apply(_blcelc.iEngineSequenceManager_FindSequenceByName,args)
    def RunSequenceByName(*args): return apply(_blcelc.iEngineSequenceManager_RunSequenceByName,args)
    def FireTimedOperation(*args): return apply(_blcelc.iEngineSequenceManager_FireTimedOperation,args)
    def __del__(self, destroy= _blcelc.delete_iEngineSequenceManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEngineSequenceManager instance at %s>" % (self.this,)

class iEngineSequenceManagerPtr(iEngineSequenceManager):
    def __init__(self,this):
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
    def IsType(*args): return apply(_blcelc.iScriptObject_IsType,args)
    def GetPointer(*args): return apply(_blcelc.iScriptObject_GetPointer,args)
    def SetPointer(*args): return apply(_blcelc.iScriptObject_SetPointer,args)
    def Call(*args): return apply(_blcelc.iScriptObject_Call,args)
    def IntCall(*args): return apply(_blcelc.iScriptObject_IntCall,args)
    def FloatCall(*args): return apply(_blcelc.iScriptObject_FloatCall,args)
    def DoubleCall(*args): return apply(_blcelc.iScriptObject_DoubleCall,args)
    def StringCall(*args): return apply(_blcelc.iScriptObject_StringCall,args)
    def ObjectCall(*args): return apply(_blcelc.iScriptObject_ObjectCall,args)
    def SetInt(*args): return apply(_blcelc.iScriptObject_SetInt,args)
    def SetFloat(*args): return apply(_blcelc.iScriptObject_SetFloat,args)
    def SetDouble(*args): return apply(_blcelc.iScriptObject_SetDouble,args)
    def Set(*args): return apply(_blcelc.iScriptObject_Set,args)
    def SetTruth(*args): return apply(_blcelc.iScriptObject_SetTruth,args)
    def GetFloat(*args): return apply(_blcelc.iScriptObject_GetFloat,args)
    def Get(*args): return apply(_blcelc.iScriptObject_Get,args)
    def GetTruth(*args): return apply(_blcelc.iScriptObject_GetTruth,args)
    def __del__(self, destroy= _blcelc.delete_iScriptObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iScriptObject_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iScriptObject_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iScriptObject instance at %s>" % (self.this,)

class iScriptObjectPtr(iScriptObject):
    def __init__(self,this):
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
    def Initialize(*args): return apply(_blcelc.iScript_Initialize,args)
    def RunText(*args): return apply(_blcelc.iScript_RunText,args)
    def LoadModule(*args): return apply(_blcelc.iScript_LoadModule,args)
    def Call(*args): return apply(_blcelc.iScript_Call,args)
    def IntCall(*args): return apply(_blcelc.iScript_IntCall,args)
    def FloatCall(*args): return apply(_blcelc.iScript_FloatCall,args)
    def DoubleCall(*args): return apply(_blcelc.iScript_DoubleCall,args)
    def StringCall(*args): return apply(_blcelc.iScript_StringCall,args)
    def ObjectCall(*args): return apply(_blcelc.iScript_ObjectCall,args)
    def NewObject(*args): return apply(_blcelc.iScript_NewObject,args)
    def StoreInt(*args): return apply(_blcelc.iScript_StoreInt,args)
    def StoreFloat(*args): return apply(_blcelc.iScript_StoreFloat,args)
    def StoreDouble(*args): return apply(_blcelc.iScript_StoreDouble,args)
    def Store(*args): return apply(_blcelc.iScript_Store,args)
    def SetTruth(*args): return apply(_blcelc.iScript_SetTruth,args)
    def RetrieveFloat(*args): return apply(_blcelc.iScript_RetrieveFloat,args)
    def Retrieve(*args): return apply(_blcelc.iScript_Retrieve,args)
    def GetTruth(*args): return apply(_blcelc.iScript_GetTruth,args)
    def Remove(*args): return apply(_blcelc.iScript_Remove,args)
    def __del__(self, destroy= _blcelc.delete_iScript):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iScript_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iScript_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iScript instance at %s>" % (self.this,)

class iScriptPtr(iScript):
    def __init__(self,this):
        _swig_setattr(self, iScript, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iScript, 'thisown', 0)
        _swig_setattr(self, iScript,self.__class__,iScript)
_blcelc.iScript_swigregister(iScriptPtr)
iScript_scfGetVersion = _blcelc.iScript_scfGetVersion


class iNetworkPacket(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNetworkPacket, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNetworkPacket, name)
    def Read(*args): return apply(_blcelc.iNetworkPacket_Read,args)
    def Write(*args): return apply(_blcelc.iNetworkPacket_Write,args)
    def New(*args): return apply(_blcelc.iNetworkPacket_New,args)
    def FilterSocket(*args): return apply(_blcelc.iNetworkPacket_FilterSocket,args)
    def __del__(self, destroy= _blcelc.delete_iNetworkPacket):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iNetworkPacket_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iNetworkPacket_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNetworkPacket instance at %s>" % (self.this,)

class iNetworkPacketPtr(iNetworkPacket):
    def __init__(self,this):
        _swig_setattr(self, iNetworkPacket, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNetworkPacket, 'thisown', 0)
        _swig_setattr(self, iNetworkPacket,self.__class__,iNetworkPacket)
_blcelc.iNetworkPacket_swigregister(iNetworkPacketPtr)
iNetworkPacket_scfGetVersion = _blcelc.iNetworkPacket_scfGetVersion


class iNetworkPacket2(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNetworkPacket2, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNetworkPacket2, name)
    def Read(*args): return apply(_blcelc.iNetworkPacket2_Read,args)
    def Write(*args): return apply(_blcelc.iNetworkPacket2_Write,args)
    def New(*args): return apply(_blcelc.iNetworkPacket2_New,args)
    def FilterSocket(*args): return apply(_blcelc.iNetworkPacket2_FilterSocket,args)
    def __del__(self, destroy= _blcelc.delete_iNetworkPacket2):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNetworkPacket2 instance at %s>" % (self.this,)

class iNetworkPacket2Ptr(iNetworkPacket2):
    def __init__(self,this):
        _swig_setattr(self, iNetworkPacket2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNetworkPacket2, 'thisown', 0)
        _swig_setattr(self, iNetworkPacket2,self.__class__,iNetworkPacket2)
_blcelc.iNetworkPacket2_swigregister(iNetworkPacket2Ptr)

class iNetworkManager(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNetworkManager, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNetworkManager, name)
    def RegisterConnection(*args): return apply(_blcelc.iNetworkManager_RegisterConnection,args)
    def RegisterListener(*args): return apply(_blcelc.iNetworkManager_RegisterListener,args)
    def UnregisterEndPoint(*args): return apply(_blcelc.iNetworkManager_UnregisterEndPoint,args)
    def RegisterConnectedSocket(*args): return apply(_blcelc.iNetworkManager_RegisterConnectedSocket,args)
    def UnregisterConnectedSocket(*args): return apply(_blcelc.iNetworkManager_UnregisterConnectedSocket,args)
    def RegisterListeningSocket(*args): return apply(_blcelc.iNetworkManager_RegisterListeningSocket,args)
    def UnregisterListeningSocket(*args): return apply(_blcelc.iNetworkManager_UnregisterListeningSocket,args)
    def Send(*args): return apply(_blcelc.iNetworkManager_Send,args)
    def SendToAll(*args): return apply(_blcelc.iNetworkManager_SendToAll,args)
    def __del__(self, destroy= _blcelc.delete_iNetworkManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iNetworkManager_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iNetworkManager_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNetworkManager instance at %s>" % (self.this,)

class iNetworkManagerPtr(iNetworkManager):
    def __init__(self,this):
        _swig_setattr(self, iNetworkManager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNetworkManager, 'thisown', 0)
        _swig_setattr(self, iNetworkManager,self.__class__,iNetworkManager)
_blcelc.iNetworkManager_swigregister(iNetworkManagerPtr)
iNetworkManager_scfGetVersion = _blcelc.iNetworkManager_scfGetVersion


CS_NET_SOCKET_TYPE_TCP = _blcelc.CS_NET_SOCKET_TYPE_TCP
CS_NET_SOCKET_TYPE_UDP = _blcelc.CS_NET_SOCKET_TYPE_UDP
CS_NET_SOCKET_NOERROR = _blcelc.CS_NET_SOCKET_NOERROR
CS_NET_SOCKET_CANNOT_CREATE = _blcelc.CS_NET_SOCKET_CANNOT_CREATE
CS_NET_SOCKET_UNSUPPORTED_SOCKET_TYPE = _blcelc.CS_NET_SOCKET_UNSUPPORTED_SOCKET_TYPE
CS_NET_SOCKET_NOTCONNECTED = _blcelc.CS_NET_SOCKET_NOTCONNECTED
CS_NET_SOCKET_CANNOT_SETBLOCK = _blcelc.CS_NET_SOCKET_CANNOT_SETBLOCK
CS_NET_SOCKET_CANNOT_SETREUSE = _blcelc.CS_NET_SOCKET_CANNOT_SETREUSE
CS_NET_SOCKET_CANNOT_BIND = _blcelc.CS_NET_SOCKET_CANNOT_BIND
CS_NET_SOCKET_CANNOT_LISTEN = _blcelc.CS_NET_SOCKET_CANNOT_LISTEN
CS_NET_SOCKET_CANNOT_SELECT = _blcelc.CS_NET_SOCKET_CANNOT_SELECT
CS_NET_SOCKET_CANNOT_IOCTL = _blcelc.CS_NET_SOCKET_CANNOT_IOCTL
CS_NET_SOCKET_CANNOT_ACCEPT = _blcelc.CS_NET_SOCKET_CANNOT_ACCEPT
CS_NET_SOCKET_WOULDBLOCK = _blcelc.CS_NET_SOCKET_WOULDBLOCK
CS_NET_SOCKET_NODATA = _blcelc.CS_NET_SOCKET_NODATA
CS_NET_SOCKET_CANNOT_RESOLVE = _blcelc.CS_NET_SOCKET_CANNOT_RESOLVE
CS_NET_SOCKET_CANNOT_CONNECT = _blcelc.CS_NET_SOCKET_CANNOT_CONNECT
CS_NET_SOCKET_BROADCAST_ERROR = _blcelc.CS_NET_SOCKET_BROADCAST_ERROR
CS_NET_ERR_NO_ERROR = _blcelc.CS_NET_ERR_NO_ERROR
CS_NET_ERR_CANNOT_RESOLVE_ADDRESS = _blcelc.CS_NET_ERR_CANNOT_RESOLVE_ADDRESS
CS_NET_ERR_CANNOT_CONNECT = _blcelc.CS_NET_ERR_CANNOT_CONNECT
CS_NET_ERR_CANNOT_SEND = _blcelc.CS_NET_ERR_CANNOT_SEND
CS_NET_ERR_INVALID_SOCKET = _blcelc.CS_NET_ERR_INVALID_SOCKET
CS_NET_ERR_CANNOT_BIND = _blcelc.CS_NET_ERR_CANNOT_BIND
CS_NET_ERR_CANNOT_LISTEN = _blcelc.CS_NET_ERR_CANNOT_LISTEN
CS_NET_ERR_CANNOT_CREATE = _blcelc.CS_NET_ERR_CANNOT_CREATE
CS_NET_ERR_CANNOT_ACCEPT = _blcelc.CS_NET_ERR_CANNOT_ACCEPT
CS_NET_ERR_CANNOT_SET_BLOCKING_MODE = _blcelc.CS_NET_ERR_CANNOT_SET_BLOCKING_MODE
CS_NET_ERR_CANNOT_RECEIVE = _blcelc.CS_NET_ERR_CANNOT_RECEIVE
CS_NET_ERR_CANNOT_PARSE_ADDRESS = _blcelc.CS_NET_ERR_CANNOT_PARSE_ADDRESS
CS_NET_ERR_CANNOT_GET_VERSION = _blcelc.CS_NET_ERR_CANNOT_GET_VERSION
CS_NET_ERR_WRONG_VERSION = _blcelc.CS_NET_ERR_WRONG_VERSION
CS_NET_ERR_CANNOT_CLEANUP = _blcelc.CS_NET_ERR_CANNOT_CLEANUP
CS_NET_ERR_NO_SUCH_OPTION = _blcelc.CS_NET_ERR_NO_SUCH_OPTION
CS_NET_ERR_CANNOT_SET_OPTION = _blcelc.CS_NET_ERR_CANNOT_SET_OPTION
class csNetworkDriverCapabilities(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, csNetworkDriverCapabilities, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, csNetworkDriverCapabilities, name)
    __swig_setmethods__["ConnectionReliable"] = _blcelc.csNetworkDriverCapabilities_ConnectionReliable_set
    __swig_getmethods__["ConnectionReliable"] = _blcelc.csNetworkDriverCapabilities_ConnectionReliable_get
    if _newclass:ConnectionReliable = property(_blcelc.csNetworkDriverCapabilities_ConnectionReliable_get,_blcelc.csNetworkDriverCapabilities_ConnectionReliable_set)
    __swig_setmethods__["ConnectionUnreliable"] = _blcelc.csNetworkDriverCapabilities_ConnectionUnreliable_set
    __swig_getmethods__["ConnectionUnreliable"] = _blcelc.csNetworkDriverCapabilities_ConnectionUnreliable_get
    if _newclass:ConnectionUnreliable = property(_blcelc.csNetworkDriverCapabilities_ConnectionUnreliable_get,_blcelc.csNetworkDriverCapabilities_ConnectionUnreliable_set)
    __swig_setmethods__["BehaviorBlocking"] = _blcelc.csNetworkDriverCapabilities_BehaviorBlocking_set
    __swig_getmethods__["BehaviorBlocking"] = _blcelc.csNetworkDriverCapabilities_BehaviorBlocking_get
    if _newclass:BehaviorBlocking = property(_blcelc.csNetworkDriverCapabilities_BehaviorBlocking_get,_blcelc.csNetworkDriverCapabilities_BehaviorBlocking_set)
    __swig_setmethods__["BehaviorNonBlocking"] = _blcelc.csNetworkDriverCapabilities_BehaviorNonBlocking_set
    __swig_getmethods__["BehaviorNonBlocking"] = _blcelc.csNetworkDriverCapabilities_BehaviorNonBlocking_get
    if _newclass:BehaviorNonBlocking = property(_blcelc.csNetworkDriverCapabilities_BehaviorNonBlocking_get,_blcelc.csNetworkDriverCapabilities_BehaviorNonBlocking_set)
    def __init__(self,*args):
        _swig_setattr(self, csNetworkDriverCapabilities, 'this', apply(_blcelc.new_csNetworkDriverCapabilities,args))
        _swig_setattr(self, csNetworkDriverCapabilities, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csNetworkDriverCapabilities):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csNetworkDriverCapabilities instance at %s>" % (self.this,)

class csNetworkDriverCapabilitiesPtr(csNetworkDriverCapabilities):
    def __init__(self,this):
        _swig_setattr(self, csNetworkDriverCapabilities, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csNetworkDriverCapabilities, 'thisown', 0)
        _swig_setattr(self, csNetworkDriverCapabilities,self.__class__,csNetworkDriverCapabilities)
_blcelc.csNetworkDriverCapabilities_swigregister(csNetworkDriverCapabilitiesPtr)

class iNetworkEndPoint(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNetworkEndPoint, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNetworkEndPoint, name)
    def Terminate(*args): return apply(_blcelc.iNetworkEndPoint_Terminate,args)
    def SetOption(*args): return apply(_blcelc.iNetworkEndPoint_SetOption,args)
    def GetLastError(*args): return apply(_blcelc.iNetworkEndPoint_GetLastError,args)
    def __del__(self, destroy= _blcelc.delete_iNetworkEndPoint):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iNetworkEndPoint_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iNetworkEndPoint_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNetworkEndPoint instance at %s>" % (self.this,)

class iNetworkEndPointPtr(iNetworkEndPoint):
    def __init__(self,this):
        _swig_setattr(self, iNetworkEndPoint, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNetworkEndPoint, 'thisown', 0)
        _swig_setattr(self, iNetworkEndPoint,self.__class__,iNetworkEndPoint)
_blcelc.iNetworkEndPoint_swigregister(iNetworkEndPointPtr)
iNetworkEndPoint_scfGetVersion = _blcelc.iNetworkEndPoint_scfGetVersion


class iNetworkConnection(iNetworkEndPoint):
    __swig_setmethods__ = {}
    for _s in [iNetworkEndPoint]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNetworkConnection, name, value)
    __swig_getmethods__ = {}
    for _s in [iNetworkEndPoint]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNetworkConnection, name)
    def Send(*args): return apply(_blcelc.iNetworkConnection_Send,args)
    def IsConnected(*args): return apply(_blcelc.iNetworkConnection_IsConnected,args)
    def Receive(*args): return apply(_blcelc.iNetworkConnection_Receive,args)
    def IsDataWaiting(*args): return apply(_blcelc.iNetworkConnection_IsDataWaiting,args)
    def __del__(self, destroy= _blcelc.delete_iNetworkConnection):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iNetworkConnection_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iNetworkConnection_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNetworkConnection instance at %s>" % (self.this,)

class iNetworkConnectionPtr(iNetworkConnection):
    def __init__(self,this):
        _swig_setattr(self, iNetworkConnection, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNetworkConnection, 'thisown', 0)
        _swig_setattr(self, iNetworkConnection,self.__class__,iNetworkConnection)
_blcelc.iNetworkConnection_swigregister(iNetworkConnectionPtr)
iNetworkConnection_scfGetVersion = _blcelc.iNetworkConnection_scfGetVersion


class iNetworkListener(iNetworkEndPoint):
    __swig_setmethods__ = {}
    for _s in [iNetworkEndPoint]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNetworkListener, name, value)
    __swig_getmethods__ = {}
    for _s in [iNetworkEndPoint]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNetworkListener, name)
    def Accept(*args): return apply(_blcelc.iNetworkListener_Accept,args)
    def __del__(self, destroy= _blcelc.delete_iNetworkListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iNetworkListener_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iNetworkListener_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNetworkListener instance at %s>" % (self.this,)

class iNetworkListenerPtr(iNetworkListener):
    def __init__(self,this):
        _swig_setattr(self, iNetworkListener, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNetworkListener, 'thisown', 0)
        _swig_setattr(self, iNetworkListener,self.__class__,iNetworkListener)
_blcelc.iNetworkListener_swigregister(iNetworkListenerPtr)
iNetworkListener_scfGetVersion = _blcelc.iNetworkListener_scfGetVersion


class iNetworkDriver(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNetworkDriver, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNetworkDriver, name)
    def NewConnection(*args): return apply(_blcelc.iNetworkDriver_NewConnection,args)
    def NewListener(*args): return apply(_blcelc.iNetworkDriver_NewListener,args)
    def GetCapabilities(*args): return apply(_blcelc.iNetworkDriver_GetCapabilities,args)
    def GetLastError(*args): return apply(_blcelc.iNetworkDriver_GetLastError,args)
    def __del__(self, destroy= _blcelc.delete_iNetworkDriver):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iNetworkDriver_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iNetworkDriver_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNetworkDriver instance at %s>" % (self.this,)

class iNetworkDriverPtr(iNetworkDriver):
    def __init__(self,this):
        _swig_setattr(self, iNetworkDriver, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNetworkDriver, 'thisown', 0)
        _swig_setattr(self, iNetworkDriver,self.__class__,iNetworkDriver)
_blcelc.iNetworkDriver_swigregister(iNetworkDriverPtr)
iNetworkDriver_scfGetVersion = _blcelc.iNetworkDriver_scfGetVersion


class iNetworkSocket2(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iNetworkSocket2, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iNetworkSocket2, name)
    def IsConnected(*args): return apply(_blcelc.iNetworkSocket2_IsConnected,args)
    def LastError(*args): return apply(_blcelc.iNetworkSocket2_LastError,args)
    def SetSocketBlock(*args): return apply(_blcelc.iNetworkSocket2_SetSocketBlock,args)
    def SetSocketReuse(*args): return apply(_blcelc.iNetworkSocket2_SetSocketReuse,args)
    def SetSocketBroadcast(*args): return apply(_blcelc.iNetworkSocket2_SetSocketBroadcast,args)
    def SetBroadcastOptions(*args): return apply(_blcelc.iNetworkSocket2_SetBroadcastOptions,args)
    def Connect(*args): return apply(_blcelc.iNetworkSocket2_Connect,args)
    def Send(*args): return apply(_blcelc.iNetworkSocket2_Send,args)
    def Recv(*args): return apply(_blcelc.iNetworkSocket2_Recv,args)
    def Close(*args): return apply(_blcelc.iNetworkSocket2_Close,args)
    def Disconnect(*args): return apply(_blcelc.iNetworkSocket2_Disconnect,args)
    def WaitForConnection(*args): return apply(_blcelc.iNetworkSocket2_WaitForConnection,args)
    def Accept(*args): return apply(_blcelc.iNetworkSocket2_Accept,args)
    def ReadLine(*args): return apply(_blcelc.iNetworkSocket2_ReadLine,args)
    def RemoteName(*args): return apply(_blcelc.iNetworkSocket2_RemoteName,args)
    def __del__(self, destroy= _blcelc.delete_iNetworkSocket2):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["scfGetVersion"] = lambda x: _blcelc.iNetworkSocket2_scfGetVersion
    if _newclass:scfGetVersion = staticmethod(_blcelc.iNetworkSocket2_scfGetVersion)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iNetworkSocket2 instance at %s>" % (self.this,)

class iNetworkSocket2Ptr(iNetworkSocket2):
    def __init__(self,this):
        _swig_setattr(self, iNetworkSocket2, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iNetworkSocket2, 'thisown', 0)
        _swig_setattr(self, iNetworkSocket2,self.__class__,iNetworkSocket2)
_blcelc.iNetworkSocket2_swigregister(iNetworkSocket2Ptr)
iNetworkSocket2_scfGetVersion = _blcelc.iNetworkSocket2_scfGetVersion


class csObject(iObject):
    __swig_setmethods__ = {}
    for _s in [iObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, csObject, name, value)
    __swig_getmethods__ = {}
    for _s in [iObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, csObject, name)
    def __init__(self,*args):
        _swig_setattr(self, csObject, 'this', apply(_blcelc.new_csObject,args))
        _swig_setattr(self, csObject, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    def SetName(*args): return apply(_blcelc.csObject_SetName,args)
    def GetName(*args): return apply(_blcelc.csObject_GetName,args)
    def GetID(*args): return apply(_blcelc.csObject_GetID,args)
    def SetObjectParent(*args): return apply(_blcelc.csObject_SetObjectParent,args)
    def GetObjectParent(*args): return apply(_blcelc.csObject_GetObjectParent,args)
    def ObjAdd(*args): return apply(_blcelc.csObject_ObjAdd,args)
    def ObjRemove(*args): return apply(_blcelc.csObject_ObjRemove,args)
    def ObjRemoveAll(*args): return apply(_blcelc.csObject_ObjRemoveAll,args)
    def ObjAddChildren(*args): return apply(_blcelc.csObject_ObjAddChildren,args)
    def GetChild(*args): return apply(_blcelc.csObject_GetChild,args)
    def GetIterator(*args): return apply(_blcelc.csObject_GetIterator,args)
    __swig_setmethods__["scfRefCount"] = _blcelc.csObject_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csObject_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csObject_scfRefCount_get,_blcelc.csObject_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csObject_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csObject_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csObject_scfWeakRefOwners_get,_blcelc.csObject_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return apply(_blcelc.csObject_scfRemoveRefOwners,args)
    __swig_setmethods__["scfParent"] = _blcelc.csObject_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csObject_scfParent_get
    if _newclass:scfParent = property(_blcelc.csObject_scfParent_get,_blcelc.csObject_scfParent_set)
    def IncRef(*args): return apply(_blcelc.csObject_IncRef,args)
    def DecRef(*args): return apply(_blcelc.csObject_DecRef,args)
    def GetRefCount(*args): return apply(_blcelc.csObject_GetRefCount,args)
    def AddRefOwner(*args): return apply(_blcelc.csObject_AddRefOwner,args)
    def RemoveRefOwner(*args): return apply(_blcelc.csObject_RemoveRefOwner,args)
    def QueryInterface(*args): return apply(_blcelc.csObject_QueryInterface,args)
    def ObjReleaseOld(*args): return apply(_blcelc.csObject_ObjReleaseOld,args)
    def __repr__(self):
        return "<C csObject instance at %s>" % (self.this,)

class csObjectPtr(csObject):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csView, 'this', apply(_blcelc.new_csView,args))
        _swig_setattr(self, csView, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csView):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetEngine(*args): return apply(_blcelc.csView_GetEngine,args)
    def SetEngine(*args): return apply(_blcelc.csView_SetEngine,args)
    def GetCamera(*args): return apply(_blcelc.csView_GetCamera,args)
    def SetCamera(*args): return apply(_blcelc.csView_SetCamera,args)
    def GetContext(*args): return apply(_blcelc.csView_GetContext,args)
    def SetContext(*args): return apply(_blcelc.csView_SetContext,args)
    def SetRectangle(*args): return apply(_blcelc.csView_SetRectangle,args)
    def ClearView(*args): return apply(_blcelc.csView_ClearView,args)
    def AddViewVertex(*args): return apply(_blcelc.csView_AddViewVertex,args)
    def RestrictClipperToScreen(*args): return apply(_blcelc.csView_RestrictClipperToScreen,args)
    def UpdateClipper(*args): return apply(_blcelc.csView_UpdateClipper,args)
    def GetClipper(*args): return apply(_blcelc.csView_GetClipper,args)
    def Draw(*args): return apply(_blcelc.csView_Draw,args)
    __swig_setmethods__["scfRefCount"] = _blcelc.csView_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc.csView_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc.csView_scfRefCount_get,_blcelc.csView_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc.csView_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc.csView_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc.csView_scfWeakRefOwners_get,_blcelc.csView_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return apply(_blcelc.csView_scfRemoveRefOwners,args)
    __swig_setmethods__["scfParent"] = _blcelc.csView_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc.csView_scfParent_get
    if _newclass:scfParent = property(_blcelc.csView_scfParent_get,_blcelc.csView_scfParent_set)
    def IncRef(*args): return apply(_blcelc.csView_IncRef,args)
    def DecRef(*args): return apply(_blcelc.csView_DecRef,args)
    def GetRefCount(*args): return apply(_blcelc.csView_GetRefCount,args)
    def AddRefOwner(*args): return apply(_blcelc.csView_AddRefOwner,args)
    def RemoveRefOwner(*args): return apply(_blcelc.csView_RemoveRefOwner,args)
    def QueryInterface(*args): return apply(_blcelc.csView_QueryInterface,args)
    def __repr__(self):
        return "<C csView instance at %s>" % (self.this,)

class csViewPtr(csView):
    def __init__(self,this):
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
    def __init__(self,*args):
        _swig_setattr(self, csColliderWrapper, 'this', apply(_blcelc.new_csColliderWrapper,args))
        _swig_setattr(self, csColliderWrapper, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csColliderWrapper):
        try:
            if self.thisown: destroy(self)
        except: pass
    def GetCollider(*args): return apply(_blcelc.csColliderWrapper_GetCollider,args)
    def GetCollideSystem(*args): return apply(_blcelc.csColliderWrapper_GetCollideSystem,args)
    def Collide(*args): return apply(_blcelc.csColliderWrapper_Collide,args)
    __swig_getmethods__["GetColliderWrapper"] = lambda x: _blcelc.csColliderWrapper_GetColliderWrapper
    if _newclass:GetColliderWrapper = staticmethod(_blcelc.csColliderWrapper_GetColliderWrapper)
    __swig_getmethods__["GetColliderWrapper"] = lambda x: _blcelc.csColliderWrapper_GetColliderWrapper
    if _newclass:GetColliderWrapper = staticmethod(_blcelc.csColliderWrapper_GetColliderWrapper)
    def IncRef(*args): return apply(_blcelc.csColliderWrapper_IncRef,args)
    def DecRef(*args): return apply(_blcelc.csColliderWrapper_DecRef,args)
    def GetRefCount(*args): return apply(_blcelc.csColliderWrapper_GetRefCount,args)
    def AddRefOwner(*args): return apply(_blcelc.csColliderWrapper_AddRefOwner,args)
    def RemoveRefOwner(*args): return apply(_blcelc.csColliderWrapper_RemoveRefOwner,args)
    def QueryInterface(*args): return apply(_blcelc.csColliderWrapper_QueryInterface,args)
    def __repr__(self):
        return "<C csColliderWrapper instance at %s>" % (self.this,)

class csColliderWrapperPtr(csColliderWrapper):
    def __init__(self,this):
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
    __swig_getmethods__["InitializeCollisionWrapper"] = lambda x: _blcelc.csColliderHelper_InitializeCollisionWrapper
    if _newclass:InitializeCollisionWrapper = staticmethod(_blcelc.csColliderHelper_InitializeCollisionWrapper)
    __swig_getmethods__["InitializeCollisionWrappers"] = lambda x: _blcelc.csColliderHelper_InitializeCollisionWrappers
    if _newclass:InitializeCollisionWrappers = staticmethod(_blcelc.csColliderHelper_InitializeCollisionWrappers)
    def __init__(self,*args):
        _swig_setattr(self, csColliderHelper, 'this', apply(_blcelc.new_csColliderHelper,args))
        _swig_setattr(self, csColliderHelper, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete_csColliderHelper):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C csColliderHelper instance at %s>" % (self.this,)

class csColliderHelperPtr(csColliderHelper):
    def __init__(self,this):
        _swig_setattr(self, csColliderHelper, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, csColliderHelper, 'thisown', 0)
        _swig_setattr(self, csColliderHelper,self.__class__,csColliderHelper)
_blcelc.csColliderHelper_swigregister(csColliderHelperPtr)
csColliderHelper_InitializeCollisionWrapper = _blcelc.csColliderHelper_InitializeCollisionWrapper

csColliderHelper_InitializeCollisionWrappers = _blcelc.csColliderHelper_InitializeCollisionWrappers


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

CS_IS_KEYBOARD_EVENT = _blcelc.CS_IS_KEYBOARD_EVENT

CS_IS_MOUSE_EVENT = _blcelc.CS_IS_MOUSE_EVENT

CS_IS_JOYSTICK_EVENT = _blcelc.CS_IS_JOYSTICK_EVENT

CS_IS_INPUT_EVENT = _blcelc.CS_IS_INPUT_EVENT

CS_IS_NETWORK_EVENT = _blcelc.CS_IS_NETWORK_EVENT

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
CSMASK_Network = (1 << csevNetwork)
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
    __swig_setmethods__["scfRefCount"] = _blcelc._csPyEventHandler_scfRefCount_set
    __swig_getmethods__["scfRefCount"] = _blcelc._csPyEventHandler_scfRefCount_get
    if _newclass:scfRefCount = property(_blcelc._csPyEventHandler_scfRefCount_get,_blcelc._csPyEventHandler_scfRefCount_set)
    __swig_setmethods__["scfWeakRefOwners"] = _blcelc._csPyEventHandler_scfWeakRefOwners_set
    __swig_getmethods__["scfWeakRefOwners"] = _blcelc._csPyEventHandler_scfWeakRefOwners_get
    if _newclass:scfWeakRefOwners = property(_blcelc._csPyEventHandler_scfWeakRefOwners_get,_blcelc._csPyEventHandler_scfWeakRefOwners_set)
    def scfRemoveRefOwners(*args): return apply(_blcelc._csPyEventHandler_scfRemoveRefOwners,args)
    __swig_setmethods__["scfParent"] = _blcelc._csPyEventHandler_scfParent_set
    __swig_getmethods__["scfParent"] = _blcelc._csPyEventHandler_scfParent_get
    if _newclass:scfParent = property(_blcelc._csPyEventHandler_scfParent_get,_blcelc._csPyEventHandler_scfParent_set)
    def IncRef(*args): return apply(_blcelc._csPyEventHandler_IncRef,args)
    def DecRef(*args): return apply(_blcelc._csPyEventHandler_DecRef,args)
    def GetRefCount(*args): return apply(_blcelc._csPyEventHandler_GetRefCount,args)
    def AddRefOwner(*args): return apply(_blcelc._csPyEventHandler_AddRefOwner,args)
    def RemoveRefOwner(*args): return apply(_blcelc._csPyEventHandler_RemoveRefOwner,args)
    def QueryInterface(*args): return apply(_blcelc._csPyEventHandler_QueryInterface,args)
    def __init__(self,*args):
        _swig_setattr(self, _csPyEventHandler, 'this', apply(_blcelc.new__csPyEventHandler,args))
        _swig_setattr(self, _csPyEventHandler, 'thisown', 1)
    def __del__(self, destroy= _blcelc.delete__csPyEventHandler):
        try:
            if self.thisown: destroy(self)
        except: pass
    def HandleEvent(*args): return apply(_blcelc._csPyEventHandler_HandleEvent,args)
    def __repr__(self):
        return "<C _csPyEventHandler instance at %s>" % (self.this,)

class _csPyEventHandlerPtr(_csPyEventHandler):
    def __init__(self,this):
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
def _csInitializer_RequestPlugins (reg, plugins):
	"""Replacement of C++ version with variable argument list."""
	def _get_tuple (x):
		if callable(x):
			return tuple(x())
		else:
			return tuple(x)
	ok = 1
	for plugName, intName, scfId, version in map(
			lambda x: _get_tuple(x), plugins):
		res = csInitializer._RequestPlugin(
			reg, plugName, intName, scfId, version
		)
		if not res:
			ok = 0
	return ok
csInitializer.RequestPlugins = staticmethod(_csInitializer_RequestPlugins)
csInitializer.SetupEventHandler = staticmethod(_csInitializer_SetupEventHandler)


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
	return (
		name, intf.__name__, cvar.iSCF_SCF.GetInterfaceID(intf.__name__),
		eval('%s_scfGetVersion()' % intf.__name__, locals(), globals())
	)
def CS_REQUEST_VFS ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.kernel.vfs", iVFS
	)
def CS_REQUEST_FONTSERVER ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.font.server.default", iFontServer
	)
def CS_REQUEST_IMAGELOADER ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.graphic.image.io.multiplex", iImageIO
	)
def CS_REQUEST_NULL3D ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.graphics3d.null", iGraphics3D
	)
def CS_REQUEST_SOFTWARE3D ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.graphics3d.software", iGraphics3D
	)
def CS_REQUEST_OPENGL3D ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.graphics3d.opengl", iGraphics3D
	)
def CS_REQUEST_ENGINE ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.engine.3d", iEngine
	)
def CS_REQUEST_LEVELLOADER ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.level.loader", iLoader
	)
def CS_REQUEST_LEVELSAVER ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.level.saver", iSaver
	)
def CS_REQUEST_REPORTER ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.utilities.reporter", iReporter
	)
def CS_REQUEST_REPORTERLISTENER ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.utilities.stdrep", iStandardReporterListener
	)
def CS_REQUEST_CONSOLEOUT ():
	return CS_REQUEST_PLUGIN(
		"crystalspace.console.output.simple", iConsoleOutput
	)


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


class iCelPlLayer(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iCelPlLayer, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iCelPlLayer, name)
    def FindEntity(*args): return apply(_blcelc.iCelPlLayer_FindEntity,args)
    def RemoveEntity(*args): return apply(_blcelc.iCelPlLayer_RemoveEntity,args)
    def LoadPropertyClassFactory(*args): return apply(_blcelc.iCelPlLayer_LoadPropertyClassFactory,args)
    def CreatePropertyClass(*args): return apply(_blcelc.iCelPlLayer_CreatePropertyClass,args)
    def FetchString(*args): return apply(_blcelc.iCelPlLayer_FetchString,args)
    def GetBehaviourLayerCount(*args): return apply(_blcelc.iCelPlLayer_GetBehaviourLayerCount,args)
    def GetBehaviourLayer(*args): return apply(_blcelc.iCelPlLayer_GetBehaviourLayer,args)
    def FindBehaviourLayer(*args): return apply(_blcelc.iCelPlLayer_FindBehaviourLayer,args)
    def __del__(self, destroy= _blcelc.delete_iCelPlLayer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPlLayer instance at %s>" % (self.this,)

class iCelPlLayerPtr(iCelPlLayer):
    def __init__(self,this):
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
    def GetName(*args): return apply(_blcelc.iCelEntity_GetName,args)
    def SetName(*args): return apply(_blcelc.iCelEntity_SetName,args)
    def SetBehaviour(*args): return apply(_blcelc.iCelEntity_SetBehaviour,args)
    def GetBehaviour(*args): return apply(_blcelc.iCelEntity_GetBehaviour,args)
    def GetID(*args): return apply(_blcelc.iCelEntity_GetID,args)
    def GetPropertyClassList(*args): return apply(_blcelc.iCelEntity_GetPropertyClassList,args)
    def CreateBehaviour(*args): return apply(_blcelc.iCelEntity_CreateBehaviour,args)
    def __del__(self, destroy= _blcelc.delete_iCelEntity):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelEntity instance at %s>" % (self.this,)

class iCelEntityPtr(iCelEntity):
    def __init__(self,this):
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
    def GetCount(*args): return apply(_blcelc.iCelEntityList_GetCount,args)
    def Get(*args): return apply(_blcelc.iCelEntityList_Get,args)
    def Add(*args): return apply(_blcelc.iCelEntityList_Add,args)
    def Remove(*args): return apply(_blcelc.iCelEntityList_Remove,args)
    def RemoveAll(*args): return apply(_blcelc.iCelEntityList_RemoveAll,args)
    def Find(*args): return apply(_blcelc.iCelEntityList_Find,args)
    def FindByName(*args): return apply(_blcelc.iCelEntityList_FindByName,args)
    def __del__(self, destroy= _blcelc.delete_iCelEntityList):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelEntityList instance at %s>" % (self.this,)

class iCelEntityListPtr(iCelEntityList):
    def __init__(self,this):
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
    def GetName(*args): return apply(_blcelc.iCelBlLayer_GetName,args)
    def CreateBehaviour(*args): return apply(_blcelc.iCelBlLayer_CreateBehaviour,args)
    def __del__(self, destroy= _blcelc.delete_iCelBlLayer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelBlLayer instance at %s>" % (self.this,)

class iCelBlLayerPtr(iCelBlLayer):
    def __init__(self,this):
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
    def GetParameterCount(*args): return apply(_blcelc.iCelParameterBlock_GetParameterCount,args)
    def GetParameter(*args): return apply(_blcelc.iCelParameterBlock_GetParameter,args)
    def __del__(self, destroy= _blcelc.delete_iCelParameterBlock):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelParameterBlock instance at %s>" % (self.this,)

class iCelParameterBlockPtr(iCelParameterBlock):
    def __init__(self,this):
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
    def GetName(*args): return apply(_blcelc.iCelBehaviour_GetName,args)
    def GetBehaviourLayer(*args): return apply(_blcelc.iCelBehaviour_GetBehaviourLayer,args)
    def SendMessage(*args): return apply(_blcelc.iCelBehaviour_SendMessage,args)
    def GetPythonObject(*args): return apply(_blcelc.iCelBehaviour_GetPythonObject,args)
    def __del__(self, destroy= _blcelc.delete_iCelBehaviour):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelBehaviour instance at %s>" % (self.this,)

class iCelBehaviourPtr(iCelBehaviour):
    def __init__(self,this):
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
    def GetName(*args): return apply(_blcelc.iCelPropertyClass_GetName,args)
    def GetEntity(*args): return apply(_blcelc.iCelPropertyClass_GetEntity,args)
    def SetEntity(*args): return apply(_blcelc.iCelPropertyClass_SetEntity,args)
    def GetPropertyOrActionType(*args): return apply(_blcelc.iCelPropertyClass_GetPropertyOrActionType,args)
    def IsPropertyReadOnly(*args): return apply(_blcelc.iCelPropertyClass_IsPropertyReadOnly,args)
    def GetPropertyLong(*args): return apply(_blcelc.iCelPropertyClass_GetPropertyLong,args)
    def GetPropertyFloat(*args): return apply(_blcelc.iCelPropertyClass_GetPropertyFloat,args)
    def GetPropertyBool(*args): return apply(_blcelc.iCelPropertyClass_GetPropertyBool,args)
    def GetPropertyString(*args): return apply(_blcelc.iCelPropertyClass_GetPropertyString,args)
    def GetPropertyVector(*args): return apply(_blcelc.iCelPropertyClass_GetPropertyVector,args)
    def GetPropertyAndActionCount(*args): return apply(_blcelc.iCelPropertyClass_GetPropertyAndActionCount,args)
    def GetPropertyOrActionID(*args): return apply(_blcelc.iCelPropertyClass_GetPropertyOrActionID,args)
    def GetPropertyOrActionDescription(*args): return apply(_blcelc.iCelPropertyClass_GetPropertyOrActionDescription,args)
    def PerformAction(*args): return apply(_blcelc.iCelPropertyClass_PerformAction,args)
    def SetPropertyLong(*args): return apply(_blcelc.iCelPropertyClass_SetPropertyLong,args)
    def SetPropertyFloat(*args): return apply(_blcelc.iCelPropertyClass_SetPropertyFloat,args)
    def SetPropertyBool(*args): return apply(_blcelc.iCelPropertyClass_SetPropertyBool,args)
    def SetPropertyString(*args): return apply(_blcelc.iCelPropertyClass_SetPropertyString,args)
    def SetPropertyVector3(*args): return apply(_blcelc.iCelPropertyClass_SetPropertyVector3,args)
    def __del__(self, destroy= _blcelc.delete_iCelPropertyClass):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPropertyClass instance at %s>" % (self.this,)

class iCelPropertyClassPtr(iCelPropertyClass):
    def __init__(self,this):
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
    def GetCount(*args): return apply(_blcelc.iCelPropertyClassList_GetCount,args)
    def Get(*args): return apply(_blcelc.iCelPropertyClassList_Get,args)
    def Find(*args): return apply(_blcelc.iCelPropertyClassList_Find,args)
    def FindByName(*args): return apply(_blcelc.iCelPropertyClassList_FindByName,args)
    def FindByInterface(*args): return apply(_blcelc.iCelPropertyClassList_FindByInterface,args)
    def __del__(self, destroy= _blcelc.delete_iCelPropertyClassList):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPropertyClassList instance at %s>" % (self.this,)

class iCelPropertyClassListPtr(iCelPropertyClassList):
    def __init__(self,this):
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
    def GetDynamicSystem(*args): return apply(_blcelc.iPcDynamicSystem_GetDynamicSystem,args)
    def AddForceDuration(*args): return apply(_blcelc.iPcDynamicSystem_AddForceDuration,args)
    def AddForceFrame(*args): return apply(_blcelc.iPcDynamicSystem_AddForceFrame,args)
    def ClearForces(*args): return apply(_blcelc.iPcDynamicSystem_ClearForces,args)
    def ClearAllForces(*args): return apply(_blcelc.iPcDynamicSystem_ClearAllForces,args)
    def __del__(self, destroy= _blcelc.delete_iPcDynamicSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcDynamicSystem instance at %s>" % (self.this,)

class iPcDynamicSystemPtr(iPcDynamicSystem):
    def __init__(self,this):
        _swig_setattr(self, iPcDynamicSystem, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcDynamicSystem, 'thisown', 0)
        _swig_setattr(self, iPcDynamicSystem,self.__class__,iPcDynamicSystem)
_blcelc.iPcDynamicSystem_swigregister(iPcDynamicSystemPtr)

celCreateDynamicSystem = _blcelc.celCreateDynamicSystem

celGetDynamicSystem = _blcelc.celGetDynamicSystem

scfQuery_iPcDynamicSystem = _blcelc.scfQuery_iPcDynamicSystem

celQueryPC_iPcDynamicSystem = _blcelc.celQueryPC_iPcDynamicSystem

class iPcDynamicBody(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcDynamicBody, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcDynamicBody, name)
    def SetMesh(*args): return apply(_blcelc.iPcDynamicBody_SetMesh,args)
    def GetMesh(*args): return apply(_blcelc.iPcDynamicBody_GetMesh,args)
    def SetDynamicSystem(*args): return apply(_blcelc.iPcDynamicBody_SetDynamicSystem,args)
    def GetDynamicSystem(*args): return apply(_blcelc.iPcDynamicBody_GetDynamicSystem,args)
    def GetBody(*args): return apply(_blcelc.iPcDynamicBody_GetBody,args)
    def SetParameters(*args): return apply(_blcelc.iPcDynamicBody_SetParameters,args)
    def MakeStatic(*args): return apply(_blcelc.iPcDynamicBody_MakeStatic,args)
    def IsStatic(*args): return apply(_blcelc.iPcDynamicBody_IsStatic,args)
    def AttachColliderSphere(*args): return apply(_blcelc.iPcDynamicBody_AttachColliderSphere,args)
    def AttachColliderCylinder(*args): return apply(_blcelc.iPcDynamicBody_AttachColliderCylinder,args)
    def AttachColliderBox(*args): return apply(_blcelc.iPcDynamicBody_AttachColliderBox,args)
    def AttachColliderPlane(*args): return apply(_blcelc.iPcDynamicBody_AttachColliderPlane,args)
    def AttachColliderMesh(*args): return apply(_blcelc.iPcDynamicBody_AttachColliderMesh,args)
    def AddForceOnce(*args): return apply(_blcelc.iPcDynamicBody_AddForceOnce,args)
    def AddForceDuration(*args): return apply(_blcelc.iPcDynamicBody_AddForceDuration,args)
    def AddForceFrame(*args): return apply(_blcelc.iPcDynamicBody_AddForceFrame,args)
    def ClearForces(*args): return apply(_blcelc.iPcDynamicBody_ClearForces,args)
    def __del__(self, destroy= _blcelc.delete_iPcDynamicBody):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcDynamicBody instance at %s>" % (self.this,)

class iPcDynamicBodyPtr(iPcDynamicBody):
    def __init__(self,this):
        _swig_setattr(self, iPcDynamicBody, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcDynamicBody, 'thisown', 0)
        _swig_setattr(self, iPcDynamicBody,self.__class__,iPcDynamicBody)
_blcelc.iPcDynamicBody_swigregister(iPcDynamicBodyPtr)

celCreateDynamicBody = _blcelc.celCreateDynamicBody

celGetDynamicBody = _blcelc.celGetDynamicBody

scfQuery_iPcDynamicBody = _blcelc.scfQuery_iPcDynamicBody

celQueryPC_iPcDynamicBody = _blcelc.celQueryPC_iPcDynamicBody

class iBillboard(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iBillboard, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iBillboard, name)
    def GetName(*args): return apply(_blcelc.iBillboard_GetName,args)
    def GetFlags(*args): return apply(_blcelc.iBillboard_GetFlags,args)
    def SetMaterialName(*args): return apply(_blcelc.iBillboard_SetMaterialName,args)
    def GetMaterialName(*args): return apply(_blcelc.iBillboard_GetMaterialName,args)
    def SetSize(*args): return apply(_blcelc.iBillboard_SetSize,args)
    def GetImageSize(*args): return apply(_blcelc.iBillboard_GetImageSize,args)
    def GetSize(*args): return apply(_blcelc.iBillboard_GetSize,args)
    def SetPosition(*args): return apply(_blcelc.iBillboard_SetPosition,args)
    def GetPosition(*args): return apply(_blcelc.iBillboard_GetPosition,args)
    def Move(*args): return apply(_blcelc.iBillboard_Move,args)
    def __del__(self, destroy= _blcelc.delete_iBillboard):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBillboard instance at %s>" % (self.this,)

class iBillboardPtr(iBillboard):
    def __init__(self,this):
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
    def CreateBillboard(*args): return apply(_blcelc.iBillboardManager_CreateBillboard,args)
    def FindBillboard(*args): return apply(_blcelc.iBillboardManager_FindBillboard,args)
    def RemoveBillboard(*args): return apply(_blcelc.iBillboardManager_RemoveBillboard,args)
    def GetBillboardCount(*args): return apply(_blcelc.iBillboardManager_GetBillboardCount,args)
    def GetBillboard(*args): return apply(_blcelc.iBillboardManager_GetBillboard,args)
    def RemoveAll(*args): return apply(_blcelc.iBillboardManager_RemoveAll,args)
    def SetFlags(*args): return apply(_blcelc.iBillboardManager_SetFlags,args)
    def __del__(self, destroy= _blcelc.delete_iBillboardManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iBillboardManager instance at %s>" % (self.this,)

class iBillboardManagerPtr(iBillboardManager):
    def __init__(self,this):
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
    def SetBillboardName(*args): return apply(_blcelc.iPcBillboard_SetBillboardName,args)
    def GetBillboardName(*args): return apply(_blcelc.iPcBillboard_GetBillboardName,args)
    def GetBillboard(*args): return apply(_blcelc.iPcBillboard_GetBillboard,args)
    def __del__(self, destroy= _blcelc.delete_iPcBillboard):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcBillboard instance at %s>" % (self.this,)

class iPcBillboardPtr(iPcBillboard):
    def __init__(self,this):
        _swig_setattr(self, iPcBillboard, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcBillboard, 'thisown', 0)
        _swig_setattr(self, iPcBillboard,self.__class__,iPcBillboard)
_blcelc.iPcBillboard_swigregister(iPcBillboardPtr)

celCreateBillboard = _blcelc.celCreateBillboard

celGetBillboard = _blcelc.celGetBillboard

scfQuery_iPcBillboard = _blcelc.scfQuery_iPcBillboard

celQueryPC_iPcBillboard = _blcelc.celQueryPC_iPcBillboard

class iPcRegion(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcRegion, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcRegion, name)
    def SetWorldFile(*args): return apply(_blcelc.iPcRegion_SetWorldFile,args)
    def GetWorldDir(*args): return apply(_blcelc.iPcRegion_GetWorldDir,args)
    def GetWorldFile(*args): return apply(_blcelc.iPcRegion_GetWorldFile,args)
    def CreateEmptySector(*args): return apply(_blcelc.iPcRegion_CreateEmptySector,args)
    def SetRegionName(*args): return apply(_blcelc.iPcRegion_SetRegionName,args)
    def GetRegionName(*args): return apply(_blcelc.iPcRegion_GetRegionName,args)
    def Load(*args): return apply(_blcelc.iPcRegion_Load,args)
    def Unload(*args): return apply(_blcelc.iPcRegion_Unload,args)
    def FindSector(*args): return apply(_blcelc.iPcRegion_FindSector,args)
    def GetStartSector(*args): return apply(_blcelc.iPcRegion_GetStartSector,args)
    def GetStartPosition(*args): return apply(_blcelc.iPcRegion_GetStartPosition,args)
    def PointCamera(*args): return apply(_blcelc.iPcRegion_PointCamera,args)
    def LoadWorld(*args): return apply(_blcelc.iPcRegion_LoadWorld,args)
    def __del__(self, destroy= _blcelc.delete_iPcRegion):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcRegion instance at %s>" % (self.this,)

class iPcRegionPtr(iPcRegion):
    def __init__(self,this):
        _swig_setattr(self, iPcRegion, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcRegion, 'thisown', 0)
        _swig_setattr(self, iPcRegion,self.__class__,iPcRegion)
_blcelc.iPcRegion_swigregister(iPcRegionPtr)

celCreateRegion = _blcelc.celCreateRegion

celGetRegion = _blcelc.celGetRegion

scfQuery_iPcRegion = _blcelc.scfQuery_iPcRegion

celQueryPC_iPcRegion = _blcelc.celQueryPC_iPcRegion

class iPcCommandInput(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCommandInput, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCommandInput, name)
    def Activate(*args): return apply(_blcelc.iPcCommandInput_Activate,args)
    def LoadConfig(*args): return apply(_blcelc.iPcCommandInput_LoadConfig,args)
    def Bind(*args): return apply(_blcelc.iPcCommandInput_Bind,args)
    def GetBind(*args): return apply(_blcelc.iPcCommandInput_GetBind,args)
    def RemoveBind(*args): return apply(_blcelc.iPcCommandInput_RemoveBind,args)
    def __del__(self, destroy= _blcelc.delete_iPcCommandInput):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCommandInput instance at %s>" % (self.this,)

class iPcCommandInputPtr(iPcCommandInput):
    def __init__(self,this):
        _swig_setattr(self, iPcCommandInput, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcCommandInput, 'thisown', 0)
        _swig_setattr(self, iPcCommandInput,self.__class__,iPcCommandInput)
_blcelc.iPcCommandInput_swigregister(iPcCommandInputPtr)

celCreateCommandInput = _blcelc.celCreateCommandInput

celGetCommandInput = _blcelc.celGetCommandInput

class iPcLinearMovement(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcLinearMovement, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcLinearMovement, name)
    def SetRotation(*args): return apply(_blcelc.iPcLinearMovement_SetRotation,args)
    def SetSpeed(*args): return apply(_blcelc.iPcLinearMovement_SetSpeed,args)
    def SetCameraPitchSpeed(*args): return apply(_blcelc.iPcLinearMovement_SetCameraPitchSpeed,args)
    def SetVelocity(*args): return apply(_blcelc.iPcLinearMovement_SetVelocity,args)
    def GetVelocity(*args): return apply(_blcelc.iPcLinearMovement_GetVelocity,args)
    def InitCD(*args): return apply(_blcelc.iPcLinearMovement_InitCD,args)
    def GetDRData(*args): return apply(_blcelc.iPcLinearMovement_GetDRData,args)
    def SetDRData(*args): return apply(_blcelc.iPcLinearMovement_SetDRData,args)
    def NeedDRData(*args): return apply(_blcelc.iPcLinearMovement_NeedDRData,args)
    def SetPosition(*args): return apply(_blcelc.iPcLinearMovement_SetPosition,args)
    def GetLastPosition(*args): return apply(_blcelc.iPcLinearMovement_GetLastPosition,args)
    def GetSector(*args): return apply(_blcelc.iPcLinearMovement_GetSector,args)
    def SetReady(*args): return apply(_blcelc.iPcLinearMovement_SetReady,args)
    def IsReady(*args): return apply(_blcelc.iPcLinearMovement_IsReady,args)
    def IsOnGround(*args): return apply(_blcelc.iPcLinearMovement_IsOnGround,args)
    def ExtrapolatePosition(*args): return apply(_blcelc.iPcLinearMovement_ExtrapolatePosition,args)
    def __del__(self, destroy= _blcelc.delete_iPcLinearMovement):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcLinearMovement instance at %s>" % (self.this,)

class iPcLinearMovementPtr(iPcLinearMovement):
    def __init__(self,this):
        _swig_setattr(self, iPcLinearMovement, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcLinearMovement, 'thisown', 0)
        _swig_setattr(self, iPcLinearMovement,self.__class__,iPcLinearMovement)
_blcelc.iPcLinearMovement_swigregister(iPcLinearMovementPtr)

celCreateLinearMovement = _blcelc.celCreateLinearMovement

celGetLinearMovement = _blcelc.celGetLinearMovement

class iPcCamera(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcCamera, name)
    freelook = _blcelc.iPcCamera_freelook
    follow = _blcelc.iPcCamera_follow
    rotational = _blcelc.iPcCamera_rotational
    firstperson = _blcelc.iPcCamera_firstperson
    def SetRegion(*args): return apply(_blcelc.iPcCamera_SetRegion,args)
    def SetMode(*args): return apply(_blcelc.iPcCamera_SetMode,args)
    def GetMode(*args): return apply(_blcelc.iPcCamera_GetMode,args)
    def SetFollowPos(*args): return apply(_blcelc.iPcCamera_SetFollowPos,args)
    def GetFollowPos(*args): return apply(_blcelc.iPcCamera_GetFollowPos,args)
    def SetPitch(*args): return apply(_blcelc.iPcCamera_SetPitch,args)
    def GetPitch(*args): return apply(_blcelc.iPcCamera_GetPitch,args)
    def SetRoll(*args): return apply(_blcelc.iPcCamera_SetRoll,args)
    def GetRoll(*args): return apply(_blcelc.iPcCamera_GetRoll,args)
    def SetRectangle(*args): return apply(_blcelc.iPcCamera_SetRectangle,args)
    def GetCamera(*args): return apply(_blcelc.iPcCamera_GetCamera,args)
    def GetView(*args): return apply(_blcelc.iPcCamera_GetView,args)
    def SetClearZBuffer(*args): return apply(_blcelc.iPcCamera_SetClearZBuffer,args)
    def GetClearZBuffer(*args): return apply(_blcelc.iPcCamera_GetClearZBuffer,args)
    def SetClearScreen(*args): return apply(_blcelc.iPcCamera_SetClearScreen,args)
    def GetClearScreen(*args): return apply(_blcelc.iPcCamera_GetClearScreen,args)
    def __del__(self, destroy= _blcelc.delete_iPcCamera):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCamera instance at %s>" % (self.this,)

class iPcCameraPtr(iPcCamera):
    def __init__(self,this):
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
    def SetCamera(*args): return apply(_blcelc.iPcMeshSelect_SetCamera,args)
    def SetMouseButtons(*args): return apply(_blcelc.iPcMeshSelect_SetMouseButtons,args)
    def GetMouseButtons(*args): return apply(_blcelc.iPcMeshSelect_GetMouseButtons,args)
    def SetGlobalSelection(*args): return apply(_blcelc.iPcMeshSelect_SetGlobalSelection,args)
    def HasGlobalSelection(*args): return apply(_blcelc.iPcMeshSelect_HasGlobalSelection,args)
    def SetFollowMode(*args): return apply(_blcelc.iPcMeshSelect_SetFollowMode,args)
    def HasFollowMode(*args): return apply(_blcelc.iPcMeshSelect_HasFollowMode,args)
    def SetFollowAlwaysMode(*args): return apply(_blcelc.iPcMeshSelect_SetFollowAlwaysMode,args)
    def HasFollowAlwaysMode(*args): return apply(_blcelc.iPcMeshSelect_HasFollowAlwaysMode,args)
    def SetDragMode(*args): return apply(_blcelc.iPcMeshSelect_SetDragMode,args)
    def HasDragMode(*args): return apply(_blcelc.iPcMeshSelect_HasDragMode,args)
    def SetDragPlaneNormal(*args): return apply(_blcelc.iPcMeshSelect_SetDragPlaneNormal,args)
    def GetDragPlaneNormal(*args): return apply(_blcelc.iPcMeshSelect_GetDragPlaneNormal,args)
    def SetSendmoveEvent(*args): return apply(_blcelc.iPcMeshSelect_SetSendmoveEvent,args)
    def HasSendmoveEvent(*args): return apply(_blcelc.iPcMeshSelect_HasSendmoveEvent,args)
    def SetSendupEvent(*args): return apply(_blcelc.iPcMeshSelect_SetSendupEvent,args)
    def HasSendupEvent(*args): return apply(_blcelc.iPcMeshSelect_HasSendupEvent,args)
    def SetSenddownEvent(*args): return apply(_blcelc.iPcMeshSelect_SetSenddownEvent,args)
    def HasSenddownEvent(*args): return apply(_blcelc.iPcMeshSelect_HasSenddownEvent,args)
    def __del__(self, destroy= _blcelc.delete_iPcMeshSelect):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMeshSelect instance at %s>" % (self.this,)

class iPcMeshSelectPtr(iPcMeshSelect):
    def __init__(self,this):
        _swig_setattr(self, iPcMeshSelect, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcMeshSelect, 'thisown', 0)
        _swig_setattr(self, iPcMeshSelect,self.__class__,iPcMeshSelect)
_blcelc.iPcMeshSelect_swigregister(iPcMeshSelectPtr)

celCreateMeshSelect = _blcelc.celCreateMeshSelect

celGetMeshSelect = _blcelc.celGetMeshSelect

class iPcMesh(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcMesh, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcMesh, name)
    def LoadMesh(*args): return apply(_blcelc.iPcMesh_LoadMesh,args)
    def SetMesh(*args): return apply(_blcelc.iPcMesh_SetMesh,args)
    def CreateEmptyThing(*args): return apply(_blcelc.iPcMesh_CreateEmptyThing,args)
    def GetMesh(*args): return apply(_blcelc.iPcMesh_GetMesh,args)
    def MoveMesh(*args): return apply(_blcelc.iPcMesh_MoveMesh,args)
    def SetAction(*args): return apply(_blcelc.iPcMesh_SetAction,args)
    def GetAction(*args): return apply(_blcelc.iPcMesh_GetAction,args)
    def Hide(*args): return apply(_blcelc.iPcMesh_Hide,args)
    def Show(*args): return apply(_blcelc.iPcMesh_Show,args)
    def IsVisible(*args): return apply(_blcelc.iPcMesh_IsVisible,args)
    def __del__(self, destroy= _blcelc.delete_iPcMesh):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMesh instance at %s>" % (self.this,)

class iPcMeshPtr(iPcMesh):
    def __init__(self,this):
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
    def WakeUp(*args): return apply(_blcelc.iPcTimer_WakeUp,args)
    def Clear(*args): return apply(_blcelc.iPcTimer_Clear,args)
    def __del__(self, destroy= _blcelc.delete_iPcTimer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcTimer instance at %s>" % (self.this,)

class iPcTimerPtr(iPcTimer):
    def __init__(self,this):
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
    def SetMesh(*args): return apply(_blcelc.iPcSolid_SetMesh,args)
    def GetMesh(*args): return apply(_blcelc.iPcSolid_GetMesh,args)
    def GetCollider(*args): return apply(_blcelc.iPcSolid_GetCollider,args)
    def __del__(self, destroy= _blcelc.delete_iPcSolid):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcSolid instance at %s>" % (self.this,)

class iPcSolidPtr(iPcSolid):
    def __init__(self,this):
        _swig_setattr(self, iPcSolid, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcSolid, 'thisown', 0)
        _swig_setattr(self, iPcSolid,self.__class__,iPcSolid)
_blcelc.iPcSolid_swigregister(iPcSolidPtr)

celCreateSolid = _blcelc.celCreateSolid

celGetSolid = _blcelc.celGetSolid

class iPcGravity(iBase):
    __swig_setmethods__ = {}
    for _s in [iBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, iPcGravity, name, value)
    __swig_getmethods__ = {}
    for _s in [iBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, iPcGravity, name)
    def CreateGravityColliderFromMesh(*args): return apply(_blcelc.iPcGravity_CreateGravityColliderFromMesh,args)
    def CreateGravityCollider(*args): return apply(_blcelc.iPcGravity_CreateGravityCollider,args)
    def GetGravityCollider(*args): return apply(_blcelc.iPcGravity_GetGravityCollider,args)
    def SetMovable(*args): return apply(_blcelc.iPcGravity_SetMovable,args)
    def GetMovable(*args): return apply(_blcelc.iPcGravity_GetMovable,args)
    def SetSolid(*args): return apply(_blcelc.iPcGravity_SetSolid,args)
    def GetSolid(*args): return apply(_blcelc.iPcGravity_GetSolid,args)
    def SetWeight(*args): return apply(_blcelc.iPcGravity_SetWeight,args)
    def GetWeight(*args): return apply(_blcelc.iPcGravity_GetWeight,args)
    def ClearForces(*args): return apply(_blcelc.iPcGravity_ClearForces,args)
    def ClearPermanentForces(*args): return apply(_blcelc.iPcGravity_ClearPermanentForces,args)
    def ResetSpeed(*args): return apply(_blcelc.iPcGravity_ResetSpeed,args)
    def ApplyForce(*args): return apply(_blcelc.iPcGravity_ApplyForce,args)
    def ApplyPermanentForce(*args): return apply(_blcelc.iPcGravity_ApplyPermanentForce,args)
    def IsResting(*args): return apply(_blcelc.iPcGravity_IsResting,args)
    def SetActive(*args): return apply(_blcelc.iPcGravity_SetActive,args)
    def IsActive(*args): return apply(_blcelc.iPcGravity_IsActive,args)
    def __del__(self, destroy= _blcelc.delete_iPcGravity):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcGravity instance at %s>" % (self.this,)

class iPcGravityPtr(iPcGravity):
    def __init__(self,this):
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
    def SetMesh(*args): return apply(_blcelc.iPcMovable_SetMesh,args)
    def GetMesh(*args): return apply(_blcelc.iPcMovable_GetMesh,args)
    def SetPos(*args): return apply(_blcelc.iPcMovable_SetPos,args)
    def Move(*args): return apply(_blcelc.iPcMovable_Move,args)
    def AddConstraint(*args): return apply(_blcelc.iPcMovable_AddConstraint,args)
    def RemoveConstraint(*args): return apply(_blcelc.iPcMovable_RemoveConstraint,args)
    def RemoveAllConstraints(*args): return apply(_blcelc.iPcMovable_RemoveAllConstraints,args)
    def __del__(self, destroy= _blcelc.delete_iPcMovable):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMovable instance at %s>" % (self.this,)

class iPcMovablePtr(iPcMovable):
    def __init__(self,this):
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
    def AddEntity(*args): return apply(_blcelc.iPcInventory_AddEntity,args)
    def RemoveEntity(*args): return apply(_blcelc.iPcInventory_RemoveEntity,args)
    def RemoveAll(*args): return apply(_blcelc.iPcInventory_RemoveAll,args)
    def GetEntityCount(*args): return apply(_blcelc.iPcInventory_GetEntityCount,args)
    def GetEntity(*args): return apply(_blcelc.iPcInventory_GetEntity,args)
    def SetStrictCharacteristics(*args): return apply(_blcelc.iPcInventory_SetStrictCharacteristics,args)
    def HasStrictCharacteristics(*args): return apply(_blcelc.iPcInventory_HasStrictCharacteristics,args)
    def SetConstraints(*args): return apply(_blcelc.iPcInventory_SetConstraints,args)
    def GetConstraints(*args): return apply(_blcelc.iPcInventory_GetConstraints,args)
    def RemoveConstraints(*args): return apply(_blcelc.iPcInventory_RemoveConstraints,args)
    def RemoveAllConstraints(*args): return apply(_blcelc.iPcInventory_RemoveAllConstraints,args)
    def GetCurrentCharacteristic(*args): return apply(_blcelc.iPcInventory_GetCurrentCharacteristic,args)
    def MarkDirty(*args): return apply(_blcelc.iPcInventory_MarkDirty,args)
    def TestConstraints(*args): return apply(_blcelc.iPcInventory_TestConstraints,args)
    def Dump(*args): return apply(_blcelc.iPcInventory_Dump,args)
    def __del__(self, destroy= _blcelc.delete_iPcInventory):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcInventory instance at %s>" % (self.this,)

class iPcInventoryPtr(iPcInventory):
    def __init__(self,this):
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
    def SetCharacteristic(*args): return apply(_blcelc.iPcCharacteristics_SetCharacteristic,args)
    def SetInheritedCharacteristic(*args): return apply(_blcelc.iPcCharacteristics_SetInheritedCharacteristic,args)
    def GetCharacteristic(*args): return apply(_blcelc.iPcCharacteristics_GetCharacteristic,args)
    def GetLocalCharacteristic(*args): return apply(_blcelc.iPcCharacteristics_GetLocalCharacteristic,args)
    def GetInheritedCharacteristic(*args): return apply(_blcelc.iPcCharacteristics_GetInheritedCharacteristic,args)
    def ClearCharacteristic(*args): return apply(_blcelc.iPcCharacteristics_ClearCharacteristic,args)
    def HasCharacteristic(*args): return apply(_blcelc.iPcCharacteristics_HasCharacteristic,args)
    def ClearAll(*args): return apply(_blcelc.iPcCharacteristics_ClearAll,args)
    def AddToInventory(*args): return apply(_blcelc.iPcCharacteristics_AddToInventory,args)
    def RemoveFromInventory(*args): return apply(_blcelc.iPcCharacteristics_RemoveFromInventory,args)
    def MarkDirty(*args): return apply(_blcelc.iPcCharacteristics_MarkDirty,args)
    def TestConstraints(*args): return apply(_blcelc.iPcCharacteristics_TestConstraints,args)
    def Dump(*args): return apply(_blcelc.iPcCharacteristics_Dump,args)
    def __del__(self, destroy= _blcelc.delete_iPcCharacteristics):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCharacteristics instance at %s>" % (self.this,)

class iPcCharacteristicsPtr(iPcCharacteristics):
    def __init__(self,this):
        _swig_setattr(self, iPcCharacteristics, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, iPcCharacteristics, 'thisown', 0)
        _swig_setattr(self, iPcCharacteristics,self.__class__,iPcCharacteristics)
_blcelc.iPcCharacteristics_swigregister(iPcCharacteristicsPtr)

celCreateCharacteristics = _blcelc.celCreateCharacteristics

celGetCharacteristics = _blcelc.celGetCharacteristics

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

