#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

std::map<UINT, CString> CDebug::m_mapMsgs;


CDebug::CDebug()
{

}

CDebug::~CDebug()
{

}

//////////////////////////////////////////////////////////////////////////
// protected

// Parse winuser.h
int CDebug::LoadWinUserHeaderFile()
{
	int nRet = 0;

	HKEY hKey = NULL;
	LPCTSTR lpszPath = _T("SOFTWARE\\Microsoft\\VisualStudio\\10.0\\Setup\\VS\\");

	do
	{
		LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszPath, 0, KEY_ALL_ACCESS, &hKey);
		if (lRet != ERROR_SUCCESS)
		{
			nRet = -1;
			break;
		}

		BYTE btPath[MAX_PATH] = {0};
		DWORD dwType = REG_SZ;
		DWORD dwDataLen = 80;

		lRet = RegQueryValueEx(hKey, _T("VS"), NULL, &dwType, btPath, &dwDataLen);

		if (lRet != ERROR_SUCCESS)
		{
			nRet = -2;
			break;
		}
	}
	while (0);

	RegCloseKey(hKey);

	return nRet;
}

//////////////////////////////////////////////////////////////////////////
// public

LPCTSTR CDebug::GetMessageText(UINT uMsg)
{
	// If the map is empty, init the map
	if (m_mapMsgs.empty())
	{
		LoadWinUserHeaderFile();
	}

	// Find
	std::map<UINT, CString>::iterator it = m_mapMsgs.find(uMsg);
	if (it == m_mapMsgs.end())
	{
		return _T("");
	}

	return it->second;
}
