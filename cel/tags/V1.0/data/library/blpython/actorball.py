# a behaviour for an actor ball that can be controlled by applying force
# it is a good start for playing with physics
# uses: mechanicsobject, defaultcamera, tooltip, commandinput

# it also prints a time out in the tooltip as an example.

from pycel import *

class actorball:
	def __init__(self,celEntity):
		print "Initializing actorball...",celEntity.Name
		# the timeout counter
		self.counter = 60
		# movement variables
		self.forward = 0
		self.side = 0
		# make sure we have a tooltip
		self.tooltip = celToolTip(celEntity)
		# make sure we have a pcdefaultcamera
		self.camera = celDefaultCamera(celEntity)
		# get a virtualclock interface to count time
		self.time = Clock.GetCurrentTicks()

		# get a mechanics object
		self.meshobject = celGetMechanicsObject(celEntity)
		self.mechsystem = self.meshobject.GetMechanicsSystem()
		self.mechsystem.Gravity = csVector3(0,0,0)
		# make sure we have a pccommandinput
		input = celCommandInput(celEntity)
		# bind inputs
		input.LoadConfig("actorball")
		self.gravity=False
	
	# TRIGGER CALLBACKS
	def pctrigger_leavetrigger(self,celEntity,args):
		pass
	def pctrigger_entertrigger(self,celEntity,args):
		pass
	# INVENTORY CALLBACK
	def pcinventory_addchild(self,celEntity,args):
		pass
	# TIMER CALLBACKS
	def pctimer_wakeupframe(self,celEntity,args):
		if self.counter > 0:
			self.tooltip.Text = "remaining time:"+str(self.counter)
		else:
			self.tooltip.Text = "GAME OVER"
		newtime = Clock.GetCurrentTicks()
		if self.forward:
			self.meshobject.AddForceFrame(csVector3(0,0,self.forward*20),False,csVector3(0,0,0))
		if self.side:
			self.meshobject.AddForceFrame(csVector3(self.side*20,0,0), False,csVector3(0,0,0))
	
	# LOOK UP/DOWN
	def pccommandinput_gravity1(self,celEntity,args):
		if self.gravity:
			self.mechsystem.Gravity = csVector3(0,0.0,0)
		else:
			self.mechsystem.Gravity = csVector3(0,-0.2,0)
		self.gravity = not self.gravity
	def pccommandinput_lookup1(self,celEntity,args):
		self.camera.SetPitchVelocity(1.0)

	def pccommandinput_lookup0(self,celEntity,args):
		self.camera.SetPitchVelocity(0.0)
	def pccommandinput_lookup_(self,celEntity,args):
		pass

	def pccommandinput_lookdown1(self,celEntity,args):
		self.camera.SetPitchVelocity(-1.0)
	def pccommandinput_lookdown0(self,celEntity,args):
		self.camera.SetPitchVelocity(0.0)
	def pccommandinput_lookdown_(self,celEntity,args):
		pass

	#JUMP
	def pccommandinput_jump1(self,celEntity,args):
		newtime = Clock.GetCurrentTicks()
		# this try to apply the force only if the object is not
		# touching the ground
		if newtime - self.time > -100:
			self.meshobject.AddForceOnce(csVector3(0,300,0),False,csVector3(0,0,0))
		self.time = newtime
	def pccommandinput_jump0(self,celEntity,args):
		pass
	def pccommandinput_jump_(self,celEntity,args):
		pass
	# MOVEMENT
	def pccommandinput_forteleft1(self,celEntity,args):
		self.side = 1
	def pccommandinput_forteleft0(self,celEntity,args):
		self.side = 0
	def pccommandinput_forteleft_(self,celEntity,args):
		pass
	def pccommandinput_forteright1(self,celEntity,args):
		self.side = -1
	def pccommandinput_forteright0(self,celEntity,args):
		self.side = 0
	
	def pccommandinput_forteright_(self,celEntity,args):
		pass
	def pccommandinput_forward1(self,celEntity,args):
		self.forward = -1
	def pccommandinput_forward0(self,celEntity,args):
		self.forward = 0
	def pccommandinput_forward_(self,celEntity,args):
		pass
	def pccommandinput_backward1(self,celEntity,args):
		self.forward = 1.4
	def pccommandinput_backward0(self,celEntity,args):
		self.forward = 0
	def pccommandinput_backward_(self,celEntity,args):
		pass
	# CAMERA MODES
	def pccommandinput_cammode1(self,celEntity,args):
		self.camera.Mode = self.camera.GetNextMode ()

	# COLLISION CALLBACK
	# all pcmechanicobjects have this
	def pcdynamicbody_collision(self,celEntity,args):
		newtime = Clock.GetCurrentTicks()
		self.time = newtime

