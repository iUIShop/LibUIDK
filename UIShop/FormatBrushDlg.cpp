// FormatBrushDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "FormatBrushDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CMainFrame *g_pFrm;


// CFormatBrushDlg dialog

IMPLEMENT_DYNAMIC(CFormatBrushDlg, CDialog)

CFormatBrushDlg::CFormatBrushDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFormatBrushDlg::IDD, pParent)
{
	m_pProp = NULL;
}

CFormatBrushDlg::~CFormatBrushDlg()
{
}

void CFormatBrushDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FORMAT_BRUSH, m_lstFormatBrush);
}


BEGIN_MESSAGE_MAP(CFormatBrushDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFormatBrushDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFormatBrushDlg message handlers

BOOL CFormatBrushDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	ASSERT(m_pProp != NULL);
	m_lstFormatBrush.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_lstFormatBrush.InsertColumn(0, _T("Property Name"), LVCFMT_LEFT, 200);

	LONG lVarCount = m_pProp->GetPropertyCount();
	int nIndex = 0;
	for (LONG i = 0; i < lVarCount ; i++)
	{
		const IUI_VARIANT *pVariant = m_pProp->GetProperty(i);
		if (pVariant->vtEx == VT_EX_STRUCT_SIZE || pVariant->vtEx == VT_EX_CONTROL_ID_MACRO || pVariant->vtEx == VT_EX_CONTROL_ID
			|| pVariant->vtEx == VT_EX_RECT_WH || pVariant->vtEx == VT_EX_RECT || pVariant->vtEx == VT_EX_CONTROL_TYPE || !pVariant->bSaveToSkinFile)
			continue;

		int nItem = m_lstFormatBrush.InsertItem(nIndex++, pVariant->strPropWndName);
		m_lstFormatBrush.SetItemData(nItem, i);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CFormatBrushDlg::SetCtrlProp(const CTRLPROPERTIES *pProp)
{
	m_pProp = pProp;

	return 0;
}

void CFormatBrushDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	int nCount = m_lstFormatBrush.GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		BOOL bCheck = m_lstFormatBrush.GetCheck(i);

		if (bCheck)
		{
			int nIndexInProp = m_lstFormatBrush.GetItemData(i);
			g_pFrm->AddFormatBrushPropIndex(nIndexInProp);
		}
	}

	OnOK();
}
