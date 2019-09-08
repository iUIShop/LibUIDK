// ChatFrame.h : interface of the CChatFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATFRAME_H__C918A4A4_2886_40AF_A0F5_44ECBBCABFD5__INCLUDED_)
#define AFX_CHATFRAME_H__C918A4A4_2886_40AF_A0F5_44ECBBCABFD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChatFrame : public CUIWnd
{
public:
	CChatFrame();
	virtual ~CChatFrame();
	void WindowID() { IDD = IDW_CHAT_FRAME; }

protected:
	int InitControls();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatFrame)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CChatFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnVideo();
	afx_msg void OnBtnAudio();
	afx_msg void OnBtnTransFile();
	afx_msg void OnBtnCreateGroup();
	afx_msg void OnBtnRemote();
	afx_msg void OnBtnMore();
	afx_msg void OnBtnSend();
	afx_msg void OnBtnSend2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSkinButton *m_pBtnMin;
	CSkinButton *m_pChkMax;
	CSkinButton *m_pBtnExit;
	CSkinButton *m_pBtnVideo;
	CSkinButton *m_pBtnAudio;
	CSkinButton *m_pBtnTransFile;
	CSkinButton *m_pBtnCreateGroup;
	CSkinButton *m_pBtnRemote;
	CSkinButton *m_pBtnMore;
	CSkinButton *m_pChkFont;
	CSkinButton *m_pChkFace;
	CSkinButton *m_pBtnClose;
	CSkinButton *m_pBtnSend;
	CSkinButton *m_pBtnSend2;

public:
	BOOL m_bMax;
	BOOL m_bFont;
	BOOL m_bFace;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_CHATFRAME_H__C918A4A4_2886_40AF_A0F5_44ECBBCABFD5__INCLUDED_)
