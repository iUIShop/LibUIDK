
// UIShop.h : main header file for the UIShop application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CUIShopApp:
// See UIShop.cpp for the implementation of this class
//

class CUIShopApp : public CWinAppEx
{
public:
	CUIShopApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void AddDocTemplate2(CDocTemplate* pTemplate);
	void LoadStdProfileSettings2(UINT nMaxMRU = _AFX_MRU_COUNT); // load MRU file list and last preview state
	CRecentFileList *GetRecentFileList();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszImageName);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

protected:
	ULONG_PTR m_gdiplusToken;
	ATOM m_atom;
};

extern CUIShopApp theApp;
