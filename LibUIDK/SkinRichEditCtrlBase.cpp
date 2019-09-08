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

	// RichEdit是否需要刷新，一般在WM_SIZE中，置为TRUE，在OnDraw中，如果发现为TRUE，
	// 就根据新的控件尺寸，调整内容的显示后，置为FALSE
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

// 测量RichEdit文本在指定宽度范围内显示时，需要多高。
int GetNeedHeight(CRichEditCtrlBase *pRe, int nCpFirst, int nCpLast, LONG lWidth)
{
	CRect rcPage(0, 0, GetXTwipsFromPixel(lWidth), GetYTwipsFromPixel(9999));

	HDC hDC = GetDC(GetDesktopWindow());
	LRESULT lr = 0;
	FORMATRANGE sf = {0};
	sf.hdc = hDC;
	sf.hdcTarget = hDC;
	sf.rc = rcPage; // FORMATRANGE::rc的单位为缇(twips)，
	sf.rcPage = rcPage;
	sf.chrg.cpMin = nCpFirst; // 如果FORMATRANGE::chrg.cpMin为0，且FORMATRANGE::chrg.cpMax为-1，则测量Richedit所有字符
	sf.chrg.cpMax = nCpLast;

	// 第二遍调用FormatRange是为了释放内存，详情请参考MSDN:EM_FORMATRANGE
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

	// 新测量出来的高度，单位为缇，需转成像素使用。
	int nNeedHeight = GetYPixelFromTwips(sf.rc.bottom);

	return nNeedHeight;
}

// 得到指定行的坐标，后三个参数都可以为NULL
// 求RichEdit中指定cp的坐标，有几种方法：
// 1. 使用PosFromChar或GetCharPos(两者内部都是调用EM_POSFROMCHAR)，可以得到指定cp左上角坐标。
// 通过指定cp的下一个cp和下一行cp, 可以得到指定cp的完整坐标(注意，当整个RichEdit就一个字符时，
// 仍然可以通过PosFromChar(1)，得到这个字符右上角坐标，虽然并没有cp为1的字符, 这是因为每个段落
// 末尾，都有一个段落结束符。 【但是】如果一个段落显示成了两行，则第一行的行尾，没有段落结束符,
// 所以通过第一行最后一个字符的cp + 1，无法得到最后一个字符右上角坐标，而是得到下一行第一个字符左上
// 角坐标。 不过，当有某个段落显示成多行时，我们不需要计算这种情况的气泡宽度，因为整个RichEdit
// 的宽度，就是气泡的宽度，不需要逐行计算)
// 2. 通过使用FormatRange，可以模拟输出，从而得到所需要的高度坐标（宽度自己指定）
// 3. 通过ITextRange::GetPoint，可以得到选中范围完整坐标。 它与方法一在微软RichEdit内部都是由
// CDisplayML::PointFromTp实现。 不过使用这个接口有个注意事项，在VS2008 sp1本地MSDN介绍中，
// ITextRange::GetPoint第一个参数只有tomStart或tomEnd与TA_TOP、TA_BASELINE、TA_BOTTOM或TA_LEFT、
// TA_CENTER、TA_RIGHT的组合。 但这样只能得到可视范围内cp的坐标。 当cp滚动到可见范围外后，得到的是0。
// 不过。 在线的MSDN上，提供了一些额外的参数，可以得到可视范围外的坐标，所有参数如下：
// None					0 		无选项。
// IncludeInset			1 		将左侧和顶部嵌入添加到矩形的左侧和顶部坐标，并从坐标的右侧和底部除去右侧和底部嵌入。
// Start				32		文本范围的开始位置。
// ClientCoordinates	256		返回工作区坐标而不是屏幕坐标。
// AllowOffClient		512 	允许工作区之外的点。
// Transform			1024 	使用宿主应用程序提供的世界转换的变换坐标。
// NoHorizontalScroll	65536 	水平滚动已禁用。
// NoVerticalScroll		262144 	垂直滚动已禁用。
// 所以ITextRange::GetPoint的第一个参数，只要或上512，就可以得到可视区外的坐标了。 不过。这些新的属
// 性，不支持旧版本的RichEdit.
int CRichEditCtrlBase::GetLineRect(int nLineIndex, __out POINT *pptStart, __out int *pnLineWidth, __out int *pnLineHeight)
{
	if (pptStart == NULL && pnLineWidth == NULL && pnLineHeight == NULL)
	{
		return -1;
	}

	// 得到本行起始cp
	int nCpFirst = -1;
	RE_CALL_FUN_RETURN(nCpFirst, LineIndex(nLineIndex));

	// 得到起始cp的左上角坐标
	CPoint ptFirst;
	RE_CALL_FUN_RETURN(ptFirst, GetCharPos(nCpFirst));

	if (pptStart != NULL)
	{
		*pptStart = ptFirst;
	}

	if (pnLineWidth != NULL)
	{
		// nCpFirst所在行的字符长度
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

		// 如果有下一行，得到下一行起始cp的坐标后，就可以得到行高
		if (nNextLineCpFirst != -1)
		{
			CPoint ptNextLineFirst;
			RE_CALL_FUN_RETURN(ptNextLineFirst, GetCharPos(nNextLineCpFirst));

			*pnLineHeight = ptNextLineFirst.y - ptFirst.y;
		}

		// 如果没有下一行，通过FormatRange来计算行高
		else
		{
			// nCpFirst所在行的字符长度
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

	// Windowless的RichEdit的ITextServices，由CWLRichEditCtrl设置，有句柄的RichEdit的ITextServices
	// 从IRichEditOle接口中查询。
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

// CSkinRichEditCtrl::GetDC由ITextHost::TxGetDC调用，而ITextHost::TxGetDC是一个调用特别
// 频繁的方法，所以，CSkinRichEditCtrl::GetDC返回的HDC，要缓存起来，只创建一次，在
// CSkinRichEditCtrl销毁时释放这个HDC，否则会引起严重的资源泄漏。
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

		// 注：下面的rc很重要。rc一定要与RichEdit原生显示面积完全一样，即RichEdit的客户区（特别注意，
		// 一定不要包含非客户区）。也就是说，自己调用TxDraw显示的内容，要与WM_PAINT完全对上，否则会引起严重
		// 的闪烁问题。在这个问题上，花了很久解决。这是因为，TxDraw绘制的时候，会计算垂直滚动信息，WM_PAINT
		// 绘制时，也会计算垂直滚动信息，但如果TxDraw使用的rc的宽度与WM_PAINT使用的rc的宽度不一样，则计算出
		// 来的垂直滚动范围就不一样。这样，在RichEdit尺寸变化时，两者来回设置滚动条，导致来回刷新，就引起闪烁了。
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

// 插入了一个Gif
#define ID_TIMER_GIF	10

VOID CALLBACK RichEditGifTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (idEvent == ID_TIMER_GIF)
	{
		// 请求Image ole列表
		std::map<CImageOleCtrl *, int> *pmapGifOles =
			(std::map<CImageOleCtrl *, int> *)::SendMessage(hwnd, EM_GET_GIF_OLE_LIST, 0, 0);

		if (pmapGifOles != NULL && !pmapGifOles->empty())
		{
			std::map<CImageOleCtrl *, int>::iterator it = pmapGifOles->begin();
			for (; it != pmapGifOles->end(); ++it)
			{
				// 这里得到的坐标是相对于屏幕的
				CRect rcOle;
				it->first->IOleCtrlBase::GetWindowRect(rcOle);

				CRect rcRichEdit;
				GetWindowRect(hwnd, rcRichEdit);

				// 因为RichEdit内容，都是在父窗口上画的，所以刷新父窗口
				if (rcRichEdit.IntersectRect(rcRichEdit, rcOle))
				{
					// 因为OLE是画到RichEdit父窗口上的，所以把OLE坐标，转成相对于RichEdit父窗口坐标
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

	// 启动定时器
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
		for (int i = 0; i < 1 + COMBINEIMAGESIZE4; ++i)
		{
			ReleaseIUIImage(strImageName[i]);
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
		reobject.dwUser = 7878; // 签名，当dwUser为7878时，即表示是我们插入的ole
		reobject.poleobj = pOleObject;
		reobject.polesite = pClientSite;
		reobject.pstg = pStorage;

		// REOBJECT::sizel的单位是0.01毫米
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

	// CHARFORMAT::yOffset和PARAFORMAT2中变量的单位是缇，1缇等于1/1440英寸
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
