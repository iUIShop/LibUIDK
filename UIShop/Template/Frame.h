// MainFrm.h : interface of the CMainFrame class
//
$$IF(PROJECT_VC60)
/////////////////////////////////////////////////////////////////////////////

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
$$ENDIF
#pragma once
$$IF(PROJECT_VC60)
#endif // _MSC_VER > 1000
$$ENDIF

$$IF(MDI)

#include "ClassViewWnd.h"
#include "FileViewWnd.h"

$$ENDIF
$$IF(IM)

#include "FriendPage.h"
#include "GroupPage.h"
#include "RecentPage.h"
#include "StartMenu.h"

$$ENDIF // IM

$$IF(MDI)
class CMainFrame : public CIUIMDIFrameWnd
$$ENDIF
$$IF(!MDI)
class CMainFrame : public CUIWnd
$$ENDIF
{
public:
	CMainFrame();
	virtual ~CMainFrame();
	void WindowID() { IDD = IDW_MAIN; }

protected:
	int InitControls();
$$IF(MDI)
	int InitMenuBar();

public:
	virtual CMenuBar *GetDefaultMenuBar();
	virtual BOOL GetMDIClientWindowID(UINT *puID);
	virtual BOOL GetMDITabImages(CString *pstrImageNameN, CString *pstrImageNameH,
		CString *pstrImageNameS, CString *pstrImageNameD,
		CString *pstrImageNameCN, CString *pstrImageNameCH,
		CString *pstrImageNameCS, CString *pstrImageNameCD);
	virtual BOOL GetMDITabsRect(LPRECT lpRect);
	virtual BOOL GetSwitchDocumentButtonID(UINT *puID);
	virtual BOOL GetCloseDocumentButtonID(UINT *puID);
$$ENDIF

public:

$$IF(PROJECT_VC60)
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL
$$ENDIF

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// Generated message map functions
protected:
$$IF(PROJECT_VC60)
	//{{AFX_MSG(CMainFrame)
$$ENDIF
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
$$IF(DLG)
	afx_msg void OnBtnOk();
$$ENDIF
$$IF(MDI)
	afx_msg void OnBtnProject();
	afx_msg void OnBtnEdit();
	afx_msg void OnBtnView();
	afx_msg void OnBtnLeft();
	afx_msg void OnBtnRight();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
$$ENDIF // MDI
$$IF(IM)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg LRESULT OnStartMenuDestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStartMenuHidden(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnSkin();
	afx_msg void OnBtnHeader();
	afx_msg void OnChkStatus();
	afx_msg void OnBtnMail();
	afx_msg void OnBtnZone();
	afx_msg void OnBtnMessage();
	afx_msg void OnBtnSearchWeb();
	afx_msg void OnChkStart();
	afx_msg void OnBtnGame();
	afx_msg void OnBtnSetting();
	afx_msg void OnBtnMsg();
	afx_msg void OnBtnSecure();
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnAppMgr();
	afx_msg void OnBtnWeather();
	afx_msg void OnBtnSign();
$$ENDIF // IM
$$IF(PROJECT_VC60)
	//}}AFX_MSG
$$ENDIF
	DECLARE_MESSAGE_MAP()

protected:
$$IF(DLG)
	CSkinStatic *m_pStaTitle;
	CSkinButton *m_pBtnMin;
	CSkinButton *m_pChkMax;
	CSkinButton *m_pBtnClose;
	CSkinStatic *m_pStaMsg;
	CSkinButton *m_pBtnOk;
$$ENDIF // DLG
$$IF(MDI)
	CSkinButton *m_pBtnMin;
	CSkinButton *m_pChkMax;
	CSkinButton *m_pBtnClose;
	CMenuBar *m_pMenuBarMain;
	CSkinButton *m_pBtnProject;
	CSkinButton *m_pBtnEdit;
	CSkinButton *m_pBtnView;
	CSkinButton *m_pBtnLeft;
	CSkinButton *m_pBtnRight;
	CSkinButton *m_pBtnUp;
	CSkinButton *m_pBtnDown;
	CSkinButton *m_pRadClassView;
	CSkinButton *m_pRadFileView;
	CTaskWndMgr *m_pTWMgrLeft;

	HIUIIMAGE m_hMDITab[8];
	CTaskWndMgr m_LeftPanelMgr;
	CClassViewWnd m_wndClassView;
	CFileViewWnd m_wndFileView;
$$ENDIF // MDI
$$IF(IM)
	CSkinButton *m_pBtnSkin;
	CSkinButton *m_pBtnMin;
	CSkinButton *m_pChkMax;
	CSkinButton *m_pBtnClose;
	CSkinButton *m_pBtnHeader;
	CSkinButton *m_pChkStatus;
	CSkinButton *m_pBtnMail;
	CSkinButton *m_pBtnZone;
	CSkinButton *m_pBtnMessage;
	CSkinEdit *m_pEdtSearch;
	CSkinButton *m_pBtnSearchWeb;
	CSkinButton *m_pRadFriend;
	CSkinButton *m_pRadGroup;
	CSkinButton *m_pRadRecent;
	CSkinButton *m_pChkStart;
	CSkinButton *m_pBtnGame;
	CSkinButton *m_pBtnSetting;
	CSkinButton *m_pBtnMsg;
	CSkinButton *m_pBtnSecure;
	CSkinButton *m_pBtnSearch;
	CSkinButton *m_pBtnAppMgr;
	CSkinButton *m_pBtnWeather;
	CSkinStatic *m_pStaName;
	CSkinButton *m_pBtnSign;
	CSkinStatic *m_pStaTitle;
	CTaskWndMgr *m_pTWMgrMain;
	CUniformGridPanel *m_pUGridPnlTab;

protected:
	CFriendPage m_wndFriend;
	CGroupPage m_wndGroup;
	CRecentPage m_wndRecent;
	CStartMenu m_wndStartMenu;

	int m_nColorIndex;
$$ENDIF // IM

public:
	BOOL m_bMax;
$$IF(IM)
	BOOL m_bStatus;
	CString m_strSearch;
	BOOL m_bStart;
$$ENDIF // IM
};
$$IF(PROJECT_VC60)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
$$ENDIF
