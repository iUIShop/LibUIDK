// UIMgr.cpp: implementation of the COldUIMgr and CUIMgr class.
//

#include "stdafx.h"
#include "Version.h"
#ifdef _USE_LICENSE
#include "License/License.h"
#endif

// for parse xml
#if(_MSC_VER > 1200) // VC6.0
#import <msxml6.dll> rename_namespace("XML")
#else
#include <MsXml2.h>
#import <msxml3.dll> rename_namespace("XML")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define LIBUIDK_BUILD_VERSION		7723		// The version of xui format

//////////////////////////////////////////////////////////////////////////
// COldUIMgr

class COldUIMgr
{
public:
	COldUIMgr();
	virtual ~COldUIMgr();

public:
	// Path
	static int SetUIPath(LPCTSTR lpszUIPath, BOOL bCheckUnicode = TRUE, BOOL bCheckVersion = FALSE);
	static int SetUIPath(LPCTSTR lpszUIID, LPCTSTR lpszResouceID, LPCTSTR lpszResourceType);

	static CString GetUIPath();
	static CString GetUIResourcePath();
	static CString GetUIPathWithoutTitle();

	// Load, save and release skin
	static int LoadSkin();
	static int ReleaseSkin();
	static int ReleaseMemoryFile();

	// Unicode
	static BOOL IsSkinUnicode();

	// Version
	static int GetUIFileMajorVersion();
	static int GetUIFileMinorVersion();
	static int GetUIFileBuildVersion();
	static int GetLibUIDKMajorVersion();
	static int GetLibUIDKMinorVersion();
	static int GetLibUIDKBuildVersion();

	// DPI
	static int GetLogPixelsX();
	static int GetLogPixelsY();
	static int EnableDPI(BOOL bEnable);
	static BOOL IsEnableDPI();

	static RECT MAKERECT(LPCTSTR lpszRect);
	static int StringToLOGFONT(LPCTSTR lpszStruct, LOGFONT *plf);
	static CString StructToString(void *pStruct, UINT uSizeStruct);
	static int ParseFontProperty(const CString &strValue, IUI_VARIANT &varFontID);
	static int ParseColorProperty(const CString &strValue, IUI_VARIANT &varColorID);

	// Window
	static std::vector<UIWNDPROPERTIES *> *GetUIWndPropertiesList()
	{
		return &m_vpWindows;
	}
	static UIWNDPROPERTIES *GetUIWNDPROPERTIES(LPCTSTR lpszWindowID);

	// Control
	static CTRLPROPERTIES *GetCONTROL(LPCTSTR lpszWindowID, LPCTSTR lpszCtrlID);

	// Resource
	static RESCOLORPROPERTIES *FindColorResource(const IUI_VARIANT &varColor);
	static RESFONTPROPERTIES *FindFontResource(const LOGFONT &lf);

protected:
	static int LoadSetting(int *pnLogPixelsX, int *pnLogPixelsY);
	static int LoadUIWnd(UIWNDPROPERTIES *pWndProperties);
	static BOOL LoadControlCommon(const CString &strKey, const CString &strValue, CTRLPROPERTIES *pCtrlProperties);
	static BOOL LoadButtonBase(const CString &strKey, const CString &strValue, BTNBASEPROPERTIES *pBtnBaseProp);
	static BOOL LoadToggleButton(const CString &strKey, const CString &strValue, TOGGLEBTNPROPERTIES *pToggleBtnProp);
	static int LoadPushButton(BTNPROPERTIES *pBtnProperties);
	static int LoadCheckBox(CHKPROPERTIES *pChkProperties);
	static int LoadRadioButton(RADPROPERTIES *pRadProperties);
	static int LoadStatic(TXTPROPERTIES *pStaProperties);
	static int LoadEdit(EDTPROPERTIES *pEdtProperties);
	static int LoadComboBox(CMBPROPERTIES *pCmbProperties);
	static int LoadSpinButton(SPNPROPERTIES *pSpnProperties);
	static int LoadProgress(PRGPROPERTIES *pPrgProperties);
	static int LoadSlider(SLDPROPERTIES *pSldProperties);
	static int LoadHotKey(HOTPROPERTIES *pHotProperties);
	static int LoadIPAddress(IPAPROPERTIES *pIPAProperties);
	static int LoadListCtrl(LSTCTRLPROPERTIES *pLstCtrlProperties);
	static int LoadTreeCtrl(TREECTRLPROPERTIES *pTreeCtrlProperties);
	static int LoadScrollBarEx(SBEXPROPERTIES *pSBExProperties);
	static int LoadThumbnail(THMPROPERTIES *pThmProperties);
	static int LoadTreeList(TREELISTPROPERTIES *pTreeListProperties);
	static int LoadHtmlCtrl(HTMLCTRLPROPERTIES *pHtmlCtrlProperties);
	static int AddControlToWndList(LPCTSTR lpszSection, CTRLPROPERTIES *pCtrl);

private:
	static void LicenseWarning()
	{
		AfxMessageBox(_T("This libarary hasn't been licensed! register it from http://www.iuishop.com"));
		ASSERT(FALSE);
	}

public:
	static BOOL m_bLoadFromFile;

	// for hard disk skin
	static CString m_strUIPath;
	static CString m_strUIPathWithoutTitle;
	static CString m_strUIResourcePath;

	// for resource skin
	static CString m_strUIID;
	static CString m_strUIResourcehID;
	static CString m_strUIResourceType;

	// for version
	static int m_nUIFileMajorVersion;
	static int m_nUIFileMinorVersion;
	static int m_nUIFileBuildVersion;
	static int m_nLibUIDKMajorVersion;
	static int m_nLibUIDKMinorVersion;
	static int m_nLibUIDKBuildVersion;

	// for DPI
	static BOOL m_bEnableDPI;
	static int m_nLogPixelsX;
	static int m_nLogPixelsY;

	// memory file
	static CMemStdioFile m_fileTheUI;

	// Next font resource ID.
	static int m_nNextFontResID;
	// Next color resource ID.
	static int m_nNextColorResID;
	// Next header resource ID.
	static int m_nNextHeaderResID;

public:
	static std::vector<UIWNDPROPERTIES *> m_vpWindows;
	static std::vector<RESFONTPROPERTIES *> m_vpFontRes;
	static std::vector<RESCOLORPROPERTIES *> m_vpColorRes;
	static std::vector<HORSBPROPERTIES *> m_vpStyleHorScrollBar;
	static std::vector<VERSBPROPERTIES *> m_vpStyleVerScrollBar;
	static std::vector<SPLITTERPROPERTIES *> m_vpStyleSplitter;
	static std::vector<STACKPNLPROPERTIES *> m_vpStylePanelStack;
	static std::vector<LINEWLPROPERTIES *> m_vpStyleWLLine;
	static std::vector<PICWLPROPERTIES *> m_vpStyleWLPic;
	static std::vector<HEADERPROPERTIES *> m_vpHeaderResource;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL COldUIMgr::m_bLoadFromFile = TRUE;
CString COldUIMgr::m_strUIPath = _T("");
CString COldUIMgr::m_strUIPathWithoutTitle = _T("");
CString COldUIMgr::m_strUIResourcePath = _T("");
CString COldUIMgr::m_strUIID = _T("");
CString COldUIMgr::m_strUIResourcehID = _T("");
CString COldUIMgr::m_strUIResourceType = _T("");
CMemStdioFile COldUIMgr::m_fileTheUI;
int COldUIMgr::m_nNextFontResID = 0;
int COldUIMgr::m_nNextColorResID  = 0;
int COldUIMgr::m_nNextHeaderResID = 0;
std::vector<UIWNDPROPERTIES *> COldUIMgr::m_vpWindows;
std::vector<RESFONTPROPERTIES *> COldUIMgr::m_vpFontRes;
std::vector<RESCOLORPROPERTIES *> COldUIMgr::m_vpColorRes;
std::vector<HORSBPROPERTIES *> COldUIMgr::m_vpStyleHorScrollBar;
std::vector<VERSBPROPERTIES *> COldUIMgr::m_vpStyleVerScrollBar;
std::vector<SPLITTERPROPERTIES *> COldUIMgr::m_vpStyleSplitter;
std::vector<LINEWLPROPERTIES *> COldUIMgr::m_vpStyleWLLine;
std::vector<STACKPNLPROPERTIES *> COldUIMgr::m_vpStylePanelStack;
std::vector<PICWLPROPERTIES *> COldUIMgr::m_vpStyleWLPic;
std::vector<HEADERPROPERTIES *> COldUIMgr::m_vpHeaderResource;
int COldUIMgr::m_nUIFileMajorVersion = -1;
int COldUIMgr::m_nUIFileMinorVersion = -1;
int COldUIMgr::m_nUIFileBuildVersion = -1;
int COldUIMgr::m_nLibUIDKMajorVersion = 5;
int COldUIMgr::m_nLibUIDKMinorVersion = 5;
int COldUIMgr::m_nLibUIDKBuildVersion = LIBUIDK_BUILD_VERSION;
int COldUIMgr::m_nLogPixelsX = -1;
int COldUIMgr::m_nLogPixelsY = -1;
BOOL COldUIMgr::m_bEnableDPI = FALSE;


COldUIMgr::COldUIMgr()
{

}

COldUIMgr::~COldUIMgr()
{

}

int COldUIMgr::SetUIPath(LPCTSTR lpszUIPath, BOOL bCheckUnicode/* = TRUE*/, BOOL bCheckVersion/* = FALSE*/)
{
	m_bLoadFromFile = TRUE;

	ASSERT(lpszUIPath != NULL);
	ASSERT(AfxIsValidString(lpszUIPath));

	BOOL bIsAbsolutePath = FALSE;
	int nLen = (int)_tcslen(lpszUIPath);
	int i = 0;
	for (i = 0; i < nLen; ++i)
	{
		if (lpszUIPath[i] == ':')
		{
			bIsAbsolutePath = TRUE;
			break;
		}
	}

	if (bIsAbsolutePath)
	{
		m_strUIPath = lpszUIPath;
	}
	else
	{
		TCHAR szFullName[MAX_PATH] = {0};
		GetModuleFileName(NULL, szFullName, MAX_PATH - 1);
		TCHAR szDisc[_MAX_PATH] = {0};
		TCHAR szPath[_MAX_PATH] = {0};
		TSPLITPATH(szFullName, szDisc, _MAX_PATH, szPath, _MAX_PATH, NULL, 0, NULL, 0);
		TSTRCAT(szDisc, _MAX_PATH, szPath);
		m_strUIPath = szDisc;
		m_strUIPath += lpszUIPath;
	}

	if (!PathFileExists(m_strUIPath))
	{
		m_strUIPath.Empty();
		return E_SETUIPATH;
	}

	if (bCheckUnicode)
	{
		BOOL bUnicodeProject = FALSE;
#ifdef _UNICODE
		bUnicodeProject = TRUE;
#endif
#ifdef UNICODE
		bUnicodeProject = TRUE;
#endif
		if (bUnicodeProject && _tcslen(lpszUIPath) > 0)
		{
			FILE *fp = TFOPEN(m_strUIPath, _T("rb"));
			ASSERT(fp != NULL);
			BYTE cbUnicode[2] = {0};
			fread(cbUnicode, sizeof(BYTE) * 2, 1, fp);
			fclose(fp);
			if (cbUnicode[0] != 0xFF || cbUnicode[1] != 0xFE)
			{
				CString str;
				str.Format(_T("The skin file:\"%s\" dosn't support Unicode!"), m_strUIPath);
				AfxMessageBox(str);
				return E_NOTUNICODE;
			}
		}
		else if (!bUnicodeProject && _tcslen(lpszUIPath) > 0)
		{
			FILE *fp = TFOPEN(m_strUIPath, _T("rb"));
			ASSERT(fp != NULL);
			BYTE cbUnicode[2] = {0};
			fread(cbUnicode, sizeof(BYTE) * 2, 1, fp);
			fclose(fp);
			if (cbUnicode[0] == 0xFF && cbUnicode[1] == 0xFE)
			{
				CString str;
				str.Format(_T("The skin file:\"%s\" supports Unicode, but the skin editor doesn't!"), m_strUIPath);
				AfxMessageBox(str);
				return E_NOTUNICODE;
			}
		}
	}

	m_nUIFileBuildVersion = GetPrivateProfileInt(_T("Version"), _T("Build"), m_nUIFileBuildVersion, m_strUIPath);
	if (m_nUIFileBuildVersion < m_nLibUIDKBuildVersion)
	{
		TRACE(_T("[LibUIDK]===Warning: The version of skin file older than the LibUIDK, so some features will be lost.\n"));
		if (bCheckVersion)
		{
			return E_OLDERVERSION;
		}
	}
	if (m_nUIFileBuildVersion > m_nLibUIDKBuildVersion)
	{
		TRACE(_T("[LibUIDK]===Warning: The version of skin file newer than the LibUIDK, so some features will be lost.\n"));
		if (bCheckVersion)
		{
			return E_NEWERVERSION;
		}
	}

	// init path of resource.h file.
	TCHAR szDisc[_MAX_PATH] = {0};
	TCHAR szDir[_MAX_PATH] = {0};
	TSPLITPATH(m_strUIPath, szDisc, _MAX_PATH, szDir, _MAX_PATH, NULL, 0, NULL, 0);
	TSTRCAT(szDisc, _MAX_PATH, szDir);
	if (szDisc[_tcslen(szDisc) - 1] == '/' || szDisc[_tcslen(szDisc) - 1] == '\\')
	{
		TSTRCAT(szDisc, _MAX_PATH, _T("Resource.h"));
	}
	else
	{
		TSTRCAT(szDisc, _MAX_PATH, _T("\\Resource.h"));
	}
	m_strUIResourcePath = szDisc;

	// init windows tree struct
	DWORD dwTime = GetTickCount();
	int nRet = LoadSkin();
	DWORD dwTime2 = GetTickCount() - dwTime;
	TRACE(_T("[LibUIDK]===Load skins spend %d ms\n"), dwTime2);
	if (nRet != 0)
	{
		return nRet;
	}

	// 	// change skin for all windows.
	// 	CWnd *pWnd = AfxGetMainWnd();
	// 	if (pWnd->GetSafeHwnd() != NULL)
	// 	{
	// 		TCHAR szBuf[MAX_PATH] = {0};
	// 		DWORD dwSize = MAX_PATH;
	// 		DWORD dwRet = GetPrivateProfileSectionNames(szBuf, MAX_PATH, m_strUIPath);
	//
	// 		while(dwRet >= dwSize - 2)
	// 		{
	// 			dwSize *= 2;
	// 			TCHAR *p = new TCHAR[dwSize];
	// 			ASSERT(p != NULL);
	// 			dwRet = GetPrivateProfileSectionNames(p, dwSize, m_strUIPath);
	// 			delete []p;
	// 			p = NULL;
	// 		}
	//
	// 		TCHAR *p = new TCHAR[dwSize];
	// 		ASSERT(p != NULL);
	// 		dwRet = GetPrivateProfileSectionNames(p, dwSize, m_strUIPath);
	// 		CString strSection;
	// 		DWORD pos = 0;
	// 		i = 0;
	// 		while (pos < dwRet)
	// 		{
	// 			AfxExtractSubString(strSection, p, i, '\0');
	// 			pos += (strSection.GetLength() + 1);
	// 			i++;
	// 			ASSERT(strSection.GetLength() != 0);
	//
	// 			CUIWnd *pUIWnd = COldUIMgr::GetUIWnd(strSection);
	// 			if (pUIWnd->GetSafeHwnd() != NULL)
	// 			{
	//  				pUIWnd->SendMessage(WM_CHANGESKIN);
	//  				pUIWnd->Invalidate();
	// 			}
	// 		}
	// 		delete []p;
	// 		p = NULL;
	// 	}

	return 0;
}

int COldUIMgr::SetUIPath(LPCTSTR lpszUIID, LPCTSTR lpszResouceID, LPCTSTR lpszResourceType)
{
	m_bLoadFromFile = FALSE;

	m_strUIID = lpszUIID;
	m_strUIResourcehID = lpszResouceID;
	m_strUIResourceType = lpszResourceType;
	// init windows tree struct
	DWORD dwTime = GetTickCount();
	int nRet = LoadSkin();
	DWORD dwTime2 = GetTickCount() - dwTime;
	TRACE(_T("[LibUIDK]===Load skins spend %d ms\n"), dwTime2);
	if (nRet != 0)
	{
		return nRet;
	}

	return 0;
}

CString COldUIMgr::GetUIPath()
{
	return m_strUIPath;
}

CString COldUIMgr::GetUIResourcePath()
{
	return m_strUIResourcePath;
}

CString COldUIMgr::GetUIPathWithoutTitle()
{
	TCHAR szDisc[_MAX_PATH] = {0};
	TCHAR szPath[_MAX_PATH] = {0};
	TSPLITPATH(m_strUIPath, szDisc, _MAX_PATH, szPath, _MAX_PATH, NULL, 0, NULL, 0);
	m_strUIPathWithoutTitle = szDisc;
	m_strUIPathWithoutTitle += szPath;
	if (m_strUIPathWithoutTitle.Right(1) != "/" && m_strUIPathWithoutTitle.Right(1) != "\\")
	{
		m_strUIPathWithoutTitle += "\\";
	}

	return m_strUIPathWithoutTitle;
}

BOOL COldUIMgr::IsSkinUnicode()
{
	return m_fileTheUI.IsUnicode();
}

int COldUIMgr::GetUIFileMajorVersion()
{
	return m_nUIFileMajorVersion;
}

int COldUIMgr::GetUIFileMinorVersion()
{
	return m_nUIFileMinorVersion;
}

int COldUIMgr::GetUIFileBuildVersion()
{
	return m_nUIFileBuildVersion;
}

int COldUIMgr::GetLibUIDKMajorVersion()
{
	return m_nLibUIDKMajorVersion;
}

int COldUIMgr::GetLibUIDKMinorVersion()
{
	return m_nLibUIDKMinorVersion;
}

int COldUIMgr::GetLibUIDKBuildVersion()
{
	return m_nLibUIDKBuildVersion;
}

int COldUIMgr::GetLogPixelsX()
{
	return m_nLogPixelsX;
}

int COldUIMgr::GetLogPixelsY()
{
	return m_nLogPixelsY;
}

int COldUIMgr::EnableDPI(BOOL bEnable)
{
	m_bEnableDPI = bEnable;
	return 0;
}

BOOL COldUIMgr::IsEnableDPI()
{
	return m_bEnableDPI;
}

RECT COldUIMgr::MAKERECT(LPCTSTR lpszRect)
{
	RECT rect = { 0, 0, 0, 0 };

	CString str;
	if (!AfxExtractSubString(str, lpszRect, 0, ','))
	{
		return rect;
	}
	rect.left = _ttoi(str.GetBuffer(str.GetLength()));

	if (!AfxExtractSubString(str, lpszRect, 1, ','))
	{
		return rect;
	}
	rect.top = _ttoi(str);

	if (!AfxExtractSubString(str, lpszRect, 2, ','))
	{
		return rect;
	}
	rect.right = _ttoi(str);

	if (!AfxExtractSubString(str, lpszRect, 3, ','))
	{
		return rect;
	}
	rect.bottom = _ttoi(str);

	return rect;
}

int COldUIMgr::StringToLOGFONT(LPCTSTR lpszStruct, LOGFONT *plf)
{
	if (plf == NULL)
	{
		return -1;
	}

	_bstr_t pszIniFile = _T("c:\\struct~.ini");
	if (IsSkinUnicode())
	{
		LOGFONTW lfW;
		WritePrivateProfileStringW(L"Struct", L"Value", (LPCWSTR)_bstr_t(lpszStruct), pszIniFile);
		BOOL bRet = GetPrivateProfileStructW(L"Struct", L"Value", &lfW, sizeof(LOGFONTW), pszIniFile);
#ifdef _UNICODE
		memcpy(plf, &lfW, sizeof(LOGFONTW));
#else
		memcpy(plf, &lfW, sizeof(LOGFONTW) - sizeof(WCHAR) * 32);
		TSTRCPY(plf->lfFaceName, 32, (LPCSTR)_bstr_t(lfW.lfFaceName));
#endif
	}
	else
	{
		LOGFONTA lfA;
		WritePrivateProfileStringA("Struct", "Value", (LPCSTR)_bstr_t(lpszStruct), pszIniFile);
		BOOL bRet = GetPrivateProfileStructA("Struct", "Value", &lfA, sizeof(LOGFONTA), pszIniFile);
#ifdef _UNICODE
		memcpy(plf, &lfA, sizeof(LOGFONTA) - sizeof(CHAR) * 32);
		TSTRCPY(plf->lfFaceName, 32, (LPCWSTR)_bstr_t(lfA.lfFaceName));
#else
		memcpy(plf, &lfA, sizeof(LOGFONTA));
#endif
	}
	DeleteFile(pszIniFile);

	return 0;
}

CString COldUIMgr::StructToString(void *pStruct, UINT uSizeStruct)
{
	if (pStruct == NULL)
	{
		return _T("");
	}

	WritePrivateProfileStruct(_T("Struct"), _T("Value"), pStruct, uSizeStruct, _T("struct~.ini"));
	TCHAR szBuf[MAX_PATH + 1] = {0};
	GetPrivateProfileString(_T("Struct"), _T("Value"), _T(""), szBuf, MAX_PATH, _T("struct~.ini"));
	CString strRet(szBuf);
	return strRet;
}

int COldUIMgr::ParseFontProperty(const CString &strValue, IUI_VARIANT &varFontID)
{
	LOGFONT lf;
	StringToLOGFONT(strValue, &lf);

	// Check the font exist.
	RESFONTPROPERTIES *pFoundFontProp = (RESFONTPROPERTIES *)COldUIMgr::FindFontResource(lf);
	if (pFoundFontProp == NULL)
	{
		// Create a new font resource.
		pFoundFontProp = new RESFONTPROPERTIES();

		// ID
		CString strFontResID;
		strFontResID.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RESOURCE_GENERAL_FONT), ++CUIMgr::m_nNextFontResID);
		pFoundFontProp->m_strID = strFontResID;

		// Font content
		pFoundFontProp->m_logFont = lf;
		if (COldUIMgr::IsEnableDPI())
		{
			GetDPILogFont(&pFoundFontProp->m_logFont);
		}

		// Save the font resource
		m_vpFontRes.push_back(pFoundFontProp);
	}

	// Use the font resource
	varFontID = (LPCTSTR)pFoundFontProp->m_strID;	// Only assign the value. NOT other property

	return 0;
}

int COldUIMgr::ParseColorProperty(const CString &strValue, IUI_VARIANT &varColorID)
{
	COLORREF crColor = LongFromString(strValue);

	// Check the color exist.
	RESCOLORPROPERTIES *pFoundColorProp = (RESCOLORPROPERTIES *)COldUIMgr::FindColorResource((LONG)crColor);
	if (pFoundColorProp == NULL)
	{
		// Create a new color resource.
		pFoundColorProp = new RESCOLORPROPERTIES();

		// ID
		CString strColorResID;
		strColorResID.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RESOURCE_COLOR), ++CUIMgr::m_nNextColorResID);
		pFoundColorProp->m_strID = strColorResID;

		// Color content
		pFoundColorProp->m_crColor = (LONG)crColor;

		// Save to the color resource list
		m_vpColorRes.push_back(pFoundColorProp);
	}

	// Use the color resource
	varColorID = (LPCTSTR)pFoundColorProp->m_strID;	// Only assign the value. NOT other property.

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int COldUIMgr::ReleaseMemoryFile()
{
	BYTE *pBuf = m_fileTheUI.Detach();
	delete []pBuf;

	return 0;
}

int COldUIMgr::ReleaseSkin()
{
	BYTE *pBuf = m_fileTheUI.Detach();
	delete []pBuf;

	size_t nWndCount = m_vpWindows.size();

	for (size_t i = 0; i < nWndCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		size_t nChildCount = pUIProp->m_vControls.size();
		for (size_t j = 0; j < nChildCount; ++j)
		{
			CTRLPROPERTIES *pControl = pUIProp->m_vControls[j];

			delete pControl;
		}
		pUIProp->m_vControls.clear();

		delete pUIProp;
	}
	m_vpWindows.clear();

	return 0;
}

int COldUIMgr::LoadSkin()
{
	// The Resource.h need not to be load

	// Load the.ui.
	m_fileTheUI.Detach();
	if (m_bLoadFromFile)
	{
		FILE *fp = TFOPEN(m_strUIPath, _T("rb"));
		if (fp == NULL)
		{
			return -1;
		}

		fseek(fp, 0, SEEK_END);
		int nLen = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		BYTE *pBuf = new BYTE[nLen];
		memset(pBuf, 0, nLen);
		fread(pBuf, nLen, 1, fp);
		fclose(fp);
		m_fileTheUI.Attach(pBuf, nLen, 0);
	}
	else
	{
		m_fileTheUI.OpenResource(m_strUIID, m_strUIResourceType);
	}

	m_vpWindows.clear();

	CString strBuf;
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) != _T("["))
		{
			continue;
		}

		CString strSection = strBuf;
		strSection.Delete(0, 1);
		strSection.TrimRight();
		strSection.TrimRight(']');

		if (strSection == _T("Settings"))
		{
			LoadSetting(&m_nLogPixelsX, &m_nLogPixelsY);
			continue;
		}

		m_fileTheUI.ReadString(strBuf);
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(5) == _T("Type="))
		{
			CString strType = strBuf;
			strType.Delete(0, 5);
			strType.TrimRight();

			if (strType.CompareNoCase(_T("Window")) == 0)
			{
				UIWNDPROPERTIES *pUIProp = new UIWNDPROPERTIES;
				LoadUIWnd(pUIProp);
				m_vpWindows.push_back(pUIProp);
			}
			// Push button
			if (strType.CompareNoCase(GetControlTypeString(CT_PUSHBUTTON)) == 0)
			{
				BTNPROPERTIES *pBtn = new BTNPROPERTIES;
				LoadPushButton(pBtn);
				AddControlToWndList(strSection, pBtn);
			}
			// Check box
			if (strType.CompareNoCase(GetControlTypeString(CT_CHECK)) == 0)
			{
				CHKPROPERTIES *pChk = new CHKPROPERTIES;
				LoadCheckBox(pChk);
				AddControlToWndList(strSection, pChk);
			}
			// Radio button
			if (strType.CompareNoCase(GetControlTypeString(CT_RADIO)) == 0)
			{
				RADPROPERTIES *pRad = new RADPROPERTIES;
				LoadRadioButton(pRad);
				AddControlToWndList(strSection, pRad);
			}
			// Static
			if (strType.CompareNoCase(GetControlTypeString(CT_STATIC)) == 0)
			{
				TXTPROPERTIES *pSta = new TXTPROPERTIES;
				LoadStatic(pSta);
				AddControlToWndList(strSection, pSta);
			}
			// Edit
			if (strType.CompareNoCase(GetControlTypeString(CT_EDIT)) == 0)
			{
				EDTPROPERTIES *pEdt = new EDTPROPERTIES;
				LoadEdit(pEdt);
				AddControlToWndList(strSection, pEdt);
			}
			// Combo box
			if (strType.CompareNoCase(GetControlTypeString(CT_COMBOBOX)) == 0)
			{
				CMBPROPERTIES *pCmb = new CMBPROPERTIES;
				LoadComboBox(pCmb);
				AddControlToWndList(strSection, pCmb);
			}
			// Spin button
			if (strType.CompareNoCase(GetControlTypeString(CT_SPIN)) == 0)
			{
				SPNPROPERTIES *pSpn = new SPNPROPERTIES;
				LoadSpinButton(pSpn);
				AddControlToWndList(strSection, pSpn);
			}
			// Progress
			if (strType.CompareNoCase(GetControlTypeString(CT_PROGRESS)) == 0)
			{
				PRGPROPERTIES *pPrg = new PRGPROPERTIES;
				LoadProgress(pPrg);
				AddControlToWndList(strSection, pPrg);
			}
			// Slider
			if (strType.CompareNoCase(GetControlTypeString(CT_SLIDER)) == 0)
			{
				SLDPROPERTIES *pSld = new SLDPROPERTIES;
				LoadSlider(pSld);
				AddControlToWndList(strSection, pSld);
			}
			// HotKey
			if (strType.CompareNoCase(GetControlTypeString(CT_HOTKEY)) == 0)
			{
				HOTPROPERTIES *pHot = new HOTPROPERTIES;
				LoadHotKey(pHot);
				AddControlToWndList(strSection, pHot);
			}
			// IPAddress
			if (strType.CompareNoCase(GetControlTypeString(CT_IPADDRESS)) == 0)
			{
				IPAPROPERTIES *pIPA = new IPAPROPERTIES;
				LoadIPAddress(pIPA);
				AddControlToWndList(strSection, pIPA);
			}
			// Extended Slider
			if (strType.CompareNoCase(GetControlTypeString(CT_SLIDEREX)) == 0)
			{
				SLDEXPROPERTIES *pSldEx = new SLDEXPROPERTIES;
				LoadSlider(pSldEx);
				AddControlToWndList(strSection, pSldEx);
			}
			// ListCtrl
			if (strType.CompareNoCase(GetControlTypeString(CT_LISTCTRL)) == 0)
			{
				LSTCTRLPROPERTIES *pLstCtrl = new LSTCTRLPROPERTIES;
				LoadListCtrl(pLstCtrl);
				AddControlToWndList(strSection, pLstCtrl);
			}
			// TreeCtrl
			if (strType.CompareNoCase(GetControlTypeString(CT_TREECTRL)) == 0)
			{
				TREECTRLPROPERTIES *pTree = new TREECTRLPROPERTIES;
				LoadTreeCtrl(pTree);
				AddControlToWndList(strSection, pTree);
			}
			// ScrollBarEx
			if (strType.CompareNoCase(GetControlTypeString(CT_SCROLLBAREX)) == 0)
			{
				SBEXPROPERTIES *pSBEx = new SBEXPROPERTIES;
				LoadScrollBarEx(pSBEx);
				AddControlToWndList(strSection, pSBEx);
			}
			// Thumbnail
			if (strType.CompareNoCase(GetControlTypeString(CT_THUMBNAIL)) == 0)
			{
				THMPROPERTIES *pThm = new THMPROPERTIES;
				LoadThumbnail(pThm);
				AddControlToWndList(strSection, pThm);
			}
			// TreeList
			if (strType.CompareNoCase(GetControlTypeString(CT_TREELIST)) == 0)
			{
				TREELISTPROPERTIES *pTreeList = new TREELISTPROPERTIES;
				LoadTreeList(pTreeList);
				AddControlToWndList(strSection, pTreeList);
			}
			// RichEdit
			if (strType.CompareNoCase(GetControlTypeString(CT_WL_RICHEDIT)) == 0)
			{
				RICHEDITPROPERTIES *pRichEdt = new RICHEDITPROPERTIES;
				LoadEdit(pRichEdt);
				AddControlToWndList(strSection, pRichEdt);
			}
			// IM RichEdit
			if (strType.CompareNoCase(GetControlTypeString(CT_WL_RICHEDIT_IM)) == 0)
			{
				IMREPROPERTIES *pIMRichEdt = new IMREPROPERTIES;
				LoadEdit(pIMRichEdt);
				AddControlToWndList(strSection, pIMRichEdt);
			}
			// HtmlCtrl
			if (strType.CompareNoCase(GetControlTypeString(CT_HTMLCTRL)) == 0)
			{
				HTMLCTRLPROPERTIES *pHtmlCtrl = new HTMLCTRLPROPERTIES;
				LoadHtmlCtrl(pHtmlCtrl);
				AddControlToWndList(strSection, pHtmlCtrl);
			}
		}
	}

	if (!m_bLoadFromFile)
	{
		BYTE *pBuf = m_fileTheUI.Detach();
		delete [] pBuf;
	}

	return 0;
}

int COldUIMgr::LoadSetting(int *pnLogPixelsX, int *pnLogPixelsY)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		if (strBuf.IsEmpty())
		{
			continue;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (strKey.CompareNoCase(_T("LogPixelsX")) == 0)
		{
			if (pnLogPixelsX != NULL)
			{
				*pnLogPixelsX = _ttoi(strValue);
			}
		}
		else if (strKey.CompareNoCase(_T("LogPixelsY")) == 0)
		{
			if (pnLogPixelsY != NULL)
			{
				*pnLogPixelsY = _ttoi(strValue);
			}
		}
	}

	return 0;
}

int COldUIMgr::LoadUIWnd(UIWNDPROPERTIES *pWndProperties)
{
	int nR = 255;
	int nG = 0;
	int nB = 255;

	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (strKey.CompareNoCase(_T("IDs")) == 0)
		{
			pWndProperties->m_strID = strValue;
		}
		else if (strKey.CompareNoCase(_T("BkN")) == 0)
		{
			pWndProperties->m_strBKN = strValue;
		}
		if (strKey.CompareNoCase(_T("BkH")) == 0)
		{
			pWndProperties->m_strBKH = strValue;
		}
		if (strKey.CompareNoCase(_T("BkS")) == 0)
		{
			pWndProperties->m_strBKS = strValue;
		}
		if (strKey.CompareNoCase(_T("BkD")) == 0)
		{
			pWndProperties->m_strBKD = strValue;
		}
		if (strKey.CompareNoCase(_T("BkNo")) == 0)
		{
			pWndProperties->m_strBKNo = strValue;
		}
		if (strKey.CompareNoCase(_T("IsUseMaskColor")) == 0)
		{
			pWndProperties->m_bUseMaskColor = ((_ttoi(strValue) == 0) ? false : true);
		}
		if (strKey.CompareNoCase(_T("RValue")) == 0)
		{
			nR = _ttoi(strValue);
		}
		if (strKey.CompareNoCase(_T("GValue")) == 0)
		{
			nG = _ttoi(strValue);
		}
		if (strKey.CompareNoCase(_T("BValue")) == 0)
		{
			nB = _ttoi(strValue);
		}
		if (strKey.CompareNoCase(_T("ResizingPtX")) == 0)
		{
			LONG x = _ttoi(strValue);
			if (COldUIMgr::IsEnableDPI())
			{
				GetDPIPointX(x);
			}
			pWndProperties->m_ptWindowResize.SetPointX(x);
		}
		if (strKey.CompareNoCase(_T("ResizingPtY")) == 0)
		{
			LONG y = _ttoi(strValue);
			if (COldUIMgr::IsEnableDPI())
			{
				GetDPIPointY(y);
			}
			pWndProperties->m_ptWindowResize.SetPointY(y);
		}
		if (strKey.CompareNoCase(_T("TitleBarHeight")) == 0)
		{
			pWndProperties->m_lTitleBarHeight = LongFromString(strValue);
		}
	}

	if (pWndProperties->m_bUseMaskColor)
	{
		COLORREF crMask = RGB(nR, nG, nB);
		// Check the color exist.
		RESCOLORPROPERTIES *pFoundColorProp = (RESCOLORPROPERTIES *)CUIMgr::FindColorResource((LONG)crMask);
		if (pFoundColorProp == NULL)
		{
			// Create a new color resource.
			pFoundColorProp = new RESCOLORPROPERTIES();

			// ID
			CString strColorResID;
			strColorResID.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RESOURCE_COLOR), ++CUIMgr::m_nNextColorResID);
			pFoundColorProp->m_strID = strColorResID;

			// Color content
			pFoundColorProp->m_crColor = (LONG)crMask;

			// Save to the color resource list
			m_vpColorRes.push_back(pFoundColorProp);
		}

		// Use the color resource
		pWndProperties->m_strMaskColorResID = (LPCTSTR)pFoundColorProp->m_strID;
	}

	return 0;
}

BOOL COldUIMgr::LoadControlCommon(const CString &strKey, const CString &strValue, CTRLPROPERTIES *pCtrlProperties)
{
	if (strKey.CompareNoCase(_T("IDs")) == 0)
	{
		pCtrlProperties->m_strID = strValue;
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("Rect")) == 0)
	{
		pCtrlProperties->m_rcControl = MAKERECT(strValue);
		if (COldUIMgr::IsEnableDPI())
		{
			RECT rc = pCtrlProperties->m_rcControl;
			GetDPIRect(&rc);
			pCtrlProperties->m_rcControl = rc;
		}

		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("IsVisible")) == 0)
	{
		pCtrlProperties->m_bVisible = boolFromString(strValue);
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("IsDisabled")) == 0)
	{
		pCtrlProperties->m_bDisabled = boolFromString(strValue);
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("IsTabStop")) == 0)
	{
		pCtrlProperties->m_bTabStop = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("HorLayoutMode")) == 0)
	{
		pCtrlProperties->m_eHorLayoutMode.InitByEnumString(strValue);
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("VerLayoutMode")) == 0)
	{
		pCtrlProperties->m_eVerLayoutMode.InitByEnumString(strValue);
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("IsCanBackgroundChange")) == 0)
	{
		// Not used
	}
	else if (strKey.CompareNoCase(_T("IsSpecifyImages")) == 0)
	{
		pCtrlProperties->m_bSpecifyBackgroundImages = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("IsCombineImages")) == 0)
	{
		pCtrlProperties->m_bCombineBackgroundImages = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("BkCombine")) == 0)
	{
		pCtrlProperties->m_strBKCombine = strValue;
	}
	else if (strKey.CompareNoCase(_T("BkN")) == 0)
	{
		pCtrlProperties->m_strBKN = strValue;
	}
	else if (strKey.CompareNoCase(_T("BkH")) == 0)
	{
		pCtrlProperties->m_strBKH = strValue;
	}
	else if (strKey.CompareNoCase(_T("BkS")) == 0)
	{
		pCtrlProperties->m_strBKS = strValue;
	}
	else if (strKey.CompareNoCase(_T("BkD")) == 0)
	{
		pCtrlProperties->m_strBKD = strValue;
	}
	else if (strKey.CompareNoCase(_T("FontN")) == 0)
	{
		ParseFontProperty(strValue, pCtrlProperties->m_strResFontIDN);
	}
	else if (strKey.CompareNoCase(_T("IsHSameNFont")) == 0)
	{
		pCtrlProperties->m_bHSameNFont = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("FontH")) == 0)
	{
		ParseFontProperty(strValue, pCtrlProperties->m_strResFontIDH);
	}
	else if (strKey.CompareNoCase(_T("IsSSameNFont")) == 0)
	{
		pCtrlProperties->m_bSSameNFont = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("FontS")) == 0)
	{
		ParseFontProperty(strValue, pCtrlProperties->m_strResFontIDS);
	}
	else if (strKey.CompareNoCase(_T("IsDSameNFont")) == 0)
	{
		pCtrlProperties->m_bDSameNFont = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("FontD")) == 0)
	{
		ParseFontProperty(strValue, pCtrlProperties->m_strResFontIDD);
	}
	else if (strKey.CompareNoCase(_T("ColorN")) == 0)
	{
		ParseColorProperty(strValue, pCtrlProperties->m_strResColorIDN);
	}
	else if (strKey.CompareNoCase(_T("IsHSameNColor")) == 0)
	{
		pCtrlProperties->m_bHSameNColor = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("ColorH")) == 0)
	{
		ParseColorProperty(strValue, pCtrlProperties->m_strResColorIDH);
	}
	else if (strKey.CompareNoCase(_T("IsSSameNColor")) == 0)
	{
		pCtrlProperties->m_bSSameNColor = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("ColorS")) == 0)
	{
		ParseColorProperty(strValue, pCtrlProperties->m_strResColorIDS);
	}
	else if (strKey.CompareNoCase(_T("IsDSameNColor")) == 0)
	{
		pCtrlProperties->m_bDSameNColor = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("ColorD")) == 0)
	{
		ParseColorProperty(strValue, pCtrlProperties->m_strResColorIDD);
	}

	return FALSE;
}

BOOL COldUIMgr::LoadButtonBase(const CString &strKey, const CString &strValue, BTNBASEPROPERTIES *pBtnBaseProp)
{
	if (LoadControlCommon(strKey, strValue, pBtnBaseProp))
	{

	}
	else if (strKey.CompareNoCase(_T("Caption")) == 0)
	{
		pBtnBaseProp->m_strCaption = strValue;
	}
	else if (strKey.CompareNoCase(_T("IsR2LReadOrder")) == 0)
	{
		pBtnBaseProp->m_bR2LReadOrder = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("IsMultiline")) == 0)
	{
		pBtnBaseProp->m_bMultiline = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("IsEndEllipsis")) == 0)
	{
		pBtnBaseProp->m_bEndEllipsis = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("UseToolTip")) == 0)
	{
		pBtnBaseProp->m_bUseToolTip = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("ToolTips")) == 0)
	{
		pBtnBaseProp->m_strToolTip = strValue;
	}
	else if (strKey.CompareNoCase(_T("Tag")) == 0)
	{
		CString strTag = strValue;
		strTag = strTag.Right(strTag.GetLength() - 1);
		strTag = strTag.Left(strTag.GetLength() - 1);
		pBtnBaseProp->m_strTag = strTag;
	}
	else if (strKey.CompareNoCase(_T("MaskBmp")) == 0)
	{
		pBtnBaseProp->m_strMaskBmp = strValue;
	}
	else if (strKey.CompareNoCase(_T("HorAlignMode")) == 0)
	{
		pBtnBaseProp->m_eTextHorAlignMode.InitByEnumString(strValue);
	}
	else if (strKey.CompareNoCase(_T("HorAlig")) == 0)
	{
		long nValue = _ttoi(strValue);
		if (IsEnableDPI())
		{
			GetDPIPointX(nValue);
		}
		RECT rcMargin = pBtnBaseProp->m_rcPadding4Text;
		rcMargin.left = nValue;
		pBtnBaseProp->m_rcPadding4Text = rcMargin;
	}
	else if (strKey.CompareNoCase(_T("VerAlignMode")) == 0)
	{
		pBtnBaseProp->m_eTextVerAlignMode.InitByEnumString(strValue);
	}
	else if (strKey.CompareNoCase(_T("VerAlig")) == 0)
	{
		long nValue = _ttoi(strValue);
		if (IsEnableDPI())
		{
			GetDPIPointY(nValue);
		}
		RECT rcMargin = pBtnBaseProp->m_rcPadding4Text;
		rcMargin.top = nValue;
		pBtnBaseProp->m_rcPadding4Text = rcMargin;
	}
	else if (strKey.CompareNoCase(_T("IsSpecifyForegroundImages")) == 0)
	{
		pBtnBaseProp->m_bSpecifyForegroundImages = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("IsCombineForegroundImages")) == 0)
	{
		pBtnBaseProp->m_bCombineForegroundImages = boolFromString(strValue);
	}
	else if (strKey.CompareNoCase(_T("FkCombine")) == 0)
	{
		pBtnBaseProp->m_strFGCombine = strValue;
	}
	else if (strKey.CompareNoCase(_T("FkN")) == 0)
	{
		pBtnBaseProp->m_strFGN = strValue;
	}
	else if (strKey.CompareNoCase(_T("FkH")) == 0)
	{
		pBtnBaseProp->m_strFGH = strValue;
	}
	else if (strKey.CompareNoCase(_T("FkS")) == 0)
	{
		pBtnBaseProp->m_strFGS = strValue;
	}
	else if (strKey.CompareNoCase(_T("FkD")) == 0)
	{
		pBtnBaseProp->m_strFGD = strValue;
	}
	else if (strKey.CompareNoCase(_T("ForegroundHorAlignMode")) == 0)
	{
		pBtnBaseProp->m_eForegroundHorAlignMode.InitByEnumString(strValue);
	}
	else if (strKey.CompareNoCase(_T("ForegroundHorAlign")) == 0)
	{
		long nValue = _ttoi(strValue);
		if (IsEnableDPI())
		{
			GetDPIPointX(nValue);
		}
		CRect rcTmp = pBtnBaseProp->m_rcPadding4Foreground;
		rcTmp.left = nValue;
		pBtnBaseProp->m_rcPadding4Foreground = rcTmp;
	}
	else if (strKey.CompareNoCase(_T("ForegroundVerAlignMode")) == 0)
	{
		pBtnBaseProp->m_eForegroundVerAlignMode.InitByEnumString(strValue);
	}
	else if (strKey.CompareNoCase(_T("ForegroundVerAlign")) == 0)
	{
		long nValue = _ttoi(strValue);
		if (IsEnableDPI())
		{
			GetDPIPointX(nValue);
		}
		CRect rcTmp = pBtnBaseProp->m_rcPadding4Foreground;
		rcTmp.top = nValue;
		pBtnBaseProp->m_rcPadding4Foreground = rcTmp;
	}

	return FALSE;
}

BOOL COldUIMgr::LoadToggleButton(const CString &strKey, const CString &strValue, TOGGLEBTNPROPERTIES *pToggleBtnProp)
{
	if (LoadButtonBase(strKey, strValue, pToggleBtnProp))
	{
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("BKCN")) == 0)
	{
		pToggleBtnProp->m_strBKCN = strValue;
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("BKCH")) == 0)
	{
		pToggleBtnProp->m_strBKCH = strValue;
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("BKCS")) == 0)
	{
		pToggleBtnProp->m_strBKCS = strValue;
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("BKCD")) == 0)
	{
		pToggleBtnProp->m_strBKCD = strValue;
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("FkCN")) == 0)
	{
		pToggleBtnProp->m_strFGCN = strValue;
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("FkCH")) == 0)
	{
		pToggleBtnProp->m_strFGCH = strValue;
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("FkCS")) == 0)
	{
		pToggleBtnProp->m_strFGCS = strValue;
		return TRUE;
	}
	else if (strKey.CompareNoCase(_T("FkCD")) == 0)
	{
		pToggleBtnProp->m_strFGCD = strValue;
		return TRUE;
	}

	return FALSE;
}

int COldUIMgr::LoadPushButton(BTNPROPERTIES *pBtnProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadButtonBase(strKey, strValue, pBtnProperties))
		{

		}
		else if (strKey.CompareNoCase(_T("ButtonType")) == 0)
		{
			pBtnProperties->m_eBtnType.InitByEnumString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsDefPushButton")) == 0)
		{
			pBtnProperties->m_bDefPushButton = boolFromString(strValue);
		}
	}

	return 0;
}

int COldUIMgr::LoadCheckBox(CHKPROPERTIES *pChkProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadToggleButton(strKey, strValue, pChkProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("CheckBoxType")) == 0)
		{
			pChkProperties->m_eChkType.InitByEnumString(strValue);
		}
	}

	return 0;
}

int COldUIMgr::LoadRadioButton(RADPROPERTIES *pRadProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadToggleButton(strKey, strValue, pRadProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsGroup")) == 0)
		{
			pRadProperties->m_bGroup = boolFromString(strValue);
		}
	}

	return 0;
}

int COldUIMgr::LoadStatic(TXTPROPERTIES *pStaProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pStaProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("Caption")) == 0)
		{
			pStaProperties->m_strCaption = strValue;
		}
		else if (strKey.CompareNoCase(_T("Nowrap")) == 0)
		{
			pStaProperties->m_bNoWrap = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsPathEllipsis")) == 0)
		{
			pStaProperties->m_bPathEllipsis = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("UseToolTip")) == 0)
		{
			pStaProperties->m_bUseToolTip = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ToolTips")) == 0)
		{
			pStaProperties->m_strToolTip == strValue.GetBuffer(0);
		}
		else if (strKey.CompareNoCase(_T("IsR2LReadOrder")) == 0)
		{
			pStaProperties->m_bR2LReadOrder = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("HorAlignMode")) == 0)
		{
			pStaProperties->m_eTextHorAlignMode.InitByEnumString(strValue);
		}
		else if (strKey.CompareNoCase(_T("HorAlig")) == 0)
		{
			long nValue = _ttoi(strValue);
			if (IsEnableDPI())
			{
				GetDPIPointX(nValue);
			}

			CRect rcTmp = pStaProperties->m_rcPadding4Text;
			rcTmp.left = nValue;
			pStaProperties->m_rcPadding4Text = rcTmp;
		}
		else if (strKey.CompareNoCase(_T("VerAlignMode")) == 0)
		{
			pStaProperties->m_eTextVerAlignMode.InitByEnumString(strValue);
		}
		else if (strKey.CompareNoCase(_T("VerAlig")) == 0)
		{
			long nValue = _ttoi(strValue);
			if (IsEnableDPI())
			{
				GetDPIPointY(nValue);
			}

			CRect rcTmp = pStaProperties->m_rcPadding4Text;
			rcTmp.top = nValue;
			pStaProperties->m_rcPadding4Text = rcTmp;
		}
		else if (strKey.CompareNoCase(_T("FontN")) == 0)
		{
			ParseFontProperty(strValue, pStaProperties->m_strResFontIDN);
		}
		else if (strKey.CompareNoCase(_T("IsDSameNFont")) == 0)
		{
			pStaProperties->m_bDSameNFont = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("FontD")) == 0)
		{
			ParseFontProperty(strValue, pStaProperties->m_strResFontIDD);
		}
		else if (strKey.CompareNoCase(_T("ColorN")) == 0)
		{
			ParseColorProperty(strValue, pStaProperties->m_strResColorIDN);
		}
		else if (strKey.CompareNoCase(_T("IsDSameNColor")) == 0)
		{
			pStaProperties->m_bDSameNColor = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ColorD")) == 0)
		{
			ParseColorProperty(strValue, pStaProperties->m_strResColorIDD);
		}
		else if (strKey.CompareNoCase(_T("IsSpecifyImages")) == 0)
		{
			pStaProperties->m_bSpecifyBackgroundImages = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsCombineImages")) == 0)
		{
			pStaProperties->m_bCombineBackgroundImages = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("BkCombine")) == 0)
		{
			pStaProperties->m_strBKCombine = strValue;
		}
		else if (strKey.CompareNoCase(_T("BkN")) == 0)
		{
			pStaProperties->m_strBKN = strValue;
		}
		else if (strKey.CompareNoCase(_T("BkD")) == 0)
		{
			pStaProperties->m_strBKD = strValue;
		}
	}

	return 0;
}

int COldUIMgr::LoadEdit(EDTPROPERTIES *pEdtProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pEdtProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsTabStop")) == 0)
		{
			pEdtProperties->m_bTabStop = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsHScroll")) == 0)
		{
			pEdtProperties->m_bHScroll = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsVScroll")) == 0)
		{
			pEdtProperties->m_bVScroll = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsWantReturn")) == 0)
		{
			pEdtProperties->m_bWantReturn = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsAutoHScroll")) == 0)
		{
			pEdtProperties->m_bAutoHScroll = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsAutoVScroll")) == 0)
		{
			pEdtProperties->m_bAutoVScroll = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsLowerCase")) == 0)
		{
			pEdtProperties->m_bLowercase = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsNumber")) == 0)
		{
			pEdtProperties->m_bNumber = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsPassword")) == 0)
		{
			pEdtProperties->m_bPassword = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsReadOnly")) == 0)
		{
			pEdtProperties->m_bReadOnly = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsUpperCase")) == 0)
		{
			pEdtProperties->m_bUppercase = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsMultiline")) == 0)
		{
			pEdtProperties->m_bMultiline = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsR2LReadOrder")) == 0)
		{
			pEdtProperties->m_bR2LReadOrder = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("HorAlig")) == 0)
		{
			long nValue = _ttoi(strValue);
			if (IsEnableDPI())
			{
				GetDPIPointX(nValue);
			}

			CRect rcTmp = pEdtProperties->m_rcPadding;
			rcTmp.left = nValue;
			pEdtProperties->m_rcPadding = rcTmp;
		}
		else if (strKey.CompareNoCase(_T("VerAlig")) == 0)
		{
			long nValue = _ttoi(strValue);
			if (IsEnableDPI())
			{
				GetDPIPointY(nValue);
			}
			CRect rcTmp = pEdtProperties->m_rcPadding;
			rcTmp.top = nValue;
			pEdtProperties->m_rcPadding = rcTmp;
		}
		else if (strKey.CompareNoCase(_T("HorAligMode")) == 0)
		{
			pEdtProperties->m_eTextHorAlignMode.InitByEnumString(strValue);
		}
		else if (strKey.CompareNoCase(_T("FontN")) == 0)
		{
			ParseFontProperty(strValue, pEdtProperties->m_strResFontIDN);
		}
		else if (strKey.CompareNoCase(_T("Tag")) == 0)
		{
			CString strTag = strValue;
			strTag = strTag.Right(strTag.GetLength() - 1);
			strTag = strTag.Left(strTag.GetLength() - 1);
			pEdtProperties->m_strTag = strTag;
		}
	}

	return 0;
}

int COldUIMgr::LoadComboBox(CMBPROPERTIES *pCmbProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pCmbProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("ComboStyle")) == 0)
		{
			if (strValue == _T("0"))
			{
				pCmbProperties->m_eComboStyle = CS_SIMPLE;
			}
			else if (strValue == _T("1"))
			{
				pCmbProperties->m_eComboStyle = CS_DROPDOWN;
			}
			else if (strValue == _T("2"))
			{
				pCmbProperties->m_eComboStyle = CS_DROPDOWNLIST;
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else if (strKey.CompareNoCase(_T("IsTabStop")) == 0)
		{
			pCmbProperties->m_bTabStop = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsVScroll")) == 0)
		{
			pCmbProperties->m_bVScroll = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsAutoHScroll")) == 0)
		{
			pCmbProperties->m_bAutoHScroll = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsDisableNoScroll")) == 0)
		{
			pCmbProperties->m_bDisableNoScroll = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsSort")) == 0)
		{
			pCmbProperties->m_bSort = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsUpperCase")) == 0)
		{
			pCmbProperties->m_bUppercase = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsLowerCase")) == 0)
		{
			pCmbProperties->m_bLowercase = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("DropListHeight")) == 0)
		{
			pCmbProperties->m_lDropListHeight = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("BtnWidth")) == 0)
		{
			pCmbProperties->m_lBtnWidth = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("FontN")) == 0)
		{
			ParseFontProperty(strValue, pCmbProperties->m_strResFontIDN);
		}
		else if (strKey.CompareNoCase(_T("Tag")) == 0)
		{
			CString strTag = strValue;
			strTag = strTag.Right(strTag.GetLength() - 1);
			strTag = strTag.Left(strTag.GetLength() - 1);
			pCmbProperties->m_strTag = strTag;
		}
	}

	return 0;
}

int COldUIMgr::LoadSpinButton(SPNPROPERTIES *pSpnProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pSpnProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsVertical")) == 0)
		{
			pSpnProperties->m_bVertical = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("Alignment")) == 0)
		{
			pSpnProperties->m_eAlignMode.InitByEnumString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsTabStop")) == 0)
		{
			pSpnProperties->m_bTabStop = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsAutoBuddy")) == 0)
		{
			pSpnProperties->m_bAutoBuddy = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsSetBuddyInt")) == 0)
		{
			pSpnProperties->m_bSetBuddyInt = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsNoThousands")) == 0)
		{
			pSpnProperties->m_bNoThousands = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsWrap")) == 0)
		{
			pSpnProperties->m_bWrap = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsArrowKeys")) == 0)
		{
			pSpnProperties->m_bArrowKeys = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsHotTrack")) == 0)
		{
			pSpnProperties->m_bHotTrack = boolFromString(strValue);
		}
	}

	return 0;
}

int COldUIMgr::LoadProgress(PRGPROPERTIES *pPrgProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pPrgProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsVertical")) == 0)
		{
			pPrgProperties->m_bVertical = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsShowText")) == 0)
		{
			pPrgProperties->m_bShowText = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("Thumb")) == 0)
		{
			pPrgProperties->m_strThumbN = strValue;
			pPrgProperties->m_strThumbD = strValue;
		}
		else if (strKey.CompareNoCase(_T("HorAlig")) == 0)
		{
			long nValue = _ttoi(strValue);
			if (IsEnableDPI())
			{
				GetDPIPointX(nValue);
			}
			pPrgProperties->m_lHorAlignN = nValue;
		}
		else if (strKey.CompareNoCase(_T("VerAlig")) == 0)
		{
			long nValue = _ttoi(strValue);
			if (IsEnableDPI())
			{
				GetDPIPointY(nValue);
			}
			pPrgProperties->m_lVerAlignN = nValue;
		}
	}

	return 0;
}

int COldUIMgr::LoadSlider(SLDPROPERTIES *pSldProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pSldProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsVertical")) == 0)
		{
			pSldProperties->m_bVertical = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsUseMark")) == 0)
		{
			pSldProperties->m_bUseMark = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThumbN")) == 0)
		{
			pSldProperties->m_strThumbN = strValue;
		}
		else if (strKey.CompareNoCase(_T("ThumbH")) == 0)
		{
			pSldProperties->m_strThumbH = strValue;
		}
		else if (strKey.CompareNoCase(_T("ThumbS")) == 0)
		{
			pSldProperties->m_strThumbS = strValue;
		}
		else if (strKey.CompareNoCase(_T("ThumbD")) == 0)
		{
			pSldProperties->m_strThumbD = strValue;
		}
		else if (strKey.CompareNoCase(_T("MarkBkN")) == 0)
		{
			pSldProperties->m_strBKH = strValue;
		}
		else if (strKey.CompareNoCase(_T("MarkBkD")) == 0)
		{
			pSldProperties->m_strBKS = strValue;
		}
	}

	return 0;
}

int COldUIMgr::LoadHotKey(HOTPROPERTIES *pHotProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pHotProperties))
		{
		}
	}

	return 0;
}

int COldUIMgr::LoadIPAddress(IPAPROPERTIES *pIPAProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pIPAProperties))
		{
		}
	}

	return 0;
}

int COldUIMgr::LoadListCtrl(LSTCTRLPROPERTIES *pLstCtrlProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif

	// Create a new header resource
	HEADERPROPERTIES *pHeaderRes = new HEADERPROPERTIES();

	// Header resource ID
	CString strHeaderResID;
	strHeaderResID.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_HEADERCTRL), ++CUIMgr::m_nNextHeaderResID);
	pHeaderRes->m_strID = strHeaderResID;

	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);

			m_vpHeaderResource.push_back(pHeaderRes);

			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pLstCtrlProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("ListStyle")) == 0)
		{
			pLstCtrlProperties->m_eListStyle.InitByEnumString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ListAlign")) == 0)
		{
			pLstCtrlProperties->m_eListAlign.InitByEnumString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ListSort")) == 0)
		{
			pLstCtrlProperties->m_eListSort.InitByEnumString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsTabStop")) == 0)
		{
			pLstCtrlProperties->m_bTabStop = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsAutoArrange")) == 0)
		{
			pLstCtrlProperties->m_bAutoArrange = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsEditLabels")) == 0)
		{
			pLstCtrlProperties->m_bEditLabels = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsNoHeader")) == 0)
		{
			pLstCtrlProperties->m_bNoHeader = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsNoLabelWrap")) == 0)
		{
			pLstCtrlProperties->m_bNoLabelWrap = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsNoScroll")) == 0)
		{
			pLstCtrlProperties->m_bNoScroll = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsNoSortHeader")) == 0)
		{
			pLstCtrlProperties->m_bNoSortHeader = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsShowSelAlways")) == 0)
		{
			pLstCtrlProperties->m_bShowSelAlways = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsSingleSel")) == 0)
		{
			pLstCtrlProperties->m_bSingleSel = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsOwnerData")) == 0)
		{
			pLstCtrlProperties->m_bOwnerData = boolFromString(strValue);
		}

		else if (strKey.CompareNoCase(_T("HeaderBkN")) == 0)
		{
			pHeaderRes->m_strBKN = strValue;
		}
		else if (strKey.CompareNoCase(_T("HeaderBkH")) == 0)
		{
			pHeaderRes->m_strBKH = strValue;
		}
		else if (strKey.CompareNoCase(_T("HeaderBkS")) == 0)
		{
			pHeaderRes->m_strBKS = strValue;
		}
		else if (strKey.CompareNoCase(_T("HeaderBkD")) == 0)
		{
			pHeaderRes->m_strBKD = strValue;
		}
		else if (strKey.CompareNoCase(_T("Tag")) == 0)
		{
			CString strTag = strValue;
			strTag = strTag.Right(strTag.GetLength() - 1);
			strTag = strTag.Left(strTag.GetLength() - 1);
			pLstCtrlProperties->m_strTag = strTag;
		}
	}
	return 0;
}

int COldUIMgr::LoadTreeCtrl(TREECTRLPROPERTIES *pTreeCtrlProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pTreeCtrlProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsTabStop")) == 0)
		{
			pTreeCtrlProperties->m_bTabStop = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsCheckBoxed")) == 0)
		{
			pTreeCtrlProperties->m_bCheckBoxes = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsDisDragDrop")) == 0)
		{
			pTreeCtrlProperties->m_bDisableDragDrop = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsEditLabels")) == 0)
		{
			pTreeCtrlProperties->m_bEditLabels = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsFullRowSel")) == 0)
		{
			pTreeCtrlProperties->m_bFullRowSel = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsHasButtons")) == 0)
		{
			pTreeCtrlProperties->m_bHasButtons = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsHasLines")) == 0)
		{
			pTreeCtrlProperties->m_bHasLines = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsInfoTip")) == 0)
		{
			pTreeCtrlProperties->m_bInfoTip = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsLineAtRoot")) == 0)
		{
			pTreeCtrlProperties->m_bLinesAtRoot = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsNonEvenHeight")) == 0)
		{
			pTreeCtrlProperties->m_bNonEvenHeight = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsScroll")) == 0)
		{
			pTreeCtrlProperties->m_bScroll = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsShowSelAlways")) == 0)
		{
			pTreeCtrlProperties->m_bShowSelAlways = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsSingleExpand")) == 0)
		{
			pTreeCtrlProperties->m_bSingleExpand = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsTooltips")) == 0)
		{
			pTreeCtrlProperties->m_bToolTips = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsTrackSel")) == 0)
		{
			pTreeCtrlProperties->m_bTrackSel = boolFromString(strValue);
		}
	}

	return 0;
}

int COldUIMgr::LoadScrollBarEx(SBEXPROPERTIES *pSBExProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif

	HORSBPROPERTIES *pHorSb = new HORSBPROPERTIES;
	VERSBPROPERTIES *pVerSb = new VERSBPROPERTIES;

	pHorSb->m_bCombineThumbImages = false;
	pVerSb->m_bCombineThumbImages = false;

	LONG lLTUPx = 0;
	LONG lLTUPy = 0;
	LONG lRTDNx = 0;
	LONG lRTDNy = 0;

	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pSBExProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsVertical")) == 0)
		{
			pSBExProperties->m_bVertical = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThumbN")) == 0)
		{
			if (pSBExProperties->m_bVertical)
			{
				pVerSb->m_strThumbN = strValue;
			}
			else
			{
				pHorSb->m_strThumbN = strValue;
			}
		}
		else if (strKey.CompareNoCase(_T("ThumbH")) == 0)
		{
			if (pSBExProperties->m_bVertical)
			{
				pVerSb->m_strThumbH = strValue;
			}
			else
			{
				pHorSb->m_strThumbH = strValue;
			}
		}
		else if (strKey.CompareNoCase(_T("ThumbS")) == 0)
		{
			if (pSBExProperties->m_bVertical)
			{
				pVerSb->m_strThumbS = strValue;
			}
			else
			{
				pHorSb->m_strThumbS = strValue;
			}
		}
		else if (strKey.CompareNoCase(_T("ThumbD")) == 0)
		{
			if (pSBExProperties->m_bVertical)
			{
				pVerSb->m_strThumbD = strValue;
			}
			else
			{
				pHorSb->m_strThumbD = strValue;
			}
		}
		else if (strKey.CompareNoCase(_T("BkN")) == 0)
		{
			pSBExProperties->m_strBKN = strValue;
		}
		else if (strKey.CompareNoCase(_T("BkH")) == 0)
		{
			pSBExProperties->m_strBKH = strValue;
		}
		else if (strKey.CompareNoCase(_T("BkS")) == 0)
		{
			pSBExProperties->m_strBKS = strValue;
		}
		else if (strKey.CompareNoCase(_T("BkD")) == 0)
		{
			pSBExProperties->m_strBKD = strValue;
		}
		else if (strKey.CompareNoCase(_T("LTUPx")) == 0)
		{
			lLTUPx = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("LTUPy")) == 0)
		{
			lLTUPy = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("RTDNx")) == 0)
		{
			lRTDNx = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("RTDNy")) == 0)
		{
			lRTDNy = LongFromString(strValue);
		}
	}

	if (pSBExProperties->m_bVertical)
	{
		m_vpStyleVerScrollBar.push_back(pVerSb);
	}
	else
	{
		m_vpStyleHorScrollBar.push_back(pHorSb);
	}

	return 0;
}

int COldUIMgr::LoadThumbnail(THMPROPERTIES *pThmProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pThmProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsListStyle")) == 0)
		{
			pThmProperties->m_bListStyle = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsTabStop")) == 0)
		{
			pThmProperties->m_bTabStop = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThmBkN")) == 0)
		{
			pThmProperties->m_strItemBkN = strValue;
		}
		else if (strKey.CompareNoCase(_T("ThmBkH")) == 0)
		{
			pThmProperties->m_strItemBkH = strValue;
		}
		else if (strKey.CompareNoCase(_T("ThmBkS")) == 0)
		{
			pThmProperties->m_strItemBkS = strValue;
		}
		else if (strKey.CompareNoCase(_T("ThmBkD")) == 0)
		{
			pThmProperties->m_strItemBkD = strValue;
		}
		else if (strKey.CompareNoCase(_T("TotalWidth")) == 0)
		{
			pThmProperties->m_lTotalItemWidth = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("TotalHeight")) == 0)
		{
			pThmProperties->m_lTotalItemHeight = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThmRectLeftN")) == 0)
		{
			pThmProperties->m_lThumbnailLeftN = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThmRectTopN")) == 0)
		{
			pThmProperties->m_lThumbnailTopN = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThmRectRightN")) == 0)
		{
			pThmProperties->m_lThumbnailRightN = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThmRectBottomN")) == 0)
		{
			pThmProperties->m_lThumbnailBottomN = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThmRectLeftH")) == 0)
		{
			pThmProperties->m_lThumbnailLeftH = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThmRectTopH")) == 0)
		{
			pThmProperties->m_lThumbnailTopH = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThmRectRightH")) == 0)
		{
			pThmProperties->m_lThumbnailRightH = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("ThmRectBottomH")) == 0)
		{
			pThmProperties->m_lThumbnailBottomH = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("LblRectLeftN")) == 0)
		{
			pThmProperties->m_lLabelLeftN = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("LblRectTopN")) == 0)
		{
			pThmProperties->m_lLabelTopN = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("LblRectRightN")) == 0)
		{
			pThmProperties->m_lLabelRightN = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("LblRectBottomN")) == 0)
		{
			pThmProperties->m_lLabelBottomN = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("LblRectLeftH")) == 0)
		{
			pThmProperties->m_lLabelLeftH = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("LblRectTopH")) == 0)
		{
			pThmProperties->m_lLabelTopH = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("LblRectRightH")) == 0)
		{
			pThmProperties->m_lLabelRightH = LongFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("LblRectBottomH")) == 0)
		{
			pThmProperties->m_lLabelBottomH = LongFromString(strValue);
		}
	}

	return 0;
}

int COldUIMgr::LoadTreeList(TREELISTPROPERTIES *pTreeListProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pTreeListProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsTabStop")) == 0)
		{
			pTreeListProperties->m_bTabStop = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsVariableItemHeight")) == 0)
		{
			pTreeListProperties->m_bVariableItemHeight = boolFromString(strValue);
		}
	}

	return 0;
}

int COldUIMgr::LoadHtmlCtrl(HTMLCTRLPROPERTIES *pHtmlCtrlProperties)
{
	CString strBuf;
#if(_MSC_VER <= 1200) // VC6.0
	DWORD nOldPos = m_fileTheUI.GetPosition();
#else
	ULONGLONG nOldPos = m_fileTheUI.GetPosition();
#endif
	while (m_fileTheUI.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.Left(1) == '[')
		{
			m_fileTheUI.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (LoadControlCommon(strKey, strValue, pHtmlCtrlProperties))
		{
		}
		else if (strKey.CompareNoCase(_T("IsHideBorder")) == 0)
		{
			pHtmlCtrlProperties->m_bHideBorder = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsHideScrollBar")) == 0)
		{
			pHtmlCtrlProperties->m_bHideScrollBar = boolFromString(strValue);
		}
		else if (strKey.CompareNoCase(_T("IsHideContextMenu")) == 0)
		{
			pHtmlCtrlProperties->m_bHideContextMenu = boolFromString(strValue);
		}
	}

	return 0;
}

int COldUIMgr::AddControlToWndList(LPCTSTR lpszSection, CTRLPROPERTIES *pCtrl)
{
	CString strSection = lpszSection;
	int nIndex = strSection.ReverseFind('_');
	ASSERT(nIndex > 0);
	strSection = strSection.Left(nIndex);

	size_t nSize = m_vpWindows.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		if (strSection.Compare(pUIProp->m_strID) == 0)
		{
			pUIProp->m_vControls.push_back(pCtrl);
			break;
		}
	}

	return 0;
}

UIWNDPROPERTIES *COldUIMgr::GetUIWNDPROPERTIES(LPCTSTR lpszWindowID)
{
	UIWNDPROPERTIES *pRet = NULL;
	size_t nSize = m_vpWindows.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		if (pUIProp->m_strID == lpszWindowID)
		{
			pRet = pUIProp;
			break;
		}
	}

	return pRet;
}

CTRLPROPERTIES *COldUIMgr::GetCONTROL(LPCTSTR lpszWindowID, LPCTSTR lpszCtrlID)
{
	UIWNDPROPERTIES *pWnd = GetUIWNDPROPERTIES(lpszWindowID);

	size_t nSize = pWnd->m_vControls.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		if (pWnd->m_vControls[i]->m_strID == lpszCtrlID)
		{
			return pWnd->m_vControls[i];
		}
	}

	return NULL;
}

RESCOLORPROPERTIES *COldUIMgr::FindColorResource(const IUI_VARIANT &varColor)
{
	RESCOLORPROPERTIES *pRetProp = NULL;

	size_t nSize = m_vpColorRes.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		RESCOLORPROPERTIES *pColorProp = m_vpColorRes[i];

		COLORREF crResource = (LONG)pColorProp->m_crColor;
		if (crResource != (COLORREF)(LONG)varColor)
		{
			continue;
		}

		pRetProp = pColorProp;
		break;
	}

	return pRetProp;
}

RESFONTPROPERTIES *COldUIMgr::FindFontResource(const LOGFONT &lf)
{
	RESFONTPROPERTIES *pRetProp = NULL;

	size_t nSize = m_vpFontRes.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		RESFONTPROPERTIES *pFontProp = m_vpFontRes[i];

		LOGFONT lfGet = pFontProp->m_logFont;
		if (memcmp(&lfGet, &lf, sizeof(LOGFONT)) == 0)
		{
			pRetProp = pFontProp;
			break;
		}
	}

	return pRetProp;
}

//////////////////////////////////////////////////////////////////////////
//

BOOL CUIMgr::m_bLoadFromFile = TRUE;
BOOL CUIMgr::m_bConverting = FALSE;
BOOL CUIMgr::m_bDelayLoadWindow = TRUE;
CString CUIMgr::m_strUIPath = _T("");
CString CUIMgr::m_strUIPathWithoutTitle = _T("");
CString CUIMgr::m_strUIResourcePath = _T("");
CString CUIMgr::m_strUIID = _T("");
CString CUIMgr::m_strUIResourcehID = _T("");
CString CUIMgr::m_strUIResourceType = _T("");
LONG CUIMgr::m_lXUIMajorVersion = -1;
LONG CUIMgr::m_lXUIMinorVersion = -1;
LONG CUIMgr::m_lXUIBuildVersion = -1;
LONG CUIMgr::m_lXUIChangeList = -1;
LONG CUIMgr::m_lLibUIDKMajorVersion = 7;
LONG CUIMgr::m_lLibUIDKMinorVersion = 0;
LONG CUIMgr::m_lLibUIDKBuildVersion = LIBUIDK_BUILD_VERSION;
LONG CUIMgr::m_lLogPixelsX = -1;
LONG CUIMgr::m_lLogPixelsY = -1;
BOOL CUIMgr::m_bEnableDPI = FALSE;
BOOL CUIMgr::m_bRunAsDPIX = FALSE;
BOOL CUIMgr::m_bRunAsDPIY = FALSE;
int CUIMgr::m_nRunAsDPIX = 96;
int CUIMgr::m_nRunAsDPIY = 96;

std::vector<UIWNDPROPERTIES *> CUIMgr::m_vpWindows;
// CUIMgr::m_vIDs only load from Resource.h, after save Resource.h, reload Resource.h to update CUIMgr::m_vIDs.
std::vector<std::pair<CString, int> > CUIMgr::m_vIDs;
std::map<CString, int> CUIMgr::m_mapStrIntIDs;	// Another m_vIDs.
std::map<int, CString> CUIMgr::m_mapIntStrIDs;	// Another m_vIDs.
CMemStdioFile CUIMgr::m_fileResourceH;
// Font Resource
CString CUIMgr::m_strCurrentFont;
FONTRESOURCEITEM CUIMgr::m_resDefaultFont;
std::vector<FONTRESOURCEITEM *> CUIMgr::m_vpResFont;

// Color Resource
RESCOLORPROPERTIES CUIMgr::m_resDefaultColor;
std::vector<RESCOLORPROPERTIES *> CUIMgr::m_vpColorRes;
// Style
std::vector<BTNPROPERTIES *> CUIMgr::m_vpStylePushButton;
std::vector<CHKPROPERTIES *> CUIMgr::m_vpStyleCheckBox;
std::vector<RADPROPERTIES *> CUIMgr::m_vpStyleRadioButton;
std::vector<CMBPROPERTIES *> CUIMgr::m_vpStyleComboBox;
std::vector<TXTPROPERTIES *> CUIMgr::m_vpStyleStatic;
std::vector<EDTPROPERTIES *> CUIMgr::m_vpStyleEdit;
std::vector<SLDPROPERTIES *> CUIMgr::m_vpStyleSlider;
std::vector<PRGPROPERTIES *> CUIMgr::m_vpStyleProgress;
std::vector<SPNPROPERTIES *> CUIMgr::m_vpStyleSpin;
std::vector<HOTPROPERTIES *> CUIMgr::m_vpStyleHotKey;
std::vector<IPAPROPERTIES *> CUIMgr::m_vpStyleIPAddress;
std::vector<SLDEXPROPERTIES *> CUIMgr::m_vpStyleSliderEx;
std::vector<LSTCTRLPROPERTIES *> CUIMgr::m_vpStyleListCtrl;
std::vector<TREECTRLPROPERTIES *> CUIMgr::m_vpStyleTreeCtrl;
std::vector<TREELISTPROPERTIES *> CUIMgr::m_vpStyleTreeList;
std::vector<RICHEDITPROPERTIES *> CUIMgr::m_vpStyleRichEdit;
std::vector<IMREPROPERTIES *> CUIMgr::m_vpStyleIMRichEdit;
std::vector<HTMLCTRLPROPERTIES *> CUIMgr::m_vpStyleHtmlCtrl;
std::vector<DATETIMECTRLPROPERTIES *> CUIMgr::m_vpStyleDateTime;
std::vector<WAVECTRLPROPERTIES *> CUIMgr::m_vpStyleWaveCtrl;
std::vector<MENUPROPERTIES *> CUIMgr::m_vpStyleMenu;
std::vector<MENUBARPROPERTIES *> CUIMgr::m_vpStyleMenuBar;
std::vector<HEADERPROPERTIES *> CUIMgr::m_vpStyleHeader;
std::vector<HORSBPROPERTIES *> CUIMgr::m_vpStyleHorScrollBar;
std::vector<VERSBPROPERTIES *> CUIMgr::m_vpStyleVerScrollBar;
std::vector<SPLITTERPROPERTIES *> CUIMgr::m_vpStyleSplitter;
std::vector<LINEWLPROPERTIES *> CUIMgr::m_vpStyleWLLine;
std::vector<STACKPNLPROPERTIES *> CUIMgr::m_vpStylePanelStack;
std::vector<PICWLPROPERTIES *> CUIMgr::m_vpStyleWLPic;

std::map<CString, CString> CUIMgr::m_mapLanguage;
std::map<HWND, CString> CUIMgr::m_mapCreatedHWndStringID;
COLORREF CUIMgr::m_crAlphaBlend = RGB(255, 255, 255);
ULONG_PTR CUIMgr::m_gdiplusToken = NULL;

int CUIMgr::m_nNextFontResID = 0;
int CUIMgr::m_nNextColorResID = 0;
int CUIMgr::m_nNextHeaderResID = 0;

LPCTSTR LibUIDK::g_pszDefaultFontID = _T("{20C69D19-3DFD-4e9f-8C0D-B1CE0A89A3D6}");

BOOL g_bCheckChangeList = TRUE;	// if the the.xui load by UIShop.exe, don't check change list, otherwise, can't load the the.xui.

void feedtabs(XML::IXMLDOMDocumentPtr pDoc, XML::IXMLDOMNodePtr pParentNode, XML::IXMLDOMNodePtr pRefNode, int tabs)
{
	CString s("\r\n");

	// add \r\n\t\t\t...(tabs) before first child
	if (pRefNode)
	{
		for (int i = 0; i < tabs; i++)
		{
			s += "\t";
		}

		pParentNode->insertBefore(pDoc->createTextNode(_bstr_t(s)), (IUnknown *)pRefNode);
	}

	// add \r\n\t\t\t...((tabs-1)) after last child
	else
	{
		for (int i = 0; i < tabs - 1; i++)
		{
			s += "\t";
		}

		pParentNode->appendChild(pDoc->createTextNode(_bstr_t(s)));
	}
}

void WellFormXml(XML::IXMLDOMDocumentPtr pDoc, XML::IXMLDOMNodePtr pParentNode, int tabs)
{
	if (pParentNode->hasChildNodes())
	{
		// add "\r\n\t.."(tabs '\t') before first child
		XML::IXMLDOMNodePtr pFirstChild = pParentNode->GetfirstChild();
		feedtabs(pDoc, pParentNode, pFirstChild, tabs);

		// add "\r\n\t.."(tabs-1 '\t') after last child
		feedtabs(pDoc, pParentNode, NULL, tabs);

		// apply to children
		WellFormXml(pDoc, pFirstChild, tabs + 1);
	}

	// apply to brother
	XML::IXMLDOMNodePtr pNextSibling = pParentNode->GetnextSibling();
	if (pNextSibling)
	{
		XML::IXMLDOMNodePtr pGrandParent = pParentNode->GetparentNode();
		if ((pGrandParent != NULL) && wcscmp(pNextSibling->GetnodeTypeString(), L"text"))
		{
			// add "\r\n\t.."(tabs-1 '\t') before next sibling
			feedtabs(pDoc, pGrandParent, pNextSibling, tabs - 1);
			WellFormXml(pDoc, pNextSibling, tabs);
		}
	}
}

XML::IXMLDOMElementPtr GetElement(XML::IXMLDOMElementPtr pParentElement, _bstr_t strElement)
{
	XML::IXMLDOMElementPtr pRet = NULL;

	XML::IXMLDOMNodePtr pNode = pParentElement->GetfirstChild();
	while (pNode != NULL)
	{
		_bstr_t strName = pNode->GetnodeName();
		if (strName == strElement)
		{
			pRet = pNode;
			break;
		}

		pNode = pNode->GetnextSibling();
	}

	return pRet;
}

//////////////////////////////////////////////////////////////////////////
// for save

XML::IXMLDOMElementPtr SaveOpenProgram(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pParentElement)
{
	HRESULT hr = S_OK;
	XML::IXMLDOMElementPtr pElement = xmlDoc->createElement("Configuration");
	pElement->setAttribute(_T("OpenProgram"), _variant_t("UIShop.exe"));
	hr = pParentElement->appendChild(pElement);

	return pElement;
}

XML::IXMLDOMElementPtr SaveXUIVersion(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pParentElement)
{
	HRESULT hr = S_OK;
	XML::IXMLDOMElementPtr pVersionElement = xmlDoc->createElement("Version");

	// Version
	CString strVersion;
	strVersion.Format(_T("%02d%02d"), CUIMgr::GetLibUIDKMajorVersion(), CUIMgr::GetLibUIDKMinorVersion());
	pVersionElement->setAttribute(_T("Version"), strVersion.GetBuffer(0));

	// Build
	CString strBuild;
	strBuild.Format(_T("%d"), LIBUIDK_BUILD_VERSION);
	pVersionElement->setAttribute(_T("Build"), _variant_t(strBuild));

	// ChangeList
	CString strChangeList;
	strChangeList.Format(_T("%d"), CUIMgr::GetLibUIDKChangeList());
	pVersionElement->setAttribute(_T("ChangeList"), _variant_t(strChangeList));

	hr = pParentElement->appendChild(pVersionElement);

	return pVersionElement;
}

XML::IXMLDOMElementPtr SaveConfig(XML::IXMLDOMDocumentPtr xmlDoc)
{
	HRESULT hr = S_OK;

	XML::IXMLDOMProcessingInstructionPtr pi = xmlDoc->createProcessingInstruction(
			_T("xml"),
			_T("version='1.0' encoding='utf-8'"));

	hr = xmlDoc->appendChild(pi);
	XML::IXMLDOMCommentPtr pc = xmlDoc->createComment(_T("DO NOT EDIT OR DELETE THIS LIBUIDK UI FILE!"));
	hr = xmlDoc->appendChild(pc);
	//#ifdef _DEBUG
	pc = xmlDoc->createComment(_T("www.iuishop.com"));
	hr = xmlDoc->appendChild(pc);

	pc = xmlDoc->createComment(_T("iuishop@iuishop.com"));
	hr = xmlDoc->appendChild(pc);
	//#endif

	pc = xmlDoc->createComment(_T("LibUIDK"));
	hr = xmlDoc->appendChild(pc);

	XML::IXMLDOMElementPtr pRootElement = xmlDoc->createElement("LibUIDKUI");
	hr = xmlDoc->appendChild(pRootElement);

	SaveOpenProgram(xmlDoc, pRootElement);
	SaveXUIVersion(xmlDoc, pRootElement);

	return pRootElement;
}

// If save the storyboard item's property, DON'T check default value.
// Return saved count.
int SaveElementAttributeVariant(XML::IXMLDOMDocumentPtr xmlDoc,
	XML::IXMLDOMElementPtr pParentElement,
	const IUI_VARIANT &variant,
	BOOL bCheckDefaultValue = TRUE)
{
	int nSavedCount = 0;

	if (variant.vtEx == VT_EMPTY
		|| variant.vtEx == VT_EX_CONTROL_ID_MACRO
		|| variant.vtEx == VT_EX_STYLE_PUSHBUTTON
		|| variant.vtEx == VT_EX_STYLE_EDIT
		|| variant.vtEx == VT_EX_RESOURCE_FONT
		|| variant.vtEx == VT_EX_RESOURCE_COLOR
		|| variant.vtEx == VT_EX_STYLE_MENU
		|| variant.vtEx == VT_EX_STYLE_HEADERCTRL
		|| variant.vtEx == VT_EX_STYLE_SPIN
		|| variant.vtEx == VT_EX_STYLE_HORSCROLLBAR
		|| variant.vtEx == VT_EX_STYLE_VERSCROLLBAR
		|| variant.vtEx == VT_EX_WINDOW_ID
		|| variant.vtEx == VT_EX_WINDOW_ID_NOT_INCLUDE_SELF
		|| variant.vtEx == VT_EX_TASKMGR_ID)
	{
		if (bCheckDefaultValue)
		{
			if (variant.vt == VT_BOOL || variant.vt == VT_I4 || variant.vt == VT_BSTR)	// If the value equal to the default value, DO NOT write to the .XUI file.
			{
				if (variant == variant.varDefaultValue)
				{
					return nSavedCount;
				}
			}
			if (variant.vtEx == VT_EX_RESOURCE_FONT || variant.vtEx == VT_EX_RESOURCE_COLOR)
			{
				if (variant == _T(""))
				{
					return nSavedCount;
				}
			}
		}

		XML::IXMLDOMElementPtr pElement = GetElement(pParentElement, variant.strXUIElementName);
		if (pElement == NULL)
		{
			pElement = xmlDoc->createElement(variant.strXUIElementName);
		}

		if (variant.vt == VT_BOOL)
		{
			pElement->setAttribute(variant.strXUIAttributeName, bool(variant) ? _T("true") : _T("false"));
		}
		else
		{
			pElement->setAttribute(variant.strXUIAttributeName, variant);
		}
		pParentElement->appendChild(pElement);
		nSavedCount++;

		return nSavedCount;
	}
	else if (variant.vtEx == VT_EX_RECT)
	{
		CRect rect = variant;
		if (bCheckDefaultValue)
		{
			IUI_VARIANT varDefaultRect = variant.varDefaultValue;
			varDefaultRect.vtEx = VT_EX_RECT;
			CRect rcDefault = varDefaultRect;

			if (rect == rcDefault)
			{
				return nSavedCount;
			}
		}

		XML::IXMLDOMElementPtr pElement = GetElement(pParentElement, variant.strXUIElementName);
		if (pElement == NULL)
		{
			pElement = xmlDoc->createElement(variant.strXUIElementName);
		}

		CString strValue;
		strValue.Format(_T("%d"), rect.left);
		pElement->setAttribute(_T("Left"), strValue.GetBuffer(0));
		strValue.Format(_T("%d"), rect.top);
		pElement->setAttribute(_T("Top"), strValue.GetBuffer(0));
		strValue.Format(_T("%d"), rect.right);
		pElement->setAttribute(_T("Right"), strValue.GetBuffer(0));
		strValue.Format(_T("%d"), rect.bottom);
		pElement->setAttribute(_T("Bottom"), strValue.GetBuffer(0));
		pParentElement->appendChild(pElement);
		nSavedCount++;

		return nSavedCount;
	}
	else if (variant.vtEx == VT_EX_RECT_WH)
	{
		CRect rect = variant;
		if (bCheckDefaultValue)
		{
			IUI_VARIANT varDefaultRect = variant.varDefaultValue;
			varDefaultRect.vtEx = VT_EX_RECT_WH;
			CRect rcDefault = varDefaultRect;

			if (rect == rcDefault)
			{
				return nSavedCount;
			}
		}

		XML::IXMLDOMElementPtr pElement = GetElement(pParentElement, variant.strXUIElementName);
		if (pElement == NULL)
		{
			pElement = xmlDoc->createElement(variant.strXUIElementName);
		}

		CString strValue;
		strValue.Format(_T("%d,%d"), rect.Width(), rect.Height());
		pElement->setAttribute(_T("Size"), strValue.GetBuffer(0));
		strValue.Format(_T("%d,%d"), rect.left, rect.top);
		pElement->setAttribute(_T("Location"), strValue.GetBuffer(0));
		pParentElement->appendChild(pElement);
		nSavedCount++;

		return nSavedCount;
	}
	else if (variant.vtEx == VT_EX_POINT)
	{
		CPoint pt = (POINT)variant;
		if (bCheckDefaultValue)
		{
			IUI_VARIANT varDefaultPoint = variant.varDefaultValue;
			varDefaultPoint.vtEx = VT_EX_POINT;
			POINT ptDefault = varDefaultPoint;

			if (pt == ptDefault)
			{
				return nSavedCount;
			}
		}

		XML::IXMLDOMElementPtr pElement = GetElement(pParentElement, variant.strXUIElementName);
		if (pElement == NULL)
		{
			pElement = xmlDoc->createElement(variant.strXUIElementName);
		}

		pElement->setAttribute(_T("X"), pt.x);
		pElement->setAttribute(_T("Y"), pt.y);
		pParentElement->appendChild(pElement);
		nSavedCount++;

		return nSavedCount;
	}
	else if (variant.vtEx == VT_EX_COLOR)
	{
		XML::IXMLDOMElementPtr pElement = GetElement(pParentElement, variant.strXUIElementName);
		if (pElement == NULL)
		{
			pElement = xmlDoc->createElement(variant.strXUIElementName);
		}

		CString strValue;
		strValue.Format(_T("#%08X"), (COLORREF)variant);
		pElement->setAttribute(variant.strXUIAttributeName, strValue.GetBuffer(0));
		pParentElement->appendChild(pElement);
		nSavedCount++;

		return nSavedCount;
	}
	else if (variant.vtEx == VT_EX_FONT)
	{
		XML::IXMLDOMElementPtr pElement = GetElement(pParentElement, variant.strXUIElementName);
		if (pElement == NULL)
		{
			pElement = xmlDoc->createElement(variant.strXUIElementName);
		}

		LOGFONT lf = variant;
		pElement->setAttribute(_T("Height"), lf.lfHeight);
		pElement->setAttribute(_T("Width"), lf.lfWidth);
		pElement->setAttribute(_T("lfEscapement"), lf.lfEscapement);
		pElement->setAttribute(_T("Orientation"), lf.lfOrientation);
		pElement->setAttribute(_T("Weight"), lf.lfWeight);
		pElement->setAttribute(_T("Italic"), lf.lfItalic);
		pElement->setAttribute(_T("Underline"), lf.lfUnderline);
		pElement->setAttribute(_T("StrikeOut"), lf.lfStrikeOut);
		pElement->setAttribute(_T("CharSet"), lf.lfCharSet);
		pElement->setAttribute(_T("OutPrecision"), lf.lfOutPrecision);
		pElement->setAttribute(_T("ClipPrecision"), lf.lfClipPrecision);
		pElement->setAttribute(_T("Quality"), lf.lfQuality);
		pElement->setAttribute(_T("PitchAndFamily"), lf.lfPitchAndFamily);
		pElement->setAttribute(_T("FaceName"), lf.lfFaceName);
		pParentElement->appendChild(pElement);
		nSavedCount++;

		return nSavedCount;
	}
	else if (variant.vtEx == VT_EX_BITMAPPATH)
	{
		if (variant != _T(""))
		{
			XML::IXMLDOMElementPtr pElement = GetElement(pParentElement, variant.strXUIElementName);
			if (pElement == NULL)
			{
				pElement = xmlDoc->createElement(variant.strXUIElementName);
			}

			pElement->setAttribute(variant.strXUIAttributeName, variant);
			pParentElement->appendChild(pElement);
			nSavedCount++;
		}

		return nSavedCount;
	}
	else if (variant.vtEx == VT_EX_IMAGE_RESIZE_MODE
		|| variant.vtEx == VT_EX_LAYOUT_HOR
		|| variant.vtEx == VT_EX_LAYOUT_VER
		|| variant.vtEx == VT_EX_TEXT_ALIGN_HOR
		|| variant.vtEx == VT_EX_TEXT_ALIGN_VER
		|| variant.vtEx == VT_EX_FOREGROUND_ALIGN_HOR
		|| variant.vtEx == VT_EX_FOREGROUND_ALIGN_VER
		|| variant.vtEx == VT_EX_BUTTON_TYPE
		|| variant.vtEx == VT_EX_CHECKBOX_TYPE
		|| variant.vtEx == VT_EX_WINDOW_TYPE
		|| variant.vtEx == VT_EX_COMBOBOX_STYLE
		|| variant.vtEx == VT_EX_SPN_ALIGN
		|| variant.vtEx == VT_EX_SLD_EXT_STYLE
		|| variant.vtEx == VT_EX_LST_STYLE
		|| variant.vtEx == VT_EX_LST_ALIGN
		|| variant.vtEx == VT_EX_LST_SORT
		|| variant.vtEx == VT_EX_DATETIME_FORMAT
		|| variant.vtEx == VT_EX_BACKGROUND_TYPE
		|| variant.vtEx == VT_EX_PROGRESS_THUMB_MODE
		|| variant.vtEx == VT_EX_PROGRESS_THUMB_POSITION
		|| variant.vtEx == VT_EX_ALPHA_FORMAT
		|| variant.vtEx == VT_EX_MENU_BORDER_STYLE
		|| variant.vtEx == VT_EX_SPLITTER_ORIENTATION
		|| variant.vtEx == VT_EX_STACK_PANEL_ORIENTATION
		|| variant.vtEx == VT_EX_PEN_STYLE)
	{
		if (bCheckDefaultValue)
		{
			if (variant == variant.varDefaultValue)
			{
				return nSavedCount;
			}
		}

		XML::IXMLDOMElementPtr pElement = GetElement(pParentElement, variant.strXUIElementName);
		if (pElement == NULL)
		{
			pElement = xmlDoc->createElement(variant.strXUIElementName);
		}

		pElement->setAttribute(variant.strXUIAttributeName, variant.GetEnumString());
		pParentElement->appendChild(pElement);
		nSavedCount++;

		return nSavedCount;
	}
	else
	{
		ASSERT(FALSE);
	}

	return nSavedCount;
}

int SaveControlProperties(XML::IXMLDOMDocumentPtr xmlDoc,
	XML::IXMLDOMElementPtr pControl,
	const CTRLPROPERTIES *pCtrlProp,
	const CTRLPROPERTIES *pParentCtrlProp)
{
	if (pCtrlProp == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	// Save control's parent id.
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_UIWND && pParentCtrlProp != NULL)
	{
		XML:: IXMLDOMElementPtr pElement = xmlDoc->createElement("Parent");
		pElement->setAttribute("ID", (LPCTSTR)pParentCtrlProp->m_strID);
		pControl->appendChild(pElement);
	}

	// Save control's properties
	LONG lVarCount = pCtrlProp->GetPropertyCount();
	for (LONG i = 0; i < lVarCount ; i++)
	{
		const IUI_VARIANT *pVariant = pCtrlProp->GetProperty(i);
		if (pVariant->vtEx == VT_EX_STRUCT_SIZE || pVariant->vtEx == VT_EX_CONTROL_ID || pVariant->vtEx == VT_EX_CONTROL_TYPE)
		{
			continue;
		}

		if (pVariant->bNeedToConvert)	// If the property's bNeedToConvert field is true, don't save to *.xui file(but need to read).
		{
			continue;
		}

		if (!pVariant->bSaveToSkinFile)
		{
			continue;
		}

#ifdef _DEBUG
		CString strXUIName = (LPCTSTR)pVariant->strXUIElementName;
#endif // _DEBUG
		int nRet = SaveElementAttributeVariant(xmlDoc, pControl, *pVariant);
	}

	return 0;
}

CTRLPROPERTIES *FindCtrlProp(const UIWNDPROPERTIES *pUIWndProp, LPCTSTR lpszCtrlID)
{
	if (pUIWndProp == NULL)
	{
		return NULL;
	}
	if (lpszCtrlID == NULL)
	{
		return NULL;
	}

	CTRLPROPERTIES *pRet = NULL;

	int nCtrlCount = (int)pUIWndProp->m_vControls.size();

	for (int i = 0; i < nCtrlCount; ++i)
	{
		if (pUIWndProp->m_vControls[i]->m_strID == lpszCtrlID)
		{
			pRet = pUIWndProp->m_vControls[i];
			break;
		}
	}

	return pRet;
}

// return save properties count, not include ID properties.
int SaveCtrlStoryProp(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pStoryFrameNode,
	const CTRLPROPERTIES *pNearCtrlProp, const CTRLPROPERTIES *pCtrlProp)
{
	if (pNearCtrlProp == NULL || pCtrlProp == NULL)
	{
		return -1;
	}

	// create control section
	XML::IXMLDOMElementPtr pControl = xmlDoc->createElement((LPCTSTR)GetControlTypeString(pCtrlProp->m_eControlType));

	// Save control's properties
	int nSavedCount = 0;
	LONG lVarCount = pCtrlProp->GetPropertyCount();
	for (LONG i = 0; i < lVarCount ; i++)
	{
		const IUI_VARIANT *pVariant = pCtrlProp->GetProperty(i);
		if (pVariant->vtEx == VT_EX_STRUCT_SIZE || pVariant->vtEx == VT_EX_CONTROL_ID || pVariant->vtEx == VT_EX_CONTROL_TYPE)
		{
			continue;
		}

		if (pVariant->bNeedToConvert)	// If the property's bNeedToConvert field is true, don't save to *.xui file(but need to read).
		{
			continue;
		}

		if (!pVariant->bSaveToSkinFile)
		{
			continue;
		}

		const IUI_VARIANT *pOrigVariant = pNearCtrlProp->GetProperty(i);

		// Save control's ID and diff value.
		if (pVariant->vtEx != VT_EX_CONTROL_ID_MACRO)
		{
			if (*pOrigVariant == *pVariant)
			{
				continue;
			}
		}

		int nCount = SaveElementAttributeVariant(xmlDoc, pControl, *pVariant, FALSE);
		if (nCount > 0)
		{
			// If only save control ID. DO NOT save it to xui file.
			if (pVariant->vtEx != VT_EX_CONTROL_ID_MACRO)
			{
				nSavedCount++;
			}
		}
	}

	// Only properties be saved, to add node.
	if (nSavedCount > 0)
	{
		pStoryFrameNode->appendChild(pControl);
	}

	return nSavedCount;
}

int SaveWindowStoryboard(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pUIWnd, const UIWNDPROPERTIES *pUIWndProp)
{
	CONTROL_TYPE ect = (CONTROL_TYPE)pUIWndProp->m_eControlType;
	if (ect != CT_UIWND)
	{
		return -1;
	}

	int nStoryFrameCount = (int)pUIWndProp->m_vStoryFrame.size();
	if (nStoryFrameCount <= 0)
	{
		return 1;
	}

	XML::IXMLDOMElementPtr pStoryboard = xmlDoc->createElement(_T("Storyboard"));
	pUIWnd->appendChild(pStoryboard);

	// Save story item
	for (LONG i = 0; i < nStoryFrameCount ; i++)
	{
		const STORY_FRAME *pStoryFrame = pUIWndProp->m_vStoryFrame[i];

		XML::IXMLDOMElementPtr pStoryFrameNode = xmlDoc->createElement(_T("Frame"));

		CString strTime = StringFromInt(pStoryFrame->nFrameIndex);
		pStoryFrameNode->setAttribute(_T("Index"), strTime.GetBuffer(0));

		// Save the different property
		int nSavedCount = 0;
		int nStoryFrameCtrlCount = (int)pStoryFrame->m_vStoryFrameCtrlItem.size();
		for (int j = 0; j < nStoryFrameCtrlCount; ++j)
		{
			const CTRLPROPERTIES *pCtrlProp = pStoryFrame->m_vStoryFrameCtrlItem[j];

			const CTRLPROPERTIES *pNearCtrlProp = pUIWndProp->FindLeftNearControlInStoryBoard(
					pCtrlProp->m_strID, pStoryFrame->nFrameIndex, FALSE);

			int nCount = SaveCtrlStoryProp(xmlDoc, pStoryFrameNode, pNearCtrlProp, pCtrlProp);
			if (nCount > 0)
			{
				nSavedCount++;
			}
		}

		if (nSavedCount > 0)
		{
			pStoryboard->appendChild(pStoryFrameNode);
		}
	}

	return 0;
}

XML::IXMLDOMElementPtr SaveWindow(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pWindows, UIWNDPROPERTIES *pUIProp)
{
	HRESULT hr = S_OK;

	XML::IXMLDOMElementPtr pWindow = xmlDoc->createElement("Window");

	SaveControlProperties(xmlDoc, pWindow, pUIProp, NULL);

	hr = pWindows->appendChild(pWindow);

	// Save children
	XML::IXMLDOMElementPtr pControls = xmlDoc->createElement(_T("Controls"));
	pWindow->appendChild(pControls);

	size_t nControlCount = pUIProp->m_vControls.size();
	for (size_t i = 0; i < nControlCount; ++i)
	{
		CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[i];

		// create control section
		XML::IXMLDOMElementPtr pControl = xmlDoc->createElement((LPCTSTR)GetControlTypeString(pCtrlProp->m_eControlType));
		pControls->appendChild(pControl);

		SaveControlProperties(xmlDoc, pControl, pCtrlProp, pUIProp);
	}

	// Save storyboard
	SaveWindowStoryboard(xmlDoc, pWindow, pUIProp);

	return pWindow;
}

int SaveAllWindows(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pParentElement, std::vector<UIWNDPROPERTIES *> *pvWindows)
{
	HRESULT hr = S_OK;

	XML::IXMLDOMElementPtr pWindows = xmlDoc->createElement("Windows");
	hr = pParentElement->appendChild(pWindows);

	size_t nCount = pvWindows->size();
	for (size_t i = 0; i < nCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = (*pvWindows)[i];

		XML::IXMLDOMElementPtr pWindow = SaveWindow(xmlDoc, pWindows, pUIProp);
	}

	return 0;
}

int SaveFontResource(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pResourceParent, FONTRESOURCEITEM *presDefaultFont)
{
	// create resource section
	if (presDefaultFont->m_bConditionFont)
	{
		CString strControlType = GetControlTypeString(CT_RESOURCE_CONDITION_FONT);
		XML::IXMLDOMElementPtr pResourceElement = xmlDoc->createElement((LPCTSTR)strControlType);
		pResourceParent->appendChild(pResourceElement);

		if (presDefaultFont->m_bConditionFont)
		{
			pResourceElement->setAttribute("ID", (LPCTSTR)presDefaultFont->m_strFontID);

			std::map<CString, RESFONTPROPERTIES *>::iterator it = presDefaultFont->m_mapConditionFont.begin();
			for (;  it != presDefaultFont->m_mapConditionFont.end() ; ++it)
			{
				strControlType = GetControlTypeString(it->second->m_eControlType);	// Create "FONT"
				XML::IXMLDOMElementPtr pFONT = xmlDoc->createElement((LPCTSTR)strControlType);
				pResourceElement->appendChild(pFONT);

				SaveControlProperties(xmlDoc, pFONT, it->second, NULL);
			}
		}
	}
	else
	{
		CString strControlType = GetControlTypeString(CT_RESOURCE_GENERAL_FONT);
		XML::IXMLDOMElementPtr pResourceElement = xmlDoc->createElement((LPCTSTR)strControlType);
		pResourceParent->appendChild(pResourceElement);

		SaveControlProperties(xmlDoc, pResourceElement, presDefaultFont->m_pFontResProp, NULL);
	}

	return 0;
}

// VC6.0 not support c++ template:
// template <class T>
// int SaveResource(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pResourceParent, std::vector<T *> *pvpResource)

int SaveFontResources(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pResourceParent, std::vector<FONTRESOURCEITEM *> *pvpFontList)
{
	// Save owner-define font
	size_t nCount = pvpFontList->size();
	for (size_t i = 0; i < nCount; ++i)
	{
		FONTRESOURCEITEM *presFont = (*pvpFontList)[i];

		SaveFontResource(xmlDoc, pResourceParent, presFont);
	}

	return 0;
}

int SaveDefaultColorResource(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pResourceParent, const RESCOLORPROPERTIES *pResDefaultColor)
{
	// create resource section
	XML::IXMLDOMElementPtr pResourceElement = xmlDoc->createElement((LPCTSTR)GetControlTypeString(pResDefaultColor->m_eControlType));
	pResourceParent->appendChild(pResourceElement);

	SaveControlProperties(xmlDoc, pResourceElement, pResDefaultColor, NULL);

	return 0;
}

int SaveColorResource(XML::IXMLDOMDocumentPtr xmlDoc, XML::IXMLDOMElementPtr pResourceParent, std::vector<RESCOLORPROPERTIES *> *pvpResource)
{
	size_t nCount = pvpResource->size();
	for (size_t i = 0; i < nCount; ++i)
	{
		RESCOLORPROPERTIES *pResourceProp = (*pvpResource)[i];

		// create resource section
		XML::IXMLDOMElementPtr pResourceElement = xmlDoc->createElement((LPCTSTR)GetControlTypeString(pResourceProp->m_eControlType));
		pResourceParent->appendChild(pResourceElement);

		SaveControlProperties(xmlDoc, pResourceElement, pResourceProp, NULL);
	}

	return 0;
}

int SaveAllResources(
	XML::IXMLDOMDocumentPtr xmlDoc,
	XML::IXMLDOMElementPtr pParentElement,
	FONTRESOURCEITEM *presDefaultFont,
	std::vector<FONTRESOURCEITEM *> *pvpFontList,
	const RESCOLORPROPERTIES *pResDefaultColor,
	std::vector<RESCOLORPROPERTIES *> *pvpColorRes)
{
	HRESULT hr = S_OK;

	XML::IXMLDOMElementPtr pResources = xmlDoc->createElement("Resources");
	hr = pParentElement->appendChild(pResources);

	// Default font resource
	XML::IXMLDOMElementPtr pDefaultFont = xmlDoc->createElement("DefaultFont");
	pResources->appendChild(pDefaultFont);

	SaveFontResource(xmlDoc, pDefaultFont, presDefaultFont);

	// Font resource
	XML::IXMLDOMElementPtr pFonts = xmlDoc->createElement("Fonts");
	pResources->appendChild(pFonts);

	SaveFontResources(xmlDoc, pFonts, pvpFontList);

	// Default Color resource
	XML::IXMLDOMElementPtr pDefaultColor = xmlDoc->createElement("DefaultColor");
	pResources->appendChild(pDefaultColor);

	SaveDefaultColorResource(xmlDoc, pDefaultColor, pResDefaultColor);

	// Color resource
	XML::IXMLDOMElementPtr pColors = xmlDoc->createElement("Colors");
	pResources->appendChild(pColors);

	SaveColorResource(xmlDoc, pColors, pvpColorRes);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// for load

int LoadFont(XML::IXMLDOMElementPtr pFontNode, LOGFONT *plf)
{
	if (pFontNode == NULL)
	{
		return -1;
	}

	if (plf == NULL)
	{
		return -2;
	}

	_variant_t var = pFontNode->getAttribute(_T("Height"));
	if (var.vt != VT_NULL)
	{
		plf->lfHeight = (LONG)var;
	}

	var = pFontNode->getAttribute(_T("Width"));
	if (var.vt != VT_NULL)
	{
		plf->lfWidth = (LONG)var;
	}

	var = pFontNode->getAttribute(_T("lfEscapement"));
	if (var.vt != VT_NULL)
	{
		plf->lfEscapement = (LONG)var;
	}

	var = pFontNode->getAttribute(_T("Orientation"));
	if (var.vt != VT_NULL)
	{
		plf->lfOrientation = (LONG)var;
	}

	var = pFontNode->getAttribute(_T("Weight"));
	if (var.vt != VT_NULL)
	{
		plf->lfWeight = (LONG)var;
	}

	var = pFontNode->getAttribute(_T("Italic"));
	if (var.vt != VT_NULL)
	{
		plf->lfItalic = (BYTE)var;
	}

	var = pFontNode->getAttribute(_T("Underline"));
	if (var.vt != VT_NULL)
	{
		plf->lfUnderline = (BYTE)var;
	}

	var = pFontNode->getAttribute(_T("StrikeOut"));
	if (var.vt != VT_NULL)
	{
		plf->lfStrikeOut = (BYTE)var;
	}

	var = pFontNode->getAttribute(_T("CharSet"));
	if (var.vt != VT_NULL)
	{
		plf->lfCharSet = (BYTE)var;
	}

	var = pFontNode->getAttribute(_T("OutPrecision"));
	if (var.vt != VT_NULL)
	{
		plf->lfOutPrecision = (BYTE)var;
	}

	var = pFontNode->getAttribute(_T("ClipPrecision"));
	if (var.vt != VT_NULL)
	{
		plf->lfClipPrecision = (BYTE)var;
	}

	var = pFontNode->getAttribute(_T("Quality"));
	if (var.vt != VT_NULL)
	{
		plf->lfQuality = (BYTE)var;
	}

	var = pFontNode->getAttribute(_T("PitchAndFamily"));
	if (var.vt != VT_NULL)
	{
		plf->lfPitchAndFamily = (BYTE)var;
	}

	_variant_t varFacename = pFontNode->getAttribute(_T("FaceName"));
	if (varFacename.vt != VT_NULL)
	{
		TSTRCPY(plf->lfFaceName, 32, _bstr_t(varFacename));
	}

	return 0;
}

int LoadElementAttributeVariant(XML::IXMLDOMElementPtr pParentElement,
	IUI_VARIANT *pVariant,
	CTRLPROPERTIES *pCtrlProperties)
{
	ASSERT(pVariant != NULL);
	ASSERT(pVariant->strXUIElementName.length() > 0);

	CString str = (LPCTSTR)pVariant->strXUIElementName;
	XML::IXMLDOMElementPtr pElement = pParentElement->selectSingleNode((LPCTSTR)str);
	if (pElement == NULL)
	{
		return 1;
	}

	if (pVariant->vtEx == VT_EMPTY
		|| pVariant->vtEx == VT_EX_CONTROL_ID_MACRO
		|| pVariant->vtEx == VT_EX_STYLE_EDIT
		|| pVariant->vtEx == VT_EX_RESOURCE_FONT
		|| pVariant->vtEx == VT_EX_RESOURCE_COLOR
		|| pVariant->vtEx == VT_EX_STYLE_MENU
		|| pVariant->vtEx == VT_EX_STYLE_HEADERCTRL
		|| pVariant->vtEx == VT_EX_STYLE_SPIN
		|| pVariant->vtEx == VT_EX_STYLE_HORSCROLLBAR
		|| pVariant->vtEx == VT_EX_STYLE_VERSCROLLBAR
		|| pVariant->vtEx == VT_EX_WINDOW_ID
		|| pVariant->vtEx == VT_EX_WINDOW_ID_NOT_INCLUDE_SELF
		|| pVariant->vtEx == VT_EX_TASKMGR_ID)
	{
		switch (pVariant->vt)
		{
		case VT_BOOL:
		{
			_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
			if (var.vt != VT_NULL)
			{
				var.wReserved1 = 0;
				var.wReserved2 = 0;
				var.wReserved3 = 0;

				CString str = StringFromVariant(var);
				if (str.CompareNoCase(_T("false")) == 0 || str.CompareNoCase(_T("0")) == 0)
				{
					*pVariant = false;
				}
				else
				{
					*pVariant = true;
				}

				pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.
			}
		}
		break;
		case VT_I4:
		case VT_INT:
		{
			_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
			if (var.vt != VT_NULL)
			{
				var.wReserved1 = 0;
				var.wReserved2 = 0;
				var.wReserved3 = 0;

				*pVariant = LONG(var);
				pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.
			}
		}
		break;
		case VT_BSTR:
		default:
		{
			_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
			if (var.vt != VT_NULL)
			{
				var.wReserved1 = 0;
				var.wReserved2 = 0;
				var.wReserved3 = 0;

				*pVariant = var;
				pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.
			}

			if (pVariant->vtEx == VT_EX_CONTROL_ID_MACRO)	// Get ID
			{
				pCtrlProperties->m_nID = (LONG)CUIMgr::StringIDToInt(_bstr_t(var));
			}
		}
		break;
		}

		return 0;
	}
	else if (pVariant->vtEx == VT_EX_RECT)
	{
		ASSERT(pVariant->vt == (VT_ARRAY | VT_I4));

		CRect rect(0, 0, 0, 0);
		_variant_t varLeft = pElement->getAttribute(_T("Left"));
		if (varLeft.vt != VT_NULL)
		{
			rect.left = varLeft;
		}
		_variant_t varTop = pElement->getAttribute(_T("Top"));
		if (varTop.vt != VT_NULL)
		{
			rect.top = varTop;
		}
		_variant_t varRight = pElement->getAttribute(_T("Right"));
		if (varRight.vt != VT_NULL)
		{
			rect.right = varRight;
		}
		_variant_t varBottom = pElement->getAttribute(_T("Bottom"));
		if (varBottom.vt != VT_NULL)
		{
			rect.bottom = varBottom;
		}

		*pVariant = rect;
		pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.

		return 0;
	}
	else if (pVariant->vtEx == VT_EX_RECT_WH)
	{
		ASSERT(pVariant->vt == (VT_ARRAY | VT_I4));

		_variant_t varSize = pElement->getAttribute(_T("Size"));
		CString strFirst, strSecond;
		RECT rect;
		rect.left = rect.top = rect.right = rect.bottom = 0;
		_variant_t varLocation = pElement->getAttribute(_T("Location"));
		if (varLocation.vt != VT_NULL)
		{
			CString strLocation = StringFromVariant(varLocation);
			AfxExtractSubString(strFirst, strLocation, 0, ',');
			AfxExtractSubString(strSecond, strLocation, 1, ',');

			rect.left = IntFromString(strFirst);
			rect.top = IntFromString(strSecond);
		}
		if (varSize.vt != VT_NULL)
		{
			CString strSize = StringFromVariant(varSize);
			AfxExtractSubString(strFirst, strSize, 0, ',');
			AfxExtractSubString(strSecond, strSize, 1, ',');
			int nWidth = IntFromString(strFirst);
			int nHeight = IntFromString(strSecond);

			rect.right = rect.left + nWidth;
			rect.bottom = rect.top + nHeight;
		}
		*pVariant = rect;
		pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.

		return 0;
	}
	else if (pVariant->vtEx == VT_EX_POINT)
	{
		_variant_t varX = pElement->getAttribute(_T("X"));
		_variant_t varY = pElement->getAttribute(_T("Y"));
		if (varX.vt != VT_NULL)
		{
			pVariant->SetPointX(varX);
			pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.
		}
		if (varY.vt != VT_NULL)
		{
			pVariant->SetPointY(varY);
			pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.
		}

		return 0;
	}
	else if (pVariant->vtEx == VT_EX_COLOR)
	{
		_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
		if (var.vt == VT_NULL)
		{
			return 2;
		}

		CString strValue = StringFromVariant(var);
		if (strValue.GetLength() > 0 && strValue[0] == '#')
		{
			strValue.Delete(0, 1);
			strValue.Insert(0, _T("0x"));
		}

		*pVariant = LongFromString(strValue);
		pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.

		return 0;
	}
	else if (pVariant->vtEx == VT_EX_FONT)
	{
		LOGFONT lf;
		LoadFont(pElement, &lf);
		*pVariant = lf;
		pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.

		return 0;
	}
	else if (pVariant->vtEx == VT_EX_BITMAPPATH)
	{
		_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
		if (var.vt != VT_NULL)
		{
			var.wReserved1 = 0;
			var.wReserved2 = 0;
			var.wReserved3 = 0;

			*pVariant = var;
			pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.
		}

		return 0;
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
		_variant_t strValue = pElement->getAttribute(pVariant->strXUIAttributeName);
		if (strValue.vt != VT_NULL)
		{
			strValue.wReserved1 = 0;
			strValue.wReserved2 = 0;
			strValue.wReserved3 = 0;

			IUI_VARIANT var = strValue;
			pVariant->InitByEnumString((LPCTSTR)var);
			pVariant->wReserved1 = 1;	// means the value is loaded from *.xui file.
		}

		return 0;
	}
	else
	{
		ASSERT(FALSE);
	}

	return 0;
}

int LoadOldElementAttributeVariant(XML::IXMLDOMElementPtr pParentElement, IUI_VARIANT *pVariant, OLDCTRLPROPERTIES *pCtrlProperties)
{
	ASSERT(pVariant != NULL);
	ASSERT(pVariant->strXUIElementName.length() > 0);

	XML::IXMLDOMElementPtr pElement = pParentElement->selectSingleNode(pVariant->strXUIElementName);
	if (pElement == NULL)
	{
		return 1;
	}

	int nRet = 0;

	if (pVariant->vtEx == VT_EMPTY || pVariant->vtEx == VT_EX_CONTROL_ID_MACRO
		|| pVariant->vtEx == VT_EX_RESOURCE_FONT || pVariant->vtEx == VT_EX_RESOURCE_COLOR || pVariant->vtEx == VT_EX_STYLE_MENU
		|| pVariant->vtEx == VT_EX_STYLE_HEADERCTRL || pVariant->vtEx == VT_EX_STYLE_SPIN
		|| pVariant->vtEx == VT_EX_STYLE_HORSCROLLBAR || pVariant->vtEx == VT_EX_STYLE_VERSCROLLBAR)
	{
		switch (pVariant->vt)
		{
		case VT_BOOL:
		{
			_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
			if (var.vt != VT_NULL)
			{
				var.wReserved1 = 0;
				var.wReserved2 = 0;
				var.wReserved3 = 0;

				CString str = StringFromVariant(var);
				if (str.CompareNoCase(_T("false")) == 0 || str.CompareNoCase(_T("0")) == 0)
				{
					*pVariant = false;
				}
				else
				{
					*pVariant = true;
				}
			}
			else
			{
				nRet = -1;
			}
		}
		break;
		case VT_I4:
		case VT_INT:
		{
			_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
			if (var.vt != VT_NULL)
			{
				var.wReserved1 = 0;
				var.wReserved2 = 0;
				var.wReserved3 = 0;

				*pVariant = LONG(var);
			}
			else
			{
				nRet = -1;
			}
		}
		break;
		case VT_BSTR:
		default:
		{
			_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
			if (var.vt != VT_NULL)
			{
				var.wReserved1 = 0;
				var.wReserved2 = 0;
				var.wReserved3 = 0;

				*pVariant = var;
			}
			else
			{
				nRet = -1;
			}
		}
		break;
		}

		return nRet;
	}
	else if (pVariant->vtEx == VT_EX_RECT)
	{
		ASSERT(pVariant->vt == (VT_ARRAY | VT_I4));

		CRect rect(0, 0, 0, 0);
		int nRet = 0;

		_variant_t varLeft = pElement->getAttribute(_T("Left"));
		if (varLeft.vt != VT_NULL)
		{
			rect.left = varLeft;
		}
		else
		{
			nRet = -1;
		}

		_variant_t varTop = pElement->getAttribute(_T("Top"));
		if (varTop.vt != VT_NULL)
		{
			rect.top = varTop;
		}
		else
		{
			nRet = -1;
		}

		_variant_t varRight = pElement->getAttribute(_T("Right"));
		if (varRight.vt != VT_NULL)
		{
			rect.right = varRight;
		}
		else
		{
			nRet = -1;
		}

		_variant_t varBottom = pElement->getAttribute(_T("Bottom"));
		if (varBottom.vt != VT_NULL)
		{
			rect.bottom = varBottom;
		}
		else
		{
			nRet = -1;
		}

		*pVariant = rect;

		return nRet;
	}
	else if (pVariant->vtEx == VT_EX_RECT_WH)
	{
		ASSERT(pVariant->vt == (VT_ARRAY | VT_I4));

		int nRet = 0;

		_variant_t varSize = pElement->getAttribute(_T("Size"));
		CString strFirst, strSecond;
		RECT rect;
		_variant_t varLocation = pElement->getAttribute(_T("Location"));
		if (varLocation.vt != VT_NULL)
		{
			CString strLocation = StringFromVariant(varLocation);
			AfxExtractSubString(strFirst, strLocation, 0, ',');
			AfxExtractSubString(strSecond, strLocation, 1, ',');

			rect.left = IntFromString(strFirst);
			rect.top = IntFromString(strSecond);
		}
		else
		{
			nRet = -1;
		}

		if (varSize.vt != VT_NULL)
		{
			CString strSize = StringFromVariant(varSize);
			AfxExtractSubString(strFirst, strSize, 0, ',');
			AfxExtractSubString(strSecond, strSize, 1, ',');
			int nWidth = IntFromString(strFirst);
			int nHeight = IntFromString(strSecond);

			rect.right = rect.left + nWidth;
			rect.bottom = rect.top + nHeight;
		}
		else
		{
			nRet = -1;
		}

		*pVariant = rect;

		return nRet;
	}
	else if (pVariant->vtEx == VT_EX_POINT)
	{
		int nRet = 0;

		_variant_t varX = pElement->getAttribute(_T("X"));
		_variant_t varY = pElement->getAttribute(_T("Y"));

		if (varX.vt != VT_NULL)
		{
			pVariant->SetPointX(varX);
		}
		else
		{
			nRet = -1;
		}

		if (varY.vt != VT_NULL)
		{
			pVariant->SetPointY(varY);
		}
		else
		{
			nRet = -1;
		}

		return nRet;
	}
	else if (pVariant->vtEx == VT_EX_COLOR)
	{
		_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
		if (var.vt == VT_NULL)
		{
			return 2;
		}

		CString strValue = StringFromVariant(var);
		if (strValue.GetLength() > 0 && strValue[0] == '#')
		{
			strValue.Delete(0, 1);
			strValue.Insert(0, _T("0x"));
		}

		*pVariant = LongFromString(strValue);

		return 0;
	}
	else if (pVariant->vtEx == VT_EX_BITMAPPATH)
	{
		_variant_t var = pElement->getAttribute(pVariant->strXUIAttributeName);
		if (var.vt != VT_NULL)
		{
			var.wReserved1 = 0;
			var.wReserved2 = 0;
			var.wReserved3 = 0;

			*pVariant = var;
		}
		else
		{
			return -1;
		}

		return 0;
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
		IUI_VARIANT strValue = pElement->getAttribute(pVariant->strXUIAttributeName);
		if (strValue.vt != VT_NULL)
		{
			pVariant->InitByEnumString((LPCTSTR)strValue);
		}
		else
		{
			return -1;
		}

		return 0;
	}
	else
	{
		ASSERT(FALSE);
	}

	return 0;
}

int LoadXUIVersion(XML::IXMLDOMElementPtr pRootElement, LONG *plVersion, LONG *plBuild, LONG *plChangeList)
{
	ASSERT(pRootElement != NULL);

	XML::IXMLDOMElementPtr pVersionElement = pRootElement->selectSingleNode("Version");
	if (pVersionElement == NULL)
	{
		return -1;
	}

	_variant_t str = pVersionElement->getAttribute("Version");
	if (plVersion != NULL && str.vt != VT_NULL)
	{
		*plVersion = LONG(str);
	}

	str = pVersionElement->getAttribute("Build");
	if (plBuild != NULL && str.vt != VT_NULL)
	{
		*plBuild = LONG(str);
	}

	str = pVersionElement->getAttribute("ChangeList");
	if (plChangeList != NULL && str.vt != VT_NULL)
	{
		*plChangeList = LONG(str);
	}

	return 0;
}

int LoadProjectSettings(XML::IXMLDOMElementPtr pRootElement, CString &strOpenProgram)
{
	ASSERT(pRootElement != NULL);

	// 	BOOL bExist;// = IsNodeExist(pRootElement, "Configuration");
	// 	if (!bExist)
	// 		return -1;

	XML::IXMLDOMElementPtr pConfiguration = pRootElement->selectSingleNode("Configuration");
	if (pConfiguration == NULL)
	{
		return -1;
	}
	_variant_t str = pConfiguration->getAttribute("OpenProgram");
	strOpenProgram = StringFromVariant(str);

	return 0;
}

// Since only save different property of control while save story frame.
// If load story frame item, use near previous control's properties as default value.
// Use control's ID to find the near previous control's properties.
int LoadControlProperties(
	XML::IXMLDOMElementPtr pControl,
	__in __out CTRLPROPERTIES *pCtrlProperties,
	BOOL bLoadStoryFrameItem = FALSE,
	const UIWNDPROPERTIES *pParentUIProp = NULL,
	int nCurFrame = -1)
{
	if (pControl == NULL || pCtrlProperties == NULL)
	{
		return -1;
	}

	if (bLoadStoryFrameItem && pParentUIProp == NULL)
	{
		return -2;
	}

	LONG lVarCount = pCtrlProperties->GetPropertyCount();
	CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProperties->m_eControlType;
	for (LONG i = 0; i < lVarCount ; i++)
	{
		IUI_VARIANT *pVariant = pCtrlProperties->GetProperty(i);
		if (pVariant->vtEx == VT_EX_STRUCT_SIZE
			|| pVariant->vtEx == VT_EX_CONTROL_ID
			|| pVariant->vtEx == VT_EX_CONTROL_TYPE
			|| !pVariant->bSaveToSkinFile)
		{
			continue;
		}

		int nRet = LoadElementAttributeVariant(pControl, pVariant, pCtrlProperties);
		if (nRet != 0)// The property not found in *.xui file.
		{
			continue;
		}

		// If load story frame item, find the previous control properties as default.
		// If load first item, use orign control properties as default.
		if (bLoadStoryFrameItem && pVariant->vtEx == VT_EX_CONTROL_ID_MACRO)
		{
			// Find control properties as default from storyboard
			const CTRLPROPERTIES *pCtrlPropDefault =
				pParentUIProp->FindLeftNearControlInStoryBoard(*pVariant, nCurFrame, FALSE);
			ASSERT(pCtrlPropDefault != NULL);

			CONTROL_TYPE eControlType = pCtrlPropDefault->m_eControlType;
			if (eControlType == CT_PUSHBUTTON)
			{
				*((BTNPROPERTIES *)pCtrlProperties) = *((BTNPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_CHECK)
			{
				*((CHKPROPERTIES *)pCtrlProperties) = *((CHKPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_RADIO)
			{
				*((RADPROPERTIES *)pCtrlProperties) = *((RADPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_STATIC)
			{
				*((TXTPROPERTIES *)pCtrlProperties) = *((TXTPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_EDIT)
			{
				*((EDTPROPERTIES *)pCtrlProperties) = *((EDTPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_COMBOBOX)
			{
				*((CMBPROPERTIES *)pCtrlProperties) = *((CMBPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_PROGRESS)
			{
				*((PRGPROPERTIES *)pCtrlProperties) = *((PRGPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_SLIDER)
			{
				*((SLDPROPERTIES *)pCtrlProperties) = *((SLDPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_SPIN)
			{
				*((SPNPROPERTIES *)pCtrlProperties) = *((SPNPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_HOTKEY)
			{
				*((HOTPROPERTIES *)pCtrlProperties) = *((HOTPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_IPADDRESS)
			{
				*((IPAPROPERTIES *)pCtrlProperties) = *((IPAPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_SLIDEREX)
			{
				*((SLDEXPROPERTIES *)pCtrlProperties) = *((SLDEXPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_LISTCTRL)
			{
				*((LSTCTRLPROPERTIES *)pCtrlProperties) = *((LSTCTRLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_TREECTRL)
			{
				*((TREECTRLPROPERTIES *)pCtrlProperties) = *((TREECTRLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_SCROLLBAREX)
			{
				*((SBEXPROPERTIES *)pCtrlProperties) = *((SBEXPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_THUMBNAIL)
			{
				*((THMPROPERTIES *)pCtrlProperties) = *((THMPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_TREELIST)
			{
				*((TREELISTPROPERTIES *)pCtrlProperties) = *((TREELISTPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_HTMLCTRL)
			{
				*((HTMLCTRLPROPERTIES *)pCtrlProperties) = *((HTMLCTRLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_DATETIMECTRL)
			{
				*((DATETIMECTRLPROPERTIES *)pCtrlProperties) = *((DATETIMECTRLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WAVECTRL)
			{
				*((WAVECTRLPROPERTIES *)pCtrlProperties) = *((WAVECTRLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_PICTURE)
			{
				*((PICWLPROPERTIES *)pCtrlProperties) = *((PICWLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_MENUBAR)
			{
				*((MENUBARPROPERTIES *)pCtrlProperties) = *((MENUBARPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_SPLITTER)
			{
				*((SPLITTERPROPERTIES *)pCtrlProperties) = *((SPLITTERPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_RECTCTRL)
			{
				*((RECTWLPROPERTIES *)pCtrlProperties) = *((RECTWLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_SPLITTER)
			{
				*((SPLITTERWLPROPERTIES *)pCtrlProperties) = *((SPLITTERWLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_LINE)
			{
				*((LINEWLPROPERTIES *)pCtrlProperties) = *((LINEWLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_TEXT)
			{
				*((TXTPROPERTIES *)pCtrlProperties) = *((TXTPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_BUTTON)
			{
				*((BTNPROPERTIES *)pCtrlProperties) = *((BTNPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_CHECK)
			{
				*((CHKPROPERTIES *)pCtrlProperties) = *((CHKPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_RADIO)
			{
				*((RADPROPERTIES *)pCtrlProperties) = *((RADPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_SLIDER)
			{
				*((SLDEXPROPERTIES *)pCtrlProperties) = *((SLDEXPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_PICTURE)
			{
				*((PICWLPROPERTIES *)pCtrlProperties) = *((PICWLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_RICHEDIT)
			{
				*((RICHEDITPROPERTIES *)pCtrlProperties) = *((RICHEDITPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_WL_RICHEDIT_IM)
			{
				*((IMREPROPERTIES *)pCtrlProperties) = *((IMREPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_TASK_WND_MGR)
			{
				*((TWMGRPROPERTIES *)pCtrlProperties) = *((TWMGRPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_PNL_DOCK)
			{
				*((DOCKPNLPROPERTIES *)pCtrlProperties) = *((DOCKPNLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_UNIFORM_GRID)
			{
				*((UGRIDPNLPROPERTIES *)pCtrlProperties) = *((UGRIDPNLPROPERTIES *)pCtrlPropDefault);
			}
			else if (eControlType == CT_PNL_STACK)
			{
				*((STACKPNLPROPERTIES *)pCtrlProperties) = *((STACKPNLPROPERTIES *)pCtrlPropDefault);
			}
		}

		// If the property is a font, convert is to resource list.
		if (pVariant->vtEx == VT_EX_FONT && pVariant->bNeedToConvert)
		{
			// Find the font exist or not.
			RESFONTPROPERTIES *pFoundFontProp = CUIMgr::FindFontResource(*pVariant);
			if (pFoundFontProp == NULL)
			{
				// Create a new font resource.
				pFoundFontProp = new RESFONTPROPERTIES();

				// ID
				CString strFontResID;
				strFontResID.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RESOURCE_GENERAL_FONT), ++CUIMgr::m_nNextFontResID);
				pFoundFontProp->m_strID = strFontResID;

				// Font content
				pFoundFontProp->m_logFont = (LOGFONT) * pVariant;
				if (COldUIMgr::IsEnableDPI())
				{
					GetDPILogFont(&pFoundFontProp->m_logFont);
				}

				// Save to the font resource list
				FONTRESOURCEITEM *pFri = new FONTRESOURCEITEM;
				pFri->m_bConditionFont = false;
				pFri->m_pFontResProp = pFoundFontProp;
				CUIMgr::AddFontResourceItem(pFri);
			}

			// The old font property convert to use the new font resource.
			CString strXUIElementName = (LPCTSTR)pVariant->strXUIElementName;
			if (strXUIElementName == _T("FontN"))
			{
				pCtrlProperties->m_strResFontIDN = (LPCTSTR)pFoundFontProp->m_strID;	// Only string field need to be converted.
			}
			else if (strXUIElementName == _T("FontH"))
			{
				pCtrlProperties->m_strResFontIDH = (LPCTSTR)pFoundFontProp->m_strID;
			}
			else if (strXUIElementName == _T("FontS"))
			{
				pCtrlProperties->m_strResFontIDS = (LPCTSTR)pFoundFontProp->m_strID;
			}
			else if (strXUIElementName == _T("FontD"))
			{
				pCtrlProperties->m_strResFontIDD = (LPCTSTR)pFoundFontProp->m_strID;
			}
		}
		// If the property is a color, convert is to resource list.
		else if (pVariant->vtEx == VT_EX_COLOR && pVariant->bNeedToConvert)
		{
			// Find the color exist.
			RESCOLORPROPERTIES *pFoundColorProp = (RESCOLORPROPERTIES *)CUIMgr::FindColorResource(*pVariant);
			if (pFoundColorProp == NULL)
			{
				// Create a new color resource.
				pFoundColorProp = new RESCOLORPROPERTIES();

				// ID
				CString strColorResID;
				strColorResID.Format(_T("%s%d"), CTRLPROPERTIES::GetIDPrefix(CT_RESOURCE_COLOR), ++CUIMgr::m_nNextColorResID);
				pFoundColorProp->m_strID = strColorResID;

				// Color content
				pFoundColorProp->m_crColor = (LONG)(COLORREF) * pVariant;

				// Save to the color resource list
				CUIMgr::AddColorResourceItem(pFoundColorProp);
			}

			// The old color property convert to use the new color resource.
			CString strXUIElementName = (LPCTSTR)pVariant->strXUIElementName;
			CString strXUIAttributeName = (LPCTSTR)pVariant->strXUIAttributeName;
			if (eControlType == CT_UIWND)
			{
				UIWNDPROPERTIES *pUIProp = (UIWNDPROPERTIES *)pCtrlProperties;
				if (strXUIElementName == (LPCTSTR)pUIProp->m_crMask.strXUIElementName)
				{
					pUIProp->m_strMaskColorResID = (LPCTSTR)pFoundColorProp->m_strID;
				}
			}
			else if (eControlType == CT_TREECTRL)
			{
				TREECTRLPROPERTIES *pTreeProp = (TREECTRLPROPERTIES *)pCtrlProperties;

				if (strXUIElementName == (LPCTSTR)pTreeProp->m_crItemLine.strXUIElementName
					&& strXUIAttributeName == (LPCTSTR)pTreeProp->m_crItemLine.strXUIAttributeName)
				{
					pTreeProp->m_strItemLineColorResID = (LPCTSTR)pFoundColorProp->m_strID;
				}
				else if (strXUIElementName == (LPCTSTR)pTreeProp->m_crSelectedItemBkColor.strXUIElementName
					&& strXUIAttributeName == (LPCTSTR)pTreeProp->m_crSelectedItemBkColor.strXUIAttributeName)
				{
					pTreeProp->m_strSelectedItemBkColorResID = (LPCTSTR)pFoundColorProp->m_strID;
				}
				else if (strXUIElementName == (LPCTSTR)pTreeProp->m_crHighlightItemBkColor.strXUIElementName
					&& strXUIAttributeName == (LPCTSTR)pTreeProp->m_crHighlightItemBkColor.strXUIAttributeName)
				{
					pTreeProp->m_strHighlightItemBkColorResID = (LPCTSTR)pFoundColorProp->m_strID;
				}
			}
			else if (eControlType == CT_WAVECTRL)
			{
				WAVECTRLPROPERTIES *pWaveProp = (WAVECTRLPROPERTIES *)pCtrlProperties;

				if (strXUIElementName == (LPCTSTR)pWaveProp->m_crGridLine.strXUIElementName && strXUIAttributeName == (LPCTSTR)pWaveProp->m_crGridLine.strXUIAttributeName)
				{
					pWaveProp->m_strGridLineColorResID = (LPCTSTR)pFoundColorProp->m_strID;
				}
				else if (strXUIElementName == (LPCTSTR)pWaveProp->m_crBackground.strXUIElementName && strXUIAttributeName == (LPCTSTR)pWaveProp->m_crBackground.strXUIAttributeName)
				{
					pWaveProp->m_strBackgroundColorResID = (LPCTSTR)pFoundColorProp->m_strID;
				}
			}

			// All control have the following properties.
			if (strXUIElementName == _T("ColorN"))
			{
				pCtrlProperties->m_strResColorIDN = (LPCTSTR)pFoundColorProp->m_strID;	// Only string field need to be converted.
			}
			else if (strXUIElementName == _T("ColorH"))
			{
				pCtrlProperties->m_strResColorIDH = (LPCTSTR)pFoundColorProp->m_strID;
			}
			else if (strXUIElementName == _T("ColorS"))
			{
				pCtrlProperties->m_strResColorIDS = (LPCTSTR)pFoundColorProp->m_strID;
			}
			else if (strXUIElementName == _T("ColorD"))
			{
				pCtrlProperties->m_strResColorIDD = (LPCTSTR)pFoundColorProp->m_strID;
			}
		}
	}

	// Load the properties in old version of the.xui.
	if (eControlType == CT_PUSHBUTTON || eControlType == CT_RADIO || eControlType == CT_CHECK)
	{
		OLDBTNBASEPROPERTIES oldBtnProp;
		LONG lVarCount = oldBtnProp.GetPropertyCount();
		for (LONG i = 0; i < lVarCount ; i++)
		{
			IUI_VARIANT *pVariant = oldBtnProp.GetVariant(i);
			if (pVariant->vtEx == VT_EX_STRUCT_SIZE || pVariant->vtEx == VT_EX_CONTROL_ID || pVariant->vtEx == VT_EX_CONTROL_TYPE || !pVariant->bSaveToSkinFile)
			{
				continue;
			}

			int nRet = LoadOldElementAttributeVariant(pControl, pVariant, &oldBtnProp);
			if (nRet != 0)// The property not found in *.xui file.
			{
				continue;
			}
		}

		LONG lXUIVersion = CUIMgr::GetXUIBuildVersion();
		if (lXUIVersion < 4921)
		{
			BTNBASEPROPERTIES *pBtnProp = (BTNBASEPROPERTIES *)pCtrlProperties;
			CRect rcTextMargin;
			rcTextMargin.SetRect(0, 0, 0, 0);

			TEXT_ALIGN_HOR eTextAlignHor = pBtnProp->m_eTextHorAlignMode;
			if (eTextAlignHor == TAH_DEFAULT || eTextAlignHor == TAH_LEFT || eTextAlignHor == TAH_CENTER)
			{
				rcTextMargin.left = (LONG)oldBtnProp.m_lHorAlign;
			}
			else if (eTextAlignHor == TAH_RIGHT)
			{
				rcTextMargin.right = (LONG)oldBtnProp.m_lHorAlign;
			}

			TEXT_ALIGN_VER eTextAlignVer = pBtnProp->m_eTextVerAlignMode;
			if (eTextAlignVer == TAV_DEFAULT || eTextAlignVer == TAV_TOP || eTextAlignVer == TAV_CENTER)
			{
				rcTextMargin.top = (LONG)oldBtnProp.m_lVerAlign;
			}
			else if (eTextAlignVer == TAV_BOTTOM)
			{
				rcTextMargin.bottom = (LONG)oldBtnProp.m_lVerAlign;
			}

			pBtnProp->m_rcPadding4Text = rcTextMargin;
		}
	}

	return 0;
}

int LoadResourceStyleProperties(XML::IXMLDOMElementPtr pResourceStyle, CTRLPROPERTIES *pResStyleProp)
{
	if (pResourceStyle == NULL || pResStyleProp == NULL)
	{
		return -1;
	}

	LONG lVarCount = pResStyleProp->GetPropertyCount();
	for (LONG i = 0; i < lVarCount ; i++)
	{
		IUI_VARIANT *pVariant = pResStyleProp->GetProperty(i);
		if (pVariant->vtEx == VT_EX_STRUCT_SIZE || pVariant->vtEx == VT_EX_CONTROL_ID || pVariant->vtEx == VT_EX_CONTROL_TYPE || !pVariant->bSaveToSkinFile)
		{
			continue;
		}

		int nRet = LoadElementAttributeVariant(pResourceStyle, pVariant, pResStyleProp);
	}

	return 0;
}

int LoadControl(XML::IXMLDOMElementPtr pControl, UIWNDPROPERTIES *pUIProp)
{
	if (pControl == NULL || pUIProp == NULL)
	{
		return -1;
	}

	BSTR bstr;
	pControl->get_nodeName(&bstr);
	CString strType = (LPCTSTR)_bstr_t(bstr);
	CONTROL_TYPE eControlType = GetControlTypeEnum((LPCTSTR)_bstr_t(bstr));
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}

	CTRLPROPERTIES *pCtrlProperties = NULL;

	switch (eControlType)
	{
	case CT_PUSHBUTTON :
	{
		pCtrlProperties = new BTNPROPERTIES;
	}
	break;
	// Check box
	case CT_CHECK:
	{
		pCtrlProperties = new CHKPROPERTIES;
	}
	break;
	// Radio button
	case CT_RADIO:
	{
		pCtrlProperties = new RADPROPERTIES;
	}
	break;
	// Static
	case CT_STATIC:
	{
		pCtrlProperties = new TXTPROPERTIES;
	}
	break;
	// Edit
	case CT_EDIT:
	{
		pCtrlProperties = new EDTPROPERTIES;
	}
	break;
	// Combo box
	case CT_COMBOBOX:
	{
		pCtrlProperties = new CMBPROPERTIES;
	}
	break;

	// Spin button
	case CT_SPIN:
	{
		pCtrlProperties = new SPNPROPERTIES;
	}
	break;

	// Progress
	case CT_PROGRESS:
	{
		pCtrlProperties = new PRGPROPERTIES;
	}
	break;

	// Slider
	case CT_SLIDER:
	{
		pCtrlProperties = new SLDPROPERTIES;
	}
	break;

	// HotKey
	case CT_HOTKEY:
	{
		pCtrlProperties = new HOTPROPERTIES;
	}
	break;

	// IPAddress
	case CT_IPADDRESS:
	{
		pCtrlProperties = new IPAPROPERTIES;
	}
	break;

	// Extended Slider
	case CT_SLIDEREX:
	{
		pCtrlProperties = new SLDEXPROPERTIES;
	}
	break;

	// ListCtrl
	case CT_LISTCTRL:
	{
		pCtrlProperties = new LSTCTRLPROPERTIES;
	}
	break;

	// TreeCtrl
	case CT_TREECTRL:
	{
		pCtrlProperties = new TREECTRLPROPERTIES;
	}
	break;

	// RichEdit
	case CT_RICHEDIT:
	{
		pCtrlProperties = new RICHEDITPROPERTIES;
	}
	break;

	// IM RichEdit
	case CT_RICHEDIT_IM:
	{
		pCtrlProperties = new IMREPROPERTIES;
	}
	break;

	// ScrollBar
	case CT_SCROLLBAR:
	{
		pCtrlProperties = new SBPROPERTIES;
	}
	break;

	// ScrollBarEx
	case CT_SCROLLBAREX:
	{
		pCtrlProperties = new SBEXPROPERTIES;
	}
	break;

	// Thumbnail
	case CT_THUMBNAIL:
	{
		pCtrlProperties = new THMPROPERTIES;
	}
	break;

	// TreeList
	case CT_TREELIST:
	{
		pCtrlProperties = new TREELISTPROPERTIES;
	}
	break;

	// HtmlCtrl
	case CT_HTMLCTRL:
	{
		pCtrlProperties = new HTMLCTRLPROPERTIES;
	}
	break;

	// DateTimeCtrl
	case CT_DATETIMECTRL:
	{
		pCtrlProperties = new DATETIMECTRLPROPERTIES;
	}
	break;

	// WaveCtrl
	case CT_WAVECTRL:
	{
		pCtrlProperties = new WAVECTRLPROPERTIES;
	}
	break;

	// PictureCtrl
	case CT_PICTURE:
	{
		pCtrlProperties = new PICWLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_PICTURE;
	}
	break;

	// MenuBar
	case CT_MENUBAR:
	{
		pCtrlProperties = new MENUBARPROPERTIES;
	}
	break;

	case CT_SPLITTER:
	{
		pCtrlProperties = new SPLITTERPROPERTIES;
	}
	break;

	// RectCtrl windowless
	case CT_WL_RECTCTRL:
	{
		pCtrlProperties = new RECTWLPROPERTIES;
	}
	break;

	// Splitter windowless
	case CT_WL_SPLITTER:
	{
		pCtrlProperties = new SPLITTERWLPROPERTIES;
	}
	break;

	// Line windowless
	case CT_WL_LINE:
	{
		pCtrlProperties = new LINEWLPROPERTIES;
	}
	break;

	// Text windowless
	case CT_WL_TEXT:
	{
		pCtrlProperties = new TXTPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_TEXT;
	}
	break;

	// Button windowless
	case CT_WL_BUTTON:
	{
		pCtrlProperties = new BTNPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_BUTTON;
	}
	break;

	case CT_WL_CHECK:
	{
		pCtrlProperties = new CHKPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_CHECK;
	}
	break;

	case CT_WL_RADIO:
	{
		pCtrlProperties = new RADPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_RADIO;
	}
	break;

	// Slider windowless
	case CT_WL_SLIDER:
	{
		pCtrlProperties = new SLDEXPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_SLIDER;
	}
	break;

	// Picture windowless
	case CT_WL_PICTURE:
	{
		pCtrlProperties = new PICWLPROPERTIES;
	}
	break;

	case CT_WL_RICHEDIT:
	{
		pCtrlProperties = new RICHEDITPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_RICHEDIT;
	}
	break;

	case CT_WL_RICHEDIT_IM:
	{
		pCtrlProperties = new IMREPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_RICHEDIT_IM;
	}
	break;

	// TaskWndMgr
	case CT_TASK_WND_MGR:
	{
		pCtrlProperties = new TWMGRPROPERTIES;
		pCtrlProperties->m_eControlType = CT_TASK_WND_MGR;
	}
	break;

	// Panel
	case CT_PNL_DOCK:
	{
		pCtrlProperties = new DOCKPNLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_PNL_DOCK;
	}
	break;

	case CT_UNIFORM_GRID:
	{
		pCtrlProperties = new UGRIDPNLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_UNIFORM_GRID;
	}
	break;

	case CT_PNL_STACK:
	{
		pCtrlProperties = new STACKPNLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_PNL_STACK;
	}
	break;

	default:
		ASSERT(FALSE);
	}

	if (strType.CompareNoCase(_T("RECT")) == 0)	// Compatible the old version
	{
		pCtrlProperties = new RECTWLPROPERTIES;
	}

	LoadControlProperties(pControl, pCtrlProperties);

	pUIProp->m_vControls.push_back(pCtrlProperties);

	return 0;
}

int LoadStoryControl(XML::IXMLDOMElementPtr pControl, STORY_FRAME *pStoryItem, const UIWNDPROPERTIES *pParentUIProp)
{
	if (pControl == NULL || pStoryItem == NULL)
	{
		return -1;
	}

	BSTR bstr;
	pControl->get_nodeName(&bstr);
	CString strType = (LPCTSTR)_bstr_t(bstr);
	CONTROL_TYPE eControlType = GetControlTypeEnum((LPCTSTR)_bstr_t(bstr));
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}

	CTRLPROPERTIES *pCtrlProperties = NULL;
	// Push button
	if (eControlType == CT_PUSHBUTTON)
	{
		pCtrlProperties = new BTNPROPERTIES;
	}
	// Check box
	else if (eControlType == CT_CHECK)
	{
		pCtrlProperties = new CHKPROPERTIES;
	}
	// Radio button
	else if (eControlType == CT_RADIO)
	{
		pCtrlProperties = new RADPROPERTIES;
	}
	// Static
	else if (eControlType == CT_STATIC)
	{
		pCtrlProperties = new TXTPROPERTIES;
	}
	// Edit
	else if (eControlType == CT_EDIT)
	{
		pCtrlProperties = new EDTPROPERTIES;
	}
	// Combo box
	else if (eControlType == CT_COMBOBOX)
	{
		pCtrlProperties = new CMBPROPERTIES;
	}
	// Spin button
	else if (eControlType == CT_SPIN)
	{
		pCtrlProperties = new SPNPROPERTIES;
	}
	// Progress
	else if (eControlType == CT_PROGRESS)
	{
		pCtrlProperties = new PRGPROPERTIES;
	}
	// Slider
	else if (eControlType == CT_SLIDER)
	{
		pCtrlProperties = new SLDPROPERTIES;
	}
	// HotKey
	else if (eControlType == CT_HOTKEY)
	{
		pCtrlProperties = new HOTPROPERTIES;
	}
	// IPAddress
	else if (eControlType == CT_IPADDRESS)
	{
		pCtrlProperties = new IPAPROPERTIES;
	}
	// Extended Slider
	else if (eControlType == CT_SLIDEREX)
	{
		pCtrlProperties = new SLDEXPROPERTIES;
	}
	// ListCtrl
	else if (eControlType == CT_LISTCTRL)
	{
		pCtrlProperties = new LSTCTRLPROPERTIES;
	}
	// TreeCtrl
	else if (eControlType == CT_TREECTRL)
	{
		pCtrlProperties = new TREECTRLPROPERTIES;
	}
	// ScrollBar
	else if (eControlType == CT_SCROLLBAR)
	{
		pCtrlProperties = new SBPROPERTIES;
	}
	// ScrollBarEx
	else if (eControlType == CT_SCROLLBAREX)
	{
		pCtrlProperties = new SBEXPROPERTIES;
	}
	// Thumbnail
	else if (eControlType == CT_THUMBNAIL)
	{
		pCtrlProperties = new THMPROPERTIES;
	}
	// TreeList
	else if (eControlType == CT_TREELIST)
	{
		pCtrlProperties = new TREELISTPROPERTIES;
	}
	// HtmlCtrl
	else if (eControlType == CT_HTMLCTRL)
	{
		pCtrlProperties = new HTMLCTRLPROPERTIES;
	}
	// DateTimeCtrl
	else if (eControlType == CT_DATETIMECTRL)
	{
		pCtrlProperties = new DATETIMECTRLPROPERTIES;
	}
	// WaveCtrl
	else if (eControlType == CT_WAVECTRL)
	{
		pCtrlProperties = new WAVECTRLPROPERTIES;
	}
	// PictureCtrl
	else if (eControlType == CT_PICTURE)
	{
		pCtrlProperties = new PICWLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_PICTURE;
	}
	// MenuBar
	else if (eControlType == CT_MENUBAR)
	{
		pCtrlProperties = new MENUBARPROPERTIES;
	}
	else if (eControlType == CT_SPLITTER)
	{
		pCtrlProperties = new SPLITTERPROPERTIES;
	}
	// RectCtrl windowless
	else if (eControlType == CT_WL_RECTCTRL || strType.CompareNoCase(_T("RECT")) == 0)	// Compatible the old version
	{
		pCtrlProperties = new RECTWLPROPERTIES;
	}
	// Splitter windowless
	else if (eControlType == CT_WL_SPLITTER)
	{
		pCtrlProperties = new SPLITTERWLPROPERTIES;
	}
	// Line windowless
	else if (eControlType == CT_WL_LINE)
	{
		pCtrlProperties = new LINEWLPROPERTIES;
	}
	// Text windowless
	else if (eControlType == CT_WL_TEXT)
	{
		pCtrlProperties = new TXTPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_TEXT;
	}
	// Button windowless
	else if (eControlType == CT_WL_BUTTON)
	{
		pCtrlProperties = new BTNPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_BUTTON;
	}
	else if (eControlType == CT_WL_CHECK)
	{
		pCtrlProperties = new CHKPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_CHECK;
	}
	else if (eControlType == CT_WL_RADIO)
	{
		pCtrlProperties = new RADPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_RADIO;
	}
	// Slider windowless
	else if (eControlType == CT_WL_SLIDER)
	{
		pCtrlProperties = new SLDEXPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_SLIDER;
	}
	// Picture windowless
	else if (eControlType == CT_WL_PICTURE)
	{
		pCtrlProperties = new PICWLPROPERTIES;
	}
	else if (eControlType == CT_WL_RICHEDIT)
	{
		pCtrlProperties = new RICHEDITPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_RICHEDIT;
	}
	else if (eControlType == CT_WL_RICHEDIT_IM)
	{
		pCtrlProperties = new IMREPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_RICHEDIT_IM;
	}
	// TaskWndMgr
	else if (eControlType == CT_TASK_WND_MGR)
	{
		pCtrlProperties = new TWMGRPROPERTIES;
		pCtrlProperties->m_eControlType = CT_TASK_WND_MGR;
	}
	// Panel
	else if (eControlType == CT_PNL_DOCK)
	{
		pCtrlProperties = new DOCKPNLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_PNL_DOCK;
	}
	else if (eControlType == CT_UNIFORM_GRID)
	{
		pCtrlProperties = new UGRIDPNLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_UNIFORM_GRID;
	}
	else if (eControlType == CT_PNL_STACK)
	{
		pCtrlProperties = new STACKPNLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_PNL_STACK;
	}
	else
	{
		ASSERT(FALSE);
	}

	LoadControlProperties(pControl, pCtrlProperties, TRUE, pParentUIProp, pStoryItem->nFrameIndex);

	pStoryItem->m_vStoryFrameCtrlItem.push_back(pCtrlProperties);

	return 0;
}


int LoadControls(XML::IXMLDOMElementPtr pControls, UIWNDPROPERTIES *pUIProp)
{
	if (pControls == NULL || pUIProp == NULL)
	{
		return -1;
	}

#ifdef _DEBUG
	BSTR bstr;
	pControls->get_nodeName(&bstr);
	CString strValue = (LPCTSTR)_bstr_t(bstr);
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}
	ASSERT(strValue == _T("Controls"));
#endif

	XML::IXMLDOMNode *pChildNode = NULL;
	HRESULT hr = pControls->get_firstChild(&pChildNode);
	while (!FAILED(hr) && pChildNode != NULL)
	{
		LoadControl(pChildNode, pUIProp);

		hr = pChildNode->get_nextSibling(&pChildNode);
	}

	return 0;
}

int LoadStoryboard(XML::IXMLDOMElementPtr pStoryboard, UIWNDPROPERTIES *pUIProp)
{
	if (pStoryboard == NULL || pUIProp == NULL)
	{
		return -1;
	}

#ifdef _DEBUG
	BSTR bstr;
	pStoryboard->get_nodeName(&bstr);
	CString strValue = (LPCTSTR)_bstr_t(bstr);
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}
	ASSERT(strValue == _T("Storyboard"));
#endif

	XML::IXMLDOMNode *pFrameNode = NULL;
	HRESULT hr = pStoryboard->get_firstChild(&pFrameNode);
	while (!FAILED(hr) && pFrameNode != NULL)
	{
		XML::IXMLDOMElementPtr pXmlFrame = pFrameNode;

		_variant_t varTime = pXmlFrame->getAttribute(_T("Index"));
		STORY_FRAME *pStoryFrame = new STORY_FRAME;
		pStoryFrame->nFrameIndex = (LONG)varTime;
		pUIProp->InsertStoryFrame(pStoryFrame);

		XML::IXMLDOMNode *pXmlCtrl = NULL;
		HRESULT hr2 = pXmlFrame->get_firstChild(&pXmlCtrl);
		while (!FAILED(hr2) && pXmlCtrl != NULL)
		{
			LoadStoryControl(pXmlCtrl, pStoryFrame, pUIProp);

			hr2 = pXmlCtrl->get_nextSibling(&pXmlCtrl);
		}

		hr = pFrameNode->get_nextSibling(&pFrameNode);
	}

	return 0;
}

int LoadWindow(XML::IXMLDOMElementPtr pWindow, UIWNDPROPERTIES *pUIProp)
{
	if (pWindow == NULL || pUIProp == NULL)
	{
		return -1;
	}

#ifdef _DEBUG
	BSTR bstr;
	pWindow->get_nodeName(&bstr);
	CString strValue = (LPCTSTR)_bstr_t(bstr);
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}
	ASSERT(strValue == _T("Window"));
#endif

	// Load Window
	LoadControlProperties(pWindow, pUIProp);

	// Children
	XML::IXMLDOMElementPtr pControls = pWindow->selectSingleNode(_T("Controls"));
	if (pControls != NULL)
	{
		LoadControls(pControls, pUIProp);
	}

	// Storyboard
	XML::IXMLDOMElementPtr pStoryboard = pWindow->selectSingleNode(_T("Storyboard"));
	if (pStoryboard != NULL)
	{
		LoadStoryboard(pStoryboard, pUIProp);
	}

	return 0;
}

int LoadWindows(XML::IXMLDOMElementPtr pWindows, std::vector<UIWNDPROPERTIES *> *pvpWindows)
{
	if (pWindows == NULL || pvpWindows == NULL)
	{
		return -1;
	}

#ifdef _DEBUG
	BSTR bstr;
	pWindows->get_nodeName(&bstr);
	CString strValue = (LPCTSTR)_bstr_t(bstr);
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}
	ASSERT(strValue == _T("Windows"));
#endif

	XML::IXMLDOMNode *pChildNode = NULL;
	HRESULT hr = pWindows->get_firstChild(&pChildNode);
	while (!FAILED(hr) && pChildNode != NULL)
	{
		UIWNDPROPERTIES *pUIProp = new UIWNDPROPERTIES;
		LoadWindow(pChildNode, pUIProp);
		pvpWindows->push_back(pUIProp);

		hr = pChildNode->get_nextSibling(&pChildNode);
	}

	return 0;
}

int LoadConditionFont(XML::IXMLDOMElementPtr pConditionFont, FONTRESOURCEITEM *presFont)
{
	_variant_t var = pConditionFont->getAttribute("ID");
	if (var.vt == NULL)
	{
		return -1;
	}

	presFont->m_bConditionFont = true;
	presFont->m_strFontID = StringFromVariant(var);

	XML::IXMLDOMNode *pFONT = NULL;
	HRESULT hr = pConditionFont->get_firstChild(&pFONT);
	while (!FAILED(hr) && pFONT != NULL)
	{
		BSTR bstr;
		pFONT->get_nodeName(&bstr);
		CONTROL_TYPE ect = GetControlTypeEnum((LPCTSTR)_bstr_t(bstr));
		if (bstr != NULL)
		{
			SysFreeString(bstr);
		}
		ASSERT(ect == CT_RESOURCE_GENERAL_FONT);
		// The font resource include some conditions
		if (ect == CT_RESOURCE_GENERAL_FONT)
		{
			RESFONTPROPERTIES *pFontProp = new RESFONTPROPERTIES;
			pFontProp->m_strID.strPropWndName = _T("Condition Name");
			pFontProp->m_strID.strDescription = _T("Input the condition name for the font to use.");

			LoadResourceStyleProperties(pFONT, pFontProp);
			presFont->m_mapConditionFont[(LPCTSTR)pFontProp->m_strID] = pFontProp;
		}

		hr = pFONT->get_nextSibling(&pFONT);
	}

	return 0;
}

int LoadFontResource(XML::IXMLDOMNode *pFontNode, FONTRESOURCEITEM *presFont)
{
	if (pFontNode == NULL)
	{
		return -1;
	}
	if (presFont == NULL)
	{
		return -2;
	}

	BSTR bstr;
	pFontNode->get_nodeName(&bstr);
	CONTROL_TYPE ect = GetControlTypeEnum((LPCTSTR)_bstr_t(bstr));
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}

	// The font resource may be not include conditions
	if (ect == CT_RESOURCE_GENERAL_FONT)
	{
		RESFONTPROPERTIES *presFontProp = new RESFONTPROPERTIES;
		LoadResourceStyleProperties(pFontNode, presFontProp);

		presFont->m_bConditionFont = false;
		presFont->m_pFontResProp = presFontProp;
	}
	// The font resource include some conditions
	else if (ect == CT_RESOURCE_CONDITION_FONT)
	{
		LoadConditionFont(pFontNode, presFont);
	}

	return 0;
}

int LoadFontResources(XML::IXMLDOMElementPtr pResources, FONTRESOURCEITEM *presDefaultFont, std::vector<FONTRESOURCEITEM *> *pvpFontRes)
{
	if (pResources == NULL || pvpFontRes == NULL)
	{
		return -1;
	}

#ifdef _DEBUG
	BSTR bstr;
	pResources->get_nodeName(&bstr);
	CString strValue = (LPCTSTR)_bstr_t(bstr);
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}
	ASSERT(strValue == _T("Resources"));
#endif

	//
	// Default Font
	//
	XML::IXMLDOMElementPtr pFont = pResources->selectSingleNode("DefaultFont");
	if (pFont != NULL)
	{
		XML::IXMLDOMNode *pChildNode = NULL;
		HRESULT hr = pFont->get_firstChild(&pChildNode);
		while (!FAILED(hr) && pChildNode != NULL)
		{
			LoadFontResource(pChildNode, presDefaultFont);

			// Assign guid as default font id.
			presDefaultFont->m_strFontID = g_pszDefaultFontID;

			// Default font not show font ID, must be "Default".
			if (!presDefaultFont->m_bConditionFont)
			{
				presDefaultFont->m_pFontResProp->m_strID.bShowInPropertiesWnd = false;
				presDefaultFont->m_pFontResProp->m_strID.bSaveToSkinFile = false;
				presDefaultFont->m_pFontResProp->m_strID = _T("Default");
			}

			// AddRef
			CreateIUIFont(presDefaultFont->m_strFontID);

			hr = pChildNode->get_nextSibling(&pChildNode);
		}
	}

	//
	// Font
	//
	XML::IXMLDOMElementPtr pFonts = pResources->selectSingleNode("Fonts");
	if (pFonts != NULL)
	{
		XML::IXMLDOMNode *pChildNode = NULL;
		HRESULT hr = pFonts->get_firstChild(&pChildNode);
		while (!FAILED(hr) && pChildNode != NULL)
		{
			FONTRESOURCEITEM *pFri = new FONTRESOURCEITEM;
			LoadFontResource(pChildNode, pFri);

			pvpFontRes->push_back(pFri);

			hr = pChildNode->get_nextSibling(&pChildNode);
		}
	}

	// If not load the default font. specify one.
	BOOL bLoadDefaultFont = FALSE;
	if (presDefaultFont->m_bConditionFont)
	{
		if (presDefaultFont->m_mapConditionFont.size() > 0)
		{
			bLoadDefaultFont = TRUE;
		}
	}
	else
	{
		if (presDefaultFont->m_pFontResProp != NULL)
		{
			bLoadDefaultFont = TRUE;
		}
	}

	if (!bLoadDefaultFont)
	{
		RESFONTPROPERTIES *presFontProp = new RESFONTPROPERTIES;
		presFontProp->m_strID.bShowInPropertiesWnd = false;
		presFontProp->m_strID.bSaveToSkinFile = false;
		presDefaultFont->m_pFontResProp = presFontProp;
	}

	return 0;
}

int LoadColorResources(XML::IXMLDOMElementPtr pResources, RESCOLORPROPERTIES *pResDefaultColor, std::vector<RESCOLORPROPERTIES *> *pvpColorRes)
{
	if (pResources == NULL || pvpColorRes == NULL)
	{
		return -1;
	}

#ifdef _DEBUG
	BSTR bstr;
	pResources->get_nodeName(&bstr);
	CString strValue = (LPCTSTR)_bstr_t(bstr);
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}
	ASSERT(strValue == _T("Resources"));
#endif

	//
	// Default Color
	//
	XML::IXMLDOMElementPtr pColor = pResources->selectSingleNode("DefaultColor");
	if (pColor != NULL)
	{
		XML::IXMLDOMNode *pChildNode = NULL;
		HRESULT hr = pColor->get_firstChild(&pChildNode);
		while (!FAILED(hr) && pChildNode != NULL)
		{
#ifdef _DEBUG
			pChildNode->get_nodeName(&bstr);
			CONTROL_TYPE ect = GetControlTypeEnum((LPCTSTR)_bstr_t(bstr));
			if (bstr != NULL)
			{
				SysFreeString(bstr);
			}
			ASSERT(ect == CT_RESOURCE_COLOR);
#endif // _DEBUG

			LoadResourceStyleProperties(pChildNode, pResDefaultColor);

			hr = pChildNode->get_nextSibling(&pChildNode);
		}
	}

	//
	// Color
	//
	XML::IXMLDOMElementPtr pColors = pResources->selectSingleNode("Colors");
	if (pColors != NULL)
	{
		XML::IXMLDOMNode *pChildNode = NULL;
		HRESULT hr = pColors->get_firstChild(&pChildNode);
		while (!FAILED(hr) && pChildNode != NULL)
		{
#ifdef _DEBUG
			pChildNode->get_nodeName(&bstr);
			CONTROL_TYPE ect = GetControlTypeEnum((LPCTSTR)_bstr_t(bstr));
			if (bstr != NULL)
			{
				SysFreeString(bstr);
			}
			ASSERT(ect == CT_RESOURCE_COLOR);
#endif // _DEBUG

			RESCOLORPROPERTIES *pColorRes = new RESCOLORPROPERTIES();
			LoadResourceStyleProperties(pChildNode, pColorRes);
			pvpColorRes->push_back(pColorRes);

			hr = pChildNode->get_nextSibling(&pChildNode);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIMgr::CUIMgr()
{
	//	m_uXUIVersion = sm_nCurXUIVersion;

}

CUIMgr::~CUIMgr()
{
	//	ReleaseProjectMenuChildren(&m_RootMenu);
}


#define LOAD_STYLE(StyleGroupName, vpStyle, StructPropName) \
	{ \
		XML::IXMLDOMElementPtr pStyleGroup = pStyles->selectSingleNode(StyleGroupName); \
		if (pStyleGroup != NULL) \
		{ \
			XML::IXMLDOMNode *pChildNode = NULL; \
			HRESULT hr = pStyleGroup->get_firstChild(&pChildNode); \
			while (!FAILED(hr) && pChildNode != NULL) \
			{ \
				BSTR bstr; \
				pChildNode->get_nodeName(&bstr); \
				CONTROL_TYPE eControlType = GetControlTypeEnum((LPCTSTR)_bstr_t(bstr)); \
				if (bstr != NULL) \
					SysFreeString(bstr); \
				StructPropName *pStyleProp = new StructPropName(); \
				pStyleProp->m_eControlType = eControlType; \
				LoadResourceStyleProperties(pChildNode, pStyleProp); \
				vpStyle.push_back(pStyleProp); \
				hr = pChildNode->get_nextSibling(&pChildNode); \
			} \
		} \
	}


int CUIMgr::LoadSkin()
{
	DWORD dw1 = GetTickCount();
	// Load Resource.h
	LoadIDDefined();

	DWORD dw2 = GetTickCount() - dw1;
	TRACE(_T("[LibUIDK]=== Resource.h spend %d ms\n"), dw2);

	int nRet = 0;

	// if the file is old version, convert to current version first.
	CString strExt = GetFileExt(m_strUIPath, TRUE);
	if (strExt.CompareNoCase(_T(".ui")) == 0)
	{
		nRet = ConvertUI2CurVer();
		return nRet;
	}

#if(_MSC_VER <= 1200) // VC6.0
	CoInitialize(NULL);
#endif
	XML::IXMLDOMDocumentPtr xmlDoc(__uuidof(DOMDocument));
	xmlDoc->async = FALSE;
	HRESULT hr = E_FAIL;
	if (m_bLoadFromFile)
	{
		hr = xmlDoc->load(m_strUIPath.GetBuffer(0));
	}
	else
	{
		LPVOID lpBuffer = NULL;
		DWORD dwSize = 0;
		int nRet = LoadCustomResourceA((LPCSTR)_bstr_t(m_strUIID), (LPCSTR)_bstr_t(m_strUIResourceType), &lpBuffer, &dwSize);
		if (nRet != 0)
		{
			return E_NOXUIRC;
		}

		// Transform UTF-8 to UTF-16
		std::wstring wstrXml = MultiByteToUTF16((char *)lpBuffer + 3, dwSize - 3, CP_UTF8);	// Ignore BOM
		hr = xmlDoc->loadXML(wstrXml.c_str());
	}

	XML::IXMLDOMElementPtr pRootElement = xmlDoc->GetdocumentElement();
	if (pRootElement == NULL)
	{
		return E_NOROOTELEMENT;
	}
#ifdef _DEBUG
	BSTR bstr;
	pRootElement->get_nodeName(&bstr);
	if (bstr != NULL)
	{
		SysFreeString(bstr);
	}
#endif

	// Load project settings
	CString strOpenProgram;
	LoadProjectSettings(pRootElement, strOpenProgram);

	// Load version of *.xui file.
	LONG lVersion = 0;
	nRet = LoadXUIVersion(pRootElement, &lVersion, &m_lXUIBuildVersion, &m_lXUIChangeList);
	if (nRet != 0)
	{
		return nRet;
	}
	m_lXUIMajorVersion = lVersion / 100;
	m_lXUIMinorVersion = lVersion % 100;

	BOOL bPromptConvert = FALSE;
	BOOL bPromptCannotOpen = FALSE;
	if (m_lXUIMajorVersion < m_lLibUIDKMajorVersion)
	{
		bPromptCannotOpen = TRUE;
	}
	else if (m_lXUIMajorVersion == m_lLibUIDKMajorVersion)
	{
		if (m_lXUIMinorVersion < m_lLibUIDKMinorVersion)
		{
			bPromptConvert = TRUE;
		}
		else if (m_lXUIMinorVersion == m_lLibUIDKMinorVersion)
		{
			if (m_lXUIBuildVersion < m_lLibUIDKBuildVersion)
			{
				bPromptConvert = TRUE;
			}
			else if (m_lXUIBuildVersion == m_lLibUIDKBuildVersion)
			{

			}
			else
			{
				bPromptCannotOpen = TRUE;
			}
		}
		else
		{
			bPromptCannotOpen = TRUE;
		}
	}
	else
	{
		bPromptCannotOpen = TRUE;
	}
	if (bPromptConvert)
	{
		ASSERT(!bPromptCannotOpen);
		CString strInfo;
		strInfo.Format(_T("The project\r\n\r\n'%s'\r\n\r\nmust be converted to the current project format. After it has been converted,\r\nyou will not be able to edit this project in previous versions of UIShop.\r\nConvert and open this project?"), m_strUIPath);
		int nRet = AfxMessageBox(strInfo, MB_YESNO | MB_ICONWARNING);
		if (nRet == IDNO)
		{
			return E_CANCELTRANSFERTONEWVERSION;
		}

		// Backup old the.xui
		CopyFile(m_strUIPath, m_strUIPath + _T(".bak"), TRUE);
	}
	if (bPromptCannotOpen)
	{
		ASSERT(!bPromptConvert);
		AfxMessageBox(_T("The selected file is a LibUIDK skin file, but was created by a newer version of this application and cannot be opened."), MB_ICONSTOP);
		return E_NEWERVERSION;
	}

	// Load windows
	XML::IXMLDOMElementPtr pWindows = pRootElement->selectSingleNode("Windows");
	if (pWindows == NULL)
	{
		return E_NOWINDOWSNODE;
	}

	LoadWindows(pWindows, &m_vpWindows);

	// Load Resources, the old version of xui file may be not include 'Resources' node.
	XML::IXMLDOMElementPtr pResources = pRootElement->selectSingleNode("Resources");
	if (pResources == NULL)
	{
		TRACE(_T("[LibUIDK]===Not found 'Resources' node.\n"));

		m_resDefaultFont.m_pFontResProp = new RESFONTPROPERTIES;	// Attach a default font.
	}
	else
	{
		LoadFontResources(pResources, &m_resDefaultFont, &m_vpResFont);
		LoadColorResources(pResources, &m_resDefaultColor, &m_vpColorRes);
	}

	// Load Styles, the old version of xui file may be not include 'Styles' node.
	XML::IXMLDOMElementPtr pStyles = pRootElement->selectSingleNode("Styles");
	if (pStyles == NULL)
	{
		TRACE(_T("[LibUIDK]===Not found 'Styles' node.\n"));
	}
	else
	{
		LOAD_STYLE("PushButtons", m_vpStylePushButton, BTNPROPERTIES)
		LOAD_STYLE("CheckBoxes", m_vpStyleCheckBox, CHKPROPERTIES)
		LOAD_STYLE("RadioButtons", m_vpStyleRadioButton, RADPROPERTIES)
		LOAD_STYLE("ComboBoxes", m_vpStyleComboBox, CMBPROPERTIES)
		LOAD_STYLE("Statics", m_vpStyleStatic, TXTPROPERTIES)
		LOAD_STYLE("Edits", m_vpStyleEdit, EDTPROPERTIES)
		LOAD_STYLE("Sliders", m_vpStyleSlider, SLDPROPERTIES)
		LOAD_STYLE("Progresses", m_vpStyleProgress, PRGPROPERTIES)
		LOAD_STYLE("Spins", m_vpStyleSpin, SPNPROPERTIES)
		LOAD_STYLE("HotKeys", m_vpStyleHotKey, HOTPROPERTIES)
		LOAD_STYLE("IPAdresses", m_vpStyleIPAddress, IPAPROPERTIES)
		LOAD_STYLE("SliderExes", m_vpStyleSliderEx, SLDEXPROPERTIES)
		LOAD_STYLE("ListControls", m_vpStyleListCtrl, LSTCTRLPROPERTIES)
		LOAD_STYLE("TreeControls", m_vpStyleTreeCtrl, TREECTRLPROPERTIES)
		LOAD_STYLE("TreeLists", m_vpStyleTreeList, TREELISTPROPERTIES)
		LOAD_STYLE("RichEdits", m_vpStyleRichEdit, RICHEDITPROPERTIES)
		LOAD_STYLE("IMRichEdits", m_vpStyleIMRichEdit, IMREPROPERTIES)
		LOAD_STYLE("HtmlControls", m_vpStyleHtmlCtrl, HTMLCTRLPROPERTIES)
		LOAD_STYLE("DateTimes", m_vpStyleDateTime, DATETIMECTRLPROPERTIES)
		LOAD_STYLE("WaveControls", m_vpStyleWaveCtrl, WAVECTRLPROPERTIES)
		LOAD_STYLE("Menus", m_vpStyleMenu, MENUPROPERTIES)
		LOAD_STYLE("Menubars", m_vpStyleMenuBar, MENUBARPROPERTIES)
		LOAD_STYLE("Headers", m_vpStyleHeader, HEADERPROPERTIES)
		LOAD_STYLE("HorScrollbars", m_vpStyleHorScrollBar, HORSBPROPERTIES)
		LOAD_STYLE("VerScrollbars", m_vpStyleVerScrollBar, VERSBPROPERTIES)
		LOAD_STYLE("Splitterbars", m_vpStyleSplitter, SPLITTERPROPERTIES)
		LOAD_STYLE("StackPanel", m_vpStylePanelStack, STACKPNLPROPERTIES)
		LOAD_STYLE("WindowlessLine", m_vpStyleWLLine, LINEWLPROPERTIES)
		LOAD_STYLE("WindowlessPicture", m_vpStyleWLPic, PICWLPROPERTIES)
	}

	return nRet;
}

int CUIMgr::LoadIDDefined()
{
	m_vIDs.clear();
	m_mapStrIntIDs.clear();
	m_mapIntStrIDs.clear();
	m_fileResourceH.Detach();

	if (m_bLoadFromFile)
	{
		CString strResourcePath = GetUIResourcePath();
		ASSERT(!strResourcePath.IsEmpty());

		FILE *fp = TFOPEN(strResourcePath.GetBuffer(strResourcePath.GetLength()), _T("rb"));
		if (fp == NULL)
		{
			return -1;
		}

		fseek(fp, 0, SEEK_END);
		int nLen = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		BYTE *pBuf = new BYTE[nLen];
		memset(pBuf, 0, nLen);
		fread(pBuf, nLen, 1, fp);
		fclose(fp);
		m_fileResourceH.Attach(pBuf, nLen, 0);
	}
	else
	{
		m_fileResourceH.OpenResource(m_strUIResourcehID, m_strUIResourceType);
	}

	std::string strA;
	while (m_fileResourceH.ReadStringA(strA))
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
			strID = str.Right(str.GetLength() - nRight - 1);

			str.TrimLeft();
			str.TrimRight(strID);
			str.TrimRight();
			str.TrimLeft(_T("#define"));
			str.TrimLeft();

			//
			std::pair<CString, int> aID;
			aID.first = str;
			aID.second = _ttoi(strID);
			m_vIDs.push_back(aID);

			//
			m_mapStrIntIDs[aID.first] = aID.second;
			m_mapIntStrIDs[aID.second] = aID.first;
		}
	}

	if (m_bLoadFromFile)
	{
		BYTE *pBuf = m_fileResourceH.Detach();
		delete []pBuf;
	}

	return 0;
}

int CUIMgr::DelayLoadWindow(UIWNDPROPERTIES *pUIWndProp)
{
	return 0;
}

int AddBOM(const CString &strTemp, const CString &strFile)
{
	// Add BOM(byte-order mark, UFT-8 header) to the target file
	// Since the msxml DON'T add the BOM.
	// If no BOM, the ansi text is equal to utf-8 text.
	FILE *fpTemp = TFOPEN(strTemp, _T("rb"));
	ASSERT(fpTemp != NULL);
	fseek(fpTemp, 0, SEEK_END);
	int nLen = ftell(fpTemp);
	BOOL bHasBOM = FALSE;
	if (nLen >= 3)
	{
		fseek(fpTemp, 0, SEEK_SET);
		BYTE btUTF8[3];
		fread(&btUTF8, 3, 1, fpTemp);

		if (btUTF8[0] == 0xEF && btUTF8[1] == 0xBB && btUTF8[2] == 0xBF)
		{
			bHasBOM = TRUE;
		}
	}
	fclose(fpTemp);

	//
	if (bHasBOM)
	{
		CopyFile(strTemp, strFile, FALSE);
		DeleteFile(strTemp);
	}
	else
	{
		FILE *fp = TFOPEN(strFile, _T("wb"));
		BYTE btUTF8[3];
		btUTF8[0] = 0xEF;
		btUTF8[1] = 0xBB;
		btUTF8[2] = 0xBF;
		fwrite(btUTF8, sizeof(btUTF8), 1, fp);

		// Copy file
		FILE *fpTemp = TFOPEN(strTemp, _T("rb"));
		ASSERT(fpTemp != NULL);
		for (int i = 0; i < nLen ; i++)
		{
			BYTE bt;
			fread(&bt, 1, 1, fpTemp);
			fwrite(&bt, 1, 1, fp);
		}
		fclose(fp);
		fclose(fpTemp);

		// Delete temp file
		DeleteFile(strTemp);
	}

	return 0;
}


#define SAVE_STYLE(STYLE_NAME_MACRO, vpStyle, StructPropName) \
	{ \
		XML::IXMLDOMElementPtr pStyleElementGroup = xmlDoc->createElement(STYLE_NAME_MACRO); \
		pStyles->appendChild(pStyleElementGroup); \
		size_t nCount = vpStyle.size(); \
		for (size_t i=0; i<nCount; ++i) \
		{ \
			StructPropName *pStyleProp = vpStyle[i]; \
			XML::IXMLDOMElementPtr pStyleElement = xmlDoc->createElement((LPCTSTR)GetControlTypeString(pStyleProp->m_eControlType)); \
			pStyleElementGroup->appendChild(pStyleElement); \
			SaveControlProperties(xmlDoc, pStyleElement, pStyleProp, NULL); \
		} \
	}


int CUIMgr::SaveSkin()
{
	int nRet = 0;

	// save *.xui project file.
	HRESULT hr = S_OK;
	XML::IXMLDOMDocumentPtr xmlDoc(__uuidof(DOMDocument));

	// save xml header
	if (!m_bConverting)
	{
		CWindowDC dc(NULL);
		m_lLogPixelsX = GetDPIX();
		m_lLogPixelsY = GetDPIY();
	}
	XML::IXMLDOMElementPtr pRootElement = SaveConfig(xmlDoc);

	// save all windows
	SaveAllWindows(xmlDoc, pRootElement, &m_vpWindows);

	// Save all resources
	SaveAllResources(xmlDoc, pRootElement, &m_resDefaultFont, &m_vpResFont, &m_resDefaultColor, &m_vpColorRes);

	// Save all styles
	XML::IXMLDOMElementPtr pStyles = xmlDoc->createElement("Styles");
	hr = pRootElement->appendChild(pStyles);

	SAVE_STYLE("PushButtons", m_vpStylePushButton, BTNPROPERTIES)
	SAVE_STYLE("CheckBoxes", m_vpStyleCheckBox, CHKPROPERTIES)
	SAVE_STYLE("RadioButtons", m_vpStyleRadioButton, RADPROPERTIES)
	SAVE_STYLE("ComboBoxes", m_vpStyleComboBox, CMBPROPERTIES)
	SAVE_STYLE("Statics", m_vpStyleStatic, TXTPROPERTIES)
	SAVE_STYLE("Edits", m_vpStyleEdit, EDTPROPERTIES)
	SAVE_STYLE("Sliders", m_vpStyleSlider, SLDPROPERTIES)
	SAVE_STYLE("Progresses", m_vpStyleProgress, PRGPROPERTIES)
	SAVE_STYLE("Spins", m_vpStyleSpin, SPNPROPERTIES)
	SAVE_STYLE("HotKeys", m_vpStyleHotKey, HOTPROPERTIES)
	SAVE_STYLE("IPAdresses", m_vpStyleIPAddress, IPAPROPERTIES)
	SAVE_STYLE("SliderExes", m_vpStyleSliderEx, SLDEXPROPERTIES)
	SAVE_STYLE("ListControls", m_vpStyleListCtrl, LSTCTRLPROPERTIES)
	SAVE_STYLE("TreeControls", m_vpStyleTreeCtrl, TREECTRLPROPERTIES)
	SAVE_STYLE("TreeLists", m_vpStyleTreeList, TREELISTPROPERTIES)
	SAVE_STYLE("RichEdits", m_vpStyleRichEdit, RICHEDITPROPERTIES)
	SAVE_STYLE("IMRichEdits", m_vpStyleIMRichEdit, IMREPROPERTIES)
	SAVE_STYLE("HtmlControls", m_vpStyleHtmlCtrl, HTMLCTRLPROPERTIES)
	SAVE_STYLE("DateTimes", m_vpStyleDateTime, DATETIMECTRLPROPERTIES)
	SAVE_STYLE("WaveControls", m_vpStyleWaveCtrl, WAVECTRLPROPERTIES)
	SAVE_STYLE("Menus", m_vpStyleMenu, MENUPROPERTIES)
	SAVE_STYLE("Menubars", m_vpStyleMenuBar, MENUBARPROPERTIES)
	SAVE_STYLE("Headers", m_vpStyleHeader, HEADERPROPERTIES)
	SAVE_STYLE("HorScrollbars", m_vpStyleHorScrollBar, HORSBPROPERTIES)
	SAVE_STYLE("VerScrollbars", m_vpStyleVerScrollBar, VERSBPROPERTIES)
	SAVE_STYLE("Splitterbars", m_vpStyleSplitter, SPLITTERPROPERTIES)
	SAVE_STYLE("StackPanel", m_vpStylePanelStack, STACKPNLPROPERTIES)
	SAVE_STYLE("WindowlessLine", m_vpStyleWLLine, LINEWLPROPERTIES)
	SAVE_STYLE("WindowlessPicture", m_vpStyleWLPic, PICWLPROPERTIES)

	// Format xml code
	WellFormXml(xmlDoc, pRootElement, 1);

	// Generate xml file
	CString strTemp = GetUIPath();
	strTemp += '~';

	xmlDoc->save(_variant_t(strTemp.GetBuffer(0)));

	AddBOM(strTemp, GetUIPath());

	// Save Resource.h by CUIMgr::m_vpWindows not by CUIMgr::m_vIDs.
	SaveIDDefined();

	return nRet;
}

BOOL GenerateOneID(LPCTSTR lpszID, int &rnNextID, std::vector<std::pair<CString, int> > &IdArray)
{
	CString strPreDefinedID[] =
	{
		_T("IDC_STATIC"),

		_T("IDOK"),
		_T("IDCANCEL"),
		_T("IDABORT"),
		_T("IDRETRY"),
		_T("IDIGNORE"),
		_T("IDYES"),
		_T("IDNO"),
#if(WINVER >= 0x0400)
		_T("IDCLOSE"),
		_T("IDHELP"),
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)
		_T("IDTRYAGAIN"),
		_T("IDCONTINUE"),
#endif /* WINVER >= 0x0500 */
	};

	int nPreDefinedIDCount = sizeof(strPreDefinedID) / sizeof(CString);

	// Check the lpszID exist or not.
	BOOL bIsExist = FALSE;

	// If the id equal to the pre-defined ID
	for (int j = 0; j < nPreDefinedIDCount; ++j)
	{
		if (strPreDefinedID[j] == lpszID)
		{
			bIsExist = TRUE;
			break;
		}
	}

	if (bIsExist)
	{
		return FALSE;
	}

	size_t sizeTemp = IdArray.size();
	for (size_t i = 0; i < sizeTemp; ++i)
	{
		const std::pair<CString, int> *pIDs = &IdArray[i];

		// If the id equal to the pre-defined ID
		if (pIDs->first.Compare(lpszID) == 0)
		{
			bIsExist = TRUE;
			break;
		}
	}

	if (bIsExist)
	{
		return FALSE;
	}

	// If the new ID not exist, add to ID list IdArray.
	IdArray.push_back(std::pair<CString, int>(lpszID, rnNextID));

	rnNextID++;

	return TRUE;
}

BOOL GenerateIDsArray(const std::vector<UIWNDPROPERTIES *> *pvWindows, std::vector<std::pair<CString, int> > &vIDs)
{
	int nNextID = 1000;
	size_t nWindowCount = pvWindows->size();
	for (size_t i = 0; i < nWindowCount; ++i)
	{
		const UIWNDPROPERTIES *pUIProp = (*pvWindows)[i];

		CString strWndID = (LPCTSTR)pUIProp->m_strID;
		GenerateOneID(strWndID, nNextID, vIDs);

		size_t nCtrlCount = pUIProp->m_vControls.size();
		for (size_t j = 0; j < nCtrlCount; ++j)
		{
			CTRLPROPERTIES *pCtrl = pUIProp->m_vControls[j];
			GenerateOneID(pCtrl->m_strID, nNextID, vIDs);
		}
	}

	return TRUE;
}

BOOL GenerateDefineIDsArray(const std::vector<std::pair<CString, int> > &vIDs, CStringArray &IdArray)
{
	int nNextID = 1000;
	CStringArray saTemp;

	size_t nIDCount = vIDs.size();
	for (size_t i = 0; i < nIDCount; ++i)
	{
		const std::pair<CString, int> *pID = &vIDs[i];

		CString strID;
		strID = "#define ";
		strID += pID->first;

		CString strNextID;
		strNextID.Format(_T("%d"), pID->second);
		if (strID.GetLength() < 39)
		{
			int nLen = 39 - strID.GetLength();
			for (int i = 0; i < nLen; ++i)
			{
				strID += " ";
			}
		}
		else
		{
			strID += " ";
		}
		strID += strNextID;
		strID += "\n";
		TCHAR szBuf[MAX_PATH] = {0};
		TSTRCPY(szBuf, MAX_PATH, strID.GetBuffer(0));
		IdArray.Add(strID);
	}

	return TRUE;
}

int CUIMgr::SaveIDDefined()
{
	// Check the id list changed or not.
	// If the Resource.h not change, Don't save it, Since stdafx.h include the Resource.h. This can improve the compile speed.
	std::vector<std::pair<CString, int> > vIDs;
	GenerateIDsArray(&m_vpWindows, vIDs);

	if (vIDs == m_vIDs)
	{
		return 1;	// Not change.
	}

	int nRet = CheckFileWritable(m_strUIResourcePath, TRUE);
	if (nRet != 0)
	{
		return nRet;
	}

	// create the Resource.h file alway as ANSI
	CString strTempResource = m_strUIResourcePath;
	strTempResource += _T("~");
	CStdioFile fileResource;
	if (!fileResource.Open(strTempResource, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		return -1;
	}

	// Warning
	fileResource.WriteString(_T("//WARNING: DO NOT EDIT OR DELETE THIS HEADER FILE!\n\n"));

	// file header
	fileResource.WriteString(_T("#pragma once\n\n"));

	CStringArray IdArray;
	GenerateDefineIDsArray(vIDs, IdArray);
	INT_PTR nIDCount = IdArray.GetSize();
	for (INT_PTR i = 0; i < nIDCount; ++i)
	{
		const CString strID = IdArray.GetAt(i);
		fileResource.WriteString(strID);
	}

	fileResource.Close();

	// Copy temp file to resource.h
	CopyFile(strTempResource, m_strUIResourcePath, FALSE);
	DeleteFile(strTempResource);

	// Update CUIMgr::m_vIDs.
	LoadIDDefined();
	ASSERT(m_vIDs == vIDs);

	return 0;
}

int CUIMgr::SetDelayLoadWindow(BOOL bDelayLoadWindow)
{
	m_bDelayLoadWindow = bDelayLoadWindow;

	return 0;
}

BOOL CUIMgr::IsDelayLoadWindow()
{
	return m_bDelayLoadWindow;
}

int CompareVersion()
{
	int nCurMajor = CUIMgr::GetXUIMajorVersion();
	int nCurMinor = CUIMgr::GetXUIMinorVersion();
	int nHighlightMajor = CUIMgr::GetLibUIDKMajorVersion();
	int nHighlightManor = CUIMgr::GetLibUIDKMinorVersion();

	if (nCurMajor < nHighlightMajor)
	{
		return -1;
	}
	if (nCurMajor > nHighlightMajor)
	{
		return 1;
	}

	// nCurMajor == nHighlightMajor
	if (nCurMinor < nHighlightManor)
	{
		return -1;
	}
	if (nCurMinor > nHighlightManor)
	{
		return 1;
	}

	// nCurMajor == nHighlightMajor && nCurMinor == nHighlightManor
	return 0;
}

int CompareChangeList()
{
	LONG lXUIChangeList = CUIMgr::GetXUIChangeList();
	LONG lLibUIDKChangeList = CUIMgr::GetLibUIDKChangeList();

	if (lXUIChangeList != lLibUIDKChangeList)
	{
		return -1;
	}

	return 0;
}

int CheckLicense()
{
#ifdef TRIAL_DATE
	SYSTEMTIME stime;
	GetSystemTime(&stime);
	int nTime = stime.wYear * 10000 + stime.wMonth * 100 + stime.wDay;
	if (nTime > 20180630)
	{
		return -1;
	}
#endif // TRIAL_DATE

#ifndef TRIAL // the trial version don't check license key.
#ifdef _USE_LICENSE
	if (IsDebuggerPresent())
	{
		CLicense license;
#ifdef ENTERPRISE
#ifdef _USE_DOG
		if (!license.IsDogLicensed(3))
#else
		if (!license.IsLicensed(3))
#endif
#else
#ifdef _USE_DOG
		if (!license.IsDogLicensed(2))
#else
		if (!license.IsLicensed(2))
#endif
#endif
		{
			return E_INVALIDLICENSE;
		}
	}
#endif // _USE_LICENSE
#endif // TRIAL

	return 0;
}

#if (_MSC_VER <= 1310) // VC2003

fnDPA_InsertPtr IUI_DPA_InsertPtr = NULL;
fnDPA_DeletePtr IUI_DPA_DeletePtr = NULL;
fnDPA_GetPtr IUI_DPA_GetPtr = NULL;
fnDPA_SetPtr IUI_DPA_SetPtr = NULL;
fnDPA_EnumCallback IUI_DPA_EnumCallback = NULL;
fnDPA_DeleteAllPtrs IUI_DPA_DeleteAllPtrs = NULL;
fnDPA_DestroyCallback IUI_DPA_DestroyCallback = NULL;
fnDPA_Destroy IUI_DPA_Destroy = NULL;
fnDPA_Sort IUI_DPA_Sort = NULL;
fnDPA_Create IUI_DPA_Create = NULL;
fnDPA_CreateEx IUI_DPA_CreateEx = NULL;
fnDPA_Grow IUI_DPA_Grow = NULL;
fnDPA_Clone IUI_DPA_Clone = NULL;
fnDPA_GetPtrIndex IUI_DPA_GetPtrIndex = NULL;

fnDSA_Destroy IUI_DSA_Destroy = NULL;
fnDSA_GetItemPtr IUI_DSA_GetItemPtr = NULL;
fnDSA_InsertItem IUI_DSA_InsertItem = NULL;
fnDSA_Create IUI_DSA_Create = NULL;
fnDSA_GetItem IUI_DSA_GetItem = NULL;
fnDSA_SetItem IUI_DSA_SetItem = NULL;
fnDSA_DeleteItem IUI_DSA_DeleteItem = NULL;
fnDSA_DestroyCallback IUI_DSA_DestroyCallback = NULL;
fnReleaseActCtx IUI_ReleaseActCtx = NULL;

#endif

int GetExportProcAddress()
{
#if (_MSC_VER <= 1310) // VC2003
	HMODULE hComCtrl32 = LoadLibrary(_T("ComCtl32.dll"));

	IUI_DPA_InsertPtr = (fnDPA_InsertPtr)GetProcAddress(hComCtrl32, _T("DPA_InsertPtr"));
	_ASSERT(IUI_DPA_InsertPtr != NULL);

	IUI_DPA_DeletePtr = (fnDPA_DeletePtr)GetProcAddress(hComCtrl32, _T("DPA_DeletePtr"));
	_ASSERT(IUI_DPA_DeletePtr != NULL);

	IUI_DPA_GetPtr = (fnDPA_GetPtr)GetProcAddress(hComCtrl32, _T("DPA_GetPtr"));
	_ASSERT(IUI_DPA_GetPtr != NULL);

	IUI_DPA_SetPtr = (fnDPA_SetPtr)GetProcAddress(hComCtrl32, _T("DPA_SetPtr"));
	_ASSERT(IUI_DPA_SetPtr != NULL);

	IUI_DPA_EnumCallback = (fnDPA_EnumCallback)GetProcAddress(hComCtrl32, _T("DPA_EnumCallback"));
	_ASSERT(IUI_DPA_EnumCallback != NULL);

	IUI_DPA_DeleteAllPtrs = (fnDPA_DeleteAllPtrs)GetProcAddress(hComCtrl32, _T("DPA_DeleteAllPtrs"));
	_ASSERT(IUI_DPA_DeleteAllPtrs != NULL);

	IUI_DPA_DestroyCallback = (fnDPA_DestroyCallback)GetProcAddress(hComCtrl32, _T("DPA_DestroyCallback"));
	_ASSERT(IUI_DPA_DestroyCallback != NULL);

	IUI_DPA_Destroy = (fnDPA_Destroy)GetProcAddress(hComCtrl32, _T("DPA_Destroy"));
	_ASSERT(IUI_DPA_Destroy != NULL);

	IUI_DPA_Sort = (fnDPA_Sort)GetProcAddress(hComCtrl32, _T("DPA_Sort"));
	_ASSERT(IUI_DPA_Sort != NULL);

	IUI_DPA_Create = (fnDPA_Create)GetProcAddress(hComCtrl32, _T("DPA_Create"));
	_ASSERT(IUI_DPA_Create != NULL);

	IUI_DPA_CreateEx = (fnDPA_CreateEx)GetProcAddress(hComCtrl32, _T("DPA_CreateEx"));
	_ASSERT(IUI_DPA_CreateEx != NULL);

	IUI_DPA_Grow = (fnDPA_Grow)GetProcAddress(hComCtrl32, _T("DPA_Grow"));
	_ASSERT(IUI_DPA_Grow != NULL);

	IUI_DPA_Clone = (fnDPA_Clone)GetProcAddress(hComCtrl32, _T("DPA_Clone"));
	_ASSERT(IUI_DPA_Clone != NULL);

	IUI_DPA_GetPtrIndex = (fnDPA_GetPtrIndex)GetProcAddress(hComCtrl32, _T("DPA_GetPtrIndex"));
	_ASSERT(IUI_DPA_GetPtrIndex != NULL);

	// DSA_*
	IUI_DSA_Destroy = (fnDSA_Destroy)GetProcAddress(hComCtrl32, _T("DSA_Destroy"));
	_ASSERT(IUI_DSA_Destroy != NULL);

	IUI_DSA_GetItemPtr = (fnDSA_GetItemPtr)GetProcAddress(hComCtrl32, _T("DSA_GetItemPtr"));
	_ASSERT(IUI_DSA_GetItemPtr != NULL);

	IUI_DSA_InsertItem = (fnDSA_InsertItem)GetProcAddress(hComCtrl32, _T("DSA_InsertItem"));
	_ASSERT(IUI_DSA_InsertItem != NULL);

	IUI_DSA_Create = (fnDSA_Create)GetProcAddress(hComCtrl32, _T("DSA_Create"));
	_ASSERT(IUI_DSA_Create != NULL);

	IUI_DSA_GetItem = (fnDSA_GetItem)GetProcAddress(hComCtrl32, _T("DSA_GetItem"));
	_ASSERT(IUI_DSA_GetItem != NULL);

	IUI_DSA_SetItem = (fnDSA_SetItem)GetProcAddress(hComCtrl32, _T("DSA_SetItem"));
	_ASSERT(IUI_DSA_SetItem != NULL);

	IUI_DSA_DeleteItem = (fnDSA_DeleteItem)GetProcAddress(hComCtrl32, _T("DSA_DeleteItem"));
	_ASSERT(IUI_DSA_DeleteItem != NULL);

	IUI_DSA_DestroyCallback = (fnDSA_DestroyCallback)GetProcAddress(hComCtrl32, _T("DSA_DestroyCallback"));
	_ASSERT(IUI_DSA_DestroyCallback != NULL);

	FreeLibrary(hComCtrl32);
	hComCtrl32 = NULL;

	HMODULE hKernel32 = LoadLibrary(_T("Kernel32.dll"));

	IUI_ReleaseActCtx = (fnReleaseActCtx)GetProcAddress(hKernel32, _T("ReleaseActCtx"));
	_ASSERT(IUI_ReleaseActCtx != NULL);

	FreeLibrary(hKernel32);
	hKernel32 = NULL;
#endif // #if (_MSC_VER <= 1310) // VC2003

	return 0;
}

int CUIMgr::SetUIPath(LPCTSTR lpszUIPath, BOOL bCheckVersion/* = TRUE*/)
{
	int nRet = CheckLicense();
	if (nRet != 0)
	{
		LicenseWarning();
		return nRet;
	}

	// Init Ole
	_AFX_THREAD_STATE *pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm)
	{
		if (!AfxOleInit())
		{
			return E_OLEUNINIT;
		}
	}

	GetExportProcAddress();

	// Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_bLoadFromFile = TRUE;

	ASSERT(lpszUIPath != NULL);
	ASSERT(AfxIsValidString(lpszUIPath));

	BOOL bIsAbsolutePath = FALSE;
	int nLen = (int)_tcslen(lpszUIPath);
	int i = 0;
	for (i = 0; i < nLen; ++i)
	{
		if (lpszUIPath[i] == ':')
		{
			bIsAbsolutePath = TRUE;
			break;
		}
	}

	if (bIsAbsolutePath)
	{
		m_strUIPath = lpszUIPath;
	}
	else
	{
		TCHAR szFullName[MAX_PATH] = {0};
		GetModuleFileName(NULL, szFullName, MAX_PATH - 1);
		TCHAR szDisc[_MAX_PATH] = {0};
		TCHAR szPath[_MAX_PATH] = {0};
		TSPLITPATH(szFullName, szDisc, _MAX_PATH, szPath, _MAX_PATH, NULL, 0, NULL, 0);
		TSTRCAT(szDisc, _MAX_PATH, szPath);
		m_strUIPath = szDisc;
		m_strUIPath += lpszUIPath;
	}

	if (!PathFileExists(m_strUIPath))
	{
		m_strUIPath.Empty();
		if (!pState->m_bNeedTerm)
		{
			AfxOleTerm();
		}
		return E_SETUIPATH;
	}

	// init path of resource.h file.
	TCHAR szDisc[_MAX_PATH] = {0};
	TCHAR szDir[_MAX_PATH] = {0};
	TSPLITPATH(m_strUIPath, szDisc, _MAX_PATH, szDir, _MAX_PATH, NULL, 0, NULL, 0);
	TSTRCAT(szDisc, _MAX_PATH, szDir);
	if (szDisc[_tcslen(szDisc) - 1] == '/' || szDisc[_tcslen(szDisc) - 1] == '\\')
	{
		TSTRCAT(szDisc, _MAX_PATH, _T("Resource.h"));
	}
	else
	{
		TSTRCAT(szDisc, _MAX_PATH, _T("\\Resource.h"));
	}
	m_strUIResourcePath = szDisc;

	// init windows tree struct
	DWORD dwTime = GetTickCount();
	nRet = LoadSkin();
	DWORD dwTime2 = GetTickCount() - dwTime;
	TRACE(_T("[LibUIDK]===Load skins spend %d ms\n"), dwTime2);
	if (nRet != 0)
	{
		if (!pState->m_bNeedTerm)
		{
			AfxOleTerm();
		}
		return nRet;
	}

	if (CompareVersion() < 0)
	{
		TRACE(_T("[LibUIDK]===Warning: The version of skin file older than the LibUIDK, so some features will be lost.\n"));
		if (!pState->m_bNeedTerm)
		{
			AfxOleTerm();
		}
		return E_OLDERVERSION;
	}

	// If the the.xui load by UIShop.exe or load by myself, Don't check change list.
#ifndef TRIAL // the trial version don't check license key.
#ifdef _USE_LICENSE
	if (g_bCheckChangeList && CompareChangeList() != 0)
	{
		TRACE(_T("[LibUIDK]===Error: The change list in *.xui file not match the LibUIDK library.\n"));
		if (!pState->m_bNeedTerm)
		{
			AfxOleTerm();
		}
		return E_CHANGELIST_NOTMATCH;
	}
#endif
#endif

	// Init LibUIDK_g_iui_thread_state
	_IUI_THREAD_STATE *pIUIState = IUIGetThreadState();
	pIUIState->m_hTrackingMenu = NULL;
	pIUIState->m_hTrackingWindow = NULL;
	pIUIState->m_hWndMenu = NULL;
	pIUIState->m_pWLCapture = NULL;
	pIUIState->m_pWLMouseIn = NULL;

	return 0;
}

int CUIMgr::SetUIPath(LPCTSTR lpszUIID, LPCTSTR lpszResouceID, LPCTSTR lpszResourceType)
{
	int nRet = CheckLicense();
	if (nRet != 0)
	{
		LicenseWarning();
		return nRet;
	}

	// Init Ole
	_AFX_THREAD_STATE *pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm)
	{
		if (!AfxOleInit())
		{
			return E_OLEUNINIT;
		}
	}

	GetExportProcAddress();

	// Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_bLoadFromFile = FALSE;

	m_strUIID = lpszUIID;
	m_strUIResourcehID = lpszResouceID;
	m_strUIResourceType = lpszResourceType;
	// init windows tree struct
	DWORD dwTime = GetTickCount();
	nRet = LoadSkin();
	DWORD dwTime2 = GetTickCount() - dwTime;
	TRACE(_T("[LibUIDK]===Load skins spend %d ms\n"), dwTime2);
	if (nRet != 0)
	{
		return nRet;
	}

	return 0;
}

int CUIMgr::SetSource(LPCTSTR lpszUIPath)
{
	m_strUIPath = lpszUIPath;
	return 0;
}

CString CUIMgr::GetUIPath()
{
	return m_strUIPath;
}

CString CUIMgr::GetUIResourcePath()
{
	return m_strUIResourcePath;
}

CString CUIMgr::GetUIPathWithoutTitle()
{
	TCHAR szDisc[_MAX_PATH] = {0};
	TCHAR szPath[_MAX_PATH] = {0};
	TSPLITPATH(m_strUIPath, szDisc, _MAX_PATH, szPath, _MAX_PATH, NULL, 0, NULL, 0);
	m_strUIPathWithoutTitle = szDisc;
	m_strUIPathWithoutTitle += szPath;
	if (m_strUIPathWithoutTitle.Right(1) != "/" && m_strUIPathWithoutTitle.Right(1) != "\\")
	{
		m_strUIPathWithoutTitle += "\\";
	}

	return m_strUIPathWithoutTitle;
}

CString CUIMgr::GetUIResourceType()
{
	return m_strUIResourceType;
}

BOOL CUIMgr::IsLoadFromFile()
{
	return m_bLoadFromFile;
}

int CUIMgr::ConvertUI2CurVer()
{
	int nRet = PromptConvert();
	if (nRet != 0)
	{
		return nRet;
	}

	// Load *.ui
	COldUIMgr::SetUIPath(m_strUIPath, FALSE, FALSE);

	// Copy date from COldUIMgr to CUIMgr
	m_lLogPixelsX = COldUIMgr::GetLogPixelsX();
	m_lLogPixelsY = COldUIMgr::GetLogPixelsY();
	m_bEnableDPI = COldUIMgr::IsEnableDPI();
	m_lXUIMajorVersion = m_lLibUIDKMajorVersion;
	m_lXUIMinorVersion = m_lLibUIDKMinorVersion;
	m_lXUIBuildVersion = m_lLibUIDKBuildVersion;
	m_vpWindows = COldUIMgr::m_vpWindows;
	//	m_vpFontRes = COldUIMgr::m_vpFontRes;
	m_vpColorRes = COldUIMgr::m_vpColorRes;

	CString strNewPrj = m_strUIPath;
	PathRemoveFileSpecEx(strNewPrj, TRUE);
	strNewPrj += GetFileTitleEx(m_strUIPath);
	strNewPrj += _T(".xui");
	m_strUIPath = strNewPrj;

	nRet = CheckFileWritable(m_strUIPath, TRUE);
	if (nRet != 0)
	{
		return -3;
	}

	m_bConverting = TRUE;
	SaveSkin();
	m_bConverting = FALSE;

	return 0;
}

int CUIMgr::ConvertXUI2CurVer()
{
	//	ASSERT(m_uXUIVersion < sm_nCurXUIVersion);

	int nRet = PromptConvert();
	if (nRet != 0)
	{
		return nRet;
	}

	// Load open function name.

	return 0;
}

int CUIMgr::PromptConvert()
{
	CString strInfo;
	strInfo.Format(_T("The project\r\n\r\n'%s'\r\n\r\nmust be converted to the current project format. After it has been converted,\r\nyou will not be able to edit this project in previous versions of UIShop.\r\nConvert and open this project?"), m_strUIPath);
	int nRet = AfxMessageBox(strInfo, MB_YESNO | MB_ICONWARNING);
	if (nRet == IDNO)
	{
		return -1;    // return NULL will remove file from recent file list.
	}

	return 0;
}

UIWNDPROPERTIES *CUIMgr::AllocUIWNDPROPERTIES()
{
	return new UIWNDPROPERTIES;
}

int CUIMgr::FreeUIWNDPROPERTIES(UIWNDPROPERTIES *pUIProp)
{
	size_t nChildCount = pUIProp->m_vControls.size();
	for (size_t j = 0; j < nChildCount; ++j)
	{
		CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];
		delete pCtrlProp;
	}
	pUIProp->m_vControls.clear();

	delete pUIProp;

	return 0;
}

int CUIMgr::GetWindowCount()
{
	size_t nCount = m_vpWindows.size();
	return (int)nCount;
}

UIWNDPROPERTIES *CUIMgr::GetWindowItem(int nIndex)
{
	int nCount = (int)m_vpWindows.size();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return NULL;
	}

	return m_vpWindows[nIndex];
}

int CUIMgr::AddWindowItem(UIWNDPROPERTIES *pWindow)
{
	m_vpWindows.push_back(pWindow);

	size_t nCount = m_vpWindows.size();
	return int(nCount - 1);
}

int CUIMgr::DeleteWindowItem(int nIndex)
{
	int nCount = (int)m_vpWindows.size();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	UIWNDPROPERTIES *pUIProp = m_vpWindows[nIndex];

	size_t nChildCount = pUIProp->m_vControls.size();
	for (size_t j = 0; j < nChildCount; ++j)
	{
		CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];
		delete pCtrlProp;
	}
	pUIProp->m_vControls.clear();

	delete pUIProp;

	m_vpWindows.erase(m_vpWindows.begin() + nIndex);

	return 0;
}

int CUIMgr::DeleteAllWindowItems()
{
	size_t nWndCount = m_vpWindows.size();
	for (size_t i = 0; i < nWndCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		size_t nChildCount = pUIProp->m_vControls.size();
		for (size_t j = 0; j < nChildCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];
			delete pCtrlProp;
		}
		pUIProp->m_vControls.clear();

		delete pUIProp;
	}

	m_vpWindows.clear();

	return 0;
}

int CUIMgr::GetWindowIndex(const UIWNDPROPERTIES *pUIProp)
{
	size_t nIndex = -1;

	size_t nCount = m_vpWindows.size();
	for (size_t i = 0; i < nCount ; ++i)
	{
		UIWNDPROPERTIES *p = m_vpWindows[i];
		if (p == pUIProp)
		{
			nIndex = i;
			break;
		}
	}

	return (int)nIndex;
}

int CUIMgr::GetWindowIndex(const CString &strWindowID)
{
	size_t nIndex = -1;

	size_t nCount = m_vpWindows.size();
	for (size_t i = 0; i < nCount ; ++i)
	{
		UIWNDPROPERTIES *p = m_vpWindows[i];
		if (p->m_strID == (LPCTSTR)strWindowID)
		{
			nIndex = i;
			break;
		}
	}

	return (int)nIndex;
}

int CUIMgr::SwapWindowItem(int nFirst, int nLast)
{
	int nCount = (int)m_vpWindows.size();
	if (nFirst < 0 || nFirst >= nCount || nLast < 0 || nLast >= nCount)
	{
		return -1;
	}

	if (nFirst == nLast)
	{
		return 1;
	}

	UIWNDPROPERTIES *pTempUI = m_vpWindows[nFirst];
	m_vpWindows[nFirst] = m_vpWindows[nLast];
	m_vpWindows[nLast] = pTempUI;

	return 0;
}

std::vector<UIWNDPROPERTIES *> *CUIMgr::GetUIWndPropertiesList()
{
	return &m_vpWindows;
}

UIWNDPROPERTIES *CUIMgr::GetUIWNDPROPERTIES(LPCTSTR lpszWindowID)
{
	UIWNDPROPERTIES *pRet = NULL;

	size_t nSize = m_vpWindows.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		if (pUIProp->m_strID == lpszWindowID)
		{
			pRet = pUIProp;
			break;
		}
	}

	return pRet;
}

UIWNDPROPERTIES *CUIMgr::GetUIWNDPROPERTIES(UINT uWindowID)
{
	UIWNDPROPERTIES *pRet = NULL;

	size_t nSize = m_vpWindows.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		if ((LONG)pUIProp->m_nID == uWindowID)
		{
			pRet = pUIProp;
			break;
		}
	}

	return pRet;
}

struct EnumWindowParam
{
	std::map<HWND, CString> *pMapCreatedHWndStringID;
	DWORD dwCurrentProcID;
};

BOOL CALLBACK EnumChildWindowCallBack(HWND hWnd, LPARAM lParam)
{
	EnumWindowParam *pEwp = (EnumWindowParam *)lParam;

	DWORD dwPid = 0;
	GetWindowThreadProcessId(hWnd, &dwPid);

	if (dwPid == pEwp->dwCurrentProcID)
	{
		BOOL bUIWnd = (BOOL)::SendMessage(hWnd, WM_ISUIWND, 0, 0);
		if (bUIWnd)
		{
#ifdef _DEBUG
			TCHAR szbuf[MAX_PATH] = {0};
			::GetWindowText(hWnd, szbuf, MAX_PATH);
#endif
			LPCTSTR lpStringID = (LPCTSTR)::GetProp(hWnd, GETPROP_UIWND_STRINGID);
			(*(pEwp->pMapCreatedHWndStringID))[hWnd] = lpStringID;
		}

		EnumChildWindows(hWnd, EnumChildWindowCallBack, lParam);
	}

	return TRUE;
}

// Get all running CUIWnd
int CUIMgr::InitRunningUIWnd()
{
	m_mapCreatedHWndStringID.clear();

	EnumWindowParam ewp;
	ewp.pMapCreatedHWndStringID = &m_mapCreatedHWndStringID;
	ewp.dwCurrentProcID = ::GetCurrentProcessId();

	EnumChildWindows(NULL, EnumChildWindowCallBack, (LPARAM)&ewp);

	return 0;
}

UIWNDPROPERTIES *CUIMgr::GetParent(const IUI_VARIANT *pVarCtrlProp, BOOL *pbParentWindow)
{
	if (pVarCtrlProp == NULL)
	{
		return NULL;
	}

	UIWNDPROPERTIES *pRetProp = NULL;

	// Find in window list
	size_t nWindowsCount = m_vpWindows.size();
	for (size_t i = 0; i < nWindowsCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		LONG lVarCount = pUIProp->GetPropertyCount();
		for (LONG lVar = 0; lVar < lVarCount; ++lVar)
		{
			IUI_VARIANT *pVariant = pUIProp->GetProperty(lVar);

			if (pVariant == pVarCtrlProp)
			{
				pRetProp = pUIProp;

				if (pbParentWindow != NULL)
				{
					*pbParentWindow = TRUE;
				}

				break;
			}
		}

		if (pRetProp != NULL)
		{
			break;
		}

		// Find children
		size_t nChildrenCount = pUIProp->m_vControls.size();
		for (size_t j = 0; j < nChildrenCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];

			LONG lVarCount = pCtrlProp->GetPropertyCount();
			for (LONG lVar = 0; lVar < lVarCount; ++lVar)
			{
				IUI_VARIANT *pVariant = pCtrlProp->GetProperty(lVar);

				if (pVariant == pVarCtrlProp)
				{
					pRetProp = pUIProp;

					if (pbParentWindow != NULL)
					{
						*pbParentWindow = FALSE;
					}

					break;
				}
			}

			if (pRetProp != NULL)
			{
				break;
			}
		}

		if (pRetProp != NULL)
		{
			break;
		}
	}

	return pRetProp;
}

#define FIND_RES_STYLE(ResStyleVar) \
	if (*ppCtrlProp == NULL) \
	{ \
		size_t nCount = ResStyleVar.size(); \
		for (size_t i=0; i<nCount; ++i) \
		{ \
			CTRLPROPERTIES *pCtrlProp = ResStyleVar[i]; \
			LONG lVarCount = pCtrlProp->GetPropertyCount(); \
			for (LONG lVar=0; lVar<lVarCount; ++lVar) \
			{ \
				IUI_VARIANT *pVariant = pCtrlProp->GetProperty(lVar); \
				if (pVariant == pVarCtrlProp) \
				{ \
					*ppCtrlProp = pCtrlProp; \
					break; \
				} \
			} \
			if (*ppCtrlProp != NULL) \
			{ \
				break; \
			} \
		} \
	}

BOOL FindProp(CTRLPROPERTIES *pCtrlProp, __in const IUI_VARIANT *pVarCtrlProp, __out CTRLPROPERTIES **ppCtrlProp)
{
	if (pCtrlProp == NULL)
	{
		return FALSE;
	}
	if (pVarCtrlProp = NULL)
	{
		return FALSE;
	}
	if (ppCtrlProp == NULL)
	{
		return FALSE;
	}

	LONG lVarCount = pCtrlProp->GetPropertyCount();
	for (LONG lVar = 0; lVar < lVarCount; ++lVar)
	{
		IUI_VARIANT *pVariant = pCtrlProp->GetProperty(lVar);
		if (pVariant == pVarCtrlProp)
		{
			*ppCtrlProp = pCtrlProp;
			break;
		}
	}
	if (*ppCtrlProp != NULL)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL FindFontProp(__in FONTRESOURCEITEM *presFont, __in const IUI_VARIANT *pVarCtrlProp, __out CTRLPROPERTIES **ppCtrlProp)
{
	if (presFont == NULL)
	{
		return FALSE;
	}

	if (presFont->m_bConditionFont)
	{
		std::map<CString, RESFONTPROPERTIES *>::iterator it = presFont->m_mapConditionFont.begin();
		for (; it != presFont->m_mapConditionFont.end(); ++it)
		{
			CTRLPROPERTIES *pCtrlProp = it->second;
			BOOL bFound = FindProp(pCtrlProp, pVarCtrlProp, ppCtrlProp);
			if (bFound)
			{
				return TRUE;
			}
		}
	}
	else
	{
		CTRLPROPERTIES *pCtrlProp = presFont->m_pFontResProp;
		BOOL bFound = FindProp(pCtrlProp, pVarCtrlProp, ppCtrlProp);
		if (bFound)
		{
			return TRUE;
		}
	}

	return FALSE;
}

int CUIMgr::FindCtrlProp(/*in*/const IUI_VARIANT *pVarCtrlProp, /*out*/CTRLPROPERTIES **ppCtrlProp)
{
	if (pVarCtrlProp == NULL || ppCtrlProp == NULL)
	{
		return -2;
	}

	*ppCtrlProp = NULL;

	// Find in window list
	size_t nWindowsCount = m_vpWindows.size();
	size_t i = 0;
	for (i = 0; i < nWindowsCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		LONG lVarCount = pUIProp->GetPropertyCount();
		for (LONG lVar = 0; lVar < lVarCount; ++lVar)
		{
			IUI_VARIANT *pVariant = pUIProp->GetProperty(lVar);

			if (pVariant == pVarCtrlProp)
			{
				*ppCtrlProp = pUIProp;

				break;
			}
		}

		if (*ppCtrlProp != NULL)
		{
			break;
		}

		// Find children
		size_t nChildrenCount = pUIProp->m_vControls.size();
		for (size_t j = 0; j < nChildrenCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];

			LONG lVarCount = pCtrlProp->GetPropertyCount();
			for (LONG lVar = 0; lVar < lVarCount; ++lVar)
			{
				IUI_VARIANT *pVariant = pCtrlProp->GetProperty(lVar);

				if (pVariant == pVarCtrlProp)
				{
					*ppCtrlProp = pCtrlProp;

					break;
				}
			}

			if (*ppCtrlProp != NULL)
			{
				break;
			}
		}

		if (*ppCtrlProp != NULL)
		{
			break;
		}
	}

	// Find in Default Font resource
	BOOL bFound = FindFontProp(&m_resDefaultFont, pVarCtrlProp, ppCtrlProp);
	if (bFound)
	{
		return 0;
	}

	// Find in Font list.
	size_t nCount = m_vpResFont.size();
	for (i = 0; i < nCount; ++i)
	{
		FONTRESOURCEITEM *pFontItem = m_vpResFont[i];

		BOOL bFound = FindFontProp(pFontItem, pVarCtrlProp, ppCtrlProp);
		if (bFound)
		{
			return 0;
		}
	}

	// Find in resource and style
	FIND_RES_STYLE(m_vpColorRes)
	FIND_RES_STYLE(m_vpStylePushButton)
	FIND_RES_STYLE(m_vpStyleCheckBox)
	FIND_RES_STYLE(m_vpStyleRadioButton)
	FIND_RES_STYLE(m_vpStyleComboBox)
	FIND_RES_STYLE(m_vpStyleStatic)
	FIND_RES_STYLE(m_vpStyleEdit)
	FIND_RES_STYLE(m_vpStyleSlider)
	FIND_RES_STYLE(m_vpStyleProgress)
	FIND_RES_STYLE(m_vpStyleSpin)
	FIND_RES_STYLE(m_vpStyleHotKey)
	FIND_RES_STYLE(m_vpStyleIPAddress)
	FIND_RES_STYLE(m_vpStyleSliderEx)
	FIND_RES_STYLE(m_vpStyleListCtrl)
	FIND_RES_STYLE(m_vpStyleTreeCtrl)
	FIND_RES_STYLE(m_vpStyleTreeList)
	FIND_RES_STYLE(m_vpStyleRichEdit)
	FIND_RES_STYLE(m_vpStyleIMRichEdit)
	FIND_RES_STYLE(m_vpStyleHtmlCtrl)
	FIND_RES_STYLE(m_vpStyleDateTime)
	FIND_RES_STYLE(m_vpStyleWaveCtrl)
	FIND_RES_STYLE(m_vpStyleMenu)
	FIND_RES_STYLE(m_vpStyleMenuBar)
	FIND_RES_STYLE(m_vpStyleHeader)
	FIND_RES_STYLE(m_vpStyleHorScrollBar)
	FIND_RES_STYLE(m_vpStyleVerScrollBar)
	FIND_RES_STYLE(m_vpStyleSplitter)
	FIND_RES_STYLE(m_vpStylePanelStack)
	FIND_RES_STYLE(m_vpStyleWLLine)
	FIND_RES_STYLE(m_vpStyleWLPic)

	return (*ppCtrlProp != NULL) ? 0 : -1;
}

int CUIMgr::GetColorResource(LPCTSTR lpszResID, COLORREF *pcr)
{
	if (pcr == NULL)
	{
		return -1;
	}

	RESCOLORPROPERTIES *pRet = NULL;

	size_t nSize = m_vpColorRes.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		RESCOLORPROPERTIES *pResProp = m_vpColorRes[i];

		if (pResProp->m_strID == lpszResID)
		{
			pRet = pResProp;
			break;
		}
	}

	if (pRet == NULL)
	{
		return -1;
	}

	ASSERT(pRet->IsColorResource());

	*pcr = pRet->m_crColor;

	return 0;
}

int CUIMgr::GetResourceCount(RESOURCE_TYPE eResType)
{
	if (eResType == REST_FONT)
	{
		size_t nCount = m_vpResFont.size();
		return int(nCount);
	}
	else if (eResType == REST_COLOR)
	{
		size_t nCount = m_vpColorRes.size();
		return int(nCount);
	}

	ASSERT(FALSE);
	return -1;
}

int CUIMgr::SetDefaultFont(LPCTSTR lpszFont)
{
	m_strCurrentFont = lpszFont;

	return 0;
}

CString CUIMgr::GetDefaultFont()
{
	return (LPCTSTR)m_resDefaultFont.m_strFontID;
}

int CUIMgr::GetFontResource(const CString &strCondition, LPCTSTR lpszResID, LOGFONT *plf)
{
	if (plf == NULL)
	{
		return -1;
	}

	RESFONTPROPERTIES *pRet = NULL;

	size_t nSize = m_vpResFont.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		FONTRESOURCEITEM *pResFont = m_vpResFont[i];
		if (pResFont->m_bConditionFont)
		{
			RESFONTPROPERTIES *presFontProp = pResFont->m_mapConditionFont[strCondition];

			ASSERT((CONTROL_TYPE)presFontProp->m_eControlType == CT_RESOURCE_DEFAULT_FONT);

			if (presFontProp->m_strID == lpszResID)
			{
				pRet = presFontProp;
				break;
			}
		}
		else
		{
			if (pResFont->m_pFontResProp->m_strID == lpszResID)
			{
				pRet = pResFont->m_pFontResProp;
				break;
			}
		}
	}

	if (pRet == NULL)
	{
		return -1;
	}

	ASSERT(pRet->IsFontResource());

	*plf = pRet->m_logFont;

	return 0;
}

RESFONTPROPERTIES *CUIMgr::FindFontResource(const IUI_VARIANT &varFont)
{
	RESFONTPROPERTIES *pRetProp = NULL;

	size_t nSize = m_vpResFont.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		FONTRESOURCEITEM *pResFont = m_vpResFont[i];

		if (pResFont->m_bConditionFont)
		{
			std::map<CString, RESFONTPROPERTIES *>::iterator it = pResFont->m_mapConditionFont.begin();
			for (; it != pResFont->m_mapConditionFont.end(); ++it)
			{
				if (it->second->m_logFont == varFont)
				{
					pRetProp = it->second;
					break;
				}
			}

			if (pRetProp != NULL)
			{
				break;
			}
		}
		else
		{
			if (pResFont->m_pFontResProp->m_logFont != varFont)
			{
				pRetProp = pResFont->m_pFontResProp;
				break;
			}
		}
	}

	return pRetProp;
}

RESFONTPROPERTIES *CUIMgr::GetDefaultFontResource(const CString &strCondition)
{
	if (m_resDefaultFont.m_bConditionFont)
	{
		// If not specify the condition name, use first one.
		if (strCondition.IsEmpty())
		{
			return m_resDefaultFont.m_mapConditionFont.begin()->second;
		}
		else
		{
			return m_resDefaultFont.m_mapConditionFont[strCondition];
		}
	}
	else
	{
		return m_resDefaultFont.m_pFontResProp;
	}
}

FONTRESOURCEITEM *CUIMgr::GetDefaultFontResource()
{
	return &m_resDefaultFont;
}

int CUIMgr::AddFontResourceItem(FONTRESOURCEITEM *pFontResource)
{
	if (pFontResource == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	m_vpResFont.push_back(pFontResource);

	size_t nCount = m_vpResFont.size();
	return int(nCount - 1);
}

int CUIMgr::DeleteFontResourceItem(int nIndex)
{
	int nCount = (int)m_vpResFont.size();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	FONTRESOURCEITEM *pResFont = m_vpResFont[nIndex];

	if (pResFont->m_bConditionFont)
	{
		std::map<CString, RESFONTPROPERTIES *>::iterator it = pResFont->m_mapConditionFont.begin();
		for (; it != pResFont->m_mapConditionFont.end(); ++it)
		{
			delete it->second;
		}
	}
	else
	{
		delete pResFont->m_pFontResProp;
	}

	m_vpResFont.erase(m_vpResFont.begin() + nIndex);

	return 0;
}

int CUIMgr::DeleteFontResourceItem(const RESFONTPROPERTIES *pResProp)
{
	//
	// Find from default font
	//
	if (m_resDefaultFont.m_bConditionFont)
	{
		int nRet = m_resDefaultFont.DeleteConditionFontChild(pResProp);
		if (nRet == 0)
		{
			return 0;
		}
	}

	//
	// Find from font list
	//
	size_t nCount = m_vpResFont.size();

	for (size_t i = 0; i < nCount; ++i)
	{
		FONTRESOURCEITEM *p = m_vpResFont[i];

		if (p->m_bConditionFont)
		{
			int nRet = p->DeleteConditionFontChild(pResProp);
			if (nRet == 0)
			{
				break;
			}
		}
		else
		{
			if (p->m_pFontResProp == pResProp)
			{
				p->Release();
				delete p;
				m_vpResFont.erase(m_vpResFont.begin() + i);
				break;
			}
		}
	}

	return 0;
}

int CUIMgr::DeleteFontResourceItem(const FONTRESOURCEITEM *pResFont)
{
	//
	// Find from default font
	//
	if (&m_resDefaultFont == pResFont)	// The default font can't be deleted.
	{
		return -2;
	}

	//
	// Find from font list
	//
	int nIndex = -1;
	size_t nCount = m_vpResFont.size();

	for (size_t i = 0; i < nCount; ++i)
	{
		FONTRESOURCEITEM *p = m_vpResFont[i];

		if (p == pResFont)
		{
			p->Release();
			delete p;
			m_vpResFont.erase(m_vpResFont.begin() + i);

			nIndex = (int)i;
			break;
		}
	}

	return nIndex;
}

FONTRESOURCEITEM *CUIMgr::GetFontResourceItem(int nIndex)
{
	int nCount = (int)m_vpResFont.size();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return NULL;
	}

	return m_vpResFont[nIndex];
}

FONTRESOURCEITEM *CUIMgr::GetFontResourceItem(LPCTSTR lpszResID)
{
	size_t nCount = m_vpResFont.size();

	for (size_t i = 0; i < nCount; ++i)
	{
		FONTRESOURCEITEM *pRes = m_vpResFont[i];

		if (pRes->m_bConditionFont)
		{
			if (pRes->m_strFontID == lpszResID)
			{
				return pRes;
			}
		}
		else
		{
			if (pRes->m_pFontResProp->m_strID == lpszResID)
			{
				return pRes;
			}
		}
	}

	return NULL;
}

RESCOLORPROPERTIES *CUIMgr::FindColorResource(const IUI_VARIANT &varColor)
{
	RESCOLORPROPERTIES *pRetProp = NULL;

	size_t nSize = m_vpColorRes.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		RESCOLORPROPERTIES *pColorProp = m_vpColorRes[i];

		COLORREF crResource = (LONG)pColorProp->m_crColor;
		if (crResource != (COLORREF)(LONG)varColor)
		{
			continue;
		}

		pRetProp = pColorProp;
		break;
	}

	return pRetProp;
}

RESCOLORPROPERTIES *CUIMgr::GetDefaultColorResource()
{
	m_resDefaultColor.m_strID.bSaveToSkinFile = m_resDefaultColor.m_strID.bShowInPropertiesWnd = false;

	return &m_resDefaultColor;
}

int CUIMgr::AddColorResourceItem(CTRLPROPERTIES *pResource)
{
	if (pResource == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	m_vpColorRes.push_back((RESCOLORPROPERTIES *)pResource);

	size_t nCount = m_vpColorRes.size();
	return int(nCount - 1);
}

int CUIMgr::DeleteColorResourceItem(int nIndex)
{
	int nCount = (int)m_vpColorRes.size();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	RESCOLORPROPERTIES *pResProp = m_vpColorRes[nIndex];
	delete pResProp;

	m_vpColorRes.erase(m_vpColorRes.begin() + nIndex);

	return 0;
}

int CUIMgr::DeleteColorResourceItem(const RESCOLORPROPERTIES *pResProp)
{
	size_t nCount = m_vpColorRes.size();

	for (size_t i = 0; i < nCount; ++i)
	{
		RESCOLORPROPERTIES *p = m_vpColorRes[i];

		if (p == pResProp)
		{
			delete p;
			m_vpColorRes.erase(m_vpColorRes.begin() + i);
			break;
		}
	}

	return 0;
}

RESCOLORPROPERTIES *CUIMgr::GetColorResourceItem(int nIndex)
{
	int nCount = (int)m_vpColorRes.size();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return NULL;
	}

	return m_vpColorRes[nIndex];
}

RESCOLORPROPERTIES *CUIMgr::GetColorResourceItem(LPCTSTR lpszResID)
{
	size_t nCount = m_vpColorRes.size();

	for (size_t i = 0; i < nCount; ++i)
	{
		RESCOLORPROPERTIES *pRes = m_vpColorRes[i];
		if (pRes->m_strID == lpszResID)
		{
			return pRes;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// Style

#define BEGIN_GET_STYLE_COUNT(eStyleType, eStyle, vpStyle) \
	if (eStyleType == eStyle) \
	{ \
		size_t nCount = vpStyle.size(); \
		return int(nCount); \
	}

#define GET_STYLE_COUNT(eStyleType, eStyle, vpStyle) \
	else if (eStyleType == eStyle) \
	{ \
		size_t nCount = vpStyle.size(); \
		return int(nCount); \
	}

int CUIMgr::GetStyleCount(STYLE_TYPE eStyleType)
{
	BEGIN_GET_STYLE_COUNT(eStyleType, STYLET_PUSHBUTTON, m_vpStylePushButton)
	GET_STYLE_COUNT(eStyleType, STYLET_CHECK, m_vpStyleCheckBox)
	GET_STYLE_COUNT(eStyleType, STYLET_RADIO, m_vpStyleRadioButton)
	GET_STYLE_COUNT(eStyleType, STYLET_COMBOBOX, m_vpStyleComboBox)
	GET_STYLE_COUNT(eStyleType, STYLET_STATIC, m_vpStyleStatic)
	GET_STYLE_COUNT(eStyleType, STYLET_EDIT, m_vpStyleEdit)
	GET_STYLE_COUNT(eStyleType, STYLET_SLIDER, m_vpStyleSlider)
	GET_STYLE_COUNT(eStyleType, STYLET_PROGRESS, m_vpStyleProgress)
	GET_STYLE_COUNT(eStyleType, STYLET_SPIN, m_vpStyleSpin)
	GET_STYLE_COUNT(eStyleType, STYLET_HOTKEY, m_vpStyleHotKey)
	GET_STYLE_COUNT(eStyleType, STYLET_IPADDRESS, m_vpStyleIPAddress)
	GET_STYLE_COUNT(eStyleType, STYLET_SLIDEREX, m_vpStyleSliderEx)
	GET_STYLE_COUNT(eStyleType, STYLET_LISTCTRL, m_vpStyleListCtrl)
	GET_STYLE_COUNT(eStyleType, STYLET_TREECTRL, m_vpStyleTreeCtrl)
	GET_STYLE_COUNT(eStyleType, STYLET_TREELIST, m_vpStyleTreeList)
	GET_STYLE_COUNT(eStyleType, STYLET_RICHEDIT, m_vpStyleRichEdit)
	GET_STYLE_COUNT(eStyleType, STYLET_IMRICHEDIT, m_vpStyleIMRichEdit)
	GET_STYLE_COUNT(eStyleType, STYLET_HTMLCTRL, m_vpStyleHtmlCtrl)
	GET_STYLE_COUNT(eStyleType, STYLET_DATETIMECTRL, m_vpStyleDateTime)
	GET_STYLE_COUNT(eStyleType, STYLET_WAVECTRL, m_vpStyleWaveCtrl)
	GET_STYLE_COUNT(eStyleType, STYLET_MENU, m_vpStyleMenu)
	GET_STYLE_COUNT(eStyleType, STYLET_MENUBAR, m_vpStyleMenuBar)
	GET_STYLE_COUNT(eStyleType, STYLET_HEADERCTRL, m_vpStyleHeader)
	GET_STYLE_COUNT(eStyleType, STYLET_HORSCROLLBAR, m_vpStyleHorScrollBar)
	GET_STYLE_COUNT(eStyleType, STYLET_VERSCROLLBAR, m_vpStyleVerScrollBar)
	GET_STYLE_COUNT(eStyleType, STYLET_SPLITTER, m_vpStyleSplitter)
	GET_STYLE_COUNT(eStyleType, STYLET_PANEL_STACK, m_vpStylePanelStack)
	GET_STYLE_COUNT(eStyleType, STYLET_WL_LINE, m_vpStyleWLLine)
	GET_STYLE_COUNT(eStyleType, STYLET_WL_PICTURE, m_vpStyleWLPic)

	ASSERT(FALSE);
	return -1;
}

#define BEGIN_ADD_STYLE_ITEM(eStyleType, pStyle, eStyle, ctrlType, vpStyle, StylePropName) \
	if (eStyleType == eStyle) \
	{ \
		if ((CONTROL_TYPE)pStyle->m_eControlType != ctrlType) \
		{ \
			ASSERT(FALSE); \
			return -1; \
		} \
		vpStyle.push_back((StylePropName *)pStyle); \
		size_t nCount = vpStyle.size(); \
		return int(nCount - 1); \
	}

#define ADD_STYLE_ITEM(eStyleType, pStyle, eStyle, ctrlType, vpStyle, StylePropName) \
	else if (eStyleType == eStyle) \
	{ \
		if ((CONTROL_TYPE)pStyle->m_eControlType != ctrlType) \
		{ \
			ASSERT(FALSE); \
			return -1; \
		} \
		vpStyle.push_back((StylePropName *)pStyle); \
		size_t nCount = vpStyle.size(); \
		return int(nCount - 1); \
	}

int CUIMgr::AddStyleItem(STYLE_TYPE eStyleType, CTRLPROPERTIES *pStyle)
{
	BEGIN_ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_PUSHBUTTON, CT_STYLE_PUSHBUTTON, m_vpStylePushButton, BTNPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_CHECK, CT_STYLE_CHECK, m_vpStyleCheckBox, CHKPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_RADIO, CT_STYLE_RADIO, m_vpStyleRadioButton, RADPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_COMBOBOX, CT_STYLE_COMBOBOX, m_vpStyleComboBox, CMBPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_STATIC, CT_STYLE_STATIC, m_vpStyleStatic, TXTPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_EDIT, CT_STYLE_EDIT, m_vpStyleEdit, EDTPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_SLIDER, CT_STYLE_SLIDER, m_vpStyleSlider, SLDPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_PROGRESS, CT_STYLE_PROGRESS, m_vpStyleProgress, PRGPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_SPIN, CT_STYLE_SPIN, m_vpStyleSpin, SPNPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_HOTKEY, CT_STYLE_HOTKEY, m_vpStyleHotKey, HOTPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_IPADDRESS, CT_STYLE_IPADDRESS, m_vpStyleIPAddress, IPAPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_SLIDEREX, CT_STYLE_SLIDEREX, m_vpStyleSliderEx, SLDEXPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_LISTCTRL, CT_STYLE_LISTCTRL, m_vpStyleListCtrl, LSTCTRLPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_TREECTRL, CT_STYLE_TREECTRL, m_vpStyleTreeCtrl, TREECTRLPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_TREELIST, CT_STYLE_TREELIST, m_vpStyleTreeList, TREELISTPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_RICHEDIT, CT_STYLE_RICHEDIT, m_vpStyleRichEdit, RICHEDITPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_IMRICHEDIT, CT_STYLE_IMRICHEDIT, m_vpStyleIMRichEdit, IMREPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_HTMLCTRL, CT_STYLE_HTMLCTRL, m_vpStyleHtmlCtrl, HTMLCTRLPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_DATETIMECTRL, CT_STYLE_DATETIMECTRL, m_vpStyleDateTime, DATETIMECTRLPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_WAVECTRL, CT_STYLE_WAVECTRL, m_vpStyleWaveCtrl, WAVECTRLPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_MENU, CT_STYLE_MENU, m_vpStyleMenu, MENUPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_MENUBAR, CT_STYLE_MENUBAR, m_vpStyleMenuBar, MENUBARPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_HEADERCTRL, CT_STYLE_HEADERCTRL, m_vpStyleHeader, HEADERPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_HORSCROLLBAR, CT_STYLE_HORSCROLLBAR, m_vpStyleHorScrollBar, HORSBPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_VERSCROLLBAR, CT_STYLE_VERSCROLLBAR, m_vpStyleVerScrollBar, VERSBPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_SPLITTER, CT_STYLE_SPLITTER, m_vpStyleSplitter, SPLITTERPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_PANEL_STACK, CT_STYLE_PANEL_STACK, m_vpStylePanelStack, STACKPNLPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_WL_LINE, CT_STYLE_WL_LINE, m_vpStyleWLLine, LINEWLPROPERTIES)
	ADD_STYLE_ITEM(eStyleType, pStyle, STYLET_WL_PICTURE, CT_STYLE_WL_PICTURE, m_vpStyleWLPic, PICWLPROPERTIES)

	ASSERT(FALSE);
	return -1;
}

#define BEGIN_DELETE_STYLE_ITEM(eStyleType, nIndex, eStyle, vpStyle, StylePropName) \
	if (eStyleType == eStyle) \
	{ \
		int nCount = (int)vpStyle.size(); \
		if (nIndex < 0 || nIndex >= nCount) \
			return -1; \
		StylePropName *pStyleProp = vpStyle[nIndex]; \
		delete pStyleProp; \
		vpStyle.erase(vpStyle.begin() + nIndex); \
		return 0; \
	}
#define DELETE_STYLE_ITEM(eStyleType, nIndex, eStyle, vpStyle, StylePropName) \
	else if (eStyleType == eStyle) \
	{ \
		int nCount = (int)vpStyle.size(); \
		if (nIndex < 0 || nIndex >= nCount) \
			return -1; \
		StylePropName *pStyleProp = vpStyle[nIndex]; \
		delete pStyleProp; \
		vpStyle.erase(vpStyle.begin() + nIndex); \
		return 0; \
	}

int CUIMgr::DeleteStyleItem(STYLE_TYPE eStyleType, int nIndex)
{
	BEGIN_DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_PUSHBUTTON, m_vpStylePushButton, BTNPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_CHECK, m_vpStyleCheckBox, CHKPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_RADIO, m_vpStyleRadioButton, RADPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_COMBOBOX, m_vpStyleComboBox, CMBPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_STATIC, m_vpStyleStatic, TXTPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_EDIT, m_vpStyleEdit, EDTPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_SLIDER, m_vpStyleSlider, SLDPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_PROGRESS, m_vpStyleProgress, PRGPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_SPIN, m_vpStyleSpin, SPNPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_HOTKEY, m_vpStyleHotKey, HOTPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_IPADDRESS, m_vpStyleIPAddress, IPAPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_SLIDEREX, m_vpStyleSliderEx, SLDEXPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_LISTCTRL, m_vpStyleListCtrl, LSTCTRLPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_TREECTRL, m_vpStyleTreeCtrl, TREECTRLPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_TREELIST, m_vpStyleTreeList, TREELISTPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_RICHEDIT, m_vpStyleRichEdit, RICHEDITPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_IMRICHEDIT, m_vpStyleIMRichEdit, IMREPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_HTMLCTRL, m_vpStyleHtmlCtrl, HTMLCTRLPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_DATETIMECTRL, m_vpStyleDateTime, DATETIMECTRLPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_WAVECTRL, m_vpStyleWaveCtrl, WAVECTRLPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_MENU, m_vpStyleMenu, MENUPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_MENUBAR, m_vpStyleMenuBar, MENUBARPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_HEADERCTRL, m_vpStyleHeader, HEADERPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_HORSCROLLBAR, m_vpStyleHorScrollBar, HORSBPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_VERSCROLLBAR, m_vpStyleVerScrollBar, VERSBPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_SPLITTER, m_vpStyleSplitter, SPLITTERPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_PANEL_STACK, m_vpStylePanelStack, STACKPNLPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_WL_LINE, m_vpStyleWLLine, LINEWLPROPERTIES)
	DELETE_STYLE_ITEM(eStyleType, nIndex, STYLET_WL_PICTURE, m_vpStyleWLPic, PICWLPROPERTIES)

	ASSERT(FALSE);
	return -1;
}

#define BEGIN_DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, eStyle, vpStyle, StylePropName) \
	if (eStyleType == eStyle) \
	{ \
		size_t nCount = vpStyle.size(); \
		for (size_t i=0; i<nCount; ++i) \
		{ \
			StylePropName *p = vpStyle[i]; \
			if (p == pStyleProp) \
			{ \
				delete p; \
				vpStyle.erase(vpStyle.begin() + i); \
				break; \
			} \
		} \
	}

#define DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, eStyle, vpStyle, StylePropName) \
	else if (eStyleType == eStyle) \
	{ \
		size_t nCount = vpStyle.size(); \
		for (size_t i=0; i<nCount; ++i) \
		{ \
			StylePropName *p = vpStyle[i]; \
			if (p == pStyleProp) \
			{ \
				delete p; \
				vpStyle.erase(vpStyle.begin() + i); \
				break; \
			} \
		} \
	}

#define END_DELETE_STYLE_ITEM_PROP() \
	else \
	{ \
		ASSERT(FALSE); \
	}

int CUIMgr::DeleteStyleItem(STYLE_TYPE eStyleType, const CTRLPROPERTIES *pStyleProp)
{
	BEGIN_DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_PUSHBUTTON, m_vpStylePushButton, BTNPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_CHECK, m_vpStyleCheckBox, CHKPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_RADIO, m_vpStyleRadioButton, RADPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_COMBOBOX, m_vpStyleComboBox, CMBPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_STATIC, m_vpStyleStatic, TXTPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_EDIT, m_vpStyleEdit, EDTPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_SLIDER, m_vpStyleSlider, SLDPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_PROGRESS, m_vpStyleProgress, PRGPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_SPIN, m_vpStyleSpin, SPNPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_HOTKEY, m_vpStyleHotKey, HOTPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_IPADDRESS, m_vpStyleIPAddress, IPAPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_SLIDEREX, m_vpStyleSliderEx, SLDEXPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_LISTCTRL, m_vpStyleListCtrl, LSTCTRLPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_TREECTRL, m_vpStyleTreeCtrl, TREECTRLPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_TREELIST, m_vpStyleTreeList, TREELISTPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_RICHEDIT, m_vpStyleRichEdit, RICHEDITPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_IMRICHEDIT, m_vpStyleIMRichEdit, IMREPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_HTMLCTRL, m_vpStyleHtmlCtrl, HTMLCTRLPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_DATETIMECTRL, m_vpStyleDateTime, DATETIMECTRLPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_WAVECTRL, m_vpStyleWaveCtrl, WAVECTRLPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_MENU, m_vpStyleMenu, MENUPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_MENUBAR, m_vpStyleMenuBar, MENUBARPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_HEADERCTRL, m_vpStyleHeader, HEADERPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_HORSCROLLBAR, m_vpStyleHorScrollBar, HORSBPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_VERSCROLLBAR, m_vpStyleVerScrollBar, VERSBPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_SPLITTER, m_vpStyleSplitter, SPLITTERPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_PANEL_STACK, m_vpStylePanelStack, STACKPNLPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_WL_LINE, m_vpStyleWLLine, LINEWLPROPERTIES)
	DELETE_STYLE_ITEM_PROP(eStyleType, pStyleProp, STYLET_WL_PICTURE, m_vpStyleWLPic, PICWLPROPERTIES)
	END_DELETE_STYLE_ITEM_PROP()

	return 0;
}

#define BEGIN_GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, eStyle, vpStyle) \
	if (eStyleType == eStyle) \
	{ \
		int nCount = (int)vpStyle.size(); \
		if (nIndex < 0 || nIndex >= nCount) \
			return NULL; \
		return vpStyle[nIndex]; \
	}

#define GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, eStyle, vpStyle) \
	else if (eStyleType == eStyle) \
	{ \
		int nCount = (int)vpStyle.size(); \
		if (nIndex < 0 || nIndex >= nCount) \
			return NULL; \
		return vpStyle[nIndex]; \
	}

CTRLPROPERTIES *CUIMgr::GetStyleItem(STYLE_TYPE eStyleType, int nIndex)
{
	BEGIN_GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_PUSHBUTTON, m_vpStylePushButton)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_CHECK, m_vpStyleCheckBox)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_RADIO, m_vpStyleRadioButton)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_COMBOBOX, m_vpStyleComboBox)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_STATIC, m_vpStyleStatic)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_EDIT, m_vpStyleEdit)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_SLIDER, m_vpStyleSlider)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_PROGRESS, m_vpStyleProgress)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_SPIN, m_vpStyleSpin)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_HOTKEY, m_vpStyleHotKey)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_IPADDRESS, m_vpStyleIPAddress)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_SLIDEREX, m_vpStyleSliderEx)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_LISTCTRL, m_vpStyleListCtrl)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_TREECTRL, m_vpStyleTreeCtrl)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_TREELIST, m_vpStyleTreeList)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_RICHEDIT, m_vpStyleRichEdit)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_IMRICHEDIT, m_vpStyleIMRichEdit)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_HTMLCTRL, m_vpStyleHtmlCtrl)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_DATETIMECTRL, m_vpStyleDateTime)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_WAVECTRL, m_vpStyleWaveCtrl)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_MENU, m_vpStyleMenu)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_MENUBAR, m_vpStyleMenuBar)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_HEADERCTRL, m_vpStyleHeader)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_HORSCROLLBAR, m_vpStyleHorScrollBar)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_VERSCROLLBAR, m_vpStyleVerScrollBar)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_SPLITTER, m_vpStyleSplitter)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_PANEL_STACK, m_vpStylePanelStack)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_WL_LINE, m_vpStyleWLLine)
	GET_STYLE_ITEM_BY_INDEX(eStyleType, nIndex, STYLET_WL_PICTURE, m_vpStyleWLPic)

	ASSERT(FALSE);
	return NULL;
}

#define BEGIN_GET_STYLE_ITEM(eStyleType, lpszStyleID, eStyle, vpStyle) \
	if (eStyleType == eStyle) \
	{ \
		size_t nCount = vpStyle.size(); \
		for (size_t i=0; i<nCount; ++i) \
		{ \
			CTRLPROPERTIES *pStyleProp = vpStyle[i]; \
			if (pStyleProp->m_strID == lpszStyleID) \
			{ \
				return pStyleProp; \
			} \
		} \
	}

#define GET_STYLE_ITEM(eStyleType, lpszStyleID, eStyle, vpStyle) \
	else if (eStyleType == eStyle) \
	{ \
		size_t nCount = vpStyle.size(); \
		for (size_t i=0; i<nCount; ++i) \
		{ \
			CTRLPROPERTIES *pStyleProp = vpStyle[i]; \
			if (pStyleProp->m_strID == lpszStyleID) \
			{ \
				return pStyleProp; \
			} \
		} \
	}
#define END_GET_SYTLE_ITME() \
	else \
	{\
		ASSERT(FALSE); \
	}

CTRLPROPERTIES *CUIMgr::GetStyleItem(STYLE_TYPE eStyleType, LPCTSTR lpszStyleID)
{
	BEGIN_GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_PUSHBUTTON, m_vpStylePushButton)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_CHECK, m_vpStyleCheckBox)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_RADIO, m_vpStyleRadioButton)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_COMBOBOX, m_vpStyleComboBox)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_STATIC, m_vpStyleStatic)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_EDIT, m_vpStyleEdit)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_SLIDER, m_vpStyleSlider)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_PROGRESS, m_vpStyleProgress)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_SPIN, m_vpStyleSpin)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_HOTKEY, m_vpStyleHotKey)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_IPADDRESS, m_vpStyleIPAddress)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_SLIDEREX, m_vpStyleSliderEx)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_LISTCTRL, m_vpStyleListCtrl)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_TREECTRL, m_vpStyleTreeCtrl)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_TREELIST, m_vpStyleTreeList)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_RICHEDIT, m_vpStyleRichEdit)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_IMRICHEDIT, m_vpStyleIMRichEdit)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_HTMLCTRL, m_vpStyleHtmlCtrl)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_DATETIMECTRL, m_vpStyleDateTime)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_WAVECTRL, m_vpStyleWaveCtrl)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_MENU, m_vpStyleMenu)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_MENUBAR, m_vpStyleMenuBar)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_HEADERCTRL, m_vpStyleHeader)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_HORSCROLLBAR, m_vpStyleHorScrollBar)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_VERSCROLLBAR, m_vpStyleVerScrollBar)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_SPLITTER, m_vpStyleSplitter)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_PANEL_STACK, m_vpStylePanelStack)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_WL_LINE, m_vpStyleWLLine)
	GET_STYLE_ITEM(eStyleType, lpszStyleID, STYLET_WL_PICTURE, m_vpStyleWLPic)
	END_GET_SYTLE_ITME()

	return NULL;
}

std::vector<BTNPROPERTIES *> *CUIMgr::GetStylePushButton()
{
	return &m_vpStylePushButton;
}

std::vector<CHKPROPERTIES *> *CUIMgr::GetStyleCheckBox()
{
	return &m_vpStyleCheckBox;
}

std::vector<RADPROPERTIES *> *CUIMgr::GetStyleRadioButton()
{
	return &m_vpStyleRadioButton;
}

std::vector<CMBPROPERTIES *> *CUIMgr::GetStyleComboBox()
{
	return &m_vpStyleComboBox;
}

std::vector<TXTPROPERTIES *> *CUIMgr::GetStyleStatic()
{
	return &m_vpStyleStatic;
}

std::vector<EDTPROPERTIES *> *CUIMgr::GetStyleEdit()
{
	return &m_vpStyleEdit;
}

std::vector<SLDPROPERTIES *> *CUIMgr::GetStyleSlider()
{
	return &m_vpStyleSlider;
}

std::vector<PRGPROPERTIES *> *CUIMgr::GetStyleProgress()
{
	return &m_vpStyleProgress;
}

std::vector<SPNPROPERTIES *> *CUIMgr::GetStyleSpin()
{
	return &m_vpStyleSpin;
}

std::vector<HOTPROPERTIES *> *CUIMgr::GetStyleHotKey()
{
	return &m_vpStyleHotKey;
}

std::vector<IPAPROPERTIES *> *CUIMgr::GetStyleIPAddress()
{
	return &m_vpStyleIPAddress;
}

std::vector<SLDEXPROPERTIES *> *CUIMgr::GetStyleSliderEx()
{
	return &m_vpStyleSliderEx;
}

std::vector<LSTCTRLPROPERTIES *> *CUIMgr::GetStyleListCtrl()
{
	return &m_vpStyleListCtrl;
}

std::vector<TREECTRLPROPERTIES *> *CUIMgr::GetStyleTreeCtrl()
{
	return &m_vpStyleTreeCtrl;
}

std::vector<TREELISTPROPERTIES *> *CUIMgr::GetStyleTreeList()
{
	return &m_vpStyleTreeList;
}

std::vector<RICHEDITPROPERTIES *> *CUIMgr::GetStyleRichEdit()
{
	return &m_vpStyleRichEdit;
}

std::vector<IMREPROPERTIES *> *CUIMgr::GetStyleIMRichEdit()
{
	return &m_vpStyleIMRichEdit;
}

std::vector<HTMLCTRLPROPERTIES *> *CUIMgr::GetStyleHtmlCtrl()
{
	return &m_vpStyleHtmlCtrl;
}

std::vector<DATETIMECTRLPROPERTIES *> *CUIMgr::GetStyleDateTime()
{
	return &m_vpStyleDateTime;
}

std::vector<WAVECTRLPROPERTIES *> *CUIMgr::GetStyleWaveCtrl()
{
	return &m_vpStyleWaveCtrl;
}

std::vector<MENUPROPERTIES *> *CUIMgr::GetStyleMenu()
{
	return &m_vpStyleMenu;
}

std::vector<MENUBARPROPERTIES *> *CUIMgr::GetStyleMenuBar()
{
	return &m_vpStyleMenuBar;
}

std::vector<HEADERPROPERTIES *> *CUIMgr::GetStyleHeader()
{
	return &m_vpStyleHeader;
}

std::vector<HORSBPROPERTIES *> *CUIMgr::GetStyleHorScrollBar()
{
	return &m_vpStyleHorScrollBar;
}

std::vector<VERSBPROPERTIES *> *CUIMgr::GetStyleVerScrollBar()
{
	return &m_vpStyleVerScrollBar;
}

std::vector<SPLITTERPROPERTIES *> *CUIMgr::GetStyleSplitter()
{
	return &m_vpStyleSplitter;
}

std::vector<STACKPNLPROPERTIES *> *CUIMgr::GetStylePanelStack()
{
	return &m_vpStylePanelStack;
}

std::vector<LINEWLPROPERTIES *> *CUIMgr::GetStyleWLLine()
{
	return &m_vpStyleWLLine;
}

std::vector<PICWLPROPERTIES *> *CUIMgr::GetStyleWLPic()
{
	return &m_vpStyleWLPic;
}

//////////////////////////////////////////////////////////////////////////

CTRLPROPERTIES *CUIMgr::GetCONTROL(LPCTSTR lpszWindowID, LPCTSTR lpszCtrlID)
{
	UIWNDPROPERTIES *pWnd = GetUIWNDPROPERTIES(lpszWindowID);

	size_t nSize = pWnd->m_vControls.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		if (pWnd->m_vControls[i]->m_strID == lpszCtrlID)
		{
			return pWnd->m_vControls[i];
		}
	}

	return NULL;
}

BOOL IsUseImage(const CTRLPROPERTIES *pCtrlProp, LPCTSTR lpszImage)
{
	BOOL bRet = FALSE;

	// Check each properties.
	LONG lVarCount = pCtrlProp->GetPropertyCount();
	for (LONG i = 0; i < lVarCount ; i++)
	{
		const IUI_VARIANT *pVariant = pCtrlProp->GetProperty(i);

		if (pVariant->vtEx != VT_EX_BITMAPPATH)
		{
			continue;
		}

		CString strImage = (LPCTSTR)(*pVariant);
		if (strImage.CompareNoCase(lpszImage) == 0)
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

std::vector<CString> GetUseImage(const CTRLPROPERTIES *pCtrlProp)
{
	std::vector<CString> vImages;

	// Check each properties.
	LONG lVarCount = pCtrlProp->GetPropertyCount();
	for (LONG i = 0; i < lVarCount ; i++)
	{
		const IUI_VARIANT *pVariant = pCtrlProp->GetProperty(i);

		if (pVariant->vtEx != VT_EX_BITMAPPATH)
		{
			continue;
		}

		CString strImage = (LPCTSTR)(*pVariant);
		if (strImage.IsEmpty())
		{
			continue;
		}

		vImages.push_back(strImage);
	}

	return vImages;
}


#define COMPARE_STYLE(vpStyle) \
	if (pCtrlRet != NULL) \
		return pCtrlRet; \
	nStyleCount = vpStyle.size(); \
	for (i=0; i<nStyleCount; ++i) \
	{ \
		CTRLPROPERTIES *pCtrlProp = vpStyle[i]; \
		BOOL bRet = IsUseImage(pCtrlProp, lpszImage); \
		if (bRet) \
		{ \
			pCtrlRet = pCtrlProp; \
			break; \
		} \
	}

CTRLPROPERTIES *CUIMgr::GetCONTROL(LPCTSTR lpszImage)
{
	CTRLPROPERTIES *pCtrlRet = NULL;

	// Find in window list
	size_t nWindowsCount = m_vpWindows.size();
	size_t i = 0;
	for (i = 0; i < nWindowsCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		BOOL bRet = IsUseImage(pUIProp, lpszImage);
		if (bRet)
		{
			pCtrlRet = pUIProp;
			break;
		}

		// Find children
		size_t nChildrenCount = pUIProp->m_vControls.size();
		for (size_t j = 0; j < nChildrenCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];

			bRet = IsUseImage(pCtrlProp, lpszImage);
			if (bRet)
			{
				pCtrlRet = pCtrlProp;
				break;
			}
		}

		// Find control within children.
		if (bRet)
		{
			break;
		}
	}

	size_t nStyleCount = 0;

	// Find in style list
	COMPARE_STYLE(m_vpStylePushButton)
	COMPARE_STYLE(m_vpStyleCheckBox)
	COMPARE_STYLE(m_vpStyleRadioButton)
	COMPARE_STYLE(m_vpStyleComboBox)
	COMPARE_STYLE(m_vpStyleStatic)
	COMPARE_STYLE(m_vpStyleEdit)
	COMPARE_STYLE(m_vpStyleSlider)
	COMPARE_STYLE(m_vpStyleProgress)
	COMPARE_STYLE(m_vpStyleSpin)
	COMPARE_STYLE(m_vpStyleHotKey)
	COMPARE_STYLE(m_vpStyleIPAddress)
	COMPARE_STYLE(m_vpStyleSliderEx)
	COMPARE_STYLE(m_vpStyleListCtrl)
	COMPARE_STYLE(m_vpStyleTreeCtrl)
	COMPARE_STYLE(m_vpStyleTreeList)
	COMPARE_STYLE(m_vpStyleRichEdit)
	COMPARE_STYLE(m_vpStyleIMRichEdit)
	COMPARE_STYLE(m_vpStyleHtmlCtrl)
	COMPARE_STYLE(m_vpStyleDateTime)
	COMPARE_STYLE(m_vpStyleWaveCtrl)
	COMPARE_STYLE(m_vpStyleMenu)
	COMPARE_STYLE(m_vpStyleMenuBar)
	COMPARE_STYLE(m_vpStyleHeader)
	COMPARE_STYLE(m_vpStyleHorScrollBar)
	COMPARE_STYLE(m_vpStyleVerScrollBar)
	COMPARE_STYLE(m_vpStyleSplitter)
	COMPARE_STYLE(m_vpStyleWLLine)
	COMPARE_STYLE(m_vpStylePanelStack)
	COMPARE_STYLE(m_vpStyleWLPic)

	return pCtrlRet;
}

#define GET_STYLE_IMAGE(vpStyle) \
	nStyleCount = vpStyle.size(); \
	for (i=0; i<nStyleCount; ++i) \
	{ \
		CTRLPROPERTIES *pCtrlProp = vpStyle[i]; \
		std::vector<CString> vStylePropImages = GetUseImage(pCtrlProp); \
		if (vStylePropImages.size() > 0) \
		{ \
			vImageName.insert(vImageName.end(), vStylePropImages.begin(), vStylePropImages.end());\
		} \
	}

std::vector<CString> CUIMgr::GetAllImages(BOOL bAllowDuplicate)
{
	std::vector<CString> vImageName;

	// Get in window list
	size_t nWindowsCount = m_vpWindows.size();
	size_t i = 0;
	for (i = 0; i < nWindowsCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		std::vector<CString> vUIPropImages = GetUseImage(pUIProp);
		if (vUIPropImages.size() > 0)
		{
			vImageName.insert(vImageName.end(), vUIPropImages.begin(), vUIPropImages.end());
		}

		// Get children image
		size_t nChildrenCount = pUIProp->m_vControls.size();
		for (size_t j = 0; j < nChildrenCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];

			std::vector<CString> vCtrlPropImages = GetUseImage(pCtrlProp);
			if (vCtrlPropImages.size() > 0)
			{
				vImageName.insert(vImageName.end(), vCtrlPropImages.begin(), vCtrlPropImages.end());
			}
		}
	}

	size_t nStyleCount = 0;

	// Get in style list
	GET_STYLE_IMAGE(m_vpStylePushButton)
	GET_STYLE_IMAGE(m_vpStyleCheckBox)
	GET_STYLE_IMAGE(m_vpStyleRadioButton)
	GET_STYLE_IMAGE(m_vpStyleComboBox)
	GET_STYLE_IMAGE(m_vpStyleStatic)
	GET_STYLE_IMAGE(m_vpStyleEdit)
	GET_STYLE_IMAGE(m_vpStyleSlider)
	GET_STYLE_IMAGE(m_vpStyleProgress)
	GET_STYLE_IMAGE(m_vpStyleSpin)
	GET_STYLE_IMAGE(m_vpStyleHotKey)
	GET_STYLE_IMAGE(m_vpStyleIPAddress)
	GET_STYLE_IMAGE(m_vpStyleSliderEx)
	GET_STYLE_IMAGE(m_vpStyleListCtrl)
	GET_STYLE_IMAGE(m_vpStyleTreeCtrl)
	GET_STYLE_IMAGE(m_vpStyleTreeList)
	GET_STYLE_IMAGE(m_vpStyleRichEdit)
	GET_STYLE_IMAGE(m_vpStyleIMRichEdit)
	GET_STYLE_IMAGE(m_vpStyleHtmlCtrl)
	GET_STYLE_IMAGE(m_vpStyleDateTime)
	GET_STYLE_IMAGE(m_vpStyleWaveCtrl)
	GET_STYLE_IMAGE(m_vpStyleMenu)
	GET_STYLE_IMAGE(m_vpStyleMenuBar)
	GET_STYLE_IMAGE(m_vpStyleHeader)
	GET_STYLE_IMAGE(m_vpStyleHorScrollBar)
	GET_STYLE_IMAGE(m_vpStyleVerScrollBar)
	GET_STYLE_IMAGE(m_vpStyleSplitter)
	GET_STYLE_IMAGE(m_vpStylePanelStack)
	GET_STYLE_IMAGE(m_vpStyleWLLine)
	GET_STYLE_IMAGE(m_vpStyleWLPic)

	// Delete duplicate elements.
	if (!bAllowDuplicate)
	{
		std::vector<CString> vImageUnique;

		size_t nImageCount = vImageName.size();
		for (size_t i = 0; i < nImageCount; ++i)
		{
			CString strImage = vImageName[i];

			BOOL bFind = IsStringExist(&vImageUnique, strImage, TRUE);
			if (bFind)
			{
				continue;
			}

			vImageUnique.push_back(strImage);
		}

		// Clear vImageName
		vImageName.clear();

		// Copy vImageUnique to vImageName
		vImageName = vImageUnique;
	}

	return vImageName;
}

LONG CUIMgr::GetXUIMajorVersion()
{
	return m_lXUIMajorVersion;
}

LONG CUIMgr::GetXUIMinorVersion()
{
	return m_lXUIMinorVersion;
}

LONG CUIMgr::GetXUIBuildVersion()
{
	return m_lXUIBuildVersion;
}

LONG CUIMgr::GetXUIChangeList()
{
	return m_lXUIChangeList;
}

LONG CUIMgr::GetLibUIDKMajorVersion()
{
	return m_lLibUIDKMajorVersion;
}

LONG CUIMgr::GetLibUIDKMinorVersion()
{
	return m_lLibUIDKMinorVersion;
}

LONG CUIMgr::GetLibUIDKBuildVersion()
{
	return m_lLibUIDKBuildVersion;
}

LONG CUIMgr::GetLibUIDKChangeList()
{
	return _LIBUIDK_NEEDED_CHANGELIST;
}

LONG CUIMgr::GetLogPixelsX()
{
	return m_lLogPixelsX;
}

LONG CUIMgr::GetLogPixelsY()
{
	return m_lLogPixelsY;
}

int CUIMgr::EnableDPI(BOOL bEnable)
{
	m_bEnableDPI = bEnable;
	return 0;
}

BOOL CUIMgr::IsEnableDPI()
{
	return m_bEnableDPI;
}

int CUIMgr::RunAsDPIX(int nXDPI)
{
	m_bEnableDPI = TRUE;
	m_bRunAsDPIX = TRUE;
	m_nRunAsDPIX = nXDPI;

	return 0;
}

BOOL CUIMgr::IsRunAsDPIX()
{
	return m_bRunAsDPIX;
}

int CUIMgr::RunAsDPIY(int nYDPI)
{
	m_bEnableDPI = TRUE;
	m_bRunAsDPIY = TRUE;
	m_nRunAsDPIY = nYDPI;

	return 0;
}

BOOL CUIMgr::IsRunAsDPIY()
{
	return m_bRunAsDPIY;
}

int CUIMgr::GetRunAsDPIX(int *pnDPIX)
{
	if (pnDPIX == NULL)
	{
		return -1;
	}

	if (!m_bRunAsDPIX)
	{
		return -2;
	}

	*pnDPIX = m_nRunAsDPIX;

	return 0;
}

int CUIMgr::GetRunAsDPIY(int *pnDPIY)
{
	if (pnDPIY == NULL)
	{
		return -1;
	}

	if (!m_bRunAsDPIY)
	{
		return -2;
	}

	*pnDPIY = m_nRunAsDPIY;

	return 0;
}

#define RELEASE_RESOURCE_STYLE(vpResourceStyle, StructResStyleName) \
	nCount = vpResourceStyle.size(); \
	for (i=0; i<nCount; ++i) \
	{ \
		StructResStyleName *pResStyleProp = vpResourceStyle[i]; \
		delete pResStyleProp; \
	} \
	vpResourceStyle.clear();

int CUIMgr::ReleaseSkin()
{
	// Init LibUIDK_g_iui_thread_state
	_IUI_THREAD_STATE *pIUIState = IUIGetThreadState();
	pIUIState->m_hTrackingMenu = NULL;
	pIUIState->m_hTrackingWindow = NULL;
	pIUIState->m_hWndMenu = NULL;
	pIUIState->m_pWLCapture = NULL;
	pIUIState->m_pWLMouseIn = NULL;

	// Don't release ole at here. if you do, you will:
	// 1. Always crash while close the UI editor that opened a skin.
	// 2. Can't update recent file list.
	// Release ole.
	// AfxOleTerm();

	BYTE *pBuf = m_fileResourceH.Detach();
	delete []pBuf;

	m_strUIPath.Empty();
	m_strUIPathWithoutTitle.Empty();
	m_strUIResourcehID.Empty();
	m_strUIResourcePath.Empty();

	// Release windows.
	size_t i = 0;
	size_t nWndCount = m_vpWindows.size();
	for (i = 0; i < nWndCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = m_vpWindows[i];

		// controls
		size_t nChildCount = pUIProp->m_vControls.size();
		for (size_t j = 0; j < nChildCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];
			delete pCtrlProp;
		}
		pUIProp->m_vControls.clear();

		// storyboard
		size_t nStoryItemCount = pUIProp->m_vStoryFrame.size();
		for (size_t k = 0; k < nStoryItemCount; ++k)
		{
			STORY_FRAME *pItem = pUIProp->m_vStoryFrame[k];

			size_t nCtrlCount = pItem->m_vStoryFrameCtrlItem.size();
			for (size_t m = 0; m < nCtrlCount; ++m)
			{
				CTRLPROPERTIES *pCtrlProp = pItem->m_vStoryFrameCtrlItem[m];
				delete pCtrlProp;
			}

			pItem->m_vStoryFrameCtrlItem.clear();

			delete pItem;
		}
		pUIProp->m_vStoryFrame.clear();

		delete pUIProp;
	}

	m_vpWindows.clear();

	size_t nCount = 0;

	// Release font resources.
	ReleaseIUIFont(m_resDefaultFont.m_strFontID);

	m_resDefaultFont.Release();
	nCount = m_vpResFont.size();
	for (i = 0; i < nCount; ++i)
	{
		m_vpResFont[i]->Release();
		delete m_vpResFont[i];
	}
	m_vpResFont.clear();

	// Release color resources.
	RELEASE_RESOURCE_STYLE(m_vpColorRes, RESCOLORPROPERTIES)

	// Release style.
	RELEASE_RESOURCE_STYLE(m_vpStylePushButton, BTNPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleCheckBox, CHKPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleRadioButton, RADPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleComboBox, CMBPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleStatic, TXTPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleEdit, EDTPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleSlider, SLDPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleProgress, PRGPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleSpin, SPNPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleHotKey, HOTPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleIPAddress, IPAPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleSliderEx, SLDEXPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleListCtrl, LSTCTRLPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleTreeCtrl, TREECTRLPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleTreeList, TREELISTPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleRichEdit, RICHEDITPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleIMRichEdit, IMREPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleHtmlCtrl, HTMLCTRLPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleDateTime, DATETIMECTRLPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleWaveCtrl, WAVECTRLPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleMenu, MENUPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleMenuBar, MENUBARPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleHeader, HEADERPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleHorScrollBar, HORSBPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleVerScrollBar, VERSBPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleSplitter, SPLITTERPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStylePanelStack, STACKPNLPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleWLLine, LINEWLPROPERTIES)
	RELEASE_RESOURCE_STYLE(m_vpStyleWLPic, PICWLPROPERTIES)

	// If the *.xui is convert by *.ui, Release the COldUIMgr
	COldUIMgr::ReleaseMemoryFile();

	// Release GDI+
	if (m_gdiplusToken)
	{
		GdiplusShutdown(m_gdiplusToken);
	}
	m_gdiplusToken = NULL;

	return 0;
}

int CUIMgr::SaveSkin(std::vector<UIWNDPROPERTIES *> vpWindows, LPCTSTR lpszTarget)
{
	CString strTemp = lpszTarget;
	strTemp += '~';

	// save *.xui project file.
	HRESULT hr = S_OK;

	XML::IXMLDOMDocumentPtr xmlDoc(__uuidof(DOMDocument));

	// save xml header
	XML::IXMLDOMElementPtr pRootElement = SaveConfig(xmlDoc);

	// save all windows
	SaveAllWindows(xmlDoc, pRootElement, &vpWindows);

	// Format xml code
	WellFormXml(xmlDoc, pRootElement, 1);
	xmlDoc->save(_variant_t(strTemp.GetBuffer(0)));

	AddBOM(strTemp, lpszTarget);

	return 0;
}


int CUIMgr::CreateIniLanguageFile(LPCTSTR lpszImageName)
{
	// Create the language file alway as unicode.
	FILE *fp = NULL;
	errno_t e = FOPENW(&fp, (LPCWSTR)_bstr_t(lpszImageName), L"wb");
	if (e != 0 || fp == NULL)
	{
		AfxMessageBox(_T("Failed to create language file!"));
		return -1;
	}

	// Warning
	BYTE btUnicode[2];
	btUnicode[0] = 0xFF;
	btUnicode[1] = 0xFE;
	fwrite(btUnicode, sizeof(btUnicode), 1, fp);
	fputws(L"// LibUIDK Language file\r\n", fp);
	fputws(L"[Version]\r\n", fp);
	fputws(L"value=1000\r\n", fp);

	size_t nSize = m_vpWindows.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		UIWNDPROPERTIES *pUIWndProp = m_vpWindows[i];
		ASSERT(pUIWndProp != NULL);

		// [窗口ID]
		fputws(L"\r\n[", fp);
		fputws((LPCWSTR)pUIWndProp->m_strID, fp);
		fputws(L"]\r\n", fp);

		if (!bool(pUIWndProp->m_bLoaded))
		{
			CUIMgr::DelayLoadWindow(pUIWndProp);
		}
		// Loop controls
		size_t nCtrlCount = pUIWndProp->m_vControls.size();
		for (size_t j = 0; j < nCtrlCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIWndProp->m_vControls[j];

			CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
			ASSERT(eControlType != CT_ARROW);

			CString strControl;

			// Push button
			if (eControlType == CT_PUSHBUTTON || eControlType == CT_WL_BUTTON)
			{
				LPCTSTR pszCaption = ((BTNPROPERTIES *)pCtrlProp)->m_strCaption;
				strControl.Format(_T("%s=%s\r\n"), (LPCTSTR)pCtrlProp->m_strID, pszCaption);
			}
			// Check box
			else if (eControlType == CT_CHECK || eControlType == CT_WL_CHECK)
			{
				LPCTSTR pszCaption = ((CHKPROPERTIES *)pCtrlProp)->m_strCaption;
				strControl.Format(_T("%s=%s\r\n"), (LPCTSTR)pCtrlProp->m_strID, pszCaption);
			}
			// Radio button
			else if (eControlType == CT_RADIO || eControlType == CT_WL_RADIO)
			{
				LPCTSTR pszCaption = ((RADPROPERTIES *)pCtrlProp)->m_strCaption;
				strControl.Format(_T("%s=%s\r\n"), (LPCTSTR)pCtrlProp->m_strID, pszCaption);
			}
			// Static
			else if (eControlType == CT_STATIC || eControlType == CT_WL_TEXT)
			{
				LPCTSTR pszCaption = ((TXTPROPERTIES *)pCtrlProp)->m_strCaption;
				strControl.Format(_T("%s=%s\r\n"), (LPCTSTR)pCtrlProp->m_strID, pszCaption);
			}

			if (!strControl.IsEmpty())
			{
				fputws((LPCWSTR)(_bstr_t)strControl.GetBuffer(0), fp);
			}
		}
	}

	fclose(fp);

	return 0;
}

int LoadIniSection(CMemStdioFile &fileLanguage, LPCTSTR lpszSection, std::map<CString, CString> &mapLanguage)
{
	if (_tcslen(lpszSection) == 0)
	{
		return -1;
	}

	do
	{
#if(_MSC_VER <= 1200) // VC6.0
		DWORD nOldPos = fileLanguage.GetPosition();
#else
		ULONGLONG nOldPos = fileLanguage.GetPosition();
#endif

		CString strBuf;
		BOOL bRet = fileLanguage.ReadString(strBuf);
		if (!bRet)
		{
			break;
		}

		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.IsEmpty())
		{
			// skip space line
			continue;
		}

		if (strBuf.Left(1) == '[')
		{
			fileLanguage.Seek(nOldPos, CFile::begin);
			break;
		}

		CString strKey;
		AfxExtractSubString(strKey, strBuf, 0, '=');
		strKey.TrimRight();

		CString strValue;
		AfxExtractSubString(strValue, strBuf, 1, '=');
		strValue.TrimLeft();
		strValue.TrimRight();

		if (strKey.IsEmpty())
		{
			continue;
		}

		CString strMapKey = lpszSection;
		strMapKey += '/';
		strMapKey += strKey;

		mapLanguage[strMapKey] = strValue;
	}
	while (true);

	return 0;
}

int CUIMgr::LoadIniLanguageFile(LPCTSTR lpszLanguageFile)
{
	// Parse language file
	CMemStdioFile fileLanguage;

	FILE *fp = NULL;
	errno_t e = FOPENW(&fp, (LPCWSTR)(_bstr_t)lpszLanguageFile, L"rb");
	if (e != 0 || fp == NULL)
	{
		AfxMessageBox(_T("Failed to open language file!"));
		return -1;
	}

	m_mapLanguage.clear();

	fseek(fp, 0, SEEK_END);
	int nLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	BYTE *pBuf = new BYTE[nLen];
	memset(pBuf, 0, nLen);
	fread(pBuf, nLen, 1, fp);
	fclose(fp);
	fileLanguage.Attach(pBuf, nLen, 0);

	CString strBuf;
	while (fileLanguage.ReadString(strBuf))
	{
		strBuf.TrimLeft();
		strBuf.TrimRight();

		if (strBuf.IsEmpty())
		{
			continue;
		}

		if (strBuf.Left(1) != _T("["))
		{
			continue;
		}

		CString strSection = strBuf;
		strSection.Delete(0, 1);
		strSection.TrimRight(']');

		if (strSection == _T("Version"))
		{
			// Version
			continue;
		}

		LoadIniSection(fileLanguage, strSection, m_mapLanguage);
	}

	pBuf = fileLanguage.Detach();
	delete [] pBuf;

	return 0;
}

int CUIMgr::SwitchIniLanguage(LPCTSTR lpszLanguageFile)
{
	// Parse language file
	int nRet = LoadIniLanguageFile(lpszLanguageFile);
	if (nRet != 0)
	{
		return nRet;
	}

	// Update ui.
	size_t nSize = m_vpWindows.size();
	for (size_t i = 0; i < nSize; ++i)
	{
		UIWNDPROPERTIES *pUIWndProp = m_vpWindows[i];
		ASSERT(pUIWndProp != NULL);

		if (!bool(pUIWndProp->m_bLoaded))
		{
			CUIMgr::DelayLoadWindow(pUIWndProp);
		}

		size_t nCtrlCount = pUIWndProp->m_vControls.size();
		for (size_t j = 0; j < nCtrlCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIWndProp->m_vControls[j];

			CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
			ASSERT(eControlType != CT_ARROW);

			// The following control type need to set window text

			// Generate map key
			CString strMapKey = (LPCTSTR)pUIWndProp->m_strID;
			strMapKey += '/';
			strMapKey += (LPCTSTR)pCtrlProp->m_strID;

			if (eControlType != CT_PUSHBUTTON
				&&  eControlType != CT_CHECK
				&& eControlType != CT_RADIO
				&& eControlType != CT_STATIC
				&& eControlType != CT_WL_TEXT
				&& eControlType != CT_WL_BUTTON
				&& eControlType != CT_WL_CHECK
				&& eControlType != CT_WL_RADIO)
			{
				continue;
			}

			// Update control properties
			CString strCaption = m_mapLanguage[strMapKey];
			// Push button
			if (eControlType == CT_PUSHBUTTON || eControlType == CT_WL_BUTTON)
			{
				((BTNPROPERTIES *)pCtrlProp)->m_strCaption = strCaption;

			}
			// Check box
			else if (eControlType == CT_CHECK || eControlType == CT_WL_CHECK)
			{
				((CHKPROPERTIES *)pCtrlProp)->m_strCaption = strCaption;
			}
			// Radio button
			else if (eControlType == CT_RADIO || eControlType == CT_WL_RADIO)
			{
				((RADPROPERTIES *)pCtrlProp)->m_strCaption = strCaption;
			}
			// Static
			else if (eControlType == CT_STATIC || eControlType == CT_WL_TEXT)
			{
				((TXTPROPERTIES *)pCtrlProp)->m_strCaption = strCaption;
			}
		}
	}

	return 0;
}

int CUIMgr::SwitchIniLanguageOnRunTime(LPCTSTR lpszLanguageFile)
{
	int nRet = SwitchIniLanguage(lpszLanguageFile);
	if (nRet != 0)
	{
		return nRet;
	}

	// Get all running CUIWnd to m_mapCreatedHWndStringID
	InitRunningUIWnd();

	// Change language for all running CUIWnd
	std::map<HWND, CString>::iterator it = m_mapCreatedHWndStringID.begin();
	for (; it != m_mapCreatedHWndStringID.end(); it++)
	{
		HWND hUIWnd = it->first;
		CString strUIWndID = it->second;

		if (hUIWnd == NULL || strUIWndID.IsEmpty())
		{
			ASSERT(FALSE);
			continue;
		}

		UIWNDPROPERTIES *pUIWndProp = GetUIWNDPROPERTIES(strUIWndID);
		if (pUIWndProp == NULL)
		{
			ASSERT(FALSE);
			continue;
		}

		size_t nCtrlCount = pUIWndProp->m_vControls.size();
		for (size_t j = 0; j < nCtrlCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIWndProp->m_vControls[j];

			CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
			ASSERT(eControlType != CT_ARROW);

			// The following control type need to set window text

			// Generate map key
			CString strMapKey = (LPCTSTR)pUIWndProp->m_strID;
			strMapKey += '/';
			strMapKey += (LPCTSTR)pCtrlProp->m_strID;

			if (eControlType != CT_PUSHBUTTON
				&&  eControlType != CT_CHECK
				&& eControlType != CT_RADIO
				&& eControlType != CT_STATIC
				&& eControlType != CT_WL_TEXT
				&& eControlType != CT_WL_BUTTON
				&& eControlType != CT_WL_CHECK
				&& eControlType != CT_WL_RADIO)
			{
				continue;
			}

			// Update control properties
			CString strCaption = m_mapLanguage[strMapKey];
			int nID = LONG(pCtrlProp->m_nID);
			ASSERT(nID != -1);

			if (eControlType == CT_WL_TEXT
				|| eControlType == CT_WL_BUTTON
				|| eControlType == CT_WL_CHECK
				|| eControlType == CT_WL_RADIO)
			{
				SetDlgWLItemText(hUIWnd, nID, strCaption);
			}
			else
			{
				SetDlgItemText(hUIWnd, nID, strCaption);
			}
		}
	}

	return 0;
}

RECT CUIMgr::MAKERECT(LPCTSTR lpszRect)
{
	RECT rect = { 0, 0, 0, 0 };

	CString str;
	if (!AfxExtractSubString(str, lpszRect, 0, ','))
	{
		return rect;
	}
	rect.left = _ttoi(str.GetBuffer(str.GetLength()));

	if (!AfxExtractSubString(str, lpszRect, 1, ','))
	{
		return rect;
	}
	rect.top = _ttoi(str);

	if (!AfxExtractSubString(str, lpszRect, 2, ','))
	{
		return rect;
	}
	rect.right = _ttoi(str);

	if (!AfxExtractSubString(str, lpszRect, 3, ','))
	{
		return rect;
	}
	rect.bottom = _ttoi(str);

	return rect;
}

int CUIMgr::StringIDToInt(LPCTSTR lpszIDs)
{
	return m_mapStrIntIDs[lpszIDs];
}

CString CUIMgr::StringIDFromInt(int nID)
{
	return m_mapIntStrIDs[nID];
}

int CUIMgr::GetMaxIDValue()
{
	size_t nCount = m_vIDs.size();
	if (nCount == 0)
	{
		return -1;
	}

	return m_vIDs[nCount - 1].second;
}

int CUIMgr::SetAlphaBlendColor(COLORREF crAlphaBlend)
{
	m_crAlphaBlend = crAlphaBlend;

	return 0;
}

COLORREF CUIMgr::GetAlphaBlendColor()
{
	return m_crAlphaBlend;
}

//////////////////////////////////////////////////////////////////////////
// UI
CUIWnd *CUIMgr::LoadFrame(UINT nResID, DWORD dwStyle, UINT nWinID, CUIWndHandler *pHandler)
{
	CUIWnd *pFrame = new CUIWnd;
	pFrame->SetIDD(nWinID);

	if (pHandler != NULL)
	{
		pFrame->m_pUIWndHandler = pHandler;
		pHandler->m_pBindUIWnd = pFrame;
	}

	// create and load the frame with its resources

	BOOL bRet = pFrame->LoadFrame(nResID, dwStyle, NULL);

	if (!bRet)
	{
		DWORD dwLastError = pFrame->GetLastCreateError();
		switch (dwLastError)
		{
		case E_LOADBMPB: // 1 failed to load the base image
		case E_LOADBMPN: // 2 failed to load the background image of normal status(end by _N.bmp)
		case E_LOADBMPH: // 3 failed to laod the background image of highlight status(end by _H.bmp)
		case E_LOADBMPD: // 4 failed to load the background image of disable status(end by _D.bmp)
		case E_LOADBMPS: // 5 failed to load the background image of selected status(end by _S.bmp)
		case E_LOADBMPC: // 6 failed to load the background image of checked status(end by _C.bmp)
			TRACE(_T("Failed to load background image"));
			break;
		case E_ATTACHBMPB: // 50 failed to attach the HBITMAP of base image to CBitmap object
		case E_ATTACHBMPN: // 51 failed to attach the HBITMAP of normal image to CBitmap object
		case E_ATTACHBMPH: // 52 failed to attach the HBITMAP of highlight image to CBitmap object
		case E_ATTACHBMPD: // 53 failed to attach the HBITMAP of disable image to CBitmap object
		case E_ATTACHBMPS: // 54 failed to attach the HBITMAP of selected image to CBitmap object
		case E_ATTACHBMPC: // 55 failed to attach the HBITMAP of checked image to CBitmap object
		case E_ADJUSTCHILD:// 56 failed to adjust the child controls while change the skin
			TRACE(_T("Failed to attach bitmap to CBitmao object"));
			break;
		case E_CREATECHILD:// 110 failed to create the child window
			TRACE(_T("Failed to create child windows"));
			break;
		default:
			break;
		}

		delete pFrame;
		pFrame = NULL;

		return NULL;
	}

	return pFrame;
}

int CUIMgr::DoModal(UINT nWinID, CUIWndHandler *pHandler)
{
	CUIWnd dlg;
	dlg.SetIDD(nWinID);

	if (pHandler != NULL)
	{
		dlg.m_pUIWndHandler = pHandler;
		pHandler->m_pBindUIWnd = &dlg;
	}

	int nRet = dlg.DoModal();

	return nRet;
}

//////////////////////////////////////////////////////////////////////////
// Image

int LibUIDK::GetImageFileName(HIUIIMAGE hIUIImage, CString *pstrFileName)
{
	if (hIUIImage == NULL || pstrFileName == NULL)
	{
		return -1;
	}

	*pstrFileName = hIUIImage->GetSafeImageName();

	return 0;
}

int LibUIDK::ControlResMgr::IUISetControlImage2(
	UINT uMask,
	__inout HIUIIMAGE *phIUIImage,
	LPCTSTR lpszImageN,
	LPCTSTR lpszImageD)
{
	int nIndex = 0;

	if (IsIncludeFlag(uMask, CONTROL_STATE_NORMAL))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageN);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_DISABLED))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageD);
	}

	return 0;
}

int LibUIDK::ControlResMgr::IUISetControlImage4(
	UINT uMask,
	__inout HIUIIMAGE *phIUIImage,
	LPCTSTR lpszImageNameN,
	LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS,
	LPCTSTR lpszImageNameD)
{
	if (phIUIImage == NULL)
	{
		return -1;
	}

	int nIndex = 0;

	if (IsIncludeFlag(uMask, CONTROL_STATE_NORMAL))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameN);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_HIGHLIGHT))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameH);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_SELECTED))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameS);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_DISABLED))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameD);
	}

	return 0;
}

int LibUIDK::ControlResMgr::IUISetControlCheckedImage4(
	UINT uMask,
	__inout HIUIIMAGE *phIUIImage,
	LPCTSTR lpszImageNameCN,
	LPCTSTR lpszImageNameCH,
	LPCTSTR lpszImageNameCS,
	LPCTSTR lpszImageNameCD)
{
	int nIndex = 0;

	if (IsIncludeFlag(uMask, CONTROL_STATE_CHECKED_NORMAL))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameCN);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_CHECKED_HIGHLIGHT))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameCH);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_CHECKED_SELECTED))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameCS);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_CHECKED_DISABLED))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameCD);
	}

	return 0;
}

int LibUIDK::ControlResMgr::IUISetControlForeground2(
	UINT uMask,
	__inout HIUIIMAGE *phIUIImage,
	LPCTSTR lpszImageNameN,
	LPCTSTR lpszImageNameD)
{
	int nIndex = 0;

	if (IsIncludeFlag(uMask, CONTROL_STATE_NORMAL))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameN);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_DISABLED))
	{
		IUISetControlImage(phIUIImage + nIndex, lpszImageNameD);
	}

	return 0;
}

int LibUIDK::ControlResMgr::IUIGetControlImage(HIUIIMAGE hIUIImage, CString *pstrImageName)
{
	if (hIUIImage == NULL || pstrImageName == NULL)
	{
		return -1;
	}

	return GetImageFileName(hIUIImage, pstrImageName);
}

int LibUIDK::ControlResMgr::IUIGetControlImage2(
	UINT uMask,
	HIUIIMAGE hIUICombineImage,
	HIUIIMAGE *phIUIImage,
	BOOL bCombineImage,
	CString *pstrImageNameCombine,
	CString *pstrImageNameN,
	CString *pstrImageNameD)
{
	if (bCombineImage)
	{
		if (pstrImageNameCombine == NULL || hIUICombineImage == NULL)
		{
			return -1;
		}

		GetImageFileName(hIUICombineImage, pstrImageNameCombine);
	}
	else
	{
		if (phIUIImage == NULL)
		{
			return -1;
		}

		if (IsIncludeFlag(uMask, CONTROL_STATE_NORMAL))
		{
			if (pstrImageNameN == NULL)
			{
				return -1;
			}

			GetImageFileName(*phIUIImage, pstrImageNameN);
		}

		if (IsIncludeFlag(uMask, CONTROL_STATE_DISABLED))
		{
			if (pstrImageNameD == NULL)
			{
				return -1;
			}

			GetImageFileName(*(phIUIImage + 1), pstrImageNameD);
		}
	}

	return 0;
}

int LibUIDK::ControlResMgr::IUIGetControlImage4(
	UINT uMask,
	HIUIIMAGE hIUICombineImage,
	HIUIIMAGE *phIUIImage,
	BOOL bCombineImage,
	CString *pstrImageNameCombine,
	CString *pstrImageNameN,
	CString *pstrImageNameH,
	CString *pstrImageNameS,
	CString *pstrImageNameD)
{
	if (bCombineImage)
	{
		if (pstrImageNameCombine == NULL || hIUICombineImage == NULL)
		{
			return -1;
		}

		GetImageFileName(hIUICombineImage, pstrImageNameCombine);
	}
	else
	{
		if (phIUIImage == NULL)
		{
			return -1;
		}

		if (IsIncludeFlag(uMask, CONTROL_STATE_NORMAL))
		{
			if (pstrImageNameN == NULL)
			{
				return -1;
			}

			GetImageFileName(*phIUIImage, pstrImageNameN);
		}

		if (IsIncludeFlag(uMask, CONTROL_STATE_HIGHLIGHT))
		{
			if (pstrImageNameH == NULL)
			{
				return -1;
			}

			GetImageFileName(*(phIUIImage + 1), pstrImageNameH);
		}

		if (IsIncludeFlag(uMask, CONTROL_STATE_SELECTED))
		{
			if (pstrImageNameS == NULL)
			{
				return -1;
			}

			GetImageFileName(*(phIUIImage + 2), pstrImageNameS);
		}

		if (IsIncludeFlag(uMask, CONTROL_STATE_DISABLED))
		{
			if (pstrImageNameD == NULL)
			{
				return -1;
			}

			GetImageFileName(*(phIUIImage + 3), pstrImageNameD);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Font

CFontMgr g_FontMgr; // Using it's CFontMgr::~CFontMgr

std::map<CString, HIUIFONT> CFontMgr::m_mapFonts;

CFontMgr::~CFontMgr()
{
	// Release font
	if (!m_mapFonts.empty())
	{
		std::map<CString, HIUIFONT>::iterator it = m_mapFonts.begin();
		while (it != m_mapFonts.end())
		{
			ASSERT(it->second != NULL);

			int nRef = it->second->Release();

			if (nRef > 0)
			{
				TRACE(_T("[LibUIDK]===Font '%s' need call ReleaseIUIFont to release\n"),
					it->second->m_strFontResID);
			}

			DeleteObject((HFONT)(*it->second));
			delete it->second;
			m_mapFonts.erase(it);

			it = m_mapFonts.begin();
		}
	}
}

HIUIFONT LibUIDK::CreateEmptyIUIFontInternal(LPCTSTR lpszFontResID)
{
	if (lpszFontResID == NULL || CString(lpszFontResID).IsEmpty())
	{
		return NULL;
	}

	std::map<CString, HIUIFONT>::iterator it = CFontMgr::m_mapFonts.find(lpszFontResID);

	// Not found the font, create the font, and add to map.
	if (it == CFontMgr::m_mapFonts.end())
	{
		CIUIFont *pFont = new CIUIFont;

		if (pFont != NULL)
		{
			pFont->m_strFontResID = lpszFontResID;
			pFont->AddRef();

			CFontMgr::m_mapFonts[lpszFontResID] = pFont;

			return pFont;
		}
	}
	else
	{
		it->second->AddRef();
		return it->second;
	}

	return NULL;
}

// Internal use
HIUIFONT LibUIDK::CreateIUIFontInternal(LPCTSTR lpszFontResID)
{
	HIUIFONT hRet = CreateEmptyIUIFontInternal(lpszFontResID);
	if (hRet == NULL)
	{
		return NULL;
	}

	if (*hRet == NULL)
	{
		hRet->Load(lpszFontResID);
	}

	return hRet;
}

int LibUIDK::ReleaseIUIFontInternal(HIUIFONT hIUIFont)
{
	if (hIUIFont == NULL)
	{
		return -1;
	}

	CString strFontResID;
	GetFontResID(hIUIFont, &strFontResID);

	return ReleaseIUIFont(strFontResID);
}

HFONT LibUIDK::GetHFont(HIUIFONT hIUIFont)
{
	if (hIUIFont == NULL)
	{
		return NULL;
	}

	hIUIFont->SafeLoadSavedFont();

	return HFONT(*hIUIFont);
}

int LibUIDK::GetFontResID(HIUIFONT hIUIFont, CString *pstrFontResID)
{
	if (hIUIFont == NULL || pstrFontResID == NULL)
	{
		return -1;
	}

	*pstrFontResID = hIUIFont->m_strFontResID;

	return 0;
}

// Out use
int FontManager::AddCustomFont(LPCTSTR lpszFontResID, HFONT hFont)
{
	if (lpszFontResID == NULL || hFont == NULL || CString(lpszFontResID).IsEmpty())
	{
		return -1;
	}

	// Find the id exist
	std::map<CString, HIUIFONT>::iterator it = CFontMgr::m_mapFonts.find(lpszFontResID);

	// Not found the font, create the font, and add to map.
	if (it != CFontMgr::m_mapFonts.end())
	{
		return -2; // Already exist
	}

	HIUIFONT hIUIFont = new CIUIFont(hFont);
	hIUIFont->AddRef();

	CFontMgr::m_mapFonts[lpszFontResID] = hIUIFont;

	return 0;
}

int FontManager::UpdateIUIFont(LPCTSTR lpszFontResID, HFONT hFont, BOOL bDeleteOld)
{
	if (lpszFontResID == NULL || hFont == NULL || CString(lpszFontResID).IsEmpty())
	{
		return -1;
	}

	// Find the id exist
	std::map<CString, HIUIFONT>::iterator it = CFontMgr::m_mapFonts.find(lpszFontResID);

	// Not found the font
	if (it == CFontMgr::m_mapFonts.end())
	{
		return -2;
	}

	if (bDeleteOld)
	{
		DeleteObject((HFONT)it->second);
	}
	else
	{
		it->second->Release();
	}

	HIUIFONT hIUIFont = new CIUIFont(hFont);
	hIUIFont->AddRef();

	CFontMgr::m_mapFonts[lpszFontResID] = hIUIFont;

	return 0;
}

BOOL FontManager::FindIUIFont(LPCTSTR lpszFontResID)
{
	if (lpszFontResID == NULL || CString(lpszFontResID).IsEmpty())
	{
		return FALSE;
	}

	// Find the id exist
	std::map<CString, HIUIFONT>::iterator it = CFontMgr::m_mapFonts.find(lpszFontResID);

	// Not found the font
	if (it == CFontMgr::m_mapFonts.end())
	{
		return FALSE;
	}

	return TRUE;
}

HFONT FontManager::CreateIUIFont(LPCTSTR lpszFontResID)
{
	HIUIFONT hIUIFont = CreateIUIFontInternal(lpszFontResID);
	if (hIUIFont == NULL)
	{
		return NULL;
	}

	return hIUIFont->GetSafeHFont();
}

int FontManager::ReleaseIUIFont(LPCTSTR lpszFontResID)
{
	if (lpszFontResID == NULL || CString(lpszFontResID).IsEmpty() || CFontMgr::m_mapFonts.empty())
	{
		return -1;
	}

	std::map<CString, HIUIFONT>::iterator it = CFontMgr::m_mapFonts.find(lpszFontResID);

	// Not found the font
	if (it == CFontMgr::m_mapFonts.end())
	{
		return -2;
	}

	ASSERT(it->second != NULL);

	int nAddRef = it->second->Release();
	if (nAddRef == 0)
	{
		delete it->second;
		CFontMgr::m_mapFonts.erase(it);
	}

	return nAddRef;
}

int FontManager::ReleaseIUIFont(CFont *&pFont)
{
	if (pFont != NULL)
	{
		pFont->DeleteObject();
		delete pFont;
		pFont = NULL;
	}

	return 0;
}

//
int LibUIDK::IUISetControlFont(__inout HIUIFONT *phIUIFont, LPCTSTR lpszFontResID)
{
	if (phIUIFont == NULL)
	{
		return -1;
	}

	CString strNewFontResID = lpszFontResID;

	// 如果新旧字体不一样，释放旧字体
	if (*phIUIFont != NULL)
	{
		if (strNewFontResID != (*phIUIFont)->m_strFontResID)
		{
			ReleaseIUIFontInternal(*phIUIFont);
		}
	}

	// 输入ID有效
	if (!strNewFontResID.IsEmpty())
	{
		*phIUIFont = CreateEmptyIUIFontInternal(lpszFontResID);
	}

	return 0;
}

int LibUIDK::IUISetControlFont2(
	UINT uMask,
	__inout HIUIFONT *phIUIFont,
	LPCTSTR lpszFontResIDN,
	LPCTSTR lpszFontResIDD)
{
	int nIndex = 0;

	if (IsIncludeFlag(uMask, CONTROL_STATE_NORMAL))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDN);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_DISABLED))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDD);
	}

	return 0;
}

int LibUIDK::IUISetControlFont4(
	UINT uMask,
	__inout HIUIFONT *phIUIFont,
	LPCTSTR lpszFontResIDN,
	LPCTSTR lpszFontResIDH,
	LPCTSTR lpszFontResIDS,
	LPCTSTR lpszFontResIDD)
{
	int nIndex = 0;

	if (IsIncludeFlag(uMask, CONTROL_STATE_NORMAL))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDN);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_HIGHLIGHT))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDH);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_SELECTED))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDS);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_DISABLED))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDD);
	}

	return 0;
}

int LibUIDK::IUISetControlCheckedFont4(
	UINT uMask,
	__inout HIUIFONT *phIUIFont,
	LPCTSTR lpszFontResIDCN,
	LPCTSTR lpszFontResIDCH,
	LPCTSTR lpszFontResIDCS,
	LPCTSTR lpszFontResIDCD)
{
	int nIndex = 0;

	if (IsIncludeFlag(uMask, CONTROL_STATE_CHECKED_NORMAL))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDCN);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_CHECKED_HIGHLIGHT))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDCH);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_CHECKED_SELECTED))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDCS);
	}
	nIndex++;

	if (IsIncludeFlag(uMask, CONTROL_STATE_CHECKED_DISABLED))
	{
		IUISetControlFont(phIUIFont + nIndex, lpszFontResIDCD);
	}

	return 0;
}
