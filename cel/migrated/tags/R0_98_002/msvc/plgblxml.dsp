# Microsoft Developer Studio Project File - Name="plgblxml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=plgblxml - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "plgblxml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plgblxml.mak" CFG="plgblxml - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plgblxml - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plgblxml - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "plgblxml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "release"
# PROP BASE Intermediate_Dir "release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\out\release\build\plgblxml\"
# PROP Intermediate_Dir "..\out\release\build\plgblxml\"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /Gy /GF /MD /Ob2 /Og /Oi /Ot /Oy /D "_MT" /D "_MBCS" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "NDEBUG" /D "_WINDOWS"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\release\build\plgblxml\plgblxml.res" /D "NDEBUG" /D "_WINDOWS" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib delayimp.lib libcstool.lib libcsgfx.lib libcsgeom.lib libcsutil.lib  /nologo /version:4.0 /subsystem:windows /machine:I386 /OPT:NOREF /dll /out:"..\blxml.dll"  
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "plgblxml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "debug"
# PROP BASE Intermediate_Dir "debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\out\debug\build\plgblxml\"
# PROP Intermediate_Dir "..\out\debug\build\plgblxml\"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /GR /MDd /ZI /Od /D "_MT" /D "_MBCS" /D "_DEBUG" /D "CS_DEBUG" /D "_WINDOWS" /D "WIN32" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "_DEBUG" /D "CS_DEBUG" /D "_WINDOWS"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\debug\build\plgblxml\plgblxml.res" /D "_DEBUG" /D "CS_DEBUG" /D "_WINDOWS" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib delayimp.lib libcstool_d.lib libcsgfx_d.lib libcsgeom_d.lib libcsutil_d.lib  /nologo /version:4.0 /subsystem:windows /machine:I386 /debug /pdbtype:sept /dll /out:"..\blxml.dll"  
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "plgblxml - Win32 Release"
# Name "plgblxml - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\behave_xml.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\blxml.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\xmlscript.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\token.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\behave_xml.h
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\blxml.h
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\xmlscript.h
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\token.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\msvc\plgblxml.rc
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\blxml.csplugin
# End Source File
# End Group
# End Target
# End Project