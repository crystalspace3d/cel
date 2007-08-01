# a "behaviour" for an entity that continuously looks at the player.
# this goes together with a pctimer with a wakeupframe on the entity, but
# the behaviour will create it automatically if not present already.
from pycel import *

import random

class lookat:
	def __init__(self,celEntity):
		print "initializing lookat",celEntity.Name
		# some variables for the counter
		self.monitor		=	"camera"

		# get the mesh and movable for this entity
		self.mesh = celGetMesh(celEntity)
		if not self.mesh:
			print "lookat requires entity having a mesh!"
		self.movable = self.mesh.Mesh.GetMovable()
		# get the movable for monitored entity
		monitor_ent=Entities[self.monitor]
		if not monitor_ent:
			print "lookat could not find",self.monitor,"entity"
		monitor_mesh  = celGetMesh(monitor_ent)
		if monitor_mesh:
			self.monitor_mov  = monitor_mesh.Mesh.GetMovable()
		else:
			"lookat requires monitored entity having a mesh!"
		
		# set up the frame callback
		self.timer = celGetTimer(celEntity)
		if not self.timer:
			self.timer = celAddTimer(celEntity)
			self.timer.WakeUpFrame (CEL_EVENT_PRE)


	def pctimer_wakeupframe(self,celEntity,args):
		monitored_pos = self.monitor_mov.GetPosition()
		self_pos = self.movable.GetPosition()
		self.movable.GetTransform().LookAt(monitored_pos-self_pos,csVector3(0,1,0))
		self.movable.UpdateMove()

