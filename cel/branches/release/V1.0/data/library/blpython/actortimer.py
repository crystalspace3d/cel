# Python behaviour that can be used to print passed time.
# Uses: timer,tooltip

from pycel import *

class actortimer:
	# INITIALIZATION
	def __init__(self,celEntity):
		print "Initializing actortimer...",celEntity.Name
		# create/get timer and tooltip
		self.timer = celTimer(celEntity)
		self.tooltip = celToolTip(celEntity)
		self.timer.WakeUp (100,True)
		self.tooltip.SetBackgroundColor(-1,-1,-1)
		self.tooltip.SetTextColor(255,255,255)
		# bind some keys
		self.time = Clock.GetCurrentTicks()
	
	# ENTITY CALLBACKS
	# the engine will call this functions in response
	# to certain events. 
	def format_time(self,time):
		timei = int(time)
		min = timei/60
		sec = timei%60
		rest = time-(min*60)-sec
		return str(min)+":"+str(sec)+":"+str(rest)

	# pctimer wakeup callback
	def pctimer_wakeup (self,celentity,args):
		self.tooltip.Show(100,100)
		time = (Clock.GetCurrentTicks()-self.time)/1000.0
		self.tooltip.SetText("time: "+self.format_time(time))
	
