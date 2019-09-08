#pragma once


// CIUIMFCToolBar

class CIUIMFCToolBar : public CMFCToolBar
{
	DECLARE_DYNAMIC(CIUIMFCToolBar)

public:
	CIUIMFCToolBar();
	virtual ~CIUIMFCToolBar();

public:
	// Fixed SelectObject error
	virtual BOOL LoadBitmapEx(CMFCToolBarInfo& params, BOOL bLocked = FALSE);

protected:
	DECLARE_MESSAGE_MAP()

protected:
};


