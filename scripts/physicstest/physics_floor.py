from blcelc import *

class physics_floor:
	def __init__(self,celEntity):
		print "Initializing floor..."
	def real_init(self,celEntity,room,dynsys):
		#mesh = celCreateMesh(physicallayer_ptr,celEntity)
		# @@@ Ugly hardcoding of path!
		#mesh.LoadMesh("box", "/this/data/box")
		#pos = csVector3 (1,-1,5)
		#mesh.MoveMesh(room,pos)

		self.dynbody = celCreateDynamicBody(physicallayer_ptr,celEntity)
		self.dynbody.SetDynamicSystem(dynsys)
		self.dynbody.SetParameters(0.9,100.0,1000.0,0.001,5.0)
		self.dynbody.AttachColliderPlane(csPlane3(0,-1,0,-1))
		#trans = mesh.GetMesh().GetMovable().GetFullTransform()
		#self.dynbody.AttachColliderBox(csVector3(30,.2,30),
			#csOrthoTransform())
		#self.dynbody.AttachColliderMesh()
		self.dynbody.MakeStatic(1)

