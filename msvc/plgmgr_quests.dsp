# Microsoft Developer Studio Project File - Name="plgmgr_quests" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=plgmgr_quests - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "plgmgr_quests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plgmgr_quests.mak" CFG="plgmgr_quests - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plgmgr_quests - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plgmgr_quests - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "plgmgr_quests - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "release"
# PROP BASE Intermediate_Dir "release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\out\release\build\plgmgr_quests\"
# PROP Intermediate_Dir "..\out\release\build\plgmgr_quests\"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /Gy /GF /MD /Ob2 /Og /Oi /Ot /Oy /D "_MT" /D "_MBCS" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include" /I "..\..\..\CS\include" /I "$(CRYSTAL)\include" /I "$(CRYSTAL)\include\crystalspace"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "NDEBUG" /D "_WINDOWS"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\release\build\plgmgr_quests\plgmgr_quests.res" /D "NDEBUG" /D "_WINDOWS" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include" /i "..\..\..\CS\include" /i "$(CRYSTAL)\include" /i "$(CRYSTAL)\include\crystalspace"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib delayimp.lib advapi32.lib user32.lib gdi32.lib shell32.lib libcrystalspace.lib  /nologo /version:4.0 /machine:I386 /OPT:NOREF /dll /out:"..\mgr_quests.dll" /subsystem:windows /libpath:"..\..\..\CS\out\release\libs" /libpath:"..\..\..\CS\libs\csutil\win32" /libpath:"$(CRYSTAL)\out\release\libs" /libpath:"$(CRYSTAL)\libs" /libpath:"$(CRYSTAL)\libs\csutil\win32" 
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "plgmgr_quests - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "debug"
# PROP BASE Intermediate_Dir "debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\out\debug\build\plgmgr_quests\"
# PROP Intermediate_Dir "..\out\debug\build\plgmgr_quests\"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /GR /MDd /ZI /Od /D "_MT" /D "_MBCS" /D "_DEBUG" /D "CS_DEBUG" /D "_WINDOWS" /D "WIN32" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include" /I "..\..\..\CS\include" /I "$(CRYSTAL)\include" /I "$(CRYSTAL)\include\crystalspace"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "_DEBUG" /D "CS_DEBUG" /D "_WINDOWS"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\debug\build\plgmgr_quests\plgmgr_quests.res" /D "_DEBUG" /D "CS_DEBUG" /D "_WINDOWS" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include" /i "..\..\..\CS\include" /i "$(CRYSTAL)\include" /i "$(CRYSTAL)\include\crystalspace"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib delayimp.lib advapi32.lib user32.lib gdi32.lib shell32.lib libcrystalspace_d.lib  /nologo /version:4.0 /machine:I386 /debug /pdbtype:sept /dll /out:"..\mgr_quests.dll" /subsystem:windows /libpath:"..\..\..\CS\out\debug\libs" /libpath:"..\..\..\CS\libs\csutil\win32" /libpath:"$(CRYSTAL)\out\debug\libs" /libpath:"$(CRYSTAL)\libs" /libpath:"$(CRYSTAL)\libs\csutil\win32" 
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "plgmgr_quests - Win32 Release"
# Name "plgmgr_quests - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\tools\quests\quests.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_changeproperty.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_debugprint.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_inventory.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_newstate.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_sequence.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_sequencefinish.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\seqop_debugprint.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_entersector.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_meshentersector.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_propertychange.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_sequencefinish.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_timeout.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_trigger.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\tools\quests\quests.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_changeproperty.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_debugprint.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_inventory.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_newstate.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_sequence.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\reward_sequencefinish.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\seqop_debugprint.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_entersector.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_meshentersector.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_propertychange.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_sequencefinish.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_timeout.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_trigger.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\msvc\plgmgr_quests.rc
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\mgr_quests.csplugin
# End Source File
# End Group
# End Target
# End Project
