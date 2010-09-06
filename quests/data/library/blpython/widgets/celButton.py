from pycel import *
import celMenu
class celButton:
    api_version = 2 # use new version of message callbacks.
    def __init__(self,celEntity):
        self.message = ''
        self.owner = None
        self.entity = celEntity
        self.active = True
            
    def pcbillboard_select(self,pc,args):
        if self.active:
            params = parblock({'sender':self.entity.Name})
            if self.owner:
                self.owner.Behaviour.SendMessage(self.message, None, params)
            
    def pcbillboard_unselect(self,pc,args):
        pass
    
    def pcbillboard_doubleclick(self, pc, args):
        pass
    
    def setparameters(self,pc,args):
        self.message = args[getid('message')]
        self.owner = args[getid('owner')]

    def setactive(self, pc, args):
        if not self.active:
            self.active = True
            bb = celBillboard(self.entity).Billboard
            bb.SetTextFgColor(celMenu.GetFontColor())
    
    def setinactive(self, pc, args):
        if self.active:
            self.active = False
            bb = celBillboard(self.entity).Billboard
            bb.SetTextFgColor(csColor(0, 0, 0))
    
    def destruct(self, pc, args):
        pass
