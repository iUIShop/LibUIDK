// FullPathRecentFileList.h: interface for the CFullPathRecentFileList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FULLPATHRECENTFILELIST_H__09117F9F_7E73_495B_A4F9_A7B4066A2A9A__INCLUDED_)
#define AFX_FULLPATHRECENTFILELIST_H__09117F9F_7E73_495B_A4F9_A7B4066A2A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXADV.H>

class CFullPathRecentFileList : public CRecentFileList  
{
public:
	CFullPathRecentFileList(UINT nStart, LPCTSTR lpszSection,
		LPCTSTR lpszEntryFormat, int nSize,
		int nMaxDispLen = AFX_ABBREV_FILENAME_LEN);
	virtual ~CFullPathRecentFileList();

public:
	BOOL GetDisplayName2(CString& strName, int nIndex,
		LPCTSTR lpszCurDir, int nCurDir, BOOL bAtLeastName = TRUE) const;
	virtual void UpdateMenu(CCmdUI* pCmdUI);
};

#endif // !defined(AFX_FULLPATHRECENTFILELIST_H__09117F9F_7E73_495B_A4F9_A7B4066A2A9A__INCLUDED_)
