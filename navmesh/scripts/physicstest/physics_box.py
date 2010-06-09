from pycel import *

class physics_box:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "Initializing box..."
		self.entity = celEntity
	def real_init(self,sector,dynsys):
		pcmesh = celMesh(self.entity)
		# @@@ Ugly hardcoding of path!
		pcmesh.SetMesh("box", "/cellib/objects/box")
		pos = csVector3 (1, 3, 5)
		pcmesh.MoveMesh(sector,pos)

		self.dynbody = celMechanicsObject(self.entity)
		self.dynbody.SetMechanicsSystem(dynsys)
		self.dynbody.AttachColliderBox(csVector3(.5,.5,.5),
			csOrthoTransform())

