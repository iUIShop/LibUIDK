// UIShopDoc.h : interface of the CUIShopDoc class
//


#pragma once


#include "Action.h"
class CUIDesignWnd;


class CControlWnd;
class CUIShopDoc : public CDocument
{
friend class CUIShopView;

protected: // create from serialization only
	CUIShopDoc();
	virtual ~CUIShopDoc();
	DECLARE_DYNCREATE(CUIShopDoc)

public:
	CUIDesignWnd *FindUIDesignWndFromPoint(UIWNDPROPERTIES *pUIWndProp) const;
	int FindObject(const CTRLPROPERTIES *pCtrlProp, CUIDesignWnd **ppUIWnd, CControlWnd **ppCtrlWnd) const;

public:
	BOOL OpenFile(LPCTSTR lpszFile);
	int CloseDocument();
	BOOL InitIDsArray(CStringArray &rOldIDsArray, LPCTSTR lpszID, int &rnNextID, CStringArray &IdArray);
	BOOL GetIDsArray(CStringArray &IdArray);
	void SetTreeCtrl(CTreeCtrl *pTree);
	void CallOnFileSave();

	// Undo and redo
	int AddUndo(const ::CAction *pAction);
	int Undo();
	BOOL CanUndo();

	int Redo();
	BOOL CanRedo();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual BOOL DoFileSave();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

private:
	CTreeCtrl *m_pTree;

public:
	// Undo & Redo
	CList<::CAction> m_Undo;
	CList<::CAction> m_Redo;
};
