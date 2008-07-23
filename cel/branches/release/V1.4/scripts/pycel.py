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
oreg = None
try:
    # from cspython
    oreg = object_reg
except:
    # from blpython
    try:
        oreg = object_reg_ptr
    except:
        raise EnvironmentError, "CrystalSpace not initialized"

pl = oreg.Get(iCelPlLayer)
if not pl:
    raise EnvironmentError, "Can't find the PhysicalLayer"

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
Reporter = oreg.Get(iReporter)
FontServer = oreg.Get(iFontServer)
KeyboardDriver = oreg.Get(iKeyboardDriver)
MouseDriver = oreg.Get(iMouseDriver)
JoystickDriver = oreg.Get(iJoystickDriver)
PluginManager = oreg.Get(iPluginManager)
StringSet = oreg.Get("crystalspace.shared.stringset", iStringSet)
# the following is for the rendermanager branch.
try:
	SVStringSet = oreg.Get("crystalspace.shader.variablenameset", iShaderVarStringSet)
except:
	pass

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
 CEL_DATA_LONG : _blcelc.iCelPropertyClass_GetPropertyLongByID,
 CEL_DATA_FLOAT : _blcelc.iCelPropertyClass_GetPropertyFloatByID,
 CEL_DATA_BOOL : _blcelc.iCelPropertyClass_GetPropertyBoolByID,
 CEL_DATA_STRING : _blcelc.iCelPropertyClass_GetPropertyStringByID,
 CEL_DATA_VECTOR2 : _blcelc.iCelPropertyClass_GetPropertyVector2ByID,
 CEL_DATA_VECTOR3 : _blcelc.iCelPropertyClass_GetPropertyVector3ByID,
 CEL_DATA_COLOR : _blcelc.iCelPropertyClass_GetPropertyColorByID,
 CEL_DATA_ENTITY : _blcelc.iCelPropertyClass_GetPropertyEntityByID
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

def iCelEntityPlFakeArray__getattr__(self,name):
	return self.__getitem__(name)

iCelEntityPlFakeArray.__getattr__ = iCelEntityPlFakeArray__getattr__

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

_known_pcs = {}

def CEL_IMPLEMENT_FACTORY(cls,name):
    """
    Create and register a factory for a PropertyClass class.
    Also creates a wrapper in the form cel<clsname> to query or
    create the pcclass from an entity.
    """ 
    cls.__pccode__ = name # tag the class so we can find the code if needed
    _known_pcs[name] = cls
    fact = pyPcCommonFactory(cls,name)
    pl.RegisterPropertyClassFactory(fact)
    setattr(pycel,"cel"+cls.__name__,PcFinder(name))


# Some functions to reload property classes on the fly.
# Useful for developing process.
def __ReloadPyPC(module,code,hard):
    """
    Reload a python property class given python module name and cel name.
    module: python module name where the pc resides.
    code: cel code for the property class.
    hard: whether to reinstantiate already instantiated pcs.
    """ 
    import sys
    pcfact = pl.FindPropertyClassFactory(code)
    if not module in sys.modules:
        Reporter.ReportWarning("pycel",
            "Unable to reload pc "+code+" (module "+module+" not loaded)")
        return
    if not pcfact:
        Reporter.ReportWarning("pycel",
            "Unable to reload pc "+code+" (pc factory not loaded)")
        return
    pl.UnregisterPropertyClassFactory(pcfact)
    reload(sys.modules[module])
    if not hard:
        Reporter.ReportNotify("pycel",code+" reloaded.")
        return
    ent_count = 0
    for ent in Entities:
        pclist = list(ent.PropertyClassList)
        for pc in pclist:
             if pc.Name == code:
                 tag = pc.Tag
                 ent.PropertyClassList.Remove(pc)
                 newpc = PhysicalLayer.CreatePropertyClass(ent,code,tag)
                 if not newpc:
                      Reporter.ReportError("pycel","Unable to reload pc "+code+". Reloading old one.")
                      pl.RegisterPropertyClassFactory(pcfact)
                      ent.PropertyClassList.Add(pc)
                      return
                 else:
                      ent_count+=1
    Reporter.ReportNotify("pycel",code+" reloaded for "+str(ent_count)+" entities.")
    return True

def ReloadPyPc(pccode,hard=True):
    """
    Reload a python property class given its cel name.
    pccode: code for the property class
    hard: whether to reinitialize property classes already instantiated.
    """
    import traceback
    try:
        pcclass = _known_pcs[pccode]
    except:
        Reporter.ReportWarning("pycel",pccode+" not known. cant be reloaded.")
        return
    try:
        __ReloadPyPC(pcclass.__module__,pccode,hard)
    except:
        Reporter.ReportError("pycel",pccode+" failed loading.")
        traceback.print_exc()



