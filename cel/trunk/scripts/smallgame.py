from blcelc import *

class smallgame:
	def __init__(self,celEntity):
		print "Initializing game..."
		celRegisterPCFactory(object_reg_ptr,"cel.pcfactory.region")
		region = celCreateRegion(physicallayer_ptr,celEntity,"pcregion");
		print region


