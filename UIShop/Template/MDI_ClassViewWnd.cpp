// ClassViewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "$$Root$$.h"
#include "ClassViewWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassViewWnd

CClassViewWnd::CClassViewWnd()
{
	m_treeClass = NULL;
}

CClassViewWnd::~CClassViewWnd()
{
}


BEGIN_MESSAGE_MAP(CClassViewWnd, CUIWnd)
	//{{AFX_MSG_MAP(CClassViewWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

int CClassViewWnd::InitClassTree()
{
	CBitmap bmp;
	if (!bmp.LoadBitmap(IDB_CLASS_VIEW))
	{
		TRACE(_T("Can't load bitmap: %x\n"), IDB_CLASS_VIEW);
		ASSERT(FALSE);
		return -1;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	m_imgList.Create(16, bmpObj.bmHeight, ILC_COLOR24|ILC_MASK, 0, 0);
	m_imgList.Add(&bmp, RGB(255, 0, 0));

	m_treeClass = (CSkinTreeCtrl *)GetDlgItem(IDC_TREECTRL_CLASS);
	m_treeClass->SetImageList(&m_imgList, TVSIL_NORMAL);

	HTREEITEM hRoot = m_treeClass->InsertItem(_T("LibUIDK classes"), 0, 0);
	m_treeClass->InsertItem(_T("CClockCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CHtmlCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CIUIDocManager"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CIUIDocument"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CIUIFrameWnd"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CIUIMDIChildWnd"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CIUIMDIClientWnd"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CIUIMDIFrameWnd"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CIUIMultiDocTemplate"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CIUIView"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CIUIWinApp"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CMemStdioFile"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CMenuBar"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CScrollBarEx"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinButton"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinComboBox"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinDateTimeCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinEdit"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinHeaderCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinHotKeyCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinIPAddressCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinListCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinProgressCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinRichEditCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinSliderCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinSpinButtonCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinStatic"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSkinTreeCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CSliderCtrlEx"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CStackPanel"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CTaskWndMgr"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CThumbnailCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CTreeListCtrl"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CUIMgr"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CUIWnd"), 1, 1, hRoot);
	m_treeClass->InsertItem(_T("CWaveCtrl"), 1, 1, hRoot);

	m_treeClass->Expand(hRoot, TVE_EXPAND);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CClassViewWnd message handlers

int CClassViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	InitClassTree();

	return 0;
}
