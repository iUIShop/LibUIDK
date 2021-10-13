// SafeMgr.h : main header file for the SAFEMGR application
//

#if !defined(AFX_SAFEMGR_H__479B3322_A02E_4FE1_810A_BBD6AFC5DD72__INCLUDED_)
#define AFX_SAFEMGR_H__479B3322_A02E_4FE1_810A_BBD6AFC5DD72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CSafeMgrApp:
// See SafeMgr.cpp for the implementation of this class
//

class CSafeMgrApp : public CWinApp
{
public:
	CSafeMgrApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSafeMgrApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CSafeMgrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CMainFrame *m_pFrame;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFEMGR_H__479B3322_A02E_4FE1_810A_BBD6AFC5DD72__INCLUDED_)
