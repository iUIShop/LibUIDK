// PromptDocManager.cpp: implementation of the CPromptDocManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PromptDocManager.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPromptDocManager::CPromptDocManager()
{
	m_bOpenProject = FALSE;
}

CPromptDocManager::~CPromptDocManager()
{

}

void CPromptDocManager::OnFileOpen()
{
	m_bOpenProject = TRUE;

	// prompt the user (with all document templates)
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled

	AfxGetApp()->OpenDocumentFile(newName);
		// if returns NULL, the user has already been alerted

	m_bOpenProject = FALSE;
}

BOOL CPromptDocManager::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	CFileDialog dlgFile(bOpenFileDialog);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		IUI_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		_AfxAppendFilterSuffix2(m_strProjectFilterExt, m_strProjectFilterName, strFilter, dlgFile.m_ofn, &strDefault);
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	INT_PTR nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}

void CPromptDocManager::SetProjectFilterExt(const CString &strExt)
{
	m_strProjectFilterExt = strExt;
}

CString CPromptDocManager::GetProjectFilterExt() const
{
	return m_strProjectFilterExt;
}

void CPromptDocManager::SetProjectFilterName(const CString &strName)
{
	m_strProjectFilterName = strName;
}

CString CPromptDocManager::GetProjectFilterName() const
{
	return m_strProjectFilterName;
}
