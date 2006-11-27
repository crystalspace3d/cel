# an example sound playing behaviour, plays the sound on collision.
from pycel import *

class crashsound:
	def __init__(self,celEntity):
		print "Initializing playsound...",celEntity.Name
		# init sound
		pcsnd = celGetSoundSource(celEntity)
		if not pcsnd:
			print "No sound in this entity"
			return
		self.snd = pcsnd.SoundSource
		self.snd.SetVolume(1.0)
		self.stream = self.snd.GetStream()
		self.stream.SetLoopState(False)
		self.stream.Pause()
	def pccommandinput_soundup1(self,celEntity,args):
		if self.stream.GetPauseState() == CS_SNDSYS_STREAM_PAUSED:
			self.timer.Clear()
			self.stream.ResetPosition()
			self.stream.Unpause()
		self.counter+=0.1
		if self.counter >= 2.0:
			self.counter = 2.0
		self.snd.SetVolume(self.counter)
	def pccommandinput_soundup0(self,celEntity,args):
		self.timer.WakeUpFrame (CEL_EVENT_PRE)
	def pccommandinput_soundup_(self,celEntity,args):
		self.pccommandinput_soundup1(celEntity,args)
	def pctimer_wakeupframe(self,celEntity,args):
		self.counter-=0.001
		self.snd.SetVolume(self.counter)
		if self.counter < 0.0:
			self.stream.Pause()
			self.timer.Clear()
	def pcdynamicbody_collision(self,celEntity,args):
		print "COLLISION SOUND"
		if self.stream.GetPauseState() == CS_SNDSYS_STREAM_PAUSED:
			self.stream.ResetPosition()
			self.stream.Unpause()

