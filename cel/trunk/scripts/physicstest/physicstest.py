from blcelc import *

class physicstest:
	def __init__(self,celEntity):
		print "Initializing game..."
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.region")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.mesh")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.solid")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.camera")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.gravity")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.movable")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.pckeyinput")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.dynsys")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.dynbody")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.timer")

		g3d = CS_QUERY_REGISTRY(object_reg, iGraphics3D)
		txtmgr = g3d.GetTextureManager()
		engine = CS_QUERY_REGISTRY(object_reg, iEngine)
		col = csColor (0,0,0);
		woodtxt = engine.CreateTexture ("wood", "/lib/stdtex/parket.jpg",col,0)

		woodtxt.Register(txtmgr)
		woodmat = engine.CreateMaterial ("wood", woodtxt)
		woodmat.Register(txtmgr)

		region = celCreateRegion(physicallayer_ptr,celEntity,"main")
		region.CreateEmptySector("room")
		region.SetRegionName("balls")
		rc = region.Load()
		room = region.GetStartSector()

		# Dynamic system.
		dynsys = celCreateDynamicSystem(physicallayer_ptr,celEntity)

		# @@@ The below is not very nice.
		bl = physicallayer_ptr.GetBehaviourLayer(0)

		# Create a floor
		floor = celCreateEntity(physicallayer_ptr,"physics_floor")
		floor_behaviour = floor.CreateBehaviour(bl,"physics_floor")
		floor_python = floor_behaviour.GetPythonObject ()
		floor_python.real_init(floor,room,dynsys)

		# Create a box
		box = celCreateEntity(physicallayer_ptr,"physics_box")
		box_behaviour = box.CreateBehaviour(bl,"physics_box")
		box_python = box_behaviour.GetPythonObject ()
		box_python.real_init(box,room,dynsys)

		# Create an actor
		actor = celCreateEntity(physicallayer_ptr,"physics_actor")
		actor_behaviour = actor.CreateBehaviour(bl,"physics_actor")
		actor_python = actor_behaviour.GetPythonObject()
		actor_python.real_init(actor,region,dynsys)

