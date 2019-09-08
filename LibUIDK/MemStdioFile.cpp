// MemStdioFile.cpp: implementation of the CMemStdioFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlconv.h>
#include <comutil.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemStdioFile::CMemStdioFile()
{

}

CMemStdioFile::~CMemStdioFile()
{

}

BOOL CMemStdioFile::OpenResource(LPCTSTR lpszID, LPCTSTR lpszType)
{
	HRSRC hRsrc = FindResource(NULL, lpszID, lpszType);
	if (NULL == hRsrc)
	{
		return FALSE;
	}

	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (0 == dwSize)
	{
		return FALSE;
	}

	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		return FALSE;
	}

	LPVOID pBuffer = LockResource(hGlobal);
	if (NULL == pBuffer)
	{
		return FALSE;
	}

	BYTE *pBuf = new BYTE[dwSize + 2];
	memset(pBuf, 0, dwSize + 2);
	memcpy(pBuf, pBuffer, dwSize);
	Attach((BYTE *)pBuf, dwSize + 2, 100);

	return TRUE;
}

BOOL CMemStdioFile::IsUnicode()
{
	if (m_lpBuffer[0] == 0xFF && m_lpBuffer[1] == 0xFE)	// Unicode
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CMemStdioFile::ReadString(CString &rString)
{
	// check unicode
	if (m_lpBuffer[0] == 0xFF && m_lpBuffer[1] == 0xFE)	// Unicode
	{
		std::wstring str;
		if (!ReadStringW(str))
		{
			return FALSE;
		}
		rString = (TCHAR *)_bstr_t(str.c_str());
	}
	else
	{
		std::string str;
		if (!ReadStringA(str))
		{
			return FALSE;
		}
		rString = (TCHAR *)_bstr_t(str.c_str());
	}

	return TRUE;
}

BOOL CMemStdioFile::ReadStringA(std::string &rstring)
{
#if(_MSC_VER <= 1200) // VC6.0
	DWORD dwPos = GetPosition();
#else
	ULONGLONG dwPos = GetPosition();
#endif
	if (dwPos == m_nBufferSize)
	{
		return FALSE;
	}

	BOOL bRet = AfxExtractSubStringA(rstring, (LPCSTR)m_lpBuffer + dwPos, int(m_nBufferSize - dwPos), 0, '\r');

	if (bRet)
	{
		size_t nAdd = rstring.length() + 1;
		if (dwPos + nAdd > m_nBufferSize)
		{
			Seek(0, CFile::end);
		}
		else
		{
			Seek(dwPos + nAdd, CFile::begin);
		}
	}
	else
	{
		Seek(0, CFile::end);
	}

	return bRet;
}

BOOL CMemStdioFile::ReadStringW(std::wstring &rstring)
{
#if(_MSC_VER <= 1200) // VC6.0
	DWORD dwPos = GetPosition();
#else
	ULONGLONG dwPos = GetPosition();
#endif
	if (dwPos == m_nBufferSize)
	{
		return FALSE;
	}

	BOOL bRet = AfxExtractSubStringW(rstring, (LPCWSTR)((LPCSTR)m_lpBuffer + dwPos), int(m_nBufferSize - dwPos), 0, '\r');

	if (bRet)
	{
		size_t nAdd = (rstring.length() + 1) * (sizeof(WCHAR) / sizeof(CHAR));
		if (dwPos + nAdd > m_nBufferSize)
		{
			Seek(0, CFile::end);
		}
		else
		{
			Seek(dwPos + nAdd, CFile::begin);
		}
	}
	else
	{
		Seek(0, CFile::end);
	}

	return bRet;
}
