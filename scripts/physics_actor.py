from blcelc import *

class physics_actor:
	def __init__(self,celEntity):
		print "Initializing actor..."

	def real_init(self,celEntity,region,dynsys):
		self.forward = 0
		self.backward = 0
		self.straferight = 0
		self.strafeleft = 0
		self.rotateright = 0
		self.rotateleft = 0
		self.autorun = 0
		self.running = 0

		self.angle = 0

		mesh = celCreateMesh(physicallayer_ptr,celEntity)
		# @@@ Ugly hardcoding of path!
		mesh.LoadMesh("ball", "/this/apps/celtest/data/ball")
		ballstate = SCF_QUERY_INTERFACE(mesh.GetMesh().GetMeshObject(),
			iBallState)
		ballstate.SetRadius(.5,.5,.5)
		engine = CS_QUERY_REGISTRY(object_reg, iEngine)
		material = engine.GetMaterialList().FindByName("wood")
		ballstate.SetMaterialWrapper(material)

		pos = csVector3 (0, 2, 0)
		room = region.GetStartSector()
		mesh.MoveMesh(room,pos)

		self.dynbody = celCreateDynamicBody(physicallayer_ptr,celEntity)
		self.dynbody.SetDynamicSystem(dynsys)
		self.dynbody.SetParameters(0.9,1.0,0.0,0.0,5.0)
		trans = mesh.GetMesh().GetMovable().GetFullTransform()
		self.dynbody.AttachColliderSphere(.5, csVector3(0,0,0))
		self.rigidbody = self.dynbody.GetBody()
		input = celCreateCommandInput(physicallayer_ptr,celEntity)

		self.camera = celCreateCamera(physicallayer_ptr,celEntity)
		self.camera.SetRegion(region)
		self.camera.SetMode(self.camera.freelook)
		self.camera.GetCamera().GetTransform().SetOrigin(csVector3(0,0,-5))
		self.camera.GetCamera().GetTransform().LookAt(csVector3(0,0,1),csVector3(0,1,0))
		#self.camera.SetFollowPos(csVector3(0,1,3),csVector3(0,0,0))
		self.camera.SetClearScreen(1)
		self.camera.SetClearZBuffer(1)

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
			movementspeed = 50.0
		else:
			movementspeed = 20.0
		velocity = csVector3 (0, 0, 0)
		if (self.autorun or self.forward) and self.straferight:
			velocity.x = -0.75 * movementspeed
			velocity.z = -0.75 * movementspeed
		elif (self.autorun or self.forward) and self.strafeleft:
			velocity.x = 0.75 * movementspeed
			velocity.z = -0.75 * movementspeed
		elif self.autorun or self.forward:
			velocity.x = 0
			velocity.z = -movementspeed
		elif self.backward and self.straferight:
			velocity.x = -0.75 * movementspeed
			velocity.z = 0.75 * movementspeed
		elif self.backward and self.strafeleft:
			velocity.x = 0.75 * movementspeed
			velocity.z = 0.75 * movementspeed
		elif self.backward:
			velocity.x = 0
			velocity.z = movementspeed
		elif self.straferight:
			velocity.x = -movementspeed
			velocity.z = 0
		elif self.strafeleft:
			velocity.x = movementspeed
			velocity.z = 0
		else:
			velocity.x = 0
			velocity.z = 0
		self.rigidbody.AddForce(velocity)

		if self.rotateright:
			self.angle += 5
		elif self.rotateleft:
			self.angle -= 5
		print self.angle
		self.camera.SetRoll(self.angle)

		#if jump and self.linmove.IsOnGround():
			#velocity.y = 6.31
			#self.linmove.SetVelocity(velocity)

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
	def pckeyinput_jump0(self,celEntity,args):
		dummy = 0

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

