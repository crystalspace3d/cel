# Python behaviour that can be used as a base to control the player.
# can also control a cal3d or particles mesh if present in the entity.
# Uses: defaultcamera,commandinput,actormove,linmove,meshselect,mesh,tooltip

from pycel import *

class actor:
	# INITIALIZATION
	def __init__(self,celEntity):
		print "Initializing actor...",celEntity.Name
		# check for pcdefaultcamera
		self.camera = celDefaultCamera(celEntity)
		# check for pccommandinput
		input = celCommandInput(celEntity)
		# check for actormove
		self.actormove = celActorMove(celEntity)
		# check for mesh
		self.mesh = celGetMesh(celEntity)
		#if not self.mesh:
		#	self.mesh = celCreateMesh(physicallayer_ptr,celEntity)
		#	self.mesh.LoadMesh("box", "/cellib/objects/box")
		#	pos = csVector3 (0, 1, 0)
		#	self.mesh.MoveMesh(room,pos)
		meshobj = self.mesh.Mesh
		self.cal3dstate = SCF_QUERY_INTERFACE(meshobj.GetMeshObject(), iSpriteCal3DState)
		# check for linmove
		self.linmove = celLinearMovement(celEntity)
		# check for tooltip
		self.tooltip = celToolTip(celEntity)
		# set some default parameters
		#self.camera.SetModeName ("thirdperson")
		#self.actormove.SetMovementSpeed(1.5)
		#self.actormove.SetRunningSpeed (3.2)
		#self.actormove.SetRotationSpeed (2.0)
		#self.actormove.SetJumpingVelocity (5.9)

		# init collider, note that this invalidates any collider
		# set to the mesh
		#self.linmove.InitCD(csVector3(0.7,1.0/3.0,1.5),csVector3(.5,2.0/3.0,1.5),
		#	csVector3(0,0.0,0.75))

		# bind some keys
		input.LoadConfig("actor")
	
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
	def pccommandinput_forward1(self,celentity,args):
		meshobj = self.mesh.Mesh
		self.actormove.MovingForward = True
	def pccommandinput_forward0(self,celentity,args):
		self.actormove.MovingForward = 0
	def pccommandinput_forward_(self,celentity,args):
		pass
	def pccommandinput_backward1(self,celEntity,args):
		self.actormove.MovingBackward = 1
	def pccommandinput_backward0(self,celEntity,args):
		self.actormove.MovingBackward = 0
	def pccommandinput_backward_(self,celentity,args):
		pass
		
	# ROTATE
	def pccommandinput_rotateleft1(self,celEntity,args):
		self.actormove.RotatingLeft = 1
	def pccommandinput_rotateleft0(self,celEntity,args):
		self.actormove.RotatingLeft = 0
	def pccommandinput_rotateleft_(self,celentity,args):
		pass
	def pccommandinput_rotateright1(self,celEntity,args):
		self.actormove.RotatingRight = 1
	def pccommandinput_rotateright0(self,celEntity,args):
		self.actormove.RotatingRight = 0
	def pccommandinput_rotateright_(self,celentity,args):
		pass

	# STRAFE
	def pccommandinput_strafeleft1(self,celEntity,args):
		self.actormove.StrafingLeft = 1
		if self.cal3dstate:
			self.cal3dstate.SetAnimCycle("Strafeleft",1)

	def pccommandinput_strafeleft0(self,celEntity,args):
		self.actormove.StrafingLeft = 0
	def pccommandinput_straferight1(self,celEntity,args):
		self.actormove.StrafingRight = 1
		if self.cal3dstate:
			self.cal3dstate.SetAnimCycle("Straferight",1)	
	def pccommandinput_straferight0(self,celEntity,args):
		self.actormove.StrafingRight = 0

	# LOOK UP AND DOWN
	def pccommandinput_lookup1(self,celEntity,args):
		self.camera.PitchVelocity = 1.0

	def pccommandinput_lookup0(self,celEntity,args):
		self.camera.PitchVelocity = 0.0

	def pccommandinput_lookdown1(self,celEntity,args):
		self.camera.PitchVelocity = -1.0

	def pccommandinput_lookdown0(self,celEntity,args):
		self.camera.PitchVelocity = 0.0

	# JUMP
	def pccommandinput_jump1(self,celEntity,args):
		if self.cal3dstate:
			self.cal3dstate.SetAnimAction("Jump",0.1,0.2)
		self.actormove.Jump()
	def pccommandinput_jump0(self,celEntity,args):
		pass

	# RUN
	def pccommandinput_run1(self,celEntity,args):
		self.actormove.Running = True

	def pccommandinput_run0(self,celEntity,args):
		self.actormove.Running = False

	# CAMERA MODES
	def pccommandinput_cammode1(self,celEntity,args):
		self.camera.Mode = self.camera.GetNextMode ()

	# TRIGGER CALLBACKS
	# you will receive this callbacks when the entity enters
	# or leaves triggers.
	def pctrigger_entertrigger(self,celEntity,args):
		pass
	def pctrigger_leavetrigger(self,celEntity,args):
		pass
	def pclinearmovement_collision(self,celEntity,args):
		pass

