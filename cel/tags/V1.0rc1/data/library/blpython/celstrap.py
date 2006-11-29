#! BLCEL
# An application that can be run from bootstrap and receives
# the world as a parameter from the command line.

# It is intended for zipping with the world and then running
# as follows:
#
# export PYTHONPATH=_full_path_to_zipfile_.zip
# $(CEL)/bootstrap cel.behaviourlayer.python celstrap _full_path_to_world_.zip
# 
# although it can also run if celstrap.py and all other scripts are
# in the pythonpath (for example ($PWD)/scripts/)
# blender2crystal will be able to run using this from the RUN button.

# Usually you won't need to touch this file, but it is as easy to add
# new functionality to it.
# The file needs to evolve

# This is the list of property classes the app is registering.
# Add any new ones here.

from pycel import *

pcclasses = ["region","tooltip","mesh","solid","meshselect","test","zonemanager","trigger","quest","light","inventory","defaultcamera","gravity","movable","pccommandinput","linmove","actormove","colldet","timer","mechsys","mechobject","soundlistener","soundsource","billboard","properties","hover","craft","mechjoint","mover"]

# The class that implements the main application behaviour.
# if you want to make your own just copy it and set the same name for
# the file as for the class. ie: celstrap.py has class celstrap.

# To use this the player must have a camera property class and must be 
# called "camera" :(. Have to find a better way to manage the player.

class celstrap:
	def __init__(self,celEntity):
		print "Initializing game..."
		self.entity=celEntity
		# Register property classes
		for pcclass in pcclasses:
			PcFactories.append("cel.pcfactory."+pcclass)
		# Initialize app entity
		self.entity = celEntity
		# key bindings
		self.input = celCommandInput(self.entity)
		self.input.LoadConfig("Bootstrap")

		self.loadMovieRecorder()
		# graphic interfaces
		self.font = Graphics2D.GetFontServer ().LoadFont("*large")
		self.timer = celTimer(celEntity)
		# check for tooltip
		self.logs=[]
		# the zone manager entity
		self.game_entity=CreateEntity("ZoneManager",None,None)

		# create the tooltip for logs
		self.createTooltip()
		self.do_save=False
		self.map = "level.xml"
		self.loadMap(self.map)
		print "End initializing game..."

	def loadMovieRecorder(self):
		# load the movierecorder
		self.mr = CS_QUERY_REGISTRY(oreg,iMovieRecorder)
		if not self.mr:
			plugmgr = CS_QUERY_REGISTRY(object_reg_ptr,iPluginManager)
			self.mr = CS_LOAD_PLUGIN (plugmgr, "crystalspace.utilities.movierecorder",iMovieRecorder)
			if self.mr:
				object_reg_ptr.Register (self.mr, "iMovieRecorder")
			else:
				self.mr = None

	# doing it better
	def createTooltip(self):
		self.tooltip = celToolTip(self.entity)
		self.tooltip.SetBackgroundColor(-1,-1,-1)
		self.tooltip.SetTextColor(255,255,255)
		self.tooltip.Show(20,20)
		self.log_event("celstart started")
		self.log_event("i: start record")
		self.log_event("o: stop record")
		self.log_event("p: pause record")

	# TODO this way to load the map is not very nice, but
	# it is the same celtest uses, i don't know about
	def loadMap(self,map_file):
		if (not Graphics3D.BeginDraw (CSDRAW_2DGRAPHICS)):
			return
		Graphics2D.Clear(0)
		Graphics2D.Write(self.font,100,100,Graphics2D.FindRGB(255,255,255),Graphics2D.FindRGB(0,0,0),"Loading... "+str(map_file))
		Graphics2D.Print(csRect(0,0,640,480))
		Graphics3D.FinishDraw ()
		# XXX ugly hardcoded name for the player
		self.playername="camera"
		# create an initial environment to load the map
		zoneManager = celZoneManager(self.game_entity)
		Vfs.ChDirAuto("/tmp/celstart/");
		zoneManager.Load("/tmp/celstart",map_file)
		# the map won't load until we put a dummy camera into
		# the region
		dummy = CreateEntity("dcamera",None,None)
		dummy_cam = celDefaultCamera(dummy)
		self.startregion=zoneManager.LastStartRegionName
		self.startname=zoneManager.LastStartName
		dummy_cam.SetZoneManager(zoneManager,True,self.startregion,"")
		
		# now the map is loaded we want to find the real
		# player entity and do some setup.
		actor = Entities[self.playername]
		if celGetMesh(actor):
			zoneManager.PointMesh(self.playername,self.startregion,self.startname)
		else:
			zoneManager.PointCamera(self.playername,self.startregion,self.startname)
		# kill the dummy camera
		RemoveEntity(dummy)

	# video record commands
	def pccommandinput_startrecord1(self,celEntity,args):
		self.log_event("start recording")
		if self.mr:
			self.mr.Start()
	def pccommandinput_stoprecord1(self,celEntity,args):
		self.log_event("stop recording")
		if self.mr:
			self.mr.Stop()
	def pccommandinput_pauserecord1(self,celEntity,args):
		if self.mr:
			if self.mr.IsPaused():
				self.log_event("unpause recording")
				self.mr.UnPause()
			else:
				self.log_event("pause recording")
				self.mr.Pause()
	def log_event(self,event):
		self.logs.append(event)
		if len(self.logs) > 5:
			self.logs.pop(0)
		tooltip_text="\n".join(self.logs)
		self.tooltip.Show(20,20)
		self.tooltip.SetText(tooltip_text)

	# this are callbacks for pczonemanager (not used at the moment
	# as the zonemanager is a separate entity)
	#def pczonemanager_startloading(self,celEntity,args):
	#	self.log_event("start loading")
	#def pczonemanager_stoploading(self,celEntity,args):
	#	self.log_event("stop loading")
	#def pczonemanager_addregion(self,celEntity,args):
	#	self.log_event("add region")
	#def pczonemanager_remregion(self,celEntity,args):
	#	self.log_event("remove region")

	# starting command, here we can pass map name, player name...
	def load(self,celEntity,args):
		# mount the file at some path in vfs
		self.map = args[getid("cel.parameter.parameter1")]
		#vfs.ChDirAuto("/celstrap/");
		self.loadMap(self.map)
	
