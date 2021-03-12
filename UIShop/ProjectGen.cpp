// ProjectGen.cpp: implementation of the CProjectGen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProjectGen.h"
#include <atlconv.h>
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjectGen::CProjectGen()
{
}

CProjectGen::~CProjectGen()
{

}

int CProjectGen::Release()
{
	m_vConditions.clear();
	m_vFiles.clear();
	m_strInputFolder.Empty();
	m_strProjectName.Empty();
	//    m_strProjectFolder.Empty();

	return 0;
}

int CProjectGen::SetInputFolder(LPCTSTR lpszFolder)
{
	m_strInputFolder = lpszFolder;
	if (m_strInputFolder.Right(1) != "\\" && m_strInputFolder.Right(1) != "/")
	{
		m_strInputFolder += "\\";
	}

	return 0;
}

CStringA CProjectGen::GetInputFolder()
{
	return m_strInputFolder;
}

int CProjectGen::SetProjectName(LPCTSTR lpszPrjName)
{
	m_strProjectName = lpszPrjName;

	m_strProjectFolder = m_strInputFolder + m_strProjectName;

	if (m_strProjectFolder.Right(1) != "\\" && m_strProjectFolder.Right(1) != "/")
	{
		m_strProjectFolder += "\\";
	}

	return 0;
}

CStringA CProjectGen::GetProjectName()
{
	return m_strProjectName;
}

CStringA CProjectGen::GetProjectFolder()
{
	return m_strProjectFolder;
}

int CProjectGen::LoadTemplate(LPVOID pData, DWORD dwSize)
{
	USES_CONVERSION;
	Release();

	InitProjectGUID();

	// Create a temp file.
	CHAR szPath[MAX_PATH] = {0};
	if (FAILED(SHGetFolderPathA(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath)))
	{
		return -1;
	}
	PathAppendA(szPath, "iUIShop\\");
	if (!PathFileExistsA(szPath))
	{
		CreateDirectoryA(szPath, NULL);
	}
	PathAppendA(szPath, "LibUIDK\\");
	if (!PathFileExistsA(szPath))
	{
		CreateDirectoryA(szPath, NULL);
	}

	CStringA strTemp = szPath;
	strTemp += "NewProj.inf~";
	FILE *fp = NULL;
	fopen_s(&fp, strTemp.GetBuffer(0), "wb");
	if (fp == NULL)
	{
		return -1;
	}
	fwrite(pData, dwSize, 1, fp);
	fclose(fp);

	fopen_s(&fp, strTemp.GetBuffer(0), "rb");
	if (fp == NULL)
	{
		return -1;
	}

	while (!feof(fp))
	{
		char szBuf[_MAX_PATH] = {0};
		char *p = fgets(szBuf, _MAX_PATH, fp);
		if (p == NULL)
		{
			break;
		}

		CStringA strBuf = szBuf;
		strBuf.TrimLeft();
		strBuf.TrimRight();

		//
		int n = strBuf.GetLength();
		if (strBuf.GetLength() == 0)
		{
			continue;
		}

		//
		if (strBuf.Left(4) == "$$//")
		{
			continue;
		}

		//
		if (strBuf.Left(4) == "$$IF")
		{
			ParseIFStringInNewProjFile(strBuf);
			ParseIFSectionInNewProjFile(fp);
			continue;
		}

		if (strBuf.Left(2) == "$$") // Don't support other $$, such as $$BEGINLOOP, $$ENDLOOP
		{
			continue;
		}

		//
		GenerateCOPYFILE(strBuf);
	}

	fclose(fp);
	DeleteFileA(strTemp);

	return 0;
}

int CProjectGen::ProcessTemplate()
{
	size_t nFiles = m_vFiles.size();
	for (size_t i = 0; i < nFiles; ++i)
	{
		COPYFILE cf = m_vFiles[i];
		BOOL bCopy = TRUE;

		// check conditions for each file. if any one condition isn't TRUE, the file not copy.
		size_t nConditions = cf.vConditions.size();
		for (size_t j = 0; j < nConditions; ++j)
		{
			Condition c = cf.vConditions[j];

			if (c.bIncludeOrOperator)
			{
				BOOL bTrue = FALSE;
				size_t nMacros = c.vMacros.size();
				for (size_t k = 0; k < nMacros; ++k)
				{
					if (m_Dictionary[(LPTSTR)_bstr_t(c.vMacros[k].strMacro)] == (LPTSTR)_bstr_t(c.vMacros[k].strMacroValue))
					{
						bTrue = TRUE;
						break;
					}
				}
				if (!bTrue)
				{
					bCopy = FALSE;
					break;
				}
			}
			else
			{
#ifdef _DEBUG
				CStringA strValue = (LPCSTR)_bstr_t(m_Dictionary[(LPCTSTR)_bstr_t(c.vMacros[0].strMacro)]);
#endif
				if (m_Dictionary[(LPCTSTR)_bstr_t(c.vMacros[0].strMacro)] != (LPCTSTR)_bstr_t(c.vMacros[0].strMacroValue))
				{
					bCopy = FALSE;
					break;
				}
			}
		}

		// Copy files
		if (bCopy)
		{
			if (cf.bFile)
			{
				CopyResDataToProjectFile(cf.strSource, cf.strTarget, cf.bTranscode);
			}
			else
			{
				CreateDirectoryA(m_strProjectFolder + cf.strTarget, 0);
			}
		}
	}

	return 0;
}

int CProjectGen::LoadCustomResource(LPCSTR lpszResName, LPVOID *lpData, DWORD *pdwSize)
{
	return LoadCustomResourceA(lpszResName, "TEMPLATE", lpData, pdwSize);
}

CString ClearVSVersion(CMapStringToString *pDictionary)
{
	CString strOldSet;

	if ((*pDictionary)[_T("PROJECT_VC60")] == _T("1"))
	{
		strOldSet = _T("PROJECT_VC60");
	}
	(*pDictionary)[_T("PROJECT_VC60")] = _T("0");

	if ((*pDictionary)[_T("PROJECT_VC71")] == _T("1"))
	{
		strOldSet = _T("PROJECT_VC71");
	}
	(*pDictionary)[_T("PROJECT_VC71")] = _T("0");

	if ((*pDictionary)[_T("PROJECT_VC80")] == _T("1"))
	{
		strOldSet = _T("PROJECT_VC80");
	}
	(*pDictionary)[_T("PROJECT_VC80")] = _T("0");

	if ((*pDictionary)[_T("PROJECT_VC90")] == _T("1"))
	{
		strOldSet = _T("PROJECT_VC90");
	}
	(*pDictionary)[_T("PROJECT_VC90")] = _T("0");

	if ((*pDictionary)[_T("PROJECT_VC100")] == _T("1"))
	{
		strOldSet = _T("PROJECT_VC100");
	}
	(*pDictionary)[_T("PROJECT_VC100")] = _T("0");

	if ((*pDictionary)[_T("PROJECT_VC110")] == _T("1"))
	{
		strOldSet = _T("PROJECT_VC110");
	}
	(*pDictionary)[_T("PROJECT_VC110")] = _T("0");

	if ((*pDictionary)[_T("PROJECT_VC120")] == _T("1"))
	{
		strOldSet = _T("PROJECT_VC120");
	}
	(*pDictionary)[_T("PROJECT_VC120")] = _T("0");

	if ((*pDictionary)[_T("PROJECT_VC140")] == _T("1"))
	{
		strOldSet = _T("PROJECT_VC140");
	}
	(*pDictionary)[_T("PROJECT_VC140")] = _T("0");

	return strOldSet;
}

int CProjectGen::CopyResDataToProjectFile(LPCSTR lpszResName, LPCSTR lpszTarget, BOOL bTranscode)
{
	CStringA strResName = lpszResName;

	CString strOldSet;

	if (strResName == "Root_80.sln")
	{
		CStringA strTarget = lpszTarget;
		if (strTarget.Find("_vs2005.sln") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC80")] = _T("1");
		}
		else if (strTarget.Find("_vs2008.sln") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC90")] = _T("1");
		}
	}
	else if (strResName == "Root_100.sln")
	{
		CStringA strTarget = lpszTarget;
		if (strTarget.Find("_vs2010.sln") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC100")] = _T("1");
		}
		else if (strTarget.Find("_vs2012.sln") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC110")] = _T("1");
		}
		else if (strTarget.Find("_vs2013.sln") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC120")] = _T("1");
		}
		else if (strTarget.Find("_vs2015.sln") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC140")] = _T("1");
		}
	}
	else if (strResName == "Root_100.vcxproj")
	{
		CStringA strTarget = lpszTarget;
		if (strTarget.Find("_vs2010.vcxproj") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC100")] = _T("1");
		}
		else if (strTarget.Find("_vs2012.vcxproj") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC110")] = _T("1");
		}
		else if (strTarget.Find("_vs2013.vcxproj") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC120")] = _T("1");
		}
		else if (strTarget.Find("_vs2015.vcxproj") >= 0)
		{
			strOldSet = ClearVSVersion(&m_Dictionary);
			m_Dictionary[_T("PROJECT_VC140")] = _T("1");
		}
	}

	int nRet = 0;
	do
	{
		USES_CONVERSION;
		void *pData = NULL;
		DWORD dwSize = -1;
		nRet = LoadCustomResource(lpszResName, &pData, &dwSize);
		if (nRet != 0)
		{
			break;
		}

		// Create target file.
		CStringA strTarget = GetProjectFolder() + lpszTarget;
		FILE *fpTarget = NULL;
		fopen_s(&fpTarget, strTarget.GetBuffer(0), "wb");
		if (fpTarget == NULL)
		{
			nRet = -1;
			break;
		}

		// The file needn't be transcoded.
		if (!bTranscode)
		{
			fwrite(pData, dwSize, 1, fpTarget);
			fclose(fpTarget);

			nRet = 0;
			break;
		}

		InitFILE_NAME_SYMBOL(lpszTarget);

		// The file need be transcoded.

		// Create a temp file.
		CStringA strTempFile = GetProjectFolder() + lpszTarget;
		strTempFile += "~";
		FILE *fpSource = NULL;
		fopen_s(&fpSource, strTempFile.GetBuffer(0), "wb");
		if (fpSource == NULL)
		{
			nRet = -1;
			break;
		}
		fwrite(pData, dwSize, 1, fpSource);
		fclose(fpSource);

		// reopen the temp file.
		fopen_s(&fpSource, strTempFile.GetBuffer(0), "rb");
		if (fpSource == NULL)
		{
			nRet = -1;
			break;
		}

		while (!feof(fpSource))
		{
			char szBuf[_MAX_PATH] = {0};
			char *p = fgets(szBuf, _MAX_PATH, fpSource);
			if (p == NULL)
			{
				break;
			}

			CStringA strBuf = szBuf;
			strBuf.TrimLeft();
			strBuf.TrimRight();

			//
			if (strBuf.Left(4) == "$$//")
			{
				continue;
			}

			//
			if (strBuf.Left(4) == "$$IF")
			{
				ParseIFStringInTemplateFile(strBuf);
				ParseIFSectionInTemplateFile(fpSource, fpTarget);
				continue;
			}

			//
			ASSERT(m_vConditions.size() == 0);
			CStringA strNew;
			ParseString(szBuf, strNew);

			fwrite(strNew.GetBuffer(strNew.GetLength()), strNew.GetLength(), 1, fpTarget);
		}

		fclose(fpTarget);
		fclose(fpSource);
		DeleteFileA(strTempFile);
	}
	while (false);

	// Restore the old Dictionary
	if (!strOldSet.IsEmpty())
	{
		ClearVSVersion(&m_Dictionary);
		m_Dictionary[strOldSet] = _T("1");
	}

	return 0;
}

int CProjectGen::InitFILE_NAME_SYMBOL(LPCSTR lpszImageName)
{
	USES_CONVERSION;
	// include
	GUID guid;
	CoCreateGuid(&guid);
	wchar_t wszName[64] = {0};
	StringFromGUID2(guid, wszName, sizeof(wszName));
	char szName[64] = {0};
	size_t nSizeConverted = 0;
	wcstombs_s(&nSizeConverted, szName, sizeof(szName), wszName, sizeof(szName));
	CStringA strGUID = szName;
	strGUID.TrimLeft("{");
	strGUID.TrimRight("}");
	strGUID.Replace("-", "_");
	CStringA strSymbol = "AFX_";
	char szFileName[_MAX_PATH] = {0};
	LPSTR p = (LPSTR)lpszImageName;
	_splitpath_s(p, NULL, 0, NULL, 0, szFileName, _MAX_PATH, NULL, 0);
	CStringA strProjectName = szFileName;
	strProjectName.MakeUpper();
	strSymbol += strProjectName;
	strSymbol += "_H__";
	strSymbol += strGUID;
	strSymbol += "_";
	m_Dictionary[_T("FILE_NAME_SYMBOL")] = (LPCTSTR)_bstr_t(strSymbol);

	return 0;
}

int CProjectGen::InitProjectGUID()
{
	GUID guid;

	// project guid
	CoCreateGuid(&guid);
	wchar_t wszName[64] = {0};
	StringFromGUID2(guid, wszName, sizeof(wszName));
	char szName[64] = {0};
	size_t nSizeConverted = 0;
	wcstombs_s(&nSizeConverted, szName, sizeof(szName), wszName, sizeof(szName));
	CStringA strGUID = szName;
	m_Dictionary[_T("PROJECT_GUID")] = (LPCTSTR)_bstr_t(strGUID);

	// solution guid
	CoCreateGuid(&guid);
	StringFromGUID2(guid, wszName, sizeof(wszName));
	wcstombs_s(&nSizeConverted, szName, sizeof(szName), wszName, sizeof(szName));
#ifdef _UNICODE
	strGUID = wszName;
#else
	strGUID = szName;
#endif
	m_Dictionary[_T("SOLUTION_GUID")] = (LPCTSTR)_bstr_t(strGUID);

	// solution2 guid
	CoCreateGuid(&guid);
	StringFromGUID2(guid, wszName, sizeof(wszName));
	wcstombs_s(&nSizeConverted, szName, sizeof(szName), wszName, sizeof(szName));
#ifdef _UNICODE
	strGUID = wszName;
#else
	strGUID = szName;
#endif
	m_Dictionary[_T("SOLUTION_GUID2")] = (LPCTSTR)_bstr_t(strGUID);

	return 0;
}

int CProjectGen::ParseIFSectionInNewProjFile(FILE *fp)
{
	while (!feof(fp))
	{
		char szBuf[_MAX_PATH] = {0};
		char *p = fgets(szBuf, _MAX_PATH, fp);
		if (p == NULL)
		{
			break;
		}

		CStringA strBuf = szBuf;
		strBuf.TrimLeft();
		strBuf.TrimRight();

		//
		if (strBuf.GetLength() == 0)
		{
			continue;
		}

		//
		if (strBuf.Left(4) == "$$//")
		{
			continue;
		}

		//
		if (strBuf.Left(7) == "$$ENDIF")
		{
			RemoveLastIF();
			break;
		}

		//
		if (strBuf.Left(4) == "$$IF")
		{
			ParseIFStringInNewProjFile(strBuf);
			ParseIFSectionInNewProjFile(fp);
		}

		//
		GenerateCOPYFILE(strBuf);
	}

	return 0;
}

int CProjectGen::ParseIFStringInNewProjFile(LPCSTR lpszIF)
{
	USES_CONVERSION;
	CStringA strBuf = lpszIF + strlen("$$IF");
	strBuf.TrimLeft();
	strBuf.TrimRight();
	strBuf.TrimLeft("(");
	strBuf.TrimRight(")");

	Condition c;
	c.bIFSection = TRUE;
	c.bNeedNotOperator = TRUE;
	BOOL bRet = IsIncludeOrOperator(strBuf.GetBuffer(0));
	if (bRet)
	{
		c.bIncludeOrOperator = TRUE;

		int i = 0;
		CStringA strMacro;
		while (ExtractSubString(strMacro, strBuf, i++, "||"))
		{
			StringToString s2s;
			if (strMacro.Left(1) == "!")
			{
				s2s.strMacro = strMacro.Right(strMacro.GetLength() - 1);
				s2s.strMacroValue = "0";
			}
			else
			{
				s2s.strMacro = strMacro;
				s2s.strMacroValue = "1";
			}

			c.vMacros.push_back(s2s);
		}
	}
	else
	{
		c.bIncludeOrOperator = FALSE;
		StringToString s2s;
		if (strBuf.Left(1) == "!")
		{
			s2s.strMacro = strBuf.Right(strBuf.GetLength() - 1);
			s2s.strMacroValue = "0";
		}
		else
		{
			s2s.strMacro = strBuf;
			s2s.strMacroValue = "1";
		}

		c.vMacros.push_back(s2s);
	}

	m_vConditions.push_back(c);

	return 0;
}

int CProjectGen::ParseELIFStringInNewProjFile(LPCSTR lpszELIF)
{
	return 0;
}

int CProjectGen::CustomConfition(FILE *fpTarget)
{
	return 0;
}

int CProjectGen::GenerateCOPYFILE(LPCSTR lpszLine)
{
	CStringA strNew;
	ParseString(lpszLine, strNew);

	// /
	COPYFILE cf;
	if (strNew.Left(1) == "/") // folder
	{
		cf.bFile = FALSE;
		cf.strTarget = strNew.Right(strNew.GetLength() - 1);
	}
	else // file
	{
		cf.bFile = TRUE;
		if (strNew.Left(1) == "=") // don't transcode.
		{
			strNew.Delete(0, 1);
			cf.bTranscode = FALSE;
		}
		else
		{
			cf.bTranscode = TRUE;
		}
		std::string strSource;
		AfxExtractSubStringA(strSource, strNew, strNew.GetLength(), 0, '\t');
		cf.strSource = strSource.c_str();
		std::string strTarget;
		AfxExtractSubStringA(strTarget, strNew, strNew.GetLength(), 1, '\t');
		cf.strTarget = strTarget.c_str();
	}

	cf.vConditions = m_vConditions;
	m_vFiles.push_back(cf);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
int CProjectGen::ParseIFStringInTemplateFile(LPCSTR lpszIF)
{
	USES_CONVERSION;
	CStringA strBuf = lpszIF + strlen("$$IF");
	strBuf.TrimLeft();
	strBuf.TrimRight();
	strBuf.TrimLeft("(");
	strBuf.TrimRight(")");

	Condition c;
	c.bIFSection = TRUE;
	c.bNeedNotOperator = TRUE;
	BOOL bRet = IsIncludeOrOperator(strBuf.GetBuffer(0));    // Check include '||' or not. Such as $$IF(MDI||PROJECT_VC60)
	if (bRet)
	{
		c.bIncludeOrOperator = TRUE;

		int i = 0;
		CStringA strMacro;
		while (ExtractSubString(strMacro, strBuf, i++, "||"))
		{
			StringToString s2s;
			if (strMacro.Left(1) == "!")
			{
				s2s.strMacro = strMacro.Right(strMacro.GetLength() - 1);
				s2s.strMacroValue = "0";
			}
			else
			{
				s2s.strMacro = strMacro;
				s2s.strMacroValue = "1";
			}

			c.vMacros.push_back(s2s);
		}
	}
	else
	{
		c.bIncludeOrOperator = FALSE;
		StringToString s2s;
		if (strBuf.Left(1) == "!")
		{
			s2s.strMacro = strBuf.Right(strBuf.GetLength() - 1);
			s2s.strMacroValue = "0";
		}
		else
		{
			s2s.strMacro = strBuf;
			s2s.strMacroValue = "1";
		}

		c.vMacros.push_back(s2s);
	}

	m_vConditions.push_back(c);

	return 0;
}

int CProjectGen::ParseIFSectionInTemplateFile(FILE *fpSource, FILE *fpTarget)
{
	USES_CONVERSION;
	while (!feof(fpSource))
	{
		char szBuf[_MAX_PATH] = {0};
		char *p = fgets(szBuf, _MAX_PATH, fpSource);
		if (p == NULL)
		{
			break;
		}

		CStringA strBuf = szBuf;
		strBuf.TrimLeft();
		strBuf.TrimRight();

		//
		if (strBuf.Left(4) == "$$//")
		{
			continue;
		}

		//
		if (strBuf.Left(7) == "$$ENDIF")
		{
			CustomConfition(fpTarget);
			RemoveLastIF();
			break;
		}

		//
		if (strBuf.Left(4) == "$$IF")
		{
			ParseIFStringInTemplateFile(strBuf);
			ParseIFSectionInTemplateFile(fpSource, fpTarget);
			continue;
		}

		//
		BOOL bWrite = TRUE;
		size_t nConditions = m_vConditions.size();
		for (size_t i = 0; i < nConditions; ++i)
		{
			Condition c = m_vConditions[i];
			if (c.bIncludeOrOperator)
			{
				BOOL bTrue = FALSE;
				size_t nMacros = c.vMacros.size();
				for (size_t j = 0; j < nMacros; ++j)
				{
					StringToString s2s = c.vMacros[j];
					if (m_Dictionary[(LPCTSTR)_bstr_t(s2s.strMacro)] == (LPCTSTR)_bstr_t(s2s.strMacroValue))
					{
						bTrue = TRUE;
					}
				}
				if (!bTrue)
				{
					bWrite = FALSE;
				}
			}
			else
			{
				CString strValue = m_Dictionary[(LPCTSTR)_bstr_t(c.vMacros[0].strMacro)];
				if (strValue.IsEmpty())    // If the key not exist
				{
					strValue = _T("0");
				}

				if (strValue != (LPCTSTR)_bstr_t(c.vMacros[0].strMacroValue))
				{
					bWrite = FALSE;
				}
			}
		}
		if (bWrite)
		{
			CStringA strNew;
			ParseString(szBuf, strNew);

			fwrite(strNew.GetBuffer(0), strNew.GetLength(), 1, fpTarget);
		}
	}

	return 0;
}

int CProjectGen::ParseString(LPCSTR lpszSource, CStringA &rstrTarget)
{
	int nFirst = -1;
	int nSecond = -1;
	CStringA strMacro;
	BOOL bHas = IsHasMacro(lpszSource, &nFirst, &nSecond, strMacro);

	if (bHas)
	{
		CStringA strSource = lpszSource;
		CStringA strTarget;
		strTarget = strSource.Left(nFirst);
		strTarget += m_Dictionary[(LPCTSTR)_bstr_t(strMacro)];
		strTarget += lpszSource + nSecond + 2;

		if (IsHasMacro(strTarget, &nFirst, &nSecond, strMacro))
		{
			ParseString(strTarget, rstrTarget);
		}
		else
		{
			rstrTarget = strTarget;
		}
	}
	else
	{
		rstrTarget = lpszSource;
	}

	return 0;
}

BOOL CProjectGen::IsHasMacro(LPCSTR lpszBuf, int *pnFirst, int *pnSecond, CStringA &rstrMacro)
{
	CStringA strBuf = lpszBuf;
	int nFirst = strBuf.Find("$$");
	if (nFirst == -1)
	{
		return FALSE;
	}
	int nSecond = strBuf.Find("$$", nFirst + 2);
	if (nSecond == -1)
	{
		return FALSE;
	}

	*pnFirst = nFirst;
	*pnSecond = nSecond;
	rstrMacro = strBuf.Right(strBuf.GetLength() - nFirst - 2);
	rstrMacro = rstrMacro.Left(nSecond - nFirst - 2);

	return TRUE;
}

int CProjectGen::RemoveLastIF()
{
	size_t nConditions = m_vConditions.size();
	vector<Condition>::reverse_iterator rit = m_vConditions.rbegin();
	for (; rit != m_vConditions.rend(); ++rit)
	{
		Condition c = *rit;
		m_vConditions.erase(m_vConditions.end() - 1);
		if (c.bIFSection)
		{
			break;
		}
	}

	return 0;
}

BOOL CProjectGen::IsIncludeOrOperator(LPCSTR lpMacros)
{
	CStringA strBuf = lpMacros;
	int nPos = strBuf.Find("||");
	if (nPos == -1)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CProjectGen::ExtractSubString(CStringA &rString, LPCSTR lpszFullString, int iSubString, CHAR *szSep)
{
	USES_CONVERSION;
	if (lpszFullString == NULL)
	{
		return FALSE;
	}

	while (iSubString--)
	{
		CStringA strBuf = lpszFullString;
		int nPos = strBuf.Find(szSep);
		if (nPos == -1)
		{
			rString.Empty();        // return empty string as well
			return FALSE;
		}
		lpszFullString += (nPos + strlen(szSep));       // point past the separator
	}

	CStringA strBuf = lpszFullString;
	int nPos = strBuf.Find(szSep);
	int nLen = -1;
	if (nPos == -1)
	{
		nLen = lstrlenA(lpszFullString);
	}
	else
	{
		nLen = nPos;
	}

	ASSERT(nLen >= 0);
	memcpy(rString.GetBufferSetLength(nLen), lpszFullString, nLen * sizeof(TCHAR));
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// CClassGen

CClassGen::CClassGen()
{
	m_pUIProp = NULL;
}

CClassGen::~CClassGen()
{

}

int CClassGen::Release()
{
	m_vConditions.clear();
	m_vFiles.clear();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CClassGen::SetClassName(LPCTSTR lpszClassName)
{
	m_strClassName = (LPCSTR)_bstr_t(lpszClassName);

	return 0;
}

CStringA CClassGen::GetClassName()
{
	return m_strClassName;
}

CStringA CClassGen::GetProjectFolder()
{
	return m_strInputFolder;
}

int CClassGen::SetUIWndProperties(const UIWNDPROPERTIES *pUIProp)
{
	m_pUIProp = pUIProp;
	return 0;
}

CString CClassGen::GetVariableName(const CTRLPROPERTIES *pCtrlProp)
{
	ASSERT(pCtrlProp != NULL);
	CString strBuf;

	strBuf += _T("m_p");
	strBuf += pCtrlProp->GetVariablePrefix();
	strBuf += pCtrlProp->GetVariableName();

	return strBuf;
}

int CClassGen::CustomConfition(FILE *fpTarget)
{
	size_t nConditions = m_vConditions.size();
	if (nConditions <= 0)
	{
		return -1;
	}

	if (m_Dictionary[_T("GEN_HANDLER")] != _T("1"))
	{
		return -2;
	}

	Condition c = m_vConditions[nConditions - 1];
	if (c.vMacros[0].strMacro == "GEN_VARIABLE_DECLARE")
	{
		GenerateVariableDeclare(fpTarget);
	}
	else if (c.vMacros[0].strMacro == "GEN_VARIABLE_INIT")
	{
		GenerateVariableInit(fpTarget);
	}
	else if (c.vMacros[0].strMacro == "GEN_VARIABLE_ASSIGN")
	{
		GenerateVariableAssign(fpTarget, FALSE);
	}
	else if (c.vMacros[0].strMacro == "GEN_HANDLER_DECLARE")
	{
		GenerateHandlerDeclare(fpTarget);
	}
	else if (c.vMacros[0].strMacro == "GEN_HANDLER_MSG_MAP")
	{
		GenerateHandlerMsgMap(fpTarget);
	}
	else if (c.vMacros[0].strMacro == "GEN_HANDLER_IMPLE")
	{
		GenerateHandlerImplementation(fpTarget);
	}
	else if (c.vMacros[0].strMacro == "GEN_HANDLER_INIT_CONTROLS_DECLARE")
	{
		GenerateInitControlsDeclare(fpTarget);
	}
	else if (c.vMacros[0].strMacro == "GEN_HANDLER_INIT_CONTROLS_IMPLE")
	{
		GenerateInitControlsImplementation(fpTarget);
	}
	else if (c.vMacros[0].strMacro == "GEN_HANDLER_CALL_INIT")
	{
		GenerateCallInitControls(fpTarget);
	}
	else if (c.vMacros[0].strMacro == _T("GEN_DO_DATA_EXCHANGE_VARIABLE_DECLARE"))
	{
		GenerateDoDataExchangeVariableDeclare(fpTarget);
	}
	else if (c.vMacros[0].strMacro == _T("GEN_DO_DATA_EXCHANGE_VARIABLE_INIT"))
	{
		GenerateDoDataExchangeVariableInit(fpTarget);
	}
	else if (c.vMacros[0].strMacro == _T("GEN_DO_DATA_EXCHANGE_VARIABLE_BIND"))
	{
		GenerateDoDataExchangeVariableBind(fpTarget);
	}

	return 0;
}

// CSkinButton *m_pBtn1;
int CClassGen::GenerateVariableDeclare(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CString strBuf;
	CStringA strBufA;

	// write protected
	strBufA = "\r\nprotected:\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		int nRet = ::GenerateVariableDeclare(pCtrlProp, strBufA);
		if (nRet != 0)
		{
			continue;
		}

		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
	}

	return 0;
}

// m_pBtn1 = NULL;
int CClassGen::GenerateVariableInit(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CStringA strBufA;

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		::GenerateVariableInit(pCtrlProp, strBufA);

		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
	}

	return 0;
}

int CClassGen::GenerateVariableAssign(FILE *fpTarget, BOOL bAddCaller)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CString strBuf;
	CStringA strBufA;

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		int nRet = GenerateVariableAssignString(pCtrlProp, strBufA, bAddCaller);
		if (nRet != 0)
		{
			continue;
		}

		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
	}

	strBufA = "\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	return 0;
}

// afx_msg void OnBtn1();
int CClassGen::GenerateHandlerDeclare(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CString strBuf;
	CStringA strBufA;

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		if (eControlType == CT_PUSHBUTTON)
		{
			BTNPROPERTIES *pBtnProp = (BTNPROPERTIES *)pCtrlProp;

			GenerateHandlerDeclareString(pCtrlProp, strBufA);
			fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
		}
		else if (eControlType == CT_CHECK)
		{
			CHKPROPERTIES *pChkProp = (CHKPROPERTIES *)pCtrlProp;

			GenerateHandlerDeclareString(pCtrlProp, strBufA);
			fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
		}
	}

	return 0;
}

// ON_BN_CLICKED(IDC_BTN_1, OnBtn1)
int CClassGen::GenerateHandlerMsgMap(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CStringA strBufA;

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		if (eControlType == CT_PUSHBUTTON || eControlType == CT_CHECK)
		{
			GenerateButtonHandlerMsgMap(pCtrlProp, strBufA);

			fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
		}
		else if (eControlType == CT_CHECK)
		{

		}
	}

	return 0;
}

// void CYourWnd::OnBtn1() { AfxMessage(_T("ID"); }
int CClassGen::GenerateHandlerImplementation(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CStringA strBufA;

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		if (eControlType == CT_PUSHBUTTON || eControlType == CT_CHECK)
		{
			strBufA = "\r\n";
			fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

			CStringA strHandle;
			CString strClassName = m_Dictionary[_T("Class")];
			::GenerateHandlerImplementation(pCtrlProp, (LPCSTR)_bstr_t(strClassName), strHandle);

			fwrite(strHandle.GetBuffer(0), strHandle.GetLength(), 1, fpTarget);
		}
	}

	return 0;
}

// Int InitTree1();
int CClassGen::GenerateInitControlsDeclare(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CString strBuf;
	CStringA strBufA;

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];
		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;

		if (eControlType != CT_COMBOBOX
			&& eControlType != CT_TREECTRL
			&& eControlType != CT_LISTCTRL
			&& eControlType != CT_TREELIST
			&& eControlType != CT_THUMBNAIL
			&& eControlType != CT_HTMLCTRL
			&& eControlType != CT_PNL_STACK)
		{
			continue;
		}

		strBuf = _T("\tint ");
		strBuf += _T("Init");
		strBuf += pCtrlProp->GetVariablePrefix();
		strBuf += pCtrlProp->GetVariableName();
		strBuf += _T("();\r\n");
		strBufA = (LPCSTR)_bstr_t(strBuf);

		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
	}

	return 0;
}

// Init CYourWnd::InitTree1() {}
int CClassGen::GenerateInitControlsImplementation(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CString strBuf;
	CStringA strBufA;

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];
		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;

		if (eControlType != CT_COMBOBOX
			&& eControlType != CT_TREECTRL
			&& eControlType != CT_LISTCTRL
			&& eControlType != CT_TREELIST
			&& eControlType != CT_THUMBNAIL
			&& eControlType != CT_HTMLCTRL
			&& eControlType != CT_PNL_STACK)
		{
			continue;
		}

		strBufA = "\r\n";
		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

		strBuf = _T("int ");
		strBuf += m_Dictionary[_T("Class")];
		strBuf += _T("::Init");
		strBuf += pCtrlProp->GetVariablePrefix();
		strBuf += pCtrlProp->GetVariableName();
		strBuf += _T("()\r\n{\r\n");

		if (eControlType == CT_COMBOBOX)
		{

		}
		else if (eControlType == CT_TREECTRL)
		{
			strBuf += _T("\tHTREEITEM hRoot = ");
			strBuf += CClassGen::GetVariableName(pCtrlProp);
			strBuf += _T("->InsertItem(_T(\"Root Item\"));\r\n\t");
			strBuf += CClassGen::GetVariableName(pCtrlProp);
			strBuf += _T("->InsertItem(_T(\"Sub Item\"), hRoot);\r\n\t");
			strBuf += CClassGen::GetVariableName(pCtrlProp);
			strBuf += _T("->Expand(hRoot, TVE_EXPAND);\r\n\r\n");
		}
		else if (eControlType == CT_LISTCTRL)
		{
			LSTCTRLPROPERTIES *pLstProp = (LSTCTRLPROPERTIES *)pCtrlProp;
			switch ((LST_STYLE)pLstProp->m_eListStyle)
			{
			case LSTS_REPORT:
				strBuf += _T("\t");
				strBuf += CClassGen::GetVariableName(pCtrlProp);
				strBuf += _T("->InsertColumn(0, _T(\"Name\"), LVCFMT_LEFT, 50);\r\n\t");
				strBuf += CClassGen::GetVariableName(pCtrlProp);
				strBuf += _T("->InsertColumn(1, _T(\"Age\"), LVCFMT_LEFT, 150);\r\n\tint nIndex = ");
				strBuf += CClassGen::GetVariableName(pCtrlProp);
				strBuf += _T("->InsertItem(0, _T(\"David\"));\r\n\t");
				strBuf += CClassGen::GetVariableName(pCtrlProp);
				strBuf += _T("->SetItemText(nIndex, 1, _T(\"23\"));\r\n\tnIndex = ");
				strBuf += CClassGen::GetVariableName(pCtrlProp);
				strBuf += _T("->InsertItem(1, _T(\"John\"));\r\n\t");
				strBuf += CClassGen::GetVariableName(pCtrlProp);
				strBuf += _T("->SetItemText(nIndex, 1, _T(\"25\"));\r\n\r\n");
				break;
			case LSTS_ICON:
			case LSTS_SMALLICON:
			case LSTS_LIST:
			case LSTS_THUMBNAIL:
				strBuf += _T("\t");
				strBuf += CClassGen::GetVariableName(pCtrlProp);
				strBuf += _T("->InsertItem(0, _T(\"David\"));\r\n\t");
				strBuf += CClassGen::GetVariableName(pCtrlProp);
				strBuf += _T("->InsertItem(1, _T(\"John\"));\r\n\r\n");
				break;
			default:
				break;
			}
		}
		else if (eControlType == CT_TREELIST)
		{

		}
		else if (eControlType == CT_THUMBNAIL)
		{

		}
		else if (eControlType == CT_HTMLCTRL)
		{

		}
		else if (eControlType == CT_PNL_STACK)
		{

		}

		strBuf += _T("\treturn 0;\r\n}\r\n");
		strBufA = (LPCSTR)_bstr_t(strBuf);

		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
	}

	return 0;
}

// Call InitTree1();
int CClassGen::GenerateCallInitControls(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CString strBuf;
	CStringA strBufA;

	BOOL bWrite = FALSE;
	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];
		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;

		if (eControlType != CT_COMBOBOX
			&& eControlType != CT_TREECTRL
			&& eControlType != CT_LISTCTRL
			&& eControlType != CT_TREELIST
			&& eControlType != CT_THUMBNAIL
			&& eControlType != CT_HTMLCTRL
			&& eControlType != CT_PNL_STACK)
		{
			continue;
		}

		if (!bWrite)
		{
			strBufA = "\r\n";
			fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

			bWrite = TRUE;
		}

		strBuf = _T("\t");
		strBuf += _T("Init");
		strBuf += pCtrlProp->GetVariablePrefix();
		strBuf += pCtrlProp->GetVariableName();
		strBuf += _T("();\r\n");
		strBufA = (LPCSTR)_bstr_t(strBuf);

		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
	}

	return 0;
}

// BOOL m_bFlag;
int CClassGen::GenerateDoDataExchangeVariableDeclare(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CStringA strBufA;

	// write public
	strBufA = "\r\npublic:\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		int nRet = ::GenerateDoDataExchangeVariableDeclare(pCtrlProp, strBufA);
		if (nRet != 0)
		{
			continue;
		}


		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
	}

	return 0;
}

// m_str1 = _T("");
int CClassGen::GenerateDoDataExchangeVariableInit(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CString strBuf;
	CStringA strBufA;

	// write public
	strBufA = "\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];
		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;

		if (eControlType != CT_EDIT && eControlType != CT_CHECK && eControlType != CT_COMBOBOX)
		{
			continue;
		}

		strBuf = _T("\t");
		if (eControlType == CT_EDIT || eControlType == CT_COMBOBOX)
		{
			strBuf += _T("m_str");
		}
		else if (eControlType == CT_CHECK)
		{
			strBuf += _T("m_b");
		}

		strBuf += pCtrlProp->GetVariableName();

		if (eControlType == CT_EDIT || eControlType == CT_COMBOBOX)
		{
			strBuf += _T(" = _T(\"\")");
		}
		else if (eControlType == CT_CHECK)
		{
			strBuf += _T(" = FALSE");
		}

		strBuf += _T(";\r\n");
		strBufA = (LPCSTR)_bstr_t(strBuf);

		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
	}

	return 0;
}

// DDX_Text(pDX, IDC_EDT_1, m_str1);
int CClassGen::GenerateDoDataExchangeVariableBind(FILE *fpTarget)
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	CString strBuf;
	CStringA strBufA;

	// write public
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];
		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;

		if (eControlType != CT_EDIT && eControlType != CT_CHECK && eControlType != CT_COMBOBOX)
		{
			continue;
		}

		strBuf = _T("\t");
		if (eControlType == CT_EDIT)
		{
			strBuf += _T("DDX_Text(pDX, ");
		}
		else if (eControlType == CT_CHECK)
		{
			strBuf += _T("DDX_Check(pDX, ");
		}
		else if (eControlType == CT_COMBOBOX)
		{
			strBuf += _T("DDX_CBString(pDX, ");
		}

		strBuf += pCtrlProp->m_strID;
		strBuf += _T(", ");

		if (eControlType == CT_EDIT || eControlType == CT_COMBOBOX)
		{
			strBuf += _T("m_str");
		}
		else if (eControlType == CT_CHECK)
		{
			strBuf += _T("m_b");
		}

		strBuf += pCtrlProp->GetVariableName();
		strBuf += _T(");\r\n");

		strBufA = (LPCSTR)_bstr_t(strBuf);
		fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////

CEventHandlerGen::CEventHandlerGen()
{

}

CEventHandlerGen::~CEventHandlerGen()
{

}

int CEventHandlerGen::GenerateHFile()
{
	// Create target file.
	CStringA strClassName = GetClassName();
	CStringA strTarget = GetInputFolder();
	if (strTarget.Right(1) != '\\' && strTarget.Right(1) != '/')
	{
		strTarget += '\\';
	}

	CStringA strHFile = strTarget;
	strHFile += m_Dictionary[_T("HFile")];
	CStringA strTempHFile = strHFile;
	strTempHFile += "~";

	FILE *fpTarget = NULL;
	fopen_s(&fpTarget, strTempHFile.GetBuffer(0), "wb");
	if (fpTarget == NULL)
	{
		return -1;
	}

	// Generate #pragma once
	fputs("#pragma once\r\n\r\n\r\n", fpTarget);

	// Generate event handler class define
	CStringA strBufA;
	strBufA = GenerateClassDefine(strClassName);
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate int InitControls(CUIWnd *pUIWnd);
	strBufA = "\tint InitControls(CUIWnd *pUIWnd);\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	//
	GenerateInitControlsDeclare(fpTarget);

	strBufA = "\r\nprotected:\r\n\tafx_msg LRESULT OnCreate(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	strBufA = "\tafx_msg LRESULT OnDestroy(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	strBufA = "\tafx_msg LRESULT OnGetChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	strBufA = "\tafx_msg LRESULT OnReleaseChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	CString strBuf;

	// Generate control's message handler
	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		if (eControlType == CT_PUSHBUTTON || eControlType == CT_CHECK)
		{
			strBuf = _T("\tafx_msg LRESULT ");
			strBuf += CombineButtonHandlerDeclare(strClassName, pCtrlProp);
			strBuf += _T(";\r\n");
			strBufA = (LPCSTR)_bstr_t(strBuf);

			fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
		}
	}

	strBufA = "\tIUI_DECLARE_MESSAGE_MAP()";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	strBufA = "\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate variable
	GenerateVariableDeclare(fpTarget);

	strBufA = "};\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	fclose(fpTarget);
	CopyFileA(strTempHFile, strHFile, FALSE);
	DeleteFileA(strTempHFile);

	return 0;
}

int CEventHandlerGen::GenerateCPPFile()
{
	// Create target file.
	CStringA strClassName = GetClassName();
	CStringA strTarget = GetInputFolder();
	if (strTarget.Right(1) != '\\' && strTarget.Right(1) != '/')
	{
		strTarget += '\\';
	}

	CStringA strCPPFile = strTarget;
	strCPPFile += m_Dictionary[_T("CppFile")];
	CStringA strTempCPPFile = strCPPFile;
	strTempCPPFile += "~";

	FILE *fpTarget = NULL;
	fopen_s(&fpTarget, strTempCPPFile.GetBuffer(0), "wb");
	if (fpTarget == NULL)
	{
		return -1;
	}

	// Generate #include "StdAfx.h"
	fputs("#include \"StdAfx.h\"\r\n", fpTarget);

	// Generate #include "CMainFrameHandle.h"
	CStringA strBufA;
	strBufA = "#include \"";
	strBufA += m_Dictionary[_T("HFile")];
	strBufA += "\"\r\n\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate IUI_DEBUG_NEW
	strBufA = "#ifdef _DEBUG\r\n#define new	IUI_DEBUG_NEW\r\n#endif // _DEBUG\r\n\r\n\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);


	// Generate Message Map
	strBufA = "IUI_BEGIN_MESSAGE_MAP(";
	strBufA += strClassName;
	strBufA += ")\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	BOOL bAddTab = FALSE;
	strBufA = "IUI_ON_MESSAGE(WM_CREATE, &";
	if (bAddTab)
	{
		strBufA.Insert(0, "\t");
	}
	strBufA += strClassName;
	strBufA += "::OnCreate)\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	strBufA = "IUI_ON_MESSAGE(WM_DESTROY, &";
	if (bAddTab)
	{
		strBufA.Insert(0, "\t");
	}
	strBufA += strClassName;
	strBufA += "::OnDestroy)\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	strBufA = "IUI_ON_MESSAGE(WM_GETCHILDMSGHANDLER, &";
	if (bAddTab)
	{
		strBufA.Insert(0, "\t");
	}
	strBufA += strClassName;
	strBufA += "::OnGetChildMsgHandler)\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	strBufA = "IUI_ON_MESSAGE(WM_RELEASECHILDMSGHANDLER, &";
	if (bAddTab)
	{
		strBufA.Insert(0, "\t");
	}
	strBufA += strClassName;
	strBufA += "::OnReleaseChildMsgHandler)\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	CString strBuf;

	size_t nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		if (eControlType == CT_PUSHBUTTON || eControlType == CT_CHECK)
		{
			// Generate IUI_ON_BN_CLICKED(IDC_BTN_CHILD, &CChildWndHandler::OnBtnChild)
			// Generate {IDW_MAIN, WM_COMMAND, BN_CLICKED, IDC_RAD_ABC, 0, 0, CMainFrame_OnRadAbc},
			strBuf = _T("IUI_ON_BN_CLICKED(");
			if (bAddTab)
			{
				strBufA.Insert(0, "\t");
			}
			strBuf += (LPCTSTR)pCtrlProp->m_strID;
			strBuf += _T(", &");
			strBuf += (LPCTSTR)_bstr_t(strClassName);
			strBuf += _T("::");
			strBuf += CombineButtonHandlerName(strClassName, pCtrlProp);
			strBuf += _T(")\r\n");
			strBufA = (LPCSTR)_bstr_t(strBuf);
			fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
		}
	}

	strBufA = "IUI_END_MESSAGE_MAP()\r\n\r\n\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate constructed function
	strBufA = strClassName;
	strBufA += "::";
	strBufA += strClassName;
	strBufA += "(UINT uWinID)\r\n\t: CUIWndHandler(uWinID)\r\n{\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	GenerateVariableInit(fpTarget);

	strBufA = "}\r\n\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate Destructor function
	strBufA = strClassName;
	strBufA += "::~";
	strBufA += strClassName;
	strBufA += "()\r\n{\r\n}\r\n\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate InitControls(CUIWnd *pUIWnd)
	strBufA = "//////////////////////////////////////////////////////////////////////////\r\n";
	strBufA += "// protected\r\n\r\n";
	strBufA += "int ";
	strBufA += strClassName;
	strBufA += "::InitControls(CUIWnd *pUIWnd)\r\n{\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	GenerateVariableAssign(fpTarget, TRUE);

	strBufA = "\treturn 0;\r\n}\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	GenerateInitControlsImplementation(fpTarget);

	// Messages section
	strBufA = "\r\n//////////////////////////////////////////////////////////////////////////\r\n";
	strBufA += "// Messages\r\n\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate OnCreate
	strBufA = "LRESULT ";
	strBufA += strClassName;
	strBufA += "::OnCreate(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)\r\n";
	strBufA += "{\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	strBufA = "\tif (InitControls(pUIWnd) != 0)\r\n\t{\r\n\t\treturn -1;\r\n\t}\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	GenerateCallInitControls(fpTarget);

	strBufA = "\r\n\treturn 0;\r\n}\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate OnDestroy
	strBufA = "\r\nLRESULT ";
	strBufA += strClassName;
	strBufA += "::OnDestroy(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)\r\n";
	strBufA += "{\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	strBufA = "\treturn 0;\r\n}\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate OnGetChildMsgHandler
	strBufA = "\r\n// 为控件绑定的窗口指定消息映射类\r\nLRESULT ";
	strBufA += strClassName;
	strBufA += "::OnGetChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)\r\n{\r\n";
	strBufA += "\tUINT uWinID = (UINT)wParam;\r\n";
	strBufA += "\tCHILDMSGHANDLER *pChildMsgHandler = (CHILDMSGHANDLER *)lParam;\r\n";
	strBufA += "\tif (pChildMsgHandler == NULL)\r\n";
	strBufA += "\t{\r\n\t\treturn 0;\r\n\t}\r\n\r\n";
	strBufA += "\treturn 0;\r\n}\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate OnReleaseChildMsgHandler
	strBufA = "\r\n// 释放控件绑定的窗口对应的消息映射类, 释放后返回TRUE，否则返回FALSE\r\nLRESULT ";
	strBufA += strClassName;
	strBufA += "::OnReleaseChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)\r\n{\r\n";
	strBufA += "\tUINT uWinID = (UINT)wParam;\r\n";
	strBufA += "\tCHILDMSGHANDLER *pChildMsgHandler = (CHILDMSGHANDLER *)lParam;\r\n";
	strBufA += "\tif (pChildMsgHandler == NULL)\r\n";
	strBufA += "\t{\r\n\t\treturn (LRESULT)FALSE;\r\n\t}\r\n\r\n";
	strBufA += "\treturn (LRESULT)FALSE;\r\n}\r\n";
	fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);

	// Generate handler: LRESULT CYourHandler::OnRadAbc(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
	nCount = m_pUIProp->m_vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		const CTRLPROPERTIES *pCtrlProp = m_pUIProp->m_vControls[i];

		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		if (eControlType == CT_PUSHBUTTON || eControlType == CT_CHECK)
		{
			strBuf = _T("\r\nLRESULT ");
			strBuf += strClassName;
			strBuf += _T("::");
			strBuf += CombineButtonHandlerDeclare(strClassName, pCtrlProp);
			strBuf += _T("\r\n{\r\n");

			// Generate AfxMessageBox(_T("ID"));
			strBuf += _T("\tAfxMessageBox(_T(\"");
			strBuf += (LPCTSTR)pCtrlProp->m_strID;

			// Generate return 0;
			strBuf += _T("\"));\r\n\treturn 0;\r\n}\r\n");

			strBufA = (LPCSTR)_bstr_t(strBuf);
			fwrite(strBufA.GetBuffer(0), strBufA.GetLength(), 1, fpTarget);
		}
	}

	fclose(fpTarget);
	CopyFileA(strTempCPPFile, strCPPFile, FALSE);
	DeleteFileA(strTempCPPFile);

	return 0;
}

CStringA CEventHandlerGen::CombineHanlerMapVariable(LPCSTR lpszClassName)
{
	CStringA strBufA;

	strBufA = "g_";
	strBufA += lpszClassName;
	strBufA += "HandlerMap";

	return strBufA;
}

CStringA CEventHandlerGen::CombineInitFunction(LPCSTR lpszClassName)
{
	CStringA strBufA;

	strBufA = "int Init";
	strBufA += lpszClassName;
	strBufA += "MsgMap(CUIWnd *pUIWnd)";

	return strBufA;
}

CStringA CEventHandlerGen::GenerateClassDefine(LPCSTR lpszClassName)
{
	CStringA strBufA;

	strBufA = "class ";
	strBufA += lpszClassName;
	strBufA += " : public CUIWndHandler\r\n{\r\n";
	strBufA += "public:\r\n\t";
	strBufA += lpszClassName;
	strBufA += "(UINT uWinID);\r\n\t";
	strBufA += "virtual ~";
	strBufA += lpszClassName;
	strBufA += "();\r\n\r\nprotected:\r\n";

	return strBufA;
}

CString CEventHandlerGen::CombineButtonHandlerName(LPCSTR lpszClassName, const CTRLPROPERTIES *pCtrlProp)
{
	CString strBuf;

	strBuf = _T("On");

	CONTROL_TYPE eControlType = pCtrlProp->m_eControlType;
	if (eControlType == CT_PUSHBUTTON)
	{
		strBuf += _T("Btn");
	}
	else if (eControlType == CT_CHECK)
	{
		strBuf += _T("Chk");
	}
	else
	{
		ASSERT(FALSE);
	}
	strBuf += pCtrlProp->GetVariableName();

	return strBuf;
}

CString CEventHandlerGen::CombineButtonHandlerDeclare(LPCSTR lpszClassName, const CTRLPROPERTIES *pCtrlProp)
{
	CString strBuf;

	strBuf += CombineButtonHandlerName(lpszClassName, pCtrlProp);
	strBuf += _T("(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)");

	return strBuf;
}

int CEventHandlerGen::GenerateEventHandlerFile()
{
	if (m_pUIProp == NULL)
	{
		return -1;
	}

	USES_CONVERSION;
	Release();

	InitProjectGUID();

	GenerateHFile();
	GenerateCPPFile();

	return 0;
}
