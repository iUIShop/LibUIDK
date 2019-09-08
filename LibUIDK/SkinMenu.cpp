// CustMenu.cpp: implementation of the CSkinMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////

class CSkinMenuText : public CSkinMenuItem	//Normal item with text and an optional icon
{
public:
	CSkinMenuText(DWORD dwID, LPCTSTR strText, HICON hIcon = NULL, CSkinMenu *pSkinMenu = NULL) : CSkinMenuItem()
	{
		m_dwID = dwID;
		m_strText = strText;
		m_hIcon = hIcon;
		m_pSkinMenu = pSkinMenu;
	}
};

class CSkinMenuSeparator : public CSkinMenuItem //A separator item
{
public:
	CSkinMenuSeparator() : CSkinMenuItem()
	{
		m_bSeparator = true;
	}
};

class CSkinMenuIcon : public CSkinMenuItem //A button only item
{
public:
	CSkinMenuIcon(DWORD dwID, HICON hIcon = NULL) : CSkinMenuItem()
	{
		m_dwID = dwID;
		m_hIcon = hIcon;
	}
};

class CSkinMenuSideBar : public CSkinMenuItem //A gradient sidebar
{
public:
	CSkinMenuSideBar(int nSize = 32, LPCTSTR strText = NULL, HICON hIcon = NULL, DWORD dwID = 0) : CSkinMenuItem()
	{
		m_dwID = dwID;
		m_bSideBar = true;
		m_strText = strText;
		m_hIcon = hIcon;
		m_nSize = nSize;
	}
};

// constants used for drawing
const int CXGAP = 6;				// num pixels between button and text
const int CXTEXTMARGIN = 2;		// num pixels after hilite to start text
const int CYBUTTONMARGIN = 2;	// ditto for height
//draw embossed icon for the disabled item
const DWORD	MAGICROP = 0xb8074a;

// DrawText flags
const int DT_DEFAULT = DT_SINGLELINE | DT_LEFT | DT_VCENTER;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinMenu, CMenu)

// The following variable need to be deleted.
struct MENU_MEMORY
{
	std::vector<CSkinMenuItem *> m_vpMenuItemData;
	std::vector<CSkinMenu *> m_vpSkinMenu;
};

#define m_vpMenuItemData (((MENU_MEMORY *)m_pMember)->m_vpMenuItemData)
#define m_vpSkinMenu (((MENU_MEMORY *)m_pMember)->m_vpSkinMenu)


CSkinMenu::CSkinMenu()
{
	m_pMember = new MENU_MEMORY;
}

CSkinMenu::~CSkinMenu()
{
	ReleaseObject();

	if (m_pMember != NULL)
	{
		size_t nMenuItemDataCount = m_vpMenuItemData.size();
		for (size_t i = 0; i < nMenuItemDataCount; ++i)
		{
			CSkinMenuItem *pItem = m_vpMenuItemData[i];
			SafeDelete(pItem);
		}
		m_vpMenuItemData.clear();

		size_t nSkinMenuCount = m_vpSkinMenu.size();
		for (size_t j = 0; j < nSkinMenuCount; ++j)
		{
			CSkinMenu *pSkinMenu = m_vpSkinMenu[j];
			SafeDelete(pSkinMenu);
		}
		m_vpSkinMenu.clear();

		delete (MENU_MEMORY *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinMenu::BindStyle(LPCTSTR lpszStyleID)
{
	if (m_hMenu != NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	MENUPROPERTIES *pMenuProp = (MENUPROPERTIES *)CUIMgr::GetStyleItem(STYLET_MENU, lpszStyleID);

	return BindStyle(pMenuProp);
}

int CSkinMenu::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	if (m_hMenu != NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}

	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_MENU && ect != CT_MENU)
	{
		return -3;
	}

	MENUPROPERTIES *pMenuProp = (MENUPROPERTIES *)pCtrlProp;

	// Default font
	CString strCurrentFont = CUIMgr::GetDefaultFont();
	RESFONTPROPERTIES *pResDefaultFont = CUIMgr::GetDefaultFontResource(strCurrentFont);
#ifdef _DEBUG
	LOGFONT lf = pResDefaultFont->m_logFont;
	lf;
#endif // _DEBUG

	CFont *pFont = NULL;

	// Set menu border
	SetMenuBorderStyle(pMenuProp->m_eMenuBorderStyle);
	SetFlatMenuBorderColor(pMenuProp->m_crFlatMenuBorder);

	// Height of menu item
	SetItemHeight(pMenuProp->m_nMenuItemHeight);

	// Set normal menu item
	SetNormalItemTextColor(pMenuProp->m_crNormalItemText);
	SetNormalItemFont(&(LOGFONT)pMenuProp->m_lfNormalItemText);
	SetNormalItemBkType(pMenuProp->m_eNormalItemBkType);
	SetNormalItemBkColor(pMenuProp->m_crNormalItemBkColor);
	if (m_MenuMember.m_NormalItemProp.m_eBackgroundType == BKT_IMAGE)
	{
		SetNormalItemBkImage(pMenuProp->m_strNormalItemBkImage);

		m_MenuMember.m_NormalItemProp.m_ptResize = pMenuProp->m_ptNormalItemBkResize;
	}

	// Set highlight menu item
	SetHighlightItemTextColor(pMenuProp->m_crHighlightItemText);
	SetHighlightItemFont(&(LOGFONT)pMenuProp->m_lfHighlightItemText);
	SetHighlightItemBkType(pMenuProp->m_eHighlightItemBkType);
	SetHighlightItemBkColor(pMenuProp->m_crHighlightItemBkColor);
	if (m_MenuMember.m_HighlightItemProp.m_eBackgroundType == BKT_IMAGE)
	{
		SetHighlightItemBkImage(pMenuProp->m_strHighlightItemBkImage);

		m_MenuMember.m_HighlightItemProp.m_ptResize = pMenuProp->m_ptHighlightItemBkResize;
	}

	// Set disabled menu item
	SetDisabledItemTextColor(pMenuProp->m_crDisabledItemText);
	SetDisabledItemFont(&(LOGFONT)pMenuProp->m_lfDisabledItemText);
	SetDisabledItemBkType(pMenuProp->m_eDisabledItemBkType);
	SetDisabledItemBkColor(pMenuProp->m_crDisabledItemBkColor);
	if (m_MenuMember.m_DisabledItemProp.m_eBackgroundType == BKT_IMAGE)
	{
		SetDisabledItemBkImage(pMenuProp->m_strDisabledItemBkImage);

		m_MenuMember.m_DisabledItemProp.m_ptResize = pMenuProp->m_ptDisabledItemBkResize;
	}

	// Set side bar like start menu
	// 	SetSideBarStartColor(pMenuProp->crSideBarStart);
	// 	SetSideBarEndColor(pMenuProp->crSideBarEnd);

	// Set separator
	SetSeparatorBkType(pMenuProp->m_eSeparatorBkType);
	SetSeparatorColor(pMenuProp->m_crSeparator);
	SetSeparatorLeftMargin(pMenuProp->m_nSeparatorLeftMargin);
	SetSeparatorRightMargin(pMenuProp->m_nSeparatorRightMargin);
	if (m_MenuMember.m_eSeparatorBkType == BKT_IMAGE)
	{
		SetSeparatorBkImage(pMenuProp->m_strSeparatorBkImage);

		m_MenuMember.m_ptSeparatorResize = pMenuProp->m_ptSeparatorBkImageResize;
	}

	// Menu check icon
	SetMenuCheckImage(pMenuProp->m_strMenuCheckImage);

	// Set icon area
	SetIconAreaWidth(pMenuProp->m_nIconAreaWidth);
	SetIconAreaStartColor(pMenuProp->m_crIconAreaStart);
	SetIconAreaEndColor(pMenuProp->m_crIconAreaEnd);
	DrawIconBk(pMenuProp->m_bDrawIconBk);

	return 0;
}

int CSkinMenu::BindMenuStyle(MENUMEMBER *pMenuMember)
{
	// Menu border
	SetMenuBorderStyle(pMenuMember->m_eMbs);
	SetFlatMenuBorderColor(pMenuMember->m_crFlatMenuBorder);

	// Height of menu item
	SetItemHeight(pMenuMember->m_nItemHeight);

	// Normal menu item
	SetNormalItemTextColor(pMenuMember->m_NormalItemProp.m_crItemText);
	SetNormalItemFont(&pMenuMember->m_NormalItemProp.m_lfItem);
	SetNormalItemBkType(pMenuMember->m_NormalItemProp.m_eBackgroundType);
	SetNormalItemBkColor(pMenuMember->m_NormalItemProp.m_crItemBk);
	SetNormalItemBkImage(pMenuMember->m_NormalItemProp.m_hItemBk->GetSafeImageName());

	// Highlight menu item
	SetHighlightItemTextColor(pMenuMember->m_HighlightItemProp.m_crItemText);
	SetHighlightItemFont(&pMenuMember->m_HighlightItemProp.m_lfItem);
	SetHighlightItemBkType(pMenuMember->m_HighlightItemProp.m_eBackgroundType);
	SetHighlightItemBkColor(pMenuMember->m_HighlightItemProp.m_crItemBk);
	SetHighlightItemBkImage(pMenuMember->m_HighlightItemProp.m_hItemBk->GetSafeImageName());

	// Disable menu item
	SetDisabledItemTextColor(pMenuMember->m_DisabledItemProp.m_crItemText);
	SetDisabledItemFont(&pMenuMember->m_DisabledItemProp.m_lfItem);
	SetDisabledItemBkType(pMenuMember->m_DisabledItemProp.m_eBackgroundType);
	SetDisabledItemBkColor(pMenuMember->m_DisabledItemProp.m_crItemBk);
	SetDisabledItemBkImage(pMenuMember->m_DisabledItemProp.m_hItemBk->GetSafeImageName());

	// Side bar like start menu
	SetSideBarStartColor(pMenuMember->m_crSideBarStart);
	SetSideBarEndColor(pMenuMember->m_crSideBarEnd);

	// Separator
	SetSeparatorBkType(pMenuMember->m_eSeparatorBkType);
	SetSeparatorColor(pMenuMember->m_crSeparator);
	SetSeparatorLeftMargin(pMenuMember->m_nSeparatorLeftMargin);
	SetSeparatorRightMargin(pMenuMember->m_nSeparatorRightMargin);
	SetSeparatorBkImage(pMenuMember->m_hSeparator->GetSafeImageName());

	// Menu check icon
	SetMenuCheckImage(pMenuMember->m_himgMenuCheck->GetSafeImageName());

	// Icon area
	SetIconAreaWidth(pMenuMember->m_nIconAreaWidth);
	SetIconAreaStartColor(pMenuMember->m_crIconAreaStart);
	SetIconAreaEndColor(pMenuMember->m_crIconAreaEnd);
	DrawIconBk(pMenuMember->m_bDrawIconBk);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// protected

//clear all memory and handles
int CSkinMenu::ReleaseObject()
{
	if (m_hMenu == NULL)
	{
		return -1;
	}

	// Bitmap
	ReleaseIUIImage(m_MenuMember.m_NormalItemProp.m_hItemBk);
	ReleaseIUIImage(m_MenuMember.m_HighlightItemProp.m_hItemBk);
	ReleaseIUIImage(m_MenuMember.m_DisabledItemProp.m_hItemBk);
	ReleaseIUIImage(m_MenuMember.m_hSeparator);

	return 0;
}

// Shorthand to fill a rectangle with a solid color.
int CSkinMenu::FillRect(CDC *pDC, const CRect &rc, COLORREF color)
{
	CBrush brush(color);
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// message and virtual

// Static member for keyboard operation, you can used it in you parent window
// it work with shortcut key
LRESULT CSkinMenu::OnMenuChar(UINT nChar, UINT nFlags, CMenu *pMenu)
{
	UINT iCurrentItem = (UINT) - 1; // guaranteed higher than any command ID
	CUIntArray arItemsMatched;		// items that match the character typed

	UINT nItem = pMenu->GetMenuItemCount();
	UINT i = 0;
	for (i = 0; i < nItem; i++)
	{
		MENUITEMINFO	info;
		memset(&info, 0, sizeof(info));
		info.cbSize = sizeof(info);
		info.fMask = MIIM_DATA | MIIM_TYPE | MIIM_STATE;
		::GetMenuItemInfo(*pMenu, i, TRUE, &info);

		CSkinMenuItem *pData = (CSkinMenuItem *)info.dwItemData;
		if ((info.fType & MFT_OWNERDRAW) && pData && pData->IsMyData())
		{
			CString	text = pData->m_strText;
			int iAmpersand = text.Find('&');
			if (iAmpersand >= 0 && toupper(nChar) == toupper(text[iAmpersand + 1]))
			{
				arItemsMatched.Add(i);
			}
		}
		if (info.fState & MFS_HILITE)
		{
			iCurrentItem = i;    // note index of current item
		}
	}


	// arItemsMatched now contains indexes of items that match the char typed.
	//
	//   * if none: beep
	//   * if one:  execute it
	//   * if more than one: hilite next
	//
	UINT_PTR nFound = arItemsMatched.GetSize();
	if (nFound == 0)
	{
		return 0;
	}
	else if (nFound == 1)
	{
		return MAKELONG(arItemsMatched[0], MNC_EXECUTE);
	}

	// more than one found--return 1st one past current selected item;
	UINT iSelect = 0;
	for (i = 0; i < nFound; i++)
	{
		if (arItemsMatched[i] > iCurrentItem)
		{
			iSelect = i;
			break;
		}
	}
	return MAKELONG(arItemsMatched[iSelect], MNC_SELECT);
}

void CSkinMenu::MeasureItem(LPMEASUREITEMSTRUCT lpmis)
{
	if (lpmis->CtlType != ODT_MENU)
	{
		return;
	}

	CSkinMenuItem *pItem = (CSkinMenuItem *)lpmis->itemData;

	if (pItem == NULL || !pItem->IsMyData())
	{
		return;
	}

	if (pItem->m_bSideBar)
	{
		lpmis->itemWidth = pItem->m_nSize;
		lpmis->itemHeight = 0;
	}
	else if (pItem->m_bSeparator)
	{
		BOOL bSeparatorImageHeight = FALSE;
		BACKGROUND_TYPE bt = GetSeparatorBkType();
		if (bt == BKT_IMAGE)
		{
			if (m_MenuMember.m_hSeparator != NULL)
			{
				lpmis->itemHeight = m_MenuMember.m_hSeparator->GetHeight();
				bSeparatorImageHeight = TRUE;
			}
		}

		if (!bSeparatorImageHeight)
		{
			// separator: use half system height and zero width
			lpmis->itemHeight = ::GetSystemMetrics(SM_CYMENUCHECK) >> 1;
		}

		lpmis->itemWidth  = 0;
	}
	else	// General menu item
	{
		// the height of the item
		lpmis->itemHeight = m_MenuMember.m_nItemHeight;

		// Calculate the size needed to draw the text: use DrawText with DT_CALCRECT
		CWindowDC dc(NULL);
		CRect rcText(0, 0, 0, 0);
		CFont font;
		font.CreateFontIndirect(&m_MenuMember.m_NormalItemProp.m_lfItem);
		CFont *pOldFont = dc.SelectObject(&font);
		dc.DrawText(pItem->m_strText, rcText, DT_DEFAULT | DT_CALCRECT);
		dc.SelectObject(pOldFont);

		// width is width of text plus a bunch of stuff
		int cx = rcText.Width();
		cx += CXTEXTMARGIN << 1;		// L/R margin for readability
		cx += CXGAP;					// space between button and menu text
		cx += (pItem->m_nSize + CYBUTTONMARGIN * 2) << 1;		// button width (L=button; R=empty margin)

		lpmis->itemWidth = cx;
	}

	// whatever value I return in lpmis->itemWidth, Windows will add the
	// width of a menu check mark, so I must subtract to defeat Windows. Argh.
	//
	lpmis->itemWidth -= GetSystemMetrics(SM_CXMENUCHECK) - 1;
}

void CSkinMenu::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	ASSERT(lpdis != NULL);

	if (lpdis->CtlType != ODT_MENU)
	{
		return;
	}

	CSkinMenuItem *pItem = (CSkinMenuItem *)lpdis->itemData;
	if (pItem == NULL)
	{
		return;
	}

	ASSERT(lpdis->itemAction != ODA_FOCUS);
	ASSERT(lpdis->hDC != NULL);

	CDC dc;
	dc.Attach(lpdis->hDC);

	CRect rcItem = lpdis->rcItem;
	rcItem.right += 3;// hide the vertical break bar line, the width of break bar is 4 pixels.

	if (pItem->m_bSideBar)
	{
		DrawSideBar(&dc, rcItem, pItem->m_hIcon, pItem->m_strText);
	}
	else if (pItem->m_bSeparator)
	{
		DrawSeparator(&dc, rcItem);
	}
	else	// General menu item
	{
		DrawGeneralItem(&dc, rcItem, lpdis);
	}

	dc.Detach();
}

//////////////////////////////////////////////////////////////////////////
// protected

int CSkinMenu::DrawSideBar(CDC *pDC, const CRect &rcItem, HICON hIcon, CString strText)
{
	CRect rcClipBox;
	pDC->GetClipBox(rcClipBox);
	//before drawing the sidebar, we must fill the entire menu area with its background color,
	//or else, the break bar area will remain the the default menu color
	//so, if you want to avoid strange color and don't want a sidebar, just add a sidebar with
	//zero width

	//draw the side bar
	CRect rc = rcItem;
	rc.top = rcClipBox.top;
	rc.bottom = rcClipBox.bottom;

	rc.right += 3;	//fill the gap produced by the menu break

	HBITMAP	bmpBar = CreateGradientBitmap(pDC->m_hDC, m_MenuMember.m_crSideBarStart, m_MenuMember.m_crSideBarEnd, rc.Width(), rc.Height(), 0, 256);
	if (bmpBar)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(memDC.m_hDC, bmpBar);
		pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		::SelectObject(memDC, hOldBmp);
		::DeleteObject(bmpBar);
	}

	//Draw Sidebar text
	CFont	vertFont;
	vertFont.CreateFont(16, 0, 900, 900, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
	CFont *pOldFont = pDC->SelectObject(&vertFont);
	COLORREF oldColor = pDC->GetTextColor();
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(rc.left + 2, rc.bottom - 4, strText);
	pDC->SetTextColor(oldColor);
	pDC->SelectObject(pOldFont);
	vertFont.DeleteObject();

	return 0;
}

int CSkinMenu::DrawSeparator(CDC *pDC, const CRect &rcItem)
{
	if (m_MenuMember.m_eSeparatorBkType == BKT_COLOR)
	{
		// Draw color background
		DrawColorBackGround(pDC, rcItem, FALSE, FALSE);

		// Draw the icon area
		CRect rcArea(rcItem.TopLeft(), CSize(m_MenuMember.m_nIconAreaWidth, rcItem.Height()));
		DrawIconArea(pDC, rcArea, FALSE, FALSE, FALSE);

		// draw separator line
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, m_MenuMember.m_crSeparator);
		CPen *pOldPen = pDC->SelectObject(&pen);

		int nY = rcItem.top + rcItem.Height() / 2;
		pDC->MoveTo(m_MenuMember.m_nIconAreaWidth + m_MenuMember.m_nSeparatorLeftMargin, nY);
		pDC->LineTo(rcItem.Width() - m_MenuMember.m_nSeparatorRightMargin, nY);

		pDC->SelectObject(pOldPen);
	}
	else
	{
		// Must draw to compatible dc, otherwise, draw error.
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(pDC, rcItem.Width(), rcItem.Height());

		CBitmap *pbmpOldMem = dcMem.SelectObject(&bmpMem);

		IUINineGridBlt(dcMem.GetSafeHdc(), 0, 0, rcItem.Width(), rcItem.Height(),
			m_MenuMember.m_hSeparator, m_MenuMember.m_ptSeparatorResize, 1, 1);

		pDC->BitBlt(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), &dcMem, 0, 0, SRCCOPY);
		dcMem.SelectObject(pbmpOldMem);
	}

	return 0;
}

int CSkinMenu::DrawGeneralItem(CDC *pDC, const CRect &rcItem, LPDRAWITEMSTRUCT lpdis)
{
	BOOL bDisabled = lpdis->itemState & ODS_GRAYED;
	BOOL bSelected = lpdis->itemState & ODS_SELECTED;
	BOOL bChecked  = lpdis->itemState & ODS_CHECKED;
	CSkinMenuItem *pItem = (CSkinMenuItem *)lpdis->itemData;

	// draw the background first
	if (bDisabled)
	{
		DrawGeneralItemState(pDC, rcItem, lpdis, &m_MenuMember.m_DisabledItemProp);
	}
	else
	{
		if (bSelected)
		{
			DrawGeneralItemState(pDC, rcItem, lpdis, &m_MenuMember.m_HighlightItemProp);
		}
		else	// Normal
		{
			DrawGeneralItemState(pDC, rcItem, lpdis, &m_MenuMember.m_NormalItemProp);
		}
	}

	CRect rcButton(rcItem.TopLeft(), CSize(m_MenuMember.m_nIconAreaWidth, rcItem.Height()));

	// draw the icon actually
	if (pItem->m_hIcon)
	{
		CRect rcIcon = rcButton;
		rcIcon.DeflateRect(2, 2);
		DrawIcon(pDC, rcIcon, pItem->m_hIcon, bSelected, bDisabled);
	}
	else if (bChecked)
	{
		//draw the check mark
		CRect rcCheck = rcButton;
		rcCheck.DeflateRect(2, 2);
		DrawCheckMark(pDC, rcCheck, bSelected);
	}

	//draw text finally
	CRect rcText = rcItem;				 // start w/whole item
	rcText.left += rcButton.Width() + CXGAP + CXTEXTMARGIN; // left margin
	rcText.right -= pItem->m_nSize;				 // right margin
	rcText.left -= 5;		// add by scottliu
	rcText.right += 10;		//
	DrawText(pDC, rcText, pItem->m_strText, bSelected, bDisabled, lpdis->itemState & ODS_DEFAULT ? TRUE : FALSE);

	return 0;
}

int CSkinMenu::DrawGeneralItemState(CDC *pDC, const CRect &rcItem, LPDRAWITEMSTRUCT lpdis, const MENU_ITEM_PROPERTIES *pmip)
{
	BOOL bDisabled = lpdis->itemState & ODS_GRAYED;
	BOOL bSelected = lpdis->itemState & ODS_SELECTED;
	BOOL bChecked  = lpdis->itemState & ODS_CHECKED;
	CSkinMenuItem *pItem = (CSkinMenuItem *)lpdis->itemData;

	CRect rcButton(rcItem.TopLeft(), CSize(m_MenuMember.m_nIconAreaWidth, rcItem.Height()));

	if (pmip->m_eBackgroundType == BKT_COLOR)
	{
		DrawColorBackGround(pDC, rcItem, bSelected, bDisabled);

		// Draw the icon area
		CRect rcArea(rcItem.TopLeft(), CSize(m_MenuMember.m_nIconAreaWidth, rcItem.Height()));
		DrawIconArea(pDC, rcArea, bSelected, bDisabled, bChecked);

		// draw the button, not the icon
		if ((pItem->m_hIcon || bChecked) && m_MenuMember.m_bDrawIconBk)
		{
			DrawButton(pDC, rcButton, bSelected, bDisabled, bChecked);
		}
	}
	else if (pmip->m_eBackgroundType == BKT_IMAGE)
	{
		// Must draw to compatible dc, otherwise, draw error.
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(pDC, rcItem.Width(), rcItem.Height());

		CBitmap *pbmpOldMem = dcMem.SelectObject(&bmpMem);

		IUINineGridBlt(dcMem.GetSafeHdc(), 0, 0, rcItem.Width(), rcItem.Height(),
			pmip->m_hItemBk, pmip->m_ptResize, 1, 1);

		pDC->BitBlt(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), &dcMem, 0, 0, SRCCOPY);
		dcMem.SelectObject(pbmpOldMem);
	}
	else
	{
		ASSERT(FALSE);
	}

	return 0;
}

int CSkinMenu::DrawColorBackGround(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled)
{
	if (bDisabled)
	{
		FillRect(pDC, rect, m_MenuMember.m_DisabledItemProp.m_crItemBk);
	}
	else
	{
		if (bSelected)
		{
			rect.DeflateRect(1, 1, 1, 1);
			FillRect(pDC, rect, m_MenuMember.m_HighlightItemProp.m_crItemBk);
		}
		else	// While moving on the disabled menu item, don't changed the background.
		{
			FillRect(pDC, rect, m_MenuMember.m_NormalItemProp.m_crItemBk);
		}
	}

	return 0;
}

int CSkinMenu::DrawIconArea(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked)
{
	if (!bSelected || bDisabled)
	{
		HBITMAP	bmpBar = CreateGradientBitmap(pDC->m_hDC, m_MenuMember.m_crIconAreaStart, m_MenuMember.m_crIconAreaEnd, rect.Width(), rect.Height(), 2, 256);
		if (bmpBar)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			HBITMAP hOldBmp = (HBITMAP)::SelectObject(memDC.m_hDC, bmpBar);
			pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
			::SelectObject(memDC.m_hDC, hOldBmp);
			::DeleteObject(bmpBar);
		}
	}

	return 0;
}

// draw the icon button, the icon is not included
int CSkinMenu::DrawButton(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked)
{
	if (!m_MenuMember.m_bDrawIconBk)
	{
		return 1;
	}

	// normal: fill BG depending on state
	FillRect(pDC, rect, (bChecked && !bSelected) ? m_MenuMember.m_NormalItemProp.m_crItemBk + RGB(2, 2, 2) : m_MenuMember.m_NormalItemProp.m_crItemBk);

	// draw pushed-in or popped-out edge
	if (!bDisabled && (bSelected || bChecked))
	{
		pDC->DrawEdge(rect, bChecked ? BDR_SUNKENOUTER : BDR_RAISEDINNER, BF_RECT);
	}

	return 0;
}

//draw the icon
int CSkinMenu::DrawIcon(CDC *pDC, CRect rect, HICON hIcon, BOOL bSelected, BOOL bDisabled)
{
	if (bDisabled)
	{
		DrawEmbossed(pDC, hIcon, rect);
	}
	else
	{
		::DrawIconEx(pDC->m_hDC, rect.left, rect.top, hIcon, rect.Width(), rect.Height(), 0, NULL, DI_NORMAL | DI_MASK);
	}

	return 0;
}

int CSkinMenu::DrawEmbossed(CDC *pDC, HICON hIcon, CRect rect, BOOL bColor/* = FALSE*/)
{
	CDC	memdc;
	memdc.CreateCompatibleDC(pDC);
	int cx = rect.Width();
	int cy = rect.Height();


	// create mono or color bitmap
	CBitmap bm;
	if (bColor)
	{
		bm.CreateCompatibleBitmap(pDC, cx, cy);
	}
	else
	{
		bm.CreateBitmap(cx, cy, 1, 1, NULL);
	}

	// draw image into memory DC--fill BG white first
	CBitmap *pOldBitmap = memdc.SelectObject(&bm);
	//FillRect(&memdc, CRect(0, 0, cx, cy), m_crNormalItemBk);
	memdc.PatBlt(0, 0, cx, cy, WHITENESS);
	::DrawIconEx(memdc.m_hDC, 0, 0, hIcon, cx, cy, 1, NULL, DI_NORMAL);

	// This seems to be required. Why, I don't know. ???
	COLORREF colorOldBG = pDC->SetBkColor(RGB(255, 255, 255));

	// Draw using hilite offset by (1,1), then shadow
	CBrush brShadow(GetSysColor(COLOR_3DSHADOW));
	CBrush brHilite(GetSysColor(COLOR_3DHIGHLIGHT));
	CBrush *pOldBrush = pDC->SelectObject(&brHilite);
	pDC->BitBlt(rect.left + 1, rect.top + 1, cx, cy, &memdc, 0, 0, MAGICROP);
	pDC->SelectObject(&brShadow);
	pDC->BitBlt(rect.left, rect.top, cx, cy, &memdc, 0, 0, MAGICROP);
	pDC->SelectObject(pOldBrush);
	pDC->SetBkColor(colorOldBG);		 // restore
	memdc.SelectObject(pOldBitmap);		 // ...
	bm.DeleteObject();
	brShadow.DeleteObject();
	brHilite.DeleteObject();

	return 0;
}

//draw the check mark
int CSkinMenu::DrawCheckMark(CDC *pDC, CRect rect, BOOL bSelected)
{
	CRect rcDest = rect;

	if (m_MenuMember.m_himgMenuCheck == NULL)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(pDC, rcDest.Width(), rcDest.Height());
		CBitmap *pbmpOld = dcMem.SelectObject(&bmpMem);

		dcMem.DrawFrameControl(CRect(0, 0, rcDest.Width(), rcDest.Height()), DFC_MENU, DFCS_MENUCHECK | DFCS_TRANSPARENT);

		TransparentBlt2(pDC->GetSafeHdc(), 0, rcDest.top, rcDest.Width(), rcDest.Height(), dcMem.GetSafeHdc(), 0, 0, rcDest.Width(), rcDest.Height(), RGB(255, 255, 255));

		dcMem.SelectObject(pbmpOld);
	}
	else
	{
		IUICenterBitBlt(pDC->GetSafeHdc(), rcDest, m_MenuMember.m_himgMenuCheck, SRCCOPY);
	}

	return 0;
}

int CSkinMenu::DrawMenuText(CDC &dc, CRect rc, CString text, COLORREF color)
{
	CString left = text;
	CString right;
	int iTabPos = left.Find('\t');
	if (iTabPos >= 0)
	{
		right = left.Right(left.GetLength() - iTabPos - 1);
		left  = left.Left(iTabPos);
	}
	dc.SetTextColor(color);
	dc.DrawText(left, &rc, DT_DEFAULT);
	if (iTabPos > 0)
	{
		dc.DrawText(right, &rc, DT_DEFAULT | DT_RIGHT);
	}

	return 0;
}

//Draw menu text
int CSkinMenu::DrawText(CDC *pDC, CRect rect, CString strText, BOOL bSelected, BOOL bDisabled, BOOL bBold)
{
	// Create font
	LOGFONT	logFont;
	if (bDisabled)
	{
		memcpy(&logFont, &m_MenuMember.m_DisabledItemProp.m_lfItem, sizeof(LOGFONT));
	}
	else
	{
		if (bSelected)
		{
			memcpy(&logFont, &m_MenuMember.m_HighlightItemProp.m_lfItem, sizeof(LOGFONT));
		}
		else
		{
			memcpy(&logFont, &m_MenuMember.m_NormalItemProp.m_lfItem, sizeof(LOGFONT));
		}
	}
	if (bBold)
	{
		logFont.lfWeight = FW_BOLD;
	}

	CFont font;
	font.CreateFontIndirect(&logFont);
	CFont *pOldFont = pDC->SelectObject(&font);

	pDC->SetBkMode(TRANSPARENT);

	// Draw text with specified color
	COLORREF crText = RGB(0, 0, 0);
	if (bDisabled)
	{
		crText = m_MenuMember.m_DisabledItemProp.m_crItemText;
	}
	else
	{
		crText = bSelected ? m_MenuMember.m_HighlightItemProp.m_crItemText : m_MenuMember.m_NormalItemProp.m_crItemText;
	}
	DrawMenuText(*pDC, rect, strText, crText);

	pDC->SelectObject(pOldFont);

	font.DeleteObject();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

BOOL CSkinMenu::LoadMenu(UINT nIDResource)
{
	return LoadMenu(MAKEINTRESOURCE(nIDResource));
}

BOOL CSkinMenu::LoadMenu(LPCTSTR lpszResourceName)
{
	// Load a temporary menu 'menuTmp'
	CMenu menuTmp;
	BOOL bRet = menuTmp.LoadMenu(lpszResourceName);
	if (!bRet)
	{
		return FALSE;
	}

	// Create a new skin menu
	bRet = CreatePopupMenu();	// This menu item will be as menu bar button.
	if (!bRet)
	{
		return FALSE;
	}

	// Copy the temporary menu item to new menu.
	CopyMenu(menuTmp.m_hMenu, this);

	return bRet;
}

CSkinMenu *CSkinMenu::CopyMenu(HMENU hMenuFrom, CSkinMenu *pMenuTo)
{
	int nItemCount = ::GetMenuItemCount(hMenuFrom);
	for (int i = 0; i < nItemCount; ++i)
	{
		// If pMenuItemFrom is NULL, it's general menu item, otherwise, it's menu item that can pop up another menu.

		HMENU hMenuItemFrom = ::GetSubMenu(hMenuFrom, i);

		// Get menu item text
		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(MENUITEMINFO));
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_TYPE | MIIM_DATA;

		if (!::GetMenuItemInfo(hMenuFrom, i, TRUE, &mii))
		{
			DWORD dwError = GetLastError();
		}

		// Get string from CSkinMenu
		CSkinMenuItem *pData = (CSkinMenuItem *)mii.dwItemData;

		CString strMenuItemText;
		int nResult = ::GetMenuString(hMenuFrom, i, strMenuItemText.GetBuffer(256), 256, MF_BYPOSITION);
		strMenuItemText.ReleaseBuffer();
		if (strMenuItemText.IsEmpty() && pData != NULL)
		{
			strMenuItemText = pData->m_strText;
		}

		// General menu item or separator
		if (hMenuItemFrom == NULL)
		{
			UINT nItemID = ::GetMenuItemID(hMenuFrom, i);

			UINT state = ::GetMenuState(hMenuFrom, i, MF_BYPOSITION);
			if (state & MF_SEPARATOR)
			{
				pMenuTo->AppendSkinSeparator();
			}
			else if (state & MF_GRAYED)
			{
				pMenuTo->AppendSkinMenuString(MF_GRAYED, nItemID, strMenuItemText);
			}
			else
			{
				pMenuTo->AppendSkinMenuString(0, nItemID, strMenuItemText);
			}
		}

		// Popup menu item
		else
		{
			CSkinMenu *pSubPopupMenuTo = new CSkinMenu;
			m_vpSkinMenu.push_back(pSubPopupMenuTo);
			pSubPopupMenuTo->BindMenuStyle(&m_MenuMember);

			BOOL bRet = pSubPopupMenuTo->CreatePopupMenu();
			if (!bRet)
			{
				break;
			}

			CopyMenu(hMenuItemFrom, pSubPopupMenuTo);

			pMenuTo->AppendSkinPopup(0, pSubPopupMenuTo, i + 1, strMenuItemText);
		}
	}

	return pMenuTo;
}

// Append a pop-up menu to the end of exist menu.
BOOL CSkinMenu::AppendSkinPopup(UINT nFlags, CSkinMenu *pPopupSkinMenu, DWORD dwID, LPCTSTR strText)
{
	if (pPopupSkinMenu == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CSkinMenuText *pItem = new CSkinMenuText(dwID, strText, NULL, pPopupSkinMenu);
	m_vpMenuItemData.push_back(pItem);

	nFlags |= (MF_OWNERDRAW | MF_POPUP);

	if (m_MenuMember.m_bBreak)
	{
		nFlags |= MF_MENUBREAK;
	}
	if (m_MenuMember.m_bBreakBar)
	{
		nFlags |= MF_MENUBARBREAK;
	}

	m_MenuMember.m_bBreak = m_MenuMember.m_bBreakBar = FALSE;

	// if nFlags include MF_OWNERDRAW, pItem will be stored in the itemData member
	// of the structure pointed to by the lParam parameter of the WM_MEASUREITEM or WM_DRAWITEM message sent when the menu is created or its appearance is updated.
	return AppendMenu(nFlags, (UINT_PTR)(pPopupSkinMenu->m_hMenu), (LPCTSTR)pItem);
}

// Add a general menu item, an accelerator key could be specified, and the accelerator text will be added automatically
BOOL CSkinMenu::AppendSkinMenuString(UINT nFlags, DWORD dwID, LPCTSTR strText, HICON hIcon/* = NULL */, ACCEL *pAccel/* = 0 */)
{
	CSkinMenuText *pItem = new CSkinMenuText(dwID, strText);
	m_vpMenuItemData.push_back(pItem);

	nFlags |= MF_OWNERDRAW;
	if (m_MenuMember.m_bBreak)
	{
		nFlags |= MF_MENUBREAK;
	}
	if (m_MenuMember.m_bBreakBar)
	{
		nFlags |= MF_MENUBARBREAK;
	}
	m_MenuMember.m_bBreak = m_MenuMember.m_bBreakBar = FALSE;

	if (pItem->m_bSeparator)
	{
		nFlags |= MF_SEPARATOR;
	}

	if (pAccel)
	{
		//		CBCGKeyHelper	keyhelper(pAccel);
		//		CString	strAccel;
		//		keyhelper.Format(strAccel);
		//		if (strAccel.GetLength()>0)
		//		{
		//			pItem->m_strText += _T("\t");
		//			pItem->m_strText += strAccel;
		//		}
	}

	return AppendMenu(nFlags, pItem->m_dwID, (LPCTSTR)pItem);
}

// Add a separator line
BOOL CSkinMenu::AppendSkinSeparator()
{
	UINT nFlags = MF_OWNERDRAW | MF_SEPARATOR;
	if (m_MenuMember.m_bBreak)
	{
		nFlags |= MF_MENUBREAK;
	}
	if (m_MenuMember.m_bBreakBar)
	{
		nFlags |= MF_MENUBARBREAK;
	}
	m_MenuMember.m_bBreak = m_MenuMember.m_bBreakBar = FALSE;

	CSkinMenuSeparator *pItem = new CSkinMenuSeparator;
	m_vpMenuItemData.push_back(pItem);

	return AppendMenu(nFlags, 0, (LPCTSTR)pItem);
}


// Add a gradient sidebar, it must be the first item in a popup menu
BOOL CSkinMenu::AddSideBar(int nSize/* = 32*/, LPCTSTR strText/* = NULL*/, HICON hIcon/* = NULL*/, DWORD dwID/* = 0*/)
{
	CSkinMenuSideBar *pItem = new CSkinMenuSideBar(nSize, strText, hIcon, dwID);
	ASSERT(pItem);
	m_vpMenuItemData.push_back(pItem);

	m_MenuMember.m_bBreak = TRUE;
	m_MenuMember.m_bBreakBar = FALSE;

	return AppendMenu(MF_OWNERDRAW, pItem->m_dwID, (LPCTSTR)pItem);
}

BOOL CSkinMenu::InsertSkinMenu(UINT nPos, UINT nFlags, CSkinMenuItem *pItem)
{
	ASSERT(pItem);

	nFlags |= MF_OWNERDRAW;
	if (m_MenuMember.m_bBreak)
	{
		nFlags |= MF_MENUBREAK;
	}
	if (m_MenuMember.m_bBreakBar)
	{
		nFlags |= MF_MENUBARBREAK;
	}
	m_MenuMember.m_bBreak = m_MenuMember.m_bBreakBar = FALSE;

	return InsertMenu(nPos, nFlags, pItem->m_dwID, (LPCTSTR)pItem);
}

BOOL CSkinMenu::ModifySkinMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem/* = 0*/, LPCTSTR lpszNewItem/* = NULL*/)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_DATA;
	BOOL bRet = GetMenuItemInfo(nPosition, &mii, (nFlags & MF_BYPOSITION) ? TRUE : FALSE);
	CSkinMenuText *pData = (CSkinMenuText *)mii.dwItemData;

	if (nFlags & MF_OWNERDRAW)
	{
		pData->m_strText = lpszNewItem;
	}
	else if (nFlags & MF_SEPARATOR)
	{

	}
	else
	{
		pData->m_strText = lpszNewItem;
	}

	return 0;
}

// Add a new column, the next item added will be in the next column
int CSkinMenu::Break()
{
	m_MenuMember.m_bBreak = TRUE;

	return 0;
}

// Add a new column with a break line. Same as Break, except that a break line is drawn between two columns
int CSkinMenu::BreakBar()
{
	m_MenuMember.m_bBreakBar = TRUE;

	return 0;
}

BOOL CSkinMenu::SetIcon(UINT nID, HICON *hIcon, BOOL bByPos)
{
	MENUITEMINFO Item;
	Item.cbSize = sizeof(MENUITEMINFO);
	Item.fMask = MIIM_DATA;
	GetMenuItemInfo(nID, &Item, bByPos);

	CSkinMenuItem *pItem = (CSkinMenuItem *)Item.dwItemData;
	pItem->m_hIcon = *hIcon;

	// The menu item data is a point, so, needn't call SetMenuItemInfo

	return TRUE;
}

AFX_STATIC CMenu *AFXAPI AfxFindPopupMenuFromID(CMenu *pMenu, UINT nID)
{
	ASSERT_VALID(pMenu);
	// walk through all items, looking for ID match
	UINT nItems = pMenu->GetMenuItemCount();
	for (int iItem = 0; iItem < (int)nItems; iItem++)
	{
		CMenu *pPopup = pMenu->GetSubMenu(iItem);
		if (pPopup != NULL)	// with pop up style
		{
			// Check menu item itself.
#if(_MSC_VER <= 1200) // VC6.0
			if (pPopup->m_hMenu == (HMENU)nID)
#else
			if (pPopup->m_hMenu == (HMENU)(intptr_t)nID)
#endif
			{
				pMenu = CMenu::FromHandlePermanent(pMenu->m_hMenu);
				return pMenu;
			}
			// recurse to child popup
			pPopup = AfxFindPopupMenuFromID(pPopup, nID);
			// check popups on this popup
			if (pPopup != NULL)
			{
				return pPopup;
			}
		}
		else if (pMenu->GetMenuItemID(iItem) == nID)
		{
			// it is a normal item inside our pop up
			pMenu = CMenu::FromHandlePermanent(pMenu->m_hMenu);
			return pMenu;
		}
	}
	// not found
	return NULL;
}

LRESULT MenuOnwerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MEASUREITEM)
	{
		MEASUREITEMSTRUCT *pmis = (MEASUREITEMSTRUCT *)lParam;

		// The following code modify from CWnd::OnMeasureItem

		if (pmis->CtlType == ODT_MENU)
		{
			ASSERT(pmis->CtlID == 0);
			CMenu *pMenu;

			_AFX_THREAD_STATE *pThreadState = AfxGetThreadState();
			if (pThreadState->m_hTrackingWindow == hwnd)
			{
				// start from pop-up
				pMenu = CMenu::FromHandle(pThreadState->m_hTrackingMenu);
			}
			else
			{
				// start from menu bar
				CWnd *pWnd = CWnd::FromHandle(hwnd);
				pMenu = pWnd->GetMenu();
			}

			// If the menu item is pop-up style, the pmis->itemID is the handle of it's pop-up menu. See AppendSkinPopup
			pMenu = AfxFindPopupMenuFromID(pMenu, pmis->itemID);
			if (pMenu != NULL)
			{
				pMenu->MeasureItem(pmis);
			}
			else
				TRACE1("[LibUIDK]===Warning: unknown WM_MEASUREITEM for menu item 0x%04X.\n",
					pmis->itemID);
		}
	}

	WNDPROC wndprocOld = (WNDPROC)GetProp(hwnd, GETPROP_OLDWNDPROC);
	return CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);
}

BOOL CSkinMenu::TrackPopupMenu(UINT nFlags, int x, int y, CWnd *pWnd, LPCRECT lpRect/* = NULL*/)
{
	if (pWnd == NULL)
	{
		return FALSE;
	}

	// For handle WM_MEASUREITEM message in menu's owner window
	WNDPROC procOld = (WNDPROC)IUISetWindowLong(pWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)MenuOnwerWndProc);
	SetProp(pWnd->GetSafeHwnd(), GETPROP_OLDWNDPROC, procOld);

	_IUI_THREAD_STATE *pIUIThreadState = IUIGetThreadState();
	HWND hWndOld = pIUIThreadState->m_hTrackingWindow;
	HMENU hMenuOld = pIUIThreadState->m_hTrackingMenu;
	HWND hWndMenuOld = pIUIThreadState->m_hWndMenu;
	pIUIThreadState->m_hTrackingWindow = pWnd->GetSafeHwnd();
	pIUIThreadState->m_hTrackingMenu = m_hMenu;
	// pIUIThreadState->m_hWndMenu be assign in MenuWndProc in Global.cpp file.
	BOOL bRet = CMenu::TrackPopupMenu(nFlags, x, y, pWnd, lpRect);

	pIUIThreadState->m_hTrackingWindow = hWndOld;
	pIUIThreadState->m_hTrackingMenu = hMenuOld;
	pIUIThreadState->m_hWndMenu = hWndMenuOld;

	IUISetWindowLong(pWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)procOld);

	RemoveProp(pWnd->GetSafeHwnd(), GETPROP_OLDWNDPROC);

	return bRet;
}

// Find a pop up menu from a menu item id
CSkinMenu *CSkinMenu::FindSubMenuFromID(DWORD dwID)
{
	CSkinMenu *pSubMenu;
	CSkinMenu *pResult;
#if (_MSC_VER >= 1600) // VC2010
	int i = 0;
#else
	UINT i = 0;
#endif
	for (i = 0; i < GetMenuItemCount(); i++)
	{
		if (GetMenuItemID(i) == dwID)
		{
			return this;
		}
	}

	for (i = 0; i < GetMenuItemCount(); i++)
	{
		pSubMenu = (CSkinMenu *)GetSubMenu(i);
		if (pSubMenu)
		{
			pResult = pSubMenu->FindSubMenuFromID(dwID);
			if (pResult)
			{
				return pResult;
			}
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// public

int CSkinMenu::SetMenuBorderStyle(MENU_BORDER_STYLE eMbs)
{
	m_MenuMember.m_eMbs = eMbs;

	return 0;
}

MENU_BORDER_STYLE CSkinMenu::GetMenuBorderStyle() const
{
	return m_MenuMember.m_eMbs;
}

int CSkinMenu::SetFlatMenuBorderColor(COLORREF crBorder)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_crFlatMenuBorder = crBorder;

	return 0;
}

COLORREF CSkinMenu::GetFlatMenuBorderColor() const
{
	return m_MenuMember.m_crFlatMenuBorder;
}

int CSkinMenu::SetItemHeight(int nHeight)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_nItemHeight = nHeight;

	return 0;
}

int CSkinMenu::GetItemHeight() const
{
	return m_MenuMember.m_nItemHeight;
}

int CSkinMenu::SetNormalItemTextColor(COLORREF crItemText)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_NormalItemProp.m_crItemText = crItemText;

	return 0;
}

COLORREF CSkinMenu::GetNormalItemTextColor() const
{
	return m_MenuMember.m_NormalItemProp.m_crItemBk;
}

int CSkinMenu::SetNormalItemFont(const LOGFONT *plf)
{
	if (plf == NULL)
	{
		return -1;
	}

	m_MenuMember.m_NormalItemProp.m_lfItem = *plf;

	return 0;
}

int CSkinMenu::GetNormalItemFont(LOGFONT *plf) const
{
	if (plf == NULL)
	{
		return -1;
	}

	*plf = m_MenuMember.m_NormalItemProp.m_lfItem;

	return 0;
}

int CSkinMenu::SetNormalItemBkType(BACKGROUND_TYPE eBkType)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_NormalItemProp.m_eBackgroundType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinMenu::GetNormalItemBkType() const
{
	return m_MenuMember.m_NormalItemProp.m_eBackgroundType;
}

int CSkinMenu::SetNormalItemBkColor(COLORREF crItemBk)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_NormalItemProp.m_crItemBk = crItemBk;

	return 0;
}

COLORREF CSkinMenu::GetNormalItemBkColor() const
{
	return m_MenuMember.m_NormalItemProp.m_crItemBk;
}

int CSkinMenu::SetNormalItemBkImage(LPCTSTR lpszImageName)
{
	ASSERT(m_hMenu == NULL);

	IUISetControlImage(&m_MenuMember.m_NormalItemProp.m_hItemBk, lpszImageName);

	return 0;
}

int CSkinMenu::GetNormalItemBkImage(CString *pstrImageNameItem)
{
	return IUIGetControlImage(m_MenuMember.m_NormalItemProp.m_hItemBk, pstrImageNameItem);
}

int CSkinMenu::SetHighlightItemTextColor(COLORREF crItemText)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_HighlightItemProp.m_crItemText = crItemText;

	return 0;
}

COLORREF CSkinMenu::GetHighlightItemTextColor() const
{
	return m_MenuMember.m_HighlightItemProp.m_crItemText;
}

int CSkinMenu::SetHighlightItemFont(const LOGFONT *plf)
{
	if (plf == NULL)
	{
		return -1;
	}

	m_MenuMember.m_HighlightItemProp.m_lfItem = *plf;

	return 0;
}

int CSkinMenu::GetHighlightItemFont(LOGFONT *plf) const
{
	if (plf == NULL)
	{
		return -1;
	}

	*plf = m_MenuMember.m_HighlightItemProp.m_lfItem;

	return 0;
}

int CSkinMenu::SetHighlightItemBkType(BACKGROUND_TYPE eBkType)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_HighlightItemProp.m_eBackgroundType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinMenu::GetHighlightItemBkType() const
{
	return m_MenuMember.m_HighlightItemProp.m_eBackgroundType;
}

int CSkinMenu::SetHighlightItemBkColor(COLORREF crItemBk)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_HighlightItemProp.m_crItemBk = crItemBk;

	return 0;
}

COLORREF CSkinMenu::GetHighlightItemBkColor() const
{
	return m_MenuMember.m_HighlightItemProp.m_crItemBk;
}

int CSkinMenu::SetHighlightItemBkImage(LPCTSTR lpszImageNameItem)
{
	ASSERT(m_hMenu == NULL);

	IUISetControlImage(&m_MenuMember.m_HighlightItemProp.m_hItemBk, lpszImageNameItem);

	return 0;
}

int CSkinMenu::GetHighlightItemBkImage(CString *pstrImageNameItem)
{
	return IUIGetControlImage(m_MenuMember.m_HighlightItemProp.m_hItemBk, pstrImageNameItem);
}

int CSkinMenu::SetDisabledItemTextColor(COLORREF crItemText)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_DisabledItemProp.m_crItemText = crItemText;

	return 0;
}

COLORREF CSkinMenu::GetDisabledItemTextColor() const
{
	return m_MenuMember.m_DisabledItemProp.m_crItemText;
}

int CSkinMenu::SetDisabledItemFont(const LOGFONT *plf)
{
	if (plf == NULL)
	{
		return -1;
	}

	m_MenuMember.m_DisabledItemProp.m_lfItem = *plf;

	return 0;
}

int CSkinMenu::GetDisabledItemFont(LOGFONT *plf) const
{
	if (plf == NULL)
	{
		return -1;
	}

	*plf = m_MenuMember.m_DisabledItemProp.m_lfItem;

	return 0;
}

int CSkinMenu::SetDisabledItemBkType(BACKGROUND_TYPE eBkType)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_DisabledItemProp.m_eBackgroundType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinMenu::GetDisabledItemBkType() const
{
	return m_MenuMember.m_DisabledItemProp.m_eBackgroundType;
}

int CSkinMenu::SetDisabledItemBkColor(COLORREF crItemBk)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_DisabledItemProp.m_crItemBk = crItemBk;

	return 0;
}

COLORREF CSkinMenu::GetDisabledItemBkColor() const
{
	return m_MenuMember.m_DisabledItemProp.m_crItemBk;
}

int CSkinMenu::SetDisabledItemBkImage(LPCTSTR lpszImageNameItem)
{
	ASSERT(m_hMenu == NULL);

	IUISetControlImage(&m_MenuMember.m_DisabledItemProp.m_hItemBk, lpszImageNameItem);

	return 0;
}

int CSkinMenu::GetDisabledItemBkImage(CString *pstrImageNameItem)
{
	IUIGetControlImage(m_MenuMember.m_DisabledItemProp.m_hItemBk, pstrImageNameItem);

	return 0;
}

int CSkinMenu::SetSideBarStartColor(COLORREF crSideBarStart)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_crSideBarStart = crSideBarStart;

	return 0;
}

COLORREF CSkinMenu::GetSideBarStartColor() const
{
	return m_MenuMember.m_crSideBarStart;
}

int CSkinMenu::SetSideBarEndColor(COLORREF crSideBarEnd)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_crSideBarEnd = crSideBarEnd;

	return 0;
}

COLORREF CSkinMenu::GetSideBarEndColor() const
{
	return m_MenuMember.m_crSideBarEnd;
}

int CSkinMenu::SetSeparatorBkType(BACKGROUND_TYPE eBkType)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_eSeparatorBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinMenu::GetSeparatorBkType() const
{
	return m_MenuMember.m_eSeparatorBkType;
}

int CSkinMenu::SetSeparatorColor(COLORREF crSeparator)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_crSeparator = crSeparator;

	return 0;
}

COLORREF CSkinMenu::GetSeparatorColor() const
{
	return m_MenuMember.m_crSeparator;
}

int CSkinMenu::SetSeparatorLeftMargin(int nLeftMargin)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_nSeparatorLeftMargin = nLeftMargin;

	return 0;
}

int CSkinMenu::GetSeparatorLeftMargin() const
{
	return m_MenuMember.m_nSeparatorLeftMargin;
}

int CSkinMenu::SetSeparatorRightMargin(int nRightMargin)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_nSeparatorRightMargin = nRightMargin;

	return 0;
}

int CSkinMenu::GetSeparatorRightMargin() const
{
	return m_MenuMember.m_nSeparatorRightMargin;
}

int CSkinMenu::SetSeparatorBkImage(LPCTSTR lpszImageNameSeparator)
{
	return IUISetControlImage(&m_MenuMember.m_hSeparator, lpszImageNameSeparator);
}

int CSkinMenu::GetSeparatorBkImage(CString *pstrImageNameSeparator)
{
	return IUIGetControlImage(m_MenuMember.m_hSeparator, pstrImageNameSeparator);
}

int CSkinMenu::SetMenuCheckImage(LPCTSTR lpszImageNameCheck)
{
	return IUISetControlImage(&m_MenuMember.m_himgMenuCheck, lpszImageNameCheck);
}

int CSkinMenu::GetMenuCheckImage(CString *pstrImageNameCheck)
{
	return IUIGetControlImage(m_MenuMember.m_himgMenuCheck, pstrImageNameCheck);
}

int CSkinMenu::SetIconAreaWidth(int nWidth)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_nIconAreaWidth = nWidth;

	return 0;
}

int CSkinMenu::GetIconAreaWidth() const
{
	return m_MenuMember.m_nIconAreaWidth;
}

int CSkinMenu::SetIconAreaStartColor(COLORREF crIconAreaStart)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_crIconAreaStart = crIconAreaStart;

	return 0;
}

COLORREF CSkinMenu::GetIconAreaStartColor() const
{
	return m_MenuMember.m_crIconAreaStart;
}

int CSkinMenu::SetIconAreaEndColor(COLORREF crIconAreaEnd)
{
	ASSERT(m_hMenu == NULL);

	m_MenuMember.m_crIconAreaEnd = crIconAreaEnd;

	return 0;
}

COLORREF CSkinMenu::GetIconAreaEndColor() const
{
	return m_MenuMember.m_crIconAreaEnd;
}

int CSkinMenu::DrawIconBk(BOOL bDraw)
{
	m_MenuMember.m_bDrawIconBk = bDraw;

	return 0;
}

BOOL CSkinMenu::IsDrawIconBk() const
{
	return m_MenuMember.m_bDrawIconBk;
}
