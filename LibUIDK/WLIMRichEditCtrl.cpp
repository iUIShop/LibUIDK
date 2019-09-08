#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWLIMRichEditCtrl::CWLIMRichEditCtrl()
	: CIMRichEditCtrlBase(true, (CWnd *)this)
{

}

CWLIMRichEditCtrl::~CWLIMRichEditCtrl()
{

}

int CWLIMRichEditCtrl::OnDraw(CDC *pMemDCParent)
{
	OnDrawNonRichText(pMemDCParent);
	return OnDrawRichText(pMemDCParent);
}

void CWLIMRichEditCtrl::DoIMSize(UINT nType, int cx, int cy)
{
	// …Ë÷√∂Œ¬‰∏Ò Ω
	DoSetParaFormat();
}

LRESULT CWLIMRichEditCtrl::DoIMVScroll(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
