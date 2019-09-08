// ResMgr.cpp: implementation of the CResMgr class.
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CSharedImage

CShareImage::CShareImage()
{
	m_nRefCount = 0;
	m_eControlType = CT_ARROW;
}

CShareImage::~CShareImage()
{

}

//////////////////////////////////////////////////////////////////////////
// CResMgr

std::map<CString, CShareImage *> CResMgr::m_mapSharedImage;

CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{

}

//////////////////////////////////////////////////////////////////////////
// public

int CResMgr::AddImage(const CString &strImageName, __in CShareImage *pSharedImage)
{
	if (strImageName.IsEmpty())
		return E_PARA_INVALID;
	if (pSharedImage == NULL)
		return E_PARA2_NULL;

	std::map<CString, CShareImage *>::iterator itFind = m_mapSharedImage.find(strImageName);
	if (itFind != m_mapSharedImage.end())
		return E_ELEMENT_ALREDY_EXIST;

	m_mapSharedImage[strImageName] = pSharedImage;

	return 0;
}

int CResMgr::GetImage(const CString &strImageName, CShareImage **ppSharedImage)
{
	if (strImageName.IsEmpty())
		return E_PARA_INVALID;
	if (ppSharedImage == NULL)
		return E_PARA2_NULL;

	std::map<CString, CShareImage *>::iterator itFind = m_mapSharedImage.find(strImageName);
	if (itFind == m_mapSharedImage.end())
		return -1;

	*ppSharedImage = itFind->second;

	return 0;
}
