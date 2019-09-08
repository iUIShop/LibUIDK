// IUIWinApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIUIWinApp

BEGIN_MESSAGE_MAP(CIUIWinApp, CWinApp)
	//{{AFX_MSG_MAP(CIUIWinApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIUIWinApp construction

CIUIWinApp::CIUIWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


//////////////////////////////////////////////////////////////////////////
// protected

void CIUIWinApp::OnFileNew()
{
	CWinApp::OnFileNew();
}

void CIUIWinApp::OnFileOpen()
{
	CWinApp::OnFileOpen();
}

void CIUIWinApp::AddDocTemplate(CDocTemplate *pTemplate)
{
	if (m_pDocManager == NULL)
	{
		m_pDocManager = new CIUIDocManager;
	}
	m_pDocManager->AddDocTemplate(pTemplate);
}

void CIUIWinApp::OnUpdateRecentFileMenu(CCmdUI *pCmdUI)
{
	CWinApp::OnUpdateRecentFileMenu(pCmdUI);
}

BOOL CIUIWinApp::OnOpenRecentFile(UINT nID)
{
	return CWinApp::OnOpenRecentFile(nID);
}
