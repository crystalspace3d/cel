# Python behaviour that can be used as a base to control a camera
# whose player does not move.
# Uses: defaultcamera,commandinput,actormove,linmove

from pycel import *

class cameracontrol:
	api_version = 2 # use new version of message callbacks.
	# INITIALIZATION
	def __init__(self,celEntity):
		print "Initializing cameracontrol...",celEntity.Name
		# create/get pcdefaultcamera
		self.camera = celDefaultCamera(celEntity)
		# check for pcdefaultcamera
		input = celCommandInput(celEntity)
		# check for actormove
		self.actormove = celActorMove(celEntity)

		# bind some keys
		input.LoadConfig("cameracontrol")
	
	# ENTITY CALLBACKS
	# the engine will call this functions in response
	# to certain events. 
	
	# pctimer wakeup callback
	def pctimer_wakeup (self,pc,args):
		pass
		
	# walk callbacks. note that for each bind defined in
	# pccommandinput there are three functions calles:
	# pccommandinput_(bindcode)1     -> Key down event
	# pccommandinput_(bindcode)0     -> Key release event
	# pccommandinput_(bindcode)_     -> Key hold events
	# ROTATE
	def pccommandinput_rotateleft1(self,pc,args):
		self.actormove.RotatingLeft = 1
	def pccommandinput_rotateleft0(self,pc,args):
		self.actormove.RotatingLeft = 0
	def pccommandinput_rotateleft_(self,pc,args):
		pass
	def pccommandinput_rotateright1(self,pc,args):
		self.actormove.RotatingRight = 1
	def pccommandinput_rotateright0(self,pc,args):
		self.actormove.RotatingRight = 0
	def pccommandinput_rotateright_(self,pc,args):
		pass

	# LOOK UP AND DOWN
	def pccommandinput_lookup1(self,pc,args):
		self.camera.PitchVelocity = 1.0

	def pccommandinput_lookup0(self,pc,args):
		self.camera.PitchVelocity = 0.0

	def pccommandinput_lookdown1(self,pc,args):
		self.camera.PitchVelocity = -1.0

	def pccommandinput_lookdown0(self,pc,args):
		self.camera.PitchVelocity = 0.0

	# CAMERA MODES
	def pccommandinput_cammode1(self,pc,args):
		self.camera.Mode = self.camera.GetNextMode ()

