// TabOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uishop.h"
#include "TabOrderDlg.h"
#include "UIDesignWnd.h"
#include "MainFrm.h"
#include "UIShopView.h"
#include <algorithm>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CMainFrame *g_pFrm;

/////////////////////////////////////////////////////////////////////////////
// CTabOrderDlg dialog


CTabOrderDlg::CTabOrderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabOrderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabOrderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pWndBuddy = NULL;
	m_bHandleNotify = TRUE;
}


void CTabOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabOrderDlg)
	DDX_Control(pDX, IDC_LST_TABORDER, m_ctrlControlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabOrderDlg, CDialog)
	//{{AFX_MSG_MAP(CTabOrderDlg)
	ON_BN_CLICKED(IDC_BTN_MOVEUP, OnBtnMoveUp)
	ON_BN_CLICKED(IDC_BTN_MOVEDOWN, OnBtnMoveDown)
	ON_BN_CLICKED(IDC_BTN_AUTO_ADJUST, &CTabOrderDlg::OnBnClickedBtnAutoAdjust)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_TABORDER, OnItemchangedLstTabOrder)
	ON_BN_CLICKED(IDC_BTN_HIGHLIGHT, &CTabOrderDlg::OnBnClickedBtnHighlight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabOrderDlg message handlers

BOOL CTabOrderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_ctrlControlList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	m_ctrlControlList.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 100);
	m_ctrlControlList.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 300);
	m_ctrlControlList.InsertColumn(2, _T("Old Index"), LVCFMT_LEFT, 80);

	CWnd *pBtnMoveDown = GetDlgItem(IDC_BTN_MOVEDOWN);
	ASSERT(pBtnMoveDown->GetSafeHwnd() != NULL);
	pBtnMoveDown->EnableWindow(FALSE);
	CWnd *pBtnMoveUp = GetDlgItem(IDC_BTN_MOVEUP);
	ASSERT(pBtnMoveUp->GetSafeHwnd());
	pBtnMoveUp->EnableWindow(FALSE);

	UpdateControlsList();

	// Update Title
	CUIDesignWnd *pPreview = (CUIDesignWnd *)m_pWndBuddy;
	ASSERT(pPreview->GetSafeHwnd() != NULL);
	UIWNDPROPERTIES *pUIProp = pPreview->GetUIWndProperties();

	CString strTitle = _T("Tab Order --- ");
	strTitle += pUIProp->m_strID;
	SetWindowText(strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabOrderDlg::OnBtnMoveUp()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlControlList.GetFirstSelectedItemPosition();
	ASSERT(pos != NULL);
	int nItem = m_ctrlControlList.GetNextSelectedItem(pos);

	m_ctrlControlList.SetFocus();

	// Store the old item information.
	DWORD_PTR dwData = m_ctrlControlList.GetItemData(nItem);
	CString strItem = m_ctrlControlList.GetItemText(nItem, 0);
	CString strSubItem1 = m_ctrlControlList.GetItemText(nItem, 1);
	CString strSubItem2 = m_ctrlControlList.GetItemText(nItem, 2);

	// Delete the old item.
	m_ctrlControlList.DeleteItem(nItem);

	// Insert new item and set old information.
	m_ctrlControlList.InsertItem(nItem-1, strItem);
	m_ctrlControlList.SetItemText(nItem-1, 1, strSubItem1);
	m_ctrlControlList.SetItemText(nItem-1, 2, strSubItem2);
	m_ctrlControlList.SetItemData(nItem-1, dwData);
	m_ctrlControlList.SetItemState(nItem-1, LVIS_SELECTED, LVIS_SELECTED);

	// Ensure the new item visible
	m_ctrlControlList.EnsureVisible(nItem - 1, FALSE);
}

void CTabOrderDlg::OnBtnMoveDown()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlControlList.GetFirstSelectedItemPosition();
	ASSERT(pos != NULL);
	int nItem = m_ctrlControlList.GetNextSelectedItem(pos);

	m_ctrlControlList.SetFocus();

	// Store the old item information.
	DWORD_PTR dwData = m_ctrlControlList.GetItemData(nItem);
	CString strItem = m_ctrlControlList.GetItemText(nItem, 0);
	CString strSubItem1 = m_ctrlControlList.GetItemText(nItem, 1);
	CString strSubItem2 = m_ctrlControlList.GetItemText(nItem, 2);

	// Delete the old item.
	m_ctrlControlList.DeleteItem(nItem);

	// Insert new item and set old information.
	m_ctrlControlList.InsertItem(nItem+1, strItem);
	m_ctrlControlList.SetItemText(nItem+1, 1, strSubItem1);
	m_ctrlControlList.SetItemText(nItem+1, 2, strSubItem2);
	m_ctrlControlList.SetItemData(nItem+1, dwData);
	m_ctrlControlList.SetItemState(nItem+1, LVIS_SELECTED, LVIS_SELECTED);

	// Ensure the new item visible
	m_ctrlControlList.EnsureVisible(nItem + 1, FALSE);
}

CWnd * CTabOrderDlg::SetBuddy(CWnd *pWndBuddy)
{
	CWnd *pOldWnd = m_pWndBuddy;
	m_pWndBuddy = pWndBuddy;
	return pOldWnd;
}

void CTabOrderDlg::OnItemchangedLstTabOrder(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	//
	// Update the state of Move Up and Move Down button.
	//
	CUIDesignWnd *pWnd = (CUIDesignWnd *)m_pWndBuddy;

	int nItem = -1;
	POSITION pos = m_ctrlControlList.GetFirstSelectedItemPosition();
	CWnd *pBtnMoveDown = GetDlgItem(IDC_BTN_MOVEDOWN);
	CWnd *pBtnMoveUp = GetDlgItem(IDC_BTN_MOVEUP);
	if (pos != NULL)
	{
		nItem = m_ctrlControlList.GetNextSelectedItem(pos);
	}
	else
	{
		pBtnMoveDown->EnableWindow(FALSE);
		pBtnMoveUp->EnableWindow(FALSE);
	}

	size_t nControlCount = pWnd->m_listCtrl.size();
	ASSERT(pBtnMoveDown->GetSafeHwnd() != NULL);
	if (nItem == nControlCount-1 || nItem == -1)
		pBtnMoveDown->EnableWindow(FALSE);
	else
		pBtnMoveDown->EnableWindow(TRUE);

	ASSERT(pBtnMoveUp->GetSafeHwnd() != NULL);
	if (nItem == 0 || nItem == -1)
		pBtnMoveUp->EnableWindow(FALSE);
	else
		pBtnMoveUp->EnableWindow(TRUE);


	//
	// Make the editing control 'background show' (Show windowless control as parent)
	//
	if (pNMListView->iItem >= 0 && m_bHandleNotify)
	{
		CControlWnd *pCtrlWnd = pWnd->m_listCtrl[pNMListView->iItem];
		ASSERT(pCtrlWnd->GetSafeHwnd() != NULL);

		if ((CONTROL_TYPE)pCtrlWnd->GetControlProperties()->m_eControlType == CT_WL_PICTURE)
		{
			BOOL bChecked = m_ctrlControlList.GetCheck(pNMListView->iItem);
			if (bChecked)
			{
				pCtrlWnd->ShowWindow(SW_HIDE);
			}
			else
			{
				pCtrlWnd->ShowWindow(SW_SHOW);
			}

			pWnd->RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
		}
	}

	*pResult = 0;
}

void CTabOrderDlg::UpdateControlsList()
{
	m_ctrlControlList.DeleteAllItems();

	CUIDesignWnd *pPreview = (CUIDesignWnd *)m_pWndBuddy;
	if (pPreview->GetSafeHwnd() == NULL)
	{
		// May be the window wasn't edited.
		ASSERT(FALSE);
		return;
	}

	UIWNDPROPERTIES *pUIProp = pPreview->GetUIWndProperties();
	int nCtrlCount = (int)pUIProp->m_vControls.size();
	for (int i=0; i<nCtrlCount; ++i)
	{
		CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[i];

		m_bHandleNotify = FALSE;
		int nIndex = m_ctrlControlList.InsertItem(i, GetControlTypeString(pCtrlProp->m_eControlType));
		m_bHandleNotify = TRUE;

		// Check the item
		if (!pPreview->m_listCtrl[i]->IsWindowVisible())
		{
			m_ctrlControlList.SetCheck(nIndex, TRUE);
		}

		m_ctrlControlList.SetItemText(nIndex, 1, pCtrlProp->m_strID);
		CString strIndex;
		strIndex.Format(_T("%d"), i + 1);
		m_ctrlControlList.SetItemText(nIndex, 2, strIndex);
		m_ctrlControlList.SetItemData(nIndex, i);
	}
}

void CTabOrderDlg::OnOK()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CUIDesignWnd *pWnd = (CUIDesignWnd *)m_pWndBuddy;
	UIWNDPROPERTIES *pUIProp = pWnd->GetUIWndProperties();

	vector<CTRLPROPERTIES *> vTemp;
	int nCount = m_ctrlControlList.GetItemCount();
	ASSERT(nCount == pUIProp->m_vControls.size());
	for (int i=0; i<nCount; ++i)
	{
		DWORD_PTR dwOldIndex = m_ctrlControlList.GetItemData(i);
		CTRLPROPERTIES *p = pUIProp->m_vControls[dwOldIndex];
		vTemp.push_back(p);
	}
	pUIProp->m_vControls.clear();
	ASSERT(pUIProp->m_vControls.size() == 0);
	pUIProp->m_vControls.assign(vTemp.begin(), vTemp.end());

	pWnd->ShowIndexFlag(pWnd->IsIndexFlagShow());

	CUIShopDoc *pDoc = g_pFrm->GetUIShopDoc();
	pDoc->SetModifiedFlag(TRUE);

	CDialog::OnOK();
}

bool T2BL2R(CTRLPROPERTIES *pCtrlProp1, CTRLPROPERTIES *pCtrlProp2)	// Top to bottom and left to right order
{
	CRect rc1 = pCtrlProp1->m_rcControl;
	CRect rc2 = pCtrlProp2->m_rcControl;

	if (rc1.top < rc2.top)
	{
		return true;
	}
	else if (rc1.top == rc2.top)
	{
		if (rc1.left <= rc2.left)
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

void CTabOrderDlg::OnBnClickedBtnAutoAdjust()
{
	CUIDesignWnd *pWnd = (CUIDesignWnd *)m_pWndBuddy;
	UIWNDPROPERTIES *pUIProp = pWnd->GetUIWndProperties();

	// Copy control property to temp vector
	vector<CTRLPROPERTIES *> vTemp;
	vTemp.assign(pUIProp->m_vControls.begin(), pUIProp->m_vControls.end());

	// Sort the temp vector
	std::stable_sort(vTemp.begin(), vTemp.end(), T2BL2R);

	// Recopy to control property list.
	pUIProp->m_vControls.clear();
	pUIProp->m_vControls.assign(vTemp.begin(), vTemp.end());

	CUIShopDoc *pDoc = g_pFrm->GetUIShopDoc();
	pDoc->SetModifiedFlag(TRUE);

	pWnd->ShowIndexFlag(pWnd->IsIndexFlagShow());

	CDialog::OnOK();
}

void CTabOrderDlg::OnBnClickedBtnHighlight()
{
	POSITION pos = m_ctrlControlList.GetFirstSelectedItemPosition();
	ASSERT(pos != NULL);
	int nItem = m_ctrlControlList.GetNextSelectedItem(pos);

	m_ctrlControlList.SetFocus();

	DWORD_PTR dwData = m_ctrlControlList.GetItemData(nItem);

	// Highlight the selected control
	CUIDesignWnd *pWnd = (CUIDesignWnd *)m_pWndBuddy;
	UIWNDPROPERTIES *pUIProp = pWnd->GetUIWndProperties();
	CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[dwData];
	ASSERT(pCtrlProp != NULL);

	pWnd->HighlightControl(pCtrlProp);
}
