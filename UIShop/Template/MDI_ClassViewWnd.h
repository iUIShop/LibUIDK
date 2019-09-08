#if !defined(AFX_CLASSVIEWWND_H__7A48048A_B6F0_4503_9760_915B4C11CA34__INCLUDED_)
#define AFX_CLASSVIEWWND_H__7A48048A_B6F0_4503_9760_915B4C11CA34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassViewWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClassViewWnd window

class CClassViewWnd : public CUIWnd
{
public:
	CClassViewWnd();
	virtual ~CClassViewWnd();
	void WindowID() { IDD = IDW_CLASS_VIEW; }


protected:
	int InitClassTree();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassViewWnd)
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CClassViewWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSkinTreeCtrl *m_treeClass;
	CImageList m_imgList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSVIEWWND_H__7A48048A_B6F0_4503_9760_915B4C11CA34__INCLUDED_)
