# Microsoft Developer Studio Project File - Name="MPCrun" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=MPCrun - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MPCrun.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MPCrun.mak" CFG="MPCrun - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MPCrun - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MPCrun - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MPCrun - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "MPCrun - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MPCrun___Win32_Debug"
# PROP BASE Intermediate_Dir "MPCrun___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MPCrun___Win32_Debug"
# PROP Intermediate_Dir "MPCrun___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Gm /GX /ZI /Od /I "..\include" /I "..\..\src\run" /I "..\..\interpreter\src" /I "..\..\interpreter\syslib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "SDK_MOD" /D "SIM_MOD" /D "_MPC_VC6_" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "MPCrun - Win32 Release"
# Name "MPCrun - Win32 Debug"
# Begin Group "interpreter"

# PROP Default_Filter ""
# Begin Group "syslib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\interpreter\syslib\m_base.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\syslib\m_ctype.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\syslib\m_net.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\syslib\m_sound.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\syslib\mrc_base_i.h
# End Source File
# End Group
# Begin Group "src No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\interpreter\src\clibrary.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\expression.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\heap.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\include.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\interpreter.h
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\lex.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\mrc_base_i.h
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\myDebug.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\myDebug.h
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\myfunc.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\myfunc.h
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\parse.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\picoc.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\picoc.h
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\platform.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\platform.h
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\table.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\type.c
# End Source File
# Begin Source File

SOURCE=..\..\interpreter\src\variable.c
# End Source File
# End Group
# End Group
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\run\main.c
# End Source File
# Begin Source File

SOURCE=..\..\src\run\momo.c
# End Source File
# Begin Source File

SOURCE=..\..\src\run\momo.h
# End Source File
# End Group
# End Target
# End Project
