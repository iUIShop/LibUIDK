// ProjectGen.h: interface for the CProjectGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTGEN_H__625A6D78_9033_42B1_AB51_903A803CB12F__INCLUDED_)
#define AFX_PROJECTGEN_H__625A6D78_9033_42B1_AB51_903A803CB12F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

struct StringToString
{
	CStringA strMacro;
	CStringA strMacroValue;
};

struct Condition
{
	BOOL bIFSection;
	BOOL bNeedNotOperator;
	BOOL bIncludeOrOperator;
	vector<StringToString> vMacros;
};

struct COPYFILE
{
	BOOL bFile;
	BOOL bTranscode; // only valid when bFile is TRUE;
	CStringA strSource;
	CStringA strTarget;
	vector<Condition> vConditions;
};

class CProjectGen
{
public:
	CProjectGen();
	virtual ~CProjectGen();
	virtual int Release();

public:
	int LoadTemplate(LPVOID pData, DWORD dwSize);
	int ProcessTemplate();
	int LoadCustomResource(LPCSTR lpszResName, LPVOID *lpData, DWORD *pdwSize);
	int CopyResDataToProjectFile(LPCSTR lpszResName, LPCSTR lpszTarget, BOOL bTranscode);
	int InitFILE_NAME_SYMBOL(LPCSTR lpszImageName);
	int InitProjectGUID();

	int SetInputFolder(LPCTSTR lpszFolder);
	CStringA GetInputFolder();

	int SetProjectName(LPCTSTR lpszPrjName);
	CStringA GetProjectName();

	virtual CStringA GetProjectFolder();

	// In NewProj.inf file
	int ParseIFSectionInNewProjFile(FILE *fp);
	int ParseIFStringInNewProjFile(LPCSTR lpszIF);
	int ParseELIFStringInNewProjFile(LPCSTR lpszELIF);
	virtual int CustomConfition(FILE *fpTarget);
	int GenerateCOPYFILE(LPCSTR lpszLine);

	// In template file
	int ParseIFStringInTemplateFile(LPCSTR lpszIF);
	int ParseIFSectionInTemplateFile(FILE *fpSource, FILE *fpTarget);

	int ParseString(LPCSTR lpszSource, CStringA &rstrTarget);
	BOOL IsHasMacro(LPCSTR lpszBuf, int *pnFirst, int *pnSecond, CStringA &rstrMacro);
	int RemoveLastIF();

	// String
	BOOL IsIncludeOrOperator(LPCSTR lpMacros);
	BOOL ExtractSubString(CStringA& rString, LPCSTR lpszFullString, int iSubString, CHAR *szSep);

public:
	CMapStringToString m_Dictionary;

protected:
	CStringA m_strInputFolder;
	CStringA m_strProjectFolder;
	CStringA m_strProjectName;

	vector<Condition> m_vConditions;
	vector<COPYFILE> m_vFiles;
};

class CClassGen : public CProjectGen
{
public:
	CClassGen();
	virtual ~CClassGen();
	virtual int Release();

public:
	int SetClassName(LPCTSTR lpszClassName);
	CStringA GetClassName();
	virtual CStringA GetProjectFolder();
	int SetUIWndProperties(const UIWNDPROPERTIES *pUIProp);

	static CString GetVariableName(const CTRLPROPERTIES *pCtrlProp);

	virtual int CustomConfition(FILE *fpTarget);
	virtual int GenerateVariableDeclare(FILE *fpTarget);
	virtual int GenerateVariableInit(FILE *fpTarget);
	virtual int GenerateVariableAssign(FILE *fpTarget, BOOL bAddCaller);
	virtual int GenerateHandlerDeclare(FILE *fpTarget);
	virtual int GenerateHandlerMsgMap(FILE *fpTarget);
	virtual int GenerateHandlerImplementation(FILE *fpTarget);
	virtual int GenerateInitControlsDeclare(FILE *fpTarget);
	virtual int GenerateInitControlsImplementation(FILE *fpTarget);
	virtual int GenerateCallInitControls(FILE *fpTarget);
	virtual int GenerateDoDataExchangeVariableDeclare(FILE *fpTarget);
	virtual int GenerateDoDataExchangeVariableInit(FILE *fpTarget);
	virtual int GenerateDoDataExchangeVariableBind(FILE *fpTarget);

protected:
	CStringA m_strClassName;
	const UIWNDPROPERTIES *m_pUIProp;
};


class CEventHandlerGen : public CClassGen
{
public:
	CEventHandlerGen();
	~CEventHandlerGen();

protected:
	virtual int GenerateHFile();
	virtual int GenerateCPPFile();
	virtual CStringA CombineHanlerMapVariable(LPCSTR lpszClassName);
	virtual CStringA CombineInitFunction(LPCSTR lpszClassName);
	virtual CStringA GenerateClassDefine(LPCSTR lpszClassName);
	virtual CString CombineButtonHandlerName(LPCSTR lpszClassName, const CTRLPROPERTIES *pCtrlProp);
	virtual CString CombineButtonHandlerDeclare(LPCSTR lpszClassName, const CTRLPROPERTIES *pCtrlProp);

public:
	virtual int GenerateEventHandlerFile();
};


#endif // !defined(AFX_PROJECTGEN_H__625A6D78_9033_42B1_AB51_903A803CB12F__INCLUDED_)
