# Python behaviour that can be used as a base to control a hovercraft.
# Uses: defaultcamera,commandinput,craft,mesh

from pycel import *

class hoveractor:
	# INITIALIZATION
	def __init__(self,celEntity):
		print "Initializing hoveractor...",celEntity.Name
		# check for pcdefaultcamera
		self.camera = celGetDefaultCamera(celEntity)
		if not self.camera:
			self.camera = celAddDefaultCamera(celEntity)
			self.camera.SetModeName ("thirdperson")
		# check for pccommandinput
		input = celCommandInput(celEntity)
		# check for mechanics object
		self.mechobj = celGetMechanicsObject(celEntity)
		if not self.mechobj:
			print "Object does not have a mechobj!!!"
			return
		# check for actormove
		self.craftmove = celCraftController(celEntity)
		# check for mesh
		self.mesh = celGetMesh(celEntity)
		meshobj = self.mesh.Mesh
		if not self.mesh:
			print "Object does not have a mesh!!!"
			return
		# bind some keys
		input.LoadConfig("hoveractor")
	
	# ENTITY CALLBACKS
	# the engine will call this functions in response
	# to certain events. 
	# pctimer wakeup callback
	def pctimer_wakeup (self,celentity,args):
		pass
		
	# walk callbacks. note that for each bind defined in
	# pccommandinput there are three functions calles:
	# pccommandinput_(bindcode)1     -> Key down event
	# pccommandinput_(bindcode)0     -> Key release event
	# pccommandinput_(bindcode)_     -> Key hold events
	def pccommandinput_thrust1(self,celentity,args):
		self.craftmove.ThrustOn()
	def pccommandinput_thrust0(self,celentity,args):
		self.craftmove.ThrustOff()
	def pccommandinput_thrust_(self,celentity,args):
		pass
	def pccommandinput_forward1(self,celentity,args):
		self.craftmove.StartTurnUp()
	def pccommandinput_forward0(self,celentity,args):
		self.craftmove.StopTurnUp()
	def pccommandinput_forward_(self,celentity,args):
		pass
	def pccommandinput_backward1(self,celEntity,args):
		self.craftmove.StartTurnDown()
	def pccommandinput_backward0(self,celEntity,args):
		self.craftmove.StopTurnDown()
	def pccommandinput_backward_(self,celentity,args):
		pass
		
	# ROTATE
	def pccommandinput_rotateleft1(self,celEntity,args):
		self.craftmove.StartTurnLeft()
	def pccommandinput_rotateleft0(self,celEntity,args):
		self.craftmove.StopTurnLeft()
	def pccommandinput_rotateleft_(self,celentity,args):
		pass
	def pccommandinput_rotateright1(self,celEntity,args):
		self.craftmove.StartTurnRight()
	def pccommandinput_rotateright0(self,celEntity,args):
		self.craftmove.StopTurnRight()
	def pccommandinput_rotateright_(self,celentity,args):
		pass

	# STRAFE
	def pccommandinput_strafeleft1(self,celEntity,args):
		pass
	def pccommandinput_strafeleft0(self,celEntity,args):
		pass
	def pccommandinput_straferight1(self,celEntity,args):
		pass
	def pccommandinput_straferight0(self,celEntity,args):
		pass

	# LOOK UP AND DOWN
	def pccommandinput_lookup1(self,celEntity,args):
		self.camera.PitchVelocity = 1.0

	def pccommandinput_lookup0(self,celEntity,args):
		self.camera.PitchVelocity = 0.0

	def pccommandinput_lookdown1(self,celEntity,args):
		self.camera.PitchVelocity = -1.0

	def pccommandinput_lookdown0(self,celEntity,args):
		self.camera.PitchVelocity = 0.0

	# CAMERA MODES
	def pccommandinput_cammode1(self,celEntity,args):
		self.camera.Mode = self.camera.GetNextMode ()
	def pccommandinput_jump1(self,celEntity,args):
		self.mechobj.AddForceDuration(csVector3 (0, 25.0, 0), False,csVector3 (0, 0, 0), 0.2)

	# TRIGGER CALLBACKS
	# you will receive this callbacks when the entity enters
	# or leaves triggers.
	def pctrigger_entertrigger(self,celEntity,args):
		pass
	def pctrigger_leavetrigger(self,celEntity,args):
		pass
	def pcdynamicbody_collision(self,celEntity,args):
		pass

