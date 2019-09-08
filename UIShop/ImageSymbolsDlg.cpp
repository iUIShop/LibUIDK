// ImageSymbolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "ImageSymbolsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CImageSymbolsDlg dialog

IMPLEMENT_DYNAMIC(CImageSymbolsDlg, CDialog)

CImageSymbolsDlg::CImageSymbolsDlg(IMAGE_SYMBOLS_DLG_MODE eMode, CWnd* pParent /*=NULL*/)
	: CDialog(CImageSymbolsDlg::IDD, pParent)
{
	m_eMode = eMode;
}

CImageSymbolsDlg::~CImageSymbolsDlg()
{
}

void CImageSymbolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_IMAGE_SYMBOLS, m_treeImageSymbols);
	DDX_Control(pDX, IDC_BTN_MOVE_TO_UNUSED, m_btnMoveToUnused);
}


BEGIN_MESSAGE_MAP(CImageSymbolsDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_MOVE_TO_UNUSED, &CImageSymbolsDlg::OnBnClickedBtnMoveToUnused)
END_MESSAGE_MAP()


// CImageSymbolsDlg message handlers

BOOL CImageSymbolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_vFiles.clear();

	if (m_eMode == ISDM_UNUSED)
	{
		//
		SetWindowText(_T("Files not used"));

		//
		CString strXUIPath = CUIMgr::GetUIPath();
		CString strXUIFile = GetFileName(strXUIPath);

		CFileFind finder;
		BOOL bWorking = finder.FindFile(CUIMgr::GetUIPathWithoutTitle() + _T("*.*"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			// skip . and .. files; otherwise, we'd
			// recur infinitely!
			if (finder.IsDots())
				continue;

			if (finder.IsDirectory())
				continue;

			CString strFile = finder.GetFileName();

			if (strFile.Compare(_T("Resource.h")) == 0)
				continue;
			if (strFile.Compare(strXUIFile) == 0)
				continue;
			if (strFile.Compare(_T("Settings.ini")) == 0)
				continue;

			CTRLPROPERTIES *pCtrlPorp = CUIMgr::GetCONTROL(strFile);
			if (pCtrlPorp == NULL)
			{
				m_treeImageSymbols.InsertItem(strFile);

				m_vFiles.push_back(strFile);
			}
		}
	}
	else if (m_eMode == ISDM_UNFOUND)
	{
		//
		SetWindowText(_T("Images not found"));

		//
		std::vector<CString> vImages = CUIMgr::GetAllImages(FALSE);

		// Delete duplicate elements.
		std::vector<CString> vImageFiles;

		int nImageCount = vImages.size();
		for (int i = 0; i < nImageCount; ++i)
		{
			CString strImage = vImages[i];

			if (PathFileExists(CUIMgr::GetUIPathWithoutTitle() + strImage))
				continue;

			m_treeImageSymbols.InsertItem(strImage);

			m_vFiles.push_back(strImage);
		}

		m_btnMoveToUnused.ShowWindow(SW_HIDE);
	}
	else
	{
		ASSERT(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImageSymbolsDlg::OnBnClickedBtnMoveToUnused()
{
	if (m_eMode != ISDM_UNUSED)
	{
		ASSERT(FALSE);
		return;
	}

	GUID guid;
	HRESULT hr = CoCreateGuid(&guid);
	if (FAILED(hr))
		return;

	// Generate guid directory
	wchar_t wszName[64] = {0};
	StringFromGUID2(guid, wszName, sizeof(wszName));

	// Create 'Unused' folder
	CString strUnusedFolder = (LPCTSTR)_bstr_t(wszName);
	CString strUnused = CUIMgr::GetUIPathWithoutTitle() + strUnusedFolder;
	if (!PathFileExists(strUnused))
	{
		CreateDirectory(strUnused, NULL);
	}

	// Move unused files to 'Unused' folder
	int nCount = m_vFiles.size();
	for (int i = 0; i < nCount; ++i)
	{
		CString strFile = m_vFiles[i];

		if (strFile.CompareNoCase(strUnusedFolder) == 0)
			continue;

		CString strTarget = strUnused + _T("\\") + strFile;
		MoveFile(CUIMgr::GetUIPathWithoutTitle() + strFile, strTarget);
	}
}
