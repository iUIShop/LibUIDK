#include "StdAfx.h"
#include <WindowsX.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Copy from windows 2000 source code.
// should caller pass in message that indicates termination
// (WM_LBUTTONUP, WM_RBUTTONUP)?
//
// in:
//      hwnd    to do check on
//      x, y    in client coordinates
//
// returns:
//      TRUE    the user began to drag (moved mouse outside double click rect)
//      FALSE   mouse came up inside click rect
//
// BUGBUG, should support VK_ESCAPE to cancel

BOOL PASCAL LibUIDK::CheckForDragBegin(HWND hwnd, int x, int y)
{
	RECT rc;
	int dxClickRect = GetSystemMetrics(SM_CXDRAG);
	int dyClickRect = GetSystemMetrics(SM_CYDRAG);

	if (dxClickRect < 4)
	{
		dxClickRect = dyClickRect = 4;
	}

	// See if the user moves a certain number of pixels in any direction

	SetRect(&rc, x - dxClickRect, y - dyClickRect, x + dxClickRect, y + dyClickRect);
	MapWindowRect(hwnd, HWND_DESKTOP, &rc); // client -> screen

	//
	//  SUBTLE!  We use PeekMessage+WaitMessage instead of GetMessage,
	//  because WaitMessage will return when there is an incoming
	//  SendMessage, whereas GetMessage does not.  This is important,
	//  because the incoming message might've been WM_CAPTURECHANGED.
	//

	SetCapture(hwnd);
	do
	{
		MSG msg32;
		if (PeekMessage(&msg32, NULL, 0, 0, PM_REMOVE))
		{
			// See if the application wants to process the message...
			if (CallMsgFilter(&msg32, MSGF_COMMCTRL_BEGINDRAG) != 0)
			{
				continue;
			}

			switch (msg32.message)
			{
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
				ReleaseCapture();
				return FALSE;

			case WM_MOUSEMOVE:
				if (IsWindow(hwnd) && !PtInRect(&rc, msg32.pt))
				{
					ReleaseCapture();
					return TRUE;
				}
				break;

			default:
				TranslateMessage(&msg32);
				DispatchMessage(&msg32);
				break;
			}
		}
		else
		{
			WaitMessage();
		}

		// WM_CANCELMODE messages will unset the capture, in that
		// case I want to exit this loop
	}
	while (IsWindow(hwnd) && GetCapture() == hwnd);

	return FALSE;
}
