// PromptDocManager.h: interface for the CPromptDocManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROMPTDOCMANAGER_H__E3084494_AB65_449B_8F4F_77FB5E22A18F__INCLUDED_)
#define AFX_PROMPTDOCMANAGER_H__E3084494_AB65_449B_8F4F_77FB5E22A18F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////////
// Use a modified file open dialog for prompt open or save project file.

class CPromptDocManager : public CDocManager  
{
public:
	CPromptDocManager();
	virtual ~CPromptDocManager();

public:
	virtual void OnFileOpen();
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);

public:
	void SetProjectFilterExt(const CString &strExt);
	CString GetProjectFilterExt() const;

	void SetProjectFilterName(const CString &strName);
	CString GetProjectFilterName() const;

protected:
	CString m_strProjectFilterExt;
	CString m_strProjectFilterName;
	BOOL m_bOpenProject;
};

#endif // !defined(AFX_PROMPTDOCMANAGER_H__E3084494_AB65_449B_8F4F_77FB5E22A18F__INCLUDED_)
