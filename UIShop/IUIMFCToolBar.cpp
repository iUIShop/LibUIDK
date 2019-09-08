// IUIMFCToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "IUIMFCToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CIUIMFCToolBar



IMPLEMENT_DYNAMIC(CIUIMFCToolBar, CMFCToolBar)

CIUIMFCToolBar::CIUIMFCToolBar()
{

}

CIUIMFCToolBar::~CIUIMFCToolBar()
{
}


BEGIN_MESSAGE_MAP(CIUIMFCToolBar, CMFCToolBar)
END_MESSAGE_MAP()



// CIUIMFCToolBar message handlers


BOOL CIUIMFCToolBar::LoadBitmapEx(CMFCToolBarInfo& params, BOOL bLocked)
{
	m_bLocked = bLocked;

	if (m_bLocked)
	{
		// Don't add bitmap to the shared resources!
		if (!m_ImagesLocked.Load(params.m_uiHotResID, NULL, TRUE))
		{
			return FALSE;
		}

		if (params.m_uiColdResID != 0)
		{
			if (!m_ColdImagesLocked.Load(params.m_uiColdResID, NULL, TRUE))
			{
				return FALSE;
			}

			ASSERT(m_ImagesLocked.GetCount() == m_ColdImagesLocked.GetCount());
		}
		else if (m_bAutoGrayInactiveImages)
		{
			m_ImagesLocked.CopyTo(m_ColdImagesLocked);
			m_ColdImagesLocked.GrayImages(m_nGrayImagePercentage);
		}

		if (params.m_uiDisabledResID != 0)
		{
			if (!m_DisabledImagesLocked.Load(params.m_uiDisabledResID, NULL, TRUE))
			{
				return FALSE;
			}

			ASSERT(m_ImagesLocked.GetCount() == m_DisabledImagesLocked.GetCount());
		}

		// Load large images:
		if (params.m_uiLargeHotResID != 0)
		{
			if (!m_LargeImagesLocked.Load(params.m_uiLargeHotResID, NULL, TRUE))
			{
				return FALSE;
			}

			ASSERT(m_ImagesLocked.GetCount() == m_LargeImagesLocked.GetCount());
		}

		if (params.m_uiLargeColdResID != 0)
		{
			ASSERT(params.m_uiColdResID != 0);

			if (!m_LargeColdImagesLocked.Load(params.m_uiLargeColdResID, NULL, TRUE))
			{
				return FALSE;
			}

			ASSERT(m_ImagesLocked.GetCount() == m_LargeColdImagesLocked.GetCount());
		}

		if (params.m_uiLargeDisabledResID != 0)
		{
			ASSERT(params.m_uiDisabledResID != 0);

			if (!m_LargeDisabledImagesLocked.Load(params.m_uiLargeDisabledResID, NULL, TRUE))
			{
				return FALSE;
			}

			ASSERT(m_ImagesLocked.GetCount() == m_LargeDisabledImagesLocked.GetCount());
		}

		if (params.m_uiMenuResID != 0)
		{
			if (!m_MenuImagesLocked.Load(params.m_uiMenuResID, NULL, TRUE))
			{
				return FALSE;
			}

			ASSERT(m_ImagesLocked.GetCount() == m_MenuImagesLocked.GetCount());
		}

		if (params.m_uiMenuDisabledResID != 0)
		{
			if (!m_MenuImagesLocked.Load(params.m_uiMenuResID, NULL, TRUE))
			{
				return FALSE;
			}

			ASSERT(m_ImagesLocked.GetCount() == m_MenuImagesLocked.GetCount());
		}

		return TRUE;
	}

	if (!m_Images.Load(params.m_uiHotResID, NULL, TRUE))
	{
		return FALSE;
	}

	m_iImagesOffset = m_Images.GetResourceOffset(params.m_uiHotResID);
	ASSERT(m_iImagesOffset >= 0);

	if (params.m_uiColdResID != 0)
	{
		if (!m_ColdImages.Load(params.m_uiColdResID, NULL, TRUE))
		{
			return FALSE;
		}

		ASSERT(m_Images.GetCount() == m_ColdImages.GetCount());
		ASSERT(m_Images.GetImageSize().cy == m_ColdImages.GetImageSize().cy);
	}
	else if (m_bAutoGrayInactiveImages)
	{
		m_Images.CopyTo(m_ColdImages);
		m_ColdImages.GrayImages(m_nGrayImagePercentage);
	}

	if (params.m_uiMenuResID != 0)
	{
		if (!m_MenuImages.Load(params.m_uiMenuResID, NULL, TRUE))
		{
			return FALSE;
		}

		ASSERT(m_Images.GetCount() == m_MenuImages.GetCount());
		ASSERT(m_MenuImages.GetImageSize().cy == m_sizeMenuImage.cy);
	}

	if (params.m_uiDisabledResID != 0)
	{
		if (!m_DisabledImages.Load(params.m_uiDisabledResID, NULL, TRUE))
		{
			return FALSE;
		}

		ASSERT(m_Images.GetCount() == m_DisabledImages.GetCount());
	}

	if (params.m_uiMenuDisabledResID != 0)
	{
		if (!m_DisabledMenuImages.Load(params.m_uiMenuDisabledResID, NULL, TRUE))
		{
			return FALSE;
		}

		ASSERT(m_Images.GetCount() == m_DisabledMenuImages.GetCount());
	}

	ASSERT(m_Images.GetImageSize().cy == m_sizeImage.cy);

	// Load large images:
	if (params.m_uiLargeHotResID != 0)
	{
		if (!m_LargeImages.Load(params.m_uiLargeHotResID, NULL, TRUE))
		{
			return FALSE;
		}

		ASSERT(m_Images.GetCount() == m_LargeImages.GetCount());
	}

	if (params.m_uiLargeColdResID != 0)
	{
		ASSERT(params.m_uiColdResID != 0);

		if (!m_LargeColdImages.Load(params.m_uiLargeColdResID, NULL, TRUE))
		{
			return FALSE;
		}

		ASSERT(m_Images.GetCount() == m_LargeColdImages.GetCount());
	}

	if (params.m_uiLargeDisabledResID != 0)
	{
		ASSERT(params.m_uiDisabledResID != 0);

		if (!m_LargeDisabledImages.Load(params.m_uiLargeDisabledResID, NULL, TRUE))
		{
			return FALSE;
		}

		ASSERT(m_Images.GetCount() == m_LargeDisabledImages.GetCount());
	}

	return TRUE;
}
