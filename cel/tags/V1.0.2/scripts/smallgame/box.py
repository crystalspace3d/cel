from cspace import *
from blcelc import *

class box:
	def __init__(self,celEntity):
		print "Initializing box..."
	def real_init(self,celEntity,room):
		gravity = celGetGravity(celEntity)
		gravity.SetWeight (1)
		v = csVector3 (0, -9.8, 0)
		gravity.ApplyPermanentForce (v)

