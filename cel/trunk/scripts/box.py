from blcelc import *

class box:
	def __init__(self,celEntity):
		print "Initializing box..."
	def real_init(self,celEntity,args):
		global room
		#solid = celCreateSolid(physicallayer_ptr,celEntity)
		#gravity = celCreateGravity(physicallayer_ptr,celEntity)
		#movable = celCreateMovable(physicallayer_ptr,celEntity)
		mesh = celCreateMesh(physicallayer_ptr,celEntity)
		# @@@ Ugly hardcoding of path!
		mesh.LoadMesh("fclosed", "/this/apps/celtest/data/box")
		pos = csVector3 (2, 2, 5)
		mesh.MoveMesh(room,pos)

		#gravity.SetWeight (1)
		#v = csVector3 (0, -9.8, 0)
		#gravity.ApplyPermanentForce (v)

