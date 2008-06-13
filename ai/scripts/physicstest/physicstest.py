from pycel import *

class physicstest:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "Initializing game..."
		txtmgr = Graphics3D.GetTextureManager()
		col = csColor (0,0,0);
		woodtxt = Loader.LoadTexture ("wood", "/lib/stdtex/parket.jpg",CS_TEXTURE_3D)
		region = celRegion(celEntity,"main")
		region.CreateEmptySector("room")
		region.SetRegionName("balls")
		rc = region.Load()
		room = region.GetStartSector()

		# Dynamic system.
		dynsys = celMechanicsSystem(celEntity)

		# @@@ The below is not very nice.
		bl = PhysicalLayer.BehaviourLayers[0]

		# Create a floor
		floor = PhysicalLayer.CreateEntity("physics_floor",bl,
						   "physics_floor")
		floor.Behaviour.real_init(room,dynsys)

		# Create a box
		box = PhysicalLayer.CreateEntity("physics_box",bl,
						 "physics_box")
		box.Behaviour.real_init(room,dynsys)

		# Create an actor
		actor = PhysicalLayer.CreateEntity("physics_actor",bl,
						   "physics_actor")
		actor.Behaviour.real_init(room,dynsys)

