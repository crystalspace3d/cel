# Microsoft Developer Studio Project File - Name="plgstdphyslayer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=plgstdphyslayer - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "plgstdphyslayer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plgstdphyslayer.mak" CFG="plgstdphyslayer - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plgstdphyslayer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plgstdphyslayer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "plgstdphyslayer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "release"
# PROP BASE Intermediate_Dir "release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\out\release\build\plgstdphyslayer\"
# PROP Intermediate_Dir "..\out\release\build\plgstdphyslayer\"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /Gy /GF /MD /Ob2 /Og /Oi /Ot /Oy /D "_MT" /D "_MBCS" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include" /I "..\..\..\CS\include" /I "..\..\..\CS\include\csutil\win32" /I "$(CRYSTAL)\include" /I "$(CRYSTAL)\include\crystalspace"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "NDEBUG" /D "_WINDOWS"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\release\build\plgstdphyslayer\plgstdphyslayer.res" /D "NDEBUG" /D "_WINDOWS" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include" /i "..\..\..\CS\include" /i "..\..\..\CS\include\csutil\win32" /i "$(CRYSTAL)\include" /i "$(CRYSTAL)\include\crystalspace"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib delayimp.lib advapi32.lib user32.lib gdi32.lib shell32.lib libcrystalspace.lib  /nologo /version:4.0 /machine:I386 /OPT:NOREF /dll /out:"..\stdphyslayer.dll" /subsystem:windows /libpath:"..\..\..\CS\out\release\libs" /libpath:"..\..\..\CS\libs\csutil\win32" /libpath:"$(CRYSTAL)\out\release\libs" /libpath:"$(CRYSTAL)\libs" /libpath:"$(CRYSTAL)\libs\csutil\win32" 
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "plgstdphyslayer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "debug"
# PROP BASE Intermediate_Dir "debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\out\debug\build\plgstdphyslayer\"
# PROP Intermediate_Dir "..\out\debug\build\plgstdphyslayer\"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /GR /MDd /ZI /Od /D "_MT" /D "_MBCS" /D "_DEBUG" /D "CS_DEBUG" /D "_WINDOWS" /D "WIN32" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include" /I "..\..\..\CS\include" /I "..\..\..\CS\include\csutil\win32" /I "$(CRYSTAL)\include" /I "$(CRYSTAL)\include\crystalspace"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "_DEBUG" /D "CS_DEBUG" /D "_WINDOWS"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\debug\build\plgstdphyslayer\plgstdphyslayer.res" /D "_DEBUG" /D "CS_DEBUG" /D "_WINDOWS" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include" /i "..\..\..\CS\include" /i "..\..\..\CS\include\csutil\win32" /i "$(CRYSTAL)\include" /i "$(CRYSTAL)\include\crystalspace"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib delayimp.lib advapi32.lib user32.lib gdi32.lib shell32.lib libcrystalspace_d.lib  /nologo /version:4.0 /machine:I386 /debug /pdbtype:sept /dll /out:"..\stdphyslayer.dll" /subsystem:windows /libpath:"..\..\..\CS\out\debug\libs" /libpath:"..\..\..\CS\libs\csutil\win32" /libpath:"$(CRYSTAL)\out\debug\libs" /libpath:"$(CRYSTAL)\libs" /libpath:"$(CRYSTAL)\libs\csutil\win32" 
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "plgstdphyslayer - Win32 Release"
# Name "plgstdphyslayer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\stdphyslayer\entity.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\entitytpl.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\etracker.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\message.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\numreg.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\pl.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\propclas.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\stdphyslayer\entity.h
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\entitytpl.h
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\etracker.h
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\message.h
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\numreg.h
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\pl.h
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\propclas.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\msvc\plgstdphyslayer.rc
# End Source File
# Begin Source File

SOURCE=..\plugins\stdphyslayer\stdphyslayer.csplugin
# End Source File
# End Group
# End Target
# End Project
