# Microsoft Developer Studio Project File - Name="libmgr_quests_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libmgr_quests_static - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libmgr_quests_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libmgr_quests_static.mak" CFG="libmgr_quests_static - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libmgr_quests_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libmgr_quests_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libmgr_quests_static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "release"
# PROP BASE Intermediate_Dir "release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\out\release\build\libmgr_quests_static\"
# PROP Intermediate_Dir "..\out\release\build\libmgr_quests_static\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /Gy /GF /MD /Ob2 /Og /Oi /Ot /Oy /D "_LIB" /D "_MT" /D "_MBCS" /D "NDEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED" /D "WIN32" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include" /I "..\..\..\CS\include" /I "$(CRYSTAL)\include" /I "$(CRYSTAL)\include\crystalspace"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "NDEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\release\build\libmgr_quests_static\libmgr_quests_static.res" /D "NDEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include" /i "..\..\..\CS\include" /i "$(CRYSTAL)\include" /i "$(CRYSTAL)\include\crystalspace"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\out\release\libs\libmgr_quests_static.lib"
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib libcrystalspace.lib  /nologo /version:4.0 /machine:I386 /OPT:NOREF /subsystem:windows /libpath:"..\..\..\CS\out\release\libs" /libpath:"..\..\..\CS\libs\csutil\win32" /libpath:"$(CRYSTAL)\out\release\libs" /libpath:"$(CRYSTAL)\libs" /libpath:"$(CRYSTAL)\libs\csutil\win32" 
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "libmgr_quests_static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "debug"
# PROP BASE Intermediate_Dir "debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\out\debug\build\libmgr_quests_static\"
# PROP Intermediate_Dir "..\out\debug\build\libmgr_quests_static\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /GR /MDd /ZI /Od /D "_LIB" /D "_MT" /D "_MBCS" /D "_DEBUG" /D "CS_DEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED" /D "WIN32" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include" /I "..\..\..\CS\include" /I "$(CRYSTAL)\include" /I "$(CRYSTAL)\include\crystalspace"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "_DEBUG" /D "CS_DEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\debug\build\libmgr_quests_static\libmgr_quests_static.res" /D "_DEBUG" /D "CS_DEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include" /i "..\..\..\CS\include" /i "$(CRYSTAL)\include" /i "$(CRYSTAL)\include\crystalspace"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\out\debug\libs\libmgr_quests_static_d.lib"
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib libcrystalspace_d.lib  /nologo /version:4.0 /machine:I386 /debug /pdbtype:sept /subsystem:windows /libpath:"..\..\..\CS\out\debug\libs" /libpath:"..\..\..\CS\libs\csutil\win32" /libpath:"$(CRYSTAL)\out\debug\libs" /libpath:"$(CRYSTAL)\libs" /libpath:"$(CRYSTAL)\libs\csutil\win32" 
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "libmgr_quests_static - Win32 Release"
# Name "libmgr_quests_static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\msvc\libmgr_quests_staticreg.cpp
# End Source File
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

SOURCE=..\plugins\tools\quests\seqop_transform.cpp
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

SOURCE=..\plugins\tools\quests\seqop_transform.h
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
# End Target
# End Project
