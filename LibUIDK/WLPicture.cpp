// WLPicture.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct WLPICMEMBER
{
};


//////////////////////////////////////////////////////////////////////////
// CWLPicture

CWLPicture::CWLPicture()
{
	m_pMember = NULL;
	m_pMember = new WLPICMEMBER;
}

CWLPicture::~CWLPicture()
{
	if (m_pMember != NULL)
	{
		delete (WLPICMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BOOL CWLPicture::Create(LPCTSTR lpszControlName, DWORD dwStyle,
	const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	BOOL bRet = CWLWnd::Create(_T("IUIWL_Picture"), lpszControlName, dwStyle, rect, pParentWnd, nID, pVirtualParent);
	if (!bRet)
	{
		return FALSE;
	}

	return TRUE;
}

int CWLPicture::BindStyle(LPCTSTR lpszStyleID)
{
	if (!IsCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_WL_PICTURE, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CWLPicture::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	if (!IsCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_WL_PICTURE && ect != CT_WL_PICTURE)
	{
		return -3;
	}

	PICWLPROPERTIES *pWLPicProp = (PICWLPROPERTIES *)pCtrlProp;

	// Set Background image
	SetBkImage((LPCTSTR)pWLPicProp->m_strBKN);
	SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
	SetBkImageResizePoint(pCtrlProp->m_ptImageResize);

	SIZE sizeRepeat;
	sizeRepeat.cx = pWLPicProp->m_uXRepeatPixel;
	sizeRepeat.cy = pWLPicProp->m_uYRepeatPixel;
	SetRepeatPixel(&sizeRepeat);

	BYTE btBkTransparent = (BYTE)(LONG)pWLPicProp->m_lBkImgTransparent;
	SetBkImgTransparent(btBkTransparent);

	BindBkColor(pCtrlProp);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// virtual

int CWLPicture::OnDraw(CDC *pMemDCParent)
{
	return CRectCtrl::OnDraw(pMemDCParent);
}

//////////////////////////////////////////////////////////////////////////
// public
