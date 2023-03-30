#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "UIShop.h"
#include "IDMgr.h"
#include "TabOrderDlg.h"
#include "ClassWizardDlg.h"
#include "AddAllClassDlg.h"
#include "EventHandlerWizardDlg.h"
#include "Global.h"
#include <strsafe.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


int OpenFolderAndSelectItems(LPCWSTR lpsFolder, LPCWSTR *lpsItems, int nCount)
{
	if (lpsItems == NULL || lpsFolder == NULL)
	{
		return 1;
	}

	typedef HRESULT(WINAPI * PSHOpenFolderAndSelectItems)(LPCITEMIDLIST, UINT, LPCITEMIDLIST *, DWORD);

	PSHOpenFolderAndSelectItems pFuc = NULL;
	HMODULE hMod = NULL;

	LPSHELLFOLDER pDesktopFolder = NULL;
	LPITEMIDLIST pIDList = NULL;
	LPCITEMIDLIST lpIDFolder = NULL;
	LPCITEMIDLIST *lpIDFiles = NULL;

	ULONG chEaten;
	ULONG dwAttributes;
	HRESULT hr;
	HRESULT hCom;

	CString strFolder = lpsFolder;
	strFolder.Replace('/', '\\');
	strFolder.TrimRight('\\');

	if (!PathFileExists(strFolder))
	{
		return 2;
	}

	strFolder.AppendChar('\\');

	hMod = ::LoadLibrary(L"SHELL32.DLL");

	if (hMod != NULL)
	{
		pFuc = (PSHOpenFolderAndSelectItems)::GetProcAddress(hMod, "SHOpenFolderAndSelectItems");
	}

	if (pFuc != NULL)
	{
		if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
		{
			hr = pDesktopFolder->ParseDisplayName(NULL, 0, (LPOLESTR)(LPCWSTR)strFolder,
					&chEaten, &pIDList, &dwAttributes);

			if (FAILED(hr))
			{
				::FreeLibrary(hMod);

				return 2;
			}

			lpIDFolder = pIDList;

			if (lpsItems && nCount > 0)
			{
				lpIDFiles = new LPCITEMIDLIST[nCount];

				for (int i = 0; i < nCount; i++)
				{
					hr = pDesktopFolder->ParseDisplayName(NULL, 0, (LPOLESTR)lpsItems[i], &chEaten, &pIDList,
							&dwAttributes);
					lpIDFiles[i] = pIDList;
				}
			}

			hCom = CoInitialize(NULL);
			hr = pFuc(lpIDFolder, (lpIDFiles ? nCount : 0), lpIDFiles, NULL);

			pDesktopFolder->Release();

			if (lpIDFiles)
			{
				delete [] lpIDFiles;
			}

			if (hCom == S_OK)
			{
				::CoUninitialize();
			}

			::FreeLibrary(hMod);

			return (hr == S_OK ? 0 : 1);
		}
	}
	else
	{
		if (hMod != NULL)
		{
			::FreeLibrary(hMod);
		}

		CString strParam = lpsItems[0];

		if (PathFileExists(strParam))
		{
			strParam = _T("/select,") + strParam;
		}
		else
		{
			strParam = strFolder;
		}

		HINSTANCE hRet = ShellExecute(NULL, TEXT("open"), TEXT("explorer.exe"), strParam, NULL,
				SW_SHOWNORMAL);

		if ((UINT_PTR)hRet <= HINSTANCE_ERROR)
		{
			return -1;
		}
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
	m_hWindowsRoot = NULL;
	m_hResourceRoot = NULL;
	m_hResDefaultFontBranch = NULL;
	m_hResDefaultFont = NULL;
	m_hResFontBranch = NULL;
	m_hResDefaultColorBranch = NULL;
	m_hResDefaultColor = NULL;
	m_hResColorBranch = NULL;
	m_hStyleRoot = NULL;
	m_hStylePushButtonRoot = NULL;
	m_hStyleCheckRoot = NULL;
	m_hStyleRadioRoot = NULL;
	m_hStyleComboBoxRoot = NULL;
	m_hStyleStaticRoot = NULL;
	m_hStyleEditRoot = NULL;
	m_hStyleSliderRoot = NULL;
	m_hStyleProgressRoot = NULL;
	m_hStyleSpinRoot = NULL;
	m_hStyleHotKeyRoot = NULL;
	m_hStyleIPAddressRoot = NULL;
	m_hStyleSliderExRoot = NULL;
	m_hStyleListCtrlRoot = NULL;
	m_hStyleTreeCtrlRoot = NULL;
	m_hStyleTreeListRoot = NULL;
	m_hStyleRichEditRoot = NULL;
	m_hStyleIMRichEditRoot = NULL;
	m_hStyleHtmlCtrlRoot = NULL;
	m_hStyleDateTimeCtrlRoot = NULL;
	m_hStyleWaveCtrlRoot = NULL;
	m_hStyleMenuRoot = NULL;
	m_hStyleMenuBarRoot = NULL;
	m_hStyleHeaderRoot = NULL;
	m_hStyleHorScrollbarRoot = NULL;
	m_hStyleVerScrollbarRoot = NULL;
	m_hStyleSplitterRoot = NULL;
	m_hStyleWLLineRoot = NULL;
	m_hStyleWLPicRoot = NULL;
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(IDSM_INSERT, OnInsertWindow)
	ON_COMMAND(IDSM_PROPERTIES, OnWindowProperties)
	ON_COMMAND(IDSM_TEST, OnTestWindow)
	ON_COMMAND(IDSM_TEST_HIGH_DPI, OnTestHighDpi)
	ON_COMMAND(IDSM_TEST_CUR_FRAME, OnTestCurFrame)
	ON_COMMAND(IDSM_DELETEWINDOW, OnDeleteWindow)
	ON_COMMAND(IDSM_TABORDER, OnTabOrder)
	ON_COMMAND(IDSM_FINDINHD, OnFindInHD)
	ON_UPDATE_COMMAND_UI(IDSM_DELETEWINDOW, &CFileView::OnUpdateDeletewindow)
	ON_UPDATE_COMMAND_UI(IDSM_TABORDER, &CFileView::OnUpdateTaborder)
	ON_UPDATE_COMMAND_UI(IDSM_TEST, &CFileView::OnUpdateTest)
	ON_UPDATE_COMMAND_UI(IDSM_TEST_HIGH_DPI, &CFileView::OnUpdateTestHighDpi)
	ON_UPDATE_COMMAND_UI(IDSM_TEST_CUR_FRAME, &CFileView::OnUpdateTestCurFrame)
	ON_UPDATE_COMMAND_UI(IDSM_PROPERTIES, &CFileView::OnUpdateProperties)
	ON_COMMAND(IDSM_ADD_CLASS, &CFileView::OnAddClass)
	ON_UPDATE_COMMAND_UI(IDSM_ADD_CLASS, &CFileView::OnUpdateAddClass)
	ON_COMMAND(IDSM_ADD_ALL_CLASSES, &CFileView::OnAddAllClasses)
	ON_UPDATE_COMMAND_UI(IDSM_ADD_ALL_CLASSES, &CFileView::OnUpdateAddAllClasses)
#ifdef ADV
	ON_COMMAND(IDSM_ADD_EVENT_HANDLER_CLASS, &CFileView::OnAddEventHandlerClass)
	ON_UPDATE_COMMAND_UI(IDSM_ADD_EVENT_HANDLER_CLASS, &CFileView::OnUpdateAddEventHandlerClass)
#endif
	ON_COMMAND(IDSM_ADD_ALL_EVENT_HANDLER_CLASSES, &CFileView::OnAddAllEventHandlerClasses)
	ON_UPDATE_COMMAND_UI(IDSM_ADD_ALL_EVENT_HANDLER_CLASSES, &CFileView::OnUpdateAddAllEventHandlerClasses)
	ON_COMMAND(IDSM_INSERT_GENERAL_FONT, &CFileView::OnInsertGeneralFont)
	ON_COMMAND(IDSM_INSERT_CONDITION_FONT_CHILD, &CFileView::OnInsertConditionFontChild)
	ON_UPDATE_COMMAND_UI(IDSM_INSERT_CONDITION_FONT_CHILD, &CFileView::OnUpdateInsertConditionFont)
	ON_COMMAND(IDSM_INSERT_RES_COLOR, &CFileView::OnInsertResColor)
	ON_COMMAND(IDSM_DELETE_RESOURCE, &CFileView::OnDeleteResource)
	ON_UPDATE_COMMAND_UI(IDSM_DELETE_RESOURCE, &CFileView::OnUpdateDeleteResource)
	ON_COMMAND(IDSM_CONVERT_TO_CONDITION_FONT, &CFileView::OnConvertToConditionFont)
	ON_UPDATE_COMMAND_UI(IDSM_CONVERT_TO_CONDITION_FONT, &CFileView::OnUpdateConvertToConditionFont)
	ON_COMMAND(IDSM_INSERT_STYLE_ITEM, &CFileView::OnInsertStyleItem)
	ON_UPDATE_COMMAND_UI(IDSM_INSERT_STYLE_ITEM, &CFileView::OnUpdateInsertStyleItem)
	ON_COMMAND(IDSM_DELETE_STYLE, &CFileView::OnDeleteStyle)
	ON_UPDATE_COMMAND_UI(IDSM_DELETE_STYLE, &CFileView::OnUpdateDeleteStyle)
END_MESSAGE_MAP()

extern CMainFrame *g_pFrm;


//////////////////////////////////////////////////////////////////////////
// protected

int CFileView::InsertWindowsGroup(HTREEITEM hRoot)
{
	if (hRoot == NULL)
	{
		m_hWindowsRoot = m_treeWindow.InsertItem(_T("Windows"));

		// set item data to tree item
		TREE_ITEMDATA *pData = new TREE_ITEMDATA;
		pData->eTreeItemType = WINDOWS_ROOT;
		m_treeWindow.SetItemData(m_hWindowsRoot, (DWORD_PTR)pData);

		UpdateUIWindowsCount();
	}

	// Show window's ID.
	const std::vector<UIWNDPROPERTIES *> *pvWindows = CUIMgr::GetUIWndPropertiesList();
	size_t nSize = pvWindows->size();
	for (size_t i = 0; i < nSize; ++i)
	{
		UIWNDPROPERTIES *pUIWndProp = (*pvWindows)[i];

		// insert window to tree
		HTREEITEM hItem = m_treeWindow.InsertItem(pUIWndProp->m_strID, 1, 1, m_hWindowsRoot);

		// set item data to tree item
		TREE_ITEMDATA *pData = new TREE_ITEMDATA;
		pData->eTreeItemType = WINDOWS_NODE;
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);

		// create the CUIDesignWnd
		pData->wndUIDesign.SetUIWndProperties(pUIWndProp);
	}

	m_treeWindow.SelectItem(m_hWindowsRoot);
	m_treeWindow.Expand(m_hWindowsRoot, TVE_EXPAND);

	return 0;
}

int CFileView::InitFontResourceBranch()
{
	//
	// Insert Default Font branch
	//
	m_hResDefaultFontBranch = m_treeWindow.InsertItem(_T("Default Font"), m_hResourceRoot);
	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = RES_DEFAULT_FONT_BRANCH;
	m_treeWindow.SetItemData(m_hResDefaultFontBranch, (DWORD_PTR)pData);

	//
	// Insert default font
	//
	FONTRESOURCEITEM *pResDefaultFont = CUIMgr::GetDefaultFontResource();
	if (pResDefaultFont->m_bConditionFont)
	{
		//
		// Insert condition child font
		//
		m_hResDefaultFont = InsertConditionFont(m_hResDefaultFontBranch, pResDefaultFont);
	}
	else
	{
		//
		// Insert general font
		//
		m_hResDefaultFont = InsertGeneralFont(m_hResDefaultFontBranch, pResDefaultFont);
	}

	//
	// Insert Font resource branch
	//
	m_hResFontBranch = m_treeWindow.InsertItem(_T("Font"), m_hResourceRoot);
	pData = new TREE_ITEMDATA;
	pData->eTreeItemType = RES_FONT_BRANCH;
	m_treeWindow.SetItemData(m_hResFontBranch, (DWORD_PTR)pData);

	int nCount = CUIMgr::GetResourceCount(REST_FONT);
	for (int i = 0; i < nCount; ++i)
	{
		FONTRESOURCEITEM *pResFont = CUIMgr::GetFontResourceItem(i);
		ASSERT(pResFont != NULL);

		if (pResFont->m_bConditionFont)
		{
			// Insert condition child font
			InsertConditionFont(m_hResFontBranch, pResFont);
		}
		else
		{
			// Insert general font
			InsertGeneralFont(m_hResFontBranch, pResFont);
		}
	}

	return 0;
}

HTREEITEM CFileView::InsertConditionFont(HTREEITEM hParentItem, FONTRESOURCEITEM *pConditionFont)
{
	HTREEITEM hRetItem = m_treeWindow.InsertItem(pConditionFont->m_strFontID, 4, 4, hParentItem);

	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = RES_CONDITION_FONT_NODE;
	pData->pResFont = pConditionFont;
	m_treeWindow.SetItemData(hRetItem, (DWORD_PTR)pData);

	InsertConditionFontChildren(hRetItem, pConditionFont);

	return hRetItem;
}

int CFileView::InsertConditionFontChildren(HTREEITEM hParentItem, FONTRESOURCEITEM *pConditionFont)
{
	// Insert condition font children
	std::map<CString, RESFONTPROPERTIES *>::iterator it = pConditionFont->m_mapConditionFont.begin();
	for (; it != pConditionFont->m_mapConditionFont.end(); ++it)
	{
		HTREEITEM hFont = m_treeWindow.InsertItem(it->first, 5, 5, hParentItem);
		TREE_ITEMDATA *pData = new TREE_ITEMDATA;
		pData->eTreeItemType = RES_CONDITION_CHILD_FONT_NODE;
		pData->pResourceOrStyle = it->second;
		m_treeWindow.SetItemData(hFont, (DWORD_PTR)pData);
	}

	return 0;
}

HTREEITEM CFileView::InsertGeneralFont(HTREEITEM hParentItem, FONTRESOURCEITEM *pGeneralFont)
{
	if (pGeneralFont == NULL)
	{
		return NULL;
	}
	if (pGeneralFont->m_bConditionFont)
	{
		return NULL;
	}
	if (pGeneralFont->m_pFontResProp == NULL)
	{
		return NULL;
	}

	HTREEITEM hRetItem = m_treeWindow.InsertItem(pGeneralFont->m_pFontResProp->m_strID, 3, 3, hParentItem);

	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = RES_GENERAL_FONT_NODE;
	pData->pResFont = pGeneralFont;
	m_treeWindow.SetItemData(hRetItem, (DWORD_PTR)pData);

	return hRetItem;
}

int CFileView::InitColorResourceBranch()
{
	//
	// Insert Default color resource
	//
	m_hResDefaultColorBranch = m_treeWindow.InsertItem(_T("Default Color"), m_hResourceRoot);
	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = RES_DEFAULT_COLOR_BRANCH;
	m_treeWindow.SetItemData(m_hResDefaultColorBranch, (DWORD_PTR)pData);

	// Insert default resource to tree
	RESCOLORPROPERTIES *pResDefaultColor = CUIMgr::GetDefaultColorResource();
	ASSERT((CONTROL_TYPE)pResDefaultColor->m_eControlType == CT_RESOURCE_COLOR);

	m_hResDefaultColor = m_treeWindow.InsertItem(_T("Default"), 2, 2, m_hResDefaultColorBranch);
	pData = new TREE_ITEMDATA;
	pData->eTreeItemType = RES_COLOR_NODE;
	pData->pResourceOrStyle = pResDefaultColor;
	m_treeWindow.SetItemData(m_hResDefaultColor, (DWORD_PTR)pData);

	//
	// Insert Color resource
	//
	m_hResColorBranch = m_treeWindow.InsertItem(_T("Color"), m_hResourceRoot);
	pData = new TREE_ITEMDATA;
	pData->eTreeItemType = RES_COLOR_BRANCH;
	m_treeWindow.SetItemData(m_hResColorBranch, (DWORD_PTR)pData);

	int nCount = CUIMgr::GetResourceCount(REST_COLOR);
	for (int i = 0; i < nCount; ++i)
	{
		RESCOLORPROPERTIES *pResProp = CUIMgr::GetColorResourceItem(i);
		ASSERT(pResProp != NULL);
		ASSERT((CONTROL_TYPE)pResProp->m_eControlType == CT_RESOURCE_COLOR);

		TREE_ITEMDATA *pData = new TREE_ITEMDATA;
		// Insert resource to tree
		HTREEITEM hItem = m_treeWindow.InsertItem(pResProp->m_strID, 2, 2, m_hResColorBranch);
		pData->eTreeItemType = RES_COLOR_NODE;

		// set item data to tree item
		pData->pResourceOrStyle = pResProp;
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}

	return 0;
}

HTREEITEM CFileView::InsertEachStyle(LPCTSTR lpszNode, TREE_ITEM_TYPE eTreeItemTypeRoot, TREE_ITEM_TYPE eTreeItemTypeNode, STYLE_TYPE eStyleType, CONTROL_TYPE ectStyleType)
{
	HTREEITEM hItemRet = NULL;
	hItemRet = m_treeWindow.InsertItem(lpszNode, m_hStyleRoot);
	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = eTreeItemTypeRoot;
	m_treeWindow.SetItemData(hItemRet, (DWORD_PTR)pData);

	int nCount = CUIMgr::GetStyleCount(eStyleType);
	for (int i = 0; i < nCount; ++i)
	{
		CTRLPROPERTIES *pStyleProp = CUIMgr::GetStyleItem(eStyleType, i);
		ASSERT(pStyleProp != NULL);
		ASSERT((CONTROL_TYPE)pStyleProp->m_eControlType == ectStyleType);

		TREE_ITEMDATA *pData = new TREE_ITEMDATA;
		// Insert style to tree
		HTREEITEM hItem = m_treeWindow.InsertItem(pStyleProp->m_strID, hItemRet);
		pData->eTreeItemType = eTreeItemTypeNode;

		// set item data to tree item
		pData->pResourceOrStyle = pStyleProp;
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}

	return hItemRet;
}

int CFileView::InitStyleBrach()
{
	m_hStyleRoot = m_treeWindow.InsertItem(_T("Style"));
	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = STYLE_ROOT;
	m_treeWindow.SetItemData(m_hStyleRoot, (DWORD_PTR)pData);

	// Insert each style
	m_hStylePushButtonRoot = InsertEachStyle(_T("PushButton"), STYLE_PUSHBUTTON_ROOT, STYLE_PUSHBUTTON_NODE, STYLET_PUSHBUTTON, CT_STYLE_PUSHBUTTON);
	m_hStyleCheckRoot = InsertEachStyle(_T("CheckBox"), STYLE_CHECK_ROOT, STYLE_CHECK_NODE, STYLET_CHECK, CT_STYLE_CHECK);
	m_hStyleRadioRoot = InsertEachStyle(_T("RadioButton"), STYLE_RADIO_ROOT, STYLE_RADIO_NODE, STYLET_RADIO, CT_STYLE_RADIO);
	m_hStyleComboBoxRoot = InsertEachStyle(_T("ComboBox"), STYLE_COMBOBOX_ROOT, STYLE_COMBOBOX_NODE, STYLET_COMBOBOX, CT_STYLE_COMBOBOX);
	m_hStyleStaticRoot = InsertEachStyle(_T("Static"), STYLE_STATIC_ROOT, STYLE_STATIC_NODE, STYLET_STATIC, CT_STYLE_STATIC);
	m_hStyleEditRoot = InsertEachStyle(_T("Edit"), STYLE_EDIT_ROOT, STYLE_EDIT_NODE, STYLET_EDIT, CT_STYLE_EDIT);
	m_hStyleSliderRoot = InsertEachStyle(_T("Slider"), STYLE_SLIDER_ROOT, STYLE_SLIDER_NODE, STYLET_SLIDER, CT_STYLE_SLIDER);
	m_hStyleProgressRoot = InsertEachStyle(_T("Progress"), STYLE_PROGRESS_ROOT, STYLE_PROGRESS_NODE, STYLET_PROGRESS, CT_STYLE_PROGRESS);
	m_hStyleSpinRoot = InsertEachStyle(_T("Spin"), STYLE_SPIN_ROOT, STYLE_SPIN_NODE, STYLET_SPIN, CT_STYLE_SPIN);
	m_hStyleHotKeyRoot = InsertEachStyle(_T("HotKey"), STYLE_HOTKEY_ROOT, STYLE_HOTKEY_NODE, STYLET_HOTKEY, CT_STYLE_HOTKEY);
	m_hStyleIPAddressRoot = InsertEachStyle(_T("IPAdress"), STYLE_IPADDRESS_ROOT, STYLE_IPADDRESS_NODE, STYLET_IPADDRESS, CT_STYLE_IPADDRESS);
	m_hStyleSliderExRoot = InsertEachStyle(_T("SliderEx"), STYLE_SLIDEREX_ROOT, STYLE_SLIDEREX_NODE, STYLET_SLIDEREX, CT_STYLE_SLIDEREX);
	m_hStyleListCtrlRoot = InsertEachStyle(_T("ListControl"), STYLE_LISTCTRL_ROOT, STYLE_LISTCTRL_NODE, STYLET_LISTCTRL, CT_STYLE_LISTCTRL);
	m_hStyleTreeCtrlRoot = InsertEachStyle(_T("TreeControl"), STYLE_TREECTRL_ROOT, STYLE_TREECTRL_NODE, STYLET_TREECTRL, CT_STYLE_TREECTRL);
	m_hStyleTreeListRoot = InsertEachStyle(_T("TreeList"), STYLE_TREELIST_ROOT, STYLE_TREELIST_NODE, STYLET_TREELIST, CT_STYLE_TREELIST);
	m_hStyleRichEditRoot = InsertEachStyle(_T("RichEdit"), STYLE_RICHEDIT_ROOT, STYLE_RICHEDIT_NODE, STYLET_RICHEDIT, CT_STYLE_RICHEDIT);
	m_hStyleIMRichEditRoot = InsertEachStyle(_T("IM RichEdit"), STYLE_IMRICHEDIT_ROOT, STYLE_IMRICHEDIT_NODE, STYLET_IMRICHEDIT, CT_STYLE_IMRICHEDIT);
	m_hStyleHtmlCtrlRoot = InsertEachStyle(_T("HtmlControl"), STYLE_HTMLCTRL_ROOT, STYLE_HTMLCTRL_NODE, STYLET_HTMLCTRL, CT_STYLE_HTMLCTRL);
	m_hStyleDateTimeCtrlRoot = InsertEachStyle(_T("DateTimeControl"), STYLE_DATETIMECTRL_ROOT, STYLE_DATETIMECTRL_NODE, STYLET_DATETIMECTRL, CT_STYLE_DATETIMECTRL);
	m_hStyleWaveCtrlRoot = InsertEachStyle(_T("WaveControl"), STYLE_WAVECTRL_ROOT, STYLE_WAVECTRL_NODE, STYLET_WAVECTRL, CT_STYLE_WAVECTRL);
	m_hStyleMenuRoot = InsertEachStyle(_T("Menu"), STYLE_MENU_ROOT, STYLE_MENU_NODE, STYLET_MENU, CT_STYLE_MENU);
	m_hStyleMenuBarRoot = InsertEachStyle(_T("MenuBar"), STYLE_MENUBAR_ROOT, STYLE_MENUBAR_NODE, STYLET_MENUBAR, CT_STYLE_MENUBAR);
	m_hStyleHeaderRoot = InsertEachStyle(_T("Header"), STYLE_HEADERCTRL_ROOT, STYLE_HEADERCTRL_NODE, STYLET_HEADERCTRL, CT_STYLE_HEADERCTRL);
	m_hStyleHorScrollbarRoot = InsertEachStyle(_T("Horizontrl Scrollbar"), STYLE_HORSCROLLBAR_ROOT, STYLE_HORSCROLLBAR_NODE, STYLET_HORSCROLLBAR, CT_STYLE_HORSCROLLBAR);
	m_hStyleVerScrollbarRoot = InsertEachStyle(_T("Vertical Scrollbar"), STYLE_VERSCROLLBAR_ROOT, STYLE_VERSCROLLBAR_NODE, STYLET_VERSCROLLBAR, CT_STYLE_VERSCROLLBAR);
	m_hStyleSplitterRoot = InsertEachStyle(_T("Splitter Bar"), STYLE_SPLITTER_ROOT, STYLE_SPLITTER_NODE, STYLET_SPLITTER, CT_STYLE_SPLITTER);

	m_hStyleWLLineRoot = InsertEachStyle(_T("Windowless Line"), STYLE_WL_LINE_ROOT, STYLE_WL_LINE_NODE, STYLET_WL_LINE, CT_STYLE_WL_LINE);
	m_hStyleWLPicRoot = InsertEachStyle(_T("Windowless Picture"), STYLE_WL_PIC_ROOT, STYLE_WL_PIC_NODE, STYLET_WL_PICTURE, CT_STYLE_WL_PICTURE);

	return 0;
}

int CFileView::UpdateUIWindowsCount()
{
	const std::vector<UIWNDPROPERTIES *> *pvWindows = CUIMgr::GetUIWndPropertiesList();
	size_t nSize = pvWindows->size();

	CString strWindow;
	strWindow.Format(_T("Windows (%d windows)"), nSize);
	m_treeWindow.SetItemText(m_hWindowsRoot, strWindow);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_treeWindow.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}


void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_treeWindow.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

BOOL CFileView::OnOpenFile()
{
	HTREEITEM hRoot = m_treeWindow.GetRootItem();

	// Insert "Windows" group
	InsertWindowsGroup(hRoot);

	// Insert Resource branch
	m_hResourceRoot = m_treeWindow.InsertItem(_T("Resource"));
	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = RESOURCE_ROOT;
	m_treeWindow.SetItemData(m_hResourceRoot, (DWORD_PTR)pData);

	InitFontResourceBranch();
	InitColorResourceBranch();

	// Insert Style Branch
	InitStyleBrach();

	//    g_pFrm->ShowTBControlBar(TRUE);

	return TRUE;
}

HTREEITEM CFileView::FindItem(LPCTSTR lpszWindowID, HTREEITEM hItem)
{
	if (hItem == NULL)
	{
		hItem = m_treeWindow.GetRootItem();
	}

	HTREEITEM hRet = NULL;
	while (hItem != NULL)
	{
		// Find itself
		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hItem);
		if (pData->pResourceOrStyle != NULL)    // it's resource
		{
			CString strID = (LPCTSTR)pData->pResourceOrStyle->m_strID;
			if (strID == lpszWindowID)
			{
				hRet = hItem;
				break;
			}
		}
		else
		{
			UIWNDPROPERTIES *pUIProp = pData->wndUIDesign.GetUIWndProperties();
			if (pUIProp != NULL)
			{
				CString strID = (LPCTSTR)pUIProp->m_strID;
				if (strID == lpszWindowID)
				{
					hRet = hItem;
					break;
				}
			}
		}

		// Find its children
		HTREEITEM hChild = m_treeWindow.GetChildItem(hItem);
		if (hChild != NULL)
		{
			hRet = FindItem(lpszWindowID, hChild);
			if (hRet != NULL)
			{
				break;
			}
		}

		// Release its sibling
		hItem = m_treeWindow.GetNextSiblingItem(hItem);
	}

	return hRet;
}

HTREEITEM CFileView::FindItem(const CTRLPROPERTIES *pProp, HTREEITEM hItem)
{
	if (hItem == NULL)
	{
		hItem = m_treeWindow.GetRootItem();
	}

	HTREEITEM hRet = NULL;
	while (hItem != NULL)
	{
		// Find itself
		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hItem);
		if (pData->eTreeItemType == WINDOWS_NODE)
		{
			if (pData->wndUIDesign.GetUIWndProperties() == pProp)
			{
				hRet = hItem;
				break;
			}
		}
		else if (pData->eTreeItemType == RES_CONDITION_CHILD_FONT_NODE || pData->eTreeItemType == RES_COLOR_NODE
			|| pData->eTreeItemType == STYLE_PUSHBUTTON_NODE || pData->eTreeItemType == STYLE_HORSCROLLBAR_NODE || pData->eTreeItemType == STYLE_VERSCROLLBAR_NODE)
		{
			if (pData->pResourceOrStyle == pProp)
			{
				hRet = hItem;
				break;
			}
		}
		else if (pData->eTreeItemType == RES_GENERAL_FONT_NODE || pData->eTreeItemType == RES_CONDITION_FONT_NODE)
		{
			if (pData->pResFont->m_pFontResProp == pProp)
			{
				hRet = hItem;
				break;
			}
		}

		// Find its children
		HTREEITEM hChild = m_treeWindow.GetChildItem(hItem);
		if (hChild != NULL)
		{
			hRet = FindItem(pProp, hChild);
			if (hRet != NULL)
			{
				break;
			}
		}

		// Release its sibling
		hItem = m_treeWindow.GetNextSiblingItem(hItem);
	}

	return hRet;
}

CString GetNextResID(TREE_ITEM_TYPE eTreeItemType)
{
	CString strResID;

	if (eTreeItemType == RES_GENERAL_FONT_NODE)
	{
		strResID = CIDMgr::NextFontResID();

		FONTRESOURCEITEM *pResFont = CUIMgr::GetFontResourceItem(strResID);
		while (pResFont != NULL)
		{
			strResID = GetNextResID(eTreeItemType);

			pResFont = CUIMgr::GetFontResourceItem(strResID);
		}
	}
	else if (eTreeItemType == RES_COLOR_NODE)
	{
		strResID = CIDMgr::NextColorResID();

		RESCOLORPROPERTIES *pResProp = CUIMgr::GetColorResourceItem(strResID);
		while (pResProp != NULL)
		{
			strResID = GetNextResID(eTreeItemType);

			pResProp = CUIMgr::GetColorResourceItem(strResID);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return strResID;
}


#define BEGIN_GET_NEXT_STYLE_ID(TreeItemTypeValue, StyleTypeValue, CIDMgrNextFun) \
	if (eTreeItemType == TreeItemTypeValue) \
	{ \
		strStyleID = CIDMgrNextFun; \
		CTRLPROPERTIES *pStyleProp = CUIMgr::GetStyleItem(StyleTypeValue, strStyleID); \
		while (pStyleProp != NULL) \
		{ \
			strStyleID = GetNextStyleID(eTreeItemType); \
			pStyleProp = CUIMgr::GetStyleItem(StyleTypeValue, strStyleID); \
		} \
	}
#define GET_NEXT_STYLE_ID(TreeItemTypeValue, StyleTypeValue, CIDMgrNextFun) \
	else if (eTreeItemType == TreeItemTypeValue) \
	{ \
		strStyleID = CIDMgrNextFun; \
		CTRLPROPERTIES *pStyleProp = CUIMgr::GetStyleItem(StyleTypeValue, strStyleID); \
		while (pStyleProp != NULL) \
		{ \
			strStyleID = GetNextStyleID(eTreeItemType); \
			pStyleProp = CUIMgr::GetStyleItem(StyleTypeValue, strStyleID); \
		} \
	}
#define END_GET_NEXT_STYLE_ID() \
	else \
	{ \
		ASSERT(FALSE); \
	}

CString GetNextStyleID(TREE_ITEM_TYPE eTreeItemType)
{
	CString strStyleID;

	BEGIN_GET_NEXT_STYLE_ID(STYLE_PUSHBUTTON_NODE, STYLET_PUSHBUTTON, CIDMgr::NextButtonID())
		GET_NEXT_STYLE_ID(STYLE_CHECK_NODE, STYLET_CHECK, CIDMgr::NextCheckBoxID())
		GET_NEXT_STYLE_ID(STYLE_RADIO_NODE, STYLET_RADIO, CIDMgr::NextRadioButtonID())
		GET_NEXT_STYLE_ID(STYLE_COMBOBOX_NODE, STYLET_COMBOBOX, CIDMgr::NextComboBoxID())
		GET_NEXT_STYLE_ID(STYLE_STATIC_NODE, STYLET_STATIC, CIDMgr::NextStaticID())
		GET_NEXT_STYLE_ID(STYLE_EDIT_NODE, STYLET_EDIT, CIDMgr::NextEditID())
		GET_NEXT_STYLE_ID(STYLE_SLIDER_NODE, STYLET_SLIDER, CIDMgr::NextSliderID())
		GET_NEXT_STYLE_ID(STYLE_PROGRESS_NODE, STYLET_PROGRESS, CIDMgr::NextProgressID())
		GET_NEXT_STYLE_ID(STYLE_SPIN_NODE, STYLET_SPIN, CIDMgr::NextSpinID())
		GET_NEXT_STYLE_ID(STYLE_HOTKEY_NODE, STYLET_HOTKEY, CIDMgr::NextHotKeyID())
		GET_NEXT_STYLE_ID(STYLE_IPADDRESS_NODE, STYLET_IPADDRESS, CIDMgr::NextIPAddressID())
		GET_NEXT_STYLE_ID(STYLE_SLIDEREX_NODE, STYLET_SLIDEREX, CIDMgr::NextSliderExID())
		GET_NEXT_STYLE_ID(STYLE_LISTCTRL_NODE, STYLET_LISTCTRL, CIDMgr::NextListCtrlID())
		GET_NEXT_STYLE_ID(STYLE_TREECTRL_NODE, STYLET_TREECTRL, CIDMgr::NextTreeCtrlID())
		GET_NEXT_STYLE_ID(STYLE_TREELIST_NODE, STYLET_TREELIST, CIDMgr::NextTreeLstID())
		GET_NEXT_STYLE_ID(STYLE_RICHEDIT_NODE, STYLET_RICHEDIT, CIDMgr::NextRichEditID())
		GET_NEXT_STYLE_ID(STYLE_IMRICHEDIT_NODE, STYLET_IMRICHEDIT, CIDMgr::NextIMRichEditID())
		GET_NEXT_STYLE_ID(STYLE_HTMLCTRL_NODE, STYLET_HTMLCTRL, CIDMgr::NextHtmlCtrlID())
		GET_NEXT_STYLE_ID(STYLE_DATETIMECTRL_NODE, STYLET_DATETIMECTRL, CIDMgr::NextDateTimeCtrlID())
		GET_NEXT_STYLE_ID(STYLE_WAVECTRL_NODE, STYLET_WAVECTRL, CIDMgr::NextWaveCtrlID())
		GET_NEXT_STYLE_ID(STYLE_MENU_NODE, STYLET_MENU, CIDMgr::NextMenuID())
		GET_NEXT_STYLE_ID(STYLE_MENUBAR_NODE, STYLET_MENUBAR, CIDMgr::NextMenuBarID())
		GET_NEXT_STYLE_ID(STYLE_HEADERCTRL_NODE, STYLET_HEADERCTRL, CIDMgr::NextHeaderID())
		GET_NEXT_STYLE_ID(STYLE_HORSCROLLBAR_NODE, STYLET_HORSCROLLBAR, CIDMgr::NextHorScrollbarID())
		GET_NEXT_STYLE_ID(STYLE_VERSCROLLBAR_NODE, STYLET_VERSCROLLBAR, CIDMgr::NextVerScrollbarID())
		GET_NEXT_STYLE_ID(STYLE_SPLITTER_NODE, STYLET_SPLITTER, CIDMgr::NextSplitterID())
	END_GET_NEXT_STYLE_ID()

	return strStyleID;
}

int CFileView::InsertResource(TREE_ITEM_TYPE eTreeItemType)
{
	CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
	ASSERT_VALID(pDoc);

	// Assign new resource ID
	CString strResID = GetNextResID(eTreeItemType);

	CTRLPROPERTIES *pResProp = NULL;

	HTREEITEM hRoot = NULL;
	HTREEITEM hItem = NULL;
	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = eTreeItemType;

	if (eTreeItemType == RES_GENERAL_FONT_NODE)
	{
		hRoot = m_hResFontBranch;
		pResProp = new RESFONTPROPERTIES();

		//
		hItem = m_treeWindow.InsertItem(strResID, 3, 3, hRoot);

		// Insert to CUIMgr
		FONTRESOURCEITEM *pFri = new FONTRESOURCEITEM;
		pFri->m_bConditionFont = false;
		pFri->m_pFontResProp = (RESFONTPROPERTIES *)pResProp;
		int nIndex = CUIMgr::AddFontResourceItem(pFri);

		// Set Item data
		pData->pResFont = pFri;
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}
	else if (eTreeItemType == RES_COLOR_NODE)
	{
		hRoot = m_hResColorBranch;
		pResProp = new RESCOLORPROPERTIES();

		//
		hItem = m_treeWindow.InsertItem(strResID, 2, 2, hRoot);

		// Insert to CUIMgr
		CUIMgr::AddColorResourceItem(pResProp);

		// Set Item data
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}
	else if (eTreeItemType == STYLE_MENU_NODE)
	{
		hRoot = m_hStyleMenuRoot;
		pResProp = new MENUPROPERTIES();

		//
		hItem = m_treeWindow.InsertItem(strResID, hRoot);

		// Set Item data
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}
	else if (eTreeItemType == STYLE_HEADERCTRL_NODE)
	{
		hRoot = m_hStyleHeaderRoot;
		pResProp = new HEADERPROPERTIES();

		//
		hItem = m_treeWindow.InsertItem(strResID, hRoot);

		// Set Item data
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}
	else if (eTreeItemType == STYLE_HORSCROLLBAR_NODE)
	{
		hRoot = m_hStyleHorScrollbarRoot;
		pResProp = new HORSBPROPERTIES();

		//
		hItem = m_treeWindow.InsertItem(strResID, hRoot);

		// Set Item data
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}
	else if (eTreeItemType == STYLE_VERSCROLLBAR_NODE)
	{
		hRoot = m_hStyleVerScrollbarRoot;
		pResProp = new VERSBPROPERTIES();

		//
		hItem = m_treeWindow.InsertItem(strResID, hRoot);

		// Set Item data
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}
	else if (eTreeItemType == STYLE_SPLITTER_NODE)
	{
		hRoot = m_hStyleSplitterRoot;
		pResProp = new SPLITTERPROPERTIES();

		//
		hItem = m_treeWindow.InsertItem(strResID, hRoot);

		// Set Item data
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}
	else if (eTreeItemType == STYLE_WL_LINE_NODE)
	{
		hRoot = m_hStyleWLLineRoot;
		pResProp = new LINEWLPROPERTIES();

		//
		hItem = m_treeWindow.InsertItem(strResID, hRoot);

		// Set Item data
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}
	else if (eTreeItemType == STYLE_WL_PIC_NODE)
	{
		hRoot = m_hStyleWLPicRoot;
		pResProp = new PICWLPROPERTIES();

		//
		hItem = m_treeWindow.InsertItem(strResID, hRoot);

		// Set Item data
		m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);
	}
	else
	{
		delete pData;
		ASSERT(FALSE);
	}

	m_treeWindow.EnsureVisible(hItem);
	m_treeWindow.SelectItem(hItem);

	pResProp->m_strID = strResID;

	pData->pResourceOrStyle = pResProp;

	// Update
	m_treeWindow.Invalidate();

	pDoc->SetModifiedFlag(TRUE);

	return 0;
}

#define BEGIN_NEW_STYLE(eTreeItemTypeValue, hStyleItemRoot, StyleClassName, StyleType) \
	if (eTreeItemType == eTreeItemTypeValue) \
	{ \
		hRoot = hStyleItemRoot; \
		pStyleProp = new StyleClassName(); \
		pStyleProp->m_eControlType = StyleType; \
	}
#define NEW_STYLE(eTreeItemTypeValue, hStyleItemRoot, StyleClassName, StyleType) \
	else if (eTreeItemType == eTreeItemTypeValue) \
	{ \
		hRoot = hStyleItemRoot; \
		pStyleProp = new StyleClassName(); \
		pStyleProp->m_eControlType = StyleType; \
	}
#define END_NEW_STYLE() \
	else \
	{ \
		ASSERT(FALSE); \
	}

#define BEGIN_ADD_STYLE(eTreeItemTypeValue, StyleType) \
	if (eTreeItemType == eTreeItemTypeValue) \
	{ \
		CUIMgr::AddStyleItem(StyleType, pStyleProp); \
	}
#define ADD_STYLE(eTreeItemTypeValue, StyleType) \
	else if (eTreeItemType == eTreeItemTypeValue) \
	{ \
		CUIMgr::AddStyleItem(StyleType, pStyleProp); \
	}
#define END_ADD_STYLE() \
	else \
	{ \
		ASSERT(FALSE); \
	}

int CFileView::InsertStyle(TREE_ITEM_TYPE eTreeItemType)
{
	CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
	ASSERT_VALID(pDoc);

	HTREEITEM hRoot = NULL;
	CTRLPROPERTIES *pStyleProp = NULL;

	BEGIN_NEW_STYLE(STYLE_PUSHBUTTON_NODE, m_hStylePushButtonRoot, BTNPROPERTIES, CT_STYLE_PUSHBUTTON)
	NEW_STYLE(STYLE_CHECK_NODE, m_hStyleCheckRoot, CHKPROPERTIES, CT_STYLE_CHECK)
	NEW_STYLE(STYLE_RADIO_NODE, m_hStyleRadioRoot, RADPROPERTIES, CT_STYLE_RADIO)
	NEW_STYLE(STYLE_COMBOBOX_NODE, m_hStyleComboBoxRoot, CMBPROPERTIES, CT_STYLE_COMBOBOX)
	NEW_STYLE(STYLE_STATIC_NODE, m_hStyleStaticRoot, TXTPROPERTIES, CT_STYLE_STATIC)
	NEW_STYLE(STYLE_EDIT_NODE, m_hStyleEditRoot, EDTPROPERTIES, CT_STYLE_EDIT)
	NEW_STYLE(STYLE_SLIDER_NODE, m_hStyleSliderRoot, SLDPROPERTIES, CT_STYLE_SLIDER)
	NEW_STYLE(STYLE_PROGRESS_NODE, m_hStyleProgressRoot, PRGPROPERTIES, CT_STYLE_PROGRESS)
	NEW_STYLE(STYLE_SPIN_NODE, m_hStyleSpinRoot, SPNPROPERTIES, CT_STYLE_SPIN)
	NEW_STYLE(STYLE_HOTKEY_NODE, m_hStyleHotKeyRoot, HOTPROPERTIES, CT_STYLE_HOTKEY)
	NEW_STYLE(STYLE_IPADDRESS_NODE, m_hStyleIPAddressRoot, IPAPROPERTIES, CT_STYLE_IPADDRESS)
	NEW_STYLE(STYLE_SLIDEREX_NODE, m_hStyleSliderExRoot, SLDEXPROPERTIES, CT_STYLE_SLIDEREX)
	NEW_STYLE(STYLE_LISTCTRL_NODE, m_hStyleListCtrlRoot, LSTCTRLPROPERTIES, CT_STYLE_LISTCTRL)
	NEW_STYLE(STYLE_TREECTRL_NODE, m_hStyleTreeCtrlRoot, TREECTRLPROPERTIES, CT_STYLE_TREECTRL)
	NEW_STYLE(STYLE_TREELIST_NODE, m_hStyleTreeListRoot, TREELISTPROPERTIES, CT_STYLE_TREELIST)
	NEW_STYLE(STYLE_RICHEDIT_NODE, m_hStyleRichEditRoot, RICHEDITPROPERTIES, CT_STYLE_RICHEDIT)
	NEW_STYLE(STYLE_IMRICHEDIT_NODE, m_hStyleIMRichEditRoot, IMREPROPERTIES, CT_STYLE_IMRICHEDIT)
	NEW_STYLE(STYLE_HTMLCTRL_NODE, m_hStyleHtmlCtrlRoot, HTMLCTRLPROPERTIES, CT_STYLE_HTMLCTRL)
	NEW_STYLE(STYLE_DATETIMECTRL_NODE, m_hStyleDateTimeCtrlRoot, DATETIMECTRLPROPERTIES, CT_STYLE_DATETIMECTRL)
	NEW_STYLE(STYLE_WAVECTRL_NODE, m_hStyleWaveCtrlRoot, WAVECTRLPROPERTIES, CT_STYLE_WAVECTRL)
	NEW_STYLE(STYLE_MENU_NODE, m_hStyleMenuRoot, MENUPROPERTIES, CT_STYLE_MENU)
	NEW_STYLE(STYLE_MENUBAR_NODE, m_hStyleMenuBarRoot, MENUBARPROPERTIES, CT_STYLE_MENUBAR)
	NEW_STYLE(STYLE_HEADERCTRL_NODE, m_hStyleHeaderRoot, HEADERPROPERTIES, CT_STYLE_HEADERCTRL)
	NEW_STYLE(STYLE_HORSCROLLBAR_NODE, m_hStyleHorScrollbarRoot, HORSBPROPERTIES, CT_STYLE_HORSCROLLBAR)
	NEW_STYLE(STYLE_VERSCROLLBAR_NODE, m_hStyleVerScrollbarRoot, VERSBPROPERTIES, CT_STYLE_VERSCROLLBAR)
	NEW_STYLE(STYLE_SPLITTER_NODE, m_hStyleSplitterRoot, SPLITTERPROPERTIES, CT_STYLE_SPLITTER)
	NEW_STYLE(STYLE_WL_LINE_NODE, m_hStyleWLLineRoot, LINEWLPROPERTIES, CT_STYLE_WL_LINE)
	NEW_STYLE(STYLE_WL_PIC_NODE, m_hStyleWLPicRoot, PICWLPROPERTIES, CT_STYLE_WL_PICTURE)
	END_NEW_STYLE()

	// Assign new resource ID
	CString strResID = GetNextStyleID(eTreeItemType);

	HTREEITEM hItem = m_treeWindow.InsertItem(strResID, hRoot);
	m_treeWindow.EnsureVisible(hItem);
	m_treeWindow.SelectItem(hItem);

	// Set item data to tree item
	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = eTreeItemType;
	m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);

	pStyleProp->m_strID = strResID;

	BEGIN_ADD_STYLE(STYLE_PUSHBUTTON_NODE, STYLET_PUSHBUTTON)
	ADD_STYLE(STYLE_CHECK_NODE, STYLET_CHECK)
	ADD_STYLE(STYLE_RADIO_NODE, STYLET_RADIO)
	ADD_STYLE(STYLE_COMBOBOX_NODE, STYLET_COMBOBOX)
	ADD_STYLE(STYLE_STATIC_NODE, STYLET_STATIC)
	ADD_STYLE(STYLE_EDIT_NODE, STYLET_EDIT)
	ADD_STYLE(STYLE_SLIDER_NODE, STYLET_SLIDER)
	ADD_STYLE(STYLE_PROGRESS_NODE, STYLET_PROGRESS)
	ADD_STYLE(STYLE_SPIN_NODE, STYLET_SPIN)
	ADD_STYLE(STYLE_HOTKEY_NODE, STYLET_HOTKEY)
	ADD_STYLE(STYLE_IPADDRESS_NODE, STYLET_IPADDRESS)
	ADD_STYLE(STYLE_SLIDEREX_NODE, STYLET_SLIDEREX)
	ADD_STYLE(STYLE_LISTCTRL_NODE, STYLET_LISTCTRL)
	ADD_STYLE(STYLE_TREECTRL_NODE, STYLET_TREECTRL)
	ADD_STYLE(STYLE_TREELIST_NODE, STYLET_TREELIST)
	ADD_STYLE(STYLE_RICHEDIT_NODE, STYLET_RICHEDIT)
	ADD_STYLE(STYLE_IMRICHEDIT_NODE, STYLET_IMRICHEDIT)
	ADD_STYLE(STYLE_HTMLCTRL_NODE, STYLET_HTMLCTRL)
	ADD_STYLE(STYLE_DATETIMECTRL_NODE, STYLET_DATETIMECTRL)
	ADD_STYLE(STYLE_WAVECTRL_NODE, STYLET_WAVECTRL)
	ADD_STYLE(STYLE_MENU_NODE, STYLET_MENU)
	ADD_STYLE(STYLE_MENUBAR_NODE, STYLET_MENUBAR)
	ADD_STYLE(STYLE_HEADERCTRL_NODE, STYLET_HEADERCTRL)
	ADD_STYLE(STYLE_HORSCROLLBAR_NODE, STYLET_HORSCROLLBAR)
	ADD_STYLE(STYLE_VERSCROLLBAR_NODE, STYLET_VERSCROLLBAR)
	ADD_STYLE(STYLE_SPLITTER_NODE, STYLET_SPLITTER)
	END_ADD_STYLE()

	pData->pResourceOrStyle = pStyleProp;

	// Update
	m_treeWindow.Invalidate();

	pDoc->SetModifiedFlag(TRUE);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	if (!m_treeWindow.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_treeWindow.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	AdjustLayout();

	return 0;
}

// Release hItem and its sibling, is hItem is NULL, release all item of the tree.
int ReleaseTree(CTreeCtrl *pTree, HTREEITEM hItem)
{
	if (pTree == NULL)
	{
		return -1;
	}

	if (hItem == NULL)
	{
		hItem = pTree->GetRootItem();
	}


	while (hItem != NULL)
	{
		// Release itself
		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)pTree->GetItemData(hItem);
		SafeDelete(pData);
		pTree->SetItemData(hItem, 0);

		// Release its children
		HTREEITEM hChild = pTree->GetChildItem(hItem);
		if (hChild != NULL)
		{
			ReleaseTree(pTree, hChild);
		}

		// Release its sibling
		hItem = pTree->GetNextSiblingItem(hItem);
	}

	return 0;
}

void CFileView::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: Add your message handler code here

	// Release tree
	//    ReleaseTree(&m_treeWindow, NULL);
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_treeWindow.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd *pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_treeWindow.SetFocus();
}

void CFileView::OnContextMenu(CWnd *pWnd, CPoint point)
{
	CUIShopDoc *pDoc = g_pFrm->GetUIShopDoc();
	if (pDoc == NULL)
	{
		return;
	}

	CTreeCtrl *pWndTree = (CTreeCtrl *) &m_treeWindow;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();

	HTREEITEM hSel = pWndTree->GetSelectedItem();
	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)pWndTree->GetItemData(hSel);
	if (pData->eTreeItemType >= WINDOWS_ROOT && pData->eTreeItemType <= WINDOWS_NODE)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
	}
	else if (pData->eTreeItemType >= RESOURCE_ROOT && pData->eTreeItemType <= RES_COLOR_NODE)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_RESOURCE, point.x, point.y, this, TRUE);
	}
	else if (pData->eTreeItemType >= STYLE_ROOT && pData->eTreeItemType <= STYLE_SPLITTER_NODE)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_STYLE, point.x, point.y, this, TRUE);
	}
}

void CFileView::OnInsertWindow()
{
	CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
	ASSERT_VALID(pDoc);

	// Assign new window ID
	CString strWindowID = CIDMgr::NextWindowID();
	UIWNDPROPERTIES *pUIProp = CUIMgr::GetUIWNDPROPERTIES(strWindowID);

	// 循环遍历，如果这个ID已被占用，就重新分配ID，直到没被占用。
	while (pUIProp != NULL)
	{
		strWindowID = CIDMgr::NextWindowID();
		pUIProp = CUIMgr::GetUIWNDPROPERTIES(strWindowID);
	}

	HTREEITEM hItem = m_treeWindow.InsertItem(strWindowID, m_hWindowsRoot);
	m_treeWindow.EnsureVisible(hItem);

	// set item data to tree item
	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = WINDOWS_NODE;
	m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);

	pUIProp = CUIMgr::AllocUIWNDPROPERTIES();
	pUIProp->m_strID = strWindowID;
	pUIProp->m_bLoaded = true;
	CUIMgr::AddWindowItem(pUIProp);

	//
	UpdateUIWindowsCount();

	// create the CUIDesignWnd
	pData->wndUIDesign.SetUIWndProperties(pUIProp);

	// init the CUIDesignWnd
	ASSERT(pData->wndUIDesign.GetSafeHwnd() == NULL);
	CUIShopView *pUIShopView = g_pFrm->GetUIShopView();
	BOOL bRet = pData->wndUIDesign.Create(NULL, strWindowID, WS_CHILD, CRect(0, 0, VIEW_WIDTH_MAX, VIEW_HEIGHT_MAX), pUIShopView, 1111);
	if (!bRet)
	{
		ASSERT(FALSE);
	}

	pDoc->SetModifiedFlag(TRUE);
}

void CFileView::OnWindowProperties()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CUIShopDoc *pDoc = (CUIShopDoc *)pFrm->GetUIShopDoc();
	if (pDoc != NULL)
	{
		pDoc->UpdateAllViews(NULL);
	}
}

void CFileView::OnTestWindow()
{
	// TODO: Add your command handler code here
	CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
	ASSERT_VALID(pDoc);

	CUIMgr::EnableDPI(FALSE);

	if (pDoc->IsModified())
	{
		CString strPathName = pDoc->GetPathName();
		if (strPathName.IsEmpty())
		{
			int nRet = AfxMessageBox(_T("The project file has not created, must create it for test the window, create it now?"), MB_YESNO);
			if (nRet == IDYES)
			{
				pDoc->CallOnFileSave();
				strPathName = pDoc->GetPathName();
			}
			else
			{
				return;
			}
		}
		else
		{
			pDoc->CallOnFileSave();
		}
	}

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = "TestWindow";
	wndcls.lpszClassName = (LPCTSTR) strMyClass;

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("Class Registration Failed\n"));
		return;
	}

	DWORD dwStyle = WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX;

	HTREEITEM hTI = m_treeWindow.GetSelectedItem();
	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hTI);
	ASSERT(pData != NULL && pData->eTreeItemType == WINDOWS_NODE);

	if (pData->wndTest.GetSafeHwnd() != NULL)
	{
		pData->wndTest.SendMessage(WM_CLOSE);
	}

	UIWNDPROPERTIES *pUIProp = pData->wndUIDesign.GetUIWndProperties();
	if (!pUIProp->m_bLoaded)
	{
		CUIMgr::DelayLoadWindow(pUIProp);
	}
	pData->wndTest.SetUIWndProperties(pUIProp);
	if (BOOL bRet = pData->wndTest.LoadFrame(IDR_MAINFRAME, dwStyle, NULL))
	{
		pData->wndTest.ShowWindow(SW_SHOW);
		pData->wndTest.SetWindowText(pUIProp->m_strID);

		if (pData->wndTest.SendMessage(WM_ISUPDATELAYEREDWINDOW))
		{
			pData->wndTest.UpdateUIWindow();
		}
	}

	DWORD dwLastError = pData->wndTest.GetLastCreateError();
	CString strMsg;
	switch (dwLastError)
	{
	case E_LOADBMPB:
		strMsg = "failed to load the base background image";
		AfxMessageBox(strMsg);
		break;
	case E_LOADBMPN:
		strMsg = "failed to load the background image of normal status";
		AfxMessageBox(strMsg);
		break;
	case E_ATTACHBMPB:
		strMsg = "failed to attach the HBITMAP object of base background image to the CBitmap object";
		AfxMessageBox(strMsg);
		break;
	case E_ATTACHBMPN:
		strMsg = "failed to attach the HBITMAP object of background image of normal status to the CBitmap object";
		AfxMessageBox(strMsg);
		break;
	case E_CREATECHILD:
		strMsg = "failed to create all of the child window";
		AfxMessageBox(strMsg);
		break;
	default:
		break;
	}
}

void CFileView::OnTestHighDpi()
{
	// TODO: Add your command handler code here
	CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
	ASSERT_VALID(pDoc);

	if (pDoc->IsModified())
	{
		CString strPathName = pDoc->GetPathName();
		if (strPathName.IsEmpty())
		{
			int nRet = AfxMessageBox(_T("The project file has not created, must create it for test the window, create it now?"), MB_YESNO);
			if (nRet == IDYES)
			{
				pDoc->CallOnFileSave();
				strPathName = pDoc->GetPathName();
			}
			else
			{
				return;
			}
		}
		else
		{
			pDoc->CallOnFileSave();
		}
	}

	CUIMgr::EnableDPI(TRUE);
	CUIMgr::RunAsDPIX(143);
	CUIMgr::RunAsDPIY(143);

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = "TestWindow";
	wndcls.lpszClassName = (LPCTSTR) strMyClass;

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("Class Registration Failed\n"));
		return;
	}

	DWORD dwStyle = WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX;

	HTREEITEM hTI = m_treeWindow.GetSelectedItem();
	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hTI);
	ASSERT(pData != NULL && pData->eTreeItemType == WINDOWS_NODE);

	if (pData->wndTest.GetSafeHwnd() != NULL)
	{
		pData->wndTest.SendMessage(WM_CLOSE);
	}

	UIWNDPROPERTIES *pUIProp = pData->wndUIDesign.GetUIWndProperties();
	if (!pUIProp->m_bLoaded)
	{
		CUIMgr::DelayLoadWindow(pUIProp);
	}
	pData->wndTest.SetUIWndProperties(pUIProp);
	if (BOOL bRet = pData->wndTest.LoadFrame(IDR_MAINFRAME, dwStyle, NULL))
	{
		pData->wndTest.ShowWindow(SW_SHOW);
		pData->wndTest.SetWindowText(pUIProp->m_strID);

		if (pData->wndTest.SendMessage(WM_ISUPDATELAYEREDWINDOW))
		{
			pData->wndTest.UpdateUIWindow();
		}
	}

	DWORD dwLastError = pData->wndTest.GetLastCreateError();
	CString strMsg;
	switch (dwLastError)
	{
	case E_LOADBMPB:
		strMsg = "failed to load the base background image";
		AfxMessageBox(strMsg);
		break;
	case E_LOADBMPN:
		strMsg = "failed to load the background image of normal status";
		AfxMessageBox(strMsg);
		break;
	case E_ATTACHBMPB:
		strMsg = "failed to attach the HBITMAP object of base background image to the CBitmap object";
		AfxMessageBox(strMsg);
		break;
	case E_ATTACHBMPN:
		strMsg = "failed to attach the HBITMAP object of background image of normal status to the CBitmap object";
		AfxMessageBox(strMsg);
		break;
	case E_CREATECHILD:
		strMsg = "failed to create all of the child window";
		AfxMessageBox(strMsg);
		break;
	default:
		break;
	}
}

void CFileView::OnTestCurFrame()
{
	// TODO: Add your command handler code here
	CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
	ASSERT_VALID(pDoc);

	if (pDoc->IsModified())
	{
		CString strPathName = pDoc->GetPathName();
		if (strPathName.IsEmpty())
		{
			int nRet = AfxMessageBox(_T("The project file has not created, must create it for test the window, create it now?"), MB_YESNO);
			if (nRet == IDYES)
			{
				pDoc->CallOnFileSave();
				strPathName = pDoc->GetPathName();
			}
			else
			{
				return;
			}
		}
		else
		{
			pDoc->CallOnFileSave();
		}
	}

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = "TestWindow";
	wndcls.lpszClassName = (LPCTSTR) strMyClass;

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("Class Registration Failed\n"));
		return;
	}

	DWORD dwStyle = WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX;

	HTREEITEM hTI = m_treeWindow.GetSelectedItem();
	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hTI);
	ASSERT(pData != NULL && pData->eTreeItemType == WINDOWS_NODE);

	if (pData->wndTest.GetSafeHwnd() != NULL)
	{
		pData->wndTest.SendMessage(WM_CLOSE);
	}

	UIWNDPROPERTIES *pUIProp = pData->wndUIDesign.GetUIWndProperties();
	if (!pUIProp->m_bLoaded)
	{
		CUIMgr::DelayLoadWindow(pUIProp);
	}
	pData->wndTest.SetUIWndProperties(pUIProp);
	pData->wndTest.DisableStoryboard();
	if (BOOL bRet = pData->wndTest.LoadFrame(IDR_MAINFRAME, dwStyle, NULL))
	{
		pData->wndTest.ShowWindow(SW_SHOW);
		pData->wndTest.SetWindowText(pUIProp->m_strID);
	}

	DWORD dwLastError = pData->wndTest.GetLastCreateError();
	CString strMsg;
	switch (dwLastError)
	{
	case E_LOADBMPB:
		strMsg = "failed to load the base background image";
		AfxMessageBox(strMsg);
		break;
	case E_LOADBMPN:
		strMsg = "failed to load the background image of normal status";
		AfxMessageBox(strMsg);
		break;
	case E_ATTACHBMPB:
		strMsg = "failed to attach the HBITMAP object of base background image to the CBitmap object";
		AfxMessageBox(strMsg);
		break;
	case E_ATTACHBMPN:
		strMsg = "failed to attach the HBITMAP object of background image of normal status to the CBitmap object";
		AfxMessageBox(strMsg);
		break;
	case E_CREATECHILD:
		strMsg = "failed to create all of the child window";
		AfxMessageBox(strMsg);
		break;
	default:
		break;
	}

#ifdef ADV
	int nCurFrame = g_pFrm->GetStoryboardPanel()->GetCurFrame();
	pData->wndTest.SeekToFrame(nCurFrame);
#endif // ADV
}

void CFileView::OnDeleteWindow()
{
	int nRet = AfxMessageBox(_T("Do you want to delete the window?"), MB_YESNO);
	if (nRet == IDYES)
	{
		HTREEITEM hTI = m_treeWindow.GetSelectedItem();
		CString strWindowID = m_treeWindow.GetItemText(hTI);
		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hTI);
		if (pData->wndTest.GetSafeHwnd() != NULL)
		{
			pData->wndTest.SendMessage(WM_CLOSE);
		}

		if (m_treeWindow.DeleteItem(hTI))
		{
			// Update windows count
			UpdateUIWindowsCount();

			//
			CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
			if (pDoc != NULL)
			{
				pDoc->SetModifiedFlag(TRUE);
			}
		}
	}
}

void CFileView::OnTabOrder()
{
	// TODO: Add your command handler code here
	HTREEITEM hTI = m_treeWindow.GetSelectedItem();
	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hTI);
	ASSERT(pData != NULL && pData->eTreeItemType == WINDOWS_NODE);

	CTabOrderDlg dlg;
	dlg.SetBuddy(&(pData->wndUIDesign));
	if (dlg.DoModal() == IDOK)
	{
	}
}

void CFileView::OnFindInHD()
{
	// TODO: Add your command handler code here
	TCHAR szDisc[MAX_PATH] = {0};
	TCHAR szFolder[MAX_PATH] = {0};
	CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
	ASSERT_VALID(pDoc);
	_tsplitpath_s(pDoc->GetPathName(), szDisc, MAX_PATH, szFolder, MAX_PATH, NULL, 0, NULL, 0);
	_tcscat_s(szDisc, MAX_PATH, szFolder);

	LPCWSTR pFile = CUIMgr::GetUIPath();
	LPCWSTR *p = &pFile;
	OpenFolderAndSelectItems(szDisc, p, 1);
}

void CFileView::OnUpdateDeletewindow(CCmdUI *pCmdUI)
{
	if (m_treeWindow.GetSelectedItem() == m_hWindowsRoot)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnUpdateTaborder(CCmdUI *pCmdUI)
{
	if (m_treeWindow.GetSelectedItem() == m_hWindowsRoot)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnUpdateTest(CCmdUI *pCmdUI)
{
	if (m_treeWindow.GetSelectedItem() == m_hWindowsRoot)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnUpdateTestHighDpi(CCmdUI *pCmdUI)
{
	if (m_treeWindow.GetSelectedItem() == m_hWindowsRoot)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnUpdateTestCurFrame(CCmdUI *pCmdUI)
{
	if (m_treeWindow.GetSelectedItem() == m_hWindowsRoot)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnUpdateProperties(CCmdUI *pCmdUI)
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		pCmdUI->Enable(FALSE);
	}

	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hSelItem);
	if (pData->eTreeItemType == WINDOWS_ROOT
		|| pData->eTreeItemType == RESOURCE_ROOT || pData->eTreeItemType == RES_COLOR_BRANCH
		|| pData->eTreeItemType == STYLE_ROOT || pData->eTreeItemType == STYLE_HORSCROLLBAR_ROOT || pData->eTreeItemType == STYLE_VERSCROLLBAR_ROOT)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnAddClass()
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		ASSERT(FALSE);
		return;
	}
	CString strWindowID = m_treeWindow.GetItemText(hSelItem);
	ASSERT(!strWindowID.IsEmpty());

	CClassWizardDlg dlg;
	dlg.m_strWindowID = strWindowID;
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
	{
		return;
	}

	// Macro Define
	CClassGen classGen;
	classGen.SetInputFolder(dlg.m_strLocation);
	classGen.SetClassName(dlg.m_strClassName);
	UIWNDPROPERTIES *pWndProperties = CUIMgr::GetUIWNDPROPERTIES(strWindowID);
	ASSERT(pWndProperties != NULL);
	classGen.SetUIWndProperties(pWndProperties);

	classGen.m_Dictionary.RemoveAll();

	// Class Name
	CString strLowerClassName = dlg.m_strClassName;
	strLowerClassName.MakeLower();
	classGen.m_Dictionary[_T("class")] = strLowerClassName;

	classGen.m_Dictionary[_T("Class")] = dlg.m_strClassName;

	CString strUpperClassName = dlg.m_strClassName;
	strUpperClassName.MakeUpper();
	classGen.m_Dictionary[_T("CLASS")] = strUpperClassName;

	// Window ID
	classGen.m_Dictionary[_T("WindowID")] = dlg.m_strWindowID;

	// .h File
	classGen.m_Dictionary[_T("HFile")] = dlg.m_strHFile;

	// .cpp File
	classGen.m_Dictionary[_T("CppFile")] = dlg.m_strCppFile;

	// Generate event handler
	if (dlg.m_bGenEventHandler)
	{
		classGen.m_Dictionary[_T("GEN_HANDLER")] = _T("1");
	}

	// Generate control variable
	if (dlg.m_bGenVariable)
	{
		classGen.m_Dictionary[_T("GEN_VARIABLE")] = _T("1");
	}

	// Generate DoDataExchange
	if (dlg.m_bGenDoDataExchange)
	{
		classGen.m_Dictionary[_T("GEN_DO_DATA_EXCHANGE")] = _T("1");
	}

	//
	void *pData = NULL;
	DWORD dwSize = -1;
	nRet = classGen.LoadCustomResource("NewClass.inf", &pData, &dwSize);
	if (nRet != 0)
	{
		AfxMessageBox(_T("Can't load \"NewClass.inf\" file!"));
		return;
	}

	nRet = classGen.LoadTemplate(pData, dwSize);
	if (nRet != 0)
	{
		AfxMessageBox(_T("Can't parse \"NewClass.inf\" file!"));
		return;
	}

	nRet = classGen.ProcessTemplate();
	if (nRet != 0)
	{
		AfxMessageBox(_T("Failed to new class!"));
		return;
	}
}

void CFileView::OnUpdateAddClass(CCmdUI *pCmdUI)
{
	if (m_treeWindow.GetSelectedItem() == m_hWindowsRoot)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnAddAllClasses()
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		ASSERT(FALSE);
		return;
	}
	CString strWindowID = m_treeWindow.GetItemText(hSelItem);
	ASSERT(!strWindowID.IsEmpty());

	CAddAllClassDlg dlg;
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
	{
		return;
	}

	CString strFile = CUIMgr::GetUIPathWithoutTitle() + _T("Settings.ini");
	const std::vector<UIWNDPROPERTIES *> *pvWindows = CUIMgr::GetUIWndPropertiesList();
	size_t nSize = pvWindows->size();
	for (size_t i = 0; i < nSize; ++i)
	{
		UIWNDPROPERTIES *pUIWndProp = (*pvWindows)[i];
		ASSERT(pUIWndProp != NULL);

		// Window ID
		LPCTSTR lpszWindowID = pUIWndProp->m_strID;

		CClassGen classGen;
		//
		classGen.m_Dictionary.RemoveAll();

		// Class name for this window ID.
		TCHAR szClassName[MAX_PATH] = {0};
		GetPrivateProfileString(lpszWindowID, g_lpszClassName, _T(""), szClassName, MAX_PATH, strFile);
		if (_tcslen(szClassName) <= 0)
		{
			continue;
		}

		// .h File
		TCHAR szClassHFile[MAX_PATH] = {0};
		GetPrivateProfileString(lpszWindowID, g_lpszClassHFile, _T(""), szClassHFile, MAX_PATH, strFile);
		if (_tcslen(szClassHFile) <= 0)
		{
			continue;
		}
		classGen.m_Dictionary[_T("HFile")] = szClassHFile;

		// .Cpp file
		TCHAR szClassCppFile[MAX_PATH] = {0};
		GetPrivateProfileString(lpszWindowID, g_lpszClassCppFile, _T(""), szClassCppFile, MAX_PATH, strFile);
		if (_tcslen(szClassCppFile) <= 0)
		{
			continue;
		}
		classGen.m_Dictionary[_T("CppFile")] = szClassCppFile;

		classGen.SetClassName(szClassName);

		// Macro Define
		classGen.SetInputFolder(dlg.m_strLocation);

		//
		classGen.SetUIWndProperties(pUIWndProp);

		// Class Name
		CString strLowerClassName = szClassName;
		strLowerClassName.MakeLower();
		classGen.m_Dictionary[_T("class")] = strLowerClassName;

		classGen.m_Dictionary[_T("Class")] = szClassName;

		CString strUpperClassName = szClassName;
		strUpperClassName.MakeUpper();
		classGen.m_Dictionary[_T("CLASS")] = strUpperClassName;

		// Window ID
		classGen.m_Dictionary[_T("WindowID")] = lpszWindowID;

		// Code Style
		BOOL bCodeStyleVC6 = GetPrivateProfileInt(lpszWindowID, g_lpszCodeStyleVC6, 0, strFile);
		if (bCodeStyleVC6)
		{
			classGen.m_Dictionary[_T("CODESTYLE_VC60")] = _T("1");
		}
		else
		{
			classGen.m_Dictionary[_T("CODESTYLE_VCNET")] = _T("1");
		}

		// Generate event handler
		classGen.m_Dictionary[_T("GEN_HANDLER")] = _T("1");

		// Generate control variable
		classGen.m_Dictionary[_T("GEN_VARIABLE")] = _T("1");

		// Generate DoDataExchange
		classGen.m_Dictionary[_T("GEN_DO_DATA_EXCHANGE")] = _T("1");

		//
		void *pData = NULL;
		DWORD dwSize = -1;
		nRet = classGen.LoadCustomResource("NewClass.inf", &pData, &dwSize);
		if (nRet != 0)
		{
			AfxMessageBox(_T("Can't load \"NewClass.inf\" file!"));
			return;
		}

		nRet = classGen.LoadTemplate(pData, dwSize);
		if (nRet != 0)
		{
			AfxMessageBox(_T("Can't parse \"NewClass.inf\" file!"));
			return;
		}

		nRet = classGen.ProcessTemplate();
		if (nRet != 0)
		{
			AfxMessageBox(_T("Failed to new class!"));
			return;
		}
	}
}

void CFileView::OnUpdateAddAllClasses(CCmdUI *pCmdUI)
{
	if (m_treeWindow.GetSelectedItem() == m_hWindowsRoot)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

// Generate default class name by window ID.
CString GenerateClassNameByWindowID(LPCTSTR lpszWinID)
{
	CString strName = lpszWinID;

	BOOL bDialog = FALSE;        // window ID has prefix IDD_

	// Remove IDW_ or IDD_ prefix
	CString strIDWPrefix = _T("IDW_");
	CString strIDDPrefix = _T("IDD_");
	if (strName.Left(strIDWPrefix.GetLength()) == strIDWPrefix)
	{
		strName.Delete(0, strIDWPrefix.GetLength());
	}
	else if (strName.Left(strIDDPrefix.GetLength()) == strIDDPrefix)
	{
		bDialog = TRUE;
		strName.Delete(0, strIDDPrefix.GetLength());
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

	CString strClassName = _T("C");
	strClassName += strName;

	if (bDialog)
	{
		strClassName += _T("DlgHandler");
	}
	else
	{
		strClassName += _T("WndHandler");
	}

	return strClassName;
}

void CFileView::OnAddEventHandlerClass()
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		ASSERT(FALSE);
		return;
	}
	CString strWindowID = m_treeWindow.GetItemText(hSelItem);
	ASSERT(!strWindowID.IsEmpty());

	CEventHandlerWizardDlg dlg;

	dlg.m_strClassName = GenerateClassNameByWindowID(strWindowID);
	dlg.m_strWindowID = strWindowID;
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
	{
		return;
	}

	// Macro Define
	CEventHandlerGen eventHandlerGen;
	eventHandlerGen.SetInputFolder(dlg.m_strLocation);
	eventHandlerGen.SetClassName(dlg.m_strClassName);
	UIWNDPROPERTIES *pWndProperties = CUIMgr::GetUIWNDPROPERTIES(strWindowID);
	ASSERT(pWndProperties != NULL);
	eventHandlerGen.SetUIWndProperties(pWndProperties);

	eventHandlerGen.m_Dictionary.RemoveAll();

	// Class Name
	CString strLowerClassName = dlg.m_strClassName;
	strLowerClassName.MakeLower();
	eventHandlerGen.m_Dictionary[_T("class")] = strLowerClassName;

	eventHandlerGen.m_Dictionary[_T("Class")] = dlg.m_strClassName;

	CString strUpperClassName = dlg.m_strClassName;
	strUpperClassName.MakeUpper();
	eventHandlerGen.m_Dictionary[_T("CLASS")] = strUpperClassName;

	// Window ID
	eventHandlerGen.m_Dictionary[_T("WindowID")] = dlg.m_strWindowID;

	// .h File
	eventHandlerGen.m_Dictionary[_T("HFile")] = dlg.m_strHFile;

	// .cpp File
	eventHandlerGen.m_Dictionary[_T("CppFile")] = dlg.m_strCppFile;

	eventHandlerGen.GenerateEventHandlerFile();
}

void CFileView::OnUpdateAddEventHandlerClass(CCmdUI *pCmdUI)
{
	if (m_treeWindow.GetSelectedItem() == m_hWindowsRoot)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnAddAllEventHandlerClasses()
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		ASSERT(FALSE);
		return;
	}
	CString strWindowID = m_treeWindow.GetItemText(hSelItem);
	ASSERT(!strWindowID.IsEmpty());

	CAddAllClassDlg dlg;
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
	{
		return;
	}

	CString strFile = CUIMgr::GetUIPathWithoutTitle() + _T("Settings.ini");
	const std::vector<UIWNDPROPERTIES *> *pvWindows = CUIMgr::GetUIWndPropertiesList();
	size_t nSize = pvWindows->size();
	for (size_t i = 0; i < nSize; ++i)
	{
		UIWNDPROPERTIES *pUIWndProp = (*pvWindows)[i];
		ASSERT(pUIWndProp != NULL);

		// Window ID
		LPCTSTR lpszWindowID = pUIWndProp->m_strID;

		CEventHandlerGen eventHandlerGen;
		//
		eventHandlerGen.m_Dictionary.RemoveAll();

		// Class name for this window ID.
		TCHAR szClassName[MAX_PATH] = {0};
		GetPrivateProfileString(lpszWindowID,
			g_lpszClassName,
			_T(""),
			szClassName,
			MAX_PATH,
			strFile);

		if (_tcslen(szClassName) <= 0)
		{
			StringCchCopy(szClassName,
				MAX_PATH,
				(LPCTSTR)_bstr_t((LPCTSTR)GenerateClassNameByWindowID(lpszWindowID)));
		}

		// .h File
		TCHAR szClassHFile[MAX_PATH] = {0};
		GetPrivateProfileString(lpszWindowID, g_lpszClassHFile, _T(""), szClassHFile, MAX_PATH, strFile);
		if (_tcslen(szClassHFile) <= 0)
		{
			StringCchCopy(szClassHFile, MAX_PATH, GenerateHFile(szClassName));
		}
		eventHandlerGen.m_Dictionary[_T("HFile")] = szClassHFile;

		// .Cpp file
		TCHAR szClassCppFile[MAX_PATH] = {0};
		GetPrivateProfileString(lpszWindowID, g_lpszClassCppFile, _T(""), szClassCppFile, MAX_PATH, strFile);
		if (_tcslen(szClassCppFile) <= 0)
		{
			StringCchCopy(szClassCppFile, MAX_PATH, GenerateCppFile(szClassName));
		}
		eventHandlerGen.m_Dictionary[_T("CppFile")] = szClassCppFile;

		eventHandlerGen.SetClassName(szClassName);

		// Macro Define
		eventHandlerGen.SetInputFolder(dlg.m_strLocation);

		//
		eventHandlerGen.SetUIWndProperties(pUIWndProp);

		// Class Name
		CString strLowerClassName = szClassName;
		strLowerClassName.MakeLower();
		eventHandlerGen.m_Dictionary[_T("class")] = strLowerClassName;

		eventHandlerGen.m_Dictionary[_T("Class")] = szClassName;

		CString strUpperClassName = szClassName;
		strUpperClassName.MakeUpper();
		eventHandlerGen.m_Dictionary[_T("CLASS")] = strUpperClassName;

		// Window ID
		eventHandlerGen.m_Dictionary[_T("WindowID")] = lpszWindowID;

		// Code Style
		BOOL bCodeStyleVC6 = GetPrivateProfileInt(lpszWindowID, g_lpszCodeStyleVC6, 0, strFile);
		if (bCodeStyleVC6)
		{
			eventHandlerGen.m_Dictionary[_T("CODESTYLE_VC60")] = _T("1");
		}
		else
		{
			eventHandlerGen.m_Dictionary[_T("CODESTYLE_VCNET")] = _T("1");
		}

		eventHandlerGen.GenerateEventHandlerFile();
	}
}

void CFileView::OnUpdateAddAllEventHandlerClasses(CCmdUI *pCmdUI)
{
	if (m_treeWindow.GetSelectedItem() == m_hWindowsRoot)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CFileView::OnInsertGeneralFont()
{
	InsertResource(RES_GENERAL_FONT_NODE);
}

void CFileView::OnConvertToConditionFont()
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		return;
	}

	//
	// Make RESFONTPROPERTIES as a map element.
	//

	// Change the FONTRESOURCEITEM of old item Data
	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hSelItem);
	ASSERT(pData->eTreeItemType == RES_GENERAL_FONT_NODE);
	ASSERT(pData->pResFont != NULL);
	ASSERT(!pData->pResFont->m_bConditionFont);

	// Change item data type.
	pData->eTreeItemType = RES_CONDITION_FONT_NODE;

	// Change the RESFONTPROPERTIES.
	pData->pResFont->ConvertGeneralFontToConditionFont(_T("Condition1"));

	// Change tree item
	m_treeWindow.SetItemImage(hSelItem, 4, 4);

	//
	// Insert condition children font item.
	//
	InsertConditionFontChildren(hSelItem, pData->pResFont);

	m_treeWindow.Expand(hSelItem, TVE_EXPAND);
}

void CFileView::OnUpdateConvertToConditionFont(CCmdUI *pCmdUI)
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hSelItem);
	if (pData->eTreeItemType == RES_GENERAL_FONT_NODE)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CFileView::OnInsertConditionFontChild()
{
	HTREEITEM hParentItem = m_treeWindow.GetSelectedItem();
	if (hParentItem == NULL)
	{
		return;
	}

	TREE_ITEMDATA *pParentData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hParentItem);

	// Prepare data
	CString strID = CIDMgr::NextConditionFontChildID(pParentData->pResFont);

	RESFONTPROPERTIES *pFontProp = new RESFONTPROPERTIES;
	pFontProp->m_strID = strID;
	pFontProp->m_strID.strPropWndName = _T("Condition Name");
	pFontProp->m_strID.strDescription = _T("Input the condition name for the font to use.");

	TREE_ITEMDATA *pData = new TREE_ITEMDATA;
	pData->eTreeItemType = RES_CONDITION_CHILD_FONT_NODE;
	pData->pResourceOrStyle = pFontProp;

	// Insert to tree
	HTREEITEM hItem = m_treeWindow.InsertItem(strID, 3, 3, hParentItem);
	m_treeWindow.SetItemData(hItem, (DWORD_PTR)pData);

	// Insert to CUIMgr
	pParentData->pResFont->m_mapConditionFont[strID] = pFontProp;

	//
	m_treeWindow.EnsureVisible(hItem);
	m_treeWindow.SelectItem(hItem);

	// Update
	m_treeWindow.Invalidate();

	CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
	ASSERT_VALID(pDoc);
	pDoc->SetModifiedFlag(TRUE);
}

void CFileView::OnUpdateInsertConditionFont(CCmdUI *pCmdUI)
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hSelItem);
	if (pData->eTreeItemType == RES_CONDITION_FONT_NODE)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CFileView::OnInsertResColor()
{
	InsertResource(RES_COLOR_NODE);
}

void CFileView::OnDeleteResource()
{
	int nRet = AfxMessageBox(_T("Do you want to delete the Resource?"), MB_YESNO);
	if (nRet == IDYES)
	{
		HTREEITEM hTI = m_treeWindow.GetSelectedItem();
#ifdef _DEBUG
		CString strResourceID = m_treeWindow.GetItemText(hTI);
#endif
		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hTI);

		if (m_treeWindow.DeleteItem(hTI))
		{
			CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
			if (pDoc != NULL)
			{
				pDoc->SetModifiedFlag(TRUE);
			}
		}
	}
}

void CFileView::OnUpdateDeleteResource(CCmdUI *pCmdUI)
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == m_hResourceRoot || hSelItem == m_hResDefaultFontBranch || hSelItem == m_hResDefaultFont || hSelItem == m_hResFontBranch
		|| hSelItem == m_hResDefaultColorBranch || hSelItem == m_hResDefaultColor || hSelItem == m_hResColorBranch)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CFileView::OnInsertStyleItem()
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		return;
	}

	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hSelItem);

	if (pData->eTreeItemType == STYLE_PUSHBUTTON_ROOT || pData->eTreeItemType == STYLE_PUSHBUTTON_NODE)
	{
		InsertStyle(STYLE_PUSHBUTTON_NODE);
	}
	else if (pData->eTreeItemType == STYLE_CHECK_ROOT || pData->eTreeItemType == STYLE_CHECK_NODE)
	{
		InsertStyle(STYLE_CHECK_NODE);
	}
	else if (pData->eTreeItemType == STYLE_RADIO_ROOT || pData->eTreeItemType == STYLE_RADIO_NODE)
	{
		InsertStyle(STYLE_RADIO_NODE);
	}
	else if (pData->eTreeItemType == STYLE_COMBOBOX_ROOT || pData->eTreeItemType == STYLE_COMBOBOX_NODE)
	{
		InsertStyle(STYLE_COMBOBOX_NODE);
	}
	else if (pData->eTreeItemType == STYLE_STATIC_ROOT || pData->eTreeItemType == STYLE_STATIC_NODE)
	{
		InsertStyle(STYLE_STATIC_NODE);
	}
	else if (pData->eTreeItemType == STYLE_EDIT_ROOT || pData->eTreeItemType == STYLE_EDIT_NODE)
	{
		InsertStyle(STYLE_EDIT_NODE);
	}
	else if (pData->eTreeItemType == STYLE_SLIDER_ROOT || pData->eTreeItemType == STYLE_SLIDER_NODE)
	{
		InsertStyle(STYLE_SLIDER_NODE);
	}
	else if (pData->eTreeItemType == STYLE_PROGRESS_ROOT || pData->eTreeItemType == STYLE_PROGRESS_NODE)
	{
		InsertStyle(STYLE_PROGRESS_NODE);
	}
	else if (pData->eTreeItemType == STYLE_SPIN_ROOT || pData->eTreeItemType == STYLE_SPIN_NODE)
	{
		InsertStyle(STYLE_SPIN_NODE);
	}
	else if (pData->eTreeItemType == STYLE_HOTKEY_ROOT || pData->eTreeItemType == STYLE_HOTKEY_NODE)
	{
		InsertStyle(STYLE_HOTKEY_NODE);
	}
	else if (pData->eTreeItemType == STYLE_IPADDRESS_ROOT || pData->eTreeItemType == STYLE_IPADDRESS_NODE)
	{
		InsertStyle(STYLE_IPADDRESS_NODE);
	}
	else if (pData->eTreeItemType == STYLE_SLIDEREX_ROOT || pData->eTreeItemType == STYLE_SLIDEREX_NODE)
	{
		InsertStyle(STYLE_SLIDEREX_NODE);
	}
	else if (pData->eTreeItemType == STYLE_LISTCTRL_ROOT || pData->eTreeItemType == STYLE_LISTCTRL_NODE)
	{
		InsertStyle(STYLE_LISTCTRL_NODE);
	}
	else if (pData->eTreeItemType == STYLE_TREECTRL_ROOT || pData->eTreeItemType == STYLE_TREECTRL_NODE)
	{
		InsertStyle(STYLE_TREECTRL_NODE);
	}
	else if (pData->eTreeItemType == STYLE_TREELIST_ROOT || pData->eTreeItemType == STYLE_TREELIST_NODE)
	{
		InsertStyle(STYLE_TREELIST_NODE);
	}
	else if (pData->eTreeItemType == STYLE_RICHEDIT_ROOT || pData->eTreeItemType == STYLE_RICHEDIT_NODE)
	{
		InsertStyle(STYLE_RICHEDIT_NODE);
	}
	else if (pData->eTreeItemType == STYLE_IMRICHEDIT_ROOT || pData->eTreeItemType == STYLE_IMRICHEDIT_NODE)
	{
		InsertStyle(STYLE_IMRICHEDIT_NODE);
	}
	else if (pData->eTreeItemType == STYLE_HTMLCTRL_ROOT || pData->eTreeItemType == STYLE_HTMLCTRL_NODE)
	{
		InsertStyle(STYLE_HTMLCTRL_NODE);
	}
	else if (pData->eTreeItemType == STYLE_DATETIMECTRL_ROOT || pData->eTreeItemType == STYLE_DATETIMECTRL_NODE)
	{
		InsertStyle(STYLE_DATETIMECTRL_NODE);
	}
	else if (pData->eTreeItemType == STYLE_WAVECTRL_ROOT || pData->eTreeItemType == STYLE_WAVECTRL_NODE)
	{
		InsertStyle(STYLE_WAVECTRL_NODE);
	}
	else if (pData->eTreeItemType == STYLE_MENU_ROOT || pData->eTreeItemType == STYLE_MENU_NODE)
	{
		InsertStyle(STYLE_MENU_NODE);
	}
	else if (pData->eTreeItemType == STYLE_MENUBAR_ROOT || pData->eTreeItemType == STYLE_MENUBAR_NODE)
	{
		InsertStyle(STYLE_MENUBAR_NODE);
	}
	else if (pData->eTreeItemType == STYLE_HEADERCTRL_ROOT || pData->eTreeItemType == STYLE_HEADERCTRL_NODE)
	{
		InsertStyle(STYLE_HEADERCTRL_NODE);
	}
	else if (pData->eTreeItemType == STYLE_HORSCROLLBAR_ROOT || pData->eTreeItemType == STYLE_HORSCROLLBAR_NODE)
	{
		InsertStyle(STYLE_HORSCROLLBAR_NODE);
	}
	else if (pData->eTreeItemType == STYLE_VERSCROLLBAR_ROOT || pData->eTreeItemType == STYLE_VERSCROLLBAR_NODE)
	{
		InsertStyle(STYLE_VERSCROLLBAR_NODE);
	}
	else if (pData->eTreeItemType == STYLE_SPLITTER_ROOT || pData->eTreeItemType == STYLE_SPLITTER_NODE)
	{
		InsertStyle(STYLE_SPLITTER_NODE);
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CFileView::OnUpdateInsertStyleItem(CCmdUI *pCmdUI)
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hSelItem);
	if ((pData->eTreeItemType >= STYLE_PUSHBUTTON_ROOT && pData->eTreeItemType <= STYLE_SPLITTER_ROOT)
		|| (pData->eTreeItemType >= STYLE_PUSHBUTTON_NODE && pData->eTreeItemType <= STYLE_SPLITTER_NODE))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CFileView::OnDeleteStyle()
{
	int nRet = AfxMessageBox(_T("Do you want to delete the style?"), MB_YESNO);
	if (nRet == IDYES)
	{
		HTREEITEM hTI = m_treeWindow.GetSelectedItem();
#ifdef _DEBUG
		CString strStyleID = m_treeWindow.GetItemText(hTI);
#endif
		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hTI);

		if (m_treeWindow.DeleteItem(hTI))
		{
			CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
			if (pDoc != NULL)
			{
				pDoc->SetModifiedFlag(TRUE);
			}
		}
	}
}

void CFileView::OnUpdateDeleteStyle(CCmdUI *pCmdUI)
{
	HTREEITEM hSelItem = m_treeWindow.GetSelectedItem();
	if (hSelItem == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)m_treeWindow.GetItemData(hSelItem);
	if (pData->eTreeItemType >= STYLE_PUSHBUTTON_NODE && pData->eTreeItemType <= STYLE_SPLITTER_NODE)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
