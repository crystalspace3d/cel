from cspace import *
from blcelc import *

class smallgame:
	def __init__(self,celEntity):
		print "Initializing game..."
		region = celCreateRegion(physicallayer_ptr,celEntity,"main")
		# @@@ Ugly hardcoding of path!
		region.SetWorldFile("/cellib/objects", "portal_world")
		region.SetRegionName("portals")
		rc = region.Load()
		room = region.GetStartSector()

		# @@@ The below is not very nice.
		bl = physicallayer_ptr.GetBehaviourLayer(0)

		# Create a box
		box = physicallayer_ptr.FindEntity("box")
		box_behaviour = box.GetBehaviour()
		box_python = box_behaviour.GetPythonObject ()
		box_python.real_init(box,room)

		# Create an actor
		actor = celCreateEntity(physicallayer_ptr,"actor")
		actor_behaviour = actor.CreateBehaviour(bl,"actor")
		actor_python = actor_behaviour.GetPythonObject()
		actor_python.real_init(actor,region)
