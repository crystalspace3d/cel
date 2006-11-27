# a "behaviour" for an npc with different flavours, controlled by the "chosen"
# pcproperty, with the following values:
# - 0: will walk and stop randomly. (default)
# - 1: will stay completely still.
# . 2: will walk reandomly (doesnt stop).
# also can control simple actions on an spr3d.
# uses: linmove, actormove, timer, mesh, properties
from pycel import *

import random

class npc:
	def __init__(self,celEntity):
		print "Initializing npc...",celEntity.Name
		# get or create linmove
		self.linmove = celLinearMovement(celEntity)
		# get or create actormove
		self.actormove = celActorMove(celEntity)
		self.actormove.MovementSpeed = 1.0
		self.actormove.JumpingVelocity = 4.1

		# set timer if there is not one
		self.timer = celGetTimer(celEntity)
                if not self.timer:
                        self.timer = celAddTimer(celEntity)
                        self.timer.WakeUp (500,True)

		# get pcproperties
		self.thechosen=False
		self.prop = celGetProperties(celEntity)
		if self.prop:
			i=self.prop.GetPropertyIndex("chosen")
			if not i == None:
				self.thechosen=self.prop.GetPropertyLong(i)
		# check for mesh
                self.mesh = celGetMesh(celEntity)
                if self.mesh:
                	meshobj = self.mesh.Mesh
			self.spr3dstate = SCF_QUERY_INTERFACE(meshobj.GetMeshObject(), iSprite3DState)
			if self.spr3dstate:
				self.spr3dstate.SetAction("walk")
		else:
			self.spr3dstate = None
		# check for thechosen state
		if self.thechosen == 1:
			if self.spr3dstate:
				self.spr3dstate.SetAction("stand")
		else:
			if self.spr3dstate:
				self.spr3dstate.SetAction("walk")

	def real_init(self,celEntity,room):
		pass
	def pcmeshsel_up(self,celEntity,args):
		pl = physicallayer_ptr
		actor = Entities["camera"]
		toolt = celGetToolTip(actor)
		if toolt:
			toolt.Text = celEntity.Name
		

	def pctimer_wakeup(self,celEntity,args):
		# ADVANCE
		advance = random.random()
		is_advancing = advance<0.5
		# the chosen 1 does stay still
		if self.thechosen==1:
			return
		# ROTATE
		value = random.random()
		if value<0.3 and is_advancing:
			self.actormove.RotatingLeft = 1
			self.actormove.RotatingRight = 0
		elif value<0.8 and is_advancing:
			self.actormove.RotatingLeft = 0
			self.actormove.RotatingRight = 1
		else:
			self.actormove.RotatingLeft = 0
			self.actormove.RotatingRight = 0
		# the chosen 2 can only run forward
		if self.thechosen==2:
			self.actormove.MovingForward = 1
			return
		if is_advancing:
			if not self.actormove.MovingForward:
				self.actormove.MovingForward = 1
				self.actormove.MovingBackward = 0
				if self.spr3dstate:
					self.spr3dstate.SetAction("walk")
		else:
			if self.actormove.MovingForward:
				self.actormove.MovingForward = 0
				self.actormove.MovingBackward = 0
				if self.spr3dstate:
					self.spr3dstate.SetAction("stand")


