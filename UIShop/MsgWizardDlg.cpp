// MsgWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "MsgWizardDlg.h"
#include "Global.h"
#include "MainFrm.h"
#include "UIShopView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CMsgWizardDlg dialog

IMPLEMENT_DYNAMIC(CMsgWizardDlg, CDialog)

CMsgWizardDlg::CMsgWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgWizardDlg::IDD, pParent)
{
}

CMsgWizardDlg::~CMsgWizardDlg()
{
}

void CMsgWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_MESSAGE, m_treeMessages);
	DDX_Control(pDX, IDC_RICHEDIT_CODE, m_reCode);
}


BEGIN_MESSAGE_MAP(CMsgWizardDlg, CDialog)
	ON_NOTIFY(TVN_ITEMCHANGED, IDC_TREE_MESSAGE, &CMsgWizardDlg::OnTvnItemChangedTreeControls)
END_MESSAGE_MAP()


// CMsgWizardDlg message handlers

BOOL CMsgWizardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	HTREEITEM hItem = m_treeMessages.InsertItem(_T("Variable"));
	m_treeMessages.SelectItem(hItem);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgWizardDlg::OnTvnItemChangedTreeControls(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: Add your control notification handler code here

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CUIShopView *pView = pFrame->GetUIShopView();
	if (pView == NULL)
	{
		return;
	}
	CUIDesignWnd *pUIDesignWnd = pView->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		return;
	}
	// Show all selected control's variable.
	vector<CControlWnd *> vSelControls;
	pUIDesignWnd->GetSelectedControls(&vSelControls);
	if (vSelControls.empty())
	{
		return;
	}

	HTREEITEM hSel = m_treeMessages.GetSelectedItem();
	if (hSel != NULL)
	{
		CStringA strCode;

		strCode = "In *.h file:\r\n";
		strCode += "\r\n";

		int nSelControlCount = vSelControls.size();

		// CSkinButton *m_pBtn1;
		for (int i = 0; i < nSelControlCount; ++i)
		{
			CControlWnd *pCtrlWnd = vSelControls[i];
			CTRLPROPERTIES *pCtrlProp = pCtrlWnd->GetControlProperties();

			CStringA strVar;
			GenerateVariableDeclare(pCtrlProp, strVar);
			strCode += strVar;
		}


		// BOOL m_bFlag;
		for (int i = 0; i < nSelControlCount; ++i)
		{
			CControlWnd *pCtrlWnd = vSelControls[i];
			CTRLPROPERTIES *pCtrlProp = pCtrlWnd->GetControlProperties();
			CONTROL_TYPE eControlType = pCtrlProp->m_eControlType;
			if (eControlType == CT_CHECK)
			{
				CStringA strDoData;
				int nRet = GenerateDoDataExchangeVariableDeclare(pCtrlProp, strDoData);
				if (nRet == 0)
				{
					strCode += "\r\n";
					strCode += strDoData;
				}
			}
		}

		// afx_msg void OnBtnXXX();
		for (int i = 0; i < nSelControlCount; ++i)
		{
			CControlWnd *pCtrlWnd = vSelControls[i];
			CTRLPROPERTIES *pCtrlProp = pCtrlWnd->GetControlProperties();
			CONTROL_TYPE eControlType = pCtrlProp->m_eControlType;
			if (eControlType == CT_PUSHBUTTON)
			{
				CStringA strHandler;
				int nRet = GenerateHandlerDeclareString(pCtrlProp, strHandler);
				if (nRet == 0)
				{
					strCode += "\r\n";
					strCode += strHandler;
				}
			}
		}

		strCode += "\r\n\r\nIn *.cpp file\r\n";

		// m_pBtn1 = NULL;
		for (int i = 0; i < nSelControlCount; ++i)
		{
			CControlWnd *pCtrlWnd = vSelControls[i];
			CTRLPROPERTIES *pCtrlProp = pCtrlWnd->GetControlProperties();

			CStringA strNULL;
			GenerateVariableInit(pCtrlProp, strNULL);
			strCode += strNULL;
		}

		// ON_BN_CLICKED(IDC_BTN_1, OnBtn1)
		for (int i = 0; i < nSelControlCount; ++i)
		{
			CControlWnd *pCtrlWnd = vSelControls[i];
			CTRLPROPERTIES *pCtrlProp = pCtrlWnd->GetControlProperties();
			CONTROL_TYPE eControlType = pCtrlProp->m_eControlType;
			if (eControlType == CT_PUSHBUTTON)
			{
				CStringA strHandleMsgMap;
				int nRet = GenerateButtonHandlerMsgMap(pCtrlProp, strHandleMsgMap);
				if (nRet == 0)
				{
					strCode += "\r\n";
					strCode += strHandleMsgMap;
				}
			}
		}

		// m_pBtn1 = (CSkinButton *)GetDlgItem(IDC_BTN_1);
		// m_pRc1 = GetRectChild(IDC_WL_RECT_1);
		strCode += "\r\n";
		for (int i = 0; i < nSelControlCount; ++i)
		{
			CControlWnd *pCtrlWnd = vSelControls[i];
			CTRLPROPERTIES *pCtrlProp = pCtrlWnd->GetControlProperties();

			CStringA strVariable;
			GenerateVariableAssignString(pCtrlProp, strVariable, FALSE);
			strCode += strVariable;
		}

		// void CYourWnd::OnBtn1() { AfxMessage(_T("ID"); }
		for (int i = 0; i < nSelControlCount; ++i)
		{
			CControlWnd *pCtrlWnd = vSelControls[i];
			CTRLPROPERTIES *pCtrlProp = pCtrlWnd->GetControlProperties();
			CONTROL_TYPE eControlType = pCtrlProp->m_eControlType;
			if (eControlType == CT_PUSHBUTTON)
			{
				CString strFile = CUIMgr::GetUIPathWithoutTitle() + _T("Settings.ini");
				TCHAR szClassName[MAX_PATH] = {0};
				GetPrivateProfileString(pCtrlProp->m_strID, g_lpszClassName, _T("YourWnd"), szClassName, MAX_PATH, strFile);
				CStringA strClassName = (LPCSTR)_bstr_t(szClassName);

				CStringA strHandle;
				int nRet = GenerateHandlerImplementation(pCtrlProp, strClassName, strHandle);
				if (nRet == 0)
				{
					strCode += "\r\n";
					strCode += strHandle;
				}
			}
		}

		SetWindowTextA(m_reCode.GetSafeHwnd(), strCode);
	}

	*pResult = 0;
}
