from blcelc import *

class physics_box:
	def __init__(self,celEntity):
		print "Initializing box..."
	def real_init(self,celEntity,room,dynsys):
		mesh = celCreateMesh(physicallayer_ptr,celEntity)
		# @@@ Ugly hardcoding of path!
		mesh.LoadMesh("box", "/this/apps/celtest/data/box")
		pos = csVector3 (1, 3, 5)
		mesh.MoveMesh(room,pos)

		self.dynbody = celCreateDynamicBody(physicallayer_ptr,celEntity)
		self.dynbody.SetDynamicSystem(dynsys)
		self.dynbody.SetParameters(0.9,1.0,0.0,0.0,5.0)
		self.dynbody.AttachColliderBox(csVector3(.5,.5,.5),
			csOrthoTransform())

