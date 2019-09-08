#include "StdAfx.h"
#include "Global.h"
#include "resource.h"
#include "ProjectGen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LPCTSTR g_lpszClassWizardSection = _T("ClassWizard");
LPCTSTR g_lpszLocationKey = _T("Location");
LPCTSTR g_lpszClassName = _T("ClassName");
LPCTSTR g_lpszClassHFile = _T("ClassHFile");
LPCTSTR g_lpszClassCppFile = _T("ClassCppFile");
LPCTSTR g_lpszCodeStyleVC6 = _T("CodeStyleVC6");
LPCTSTR g_lpszMergeToolPath = _T("MergeToolPath");

LPCTSTR g_lpszEventHandlerWizardSection = _T("EventHandlerWizard");

void IUI_AfxAppendFilterSuffix(CString &filter, OPENFILENAME &ofn, CDocTemplate *pTemplate, CString *pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
		!strFilterExt.IsEmpty() &&
		pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
		!strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

void _AfxAppendFilterSuffix2(const CString &strFilterExt, const CString &strFilterName, CString &filter, OPENFILENAME &ofn, CString *pstrDefaultExt)
{
	if (!strFilterExt.IsEmpty() && !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

// m_pBtn1 = (CSkinButton *)GetDlgItem(IDC_BTN_1);
// m_pRc1 = GetRectChild(IDC_WL_RECT_1);
// bAddCaller: TRUE: add "pUIWnd->"
int GenerateVariableAssignString(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA, BOOL bAddCaller)
{
	if (pCtrlProp == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	CString strBuf;

	strBuf = '\t';
	strBuf += CClassGen::GetVariableName(pCtrlProp);
	CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (eControlType == CT_WL_RECTCTRL)
	{
		strBuf += _T(" = ");
	}
	else
	{
		strBuf += _T(" = (");
		strBuf += pCtrlProp->GetClassName();
	}

	if (eControlType == CT_WL_RECTCTRL)
	{
		if (bAddCaller)
		{
			strBuf += _T("pUIWnd->");
		}

		strBuf += _T("GetRectChild(");
	}
	else if (eControlType == CT_WL_LINE
		|| eControlType == CT_WL_TEXT
		|| eControlType == CT_WL_BUTTON
		|| eControlType == CT_WL_CHECK
		|| eControlType == CT_WL_RADIO
		|| eControlType == CT_WL_SLIDER
		|| eControlType == CT_WL_PICTURE
		|| eControlType == CT_WL_RICHEDIT
		|| eControlType == CT_WL_RICHEDIT_IM
		|| eControlType == CT_WL_SPLITTER
		|| eControlType == CT_TASK_WND_MGR
		|| eControlType == CT_PNL_DOCK
		|| eControlType == CT_UNIFORM_GRID
		|| eControlType == CT_PNL_STACK)
	{
		strBuf += _T(" *)");

		if (bAddCaller)
		{
			strBuf += _T("pUIWnd->");
		}

		strBuf += _T("GetDlgWLItem(");
	}
	else
	{
		strBuf += _T(" *)");

		if (bAddCaller)
		{
			strBuf += _T("pUIWnd->");
		}

		strBuf += _T("GetDlgItem(");
	}

	strBuf += pCtrlProp->m_strID;
	strBuf += _T(");\r\n");

	// ASSERT
	strBuf += '\t';
	strBuf += _T("ASSERT(");
	strBuf += CClassGen::GetVariableName(pCtrlProp);
	if (eControlType != CT_WL_RECTCTRL
		&& eControlType != CT_WL_SPLITTER
		&& eControlType != CT_WL_LINE
		&& eControlType != CT_WL_TEXT
		&& eControlType != CT_WL_BUTTON
		&& eControlType != CT_WL_CHECK
		&& eControlType != CT_WL_RADIO
		&& eControlType != CT_WL_SLIDER
		&& eControlType != CT_WL_PICTURE
		&& eControlType != CT_WL_RICHEDIT
		&& eControlType != CT_WL_RICHEDIT_IM
		&& eControlType != CT_TASK_WND_MGR
		&& eControlType != CT_PNL_DOCK
		&& eControlType != CT_UNIFORM_GRID
		&& eControlType != CT_PNL_STACK)
	{
		strBuf += _T("->GetSafeHwnd()");
		strBuf += _T(" != NULL");
	}
	else
	{
		strBuf += _T("->IsCreated()");
	}
	strBuf += _T(");\r\n");

	strBufA = (LPCSTR)_bstr_t(strBuf);

	return 0;
}

int GenerateVariableInit(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA)
{
	strBufA = '\t';
	strBufA += CClassGen::GetVariableName(pCtrlProp);
	strBufA += _T(" = NULL;\r\n");

	return 0;
}

// CString m_str1;
int GenerateDoDataExchangeVariableDeclare(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA)
{
	CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;

	if (eControlType != CT_EDIT && eControlType != CT_CHECK && eControlType != CT_COMBOBOX)
	{
		return -1;
	}

	strBufA = '\t';
	if (eControlType == CT_EDIT || eControlType == CT_COMBOBOX)
	{
		strBufA += "CString m_str";
	}
	else if (eControlType == CT_CHECK)
	{
		strBufA += "BOOL m_b";
	}

	strBufA += pCtrlProp->GetVariableName();
	strBufA += ";\r\n";

	return 0;
}

// ON_BN_CLICKED(IDC_BTN_1, OnBtn1)
int GenerateButtonHandlerMsgMap(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA)
{
	CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (eControlType != CT_PUSHBUTTON && eControlType != CT_CHECK)
	{
		return -1;
	}

	strBufA = '\t';
	strBufA += _T("ON_BN_CLICKED(");
	strBufA += pCtrlProp->m_strID;
	strBufA += _T(", On");
	if (eControlType == CT_PUSHBUTTON)
	{
		strBufA += _T("Btn");
	}
	else if (eControlType == CT_CHECK)
	{
		strBufA += _T("Chk");
	}
	else
	{
		ASSERT(FALSE);
	}
	strBufA += pCtrlProp->GetVariableName();
	strBufA += _T(")\r\n");

	return 0;
}

// void CYourWnd::OnBtn1() { AfxMessage(_T("ID"); }
int GenerateHandlerImplementation(const CTRLPROPERTIES *pCtrlProp, const CStringA &strClassName, CStringA &strBufA)
{
	CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (eControlType != CT_PUSHBUTTON && eControlType != CT_CHECK)
	{
		return -1;
	}

	strBufA = _T("void ");
	strBufA += strClassName;
	strBufA += _T("::On");
	if (eControlType == CT_PUSHBUTTON)
	{
		strBufA += _T("Btn");
	}
	else if (eControlType == CT_CHECK)
	{
		strBufA += _T("Chk");
	}

	strBufA += pCtrlProp->GetVariableName();
	strBufA += _T("()\r\n{\r\n");
	strBufA += _T("\tAfxMessageBox(_T(\"");
	strBufA += pCtrlProp->m_strID;
	strBufA += _T("\"));\r\n}\r\n");

	return 0;
}

// CSkinButton *m_pBtn1;
int GenerateVariableDeclare(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA)
{
	if (pCtrlProp == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp->m_strID == _T("IDC_STATIC"))	// IDC_STATIC should not generate variable
	{
		return -2;
	}

	CString strBuf;

	strBuf = '\t';
	strBuf += pCtrlProp->GetClassName();
	strBuf += _T(" *");
	strBuf += CClassGen::GetVariableName(pCtrlProp);
	strBuf += _T(";\r\n");
	strBufA = (LPCSTR)_bstr_t(strBuf);

	return 0;
}

// afx_msg void OnBtnXXX();
int GenerateHandlerDeclareString(const CTRLPROPERTIES *pCtrlProp, CStringA &strBufA)
{
	if (pCtrlProp == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	CString strBuf;

	CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (eControlType != CT_PUSHBUTTON && eControlType != CT_CHECK)
	{
		return -1;
	}

	strBuf = '\t';
	strBuf += _T("afx_msg void On");
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
	strBuf += _T("();\r\n");
	strBufA = (LPCSTR)_bstr_t(strBuf);

	return 0;
}

// Generate *.h file by class name
CString GenerateHFile(LPCTSTR lpszClassName)
{
	CString strHFile = lpszClassName;

	if ((strHFile.GetLength() >= 1 && strHFile[0] == 'C'))
	{
		strHFile.Delete(0);
	}

	if (strHFile.GetLength() > 0)
	{
		strHFile += _T(".h");
	}

	return strHFile;
}

// Generate *.cpp file by class name;
CString GenerateCppFile(LPCTSTR lpszClassName)
{
	CString strCppFile = lpszClassName;

	if ((strCppFile.GetLength() >= 1 && strCppFile[0] == 'C'))
	{
		strCppFile.Delete(0);
	}

	if (strCppFile.GetLength() > 0)
	{
		strCppFile += _T(".cpp");
	}

	return strCppFile;
}

CString GetRelativePath(const CString &strFrom, const CString &strTo)
{
	CString strRet;

	TCHAR szRelativePath[MAX_PATH] = {0};
	BOOL bRet = PathRelativePathTo(szRelativePath, strFrom, FILE_ATTRIBUTE_DIRECTORY, strTo, FILE_ATTRIBUTE_DIRECTORY);
	if (bRet)
	{
		strRet = szRelativePath;
		if (strRet.Left(1) == '\\')
		{
			strRet.Delete(0);
		}
	}
	else
	{
		strRet = strTo;
	}

	return strRet;
}
