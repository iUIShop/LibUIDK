#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CIMRichEditCtrl::CIMRichEditCtrl()
	: CIMRichEditCtrlBase(false, this)
{

}

CIMRichEditCtrl::~CIMRichEditCtrl()
{

}

int CIMRichEditCtrl::OnDraw(CDC *pMemDCParent)
{
	// 绘制非RichText内容，包括头像和气泡。
	// 在得到气泡和头像坐标的时候，为只读模式，不再设置段落格式
	OnDrawNonRichText(pMemDCParent);

	// 绘制Rich Text
	return OnDrawRichText(pMemDCParent);
}

void CIMRichEditCtrl::DoIMSize(UINT nType, int cx, int cy)
{
	// 设置段落格式
	DoSetParaFormat();
}

LRESULT CIMRichEditCtrl::DoIMVScroll(WPARAM wParam, LPARAM lParam)
{
	// 如果在WM_VSCROLL的时候，调用DoSetVisibleParaFormat设置段落格式后计算气泡的坐标
	// 则在滚动结束后，如果可见的第一行是设置了左缩进的段落中的某一行，
	// 滚动位置会修正到以行为步进的位置，否则会停留在以像素为步进的位置。
	// 虽然WM_SIZE的响应函数OnRichEditSize也是调用DoSetVisibleParaFormat，
	// 但WM_SIZE中设置段落格式，就不会有这个问题。
	// 所以，在WM_VSCROLL中，只计算气泡坐标，不设置段落格式。

	// 怀疑：设置段落格式，并不能永久导致垂直滚动时，以“行”为步进，否则WM_SIZE中也设置过段落格式，
	// 设置之后，仍然是以像素为步进的。 也就是说设置段落格式，导致RichEdit把第一个可见行修正到行的top
	// 坐标这种行为，只是针对这一次设置。之所以WM_VSCROLL中设置了段落格式后，变成以“行”为步进，估计
	// 是因为，WM_VSCROLL在垂直滚动过程中，是不断被响应的，这就会不断导致RichEdit把第一个可见行修正
	// 到行的top坐标。 这样看上去，就是按行滚动了。

	// 所以，计算头像和气泡坐标的工作，放到WM_PAINT中进行，这样，不认论是WM_SIZE、WM_VSCROOL还是其它
	// 操作导致的刷新，都会计算气泡坐标。

	return 0;
}

LRESULT CIMRichEditCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_VSCROLL)
	{
		DoIMVScroll(wParam, lParam);
	}
	else if (message == WM_SIZE)
	{
		DoIMSize(wParam, LOWORD(lParam), HIWORD(lParam));
	}
	else if (message == WM_DESTROY)
	{
		CIMRichEditCtrlBase::ReleaseObject();
	}

	return CSkinRichEditCtrl::WindowProc(message, wParam, lParam);
}
