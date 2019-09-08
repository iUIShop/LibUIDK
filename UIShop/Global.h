#pragma once



extern LPCTSTR g_lpszClassWizardSection;
extern LPCTSTR g_lpszLocationKey;
extern LPCTSTR g_lpszClassName;
extern LPCTSTR g_lpszClassHFile;
extern LPCTSTR g_lpszClassCppFile;
extern LPCTSTR g_lpszCodeStyleVC6;
extern LPCTSTR g_lpszMergeToolPath;

extern LPCTSTR g_lpszEventHandlerWizardSection;

void IUI_AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn, CDocTemplate* pTemplate, CString* pstrDefaultExt);
void _AfxAppendFilterSuffix2(const CString &strFilterExt, const CString &strFilterName, CString& filter, OPENFILENAME& ofn, CString* pstrDefaultExt);

//
// The following interface include \t pre and ;\r\n ending.
//

// m_pBtn1 = NULL;
int GenerateVariableInit(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA);

// BOOL m_bFlag;
int GenerateDoDataExchangeVariableDeclare(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA);

// ON_BN_CLICKED(IDC_BTN_1, OnBtn1)
int GenerateButtonHandlerMsgMap(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA);

// void CYourWnd::OnBtn1() { AfxMessage(_T("ID"); }
int GenerateHandlerImplementation(const CTRLPROPERTIES *pCtrlProp, const CStringA &strClassName, CStringA &strBufA);

// CSkinButton *m_pBtn1;
int GenerateVariableDeclare(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA);

// m_pBtn1 = (CSkinButton *)GetDlgItem(IDC_BTN_1);
// m_pRc1 = GetRectChild(IDC_WL_RECT_1);
// bAddCaller: TRUE: add "pUIWnd->"
int GenerateVariableAssignString(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA, BOOL bAddCaller);

// afx_msg void OnBtnXXX();
int GenerateHandlerDeclareString(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA);

// Generate *.h file by class name
CString GenerateHFile(LPCTSTR lpszClassName);

// Generate *.cpp file by class name;
CString GenerateCppFile(LPCTSTR lpszClassName);

CString GetRelativePath(const CString &strFrom, const CString &strTo);


#define VIEW_WIDTH_MAX					2200
#define VIEW_HEIGHT_MAX					1400
