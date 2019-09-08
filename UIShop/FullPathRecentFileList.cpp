// FullPathRecentFileList.cpp: implementation of the CFullPathRecentFileList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FullPathRecentFileList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFullPathRecentFileList::CFullPathRecentFileList(UINT nStart, LPCTSTR lpszSection,
		LPCTSTR lpszEntryFormat, int nSize,
		int nMaxDispLen/* = AFX_ABBREV_FILENAME_LEN*/) : CRecentFileList(nStart, lpszSection, lpszEntryFormat, nSize, nMaxDispLen)
{

}

CFullPathRecentFileList::~CFullPathRecentFileList()
{

}

// copy from CRecentFileList::UpdateMenu in file <SRC/FILELIST.CPP>
void CFullPathRecentFileList::UpdateMenu(CCmdUI* pCmdUI)
{
	ASSERT(m_arrNames != NULL);

	CMenu* pMenu = pCmdUI->m_pMenu;
	if (m_strOriginal.IsEmpty() && pMenu != NULL)
		pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);

	if (m_arrNames[0].IsEmpty())
	{
		// no MRU files
		if (!m_strOriginal.IsEmpty())
			pCmdUI->SetText(m_strOriginal);
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pCmdUI->m_pMenu == NULL)
		return;

	int iMRU = 0;
	for (; iMRU < m_nSize; iMRU++)
		pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

	TCHAR szCurDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, szCurDir);
	int nCurDir = lstrlen(szCurDir);
	ASSERT(nCurDir >= 0);
	szCurDir[nCurDir] = '\\';
	szCurDir[++nCurDir] = '\0';

	CString strName;
	CString strTemp;
	for (iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		if (m_arrNames[iMRU].IsEmpty())
			break;

		if (!GetDisplayName2(strName, iMRU, szCurDir, nCurDir))
			break;

		// double up any '&' characters so they are not underlined
		LPCTSTR lpszSrc = strName;
		LPTSTR lpszDest = strTemp.GetBuffer(strName.GetLength()*2);
		while (*lpszSrc != 0)
		{
			if (*lpszSrc == '&')
				*lpszDest++ = '&';
			if (_istlead(*lpszSrc))
				*lpszDest++ = *lpszSrc++;
			*lpszDest++ = *lpszSrc++;
		}
		*lpszDest = 0;
		strTemp.ReleaseBuffer();

		// insert mnemonic + the file name
		TCHAR buf[10];
		wsprintf(buf, _T("&%d "), (iMRU+1+m_nStart) % 10);
		pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
			MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++,
			CString(buf) + strTemp);
	}

	// update end menu count
	pCmdUI->m_nIndex--; // point to last menu added
	pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

	pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
}

// copy from CRecentFileList::GetDisplayName in file <SRC/FILELIST.CPP>
BOOL CFullPathRecentFileList::GetDisplayName2(CString& strName, int nIndex, LPCTSTR lpszCurDir, int nCurDir, BOOL bAtLeastName/* = TRUE*/) const
{
	ASSERT(lpszCurDir == NULL || AfxIsValidString(lpszCurDir, nCurDir));

	ASSERT(m_arrNames != NULL);
	ASSERT(nIndex < m_nSize);
	if (m_arrNames[nIndex].IsEmpty())
		return FALSE;

	LPTSTR lpch = strName.GetBuffer(_MAX_PATH);
	lstrcpy(lpch, m_arrNames[nIndex]);
	strName.ReleaseBuffer();
	return TRUE;
}
