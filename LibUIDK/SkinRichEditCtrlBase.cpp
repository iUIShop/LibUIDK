// SkinRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#if(_MSC_VER <= 1200) // VC6.0
#include <ObjIdl.h>
#endif
#include <RichOle.h>
#include "TextHost.h"
#include <atlbase.h>
#include <atlwin.h>
#pragma comment (lib, "Imm32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const LONG g_cInitTextMax = (32 * 1024) - 1;

#define RE_CALL_FUN(fun) \
	if (m_bWLWnd) \
	{\
		((CWLRichEditCtrl *)m_pBindWLWnd)->fun;\
	}\
	else\
	{\
		((CSkinRichEditCtrl *)m_pBindWnd)->fun;\
	}\

#define RE_CALL_FUN_RETURN(nRet, fun) \
	if (m_bWLWnd) \
	{\
		(nRet) = ((CWLRichEditCtrl *)m_pBindWLWnd)->fun;\
	}\
	else\
	{\
		(nRet) = ((CSkinRichEditCtrl *)m_pBindWnd)->fun;\
	}\

//////////////////////////////////////////////////////////////////////////


CRichEditCtrlBase::CRichEditCtrlBase()
{
	m_hPaintDC = NULL;

	m_pTextServices = NULL;
	m_pTextDocument = NULL;
	m_pTextRange = NULL;

	for (int i = 0; i < 4; ++i)
	{
		m_himgBk[i] = NULL;
	}

	// RichEdit�Ƿ���Ҫˢ�£�һ����WM_SIZE�У���ΪTRUE����OnDraw�У��������ΪTRUE��
	// �͸����µĿؼ��ߴ磬�������ݵ���ʾ����ΪFALSE
	m_bNeedUpdate = FALSE;
	m_bAlreadyStartGifTimer = FALSE;
}

CRichEditCtrlBase::~CRichEditCtrlBase()
{
	for (int i = 0; i < 4; ++i)
	{
		ReleaseIUIImage(m_himgBk[i]);
		m_himgBk[i] = NULL;
	}
}

int CRichEditCtrlBase::ReleaseObject()
{
	return 0;
}

// ����RichEdit�ı���ָ����ȷ�Χ����ʾʱ����Ҫ��ߡ�
int GetNeedHeight(CRichEditCtrlBase *pRe, int nCpFirst, int nCpLast, LONG lWidth)
{
	CRect rcPage(0, 0, GetXTwipsFromPixel(lWidth), GetYTwipsFromPixel(9999));

	HDC hDC = GetDC(GetDesktopWindow());
	LRESULT lr = 0;
	FORMATRANGE sf = {0};
	sf.hdc = hDC;
	sf.hdcTarget = hDC;
	sf.rc = rcPage; // FORMATRANGE::rc�ĵ�λΪ�(twips)��
	sf.rcPage = rcPage;
	sf.chrg.cpMin = nCpFirst; // ���FORMATRANGE::chrg.cpMinΪ0����FORMATRANGE::chrg.cpMaxΪ-1�������Richedit�����ַ�
	sf.chrg.cpMax = nCpLast;

	// �ڶ������FormatRange��Ϊ���ͷ��ڴ棬������ο�MSDN:EM_FORMATRANGE
	if (pRe->IsWindowless())
	{
		((CWLRichEditCtrl *)pRe->GetBindWindowless())->FormatRange(&sf, FALSE);
		((CWLRichEditCtrl *)pRe->GetBindWindowless())->FormatRange(NULL, FALSE);
	}
	else
	{
		((CSkinRichEditCtrl *)pRe->GetBindWindow())->FormatRange(&sf, FALSE);
		((CSkinRichEditCtrl *)pRe->GetBindWindow())->FormatRange(NULL, FALSE);
	}

	::ReleaseDC(GetDesktopWindow(), hDC);

	// �²��������ĸ߶ȣ���λΪ羣���ת������ʹ�á�
	int nNeedHeight = GetYPixelFromTwips(sf.rc.bottom);

	return nNeedHeight;
}

// �õ�ָ���е����꣬����������������ΪNULL
// ��RichEdit��ָ��cp�����꣬�м��ַ�����
// 1. ʹ��PosFromChar��GetCharPos(�����ڲ����ǵ���EM_POSFROMCHAR)�����Եõ�ָ��cp���Ͻ����ꡣ
// ͨ��ָ��cp����һ��cp����һ��cp, ���Եõ�ָ��cp����������(ע�⣬������RichEdit��һ���ַ�ʱ��
// ��Ȼ����ͨ��PosFromChar(1)���õ�����ַ����Ͻ����꣬��Ȼ��û��cpΪ1���ַ�, ������Ϊÿ������
// ĩβ������һ������������� �����ǡ����һ��������ʾ�������У����һ�е���β��û�ж��������,
// ����ͨ����һ�����һ���ַ���cp + 1���޷��õ����һ���ַ����Ͻ����꣬���ǵõ���һ�е�һ���ַ�����
// �����ꡣ ����������ĳ��������ʾ�ɶ���ʱ�����ǲ���Ҫ����������������ݿ�ȣ���Ϊ����RichEdit
// �Ŀ�ȣ��������ݵĿ�ȣ�����Ҫ���м���)
// 2. ͨ��ʹ��FormatRange������ģ��������Ӷ��õ�����Ҫ�ĸ߶����꣨����Լ�ָ����
// 3. ͨ��ITextRange::GetPoint�����Եõ�ѡ�з�Χ�������ꡣ ���뷽��һ��΢��RichEdit�ڲ�������
// CDisplayML::PointFromTpʵ�֡� ����ʹ������ӿ��и�ע�������VS2008 sp1����MSDN�����У�
// ITextRange::GetPoint��һ������ֻ��tomStart��tomEnd��TA_TOP��TA_BASELINE��TA_BOTTOM��TA_LEFT��
// TA_CENTER��TA_RIGHT����ϡ� ������ֻ�ܵõ����ӷ�Χ��cp�����ꡣ ��cp�������ɼ���Χ��󣬵õ�����0��
// ������ ���ߵ�MSDN�ϣ��ṩ��һЩ����Ĳ��������Եõ����ӷ�Χ������꣬���в������£�
// None					0 		��ѡ�
// IncludeInset			1 		�����Ͷ���Ƕ����ӵ����ε����Ͷ������꣬����������Ҳ�͵ײ���ȥ�Ҳ�͵ײ�Ƕ�롣
// Start				32		�ı���Χ�Ŀ�ʼλ�á�
// ClientCoordinates	256		���ع����������������Ļ���ꡣ
// AllowOffClient		512 	��������֮��ĵ㡣
// Transform			1024 	ʹ������Ӧ�ó����ṩ������ת���ı任���ꡣ
// NoHorizontalScroll	65536 	ˮƽ�����ѽ��á�
// NoVerticalScroll		262144 	��ֱ�����ѽ��á�
// ����ITextRange::GetPoint�ĵ�һ��������ֻҪ����512���Ϳ��Եõ���������������ˡ� ��������Щ�µ���
// �ԣ���֧�־ɰ汾��RichEdit.
int CRichEditCtrlBase::GetLineRect(int nLineIndex, __out POINT *pptStart, __out int *pnLineWidth, __out int *pnLineHeight)
{
	if (pptStart == NULL && pnLineWidth == NULL && pnLineHeight == NULL)
	{
		return -1;
	}

	// �õ�������ʼcp
	int nCpFirst = -1;
	RE_CALL_FUN_RETURN(nCpFirst, LineIndex(nLineIndex));

	// �õ���ʼcp�����Ͻ�����
	CPoint ptFirst;
	RE_CALL_FUN_RETURN(ptFirst, GetCharPos(nCpFirst));

	if (pptStart != NULL)
	{
		*pptStart = ptFirst;
	}

	if (pnLineWidth != NULL)
	{
		// nCpFirst�����е��ַ�����
		int nCharCount = -1;
		RE_CALL_FUN_RETURN(nCharCount, LineLength(nCpFirst));

		CPoint ptLast;
		RE_CALL_FUN_RETURN(ptLast, GetCharPos(nCpFirst + nCharCount));

#ifdef _DEBUG
#endif // _DEBUG

		*pnLineWidth = ptLast.x - ptFirst.x;
	}

	if (pnLineHeight != NULL)
	{
		int nNextLineCpFirst = -1;
		RE_CALL_FUN_RETURN(nNextLineCpFirst, LineIndex(nLineIndex + 1));

		// �������һ�У��õ���һ����ʼcp������󣬾Ϳ��Եõ��и�
		if (nNextLineCpFirst != -1)
		{
			CPoint ptNextLineFirst;
			RE_CALL_FUN_RETURN(ptNextLineFirst, GetCharPos(nNextLineCpFirst));

			*pnLineHeight = ptNextLineFirst.y - ptFirst.y;
		}

		// ���û����һ�У�ͨ��FormatRange�������и�
		else
		{
			// nCpFirst�����е��ַ�����
			int nCharCount = -1;
			RE_CALL_FUN_RETURN(nCharCount, LineLength(nCpFirst));

			CRect rcClient;
			RE_CALL_FUN(GetClientRect(rcClient));
			int nLineHeight = GetNeedHeight(this, nCpFirst, nCpFirst + nCharCount, rcClient.Width());

			*pnLineHeight = nLineHeight;

#if 0
			CComPtr<ITextDocument> pTextDocument = GetITextDocument();

			ITextRange *pTextRange = NULL;
			pTextDocument->Range(nCpFirst, nCpFirst + nCharCount, &pTextRange);

#ifdef _DEBUG
			BSTR bstr;
			pTextRange->GetText(&bstr);
#endif // _DEBUG


			CPoint pt;
			pTextRange->GetPoint(tomStart | TA_BOTTOM | TA_RIGHT | 256 | 512, &pt.x, &pt.y);
			if (pt.x == 0 || pt.y == 0)
			{
				int n = 0;
			}

			pTextRange->GetPoint(tomEnd | TA_BOTTOM | TA_RIGHT | 256 | 512, &pt.x, &pt.y);
			*pnLineHeight = pt.y - ptFirst.y;
#endif
		}
	}

	return 0;
}

int CRichEditCtrlBase::SetImages(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	m_bBkCombine = false;
	IUISetControlImage4(uMask, m_himgBk,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	return 0;
}

int CRichEditCtrlBase::GetImages(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage4(uMask,
		m_himgCombineBk, m_himgBk, *pbCombineImage,
		pstrCombineImageName,
		pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);

	return 0;
}

ITextServices *CRichEditCtrlBase::GetTextServices()
{
	ASSERT(IsControlCreated());

	// Windowless��RichEdit��ITextServices����CWLRichEditCtrl���ã��о����RichEdit��ITextServices
	// ��IRichEditOle�ӿ��в�ѯ��
	if (m_pTextServices == NULL)
	{
		if (!m_bWLWnd)
		{
			IRichEditOle *pRichEditOle = ((CSkinRichEditCtrl *)m_pBindWnd)->GetIRichEditOle();

			HRESULT hr = pRichEditOle->QueryInterface(IID_ITextServices, (void **)&m_pTextServices);
			m_pTextServices->AddRef();
		}
	}

	return m_pTextServices;
}

ITextDocument *CRichEditCtrlBase::GetITextDocument()
{
	ASSERT(IsControlCreated());

	if (m_pTextDocument == NULL)
	{
		GUID guid = __uuidof(ITextDocument);
		CComPtr<IRichEditOle> pRichEditOle = NULL;
		if (m_bWLWnd)
		{
			pRichEditOle = ((CWLRichEditCtrl *)m_pBindWLWnd)->GetIRichEditOle();
		}
		else
		{
			pRichEditOle = ((CSkinRichEditCtrl *)m_pBindWnd)->GetIRichEditOle();
		}

		HRESULT hr = pRichEditOle->QueryInterface(guid, (void **)&m_pTextDocument);
		m_pTextDocument->AddRef();
	}

	return m_pTextDocument;
}

// CSkinRichEditCtrl::GetDC��ITextHost::TxGetDC���ã���ITextHost::TxGetDC��һ�������ر�
// Ƶ���ķ��������ԣ�CSkinRichEditCtrl::GetDC���ص�HDC��Ҫ����������ֻ����һ�Σ���
// CSkinRichEditCtrl����ʱ�ͷ����HDC��������������ص���Դй©��
HDC CRichEditCtrlBase::GetDC()
{
	// Need release
	if (m_hPaintDC == NULL)
	{
		if (m_bWLWnd)
		{
			m_hPaintDC = ((CWLRichEditCtrl *)m_pBindWLWnd)->GetParent()->GetDC()->GetSafeHdc();
		}
		else
		{
			m_hPaintDC = ((CSkinRichEditCtrl *)m_pBindWnd)->GetParent()->GetDC()->GetSafeHdc();
		}
	}

	return m_hPaintDC;
}

int CRichEditCtrlBase::OnDrawRichText(CDC *pMemDCParent)
{
	ITextServices *pTextServices = GetTextServices();

	if (pTextServices != NULL)
	{
		CRect rc;
		if (m_bWLWnd)
		{
			((CWLRichEditCtrl *)m_pBindWLWnd)->m_pTwh->TxGetClientRect(rc);
		}
		else
		{
			((CSkinRichEditCtrl *)m_pBindWnd)->GetClientRect(rc);
			((CSkinRichEditCtrl *)m_pBindWnd)->ClientToScreen(rc);
			((CSkinRichEditCtrl *)m_pBindWnd)->GetParent()->ScreenToClient(rc);
		}

		// ע�������rc����Ҫ��rcһ��Ҫ��RichEditԭ����ʾ�����ȫһ������RichEdit�Ŀͻ������ر�ע�⣬
		// һ����Ҫ�����ǿͻ�������Ҳ����˵���Լ�����TxDraw��ʾ�����ݣ�Ҫ��WM_PAINT��ȫ���ϣ��������������
		// ����˸���⡣����������ϣ����˺ܾý����������Ϊ��TxDraw���Ƶ�ʱ�򣬻���㴹ֱ������Ϣ��WM_PAINT
		// ����ʱ��Ҳ����㴹ֱ������Ϣ�������TxDrawʹ�õ�rc�Ŀ����WM_PAINTʹ�õ�rc�Ŀ�Ȳ�һ����������
		// ���Ĵ�ֱ������Χ�Ͳ�һ������������RichEdit�ߴ�仯ʱ�������������ù���������������ˢ�£���������˸�ˡ�
		//rc.OffsetRect(-rc.left, 0);
		pTextServices->TxDraw(
			DVASPECT_CONTENT,          // Draw Aspect
			/*-1*/0,                // Lindex
			NULL,                    // Info for drawing optimazation
			NULL,                    // target device information
			pMemDCParent->GetSafeHdc(),                    // Draw device HDC
			NULL,                        // Target device HDC
			(RECTL *)&rc,            // Bounding client rectangle
			NULL,                     // Clipping rectangle for metafiles
			(RECT *)&rc,        // Update rectangle
			NULL,                        // Call back function
			NULL,                    // Call back parameter
			0);                        // What view of the object
	}

	return 0;
}

// ������һ��Gif
#define ID_TIMER_GIF	10

VOID CALLBACK RichEditGifTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (idEvent == ID_TIMER_GIF)
	{
		// ����Image ole�б�
		std::map<CImageOleCtrl *, int> *pmapGifOles =
			(std::map<CImageOleCtrl *, int> *)::SendMessage(hwnd, EM_GET_GIF_OLE_LIST, 0, 0);

		if (pmapGifOles != NULL && !pmapGifOles->empty())
		{
			std::map<CImageOleCtrl *, int>::iterator it = pmapGifOles->begin();
			for (; it != pmapGifOles->end(); ++it)
			{
				// ����õ����������������Ļ��
				CRect rcOle;
				it->first->IOleCtrlBase::GetWindowRect(rcOle);

				CRect rcRichEdit;
				GetWindowRect(hwnd, rcRichEdit);

				// ��ΪRichEdit���ݣ������ڸ������ϻ��ģ�����ˢ�¸�����
				if (rcRichEdit.IntersectRect(rcRichEdit, rcOle))
				{
					// ��ΪOLE�ǻ���RichEdit�������ϵģ����԰�OLE���꣬ת�������RichEdit����������
					::ScreenToClient(GetParent(hwnd), (LPPOINT)&rcOle);
					::ScreenToClient(GetParent(hwnd), ((LPPOINT)&rcOle) + 1);

					InvalidateRect(GetParent(hwnd), rcOle, FALSE);
				}
			}
		}
	}
}

int CRichEditCtrlBase::OnAddGifOle(CImageOleCtrl *pImageOleCtrl)
{
	m_mapGifOles[pImageOleCtrl] = 1;

	// ������ʱ��
	if (!m_bAlreadyStartGifTimer)
	{
		CWnd *pWndTimer = NULL;
		if (m_bWLWnd)
		{
			pWndTimer = m_pBindWLWnd->GetParent();
		}
		else
		{
			pWndTimer = m_pBindWnd;
		}

		ASSERT(::IsWindow(pWndTimer->GetSafeHwnd()));
		pWndTimer->SetTimer(ID_TIMER_GIF, 100, RichEditGifTimerProc);
	}

	return 0;
}

int CRichEditCtrlBase::SetInset(LPCRECT lpRect)
{
	if (lpRect == NULL)
	{
		return -1;
	}

	m_rcInset = *lpRect;

	CRect rcClient;
	RE_CALL_FUN(GetClientRect(rcClient));
	rcClient.DeflateRect(lpRect);
	RE_CALL_FUN(SetRect(rcClient));

	return 0;
}

int CRichEditCtrlBase::GetInset(LPRECT lpRect) const
{
	if (lpRect == NULL)
	{
		return -1;
	}

	*lpRect = m_rcInset;

	return 0;
}

int CRichEditCtrlBase::BindStyle(LPCTSTR lpszStyleID)
{
	if (lpszStyleID == NULL)
	{
		return -2;
	}

	RICHEDITPROPERTIES *pCtrlProp = (RICHEDITPROPERTIES *)CUIMgr::GetStyleItem(STYLET_RICHEDIT, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CRichEditCtrlBase::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	if (!IsControlCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_RICHEDIT
		&& ect != CT_WL_RICHEDIT
		&& ect != CT_RICHEDIT)
	{
		return -3;
	}

	RICHEDITPROPERTIES *pRichEditProp = (RICHEDITPROPERTIES *)pCtrlProp;

	// font and color
	CString strResFontID;
	CTRLPROPERTIES::IUIGetControlFont1(pCtrlProp, &strResFontID);
	HFONT hFont = CreateIUIFont(strResFontID);
	if (hFont != NULL)
	{
		// Text color
		COLORREF crN = RGB(0, 0, 0);
		CTRLPROPERTIES::IUIGetControlColor4(pCtrlProp, &crN, NULL, NULL, NULL);

		LOGFONT lf = {0};
		GetObject(hFont, sizeof(LOGFONT), &lf);

		CHARFORMAT2 cf;
		memset(&cf, 0, sizeof(CHARFORMAT2));
		cf.cbSize = sizeof(CHARFORMAT2);
		cf.dwMask = CFM_FACE | CFM_COLOR;
		cf.crTextColor = crN;
		TSTRCPY(cf.szFaceName, LF_FACESIZE, _T("Microsoft YaHei UI"));

		RE_CALL_FUN(SetDefaultCharFormat(cf));
	}

	// Set bitmaps for background
	bool bSpecifyBackgroundImages = pCtrlProp->m_bSpecifyBackgroundImages;
	if (bSpecifyBackgroundImages)
	{
		// Set resizing point
		CString strImageName[1 + COMBINEIMAGESIZE4];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetBackground4(pCtrlProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			SetBkCombineImage(strImageName[0]);
		}
		else
		{
			SetImages(CONTROL_STATE_UNCHECKED_ALL,
				strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CRichEditCtrlBase::InsertBitmap(HBITMAP hBitmap)
{
	//
	// 1. Creates a byte array object, using global memory as the physical device
	//
	LPLOCKBYTES lpLockBytes = NULL;
	SCODE sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
	{
		AfxThrowOleException(sc);
	}
	ASSERT(lpLockBytes != NULL);

	//
	// 2. Creates and opens a new compound file storage object on top of a byte-array object.
	//
	IStorage *pStorage = NULL;
	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes, STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &pStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(pStorage != NULL);

	//
	// 3. Generate OLE object inserted to rich edit control. Creates a static object (containing only a representation, with no native data) from a data transfer object.
	//

	STGMEDIUM stgm;
	stgm.tymed = TYMED_GDI;    // Storage medium = HBITMAP handle
	stgm.hBitmap = hBitmap;
	stgm.pUnkForRelease = NULL; // Use ReleaseStgMedium

	COleDataSource ds;
	ds.CacheData(CF_BITMAP, &stgm);
	LPDATAOBJECT lpDataObject = (LPDATAOBJECT)ds.GetInterface(&IID_IDataObject);

	// Get interface of RichEdit's OLEClientSite
	LPOLECLIENTSITE lpClientSite = NULL;
	CComPtr<IRichEditOle> pRichEditOle = NULL;
	if (m_bWLWnd)
	{
		pRichEditOle = ((CWLRichEditCtrl *)m_pBindWLWnd)->GetIRichEditOle();
	}
	else
	{
		pRichEditOle = ((CSkinRichEditCtrl *)m_pBindWnd)->GetIRichEditOle();
	}

	pRichEditOle->GetClientSite(&lpClientSite);

	FORMATETC fm;
	fm.cfFormat = CF_BITMAP;			// Clipboard format = CF_BITMAP
	fm.ptd = NULL;						// Target Device = Screen
	fm.dwAspect = DVASPECT_CONTENT;		// Level of detail = Full content
	fm.lindex = -1;						// Index = Not applicaple
	fm.tymed = TYMED_GDI;

	IOleObject *pOleObject = NULL;
	sc = OleCreateStaticFromData(lpDataObject, IID_IOleObject, OLERENDER_FORMAT, &fm, lpClientSite, pStorage, (void **)&pOleObject);
	if (sc != S_OK)
	{
		AfxThrowOleException(sc);
	}

	//
	// 4. Insert OLE object
	//
	CLSID clsid;
	sc = pOleObject->GetUserClassID(&clsid);
	if (sc != S_OK)
	{
		AfxThrowOleException(sc);
	}

	REOBJECT reobject;
	ZeroMemory(&reobject, sizeof(REOBJECT));
	reobject.cbStruct = sizeof(REOBJECT);
	reobject.clsid = clsid;
	reobject.cp = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.poleobj = pOleObject;
	reobject.polesite = lpClientSite;
	reobject.pstg = pStorage;

	HRESULT hr = pRichEditOle->InsertObject(&reobject);
	if (FAILED(hr))
	{
		return -1;
	}

	return 0;
}

int CRichEditCtrlBase::InsertBitmap(LPCTSTR lpszBitmapPath)
{
	HBITMAP hBitmap = CIUIImage::LoadHBITMAP(lpszBitmapPath);

	return InsertBitmap(hBitmap);
}


int CRichEditCtrlBase::InsertGif(LPCTSTR lpszGifPath, LONG cp)
{
	return S_OK;
}

int CRichEditCtrlBase::InsertOle(IOleObject *pOleObject, LONG cp, int nWidth, int nHeight)
{
	try
	{
		//
		// 1. Get IRichEditOle and IOleClientSite interface
		//
		CComPtr<IRichEditOle> pRichEditOle = NULL;
		if (m_bWLWnd)
		{
			pRichEditOle = ((CWLRichEditCtrl *)m_pBindWLWnd)->GetIRichEditOle();
		}
		else
		{
			pRichEditOle = ((CSkinRichEditCtrl *)m_pBindWnd)->GetIRichEditOle();
		}

		CComPtr<IOleClientSite> pClientSite = NULL;
		HRESULT hr = pRichEditOle->GetClientSite(&pClientSite);
		if (FAILED(hr))
		{
			return -1;
		}

		//
		// 2. Create ILockBytes interface
		//
		CComPtr<ILockBytes> pLockBytes = NULL;
		hr = ::CreateILockBytesOnHGlobal(NULL, TRUE, &pLockBytes);
		if (FAILED(hr))
		{
			return -2;
		}

		//
		// 3. Create IStorage interface
		//
		CComPtr<IStorage> pStorage = NULL;
		hr = ::StgCreateDocfileOnILockBytes(pLockBytes,
				STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &pStorage);
		if (FAILED(hr))
		{
			return -3;
		}

		//
		// 4. Add OLE to Richedit
		//
		CLSID clsid = CLSID_NULL;
		hr = pOleObject->GetUserClassID(&clsid);

		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		reobject.clsid = clsid;
		reobject.cp = cp;
		reobject.dvaspect = DVASPECT_CONTENT;
		reobject.dwFlags = REO_BELOWBASELINE;
		reobject.dwUser = 7878; // ǩ������dwUserΪ7878ʱ������ʾ�����ǲ����ole
		reobject.poleobj = pOleObject;
		reobject.polesite = pClientSite;
		reobject.pstg = pStorage;

		// REOBJECT::sizel�ĵ�λ��0.01����
		SIZEL sizel = {GetXHimetricFromPixel(nWidth), GetYHimetricFromPixel(nHeight)};
		reobject.sizel = sizel;
		hr = pRichEditOle->InsertObject(&reobject);
		if (FAILED(hr))
		{
			return -4;
		}

		CComPtr<IOleClientSite> pOldClientSite = NULL;
		hr = pOleObject->GetClientSite(&pOldClientSite);
		if (pOldClientSite == NULL)
		{
			pOleObject->SetClientSite(pClientSite);
		}
	}
	catch (...)
	{
	}

	return 0;
}

int CRichEditCtrlBase::InsertText(LPCTSTR lpszText, int iPos, COLORREF cr)
{
	if (lpszText == NULL || _tcslen(lpszText) == 0)
	{
		return 1;
	}

	long iStartPos = 0;
	long iEndPos = 0;

	if (iPos == SEEK_END)
	{
		if (m_bWLWnd)
		{
			iStartPos = ((CWLRichEditCtrl *)m_pBindWLWnd)->GetTextLengthEx(GTL_DEFAULT | GTL_NUMCHARS);
			((CWLRichEditCtrl *)m_pBindWLWnd)->SetSel(iStartPos, iStartPos);
		}
		else
		{
#if(_MSC_VER <= 1200) // VC6.0
			iStartPos = ((CSkinRichEditCtrl *)m_pBindWnd)->GetTextLength();
#else
			iStartPos = ((CSkinRichEditCtrl *)m_pBindWnd)->GetTextLengthEx(GTL_DEFAULT | GTL_NUMCHARS);
#endif
			((CSkinRichEditCtrl *)m_pBindWnd)->SetSel(iStartPos, iStartPos);
		}
	}
	else if (iPos == SEEK_CUR)
	{
		RE_CALL_FUN(GetSel(iStartPos, iEndPos));
	}
	else if (iPos == SEEK_SET)
	{
		RE_CALL_FUN(SetSel(0, 0));
	}

	// CHARFORMAT::yOffset��PARAFORMAT2�б����ĵ�λ��羣�1羵���1/1440Ӣ��
	CHARFORMAT cf = {0};
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask |= CFM_COLOR;
	cf.dwMask |= CFM_OFFSET;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.crTextColor = cr;
	cf.yOffset = GetYTwipsFromPixel(10);

	RE_CALL_FUN(ReplaceSel(lpszText));
	iEndPos = iStartPos + (long)_tcslen(lpszText);
	RE_CALL_FUN(SetSel(iStartPos, iEndPos));
	RE_CALL_FUN(SetSelectionCharFormat(cf));

	// 	if (iPos != SEEK_CUR)
	// 	{
	// 		m_reHost.SendMessage(WM_VSCROLL, (iPos == 0 ? SB_TOP : SB_BOTTOM), 0);
	// 	}

	return 0;
}
