# an example fovcontrol behaviour
# uses: DefaultCamera from player, CommandInput, Timer

from pycel import *

class fovcontrol:
	def __init__(self,celEntity):
		print "Initializing fovcontrol...",celEntity.Name
		self.camera = celGetDefaultCamera(Entities["camera"]).Camera
		self.initfov = self.camera.GetFOV ()
		self.input = celCommandInput(celEntity)
		self.input.LoadConfig("fovcontrol")
		self.timer = celTimer(celEntity)
		self.timer.WakeUpFrame(CEL_EVENT_PRE)
		self.zoomin = False
		self.zoomout = False
		self.time = Clock.GetCurrentTicks()

        def pctimer_wakeupframe(self,celEntity,args):
		self.time = Clock.GetCurrentTicks() - self.time
		if self.zoomin:
			self.camera.SetFOV(int(self.camera.GetFOV()+(5*(self.time/1000.0))),Graphics2D.GetWidth())
		if self.zoomout:
			self.camera.SetFOV(int(self.camera.GetFOV()-(5*(self.time/1000.0))),Graphics2D.GetWidth())
	def pccommandinput_zoomin1(self,celEntity,args):
		self.zoomin = True
	def pccommandinput_zoomout1(self,celEntity,args):
		self.zoomout = True
	def pccommandinput_zoomin0(self,celEntity,args):
		self.zoomin = False
	def pccommandinput_zoomout0(self,celEntity,args):
		self.zoomout = False
	def pccommandinput_zoom01(self,celEntity,args):
		self.camera.SetFOV(self.initfov,Graphics2D.GetWidth())
	def pccommandinput_zoom00(self,celEntity,args):
		pass
	def pccommandinput_zoomin_(self,celEntity,args):
		pass
	def pccommandinput_zoom0_(self,celEntity,args):
		pass
	def pccommandinput_zoomout_(self,celEntity,args):
		pass

