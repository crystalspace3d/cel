from pycel import *

class physics_actor:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "Initializing actor..."
		self.entity = celEntity

	def real_init(self,sector,dynsys):
		self.angle = 0
		pcmesh = celMesh(self.entity)
		# @@@ Ugly hardcoding of path!
		pcmesh.SetMesh("ball", "/cellib/objects/ball")
		material = Engine.GetMaterialList().FindByName("wood")
		pcmesh.Mesh.GetMeshObject().SetMaterialWrapper(material)

		pos = csVector3 (0, 2, 0)
		pcmesh.MoveMesh(sector,pos)

		self.dynbody = celMechanicsObject(self.entity)
		self.dynbody.SetMechanicsSystem(dynsys)
		self.dynbody.AttachColliderSphere(.5, csVector3(0,0,0))

		self.camera = celDefaultCamera(self.entity)
		self.camera.SetMode(self.camera.freelook)
		self.camera.SetClearScreen(1)
		self.camera.SetClearZBuffer(1)

		input = celCommandInput(self.entity)
		input.Bind("up", "forward")
		input.Bind("down", "backward")
		input.Bind("left", "strafeleft")
		input.Bind("right", "straferight")
		input.Bind("shift", "run")
		input.Bind("m", "cammode")
		input.Bind(" ", "jump")

	def pccommandinput_forward1(self,pc,args):
		self.dynbody.AddForceDuration(csVector3(0,0,-50),False,csVector3(0),10000000)
	def pccommandinput_forward0(self,pc,args):
		self.dynbody.ClearForces()

	def pccommandinput_backward1(self,pc,args):
		self.dynbody.AddForceDuration(csVector3(0,0,50),False,csVector3(0),10000000)
	def pccommandinput_backward0(self,pc,args):
		self.dynbody.ClearForces()

	def pccommandinput_strafeleft1(self,pc,args):
		self.dynbody.AddForceDuration(csVector3(20,0,0),False,csVector3(0),0.5)
	def pccommandinput_straferight1(self,pc,args):
		self.dynbody.AddForceDuration(csVector3(-20,0,0),False,csVector3(0),0.5)
	def pccommandinput_jump1(self,pc,args):
		self.dynbody.AddForceDuration(csVector3(0,100,0),False,csVector3(0),0.5)

