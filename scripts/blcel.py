# This file was created automatically by SWIG.
import blcelc
class iBase:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iBase):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iBase.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iBase.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def DecRef(*args): return apply(blcelc.iBase_DecRef,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iBase,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iBase(self)
    def __repr__(self):
        return "<C iBase instance at %s>" % (self.this,)

class iBasePtr(iBase):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iBase
blcelc.iBase_swigregister(iBasePtr)
class iSCF(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iSCF):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iSCF.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iSCF.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def CreateInstance(*args): return apply(blcelc.iSCF_CreateInstance,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iSCF,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iSCF(self)
    def __repr__(self):
        return "<C iSCF instance at %s>" % (self.this,)

class iSCFPtr(iSCF):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iSCF
blcelc.iSCF_swigregister(iSCFPtr)
class csVector3:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,csVector3):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = csVector3.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = csVector3.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    __setmethods__["x"] = blcelc.csVector3_x_set
    __getmethods__["x"] = blcelc.csVector3_x_get
    __setmethods__["y"] = blcelc.csVector3_y_set
    __getmethods__["y"] = blcelc.csVector3_y_get
    __setmethods__["z"] = blcelc.csVector3_z_set
    __getmethods__["z"] = blcelc.csVector3_z_get
    def __init__(self,*args):
        self.this = apply(blcelc.new_csVector3,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_csVector3(self)
    def __repr__(self):
        return "<C csVector3 instance at %s>" % (self.this,)

class csVector3Ptr(csVector3):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = csVector3
blcelc.csVector3_swigregister(csVector3Ptr)
class csRGBpixel:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,csRGBpixel):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = csRGBpixel.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = csRGBpixel.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    __setmethods__["red"] = blcelc.csRGBpixel_red_set
    __getmethods__["red"] = blcelc.csRGBpixel_red_get
    __setmethods__["green"] = blcelc.csRGBpixel_green_set
    __getmethods__["green"] = blcelc.csRGBpixel_green_get
    __setmethods__["blue"] = blcelc.csRGBpixel_blue_set
    __getmethods__["blue"] = blcelc.csRGBpixel_blue_get
    __setmethods__["alpha"] = blcelc.csRGBpixel_alpha_set
    __getmethods__["alpha"] = blcelc.csRGBpixel_alpha_get
    def __init__(self,*args):
        self.this = apply(blcelc.new_csRGBpixel,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_csRGBpixel(self)
    def __repr__(self):
        return "<C csRGBpixel instance at %s>" % (self.this,)

class csRGBpixelPtr(csRGBpixel):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = csRGBpixel
blcelc.csRGBpixel_swigregister(csRGBpixelPtr)
class csPixelFormat:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,csPixelFormat):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = csPixelFormat.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = csPixelFormat.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    __setmethods__["RedMask"] = blcelc.csPixelFormat_RedMask_set
    __getmethods__["RedMask"] = blcelc.csPixelFormat_RedMask_get
    __setmethods__["GreenMask"] = blcelc.csPixelFormat_GreenMask_set
    __getmethods__["GreenMask"] = blcelc.csPixelFormat_GreenMask_get
    __setmethods__["BlueMask"] = blcelc.csPixelFormat_BlueMask_set
    __getmethods__["BlueMask"] = blcelc.csPixelFormat_BlueMask_get
    __setmethods__["RedShift"] = blcelc.csPixelFormat_RedShift_set
    __getmethods__["RedShift"] = blcelc.csPixelFormat_RedShift_get
    __setmethods__["GreenShift"] = blcelc.csPixelFormat_GreenShift_set
    __getmethods__["GreenShift"] = blcelc.csPixelFormat_GreenShift_get
    __setmethods__["BlueShift"] = blcelc.csPixelFormat_BlueShift_set
    __getmethods__["BlueShift"] = blcelc.csPixelFormat_BlueShift_get
    __setmethods__["RedBits"] = blcelc.csPixelFormat_RedBits_set
    __getmethods__["RedBits"] = blcelc.csPixelFormat_RedBits_get
    __setmethods__["GreenBits"] = blcelc.csPixelFormat_GreenBits_set
    __getmethods__["GreenBits"] = blcelc.csPixelFormat_GreenBits_get
    __setmethods__["BlueBits"] = blcelc.csPixelFormat_BlueBits_set
    __getmethods__["BlueBits"] = blcelc.csPixelFormat_BlueBits_get
    __setmethods__["PalEntries"] = blcelc.csPixelFormat_PalEntries_set
    __getmethods__["PalEntries"] = blcelc.csPixelFormat_PalEntries_get
    __setmethods__["PixelBytes"] = blcelc.csPixelFormat_PixelBytes_set
    __getmethods__["PixelBytes"] = blcelc.csPixelFormat_PixelBytes_get
    def __init__(self,*args):
        self.this = apply(blcelc.new_csPixelFormat,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_csPixelFormat(self)
    def __repr__(self):
        return "<C csPixelFormat instance at %s>" % (self.this,)

class csPixelFormatPtr(csPixelFormat):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = csPixelFormat
blcelc.csPixelFormat_swigregister(csPixelFormatPtr)
class csGraphics3DCaps:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,csGraphics3DCaps):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = csGraphics3DCaps.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = csGraphics3DCaps.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    __setmethods__["CanClip"] = blcelc.csGraphics3DCaps_CanClip_set
    __getmethods__["CanClip"] = blcelc.csGraphics3DCaps_CanClip_get
    __setmethods__["minTexHeight"] = blcelc.csGraphics3DCaps_minTexHeight_set
    __getmethods__["minTexHeight"] = blcelc.csGraphics3DCaps_minTexHeight_get
    __setmethods__["minTexWidth"] = blcelc.csGraphics3DCaps_minTexWidth_set
    __getmethods__["minTexWidth"] = blcelc.csGraphics3DCaps_minTexWidth_get
    __setmethods__["maxTexHeight"] = blcelc.csGraphics3DCaps_maxTexHeight_set
    __getmethods__["maxTexHeight"] = blcelc.csGraphics3DCaps_maxTexHeight_get
    __setmethods__["maxTexWidth"] = blcelc.csGraphics3DCaps_maxTexWidth_set
    __getmethods__["maxTexWidth"] = blcelc.csGraphics3DCaps_maxTexWidth_get
    __setmethods__["fog"] = blcelc.csGraphics3DCaps_fog_set
    __getmethods__["fog"] = blcelc.csGraphics3DCaps_fog_get
    __setmethods__["NeedsPO2Maps"] = blcelc.csGraphics3DCaps_NeedsPO2Maps_set
    __getmethods__["NeedsPO2Maps"] = blcelc.csGraphics3DCaps_NeedsPO2Maps_get
    __setmethods__["MaxAspectRatio"] = blcelc.csGraphics3DCaps_MaxAspectRatio_set
    __getmethods__["MaxAspectRatio"] = blcelc.csGraphics3DCaps_MaxAspectRatio_get
    def __init__(self,*args):
        self.this = apply(blcelc.new_csGraphics3DCaps,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_csGraphics3DCaps(self)
    def __repr__(self):
        return "<C csGraphics3DCaps instance at %s>" % (self.this,)

class csGraphics3DCapsPtr(csGraphics3DCaps):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = csGraphics3DCaps
blcelc.csGraphics3DCaps_swigregister(csGraphics3DCapsPtr)
class csImageArea:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,csImageArea):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = csImageArea.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = csImageArea.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    __setmethods__["x"] = blcelc.csImageArea_x_set
    __getmethods__["x"] = blcelc.csImageArea_x_get
    __setmethods__["y"] = blcelc.csImageArea_y_set
    __getmethods__["y"] = blcelc.csImageArea_y_get
    __setmethods__["w"] = blcelc.csImageArea_w_set
    __getmethods__["w"] = blcelc.csImageArea_w_get
    __setmethods__["h"] = blcelc.csImageArea_h_set
    __getmethods__["h"] = blcelc.csImageArea_h_get
    __setmethods__["data"] = blcelc.csImageArea_data_set
    __getmethods__["data"] = blcelc.csImageArea_data_get
    def __init__(self,*args):
        self.this = apply(blcelc.new_csImageArea,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_csImageArea(self)
    def __repr__(self):
        return "<C csImageArea instance at %s>" % (self.this,)

class csImageAreaPtr(csImageArea):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = csImageArea
blcelc.csImageArea_swigregister(csImageAreaPtr)
class iComponent(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iComponent):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iComponent.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iComponent.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def Initialize(*args): return apply(blcelc.iComponent_Initialize,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iComponent,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iComponent(self)
    def __repr__(self):
        return "<C iComponent instance at %s>" % (self.this,)

class iComponentPtr(iComponent):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iComponent
blcelc.iComponent_swigregister(iComponentPtr)
class iEventHandler(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iEventHandler):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iEventHandler.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iEventHandler.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def HandleEvent(*args): return apply(blcelc.iEventHandler_HandleEvent,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iEventHandler,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iEventHandler(self)
    def __repr__(self):
        return "<C iEventHandler instance at %s>" % (self.this,)

class iEventHandlerPtr(iEventHandler):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iEventHandler
blcelc.iEventHandler_swigregister(iEventHandlerPtr)
class iTextureWrapper(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iTextureWrapper):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iTextureWrapper.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iTextureWrapper.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(blcelc.new_iTextureWrapper,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iTextureWrapper(self)
    def __repr__(self):
        return "<C iTextureWrapper instance at %s>" % (self.this,)

class iTextureWrapperPtr(iTextureWrapper):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iTextureWrapper
blcelc.iTextureWrapper_swigregister(iTextureWrapperPtr)
class iTextureHandle(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iTextureHandle):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iTextureHandle.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iTextureHandle.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetMipMapDimensions(*args): return apply(blcelc.iTextureHandle_GetMipMapDimensions,args)
    def GetMeanColor(*args): return apply(blcelc.iTextureHandle_GetMeanColor,args)
    def GetCacheData(*args): return apply(blcelc.iTextureHandle_GetCacheData,args)
    def SetCacheData(*args): return apply(blcelc.iTextureHandle_SetCacheData,args)
    def GetPrivateObject(*args): return apply(blcelc.iTextureHandle_GetPrivateObject,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iTextureHandle,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iTextureHandle(self)
    def __repr__(self):
        return "<C iTextureHandle instance at %s>" % (self.this,)

class iTextureHandlePtr(iTextureHandle):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iTextureHandle
blcelc.iTextureHandle_swigregister(iTextureHandlePtr)
class iMaterialHandle(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iMaterialHandle):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iMaterialHandle.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iMaterialHandle.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetTexture(*args): return apply(blcelc.iMaterialHandle_GetTexture,args)
    def GetFlatColor(*args): return apply(blcelc.iMaterialHandle_GetFlatColor,args)
    def GetReflection(*args): return apply(blcelc.iMaterialHandle_GetReflection,args)
    def Prepare(*args): return apply(blcelc.iMaterialHandle_Prepare,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iMaterialHandle(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialHandle instance at %s>" % (self.this,)

class iMaterialHandlePtr(iMaterialHandle):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iMaterialHandle
blcelc.iMaterialHandle_swigregister(iMaterialHandlePtr)
class iMaterialWrapper(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iMaterialWrapper):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iMaterialWrapper.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iMaterialWrapper.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetMaterialHandle(*args): return apply(blcelc.iMaterialWrapper_GetMaterialHandle,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iMaterialWrapper(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialWrapper instance at %s>" % (self.this,)

class iMaterialWrapperPtr(iMaterialWrapper):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iMaterialWrapper
blcelc.iMaterialWrapper_swigregister(iMaterialWrapperPtr)
class iFont(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iFont):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iFont.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iFont.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def SetSize(*args): return apply(blcelc.iFont_SetSize,args)
    def GetSize(*args): return apply(blcelc.iFont_GetSize,args)
    def GetMaxSize(*args): return apply(blcelc.iFont_GetMaxSize,args)
    def GetGlyphSize(*args): return apply(blcelc.iFont_GetGlyphSize,args)
    def GetGlyphBitmap(*args): return apply(blcelc.iFont_GetGlyphBitmap,args)
    def GetDimensions(*args): return apply(blcelc.iFont_GetDimensions,args)
    def GetLength(*args): return apply(blcelc.iFont_GetLength,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iFont(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFont instance at %s>" % (self.this,)

class iFontPtr(iFont):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iFont
blcelc.iFont_swigregister(iFontPtr)
class iFontServer(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iFontServer):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iFontServer.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iFontServer.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def LoadFont(*args): return apply(blcelc.iFontServer_LoadFont,args)
    def GetFontCount(*args): return apply(blcelc.iFontServer_GetFontCount,args)
    def GetFont(*args): return apply(blcelc.iFontServer_GetFont,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iFontServer(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iFontServer instance at %s>" % (self.this,)

class iFontServerPtr(iFontServer):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iFontServer
blcelc.iFontServer_swigregister(iFontServerPtr)
class iGraphics2D(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iGraphics2D):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iGraphics2D.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iGraphics2D.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def Open(*args): return apply(blcelc.iGraphics2D_Open,args)
    def Close(*args): return apply(blcelc.iGraphics2D_Close,args)
    def GetWidth(*args): return apply(blcelc.iGraphics2D_GetWidth,args)
    def GetHeight(*args): return apply(blcelc.iGraphics2D_GetHeight,args)
    def GetFullScreen(*args): return apply(blcelc.iGraphics2D_GetFullScreen,args)
    def GetPage(*args): return apply(blcelc.iGraphics2D_GetPage,args)
    def DoubleBuffer(*args): return apply(blcelc.iGraphics2D_DoubleBuffer,args)
    def GetDoubleBufferState(*args): return apply(blcelc.iGraphics2D_GetDoubleBufferState,args)
    def GetPixelFormat(*args): return apply(blcelc.iGraphics2D_GetPixelFormat,args)
    def GetPixelBytes(*args): return apply(blcelc.iGraphics2D_GetPixelBytes,args)
    def GetPalEntryCount(*args): return apply(blcelc.iGraphics2D_GetPalEntryCount,args)
    def GetPalette(*args): return apply(blcelc.iGraphics2D_GetPalette,args)
    def SetRGB(*args): return apply(blcelc.iGraphics2D_SetRGB,args)
    def SetClipRect(*args): return apply(blcelc.iGraphics2D_SetClipRect,args)
    def GetClipRect(*args): return apply(blcelc.iGraphics2D_GetClipRect,args)
    def BeginDraw(*args): return apply(blcelc.iGraphics2D_BeginDraw,args)
    def FinishDraw(*args): return apply(blcelc.iGraphics2D_FinishDraw,args)
    def Print(*args): return apply(blcelc.iGraphics2D_Print,args)
    def Clear(*args): return apply(blcelc.iGraphics2D_Clear,args)
    def ClearAll(*args): return apply(blcelc.iGraphics2D_ClearAll,args)
    def DrawLine(*args): return apply(blcelc.iGraphics2D_DrawLine,args)
    def DrawBox(*args): return apply(blcelc.iGraphics2D_DrawBox,args)
    def ClipLine(*args): return apply(blcelc.iGraphics2D_ClipLine,args)
    def DrawPixel(*args): return apply(blcelc.iGraphics2D_DrawPixel,args)
    def GetPixelAt(*args): return apply(blcelc.iGraphics2D_GetPixelAt,args)
    def GetPixel(*args): return apply(blcelc.iGraphics2D_GetPixel,args)
    def SaveArea(*args): return apply(blcelc.iGraphics2D_SaveArea,args)
    def RestoreArea(*args): return apply(blcelc.iGraphics2D_RestoreArea,args)
    def FreeArea(*args): return apply(blcelc.iGraphics2D_FreeArea,args)
    def Write(*args): return apply(blcelc.iGraphics2D_Write,args)
    def GetFontServer(*args): return apply(blcelc.iGraphics2D_GetFontServer,args)
    def SetMousePosition(*args): return apply(blcelc.iGraphics2D_SetMousePosition,args)
    def SetMouseCursor(*args): return apply(blcelc.iGraphics2D_SetMouseCursor,args)
    def ScreenShot(*args): return apply(blcelc.iGraphics2D_ScreenShot,args)
    def CreateOffScreenCanvas(*args): return apply(blcelc.iGraphics2D_CreateOffScreenCanvas,args)
    def AllowResize(*args): return apply(blcelc.iGraphics2D_AllowResize,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iGraphics2D(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iGraphics2D instance at %s>" % (self.this,)

class iGraphics2DPtr(iGraphics2D):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iGraphics2D
blcelc.iGraphics2D_swigregister(iGraphics2DPtr)
class iHalo(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iHalo):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iHalo.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iHalo.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetWidth(*args): return apply(blcelc.iHalo_GetWidth,args)
    def GetHeight(*args): return apply(blcelc.iHalo_GetHeight,args)
    def SetColor(*args): return apply(blcelc.iHalo_SetColor,args)
    def GetColor(*args): return apply(blcelc.iHalo_GetColor,args)
    def Draw(*args): return apply(blcelc.iHalo_Draw,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iHalo(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iHalo instance at %s>" % (self.this,)

class iHaloPtr(iHalo):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iHalo
blcelc.iHalo_swigregister(iHaloPtr)
class iGraphics3D(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iGraphics3D):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iGraphics3D.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iGraphics3D.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def Open(*args): return apply(blcelc.iGraphics3D_Open,args)
    def Close(*args): return apply(blcelc.iGraphics3D_Close,args)
    def SetDimensions(*args): return apply(blcelc.iGraphics3D_SetDimensions,args)
    def BeginDraw(*args): return apply(blcelc.iGraphics3D_BeginDraw,args)
    def FinishDraw(*args): return apply(blcelc.iGraphics3D_FinishDraw,args)
    def Print(*args): return apply(blcelc.iGraphics3D_Print,args)
    def DrawPolygon(*args): return apply(blcelc.iGraphics3D_DrawPolygon,args)
    def DrawPolygonDebug(*args): return apply(blcelc.iGraphics3D_DrawPolygonDebug,args)
    def DrawLine(*args): return apply(blcelc.iGraphics3D_DrawLine,args)
    def DrawTriangleMesh(*args): return apply(blcelc.iGraphics3D_DrawTriangleMesh,args)
    def DrawPolygonMesh(*args): return apply(blcelc.iGraphics3D_DrawPolygonMesh,args)
    def OpenFogObject(*args): return apply(blcelc.iGraphics3D_OpenFogObject,args)
    def DrawFogPolygon(*args): return apply(blcelc.iGraphics3D_DrawFogPolygon,args)
    def CloseFogObject(*args): return apply(blcelc.iGraphics3D_CloseFogObject,args)
    def GetCaps(*args): return apply(blcelc.iGraphics3D_GetCaps,args)
    def GetZBuffAt(*args): return apply(blcelc.iGraphics3D_GetZBuffAt,args)
    def GetZBuffValue(*args): return apply(blcelc.iGraphics3D_GetZBuffValue,args)
    def DumpCache(*args): return apply(blcelc.iGraphics3D_DumpCache,args)
    def ClearCache(*args): return apply(blcelc.iGraphics3D_ClearCache,args)
    def RemoveFromCache(*args): return apply(blcelc.iGraphics3D_RemoveFromCache,args)
    def GetWidth(*args): return apply(blcelc.iGraphics3D_GetWidth,args)
    def GetHeight(*args): return apply(blcelc.iGraphics3D_GetHeight,args)
    def SetPerspectiveCenter(*args): return apply(blcelc.iGraphics3D_SetPerspectiveCenter,args)
    def SetPerspectiveAspect(*args): return apply(blcelc.iGraphics3D_SetPerspectiveAspect,args)
    def SetObjectToCamera(*args): return apply(blcelc.iGraphics3D_SetObjectToCamera,args)
    def GetDriver2D(*args): return apply(blcelc.iGraphics3D_GetDriver2D,args)
    def GetTextureManager(*args): return apply(blcelc.iGraphics3D_GetTextureManager,args)
    def CreateHalo(*args): return apply(blcelc.iGraphics3D_CreateHalo,args)
    def DrawPixmap(*args): return apply(blcelc.iGraphics3D_DrawPixmap,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iGraphics3D,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iGraphics3D(self)
    def __repr__(self):
        return "<C iGraphics3D instance at %s>" % (self.this,)

class iGraphics3DPtr(iGraphics3D):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iGraphics3D
blcelc.iGraphics3D_swigregister(iGraphics3DPtr)
class iCamera(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iCamera):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iCamera.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iCamera.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetFOV(*args): return apply(blcelc.iCamera_GetFOV,args)
    def GetInvFOV(*args): return apply(blcelc.iCamera_GetInvFOV,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iCamera,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iCamera(self)
    def __repr__(self):
        return "<C iCamera instance at %s>" % (self.this,)

class iCameraPtr(iCamera):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iCamera
blcelc.iCamera_swigregister(iCameraPtr)
class iSector(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iSector):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iSector.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iSector.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(blcelc.new_iSector,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iSector(self)
    def __repr__(self):
        return "<C iSector instance at %s>" % (self.this,)

class iSectorPtr(iSector):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iSector
blcelc.iSector_swigregister(iSectorPtr)
class iThingState(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iThingState):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iThingState.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iThingState.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def CreatePolygon(*args): return apply(blcelc.iThingState_CreatePolygon,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iThingState,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iThingState(self)
    def __repr__(self):
        return "<C iThingState instance at %s>" % (self.this,)

class iThingStatePtr(iThingState):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iThingState
blcelc.iThingState_swigregister(iThingStatePtr)
class iMeshObject(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iMeshObject):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iMeshObject.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iMeshObject.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def Query_iThingState(*args): return apply(blcelc.iMeshObject_Query_iThingState,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iMeshObject,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iMeshObject(self)
    def __repr__(self):
        return "<C iMeshObject instance at %s>" % (self.this,)

class iMeshObjectPtr(iMeshObject):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iMeshObject
blcelc.iMeshObject_swigregister(iMeshObjectPtr)
class iMeshWrapper(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iMeshWrapper):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iMeshWrapper.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iMeshWrapper.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetMeshObject(*args): return apply(blcelc.iMeshWrapper_GetMeshObject,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iMeshWrapper,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iMeshWrapper(self)
    def __repr__(self):
        return "<C iMeshWrapper instance at %s>" % (self.this,)

class iMeshWrapperPtr(iMeshWrapper):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iMeshWrapper
blcelc.iMeshWrapper_swigregister(iMeshWrapperPtr)
class iLightMap(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iLightMap):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iLightMap.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iLightMap.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetMapData(*args): return apply(blcelc.iLightMap_GetMapData,args)
    def GetWidth(*args): return apply(blcelc.iLightMap_GetWidth,args)
    def GetHeight(*args): return apply(blcelc.iLightMap_GetHeight,args)
    def GetRealWidth(*args): return apply(blcelc.iLightMap_GetRealWidth,args)
    def GetRealHeight(*args): return apply(blcelc.iLightMap_GetRealHeight,args)
    def GetCacheData(*args): return apply(blcelc.iLightMap_GetCacheData,args)
    def SetCacheData(*args): return apply(blcelc.iLightMap_SetCacheData,args)
    def GetMeanLighting(*args): return apply(blcelc.iLightMap_GetMeanLighting,args)
    def GetSize(*args): return apply(blcelc.iLightMap_GetSize,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iLightMap(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iLightMap instance at %s>" % (self.this,)

class iLightMapPtr(iLightMap):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iLightMap
blcelc.iLightMap_swigregister(iLightMapPtr)
class iPolygon3D(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPolygon3D):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPolygon3D.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPolygon3D.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetLightMap(*args): return apply(blcelc.iPolygon3D_GetLightMap,args)
    def GetMaterialHandle(*args): return apply(blcelc.iPolygon3D_GetMaterialHandle,args)
    def SetMaterial(*args): return apply(blcelc.iPolygon3D_SetMaterial,args)
    def GetVertexCount(*args): return apply(blcelc.iPolygon3D_GetVertexCount,args)
    def GetVertex(*args): return apply(blcelc.iPolygon3D_GetVertex,args)
    def GetVertexW(*args): return apply(blcelc.iPolygon3D_GetVertexW,args)
    def GetVertexC(*args): return apply(blcelc.iPolygon3D_GetVertexC,args)
    def CreateVertexByIndex(*args): return apply(blcelc.iPolygon3D_CreateVertexByIndex,args)
    def CreateVertex(*args): return apply(blcelc.iPolygon3D_CreateVertex,args)
    def GetAlpha(*args): return apply(blcelc.iPolygon3D_GetAlpha,args)
    def SetAlpha(*args): return apply(blcelc.iPolygon3D_SetAlpha,args)
    def CreatePlane(*args): return apply(blcelc.iPolygon3D_CreatePlane,args)
    def SetPlane(*args): return apply(blcelc.iPolygon3D_SetPlane,args)
    def SetTextureSpace(*args): return apply(blcelc.iPolygon3D_SetTextureSpace,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iPolygon3D,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPolygon3D(self)
    def __repr__(self):
        return "<C iPolygon3D instance at %s>" % (self.this,)

class iPolygon3DPtr(iPolygon3D):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPolygon3D
blcelc.iPolygon3D_swigregister(iPolygon3DPtr)
class iImage(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iImage):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iImage.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iImage.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetImageData(*args): return apply(blcelc.iImage_GetImageData,args)
    def GetWidth(*args): return apply(blcelc.iImage_GetWidth,args)
    def GetHeight(*args): return apply(blcelc.iImage_GetHeight,args)
    def GetSize(*args): return apply(blcelc.iImage_GetSize,args)
    def Rescale(*args): return apply(blcelc.iImage_Rescale,args)
    def MipMap(*args): return apply(blcelc.iImage_MipMap,args)
    def SetName(*args): return apply(blcelc.iImage_SetName,args)
    def GetName(*args): return apply(blcelc.iImage_GetName,args)
    def GetFormat(*args): return apply(blcelc.iImage_GetFormat,args)
    def GetPalette(*args): return apply(blcelc.iImage_GetPalette,args)
    def GetAlpha(*args): return apply(blcelc.iImage_GetAlpha,args)
    def SetFormat(*args): return apply(blcelc.iImage_SetFormat,args)
    def Clone(*args): return apply(blcelc.iImage_Clone,args)
    def Crop(*args): return apply(blcelc.iImage_Crop,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iImage,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iImage(self)
    def __repr__(self):
        return "<C iImage instance at %s>" % (self.this,)

class iImagePtr(iImage):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iImage
blcelc.iImage_swigregister(iImagePtr)
class iTextureManager(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iTextureManager):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iTextureManager.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iTextureManager.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def RegisterTexture(*args): return apply(blcelc.iTextureManager_RegisterTexture,args)
    def PrepareTextures(*args): return apply(blcelc.iTextureManager_PrepareTextures,args)
    def FreeImages(*args): return apply(blcelc.iTextureManager_FreeImages,args)
    def ResetPalette(*args): return apply(blcelc.iTextureManager_ResetPalette,args)
    def ReserveColor(*args): return apply(blcelc.iTextureManager_ReserveColor,args)
    def FindRGB(*args): return apply(blcelc.iTextureManager_FindRGB,args)
    def SetPalette(*args): return apply(blcelc.iTextureManager_SetPalette,args)
    def SetVerbose(*args): return apply(blcelc.iTextureManager_SetVerbose,args)
    def GetTextureFormat(*args): return apply(blcelc.iTextureManager_GetTextureFormat,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iTextureManager,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iTextureManager(self)
    def __repr__(self):
        return "<C iTextureManager instance at %s>" % (self.this,)

class iTextureManagerPtr(iTextureManager):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iTextureManager
blcelc.iTextureManager_swigregister(iTextureManagerPtr)
class iPolygonTexture(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPolygonTexture):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPolygonTexture.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPolygonTexture.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetMaterialHandle(*args): return apply(blcelc.iPolygonTexture_GetMaterialHandle,args)
    def GetFDU(*args): return apply(blcelc.iPolygonTexture_GetFDU,args)
    def GetFDV(*args): return apply(blcelc.iPolygonTexture_GetFDV,args)
    def GetWidth(*args): return apply(blcelc.iPolygonTexture_GetWidth,args)
    def GetHeight(*args): return apply(blcelc.iPolygonTexture_GetHeight,args)
    def GetShiftU(*args): return apply(blcelc.iPolygonTexture_GetShiftU,args)
    def GetIMinU(*args): return apply(blcelc.iPolygonTexture_GetIMinU,args)
    def GetIMinV(*args): return apply(blcelc.iPolygonTexture_GetIMinV,args)
    def GetTextureBox(*args): return apply(blcelc.iPolygonTexture_GetTextureBox,args)
    def GetOriginalWidth(*args): return apply(blcelc.iPolygonTexture_GetOriginalWidth,args)
    def GetPolygon(*args): return apply(blcelc.iPolygonTexture_GetPolygon,args)
    def DynamicLightsDirty(*args): return apply(blcelc.iPolygonTexture_DynamicLightsDirty,args)
    def RecalculateDynamicLights(*args): return apply(blcelc.iPolygonTexture_RecalculateDynamicLights,args)
    def GetLightMap(*args): return apply(blcelc.iPolygonTexture_GetLightMap,args)
    def GetLightCellSize(*args): return apply(blcelc.iPolygonTexture_GetLightCellSize,args)
    def GetLightCellShift(*args): return apply(blcelc.iPolygonTexture_GetLightCellShift,args)
    def GetCacheData(*args): return apply(blcelc.iPolygonTexture_GetCacheData,args)
    def SetCacheData(*args): return apply(blcelc.iPolygonTexture_SetCacheData,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iPolygonTexture,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPolygonTexture(self)
    def __repr__(self):
        return "<C iPolygonTexture instance at %s>" % (self.this,)

class iPolygonTexturePtr(iPolygonTexture):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPolygonTexture
blcelc.iPolygonTexture_swigregister(iPolygonTexturePtr)
class iCameraPosition(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iCameraPosition):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iCameraPosition.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iCameraPosition.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(blcelc.new_iCameraPosition,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iCameraPosition(self)
    def __repr__(self):
        return "<C iCameraPosition instance at %s>" % (self.this,)

class iCameraPositionPtr(iCameraPosition):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iCameraPosition
blcelc.iCameraPosition_swigregister(iCameraPositionPtr)
class iSectorList(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iSectorList):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iSectorList.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iSectorList.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetCount(*args): return apply(blcelc.iSectorList_GetCount,args)
    def Get(*args): return apply(blcelc.iSectorList_Get,args)
    def Add(*args): return apply(blcelc.iSectorList_Add,args)
    def Remove(*args): return apply(blcelc.iSectorList_Remove,args)
    def FindByName(*args): return apply(blcelc.iSectorList_FindByName,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iSectorList(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iSectorList instance at %s>" % (self.this,)

class iSectorListPtr(iSectorList):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iSectorList
blcelc.iSectorList_swigregister(iSectorListPtr)
class iMaterialList(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iMaterialList):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iMaterialList.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iMaterialList.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def FindByName(*args): return apply(blcelc.iMaterialList_FindByName,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iMaterialList(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iMaterialList instance at %s>" % (self.this,)

class iMaterialListPtr(iMaterialList):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iMaterialList
blcelc.iMaterialList_swigregister(iMaterialListPtr)
class iTextureList(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iTextureList):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iTextureList.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iTextureList.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def FindByName(*args): return apply(blcelc.iTextureList_FindByName,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iTextureList(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iTextureList instance at %s>" % (self.this,)

class iTextureListPtr(iTextureList):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iTextureList
blcelc.iTextureList_swigregister(iTextureListPtr)
class iEngine(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iEngine):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iEngine.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iEngine.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetTextureFormat(*args): return apply(blcelc.iEngine_GetTextureFormat,args)
    def DeleteAll(*args): return apply(blcelc.iEngine_DeleteAll,args)
    def CreateTexture(*args): return apply(blcelc.iEngine_CreateTexture,args)
    def CreateSector(*args): return apply(blcelc.iEngine_CreateSector,args)
    def CreateSectorWallsMesh(*args): return apply(blcelc.iEngine_CreateSectorWallsMesh,args)
    def GetSectors(*args): return apply(blcelc.iEngine_GetSectors,args)
    def GetMaterialList(*args): return apply(blcelc.iEngine_GetMaterialList,args)
    def GetTextureList(*args): return apply(blcelc.iEngine_GetTextureList,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iEngine(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iEngine instance at %s>" % (self.this,)

class iEnginePtr(iEngine):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iEngine
blcelc.iEngine_swigregister(iEnginePtr)
class iObjectRegistry(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iObjectRegistry):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iObjectRegistry.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iObjectRegistry.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def Query_iEngine(*args): return apply(blcelc.iObjectRegistry_Query_iEngine,args)
    def Query_iGraphics3D(*args): return apply(blcelc.iObjectRegistry_Query_iGraphics3D,args)
    def Print(*args): return apply(blcelc.iObjectRegistry_Print,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iObjectRegistry,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iObjectRegistry(self)
    def __repr__(self):
        return "<C iObjectRegistry instance at %s>" % (self.this,)

class iObjectRegistryPtr(iObjectRegistry):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iObjectRegistry
blcelc.iObjectRegistry_swigregister(iObjectRegistryPtr)
class iCelPlLayer(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iCelPlLayer):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iCelPlLayer.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iCelPlLayer.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def CreatePropertyClass(*args): return apply(blcelc.iCelPlLayer_CreatePropertyClass,args)
    def FetchString(*args): return apply(blcelc.iCelPlLayer_FetchString,args)
    def GetBehaviourLayerCount(*args): return apply(blcelc.iCelPlLayer_GetBehaviourLayerCount,args)
    def GetBehaviourLayer(*args): return apply(blcelc.iCelPlLayer_GetBehaviourLayer,args)
    def FindBehaviourLayer(*args): return apply(blcelc.iCelPlLayer_FindBehaviourLayer,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iCelPlLayer(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPlLayer instance at %s>" % (self.this,)

class iCelPlLayerPtr(iCelPlLayer):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iCelPlLayer
blcelc.iCelPlLayer_swigregister(iCelPlLayerPtr)
class iCelEntity(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iCelEntity):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iCelEntity.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iCelEntity.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetName(*args): return apply(blcelc.iCelEntity_GetName,args)
    def SetName(*args): return apply(blcelc.iCelEntity_SetName,args)
    def SetBehaviour(*args): return apply(blcelc.iCelEntity_SetBehaviour,args)
    def GetBehaviour(*args): return apply(blcelc.iCelEntity_GetBehaviour,args)
    def GetID(*args): return apply(blcelc.iCelEntity_GetID,args)
    def GetPropertyClassList(*args): return apply(blcelc.iCelEntity_GetPropertyClassList,args)
    def LoadBehaviour(*args): return apply(blcelc.iCelEntity_LoadBehaviour,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iCelEntity(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelEntity instance at %s>" % (self.this,)

class iCelEntityPtr(iCelEntity):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iCelEntity
blcelc.iCelEntity_swigregister(iCelEntityPtr)
class iCelBlLayer(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iCelBlLayer):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iCelBlLayer.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iCelBlLayer.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetName(*args): return apply(blcelc.iCelBlLayer_GetName,args)
    def CreateBehaviour(*args): return apply(blcelc.iCelBlLayer_CreateBehaviour,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iCelBlLayer(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelBlLayer instance at %s>" % (self.this,)

class iCelBlLayerPtr(iCelBlLayer):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iCelBlLayer
blcelc.iCelBlLayer_swigregister(iCelBlLayerPtr)
class iCelBehaviour(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iCelBehaviour):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iCelBehaviour.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iCelBehaviour.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetName(*args): return apply(blcelc.iCelBehaviour_GetName,args)
    def GetBehaviourLayer(*args): return apply(blcelc.iCelBehaviour_GetBehaviourLayer,args)
    def SendMessage(*args): return apply(blcelc.iCelBehaviour_SendMessage,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iCelBehaviour(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelBehaviour instance at %s>" % (self.this,)

class iCelBehaviourPtr(iCelBehaviour):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iCelBehaviour
blcelc.iCelBehaviour_swigregister(iCelBehaviourPtr)
class iCelPropertyClass(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iCelPropertyClass):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iCelPropertyClass.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iCelPropertyClass.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetName(*args): return apply(blcelc.iCelPropertyClass_GetName,args)
    def GetEntity(*args): return apply(blcelc.iCelPropertyClass_GetEntity,args)
    def SetEntity(*args): return apply(blcelc.iCelPropertyClass_SetEntity,args)
    def GetPropertyOrActionType(*args): return apply(blcelc.iCelPropertyClass_GetPropertyOrActionType,args)
    def IsPropertyReadOnly(*args): return apply(blcelc.iCelPropertyClass_IsPropertyReadOnly,args)
    def GetPropertyLong(*args): return apply(blcelc.iCelPropertyClass_GetPropertyLong,args)
    def GetPropertyFloat(*args): return apply(blcelc.iCelPropertyClass_GetPropertyFloat,args)
    def GetPropertyBool(*args): return apply(blcelc.iCelPropertyClass_GetPropertyBool,args)
    def GetPropertyString(*args): return apply(blcelc.iCelPropertyClass_GetPropertyString,args)
    def GetPropertyVector(*args): return apply(blcelc.iCelPropertyClass_GetPropertyVector,args)
    def GetPropertyAndActionCount(*args): return apply(blcelc.iCelPropertyClass_GetPropertyAndActionCount,args)
    def GetPropertyOrActionID(*args): return apply(blcelc.iCelPropertyClass_GetPropertyOrActionID,args)
    def GetPropertyOrActionDescription(*args): return apply(blcelc.iCelPropertyClass_GetPropertyOrActionDescription,args)
    def PerformAction(*args): return apply(blcelc.iCelPropertyClass_PerformAction,args)
    def SetPropertyLong(*args): return apply(blcelc.iCelPropertyClass_SetPropertyLong,args)
    def SetPropertyFloat(*args): return apply(blcelc.iCelPropertyClass_SetPropertyFloat,args)
    def SetPropertyBool(*args): return apply(blcelc.iCelPropertyClass_SetPropertyBool,args)
    def SetPropertyString(*args): return apply(blcelc.iCelPropertyClass_SetPropertyString,args)
    def SetPropertyVector3(*args): return apply(blcelc.iCelPropertyClass_SetPropertyVector3,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iCelPropertyClass(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iCelPropertyClass instance at %s>" % (self.this,)

class iCelPropertyClassPtr(iCelPropertyClass):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iCelPropertyClass
blcelc.iCelPropertyClass_swigregister(iCelPropertyClassPtr)
class iCelPropertyClassList(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iCelPropertyClassList):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iCelPropertyClassList.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iCelPropertyClassList.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def GetCount(*args): return apply(blcelc.iCelPropertyClassList_GetCount,args)
    def Get(*args): return apply(blcelc.iCelPropertyClassList_Get,args)
    def Find(*args): return apply(blcelc.iCelPropertyClassList_Find,args)
    def FindByName(*args): return apply(blcelc.iCelPropertyClassList_FindByName,args)
    def FindByInterface(*args): return apply(blcelc.iCelPropertyClassList_FindByInterface,args)
    def __init__(self,*args):
        self.this = apply(blcelc.new_iCelPropertyClassList,args)
        self.thisown = 1
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iCelPropertyClassList(self)
    def __repr__(self):
        return "<C iCelPropertyClassList instance at %s>" % (self.this,)

class iCelPropertyClassListPtr(iCelPropertyClassList):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iCelPropertyClassList
blcelc.iCelPropertyClassList_swigregister(iCelPropertyClassListPtr)
class iPcRegion(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcRegion):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcRegion.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcRegion.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def SetWorldFile(*args): return apply(blcelc.iPcRegion_SetWorldFile,args)
    def GetWorldDir(*args): return apply(blcelc.iPcRegion_GetWorldDir,args)
    def GetWorldFile(*args): return apply(blcelc.iPcRegion_GetWorldFile,args)
    def SetRegionName(*args): return apply(blcelc.iPcRegion_SetRegionName,args)
    def GetRegionName(*args): return apply(blcelc.iPcRegion_GetRegionName,args)
    def Load(*args): return apply(blcelc.iPcRegion_Load,args)
    def Unload(*args): return apply(blcelc.iPcRegion_Unload,args)
    def FindSector(*args): return apply(blcelc.iPcRegion_FindSector,args)
    def GetStartSector(*args): return apply(blcelc.iPcRegion_GetStartSector,args)
    def GetStartPosition(*args): return apply(blcelc.iPcRegion_GetStartPosition,args)
    def PointCamera(*args): return apply(blcelc.iPcRegion_PointCamera,args)
    def LoadWorld(*args): return apply(blcelc.iPcRegion_LoadWorld,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcRegion(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcRegion instance at %s>" % (self.this,)

class iPcRegionPtr(iPcRegion):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcRegion
blcelc.iPcRegion_swigregister(iPcRegionPtr)
class iPcCamera(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcCamera):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcCamera.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcCamera.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    freelook = blcelc.iPcCamera_freelook
    follow = blcelc.iPcCamera_follow
    rotational = blcelc.iPcCamera_rotational
    firstperson = blcelc.iPcCamera_firstperson
    def SetRegion(*args): return apply(blcelc.iPcCamera_SetRegion,args)
    def SetMode(*args): return apply(blcelc.iPcCamera_SetMode,args)
    def GetMode(*args): return apply(blcelc.iPcCamera_GetMode,args)
    def SetFollowPos(*args): return apply(blcelc.iPcCamera_SetFollowPos,args)
    def GetFollowPos(*args): return apply(blcelc.iPcCamera_GetFollowPos,args)
    def SetPitch(*args): return apply(blcelc.iPcCamera_SetPitch,args)
    def GetPitch(*args): return apply(blcelc.iPcCamera_GetPitch,args)
    def SetRoll(*args): return apply(blcelc.iPcCamera_SetRoll,args)
    def GetRoll(*args): return apply(blcelc.iPcCamera_GetRoll,args)
    def SetRectangle(*args): return apply(blcelc.iPcCamera_SetRectangle,args)
    def GetCamera(*args): return apply(blcelc.iPcCamera_GetCamera,args)
    def GetView(*args): return apply(blcelc.iPcCamera_GetView,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcCamera(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCamera instance at %s>" % (self.this,)

class iPcCameraPtr(iPcCamera):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcCamera
blcelc.iPcCamera_swigregister(iPcCameraPtr)
class iPcMeshSelect(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcMeshSelect):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcMeshSelect.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcMeshSelect.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def SetCamera(*args): return apply(blcelc.iPcMeshSelect_SetCamera,args)
    def SetMouseButtons(*args): return apply(blcelc.iPcMeshSelect_SetMouseButtons,args)
    def GetMouseButtons(*args): return apply(blcelc.iPcMeshSelect_GetMouseButtons,args)
    def SetGlobalSelection(*args): return apply(blcelc.iPcMeshSelect_SetGlobalSelection,args)
    def HasGlobalSelection(*args): return apply(blcelc.iPcMeshSelect_HasGlobalSelection,args)
    def SetFollowMode(*args): return apply(blcelc.iPcMeshSelect_SetFollowMode,args)
    def HasFollowMode(*args): return apply(blcelc.iPcMeshSelect_HasFollowMode,args)
    def SetFollowAlwaysMode(*args): return apply(blcelc.iPcMeshSelect_SetFollowAlwaysMode,args)
    def HasFollowAlwaysMode(*args): return apply(blcelc.iPcMeshSelect_HasFollowAlwaysMode,args)
    def SetDragMode(*args): return apply(blcelc.iPcMeshSelect_SetDragMode,args)
    def HasDragMode(*args): return apply(blcelc.iPcMeshSelect_HasDragMode,args)
    def SetDragPlaneNormal(*args): return apply(blcelc.iPcMeshSelect_SetDragPlaneNormal,args)
    def GetDragPlaneNormal(*args): return apply(blcelc.iPcMeshSelect_GetDragPlaneNormal,args)
    def SetSendmoveEvent(*args): return apply(blcelc.iPcMeshSelect_SetSendmoveEvent,args)
    def HasSendmoveEvent(*args): return apply(blcelc.iPcMeshSelect_HasSendmoveEvent,args)
    def SetSendupEvent(*args): return apply(blcelc.iPcMeshSelect_SetSendupEvent,args)
    def HasSendupEvent(*args): return apply(blcelc.iPcMeshSelect_HasSendupEvent,args)
    def SetSenddownEvent(*args): return apply(blcelc.iPcMeshSelect_SetSenddownEvent,args)
    def HasSenddownEvent(*args): return apply(blcelc.iPcMeshSelect_HasSenddownEvent,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcMeshSelect(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMeshSelect instance at %s>" % (self.this,)

class iPcMeshSelectPtr(iPcMeshSelect):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcMeshSelect
blcelc.iPcMeshSelect_swigregister(iPcMeshSelectPtr)
class iPcMesh(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcMesh):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcMesh.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcMesh.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def LoadMesh(*args): return apply(blcelc.iPcMesh_LoadMesh,args)
    def SetMesh(*args): return apply(blcelc.iPcMesh_SetMesh,args)
    def CreateEmptyThing(*args): return apply(blcelc.iPcMesh_CreateEmptyThing,args)
    def GetMesh(*args): return apply(blcelc.iPcMesh_GetMesh,args)
    def MoveMesh(*args): return apply(blcelc.iPcMesh_MoveMesh,args)
    def SetAction(*args): return apply(blcelc.iPcMesh_SetAction,args)
    def GetAction(*args): return apply(blcelc.iPcMesh_GetAction,args)
    def Hide(*args): return apply(blcelc.iPcMesh_Hide,args)
    def Show(*args): return apply(blcelc.iPcMesh_Show,args)
    def IsVisible(*args): return apply(blcelc.iPcMesh_IsVisible,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcMesh(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMesh instance at %s>" % (self.this,)

class iPcMeshPtr(iPcMesh):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcMesh
blcelc.iPcMesh_swigregister(iPcMeshPtr)
class iPcTimer(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcTimer):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcTimer.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcTimer.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def WakeUp(*args): return apply(blcelc.iPcTimer_WakeUp,args)
    def Clear(*args): return apply(blcelc.iPcTimer_Clear,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcTimer(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcTimer instance at %s>" % (self.this,)

class iPcTimerPtr(iPcTimer):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcTimer
blcelc.iPcTimer_swigregister(iPcTimerPtr)
class iPcSolid(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcSolid):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcSolid.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcSolid.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def SetMesh(*args): return apply(blcelc.iPcSolid_SetMesh,args)
    def GetMesh(*args): return apply(blcelc.iPcSolid_GetMesh,args)
    def GetCollider(*args): return apply(blcelc.iPcSolid_GetCollider,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcSolid(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcSolid instance at %s>" % (self.this,)

class iPcSolidPtr(iPcSolid):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcSolid
blcelc.iPcSolid_swigregister(iPcSolidPtr)
class iPcGravity(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcGravity):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcGravity.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcGravity.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def CreateGravityColliderFromMesh(*args): return apply(blcelc.iPcGravity_CreateGravityColliderFromMesh,args)
    def CreateGravityCollider(*args): return apply(blcelc.iPcGravity_CreateGravityCollider,args)
    def GetGravityCollider(*args): return apply(blcelc.iPcGravity_GetGravityCollider,args)
    def SetMovable(*args): return apply(blcelc.iPcGravity_SetMovable,args)
    def GetMovable(*args): return apply(blcelc.iPcGravity_GetMovable,args)
    def SetSolid(*args): return apply(blcelc.iPcGravity_SetSolid,args)
    def GetSolid(*args): return apply(blcelc.iPcGravity_GetSolid,args)
    def SetWeight(*args): return apply(blcelc.iPcGravity_SetWeight,args)
    def GetWeight(*args): return apply(blcelc.iPcGravity_GetWeight,args)
    def ClearForces(*args): return apply(blcelc.iPcGravity_ClearForces,args)
    def ClearPermanentForces(*args): return apply(blcelc.iPcGravity_ClearPermanentForces,args)
    def ResetSpeed(*args): return apply(blcelc.iPcGravity_ResetSpeed,args)
    def ApplyForce(*args): return apply(blcelc.iPcGravity_ApplyForce,args)
    def ApplyPermanentForce(*args): return apply(blcelc.iPcGravity_ApplyPermanentForce,args)
    def IsResting(*args): return apply(blcelc.iPcGravity_IsResting,args)
    def SetActive(*args): return apply(blcelc.iPcGravity_SetActive,args)
    def IsActive(*args): return apply(blcelc.iPcGravity_IsActive,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcGravity(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcGravity instance at %s>" % (self.this,)

class iPcGravityPtr(iPcGravity):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcGravity
blcelc.iPcGravity_swigregister(iPcGravityPtr)
class iPcMovable(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcMovable):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcMovable.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcMovable.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def SetMesh(*args): return apply(blcelc.iPcMovable_SetMesh,args)
    def GetMesh(*args): return apply(blcelc.iPcMovable_GetMesh,args)
    def SetPos(*args): return apply(blcelc.iPcMovable_SetPos,args)
    def Move(*args): return apply(blcelc.iPcMovable_Move,args)
    def AddConstraint(*args): return apply(blcelc.iPcMovable_AddConstraint,args)
    def RemoveConstraint(*args): return apply(blcelc.iPcMovable_RemoveConstraint,args)
    def RemoveAllConstraints(*args): return apply(blcelc.iPcMovable_RemoveAllConstraints,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcMovable(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcMovable instance at %s>" % (self.this,)

class iPcMovablePtr(iPcMovable):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcMovable
blcelc.iPcMovable_swigregister(iPcMovablePtr)
class iPcInventory(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcInventory):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcInventory.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcInventory.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def AddEntity(*args): return apply(blcelc.iPcInventory_AddEntity,args)
    def RemoveEntity(*args): return apply(blcelc.iPcInventory_RemoveEntity,args)
    def RemoveAll(*args): return apply(blcelc.iPcInventory_RemoveAll,args)
    def GetEntityCount(*args): return apply(blcelc.iPcInventory_GetEntityCount,args)
    def GetEntity(*args): return apply(blcelc.iPcInventory_GetEntity,args)
    def SetStrictCharacteristics(*args): return apply(blcelc.iPcInventory_SetStrictCharacteristics,args)
    def HasStrictCharacteristics(*args): return apply(blcelc.iPcInventory_HasStrictCharacteristics,args)
    def SetConstraints(*args): return apply(blcelc.iPcInventory_SetConstraints,args)
    def GetConstraints(*args): return apply(blcelc.iPcInventory_GetConstraints,args)
    def RemoveConstraints(*args): return apply(blcelc.iPcInventory_RemoveConstraints,args)
    def RemoveAllConstraints(*args): return apply(blcelc.iPcInventory_RemoveAllConstraints,args)
    def GetCurrentCharacteristic(*args): return apply(blcelc.iPcInventory_GetCurrentCharacteristic,args)
    def MarkDirty(*args): return apply(blcelc.iPcInventory_MarkDirty,args)
    def TestConstraints(*args): return apply(blcelc.iPcInventory_TestConstraints,args)
    def Dump(*args): return apply(blcelc.iPcInventory_Dump,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcInventory(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcInventory instance at %s>" % (self.this,)

class iPcInventoryPtr(iPcInventory):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcInventory
blcelc.iPcInventory_swigregister(iPcInventoryPtr)
class iPcCharacteristics(iBase):
    __setmethods__ = {}
    for _s in [iBase]: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,iPcCharacteristics):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = iPcCharacteristics.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in [iBase]: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = iPcCharacteristics.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def SetCharacteristic(*args): return apply(blcelc.iPcCharacteristics_SetCharacteristic,args)
    def SetInheritedCharacteristic(*args): return apply(blcelc.iPcCharacteristics_SetInheritedCharacteristic,args)
    def GetCharacteristic(*args): return apply(blcelc.iPcCharacteristics_GetCharacteristic,args)
    def GetLocalCharacteristic(*args): return apply(blcelc.iPcCharacteristics_GetLocalCharacteristic,args)
    def GetInheritedCharacteristic(*args): return apply(blcelc.iPcCharacteristics_GetInheritedCharacteristic,args)
    def ClearCharacteristic(*args): return apply(blcelc.iPcCharacteristics_ClearCharacteristic,args)
    def HasCharacteristic(*args): return apply(blcelc.iPcCharacteristics_HasCharacteristic,args)
    def ClearAll(*args): return apply(blcelc.iPcCharacteristics_ClearAll,args)
    def AddToInventory(*args): return apply(blcelc.iPcCharacteristics_AddToInventory,args)
    def RemoveFromInventory(*args): return apply(blcelc.iPcCharacteristics_RemoveFromInventory,args)
    def MarkDirty(*args): return apply(blcelc.iPcCharacteristics_MarkDirty,args)
    def TestConstraints(*args): return apply(blcelc.iPcCharacteristics_TestConstraints,args)
    def Dump(*args): return apply(blcelc.iPcCharacteristics_Dump,args)
    def __del__(self,blcelc=blcelc):
        if getattr(self,'thisown',0):
            blcelc.delete_iPcCharacteristics(self)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C iPcCharacteristics instance at %s>" % (self.this,)

class iPcCharacteristicsPtr(iPcCharacteristics):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = iPcCharacteristics
blcelc.iPcCharacteristics_swigregister(iPcCharacteristicsPtr)
ptrvalue = blcelc.ptrvalue

ptrset = blcelc.ptrset

ptrcreate = blcelc.ptrcreate

ptrfree = blcelc.ptrfree

ptradd = blcelc.ptradd

MakeVersion = blcelc.MakeVersion

csQueryRegistry_iCelPlLayer = blcelc.csQueryRegistry_iCelPlLayer

celCreateEntity = blcelc.celCreateEntity

csQueryRegistry_iCelBlLayer = blcelc.csQueryRegistry_iCelBlLayer

celQueryPC_iPcRegion = blcelc.celQueryPC_iPcRegion

celCreateRegion = blcelc.celCreateRegion

scfQuery_iPcRegion = blcelc.scfQuery_iPcRegion

celCreateCamera = blcelc.celCreateCamera

scfQuery_iPcCamera = blcelc.scfQuery_iPcCamera

CEL_MOUSE_BUTTON1 = blcelc.CEL_MOUSE_BUTTON1
CEL_MOUSE_BUTTON2 = blcelc.CEL_MOUSE_BUTTON2
CEL_MOUSE_BUTTON3 = blcelc.CEL_MOUSE_BUTTON3
celPcMeshSelect = blcelc.celPcMeshSelect

celPcMesh = blcelc.celPcMesh

celPcTimer = blcelc.celPcTimer

celPcSolid = blcelc.celPcSolid

celPcGravity = blcelc.celPcGravity

celPcMovable = blcelc.celPcMovable

celPcInventory = blcelc.celPcInventory

celPcCharacteristics = blcelc.celPcCharacteristics

CEL_DATA_NONE = blcelc.CEL_DATA_NONE
CEL_DATA_BOOL = blcelc.CEL_DATA_BOOL
CEL_DATA_BYTE = blcelc.CEL_DATA_BYTE
CEL_DATA_WORD = blcelc.CEL_DATA_WORD
CEL_DATA_LONG = blcelc.CEL_DATA_LONG
CEL_DATA_UBYTE = blcelc.CEL_DATA_UBYTE
CEL_DATA_UWORD = blcelc.CEL_DATA_UWORD
CEL_DATA_ULONG = blcelc.CEL_DATA_ULONG
CEL_DATA_FLOAT = blcelc.CEL_DATA_FLOAT
CEL_DATA_VECTOR3 = blcelc.CEL_DATA_VECTOR3
CEL_DATA_STRING = blcelc.CEL_DATA_STRING
CEL_DATA_PCLASS = blcelc.CEL_DATA_PCLASS
CEL_DATA_ENTITY = blcelc.CEL_DATA_ENTITY
CEL_DATA_BUFFER = blcelc.CEL_DATA_BUFFER
CEL_DATA_ACTION = blcelc.CEL_DATA_ACTION

