# an example dummy behaviour (for a billboard)

class dummy:
	api_version = 2 # use new version of message callbacks.
	def __init__(self,celEntity):
		print "Initializing dummy..."
	def real_init(self,pc,room):
		pass
	def pcbillboard_select(self,pc,args):
		print "bill select"
	def pcbillboard_move(self,pc,args):
		print "bill select"
