from pycel import *
import celMenu
class celTextInput:
    api_version = 2 # use new version of message callbacks.
    def __init__(self,celEntity):
        self.message = ''
        self.owner = None
        self.entity = celEntity
        self.active = False
        self.index = 0
        self.bb = celBillboard(self.entity).Billboard
        self.bb.SetText('')
        #Keep the text in a list so we can do insertion at cursor position
        self.text = []
        self.pcinput = celCommandInput(self.entity)
        self.pcinput.Bind('key', 'keypress')
        self.pcinput.SetCookedMode(True)
        self.pcinput.SetSendTrigger(True)
            
    def pcbillboard_select(self,pc,args):
        params = parblock({'sender':self.entity.Name})
        if self.owner:
            self.owner.Behaviour.SendMessage(self.message, None, params)
            
    def pcbillboard_unselect(self,pc,args):
        pass
    
    def pcbillboard_doubleclick(self, pc, args):
        pass
    
    def setactive(self, pc, args):
        if not self.active:
            self.active = True
            text = self.bb.GetText()
            if text:
                if len(text) > 0:
                    self.text = list(text)
                    self.index = len(text)
                else:
                    self.text = []
            #Insert the cursor
            newtext = ''
            for i, letter in enumerate(self.text):
                #Insert the cursor at right position
                if i is self.index:
                    newtext += '|'
                newtext += letter
            #Also allow cursor to be at the end
            if self.index is len(newtext):
                newtext += '|'
            self.bb.SetText(newtext)
        
    def setinactive(self, pc, args):
        if self.active:
            self.active = False
            #Remove the cursor
            newtext = ''
            for i, letter in enumerate(self.text):
                newtext += letter
            self.bb.SetText(newtext)
    
    def setparameters(self,pc,args):
        self.message = args[getid('cel.parameter.message')]
        self.owner = args[getid('cel.parameter.owner')]
        
    def pccommandinput_keypress1(self, pc, args):
        trigger = args[getid('cel.parameter.trigger')]
        if self.active:
            self.handlekey(trigger)
   
    def handlekey(self, trigger):
        #Check if shift is pressed
        shifttest = trigger[1:]
        if shifttest.startswith('Shift'):
            key = trigger[7:]
            if key in 'abcdefghijklmnopqrstuvwxyz' and len(key) > 0:
                trigger = key.upper()
                
        #Move the cursor
        if trigger == 'Left' and self.index > 0:
            self.index -= 1
        if trigger == 'Right' and self.index < len(self.text):
            self.index += 1
         
        #Insert a character
        elif trigger in 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ':
            self.text.insert(self.index, trigger)
            self.index += 1
        #Insert a space
        elif trigger == 'Space':
            self.text.insert(self.index, ' ')
            self.index += 1
        #Delete a character
        elif trigger == 'Back':
            if len(self.text) > 0:
                self.index -= 1
                self.text.pop(self.index)
        newtext = ''
        for i, letter in enumerate(self.text):
            #Insert the cursor at right position
            if i is self.index:
                newtext += '|'
            newtext += letter
        #Also allow cursor to be at the end
        if self.index is len(newtext):
            newtext += '|'
        self.bb.SetText(newtext)
        
    def pccommandinput_keypress0(self, pc, args):
        pass
    
    def pccommandinput_keypress_(self, pc, args):
        trigger = args[getid('cel.parameter.trigger')]
        if self.active:
            self.handlekey(trigger)
    
    def destruct(self, pc, args):
        self.entity.PropertyClassList.RemoveAll()
