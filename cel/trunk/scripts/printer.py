from blcel import *

class printer:
	def __init__(self,celEntity):
		print "entity.__init__"
	def pctimer_wakeup(self,entityptr,args):
		print "printer.pctimer_wakeup"
	def pcinventory_added(self,entityptr,args):
		print "printer.pcinventory_added"


