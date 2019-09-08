// SyncSkinTabOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "SyncSkinTabOrderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSyncSkinTabOrderDlg dialog

IMPLEMENT_DYNAMIC(CSyncSkinTabOrderDlg, CDialog)

CSyncSkinTabOrderDlg::CSyncSkinTabOrderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSyncSkinTabOrderDlg::IDD, pParent)
	, m_strSkinSource(_T(""))
	, m_strSkinTarget(_T(""))
{
#ifdef _DEBUG
	m_strSkinSource = _T("C:\\Users\\Administrator\\Desktop\\SpermUI.固定分辨率\\the.xui");
	m_strSkinTarget = _T("C:\\Users\\Administrator\\Desktop\\SpermUI.多分辨率\\the.xui");
#endif // _DEBUG
}

CSyncSkinTabOrderDlg::~CSyncSkinTabOrderDlg()
{
}

void CSyncSkinTabOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_SKIN_SOURCE, m_strSkinSource);
	DDX_Text(pDX, IDC_EDT_SKIN_TARGET, m_strSkinTarget);
}


BEGIN_MESSAGE_MAP(CSyncSkinTabOrderDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSE_SOURCE, &CSyncSkinTabOrderDlg::OnBnClickedBtnBrowseSource)
	ON_BN_CLICKED(IDC_BTN_BROWSE_TARGET, &CSyncSkinTabOrderDlg::OnBnClickedBtnBrowseTarget)
	ON_BN_CLICKED(IDOK, &CSyncSkinTabOrderDlg::OnBnClickedOk)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

class CSyncWindow
{
public:
	CSyncWindow(){};
	~CSyncWindow(){};

public:
	CString m_strWindowName;
	std::vector<CString> m_vControls;
};

int CSyncSkinTabOrderDlg::SyncSkinTabOrder(const CString &strSkinSource, const CString &strSkinTarget)
{
	CUIMgr::SetUIPath(strSkinSource);

	// Parse windows tree
	std::vector<UIWNDPROPERTIES *> *pvSourceWindowList = CUIMgr::GetUIWndPropertiesList();

	std::vector<CSyncWindow> vSourceWindows;
	int nSourceWindowCount = pvSourceWindowList->size();
	for (int i = 0; i < nSourceWindowCount; ++i)
	{
		UIWNDPROPERTIES *pWindow = (*pvSourceWindowList)[i];

		CSyncWindow syncWin;
		syncWin.m_strWindowName = (LPCTSTR)pWindow->m_strID;

		int nChildCount = pWindow->m_vControls.size();
		for (int j = 0; j < nChildCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pWindow->m_vControls[j];

			syncWin.m_vControls.push_back((LPCTSTR)pCtrlProp->m_strID);
		}

		vSourceWindows.push_back(syncWin);
	}
	ASSERT(nSourceWindowCount == vSourceWindows.size());

	CUIMgr::ReleaseSkin();

	// Generate source skin tree
	CString strSourceTree = GetFilePath(strSkinSource);
	strSourceTree += _T("WindowTree.txt");
	FILE *fpSource = TFOPEN(strSourceTree.GetBuffer(0), _T("wb"));

	int nSourceCount = vSourceWindows.size();
	for (int i = 0; i < nSourceCount; ++i)
	{
		CStringA strWindowID = _bstr_t(vSourceWindows[i].m_strWindowName.GetBuffer(0));
		fputs(strWindowID, fpSource);
		fputs("\r\n", fpSource);

		int nChildCount = vSourceWindows[i].m_vControls.size();
		for (int j = 0; j < nChildCount; ++j)
		{
			CStringA strCtrlID = _bstr_t(vSourceWindows[i].m_vControls[j].GetBuffer(0));

			fputs("	", fpSource);
			fputs(strCtrlID, fpSource);
			fputs("\r\n", fpSource);
		}
	}

	fclose(fpSource);

	//
	std::vector<CString> vErrorList;

	// Load target skin
	CUIMgr::SetUIPath(strSkinTarget);
	std::vector<UIWNDPROPERTIES *> *pvTargetWindowList = CUIMgr::GetUIWndPropertiesList();
	int nTargetWindowCount = pvTargetWindowList->size();
	for (int i = 0; i < nSourceWindowCount; ++i)
	{
		CString strWindowID = vSourceWindows[i].m_strWindowName;

		// Find window from target list
		int nTargetIndex = CUIMgr::GetWindowIndex(strWindowID);
		if (nTargetIndex < 0)
		{
			CString strError;
			strError.Format(_T("Window ID: %s not found in target skin."), strWindowID);
			vErrorList.push_back(strError);

			continue;
		}

		// Adjust the window order
		CUIMgr::SwapWindowItem(i, nTargetIndex);

		// Adjust the controls order
		UIWNDPROPERTIES *pTargetUIWnd = CUIMgr::GetWindowItem(i);
		ASSERT(strWindowID == (LPCTSTR)pTargetUIWnd->m_strID);

		int nChildCount = vSourceWindows[i].m_vControls.size();
		for (int j = 0; j < nChildCount; ++j)
		{
			CString strCtrlID = vSourceWindows[i].m_vControls[j];

			// Adjust the controls order
			int nControlOldIndex = pTargetUIWnd->GetControlIndex(strCtrlID);
			if (nControlOldIndex < 0)
			{
				CString strError;
				strError.Format(_T("Control ID: %s in Window %s not found in target skin."), strCtrlID, strWindowID);
				vErrorList.push_back(strError);

				continue;
			}

			pTargetUIWnd->SwapControlItem(j, nControlOldIndex);
		}
	}

	// Generate target skin tree
	CString strTargetTree = GetFilePath(strSkinTarget);
	strTargetTree += _T("WindowTree.txt");
	FILE *fpTarget = TFOPEN(strTargetTree.GetBuffer(0), _T("wb"));

	int nTargetCount = pvTargetWindowList->size();
	for (int i = 0; i < nTargetCount; ++i)
	{
		UIWNDPROPERTIES *pUIWnd = (*pvTargetWindowList)[i];

		CStringA strWindowID = (LPCSTR)pUIWnd->m_strID;
		fputs(strWindowID, fpTarget);
		fputs("\r\n", fpTarget);

		int nChildCount = pUIWnd->m_vControls.size();
		for (int j = 0; j < nChildCount; ++j)
		{
			CStringA strCtrlID = (LPCSTR)pUIWnd->m_vControls[j]->m_strID;

			fputs("	", fpTarget);
			fputs(strCtrlID, fpTarget);
			fputs("\r\n", fpTarget);
		}
	}

	fclose(fpTarget);

	// Save target skin
	CUIMgr::SaveSkin();
	CUIMgr::ReleaseSkin();

	return 0;
}

// CSyncSkinTabOrderDlg message handlers

void CSyncSkinTabOrderDlg::OnBnClickedBtnBrowseSource()
{
	if (!UpdateData())
		return;

	TCHAR szFilters[] = _T("LibUIDK Skin File(*.xui)|*.xui||");
	CFileDialog dlg(TRUE, _T("xui"), NULL, OFN_OVERWRITEPROMPT, szFilters);
	dlg.m_ofn.lpstrInitialDir = CUIMgr::GetUIPathWithoutTitle();
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
		return;

	m_strSkinSource = dlg.GetPathName();

	UpdateData(FALSE);
}

void CSyncSkinTabOrderDlg::OnBnClickedBtnBrowseTarget()
{
	if (!UpdateData())
		return;

	TCHAR szFilters[] = _T("LibUIDK Skin File(*.xui)|*.xui||");
	CFileDialog dlg(TRUE, _T("xui"), NULL, OFN_OVERWRITEPROMPT, szFilters);
	dlg.m_ofn.lpstrInitialDir = CUIMgr::GetUIPathWithoutTitle();
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
		return;

	m_strSkinTarget = dlg.GetPathName();

	UpdateData(FALSE);
}

void CSyncSkinTabOrderDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	if (!UpdateData())
		return;

	if (m_strSkinSource.IsEmpty())
	{
		AfxMessageBox(_T("Please input source skin path!"));
		return;
	}

	if (m_strSkinTarget.IsEmpty())
	{
		AfxMessageBox(_T("Please input target skin path!"));
		return;
	}

	int nRet = SyncSkinTabOrder(m_strSkinSource, m_strSkinTarget);
	if (nRet == 0)
	{
		AfxMessageBox(_T("Sync OK."));
	}
	else
	{
		AfxMessageBox(_T("Failed!"));
	}

	OnOK();
}
