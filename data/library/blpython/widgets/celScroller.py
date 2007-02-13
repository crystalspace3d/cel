from pycel import *
import celMenu

#The scroller is not meant to be used as part of a menu. It sort of is a menu
#already.
class celScroller:
    def __init__(self, owner, pos, dims):
        self.items = []
        self.names = []
        self.blpython = BehaviourLayers['blpython']
        self.pos = pos
        self.dims = dims
        self.owner = owner
        self.fconst = celMenu.GetFontConstant()
        self.fcolor = celMenu.GetFontColor()
        self.selectedname = ''
        self.selectedindex = 0
        self.spacing = 10000
         
    def additem(self, name, message):
        #Check that its not a dupe
        if name not in self.names:
            button = pl.CreateEntity(name, self.blpython, 'celButton')
            pars = parblock({'owner' : self.owner, 'message' : message})
            button.Behaviour.SendMessage('setparameters', None, pars)
            pcbb = celBillboard(button)
            pcbb.EnableEvents(True)
            buttonbb = pcbb.Billboard
            buttonbb.SetPosition(self.pos[0], self.pos[1] + len(self.items) * self.spacing)
            buttonbb.SetSize(self.dims[0], self.spacing)
            buttonbb.SetMaterialName('unselection')
            buttonbb.SetTextOffset(3000, 2000)
            buttonbb.SetTextFgColor(self.fcolor)
            buttonbb.SetText(name)
            buttonbb.SetTextFont('/outlaws/fonts/lcd2.ttf', self.fconst)
            self.items.append(button)
            self.names.append(name)
        
    def clear(self):
        self.names = []
        for i in xrange(len(self.items)):
            item = self.items.pop(0)
            RemoveEntity(item)
            
    #Select an item by its name.
    def selectname(self, name):
        self.selectedname = name
        for i, item in enumerate(self.items):
            bb = celBillboard(item).Billboard
            if bb.GetText() == name:
                self.selectedindex = i
                bb.SetMaterialName('selection')
            else:
                bb.SetMaterialName('unselection')
    
    #Select an item by its index.
    def selectindex(self, index):
        self.selectedindex = index
        for i, item in enumerate(self.items):
            bb = celBillboard(item).Billboard
            if index == i:
                bb.SetMaterialName('selection')
                self.selectedname = bb.GetText()
            else:
                bb.SetMaterialName('unselection')
