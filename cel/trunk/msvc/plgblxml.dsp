# Microsoft Developer Studio Project File - Name="plgblxml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=plgblxml - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "plgblxml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plgblxml.mak" CFG="plgblxml - Win32 Debug"
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
# PROP Output_Dir "release\temp\plgblxml"
# PROP Intermediate_Dir "release\temp\plgblxml"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /Gy /GF /W3 /Gm /G5 /Ob2 /Og /Oi /Ot /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# ADD CPP /nologo /vmb /vms /Gy /GF /MD /W3 /Gm /GX /G5 /Ob2 /Og /Oi /Ot /Oy /I "." /I ".." /I "..\include" /D "NDEBUG" /D "_MT" /D "_WINDOWS" /D "WIN32" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /FD /I ".." /I "..\include" /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" 
# ADD RSC /l 0x409 /d "NDEBUG" /fo".\release\temp\plgblxml\plgblxml.res"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib delayimp.lib libcstool.lib libcsgfx.lib libcsgeom.lib libcsutil.lib libcstool.lib libcsgfx.lib libcsgeom.lib libcsutil.lib  /nologo /version:4.0 /subsystem:windows /dll /machine:I386 /out:"release\temp\plgblxml\blxml.dll"  /OPT:NOREF
# Begin Special Build Tool
TargetPath=".\release\temp\plgblxml\blxml.dll"
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Moving output to build root.	copy "$(TargetPath)" ..
# End Special Build Tool

!ELSEIF  "$(CFG)" == "plgblxml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "debug"
# PROP BASE Intermediate_Dir "debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\temp\plgblxml"
# PROP Intermediate_Dir "debug\temp\plgblxml"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# ADD CPP /nologo /vmb /vms /MDd /W3 /Gm /GR /GX /G5 /ZI /Od /I "." /I ".." /I "..\include" /D "_DEBUG" /D "_MT" /D "WIN32" /D "_WINDOWS" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /D "CS_DEBUG" /FD /I ".." /I "..\include" /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" 
# ADD RSC /l 0x409 /d "_DEBUG" /d "CS_DEBUG" /fo".\debug\temp\plgblxml\plgblxml.res"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib delayimp.lib libcstool_d.lib libcsgfx_d.lib libcsgeom_d.lib libcsutil_d.lib  /nologo /version:1.0 /subsystem:windows /dll /profile /debug /machine:I386 /out:"debug\temp\plgblxml\blxml.dll" 
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
TargetPath=".\debug\temp\plgblxml\blxml.dll"
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Moving output to build root.	copy "$(TargetPath)" ..
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "plgblxml - Win32 Debug"
# Name "plgblxml - Win32 Release"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\behave_xml.h
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\blxml.h
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\token.h
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\xmlscript.h
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
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\behave_xml.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\blxml.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\token.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\xml\xmlscript.cpp
# End Source File

# End Group

# End Target
# End Project
