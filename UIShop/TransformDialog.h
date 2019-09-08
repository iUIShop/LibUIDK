#pragma once

class CTransformDialog
{
public:
	CTransformDialog(void);
	~CTransformDialog(void);

protected:
	int TransformExe(LPCTSTR lpszSourcePath);
	int TransformRC(LPCTSTR lpszSourcePath);
	int TransformDsp(LPCTSTR lpszSourcePath);

	// Load from .RC file
	int LoadResourceFile(LPCTSTR lpszResource);
	int SetTarget(LPCTSTR lpszTarget);
	int Translate();
	int ParseDlgStyle(const CString &strStyles, /*[out]*/ DWORD &dwStyle);
	int ParseDlgExStyle(const CString &strExStyles, /*[out]*/ DWORD &dwExStyle);
	int ParseDlgFont(const CString &strFont, LOGFONT *plf);
	int MapRect(const LOGFONT &lf, RECT &rect);
	int ParseChildren(UIWNDPROPERTIES *pUIWnd, const LOGFONT &lf);
	int ParseChild(const CString &strLine, CString &strCaption, CString &strID, LPRECT lpRect, DWORD &dwStyle);
	int ParseChildWithoutCpation(const CString &strLine, CString &strID, LPRECT lpRect, DWORD &dwStyle);
	int LoadIDDefined(LPCTSTR lpszID, LPCTSTR lpszType);
	int StringIDToInt(LPCTSTR lpszIDs);
	int PreReadNextLine(CString &str);	// Pre-read next line, if the next line is need combined with current line, combine it.
	int ParseCONTROLControl(const CString &str, UIWNDPROPERTIES *pUIWnd, const LOGFONT &lf);
	int ParseStyleString(const CString &strStyles, DWORD &dwStyle);

public:
	int Transform(LPCTSTR lpszSourcePath, LPCTSTR lpszXUITargetPath);	// Support .EXE and .RC to generate .XUI file.

protected:
	CString m_strXUITargetPath;
	int m_nCurrentLine;

	BOOL m_bHasTabControl;
	BOOL m_bHasGroupBox;

private:
	CMemStdioFile m_ResourceFile;

	// Parse header file.
	std::vector<std::pair<CString, int> > m_vIDs;
	CMemStdioFile m_fileStyleDefined;				// Get by parse WinUser.h file

public:
	// The list of dialog
	std::vector<UIWNDPROPERTIES *> m_vpWindows;
	std::vector<CString> m_vNotSupportedControlType;
};
