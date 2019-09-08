// UIShopDoc.cpp : implementation of the CUIShopDoc class
//

#include "stdafx.h"
#include "UIShop.h"

#include "UIShopDoc.h"
#include "MainFrm.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CUIShopDoc

IMPLEMENT_DYNCREATE(CUIShopDoc, CDocument)

BEGIN_MESSAGE_MAP(CUIShopDoc, CDocument)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIShopDoc construction/destruction
extern CMainFrame *g_pFrm;

CUIShopDoc::CUIShopDoc()
{
	// TODO: add one-time construction code here
	m_pTree = NULL;
}

CUIShopDoc::~CUIShopDoc()
{
}

//////////////////////////////////////////////////////////////////////////
// protected

CUIDesignWnd *CUIShopDoc::FindUIDesignWndFromPoint(UIWNDPROPERTIES *pUIWndProp) const
{
	if (pUIWndProp == NULL)
	{
		return NULL;
	}

	CUIDesignWnd *pRet = NULL;

	BOOL bFound = FALSE;

	HTREEITEM hTI = m_pTree->GetRootItem();	// Windows branch
	while (hTI != NULL)
	{
		HTREEITEM hTIChild = m_pTree->GetNextItem(hTI, TVGN_CHILD);

		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_pTree->GetItemData(hTIChild);
		ASSERT(pData != NULL);
		ASSERT(pData->eTreeItemType == WINDOWS_NODE);
		ASSERT(pData->wndUIDesign.GetSafeHwnd() != NULL);

		if (pData->wndUIDesign.GetUIWndProperties() == pUIWndProp)
		{
			pRet = &pData->wndUIDesign;

			break;
		}

		hTI = m_pTree->GetNextItem(hTI, TVGN_NEXT);
	}

	return pRet;
}

int CUIShopDoc::FindObject(const CTRLPROPERTIES *pCtrlProp, CUIDesignWnd **ppUIWnd, CControlWnd **ppCtrlWnd) const
{
	if (ppUIWnd == NULL || ppCtrlWnd == NULL)
	{
		return -2;
	}

	*ppUIWnd = NULL;
	*ppCtrlWnd = NULL;

	BOOL bFound = FALSE;

	HTREEITEM hTI = m_pTree->GetRootItem();	// Windows branch
	while (hTI != NULL)
	{
		HTREEITEM hTIChild = m_pTree->GetNextItem(hTI, TVGN_CHILD);

		// Check children
		while (hTIChild != NULL)
		{
			TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_pTree->GetItemData(hTIChild);
			ASSERT(pData != NULL);

			if (pData->eTreeItemType == WINDOWS_NODE)
			{
				ASSERT(pData->wndUIDesign.GetSafeHwnd() != NULL);

				if (pData->wndUIDesign.GetUIWndProperties() == pCtrlProp)
				{
					bFound = TRUE;

					*ppUIWnd = &pData->wndUIDesign;
					*ppCtrlWnd = NULL;

					break;
				}

				// Loop the CUIDesignWnd
				size_t nCount = pData->wndUIDesign.m_listCtrl.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					CControlWnd *pCtrlWnd = pData->wndUIDesign.m_listCtrl[i];
					if (pCtrlWnd->GetControlProperties() == pCtrlProp)
					{
						bFound = TRUE;

						*ppUIWnd = &pData->wndUIDesign;
						*ppCtrlWnd = pCtrlWnd;
						break;
					}
				}
			}
			else if (pData->eTreeItemType == RES_GENERAL_FONT_NODE || pData->eTreeItemType == RES_COLOR_NODE
				|| pData->eTreeItemType == STYLE_PUSHBUTTON_NODE || pData->eTreeItemType == STYLE_HORSCROLLBAR_NODE || pData->eTreeItemType == STYLE_VERSCROLLBAR_NODE)
			{
				if (pData->pResourceOrStyle == pCtrlProp)
				{
					bFound = TRUE;
					break;
				}
			}

			hTIChild = m_pTree->GetNextItem(hTIChild, TVGN_NEXT);
		}

		if (bFound)
		{
			break;
		}

		hTI = m_pTree->GetNextItem(hTI, TVGN_NEXT);
	}

	if (bFound)
	{
		return 0;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CUIShopDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_pTree = g_pFrm->GetFileView()->GetWindowTree();
	m_pTree->DeleteAllItems();
	m_pTree->InsertItem(_T("Windows"));

	UpdateAllViews(NULL);

	return TRUE;
}

// Don't use Serialize to open file
BOOL CUIShopDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
#ifdef _DEBUG
	if (IsModified())
	{
		TRACE(traceAppMsg, 0, "Warning: OnOpenDocument replaces an unsaved document.\n");
	}
#endif

	ENSURE(lpszPathName);

	CFileException fe;
	CFile *pFile = GetFile(lpszPathName,
			CFile::modeRead | CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	ReleaseFile(pFile, FALSE);

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CWaitCursor wait;
	BOOL bRet = OpenFile(lpszPathName);
	if (!bRet)
	{
		DeleteContents();   // remove failed contents
		return FALSE;
	}

	SetModifiedFlag(FALSE);     // start off with unmodified

	// TODO:  Add your specialized creation code here

	return TRUE;
}

BOOL CUIShopDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	DWORD dw = GetTickCount();
	CUIMgr::SetSource(lpszPathName);
	CUIMgr::SaveSkin();
	DWORD dw2 = GetTickCount() - dw;

	SetModifiedFlag(FALSE);     // back to unmodified

	return TRUE;        // success
}

void CUIShopDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	CloseDocument();

	CUIMgr::ReleaseSkin();
	m_pTree = g_pFrm->GetFileView()->GetWindowTree();
	m_pTree->DeleteAllItems();

	g_pFrm->GetPropertiesWnd()->DeleteAllItems();

	CDocument::DeleteContents();
}

BOOL CUIShopDoc::DoFileSave()
{
	DWORD dwAttrib = GetFileAttributes(m_strPathName);
	if (dwAttrib & FILE_ATTRIBUTE_READONLY)
	{
		OpenForEditInPerforce(m_strPathName);

		DWORD dwAttrib = GetFileAttributes(m_strPathName);
		if (dwAttrib & FILE_ATTRIBUTE_READONLY)
		{
			// we do not have read-write access or the file does not (now) exist
			if (!DoSave(NULL))
			{
				TRACE(traceAppMsg, 0, "Warning: File save with new name failed.\n");
				return FALSE;
			}
		}
		else
		{
			if (!DoSave(m_strPathName))
			{
				TRACE(traceAppMsg, 0, "Warning: File save failed.\n");
				return FALSE;
			}
		}
	}
	else
	{
		if (!DoSave(m_strPathName))
		{
			TRACE(traceAppMsg, 0, "Warning: File save failed.\n");
			return FALSE;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CUIShopDoc diagnostics

#ifdef _DEBUG
void CUIShopDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUIShopDoc::Dump(CDumpContext &dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////
// public

BOOL CUIShopDoc::OpenFile(LPCTSTR lpszFile)
{
	int nRet = CUIMgr::SetUIPath(lpszFile);

	// Since there was no Rect property in UIWNDPROPERTIES less than 4515 version of the *.xui,
	// use the size of background image as the Rect property value while loading UI.
	int nXUIVersion = CUIMgr::GetXUIBuildVersion();
	if (nXUIVersion <= 4548)
	{
		int nCount = CUIMgr::GetWindowCount();
		for (int i = 0; i < nCount; ++i)
		{
			UIWNDPROPERTIES *pUIWndProp = CUIMgr::GetWindowItem(i);

			CIUIImage himgBk;
			int nRet = himgBk.AutoLoad((LPCTSTR)pUIWndProp->m_strBKNo);
			if (nRet == 0)
			{
				CRect rcUIWnd;
				rcUIWnd.SetRect(0, 0, himgBk.GetWidth(), himgBk.GetHeight());
				pUIWndProp->m_rcControl = rcUIWnd;
			}
		}
	}

	if (nRet == E_OLDERVERSION)
	{
		CString strInfo = _T("The version of skin file older than the LibUIDK, if you save the skin file, it will be converted to the new version.\r\nAfter it has been converted, you will not be able to edit this skin file in previous versions of UIShop.\r\nopen this skin file?");
		int nRet = AfxMessageBox(strInfo, MB_YESNO);
		if (nRet == IDNO)
		{
			return FALSE;
		}
		nRet = CUIMgr::SetUIPath(lpszFile);
		if (nRet == 0)
		{
			SetPathName(lpszFile);
		}
	}
	else if (nRet == E_NEWERVERSION)
	{
		// Already prompt in CUIMgr::LoadSkin
		// 		CString strInfo;
		// 		strInfo.Format(_T("The selected file\r\n\r\n'%s'\r\n\r\nwas created by a newer version of UIShop and cannot be opened!"), lpszFile);
		// 		int nRet = AfxMessageBox(strInfo);
		return FALSE;
	}
	else if (nRet == E_NOROOTELEMENT)
	{
		CString strInfo;
		strInfo.Format(_T("The selected file\r\n\r\n'%s'\r\n\r\n not found root element!"), lpszFile);
		int nRet = AfxMessageBox(strInfo);
		return FALSE;
	}
	else if (nRet == E_NOWINDOWSNODE)
	{
		CString strInfo;
		strInfo.Format(_T("The selected file\r\n\r\n'%s'\r\n\r\n not found 'Windows' node!"), lpszFile);
		int nRet = AfxMessageBox(strInfo);
		return FALSE;
	}
	else if (nRet == 0)
	{
	}
	else
	{
		return FALSE;
	}

	CFileView *pFileView = g_pFrm->GetFileView();
	ASSERT(pFileView != NULL);
	return pFileView->OnOpenFile();
}

BOOL CUIShopDoc::GetIDsArray(CStringArray &IdArray)
{
	int nNextID = 1000;
	CStringArray saTemp;

	HTREEITEM hRoot = m_pTree->GetRootItem();
	HTREEITEM hItem = m_pTree->GetNextItem(hRoot, TVGN_CHILD);

	while (hItem != NULL)
	{
		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_pTree->GetItemData(hItem);
		ASSERT(pData != NULL);

		CString strWndID = m_pTree->GetItemText(hItem);
		InitIDsArray(saTemp, strWndID, nNextID, IdArray);

		size_t nCtrlCount = pData->wndUIDesign.m_listCtrl.size();
		for (size_t i = 0; i < nCtrlCount; ++i)
		{
			CTRLPROPERTIES *pCtrl = pData->wndUIDesign.m_listCtrl[i]->GetControlProperties();
			InitIDsArray(saTemp, pCtrl->m_strID, nNextID, IdArray);
		}

		hItem = m_pTree->GetNextItem(hItem, TVGN_NEXT);
	}

	return TRUE;
}

BOOL CUIShopDoc::InitIDsArray(CStringArray &rOldIDsArray, LPCTSTR lpszID, int &rnNextID, CStringArray &IdArray)
{
	BOOL bIsExist = FALSE;
	INT_PTR sizeTemp = rOldIDsArray.GetSize();
	for (INT_PTR j = 0; j < sizeTemp; ++j)
	{
		if (rOldIDsArray[j].Compare(lpszID) == 0)
		{
			bIsExist = TRUE;
			break;
		}
	}

	if (!bIsExist)
	{
		rOldIDsArray.Add(lpszID);
		CString strID;
		strID = "#define ";
		strID += lpszID;

		CString strNextID;
		strNextID.Format(_T("%d"), rnNextID);
		if (strID.GetLength() < 39)
		{
			int nLen = 39 - strID.GetLength();
			for (int i = 0; i < nLen; ++i)
			{
				strID += " ";
			}
		}
		else
		{
			strID += " ";
		}
		strID += strNextID;
		strID += "\n";
		TCHAR szBuf[MAX_PATH] = {0};
		_tcscpy_s(szBuf, strID.GetBuffer(strID.GetLength()));
		IdArray.Add(strID);

		rnNextID++;
	}

	return TRUE;
}

int CUIShopDoc::CloseDocument()
{
	if (m_pTree == NULL)
	{
		return -1;
	}

	HTREEITEM hTI = m_pTree->GetRootItem();
	hTI = m_pTree->GetNextItem(hTI, TVGN_CHILD);
	while (hTI != NULL)
	{
		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_pTree->GetItemData(hTI);
		if (pData != NULL)
		{
			if (pData->wndTest.GetSafeHwnd() != NULL)
			{
				pData->wndTest.SendMessage(WM_CLOSE);
			}
		}
		hTI = m_pTree->GetNextItem(hTI, TVGN_NEXT);
	}

	return 0;
}

void CUIShopDoc::SetTreeCtrl(CTreeCtrl *pTree)
{
	m_pTree = pTree;
}

void CUIShopDoc::CallOnFileSave()
{
	OnFileSave();
}

int CUIShopDoc::AddUndo(const ::CAction *pAction)
{
	ASSERT(pAction->m_eAction != ::CAction::actionUnknown);

	if (pAction->m_eAction == ::CAction::actionNewControl)
	{
		ASSERT(pAction->m_pUIWndProp != NULL);
		ASSERT(pAction->m_pCtrlProp != NULL);
	}
	else if (pAction->m_eAction == ::CAction::actionDeleteControl)
	{
		ASSERT(pAction->m_pUIWndProp != NULL);
		ASSERT(pAction->m_pCtrlProp == NULL);	// Shouldn't be assigned.
		ASSERT(!pAction->m_ControlDelete.IsEmpty());
	}
	else if (pAction->m_eAction == ::CAction::actionMoveControl)
	{
		ASSERT(pAction->m_pUIWndProp != NULL);
		ASSERT(pAction->m_pCtrlProp == NULL);	// Shouldn't be assigned.
		ASSERT(!pAction->m_ControlMove.IsEmpty());
	}
	else if (pAction->m_eAction == ::CAction::actionModifyProperty)
	{
		ASSERT(pAction->m_pUIWndProp == NULL);	// The resource and style not belong to CUIDesignWnd.
		ASSERT(pAction->m_pCtrlProp != NULL);
		ASSERT(pAction->m_pPropWnd != NULL);
		ASSERT(pAction->m_nPropIndexInCTRLPROPERTIES >= 0);
		ASSERT(pAction->m_varPropertyValueOld.vt != VT_NULL);
		ASSERT(pAction->m_varPropertyValueNew.vt != VT_NULL);
	}
	else
	{
		ASSERT(FALSE);
	}

	m_Undo.AddHead(*pAction);
	while (m_Undo.GetCount() > MAX_UNDO)
	{
		m_Undo.RemoveTail();
	}

	m_Redo.RemoveAll();

	return 0;
}

int CUIShopDoc::Undo()
{
	::CAction action = m_Undo.RemoveHead();
	if (action.m_eAction == ::CAction::actionNewControl)
	{
		// Undo the new control(Delete control)
		ASSERT(action.m_pUIWndProp != NULL);
		ASSERT(action.m_pCtrlProp != NULL);

		CUIDesignWnd *pUIDesignWnd = NULL;
		CControlWnd *pCtrlWnd = NULL;
		int nRet = FindObject(action.m_pCtrlProp, &pUIDesignWnd, &pCtrlWnd);
		if (nRet == 0)
		{
			ASSERT(pUIDesignWnd != NULL);
			ASSERT(pCtrlWnd != NULL);

			// If the deleted control is focus control.
			if (pUIDesignWnd->GetFocusCtrl() == pCtrlWnd->GetControlProperties())
			{
				pUIDesignWnd->SetFocusCtrl(NULL, TRUE);
			}

			// delete control window
			pUIDesignWnd->DeleteControl(pCtrlWnd, FALSE);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (action.m_eAction == ::CAction::actionDeleteControl)
	{
		ASSERT(action.m_pUIWndProp != NULL);
		ASSERT(action.m_pCtrlProp == NULL);	// Shouldn't be assigned.
		ASSERT(!action.m_ControlDelete.IsEmpty());

		POSITION pos = action.m_ControlDelete.GetHeadPosition();
		while (pos != NULL)
		{
			CControlDelete cd = action.m_ControlDelete.GetNext(pos);
			ASSERT(cd.m_pCtrlProp != NULL);

			CUIDesignWnd *pUIDesignWnd = FindUIDesignWndFromPoint(action.m_pUIWndProp);
			if (pUIDesignWnd != NULL)
			{
				CControlWnd *pCtrlNew = pUIDesignWnd->AddNewControl(cd.m_pCtrlProp->m_rcControl,
						(CONTROL_TYPE)cd.m_pCtrlProp->m_eControlType, cd.m_pCtrlProp);
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
	else if (action.m_eAction == ::CAction::actionMoveControl)
	{
		ASSERT(action.m_pUIWndProp != NULL);
		ASSERT(action.m_pCtrlProp == NULL);		// Shouldn't be assigned.

		// Undo the control(s) move
		POSITION pos = action.m_ControlMove.GetHeadPosition();
		while (pos != NULL)
		{
			CControlMove cm = action.m_ControlMove.GetNext(pos);
			ASSERT(cm.m_pCtrlProp != NULL);

			CUIDesignWnd *pUIDesignWnd = NULL;
			CControlWnd *pCtrlWnd = NULL;
			int nRet = FindObject(cm.m_pCtrlProp, &pUIDesignWnd, &pCtrlWnd);
			if (nRet == 0)
			{
				pCtrlWnd->MoveWindow(cm.m_rectOld);
				pCtrlWnd->Invalidate();
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
	else if (action.m_eAction == ::CAction::actionModifyProperty)
	{
		ASSERT(action.m_pUIWndProp == NULL);	// The resource and style not belong to CUIDesignWnd.
		ASSERT(action.m_pCtrlProp != NULL);
		ASSERT(action.m_pPropWnd != NULL);
		ASSERT(action.m_nPropIndexInCTRLPROPERTIES >= 0);
		ASSERT(action.m_varPropertyValueOld.vt != VT_NULL);
		ASSERT(action.m_varPropertyValueNew.vt != VT_NULL);

		LONG lVarCount = action.m_pCtrlProp->GetPropertyCount();
		if (action.m_nPropIndexInCTRLPROPERTIES >= lVarCount)
		{
			ASSERT(FALSE);
		}
		else
		{
			IUI_VARIANT *pVariant = action.m_pCtrlProp->GetProperty(action.m_nPropIndexInCTRLPROPERTIES);
			ASSERT(pVariant != NULL);

			// Update UI
			CMFCPropertyGridProperty *pProp = action.m_pPropWnd->m_wndPropList.FindItemByData((DWORD_PTR)pVariant, TRUE);
			if (pProp != NULL)	// If the focus change, the pProp may be NULL.
			{
				pProp->SetValue(action.m_varPropertyValueOld);
				action.m_pPropWnd->UpdatePropertyAndUI(pProp);
			}
			else	// May be the focus has changed.
			{
				action.m_pPropWnd->UpdatePropertyAndUI(pVariant, &action.m_varPropertyValueOld);
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	// Add to redo list
	m_Redo.AddHead(action);

	while (m_Redo.GetCount() > MAX_UNDO)
	{
		m_Redo.RemoveTail();
	}

	SetModifiedFlag(TRUE);

	return 0;
}

BOOL CUIShopDoc::CanUndo()
{
	return (m_Undo.GetCount() > 0);
}

int CUIShopDoc::Redo()
{
	::CAction action = m_Redo.RemoveHead();
	if (action.m_eAction == ::CAction::actionNewControl)
	{
		// Redo the new control
		ASSERT(action.m_pUIWndProp != NULL);
		ASSERT(action.m_pCtrlProp != NULL);

		CUIDesignWnd *pUIDesignWnd = FindUIDesignWndFromPoint(action.m_pUIWndProp);
		if (pUIDesignWnd != NULL)
		{
			// Re-create the control, and attach action.m_pCtrlProp to it.
			CControlWnd *pCtrlNew = pUIDesignWnd->AddNewControl(action.m_pCtrlProp->m_rcControl,
					(CONTROL_TYPE)action.m_pCtrlProp->m_eControlType, action.m_pCtrlProp);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (action.m_eAction == ::CAction::actionDeleteControl)
	{
		ASSERT(action.m_pUIWndProp != NULL);
		ASSERT(action.m_pCtrlProp == NULL);	// Shouldn't be assigned.
		ASSERT(!action.m_ControlDelete.IsEmpty());

		POSITION pos = action.m_ControlDelete.GetHeadPosition();
		while (pos != NULL)
		{
			CControlDelete cd = action.m_ControlDelete.GetNext(pos);
			ASSERT(cd.m_pCtrlProp != NULL);

			CUIDesignWnd *pUIDesignWnd = NULL;
			CControlWnd *pCtrlWnd = NULL;
			int nRet = FindObject(cd.m_pCtrlProp, &pUIDesignWnd, &pCtrlWnd);
			if (nRet == 0)
			{
				// If the deleted control is focus control.
				if (pUIDesignWnd->GetFocusCtrl() == pCtrlWnd->GetControlProperties())
				{
					pUIDesignWnd->SetFocusCtrl(NULL, TRUE);
				}

				pUIDesignWnd->DeleteControl(pCtrlWnd, FALSE);
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
	else if (action.m_eAction == ::CAction::actionMoveControl)
	{
		ASSERT(action.m_pUIWndProp != NULL);
		ASSERT(action.m_pCtrlProp == NULL);	// Shouldn't be assigned.

		// Redo the control(s) move
		POSITION pos = action.m_ControlMove.GetHeadPosition();
		while (pos != NULL)
		{
			CControlMove cm = action.m_ControlMove.GetNext(pos);
			ASSERT(cm.m_pCtrlProp != NULL);

			CUIDesignWnd *pUIDesignWnd = NULL;
			CControlWnd *pCtrlWnd = NULL;
			int nRet = FindObject(cm.m_pCtrlProp, &pUIDesignWnd, &pCtrlWnd);
			if (nRet == 0)
			{
				pCtrlWnd->MoveWindow(cm.m_rectNew);
				pCtrlWnd->Invalidate();
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
	else if (action.m_eAction == ::CAction::actionModifyProperty)
	{
		ASSERT(action.m_pUIWndProp == NULL);	// The resource and style not belong to CUIDesignWnd.
		ASSERT(action.m_pCtrlProp != NULL);
		ASSERT(action.m_pPropWnd != NULL);
		ASSERT(action.m_nPropIndexInCTRLPROPERTIES >= 0);
		ASSERT(action.m_varPropertyValueOld.vt != VT_NULL);
		ASSERT(action.m_varPropertyValueNew.vt != VT_NULL);

		LONG lVarCount = action.m_pCtrlProp->GetPropertyCount();
		if (action.m_nPropIndexInCTRLPROPERTIES >= lVarCount)
		{
			ASSERT(FALSE);
		}
		else
		{
			IUI_VARIANT *pVariant = action.m_pCtrlProp->GetProperty(action.m_nPropIndexInCTRLPROPERTIES);
			ASSERT(pVariant != NULL);

			// Update UI
			CMFCPropertyGridProperty *pProp = action.m_pPropWnd->m_wndPropList.FindItemByData((DWORD_PTR)pVariant, TRUE);
			if (pProp != NULL)	// If the focus change, the pProp may be NULL.
			{
				pProp->SetValue(action.m_varPropertyValueNew);
				action.m_pPropWnd->UpdatePropertyAndUI(pProp);
			}
			else	// May be the focus has changed.
			{
				action.m_pPropWnd->UpdatePropertyAndUI(pVariant, &action.m_varPropertyValueNew);
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	// Add to undo list
	m_Undo.AddHead(action);

	while (m_Undo.GetCount() > MAX_UNDO)
	{
		m_Undo.RemoveTail();
	}

	SetModifiedFlag(TRUE);

	return 0;
}

BOOL CUIShopDoc::CanRedo()
{
	return (m_Redo.GetCount() > 0);
}
