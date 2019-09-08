#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////

CRichMsgItem::CRichMsgItem()
{
	m_uOleWinID = -1;
	m_lParam = 0;
}

CRichMsgItem::~CRichMsgItem()
{

}

// IOleCtrlBase作为OLE子控件的容器。
// IOleCtrlBase可以加载界面编辑器中的窗口，加载不同的窗口，实现不同的OLE
// 例如，在界面编辑器中创建一个窗口，窗口里创建一个图片控件。 IOleCtrlBase加载这个窗口后，就变成一个Image Ole
// 在界面编辑器中创建一个窗口，里面创建一个视频控件，IOleCtrlBase加载后，就变成一个视频OLE
// IOleCtrlBase只需要提供以下三大类功能即可：
// 1. IOleCtrlBase与其子控件的交互
// 2. 让其子控件与RichEdit交互
// 3. 绘制子控件
// LibUIDK仅内置制作IM软件最基础的图片OLE。 其它诸如文件、名片OLE，由用户从IOleCtrlBase派生自己实现
IOleCtrlBase::IOleCtrlBase(HWND hRichEdit)
{
	IDD = -1;
	m_pOleClientSite = NULL;
	m_lParam = 0;

	// 由于OLE控件不一定调用Create被创建出来，所以这里保存它的父窗口
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;
	pMember->m_pParent = CWnd::FromHandle(hRichEdit);
}

IOleCtrlBase::~IOleCtrlBase()
{

}

BOOL IOleCtrlBase::Create(LPCTSTR lpszClassName, LPCTSTR lpszControlName, DWORD dwStyle,
	CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	return CWLWnd::Create(lpszClassName, lpszControlName, dwStyle, CRect(0, 0, 0, 0),
			pParentWnd, nID, pVirtualParent);
}

UINT IOleCtrlBase::GetIDD() const
{
	return IDD;
}

int IOleCtrlBase::SetIDD(UINT nID)
{
	IDD = nID;

	return 0;
}

// IUnknown抽象类接口
HRESULT IOleCtrlBase::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
{
	if (this == NULL)
	{
		return S_FALSE;
	}

	// 必须把this指针类型转换为相应的指针。 否则Ole无法插入到RichEdit中。
	if (riid == IID_IViewObject)
	{
		*ppvObject = (IViewObject *)this;
	}
	else if (riid == IID_IViewObject2)
	{
		*ppvObject = (IViewObject2 *)this;
	}
	else if (riid == IID_IUnknown || riid == IID_IOleObject)
	{
		*ppvObject = (IOleObject *)this;
	}
	else
	{
		return E_NOINTERFACE;
	}

	// 加不加都行，反正一个Ole对应一个C++对象
	AddRef();

	return S_OK;
}

ULONG IOleCtrlBase::AddRef(void)
{
	return S_OK;
}

ULONG IOleCtrlBase::Release(void)
{
	return S_OK;
}

// 由于CRichEditCtrlBase::InsertOle在InsertObject之后，会调用SetClientSite
// 所以可以认为，当m_pOleClientSite的值有效时，就表示OLE已被插入到了RichEdit中。
HRESULT IOleCtrlBase::SetClientSite(/* [unique][in] */ IOleClientSite *pClientSite)
{
	m_pOleClientSite = pClientSite;

	if (m_pOleClientSite == NULL)
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT IOleCtrlBase::GetClientSite(/* [out] */IOleClientSite **ppClientSite)
{
	if (ppClientSite == NULL)
	{
		return E_INVALIDARG;
	}

	*ppClientSite = m_pOleClientSite;

	return S_OK;
}

// {92E05F99-41F4-4914-A50E-96D14D175760}
const GUID LibUIDK::g_guidImageOle =
{ 0x92e05f99, 0x41f4, 0x4914, { 0xa5, 0xe, 0x96, 0xd1, 0x4d, 0x17, 0x57, 0x60 } };

HRESULT IOleCtrlBase::GetUserClassID(/* [out] */ CLSID *pClsid)
{
	*pClsid = g_guidImageOle;

	return S_OK;
}

HRESULT IOleCtrlBase::SetExtent(/* [in] */ DWORD dwDrawAspect, /* [in] */ SIZEL *psizel)
{
	m_sizeExtent = *psizel;
	return S_OK;
}

HRESULT IOleCtrlBase::GetExtent(/* [in] */ DWORD dwDrawAspect, /* [out] */ SIZEL *psizel)
{
	if (psizel == NULL)
	{
		return E_INVALIDARG;
	}

	*psizel = m_sizeExtent;

	return S_OK;
}

HRESULT IOleCtrlBase::Advise(/* [unique][in] */ IAdviseSink *pAdvSink, /* [out] */ DWORD *pdwConnection)
{

	return S_OK;
}

HRESULT IOleCtrlBase::Unadvise(
	/* [in] */ DWORD dwConnection)
{

	return S_OK;
}

// IViewObject2 需要实现的方法
HRESULT IOleCtrlBase::SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink)
{
	return S_OK;
}

HRESULT IOleCtrlBase::GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink)
{
	return S_OK;
}

HRESULT IOleCtrlBase::Draw(
	DWORD dwDrawAspect, LONG lindex,
	void *pvAspect,
	DVTARGETDEVICE *ptd,
	HDC hdcTargetDev,
	HDC hdcDraw,
	LPCRECTL lprcBounds,
	LPCRECTL lprcWBounds,
	BOOL(STDMETHODCALLTYPE *pfnContinue)(ULONG_PTR dwContinue),
	ULONG_PTR dwContinue)
{
	// lprcBounds的坐标，是相对于hdcDraw的。 由于我们是把Windowsed RichEdit的内容，画到了
	// RichEdit的父窗口上。 所以，这里的lprcBounds的坐标，也是相对于Windowsed RichEdit的父窗口
	// 我们把它转成相对于GetParent的坐标。
	m_rect = (RECT *)lprcBounds;

	// 由于RichEdit的OLE不是绘制在RichEdit的DC上，而是绘制在RichEdit父窗口的DC上，而
	// OLE的坐标是相对于RichEdit的，所以要设置视图原点，否则绘制不到正确的位置上
	CPoint ptOrg;

	if (GetParent() != NULL)
	{
		// 判断Ole的父窗口是不是Windowsed RichEdit控件
		CString strClass;
		::GetClassName(GetParent()->GetSafeHwnd(), strClass.GetBufferSetLength(MAX_PATH), MAX_PATH);
		strClass.ReleaseBuffer();

		if (strClass == GetRichEditClassName())
		{
			CWnd *pRichParent = GetParent()->GetParent();
			pRichParent->ClientToScreen(m_rect);
			GetParent()->ScreenToClient(m_rect);

			CRect rcRichEdit;
			GetParent()->GetWindowRect(rcRichEdit);
			pRichParent->ScreenToClient(rcRichEdit);
			ptOrg = rcRichEdit.TopLeft();
		}
	}

	// 绘制自己
	if (IDD != -1)
	{
		CDC *pParentDC = CDC::FromHandle(hdcDraw);

		CWLWnd::OnDraw(pParentDC, &ptOrg);
	}

	return S_OK;
}

HRESULT IOleCtrlBase::GetExtent(
	DWORD dwDrawAspect,
	LONG lindex,
	DVTARGETDEVICE *ptd,
	LPSIZEL lpsizel)
{
	return S_OK;
}

BOOL IOleCtrlBase::IsOleControl() const
{
	return TRUE;
}

int IOleCtrlBase::GetWindowRect(LPRECT lprc) const
{
	if (lprc == NULL)
	{
		return -1;
	}

	// 判断Ole的父窗口是不是Windowsed RichEdit控件
	CString strClass;
	::GetClassName(GetParent()->GetSafeHwnd(), strClass.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strClass.ReleaseBuffer();

	*lprc = m_rect;
	if (strClass == GetRichEditClassName())
	{
		GetParent()->ClientToScreen(lprc);
	}

	return 0;
}

int IOleCtrlBase::GetClientRect(LPRECT lpRect) const
{
	if (lpRect == NULL)
	{
		return -1;
	}

	lpRect->left = lpRect->top = 0;
	lpRect->right = m_rect.Width();
	lpRect->bottom = m_rect.Height();

	return 0;
}

LPARAM IOleCtrlBase::SetUserData(LPARAM lParam)
{
	LPARAM lOldParam = m_lParam;
	m_lParam = lParam;
	return lOldParam;
}

LPARAM IOleCtrlBase::GetUserData() const
{
	return m_lParam;
}

//////////////////////////////////////////////////////////////////////////
// protected

int IOleCtrlBase::OnCreate()
{
	CString strID = CUIMgr::StringIDFromInt(IDD);
	UIWNDPROPERTIES *pUIProp = CUIMgr::GetUIWNDPROPERTIES(strID);
	if (pUIProp != NULL)
	{
		// May be delay loaded.
		if (!BOOL(pUIProp->m_bLoaded))
		{
			CUIMgr::DelayLoadWindow(pUIProp);
		}

		m_uiwndProperties = *pUIProp;
	}

	CWLWnd::SetBkImage((LPCTSTR)m_uiwndProperties.m_strBKNo, FALSE);
	SetBkImageResizeMode(m_uiwndProperties.m_eBkImageResizeMode);
	SetBkImageResizePoint(m_uiwndProperties.m_ptImageResize);

	std::vector<CChildInfo> vChildren;
	CreateChildControls(&m_uiwndProperties, GetParent(), NULL, NULL, NULL, NULL, &vChildren);

	return 0;
}
