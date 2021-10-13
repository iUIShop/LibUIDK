# Microsoft Developer Studio Project File - Name="$$root$$" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=$$root$$ - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "$$root$$.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "$$root$$.mak" CFG="$$root$$ - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "$$root$$ - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "$$root$$ - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "$$root$$ - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Bin/$$root$$.exe"

!ELSEIF  "$(CFG)" == "$$root$$ - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Bin/$$root$$_D.exe" /pdbtype:sept

!ENDIF

# Begin Target

# Name "$$root$$ - Win32 Release"
# Name "$$root$$ - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
$$IF(IM)
# Begin Source File

SOURCE=.\ChatFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\FriendPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FriendsTree.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupPage.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RecentPage.cpp
# End Source File
# Begin Source File

SOURCE=.\StartMenu.cpp
# End Source File
$$ENDIF // IM
$$IF(MDI)
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassViewWnd.cpp
# End Source File
$$ENDIF // MDI
# Begin Source File

SOURCE=.\$$root$$.cpp
# End Source File
# Begin Source File

SOURCE=.\$$root$$.rc
# End Source File
$$IF(MDI)
# Begin Source File

SOURCE=.\$$root$$Doc.cpp
# End Source File
# Begin Source File

SOURCE=.\$$root$$View.cpp
# End Source File
# Begin Source File

SOURCE=.\FileViewWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
$$ENDIF // MDI
# Begin Source File

SOURCE=.\MainWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
$$IF(IM)
# Begin Source File

SOURCE=.\ChatFrame.h
# End Source File
# Begin Source File

SOURCE=.\FriendPage.h
# End Source File
# Begin Source File

SOURCE=.\FriendsTree.h
# End Source File
# Begin Source File

SOURCE=.\GroupPage.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\RecentPage.h
# End Source File
# Begin Source File

SOURCE=.\StartMenu.h
# End Source File
$$ENDIF // IM
$$IF(MDI)
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ClassViewWnd.h
# End Source File
$$ENDIF // MDI
# Begin Source File

SOURCE=.\$$root$$.h
# End Source File
$$IF(MDI)
# Begin Source File

SOURCE=.\$$root$$Doc.h
# End Source File
# Begin Source File

SOURCE=.\$$root$$View.h
# End Source File
# Begin Source File

SOURCE=.\FileViewWnd.h
# End Source File
# Begin Source File

SOURCE=.\res\classview.bmp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
$$ENDIF // MDI
# Begin Source File

SOURCE=.\MainWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\$$root$$.ico
# End Source File
# Begin Source File

SOURCE=.\res\$$root$$.rc2
# End Source File
$$IF(MDI)
# Begin Source File

SOURCE=.\res\$$root$$Doc.ico
# End Source File
$$ENDIF
# End Group
$$IF(MDI)
# Begin Source File

SOURCE=.\$$root$$.reg
# End Source File
$$ENDIF
# End Target
# End Project
