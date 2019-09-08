# Microsoft Developer Studio Project File - Name="LibUIDK" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LibUIDK - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibUIDK.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibUIDK.mak" CFG="LibUIDK - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LibUIDK - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LibUIDK - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LibUIDK - Win32 Debug Static" (based on "Win32 (x86) Static Library")
!MESSAGE "LibUIDK - Win32 Release Static" (based on "Win32 (x86) Static Library")
!MESSAGE "LibUIDK - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "LibUIDK - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "LibUIDK - Win32 Debug Unicode Static" (based on "Win32 (x86) Static Library")
!MESSAGE "LibUIDK - Win32 Release Unicode Static" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LibUIDK - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Release60"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /Fd"Lib/LibUIDK60.pdb" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\LibUIDK60.lib"

!ELSEIF  "$(CFG)" == "LibUIDK - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Debug60"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /Fd"Lib/LibUIDK60d.pdb" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\LibUIDK60d.lib"

!ELSEIF  "$(CFG)" == "LibUIDK - Win32 Debug Static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LibUIDK___Win32_Debug_Static"
# PROP BASE Intermediate_Dir "LibUIDK___Win32_Debug_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Debugs60"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /Fd"Lib/LibUIDKs60d.pdb" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\LibUIDKs60d.lib"

!ELSEIF  "$(CFG)" == "LibUIDK - Win32 Release Static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LibUIDK___Win32_Release_Static"
# PROP BASE Intermediate_Dir "LibUIDK___Win32_Release_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Releases60"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /Fd"Lib/LibUIDKs60.pdb" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\LibUIDKs60.lib"

!ELSEIF  "$(CFG)" == "LibUIDK - Win32 Debug Unicode"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LibUIDK___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "LibUIDK___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Debug60u"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /Fd"Lib/LibUIDK60ud.pdb" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\LibUIDK60ud.lib"

!ELSEIF  "$(CFG)" == "LibUIDK - Win32 Release Unicode"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LibUIDK___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "LibUIDK___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Release60u"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /Fd"Lib/LibUIDK60u.pdb" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\LibUIDK60u.lib"

!ELSEIF  "$(CFG)" == "LibUIDK - Win32 Debug Unicode Static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LibUIDK___Win32_Debug_Unicode_Static"
# PROP BASE Intermediate_Dir "LibUIDK___Win32_Debug_Unicode_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Debugs60u"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /Yu"stdafx.h" /Fd"Lib/LibUIDKs60ud.pdb" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug60u\LibUIDK60ud.lib"
# ADD LIB32 /nologo /out:"Lib\LibUIDKs60ud.lib"

!ELSEIF  "$(CFG)" == "LibUIDK - Win32 Release Unicode Static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LibUIDK___Win32_Release_Unicode_Static"
# PROP BASE Intermediate_Dir "LibUIDK___Win32_Release_Unicode_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Releases60u"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /Fp"Releases60u/LibUIDKs60u.pch" /Yu"stdafx.h" /Fd"Lib/LibUIDKs60u.pdb" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release60u\LibUIDK60u.lib"
# ADD LIB32 /nologo /out:"Lib\LibUIDKs60u.lib"

!ENDIF 

# Begin Target

# Name "LibUIDK - Win32 Release"
# Name "LibUIDK - Win32 Debug"
# Name "LibUIDK - Win32 Debug Static"
# Name "LibUIDK - Win32 Release Static"
# Name "LibUIDK - Win32 Debug Unicode"
# Name "LibUIDK - Win32 Release Unicode"
# Name "LibUIDK - Win32 Debug Unicode Static"
# Name "LibUIDK - Win32 Release Unicode Static"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "RichEdit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageOleCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\IMRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\IMRichEditCtrlBase.cpp
# End Source File
# Begin Source File

SOURCE=.\OleCtrlBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinRichEditCtrlBase.cpp
# End Source File
# Begin Source File

SOURCE=.\WLIMRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WLRichEditCtrl.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ControlBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateControls.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\CUSTSITE.CPP
# End Source File
# Begin Source File

SOURCE=.\DataType.cpp
# End Source File
# Begin Source File

SOURCE=.\Debug.cpp
# End Source File
# Begin Source File

SOURCE=.\DockPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\DragDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\GridPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIDocManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIImage.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIMDIChildWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIMDIClientWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIMDIFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIMultiDocTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIView.cpp
# End Source File
# Begin Source File

SOURCE=.\IUIWinApp.cpp
# End Source File
# Begin Source File

SOURCE=.\MemStdioFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\NewWindowsUIPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDropTargetEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel.cpp
# End Source File
# Begin Source File

SOURCE=.\PicCharacterCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PictureCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RectCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Render.cpp
# End Source File
# Begin Source File

SOURCE=.\ResMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchLightProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinButton.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinDateTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinHotKeyCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinIPAddressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinSliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinSpinButtonCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SliderCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitterBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StackPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SubtitleWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskWndMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ThumbnailCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\treeview.cpp
# End Source File
# Begin Source File

SOURCE=.\tvmem.cpp
# End Source File
# Begin Source File

SOURCE=.\tvpaint.cpp
# End Source File
# Begin Source File

SOURCE=.\tvscroll.cpp
# End Source File
# Begin Source File

SOURCE=.\UIMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\UIWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UIWndBase.cpp
# End Source File
# Begin Source File

SOURCE=.\UniformGridPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WLButton.cpp
# End Source File
# Begin Source File

SOURCE=.\WLLine.cpp
# End Source File
# Begin Source File

SOURCE=.\WLPicture.cpp
# End Source File
# Begin Source File

SOURCE=.\WLSliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WLSplitterBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WLText.cpp
# End Source File
# Begin Source File

SOURCE=.\WLWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CUSTSITE.H
# End Source File
# Begin Source File

SOURCE=.\LibUIDK.h
# End Source File
# Begin Source File

SOURCE=.\ResourceMgr.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\targetver.h
# End Source File
# Begin Source File

SOURCE=.\TextHost.h
# End Source File
# Begin Source File

SOURCE=.\treeview.h
# End Source File
# Begin Source File

SOURCE=.\Utils.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "License"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\License\global.h
# End Source File
# Begin Source File

SOURCE=.\License\License.cpp
# End Source File
# Begin Source File

SOURCE=.\License\License.h
# End Source File
# Begin Source File

SOURCE=.\License\MD5.cpp
# End Source File
# Begin Source File

SOURCE=.\License\MD5.h
# End Source File
# Begin Source File

SOURCE=.\License\md5c.cpp
# End Source File
# Begin Source File

SOURCE=.\License\MD5h.h
# End Source File
# End Group
# Begin Group "ControlBase"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ButtonBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlMember.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
