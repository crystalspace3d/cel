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
		mesh.LoadMesh("ball", "/cellib/objects/ball")
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

	def pckeyinput_forward1(self,celentity,args):
		self.dynbody.AddForceDuration(csVector3(0,0,50),10000000)
	def pckeyinput_forward0(self,celentity,args):
		self.dynbody.ClearForces()

	def pckeyinput_backward1(self,celEntity,args):
		self.dynbody.AddForceDuration(csVector3(0,0,-50),10000000)
	def pckeyinput_backward0(self,celEntity,args):
		self.dynbody.ClearForces()

	def pckeyinput_strafeleft1(self,celEntity,args):
		self.dynbody.AddForceFrame(csVector3(-50,0,0))
	def pckeyinput_strafeleft0(self,celEntity,args):
		dummy = 0

	def pckeyinput_straferight1(self,celEntity,args):
		self.dynbody.AddForceFrame(csVector3(50,0,0))
	def pckeyinput_straferight0(self,celEntity,args):
		dummy = 0

	def pckeyinput_jump1(self,celEntity,args):
		self.dynbody.AddForceDuration(csVector3(0,100,0),0.5)
	def pckeyinput_jump0(self,celEntity,args):
		dummy = 0

