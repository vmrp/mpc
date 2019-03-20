# Microsoft Developer Studio Project File - Name="simulator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=simulator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "simulator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "simulator.mak" CFG="simulator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "simulator - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "simulator - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "simulator - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "simulator - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "SDK_MOD" /D "SIM_MOD" /D "DEBUG" /D "_AFXDLL" /FR /FD /GZ /GZ /GZ /GZ /GZ /GZ /GZ /GZ /GZ /GZ /GZ /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 WS2_32.lib winmm.lib dsound.lib dxguid.lib sdklib.lib vfw32.lib SIM_mr_helperexb.lib simlib.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\simulator" /libpath:"..\simulator\directx"
# SUBTRACT LINK32 /profile /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "simulator - Win32 Release"
# Name "simulator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\simulator\simulator.cpp
# End Source File
# Begin Source File

SOURCE=..\simulator\simulator.rc
# End Source File
# Begin Source File

SOURCE=..\simulator\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\simulator\Check.h
# End Source File
# Begin Source File

SOURCE=..\simulator\checkmrp.h
# End Source File
# Begin Source File

SOURCE=..\simulator\ini.h
# End Source File
# Begin Source File

SOURCE=..\simulator\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\simulator\mrporting.h
# End Source File
# Begin Source File

SOURCE=..\simulator\OTHERPage.h
# End Source File
# Begin Source File

SOURCE=..\simulator\Register.h
# End Source File
# Begin Source File

SOURCE=..\simulator\Resource.h
# End Source File
# Begin Source File

SOURCE=..\simulator\ScreenPage.h
# End Source File
# Begin Source File

SOURCE=..\simulator\sim_api.h
# End Source File
# Begin Source File

SOURCE=..\simulator\sim_plat.h
# End Source File
# Begin Source File

SOURCE=..\simulator\sim_sound.h
# End Source File
# Begin Source File

SOURCE=..\simulator\sim_win.h
# End Source File
# Begin Source File

SOURCE=..\simulator\simulator.h
# End Source File
# Begin Source File

SOURCE=..\simulator\simulatorDoc.h
# End Source File
# Begin Source File

SOURCE=..\simulator\simulatorView.h
# End Source File
# Begin Source File

SOURCE=..\simulator\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\simulator\TraceWin.h
# End Source File
# Begin Source File

SOURCE=..\simulator\UAPage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\simulator\res\1281601.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\1281602.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\12816102.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\1762201.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\1762202.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\2403201.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\2403202.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\check.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\check1.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\focus.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\radio.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\radio1.bmp
# End Source File
# Begin Source File

SOURCE=..\simulator\res\simulator.ico
# End Source File
# Begin Source File

SOURCE=..\simulator\res\simulator.rc2
# End Source File
# Begin Source File

SOURCE=..\simulator\res\simulatorDoc.ico
# End Source File
# Begin Source File

SOURCE=..\simulator\res\unfocus.bmp
# End Source File
# End Group
# End Target
# End Project
