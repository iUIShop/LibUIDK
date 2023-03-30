#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "UIShop.h"
#include "IUIMFCPropertyGridCtrl.h"
#include "IUIPropertyGridFontProperty.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define IDC_PROLIST					2
extern CMainFrame *g_pFrm;


int InitWindowIDOptions(CMFCPropertyGridProperty *pProp, BOOL bIncludeSelf)
{
	CUIShopView *pView = g_pFrm->GetUIShopView();
	if (pView == NULL)
	{
		return 1;
	}

	CUIDesignWnd *pWndDesign = pView->GetCurUIDesignWnd();
	if (pWndDesign == NULL)
	{
		return 2;
	}

	UIWNDPROPERTIES *pWndProp = pWndDesign->GetUIWndProperties();
	if (pWndProp == NULL)
	{
		return 3;
	}

	CString strCurWinID = (LPCTSTR)pWndProp->m_strID;

	std::vector<UIWNDPROPERTIES *> *pWindows = CUIMgr::GetUIWndPropertiesList();
	size_t nCount = pWindows->size();
	for (int i = 0; i < nCount; ++i)
	{
		UIWNDPROPERTIES *pWindow = (*pWindows)[i];

		// Don't add current window ID.
		if (!bIncludeSelf)
		{
			if (strCurWinID == (LPCTSTR)pWindow->m_strID)
			{
				continue;
			}
		}

		pProp->AddOption(pWindow->m_strID);
	}

	return 0;
}

int InitTaskWndMgrIDOptions(CMFCPropertyGridProperty *pProp)
{
	CUIShopView *pView = g_pFrm->GetUIShopView();
	if (pView == NULL)
	{
		return 1;
	}

	CUIDesignWnd *pWndDesign = pView->GetCurUIDesignWnd();
	if (pWndDesign == NULL)
	{
		return 2;
	}

	UIWNDPROPERTIES *pUIWndProp = pWndDesign->GetUIWndProperties();
	if (pUIWndProp == NULL)
	{
		return 3;
	}

	int nCount = (int)pUIWndProp->m_vControls.size();
	for (int i = 0; i < nCount; ++i)
	{
		CTRLPROPERTIES *pCtrlProp = pUIWndProp->m_vControls[i];

		if ((CONTROL_TYPE)pCtrlProp->m_eControlType == CT_TASK_WND_MGR)
		{
			pProp->AddOption(pCtrlProp->m_strID);
		}
	}

	return 0;
}

int InitResourceOptions(CMFCPropertyGridProperty *pProp, CONTROL_TYPE eResourceType)
{
	if (eResourceType == CT_RESOURCE_GENERAL_FONT)
	{
		int nCount = CUIMgr::GetResourceCount(REST_FONT);
		for (int i = 0; i < nCount; ++i)
		{
			FONTRESOURCEITEM *pResFont = CUIMgr::GetFontResourceItem(i);

			if (pResFont->m_bConditionFont)
			{
				pProp->AddOption(pResFont->m_strFontID);
			}
			else
			{
				pProp->AddOption(pResFont->m_pFontResProp->m_strID);
			}
		}
	}
	else if (eResourceType == CT_RESOURCE_COLOR)
	{
		int nCount = CUIMgr::GetResourceCount(REST_COLOR);
		for (int i = 0; i < nCount; ++i)
		{
			RESCOLORPROPERTIES *pResProp = CUIMgr::GetColorResourceItem(i);

			pProp->AddOption(pResProp->m_strID);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return 0;
}

#define BEGIN_INIT_STYLE_OPTIONS(STYLE_TYPE_MACRO, STYLE_MACRO) \
	if (eStyleType == STYLE_MACRO) \
	{ \
		int nCount = CUIMgr::GetStyleCount(STYLE_TYPE_MACRO); \
		for (int i=0; i<nCount; ++i) \
		{ \
			CTRLPROPERTIES *pStyleProp = CUIMgr::GetStyleItem(STYLE_TYPE_MACRO, i); \
			pProp->AddOption(pStyleProp->m_strID); \
		} \
	}
#define INIT_STYLE_OPTIONS(STYLE_TYPE_MACRO, STYLE_MACRO) \
	else if (eStyleType == STYLE_MACRO) \
	{ \
		int nCount = CUIMgr::GetStyleCount(STYLE_TYPE_MACRO); \
		for (int i=0; i<nCount; ++i) \
		{ \
			CTRLPROPERTIES *pStyleProp = CUIMgr::GetStyleItem(STYLE_TYPE_MACRO, i); \
			pProp->AddOption(pStyleProp->m_strID); \
		} \
	}
#define END_INIT_STYLE_OPTIONS() \
	else \
	{ \
		ASSERT(FALSE); \
	}

int InitStyleOptions(CMFCPropertyGridProperty *pProp, CONTROL_TYPE eStyleType)
{
	BEGIN_INIT_STYLE_OPTIONS(STYLET_PUSHBUTTON, CT_STYLE_PUSHBUTTON)
	INIT_STYLE_OPTIONS(STYLET_CHECK, CT_STYLE_CHECK)
	INIT_STYLE_OPTIONS(STYLET_RADIO, CT_STYLE_RADIO)
	INIT_STYLE_OPTIONS(STYLET_COMBOBOX, CT_STYLE_COMBOBOX)
	INIT_STYLE_OPTIONS(STYLET_STATIC, CT_STYLE_STATIC)
	INIT_STYLE_OPTIONS(STYLET_EDIT, CT_STYLE_EDIT)
	INIT_STYLE_OPTIONS(STYLET_SLIDER, CT_STYLE_SLIDER)
	INIT_STYLE_OPTIONS(STYLET_PROGRESS, CT_STYLE_PROGRESS)
	INIT_STYLE_OPTIONS(STYLET_SPIN, CT_STYLE_SPIN)
	INIT_STYLE_OPTIONS(STYLET_HOTKEY, CT_STYLE_HOTKEY)
	INIT_STYLE_OPTIONS(STYLET_IPADDRESS, CT_STYLE_IPADDRESS)
	INIT_STYLE_OPTIONS(STYLET_SLIDEREX, CT_STYLE_SLIDEREX)
	INIT_STYLE_OPTIONS(STYLET_LISTCTRL, CT_STYLE_LISTCTRL)
	INIT_STYLE_OPTIONS(STYLET_TREECTRL, CT_STYLE_TREECTRL)
	INIT_STYLE_OPTIONS(STYLET_TREELIST, CT_STYLE_TREELIST)
	INIT_STYLE_OPTIONS(STYLET_RICHEDIT, CT_STYLE_RICHEDIT)
	INIT_STYLE_OPTIONS(STYLET_IMRICHEDIT, CT_STYLE_IMRICHEDIT)
	INIT_STYLE_OPTIONS(STYLET_HTMLCTRL, CT_STYLE_HTMLCTRL)
	INIT_STYLE_OPTIONS(STYLET_DATETIMECTRL, CT_STYLE_DATETIMECTRL)
	INIT_STYLE_OPTIONS(STYLET_WAVECTRL, CT_STYLE_WAVECTRL)
	INIT_STYLE_OPTIONS(STYLET_MENU, CT_STYLE_MENU)
	INIT_STYLE_OPTIONS(STYLET_MENUBAR, CT_STYLE_MENUBAR)
	INIT_STYLE_OPTIONS(STYLET_HEADERCTRL, CT_STYLE_HEADERCTRL)
	INIT_STYLE_OPTIONS(STYLET_HORSCROLLBAR, CT_STYLE_HORSCROLLBAR)
	INIT_STYLE_OPTIONS(STYLET_VERSCROLLBAR, CT_STYLE_VERSCROLLBAR)
	INIT_STYLE_OPTIONS(STYLET_SPLITTER, CT_STYLE_SPLITTER)
	END_INIT_STYLE_OPTIONS()

	return 0;
}

int InitPropListOptions(CMFCPropertyGridProperty *pProp, const IUI_VARIANT &varValue)
{
	if (varValue.vtEx == VT_EX_WINDOW_ID)
	{
		InitWindowIDOptions(pProp, TRUE);
	}
	else if (varValue.vtEx == VT_EX_WINDOW_ID_NOT_INCLUDE_SELF)
	{
		InitWindowIDOptions(pProp, FALSE);
	}
	else if (varValue.vtEx == VT_EX_TASKMGR_ID)
	{
		InitTaskWndMgrIDOptions(pProp);
	}
	else if (varValue.vtEx == VT_EX_RESOURCE_FONT)
	{
		InitResourceOptions(pProp, CT_RESOURCE_GENERAL_FONT);
	}
	else if (varValue.vtEx == VT_EX_RESOURCE_COLOR)
	{
		InitResourceOptions(pProp, CT_RESOURCE_COLOR);
	}
	else if (varValue.vtEx == VT_EX_STYLE_PUSHBUTTON)
	{
		InitStyleOptions(pProp, CT_STYLE_PUSHBUTTON);
	}
	else if (varValue.vtEx == VT_EX_STYLE_CHECK)
	{
		InitStyleOptions(pProp, CT_STYLE_CHECK);
	}
	else if (varValue.vtEx == VT_EX_STYLE_RADIO)
	{
		InitStyleOptions(pProp, CT_STYLE_RADIO);
	}
	else if (varValue.vtEx == VT_EX_STYLE_COMBOBOX)
	{
		InitStyleOptions(pProp, CT_STYLE_COMBOBOX);
	}
	else if (varValue.vtEx == VT_EX_STYLE_STATIC)
	{
		InitStyleOptions(pProp, CT_STYLE_STATIC);
	}
	else if (varValue.vtEx == VT_EX_STYLE_EDIT)
	{
		InitStyleOptions(pProp, CT_STYLE_EDIT);
	}
	else if (varValue.vtEx == VT_EX_STYLE_SLIDER)
	{
		InitStyleOptions(pProp, CT_STYLE_SLIDER);
	}
	else if (varValue.vtEx == VT_EX_STYLE_PROGRESS)
	{
		InitStyleOptions(pProp, CT_STYLE_PROGRESS);
	}
	else if (varValue.vtEx == VT_EX_STYLE_SPIN)
	{
		InitStyleOptions(pProp, CT_STYLE_SPIN);
	}
	else if (varValue.vtEx == VT_EX_STYLE_HOTKEY)
	{
		InitStyleOptions(pProp, CT_STYLE_HOTKEY);
	}
	else if (varValue.vtEx == VT_EX_STYLE_IPADDRESS)
	{
		InitStyleOptions(pProp, CT_STYLE_IPADDRESS);
	}
	else if (varValue.vtEx == VT_EX_STYLE_SLIDEREX)
	{
		InitStyleOptions(pProp, CT_STYLE_SLIDEREX);
	}
	else if (varValue.vtEx == VT_EX_STYLE_LISTCTRL)
	{
		InitStyleOptions(pProp, CT_STYLE_LISTCTRL);
	}
	else if (varValue.vtEx == VT_EX_STYLE_TREECTRL)
	{
		InitStyleOptions(pProp, CT_STYLE_TREECTRL);
	}
	else if (varValue.vtEx == VT_EX_STYLE_TREELIST)
	{
		InitStyleOptions(pProp, CT_STYLE_TREELIST);
	}
	else if (varValue.vtEx == VT_EX_STYLE_RICHEDIT)
	{
		InitStyleOptions(pProp, CT_STYLE_RICHEDIT);
	}
	else if (varValue.vtEx == VT_EX_STYLE_IMRICHEDIT)
	{
		InitStyleOptions(pProp, CT_STYLE_IMRICHEDIT);
	}
	else if (varValue.vtEx == VT_EX_STYLE_HTMLCTRL)
	{
		InitStyleOptions(pProp, CT_STYLE_HTMLCTRL);
	}
	else if (varValue.vtEx == VT_EX_STYLE_DATETIMECTRL)
	{
		InitStyleOptions(pProp, CT_STYLE_DATETIMECTRL);
	}
	else if (varValue.vtEx == VT_EX_STYLE_WAVECTRL)
	{
		InitStyleOptions(pProp, CT_STYLE_WAVECTRL);
	}
	else if (varValue.vtEx == VT_EX_STYLE_MENU)
	{
		InitStyleOptions(pProp, CT_STYLE_MENU);
	}
	else if (varValue.vtEx == VT_EX_STYLE_MENUBAR)
	{
		InitStyleOptions(pProp, CT_STYLE_MENUBAR);
	}
	else if (varValue.vtEx == VT_EX_STYLE_HEADERCTRL)
	{
		InitStyleOptions(pProp, CT_STYLE_HEADERCTRL);
	}
	else if (varValue.vtEx == VT_EX_STYLE_HORSCROLLBAR)
	{
		InitStyleOptions(pProp, CT_STYLE_HORSCROLLBAR);
	}
	else if (varValue.vtEx == VT_EX_STYLE_VERSCROLLBAR)
	{
		InitStyleOptions(pProp, CT_STYLE_VERSCROLLBAR);
	}
	else
	{
		INT_PTR nCount = varValue.lstStringOptions.GetCount();

		for (POSITION pos = varValue.lstStringOptions.GetHeadPosition(); pos != NULL;)
		{
			pProp->AddOption(varValue.lstStringOptions.GetNext(pos));
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	m_pCtrlProp = NULL;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, CPropertiesWnd::OnPropertyChanged)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

int CPropertiesWnd::AddSubItem(CMFCPropertyGridProperty *pParent, const IUI_VARIANT *pVariant)
{
	if (pVariant->vt == VT_NULL)
	{
		return -1;
	}

	CMFCPropertyGridProperty *pItem = NULL;

	if (pVariant->vtEx == VT_EMPTY || pVariant->vtEx == VT_EX_CONTROL_ID_MACRO)
	{
		pItem = new CMFCPropertyGridProperty(pVariant->strPropWndName, *pVariant, pVariant->strDescription);
		if (pVariant->vt == VT_INT || pVariant->vt == VT_I4 || pVariant->vt == VT_UI4)
		{
			pItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		}
	}
	else if (pVariant->vtEx == VT_EX_STRUCT_SIZE || pVariant->vtEx == VT_EX_CONTROL_ID)
	{
		return -1;
	}
	else if (pVariant->vtEx == VT_EX_CONTROL_TYPE)
	{
		CString strType = GetControlTypeString(CONTROL_TYPE(*pVariant));
#ifdef _DEBUG
		strType += _T("---");
		strType += HexStringFromInt((int)m_pCtrlProp, TRUE, TRUE, 8);
#endif // _DEBUG
		pItem = new CMFCPropertyGridProperty(pVariant->strPropWndName, strType.GetBuffer(0), pVariant->strDescription);
	}
	else if (pVariant->vtEx == VT_EX_RECT)
	{
		pItem = new CMFCPropertyGridProperty(pVariant->strPropWndName);
	}
	else if (pVariant->vtEx == VT_EX_RECT_WH)
	{
		pItem = new CMFCPropertyGridProperty(pVariant->strPropWndName);
	}
	else if (pVariant->vtEx == VT_EX_POINT)
	{
		pItem = new CMFCPropertyGridProperty(pVariant->strPropWndName);
	}
	else if (pVariant->vtEx == VT_EX_COLOR)
	{
		CMFCPropertyGridColorProperty *pColorItem = new CMFCPropertyGridColorProperty(pVariant->strPropWndName, *pVariant, NULL, pVariant->strDescription);
		pColorItem->EnableOtherButton(_T("Other..."));
		pItem = pColorItem;
	}
	else if (pVariant->vtEx == VT_EX_IMAGE_RESIZE_MODE
		|| pVariant->vtEx == VT_EX_STYLE_PUSHBUTTON
		|| pVariant->vtEx == VT_EX_STYLE_EDIT
		|| pVariant->vtEx == VT_EX_LAYOUT_HOR
		|| pVariant->vtEx == VT_EX_LAYOUT_VER
		|| pVariant->vtEx == VT_EX_TEXT_ALIGN_HOR
		|| pVariant->vtEx == VT_EX_TEXT_ALIGN_VER
		|| pVariant->vtEx == VT_EX_FOREGROUND_ALIGN_HOR
		|| pVariant->vtEx == VT_EX_FOREGROUND_ALIGN_VER
		|| pVariant->vtEx == VT_EX_BUTTON_TYPE
		|| pVariant->vtEx == VT_EX_CHECKBOX_TYPE
		|| pVariant->vtEx == VT_EX_WINDOW_TYPE
		|| pVariant->vtEx == VT_EX_COMBOBOX_STYLE
		|| pVariant->vtEx == VT_EX_SPN_ALIGN
		|| pVariant->vtEx == VT_EX_SLD_EXT_STYLE
		|| pVariant->vtEx == VT_EX_LST_STYLE
		|| pVariant->vtEx == VT_EX_LST_ALIGN
		|| pVariant->vtEx == VT_EX_LST_SORT
		|| pVariant->vtEx == VT_EX_DATETIME_FORMAT
		|| pVariant->vtEx == VT_EX_BACKGROUND_TYPE
		|| pVariant->vtEx == VT_EX_PROGRESS_THUMB_MODE
		|| pVariant->vtEx == VT_EX_PROGRESS_THUMB_POSITION
		|| pVariant->vtEx == VT_EX_ALPHA_FORMAT
		|| pVariant->vtEx == VT_EX_MENU_BORDER_STYLE
		|| pVariant->vtEx == VT_EX_SPLITTER_ORIENTATION
		|| pVariant->vtEx == VT_EX_STACK_PANEL_ORIENTATION
		|| pVariant->vtEx == VT_EX_PEN_STYLE
		|| pVariant->vtEx == VT_EX_WINDOW_ID
		|| pVariant->vtEx == VT_EX_WINDOW_ID_NOT_INCLUDE_SELF
		|| pVariant->vtEx == VT_EX_TASKMGR_ID
		|| pVariant->vtEx == VT_EX_RESOURCE_FONT
		|| pVariant->vtEx == VT_EX_RESOURCE_COLOR
		|| pVariant->vtEx == VT_EX_STYLE_MENU
		|| pVariant->vtEx == VT_EX_STYLE_HEADERCTRL
		|| pVariant->vtEx == VT_EX_STYLE_SPIN
		|| pVariant->vtEx == VT_EX_STYLE_HORSCROLLBAR
		|| pVariant->vtEx == VT_EX_STYLE_VERSCROLLBAR)
	{
		pItem = new CMFCPropertyGridProperty(pVariant->strPropWndName,
			COleVariant(pVariant->GetEnumString()), pVariant->strDescription);
		InitPropListOptions(pItem, *pVariant);
	}
	else if (pVariant->vtEx == VT_EX_FONT)
	{
		pItem = new CIUIPropertyGridFontProperty(pVariant->strPropWndName,
			(LOGFONT)*pVariant, CF_EFFECTS | CF_SCREENFONTS, pVariant->strDescription);
	}
	else if (pVariant->vtEx == VT_EX_BITMAPPATH)
	{
		static TCHAR BASED_CODE szFilter[] = _T("Image Files(*.bmp; *.png; *.jpg; *.jpeg; *.gif)|*.bmp;*.png;*.jpg;*.jpeg;*.gif|All Files(*.*)|*.*||");
		pItem = new CIUIMFCPropertyGridFileProperty(pVariant->strPropWndName, TRUE,
			*pVariant, _T("bmp"), 0, szFilter, pVariant->strDescription);
	}
	else
	{
		ASSERT(FALSE);
	}

	pItem->SetData((DWORD_PTR)pVariant);

	if (pVariant->vt != VT_BOOL && pVariant->vtEx != VT_EX_POINT && pVariant->vtEx != VT_EX_RECT && pVariant->vtEx != VT_EX_RECT_WH)
	{
		pItem->AllowEdit(!pVariant->bReadOnly);
	}

	pParent->AddSubItem(pItem);

	if (pVariant->vtEx == VT_EX_POINT)
	{
		POINT pt = *pVariant;

		CMFCPropertyGridProperty *pSubItem = new CMFCPropertyGridProperty(_T("X Pos"), COleVariant(pt.x), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);

		pSubItem = new CMFCPropertyGridProperty(_T("Y Pos"), COleVariant(pt.y), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);
	}
	else if (pVariant->vtEx == VT_EX_RECT)
	{
		CRect rect = *pVariant;
		CMFCPropertyGridProperty *pSubItem = new CMFCPropertyGridProperty(_T("Left"), COleVariant(rect.left), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);

		pSubItem = new CMFCPropertyGridProperty(_T("Top"), COleVariant(rect.top), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);

		pSubItem = new CMFCPropertyGridProperty(_T("Right"), COleVariant(rect.right), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);

		pSubItem = new CMFCPropertyGridProperty(_T("Bottom"), COleVariant(rect.bottom), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);
	}
	else if (pVariant->vtEx == VT_EX_RECT_WH)
	{
		CRect rect = *pVariant;
		CMFCPropertyGridProperty *pSubItem = new CMFCPropertyGridProperty(_T("Left"), COleVariant(rect.left), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);

		pSubItem = new CMFCPropertyGridProperty(_T("Top"), COleVariant(rect.top), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);

		pSubItem = new CMFCPropertyGridProperty(_T("Width"), COleVariant((LONG)rect.Width()), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);

		pSubItem = new CMFCPropertyGridProperty(_T("Height"), COleVariant((LONG)rect.Height()), pVariant->strDescription);
		pSubItem->EnableSpinControl(TRUE, pVariant->nMinValue, pVariant->nMaxValue);
		pSubItem->SetData((DWORD_PTR)pVariant);
		UpdateDisabledStatus(pVariant, pSubItem);
		pItem->AddSubItem(pSubItem);
	}

	UpdateDisabledStatus(pVariant, pItem);

	return 0;
}

int CPropertiesWnd::UpdatePropertyAndUI(IUI_VARIANT *pVariant, const IUI_VARIANT *pVarSetting)
{
	if (pVariant == NULL || pVarSetting == NULL)
	{
		return -1;
	}

	// Update value.
	*pVariant = *pVarSetting;

	// Update UI.
	CTRLPROPERTIES *pCtrlProp = NULL;
	int nRet = CUIMgr::FindCtrlProp(pVariant, &pCtrlProp);
	if (nRet == 0 || pCtrlProp == NULL)
	{
		ASSERT(FALSE);
		return -2;
	}

	CFileView *pFileView = g_pFrm->GetFileView();
	CViewTree *pViewTree = pFileView->GetWindowTree();

	if (pVariant->vtEx == VT_EX_CONTROL_ID_MACRO)
	{
		HTREEITEM hItem = pFileView->FindItem(pCtrlProp, NULL);
		if (hItem != NULL)
		{
			pViewTree->SetItemText(hItem, (LPCTSTR)*pVariant);
		}
	}
	else if (pVariant->vtEx == VT_EX_BITMAPPATH)
	{
		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		if (eControlType == CT_UIWND)
		{
			HTREEITEM hItem = pFileView->FindItem(pCtrlProp->m_strID, NULL);
			TREE_ITEMDATA *pData = (TREE_ITEMDATA *)pViewTree->GetItemData(hItem);
			ASSERT(pData != NULL);
			pData->wndUIDesign.SetBkBitmap();
		}
	}

	// Some properties bind to this property.
	if (pVariant->lBeBindCount > 0)
	{
		UpdateAllPropertiesDisabledStatus();
	}

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CUIShopView *pView = pFrm->GetUIShopView();
	pView->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	return 0;
}

// After pProp changed, How the UI update.
// If the pProp is an ID, the pProp's Orig value should be update also.
int CPropertiesWnd::UpdatePropertyAndUI(CMFCPropertyGridProperty *pProp)
{
	IUI_VARIANT *pVariant = (IUI_VARIANT *)pProp->GetData();
	ASSERT(pVariant != NULL);

	if (pVariant->vtEx == VT_EX_CONTROL_ID_MACRO)
	{
		IUI_VARIANT variantNewID = (VARIANT)pProp->GetValue();

		BOOL bParentWindow = FALSE;
		UIWNDPROPERTIES *pUIWndProp = CUIMgr::GetParent(pVariant, &bParentWindow);
		if (pUIWndProp == NULL)	// Maybe Resource or Style
		{
			*pVariant = (VARIANT)pProp->GetValue();

			CFileView *pFileView = g_pFrm->GetFileView();
			CViewTree *pViewTree = pFileView->GetWindowTree();
			HTREEITEM hItem = pFileView->FindItem(m_pCtrlProp, NULL);
			if (hItem != NULL)
			{
				pViewTree->SetItemText(hItem, (LPCTSTR)*pVariant);
			}
		}
		else
		{
			// Check the ID exist or not.
			// If the modified ID belong to a parent window
			if (bParentWindow)
			{
				int nIndex = CUIMgr::GetWindowIndex((LPCTSTR)variantNewID);
				// The new ID already exist
				if (nIndex >= 0)
				{
					AfxMessageBox(_T("A control with this ID already exists."));
					pProp->SetValue(pProp->GetOriginalValue());

					return -1;
				}

				// If property is Window, Resource or StyleID, update the Window Resource or Style ID in window list.
				*pVariant = (VARIANT)pProp->GetValue();
				pProp->SetOriginalValue(*pVariant);

				CFileView *pFileView = g_pFrm->GetFileView();
				CViewTree *pViewTree = pFileView->GetWindowTree();
				HTREEITEM hItem = pFileView->FindItem(m_pCtrlProp, NULL);
				if (hItem != NULL)
				{
					pViewTree->SetItemText(hItem, (LPCTSTR)*pVariant);
				}
			}
			// If the modified ID belong to a control
			else
			{
				CTRLPROPERTIES *pCtrlProp = CUIMgr::GetCONTROL(pUIWndProp->m_strID, (LPCTSTR)variantNewID);
				// The new ID already exist
				if (pCtrlProp != NULL)
				{
					AfxMessageBox(_T("A control with this ID already exists."));
					pProp->SetValue(pProp->GetOriginalValue());

					return -1;
				}

				*pVariant = (VARIANT)pProp->GetValue();
				pProp->SetOriginalValue(*pVariant);
			}
		}
	}
	else if (pVariant->vtEx == VT_EMPTY || pVariant->vtEx == VT_EX_CONTROL_TYPE)
	{
		*pVariant = (VARIANT)pProp->GetValue();
	}
	else if (pVariant->vtEx == VT_EX_BITMAPPATH)
	{
		*pVariant = (VARIANT)pProp->GetValue();
		ASSERT(m_pCtrlProp != NULL);
		CONTROL_TYPE eControlType = (CONTROL_TYPE)m_pCtrlProp->m_eControlType;
		if (eControlType == CT_UIWND)
		{
			CFileView *pFileView = g_pFrm->GetFileView();
			CViewTree *pViewTree = pFileView->GetWindowTree();
			HTREEITEM hItem = pFileView->FindItem(m_pCtrlProp->m_strID, NULL);
			TREE_ITEMDATA *pData = (TREE_ITEMDATA *)pViewTree->GetItemData(hItem);
			ASSERT(pData != NULL);
			pData->wndUIDesign.SetBkBitmap();
		}
	}
	else if (pVariant->vtEx == VT_EX_STRUCT_SIZE || pVariant->vtEx == VT_EX_CONTROL_ID)
	{
		ASSERT(FALSE);
		return -1;
	}
	else if (pVariant->vtEx == VT_EX_POINT)
	{
		if (CString(pProp->GetName()).CompareNoCase(_T("X Pos")) == 0)
		{
			pVariant->SetPointX((_variant_t)pProp->GetValue());
		}
		else if (CString(pProp->GetName()).CompareNoCase(_T("Y Pos")) == 0)
		{
			pVariant->SetPointY((_variant_t)pProp->GetValue());
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (pVariant->vtEx == VT_EX_RECT)
	{
		if (CString(pProp->GetName()).CompareNoCase(_T("Left")) == 0)
		{
			pVariant->SetRectLeft((_variant_t)pProp->GetValue());
		}
		else if (CString(pProp->GetName()).CompareNoCase(_T("Top")) == 0)
		{
			pVariant->SetRectTop((_variant_t)pProp->GetValue());
		}
		else if (CString(pProp->GetName()).CompareNoCase(_T("Right")) == 0)
		{
			pVariant->SetRectRight((_variant_t)pProp->GetValue());
		}
		else if (CString(pProp->GetName()).CompareNoCase(_T("Bottom")) == 0)
		{
			pVariant->SetRectBottom((_variant_t)pProp->GetValue());
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (pVariant->vtEx == VT_EX_RECT_WH)
	{
		if (CString(pProp->GetName()).CompareNoCase(_T("Left")) == 0)
		{
			pVariant->SetRectLeftNoSize((_variant_t)pProp->GetValue());
		}
		else if (CString(pProp->GetName()).CompareNoCase(_T("Top")) == 0)
		{
			pVariant->SetRectTopNoSize((_variant_t)pProp->GetValue());
		}
		else if (CString(pProp->GetName()).CompareNoCase(_T("Width")) == 0)
		{
			pVariant->SetRectWidth((_variant_t)pProp->GetValue());
		}
		else if (CString(pProp->GetName()).CompareNoCase(_T("Height")) == 0)
		{
			pVariant->SetRectHeight((_variant_t)pProp->GetValue());
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (pVariant->vtEx == VT_EX_COLOR)
	{
		CMFCPropertyGridColorProperty *pColorProp = (CMFCPropertyGridColorProperty *)pProp;
		*pVariant = (LONG)pColorProp->GetColor();
	}
	else if (pVariant->vtEx == VT_EX_IMAGE_RESIZE_MODE
		|| pVariant->vtEx == VT_EX_LAYOUT_HOR
		|| pVariant->vtEx == VT_EX_LAYOUT_VER
		|| pVariant->vtEx == VT_EX_TEXT_ALIGN_HOR
		|| pVariant->vtEx == VT_EX_TEXT_ALIGN_VER
		|| pVariant->vtEx == VT_EX_FOREGROUND_ALIGN_HOR
		|| pVariant->vtEx == VT_EX_FOREGROUND_ALIGN_VER
		|| pVariant->vtEx == VT_EX_BUTTON_TYPE
		|| pVariant->vtEx == VT_EX_CHECKBOX_TYPE
		|| pVariant->vtEx == VT_EX_WINDOW_TYPE
		|| pVariant->vtEx == VT_EX_COMBOBOX_STYLE
		|| pVariant->vtEx == VT_EX_SPN_ALIGN
		|| pVariant->vtEx == VT_EX_SLD_EXT_STYLE
		|| pVariant->vtEx == VT_EX_LST_STYLE
		|| pVariant->vtEx == VT_EX_LST_ALIGN
		|| pVariant->vtEx == VT_EX_LST_SORT
		|| pVariant->vtEx == VT_EX_DATETIME_FORMAT
		|| pVariant->vtEx == VT_EX_BACKGROUND_TYPE
		|| pVariant->vtEx == VT_EX_PROGRESS_THUMB_MODE
		|| pVariant->vtEx == VT_EX_PROGRESS_THUMB_POSITION
		|| pVariant->vtEx == VT_EX_ALPHA_FORMAT
		|| pVariant->vtEx == VT_EX_MENU_BORDER_STYLE
		|| pVariant->vtEx == VT_EX_SPLITTER_ORIENTATION
		|| pVariant->vtEx == VT_EX_STACK_PANEL_ORIENTATION
		|| pVariant->vtEx == VT_EX_PEN_STYLE)
	{
		pVariant->InitByEnumString((VARIANT)pProp->GetValue());
	}
	else if (pVariant->vtEx == VT_EX_WINDOW_ID
		|| pVariant->vtEx == VT_EX_WINDOW_ID_NOT_INCLUDE_SELF
		|| pVariant->vtEx == VT_EX_STYLE_EDIT
		|| pVariant->vtEx == VT_EX_TASKMGR_ID
		|| pVariant->vtEx == VT_EX_RESOURCE_FONT
		|| pVariant->vtEx == VT_EX_RESOURCE_COLOR
		|| pVariant->vtEx == VT_EX_STYLE_MENU
		|| pVariant->vtEx == VT_EX_STYLE_HEADERCTRL
		|| pVariant->vtEx == VT_EX_STYLE_SPIN
		|| pVariant->vtEx == VT_EX_STYLE_HORSCROLLBAR
		|| pVariant->vtEx == VT_EX_STYLE_VERSCROLLBAR)
	{
		*pVariant = (VARIANT)pProp->GetValue();
	}
	else if (pVariant->vtEx == VT_EX_FONT)
	{
		CIUIPropertyGridFontProperty *pFontProp = (CIUIPropertyGridFontProperty *)pProp;
		*pVariant = *pFontProp->GetLogFont();
	}
	else
	{
		ASSERT(FALSE);
	}

	// Some properties bind to this property.
	if (pVariant->lBeBindCount > 0)
	{
		UpdateAllPropertiesDisabledStatus();
	}

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CUIShopView *pView = pFrm->GetUIShopView();
	pView->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	return 0;
}

CMFCPropertyGridProperty *CPropertiesWnd::SuperadditionProperties(CMFCPropertyGridProperty *pParentGridProp, CTRLPROPERTIES *pCtrlProp)
{
	CMFCPropertyGridProperty *pLastGroup = NULL;

	LONG lVarCount = pCtrlProp->GetPropertyCount();
	for (LONG i = 0; i < lVarCount ; i++)
	{
		const IUI_VARIANT *pVariant = pCtrlProp->GetProperty(i);
		if (!pVariant->bShowInPropertiesWnd)
		{
			continue;
		}
		if (pVariant->bNeedToConvert)
		{
			continue;
		}

		if (pVariant->bGroup)
		{
			CMFCPropertyGridProperty *pTarget = FindItemByString(pParentGridProp, pVariant->strGroupName);
			if (pTarget == NULL)
			{
				pLastGroup = new CMFCPropertyGridProperty(pVariant->strGroupName);
				if (pParentGridProp == NULL)
				{
					m_wndPropList.AddProperty(pLastGroup);
				}
				else
				{
					pParentGridProp->AddSubItem(pLastGroup);
				}
			}
			else
			{
				pLastGroup = pTarget;
			}

			AddSubItem(pLastGroup, pVariant);
		}
		else
		{
			ASSERT(pLastGroup != NULL);
			AddSubItem(pLastGroup, pVariant);
		}
	}

	return pLastGroup;
}

int CPropertiesWnd::UpdateDisabledStatus(const IUI_VARIANT *pVariant, CMFCPropertyGridProperty *pItem)
{
	// 	if (pVariant->bDisabledWhileBindIs)
	// 	{
	// 		ASSERT(!pVariant->bDisabledWhileBindIsnot);
	// 		ASSERT(pVariant->GetBindVariant() != NULL);
	// 		IUI_VARIANT varCompare = pVariant->GetBindCompareValue();
	//
	// 		if (*pVariant->GetBindVariant() == varCompare)
	// 			pItem->Enable(FALSE);
	// 		else
	// 			pItem->Enable(TRUE);
	// 	}

	return 0;
}

int CPropertiesWnd::UpdateSubPropertyDisabledStatus(CMFCPropertyGridProperty *pParentProp)
{
	int nSubCount = pParentProp->GetSubItemsCount();
	for (int i = 0; i < nSubCount; ++i)
	{
		CMFCPropertyGridProperty *pSubProp = pParentProp->GetSubItem(i);

		if (pSubProp->IsGroup())
		{
			UpdateSubPropertyDisabledStatus(pSubProp);
		}
		else
		{
			IUI_VARIANT *pVariant = (IUI_VARIANT *)pSubProp->GetData();
			UpdateDisabledStatus(pVariant, pSubProp);
		}
	}
	return 0;
}

int CPropertiesWnd::UpdateAllPropertiesDisabledStatus()
{
	int nPropCount = m_wndPropList.GetPropertyCount();
	for (int i = 0; i < nPropCount; ++i)
	{
		CMFCPropertyGridProperty *pProp = m_wndPropList.GetProperty(i);
		if (pProp->IsGroup())
		{
			UpdateSubPropertyDisabledStatus(pProp);
			continue;
		}

		IUI_VARIANT *pVariant = (IUI_VARIANT *)pProp->GetData();
		UpdateDisabledStatus(pVariant, pProp);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

CMFCPropertyGridProperty *CPropertiesWnd::FindItemByString(CMFCPropertyGridProperty *pParentProp, const CString &strItemName)
{
	if (pParentProp == NULL)
	{
		int nCount = m_wndPropList.GetPropertyCount();
		for (int i = 0; i < nCount ; i++)
		{
			CMFCPropertyGridProperty *pProp = m_wndPropList.GetProperty(i);
			if (strItemName == pProp->GetName())
			{
				return pProp;
			}
		}

		return NULL;
	}
	else
	{
		int nCount = pParentProp->GetSubItemsCount();
		for (int i = 0; i < nCount ; i++)
		{
			CMFCPropertyGridProperty *pProp = pParentProp->GetSubItem(i);
			if (strItemName == pProp->GetName())
			{
				return pProp;
			}
		}

		return NULL;
	}

	return NULL;
}

int CPropertiesWnd::ShowProperties(CTRLPROPERTIES *pCtrlProp)
{
	m_wndPropList.RemoveAll();
	m_pCtrlProp = pCtrlProp;

	if (pCtrlProp == NULL)
	{
		m_wndPropList.Invalidate();
		return 0;
	}

	CMFCPropertyGridProperty *pLastProp = SuperadditionProperties(NULL, pCtrlProp);

	m_wndPropList.AdjustLayout();

	return 0;
}

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CPropertiesWnd::SetVSDotNetLook(BOOL bSet)
{
	m_wndPropList.SetVSDotNetLook(bSet);
	m_wndPropList.SetGroupNameFullWidth(bSet);
}

int CPropertiesWnd::DeleteAllItems()
{
	m_wndPropList.RemoveAll();
	m_wndPropList.AdjustLayout();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_PROLIST))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnSetFocus(CWnd *pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	UINT nCtrlID = (UINT)wParam;
	if (nCtrlID != IDC_PROLIST)
	{
		return 0;
	}

	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty *)lParam;

	IUI_VARIANT *pVariant = (IUI_VARIANT *)pProp->GetData();
	ASSERT(pVariant != NULL);

	// Add to undo list
	::CAction action;
	action.m_eAction = ::CAction::actionModifyProperty;
	action.m_pPropWnd = this;
	action.m_pCtrlProp = (CTRLPROPERTIES *)m_pCtrlProp;
	action.m_nPropIndexInCTRLPROPERTIES = m_pCtrlProp->GetIndex(pVariant);
	action.m_varPropertyValueOld = *pVariant;

	// Update the value
	UpdatePropertyAndUI(pProp);

	// Add to undo list
	action.m_varPropertyValueNew = *pVariant;
	g_pFrm->GetUIShopDoc()->AddUndo(&action);

	// Set modified flag
	g_pFrm->GetUIShopDoc()->SetModifiedFlag();

	return 0;
}
