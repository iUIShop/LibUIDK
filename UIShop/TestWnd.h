#if !defined(AFX_TESTWND_H__51D247B5_0BBE_4776_BCF3_B0A23547895B__INCLUDED_)
#define AFX_TESTWND_H__51D247B5_0BBE_4776_BCF3_B0A23547895B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestWnd.h : header file
//

#include "../LibUIDK/LibUIDK.h"
/////////////////////////////////////////////////////////////////////////////
// CTestWnd window

#define VIRTUAL_LIST_ITEM_COUNT			100000

class CVirtualListItem
{
public:
	CVirtualListItem()
	{
		m_iImage = 0;
	}
	~CVirtualListItem()
	{

	}

public:
	int m_iImage;
	TCHAR m_szItemText[12];
	TCHAR m_szSubItem1Text[16];
};


class CTestWnd : public CUIWnd
{
// Construction
public:
	CTestWnd();

protected:
	int InitVirtualListItems();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestWnd();
	void WindowID() { }

	// Generated message map functions
protected:
	//{{AFX_MSG(CTestWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMenuClose();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CImageList m_imgList;
	CVirtualListItem m_VirtualListItems[VIRTUAL_LIST_ITEM_COUNT];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTWND_H__51D247B5_0BBE_4776_BCF3_B0A23547895B__INCLUDED_)
