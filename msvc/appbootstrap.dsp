# Microsoft Developer Studio Project File - Name="appbootstrap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=appbootstrap - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "appbootstrap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "appbootstrap.mak" CFG="appbootstrap - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "appbootstrap - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "appbootstrap - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "appbootstrap - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "release"
# PROP BASE Intermediate_Dir "release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "release\temp\appbootstrap"
# PROP Intermediate_Dir "release\temp\appbootstrap"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /Gy /GF /W3 /Gm /G5 /Ob2 /Og /Oi /Ot /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GX /G5 /Ob2 /Og /Oi /Ot /Oy /I "." /I ".." /I "..\include" /D "NDEBUG" /D "_MT" /D "_CONSOLE" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__" /FD /I ".." /I "..\include" /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" 
# ADD RSC /l 0x409 /d "NDEBUG" /fo".\release\temp\appbootstrap\appbootstrap.res"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib advapi32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 shell32.lib gdi32.lib user32.lib advapi32.lib libcstool.lib libcsgfx.lib libcsgeom.lib libcsutil.lib /nologo /subsystem:console /machine:I386 /out:"release\temp\appbootstrap\bootstrap.exe"  /OPT:NOREF
# Begin Special Build Tool
TargetPath".\release\temp\appbootstrap\bootstrap.exe"
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Moving output to build root.	copy "$(TargetPath)" ..
# End Special Build Tool

!ELSEIF  "$(CFG)" == "appbootstrap - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "debug"
# PROP BASE Intermediate_Dir "debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\temp\appbootstrap"
# PROP Intermediate_Dir "debug\temp\appbootstrap"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /vmb /vms /MDd /W3 /Gm /GR /GX /G5 /ZI /Od /I "." /I ".." /I "..\include" /D "_DEBUG" /D "_MT" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__" /D "CS_DEBUG" /FD /I ".." /I "..\include" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" 
# ADD RSC /l 0x409 /d "_DEBUG" /d "CS_DEBUG" /fo".\debug\temp\appbootstrap\appbootstrap.res"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib advapi32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 shell32.lib gdi32.lib user32.lib advapi32.lib libcstool_d.lib libcsgfx_d.lib libcsgeom_d.lib libcsutil_d.lib /nologo /subsystem:console /debug /machine:I386 /out:"debug\temp\appbootstrap\bootstrap.exe" /pdbtype:sept 
# Begin Special Build Tool
TargetPath=".\debug\temp\appbootstrap\bootstrap.exe"
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Moving output to build root.	copy "$(TargetPath)" ..
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "appbootstrap - Win32 Debug"
# Name "appbootstrap - Win32 Release"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\bootstrap\bootstrap.h
# End Source File

# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\msvc\appbootstrap.rc
# End Source File

# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\bootstrap\bootstrap.cpp
# End Source File

# End Group

# End Target
# End Project
