#include "StdAfx.h"
#include "IDMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int CIDMgr::m_nWndStart = 0;
int CIDMgr::m_nBtnStart = 0;
int CIDMgr::m_nChkStart = 0;
int CIDMgr::m_nRadStart = 0;
int CIDMgr::m_nEdtStart = 0;
int CIDMgr::m_nCmbStart = 0;
int CIDMgr::m_nStaStart = 0;
int CIDMgr::m_nPrgStart = 0;
int CIDMgr::m_nSldStart = 0;
int CIDMgr::m_nSpnStart = 0;
int CIDMgr::m_nHotStart = 0;
int CIDMgr::m_nIPAStart = 0;
int CIDMgr::m_nSldExStart = 0;
int CIDMgr::m_nLstCtrlStart = 0;
int CIDMgr::m_nTreeCtrlStart = 0;
int CIDMgr::m_nScrollBarExStart = 0;
int CIDMgr::m_nThumbnailStart = 0;
int CIDMgr::m_nTreeLstStart = 0;
int CIDMgr::m_nRichEditStart = 0;
int CIDMgr::m_nIMRichEditStart = 0;
int CIDMgr::m_nHtmlCtrlStart = 0;
int CIDMgr::m_nDateTimeCtrlStart = 0;
int CIDMgr::m_nWaveCtrlStart = 0;
int CIDMgr::m_nPicCtrlStart = 0;
int CIDMgr::m_nMenuStart = 0;
int CIDMgr::m_nMenuBarStart = 0;
int CIDMgr::m_nSplitterStart = 0;
int CIDMgr::m_nHeaderStart = 0;
int CIDMgr::m_nHorScrollbarStart = 0;
int CIDMgr::m_nVerScrollbarStart = 0;
int CIDMgr::m_nWLRectStart = 0;
int CIDMgr::m_nWLSplitterStart = 0;
int CIDMgr::m_nWLLineStart = 0;
int CIDMgr::m_nWLTextStart = 0;
int CIDMgr::m_nWLBtnStart = 0;
int CIDMgr::m_nWLChkStart = 0;
int CIDMgr::m_nWLRadStart = 0;
int CIDMgr::m_nWLSldStart = 0;
int CIDMgr::m_nWLPicStart = 0;
int CIDMgr::m_nWLRichEditStart = 0;
int CIDMgr::m_nWLIMStart = 0;
int CIDMgr::m_nTaskWndMgrStart = 0;
int CIDMgr::m_nPnlDockStart = 0;
int CIDMgr::m_nPnlUniformGridStart = 0;
int CIDMgr::m_nPnlStackStart = 0;

int CIDMgr::m_nFontResStart = 0;
int CIDMgr::m_nColorResStart = 0;

int CIDMgr::m_nNextControlID = -1;

CIDMgr::CIDMgr()
{
}

CIDMgr::~CIDMgr()
{
}

// manager the control's ID.
CString CIDMgr::NextWindowID()
{
	CString strRet;
	strRet.Format(_T("IDW_WINDOW%d"), ++m_nWndStart);
	return strRet;
}

CString CIDMgr::NextButtonID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_PUSHBUTTON), ++m_nBtnStart);
	return strRet;
}

CString CIDMgr::NextCheckBoxID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_CHECK), ++m_nChkStart);
	return strRet;
}

CString CIDMgr::NextRadioButtonID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RADIO), ++m_nRadStart);
	return strRet;
}

CString CIDMgr::NextEditID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_EDIT), ++m_nEdtStart);
	return strRet;
}

CString CIDMgr::NextComboBoxID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_COMBOBOX), ++m_nCmbStart);
	return strRet;
}

CString CIDMgr::NextStaticID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_STATIC), ++m_nStaStart);
	return strRet;
}

CString CIDMgr::NextProgressID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_PROGRESS), ++m_nPrgStart);
	return strRet;
}

CString CIDMgr::NextSliderID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_SLIDER), ++m_nSldStart);
	return strRet;
}

CString CIDMgr::NextSpinID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_SPIN), ++m_nSpnStart);
	return strRet;
}

CString CIDMgr::NextHotKeyID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_HOTKEY), ++m_nHotStart);
	return strRet;
}

CString CIDMgr::NextIPAddressID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_IPADDRESS), ++m_nIPAStart);
	return strRet;
}

CString CIDMgr::NextSliderExID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_SLIDEREX), ++m_nSldExStart);
	return strRet;
}

CString CIDMgr::NextListCtrlID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_LISTCTRL), ++m_nLstCtrlStart);
	return strRet;
}

CString CIDMgr::NextTreeCtrlID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_TREECTRL), ++m_nTreeCtrlStart);
	return strRet;
}

CString CIDMgr::NextScrollBarExID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_SCROLLBAREX), ++m_nScrollBarExStart);
	return strRet;
}

CString CIDMgr::NextThumbnailID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_THUMBNAIL), ++m_nThumbnailStart);
	return strRet;
}

CString CIDMgr::NextTreeLstID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_TREELIST), ++m_nTreeLstStart);
	return strRet;
}

CString CIDMgr::NextRichEditID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RICHEDIT), ++m_nRichEditStart);
	return strRet;
}

CString CIDMgr::NextIMRichEditID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RICHEDIT_IM), ++m_nIMRichEditStart);
	return strRet;
}

CString CIDMgr::NextHtmlCtrlID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_HTMLCTRL), ++m_nHtmlCtrlStart);
	return strRet;
}

CString CIDMgr::NextDateTimeCtrlID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_DATETIMECTRL), ++m_nDateTimeCtrlStart);
	return strRet;
}

CString CIDMgr::NextWaveCtrlID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WAVECTRL), ++m_nWaveCtrlStart);
	return strRet;
}

CString CIDMgr::NextPicCtrlID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_PICTURE), ++m_nPicCtrlStart);
	return strRet;
}

CString CIDMgr::NextMenuID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_MENU), ++m_nMenuStart);
	return strRet;
}

CString CIDMgr::NextMenuBarID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_MENUBAR), ++m_nMenuBarStart);
	return strRet;
}

CString CIDMgr::NextSplitterID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_SPLITTER), ++m_nSplitterStart);
	return strRet;
}

CString CIDMgr::NextHeaderID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_HEADERCTRL), ++m_nHeaderStart);
	return strRet;
}

CString CIDMgr::NextHorScrollbarID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_HORSCROLLBAR), ++m_nHorScrollbarStart);
	return strRet;
}

CString CIDMgr::NextVerScrollbarID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_VERSCROLLBAR), ++m_nVerScrollbarStart);
	return strRet;
}

CString CIDMgr::NextWLRectID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_RECTCTRL), ++m_nWLRectStart);
	return strRet;
}

CString CIDMgr::NextWLSplitterID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_SPLITTER), ++m_nWLSplitterStart);
	return strRet;
}

CString CIDMgr::NextWLLineID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_LINE), ++m_nWLLineStart);
	return strRet;
}

CString CIDMgr::NextWLTextID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_TEXT), ++m_nWLTextStart);
	return strRet;
}

CString CIDMgr::NextWLBtnID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_BUTTON), ++m_nWLBtnStart);
	return strRet;
}

CString CIDMgr::NextWLChkID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_CHECK), ++m_nWLChkStart);
	return strRet;
}
CString CIDMgr::NextWLRadID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_RADIO), ++m_nWLRadStart);
	return strRet;
}

CString CIDMgr::NextWLSliderID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_SLIDER), ++m_nWLSldStart);
	return strRet;
}

CString CIDMgr::NextWLPicID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_PICTURE), ++m_nWLPicStart);
	return strRet;
}

CString CIDMgr::NextWLRichEditID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_RICHEDIT), ++m_nWLRichEditStart);
	return strRet;
}

CString CIDMgr::NextWLIMID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_WL_RICHEDIT_IM), ++m_nWLIMStart);
	return strRet;
}

CString CIDMgr::NextTaskWndMgrID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_TASK_WND_MGR), ++m_nTaskWndMgrStart);
	return strRet;
}

CString CIDMgr::NextPnlDockID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_PNL_DOCK), ++m_nPnlDockStart);
	return strRet;
}

CString CIDMgr::NextPnlUniformGridID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_UNIFORM_GRID), ++m_nPnlUniformGridStart);
	return strRet;
}

CString CIDMgr::NextPnlStackID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_PNL_STACK), ++m_nPnlStackStart);
	return strRet;
}

CString CIDMgr::NextConditionFontChildID(FONTRESOURCEITEM *pResFont)
{
	if (pResFont == NULL)
	{
		ASSERT(FALSE);
		return _T("");
	}

	CString strID;
	int nIndex = 0;
	while (true)
	{
		// Generate a new ID
		strID.Format(_T("Condition%d"), ++nIndex);

		std::map<CString, RESFONTPROPERTIES *>::iterator itFound = pResFont->m_mapConditionFont.find(strID);
		if (itFound == pResFont->m_mapConditionFont.end())	// not found, the new ID is OK.
		{
			break;
		}
	}

	return strID;
}

CString CIDMgr::NextFontResID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RESOURCE_GENERAL_FONT), ++m_nFontResStart);
	return strRet;
}

CString CIDMgr::NextColorResID()
{
	CString strRet;
	strRet.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RESOURCE_COLOR), ++m_nColorResStart);
	return strRet;
}

int CIDMgr::NextControlIDValue()
{
	if (m_nNextControlID == -1)
	{
		m_nNextControlID = CUIMgr::GetMaxIDValue();
	}

	m_nNextControlID++;

	return m_nNextControlID;
}
