// TestWnd.cpp : implementation file
//

#include "stdafx.h"
#include "uishop.h"
#include "TestWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int GenerateWave(CWave *pWave)
{
	if (pWave == NULL)
		return -1;

	POINT item;

	item.x = 0;
	item.y = 90;
	pWave->AddWaveItem(&item);

	item.x = 1;
	item.y = 80;
	pWave->AddWaveItem(&item);

	item.x = 2;
	item.y = 87;
	pWave->AddWaveItem(&item);

	item.x = 3;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 4;
	item.y = 69;
	pWave->AddWaveItem(&item);

	item.x = 5;
	item.y = 65;
	pWave->AddWaveItem(&item);

	item.x = 6;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 7;
	item.y = 65;
	pWave->AddWaveItem(&item);

	item.x = 8;
	item.y = 65;
	pWave->AddWaveItem(&item);

	item.x = 9;
	item.y = 78;
	pWave->AddWaveItem(&item);

	//
	item.x = 10;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 11;
	item.y = 72;
	pWave->AddWaveItem(&item);

	item.x = 12;
	item.y = 66;
	pWave->AddWaveItem(&item);

	item.x = 13;
	item.y = 71;
	pWave->AddWaveItem(&item);

	item.x = 14;
	item.y = 68;
	pWave->AddWaveItem(&item);

	item.x = 15;
	item.y = 75;
	pWave->AddWaveItem(&item);

	item.x = 16;
	item.y = 72;
	pWave->AddWaveItem(&item);

	item.x = 17;
	item.y = 75;
	pWave->AddWaveItem(&item);

	item.x = 18;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 19;
	item.y = 65;
	pWave->AddWaveItem(&item);

	//
	item.x = 20;
	item.y = 60;
	pWave->AddWaveItem(&item);

	item.x = 21;
	item.y = 45;
	pWave->AddWaveItem(&item);

	item.x = 22;
	item.y = 40;
	pWave->AddWaveItem(&item);

	item.x = 23;
	item.y = 42;
	pWave->AddWaveItem(&item);

	item.x = 24;
	item.y = 38;
	pWave->AddWaveItem(&item);

	item.x = 25;
	item.y = 45;
	pWave->AddWaveItem(&item);

	item.x = 26;
	item.y = 43;
	pWave->AddWaveItem(&item);

	item.x = 27;
	item.y = 47;
	pWave->AddWaveItem(&item);

	item.x = 28;
	item.y = 35;
	pWave->AddWaveItem(&item);

	item.x = 29;
	item.y = 45;
	pWave->AddWaveItem(&item);

	//
	item.x = 30;
	item.y = 50;
	pWave->AddWaveItem(&item);

	item.x = 31;
	item.y = 55;
	pWave->AddWaveItem(&item);

	item.x = 32;
	item.y = 52;
	pWave->AddWaveItem(&item);

	item.x = 33;
	item.y = 54;
	pWave->AddWaveItem(&item);

	item.x = 34;
	item.y = 57;
	pWave->AddWaveItem(&item);

	item.x = 35;
	item.y = 53;
	pWave->AddWaveItem(&item);

	item.x = 36;
	item.y = 60;
	pWave->AddWaveItem(&item);

	item.x = 37;
	item.y = 58;
	pWave->AddWaveItem(&item);

	item.x = 38;
	item.y = 62;
	pWave->AddWaveItem(&item);

	item.x = 39;
	item.y = 55;
	pWave->AddWaveItem(&item);

#if 1
	//
	item.x = 40;
	item.y = 60;
	pWave->AddWaveItem(&item);

	item.x = 41;
	item.y = 52;
	pWave->AddWaveItem(&item);

	item.x = 42;
	item.y = 64;
	pWave->AddWaveItem(&item);

	item.x = 43;
	item.y = 67;
	pWave->AddWaveItem(&item);

	item.x = 44;
	item.y = 85;
	pWave->AddWaveItem(&item);

	item.x = 45;
	item.y = 82;
	pWave->AddWaveItem(&item);

	item.x = 46;
	item.y = 90;
	pWave->AddWaveItem(&item);

	item.x = 47;
	item.y = 87;
	pWave->AddWaveItem(&item);

	item.x = 48;
	item.y = 91;
	pWave->AddWaveItem(&item);

	item.x = 49;
	item.y = 70;
	pWave->AddWaveItem(&item);

	//
	item.x = 50;
	item.y = 77;
	pWave->AddWaveItem(&item);

	item.x = 51;
	item.y = 73;
	pWave->AddWaveItem(&item);

	item.x = 52;
	item.y = 80;
	pWave->AddWaveItem(&item);

	item.x = 53;
	item.y = 75;
	pWave->AddWaveItem(&item);

	item.x = 54;
	item.y = 83;
	pWave->AddWaveItem(&item);

	item.x = 55;
	item.y = 80;
	pWave->AddWaveItem(&item);

	item.x = 56;
	item.y = 75;
	pWave->AddWaveItem(&item);

	item.x = 57;
	item.y = 83;
	pWave->AddWaveItem(&item);

	item.x = 58;
	item.y = 80;
	pWave->AddWaveItem(&item);

	item.x = 59;
	item.y = 78;
	pWave->AddWaveItem(&item);

	//
	item.x = 60;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 61;
	item.y = 65;
	pWave->AddWaveItem(&item);

	item.x = 62;
	item.y = 60;
	pWave->AddWaveItem(&item);

	item.x = 63;
	item.y = 62;
	pWave->AddWaveItem(&item);

	item.x = 64;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 65;
	item.y = 80;
	pWave->AddWaveItem(&item);

	item.x = 66;
	item.y = 73;
	pWave->AddWaveItem(&item);

	item.x = 67;
	item.y = 81;
	pWave->AddWaveItem(&item);

	item.x = 68;
	item.y = 81;
	pWave->AddWaveItem(&item);

	item.x = 69;
	item.y = 84;
	pWave->AddWaveItem(&item);

	//
	item.x = 70;
	item.y = 73;
	pWave->AddWaveItem(&item);

	item.x = 71;
	item.y = 76;
	pWave->AddWaveItem(&item);

	item.x = 72;
	item.y = 73;
	pWave->AddWaveItem(&item);

	item.x = 73;
	item.y = 74;
	pWave->AddWaveItem(&item);

	item.x = 74;
	item.y = 77;
	pWave->AddWaveItem(&item);

	item.x = 75;
	item.y = 81;
	pWave->AddWaveItem(&item);

	item.x = 76;
	item.y = 77;
	pWave->AddWaveItem(&item);

	item.x = 77;
	item.y = 80;
	pWave->AddWaveItem(&item);

	item.x = 78;
	item.y = 77;
	pWave->AddWaveItem(&item);

	item.x = 79;
	item.y = 73;
	pWave->AddWaveItem(&item);

	//
	item.x = 80;
	item.y = 75;
	pWave->AddWaveItem(&item);

	item.x = 81;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 82;
	item.y = 72;
	pWave->AddWaveItem(&item);

	item.x = 83;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 84;
	item.y = 75;
	pWave->AddWaveItem(&item);

	item.x = 85;
	item.y = 60;
	pWave->AddWaveItem(&item);

	item.x = 86;
	item.y = 64;
	pWave->AddWaveItem(&item);

	item.x = 87;
	item.y = 64;
	pWave->AddWaveItem(&item);

	item.x = 88;
	item.y = 67;
	pWave->AddWaveItem(&item);

	item.x = 88;
	item.y = 64;
	pWave->AddWaveItem(&item);

	item.x = 90;
	item.y = 79;
	pWave->AddWaveItem(&item);

	//
	item.x = 91;
	item.y = 78;
	pWave->AddWaveItem(&item);

	item.x = 92;
	item.y = 80;
	pWave->AddWaveItem(&item);

	item.x = 93;
	item.y = 75;
	pWave->AddWaveItem(&item);

	item.x = 94;
	item.y = 73;
	pWave->AddWaveItem(&item);

	item.x = 95;
	item.y = 77;
	pWave->AddWaveItem(&item);

	item.x = 96;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 97;
	item.y = 75;
	pWave->AddWaveItem(&item);

	item.x = 98;
	item.y = 70;
	pWave->AddWaveItem(&item);

	item.x = 99;
	item.y = 65;
	pWave->AddWaveItem(&item);

	//
	item.x = 100;
	item.y = 60;
	pWave->AddWaveItem(&item);

	item.x = 101;
	item.y = 55;
	pWave->AddWaveItem(&item);

	item.x = 102;
	item.y = 55;
	pWave->AddWaveItem(&item);

	item.x = 103;
	item.y = 57;
	pWave->AddWaveItem(&item);

	item.x = 104;
	item.y = 50;
	pWave->AddWaveItem(&item);

	item.x = 105;
	item.y = 45;
	pWave->AddWaveItem(&item);

	item.x = 106;
	item.y = 47;
	pWave->AddWaveItem(&item);

	item.x = 107;
	item.y = 40;
	pWave->AddWaveItem(&item);

	item.x = 108;
	item.y = 55;
	pWave->AddWaveItem(&item);

	item.x = 109;
	item.y = 50;
	pWave->AddWaveItem(&item);

	//
	item.x = 110;
	item.y = 53;
	pWave->AddWaveItem(&item);

	item.x = 111;
	item.y = 45;
	pWave->AddWaveItem(&item);

	item.x = 112;
	item.y = 40;
	pWave->AddWaveItem(&item);

	item.x = 113;
	item.y = 41;
	pWave->AddWaveItem(&item);

	item.x = 114;
	item.y = 35;
	pWave->AddWaveItem(&item);

	item.x = 115;
	item.y = 30;
	pWave->AddWaveItem(&item);

	item.x = 116;
	item.y = 27;
	pWave->AddWaveItem(&item);

	item.x = 117;
	item.y = 20;
	pWave->AddWaveItem(&item);

	item.x = 118;
	item.y = 17;
	pWave->AddWaveItem(&item);

	item.x = 119;
	item.y = 10;
	pWave->AddWaveItem(&item);

	item.x = 120;
	item.y = 13;
	pWave->AddWaveItem(&item);
#endif

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTestWnd

CTestWnd::CTestWnd()
{
}

CTestWnd::~CTestWnd()
{
}


BEGIN_MESSAGE_MAP(CTestWnd, CUIWnd)
	//{{AFX_MSG_MAP(CTestWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_COMMAND(IDSM_CLOSE, OnMenuClose)
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

int CTestWnd::InitVirtualListItems()
{
	for (int i = 0; i < VIRTUAL_LIST_ITEM_COUNT; ++i)
	{
		CString strItem;
		strItem.Format(_T("Item %d"), i);

		CString strSubItem1;
		strSubItem1.Format(_T("Sub Item %d"), i);

		TSTRCPY(m_VirtualListItems[i].m_szItemText, 12, strItem);
		TSTRCPY(m_VirtualListItems[i].m_szSubItem1Text, 16, strSubItem1);
	}

	m_VirtualListItems[0].m_iImage = 1;	// Means inited.

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTestWnd message handlers

void CTestWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));

	CUIWnd::OnLButtonDown(nFlags, point);
}

int CTestWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	SetDragResizeFlag(TRUE);

	HWND hWnd = NULL;
	HICON hIcon1 = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	HICON hIcon2 = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_UIShopTYPE));
	HICON hIcon3 = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ALPHAICON));
	if (m_imgList.GetSafeHandle() == NULL)
	{
		m_imgList.Create(16, 16, ILC_COLOR32|ILC_MASK, 1, 1);
		m_imgList.Add(hIcon1);
		m_imgList.Add(hIcon2);
		m_imgList.Add(hIcon3);
	}

	// If the child window is Edit box, set a test string for it
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(this->m_hWnd, hWnd, _T("Edit"), NULL);
		if (::IsWindow(hWnd))
		{
			::SendMessage(hWnd, WM_SETTEXT, 0, LPARAM(_T("This is an Edit box")));
		}
	}
	while(hWnd != NULL);

	// If the child window is combo box, insert test string and set the current selected item
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(this->m_hWnd, hWnd, _T("ComboBox"), NULL);
		if (::IsWindow(hWnd))
		{
			for (int i=0; i<10; ++i)
			{
				CString str;
				str.Format(_T("combo box item %d"), i);
				::SendMessage(hWnd, CB_INSERTSTRING, i, (LPARAM)str.GetBuffer(str.GetLength()));
			}
			::SendMessage(hWnd, CB_SETCURSEL, 0, 0);
		}
	}
	while (hWnd != NULL);

	// If the child window is progress control, set it's position to 30
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(this->m_hWnd, hWnd, _T("msctls_progress32"), NULL);
		if (::IsWindow(hWnd))
		{
			::SendMessage(hWnd, PBM_SETPOS, 6, 0L);
			::SendMessage(hWnd, PBM_SETRANGE32, 0, 20);
		}
	}
	while (hWnd != NULL);

	// If the child window is IP address control, insert test item
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, _T("SysIPAddress32"), NULL);
		if (::IsWindow(hWnd))
		{
			CSkinIPAddressCtrl *pWnd = (CSkinIPAddressCtrl *)CWnd::FromHandle(hWnd);
			pWnd->SetAddress(192, 168, 0, 1);
		}
	}
	while(hWnd != NULL);

	// If the child window is extanted slider, set it's position to 30
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, WC_TRACKBAREX, NULL);
		if (::IsWindow(hWnd))
		{
			CSliderCtrlEx *pWnd = (CSliderCtrlEx *)CWnd::FromHandle(hWnd);
			pWnd->SetPos(30);
		}
	}
	while(hWnd != NULL);

	// If the child window is thumbnail view, insert an item
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(this->m_hWnd, hWnd, WC_THUMBNAILVIEW, NULL);
		if (::IsWindow(hWnd))
		{
			CThumbnailCtrl *pWnd = (CThumbnailCtrl*)CWnd::FromHandle(hWnd);
			for (int i=0; i<10; ++i)
			{
				CString strItem;
				strItem.Format(_T("Item_%d"), i);
				pWnd->InsertItem(i, strItem);
			}
		}
	}
	while (hWnd != NULL);

	// If the child window is tree or tree control, insert test item
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, _T("SysTreeView32"), NULL);
		if (::IsWindow(hWnd))
		{
			CTreeCtrl *pWnd = (CTreeCtrl *)CWnd::FromHandle(hWnd);
			pWnd->SetImageList(&m_imgList, TVSIL_NORMAL);
			int i = 0;
			for (i=0; i<10; ++i)
			{
				CString strItem;
				strItem.Format(_T("Item0_%d"), i);
				HTREEITEM hTI2 = pWnd->InsertItem(strItem, 0, 0, TVI_ROOT);

				for (int j=0; j<5; ++j)
				{
					CString strSubItem;
					strSubItem.Format(_T("Item0_%d_%d"), i, j);
					pWnd->InsertItem(strSubItem, i % 3, 1, hTI2);
				}
				pWnd->Expand(hTI2, TVE_EXPAND);
			}
		}
	}
	while (hWnd != NULL);

	// If the child window is tree or tree control, insert test item
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, _T("SysTreeCtrl32"), NULL);
		if (::IsWindow(hWnd))
		{
			CTreeCtrl *pWnd = (CTreeCtrl *)CWnd::FromHandle(hWnd);
			pWnd->SetImageList(&m_imgList, TVSIL_NORMAL);
			int i = 0;
			for (i=0; i<10; ++i)
			{
				CString strItem;
				strItem.Format(_T("Item0_%d"), i);
				HTREEITEM hTI2 = pWnd->InsertItem(strItem, 0, 0, TVI_ROOT);

				for (int j=0; j<5; ++j)
				{
					CString strSubItem;
					strSubItem.Format(_T("Item0_%d_%d"), i, j);
					pWnd->InsertItem(strSubItem, i % 3, 1, hTI2);
				}
				pWnd->Expand(hTI2, TVE_EXPAND);
			}
		}
	}
	while (hWnd != NULL);

	// If the child window is tree or tree-list control, insert test item
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, WC_TREELISTVIEW, NULL);
		if (::IsWindow(hWnd))
		{
			CTreeListCtrl *pWnd = (CTreeListCtrl *)CWnd::FromHandle(hWnd);
			CTreeList *pTree = pWnd->GetTreeListWnd();
			pWnd->InsertColumn(0, _T("Column0"), LVCFMT_LEFT, 100);
			pWnd->InsertColumn(1, _T("Column1"), LVCFMT_LEFT, 100);

			pTree->SetItemHeight(20);

			HTREEITEM hTI = NULL;
			int i = 0;

			hTI = pWnd->InsertItem(_T("Root 0"), TVI_ROOT);
			pTree->SetItemHeightEx(hTI, 20);
			for (i=0; i<10; ++i)
			{
				CString strItem;
				strItem.Format(_T("Item0_%d"), i);
				HTREEITEM hTI2 = pWnd->InsertItem(strItem, hTI);
				pTree->SetItemHeightEx(hTI2, 40);

				for (int j=0; j<5; ++j)
				{
					CString strSubItem;
					strSubItem.Format(_T("Item0_%d_%d"), i, j);
					HTREEITEM hTI3 = pWnd->InsertItem(strSubItem, hTI2);
					pTree->SetItemHeightEx(hTI3, 60);
				}
			}

			hTI = pWnd->InsertItem(_T("Root 1"), TVI_ROOT);
			pTree->SetItemHeightEx(hTI, 20);
			for (i=0; i<10; ++i)
			{
				CString strItem;
				strItem.Format(_T("Item1_%d"), i);
				HTREEITEM hTI2 = pWnd->InsertItem(strItem, hTI);
				pTree->SetItemHeightEx(hTI2, 40);

				for (int j=0; j<5; ++j)
				{
					CString strSubItem;
					strSubItem.Format(_T("Item0_%d_%d"), i, j);
					HTREEITEM hTI3 = pWnd->InsertItem(strSubItem, hTI2);
					pTree->SetItemHeightEx(hTI3, 60);
				}
			}
		}
	}
	while(hWnd != NULL);

	// If the child windows is a Rich Edit, insert test string
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, _T("RichEdit20A"), NULL);
		if (::IsWindow(hWnd))
		{
			::SendMessage(hWnd, WM_SETTEXT, 0, LPARAM(_T("This is a Rich Edit box")));
		}
	}
	while(hWnd != NULL);

	// If the child windows is a ListCtrl, insert test items
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, _T("SysListView32"), NULL);
		if (::IsWindow(hWnd))
		{
			LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
			BOOL bOwnerData = ((lStyle & LVS_OWNERDATA) == LVS_OWNERDATA);
			if ((lStyle & LVS_REPORT) == LVS_REPORT)
			{
				CSkinListCtrl *pWnd = (CSkinListCtrl *)CWnd::FromHandle(hWnd);
				pWnd->SetImageList(&m_imgList, LVSIL_SMALL);
				pWnd->EnableToolTips(TRUE);
				pWnd->InsertColumn(0, _T("Column0"), LVCFMT_LEFT, 100);
				pWnd->InsertColumn(1, _T("Column1"), LVCFMT_LEFT, 100);

				if (bOwnerData)
				{
					if (m_VirtualListItems[0].m_iImage != 1)
					{
						InitVirtualListItems();
					}

					pWnd->SetItemCountEx(VIRTUAL_LIST_ITEM_COUNT, LVSICF_NOINVALIDATEALL);
				}
				else
				{
					for (int i=0; i < 50; ++i)
					{
						CString strItem;
						strItem.Format(_T("Item_%d_0"), i);
						int nIndex = pWnd->InsertItem(i, strItem, i%3);
						pWnd->SetItemData(nIndex, i);
						DWORD_PTR nData = pWnd->GetItemData(nIndex);
						ASSERT(nData == i);
						strItem.Format(_T("Item_%d_1"), i);
						pWnd->SetItemText(nIndex, 1, strItem);
					}
				}
			}
			else if ((lStyle & LVS_ICON) == LVS_ICON)
			{
				CSkinListCtrl *pWnd = (CSkinListCtrl *)CWnd::FromHandle(hWnd);
				pWnd->SetImageList(&m_imgList, LVSIL_NORMAL);
				pWnd->EnableToolTips(TRUE);

				if (!bOwnerData)
				{
					for (int i=0; i<10; ++i)
					{
						CString strItem;
						strItem.Format(_T("Item_%d_0"), i);
						pWnd->InsertItem(i, strItem, i%3);
					}
				}
			}
		}
	}
	while(hWnd != NULL);

	// If the child window is a extend scrollbar, set the thumb width and position.
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, _T("ScrollBarEx"), NULL);
		if (::IsWindow(hWnd))
		{
			CScrollBarEx *pScrollBarEx = (CScrollBarEx *)CWnd::FromHandle(hWnd);
			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			si.nPage = 20;
			si.nMin = 0;
			si.nMax = 100;
			si.nPos = 30;
			pScrollBarEx->SetScrollInfo(&si);
		}
	}
	while(hWnd != NULL);

	// If the child window is a wave control, add test data to it.
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, WC_WAVECTRL, NULL);
		if (::IsWindow(hWnd))
		{
			CWaveCtrl *pWaveCtrl = (CWaveCtrl *)CWnd::FromHandle(hWnd);

			static CWave wave;
			GenerateWave(&wave);
			pWaveCtrl->AddWave(&wave);
		}
	}
	while(hWnd != NULL);

	// If the child window is a menu bar, assign IDR_MAINFRAME menu to it.
	hWnd = NULL;
	do
	{
		hWnd = ::FindWindowEx(m_hWnd, hWnd, WC_MENUBAR, NULL);
		if (::IsWindow(hWnd))
		{
			CMenuBar *pMenuBar = (CMenuBar *)CWnd::FromHandle(hWnd);

			pMenuBar->SetMenuID(IDR_MAINFRAME);
		}
	}
	while(hWnd != NULL);

	return 0;
}

void CTestWnd::OnMenuClose()
{
	// TODO: Add your command handler code here
	SendMessage(WM_CLOSE);
}

void CTestWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMenu menu;
	menu.LoadMenu(IDR_CLOSETESTWINDOW);
	CMenu *pMenu;
	pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu->GetSafeHmenu() != NULL);
	POINT pt;
	GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);

	CUIWnd::OnRButtonUp(nFlags, point);
}

static int CALLBACK TestListCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	BOOL bFlag = (BOOL)lParamSort;

	if (bFlag)
		return int(lParam1 - lParam2);
	else
		return int(lParam2 - lParam1);

	return 0;
}

LRESULT CTestWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_NOTIFY)
	{
		NMHDR *pnmhdr = (NMHDR *)lParam;

		// Sort the test list control.
		if (pnmhdr->code == LVN_COLUMNCLICK)
		{
			NMLISTVIEW *plv = (NMLISTVIEW *)lParam;
			int nHeaderItem = plv->iSubItem;

			CSkinListCtrl *pList = (CSkinListCtrl *)CWnd::FromHandle(pnmhdr->hwndFrom);
			static BOOL bFlag = FALSE;
			pList->SortItems(TestListCompareProc, bFlag);
			bFlag = !bFlag;
		}
		// For virtual list
		else if (pnmhdr->code == LVN_GETDISPINFO)
		{
			NMLVDISPINFO *pDispInfo = (NMLVDISPINFO *)lParam;
			LV_ITEM *pItem = &pDispInfo->item;

			int iItem = pItem->iItem;

			if (pItem->mask & LVIF_TEXT) //valid text buffer?
			{
				switch(pItem->iSubItem)
				{
				case 0: // fill in main text
					_tcscpy_s(pItem->pszText, pItem->cchTextMax, m_VirtualListItems[iItem].m_szItemText);
					break;
				case 1: // fill in sub item 1 text
					_tcscpy_s(pItem->pszText, pItem->cchTextMax, m_VirtualListItems[iItem].m_szSubItem1Text);
					break;
				}
			}

			if (pItem->mask & LVIF_IMAGE) //valid image?
			{
				pItem->iImage = m_VirtualListItems[iItem].m_iImage;
			}
		}
	}

	return CUIWnd::WindowProc(message, wParam, lParam);
}
