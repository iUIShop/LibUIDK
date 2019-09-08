// IUIDocument.cpp : implementation of the CIUIDocument class
//

#include "stdafx.h"
#if(_MSC_VER <= 1200) // VC6.0
#include <../src/afximpl.h>
#else
#include <../src/mfc/afximpl.h>
#endif
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIUIDocument

IMPLEMENT_DYNCREATE(CIUIDocument, CDocument)

BEGIN_MESSAGE_MAP(CIUIDocument, CDocument)
	//{{AFX_MSG_MAP(CIUIDocument)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIUIDocument construction/destruction

CIUIDocument::CIUIDocument()
{
}

CIUIDocument::~CIUIDocument()
{
}

/////////////////////////////////////////////////////////////////////////////
// CIUIDocument diagnostics

#ifdef _DEBUG
void CIUIDocument::AssertValid() const
{
	CObject::AssertValid();

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CIUIView *pView = GetNextIUIView(pos);
		ASSERT_VALID(pView);
	}
}

void CIUIDocument::Dump(CDumpContext &dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


//////////////////////////////////////////////////////////////////////////
// virtual

BOOL CIUIDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CIUIDocument::OnCloseDocument()
{
	// destroy all frames viewing this document
	// the last destroy may destroy us
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;  // don't destroy document while closing views
	while (!m_viewList.IsEmpty())
	{
		// get frame attached to the view
		CIUIView *pView = (CIUIView *)m_viewList.GetHead();
		ASSERT_VALID(pView);
		CIUIFrameWnd *pFrame = pView->GetParentFrame();
		ASSERT_VALID(pFrame);

		// and close it
		PreCloseIUIFrame(pFrame);
		pFrame->DestroyWindow();	// delete the CIUIView

		// Remove the view from view list
		m_viewList.RemoveAt(m_viewList.Find(pView, NULL));
	}
	m_bAutoDelete = bAutoDelete;

	// clean up contents of document before destroying the document itself
	DeleteContents();

	// delete the document if necessary
	if (m_bAutoDelete)
	{
		delete this;    // in CDocument::~CDocument,
	}
}

void CIUIDocument::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

BOOL CIUIDocument::CanCloseIUIFrame(CIUIFrameWnd *pIUIFrame)
{
	ASSERT_VALID(pIUIFrame);
	UNUSED(pIUIFrame);   // unused in release builds

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CIUIView *pView = GetNextIUIView(pos);
		ASSERT_VALID(pView);
		CIUIFrameWnd *pFrame = pView->GetParentFrame();
		// assume frameless views are ok to close
		if (pFrame != NULL)
		{
			// assumes 1 document per frame
			ASSERT_VALID(pFrame);
			if (pFrame->m_nWindow > 0)
			{
				return TRUE;    // more than one frame refering to us
			}
		}
	}

	// otherwise only one frame that we know about
	return SaveModified();
}

void CIUIDocument::PreCloseIUIFrame(CIUIFrameWnd *pIUIFrame)
{
	// default does nothing
}

void CIUIDocument::UpdateFrameCounts()
// assumes 1 doc per frame
{
	// walk all frames of views (mark and sweep approach)
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CIUIView *pView = (CIUIView *)GetNextIUIView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		if (pView->IsWindowVisible())   // Do not count invisible windows.
		{
			CIUIMDIFrameWnd *pFrame = (CIUIMDIFrameWnd *)pView->GetParent();
			if (pFrame != NULL)
			{
				pFrame->m_nWindow = -1;    // unknown
			}
		}
	}

	// now do it again counting the unique ones
	int nFrames = 0;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CIUIView *pView = GetNextIUIView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		if (pView->IsWindowVisible())   // Do not count invisible windows.
		{
			CIUIFrameWnd *pFrame = pView->GetParentFrame();
			if (pFrame != NULL && pFrame->m_nWindow == -1)
			{
				ASSERT_VALID(pFrame);
				// not yet counted (give it a 1 based number)
				pFrame->m_nWindow = ++nFrames;
			}
		}
	}

	// lastly walk the frames and update titles (assume same order)
	// go through frames updating the appropriate one
	int iFrame = 1;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CIUIView *pView = GetNextIUIView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		if (pView->IsWindowVisible())   // Do not count invisible windows.
		{
			CIUIFrameWnd *pFrame = pView->GetParentFrame();
			if (pFrame != NULL && pFrame->m_nWindow == iFrame)
			{
				ASSERT_VALID(pFrame);
				if (nFrames == 1)
				{
					pFrame->m_nWindow = 0;    // the only one of its kind
				}
				pFrame->OnUpdateFrameTitle(TRUE);
				iFrame++;
			}
		}
	}
	ASSERT(iFrame == nFrames + 1);
}

CIUIView *CIUIDocument::GetNextIUIView(POSITION &rPosition) const
{
	ASSERT(rPosition != BEFORE_START_POSITION);
	// use CDocument::GetFirstViewPosition instead !
	if (rPosition == NULL)
	{
		return NULL;    // nothing left
	}
	CIUIView *pView = (CIUIView *)m_viewList.GetNext(rPosition);
	ASSERT_KINDOF(CIUIView, pView);
	return pView;
}

//////////////////////////////////////////////////////////////////////////
// public

void CIUIDocument::AddView(CIUIView *pView)
{
	ASSERT_VALID(pView);
	ASSERT(pView->m_pDocument == NULL); // must not be already attached
	ASSERT(m_viewList.Find(pView, NULL) == NULL);   // must not be in list

	m_viewList.AddTail(pView);
	ASSERT(pView->m_pDocument == NULL); // must be un-attached
	pView->m_pDocument = this;

	OnChangedViewList();    // must be the last thing done to the document
}


//////////////////////////////////////////////////////////////////////////
// CIUIDocument2

CAction::CAction()
{

}

CAction::CAction(const CAction &action)
{
	if (&action == this)
	{
		return;
	}

	m_eAction = action.m_eAction;
	m_lParam = action.m_lParam;
}

CAction::~CAction()
{

}

CAction &CAction::operator = (const CAction &action)
{
	if (this == &action)
	{
		return *this;
	}

	m_eAction = action.m_eAction;
	m_lParam = action.m_lParam;

	return *this;
}

//
CRecycleBin::CRecycleBin()
{

}

CRecycleBin::~CRecycleBin()
{

}

CIUIDocument2::CIUIDocument2()
{
	m_nMaxUndo = 10000;
}

CIUIDocument2::~CIUIDocument2()
{

}

int CIUIDocument2::ReleseRedoList()
{
	// 由于同一个指针可能即在Undo List中，又在Redo List中，所以当在Undo List中释放后，再在Redo List中释放，会Crash，
	// 所以引入“回收站”概念，当删除具体对象时，放入到回收站。

	m_Redo.RemoveAll();

	return 0;
}

const CString &CIUIDocument2::GetTitle() const
{
	return m_strTitle;
}

void CIUIDocument2::SetTitle(LPCTSTR lpszTitle)
{
	m_strTitle = lpszTitle;
}

int CIUIDocument2::SetDefaultTitle()
{
	CString strDocName;
	BOOL bRet = AfxExtractSubString(strDocName, m_strDocStrings, (int)CDocTemplate::docName);
	if (!bRet || strDocName.IsEmpty())
	{
		// use generic 'untitled'
		VERIFY(strDocName.LoadString(AFX_IDS_UNTITLED));
	}

	SetTitle(strDocName);

	return 0;
}

const CString &CIUIDocument2::GetPathName() const
{
	ASSERT(this != NULL);
	return m_strPathName;
}

void CIUIDocument2::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU/* = TRUE*/)
{
	// store the path fully qualified
	TCHAR szFullPath[_MAX_PATH];
	AfxFullPath(szFullPath, lpszPathName);
	m_strPathName = szFullPath;
	ASSERT(!m_strPathName.IsEmpty());       // must be set to something
	ASSERT(this != NULL);

	// set the document title based on path name
	TCHAR szTitle[_MAX_FNAME];
	if (AfxGetFileTitle(szFullPath, szTitle, _MAX_FNAME) == 0)
	{
		SetTitle(szTitle);
	}

	// add it to the file MRU list
	if (bAddToMRU)
	{
		AfxGetApp()->AddToRecentFileList(m_strPathName);
	}
}

void CIUIDocument2::SetResourceID(UINT nID)
{
	m_nIDResource = nID;

	if (m_strDocStrings.IsEmpty() && !m_strDocStrings.LoadString(m_nIDResource))
	{
		TRACE1("Warning: no document names in string for template #%d.\n",
			m_nIDResource);
	}

	SetDefaultTitle();
}

UINT CIUIDocument2::GetResourceID()
{
	return m_nIDResource;
}

BOOL CIUIDocument2::CanCloseFrame(CUIWnd *pFrame)
{
	return TRUE;
}

BOOL CIUIDocument2::SaveModified()
{
	if (!IsModified())
	{
		return TRUE;    // ok to continue
	}

	// get name/title of document
	CString name;
	if (m_strPathName.IsEmpty())
	{
		// get name based on caption
		name = m_strTitle;
		if (name.IsEmpty())
		{
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));
		}
	}
	else
	{
		// get name based on file title of path name
		name = m_strPathName;
		AfxGetFileTitle(m_strPathName, name.GetBuffer(_MAX_PATH), _MAX_PATH);
		name.ReleaseBuffer();
	}

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		if (!DoFileSave())
		{
			return FALSE;    // don't continue
		}
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}

BOOL CIUIDocument2::DoFileSave()
{
	DWORD dwAttrib = GetFileAttributes(m_strPathName);
	if (dwAttrib & FILE_ATTRIBUTE_READONLY)
	{
		// we do not have read-write access or the file does not (now) exist
		if (!DoSave(NULL))
		{
			TRACE(_T("Warning: File save with new name failed.\n"));
			return FALSE;
		}
	}
	else
	{
		if (!DoSave(m_strPathName))
		{
			TRACE(_T("Warning: File save failed.\n"));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CIUIDocument2::DoSave(LPCTSTR lpszPathName, BOOL bReplace/* = TRUE*/)
// Save the document data to a file
// lpszPathName = path name where to save document file
// if lpszPathName is NULL then the user will be prompted (SaveAs)
// note: lpszPathName can be different than 'm_strPathName'
// if 'bReplace' is TRUE will change file name if successful (SaveAs)
// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
			{
				newName.ReleaseBuffer(iBad);
			}

			// append the default suffix if there is one
			CString strExt;

			BOOL bRet = AfxExtractSubString(strExt, m_strDocStrings, (int)CDocTemplate::filterExt);
			if (bRet /*pTemplate->GetDocString(strExt, CDocTemplate::filterExt)*/ &&
				!strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		if (!DoPromptFileName(newName,
				bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
				OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, NULL))
		{
			return FALSE;    // don't even attempt to save
		}
	}

	if (!OnSaveDocument(newName))
	{
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
	{
		SetPathName(newName);
		AfxGetMainWnd()->SetWindowText(newName);
	}

	return TRUE;        // success
}

BOOL CIUIDocument2::OnOpenDocument(LPCTSTR lpszPathName)
{
#ifdef _DEBUG
	if (IsModified())
	{
		TRACE(_T("Warning: OnOpenDocument replaces an unsaved document.\n"));
	}
#endif

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}

BOOL CIUIDocument2::OnSaveDocument(LPCTSTR lpszPathName)
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_SAVE, CN_COMMAND));

	return TRUE;
}

AFX_STATIC void AFXAPI _AfxAppendFilterSuffix(CString &filter, CString strDocStrings, OPENFILENAME &ofn, CString *pstrDefaultExt)
{
	CString strFilterExt, strFilterName;
	BOOL bRet = AfxExtractSubString(strFilterExt, strDocStrings, (int)CDocTemplate::filterExt);
	BOOL bRet2 = AfxExtractSubString(strFilterName, strDocStrings, (int)CDocTemplate::filterName);
	if (bRet && !strFilterExt.IsEmpty() && bRet2 && !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

BOOL CIUIDocument2::DoPromptFileName(CString &fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate *pTemplate)
{
	CFileDialog dlgFile(bOpenFileDialog);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	_AfxAppendFilterSuffix(strFilter, m_strDocStrings, dlgFile.m_ofn, &strDefault);

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

	UINT_PTR nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}

int CIUIDocument2::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible/* = TRUE*/)
{
	if (!SaveModified())
	{
		return -1;    // leave the original one
	}

	// open an existing document
	BOOL bWasModified = IsModified();
	SetModifiedFlag(FALSE);  // not dirty for open

	if (!OnOpenDocument(lpszPathName))
	{
		// user has been alerted to what failed in OnOpenDocument
		TRACE0("CIUIDocument2::OnOpenDocument returned FALSE.\n");
		return -1;        // open failed
	}
	SetPathName(lpszPathName);

	// send initial update to all views (and other controls) in the frame
	AfxGetMainWnd()->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

	return 0;
}

int CIUIDocument2::OnFileNew()
{
	if (!SaveModified())
	{
		return -1;    // leave the original one
	}

	if (!OnNewDocument())
	{
		return -2;
	}

	SetModifiedFlag(FALSE);

	return 0;
}

int CIUIDocument2::OnFileOpen()
{
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
			OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
	{
		return -1;    // open cancelled
	}

	OpenDocumentFile(newName);

	return 0;
}

int CIUIDocument2::OnFileSave()		// ID_FILE_SAVE handler
{
	BOOL bRet = DoFileSave();

	return bRet ? 0 : -1;
}

int CIUIDocument2::OnFileSaveAs()
{
	if (!DoSave(NULL))
	{
		TRACE0("Warning: File save-as failed.\n");

		return -1;
	}

	return 0;
}

//
int CIUIDocument2::AddView(CWnd *pView)
{
	m_vpViews.push_back(pView);

	return 0;
}

int CIUIDocument2::UpdateAllViews(CWnd *pSender)
{
	int nViewCount = (int)m_vpViews.size();
	for (int i = 0; i < nViewCount; ++i)
	{
		if (m_vpViews[i] != pSender)
		{
			m_vpViews[i]->SendMessage(WM_UPDATEVIEW, (WPARAM)pSender, (LPARAM)this);
		}
	}

	return 0;
}

// Undo and redo
int CIUIDocument2::AddUndo(const CAction *pAction)
{
	ASSERT(pAction->m_eAction != CAction::actionUnknown);

	CAction action = *pAction;
	m_Undo.AddHead(action);
	while (m_Undo.GetCount() > m_nMaxUndo)
	{
		m_Undo.RemoveTail();
	}

	//
	// 释放Redo列表中的内存，并清空Redo列表。? 清空Redo列表，但不一定要释放内存，因为有一些节点的内存，还可能在使用。
	// 例如：创建音符，删除音符，Redo恢复音符，再删除音符，如果这时释放Redo列表中的内存，那么Note节点被释放后，就无法恢复了。
	ReleseRedoList();

	return 0;
}

int CIUIDocument2::Undo()
{
	CAction action = m_Undo.RemoveHead();

	// Add to redo list
	m_Redo.AddHead(action);

	while (m_Redo.GetCount() > m_nMaxUndo)
	{
		m_Redo.RemoveTail();
	}

	SetModifiedFlag(TRUE);

	return 0;
}

BOOL CIUIDocument2::CanUndo()
{
	return (m_Undo.GetCount() > 0);
}

int CIUIDocument2::Redo()
{
	CAction action = m_Redo.RemoveHead();

	// Add to undo list
	m_Undo.AddHead(action);

	while (m_Undo.GetCount() > m_nMaxUndo)
	{
		m_Undo.RemoveTail();
	}

	SetModifiedFlag(TRUE);

	return 0;
}

BOOL CIUIDocument2::CanRedo()
{
	return (m_Redo.GetCount() > 0);
}

// Modified
BOOL CIUIDocument2::IsModified()
{
	ASSERT(this != NULL);
	return m_bModified;
}

void CIUIDocument2::SetModifiedFlag(BOOL bModified/* = TRUE*/)
{
	ASSERT(this != NULL);
	m_bModified = bModified;
}

BOOL CIUIDocument2::OnNewDocument()
{
#ifdef _DEBUG
	if (IsModified())
	{
		TRACE(_T("Warning: OnNewDocument replaces an unsaved document.\n"));
	}
#endif

	DeleteContents();
	m_strPathName.Empty();      // no path name yet
	SetModifiedFlag(FALSE);     // make clean

	return TRUE;
}

void CIUIDocument2::DeleteContents() // delete doc items etc
{

}
