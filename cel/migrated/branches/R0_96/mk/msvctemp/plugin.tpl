# Microsoft Developer Studio Project File - Name="%project%" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=%project% - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "%makefile%".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "%makefile%" CFG="%project% - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "%project% - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "%project% - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "%project% - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "csrelease"
# PROP BASE Intermediate_Dir "csrelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "csrelease\temp\%project%"
# PROP Intermediate_Dir "csrelease\temp\%project%"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /Gy /GF /MT /W4 /Gm /G5 /Ob2 /Og /Oi /Ot /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# ADD CPP /nologo /vmb /vms /Gy /GF /MD /W4 /Gm /G5 /Ob2 /Og /Oi /Ot /Oy /I "..\.." /I "..\..\include\cssys\win32" /I "..\..\include" /I "..\..\libs" /I "..\..\support" /I "..\..\apps" /I "..\..\plugins" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__" /FD %cflags% /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" 
# ADD RSC /l 0x409 /d "NDEBUG" /i "..\..\include\cssys\win32" /i "..\..\include"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib gdi32.lib advapi32.lib %libs% /nologo /version:4.0 /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /nodefaultlib:"LIBCMT" /nodefaultlib:"LIBCMTD" /out:"csrelease\temp\%project%\%target%" /libpath:"..\..\libs\cssys\win32\libs" %lflags% /OPT:NOREF
# Begin Special Build Tool
TargetPath=".\csrelease\temp\%project%\%target%"
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Moving output to CS root.	copy "$(TargetPath)"  ..\..	echo Moving output to MSVC Release Bin.	copy "$(TargetPath)"  csrelease\bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "%project% - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "csdebug"
# PROP BASE Intermediate_Dir "csdebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "csdebug\temp\%project%"
# PROP Intermediate_Dir "csdebug\temp\%project%"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /MTd /W4 /Gm /G5 /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# ADD CPP /nologo /vmb /vms /MDd /W4 /Gm /G5 /ZI /Od /I "..\..\plugins" /I "..\.." /I "..\..\include\cssys\win32" /I "..\..\include" /I "..\..\libs" /I "..\..\support" /I "..\..\apps" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WIN32_VOLATILE" /D "__CRYSTAL_SPACE__" /D "CS_DEBUG" /D "CS_EXTENSIVE_MEMDEBUG" /FD %cflags% /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" 
# ADD RSC /l 0x409 /d "_DEBUG" /d "CS_DEBUG" /i "..\..\include\cssys\win32" /i "..\..\include"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib gdi32.lib advapi32.lib %libs% /nologo /version:1.0 /subsystem:windows /dll /profile /debug /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /nodefaultlib:"LIBCMTD" /out:"csdebug\temp\%project%\%target%" /libpath:"..\..\libs\cssys\win32\libs" %lflags%
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
TargetPath=".\csdebug\temp\%project%\%target%"
SOURCE="$(InputPath)"
PostBuild_Cmds=copy %0 post_build_debug.txt	echo Moving output to CS root.	copy "$(TargetPath)"  ..\..	echo Moving output to MSVC Debug Bin.	copy "$(TargetPath)"  csdebug\bin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "%project% - Win32 Release"
# Name "%project% - Win32 Debug"
%groups%
# End Target
# End Project