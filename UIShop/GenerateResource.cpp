#include "StdAfx.h"
#include "GenerateResource.h"
#include "Resource.h"
#include "global.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//

CGenerateResource::CGenerateResource()
{
}

CGenerateResource::~CGenerateResource()
{
}


//////////////////////////////////////////////////////////////////////////
// protected

int GetImageList(const CTRLPROPERTIES *pCtrlProp, std::vector<CString> *pvImageList, std::vector<CString> *pvBmpList)
{
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
		if (!strImage.IsEmpty())
		{
			CString strExt = GetFileExt(strImage, FALSE);
			if (strExt.CompareNoCase(_T("bmp")) == 0)
			{
				pvBmpList->push_back(strImage);
			}
			else
			{
				pvImageList->push_back(strImage);
			}
		}
	}

	return 0;
}

#define Get_Style_Image_List(StyleType, func)	\
	{\
		std::vector<StyleType *> *pvStyles = CUIMgr::func;\
		size_t nStyleCount = pvStyles->size();\
		for (size_t i=0; i<nStyleCount; ++i)\
		{\
			StyleType *pStyle = *(pvStyles->begin() + i);\
			GetImageList(pStyle, pvImageList, pvBmpList);\
		}\
	}

bool ImageNameCompare(CString str1, CString str2)
{
	int nRet = str1.CompareNoCase(str2);

	return (nRet < 0) ? true : false;
}

bool ImageNameCompare2(CString str1, CString str2)
{
	int nRet = str1.CompareNoCase(str2);

	return (nRet == 0) ? true : false;
}

int CGenerateResource::GetUIFileList(std::vector<CString> *pvImageList, std::vector<CString> *pvBmpList)
{
	CTRLPROPERTIES *pCtrlRet = NULL;

	// Find in window list
	std::vector<UIWNDPROPERTIES *> *pvWindows = CUIMgr::GetUIWndPropertiesList();
	size_t nWindowsCount = pvWindows->size();
	for (size_t i = 0; i < nWindowsCount; ++i)
	{
		UIWNDPROPERTIES *pUIProp = *(pvWindows->begin() + i);
		if (!(bool)pUIProp->m_bLoaded)
		{
			CUIMgr::DelayLoadWindow(pUIProp);
		}

		GetImageList(pUIProp, pvImageList, pvBmpList);

		// Find children
		size_t nChildrenCount = pUIProp->m_vControls.size();
		for (size_t j = 0; j < nChildrenCount; ++j)
		{
			CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[j];

			GetImageList(pCtrlProp, pvImageList, pvBmpList);
		}
	}

	Get_Style_Image_List(BTNPROPERTIES, GetStylePushButton());
	Get_Style_Image_List(CHKPROPERTIES, GetStyleCheckBox());
	Get_Style_Image_List(RADPROPERTIES, GetStyleRadioButton());
	Get_Style_Image_List(CMBPROPERTIES, GetStyleComboBox());
	Get_Style_Image_List(TXTPROPERTIES, GetStyleStatic());
	Get_Style_Image_List(EDTPROPERTIES, GetStyleEdit());
	Get_Style_Image_List(SLDPROPERTIES, GetStyleSlider());
	Get_Style_Image_List(PRGPROPERTIES, GetStyleProgress());
	Get_Style_Image_List(SPNPROPERTIES, GetStyleSpin());
	Get_Style_Image_List(HOTPROPERTIES, GetStyleHotKey());
	Get_Style_Image_List(IPAPROPERTIES, GetStyleIPAddress());
	Get_Style_Image_List(SLDEXPROPERTIES, GetStyleSliderEx());
	Get_Style_Image_List(LSTCTRLPROPERTIES, GetStyleListCtrl());
	Get_Style_Image_List(TREECTRLPROPERTIES, GetStyleTreeCtrl());
	Get_Style_Image_List(TREELISTPROPERTIES, GetStyleTreeList());
	Get_Style_Image_List(RICHEDITPROPERTIES, GetStyleRichEdit());
	Get_Style_Image_List(HTMLCTRLPROPERTIES, GetStyleHtmlCtrl());
	Get_Style_Image_List(DATETIMECTRLPROPERTIES, GetStyleDateTime());
	Get_Style_Image_List(WAVECTRLPROPERTIES, GetStyleWaveCtrl());
	Get_Style_Image_List(MENUPROPERTIES, GetStyleMenu());
	Get_Style_Image_List(MENUBARPROPERTIES, GetStyleMenuBar());
	Get_Style_Image_List(HEADERPROPERTIES, GetStyleHeader());
	Get_Style_Image_List(HORSBPROPERTIES, GetStyleHorScrollBar());
	Get_Style_Image_List(VERSBPROPERTIES, GetStyleVerScrollBar());
	Get_Style_Image_List(LINEWLPROPERTIES, GetStyleWLLine());
	Get_Style_Image_List(PICWLPROPERTIES, GetStyleWLPic());

	// Sort
#ifdef _DEBUG
	size_t nCount = pvImageList->size();
	for (int i = 0; i < nCount; ++i)
	{
		CString s = *(pvImageList->begin() + i);

		TRACE("%s\n", s);
	}
	TRACE("above is before sort\n");
#endif

	std::sort(pvImageList->begin(), pvImageList->end(), ImageNameCompare);
	std::sort(pvBmpList->begin(), pvBmpList->end(), ImageNameCompare);

#ifdef _DEBUG
	size_t nCount2 = pvImageList->size();
	for (int i = 0; i < nCount2; ++i)
	{
		CString s = *(pvImageList->begin() + i);

		TRACE("%s\n", s);
	}
	TRACE("above is after sort\n");
#endif

	// Delete repeated elements.
	std::vector<CString>::iterator pos = unique(pvImageList->begin(), pvImageList->end(), ImageNameCompare2);
	std::vector<CString>::iterator posBmp = unique(pvBmpList->begin(), pvBmpList->end(), ImageNameCompare2);

#ifdef _DEBUG
	size_t nCount3 = pvImageList->size();
	for (int i = 0; i < nCount3; ++i)
	{
		CString s = *(pvImageList->begin() + i);

		TRACE("%s\n", s);
	}
	TRACE("above is after unique sort\n");
#endif

	pvImageList->erase(pos, pvImageList->end());
	pvBmpList->erase(posBmp, pvBmpList->end());

#ifdef _DEBUG
	size_t nCount4 = pvImageList->size();
	for (int i = 0; i < nCount4; ++i)
	{
		CString s = *(pvImageList->begin() + i);

		TRACE("%s\n", s);
	}
	TRACE("above is erase repeated element\n");
#endif

	return 0;
}

// nFormatLength: the length after format the string
// nOverCount: if the original length of str is more than nFormatLength, add nOverCount space to the end of str.
int AddSpaceA(CStringA &str, int nFormatLength, int nOverCount)
{
	int nStrLen = str.GetLength();
	if (nStrLen < nFormatLength)
	{
		int nLast = nFormatLength - nStrLen;
		for (int i = 0; i < nLast; ++i)
		{
			str += _T(" ");
		}
	}
	else
	{
		for (int i = 0; i < nOverCount; ++i)
		{
			str += _T(" ");
		}
	}

	return 0;
}

int AddSpace(CString &str, int nFormatLength, int nOverCount)
{
	int nStrLen = str.GetLength();
	if (nStrLen < nFormatLength)
	{
		int nLast = nFormatLength - nStrLen;
		for (int i = 0; i < nLast; ++i)
		{
			str += _T(" ");
		}
	}
	else
	{
		for (int i = 0; i < nOverCount; ++i)
		{
			str += _T(" ");
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CGenerateResource::SetSource(LPCTSTR lpszSource)
{
	m_strSource = lpszSource;
	m_nLanguage = 0;

	return 0;
}

int CGenerateResource::SetLanguage(int nLanguage)
{
	m_nLanguage = nLanguage;

	return 0;
}

int CGenerateResource::RelateSavePath(BOOL bRelateSavePath)
{
	m_bRelateSavePath = bRelateSavePath;

	return 0;
}

int CGenerateResource::SetCustomRelatePath(LPCTSTR lpszPath)
{
	if (m_bRelateSavePath)
	{
		return -1;
	}

	m_strCustomRelatePath = lpszPath;

	return 0;
}

int CGenerateResource::GeneralResource()
{
	// Get buffer of RC file top part.
	void *pbtLibUIDKSkinRcTop = NULL;
	DWORD dwLibUIDKSkinTopSize = 0;
	int nRet = LoadCustomResourceA(MAKEINTRESOURCEA((m_nLanguage == 1) ? LibUIDKSkinRcTop_Chs : LibUIDKSkinRcTop), "LibUIDKSkin", &pbtLibUIDKSkinRcTop, &dwLibUIDKSkinTopSize);
	if (nRet != 0)
	{
		AfxMessageBox(_T("Failt to load resource!"));
		return -1;
	}

	// Get buffer of RC file bottom part.
	void *pbtLibUIDKSkinRcBottom = NULL;
	DWORD dwLibUIDKSkinRcBottomSize = 0;
	nRet = LoadCustomResourceA(MAKEINTRESOURCEA((m_nLanguage == 1) ? LibUIDKSkinRcBottom_Chs : LibUIDKSkinRcBottom), "LibUIDKSkin", &pbtLibUIDKSkinRcBottom, &dwLibUIDKSkinRcBottomSize);
	if (nRet != 0)
	{
		AfxMessageBox(_T("Failt to load resource!"));
		return -2;
	}

	// Get image file list.
	std::vector<CString> vImageList;
	std::vector<CString> vBmpList;
	GetUIFileList(&vImageList, &vBmpList);

	// Generate skin folder relative path.
	CString strRelativePath;
	if (m_bRelateSavePath)
	{
		strRelativePath = CUIMgr::GetUIPathWithoutTitle();
		CString strRcPath = GetFilePath(m_strSource);
		strRelativePath = GetRelativePath(strRcPath, strRelativePath);
	}
	else
	{
		strRelativePath = m_strCustomRelatePath;
	}
	// The last char must be '\'
	int nLen = strRelativePath.GetLength();
	ASSERT(nLen > 0);
	if (strRelativePath[nLen - 1] != '\\')
	{
		strRelativePath += '\\';
	}
	strRelativePath.Replace(_T("\\"), _T("\\\\"));

	FILE *fp = NULL;
	fopen_s(&fp, (LPCSTR)_bstr_t(m_strSource), "wb");
	if (fp == NULL)
	{
		return -1;
	}

	// Write include resource.h
	fputs("//Microsoft Developer Studio generated resource script.\r\n//\r\n", fp);

	// Write rc top part.
	fwrite(pbtLibUIDKSkinRcTop, dwLibUIDKSkinTopSize, 1, fp);

	// Write the.xui.
	CString strXUIPath = CUIMgr::GetUIPath();
	CString strXUIFile = GetFileName(strXUIPath);
	CString strID = strXUIFile;
	strID.MakeUpper();
	AddSpace(strID, 24, 1);
	strID += _T("LIBUIDKSKIN DISCARDABLE \"");
	strID += strRelativePath;
	strID += strXUIFile;
	strID += _T("\"\r\n");

	_bstr_t bstrXUIFile = strID;
	fputs((LPCSTR)bstrXUIFile, fp);

	// Write resource.h
	CString strResourceH = _T("RESOURCE.H");
	AddSpace(strResourceH, 24, 1);
	strResourceH += _T("LIBUIDKSKIN DISCARDABLE \"");
	strResourceH += strRelativePath;
	strResourceH += _T("Resource.h");
	strResourceH += _T("\"\r\n");
	fputs((LPCSTR)_bstr_t((LPCTSTR)strResourceH), fp);

	// Write non-bitmap files
	size_t nImageCount = vImageList.size();
	for (size_t i = 0; i < nImageCount; ++i)
	{
		CString strImageFile = vImageList[i];

		// Encode file name, exclude ext.
		CString strImageID = CIUIImage::GetFileNameHexCode(strImageFile, FALSE);
		strImageID.MakeUpper();
		AddSpace(strImageID, 24, 1);
		strImageID += _T("LIBUIDKSKIN DISCARDABLE \"");
		strImageID += strRelativePath;
		strImageID += strImageFile;
		strImageID += _T("\"\r\n");
		fputs(_bstr_t(strImageID), fp);
	}

	// Write bitmap files
	size_t nBmpCount = vBmpList.size();
	if (nBmpCount > 0)
	{
		fputs("\r\n/////////////////////////////////////////////////////////////////////////////\r\n", fp);
		fputs("//\r\n", fp);
		fputs("// Bitmap\r\n", fp);
		fputs("//\r\n\r\n", fp);
	}

	for (size_t i = 0; i < nBmpCount; ++i)
	{
		CString strBmpFile = vBmpList[i];

		CString strBmpID = CIUIImage::GetFileNameHexCode(strBmpFile, FALSE);
		strBmpID.MakeUpper();
		AddSpace(strBmpID, 24, 1);
		strBmpID += _T("BITMAP DISCARDABLE \"");
		strBmpID += strRelativePath;
		strBmpID += strBmpFile;
		strBmpID += _T("\"\r\n");
		fputs(_bstr_t(strBmpID), fp);
	}

	// Write rc bottom part.
	fwrite(pbtLibUIDKSkinRcBottom, dwLibUIDKSkinRcBottomSize, 1, fp);

	fclose(fp);

	return 0;
}
