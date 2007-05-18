# a behaviour for a trigger that changes camera mode
# on approaching. actorname is the name for entity that
# carries the camera
from pycel import *

actorname = "camera"

class camerachange:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		actor = Entites(actorname)
		self.camera = celGetDefaultCamera(actor)
		self.prevcamera = self.camera.ModeName
	
	def pctrigger_entityenters(self,pc,args):
		self.camera.ModeName = "firstperson"
	def pctrigger_entityleaves(self,pc,args):
		self.camera.ModeName = self.prevcamera
