from blcelc import *

class actor:
	def __init__(self,celEntity):
		print "Initializing actor..."
		print self
		print celEntity
		timer = celCreateTimer(physicallayer_ptr,celEntity)
		timer.WakeUp(2000,1)

	def real_init(self,celEntity,room):
		print self
		print celEntity
		#mesh = celCreateMesh(physicallayer_ptr,celEntity)
		#linmove = celCreateLinearMovement(physicallayer_ptr,celEntity)
		#input = celCreateCommandInput(physicallayer_ptr,celEntity)
		#timer = celCreateTimer(physicallayer_ptr,celEntity)

		#timer.WakeUp(2000,1)

		# @@@ Ugly hardcoding of path!
		#mesh.LoadMesh("box", "/this/apps/celtest/data/box")
		#pos = csVector3 (0, 0, 0)
		#mesh.MoveMesh(room,pos)

		#input.Bind("up", "forward")
		#input.Bind("down", "backward")
		#input.Bind("shift", "run")
		#input.Bind("m", "cammode")

	def pctimer_wakeup(self,celentity,args):
		print 'wakeup'

	def pckeyinput_forward1(self,celentity,args):
		print 'forward1'
	def pckeyinput_forward0(self,celentity,args):
		print "forward0"
	#def pckeyinput_backward1(self,celEntity,args):
		#print "backward"
