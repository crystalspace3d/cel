# This file was created automatically by SWIG.
import blcelc
class iBasePtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def DecRef(self):
        val = blcelc.iBase_DecRef(self.this)
        return val
    def __repr__(self):
        return "<C iBase instance>"
class iBase(iBasePtr):
    def __init__(self,this):
        self.this = this




class iSCFPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def CreateInstance(self,arg0,arg1,arg2):
        val = blcelc.iSCF_CreateInstance(self.this,arg0,arg1,arg2)
        return val
    def __repr__(self):
        return "<C iSCF instance>"
class iSCF(iSCFPtr):
    def __init__(self,this):
        self.this = this




class csVector3Ptr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            blcelc.delete_csVector3(self.this)
    def __setattr__(self,name,value):
        if name == "x" :
            blcelc.csVector3_x_set(self.this,value)
            return
        if name == "y" :
            blcelc.csVector3_y_set(self.this,value)
            return
        if name == "z" :
            blcelc.csVector3_z_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "x" : 
            return blcelc.csVector3_x_get(self.this)
        if name == "y" : 
            return blcelc.csVector3_y_get(self.this)
        if name == "z" : 
            return blcelc.csVector3_z_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C csVector3 instance>"
class csVector3(csVector3Ptr):
    def __init__(self,arg0,arg1,arg2) :
        self.this = blcelc.new_csVector3(arg0,arg1,arg2)
        self.thisown = 1




class csRGBpixelPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "red" :
            blcelc.csRGBpixel_red_set(self.this,value)
            return
        if name == "green" :
            blcelc.csRGBpixel_green_set(self.this,value)
            return
        if name == "blue" :
            blcelc.csRGBpixel_blue_set(self.this,value)
            return
        if name == "alpha" :
            blcelc.csRGBpixel_alpha_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "red" : 
            return blcelc.csRGBpixel_red_get(self.this)
        if name == "green" : 
            return blcelc.csRGBpixel_green_get(self.this)
        if name == "blue" : 
            return blcelc.csRGBpixel_blue_get(self.this)
        if name == "alpha" : 
            return blcelc.csRGBpixel_alpha_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C csRGBpixel instance>"
class csRGBpixel(csRGBpixelPtr):
    def __init__(self,this):
        self.this = this




class csPixelFormatPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "RedMask" :
            blcelc.csPixelFormat_RedMask_set(self.this,value)
            return
        if name == "GreenMask" :
            blcelc.csPixelFormat_GreenMask_set(self.this,value)
            return
        if name == "BlueMask" :
            blcelc.csPixelFormat_BlueMask_set(self.this,value)
            return
        if name == "RedShift" :
            blcelc.csPixelFormat_RedShift_set(self.this,value)
            return
        if name == "GreenShift" :
            blcelc.csPixelFormat_GreenShift_set(self.this,value)
            return
        if name == "BlueShift" :
            blcelc.csPixelFormat_BlueShift_set(self.this,value)
            return
        if name == "RedBits" :
            blcelc.csPixelFormat_RedBits_set(self.this,value)
            return
        if name == "GreenBits" :
            blcelc.csPixelFormat_GreenBits_set(self.this,value)
            return
        if name == "BlueBits" :
            blcelc.csPixelFormat_BlueBits_set(self.this,value)
            return
        if name == "PalEntries" :
            blcelc.csPixelFormat_PalEntries_set(self.this,value)
            return
        if name == "PixelBytes" :
            blcelc.csPixelFormat_PixelBytes_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "RedMask" : 
            return blcelc.csPixelFormat_RedMask_get(self.this)
        if name == "GreenMask" : 
            return blcelc.csPixelFormat_GreenMask_get(self.this)
        if name == "BlueMask" : 
            return blcelc.csPixelFormat_BlueMask_get(self.this)
        if name == "RedShift" : 
            return blcelc.csPixelFormat_RedShift_get(self.this)
        if name == "GreenShift" : 
            return blcelc.csPixelFormat_GreenShift_get(self.this)
        if name == "BlueShift" : 
            return blcelc.csPixelFormat_BlueShift_get(self.this)
        if name == "RedBits" : 
            return blcelc.csPixelFormat_RedBits_get(self.this)
        if name == "GreenBits" : 
            return blcelc.csPixelFormat_GreenBits_get(self.this)
        if name == "BlueBits" : 
            return blcelc.csPixelFormat_BlueBits_get(self.this)
        if name == "PalEntries" : 
            return blcelc.csPixelFormat_PalEntries_get(self.this)
        if name == "PixelBytes" : 
            return blcelc.csPixelFormat_PixelBytes_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C csPixelFormat instance>"
class csPixelFormat(csPixelFormatPtr):
    def __init__(self,this):
        self.this = this




class csGraphics3DCapsPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "CanClip" :
            blcelc.csGraphics3DCaps_CanClip_set(self.this,value)
            return
        if name == "minTexHeight" :
            blcelc.csGraphics3DCaps_minTexHeight_set(self.this,value)
            return
        if name == "minTexWidth" :
            blcelc.csGraphics3DCaps_minTexWidth_set(self.this,value)
            return
        if name == "maxTexHeight" :
            blcelc.csGraphics3DCaps_maxTexHeight_set(self.this,value)
            return
        if name == "maxTexWidth" :
            blcelc.csGraphics3DCaps_maxTexWidth_set(self.this,value)
            return
        if name == "fog" :
            blcelc.csGraphics3DCaps_fog_set(self.this,value)
            return
        if name == "NeedsPO2Maps" :
            blcelc.csGraphics3DCaps_NeedsPO2Maps_set(self.this,value)
            return
        if name == "MaxAspectRatio" :
            blcelc.csGraphics3DCaps_MaxAspectRatio_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "CanClip" : 
            return blcelc.csGraphics3DCaps_CanClip_get(self.this)
        if name == "minTexHeight" : 
            return blcelc.csGraphics3DCaps_minTexHeight_get(self.this)
        if name == "minTexWidth" : 
            return blcelc.csGraphics3DCaps_minTexWidth_get(self.this)
        if name == "maxTexHeight" : 
            return blcelc.csGraphics3DCaps_maxTexHeight_get(self.this)
        if name == "maxTexWidth" : 
            return blcelc.csGraphics3DCaps_maxTexWidth_get(self.this)
        if name == "fog" : 
            return blcelc.csGraphics3DCaps_fog_get(self.this)
        if name == "NeedsPO2Maps" : 
            return blcelc.csGraphics3DCaps_NeedsPO2Maps_get(self.this)
        if name == "MaxAspectRatio" : 
            return blcelc.csGraphics3DCaps_MaxAspectRatio_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C csGraphics3DCaps instance>"
class csGraphics3DCaps(csGraphics3DCapsPtr):
    def __init__(self,this):
        self.this = this




class csImageAreaPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "x" :
            blcelc.csImageArea_x_set(self.this,value)
            return
        if name == "y" :
            blcelc.csImageArea_y_set(self.this,value)
            return
        if name == "w" :
            blcelc.csImageArea_w_set(self.this,value)
            return
        if name == "h" :
            blcelc.csImageArea_h_set(self.this,value)
            return
        if name == "data" :
            blcelc.csImageArea_data_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "x" : 
            return blcelc.csImageArea_x_get(self.this)
        if name == "y" : 
            return blcelc.csImageArea_y_get(self.this)
        if name == "w" : 
            return blcelc.csImageArea_w_get(self.this)
        if name == "h" : 
            return blcelc.csImageArea_h_get(self.this)
        if name == "data" : 
            return blcelc.csImageArea_data_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C csImageArea instance>"
class csImageArea(csImageAreaPtr):
    def __init__(self,arg0,arg1,arg2,arg3) :
        self.this = blcelc.new_csImageArea(arg0,arg1,arg2,arg3)
        self.thisown = 1




class iComponentPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Initialize(self,arg0):
        val = blcelc.iComponent_Initialize(self.this,arg0.this)
        return val
    def __repr__(self):
        return "<C iComponent instance>"
class iComponent(iComponentPtr):
    def __init__(self,this):
        self.this = this




class iEventHandlerPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def HandleEvent(self,arg0):
        val = blcelc.iEventHandler_HandleEvent(self.this,arg0)
        return val
    def __repr__(self):
        return "<C iEventHandler instance>"
class iEventHandler(iEventHandlerPtr):
    def __init__(self,this):
        self.this = this




class iTextureWrapperPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __repr__(self):
        return "<C iTextureWrapper instance>"
class iTextureWrapper(iTextureWrapperPtr):
    def __init__(self,this):
        self.this = this




class iTextureHandlePtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetMipMapDimensions(self,arg0,arg1,arg2):
        val = blcelc.iTextureHandle_GetMipMapDimensions(self.this,arg0,arg1,arg2)
        return val
    def GetMeanColor(self,arg0,arg1,arg2):
        val = blcelc.iTextureHandle_GetMeanColor(self.this,arg0,arg1,arg2)
        return val
    def GetCacheData(self):
        val = blcelc.iTextureHandle_GetCacheData(self.this)
        return val
    def SetCacheData(self,arg0):
        val = blcelc.iTextureHandle_SetCacheData(self.this,arg0)
        return val
    def GetPrivateObject(self):
        val = blcelc.iTextureHandle_GetPrivateObject(self.this)
        return val
    def __repr__(self):
        return "<C iTextureHandle instance>"
class iTextureHandle(iTextureHandlePtr):
    def __init__(self,this):
        self.this = this




class iMaterialHandlePtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetTexture(self):
        val = blcelc.iMaterialHandle_GetTexture(self.this)
        val = iTextureHandlePtr(val)
        return val
    def GetFlatColor(self,arg0):
        val = blcelc.iMaterialHandle_GetFlatColor(self.this,arg0.this)
        return val
    def GetReflection(self,arg0,arg1,arg2):
        val = blcelc.iMaterialHandle_GetReflection(self.this,arg0,arg1,arg2)
        return val
    def Prepare(self):
        val = blcelc.iMaterialHandle_Prepare(self.this)
        return val
    def __repr__(self):
        return "<C iMaterialHandle instance>"
class iMaterialHandle(iMaterialHandlePtr):
    def __init__(self,this):
        self.this = this




class iMaterialWrapperPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetMaterialHandle(self):
        val = blcelc.iMaterialWrapper_GetMaterialHandle(self.this)
        val = iMaterialHandlePtr(val)
        return val
    def __repr__(self):
        return "<C iMaterialWrapper instance>"
class iMaterialWrapper(iMaterialWrapperPtr):
    def __init__(self,this):
        self.this = this




class iFontPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def SetSize(self,arg0):
        val = blcelc.iFont_SetSize(self.this,arg0)
        return val
    def GetSize(self):
        val = blcelc.iFont_GetSize(self.this)
        return val
    def GetMaxSize(self,arg0,arg1):
        val = blcelc.iFont_GetMaxSize(self.this,arg0,arg1)
        return val
    def GetGlyphSize(self,arg0,arg1,arg2):
        val = blcelc.iFont_GetGlyphSize(self.this,arg0,arg1,arg2)
        return val
    def GetGlyphBitmap(self,arg0,arg1,arg2):
        val = blcelc.iFont_GetGlyphBitmap(self.this,arg0,arg1,arg2)
        return val
    def GetDimensions(self,arg0,arg1,arg2):
        val = blcelc.iFont_GetDimensions(self.this,arg0,arg1,arg2)
        return val
    def GetLength(self,arg0,arg1):
        val = blcelc.iFont_GetLength(self.this,arg0,arg1)
        return val
    def __repr__(self):
        return "<C iFont instance>"
class iFont(iFontPtr):
    def __init__(self,this):
        self.this = this




class iFontServerPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def LoadFont(self,arg0):
        val = blcelc.iFontServer_LoadFont(self.this,arg0)
        return val
    def GetFontCount(self):
        val = blcelc.iFontServer_GetFontCount(self.this)
        return val
    def GetFont(self,arg0):
        val = blcelc.iFontServer_GetFont(self.this,arg0)
        val = iFontPtr(val)
        return val
    def __repr__(self):
        return "<C iFontServer instance>"
class iFontServer(iFontServerPtr):
    def __init__(self,this):
        self.this = this




class iGraphics2DPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Open(self):
        val = blcelc.iGraphics2D_Open(self.this)
        return val
    def Close(self):
        val = blcelc.iGraphics2D_Close(self.this)
        return val
    def GetWidth(self):
        val = blcelc.iGraphics2D_GetWidth(self.this)
        return val
    def GetHeight(self):
        val = blcelc.iGraphics2D_GetHeight(self.this)
        return val
    def GetFullScreen(self):
        val = blcelc.iGraphics2D_GetFullScreen(self.this)
        return val
    def GetPage(self):
        val = blcelc.iGraphics2D_GetPage(self.this)
        return val
    def DoubleBuffer(self,arg0):
        val = blcelc.iGraphics2D_DoubleBuffer(self.this,arg0)
        return val
    def GetDoubleBufferState(self):
        val = blcelc.iGraphics2D_GetDoubleBufferState(self.this)
        return val
    def GetPixelFormat(self):
        val = blcelc.iGraphics2D_GetPixelFormat(self.this)
        val = csPixelFormatPtr(val)
        return val
    def GetPixelBytes(self):
        val = blcelc.iGraphics2D_GetPixelBytes(self.this)
        return val
    def GetPalEntryCount(self):
        val = blcelc.iGraphics2D_GetPalEntryCount(self.this)
        return val
    def GetPalette(self):
        val = blcelc.iGraphics2D_GetPalette(self.this)
        val = csRGBpixelPtr(val)
        return val
    def SetRGB(self,arg0,arg1,arg2,arg3):
        val = blcelc.iGraphics2D_SetRGB(self.this,arg0,arg1,arg2,arg3)
        return val
    def SetClipRect(self,arg0,arg1,arg2,arg3):
        val = blcelc.iGraphics2D_SetClipRect(self.this,arg0,arg1,arg2,arg3)
        return val
    def GetClipRect(self,arg0,arg1,arg2,arg3):
        val = blcelc.iGraphics2D_GetClipRect(self.this,arg0,arg1,arg2,arg3)
        return val
    def BeginDraw(self):
        val = blcelc.iGraphics2D_BeginDraw(self.this)
        return val
    def FinishDraw(self):
        val = blcelc.iGraphics2D_FinishDraw(self.this)
        return val
    def Print(self,arg0):
        val = blcelc.iGraphics2D_Print(self.this,arg0)
        return val
    def Clear(self,arg0):
        val = blcelc.iGraphics2D_Clear(self.this,arg0)
        return val
    def ClearAll(self,arg0):
        val = blcelc.iGraphics2D_ClearAll(self.this,arg0)
        return val
    def DrawLine(self,arg0,arg1,arg2,arg3,arg4):
        val = blcelc.iGraphics2D_DrawLine(self.this,arg0,arg1,arg2,arg3,arg4)
        return val
    def DrawBox(self,arg0,arg1,arg2,arg3,arg4):
        val = blcelc.iGraphics2D_DrawBox(self.this,arg0,arg1,arg2,arg3,arg4)
        return val
    def ClipLine(self,arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7):
        val = blcelc.iGraphics2D_ClipLine(self.this,arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7)
        return val
    def DrawPixel(self,arg0,arg1,arg2):
        val = blcelc.iGraphics2D_DrawPixel(self.this,arg0,arg1,arg2)
        return val
    def GetPixelAt(self,arg0,arg1):
        val = blcelc.iGraphics2D_GetPixelAt(self.this,arg0,arg1)
        return val
    def GetPixel(self,arg0,arg1,arg2,arg3,arg4):
        val = blcelc.iGraphics2D_GetPixel(self.this,arg0,arg1,arg2,arg3,arg4)
        return val
    def SaveArea(self,arg0,arg1,arg2,arg3):
        val = blcelc.iGraphics2D_SaveArea(self.this,arg0,arg1,arg2,arg3)
        val = csImageAreaPtr(val)
        return val
    def RestoreArea(self,arg0,arg1):
        val = blcelc.iGraphics2D_RestoreArea(self.this,arg0.this,arg1)
        return val
    def FreeArea(self,arg0):
        val = blcelc.iGraphics2D_FreeArea(self.this,arg0.this)
        return val
    def Write(self,arg0,arg1,arg2,arg3,arg4,arg5):
        val = blcelc.iGraphics2D_Write(self.this,arg0.this,arg1,arg2,arg3,arg4,arg5)
        return val
    def GetFontServer(self):
        val = blcelc.iGraphics2D_GetFontServer(self.this)
        val = iFontServerPtr(val)
        return val
    def SetMousePosition(self,arg0,arg1):
        val = blcelc.iGraphics2D_SetMousePosition(self.this,arg0,arg1)
        return val
    def SetMouseCursor(self,arg0):
        val = blcelc.iGraphics2D_SetMouseCursor(self.this,arg0)
        return val
    def ScreenShot(self):
        val = blcelc.iGraphics2D_ScreenShot(self.this)
        val = iImagePtr(val)
        return val
    def CreateOffScreenCanvas(self,arg0,arg1,arg2,arg3,arg4,*args):
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        val = apply(blcelc.iGraphics2D_CreateOffScreenCanvas,(self.this,arg0,arg1,arg2,arg3,arg4.this,)+args)
        val = iGraphics2DPtr(val)
        return val
    def AllowResize(self,arg0):
        val = blcelc.iGraphics2D_AllowResize(self.this,arg0)
        return val
    def __repr__(self):
        return "<C iGraphics2D instance>"
class iGraphics2D(iGraphics2DPtr):
    def __init__(self,this):
        self.this = this




class iHaloPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetWidth(self):
        val = blcelc.iHalo_GetWidth(self.this)
        return val
    def GetHeight(self):
        val = blcelc.iHalo_GetHeight(self.this)
        return val
    def SetColor(self,arg0,arg1,arg2):
        val = blcelc.iHalo_SetColor(self.this,arg0,arg1,arg2)
        return val
    def GetColor(self,arg0,arg1,arg2):
        val = blcelc.iHalo_GetColor(self.this,arg0,arg1,arg2)
        return val
    def Draw(self,arg0,arg1,arg2,arg3,arg4,arg5,arg6):
        val = blcelc.iHalo_Draw(self.this,arg0,arg1,arg2,arg3,arg4,arg5,arg6)
        return val
    def __repr__(self):
        return "<C iHalo instance>"
class iHalo(iHaloPtr):
    def __init__(self,this):
        self.this = this




class iGraphics3DPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Open(self):
        val = blcelc.iGraphics3D_Open(self.this)
        return val
    def Close(self):
        val = blcelc.iGraphics3D_Close(self.this)
        return val
    def SetDimensions(self,arg0,arg1):
        val = blcelc.iGraphics3D_SetDimensions(self.this,arg0,arg1)
        return val
    def BeginDraw(self,arg0):
        val = blcelc.iGraphics3D_BeginDraw(self.this,arg0)
        return val
    def FinishDraw(self):
        val = blcelc.iGraphics3D_FinishDraw(self.this)
        return val
    def Print(self,arg0):
        val = blcelc.iGraphics3D_Print(self.this,arg0)
        return val
    def DrawPolygon(self,arg0):
        val = blcelc.iGraphics3D_DrawPolygon(self.this,arg0)
        return val
    def DrawPolygonDebug(self,arg0):
        val = blcelc.iGraphics3D_DrawPolygonDebug(self.this,arg0)
        return val
    def DrawLine(self,arg0,arg1,arg2,arg3):
        val = blcelc.iGraphics3D_DrawLine(self.this,arg0.this,arg1.this,arg2,arg3)
        return val
    def DrawTriangleMesh(self,arg0):
        val = blcelc.iGraphics3D_DrawTriangleMesh(self.this,arg0)
        return val
    def DrawPolygonMesh(self,arg0):
        val = blcelc.iGraphics3D_DrawPolygonMesh(self.this,arg0)
        return val
    def OpenFogObject(self,arg0,arg1):
        val = blcelc.iGraphics3D_OpenFogObject(self.this,arg0,arg1)
        return val
    def DrawFogPolygon(self,arg0,arg1,arg2):
        val = blcelc.iGraphics3D_DrawFogPolygon(self.this,arg0,arg1,arg2)
        return val
    def CloseFogObject(self,arg0):
        val = blcelc.iGraphics3D_CloseFogObject(self.this,arg0)
        return val
    def GetCaps(self):
        val = blcelc.iGraphics3D_GetCaps(self.this)
        val = csGraphics3DCapsPtr(val)
        return val
    def GetZBuffAt(self,arg0,arg1):
        val = blcelc.iGraphics3D_GetZBuffAt(self.this,arg0,arg1)
        return val
    def GetZBuffValue(self,arg0,arg1):
        val = blcelc.iGraphics3D_GetZBuffValue(self.this,arg0,arg1)
        return val
    def DumpCache(self):
        val = blcelc.iGraphics3D_DumpCache(self.this)
        return val
    def ClearCache(self):
        val = blcelc.iGraphics3D_ClearCache(self.this)
        return val
    def RemoveFromCache(self,arg0):
        val = blcelc.iGraphics3D_RemoveFromCache(self.this,arg0.this)
        return val
    def GetWidth(self):
        val = blcelc.iGraphics3D_GetWidth(self.this)
        return val
    def GetHeight(self):
        val = blcelc.iGraphics3D_GetHeight(self.this)
        return val
    def SetPerspectiveCenter(self,arg0,arg1):
        val = blcelc.iGraphics3D_SetPerspectiveCenter(self.this,arg0,arg1)
        return val
    def SetPerspectiveAspect(self,arg0):
        val = blcelc.iGraphics3D_SetPerspectiveAspect(self.this,arg0)
        return val
    def SetObjectToCamera(self,arg0):
        val = blcelc.iGraphics3D_SetObjectToCamera(self.this,arg0)
        return val
    def GetDriver2D(self):
        val = blcelc.iGraphics3D_GetDriver2D(self.this)
        val = iGraphics2DPtr(val)
        return val
    def GetTextureManager(self):
        val = blcelc.iGraphics3D_GetTextureManager(self.this)
        val = iTextureManagerPtr(val)
        return val
    def CreateHalo(self,arg0,arg1,arg2,arg3,arg4,arg5):
        val = blcelc.iGraphics3D_CreateHalo(self.this,arg0,arg1,arg2,arg3,arg4,arg5)
        val = iHaloPtr(val)
        return val
    def DrawPixmap(self,arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8):
        val = blcelc.iGraphics3D_DrawPixmap(self.this,arg0.this,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)
        return val
    def __repr__(self):
        return "<C iGraphics3D instance>"
class iGraphics3D(iGraphics3DPtr):
    def __init__(self,this):
        self.this = this




class iCameraPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetFOV(self):
        val = blcelc.iCamera_GetFOV(self.this)
        return val
    def GetInvFOV(self):
        val = blcelc.iCamera_GetInvFOV(self.this)
        return val
    def __repr__(self):
        return "<C iCamera instance>"
class iCamera(iCameraPtr):
    def __init__(self,this):
        self.this = this




class iSectorPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __repr__(self):
        return "<C iSector instance>"
class iSector(iSectorPtr):
    def __init__(self,this):
        self.this = this




class iThingStatePtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def CreatePolygon(self,arg0):
        val = blcelc.iThingState_CreatePolygon(self.this,arg0)
        val = iPolygon3DPtr(val)
        return val
    def __repr__(self):
        return "<C iThingState instance>"
class iThingState(iThingStatePtr):
    def __init__(self,this):
        self.this = this




class iMeshObjectPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Query_iThingState(self):
        val = blcelc.iMeshObject_Query_iThingState(self.this)
        return val
    def __repr__(self):
        return "<C iMeshObject instance>"
class iMeshObject(iMeshObjectPtr):
    def __init__(self,this):
        self.this = this




class iMeshWrapperPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetMeshObject(self):
        val = blcelc.iMeshWrapper_GetMeshObject(self.this)
        val = iMeshObjectPtr(val)
        return val
    def __repr__(self):
        return "<C iMeshWrapper instance>"
class iMeshWrapper(iMeshWrapperPtr):
    def __init__(self,this):
        self.this = this




class iLightMapPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetMapData(self):
        val = blcelc.iLightMap_GetMapData(self.this)
        return val
    def GetWidth(self):
        val = blcelc.iLightMap_GetWidth(self.this)
        return val
    def GetHeight(self):
        val = blcelc.iLightMap_GetHeight(self.this)
        return val
    def GetRealWidth(self):
        val = blcelc.iLightMap_GetRealWidth(self.this)
        return val
    def GetRealHeight(self):
        val = blcelc.iLightMap_GetRealHeight(self.this)
        return val
    def GetCacheData(self):
        val = blcelc.iLightMap_GetCacheData(self.this)
        return val
    def SetCacheData(self,arg0):
        val = blcelc.iLightMap_SetCacheData(self.this,arg0)
        return val
    def GetMeanLighting(self,arg0,arg1,arg2):
        val = blcelc.iLightMap_GetMeanLighting(self.this,arg0,arg1,arg2)
        return val
    def GetSize(self):
        val = blcelc.iLightMap_GetSize(self.this)
        return val
    def __repr__(self):
        return "<C iLightMap instance>"
class iLightMap(iLightMapPtr):
    def __init__(self,this):
        self.this = this




class iPolygon3DPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetLightMap(self):
        val = blcelc.iPolygon3D_GetLightMap(self.this)
        val = iLightMapPtr(val)
        return val
    def GetMaterialHandle(self):
        val = blcelc.iPolygon3D_GetMaterialHandle(self.this)
        val = iMaterialHandlePtr(val)
        return val
    def SetMaterial(self,arg0):
        val = blcelc.iPolygon3D_SetMaterial(self.this,arg0.this)
        return val
    def GetVertexCount(self):
        val = blcelc.iPolygon3D_GetVertexCount(self.this)
        return val
    def GetVertex(self,arg0):
        val = blcelc.iPolygon3D_GetVertex(self.this,arg0)
        val = csVector3Ptr(val)
        return val
    def GetVertexW(self,arg0):
        val = blcelc.iPolygon3D_GetVertexW(self.this,arg0)
        val = csVector3Ptr(val)
        return val
    def GetVertexC(self,arg0):
        val = blcelc.iPolygon3D_GetVertexC(self.this,arg0)
        val = csVector3Ptr(val)
        return val
    def CreateVertexByIndex(self,arg0):
        val = blcelc.iPolygon3D_CreateVertexByIndex(self.this,arg0)
        return val
    def CreateVertex(self,arg0):
        val = blcelc.iPolygon3D_CreateVertex(self.this,arg0.this)
        return val
    def GetAlpha(self):
        val = blcelc.iPolygon3D_GetAlpha(self.this)
        return val
    def SetAlpha(self,arg0):
        val = blcelc.iPolygon3D_SetAlpha(self.this,arg0)
        return val
    def CreatePlane(self,arg0,arg1):
        val = blcelc.iPolygon3D_CreatePlane(self.this,arg0.this,arg1)
        return val
    def SetPlane(self,arg0):
        val = blcelc.iPolygon3D_SetPlane(self.this,arg0)
        return val
    def SetTextureSpace(self,arg0,arg1,arg2):
        val = blcelc.iPolygon3D_SetTextureSpace(self.this,arg0.this,arg1.this,arg2)
        return val
    def __repr__(self):
        return "<C iPolygon3D instance>"
class iPolygon3D(iPolygon3DPtr):
    def __init__(self,this):
        self.this = this




class iImagePtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetImageData(self):
        val = blcelc.iImage_GetImageData(self.this)
        return val
    def GetWidth(self):
        val = blcelc.iImage_GetWidth(self.this)
        return val
    def GetHeight(self):
        val = blcelc.iImage_GetHeight(self.this)
        return val
    def GetSize(self):
        val = blcelc.iImage_GetSize(self.this)
        return val
    def Rescale(self,arg0,arg1):
        val = blcelc.iImage_Rescale(self.this,arg0,arg1)
        return val
    def MipMap(self,arg0,arg1):
        val = blcelc.iImage_MipMap(self.this,arg0,arg1.this)
        val = iImagePtr(val)
        return val
    def SetName(self,arg0):
        val = blcelc.iImage_SetName(self.this,arg0)
        return val
    def GetName(self):
        val = blcelc.iImage_GetName(self.this)
        return val
    def GetFormat(self):
        val = blcelc.iImage_GetFormat(self.this)
        return val
    def GetPalette(self):
        val = blcelc.iImage_GetPalette(self.this)
        val = csRGBpixelPtr(val)
        return val
    def GetAlpha(self):
        val = blcelc.iImage_GetAlpha(self.this)
        return val
    def SetFormat(self,arg0):
        val = blcelc.iImage_SetFormat(self.this,arg0)
        return val
    def Clone(self):
        val = blcelc.iImage_Clone(self.this)
        val = iImagePtr(val)
        return val
    def Crop(self,arg0,arg1,arg2,arg3):
        val = blcelc.iImage_Crop(self.this,arg0,arg1,arg2,arg3)
        val = iImagePtr(val)
        return val
    def __repr__(self):
        return "<C iImage instance>"
class iImage(iImagePtr):
    def __init__(self,this):
        self.this = this




class iTextureManagerPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def RegisterTexture(self,arg0,arg1):
        val = blcelc.iTextureManager_RegisterTexture(self.this,arg0.this,arg1)
        return val
    def PrepareTextures(self):
        val = blcelc.iTextureManager_PrepareTextures(self.this)
        return val
    def FreeImages(self):
        val = blcelc.iTextureManager_FreeImages(self.this)
        return val
    def ResetPalette(self):
        val = blcelc.iTextureManager_ResetPalette(self.this)
        return val
    def ReserveColor(self,arg0,arg1,arg2):
        val = blcelc.iTextureManager_ReserveColor(self.this,arg0,arg1,arg2)
        return val
    def FindRGB(self,arg0,arg1,arg2):
        val = blcelc.iTextureManager_FindRGB(self.this,arg0,arg1,arg2)
        return val
    def SetPalette(self):
        val = blcelc.iTextureManager_SetPalette(self.this)
        return val
    def SetVerbose(self,arg0):
        val = blcelc.iTextureManager_SetVerbose(self.this,arg0)
        return val
    def GetTextureFormat(self):
        val = blcelc.iTextureManager_GetTextureFormat(self.this)
        return val
    def __repr__(self):
        return "<C iTextureManager instance>"
class iTextureManager(iTextureManagerPtr):
    def __init__(self,this):
        self.this = this




class iPolygonTexturePtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetMaterialHandle(self):
        val = blcelc.iPolygonTexture_GetMaterialHandle(self.this)
        val = iMaterialHandlePtr(val)
        return val
    def GetFDU(self):
        val = blcelc.iPolygonTexture_GetFDU(self.this)
        return val
    def GetFDV(self):
        val = blcelc.iPolygonTexture_GetFDV(self.this)
        return val
    def GetWidth(self):
        val = blcelc.iPolygonTexture_GetWidth(self.this)
        return val
    def GetHeight(self):
        val = blcelc.iPolygonTexture_GetHeight(self.this)
        return val
    def GetShiftU(self):
        val = blcelc.iPolygonTexture_GetShiftU(self.this)
        return val
    def GetIMinU(self):
        val = blcelc.iPolygonTexture_GetIMinU(self.this)
        return val
    def GetIMinV(self):
        val = blcelc.iPolygonTexture_GetIMinV(self.this)
        return val
    def GetTextureBox(self,arg0,arg1,arg2,arg3):
        val = blcelc.iPolygonTexture_GetTextureBox(self.this,arg0,arg1,arg2,arg3)
        return val
    def GetOriginalWidth(self):
        val = blcelc.iPolygonTexture_GetOriginalWidth(self.this)
        return val
    def GetPolygon(self):
        val = blcelc.iPolygonTexture_GetPolygon(self.this)
        val = iPolygon3DPtr(val)
        return val
    def DynamicLightsDirty(self):
        val = blcelc.iPolygonTexture_DynamicLightsDirty(self.this)
        return val
    def RecalculateDynamicLights(self):
        val = blcelc.iPolygonTexture_RecalculateDynamicLights(self.this)
        return val
    def GetLightMap(self):
        val = blcelc.iPolygonTexture_GetLightMap(self.this)
        val = iLightMapPtr(val)
        return val
    def GetLightCellSize(self):
        val = blcelc.iPolygonTexture_GetLightCellSize(self.this)
        return val
    def GetLightCellShift(self):
        val = blcelc.iPolygonTexture_GetLightCellShift(self.this)
        return val
    def GetCacheData(self,arg0):
        val = blcelc.iPolygonTexture_GetCacheData(self.this,arg0)
        return val
    def SetCacheData(self,arg0,arg1):
        val = blcelc.iPolygonTexture_SetCacheData(self.this,arg0,arg1)
        return val
    def __repr__(self):
        return "<C iPolygonTexture instance>"
class iPolygonTexture(iPolygonTexturePtr):
    def __init__(self,this):
        self.this = this




class iCameraPositionPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __repr__(self):
        return "<C iCameraPosition instance>"
class iCameraPosition(iCameraPositionPtr):
    def __init__(self,this):
        self.this = this




class iSectorListPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetCount(self):
        val = blcelc.iSectorList_GetCount(self.this)
        return val
    def Get(self,arg0):
        val = blcelc.iSectorList_Get(self.this,arg0)
        val = iSectorPtr(val)
        return val
    def Add(self,arg0):
        val = blcelc.iSectorList_Add(self.this,arg0.this)
        return val
    def Remove(self,arg0):
        val = blcelc.iSectorList_Remove(self.this,arg0.this)
        return val
    def FindByName(self,arg0):
        val = blcelc.iSectorList_FindByName(self.this,arg0)
        val = iSectorPtr(val)
        return val
    def __repr__(self):
        return "<C iSectorList instance>"
class iSectorList(iSectorListPtr):
    def __init__(self,this):
        self.this = this




class iMaterialListPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def FindByName(self,arg0):
        val = blcelc.iMaterialList_FindByName(self.this,arg0)
        val = iMaterialWrapperPtr(val)
        return val
    def __repr__(self):
        return "<C iMaterialList instance>"
class iMaterialList(iMaterialListPtr):
    def __init__(self,this):
        self.this = this




class iTextureListPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def FindByName(self,arg0):
        val = blcelc.iTextureList_FindByName(self.this,arg0)
        val = iTextureWrapperPtr(val)
        return val
    def __repr__(self):
        return "<C iTextureList instance>"
class iTextureList(iTextureListPtr):
    def __init__(self,this):
        self.this = this




class iEnginePtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetTextureFormat(self):
        val = blcelc.iEngine_GetTextureFormat(self.this)
        return val
    def DeleteAll(self):
        val = blcelc.iEngine_DeleteAll(self.this)
        return val
    def CreateTexture(self,arg0,arg1,arg2,arg3):
        val = blcelc.iEngine_CreateTexture(self.this,arg0,arg1,arg2,arg3)
        val = iTextureWrapperPtr(val)
        return val
    def CreateSector(self,arg0):
        val = blcelc.iEngine_CreateSector(self.this,arg0)
        val = iSectorPtr(val)
        return val
    def CreateSectorWallsMesh(self,arg0,arg1):
        val = blcelc.iEngine_CreateSectorWallsMesh(self.this,arg0.this,arg1)
        return val
    def GetSectors(self):
        val = blcelc.iEngine_GetSectors(self.this)
        val = iSectorListPtr(val)
        return val
    def GetMaterialList(self):
        val = blcelc.iEngine_GetMaterialList(self.this)
        val = iMaterialListPtr(val)
        return val
    def GetTextureList(self):
        val = blcelc.iEngine_GetTextureList(self.this)
        val = iTextureListPtr(val)
        return val
    def __repr__(self):
        return "<C iEngine instance>"
class iEngine(iEnginePtr):
    def __init__(self,this):
        self.this = this




class iObjectRegistryPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Query_iEngine(self):
        val = blcelc.iObjectRegistry_Query_iEngine(self.this)
        val = iEnginePtr(val)
        return val
    def Query_iGraphics3D(self):
        val = blcelc.iObjectRegistry_Query_iGraphics3D(self.this)
        val = iGraphics3DPtr(val)
        return val
    def Print(self,arg0,arg1):
        val = blcelc.iObjectRegistry_Print(self.this,arg0,arg1)
        return val
    def __repr__(self):
        return "<C iObjectRegistry instance>"
class iObjectRegistry(iObjectRegistryPtr):
    def __init__(self,this):
        self.this = this




class iCelPlLayerPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def CreateEntity(self):
        val = blcelc.iCelPlLayer_CreateEntity(self.this)
        return val
    def CreatePropertyClass(self,arg0,arg1):
        val = blcelc.iCelPlLayer_CreatePropertyClass(self.this,arg0.this,arg1)
        val = iCelPropertyClassPtr(val)
        return val
    def FetchString(self,arg0):
        val = blcelc.iCelPlLayer_FetchString(self.this,arg0)
        return val
    def GetBehaviourLayerCount(self):
        val = blcelc.iCelPlLayer_GetBehaviourLayerCount(self.this)
        return val
    def GetBehaviourLayer(self,arg0):
        val = blcelc.iCelPlLayer_GetBehaviourLayer(self.this,arg0)
        val = iCelBlLayerPtr(val)
        return val
    def FindBehaviourLayer(self,arg0):
        val = blcelc.iCelPlLayer_FindBehaviourLayer(self.this,arg0)
        val = iCelBlLayerPtr(val)
        return val
    def __repr__(self):
        return "<C iCelPlLayer instance>"
class iCelPlLayer(iCelPlLayerPtr):
    def __init__(self,this):
        self.this = this




class iCelEntityPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetName(self):
        val = blcelc.iCelEntity_GetName(self.this)
        return val
    def SetName(self,arg0):
        val = blcelc.iCelEntity_SetName(self.this,arg0)
        return val
    def SetBehaviour(self,arg0):
        val = blcelc.iCelEntity_SetBehaviour(self.this,arg0.this)
        return val
    def GetBehaviour(self):
        val = blcelc.iCelEntity_GetBehaviour(self.this)
        val = iCelBehaviourPtr(val)
        return val
    def GetID(self):
        val = blcelc.iCelEntity_GetID(self.this)
        return val
    def GetPropertyClassList(self):
        val = blcelc.iCelEntity_GetPropertyClassList(self.this)
        val = iCelPropertyClassListPtr(val)
        return val
    def __repr__(self):
        return "<C iCelEntity instance>"
class iCelEntity(iCelEntityPtr):
    def __init__(self,this):
        self.this = this




class iCelBlLayerPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetName(self):
        val = blcelc.iCelBlLayer_GetName(self.this)
        return val
    def CreateBehaviour(self,arg0,arg1):
        val = blcelc.iCelBlLayer_CreateBehaviour(self.this,arg0.this,arg1)
        val = iCelBehaviourPtr(val)
        return val
    def __repr__(self):
        return "<C iCelBlLayer instance>"
class iCelBlLayer(iCelBlLayerPtr):
    def __init__(self,this):
        self.this = this




class iCelBehaviourPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetName(self):
        val = blcelc.iCelBehaviour_GetName(self.this)
        return val
    def GetBehaviourLayer(self):
        val = blcelc.iCelBehaviour_GetBehaviourLayer(self.this)
        val = iCelBlLayerPtr(val)
        return val
    def SendMessageV(self,arg0,arg1,arg2):
        val = blcelc.iCelBehaviour_SendMessageV(self.this,arg0,arg1.this,arg2)
        return val
    def __repr__(self):
        return "<C iCelBehaviour instance>"
class iCelBehaviour(iCelBehaviourPtr):
    def __init__(self,this):
        self.this = this




class iCelPropertyClassPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetName(self):
        val = blcelc.iCelPropertyClass_GetName(self.this)
        return val
    def GetEntity(self):
        val = blcelc.iCelPropertyClass_GetEntity(self.this)
        val = iCelEntityPtr(val)
        return val
    def SetEntity(self,arg0):
        val = blcelc.iCelPropertyClass_SetEntity(self.this,arg0.this)
        return val
    def GetPropertyOrActionType(self,arg0):
        val = blcelc.iCelPropertyClass_GetPropertyOrActionType(self.this,arg0)
        return val
    def IsPropertyReadOnly(self,arg0):
        val = blcelc.iCelPropertyClass_IsPropertyReadOnly(self.this,arg0)
        return val
    def GetPropertyLong(self,arg0):
        val = blcelc.iCelPropertyClass_GetPropertyLong(self.this,arg0)
        return val
    def GetPropertyFloat(self,arg0):
        val = blcelc.iCelPropertyClass_GetPropertyFloat(self.this,arg0)
        return val
    def GetPropertyBool(self,arg0):
        val = blcelc.iCelPropertyClass_GetPropertyBool(self.this,arg0)
        return val
    def GetPropertyString(self,arg0):
        val = blcelc.iCelPropertyClass_GetPropertyString(self.this,arg0)
        return val
    def GetPropertyVector(self,arg0,arg1):
        val = blcelc.iCelPropertyClass_GetPropertyVector(self.this,arg0,arg1.this)
        return val
    def GetPropertyAndActionCount(self):
        val = blcelc.iCelPropertyClass_GetPropertyAndActionCount(self.this)
        return val
    def GetPropertyOrActionID(self,arg0):
        val = blcelc.iCelPropertyClass_GetPropertyOrActionID(self.this,arg0)
        return val
    def GetPropertyOrActionDescription(self,arg0):
        val = blcelc.iCelPropertyClass_GetPropertyOrActionDescription(self.this,arg0)
        return val
    def PerformAction(self,arg0,arg1):
        val = blcelc.iCelPropertyClass_PerformAction(self.this,arg0,arg1)
        return val
    def SetPropertyLong(self,arg0,arg1):
        val = blcelc.iCelPropertyClass_SetPropertyLong(self.this,arg0,arg1)
        return val
    def SetPropertyFloat(self,arg0,arg1):
        val = blcelc.iCelPropertyClass_SetPropertyFloat(self.this,arg0,arg1)
        return val
    def SetPropertyBool(self,arg0,arg1):
        val = blcelc.iCelPropertyClass_SetPropertyBool(self.this,arg0,arg1)
        return val
    def SetPropertyString(self,arg0,arg1):
        val = blcelc.iCelPropertyClass_SetPropertyString(self.this,arg0,arg1)
        return val
    def SetPropertyVector3(self,arg0,arg1):
        val = blcelc.iCelPropertyClass_SetPropertyVector3(self.this,arg0,arg1.this)
        return val
    def __repr__(self):
        return "<C iCelPropertyClass instance>"
class iCelPropertyClass(iCelPropertyClassPtr):
    def __init__(self,this):
        self.this = this




class iCelPropertyClassListPtr(iBasePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetCount(self):
        val = blcelc.iCelPropertyClassList_GetCount(self.this)
        return val
    def Get(self,arg0):
        val = blcelc.iCelPropertyClassList_Get(self.this,arg0)
        val = iCelPropertyClassPtr(val)
        return val
    def Find(self,arg0):
        val = blcelc.iCelPropertyClassList_Find(self.this,arg0.this)
        return val
    def FindByName(self,arg0):
        val = blcelc.iCelPropertyClassList_FindByName(self.this,arg0)
        val = iCelPropertyClassPtr(val)
        return val
    def FindByInterface(self,arg0,arg1):
        val = blcelc.iCelPropertyClassList_FindByInterface(self.this,arg0,arg1)
        val = iBasePtr(val)
        return val
    def __repr__(self):
        return "<C iCelPropertyClassList instance>"
class iCelPropertyClassList(iCelPropertyClassListPtr):
    def __init__(self,this):
        self.this = this






#-------------- FUNCTION WRAPPERS ------------------

ptrcast = blcelc.ptrcast

ptrvalue = blcelc.ptrvalue

ptrset = blcelc.ptrset

ptrcreate = blcelc.ptrcreate

ptrfree = blcelc.ptrfree

ptradd = blcelc.ptradd

ptrmap = blcelc.ptrmap

MakeVersion = blcelc.MakeVersion



#-------------- VARIABLE WRAPPERS ------------------

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
