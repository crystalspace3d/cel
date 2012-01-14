from pycel import *

class smallgame:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "Initializing game..."
		region = celRegion(celEntity,"main")
		# @@@ Ugly hardcoding of path!
		region.SetWorldFile("/cellib/objects", "portal_world")
		region.SetRegionName("portals")
		rc = region.Load()
		room = region.GetStartSector()

		# @@@ The below is not very nice.
		bl = BehaviourLayers[0]

		# Create a box
		box = Entities["box"]
		box.Behaviour.real_init(room)

		# Create an actor
		actor = PhysicalLayer.CreateEntity("actor",bl,"actor")
		actor.Behaviour.real_init(region)

