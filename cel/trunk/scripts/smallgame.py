from blcelc import *

class smallgame:
	def __init__(self,celEntity):
		print "Initializing game..."
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.region")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.mesh")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.solid")
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.camera")
		region = celCreateRegion(physicallayer_ptr,celEntity,"pcregion");
		print region
		region.SetWorldFile("/lev/partsys", "world")
		region.SetRegionName("partsys")
		rc = region.Load()
		print rc

		camera = celCreateCamera(physicallayer_ptr,celEntity,"pccamera")
		print camera
		camera.SetRegion(region)



