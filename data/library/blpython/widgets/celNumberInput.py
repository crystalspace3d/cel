from pycel import *
import celMenu
class celNumberInput:
    def __init__(self,celEntity):
        self.message = ''
        self.owner = None
        self.entity = celEntity
        self.active = False
        self.index = 0
        self.bb = celBillboard(self.entity).Billboard
        self.bb.SetText('')
        #Keep the text in a list so we can do insertion at cursor position
        self.text = ['0']
        self.pcinput = celCommandInput(self.entity)
        self.pcinput.Bind('key', 'keypress')
        self.pcinput.SetCookedMode(True)
        self.pcinput.SetSendTrigger(True)
        
        fconst = celMenu.GetFontConstant()
        pos = self.bb.GetPosition()
        size = self.bb.GetSize()
        self.bb.SetTextOffset(size[0] / 2 - 2000, 1000)
        self.bb.SetText('0')
        #Add the directional arrows
        self.arrows = celMenu.celMenu(self.entity)
        self.arrows.addElement('<', 'left_click', pos, [6000, 6000], fconst * 0.8, 'unselection')
        self.arrows.addElement('>', 'right_click', [pos[0] + size[0] - 3000, pos[1]], [6000, 6000], fconst * 0.8, 'unselection')
            
    def pcbillboard_select(self,celEntity,args):
        params = parblock({'sender':self.entity.Name})
        if self.owner:
            self.owner.Behaviour.SendMessage(self.message, None, params)
            
    def pcbillboard_unselect(self,celEntity,args):
        pass
    
    def pcbillboard_doubleclick(self, celEntity, args):
        pass
    
    def setactive(self, celEntity, args):
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
        
    def setinactive(self, celEntity, args):
        if self.active:
            self.active = False
            #Remove the cursor
            newtext = ''
            for i, letter in enumerate(self.text):
                newtext += letter
            self.bb.SetText(newtext)
    
    def setparameters(self,celEntity,args):
        self.message = args[getid('cel.parameter.message')]
        self.owner = args[getid('cel.parameter.owner')]
        
    def pccommandinput_keypress1(self, celEntity, args):
        trigger = args[getid('cel.parameter.trigger')]
        if self.active:
            self.handlekey(trigger)
   
    def handlekey(self, trigger):
        #Move the cursor
        if trigger == 'Left' and self.index > 0:
            self.index -= 1
        if trigger == 'Right' and self.index < len(self.text):
            self.index += 1
         
        #Insert a character
        elif trigger in '1234567890':
            self.text.insert(self.index, trigger)
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
        
    def pccommandinput_keypress0(self, celEntity, args):
        pass
    
    def pccommandinput_keypress_(self, celEntity, args):
        trigger = args[getid('cel.parameter.trigger')]
        if self.active:
            self.handlekey(trigger)
    
    def destruct(self, celEntity, args):
        self.arrows.clear()
        self.entity.PropertyClassList.RemoveAll()
    
    def left_click(self, celEntity, args):
        newtext = ''
        for i, letter in enumerate(self.text):
            newtext += letter
        newint = int(newtext) - 1
        self.text = list(str(newint))
        self.bb.SetText(str(newint))
        
    def right_click(self, celEntity, args):
        newtext = ''
        for i, letter in enumerate(self.text):
            newtext += letter
        newint = int(newtext) + 1
        self.text = list(str(newint))
        self.bb.SetText(str(newint))