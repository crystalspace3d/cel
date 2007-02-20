# a behaviour for a trigger that changes camera mode
# on approaching. actorname is the name for entity that
# carries the camera
from pycel import *

actorname = "camera"

class camerachange:
	def __init__(self,celEntity):
		actor = Entites(actorname)
		self.camera = celGetDefaultCamera(actor)
		self.prevcamera = self.camera.ModeName
	
	def pctrigger_entityenters(self,celEntity,args):
		self.camera.ModeName = "firstperson"
	def pctrigger_entityleaves(self,celEntity,args):
		selfcamera.ModeName = self.prevcamera
