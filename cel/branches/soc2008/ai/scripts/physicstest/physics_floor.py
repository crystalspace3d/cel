from pycel import *

class physics_floor:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "Initializing floor..."
		self.entity = celEntity
	def CreateGeometry(self,sector):
		mapper = DensityTextureMapper(0.3)
		quad = TesselatedQuad(csVector3(0,0,0),csVector3(0,0,100),csVector3(100,0,0))
		quad.SetLevel(300)
		quad.SetMapper(mapper)
		genquad = GeneralMeshBuilder.CreateFactoryAndMesh(Engine,
				sector,"plane","plane_factory",quad)
		genstate = genquad.GetMeshObject ().QueryInterface(iGeneralMeshState)
		genfactstate = genquad.GetMeshObject ().GetFactory().QueryInterface(iGeneralFactoryState)
		genstate.SetManualColors(True) # XXX doesnt want to lit...
		return genquad
	def CreateLight(self,sector):
		light = Engine.CreateLight("", csVector3 (0, 3, 0),
			100,csColor (1, 1, 1), CS_LIGHT_DYNAMICTYPE_STATIC)
		sector.GetLights().Add(light)
	def real_init(self,sector,dynsys):
		pcmesh = celMesh(self.entity)
		pcmesh.SetMesh(self.CreateGeometry(sector),False)
		wood = Engine.GetMaterialList().FindByName("wood")
		pcmesh.Mesh.GetMeshObject().SetMaterialWrapper(wood)

		pos = csVector3 (-50,-1,-50)
		pcmesh.MoveMesh(sector,pos)

		self.dynbody = celMechanicsObject(self.entity)
		self.dynbody.SetMechanicsSystem(dynsys)
		self.dynbody.AttachColliderPlane(csPlane3(0,-1,0,-1))
		self.dynbody.MakeStatic(1)
		self.CreateLight(sector)

