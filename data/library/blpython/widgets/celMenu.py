from pycel import *

#The glue module which abstracts the widgets into a menu. Allows one widget to have focus.
#You really want to use this module to use the widgets.


def GetFontConstant():
    return Graphics2D.GetHeight() / 75.0

def GetFontColor():
    return csColor(0.9, 0.9, 0.9)

class celMenu:
    api_version = 2 # use new version of message callbacks.
    def __init__(self, owner):
        self.elements = []
        self.blpython = BehaviourLayers['blpython']
        self.owner = owner
        self.font = '/outlaws/fonts/lcd2.ttf'
    
    #Add a menu element. sends message to menu when clicked. the behaviour chooses the widget.
    def addElement(self, name, message, position, sizes, fsize, material, behaviour = 'celButton'):
        elementEntity = celCreateEntity(pl, name)
        pcbillboard = celBillboard(elementEntity)
        buttonbb = pcbillboard.Billboard
        buttonbb.SetMaterialName(material)
        buttonbb.SetSize(sizes[0], sizes[1])
        buttonbb.SetPosition(position[0], position[1])
        buttonbb.SetTextOffset(1500, 1500)
        buttonbb.SetTextFgColor(GetFontColor())
        buttonbb.SetTextFont(self.font, fsize)
        buttonbb.SetText(name)
        params=parblock({'message' : message, 'owner': self.owner})
        if message != '':
            elementEntity.CreateBehaviour(self.blpython, behaviour)
            pcbillboard.EnableEvents(True)
            elementEntity.Behaviour.SendMessage('setparameters', None, params)
        self.elements.append(elementEntity)
        return elementEntity
        
    #Given an [x,y] list for position, and spacing, arrange elements 
    def align(self, position, spacing):
        for i, element in enumerate(self.elements):
                bb = celBillboard(element).Billboard
                bb.SetPosition(position[0] + (spacing[0]*i),position[1]+(spacing[1]*i))
    
    #Remove all elements from menu
    def clear(self):
        for i in xrange(len(self.elements)):
            element = self.elements.pop(0)
            element.PropertyClassList.RemoveAll()
            RemoveEntity(element)
    
    #make one selected
    def updateSelect(self, selected):
        index = 0
        for i, element in enumerate(self.elements):
            if element.Name == selected:
                index = i
            bb = celBillboard(element).Billboard
            bb.SetMaterialName('button-bg')
        
        bb = celBillboard(self.elements[index]).Billboard
        bb.SetMaterialName('button-bg')
        return index
    
    #Activate an item in the menu
    def activate(self, name):
        for element in self.elements:
            if element.Name == name:
                if element.Behaviour:
                    element.Behaviour.SendMessage('setactive', None, celGenericParameterBlock(0))
            else:
                if element.Behaviour:
                    element.Behaviour.SendMessage('setinactive', None, celGenericParameterBlock(0))

    #Deactivate all elements
    def deactivateAll(self):
        for element in self.elements:
            if element.Behaviour:
                element.Behaviour.SendMessage('setinactive', None, celGenericParameterBlock(0))
