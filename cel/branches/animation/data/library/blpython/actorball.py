# a behaviour for an actor ball that can be controlled by applying force
# it is a good start for playing with physics
# uses: mechanicsobject, defaultcamera, tooltip, commandinput

# it also prints a time out in the tooltip as an example.

from pycel import *

class actorball:
	api_version = 2 # use new version of message callbacks.
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
	def pctrigger_leavetrigger(self,pc,args):
		pass
	def pctrigger_entertrigger(self,pc,args):
		pass
	# INVENTORY CALLBACK
	def pcinventory_addchild(self,pc,args):
		pass
	# TIMER CALLBACKS
	def pctimer_wakeupframe(self,pc,args):
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
	def pccommandinput_gravity1(self,pc,args):
		if self.gravity:
			self.mechsystem.Gravity = csVector3(0,0.0,0)
		else:
			self.mechsystem.Gravity = csVector3(0,-0.2,0)
		self.gravity = not self.gravity
	def pccommandinput_lookup1(self,pc,args):
		self.camera.SetPitchVelocity(1.0)

	def pccommandinput_lookup0(self,pc,args):
		self.camera.SetPitchVelocity(0.0)
	def pccommandinput_lookup_(self,pc,args):
		pass

	def pccommandinput_lookdown1(self,pc,args):
		self.camera.SetPitchVelocity(-1.0)
	def pccommandinput_lookdown0(self,pc,args):
		self.camera.SetPitchVelocity(0.0)
	def pccommandinput_lookdown_(self,pc,args):
		pass

	#JUMP
	def pccommandinput_jump1(self,pc,args):
		newtime = Clock.GetCurrentTicks()
		# this try to apply the force only if the object is not
		# touching the ground
		if newtime - self.time > -100:
			self.meshobject.AddForceOnce(csVector3(0,300,0),False,csVector3(0,0,0))
		self.time = newtime
	def pccommandinput_jump0(self,pc,args):
		pass
	def pccommandinput_jump_(self,pc,args):
		pass
	# MOVEMENT
	def pccommandinput_forteleft1(self,pc,args):
		self.side = 1
	def pccommandinput_forteleft0(self,pc,args):
		self.side = 0
	def pccommandinput_forteleft_(self,pc,args):
		pass
	def pccommandinput_forteright1(self,pc,args):
		self.side = -1
	def pccommandinput_forteright0(self,pc,args):
		self.side = 0
	
	def pccommandinput_forteright_(self,pc,args):
		pass
	def pccommandinput_forward1(self,pc,args):
		self.forward = -1
	def pccommandinput_forward0(self,pc,args):
		self.forward = 0
	def pccommandinput_forward_(self,pc,args):
		pass
	def pccommandinput_backward1(self,pc,args):
		self.forward = 1.4
	def pccommandinput_backward0(self,pc,args):
		self.forward = 0
	def pccommandinput_backward_(self,pc,args):
		pass
	# CAMERA MODES
	def pccommandinput_cammode1(self,pc,args):
		self.camera.Mode = self.camera.GetNextMode ()

	# COLLISION CALLBACK
	# all pcmechanicobjects have this
	def pcdynamicbody_collision(self,pc,args):
		newtime = Clock.GetCurrentTicks()
		self.time = newtime

