###########################
# pycel.py High Level Interface for blpython
# For nicer access to many common functionality.
# Makes use of the fact there is a physical layer present, and uses
# the variable registered in blpython to access it (physicallayer_ptr).
# blpython mashalls this file into itself, and imports it, so you dont have
# to distribute it, just do: "import pycel" from your scripts.
###########################
from cspace import *
from blcelc import *

# pointers
oreg = object_reg_ptr
pl = physicallayer_ptr

# fast functions from the pl
getid = pl.FetchStringID
fromid = pl.FetchString
parblock = pl.CreateParameterBlock
CreateEntity = pl.CreateEntity
RemoveEntity = pl.RemoveEntity

# objects in the physical layer
Entities = pl.Entities
EntityTemplates = pl.EntityTemplates
BehaviourLayers = pl.BehaviourLayers
PcFactories = pl.PcFactories

# pretty physical layer
PhysicalLayer = physicallayer_ptr

# some plugins ready to use
Engine = CS_QUERY_REGISTRY(oreg,iEngine)
View = CS_QUERY_REGISTRY(oreg,iView)
Vfs = CS_QUERY_REGISTRY(oreg,iVFS)
Clock = CS_QUERY_REGISTRY(oreg,iVirtualClock)
Graphics2D = CS_QUERY_REGISTRY(oreg,iGraphics2D)
Graphics3D = CS_QUERY_REGISTRY(oreg,iGraphics3D)
Config = CS_QUERY_REGISTRY(oreg,iConfigManager)
StringSet = CS_QUERY_REGISTRY_TAG_INTERFACE (oreg,"crystalspace.shared.stringset", iStringSet)

# Prettier handlers for pc creators
# (these dont need the pl parameter)
celMechanicsSystem = lambda ent: celGetSetMechanicsSystem(physicallayer_ptr,ent)
celAddMechanicsSystem = lambda ent: celCreateMechanicsSystem(physicallayer_ptr,ent)
celMechanicsObject = lambda ent: celGetSetMechanicsObject(physicallayer_ptr,ent)
celAddMechanicsObject = lambda ent: celCreateMechanicsObject(physicallayer_ptr,ent)
celMechanicsJoint = lambda ent: celGetSetMechanicsJoint(physicallayer_ptr,ent)
celAddMechanicsJoint = lambda ent: celCreateMechanicsJoint(physicallayer_ptr,ent)
celMechanicsThrusterReactionary = lambda ent: celGetSetMechanicsThrusterReactionary(physicallayer_ptr,ent)
celAddMechanicsThrusterReactionary = lambda ent: celCreateMechanicsThrusterReactionary(physicallayer_ptr,ent)
celMechanicsBalancedGroup = lambda ent: celGetSetMechanicsBalancedGroup(physicallayer_ptr,ent)
celAddMechanicsBalancedGroup = lambda ent: celCreateMechanicsBalancedGroup(physicallayer_ptr,ent)
celMechanicsThrusterController = lambda ent: celGetSetMechanicsThrusterController(physicallayer_ptr,ent)
celAddMechanicsThrusterController = lambda ent: celCreateMechanicsThrusterController(physicallayer_ptr,ent)
celBillboard = lambda ent: celGetSetBillboard(physicallayer_ptr,ent)
celAddBillboard = lambda ent: celCreateBillboard(physicallayer_ptr,ent)
celZoneManager = lambda ent: celGetSetZoneManager(physicallayer_ptr,ent)
celAddZoneManager = lambda ent: celCreateZoneManager(physicallayer_ptr,ent)
celCommandInput = lambda ent: celGetSetCommandInput(physicallayer_ptr,ent)
celAddCommandInput = lambda ent: celCreateCommandInput(physicallayer_ptr,ent)
celLinearMovement = lambda ent: celGetSetLinearMovement(physicallayer_ptr,ent)
celAddLinearMovement = lambda ent: celCreateLinearMovement(physicallayer_ptr,ent)
celActorMove = lambda ent: celGetSetActorMove(physicallayer_ptr,ent)
celAddActorMove = lambda ent: celCreateActorMove(physicallayer_ptr,ent)
celCamera = lambda ent: celGetSetCamera(physicallayer_ptr,ent)
celAddCamera = lambda ent: celCreateCamera(physicallayer_ptr,ent)
celDefaultCamera = lambda ent: celGetSetDefaultCamera(physicallayer_ptr,ent)
celAddDefaultCamera = lambda ent: celCreateDefaultCamera(physicallayer_ptr,ent)
celSimpleCamera = lambda ent: celGetSetSimpleCamera(physicallayer_ptr,ent)
celAddSimpleCamera = lambda ent: celCreateSimpleCamera(physicallayer_ptr,ent)
celMeshSelect = lambda ent: celGetSetMeshSelect(physicallayer_ptr,ent)
celAddMeshSelect = lambda ent: celCreateMeshSelect(physicallayer_ptr,ent)
celMesh = lambda ent: celGetSetMesh(physicallayer_ptr,ent)
celAddMesh = lambda ent: celCreateMesh(physicallayer_ptr,ent)
celTimer = lambda ent: celGetSetTimer(physicallayer_ptr,ent)
celAddTimer = lambda ent: celCreateTimer(physicallayer_ptr,ent)
celProjectile = lambda ent: celGetSetProjectile(physicallayer_ptr,ent)
celAddProjectile = lambda ent: celCreateProjectile(physicallayer_ptr,ent)
celSolid = lambda ent: celGetSetSolid(physicallayer_ptr,ent)
celAddSolid = lambda ent: celCreateSolid(physicallayer_ptr,ent)
celGravity = lambda ent: celGetSetGravity(physicallayer_ptr,ent)
celAddGravity = lambda ent: celCreateGravity(physicallayer_ptr,ent)
celMovable = lambda ent: celGetSetMovable(physicallayer_ptr,ent)
celAddMovable = lambda ent: celCreateMovable(physicallayer_ptr,ent)
celInventory = lambda ent: celGetSetInventory(physicallayer_ptr,ent)
celAddInventory = lambda ent: celCreateInventory(physicallayer_ptr,ent)
celCharacteristics = lambda ent: celGetSetCharacteristics(physicallayer_ptr,ent)
celAddCharacteristics = lambda ent: celCreateCharacteristics(physicallayer_ptr,ent)
celToolTip = lambda ent: celGetSetToolTip(physicallayer_ptr,ent)
celAddToolTip = lambda ent: celCreateToolTip(physicallayer_ptr,ent)
celSoundSource = lambda ent: celGetSetSoundSource(physicallayer_ptr,ent)
celAddSoundSource = lambda ent: celCreateSoundSource(physicallayer_ptr,ent)
celSoundListener = lambda ent: celGetSetSoundListener(physicallayer_ptr,ent)
celAddSoundListener = lambda ent: celCreateSoundListener(physicallayer_ptr,ent)
celProperties = lambda ent: celGetSetProperties(physicallayer_ptr,ent)
celAddProperties = lambda ent: celCreateProperties(physicallayer_ptr,ent)
celMover = lambda ent: celGetSetMover(physicallayer_ptr,ent)
celAddMover = lambda ent: celCreateMover(physicallayer_ptr,ent)
celHover = lambda ent: celGetSetHover(physicallayer_ptr,ent)
celAddHover = lambda ent: celCreateHover(physicallayer_ptr,ent)
celCraftController = lambda ent: celGetSetCraftController(physicallayer_ptr,ent)
celAddCraftController = lambda ent: celCreateCraftController(physicallayer_ptr,ent)
celWheeled = lambda ent: celGetSetWheeled(physicallayer_ptr,ent)
celAddWheeled = lambda ent: celCreateWheeled(physicallayer_ptr,ent)
celDamage = lambda ent: celGetSetDamage(physicallayer_ptr,ent)
celAddDamage = lambda ent: celCreateDamage(physicallayer_ptr,ent)
