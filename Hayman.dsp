# Microsoft Developer Studio Project File - Name="Hayman" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Hayman - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Hayman.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Hayman.mak" CFG="Hayman - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Hayman - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Hayman - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Hayman - Win32 Debug Unicode" (based on "Win32 (x86) Application")
!MESSAGE "Hayman - Win32 Release Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Hayman - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"_PCH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /ignore:4089
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Hayman - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"_PCH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Hayman - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Hayman___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "Hayman___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug Unicode"
# PROP Intermediate_Dir "Debug Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"_PCH.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /Yu"_PCH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Hayman - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Hayman___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "Hayman___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release Unicode"
# PROP Intermediate_Dir "Release Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"_PCH.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /Yu"_PCH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /pdb:none /debug /machine:I386 /OPT:REF /ignore:4089
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\Finch\SymEditSmall.exe "Release Unicode\Hayman.exe"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Hayman - Win32 Release"
# Name "Hayman - Win32 Debug"
# Name "Hayman - Win32 Debug Unicode"
# Name "Hayman - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\_Include\_PCH.cpp
# ADD CPP /Yc"_PCH.h"
# End Source File
# Begin Source File

SOURCE=.\C_DataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\C_Engine.cpp
# End Source File
# Begin Source File

SOURCE=.\C_Options.cpp
# End Source File
# Begin Source File

SOURCE=.\Hayman.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\_Include\_C_Align.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_AppDialog.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Application.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_AutoUpdate.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Buffer.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_CoInitializeEx.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Color.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_COM.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_ComboBox.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_ComboBoxX.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_CommandLine.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Container.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_CriticalSection.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_DataBase.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Debug.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Event.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Files.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Grip.h
# End Source File
# Begin Source File

SOURCE=..\HART\_C_HART.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_HDC.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_ImageList.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_INI.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Internet.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Language.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Library.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Link.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_List.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_ListView.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Log.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Menu.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_MenuX.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_MRU.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_ProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Rebar.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Registry.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Resources.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_SerialPort.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Settings.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_SetupDi.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_String.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_StringTable.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_SubClassing.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_TaskbarList.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Theme.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Thread.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_ThreadEngine.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Time.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_ToolBar.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_TrackMouseEvent.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Transparent.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_Windows.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_WindowsAPI.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_WindowsHook.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_XML.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_XML_Interface.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_XMLv1.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_C_XMLv2.h
# End Source File
# Begin Source File

SOURCE=..\_Include\_PCH.h
# End Source File
# Begin Source File

SOURCE=..\_SDKs\Windows\_WDK81\Include\um\bthledef.h
# End Source File
# Begin Source File

SOURCE=..\_SDKs\Windows\_DDK8\Include\shared\d3dkmdt.h
# End Source File
# Begin Source File

SOURCE=..\_SDKs\Windows\Direct3D\d3dkmt.h
# End Source File
# Begin Source File

SOURCE=..\_SDKs\Windows\_WDK81\Include\km\d3dkmthk.h
# End Source File
# Begin Source File

SOURCE=..\_SDKs\Windows\_DDK8\Include\shared\d3dukmdt.h
# End Source File
# Begin Source File

SOURCE=..\_SDKs\Windows\_DDK7\inc\api\wincodec.h
# End Source File
# Begin Source File

SOURCE=..\_SDKs\Windows\_SDK7\Include\winhttp.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\C_DataBase.h
# End Source File
# Begin Source File

SOURCE=.\C_Engine.h
# End Source File
# Begin Source File

SOURCE=.\C_Options.h
# End Source File
# Begin Source File

SOURCE=.\Hayman.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\_Resources\App.ico
# End Source File
# Begin Source File

SOURCE=.\Application.manifest.h
# End Source File
# Begin Source File

SOURCE=.\Application.manifest.xml
# End Source File
# Begin Source File

SOURCE=.\_Resources\Command.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\Device.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\DotBlue.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\DotGreen.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\DotRed.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\DotYellow.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\ExclamationMark.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\Hand_Own.cur
# End Source File
# Begin Source File

SOURCE=.\_Resources\Info.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\Log.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\MRU.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\Options.ico
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resources.rc
# End Source File
# Begin Source File

SOURCE=.\_Resources\Save.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\SendCommand.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\SerialPort.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\Start.ico
# End Source File
# Begin Source File

SOURCE=.\_Resources\Stop.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\_Info.txt

!IF  "$(CFG)" == "Hayman - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Hayman - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Hayman - Win32 Debug Unicode"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Hayman - Win32 Release Unicode"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
