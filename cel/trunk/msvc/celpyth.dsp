# Microsoft Developer Studio Project File - Name="celpyth" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=celpyth - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "celpyth.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "celpyth.mak" CFG="celpyth - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "celpyth - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "celpyth - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "celpyth - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "release\temp\celpyth"
# PROP Intermediate_Dir "release\temp\celpyth"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\\" /D "NDEBUG" /D "_WINDOWS" /D "_MT" /D "WIN32" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__"  /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 libcsutil.lib libcstool.lib libcsgeom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib delayimp.lib   /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMTD" /nodefaultlib:"LIBCMT" 
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\release\temp\celpyth\celpyth.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Moving output to CEL root.	copy $(TargetPath)  ..
# End Special Build Tool

!ELSEIF  "$(CFG)" == "celpyth - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\temp\celpyth"
# PROP Intermediate_Dir "debug\temp\celpyth"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\\" /I "..\..\CS\include" /D "_DEBUG" /D "_MT" /D "WIN32" /D "_WINDOWS" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__"  /D "CS_DEBUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libcsutil_d.lib libcstool_d.lib libcsgeom_d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib delayimp.lib   /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"d:\kwf\prog\CS\mk\visualc\csdebug\libs" /libpath:"d:\kwf\prog\CS\mk\visualc\csdebug\bin\libs" 
# Begin Special Build Tool
TargetPath=.\debug\temp\celpyth\celpyth.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Moving output to CEL root.	copy $(TargetPath)  ..
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "celpyth - Win32 Release"
# Name "celpyth - Win32 Debug"

# End Target
# End Project
