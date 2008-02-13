###########################
# pycel.py High Level Interface for blpython
# For nicer access to many common functionality.
#
# Makes use of the fact there is a physical layer and object registry present, 
# and uses the variables registered by blpython to access them and make things
# nicer for the user.
#
# blpython mashalls this file into itself, and imports it, so you dont have
# to distribute it, just do: "import pycel" from your scripts.
#
# If you really want to use it from pure python cel application note 
# object_reg_ptr and physicallayer_ptr must be defined before importing this.
###########################
from cspace import *
from blcelc import *
import _blcelc

# Pointers
try:
	# from cspython
	oreg = object_reg
except:
	# from blpython
	oreg = object_reg_ptr
pl = oreg.Get(iCelPlLayer)
if not pl:
  print "pycel error: Can't find the PhysicalLayer!"

# Fast functions from the pl
getid = lambda s: _blcelc.iCelPlLayer_FetchStringID(pl,s)
parid = lambda s: _blcelc.iCelPlLayer_FetchStringID(pl,"cel.parameter."+s) 
actid = lambda s: _blcelc.iCelPlLayer_FetchStringID(pl,"cel.action."+s) 
propid = lambda s: _blcelc.iCelPlLayer_FetchStringID(pl,"cel.property."+s) 
fromid = pl.FetchString
parblock = pl.CreateParameterBlock
CreateEntity = pl.CreateEntity
RemoveEntity = pl.RemoveEntity

# Objects in the physical layer
Entities = pl.Entities
EntityTemplates = pl.EntityTemplates
BehaviourLayers = pl.BehaviourLayers
PcFactories = pl.PcFactories

# Pretty physical layer
PhysicalLayer = pl
ObjectRegistry = oreg

# Some plugins ready to use
Engine = oreg.Get(iEngine)
View = oreg.Get(iView)
Vfs = oreg.Get(iVFS)
Clock = oreg.Get(iVirtualClock)
Graphics2D = oreg.Get(iGraphics2D)
Graphics3D = oreg.Get(iGraphics3D)
Config = oreg.Get(iConfigManager)
Loader = oreg.Get(iLoader)
FontServer = oreg.Get(iFontServer)
KeyboardDriver = oreg.Get(iKeyboardDriver)
MouseDriver = oreg.Get(iMouseDriver)
JoystickDriver = oreg.Get(iJoystickDriver)
PluginManager = oreg.Get(iPluginManager)
StringSet = oreg.Get("crystalspace.shared.stringset", iStringSet)

# Helper class to buffer cel ids for faster access.
# This works by only requesting ids from cel the first time the string is
# used, then the string is saved as an instance attribute so it can be
# retrieved using python fast attribute access.
class CelIdsBuffer:
    def __init__(self,func):
        self.func = func
    def __getattr__(self,attr):
        setattr(self,attr,self.func(attr))
        return getattr(self,attr)
    __getitem__ = __getattr__

# Some ready to use buffers, these are not intended to be used by user at
# the moment. pycel should mangle all places where ids are used so strings
# can be used transparently.
CelIds = CelIdsBuffer(getid)
ParIds = CelIdsBuffer(parid)
ActIds = CelIdsBuffer(actid)
PropIds = CelIdsBuffer(propid)

# Some helper code to allow using str for parameterblocks (instead of ids)
get_pblockpar = _blcelc.iCelParameterBlock_GetParameter
set_pblockpar = _blcelc.iCelParameterBlock___setitem__

def getParItem(self,item):
    if isinstance(item,str):
        return get_pblockpar(self,getattr(ParIds,item))
    else:
        return get_pblockpar(self,item)

def setParItem(self,item,value):
    if isinstance(item,str):
        return set_pblockpar(self,getattr(ParIds,item),value)
    else:
        return set_pblockpar(self,item,value)

iCelParameterBlock.__getitem__ = getParItem
iCelParameterBlock.__setitem__ = setParItem

# Helpers to manage pcclass properties as python attributes
celpar_outmapping = {
 CEL_DATA_LONG : _blcelc.iCelPropertyClass_GetPropertyLong,
 CEL_DATA_FLOAT : _blcelc.iCelPropertyClass_GetPropertyFloat,
 CEL_DATA_BOOL : _blcelc.iCelPropertyClass_GetPropertyBool,
 CEL_DATA_STRING : _blcelc.iCelPropertyClass_GetPropertyString,
 CEL_DATA_VECTOR2 : _blcelc.iCelPropertyClass_GetPropertyVector2,
 CEL_DATA_VECTOR3 : _blcelc.iCelPropertyClass_GetPropertyVector3,
 CEL_DATA_COLOR : _blcelc.iCelPropertyClass_GetPropertyColor,
 CEL_DATA_ENTITY : _blcelc.iCelPropertyClass_GetPropertyEntity
}
celpar_inmapping = {
 CEL_DATA_LONG : _blcelc.iCelPropertyClass_SetPropertyLong,
 CEL_DATA_FLOAT : _blcelc.iCelPropertyClass_SetPropertyFloat,
 CEL_DATA_BOOL : _blcelc.iCelPropertyClass_SetPropertyBool,
 CEL_DATA_STRING : _blcelc.iCelPropertyClass_SetPropertyString,
 CEL_DATA_VECTOR2 : _blcelc.iCelPropertyClass_SetPropertyVector2,
 CEL_DATA_VECTOR3 : _blcelc.iCelPropertyClass_SetPropertyVector3,
 CEL_DATA_COLOR : _blcelc.iCelPropertyClass_SetPropertyColor,
 CEL_DATA_ENTITY : _blcelc.iCelPropertyClass_SetPropertyEntity
}
def PcSetterFallback(self,attr,value):
        parid = getattr(PropIds,attr)
        partype = self.GetPropertyOrActionType(parid)
	try:
	    return celpar_inmapping[partype](self,parid,value)
        except:
            if partype is CEL_DATA_NONE:
                raise AttributeError,"no property with name "+attr
            elif partype is CEL_DATA_ACTION:
                raise AttributeError,attr+" is an action"

def PcGetterFallback(self,attr):
        parid = getattr(PropIds,attr)
        partype = self.GetPropertyOrActionType(parid)
	try:
	    return celpar_outmapping[partype](self,parid)
        except:
            if partype is CEL_DATA_NONE:
                raise AttributeError,"no property with name "+attr
            elif partype is CEL_DATA_ACTION:
                raise AttributeError,attr+" is an action"

iCelPropertyClass.SetterFallback = PcSetterFallback
iCelPropertyClass.GetterFallback = PcGetterFallback

# Prettier handlers for pc creators
# (these dont need the pl parameter)
# Generates cel<PcClass> and celAdd<PcClass> functions
def AddPlArg(funct):
    def _inner(*args):
        args=(pl,)+args
        return funct(*args)
    return _inner

import pycel
for pcaccessor in dir():
    if pcaccessor.startswith("celGetSet"):
        pcname = pcaccessor.replace("celGetSet","")
        setattr(pycel,"cel"+pcname,
                AddPlArg(getattr(pycel,pcaccessor)))
        setattr(pycel,"celAdd"+pcname,
                AddPlArg(getattr(pycel,"celCreate"+pcname)))

# python property classes helpers
class PcFinder:
    def __init__(self,name):
        self.name = name
    def __call__(self,entity,tag=None):
        if tag:
            pc = entity.PropertyClassList.FindByNameAndTag(self.name,tag)
        else:
            pc = entity.PropertyClassList.FindByName(self.name)
        if not pc:
            pc = pl.CreatePropertyClass(entity,self.name)
        if pc:
            return pc.QueryInterface(iPcPython).GetPythonObject()

class pyPcCommonFactory(PcCommonFactory):
    """
    base factory for python property factories
    you dont need to use this class, simply use
    pycel.CEL_IMPLEMENT_FACTORY on your pcclass.
    """
    def __init__(self,cls,name):
        PcCommonFactory.__init__(self)
        self.name = name
        self.cls = cls
        self.__disown__() # give control to c++
    def GetName(self):
        return self.name
    def CreateScriptPropertyClass(self,name):
        a = self.cls(oreg)
        a.SetPythonObject(a)
        a.SetName(name)
        a.__disown__() # give control to c++
        return a

def CEL_IMPLEMENT_FACTORY(cls,name):
    """
    Create and register a factory for a PropertyClass class.
    Also creates a wrapper in the form cel<clsname> to query or
    create the pcclass from an entity.
    """ 
    fact = pyPcCommonFactory(cls,name)
    pl.RegisterPropertyClassFactory(fact)
    setattr(pycel,"cel"+cls.__name__,PcFinder(name))


