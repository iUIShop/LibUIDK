#include "StdAfx.h"
#include "TransformDialog.h"
#include "IDMgr.h"

// for parse xml
#if(_MSC_VER > 1200) // VC6.0
#import <msxml6.dll> rename_namespace("XML")
#else
#include <MsXml2.h>
#import <msxml3.dll> rename_namespace("XML")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString g_vControlType[] =
{
	_T("STYLE"),
	_T("EXSTYLE"),
	_T("CAPTION"),
	_T("MENU"),
	_T("FONT"),

	// CONTROL_TYPE 	text, id, x, y, width, height [, style [, extended-style]]
	_T("DEFPUSHBUTTON"),
	_T("PUSHBUTTON"),
	_T("LTEXT"),
	_T("CTEXT"),
	_T("RTEXT"),
	_T("GROUPBOX"),
	_T("AUTO3STATE"),
	_T("AUTOCHECKBOX"),
	_T("AUTORADIOBUTTON"),
	_T("CHECKBOX"),
	_T("PUSHBOX"),
	_T("RADIOBUTTON"),
	_T("STATE3")

	// ICON 		text, id, x, y, [[width, height, style [[, extended-style]]]]
	_T("ICON"),

	// CONTORL_TYPE 	id, x, y, width, height [[, style [[, extended-style]]]]
	_T("EDITTEXT"),
	_T("COMBOBOX"),
	_T("LISTBOX"),
	_T("SCROLLBAR"),

	// CONTROL 	text, id, class, style, x, y, width, height [, extended-style]
	_T("CONTROL")
};

int g_nControlTypeCount = sizeof(g_vControlType) / sizeof(CString);

CTransformDialog::CTransformDialog(void)
{
	m_nCurrentLine = 0;
	m_bHasTabControl = FALSE;
	m_bHasGroupBox = FALSE;
}

CTransformDialog::~CTransformDialog(void)
{
}

// PURPOSE:  Resource name callback
BOOL EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LPARAM lParam)
{
	ASSERT(lpszType == RT_DIALOG);
	ASSERT(!((ULONG)lpszName >> 16));

	CTransformDialog *pTd = (CTransformDialog *)lParam;

	HRSRC hRes = ::FindResource(hModule, lpszName, lpszType);
	if (hRes == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hModule, hRes);
	if (NULL == hGlobal)
		return FALSE;

	CDialog dlg;	// A temporary object, will be destructor at the end of this block
	BOOL bRet = dlg.CreateIndirect(hGlobal);
	if (!bRet)
	{
		CString strMsg;
		strMsg.Format(_T("Failed to parse dialog %d"), (ULONG)lpszName);
		AfxMessageBox(strMsg);
	}
	else
	{
		// Generate a new UIWnd
		UIWNDPROPERTIES *pUIWnd = new UIWNDPROPERTIES;

		// ID
		pUIWnd->m_strID = CIDMgr::NextWindowID();

		// Background
		pUIWnd->m_strBKNo = _T("TransformBk.bmp");

		// Minimize maximize and close box
		LONG lStyleDlg = GetWindowLong(dlg.GetSafeHwnd(), GWL_STYLE);
		if (lStyleDlg & WS_CAPTION)
		{
			pUIWnd->m_lTitleBarHeight = 30;
			// Add caption control
			CString strCaption;
			dlg.GetWindowText(strCaption);
			if (!strCaption.IsEmpty())
			{
				TXTPROPERTIES *pCaptionCtrl = new TXTPROPERTIES;
				pCaptionCtrl->m_strID = _T("IDC_STA_CAPTION");
				RECT rcCtrl = {5, 2, 150, 27};
				pCaptionCtrl->m_rcControl = rcCtrl;
				pCaptionCtrl->m_strCaption = strCaption;

				pUIWnd->m_vControls.push_back(pCaptionCtrl);
			}

			if (lStyleDlg & WS_SYSMENU)			// Has close box
			{
				CRect rcDlg;
				dlg.GetWindowRect(rcDlg);

				if (lStyleDlg & WS_MINIMIZEBOX)	// Has minimize box
				{
					BTNPROPERTIES *pMinBtn = new BTNPROPERTIES;
					pMinBtn->m_strID = _T("IDC_BTN_MIN");
					RECT rcCtrl = {rcDlg.Width() - 80, 2, rcDlg.Width() - 55, 27};
					pMinBtn->m_rcControl = rcCtrl;
					pMinBtn->m_eBtnType = BT_MINIMIZEBOX;

					pUIWnd->m_vControls.push_back(pMinBtn);
				}
				if (lStyleDlg & WS_MAXIMIZEBOX)	// Has maximize box
				{
					CHKPROPERTIES *pMaxBox = new CHKPROPERTIES;
					pMaxBox->m_strID = _T("IDC_CHK_MAX");
					RECT rcCtrl = {rcDlg.Width() - 55, 2, rcDlg.Width() - 30, 27};
					pMaxBox->m_rcControl = rcCtrl;
					pMaxBox->m_eChkType = CT_MAXIMIZEBOX;

					pUIWnd->m_vControls.push_back(pMaxBox);
				}

				// Add close box
				BTNPROPERTIES *pCloseBtn = new BTNPROPERTIES;
				pCloseBtn->m_strID = _T("IDC_BTN_CLOSE");
				RECT rcCtrl = {rcDlg.Width() - 30, 2, rcDlg.Width() - 5, 27};
				pCloseBtn->m_rcControl = rcCtrl;
				pCloseBtn->m_eBtnType = BT_CLOSEBOX;

				pUIWnd->m_vControls.push_back(pCloseBtn);
			}
		}

		pTd->m_vpWindows.push_back(pUIWnd);

		CWnd *pChild = dlg.GetWindow(GW_CHILD);
		while (pChild != NULL)
		{
			// Class name
			TCHAR szClassName[MAX_PATH] = {0};
			GetClassName(pChild->GetSafeHwnd(), szClassName, MAX_PATH);
			CString strClassName = szClassName;

			// Cation
			CString strText;
			pChild->GetWindowText(strText);

			CTRLPROPERTIES *pCtrlProp = NULL;
			if (strClassName == WC_BUTTON)
			{
				LONG lStyle = GetWindowLong(pChild->GetSafeHwnd(), GWL_STYLE);

				// The right 4-bit style value defined the button type.
				LONG lType = lStyle << 24;
				lType = lType >> 24;
				if (lType == BS_GROUPBOX)
				{
					// Ignore group box.
				}
				else if (lType == BS_AUTOCHECKBOX)	// BS_GROUPBOX is 0111, BS_AUTOCHECKBOX is 0011, so first test group box.
				{
					pCtrlProp = new CHKPROPERTIES;
					pCtrlProp->m_strID = CIDMgr::NextCheckBoxID();
				}
				else if (lType == BS_AUTORADIOBUTTON)
				{
					pCtrlProp = new RADPROPERTIES;
					pCtrlProp->m_strID = CIDMgr::NextRadioButtonID();
				}
				else if (lType == BS_DEFPUSHBUTTON)
				{
					pCtrlProp = new BTNPROPERTIES;
					pCtrlProp->m_strID = CIDMgr::NextButtonID();
					((BTNPROPERTIES *)pCtrlProp)->m_bDefPushButton = true;
				}
				else
				{
					pCtrlProp = new BTNPROPERTIES;
					pCtrlProp->m_strID = CIDMgr::NextButtonID();
				}

				if (pCtrlProp != NULL)
					((BTNBASEPROPERTIES *)pCtrlProp)->m_strCaption = strText.GetBuffer(0);
			}
			else if (strClassName == WC_COMBOBOX)
			{
				pCtrlProp = new CMBPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextComboBoxID();
			}
			else if (strClassName == WC_STATIC)
			{
				pCtrlProp = new TXTPROPERTIES;

				pCtrlProp->m_strID = CIDMgr::NextStaticID();
				((TXTPROPERTIES *)pCtrlProp)->m_strCaption = strText.GetBuffer(0);
			}
			else if (strClassName == WC_EDIT)
			{
				pCtrlProp = new EDTPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextEditID();
			}
			else if (strClassName == TRACKBAR_CLASS)
			{
				pCtrlProp = new SLDEXPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextSliderExID();
			}
			else if (strClassName == PROGRESS_CLASS)
			{
				pCtrlProp = new PRGPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextProgressID();
			}
			else if (strClassName == UPDOWN_CLASS)
			{
				pCtrlProp = new SPNPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextSpinID();
			}
			else if (strClassName == HOTKEY_CLASS)
			{
				pCtrlProp = new HOTPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextHotKeyID();
			}
			else if (strClassName == WC_IPADDRESS)
			{
				pCtrlProp = new IPAPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextIPAddressID();
			}
			else if (strClassName == WC_LISTVIEW || strClassName == WC_LISTBOX)
			{
				pCtrlProp = new LSTCTRLPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextListCtrlID();
			}
			else if (strClassName == WC_TREEVIEW)
			{
				pCtrlProp = new TREECTRLPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextTreeCtrlID();
			}
			else if (strClassName == WC_SCROLLBAR)
			{
				pCtrlProp = new SBEXPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextScrollBarExID();
			}
			else if (strClassName == DATETIMEPICK_CLASS)
			{
				pCtrlProp = new DATETIMECTRLPROPERTIES;
				pCtrlProp->m_strID = CIDMgr::NextDateTimeCtrlID();
			}
			else
			{
				TRACE(_T("LibUIDK===Not support this control type\n"));
				pTd->m_vNotSupportedControlType.push_back(strClassName);
				ASSERT(FALSE);
			}

			if (pCtrlProp != NULL)
			{
				// Visible
				BOOL bVisible = pChild->IsWindowVisible();
				pCtrlProp->m_bVisible = bVisible ? true : false;

				// Disabled
				BOOL bEnabled = pChild->IsWindowEnabled();
				pCtrlProp->m_bDisabled = bEnabled ? false : true;

				// Rect
				RECT rect;
				pChild->GetWindowRect(&rect);
				dlg.ScreenToClient(&rect);

				// Offset the height of title bar
				if (lStyleDlg & WS_CAPTION)
				{
					rect.bottom += 30;
					rect.top += 30;
				}

				pCtrlProp->m_rcControl = rect;

				pUIWnd->m_vControls.push_back(pCtrlProp);
			}

			pChild = pChild->GetWindow(GW_HWNDNEXT);
		}
	}


	// MSDN: Use the CWnd::DestroyWindow function to destroy a dialog box created by the CreateIndirect function.
	// CDialog::~CDialog() call the DestroyWindow, So, we needn't call CWnd::DestroyWindow.
	// dlg.DestroyWindow();

	return TRUE;
}

// PURPOSE:  Resource type callback
BOOL EnumResTypeProc(HMODULE hModule, LPTSTR lpszType, LPARAM lParam)
{
	CTransformDialog *pTd = (CTransformDialog *)lParam;

	// The type may be a string or an unsigned decimal integer, so test it.
	if ((ULONG)lpszType & 0xFFFF0000)
	{
		// The lpszName is a string.
	}
	else
	{
		// The lpszName is an unsigned decimal integer
		if (lpszType == RT_DIALOG)
		{
			// Find the names of all resources of type lpszType.
			EnumResourceNames(hModule, lpszType, (ENUMRESNAMEPROC)EnumResNameProc, lParam);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//

int CTransformDialog::TransformExe(LPCTSTR lpszSourcePath)
{
	// Load the .EXE whose resources you want to list.
	HMODULE hExe = LoadLibrary(lpszSourcePath);
	if (hExe == NULL)
	{
		// Add code to fail as securely as possible.
		return -2;
	}

	EnumResourceTypes(hExe, (ENUMRESTYPEPROC)EnumResTypeProc, (LPARAM)this);

	// Unload the executable file whose resources were enumerated.
	FreeLibrary(hExe);

	return 0;
}

int CTransformDialog::TransformRC(LPCTSTR lpszSourcePath)
{
	return LoadResourceFile(lpszSourcePath);
}

int CTransformDialog::TransformDsp(LPCTSTR lpszSourcePath)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// protected for load from .RC file

int CTransformDialog::ParseDlgStyle(const CString &strStyles, /*[out]*/ DWORD &dwStyle)
{
	CString str = strStyles;
	PreReadNextLine(str);
	str.MakeUpper();

	// Remove STYLE header
	str.Delete(0, 5);

	return ParseStyleString(str, dwStyle);
}

int CTransformDialog::ParseDlgExStyle(const CString &strExStyles, /*[out]*/ DWORD &dwExStyle)
{
	CString str = strExStyles;
	PreReadNextLine(str);
	str.MakeUpper();

	// Remove EXSTYLE header
	str.Delete(0, 7);

	int i = 0;
	CString strExStyle;
	while (AfxExtractSubString(strExStyle, str, i++, '|'))
	{
		strExStyle.TrimLeft();
		strExStyle.TrimRight();

		int nID = StringIDToInt(strExStyle);
		if (nID != -1)
			dwExStyle |= nID;
	}

	return 0;
}

int CTransformDialog::ParseDlgFont(const CString &strFont, LOGFONT *plf)
{
	if (plf == NULL)
		return -1;

	memset(plf, 0, sizeof(LOGFONT));
	plf->lfWeight = 400;
	plf->lfCharSet = 134;

	// Remove "FONT"
	CString str = strFont;
	int nPos = str.Find(_T("FONT"));
	if (nPos < 0)
	{
		ASSERT(FALSE);
		return -1;
	}

	str.Delete(nPos, 4);

	// Get font size
	CString strEle;
	BOOL bRet = AfxExtractSubString(strEle, str, 0, ',');
	if (bRet)
	{
		strEle.TrimLeft();
		strEle.TrimRight();
		int nPointSize = IntFromString(strEle);
		CWindowDC dc(NULL);
		plf->lfHeight = -MulDiv(nPointSize, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 72);
	}

	// Get face name
	bRet = AfxExtractSubString(strEle, str, 1, ',');
	if (bRet)
	{
		strEle.TrimLeft();
		strEle.TrimRight();
		TSTRCPY(plf->lfFaceName, LF_FACESIZE, strEle);
	}

	return 0;
}

// Make dialog coordinate from dialog to pixel. see depot\Document\MFC Controls\RC\对话框坐标单位.txt
int CTransformDialog::MapRect(const LOGFONT &lf, RECT &rect)
{
	CFont font;
	font.CreateFontIndirect(&lf);

	CWindowDC dc(NULL);
	CFont *pOldFont = dc.SelectObject(&font);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	int baseUnitY = tm.tmHeight;
	CSize size = dc.GetTextExtent(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), 52);
	int baseUnitX = (size.cx / 26 + 1) / 2;
	dc.SelectObject(pOldFont);

	rect.left = MulDiv(rect.left, baseUnitX, 4);
	rect.top = MulDiv(rect.top, baseUnitY, 8);
	rect.right = MulDiv(rect.right, baseUnitX, 4);
	rect.bottom = MulDiv(rect.bottom, baseUnitY, 8);

	return 0;
}

int CTransformDialog::ParseChildren(UIWNDPROPERTIES *pUIWnd, const LOGFONT &lf)
{
	std::string strA;
	while (m_ResourceFile.ReadStringA(strA))
	{
		++m_nCurrentLine;

		CString str = (LPCTSTR)_bstr_t(strA.c_str());
		FormatString(str);
		str.TrimLeft();
		str.TrimRight();

		if (str.CompareNoCase(_T("END")) == 0)	// Not include children.
		{
			break;
		}

		// Pre-Read next line, if next line belongs to this line, connect the two lines.
		PreReadNextLine(str);

		CString strControlType;
		AfxExtractSubString(strControlType, str, 0, 0x20);

		str.Delete(0, strControlType.GetLength());

		// CONTROL_TYPE text, id, x, y, width, height [, style [, extended-style]]
		if (strControlType.CompareNoCase(_T("DEFPUSHBUTTON")) == 0
			|| strControlType.CompareNoCase(_T("PUSHBUTTON")) == 0
			|| strControlType.CompareNoCase(_T("LTEXT")) == 0
			|| strControlType.CompareNoCase(_T("CTEXT")) == 0
			|| strControlType.CompareNoCase(_T("RTEXT")) == 0
			|| strControlType.CompareNoCase(_T("GROUPBOX")) == 0
			|| strControlType.CompareNoCase(_T("AUTO3STATE")) == 0
			|| strControlType.CompareNoCase(_T("AUTOCHECKBOX")) == 0
			|| strControlType.CompareNoCase(_T("AUTORADIOBUTTON")) == 0
			|| strControlType.CompareNoCase(_T("CHECKBOX")) == 0
			|| strControlType.CompareNoCase(_T("PUSHBOX")) == 0
			|| strControlType.CompareNoCase(_T("RADIOBUTTON")) == 0
			|| strControlType.CompareNoCase(_T("STATE3")) == 0)
		{
			CString strCaption;
			CString strID;
			RECT rect;
			DWORD dwStyle = 0;
			ParseChild(str, strCaption, strID, &rect, dwStyle);
			MapRect(lf, rect);

			if (strControlType.CompareNoCase(_T("DEFPUSHBUTTON")) == 0)
			{
				BTNPROPERTIES *pBtnProp = new BTNPROPERTIES;
				pUIWnd->m_vControls.push_back(pBtnProp);
				pBtnProp->m_strID = strID;
				pBtnProp->m_strCaption = strCaption;
				pBtnProp->m_rcControl = rect;
			}
			else if (strControlType.CompareNoCase(_T("PUSHBUTTON")) == 0)
			{
				BTNPROPERTIES *pBtnProp = new BTNPROPERTIES;
				pUIWnd->m_vControls.push_back(pBtnProp);
				pBtnProp->m_strID = strID;
				pBtnProp->m_strCaption = strCaption;
				pBtnProp->m_rcControl = rect;
			}
			else if (strControlType.CompareNoCase(_T("LTEXT")) == 0)
			{
				TXTPROPERTIES *pTxtProp = new TXTPROPERTIES;
				pUIWnd->m_vControls.push_back(pTxtProp);
				pTxtProp->m_strID = strID;
				pTxtProp->m_strCaption = strCaption;
				pTxtProp->m_rcControl = rect;
			}
			else if (strControlType.CompareNoCase(_T("CTEXT")) == 0)
			{
				TXTPROPERTIES *pTxtProp = new TXTPROPERTIES;
				pUIWnd->m_vControls.push_back(pTxtProp);
				pTxtProp->m_strID = strID;
				pTxtProp->m_strCaption = strCaption;
				pTxtProp->m_rcControl = rect;
				pTxtProp->m_eTextHorAlignMode = TAH_CENTER;
			}

			else if (strControlType.CompareNoCase(_T("RTEXT")) == 0)
			{
				TXTPROPERTIES *pTxtProp = new TXTPROPERTIES;
				pUIWnd->m_vControls.push_back(pTxtProp);
				pTxtProp->m_strID = strID;
				pTxtProp->m_strCaption = strCaption;
				pTxtProp->m_rcControl = rect;
				pTxtProp->m_eTextHorAlignMode = TAH_RIGHT;
			}
			else if (strControlType.CompareNoCase(_T("GROUPBOX")) == 0)
			{
				m_bHasGroupBox = TRUE;
			}
			else
			{
				m_vNotSupportedControlType.push_back(strControlType);
				ASSERT(FALSE);
			}
		}
		// CONTROL_TYPE id, x, y, width, height [[, style [[, extended-style]]]]
		else if (strControlType.CompareNoCase(_T("EDITTEXT")) == 0
			|| strControlType.CompareNoCase(_T("COMBOBOX")) == 0
			|| strControlType.CompareNoCase(_T("LISTBOX")) == 0
			|| strControlType.CompareNoCase(_T("SCROLLBAR")) == 0)
		{
			CString strID;
			RECT rect;
			DWORD dwStyle = 0;
			ParseChildWithoutCpation(str, strID, &rect, dwStyle);
			MapRect(lf, rect);

			if (strControlType.CompareNoCase(_T("EDITTEXT")) == 0)
			{
				EDTPROPERTIES *pEdtProp = new EDTPROPERTIES();
				pUIWnd->m_vControls.push_back(pEdtProp);
				pEdtProp->m_strID = strID;
				pEdtProp->m_rcControl = rect;
				if (dwStyle & ES_AUTOHSCROLL)
					pEdtProp->m_bAutoHScroll = true;
				if (dwStyle & ES_AUTOVSCROLL)
					pEdtProp->m_bAutoVScroll = true;
				if (dwStyle & ES_LOWERCASE)
					pEdtProp->m_bLowercase = true;
				if (dwStyle & ES_UPPERCASE)
					pEdtProp->m_bUppercase = true;
				if (dwStyle & ES_NUMBER)
					pEdtProp->m_bNumber = true;
				if (dwStyle & ES_PASSWORD)
					pEdtProp->m_bPassword = true;
				if (dwStyle & ES_READONLY)
					pEdtProp->m_bReadOnly = true;
				if (dwStyle & ES_MULTILINE)
					pEdtProp->m_bMultiline = true;
				if (dwStyle & ES_WANTRETURN)
					pEdtProp->m_bWantReturn = true;
			}
			else if (strControlType.CompareNoCase(_T("COMBOBOX")) == 0)
			{
				CMBPROPERTIES *pCmbProp = new CMBPROPERTIES();
				pUIWnd->m_vControls.push_back(pCmbProp);
				pCmbProp->m_strID = strID;

				// Adjust the rect
				CRect rcCombBox = rect;
				if (dwStyle & CBS_DROPDOWNLIST)
				{
					rcCombBox.bottom = rcCombBox.top + 24;
					pCmbProp->m_rcControl = rcCombBox;
					pCmbProp->m_lDropListHeight = (LONG)abs(rect.bottom - rcCombBox.bottom);
					pCmbProp->m_eComboStyle = CS_DROPDOWNLIST;
				}
				else if (dwStyle & CBS_DROPDOWN)
				{
					rcCombBox.bottom = rcCombBox.top + 24;
					pCmbProp->m_rcControl = rcCombBox;
					pCmbProp->m_lDropListHeight = (LONG)abs(rect.bottom - rcCombBox.bottom);
					pCmbProp->m_eComboStyle = CS_DROPDOWN;
				}
				else if (dwStyle & CBS_SIMPLE)
				{
					pCmbProp->m_rcControl = rect;
					pCmbProp->m_eComboStyle = CS_SIMPLE;
				}
				else
				{
					ASSERT(FALSE);
				}
			}
			else if (strControlType.CompareNoCase(_T("LISTBOX")) == 0)
			{
				LSTCTRLPROPERTIES *pListProp = new LSTCTRLPROPERTIES();
				pUIWnd->m_vControls.push_back(pListProp);
				pListProp->m_strID = strID;
				pListProp->m_rcControl = rect;
				pListProp->m_eListStyle = LSTS_REPORT;
			}
			else if (strControlType.CompareNoCase(_T("SCROLLBAR")) == 0)
			{
				SBEXPROPERTIES *pSbExProp = new SBEXPROPERTIES();
				pUIWnd->m_vControls.push_back(pSbExProp);
				pSbExProp->m_strID = strID;
				pSbExProp->m_rcControl = rect;
			}
		}
		// CONTROL_TYPE text, id, x, y, [[width, height, style [[, extended-style]]]]
		else if (strControlType.CompareNoCase(_T("ICON")) == 0)
		{
			CString strResID;
			CString strID;
			RECT rect;
			DWORD dwStyle = 0;
			ParseChild(str, strResID, strID, &rect, dwStyle);
			MapRect(lf, rect);

			//
			PICWLPROPERTIES *pPicProp = new PICWLPROPERTIES;
			pUIWnd->m_vControls.push_back(pPicProp);
			pPicProp->m_strID = strID;
			pPicProp->m_rcControl = rect;
		}
		else if (strControlType.CompareNoCase(_T("CONTROL")) == 0)
		{
			ParseCONTROLControl(str, pUIWnd, lf);
		}
		else
		{
			m_vNotSupportedControlType.push_back(strControlType);
			ASSERT(FALSE);
		}
	}

	return 0;
}

int CTransformDialog::ParseChild(const CString &strLine, CString &strCaption, CString &strID, LPRECT lpRect, DWORD &dwStyle)
{
	if (lpRect == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	CString str = strLine;
	str.TrimLeft();
	str.TrimRight();

	int nLen = str.GetLength();
	BOOL bYinHaoBeforDouHao = FALSE;
	int nNextElementBeginIndex = 0;
	int nEleIndex = 0;
	for (int i=0; i<nLen; ++i)
	{
		TCHAR ch = str[i];

		if (ch == ',')
		{
			// Douhao only valid while no Yinhao before it.
			if (!bYinHaoBeforDouHao)
			{
				CString strElement = str.Mid(nNextElementBeginIndex, i - nNextElementBeginIndex);
				nNextElementBeginIndex = i + 1;

				if (nEleIndex == 0)
				{
					strCaption = strElement;
					RemoveQuotationMarks(strCaption, TRUE);
				}
				else if (nEleIndex == 1)
				{
					strID = strElement;
				}
				else if (nEleIndex == 2)
				{
					lpRect->left = IntFromString(strElement);
				}
				else if (nEleIndex == 3)
				{
					lpRect->top = IntFromString(strElement);
				}
				else if (nEleIndex == 4)
				{
					lpRect->right = lpRect->left + IntFromString(strElement);
				}
				else if (nEleIndex == 5)
				{
					lpRect->bottom = lpRect->top + IntFromString(strElement);
				}

				nEleIndex++;
			}
		}
		else if (ch ==  '"')
		{
			if (bYinHaoBeforDouHao)
				bYinHaoBeforDouHao = FALSE;
			else
				bYinHaoBeforDouHao = TRUE;
		}

		// If the last char not Douhao
		if (i == nLen -1)
		{
			CString strElement = str.Mid(nNextElementBeginIndex, i - nNextElementBeginIndex + 1);
			nNextElementBeginIndex = i;

			if (nEleIndex == 5)
			{
				lpRect->bottom = lpRect->top + IntFromString(strElement);
			}
			else if (nEleIndex == 6)
			{
				ParseDlgStyle(strElement, dwStyle);
			}

			nEleIndex++;
		}
	}

	return 0;
}

int CTransformDialog::ParseChildWithoutCpation(const CString &strLine, CString &strID, LPRECT lpRect, DWORD &dwStyle)
{
	if (lpRect == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	CString str = strLine;
	str.TrimLeft();
	str.TrimRight();

	int nLen = str.GetLength();
	BOOL bYinHaoBeforDouHao = FALSE;
	int nNextElementBeginIndex = 0;
	int nEleIndex = 0;
	for (int i=0; i<nLen; ++i)
	{
		TCHAR ch = str[i];

		if (ch == ',')
		{
			// Douhao only valid while no Yinhao before it.
			if (!bYinHaoBeforDouHao)
			{
				CString strElement = str.Mid(nNextElementBeginIndex, i - nNextElementBeginIndex);
				nNextElementBeginIndex = i + 1;

				if (nEleIndex == 0)
				{
					strID = strElement;
				}
				else if (nEleIndex == 1)
				{
					lpRect->left = IntFromString(strElement);
				}
				else if (nEleIndex == 2)
				{
					lpRect->top = IntFromString(strElement);
				}
				else if (nEleIndex == 3)
				{
					lpRect->right = lpRect->left + IntFromString(strElement);
				}
				else if (nEleIndex == 4)
				{
					lpRect->bottom = lpRect->top + IntFromString(strElement);
				}
				else if (nEleIndex == 5)
				{
					ParseStyleString(strElement, dwStyle);
				}

				nEleIndex++;
			}
		}
		else if (ch ==  '"')
		{
			if (bYinHaoBeforDouHao)
				bYinHaoBeforDouHao = FALSE;
			else
				bYinHaoBeforDouHao = TRUE;
		}

		// If the last char not Douhao
		if (i == nLen -1)
		{
			CString strElement = str.Mid(nNextElementBeginIndex, i - nNextElementBeginIndex + 1);
			nNextElementBeginIndex = i;

			if (nEleIndex == 4)
			{
				lpRect->bottom = lpRect->top + IntFromString(strElement);
			}
			else if (nEleIndex == 5)
			{
				ParseStyleString(strElement, dwStyle);
			}
			else if (nEleIndex == 6)
			{
			//	ParseStyleString(strElement, dwExStyle);
			}

			nEleIndex++;
		}
	}

	return 0;
}

int CTransformDialog::LoadIDDefined(LPCTSTR lpszID, LPCTSTR lpszType)
{
	m_vIDs.clear();
	m_fileStyleDefined.Detach();

	BOOL bRet = m_fileStyleDefined.OpenResource(lpszID, lpszType);
	if (!bRet)
	{
		ASSERT(FALSE);
		return -1;
	}

	std::string strA;
	while (m_fileStyleDefined.ReadStringA(strA))
	{
		CString str = (LPCTSTR)_bstr_t(strA.c_str());
		int nTest = str.Find(_T("#define"));
		if (nTest != -1)
		{
			str.TrimRight();

			// the rightest space or tab
			int nRight = str.ReverseFind(' ');
			int nLastTab = str.ReverseFind('\t');
			nRight = max(nRight, nLastTab);

			CString strID;
			strID = str.Right(str.GetLength()-nRight -1);

			str.TrimLeft();
			str.TrimRight(strID);
			str.TrimRight();
			str.TrimLeft(_T("#define"));
			str.TrimLeft();

			std::pair<CString, int> aID;
			aID.first = str;
			if (str == _T("WS_POPUP"))
			{
				int n = 0;
			}
			aID.second = UINTFromHexString(strID);
			m_vIDs.push_back(aID);
		}
	}

	m_fileStyleDefined.Detach();

	return 0;
}

int CTransformDialog::StringIDToInt(LPCTSTR lpszIDs)
{
	int nRet = -1;

	size_t nIDs = m_vIDs.size();
	for (size_t i=0; i<nIDs; ++i)
	{
		std::pair<CString, int> aID = m_vIDs[i];
		if (aID.first == lpszIDs)
		{
			nRet = aID.second;
			break;
		}
	}

	return nRet;
}

int CTransformDialog::PreReadNextLine(CString &str)
{
	// Pre-read next line.
	std::string strA;
	ULONGLONG dwCurPos = m_ResourceFile.GetPosition();
	while (m_ResourceFile.ReadStringA(strA))
	{
		++m_nCurrentLine;
		ULONGLONG dwAfterReadPos = m_ResourceFile.GetPosition();

		CString strNextLine = (LPCTSTR)_bstr_t(strA.c_str());
		FormatString(strNextLine);
		strNextLine.TrimLeft();
		strNextLine.TrimRight();

		if (strNextLine.CompareNoCase(_T("END")) == 0 || strNextLine.CompareNoCase(_T("BEGIN")) == 0)
		{
			m_ResourceFile.Seek(dwCurPos, SEEK_SET);
			--m_nCurrentLine;
			break;
		}

		CString strNextControlType;
		AfxExtractSubString(strNextControlType, strNextLine, 0, 0x20);

		BOOL bFoundControlType = FALSE;
		for (int i = 0; i < g_nControlTypeCount ; i++)
		{
			if (strNextControlType.CompareNoCase(g_vControlType[i]) == 0)
			{
				bFoundControlType = TRUE;
				break;
			}
		}
		if (bFoundControlType)
		{
			m_ResourceFile.Seek(dwCurPos, SEEK_SET);
			--m_nCurrentLine;

			break;
		}
		// If next line belongs to previous line.
		else
		{
			str += strNextLine;
			dwCurPos = dwAfterReadPos;
		}
	}

	return 0;
}

int CTransformDialog::ParseCONTROLControl(const CString &str, UIWNDPROPERTIES *pUIWnd, const LOGFONT &lf)
{
	CString strLine = str;
	FormatString(strLine);
	strLine.TrimLeft();
	strLine.TrimRight();

	// Parse CONTROL, the order is Caption, ID, Type, Style, Rect, ExStyle
	int nLen = strLine.GetLength();
	BOOL bYinHaoBeforDouHao = FALSE;
	int nNextElementBeginIndex = 0;
	int nEleIndex = 0;

	CString strCaption;
	CString strID;
	CString strType;
	CRect rect;
	DWORD dwStyle = 0;
	DWORD dwExStyle = 0;
	for (int i=0; i<nLen; ++i)
	{
		TCHAR ch = strLine[i];

		if (ch == ',')
		{
			// Douhao only valid while no Yinhao before it.
			if (!bYinHaoBeforDouHao)
			{
				CString strElement = strLine.Mid(nNextElementBeginIndex, i - nNextElementBeginIndex);
				nNextElementBeginIndex = i + 1;	// The begin of next element

				if (nEleIndex == 0)
				{
					strCaption = strElement;
					RemoveQuotationMarks(strCaption, TRUE);
				}
				else if (nEleIndex == 1)
				{
					strID = strElement;
				}
				else if (nEleIndex == 2)
				{
					strType = strElement;
				}
				else if (nEleIndex == 3)
				{
					ParseStyleString(strElement, dwStyle);
				}
				else if (nEleIndex == 4)
				{
					rect.left = IntFromString(strElement);
				}
				else if (nEleIndex == 5)
				{
					rect.top = IntFromString(strElement);
				}
				else if (nEleIndex == 6)
				{
					rect.right = rect.left + IntFromString(strElement);
				}
				else if (nEleIndex == 7)
				{
					rect.bottom = rect.top + IntFromString(strElement);
				}

				nEleIndex++;
			}
		}
		else if (ch == '"')
		{
			if (bYinHaoBeforDouHao)
				bYinHaoBeforDouHao = FALSE;
			else
				bYinHaoBeforDouHao = TRUE;
		}

		// If the last char not Douhao
		if (i == nLen -1)
		{
			CString strElement = strLine.Mid(nNextElementBeginIndex, i - nNextElementBeginIndex + 1);
			nNextElementBeginIndex = i;

			if (nEleIndex == 7)
			{
				rect.bottom = rect.top + IntFromString(strElement);
			}
			else if (nEleIndex == 8)
			{
				ParseStyleString(strElement, dwExStyle);
			}

			nEleIndex++;
		}
	}

	// Create control
	MapRect(lf, rect);

	if (strType.CompareNoCase(_T("\"Static\"")) == 0)
	{
		if ((dwStyle & SS_BITMAP) == SS_BITMAP)	// Picture
		{
			TXTPROPERTIES *pTxtProp = new TXTPROPERTIES();
			pUIWnd->m_vControls.push_back(pTxtProp);
			pTxtProp->m_strID = strID;
			pTxtProp->m_rcControl = rect;
		}
	}
	else if (strType.CompareNoCase(_T("\"SysTreeView32\"")) == 0)
	{
		TREECTRLPROPERTIES *pTreeProp = new TREECTRLPROPERTIES();
		pUIWnd->m_vControls.push_back(pTreeProp);
		pTreeProp->m_strID = strID;
		pTreeProp->m_rcControl = rect;
		if (dwStyle & TVS_HASBUTTONS)
			pTreeProp->m_bHasButtons = true;
		if (dwStyle & TVS_HASLINES)
			pTreeProp->m_bHasLines = true;
		if (dwStyle & TVS_LINESATROOT)
			pTreeProp->m_bLinesAtRoot = true;
		if (dwStyle & TVS_EDITLABELS)
			pTreeProp->m_bEditLabels = true;
		if (dwStyle & TVS_FULLROWSELECT)
			pTreeProp->m_bFullRowSel = true;
		if (dwStyle & TVS_DISABLEDRAGDROP)
			pTreeProp->m_bDisableDragDrop = true;
		if (dwStyle & TVS_SHOWSELALWAYS)
			pTreeProp->m_bShowSelAlways = true;
		if (dwStyle & WS_TABSTOP)
			pTreeProp->m_bTabStop = true;
		if (dwStyle & TVS_INFOTIP)
			pTreeProp->m_bInfoTip = true;
		if (dwStyle & TVS_CHECKBOXES)
			pTreeProp->m_bCheckBoxes = true;
		if (dwStyle & TVS_TRACKSELECT)
			pTreeProp->m_bTrackSel = true;
		if (dwStyle & TVS_SINGLEEXPAND)
			pTreeProp->m_bSingleExpand = true;
		if (dwStyle & TVS_NONEVENHEIGHT)
			pTreeProp->m_bNonEvenHeight = true;
		if (dwStyle & TVS_NOSCROLL)
			pTreeProp->m_bScroll = false;
		else
			pTreeProp->m_bScroll = true;
		if (dwStyle & TVS_NOTOOLTIPS)
			pTreeProp->m_bToolTips = false;
		else
			pTreeProp->m_bToolTips = true;
	}
	else if (strType.CompareNoCase(_T("\"SysListView32\"")) == 0)
	{
		LSTCTRLPROPERTIES *pListProp = new LSTCTRLPROPERTIES();
		pUIWnd->m_vControls.push_back(pListProp);
		pListProp->m_strID = strID;
		pListProp->m_rcControl = rect;
		if (dwStyle & LVS_LIST)
			pListProp->m_eListStyle = LSTS_LIST;
		else if (dwStyle & LVS_REPORT)
			pListProp->m_eListStyle = LSTS_REPORT;
		else if (dwStyle & LVS_SMALLICON)
			pListProp->m_eListStyle = LSTS_SMALLICON;
		else
			pListProp->m_eListStyle = LSTS_ICON;

		if (dwStyle & LVS_EDITLABELS)
			pListProp->m_bEditLabels = true;
		if (dwStyle & LVS_SHOWSELALWAYS)
			pListProp->m_bShowSelAlways = true;
		if (dwStyle & LVS_SINGLESEL)
			pListProp->m_bSingleSel = true;
		if (dwStyle & LVS_OWNERDATA)
			pListProp->m_bOwnerData = true;
		if (LVS_EX_TRACKSELECT & dwExStyle)
			pListProp->m_bTrackSelect = true;
		if (LVS_EX_TWOCLICKACTIVATE & dwExStyle)
			pListProp->m_bTwoClickActivate = true;
		if (LVS_EX_FULLROWSELECT & dwExStyle)
			pListProp->m_bFullRowSelect = true;
		if (LVS_EX_SUBITEMIMAGES & dwExStyle)
			pListProp->m_bSubItemImages = true;
		if (dwExStyle & LVS_EX_INFOTIP)
			pListProp->m_bInfoTooltip = true;
		if (dwExStyle & LVS_EX_CHECKBOXES)
			pListProp->m_bCheckBoxes = true;
		if (LVS_AUTOARRANGE & dwStyle)
			pListProp->m_bAutoArrange = true;	// for icon and small icon view
		if (LVS_NOSCROLL & dwStyle)
			pListProp->m_bNoScroll = true;		// This style is not compatible with the LVS_LIST or LVS_REPORT styles
		if (LVS_NOLABELWRAP & dwStyle)
			pListProp->m_bNoLabelWrap = true;	// for icon view
		if (LVS_NOCOLUMNHEADER & dwStyle)
			pListProp->m_bNoHeader = true;
		if (LVS_NOSORTHEADER & dwStyle)
			pListProp->m_bNoSortHeader = true;
		if (LVS_EX_HEADERDRAGDROP & dwExStyle)
			pListProp->m_bHeaderDragDrop = true;
	}
	else if (strType.CompareNoCase(_T("\"Button\"")) == 0)
	{
		if (dwStyle & BS_AUTOCHECKBOX)
		{
			CHKPROPERTIES *pChkProp = new CHKPROPERTIES();
			pUIWnd->m_vControls.push_back(pChkProp);
			pChkProp->m_strCaption = strCaption;
			pChkProp->m_strID = strID;
			pChkProp->m_rcControl = rect;
			if (dwStyle & WS_DISABLED)
				pChkProp->m_bDisabled = true;
		}
		else
		{
			m_vNotSupportedControlType.push_back(strType);
			ASSERT(FALSE);
		}
	}
	else if (strType.CompareNoCase(_T("\"msctls_trackbar32\"")) == 0)
	{
		SLDEXPROPERTIES *pSliderExProp = new SLDEXPROPERTIES();
		pUIWnd->m_vControls.push_back(pSliderExProp);
		pSliderExProp->m_strID = strID;
		pSliderExProp->m_rcControl = rect;
	}
	else if (strType.CompareNoCase(_T("\"SysDateTimePick32\"")) == 0)
	{
		DATETIMECTRLPROPERTIES *pDTProp = new DATETIMECTRLPROPERTIES();
		pUIWnd->m_vControls.push_back(pDTProp);
		pDTProp->m_strID = strID;
		pDTProp->m_rcControl = rect;
		pDTProp->m_eFormat = DTF_SHORT_DATE;
		if (dwStyle & DTS_LONGDATEFORMAT)
		{
			pDTProp->m_eFormat = DTF_LONG_DATE;
		}
		if (dwStyle & DTS_UPDOWN)
		{
			pDTProp->m_bUseSpinControl = TRUE;
			pDTProp->m_eFormat = DTF_TIME;
		}
		if (dwStyle & DTS_SHOWNONE)
		{
			pDTProp->m_bShowNone = TRUE;
		}
		if (dwStyle & DTS_APPCANPARSE)
		{
			pDTProp->m_bAllowEdit = TRUE;
		}
	}
	else if (strType.CompareNoCase(_T("\"msctls_progress32\"")) == 0)
	{
		PRGPROPERTIES *pPrgProp = new PRGPROPERTIES();
		pUIWnd->m_vControls.push_back(pPrgProp);
		pPrgProp->m_strID = strID;
		pPrgProp->m_rcControl = rect;
		if (dwStyle & PBS_VERTICAL)
			pPrgProp->m_bVertical = true;
	}
	else if (strType.CompareNoCase(_T("\"SysIPAddress32\"")) == 0)
	{
		IPAPROPERTIES *pIPAProp = new IPAPROPERTIES();
		pUIWnd->m_vControls.push_back(pIPAProp);
		pIPAProp->m_strID = strID;
		pIPAProp->m_rcControl = rect;
	}
	else if (strType.CompareNoCase(_T("\"msctls_updown32\"")) == 0)
	{
		SPNPROPERTIES *pSpnProp = new SPNPROPERTIES();
		pUIWnd->m_vControls.push_back(pSpnProp);
		pSpnProp->m_strID = strID;
		pSpnProp->m_rcControl = rect;
		if (dwStyle & UDS_SETBUDDYINT)
			pSpnProp->m_bSetBuddyInt = true;
		if (dwStyle & UDS_ARROWKEYS)
			pSpnProp->m_bArrowKeys = true;
		if (dwStyle & UDS_HORZ)
			pSpnProp->m_bVertical = false;
		else
			pSpnProp->m_bVertical = true;
	}
	else if (strType.CompareNoCase(_T("\"RichEdit20A\"")) == 0)
	{
		RICHEDITPROPERTIES *pReProp = new RICHEDITPROPERTIES();
		pUIWnd->m_vControls.push_back(pReProp);
		pReProp->m_strID = strID;
		pReProp->m_rcControl = rect;
	}
	else if (strType.CompareNoCase(_T("\"msctls_hotkey32\"")) == 0)
	{
		HOTPROPERTIES *pHotProp = new HOTPROPERTIES();
		pUIWnd->m_vControls.push_back(pHotProp);
		pHotProp->m_strID = strID;
		pHotProp->m_rcControl = rect;
	}
	else if (strType.CompareNoCase(_T("\"ComboBoxEx32\"")) == 0)
	{
		m_vNotSupportedControlType.push_back(strType);
		ASSERT(FALSE);	// The LibUIDK not include this control.
	}
	else if (strType.CompareNoCase(_T("\"{8856F961-340A-11D0-A96B-00C04FD705A2}\"")) == 0)	// "Microsoft Web Browser" Active control
	{
		HTMLCTRLPROPERTIES *pHtmlCtrl = new HTMLCTRLPROPERTIES();
		pUIWnd->m_vControls.push_back(pHtmlCtrl);
		pHtmlCtrl->m_strID = strID;
		pHtmlCtrl->m_rcControl = rect;
	}
	else if (strType.CompareNoCase(_T("\"SysTabControl32\"")) == 0)
	{
		m_bHasTabControl = TRUE;
	}
	else
	{
		m_vNotSupportedControlType.push_back(strType);
		ASSERT(FALSE);
	}

	return 0;
}

int CTransformDialog::ParseStyleString(const CString &strStyles, DWORD &dwStyle)
{
	int i = 0;
	CString strStyle;
	while (AfxExtractSubString(strStyle, strStyles, i++, '|'))
	{
		strStyle.TrimLeft();
		strStyle.TrimRight();

		int nID = StringIDToInt(strStyle);
		if (nID != -1)
			dwStyle |= nID;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CTransformDialog::LoadResourceFile(LPCTSTR lpszResource)
{
	m_bHasTabControl = FALSE;
	m_bHasGroupBox = FALSE;

	// The dialog and controls style macro defined in WinUser.h, so parse it.
	int nRet = LoadIDDefined(_T("WINUSER_H"), _T("WINUSER"));
	if (nRet != 0)
		return nRet;

	// Load and parse *.rc file.
	FILE *fp = TFOPEN(lpszResource, _T("rb"));
	if (fp == NULL)
	{
		return -1;
	}

	m_ResourceFile.Detach();
	m_vpWindows.clear();

	fseek(fp, 0, SEEK_END);
	int nLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	BYTE *pBuf = new BYTE[nLen];
	memset(pBuf, 0, nLen);
	fread(pBuf, nLen, 1, fp);
	fclose(fp);
	m_ResourceFile.Attach(pBuf, nLen, 0);

	std::string strA;
	while (m_ResourceFile.ReadStringA(strA))
	{
		++m_nCurrentLine;

		CString str = (LPCTSTR)_bstr_t(strA.c_str());
		FormatString(str);
		str.TrimLeft();

		// Ignore empty line
		if (str.IsEmpty())
			continue;

		// Ignore comment line
		if (str.Left(2) == _T("//"))
			continue;

		// Ignore macro line
		if (str.Left(1) == _T("#"))
			continue;

		// Ignore "#
		if (str.Left(2) == _T("\"#"))
			continue;

		// Ignore "\ .
		if (str.Left(2) == _T("\"\\"))
			continue;

		CString strType;
		BOOL bRet = AfxExtractSubString(strType, str, 1, 0x20);
		CString strParam2;
		BOOL bHasParam2 = AfxExtractSubString(strParam2, str, 2, 0x20);

		// Is a dialog
		if (bRet && (strType.CompareNoCase(_T("DIALOG")) == 0 || strType.CompareNoCase(_T("DIALOGEX")) == 0) && bHasParam2 && !strParam2.IsEmpty())
		{
			UIWNDPROPERTIES *pUIWnd = new UIWNDPROPERTIES;
			LOGFONT lf;

			// Remove DISCARDABLE
			CString strCopy = str;
			strCopy.MakeUpper();
			int nPos = strCopy.Find(_T("DISCARDABLE"));
			if (nPos > 0)
				str.Delete(nPos, 11);

			// Parse Dialog ID
			CString strID;
			BOOL bRet = AfxExtractSubString(strID, str, 0, 0x20);
			ASSERT(bRet);

			strID.TrimRight();
			pUIWnd->m_strID = strID;

			// Parse rect
			int nRectPos = str.Find(strType);
			ASSERT(nRectPos > 0);
			str.Delete(0, nRectPos + strType.GetLength());

			pUIWnd->m_rcControl = CUIMgr::MAKERECT(str);

			// Parse Style, Caption, Font and children.
			while (m_ResourceFile.ReadStringA(strA))
			{
				++m_nCurrentLine;

				CString str = (LPCTSTR)_bstr_t(strA.c_str());
				FormatString(str);
				str.TrimLeft();
				str.TrimRight();

				if (str.IsEmpty())
					continue;

				CString strProp;
				AfxExtractSubString(strProp, str, 0, 0x20);
				strProp.TrimRight();

				// Parse Style
				if (strProp.CompareNoCase(_T("STYLE")) == 0)
				{
					DWORD dwStyle = 0;
					ParseDlgStyle(str, dwStyle);
				}
				// Parse extended style
				else if (strProp.CompareNoCase(_T("EXSTYLE")) == 0)
				{
					DWORD dwExStyle = 0;
					ParseDlgExStyle(str, dwExStyle);
				}
				else if (strProp.CompareNoCase(_T("CAPTION")) == 0)
				{

				}
				else if (strProp.CompareNoCase(_T("MENU")) == 0)
				{

				}
				else if (strProp.CompareNoCase(_T("FONT")) == 0)
				{
					ParseDlgFont(str, &lf);

					// Adjust the rect of UI Window
					RECT rc = pUIWnd->m_rcControl;
					MapRect(lf, rc);
					pUIWnd->m_rcControl = rc;
				}
				// Parse child controls
				else if (strProp.CompareNoCase(_T("BEGIN")) == 0)
				{
					ParseChildren(pUIWnd, lf);
					break;
				}
				else
				{
					break;
				}
				// Parse caption
				// Parse font
			}

			m_vpWindows.push_back(pUIWnd);
		}
	}

	m_ResourceFile.Detach();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CTransformDialog::Transform(LPCTSTR lpszSourcePath, LPCTSTR lpszXUITargetPath)
{
	if (lpszSourcePath == NULL || lpszXUITargetPath == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	m_strXUITargetPath = lpszXUITargetPath;

	CString strExt = GetFileExt(lpszSourcePath, FALSE);
	if (strExt.CompareNoCase(_T("exe")) == 0)
	{
		TransformExe(lpszSourcePath);
	}
	else if (strExt.CompareNoCase(_T("rc")) == 0)
	{
		int nRet = TransformRC(lpszSourcePath);
		if (nRet != 0)
			return nRet;
	}
	else if (strExt.CompareNoCase(_T("dsp")) == 0)
	{
		TransformDsp(lpszSourcePath);
	}

	// Save the new window
	CUIMgr::SaveSkin(m_vpWindows, m_strXUITargetPath);

	// Release windows.
	size_t i = 0;
	size_t nWndCount = m_vpWindows.size();
	for (i=0; i<nWndCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		size_t nChildCount = pUIProp->m_vControls.size();
		for (size_t j=0; j<nChildCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];
			delete pCtrlProp;
		}
		pUIProp->m_vControls.clear();

		delete pUIProp;
	}

	m_vpWindows.clear();

	//
	CString strInfo = _T("Finish transform the RC file\r\n");

	if (m_bHasGroupBox)
	{
		strInfo += _T("The RC file include group box, it can be drawn on the background image.\r\n");
	}
	if (m_bHasTabControl)
	{
		strInfo += _T("The RC file include tab control, Please use Radio Button to instead it.\r\n");
	}

	size_t nNotSupportCount = m_vNotSupportedControlType.size();
	if (nNotSupportCount > 0)
	{
		strInfo += _T("The following control type not support:");
	}
	for (i=0; i<nNotSupportCount; ++i)
	{
		strInfo += ' ',
		strInfo += m_vNotSupportedControlType[i];
		if (i != nNotSupportCount - 1)
			strInfo += ',';
	}

	AfxMessageBox(strInfo);

	return 0;
}
