#pragma once


//////////////////////////////////////////////////////////////////////////
// CIUIMFCPropertyGridFileProperty

class CIUIMFCPropertyGridFileProperty : public CMFCPropertyGridFileProperty
{
	DECLARE_DYNAMIC(CIUIMFCPropertyGridFileProperty)

	// Construction
public:
	CIUIMFCPropertyGridFileProperty(const CString& strName, const CString& strFolderName, DWORD_PTR dwData = 0, LPCTSTR lpszDescr = NULL);
	CIUIMFCPropertyGridFileProperty(const CString& strName, BOOL bOpenFileDialog, const CString& strFileName, LPCTSTR lpszDefExt = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);
	virtual ~CIUIMFCPropertyGridFileProperty();

	// Overrides
public:
	virtual void OnClickButton(CPoint point);
};

//////////////////////////////////////////////////////////////////////////
// CIUIMFCPropertyGridCtrl

class CIUIMFCPropertyGridCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CIUIMFCPropertyGridCtrl)

public:
	CIUIMFCPropertyGridCtrl();
	virtual ~CIUIMFCPropertyGridCtrl();

public:
	virtual BOOL EditItem(CMFCPropertyGridProperty* pProp, LPPOINT lptClick = NULL);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	BOOL m_bHandleEditChanged;
};
