// OleDropTargetEx.cpp: implementation of the CIUIOleDropTarget class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


#if (_MSC_VER >= 1500)	// vs2008

#pragma comment (lib, "UxTheme.lib")

//
// A class to store user defined text descriptions
//

CDropDescription::CDropDescription()
{
	m_lpszInsert = NULL;
	::ZeroMemory(m_pDescriptions, sizeof(m_pDescriptions));
	::ZeroMemory(m_pDescriptions1, sizeof(m_pDescriptions1));
}

CDropDescription::~CDropDescription()
{
	delete [] m_lpszInsert;
	for (unsigned n = 0; n <= nMaxDropImage; n++)
	{
		delete [] m_pDescriptions[n];
		delete [] m_pDescriptions1[n];
	}
}

// Set text for specified image type.
// lpszText:  String to be used without szInsert.
// lpszText1: String to be used with szInsert.
//            This may be NULL. Then lpszText is returned when requesting the string with szInsert.
// NOTES:
//  - Because the %1 placeholder is used to insert the szInsert text, a single percent character
//    inside the strings must be esacped by another one.
//  - When passing NULL for lpszText (or just not calling this for a specific image type),
//    the default system text is shown.
//    To avoid this, pass an empty string. However, this will result in an empty text area
//     shown below and right of the new style cursor.
bool CDropDescription::SetText(DROPIMAGETYPE nType, LPCWSTR lpszText, LPCWSTR lpszText1)
{
	ASSERT(IsValidType(nType));

	bool bRet = false;
	if (IsValidType(nType))
	{
		delete [] m_pDescriptions[nType];
		delete [] m_pDescriptions1[nType];
		m_pDescriptions[nType] = m_pDescriptions1[nType] = NULL;
		if (lpszText)
		{
			m_pDescriptions[nType] = new WCHAR[wcslen(lpszText) + 1];
			wcscpy_s(m_pDescriptions[nType], nMaxDropImage + 1, lpszText);
			bRet = true;
		}
		if (lpszText1)
		{
			m_pDescriptions1[nType] = new WCHAR[wcslen(lpszText1) + 1];
			wcscpy_s(m_pDescriptions1[nType], nMaxDropImage + 1, lpszText1);
			bRet = true;
		}
	}
	return bRet;
}

bool CDropDescription::SetInsert(LPCWSTR lpszInsert)
{
	delete [] m_lpszInsert;
	m_lpszInsert = NULL;
	if (lpszInsert)
	{
		m_lpszInsert = new WCHAR[wcslen(lpszInsert) + 1];
		wcscpy_s(m_lpszInsert, wcslen(lpszInsert) + 1, lpszInsert);
	}
	return NULL != m_lpszInsert;
}

// Get text string for specified image type.
// When b1 is true, return the string that includes the szInsert string when present.
LPCWSTR CDropDescription::GetText(DROPIMAGETYPE nType, bool b1) const
{
	return IsValidType(nType) ?
		((b1 && m_pDescriptions1[nType]) ? m_pDescriptions1[nType] : m_pDescriptions[nType]) : NULL;
}

// Set drop description data insert and message text.
// Returns true when szInsert or szMessage has been changed.
bool CDropDescription::SetDescription(DROPDESCRIPTION *pDropDescription, LPCWSTR lpszMsg) const
{
	return CopyInsert(pDropDescription, m_lpszInsert) | CopyMessage(pDropDescription, lpszMsg);
}

// Set drop description data insert and message text according to image type.
// Returns true when szInsert or szMessage has been changed.
bool CDropDescription::SetDescription(DROPDESCRIPTION *pDropDescription, DROPIMAGETYPE nType) const
{
	return SetDescription(pDropDescription, GetText(nType, HasInsert()));
}

// Set drop description data message text according to image type.
// When szInsert of the description is not empty, copy the string that includes szInsert.
// Returns true when szMessage has been changed.
bool CDropDescription::CopyText(DROPDESCRIPTION *pDropDescription, DROPIMAGETYPE nType) const
{
	return CopyMessage(pDropDescription, GetText(nType, HasInsert(pDropDescription)));
}

// Copy string to drop description szMessage member.
// Returns true if strings has been changed.
bool CDropDescription::CopyMessage(DROPDESCRIPTION *pDropDescription, LPCWSTR lpszMsg) const
{
	ASSERT(pDropDescription);
	return CopyDescription(pDropDescription->szMessage, sizeof(pDropDescription->szMessage) / sizeof(pDropDescription->szMessage[0]), lpszMsg);
}

// Copy string to drop description szInsert member.
// Returns true if strings has been changed.
bool CDropDescription::CopyInsert(DROPDESCRIPTION *pDropDescription, LPCWSTR lpszInsert) const
{
	ASSERT(pDropDescription);
	return CopyDescription(pDropDescription->szInsert, sizeof(pDropDescription->szInsert) / sizeof(pDropDescription->szInsert[0]), lpszInsert);
}

// Helper function for CopyMessage() and CopyInsert().
bool CDropDescription::CopyDescription(LPWSTR lpszDest, size_t nDstSize, LPCWSTR lpszSrc) const
{
	ASSERT(lpszDest);
	ASSERT(nDstSize > 0);

	bool bChanged = false;
	if (lpszSrc && *lpszSrc)
	{
		if (wcscmp(lpszDest, lpszSrc))
		{
			bChanged = true;
			wcsncpy_s(lpszDest, nDstSize, lpszSrc, _TRUNCATE);
		}
	}
	else if (lpszDest[0])
	{
		bChanged = true;
		lpszDest[0] = L'\0';
	}
	return bChanged;
}


//////////////////////////////////////////////////////////////////////////
// CIUIOleDropSource

CIUIOleDropSource::CIUIOleDropSource()
{
	m_bSetCursor = TRUE;			// Must set default cursor
	m_pIDataObj = NULL;				// IDataObject of CIUIOleDataSource
}

// Called by COleDataSource::DoDragDrop().
// Override used here to set result flags.
BOOL CIUIOleDropSource::OnBeginDrag(CWnd *pWnd)
{
	// Base class version returns m_bDragStarted.
	BOOL bRet = COleDropSource::OnBeginDrag(pWnd);
	// Set result flags.
	// DRAG_RES_STARTED: Drag has been started by leaving the start rect or after delay time has expired.
	if (m_bDragStarted)
	{
		m_nResult = DRAG_RES_STARTED;
	}
	// DRAG_RES_RELEASED: Mouse button has been released before leaving start rect or delay time has expired.
	else if (GetKeyState((m_dwButtonDrop & MK_LBUTTON) ? VK_LBUTTON : VK_RBUTTON) >= 0)
	{
		m_nResult = DRAG_RES_RELEASED;
	}
	// DRAG_RES_CANCELLED: ESC pressed or other mouse button activated.
	else
	{
		m_nResult = DRAG_RES_CANCELLED;
	}
	return bRet;
}

// Override used here to set result flags.
SCODE CIUIOleDropSource::QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState)
{
	SCODE scRet = COleDropSource::QueryContinueDrag(bEscapePressed, dwKeyState);
	if (0 == (dwKeyState & m_dwButtonDrop))			// mouse button has been released
	{
		m_nResult |= DRAG_RES_RELEASED;
	}
	if (DRAGDROP_S_CANCEL == scRet)					// ESC pressed, other mouse button activated, or
	{
		m_nResult |= DRAG_RES_CANCELLED;    //  mouse button released when not started
	}
	return scRet;
}

// Updates the drag image cursor
SCODE CIUIOleDropSource::GiveFeedback(DROPEFFECT dropEffect)
{
	SCODE sc = COleDropSource::GiveFeedback(dropEffect);

	if (m_bDragStarted && m_pIDataObj)
	{
		// 		if (0 != CDragDropHelper::GetGlobalDataDWord(m_pIDataObj, _T("IsShowingLayered")))
		// 		{
		// 			if (m_bSetCursor)
		// 			{
		// 				// NOTE:
		// 				//  Add '#define OEMRESOURCE' on top of stdafx.h.
		// 				//  This is necessary to include the OCR_ definitions from winuser.h.
		// 				HCURSOR hCursor = (HCURSOR)::LoadImage(NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		// 				::SetCursor(hCursor);
		// 				m_bSetCursor = FALSE;
		// 			}
		//
		// 			SetDragImageCursor(dropEffect);         // Select and show new style drag cursor
		// 			sc = S_OK;                              // Don't show default (old style) cursor
		// 		}
		// 		else
		// 		{
		// 			m_bSetCursor = TRUE;
		// 		}
	}

	return sc;
}

//////////////////////////////////////////////////////////////////////////
// CIUIOleDataSource

CIUIOleDataSource::CIUIOleDataSource()
{
	m_bAllowShowDropDescription = FALSE;
	m_bHasSetDropDescription = FALSE;
	m_pDragSourceHelper = NULL;
	m_pDragSourceHelper2 = NULL;

	// Create drag source helper to show drag images.
#if defined(IID_PPV_ARGS) // The IID_PPV_ARGS macro has been introduced with Visual Studio 2005
	::CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pDragSourceHelper));
#else
	::CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER, IID_IDragSourceHelper, (void **)&m_pDragSourceHelper)));
#endif
	if (m_pDragSourceHelper)
	{
		// With Vista, IDragSourceHelper2 has been inherited from by IDragSourceHelper.
#if defined(IID_PPV_ARGS) && defined(__IDragSourceHelper2_INTERFACE_DEFINED__)
		// Can't use the IID_PPV_ARGS macro when building for pre Vista because the GUID for IDragSourceHelper2 is not defined.
		m_pDragSourceHelper->QueryInterface(IID_PPV_ARGS(&m_pDragSourceHelper2));
#else
		m_pDragSourceHelper->QueryInterface(IID_IDragSourceHelper2, (void **)&m_pDragSourceHelper2);
#endif
		if (NULL != m_pDragSourceHelper2)
		{
			// No need to have two instances of the source helper in memory.
			m_pDragSourceHelper->Release();
			m_pDragSourceHelper = (IDragSourceHelper *)m_pDragSourceHelper2;
		}
	}
}

CIUIOleDataSource::~CIUIOleDataSource()
{
	if (m_pDragSourceHelper != NULL)
	{
		m_pDragSourceHelper->Release();
		m_pDragSourceHelper = NULL;
	}
}

int CIUIOleDataSource::AllowShowDropDescription()
{
	if (m_pDragSourceHelper2 == NULL)
	{
		return -1;
	}

	HRESULT hr = m_pDragSourceHelper2->SetFlags(DSH_ALLOWDROPDESCRIPTIONTEXT);
	if (FAILED(hr))
	{
		return -2;
	}

	m_bAllowShowDropDescription = TRUE;

	return 0;
}

BOOL CIUIOleDataSource::IsAllowShowDropDescription()
{
	return m_bAllowShowDropDescription;
}

int CIUIOleDataSource::SetDropDescription(DROPDESCRIPTION *pdd)
{
	if (pdd == NULL)
	{
		return -1;
	}

	//	m_DropDescription = *pdd;

	m_bHasSetDropDescription = TRUE;

	return 0;
}

int CIUIOleDataSource::SetDropDescription(DROPIMAGETYPE type, WCHAR *pwszMessage, WCHAR *pwszInsert)
{
	DROPDESCRIPTION dd;
	dd.type = type;
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	wcsncpy(dd.szMessage, pwszMessage, MAX_PATH - 1);
	wcsncpy(dd.szMessage, pwszMessage, MAX_PATH - 1);
#else
	wcscpy_s(dd.szMessage, MAX_PATH, pwszMessage);
	wcscpy_s(dd.szInsert, MAX_PATH, pwszInsert);
#endif

	return SetDropDescription(&dd);
}

HRESULT CIUIOleDataSource::SetDragImageFromBitmap(HBITMAP hBitmap, const CPoint &point/* = CPoint(0, 0)*/, COLORREF crColorKey/* = GetSysColor(COLOR_WINDOW)*/)
{
	if (m_pDragSourceHelper2 == NULL)
	{
		return E_FAIL;
	}

	// Show the drag image
	SHDRAGIMAGE di;
	BITMAP bm;
	GetObject(hBitmap, sizeof(bm), &bm);
	di.sizeDragImage.cx = bm.bmWidth;
	di.sizeDragImage.cy = bm.bmHeight;
	di.hbmpDragImage = hBitmap;
	di.crColorKey = crColorKey;
	di.ptOffset.x = point.x;
	di.ptOffset.y = point.y;

	LPDATAOBJECT lpDataObject = (LPDATAOBJECT)GetInterface(&IID_IDataObject);

	return m_pDragSourceHelper->InitializeFromBitmap(&di, lpDataObject);
}

HRESULT CIUIOleDataSource::SetDragImageFromWindow(HWND hwnd, const CPoint &point/* = CPoint(0, 0)*/)
{
	if (m_pDragSourceHelper2 == NULL)
	{
		return E_FAIL;
	}

	// Show the drag image
	LPDATAOBJECT lpDataObject = (LPDATAOBJECT)GetInterface(&IID_IDataObject);
	POINT pt = point;

	return m_pDragSourceHelper->InitializeFromWindow(hwnd, &pt, lpDataObject);
}

// Perform drag & drop with support for DropDescription.
// To support drop descriptions, we must use our COleDropSource derived class
//  that handles drop descriptions within the GiveFeedback() function.
DROPEFFECT CIUIOleDataSource::DoDragDropEx(DROPEFFECT dwEffect, LPCRECT lpRectStartDrag /*= NULL*/)
{
	// When visual styles are disabled, new cursors(means new style drag-image) are not shown.
	// Then we must show the old style cursors.
	// NOTE: When pre XP Windows versions must be supported (WINVER < 0x0501),
	//  delay loading for uxtheme.dll must be specified in the project settings.
	BOOL bCanUseDescription = (m_pDragSourceHelper2 != NULL) && ::IsAppThemed();	// IsAppThemed Minimum supported client is Windows Vista [desktop apps only]


	//
	// 1. When drop descriptions can be used and description text strings has been defined,
	//  create a DropDescription data object with image type DROPIMAGE_INVALID.
	//
	if (bCanUseDescription && m_bHasSetDropDescription)
	{
		// Show the text of drag image.
		HRESULT hr = m_pDragSourceHelper2->SetFlags(DSH_ALLOWDROPDESCRIPTIONTEXT);

		CLIPFORMAT cfDropDescription = ::RegisterClipboardFormat(CFSTR_DROPDESCRIPTION);
		if (cfDropDescription != 0)
		{
			HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(DROPDESCRIPTION));
			if (hGlobal != NULL)
			{
				DROPDESCRIPTION *pDropDescription = (DROPDESCRIPTION *)::GlobalLock(hGlobal);
				pDropDescription->type = DROPIMAGE_INVALID;
				::GlobalUnlock(hGlobal);

				//CacheGlobalData(cfDropDescription, hGlobal);	// or call IDataObject::SetData

				FORMATETC formatEtc;
				formatEtc.cfFormat = cfDropDescription;
				formatEtc.tymed = TYMED_HGLOBAL;

				STGMEDIUM medium;
				medium.tymed = TYMED_HGLOBAL;
				medium.hGlobal = hGlobal;
				medium.pUnkForRelease = NULL;

				LPDATAOBJECT lpDataObject = (LPDATAOBJECT)(GetInterface(&IID_IDataObject));
				//		HRESULT hr = lpDataObject->SetData(&formatEtc, &medium, TRUE);
				if (FAILED(hr))
				{
					GlobalFree(hGlobal);
				}
			}
		}
	}

	//
	// 2. Use our COleDropSource derived class here to support drop descriptions.
	//
	CIUIOleDropSource dropSource;
	if (bCanUseDescription)
	{
		dropSource.m_pIDataObj = (LPDATAOBJECT)(GetInterface(&IID_IDataObject));
		if (m_bHasSetDropDescription)
		{
			dropSource.m_pDropDescription = &m_DropDescription;
		}
	}

	//
	// 3. Perform the drag operation.
	// This will block until the operation has finished.
	//
	dwEffect = COleDataSource::DoDragDrop(dwEffect, lpRectStartDrag, (COleDropSource *)&dropSource);
	// 	m_nDragResult = dropSource.m_nResult;			// Get the drag status
	// 	if (dwEffect & ~DROPEFFECT_SCROLL)
	// 		m_nDragResult |= DRAG_RES_DROPPED;			// Indicate that data has been successfully dropped
	return dwEffect;
}

BEGIN_INTERFACE_MAP(CIUIOleDataSource, COleDataSource)
INTERFACE_PART(CIUIOleDataSource, IID_IDataObject, DataObj)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) CIUIOleDataSource::XDataObj::AddRef()
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CIUIOleDataSource::XDataObj::Release()
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->ExternalRelease();
}

STDMETHODIMP CIUIOleDataSource::XDataObj::QueryInterface(REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

struct AFX_DATACACHE_ENTRY
{
	FORMATETC m_formatEtc;
	STGMEDIUM m_stgMedium;
	DATADIR m_nDataDir;
};

// helper methods to fix IDropSourceHelper

STDMETHODIMP CIUIOleDataSource::XDataObj::GetData(LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)

#ifdef _DEBUG
	TCHAR szFormatName[MAX_PATH] = {0};
	int nRet = GetClipboardFormatName(pFormatetc->cfFormat, szFormatName, MAX_PATH);
#endif

	return pThis->m_xDataObject.GetData(pFormatetc, pmedium);
}

STDMETHODIMP CIUIOleDataSource::XDataObj::GetDataHere(LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->m_xDataObject.GetDataHere(pFormatetc, pmedium);
}

STDMETHODIMP CIUIOleDataSource::XDataObj::QueryGetData(LPFORMATETC pFormatetc)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->m_xDataObject.QueryGetData(pFormatetc);
}

STDMETHODIMP CIUIOleDataSource::XDataObj::GetCanonicalFormatEtc(LPFORMATETC pFormatetcIn, LPFORMATETC pFormatetcOut)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->m_xDataObject.GetCanonicalFormatEtc(pFormatetcIn, pFormatetcOut);
}

STDMETHODIMP CIUIOleDataSource::XDataObj::SetData(LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium, BOOL fRelease)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)

#ifdef _DEBUG
	TCHAR szFormatName[MAX_PATH] = {0};
	int nRet = GetClipboardFormatName(pFormatetc->cfFormat, szFormatName, MAX_PATH);
#endif

	// normal processing
	HRESULT hr = pThis->m_xDataObject.SetData(pFormatetc, pmedium, fRelease);

	// for save drag-image.
	if (hr == DATA_E_FORMATETC)
	{
		// cache the data explicitly
		pThis->CacheData(pFormatetc->cfFormat, pmedium, pFormatetc);
		return S_OK;
	}

	// normal error
	return hr;
}

STDMETHODIMP CIUIOleDataSource::XDataObj::EnumFormatEtc(DWORD dwDirection, LPENUMFORMATETC *ppenumFormatetc)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->m_xDataObject.EnumFormatEtc(dwDirection, ppenumFormatetc);
}

STDMETHODIMP CIUIOleDataSource::XDataObj::DAdvise(LPFORMATETC pFormatetc, DWORD advf, LPADVISESINK pAdvSink, LPDWORD pdwConnection)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->m_xDataObject.DAdvise(pFormatetc, advf, pAdvSink, pdwConnection);
}

STDMETHODIMP CIUIOleDataSource::XDataObj::DUnadvise(DWORD dwConnection)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->m_xDataObject.DUnadvise(dwConnection);
}

STDMETHODIMP CIUIOleDataSource::XDataObj::EnumDAdvise(LPENUMSTATDATA *ppenumAdvise)
{
	METHOD_PROLOGUE(CIUIOleDataSource, DataObj)
	return pThis->m_xDataObject.EnumDAdvise(ppenumAdvise);
}


#endif //(_MSC_VER >= 1500)	// vs2008


//////////////////////////////////////////////////////////////////////
// CIUIDropMsgParam Construction/Destruction

CIUIDropMsgParam::CIUIDropMsgParam()
{
	m_pDataObject = NULL;
	m_dwKeyState = 0;
	m_dropEffect = (DROPEFFECT) - 1;
	m_dropEffectList = DROPEFFECT_NONE;
}

CIUIDropMsgParam::CIUIDropMsgParam(COleDataObject *pDataObject, DWORD dwKeyState, DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point)
{
	m_pDataObject = pDataObject;
	m_dwKeyState = dwKeyState;
	m_dropEffect = dropEffect;
	m_dropEffectList = dropEffectList;
	m_point = point;
}

CIUIDropMsgParam::~CIUIDropMsgParam()
{
	m_pDataObject = NULL;
}

#if (_MSC_VER <= 1200)

namespace VC6GUID
{
	const GUID FAR CLSID_DragDropHelper = {0x4657278a, 0x411b, 0x11d2, {0x83, 0x9a, 0x0, 0xc0, 0x4f, 0xd9, 0x18, 0xd0}};
	const GUID IID_IDropTargetHelper = {0x4657278b, 0x411b, 0x11d2, {0x83, 0x9a, 0x0, 0xc0, 0x4f, 0xd9, 0x18, 0xd0}};
}

#endif

//////////////////////////////////////////////////////////////////////
//CIUIOleDropTarget Construction/Destruction
CIUIOleDropTarget::CIUIOleDropTarget()
{
#if (_MSC_VER <= 1200)
	if (FAILED(CoCreateInstance(VC6GUID::CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER, VC6GUID::IID_IDropTargetHelper, (void **)&m_pDropTargetHelper)))
#else
	if (FAILED(CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER, IID_IDropTargetHelper, (void **)&m_pDropTargetHelper)))
#endif
	{
		m_pDropTargetHelper = NULL;
	}
}

CIUIOleDropTarget::~CIUIOleDropTarget()
{
	if (m_pDropTargetHelper != NULL)
	{
		m_pDropTargetHelper->Release();
		m_pDropTargetHelper = NULL;
	}
}

//////////////////////////////////////////////////////////////////////

HWND CIUIOleDropTarget::GetSafeHwnd()
{
	return this == NULL ? NULL : m_hWnd;
}

// Use IDropTargetHelper to show the custom drag image.

//////////////////////////////////////////////////////////////////////
DROPEFFECT CIUIOleDropTarget::OnDragEnter(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	CIUIDropMsgParam DropInfo(pDataObject, dwKeyState, 0, 0, point);
	DROPEFFECT dwDropEffect = (DROPEFFECT)pWnd->SendMessage(DROPM_DRAGENTER, (WPARAM)&DropInfo, 0);

	m_pDropTargetHelper->DragEnter(pWnd->GetSafeHwnd(), pDataObject->GetIDataObject(TRUE), &point, dwDropEffect);

	return dwDropEffect;
}

DROPEFFECT CIUIOleDropTarget::OnDragOver(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	CIUIDropMsgParam DropInfo(pDataObject, dwKeyState, 0, 0, point);
	DROPEFFECT dwDropEffect = (DROPEFFECT)pWnd->SendMessage(DROPM_DRAGOVER, (WPARAM)&DropInfo, 0);

	m_pDropTargetHelper->DragOver(&point, dwDropEffect);

	return dwDropEffect;
}

BOOL CIUIOleDropTarget::OnDrop(CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_pDropTargetHelper->Drop(pDataObject->GetIDataObject(TRUE), &point, dropEffect);

	CIUIDropMsgParam DropInfo(pDataObject, 0, dropEffect, 0, point);

	return (BOOL)pWnd->SendMessage(DROPM_DROP, (WPARAM)&DropInfo, 0);
}

DROPEFFECT CIUIOleDropTarget::OnDropEx(CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_pDropTargetHelper->Drop(pDataObject->GetIDataObject(TRUE), &point, dropEffect);

	CIUIDropMsgParam DropInfo(pDataObject, 0, dropEffect, dropEffectList, point);

	return (DROPEFFECT)pWnd->SendMessage(DROPM_DROPEX, (WPARAM)&DropInfo, 0);
}

void CIUIOleDropTarget::OnDragLeave(CWnd *pWnd)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_pDropTargetHelper->DragLeave();

	pWnd->SendMessage(DROPM_DRAGLEAVE, 0, 0);

	return;
}

DROPEFFECT CIUIOleDropTarget::OnDragScroll(CWnd *pWnd, DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	return (DROPEFFECT)pWnd->SendMessage(DROPM_DRAGSCROLL, 0, 0);
}
