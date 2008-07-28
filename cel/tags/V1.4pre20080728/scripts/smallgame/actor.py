from pycel import *

class actor:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "Initializing actor..."
		self.entity = celEntity

	def real_init(self,region):
		self.forward = 0
		self.backward = 0
		self.straferight = 0
		self.strafeleft = 0
		self.rotateright = 0
		self.rotateleft = 0
		self.autorun = 0
		self.running = 0

		input = celCommandInput(self.entity)
		self.camera = celDefaultCamera(self.entity)
		#self.actormove = celActorMove(self.entity)
		mesh = celMesh(self.entity)

		self.linmove = celLinearMovement(self.entity)
		self.camera.SetRegion(region)
		self.camera.SetMode(self.camera.firstperson)
		#self.solid = celSolid(self.entity)
		#self.actormove.SetMovementSpeed(2.0)
		#self.actormove.SetRunningSpeed (5.0)
		#self.actormove.SetRotationSpeed (1.75)
		#self.actormove.SetJumpingVelocity (6.31)

		# @@@ Ugly hardcoding of path!
		room = region.GetStartSector()
		
		mesh.LoadMesh("box", "/cellib/objects/box")
		pos = csVector3 (0, 1, 0)
		mesh.MoveMesh(room,pos)


		self.linmove.InitCD(csVector3(.5,1.0/3.0,.5),csVector3(.5,2.0/3.0,.5),
			csVector3(0,0.0,0))

		input.Bind("up", "forward")
		input.Bind("down", "backward")
		input.Bind("left", "rotateleft")
		input.Bind("right", "rotateright")
		input.Bind("a", "strafeleft")
		input.Bind("d", "straferight")
		input.Bind("shift", "run")
		input.Bind("m", "cammode")
		input.Bind(" ", "jump")

	def handle_movement(self,jump):
		if self.running:
			movementspeed = 5.0
		else:
			movementspeed = 2.0
		velocity = csVector3 (0, 0, 0)
		#self.linmove.GetVelocity(velocity)
		if (self.autorun or self.forward) and self.straferight:
			velocity.x = -0.75 * movementspeed
			velocity.z = -0.75 * movementspeed
			self.linmove.SetVelocity(velocity)
		elif (self.autorun or self.forward) and self.strafeleft:
			velocity.x = 0.75 * movementspeed
			velocity.z = -0.75 * movementspeed
			self.linmove.SetVelocity(velocity)
		elif self.autorun or self.forward:
			velocity.x = 0
			velocity.z = -movementspeed
			self.linmove.SetVelocity(velocity)
		elif self.backward and self.straferight:
			velocity.x = -0.75 * movementspeed
			velocity.z = 0.75 * movementspeed
			self.linmove.SetVelocity(velocity)
		elif self.backward and self.strafeleft:
			velocity.x = 0.75 * movementspeed
			velocity.z = 0.75 * movementspeed
			self.linmove.SetVelocity(velocity)
		elif self.backward:
			velocity.x = 0
			velocity.z = movementspeed
			self.linmove.SetVelocity(velocity)
		elif self.straferight:
			velocity.x = -movementspeed
			velocity.z = 0
			self.linmove.SetVelocity(velocity)
		elif self.strafeleft:
			velocity.x = movementspeed
			velocity.z = 0
			self.linmove.SetVelocity(velocity)
		else:
			velocity.x = 0
			velocity.z = 0
			self.linmove.SetVelocity(velocity)

		if self.rotateright:
			self.linmove.SetAngularVelocity(csVector3(0,-1.75,0))
		elif self.rotateleft:
			self.linmove.SetAngularVelocity(csVector3(0,1.75,0))
		else:
			self.linmove.SetAngularVelocity(csVector3(0,0,0))

		if jump and self.linmove.IsOnGround():
			velocity.y = 6.31
			self.linmove.SetVelocity(velocity)

	def pccommandinput_forward1(self,pc,args):
		self.forward = 1
		self.handle_movement(0)
	def pccommandinput_forward0(self,pc,args):
		self.forward = 0
		self.handle_movement(0)

	def pccommandinput_backward1(self,pc,args):
		self.backward = 1
		self.handle_movement(0)
	def pccommandinput_backward0(self,pc,args):
		self.backward = 0
		self.handle_movement(0)

	def pccommandinput_rotateleft1(self,pc,args):
		self.rotateleft = 1
		self.handle_movement(0)
	def pccommandinput_rotateleft0(self,pc,args):
		self.rotateleft = 0
		self.handle_movement(0)

	def pccommandinput_rotateright1(self,pc,args):
		self.rotateright = 1
		self.handle_movement(0)
	def pccommandinput_rotateright0(self,pc,args):
		self.rotateright = 0
		self.handle_movement(0)

	def pccommandinput_strafeleft1(self,pc,args):
		self.strafeleft = 1
		self.handle_movement(0)
	def pccommandinput_strafeleft0(self,pc,args):
		self.strafeleft = 0
		self.handle_movement(0)

	def pccommandinput_straferight1(self,pc,args):
		self.straferight = 1
		self.handle_movement(0)
	def pccommandinput_straferight0(self,pc,args):
		self.straferight = 0
		self.handle_movement(0)

	def pccommandinput_jump1(self,pc,args):
		self.handle_movement(1)
	def pccommandinput_jump0(self,pc,args):
		dummy = 0

	def pccommandinput_run1(self,pc,args):
		if not self.autorun:
			self.running = 1
		self.handle_movement(0)
	def pccommandinput_run0(self,pc,args):
		if not self.autorun:
			self.running = 0
		self.handle_movement(0)

	def pccommandinput_cammode1(self,pc,args):
		self.camera.SetMode(self.camera.GetNextMode ())

