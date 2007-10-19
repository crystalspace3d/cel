from pycel import *

class box:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "Initializing box..."
		self.entity = celEntity
	def real_init(self,room):
		gravity = celGravity(self.entity)
		gravity.SetWeight (1)
		v = csVector3 (0, -9.8, 0)
		gravity.ApplyPermanentForce (v)

