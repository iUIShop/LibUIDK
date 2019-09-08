#pragma once


//////////////////////////////////////////////////////////////////////////
// Generate a *.rc file to store skins.

class CGenerateResource
{
public:
	CGenerateResource();
	~CGenerateResource();

protected:
	int GetUIFileList(std::vector<CString> *pvImageList, std::vector<CString> *pvBmpList);

public:
	int SetSource(LPCTSTR lpszSource);
	int SetLanguage(int nLanguage);
	int RelateSavePath(BOOL bRelateSavePath);
	int SetCustomRelatePath(LPCTSTR lpszPath);

	int GeneralResource();

protected:
	CString m_strSource;
	int m_nLanguage;	// 0: Englist (United States); 1: Chinese (PRC).
	BOOL m_bRelateSavePath;	// TRUE: the path of file in rc file is file full path remove save path. 
							//  if the file path is C:\abc\def\1.png, and the save path is C:\abc, the path in rc file is def\1.png.
							// FALSE: directly use the custom path as file path in rc file.
							//  if the custom path is AAA, the path in rc file is AAA\1.png.
	CString m_strCustomRelatePath;	// Only valid while m_bRelateSavePath is FALSE.
};
