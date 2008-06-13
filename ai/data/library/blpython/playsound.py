# an example sound playing behaviour
from pycel import *

class playsound:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "Initializing playsound...",celEntity.Name
		# init sound
		pcsnd = celGetSoundSource(celEntity)
		if not pcsnd:
			print "No sound in this entity"
			return
		self.snd = pcsnd.SoundSource
		self.snd.SetVolume(0.0)
		self.stream = self.snd.GetStream()
		self.stream.SetLoopState(True)
		# init timer
		self.timer = celTimer(celEntity)
		# init commandinput
		input = celCommandInput(celEntity)
		input.LoadConfig("playsound")
		# counter variables
		self.counter = 0
	def pccommandinput_soundup1(self,pc,args):
		if self.stream.GetPauseState() == CS_SNDSYS_STREAM_PAUSED:
			self.timer.Clear()
			self.stream.ResetPosition()
			self.stream.Unpause()
		self.counter+=0.1
		if self.counter >= 1.0:
			self.counter = 1.0
		self.snd.SetVolume(self.counter)
	def pccommandinput_soundup0(self,pc,args):
		self.timer.WakeUpFrame (CEL_EVENT_PRE)
	def pccommandinput_soundup_(self,pc,args):
		self.pccommandinput_soundup1(pc,args)
	def pctimer_wakeupframe(self,pc,args):
		self.counter-=0.001
		self.snd.SetVolume(self.counter)
		if self.counter < 0.0:
			self.stream.Pause()
			self.timer.Clear()
	def pcdynamicbody_collision(self,pc,args):
		pass

