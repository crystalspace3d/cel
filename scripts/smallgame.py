from blcelc import *

class smallgame:
	def __init__(self,celEntity):
		print "Initializing game..."
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.region")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.mesh")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.solid")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.camera")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.gravity")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.movable")

		region = celCreateRegion(physicallayer_ptr,celEntity,"main")
		region.SetWorldFile("/lev/partsys", "world")
		region.SetRegionName("partsys")
		rc = region.Load()
		room = region.GetStartSector()

		camera = celCreateCamera(physicallayer_ptr,celEntity)
		camera.SetRegion(region)

		# @@@ The below is not very nice.
		bl = physicallayer_ptr.GetBehaviourLayer(0)
		box = celCreateEntity(physicallayer_ptr,"box")
		box_behaviour = box.CreateBehaviour (bl,"box")
		box_python = box_behaviour.GetPythonObject ()
		box_python.real_init(box,room)

