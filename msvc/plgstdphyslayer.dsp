# Microsoft Developer Studio Project File - Name="plgstdphyslayer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=plgstdphyslayer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "plgstdphyslayer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plgstdphyslayer.mak" CFG="plgstdphyslayer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plgstdphyslayer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plgstdphyslayer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "release\temp\plgstdphyslayer"
# PROP Intermediate_Dir "release\temp\plgstdphyslayer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PFMOVE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\" /D "NDEBUG" /D "_WINDOWS" /D "_MT" /D "WIN32" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__"  /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib delayimp.lib libcstool.lib libcsgfx.lib libcsgeom.lib libcsutil.lib  /nologo /dll /machine:I386 /nodefaultlib:"LIBCMTD" 
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\release\temp\plgstdphyslayer\stdphyslayer.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Moving output to CEL root.	copy $(TargetPath)  ..	
# End Special Build Tool

!ELSEIF  "$(CFG)" == "plgstdphyslayer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\temp\plgstdphyslayer"
# PROP Intermediate_Dir "debug\temp\plgstdphyslayer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PFMOVE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\" /I /D "_DEBUG" /D "_MT" /D "WIN32" /D "_WINDOWS" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__"  /D "CS_DEBUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib delayimp.lib libcstool_d.lib libcsgfx_d.lib libcsgeom_d.lib libcsutil_d.lib  /nologo /dll /debug /machine:I386 /pdbtype:sept 
# Begin Special Build Tool
TargetPath=.\debug\temp\plgstdphyslayer\stdphyslayer.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Moving output to CEL root.	copy $(TargetPath)  ..	
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "plgstdphyslayer - Win32 Release"
# Name "plgstdphyslayer - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\stdphyslayer\entity.h
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

# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\stdphyslayer\entity.cpp
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

# End Target
# End Project
