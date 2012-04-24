# a "behaviour" for an entity that appears and dissapears.
# you can set up a wakeup and the behaviour will blink using that instead
# of the predefined.
from pycel import *

class blink:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "initializing blink",celEntity.Name
		# some variables for the counter
		self.on		=	True
		self.counter	=	25
		# see if this has a mesh propclass
		self.mesh = celGetMesh(celEntity)
		if not self.mesh:
			print "Entity does not have a mesh (required for alphapulse)!!"
			return
		# get some pointers to interfaces and start position
		self.movable = self.mesh.Mesh.GetMovable()
		self.startpos = csVector3(self.movable.GetPosition())

		# set up the frame callback
		self.timer = celGetTimer(celEntity)
		if not self.timer:
			self.timer = celAddTimer(celEntity)
			self.timer.WakeUp (500,True)

	def pctimer_wakeup(self,pc,args):
		if self.on:
			self.on=False
			self.movable.SetPosition(csVector3(10000,1000,10000))
			self.movable.UpdateMove()
		else:
			self.on=True
			self.movable.SetPosition(self.startpos)
			self.movable.UpdateMove()

	def pctimer_wakeupframe(self,pc,args):
		self.counter =self.counter-1
		if self.counter<0:
			self.counter=25
			if self.on:
				self.on=False
				self.movable.SetPosition(csVector3(10000,1000,10000))
				self.movable.UpdateMove()
			else:
				self.on=True
				self.movable.SetPosition(self.startpos)
				self.movable.UpdateMove()

