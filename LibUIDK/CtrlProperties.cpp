#include "StdAfx.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTRLPROPERTIES::CTRLPROPERTIES()
{
	m_lStructSize.vtEx = VT_EX_STRUCT_SIZE;
	m_lStructSize.strVarName = _T("m_lStructSize");
	m_lStructSize.bShowInPropertiesWnd = m_lStructSize.bSaveToSkinFile = false;
	m_lStructSize = (LONG)sizeof(CTRLPROPERTIES);

	//
	m_eControlType = CT_ARROW;
	m_eControlType.vtEx = VT_EX_CONTROL_TYPE;
	m_eControlType.bReadOnly = true;
	m_eControlType.bGroup = true;
	m_eControlType.strGroupName = _T("General");
	m_eControlType.strVarName = _T("m_eControlType");
	m_eControlType.strUIKeyName = _T("Type=");
	m_eControlType.strPropWndName = _T("Type");
	m_eControlType.strDescription = _T("Display the type of the control.");

	//
	m_strID = _T("");
	m_strID.vtEx = VT_EX_CONTROL_ID_MACRO;
	m_strID.strVarName = _T("strID");
	m_strID.strUIKeyName = _T("IDs");
	m_strID.strXUIElementName = _T("ID");
	m_strID.strPropWndName = _T("ID");
	m_strID.strDescription = _T("Specifies the identifier of the control.");

	m_nID = (LONG) - 1;
	m_nID.vtEx = VT_EX_CONTROL_ID;
	m_nID.strVarName = _T("m_nID");
	m_nID.bShowInPropertiesWnd = m_nID.bSaveToSkinFile = false;

	m_strBindStyle.varDefaultValue = m_strBindStyle = _T("");
	m_strBindStyle.strXUIElementName = _T("BindStyle");
	m_strBindStyle.strPropWndName = _T("Bind Style");

	//
	m_rcControl.vtEx = VT_EX_RECT_WH;
	m_rcControl.varDefaultValue = m_rcControl = CRect(0, 0, 0, 0);
	m_rcControl.strVarName = _T("rect");
	m_rcControl.strUIKeyName = m_rcControl.strXUIElementName = _T("Rect");
	m_rcControl.bShowInPropertiesWnd = false;

	//
	m_bVisible.varDefaultValue = m_bVisible = true;
	m_bVisible.strVarName = _T("m_bVisible");
	m_bVisible.strUIKeyName = m_bVisible.strXUIElementName = _T("IsVisible");
	m_bVisible.strPropWndName = _T("Visible");
	m_bVisible.strDescription = _T("Specifies that the control is initially visible.");

	//
	m_bDisabled.varDefaultValue = m_bDisabled = false;
	m_bDisabled.strVarName = _T("m_bDisabled");
	m_bDisabled.strUIKeyName = m_bDisabled.strXUIElementName = _T("IsDisabled");
	m_bDisabled.strPropWndName = _T("Disabled");
	m_bDisabled.strDescription = _T("Specifies that the control will be initially disabled.");

	m_bTabStop.varDefaultValue = m_bTabStop = false;
	m_bTabStop.strUIKeyName = m_bTabStop.strXUIElementName = _T("IsTabStop");
	m_bTabStop.strPropWndName = _T("Tabstop");
	m_bTabStop.strDescription = _T("Specifies that the user can move to this control with the TAB key.");

	m_bClipChildren.varDefaultValue = m_bClipChildren = true;
	m_bClipChildren.strXUIElementName = _T("IsClipChildren");
	m_bClipChildren.strPropWndName = _T("Clip Children");

	m_bClipSiblings.varDefaultValue = m_bClipSiblings = true;
	m_bClipSiblings.strXUIElementName = _T("IsClipSiblings");
	m_bClipSiblings.strPropWndName = _T("Clip Siblings");

	// Layout
	m_eHorLayoutMode.varDefaultValue = m_eHorLayoutMode = CLH_DEFAULT;
	m_eHorLayoutMode.vtEx = VT_EX_LAYOUT_HOR;
	m_eHorLayoutMode.bReadOnly = true;
	m_eHorLayoutMode.bGroup = true;
	m_eHorLayoutMode.strGroupName = _T("Layout Mode");
	m_eHorLayoutMode.strVarName = _T("m_eHorLayoutMode");
	m_eHorLayoutMode.strUIKeyName = _T("HorLayoutMode");
	m_eHorLayoutMode.strXUIElementName = _T("LayoutMode");
	m_eHorLayoutMode.strXUIAttributeName = _T("Horizontal");
	m_eHorLayoutMode.strPropWndName = _T("Horizontal Layout");
	m_eHorLayoutMode.strDescription = _T("Specifies the horizontal layout mode for the control.");
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_DEFAULT).GetEnumString());
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_LEFT).GetEnumString());
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_RIGHT).GetEnumString());
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_CENTER).GetEnumString());
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_FILL).GetEnumString());
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_FULL_FILL).GetEnumString());
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_FREE).GetEnumString());
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_RIGHT_EVEN_ZOOM_OUT).GetEnumString());
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO).GetEnumString());
	m_eHorLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLH_KEEP_CHILDREN_ASPECT_RATIO).GetEnumString());

	m_eVerLayoutMode.varDefaultValue = m_eVerLayoutMode = CLV_DEFAULT;
	m_eVerLayoutMode.vtEx = VT_EX_LAYOUT_VER;
	m_eVerLayoutMode.bReadOnly = true;
	m_eVerLayoutMode.strVarName = _T("m_eVerLayoutMode");
	m_eVerLayoutMode.strUIKeyName = _T("VerLayoutMode");
	m_eVerLayoutMode.strXUIElementName = _T("LayoutMode");
	m_eVerLayoutMode.strXUIAttributeName = _T("Vertical");
	m_eVerLayoutMode.strPropWndName = _T("Vertical Layout");
	m_eVerLayoutMode.strDescription = _T("Specifies the vertical layout mode for the control.");
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_DEFAULT).GetEnumString());
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_TOP).GetEnumString());
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_BOTTOM).GetEnumString());
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_CENTER).GetEnumString());
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_FILL).GetEnumString());
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_FULL_FILL).GetEnumString());
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_FREE).GetEnumString());
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_BOTTOM_EVEN_ZOOM_OUT).GetEnumString());
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO).GetEnumString());
	m_eVerLayoutMode.lstStringOptions.AddTail(IUI_VARIANT(CLV_KEEP_CHILDREN_ASPECT_RATIO).GetEnumString());

	m_rcLayoutMargin.vtEx = VT_EX_RECT;
	m_rcLayoutMargin.varDefaultValue = m_rcLayoutMargin = CRect(0, 0, 0, 0);
	m_rcLayoutMargin.strPropWndName = _T("Parent Margin");
	m_rcLayoutMargin.strXUIElementName = _T("ParentLayoutMargin");
	m_rcLayoutMargin.nMinValue = -3840;
	m_rcLayoutMargin.nMaxValue = 3840;

	m_rcRegion.vtEx = VT_EX_RECT;
	m_rcRegion.varDefaultValue = m_rcRegion = CRect(0, 0, 0, 0);
	m_rcRegion.strPropWndName = _T("Region");
	m_rcRegion.strXUIElementName = _T("Region");
	m_rcRegion.nMinValue = -3840;
	m_rcRegion.nMaxValue = 3840;

	// Background
	m_bDrawDefault.varDefaultValue = m_bDrawDefault = false;
	m_bDrawDefault.bGroup = true;
	m_bDrawDefault.strGroupName = _T("Background");
	m_bDrawDefault.strXUIElementName = _T("IsDrawDefault");
	m_bDrawDefault.strPropWndName = _T("Draw Default");

	m_bSpecifyBackgroundImages.varDefaultValue = m_bSpecifyBackgroundImages = true;
	m_bSpecifyBackgroundImages.strUIKeyName = m_bSpecifyBackgroundImages.strXUIElementName = _T("IsSpecifyImages");
	m_bSpecifyBackgroundImages.strPropWndName = _T("Specify Images");

	m_bCombineBackgroundImages.varDefaultValue = m_bCombineBackgroundImages = false;
	m_bCombineBackgroundImages.strUIKeyName = m_bCombineBackgroundImages.strXUIElementName = _T("IsCombineImages");
	m_bCombineBackgroundImages.strPropWndName = _T("Combine Images");

	m_strBKCombine = _T("");
	m_strBKCombine.vtEx = VT_EX_BITMAPPATH;
	m_strBKCombine.strUIKeyName = m_strBKCombine.strXUIElementName = _T("BkCombine");
	m_strBKCombine.strXUIAttributeName = _T("RelativePath");
	m_strBKCombine.strPropWndName = _T("CombineImage");

	m_strBKN = _T("");
	m_strBKN.vtEx = VT_EX_BITMAPPATH;
	m_strBKN.strUIKeyName = _T("BkN");
	m_strBKN.strXUIElementName = _T("BackgroundNormal");
	m_strBKN.strXUIAttributeName = _T("RelativePath");
	m_strBKN.strPropWndName = _T("Normal Image");

	m_strBKH = _T("");
	m_strBKH.vtEx = VT_EX_BITMAPPATH;
	m_strBKH.strUIKeyName = _T("BkH");
	m_strBKH.strXUIElementName = _T("BackgroundHighlight");
	m_strBKH.strXUIAttributeName = _T("RelativePath");
	m_strBKH.strPropWndName = _T("Highlight Image");

	m_strBKS = _T("");
	m_strBKS.vtEx = VT_EX_BITMAPPATH;
	m_strBKS.strUIKeyName = _T("BkS");
	m_strBKS.strXUIElementName = _T("BackgroundSelected");
	m_strBKS.strXUIAttributeName = _T("RelativePath");
	m_strBKS.strPropWndName = _T("Selected Image");

	m_strBKD = _T("");
	m_strBKD.vtEx = VT_EX_BITMAPPATH;
	m_strBKD.strUIKeyName = _T("BkD");
	m_strBKD.strXUIElementName = _T("BackgroundDisabled");
	m_strBKD.strXUIAttributeName = _T("RelativePath");
	m_strBKD.strPropWndName = _T("Disabled Image");

	m_strResColorBk = _T("");
	m_strResColorBk.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorBk.strXUIElementName = _T("ColorResBk");
	m_strResColorBk.strPropWndName = _T("Background Color");

	m_lBkImgTransparent.varDefaultValue = m_lBkImgTransparent = (LONG)255;
	m_lBkImgTransparent.strXUIElementName = _T("BkImgTransparent");
	m_lBkImgTransparent.strPropWndName = _T("Transparent");
	m_lBkImgTransparent.nMinValue = 0;
	m_lBkImgTransparent.nMaxValue = 255;

	m_eBkImageResizeMode.varDefaultValue = m_eBkImageResizeMode = IRM_9GRID;
	m_eBkImageResizeMode.vtEx = VT_EX_IMAGE_RESIZE_MODE;
	m_eBkImageResizeMode.strXUIElementName = _T("BkImageResizeMode");
	m_eBkImageResizeMode.strPropWndName = _T("Background Image Resize Mode");
	m_eBkImageResizeMode.lstStringOptions.AddTail(IUI_VARIANT(IRM_9GRID).GetEnumString());
	m_eBkImageResizeMode.lstStringOptions.AddTail(IUI_VARIANT(IRM_STRETCH).GetEnumString());
	m_eBkImageResizeMode.lstStringOptions.AddTail(IUI_VARIANT(IRM_STRETCH_HIGH_QUALITY).GetEnumString());
	m_eBkImageResizeMode.lstStringOptions.AddTail(IUI_VARIANT(IRM_TILE).GetEnumString());
	m_eBkImageResizeMode.lstStringOptions.AddTail(IUI_VARIANT(IRM_CENTER).GetEnumString());

	m_ptImageResize.varDefaultValue = m_ptImageResize = CPoint(2, 2);
	m_ptImageResize.vtEx = VT_EX_POINT;
	m_ptImageResize.strUIKeyName = m_ptImageResize.strXUIElementName = _T("ResizingPoint");
	m_ptImageResize.strPropWndName = _T("Image Resize Point");
	m_ptImageResize.nMinValue = 0;
	m_ptImageResize.nMaxValue = 3840;

	m_uXRepeatPixel.varDefaultValue = m_uXRepeatPixel = (LONG)1;
	m_uXRepeatPixel.strXUIElementName = _T("XRepeatPixel");
	m_uXRepeatPixel.strPropWndName = _T("X Repeat Pixel");
	m_uXRepeatPixel.nMinValue = 1;
	m_uXRepeatPixel.nMaxValue = 3840;

	m_uYRepeatPixel.varDefaultValue = m_uYRepeatPixel = (LONG)1;
	m_uYRepeatPixel.strXUIElementName = _T("YRepeatPixel");
	m_uYRepeatPixel.strPropWndName = _T("Y Repeat Pixel");
	m_uYRepeatPixel.nMinValue = 1;
	m_uYRepeatPixel.nMaxValue = 3840;

	// Font properties for compatible old version of *.xui.
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -12;
	lf.lfWeight = 400;
	lf.lfOutPrecision = 3;
	lf.lfClipPrecision = 2;
	lf.lfQuality = 1;
	lf.lfPitchAndFamily = 34;
	TSTRCPY(lf.lfFaceName, 32, _T("Tahoma"));

	m_lfN = lf;
	m_lfN.vtEx = VT_EX_FONT;
	m_lfN.bNeedToConvert = true;
	m_lfN.strUIKeyName = m_lfN.strXUIElementName = _T("FontN");
	m_lfN.strPropWndName = _T("Normal");

	m_lfH = lf;
	m_lfH.vtEx = VT_EX_FONT;
	m_lfH.bNeedToConvert = true;
	m_lfH.strUIKeyName = m_lfH.strXUIElementName = _T("FontH");
	m_lfH.strPropWndName = _T("Highlight");

	m_lfS = lf;
	m_lfS.vtEx = VT_EX_FONT;
	m_lfS.bNeedToConvert = true;
	m_lfS.strUIKeyName = m_lfS.strXUIElementName = _T("FontS");
	m_lfS.strPropWndName = _T("Selected");

	m_lfD = lf;
	m_lfD.vtEx = VT_EX_FONT;
	m_lfD.bNeedToConvert = true;
	m_lfD.strUIKeyName = m_lfD.strXUIElementName = _T("FontD");
	m_lfD.strPropWndName = _T("Disabled");

	// Font
	m_strResFontIDN = _T("");
	m_strResFontIDN.vtEx = VT_EX_RESOURCE_FONT;
	m_strResFontIDN.bGroup = true;
	m_strResFontIDN.strGroupName = _T("Font(Size)");
	m_strResFontIDN.strXUIElementName = _T("FontResN");
	m_strResFontIDN.strPropWndName = _T("Normal Font");

	m_bHSameNFont.varDefaultValue = m_bHSameNFont = true;
	m_bHSameNFont.strUIKeyName = m_bHSameNFont.strXUIElementName = _T("IsHSameNFont");
	m_bHSameNFont.strPropWndName = _T("Highlight Font Same as Normal");

	m_strResFontIDH = _T("");
	m_strResFontIDH.vtEx = VT_EX_RESOURCE_FONT;
	m_strResFontIDH.strXUIElementName = _T("FontResH");
	m_strResFontIDH.strPropWndName = _T("Highlight Font");

	m_bSSameNFont.varDefaultValue = m_bSSameNFont = true;
	m_bSSameNFont.strUIKeyName = m_bSSameNFont.strXUIElementName = _T("IsSSameNFont");
	m_bSSameNFont.strPropWndName = _T("Selected Font Same as Normal");

	m_strResFontIDS = _T("");
	m_strResFontIDS.vtEx = VT_EX_RESOURCE_FONT;
	m_strResFontIDS.strXUIElementName = _T("FontResS");
	m_strResFontIDS.strPropWndName = _T("Selected Font");

	m_bDSameNFont.varDefaultValue = m_bDSameNFont = true;
	m_bDSameNFont.strUIKeyName = m_bDSameNFont.strXUIElementName = _T("IsDSameNFont");
	m_bDSameNFont.strPropWndName = _T("Disabled Font Same as Normal");

	m_strResFontIDD = _T("");
	m_strResFontIDD.vtEx = VT_EX_RESOURCE_FONT;
	m_strResFontIDD.strXUIElementName = _T("FontResD");
	m_strResFontIDD.strPropWndName = _T("Disabled Font");

	// Color properties for compatible old version of *.xui.
	m_crN = (LONG)RGB(0, 0, 0);	// Because the vc6 not _variant_t::_variant_t(LONG &), so convert RGB to LONG.
	m_crN.vtEx = VT_EX_COLOR;
	m_crN.bNeedToConvert = true;
	m_crN.strUIKeyName = m_crN.strXUIElementName = _T("ColorN");
	m_crN.strPropWndName = _T("Normal");

	m_crH = (LONG)RGB(0, 0, 0);
	m_crH.vtEx = VT_EX_COLOR;
	m_crH.bNeedToConvert = true;
	m_crH.strUIKeyName = m_crH.strXUIElementName = _T("ColorH");
	m_crH.strPropWndName = _T("Highlight");

	m_crS = (LONG)RGB(0, 0, 0);
	m_crS.vtEx = VT_EX_COLOR;
	m_crS.bNeedToConvert = true;
	m_crS.strUIKeyName = m_crS.strXUIElementName = _T("ColorS");
	m_crS.strPropWndName = _T("Selected");

	m_crD = (LONG)RGB(0, 0, 0);
	m_crD.vtEx = VT_EX_COLOR;
	m_crD.bNeedToConvert = true;
	m_crD.strUIKeyName = m_crD.strXUIElementName = _T("ColorD");
	m_crD.strPropWndName = _T("Disabled");

	m_strResColorIDN = _T("");
	m_strResColorIDN.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDN.bGroup = true;
	m_strResColorIDN.strGroupName = _T("Text Color");
	m_strResColorIDN.strXUIElementName = _T("ColorResN");
	m_strResColorIDN.strPropWndName = _T("Normal Color");

	m_bHSameNColor.varDefaultValue = m_bHSameNColor = true;
	m_bHSameNColor.strUIKeyName = m_bHSameNColor.strXUIElementName = _T("IsHSameNColor");
	m_bHSameNColor.strPropWndName = _T("Highlight Color Same as Normal");

	m_strResColorIDH = _T("");
	m_strResColorIDH.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDH.strXUIElementName = _T("ColorResH");
	m_strResColorIDH.strPropWndName = _T("Highlight Color");

	m_bSSameNColor.varDefaultValue = m_bSSameNColor = true;
	m_bSSameNColor.strUIKeyName = m_bSSameNColor.strXUIElementName = _T("IsSSameNColor");
	m_bSSameNColor.strPropWndName = _T("Selected Color Same as Normal");

	m_strResColorIDS = _T("");
	m_strResColorIDS.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDS.strXUIElementName = _T("ColorResS");
	m_strResColorIDS.strPropWndName = _T("Selected Color");

	m_bDSameNColor.varDefaultValue = m_bDSameNColor = true;
	m_bDSameNColor.strUIKeyName = m_bDSameNColor.strXUIElementName = _T("IsDSameNColor");
	m_bDSameNColor.strPropWndName = _T("Disabled Color Same as Normal");

	m_strResColorIDD = _T("");
	m_strResColorIDD.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDD.strXUIElementName = _T("ColorResD");
	m_strResColorIDD.strPropWndName = _T("Disabled Color");
}

CTRLPROPERTIES::~CTRLPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////
// protected

int CTRLPROPERTIES::InitBoolVariant(IUI_VARIANT *pVariant, bool bInitValue, LPCTSTR lpszXUIElementName, LPCTSTR lpszPropWndName, bool bGroup/* = false*/, LPCTSTR lpszGroupName/* = NULL*/)
{
	if (pVariant == NULL)
	{
		return -1;
	}
	if (lpszXUIElementName == NULL)
	{
		return -2;
	}
	if (lpszPropWndName == NULL)
	{
		return -3;
	}
	if (bGroup && lpszGroupName == NULL)
	{
		return -4;
	}

	pVariant->varDefaultValue = *pVariant = bInitValue;
	pVariant->strXUIElementName = lpszXUIElementName;
	pVariant->strPropWndName = lpszPropWndName;

	if (bGroup)
	{
		pVariant->bGroup = true;
		pVariant->strGroupName = lpszGroupName;
	}

	return 0;
}

int CTRLPROPERTIES::InitResFontIDVariant(IUI_VARIANT *pVariant, LPCTSTR lpszXUIElementName, LPCTSTR lpszPropWndName, bool bGroup/* = false*/, LPCTSTR lpszGroupName/* = NULL*/)
{
	if (pVariant == NULL)
	{
		return -1;
	}
	if (lpszXUIElementName == NULL)
	{
		return -2;
	}
	if (lpszPropWndName == NULL)
	{
		return -3;
	}
	if (bGroup && lpszGroupName == NULL)
	{
		return -4;
	}

	*pVariant = _T("");
	pVariant->vtEx = VT_EX_RESOURCE_FONT;
	pVariant->strXUIElementName = lpszXUIElementName;
	pVariant->strPropWndName = lpszPropWndName;

	if (bGroup)
	{
		pVariant->bGroup = true;
		pVariant->strGroupName = lpszGroupName;
	}

	return 0;
}

int CTRLPROPERTIES::InitResColorIDVariant(IUI_VARIANT *pVariant, LPCTSTR lpszXUIElementName, LPCTSTR lpszPropWndName, bool bGroup/* = false*/, LPCTSTR lpszGroupName/* = NULL*/)
{
	if (pVariant == NULL)
	{
		return -1;
	}
	if (lpszXUIElementName == NULL)
	{
		return -2;
	}
	if (lpszPropWndName == NULL)
	{
		return -3;
	}
	if (bGroup && lpszGroupName == NULL)
	{
		return -4;
	}

	*pVariant = _T("");
	pVariant->vtEx = VT_EX_RESOURCE_COLOR;
	pVariant->strXUIElementName = lpszXUIElementName;
	pVariant->strPropWndName = lpszPropWndName;

	if (bGroup)
	{
		pVariant->bGroup = true;
		pVariant->strGroupName = lpszGroupName;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

LONG CTRLPROPERTIES::GetPropertyCount() const
{
	LONG lTotalSize = m_lStructSize;
	LONG lVirtualSize = (LONG)(LPARAM)(&m_lStructSize) - (LONG)(LPARAM)this;
	LONG lVariantSize = sizeof(IUI_VARIANT);
	LONG lVariantSize2 = (LONG)(LPARAM)(&m_eControlType) - (LONG)(LPARAM)(&m_lStructSize);
	if (lVariantSize2 != lVariantSize)
	{
		AfxMessageBox(_T("sizeof(IUI_VARIANT) is undefined!"));
	}

	LONG lVarCount = 0;
	if ((CONTROL_TYPE)m_eControlType == CT_UIWND)
	{
		UIWNDPROPERTIES *pUIProp = (UIWNDPROPERTIES *)this;
		lVarCount = ((LONG)(LPARAM)(&(pUIProp->m_vControls)) - (LONG)LPARAM(&m_lStructSize)) / lVariantSize;
	}
	else
	{
		lVarCount = (lTotalSize - lVirtualSize) / lVariantSize;
		if ((lTotalSize - lVirtualSize) % lVariantSize != 0)
		{
			AfxMessageBox(_T("lVarCount is invalid!"));
		}
	}

	return lVarCount;
}

IUI_VARIANT *CTRLPROPERTIES::GetProperty(int nIndex)
{
	int nCount = GetPropertyCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return NULL;
	}

	LONG lVariantSize = sizeof(IUI_VARIANT);
	IUI_VARIANT *pVariant = (IUI_VARIANT *)((BYTE *)(&m_lStructSize) + lVariantSize * nIndex);
	return pVariant;
}

const IUI_VARIANT *CTRLPROPERTIES::GetProperty(int nIndex) const
{
	int nCount = GetPropertyCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return NULL;
	}

	LONG lVariantSize = sizeof(IUI_VARIANT);
	IUI_VARIANT *pVariant = (IUI_VARIANT *)((BYTE *)(&m_lStructSize) + lVariantSize * nIndex);
	return pVariant;
}

int CTRLPROPERTIES::SetProperty(int nIndex, const IUI_VARIANT *pVariant)
{
	int nCount = GetPropertyCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	LONG lVariantSize = sizeof(IUI_VARIANT);
	IUI_VARIANT *pFindVariant = (IUI_VARIANT *)((BYTE *)(&m_lStructSize) + lVariantSize * nIndex);

	*pFindVariant = *pVariant;

	return 0;
}

int CTRLPROPERTIES::GetIndex(const IUI_VARIANT *pVar) const
{
	if (pVar == NULL)
	{
		return -1;
	}

	LONG lVariantSize = sizeof(IUI_VARIANT);
	INT_PTR nOffset = ((BYTE *)pVar - (BYTE *)(&m_lStructSize));
#ifdef _WIN64
	ASSERT((UINT_PTR)nOffset >> 32 == 0);
#endif
	int nIndex = int(nOffset / lVariantSize);
	ASSERT((nOffset % lVariantSize) == 0);

	return nIndex;
}

CString CTRLPROPERTIES::GetIDPrefix(CONTROL_TYPE eControlType)
{
	CString strName;
	if (eControlType == CT_PUSHBUTTON)
	{
		strName = _T("IDC_BTN_");
	}
	else if (eControlType == CT_CHECK)
	{
		strName = _T("IDC_CHK_");
	}
	else if (eControlType == CT_RADIO)
	{
		strName = _T("IDC_RAD_");
	}
	else if (eControlType == CT_STATIC)
	{
		strName = _T("IDC_STA_");
	}
	else if (eControlType == CT_EDIT)
	{
		strName = _T("IDC_EDT_");
	}
	else if (eControlType == CT_COMBOBOX)
	{
		strName = _T("IDC_CMB_");
	}
	else if (eControlType == CT_SPIN)
	{
		strName = _T("IDC_SPN_");
	}
	else if (eControlType == CT_PROGRESS)
	{
		strName = _T("IDC_PRG_");
	}
	else if (eControlType == CT_SLIDER)
	{
		strName = _T("IDC_SLD_");
	}
	else if (eControlType == CT_HOTKEY)
	{
		strName = _T("IDC_HOT_");
	}
	else if (eControlType == CT_IPADDRESS)
	{
		strName = _T("IDC_IPA_");
	}
	else if (eControlType == CT_SLIDEREX)
	{
		strName = _T("IDC_SLDEX_");
	}
	else if (eControlType == CT_LISTCTRL)
	{
		strName = _T("IDC_LSTCTRL_");
	}
	else if (eControlType == CT_TREECTRL)
	{
		strName = _T("IDC_TREECTRL_");
	}
	else if (eControlType == CT_DATETIMECTRL)
	{
		strName = _T("IDC_DATETIME_");
	}
	else if (eControlType == CT_RICHEDIT)
	{
		strName = _T("IDC_RE_");
	}
	else if (eControlType == CT_RICHEDIT_IM)
	{
		strName = _T("IDC_IM_");
	}
	else if (eControlType == CT_SCROLLBAR)
	{
		strName = _T("IDC_SBEX_");
	}
	else if (eControlType == CT_SCROLLBAREX)
	{
		strName = _T("IDC_SBEX_");
	}
	else if (eControlType == CT_THUMBNAIL)
	{
		strName = _T("IDC_THM_");
	}
	else if (eControlType == CT_TREELIST)
	{
		strName = _T("IDC_TREELST_");
	}
	else if (eControlType == CT_HTMLCTRL)
	{
		strName = _T("IDC_HTMLCTRL_");
	}
	else if (eControlType == CT_WAVECTRL)
	{
		strName = _T("IDC_WAVE_");
	}
	else if (eControlType == CT_PICTURE)
	{
		strName = _T("IDC_PIC_");
	}
	else if (eControlType == CT_MENUBAR)
	{
		strName = _T("IDC_MENUBAR_");
	}
	else if (eControlType == CT_SPLITTER)
	{
		strName = _T("IDC_SPLITTER_");
	}
	else if (eControlType == CT_WL_RECTCTRL)
	{
		strName = _T("IDC_WL_RECT_");
	}
	else if (eControlType == CT_WL_SPLITTER)
	{
		strName = _T("IDC_WL_SPLITTER_");
	}
	else if (eControlType == CT_WL_LINE)
	{
		strName = _T("IDC_WL_LINE_");
	}
	else if (eControlType == CT_WL_TEXT)
	{
		strName = _T("IDC_WL_TXT_");
	}
	else if (eControlType == CT_WL_BUTTON)
	{
		strName = _T("IDC_WL_BTN_");
	}
	else if (eControlType == CT_WL_CHECK)
	{
		strName = _T("IDC_WL_CHK_");
	}
	else if (eControlType == CT_WL_RADIO)
	{
		strName = _T("IDC_WL_RAD_");
	}
	else if (eControlType == CT_WL_SLIDER)
	{
		strName = _T("IDC_WL_SLD_");
	}
	else if (eControlType == CT_WL_PICTURE)
	{
		strName = _T("IDC_WL_PIC_");
	}
	else if (eControlType == CT_WL_RICHEDIT)
	{
		strName = _T("IDC_WL_RE_");
	}
	else if (eControlType == CT_WL_RICHEDIT_IM)
	{
		strName = _T("IDC_WL_IM_");
	}
	else if (eControlType == CT_TASK_WND_MGR)
	{
		strName = _T("IDC_TWMGR_");
	}
	else if (eControlType == CT_PNL_DOCK)
	{
		strName = _T("IDC_PNL_DOCK_");
	}
	else if (eControlType == CT_UNIFORM_GRID)
	{
		strName = _T("IDC_PNL_UGRID_");
	}
	else if (eControlType == CT_PNL_STACK)
	{
		strName = _T("IDC_PNL_STACK_");
	}
	else if (eControlType == CT_MENU)
	{
		strName = _T("IDR_MENU_");
	}
	else if (eControlType == CT_HEADERCTRL)
	{
		strName = _T("IDR_HEADER_");
	}
	else if (eControlType == CT_HORSCROLLBAR)
	{
		strName = _T("IDR_HOR_SB_");
	}
	else if (eControlType == CT_VERSCROLLBAR)
	{
		strName = _T("IDR_VER_SB_");
	}
	// Resource
	else if (eControlType == CT_RESOURCE_GENERAL_FONT)
	{
		strName = _T("IDR_FONT_");
	}
	else if (eControlType == CT_RESOURCE_COLOR)
	{
		strName = _T("IDR_COLOR_");
	}
	else
	{
		ASSERT(FALSE);
	}

	return strName;
}

CString CTRLPROPERTIES::GetClassName() const
{
	CString strName;
	CONTROL_TYPE ect = (CONTROL_TYPE)m_eControlType;
	if (ect == CT_PUSHBUTTON || ect == CT_CHECK || ect == CT_RADIO)
	{
		strName = _T("CSkinButton");
	}
	// Static
	else if (ect == CT_STATIC)
	{
		strName = _T("CSkinStatic");
	}
	// Edit
	else if (ect == CT_EDIT)
	{
		strName = _T("CSkinEdit");
	}
	// Combo box
	else if (ect == CT_COMBOBOX)
	{
		strName = _T("CSkinComboBox");
	}
	// Spin button
	else if (ect == CT_SPIN)
	{
		strName = _T("CSkinSpinButtonCtrl");
	}
	// Progress
	else if (ect == CT_PROGRESS)
	{
		strName = _T("CSkinProgressCtrl");
	}
	// Slider
	else if (ect == CT_SLIDER)
	{
		strName = _T("CSkinSliderCtrl");
	}
	// HotKey
	else if (ect == CT_HOTKEY)
	{
		strName = _T("CSkinHotKeyCtrl");
	}
	// IPAddress
	else if (ect == CT_IPADDRESS)
	{
		strName = _T("CSkinIPAddressCtrl");
	}
	// Extended Slider
	else if (ect == CT_SLIDEREX)
	{
		strName = _T("CSliderCtrlEx");
	}
	// ListCtrl
	else if (ect == CT_LISTCTRL)
	{
		strName = _T("CSkinListCtrl");
	}
	// TreeCtrl
	else if (ect == CT_TREECTRL)
	{
		strName = _T("CSkinTreeCtrl");
	}
	// ScrollBar
	else if (ect == CT_SCROLLBAR)
	{
		strName = _T("CScrollBarEx");
	}
	// ScrollBarEx
	else if (ect == CT_SCROLLBAREX)
	{
		strName = _T("CScrollBarEx");
	}
	// Thumbnail
	else if (ect == CT_THUMBNAIL)
	{
		strName = _T("CThumbnailCtrl");
	}
	// TreeList
	else if (ect == CT_TREELIST)
	{
		strName = _T("CTreeListCtrl");
	}
	// RichEdit
	else if (ect == CT_RICHEDIT)
	{
		strName = _T("CSkinRichEditCtrl");
	}
	else if (ect == CT_RICHEDIT_IM)
	{
		strName = _T("CIMRichEditCtrl");
	}
	// IM RichEdit
	else if (ect == CT_WL_RICHEDIT_IM)
	{
		strName = _T("CIMRichEditCtrl");
	}
	// HtmlCtrl
	else if (ect == CT_HTMLCTRL)
	{
		strName = _T("CHtmlCtrl");
	}
	// DateTimeCtrl
	else if (ect == CT_DATETIMECTRL)
	{
		strName = _T("CSkinDateTimeCtrl");
	}
	// WaveCtrl
	else if (ect == CT_WAVECTRL)
	{
		strName = _T("CWaveCtrl");
	}
	// PictureCtrl
	else if (ect == CT_PICTURE)
	{
		strName = _T("CPictureCtrl");
	}
	// MenuBar
	else if (ect == CT_MENUBAR)
	{
		strName = _T("CMenuBar");
	}
	else if (ect == CT_SPLITTER)
	{
		strName = _T("CSplitterBar");
	}
	// RectCtrl
	else if (ect == CT_WL_RECTCTRL)
	{
		strName = _T("CRectCtrl");
	}
	// Splitter
	else if (ect == CT_WL_SPLITTER)
	{
		strName = _T("CWLSplitterBar");
	}
	else if (ect == CT_WL_LINE)
	{
		strName = _T("CWLLine");
	}
	else if (ect == CT_WL_TEXT)
	{
		strName = _T("CWLText");
	}
	else if (ect == CT_WL_BUTTON || ect == CT_WL_CHECK || ect == CT_WL_RADIO)
	{
		strName = _T("CWLButton");
	}
	else if (ect == CT_WL_SLIDER)
	{
		strName = _T("CWLSliderCtrl");
	}
	else if (ect == CT_WL_PICTURE)
	{
		strName = _T("CWLPicture");
	}
	else if (ect == CT_WL_RICHEDIT)
	{
		strName = _T("CWLRichEditCtrl");
	}
	else if (ect == CT_WL_RICHEDIT_IM)
	{
		strName = _T("CWLIMRichEditCtrl");
	}
	else if (ect == CT_TASK_WND_MGR)
	{
		strName = _T("CTaskWndMgr");
	}
	else if (ect == CT_PNL_DOCK)
	{
		strName = _T("CDockPanel");
	}
	else if (ect == CT_UNIFORM_GRID)
	{
		strName = _T("CUniformGridPanel");
	}
	else if (ect == CT_PNL_STACK)
	{
		strName = _T("CStackPanel");
	}
	else
	{
		ASSERT(FALSE);
	}

	return strName;
}

CString CTRLPROPERTIES::GetVariablePrefix() const
{
	CString strName;
	CONTROL_TYPE ect = (CONTROL_TYPE)m_eControlType;
	if (ect == CT_PUSHBUTTON)
	{
		strName = _T("Btn");
	}
	else if (ect == CT_CHECK)
	{
		strName = _T("Chk");
	}
	else if (ect == CT_RADIO)
	{
		strName = _T("Rad");
	}
	// Static
	else if (ect == CT_STATIC)
	{
		strName = _T("Sta");
	}
	// Edit
	else if (ect == CT_EDIT)
	{
		strName = _T("Edt");
	}
	// Combo box
	else if (ect == CT_COMBOBOX)
	{
		strName = _T("Cmb");
	}
	// Spin button
	else if (ect == CT_SPIN)
	{
		strName = _T("Spn");
	}
	// Progress
	else if (ect == CT_PROGRESS)
	{
		strName = _T("Prg");
	}
	// Slider
	else if (ect == CT_SLIDER)
	{
		strName = _T("Sld");
	}
	// HotKey
	else if (ect == CT_HOTKEY)
	{
		strName = _T("HotKey");
	}
	// IPAddress
	else if (ect == CT_IPADDRESS)
	{
		strName = _T("IPA");
	}
	// Extended Slider
	else if (ect == CT_SLIDEREX)
	{
		strName = _T("SldEx");
	}
	// ListCtrl
	else if (ect == CT_LISTCTRL)
	{
		strName = _T("Lst");
	}
	// TreeCtrl
	else if (ect == CT_TREECTRL)
	{
		strName = _T("Tree");
	}
	// ScrollBar
	else if (ect == CT_SCROLLBAR)
	{
		strName = _T("Sb");
	}
	// ScrollBarEx
	else if (ect == CT_SCROLLBAREX)
	{
		strName = _T("SbEx");
	}
	// Thumbnail
	else if (ect == CT_THUMBNAIL)
	{
		strName = _T("Thm");
	}
	// TreeList
	else if (ect == CT_TREELIST)
	{
		strName = _T("TreeList");
	}
	// RichEdit
	else if (ect == CT_RICHEDIT)
	{
		strName = _T("Re");
	}
	// IM RichEdit
	else if (ect == CT_RICHEDIT_IM)
	{
		strName = _T("Imre");
	}
	// HtmlCtrl
	else if (ect == CT_HTMLCTRL)
	{
		strName = _T("Html");
	}
	// DateTimeCtrl
	else if (ect == CT_DATETIMECTRL)
	{
		strName = _T("DateTime");
	}
	// WaveCtrl
	else if (ect == CT_WAVECTRL)
	{
		strName = _T("Wave");
	}
	// PictureCtrl
	else if (ect == CT_PICTURE)
	{
		strName = _T("Pic");
	}
	// MenuBar
	else if (ect == CT_MENUBAR)
	{
		strName = _T("MenuBar");
	}
	else if (ect == CT_SPLITTER)
	{
		strName = _T("Splitter");
	}
	// RectCtrl
	else if (ect == CT_WL_RECTCTRL)
	{
		strName = _T("Rc");
	}
	// Splitter
	else if (ect == CT_WL_SPLITTER)
	{
		strName = _T("WLSplitter");
	}
	// Line
	else if (ect == CT_WL_LINE)
	{
		strName = _T("WLLine");
	}
	// Text
	else if (ect == CT_WL_TEXT)
	{
		strName = _T("WLTxt");
	}
	// Button windowless
	else if (ect == CT_WL_BUTTON)
	{
		strName = _T("WLBtn");
	}
	else if (ect == CT_WL_CHECK)
	{
		strName = _T("WLChk");
	}
	else if (ect == CT_WL_RADIO)
	{
		strName = _T("WLRad");
	}
	// Slider windowless
	else if (ect == CT_WL_SLIDER)
	{
		strName = _T("WLSld");
	}
	// Picture
	else if (ect == CT_WL_PICTURE)
	{
		strName = _T("WLPic");
	}
	else if (ect == CT_WL_RICHEDIT)
	{
		strName = _T("WLRe");
	}
	else if (ect == CT_WL_RICHEDIT_IM)
	{
		strName = _T("WLIm");
	}
	// TaskWndMgr
	else if (ect == CT_TASK_WND_MGR)
	{
		strName = _T("TWMgr");
	}
	// Dock panel
	else if (ect == CT_PNL_DOCK)
	{
		strName = _T("DockPnl");
	}
	else if (ect == CT_UNIFORM_GRID)
	{
		strName = _T("UGridPnl");
	}
	else if (ect == CT_PNL_STACK)
	{
		strName = _T("StackPnl");
	}
	else
	{
		ASSERT(FALSE);
	}

	return strName;
}

CString CTRLPROPERTIES::GetVariableName() const
{
	CString strName = (LPCTSTR)_bstr_t(m_strID);

	CString strPrefix = CTRLPROPERTIES::GetIDPrefix(m_eControlType);
	if (strName.Left(strPrefix.GetLength()) == strPrefix)
	{
		strName.Delete(0, strPrefix.GetLength());
	}

	strName.MakeLower();
	int nLength = strName.GetLength();
	for (int i = 0; i < nLength; ++i)
	{
		BOOL bMakeUpper = FALSE;
		if (i > 0)
		{
			if (strName[i - 1] == '_' || strName[i - 1] == '-')
			{
				bMakeUpper = TRUE;
			}
		}
		if (i == 0)
		{
			bMakeUpper = TRUE;
		}

		if (bMakeUpper)
		{
			CString strUpper;
			strUpper = strName[i];
			strUpper.MakeUpper();
			strName.SetAt(i, strUpper[0]);
		}
	}

	strName.Remove('-');
	strName.Remove('_');

	return strName;
}

int CTRLPROPERTIES::DisableBackgroundProperties(BOOL bOnlyHighlightSelected)
{
	if (!bOnlyHighlightSelected)
	{
		m_bSpecifyBackgroundImages.bSaveToSkinFile = m_bSpecifyBackgroundImages.bShowInPropertiesWnd = false;
		m_bCombineBackgroundImages.bSaveToSkinFile = m_bCombineBackgroundImages.bShowInPropertiesWnd = false;
		m_strBKCombine.bSaveToSkinFile = m_strBKCombine.bShowInPropertiesWnd = false;
		m_strBKN.bSaveToSkinFile = m_strBKN.bShowInPropertiesWnd = false;
		m_strBKD.bSaveToSkinFile = m_strBKD.bShowInPropertiesWnd = false;

		m_eBkImageResizeMode.bSaveToSkinFile = m_eBkImageResizeMode.bShowInPropertiesWnd = false;
		m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = false;
		m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = false;
		m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = false;
	}

	m_strBKH.bSaveToSkinFile = m_strBKH.bShowInPropertiesWnd = false;
	m_strBKS.bSaveToSkinFile = m_strBKS.bShowInPropertiesWnd = false;

	return 0;
}

int CTRLPROPERTIES::DisableFontProperties(BOOL bOnlyHighlightSelected)
{
	if (!bOnlyHighlightSelected)
	{
		m_lfN.bSaveToSkinFile = m_lfN.bShowInPropertiesWnd = false;
		m_lfD.bSaveToSkinFile = m_lfD.bShowInPropertiesWnd = false;

		m_strResFontIDN.bSaveToSkinFile = m_strResFontIDN.bShowInPropertiesWnd = false;
		m_bDSameNFont.bSaveToSkinFile = m_bDSameNFont.bShowInPropertiesWnd = false;
		m_strResFontIDD.bSaveToSkinFile = m_strResFontIDD.bShowInPropertiesWnd = false;
	}

	m_lfH.bSaveToSkinFile = m_lfH.bShowInPropertiesWnd = false;
	m_lfS.bSaveToSkinFile = m_lfS.bShowInPropertiesWnd = false;

	m_bHSameNFont.bSaveToSkinFile = m_bHSameNFont.bShowInPropertiesWnd = false;
	m_strResFontIDH.bSaveToSkinFile = m_strResFontIDH.bShowInPropertiesWnd = false;
	m_bSSameNFont.bSaveToSkinFile = m_bSSameNFont.bShowInPropertiesWnd = false;
	m_strResFontIDS.bSaveToSkinFile = m_strResFontIDS.bShowInPropertiesWnd = false;

	return 0;
}

int CTRLPROPERTIES::DisableColorProperties(BOOL bOnlyHighlightSelected)
{
	if (!bOnlyHighlightSelected)
	{
		m_crN.bSaveToSkinFile = m_crN.bShowInPropertiesWnd = false;
		m_crD.bSaveToSkinFile = m_crD.bShowInPropertiesWnd = false;

		m_strResColorIDN.bSaveToSkinFile = m_strResColorIDN.bShowInPropertiesWnd = false;
		m_bDSameNColor.bSaveToSkinFile = m_bDSameNColor.bShowInPropertiesWnd = false;
		m_strResColorIDD.bSaveToSkinFile = m_strResColorIDD.bShowInPropertiesWnd = false;
	}

	m_crH.bSaveToSkinFile = m_crH.bShowInPropertiesWnd = false;
	m_crS.bSaveToSkinFile = m_crS.bShowInPropertiesWnd = false;

	m_bHSameNColor.bSaveToSkinFile = m_bHSameNColor.bShowInPropertiesWnd = false;
	m_strResColorIDH.bSaveToSkinFile = m_strResColorIDH.bShowInPropertiesWnd = false;
	m_bSSameNColor.bSaveToSkinFile = m_bSSameNColor.bShowInPropertiesWnd = false;
	m_strResColorIDS.bSaveToSkinFile = m_strResColorIDS.bShowInPropertiesWnd = false;

	return 0;
}

bool CTRLPROPERTIES::IsEnableCustomScrollBar() const
{
	ASSERT(FALSE);
	return FALSE;
}

bool CTRLPROPERTIES::IsFontResource() const
{
	ASSERT(FALSE);
	return false;
}

bool CTRLPROPERTIES::IsColorResource() const
{
	ASSERT(FALSE);
	return false;
}

int CTRLPROPERTIES::IUIGetControlFont1(const CTRLPROPERTIES *pCtrlProp, CString *pstrFontResID)
{
	if (pCtrlProp == NULL || pstrFontResID == NULL)
	{
		return -1;
	}

	*pstrFontResID = (LPCTSTR)pCtrlProp->m_strResFontIDN;

	return 0;
}

int CTRLPROPERTIES::IUIGetControlFont2(const CTRLPROPERTIES *pCtrlProp, CString *pstrFontResID)
{
	if (pCtrlProp == NULL || pstrFontResID == NULL)
	{
		return -1;
	}

	int nIndex = 0;
	*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;

	if (pCtrlProp->m_bDSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDD;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetControlFont4(const CTRLPROPERTIES *pCtrlProp, CString *pstrFontResID)
{
	if (pCtrlProp == NULL || pstrFontResID == NULL)
	{
		return -1;
	}

	int nIndex = 0;
	*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;

	if (pCtrlProp->m_bHSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDH;
	}

	if (pCtrlProp->m_bSSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDS;
	}

	if (pCtrlProp->m_bDSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDD;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetControlFont8(const TOGGLEBTNPROPERTIES *pCtrlProp, CString *pstrFontResID)
{
	if (pCtrlProp == NULL || pstrFontResID == NULL)
	{
		return -1;
	}

	int nIndex = 0;
	*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;

	if (pCtrlProp->m_bHSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDH;
	}

	if (pCtrlProp->m_bSSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDS;
	}

	if (pCtrlProp->m_bDSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDD;
	}

	// Checked state
	if (pCtrlProp->m_bCNSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDCN;
	}

	if (pCtrlProp->m_bCHSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDCH;
	}

	if (pCtrlProp->m_bCSSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDCS;
	}

	if (pCtrlProp->m_bCDSameNFont)
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDN;
	}
	else
	{
		*(pstrFontResID + nIndex++) = (LPCTSTR)pCtrlProp->m_strResFontIDCD;
	}

	return 0;
}

int _InitOtherStateColor(COLORREF *pColor, bool bSameNColor, COLORREF crNormal, LPCTSTR lpszColorID)
{
	if (pColor == NULL)
	{
		return -1;
	}

	if (bSameNColor)
	{
		*pColor = crNormal;
	}
	else
	{
		if (lpszColorID == NULL)
		{
			return -2;
		}

		RESCOLORPROPERTIES *pColorProp = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem(
				lpszColorID);

		*pColor = (pColorProp == NULL) ? crNormal : pColorProp->m_crColor;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetControlColor4(const CTRLPROPERTIES *pCtrlProp,
	COLORREF *pcrN, COLORREF *pcrH, COLORREF *pcrS, COLORREF *pcrD)
{
	RESCOLORPROPERTIES *pColorPropN = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem(
			(LPCTSTR)pCtrlProp->m_strResColorIDN);

	// Default color
	RESCOLORPROPERTIES *pResDefaultColor = CUIMgr::GetDefaultColorResource();
	COLORREF crDefault = pResDefaultColor->m_crColor;

	if (pcrN != NULL)
	{
		*pcrN = (pColorPropN == NULL) ? crDefault : pColorPropN->m_crColor;
	}

	if (pcrH != NULL)
	{
		if (pCtrlProp->m_bHSameNColor)
		{
			*pcrH = *pcrN;
		}
		else
		{
			RESCOLORPROPERTIES *pColorPropH = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem(
					(LPCTSTR)pCtrlProp->m_strResColorIDH);

			*pcrH = (pColorPropH == NULL) ? crDefault : pColorPropH->m_crColor;
		}
	}

	if (pcrS != NULL)
	{
		if (pCtrlProp->m_bSSameNColor)
		{
			*pcrS = *pcrN;
		}
		else
		{
			RESCOLORPROPERTIES *pColorPropS = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem(
					(LPCTSTR)pCtrlProp->m_strResColorIDS);

			*pcrS = (pColorPropS == NULL) ? crDefault : pColorPropS->m_crColor;
		}
	}

	if (pcrD != NULL)
	{
		if (pCtrlProp->m_bDSameNColor)
		{
			*pcrD = *pcrN;
		}
		else
		{
			RESCOLORPROPERTIES *pColorPropD = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem(
					(LPCTSTR)pCtrlProp->m_strResColorIDD);

			*pcrD = (pColorPropD == NULL) ? crDefault : pColorPropD->m_crColor;
		}
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetControlColor8(const TOGGLEBTNPROPERTIES *pCtrlProp,
	COLORREF *pcrN, COLORREF *pcrH, COLORREF *pcrS, COLORREF *pcrD,
	COLORREF *pcrCN, COLORREF *pcrCH, COLORREF *pcrCS, COLORREF *pcrCD)
{
	RESCOLORPROPERTIES *pColorPropN = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem(
			(LPCTSTR)pCtrlProp->m_strResColorIDN);

	// Default color
	RESCOLORPROPERTIES *pResDefaultColor = CUIMgr::GetDefaultColorResource();
	COLORREF crDefault = pResDefaultColor->m_crColor;

	COLORREF crNormal = RGB(255, 255, 255);
	if (pcrN != NULL)
	{
		*pcrN = (pColorPropN == NULL) ? crDefault : pColorPropN->m_crColor;
		crNormal = *pcrN;
	}

	_InitOtherStateColor(pcrH, pCtrlProp->m_bHSameNColor, crNormal, pCtrlProp->m_strResColorIDH);
	_InitOtherStateColor(pcrS, pCtrlProp->m_bSSameNColor, crNormal, pCtrlProp->m_strResColorIDS);
	_InitOtherStateColor(pcrD, pCtrlProp->m_bDSameNColor, crNormal, pCtrlProp->m_strResColorIDD);
	_InitOtherStateColor(pcrCN, pCtrlProp->m_bCNSameNColor, crNormal, pCtrlProp->m_strResColorIDCN);
	_InitOtherStateColor(pcrCH, pCtrlProp->m_bCHSameNColor, crNormal, pCtrlProp->m_strResColorIDCH);
	_InitOtherStateColor(pcrCS, pCtrlProp->m_bCSSameNColor, crNormal, pCtrlProp->m_strResColorIDCS);
	_InitOtherStateColor(pcrCD, pCtrlProp->m_bCDSameNColor, crNormal, pCtrlProp->m_strResColorIDCD);

	return 0;
}

int CTRLPROPERTIES::IUIGetBackground2(const CTRLPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineBackgroundImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strBKCombine;

		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKCombine);
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKN;

		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKN);

		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKD);
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKD;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetBackground4(const CTRLPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineBackgroundImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strBKCombine;

		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKCombine);
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKN;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKN);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKH;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKH);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKS;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKS);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKD;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKD);
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetToggleButtonBackground8(const TOGGLEBTNPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	bool bSpecify = pCtrlProp->m_bSpecifyBackgroundImages;
	if (!bSpecify)
	{
		return -1;
	}

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineBackgroundImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strBKCombine;

		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKCombine);
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKN;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKN);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKH;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKH);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKS;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKS);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKD;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKD);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKCN;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKCN);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKCH;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKCH);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKCS;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKCS);

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strBKCD;
		CreateEmptyIUIImage((LPCTSTR)pCtrlProp->m_strBKCD);
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetToggleButtonForeground8(const TOGGLEBTNPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	bool bSpecify = pCtrlProp->m_bSpecifyForegroundImages;
	if (!bSpecify)
	{
		return -1;
	}

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineForegroundImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strFGCombine;
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGN;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGH;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGS;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGD;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGCN;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGCH;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGCS;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGCD;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetButtonForeground4(const BTNBASEPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineForegroundImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strFGCombine;
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGN;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGH;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGS;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGD;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetStaticForeground2(const TXTPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	bool bSpecify = pCtrlProp->m_bSpecifyForegroundImages;
	if (!bSpecify)
	{
		return -1;
	}

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineForegroundImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strFGCombine;
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGN;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strFGD;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetThumbImages4(const HORSBPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineThumbImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strThumbCombine;
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbN;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbH;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbS;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbD;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetThumbImages4(const VERSBPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineThumbImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strThumbCombine;
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbN;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbH;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbS;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbD;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetThumbImages4(const SLDPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineThumbImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strThumbCombine;
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbN;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbH;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbS;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbD;
	}

	return 0;
}

int CTRLPROPERTIES::IUIGetSliderThumb4(const SLDEXPROPERTIES *pCtrlProp,
	BOOL *pbCombineImage, CString *pstrImageName)
{
	if (pbCombineImage == NULL)
	{
		return -1;
	}

	bool bCombineImage = pCtrlProp->m_bCombineThumbImages;
	*pbCombineImage = bCombineImage;

	if (bCombineImage)
	{
		*pstrImageName = (LPCTSTR)pCtrlProp->m_strThumbCombine;
	}
	else
	{
		int nIndex = 1;

		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbN;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbH;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbS;
		*(pstrImageName + (nIndex++)) = (LPCTSTR)pCtrlProp->m_strThumbD;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

OLDCTRLPROPERTIES::OLDCTRLPROPERTIES()
{
	m_lStructSize.vtEx = VT_EX_STRUCT_SIZE;
	m_lStructSize.strVarName = _T("m_lStructSize");
	m_lStructSize.bShowInPropertiesWnd = m_lStructSize.bSaveToSkinFile = false;
	m_lStructSize = (LONG)sizeof(OLDCTRLPROPERTIES);
}

OLDCTRLPROPERTIES::~OLDCTRLPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////
// public

LONG OLDCTRLPROPERTIES::GetPropertyCount() const
{
	LONG lTotalSize = m_lStructSize;
	LONG lVirtualSize = (LONG)(LPARAM)(&m_lStructSize) - (LONG)(LPARAM)this;
	LONG lVariantSize = sizeof(IUI_VARIANT);

	LONG lVarCount = 0;
	lVarCount = (lTotalSize - lVirtualSize) / lVariantSize;
	if ((lTotalSize - lVirtualSize) % lVariantSize != 0)
	{
		AfxMessageBox(_T("lVarCount is invalid!"));
	}

	return lVarCount;
}

IUI_VARIANT *OLDCTRLPROPERTIES::GetVariant(int nIndex)
{
	int nCount = GetPropertyCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return NULL;
	}

	LONG lVariantSize = sizeof(IUI_VARIANT);
	IUI_VARIANT *pVariant = (IUI_VARIANT *)((BYTE *)(&m_lStructSize) + lVariantSize * nIndex);
	return pVariant;
}

const IUI_VARIANT *OLDCTRLPROPERTIES::GetVariant(int nIndex) const
{
	int nCount = GetPropertyCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return NULL;
	}

	LONG lVariantSize = sizeof(IUI_VARIANT);
	IUI_VARIANT *pVariant = (IUI_VARIANT *)((BYTE *)(&m_lStructSize) + lVariantSize * nIndex);
	return pVariant;
}

int OLDCTRLPROPERTIES::SetVariant(int nIndex, const IUI_VARIANT *pVariant)
{
	int nCount = GetPropertyCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	LONG lVariantSize = sizeof(IUI_VARIANT);
	IUI_VARIANT *pFindVariant = (IUI_VARIANT *)((BYTE *)(&m_lStructSize) + lVariantSize * nIndex);

	*pFindVariant = *pVariant;

	return 0;
}

int OLDCTRLPROPERTIES::GetIndex(const IUI_VARIANT *pVar) const
{
	if (pVar == NULL)
	{
		return -1;
	}

	LONG lVariantSize = sizeof(IUI_VARIANT);
	INT_PTR nOffset = ((BYTE *)pVar - (BYTE *)(&m_lStructSize));
#ifdef _WIN64
	ASSERT((UINT_PTR)nOffset >> 32 == 0);
#endif
	int nIndex = int(nOffset / lVariantSize);
	ASSERT((nOffset % lVariantSize) == 0);

	return nIndex;
}

//////////////////////////////////////////////////////////////////////////

RECTWLPROPERTIES::RECTWLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(RECTWLPROPERTIES);
	m_eControlType = CT_WL_RECTCTRL;

	m_bTabStop.bSaveToSkinFile = m_bTabStop.bShowInPropertiesWnd = false;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = false;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = false;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = false;

	DisableBackgroundProperties(FALSE);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_strBindWindowID.varDefaultValue = m_strBindWindowID = _T("");
	m_strBindWindowID.vtEx = VT_EX_WINDOW_ID;
	m_strBindWindowID.strXUIElementName = _T("BindWindowID");
	m_strBindWindowID.strPropWndName = _T("Bind Window ID");
};

RECTWLPROPERTIES::~RECTWLPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

SPLITTERWLPROPERTIES::SPLITTERWLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(SPLITTERWLPROPERTIES);
	m_eControlType = CT_WL_SPLITTER;

	m_bVisible.bSaveToSkinFile = m_bVisible.bShowInPropertiesWnd = false;
	m_bDisabled.bSaveToSkinFile = m_bDisabled.bShowInPropertiesWnd = false;
	m_bTabStop.bSaveToSkinFile = m_bTabStop.bShowInPropertiesWnd = false;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = false;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = false;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = false;

	DisableBackgroundProperties(FALSE);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_eOrientation.varDefaultValue = m_eOrientation = SO_WE;
	m_eOrientation.vtEx = VT_EX_SPLITTER_ORIENTATION;
	m_eOrientation.bReadOnly = true;
	m_eOrientation.bGroup = true;
	m_eOrientation.strGroupName = _T("General");
	m_eOrientation.strXUIElementName = _T("Orientation");
	m_eOrientation.strPropWndName = _T("Orientation");
	m_eOrientation.lstStringOptions.AddTail(IUI_VARIANT(SO_WE).GetEnumString());
	m_eOrientation.lstStringOptions.AddTail(IUI_VARIANT(SO_NS).GetEnumString());
};

SPLITTERWLPROPERTIES::~SPLITTERWLPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

LINEWLPROPERTIES::LINEWLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(LINEWLPROPERTIES);
	m_eControlType = CT_WL_LINE;

	m_bTabStop.bSaveToSkinFile = m_bTabStop.bShowInPropertiesWnd = false;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = false;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = false;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = false;

	DisableBackgroundProperties(FALSE);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_ePenStyle.varDefaultValue = m_ePenStyle = LPS_SOLID;
	m_ePenStyle.vtEx = VT_EX_PEN_STYLE;
	m_ePenStyle.bGroup = true;
	m_ePenStyle.strGroupName = _T("General");
	m_ePenStyle.strXUIElementName = _T("PenStyle");
	m_ePenStyle.strPropWndName = _T("Pen Style");
	m_ePenStyle.lstStringOptions.AddTail(IUI_VARIANT(LPS_SOLID).GetEnumString());
	m_ePenStyle.lstStringOptions.AddTail(IUI_VARIANT(LPS_DASH).GetEnumString());
	m_ePenStyle.lstStringOptions.AddTail(IUI_VARIANT(LPS_DOT).GetEnumString());
	m_ePenStyle.lstStringOptions.AddTail(IUI_VARIANT(LPS_DASHDOT).GetEnumString());
	m_ePenStyle.lstStringOptions.AddTail(IUI_VARIANT(LPS_DASHDOTDOT).GetEnumString());
	m_ePenStyle.lstStringOptions.AddTail(IUI_VARIANT(LPS_NULL).GetEnumString());
	m_ePenStyle.lstStringOptions.AddTail(IUI_VARIANT(LPS_INSIDEFRAME).GetEnumString());
	m_ePenStyle.lstStringOptions.AddTail(IUI_VARIANT(LPS_USERSTYLE).GetEnumString());
	m_ePenStyle.lstStringOptions.AddTail(IUI_VARIANT(LPS_ALTERNATE).GetEnumString());

	m_lLineWidth.varDefaultValue = m_lLineWidth = (LONG)1;
	m_lLineWidth.strXUIElementName = _T("LineWidth");
	m_lLineWidth.strPropWndName = _T("Line Width");
	m_lLineWidth.nMinValue = 1;
	m_lLineWidth.nMaxValue = 255;

	m_strResColorIDStart = _T("");
	m_strResColorIDStart.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDStart.bGroup = true;
	m_strResColorIDStart.strGroupName = _T("Line");
	m_strResColorIDStart.strXUIElementName = _T("ColorResStart");
	m_strResColorIDStart.strPropWndName = _T("Start Color");

	m_strResColorIDEnd = _T("");
	m_strResColorIDEnd.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDEnd.strXUIElementName = _T("ColorResEnd");
	m_strResColorIDEnd.strPropWndName = _T("End Color");

	m_ptStart = CPoint(0, 0);
	m_ptStart.vtEx = VT_EX_POINT;
	m_ptStart.strXUIElementName = _T("StartPoint");
	m_ptStart.strPropWndName = _T("Start Point");
	m_ptStart.nMinValue = -3829;
	m_ptStart.nMaxValue = 3840;

	m_ptEnd = CPoint(0, 0);		// to another side
	m_ptEnd.vtEx = VT_EX_POINT;
	m_ptEnd.strXUIElementName = _T("EndPoint");
	m_ptEnd.strPropWndName = _T("End Point");
	m_ptEnd.nMinValue = -3829;
	m_ptEnd.nMaxValue = 3840;
};

LINEWLPROPERTIES::~LINEWLPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

PICWLPROPERTIES::PICWLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(PICWLPROPERTIES);
	m_eControlType = CT_WL_PICTURE;

	m_bSpecifyBackgroundImages.bSaveToSkinFile = m_bSpecifyBackgroundImages.bShowInPropertiesWnd = false;
	m_bCombineBackgroundImages.bSaveToSkinFile = m_bCombineBackgroundImages.bShowInPropertiesWnd = false;
	m_strBKCombine.bSaveToSkinFile = m_strBKCombine.bShowInPropertiesWnd = false;
	m_strBKH.bSaveToSkinFile = m_strBKH.bShowInPropertiesWnd = false;
	m_strBKS.bSaveToSkinFile = m_strBKS.bShowInPropertiesWnd = false;
	m_strBKD.bSaveToSkinFile = m_strBKD.bShowInPropertiesWnd = false;
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_eBkImageResizeMode.bSaveToSkinFile = m_eBkImageResizeMode.bShowInPropertiesWnd = true;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = true;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = true;

	// Only use the m_strBKN to store the image.
	m_strBKN.bSaveToSkinFile = m_strBKN.bShowInPropertiesWnd = true;
	m_strBKN.bGroup = true;
	m_strBKN.strGroupName = _T("Image");
	m_strBKN.strXUIElementName = m_strBKN.strPropWndName = _T("Image");

	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_ptImageResize = CPoint(2, 2);
	m_ptImageResize.vtEx = VT_EX_POINT;
	m_ptImageResize.strXUIElementName = _T("ImageResizePoint");
	m_ptImageResize.strPropWndName = _T("Image Resize Point");
	m_ptImageResize.nMinValue = 0;
	m_ptImageResize.nMaxValue = 3840;


	m_strFg = _T("");
	m_strFg.vtEx = VT_EX_BITMAPPATH;
	m_strFg.bGroup = true;
	m_strFg.strGroupName = _T("Foreground");
	m_strFg.strXUIElementName = _T("ForegroundNormal");
	m_strFg.strPropWndName = _T("Normal Image");

	m_btFgImgTransparent.varDefaultValue = m_btFgImgTransparent = (LONG)255;
	m_btFgImgTransparent.strXUIElementName = _T("FgImgTransparent");
	m_btFgImgTransparent.strPropWndName = _T("Transparent");
	m_btFgImgTransparent.nMinValue = 0;
	m_btFgImgTransparent.nMaxValue = 255;

	m_eForegroundHorAlignMode.varDefaultValue = m_eForegroundHorAlignMode = FAH_CENTER;
	m_eForegroundHorAlignMode.vtEx = VT_EX_FOREGROUND_ALIGN_HOR;
	m_eForegroundHorAlignMode.bReadOnly = true;
	m_eForegroundHorAlignMode.strUIKeyName = _T("ForegroundHorAlignMode");
	m_eForegroundHorAlignMode.strXUIElementName = _T("ForegroundAlignMode");
	m_eForegroundHorAlignMode.strXUIAttributeName = _T("Horizontal");
	m_eForegroundHorAlignMode.strPropWndName = _T("Horizontal Alignment");
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_LEFT).GetEnumString());
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_RIGHT).GetEnumString());
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_CENTER).GetEnumString());
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_STRETCH).GetEnumString());

	m_eForegroundVerAlignMode.varDefaultValue = m_eForegroundVerAlignMode = FAV_CENTER;
	m_eForegroundVerAlignMode.vtEx = VT_EX_FOREGROUND_ALIGN_VER;
	m_eForegroundVerAlignMode.bReadOnly = true;
	m_eForegroundVerAlignMode.strUIKeyName = _T("ForegroundVerAlignMode");
	m_eForegroundVerAlignMode.strXUIElementName = _T("ForegroundAlignMode");
	m_eForegroundVerAlignMode.strXUIAttributeName = _T("Vertical");
	m_eForegroundVerAlignMode.strPropWndName = _T("Vertical Alignment");
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_TOP).GetEnumString());
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_BOTTOM).GetEnumString());
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_CENTER).GetEnumString());
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_STRETCH).GetEnumString());

	m_rcForegroundMargin.vtEx = VT_EX_RECT;
	m_rcForegroundMargin.varDefaultValue = m_rcForegroundMargin = CRect(0, 0, 0, 0);
	m_rcForegroundMargin.strPropWndName = _T("Foreground Margin");
	m_rcForegroundMargin.strXUIElementName = _T("ForegroundMargin");
	m_rcForegroundMargin.nMinValue = -3840;
	m_rcForegroundMargin.nMaxValue = 3840;
};

PICWLPROPERTIES::~PICWLPROPERTIES()
{

}

TWMGRPROPERTIES::TWMGRPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(TWMGRPROPERTIES);
	m_eControlType = CT_TASK_WND_MGR;

	m_bClipChildren.bSaveToSkinFile = m_bClipChildren.bShowInPropertiesWnd = false;
	m_bClipSiblings.bSaveToSkinFile = m_bClipSiblings.bShowInPropertiesWnd = false;
	m_bSpecifyBackgroundImages.bSaveToSkinFile = m_bSpecifyBackgroundImages.bShowInPropertiesWnd = false;
	m_bCombineBackgroundImages.bSaveToSkinFile = m_bCombineBackgroundImages.bShowInPropertiesWnd = false;
	m_strBKCombine.bSaveToSkinFile = m_strBKCombine.bShowInPropertiesWnd = false;
	m_strBKH.bSaveToSkinFile = m_strBKH.bShowInPropertiesWnd = false;
	m_strBKS.bSaveToSkinFile = m_strBKS.bShowInPropertiesWnd = false;
	m_strBKD.bSaveToSkinFile = m_strBKD.bShowInPropertiesWnd = false;
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_eBkImageResizeMode.bSaveToSkinFile = m_eBkImageResizeMode.bShowInPropertiesWnd = true;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = true;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = true;

	// Only use the m_strBKN to store the image.
	m_strBKN.bSaveToSkinFile = m_strBKN.bShowInPropertiesWnd = true;
	m_strBKN.bGroup = true;
	m_strBKN.strGroupName = _T("Image");
	m_strBKN.strXUIElementName = m_strBKN.strPropWndName = _T("Image");

	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_ptImageResize = CPoint(2, 2);
	m_ptImageResize.vtEx = VT_EX_POINT;
	m_ptImageResize.strXUIElementName = _T("ImageResizePoint");
	m_ptImageResize.strPropWndName = _T("Image Resize Point");
	m_ptImageResize.nMinValue = 0;
	m_ptImageResize.nMaxValue = 3840;
}

TWMGRPROPERTIES::~TWMGRPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

DOCKPNLPROPERTIES::DOCKPNLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(DOCKPNLPROPERTIES);
	m_eControlType = CT_PNL_DOCK;

	m_bClipChildren.bSaveToSkinFile = m_bClipChildren.bShowInPropertiesWnd = false;
	m_bClipSiblings.bSaveToSkinFile = m_bClipSiblings.bShowInPropertiesWnd = false;
	m_bSpecifyBackgroundImages.bSaveToSkinFile = m_bSpecifyBackgroundImages.bShowInPropertiesWnd = false;
	m_bCombineBackgroundImages.bSaveToSkinFile = m_bCombineBackgroundImages.bShowInPropertiesWnd = false;
	m_strBKCombine.bSaveToSkinFile = m_strBKCombine.bShowInPropertiesWnd = false;
	m_strBKH.bSaveToSkinFile = m_strBKH.bShowInPropertiesWnd = false;
	m_strBKS.bSaveToSkinFile = m_strBKS.bShowInPropertiesWnd = false;
	m_strBKD.bSaveToSkinFile = m_strBKD.bShowInPropertiesWnd = false;
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_eBkImageResizeMode.bSaveToSkinFile = m_eBkImageResizeMode.bShowInPropertiesWnd = true;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = true;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = true;

	// Only use the m_strBKN to store the image.
	m_strBKN.bSaveToSkinFile = m_strBKN.bShowInPropertiesWnd = true;
	m_strBKN.bGroup = true;
	m_strBKN.strGroupName = _T("Image");
	m_strBKN.strXUIElementName = m_strBKN.strPropWndName = _T("Image");

	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_ptImageResize = CPoint(2, 2);
	m_ptImageResize.vtEx = VT_EX_POINT;
	m_ptImageResize.strXUIElementName = _T("ImageResizePoint");
	m_ptImageResize.strPropWndName = _T("Image Resize Point");
	m_ptImageResize.nMinValue = 0;
	m_ptImageResize.nMaxValue = 3840;

	m_bLastChildFill.varDefaultValue = m_bLastChildFill = true;
	m_bLastChildFill.bGroup = true;
	m_bLastChildFill.strGroupName = _T("General");
	m_bLastChildFill.strXUIElementName = _T("IsLastChildFill");
	m_bLastChildFill.strPropWndName = _T("Last Child Fill");
}

DOCKPNLPROPERTIES::~DOCKPNLPROPERTIES()
{

}

UGRIDPNLPROPERTIES::UGRIDPNLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(UGRIDPNLPROPERTIES);
	m_eControlType = CT_UNIFORM_GRID;

	m_bClipChildren.bSaveToSkinFile = m_bClipChildren.bShowInPropertiesWnd = false;
	m_bClipSiblings.bSaveToSkinFile = m_bClipSiblings.bShowInPropertiesWnd = false;
	m_bSpecifyBackgroundImages.bSaveToSkinFile = m_bSpecifyBackgroundImages.bShowInPropertiesWnd = false;
	m_bCombineBackgroundImages.bSaveToSkinFile = m_bCombineBackgroundImages.bShowInPropertiesWnd = false;
	m_strBKCombine.bSaveToSkinFile = m_strBKCombine.bShowInPropertiesWnd = false;
	m_strBKH.bSaveToSkinFile = m_strBKH.bShowInPropertiesWnd = false;
	m_strBKS.bSaveToSkinFile = m_strBKS.bShowInPropertiesWnd = false;
	m_strBKD.bSaveToSkinFile = m_strBKD.bShowInPropertiesWnd = false;
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_eBkImageResizeMode.bSaveToSkinFile = m_eBkImageResizeMode.bShowInPropertiesWnd = true;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = true;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = true;

	// Only use the m_strBKN to store the image.
	m_strBKN.bSaveToSkinFile = m_strBKN.bShowInPropertiesWnd = true;
	m_strBKN.bGroup = true;
	m_strBKN.strGroupName = _T("Image");
	m_strBKN.strXUIElementName = m_strBKN.strPropWndName = _T("Image");

	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_ptImageResize = CPoint(2, 2);
	m_ptImageResize.vtEx = VT_EX_POINT;
	m_ptImageResize.strXUIElementName = _T("ImageResizePoint");
	m_ptImageResize.strPropWndName = _T("Image Resize Point");
	m_ptImageResize.nMinValue = 0;
	m_ptImageResize.nMaxValue = 3840;
}

UGRIDPNLPROPERTIES::~UGRIDPNLPROPERTIES()
{

}

STACKPNLPROPERTIES::STACKPNLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(STACKPNLPROPERTIES);
	m_eControlType = CT_UNIFORM_GRID;

	m_bClipChildren.bSaveToSkinFile = m_bClipChildren.bShowInPropertiesWnd = false;
	m_bClipSiblings.bSaveToSkinFile = m_bClipSiblings.bShowInPropertiesWnd = false;
	m_bSpecifyBackgroundImages.bSaveToSkinFile = m_bSpecifyBackgroundImages.bShowInPropertiesWnd = false;
	m_bCombineBackgroundImages.bSaveToSkinFile = m_bCombineBackgroundImages.bShowInPropertiesWnd = false;
	m_strBKCombine.bSaveToSkinFile = m_strBKCombine.bShowInPropertiesWnd = false;
	m_strBKH.bSaveToSkinFile = m_strBKH.bShowInPropertiesWnd = false;
	m_strBKS.bSaveToSkinFile = m_strBKS.bShowInPropertiesWnd = false;
	m_strBKD.bSaveToSkinFile = m_strBKD.bShowInPropertiesWnd = false;
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_eBkImageResizeMode.bSaveToSkinFile = m_eBkImageResizeMode.bShowInPropertiesWnd = true;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = true;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = true;

	// Only use the m_strBKN to store the image.
	m_strBKN.bSaveToSkinFile = m_strBKN.bShowInPropertiesWnd = true;
	m_strBKN.bGroup = true;
	m_strBKN.strGroupName = _T("Image");
	m_strBKN.strXUIElementName = m_strBKN.strPropWndName = _T("Image");

	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = true;
	m_ptImageResize = CPoint(2, 2);
	m_ptImageResize.vtEx = VT_EX_POINT;
	m_ptImageResize.strXUIElementName = _T("ImageResizePoint");
	m_ptImageResize.strPropWndName = _T("Image Resize Point");
	m_ptImageResize.nMinValue = 0;
	m_ptImageResize.nMaxValue = 3840;

	m_eOrientation.varDefaultValue = m_eOrientation = SPO_LEFT;
	m_eOrientation.vtEx = VT_EX_STACK_PANEL_ORIENTATION;
	m_eOrientation.bReadOnly = true;
	m_eOrientation.bGroup = true;
	m_eOrientation.strGroupName = _T("Orientation");
	m_eOrientation.strXUIElementName = _T("Orientation");
	m_eOrientation.strPropWndName = _T("Orientation");
	m_eOrientation.lstStringOptions.AddTail(IUI_VARIANT(SPO_LEFT).GetEnumString());
	m_eOrientation.lstStringOptions.AddTail(IUI_VARIANT(SPO_TOP).GetEnumString());
	m_eOrientation.lstStringOptions.AddTail(IUI_VARIANT(SPO_RIGHT).GetEnumString());
	m_eOrientation.lstStringOptions.AddTail(IUI_VARIANT(SPO_BOTTOM).GetEnumString());
}

STACKPNLPROPERTIES::~STACKPNLPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

HORSBPROPERTIES::HORSBPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(HORSBPROPERTIES);
	m_eControlType = CT_HORSCROLLBAR;

	m_rcControl.bSaveToSkinFile = m_rcControl.bShowInPropertiesWnd = false;
	m_bVisible.bSaveToSkinFile = m_bVisible.bShowInPropertiesWnd = false;
	m_bDisabled.bSaveToSkinFile = m_bDisabled.bShowInPropertiesWnd = false;
	m_bTabStop.bSaveToSkinFile = m_bTabStop.bShowInPropertiesWnd = false;
	m_eHorLayoutMode.bSaveToSkinFile = m_eHorLayoutMode.bShowInPropertiesWnd = false;
	m_eVerLayoutMode.bSaveToSkinFile = m_eVerLayoutMode.bShowInPropertiesWnd = false;
	m_bSpecifyBackgroundImages.bSaveToSkinFile = m_bSpecifyBackgroundImages.bShowInPropertiesWnd = false;

	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_bCombineBackgroundImages.bGroup = true;
	m_bCombineBackgroundImages.strGroupName = _T("Background");
	m_bCombineBackgroundImages.strXUIElementName = _T("IsCombineBackgroundImages");

	m_ptImageResize = CPoint(20, 10);

	m_bCombineThumbImages.varDefaultValue = m_bCombineThumbImages = false;
	m_bCombineThumbImages.bGroup = true;
	m_bCombineThumbImages.strGroupName = _T("Thumb");
	m_bCombineThumbImages.strXUIElementName = _T("IsCombineThumbImages");
	m_bCombineThumbImages.strPropWndName = _T("CombineThumbImage");

	m_strThumbCombine = _T("");
	m_strThumbCombine.vtEx = VT_EX_BITMAPPATH;
	m_strThumbCombine.strXUIElementName = _T("ThumbCombine");
	m_strThumbCombine.strXUIAttributeName = _T("RelativePath");
	m_strThumbCombine.strPropWndName = _T("Combine Thumb");

	m_strThumbN = _T("");
	m_strThumbN.vtEx = VT_EX_BITMAPPATH;
	m_strThumbN.bDisabledWhileBindIs = true;
	//	m_strThumbN.SetBindVariant(&bUseCustomScrollBar);
	m_strThumbN.SetBindCompareValue(false);
	m_strThumbN.strXUIElementName = _T("ThumbNormal");
	m_strThumbN.strXUIAttributeName = _T("RelativePath");
	m_strThumbN.strPropWndName = _T("Normal Thumb");

	m_strThumbH = _T("");
	m_strThumbH.vtEx = VT_EX_BITMAPPATH;
	m_strThumbH.bDisabledWhileBindIs = true;
	//	m_strThumbH.SetBindVariant(&bUseCustomScrollBar);
	m_strThumbH.SetBindCompareValue(false);
	m_strThumbH.strXUIElementName = _T("ThumbHighlight");
	m_strThumbH.strXUIAttributeName = _T("RelativePath");
	m_strThumbH.strPropWndName = _T("Highlight Thumb");

	m_strThumbS = _T("");
	m_strThumbS.vtEx = VT_EX_BITMAPPATH;
	m_strThumbS.bDisabledWhileBindIs = true;
	//	m_strThumbS.SetBindVariant(&bUseCustomScrollBar);
	m_strThumbS.SetBindCompareValue(false);
	m_strThumbS.strXUIElementName = _T("ThumbSelected");
	m_strThumbS.strXUIAttributeName = _T("RelativePath");
	m_strThumbS.strPropWndName = _T("Selected Thumb");

	m_strThumbD = _T("");
	m_strThumbD.vtEx = VT_EX_BITMAPPATH;
	m_strThumbD.bDisabledWhileBindIs = true;
	//	m_strThumbD.SetBindVariant(&bUseCustomScrollBar);
	m_strThumbD.SetBindCompareValue(false);
	m_strThumbD.strXUIElementName = _T("ThumbDisabled");
	m_strThumbD.strXUIAttributeName = _T("RelativePath");
	m_strThumbD.strPropWndName = _T("Disabled Thumb");

	m_ptThumbResize = CPoint(3, 3);
	m_ptThumbResize.vtEx = VT_EX_POINT;
	m_ptThumbResize.strXUIElementName = _T("ThumbResizingPoint");
	m_ptThumbResize.strPropWndName = _T("Resize Point");
	m_ptThumbResize.nMinValue = 0;
	m_ptThumbResize.nMaxValue = 1920;

	m_strLeftArrow = _T("");
	m_strLeftArrow.vtEx = VT_EX_BITMAPPATH;
	m_strLeftArrow.bGroup = TRUE;
	m_strLeftArrow.strGroupName = _T("Arrow");
	m_strLeftArrow.strXUIElementName = _T("LeftArrow");
	m_strLeftArrow.strXUIAttributeName = _T("RelativePath");
	m_strLeftArrow.strPropWndName = _T("Left Arrow");

	m_strRightArrow = _T("");
	m_strRightArrow.vtEx = VT_EX_BITMAPPATH;
	m_strRightArrow.strXUIElementName = _T("RightArrow");
	m_strRightArrow.strXUIAttributeName = _T("RelativePath");
	m_strRightArrow.strPropWndName = _T("Right Arrow");

	m_nLeftButtonWidth.varDefaultValue = m_nLeftButtonWidth = (LONG)16;
	m_nLeftButtonWidth.bGroup = true;
	m_nLeftButtonWidth.strGroupName = _T("Scroll Bar Button");
	m_nLeftButtonWidth.bDisabledWhileBindIs = true;
	//	m_nLeftButtonWidth.SetBindVariant(&bUseCustomScrollBar);
	m_nLeftButtonWidth.SetBindCompareValue(false);
	m_nLeftButtonWidth.strXUIElementName = _T("LeftUpButtonSize");
	m_nLeftButtonWidth.strPropWndName = _T("Left or Up Button Size");
	m_nLeftButtonWidth.strDescription = _T("Width of the left button if the scroll bar is horizontal, or Height of the up button if the scroll bar is vertical");
	m_nLeftButtonWidth.nMinValue = 0;
	m_nLeftButtonWidth.nMaxValue = 1920;

	m_nRightButtonWidth.varDefaultValue = m_nRightButtonWidth = (LONG)16;
	m_nRightButtonWidth.bDisabledWhileBindIs = true;
	//	m_nRightButtonWidth.SetBindVariant(&bUseCustomScrollBar);
	m_nRightButtonWidth.SetBindCompareValue(false);
	m_nRightButtonWidth.strXUIElementName = _T("RightDownButtonSize");
	m_nRightButtonWidth.strPropWndName = _T("Right or Down Button Size");
	m_nRightButtonWidth.strDescription = _T("Width of the right button if the scroll bar is horizontal, or Height of the down button if the scroll bar is vertical");
	m_nRightButtonWidth.nMinValue = 0;
	m_nRightButtonWidth.nMaxValue = 1920;
}

HORSBPROPERTIES::~HORSBPROPERTIES()
{

}

VERSBPROPERTIES::VERSBPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(VERSBPROPERTIES);
	m_eControlType = CT_VERSCROLLBAR;
	m_rcControl.bSaveToSkinFile = m_rcControl.bShowInPropertiesWnd = false;
	m_bVisible.bSaveToSkinFile = m_bVisible.bShowInPropertiesWnd = false;
	m_bDisabled.bSaveToSkinFile = m_bDisabled.bShowInPropertiesWnd = false;
	m_bTabStop.bSaveToSkinFile = m_bTabStop.bShowInPropertiesWnd = false;
	m_eHorLayoutMode.bSaveToSkinFile = m_eHorLayoutMode.bShowInPropertiesWnd = false;
	m_eVerLayoutMode.bSaveToSkinFile = m_eVerLayoutMode.bShowInPropertiesWnd = false;
	m_bSpecifyBackgroundImages.bSaveToSkinFile = m_bSpecifyBackgroundImages.bShowInPropertiesWnd = false;

	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_bCombineBackgroundImages.bGroup = true;
	m_bCombineBackgroundImages.strGroupName = _T("Background");
	m_bCombineBackgroundImages.strXUIElementName = _T("IsCombineBackgroundImages");

	m_ptImageResize = CPoint(10, 20);

	m_bCombineThumbImages.varDefaultValue = m_bCombineThumbImages = false;
	m_bCombineThumbImages.bGroup = true;
	m_bCombineThumbImages.strGroupName = _T("Thumb");
	m_bCombineThumbImages.strXUIElementName = _T("IsCombineThumbImages");
	m_bCombineThumbImages.strPropWndName = _T("CombineThumbImage");

	m_strThumbCombine = _T("");
	m_strThumbCombine.vtEx = VT_EX_BITMAPPATH;
	m_strThumbCombine.strXUIElementName = _T("ThumbCombine");
	m_strThumbCombine.strXUIAttributeName = _T("RelativePath");
	m_strThumbCombine.strPropWndName = _T("Combine Thumb");

	m_strThumbN = _T("");
	m_strThumbN.vtEx = VT_EX_BITMAPPATH;
	m_strThumbN.bDisabledWhileBindIs = true;
	//	m_strThumbN.SetBindVariant(&bUseCustomScrollBar);
	m_strThumbN.SetBindCompareValue(false);
	m_strThumbN.strXUIElementName = _T("ThumbNormal");
	m_strThumbN.strXUIAttributeName = _T("RelativePath");
	m_strThumbN.strPropWndName = _T("Normal Thumb");

	m_strThumbH = _T("");
	m_strThumbH.vtEx = VT_EX_BITMAPPATH;
	m_strThumbH.bDisabledWhileBindIs = true;
	//	m_strThumbH.SetBindVariant(&bUseCustomScrollBar);
	m_strThumbH.SetBindCompareValue(false);
	m_strThumbH.strXUIElementName = _T("ThumbHighlight");
	m_strThumbH.strXUIAttributeName = _T("RelativePath");
	m_strThumbH.strPropWndName = _T("Highlight Thumb");

	m_strThumbS = _T("");
	m_strThumbS.vtEx = VT_EX_BITMAPPATH;
	m_strThumbS.bDisabledWhileBindIs = true;
	//	m_strThumbS.SetBindVariant(&bUseCustomScrollBar);
	m_strThumbS.SetBindCompareValue(false);
	m_strThumbS.strXUIElementName = _T("ThumbSelected");
	m_strThumbS.strXUIAttributeName = _T("RelativePath");
	m_strThumbS.strPropWndName = _T("Selected Thumb");

	m_strThumbD = _T("");
	m_strThumbD.vtEx = VT_EX_BITMAPPATH;
	m_strThumbD.bDisabledWhileBindIs = true;
	//	m_strThumbD.SetBindVariant(&bUseCustomScrollBar);
	m_strThumbD.SetBindCompareValue(false);
	m_strThumbD.strXUIElementName = _T("ThumbDisabled");
	m_strThumbD.strXUIAttributeName = _T("RelativePath");
	m_strThumbD.strPropWndName = _T("Disabled Thumb");

	m_ptThumbResize = CPoint(3, 3);
	m_ptThumbResize.vtEx = VT_EX_POINT;
	m_ptThumbResize.strXUIElementName = _T("ThumbResizingPoint");
	m_ptThumbResize.strPropWndName = _T("Resize Point");
	m_ptThumbResize.nMinValue = 0;
	m_ptThumbResize.nMaxValue = 1920;

	m_strUpArrow = _T("");
	m_strUpArrow.vtEx = VT_EX_BITMAPPATH;
	m_strUpArrow.bGroup = TRUE;
	m_strUpArrow.strGroupName = _T("Arrow");
	m_strUpArrow.strXUIElementName = _T("UpArrow");
	m_strUpArrow.strXUIAttributeName = _T("RelativePath");
	m_strUpArrow.strPropWndName = _T("Up Arrow");

	m_strDownArrow = _T("");
	m_strDownArrow.vtEx = VT_EX_BITMAPPATH;
	m_strDownArrow.strXUIElementName = _T("DownArrow");
	m_strDownArrow.strXUIAttributeName = _T("RelativePath");
	m_strDownArrow.strPropWndName = _T("Down Arrow");

	m_nUpButtonHeight.varDefaultValue = m_nUpButtonHeight = (LONG)16;
	m_nUpButtonHeight.bGroup = true;
	m_nUpButtonHeight.strGroupName = _T("Scroll Bar Button");
	m_nUpButtonHeight.bDisabledWhileBindIs = true;
	//	nUpButtonHeight.SetBindVariant(&bUseCustomScrollBar);
	m_nUpButtonHeight.SetBindCompareValue(false);
	m_nUpButtonHeight.strXUIElementName = _T("LeftUpButtonSize");
	m_nUpButtonHeight.strPropWndName = _T("Left or Up Button Size");
	m_nUpButtonHeight.strDescription = _T("Width of the left button if the scroll bar is horizontal, or Height of the up button if the scroll bar is vertical");
	m_nUpButtonHeight.nMinValue = 0;
	m_nUpButtonHeight.nMaxValue = 1920;

	m_nDownButtonHeight.varDefaultValue = m_nDownButtonHeight = (LONG)16;
	m_nDownButtonHeight.bDisabledWhileBindIs = true;
	//	m_nDownButtonHeight.SetBindVariant(&bUseCustomScrollBar);
	m_nDownButtonHeight.SetBindCompareValue(false);
	m_nDownButtonHeight.strXUIElementName = _T("RightDownButtonSize");
	m_nDownButtonHeight.strPropWndName = _T("Right or Down Button Size");
	m_nDownButtonHeight.strDescription = _T("Width of the right button if the scroll bar is horizontal, or Height of the down button if the scroll bar is vertical");
	m_nDownButtonHeight.nMinValue = 0;
	m_nDownButtonHeight.nMaxValue = 1920;
}

VERSBPROPERTIES::~VERSBPROPERTIES()
{

}


BTNBASEPROPERTIES::BTNBASEPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(BTNBASEPROPERTIES);

	m_bR2LReadOrder.varDefaultValue = m_bR2LReadOrder = false;
	m_bR2LReadOrder.bGroup = true;
	m_bR2LReadOrder.strGroupName = _T("General");
	m_bR2LReadOrder.strVarName = _T("m_bR2LReadOrder");
	m_bR2LReadOrder.strUIKeyName = m_bR2LReadOrder.strXUIElementName = _T("IsR2LReadOrder");
	m_bR2LReadOrder.strPropWndName = _T("Right to Left Reading Order");
	m_bR2LReadOrder.strDescription = _T("For languages that support reading order alignment, specifies right-to-left reading order.");

	m_bMultiline.varDefaultValue = m_bMultiline = false;
	m_bMultiline.strVarName = _T("m_bMultiline");
	m_bMultiline.strUIKeyName = m_bMultiline.strXUIElementName = _T("IsMultiline");
	m_bMultiline.strPropWndName = _T("Multiline");
	m_bMultiline.strDescription = _T("Wraps control text to multiple lines if text is too long for control width.");

	m_bEndEllipsis.varDefaultValue = m_bEndEllipsis = false;
	m_bEndEllipsis.strUIKeyName = m_bEndEllipsis.strXUIElementName = _T("IsEndEllipsis");
	m_bEndEllipsis.strPropWndName = _T("End Ellipsis");
	m_bEndEllipsis.strDescription = _T("Specifies that the end of the string is truncated and ellipses are added at the end if it doesn't fit in the control's rectangle.");

	m_strCaption.varDefaultValue = m_strCaption = _T("");
	m_strCaption.strUIKeyName = m_strCaption.strXUIElementName = _T("Caption");
	m_strCaption.strPropWndName = _T("Caption");
	m_strCaption.strDescription = _T("");

	m_strTag.varDefaultValue = m_strTag = _T("");
	m_strTag.strUIKeyName = m_strTag.strXUIElementName = _T("Tag");
	m_strTag.strPropWndName = _T("Tag");
	m_strTag.strDescription = _T("");

	m_strMaskBmp = _T("");
	m_strMaskBmp.vtEx = VT_EX_BITMAPPATH;
	m_strMaskBmp.strUIKeyName = _T("MaskBmp");
	m_strMaskBmp.strXUIElementName = _T("MaskBitmap");
	m_strMaskBmp.strXUIAttributeName = _T("RelativePath");
	m_strMaskBmp.strPropWndName = _T("Mask Bitmap");
	m_strMaskBmp.strDescription = _T("");

	// Click the button will popup a modal dialog;
	m_strModalDlgID.varDefaultValue = m_strModalDlgID = _T("");
	m_strModalDlgID.vtEx = VT_EX_WINDOW_ID;
	m_strModalDlgID.strXUIElementName = _T("ModalDlgID");
	m_strModalDlgID.strPropWndName = _T("Popup modal dialog ID");

	// Click the button will popup a modeless window.
	m_strModelessWndID.varDefaultValue = m_strModelessWndID = _T("");
	m_strModelessWndID.vtEx = VT_EX_WINDOW_ID;
	m_strModelessWndID.strXUIElementName = _T("ModelessWndID");
	m_strModelessWndID.strPropWndName = _T("Popup modeless window ID");

	m_bUseToolTip.varDefaultValue = m_bUseToolTip = false;
	m_bUseToolTip.bGroup = true;
	m_bUseToolTip.strGroupName = _T("ToolTip");
	m_bUseToolTip.strVarName = _T("m_bUseToolTip");
	m_bUseToolTip.strUIKeyName = _T("UseToolTip");
	m_bUseToolTip.strXUIElementName = _T("ToolTip");
	m_bUseToolTip.strXUIAttributeName = _T("Enable");
	m_bUseToolTip.strPropWndName = _T("Enable Tooltip");
	m_bUseToolTip.strDescription = _T("Enable Tooltip.");

	m_strToolTip.varDefaultValue = m_strToolTip = _T("");
	m_strToolTip.strUIKeyName = _T("ToolTips");
	m_strToolTip.strXUIElementName = m_strToolTip.strXUIAttributeName = _T("ToolTip");
	m_strToolTip.strPropWndName = _T("ToolTip");
	m_strToolTip.strDescription = _T("");

	m_lAutoPopDelayTime.varDefaultValue = m_lAutoPopDelayTime = (LONG)5000;	// GetDoubleClickTime() * 10
	m_lAutoPopDelayTime.strXUIElementName = _T("ToolTipAutoPopDelayTime");
	m_lAutoPopDelayTime.strPropWndName = _T("ToolTip Auto Pop Delay Time");
	m_lAutoPopDelayTime.strDescription = _T("See MSDN: CToolTipCtrl::GetDelayTime and TTM_SETDELAYTIME");
	m_lAutoPopDelayTime.nMaxValue = 0;	//
	m_lAutoPopDelayTime.nMaxValue = MAXINT;

	m_lInitialDelayTime.varDefaultValue = m_lInitialDelayTime = (LONG)500;	// GetDoubleClickTime()
	m_lInitialDelayTime.strXUIElementName = _T("ToolTipInitialDelayTime");
	m_lInitialDelayTime.strPropWndName = _T("ToolTip Initial Delay Time");
	m_lInitialDelayTime.strDescription = _T("See MSDN: CToolTipCtrl::GetDelayTime and TTM_SETDELAYTIME");
	m_lInitialDelayTime.nMaxValue = 0;	//
	m_lInitialDelayTime.nMaxValue = MAXINT;

	m_lReshowDelayTime.varDefaultValue = m_lReshowDelayTime = (LONG)100;	// GetDoubleClickTime() / 5
	m_lReshowDelayTime.strXUIElementName = _T("ToolTipReshowDelayTime");
	m_lReshowDelayTime.strPropWndName = _T("ToolTip Reshow Delay Time");
	m_lReshowDelayTime.strDescription = _T("See MSDN: CToolTipCtrl::GetDelayTime and TTM_SETDELAYTIME");
	m_lReshowDelayTime.nMaxValue = 0;	//
	m_lReshowDelayTime.nMaxValue = MAXINT;

	m_eTextHorAlignMode.varDefaultValue = m_eTextHorAlignMode = TAH_DEFAULT;
	m_eTextHorAlignMode.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignMode.bReadOnly = true;
	m_eTextHorAlignMode.bGroup = true;
	m_eTextHorAlignMode.strGroupName = _T("Text Alignment");
	m_eTextHorAlignMode.strUIKeyName = _T("HorAlignMode");
	m_eTextHorAlignMode.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignMode.strXUIAttributeName = _T("Horizontal");
	m_eTextHorAlignMode.strPropWndName = _T("Horizontal");
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	m_eTextVerAlignMode.varDefaultValue = m_eTextVerAlignMode = TAV_DEFAULT;
	m_eTextVerAlignMode.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignMode.bReadOnly = true;
	m_eTextVerAlignMode.strUIKeyName = _T("VerAlignMode");
	m_eTextVerAlignMode.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignMode.strXUIAttributeName = _T("Vertical");
	m_eTextVerAlignMode.strPropWndName = _T("Vertical");
	m_eTextVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());
	m_eTextVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());

	m_rcPadding4Text.vtEx = VT_EX_RECT;
	m_rcPadding4Text.varDefaultValue = m_rcPadding4Text = CRect(0, 0, 0, 0);
	m_rcPadding4Text.strPropWndName = _T("Padding for Text");
	m_rcPadding4Text.strXUIElementName = _T("TextMarginN");
	m_rcPadding4Text.nMinValue = -3840;
	m_rcPadding4Text.nMaxValue = 3840;

	m_bSpecifyForegroundImages.varDefaultValue = m_bSpecifyForegroundImages = false;
	m_bSpecifyForegroundImages.bGroup = true;
	m_bSpecifyForegroundImages.strGroupName = _T("Foreground");
	m_bSpecifyForegroundImages.strUIKeyName = m_bSpecifyForegroundImages.strXUIElementName = _T("IsSpecifyForegroundImages");
	m_bSpecifyForegroundImages.strPropWndName = _T("Specify Images");

	m_bCombineForegroundImages.varDefaultValue = m_bCombineForegroundImages = false;
	m_bCombineForegroundImages.strUIKeyName = m_bCombineForegroundImages.strXUIElementName = _T("IsCombineForegroundImages");
	m_bCombineForegroundImages.strPropWndName = _T("Combine Images");

	m_strFGCombine = _T("");
	m_strFGCombine.vtEx = VT_EX_BITMAPPATH;
	m_strFGCombine.strXUIElementName = _T("ForeGroundCombine");
	m_strFGCombine.strPropWndName = _T("CombineImage");

	m_strFGN = _T("");
	m_strFGN.vtEx = VT_EX_BITMAPPATH;
	m_strFGN.strXUIElementName = _T("ForegroundNormal");
	m_strFGN.strPropWndName = _T("Normal Image");

	m_strFGH = _T("");
	m_strFGH.vtEx = VT_EX_BITMAPPATH;
	m_strFGH.strXUIElementName = _T("ForegroundHighlight");
	m_strFGH.strPropWndName = _T("Highlight Image");

	m_strFGS = _T("");
	m_strFGS.vtEx = VT_EX_BITMAPPATH;
	m_strFGS.strXUIElementName = _T("ForegroundSelected");
	m_strFGS.strPropWndName = _T("Selected Image");

	m_strFGD = _T("");
	m_strFGD.vtEx = VT_EX_BITMAPPATH;
	m_strFGD.strXUIElementName = _T("ForegroundDisabled");
	m_strFGD.strPropWndName = _T("Disabled Image");

	m_eForegroundHorAlignMode.varDefaultValue = m_eForegroundHorAlignMode = FAH_CENTER;
	m_eForegroundHorAlignMode.vtEx = VT_EX_FOREGROUND_ALIGN_HOR;
	m_eForegroundHorAlignMode.bReadOnly = true;
	m_eForegroundHorAlignMode.strUIKeyName = _T("ForegroundHorAlignMode");
	m_eForegroundHorAlignMode.strXUIElementName = _T("ForegroundAlignMode");
	m_eForegroundHorAlignMode.strXUIAttributeName = _T("Horizontal");
	m_eForegroundHorAlignMode.strPropWndName = _T("Horizontal Alignment");
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_LEFT).GetEnumString());
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_RIGHT).GetEnumString());
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_CENTER).GetEnumString());
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_STRETCH).GetEnumString());

	m_eForegroundVerAlignMode.varDefaultValue = m_eForegroundVerAlignMode = FAV_CENTER;
	m_eForegroundVerAlignMode.vtEx = VT_EX_FOREGROUND_ALIGN_VER;
	m_eForegroundVerAlignMode.bReadOnly = true;
	m_eForegroundVerAlignMode.strUIKeyName = _T("ForegroundVerAlignMode");
	m_eForegroundVerAlignMode.strXUIElementName = _T("ForegroundAlignMode");
	m_eForegroundVerAlignMode.strXUIAttributeName = _T("Vertical");
	m_eForegroundVerAlignMode.strPropWndName = _T("Vertical Alignment");
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_TOP).GetEnumString());
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_BOTTOM).GetEnumString());
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_CENTER).GetEnumString());
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_STRETCH).GetEnumString());

	m_rcPadding4Foreground.vtEx = VT_EX_RECT;
	m_rcPadding4Foreground.varDefaultValue = m_rcPadding4Foreground = CRect(0, 0, 0, 0);
	m_rcPadding4Foreground.strPropWndName = _T("Foreground Margin");
	m_rcPadding4Foreground.strXUIElementName = _T("ForegroundMargin");
	m_rcPadding4Foreground.nMinValue = -3840;
	m_rcPadding4Foreground.nMaxValue = 3840;
}

BTNBASEPROPERTIES::~BTNBASEPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

OLDBTNBASEPROPERTIES::OLDBTNBASEPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(OLDBTNBASEPROPERTIES);

	m_lHorAlign.varDefaultValue = m_lHorAlign = (LONG)0;
	m_lHorAlign.strUIKeyName = _T("HorAlig");
	m_lHorAlign.strXUIElementName = _T("TextAlignment");
	m_lHorAlign.strXUIAttributeName = _T("HorOffset");
	m_lHorAlign.strPropWndName = _T("Horizontal Offset");
	m_lHorAlign.nMinValue = -1920;
	m_lHorAlign.nMaxValue = 1920;

	m_lVerAlign.varDefaultValue = m_lVerAlign = (LONG)0;
	m_lVerAlign.strUIKeyName = _T("VerAlig");
	m_lVerAlign.strXUIElementName = _T("TextAlignment");
	m_lVerAlign.strXUIAttributeName = _T("VerOffset");
	m_lVerAlign.strPropWndName = _T("Vertical Offset");
	m_lVerAlign.nMinValue = -1920;
	m_lVerAlign.nMaxValue = 1920;
}

OLDBTNBASEPROPERTIES::~OLDBTNBASEPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

BTNPROPERTIES::BTNPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(BTNPROPERTIES);
	m_eControlType = CT_PUSHBUTTON;

	m_eBtnType.varDefaultValue = m_eBtnType = BT_NORMAL;
	m_eBtnType.vtEx = VT_EX_BUTTON_TYPE;
	m_eBtnType.bReadOnly = true;
	m_eBtnType.bGroup = true;
	m_eBtnType.strGroupName = _T("General");
	m_eBtnType.strUIKeyName = m_eBtnType.strXUIElementName = _T("ButtonType");
	m_eBtnType.strPropWndName = _T("Button Type");
	m_eBtnType.lstStringOptions.AddTail(IUI_VARIANT(BT_NORMAL).GetEnumString());
	m_eBtnType.lstStringOptions.AddTail(IUI_VARIANT(BT_MINIMIZEBOX).GetEnumString());
	m_eBtnType.lstStringOptions.AddTail(IUI_VARIANT(BT_CLOSEBOX).GetEnumString());

	m_bDefPushButton.varDefaultValue = m_bDefPushButton = false;
	m_bDefPushButton.strUIKeyName = m_bDefPushButton.strXUIElementName = _T("IsDefPushButton");
	m_bDefPushButton.strPropWndName = _T("Default Button");
}

BTNPROPERTIES::~BTNPROPERTIES()
{

}

TOGGLEBTNPROPERTIES::TOGGLEBTNPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(TOGGLEBTNPROPERTIES);

	m_strBKCN = _T("");
	m_strBKCN.bGroup = true;
	m_strBKCN.strGroupName = _T("Background");
	m_strBKCN.vtEx = VT_EX_BITMAPPATH;
	m_strBKCN.strUIKeyName = _T("BKCN");
	m_strBKCN.strXUIElementName = _T("BackgroundCheckedNormal");
	m_strBKCN.strXUIAttributeName = _T("RelativePath");
	m_strBKCN.strPropWndName = _T("Checked Normal Image");

	m_strBKCH = _T("");
	m_strBKCH.vtEx = VT_EX_BITMAPPATH;
	m_strBKCH.strUIKeyName = _T("BKCH");
	m_strBKCH.strXUIElementName = _T("BackgroundCheckedHighlight");
	m_strBKCH.strXUIAttributeName = _T("RelativePath");
	m_strBKCH.strPropWndName = _T("Checked Highlight Image");

	m_strBKCS = _T("");
	m_strBKCS.vtEx = VT_EX_BITMAPPATH;
	m_strBKCS.strUIKeyName = _T("BKCS");
	m_strBKCS.strXUIElementName = _T("BackgroundCheckedSelected");
	m_strBKCS.strXUIAttributeName = _T("RelativePath");
	m_strBKCS.strPropWndName = _T("Checked Selected Image");

	m_strBKCD = _T("");
	m_strBKCD.vtEx = VT_EX_BITMAPPATH;
	m_strBKCD.strUIKeyName = _T("BKCD");
	m_strBKCD.strXUIElementName = _T("BackgroundCheckedDisabled");
	m_strBKCD.strXUIAttributeName = _T("RelativePath");
	m_strBKCD.strPropWndName = _T("Checked Disabled Image");

	m_strFGCN = _T("");
	m_strFGCN.bGroup = true;
	m_strFGCN.strGroupName = _T("Foreground");
	m_strFGCN.vtEx = VT_EX_BITMAPPATH;
	m_strFGCN.strXUIElementName = _T("ForegroundCheckedNormal");
	m_strFGCN.strXUIAttributeName = _T("RelativePath");
	m_strFGCN.strPropWndName = _T("Checked Normal Image");

	m_strFGCH = _T("");
	m_strFGCH.vtEx = VT_EX_BITMAPPATH;
	m_strFGCH.strXUIElementName = _T("ForegroundCheckedHighlight");
	m_strFGCH.strXUIAttributeName = _T("RelativePath");
	m_strFGCH.strPropWndName = _T("Checked Highlight Image");

	m_strFGCS = _T("");
	m_strFGCS.vtEx = VT_EX_BITMAPPATH;
	m_strFGCS.strXUIElementName = _T("ForegroundCheckedSelected");
	m_strFGCS.strXUIAttributeName = _T("RelativePath");
	m_strFGCS.strPropWndName = _T("Checked Selected Image");

	m_strFGCD = _T("");
	m_strFGCD.vtEx = VT_EX_BITMAPPATH;
	m_strFGCD.strXUIElementName = _T("ForegroundCheckedDisabled");
	m_strFGCD.strXUIAttributeName = _T("RelativePath");
	m_strFGCD.strPropWndName = _T("Checked Disabled Image");

	CTRLPROPERTIES::InitBoolVariant(&m_bCNSameNFont, true, _T("IsCNSameNFont"), _T("Checked Normal Font Same as Normal"), true, m_strResFontIDN.strGroupName);
	CTRLPROPERTIES::InitResFontIDVariant(&m_strResFontIDCN, _T("FontResCN"), _T("Checked Normal"));

	CTRLPROPERTIES::InitBoolVariant(&m_bCHSameNFont, true, _T("IsCHSameNFont"), _T("Checked Highlight Font Same as Normal"));
	CTRLPROPERTIES::InitResFontIDVariant(&m_strResFontIDCH, _T("FontResCH"), _T("Checked Highlight"));

	CTRLPROPERTIES::InitBoolVariant(&m_bCSSameNFont, true, _T("IsCSSameNFont"), _T("Checked Selected Font Same as Normal"));
	CTRLPROPERTIES::InitResFontIDVariant(&m_strResFontIDCS, _T("FontResCS"), _T("Checked Selected"));

	CTRLPROPERTIES::InitBoolVariant(&m_bCDSameNFont, true, _T("IsCDSameNFont"), _T("Checked Disabled Font Same as Normal"));
	CTRLPROPERTIES::InitResFontIDVariant(&m_strResFontIDCD, _T("FontResCD"), _T("Checked Disabled"));

	CTRLPROPERTIES::InitBoolVariant(&m_bCNSameNColor, true, _T("IsCNSameNColor"), _T("Checked Normal Color Same as Normal"), true, m_strResColorIDN.strGroupName);
	CTRLPROPERTIES::InitResColorIDVariant(&m_strResColorIDCN, _T("ColorResCN"), _T("Checked Normal"));

	CTRLPROPERTIES::InitBoolVariant(&m_bCHSameNColor, true, _T("IsCHSameNColor"), _T("Checked Highlight Color Same as Normal"));
	CTRLPROPERTIES::InitResColorIDVariant(&m_strResColorIDCH, _T("ColorResCH"), _T("Checked Highlight"));

	CTRLPROPERTIES::InitBoolVariant(&m_bCSSameNColor, true, _T("IsCSSameNColor"), _T("Checked Selected Color Same as Normal"));
	CTRLPROPERTIES::InitResColorIDVariant(&m_strResColorIDCS, _T("ColorResCS"), _T("Checked Selected"));

	CTRLPROPERTIES::InitBoolVariant(&m_bCDSameNColor, true, _T("IsCDSameNColor"), _T("Checked Disabled Color Same as Normal"));
	CTRLPROPERTIES::InitResColorIDVariant(&m_strResColorIDCD, _T("ColorResCD"), _T("Checked Disabled"));
}

TOGGLEBTNPROPERTIES::~TOGGLEBTNPROPERTIES()
{

}

CHKPROPERTIES::CHKPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(CHKPROPERTIES);
	m_eControlType = CT_CHECK;

	m_eChkType.varDefaultValue = m_eChkType = CT_NORMAL;
	m_eChkType.vtEx = VT_EX_CHECKBOX_TYPE;
	m_eChkType.bReadOnly = true;
	m_eChkType.bGroup = true;
	m_eChkType.strGroupName = _T("General");
	m_eChkType.strUIKeyName = m_eChkType.strXUIElementName = _T("CheckBoxType");
	m_eChkType.strPropWndName = _T("CheckBox Type");
	m_eChkType.lstStringOptions.AddTail(IUI_VARIANT(CT_NORMAL).GetEnumString());
	m_eChkType.lstStringOptions.AddTail(IUI_VARIANT(CT_MAXIMIZEBOX).GetEnumString());
}

CHKPROPERTIES::~CHKPROPERTIES()
{

}

RADPROPERTIES::RADPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(RADPROPERTIES);
	m_eControlType = CT_RADIO;

	m_bGroup.varDefaultValue = m_bGroup = false;
	m_bGroup.bGroup = true;
	m_bGroup.strGroupName = _T("General");
	m_bGroup.strUIKeyName = m_bGroup.strXUIElementName = _T("IsGroup");
	m_bGroup.strPropWndName = _T("Group");
	m_bGroup.strDescription = _T("");

	// The task window ID while the radio button as a tab of CTaskWndMgr.
	m_strBindTaskWndID.varDefaultValue = m_strBindTaskWndID = _T("");
	m_strBindTaskWndID.vtEx = VT_EX_WINDOW_ID_NOT_INCLUDE_SELF;
	m_strBindTaskWndID.strXUIElementName = _T("BindTaskWndID");
	m_strBindTaskWndID.strPropWndName = _T("Bind Task Window ID");

	// The parent ID of the task window. A window may be include more then one CTaskWndMgr
	m_strBindTaskMgrID.varDefaultValue = m_strBindTaskMgrID = _T("");
	m_strBindTaskMgrID.vtEx = VT_EX_TASKMGR_ID;
	m_strBindTaskMgrID.strXUIElementName = _T("BindTaskMgrID");
	m_strBindTaskMgrID.strPropWndName = _T("Bind TaskMgr ID");
}

RADPROPERTIES::~RADPROPERTIES()
{

}

TXTPROPERTIES::TXTPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(TXTPROPERTIES);
	m_eControlType = CT_STATIC;

	m_bR2LReadOrder.varDefaultValue = m_bR2LReadOrder = false;
	m_bR2LReadOrder.bGroup = true;
	m_bR2LReadOrder.strGroupName = _T("General");
	m_bR2LReadOrder.strVarName = _T("m_bR2LReadOrder");
	m_bR2LReadOrder.strUIKeyName = m_bR2LReadOrder.strXUIElementName = _T("IsR2LReadOrder");
	m_bR2LReadOrder.strPropWndName = _T("Right to Left Reading Order");
	m_bR2LReadOrder.strDescription = _T("For languages that support reading order alignment, specifies right-to-left reading order.");

	m_bNoWrap.varDefaultValue = m_bNoWrap = true;
	m_bNoWrap.strUIKeyName = m_bNoWrap.strXUIElementName = _T("Nowrap");
	m_bNoWrap.strPropWndName = _T("Nowrap");
	m_bNoWrap.strDescription = _T("");

	m_bPathEllipsis.varDefaultValue = m_bPathEllipsis = false;
	m_bPathEllipsis.strUIKeyName = m_bPathEllipsis.strXUIElementName = _T("IsPathEllipsis");
	m_bPathEllipsis.strPropWndName = _T("Path Ellipsis");
	m_bPathEllipsis.strDescription = _T("");

	m_bEndEllipsis.varDefaultValue = m_bEndEllipsis = false;
	m_bEndEllipsis.strXUIElementName = _T("IsEndEllipsis");
	m_bEndEllipsis.strPropWndName = _T("End Ellipsis");
	m_bEndEllipsis.strDescription = _T("");

	m_strCaption.varDefaultValue = m_strCaption = _T("");
	m_strCaption.strUIKeyName = m_strCaption.strXUIElementName = _T("Caption");
	m_strCaption.strPropWndName = _T("Caption");
	m_strCaption.strDescription = _T("");

	// Foreground
	m_bSpecifyForegroundImages.varDefaultValue = m_bSpecifyForegroundImages = false;
	m_bSpecifyForegroundImages.bGroup = true;
	m_bSpecifyForegroundImages.strGroupName = _T("Foreground");
	m_bSpecifyForegroundImages.strUIKeyName = m_bSpecifyForegroundImages.strXUIElementName = _T("IsSpecifyForegroundImages");
	m_bSpecifyForegroundImages.strPropWndName = _T("Specify Images");

	m_bCombineForegroundImages.varDefaultValue = m_bCombineForegroundImages = false;
	m_bCombineForegroundImages.strUIKeyName = m_bCombineForegroundImages.strXUIElementName = _T("IsCombineForegroundImages");
	m_bCombineForegroundImages.strPropWndName = _T("Combine Images");

	m_strFGCombine = _T("");
	m_strFGCombine.vtEx = VT_EX_BITMAPPATH;
	m_strFGCombine.strXUIElementName = _T("ForeGroundCombine");
	m_strFGCombine.strPropWndName = _T("CombineImage");

	m_strFGN = _T("");
	m_strFGN.vtEx = VT_EX_BITMAPPATH;
	m_strFGN.strXUIElementName = _T("ForegroundNormal");
	m_strFGN.strPropWndName = _T("Normal Image");

	m_strFGD = _T("");
	m_strFGD.vtEx = VT_EX_BITMAPPATH;
	m_strFGD.strXUIElementName = _T("ForegroundDisabled");
	m_strFGD.strPropWndName = _T("Disabled Image");

	m_eForegroundHorAlignMode.varDefaultValue = m_eForegroundHorAlignMode = FAH_LEFT;
	m_eForegroundHorAlignMode.vtEx = VT_EX_FOREGROUND_ALIGN_HOR;
	m_eForegroundHorAlignMode.bReadOnly = true;
	m_eForegroundHorAlignMode.strUIKeyName = _T("ForegroundHorAlignMode");
	m_eForegroundHorAlignMode.strXUIElementName = _T("ForegroundAlignMode");
	m_eForegroundHorAlignMode.strXUIAttributeName = _T("Horizontal");
	m_eForegroundHorAlignMode.strPropWndName = _T("Horizontal Alignment");
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_LEFT).GetEnumString());
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_RIGHT).GetEnumString());
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_CENTER).GetEnumString());
	m_eForegroundHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAH_STRETCH).GetEnumString());

	m_eForegroundVerAlignMode.varDefaultValue = m_eForegroundVerAlignMode = FAV_CENTER;
	m_eForegroundVerAlignMode.vtEx = VT_EX_FOREGROUND_ALIGN_VER;
	m_eForegroundVerAlignMode.bReadOnly = true;
	m_eForegroundVerAlignMode.strUIKeyName = _T("ForegroundVerAlignMode");
	m_eForegroundVerAlignMode.strXUIElementName = _T("ForegroundAlignMode");
	m_eForegroundVerAlignMode.strXUIAttributeName = _T("Vertical");
	m_eForegroundVerAlignMode.strPropWndName = _T("Vertical Alignment");
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_TOP).GetEnumString());
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_BOTTOM).GetEnumString());
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_CENTER).GetEnumString());
	m_eForegroundVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(FAV_STRETCH).GetEnumString());

	m_rcPadding4Foreground.vtEx = VT_EX_RECT;
	m_rcPadding4Foreground.varDefaultValue = m_rcPadding4Foreground = CRect(0, 0, 0, 0);
	m_rcPadding4Foreground.strPropWndName = _T("Foreground Margin");
	m_rcPadding4Foreground.strXUIElementName = _T("ForegroundMargin");
	m_rcPadding4Foreground.nMinValue = -3840;
	m_rcPadding4Foreground.nMaxValue = 3840;

	// ToolTip
	m_bUseToolTip.varDefaultValue = m_bUseToolTip = false;
	m_bUseToolTip.bGroup = true;
	m_bUseToolTip.strVarName = _T("m_bUseToolTip");
	m_bUseToolTip.strGroupName = _T("ToolTip");
	m_bUseToolTip.strUIKeyName = _T("UseToolTip");
	m_bUseToolTip.strXUIElementName = _T("ToolTip");
	m_bUseToolTip.strXUIAttributeName = _T("Enable");
	m_bUseToolTip.strPropWndName = _T("Enable Tooltip");
	m_bUseToolTip.strDescription = _T("Enable Tooltip.");

	m_strToolTip.varDefaultValue = m_strToolTip = _T("");
	m_strToolTip.strUIKeyName = _T("ToolTips");
	m_strToolTip.strXUIElementName = m_strToolTip.strXUIAttributeName = _T("ToolTip");
	m_strToolTip.strPropWndName = _T("ToolTip");
	m_strToolTip.strDescription = _T("");

	// Background
	m_strBKH.bShowInPropertiesWnd = m_strBKH.bSaveToSkinFile = false;
	m_strBKS.bShowInPropertiesWnd = m_strBKS.bSaveToSkinFile = false;

	// font
	DisableFontProperties(TRUE);

	// color
	DisableColorProperties(TRUE);

	// alignment
	m_eTextHorAlignMode.varDefaultValue = m_eTextHorAlignMode = TAH_DEFAULT;
	m_eTextHorAlignMode.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignMode.bGroup = true;
	m_eTextHorAlignMode.strGroupName = _T("Text Alignment");
	m_eTextHorAlignMode.bReadOnly = true;
	m_eTextHorAlignMode.strUIKeyName = _T("HorAlignMode");
	m_eTextHorAlignMode.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignMode.strXUIAttributeName = _T("Horizontal");
	m_eTextHorAlignMode.strPropWndName = _T("Horizontal");
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	m_eTextVerAlignMode.varDefaultValue = m_eTextVerAlignMode = TAV_CENTER;
	m_eTextVerAlignMode.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignMode.bReadOnly = true;
	m_eTextVerAlignMode.strUIKeyName = _T("VerAlignMode");
	m_eTextVerAlignMode.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignMode.strXUIAttributeName = _T("Vertical");
	m_eTextVerAlignMode.strPropWndName = _T("Vertical");
	m_eTextVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());
	m_eTextVerAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());

	m_rcPadding4Text.vtEx = VT_EX_RECT;
	m_rcPadding4Text.varDefaultValue = m_rcPadding4Text = CRect(0, 0, 0, 0);
	m_rcPadding4Text.strPropWndName = _T("Text Margin");
	m_rcPadding4Text.strXUIElementName = _T("TextMargin");
	m_rcPadding4Text.nMinValue = -3840;
	m_rcPadding4Text.nMaxValue = 3840;

	m_bShadowText.varDefaultValue = m_bShadowText = false;
	m_bShadowText.bGroup = true;
	m_bShadowText.strVarName = _T("m_bShadowText");
	m_bShadowText.strGroupName = _T("Shadow Text");
	m_bShadowText.strXUIElementName = _T("ShadowText");
	m_bShadowText.strXUIAttributeName = _T("Enable");
	m_bShadowText.strPropWndName = _T("Enable Shadow Text");
	m_bShadowText.strDescription = _T("Enable Shadow Text.");

	m_crTextShadow.varDefaultValue = m_crTextShadow = (LONG)RGB(192, 192, 192);
	m_crTextShadow.vtEx = VT_EX_COLOR;
	m_crTextShadow.strXUIElementName = m_bShadowText.strXUIElementName;
	m_crTextShadow.strXUIAttributeName = _T("ShadowColor");
	m_crTextShadow.strPropWndName = _T("Text Shadow Color");

	m_ptTextShadowOffset.varDefaultValue = m_ptTextShadowOffset = CPoint(1, 1);
	m_ptTextShadowOffset.vtEx = VT_EX_POINT;
	m_ptTextShadowOffset.strXUIElementName = m_bShadowText.strXUIElementName;
	m_ptTextShadowOffset.strXUIAttributeName = _T("ShadowOffset");
	m_ptTextShadowOffset.strPropWndName = _T("Text Shadow offset");
	m_ptTextShadowOffset.nMinValue = -1;
	m_ptTextShadowOffset.nMaxValue = 8;
}

TXTPROPERTIES::~TXTPROPERTIES()
{

}

EDTPROPERTIES::EDTPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(EDTPROPERTIES);
	m_eControlType = CT_EDIT;
	m_bTabStop = true;
	m_strBindStyle.vtEx = VT_EX_STYLE_EDIT;


	m_bHScroll.bGroup = true;
	m_bHScroll.strGroupName = _T("General");
	m_bHScroll.varDefaultValue = m_bHScroll = false;
	m_bHScroll.strUIKeyName = m_bHScroll.strXUIElementName = m_bHScroll.strPropWndName = _T("IsHScroll");

	m_bVScroll.varDefaultValue = m_bVScroll = false;
	m_bVScroll.strUIKeyName = m_bVScroll.strXUIElementName = m_bVScroll.strPropWndName = _T("IsVScroll");

	m_bAutoHScroll.varDefaultValue = m_bAutoHScroll = true;
	m_bAutoHScroll.strUIKeyName = m_bAutoHScroll.strXUIElementName = m_bAutoHScroll.strPropWndName = _T("IsAutoHScroll");

	m_bAutoVScroll.varDefaultValue = m_bAutoVScroll = false;
	m_bAutoVScroll.strUIKeyName = m_bAutoVScroll.strXUIElementName = m_bAutoVScroll.strPropWndName = _T("IsAutoVScroll");

	m_bDisableNoScroll.varDefaultValue = m_bDisableNoScroll = false;
	m_bDisableNoScroll.strUIKeyName = m_bDisableNoScroll.strXUIElementName = m_bDisableNoScroll.strPropWndName = _T("IsDisableNoScroll");

	m_bLowercase.varDefaultValue = m_bLowercase = false;
	m_bLowercase.strUIKeyName = m_bLowercase.strXUIElementName = m_bLowercase.strPropWndName = _T("IsLowerCase");

	m_bUppercase.varDefaultValue = m_bUppercase = false;
	m_bUppercase.strUIKeyName = m_bUppercase.strXUIElementName = m_bUppercase.strPropWndName = _T("IsUpperCase");

	m_bNumber.varDefaultValue = m_bNumber = false;
	m_bNumber.strUIKeyName = m_bNumber.strXUIElementName = m_bNumber.strPropWndName = _T("IsNumber");

	m_bPassword.varDefaultValue = m_bPassword = false;
	m_bPassword.strUIKeyName = m_bPassword.strXUIElementName = m_bPassword.strPropWndName = _T("IsPassword");

	m_bReadOnly.varDefaultValue = m_bReadOnly = false;
	m_bReadOnly.strUIKeyName = m_bReadOnly.strXUIElementName = m_bReadOnly.strPropWndName = _T("IsReadOnly");

	m_bMultiline.varDefaultValue = m_bMultiline = false;
	m_bMultiline.strUIKeyName = m_bMultiline.strXUIElementName = m_bMultiline.strPropWndName = _T("IsMultiline");

	m_bWantReturn.varDefaultValue = m_bWantReturn = false;
	m_bWantReturn.strUIKeyName = m_bWantReturn.strXUIElementName = m_bWantReturn.strPropWndName = _T("IsWantReturn");

	m_bR2LReadOrder.varDefaultValue = m_bR2LReadOrder = false;
	m_bR2LReadOrder.strVarName = _T("m_bR2LReadOrder");
	m_bR2LReadOrder.strUIKeyName = m_bR2LReadOrder.strXUIElementName = _T("IsR2LReadOrder");
	m_bR2LReadOrder.strPropWndName = _T("Right to Left Reading Order");
	m_bR2LReadOrder.strDescription = _T("For languages that support reading order alignment, specifies right-to-left reading order.");

	m_strTag.varDefaultValue = m_strTag = _T("");
	m_strTag.strUIKeyName = m_strTag.strXUIElementName = _T("Tag");
	m_strTag.strPropWndName = _T("Tag");
	m_strTag.strDescription = _T("");

	// Text Alignment
	m_eTextHorAlignMode.varDefaultValue = m_eTextHorAlignMode = TAH_DEFAULT;
	m_eTextHorAlignMode.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignMode.bReadOnly = true;
	m_eTextHorAlignMode.bGroup = true;
	m_eTextHorAlignMode.strGroupName = _T("Text Alignment");
	m_eTextHorAlignMode.strUIKeyName = _T("HorAlignMode");
	m_eTextHorAlignMode.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignMode.strXUIAttributeName = _T("Horizontal");
	m_eTextHorAlignMode.strPropWndName = _T("Horizontal");
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignMode.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	m_rcPadding.vtEx = VT_EX_RECT;
	m_rcPadding.varDefaultValue = m_rcPadding = CRect(0, 0, 0, 0);
	m_rcPadding.strPropWndName = _T("Padding for Text");
	m_rcPadding.strXUIElementName = _T("TextMargin");
	m_rcPadding.nMinValue = -3840;
	m_rcPadding.nMaxValue = 3840;
}

EDTPROPERTIES::~EDTPROPERTIES()
{

}

RICHEDITPROPERTIES::RICHEDITPROPERTIES() : EDTPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(RICHEDITPROPERTIES);
	m_eControlType = CT_RICHEDIT;
}

RICHEDITPROPERTIES::~RICHEDITPROPERTIES()
{

}

IMREPROPERTIES::IMREPROPERTIES() : RICHEDITPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(IMREPROPERTIES);
	m_eControlType = CT_RICHEDIT_IM;
	m_bHScroll.bSaveToSkinFile = m_bHScroll.bShowInPropertiesWnd = false;
	m_bVScroll.bSaveToSkinFile = m_bVScroll.bShowInPropertiesWnd = false;
	m_bAutoHScroll.bSaveToSkinFile = m_bAutoHScroll.bShowInPropertiesWnd = false;
	m_bAutoVScroll.bSaveToSkinFile = m_bAutoVScroll.bShowInPropertiesWnd = false;
	m_bLowercase.bSaveToSkinFile = m_bLowercase.bShowInPropertiesWnd = false;
	m_bUppercase.bSaveToSkinFile = m_bUppercase.bShowInPropertiesWnd = false;
	m_bNumber.bSaveToSkinFile = m_bNumber.bShowInPropertiesWnd = false;
	m_bPassword.bSaveToSkinFile = m_bPassword.bShowInPropertiesWnd = false;
	m_bReadOnly.bSaveToSkinFile = m_bReadOnly.bShowInPropertiesWnd = false;
	m_bMultiline.bSaveToSkinFile = m_bMultiline.bShowInPropertiesWnd = false;
	m_bWantReturn.bSaveToSkinFile = m_bWantReturn.bShowInPropertiesWnd = false;
	m_eTextHorAlignMode.bSaveToSkinFile = m_eTextHorAlignMode.bShowInPropertiesWnd = false;

	m_bDisableNoScroll.bGroup = true;
	m_bDisableNoScroll.strGroupName = _T("General");

	m_strResFontIDN.strPropWndName = _T("Default Font");
	m_bHSameNFont.bSaveToSkinFile = m_bHSameNFont.bShowInPropertiesWnd = false;
	m_strResFontIDH.bSaveToSkinFile = m_strResFontIDH.bShowInPropertiesWnd = false;
	m_bSSameNFont.bSaveToSkinFile = m_bSSameNFont.bShowInPropertiesWnd = false;
	m_strResFontIDS.bSaveToSkinFile = m_strResFontIDS.bShowInPropertiesWnd = false;
	m_bDSameNFont.bSaveToSkinFile = m_bDSameNFont.bShowInPropertiesWnd = false;
	m_strResFontIDD.bSaveToSkinFile = m_strResFontIDD.bShowInPropertiesWnd = false;

	m_strResColorIDN.strPropWndName = _T("Default Color");
	m_bHSameNColor.bSaveToSkinFile = m_bHSameNColor.bShowInPropertiesWnd = false;
	m_strResColorIDH.bSaveToSkinFile = m_strResColorIDH.bShowInPropertiesWnd = false;
	m_bSSameNColor.bSaveToSkinFile = m_bSSameNColor.bShowInPropertiesWnd = false;
	m_strResColorIDS.bSaveToSkinFile = m_strResColorIDS.bShowInPropertiesWnd = false;
	m_bDSameNColor.bSaveToSkinFile = m_bDSameNColor.bShowInPropertiesWnd = false;
	m_strResColorIDD.bSaveToSkinFile = m_strResColorIDD.bShowInPropertiesWnd = false;
}

IMREPROPERTIES::~IMREPROPERTIES()
{

}

CMBPROPERTIES::CMBPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(CMBPROPERTIES);
	m_eControlType = CT_COMBOBOX;

	m_bTabStop.varDefaultValue = m_bTabStop = true;

	m_crN.strPropWndName = _T("Color");
	m_strResColorIDN.strPropWndName = _T("Color");
	DisableColorProperties(TRUE);
	m_bDSameNColor.bSaveToSkinFile = m_bDSameNColor.bShowInPropertiesWnd = false;
	m_crD.bSaveToSkinFile = m_crD.bShowInPropertiesWnd = false;
	m_strResColorIDD.bSaveToSkinFile = m_strResColorIDD.bShowInPropertiesWnd = false;

	m_eComboStyle.varDefaultValue = m_eComboStyle = CS_DROPDOWN;
	m_eComboStyle.vtEx = VT_EX_COMBOBOX_STYLE;
	m_eComboStyle.bGroup = true;
	m_eComboStyle.strGroupName = _T("General");
	m_eComboStyle.strUIKeyName = m_eComboStyle.strXUIElementName = _T("ComboStyle");
	m_eComboStyle.strPropWndName = _T("Style");
	m_eComboStyle.lstStringOptions.AddTail(IUI_VARIANT(CS_SIMPLE).GetEnumString());
	m_eComboStyle.lstStringOptions.AddTail(IUI_VARIANT(CS_DROPDOWN).GetEnumString());
	m_eComboStyle.lstStringOptions.AddTail(IUI_VARIANT(CS_DROPDOWNLIST).GetEnumString());

	m_bVScroll.varDefaultValue = m_bVScroll = true;
	m_bVScroll.strUIKeyName = m_bVScroll.strXUIElementName = _T("IsVScroll");
	m_bVScroll.strPropWndName = _T("Vertical Scroll");

	m_bAutoHScroll.varDefaultValue = m_bAutoHScroll = true;
	m_bAutoHScroll.strUIKeyName = m_bAutoHScroll.strXUIElementName = _T("IsAutoHScroll");
	m_bAutoHScroll.strPropWndName = _T("Auto HScroll");

	m_bSort.varDefaultValue = m_bSort = false;
	m_bSort.strXUIElementName = _T("IsSort");
	m_bSort.strPropWndName = _T("Sort");

	m_bDisableNoScroll.varDefaultValue = m_bDisableNoScroll = false;
	m_bDisableNoScroll.strUIKeyName = m_bDisableNoScroll.strXUIElementName = _T("IsDisableNoScroll");
	m_bDisableNoScroll.strPropWndName = _T("Disable No Scroll");

	m_bLowercase.varDefaultValue = m_bLowercase = false;
	m_bLowercase.strUIKeyName = m_bLowercase.strXUIElementName = m_bLowercase.strPropWndName = _T("IsLowerCase");

	m_bUppercase.varDefaultValue = m_bUppercase = false;
	m_bUppercase.strUIKeyName = m_bUppercase.strXUIElementName = m_bUppercase.strPropWndName = _T("IsUpperCase");

	// Text horizontal alignment
	m_eTextHorAlignModeN.varDefaultValue = m_eTextHorAlignModeN = TAH_DEFAULT;
	m_eTextHorAlignModeN.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignModeN.bReadOnly = true;
	m_eTextHorAlignModeN.bGroup = true;
	m_eTextHorAlignModeN.strGroupName = _T("Text Horizontal Align");
	m_eTextHorAlignModeN.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignModeN.strXUIAttributeName = _T("HorizontalN");
	m_eTextHorAlignModeN.strPropWndName = _T("Normal");
	m_eTextHorAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	m_eTextHorAlignModeH.varDefaultValue = m_eTextHorAlignModeH = TAH_DEFAULT;
	m_eTextHorAlignModeH.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignModeH.bReadOnly = true;
	m_eTextHorAlignModeH.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignModeH.strXUIAttributeName = _T("HorizontalH");
	m_eTextHorAlignModeH.strPropWndName = _T("Highlight");
	m_eTextHorAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	m_eTextHorAlignModeS.varDefaultValue = m_eTextHorAlignModeS = TAH_DEFAULT;
	m_eTextHorAlignModeS.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignModeS.bReadOnly = true;
	m_eTextHorAlignModeS.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignModeS.strXUIAttributeName = _T("HorizontalS");
	m_eTextHorAlignModeS.strPropWndName = _T("Selected");
	m_eTextHorAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	m_eTextHorAlignModeD.varDefaultValue = m_eTextHorAlignModeD = TAH_DEFAULT;
	m_eTextHorAlignModeD.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignModeD.bReadOnly = true;
	m_eTextHorAlignModeD.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignModeD.strXUIAttributeName = _T("HorizontalD");
	m_eTextHorAlignModeD.strPropWndName = _T("Disabled");
	m_eTextHorAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	// Text vertical alignment
	m_eTextVerAlignModeN.varDefaultValue = m_eTextVerAlignModeN = TAV_DEFAULT;
	m_eTextVerAlignModeN.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignModeN.bReadOnly = true;
	m_eTextVerAlignModeN.bGroup = true;
	m_eTextVerAlignModeN.strGroupName = _T("Text Verizontal Align");
	m_eTextVerAlignModeN.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignModeN.strXUIAttributeName = _T("VerticalN");
	m_eTextVerAlignModeN.strPropWndName = _T("Normal");
	m_eTextVerAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());
	m_eTextVerAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());

	m_eTextVerAlignModeH.varDefaultValue = m_eTextVerAlignModeH = TAV_DEFAULT;
	m_eTextVerAlignModeH.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignModeH.bReadOnly = true;
	m_eTextVerAlignModeH.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignModeH.strXUIAttributeName = _T("VerticalH");
	m_eTextVerAlignModeH.strPropWndName = _T("Highlight");
	m_eTextVerAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());
	m_eTextVerAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());

	m_eTextVerAlignModeS.varDefaultValue = m_eTextVerAlignModeS = TAV_DEFAULT;
	m_eTextVerAlignModeS.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignModeS.bReadOnly = true;
	m_eTextVerAlignModeS.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignModeS.strXUIAttributeName = _T("VerticalS");
	m_eTextVerAlignModeS.strPropWndName = _T("Selected");
	m_eTextVerAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());
	m_eTextVerAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());

	m_eTextVerAlignModeD.varDefaultValue = m_eTextVerAlignModeD = TAV_DEFAULT;
	m_eTextVerAlignModeD.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignModeD.bReadOnly = true;
	m_eTextVerAlignModeD.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignModeD.strXUIAttributeName = _T("VerticalD");
	m_eTextVerAlignModeD.strPropWndName = _T("Disabled");
	m_eTextVerAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());
	m_eTextVerAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());

	// Padding for Text
	m_rcTextMarginN.vtEx = VT_EX_RECT;
	m_rcTextMarginN.varDefaultValue = m_rcTextMarginN = CRect(1, 1, 1, 1);
	m_rcTextMarginN.bGroup = true;
	m_rcTextMarginN.strGroupName = _T("Item Text Margin");
	m_rcTextMarginN.strPropWndName = _T("Normal Item Text Margin");
	m_rcTextMarginN.strXUIElementName = _T("ItemTextMarginN");
	m_rcTextMarginN.nMinValue = -3840;
	m_rcTextMarginN.nMaxValue = 3840;

	m_rcTextMarginH.vtEx = VT_EX_RECT;
	m_rcTextMarginH.varDefaultValue = m_rcTextMarginH = CRect(1, 1, 1, 1);
	m_rcTextMarginH.strPropWndName = _T("Highlight Item Text Margin");
	m_rcTextMarginH.strXUIElementName = _T("ItemTextMarginH");
	m_rcTextMarginH.nMinValue = -3840;
	m_rcTextMarginH.nMaxValue = 3840;

	m_rcTextMarginS.vtEx = VT_EX_RECT;
	m_rcTextMarginS.varDefaultValue = m_rcTextMarginS = CRect(1, 1, 1, 1);
	m_rcTextMarginS.strPropWndName = _T("Selected Item Text Margin");
	m_rcTextMarginS.strXUIElementName = _T("ItemTextMarginS");
	m_rcTextMarginS.nMinValue = -3840;
	m_rcTextMarginS.nMaxValue = 3840;

	m_rcTextMarginD.vtEx = VT_EX_RECT;
	m_rcTextMarginD.varDefaultValue = m_rcTextMarginD = CRect(1, 1, 1, 1);
	m_rcTextMarginD.strPropWndName = _T("Disabled Item Text Margin");
	m_rcTextMarginD.strXUIElementName = _T("ItemTextMarginD");
	m_rcTextMarginD.nMinValue = -3840;
	m_rcTextMarginD.nMaxValue = 3840;

	m_lDropListItemHeight.varDefaultValue = m_lDropListItemHeight = (LONG)24;
	m_lDropListItemHeight.strXUIElementName = _T("DropListItemHeight");
	m_lDropListItemHeight.strPropWndName = _T("DropList Item Height");
	m_lDropListItemHeight.nMinValue = 1;
	m_lDropListItemHeight.nMaxValue = 255;

	m_lDropListHeight.varDefaultValue = m_lDropListHeight = (LONG)80;
	m_lDropListHeight.strUIKeyName = _T("DropListHeight");
	m_lDropListHeight.strXUIElementName = _T("DropListHeight");
	m_lDropListHeight.strPropWndName = _T("DropList Height");
	m_lDropListHeight.nMinValue = 0;
	m_lDropListHeight.nMaxValue = 1920;

	m_lBtnWidth.varDefaultValue = m_lBtnWidth = (LONG)16;
	m_lBtnWidth.strUIKeyName = _T("BtnWidth");
	m_lBtnWidth.strXUIElementName = _T("ButtonWidth");
	m_lBtnWidth.strPropWndName = _T("Button Width");
	m_lBtnWidth.nMinValue = 0;
	m_lBtnWidth.nMaxValue = 1920;

	m_strTag.varDefaultValue = m_strTag = _T("");
	m_strTag.strUIKeyName = m_strTag.strXUIElementName = _T("Tag");
	m_strTag.strPropWndName = _T("Tag");
	m_strTag.strDescription = _T("");
}

CMBPROPERTIES::~CMBPROPERTIES()
{

}

SPNPROPERTIES::SPNPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(SPNPROPERTIES);
	m_eControlType = CT_SPIN;
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_eAlignMode.varDefaultValue = m_eAlignMode = SPNA_UNATTACHED;
	m_eAlignMode.vtEx = VT_EX_SPN_ALIGN;
	m_eAlignMode.bGroup = true;
	m_eAlignMode.strGroupName = _T("General");
	m_eAlignMode.strUIKeyName = m_eAlignMode.strXUIElementName = _T("Alignment");
	m_eAlignMode.strPropWndName = _T("Alignment");
	m_eAlignMode.lstStringOptions.AddTail(IUI_VARIANT(SPNA_UNATTACHED).GetEnumString());
	m_eAlignMode.lstStringOptions.AddTail(IUI_VARIANT(SPNA_LEFT).GetEnumString());
	m_eAlignMode.lstStringOptions.AddTail(IUI_VARIANT(SPNA_RIGHT).GetEnumString());

	m_bVertical.varDefaultValue = m_bVertical = false;
	m_bVertical.strXUIElementName = _T("IsVertical");
	m_bVertical.strPropWndName = _T("Vertical");

	m_bAutoBuddy.varDefaultValue = m_bAutoBuddy = false;
	m_bAutoBuddy.strXUIElementName = _T("IsAutoBuddy");
	m_bAutoBuddy.strPropWndName = _T("Auto Buddy");

	m_bSetBuddyInt.varDefaultValue = m_bSetBuddyInt = false;
	m_bSetBuddyInt.strXUIElementName = _T("IsSetBuddyInt");
	m_bSetBuddyInt.strPropWndName = _T("Set Buddy Int");

	m_bNoThousands.varDefaultValue = m_bNoThousands = false;
	m_bNoThousands.strXUIElementName = _T("IsNoThousands");
	m_bNoThousands.strPropWndName = _T("No Thousands");

	m_bWrap.varDefaultValue = m_bWrap = false;
	m_bWrap.strXUIElementName = _T("IsWrap");
	m_bWrap.strPropWndName = _T("Wrap");

	m_bArrowKeys.varDefaultValue = m_bArrowKeys = true;
	m_bArrowKeys.strXUIElementName = _T("IsArrowKeys");
	m_bArrowKeys.strPropWndName = _T("Arrow Keys");

	m_bHotTrack.varDefaultValue = m_bHotTrack = false;
	m_bHotTrack.strXUIElementName = _T("IsHotTrack");
	m_bHotTrack.strPropWndName = _T("Hot Track");
}

SPNPROPERTIES::~SPNPROPERTIES()
{

}

SLDPROPERTIES::SLDPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(SLDPROPERTIES);
	m_eControlType = CT_SLIDER;

	// Background and mark
	m_strBKN.strPropWndName = _T("Normal Background");
	m_strBKH.strPropWndName = _T("Normal Mark");
	m_strBKH.strXUIElementName = _T("MarkNormal");
	m_strBKS.strPropWndName = _T("Disabled Mark");
	m_strBKS.strXUIElementName = _T("MarkDisabled");
	m_strBKD.strPropWndName = _T("Disabled Background");

	// Font
	DisableFontProperties(FALSE);

	// Text color
	DisableColorProperties(FALSE);

	m_eExtendedStyle.varDefaultValue = m_eExtendedStyle = SLDES_NORMAL;
	m_eExtendedStyle.bGroup = true;
	m_eExtendedStyle.strGroupName = _T("General");
	m_eExtendedStyle.strUIKeyName = m_eExtendedStyle.strXUIElementName = _T("ExtendedStyle");
	m_eExtendedStyle.strPropWndName = _T("Extended Style");
	m_eExtendedStyle.lstStringOptions.AddTail(IUI_VARIANT(SLDES_NORMAL).GetEnumString());
	m_eExtendedStyle.lstStringOptions.AddTail(IUI_VARIANT(SLDES_PROGRESS).GetEnumString());
	m_eExtendedStyle.lstStringOptions.AddTail(IUI_VARIANT(SLDES_SPLITPROGRESS).GetEnumString());

	m_bVertical.varDefaultValue = m_bVertical = false;
	m_bVertical.strXUIElementName = _T("IsVertical");
	m_bVertical.strPropWndName = _T("Vertical");

	m_bUseToolTip.varDefaultValue = m_bUseToolTip = false;
	m_bUseToolTip.strUIKeyName = _T("UseToolTip");
	m_bUseToolTip.strXUIElementName = _T("ToolTip");
	m_bUseToolTip.strXUIAttributeName = _T("Enable");
	m_bUseToolTip.strPropWndName = _T("Enable Tooltip");
	m_bUseToolTip.strDescription = _T("Enable Tooltip.");

	// Mark background
	m_bUseMark.varDefaultValue = m_bUseMark = true;
	m_bUseMark.strXUIElementName = _T("IsUseMark");
	m_bUseMark.strPropWndName = _T("Use Mark");

	m_bMarkToThumb.varDefaultValue = m_bMarkToThumb = false;
	m_bMarkToThumb.strXUIElementName = _T("IsMarkToThumb");
	m_bMarkToThumb.strPropWndName = _T("Mark To Thumb");

	// Thumb Images
	m_bCombineThumbImages.varDefaultValue = m_bCombineThumbImages = false;
	m_bCombineThumbImages.bGroup = true;
	m_bCombineThumbImages.strGroupName = _T("Thumb Images");
	m_bCombineThumbImages.strXUIElementName = _T("IsCombineThumbImages");
	m_bCombineThumbImages.strPropWndName = _T("CombineThumbImage");

	m_strThumbCombine = _T("");
	m_strThumbCombine.vtEx = VT_EX_BITMAPPATH;
	m_strThumbCombine.strXUIElementName = _T("ThumbCombine");
	m_strThumbCombine.strXUIAttributeName = _T("RelativePath");
	m_strThumbCombine.strPropWndName = _T("Combine Thumb");

	m_strThumbN = _T("");
	m_strThumbN.vtEx = VT_EX_BITMAPPATH;
	m_strThumbN.strXUIElementName = _T("ThumbN");
	m_strThumbN.strXUIAttributeName = _T("RelativePath");
	m_strThumbN.strPropWndName = _T("Normal");

	m_strThumbH = _T("");
	m_strThumbH.vtEx = VT_EX_BITMAPPATH;
	m_strThumbH.strXUIElementName = _T("ThumbH");
	m_strThumbH.strXUIAttributeName = _T("RelativePath");
	m_strThumbH.strPropWndName = _T("Highlight");

	m_strThumbS = _T("");
	m_strThumbS.vtEx = VT_EX_BITMAPPATH;
	m_strThumbS.strXUIElementName = _T("ThumbS");
	m_strThumbS.strXUIAttributeName = _T("RelativePath");
	m_strThumbS.strPropWndName = _T("Selected");

	m_strThumbD = _T("");
	m_strThumbD.vtEx = VT_EX_BITMAPPATH;
	m_strThumbD.strXUIElementName = _T("ThumbD");
	m_strThumbD.strXUIAttributeName = _T("RelativePath");
	m_strThumbD.strPropWndName = _T("Disabled");

	// Show tip window
	m_bEnableTipWindow.varDefaultValue = m_bEnableTipWindow = false;
	m_bEnableTipWindow.bGroup = true;
	m_bEnableTipWindow.strGroupName = _T("Show Tip Window");
	m_bEnableTipWindow.strXUIElementName = m_sizeTipWindowOffset.strXUIElementName = _T("EnableTipWindow");
	m_bEnableTipWindow.strXUIAttributeName = m_bEnableTipWindow.strPropWndName = _T("Enable");

	m_sizeTipWindowOffset = CPoint(0, 0);
	m_sizeTipWindowOffset.vtEx = VT_EX_POINT;
	m_sizeTipWindowOffset.strXUIAttributeName = _T("Offset");
	m_sizeTipWindowOffset.strPropWndName = _T("Tip Window Offset");
	m_sizeTipWindowOffset.nMinValue = -3840;
	m_sizeTipWindowOffset.nMaxValue = 3840;
}

SLDPROPERTIES::~SLDPROPERTIES()
{

}

SLDEXPROPERTIES::SLDEXPROPERTIES() : SLDPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(SLDEXPROPERTIES);
	m_eControlType = CT_SLIDEREX;
}

SLDEXPROPERTIES::~SLDEXPROPERTIES()
{
}

PRGPROPERTIES::PRGPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(PRGPROPERTIES);
	m_eControlType = CT_PROGRESS;

	DisableBackgroundProperties(TRUE);
	DisableFontProperties(TRUE);
	DisableColorProperties(TRUE);

	m_bVertical.varDefaultValue = m_bVertical = false;
	m_bVertical.bGroup = true;
	m_bVertical.strGroupName = _T("General");
	m_bVertical.strXUIElementName = _T("IsVertical");
	m_bVertical.strPropWndName = _T("Vertical");

	m_eThumbMode.varDefaultValue = m_eThumbMode = PTM_STRETCH;
	m_eThumbMode.bGroup = true;
	m_eThumbMode.strGroupName = _T("Progress Images");
	m_eThumbMode.strXUIElementName = _T("ProgressMode");
	m_eThumbMode.strPropWndName = _T("Progress Mode");
	m_eThumbMode.lstStringOptions.AddTail(IUI_VARIANT(PTM_STRETCH).GetEnumString());
	m_eThumbMode.lstStringOptions.AddTail(IUI_VARIANT(PTM_TILE).GetEnumString());
	m_eThumbMode.lstStringOptions.AddTail(IUI_VARIANT(PTM_CLIP).GetEnumString());

	m_bCombineThumbImages.varDefaultValue = m_bCombineThumbImages = false;
	m_bCombineThumbImages.strXUIElementName = _T("IsCombineProgressImages");
	m_bCombineThumbImages.strPropWndName = _T("CombineProgressImage");

	m_strThumbCombine = _T("");
	m_strThumbCombine.vtEx = VT_EX_BITMAPPATH;
	m_strThumbCombine.strXUIElementName = _T("ProgressCombine");
	m_strThumbCombine.strXUIAttributeName = _T("RelativePath");
	m_strThumbCombine.strPropWndName = _T("Combine Progress");

	m_strThumbN = _T("");
	m_strThumbN.vtEx = VT_EX_BITMAPPATH;
	m_strThumbN.strXUIElementName = _T("ThumbN");
	m_strThumbN.strXUIAttributeName = _T("RelativePath");
	m_strThumbN.strPropWndName = _T("Normal");

	m_strThumbD = _T("");
	m_strThumbD.vtEx = VT_EX_BITMAPPATH;
	m_strThumbD.strXUIElementName = _T("ThumbD");
	m_strThumbD.strXUIAttributeName = _T("RelativePath");
	m_strThumbD.strPropWndName = _T("Disabled");

	m_eThumbPosition.varDefaultValue = m_eThumbPosition = PTP_DEFAULT;
	m_eThumbPosition.strXUIElementName = _T("ThumbPositionMode");
	m_eThumbPosition.strPropWndName = _T("Thumb Position Mode");
	m_eThumbPosition.lstStringOptions.AddTail(IUI_VARIANT(PTP_DEFAULT).GetEnumString());
	m_eThumbPosition.lstStringOptions.AddTail(IUI_VARIANT(PTP_SPECIFIED).GetEnumString());

	m_rcThumbOriginalSpecified.vtEx = VT_EX_RECT_WH;
	m_rcThumbOriginalSpecified.varDefaultValue = m_rcThumbOriginalSpecified = CRect(0, 0, 0, 0);
	m_rcThumbOriginalSpecified.strPropWndName = _T("Original Specified Thumb Position");
	m_rcThumbOriginalSpecified.strXUIElementName = _T("ThumbOriginalPosition");
	m_rcThumbOriginalSpecified.nMinValue = 0;
	m_rcThumbOriginalSpecified.nMaxValue = 3840;

	m_bShowText.varDefaultValue = m_bShowText = true;
	m_bShowText.bGroup = true;
	m_bShowText.strGroupName = _T("Show Percent Text");
	m_bShowText.strXUIElementName = _T("IsShowText");
	m_bShowText.strPropWndName = _T("Show");

	m_lHorAlignN = m_lHorAlignN.varDefaultValue = (LONG)0;
	m_lHorAlignN.strUIKeyName = _T("HorAlig");
	m_lHorAlignN.strXUIElementName = _T("TextAlignment");
	m_lHorAlignN.strXUIAttributeName = _T("HorOffsetN");
	m_lHorAlignN.strPropWndName = _T("Horizontal Offset");
	m_lHorAlignN.nMinValue = -1920;
	m_lHorAlignN.nMaxValue = 1920;

	m_lVerAlignN.varDefaultValue = m_lVerAlignN = (LONG)0;
	m_lVerAlignN.strUIKeyName = _T("VerAlig");
	m_lVerAlignN.strXUIElementName = _T("TextAlignment");
	m_lVerAlignN.strXUIAttributeName = _T("VerOffsetN");
	m_lVerAlignN.strPropWndName = _T("Vertical Offset");
	m_lVerAlignN.nMinValue = -1920;
	m_lVerAlignN.nMaxValue = 1920;
}

PRGPROPERTIES::~PRGPROPERTIES()
{

}

HOTPROPERTIES::HOTPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(HOTPROPERTIES);
	m_eControlType = CT_HOTKEY;
}

HOTPROPERTIES::~HOTPROPERTIES()
{

}

IPAPROPERTIES::IPAPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(IPAPROPERTIES);
	m_eControlType = CT_IPADDRESS;

	// Border
	m_rcBorder.vtEx = VT_EX_RECT;
	m_rcBorder.varDefaultValue = m_rcBorder = CRect(2, 2, 2, 2);
	m_rcBorder.bGroup = true;
	m_rcBorder.strGroupName = _T("Border");
	m_rcBorder.strPropWndName = _T("Border");
	m_rcBorder.strXUIElementName = _T("Border");
	m_rcBorder.nMinValue = 0;
	m_rcBorder.nMaxValue = 3840;

	m_strResColorBorder = _T("");
	m_strResColorBorder.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorBorder.strXUIElementName = _T("ColorResBorder");
	m_strResColorBorder.strPropWndName = _T("Border Color");

	// Filler
	m_bCombineFillerImage.varDefaultValue = m_bCombineFillerImage = true;
	m_bCombineFillerImage.bGroup = true;
	m_bCombineFillerImage.strGroupName = _T("Filler");
	m_bCombineFillerImage.strXUIElementName = _T("IsCombineFillerImages");
	m_bCombineFillerImage.strPropWndName = _T("Combine Filler Image");

	m_strCombineFillerImage = _T("");
	m_strCombineFillerImage.vtEx = VT_EX_BITMAPPATH;
	m_strCombineFillerImage.strXUIElementName = _T("FillerCombine");
	m_strCombineFillerImage.strXUIAttributeName = _T("RelativePath");
	m_strCombineFillerImage.strPropWndName = _T("Combine Filler Image");

	m_strFillerImageN = _T("");
	m_strFillerImageN.vtEx = VT_EX_BITMAPPATH;
	m_strFillerImageN.strXUIElementName = _T("FillerImageN");
	m_strFillerImageN.strXUIAttributeName = _T("RelativePath");
	m_strFillerImageN.strPropWndName = _T("Normal Filler Image");

	m_strFillerImageH = _T("");
	m_strFillerImageH.vtEx = VT_EX_BITMAPPATH;
	m_strFillerImageH.strXUIElementName = _T("FillerImageH");
	m_strFillerImageH.strXUIAttributeName = _T("RelativePath");
	m_strFillerImageH.strPropWndName = _T("Highlight Filler Image");

	m_strFillerImageS = _T("");
	m_strFillerImageS.vtEx = VT_EX_BITMAPPATH;
	m_strFillerImageS.strXUIElementName = _T("FillerImageS");
	m_strFillerImageS.strXUIAttributeName = _T("RelativePath");
	m_strFillerImageS.strPropWndName = _T("Selected Filler Image");

	m_strFillerImageD = _T("");
	m_strFillerImageD.vtEx = VT_EX_BITMAPPATH;
	m_strFillerImageD.strXUIElementName = _T("FillerImageD");
	m_strFillerImageD.strXUIAttributeName = _T("RelativePath");
	m_strFillerImageD.strPropWndName = _T("Disabled Filler Image");

	m_lFillerWidth.varDefaultValue = m_lFillerWidth = (LONG)0;
	m_lFillerWidth.strXUIElementName = _T("FillerWidth");
	m_lFillerWidth.strXUIAttributeName = m_lFillerWidth.strPropWndName = _T("Width");
	m_lFillerWidth.strDescription = _T("Width is 0 means used text '.' or filler image's width.");

	// Field
	m_bCombineFieldImage.varDefaultValue = m_bCombineFieldImage = true;
	m_bCombineFieldImage.bGroup = true;
	m_bCombineFieldImage.strGroupName = _T("Field");
	m_bCombineFieldImage.strXUIElementName = _T("IsCombineFieldImages");
	m_bCombineFieldImage.strPropWndName = _T("Combine Field Image");

	m_strCombineFieldImage = _T("");
	m_strCombineFieldImage.vtEx = VT_EX_BITMAPPATH;
	m_strCombineFieldImage.strXUIElementName = _T("FieldCombine");
	m_strCombineFieldImage.strXUIAttributeName = _T("RelativePath");
	m_strCombineFieldImage.strPropWndName = _T("Combine Field Image");

	m_strFieldImageN = _T("");
	m_strFieldImageN.vtEx = VT_EX_BITMAPPATH;
	m_strFieldImageN.strXUIElementName = _T("FieldImageN");
	m_strFieldImageN.strXUIAttributeName = _T("RelativePath");
	m_strFieldImageN.strPropWndName = _T("Normal Field Image");

	m_strFieldImageH = _T("");
	m_strFieldImageH.vtEx = VT_EX_BITMAPPATH;
	m_strFieldImageH.strXUIElementName = _T("FieldImageH");
	m_strFieldImageH.strXUIAttributeName = _T("RelativePath");
	m_strFieldImageH.strPropWndName = _T("Highlight Field Image");

	m_strFieldImageS = _T("");
	m_strFieldImageS.vtEx = VT_EX_BITMAPPATH;
	m_strFieldImageS.strXUIElementName = _T("FieldImageS");
	m_strFieldImageS.strXUIAttributeName = _T("RelativePath");
	m_strFieldImageS.strPropWndName = _T("Selected Field Image");

	m_strFieldImageD = _T("");
	m_strFieldImageD.vtEx = VT_EX_BITMAPPATH;
	m_strFieldImageD.strXUIElementName = _T("FieldImageD");
	m_strFieldImageD.strXUIAttributeName = _T("RelativePath");
	m_strFieldImageD.strPropWndName = _T("Disabled Field Image");

	m_lFieldHeight.varDefaultValue = m_lFieldHeight = (LONG)0;
	m_lFieldHeight.strXUIElementName = _T("FieldHeight");
	m_lFieldHeight.strXUIAttributeName = m_lFieldHeight.strPropWndName = _T("Height");
	m_lFieldHeight.strDescription = _T("Height is 0 means not layout the child edit control.\
if you need layout the child edit, make Height to non zero value");

	m_eFieldAlignModeVer.varDefaultValue = m_eFieldAlignModeVer = CLV_FILL;
	m_eFieldAlignModeVer.vtEx = VT_EX_LAYOUT_VER;
	m_eFieldAlignModeVer.bReadOnly = true;
	m_eFieldAlignModeVer.strVarName = _T("m_eFieldAlignModeVer");
	m_eFieldAlignModeVer.strXUIElementName = _T("FieldLayoutMode");
	m_eFieldAlignModeVer.strXUIAttributeName = _T("Vertical");
	m_eFieldAlignModeVer.strPropWndName = _T("Field Vertical Layout");
	m_eFieldAlignModeVer.strDescription = _T("Specifies the child edit controls vertical layout mode.");
	m_eFieldAlignModeVer.lstStringOptions.AddTail(IUI_VARIANT(CLV_TOP).GetEnumString());
	m_eFieldAlignModeVer.lstStringOptions.AddTail(IUI_VARIANT(CLV_BOTTOM).GetEnumString());
	m_eFieldAlignModeVer.lstStringOptions.AddTail(IUI_VARIANT(CLV_CENTER).GetEnumString());
	m_eFieldAlignModeVer.lstStringOptions.AddTail(IUI_VARIANT(CLV_FILL).GetEnumString());
}

IPAPROPERTIES::~IPAPROPERTIES()
{

}

LSTCTRLPROPERTIES::LSTCTRLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(LSTCTRLPROPERTIES);
	m_eControlType = CT_LISTCTRL;

	DisableBackgroundProperties(TRUE);

	// Item text Font
	m_strResFontIDN.strGroupName = _T("Item Font(Size)");
	m_strResFontIDN.strXUIElementName = _T("ItemFontResN");
	m_strResFontIDH.strXUIElementName = _T("ItemFontResH");
	m_strResFontIDS.strXUIElementName = _T("ItemFontResS");
	m_bDSameNFont.strXUIElementName = _T("IsNDSameNFont");
	m_bDSameNFont.strPropWndName = _T("Normal Disabled Font Same as Normal");
	m_strResFontIDD.strXUIElementName = _T("ItemFontResND");
	m_strResFontIDD.strPropWndName = _T("Normal Disabled");

	// Item text Color
	m_strResColorIDN.strGroupName = _T("Item Text Color");
	m_strResColorIDN.strXUIElementName = _T("ItemTextColorResN");
	m_strResColorIDH.strXUIElementName = _T("ItemTextColorResH");
	m_strResColorIDS.strXUIElementName = _T("ItemTextColorResS");
	m_bDSameNColor.strXUIElementName = _T("IsNDSameNColor");
	m_bDSameNColor.strPropWndName = _T("Normal Disabled Color Same as Normal");
	m_strResColorIDD.strXUIElementName = _T("ItemTextColorResND");
	m_strResColorIDD.strPropWndName = _T("Normal Disabled");

	//
	m_bBorder.varDefaultValue = m_bBorder = false;
	m_bBorder.bGroup = true;
	m_bBorder.strGroupName = _T("General");
	m_bBorder.strXUIElementName = m_bBorder.strPropWndName = _T("Border");

	m_eListStyle.varDefaultValue = m_eListStyle = LSTS_REPORT;
	m_eListStyle.vtEx = VT_EX_LST_STYLE;
	m_eListStyle.strUIKeyName = m_eListStyle.strXUIElementName = _T("ListStyle");
	m_eListStyle.strPropWndName = _T("List Style");
	m_eListStyle.lstStringOptions.AddTail(IUI_VARIANT(LSTS_ICON).GetEnumString());
	m_eListStyle.lstStringOptions.AddTail(IUI_VARIANT(LSTS_SMALLICON).GetEnumString());
	m_eListStyle.lstStringOptions.AddTail(IUI_VARIANT(LSTS_LIST).GetEnumString());
	m_eListStyle.lstStringOptions.AddTail(IUI_VARIANT(LSTS_REPORT).GetEnumString());
	m_eListStyle.lstStringOptions.AddTail(IUI_VARIANT(LSTS_THUMBNAIL).GetEnumString());

	m_eListSort.varDefaultValue = m_eListSort = LSTST_UNSET;
	m_eListSort.vtEx = VT_EX_LST_SORT;
	m_eListSort.strUIKeyName = m_eListSort.strXUIElementName = _T("ListSort");
	m_eListSort.strPropWndName = _T("List Sort");
	m_eListSort.lstStringOptions.AddTail(IUI_VARIANT(LSTST_UNSET).GetEnumString());
	m_eListSort.lstStringOptions.AddTail(IUI_VARIANT(LSTST_ASCENDING).GetEnumString());
	m_eListSort.lstStringOptions.AddTail(IUI_VARIANT(LSTST_DESCENDING).GetEnumString());

	m_bEditLabels.varDefaultValue = m_bEditLabels = false;
	m_bEditLabels.strXUIElementName = _T("IsEditLabels");
	m_bEditLabels.strPropWndName = _T("Edit Labels");

	m_bShowSelAlways.varDefaultValue = m_bShowSelAlways = false;
	m_bShowSelAlways.strXUIElementName = _T("IsShowSelAlways");
	m_bShowSelAlways.strPropWndName = _T("Always Show Selected Item");

	m_bSingleSel.varDefaultValue = m_bSingleSel = false;
	m_bSingleSel.strXUIElementName = _T("IsSingleSel");
	m_bSingleSel.strPropWndName = _T("Single Select");

	m_bOwnerData.varDefaultValue = m_bOwnerData = false;
	m_bOwnerData.strXUIElementName = _T("IsOwnerData");
	m_bOwnerData.strPropWndName = _T("Owner Data");

	m_bHoldCtrlKey.varDefaultValue = m_bHoldCtrlKey = false;
	m_bHoldCtrlKey.strXUIElementName = _T("IsHoldCtrlKey");
	m_bHoldCtrlKey.strPropWndName = _T("Hold Ctrl Key");

	m_bCheckBoxes.varDefaultValue = m_bCheckBoxes = false;
	m_bCheckBoxes.strXUIElementName = _T("IsCheckBoxes");
	m_bCheckBoxes.strPropWndName = _T("Has Check Boxes");

	m_bInfoTooltip.varDefaultValue = m_bInfoTooltip = false;
	m_bInfoTooltip.strXUIElementName = _T("IsInfoTip");
	m_bInfoTooltip.strPropWndName = _T("Info ToolTip");

	m_bTrackSelect.varDefaultValue = m_bTrackSelect = false;
	m_bTrackSelect.strXUIElementName = _T("IsTrackSelect");
	m_bTrackSelect.strPropWndName = _T("Track Select");

	m_bTwoClickActivate.varDefaultValue = m_bTwoClickActivate = false;
	m_bTwoClickActivate.strXUIElementName = _T("IsTwoClickActivate");
	m_bTwoClickActivate.strPropWndName = _T("Two Click Activate");

	m_bEnableOwnerDraw.varDefaultValue = m_bEnableOwnerDraw = false;
	m_bEnableOwnerDraw.strXUIElementName = _T("IsEnableOwnerDraw");
	m_bEnableOwnerDraw.strPropWndName = _T("Enable Owner Draw");

	m_strTag.varDefaultValue = m_strTag = _T("");
	m_strTag.strUIKeyName = m_strTag.strXUIElementName = _T("Tag");
	m_strTag.strPropWndName = _T("Tag");
	m_strTag.strDescription = _T("");

	// Only fro report style
	m_bFullRowSelect.varDefaultValue = m_bFullRowSelect = false;
	m_bFullRowSelect.bGroup = true;
	m_bFullRowSelect.strGroupName = _T("Report extended Properties");
	m_bFullRowSelect.strXUIElementName = _T("IsFullRowSelect");
	m_bFullRowSelect.strPropWndName = _T("Full Row Select");

	m_bSubItemImages.varDefaultValue = m_bSubItemImages = false;
	m_bSubItemImages.strXUIElementName = _T("IsSubItemImages");
	m_bSubItemImages.strPropWndName = _T("Sub Item Images");

	// Icon and small icon view
	m_eListAlign.varDefaultValue = m_eListAlign = LSTA_TOP;
	m_eListAlign.vtEx = VT_EX_LST_ALIGN;
	m_eListAlign.bGroup = true;
	m_eListAlign.strGroupName = _T("(Small) Icon Extended Properties");
	m_eListAlign.strUIKeyName = m_eListAlign.strXUIElementName = _T("ListAlign");
	m_eListAlign.strPropWndName = _T("List Alignment");
	m_eListAlign.lstStringOptions.AddTail(IUI_VARIANT(LSTA_TOP).GetEnumString());
	m_eListAlign.lstStringOptions.AddTail(IUI_VARIANT(LSTA_LEFT).GetEnumString());

	m_bAutoArrange.varDefaultValue = m_bAutoArrange = false;
	m_bAutoArrange.strXUIElementName = _T("IsAutoArrange");
	m_bAutoArrange.strPropWndName = _T("Auto Arrange");

	m_bNoScroll.varDefaultValue = m_bNoScroll = false;
	m_bNoScroll.strXUIElementName = _T("IsNoScroll");
	m_bNoScroll.strPropWndName = _T("No Scroll");

	m_bNoLabelWrap.varDefaultValue = m_bNoLabelWrap = false;
	m_bNoLabelWrap.strXUIElementName = _T("IsNoLabelWrap");
	m_bNoLabelWrap.strPropWndName = _T("No Label Wrap");

	// Grid line
	m_bShowGridLine.varDefaultValue = m_bShowGridLine = false;
	m_bShowGridLine.bGroup = true;
	m_bShowGridLine.strGroupName = _T("Grid Line");
	m_bShowGridLine.strXUIElementName = _T("GridLine");
	m_bShowGridLine.strXUIAttributeName = m_bShowGridLine.strPropWndName = _T("Show");

	m_bDraw0Row.varDefaultValue = m_bDraw0Row = false;
	m_bDraw0Row.strXUIElementName = m_bShowGridLine.strXUIElementName;
	m_bDraw0Row.strXUIAttributeName = _T("Draw0Row");
	m_bDraw0Row.strPropWndName = _T("Draw First Row");

	m_lXOffset.varDefaultValue = m_lXOffset = (LONG)0;
	m_lXOffset.strXUIElementName = m_bShowGridLine.strXUIElementName;
	m_lXOffset.strXUIAttributeName = _T("XOffset");
	m_lXOffset.strPropWndName = _T("Horizontal Offset");
	m_lXOffset.nMinValue = -3840;
	m_lXOffset.nMaxValue = 3840;

	m_crGridLine.varDefaultValue = m_crGridLine = (LONG)RGB(212, 208, 200);
	m_crGridLine.vtEx = VT_EX_COLOR;
	m_crGridLine.strXUIElementName = m_bShowGridLine.strXUIElementName;
	m_crGridLine.strXUIAttributeName = m_crGridLine.strPropWndName = _T("Color");

	// Normal Item
	m_bEnableNormalItemEffect.varDefaultValue = m_bEnableNormalItemEffect = false;
	m_bEnableNormalItemEffect.bGroup = true;
	m_bEnableNormalItemEffect.strGroupName = _T("Normal Item Background");
	m_bEnableNormalItemEffect.strXUIElementName = _T("NormalItemBk");
	m_bEnableNormalItemEffect.strXUIAttributeName = m_bEnableNormalItemEffect.strPropWndName = _T("Effect");

	m_eNormalItemBkType.varDefaultValue = m_eNormalItemBkType = BKT_COLOR;
	m_eNormalItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eNormalItemBkType.strXUIElementName = m_bEnableNormalItemEffect.strXUIElementName;
	m_eNormalItemBkType.strXUIAttributeName = m_eNormalItemBkType.strPropWndName = _T("Type");
	m_eNormalItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eNormalItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_strNormalItemBkColorResID = _T("");
	m_strNormalItemBkColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strNormalItemBkColorResID.strXUIElementName = m_bEnableNormalItemEffect.strXUIElementName;
	m_strNormalItemBkColorResID.strXUIAttributeName = _T("ColorResID");
	m_strNormalItemBkColorResID.strPropWndName = _T("Color ID");

	m_strNormalItemBkImage = _T("");
	m_strNormalItemBkImage.vtEx = VT_EX_BITMAPPATH;
	m_strNormalItemBkImage.strXUIElementName = m_bEnableNormalItemEffect.strXUIElementName;
	m_strNormalItemBkImage.strXUIAttributeName = m_strNormalItemBkImage.strPropWndName = _T("Image");

	m_ptNormalItemBkResize = CPoint(2, 2);
	m_ptNormalItemBkResize.vtEx = VT_EX_POINT;
	m_ptNormalItemBkResize.strXUIElementName = _T("NormalItemBkResizePoint");
	m_ptNormalItemBkResize.strPropWndName = _T("Resize Point");
	m_ptNormalItemBkResize.nMinValue = 0;
	m_ptNormalItemBkResize.nMaxValue = 3940;

	// Highlight Item
	m_bEnableHighlightItemEffect.varDefaultValue = m_bEnableHighlightItemEffect = false;
	m_bEnableHighlightItemEffect.bGroup = true;
	m_bEnableHighlightItemEffect.strGroupName = _T("Highlight Item Background");
	m_bEnableHighlightItemEffect.strXUIElementName = _T("HighlightItemBk");
	m_bEnableHighlightItemEffect.strXUIAttributeName = m_bEnableHighlightItemEffect.strPropWndName = _T("Effect");

	m_eHighlightItemBkType.varDefaultValue = m_eHighlightItemBkType = BKT_COLOR;
	m_eHighlightItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eHighlightItemBkType.strXUIElementName = m_bEnableHighlightItemEffect.strXUIElementName;
	m_eHighlightItemBkType.strXUIAttributeName = m_eHighlightItemBkType.strPropWndName = _T("Type");
	m_eHighlightItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eHighlightItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_strHighlightItemBkColorResID = _T("");
	m_strHighlightItemBkColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strHighlightItemBkColorResID.strXUIElementName = m_bEnableHighlightItemEffect.strXUIElementName;
	m_strHighlightItemBkColorResID.strXUIAttributeName = _T("ColorResID");
	m_strHighlightItemBkColorResID.strPropWndName = _T("Color ID");

	m_strHighlightItemBkImage = _T("");
	m_strHighlightItemBkImage.vtEx = VT_EX_BITMAPPATH;
	m_strHighlightItemBkImage.strXUIElementName = m_bEnableHighlightItemEffect.strXUIElementName;
	m_strHighlightItemBkImage.strXUIAttributeName = m_strHighlightItemBkImage.strPropWndName = _T("Image");

	m_ptHighlightItemBkResize = CPoint(2, 2);
	m_ptHighlightItemBkResize.vtEx = VT_EX_POINT;
	m_ptHighlightItemBkResize.strXUIElementName = _T("HighlightItemBkResizePoint");
	m_ptHighlightItemBkResize.strPropWndName = _T("Resize Point");
	m_ptHighlightItemBkResize.nMinValue = 0;
	m_ptHighlightItemBkResize.nMaxValue = 3940;

	// Selected Item
	m_bSelectedFitClient.varDefaultValue = m_bSelectedFitClient = false;
	m_bSelectedFitClient.bGroup = true;
	m_bSelectedFitClient.strGroupName = _T("Selected Item Background");
	m_bSelectedFitClient.strXUIElementName = _T("SelectedItemBk");
	m_bSelectedFitClient.strXUIAttributeName = _T("IsSelectedFitClient");
	m_bSelectedFitClient.strPropWndName = _T("Selected Fit Client");

	m_eSelectedItemBkType.varDefaultValue = m_eSelectedItemBkType = BKT_COLOR;
	m_eSelectedItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eSelectedItemBkType.strXUIElementName = m_bSelectedFitClient.strXUIElementName;
	m_eSelectedItemBkType.strXUIAttributeName = m_eSelectedItemBkType.strPropWndName = _T("Type");
	m_eSelectedItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eSelectedItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_strSelectedItemBkColorResID = _T("");
	m_strSelectedItemBkColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strSelectedItemBkColorResID.strXUIElementName = m_bSelectedFitClient.strXUIElementName;
	m_strSelectedItemBkColorResID.strXUIAttributeName = _T("ColorResID");
	m_strSelectedItemBkColorResID.strPropWndName = _T("Color ID");

	m_strSelectedItemBkImage = _T("");
	m_strSelectedItemBkImage.vtEx = VT_EX_BITMAPPATH;
	m_strSelectedItemBkImage.strXUIElementName = m_bSelectedFitClient.strXUIElementName;
	m_strSelectedItemBkImage.strXUIAttributeName = m_strSelectedItemBkImage.strPropWndName = _T("Image");

	m_ptSelectedItemBkResize = CPoint(2, 2);
	m_ptSelectedItemBkResize.vtEx = VT_EX_POINT;
	m_ptSelectedItemBkResize.strXUIElementName = _T("SelectedItemBkResizePoint");
	m_ptSelectedItemBkResize.strPropWndName = _T("Resize Point");
	m_ptSelectedItemBkResize.nMinValue = 0;
	m_ptSelectedItemBkResize.nMaxValue = 3940;

	// Normal Disable Item
	m_bEnableNormalDisabledItemEffect.varDefaultValue = m_bEnableNormalDisabledItemEffect = false;
	m_bEnableNormalDisabledItemEffect.bGroup = true;
	m_bEnableNormalDisabledItemEffect.strGroupName = _T("Normal Disabled Item Background");
	m_bEnableNormalDisabledItemEffect.strXUIElementName = _T("NormalDisabledItemBk");
	m_bEnableNormalDisabledItemEffect.strXUIAttributeName = m_bEnableNormalDisabledItemEffect.strPropWndName = _T("Effect");

	m_eNormalDisabledItemBkType.varDefaultValue = m_eNormalDisabledItemBkType = BKT_COLOR;
	m_eNormalDisabledItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eNormalDisabledItemBkType.strXUIElementName = m_bEnableNormalDisabledItemEffect.strXUIElementName;
	m_eNormalDisabledItemBkType.strXUIAttributeName = m_eNormalDisabledItemBkType.strPropWndName = _T("Type");
	m_eNormalDisabledItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eNormalDisabledItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_strNormalDisabledItemBkColorResID = _T("");
	m_strNormalDisabledItemBkColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strNormalDisabledItemBkColorResID.strXUIElementName = m_bEnableNormalDisabledItemEffect.strXUIElementName;
	m_strNormalDisabledItemBkColorResID.strXUIAttributeName = _T("ColorResID");
	m_strNormalDisabledItemBkColorResID.strPropWndName = _T("Color ID");

	m_strNormalDisabledItemBkImage = _T("");
	m_strNormalDisabledItemBkImage.vtEx = VT_EX_BITMAPPATH;
	m_strNormalDisabledItemBkImage.strXUIElementName = m_bEnableNormalDisabledItemEffect.strXUIElementName;
	m_strNormalDisabledItemBkImage.strXUIAttributeName = m_strNormalDisabledItemBkImage.strPropWndName = _T("Image");

	m_ptNormalDisabledItemBkResize = CPoint(2, 2);
	m_ptNormalDisabledItemBkResize.vtEx = VT_EX_POINT;
	m_ptNormalDisabledItemBkResize.strXUIElementName = _T("NormalDisabledItemBkResizePoint");
	m_ptNormalDisabledItemBkResize.strPropWndName = _T("Resize Point");
	m_ptNormalDisabledItemBkResize.nMinValue = 0;
	m_ptNormalDisabledItemBkResize.nMaxValue = 3940;

	// Selected Disable Item
	m_bEnableSelectedDisabledItemEffect.varDefaultValue = m_bEnableSelectedDisabledItemEffect = false;
	m_bEnableSelectedDisabledItemEffect.bGroup = true;
	m_bEnableSelectedDisabledItemEffect.strGroupName = _T("Selected Disabled Item Background");
	m_bEnableSelectedDisabledItemEffect.strXUIElementName = _T("SelectedDisabledItemBk");
	m_bEnableSelectedDisabledItemEffect.strXUIAttributeName = m_bEnableSelectedDisabledItemEffect.strPropWndName = _T("Effect");

	m_eSelectedDisabledItemBkType.varDefaultValue = m_eSelectedDisabledItemBkType = BKT_COLOR;
	m_eSelectedDisabledItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eSelectedDisabledItemBkType.strXUIElementName = m_bEnableSelectedDisabledItemEffect.strXUIElementName;
	m_eSelectedDisabledItemBkType.strXUIAttributeName = m_eSelectedDisabledItemBkType.strPropWndName = _T("Type");
	m_eSelectedDisabledItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eSelectedDisabledItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_strSelectedDisabledItemBkColorResID = _T("");
	m_strSelectedDisabledItemBkColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strSelectedDisabledItemBkColorResID.strXUIElementName = m_bEnableSelectedDisabledItemEffect.strXUIElementName;
	m_strSelectedDisabledItemBkColorResID.strXUIAttributeName = _T("ColorResID");
	m_strSelectedDisabledItemBkColorResID.strPropWndName = _T("Color ID");

	m_strSelectedDisabledItemBkImage = _T("");
	m_strSelectedDisabledItemBkImage.vtEx = VT_EX_BITMAPPATH;
	m_strSelectedDisabledItemBkImage.strXUIElementName = m_bEnableSelectedDisabledItemEffect.strXUIElementName;
	m_strSelectedDisabledItemBkImage.strXUIAttributeName = m_strSelectedDisabledItemBkImage.strPropWndName = _T("Image");

	m_ptSelectedDisabledItemBkResize = CPoint(2, 2);
	m_ptSelectedDisabledItemBkResize.vtEx = VT_EX_POINT;
	m_ptSelectedDisabledItemBkResize.strXUIElementName = _T("SelectedDisabledItemBkResizePoint");
	m_ptSelectedDisabledItemBkResize.strPropWndName = _T("Resize Point");
	m_ptSelectedDisabledItemBkResize.nMinValue = 0;
	m_ptSelectedDisabledItemBkResize.nMaxValue = 3940;

	m_bSDSameNFont.varDefaultValue = m_bSDSameNFont = true;
	m_bSDSameNFont.bGroup = true;
	m_bSDSameNFont.strGroupName = m_strResFontIDN.strGroupName;
	m_bSDSameNFont.strXUIElementName = _T("IsSDSameNFont");
	m_bSDSameNFont.strPropWndName = _T("Selected Disabled Font Same as Normal");

	m_strResFontIDSD = _T("");
	m_strResFontIDSD.vtEx = VT_EX_RESOURCE_FONT;
	m_strResFontIDSD.strXUIElementName = _T("ItemFontResSD");
	m_strResFontIDSD.strPropWndName = _T("Selected Disabled");

	m_bSDSameNColor.varDefaultValue = m_bSDSameNColor = true;
	m_bSDSameNColor.bGroup = true;
	m_bSDSameNColor.strGroupName = m_strResColorIDN.strGroupName;
	m_bSDSameNColor.strXUIElementName = _T("IsSDSameNColor");
	m_bSDSameNColor.strPropWndName = _T("Selected Disabled Color Same as Normal");

	m_strResColorIDSD = _T("");
	m_strResColorIDSD.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDSD.strXUIElementName = _T("ItemTextColorResSD");
	m_strResColorIDSD.strPropWndName = _T("Selected Disabled");

	// For thumbnail view item position
	m_lTotalItemWidth.varDefaultValue = m_lTotalItemWidth = (LONG)120;
	m_lTotalItemWidth.bGroup = true;
	m_lTotalItemWidth.strGroupName = _T("Thumbnail Item Size");
	m_lTotalItemWidth.strXUIElementName = _T("TotalThumbnailItemWidth");
	m_lTotalItemWidth.strPropWndName = _T("Total Width");
	m_lTotalItemWidth.nMinValue = 1;
	m_lTotalItemWidth.nMaxValue = 3840;

	m_lTotalItemHeight.varDefaultValue = m_lTotalItemHeight = (LONG)140;
	m_lTotalItemHeight.strXUIElementName = _T("TotalThumbnailItemHeight");
	m_lTotalItemHeight.strPropWndName = _T("Total Height");
	m_lTotalItemHeight.nMinValue = 1;
	m_lTotalItemHeight.nMaxValue = 3840;

	// Thumbnail Background Position
	m_rcThumbnailBkN.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailBkN.varDefaultValue = m_rcThumbnailBkN = CRect(10, 10, 110, 130);
	m_rcThumbnailBkN.strPropWndName = _T("Thumbnail Normal Background");
	m_rcThumbnailBkN.strXUIElementName = _T("ThumbnailBkPosN");
	m_rcThumbnailBkN.nMinValue = -3840;
	m_rcThumbnailBkN.nMaxValue = 3840;

	m_rcThumbnailBkH.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailBkH.varDefaultValue = m_rcThumbnailBkH = CRect(10, 10, 110, 130);
	m_rcThumbnailBkH.strPropWndName = _T("Thumbnail Highlight Background");
	m_rcThumbnailBkH.strXUIElementName = _T("ThumbnailBkPosH");
	m_rcThumbnailBkH.nMinValue = -3840;
	m_rcThumbnailBkH.nMaxValue = 3840;

	m_rcThumbnailBkS.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailBkS.varDefaultValue = m_rcThumbnailBkS = CRect(10, 10, 110, 130);
	m_rcThumbnailBkS.strPropWndName = _T("Thumbnail Selected Background");
	m_rcThumbnailBkS.strXUIElementName = _T("ThumbnailBkPosS");
	m_rcThumbnailBkS.nMinValue = -3840;
	m_rcThumbnailBkS.nMaxValue = 3840;

	m_rcThumbnailBkD.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailBkD.varDefaultValue = m_rcThumbnailBkD = CRect(10, 10, 110, 130);
	m_rcThumbnailBkD.strPropWndName = _T("Thumbnail Disabled Background");
	m_rcThumbnailBkD.strXUIElementName = _T("ThumbnailBkPosD");
	m_rcThumbnailBkD.nMinValue = -3840;
	m_rcThumbnailBkD.nMaxValue = 3840;

	// Thumbnail Position
	m_rcThumbnailN.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailN.varDefaultValue = m_rcThumbnailN = CRect(20, 20, 100, 100);
	m_rcThumbnailN.strPropWndName = _T("Thumbnail Normal");
	m_rcThumbnailN.strXUIElementName = _T("ThumbnailPosN");
	m_rcThumbnailN.nMinValue = -3840;
	m_rcThumbnailN.nMaxValue = 3840;

	m_rcThumbnailH.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailH.varDefaultValue = m_rcThumbnailH = CRect(20, 20, 100, 100);
	m_rcThumbnailH.strPropWndName = _T("Thumbnail Highlight");
	m_rcThumbnailH.strXUIElementName = _T("ThumbnailPosH");
	m_rcThumbnailH.nMinValue = -3840;
	m_rcThumbnailH.nMaxValue = 3840;

	m_rcThumbnailS.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailS.varDefaultValue = m_rcThumbnailS = CRect(20, 20, 100, 100);
	m_rcThumbnailS.strPropWndName = _T("Thumbnail Selected");
	m_rcThumbnailS.strXUIElementName = _T("ThumbnailPosS");
	m_rcThumbnailS.nMinValue = -3840;
	m_rcThumbnailS.nMaxValue = 3840;

	m_rcThumbnailD.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailD.varDefaultValue = m_rcThumbnailD = CRect(20, 20, 100, 100);
	m_rcThumbnailD.strPropWndName = _T("Thumbnail Disabled");
	m_rcThumbnailD.strXUIElementName = _T("ThumbnailPosD");
	m_rcThumbnailD.nMinValue = -3840;
	m_rcThumbnailD.nMaxValue = 3840;

	// Thumbnail Label Position
	m_rcThumbnailLabelN.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailLabelN.varDefaultValue = m_rcThumbnailLabelN = CRect(10, 110, 130, 130);
	m_rcThumbnailLabelN.strPropWndName = _T("Thumbnail Label Normal");
	m_rcThumbnailLabelN.strXUIElementName = _T("ThumbnailLabelPosN");
	m_rcThumbnailLabelN.nMinValue = -3840;
	m_rcThumbnailLabelN.nMaxValue = 3840;

	m_rcThumbnailLabelH.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailLabelH.varDefaultValue = m_rcThumbnailLabelH = CRect(10, 110, 130, 130);
	m_rcThumbnailLabelH.strPropWndName = _T("Thumbnail Label Highlight");
	m_rcThumbnailLabelH.strXUIElementName = _T("ThumbnailLabelPosH");
	m_rcThumbnailLabelH.nMinValue = -3840;
	m_rcThumbnailLabelH.nMaxValue = 3840;

	m_rcThumbnailLabelS.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailLabelS.varDefaultValue = m_rcThumbnailLabelS = CRect(10, 110, 130, 130);
	m_rcThumbnailLabelS.strPropWndName = _T("Thumbnail Label Selected");
	m_rcThumbnailLabelS.strXUIElementName = _T("ThumbnailLabelPosS");
	m_rcThumbnailLabelS.nMinValue = -3840;
	m_rcThumbnailLabelS.nMaxValue = 3840;

	m_rcThumbnailLabelD.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailLabelD.varDefaultValue = m_rcThumbnailLabelD = CRect(10, 110, 130, 130);
	m_rcThumbnailLabelD.strPropWndName = _T("Thumbnail Label Disabled");
	m_rcThumbnailLabelD.strXUIElementName = _T("ThumbnailLabelPosD");
	m_rcThumbnailLabelD.nMinValue = -3840;
	m_rcThumbnailLabelD.nMaxValue = 3840;

	// Thumbnail Icon Position
	m_rcThumbnailIconN.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailIconN.varDefaultValue = m_rcThumbnailIconN = CRect(20, 20, 100, 100);
	m_rcThumbnailIconN.strPropWndName = _T("Thumbnail Icon Normal");
	m_rcThumbnailIconN.strXUIElementName = _T("ThumbnailIconPosN");
	m_rcThumbnailIconN.nMinValue = -3840;
	m_rcThumbnailIconN.nMaxValue = 3840;

	m_rcThumbnailIconH.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailIconH.varDefaultValue = m_rcThumbnailIconH = CRect(20, 20, 100, 100);
	m_rcThumbnailIconH.strPropWndName = _T("Thumbnail Icon Highlight");
	m_rcThumbnailIconH.strXUIElementName = _T("ThumbnailIconPosH");
	m_rcThumbnailIconH.nMinValue = -3840;
	m_rcThumbnailIconH.nMaxValue = 3840;

	m_rcThumbnailIconS.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailIconS.varDefaultValue = m_rcThumbnailIconS = CRect(20, 20, 100, 100);
	m_rcThumbnailIconS.strPropWndName = _T("Thumbnail Icon Selected");
	m_rcThumbnailIconS.strXUIElementName = _T("ThumbnailIconPosS");
	m_rcThumbnailIconS.nMinValue = -3840;
	m_rcThumbnailIconS.nMaxValue = 3840;

	m_rcThumbnailIconD.vtEx = VT_EX_RECT_WH;
	m_rcThumbnailIconD.varDefaultValue = m_rcThumbnailIconD = CRect(20, 20, 100, 100);
	m_rcThumbnailIconD.strPropWndName = _T("Thumbnail Icon Disabled");
	m_rcThumbnailIconD.strXUIElementName = _T("ThumbnailIconPosD");
	m_rcThumbnailIconD.nMinValue = -3840;
	m_rcThumbnailIconD.nMaxValue = 3840;

	// Header
	m_bNoHeader.varDefaultValue = m_bNoHeader = false;
	m_bNoHeader.bGroup = true;
	m_bNoHeader.strGroupName = _T("Header");
	m_bNoHeader.strXUIElementName = _T("IsNoHeader");
	m_bNoHeader.strPropWndName = _T("No Header");

	m_bNoSortHeader.varDefaultValue = m_bNoSortHeader = false;
	m_bNoSortHeader.strXUIElementName = _T("IsNoSortHeader");
	m_bNoSortHeader.strPropWndName = _T("Sort Header");

	m_nHeaderHeight.varDefaultValue = m_nHeaderHeight = (LONG)24;
	m_nHeaderHeight.strXUIElementName = _T("HeaderHeight");
	m_nHeaderHeight.strPropWndName = _T("Header Height");
	m_nHeaderHeight.nMinValue = 0;
	m_nHeaderHeight.nMaxValue = 1200;

	m_bHeaderTrack.varDefaultValue = m_bHeaderTrack = true;
	m_bHeaderTrack.strXUIElementName = _T("IsHeaderTrack");
	m_bHeaderTrack.strPropWndName = _T("Header Track");

	m_bHeaderDragDrop.varDefaultValue = m_bHeaderDragDrop = false;
	m_bHeaderDragDrop.strXUIElementName = _T("IsHeaderDragDrop");
	m_bHeaderDragDrop.strPropWndName = _T("Header Drag Drop");

	m_strHeaderStyleID = _T("");
	m_strHeaderStyleID.bReadOnly = true;
	m_strHeaderStyleID.vtEx = VT_EX_STYLE_HEADERCTRL;
	m_strHeaderStyleID.strXUIElementName = _T("HeaderStyleID");
	m_strHeaderStyleID.strPropWndName = _T("Header Style");

	// Scroll bar
	m_bUseCustomScrollBar.varDefaultValue = m_bUseCustomScrollBar = false;
	m_bUseCustomScrollBar.bGroup = true;
	m_bUseCustomScrollBar.strGroupName = _T("Enable Custom Scroll Bar");
	m_bUseCustomScrollBar.strXUIElementName = _T("EnableCustomScrollBar");
	m_bUseCustomScrollBar.strPropWndName = _T("Enable");

	m_strHorScrollbarID = _T("");
	m_strHorScrollbarID.bReadOnly = true;
	m_strHorScrollbarID.vtEx = VT_EX_STYLE_HORSCROLLBAR;
	m_strHorScrollbarID.strXUIElementName = _T("HorizontalScrollbarStyleID");
	m_strHorScrollbarID.strPropWndName = _T("Horizontal Scrollbar");

	m_strVerScrollbarID = _T("");
	m_strVerScrollbarID.bReadOnly = true;
	m_strVerScrollbarID.vtEx = VT_EX_STYLE_VERSCROLLBAR;
	m_strVerScrollbarID.strXUIElementName = _T("VerticalScrollbarStyleID");
	m_strVerScrollbarID.strPropWndName = _T("Vertical Scrollbar");
}

LSTCTRLPROPERTIES::~LSTCTRLPROPERTIES()
{

}

bool LSTCTRLPROPERTIES::IsEnableCustomScrollBar() const
{
	return m_bUseCustomScrollBar;
}

TREECTRLPROPERTIES::TREECTRLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(TREECTRLPROPERTIES);
	m_eControlType = CT_TREECTRL;

	// Background
	DisableBackgroundProperties(TRUE);

	// color
	m_crN.strGroupName = _T("Item Text Color");
	m_strResColorIDN.strGroupName = _T("Item Text Color");

	m_bBorder.varDefaultValue = m_bBorder = false;
	m_bBorder.bGroup = true;
	m_bBorder.strGroupName = _T("General");
	m_bBorder.strXUIElementName = m_bBorder.strPropWndName = _T("Border");

	m_bCheckBoxes.varDefaultValue = m_bCheckBoxes = false;
	m_bCheckBoxes.strXUIElementName = _T("IsCheckBoxes");
	m_bCheckBoxes.strPropWndName = _T("Has Check Boxes");

	m_bHasButtons.varDefaultValue = m_bHasButtons = false;
	m_bHasButtons.strXUIElementName = _T("IsHasButtons");
	m_bHasButtons.strPropWndName = _T("Has Buttons");

	m_bHasLines.varDefaultValue = m_bHasLines = false;
	m_bHasLines.strXUIElementName = _T("IsHasLines");
	m_bHasLines.strPropWndName = _T("Has Lines");

	m_bLinesAtRoot.varDefaultValue = m_bLinesAtRoot = false;
	m_bLinesAtRoot.strXUIElementName = _T("IsLineAtRoot");
	m_bLinesAtRoot.strPropWndName = _T("Line at Root");

	m_bFullRowSel.varDefaultValue = m_bFullRowSel = false;
	m_bFullRowSel.strXUIElementName = _T("IsFullRowSel");
	m_bFullRowSel.strPropWndName = _T("Full Row Select");
	m_bFullRowSel.strDescription = _T("Enables full-row selection in the tree view. The entire row of the selected item is highlighted, and clicking anywhere on an item's row causes it to be selected. This style cannot be used in conjunction with the TVS_HASLINES style.");

	m_bShowSelAlways.varDefaultValue = m_bShowSelAlways = false;
	m_bShowSelAlways.strXUIElementName = _T("IsShowSelAlways");
	m_bShowSelAlways.strPropWndName = _T("Always Show Select");

	m_bDisableDragDrop.varDefaultValue = m_bDisableDragDrop = false;
	m_bDisableDragDrop.strXUIElementName = _T("IsDisDragDrop");
	m_bDisableDragDrop.strPropWndName = _T("Disable Drap&Drop");

	m_bEditLabels.varDefaultValue = m_bEditLabels = false;
	m_bEditLabels.strXUIElementName = _T("IsEditLabels");
	m_bEditLabels.strPropWndName = _T("Edit Labels");

	m_bScroll.varDefaultValue = m_bScroll = true;
	m_bScroll.strXUIElementName = _T("IsScroll");
	m_bScroll.strPropWndName = _T("Scroll");

	m_bHoverScrollBarMode.varDefaultValue = m_bHoverScrollBarMode = false;
	m_bHoverScrollBarMode.strXUIElementName = _T("HoverScrollBarMode");
	m_bHoverScrollBarMode.strPropWndName = _T("Hover ScrollBar Mode");

	m_bNonEvenHeight.varDefaultValue = m_bNonEvenHeight = false;
	m_bNonEvenHeight.strXUIElementName = _T("IsNonEvenHeight");
	m_bNonEvenHeight.strPropWndName = _T("Non Even Height");

	m_bSingleExpand.varDefaultValue = m_bSingleExpand = false;
	m_bSingleExpand.strXUIElementName = _T("IsSingleExpand");
	m_bSingleExpand.strPropWndName = _T("Single Expand");

	m_bTrackSel.varDefaultValue = m_bTrackSel = false;
	m_bTrackSel.strXUIElementName = _T("IsTrackSel");
	m_bTrackSel.strPropWndName = _T("Hot Tracking");

	m_bInfoTip.varDefaultValue = m_bInfoTip = true;
	m_bInfoTip.strXUIElementName = _T("IsInfoTip");
	m_bInfoTip.strPropWndName = _T("Info ToolTip");

	m_bToolTips.varDefaultValue = m_bToolTips = false;
	m_bToolTips.strXUIElementName = _T("IsTooltips");
	m_bToolTips.strPropWndName = _T("ToolTip");

	m_bDefaultItemHeight.varDefaultValue = m_bDefaultItemHeight = true;
	m_bDefaultItemHeight.strXUIElementName = _T("DefaultItemHeight");
	m_bDefaultItemHeight.strPropWndName = _T("Default Item Height");

	m_nItemHeight.varDefaultValue = m_nItemHeight = (LONG)24;
	m_nItemHeight.strXUIElementName = _T("ItemHeight");
	m_nItemHeight.strPropWndName = _T("Item Height");
	m_nItemHeight.nMinValue = 1;
	m_nItemHeight.nMaxValue = 1920;

	m_strTag.varDefaultValue = m_strTag = _T("");
	m_strTag.strXUIElementName = _T("Tag");
	m_strTag.strPropWndName = _T("Tag");
	m_strTag.strDescription = _T("");

	m_bEnableItemLineEffect = false;
	m_bEnableItemLineEffect.bGroup = true;
	m_bEnableItemLineEffect.strGroupName = _T("Enable Item Line");
	m_bEnableItemLineEffect.strXUIElementName = _T("ItemLine");
	m_bEnableItemLineEffect.strXUIAttributeName = m_bEnableItemLineEffect.strPropWndName = _T("Enable");

	m_crItemLine = (LONG)RGB(128, 128, 128);
	m_crItemLine.vtEx = VT_EX_COLOR;
	m_crItemLine.bNeedToConvert = true;
	m_crItemLine.strXUIElementName = m_bEnableItemLineEffect.strXUIElementName;
	m_crItemLine.strXUIAttributeName = m_crItemLine.strPropWndName = _T("Color");

	m_strItemLineColorResID = _T("");
	m_strItemLineColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strItemLineColorResID.strXUIElementName = m_bEnableItemLineEffect.strXUIElementName;
	m_strItemLineColorResID.strXUIAttributeName = _T("ColorResID");
	m_strItemLineColorResID.strPropWndName = _T("Color ID");

	// Collapse and Expand image
	m_strCombineCollapseExpandedImage.varDefaultValue = m_strCombineCollapseExpandedImage = _T("");
	m_strCombineCollapseExpandedImage.vtEx = VT_EX_BITMAPPATH;
	m_strCombineCollapseExpandedImage.bGroup = true;
	m_strCombineCollapseExpandedImage.strGroupName = _T("Collapse and Expanded image");
	m_strCombineCollapseExpandedImage.strXUIElementName = _T("CombineCollapseExpandedImage");
	m_strCombineCollapseExpandedImage.strPropWndName = _T("Combine Collapse Expanded Image");

	m_strCollapseImageN = _T("");
	m_strCollapseImageN.vtEx = VT_EX_BITMAPPATH;
	m_strCollapseImageN.strXUIElementName = _T("CollapseImageN");
	m_strCollapseImageN.strPropWndName = _T("Normal Collapse Image");

	m_strCollapseImageH = _T("");
	m_strCollapseImageH.vtEx = VT_EX_BITMAPPATH;
	m_strCollapseImageH.strXUIElementName = _T("CollapseImageH");
	m_strCollapseImageH.strPropWndName = _T("Highlight Collapse Image");

	m_strCollapseImageS = _T("");
	m_strCollapseImageS.vtEx = VT_EX_BITMAPPATH;
	m_strCollapseImageS.strXUIElementName = _T("CollapseImageS");
	m_strCollapseImageS.strPropWndName = _T("Selected Collapse Image");

	m_strCollapseImageD = _T("");
	m_strCollapseImageD.vtEx = VT_EX_BITMAPPATH;
	m_strCollapseImageD.strXUIElementName = _T("CollapseImageD");
	m_strCollapseImageD.strPropWndName = _T("Disabled Collapse Image");

	m_strExpandedImageN = _T("");
	m_strExpandedImageN.vtEx = VT_EX_BITMAPPATH;
	m_strExpandedImageN.strXUIElementName = _T("ExpandedImageN");
	m_strExpandedImageN.strPropWndName = _T("Normal Expanded Image");

	m_strExpandedImageH = _T("");
	m_strExpandedImageH.vtEx = VT_EX_BITMAPPATH;
	m_strExpandedImageH.strXUIElementName = _T("ExpandedImageH");
	m_strExpandedImageH.strPropWndName = _T("Highlight Expanded Image");

	m_strExpandedImageS = _T("");
	m_strExpandedImageS.vtEx = VT_EX_BITMAPPATH;
	m_strExpandedImageS.strXUIElementName = _T("ExpandedImageS");
	m_strExpandedImageS.strPropWndName = _T("Selected Expanded Image");

	m_strExpandedImageD = _T("");
	m_strExpandedImageD.vtEx = VT_EX_BITMAPPATH;
	m_strExpandedImageD.strXUIElementName = _T("ExpandedImageD");
	m_strExpandedImageD.strPropWndName = _T("Disabled Expanded Image");

	// Text horizontal alignment
	m_eTextHorAlignModeN.varDefaultValue = m_eTextHorAlignModeN = TAH_DEFAULT;
	m_eTextHorAlignModeN.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignModeN.bReadOnly = true;
	m_eTextHorAlignModeN.bGroup = true;
	m_eTextHorAlignModeN.strGroupName = _T("Text Horizontal Align");
	m_eTextHorAlignModeN.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignModeN.strXUIAttributeName = _T("HorizontalN");
	m_eTextHorAlignModeN.strPropWndName = _T("Normal");
	m_eTextHorAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	m_eTextHorAlignModeH.varDefaultValue = m_eTextHorAlignModeH = TAH_DEFAULT;
	m_eTextHorAlignModeH.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignModeH.bReadOnly = true;
	m_eTextHorAlignModeH.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignModeH.strXUIAttributeName = _T("HorizontalH");
	m_eTextHorAlignModeH.strPropWndName = _T("Highlight");
	m_eTextHorAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	m_eTextHorAlignModeS.varDefaultValue = m_eTextHorAlignModeS = TAH_DEFAULT;
	m_eTextHorAlignModeS.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignModeS.bReadOnly = true;
	m_eTextHorAlignModeS.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignModeS.strXUIAttributeName = _T("HorizontalS");
	m_eTextHorAlignModeS.strPropWndName = _T("Selected");
	m_eTextHorAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	m_eTextHorAlignModeD.varDefaultValue = m_eTextHorAlignModeD = TAH_DEFAULT;
	m_eTextHorAlignModeD.vtEx = VT_EX_TEXT_ALIGN_HOR;
	m_eTextHorAlignModeD.bReadOnly = true;
	m_eTextHorAlignModeD.strXUIElementName = _T("TextAlignment");
	m_eTextHorAlignModeD.strXUIAttributeName = _T("HorizontalD");
	m_eTextHorAlignModeD.strPropWndName = _T("Disabled");
	m_eTextHorAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAH_DEFAULT).GetEnumString());
	m_eTextHorAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAH_LEFT).GetEnumString());
	m_eTextHorAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAH_RIGHT).GetEnumString());
	m_eTextHorAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAH_CENTER).GetEnumString());

	// Text vertical alignment
	m_eTextVerAlignModeN.varDefaultValue = m_eTextVerAlignModeN = TAV_DEFAULT;
	m_eTextVerAlignModeN.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignModeN.bReadOnly = true;
	m_eTextVerAlignModeN.bGroup = true;
	m_eTextVerAlignModeN.strGroupName = _T("Text Verizontal Align");
	m_eTextVerAlignModeN.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignModeN.strXUIAttributeName = _T("VerticalN");
	m_eTextVerAlignModeN.strPropWndName = _T("Normal");
	m_eTextVerAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());
	m_eTextVerAlignModeN.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());

	m_eTextVerAlignModeH.varDefaultValue = m_eTextVerAlignModeH = TAV_DEFAULT;
	m_eTextVerAlignModeH.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignModeH.bReadOnly = true;
	m_eTextVerAlignModeH.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignModeH.strXUIAttributeName = _T("VerticalH");
	m_eTextVerAlignModeH.strPropWndName = _T("Highlight");
	m_eTextVerAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());
	m_eTextVerAlignModeH.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());

	m_eTextVerAlignModeS.varDefaultValue = m_eTextVerAlignModeS = TAV_DEFAULT;
	m_eTextVerAlignModeS.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignModeS.bReadOnly = true;
	m_eTextVerAlignModeS.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignModeS.strXUIAttributeName = _T("VerticalS");
	m_eTextVerAlignModeS.strPropWndName = _T("Selected");
	m_eTextVerAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());
	m_eTextVerAlignModeS.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());

	m_eTextVerAlignModeD.varDefaultValue = m_eTextVerAlignModeD = TAV_DEFAULT;
	m_eTextVerAlignModeD.vtEx = VT_EX_TEXT_ALIGN_VER;
	m_eTextVerAlignModeD.bReadOnly = true;
	m_eTextVerAlignModeD.strXUIElementName = _T("TextAlignment");
	m_eTextVerAlignModeD.strXUIAttributeName = _T("VerticalD");
	m_eTextVerAlignModeD.strPropWndName = _T("Disabled");
	m_eTextVerAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAV_DEFAULT).GetEnumString());
	m_eTextVerAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAV_TOP).GetEnumString());
	m_eTextVerAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAV_CENTER).GetEnumString());
	m_eTextVerAlignModeD.lstStringOptions.AddTail(IUI_VARIANT(TAV_BOTTOM).GetEnumString());

	// Padding for Text
	m_rcTextMarginN.vtEx = VT_EX_RECT;
	m_rcTextMarginN.varDefaultValue = m_rcTextMarginN = CRect(0, 0, 0, 0);
	m_rcTextMarginN.bGroup = true;
	m_rcTextMarginN.strGroupName = _T("Item Text Margin");
	m_rcTextMarginN.strPropWndName = _T("Normal Item Text Margin");
	m_rcTextMarginN.strXUIElementName = _T("ItemTextMarginN");
	m_rcTextMarginN.nMinValue = -3840;
	m_rcTextMarginN.nMaxValue = 3840;

	m_rcTextMarginH.vtEx = VT_EX_RECT;
	m_rcTextMarginH.varDefaultValue = m_rcTextMarginH = CRect(0, 0, 0, 0);
	m_rcTextMarginH.strPropWndName = _T("Highlight Item Text Margin");
	m_rcTextMarginH.strXUIElementName = _T("ItemTextMarginH");
	m_rcTextMarginH.nMinValue = -3840;
	m_rcTextMarginH.nMaxValue = 3840;

	m_rcTextMarginS.vtEx = VT_EX_RECT;
	m_rcTextMarginS.varDefaultValue = m_rcTextMarginS = CRect(0, 0, 0, 0);
	m_rcTextMarginS.strPropWndName = _T("Selected Item Text Margin");
	m_rcTextMarginS.strXUIElementName = _T("ItemTextMarginS");
	m_rcTextMarginS.nMinValue = -3840;
	m_rcTextMarginS.nMaxValue = 3840;

	m_rcTextMarginD.vtEx = VT_EX_RECT;
	m_rcTextMarginD.varDefaultValue = m_rcTextMarginD = CRect(0, 0, 0, 0);
	m_rcTextMarginD.strPropWndName = _T("Disabled Item Text Margin");
	m_rcTextMarginD.strXUIElementName = _T("ItemTextMarginD");
	m_rcTextMarginD.nMinValue = -3840;
	m_rcTextMarginD.nMaxValue = 3840;

	// Selected Item
	m_eSelectedItemBkType.varDefaultValue = m_eSelectedItemBkType = BKT_COLOR;
	m_eSelectedItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eSelectedItemBkType.bGroup = true;
	m_eSelectedItemBkType.strGroupName = _T("Selected Item Background");
	m_eSelectedItemBkType.strXUIElementName = _T("SelectedItemBackground");
	m_eSelectedItemBkType.strXUIAttributeName = m_eSelectedItemBkType.strPropWndName = _T("Type");
	m_eSelectedItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eSelectedItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_crSelectedItemBkColor.varDefaultValue = m_crSelectedItemBkColor = (LONG)RGB(0, 0, 128);
	m_crSelectedItemBkColor.vtEx = VT_EX_COLOR;
	m_crSelectedItemBkColor.bNeedToConvert = true;
	m_crSelectedItemBkColor.strXUIElementName = m_eSelectedItemBkType.strXUIElementName;
	m_crSelectedItemBkColor.strXUIAttributeName = m_crSelectedItemBkColor.strPropWndName = _T("Color");

	m_strSelectedItemBkColorResID = _T("");
	m_strSelectedItemBkColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strSelectedItemBkColorResID.strXUIElementName = m_eSelectedItemBkType.strXUIElementName;
	m_strSelectedItemBkColorResID.strXUIAttributeName = _T("ColorResID");
	m_strSelectedItemBkColorResID.strPropWndName = _T("Color ID");

	m_strSelectedItemBkImageN = _T("");
	m_strSelectedItemBkImageN.vtEx = VT_EX_BITMAPPATH;
	m_strSelectedItemBkImageN.strXUIElementName = m_eSelectedItemBkType.strXUIElementName;
	m_strSelectedItemBkImageN.strXUIAttributeName = m_strSelectedItemBkImageN.strPropWndName = _T("NormalImage");

	m_strSelectedItemBkImageD = _T("");
	m_strSelectedItemBkImageD.vtEx = VT_EX_BITMAPPATH;
	m_strSelectedItemBkImageD.strXUIElementName = m_eSelectedItemBkType.strXUIElementName;
	m_strSelectedItemBkImageD.strXUIAttributeName = m_strSelectedItemBkImageD.strPropWndName = _T("DisabledImage");

	m_ptSelectedItemBkResize = CPoint(2, 2);
	m_ptSelectedItemBkResize.vtEx = VT_EX_POINT;
	m_ptSelectedItemBkResize.strXUIElementName = _T("SelectedBkResizePoint");
	m_ptSelectedItemBkResize.strPropWndName = _T("Resize Point");
	m_ptSelectedItemBkResize.nMinValue = 0;
	m_ptSelectedItemBkResize.nMaxValue = 1920;

	// Highlight Item
	m_bEnableHighlightItemEffect = false;
	m_bEnableHighlightItemEffect.bGroup = true;
	m_bEnableHighlightItemEffect.strGroupName = _T("Highlight Item Background");
	m_bEnableHighlightItemEffect.strXUIElementName = _T("HighlightItemBackground");
	m_bEnableHighlightItemEffect.strXUIAttributeName = m_bEnableHighlightItemEffect.strPropWndName = _T("Enable");

	m_eHighlightItemBkType = BKT_COLOR;
	m_eHighlightItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eHighlightItemBkType.strXUIElementName = m_bEnableHighlightItemEffect.strXUIElementName;
	m_eHighlightItemBkType.strXUIAttributeName = m_eHighlightItemBkType.strPropWndName = _T("Type");
	m_eHighlightItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eHighlightItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_crHighlightItemBkColor = (LONG)RGB(241, 241, 241);
	m_crHighlightItemBkColor.vtEx = VT_EX_COLOR;
	m_crHighlightItemBkColor.bNeedToConvert = true;
	m_crHighlightItemBkColor.strXUIElementName = m_bEnableHighlightItemEffect.strXUIElementName;
	m_crHighlightItemBkColor.strXUIAttributeName = m_crHighlightItemBkColor.strPropWndName = _T("Color");

	m_strHighlightItemBkColorResID = _T("");
	m_strHighlightItemBkColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strHighlightItemBkColorResID.strXUIElementName = m_bEnableHighlightItemEffect.strXUIElementName;
	m_strHighlightItemBkColorResID.strXUIAttributeName = _T("ColorResID");
	m_strHighlightItemBkColorResID.strPropWndName = _T("Color ID");

	m_strHighlightItemBkImageN = _T("");
	m_strHighlightItemBkImageN.vtEx = VT_EX_BITMAPPATH;
	m_strHighlightItemBkImageN.strXUIElementName = m_bEnableHighlightItemEffect.strXUIElementName;
	m_strHighlightItemBkImageN.strXUIAttributeName = m_strHighlightItemBkImageN.strPropWndName = _T("NormalImage");

	m_strHighlightItemBkImageD = _T("");
	m_strHighlightItemBkImageD.vtEx = VT_EX_BITMAPPATH;
	m_strHighlightItemBkImageD.strXUIElementName = m_bEnableHighlightItemEffect.strXUIElementName;
	m_strHighlightItemBkImageD.strXUIAttributeName = m_strHighlightItemBkImageD.strPropWndName = _T("NoFocusImage");

	m_ptHighlightItemBkResize = CPoint(2, 2);
	m_ptHighlightItemBkResize.vtEx = VT_EX_POINT;
	m_ptHighlightItemBkResize.strXUIElementName = _T("HighlightBkResizePoint");
	m_ptHighlightItemBkResize.strPropWndName = _T("Resize Point");
	m_ptHighlightItemBkResize.nMinValue = 0;
	m_ptHighlightItemBkResize.nMaxValue = 1920;

	// Root item
	m_bEnableRootItemEffect = false;
	m_bEnableRootItemEffect.bGroup = true;
	m_bEnableRootItemEffect.strGroupName = _T("Root Item Background");
	m_bEnableRootItemEffect.strXUIElementName = _T("RootItemBackground");
	m_bEnableRootItemEffect.strXUIAttributeName = m_bEnableRootItemEffect.strPropWndName = _T("Effect");

	m_strRootItemTextColorResID = _T("");
	m_strRootItemTextColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strRootItemTextColorResID.strXUIElementName = m_bEnableRootItemEffect.strXUIElementName;
	m_strRootItemTextColorResID.strXUIAttributeName = _T("TextColorResID");
	m_strRootItemTextColorResID.strPropWndName = _T("Text Color ID");

	m_eRootItemBkType = BKT_COLOR;
	m_eRootItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eRootItemBkType.strXUIElementName = m_bEnableRootItemEffect.strXUIElementName;
	m_eRootItemBkType.strXUIAttributeName = m_eRootItemBkType.strPropWndName = _T("Type");
	m_eRootItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eRootItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_strRootItemBKColorResID = _T("");
	m_strRootItemBKColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strRootItemBKColorResID.strXUIElementName = m_bEnableRootItemEffect.strXUIElementName;
	m_strRootItemBKColorResID.strXUIAttributeName = _T("ItemBkColorResID");
	m_strRootItemBKColorResID.strPropWndName = _T("Item Background Color ID");

	m_strRootItemBkImageF = _T("");
	m_strRootItemBkImageF.vtEx = VT_EX_BITMAPPATH;
	m_strRootItemBkImageF.strXUIElementName = m_bEnableRootItemEffect.strXUIElementName;
	m_strRootItemBkImageF.strXUIAttributeName = m_strRootItemBkImageF.strPropWndName = _T("FocusImage");

	m_strRootItemBkImageNonF = _T("");
	m_strRootItemBkImageNonF.vtEx = VT_EX_BITMAPPATH;
	m_strRootItemBkImageNonF.strXUIElementName = m_bEnableRootItemEffect.strXUIElementName;
	m_strRootItemBkImageNonF.strXUIAttributeName = m_strRootItemBkImageNonF.strPropWndName = _T("NonFocusImage");

	m_ptRootItemBkResize = CPoint(2, 2);
	m_ptRootItemBkResize.vtEx = VT_EX_POINT;
	m_ptRootItemBkResize.strXUIElementName = _T("RootItemBkImageResizePoint");
	m_ptRootItemBkResize.strPropWndName = _T("Resize Point");
	m_ptRootItemBkResize.nMinValue = 0;
	m_ptRootItemBkResize.nMaxValue = 1920;

	// Scroll bar
	m_bUseCustomScrollBar.varDefaultValue = m_bUseCustomScrollBar = false;
	m_bUseCustomScrollBar.bGroup = true;
	m_bUseCustomScrollBar.strGroupName = _T("Enable Custom Scroll Bar");
	m_bUseCustomScrollBar.strXUIElementName = _T("EnableCustomScrollBar");
	m_bUseCustomScrollBar.strPropWndName = _T("Enable");

	m_lScrollBarWidth.varDefaultValue = m_lScrollBarWidth = (LONG)17;
	m_lScrollBarWidth.strXUIElementName = _T("ScrollBarWidth");
	m_lScrollBarWidth.strPropWndName = _T("Scroll Bar Width");

	m_strHorScrollbarID = _T("");
	m_strHorScrollbarID.bReadOnly = true;
	m_strHorScrollbarID.vtEx = VT_EX_STYLE_HORSCROLLBAR;
	m_strHorScrollbarID.strXUIElementName = _T("HorizontalScrollbarStyleID");
	m_strHorScrollbarID.strPropWndName = _T("Horizontal Scrollbar");

	m_strVerScrollbarID = _T("");
	m_strVerScrollbarID.bReadOnly = true;
	m_strVerScrollbarID.vtEx = VT_EX_STYLE_VERSCROLLBAR;
	m_strVerScrollbarID.strXUIElementName = _T("VerticalScrollbarStyleID");
	m_strVerScrollbarID.strPropWndName = _T("Vertical Scrollbar");
}

TREECTRLPROPERTIES::~TREECTRLPROPERTIES()
{

}

bool TREECTRLPROPERTIES::IsEnableCustomScrollBar() const
{
	return m_bUseCustomScrollBar;
}

SBPROPERTIES::SBPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(SBPROPERTIES);
	m_eControlType = CT_SCROLLBAR;

	DisableBackgroundProperties(FALSE);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_bVertical.varDefaultValue = m_bVertical = false;
	m_bVertical.bGroup = true;
	m_bVertical.strGroupName = _T("General");
	m_bVertical.strXUIElementName = _T("IsVertical");
	m_bVertical.strPropWndName = _T("Vertical");

	m_strHorScrollbarStyleID = _T("");
	m_strHorScrollbarStyleID.bReadOnly = true;
	m_strHorScrollbarStyleID.vtEx = VT_EX_STYLE_HORSCROLLBAR;
	m_strHorScrollbarStyleID.strXUIElementName = _T("HorizontalScrollbarStyleID");
	m_strHorScrollbarStyleID.strPropWndName = _T("Horizontal Scrollbar");

	m_strVerScrollbarStyleID = _T("");
	m_strVerScrollbarStyleID.bReadOnly = true;
	m_strVerScrollbarStyleID.vtEx = VT_EX_STYLE_VERSCROLLBAR;
	m_strVerScrollbarStyleID.strXUIElementName = _T("VerticalScrollbarStyleID");
	m_strVerScrollbarStyleID.strPropWndName = _T("Vertical Scrollbar");
}

SBPROPERTIES::~SBPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

SBEXPROPERTIES::SBEXPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(SBEXPROPERTIES);
	m_eControlType = CT_SCROLLBAREX;
}

SBEXPROPERTIES::~SBEXPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

THMPROPERTIES::THMPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(THMPROPERTIES);
	m_eControlType = CT_THUMBNAIL;
	DisableBackgroundProperties(TRUE);

	m_bListStyle.varDefaultValue = m_bListStyle = false;
	m_bListStyle.bGroup = true;
	m_bListStyle.strGroupName = _T("General");
	m_bListStyle.strXUIElementName = _T("IsListStyle");
	m_bListStyle.strPropWndName = _T("List Style");

	m_bHoldCtrlKey.varDefaultValue = m_bHoldCtrlKey = false;
	m_bHoldCtrlKey.strXUIElementName = _T("IsHoldCtrlKey");
	m_bHoldCtrlKey.strPropWndName = _T("Hold Ctrl Key");

	m_bDragDrop.varDefaultValue = m_bDragDrop = false;
	m_bDragDrop.strXUIElementName = _T("IsDragDrop");
	m_bDragDrop.strPropWndName = _T("Enable Drag&Drop");

	m_lTotalItemWidth.varDefaultValue = m_lTotalItemWidth = (LONG)140;
	m_lTotalItemWidth.bGroup = true;
	m_lTotalItemWidth.strGroupName = _T("Item Settings");
	m_lTotalItemWidth.strXUIElementName = _T("TotalWidth");
	m_lTotalItemWidth.strPropWndName = _T("Total Width");
	m_lTotalItemWidth.nMinValue = -1920;
	m_lTotalItemWidth.nMaxValue = 1920;

	m_lTotalItemHeight.varDefaultValue = m_lTotalItemHeight = (LONG)140;
	m_lTotalItemHeight.strXUIElementName = _T("TotalHeight");
	m_lTotalItemHeight.strPropWndName = _T("Total Height");
	m_lTotalItemHeight.nMinValue = -1920;
	m_lTotalItemHeight.nMaxValue = 1920;

	// Normal Thumbnail Position
	m_lThumbnailLeftN.varDefaultValue = m_lThumbnailLeftN = (LONG)4;
	m_lThumbnailLeftN.bGroup = true;
	m_lThumbnailLeftN.strGroupName = _T("Normal Thumbnail Position");
	m_lThumbnailLeftN.strXUIElementName = _T("ThmRectLeftN");
	m_lThumbnailLeftN.strPropWndName = _T("Left");
	m_lThumbnailLeftN.nMinValue = -1920;
	m_lThumbnailLeftN.nMaxValue = 1920;

	m_lThumbnailTopN.varDefaultValue = m_lThumbnailTopN = (LONG)4;
	m_lThumbnailTopN.strXUIElementName = _T("ThmRectTopN");
	m_lThumbnailTopN.strPropWndName = _T("Top");
	m_lThumbnailTopN.nMinValue = -1920;
	m_lThumbnailTopN.nMaxValue = 1920;

	m_lThumbnailRightN.varDefaultValue = m_lThumbnailRightN = (LONG)96;
	m_lThumbnailRightN.strXUIElementName = _T("ThmRectRightN");
	m_lThumbnailRightN.strPropWndName = _T("Right");
	m_lThumbnailRightN.nMinValue = -1920;
	m_lThumbnailRightN.nMaxValue = 1920;

	m_lThumbnailBottomN.varDefaultValue = m_lThumbnailBottomN = (LONG)83;
	m_lThumbnailBottomN.strXUIElementName = _T("ThmRectBottomN");
	m_lThumbnailBottomN.strPropWndName = _T("Bottom");
	m_lThumbnailBottomN.nMinValue = -1920;
	m_lThumbnailBottomN.nMaxValue = 1920;

	// Highlight Thumbnail Position
	m_lThumbnailLeftH.varDefaultValue = m_lThumbnailLeftH = (LONG)4;
	m_lThumbnailLeftH.bGroup = true;
	m_lThumbnailLeftH.strGroupName = _T("Highlight Thumbnail Position");
	m_lThumbnailLeftH.strXUIElementName = _T("ThmRectLeftH");
	m_lThumbnailLeftH.strPropWndName = _T("Left");
	m_lThumbnailLeftH.nMinValue = -1920;
	m_lThumbnailLeftH.nMaxValue = 1920;

	m_lThumbnailTopH.varDefaultValue = m_lThumbnailTopH = (LONG)4;
	m_lThumbnailTopH.strXUIElementName = _T("ThmRectTopH");
	m_lThumbnailTopH.strPropWndName = _T("Top");
	m_lThumbnailTopH.nMinValue = -1920;
	m_lThumbnailTopH.nMaxValue = 1920;

	m_lThumbnailRightH.varDefaultValue = m_lThumbnailRightH = (LONG)96;
	m_lThumbnailRightH.strXUIElementName = _T("ThmRectRightH");
	m_lThumbnailRightH.strPropWndName = _T("Right");
	m_lThumbnailRightH.nMinValue = -1920;
	m_lThumbnailRightH.nMaxValue = 1920;

	m_lThumbnailBottomH.varDefaultValue = m_lThumbnailBottomH = (LONG)83;
	m_lThumbnailBottomH.strXUIElementName = _T("ThmRectBottomH");
	m_lThumbnailBottomH.strPropWndName = _T("Bottom");
	m_lThumbnailBottomH.nMinValue = -1920;
	m_lThumbnailBottomH.nMaxValue = 1920;

	// Normal Label Position
	m_lLabelLeftN.varDefaultValue = m_lLabelLeftN = (LONG)4;
	m_lLabelLeftN.bGroup = true;
	m_lLabelLeftN.strGroupName = _T("Normal Label Position");
	m_lLabelLeftN.strXUIElementName = _T("LblRectLeftN");
	m_lLabelLeftN.strPropWndName = _T("Left");
	m_lLabelLeftN.nMinValue = -1920;
	m_lLabelLeftN.nMaxValue = 1920;

	m_lLabelTopN.varDefaultValue = m_lLabelTopN = (LONG)90;
	m_lLabelTopN.strXUIElementName = _T("LblRectTopN");
	m_lLabelTopN.strPropWndName = _T("Top");
	m_lLabelTopN.nMinValue = -1920;
	m_lLabelTopN.nMaxValue = 1920;

	m_lLabelRightN.varDefaultValue = m_lLabelRightN = (LONG)96;
	m_lLabelRightN.strXUIElementName = _T("LblRectRightN");
	m_lLabelRightN.strPropWndName = _T("Right");
	m_lLabelRightN.nMinValue = -1920;
	m_lLabelRightN.nMaxValue = 1920;

	m_lLabelBottomN.varDefaultValue = m_lLabelBottomN = (LONG)116;
	m_lLabelBottomN.strXUIElementName = _T("LblRectBottomN");
	m_lLabelBottomN.strPropWndName = _T("Bottom");
	m_lLabelBottomN.nMinValue = -1920;
	m_lLabelBottomN.nMaxValue = 1920;

	// Highlight Label Position
	m_lLabelLeftH.varDefaultValue = m_lLabelLeftH = (LONG)4;
	m_lLabelLeftH.bGroup = true;
	m_lLabelLeftH.strGroupName = _T("Highlight Label Position");
	m_lLabelLeftH.strXUIElementName = _T("LblRectLeftH");
	m_lLabelLeftH.strPropWndName = _T("Left");
	m_lLabelLeftH.nMinValue = -1920;
	m_lLabelLeftH.nMaxValue = 1920;

	m_lLabelTopH.varDefaultValue = m_lLabelTopH = (LONG)90;
	m_lLabelTopH.strXUIElementName = _T("LblRectTopH");
	m_lLabelTopH.strPropWndName = _T("Top");
	m_lLabelTopH.nMinValue = -1920;
	m_lLabelTopH.nMaxValue = 1920;

	m_lLabelRightH.varDefaultValue = m_lLabelRightH = (LONG)96;
	m_lLabelRightH.strXUIElementName = _T("LblRectRightH");
	m_lLabelRightH.strPropWndName = _T("Right");
	m_lLabelRightH.nMinValue = -1920;
	m_lLabelRightH.nMaxValue = 1920;

	m_lLabelBottomH.varDefaultValue = m_lLabelBottomH = (LONG)116;
	m_lLabelBottomH.strXUIElementName = _T("LblRectBottomH");
	m_lLabelBottomH.strPropWndName = _T("Bottom");
	m_lLabelBottomH.nMinValue = -1920;
	m_lLabelBottomH.nMaxValue = 1920;

	// Item Background
	m_strItemBkN = _T("");
	m_strItemBkN.vtEx = VT_EX_BITMAPPATH;
	m_strItemBkN.bGroup = true;
	m_strItemBkN.strGroupName = _T("Item background Images");
	m_strItemBkN.strXUIElementName = _T("ItemBkN");
	m_strItemBkN.strXUIAttributeName = _T("RelativePath");
	m_strItemBkN.strPropWndName = _T("Normal");

	m_strItemBkH = _T("");
	m_strItemBkH.vtEx = VT_EX_BITMAPPATH;
	m_strItemBkH.strXUIElementName = _T("ItemBkH");
	m_strItemBkH.strXUIAttributeName = _T("RelativePath");
	m_strItemBkH.strPropWndName = _T("Highlight");

	m_strItemBkS = _T("");
	m_strItemBkS.vtEx = VT_EX_BITMAPPATH;
	m_strItemBkS.strXUIElementName = _T("ItemBkS");
	m_strItemBkS.strXUIAttributeName = _T("RelativePath");
	m_strItemBkS.strPropWndName = _T("Selected");

	m_strItemBkD = _T("");
	m_strItemBkD.vtEx = VT_EX_BITMAPPATH;
	m_strItemBkD.strXUIElementName = _T("ItemBkD");
	m_strItemBkD.strXUIAttributeName = _T("RelativePath");
	m_strItemBkD.strPropWndName = _T("Disabled");

	// Scroll bar
	m_bEnableScrollBar.varDefaultValue = m_bEnableScrollBar = true;
	m_bEnableScrollBar.bGroup = true;
	m_bEnableScrollBar.strGroupName = _T("Enable Scroll Bar");
	m_bEnableScrollBar.strXUIElementName = _T("EnableScrollBar");
	m_bEnableScrollBar.strPropWndName = _T("Enable");

	m_strVerScrollbarID = _T("");
	m_strVerScrollbarID.bReadOnly = true;
	m_strVerScrollbarID.vtEx = VT_EX_STYLE_VERSCROLLBAR;
	m_strVerScrollbarID.strXUIElementName = _T("VerticalScrollbarResID");
	m_strVerScrollbarID.strPropWndName = _T("Vertical Scrollbar");
}

THMPROPERTIES::~THMPROPERTIES()
{

}

bool THMPROPERTIES::IsEnableCustomScrollBar() const
{
	return m_bEnableScrollBar;
}

//////////////////////////////////////////////////////////////////////////

TREELISTPROPERTIES::TREELISTPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(TREELISTPROPERTIES);
	m_eControlType = CT_TREELIST;

	DisableBackgroundProperties(TRUE);

	m_bVariableItemHeight.varDefaultValue = m_bVariableItemHeight = false;
	m_bVariableItemHeight.bGroup = true;
	m_bVariableItemHeight.strGroupName = _T("General");
	m_bVariableItemHeight.strXUIElementName = _T("IsVariableItemHeight");
	m_bVariableItemHeight.strPropWndName = _T("Variable Item Height");
}

TREELISTPROPERTIES::~TREELISTPROPERTIES()
{

}

HTMLCTRLPROPERTIES::HTMLCTRLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(HTMLCTRLPROPERTIES);
	m_eControlType = CT_HTMLCTRL;
	DisableBackgroundProperties(FALSE);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);
	m_ptImageResize.bShowInPropertiesWnd = m_ptImageResize.bSaveToSkinFile = false;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = false;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = false;

	m_bHideBorder.varDefaultValue = m_bHideBorder = false;
	m_bHideBorder.bGroup = true;
	m_bHideBorder.strGroupName = _T("General");
	m_bHideBorder.strXUIElementName = _T("IsHideBorder");
	m_bHideBorder.strPropWndName = _T("Hide Border");

	m_bHideScrollBar.varDefaultValue = m_bHideScrollBar = false;
	m_bHideScrollBar.strXUIElementName = _T("IsHideScrollBar");
	m_bHideScrollBar.strPropWndName = _T("Hide ScrollBar");

	m_bHideContextMenu.varDefaultValue = m_bHideContextMenu = false;
	m_bHideContextMenu.strXUIElementName = _T("IsHideContextMenu");
	m_bHideContextMenu.strPropWndName = _T("Hide Context Menu");

	m_strStartPage = _T("http://www.iuishop.com");
	m_strStartPage.strXUIElementName = _T("StartPage");
	m_strStartPage.strPropWndName = _T("Start Page");
}

HTMLCTRLPROPERTIES::~HTMLCTRLPROPERTIES()
{

}

DATETIMECTRLPROPERTIES::DATETIMECTRLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(DATETIMECTRLPROPERTIES);
	m_eControlType = CT_DATETIMECTRL;
	DisableColorProperties(FALSE);

	m_eFormat.varDefaultValue = m_eFormat = DTF_SHORT_DATE;
	m_eFormat.vtEx = VT_EX_DATETIME_FORMAT;
	m_eFormat.bGroup = true;
	m_eFormat.strGroupName = _T("General");
	m_eFormat.strXUIElementName = _T("Format");
	m_eFormat.strPropWndName = _T("Format");
	m_eFormat.lstStringOptions.AddTail(IUI_VARIANT(DTF_SHORT_DATE).GetEnumString());
	m_eFormat.lstStringOptions.AddTail(IUI_VARIANT(DTF_LONG_DATE).GetEnumString());
	m_eFormat.lstStringOptions.AddTail(IUI_VARIANT(DTF_TIME).GetEnumString());

	m_bUseSpinControl.varDefaultValue = m_bUseSpinControl = false;
	m_bUseSpinControl.strXUIElementName = _T("IsUseSpinControl");
	m_bUseSpinControl.strPropWndName = _T("Use Spin Control");

	m_strSpinButtonStyleID.varDefaultValue = m_strSpinButtonStyleID = _T("");
	m_strSpinButtonStyleID.bReadOnly = true;
	m_strSpinButtonStyleID.vtEx = VT_EX_STYLE_SPIN;
	m_strSpinButtonStyleID.strXUIElementName = _T("SpinStyleID");
	m_strSpinButtonStyleID.strPropWndName = _T("Spin Button Style");

	m_bShowNone.varDefaultValue = m_bShowNone = false;
	m_bShowNone.strXUIElementName = _T("IsShowNone");
	m_bShowNone.strPropWndName = _T("Show None");

	m_bAllowEdit.varDefaultValue = m_bAllowEdit = false;
	m_bAllowEdit.strXUIElementName = _T("IsAllowEdit");
	m_bAllowEdit.strPropWndName = _T("Allow Edit");

	m_lButtonWidth.varDefaultValue = m_lButtonWidth = (LONG)16;
	m_lButtonWidth.strXUIElementName = _T("ButtonWidth");
	m_lButtonWidth.strPropWndName = _T("Button Width");
	m_lButtonWidth.nMinValue = 0;
	m_lButtonWidth.nMaxValue = 1920;

	m_ptTextOffset = CPoint(0, 0);
	m_ptTextOffset.vtEx = VT_EX_POINT;
	m_ptTextOffset.strXUIElementName = _T("TextOffset");
	m_ptTextOffset.strXUIAttributeName = _T("TextOffset");
	m_ptTextOffset.strPropWndName = _T("Text Offset");
	m_ptTextOffset.nMinValue = 0;
	m_ptTextOffset.nMaxValue = 1920;
}

DATETIMECTRLPROPERTIES::~DATETIMECTRLPROPERTIES()
{

}

WAVECTRLPROPERTIES::WAVECTRLPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(WAVECTRLPROPERTIES);
	m_eControlType = CT_WAVECTRL;
	DisableBackgroundProperties(TRUE);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_bEnableDragMove.varDefaultValue = m_bEnableDragMove = true;
	m_bEnableDragMove.bGroup = true;
	m_bEnableDragMove.strGroupName = _T("General");
	m_bEnableDragMove.strXUIElementName = _T("IsEnableDragMove");
	m_bEnableDragMove.strPropWndName = _T("Enable Drag Move");

	m_strTag.varDefaultValue = m_strTag = _T("");
	m_strTag.strXUIElementName = _T("Tag");
	m_strTag.strPropWndName = _T("Tag");
	m_strTag.strDescription = _T("");

	m_lGridWidth.varDefaultValue = m_lGridWidth = (LONG)30;
	m_lGridWidth.bGroup = true;
	m_lGridWidth.strGroupName = _T("Grid");
	m_lGridWidth.strXUIElementName = m_lGridHeight.strXUIElementName = m_lGridUnitX.strXUIElementName = m_lGridUnitY.strXUIElementName = _T("Grid");
	m_lGridWidth.strXUIAttributeName = m_lGridWidth.strPropWndName = _T("Width");
	m_lGridWidth.nMinValue = 1;
	m_lGridWidth.nMaxValue = 0x0FFFFFFF;

	m_lGridHeight.varDefaultValue = m_lGridHeight = (LONG)30;
	m_lGridHeight.strXUIAttributeName = m_lGridHeight.strPropWndName = _T("Height");
	m_lGridHeight.nMinValue = 1;
	m_lGridHeight.nMaxValue = 0x0FFFFFFF;

	m_crGridLine.varDefaultValue = m_crGridLine = (LONG)RGB(0, 128, 64);
	m_crGridLine.vtEx = VT_EX_COLOR;
	m_crGridLine.bNeedToConvert = true;
	m_crGridLine.strXUIElementName = m_lGridWidth.strXUIElementName;
	m_crGridLine.strXUIAttributeName = _T("GridLineColor");
	m_crGridLine.strPropWndName = _T("Grid Line Color");

	m_strGridLineColorResID = _T("");
	m_strGridLineColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strGridLineColorResID.strXUIElementName = m_lGridWidth.strXUIElementName;
	m_strGridLineColorResID.strXUIAttributeName = _T("GridLineColorResID");
	m_strGridLineColorResID.strPropWndName = _T("Grid Line Color ID");

	m_lGridUnitX.varDefaultValue = m_lGridUnitX = (LONG)1;
	m_lGridUnitX.strXUIAttributeName = _T("GridUnitX");
	m_lGridUnitX.strPropWndName = _T("Grid Unit X");
	m_lGridUnitX.nMinValue = 1;
	m_lGridUnitX.nMaxValue = 0x0FFFFFFF;

	m_lGridUnitY.varDefaultValue = m_lGridUnitY = (LONG)1;
	m_lGridUnitY.strXUIAttributeName = _T("GridUnitY");
	m_lGridUnitY.strPropWndName = _T("Grid Unit Y");
	m_lGridUnitY.nMinValue = 1;
	m_lGridUnitY.nMaxValue = 0x0FFFFFFF;

	m_lInfoAreaWidth.varDefaultValue = m_lInfoAreaWidth = (LONG)60;
	m_lInfoAreaWidth.bGroup = true;
	m_lInfoAreaWidth.strGroupName = _T("Information Area");
	m_lInfoAreaWidth.strXUIElementName = _T("InfoArea");
	m_lInfoAreaWidth.strXUIAttributeName = m_lInfoAreaWidth.strPropWndName = _T("Width");
	m_lInfoAreaWidth.nMinValue = 0;
	m_lInfoAreaWidth.nMaxValue = 0x0FFFFFFF;

	m_lInfoAreaHeight.varDefaultValue = m_lInfoAreaHeight = (LONG)20;
	m_lInfoAreaHeight.strXUIElementName = _T("InfoArea");
	m_lInfoAreaHeight.strXUIAttributeName = m_lInfoAreaHeight.strPropWndName = _T("Height");
	m_lInfoAreaHeight.nMinValue = 0;
	m_lInfoAreaHeight.nMaxValue = 0x0FFFFFFF;

	// Background
	m_crBackground.varDefaultValue = m_crBackground = (LONG)RGB(0, 0, 0);
	m_crBackground.vtEx = VT_EX_COLOR;
	m_crBackground.bNeedToConvert = true;
	m_crBackground.strXUIElementName = _T("Background");
	m_crBackground.strXUIAttributeName = m_crBackground.strPropWndName = _T("Color");

	m_strBackgroundColorResID = _T("");
	m_strBackgroundColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strBackgroundColorResID.bGroup = true;
	m_strBackgroundColorResID.strGroupName = _T("Background");
	m_strBackgroundColorResID.strXUIElementName = _T("Background");
	m_strBackgroundColorResID.strXUIAttributeName = _T("ColorResID");
	m_strBackgroundColorResID.strPropWndName = _T("Color ID");
}

WAVECTRLPROPERTIES::~WAVECTRLPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

MENUPROPERTIES::MENUPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(MENUPROPERTIES);
	m_eControlType = CT_MENU;

	m_rcControl.bShowInPropertiesWnd = m_rcControl.bSaveToSkinFile = false;
	m_bVisible.bShowInPropertiesWnd = m_bVisible.bSaveToSkinFile = false;
	m_bDisabled.bShowInPropertiesWnd = m_bDisabled.bSaveToSkinFile = false;
	m_bTabStop.bShowInPropertiesWnd = m_bTabStop.bSaveToSkinFile = false;
	m_bClipChildren.bShowInPropertiesWnd = m_bClipChildren.bSaveToSkinFile = false;
	m_bClipSiblings.bShowInPropertiesWnd = m_bClipSiblings.bSaveToSkinFile = false;
	m_eHorLayoutMode.bShowInPropertiesWnd = m_eHorLayoutMode.bSaveToSkinFile = false;
	m_eVerLayoutMode.bShowInPropertiesWnd = m_eVerLayoutMode.bSaveToSkinFile = false;
	m_ptImageResize.bShowInPropertiesWnd = m_ptImageResize.bSaveToSkinFile = false;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = false;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = false;

	DisableBackgroundProperties(FALSE);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	// Normal item
	m_crNormalItemText.varDefaultValue = m_crNormalItemText = (LONG)RGB(0, 0, 0);
	m_crNormalItemText.vtEx = VT_EX_COLOR;
	m_crNormalItemText.bGroup = true;
	m_crNormalItemText.strGroupName = _T("Normal Menu Item");
	m_crNormalItemText.strXUIElementName = _T("NormalMenuItem");
	m_crNormalItemText.strXUIAttributeName = _T("TextColor");
	m_crNormalItemText.strPropWndName = _T("Text Color");

	m_lfNormalItemText = (LOGFONT)CTRLPROPERTIES::m_lfN;
	m_lfNormalItemText.vtEx = VT_EX_FONT;
	m_lfNormalItemText.bNeedToConvert = false;
	m_lfNormalItemText.strXUIElementName = m_crNormalItemText.strXUIElementName;
	m_lfNormalItemText.strXUIAttributeName = m_lfNormalItemText.strPropWndName = _T("Font");

	m_eNormalItemBkType.varDefaultValue = m_eNormalItemBkType = BKT_COLOR;
	m_eNormalItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eNormalItemBkType.strXUIElementName = m_crNormalItemText.strXUIElementName;
	m_eNormalItemBkType.strXUIAttributeName = m_eNormalItemBkType.strPropWndName = _T("BackgroundType");
	m_eNormalItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eNormalItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_crNormalItemBkColor.varDefaultValue = m_crNormalItemBkColor = (LONG)RGB(249, 248, 247);
	m_crNormalItemBkColor.vtEx = VT_EX_COLOR;
	m_crNormalItemBkColor.strXUIElementName = m_crNormalItemText.strXUIElementName;
	m_crNormalItemBkColor.strXUIAttributeName = m_crNormalItemBkColor.strPropWndName = _T("BackgroundColor");

	m_strNormalItemBkImage = _T("");
	m_strNormalItemBkImage.vtEx = VT_EX_BITMAPPATH;
	m_strNormalItemBkImage.strXUIElementName = m_crNormalItemText.strXUIElementName;
	m_strNormalItemBkImage.strXUIAttributeName = _T("ImageRelativePath");
	m_strNormalItemBkImage.strPropWndName = _T("Image");

	m_ptNormalItemBkResize = CPoint(22, 2);
	m_ptNormalItemBkResize.vtEx = VT_EX_POINT;
	m_ptNormalItemBkResize.strXUIElementName = m_crNormalItemText.strXUIElementName;
	m_ptNormalItemBkResize.strXUIAttributeName = _T("ResizePoint");
	m_ptNormalItemBkResize.strPropWndName = _T("Resize Point");
	m_ptNormalItemBkResize.nMinValue = 0;
	m_ptNormalItemBkResize.nMaxValue = 1920;

	// Highlight item
	m_crHighlightItemText.varDefaultValue = m_crHighlightItemText = (LONG)RGB(255, 255, 255);
	m_crHighlightItemText.vtEx = VT_EX_COLOR;
	m_crHighlightItemText.bGroup = true;
	m_crHighlightItemText.strGroupName = _T("Highlight Menu Item");
	m_crHighlightItemText.strXUIElementName = _T("HighlightMenuItem");
	m_crHighlightItemText.strXUIAttributeName = _T("TextColor");
	m_crHighlightItemText.strPropWndName = _T("Text Color");

	m_lfHighlightItemText = (LOGFONT)CTRLPROPERTIES::m_lfN;
	m_lfHighlightItemText.vtEx = VT_EX_FONT;
	m_lfHighlightItemText.bNeedToConvert = false;
	m_lfHighlightItemText.strXUIElementName = m_crHighlightItemText.strXUIElementName;
	m_lfHighlightItemText.strXUIAttributeName = m_lfHighlightItemText.strPropWndName = _T("Font");

	m_eHighlightItemBkType.varDefaultValue = m_eHighlightItemBkType = BKT_COLOR;
	m_eHighlightItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eHighlightItemBkType.strXUIElementName = m_crHighlightItemText.strXUIElementName;
	m_eHighlightItemBkType.strXUIAttributeName = m_eHighlightItemBkType.strPropWndName = _T("BackgroundType");
	m_eHighlightItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eHighlightItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_crHighlightItemBkColor.varDefaultValue = m_crHighlightItemBkColor = (LONG)RGB(10, 36, 106);
	m_crHighlightItemBkColor.vtEx = VT_EX_COLOR;
	m_crHighlightItemBkColor.strXUIElementName = m_crHighlightItemText.strXUIElementName;
	m_crHighlightItemBkColor.strXUIAttributeName = m_crHighlightItemBkColor.strPropWndName = _T("BackgroundColor");

	m_strHighlightItemBkImage = _T("");
	m_strHighlightItemBkImage.vtEx = VT_EX_BITMAPPATH;
	m_strHighlightItemBkImage.strXUIElementName = m_crHighlightItemText.strXUIElementName;
	m_strHighlightItemBkImage.strXUIAttributeName = _T("ImageRelativePath");
	m_strHighlightItemBkImage.strPropWndName = _T("Image");

	m_ptHighlightItemBkResize = CPoint(22, 2);
	m_ptHighlightItemBkResize.vtEx = VT_EX_POINT;
	m_ptHighlightItemBkResize.strXUIElementName = m_crHighlightItemText.strXUIElementName;
	m_ptHighlightItemBkResize.strXUIAttributeName = _T("ResizePoint");
	m_ptHighlightItemBkResize.strPropWndName = _T("Resize Point");
	m_ptHighlightItemBkResize.nMinValue = 0;
	m_ptHighlightItemBkResize.nMaxValue = 1920;

	// Disabled item
	m_crDisabledItemText.varDefaultValue = m_crDisabledItemText = (LONG)RGB(166, 166, 166);
	m_crDisabledItemText.vtEx = VT_EX_COLOR;
	m_crDisabledItemText.bGroup = true;
	m_crDisabledItemText.strGroupName = _T("Disabled Menu Item");
	m_crDisabledItemText.strXUIElementName = _T("DisabledMenuItem");
	m_crDisabledItemText.strXUIAttributeName = _T("TextColor");
	m_crDisabledItemText.strPropWndName = _T("Text Color");

	m_lfDisabledItemText = (LOGFONT)CTRLPROPERTIES::m_lfN;
	m_lfDisabledItemText.vtEx = VT_EX_FONT;
	m_lfDisabledItemText.bNeedToConvert = false;
	m_lfDisabledItemText.strXUIElementName = m_crDisabledItemText.strXUIElementName;
	m_lfDisabledItemText.strXUIAttributeName = m_lfDisabledItemText.strPropWndName = _T("Font");

	m_eDisabledItemBkType.varDefaultValue = m_eDisabledItemBkType = BKT_COLOR;
	m_eDisabledItemBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eDisabledItemBkType.strXUIElementName = m_crDisabledItemText.strXUIElementName;
	m_eDisabledItemBkType.strXUIAttributeName = m_eDisabledItemBkType.strPropWndName = _T("BackgroundType");
	m_eDisabledItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eDisabledItemBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_crDisabledItemBkColor.varDefaultValue = m_crDisabledItemBkColor = (LONG)m_crNormalItemBkColor;
	m_crDisabledItemBkColor.vtEx = VT_EX_COLOR;
	m_crDisabledItemBkColor.strXUIElementName = m_crDisabledItemText.strXUIElementName;
	m_crDisabledItemBkColor.strXUIAttributeName = m_crDisabledItemBkColor.strPropWndName = _T("BackgroundColor");

	m_strDisabledItemBkImage = _T("");
	m_strDisabledItemBkImage.vtEx = VT_EX_BITMAPPATH;
	m_strDisabledItemBkImage.strXUIElementName = m_crDisabledItemText.strXUIElementName;
	m_strDisabledItemBkImage.strXUIAttributeName = _T("ImageRelativePath");
	m_strDisabledItemBkImage.strPropWndName = _T("Image");

	m_ptDisabledItemBkResize = CPoint(22, 2);
	m_ptDisabledItemBkResize.vtEx = VT_EX_POINT;
	m_ptDisabledItemBkResize.strXUIElementName = m_crDisabledItemText.strXUIElementName;
	m_ptDisabledItemBkResize.strXUIAttributeName = _T("ResizePoint");
	m_ptDisabledItemBkResize.strPropWndName = _T("Resize Point");
	m_ptDisabledItemBkResize.nMinValue = 0;
	m_ptDisabledItemBkResize.nMaxValue = 1920;

	// Separator
	m_eSeparatorBkType = BKT_COLOR;
	m_eSeparatorBkType.vtEx = VT_EX_BACKGROUND_TYPE;
	m_eSeparatorBkType.bGroup = true;
	m_eSeparatorBkType.strGroupName = _T("Separator");
	m_eSeparatorBkType.strXUIElementName = _T("Separator");
	m_eSeparatorBkType.strXUIAttributeName = m_eSeparatorBkType.strPropWndName = _T("BackgroundType");
	m_eSeparatorBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_COLOR).GetEnumString());
	m_eSeparatorBkType.lstStringOptions.AddTail(IUI_VARIANT(BKT_IMAGE).GetEnumString());

	m_crSeparator.varDefaultValue = m_crSeparator = (LONG)RGB(166, 166, 166);
	m_crSeparator.vtEx = VT_EX_COLOR;
	m_crSeparator.strXUIElementName = m_eSeparatorBkType.strXUIElementName;
	m_crSeparator.strXUIAttributeName = m_crSeparator.strPropWndName = _T("LineColor");

	m_nSeparatorLeftMargin.varDefaultValue = m_nSeparatorLeftMargin = (LONG)2;
	m_nSeparatorLeftMargin.strXUIElementName = m_eSeparatorBkType.strXUIElementName;
	m_nSeparatorLeftMargin.strXUIAttributeName = m_nSeparatorLeftMargin.strPropWndName = _T("LineLeftMargin");
	m_nSeparatorLeftMargin.nMinValue = 0;
	m_nSeparatorLeftMargin.nMinValue = 3840;

	m_nSeparatorRightMargin.varDefaultValue = m_nSeparatorRightMargin = (LONG)2;
	m_nSeparatorRightMargin.strXUIElementName = m_eSeparatorBkType.strXUIElementName;
	m_nSeparatorRightMargin.strXUIAttributeName = m_nSeparatorRightMargin.strPropWndName = _T("LineRightMargin");
	m_nSeparatorRightMargin.nMinValue = 0;
	m_nSeparatorRightMargin.nMinValue = 3840;

	m_strSeparatorBkImage = _T("");
	m_strSeparatorBkImage.vtEx = VT_EX_BITMAPPATH;
	m_strSeparatorBkImage.strXUIElementName = m_eSeparatorBkType.strXUIElementName;
	m_strSeparatorBkImage.strXUIAttributeName = _T("ImageRelativePath");
	m_strSeparatorBkImage.strPropWndName = _T("Image");

	m_ptSeparatorBkImageResize = CPoint(22, 2);
	m_ptSeparatorBkImageResize.vtEx = VT_EX_POINT;
	m_ptSeparatorBkImageResize.strXUIElementName = m_eSeparatorBkType.strXUIElementName;
	m_ptSeparatorBkImageResize.strXUIAttributeName = _T("ResizePoint");
	m_ptSeparatorBkImageResize.strPropWndName = _T("Resize Point");
	m_ptSeparatorBkImageResize.nMinValue = 0;
	m_ptSeparatorBkImageResize.nMaxValue = 1920;

	// Menu check image
	m_strMenuCheckImage = _T("");
	m_strMenuCheckImage.bGroup = true;
	m_strMenuCheckImage.strGroupName = _T("Menu Check Image");
	m_strMenuCheckImage.vtEx = VT_EX_BITMAPPATH;
	m_strMenuCheckImage.strXUIElementName = _T("MenuCheckImage");
	m_strMenuCheckImage.strXUIAttributeName = _T("ImageRelativePath");
	m_strMenuCheckImage.strPropWndName = _T("Image");

	// Icon Area
	m_nIconAreaWidth.varDefaultValue = m_nIconAreaWidth = (LONG)22;
	m_nIconAreaWidth.bGroup = true;
	m_nIconAreaWidth.strGroupName = _T("Icon Area");
	m_nIconAreaWidth.strXUIElementName = _T("IconArea");
	m_nIconAreaWidth.strXUIAttributeName = _T("Width");
	m_nIconAreaWidth.strPropWndName = _T("Icon Area Width");

	m_crIconAreaStart.varDefaultValue = m_crIconAreaStart = (LONG)RGB(249, 248, 247);
	m_crIconAreaStart.vtEx = VT_EX_COLOR;
	m_crIconAreaStart.strXUIElementName = m_nIconAreaWidth.strXUIElementName;
	m_crIconAreaStart.strXUIAttributeName = _T("StartColor");
	m_crIconAreaStart.strPropWndName = _T("Start Color");

	m_crIconAreaEnd.varDefaultValue = m_crIconAreaEnd = (LONG)RGB(215, 212, 204);
	m_crIconAreaEnd.vtEx = VT_EX_COLOR;
	m_crIconAreaEnd.strXUIElementName = m_nIconAreaWidth.strXUIElementName;
	m_crIconAreaEnd.strXUIAttributeName = _T("EndColor");
	m_crIconAreaEnd.strPropWndName = _T("End Color");

	m_bDrawIconBk.varDefaultValue = m_bDrawIconBk = false;
	m_bDrawIconBk.strXUIElementName = m_nIconAreaWidth.strXUIElementName;
	m_bDrawIconBk.strXUIAttributeName = _T("IsDrawIconBk");
	m_bDrawIconBk.strPropWndName = _T("Draw Icon Background");

	// Menu Border
	m_eMenuBorderStyle.varDefaultValue = m_eMenuBorderStyle = MBS_FLAT;
	m_eMenuBorderStyle.vtEx = VT_EX_MENU_BORDER_STYLE;
	m_eMenuBorderStyle.bGroup = true;
	m_eMenuBorderStyle.strGroupName = _T("Menu Border");
	m_eMenuBorderStyle.strXUIElementName = _T("MenuBorder");
	m_eMenuBorderStyle.strXUIAttributeName = _T("Style");
	m_eMenuBorderStyle.strPropWndName = _T("Style");
	m_eMenuBorderStyle.lstStringOptions.AddTail(IUI_VARIANT(MBS_NORMAL).GetEnumString());
	m_eMenuBorderStyle.lstStringOptions.AddTail(IUI_VARIANT(MBS_FLAT).GetEnumString());
	m_eMenuBorderStyle.lstStringOptions.AddTail(IUI_VARIANT(MBS_NONE).GetEnumString());

	m_crFlatMenuBorder.varDefaultValue = m_crFlatMenuBorder = (LONG)RGB(102, 102, 102);
	m_crFlatMenuBorder.vtEx = VT_EX_COLOR;
	m_crFlatMenuBorder.strXUIElementName = m_eMenuBorderStyle.strXUIElementName;
	m_crFlatMenuBorder.strXUIAttributeName = _T("FlatBorderColor");
	m_crFlatMenuBorder.strPropWndName = _T("Flat Border Color");

	// Height of menu item
	m_nMenuItemHeight.varDefaultValue = m_nMenuItemHeight = (LONG)23;
	m_nMenuItemHeight.bGroup = true;
	m_nMenuItemHeight.strGroupName = _T("General");
	m_nMenuItemHeight.strXUIElementName = _T("MenuItemHeight");
	m_nMenuItemHeight.strPropWndName = _T("Menu Item Height");
};

MENUPROPERTIES::~MENUPROPERTIES()
{

};

//////////////////////////////////////////////////////////////////////////

MENUBARPROPERTIES::MENUBARPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(MENUBARPROPERTIES);
	m_eControlType = CT_MENUBAR;

	// forbid some properties
	m_bTabStop.bShowInPropertiesWnd = m_bTabStop.bSaveToSkinFile = false;
	m_bSpecifyBackgroundImages.bShowInPropertiesWnd = m_bSpecifyBackgroundImages.bSaveToSkinFile = false;

	m_bCombineBackgroundImages.bGroup = true;
	m_bCombineBackgroundImages.strXUIElementName = _T("IsButtonCombineImages");
	m_bCombineBackgroundImages.strGroupName = _T("Menu bar Button Background");
	m_strBKCombine.strXUIElementName = _T("ButtonBkCombine");
	m_strBKN.strXUIElementName = _T("ButtonBkNormal");
	m_strBKH.strXUIElementName = _T("ButtonBkHighlight");
	m_strBKS.strXUIElementName = _T("ButtonBkSelected");
	m_strBKD.strXUIElementName = _T("ButtonBkDisabled");
	m_strResFontIDN.strXUIElementName = _T("ButtonFontResN");
	m_strResFontIDH.strXUIElementName = _T("ButtonFontResH");
	m_strResFontIDS.strXUIElementName = _T("ButtonFontResS");
	m_strResFontIDD.strXUIElementName = _T("ButtonFontResD");
	m_strResColorIDN.strXUIElementName = _T("ButtonColorResN");
	m_strResColorIDH.strXUIElementName = _T("ButtonColorResH");
	m_strResColorIDS.strXUIElementName = _T("ButtonColorResS");
	m_strResColorIDD.strXUIElementName = _T("ButtonColorResD");

	m_strResFontIDN.strGroupName = _T("Menu bar Button Font(Size)");
	m_strResColorIDN.strGroupName = _T("Menu bar Button Text Color");

	m_strMenuStyleID = _T("");
	m_strMenuStyleID.vtEx = VT_EX_STYLE_MENU;
	m_strMenuStyleID.bGroup = true;
	m_strMenuStyleID.strGroupName = _T("Menu Style");
	m_strMenuStyleID.strXUIElementName = _T("MenuStyle");
	m_strMenuStyleID.strPropWndName = _T("Menu Style");

	m_strBKCN = _T("");
	m_strBKCN.bGroup = true;
	m_strBKCN.strGroupName = _T("Menu bar Button Background");
	m_strBKCN.vtEx = VT_EX_BITMAPPATH;
	m_strBKCN.strXUIElementName = _T("ButtonBkCheckedNormal");
	m_strBKCN.strXUIAttributeName = _T("RelativePath");
	m_strBKCN.strPropWndName = _T("Checked Normal Image");

	m_strBKCH = _T("");
	m_strBKCH.vtEx = VT_EX_BITMAPPATH;
	m_strBKCH.strXUIElementName = _T("ButtonBkCheckedHighlight");
	m_strBKCH.strXUIAttributeName = _T("RelativePath");
	m_strBKCH.strPropWndName = _T("Checked Highlight Image");

	m_strBKCS = _T("");
	m_strBKCS.vtEx = VT_EX_BITMAPPATH;
	m_strBKCS.strXUIElementName = _T("ButtonBkCheckedSelected");
	m_strBKCS.strXUIAttributeName = _T("RelativePath");
	m_strBKCS.strPropWndName = _T("Checked Selected Image");

	m_strBKCD = _T("");
	m_strBKCD.vtEx = VT_EX_BITMAPPATH;
	m_strBKCD.strXUIElementName = _T("ButtonBkCheckedDisabled");
	m_strBKCD.strXUIAttributeName = _T("RelativePath");
	m_strBKCD.strPropWndName = _T("Checked Disabled Image");
};

MENUBARPROPERTIES::~MENUBARPROPERTIES()
{

};

//////////////////////////////////////////////////////////////////////////

SPLITTERPROPERTIES::SPLITTERPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(SPLITTERPROPERTIES);
	m_eControlType = CT_SPLITTER;

	// forbid some properties
	m_bVisible.bSaveToSkinFile = m_bVisible.bShowInPropertiesWnd = false;
	m_bDisabled.bSaveToSkinFile = m_bDisabled.bShowInPropertiesWnd = false;
	m_bTabStop.bSaveToSkinFile = m_bTabStop.bShowInPropertiesWnd = false;

	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_eOrientation.varDefaultValue = m_eOrientation = SO_WE;
	m_eOrientation.vtEx = VT_EX_SPLITTER_ORIENTATION;
	m_eOrientation.bReadOnly = true;
	m_eOrientation.bGroup = true;
	m_eOrientation.strGroupName = _T("General");
	m_eOrientation.strXUIElementName = _T("Orientation");
	m_eOrientation.strPropWndName = _T("Orientation");
	m_eOrientation.lstStringOptions.AddTail(IUI_VARIANT(SO_WE).GetEnumString());
	m_eOrientation.lstStringOptions.AddTail(IUI_VARIANT(SO_NS).GetEnumString());
}

SPLITTERPROPERTIES::~SPLITTERPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

CTRLPROPERTIES *STORY_FRAME::FindControl(LPCTSTR lpszID)
{
	CTRLPROPERTIES *pRet = NULL;

	int nCount = (int)m_vStoryFrameCtrlItem.size();
	for (int i = 0; i < nCount; ++i)
	{
		if (m_vStoryFrameCtrlItem[i]->m_strID == lpszID)
		{
			pRet = m_vStoryFrameCtrlItem[i];
			break;
		}
	}

	return pRet;
}

//////////////////////////////////////////////////////////////////////////

UIWNDPROPERTIES::UIWNDPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(UIWNDPROPERTIES);
	m_eControlType = CT_UIWND;
	m_bVisible.bShowInPropertiesWnd = m_bVisible.bSaveToSkinFile = false;
	m_bDisabled.bShowInPropertiesWnd = m_bDisabled.bSaveToSkinFile = false;
	m_bSpecifyBackgroundImages.bShowInPropertiesWnd = m_bSpecifyBackgroundImages.bSaveToSkinFile = false;
	m_bCombineBackgroundImages.bShowInPropertiesWnd = m_bCombineBackgroundImages.bSaveToSkinFile = false;
	m_strBKCombine.bShowInPropertiesWnd = m_strBKCombine.bSaveToSkinFile = false;
	m_rcControl = CRect(0, 0, 480, 320);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_bLoaded = false;
	m_bLoaded.bSaveToSkinFile = m_bLoaded.bShowInPropertiesWnd = false;

	m_bNoActivate.varDefaultValue = m_bNoActivate = false;
	m_bNoActivate.bGroup = TRUE;
	m_bNoActivate.strGroupName = _T("General");
	m_bNoActivate.strXUIElementName = _T("IsNoActivate");
	m_bNoActivate.strPropWndName = _T("No Activate");
	m_bNoActivate.strDescription = _T("Specifies that the window does not activate when the user clicks it.");

	m_bAppWindow.varDefaultValue = m_bAppWindow = true;
	m_bAppWindow.strXUIElementName = _T("IsAppWindow");
	m_bAppWindow.strPropWndName = _T("Application Window");
	m_bAppWindow.strDescription = _T("Specifies that the window, when visible, will have an entry in the taskbar.");

	m_bLayeredWindow.varDefaultValue = m_bLayeredWindow = true;
	m_bLayeredWindow.strXUIElementName = _T("IsLayeredWindow");
	m_bLayeredWindow.strPropWndName = _T("Layered Window");
	m_bLayeredWindow.strDescription = _T("Specifies that the window is a layered window.");

	m_bAlpha.varDefaultValue = m_bAlpha = false;
	m_bAlpha.strXUIElementName = _T("IsAlpha");
	m_bAlpha.strPropWndName = _T("Alpha");
	m_bAlpha.strDescription = _T("Use Alpha to determine the opacity of the layered window.");

	m_btAlphaValue.varDefaultValue = m_btAlphaValue = (LONG)255;
	m_btAlphaValue.strXUIElementName = _T("Alpha");
	m_btAlphaValue.strPropWndName = _T("Alpha Value");
	m_btAlphaValue.strDescription = _T("Alpha value used to describe the opacity of the layered window.When Alpha is 0, the window is completely transparent. When Alpha is 255, the window is opaque.");
	m_btAlphaValue.nMinValue = 0;
	m_btAlphaValue.nMaxValue = 255;

	m_bUpdateLayeredWindow.varDefaultValue = m_bUpdateLayeredWindow = false;
	m_bUpdateLayeredWindow.strXUIElementName = _T("IsUpdateLayeredWindow");
	m_bUpdateLayeredWindow.strPropWndName = _T("UpdateLayeredWindow");
	m_bUpdateLayeredWindow.strDescription = _T("Call 'UpdateLayeredWindow' to draw layered window.");

	m_eAlphaFormat.varDefaultValue = m_eAlphaFormat = AF_AC_SRC_ALPHA;
	m_eAlphaFormat.vtEx = VT_EX_ALPHA_FORMAT;
	m_eAlphaFormat.strXUIElementName = _T("AlphaFormat");
	m_eAlphaFormat.strPropWndName = _T("Alpha Format");
	m_eAlphaFormat.lstStringOptions.AddTail(IUI_VARIANT(AF_NONE).GetEnumString());
	m_eAlphaFormat.lstStringOptions.AddTail(IUI_VARIANT(AF_AC_SRC_ALPHA).GetEnumString());

	m_bTopmost.varDefaultValue = m_bTopmost = false;
	m_bTopmost.strXUIElementName = _T("IsTopmost");
	m_bTopmost.strPropWndName = _T("Topmost");
	m_bTopmost.strDescription = _T("Specifies that the window will take z-order precedence over all non-topmost windows.");

	m_bAcceptFiles.varDefaultValue = m_bAcceptFiles = false;
	m_bAcceptFiles.strXUIElementName = _T("IsAcceptFiles");
	m_bAcceptFiles.strPropWndName = _T("Accept Files");

	m_bToolWindow.varDefaultValue = m_bToolWindow = false;
	m_bToolWindow.strXUIElementName = _T("IsToolWindow");
	m_bToolWindow.strPropWndName = _T("Tool Window");

	m_lTitleBarHeight.varDefaultValue = m_lTitleBarHeight = (LONG)0;
	m_lTitleBarHeight.strUIKeyName = m_lTitleBarHeight.strXUIElementName = _T("TitleBarHeight");
	m_lTitleBarHeight.strPropWndName = _T("Title Bar Height");
	m_lTitleBarHeight.nMinValue = -1;
	m_lTitleBarHeight.nMaxValue = 7680;

	m_strTag.varDefaultValue = m_strTag = _T("");
	m_strTag.strXUIElementName = _T("Tag");
	m_strTag.strPropWndName = _T("Tag");
	m_strTag.strDescription = _T("");

	m_strBKN.bGroup = true;
	m_strBKN.strGroupName = _T("Background");

	m_bTransparentBk.varDefaultValue = m_bTransparentBk = false;
	m_bTransparentBk.bGroup = true;
	m_bTransparentBk.strGroupName = _T("Background");
	m_bTransparentBk.strXUIElementName = _T("IsBkTransparent");
	m_bTransparentBk.strPropWndName = _T("Transparent Background");

	m_bDrawBKColor.varDefaultValue = m_bDrawBKColor = false;
	m_bDrawBKColor.strXUIElementName = _T("IsDrawBKColor");
	m_bDrawBKColor.strPropWndName = _T("Draw Background Color");

	m_strBackgoundColorResID = _T("");
	m_strBackgoundColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strBackgoundColorResID.strXUIElementName = _T("BackgroundColorID");
	m_strBackgoundColorResID.strPropWndName = _T("Background Color ID");

	m_strBKNo = _T("");
	m_strBKNo.vtEx = VT_EX_BITMAPPATH;
	m_strBKNo.strXUIElementName = _T("BackgroundHide");
	m_strBKNo.strXUIAttributeName = _T("RelativePath");
	m_strBKNo.strPropWndName = _T("No Controls");

	m_bUseMaskColor.varDefaultValue = m_bUseMaskColor = false;
	m_bUseMaskColor.bGroup = true;
	m_bUseMaskColor.strGroupName = _T("Mask Color");
	m_bUseMaskColor.strUIKeyName = m_bUseMaskColor.strXUIElementName = _T("IsUseMaskColor");
	m_bUseMaskColor.strPropWndName = _T("Use Mask Color");
	m_bUseMaskColor.strDescription = _T("");

	m_crMask = (LONG)RGB(255, 0, 255);
	m_crMask.vtEx = VT_EX_COLOR;
	m_crMask.bNeedToConvert = true;
	m_crMask.strUIKeyName = m_crMask.strXUIElementName = _T("MaskColor");
	m_crMask.strPropWndName = _T("Mask Color");

	m_strMaskColorResID = _T("");
	m_strMaskColorResID.vtEx = VT_EX_RESOURCE_COLOR;
	m_strMaskColorResID.strXUIElementName = _T("MaskColorResID");
	m_strMaskColorResID.strPropWndName = _T("Mask Color ID");

	m_strShadowImage.varDefaultValue = m_strShadowImage = _T("");
	m_strShadowImage.vtEx = VT_EX_BITMAPPATH;
	m_strShadowImage.bGroup = true;
	m_strShadowImage.strGroupName = _T("Shadow border");
	m_strShadowImage.strXUIElementName = _T("ShadowBorder");
	m_strShadowImage.strXUIAttributeName = _T("RelativePath");
	m_strShadowImage.strDescription = _T("Specify an image as shadow border.");
	m_strShadowImage.strPropWndName = _T("Shadow border image");

	m_ptShadowResize.vtEx = VT_EX_POINT;
	m_ptShadowResize.varDefaultValue = m_ptShadowResize = CPoint(15, 15);
	m_ptShadowResize.strXUIElementName = _T("ShadowImageResizePoint");
	m_ptShadowResize.strPropWndName = _T("Shadow Border Image Resize Point");
	m_ptShadowResize.nMinValue = 0;
	m_ptShadowResize.nMaxValue = 3840;


	m_rcShadow.vtEx = VT_EX_RECT;
	m_rcShadow.varDefaultValue = m_rcShadow = CRect(0, 0, 0, 0);
	m_rcShadow.strPropWndName = _T("Shadow Size");
	m_rcShadow.strXUIElementName = _T("ShadowSize");
	m_rcShadow.nMinValue = -3840;
	m_rcShadow.nMaxValue = 3840;

	m_lRoundCornerX.varDefaultValue = m_lRoundCornerX = (LONG)0;
	m_lRoundCornerX.strXUIElementName = _T("RoundCornerX");
	m_lRoundCornerX.strPropWndName = _T("Round Corner X");
	m_lRoundCornerX.nMinValue = -1920;
	m_lRoundCornerX.nMaxValue = 1920;

	m_lRoundCornerY.varDefaultValue = m_lRoundCornerY = (LONG)0;
	m_lRoundCornerY.strXUIElementName = _T("RoundCornerY");
	m_lRoundCornerY.strPropWndName = _T("Round Corner Y");
	m_lRoundCornerY.nMinValue = -1920;
	m_lRoundCornerY.nMaxValue = 1920;

	m_bResizingBorder.varDefaultValue = m_bResizingBorder = false;
	m_bResizingBorder.bGroup = true;
	m_bResizingBorder.strGroupName = _T("Window Resize");
	m_bResizingBorder.strXUIElementName = _T("IsResizingBorder");
	m_bResizingBorder.strPropWndName = _T("Resizing Border");

	m_bKeepChildrenAspectRatio.varDefaultValue = m_bKeepChildrenAspectRatio = false;
	m_bKeepChildrenAspectRatio.strXUIElementName = _T("IsKeepChildrenAspectRatio");
	m_bKeepChildrenAspectRatio. strPropWndName = _T("Keep Children Aspect Ratio");

	m_lMaxHorRatio.varDefaultValue = m_lMaxHorRatio = (LONG)100;
	m_lMaxHorRatio.strXUIElementName = _T("MaxHorRatio");
	m_lMaxHorRatio.strPropWndName = _T("Max Horizontal Ratio(100%)");
	m_lMaxHorRatio.nMinValue = 99;
	m_lMaxHorRatio.nMaxValue = 0x0FFFFFFF;

	m_lMaxVerRatio.varDefaultValue = m_lMaxVerRatio = (LONG)100;
	m_lMaxVerRatio.strXUIElementName = _T("MaxVerRatio");
	m_lMaxVerRatio.strPropWndName = _T("Max Vertical Ratio(100%)");
	m_lMaxVerRatio.nMinValue = 99;
	m_lMaxVerRatio.nMaxValue = 0x0FFFFFFF;

	m_bWindowResizePointSameAsImageResizePoint.varDefaultValue = m_bWindowResizePointSameAsImageResizePoint = true;
	m_bWindowResizePointSameAsImageResizePoint.strXUIElementName = _T("WinResizePtSameImgResizePt");
	m_bWindowResizePointSameAsImageResizePoint.strPropWndName = _T("Window Resize Point Same as Image");

	m_ptWindowResize = CPoint(0, 0);
	m_ptWindowResize.vtEx = VT_EX_POINT;
	m_ptWindowResize.strUIKeyName = m_ptWindowResize.strXUIElementName = _T("WindowResizingPoint");
	m_ptWindowResize.strPropWndName = _T("Resize Point");
	m_ptWindowResize.nMinValue = 0;
	m_ptWindowResize.nMaxValue = 1920;

	m_eWindowType.varDefaultValue = m_eWindowType = WT_NORMAL;
	m_eWindowType.vtEx = VT_EX_WINDOW_TYPE;
	m_eWindowType.bReadOnly = true;
	m_eWindowType.bGroup = true;
	m_eWindowType.strGroupName = _T("General");
	m_eWindowType.strUIKeyName = m_eWindowType.strXUIElementName = _T("WindowType");
	m_eWindowType.strPropWndName = _T("Window Type");
	m_eWindowType.lstStringOptions.AddTail(IUI_VARIANT(WT_NORMAL).GetEnumString());
	m_eWindowType.lstStringOptions.AddTail(IUI_VARIANT(WT_MDIFRAME).GetEnumString());
	m_eWindowType.lstStringOptions.AddTail(IUI_VARIANT(WT_MDICLIENT).GetEnumString());
	m_eWindowType.lstStringOptions.AddTail(IUI_VARIANT(WT_MDICHILDFRAME).GetEnumString());
	m_eWindowType.lstStringOptions.AddTail(IUI_VARIANT(WT_MDIVIEW).GetEnumString());
	m_eWindowType.lstStringOptions.AddTail(IUI_VARIANT(WT_FORMWND).GetEnumString());

	m_rcClientArea.vtEx = VT_EX_RECT_WH;
	m_rcClientArea.varDefaultValue = m_rcClientArea = CRect(0, 0, 0, 0);
	m_rcClientArea.strPropWndName = _T("Client Area");
	m_rcClientArea.strXUIElementName = _T("ClientArea");
	m_rcClientArea.nMinValue = -3840;
	m_rcClientArea.nMaxValue = 3840;
}

UIWNDPROPERTIES::~UIWNDPROPERTIES()
{

}

int UIWNDPROPERTIES::SwapControlItem(int nFirst, int nLast)
{
	int nCount = (int)m_vControls.size();
	if (nFirst < 0 || nFirst >= nCount || nLast < 0 || nLast >= nCount)
	{
		return -1;
	}

	if (nFirst == nLast)
	{
		return 1;
	}

	CTRLPROPERTIES *pTempCtrl = m_vControls[nFirst];
	m_vControls[nFirst] = m_vControls[nLast];
	m_vControls[nLast] = pTempCtrl;

	return 0;
}

int UIWNDPROPERTIES::GetControlIndex(LPCTSTR lpszCtrlID) const
{
	size_t nIndex = -1;

	size_t nCount = m_vControls.size();
	for (size_t i = 0; i < nCount ; ++i)
	{
		CTRLPROPERTIES *p = m_vControls[i];
		if (p->m_strID == lpszCtrlID)
		{
			nIndex = i;
			break;
		}
	}

	return (int)nIndex;
}

STORY_FRAME *UIWNDPROPERTIES::FindStoryFrame(int nFrame)
{
	STORY_FRAME *pRet = NULL;

	int nCount = (int)m_vStoryFrame.size();
	for (int i = 0; i < nCount; ++i)
	{
		STORY_FRAME *pItem = m_vStoryFrame[i];
		if (pItem->nFrameIndex == nFrame)
		{
			pRet = pItem;
			break;
		}
	}

	return pRet;
}

bool StoryTimeCompare(STORY_FRAME *p1, STORY_FRAME *p2)
{
	return p1->nFrameIndex < p2->nFrameIndex;
}

int UIWNDPROPERTIES::InsertStoryFrame(STORY_FRAME *pItem)
{
	STORY_FRAME *pRet = FindStoryFrame(pItem->nFrameIndex);
	if (pRet != NULL)
	{
		return -1;
	}

	// Sort by STORY_FRAME::nFrameIndex.
	m_vStoryFrame.push_back(pItem);
	std::sort(m_vStoryFrame.begin(), m_vStoryFrame.end(), StoryTimeCompare);

	return 0;
}

CTRLPROPERTIES *UIWNDPROPERTIES::FindLeftNearControlInStoryBoard(
	LPCTSTR lpszCtrlID, int nCurFrame, BOOL bIncludeCurFrame)
{
	CTRLPROPERTIES *pRet = NULL;

	int nFrameCount = (int)m_vStoryFrame.size();
	for (int i = nFrameCount - 1; i >= 0; --i)
	{
		STORY_FRAME *pFrame = m_vStoryFrame[i];

		if (bIncludeCurFrame)
		{
			if (pFrame->nFrameIndex > nCurFrame)
			{
				continue;
			}
		}
		else
		{
			if (pFrame->nFrameIndex >= nCurFrame)
			{
				continue;
			}
		}

		CTRLPROPERTIES *pCtrlProp = pFrame->FindControl(lpszCtrlID);
		if (pCtrlProp != NULL)
		{
			pRet = pCtrlProp;
			break;
		}
	}

	if (pRet != NULL)
	{
		return pRet;
	}

	return GetDlgItem(lpszCtrlID);
}

const CTRLPROPERTIES *UIWNDPROPERTIES::FindLeftNearControlInStoryBoard(
	LPCTSTR lpszCtrlID, int nCurFrame, BOOL bIncludeCurFrame) const
{
	CTRLPROPERTIES *pRet = NULL;

	int nFrameCount = (int)m_vStoryFrame.size();
	for (int i = nFrameCount - 1; i >= 0; --i)
	{
		STORY_FRAME *pFrame = m_vStoryFrame[i];

		if (bIncludeCurFrame)
		{
			if (pFrame->nFrameIndex > nCurFrame)
			{
				continue;
			}
		}
		else
		{
			if (pFrame->nFrameIndex >= nCurFrame)
			{
				continue;
			}
		}

		CTRLPROPERTIES *pCtrlProp = pFrame->FindControl(lpszCtrlID);
		if (pCtrlProp != NULL)
		{
			pRet = pCtrlProp;
			break;
		}
	}

	if (pRet != NULL)
	{
		return pRet;
	}

	return GetDlgItem(lpszCtrlID);
}

CTRLPROPERTIES *UIWNDPROPERTIES::AllocCtrlPropertiesAtStoryFrame(int nCurFrame, LPCTSTR lpszCtrlID)
{
	CTRLPROPERTIES *pRet = NULL;

	CTRLPROPERTIES *pLeftNearProp = FindLeftNearControlInStoryBoard(lpszCtrlID, nCurFrame, FALSE);
	ASSERT(pLeftNearProp != NULL);

	CONTROL_TYPE eControlType = pLeftNearProp->m_eControlType;
	if (eControlType == CT_PUSHBUTTON)
	{
		pRet = new BTNPROPERTIES;
	}
	else if (eControlType == CT_CHECK)
	{
		pRet = new CHKPROPERTIES;
	}
	else if (eControlType == CT_RADIO)
	{
		pRet = new RADPROPERTIES;
	}
	else if (eControlType == CT_STATIC)
	{
		pRet = new TXTPROPERTIES;
	}
	else if (eControlType == CT_EDIT)
	{
		pRet = new EDTPROPERTIES;
	}
	else if (eControlType == CT_COMBOBOX)
	{
		pRet = new CMBPROPERTIES;
	}
	else if (eControlType == CT_PROGRESS)
	{
		pRet = new PRGPROPERTIES;
	}
	else if (eControlType == CT_SLIDER)
	{
		pRet = new SLDPROPERTIES;
		((SLDPROPERTIES *)pRet)->m_eExtendedStyle.bShowInPropertiesWnd = false;
		((SLDPROPERTIES *)pRet)->m_eExtendedStyle.bSaveToSkinFile = false;
		((SLDPROPERTIES *)pRet)->m_bEnableTipWindow.bShowInPropertiesWnd = false;
		((SLDPROPERTIES *)pRet)->m_bEnableTipWindow.bSaveToSkinFile = false;
		((SLDPROPERTIES *)pRet)->m_sizeTipWindowOffset.bShowInPropertiesWnd = false;
		((SLDPROPERTIES *)pRet)->m_sizeTipWindowOffset.bSaveToSkinFile = false;
		((SLDPROPERTIES *)pRet)->m_bVertical.bGroup = true;
		((SLDPROPERTIES *)pRet)->m_bVertical.strGroupName = _T("General");
	}
	else if (eControlType == CT_SPIN)
	{
		pRet = new SPNPROPERTIES;
	}
	else if (eControlType == CT_HOTKEY)
	{
		pRet = new HOTPROPERTIES;
	}
	else if (eControlType == CT_IPADDRESS)
	{
		pRet = new IPAPROPERTIES;
	}
	else if (eControlType == CT_SLIDEREX)
	{
		pRet = new SLDEXPROPERTIES;
	}
	else if (eControlType == CT_LISTCTRL)
	{
		pRet = new LSTCTRLPROPERTIES;
	}
	else if (eControlType == CT_TREECTRL)
	{
		pRet = new TREECTRLPROPERTIES;
	}
	else if (eControlType == CT_SCROLLBAREX)
	{
		pRet = new SBEXPROPERTIES;
	}
	else if (eControlType == CT_THUMBNAIL)
	{
		pRet = new THMPROPERTIES;
	}
	else if (eControlType == CT_TREELIST)
	{
		pRet = new TREELISTPROPERTIES;
	}
	else if (eControlType == CT_WL_RICHEDIT)
	{
		pRet = new RICHEDITPROPERTIES;
	}
	else if (eControlType == CT_WL_RICHEDIT_IM)
	{
		pRet = new IMREPROPERTIES;
	}
	else if (eControlType == CT_HTMLCTRL)
	{
		pRet = new HTMLCTRLPROPERTIES;
	}
	else if (eControlType == CT_DATETIMECTRL)
	{
		pRet = new DATETIMECTRLPROPERTIES;
	}
	else if (eControlType == CT_WAVECTRL)
	{
		pRet = new WAVECTRLPROPERTIES;
	}
	else if (eControlType == CT_PICTURE)
	{
		pRet = new PICWLPROPERTIES;
		pRet->m_eControlType = CT_PICTURE;
	}
	else if (eControlType == CT_MENUBAR)
	{
		pRet = new MENUBARPROPERTIES;
	}
	else if (eControlType == CT_SPLITTER)
	{
		pRet = new SPLITTERPROPERTIES;
	}
	else if (eControlType == CT_WL_RECTCTRL)
	{
		pRet = new RECTWLPROPERTIES;
	}
	else if (eControlType == CT_WL_SPLITTER)
	{
		pRet = new SPLITTERWLPROPERTIES;
	}
	else if (eControlType == CT_WL_LINE)
	{
		pRet = new LINEWLPROPERTIES;
	}
	else if (eControlType == CT_WL_TEXT)
	{
		pRet = new TXTPROPERTIES;
		pRet->m_eControlType = CT_WL_TEXT;
	}
	else if (eControlType == CT_WL_BUTTON)
	{
		pRet = new BTNPROPERTIES;
		pRet->m_eControlType = CT_WL_BUTTON;
	}
	else if (eControlType == CT_WL_CHECK)
	{
		pRet = new CHKPROPERTIES;
		pRet->m_eControlType = CT_WL_CHECK;
	}
	else if (eControlType == CT_WL_RADIO)
	{
		pRet = new RADPROPERTIES;
		pRet->m_eControlType = CT_WL_RADIO;
	}
	else if (eControlType == CT_WL_SLIDER)
	{
		pRet = new SLDEXPROPERTIES;
		pRet->m_eControlType = CT_WL_SLIDER;
	}
	else if (eControlType == CT_WL_PICTURE)
	{
		pRet = new PICWLPROPERTIES;
	}
	else if (eControlType == CT_WL_RICHEDIT)
	{
		pRet = new RICHEDITPROPERTIES;
		pRet->m_eControlType = CT_WL_RICHEDIT;
	}
	else if (eControlType == CT_WL_RICHEDIT_IM)
	{
		pRet = new IMREPROPERTIES;
		pRet->m_eControlType = CT_WL_RICHEDIT_IM;
	}
	else if (eControlType == CT_TASK_WND_MGR)
	{
		pRet = new TWMGRPROPERTIES;
		pRet->m_eControlType = CT_TASK_WND_MGR;
	}
	else if (eControlType == CT_PNL_DOCK)
	{
		pRet = new DOCKPNLPROPERTIES;
		pRet->m_eControlType = CT_PNL_DOCK;
	}
	else if (eControlType == CT_UNIFORM_GRID)
	{
		pRet = new UGRIDPNLPROPERTIES;
		pRet->m_eControlType = CT_UNIFORM_GRID;
	}
	else if (eControlType == CT_PNL_STACK)
	{
		pRet = new STACKPNLPROPERTIES;
		pRet->m_eControlType = CT_PNL_STACK;
	}

	if (pRet != NULL)
	{
		pRet->m_strID = pLeftNearProp->m_strID;

		// Use left near frame to init this properties
		*pRet = *pLeftNearProp;
	}

	return pRet;
}

CTRLPROPERTIES *UIWNDPROPERTIES::GetDlgItem(LPCTSTR lpszCtrlID)
{
	CTRLPROPERTIES *pRet = NULL;

	int nCtrlCount = (int)m_vControls.size();
	for (int i = 0; i < nCtrlCount; ++i)
	{
		CTRLPROPERTIES *pProp = m_vControls[i];

		if (CString((LPCTSTR)pProp->m_strID) == lpszCtrlID)
		{
			pRet = pProp;
			break;
		}
	}

	return pRet;
}

const CTRLPROPERTIES *UIWNDPROPERTIES::GetDlgItem(LPCTSTR lpszCtrlID) const
{
	CTRLPROPERTIES *pRet = NULL;

	int nCtrlCount = (int)m_vControls.size();
	for (int i = 0; i < nCtrlCount; ++i)
	{
		CTRLPROPERTIES *pProp = m_vControls[i];

		if (CString((LPCTSTR)pProp->m_strID) == lpszCtrlID)
		{
			pRet = pProp;
			break;
		}
	}

	return pRet;
}

CTRLPROPERTIES *UIWNDPROPERTIES::GetDlgItem(UINT nCtrlID)
{
	CTRLPROPERTIES *pRet = NULL;

	int nCtrlCount = (int)m_vControls.size();
	for (int i = 0; i < nCtrlCount; ++i)
	{
		CTRLPROPERTIES *pProp = m_vControls[i];

		if (CString((LPCTSTR)pProp->m_nID) == nCtrlID)
		{
			pRet = pProp;
			break;
		}
	}

	return pRet;
}

const CTRLPROPERTIES *UIWNDPROPERTIES::GetDlgItem(UINT nCtrlID) const
{
	CTRLPROPERTIES *pRet = NULL;

	int nCtrlCount = (int)m_vControls.size();
	for (int i = 0; i < nCtrlCount; ++i)
	{
		CTRLPROPERTIES *pProp = m_vControls[i];

		if (CString((LPCTSTR)pProp->m_nID) == nCtrlID)
		{
			pRet = pProp;
			break;
		}
	}

	return pRet;
}

RESFONTPROPERTIES::RESFONTPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(RESFONTPROPERTIES);
	m_eControlType = CT_RESOURCE_GENERAL_FONT;

	DisableBackgroundProperties(FALSE);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_rcControl.bSaveToSkinFile = m_rcControl.bShowInPropertiesWnd = false;
	m_bVisible.bSaveToSkinFile = m_bVisible.bShowInPropertiesWnd = false;
	m_bDisabled.bSaveToSkinFile = m_bDisabled.bShowInPropertiesWnd = false;
	m_bTabStop.bSaveToSkinFile = m_bTabStop.bShowInPropertiesWnd = false;
	m_bClipChildren.bSaveToSkinFile = m_bClipChildren.bShowInPropertiesWnd = false;
	m_bClipSiblings.bSaveToSkinFile = m_bClipSiblings.bShowInPropertiesWnd = false;
	m_eHorLayoutMode.bSaveToSkinFile = m_eHorLayoutMode.bShowInPropertiesWnd = false;
	m_eVerLayoutMode.bSaveToSkinFile = m_eVerLayoutMode.bShowInPropertiesWnd = false;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = false;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = false;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = false;
	m_rcLayoutMargin.bSaveToSkinFile = m_rcLayoutMargin.bShowInPropertiesWnd = false;
	m_rcRegion.bSaveToSkinFile = m_rcRegion.bShowInPropertiesWnd = false;

	m_logFont = (LOGFONT)CTRLPROPERTIES::m_lfN;
	m_logFont.vtEx = VT_EX_FONT;
	m_logFont.bNeedToConvert = false;
	m_logFont.strXUIElementName = _T("Font");
	m_logFont.strPropWndName = _T("Font(Size)");
}

RESFONTPROPERTIES::~RESFONTPROPERTIES()
{

}

bool RESFONTPROPERTIES::IsFontResource() const
{
	if (this == NULL)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

RESCOLORPROPERTIES::RESCOLORPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(RESCOLORPROPERTIES);
	m_eControlType = CT_RESOURCE_COLOR;

	DisableBackgroundProperties(FALSE);
	DisableFontProperties(FALSE);
	DisableColorProperties(FALSE);

	m_rcControl.bSaveToSkinFile = m_rcControl.bShowInPropertiesWnd = false;
	m_bVisible.bSaveToSkinFile = m_bVisible.bShowInPropertiesWnd = false;
	m_bDisabled.bSaveToSkinFile = m_bDisabled.bShowInPropertiesWnd = false;
	m_bTabStop.bSaveToSkinFile = m_bTabStop.bShowInPropertiesWnd = false;
	m_bClipChildren.bSaveToSkinFile = m_bClipChildren.bShowInPropertiesWnd = false;
	m_bClipSiblings.bSaveToSkinFile = m_bClipSiblings.bShowInPropertiesWnd = false;
	m_eHorLayoutMode.bSaveToSkinFile = m_eHorLayoutMode.bShowInPropertiesWnd = false;
	m_eVerLayoutMode.bSaveToSkinFile = m_eVerLayoutMode.bShowInPropertiesWnd = false;
	m_ptImageResize.bSaveToSkinFile = m_ptImageResize.bShowInPropertiesWnd = false;
	m_uXRepeatPixel.bSaveToSkinFile = m_uXRepeatPixel.bShowInPropertiesWnd = false;
	m_uYRepeatPixel.bSaveToSkinFile = m_uYRepeatPixel.bShowInPropertiesWnd = false;
	m_rcLayoutMargin.bSaveToSkinFile = m_rcLayoutMargin.bShowInPropertiesWnd = false;
	m_rcRegion.bSaveToSkinFile = m_rcRegion.bShowInPropertiesWnd = false;

	m_crColor.varDefaultValue = m_crColor = (LONG)RGB(0, 0, 0);
	m_crColor.vtEx = VT_EX_COLOR;
	m_crColor.strXUIElementName = _T("Color");
	m_crColor.strPropWndName = _T("Color");
}

RESCOLORPROPERTIES::~RESCOLORPROPERTIES()
{

}

bool RESCOLORPROPERTIES::IsColorResource() const
{
	if (this == NULL)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

HEADERPROPERTIES::HEADERPROPERTIES()
{
	m_lStructSize = (LONG)sizeof(HEADERPROPERTIES);
	m_eControlType = CT_HEADERCTRL;

	m_rcControl.bSaveToSkinFile = m_rcControl.bShowInPropertiesWnd = false;
	m_bVisible.bSaveToSkinFile = m_bVisible.bShowInPropertiesWnd = false;
	m_bDisabled.bSaveToSkinFile = m_bDisabled.bShowInPropertiesWnd = false;
	m_bTabStop.bSaveToSkinFile = m_bTabStop.bShowInPropertiesWnd = false;
	m_eHorLayoutMode.bSaveToSkinFile = m_eHorLayoutMode.bShowInPropertiesWnd = false;
	m_eVerLayoutMode.bSaveToSkinFile = m_eVerLayoutMode.bShowInPropertiesWnd = false;
	m_bSpecifyBackgroundImages.bSaveToSkinFile = m_bSpecifyBackgroundImages.bShowInPropertiesWnd = false;

	// Header background
	m_strHeaderBk = _T("");
	m_strHeaderBk.vtEx = VT_EX_BITMAPPATH;
	m_strHeaderBk.bGroup = true;
	m_strHeaderBk.strGroupName = _T("Header Background");
	m_strHeaderBk.strXUIElementName = _T("HeaderBk");
	m_strHeaderBk.strXUIAttributeName = _T("RelativePath");
	m_strHeaderBk.strDescription = _T("If Specify header button image, this group is ignore");
	m_strHeaderBk.strPropWndName = _T("Header Background");

	m_ptHeaderBkImageResize = CPoint(2, 2);
	m_ptHeaderBkImageResize.vtEx = VT_EX_POINT;
	m_ptHeaderBkImageResize.strXUIElementName = _T("HeaderBkResizingPoint");
	m_ptHeaderBkImageResize.strPropWndName = _T("Header Background Resize Point");
	m_ptHeaderBkImageResize.nMinValue = 0;
	m_ptHeaderBkImageResize.nMaxValue = 3840;

	// Combine Header Button Images
	m_bCombineBackgroundImages.varDefaultValue = m_bCombineBackgroundImages = false;
	m_bCombineBackgroundImages.bGroup = true;
	m_bCombineBackgroundImages.strGroupName = _T("Header Button Images");
	m_bCombineBackgroundImages.strXUIElementName = _T("IsCombineHeaderButtonImages");
	m_bCombineBackgroundImages.strPropWndName = _T("Combine Header Button Images");

	// Combine Header Button Path
	m_strBKCombine = _T("");
	m_strBKCombine.vtEx = VT_EX_BITMAPPATH;
	m_strBKCombine.strXUIElementName = _T("HeaderButtonCombine");
	m_strBKCombine.strXUIAttributeName = _T("RelativePath");
	m_strBKCombine.strPropWndName = _T("Combine Header Button Image Path");

	// Normal Header Button
	m_strBKN = _T("");
	m_strBKN.vtEx = VT_EX_BITMAPPATH;
	m_strBKN.strXUIElementName = _T("HeaderButtonBkN");
	m_strBKN.strXUIAttributeName = _T("RelativePath");
	m_strBKN.strPropWndName = _T("Normal Header Button Image");

	// Highlight header button
	m_strBKH = _T("");
	m_strBKH.vtEx = VT_EX_BITMAPPATH;
	m_strBKH.strXUIElementName = _T("HeaderButtonBkH");
	m_strBKH.strXUIAttributeName = _T("RelativePath");
	m_strBKH.strPropWndName = _T("Highlight Header Button Image");

	// Selected header button
	m_strBKS = _T("");
	m_strBKS.vtEx = VT_EX_BITMAPPATH;
	m_strBKS.strXUIElementName = _T("HeaderButtonBkS");
	m_strBKS.strXUIAttributeName = _T("RelativePath");
	m_strBKS.strPropWndName = _T("Selected Header Button Image");

	// Disabled header button
	m_strBKD = _T("");
	m_strBKD.vtEx = VT_EX_BITMAPPATH;
	m_strBKD.strXUIElementName = _T("HeaderButtonBkD");
	m_strBKD.strXUIAttributeName = _T("RelativePath");
	m_strBKD.strPropWndName = _T("Disabled Header Button Image");

	// Header Button Resize
	m_ptImageResize = CPoint(2, 2);
	m_ptImageResize.vtEx = VT_EX_POINT;
	m_ptImageResize.strXUIElementName = _T("HeaderButtonResizePoint");
	m_ptImageResize.strPropWndName = _T("Header Button Resize Point");
	m_ptImageResize.nMinValue = 0;
	m_ptImageResize.nMaxValue = 3840;

	// Header font
	m_strResFontIDN = _T("");
	m_strResFontIDN.vtEx = VT_EX_RESOURCE_FONT;
	m_strResFontIDN.bGroup = true;
	m_strResFontIDN.strGroupName = _T("Header Font(Size)");
	m_strResFontIDN.strXUIElementName = _T("HeaderFontResN");
	m_strResFontIDN.strPropWndName = _T("Normal");

	m_bHSameNFont.varDefaultValue = m_bHSameNFont = true;
	m_bHSameNFont.strXUIElementName = _T("IsHeaderHSameNFont");
	m_bHSameNFont.strPropWndName = _T("Highlight Font Same as Normal");

	m_strResFontIDH = _T("");
	m_strResFontIDH.vtEx = VT_EX_RESOURCE_FONT;
	m_strResFontIDH.strXUIElementName = _T("HeaderFontResH");
	m_strResFontIDH.strPropWndName = _T("Highlight");

	m_bSSameNFont.varDefaultValue = m_bSSameNFont = true;
	m_bSSameNFont.strXUIElementName = _T("IsHeaderSSameNFont");
	m_bSSameNFont.strPropWndName = _T("Selected Font Same as Normal");

	m_strResFontIDS = _T("");
	m_strResFontIDS.vtEx = VT_EX_RESOURCE_FONT;
	m_strResFontIDS.strXUIElementName = _T("HeaderFontResS");
	m_strResFontIDS.strPropWndName = _T("Selected");

	m_bDSameNFont.varDefaultValue = m_bDSameNFont = true;
	m_bDSameNFont.strXUIElementName = _T("IsHeaderDSameNFont");
	m_bDSameNFont.strPropWndName = _T("Disabled Font Same as Normal");

	m_strResFontIDD = _T("");
	m_strResFontIDD.vtEx = VT_EX_RESOURCE_FONT;
	m_strResFontIDD.strXUIElementName = _T("HeaderFontResD");
	m_strResFontIDD.strPropWndName = _T("Disabled");

	// Header Text Color
	m_strResColorIDN = _T("");
	m_strResColorIDN.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDN.bGroup = true;
	m_strResColorIDN.strGroupName = _T("Header Text Color");
	m_strResColorIDN.strXUIElementName = _T("HeaderColorResN");
	m_strResColorIDN.strPropWndName = _T("Normal");

	m_bHSameNColor.varDefaultValue = m_bHSameNColor = true;
	m_bHSameNColor.strXUIElementName = _T("IsHeaderHSameNColor");
	m_bHSameNColor.strPropWndName = _T("Highlight Color Same as Normal");

	m_strResColorIDH = _T("");
	m_strResColorIDH.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDH.strXUIElementName = _T("HeaderColorResH");
	m_strResColorIDH.strPropWndName = _T("Highlight");

	m_bSSameNColor.varDefaultValue = m_bSSameNColor = true;
	m_bSSameNColor.strXUIElementName = _T("IsHeaderSSameNColor");
	m_bSSameNColor.strPropWndName = _T("Selected Color Same as Normal");

	m_strResColorIDS = _T("");
	m_strResColorIDS.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDS.strXUIElementName = _T("HeaderColorResS");
	m_strResColorIDS.strPropWndName = _T("Selected");

	m_bDSameNColor.varDefaultValue = m_bDSameNColor = true;
	m_bDSameNColor.strXUIElementName = _T("IsHeaderDSameNColor");
	m_bDSameNColor.strPropWndName = _T("Disabled Color Same as Normal");

	m_strResColorIDD = _T("");
	m_strResColorIDD.vtEx = VT_EX_RESOURCE_COLOR;
	m_strResColorIDD.strXUIElementName = _T("HeaderColorResD");
	m_strResColorIDD.strPropWndName = _T("Disabled");

	m_bHeaderTrack.varDefaultValue = m_bHeaderTrack = true;
	m_bHeaderTrack.bGroup = true;
	m_bHeaderTrack.strGroupName = _T("Header");
	m_bHeaderTrack.strXUIElementName = _T("IsHeaderTrack");
	m_bHeaderTrack.strPropWndName = _T("Header Track");

	m_bHeaderDragDrop.varDefaultValue = m_bHeaderDragDrop = false;
	m_bHeaderDragDrop.strXUIElementName = _T("IsHeaderDragDrop");
	m_bHeaderDragDrop.strPropWndName = _T("Header Drag & Drop");
}

HEADERPROPERTIES::~HEADERPROPERTIES()
{

}

//////////////////////////////////////////////////////////////////////////

FONTRESOURCEITEM::FONTRESOURCEITEM()
{
	m_bConditionFont = false;
	m_pFontResProp = NULL;
}

FONTRESOURCEITEM::~FONTRESOURCEITEM()
{
}

int FONTRESOURCEITEM::Release()
{
	if (m_bConditionFont)
	{
		std::map<CString, RESFONTPROPERTIES *>::iterator it = m_mapConditionFont.begin();
		for (; it != m_mapConditionFont.end(); ++it)
		{
			delete it->second;
		}

		m_mapConditionFont.clear();
	}
	else
	{
		delete m_pFontResProp;
	}

	return 0;
}

int FONTRESOURCEITEM::ConvertGeneralFontToConditionFont(LPCTSTR lpszConditionName)
{
	if (m_bConditionFont)
	{
		ASSERT(FALSE);
		return -1;
	}

	m_bConditionFont = true;
	m_strFontID = (LPCTSTR)m_pFontResProp->m_strID;

	m_pFontResProp->m_strID.bSaveToSkinFile = true;
	m_pFontResProp->m_strID.bShowInPropertiesWnd = true;
	m_pFontResProp->m_strID.strPropWndName = _T("Condition Name");
	m_pFontResProp->m_strID.strDescription = _T("Input the condition name for the font to use.");
	m_pFontResProp->m_strID = lpszConditionName;
	m_mapConditionFont[lpszConditionName] = m_pFontResProp;

	m_pFontResProp = NULL;

	return 0;
}

RESFONTPROPERTIES *FONTRESOURCEITEM::GetConditionFont(const CString &strCondition)
{
	ASSERT(m_bConditionFont);

	if (strCondition.IsEmpty())
	{
		if (m_mapConditionFont.empty())
		{
			return NULL;
		}

		return m_mapConditionFont.begin()->second;
	}
	else
	{
		return m_mapConditionFont[strCondition];
	}
}

int FONTRESOURCEITEM::DeleteConditionFontChild(const RESFONTPROPERTIES *presFontDelete)
{
	if (!m_bConditionFont)
	{
		return -1;
	}

	BOOL bDelete = FALSE;
	std::map<CString, RESFONTPROPERTIES *>::iterator it = m_mapConditionFont.begin();
	for (; it != m_mapConditionFont.end(); ++it)
	{
		if (it->second == presFontDelete)
		{
			delete it->second;

			m_mapConditionFont.erase(it);
			bDelete = TRUE;
			break;
		}
	}

	return bDelete ? 0 : -2;
}