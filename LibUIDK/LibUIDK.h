#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)

#include <AFXCMN.H>
#include <string>
#include <vector>
#include <map>
#include <afxole.h>
#include <afxtempl.h>
#include <comutil.h>
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>
#include <afxdtctl.h>
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")
#if(_MSC_VER <= 1200)
#include <mshtml.h>
#include <afxoledb.h>
#endif
#include <RichOle.h>
#include <TOM.h>
#include <imm.h>
#include <TextServ.h>


// Define LONG_PTR
#if !defined(LONG_PTR)
#if defined(_WIN64)
typedef __int64 LONG_PTR;
#else
typedef long LONG_PTR;
#endif
#endif


// Define ULONG_PTR
#ifndef ULONG_PTR
#if defined(_WIN64)
typedef unsigned __int64 ULONG_PTR;
#else
typedef unsigned long ULONG_PTR;
#endif //_WIN64
#endif //ULONG_PTR

#ifndef GWLP_WNDPROC
#define GWLP_WNDPROC GWL_WNDPROC
#endif

#ifndef GWLP_USERDATA
#define GWLP_USERDATA GWL_USERDATA
#endif


#ifndef UintToPtr
#define UintToPtr( ui )  ((VOID *)(UINT_PTR)((unsigned int)ui))
#endif

#ifndef PtrToUint
#define PtrToUint( p ) ((UINT)(UINT_PTR) (p))
#endif

#ifndef TME_NONCLIENT
#define TME_NONCLIENT   0x00000010
#endif

#ifndef WM_NCMOUSELEAVE
#define WM_NCMOUSELEAVE                 0x02A2
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif
#ifndef LWA_COLORKEY
#define LWA_COLORKEY            0x00000001
#endif
#ifndef LWA_ALPHA
#define LWA_ALPHA               0x00000002
#endif

#ifndef WS_EX_NOACTIVATE
#if(_WIN32_WINNT >= 0x0500)
#define WS_EX_NOACTIVATE        0x08000000L
#endif /* _WIN32_WINNT >= 0x0500 */
#endif

#ifndef IDT_SCROLLWAIT
#define IDT_SCROLLWAIT  43
#endif


#ifndef MAXUINT
#define MAXUINT     ((UINT)~((UINT)0))
#endif

#ifndef MAXINT
#define MAXINT      ((INT)(MAXUINT >> 1))
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(A) (sizeof(A) / sizeof(A[0]))
#endif

#ifndef RECTWIDTH
#define RECTWIDTH(rc)   ((rc).right-(rc).left)
#endif

#ifndef RECTHEIGHT
#define RECTHEIGHT(rc)  ((rc).bottom-(rc).top)
#endif

#if _WIN32_WINNT < 0x0600
#ifndef LVS_EX_JUSTIFYCOLUMNS
#define LVS_EX_JUSTIFYCOLUMNS   0x00200000  // Icons are lined up in columns that use up the whole view area.
#endif
#endif // _WIN32_WINNT >= 0x0600

#ifndef __in
#define __in
#endif

#ifndef __out
#define __out
#endif

#ifndef __Inout
#define __Inout
#endif

#ifndef __inout
#define __inout
#endif

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif

//
// _WIN32_WINNT version constants
//
#ifndef _WIN32_WINNT_NT4
#define _WIN32_WINNT_NT4                    0x0400
#endif

#ifndef _WIN32_WINNT_WIN2K
#define _WIN32_WINNT_WIN2K                  0x0500
#endif

#ifndef _WIN32_WINNT_WINXP
#define _WIN32_WINNT_WINXP                  0x0501
#endif

#ifndef _WIN32_WINNT_WS03
#define _WIN32_WINNT_WS03                   0x0502
#endif

#ifndef _WIN32_WINNT_WIN6
#define _WIN32_WINNT_WIN6                   0x0600
#endif

#ifndef _WIN32_WINNT_VISTA
#define _WIN32_WINNT_VISTA                  0x0600
#endif

#ifndef _WIN32_WINNT_WS08
#define _WIN32_WINNT_WS08                   0x0600
#endif

#ifndef _WIN32_WINNT_LONGHORN
#define _WIN32_WINNT_LONGHORN               0x0600
#endif

#ifndef _WIN32_WINNT_WIN7
#define _WIN32_WINNT_WIN7                   0x0601
#endif

#ifndef _WIN32_WINNT_WIN8
#define _WIN32_WINNT_WIN8                   0x0602
#endif

#ifndef _WIN32_WINNT_WINBLUE
#define _WIN32_WINNT_WINBLUE                0x0603
#endif

#if(_MSC_VER <= 1200) // VC6.0
#ifndef _RICHEDIT_VER
#define _RICHEDIT_VER				0x0200
#endif
#endif

#if (_MSC_VER <= 1310) // VC2003

// DPA_*
typedef int (WINAPI *fnDPA_InsertPtr)(HDPA, int, void *);
extern fnDPA_InsertPtr IUI_DPA_InsertPtr;

typedef void *(WINAPI *fnDPA_DeletePtr)(HDPA, int);
extern fnDPA_DeletePtr IUI_DPA_DeletePtr;

typedef void *(WINAPI *fnDPA_GetPtr)(HDPA, int);
extern fnDPA_GetPtr IUI_DPA_GetPtr;

typedef BOOL (WINAPI *fnDPA_SetPtr)(HDPA, int, void *);
extern fnDPA_SetPtr IUI_DPA_SetPtr;

typedef void (WINAPI *fnDPA_EnumCallback)(HDPA, PFNDPAENUMCALLBACK, void *);
extern fnDPA_EnumCallback IUI_DPA_EnumCallback;

typedef BOOL (WINAPI *fnDPA_DeleteAllPtrs)(HDPA);
extern fnDPA_DeleteAllPtrs IUI_DPA_DeleteAllPtrs;

typedef void (WINAPI *fnDPA_DestroyCallback)(HDPA, PFNDPAENUMCALLBACK, void *);
extern fnDPA_DestroyCallback IUI_DPA_DestroyCallback;

typedef BOOL (WINAPI *fnDPA_Destroy)(HDPA);
extern fnDPA_Destroy IUI_DPA_Destroy;

typedef BOOL (WINAPI *fnDPA_Sort)(HDPA, PFNDPACOMPARE, LPARAM);
extern fnDPA_Sort IUI_DPA_Sort;

typedef HDPA(WINAPI *fnDPA_Create)(int);
extern fnDPA_Create IUI_DPA_Create;

typedef HDPA(WINAPI *fnDPA_CreateEx)(int, HANDLE);
extern fnDPA_CreateEx IUI_DPA_CreateEx;

typedef BOOL (WINAPI *fnDPA_Grow)(HDPA, int);
extern fnDPA_Grow IUI_DPA_Grow;

typedef HDPA(WINAPI *fnDPA_Clone)(const HDPA, HDPA);
extern fnDPA_Clone IUI_DPA_Clone;

typedef int (WINAPI *fnDPA_GetPtrIndex)(HDPA, const void *);
extern fnDPA_GetPtrIndex IUI_DPA_GetPtrIndex;

// DSA_*
typedef BOOL (WINAPI *fnDSA_Destroy)(HDSA);
extern fnDSA_Destroy IUI_DSA_Destroy;

typedef void *(WINAPI *fnDSA_GetItemPtr)(HDSA, int);
extern fnDSA_GetItemPtr IUI_DSA_GetItemPtr;

typedef int (WINAPI *fnDSA_InsertItem)(HDSA, int, void *);
extern fnDSA_InsertItem IUI_DSA_InsertItem;

typedef HDSA(WINAPI *fnDSA_Create)(int, int);
extern fnDSA_Create IUI_DSA_Create;

typedef BOOL (WINAPI *fnDSA_GetItem)(HDSA, int, void *);
extern fnDSA_GetItem IUI_DSA_GetItem;

typedef BOOL (WINAPI *fnDSA_SetItem)(HDSA, int, void *);
extern fnDSA_SetItem IUI_DSA_SetItem;

typedef BOOL (WINAPI *fnDSA_DeleteItem)(HDSA, int);
extern fnDSA_DeleteItem IUI_DSA_DeleteItem;

typedef void (WINAPI *fnDSA_DestroyCallback)(HDSA, PFNDSAENUMCALLBACK, void *);
extern fnDSA_DestroyCallback IUI_DSA_DestroyCallback;

typedef void (* fnReleaseActCtx)(HANDLE);
extern fnReleaseActCtx IUI_ReleaseActCtx;


#define DPA_GetPtrCount(hdpa)       (*(int *)(hdpa))
#define DPA_SetPtrCount(hdpa, cItems) (*(int *)(hdpa) = (cItems))
#define DPA_FastDeleteLastPtr(hdpa) (--*(int *)(hdpa))
#define DPA_GetPtrPtr(hdpa)         (*((void * **)((BYTE *)(hdpa) + sizeof(void *))))
#define DPA_FastGetPtr(hdpa, i)     (DPA_GetPtrPtr(hdpa)[i])
#define DPA_AppendPtr(hdpa, pitem)  IUI_DPA_InsertPtr(hdpa, DA_LAST, pitem)

#define DSA_GetItemCount(hdsa)      (*(int *)(hdsa))
#define DSA_AppendItem(hdsa, pitem) IUI_DSA_InsertItem(hdsa, DA_LAST, pitem)

#else // (_MSC_VER <= 1310) // VC2003

#define IUI_DPA_InsertPtr DPA_InsertPtr
#define IUI_DPA_DeletePtr DPA_DeletePtr
#define IUI_DPA_Destroy DPA_Destroy
#define IUI_DPA_GetPtr DPA_GetPtr
#define IUI_DPA_SetPtr DPA_SetPtr
#define IUI_DPA_EnumCallback DPA_EnumCallback
#define IUI_DPA_DeleteAllPtrs DPA_DeleteAllPtrs
#define IUI_DPA_DestroyCallback DPA_DestroyCallback
#define IUI_DPA_Sort DPA_Sort
#define IUI_DPA_Create DPA_Create
#define IUI_DPA_CreateEx DPA_CreateEx
#define IUI_DPA_Grow DPA_Grow
#define IUI_DPA_Clone DPA_Clone
#define IUI_DPA_GetPtrIndex DPA_GetPtrIndex

#define IUI_DSA_Destroy DSA_Destroy
#define IUI_DSA_GetItemPtr DSA_GetItemPtr
#define IUI_DSA_InsertItem DSA_InsertItem
#define IUI_DSA_Create DSA_Create
#define IUI_DSA_GetItem DSA_GetItem
#define IUI_DSA_SetItem DSA_SetItem
#define IUI_DSA_DeleteItem DSA_DeleteItem
#define IUI_DSA_DestroyCallback DSA_DestroyCallback

#define IUI_ReleaseActCtx ReleaseActCtx

#endif // (_MSC_VER <= 1310) // VC2003

namespace LibUIDK
{
	typedef Bitmap *HGPBITMAP;
	typedef class CIUIImage *HIUIIMAGE;

	class CControlBase;
	class CUIWnd;

	// CWLWnd class name
#define IUIWL_Button		_T("IUIWL_Button")

	//////////////////////////////////////////////////////////////////////////
	// Message Define:
	// LibUIDK use custom message ID from WM_APPLICATION to WM_LIBUIDK
#define WM_APPLICATION	WM_USER+100
#define WM_BUTTON		WM_USER+200
#define WM_STATIC		WM_USER+300
#define WM_EDIT			WM_USER+400
#define WM_COMBOBOX		WM_USER+400
#define WM_SLIDER		WM_USER+500
#define WM_SPINBUTTON	WM_USER+600
#define WM_PROGRESS		WM_USER+700
#define WM_HOTKEYCTRL	WM_USER+800
#define WM_IPADDRESS	WM_USER+900
#define WM_LISTCTRL		WM_USER+1000
#define WM_TREECTRL		WM_USER+1100
#define WM_SCROLLBAR	WM_USER+1200
#define WM_THUMBNAIL	WM_USER+1300
#define WM_TREELIST		WM_USER+1400
#define WM_RICHEDIT		WM_USER+1500
#define WM_HTMLCTRL		WM_USER+1600
#define WM_DATETIMECTRL	WM_USER+1700
#define WM_WAVECTRL		WM_USER+1800
#define WM_RECTCTRL		WM_USER+1900
#define WM_CLOCK		WM_USER+2000
#define WM_HEADER		WM_USER+2100
#define WM_MENU			WM_USER+2200
#define WM_MENUBAR		WM_USER+2300
#define WM_SPLITTER		WM_USER+2400
#define WM_PANEL		WM_USER+2500
#define WM_PICTURE		WM_USER+2600
#define WM_DRAGDROP		WM_USER+3000
#define WM_TASKWNDMGR	WM_USER+3100
#define WM_LIBUIDK		WM_USER+4000
#define WM_WINDOWLESS	WM_USER+5000
#define WM_CUSTOM		WM_USER+7000

#define WM_CHANGESKIN					WM_APPLICATION+3
#define WM_GETBKIMGRESIZEPOINT			WM_APPLICATION+6	// WPARAM is the handle of the control; LPARAM is a pointer of POINT struct.

#define WM_GETREPEATPIXEL				WM_APPLICATION+7	// WPARAM is the handle of the control; LPARAM is a pointer of POINT struct.
#define WM_NEEDRESIZE                 WM_APPLICATION+10
#define WM_ISNEEDRESIZE               WM_APPLICATION+11
#define WM_ISCHILDMOVE                WM_APPLICATION+12 // WPARAM: not used. LPARAM: handle of the child
#define WM_PARENTEVENT                WM_APPLICATION+13	// Deprecated, since not include wParam of mouse message, use PreTranslateMessage instead of.
#define WM_GETDOCVIEWCLIENTRECT       WM_APPLICATION+14 // Get the MDI or SDI document/view client rect, LPARAM is a pointer to RECT struct.
#define WM_ISUIWND                    WM_APPLICATION+15	// Is the window CUIWnd or its derive class.
#define WM_ISIUIFRAMEWND              WM_APPLICATION+16	// Is the window CIUIFrameWnd or its derive class.
#define WM_UPDATELAYEREDUIWND         WM_APPLICATION+17	// Call CUIWnd's UpdateLayeredWindow.
#define WM_ISUPDATELAYEREDWINDOW      WM_APPLICATION+18	// If the CUIWnd has called UpdateLayeredWindow.
#define WM_SHOWUIWINDOW               WM_APPLICATION+19	// Send to CUIWnd's parent that the CUIWnd show or hide. WPARAM: Specifies whether a window is being shown; LPARAM: CUIWnd *.
#define WM_GETMENUBAR                 WM_APPLICATION+20	// WPARAM, not use; LPARAM: CMenuBar **.
#define WM_GETCONTROLTYPE             WM_APPLICATION+21	// WPARAM, not use; LPARAM: CONTROL_TYPE *.
#define WM_SETWLITEMTEXT				WM_APPLICATION+22
#define WM_IUIINITIALUPDATE				WM_APPLICATION+23	// same WM_INITIALUPDATE
#define WM_SETBKIMGTRANSPARENT			WM_APPLICATION+24
#define WM_SETFGIMGTRANSPARENT			WM_APPLICATION+25
#define WM_SETBKIMAGEN					WM_APPLICATION+26	// wParam: HBITMAP, lParam: bRedraw
#define WM_SETBKIMAGEH					WM_APPLICATION+27
#define WM_SETBKIMAGES					WM_APPLICATION+28
#define WM_SETBKIMAGED					WM_APPLICATION+29
#define WM_SETBKIMAGENO					WM_APPLICATION+30
#define WM_SETBKIMAGECN					WM_APPLICATION+31
#define WM_SETBKIMAGECH					WM_APPLICATION+32
#define WM_SETBKIMAGECS					WM_APPLICATION+33
#define WM_SETBKIMAGECD					WM_APPLICATION+34
#define WM_ENDSTORYBOARD				WM_APPLICATION+35
#define WM_BEFORESTORYFRAME				WM_APPLICATION+36
#define WM_GETCHILDMSGHANDLER			WM_APPLICATION+37	// Prepare the child CUIWnd handler. WPARAM: child ID; LPARAM: CHILDMSGHANDLER *
#define WM_RELEASECHILDMSGHANDLER		WM_APPLICATION+38	// Release the child CUIWnd handler. WPARAM: child ID; LPARAM: CHILDMSGHANDLER *
#define WM_BIND_DIALOG_END				WM_APPLICATION+39	// The dialog that binding by button called EndDialog and return. WPARAM: CUIWnd *(the dialog pointer); LPARAM: the return value of DoModal.

	// Draw
#define WM_DRAWITEMEX					WM_APPLICATION+40	// The parameter is same to WM_DRAWITEM
#define WM_DRAWITEMBK					WM_APPLICATION+41	// The parameter is same to WM_DRAWITEM
#define WM_DRAWCONTROL					WM_APPLICATION+42	// Draw the control send by CUIWnd with layered style and use UpdateLayeredWindow to draw the UI.
#define WM_GETBACKGROUND				WM_APPLICATION+43	// Return HBITMAP. LPARAM: BOOL *: Compatible bitmap or not.
#define WM_DRAWPARENTPART				WM_APPLICATION+44	// WPARAM: not use, LPARAM: CDC *
#define WM_SET_DRAW						WM_APPLICATION+45	// WPARAM: Specifies the draw state. If this parameter is TRUE, the content can be drawn after a change. If this parameter is FALSE, the content cannot be drawn after a change
#define WM_GET_DRAW						WM_APPLICATION+46	// return the draw state
#define WM_TRANSPARENTDRAW				WM_APPLICATION+47	// Control send this message to it's parent to let parent to draw the control after the control draw parent part.
#define WM_ISNEEDUPDATA					WM_APPLICATION+48
#define WM_SETNEEDUPDATE				WM_APPLICATION+49
#define WM_ISREDRAW						WM_APPLICATION+50
#define WM_DRAWCHILDTEXT				WM_APPLICATION+51
#define WM_BEFOREWLDRAW					WM_APPLICATION+52
#define WM_AFTERWLDRAW					WM_APPLICATION+53

	//
#define WLNM_CREATE						WM_APPLICATION+70	// Notify windowless's parent, the windowless child be created.
#define WLNM_NOTCREATEASCHILD			WM_APPLICATION+71
	//
#define WM_OPEN_DOCUMENT				WM_APPLICATION+90	// WPARAM: CWinApp *; LPARAM: document file path.
#define WM_UPDATEVIEW					WM_APPLICATION+91	// WPARAM: CWnd *pSender; LPARAM: this:CIUIDocument2 *.

#define WM_ADD_BIND_UIWND				WM_APPLICATION+92	// Internal use
#define WM_DELETE_BIND_UIWND			WM_APPLICATION+93

#define WM_GET_INTERNAL_VAR				WM_APPLICATION+94	// For internal use.

#define WM_ISSHADOWCREATED				WM_APPLICATION+95
#define WM_ISNOCODEUIMODE				WM_APPLICATION+96


	//
#define EM_UPDATEBKBRUSH				WM_EDIT+1			// WPARAM: not use, must be 0; LPARAM: BOOL: TRUE->redraw the edit control.

	// CImageOleCtrl ole send this message to RichEdit that added a gif ole to richedit
#define EM_ADD_GIF_OLE					WM_RICHEDIT+1		// WPARAM: not use, must be 0; LPARAM: CImageOleCtrl *.
#define EM_GET_GIF_OLE_LIST				WM_RICHEDIT+2

	// Menu bar
#define WM_MENUMOUSEMOVE              WM_MENU+1
#define WM_ISMENUBAR                  WM_MENU+2			// Is the window menu bar.

#define WS_MENUWND_DESTROY            WM_MENU+50
#define WS_MENUWND_HIDING             WM_MENU+61
#define WS_MENUWND_HIDDEN             WM_MENU+62

	// WPARAM: nID; LPARAM: CTaskParam *
	;
#define WM_TASKWNDSWITCHING           WM_TASKWNDMGR+1

	// WPARAM: nID; LPARAM: CTaskParam *
	;
#define WM_TASKWNDSWITCHED            WM_TASKWNDMGR+2

	// WPARAM: nID; LPARAM: CTaskParam *. Send after the new task window be created, and before it visible. is be send before WM_TASKWNDSWITCHED.
	;
#define WM_TASKWNDCREATE				WM_TASKWNDMGR+3

	// WPARAM: nID of CTaskWndMgr object; LPARAM: CTaskWndInfo *. Send before the task window call DestroyWidnow.
	;
#define WM_TASKWNDDESTROYING			WM_TASKWNDMGR+4

	// WPARAM: nID of CTaskWndMgr object; LPARAM: CTaskWndInfo *. Send after the task window call DestroyWidnow.
	;
#define WM_TASKWNDDESTROYED				WM_TASKWNDMGR+5

	// Owner draw control types
#define ODT_TREELIST					10
#define ODT_THUMBNAIL					11
#define ODT_SCROLLBAREX					12
#define ODT_IMQIPAO						13
#define ODT_WLWND						1000

	// Splitter Bar
#define WM_DRAGGING_SPLITTERBAR			WM_APPLICATION+100	// WPARAM: not use, must be 0;	LPARAM: SplitterStruct *.
#define WM_ENDDRAG_SPLITTERBAR			WM_APPLICATION+101	// WPARAM: not use, must be 0;	LPARAM: SplitterStruct *.
#define WM_ENDTRACK_SPLITTER			WM_APPLICATION+102	// WPARAM: HWND of splitter, LPARAM: not use.

	// Windowless message
#define WM_WINDOWLESSFIRST				WM_WINDOWLESS + 0

#define WM_WLCREATE                       WM_WINDOWLESS + 0
#define WM_WLDESTROY                      WM_WINDOWLESS + 1
#define WM_WLMOUSEFIRST                   WM_WINDOWLESS + 10
#define WM_WLMOUSEMOVE                    WM_WLMOUSEFIRST + 0
#define WM_WLLBUTTONDOWN                  WM_WLMOUSEFIRST + 1		// wParam: windowless id; lParam: WLMOUSE *
#define WM_WLLBUTTONUP                    WM_WLMOUSEFIRST + 2
#define WM_WLLBUTTONDBLCLK                WM_WLMOUSEFIRST + 3		// wParam: windowless id; lParam: WLMOUSE *
#define WM_WLRBUTTONDOWN                  WM_WLMOUSEFIRST + 4
#define WM_WLRBUTTONUP                    WM_WLMOUSEFIRST + 5
#define WM_WLRBUTTONDBLCLK                WM_WLMOUSEFIRST + 6
#define WM_WLMBUTTONDOWN                  WM_WLMOUSEFIRST + 7
#define WM_WLMBUTTONUP                    WM_WLMOUSEFIRST + 8
#define WM_WLMBUTTONDBLCLK                WM_WLMOUSEFIRST + 9
#define WM_WLMOUSEWHEEL                   WM_WLMOUSEFIRST + 10
#define WM_WLXBUTTONDOWN                  WM_WLMOUSEFIRST + 11
#define WM_WLXBUTTONUP                    WM_WLMOUSEFIRST + 12
#define WM_WLXBUTTONDBLCLK                WM_WLMOUSEFIRST + 13
#define WM_WLMOUSEHWHEEL                  WM_WLMOUSEFIRST + 14
#define WM_WLMOUSELAST                    WM_WLMOUSEFIRST + 14 // == WM_WINDOWLESS + 24

#define WM_WL_COMMAND					WM_WINDOWLESS + 30
#define WM_WL_GETWINDOWRGNREF			WM_WINDOWLESS + 31

#define WM_WL_CALLONDRAW				WM_WINDOWLESS + 32			// LPARAM: TRUE, call it's self OnDraw
#define WM_WL_GETLBTNDOWNHITTESTRESULT	WM_WINDOWLESS + 33

	// Send to windowless itself and parent
#define WM_WL_ENTERSIZEMOVE				WM_WINDOWLESS + 34
#define WM_WL_EXITSIZEMOVE				WM_WINDOWLESS + 35

#define WM_WL_SHOWWINDOW				WM_WINDOWLESS + 36			// See WM_SHOWWINDOW
#define WM_WL_MOVEWINDOW				WM_WINDOWLESS + 37

#define WM_WINDOWLESSLAST				WM_WINDOWLESS + 1000

#define TVM_SET_TOTAL_SHOW_HEIGHT		WM_TREECTRL + 1
#define TVM_GET_TOTAL_SHOW_HEIGHT		WM_TREECTRL + 2
#define TVM_SET_TOP_ITEM				WM_TREECTRL + 3
#define TVM_GET_TOP_ITEM				WM_TREECTRL + 4
#define TVM_SET_TOP_ITEM_YOFFSET		WM_TREECTRL + 5
#define TVM_GET_TOP_ITEM_YOFFSET		WM_TREECTRL + 6
#define TVM_SET_TOP_ITEM_VSCROLL_POS	WM_TREECTRL + 7
#define TVM_GET_TOP_ITEM_VSCROLL_POS	WM_TREECTRL + 8
#define TVM_GET_NORMAL_IMAGE_SIZE		WM_TREECTRL + 9
#define TVM_GET_STATE_IMAGE_SIZE		WM_TREECTRL + 10
#define TVM_SET_CXMAX					WM_TREECTRL + 11
#define TVM_GET_CXMAX					WM_TREECTRL + 12
#define TVM_SET_CAPTURE_POINT			WM_TREECTRL + 13
#define TVM_GET_CAPTURE_POINT			WM_TREECTRL + 14
#define TVM_GET_MEMBER					WM_TREECTRL + 15
#define TVM_SET_TOPITEM_YSCROLLPOS		WM_TREECTRL + 16
#define TVM_GET_TOPITEM_YSCROLLPOS		WM_TREECTRL + 17

	//////////////////////////////////////////////////////////////////////////
	// The error code of return value by CUIWnd::GetLastCreateError()
#define E_LOADBMPB                    1		// failed to load the base image
#define E_LOADBMPN                    2		// failed to load the background image of normal status(end by _N.bmp)
#define E_LOADBMPH                    3		// failed to load the background image of highlight status(end by _H.bmp)
#define E_LOADBMPD                    4		// failed to load the background image of disable status(end by _D.bmp)
#define E_LOADBMPS                    5		// failed to load the background image of selected status(end by _S.bmp)
#define E_LOADBMPC                    6		// failed to load the background image of checked status(end by _CX.bmp)
#define E_ATTACHBMPB                  50	// failed to attach the HBITMAP of base image to CBitmap object
#define E_ATTACHBMPN                  51	// failed to attach the HBITMAP of normal image to CBitmap object
#define E_ATTACHBMPH                  52	// failed to attach the HBITMAP of highlight image to CBitmap object
#define E_ATTACHBMPD                  53	// failed to attach the HBITMAP of disable image to CBitmap object
#define E_ATTACHBMPS                  54	// failed to attach the HBITMAP of selected image to CBitmap object
#define E_ATTACHBMPC                  55	// failed to attach the HBITMAP of checked image to CBitmap object
#define E_ADJUSTCHILD                 56	// failed to adjust the child controls while change the skin
#define E_CREATECHILD                 110	// failed to create the child window

	//////////////////////////////////////////////////////////////////////////
	// The error code of return value while load skin
#define E_SETUIPATH						200   // failed to set .ui or .xui path, it cause by the ui path isn't exists.
#define E_INVALIDLICENSE				201   // invalid license
#define E_NOTUNICODE					202	// the .ui file not support unicode when you define _UNICODE or UNICODE micro.
#define E_OLDERVERSION					203	// the version of .ui or .xui file older than LibUIDK.
#define E_NEWERVERSION					204	// the version of .ui or .xui file newer than LibUIDK.
#define E_CHANGELIST_NOTMATCH			205	// the change list in *.xui file not match the LibUIDK library.
#define E_OLEUNINIT						210	// Not call ::OleInitialize
#define E_NOROOTELEMENT					211	// Failed to load the root element of *.xui.
#define E_CANCELTRANSFERTONEWVERSION	212	// Cancel to transfer to new version of *.xui.
#define E_NOWINDOWSNODE					213	// Not found the 'Windows' node.
#define E_NORESOURCENODE				214	// Not found the 'Resource' node.
#define E_NOXUIRC						215
#define E_LOADXML						216	// Failed to load xml document.

	//////////////////////////////////////////////////////////////////////////
	// Others error code
#define E_PARA1_NULL					-1
#define E_PARA2_NULL					-2
#define E_PARA3_NULL					-3
#define E_PARA4_NULL					-4
#define E_PARA5_NULL					-5
#define E_PARA6_NULL					-6
#define E_PARA7_NULL					-7
#define E_PARA8_NULL					-8
#define E_PARA9_NULL					-9
#define E_PARA10_NULL					-10
#define E_PARA_NULL						-20
#define E_PARA_INVALID					-21
#define E_ELEMENT_ALREDY_EXIST			-22


#define DROPM_DRAGENTER				WM_DRAGDROP+1
#define DROPM_DRAGOVER				WM_DRAGDROP+2
#define DROPM_DROP					WM_DRAGDROP+3
#define DROPM_DROPEX				WM_DRAGDROP+4
#define DROPM_DRAGLEAVE				WM_DRAGDROP+5
#define DROPM_DRAGSCROLL            WM_DRAGDROP+6

#define NM_WLCHILD					(NM_FIRST-20)
#define NM_DBLCLKEX                 (NM_FIRST-30)	// Parameter same as CSkinTreeCtrl::NM_DBLCLK
#define NM_EXCLUDE_CLIP_RECT		(NM_FIRST-31)	// Send to control's parent to call a exclude clip rect
#define NM_DROP						(NM_FIRST-32)	// send drop notify to control's parent
#define BNN_DRAWTEXT				(NM_FIRST-40)
#define TVN_HIGHLIGHTCHANGING		(NM_FIRST-50)	// Parameter same as CSkinTreeCtrl::TVN_SELCHANGING
#define TVN_HIGHLIGHTCHANGED		(NM_FIRST-51)	// Parameter same as CSkinTreeCtrl::TVN_SELCHANGED
#define TVN_INSTANTIATE_ITEM_TEMPLATE (NM_FIRST-56)	// Instantiate tree item template. LPARAM: NMTREEVIEW *
#define LVN_INSTANTIATE_ITEM_TEMPLATE (NM_FIRST-57)	// Instantiate list view item template. LPARAM: NMLISTVIEW *
#define WVN_SCROLLED				(NM_FIRST-60)
#define WVN_DRAWINFOAREA			(NM_FIRST-61)
#define WLN_MOVE					(NM_FIRST-70)	// Position of CWLWnd changed
#define WLN_SIZE					(NM_FIRST-71)	// Size of CWLWnd changed
#define WLN_MOVING					(NM_FIRST-72)	// Position of CWLWnd is moving
#define WLN_SIZING					(NM_FIRST-73)	// Size of CWLWnd is sizing
#define WLN_ENTERSIZEMOVE			(NM_FIRST-74)	// Enters the moving or sizing, See also: WM_ENTERSIZEMOVE
#define WLN_EXITSIZEMOVE			(NM_FIRST-75)	// See also: WM_EXITSIZEMOVE
#define WLN_MOUSE					(NM_FIRST-76)	// Send mouse notify to parent.
#define LVN_ANIMATION_END			(NM_FIRST-80)	// The animation of show list item of CSkinListCtrl finished.
#define LVN_SUBITEMCLICK			(NM_FIRST-81)	// Clicked a sub item.
#define LVN_WLBUTTON_CLICK			(NM_FIRST-82)	// Clicked a child CWLButton
#define PVN_ANIMATION_END			(NM_FIRST-100)	// The animation of stretch image of CPictureCtrl finished.
#define SLDN_ANIMATION_END			(NM_FIRST-110)	// The animation of thumb of CWLSliderCtrl finished.
#define SLDN_POS_CHANGED			(NM_FIRST-111)	// Before end SetPos.
#define TNVN_ITEMCLICK				(NM_FIRST-120)	// Thumbnail view item clicked.
#define NWUIN_DEBGINDRAG			(NM_FIRST-130)	// CNewWindowsUIPanel item begin drag.
#define EN_ADDED_CUSTOM_OLE			(NM_FIRST-150)	// IM RichEdit need insert a custom OLE.
#define EN_WLMOUSE					(NM_FIRST-151)	// Send to RichEdit parent that its OLE child trigger a mouse message


#if(_MSC_VER <= 1400) // VC2005

#define LVN_INCREMENTALSEARCHA   (LVN_FIRST-62)
#define LVN_INCREMENTALSEARCHW   (LVN_FIRST-63)

#ifdef UNICODE
#define LVN_INCREMENTALSEARCH    LVN_INCREMENTALSEARCHW
#else
#define LVN_INCREMENTALSEARCH    LVN_INCREMENTALSEARCHA
#endif

#define LVFI_PARAM              0x0001
#define LVFI_STRING             0x0002
#define LVFI_SUBSTRING          0x0004  // Same as LVFI_PARTIAL
#define LVFI_PARTIAL            0x0008
#define LVFI_WRAP               0x0020
#define LVFI_NEARESTXY          0x0040

#define DPA_GetPtrCount(hdpa)       (*(int *)(hdpa))
#define DPA_SetPtrCount(hdpa, cItems) (*(int *)(hdpa) = (cItems))
#define DPA_FastDeleteLastPtr(hdpa) (--*(int *)(hdpa))
#define DPA_GetPtrPtr(hdpa)         (*((void * **)((BYTE *)(hdpa) + sizeof(void *))))
#define DPA_FastGetPtr(hdpa, i)     (DPA_GetPtrPtr(hdpa)[i])
#define DSA_GetItemCount(hdsa)      (*(int *)(hdsa))
#endif // VC2005

	//////////////////////////////////////////////////////////////////////////

	class CWLWnd;


	//
	// type of controls
	//

	enum CONTROL_TYPE
	{
		CT_ARROW,
		CT_PUSHBUTTON,
		CT_CHECK,
		CT_RADIO,
		CT_COMBOBOX,
		CT_STATIC,
		CT_EDIT,
		CT_SLIDER,
		CT_PROGRESS,
		CT_SPIN,
		CT_HOTKEY,
		CT_IPADDRESS,
		CT_SLIDEREX,
		CT_LISTCTRL,
		CT_TREECTRL,
		CT_SCROLLBAR,
		CT_SCROLLBAREX,
		CT_TREELIST,
		CT_THUMBNAIL,
		CT_HTMLCTRL,
		CT_DATETIMECTRL,
		CT_RICHEDIT,
		CT_RICHEDIT_IM,
		CT_WAVECTRL,
		CT_PICTURE,
		CT_MENU,
		CT_MENUBAR,
		CT_HEADERCTRL,
		CT_HORSCROLLBAR,
		CT_VERSCROLLBAR,
		CT_SPLITTER,
		// Windowless control
		CT_WL_FIRST = CT_SPLITTER,
		CT_WL_RECTCTRL,
		CT_WL_SPLITTER,
		CT_WL_LINE,
		CT_WL_TEXT,
		CT_WL_BUTTON,
		CT_WL_CHECK,
		CT_WL_RADIO,
		CT_WL_SLIDER,
		CT_WL_PICTURE,
		CT_WL_RICHEDIT,
		CT_WL_RICHEDIT_IM,

		// CTaskWndMgr
		CT_TASK_WND_MGR,
		// Panel, also is windowless control.
		CT_PNL_DOCK,
		CT_UNIFORM_GRID,
		CT_PNL_STACK,
		CT_WL_LAST = CT_PNL_STACK,

		CT_UIWND,

		// Resource
		CT_RESOURCE_DEFAULT_FONT,
		CT_RESOURCE_GENERAL_FONT,
		CT_RESOURCE_CONDITION_FONT,

		CT_RESOURCE_DEFAULT_COLOR,
		CT_RESOURCE_COLOR,

		// Style
		CT_STYLE_PUSHBUTTON,
		CT_STYLE_CHECK,
		CT_STYLE_RADIO,
		CT_STYLE_COMBOBOX,
		CT_STYLE_STATIC,
		CT_STYLE_EDIT,
		CT_STYLE_SLIDER,
		CT_STYLE_PROGRESS,
		CT_STYLE_SPIN,
		CT_STYLE_HOTKEY,
		CT_STYLE_IPADDRESS,
		CT_STYLE_SLIDEREX,
		CT_STYLE_LISTCTRL,
		CT_STYLE_TREECTRL,
		CT_STYLE_TREELIST,
		CT_STYLE_RICHEDIT,
		CT_STYLE_IMRICHEDIT,
		CT_STYLE_HTMLCTRL,
		CT_STYLE_DATETIMECTRL,
		CT_STYLE_WAVECTRL,
		CT_STYLE_MENU,
		CT_STYLE_MENUBAR,
		CT_STYLE_HEADERCTRL,
		CT_STYLE_HORSCROLLBAR,
		CT_STYLE_VERSCROLLBAR,
		CT_STYLE_SPLITTER,
		CT_STYLE_PANEL_STACK,
		CT_STYLE_WL_LINE,
		CT_STYLE_WL_PICTURE,
		CT_LAST
	};


	// GetProp
#define GETPROP_OLDWNDPROC					_T("IUI_OLD_WNDPROC")
#define GETPROP_PANEL						_T("IUI_HWND_PANEL")
#define GETPROP_SPLITTER_POINTER			_T("IUI_SPLITTER_POINTER")
#define GETPROP_UIWND_STRINGID				_T("IUI_UIWND_STRINGID")
#define GETPROP_MENUBAR_BUTTON_INDEX		_T("IUI_MENUBAR_BUTTON_INDEX")
#define GETPROP_TASKWNDMGR_TIMERSTRUCT		_T("IUI_TASKWNDMGR_TIMER_STRUCT")
#define GETPROP_WLMGR						_T("IUI_WLMGR")


	//////////////////////////////////////////////////////////////////////////

#define COMBINEIMAGESIZE2					2
#define COMBINEIMAGESIZE4					4
#define COMBINEIMAGESIZE8					8

#define TIMER_ID_TASKWNDMGR					2000

	struct NMWLCHILD
	{
		NMHDR hdr;
		UINT uMsg;
		WPARAM wParam;
		LPARAM lParam;
	};

	//////////////////////////////////////////////////////////////////////////
	// the LPARAM parameter of NM_DROP notify
	struct NMDROP
	{
		NMHDR hdr;
		UINT message;
		LPARAM lParam;
	};

	//////////////////////////////////////////////////////////////////////////
	// the LPARAM parameter of BNN_DRAWTEXT notify
	struct NMBUTTON
	{
		NMHDR hdr;
		HDC hDC;
		RECT rc;
		UINT uFormat;
	};

	//////////////////////////////////////////////////////////////////////////
	// the LPARAM parameter of WVN_DRAWINFOAREA notify
	struct NMWAVECTRL
	{
		NMHDR hdr;
		HDC hDC;
		RECT rc;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// the LPARAM parameter of SLDN_POS_CHANGED notify
	struct NMSLIDERCTRL
	{
		NMHDR hdr;
		int nOldPos;
		int nNewPos;
	};

	//////////////////////////////////////////////////////////////////////////
	// the LPARAM parameter of WLN_MOVING
	struct NMRECT
	{
		NMHDR hdr;
		RECT rect;
		LPARAM lParam;
	};

	//////////////////////////////////////////////////////////////////////////
	// the LPARAM parameter of EN_ADD_CUSTOM_OLE
	struct NMIMRE
	{
		NMHDR hdr;
		WPARAM wParam;
		LPARAM lParam;
	};

	//////////////////////////////////////////////////////////////////////////
	// the LPARAM parameter of EN_WLMOUSE
	struct NMREWLMOUSE
	{
		NMHDR hdr;
		UINT uWLMsg;
		UINT uSenderID; // The id of OLE object.
		struct WLMOUSE *pWLMouse;
	};

	//////////////////////////////////////////////////////////////////////////
	//  Eudora is a pile of crap.
	//
	//  When they get a NM_DBLCLK notification from a treeview, they say,
	//  "Oh, I know that treeview allocates its NMHDR from the stack, and
	//  there's this local variable on Treeview's stack I'm really interested
	//  in, so I'm going to hard-code an offset from the pnmhdr and read the
	//  DWORD at that location so I can get at the local variable.  I will then
	//  stop working if this value is zero."
	//
	//  The conversion to UNICODE changed our stack layout enough that they
	//  end up always getting zero -- it's the NULL parameter which is the
	//  final argument to CCSendNotify.  Since all this stack layout stuff is
	//  sensitive to how the compiler's optimizer feels today, we create a
	//  special notify structure Just For Eudora which mimics the stack layout
	//  they expected to see in Win95.
	//
	typedef struct NMEUDORA
	{
		NMHDR   nmhdr;
		BYTE    Padding[48];
		DWORD   MustBeNonzero;      // Eudora fails to install if this is zero
	} NMEUDORA;

	//////////////////////////////////////////////////////////////////////////
	// character index

	struct CHARINDEX
	{
		TCHAR ch;		// character
		int nXOffset;	// x offset in image list
		int nWidth;		// character image width
	};

	//////////////////////////////////////////////////////////////////////////

	// text horizontal align mode
	enum TEXT_ALIGN_HOR
	{
		TAH_UNKNOWN = -1,
		TAH_DEFAULT = 0,
		TAH_LEFT = 1,
		TAH_RIGHT = 2,
		TAH_CENTER = 3,
	};

	// text vertical align mode
	enum TEXT_ALIGN_VER
	{
		TAV_UNKNOWN = -1,
		TAV_DEFAULT = 0,
		TAV_TOP = 1,
		TAV_BOTTOM = 2,
		TAV_CENTER = 3,
	};

	// foreground image horizontal align mode
	enum FOREGROUND_ALIGN_HOR
	{
		FAH_UNKNOWN = -1,
		FAH_CENTER,
		FAH_LEFT,
		FAH_RIGHT,
		FAH_STRETCH
	};

	// foreground image vertical align mode
	enum FOREGROUND_ALIGN_VER
	{
		FAV_UNKNOWN = -1,
		FAV_CENTER,
		FAV_TOP,
		FAV_BOTTOM,
		FAV_STRETCH
	};

	enum IMAGE_RESIZE_MODE
	{
		IRM_UNKNOWN,
		IRM_9GRID,
		IRM_STRETCH,
		IRM_STRETCH_HIGH_QUALITY,
		IRM_TILE,
		IRM_CENTER
	};

	// Control horizontal layout mode
	enum CONTROL_LAYOUT_HOR
	{
		CLH_UNKNOWN = -1,
		CLH_DEFAULT,
		CLH_LEFT,
		CLH_RIGHT,
		CLH_CENTER,
		CLH_FILL,
		CLH_FULL_FILL,
		CLH_FREE,
		CLH_RIGHT_EVEN_ZOOM_OUT,
		CLH_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO,		// Keep the child control's top-left point aspect ratio.
		CLH_KEEP_CHILDREN_ASPECT_RATIO				// Keep the child control's aspect ratio(include position and size).
	};

	// Control vertical layout mode
	enum CONTROL_LAYOUT_VER
	{
		CLV_UNKNOWN = -1,
		CLV_DEFAULT,
		CLV_TOP,
		CLV_BOTTOM,
		CLV_CENTER,
		CLV_FILL,
		CLV_FULL_FILL,
		CLV_FREE,
		CLV_BOTTOM_EVEN_ZOOM_OUT,
		CLV_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO,		// Keep the child control's top-left point aspect ratio.
		CLV_KEEP_CHILDREN_ASPECT_RATIO				// Keep the child control's aspect ratio(include position and size).
	};

	enum BTN_TYPE
	{
		BT_UNKNOWN = -1,
		BT_NORMAL,
		BT_MINIMIZEBOX,
		BT_CLOSEBOX
	};

	enum CHK_TYPE
	{
		CT_UNKNOWN = -1,
		CT_NORMAL,
		CT_MAXIMIZEBOX
	};

	enum WINDOW_TYPE
	{
		WT_UNKNOWN = -1,
		WT_NORMAL,
		WT_MDIFRAME,
		WT_MDICLIENT,
		WT_MDICHILDFRAME,
		WT_MDIVIEW,
		WT_FORMWND
	};

	enum CMB_STYLE
	{
		CS_UNKNOWN = -1,
		CS_SIMPLE = CBS_SIMPLE,
		CS_DROPDOWN = CBS_DROPDOWN,
		CS_DROPDOWNLIST = CBS_DROPDOWNLIST
	};

	enum SPN_ALIGN
	{
		SPNA_UNKNOWN = -1,
		SPNA_UNATTACHED = 0,
		SPNA_LEFT = 1,
		SPNA_RIGHT = 2
	};

	enum SLD_EXT_STYLE
	{
		SLDES_UNKNOWN = -1,
		SLDES_NORMAL = 0,
		SLDES_PROGRESS = 1,
		SLDES_SPLITPROGRESS = 2
	};

	enum LST_STYLE
	{
		LSTS_UNKNOWN = -1,
		LSTS_ICON = 0,
		LSTS_SMALLICON = 1,
		LSTS_LIST = 2,
		LSTS_REPORT = 3,
		LSTS_THUMBNAIL = 4
	};

	enum LST_ALIGN
	{
		LSTA_UNKNOWN = -1,
		LSTA_TOP = 0,
		LSTA_LEFT = 1
	};

	enum LST_SORT
	{
		LSTST_UNKNOWN = -1,
		LSTST_UNSET,
		LSTST_ASCENDING = 1,
		LSTST_DESCENDING = 2
	};

	enum DATETIME_FORMAT
	{
		DTF_UNKNOWN = -1,
		DTF_SHORT_DATE,
		DTF_LONG_DATE,
		DTF_TIME
	};

	enum BACKGROUND_TYPE
	{
		BKT_UNKNOWN = -1,
		BKT_IMAGE,
		BKT_COLOR
	};

	enum PROGRESS_THUMB_MODE
	{
		PTM_UNKNOWN = -1,
		PTM_STRETCH,
		PTM_TILE,
		PTM_CLIP
	};

	enum PROGRESS_THUMB_POSITION
	{
		PTP_UNKNOWN = -1,
		PTP_DEFAULT,		// The thumb area is default
		PTP_SPECIFIED		// The thumb area is specified, not from (0, 0)
	};

	enum ALPHA_FORMAT
	{
		AF_UNKNOWN = -1,
		AF_NONE,
		AF_AC_SRC_ALPHA
	};

	enum GRID_PANEL_UINT
	{
		GPU_UNKNOWN = -1,
		GPU_PIXEL,
		GPU_PERCENT
	};

	enum MENU_BORDER_STYLE
	{
		MBS_UNKNOWN = -1,
		MBS_NORMAL,			// has 3D-like border
		MBS_FLAT,			// has one pix border
		MBS_NONE			// has no border
	};

	enum STACK_PANEL_ORIENTATION
	{
		SPO_UNKNOWN = -1,
		SPO_LEFT,
		SPO_TOP,
		SPO_RIGHT,
		SPO_BOTTOM,
		SPO_CENTER,
		SPO_VCENTER
	};

	enum SPLITTER_ORIENTATION
	{
		SO_UNKNOWN = -1,
		SO_WE,	// West east,   <- | ->, the bar is vertical, can adjust the width of left and right panel.
		SO_NS	// North South. the bar is Horizontal, can adjust the height of up and down panel.
	};

	enum SPLITTER_DRAG_ORIENTATION
	{
		SDO_UNKNOWN = -1,
		SDO_LEFT,
		SDO_UP,
		SDO_RIGHT,
		SDO_DOWN
	};

	enum SPLITTER_PANEL_POS
	{
		SPP_UNKNOWN = -1,
		SPP_LEFT,			// Left panel
		SPP_TOP,			// Top panel
		SPP_RIGHT,			// Right panel
		SPP_BOTTOM			// Bottom panel
	};

	enum GRID_ELEMENT_ALIGN_HOR
	{
		GEAH_UNKNOWN = -1,
		GEAH_LEFT,
		GEAH_CENTER,
		GEAH_RIGHT,
		GEAH_STRETCH
	};

	enum GRID_ELEMENT_ALIGN_VER
	{
		GEAV_UNKNOWN = -1,
		GEAV_TOP,
		GEAV_CENTER,
		GEAV_BOTTOM,
		GEAV_STRETCH
	};

	enum GRID_ELEMENT_TYPE
	{
		GET_UNKNOWN = -1,
		GET_WINDOW,
		GET_PANEL
	};

	enum PANEL_ELEMENT_TYPE
	{
		PET_UNKNOWN = -1,
		PET_WINDOW,
		PET_WINDOWLESS,
		PET_PANEL
	};

	//

	enum RESOURCE_TYPE
	{
		REST_FONT,
		REST_COLOR
	};

	enum STYLE_TYPE
	{
		STYLET_PUSHBUTTON,
		STYLET_CHECK,
		STYLET_RADIO,
		STYLET_COMBOBOX,
		STYLET_STATIC,
		STYLET_EDIT,
		STYLET_SLIDER,
		STYLET_PROGRESS,
		STYLET_SPIN,
		STYLET_HOTKEY,
		STYLET_IPADDRESS,
		STYLET_SLIDEREX,
		STYLET_LISTCTRL,
		STYLET_TREECTRL,
		STYLET_TREELIST,
		STYLET_RICHEDIT,
		STYLET_IMRICHEDIT,
		STYLET_HTMLCTRL,
		STYLET_DATETIMECTRL,
		STYLET_WAVECTRL,
		STYLET_MENU,
		STYLET_MENUBAR,
		STYLET_HEADERCTRL,
		STYLET_HORSCROLLBAR,
		STYLET_VERSCROLLBAR,
		STYLET_SPLITTER,
		STYLET_PANEL_STACK,
		STYLET_WL_LINE,
		STYLET_WL_PICTURE
	};

	enum CONTROL_STATE
	{
		CONTROL_STATE_NORMAL = 0x00000001,
		CONTROL_STATE_HIGHLIGHT = 0x00000002,
		CONTROL_STATE_SELECTED = 0x00000004,
		CONTROL_STATE_DISABLED = 0x00000008,
		CONTROL_STATE_CHECKED_NORMAL = 0x00010000,
		CONTROL_STATE_CHECKED_HIGHLIGHT = 0x00020000,
		CONTROL_STATE_CHECKED_SELECTED = 0x00040000,
		CONTROL_STATE_CHECKED_DISABLED = 0x00080000,
		CONTROL_STATE_UNCHECKED_ND = CONTROL_STATE_NORMAL | CONTROL_STATE_DISABLED,
		CONTROL_STATE_UNCHECKED_ALL = 0x0000000F,
		CONTROL_STATE_CHECKED_ALL = 0x000F0000,
		CONTROL_STATE_ALL = 0x11111111
	};

	enum Direction
	{
		UNKNOWN,
		LEFT,
		TOP,
		RIGHT,
		BOTTOM,
	};

	enum LINE_PEN_STYLE
	{
		LPS_UNKNOWN,
		LPS_SOLID = PS_SOLID,
		LPS_DASH = PS_DASH,
		LPS_DOT = PS_DOT,
		LPS_DASHDOT = PS_DASHDOT,
		LPS_DASHDOTDOT = PS_DASHDOTDOT,
		LPS_NULL = PS_NULL,
		LPS_INSIDEFRAME = PS_INSIDEFRAME,
		LPS_USERSTYLE = PS_USERSTYLE,
		LPS_ALTERNATE = PS_ALTERNATE,
	};

	enum DRAW_PARENT_PART
	{
		DPP_DRAW,
		DPP_NODRAW,
		DPP_AUTO
	};

	enum DOCK_STYLE
	{
		DS_NONE,
		DS_TOP,
		DS_BOTTOM,
		DS_LEFT,
		DS_RIGHT,
		DS_FILL
	};

	//////////////////////////////////////////////////////////////////////////
	//

	enum VARENUMEX
	{
		VT_EX_STRUCT_SIZE = VT_EMPTY + 1,
		VT_EX_CONTROL_ID_MACRO,
		VT_EX_CONTROL_ID,
		VT_EX_CONTROL_TYPE,
		VT_EX_RECT,
		VT_EX_RECT_WH,		// Width and Height mode
		VT_EX_POINT,
		VT_EX_COLOR,
		VT_EX_FONT,
		VT_EX_BITMAPPATH,
		VT_EX_IMAGE_RESIZE_MODE,
		VT_EX_LAYOUT_HOR,
		VT_EX_LAYOUT_VER,
		VT_EX_TEXT_ALIGN_HOR,
		VT_EX_TEXT_ALIGN_VER,
		VT_EX_FOREGROUND_ALIGN_HOR,
		VT_EX_FOREGROUND_ALIGN_VER,
		VT_EX_BUTTON_TYPE,
		VT_EX_CHECKBOX_TYPE,
		VT_EX_WINDOW_TYPE,
		VT_EX_COMBOBOX_STYLE,
		VT_EX_SPN_ALIGN,
		VT_EX_SLD_EXT_STYLE,
		VT_EX_LST_STYLE,
		VT_EX_LST_ALIGN,
		VT_EX_LST_SORT,
		VT_EX_DATETIME_FORMAT,
		VT_EX_BACKGROUND_TYPE,
		VT_EX_PROGRESS_THUMB_MODE,
		VT_EX_PROGRESS_THUMB_POSITION,
		VT_EX_ALPHA_FORMAT,
		VT_EX_MENU_BORDER_STYLE,
		VT_EX_SPLITTER_ORIENTATION,
		VT_EX_STACK_PANEL_ORIENTATION,
		VT_EX_PEN_STYLE,
		// Windows
		VT_EX_WINDOW_ID,
		VT_EX_WINDOW_ID_NOT_INCLUDE_SELF,
		// TaskWndMgr
		VT_EX_TASKMGR_ID,
		// Resource
		VT_EX_RESOURCE_FONT,
		VT_EX_RESOURCE_COLOR,
		// Style
		VT_EX_STYLE_PUSHBUTTON,
		VT_EX_STYLE_CHECK,
		VT_EX_STYLE_RADIO,
		VT_EX_STYLE_COMBOBOX,
		VT_EX_STYLE_STATIC,
		VT_EX_STYLE_EDIT,
		VT_EX_STYLE_SLIDER,
		VT_EX_STYLE_PROGRESS,
		VT_EX_STYLE_SPIN,
		VT_EX_STYLE_HOTKEY,
		VT_EX_STYLE_IPADDRESS,
		VT_EX_STYLE_SLIDEREX,
		VT_EX_STYLE_LISTCTRL,
		VT_EX_STYLE_TREECTRL,
		VT_EX_STYLE_TREELIST,
		VT_EX_STYLE_RICHEDIT,
		VT_EX_STYLE_IMRICHEDIT,
		VT_EX_STYLE_HTMLCTRL,
		VT_EX_STYLE_DATETIMECTRL,
		VT_EX_STYLE_WAVECTRL,
		VT_EX_STYLE_MENU,
		VT_EX_STYLE_MENUBAR,
		VT_EX_STYLE_HEADERCTRL,
		VT_EX_STYLE_HORSCROLLBAR,
		VT_EX_STYLE_VERSCROLLBAR
	};

	class CIUIImage
	{
		enum DIBOrientation
		{
			DIBOR_DEFAULT,
			DIBOR_TOPDOWN,
			DIBOR_BOTTOMUP
		};

	public:
		CIUIImage() throw();
		virtual ~CIUIImage() throw();

		int AddRef();
		int Release();

	public:
		CString GetSafeImageName() const throw();
		HBITMAP GetSafeHBITMAP() const throw();
		operator HBITMAP() const throw();

		int Load(LPCTSTR pszFileName) throw();
		int Load(__inout IStream *pStream) throw();
		void LoadFromResource(HINSTANCE hInstance, LPCTSTR pszResourceName) throw();
		void LoadFromResource(HINSTANCE hInstance, UINT nIDResource) throw();
		int LoadFromResource(LPCTSTR lpszID, LPCTSTR lpszResType);
		int AutoLoad(LPCTSTR lpszImageName) throw();	// Auto load image from file or rc by CUIMgr::m_bLoadFromFile.
		int SafeLoadSavedImage(); // Load the image saved in m_strImageName. the ref count add 1.

		int Create(int nWidth, int nHeight, int nBPP, DWORD dwFlags = 0) throw();
		int CreateEx(int nWidth, int nHeight, int nBPP, DWORD eCompression, const DWORD *pdwBitmasks = NULL, DWORD dwFlags = 0) throw();

		int Attach(HBITMAP hBitmap, DIBOrientation eOrientation = DIBOR_DEFAULT) throw();
		int Attach(HGPBITMAP hgpBitmap);
		int Attach(CIUIImage &himgSrc);	// Keep the alpha channel.
		HBITMAP Detach() throw();

		int ConvertToDPIImage(int nCurDPIX, int nCurDPIY);

		int Destroy() throw();

		int GetBPP() const throw();
		int GetHeight() const throw();
		int GetWidth() const throw();
		bool IsHasAlphaChannel() const throw();
		bool IsGif() const throw();
		UINT GetFrameCount() const throw();
		int GetCurFrame() const throw();
		int SetCurFrame(int nCurFrame) throw();

		const void *GetPixelAddress(int x, int y) const throw();
		void *GetPixelAddress(int x, int y) throw();
		COLORREF GetPixel(int x, int y) const throw();

		void SetPixel(int x, int y, COLORREF color) throw();
		void SetPixelRGB(int x, int y, BYTE r, BYTE g, BYTE b) throw();
		void SetHasAlphaChannel(bool bHasAlphaChannel) throw();

		int SetShadow(LPCRECT lprcShadow) throw();
		CRect GetShowdow() const throw();

	public:
		// 操作HBITMAP
		static int NineGridBitmapG(HBITMAP hSrcBmp,
			POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			HBITMAP *phDstBmp, int nDestWidth, int nDestHeight);
		static int PartNineGridBitmapG(HBITMAP hSrcBmp,
			POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY, LPCRECT lprcSrcPart,
			HBITMAP *phDstBmp, int nDestWidth, int nDestHeight);
		static int PartNineGridBitmapG(HBITMAP hSrcBmp,
			POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY, int nPart, int nPartIndex,
			HBITMAP *phDstBmp, int nDestWidth, int nDestHeight);

		static int StretchBitmapG(HBITMAP hSrcBmp, HBITMAP *phDstBmp, int nDestWidth, int nDestHeight);
		static int TileBitmapG(HBITMAP hSrcBmp, HBITMAP *phDstBmp,
			int nDestWidth, int nDestHeight,
			int nXOriginSrc/* = 0*/, int nYOriginSrc/* = 0*/, int nSrcWidth/* = -1*/, int nSrcHeight/* = -1*/);
		static int CopyBitmapG(HBITMAP hSrc, HBITMAP *phDest);
		static int SubBitmapG(HBITMAP hSrcBmp, HBITMAP *phDstBmp, const CRect &rcSub);
		static int SubBitmapG(HBITMAP hSrcBmp, HBITMAP *phDstBmp, int nX, int nY, int nWidth, int nHeight);

		static int GetBitmapSizeG(HBITMAP obj, LPSIZE size);

		// 操作HIUIIMAGE
		static int ZoomImage(HIUIIMAGE himgSrc,
			IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			HIUIIMAGE *phimgDest, int nDestWidth, int nDestHeight);
		static int NineGridBitmap(HIUIIMAGE himgSrc,
			POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			HIUIIMAGE *phimgDest, int nDestWidth, int nDestHeight);
		static int PartNineGridBitmap(HIUIIMAGE himgSrc,
			POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			LPCRECT lprcSrcPart,
			HIUIIMAGE *phimgDest, int nDestWidth, int nDestHeight);
		static int PartNineGridBitmap(HIUIIMAGE himgSrc,
			POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			int nPart, int nPartIndex,
			HIUIIMAGE *phimgDest, int nDestWidth, int nDestHeight);
		static int StretchBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDest, int nDestWidth, int nDestHeight);
		static int TileBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDest, int nDestWidth, int nDestHeight,
			int nXOriginSrc/* = 0*/, int nYOriginSrc/* = 0*/, int nSrcWidth/* = -1*/, int nSrcHeight/* = -1*/);
		static int CopyBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDest);
		static int SubBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDst, const CRect &rcSub);
		static int SubBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDst, int nX, int nY, int nWidth, int nHeight);

		// 加载图片
		static HBITMAP LoadHBITMAP(LPCTSTR lpszFile);
		static HBITMAP LoadHBITMAPFromRes(UINT uID);
		static HBITMAP AutoLoadHBITMAP(LPCTSTR lpszFile);		// Auto load image from file or rc use gdi mode by CUIMgr::m_bLoadFromFile.
		static CString GetFileNameHexCode(LPCTSTR lpszImageName, BOOL bEncodeExt);

	private:
		int CreateFromGdiplusBitmap(__inout Gdiplus::Bitmap &bmSrc) throw();
		static int ComputePitch(int nWidth, int nBPP)
		{
			return (((nWidth * nBPP) + 31) / 32) * 4;
		}
		int UpdateBitmapInfo(DIBOrientation eOrientation);
		CIUIImage &operator=(const CIUIImage &himgSrc);

	public:
		static const DWORD createAlphaChannel;
		CString m_strImageName;

	private:
		int m_nAddRef;

		HBITMAP m_hBitmap;
		void *m_pBits;
		int m_nWidth;
		int m_nHeight;
		int m_nPitch;
		int m_nBPP;
		bool m_bIsDIBSection;
		bool m_bHasAlphaChannel;
		LONG m_iTransparentColor;
		COLORREF m_clrTransparentColor;

		// for gif
		bool m_bGif;
		UINT m_nFrameCount;
		int m_nCurFrame;
		PropertyItem *m_pPropertyItem;
		int m_nPropertyItemCount;

		CRect m_rcShadow;
	};

	namespace ImageManager
	{
		// 图片管理
		int IUISetControlImage(__inout HIUIIMAGE *phIUIImage, LPCTSTR lpszImageName);
		int AddCustomImage(LPCTSTR lpszImageFile, HBITMAP hImage);
		int AddCustomImage(LPCTSTR lpszImageFile, HIUIIMAGE hImage);
		int AddCustomImage(HBITMAP hImage, __out CString *pstrImageName);
		int AddCustomImage(HIUIIMAGE hImage);
		HIUIIMAGE CreateIUIImage(LPCTSTR lpszImageFile);
		// No Add ref, Needn't release. If the image not exist, no create.
		HIUIIMAGE GetIUIImage(LPCTSTR lpszImageFile);
		int ReleaseIUIImage(HIUIIMAGE hIUIImage);
	} // namespace ImageManager

	namespace FontManager
	{
		int AddCustomFont(LPCTSTR lpszFontResID, HFONT hFont);
		int UpdateIUIFont(LPCTSTR lpszFontResID, HFONT hFont, BOOL bDeleteOld);
		BOOL FindIUIFont(LPCTSTR lpszFontResID);
		HFONT CreateIUIFont(LPCTSTR lpszFontResID);
		int ReleaseIUIFont(LPCTSTR lpszFontResID);
		int ReleaseIUIFont(CFont *&pFont);
	} // namespace FontManager

	class CUIWndHandler : public CObject
	{
		friend class CUIWnd;
		friend class CUIMgr;
		friend class CTaskWndMgr;

	public:
		CUIWndHandler(UINT uWinID);
		virtual ~CUIWndHandler();

		virtual const struct IUI_MSGHANDLER_MAP *GetMessageMap() const = 0;

		UINT GetBindWindowID() const;
		CUIWnd *GetBindUIWnd();
		const CUIWnd *GetBindUIWnd() const;

		LPARAM SetUserData(LPARAM lParam);
		LPARAM GetUserData() const;

	protected:
		// return TRUE to refuse binds CUIWnd hanlde, otherwise FALSE.
		virtual BOOL PreTranslateMessage(CUIWnd *pUIWnd, MSG *pMsg);

	protected:
		int m_nMsgHandlerCount;

	private:
		UINT m_uBindWinID;
		CUIWnd *m_pBindUIWnd;
		LPARAM m_lParam;	// User data
	};

	typedef LRESULT(CUIWndHandler::*IUI_PMSGHANDLER)(CUIWnd *, WPARAM, LPARAM);

	struct IUI_MSGHANDLER_MAP	// See also: AFX_MSGMAP_ENTRY, BEGIN_MESSAGE_MAP
	{
		UINT nMessage;			// windows message
		UINT nCode;				// control code or WM_NOTIFY code
		UINT nID;				// control ID (or 0 for windows messages)
		UINT nLastID;			// used for entries specifying a range of control id's
		IUI_PMSGHANDLER pfn;	// routine to call (or special value)
	};

#define IUI_DECLARE_MESSAGE_MAP()\
protected:\
	virtual const IUI_MSGHANDLER_MAP *GetMessageMap() const;\

#define IUI_BEGIN_MESSAGE_MAP(theClass) \
	const IUI_MSGHANDLER_MAP *theClass::GetMessageMap() const\
	{\
		static const IUI_MSGHANDLER_MAP _messageEntries[] =\
			{

#define IUI_END_MESSAGE_MAP() \
	{0, 0, 0, 0, (IUI_PMSGHANDLER)NULL} \
}; \
(int)m_nMsgHandlerCount = sizeof(_messageEntries) / sizeof(IUI_MSGHANDLER_MAP);\
return _messageEntries;\
}

#define IUI_ON_COMMAND(id, memberFxn) \
	{WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)id, static_cast< IUI_PMSGHANDLER > (memberFxn)},

#define IUI_ON_COMMAND_RANGE(id, idLast, memberFxn) \
	{WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)idLast, static_cast< IUI_PMSGHANDLER > (memberFxn)},

#define IUI_ON_NOTIFY(wNotifyCode, id, memberFxn) \
	{WM_NOTIFY, wNotifyCode, (WORD)id, (WORD)id, static_cast< IUI_PMSGHANDLER > (memberFxn)},

#define IUI_ON_NOTIFY_RANGE(wNotifyCode, id, idLast, memberFxn) \
	{WM_NOTIFY, wNotifyCode, (WORD)id, (WORD)idLast, static_cast< IUI_PMSGHANDLER > (memberFxn)},

#define IUI_ON_MESSAGE(message, memberFxn) \
	{message, 0, 0, 0, (static_cast< IUI_PMSGHANDLER > (memberFxn))},

	// for general controls
#define IUI_ON_CONTROL(wNotifyCode, id, memberFxn) \
	{WM_COMMAND, (WORD)wNotifyCode, (WORD)id, (WORD)id, (static_cast< IUI_PMSGHANDLER >(memberFxn))},

#define IUI_ON_CONTROL_RANGE(wNotifyCode, id, idLast, memberFxn) \
	{WM_COMMAND, (WORD)wNotifyCode, (WORD)id, (WORD)idLast, (static_cast< IUI_PMSGHANDLER >(memberFxn))},

	// User Button Notification Codes
#define IUI_ON_BN_CLICKED(id, memberFxn) \
	IUI_ON_CONTROL(BN_CLICKED, id, memberFxn)

	class IUI_VARIANT : public _variant_t
	{
	public:
		// Constructors
		//
		IUI_VARIANT() throw();

		IUI_VARIANT(const _variant_t &varSrc) ;
		IUI_VARIANT(const _variant_t *pSrc) ;
		IUI_VARIANT(const IUI_VARIANT &varSrc) ;

		IUI_VARIANT(_variant_t &varSrc, bool fCopy) ;          // Attach _variant_t if !fCopy

		IUI_VARIANT(short sSrc, VARTYPE vtSrc = VT_I2) ;    // Creates a VT_I2, or a VT_BOOL
		IUI_VARIANT(long lSrc, VARTYPE vtSrc = VT_I4) ;     // Creates a VT_I4, a VT_ERROR, or a VT_BOOL
		IUI_VARIANT(float fltSrc) throw();                                   // Creates a VT_R4
		IUI_VARIANT(double dblSrc, VARTYPE vtSrc = VT_R8) ; // Creates a VT_R8, or a VT_DATE
		IUI_VARIANT(const CY &cySrc) throw();                                // Creates a VT_CY
		IUI_VARIANT(const _bstr_t &bstrSrc) ;               // Creates a VT_BSTR
		IUI_VARIANT(const wchar_t *pSrc) ;                  // Creates a VT_BSTR
		IUI_VARIANT(const char *pSrc) ;                     // Creates a VT_BSTR
		IUI_VARIANT(IDispatch *pSrc, bool fAddRef = true) throw();           // Creates a VT_DISPATCH
		IUI_VARIANT(bool boolSrc) throw();                                   // Creates a VT_BOOL
		IUI_VARIANT(IUnknown *pSrc, bool fAddRef = true) throw();            // Creates a VT_UNKNOWN
		IUI_VARIANT(const DECIMAL &decSrc) throw();                          // Creates a VT_DECIMAL
		IUI_VARIANT(BYTE bSrc) throw();                                      // Creates a VT_UI1

#if(_MSC_VER > 1200) // VC6.0
		IUI_VARIANT(char cSrc) throw();                                      // Creates a VT_I1
		IUI_VARIANT(unsigned short usSrc) throw();                           // Creates a VT_UI2
		IUI_VARIANT(unsigned long ulSrc) throw();                            // Creates a VT_UI4
		IUI_VARIANT(int iSrc) throw();                                       // Creates a VT_INT
		IUI_VARIANT(unsigned int uiSrc) throw();                             // Creates a VT_UINT
		IUI_VARIANT(__int64 i8Src) throw();                                  // Creates a VT_I8
		IUI_VARIANT(unsigned __int64 ui8Src) throw();                        // Creates a VT_UI8
#endif
		IUI_VARIANT(BYTE *pbVal);							// Creates a
		IUI_VARIANT(RECT rcSrc) throw();
		IUI_VARIANT(POINT ptSrc) throw();
		IUI_VARIANT(LOGFONT lf) throw();
		IUI_VARIANT(CONTROL_TYPE eSrc) throw();
		IUI_VARIANT(IMAGE_RESIZE_MODE eSrc) throw();
		IUI_VARIANT(CONTROL_LAYOUT_HOR eSrc) throw();
		IUI_VARIANT(CONTROL_LAYOUT_VER eSrc) throw();
		IUI_VARIANT(TEXT_ALIGN_HOR eSrc) throw();
		IUI_VARIANT(TEXT_ALIGN_VER eSrc) throw();
		IUI_VARIANT(FOREGROUND_ALIGN_HOR eSrc) throw();
		IUI_VARIANT(FOREGROUND_ALIGN_VER eSrc) throw();
		IUI_VARIANT(BTN_TYPE eSrc) throw();
		IUI_VARIANT(CHK_TYPE eSrc) throw();
		IUI_VARIANT(WINDOW_TYPE eSrc) throw();
		IUI_VARIANT(CMB_STYLE eSrc) throw();
		IUI_VARIANT(SPN_ALIGN eSrc) throw();
		IUI_VARIANT(SLD_EXT_STYLE eSrc) throw();
		IUI_VARIANT(LST_STYLE eSrc) throw();
		IUI_VARIANT(LST_ALIGN eSrc) throw();
		IUI_VARIANT(LST_SORT eSrc) throw();
		IUI_VARIANT(DATETIME_FORMAT eSrc) throw();
		IUI_VARIANT(BACKGROUND_TYPE eSrc) throw();
		IUI_VARIANT(PROGRESS_THUMB_MODE eSrc) throw();
		IUI_VARIANT(PROGRESS_THUMB_POSITION eSrc) throw();
		IUI_VARIANT(ALPHA_FORMAT eSrc) throw();
		IUI_VARIANT(MENU_BORDER_STYLE eSrc) throw();
		IUI_VARIANT(SPLITTER_ORIENTATION eSrc) throw();
		IUI_VARIANT(STACK_PANEL_ORIENTATION eSrc) throw();
		IUI_VARIANT(LINE_PEN_STYLE eSrc) throw();

		// Destructor
		//
		~IUI_VARIANT() throw() ;

		// Extractors
		//
#if(_MSC_VER <= 1200) // VC6.0
		operator int() const throw();
#endif
		operator BYTE *() const throw();
		operator _variant_t() const throw();
		operator LPCWSTR() const throw();
		operator LPCSTR() const throw();
		operator RECT() const throw();
		operator CRect() const throw();
		operator POINT() const throw();
		operator COLORREF() const throw();
		operator LOGFONT() const throw();
		operator CONTROL_TYPE() const throw();
		operator IMAGE_RESIZE_MODE() const throw();
		operator CONTROL_LAYOUT_HOR() const throw();
		operator CONTROL_LAYOUT_VER() const throw();
		operator TEXT_ALIGN_HOR() const throw();
		operator TEXT_ALIGN_VER() const throw();
		operator FOREGROUND_ALIGN_HOR() const throw();
		operator FOREGROUND_ALIGN_VER() const throw();
		operator BTN_TYPE() const throw();
		operator CHK_TYPE() const throw();
		operator WINDOW_TYPE() const throw();
		operator CMB_STYLE() const throw();
		operator SPN_ALIGN() const throw();
		operator SLD_EXT_STYLE() const throw();
		operator LST_STYLE() const throw();
		operator LST_ALIGN() const throw();
		operator LST_SORT() const throw();
		operator DATETIME_FORMAT() const throw();
		operator BACKGROUND_TYPE() const throw();
		operator PROGRESS_THUMB_MODE() const throw();
		operator PROGRESS_THUMB_POSITION() const throw();
		operator ALPHA_FORMAT() const throw();
		operator MENU_BORDER_STYLE() const throw();
		operator SPLITTER_ORIENTATION() const throw();
		operator STACK_PANEL_ORIENTATION() const throw();
		operator LINE_PEN_STYLE() const throw();

		// Assignment operations
		//
		IUI_VARIANT &operator=(const _variant_t &varSrc) ;
		IUI_VARIANT &operator=(const _variant_t *pSrc) ;
		IUI_VARIANT &operator=(const IUI_VARIANT &varSrc) ;

		IUI_VARIANT &operator=(short sSrc) ;                // Assign a VT_I2, or a VT_BOOL
		IUI_VARIANT &operator=(long lSrc) ;                 // Assign a VT_I4, a VT_ERROR or a VT_BOOL
		IUI_VARIANT &operator=(float fltSrc) ;              // Assign a VT_R4
		IUI_VARIANT &operator=(double dblSrc) ;             // Assign a VT_R8, or a VT_DATE
		IUI_VARIANT &operator=(const CY &cySrc) ;           // Assign a VT_CY
		IUI_VARIANT &operator=(const _bstr_t &bstrSrc) ;    // Assign a VT_BSTR
		IUI_VARIANT &operator=(const wchar_t *pSrc) ;       // Assign a VT_BSTR
		IUI_VARIANT &operator=(const char *pSrc) ;          // Assign a VT_BSTR
		IUI_VARIANT &operator=(IDispatch *pSrc) ;           // Assign a VT_DISPATCH
		IUI_VARIANT &operator=(bool boolSrc) ;              // Assign a VT_BOOL
		IUI_VARIANT &operator=(IUnknown *pSrc) ;            // Assign a VT_UNKNOWN
		IUI_VARIANT &operator=(const DECIMAL &decSrc) ;     // Assign a VT_DECIMAL
		IUI_VARIANT &operator=(BYTE bSrc) ;                 // Assign a VT_UI1

#if(_MSC_VER > 1200) // VC6.0
		IUI_VARIANT &operator=(char cSrc) ;                 // Assign a VT_I1
		IUI_VARIANT &operator=(unsigned short usSrc) ;      // Assign a VT_UI2
		IUI_VARIANT &operator=(unsigned long ulSrc) ;       // Assign a VT_UI4
		IUI_VARIANT &operator=(int iSrc) ;                  // Assign a VT_INT
		IUI_VARIANT &operator=(unsigned int uiSrc) ;        // Assign a VT_UINT
		IUI_VARIANT &operator=(__int64 i8Src) ;             // Assign a VT_I8
		IUI_VARIANT &operator=(unsigned __int64 ui8Src) ;   // Assign a VT_UI8
#endif
		IUI_VARIANT &operator=(BYTE *pbVal);
		IUI_VARIANT &operator=(const RECT &rcSrc);
		IUI_VARIANT &operator=(const POINT &ptSrc);
		IUI_VARIANT &operator=(const LOGFONT &lfSrc);
		IUI_VARIANT &operator=(CONTROL_TYPE eSrc);
		IUI_VARIANT &operator=(IMAGE_RESIZE_MODE eSrc);
		IUI_VARIANT &operator=(CONTROL_LAYOUT_HOR eSrc);
		IUI_VARIANT &operator=(CONTROL_LAYOUT_VER eSrc);
		IUI_VARIANT &operator=(TEXT_ALIGN_HOR eSrc);
		IUI_VARIANT &operator=(TEXT_ALIGN_VER eSrc);
		IUI_VARIANT &operator=(FOREGROUND_ALIGN_HOR eSrc);
		IUI_VARIANT &operator=(FOREGROUND_ALIGN_VER eSrc);
		IUI_VARIANT &operator=(BTN_TYPE eSrc);
		IUI_VARIANT &operator=(CHK_TYPE eSrc);
		IUI_VARIANT &operator=(WINDOW_TYPE eSrc);
		IUI_VARIANT &operator=(CMB_STYLE eSrc);
		IUI_VARIANT &operator=(SPN_ALIGN eSrc);
		IUI_VARIANT &operator=(SLD_EXT_STYLE eSrc);
		IUI_VARIANT &operator=(LST_STYLE eSrc);
		IUI_VARIANT &operator=(LST_ALIGN eSrc);
		IUI_VARIANT &operator=(LST_SORT eSrc);
		IUI_VARIANT &operator=(DATETIME_FORMAT eSrc);
		IUI_VARIANT &operator=(BACKGROUND_TYPE eSrc);
		IUI_VARIANT &operator=(PROGRESS_THUMB_MODE eSrc);
		IUI_VARIANT &operator=(PROGRESS_THUMB_POSITION eSrc);
		IUI_VARIANT &operator=(ALPHA_FORMAT eSrc);
		IUI_VARIANT &operator=(MENU_BORDER_STYLE eSrc);
		IUI_VARIANT &operator=(SPLITTER_ORIENTATION eSrc);
		IUI_VARIANT &operator=(STACK_PANEL_ORIENTATION eSrc);
		IUI_VARIANT &operator=(LINE_PEN_STYLE eSrc);

		// Comparison operations
		//
		bool operator==(const _variant_t &varSrc) const throw();
		bool operator==(const _variant_t *pSrc) const throw();
		bool operator!=(const _variant_t &varSrc) const throw();
		bool operator!=(const _variant_t *pSrc) const throw();

		bool operator==(LPCTSTR lpszSrc) const throw();
		bool operator!=(LPCTSTR lpszSrc) const throw();


		// Low-level operations
		//
		void Init();
		void IUI_VARIANT_Copy(const IUI_VARIANT &varSrc);
		void IUI_VARIANT_Clear();
		_variant_t &GetVARIANT() throw();
		_variant_t *GetAddress() ;
		void SetString(const char *pSrc) ; // used to set ANSI string
		int SetPointX(LONG x);
		int SetPointY(LONG y);

		// For VT_EX_RECT_WH
		int SetRectLeftNoSize(LONG lLeft);
		int SetRectTopNoSize(LONG lTop);
		int SetRectWidth(LONG lWidth);
		int SetRectHeight(LONG lHeight);

		// For VT_EX_RECT
		int SetRectLeft(LONG lLeft);
		int SetRectTop(LONG lTop);
		int SetRectRight(LONG lRight);
		int SetRectBottom(LONG lBottom);

		_bstr_t GetEnumString() const;
		int InitByEnumString(const CString &str);

		// For binding property
		int SetBindVariant(const IUI_VARIANT *pvarBind);
		const IUI_VARIANT *GetBindVariant() const;
		int RemoveBindVariant();
		int SetBindCompareValue(const IUI_VARIANT &var);
		const IUI_VARIANT GetBindCompareValue() const;

	protected:
		mutable char *strA;
		IUI_VARIANT *pvarBind;
		IUI_VARIANT *pvarCompare;

	public:
		VARTYPE vtEx;			// extended variant type. the value if enum VARENUMEX.
		bool bSaveToSkinFile;	// specifies the variable save to *.xui file or not.
		bool bNeedToConvert;	// specifies the variable is only valid in old version, it has been instead of by new property, it need to be convert to the new version.
		bool bReadOnly;
		_bstr_t strVarName;		// variable name
		_variant_t varDefaultValue;	// The default value
		_bstr_t strUIKeyName;			// key name in the.ui file
		_bstr_t strXUIElementName;		// element name in the.xui file
		_bstr_t strXUIAttributeName;	// attribute name in the.xui file
		bool bShowInPropertiesWnd;		// specifies the variable show in CPropertiesWnd or not. the following three variable is valid while bShowInPropertiesWnd is TRUE.
		bool bGroup;
		CString strGroupName;		// valid while bGroup is TRUE.
		CString strPropWndName;			// name in CPropertiesWnd
		CString strDescription;		// description of the variable
		CStringList lstStringOptions;		// options value of the variable
		int nMinValue;			// specifies the minimize value while vt is VT_I4 etc.
		int nMaxValue;			// specifies the maximize value while vt is VT_I4 etc.

		// Disabled while bind property is...
		// Disabled while bind property isn't ...
		bool bDisabledWhileBindIs;
		bool bDisabledWhileBindIsnot;
		long lBeBindCount;
	};

	//////////////////////////////////////////////////////////////////////////
	//

	struct ITEMTEXTCOLOR
	{
		HWND hWndSender;
		int nItem;					// Item index for CListCtrl or item HANDLE for CTreeCtrl.
		int nSubItem;				// Subitem for CListCtrl, otherwise, not use.
		COLORREF crItemText;
	};

	struct DRAWCHILDTEXT
	{
		HWND hWndSender;
		int item;
		int iSubItem;
		HDC hDC;
		RECT rcText;
		UINT uFormat;
		LPARAM lParam;
	};

	//////////////////////////////////////////////////////////////////////////
	//

#define GIF_TEXT                0x0001
#define GIF_POINTER             0x0002
#define GIF_PARAM               0x0004
#define GIF_ALL                 GIF_TEXT|GIF_POINTER|GIF_PARAM

	class CGridItem
	{
	public:
		CGridItem();
		~CGridItem();

	public:
		void SetText(const CString &strText);
		CString GetText() const;

		void SetPointer(void *pBuffer);
		void *GetPointer() const;

		void SetParam(LPARAM lParam);
		LPARAM GetParam() const;

	public:
		UINT m_uMask;

	private:
		CString m_strText;
		void *m_pBuffer;
		LPARAM m_lParam;
	};

	class CGrid
	{
	public:
		CGrid();
		~CGrid();

	public:
		// for header
		int InsertColumn(int nCol, const CString &strColumnHeading/* = _T("")*/);
		BOOL DeleteColumn(int nCol);

		int GetColumnCount() const;

		BOOL SetColumn(int nCol, const CGridItem *pColumn);
		BOOL GetColumn(int nCol, CGridItem *pColumn) const;

		BOOL SetColumnText(int nCol, const CString &strColumn);
		BOOL GetColumnText(int nCol, CString *pstrColumn) const;

		BOOL SetColumnPointer(int nCol, void *pBuffer);
		void *GetColumnPointer(int nCol) const;

		BOOL SetColumnData(int nCol, LPARAM lParam);
		LPARAM GetColumnData(int nCol) const;

		BOOL SwapColumn(int nFirst, int nSecond);

		// for item
		int InsertItem(int nItem, LPCTSTR lpszItem);
		BOOL DeleteItem(int nItem);
		BOOL DeleteAllItems();	// Not delete column, same as CListCtrl with report style.

		int GetItemCount() const;

		BOOL SetItem(int nItem, int nSubItem, const CGridItem *pItem);
		BOOL GetItem(int nItem, int nSubItem, CGridItem *pItem) const;

		BOOL SetItemText(int nItem, int nSubItem, const CString &strText);
		CString GetItemText(int nItem, int nSubItem) const;

		BOOL SetItemPointer(int nItem, int nSubItem, void *pBuffer);
		void *GetItemPointer(int nItem, int nSubItem) const;

		BOOL SetItemData(int nItem, int nSubItem, LPARAM lParam);
		LPARAM GetItemData(int nItem, int nSubItem) const;

		BOOL SortItems(PFNLVCOMPARE pfnCompare, DWORD dwData);
		int FindItem(LV_FINDINFO *pFindInfo, int nStart/* = -1*/) const;
		BOOL SwapItem(int nFirst, int nSecond);

		// Check column or item valid
		BOOL IsColumnValid(int nCol) const;
		BOOL IsItemValid(int nItem, int nSubItem) const;

	private:
		std::vector<CGridItem> m_vHeader;				// Same as header of CListCtrl.
		std::vector<std::vector<CGridItem> > m_vvData;	// Same as items of CListCtrl.
	};

	//////////////////////////////////////////////////////////////////////////

	typedef std::map<WNDPROC, WNDPROC> WndProcList;
	typedef std::map<HWND, WndProcList> HwndProcMap;

	class CWndProcList
	{
	private:
		CWndProcList();
		~CWndProcList();

	public:
		static int AddToMap(HWND hWnd, WNDPROC procOld, WNDPROC procNew);
		static WNDPROC GetOldWndProc(HWND hWnd, WNDPROC wndprocCur);

	protected:
		static HwndProcMap m_mapHwndProc;
	};

	//////////////////////////////////////////////////////////////////////////

	class SplitterStruct
	{
	public:
		SplitterStruct();
		~SplitterStruct();

	public:
		CRect m_rcNew;
		CRect m_rcOld;
		SPLITTER_DRAG_ORIENTATION m_eDragOrientation;
	};

	//////////////////////////////////////////////////////////////////////////
	struct IUI_LOGFONT : public LOGFONT
	{
		IUI_LOGFONT();
		IUI_LOGFONT(const LOGFONT &lf);
		IUI_LOGFONT(const IUI_LOGFONT &lf);
		virtual ~IUI_LOGFONT();

		IUI_LOGFONT &operator=(const LOGFONT &lf);
		IUI_LOGFONT &operator=(const IUI_LOGFONT &lf);
	};

	struct CTRLPROPERTIES
	{
		CTRLPROPERTIES();
		virtual ~CTRLPROPERTIES();

	protected:
		static int InitBoolVariant(IUI_VARIANT *pVariant, bool bInitValue, LPCTSTR lpszXUIElementName, LPCTSTR lpszPropWndName, bool bGroup = false, LPCTSTR lpszGroupName = NULL);
		static int InitResFontIDVariant(IUI_VARIANT *pVariant, LPCTSTR lpszXUIElementName, LPCTSTR lpszPropWndName, bool bGroup = false, LPCTSTR lpszGroupName = NULL);
		static int InitResColorIDVariant(IUI_VARIANT *pVariant, LPCTSTR lpszXUIElementName, LPCTSTR lpszPropWndName, bool bGroup = false, LPCTSTR lpszGroupName = NULL);

	public:
		virtual LONG GetPropertyCount() const;
		IUI_VARIANT *GetProperty(int nIndex);
		const IUI_VARIANT *GetProperty(int nIndex) const;
		int SetProperty(int nIndex, const IUI_VARIANT *pVariant);
		int GetIndex(const IUI_VARIANT *pVar) const;

		static CString GetIDPrefix(CONTROL_TYPE eControlType);
		CString GetClassName() const;
		CString GetVariablePrefix() const;
		CString GetVariableName() const;

		// disable some properties
		int DisableBackgroundProperties(BOOL bOnlyHighlightSelected);
		int DisableFontProperties(BOOL bOnlyHighlightSelected);
		int DisableColorProperties(BOOL bOnlyHighlightSelected);

		// Scroll bar
		virtual bool IsEnableCustomScrollBar() const;

		// Resource
		virtual bool IsFontResource() const;
		virtual bool IsColorResource() const;


		static int IUIGetControlFont1(const CTRLPROPERTIES *pCtrlProp, CString *pstrFontResID);
		static int IUIGetControlFont2(const CTRLPROPERTIES *pCtrlProp, CString *pstrFontResID);
		static int IUIGetControlFont4(const CTRLPROPERTIES *pCtrlProp, CString *pstrFontResID);
		static int IUIGetControlFont8(const struct TOGGLEBTNPROPERTIES *pCtrlProp, CString *pstrFontResID);

		static int IUIGetControlColor4(
			const CTRLPROPERTIES *pCtrlProp,
			COLORREF *pcrN,
			COLORREF *pcrH,
			COLORREF *pcrS,
			COLORREF *pcrD);
		static int IUIGetControlColor8(
			const TOGGLEBTNPROPERTIES *pCtrlProp,
			COLORREF *pcrN,
			COLORREF *pcrH,
			COLORREF *pcrS,
			COLORREF *pcrD,
			COLORREF *pcrCN,
			COLORREF *pcrCH,
			COLORREF *pcrCS,
			COLORREF *pcrCD);

		static int IUIGetBackground2(
			const CTRLPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);
		static int IUIGetBackground4(
			const CTRLPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);

		static int IUIGetToggleButtonBackground8(
			const TOGGLEBTNPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);
		static int IUIGetToggleButtonForeground8(
			const TOGGLEBTNPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);

		static int IUIGetButtonForeground4(
			const struct BTNBASEPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);
		static int IUIGetStaticForeground2(
			const struct TXTPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);

		static int IUIGetThumbImages4(
			const struct HORSBPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);
		static int IUIGetThumbImages4(
			const struct VERSBPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);
		static int IUIGetThumbImages4(
			const struct SLDPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);

		static int IUIGetSliderThumb4(
			const struct SLDEXPROPERTIES *pCtrlProp,
			BOOL *pbCombineImage,
			CString *pstrImageName);

	public:
		IUI_VARIANT m_lStructSize;
		IUI_VARIANT m_eControlType;	// enum CONTROL_TYPE.
		IUI_VARIANT m_strID;			// Macro in resource.h
		IUI_VARIANT m_nID;			// ID in resource.h
		IUI_VARIANT m_strBindStyle;
		IUI_VARIANT m_rcControl;
		IUI_VARIANT m_bVisible;
		IUI_VARIANT m_bDisabled;
		IUI_VARIANT m_bTabStop;
		IUI_VARIANT m_bClipChildren;
		IUI_VARIANT m_bClipSiblings;
		IUI_VARIANT m_eHorLayoutMode;
		IUI_VARIANT m_eVerLayoutMode;
		IUI_VARIANT m_rcLayoutMargin;		// Margin relative to parent.
		IUI_VARIANT m_rcRegion;			// The region of the control. each value means the margin to board, all are 0 means no region.

		IUI_VARIANT m_bDrawDefault;
		IUI_VARIANT m_bSpecifyBackgroundImages;
		IUI_VARIANT m_bCombineBackgroundImages;
		IUI_VARIANT m_strBKCombine;
		IUI_VARIANT m_strBKN;
		IUI_VARIANT m_strBKH;
		IUI_VARIANT m_strBKS;
		IUI_VARIANT m_strBKD;
		IUI_VARIANT m_strResColorBk;
		IUI_VARIANT m_lBkImgTransparent;	// 0-255
		IUI_VARIANT m_eBkImageResizeMode;	// IMAGE_RESIZE_MODE
		IUI_VARIANT m_ptImageResize;	// Resize image, Valid only eImageStretchMode is ISM_9GRID
		IUI_VARIANT m_uXRepeatPixel;
		IUI_VARIANT m_uYRepeatPixel;

		// For compatible old version of *.xui.
		IUI_VARIANT m_lfN;
		IUI_VARIANT m_lfH;
		IUI_VARIANT m_lfS;
		IUI_VARIANT m_lfD;

		IUI_VARIANT m_strResFontIDN;
		IUI_VARIANT m_bHSameNFont;		// the font of Highlight state same to normal
		IUI_VARIANT m_strResFontIDH;
		IUI_VARIANT m_bSSameNFont;
		IUI_VARIANT m_strResFontIDS;
		IUI_VARIANT m_bDSameNFont;
		IUI_VARIANT m_strResFontIDD;

		IUI_VARIANT m_crN;
		IUI_VARIANT m_crH;
		IUI_VARIANT m_crS;
		IUI_VARIANT m_crD;

		IUI_VARIANT m_strResColorIDN;
		IUI_VARIANT m_bHSameNColor;
		IUI_VARIANT m_strResColorIDH;
		IUI_VARIANT m_bSSameNColor;
		IUI_VARIANT m_strResColorIDS;
		IUI_VARIANT m_bDSameNColor;
		IUI_VARIANT m_strResColorIDD;
	};

	struct OLDCTRLPROPERTIES	// For old version of *.xui
	{
		OLDCTRLPROPERTIES();
		virtual ~OLDCTRLPROPERTIES();

	public:
		virtual LONG GetPropertyCount() const;
		IUI_VARIANT *GetVariant(int nIndex);
		const IUI_VARIANT *GetVariant(int nIndex) const;
		int SetVariant(int nIndex, const IUI_VARIANT *pVariant);
		int GetIndex(const IUI_VARIANT *pVar) const;

	public:
		IUI_VARIANT m_lStructSize;
	};

	struct RECTWLPROPERTIES : public CTRLPROPERTIES
	{
		RECTWLPROPERTIES();
		virtual ~RECTWLPROPERTIES();

		// The position of the rect control will show the child window
		IUI_VARIANT m_strBindWindowID;
	};

	struct SPLITTERWLPROPERTIES : public RECTWLPROPERTIES
	{
		SPLITTERWLPROPERTIES();
		virtual ~SPLITTERWLPROPERTIES();

		IUI_VARIANT m_eOrientation;
	};

	struct LINEWLPROPERTIES : public RECTWLPROPERTIES
	{
		LINEWLPROPERTIES();
		virtual ~LINEWLPROPERTIES();

		IUI_VARIANT m_ePenStyle;		// LINE_PEN_STYLE
		IUI_VARIANT m_lLineWidth;

		IUI_VARIANT m_strResColorIDStart;
		IUI_VARIANT m_strResColorIDEnd;
		IUI_VARIANT m_ptStart;
		IUI_VARIANT m_ptEnd;
	};

	struct PICWLPROPERTIES : public RECTWLPROPERTIES
	{
		PICWLPROPERTIES();
		virtual ~PICWLPROPERTIES();

		IUI_VARIANT m_strFg;
		IUI_VARIANT m_btFgImgTransparent;
		IUI_VARIANT m_eForegroundHorAlignMode;
		IUI_VARIANT m_eForegroundVerAlignMode;
		IUI_VARIANT m_rcForegroundMargin;
	};

	struct TWMGRPROPERTIES : public PICWLPROPERTIES
	{
		TWMGRPROPERTIES();
		virtual ~TWMGRPROPERTIES();
	};

	struct DOCKPNLPROPERTIES : public PICWLPROPERTIES
	{
		DOCKPNLPROPERTIES();
		virtual ~DOCKPNLPROPERTIES();

		IUI_VARIANT m_bLastChildFill;
	};

	struct UGRIDPNLPROPERTIES : public PICWLPROPERTIES
	{
		UGRIDPNLPROPERTIES();
		virtual ~UGRIDPNLPROPERTIES();
	};

	struct STACKPNLPROPERTIES : public PICWLPROPERTIES
	{
		STACKPNLPROPERTIES();
		virtual ~STACKPNLPROPERTIES();

		IUI_VARIANT m_eOrientation;
	};

	// Horizontal scroll bar
	struct HORSBPROPERTIES : public CTRLPROPERTIES
	{
		HORSBPROPERTIES();
		virtual ~HORSBPROPERTIES();

		IUI_VARIANT m_bCombineThumbImages;
		IUI_VARIANT m_strThumbCombine;
		IUI_VARIANT m_strThumbN;
		IUI_VARIANT m_strThumbH;
		IUI_VARIANT m_strThumbS;
		IUI_VARIANT m_strThumbD;
		IUI_VARIANT m_ptThumbResize;

		IUI_VARIANT m_strLeftArrow;
		IUI_VARIANT m_strRightArrow;

		IUI_VARIANT m_nLeftButtonWidth;
		IUI_VARIANT m_nRightButtonWidth;
	};

	// Vertical scroll bar
	struct VERSBPROPERTIES : public CTRLPROPERTIES
	{
		VERSBPROPERTIES();
		virtual ~VERSBPROPERTIES();

		IUI_VARIANT m_bCombineThumbImages;
		IUI_VARIANT m_strThumbCombine;
		IUI_VARIANT m_strThumbN;
		IUI_VARIANT m_strThumbH;
		IUI_VARIANT m_strThumbS;
		IUI_VARIANT m_strThumbD;
		IUI_VARIANT m_ptThumbResize;

		IUI_VARIANT m_strUpArrow;
		IUI_VARIANT m_strDownArrow;

		IUI_VARIANT m_nUpButtonHeight;
		IUI_VARIANT m_nDownButtonHeight;
	};

	struct BTNBASEPROPERTIES : public CTRLPROPERTIES
	{
		BTNBASEPROPERTIES();
		virtual ~BTNBASEPROPERTIES();

		IUI_VARIANT m_bR2LReadOrder;
		IUI_VARIANT m_bMultiline;
		IUI_VARIANT m_bEndEllipsis;		// only valid while m_bMultiline is FALSE

		IUI_VARIANT	m_strCaption;
		IUI_VARIANT m_strTag;
		IUI_VARIANT m_strMaskBmp;

		// Click the button will popup a modal dialog;
		IUI_VARIANT m_strModalDlgID;

		// Click the button will popup a modeless window.
		IUI_VARIANT m_strModelessWndID;

		IUI_VARIANT m_bUseToolTip;
		IUI_VARIANT m_strToolTip;
		IUI_VARIANT m_lAutoPopDelayTime;	// See MSDN: CToolTipCtrl::GetDelayTime.
		IUI_VARIANT m_lInitialDelayTime;
		IUI_VARIANT m_lReshowDelayTime;

		IUI_VARIANT m_eTextHorAlignMode;
		IUI_VARIANT m_eTextVerAlignMode;
		IUI_VARIANT m_rcPadding4Text;

		IUI_VARIANT m_bSpecifyForegroundImages;
		IUI_VARIANT m_bCombineForegroundImages;
		IUI_VARIANT m_strFGCombine;
		IUI_VARIANT m_strFGN;
		IUI_VARIANT m_strFGH;
		IUI_VARIANT m_strFGS;
		IUI_VARIANT m_strFGD;
		IUI_VARIANT m_eForegroundHorAlignMode;
		IUI_VARIANT m_eForegroundVerAlignMode;
		IUI_VARIANT m_rcPadding4Foreground;
	};

	// for old version *.xui
	struct OLDBTNBASEPROPERTIES : public OLDCTRLPROPERTIES
	{
		OLDBTNBASEPROPERTIES();
		virtual ~OLDBTNBASEPROPERTIES();

		// 版本范围：[, 4921)
		IUI_VARIANT m_lHorAlign;
		IUI_VARIANT m_lVerAlign;
	};

	struct BTNPROPERTIES : public BTNBASEPROPERTIES
	{
		BTNPROPERTIES();
		virtual ~BTNPROPERTIES();

		IUI_VARIANT m_eBtnType;
		IUI_VARIANT m_bDefPushButton;
	};

	struct TOGGLEBTNPROPERTIES : public BTNBASEPROPERTIES
	{
		TOGGLEBTNPROPERTIES();
		virtual ~TOGGLEBTNPROPERTIES();

		IUI_VARIANT m_strBKCN;
		IUI_VARIANT m_strBKCH;
		IUI_VARIANT m_strBKCS;
		IUI_VARIANT m_strBKCD;

		IUI_VARIANT m_strFGCN;
		IUI_VARIANT m_strFGCH;
		IUI_VARIANT m_strFGCS;
		IUI_VARIANT m_strFGCD;

		IUI_VARIANT m_bCNSameNFont;		// the font of Normal state same to normal
		IUI_VARIANT m_strResFontIDCN;
		IUI_VARIANT m_bCHSameNFont;
		IUI_VARIANT m_strResFontIDCH;
		IUI_VARIANT m_bCSSameNFont;
		IUI_VARIANT m_strResFontIDCS;
		IUI_VARIANT m_bCDSameNFont;
		IUI_VARIANT m_strResFontIDCD;

		IUI_VARIANT m_bCNSameNColor;
		IUI_VARIANT m_strResColorIDCN;
		IUI_VARIANT m_bCHSameNColor;
		IUI_VARIANT m_strResColorIDCH;
		IUI_VARIANT m_bCSSameNColor;
		IUI_VARIANT m_strResColorIDCS;
		IUI_VARIANT m_bCDSameNColor;
		IUI_VARIANT m_strResColorIDCD;
	};

	struct CHKPROPERTIES : public TOGGLEBTNPROPERTIES
	{
		CHKPROPERTIES();
		virtual ~CHKPROPERTIES();

		IUI_VARIANT m_eChkType;
	};

	struct RADPROPERTIES : public TOGGLEBTNPROPERTIES
	{
		RADPROPERTIES();
		virtual ~RADPROPERTIES();

		IUI_VARIANT m_bGroup;

		// The task window ID while the radio button as a tab of CTaskWndMgr.
		IUI_VARIANT m_strBindTaskWndID;
		// The parent ID of the task window. A window may be include more then one CTaskWndMgr
		IUI_VARIANT m_strBindTaskMgrID;
	};

	struct TXTPROPERTIES : public CTRLPROPERTIES
	{
		TXTPROPERTIES();
		virtual ~TXTPROPERTIES();

		IUI_VARIANT m_bR2LReadOrder;
		IUI_VARIANT m_bNoWrap;
		IUI_VARIANT m_bPathEllipsis; // only valid while m_bNoWrap is TRUE.
		IUI_VARIANT m_bEndEllipsis;	// only valid while m_bNoWrap is TRUE.

		IUI_VARIANT m_strCaption;

		//
		IUI_VARIANT m_bSpecifyForegroundImages;
		IUI_VARIANT m_bCombineForegroundImages;
		IUI_VARIANT m_strFGCombine;
		IUI_VARIANT m_strFGN;
		IUI_VARIANT m_strFGD;
		IUI_VARIANT m_eForegroundHorAlignMode;
		IUI_VARIANT m_eForegroundVerAlignMode;
		IUI_VARIANT m_rcPadding4Foreground;

		// Tooltip
		IUI_VARIANT m_bUseToolTip;
		IUI_VARIANT m_strToolTip;

		// alignment
		IUI_VARIANT m_eTextHorAlignMode;
		IUI_VARIANT m_eTextVerAlignMode;
		IUI_VARIANT m_rcPadding4Text;

		// Shadow Text
		IUI_VARIANT m_bShadowText;
		IUI_VARIANT m_crTextShadow;
		IUI_VARIANT m_ptTextShadowOffset;
	};

	struct EDTPROPERTIES : public CTRLPROPERTIES
	{
		EDTPROPERTIES();
		virtual ~EDTPROPERTIES();

		IUI_VARIANT m_bHScroll;
		IUI_VARIANT m_bVScroll;
		IUI_VARIANT m_bAutoHScroll;
		IUI_VARIANT m_bAutoVScroll;
		IUI_VARIANT m_bDisableNoScroll; // For Rich Edit
		IUI_VARIANT m_bLowercase;
		IUI_VARIANT m_bUppercase;
		IUI_VARIANT m_bNumber;
		IUI_VARIANT m_bPassword;
		IUI_VARIANT m_bReadOnly;
		IUI_VARIANT m_bMultiline;
		IUI_VARIANT m_bWantReturn;
		IUI_VARIANT m_bR2LReadOrder;

		IUI_VARIANT m_strTag;

		// Text Alignment
		IUI_VARIANT m_eTextHorAlignMode;
		IUI_VARIANT m_rcPadding;
	};

	struct RICHEDITPROPERTIES : public EDTPROPERTIES
	{
		RICHEDITPROPERTIES();
		virtual ~RICHEDITPROPERTIES();
	};

	struct IMREPROPERTIES : public RICHEDITPROPERTIES
	{
		IMREPROPERTIES();
		virtual ~IMREPROPERTIES();

		//	IUI_VARIANT m_sizeFace;
	};

	struct CMBPROPERTIES : public CTRLPROPERTIES
	{
		CMBPROPERTIES();
		virtual ~CMBPROPERTIES();

		IUI_VARIANT m_eComboStyle;
		IUI_VARIANT m_bVScroll;
		IUI_VARIANT m_bAutoHScroll;
		IUI_VARIANT m_bDisableNoScroll;
		IUI_VARIANT m_bSort;
		IUI_VARIANT m_bUppercase;
		IUI_VARIANT m_bLowercase;

		// Text horizontal alignment
		IUI_VARIANT m_eTextHorAlignModeN;
		IUI_VARIANT m_eTextHorAlignModeH;
		IUI_VARIANT m_eTextHorAlignModeS;
		IUI_VARIANT m_eTextHorAlignModeD;

		// Text vertical alignment
		IUI_VARIANT m_eTextVerAlignModeN;
		IUI_VARIANT m_eTextVerAlignModeH;
		IUI_VARIANT m_eTextVerAlignModeS;
		IUI_VARIANT m_eTextVerAlignModeD;

		// Text Margin
		IUI_VARIANT m_rcTextMarginN;
		IUI_VARIANT m_rcTextMarginH;
		IUI_VARIANT m_rcTextMarginS;
		IUI_VARIANT m_rcTextMarginD;

		IUI_VARIANT m_lDropListItemHeight;
		IUI_VARIANT m_lDropListHeight;
		IUI_VARIANT m_lBtnWidth;
		IUI_VARIANT m_strTag;
	};

	struct SPNPROPERTIES : public CTRLPROPERTIES
	{
		SPNPROPERTIES();
		virtual ~SPNPROPERTIES();

		IUI_VARIANT m_eAlignMode;
		IUI_VARIANT m_bVertical;
		IUI_VARIANT m_bAutoBuddy;
		IUI_VARIANT m_bSetBuddyInt;
		IUI_VARIANT m_bNoThousands;
		IUI_VARIANT m_bWrap;
		IUI_VARIANT m_bArrowKeys;
		IUI_VARIANT m_bHotTrack;
	};

	struct SLDPROPERTIES : public CTRLPROPERTIES
	{
		SLDPROPERTIES();
		virtual ~SLDPROPERTIES();

		IUI_VARIANT m_eExtendedStyle;
		IUI_VARIANT m_bVertical;
		IUI_VARIANT m_bUseToolTip;
		IUI_VARIANT m_bUseMark;
		IUI_VARIANT m_bMarkToThumb;		// TRUE: Stretch(9 Grid or tile) mark image to thumb; FALSE: to all client.

		// Thumb Images
		IUI_VARIANT m_bCombineThumbImages;
		IUI_VARIANT m_strThumbCombine;
		IUI_VARIANT m_strThumbN;
		IUI_VARIANT m_strThumbH;
		IUI_VARIANT m_strThumbS;
		IUI_VARIANT m_strThumbD;

		// Show tip window
		IUI_VARIANT m_bEnableTipWindow;
		IUI_VARIANT m_sizeTipWindowOffset;
	};

	struct SLDEXPROPERTIES : public SLDPROPERTIES
	{
		SLDEXPROPERTIES();
		virtual ~SLDEXPROPERTIES();
	};

	struct PRGPROPERTIES : public CTRLPROPERTIES
	{
		PRGPROPERTIES();
		virtual ~PRGPROPERTIES();

		IUI_VARIANT m_bVertical;

		// Progress images
		IUI_VARIANT m_eThumbMode;		// PROGRESS_THUMB_MODE
		IUI_VARIANT m_bCombineThumbImages;
		IUI_VARIANT m_strThumbCombine;
		IUI_VARIANT m_strThumbN;
		IUI_VARIANT m_strThumbD;
		IUI_VARIANT m_eThumbPosition;	// PROGRESS_THUMB_POSITION
		IUI_VARIANT m_rcThumbOriginalSpecified;

		// Show percent text
		IUI_VARIANT m_bShowText;
		IUI_VARIANT m_lHorAlignN;
		IUI_VARIANT m_lVerAlignN;
	};

	struct HOTPROPERTIES : public CTRLPROPERTIES
	{
		HOTPROPERTIES();
		virtual ~HOTPROPERTIES();
	};

	struct IPAPROPERTIES : public CTRLPROPERTIES
	{
		IPAPROPERTIES();
		virtual ~IPAPROPERTIES();

		// Border
		IUI_VARIANT m_rcBorder;
		IUI_VARIANT m_strResColorBorder;

		// Filler
		IUI_VARIANT m_bCombineFillerImage;
		IUI_VARIANT m_strCombineFillerImage;
		IUI_VARIANT m_strFillerImageN;
		IUI_VARIANT m_strFillerImageH;
		IUI_VARIANT m_strFillerImageS;
		IUI_VARIANT m_strFillerImageD;
		IUI_VARIANT m_lFillerWidth;

		// Field
		IUI_VARIANT m_bCombineFieldImage;
		IUI_VARIANT m_strCombineFieldImage;
		IUI_VARIANT m_strFieldImageN;
		IUI_VARIANT m_strFieldImageH;
		IUI_VARIANT m_strFieldImageS;
		IUI_VARIANT m_strFieldImageD;
		IUI_VARIANT m_lFieldHeight;
		IUI_VARIANT m_eFieldAlignModeVer;
	};

	struct LSTCTRLPROPERTIES : public CTRLPROPERTIES
	{
		LSTCTRLPROPERTIES();
		virtual ~LSTCTRLPROPERTIES();

		virtual bool IsEnableCustomScrollBar() const;

		// General
		IUI_VARIANT m_bBorder;
		IUI_VARIANT m_eListStyle;
		IUI_VARIANT m_eListSort;
		IUI_VARIANT m_bEditLabels;
		IUI_VARIANT m_bShowSelAlways;
		IUI_VARIANT m_bSingleSel;
		IUI_VARIANT m_bHoldCtrlKey;	// Only valid while bSingleSel is false.
		IUI_VARIANT m_bOwnerData;
		IUI_VARIANT m_bCheckBoxes;
		IUI_VARIANT m_bInfoTooltip;
		IUI_VARIANT m_bTrackSelect;
		IUI_VARIANT m_bTwoClickActivate;
		IUI_VARIANT m_bEnableOwnerDraw;
		IUI_VARIANT m_strTag;

		// Only for report style
		IUI_VARIANT m_bFullRowSelect;
		IUI_VARIANT m_bSubItemImages;

		// Icon and small icon view
		IUI_VARIANT m_eListAlign;
		IUI_VARIANT m_bAutoArrange;	// for icon and small icon view
		IUI_VARIANT m_bNoScroll;		// This style is not compatible with the LVS_LIST or LVS_REPORT styles
		IUI_VARIANT m_bNoLabelWrap;	// for icon view

		// Grid line
		IUI_VARIANT m_bShowGridLine;
		IUI_VARIANT m_bDraw0Row;		// Draw 0 row or not. Only valid while bEnableOwnerDraw is true.
		IUI_VARIANT m_lXOffset;		// The horizontal offset of the line. Only valid while bEnableOwnerDraw is true.
		IUI_VARIANT m_crGridLine;		// Only valid while bEnableOwnerDraw is true.

		// Normal Item
		IUI_VARIANT m_bEnableNormalItemEffect;
		IUI_VARIANT m_eNormalItemBkType;
		IUI_VARIANT m_strNormalItemBkColorResID;
		IUI_VARIANT m_strNormalItemBkImage;
		IUI_VARIANT m_ptNormalItemBkResize;

		// Highlight Item
		IUI_VARIANT m_bEnableHighlightItemEffect;
		IUI_VARIANT m_eHighlightItemBkType;
		IUI_VARIANT m_strHighlightItemBkColorResID;
		IUI_VARIANT m_strHighlightItemBkImage;
		IUI_VARIANT m_ptHighlightItemBkResize;

		// Selected Item
		IUI_VARIANT m_bSelectedFitClient;		// Only valid while list with report style
		IUI_VARIANT m_eSelectedItemBkType;
		IUI_VARIANT m_strSelectedItemBkColorResID;
		IUI_VARIANT m_strSelectedItemBkImage;
		IUI_VARIANT m_ptSelectedItemBkResize;

		// Normal and Disabled Item
		IUI_VARIANT m_bEnableNormalDisabledItemEffect;
		IUI_VARIANT m_eNormalDisabledItemBkType;
		IUI_VARIANT m_strNormalDisabledItemBkColorResID;
		IUI_VARIANT m_strNormalDisabledItemBkImage;
		IUI_VARIANT m_ptNormalDisabledItemBkResize;

		// Selected and Disabled Item
		IUI_VARIANT m_bEnableSelectedDisabledItemEffect;
		IUI_VARIANT m_eSelectedDisabledItemBkType;
		IUI_VARIANT m_strSelectedDisabledItemBkColorResID;
		IUI_VARIANT m_strSelectedDisabledItemBkImage;
		IUI_VARIANT m_ptSelectedDisabledItemBkResize;
		IUI_VARIANT m_bSDSameNFont;
		IUI_VARIANT m_strResFontIDSD;
		IUI_VARIANT m_bSDSameNColor;
		IUI_VARIANT m_strResColorIDSD;

		// Thumbnail view
		IUI_VARIANT m_lTotalItemWidth;
		IUI_VARIANT m_lTotalItemHeight;

		// Thumbnail Background Position
		IUI_VARIANT m_rcThumbnailBkN;
		IUI_VARIANT m_rcThumbnailBkH;
		IUI_VARIANT m_rcThumbnailBkS;
		IUI_VARIANT m_rcThumbnailBkD;

		// Thumbnail Position
		IUI_VARIANT m_rcThumbnailN;
		IUI_VARIANT m_rcThumbnailH;
		IUI_VARIANT m_rcThumbnailS;
		IUI_VARIANT m_rcThumbnailD;

		// Thumbnail Label Position
		IUI_VARIANT m_rcThumbnailLabelN;
		IUI_VARIANT m_rcThumbnailLabelH;
		IUI_VARIANT m_rcThumbnailLabelS;
		IUI_VARIANT m_rcThumbnailLabelD;

		// Thumbnail Icon Position
		IUI_VARIANT m_rcThumbnailIconN;
		IUI_VARIANT m_rcThumbnailIconH;
		IUI_VARIANT m_rcThumbnailIconS;
		IUI_VARIANT m_rcThumbnailIconD;

		// Header
		IUI_VARIANT m_bNoHeader;
		IUI_VARIANT m_bNoSortHeader;
		IUI_VARIANT m_nHeaderHeight;
		IUI_VARIANT m_bHeaderTrack;
		IUI_VARIANT m_bHeaderDragDrop;
		IUI_VARIANT m_strHeaderStyleID;

		// Custom scroll bar
		IUI_VARIANT m_bUseCustomScrollBar;
		IUI_VARIANT m_strHorScrollbarID;
		IUI_VARIANT m_strVerScrollbarID;
	};

	struct TREECTRLPROPERTIES : public CTRLPROPERTIES
	{
		TREECTRLPROPERTIES();
		virtual ~TREECTRLPROPERTIES();

		virtual bool IsEnableCustomScrollBar() const;

		IUI_VARIANT m_bBorder;
		IUI_VARIANT m_bCheckBoxes;
		IUI_VARIANT m_bHasButtons;
		IUI_VARIANT m_bHasLines;
		IUI_VARIANT m_bLinesAtRoot;
		IUI_VARIANT m_bFullRowSel;
		IUI_VARIANT m_bShowSelAlways;
		IUI_VARIANT m_bDisableDragDrop;
		IUI_VARIANT m_bEditLabels;
		IUI_VARIANT m_bScroll;
		IUI_VARIANT m_bHoverScrollBarMode;
		IUI_VARIANT m_bNonEvenHeight;
		IUI_VARIANT m_bSingleExpand;
		IUI_VARIANT m_bTrackSel;
		IUI_VARIANT m_bInfoTip;
		IUI_VARIANT m_bToolTips;
		IUI_VARIANT m_bDefaultItemHeight;
		IUI_VARIANT m_nItemHeight;
		IUI_VARIANT m_strTag;

		// Item Line
		IUI_VARIANT m_bEnableItemLineEffect;
		IUI_VARIANT m_crItemLine;
		IUI_VARIANT m_strItemLineColorResID;

		// Collapse and Expand image
		IUI_VARIANT m_strCombineCollapseExpandedImage;
		IUI_VARIANT m_strCollapseImageN;
		IUI_VARIANT m_strCollapseImageH;
		IUI_VARIANT m_strCollapseImageS;
		IUI_VARIANT m_strCollapseImageD;
		IUI_VARIANT m_strExpandedImageN;
		IUI_VARIANT m_strExpandedImageH;
		IUI_VARIANT m_strExpandedImageS;
		IUI_VARIANT m_strExpandedImageD;

		// Text horizontal alignment
		IUI_VARIANT m_eTextHorAlignModeN;
		IUI_VARIANT m_eTextHorAlignModeH;
		IUI_VARIANT m_eTextHorAlignModeS;
		IUI_VARIANT m_eTextHorAlignModeD;

		// Text vertical alignment
		IUI_VARIANT m_eTextVerAlignModeN;
		IUI_VARIANT m_eTextVerAlignModeH;
		IUI_VARIANT m_eTextVerAlignModeS;
		IUI_VARIANT m_eTextVerAlignModeD;

		// Text Margin
		IUI_VARIANT m_rcTextMarginN;
		IUI_VARIANT m_rcTextMarginH;
		IUI_VARIANT m_rcTextMarginS;
		IUI_VARIANT m_rcTextMarginD;

		// Selected Item
		IUI_VARIANT m_eSelectedItemBkType;
		IUI_VARIANT m_crSelectedItemBkColor;
		IUI_VARIANT m_strSelectedItemBkColorResID;
		IUI_VARIANT m_strSelectedItemBkImageN;
		IUI_VARIANT m_strSelectedItemBkImageD;
		IUI_VARIANT m_ptSelectedItemBkResize;

		// Highlight Item
		IUI_VARIANT m_bEnableHighlightItemEffect;
		IUI_VARIANT m_eHighlightItemBkType;
		IUI_VARIANT m_crHighlightItemBkColor;
		IUI_VARIANT m_strHighlightItemBkColorResID;
		IUI_VARIANT m_strHighlightItemBkImageN;
		IUI_VARIANT m_strHighlightItemBkImageD;
		IUI_VARIANT m_ptHighlightItemBkResize;

		// Root Item
		IUI_VARIANT m_bEnableRootItemEffect;
		IUI_VARIANT m_strRootItemTextColorResID;
		IUI_VARIANT m_eRootItemBkType;
		IUI_VARIANT m_strRootItemBKColorResID;
		IUI_VARIANT m_strRootItemBkImageF;
		IUI_VARIANT m_strRootItemBkImageNonF;
		IUI_VARIANT m_ptRootItemBkResize;

		// Custom scroll bar
		IUI_VARIANT m_bUseCustomScrollBar;
		IUI_VARIANT m_lScrollBarWidth;
		IUI_VARIANT m_strHorScrollbarID;
		IUI_VARIANT m_strVerScrollbarID;
	};

	struct SBPROPERTIES : public CTRLPROPERTIES
	{
		SBPROPERTIES();
		virtual ~SBPROPERTIES();

		IUI_VARIANT m_bVertical;
		IUI_VARIANT m_strHorScrollbarStyleID;
		IUI_VARIANT m_strVerScrollbarStyleID;
	};

	struct SBEXPROPERTIES : public SBPROPERTIES
	{
		SBEXPROPERTIES();
		virtual ~SBEXPROPERTIES();
	};

	struct THMPROPERTIES : public CTRLPROPERTIES
	{
		THMPROPERTIES();
		virtual ~THMPROPERTIES();

		virtual bool IsEnableCustomScrollBar() const;

		IUI_VARIANT m_bListStyle;
		IUI_VARIANT m_bHoldCtrlKey;
		IUI_VARIANT m_bDragDrop;

		IUI_VARIANT m_lTotalItemHeight;
		IUI_VARIANT m_lTotalItemWidth;

		IUI_VARIANT m_lThumbnailLeftN;
		IUI_VARIANT m_lThumbnailTopN;
		IUI_VARIANT m_lThumbnailRightN;
		IUI_VARIANT m_lThumbnailBottomN;

		IUI_VARIANT m_lThumbnailLeftH;
		IUI_VARIANT m_lThumbnailTopH;
		IUI_VARIANT m_lThumbnailRightH;
		IUI_VARIANT m_lThumbnailBottomH;

		IUI_VARIANT m_lLabelLeftN;
		IUI_VARIANT m_lLabelTopN;
		IUI_VARIANT m_lLabelRightN;
		IUI_VARIANT m_lLabelBottomN;

		IUI_VARIANT m_lLabelLeftH;
		IUI_VARIANT m_lLabelTopH;
		IUI_VARIANT m_lLabelRightH;
		IUI_VARIANT m_lLabelBottomH;

		IUI_VARIANT m_strItemBkN;
		IUI_VARIANT m_strItemBkH;
		IUI_VARIANT m_strItemBkS;
		IUI_VARIANT m_strItemBkD;

		// Custom scroll bar
		IUI_VARIANT m_bEnableScrollBar;
		IUI_VARIANT m_strVerScrollbarID;
	};

	struct TREELISTPROPERTIES : public CTRLPROPERTIES
	{
		TREELISTPROPERTIES();
		virtual ~TREELISTPROPERTIES();

		IUI_VARIANT m_bVariableItemHeight;
	};

	struct HTMLCTRLPROPERTIES : public CTRLPROPERTIES
	{
		HTMLCTRLPROPERTIES();
		virtual ~HTMLCTRLPROPERTIES();

		IUI_VARIANT m_bHideBorder;
		IUI_VARIANT m_bHideScrollBar;
		IUI_VARIANT m_bHideContextMenu;
		IUI_VARIANT m_strStartPage;
	};

	struct DATETIMECTRLPROPERTIES : public CTRLPROPERTIES
	{
		DATETIMECTRLPROPERTIES();
		virtual ~DATETIMECTRLPROPERTIES();

		IUI_VARIANT m_eFormat;			// DATETIME_FORMAT
		IUI_VARIANT m_bUseSpinControl;	// Must be true while format is time.
		IUI_VARIANT m_strSpinButtonStyleID;
		IUI_VARIANT m_bShowNone;
		IUI_VARIANT m_bAllowEdit;
		IUI_VARIANT m_lButtonWidth;
		IUI_VARIANT m_ptTextOffset;
	};

	struct WAVECTRLPROPERTIES : public CTRLPROPERTIES
	{
		WAVECTRLPROPERTIES();
		virtual ~WAVECTRLPROPERTIES();

		IUI_VARIANT m_bEnableDragMove;
		IUI_VARIANT m_strTag;

		// Grid
		IUI_VARIANT m_lGridWidth;
		IUI_VARIANT m_lGridHeight;
		IUI_VARIANT m_crGridLine;
		IUI_VARIANT m_strGridLineColorResID;
		IUI_VARIANT m_lGridUnitX;
		IUI_VARIANT m_lGridUnitY;

		// Info area
		IUI_VARIANT m_lInfoAreaWidth;
		IUI_VARIANT m_lInfoAreaHeight;

		// Background
		IUI_VARIANT m_crBackground;
		IUI_VARIANT m_strBackgroundColorResID;
	};

	struct MENUPROPERTIES : public CTRLPROPERTIES
	{
		MENUPROPERTIES();
		virtual ~MENUPROPERTIES();

		// Normal item
		IUI_VARIANT m_crNormalItemText;
		IUI_VARIANT m_lfNormalItemText;
		IUI_VARIANT m_eNormalItemBkType;
		IUI_VARIANT m_crNormalItemBkColor;
		IUI_VARIANT m_strNormalItemBkImage;
		IUI_VARIANT m_ptNormalItemBkResize;

		// Highlight item
		IUI_VARIANT m_crHighlightItemText;
		IUI_VARIANT m_lfHighlightItemText;
		IUI_VARIANT m_eHighlightItemBkType;
		IUI_VARIANT m_crHighlightItemBkColor;
		IUI_VARIANT m_strHighlightItemBkImage;
		IUI_VARIANT m_ptHighlightItemBkResize;

		// Disabled item
		IUI_VARIANT m_crDisabledItemText;
		IUI_VARIANT m_lfDisabledItemText;
		IUI_VARIANT m_eDisabledItemBkType;
		IUI_VARIANT m_crDisabledItemBkColor;
		IUI_VARIANT m_strDisabledItemBkImage;
		IUI_VARIANT m_ptDisabledItemBkResize;

		// Separator
		IUI_VARIANT m_eSeparatorBkType;
		IUI_VARIANT m_crSeparator;
		IUI_VARIANT m_nSeparatorLeftMargin;
		IUI_VARIANT m_nSeparatorRightMargin;
		IUI_VARIANT m_strSeparatorBkImage;
		IUI_VARIANT m_ptSeparatorBkImageResize;

		// Menu check image
		IUI_VARIANT m_strMenuCheckImage;

		// Icon area
		IUI_VARIANT m_nIconAreaWidth;
		IUI_VARIANT m_crIconAreaStart;
		IUI_VARIANT m_crIconAreaEnd;
		IUI_VARIANT m_bDrawIconBk;

		// Border
		IUI_VARIANT m_eMenuBorderStyle;
		IUI_VARIANT m_crFlatMenuBorder;

		// Height of Menu item
		IUI_VARIANT m_nMenuItemHeight;
	};

	struct MENUBARPROPERTIES : public CTRLPROPERTIES
	{
		MENUBARPROPERTIES();
		virtual ~MENUBARPROPERTIES();

		IUI_VARIANT m_strBKCN;
		IUI_VARIANT m_strBKCH;
		IUI_VARIANT m_strBKCS;
		IUI_VARIANT m_strBKCD;
		IUI_VARIANT m_strMenuStyleID;
	};

	struct SPLITTERPROPERTIES : public CTRLPROPERTIES
	{
		SPLITTERPROPERTIES();
		virtual ~SPLITTERPROPERTIES();

		IUI_VARIANT m_eOrientation;
	};

	struct STORY_FRAME
	{
	public:
		CTRLPROPERTIES *FindControl(LPCTSTR lpszID);

	public:
		int nFrameIndex;
		std::vector<CTRLPROPERTIES *> m_vStoryFrameCtrlItem;
	};

	struct UIWNDPROPERTIES : public CTRLPROPERTIES
	{
		UIWNDPROPERTIES();
		virtual ~UIWNDPROPERTIES();

		int SwapControlItem(int nFirst, int nLast);
		int GetControlIndex(LPCTSTR lpszCtrlID) const;

		STORY_FRAME *FindStoryFrame(int nFrame);
		int InsertStoryFrame(STORY_FRAME *pItem);	// sort

		// Get control properties at specifies frame.
		// if not at specifies frame in story board, return in m_vControls.
		CTRLPROPERTIES *FindLeftNearControlInStoryBoard(
			LPCTSTR lpszCtrlID, int nCurFrame, BOOL bIncludeCurFrame);
		const CTRLPROPERTIES *FindLeftNearControlInStoryBoard(
			LPCTSTR lpszCtrlID, int nCurFrame, BOOL bIncludeCurFrame) const;

		// Alloc and init
		CTRLPROPERTIES *AllocCtrlPropertiesAtStoryFrame(int nCurFrame, LPCTSTR lpszCtrlID);

		CTRLPROPERTIES *GetDlgItem(LPCTSTR lpszCtrlID);
		const CTRLPROPERTIES *GetDlgItem(LPCTSTR lpszCtrlID) const;
		CTRLPROPERTIES *GetDlgItem(UINT nCtrlID);
		const CTRLPROPERTIES *GetDlgItem(UINT nCtrlID) const;

		IUI_VARIANT m_bLoaded;		// UIWNDPROPERTIES support delay load.
		IUI_VARIANT m_bNoActivate;
		IUI_VARIANT m_bAppWindow;
		IUI_VARIANT m_bLayeredWindow;
		IUI_VARIANT m_bAlpha;
		IUI_VARIANT m_btAlphaValue;
		IUI_VARIANT m_bUpdateLayeredWindow;
		IUI_VARIANT m_eAlphaFormat;	// ALPHA_FORMAT
		IUI_VARIANT m_bTopmost;
		IUI_VARIANT m_bAcceptFiles;
		IUI_VARIANT m_bToolWindow;
		IUI_VARIANT m_lTitleBarHeight;	// -1: All area of window can be drag to move; 0: window can't be moved; >0: the height of title bar.
		IUI_VARIANT m_strTag;

		// Background
		IUI_VARIANT m_bTransparentBk;		// Only for child window
		IUI_VARIANT m_bDrawBKColor;
		IUI_VARIANT m_strBackgoundColorResID;
		IUI_VARIANT m_strBKNo;

		//
		IUI_VARIANT m_bUseMaskColor;
		IUI_VARIANT m_crMask;
		IUI_VARIANT m_strMaskColorResID;

		IUI_VARIANT m_strShadowImage;
		IUI_VARIANT m_ptShadowResize;
		IUI_VARIANT m_rcShadow;
		IUI_VARIANT m_lRoundCornerX;
		IUI_VARIANT m_lRoundCornerY;

		//
		IUI_VARIANT m_bResizingBorder;
		IUI_VARIANT m_bKeepChildrenAspectRatio;
		IUI_VARIANT m_lMaxHorRatio;	// Default to 100, means 100%
		IUI_VARIANT m_lMaxVerRatio;	// Default to 100, means 100%
		IUI_VARIANT m_bWindowResizePointSameAsImageResizePoint;
		IUI_VARIANT m_ptWindowResize;			// For resize window

		//
		IUI_VARIANT m_eWindowType;
		IUI_VARIANT m_rcClientArea;	// can auto resize by windows resizing relative to m_ptWindowResize.

		std::vector<CTRLPROPERTIES *> m_vControls;
		std::vector<STORY_FRAME *> m_vStoryFrame;		// 每个Item之间可能并不连续。 例如第2个Item可能是第10帧。
	};

	//////////////////////////////////////////////////////////////////////////
	// Resource and Style properties

	struct RESFONTPROPERTIES : public CTRLPROPERTIES
	{
		RESFONTPROPERTIES();
		virtual ~RESFONTPROPERTIES();

		virtual bool IsFontResource() const;

		IUI_VARIANT m_logFont;
	};

	struct RESCOLORPROPERTIES : public CTRLPROPERTIES
	{
		RESCOLORPROPERTIES();
		virtual ~RESCOLORPROPERTIES();

		virtual bool IsColorResource() const;

		IUI_VARIANT m_crColor;
	};

	struct HEADERPROPERTIES : public CTRLPROPERTIES
	{
		HEADERPROPERTIES();
		virtual ~HEADERPROPERTIES();

		// Header Background，CTRLPROPERTIES的四张状态图变量，用于Header button image
		IUI_VARIANT m_strHeaderBk;
		IUI_VARIANT m_ptHeaderBkImageResize;

		IUI_VARIANT m_bHeaderTrack;
		IUI_VARIANT m_bHeaderDragDrop;
	};

	struct FONTRESOURCEITEM
	{
	public:
		FONTRESOURCEITEM();
		virtual ~FONTRESOURCEITEM();

	public:
		int Release();

		int ConvertGeneralFontToConditionFont(LPCTSTR lpszConditionName);

		RESFONTPROPERTIES *GetConditionFont(const CString &strCondition);
		int DeleteConditionFontChild(const RESFONTPROPERTIES *presFontDelete);

	public:
		// if m_bConditionFont is true, One font ID macro can show different font on different condition
		// Example: the IDC_FONT_BUTTON is a condition font. on english system, is may be use tahoma font,
		// and "Simsun" on chinese system.
		bool m_bConditionFont;

		// while m_bConditonFont is false
		RESFONTPROPERTIES *m_pFontResProp;

		// while m_bConditonFont is true
		CString m_strFontID;
		std::map<CString, RESFONTPROPERTIES *> m_mapConditionFont;	// map<Condition name, RESFONTPROPERTIES *>
	};

	//////////////////////////////////////////////////////////////////////////
	//

	struct MENU_ITEM_PROPERTIES
	{
		MENU_ITEM_PROPERTIES()
		{
			// Text
			m_crItemText = RGB(0, 0, 0);
			memset(&m_lfItem, 0, sizeof(LOGFONT));

			// Background
			m_eBackgroundType = BKT_COLOR;
			m_crItemBk = RGB(249, 248, 247);
			m_hItemBk = NULL;
			m_ptResize.x = m_ptResize.y = 2;
		}

		virtual ~MENU_ITEM_PROPERTIES()
		{
			// the m_pfItem are shared, don't release theme.
			if (m_hItemBk != NULL)
			{
				ImageManager::ReleaseIUIImage(m_hItemBk);
				m_hItemBk = NULL;
			}
		}

		// Text
		COLORREF m_crItemText;
		LOGFONT m_lfItem;

		// Background
		BACKGROUND_TYPE m_eBackgroundType;
		COLORREF m_crItemBk;
		HIUIIMAGE m_hItemBk;
		POINT m_ptResize;
	};

	struct MENUMEMBER
	{
		MENUMEMBER()
		{
			// Border
			m_eMbs = MBS_FLAT;
			m_crFlatMenuBorder = RGB(102, 102, 102);

			// Height of Menu Item
			m_nItemHeight = 23;

			// Highlight menu item
			m_HighlightItemProp.m_crItemText = RGB(255, 255, 255);
			m_HighlightItemProp.m_crItemBk = RGB(10, 36, 106);

			// Disabled menu item
			m_DisabledItemProp.m_crItemText = RGB(166, 166, 166);

			m_eSeparatorBkType = BKT_COLOR;
			m_crSeparator = RGB(166, 166, 166);
			m_nSeparatorLeftMargin = 2;
			m_nSeparatorRightMargin = 2;
			m_hSeparator = NULL;
			m_ptSeparatorResize.x = m_ptSeparatorResize.y = 2;

			m_himgMenuCheck = NULL;

			// Side bar
			m_crSideBarStart = RGB(0, 0, 192);
			m_crSideBarEnd = RGB(0, 0, 0);

			// Icon area
			m_nIconAreaWidth = 22;
			m_crIconAreaStart = RGB(249, 248, 247);
			m_crIconAreaEnd = RGB(215, 212, 204);
			m_bDrawIconBk = FALSE;

			//
			m_bBreak = false;
			m_bBreakBar = false;
		}

		virtual ~MENUMEMBER()
		{
			if (m_hSeparator != NULL)
			{
				ImageManager::ReleaseIUIImage(m_hSeparator);
				m_hSeparator = NULL;
			}

			if (m_himgMenuCheck != NULL)
			{
				ImageManager::ReleaseIUIImage(m_himgMenuCheck);
				m_himgMenuCheck = NULL;
			}
		}

		// Border
		MENU_BORDER_STYLE m_eMbs;
		COLORREF m_crFlatMenuBorder;	// Only valid while m_eMbs is MBS_FLAT.

		// Height of menu item
		int m_nItemHeight;

		// Menu item properties
		MENU_ITEM_PROPERTIES m_NormalItemProp;
		MENU_ITEM_PROPERTIES m_HighlightItemProp;
		MENU_ITEM_PROPERTIES m_DisabledItemProp;

		// Separator line properties
		BACKGROUND_TYPE m_eSeparatorBkType;
		COLORREF m_crSeparator;
		int m_nSeparatorLeftMargin;
		int m_nSeparatorRightMargin;
		HIUIIMAGE m_hSeparator;
		POINT m_ptSeparatorResize;

		// Menu check image
		HIUIIMAGE m_himgMenuCheck;

		// Draw the left sidebar, like 'Start menu'.
		COLORREF m_crSideBarStart;	// Start color of the gradient sidebar bottom side
		COLORREF m_crSideBarEnd;	// end color of the gradient sidebar top side

		// Icon area
		int m_nIconAreaWidth;
		COLORREF m_crIconAreaStart;		// Background color of the button(icon) area left side
		COLORREF m_crIconAreaEnd;		// Background color of the button(icon) area right side
		BOOL m_bDrawIconBk;				// Draw icon button

		BOOL m_bBreak;		//if true, next item inserted into the menu will be added with the style MF_MENUBREAK
		BOOL m_bBreakBar;	//if true, next item inserted into the menu will be added with the style MF_MENUBARBREAK
	};

	//////////////////////////////////////////////////////////////////////////

	struct _IUI_THREAD_STATE
	{
		HWND m_hTrackingWindow;         // parent of the menu
		HMENU m_hTrackingMenu;			// HANDLE of the menu
		HWND m_hWndMenu;				// window handle of the menu

		CWLWnd *m_pWLCapture;			// current windowless window that get the capture.
		CWLWnd *m_pWLMouseIn;			// current windowless window that mouse in.

		std::map<UINT, std::vector<CUIWndHandler *> > m_mapUIWndHandler;	// first: the ID of CUIWnd; second: the CUIWndHandler list that bind the ID.
	};

	struct SLPROGRESS_ANIMATION
	{
	public:
		SLPROGRESS_ANIMATION()
		{
			crBk = -1;

			bDrawThumb = TRUE;
			crThumbStart = -1;
			crThumbEnd = -1;

			bDrawThumbMask = FALSE;
			crThumbMaskStart = RGB(255, 255, 255);
			crThumbMaskEnd = RGB(255, 255, 255);
			nThumbMaskTransparent = 100;
		}

	public:
		int nPos;

		COLORREF crBk;

		BOOL bDrawThumb;
		COLORREF crThumbStart;
		COLORREF crThumbEnd;

		BOOL bDrawThumbMask;
		COLORREF crThumbMaskStart;
		COLORREF crThumbMaskEnd;
		int nThumbMaskTransparent;
	};

	struct CPROGRESS_ANIMATION
	{
	public:
		CPROGRESS_ANIMATION()
		{
			crBk = -1;
			crThumb = -1;
		}

	public:
		int nPos;

		COLORREF crBk;
		COLORREF crThumb;
	};

	struct DELETE_TREE_ITEM_ANIMATION
	{
	public:
		DELETE_TREE_ITEM_ANIMATION()
		{
			nTransparent = 255;
			nPercent = 100;
		}

	public:
		int nTransparent;
		int nPercent;
	};

#define CMHF_TASKWNDMGR					0x0001	// dwChildMsgHandlerParam is CTaskWndInfo *
	struct CHILDMSGHANDLER
	{
	public:
		CHILDMSGHANDLER()
		{
			pUIWndHandler = NULL;
			pUIWnd = NULL;
			uChildMsgHandlerParamFlag = 0;
			dwChildMsgHandlerParam = 0;
		}

	public:
		CUIWndHandler *pUIWndHandler;	// [in, out]
		CUIWnd *pUIWnd;					// [in]
		UINT uChildMsgHandlerParamFlag;
		DWORD_PTR dwChildMsgHandlerParam;
	};

	class CChildInfo
	{
	public:
		CChildInfo()
		{
			m_pChildCtrl = NULL;
			m_rcOriginal.SetRect(0, 0, 0, 0);
			m_eControlType = CT_ARROW;
			m_pCtrlProperties = NULL;
		}

	public:
		CWnd *m_pChildCtrl;
		CRect m_rcOriginal;
		CONTROL_TYPE m_eControlType;
		CTRLPROPERTIES *m_pCtrlProperties;
	};

	struct SItemBindObject
	{
		SItemBindObject()
		{
			nImage = -1;
			bBindWLWndNeedRelease = TRUE;
			bAlreadyCreateChildren = FALSE;
			uBindWndID = 0;
			pBindUIWndProp = NULL;
			lParam = 0;
		}

		int ReleaseChildren(BOOL bDeleteMemory);

		int nImage;							// 0-base image index in image list.
		BOOL bBindWLWndNeedRelease;			// the CWLWnd pointer in vSubWLWnd need free while exit.
		BOOL bAlreadyCreateChildren;		// TRUE: already create child windowless control from uBindWndID to vSubWLWnd
		UINT uBindWndID;					// bind window ID in UIShop.exe.
		UIWNDPROPERTIES *pBindUIWndProp;	// Load from uBindWndID
		std::vector<CChildInfo> vSubWLWnd;	// windowless control create by uBindWndID template.
		CRect rcLastParentLayout;			// The old position of the (sub)item, if the size not equal to the item now, the children of the item need be relayouted.
		LPARAM lParam;
	};

	//////////////////////////////////////////////////////////////////////////
	// Globals

	LRESULT Ctrl_OnWindowPosChanged(CWnd *pCtrl, WPARAM wParam, LPARAM lParam);
	BOOL PASCAL CheckForDragBegin(HWND hwnd, int x, int y);


	// For window
	BOOL IsParent(HWND hChild, HWND hParent);
	BOOL IsChildWindow(HWND hParent, HWND hChild);
	BOOL IsWindowVisibleEx(HWND hWnd);	// If the specified window have the WS_VISIBLE style, the return value is nonzero. Otherwise, the return value is zero.

#if(_MSC_VER <= 1200) // VC6.0
	BOOL UpdateLayeredWindow(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
#endif

	int SetLayeredWindow(HWND hWnd, BOOL bSet);
	BOOL IsLayeredWindow(HWND hWnd);

	int IUIInvariantStrICmp(const char *pszLeft, const char *pszRight);
	int IUIInvariantStrICmp(const wchar_t *pwszLeft, const wchar_t *pwszRight);
	BOOL IUICompareClassName(HWND hWnd, LPCTSTR lpszClassName);


	// for DPI
	int GetDPIX();
	int GetDPIY();
	int GetDPIWidth(int nWidth);
	int GetDPIHeight(int nHeight);
	int GetDPIRect(LPRECT lpRect);
	int GetDPIPadding(LPRECT lprcPadding);
	int GetDPIPoint(LPPOINT lpPt);
	int GetDPIPointX(long &nX);
	int GetDPIPointY(long &nY);
	int GetDPILogFont(LPLOGFONT lpLogFont);
	int GetDPILogFont(IUI_VARIANT *pvarLf);


	//////////////////////////////////////////////////////////////////////////
	// GDI specific

	int BackgroundMask(CWnd *pWnd, CString szMaskBitmap, COLORREF crMask);
	int BitAlphaBlendColor(HDC hdcDest, LPCRECT lpRect, COLORREF crAlphaBlend);

	HBITMAP CreateGradientBitmap(HDC hDC, COLORREF crStart, COLORREF crEnd,
		int nWidth, int nHeight, int nDir, int nNumColors);

	//////////////////////////////////////////////////////////////////////////
	//
	int BackgroundMaskG(CWnd *pWnd, HBITMAP hBmpMask, COLORREF crMask);

	int DrawBackgroundG(CWnd *pCtrl, CDC *pDstDC, HBITMAP hbmp,
		IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY);
	int DrawNCBackgroundG(CWnd *pCtrl, HBITMAP hbmpResized, BOOL bDrawResizeScrollBar,
		HBITMAP hbmpResizeScrollBar, LPCRECT lprcBorderWidth);
	int DrawNCBackgroundG(CWnd *pCtrl, HBITMAP hbmp,
		IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
		LPCRECT lprcBorderWidth);

	int GetDPIBitmapG(HBITMAP hSrcBmp, HBITMAP *phDstBmp);
	HBITMAP GetDPIBitmapG(HBITMAP hSrcBmp, BOOL bDeleteSrc/* = TRUE*/);

	HBRUSH CreatePatternBrushG(HBITMAP hImage, COLORREF crBackground/* = RGB(255, 255, 255)*/);

	void ASSERT_HBITMAP(HBITMAP obj);
	int SafeDeleteHBITMAP(HBITMAP &hBmp);

	//////////////////////////////////////////////////////////////////////////
	// Default Draw control

	int DrawDefaultPictureCtrl(HDC hDC, LPCRECT lpcRect);

	//////////////////////////////////////////////////////////////////////////
	// Image operate

	BOOL IsImageHasAlphaChanne(HIUIIMAGE hImage);

	//////////////////////////////////////////////////////////////////////////
	// Render

	namespace Render
	{
		//
		// HDC 操作
		//
		void TransparentBlt2(HDC hdcDest,
			int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
			HDC hdcSrc,
			int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight, UINT crTransparent);

		int TileBlt(HDC hdcDest,
			int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
			HDC hdcSrc,
			int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight, BYTE btSourceConstantAlpha/* = 255*/);
		int AlphaTileBlt(HDC hdcDest,
			int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
			HDC hdcSrc,
			int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight, BYTE btSourceConstantAlpha/* = 255*/);

		int StretchAlphaBlend(HDC hdcDest,
			int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
			HDC hdcSrc,
			int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight,
			BYTE btSourceConstantAlpha);

		//
		// 文本、直线
		//
		int DrawControlText(HDC hDstDC,
			LPCRECT lprcControl, LPCRECT lprcTextMargin,
			LPCTSTR lpszText, TEXT_ALIGN_HOR eTextAlignHor, TEXT_ALIGN_VER eTextAlignVer,
			BOOL bMultiLine, BOOL bEndEllipsis, COLORREF crText, HFONT hFont);
		//////////////////////////////////////////////////////////////////////////
		// Draw format text
		int DrawFormatText(
			HDC hdcDest,
			// Text
			LPCTSTR lpszText,
			int nTextLength,
			// Target
			LPCRECT lprcDest,
			LPCRECT lprcPadding4Text,
			TEXT_ALIGN_HOR eTextHorAlign,
			TEXT_ALIGN_VER eTextVerAlign,
			UINT uTextFormat,
			// Text font and color property
			HFONT hTextFont,
			COLORREF crText,
			// The default text align is diff for dirr control.
			CONTROL_TYPE eControlType);
		// format: see ::DrawText
		int DrawTextGp(
			Graphics *pGraphicsDst,
			LPCTSTR lpszString,
			int nLength,
			HFONT hFont,
			COLORREF crText,
			LPRECT lprc,
			UINT format);
		int DrawLine(HDC hDstDC, COLORREF crLine, int nLineWidht, POINT ptBegin, POINT ptEnd);
		int DrawLine(HDC hDstDC, COLORREF crLine, int nLineWidht, int nX1, int nY1, int nX2, int nY2);

		//
		// HBITMAP 绘制
		//
		void TransparentBltHBITMAP(HDC hdcDest,
			int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
			HBITMAP hBmpSrc,
			int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight, UINT crTransparent);

		int NineGridBltG(HDC hdcDest,
			int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY);
		int NineGridBltG(HDC hdcDest, LPCRECT lprcDest,
			HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY);

		int PartNineGridBltG(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			int nPart, int nPartIndex);
		int PartNineGridBltG(HDC hdcDest, LPCRECT lprcDest,
			HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			int nPart, int nPartIndex);
		int PartNineGridBltG(HDC hdcDest, LPCRECT lprcDest,
			HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			LPCRECT lprcSrc);

		int StretchBltG(HDC hdcDest, LPCRECT lprcDest, HBITMAP hbmpSrc);
		int BitBltG(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HBITMAP hSrcBmp, int nXSrc, int nYSrc, DWORD dwRop);
		int BitBltG(HDC hdcDest, LPCRECT lprcDest, HBITMAP hSrcBmp, int nXSrc, int nYSrc, DWORD dwRop);

		int CenterBitBltG(HDC hDC, LPCRECT lprcDest, HBITMAP hBmp, DWORD dwRop);

		// For windowless control, pDstDC is parent DC, rcClient is control coordinate relate to parent.
		int DrawForegroundG(HDC hDstDC, LPCRECT lprcClient, LPCRECT lprcForegroundMargin,
			HBITMAP hbmpFg,
			FOREGROUND_ALIGN_HOR eForegroundAlignHor, FOREGROUND_ALIGN_VER eForegroundAlignVer,
			COLORREF crMask);
		int DrawImageG(HDC hDstDC, LPCRECT lprcImage, HBITMAP hImage,
			IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY);

		//
		// HIUIIMAGE 绘制
		//
		int IUINineGridBlt(HDC hdcDest,
			int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY);
		int IUINineGridBlt(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY);

		int IUINineGridBltEx(HDC hdcDest,
			int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			COLORREF crColorKey, BYTE btSourceConstantAlpha);
		int IUINineGridBltEx(HDC hdcDest,
			LPCRECT lprcDest,
			HIUIIMAGE hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			COLORREF crColorKey, BYTE btSourceConstantAlpha);

		int IUIPartNineGridBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			int nPart, int nPartIndex);
		int IUIPartNineGridBlt(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			int nPart, int nPartIndex);
		int IUIPartNineGridBlt(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			LPCRECT lprcSrc);
		int IUIPartNineGridBltResetResizePoint(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			LPCRECT lprcSrc);

		int IUIPartNineGridBltEx(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			int nPart, int nPartIndex,
			COLORREF crColorKey, BYTE btSourceConstantAlpha, BOOL bResetResizePoint);
		int IUIPartNineGridBltEx(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			int nPart, int nPartIndex,
			COLORREF crColorKey, BYTE btSourceConstantAlpha, BOOL bResetResizePoint);
		int IUIPartNineGridBltEx(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			LPCRECT lprcSrc,
			COLORREF crColorKey, BYTE btSourceConstantAlpha, BOOL bResetResizePoint);

		// 缩放贴图
		int IUIStretchBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE himgSrc, int nXSrc, int nYSrc, int nSrcWidth, int nSrcHeight);
		int IUIStretchBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE himgSrc);
		int IUIStretchBlt(HDC hdcDest, LPCRECT lprcDest, HIUIIMAGE himgSrc);

		// while himgSrc has alpha channel, and btSourceConstantAlpha is 255, IUIAlphaStretchBlt and IUIStretchBlt are same.
		int IUIAlphaStretchBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE himgSrc, BYTE btSourceConstantAlpha, IMAGE_RESIZE_MODE eStretchQuality);
		int IUIAlphaStretchBlt(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE himgSrc, BYTE btSourceConstantAlpha, IMAGE_RESIZE_MODE eStretchQuality);

		int IUIPartStretchBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE himgSrc, int nXSrc, int nYSrc, int nSrcWidth, int nSrcHeight,
			IMAGE_RESIZE_MODE eStretchQuality);
		int IUIPartStretchBlt(HDC hdcDest, LPCRECT lprcDest, HIUIIMAGE himgSrc, LPCRECT lprcSrc,
			IMAGE_RESIZE_MODE eStretchQuality);
		int IUIPartStretchBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE himgSrc, int nPart, int nPartIndex, IMAGE_RESIZE_MODE eStretchQuality);
		int IUIPartStretchBlt(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE himgSrc, int nPart, int nPartIndex, IMAGE_RESIZE_MODE eStretchQuality);

		// 平铺贴图
		int IUITileBlt(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE himgSrc, int nXOriginSrc/* = 0*/, int nYOriginSrc/* = 0*/,
			int nSrcWidth/* = -1*/, int nSrcHeight/* = -1*/);

		// 居中贴图
		int IUICenterBitBlt(HDC hDC, LPCRECT lprcDest, HIUIIMAGE himgSrc, DWORD dwRop);

		// 1：1贴图
		int IUIBitBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
			HIUIIMAGE hSrcBmp, int nXSrc, int nYSrc, DWORD dwRop);
		int IUIBitBlt(HDC hdcDest, LPCRECT lprcDest,
			HIUIIMAGE hSrcBmp, int nXSrc, int nYSrc, DWORD dwRop);

		// Same as IUITileBlt, but with a clip area. a board of the clip
		// area is smoothing. If lprcSrc, use the entire image to tile.
		// 可用来制作圆形抗锯齿头像
		int IUITileClipImage(Graphics *pGraphics, GraphicsPath *pDestClipPath, Image *pSrcImage, LPCRECT lprcSrc);

		// 下面是由上面的基础引擎，封装的简易调用
		// For windowless control, pDstDC is parent DC, rcClient is control coordinate relate to parent.
		int IUIDrawForeground(HDC hDstDC, LPCRECT lprcClient, LPCRECT lprcForegroundMargin,
			HIUIIMAGE himgFg,
			FOREGROUND_ALIGN_HOR eForegroundAlignHor, FOREGROUND_ALIGN_VER eForegroundAlignVer,
			COLORREF crMask, BYTE btSourceConstantAlpha/* = 255*/);

		// Draw himgFg with rcFgPart part to DC.
		int IUIPartDrawForeground(HDC hDstDC, LPCRECT lprcDest, LPCRECT lprcForegroundMargin,
			HIUIIMAGE himgFg, LPCRECT lprcFgPart,
			FOREGROUND_ALIGN_HOR eForegroundAlignHor, FOREGROUND_ALIGN_VER eForegroundAlignVer,
			COLORREF crMask, BYTE btSourceConstantAlpha/* = 255*/);

		int IUIDrawImage(HDC hDstDC, LPCRECT lprcDest, HIUIIMAGE hImage,
			IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY);
		int IUIPartDrawImage(HDC hDstDC, LPCRECT lprcDest, HIUIIMAGE hImage,
			IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			LPCRECT lprcSrc);
		int IUIPartDrawImage(HDC hDstDC, LPCRECT lprcDest, HIUIIMAGE hImage,
			IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
			int nPartCount, int nPartIndex);
	} // namespace Render

	int IUIBackgroundMask(CWnd *pWnd, HIUIIMAGE himgMask, COLORREF crMask);
	int IUIDrawBackground(CWnd *pCtrl, CDC *pDstDC,	HIUIIMAGE hbmp,
		IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY);
	int IUIDrawNCBackground(CWnd *pCtrl, HIUIIMAGE himgResized,
		BOOL bDrawResizeScrollBar, HIUIIMAGE himgResizeScrollBar, LPCRECT lprcBorderWidth);
	int IUIDrawNCBackground(CWnd *pCtrl, HIUIIMAGE hbmp,
		IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
		LPCRECT lprcBorderWidth);
	int IUIDrawCombineNCBackground(CWnd *pCtrl, HIUIIMAGE himgCombine,
		IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
		int nPart, int nPartIndex, LPCRECT lprcBorderWidth);

	//////////////////////////////////////////////////////////////////////////
	BOOL IUIIsWindowsVersion(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor, BYTE dwConditionMask);
	BOOL IUIIsWindowsVersionEqual(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor);
	BOOL IUIIsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor);
	BOOL IUIIsWindowsVersionOrLess(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor);
	BOOL IUIIsWindows2000();
	BOOL IUIIsWindowsXPOrGreater();
	BOOL IUIIsWindowsXPSP1OrGreater();
	BOOL IUIIsWindowsXPSP2OrGreater();
	BOOL IUIIsWindowsXPSP3OrGreater();
	BOOL IUIIsWindowsVistaOrGreater();
	BOOL IUIIsWindowsVistaSP1OrGreater();
	BOOL IUIIsWindowsVistaSP2OrGreater();
	BOOL IUIIsWindows7OrGreater();
	BOOL IUIIsWindows7SP1OrGreater();
	BOOL IUIIsWindows8OrGreater();
	BOOL IUIIsWindows8Point1OrGreater();
	BOOL IUIIsWindowsServer();

	int RemoveAeroTheme(HWND hWnd);
	BOOL IsAeroTheme();

	//////////////////////////////////////////////////////////////////////////
	// Image and resource

	int LoadCustomResourceA(LPCSTR lpszResName, LPCSTR lpType, LPVOID *lpData, DWORD *pdwSize);

	void ASSERT_CIUIImage(const CIUIImage &obj);
	void SafeDeleteHGDIOBJ(HGDIOBJ &obj);
	template <class T>
	void SafeDelete(T *&p)
	{
		if (p != NULL)
		{
			delete p;
			p = NULL;
		}
	}

	template <class T>
	void SafeArrayDelete(T *&p)
	{
		if (p != NULL)
		{
			delete []p;
			p = NULL;
		}
	}


#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	typedef int errno_t;
#endif
	int TSPLITPATH(LPCTSTR path, LPTSTR drive, size_t driveSizeInCharacters, LPTSTR dir, size_t dirSizeInCharacters, LPTSTR fname, size_t nameSizeInCharacters, LPTSTR ext, size_t extSizeInBytes);
	errno_t TSTRCPY(LPTSTR strDestination, size_t numberOfElements, LPCTSTR strSource);	// Or using StrCpy
	errno_t TSTRCAT(LPTSTR strDestination, size_t numberOfElements, LPCTSTR strSource);	// Or using StrCat
	size_t TSTRLEN(LPCTSTR lpszBuf);


	LPSTR nstrchr(LPCSTR pbyte, int ncb, int c);
	LPWSTR wnstrchr(LPCWSTR pbyte, int ncb, int c);

	int FormatString(CString &str);		// Remove more space, tab keys, split string by only one space key.
	int RemoveQuotationMarks(CString &str, BOOL bDoubleQuotationMarks);	// Remove first and last quotation marks. bDoubleQuotationMarks: TRUE, remove double quotation marks; FALSE, remove single quotation marks.

	BOOL AfxExtractSubStringA(std::string &rstring, LPCSTR lpszFullString, int nFullStringCount, int iSubString, CHAR chSep);
	BOOL AfxExtractSubStringW(std::wstring &rstring, LPCWSTR lpszFullString, int nFullStringCount, int iSubString, WCHAR chSep);

	std::wstring MultiByteToUTF16(LPCSTR lpMultiByteStr, int cbMultiByte, UINT uCodePage);	// UTF16 means Unicode or wide char.

	// for file
	FILE *TFOPEN(LPCTSTR filename, LPCTSTR mode);
	errno_t FOPENW(FILE **pFile, LPCWSTR filename, LPCWSTR mode);

	CString GetFileDrive(const CString &strFile);	// return drive name.
	CString GetFilePath(const CString &strFile);	// return only path, not include title and ext.
	CString GetFileName(const CString &strFile);	// return title and ext.

	std::string GetFileTitleExA(LPCSTR lpszFile);	// return only title, not include ext.
	std::wstring GetFileTitleExW(LPCWSTR lpszFile);	// return only title, not include ext.
	CString GetFileTitleEx(const CString &strFile);	// return only title, not include ext.

	// 可以由PathFindExtension代替，由PathFindExtension得到的扩展名，带前缀点。
	std::string GetFileExtA(LPCSTR lpszFile, BOOL bHasDot/* = TRUE*/);		// return ext.
	std::wstring GetFileExtW(LPCWSTR lpszFile, BOOL bHasDot/* = TRUE*/);		// return ext.
	CString GetFileExt(LPCTSTR lpszFile, BOOL bHasDot/* = TRUE*/);		// return ext.

	int SplitPath(const CString &strFile, CString &strPath, CString &strTitle, CString &strExt);

	int CheckFileWritable(const CString &strFile, BOOL bWarning/* = TRUE*/);
	int CreateFileFolder(const CString &strFullPath);
	CString GetExecutePath();
	BOOL PathRemoveFileSpecEx(CString &rstrPath, BOOL bHasEndBackslash);
	BOOL OpenForEditInPerforce(const CString &strFullPath);

	// for data
	CString StringFromInt(int nValue);
	CString StringFromUINT(UINT n);

	CString HexStringFromInt(int n, BOOL bHasPrefix, BOOL b0Fill, int nBits/* = 8*/);
	CString HexStringFromUINT(UINT n, BOOL bHasPrefix, BOOL b0Fill, int nBits/* = 8*/);

	// IntFromString returns the converted value, if any, or ULONG_MAX on overflow
	unsigned long UINTFromString(LPCTSTR lpString);
	unsigned long UINTFromHexString(LPCTSTR lpString);

	// IntFromString returns the value represented in the string strValue,
	// except when the representation would cause an overflow,
	// in which case it returns LONG_MAX or LONG_MIN.
	// IntFromString returns 0 if no conversion can be performed
	int IntFromString(LPCTSTR lpString);
	int IntFromHexString(LPCTSTR lpString);

	long LongFromString(LPCTSTR lpString);

	bool boolFromString(LPCTSTR lpString);

	CString StringFromVariant(const _variant_t &rVariant);
	int IntFromVariant(const _variant_t &rVariant);
	_variant_t VariantFromInt(int nValue);

	BOOL IsIntegerValue(const CString &strValue);

	void IUIScreenToClient(HWND hWnd, LPRECT lpRect);
	int GetRectWidth(LPCRECT lpRect);
	int GetRectHeight(LPCRECT lpRect);

	// 1 twips equal to 1/1440 inch.
	int GetXTwipsFromPixel(int nPixel);
	int GetYTwipsFromPixel(int nPixel);

	int GetXPixelFromTwips(int nTwips);
	int GetYPixelFromTwips(int nTwips);

	// 1 FPPTS equal to 1/20 twips
	float GetXFPPTSFromPixel(int nPixel);
	float GetYFPPTSFromPixel(int nPixel);

	// 1 himetric equal to 1/100 mm.
	int GetXHimetricFromPixel(int nPixel);
	int GetYHimetricFromPixel(int nPixel);

	// _variant_t or VARIANT or double time to CTime
	CTime VARIANT2CTime(_variant_t var);

	// Check GUID
	bool IsGUIDZero(const GUID &guid);

	// Big-endian, Little-endian
	int SwitchWordEndian(const WORD *pszStream, WORD *pszTarget);

	// Get the hex view code, such as "ABCabc123" translate to "414243616263313233". the return hex code must be ANSI.
	std::string GetHexCodeA(LPCSTR lpszSource);
	std::string GetHexCodeW(LPCWSTR lpszSource);
	std::string GetHexCode(LPCTSTR lpszSource);

	BOOL IsStringExist(const std::vector<CString> *pvStrings, const CString &rString, BOOL bNoCase);

	BOOL IsIncludeFlag(UINT_PTR uFlags, UINT_PTR uFlag);

	// For LibUIDK
	CString GetControlTypeString(CONTROL_TYPE eControlType);
	CONTROL_TYPE GetControlTypeEnum(LPCTSTR lpszControlType);

	int CreateControlFont(__out HFONT *phFont, __in FONTRESOURCEITEM *pResFontAs, const LOGFONT *plfDefault, const CString &strCurrentFont);
	HFONT CreateControlFont(__in FONTRESOURCEITEM *pResFontAs, const LOGFONT *plfDefault, const CString &strCurrentFont);

	//////////////////////////////////////////////////////////////////////////
	// Get control image name from CTRLPROPERTIES

	int IUIGetBackgroundFromParent2(const CTRLPROPERTIES *pCtrlProp, const CRect &rcCtrl,
		HIUIIMAGE hParentBKN, HIUIIMAGE hParentBKD,
		__out HIUIIMAGE *phCtrlBKN, __out HIUIIMAGE *phCtrlBKD);

	//
	// Set control image

	namespace ControlResMgr
	{
		int IUISetControlImage2(
			UINT uMask,
			__inout HIUIIMAGE *phIUIImage,
			LPCTSTR lpszImageNameN,
			LPCTSTR lpszImageNameD);

		int IUISetControlImage4(
			UINT uMask,
			__inout HIUIIMAGE *phIUIImage,
			LPCTSTR lpszImageNameN,
			LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS,
			LPCTSTR lpszImageNameD);

		int IUISetControlCheckedImage4(
			UINT uMask,
			__inout HIUIIMAGE *phIUIImage,
			LPCTSTR lpszImageNameCN,
			LPCTSTR lpszImageNameCH,
			LPCTSTR lpszImageNameCS,
			LPCTSTR lpszImageNameCD);

		int IUISetControlForeground2(
			UINT uMask,
			__inout HIUIIMAGE *phIUIImage,
			LPCTSTR lpszImageNameN,
			LPCTSTR lpszImageNameD);

		// Get control image
		int IUIGetControlImage(HIUIIMAGE hIUIImage, CString *pstrImageName);

		int IUIGetControlImage2(
			UINT uMask,
			HIUIIMAGE hIUICombineImage,
			HIUIIMAGE *phIUIImage,
			BOOL bCombineImage,
			CString *pstrImageNameCombine,
			CString *pstrImageNameN,
			CString *pstrImageNameD);

		int IUIGetControlImage4(
			UINT uMask,
			HIUIIMAGE hIUICombineImage,
			HIUIIMAGE *phIUIImage,
			BOOL bCombineImage,
			CString *pstrImageNameCombine,
			CString *pstrImageNameN,
			CString *pstrImageNameH,
			CString *pstrImageNameS,
			CString *pstrImageNameD);
	} // namespace ControlResMgr

	//
	DWORD GetControlBaseStyle(CTRLPROPERTIES *pCtrlProp);
	LONG_PTR IUISetWindowLong(HWND hWnd, int nIndex, LONG_PTR dwNewLong);
	LONG_PTR IUIGetWindowLong(HWND hWnd, int nIndex);

	int RelayoutElement(const UIWNDPROPERTIES *pUIWndProp,
		const CRect &rcChildOriginal,
		const CPoint &ptResize,
		/*out*/ CRect &rcNew,
		int nWindowWidthLimitBkImage, int nWindowHeightLimitBkImage,
		int nWindowWidth, int nWindowHeight,
		CONTROL_LAYOUT_HOR eHorLayoutMode, CONTROL_LAYOUT_VER eVerLayoutMode,
		LPCRECT lprcLayoutMargin,
		BOOL bKeepAspectRatio,
		LONG lMaxHorRatio,
		LONG lMaxVerRatio);

	// Relayout the children while the parent resizing.
	// nX: max(the current window's width, background image width)
	int RelayoutChildren(CWnd *pParent, const UIWNDPROPERTIES *pUIWnd,
		int nX, int nY, int nCurWndWidth, int nCurWndHeight,
		const std::vector<CChildInfo> *pvChildWndList,
		BOOL bKeepAspectRatio,
		LONG lMaxHorRatio,
		LONG lMaxVerRatio,
		POINT ptOffset,
		BOOL bChildInTemplate);

	BOOL CreateChildControls(const UIWNDPROPERTIES *pUIWndProp, CWnd *pParent,
		HIUIIMAGE himgBkN, HIUIIMAGE himgBkH, HIUIIMAGE himgBkS, HIUIIMAGE himgBkD,
		std::vector<CChildInfo> *pvChildWndList);

	int SyncHostWindow(CWnd *pWnd, UINT uFlags, LPCRECT lprcMargin, LPSIZE lpsizeRoundRect);

	// For combo box
	int SelectComboBoxString(CComboBox *pBox, DWORD dwItemData);
	int SelectComboBoxString(CComboBox *pBox, const CString &str);
	int SelectComboBoxStringNoCase(CComboBox *pBox, const CString &str);

	// For control text align
	UINT GetDrawTextFormat(TEXT_ALIGN_HOR eHorAlign, CONTROL_TYPE eControlType);
	UINT GetDrawTextFormat(TEXT_ALIGN_VER eVerAlign, CONTROL_TYPE eControlType);

	// For tree control
	HTREEITEM FindTreeItemByData(CTreeCtrl *pTreeCtrl, HTREEITEM hParentItem, DWORD dwItemData);

	// For CSkinMenu
	BOOL AfxInitSkinMenu(__out ATOM *pAtomRet/* = NULL*/);
	BOOL AfxUninitSkinMenu(ATOM atomMenuClass);

	_IUI_THREAD_STATE *IUIGetThreadState();

	// for windowless
	CWLWnd *GetDlgWLItem(HWND hWnd, int nItemID);	// WL means windowless
	CWLWnd *GetFirstWLWindow(HWND hWnd);
	CWLWnd *GetNextWLWindow(CWLWnd *pWnd, UINT wCmd);	// wCmd: see ::GetNextWindow in MSDN
	int SetWLWindowZOrder(CWLWnd *pWLWnd, CWLWnd *pWLWndInsertAfter);
	int SetDlgWLItemText(HWND hDlg, int nIDDlgItem, LPCTSTR lpString);
	CWLWnd *GetWLCapture();
	HRGN ApplyWLRgn(CWnd *pParent, CDC *pParentMemDC, CWLWnd *pChild);	// Apply windowless's region, return a copy of old region of pThis. you should delete the region after using.
	BOOL SetWLProp(CWLWnd *pWLWnd, LPCTSTR lpString, HANDLE hData);		// Same as SetProp for windowless object.
	HANDLE GetWLProp(CWLWnd *pWLWnd, LPCTSTR lpString);
	int DrawWLWindow(HDC hDstDC, CWLWnd *pChild);

	// for register shell file type
	BOOL IUISetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName/* = NULL*/);
	void IUIRegisterShellFileTypes(CString strDocStrings, BOOL bCompat);	// bCompat: print or print to.
	void IUIUnregisterShellFileTypes(CString &strDocStrings);


	// pCtrl may be a CWnd or CWLWnd
	int ApplyStoryboardItem(CObject *pApplyCtrl, BOOL bHWND, const CTRLPROPERTIES *pCtrlProp);

	// Find the CUIWndHandler bind with uWinID. An uWinID may be bind more than one CUIWndHandler,
	// Use lData to diff them. lData is a custom data set to CUIWndHandler.
	CUIWndHandler *FindUIWndHandler(UINT uWinID, LPARAM lData);

	// Create windowless child for spec host window.
	int _InstantiateItemBindTemplate(SItemBindObject *pItemBindObject, CWnd *pHostWnd);
	// Layout windowless child bind with tree/list item.
	int _RelayoutItemBindObject(CWnd *pCtrl, const CRect &rcDest, SItemBindObject *pItemBindObject);

	//////////////////////////////////////////////////////////////////////////
	// For debug

	class CDebug
	{
	public:
		CDebug();
		virtual ~CDebug();

	protected:
		static int LoadWinUserHeaderFile();

	public:
		// #define WM_CREATE 0x0001
		// Get string "WM_CREATE" from 0x0001
		static LPCTSTR GetMessageText(UINT uMsg);

	protected:
		static std::map<UINT, CString> m_mapMsgs;
	};


	//////////////////////////////////////////////////////////////////////////
	// Window and Control
	;
	class CControlBase
	{
	public:
		CControlBase();
		virtual ~CControlBase();

		virtual BOOL IsControlCreated() const;
		virtual HWND GetHostWnd();

		// Init control by specify style.
		;
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	protected:
		virtual int RouteWLMessage(HWND hWndThis, UINT message, WPARAM wParam, LPARAM lParam);
		virtual int DrawWLChildren(CDC *pMemDC);

		int OnSetBkImgTransparent(BYTE btTransparent, BOOL bRedraw);

		int BindBkColor(const CTRLPROPERTIES *pCtrlProp);

	public:
		bool IsWindowless() const;
		CWnd *GetBindWindow();
		CWLWnd *GetBindWindowless();

		virtual int Redraw(CWnd *pCtrl);
		virtual BOOL IsUpdateLayeredWindow();

		// Draw the parent background to control for alpha blend with background of the control.
		;
		virtual int SetDrawParentPartMode(DRAW_PARENT_PART eDrawParent);
		virtual DRAW_PARENT_PART GetDrawParentPartMode();
		// rcWin: relate to parent.
		virtual int DrawParentPart(CWnd *pCtrl, CDC *pCtrlMemDC, const CRect &rcWin, BOOL *pbUpdateLayeredWindow, CWnd *pSpecParent/* = NULL*/);
		// rect: relate to control.
		virtual int DrawParentBackground(CWnd *pCtrl, CDC *pCtrlMemDC, const CRect &rect, CWnd *pSpecParent/* = NULL*/);
		// rcArea2Control: relate to control.
		virtual int DrawParentNonClientPart(CWnd *pCtrl, CDC *pdcNonClient, const CRect &rcArea2Control, BOOL *pbUpdateLayeredWindow, CWnd *pSpecParent/* = NULL*/);

		virtual int DrawCombineBackground(
			CDC *pDC,
			const CRect &rcDest,
			int nPart,
			int nPartIndex);

		int LoadBkCombineImage(LPCTSTR lpszImage);

		int SetDrawFrameControl(bool bDraw);
		bool IsDrawFrameControl() const;

		virtual int SetBkCombineImage(LPCTSTR strCombineImage, BOOL bRedraw = TRUE);
		int GetBkCombineImage(CString *pstrCombineImage);
		bool IsBkCombineImage() const;

		virtual int SetFgCombineImage(LPCTSTR strCombineImage, BOOL bRedraw = TRUE);
		int GetFgCombineImage(CString *pstrCombineImage);
		bool IsFgCombineImage() const;

		virtual int SetBkImageTransparent(BYTE btTransparent, BOOL bRedraw = TRUE);
		BYTE GetBkImageTransparent() const;

		virtual int SetBkImageResizeMode(IMAGE_RESIZE_MODE eIrm);
		IMAGE_RESIZE_MODE GetBkImageResizeMode() const;

		virtual int SetBkImageResizePoint(POINT pt);
		int GetBkImageResizePoint(LPPOINT lpPt) const;

		virtual int SetBkImageRepeatX(LONG nRepeat);
		LONG GetBkImageRepeatX() const;
		virtual int SetBkImageRepeatY(LONG nRepeat);
		LONG GetBkImageRepeatY() const;

		virtual int SetDrawBkColor(BOOL bDraw);
		BOOL IsDrawBkColor() const;

		virtual int SetBackgroundColor(COLORREF cr);
		COLORREF GetBackgroundColor() const;

	protected:
		// Draw the control's default render.
		bool m_bWLWnd;
		bool m_bDrawDefault;
		bool m_bBkCombine;
		HIUIIMAGE m_himgCombineBk;
		IMAGE_RESIZE_MODE m_eBkImageResizeMode;
		POINT m_ptBkImageResize;
		LONG m_lBkImageRepeatX;
		LONG m_lBkImageRepeatY;
		bool m_bFgCombine;
		HIUIIMAGE m_himgCombineFg;

		DRAW_PARENT_PART m_eDrawParentPart;
		BYTE m_btBkImgTransparent;
		BYTE m_btFgImgTransparent;
		bool m_bDrawBkColor;
		COLORREF m_crBk;
		CBitmap m_bmpCompatible;

		std::vector<CWLWnd *> m_vpDynamicCreatedWLControl;

		CWnd *m_pBindWnd;		// The control pointer of this control while the control is HWND.
		CWLWnd *m_pBindWLWnd;	// The control pointer of this control while the control is windowless.
	};

	// The base class of CSkinButton and CWLButton
	class CButtonBase : public CControlBase
	{
	protected:
		CButtonBase();
		virtual ~CButtonBase();

	public:
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);
		virtual int BindPushButtonStyle(const BTNPROPERTIES *pCtrlProp);
		virtual int BindCheckBoxStyle(const CHKPROPERTIES *pCtrlProp);
		virtual int BindRadioButtonStyle(const RADPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

		virtual CONTROL_TYPE GetButtonControlType() const;

	protected:
		virtual int DrawButtonState(
			const CRect &rcClient,
			CDC *pDC,
			int nStateIndex,
			/* Background */
			const HIUIIMAGE himgMask,
			IMAGE_RESIZE_MODE eBkImgResizeMode,
			/* Foreground */
			const CRect &rcForegroundMargin,
			FOREGROUND_ALIGN_HOR eForegroundAlignHor,
			FOREGROUND_ALIGN_VER eForegroundAlignVer,
			COLORREF crFGMask,
			BOOL bChecked);

		virtual int DrawText(
			CDC *pDC,
			const CString &strText,
			const CRect &rcControl,
			const CRect &rcTextMargin,
			TEXT_ALIGN_HOR eTextAlignHor,
			TEXT_ALIGN_VER eTextAlignVer,
			HFONT hFont,
			COLORREF crText);

	public:
		CToolTipCtrl *GetToolTipCtrl();
		int SetToolTips(LPCTSTR lpszToolTips);
		int SetToolTipDelayTime(DWORD dwDuration, int iTime);

		int SetTag(LPCTSTR lpszTag);
		int GetTag(CString &strTag) const;

		int SetBindModalDlgID(UINT uID);
		UINT GetBindModalDlgID() const;

		int SetBindModelessWndID(UINT uID);
		UINT GetBindModelessWndID() const;

		int SetBindTaskWndMgr(UINT uID);
		UINT GetBindTaskWndMgr() const;

		int SetBindTaskWindow(UINT uID);
		UINT GetBindTaskWindow() const;

		int SetPushButtonType(BTN_TYPE eButtonType);
		BTN_TYPE GetPushButtonType() const;

		int SetCheckBoxType(CHK_TYPE eCheckBoxType);
		CHK_TYPE GetCheckBoxType() const;

		// color of button's text
		;
		int SetTextColor(COLORREF crN, COLORREF crH, COLORREF crS, COLORREF crD, BOOL bRedraw = TRUE);
		int GetTextColor(COLORREF *pcrN, COLORREF *pcrH, COLORREF *pcrS, COLORREF *pcrD) const;
		int SetCheckedTextColor(COLORREF crCN, COLORREF crCH, COLORREF crCS, COLORREF crCD,
			BOOL bRedraw = TRUE);
		int GetCheckedTextColor(COLORREF *pcrCN, COLORREF *pcrCH,
			COLORREF *pcrCS, COLORREF *pcrCD) const;

		// font of button's text
		;
		int SetTextFont(UINT uMask,
			LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDH, LPCTSTR lpszFontIDS, LPCTSTR lpszFontIDD,
			BOOL bRedraw = TRUE);
		int GetTextFont(UINT uMask,
			CString *pstrFontIDN, CString *pstrFontIDH,
			CString *pstrFontIDS, CString *pstrFontIDD) const;
		int SetCheckedTextFont(UINT uMask,
			LPCTSTR lpszFontIDCN, LPCTSTR lpszFontIDCH, LPCTSTR lpszFontIDCS, LPCTSTR lpszFontIDCD,
			BOOL bRedraw = TRUE);
		int GetCheckedTextFont(UINT uMask,
			CString *pstrFontIDCN, CString *pstrFontIDCH,
			CString *pstrFontIDCS, CString *pstrFontIDCD) const;

		// offset of button's text
		;
		int SetTextMargin(LPCRECT lprcTextMarginN, LPCRECT lprcTextMarginH, LPCRECT lprcTextMarginS, LPCRECT lprcTextMarginD, BOOL bRedraw = TRUE);
		int GetTextMargin(LPRECT lprcTextMarginN, LPRECT lprcTextMarginH, LPRECT lprcTextMarginS, LPRECT lprcTextMarginD) const;
		int SetCheckedTextMargin(LPCRECT lprcTextMarginCN, LPCRECT lprcTextMarginCH, LPCRECT lprcTextMarginCS, LPCRECT lprcTextMarginCD, BOOL bRedraw = TRUE);
		int GetCheckedTextMargin(LPRECT lprcTextMarginCN, LPRECT lprcTextMarginCH, LPRECT lprcTextMarginCS, LPRECT lprcTextMarginCD) const;

		// align mode of the button
		;
		int SetTextAlignHor(TEXT_ALIGN_HOR eHorAlignModeN, TEXT_ALIGN_HOR eHorAlignModeH, TEXT_ALIGN_HOR eHorAlignModeS, TEXT_ALIGN_HOR eHorAlignModeD, BOOL bRedraw = TRUE);
		int GetTextAlignHor(TEXT_ALIGN_HOR *peHorAlignModeN, TEXT_ALIGN_HOR *peHorAlignModeH, TEXT_ALIGN_HOR *peHorAlignModeS, TEXT_ALIGN_HOR *peHorAlignModeD) const;
		int SetTextAlignVer(TEXT_ALIGN_VER eVerAlignModeN, TEXT_ALIGN_VER eVerAlignModeH, TEXT_ALIGN_VER eVerAlignModeS, TEXT_ALIGN_VER eVerAlignModeD, BOOL bRedraw = TRUE);
		int GetTextAlignVer(TEXT_ALIGN_VER *peVerAlignModeN, TEXT_ALIGN_VER *peVerAlignModeH, TEXT_ALIGN_VER *peVerAlignModeS, TEXT_ALIGN_VER *peVerAlignModeD) const;
		int SetCheckedTextAlignHor(TEXT_ALIGN_HOR eHorAlignModeCN, TEXT_ALIGN_HOR eHorAlignModeCH, TEXT_ALIGN_HOR eHorAlignModeCS, TEXT_ALIGN_HOR eHorAlignModeCD, BOOL bRedraw = TRUE);
		int GetCheckedTextAlignHor(TEXT_ALIGN_HOR *peHorAlignModeCN, TEXT_ALIGN_HOR *peHorAlignModeCH, TEXT_ALIGN_HOR *peHorAlignModeCS, TEXT_ALIGN_HOR *peHorAlignModeCD) const;
		int SetCheckedTextAlignVer(TEXT_ALIGN_VER eVerAlignModeCN, TEXT_ALIGN_VER eVerAlignModeCH, TEXT_ALIGN_VER eVerAlignModeCS, TEXT_ALIGN_VER eVerAlignModeCD, BOOL bRedraw = TRUE);
		int GetCheckedTextAlignVer(TEXT_ALIGN_VER *peVerAlignModeCN, TEXT_ALIGN_VER *peVerAlignModeCH, TEXT_ALIGN_VER *peVerAlignModeCS, TEXT_ALIGN_VER *peVerAlignModeCD) const;

		// bitmap and checked bitmap
		;
		int SetBitmap(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD, BOOL bRedraw = TRUE);
		int GetBitmap(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD) const;

		int SetForegroundBitmap(UINT uMask,
			LPCTSTR lpszImageNameForegroundN,
			LPCTSTR lpszImageNameForegroundH,
			LPCTSTR lpszImageNameForegroundS,
			LPCTSTR lpszImageNameForegroundD,
			COLORREF crMask = RGB(255, 0, 255),
			BOOL bReDraw = TRUE);
		int GetForegroundBitmap(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameForegroundN,
			CString *pstrImageNameForegroundH,
			CString *pstrImageNameForegroundS,
			CString *pstrImageNameForegroundD,
			COLORREF *pcrMask) const;

		int SetForegroundIcon(HICON hIcon);
		HICON GetForegroundIcon() const;

		int SetForegroundImageMargin(LPCRECT rcForegroundMargin);
		int GetForegroundImageMargin(LPRECT lprcForegroundMargin) const;

		int SetForegroundImageAlign(
			FOREGROUND_ALIGN_HOR eAlignModeHor,
			FOREGROUND_ALIGN_VER eAlignModeVer);
		int GetForegroundImageAlign(
			FOREGROUND_ALIGN_HOR *peAlignModeHor,
			FOREGROUND_ALIGN_VER *peAlignModeVer) const;

		int SetCheckedBitmap(UINT uMask,
			LPCTSTR lpszImageNameCN, LPCTSTR lpszImageNameCH,
			LPCTSTR lpszImageNameCS, LPCTSTR lpszImageNameCD, BOOL bRedraw = TRUE);
		int SetCheckedBitmap(UINT uIDCN, UINT uIDCH, UINT uIDCS, UINT uIDCD,
			BOOL bDeleteOld = TRUE, BOOL bRedraw = TRUE);
		int GetCheckedBitmap(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameCN, CString *pstrImageNameCH,
			CString *pstrImageNameCS, CString *pstrImageNameCD) const;

		int SetCheckedForegroundBitmap(UINT uMask,
			LPCTSTR lpszImageNameForegroundCN, LPCTSTR lpszImageNameForegroundCH,
			LPCTSTR lpszImageNameForegroundCS, LPCTSTR lpszImageNameForegroundCD,
			COLORREF crMaskC = RGB(255, 0, 255), BOOL bReDraw = TRUE);
		int GetCheckedForegroundBitmap(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameForegroundCN,
			CString *pstrImageNameForegroundCH,
			CString *pstrImageNameForegroundCS,
			CString *pstrImageNameForegroundCD,
			COLORREF *pcrMaskC) const;

		int SetCheckedForegroundIcon(HICON hIcon);
		HICON GetCheckedForegroundIcon() const;

		int SetCheckedForegroundImageMargin(LPCRECT rcForegroundMargin);
		int GetCheckedForegroundImageMargin(LPRECT lprcForegroundMargin) const;

		int SetCheckedForegroundImageAlign(
			FOREGROUND_ALIGN_HOR eAlignModeHor,
			FOREGROUND_ALIGN_VER eAlignModeVer);
		int GetCheckedForegroundImageAlign(
			FOREGROUND_ALIGN_HOR *peAlignModeHorC,
			FOREGROUND_ALIGN_VER *peAlignModeVerC) const;

		// mask bitmap for checked and unchecked status
		;
		int SetMaskBitmap(UINT uMask,
			LPCTSTR lpszMaskN, LPCTSTR lpszMaskH,
			LPCTSTR lpszMaskS, LPCTSTR lpszMaskD, BOOL bRedraw = TRUE);
		int GetMaskBitmap(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameMaskN, CString *pstrImageNameMaskH,
			CString *pstrImageNameMaskS, CString *pstrImageNameMaskD) const;
		int SetCheckedMaskBitmap(UINT uMask, LPCTSTR lpszImageNameMaskCN, LPCTSTR lpszImageNameMaskCH, LPCTSTR lpszImageNameMaskCS, LPCTSTR lpszImageNameMaskCD, BOOL bRedraw = TRUE);
		int GetCheckedMaskBitmap(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameMaskCN, CString *pstrImageNameMaskCH,
			CString *pstrImageNameMaskCS, CString *pstrImageNameMaskCD) const;

		// animate text
		;
		int AnimateText(Direction eDirection, int nDelay, int nPixel = 1);
		int IsAnimateText() const;
		int StopAnimateText();

		int SetEndEllipsis(BOOL bEndEllipsis);
		BOOL IsEndEllipsis() const;

		int SetTabStop(BOOL bTabStop);
		BOOL IsTabStop() const;

		int SetActiveArea(const CRect &rect);
		int GetActiveArea(LPRECT lpRect) const;
		BOOL IsHasActiveArea() const;
		int RemoveActiveArea();

		BOOL IsCapture() const;
		BOOL IsHighlight() const;
		BOOL IsMultiLine() const;

		int SetHoverTime(DWORD dwHoverTime, BOOL bRepeat/* = FALSE*/);
		int GetHoverTime(DWORD *pdwHoverTime, BOOL *pbRepeat) const;

	protected:
		void *m_pMember;
	};

	class CSkinButton : public CButton, public CButtonBase
	{
		// Construction
	public:
		CSkinButton();
		virtual ~CSkinButton();

		virtual BOOL IsCreated() const;

	protected:
		virtual int OnDraw(CDC *pMemDC);

		// hide the same name interface in base class
	private:
		// DON'T call some interfaces in base class
		HBITMAP SetBitmap(HBITMAP hBitmap);
		HBITMAP GetBitmap() const;
		void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
		CFont *GetFont() const;
		BOOL SetTextMargin(RECT *pmargin);
		BOOL GetTextMargin(RECT *pmargin);

	public:
		int SetDraw(BOOL bDraw);
		BOOL IsDraw() const;

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinButton)
		;
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinButton)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};

	class CSkinEdit : public CEdit, public CControlBase
	{
		// Construction
	public:
		CSkinEdit();
		virtual ~CSkinEdit();

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	public:
		virtual int Render(CDC *pDC);

		BOOL SetTextColor(COLORREF clrText);
		COLORREF GetTextColor();

		BOOL SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD, BOOL bRedraw = TRUE);
		BOOL GetImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD);

		void SetDeflateRight(int nDeflate);

		int SetTextFont(LPCTSTR lpszFontID);
		int GetTextFont(CString *pstrFontID);

		int SetTextMargin(LPCRECT lpRect);
		int GetTextMargin(LPRECT lpRect) const;

		void SetTag(LPCTSTR lpszTag);
		void GetTag(CString &strTag);

		BOOL IsHasBorder() const;

		// hide the same name interface in base class
	private:
		void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
		CFont *GetFont() const;

	protected:
		int UpdateBackgroundBrush();

		virtual int OnDraw(CDC *pMemDC);

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinEdit)
	public:
		virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pLResult);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual BOOL PreTranslateMessage(MSG *pMsg);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinEdit)
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg HBRUSH CtlColor(CDC *pDC, UINT nCtlColor);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		void *m_pMember;
	};

	class CSkinComboBox : public CComboBox, public CControlBase
	{
		// Construction
	public:
		CSkinComboBox();
		virtual ~CSkinComboBox();

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	protected:
		virtual int OnDrawBackground(CDC *pdcMem, int nStateIndex);

	public:
		int GetBitmap(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD) const;
		int SetBitmap(UINT uMask, LPCTSTR lpszImageNameN,
			LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		virtual int SetBkCombineImage(LPCTSTR strCombineImage, BOOL bRedraw = TRUE);

		int SetTextFont(LPCTSTR lpszFontIDN);
		int GetTextFont(CString *pstrFontIDN) const;

		BOOL SetTextColor(COLORREF clrText);
		COLORREF GetTextColor() const;
		BOOL SetDropdownListTextColor(COLORREF crText);
		COLORREF GetDropdownListTextColor() const;

		int SetTextAlignHor(
			TEXT_ALIGN_HOR eHorAlignModeN,
			TEXT_ALIGN_HOR eHorAlignModeH,
			TEXT_ALIGN_HOR eHorAlignModeS,
			TEXT_ALIGN_HOR eHorAlignModeD,
			BOOL bRedraw = TRUE);

		int GetTextAlignHor(
			TEXT_ALIGN_HOR *peHorAlignModeN,
			TEXT_ALIGN_HOR *peHorAlignModeH,
			TEXT_ALIGN_HOR *peHorAlignModeS,
			TEXT_ALIGN_HOR *peHorAlignModeD);

		int SetTextAlignVer(
			TEXT_ALIGN_VER eVerAlignModeN,
			TEXT_ALIGN_VER eVerAlignModeH,
			TEXT_ALIGN_VER eVerAlignModeS,
			TEXT_ALIGN_VER eVerAlignModeD,
			BOOL bRedraw = TRUE);

		int GetTextAlignVer(
			TEXT_ALIGN_VER *peVerAlignModeN,
			TEXT_ALIGN_VER *peVerAlignModeH,
			TEXT_ALIGN_VER *peVerAlignModeS,
			TEXT_ALIGN_VER *peVerAlignModeD);

		int SetTextMargin(
			LPCRECT lprcTextMarginN,
			LPCRECT lprcTextMarginH,
			LPCRECT lprcTextMarginS,
			LPCRECT lprcTextMarginD,
			BOOL bRedraw = TRUE);

		int GetTextMargin(
			LPRECT lprcTextMarginN,
			LPRECT lprcTextMarginH,
			LPRECT lprcTextMarginS,
			LPRECT lprcTextMarginD) const;

		int SetButtonWidth(int nWidth);
		int GetButtonWidth(int *pnWidth) const;

		int SetComboBoxHeight(int nHeight);
		int GetComboBoxHeight(int *pnHeight) const;

#if(_MSC_VER <= 1200) // VC6.0
		int SetItemHeight(int nIndex, UINT cyItemHeight);
		int GetItemHeight(int nIndex) const;
#endif

		int SetTag(LPCTSTR lpszTag);
		int GetTag(CString &strTag) const;

		int DrawFocusRectangle(BOOL bDraw);
		BOOL IsDrawFocusRectangle();

		// hide the same name interface in base class
	private:
		void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
		CFont *GetFont() const;

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinComboBox)
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinComboBox)
		afx_msg void OnCloseup();
		afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
		CSkinEdit m_ctrlEdit;
	};

	class CSkinSliderCtrl : public CSliderCtrl, public CControlBase
	{
		// Construction
	public:
		CSkinSliderCtrl();
		virtual ~CSkinSliderCtrl();

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	protected:
		BOOL IsMouseOverThumb();

	public:
		int SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD);
		int GetImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameD);

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinSliderCtrl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
		//{{AFX_MSG(CSkinSliderCtrl)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg HBRUSH CtlColor(CDC *pDC, UINT nCtlColor);
		afx_msg void OnCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg LRESULT OnGetThumbRect(WPARAM wParam, LPARAM lParam);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	class CSkinStatic : public CStatic, public CControlBase
	{
		// Construction
	public:
		CSkinStatic();
		virtual ~CSkinStatic();

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	public:
		int SetBitmap(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD, BOOL bRedraw = TRUE);
		int GetBitmap(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameD) const;

		void SetTextMultiline(BOOL bMultiline);
		BOOL IsTextMultiline();

		int SetPathEllipsis(BOOL bPathEllipsis);
		BOOL IsPathEllipsis() const;

		int SetEndEllipsis(BOOL bEndEllipsis);
		BOOL IsEndEllipsis() const;

		void SetTextAlignHor(TEXT_ALIGN_HOR eHorAlignModeN, TEXT_ALIGN_HOR eHorAlignModeD);
		void GetTextAlignHor(TEXT_ALIGN_HOR *peHorAlignModeN, TEXT_ALIGN_HOR *peHorAlignModeD);

		void SetTextAlignVer(TEXT_ALIGN_VER eVerAlignModeN, TEXT_ALIGN_VER eVerAlignModeD);
		void GetTextAlignVer(TEXT_ALIGN_VER *peVerAlignModeN, TEXT_ALIGN_VER *peVerAlignModeD);

		void SetTextColor(COLORREF crN, COLORREF crD);
		void GetTextColor(COLORREF *pcrN, COLORREF *pcrD);

		void SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD);
		void GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD);

		int SetPadding4TextN(LPCRECT lprcMargin);
		int GetPadding4TextN(LPRECT lprcMargin) const;

		int SetPadding4TextD(LPCRECT lprcMargin);
		int GetPadding4TextD(LPRECT lprcMargin) const;

		void SetForegroundBitmap(UINT uMask, LPCTSTR lpszImageNameForegroundN, LPCTSTR lpszImageNameForegroundD,
			const CRect &rcForegroundMargin,
			FOREGROUND_ALIGN_HOR eAlignModeHor = FAH_LEFT, FOREGROUND_ALIGN_VER eAlignModeVer = FAV_CENTER,
			COLORREF crMask = RGB(255, 0, 255), BOOL bReDraw = TRUE);
		void GetForegroundBitmap(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameForegroundN, CString *pstrImageNameForegroundD,
			LPRECT lprcForegroundMargin,
			int *pnAlignModeHor, int *pnAlignModeVer, COLORREF *pcrMask);

		void SetToolTips(LPCTSTR lpszToolTips);
		CToolTipCtrl *GetToolTipCtrl();

		// Shadow text
		;
		int ShadowText(BOOL bShadowText);
		BOOL IsShadowText() const;

		int SetTextShadowColor(COLORREF crTextShadow);
		COLORREF GetTextShadowColor() const;

		int SetTextShadowOffset(LPPOINT lpptOffset);
		int GetTextShadowOffset(LPPOINT lpptOffset) const;

		// hide the same name interface in base class
	private:
		void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
		CFont *GetFont() const;

	protected:
		virtual int OnDraw(CDC *pMemDC);
		virtual int DrawText(CDC *pMemDC, LPCRECT lprcClient, BOOL bEnabled);

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinStatic)
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinStatic)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////
	// The progress control has a border
	;
	class CSkinProgressCtrl : public CProgressCtrl, public CControlBase
	{
		// Construction
	public:
		CSkinProgressCtrl();
		virtual ~CSkinProgressCtrl();

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	protected:
		virtual int DrawThumb(CDC *pMemDC);
		virtual int DrawHorizontalThumb(CDC *pMemDC);
		virtual int DrawVerticalThumb(CDC *pMemDC);
		virtual int DrawText(CDC *pMemDC);

	public:
		void ShowText(BOOL bIsShow);
		BOOL IsShowText();

		int SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD);
		int GetImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameD);

		int CombineThumbImage(BOOL bCombine);
		BOOL IsCombineThumbImage() const;

		int SetCombineThumbImage(LPCTSTR lpszImageName);
		int GetCombineThumbImage(CString *pstrImage) const;

		int SetThumbImages(UINT uMask, LPCTSTR lpszImageNameThumbN, LPCTSTR lpszImageNameThumbD);
		int GetThumbImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameThumbN, CString *pstrImageNameThumbD) const;

		int SetThumbImageMode(PROGRESS_THUMB_MODE eMode);
		PROGRESS_THUMB_MODE GetThumbImageMode() const;

		int SetThumbPositionMode(PROGRESS_THUMB_POSITION ePositionMode);
		PROGRESS_THUMB_POSITION GetThumbPositionMode() const;

		int SetThumbSpecifyOriginalPosition(LPCRECT lpThumbRect);
		int GetThumbSpecifyOriginalPosition(LPRECT lpThumbRect) const;

		int SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD);
		int GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD);

		// hide the same name interface in base class
	private:
		void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
		CFont *GetFont() const;

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinProgressCtrl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinProgressCtrl)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	class CSkinSpinButtonCtrl : public CSpinButtonCtrl, public CControlBase
	{
		// Construction
	public:
		CSkinSpinButtonCtrl();
		virtual ~CSkinSpinButtonCtrl();

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	protected:
		virtual int OnDraw(CDC *pMemDC);

	public:
		int SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		int GetImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH, CString *pstrImageNameS, CString *pstrImageNameD);

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinSpinButtonCtrl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinSpinButtonCtrl)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		void *m_pMember;
	};

	class CSkinIPAddressCtrl : public CIPAddressCtrl, public CControlBase
	{
		// Construction
	public:
		CSkinIPAddressCtrl();
		virtual ~CSkinIPAddressCtrl();

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	public:
		int SetImages(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		int GetImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD);

		// Set/Get child edit controls image
		int SetCombineFieldImage(LPCTSTR lpszImageName);
		int SetFieldImage(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		int GetFieldImage(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD);

		// Set/Get filler between two child edit control.
		int SetCombineFillerImage(LPCTSTR lpszImageName);
		int SetFillerImage(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		int GetFillerImage(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD);

		int SetBorderRect(LPCRECT lprcBorder);
		int GetBorderRect(LPRECT lprcBorder) const;

		int SetBorderColor(COLORREF crBorder);
		COLORREF GetBorderColor() const;

		int SetBackgroundColor(COLORREF crBorder);
		COLORREF GetBackgroundColor() const;

		int SetFieldHeight(UINT uHeight);
		int GetFieldHeight() const;

		int SetFieldAlignModeVer(CONTROL_LAYOUT_VER eAlignMode);
		CONTROL_LAYOUT_VER GetFieldAlignModeVer() const;

		int SetFillerWidth(UINT uWidth);
		UINT GetFillerWidth() const;

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinIPAddressCtrl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinIPAddressCtrl)
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	class CSkinHotKeyCtrl : public CHotKeyCtrl, public CControlBase
	{
		// Construction
	public:
		CSkinHotKeyCtrl();
		virtual ~CSkinHotKeyCtrl();

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	public:
		int SetImages(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		int GetImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameD);

		// Operations
	public:

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinHotKeyCtrl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinHotKeyCtrl)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	/////////////////////////////////////////////////////////////////////////////
	// CSliderCtrlEx window

#ifdef _WIN32
#define WC_TRACKBAREXA            "msctls_trackbar32ex"
#define WC_TRACKBAREXW            L"msctls_trackbar32ex"

#ifdef UNICODE
#define  WC_TRACKBAREX            WC_TRACKBAREXW
#else
#define  WC_TRACKBAREX            WC_TRACKBAREXA
#endif

#else
#define WC_TRACKBAREX             "msctls_trackbarex"
#endif

#define WM_GETTOOLTIPS            WM_SLIDER+1	// wParam: point to a CString object; lParam, handle of the sliderex

#define TBS_EX_NORMAL             0x00000000
#define TBS_EX_PROGRESS           0x00001000
#define TBS_EX_SPLITPROGRESS      0x00002000
	class CSliderCtrlEx : public CWnd, public CControlBase
	{
		// Construction
	public:
		CSliderCtrlEx();
		virtual ~CSliderCtrlEx();
		virtual BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	public:
		DWORD GetExtendedStyle();
		DWORD SetExtendedStyle(DWORD dwNewStyle);

		void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);
		void GetRange(int *pnMin, int *pnMax) const;

		void SetThumbSize(SIZE sizeThumb);
		int GetThumbSize(SIZE *psizeThumb) const;
		virtual int SetDeflate(UINT nLeftTop, UINT nRightBottom);
		int GetDeflate(UINT *pnLeftTop, UINT *pnRightBottom) const;
		void GetThumbRect(LPRECT lprc) const;

		int GetPos() const;
		void SetPos(int nPos);

		int GetProgressPos() const;
		void SetProgressPos(int nPos);

		int SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameMarkN,
			LPCTSTR lpszImageNameMarkD, LPCTSTR lpszImageNameD);
		int GetImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameMarkN,
			CString *pstrImageNameMarkD, CString *pstrImageNameD);

		int SetThumbCombineImage(LPCTSTR lpszImageNameThumbCombine, BOOL bRedraw = TRUE);
		int SetThumbImages(UINT uMask, LPCTSTR lpszImageNameThumbN, LPCTSTR lpszImageNameThumbH,
			LPCTSTR lpszImageNameThumbS, LPCTSTR lpszImageNameThumbD);
		int GetThumbImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameThumbN, CString *pstrImageNameThumbH,
			CString *pstrImageNameThumbS, CString *pstrImageNameThumbD);
		BOOL IsCombineThumbImage() const;

		int UseMarkBitmap(BOOL bUseMark);
		BOOL IsUseMarkBitmap() const;

		int MarkImageToThumb(BOOL bToThumb);
		BOOL IsMarkImageToThumb() const;

		int SetSplitCount(int nCount);
		int GetSplitCount();
		int SetSplitList(const int *pnFirst, const int *pnLast, BOOL bRedraw = TRUE);
		int AddColorImage(int nValue, HBITMAP hbmpColor);
		int RemoveColorImage(int nValue);

		int EnableToolTipsEx(BOOL bEnable, BOOL bMouseMove = TRUE);
		int SetToolTips(LPCTSTR lpszToolTips);
		CToolTipCtrl *GetToolTipCtrl();

		// For tip window
		;
		int EnableTipWindow(BOOL bEnable);
		BOOL IsEnableTipWindow() const;

		int SetTipWindow(CWnd *pTipWnd);
		CWnd *GetTipWindow();

		int SetTipWindowOffset(const LPSIZE lpsizeOffset);
		int GetTipWindowOffset(LPSIZE lpsizeOffset) const;

		// Operations
	public:

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSliderCtrlEx)
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Implementation
	public:
		;
		virtual void DrawHorzSlider(CDC *pMemDC);
		virtual void DrawVertSlider(CDC *pMemDC);

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSliderCtrlEx)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////
	//

	class CTreeCtrlEx : public CWnd
	{
	public:
		CTreeCtrlEx();
		virtual ~CTreeCtrlEx();

	public:
		virtual BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);

		// Attributes

		// Retrieves the bounding rectangle for the specified item.
		BOOL GetItemRect(__in HTREEITEM hItem, __out LPRECT lpRect, __in BOOL bTextOnly) const;

		// Gets the count of items in the control.
		UINT GetCount() const;

		// Returns the level-to-level indentation (in pixels).
		UINT GetIndent() const;

		// Sets the level-to-level indentation (in pixels).
		void SetIndent(__in UINT nIndent);

		// Retrieves the image list associated with the control.
		CImageList *GetImageList(__in int nImageList) const;

		// Sets the image list associated with this control.
		CImageList *SetImageList(__in CImageList *pImageList, __in int nImageList);

		// Retrieves the next item having the given relationship with the
		// specified item.
		HTREEITEM GetNextItem(__in HTREEITEM hItem, __in UINT nCode) const;

		// Retrieves the next child item after the specified item.
		HTREEITEM GetChildItem(__in HTREEITEM hItem) const;

		// Retrieves the next sibling item after the specified item.
		HTREEITEM GetNextSiblingItem(__in HTREEITEM hItem) const;

		// Retrieves the previous sibling item of the specified item.
		HTREEITEM GetPrevSiblingItem(__in HTREEITEM hItem) const;

		// Retrieves the parent of the specified item.
		HTREEITEM GetParentItem(__in HTREEITEM hItem) const;

		// Retrieves the topmost visible item in the control.
		HTREEITEM GetFirstVisibleItem() const;

		// Retrieves the next visible item after the specified item.
		HTREEITEM GetNextVisibleItem(__in HTREEITEM hItem) const;

		// Retrieves the previous visible item before the specified item.
		HTREEITEM GetPrevVisibleItem(__in HTREEITEM hItem) const;

		// Retrieves the last expanded item in the tree. This does not retrieve the last item visible in the tree-view window.
		HTREEITEM GetLastVisibleItem() const;

		// Retrieves the higlighted item, NULL if none.
		HTREEITEM GetSelectedItem() const;

		// Retrieves the currently drop-highlighted item, NULL if none.
		HTREEITEM GetDropHilightItem() const;

		// Retrieves the root item of the control, NULL if none.
		HTREEITEM GetRootItem() const;

		// Retrieves information about a particular item in the control.
		BOOL GetItem(__out TVITEM *pItem) const;

		// Retrieves the text associated with the given item.
		CString GetItemText(__in HTREEITEM hItem) const;

		// Retrieves the images associated with the given item.
		BOOL GetItemImage(__in HTREEITEM hItem, __out int &nImage,
			__out int &nSelectedImage) const;

		// Retrieves the state of the given item.
		UINT GetItemState(__in HTREEITEM hItem, __in UINT nStateMask) const;

		// Retrieves the user-supplied data associated with the given item.
		DWORD_PTR GetItemData(__in HTREEITEM hItem) const;

		// Sets the state of the an item.
		BOOL SetItem(__in TVITEM *pItem);
		BOOL SetItem(__in HTREEITEM hItem, __in UINT nMask, __in LPCTSTR lpszItem, __in int nImage,
			__in int nSelectedImage, __in UINT nState, __in UINT nStateMask, __in LPARAM lParam);

#if (_WIN32_IE >= 0x0600)
		BOOL SetItemEx(__in HTREEITEM hItem, __in UINT nMask, __in LPCTSTR lpszItem, __in int nImage,
			__in int nSelectedImage, __in UINT nState, __in UINT nStateMask, __in LPARAM lParam,
			__in UINT uStateEx, __in HWND hWnd, __in int iExpandedImage);
#endif

		// Sets the text of the specified item.
		BOOL SetItemText(__in HTREEITEM hItem, __in LPCTSTR lpszItem);

		// Sets the image on the specified item.
		BOOL SetItemImage(__in HTREEITEM hItem, __in int nImage, __in int nSelectedImage);

		// Sets the state of the specified item.
		BOOL SetItemState(__in HTREEITEM hItem, __in UINT nState, __in UINT nStateMask);

		// Sets the user data on the specified item.
		BOOL SetItemData(__in HTREEITEM hItem, __in DWORD_PTR dwData);

		// Determines if the specified item has children.
		BOOL ItemHasChildren(__in HTREEITEM hItem) const;

		// Retrieves the edit control used to perform in-place editing.
		CEdit *GetEditControl() const;

		// Gets the count of items presently visible in the control.
		UINT GetVisibleCount() const;

		// Retrieves the tool tip control associated with this control.
		CToolTipCtrl *GetToolTips() const;

		// Sets the tool tip control to be used by this control.
		CToolTipCtrl *SetToolTips(__in CToolTipCtrl *pWndTip);

		// Retrieves the background colour used throughout the control.
		COLORREF GetBkColor() const;

		// Sets the background color to be used throughout the control.
		COLORREF SetBkColor(__in COLORREF clr);

		// Retrieves the height of items in the control.
		SHORT GetItemHeight() const;

		// Sets the height of items in the control.
		SHORT SetItemHeight(__in SHORT cyHeight);

		// Retrieves the text color used for all items in the control.
		COLORREF GetTextColor() const;

		// Sets the text color used for all items in the control.
		COLORREF SetTextColor(__in COLORREF clr);

		// Sets the insertion mark to the specified item in the control.
		BOOL SetInsertMark(__in HTREEITEM hItem, __in BOOL fAfter = TRUE);

		// Gets the checked state of the specified item in the control.
		// (Only useful on a control with the TVS_CHECKBOXES style.)
		BOOL GetCheck(__in HTREEITEM hItem) const;

		// Sets the checked state of the specified item in the control.
		// (Only useful on a control with the TVS_CHECKBOXES style.)
		BOOL SetCheck(__in HTREEITEM hItem, __in BOOL fCheck = TRUE);

		COLORREF GetInsertMarkColor() const;
		COLORREF SetInsertMarkColor(__in COLORREF clrNew);

		// Sets the maximum time (in milliseconds) the control will
		// spend smooth scrolling its content.
		UINT SetScrollTime(__in UINT uScrollTime);

		// Retrieves the maximum time (in milliseconds) the control will
		// spend smooth scrolling its content.
		UINT GetScrollTime() const;

#if _WIN32_IE >= 0x0500
		COLORREF GetLineColor() const;
		COLORREF SetLineColor(__in COLORREF clrNew = CLR_DEFAULT);
#endif

#if (_WIN32_IE >= 0x0600)
#endif

#if (_WIN32_WINNT >= 0x0501) && defined(UNICODE)
		// Maps treeview item id to accessibility identifier.
		UINT MapItemToAccId(HTREEITEM hItem) const;

		// Maps accessibility identifier id to treeview item.
		HTREEITEM MapAccIdToItem(UINT uAccId) const;

		// Set autoscrolling rate and delay (?).
		BOOL SetAutoscrollInfo(UINT uPixelsPerSec, UINT uUpdateTime);
#endif

#if (_WIN32_WINNT >= 0x0600) && defined(UNICODE)
		// Get count of selected items in the tree control.
		UINT GetSelectedCount();

		// Get rectangle for the specified part of the specified item.
		BOOL GetItemPartRect(HTREEITEM hItem, TVITEMPART nPart, LPRECT lpRect);

		// Get the extended state of the item
		UINT GetItemStateEx(HTREEITEM hItem) const;

		// Set the extended state of the item
		BOOL SetItemStateEx(HTREEITEM hItem, UINT uStateEx);

		// Get the expanded image index for the specified item
		int GetItemExpandedImageIndex(HTREEITEM hItem) const;

		// Set the expanded image index for the specified item
		BOOL SetItemExpandedImageIndex(HTREEITEM hItem, int nIndex);

		// Get extended styles for the tree control.
		DWORD GetExtendedStyle() const;

		// Set extended styles on the tree control.
		DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles);
#endif

		// Operations

		// Inserts a new item to the control.
		HTREEITEM InsertItem(__in LPTVINSERTSTRUCT lpInsertStruct);
		HTREEITEM InsertItem(__in UINT nMask, __in LPCTSTR lpszItem, __in int nImage,
			__in int nSelectedImage, __in UINT nState, __in UINT nStateMask, __in LPARAM lParam,
			__in HTREEITEM hParent, __in HTREEITEM hInsertAfter);
		HTREEITEM InsertItem(__in LPCTSTR lpszItem, __in HTREEITEM hParent = TVI_ROOT,
			__in HTREEITEM hInsertAfter = TVI_LAST);
		HTREEITEM InsertItem(__in LPCTSTR lpszItem, __in int nImage, __in int nSelectedImage,
			__in HTREEITEM hParent = TVI_ROOT, __in HTREEITEM hInsertAfter = TVI_LAST);

		// Removes the specified item from the control.
		BOOL DeleteItem(__in HTREEITEM hItem);

		// Removes all items from the control.
		BOOL DeleteAllItems();

		// Expands the children of the specified item.
		BOOL Expand(__in HTREEITEM hItem, __in UINT nCode);

		// Selects the specified item.
		BOOL Select(__in HTREEITEM hItem, __in UINT nCode);

		// Selects the specified item.
		BOOL SelectItem(__in HTREEITEM hItem);

		// Selects an item to be the drop target in the control.
		BOOL SelectDropTarget(__in HTREEITEM hItem);

		// Draws the specified item as the drop target for the control.
		BOOL SelectSetFirstVisible(__in HTREEITEM hItem);

		// Begins editing the label of the specified item.
		CEdit *EditLabel(__in HTREEITEM hItem);

		// Determines the visual feature of the control under
		// the specified point.
		HTREEITEM HitTest(__in CPoint pt, __in UINT *pFlags = NULL) const;
		HTREEITEM HitTest(__in TVHITTESTINFO *pHitTestInfo) const;

		// Create a drag image for the specified item.
		CImageList *CreateDragImage(__in HTREEITEM hItem);

		// Sorts all children of the specified item.
		BOOL SortChildren(__in HTREEITEM hItem);

		// Scrolls the control to ensure the specified item is visible.
		BOOL EnsureVisible(__in HTREEITEM hItem);

		// Sorts items in the control using the provided callback function.
		BOOL SortChildrenCB(__in LPTVSORTCB pSort);

		// Terminates label editing operation.
		BOOL EndEditLabelNow(BOOL fCancelWithoutSave);

#if (_WIN32_WINNT >= 0x0600) && defined(UNICODE)
		// Shows information tooltip on the specified item.
		void ShowInfoTip(HTREEITEM hItem);
#endif

		// Implementation
	protected:
		void RemoveImageList(__in int nImageList);

	public:
		//{{AFX_MSG(CTreeCtrlEx)
		afx_msg void OnDestroy();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};

	class CSkinHeaderCtrl : public CHeaderCtrl, public CControlBase
	{
		// Construction
	public:
		CSkinHeaderCtrl();
		virtual ~CSkinHeaderCtrl();

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	public:
		void SetTreeCtrlEx(CWnd *pWnd);
		CWnd *GetTreeCtrlEx();

		int SetBkImage(LPCTSTR lpszImageName);
		int GetBkImage(CString *pstrImageName) const;

		int SetButtonCombineImages(LPCTSTR lpszImageNameBk);

		int SetButtonImages(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkH,
			LPCTSTR lpszImageNameBkS, LPCTSTR lpszImageNameBkD);
		int GetButtonImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameBkN, CString *pstrImageNameBkH,
			CString *pstrImageNameBkS, CString *pstrImageNameBkD) const;

		int SetButtonBkImageResizePoint(POINT pt);
		int GetButtonBkImageResizePoint(LPPOINT lpPt) const;

		int SetButtonBkImageRepeatX(LONG nRepeat);
		LONG GetButtonBkImageRepeatX() const;

		int SetButtonBkImageRepeatY(LONG nRepeat);
		LONG GetButtonBkImageRepeatY() const;

		int SetTextColor(COLORREF crN, COLORREF crH,
			COLORREF crS, COLORREF crD, BOOL bRedraw = TRUE);
		int GetTextColor(COLORREF *pcrN, COLORREF *pcrH, COLORREF *pcrS, COLORREF *pcrD) const;

		int SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDH,
			LPCTSTR lpszFontIDS, LPCTSTR lpszFontIDD, BOOL bRedraw = TRUE);
		int GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDH,
			CString *pstrFontIDS, CString *pstrFontIDD) const;

		int SetHeaderHeight(int nHeight);
		int GetHeaderHeight() const;

		int EnableHeaderTrack(BOOL bEnable);
		BOOL IsHeaderTrackEnable() const;

		int EnableHeaderDragDrop(BOOL bEnable);
		BOOL IsHeaderDragDropEnable() const;

		int HitTest(CPoint pt);

		// hide the same name interface in base class
	private:
		void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
		CFont *GetFont() const;

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinHeaderCtrl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pLResult);
		//}}AFX_VIRTUAL
		;
		virtual void DrawItem(CDC *pDC, int nItem);

	protected:
		//{{AFX_MSG(CSkinHeaderCtrl)
		afx_msg void OnPaint();
		//}}AFX_MSG
		afx_msg void OnNMEndTrack(NMHDR *pNMHDR, LRESULT *pResult);
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	/////////////////////////////////////////////////////////////////////////////
	// CScrollBarEx window

	//   _________________________________________
	//  | 0 |    3     |////// 2 /////|   4   | 1 |
	//  |___|__________|______________|_______|___|
	//
	//  0: Left or Up arrow
	//  1: Right or Down arrow
	//  2: Thumb
	//  3: Space1
	//  4: Space2

#define GIR_LTUP             0x00000001
#define GIR_RTDN             0x00000010
#define GIR_THUMB            0x00000100
#define GIR_ALL              GIR_LTUP|GIR_RTDN|GIR_THUMB

#ifdef _WIN32
#define WC_SCROLLBAREXA            "ScrollBarEx"
#define WC_SCROLLBAREXW            L"ScrollBarEx"

#ifdef UNICODE
#define  WC_SCROLLBAREX            WC_SCROLLBAREXW
#else
#define  WC_SCROLLBAREX            WC_SCROLLBAREXA
#endif

#else
#define WC_SCROLLBAREX             "ScrollBarEx"
#endif

	class CScrollBarEx : public CWnd, public CControlBase
	{
		// Construction
	public:
		CScrollBarEx();
		virtual ~CScrollBarEx();
		virtual int ReleaseObject();

		virtual BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);
		virtual int BindHorzScrollbarStyle(const HORSBPROPERTIES *pHorzScrollbarProp);
		virtual int BindVertScrollbarStyle(const VERSBPROPERTIES *pVertScrollbarProp);

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CScrollBarEx)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Implementation
		;
	public:
		int SetBkBitmaps(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkH,
			LPCTSTR lpszImageNameBkS, LPCTSTR lpszImageNameBkD);

		int SetCombineThumbBitmap(LPCTSTR lpszImageName);
		int SetThumbBitmaps(UINT uMask, LPCTSTR lpszImageNameThumbN, LPCTSTR lpszImageNameThumbH, LPCTSTR lpszImageNameThumbS, LPCTSTR lpszImageNameThumbD);
		int GetThumbBitmaps(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameThumbN, CString *pstrImageNameThumbH,
			CString *pstrImageNameThumbS, CString *pstrImageNameThumbD);
		int SetThumbImageResizePoint(POINT pt);
		POINT GetThumbImageResizePoint() const;

		int SetLTUPArrow(LPCTSTR lpszImageNameArrow);
		int GetLTUPArrow(CString *pstrImageNameArrow);

		int SetRTDNArrow(LPCTSTR lpszImageNameArrow);
		int GetRTDNArrow(CString *pstrImageNameArrow);

		void SetLTUPButton(int nValue);
		int GetLTUPButton();

		void SetRTDNButton(int nValue);
		int GetRTDNButton();

		void SetNotifyWnd(CWnd *pWnd);
		CWnd *GetNotifyWnd();

		virtual void DrawSizeBox(CDC *pMemDC);
		virtual void DrawHorzItem(CDC *pMemDC);
		virtual void DrawVertItem(CDC *pMemDC);
		virtual int DrawThumb(CDC *pMemDC, const CRect &rcThumb);

		void GetItemRect(LPRECT lprcLTUP, LPRECT lprcRTDN, LPRECT lprcThumb, UINT nMask = GIR_ALL);
		int GetThumbLength() const;

		void ShowScrollBar(BOOL bShow = TRUE);
		BOOL EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH);

		void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);
		void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const;

		BOOL SetScrollPos(int nPos, BOOL bRedraw = TRUE);
		int GetScrollPos() const;

		BOOL SetScrollInfo(LPCSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
		BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);

		int GetScrollLimit();

#if(WINVER >= 0x0500)
		BOOL GetScrollBarInfo(PSCROLLBARINFO pScrollInfo) const;
#endif //(WINVER >= 0x0500)

		// Generated message map functions
	protected:
		//{{AFX_MSG(CScrollBarEx)
		afx_msg void OnPaint();
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		//}}AFX_MSG
		afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
		DECLARE_MESSAGE_MAP()

	private:
		UINT m_nLTUP; // Width of the left arrow of a horizontal scroll bar or Height of the up arrow of a vertical scroll bar
		UINT m_nRTDN; // Width of the right arrow of a horizontal scroll bar or Height of the down arrow of a vertical scroll bar
		SCROLLINFO m_si;
		DWORD m_dwStyle;

		HIUIIMAGE m_himgBk[4];
		HIUIIMAGE m_himgButton1Arrow;
		HIUIIMAGE m_himgButton2Arrow;

		bool m_bCombineThumb;
		HIUIIMAGE m_himgCombineThumb;
		HIUIIMAGE m_himgThumb[4];
		POINT m_ptThumbImageResize;

		int m_nCapture;				// -1: mouse not be captrue; 0: captured by LTUP arrow; 1: captured by RTDN arrow;  2: captured by thumb; 3: captured by space1; 4: captured by space2;
		// 2: captured by thumb, 3: captured by space1; 4: captured by space2
		int m_nArrowFlags;			// disable or enable the arrow of the scroll
		CPoint m_ptDragThumb;		// record the point when drag the thumb
		SCROLLINFO m_siDrag;		// record the scroll information when drag the thumb
		int m_nDragOffset;			// record the offset from first position when drag the thumb
		CWnd *m_pNotifyWnd;
		BOOL m_bCaptureThumb;
		BOOL m_bIsMouseOverThumb;
		BOOL m_bCaptureLTUPButton;
		BOOL m_bIsMouseOverLTUPButton;
		BOOL m_bCaptureRTDNButton;
		BOOL m_bIsMouseOverRTDNButton;
	};

	/////////////////////////////////////////////////////////////////////////////
	// CSkinListCtrl window

#define WM_DRAWLIST							WM_LISTCTRL+1		// WPARAM(CPaintDC *pDC), LPARAM((CSkinListCtrl *)this)
#define LVM_ITEM_TEXT_COLOR					WM_LISTCTRL+2		// WPARAM: not use; LPARAM: ITEMTEXTCOLOR *; return: non-zero if it processes this message
#define LVM_DRAW_BACKGROUND					WM_LISTCTRL+3		// WPARAM: CDC *; LPARAM: not use, must be 0.
#define LVM_DRAW_GRIDLINE					WM_LISTCTRL+4		// WPARAM: CDC *; LPARAM: not use, must be 0.
#define LVM_DRAW_ITEM						WM_LISTCTRL+5		// WPARAM: CDC *; LPARAM: item index
#define LVM_DRAW_DEFAULT					WM_LISTCTRL+6		// WPARAM: HDC; LPARAM: not use, must be 0.

	// WPARAM: [in]
	//			(high word):	1: Horizontal line; 2: Vertical line.
	//			(low word):		Control identifier
	// LPARAM: [out] HPEN *.
	// return value: 0, Allow to draw the line; non-0, refuse to draw the line.
#define WM_GETLISTGRIDLINEPEN				WM_LISTCTRL+10

	class CRectCtrl;

	class CSkinListCtrl : public CListCtrl, public CControlBase
	{
		// Construction
	public:
		CSkinListCtrl();
		virtual ~CSkinListCtrl();

		virtual BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
		virtual BOOL Create(__in DWORD dwStyle, __in CRectCtrl *pRectCtrl, __in CWnd *pParentWnd, __in UINT nID);

		// Init control by specify style.
		;
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	public:
#if(_MSC_VER <= 1400) // VC8.0
		BOOL GetItemSpacing(BOOL fSmall, int *pnHorzSpacing, int *pnVertSpacing) const;
#endif

		int ReDrawControl(BOOL bErase, BOOL bUpdateScrollBar, LPCRECT lpInvalidateRect = NULL);
		int UpdateImageListCache();
		int UpdateSubItemImageListCache();

	public:
		// Header
		;
		int SetHeaderHeight(int nHeight);
		int GetHeaderHeight() const;

		int SetHeaderBkImage(LPCTSTR lpszImageNameHeaderBk);
		int GetHeaderBkImage(CString *pstrImageNameHeaderBk) const;

		int SetHeaderButtonImages(
			UINT uMask,
			LPCTSTR lpszImageNameHeaderN, LPCTSTR lpszImageNameHeaderH,
			LPCTSTR lpszImageNameHeaderS, LPCTSTR lpszImageNameHeaderD);
		int GetHeaderButtonImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameHeaderN, CString *pstrImageNameHeaderH,
			CString *pstrImageNameHeaderS, CString *pstrImageNameHeaderD) const;

		int EnableHeaderTrack(BOOL bEnable);
		BOOL IsHeaderTrackEnable() const;

		// for icon in other columns with report style
		;
		int SetImageListEx(CImageList *pImageList);
		CImageList *GetImageListEx() const;
		int GetSubItemIconRect(int nItem, int nSubItem, LPRECT lpRect) const;

		BOOL SetItemTextEx(int nItem, int nSubItem, LPCTSTR lpszText, int nImageEx);
		int GetItemImage(int nItem, int nSubItem) const;

		// Insert a windowless control to a report list sub item.
		// The report list like a CGridPanel.
		int SetItemControl(int nItem, int nSubItem, CWLWnd *pWLItem);
		int SetItemControl(int nItem, int nSubItem, UINT uBindWndID);

		CWLWnd *GetItemControl(int nItem, int nSubItem) const;
		UINT GetItemBindWndID(int nItem, int nSubItem) const;

		// for background image
		;
		int EnableBkImage(BOOL bEnable);
		BOOL IsEnableBkImage() const;

		int SetImages(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkD);
		int GetImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameBkN, CString *pstrImageNameBkD) const;

		int EnableOwnerDraw(BOOL bEnable);
		BOOL IsEnableOwnerDraw() const;

		// For grid line
		;
		int ShowGridLine(BOOL bShow);
		BOOL IsGridLineVisible();

		int DrawFirstRow(BOOL bDraw0Row);
		BOOL IsDrawFirstRow() const;

		int SetGridLineOffset(const LPPOINT lpPtOffset);
		int GetGridLineOffset(LPPOINT lpPtOffset) const;

		int SetGridLineColor(COLORREF crGridLine);
		COLORREF GetGridLineColor() const;

		// For normal item
		;
		int EnableNormalItemEffect(BOOL bEnable);
		BOOL IsEnableNormalItemEffect() const;

		int SetNormalItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetNormalItemBkType() const;

		int SetNormalItemBkColor(COLORREF cr);
		COLORREF GetNormalItemBkColor() const;

		int SetNormalItemBkImages(LPCTSTR lpszImageNameItemBk);
		int GetNormalItemBkImages(CString *pstrImageNameItemBk) const;

		int SetNormalItemTextColor(COLORREF cr);
		COLORREF GetNormalItemTextColor() const;

		// For highlight item
		;
		int EnableHighlightItemEffect(BOOL bEnable);
		BOOL IsEnableHighlightItemEffect() const;

		int SetHighlightItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetHighlightItemBkType() const;

		int SetHighlightItemBkColor(COLORREF cr, BOOL bRedraw = TRUE);
		COLORREF GetHighlightItemBkColor() const;

		int SetHighlightItemBkImages(LPCTSTR lpszImageNameBk);
		int GetHighlightItemBkImages(CString *pstrImageNameBk);

		int SetHighlightItemTextColor(COLORREF cr);
		COLORREF GetHighlightItemTextColor() const;

		int GetHighlightItem() const;

		// For selected item
		// draw selected background fit to the width of client, not to the right of last header item.
		;
		int EnableSelectedFitClient(BOOL bEnable);
		BOOL IsEnableSelectedFitClient() const;

		int SetSelectedItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetSelectedItemBkType() const;

		int SetSelectedItemBkColor(COLORREF cr, BOOL bRedraw = TRUE);
		COLORREF GetSelectedItemBkColor() const;

		int SetSelectedItemBkImages(LPCTSTR lpszImageNameBk);
		int GetSelectedItemBkImages(CString *pstrImageNameBk);

		int SetSelectedItemTextColor(COLORREF cr);
		COLORREF GetSelectedItemTextColor() const;

		// For normal disabled item
		;
		int EnableNormalDisabledItemEffect(BOOL bEnable);
		BOOL IsEnableNormalDisabledItemEffect() const;

		int SetNormalDisabledItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetNormalDisabledItemBkType() const;

		int SetNormalDisabledItemBkColor(COLORREF cr);
		COLORREF GetNormalDisabledItemBkColor() const;

		int SetNormalDisabledItemBkImages(LPCTSTR lpszImageNameBk);
		int GetNormalDisabledItemBkImages(CString *pstrImageNameBk);

		int SetNormalDisabledItemTextColor(COLORREF cr);
		COLORREF GetNormalDisabledItemTextColor() const;

		// For selected disabled item
		;
		int EnableSelectedDisabledItemEffect(BOOL bEnable);
		BOOL IsEnableSelectedDisabledItemEffect() const;

		int SetSelectedDisabledItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetSelectedDisabledItemBkType() const;

		int SetSelectedDisabledItemBkColor(COLORREF cr);
		COLORREF GetSelectedDisabledItemBkColor() const;

		int SetSelectedDisabledItemBkImages(LPCTSTR lpszImageNameBk);
		int GetSelectedDisabledItemBkImages(CString *pstrImageNameBk);

		int SetSelectedDisabledItemTextColor(COLORREF cr);
		COLORREF GetSelectedDisabledItemTextColor() const;

		// For text font
		;
		int SetItemTextFont(LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDH, LPCTSTR lpszFontIDS, LPCTSTR lpszFontIDND, LPCTSTR lpszFontIDSD);
		int GetItemTextFont(CString *pstrFontIDN, CString *pstrFontIDH, CString *pstrFontIDS, CString *pstrFontIDND, CString *pstrFontIDSD) const;

		// For thumbnail view
		;
		int SetThumbnailView(BOOL bThumbnailView);
		BOOL IsThumbnailView() const;

		int SetThumbnailItemTotalSize(SIZE sizeItem);
		int GetThumbnailItemTotalSize(LPSIZE lpSizeItem) const;

		int SetThumbnailItemBkPosition(LPCRECT lprcPositionN, LPCRECT lprcPositionH, LPCRECT lprcPositionS, LPCRECT lprcPositionD);
		int GetThumbnailItemBkPosition(LPRECT lprcPositionN, LPRECT lprcPositionH, LPRECT lprcPositionS, LPRECT lprcPositionD) const;

		int SetThumbnailItemPosition(LPCRECT lprcPositionN, LPCRECT lprcPositionH, LPCRECT lprcPositionS, LPCRECT lprcPositionD);
		int GetThumbnailItemPosition(LPRECT lprcPositionN, LPRECT lprcPositionH, LPRECT lprcPositionS, LPRECT lprcPositionD) const;

		int SetThumbnailItemLabelPosition(LPCRECT lprcPositionN, LPCRECT lprcPositionH, LPCRECT lprcPositionS, LPCRECT lprcPositionD);
		int GetThumbnailItemLabelPosition(LPRECT lprcPositionN, LPRECT lprcPositionH, LPRECT lprcPositionS, LPRECT lprcPositionD) const;

		int SetThumbnailItemIconPosition(LPCRECT lprcPositionN, LPCRECT lprcPositionH, LPCRECT lprcPositionS, LPCRECT lprcPositionD);
		int GetThumbnailItemIconPosition(LPRECT lprcPositionN, LPRECT lprcPositionH, LPRECT lprcPositionS, LPRECT lprcPositionD) const;

		int GetThumbnailItemTotalRect(int nItem, LPRECT lpRectTotal) const;
		int GetThumbnailItemRect(int nItem, LPRECT lpRect, UINT nCode, CONTROL_STATE eControlState) const;

		// Tag
		;
		int SetTag(LPCTSTR lpszTag);
		int GetTag(CString &strTag) const;

		// for custom scroll bar
		;
		int UseCustomScrollBar(BOOL bUseCustomScrollBar);
		BOOL IsUseCustomScrollBar() const;

		CScrollBarEx *GetHorzScrollBarEx() const;
		CScrollBarEx *GetVertScrollBarEx() const;

		int UpdateCustomScrollBar();

		// Animation mode
		int AnimateReportContent(std::vector<int> &vAnimationData, int nElapse/*ms*/, BOOL bTogetherAnimate);

		// hide the same name interface in base class
	private:
		void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
		CFont *GetFont() const;

		// Operations
	public:

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinListCtrl)
	public:
		virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pLResult);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL
#if(_MSC_VER <= 1200) // VC6.0
		virtual int OnToolHitTest(CPoint point, TOOLINFO *pTI) const;
#else
		virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO *pTI) const;
#endif

	protected:
		virtual int OnDrawBackground(CDC *pDC);
		virtual int OnDrawGridLine(CDC *pDC);
		virtual int OnDrawItemEx(CDC *pDC, int nItem, int nLeftMargin = 0);
		virtual int OnDrawReportItem(CDC *pDC, int nItem, const CRect &rcItem, int nLeftMargin = 0);
		virtual int OnDrawIconItem(CDC *pDC, int nItem, const CRect &rcItem);
		virtual int OnDrawThumbnailItem(CDC *pDC, int nItem, const CRect &rcItem);

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinListCtrl)
		afx_msg BOOL OnSetTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////

#define TVM_DRAW_BACKGROUND					WM_TREECTRL+3		// WPARAM: CDC *; LPARAM: not use, must be 0.
#define TVM_DRAW_ITEM						WM_TREECTRL+5		// WPARAM: CDC *; LPARAM: item handle

	class CSkinTreeCtrl : public CTreeCtrl, public CControlBase
	{
		// Construction
	public:
		CSkinTreeCtrl();
		virtual ~CSkinTreeCtrl();

		virtual BOOL Create(__in DWORD dwStyle, __in const RECT &rect, __in CWnd *pParentWnd, __in UINT nID);
		virtual BOOL Create(__in DWORD dwStyle, __in CRectCtrl *pRectCtrl, __in CWnd *pParentWnd, __in UINT nID);

		// Init control by specify style.
		;
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	public:
		int ReDrawControl(BOOL bErase, BOOL bUpdateScrollBar, LPCRECT lpInvalidateRect = NULL);
		int UpdateImageListCache();

	public:
		int SetImages(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkD);
		int GetImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameBkN, CString *pstrImageNameBkD);

		int SetTextAlignHor(
			TEXT_ALIGN_HOR eHorAlignModeN,
			TEXT_ALIGN_HOR eHorAlignModeH,
			TEXT_ALIGN_HOR eHorAlignModeS,
			TEXT_ALIGN_HOR eHorAlignModeD,
			BOOL bRedraw = TRUE);

		int GetTextAlignHor(
			TEXT_ALIGN_HOR *peHorAlignModeN,
			TEXT_ALIGN_HOR *peHorAlignModeH,
			TEXT_ALIGN_HOR *peHorAlignModeS,
			TEXT_ALIGN_HOR *peHorAlignModeD);

		int SetTextAlignVer(
			TEXT_ALIGN_VER eVerAlignModeN,
			TEXT_ALIGN_VER eVerAlignModeH,
			TEXT_ALIGN_VER eVerAlignModeS,
			TEXT_ALIGN_VER eVerAlignModeD,
			BOOL bRedraw = TRUE);

		int GetTextAlignVer(
			TEXT_ALIGN_VER *peVerAlignModeN,
			TEXT_ALIGN_VER *peVerAlignModeH,
			TEXT_ALIGN_VER *peVerAlignModeS,
			TEXT_ALIGN_VER *peVerAlignModeD);

		int SetTextMargin(
			LPCRECT lprcTextMarginN,
			LPCRECT lprcTextMarginH,
			LPCRECT lprcTextMarginS,
			LPCRECT lprcTextMarginD,
			BOOL bRedraw = TRUE);

		int GetTextMargin(
			LPRECT lprcTextMarginN,
			LPRECT lprcTextMarginH,
			LPRECT lprcTextMarginS,
			LPRECT lprcTextMarginD) const;

		// For item line
		;
		int EnableItemLine(BOOL bEnable);
		BOOL IsEnableItemLine() const;

		int SetItemLineColor(COLORREF cr);
		COLORREF GetItemLineColor() const;

		// For Collapse item image
		;
		int SetCombineCollapseExpandedImage(LPCTSTR lpszImage);

		int SetCollapseImage(UINT uMask,
			LPCTSTR lpszImageNameCollapseN, LPCTSTR lpszImageNameCollapseH,
			LPCTSTR lpszImageNameCollapseS, LPCTSTR lpszImageNameCollapseD);

		int GetCollapseImage(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD) const;

		// For Expand item image
		;
		int SetExpandedImage(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		int GetExpandedImage(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD) const;

		// For root item
		;
		int EnableRootItemEffect(BOOL bEnable);
		BOOL IsRootItemEffectEnabled() const;

		int SetRootItemTextColor(COLORREF cr);
		COLORREF GetRootItemTextColor() const;

		int SetRootItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetRootItemBkType() const;

		int SetRootItemBkColor(COLORREF cr);
		COLORREF GetRootItemBkColor() const;

		int SetRootItemBkImages(UINT uMask, LPCTSTR lpszImageNameBkFocus, LPCTSTR lpszImageNameBkNoFocus);
		int GetRootItemBkImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameBkFocus, CString *pstrImageNameBkNoFocus);

		// for highlight item
		;
		int EnableHighlightItemEffect(BOOL bEnable);
		BOOL IsHighlightItemEffectEnabled() const;
		HTREEITEM GetHighlightItem() const;

		int SetHighlightItemBkColor(COLORREF cr);
		COLORREF GetHighlightItemBkColor() const;

		int SetHighlightItemBkImages(UINT uMask, LPCTSTR lpszImageNameBkFocus, LPCTSTR lpszImageNameBkNoFocus);
		int GetHighlightItemBkImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameBkFocus, CString *pstrImageNameBkNoFocus);

		int SetHighlightItemTextColor(COLORREF cr);
		COLORREF GetHighlightItemTextColor() const;

		// for selected item
		;
		int SetSelectedItemBkColor(COLORREF cr);
		COLORREF GetSelectedItemBkColor() const;

		int SetSelectedItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetSelectedItemBkType() const;

		int SetSelectedItemBkImages(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkD);
		int GetSelectedItemBkImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameBkN, CString *pstrImageNameBkD);

		int SetSelectedItemTextColor(COLORREF cr);
		COLORREF GetSelectedItemTextColor() const;

		// for disable text color
		;
		int SetDisabledItemTextColor(COLORREF cr);
		COLORREF GetDisabledItemTextColor() const;

		// Insert a windowless control to a report list sub item.
		// The report list like a CGridPanel.
		SItemBindObject *SetItemControl(HTREEITEM hItem, CWLWnd *pWLItem);
		SItemBindObject *SetItemControl(HTREEITEM hItem, UINT uBindWndID);

		int ReleaseItemControl(HTREEITEM hItem);

		// Get the child of the item in item template.
		CWLWnd *GetItemTemplateItem(HTREEITEM hItem, UINT uItemID);

		int SetTag(LPCTSTR lpszTag);
		int GetTag(CString &strTag) const;

		// for mouse hover
		;
		int SetHoverTime(DWORD dwHoverTime, BOOL bRepeat = FALSE);
		int GetHoverTime(DWORD *pdwHoverTime, BOOL *pbRepeat) const;

		// for custom scroll bar
		;
		int UseCustomScrollBar(BOOL bUseCustomScrollBar);
		BOOL IsUseCustomScrollBar() const;

		int SetScrollBarWidth(LONG lScrollBarWidth);
		LONG GetScrollBarWidth() const;

		CScrollBarEx *GetHorzScrollBarEx() const;
		CScrollBarEx *GetVertScrollBarEx() const;

		int UpdateCustomScrollBar();

		int SetHoverScrollBarMode(BOOL bHoverShow);
		BOOL IsHoverScrollBarMode() const;

		int SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD = NULL);
		int GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD = NULL) const;

		int SetItemHeightEx(HTREEITEM hItem, UINT cyItemHeight);
		UINT GetItemHeightEx(HTREEITEM hItem) const;

		int GetChildrenCount(HTREEITEM hItem) const;

		int CollapseAll();

		// for drag & drop
		int RegisterDropTarget();

		// Operations
	public:

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinTreeCtrl)
	public:
		virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pLResult);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinTreeCtrl)
		afx_msg void OnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		virtual int OnDrawBackground(CDC *pDC);
		virtual int OnDrawItem(CDC *pDC, HTREEITEM hItem, LPCRECT lprcItem, int nLeftMargin = 0);
		virtual int OnDrawItemBk(CDC *pDC, HTREEITEM hItem, const CRect &rcItem);
		virtual int OnDrawItemLine(CDC *pDC, HTREEITEM hItem, const CRect &rcItem);
		virtual int OnDrawItemIcon(CDC *pDC, HTREEITEM hItem, const CRect &rcItem, const CRect &rcText, int nLeftMargin = 0);
		virtual int OnDrawText(CDC *pDC, HTREEITEM hItem, const CRect &rcItem, const CRect &rcText, int nLeftMargin = 0);

	private:
		void *m_pMember;
	};


#if(_MSC_VER >= 1500) // >VC2008

	//////////////////////////////////////////////////////////////////////////
	// Ole drag & drop

	// COleDropSourceEx results
#define DRAG_RES_NOT_STARTED		0		// drag not started
#define DRAG_RES_STARTED			1		// drag started (timeout expired or mouse left the start rect)
#define DRAG_RES_CANCELLED			2		// cancelled (may be started or not)
#define DRAG_RES_RELEASED			4		// left mouse button released (may be started or not)
#define DRAG_RES_DROPPED			8		// data has been dropped (return effect not DROPEEFECT_NONE)

	// Vista: DROPDESCRIPTION and FILE_ATTRIBUTES_ARRAY (ShlObj.h)
#if !defined(NTDDI_VERSION) || (NTDDI_VERSION < NTDDI_VISTA)

	// Vista: DROPDESCRIPTION type member
	// NOTE: DROPIMAGE_NOIMAGE has been added with Windows 7
	// Define the image at left of drop description.
	// See also : https://msdn.microsoft.com/en-us/library/windows/desktop/bb762498(v=vs.85).aspx
	typedef enum
	{
		DROPIMAGE_INVALID             = -1,                // no image preference (use default)
		DROPIMAGE_NONE                = 0,                 // red "no" circle
		DROPIMAGE_COPY                = DROPEFFECT_COPY,   // plus for copy
		DROPIMAGE_MOVE                = DROPEFFECT_MOVE,   // movement arrow for move
		DROPIMAGE_LINK                = DROPEFFECT_LINK,   // link arrow for link
		DROPIMAGE_LABEL               = 6,                 // tag icon to indicate metadata will be changed
		DROPIMAGE_WARNING             = 7,                 // yellow exclamation, something is amiss with the operation
		DROPIMAGE_NOIMAGE             = 8,                 // no image at all
	} DROPIMAGETYPE;

#endif
	class CDragDropHelper
	{
	public:
		static inline CLIPFORMAT RegisterFormat(LPCTSTR lpszFormat)
		{
			return static_cast<CLIPFORMAT>(::RegisterClipboardFormat(lpszFormat));
		}

		static unsigned PopCount(unsigned n);
		static bool		IsRegisteredFormat(CLIPFORMAT cfFormat, LPCTSTR lpszFormat, bool bRegister = false);
		static HGLOBAL	CopyGlobalMemory(HGLOBAL hDst, HGLOBAL hSrc, size_t nSize);
		static bool		GetGlobalData(LPDATAOBJECT pIDataObj, LPCTSTR lpszFormat, FORMATETC &FormatEtc, STGMEDIUM &StgMedium);
		static DWORD	GetGlobalDataDWord(LPDATAOBJECT pIDataObj, LPCTSTR lpszFormat);
		static bool		SetGlobalDataDWord(LPDATAOBJECT pIDataObj, LPCTSTR lpszFormat, DWORD dwData, bool bForce = true);
		static DROPIMAGETYPE DropEffectToDropImage(DROPEFFECT dwEffect);
		static bool		ClearDescription(DROPDESCRIPTION *pDropDescription);
		static size_t	GetWideLength(LPCWSTR lpszText, size_t nSize);
		static size_t	GetMultiByteLength(LPCSTR lpszText, size_t nSize);
		static LPWSTR	MultiByteToWideChar(LPCSTR lpszAnsi, int nLen, UINT nCP = CP_THREAD_ACP);
		static LPSTR	WideCharToMultiByte(LPCWSTR lpszWide, int nLen, UINT nCP = CP_THREAD_ACP);
		static UINT		GetCodePage(LCID nLCID, bool bOem);
#ifndef _UNICODE
		static bool		CompareCodePages(UINT nCP1, UINT nCP2);
		static LPSTR	MultiByteToMultiByte(LPCSTR lpszAnsi, int nLen, UINT nCP, UINT nOutCP = CP_THREAD_ACP);
#endif
	};

	class CDropDescription
	{
	public:
		CDropDescription();
		virtual ~CDropDescription();

		// Max. image type stored by this class.
		static const DROPIMAGETYPE nMaxDropImage = DROPIMAGE_WARNING;

		inline bool IsValidType(DROPIMAGETYPE nType) const
		{
			return (nType >= DROPIMAGE_NONE && nType <= nMaxDropImage);
		}

		bool	SetInsert(LPCWSTR lpszInsert);
		bool	SetText(DROPIMAGETYPE nType, LPCWSTR lpszText, LPCWSTR lpszText1);
		LPCWSTR	GetText(DROPIMAGETYPE nType, bool b1) const;
		bool	SetDescription(DROPDESCRIPTION *pDropDescription, DROPIMAGETYPE nType) const;
		bool	SetDescription(DROPDESCRIPTION *pDropDescription, LPCWSTR lpszMsg) const;
		bool	CopyText(DROPDESCRIPTION *pDropDescription, DROPIMAGETYPE nType) const;
		bool	CopyMessage(DROPDESCRIPTION *pDropDescription, LPCWSTR lpszMsg) const;
		bool	CopyInsert(DROPDESCRIPTION *pDropDescription, LPCWSTR lpszInsert) const;

		inline LPCWSTR GetInsert() const
		{
			return m_lpszInsert;
		}
		inline bool	CopyInsert(DROPDESCRIPTION *pDropDescription) const
		{
			return CopyInsert(pDropDescription, m_lpszInsert);
		}

		// Check if text has been stored for image type.
		inline bool HasText(DROPIMAGETYPE nType) const
		{
			return IsValidType(nType) && NULL != m_pDescriptions[nType];
		}

		// Check if text for image type is empty.
		inline bool IsTextEmpty(DROPIMAGETYPE nType) const
		{
			return !HasText(nType) || L'\0' == m_pDescriptions[nType][0];
		}


		inline bool HasInsert() const
		{
			return NULL != m_lpszInsert;
		}
		inline bool IsInsertEmpty() const
		{
			return !HasInsert() || L'\0' == *m_lpszInsert;
		}

		inline bool HasInsert(const DROPDESCRIPTION *pDropDescription) const
		{
			return pDropDescription && pDropDescription->szInsert[0] != L'\0';
		}

		inline bool	ClearDescription(DROPDESCRIPTION *pDropDescription) const
		{
			return CDragDropHelper::ClearDescription(pDropDescription);
		}

	protected:
		bool CopyDescription(LPWSTR lpszDest, size_t nDstSize, LPCWSTR lpszSrc) const;

		LPWSTR m_lpszInsert;
		LPWSTR m_pDescriptions[nMaxDropImage + 1];
		LPWSTR m_pDescriptions1[nMaxDropImage + 1];
	};


	class CIUIOleDropSource : public COleDropSource
	{
		friend class CIUIOleDataSource;

	public:
		CIUIOleDropSource();

	protected:
		BOOL SetDragImageCursor(DROPEFFECT dwEffect);

		virtual BOOL OnBeginDrag(CWnd *pWnd);
		virtual SCODE QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState);
		virtual SCODE GiveFeedback(DROPEFFECT dropEffect);

	protected:
		BOOL m_bSetCursor;
		int m_nResult;						// drag result flags
		LPDATAOBJECT m_pIDataObj;						// point to COleDataSource's IDataObject
		const CDropDescription *m_pDropDescription;		// set by COleDataSourceEx to its CDropDescription member
	};

	class CIUIOleDataSource : public COleDataSource
	{
	public:
		CIUIOleDataSource();
		~CIUIOleDataSource();

	public:
		// Set Drop Description
		int AllowShowDropDescription();
		BOOL IsAllowShowDropDescription();

		virtual int SetDropDescription(DROPDESCRIPTION *pdd);
		virtual int SetDropDescription(DROPIMAGETYPE type, WCHAR *pwszMessage, WCHAR *pwszInsert);

		// Set drag image
		HRESULT SetDragImageFromBitmap(HBITMAP hBitmap, const CPoint &point = CPoint(0, 0), COLORREF crColorKey = GetSysColor(COLOR_WINDOW));
		HRESULT SetDragImageFromWindow(HWND hwnd, const CPoint &point = CPoint(0, 0));

		DROPEFFECT DoDragDropEx(DROPEFFECT dwEffect, LPCRECT lpRectStartDrag = NULL);

	protected:
		IDragSourceHelper2 *m_pDragSourceHelper2;	// IDragSourceHelper2 valid only on Windows Vista or later
		IDragSourceHelper *m_pDragSourceHelper;
		CDropDescription	m_DropDescription;			// Optional text descriptions
		BOOL m_bAllowShowDropDescription;
		BOOL m_bHasSetDropDescription;

	public:
		BEGIN_INTERFACE_PART(DataObj, IDataObject)
		INIT_INTERFACE_PART(CIUIOleDataSource, DataObject)
		// IDataObject
		STDMETHOD(GetData)(LPFORMATETC, LPSTGMEDIUM);
		STDMETHOD(GetDataHere)(LPFORMATETC, LPSTGMEDIUM);
		STDMETHOD(QueryGetData)(LPFORMATETC);
		STDMETHOD(GetCanonicalFormatEtc)(LPFORMATETC, LPFORMATETC);
		STDMETHOD(SetData)(LPFORMATETC, LPSTGMEDIUM, BOOL);
		STDMETHOD(EnumFormatEtc)(DWORD, LPENUMFORMATETC *);
		STDMETHOD(DAdvise)(LPFORMATETC, DWORD, LPADVISESINK, LPDWORD);
		STDMETHOD(DUnadvise)(DWORD);
		STDMETHOD(EnumDAdvise)(LPENUMSTATDATA *);
		END_INTERFACE_PART(DataObj)

		DECLARE_INTERFACE_MAP()
	};

#endif //(_MSC_VER >= 1500) // VC2008

	// CIUIOleDropTarget member functions params
	class CIUIDropMsgParam : public CObject		// CIUIDropMsgParam is the old class COleDropInfo
	{
	public:
		CIUIDropMsgParam();
		CIUIDropMsgParam(COleDataObject *pDataObject, DWORD dwKeyState, DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point);
		virtual ~CIUIDropMsgParam();

	public:
		COleDataObject *m_pDataObject;
		DWORD m_dwKeyState;
		DROPEFFECT m_dropEffect;
		DROPEFFECT m_dropEffectList;
		CPoint m_point;
	};

	class CIUIOleDropTarget : public COleDropTarget			// CIUIOleDropTarget is the old class COleDropTargetEx
	{
	public:
		CIUIOleDropTarget();
		virtual ~CIUIOleDropTarget();

	public:
		HWND GetSafeHwnd();

#if (_MSC_VER >= 1500) // vs2008
		bool SetDropDescriptionText(DROPIMAGETYPE nType, LPCWSTR lpszText, LPCWSTR lpszText1, LPCWSTR lpszInsert = NULL)
		{
			bool bRet = false;
			if (/*m_bCanShowDescription*/1)						// Vista or later and themes are enabled
			{
				bRet = m_DropDescription.SetText(nType, lpszText, lpszText1);
				if (bRet && lpszInsert)
				{
					m_DropDescription.SetInsert(lpszInsert);
				}
			}
			return bRet;
		}
		// Set default drop description insert string.
		bool SetDropInsertText(LPCWSTR lpszText)
		{
			return m_DropDescription.SetInsert(lpszText);
		}
#endif //(_MSC_VER >= 1500) // VC2008

	protected:
		virtual DROPEFFECT OnDragEnter(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
		virtual DROPEFFECT OnDragOver(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
		virtual BOOL OnDrop(CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point);
		virtual DROPEFFECT OnDropEx(CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point);
		virtual void OnDragLeave(CWnd *pWnd);
		virtual DROPEFFECT OnDragScroll(CWnd *pWnd, DWORD dwKeyState, CPoint point);

	protected:
		// 需要_WIN32_IE >= 0x0500
		IDropTargetHelper *m_pDropTargetHelper;


#if (_MSC_VER >= 1500) // vs2008
		CDropDescription m_DropDescription;			// User defined default drop description text
#endif //(_MSC_VER >= 1500) // VC2008
	};

	/////////////////////////////////////////////////////////////////////////////
	// CThumbnailCtrl window

#ifdef _WIN32
#define WC_THUMBNAILVIEWA            "SysThumbnailView32"
#define WC_THUMBNAILVIEWW            L"SysThumbnailView32"

#ifdef UNICODE
#define  WC_THUMBNAILVIEW            WC_THUMBNAILVIEWW
#else
#define  WC_THUMBNAILVIEW            WC_THUMBNAILVIEWA
#endif
#else
#define WC_THUMBNAILVIEW             "SysThumbnailView"
#endif

#define LVN_ENDDRAG                  1

	class CThumbnailCtrl : public CWnd, public CControlBase
	{
		// Construction
	public:
		CThumbnailCtrl();
		virtual ~CThumbnailCtrl();
		BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);

	public:
		// must be call after the object is created and before the window is created
		BOOL SetItemBitmap(LPCTSTR lpszNormal, LPCTSTR lpszHighlight, LPCTSTR lpszSelected, LPCTSTR lpszDisabled = NULL);
		BOOL SetItemBitmap(HBITMAP hItemBkN, HBITMAP hItemBkH, HBITMAP hItemBkS, HBITMAP hItemBkD = NULL);

		//
		int GetItemBitmap(HBITMAP *phItemBKN, HBITMAP *phItemBKH, HBITMAP *phItemBKS, HBITMAP *phItemBKD = NULL) const;

		// must be call after the object is created and before the window is created.
		// the total size is one image take up, include all space, from one item's left side to another item's left side.
		// this interface only support non-list style
		BOOL SetItemSize(CSize sizeTotal);

		// must be call after the object is created.
		BOOL SetItemPosition(LPCRECT lprcThumbnail, LPCRECT lprcHLThumbnail, LPCRECT lprcLabel, LPCRECT lprcHLLabel);
		BOOL SetLinesPerItem(int nLines);

		//
		int GetItemPosition(LPRECT lprcThumbnail, LPRECT lprcHLThumbnail, LPRECT lprcLabel, LPRECT lprcHLLabel) const;

		// if necessary, call before the CThumbnailCtrlEx object is created
		void SetScrollBarWidth(int nWidth);

		int SetItemHeight(int nItemHeight);
		int GetItemHeight() const;

		// Attributes
	public:
		void ClientToWindow(LPRECT lpRect) const;
		void WindowToClient(LPRECT lpRect) const;

		int GetFocusItem();
		int SetFocusItem(int nItem);

		void GetScrollBarRect(CRect &rcScrollBar) const;
		void EnableDragDrop(BOOL bEnable = TRUE);// Enable Drag & Drop item in the view, muse call 	AfxOleInit() when app is init.
		void EnableScrollBarCtrl(int nBar, BOOL bEnable = TRUE);

		POSITION GetFirstSelectedItemPosition() const; //
		int GetNextSelectedItem(POSITION &pos);//
		int GetFirstVisibleItem(BOOL bPartialOK = TRUE);
		int GetLastVisibleItem(BOOL bPartialOK = TRUE);
		int GetItemCount() const;
		int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;
		int GetNextItem(int nItem, int nFlags);
		int GetHotItem() const;
		int GetTopIndex(); // only valid with list style
		UINT GetItemState(int nItem, UINT nMask) const;
		UINT GetSelectedCount();
		BOOL GetItem(LVITEM *pItem) const;
		BOOL GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const; // relative to the pos return by GetViewRect
		HBITMAP GetItemThumbnail(int nItem);
		CString GetItemTooltip(int nItem) const;
		BOOL GetViewRect(LPRECT lpRect) const;
		CString GetItemText(int nItem, int nSubItem);
#if(_MSC_VER <= 1200) // VC6.0
		DWORD GetItemData(int nItem);
#else
		DWORD_PTR GetItemData(int nItem);
#endif
		COLORREF GetBkColor() const;
		COLORREF GetTextColor() const;
		int GetCountPerPage() const;

		BOOL SetBkColor(COLORREF cr);

		int GetBkImage(CString *pstrImageNameBk);
		BOOL SetBkImage(LPCTSTR lpszImageNameBk, BOOL fTile = TRUE);

		BOOL SetItem(const LVITEM *pItem);
		BOOL SetItemState(int nItem, LVITEM *pItem);
		BOOL SetItemState(int nItem, UINT nState, UINT nMask);
		BOOL SetItemData(int nItem, DWORD dwData);
		BOOL SetItemText(int nItem, int nSubItem, LPTSTR lpszText, BOOL bRedraw = TRUE);
		BOOL SetItemFont(const LOGFONT *lfItem);
		BOOL GetItemFont(LOGFONT *lfItem) const;
		BOOL SetTextColor(COLORREF cr, BOOL bRedraw = TRUE);
		BOOL SetItemThumbnail(int nItem, HBITMAP hbmpThumbnail, BOOL bDeleteOld = TRUE, BOOL bRedraw = TRUE);
		BOOL SetItemTooltip(int nItem, LPCTSTR lpszTooltip);
		void SetShiftStart(int nShiftStart);

		// for CScrollBarEx's background
		CScrollBarEx *GetScrollBarEx();

		void SetScrollBarExBk(UINT uMask,
			LPCTSTR lpszImageNameScrollbarExN, LPCTSTR lpszImageNameScrollbarExH,
			LPCTSTR lpszImageNameScrollbarExS, LPCTSTR lpszImageNameScrollbarExD);

		void SetScrollBarExThumb(UINT uMask,
			LPCTSTR lpszImageNameScrollbarExThumbN,
			LPCTSTR lpszImageNameScrollbarExThumbH,
			LPCTSTR lpszImageNameScrollbarExThumbS,
			LPCTSTR lpszImageNameScrollbarExThumbD);
		void GetScrollBarExThumb(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameScrollbarExThumbN, CString *pstrImageNameScrollbarExThumbH,
			CString *pstrImageNameScrollbarExThumbS, CString *pstrImageNameScrollbarExThumbD);

		void SetScrollBarExLTUPButton(int nValue);
		int GetScrollBarExLTUPButton();

		void SetScrollBarExRTDNButton(int nValue);
		int GetScrollBarExRTDNButton();

		int HoldCtrlKey(BOOL bHold);
		BOOL IsHoldCtrlKey();

		// Operations
	public:
		void MoveItem(int nDest, int nSrc);
		BOOL DeleteSelectedItem();
		int  InsertItem(const LVITEM *pItem);
		int  InsertItem(int nItem, LPCTSTR lpszItem);
		int  InsertItem(int nItem, LPCTSTR lpszItem, HBITMAP hThumbnail);
		int  InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
		BOOL DeleteAllItems();
		BOOL DeleteItem(int nItem);
		BOOL EnsureVisible(int nItem, BOOL bPartialOK = TRUE);
		BOOL SelectAll(BOOL bSelected = TRUE);

		int HitTestDrop(CPoint point);
		int HitTest(CPoint pt, UINT *pFlags = NULL) const;
		void RefreshDisplay(BOOL bUpdateImmediately = TRUE);

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CThumbnailCtrl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg);
		//}}AFX_VIRTUAL
#if(_MSC_VER <= 1200) // VC6.0
		virtual int OnToolHitTest(CPoint point, TOOLINFO *pTI) const;
#else
		virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO *pTI) const;
#endif


		//Operations
	protected:
		int GetSpace() const;
		void UpdateScrollingDisplay();
		BOOL IsValidIndex(int nIndex) const;
		void SelectByRect(LPRECT rcSel, BOOL bClear = TRUE);
		BOOL DoLButtonDown(UINT nFlags, CPoint point, BOOL bIsLButtonDown = TRUE);
		void DoMultipleSelection(BOOL bShift, BOOL bCtrl, int nPos, BOOL bIsLButtonDown = TRUE);
		CSize CalcThumbnailGrid() const;
		CDC *BeginRepaint();
		void EndRepaint(CDC *pDC);
		void BeginDrag(UINT nFlags, CPoint point);
		void EndDragSelect(POINT pt);
		BOOL OnMovementKey(UINT nChar);

		// Generated message map functions
	protected:
		virtual void OnDraw(CDC *pDC);
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

		//{{AFX_MSG(CThumbnailCtrl)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		//}}AFX_MSG
		afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	/////////////////////////////////////////////////////////////////////////////
	// CTreeListCtrl window

#ifdef _WIN32
#define WC_TREELISTVIEWA            "SysTreeListView32"
#define WC_TREELISTVIEWW            L"SysTreeListView32"

#ifdef UNICODE
#define  WC_TREELISTVIEW            WC_TREELISTVIEWW
#else
#define  WC_TREELISTVIEW            WC_TREELISTVIEWA
#endif

#else
#define WC_TREELISTVIEW             "SysTreeListView"
#endif


	// Tree control's extended style
#define TVS_EX_GRIDLINES            0x00000001
#define TVS_EX_BKLINE               0x00000002
#define TVS_EX_OWNERDRAWFIXED       0x00000004

	// Height of Header control
#define IDW_TREELIST                1
#define IDW_TREELISTHEADER          0

	class CTreeList : public CTreeCtrl, public CControlBase
	{
		// Construction
	public:
		CTreeList();
		virtual ~CTreeList();

	public:

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CTreeList)
	protected:
		;
		virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL
		;
	public:
		int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);

		BOOL SetItemTextEx(HTREEITEM hTI, int nCol, LPCTSTR lpszText);
		CString GetItemTextEx(HTREEITEM hTI, int nCol) const;

		int SetHeaderPtr(CSkinHeaderCtrl *pHeader);
		CSkinHeaderCtrl *GetHeaderCtrl();

		BOOL SetBkImage(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkD);
		int GetBkImage(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameBkN, CString *pstrImageNameBkD);

		int SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD = NULL);
		int GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD = NULL) const;

		int SetHoverTime(DWORD dwHoverTime, BOOL bRepeat = FALSE);
		int GetHoverTime(DWORD *pdwHoverTime, BOOL *pbRepeat) const;

		int SetItemHeightEx(HTREEITEM hItem, UINT cyItemHeight);
		UINT GetItemHeightEx(HTREEITEM hItem) const;

		int ListIndexByHTREEITEM(HTREEITEM hTI) const;

		CListCtrl *GetListCtrl();

		// hide the same name interface in base class
	private:
		// use SetTextFont
		void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
		// use GetTextFont
		CFont *GetFont() const;
		// use SetItemTextEx
		BOOL SetItemText(HTREEITEM hItem, LPCTSTR lpszItem);
		// use GetItemTextEx
		CString GetItemText(HTREEITEM hItem) const;

		// Generated message map functions
	protected:
		//{{AFX_MSG(CTreeList)
		;
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnPaint();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
		virtual int DrawItem(CDC *pDC, HTREEITEM hTI);

	private:
		void *m_pMember;
	};

	class CTreeListCtrl : public CWnd, public CControlBase
	{
		friend class CTreeList;
	public:
		CTreeListCtrl();
		virtual ~CTreeListCtrl();
		BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, int nHeaderHeight = 24, BOOL bUseCustomScroll = FALSE, int nScrollWidth = 16);

		// Init control by specify style.
		;
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	public:

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CTreeListCtrl)
	protected:
		;
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult);
		//}}AFX_VIRTUAL

		// Implementation
		;
	public:
		CTreeList *GetTreeListWnd();
		CSkinHeaderCtrl *GetHeaderCtrl();
		CScrollBarEx *GetScrollBarEx();
		BOOL IsUseScrollBarEx();

		HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
		int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
		BOOL SetBkImage(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkD);
		int GetBkImage(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameBkN, CString *pstrImageNameBkD);

		int SetScrollBarExBk(UINT uMask,
			LPCTSTR lpszImageNameScrollbarExN, LPCTSTR lpszImageNameScrollbarExH,
			LPCTSTR lpszImageNameScrollbarExS, LPCTSTR lpszImageNameScrollbarExD);
		int SetScrollBarExThumb(UINT uMask,
			LPCTSTR lpszImageNameScrollbarExThumbN, LPCTSTR lpszImageNameScrollbarExThumbH,
			LPCTSTR lpszImageNameScrollbarExThumbS, LPCTSTR lpszImageNameScrollbarExThumbD);
		int GetScrollBarExThumb(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameScrollbarExThumbN, CString *pstrImageNameScrollbarExThumbH,
			CString *pstrImageNameScrollbarExThumbS, CString *pstrImageNameScrollbarExThumbD);

		SHORT SetItemHeight(SHORT cyHeight);
		SHORT GetItemHeight() const;

		int UpdateScrollingDisplay();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CTreeListCtrl)
		;
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()


	private:
		void *m_pMember;
	};


	// The base class of CSkinRichEditCtrl and CWLRichEditCtrl
	class CTextHost;
	class CImageOleCtrl;
	class CRichEditCtrlBase : public CControlBase
	{
		friend class CTextHost;

	protected:
		CRichEditCtrlBase();
		virtual ~CRichEditCtrlBase();

	public:
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	public:
		ITextHost *GetTextHost();
		ITextServices *GetTextServices();
		ITextDocument *GetITextDocument();

		HDC GetDC();

		int SetInset(LPCRECT lpRect);
		int GetInset(LPRECT lpRect) const;

		int InsertBitmap(HBITMAP hBitmap);
		int InsertBitmap(LPCTSTR lpszBitmapPath);
		// cp: see REOBJECT::cp
		int InsertGif(LPCTSTR lpszGifPath, LONG cp);
		int InsertOle(IOleObject *pOleObject, LONG cp, int nWidth, int nHeight);
		int InsertText(LPCTSTR lpszText, int iPos, COLORREF cr);

		int GetLineRect(int nLineIndex,
			__out POINT *pptStart, __out int *pnLineWidth, __out int *pnLineHeight);

		int SetImages(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		int GetImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD);

	protected:
		int OnDrawRichText(CDC *pMemDCParent);
		int OnAddGifOle(CImageOleCtrl *pImageOleCtrl);

	protected:
		HDC m_hPaintDC;

		ITextServices *m_pTextServices;
		ITextDocument *m_pTextDocument;
		ITextRange *m_pTextRange;

		HIUIIMAGE m_himgBk[4];
		CRect m_rcInset;
		BOOL m_bNeedUpdate;
		BOOL m_bAlreadyStartGifTimer;
		// 保存所有Gif Ole，用于刷新
		std::map<CImageOleCtrl *, int> m_mapGifOles;
	};

	class CSkinRichEditCtrl : public CRichEditCtrl, public CRichEditCtrlBase
	{
		// Construction
	public:
		CSkinRichEditCtrl();
		virtual ~CSkinRichEditCtrl();

	protected:
		virtual int OnDrawBackground(CDC *pMemDCParent);
		virtual int OnDraw(CDC *pMemDCParent);
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		DECLARE_MESSAGE_MAP()
	};

	//////////////////////////////////////////////////////////////////////////
	// CHtmlCtrl
	;
	class CHtmlCtrl : public CHtmlView, public CControlBase
	{
	public:
		CHtmlCtrl();
		virtual ~CHtmlCtrl();

		DECLARE_DISPATCH_MAP()

	public:
		// Init control by specify style.
		;
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		// html Data
	public:
		//{{AFX_DATA(CHtmlCtrl)
		// NOTE: the ClassWizard will add data members here
		//}}AFX_DATA
		;
	public:
		int ShowBorder(BOOL bShow, BOOL bRedraw = TRUE);
		int ShowIEScrollBar(BOOL bShow, BOOL bRedraw = TRUE);
		int ShowContextMenu(BOOL bShow);


		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CHtmlCtrl)
		;
	public:
#if(_MSC_VER <= 1200)
		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext = NULL);
#else
		virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO *pInfo);
		virtual HRESULT OnShowContextMenu(DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved);
		virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);
#endif
	protected:
		virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL
		virtual void PostNcDestroy() {}

	public:
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext &dc) const;
#endif

		// Generated message map functions
		//{{AFX_MSG(CHtmlCtrl)
		afx_msg void OnSize(UINT nType, int cx, int cy);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

		//<<Begin>> For lenovo////////////////////////////////////////////////////////////////////////

	protected:
		virtual void OnDocumentComplete(LPCTSTR lpszURL);

		const CString GetSystemErrorMessage(DWORD dwError);
		CString GetNextToken(CString &strSrc, const CString strDelim, BOOL bTrim = FALSE, BOOL bFindOneOf = TRUE);
		BOOL SetScriptDocument();
		BOOL GetJScript(IDispatch **ppDisp);
		BOOL GetJScripts(IHTMLElementCollection *pColl);
		CString ScanJScript(CString &strAText, CStringArray &args);

	public:
		BOOL JScriptOK();

	public:
		BOOL CallJScript(const CString strFunc, _variant_t *pVarResult = NULL);
		BOOL CallJScript(const CString strFunc, const CString strArg1, _variant_t *pVarResult = NULL);
		BOOL CallJScript(const CString strFunc, const CString strArg1, const CString strArg2, _variant_t *pVarResult = NULL);
		BOOL CallJScript(const CString strFunc, const CString strArg1, const CString strArg2, const CString strArg3, _variant_t *pVarResult = NULL);
		BOOL CallJScript(const CString strFunc, const CStringArray &paramArray, _variant_t *pVarResult = NULL);

	public:
		// 修改网页元素的内容
		BOOL PutElementHtml(CString ElemID, CString Html);
		// 取表单元素的值
		BOOL GetElementValue(CString ElemID, CString &Value);
		// 设置元素的值
		BOOL PutElementValue(CString ElemID, CString Value);
		// 给元素设置焦点
		void ElementSetFocus(CString EleName);
		IHTMLStyle *GetElementStyle(CString ElemID);
		HRESULT GetElement(LPCTSTR szElementId, IDispatch **ppdisp, BOOL *pbCollection = NULL);
		HRESULT GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement);
		HRESULT GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj);

	protected:
		IHTMLDocument2 *m_pDoc;

		//<<End>>////////////////////////////////////////////////////////////////////////

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////
	// class CSkinDateTimeCtrl

	class CSkinDateTimeCtrl : public CDateTimeCtrl, public CControlBase
	{
	public:
		CSkinDateTimeCtrl();
		virtual ~CSkinDateTimeCtrl();

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	protected:
		virtual int UpdateNeedNCPaintState();
		virtual int DrawText(CDC *pDCMem, const CRect &rcClient2Parent, int nBorderCX, int nBorderCY, HIUIIMAGE hSubBmp);
		int LayoutSpinButton();

	public:
		int SetBkImages(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		int GetBkImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD);

		int SetButtonWidth(int nWidth);
		int GetButtonWidth() const;

		int SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDH, LPCTSTR lpszFontIDS, LPCTSTR lpszFontIDD, BOOL bRedraw = TRUE);
		int GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDH, CString *pstrFontIDS, CString *pstrFontIDD) const;

		int SetTextOffset(const LPPOINT lpPt);
		int GetTextOffset(LPPOINT lpPt) const;

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSkinDateTimeCtrl)
		LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSkinDateTimeCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	protected:
		void *m_pMember;
		CSkinSpinButtonCtrl m_ctrlSpinButton;
	};

	/////////////////////////////////////////////////////////////////////////////
	// CPictureCtrl window

#ifdef _WIN32
#define WC_PICTUREVIEWA            "SysPictureView32"
#define WC_PICTUREVIEWW            L"SysPictureView32"

#ifdef UNICODE
#define  WC_PICTUREVIEW            WC_PICTUREVIEWW
#else
#define  WC_PICTUREVIEW            WC_PICTUREVIEWA
#endif
#else
#define WC_PICTUREVIEW             "SysPictureView"
#endif

	class CPictureCtrl : public CWnd, public CControlBase
	{
	public:
		CPictureCtrl();
		virtual ~CPictureCtrl();

	public:
		BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
		BOOL Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID);

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	protected:
		virtual int OnDraw(CDC *pdcMem);
		virtual int DrawImage(CDC *pdcMem, int nStretchRate/*100%*/, BYTE btTransparent = 255);

	public:
		int SetBkImage(LPCTSTR lpszImageName, BOOL bRedraw = TRUE);
		int GetBkImage(CString *pstrImageName);

		int SetFgImage(LPCTSTR lpszImageName, BOOL bRedraw = TRUE);
		int GetFgImage(CString *pstrImageName);

		int SetFgImageAlignModeHor(FOREGROUND_ALIGN_HOR eHor, BOOL bRedraw = TRUE);
		int SetFgImageAlignModeVer(FOREGROUND_ALIGN_VER eVer, BOOL bRedraw = TRUE);

		int SetFgImageTransparent(BYTE btTransparent, BOOL bRedraw = TRUE);
		BYTE GetFgImageTransparent() const;

		int SetFgMargin(LPCRECT lprcMargin);
		int GetFgMargin(LPRECT lprcMargin);

	public:
		int AnimateImage(std::vector<std::pair<int, BYTE> > &vAnimationData, int nElapse/*ms*/);

	public:
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CPictureCtrl)
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CPictureCtrl)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	/////////////////////////////////////////////////////////////////////////////
	// CSearchLightProgressCtrl window

#ifdef _WIN32
#define WC_SEARCHLIGHTPROGRESSA            "SysSearchLightProgress32"
#define WC_SEARCHLIGHTPROGRESSW            L"SysSearchLightProgress32"

#ifdef UNICODE
#define  WC_SEARCHLIGHTPROGRESS            WC_SEARCHLIGHTPROGRESSW
#else
#define  WC_SEARCHLIGHTPROGRESS            WC_SEARCHLIGHTPROGRESSA
#endif
#else
#define WC_SEARCHLIGHTPROGRESS             "SysSearchLightProgress"
#endif

	class CSearchLightProgressCtrl : public CWnd, public CControlBase
	{
	public:
		CSearchLightProgressCtrl();
		virtual ~CSearchLightProgressCtrl();

	public:
		BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
		BOOL Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID);

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	protected:
		virtual int OnDraw(CDC *pdcMem);
		virtual int DrawThumb(CDC *pdcMem, const CRect &rcThumb);
		virtual int DrawThumbMask(CDC *pdcMem, const CRect &rcThumb);
		virtual int DrawMaskImage(CDC *pdcMem);

	public:
		int SetBkColor(COLORREF crBk);
		COLORREF GetBkColor() const;

		int SetDrawThumb(BOOL bDraw);
		BOOL IsDrawThumb() const;

		int SetDrawThumbMask(BOOL bDraw);
		BOOL IsDrawThumbMask() const;

		int SetBkImage(LPCTSTR lpszImageName, BOOL bRedraw = TRUE);
		int GetBkImage(CString *pstrImageName);

		int AnimateThumb(std::vector<SLPROGRESS_ANIMATION> &vAnimationData, int nElapse/*ms*/);

	public:
		// Sets range of values for the control. (32-bit limit)
		void SetRange(int nLower, int nUpper);

		void GetRange(__out int &nLower, __out int &nUpper) const;

		// Gets the current position within the set range of the control.
		int GetPos() const;

		// Sets the current position within the set range of the control.
		int SetPos(int nPos);

	public:
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSearchLightProgressCtrl)
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSearchLightProgressCtrl)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////
	// CPicCharacterCtrl

#ifdef _WIN32
#define WC_PICCHARACTERVIEWA            "SysPicCharacterView32"
#define WC_PICCHARACTERVIEWW            L"SysPicCharacterView32"

#ifdef UNICODE
#define  WC_PICCHARACTERVIEW            WC_PICCHARACTERVIEWW
#else
#define  WC_PICCHARACTERVIEW            WC_PICCHARACTERVIEWA
#endif
#else
#define WC_PICCHARACTERVIEW             "SysPicCharacterView"
#endif

	class CPicCharacterCtrl : public CWnd, public CControlBase
	{
	public:
		CPicCharacterCtrl();
		virtual ~CPicCharacterCtrl();

	public:
		BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
		BOOL Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID);

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	public:
		int SetCharacterListImage(LPCTSTR lpszImageName, BOOL bRedraw = TRUE);
		int GetCharacterListImage(CString *pstrImageName);

		int SetCharacterIndex(CHARINDEX *pCharIndexList, int nCount);	// nCount, the CHARINDEX count in pCharIndexList.

		int FindCharacter(TCHAR ch, __out CHARINDEX *pCharIndex);

	protected:
		virtual int OnDraw(CDC *pdcMem);

	protected:
		DECLARE_MESSAGE_MAP()

		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////
	//
#define GRF_LEFT              0x0001
#define GRF_RIGHT             0x0002
#define GRF_TOP               0x0004
#define GRF_TOPLEFT           0x0008
#define GRF_TOPRIGHT          0x0010
#define GRF_BOTTOM            0x0020
#define GRF_BOTTOMLEFT        0x0040
#define GRF_BOTTOMRIGHT       0x0080

#define GRF_ALL	GRF_LEFT | GRF_TOPLEFT | GRF_TOP | GRF_TOPRIGHT | GRF_RIGHT | GRF_BOTTOMRIGHT | GRF_BOTTOM | GRF_BOTTOMLEFT


	struct WLMOUSE
	{
	public:
		WLMOUSE()
		{
			wParam = 0;
			lParam = 0;
			pWLWnd = NULL;
		}

		WPARAM wParam;
		LPARAM lParam;
		CWLWnd *pWLWnd;
	};

	class CWLWnd : public CObject, public CControlBase	// WL means windowless
	{
		friend class CUIWnd;
		friend class CControlBase;
		friend class CSkinListCtrl;
		friend int DrawWLWindow(HDC hDstDC, CWLWnd *pChild);
		friend int DrawWLWindowOffset(HDC hDstDC, CWLWnd *pChild, SIZE sizeOffset);

	public:
		CWLWnd();
		virtual ~CWLWnd();

		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszControlName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent);
		BOOL IsCreated() const;

	protected:
		virtual int OnCreate();
		virtual int OnDraw(CDC *pMemDCParent);
		virtual int OnDraw(CDC *pMemDCParent, LPPOINT lpptOrg);
		virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult);
		virtual BOOL IsMouseMessage(UINT message, WPARAM wParam, LPARAM lParam);

		CPoint GetLButtonDownPoint() const;
		UINT GetLButtonDownHitTestResult() const;		// return HTLEFT etc.
		int GetLButtonDownRect(LPRECT lpRect) const;

	public:
		virtual LRESULT WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam);	// DON'T call this interface
		virtual CString GetClassName() const;

		int ShowWindow(int nCmdShow);
		BOOL IsWindowVisible() const;

		int EnableWindow(BOOL bEnable);
		BOOL IsWindowEnabled() const;

		int SetWindowText(LPCTSTR lpszString);
		int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const;
		int GetWindowText(CString &rString) const;

		int SetBkImage(LPCTSTR lpszImageName, BOOL bRedraw = TRUE);
		int GetBkImage(CString *pstrImageBk);

		// Set the repeat pixel while stretch the background image
		;
		int SetRepeatPixel(const SIZE *psizeRepeat);
		int GetRepeatPixel(SIZE *psizeRepeat) const;

		// SetBkImgTransparent return the ole transparent value.
		;
		BYTE SetBkImgTransparent(BYTE btTransparent);
		BYTE GetBkImgTransparent() const;

		virtual HWND GetHostWnd();
		CWnd *GetParent() const;

		CWLWnd *SetCapture();
		static CWLWnd *GetCapture();
		static BOOL ReleaseCapture();

		int GetOrigWindowRect(LPRECT lpRect) const;
		int SetOrigWindowRect(LPCRECT lpRect);

		virtual int GetWindowRect(LPRECT lpRect) const;
		virtual int GetClientRect(LPRECT lpRect) const;

		virtual int ScreenToClient(LPPOINT lpPoint) const;
		virtual int ScreenToClient(LPRECT lpRect) const;

		virtual int ClientToScreen(LPPOINT lpPoint) const;
		virtual int ClientToScreen(LPRECT lpRect) const;

		int GetDlgCtrlID() const;

		int SetWindowRgn(HRGN hRgn, BOOL bRedraw);
		int GetWindowRgn(HRGN hRgn) const;

		// This control can't show and can't receive mouse message if scroll out virtual parent
		int SetVirtualParent(CWLWnd *pVirtualParent);
		CWLWnd *GetVirtualParent();

		// Hit Test the point for resizing or move the window, if bSetCursor is TRUE, set the cursor for the windowless window
		;
		virtual int HitTest(CPoint pt, BOOL bSetCursor);
		int SetBorderWidth(int nCXBorder, int nCYBorder);
		int GetBorderWidth(int *pnCXBorder, int *pnCYBorder);
		int SetTitlebarHeight(int nHeight);			// -1: All client as title bar.
		int SetResizeFlag(UINT uFlag); // uFlag parameter can be a combination of the following values: GRF_LEFT, GRF_RIGHT etc. or GRF_ALL
		UINT GetResizeFlag() const;

		int SetNotify(BOOL bNotify);		// Set send message or not
		BOOL IsNotify() const;

		int SetTag(LPVOID lpParam);
		LPVOID GetTag() const;

		virtual int MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
		virtual int MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
		virtual BOOL DestroyWindow();

		// Offset ptOffset while the windowless control drawing.
		int SetDrawOffset(POINT ptOffset);
		int GetDrawOffset(LPPOINT lpptOffset);
		void Invalidate(BOOL bErase = TRUE);	// bErase: not use

		virtual BOOL IsOleControl() const;

		DWORD SetStyle(DWORD dwStyle);
		DWORD GetStyle() const;
		DWORD AddStyle(DWORD dwAddStyle);
		DWORD RemoveStyle(DWORD dwRemoveStyle);

	public:
		virtual LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
		virtual LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) const;

		UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse, void (CALLBACK *lpfnTimer)(HWND, UINT, UINT_PTR, DWORD));
		BOOL KillTimer(UINT_PTR uIDEvent);

	protected:
		void *m_pMember;

	public:
		BOOL m_bFlag;
	};

	// 插入到RichEdit中的Ole对象的基类。 从IOleCtrlBase派生后，创建自己的Ole对象
	// 不需要生成DLL。 不需要CoCreateInstance来创建Ole对象。 只需要像普通C++对象那样使用即可。
	extern const GUID g_guidImageOle;
	class IOleCtrlBase : public IOleObject, public IViewObject2, public CWLWnd
	{
	public:
		IOleCtrlBase(HWND hRichEdit);
		virtual ~IOleCtrlBase();

		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszControlName,
			DWORD dwStyle, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent);

		UINT GetIDD() const;
		int SetIDD(UINT nID);

	public:
		// IUnknown抽象类接口
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef(void);
		virtual ULONG STDMETHODCALLTYPE Release(void);

	public:
		// IOleObject抽象类接口
		virtual HRESULT STDMETHODCALLTYPE SetHostNames(
			/* [in] */ LPCOLESTR szContainerApp,
			/* [unique][in] */ LPCOLESTR szContainerObj)
		{
			return S_OK;
		}
		virtual HRESULT STDMETHODCALLTYPE Close(
			/* [in] */ DWORD dwSaveOption)
		{
			return S_OK;
		}
		virtual HRESULT STDMETHODCALLTYPE SetMoniker(
			/* [in] */ DWORD dwWhichMoniker,
			/* [unique][in] */ IMoniker *pmk)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE GetMoniker(
			/* [in] */ DWORD dwAssign,
			/* [in] */ DWORD dwWhichMoniker,
			/* [out] */ IMoniker **ppmk)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE InitFromData(
			/* [unique][in] */ IDataObject *pDataObject,
			/* [in] */ BOOL fCreation,
			/* [in] */ DWORD dwReserved)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE GetClipboardData(
			/* [in] */ DWORD dwReserved,
			/* [out] */ IDataObject **ppDataObject)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE DoVerb(
			/* [in] */ LONG iVerb,
			/* [unique][in] */ LPMSG lpmsg,
			/* [unique][in] */ IOleClientSite *pActiveSite,
			/* [in] */ LONG lindex,
			/* [in] */ HWND hwndParent,
			/* [unique][in] */ LPCRECT lprcPosRect)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE EnumVerbs(
			/* [out] */ IEnumOLEVERB **ppEnumOleVerb)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE Update(void)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE IsUpToDate(void)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE GetUserType(
			/* [in] */ DWORD dwFormOfType,
			/* [out] */ LPOLESTR *pszUserType)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE EnumAdvise(
			/* [out] */ IEnumSTATDATA **ppenumAdvise)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE GetMiscStatus(
			/* [in] */ DWORD dwAspect,
			/* [out] */ DWORD *pdwStatus)
		{
			return E_NOTIMPL;
		}
		virtual HRESULT STDMETHODCALLTYPE SetColorScheme(
			/* [in] */ LOGPALETTE *pLogpal)
		{
			return E_NOTIMPL;
		}

		// 需要实现
		virtual HRESULT STDMETHODCALLTYPE SetClientSite(
			/* [unique][in] */ IOleClientSite *pClientSite);
		virtual HRESULT STDMETHODCALLTYPE GetClientSite(
			/* [out] */ IOleClientSite **ppClientSite);
		virtual HRESULT STDMETHODCALLTYPE GetUserClassID(
			/* [out] */ CLSID *pClsid);
		virtual HRESULT STDMETHODCALLTYPE SetExtent(
			/* [in] */ DWORD dwDrawAspect,
			/* [in] */ SIZEL *psizel);
		virtual HRESULT STDMETHODCALLTYPE GetExtent(
			/* [in] */ DWORD dwDrawAspect,
			/* [out] */ SIZEL *psizel);
		virtual HRESULT STDMETHODCALLTYPE Advise(
			/* [unique][in] */ IAdviseSink *pAdvSink,
			/* [out] */ DWORD *pdwConnection);
		virtual HRESULT STDMETHODCALLTYPE Unadvise(
			/* [in] */ DWORD dwConnection);

	public:
		// IViewObject2 的方法
		HRESULT STDMETHODCALLTYPE GetColorSet(DWORD, LONG, void *, DVTARGETDEVICE *, HDC, LOGPALETTE **)
		{
			return E_NOTIMPL;
		}
		HRESULT STDMETHODCALLTYPE Freeze(DWORD, LONG, void *, DWORD *)
		{
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE Unfreeze(DWORD dwFreeze)
		{
			return S_OK;
		}

		// 以下接口需要实现
		HRESULT STDMETHODCALLTYPE SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink);
		HRESULT STDMETHODCALLTYPE GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink);
		HRESULT STDMETHODCALLTYPE Draw(
			DWORD dwDrawAspect, LONG lindex,
			void *pvAspect,
			DVTARGETDEVICE *ptd,
			HDC hdcTargetDev,
			HDC hdcDraw,
			LPCRECTL lprcBounds,
			LPCRECTL lprcWBounds,
			BOOL(STDMETHODCALLTYPE *pfnContinue)(ULONG_PTR dwContinue),
			ULONG_PTR dwContinue);
		HRESULT STDMETHODCALLTYPE GetExtent(DWORD dwDrawAspect, LONG lindex,
			DVTARGETDEVICE *ptd, LPSIZEL lpsizel);

	public:
		virtual BOOL IsOleControl() const;
		virtual int GetWindowRect(LPRECT lpRect) const;
		virtual int GetClientRect(LPRECT lpRect) const;

		LPARAM SetUserData(LPARAM lParam);
		LPARAM GetUserData() const;

	protected:
		virtual int OnCreate();

	protected:
		UINT IDD; // Same as CDialog::IDD

	protected:
		IOleClientSite *m_pOleClientSite;
		UIWNDPROPERTIES m_uiwndProperties;
		SIZEL m_sizeExtent;
		// 相对于RichEdit的坐标，仅在OLE可见时有效
		CRect m_rect;
		LPARAM m_lParam;
	};

	class CImageOleCtrl : public IOleCtrlBase
	{
	public:
		CImageOleCtrl(HWND hRichEdit);
		virtual ~CImageOleCtrl();

		virtual int ReleaseObject();

	protected:
		HRESULT STDMETHODCALLTYPE SetClientSite(
			/* [unique][in] */ IOleClientSite *pClientSite);

		HRESULT STDMETHODCALLTYPE Draw(
			DWORD dwDrawAspect, LONG lindex,
			void *pvAspect,
			DVTARGETDEVICE *ptd,
			HDC hdcTargetDev,
			HDC hdcDraw,
			LPCRECTL lprcBounds,
			LPCRECTL lprcWBounds,
			BOOL(STDMETHODCALLTYPE *pfnContinue)(ULONG_PTR dwContinue),
			ULONG_PTR dwContinue);

	public:
		int SetImage(LPCTSTR lpszImagePath);
		int SetImage(HBITMAP hBitmap);
		HIUIIMAGE GetImage() const;

	protected:
		HIUIIMAGE m_hIUIImage;
	};

	//////////////////////////////////////////////////////////////////////////
	// class CRectCtrl
	;
	class CRectCtrl : public CWLWnd
	{
	public:
		CRectCtrl();
		virtual ~CRectCtrl();

		virtual BOOL Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent);

	public:
		int AddBindWindow(CWnd *pBindWindow);
		int DeleteBindWindow(CWnd *pBindWindow);
		virtual int MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
		virtual BOOL DestroyWindow();

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////

	class CWLSplitterBar : public CRectCtrl
	{
	public:
		CWLSplitterBar();
		~CWLSplitterBar();

		virtual BOOL Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent);

	protected:
		static LRESULT SplitterParentWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		int SetOrientation(SPLITTER_ORIENTATION eOrientation);
		SPLITTER_ORIENTATION GetOrientation() const;

		int AddPanel(CWnd *pPanel, SPLITTER_PANEL_POS ePanelPos);	// Bind another panel
		CWnd *GetPanel();

		LONG_PTR SetLongPtr(__in int nIndex, __in LONG_PTR dwNewLong);
		LONG_PTR GetLongPtr(__in int nIndex) const;

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////

	class CWLLine : public CRectCtrl
	{
	public:
		CWLLine();
		~CWLLine();

		virtual BOOL Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent);

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	protected:
		virtual int OnDraw(CDC *pMemDCParent);

	public:
		int SetLinePenStyle(int nStyle);
		int GetLinePenStyle() const;

		int SetLineWidth(int nWidth);
		int GetLineWidth() const;

		int SetPoint1Color(COLORREF cr);
		COLORREF GetPoint1Color() const;

		int SetPoint2Color(COLORREF cr);
		COLORREF GetPoint2Color() const;

		int SetPoint1Pos(const LPPOINT lpPt);
		int GetPoint1Pos(LPPOINT lpPt) const;

		int SetPoint2Pos(const LPPOINT lpPt);
		int GetPoint2Pos(LPPOINT lpPt) const;

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////

	class CWLText : public CRectCtrl
	{
	public:
		CWLText();
		~CWLText();

		virtual BOOL Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent);

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	protected:
		virtual int OnDraw(CDC *pMemDCParent);
		virtual LRESULT WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	public:
		int SetBitmap(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD, BOOL bRedraw = TRUE);
		int GetBitmap(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameD);

		void SetForegroundBitmap(UINT uMask,
			LPCTSTR lpszImageNameForegroundN, LPCTSTR lpszImageNameForegroundD,
			const CRect &rcForegroundMargin,
			FOREGROUND_ALIGN_HOR eAlignModeHor = FAH_LEFT, FOREGROUND_ALIGN_VER eAlignModeVer = FAV_CENTER,
			COLORREF crMask = RGB(255, 0, 255), BOOL bReDraw = TRUE);
		void GetForegroundBitmap(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameD,
			LPRECT rcForegroundMargin,
			int *pnAlignModeHor, int *pnAlignModeVer, COLORREF *pcrMask);

		int SetTextMultiline(BOOL bMultiline);
		BOOL IsTextMultiline();

		int SetPathEllipsis(BOOL bPathEllipsis);
		BOOL IsPathEllipsis() const;

		int SetEndEllipsis(BOOL bEndEllipsis);
		BOOL IsEndEllipsis() const;

		int SetTextAlignHor(TEXT_ALIGN_HOR eHorAlignModeN, TEXT_ALIGN_HOR eHorAlignModeD);
		int GetTextAlignHor(TEXT_ALIGN_HOR *peHorAlignModeN, TEXT_ALIGN_HOR *peHorAlignModeD);

		int SetTextAlignVer(TEXT_ALIGN_VER eVerAlignModeN, TEXT_ALIGN_VER eVerAlignModeD);
		int GetTextAlignVer(TEXT_ALIGN_VER *peVerAlignModeN, TEXT_ALIGN_VER *peVerAlignModeD);

		int SetTextColor(COLORREF crN, COLORREF crD, BOOL bRedraw = TRUE);
		int GetTextColor(COLORREF *pcrN, COLORREF *pcrD);

		int SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD);
		int GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD);

		int SetToolTips(LPCTSTR lpszToolTips);
		CToolTipCtrl *GetToolTipCtrl();

		int SetTextFont(const LOGFONT *lf);
		int GetTextFont(LOGFONT *lf) const;

		int SetTextMargin(LPCRECT lpRect);
		int GetTextMargin(LPRECT lpRect) const;

		// Shadow text
		;
		int ShadowText(BOOL bShadowText);
		BOOL IsShadowText() const;

		int SetTextShadowColor(COLORREF crTextShadow);
		COLORREF GetTextShadowColor() const;

		int SetTextShadowOffset(LPPOINT lpptOffset);
		int GetTextShadowOffset(LPPOINT lpptOffset) const;

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////

	class CWLPicture : public CRectCtrl
	{
	public:
		CWLPicture();
		virtual ~CWLPicture();

		virtual BOOL Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd,
			UINT nID, CWLWnd *pVirtualParent);

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	protected:
		virtual int OnDraw(CDC *pMemDCParent);

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////

	class CWLButton : public CRectCtrl, public CButtonBase
	{
	public:
		CWLButton();
		~CWLButton();

		virtual BOOL Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd,
			UINT nID, CWLWnd *pVirtualParent);
		virtual int ReleaseObject();
		virtual BOOL IsCreated() const;

	protected:
		int OnDraw(CDC *pMemDCParent);
		LRESULT WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	public:
		int GetCheck() const;
		void SetCheck(int nCheck);
	};

	//////////////////////////////////////////////////////////////////////////
	//

	class CWLSliderCtrl : public CRectCtrl
	{
	public:
		CWLSliderCtrl();
		virtual ~CWLSliderCtrl();

	public:
		virtual BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent);

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	protected:
		int OnDraw(CDC *pMemDCParent);
		LRESULT WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	public:
		DWORD GetExtendedStyle();
		DWORD SetExtendedStyle(DWORD dwNewStyle);

		void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);
		void GetRange(int &nMin, int &nMax) const;

		void SetThumbSize(SIZE sizeThumb);
		virtual int SetDeflate(UINT nLeftTop, UINT nRightBottom);
		int GetDeflate(UINT *pnLeftTop, UINT *pnRightBottom);
		void GetThumbRect(LPRECT lprc) const;

		int GetPos() const;
		void SetPos(int nPos);

		int GetProgressPos() const;
		void SetProgressPos(int nPos);

		int SetImages(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameMarkN,
			LPCTSTR lpszImageNameMarkD, LPCTSTR lpszImageNameD);
		int GetImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameMarkN,
			CString *pstrImageNameMarkD, CString *pstrImageNameD);

		int SetThumbCombineImage(LPCTSTR lpszImageNameThumbCombine, BOOL bRedraw = TRUE);
		int SetThumbImages(UINT uMask,
			LPCTSTR lpszImageNameThumbN, LPCTSTR lpszImageNameThumbH,
			LPCTSTR lpszImageNameThumbS, LPCTSTR lpszImageNameThumbD);
		int GetThumbImages(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameThumbN, CString *pstrImageNameThumbH,
			CString *pstrImageNameThumbS, CString *pstrImageNameThumbD);

		int UseMarkBitmap(BOOL bUseMark);
		BOOL IsUseMarkBitmap() const;

		int MarkImageToThumb(BOOL bToThumb);
		BOOL IsMarkImageToThumb() const;

		int SetSplitCount(int nCount);
		int GetSplitCount();
		int SetSplitList(const int *pnFirst, const int *pnLast, BOOL bRedraw = TRUE);
		int AddColorImage(int nValue, HBITMAP hbmpColor);
		int RemoveColorImage(int nValue);

		int EnableToolTipsEx(BOOL bEnable, BOOL bMouseMove = TRUE);
		int SetToolTips(LPCTSTR lpszToolTips);
		CToolTipCtrl *GetToolTipCtrl();

		// For tip window
		;
		int EnableTipWindow(BOOL bEnable);
		BOOL IsEnableTipWindow() const;

		int SetTipWindow(CWnd *pTipWnd);
		CWnd *GetTipWindow();

		int SetTipWindowOffset(const LPSIZE lpsizeOffset);
		int GetTipWindowOffset(LPSIZE lpsizeOffset) const;

		// Animation Move
		int AnimateThumb(std::vector<int> vAnimationData, int nElapse/*ms*/);
		int GetCurTimerTick() const;
		int SetCurTimerTick(int nValue);
		std::vector<int> *GetAnimationData();
		int SetAnimationMode(BOOL bAnimation);
		BOOL IsAnimationMode() const;

	protected:
		virtual void DrawHorzSlider(CDC *pMemDCParent);
		virtual void DrawVertSlider(CDC *pMemDCParent);

	private:
		void *m_pMember;
	};

	class CWLRichEditCtrl : public CRectCtrl, public CRichEditCtrlBase
	{
		friend class CRichEditCtrlBase;
	public:
		CWLRichEditCtrl();
		~CWLRichEditCtrl();

		virtual BOOL Create(DWORD dwStyle,
			const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent);

		virtual BOOL CreateEx(
			DWORD dwExStyle,
			DWORD dwStyle,
			const RECT &rect,
			CWnd *pParentWnd,
			UINT nID
		);

		int MoveWindow(
			int x,
			int y,
			int nWidth,
			int nHeight,
			BOOL bRepaint = TRUE
		);
		int MoveWindow(
			LPCRECT lpRect,
			BOOL bRepaint = TRUE
		);


		virtual LRESULT TxSendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
		virtual LRESULT TxSendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) const;

		//<<Begin Copy from CRichEditCtrl
	public:
		// Attributes
		BOOL CanUndo() const;
		BOOL CanRedo() const;
		UNDONAMEID GetUndoName() const;
		UNDONAMEID GetRedoName() const;
		int GetLineCount() const;
		BOOL GetModify() const;
		void SetModify(__in BOOL bModified = TRUE);
		BOOL SetTextMode(__in UINT fMode);
		UINT GetTextMode() const;
		void GetRect(__out LPRECT lpRect) const;
		CPoint GetCharPos(__in long lChar) const;
		UINT GetOptions() const;
		void SetOptions(__in WORD wOp, __in DWORD dwFlags);
		BOOL SetAutoURLDetect(__in BOOL bEnable = TRUE);
		UINT GetWordWrapMode() const;
		UINT SetWordWrapMode(__in UINT uFlags) const;
		BOOL GetPunctuation(__in UINT fType, __out PUNCTUATION *lpPunc) const;
		BOOL SetPunctuation(__in UINT fType, __in PUNCTUATION *lpPunc);

		// NOTE: first word in lpszBuffer must contain the size of the buffer!
		// NOTE: Copied line does not contain null character!
		int GetLine(__in int nIndex, LPTSTR lpszBuffer) const;
		// NOTE: Copied line does not contain null character!
		int GetLine(__in int nIndex, LPTSTR lpszBuffer, __in int nMaxLength) const;

		BOOL CanPaste(__in UINT nFormat = 0) const;
		void GetSel(__out long &nStartChar, __out long &nEndChar) const;
		void GetSel(__out CHARRANGE &cr) const;
		void LimitText(__in long nChars = 0);
		long LineFromChar(__in long nIndex) const;
		CPoint PosFromChar(__in UINT nChar) const;
		int CharFromPos(__in CPoint pt) const;

		void SetSel(__in long nStartChar, __in long nEndChar);
		void SetSel(__in CHARRANGE &cr);
		DWORD GetDefaultCharFormat(__out CHARFORMAT &cf) const;
		DWORD GetDefaultCharFormat(__out CHARFORMAT2 &cf) const;
		DWORD GetSelectionCharFormat(__out CHARFORMAT &cf) const;
		DWORD GetSelectionCharFormat(__out CHARFORMAT2 &cf) const;
		long GetEventMask() const;
		long GetLimitText() const;
		DWORD GetParaFormat(__out PARAFORMAT &pf) const;
		DWORD GetParaFormat(__out PARAFORMAT2 &pf) const;
#if(_MSC_VER > 1310) // VC2003
		// richedit EM_GETSELTEXT is ANSI
		_AFX_INSECURE_DEPRECATE("GetSelText(char *) is unsafe. Instead, use GetSelText(void) returning CString")
#endif
		long GetSelText(__out LPSTR lpBuf) const;
		int GetTextRange(__in int nFirst, __in int nLast, __out CString &refString) const;
		CString GetSelText() const;
		WORD GetSelectionType() const;
		COLORREF SetBackgroundColor(__in BOOL bSysColor, __in COLORREF cr);
		BOOL SetDefaultCharFormat(__in CHARFORMAT &cf);
		BOOL SetDefaultCharFormat(__in CHARFORMAT2 &cf);
		BOOL SetSelectionCharFormat(__in CHARFORMAT &cf);
		BOOL SetSelectionCharFormat(__in CHARFORMAT2 &cf);
		BOOL SetWordCharFormat(__in CHARFORMAT &cf);
		BOOL SetWordCharFormat(__in CHARFORMAT2 &cf);
		DWORD SetEventMask(__in DWORD dwEventMask);
		BOOL SetParaFormat(__in PARAFORMAT &pf);
		BOOL SetParaFormat(__in PARAFORMAT2 &pf);
		BOOL SetTargetDevice(__in HDC hDC, __in long lLineWidth);
		BOOL SetTargetDevice(__in CDC &dc, __in long lLineWidth);
		long GetTextLength() const;
		long GetTextLengthEx(__in DWORD dwFlags, __in UINT uCodePage = -1) const;
		BOOL SetReadOnly(__in BOOL bReadOnly = TRUE);
		int GetFirstVisibleLine() const;

		// Operations
		void EmptyUndoBuffer();
		void StopGroupTyping();
		UINT SetUndoLimit(__in UINT nLimit);

		// Gets the character index of the first character of a specified line in a multiline edit control
		int LineIndex(__in int nLine = -1) const;
		int LineLength(__in int nLine = -1) const;
		void LineScroll(__in int nLines, __in int nChars = 0);
		void ReplaceSel(LPCTSTR lpszNewText, __in BOOL bCanUndo = FALSE);
		void SetRect(__in LPCRECT lpRect);

		BOOL DisplayBand(__in LPRECT pDisplayRect);
		long FindText(__in DWORD dwFlags, __out FINDTEXTEX *pFindText) const;
		DWORD FindWordBreak(__in UINT nCode, __in DWORD nStart) const;
		long FormatRange(__in FORMATRANGE *pfr, __in BOOL bDisplay = TRUE);
		void HideSelection(__in BOOL bHide, __in BOOL bPerm);
		void PasteSpecial(__in UINT nClipFormat, __in DWORD dvAspect = 0, __in HMETAFILE hMF = 0);
		void RequestResize();
		long StreamIn(__in int nFormat, EDITSTREAM &es);
		long StreamOut(__in int nFormat, EDITSTREAM &es);

		// Clipboard operations
		BOOL Undo();
		BOOL Redo();
		void Clear();
		void Copy();
		void Cut();
		void Paste();

		// OLE support
		IRichEditOle *GetIRichEditOle() const;
		BOOL SetOLECallback(IRichEditOleCallback *pCallback);
		//<< End Copy from CRichEditCtrl

	protected:
		CTextHost *m_pTwh;
	};

	//////////////////////////////////////////////////////////////////////////

	// 当一条消息是图文混合时，需要拆分成多段，由SRichMsgItem记录每一段
	class CRichMsgItem
	{
	public:
		CRichMsgItem();
		~CRichMsgItem();

	public:
		// 协议头，用来修饰m_strMsg。 如果m_strProtocol为空，则m_strMsg为普通文本。
		CString m_strProtocol;
		CString m_strMsgContent;
		// 当m_strProtocol不为空时，消息转成OLE控件。 m_uOleWinID表示OLE控件在界面编辑器中的窗口ID。
		UINT m_uOleWinID;
		// 当m_strProtocol不为空时，消息转成OLE控件。 m_size表示OLE控件的尺寸.
		CSize m_size;
		// 当m_strProtocol不为空时，消息转成OLE控件。 m_lParam表示用户自定义数据，
		// IM控件会把这个参数回传给用户
		LPARAM m_lParam;
	};


	// CIMMsg: 聊天记录窗口中的一条消息
	// 为了支持用户自定义OLE，需要把自定义OLE编码成字符串。 自定义协议头如下：
	// 一到多个非'/'且非'i'字符 + '/' + 一到多个非'/'字符(通常是':')。
	// LibUIDK内置Image OLE编码协议头为："i/:"，所以用户定义的协议，不能是"i/:".
	// 在消息发送方，发送默认gif表情的算法为：点"发送"按钮时，遍历消息输入框, 把所有默认gif表情，
	// 编码成格式为"i/:bxn"的字符串:
	// 1.n为1位或多位数字，表示gif默认表情的索引，比如-1表示微笑。 这个索引需要预定义好，一旦确定，
	// 以后都将不允许被修改. 如果需要做手机端或mac客户端，要和windows端，
	// 保持一致，即不论在任何客户端上，-1都表示微笑.
	// 2. b为1位数字，1表示n为正值，0表示n为负值，负值表示内置表情
	// 3. x为1位数字，表示n有几个字符。不包含前面的负值符号
	// 例如，微笑表情被编码为"i/:01-1".
	// 为了区分用户输入的字符串"i/:01-1"与gif表情编码后的字符串"i/:01-1"，需要在编码表情前，
	// 把所有用户输入的"i/:"前缀，转义为"i//:"。 为了简化，在编码表情前，可以把消息中所有'/'替换为'//'
	// 例如，用户在输入框输入字符串："i/:01-1"，然后又插入了一个微笑表情。 那么在点发送时，整个RichEdit
	// 消息被编码为："i//:01-1i/:01-1"。 由于字符串的"i/:"早已先被转义为"i//:"，所以，出现的"i/:"
	// 一定是表情gif。

	// 在消息接收显示方，处理方式为：搜索消息字符串中的"i/:"，根据后面的数字，把这个字符串解析为
	// gif表情。 然后再把剩余字符串中的"//"全部替换为"/"。
	class CIMMsg
	{
		friend class CIMRichEditCtrlBase;

	public:
		CIMMsg();
		CIMMsg(const CIMMsg &rSrc);
		~CIMMsg();
		CIMMsg &operator=(const CIMMsg &rSrc);

	public:
		int GetVer() const;

		int SetMsgID(int nID);
		int GetMsgID() const;

		int SetSelf(BOOL bSelf);
		BOOL IsSelf() const;

		int SetFace(HIUIIMAGE hImage);
		HIUIIMAGE GetFace() const;

		int SetMsgSenderTitle(int nTitle);
		int GetMsgSenderTitle() const;

		int SetMsgSender(LPCTSTR lpszSender);
		CString GetMsgSender() const;

		int SetMsgStart(int nStart);
		int GetMsgStart() const;

		int SetMsgContentStart(int nStart);
		int GetMsgContentStart() const;

		int SetMsgEnd(int nEnd);
		int GetMsgEnd() const;

		int SetMsgFirstParagraphEnd(int nEnd);
		int GetMsgFirstParagraphEnd() const;

		int SetMsgParagraphCount(int nCount);
		int GetMsgParagraphCount() const;

		LPARAM SetUserData(LPARAM lParam);
		LPARAM GetUserData() const;

		int AddMsgItem(const CRichMsgItem *pMsgItem);

	private:
		void Copy(const CIMMsg &rSrc);

		// 消息本身属性
	private:
		int m_nVer; // CIMMsg版本号
		int m_nMsgID; // 消息ID，唯一标识消息
		CTime m_tMsg; // 消息时间戳
		BOOL m_bSelf: 1; // 是否是自己发的消息
		HIUIIMAGE m_himgFace; // 头像
		int m_nMsgSenderTitle; // 消息发送者头衔级别
		CString m_strMsgSender; // 消息发送者名字
		CString m_strRichMsg; // 原始消息串
		LPARAM m_lParam; // 用户自定义数据

		// 显示时需要信息
	private:
		CIMRichEditCtrlBase *m_pImreHost;

		BOOL m_bGroup: 1; // 消息分组时，表示每组第一个消息

		// cp=char position. 整条消息（从级别+名字开始）插入到RichEdit后的起始位置。
		// 当点击了查看更多，由于是从RichEdit前面插入新的消息，
		// 所以要重新更新m_nCpStart
		int m_nMsgStartCp;

		// 消息内容第一个字符起始位置，相对于消息起始cp
		int m_nMsgContentStartCp;

		// 本条消息插入到RichEdit后，最后一个字符的的位置。相对于消息起始cp
		int m_nMsgEndCp;

		// 记录消息内容一共多少个段落。 当发现内容中包含一个\n，
		// 就表示多一个段落。 在显示时，如果显示出来的行数与本值不一样，
		// 表示Richedit对内容进行了换行。
		int m_nMsgParagraphCount;

		// 消息内容，消息可能包含富文本、图片、OLE组件。
		// 图片、OLE等使用预定义的协议字符串
		// 特别注意：消息字符串应使用"\r"来分段，而不是"\r\n"或"\n".
		std::vector<CRichMsgItem *> m_vpStrRichMsg;
	};

	class CIMRichEditCtrlBase
	{
	public:
		CIMRichEditCtrlBase(BOOL bWindowlessMode, CWnd *pBindRe);
		virtual ~CIMRichEditCtrlBase();

		virtual int ReleaseObject();

	public:
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		int InsertMsg(const CIMMsg *pMsg);

		int SetSenderIndent(int nLeftIndent);
		int GetSenderIndent() const;

		int SetMsgLeftIndent(int nLeftIndent);
		int GetMsgLeftIndent() const;
		int SetMsgRightIndent(int nRightIndent);
		int GetMsgRightIndent() const;

		int SetMaxRelayoutWidth(int nWidth);
		int GetMaxRelayoutWidth() const;

		int SetMsgSenderTitleAlias(int nLevel, LPCTSTR lpszAlias);

		int ShowMsgSenderTitle(BOOL bShow);
		BOOL IsShowMsgSenderTitle() const;

		int SetFaceSize(SIZE sizeFace);
		SIZE GetFaceSize() const;

		int ShowLeftFace(BOOL bShow);
		BOOL IsShowLeftFace() const;

		int ShowRightFace(BOOL bShow);
		BOOL IsShowRightFace() const;

		int GetFirstVisibleMsg() const;
		int GetMsgPosInGroup(int nMsgIndex);

		// 是否合并同一时间段内同一个发送者的消息，返回旧的状态
		BOOL SetCombineMsg(BOOL bCombine);
		BOOL IsCombineMsg() const;

		// 当使用合并消息模式时，设置多久之内的消息需要合并，单位：秒
		int SetCombineMsgTimeSpan(int nSecond);
		int GetCombineMsgTimeSpan() const;

		BOOL IsNeedUpdate() const;
		int NeedUpdate(BOOL bNeedUpdate);

#ifdef _DEBUG
		int Test();
#endif // _DEBUG

	protected:
		int DoInsertMsg(const CIMMsg *pMsg);

		// Layout
		BOOL IsMsgShowRight(const CIMMsg *pMsg) const;
		int DoSetParaFormat();
		int DoLayoutRightMsg(const CIMMsg *pMsg);

		// Rect
		int DoCalFaceRect(const CIMMsg *pMsg, __out LPRECT lprcFace);
		int DoCalQiPaoRect(const CIMMsg *pMsg, __out LPRECT lprcQiPao);
		int DoCalNewLineQiPaoRect(const CIMMsg *pMsg, __out LPRECT lprcQiPao);
		int DoCalSingleLineQiPaoRect(const CIMMsg *pMsg, int nStartLine, int nEndLine,
			__out LPRECT lprcQiPao);
		// 消息显示时，是否有换行，即显示的行数是否大于CIMMsg::m_nMsgParagraphCount
		BOOL IsMsgNewLine(const CIMMsg *pMsg,
			__out int *pnStartLine, __out int *pnEndLine) const;

		// Draw
		virtual int OnDrawNonRichText(CDC *pMemDCParent);
		virtual int OnDrawFace(CDC *pMemDCParent, int nMsgIndex, LPCRECT lprcFace);
		virtual int OnDrawQiPao(CDC *pMemDCParent, int nMsgIndex, LPCRECT lprcQiPao);

	private:
		// 属性
		std::vector<const CIMMsg *> m_vMsg;

		BOOL m_bQiPaoMode; // 是否以气泡模式显示，默认为TRUE

		// 在气泡模式下，设置消息发送者名字的左缩进。空出的位置，用来显示头像
		int m_nSenderLeftIndent;

		// 在气泡模式下，设置消息内容的左右缩进
		int m_nMsgLeftIndent;
		int m_nMsgRightIndent;

		// 最大重新布局宽度，当小于这个宽度时，自己的消息，显示到右侧，
		// 当大于这个宽度时，所有消息，都显示到左侧。
		int m_nMaxRelayoutWidth;
		std::map<int, HBITMAP> m_mapDefineFace; // 预定义头像
		// 消息发送者级别的别称，如：军长、营长等。 第一级级别最低，数字越大，级别越高。
		std::map<int, CString> m_mapMsgSenderTitleAlias;
		BOOL m_bShowMsgSenderTitle;
		SIZE m_sizeFace;
		BOOL m_bShowLeftFace;
		BOOL m_bShowRightFace;
		// 是否合并同一时间段内同一个发送者的消息
		BOOL m_bCombineMsg;
		// 当m_bCombineMsg为TRUE(使用合并消息模式)时，设置多久之内的消息需要合并
		int m_nCombineMsgTimeSpan;

		// Helper
		BOOL m_bWindowlessMode;
		// 绑定的RichEdit控件，如果是windowless模式，指向CWLIMRichEditCtrl *
		// 否则指向CIMRichEditCtrl *。
		CWnd *m_pIMReBind;
	};

	class CIMRichEditCtrl : public CSkinRichEditCtrl, public CIMRichEditCtrlBase
	{
	public:
		CIMRichEditCtrl();
		virtual ~CIMRichEditCtrl(void);

	protected:
		virtual int OnDraw(CDC *pMemDCParent);
		virtual void DoIMSize(UINT nType, int cx, int cy);
		virtual LRESULT DoIMVScroll(WPARAM wParam, LPARAM lParam);
		LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	};

	class CWLIMRichEditCtrl : public CWLRichEditCtrl, public CIMRichEditCtrlBase
	{
	public:
		CWLIMRichEditCtrl();
		virtual ~CWLIMRichEditCtrl();

	protected:
		virtual int OnDraw(CDC *pMemDCParent);
		virtual void DoIMSize(UINT nType, int cx, int cy);
		virtual LRESULT DoIMVScroll(WPARAM wParam, LPARAM lParam);
	};

	//////////////////////////////////////////////////////////////////////////

	class CWave
	{
		friend class CWaveCtrl;
	public:
		CWave();
		~CWave();

	public:
		int AddWaveItem(const POINT *pItem);
		int GetWaveItem(int nIndex, POINT *pItem) const;
		int GetWaveItemCount() const;
		int DeleteAllWaveItems();

		COLORREF SetWaveColor(COLORREF crWave);
		COLORREF GetWaveColor() const;

		UINT SetLineWidth(UINT nLineWidth);
		UINT GetLineWidth() const;

	private:
		std::vector<POINT> m_vWaveItem;
		COLORREF m_crWave;
		UINT m_nLineWidth;
	};

	enum WaveCtrl_Origin
	{
		TOPLEFT,
		TOPRIGHT,
		BOTTOMLEFT,
		BOTTOMRIGHT,
	};


#ifdef _WIN32
#define WC_WAVECTRLA            "WaveView32"
#define WC_WAVECTRLW            L"WaveView32"

#ifdef UNICODE
#define  WC_WAVECTRL            WC_WAVECTRLW
#else
#define  WC_WAVECTRL            WC_WAVECTRLA
#endif

#else
#define WC_WAVECTRL             "WaveView"
#endif

	class CWaveCtrl : public CWnd, public CControlBase
	{
		// Construction
	public:
		CWaveCtrl();
		virtual ~CWaveCtrl();

		virtual BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);

		// Init control by specify style.
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

	protected:
		virtual int OnDraw(CDC *pDC);

		virtual int GenerateWaveRgn(const CWave *pWave, CRgn *pRgn);
		virtual int OnDrawBackground(CDC *pDC);
		virtual int OnDrawHorizontalGrid(CDC *pDC);
		virtual int OnDrawVerticalGrid(CDC *pDC);
		virtual int OnDrawWaves(CDC *pDC);
		virtual int OnDrawWaveMinValueHorizontalLine(CDC *pDC, int nMinValue);	// Valid while only has one wave.

	public:
		// Properties
		;

		int SetBkImage(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkD);
		int GetBkImage(UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImgBkN, CString *pstrImgBkD);

		WaveCtrl_Origin SetOrigin(WaveCtrl_Origin ewo, BOOL bRedraw = TRUE);
		WaveCtrl_Origin GetOrigin() const;

		int SetGridWidht(int nGridWidht, BOOL bRedraw = TRUE);
		int GetGridWidth() const;

		int SetGridHeight(int nGridHeight, BOOL bRedraw = TRUE);
		int GetGridHeight() const;

		int SetInfoAreaWidth(int nInfoAreaWidth, BOOL bRedraw = TRUE);
		int GetInfoAreaWidth() const;

		int SetInfoAreaHeight(int nInfoAreaHeight, BOOL bRedraw = TRUE);
		int GetInfoAreaHeight() const;

		COLORREF SetBkColor(COLORREF cr, BOOL bRedraw = TRUE);
		COLORREF GetBkColor() const;

		COLORREF SetGridLineColor(COLORREF cr, BOOL bRedraw = TRUE);
		COLORREF GetGridLineColor() const;

		int SetGridUnitX(int nX, BOOL bRedraw = TRUE);
		int GetGridUnitX() const;

		int SetGridUnitY(int nY, BOOL bRedraw = TRUE);
		int GetGridUnitY() const;

		int SetTag(LPCTSTR lpszTag);
		int GetTag(CString &strTag) const;

		int EnableAntiAliasing(BOOL bEnableAntiAliasing);
		BOOL IsEnableAntiAliasing() const;

		// Scroll
		int SetScrollPos(int nPixel, BOOL bRedraw = TRUE);
		int GetScrollPos() const;

		int EnableDragMove(BOOL bEnableDragMove);
		BOOL IsEnableDragMove() const;

		// Wave
		int AddWave(CWave *pWave);
		CWave *GetWave(int nIndex);
		int DeleteWave(int nIndex);
		int DeleteWave(CWave *pWave);
		int DeleteAllWaves();
		int GetWaveCount() const;

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CWaveCtrl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CWaveCtrl)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC *pDC);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		DWORD m_dwStyle;
		BOOL m_bUseGdiplus;

		HIUIIMAGE m_himgBk[2];

		//
		WaveCtrl_Origin m_eWaveCtrlOrigin;

		// Properties
		COLORREF m_crBackground;
		COLORREF m_crGridLine;
		int m_nInfoWidth;
		int m_nInfoHeight;
		int m_nGridWidth;
		int m_nGridHeight;
		int m_nGridUnitX;
		int m_nGridUnitY;
		BOOL m_bDrawWaveHorizontalLine;
		CString m_strTag;

		BOOL m_bEnableAntiAliasing;

		// Scroll control
		int m_nScrollPos;

		BOOL m_bEnableDragMove;
		BOOL m_bLButtonDown;
		CPoint m_ptLButtonDown;
		int m_nScrollPosOnLButtonDown;

		// for data
		std::vector<CWave *> m_vWaves;

		CRgn m_rgnOnlyOneWave;
	};

	class CClockCtrl : public CWnd, public CControlBase
	{
		// Construction
	public:
		CClockCtrl();
		virtual ~CClockCtrl();

		// Operations
	public:
		int SetBitmap(HGPBITMAP hbmpBk);
		HGPBITMAP GetBitmap();
		int SetPointer(HGPBITMAP hbmpPointer);
		HGPBITMAP GetPointer();

		int SetRange(int nMix, int nMax);
		int GetRange(int *pnMix, int *pnMax);
		int SetPos(int nPos);
		int GetPos();

		int SetBkCenter(LPPOINT pt);
		int GetBkCenter(LPPOINT pt);
		int SetPtCenter(LPPOINT pt);
		int GetPtCenter(LPPOINT pt);

	protected:
		virtual void OnDraw(CDC *pDC);

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CClockCtrl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CClockCtrl)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()


	private:
		void *m_pMember;
	};

	class CSkinMenuItem
	{
	public:
		DWORD m_dwMagicNum;	// A magic number to distingush our data
		DWORD m_dwID;		// Menu ID
		bool m_bSeparator;	// Separator
		bool m_bSideBar;	// A gradient sidebar
		CString m_strText;	// Menu item text
		HICON m_hIcon;		// Menu icon
		int m_nSize;		// Height of the item(Width of the sidebar if m_bSideBar is true)
		class CSkinMenu *m_pSkinMenu;	// The pointer of pop-up menu

	public:
		CSkinMenuItem()
		{
			m_dwMagicNum = 0x0505a0a0;
			m_dwID = 0;
			m_bSeparator = false;
			m_bSideBar = false;
			m_hIcon = NULL;
			m_nSize = 16;
			m_pSkinMenu = NULL;
		};

		virtual ~CSkinMenuItem()
		{
			if (m_hIcon)
			{
				::DestroyIcon(m_hIcon);
			}
		}

		BOOL IsMyData()
		{
			return m_dwMagicNum == 0x0505a0a0;
		};
	};

	class CSkinMenu : public CMenu
	{
		DECLARE_DYNAMIC(CSkinMenu)

	public:
		CSkinMenu();
		virtual ~CSkinMenu();

		// Call if need before the menu bar be created.
		;
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int BindMenuStyle(MENUMEMBER *pMenuMember);

		// Clean all memory and handles
		;
		int ReleaseObject();

	protected:
		int FillRect(CDC *pDC, const CRect &rc, COLORREF color);

	protected:
		// Message and virtual
		;
		static LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu *pMenu);

		virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	protected:
		virtual int DrawSideBar(CDC *pDC, const CRect &rcItem, HICON hIcon, CString strText);
		virtual int DrawSeparator(CDC *pDC, const CRect &rcItem);
		virtual int DrawGeneralItem(CDC *pDC, const CRect &rcItem, LPDRAWITEMSTRUCT lpdis);
		virtual int DrawGeneralItemState(CDC *pDC, const CRect &rcItem, LPDRAWITEMSTRUCT lpdis, const MENU_ITEM_PROPERTIES *pmip);

		virtual int DrawColorBackGround(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled);
		virtual int DrawIconArea(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked);
		virtual int DrawButton(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked);
		virtual int DrawIcon(CDC *pDC, CRect rect, HICON hIcon, BOOL bSelected, BOOL bDisabled);
		virtual int DrawEmbossed(CDC *pDC, HICON hIcon, CRect rect, BOOL bColor = FALSE);
		virtual int DrawCheckMark(CDC *pDC, CRect rect, BOOL bSelected);
		virtual int DrawMenuText(CDC &dc, CRect rc, CString text, COLORREF color);
		virtual int DrawText(CDC *pDC, CRect rect, CString strText, BOOL bSelected, BOOL bDisabled, BOOL bBold);

		// Below is the functions to build the menu
	public:
		// Hide CMenu::LoadMenu
		;
		BOOL LoadMenu(UINT nIDResource);
		BOOL LoadMenu(LPCTSTR lpszResourceName);

		CSkinMenu *CopyMenu(HMENU hMenuFrom, CSkinMenu *pMenuTo);
		BOOL AppendSkinPopup(UINT nFlags, CSkinMenu *pPopupSkinMenu, DWORD dwID, LPCTSTR strText);
		BOOL AppendSkinMenuString(UINT nFlags, DWORD dwID, LPCTSTR strText, HICON hIcon = NULL, ACCEL *pAccel = 0);
		BOOL AppendSkinSeparator();

		BOOL AddSideBar(int nSize = 32, LPCTSTR strText = NULL, HICON hIcon = NULL, DWORD dwID = 0);
		BOOL InsertSkinMenu(UINT nPos, UINT nFlags, CSkinMenuItem *pItem);
		BOOL ModifySkinMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);

		int Break();
		int BreakBar();

		BOOL SetIcon(UINT nID, HICON *hIcon, BOOL bByPos = FALSE); //add an icon to the given item

		// Hide the CMenu::TrackPopupMenu
		;
		BOOL TrackPopupMenu(UINT nFlags, int x, int y, CWnd *pWnd, LPCRECT lpRect = NULL);

		// Find the pop-up menu from a menu item id, you may not need it
		;
		CSkinMenu *FindSubMenuFromID(DWORD dwID);

	public:
		// Menu border
		;
		int SetMenuBorderStyle(MENU_BORDER_STYLE eMbs);
		MENU_BORDER_STYLE GetMenuBorderStyle() const;

		int SetFlatMenuBorderColor(COLORREF crBorder);
		COLORREF GetFlatMenuBorderColor() const;

		// Height of the menu item
		;
		int SetItemHeight(int nHeight);
		int GetItemHeight() const;

		// Normal item
		;
		int SetNormalItemTextColor(COLORREF crItemText);
		COLORREF GetNormalItemTextColor() const;

		int SetNormalItemFont(const LOGFONT *plf);
		int GetNormalItemFont(LOGFONT *plf) const;

		int SetNormalItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetNormalItemBkType() const;

		int SetNormalItemBkColor(COLORREF crItemBk);
		COLORREF GetNormalItemBkColor() const;

		int SetNormalItemBkImage(LPCTSTR lpszImageNameItem);
		int GetNormalItemBkImage(CString *pstrImageNameItem);

		// Highlight item
		;
		int SetHighlightItemTextColor(COLORREF crItemText);
		COLORREF GetHighlightItemTextColor() const;

		int SetHighlightItemFont(const LOGFONT *plf);
		int GetHighlightItemFont(LOGFONT *plf) const;

		int SetHighlightItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetHighlightItemBkType() const;

		int SetHighlightItemBkColor(COLORREF crItemBk);
		COLORREF GetHighlightItemBkColor() const;

		int SetHighlightItemBkImage(LPCTSTR lpszImageNameItem);
		int GetHighlightItemBkImage(CString *pstrImageNameItem);

		// Disabled item
		;
		int SetDisabledItemTextColor(COLORREF crItemText);
		COLORREF GetDisabledItemTextColor() const;

		int SetDisabledItemFont(const LOGFONT *plf);
		int GetDisabledItemFont(LOGFONT *plf) const;

		int SetDisabledItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetDisabledItemBkType() const;

		int SetDisabledItemBkColor(COLORREF crItemBk);
		COLORREF GetDisabledItemBkColor() const;

		int SetDisabledItemBkImage(LPCTSTR lpszImageNameItem);
		int GetDisabledItemBkImage(CString *pstrImageNameItem);

		// SideBar
		;
		int SetSideBarStartColor(COLORREF crSideBarStart);
		COLORREF GetSideBarStartColor() const;

		int SetSideBarEndColor(COLORREF crSideBarEnd);
		COLORREF GetSideBarEndColor() const;

		// Separator
		;
		int SetSeparatorBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetSeparatorBkType() const;

		int SetSeparatorColor(COLORREF crSeparator);
		COLORREF GetSeparatorColor() const;

		int SetSeparatorLeftMargin(int nLeftMargin);
		int GetSeparatorLeftMargin() const;

		int SetSeparatorRightMargin(int nRightMargin);
		int GetSeparatorRightMargin() const;

		int SetSeparatorBkImage(LPCTSTR lpszImageNameSeparator);
		int GetSeparatorBkImage(CString *pstrImageNameSeparator);

		// Menu check icon
		;
		int SetMenuCheckImage(LPCTSTR lpszImageNameCheck);
		int GetMenuCheckImage(CString *pstrImageNameCheck);

		// Icon area
		;
		int SetIconAreaWidth(int nWidth);
		int GetIconAreaWidth() const;

		int SetIconAreaStartColor(COLORREF crIconAreaStart);
		COLORREF GetIconAreaStartColor() const;

		int SetIconAreaEndColor(COLORREF crIconAreaEnd);
		COLORREF GetIconAreaEndColor() const;

		int DrawIconBk(BOOL bDraw);
		BOOL IsDrawIconBk() const;

	protected:
		MENUMEMBER m_MenuMember;
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////
	// CMemStdioFile

	class CMemStdioFile : public CMemFile
	{
	public:
		CMemStdioFile();
		virtual ~CMemStdioFile();
		virtual BOOL OpenResource(LPCTSTR lpszID, LPCTSTR lpszType);

	public:
		BOOL IsUnicode();

		virtual BOOL ReadString(CString &rString);
		virtual BOOL ReadStringA(std::string &rstring);
		virtual BOOL ReadStringW(std::wstring &rstring);
	};

	//////////////////////////////////////////////////////////////////////////
	// CUIWndBase: same as CUIWnd, but without the.xui.

	class CTaskWndMgr;
	class CTaskWndInfo;

	class CUIWndBase : public CWnd
	{
		DECLARE_DYNCREATE(CUIWndBase)

	public:
		CUIWndBase(CWnd *pParentWnd = NULL);	// pParentWnd only for modal window
		virtual ~CUIWndBase();

	protected:
		HWND PreModal();
		void PostModal();

		// Hit Test the point for resizing the window, if bSetCursor is TRUE, set the cursor for the window
		int HitTest(CPoint pt, BOOL bSetCursor);

		int SetLayeredWindow(BOOL bSet);
		virtual int OnDraw(CDC *pMemDC);

		int HandleTabKey(MSG *pMsg);

	public:
		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd *pParentWnd, BOOL bUseLayeredWindow = TRUE);

		int DoModal(DWORD dwStyle = 0);
		void EndDialog(int nResult);

	public:
		virtual int UpdateUIWindow(int nTransparent = 255);

		int SetBkImage(UINT uMask, LPCTSTR lpszImageNamebmp,
			LPCTSTR lpszImageNamebmpN, LPCTSTR lpszImageNamebmpH,
			LPCTSTR lpszImageNamebmpS, LPCTSTR lpszImageNamebmpD, BOOL bResetSize = TRUE);
		int SetBkImage(LPCTSTR lpszImageName);
		HIUIIMAGE GetBkImage();

		int SetTitleBarHeight(int nTitleBarHeight);
		int GetTitleBarHeight() const;

		int SetScrollSize(CSize size);
		int GetScrollSize(CSize *pSize) const;

		int SetCXBorder(int cx);
		int GetCXBorder() const;

		int SetCYBorder(int cy);
		int GetCYBorder() const;

		int SetDragResizeFlag(BOOL bDragResize);
		BOOL GetDragResizeFlag() const;

		int UseLayeredWindow(BOOL bUsed);
		BOOL IsUseLayeredWindow() const;

		// for task window
		CTaskWndMgr *FindTaskWindow(CWnd *pTaskWnd, CTaskWndInfo *pTaskInfo);
		CTaskWndMgr *FindTaskWindow(int nTabRadioID, CTaskWndInfo *pTaskInfo);

		CWnd *GetLastTabstopCtrl(BOOL *pbHWND);
		BOOL IsControlInRadioTaskWnd(CWnd *pCtrl, CTaskWndMgr **ppTaskWndMgr);

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CUIWndBase)
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CUIWndBase)
		afx_msg void OnPaint();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
		CWnd *m_pParentWnd;             // parent/owner window
	};

	//////////////////////////////////////////////////////////////////////////
	// CUIWnd

	class CIUIMultiDocTemplate;
	class CIUIFrameWnd;
	class CIUIMDIFrameWnd;
	class CIUIMDIClientWnd;
	class CIUIView;

	class CUIWnd : public CWnd
	{
		DECLARE_DYNCREATE(CUIWnd)

	public:
		CUIWnd(CWnd *pParentWnd = NULL);	// pParentWnd only for modal window
		virtual ~CUIWnd();

	public:
		UINT GetIDD() const;
		int SetIDD(UINT nID);

		virtual void SetNoCodeUIMode(BOOL bNoCodeUIMode);
		virtual BOOL IsNoCodeUIMode() const;

		BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext = NULL);
		BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID, CCreateContext *pContext = NULL);
		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd *pParentWnd, BOOL bLoadBkImageFromFile = TRUE, BOOL bUseLayeredWindow = TRUE);
		DWORD GetLastCreateError();

		// lParam: A user define param pass to pMsgHandler.
		// Since a ui window template in xui file may be create more than one instance,
		// use different lParam to differentiate theme.
		// The return CUIWnd pointer need NOT release.
		static int PopupModalWindow(
			UINT nPopupWndID,
			CWnd *pWndParent,
			CUIWndHandler *pMsgHandler,
			LPARAM lParam);
		static CUIWnd *PopupModelessWindow(
			UINT nPopupWndID,
			CWnd *pWndNotify,
			CUIWndHandler *pMsgHandler,
			LPARAM lParam,
			CWnd *pWndParent);
		static CUIWnd *CreateChildWindow(
			LPCTSTR lpszWindowName,
			UINT nChildWndID,
			DWORD dwStyle,
			CWnd *pWndParent,
			CUIWndHandler *pMsgHandler,
			LPARAM lParam);
		static CUIWnd *CreateChildWindow(
			LPCTSTR lpszWindowName,
			UINT nChildWndID,
			DWORD dwStyle,
			CRectCtrl *pCtrlRect,
			CWnd *pWndParent,
			CUIWndHandler *pMsgHandler,
			LPARAM lParam);
		int DeleteChildWindow(CUIWnd *pUIChildWnd);


		// As dialog
		;
		int DoModal(DWORD dwStyle = 0);
		void EndDialog(int nResult);
		virtual BOOL OnInitDialog();

	protected:
		// As dialog
		;
		virtual void OnOK();
		virtual void OnCancel();

	public:
		;
		int SetScrollSize(CSize size);
		int GetScrollSize(CSize *pSize);
		int GetCXBorder();
		int SetCXBorder(int cx);
		int GetCYBorder();
		int SetCYBorder(int cy);

		int SetDragResizeFlag(BOOL bDragResize);
		BOOL GetDragResizeFlag() const;

		// Set the repeat pixel while stretch the background image
		;
		int SetXRepeatPixel(UINT nPixel);
		UINT GetXRepeatPixel() const;
		int SetYRepeatPixel(UINT nPixel);
		UINT GetYRepeatPixel() const;

		int GetBackgroundImageSize(CSize *pSize);
		int GetOriginalClientAreaRect(LPRECT lpRect);
		int GetClientAreaRect(LPRECT lpRect);
		int ResizeRect(LPCRECT lpRectSrc, LPRECT lpRectResized);
		CRectCtrl *GetRectChild(int nRectID);
		CWLWnd *GetDlgWLItem(int nItemID);	// WL means windowless

		int SetWindowResizePoint(const LPPOINT lpPt);
		int GetWindowResizePoint(LPPOINT lpPt) const;

		BOOL IsKeepAspectRatio() const;
		LONG GetMaxHorRatio() const;
		LONG GetMaxVerRatio() const;

		int SetBkImage(LPCTSTR lpszImageName,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD,
			BOOL bResetSize = TRUE);
		int SetBkImage(LPCTSTR lpszImageName);
		HIUIIMAGE GetBkImage();

		int SetBkColor(COLORREF crBk);
		COLORREF GetBkColor() const;

		int SetUIWndProperties(const UIWNDPROPERTIES *pProperties);
		const UIWNDPROPERTIES *GetUIWndProperties();

		int SetTag(LPCTSTR lpszTag);
		int GetTag(CString &strTag) const;

		int SetLoadBkImageFromFile(BOOL bFromFile);
		BOOL IsLoadBkImageFromFile() const;

		int UseLayeredWindow(BOOL bUsed);
		BOOL IsUseLayeredWindow() const;

		int AutoResetRgn(BOOL bAuto);
		BOOL IsAutoResetRgn() const;

		UINT GetDefaultMenuID();
		CIUIFrameWnd *GetParentFrame() const;

		BOOL AutoMenuEnable(BOOL bAuto);
		BOOL IsAutoMenuEnable() const;

		// for frame
		// Get the top CIUIFrameWnd from this to its parent.
		;
		CIUIFrameWnd *GetTopLevelFrame() const; ///
		// Get the top CIUIFrameWnd from its children.
		;
		CIUIFrameWnd *GetTopIUIFrame() const;

		// for task window
		CTaskWndMgr *FindTaskWindow(CWnd *pTaskWnd, CTaskWndInfo *pTaskInfo);
		CTaskWndMgr *FindTaskWindow(int nTabRadioID, CTaskWndInfo *pTaskInfo);

		// Switch to task window by simulate click radio button.
		// If the radio button bind a task window, the task window will be show.
		int SwitchToTaskRadio(UINT uRadioID);

		CWnd *GetLastTabstopCtrl(BOOL *pbHWND);
		BOOL IsControlInRadioTaskWnd(CWnd *pCtrl, CTaskWndMgr **ppTaskWndMgr);

		int DisableStoryboard();
		int SetStoryboardFPS(int nFPS);
		int GetStoryboardFPS() const;
		int PlayStoryboard();
		int SeekToFrame(int nFrame);

	protected:
		virtual void WindowID()
		{
			ASSERT(FALSE);
		}
		HWND PreModal();
		void PostModal();

	protected:
		virtual int OnDraw(CDC *pMemDC);
		virtual int DrawULWChildren(CDC *pMemDC);
		virtual int DrawWLChildren(CDC *pMemDC);

		int SetLayeredWindow(BOOL bSet);
		int AlphaBlendBkImageAndColor();
		int HandleTabKey(MSG *pMsg);

	public:
		// If called UpdateLayeredWindow, call UpdateLayeredWindow, or call UpdateWindow
		int Update();

		virtual int UpdateUIWindow(int nTransparent = 255);

	protected:
		// provide your own images for background, by default, LibUIDK get theme from the path set in .ui file.
		;
		virtual int InitGdiObject(HIUIIMAGE *pbmpBase,
			HIUIIMAGE *pbmpN, HIUIIMAGE *pbmpH, HIUIIMAGE *pbmpS, HIUIIMAGE *pbmpD);

		// When resizing the window, reset images for child. if bForceSet is TRUE, always set the image, otherwise, only set that needed.
		;
		int SetChildImage(const CIUIImage &bmpN, const CIUIImage &bmpH, const CIUIImage &bmpS, const CIUIImage &bmpD);

		// Hit Test the point for resizing the window, if bSetCursor is TRUE, set the cursor for the window
		;
		int HitTest(CPoint pt, BOOL bSetCursor);

		BOOL IsChildNeedResize(LPCRECT rect);

		int ReleaseGdiObject();
		int InitPath();
		int Init();

	private:
		BOOL AdjustChildControls();
		BOOL DestroyChildControls();
		int CallLibUIDKHandlerMap(UINT message, WPARAM wParam, LPARAM lParam);
		int ApplyStoryboardItem(const CTRLPROPERTIES *pCtrlProp);

	protected:
		UINT IDD;
		CString m_strID;

	private:
		// LibUIDK don't support DDX_Control, use GetDlgItem instead of.
		void DDX_Control(CDataExchange *pDX, int nIDC, CWnd &rControl);


		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CUIWnd)
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg);
		virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CUIWnd)
		afx_msg void OnDestroy();
		afx_msg void OnPaint();
		afx_msg LRESULT OnChildMove(WPARAM wParam, LPARAM lParam);
		afx_msg void OnInitMenuPopup(CMenu *, UINT, BOOL);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
		CWnd *m_pParentWnd;             // parent/owner window

	public:
		CUIWndHandler *m_pUIWndHandler;
	};

	//////////////////////////////////////////////////////////////////////////
	// CScrollWnd

	class CScrollWnd : public CScrollView
	{
	public:
		CScrollWnd();
		virtual ~CScrollWnd();
		virtual BOOL Create(__in DWORD dwStyle, __in CRectCtrl *pRectCtrl, __in CWnd *pParentWnd, __in UINT nID);

	public:
#ifdef _DEBUG
		virtual void AssertValid() const;
#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext &dc) const;
#endif
#endif

	public:
		int SetBkColor(COLORREF crBk);
		COLORREF GetBkColor() const;

		// Advanced: for implementing custom print preview
		BOOL DoPrintPreview2(UINT nIDResource, CView *pPrintView, CRuntimeClass *pPreviewViewClass, CPrintPreviewState *pState);

	protected:
		virtual void PostNcDestroy() {}
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

		virtual void OnDraw(CDC *pDC);      // overridden to draw this view
		virtual int OnDrawBackground(CDC *pDC);
		virtual void OnEndPrintPreview(CDC *pDC, CPrintInfo *pInfo, POINT point, CPreviewView *pView);

		afx_msg void OnPaint();
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnFilePrintPreview2();			// Don't call CView::OnFilePrintPreview, Since it use Document/View frame.
		DECLARE_MESSAGE_MAP()

	protected:
		COLORREF m_crBk;
	};

	//////////////////////////////////////////////////////////////////////////
	// CFormWnd

#if(_MSC_VER > 1200) // VC6.0

	class _AFX_MOUSEANCHORWND2 : public CWnd
	{
	private:
		using CWnd::Create;

	public:
		_AFX_MOUSEANCHORWND2(CPoint &ptAnchor);
		~_AFX_MOUSEANCHORWND2();

		BOOL Create(class CFormWnd *pParent);
		void SetBitmap(UINT nID);

		CRect m_rectDrag;
		CPoint m_ptAnchor;
		BOOL m_bQuitTracking;
		UINT m_nAnchorID;
		HCURSOR m_hAnchorCursor;

		virtual BOOL PreTranslateMessage(MSG *pMsg);

		afx_msg void OnPaint();
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		DECLARE_MESSAGE_MAP()
	};

#endif // (_MSC_VER > 1200) // VC6.0

	class CFormWnd : public CUIWnd
	{
	public:
		CFormWnd();
		virtual ~CFormWnd();
		virtual void WindowID()
		{
			ASSERT(FALSE);
		}

	public:
		static AFX_DATA const SIZE sizeDefault;
		// used to specify default calculated page and line sizes

		// in logical units - call one of the following Set routines
		void SetScaleToFitSize(SIZE sizeTotal);
		void SetScrollSizes(int nMapMode, SIZE sizeTotal, const SIZE &sizePage = sizeDefault, const SIZE &sizeLine = sizeDefault);

		// Attributes
	public:
		CPoint GetScrollPosition() const;       // upper corner of scrolling
		CSize GetTotalSize() const;             // logical size

		void CheckScrollBars(BOOL &bHasHorzBar, BOOL &bHasVertBar) const;

		// for device units
		CPoint GetDeviceScrollPosition() const;
		void GetDeviceScrollSizes(int &nMapMode, SIZE &sizeTotal, SIZE &sizePage, SIZE &sizeLine) const;

		// Operations
	public:
		void ScrollToPosition(POINT pt);    // set upper left position
		void FillOutsideRect(CDC *pDC, CBrush *pBrush);
		void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
		BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);

		int UseCustomScrollBar(BOOL bUseCustomScrollBar);
		int UpdateCustomScrollBar();
		int SetVScrollBarWidth(int nWidth);
		int GetVScrollBarWidth() const;
		int SetHScrollBarHeight(int nHeight);
		int GetHScrollBarHeight() const;
		CScrollBarEx *GetHorzScrollBarEx();
		CScrollBarEx *GetVertScrollBarEx();

		// Implementation
	protected:
#if(_MSC_VER > 1200) // VC6.0
		class _AFX_MOUSEANCHORWND2 *m_pAnchorWindow; // window for wheel mouse anchor
		friend class _AFX_MOUSEANCHORWND2;
#endif
		int m_nMapMode;				 // mapping mode for window creation
		CSize m_totalLog;           // total size in logical units (no rounding)
		CSize m_totalDev;           // total size in device units
		CSize m_pageDev;            // per page scroll size in device units
		CSize m_lineDev;            // per line scroll size in device units

		BOOL m_bCenter;             // Center output if larger than total size
		BOOL m_bInsideUpdate;       // internal state for OnSize callback
		void CenterOnPoint(CPoint ptCenter);
		void ScrollToDevicePosition(POINT ptDev); // explicit scrolling no checking

	protected:
		void UpdateBars();          // adjust scrollbars etc
		BOOL GetTrueClientSize(CSize &size, CSize &sizeSb);
		// size with no bars
		void GetScrollBarSizes(CSize &sizeSb);
		void GetScrollBarState(CSize sizeClient, CSize &needSb, CSize &sizeRange, CPoint &ptMove, BOOL bInsideClient);
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	public:
		virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
		virtual void OnPrepareDC(CDC *pDC, CPrintInfo *pInfo = NULL);

		virtual CSize GetWheelScrollDistance(CSize sizeDistance, BOOL bHorz, BOOL bVert);

		// scrolling implementation support for OLE
		virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
		virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

		//{{AFX_MSG(CScrollView)
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
		afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
		//	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
		//}}AFX_MSG
		afx_msg LRESULT HandleMButtonDown(WPARAM wParam, LPARAM lParam);
		DECLARE_MESSAGE_MAP()

	protected:
		BOOL m_bUseCustomScrollBar;
		CScrollBarEx m_wndHScroll;
		CScrollBarEx m_wndVScroll;
		int m_nHScrollBarHeight;
		int m_nVScrollBarWidth;
	};

	/////////////////////////////////////////////////////////////////////////////
	// CMenuBar window. Support attach or load menu. You should call AfxInitSkinMenu before using CMenuBar.

#ifdef _WIN32
#define WC_MENUBARA            "MenuBar32"
#define WC_MENUBARW            L"MenuBar32"

#ifdef UNICODE
#define  WC_MENUBAR            WC_MENUBARW
#else
#define  WC_MENUBAR            WC_MENUBARA
#endif

#else
#define WC_MENUBAR             "MenuBar"
#endif

	;
	class CMenuBar : public CWnd, public CControlBase
	{
		friend class CIUIMultiDocTemplate;
		friend class CIUIDocument;

		// Construction
	public:
		CMenuBar();
		virtual ~CMenuBar();

		BOOL Create(DWORD dwStyle, CRect rect, CWnd *pParentWnd, UINT nWinID);
		BOOL Create(DWORD dwStyle, CRect rect, CWnd *pParentWnd, UINT nWinID, UINT nMenuID,
			HIUIIMAGE hCombineImage);
		BOOL Create(DWORD dwStyle, CRect rect, CWnd *pParentWnd, UINT nWinID, UINT nMenuID,
			HIUIIMAGE hBmpN, HIUIIMAGE hBmpH, HIUIIMAGE hBmpS, HIUIIMAGE hBmpD);

		// Call if need before the menu bar be created.
		;
		virtual int BindMenuStyle(LPCTSTR lpszStyleID);
		virtual int BindMenuStyle(const CTRLPROPERTIES *pCtrlProp);
		CTRLPROPERTIES *GetBindMenuStyle() const;

		int ReleaseMenuButton();
		int ReleaseMenuItemBKImage();

	protected:
		int RebuildMenuBar();
		int RelayoutMenuBarButton(CSkinButton *pMenuButton, int nImageHeight, int &nLeft, int &nTotalWidth);
		int SetMenuButtonProperty(CSkinButton *pMenuButton);

		// For menu bar and menu bar button
		;
	public:
		int SetMenuID(UINT uMenuID);
		int SetHMENU(HMENU hMenu);
		CSkinMenu *GetSkinMenu();

		// Pop-up menu
		;
		int PopupMenu(UINT uButtonID);

		int SetMenuButtonImages(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD);
		int SetMenuButtonCheckedImages(UINT uMask,
			LPCTSTR lpszImageNameCN, LPCTSTR lpszImageNameCH,
			LPCTSTR lpszImageNameCS, LPCTSTR lpszImageNameCD);
		int SetMenuButtonImages(LPCTSTR lpszImageName);
		int GetMenuButtonImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD) const;

		int SetMenuButtonTextColor(COLORREF crN, COLORREF crH, COLORREF crS, COLORREF crD, BOOL bRedraw = TRUE);
		int GetMenuButtonTextColor(COLORREF *pcrN, COLORREF *pcrH, COLORREF *pcrS, COLORREF *pcrD) const;

		// font of button's text
		;
		int SetMenuButtonTextFont(LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDH, LPCTSTR lpszFontIDS, LPCTSTR lpszFontIDD, BOOL bRedraw = TRUE);
		int GetMenuButtonTextFont(CString *pstrFontIDN, CString *pstrFontIDH, CString *pstrFontIDS, CString *pstrFontIDD) const;

		// For menu and menu item
	public:
		// Menu border
		;
		int SetMenuBorderStyle(MENU_BORDER_STYLE eMbs);
		MENU_BORDER_STYLE GetMenuBorderStyle() const;

		int SetFlatMenuBorderColor(COLORREF crBorder);
		COLORREF GetFlatMenuBorderColor() const;

		// Height of the menu item
		;
		int SetMenuItemHeight(int nHeight);
		int GetMenuItemHeight() const;

		// Normal item
		;
		int SetNormalMenuItemTextColor(COLORREF crMenuItemText);
		COLORREF GetNormalMenuItemTextColor() const;

		int SetNormalMenuItemFont(const LOGFONT *plf);
		int GetNormalMenuItemFont(LOGFONT *plf) const;

		int SetNormalMenuItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetNormalMenuItemBkType() const;

		int SetNormalMenuItemBkColor(COLORREF crMenuItemBk);
		COLORREF GetNormalMenuItemBkColor() const;

		// Highlight item
		;
		int SetHighlightMenuItemTextColor(COLORREF crMenuItemText);
		COLORREF GetHighlightMenuItemTextColor() const;

		int SetHighlightMenuItemFont(const LOGFONT *plf);
		int GetHighlightMenuItemFont(LOGFONT *plf) const;

		int SetHighlightMenuItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetHighlightMenuItemBkType() const;

		int SetHighlightMenuItemBkColor(COLORREF crMenuItemBk) const;
		COLORREF GetHighlightMenuItemBkColor() const;

		// Disabled item
		;
		int SetDisabledMenuItemTextColor(COLORREF crMenuItemText);
		COLORREF GetDisabledMenuItemTextColor() const;

		int SetDisabledMenuItemFont(const LOGFONT *plf);
		int GetDisabledMenuItemFont(LOGFONT *plf) const;

		int SetDisabledMenuItemBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetDisabledMenuItemBkType() const;

		int SetDisabledMenuItemBkColor(COLORREF crMenuItemBk) const;
		COLORREF GetDisabledMenuItemBkColor() const;

		// SideBar
		;
		int SetSideBarStartColor(COLORREF crSideBarStart);
		COLORREF GetSideBarStartColor() const;

		int SetSideBarEndColor(COLORREF crSideBarEnd);
		COLORREF GetSideBarEndColor() const;

		// Separator
		;
		int SetSeparatorBkType(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetSeparatorBkType() const;

		int SetSeparatorColor(COLORREF crSeparator);
		COLORREF GetSeparatorColor() const;

		int SetSeparatorLeftMargin(int nLeftMargin);
		int GetSeparatorLeftMargin() const;

		int SetSeparatorRightMargin(int nRightMargin);
		int GetSeparatorRightMargin() const;

		// Icon area
		;
		int SetIconAreaWidth(int nWidth);
		int GetIconAreaWidth() const;

		int SetIconAreaStartColor(COLORREF crIconAreaStart);
		COLORREF GetIconAreaStartColor() const;

		int SetIconAreaEndColor(COLORREF crIconAreaEnd);
		COLORREF GetIconAreaEndColor() const;

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMenuBar)
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg);
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL


		// Generated message map functions
	protected:
		//{{AFX_MSG(CMenuBar)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnEnterMenuLoop(BOOL bIsTrackPopupMenu);
		afx_msg void OnExitMenuLoop(BOOL bIsTrackPopupMenu);
		afx_msg LRESULT OnMenuMouseMove(WPARAM wParam, LPARAM lParam);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		CSkinMenu m_menuBar;
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////
	// CMenuWnd

	class CMenuWnd : public CUIWnd
	{
	public:
		CMenuWnd();
		virtual ~CMenuWnd();
		virtual BOOL Create(CWnd *pParentWnd);

	public:
		virtual int Popup(int x, int y);	// CPoint(x, y): the arrow point
		virtual int Hide();

		int SetNotifyWnd(CWnd *pWnd);
		CWnd *GetNotifyWnd();


		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMenuWnd)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMenuWnd)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		CWnd *m_pNotifyWnd;
	};

	//////////////////////////////////////////////////////////////////////////
	//

#ifdef _WIN32
#define WC_SPLITTERBARA            "SplitterBar32"
#define WC_SPLITTERBARW            L"SplitterBar32"

#ifdef UNICODE
#define  WC_SPLITTERBAR        WC_SPLITTERBARW
#else
#define  WC_SPLITTERBAR        WC_SPLITTERBARA
#endif
#else
#define WC_SPLITTERBAR             "SplitterBar"
#endif

	class CSplitterBar : public CWnd, public CControlBase
	{
	public:
		CSplitterBar();
		virtual ~CSplitterBar();
		virtual int ReleaseObject();

		virtual BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
		virtual BOOL Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID);

		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		// starting and stopping tracking
		virtual int StartTracking();
		virtual int StopTracking(BOOL bAccept);

		int OnInvertTracker(const CRect &rect);

	public:
		int SetBkImages(UINT uMask,
			LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
			LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD, BOOL bRedraw = TRUE);
		int GetBkImages(
			UINT uMask,
			BOOL *pbCombineImage,
			CString *pstrCombineImageName,
			CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD);

		int SetHCursor(HCURSOR hCursor);
		HCURSOR GetHCursor();

		int SetVCursor(HCURSOR hCursor);
		HCURSOR GetVCursor();

		int SetOrientation(SPLITTER_ORIENTATION eOrientation);
		SPLITTER_ORIENTATION GetOrientation() const;

		int AddPanel(CWnd *pPanel1, CWnd *pPanel2);	// pPanel1: left or top panel, pPanel2: right or bottom panel.
		CWnd *GetPanel1();
		CWnd *GetPanel2();
		int ReplacePanel1(CWnd *pNewPanel1);		// Replace panel1 pointer to another.
		int ReplacePanel2(CWnd *pNewPanel2);		// Replace panel1 pointer to another.

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSplitterBar)
	protected:
		virtual int OnDraw(CDC *pdcMem);

		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

	protected:
		DECLARE_MESSAGE_MAP()

	protected:
		void *m_pMember;
	};


	//////////////////////////////////////////////////////////////////////////
	//

	class CSubtitleWnd : public CUIWndBase
	{
		// Construction
	public:
		CSubtitleWnd();
		virtual ~CSubtitleWnd();

		BOOL Create(DWORD dwExStyle, DWORD dwStyle, LPCTSTR lpszWindowName, const RECT &rect, CWnd *pParentWnd, CCreateContext *pContext = NULL);

	public:
		// Window background
		int ShowWindowBackground(BOOL bShow);
		BOOL IsWindowBackgroundVisible() const;

		int SetWindowTransparent(int nTransparent);	// 0-255
		int GetWindowTransparent() const;

		int SetWindowBkColor(Color crBk);
		Color GetWindowBkColor() const;

		// Line
		int SetLineCount(int nCount);
		int GetLineCount() const;

		int SetLineSpacing(int nLineSpacing);
		int GetLineSpacing() const;

		// Stroke
		int EnableStroke(BOOL bEnable);
		BOOL IsStrokeEnabled() const;

		int SetStrokeColor(Color crStroke);
		Color GetStrokeColor() const;

		int SetStrokeCount(int nCount);
		int GetStrokeCount() const;

		// Shadow
		int EnableShadow(BOOL bEnable);
		BOOL IsShadowEnabled() const;

		// Text
		int SetSubtitle(int nLine, LPCTSTR lpszSubtitle, BOOL bRedraw = TRUE);
		int GetSubtitle(int nLine, CString &strSubtitle) const;

		int SetFont(LPCTSTR lpszFontID);
		int GetFont(CString *pstrFontID) const;

		int EnableKaraokeMode(BOOL bEnable);
		BOOL IsKaraokeModeEnabled() const;

		// Set background of played text
		int SetPlayedTextBackgroundMode(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetPlayedTextBackgroundMode() const;

		int SetPlayedTextBkColor(const std::vector<Color> *pvColors);
		int GetPlayedTextBkColor(std::vector<Color> *pvColors) const;

		int SetPlayedTextBkImage(LPCTSTR lpszImageName);
		int GetPlayedTextBkImage(CString *pstrImageName);

		// Set background of will be playing text
		int SetPlayingTextBackgroundMode(BACKGROUND_TYPE eBkType);
		BACKGROUND_TYPE GetPlayingTextBackgroundMode() const;

		int SetPlayingTextBkColor(const std::vector<Color> *pvColors);
		int GetPlayingTextBkColor(std::vector<Color> *pvColors) const;

		int SetPlayingTextBkImage(LPCTSTR lpszImageNameBk);
		int GetPlayingTextBkImage(CString *pstrImageNameBk);

		// Scroll
		int SetRange(int nMix, int nMax);
		int GetRange(int *pnMix, int *pnMax) const;

		int SetPos(int nPos);
		int GetPos() const;

	public:
		int Refresh();

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSubtitleWnd)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CSubtitleWnd)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void *m_pMember;
	};

	class CShareImage
	{
	public:
		CShareImage();
		virtual ~CShareImage();

	public:
		int m_nRefCount;
		CIUIImage m_imgShared;

		CONTROL_TYPE m_eControlType;
		CIUIImage m_imgPart[8];
	};

	//////////////////////////////////////////////////////////////////////////
	// CResMgr share the resource
	;
	class CResMgr
	{
	public:
		CResMgr();
		virtual ~CResMgr();

	public:
		static int AddImage(const CString &strImageName, __in CShareImage *pSharedImage);
		static int GetImage(const CString &strImageName, CShareImage **ppSharedImage);

	protected:
		static std::map<CString, CShareImage *> m_mapSharedImage;
	};

	//////////////////////////////////////////////////////////////////////////
	// CUIMgr parse and save *.xui
	;
	extern LPCTSTR g_pszDefaultFontID;
	class CUIMgr
	{
	public:
		CUIMgr();
		virtual ~CUIMgr();

	public:
		// Path
		;
		static int SetDelayLoadWindow(BOOL bDelayLoadWindow);
		static BOOL IsDelayLoadWindow();

		static int SetUIPath(LPCTSTR lpszUIPath, BOOL bCheckVersion = TRUE);
		static int SetUIPath(LPCTSTR lpszUIID, LPCTSTR lpszResouceID, LPCTSTR lpszResourceType);
		static int SetSource(LPCTSTR lpszUIPath);

		static CString GetUIPath();
		static CString GetUIResourcePath();
		static CString GetUIPathWithoutTitle();
		static CString GetUIResourceType();

		static BOOL IsLoadFromFile();

		// Load, save and release skin
		;
		static int LoadSkin();
		static int LoadIDDefined();

		static int DelayLoadWindow(UIWNDPROPERTIES *pUIWndProp);

		static int SaveSkin();
		static int SaveIDDefined();
		static int ReleaseSkin();

		static int SaveSkin(std::vector<UIWNDPROPERTIES *> vpWindows, LPCTSTR lpszTarget);

		// Version
		;
		static LONG GetXUIMajorVersion();
		static LONG GetXUIMinorVersion();
		static LONG GetXUIBuildVersion();
		static LONG GetXUIChangeList();
		static LONG GetLibUIDKMajorVersion();
		static LONG GetLibUIDKMinorVersion();
		static LONG GetLibUIDKBuildVersion();
		static LONG GetLibUIDKChangeList();

		// DPI
		;
		static LONG GetLogPixelsX();
		static LONG GetLogPixelsY();
		static int EnableDPI(BOOL bEnable);
		static BOOL IsEnableDPI();
		static int RunAsDPIX(int nXDPI);
		static BOOL IsRunAsDPIX();
		static int RunAsDPIY(int nYDPI);
		static BOOL IsRunAsDPIY();
		static int GetRunAsDPIX(int *pnDPIX);
		static int GetRunAsDPIY(int *pnDPIY);

		// Window
		;
		static UIWNDPROPERTIES *AllocUIWNDPROPERTIES();
		static int FreeUIWNDPROPERTIES(UIWNDPROPERTIES *pUIProp);
		static int GetWindowCount();
		static UIWNDPROPERTIES *GetWindowItem(int nIndex);
		static int AddWindowItem(UIWNDPROPERTIES *pWindow);
		static int DeleteWindowItem(int nIndex);
		static int DeleteAllWindowItems();
		static int GetWindowIndex(const UIWNDPROPERTIES *pUIProp);
		static int GetWindowIndex(const CString &strWindowID);
		static int SwapWindowItem(int nFirst, int nLast);
		static std::vector<UIWNDPROPERTIES *> *GetUIWndPropertiesList();
		static UIWNDPROPERTIES *GetUIWNDPROPERTIES(LPCTSTR lpszWindowID);
		static UIWNDPROPERTIES *GetUIWNDPROPERTIES(UINT uWindowID);
		static int InitRunningUIWnd();

		// Get Parent CUIWnd properties from children property.
		;
		static UIWNDPROPERTIES *GetParent(const IUI_VARIANT *pVarCtrlProp, BOOL *pbParentWindow);

		// Find
		;
		static int FindCtrlProp(/*in*/const IUI_VARIANT *pVarCtrlProp, /*out*/CTRLPROPERTIES **ppCtrlProp);

		// Resource
		;
		static int GetResourceCount(RESOURCE_TYPE eResType);

		// Font
		;
		static int SetDefaultFont(LPCTSTR lpszFont); // Set default font id macro define in XUI file.
		static CString GetDefaultFont();

		static int GetFontResource(const CString &strCondition, LPCTSTR lpszResID, LOGFONT *plf);
		static RESFONTPROPERTIES *FindFontResource(const IUI_VARIANT &varFont);

		static RESFONTPROPERTIES *GetDefaultFontResource(const CString &strCondition);
		static FONTRESOURCEITEM *GetDefaultFontResource();

		static int AddFontResourceItem(FONTRESOURCEITEM *pFontResource);

		static int DeleteFontResourceItem(int nIndex);
		static int DeleteFontResourceItem(const RESFONTPROPERTIES *pResProp);
		static int DeleteFontResourceItem(const FONTRESOURCEITEM *pResFont);

		static FONTRESOURCEITEM *GetFontResourceItem(int nIndex);
		static FONTRESOURCEITEM *GetFontResourceItem(LPCTSTR lpszResID);

		// Color
		;
		static int GetColorResource(LPCTSTR lpszResID, COLORREF *pcr);
		static RESCOLORPROPERTIES *FindColorResource(const IUI_VARIANT &varColor);

		static RESCOLORPROPERTIES *GetDefaultColorResource();

		static int AddColorResourceItem(CTRLPROPERTIES *pResource);

		static int DeleteColorResourceItem(int nIndex);
		static int DeleteColorResourceItem(const RESCOLORPROPERTIES *pResProp);

		static RESCOLORPROPERTIES *GetColorResourceItem(int nIndex);
		static RESCOLORPROPERTIES *GetColorResourceItem(LPCTSTR lpszResID);

		// Style
		;
		static int GetStyleCount(STYLE_TYPE eStyleType);

		static int AddStyleItem(STYLE_TYPE eStyleType, CTRLPROPERTIES *pStyle);

		static int DeleteStyleItem(STYLE_TYPE eStyleType, int nIndex);
		static int DeleteStyleItem(STYLE_TYPE eStyleType, const CTRLPROPERTIES *pStyleProp);

		static CTRLPROPERTIES *GetStyleItem(STYLE_TYPE eStyleType, int nIndex);
		static CTRLPROPERTIES *GetStyleItem(STYLE_TYPE eStyleType, LPCTSTR lpszStyleID);

		static std::vector<BTNPROPERTIES *> *GetStylePushButton();
		static std::vector<CHKPROPERTIES *> *GetStyleCheckBox();
		static std::vector<RADPROPERTIES *> *GetStyleRadioButton();
		static std::vector<CMBPROPERTIES *> *GetStyleComboBox();
		static std::vector<TXTPROPERTIES *> *GetStyleStatic();
		static std::vector<EDTPROPERTIES *> *GetStyleEdit();
		static std::vector<SLDPROPERTIES *> *GetStyleSlider();
		static std::vector<PRGPROPERTIES *> *GetStyleProgress();
		static std::vector<SPNPROPERTIES *> *GetStyleSpin();
		static std::vector<HOTPROPERTIES *> *GetStyleHotKey();
		static std::vector<IPAPROPERTIES *> *GetStyleIPAddress();
		static std::vector<SLDEXPROPERTIES *> *GetStyleSliderEx();
		static std::vector<LSTCTRLPROPERTIES *> *GetStyleListCtrl();
		static std::vector<TREECTRLPROPERTIES *> *GetStyleTreeCtrl();
		static std::vector<TREELISTPROPERTIES *> *GetStyleTreeList();
		static std::vector<RICHEDITPROPERTIES *> *GetStyleRichEdit();
		static std::vector<IMREPROPERTIES *> *GetStyleIMRichEdit();
		static std::vector<HTMLCTRLPROPERTIES *> *GetStyleHtmlCtrl();
		static std::vector<DATETIMECTRLPROPERTIES *> *GetStyleDateTime();
		static std::vector<WAVECTRLPROPERTIES *> *GetStyleWaveCtrl();
		static std::vector<MENUPROPERTIES *> *GetStyleMenu();
		static std::vector<MENUBARPROPERTIES *> *GetStyleMenuBar();
		static std::vector<HEADERPROPERTIES *> *GetStyleHeader();
		static std::vector<HORSBPROPERTIES *> *GetStyleHorScrollBar();
		static std::vector<VERSBPROPERTIES *> *GetStyleVerScrollBar();
		static std::vector<SPLITTERPROPERTIES *> *GetStyleSplitter();
		static std::vector<STACKPNLPROPERTIES *> *GetStylePanelStack();
		static std::vector<LINEWLPROPERTIES *> *GetStyleWLLine();
		static std::vector<PICWLPROPERTIES *> *GetStyleWLPic();

		// Control
		// Get the first control by lpszCtrlID within window lpszWindowID.
		;
		static CTRLPROPERTIES *GetCONTROL(LPCTSTR lpszWindowID, LPCTSTR lpszCtrlID);
		// Get the first control use the image 'lpszImage'.
		;
		static CTRLPROPERTIES *GetCONTROL(LPCTSTR lpszImage);

		// Image
		;
		static std::vector<CString> GetAllImages(BOOL bAllowDuplicate);

		// Multi-Language
		;
		static int CreateIniLanguageFile(LPCTSTR lpszLanguageFile);
		static int LoadIniLanguageFile(LPCTSTR lpszLanguageFile);
		static int SwitchIniLanguage(LPCTSTR lpszLanguageFile);
		static int SwitchIniLanguageOnRunTime(LPCTSTR lpszLanguageFile);

		static RECT MAKERECT(LPCTSTR lpszRect);
		static int StringIDToInt(LPCTSTR lpszIDs);
		static CString StringIDFromInt(int nID);
		static int GetMaxIDValue();

		// Alpha blend
		;
		static int SetAlphaBlendColor(COLORREF crAlphaBlend);
		static COLORREF GetAlphaBlendColor();

		// UI
		;
		static CUIWnd *LoadFrame(UINT nResID, DWORD dwStyle, UINT nWinID, CUIWndHandler *pHandler);
		static int DoModal(UINT nWinID, CUIWndHandler *pHandler);

	protected:
		static int ConvertUI2CurVer();	// convert *.ui to *.xui and save it
		static int ConvertXUI2CurVer();	// convert old version of xui file to current version.
		static int PromptConvert();

	private:
		static void LicenseWarning()
		{
			AfxMessageBox(_T("This libarary hasn't been licensed! register it from http://www.iuishop.com"));
			ASSERT(FALSE);
		}

	private:
		static BOOL m_bLoadFromFile;
		static BOOL m_bConverting;
		static BOOL m_bDelayLoadWindow;				// Load window properties delay.

		// for hard disk skin
		;
		static CString m_strUIPath;
		static CString m_strUIPathWithoutTitle;
		static CString m_strUIResourcePath;

		// for resource skin
		;
		static CString m_strUIID;
		static CString m_strUIResourcehID;
		static CString m_strUIResourceType;

		// for version
		;
		static LONG m_lXUIMajorVersion;
		static LONG m_lXUIMinorVersion;
		static LONG m_lXUIBuildVersion;
		static LONG m_lXUIChangeList;
		static LONG m_lLibUIDKMajorVersion;
		static LONG m_lLibUIDKMinorVersion;
		static LONG m_lLibUIDKBuildVersion;

		// for DPI
		;
		static BOOL m_bEnableDPI;
		static BOOL m_bRunAsDPIX;
		static BOOL m_bRunAsDPIY;
		static int m_nRunAsDPIX;
		static int m_nRunAsDPIY;
		static LONG m_lLogPixelsX;
		static LONG m_lLogPixelsY;

		// for *.xui and resource.h
		;
		static std::vector<UIWNDPROPERTIES *> m_vpWindows;
		static std::vector<std::pair<CString, int> > m_vIDs;
		static std::map<CString, int> m_mapStrIntIDs;
		static std::map<int, CString> m_mapIntStrIDs;
		static CMemStdioFile m_fileResourceH;

		// Font Resource
		;
		static CString m_strCurrentFont;
		static FONTRESOURCEITEM m_resDefaultFont;
		static std::vector<FONTRESOURCEITEM *> m_vpResFont;

		// Color Resource
		;
		static RESCOLORPROPERTIES m_resDefaultColor;
		static std::vector<RESCOLORPROPERTIES *> m_vpColorRes;

		// Style
		;
		static std::vector<BTNPROPERTIES *> m_vpStylePushButton;
		static std::vector<CHKPROPERTIES *> m_vpStyleCheckBox;
		static std::vector<RADPROPERTIES *> m_vpStyleRadioButton;
		static std::vector<CMBPROPERTIES *> m_vpStyleComboBox;
		static std::vector<TXTPROPERTIES *> m_vpStyleStatic;
		static std::vector<EDTPROPERTIES *> m_vpStyleEdit;
		static std::vector<SLDPROPERTIES *> m_vpStyleSlider;
		static std::vector<PRGPROPERTIES *> m_vpStyleProgress;
		static std::vector<SPNPROPERTIES *> m_vpStyleSpin;
		static std::vector<HOTPROPERTIES *> m_vpStyleHotKey;
		static std::vector<IPAPROPERTIES *> m_vpStyleIPAddress;
		static std::vector<SLDEXPROPERTIES *> m_vpStyleSliderEx;
		static std::vector<LSTCTRLPROPERTIES *> m_vpStyleListCtrl;
		static std::vector<TREECTRLPROPERTIES *> m_vpStyleTreeCtrl;
		static std::vector<TREELISTPROPERTIES *> m_vpStyleTreeList;
		static std::vector<RICHEDITPROPERTIES *> m_vpStyleRichEdit;
		static std::vector<IMREPROPERTIES *> m_vpStyleIMRichEdit;
		static std::vector<HTMLCTRLPROPERTIES *> m_vpStyleHtmlCtrl;
		static std::vector<DATETIMECTRLPROPERTIES *> m_vpStyleDateTime;
		static std::vector<WAVECTRLPROPERTIES *> m_vpStyleWaveCtrl;
		static std::vector<MENUPROPERTIES *> m_vpStyleMenu;
		static std::vector<MENUBARPROPERTIES *> m_vpStyleMenuBar;
		static std::vector<HEADERPROPERTIES *> m_vpStyleHeader;
		static std::vector<HORSBPROPERTIES *> m_vpStyleHorScrollBar;
		static std::vector<VERSBPROPERTIES *> m_vpStyleVerScrollBar;
		static std::vector<SPLITTERPROPERTIES *> m_vpStyleSplitter;
		static std::vector<STACKPNLPROPERTIES *> m_vpStylePanelStack;
		static std::vector<LINEWLPROPERTIES *> m_vpStyleWLLine;
		static std::vector<PICWLPROPERTIES *> m_vpStyleWLPic;

		// for multi-language
		;
		static std::map<CString, CString> m_mapLanguage;			// m_mapLanguage["WindowID/ControlID"] = Caption
		static std::map<HWND, CString> m_mapCreatedHWndStringID;	// m_mapCreatedHWndStringID[HWND] = "WindowStringID"

		// for alpha blend
		;
		static COLORREF m_crAlphaBlend;

		// For gdi+
		static ULONG_PTR m_gdiplusToken;

	public:
		// Next font resource ID for convert.
		;
		static int m_nNextFontResID;

		// Next color resource ID for convert.
		;
		static int m_nNextColorResID;

		// Next header resource ID for convert.
		;
		static int m_nNextHeaderResID;
	};


	//////////////////////////////////////////////////////////////////////////
	// CTaskWndMgr

#define WS_TASKSTYLE WS_VISIBLE | WS_CHILD

	class CTaskWndInfo
	{
	public:
		CTaskWndInfo();
		~CTaskWndInfo();

	public:
		BOOL m_bRadioButton;
		int m_nRadioButtonID;
		CSkinButton *m_pRadTab; // for dynamically create tab, need release by LibUIDK.
		BOOL m_bUINoCodeMode;
		CWnd *m_pTaskWnd;
		UINT m_nTaskWndID;
		DWORD m_dwTaskWndCreateStyle;
		CString m_strWindowName;
	};

	class CTaskParam
	{
	public:
		CTaskParam();
		~CTaskParam();

	public:
		CTaskWndMgr *m_pTaskWndMgr;
		CTaskWndInfo *m_pSwitchTo;
		CTaskWndInfo *m_pTobeHide;
	};

	class CTaskWndMgr : public CWLPicture
	{
	public:
		CTaskWndMgr();
		virtual ~CTaskWndMgr();

	public:
		int ReleaseObject();
		virtual BOOL DestroyWindow();

	public:
		// make nTaskRadioButtonID bind to uTaskWndID. AddTaskWindow will call xxx to get the CUIWndHandler
		int AddTaskWindow(
			UINT nTaskRadioButtonID,
			BOOL bRadioButton,
			UINT uTaskWndID,
			LPCTSTR lpszWindowName = NULL,
			DWORD dwStyle = WS_TASKSTYLE);

		// make nTaskRadioButtonID bind to pTaskPanel.
		int AddTaskWindow(
			UINT nTaskRadioButtonID,
			BOOL bRadioButton,
			CWnd *pTaskPanel,
			UINT nTaskWndID,
			LPCTSTR lpszWindowName = NULL,
			DWORD dwStyle = WS_TASKSTYLE);

		// Dynamically create property sheet.
		int AddTaskWindow(
			UINT nTaskRadioButtonID,
			const RECT &rcTab,
			LPCTSTR lpszBindTabStyle,
			LPCTSTR lpszTabName,
			UINT uTaskWndID,
			__out CSkinButton **ppRadTab = NULL,
			LPCTSTR lpszWindowName = NULL,
			DWORD dwStyle = WS_TASKSTYLE);

		int RemoveTaskWindow(UINT nTaskRadioButton, BOOL bDestroyTaskWindow);
		int RemoveAllTaskWindows(BOOL bDestroyTaskWindow);
		int DestroyTaskWindow(UINT nTaskRadioButton);
		int GetTaskWindowCount();
		// bNotFoundHideAll - TRUE: if not found nID tab, hide all task window.
		int SwitchToTaskWindow(UINT nID, BOOL bSetFocus = FALSE, BOOL bNotFoundHideAll = FALSE);
		int SwitchToTaskWindow(CSkinButton *pRadTask, BOOL bSetFocus = FALSE);
		int SwitchToTaskWindow(const CWnd *pTaskWnd, BOOL bSetFocus = FALSE);
		int SwitchToTaskWindowIndex(int nIndex, BOOL bSetFocus = FALSE);
		int SwitchToNextTaskWindow();
		int SwitchToPrevTaskWindow();
		const CTaskWndInfo *FindTaskWindow(UINT nTaskRadioButton) const;
		CTaskWndInfo *FindTaskWindow(UINT nTaskRadioButton);
		const CTaskWndInfo *FindTaskWindow(CWnd *pTaskPaenl) const;
		CTaskWndInfo *FindTaskWindow(CWnd *pTaskPaenl);
		const CTaskWndInfo *FindTaskWindowByTaskID(UINT nTaskID) const;
		CTaskWndInfo *FindTaskWindowByTaskID(UINT nTaskID);
		const CTaskWndInfo *GetTaskWindow(int nIndex) const;
		CTaskWndInfo *GetTaskWindow(int nIndex);
		UINT GetTaskRadioButtonID(CWnd *pTaskPaenl);
		UINT GetCurrentTaskWindow() const;
		int GetCurrentTaskWindowIndex() const;

		// Send message to all created task windows.
		// bSend - TRUE: SendMessage; FALSE: PostMessage
		LRESULT BroadcastMessageToAllTaskWnds(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bSend);

		// Send message to spec task window.
		LRESULT SendMessageToTaskWnd(UINT nTaskRadioButton, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual int MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

		int EnableSmooth(BOOL bSmooth);
		BOOL IsSmoothEnabled() const;

		int SetSmoothSpeed(int nSpeed);
		int GetSmoothSpeed() const;

		int SetSmoothInterval(int nInterval);
		int GetSmoothInterval() const;

	private:
		void *m_pMember;
	};

	//////////////////////////////////////////////////////////////////////////
	// Panel

	class CPanel : public CWLPicture
	{
	public:
		CPanel();
		virtual ~CPanel();

	public:
		virtual int InsertPanel(CPanel *pChildPanel, UINT nPos);
		virtual int RelayoutElements(BOOL bRedraw = FALSE);
		virtual int RemoveAllElements();

		BOOL IsCreated();

		int SetParentPanel(CPanel *pParentPanel);
		CPanel *GetParentPanel();

		int SetRedraw(BOOL bRedraw);
		int Redraw();

	protected:
		CPanel *m_pParentPanel;
		BOOL m_bRedraw;
	};

	struct STACK_ELEMENT
	{
		PANEL_ELEMENT_TYPE m_ePet; // CWnd * or CPanel *
		void *m_pElement;	// Point to CWnd * or CPanel *
	};

	struct DOCK_ELEMENT
	{
		PANEL_ELEMENT_TYPE m_ePet;
		void *m_pElement;

		DOCK_STYLE m_eDockStyle;
	};

	class CStackPanel : public CPanel
	{
	public:
		CStackPanel();
		virtual ~CStackPanel();

		// Init control by specify style.
		;
		virtual int BindStyle(LPCTSTR lpszStyleID);
		virtual int BindStyle(const CTRLPROPERTIES *pCtrlProp);

		virtual int ReleaseObject();

	protected:
		static LRESULT ElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		virtual int RelayoutElements(BOOL bRedraw = FALSE);
		virtual int MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

		int SetOrientation(STACK_PANEL_ORIENTATION eOrientation);

		int SetLastChildFill(BOOL bFill);
		BOOL IsLastChildFill() const;

		int AddElement(CWnd *pWnd, BOOL bLayout = TRUE);
		int AddElement(CPanel *pPanel);
		CUIWnd *AddElement(UINT uWinID, DWORD dwStyle);
		int RemoveElement(int nIndex);
		int RemoveElement(CWnd *pWnd);
		int MoveElementTo(STACK_ELEMENT *pWndFrom, int nNewIndex);
		int MoveElementTo(STACK_ELEMENT *pWndFrom, STACK_ELEMENT *pWndLast);
		int GetElementCount();
		int RemoveAllElements();

		int EnsureVisible(int nObjID, BOOL bPartialOK);
		int EnsureVisible(CWnd *pWnd, BOOL bPartialOK);

	protected:
		STACK_PANEL_ORIENTATION m_eOrientation;
		std::vector<STACK_ELEMENT> m_vElements;
		SCROLLINFO m_si;
		BOOL m_bLastChildFill;
	};

	//////////////////////////////////////////////////////////////////////////
	// CDockPanel
	;
	class CDockPanel : public CPanel
	{
	public:
		CDockPanel();
		virtual ~CDockPanel();
		virtual int ReleaseObject();

	protected:
		static LRESULT ElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		virtual int RelayoutElements(BOOL bRedraw = FALSE);
		virtual int MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

	public:
		int SetLastChildFill(BOOL bFill);
		BOOL IsLastChildFill() const;

		int AddElement(CWnd *pWnd, DOCK_STYLE eDockStyle, BOOL bLayout = TRUE);
		int AddElement(CWLWnd *pWnd, DOCK_STYLE eDockStyle, BOOL bLayout = TRUE);
		int AddElement(CPanel *pPanel, DOCK_STYLE eDockStyle, BOOL bLayout = TRUE);
		int AddElement(UINT uCtrlID, DOCK_STYLE eDockStyle, BOOL bLayout = TRUE);
		CUIWnd *AddChildWindowElement(UINT uWinID, DOCK_STYLE eDockStyle, BOOL bLayout = TRUE);
		int RemoveElement(int nIndex, BOOL bLayout = TRUE);
		int RemoveElement(CWnd *pWnd, BOOL bLayout = TRUE);
		int GetElementCount();
		virtual int RemoveAllElements();
		int ChangeElementOrder(CObject *pElement, int nNewIndex);
		int FindElement(const CObject *pElement, DOCK_ELEMENT *pDockElement);
		int FindElement(UINT uWinID, DOCK_ELEMENT *pDockElement);

		int SetDock(const CObject *pElement, DOCK_STYLE eDockStyle);
		DOCK_STYLE GetDock(const CObject *pElement) const;

		int SetElementSize(const CObject *pElement, int nX, int nY);	// Ignore the nX if dock style is DT_TOP or DT_BOTTOM, and nY if DT_LEFT or DT_RIGHT.

	protected:
		std::vector<DOCK_ELEMENT> m_vElements;
		BOOL m_bLastChildFill;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	;
	class CGridPanelElement
	{
	public:
		CGridPanelElement();
		virtual ~CGridPanelElement();

	public:
		GRID_ELEMENT_TYPE m_eGet;
		CWnd *m_pWnd;
		CPanel *m_pChildPanel;
		GRID_ELEMENT_ALIGN_HOR m_eHorAlign;
		GRID_ELEMENT_ALIGN_VER m_eVerAlign;
		int m_nRowSpan;		// Only valid while m_eVerAlign is GEAV_STRETCH
		int m_nColumnSpan;	// Only valid while m_eHorAlign is GEAH_STRETCH
	};

	//////////////////////////////////////////////////////////////////////////
	// CGridPanel
	;
	class CGridPanel : public CPanel
	{
	public:
		CGridPanel();
		virtual ~CGridPanel();
		BOOL Create(LPRECT lpRect, CUIWnd *pParent, CWLWnd *pVirtualParent, CPanel *pParentGridPanel = NULL, UINT nParentRowColID = MAKELPARAM(0, 0));
		BOOL CreateStatic(LPRECT lpRect, int nRows, int nCols, CUIWnd *pParent, CWLWnd *pVirtualParent, CPanel *pParentGridPanel = NULL, UINT nParentRowColID = MAKELPARAM(0, 0));

	protected:
		static LRESULT ElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		virtual int RelayoutElements(BOOL bRedraw = FALSE);
		virtual int InsertPanel(CPanel *pChildPanel, UINT nPos);
		virtual int GetCellRect(int nRow, int nCol, LPRECT lpRect) const;

		int SetGridHorizontalUnit(GRID_PANEL_UINT eGpu);
		GRID_PANEL_UINT GetGridHorizontalUnit() const;

		int SetGridVerticalUnit(GRID_PANEL_UINT eGpu);
		GRID_PANEL_UINT GetGridVerticalUnit() const;

		// Row
		;
		int InsertRow(int nIndex, int nHeight);
		int GetRowCount() const;
		int SetRowHeight(int nIndex, int nHeight);
		int GetRowHeight(int nIndex) const;
		int DeleteRow(int nIndex);

		// Column
		;
		int InsertColumn(int nIndex, int nWidth);
		int GetColumnCount() const;
		int SetColumnWidth(int nIndex, int nWidth);
		int GetColumnWidth(int nIndex) const;
		int DeleteColumn(int nIndex);

		int AddElement(CWnd *pWnd, int nRow, int nColumn, GRID_ELEMENT_ALIGN_HOR eHorAlign, GRID_ELEMENT_ALIGN_VER eVerAlign);
		int RemoveElement(int nRow, int nColumn);
		int RemoveElement(CWnd *pWnd);
		int RemoveAllElements();

	protected:
		CGrid m_Data;
		GRID_PANEL_UINT m_eHoriGpu;
		GRID_PANEL_UINT m_eVertGpu;

	};

	//////////////////////////////////////////////////////////////////////////
	// CUniformGridPanel
	;
	class CUniformGridPanel : public CPanel
	{
	public:
		CUniformGridPanel();
		virtual ~CUniformGridPanel();
		virtual int ReleaseObject();

	protected:
		static LRESULT ElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		LRESULT WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		afx_msg virtual int OnShowWindow(BOOL bShow);
		afx_msg virtual int OnDeletingItem(int nRow, int nColumn, CWnd *pWndDeleting);
		afx_msg virtual int OnDeletedItem(int nRow, int nColumn, CWnd *pWndDeleted);

	public:
		virtual int RelayoutElements(BOOL bRedraw = FALSE);
		virtual int MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

		// for grid
		;
		int InsertRow(int nIndex);
		int GetRowHeight();
		int InsertColumn(int nIndex);
		int GetColumnWidth();

		int GetRowCount() const;
		int GetColumnCount() const;

		int RemoveAllGrid();

		int GetGridRect(int nRow, int nColumn, LPRECT lprcCell, LPRECT lprcElement);

		// for element
		;
		int AddElement(CWnd *pWnd, int nRow, int nColumn, GRID_ELEMENT_ALIGN_HOR eHorAlign, GRID_ELEMENT_ALIGN_VER eVerAlign);
		int RemoveElement(int nRow, int nColumn, BOOL bRelayout = TRUE);
		int RemoveElement(CWnd *pWnd, BOOL bRelayout = TRUE);
		virtual int RemoveAllElements(BOOL bRelayout = TRUE);
		int FindElement(const CObject *pElement, CGridPanelElement **ppgpe, int *pnRow, int *pnColumn) const;
		int FindElement(HWND hElement, CGridPanelElement **ppgpe, int *pnRow, int *pnColumn) const;
		int SetElementSpan(const CObject *pElement, int nRowSpan, int nColumnSpan);
		CGridPanelElement *GetElement(int nRow, int nColumn);

	protected:
		CGrid m_Data;
	};

	//////////////////////////////////////////////////////////////////////////
	// CNewWindowsUIPanel

	class CNewWindowsUIPanel;

	struct NWUIITEM
	{
		NMHDR hdr;
		int nItem;
		POINT ptAction;
		LPARAM lParam;
	};

	// New Windows UI is the metro UI.
	class CNewWindowsUIPanel : public CUniformGridPanel
	{
	public:
		CNewWindowsUIPanel();
		~CNewWindowsUIPanel();

	protected:
		friend class CNWUIOverItemWnd;
		friend LRESULT NWUIElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		afx_msg virtual int OnDeletedItem(int nRow, int nColumn, CWnd *pWndDeleted);
		// Only LibUIDK call.
		static VOID CALLBACK CNewWindowsUIPanel_OnTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
		afx_msg void OnTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult);

	public:
		int HitTestItem(HWND hWndSender, const CRect &rcSender);

		// Only animation, the item order don't change.
		int AnimateAdjustItemOrder(int nIndexFrom, int nIndexTo);

		// Change item's order
		int MoveItem(int nIndexFrom, int nIndexTo);

		int EnableDrag(BOOL bDrag);
		BOOL IsEnableDrag();

		int AddElement(CWnd *pWnd, GRID_ELEMENT_ALIGN_HOR eHorAlign, GRID_ELEMENT_ALIGN_VER eVerAlign);
		int GetElementCount() const;
		CGridPanelElement *GetElement(int nIndex);
		virtual int RemoveAllElements(BOOL bRelayout = TRUE);

	private:
		CGrid m_DataTemp;	// Only for animation

	private:
		UINT m_uClipFormat;
		CIUIOleDropTarget m_drop;

		int m_nElementCount;
		BOOL m_bEnableDrag;

		int m_nAdjustItemOrderTimerID;
		int m_nTimerTick;
		int m_nMaxTimerStep;

		// Draw item 0, over position 1, over position 4, drop. the m_nBeginDragItemIndex is 0, the m_nIndexTo is 4. while the drag icon over position 1, m_nDragOverFromIndex is 0, while over position 4, it is 1.
		int m_nBeginDragItemIndex;		// The item index begin drag
		int m_nDragOverFromIndex;		// The position index while dragging, this value may be not equal m_nBeginDragItemIndex. such as drag item from 0 to 1, than to 2. at 1th step, the m_nDragOverFromIndex is 0, at 2th step, the m_nDragOverFromIndex is 1. but m_nBeginDragItemIndex alway 0.
		int m_nIndexTo;
	};

	//////////////////////////////////////////////////////////////////////////
	// CIUIDocument

	class CIUIDocument : public CDocument
	{
		friend CIUIMultiDocTemplate;
		friend CIUIFrameWnd;
		friend CIUIMDIFrameWnd;
		friend CIUIMDIClientWnd;
		friend CIUIView;
		DECLARE_DYNCREATE(CIUIDocument)

		// Don't call the following private interface
	private:
		virtual BOOL CanCloseFrame(CFrameWnd *pFrame)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		virtual void PreCloseFrame(CFrameWnd *pFrame)
		{
			ASSERT(FALSE);
		}

	protected:
		CIUIDocument();           // protected constructor used by dynamic creation
		virtual ~CIUIDocument();

	public:
		void AddView(CIUIView *pView);


		// Overrides
	public:
		virtual void Serialize(CArchive &ar);   // overridden for document i/o
		virtual BOOL CanCloseIUIFrame(CIUIFrameWnd *pIUIFrame); ///
		virtual void PreCloseIUIFrame(CIUIFrameWnd *pIUIFrame); ///
		virtual void UpdateFrameCounts();
		virtual CIUIView *GetNextIUIView(POSITION &rPosition) const;
	protected:
		virtual BOOL OnNewDocument();
		virtual void OnCloseDocument();

		// Implementation
	public:
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext &dc) const;
#endif

		// Generated message map functions
	protected:
		//{{AFX_MSG(CIUIDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};


	//////////////////////////////////////////////////////////////////////////
	// Document class that support non MDI frame

	class CAction
	{
	public:
		CAction();
		CAction(const CAction &action);
		~CAction();

		CAction &operator = (const CAction &action);

	public:
		enum Actions
		{
			actionUnknown,
			actionNew,
			actionDelete,
			actionEdit
		};

	public:
		Actions m_eAction;
		LPARAM m_lParam;
	};

	// Recycle bin
	class CRecycleBin
	{
	public:
		CRecycleBin();
		virtual ~CRecycleBin();

	protected:
		LPARAM m_lParam;
	};

	class CIUIDocument2
	{
	public:
		CIUIDocument2();
		virtual ~CIUIDocument2();

	protected:
		int ReleseRedoList();

		// Attributes
	public:
		const CString &GetTitle() const;
		virtual void SetTitle(LPCTSTR lpszTitle);
		virtual int SetDefaultTitle();
		const CString &GetPathName() const;
		virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
		void SetResourceID(UINT nID);
		UINT GetResourceID();

	public:
		virtual BOOL CanCloseFrame(CUIWnd *pFrame);
		virtual BOOL SaveModified();
		virtual BOOL DoFileSave();
		virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
		virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

		// helper for standard commdlg dialogs
		virtual BOOL DoPromptFileName(CString &fileName, UINT nIDSTitle,
			DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate *pTemplate);
		virtual int OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);


		//
		int OnFileNew();		// ID_FILE_NEW handler
		int OnFileOpen();		// ID_FILE_OPEN handler
		int OnFileSave();		// ID_FILE_SAVE handler
		int OnFileSaveAs();		// ID_FILE_SAVE_AS handler

		int AddView(CWnd *pView);
		int UpdateAllViews(CWnd *pSender);

		// Undo and redo
		int AddUndo(const CAction *pAction);
		int Undo();
		BOOL CanUndo();

		int Redo();
		BOOL CanRedo();

		// Modified
		virtual BOOL IsModified();
		virtual void SetModifiedFlag(BOOL bModified = TRUE);

		virtual BOOL OnNewDocument();
		virtual void DeleteContents(); // delete doc items etc

	protected:
		UINT m_nIDResource;
		CString m_strTitle;
		CString m_strPathName;
		CString m_strDocStrings;
		std::vector<CWnd *> m_vpViews;

		// Undo & Redo
		int m_nMaxUndo;
		CList<CAction, CAction &> m_Undo;
		CList<CAction, CAction &> m_Redo;
		CRecycleBin m_RecycleBin;

		BOOL m_bModified;
	};

	//////////////////////////////////////////////////////////////////////////
	// CIUIWinApp

	class CIUIWinApp : public CWinApp
	{
		friend class CIUIMDIFrameWnd;
	public:
		CIUIWinApp();

	protected:
		void OnFileNew();
		void OnFileOpen();

		void AddDocTemplate(CDocTemplate *pTemplate);

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CIUIWinApp)
		//}}AFX_VIRTUAL

	public:
		//{{AFX_MSG(CIUIWinApp)
		afx_msg void OnUpdateRecentFileMenu(CCmdUI *pCmdUI);
		afx_msg BOOL OnOpenRecentFile(UINT nID);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};

	//////////////////////////////////////////////////////////////////////////
	// CIUIView

	class CIUIView : public CUIWnd
	{
		friend class CIUIDocument;
		DECLARE_DYNCREATE(CIUIView)

	public:
		CIUIView();
		virtual ~CIUIView();
		void WindowID()
		{
			ASSERT(FALSE);
		}

	public:
		CIUIDocument *GetDocument();

	public:
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CIUIView)
		virtual void PostNcDestroy();   // default to delete this.
		virtual void OnActivateView(BOOL bActivate, CIUIView *pActivateView, CIUIView *pDeactiveView); ///
		virtual void OnActivateFrame(UINT nState, CIUIFrameWnd *pFrameWnd); ///
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CIUIView)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnMDIActivate(BOOL bActivate, CWnd *, CWnd *);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		CIUIDocument *m_pDocument;
	};

	//////////////////////////////////////////////////////////////////////////
	// CIUIFrameWnd

	class CIUIFrameWnd : public CUIWnd
	{
		friend class CIUIDocument;
		DECLARE_DYNCREATE(CIUIFrameWnd)

	public:
		static AFX_DATA const CRect rectDefault;
		CIUIFrameWnd();
		virtual ~CIUIFrameWnd();
		void WindowID()
		{
			ASSERT(FALSE);
		}

		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd *pParentWnd, BOOL bLoadBkImageFromFile = TRUE, BOOL bUseLayeredWindow = TRUE);

	protected:
		LPCTSTR GetIconWndClass(DWORD dwDefaultStyle, UINT nIDResource); ///
		int OnCreateHelper(LPCREATESTRUCT lpcs, CCreateContext *pContext);	///

	public:
		CIUIMDIFrameWnd *GetMDIFrame();
		void MDIDestroy();
		virtual CIUIDocument *GetActiveDocument();	///
		CString GetTitle() const
		{
			return m_strTitle;    ///
		}
		CIUIView *GetActiveView() const;           /// active view or NULL
		void SetActiveView(CIUIView *pViewNew, BOOL bNotify); ///
		void InitialUpdateFrame(CDocument *pDoc, BOOL bMakeVisible);	///
		void BringToTop(int nCmdShow); ///
		CWnd *CreateView(CCreateContext *pContext, UINT nID = AFX_IDW_PANE_FIRST); ///

	public:
		// Overrides
		virtual void ActivateFrame(int nCmdShow = -1);	///
		virtual void OnUpdateFrameTitle(BOOL bAddToTitle);	///
		virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext *pContext);	///
		virtual void RecalcLayout(BOOL bNotify = TRUE); ///
		virtual BOOL PreCreateWindow(CREATESTRUCT &cs); ///
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		// customize if using an 'Window' menu with non-standard IDs
		virtual HMENU GetWindowMenuPopup(HMENU hMenuBar);	///

		// Generated message map functions
	protected:
		//{{AFX_MSG(CIUIFrameWnd)
		afx_msg void OnClose();	///
		afx_msg void OnMDIActivate(BOOL bActivate, CWnd *, CWnd *);
		afx_msg void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex, BOOL bSysMenu);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		CString m_strTitle;			/// default title (original)
		HMENU m_hMenuDefault;		/// default menu resource for this frame
		HACCEL m_hAccelTable;		/// accelerator table
		int m_nWindow;	///  // general purpose window number - display as ":n"
		// -1 => unknown, 0 => only window viewing document
		// 1 => first of many windows viewing document, 2=> second
		CIUIView *m_pViewActive;       // current active view
		// TRUE => menu items without handlers will be disabled
	};

	//////////////////////////////////////////////////////////////////////////
	// CIUIMDIChildWnd

	class CIUIMDIChildWnd : public CIUIFrameWnd
	{
		friend class CIUIMDIFrameWnd;
		DECLARE_DYNCREATE(CIUIMDIChildWnd)

	public:
		CIUIMDIChildWnd();
		virtual ~CIUIMDIChildWnd();
		void WindowID()
		{
			ASSERT(FALSE);
		}
		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd *pParentWnd, CCreateContext *pContext = NULL);	///
		virtual BOOL Create(LPCTSTR lpszClassName,
			LPCTSTR lpszWindowName,
			DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
			const RECT &rect = rectDefault,
			CIUIMDIFrameWnd *pParentWnd = NULL,
			CCreateContext *pContext = NULL);	///

	public:
		CIUIMDIFrameWnd *GetMDIFrame();
		void MDIDestroy();
		virtual void ActivateFrame(int nCmdShow = -1); ///
		void AfxSetWindowText2(LPCTSTR lpszNew);

	public:
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CIUIMDIChildWnd)
	public:
		virtual BOOL PreCreateWindow(CREATESTRUCT &cs); ///
		virtual BOOL DestroyWindow();
		virtual void PostNcDestroy();   // default to delete this.
		virtual void OnUpdateFrameTitle(BOOL bAddToTitle);	///
	protected:
		virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);	///
		virtual void OnUpdateFrameMenu(BOOL bActive, CWnd *pActivateWnd, HMENU hMenuAlt); ///
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:
		//{{AFX_MSG(CIUIMDIChildWnd)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	///
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnMDIActivate(BOOL bActivate, CWnd *, CWnd *); ///
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		CIUIDocument *m_pDocument;
		BOOL m_bPseudoInactive;     // TRUE if window is MDI active according to
		//  windows, but not according to MFC...
		BOOL m_bVisible;
		HMENU m_hMenuShared;        /// menu when we are active
	};

	/////////////////////////////////////////////////////////////////////////////
	// CIUIMDIClientWnd window

	class CIUIMDIClientWnd : public CUIWnd
	{
		friend class CIUIMDIChildWnd;
		friend class CIUIMDIFrameWnd;
	public:
		CIUIMDIClientWnd();
		virtual ~CIUIMDIClientWnd();
		void WindowID() {}

	public:
		CIUIMDIChildWnd *GetActiveChildFrame();
		int SetCloseButtonInTab(BOOL bCloseBtnInTab);
		BOOL IsCloseButtonInTab();
		int UpdateMDIClientTitleBar();

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CIUIMDIClientWnd)
	protected:
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		//}}AFX_VIRTUAL


		// Generated message map functions
	protected:
		//{{AFX_MSG(CIUIMDIClientWnd)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnMDIActivate(BOOL bActivate, CWnd *pActivateWnd, CWnd *pDeactivateWnd);
		afx_msg LRESULT OnMDICreate(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnMDIDestroy(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnMDIGetActive(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnMDISetMenu(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnParentEvent(WPARAM wParam, LPARAM lParam);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		CTaskWndMgr m_TaskChildFrameMgr;
		CStackPanel m_stackPanel;
		UINT m_nMDITabID;
		UINT m_nNextChildFrameID;
		UINT m_nSwitchDocumentButtonID;
		UINT m_nCloseDocumentButtonID;
		BOOL m_bCloseBtnInTab;
	};

	//////////////////////////////////////////////////////////////////////////
	// CIUIMDIFrameWnd

	class CIUIMDIFrameWnd : public CIUIFrameWnd
	{
		friend CIUIDocument;
		friend CIUIMDIClientWnd;
		friend CIUIFrameWnd;
		friend CIUIMDIChildWnd;
	public:
		DECLARE_DYNCREATE(CIUIMDIFrameWnd)

		CIUIMDIFrameWnd();
		virtual ~CIUIMDIFrameWnd();

	public:
		void MDIActivate(CWnd *pWndActivate);	///
		CIUIMDIChildWnd *MDIGetActive(BOOL *pbMaximized = NULL) const;
		void UpdateFrameTitleForDocument(LPCTSTR lpszDocName);
		void MDINext();	///

		// Overrides
		// for menu bar
		virtual CMenuBar *GetDefaultMenuBar()
		{
			return m_pMenuBarDefault;
		}
		int SetDefaultMenuBar(CMenuBar *pMenuBar);

		// for create CIUIMDIClientWnd object
		virtual BOOL GetMDIClientWindowID(UINT *puID)
		{
			return FALSE;
		}
		virtual BOOL OnGetMDIClientRect(LPRECT lpRect)
		{
			return FALSE;
		}

		// for create tab in CIUIMDIClientWnd object
		virtual BOOL GetMDITabImages(CString *pstrImageNameN, CString *pstrImageNameH,
			CString *pstrImageNameS, CString *pstrImageNameD,
			CString *pstrImageNameCN, CString *pstrImageNameCH,
			CString *pstrImageNameCS, CString *pstrImageNameCD)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		virtual BOOL GetMDITabsRect(LPRECT lpRect)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		virtual BOOL GetSwitchDocumentButtonID(UINT *puID)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		virtual BOOL GetCloseDocumentButtonID(UINT *puID)
		{
			ASSERT(FALSE);
			return FALSE;
		}

		// for position of CIUIView
		virtual BOOL GetMDIViewRect(LPRECT lpRect)
		{
			return FALSE;
		}

		virtual void OnUpdateFrameTitle(BOOL bAddToTitle); ///

		// Generated message map functions
	protected:
		//{{AFX_MSG(CIUIMDIFrameWnd)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnDestroy();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnFileNew();
		afx_msg void OnFileOpen();
		afx_msg void OnFileClose();
		afx_msg void OnFileSave();
		afx_msg void OnFileSaveAs();
		afx_msg void OnAppExit();
		afx_msg LRESULT OnGetDocumentViewClientRect(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnMDIRefreshMenu(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnGetMenuBar(WPARAM wParam, LPARAM lParam);
#if(_MSC_VER <= 1200) // VC6.0
		afx_msg void OnFileMruFile(UINT uID);
#else
		afx_msg BOOL OnFileMruFile(UINT uID);
#endif
		afx_msg void OnUpdateFileMruFile1(CCmdUI *pCmdUI);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	protected:
		CIUIMDIClientWnd m_wndMDIClient;
		CString m_strTitle;				// default title (original)
		CMenuBar *m_pMenuBarDefault;	// default menu bar
	};

	//////////////////////////////////////////////////////////////////////////
	// CIUIDocManager

	class CIUIDocManager : public CDocManager
	{
		DECLARE_DYNAMIC(CIUIDocManager)

	public:
		CIUIDocManager();
		virtual ~CIUIDocManager();

	public:
		virtual void RegisterShellFileTypes(BOOL bCompat);
	};

	//////////////////////////////////////////////////////////////////////////
	// CIUIMultiDocTemplate

	class CIUIMultiDocTemplate : public CMultiDocTemplate
	{
		friend CIUIMDIFrameWnd;
		friend CIUIMDIClientWnd;

	public:
		CIUIMultiDocTemplate(UINT nIDResource, CRuntimeClass *pDocClass, CRuntimeClass *pFrameClass, CRuntimeClass *pViewClass);
		virtual ~CIUIMultiDocTemplate();

		// the following private interface shouldn't be called.
	private:
		virtual void InitialUpdateFrame(CFrameWnd *pFrame, CDocument *pDoc, BOOL bMakeVisible = TRUE)
		{
			ASSERT(FALSE);
		}
		virtual CFrameWnd *CreateNewFrame(CDocument *pDoc, CFrameWnd *pOther)
		{
			ASSERT(FALSE);
			return NULL;
		}

	public:
		// override CDocTemplate::InitialUpdateFrame
		virtual void InitialUpdateFrame(CIUIMDIChildWnd *pFame, CIUIDocument *pDoc, BOOL bMakeVisible = TRUE);
		virtual CIUIMDIChildWnd *CreateNewIUIFrame(CIUIDocument *pDoc);

		virtual CDocument *OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);

	private:
		UINT m_uViewID;
	};


	// Create controls.
	void SetButtonImagesByParent(CRect rect, CWnd *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinButton *CreateButton(BTNPROPERTIES *pBtn, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	void SetToggleButtonImagesByParent(TOGGLEBTNPROPERTIES *pToggleProp, CRect rect, CSkinButton *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinButton *CreateCheckBox(CHKPROPERTIES *pChk, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinButton *CreateRadioButton(RADPROPERTIES *pRad, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	void SetEditImagesByParent(EDTPROPERTIES *pEdtProp, CRect rect, CWnd *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinEdit *CreateEdit(EDTPROPERTIES *pEdt, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	void SetComboBoxImagesByParent(CRect rect, CWnd *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinComboBox *CreateComboBox(CMBPROPERTIES *pCmb, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	void SetStaticImagesByParent(CRect rect, CWnd *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgD);
	CSkinStatic *CreateStatic(TXTPROPERTIES *pSta, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgD);
	void SetSliderImagesByParent(CRect rect, CWnd *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinSliderCtrl *CreateSlider(SLDPROPERTIES *pSld, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	void SetSpinButtonImagesByParent(CRect rect, CWnd *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinSpinButtonCtrl *CreateSpinButton(SPNPROPERTIES *pSpn, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	void SetProgressImagesByParent(PRGPROPERTIES *pPrg, CRect rect, CWnd *pChild,
		HIUIIMAGE bmpN, HIUIIMAGE bmpH, HIUIIMAGE bmpS, HIUIIMAGE bmpD);
	CSkinProgressCtrl *CreateProgress(PRGPROPERTIES *pPrg, CWnd *pParent,
		HIUIIMAGE bmpN, HIUIIMAGE bmpH, HIUIIMAGE bmpS, HIUIIMAGE bmpD);
	void SetHotKeyImagesByParent(CRect rect, CWnd *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinHotKeyCtrl *CreateHotKey(HOTPROPERTIES *pHot, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	void SetIPAddressImagesByParent(CRect rect, CWnd *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinIPAddressCtrl *CreateIPAddress(IPAPROPERTIES *pIPA, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	void SetSliderExImagesByParent(SLDEXPROPERTIES *pSldEx, CRect rect, CWnd *pChild,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSliderCtrlEx *CreateSliderEx(SLDEXPROPERTIES *pSldEx, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CSkinListCtrl *CreateListCtrl(LSTCTRLPROPERTIES *pList, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgD);
	CSkinTreeCtrl *CreateTreeCtrl(TREECTRLPROPERTIES *pTree, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CScrollBarEx *CreateScrollBarEx(SBEXPROPERTIES *pSB, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CThumbnailCtrl *CreateThumbnail(THMPROPERTIES *pThm, CWnd *pParent,
		HIUIIMAGE bmpN, HIUIIMAGE bmpD);
	CTreeListCtrl *CreateTreeList(TREELISTPROPERTIES *pTreeList, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgD);
	CSkinRichEditCtrl *CreateRichEdit(RICHEDITPROPERTIES *pRichEdt, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CIMRichEditCtrl *CreateIMRichEdit(IMREPROPERTIES *pIMRichEdt, CWnd *pParent,
		HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD);
	CHtmlCtrl *CreateHtmlCtrl(HTMLCTRLPROPERTIES *pHtmlCtrl, CWnd *pParent);
	CSkinDateTimeCtrl *CreateDateTimeCtrl(DATETIMECTRLPROPERTIES *pDateTimeCtrl, CWnd *pParent);
	CWaveCtrl *CreateWaveCtrl(WAVECTRLPROPERTIES *pWaveCtrl, CWnd *pParent,
		HIUIIMAGE hbmpN, HIUIIMAGE hbmpH, HIUIIMAGE hbmpS, HIUIIMAGE hbmpD);
	CPictureCtrl *CreatePictureCtrl(PICWLPROPERTIES *pPicCtrl, CWnd *pParent,
		HIUIIMAGE hbmpN, HIUIIMAGE hbmpH, HIUIIMAGE hbmpS, HIUIIMAGE hbmpD);
	CMenuBar *CreateMenuBar(MENUBARPROPERTIES *pMenuBar, CWnd *pParent);
	CSplitterBar *CreateSplitter(SPLITTERPROPERTIES *pSplitter, CWnd *pParent);
	CRectCtrl *CreateWLRect(RECTWLPROPERTIES *pRectCtrl, CWnd *pParent);
	CWLSplitterBar *CreateWLSplitter(SPLITTERWLPROPERTIES *pSplitter, CWnd *pParent);
	CWLLine *CreateWLLine(LINEWLPROPERTIES *pLine, CWnd *pParent);
	CWLText *CreateWLText(TXTPROPERTIES *pText, CWnd *pParent);
	CWLButton *CreateWLButton(BTNPROPERTIES *pBtn, CWnd *pParent);
	CWLButton *CreateWLCheck(CHKPROPERTIES *pChk, CWnd *pParent);
	CWLButton *CreateWLRadio(RADPROPERTIES *pRad, CWnd *pParent);
	CWLSliderCtrl *CreateWLSlider(SLDEXPROPERTIES *pWLSld, CWnd *pParent);
	CWLPicture *CreateWLPic(PICWLPROPERTIES *pPic, CWnd *pParent);
	CWLRichEditCtrl *CreateWLRichEdit(RICHEDITPROPERTIES *pRichEdt, CWnd *pParent);
	CWLIMRichEditCtrl *CreateWLIMRichEdit(IMREPROPERTIES *pIMRichEdt, CWnd *pParent);
	CTaskWndMgr *CreateTaskWndMgr(TWMGRPROPERTIES *pTaskWndMgrProp, CWnd *pParent);
	CDockPanel *CreatePanelDock(DOCKPNLPROPERTIES *pDockPanelProp, CWnd *pParent);
	CUniformGridPanel *CreatePanelUniformGrid(UGRIDPNLPROPERTIES *pUGridPanelProp, CWnd *pParent);
	CStackPanel *CreatePanelStack(STACKPNLPROPERTIES *pStackPanelProp, CWnd *pParent);

	CString GetRichEditClassName();

}// name space LibUIDK


#endif //_CONTROLS_H_
