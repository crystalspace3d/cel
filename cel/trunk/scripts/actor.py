from blcelc import *

class actor:
	def __init__(self,celEntity):
		print "Initializing actor..."

	def real_init(self,celEntity,region):
		self.forward = 0
		self.backward = 0
		self.straferight = 0
		self.strafeleft = 0
		self.rotateright = 0
		self.rotateleft = 0
		self.autorun = 0
		self.running = 0

		mesh = celCreateMesh(physicallayer_ptr,celEntity)
		self.linmove = celCreateLinearMovement(physicallayer_ptr,celEntity)
		input = celCreateCommandInput(physicallayer_ptr,celEntity)

		room = region.GetStartSector()
		self.camera = celCreateCamera(physicallayer_ptr,celEntity)
		self.camera.SetRegion(region)
		self.camera.SetMode(self.camera.follow)
		self.camera.SetFollowPos(csVector3(0,1,3),csVector3(0,0,0))

		# @@@ Ugly hardcoding of path!
		mesh.LoadMesh("box", "/this/apps/celtest/data/box")
		pos = csVector3 (0, 1, 0)
		mesh.MoveMesh(room,pos)

		self.linmove.InitCD(csVector3 (-.5,-.5,-.5),csVector3(.5,.5,.5))
		self.linmove.SetReady(1)

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
		self.linmove.GetVelocity(velocity)
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
			self.linmove.SetRotation(csVector3(0,-1.75,0))
		elif self.rotateleft:
			self.linmove.SetRotation(csVector3(0,1.75,0))
		else:
			self.linmove.SetRotation(csVector3(0,0,0))

		if jump and self.linmove.IsOnGround():
			velocity.y = 6.31
			self.linmove.SetVelocity(velocity)

	def pckeyinput_forward1(self,celentity,args):
		self.forward = 1
		self.handle_movement(0)
	def pckeyinput_forward0(self,celentity,args):
		self.forward = 0
		self.handle_movement(0)

	def pckeyinput_backward1(self,celEntity,args):
		self.backward = 1
		self.handle_movement(0)
	def pckeyinput_backward0(self,celEntity,args):
		self.backward = 0
		self.handle_movement(0)

	def pckeyinput_rotateleft1(self,celEntity,args):
		self.rotateleft = 1
		self.handle_movement(0)
	def pckeyinput_rotateleft0(self,celEntity,args):
		self.rotateleft = 0
		self.handle_movement(0)

	def pckeyinput_rotateright1(self,celEntity,args):
		self.rotateright = 1
		self.handle_movement(0)
	def pckeyinput_rotateright0(self,celEntity,args):
		self.rotateright = 0
		self.handle_movement(0)

	def pckeyinput_strafeleft1(self,celEntity,args):
		self.strafeleft = 1
		self.handle_movement(0)
	def pckeyinput_strafeleft0(self,celEntity,args):
		self.strafeleft = 0
		self.handle_movement(0)

	def pckeyinput_straferight1(self,celEntity,args):
		self.straferight = 1
		self.handle_movement(0)
	def pckeyinput_straferight0(self,celEntity,args):
		self.straferight = 0
		self.handle_movement(0)

	def pckeyinput_jump1(self,celEntity,args):
		self.handle_movement(1)

	def pckeyinput_run1(self,celEntity,args):
		if not self.autorun:
			self.running = 1
		self.handle_movement(0)
	def pckeyinput_run0(self,celEntity,args):
		if not self.autorun:
			self.running = 0
		self.handle_movement(0)

	def pckeyinput_cammode1(self,celEntity,args):
		print "cam"

