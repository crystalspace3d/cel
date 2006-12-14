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
oreg = object_reg_ptr
pl = physicallayer_ptr

# Fast functions from the pl
getid = lambda s: _blcelc.iCelPlLayer_FetchStringID(pl,s)
parid = lambda s: _blcelc.iCelPlLayer_FetchStringID(pl,"cel.parameter."+s) 
actid = lambda s: _blcelc.iCelPlLayer_FetchStringID("cel.action."+s) 
propid = lambda s: _blcelc.iCelPlLayer_FetchStringID("cel.property."+s) 
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
PhysicalLayer = physicallayer_ptr

# Some plugins ready to use
Engine = CS_QUERY_REGISTRY(oreg,iEngine)
View = CS_QUERY_REGISTRY(oreg,iView)
Vfs = CS_QUERY_REGISTRY(oreg,iVFS)
Clock = CS_QUERY_REGISTRY(oreg,iVirtualClock)
Graphics2D = CS_QUERY_REGISTRY(oreg,iGraphics2D)
Graphics3D = CS_QUERY_REGISTRY(oreg,iGraphics3D)
Config = CS_QUERY_REGISTRY(oreg,iConfigManager)
Loader = CS_QUERY_REGISTRY(oreg,iLoader)
FontServer = CS_QUERY_REGISTRY(oreg,iFontServer)
KeyboardDriver = CS_QUERY_REGISTRY(oreg,iKeyboardDriver)
MouseDriver = CS_QUERY_REGISTRY(oreg,iMouseDriver)
JoystickDriver = CS_QUERY_REGISTRY(oreg,iJoystickDriver)
PluginManager = CS_QUERY_REGISTRY(oreg,iPluginManager)
StringSet = CS_QUERY_REGISTRY_TAG_INTERFACE (oreg,"crystalspace.shared.stringset", iStringSet)

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

