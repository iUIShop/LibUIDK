// GenerateResourceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uishop.h"
#include "GenerateResourceDlg.h"
#include "global.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGenerateResourceDlg dialog


CGenerateResourceDlg::CGenerateResourceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGenerateResourceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenerateResourceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nLanguage = 0;
	m_bRelateSavePath = TRUE;
}


void CGenerateResourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenerateResourceDlg)
	DDX_Control(pDX, IDC_EDT_SKIN_PATH, m_edtSkinPath);
	DDX_Control(pDX, IDC_EDT_SAVE_PATH, m_edtSavePath);
	DDX_Text(pDX, IDC_EDT_SAVE_PATH, m_strSavePath);
	DDX_Control(pDX, IDC_CMB_LANGUAGE, m_cmbLanguage);
	DDX_Control(pDX, IDC_RAD_RELATE_SAVE_PATH, m_radRelateSavePath);
	DDX_Control(pDX, IDC_RAD_RElATE_CUSTOM, m_radRelateCustom);
	DDX_Control(pDX, IDC_EDT_RELATE_CUSTOM, m_edtRelateCustom);
	DDX_Text(pDX, IDC_EDT_RELATE_BASE_ON_SAVE_PATH, m_strRelateBaseOnSavePath);
	DDX_Text(pDX, IDC_EDT_RELATE_CUSTOM, m_strRelateCustom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenerateResourceDlg, CDialog)
	//{{AFX_MSG_MAP(CGenerateResourceDlg)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_RAD_RELATE_SAVE_PATH, OnRadRelateSavePath)
	ON_BN_CLICKED(IDC_RAD_RElATE_CUSTOM, OnRadRelateCustom)
	ON_EN_CHANGE(IDC_EDT_SAVE_PATH, &CGenerateResourceDlg::OnEnChangeEdtSavePath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// proptected

int CGenerateResourceDlg::UpdateRelativePath()
{
	if (m_bRelateSavePath)
	{
		m_strRelateBaseOnSavePath = CUIMgr::GetUIPathWithoutTitle();
		CString strRcPath = GetFilePath(m_strSavePath);
		m_strRelateBaseOnSavePath = GetRelativePath(strRcPath, m_strRelateBaseOnSavePath);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CGenerateResourceDlg message handlers

BOOL CGenerateResourceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_edtSkinPath.SetWindowText(CUIMgr::GetUIPathWithoutTitle());

	m_cmbLanguage.SetCurSel(0);
	m_radRelateSavePath.SetCheck(BST_CHECKED);
	OnRadRelateSavePath();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGenerateResourceDlg::OnOK()
{
	// TODO: Add extra validation here
	BOOL bRet = UpdateData(TRUE);
	if (!bRet)
		return;

	if (m_strSavePath.IsEmpty())
	{
		AfxMessageBox(_T("Please input save path!"));
		m_edtSavePath.SetFocus();
		return;
	}

	m_bRelateSavePath = (m_radRelateSavePath.GetCheck() == BST_CHECKED);
	if (!m_bRelateSavePath && m_strRelateCustom.IsEmpty())
	{
		AfxMessageBox(_T("Please input custom path"));
		m_edtRelateCustom.SetFocus();
		return;
	}

	m_nLanguage = m_cmbLanguage.GetCurSel();

	CDialog::OnOK();
}

void CGenerateResourceDlg::OnBtnBrowse()
{
	BOOL bRet = UpdateData(TRUE);
	if (!bRet)
		return;

	TCHAR szFilters[] = _T("Visual Studio Resource Files(*.rc)|*.rc||");
	CFileDialog dlg(FALSE, _T("rc"), _T("LibUIDKSkin.rc"), OFN_OVERWRITEPROMPT, szFilters);
	dlg.m_ofn.lpstrInitialDir = CUIMgr::GetUIPathWithoutTitle();
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
		return;

	m_strSavePath = dlg.GetPathName();
	UpdateRelativePath();

	UpdateData(FALSE);
}

void CGenerateResourceDlg::OnRadRelateSavePath()
{
	m_edtRelateCustom.EnableWindow(FALSE);
}

void CGenerateResourceDlg::OnRadRelateCustom()
{
	m_edtRelateCustom.EnableWindow(TRUE);
}

void CGenerateResourceDlg::OnEnChangeEdtSavePath()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData(TRUE);
	UpdateRelativePath();
	UpdateData(FALSE);
}
