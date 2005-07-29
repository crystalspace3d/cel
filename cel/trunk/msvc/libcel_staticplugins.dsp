# Microsoft Developer Studio Project File - Name="libcel_staticplugins" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libcel_staticplugins - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libcel_staticplugins.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libcel_staticplugins.mak" CFG="libcel_staticplugins - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libcel_staticplugins - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libcel_staticplugins - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libcel_staticplugins - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "release"
# PROP BASE Intermediate_Dir "release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\out\release\build\libcel_staticplugins\"
# PROP Intermediate_Dir "..\out\release\build\libcel_staticplugins\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /Gy /GF /MD /Ob2 /Og /Oi /Ot /Oy /D "_LIB" /D "_MT" /D "_MBCS" /D "NDEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED" /D "WIN32" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include" /I "..\..\..\CS\include" /I "..\..\..\CS\include\csutil\win32" /I "$(CRYSTAL)\include" /I "$(CRYSTAL)\include\crystalspace" /I "$(CRYSTAL)\include\csutil\win32"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "NDEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\release\build\libcel_staticplugins\libcel_staticplugins.res" /D "NDEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include" /i "..\..\..\CS\include" /i "..\..\..\CS\include\csutil\win32" /i "$(CRYSTAL)\include" /i "$(CRYSTAL)\include\crystalspace" /i "$(CRYSTAL)\include\csutil\win32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\out\release\libs\libcel_staticplugins.lib"
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib libcrystalspace.lib  /nologo /version:4.0 /machine:I386 /OPT:NOREF /subsystem:windows /libpath:"..\..\..\CS\out\release\libs" /libpath:"..\..\..\CS\libs\csutil\win32" /libpath:"$(CRYSTAL)\out\release\libs" /libpath:"$(CRYSTAL)\libs" /libpath:"$(CRYSTAL)\libs\csutil\win32" 
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "libcel_staticplugins - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "debug"
# PROP BASE Intermediate_Dir "debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\out\debug\build\libcel_staticplugins\"
# PROP Intermediate_Dir "..\out\debug\build\libcel_staticplugins\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /vmb /vms /W3 /Gm /G5 /D "WIN32" /FD /c
# ADD CPP /nologo /vmb /vms /W3 /Gm /GX /G5 /FD /c /GR /MDd /ZI /Od /D "_LIB" /D "_MT" /D "_MBCS" /D "_DEBUG" /D "CS_DEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED" /D "WIN32" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__"  /I "." /I ".." /I "..\include" /I "..\..\..\CS\include" /I "..\..\..\CS\include\csutil\win32" /I "$(CRYSTAL)\include" /I "$(CRYSTAL)\include\crystalspace" /I "$(CRYSTAL)\include\csutil\win32"
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32 /D "_DEBUG" /D "CS_DEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /fo".\..\out\debug\build\libcel_staticplugins\libcel_staticplugins.res" /D "_DEBUG" /D "CS_DEBUG" /D "_LIB" /D "_WINDOWS" /D "CS_STATIC_LINKED" /D "CS_WIN32_CSCONFIG" /D "__CRYSTAL_SPACE__" /i "." /i ".." /i "..\include" /i "..\..\..\CS\include" /i "..\..\..\CS\include\csutil\win32" /i "$(CRYSTAL)\include" /i "$(CRYSTAL)\include\crystalspace" /i "$(CRYSTAL)\include\csutil\win32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\out\debug\libs\libcel_staticplugins_d.lib"
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib /nologo /machine:I386
# ADD LINK32 shell32.lib user32.lib gdi32.lib advapi32.lib libcrystalspace_d.lib  /nologo /version:4.0 /machine:I386 /debug /pdbtype:sept /subsystem:windows /libpath:"..\..\..\CS\out\debug\libs" /libpath:"..\..\..\CS\libs\csutil\win32" /libpath:"$(CRYSTAL)\out\debug\libs" /libpath:"$(CRYSTAL)\libs" /libpath:"$(CRYSTAL)\libs\csutil\win32" 
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "libcel_staticplugins - Win32 Release"
# Name "libcel_staticplugins - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\msvc\libaddon_celentity_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libaddon_celentitytpl_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libaddon_questdef_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libaddon_xmlscripts_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libbltest_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libblxml_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libcpersist_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libmgr_billboard_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libmgr_quests_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfactormove_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfbillboard_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfdefcam_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfdynmove_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfengine_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfinput_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfinv_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpflight_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfmechanics_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfmesh_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfmove_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfmover_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfportal_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfquest_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfsimpcam_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpftest_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpftools_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpftrigger_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libpfzone_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libstdphyslayer_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\msvc\libxmlpersist_staticreg.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\addons\celentity\celentity.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\addons\celentitytpl\celentitytpl.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\addons\questdef\questdef.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\addons\xmlscripts\xmlscripts.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\test\behave.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\test\bl.cpp
# End Source File
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
# Begin Source File

SOURCE=..\plugins\persist\classic\cpersist.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\persist\xml\xmlpersist.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\actormove\actormove.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\billboard\billboard.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\defcam\defcam.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\dynmove\dynmove.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\engine\engfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\input\inpfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\inv\invfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\light\lightfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mechanics\mechanics.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mechanics\thruster_controller.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mechanics\thruster_reactionary.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mesh\meshfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\colldet.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\linmove.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\movefact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\navgraph.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\navgraphrulesfps.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mover\moverfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\portal\portalfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\quest\questfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\simpcam\simpcam.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\test\testfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\tools\propfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\tools\spawn.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\tools\timerfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\tools\toolfact.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\trigger\trigger.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\zone\zone.cpp
# End Source File
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
# Begin Source File

SOURCE=..\plugins\tools\billboard\billboard.cpp
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

SOURCE=..\plugins\tools\quests\seqop_light.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\seqop_movepath.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\seqop_transform.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_entersector.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_inventory.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_meshentersector.cpp
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_meshsel.cpp
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

SOURCE=..\plugins\addons\celentity\celentity.h
# End Source File
# Begin Source File

SOURCE=..\plugins\addons\celentitytpl\celentitytpl.h
# End Source File
# Begin Source File

SOURCE=..\plugins\addons\questdef\questdef.h
# End Source File
# Begin Source File

SOURCE=..\plugins\addons\xmlscripts\xmlscripts.h
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\test\behave.h
# End Source File
# Begin Source File

SOURCE=..\plugins\behaviourlayer\test\bl.h
# End Source File
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
# Begin Source File

SOURCE=..\plugins\persist\classic\cpersist.h
# End Source File
# Begin Source File

SOURCE=..\plugins\persist\xml\xmlpersist.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\actormove\actormove.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\billboard\billboard.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\defcam\defcam.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\dynmove\dynmove.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\engine\engfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\input\inpfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\inv\invfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\light\lightfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mechanics\common.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mechanics\environment.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mechanics\mechanics.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mechanics\thruster_controller.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mechanics\thruster_reactionary.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mesh\meshfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\colldet.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\linmove.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\movefact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\navgraph.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\move\navgraphrulesfps.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\mover\moverfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\portal\portalfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\quest\questfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\simpcam\simpcam.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\test\testfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\tools\propfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\tools\spawn.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\tools\timerfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\tools\toolfact.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\trigger\trigger.h
# End Source File
# Begin Source File

SOURCE=..\plugins\propclass\zone\zone.h
# End Source File
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
# Begin Source File

SOURCE=..\plugins\tools\billboard\billboard.h
# End Source File
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

SOURCE=..\plugins\tools\quests\seqop_light.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\seqop_movepath.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\seqop_transform.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_entersector.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_inventory.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_meshentersector.h
# End Source File
# Begin Source File

SOURCE=..\plugins\tools\quests\trig_meshsel.h
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
