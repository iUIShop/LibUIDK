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
	// ���Ʒ�RichText���ݣ�����ͷ������ݡ�
	// �ڵõ����ݺ�ͷ�������ʱ��Ϊֻ��ģʽ���������ö����ʽ
	OnDrawNonRichText(pMemDCParent);

	// ����Rich Text
	return OnDrawRichText(pMemDCParent);
}

void CIMRichEditCtrl::DoIMSize(UINT nType, int cx, int cy)
{
	// ���ö����ʽ
	DoSetParaFormat();
}

LRESULT CIMRichEditCtrl::DoIMVScroll(WPARAM wParam, LPARAM lParam)
{
	// �����WM_VSCROLL��ʱ�򣬵���DoSetVisibleParaFormat���ö����ʽ��������ݵ�����
	// ���ڹ�������������ɼ��ĵ�һ�����������������Ķ����е�ĳһ�У�
	// ����λ�û�����������Ϊ������λ�ã������ͣ����������Ϊ������λ�á�
	// ��ȻWM_SIZE����Ӧ����OnRichEditSizeҲ�ǵ���DoSetVisibleParaFormat��
	// ��WM_SIZE�����ö����ʽ���Ͳ�����������⡣
	// ���ԣ���WM_VSCROLL�У�ֻ�����������꣬�����ö����ʽ��

	// ���ɣ����ö����ʽ�����������õ��´�ֱ����ʱ���ԡ��С�Ϊ����������WM_SIZE��Ҳ���ù������ʽ��
	// ����֮����Ȼ��������Ϊ�����ġ� Ҳ����˵���ö����ʽ������RichEdit�ѵ�һ���ɼ����������е�top
	// ����������Ϊ��ֻ�������һ�����á�֮����WM_VSCROLL�������˶����ʽ�󣬱���ԡ��С�Ϊ����������
	// ����Ϊ��WM_VSCROLL�ڴ�ֱ���������У��ǲ��ϱ���Ӧ�ģ���ͻ᲻�ϵ���RichEdit�ѵ�һ���ɼ�������
	// ���е�top���ꡣ ��������ȥ�����ǰ��й����ˡ�

	// ���ԣ�����ͷ�����������Ĺ������ŵ�WM_PAINT�н��У���������������WM_SIZE��WM_VSCROOL��������
	// �������µ�ˢ�£���������������ꡣ

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
		DoIMSize((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
	}
	else if (message == WM_DESTROY)
	{
		CIMRichEditCtrlBase::ReleaseObject();
	}

	return CSkinRichEditCtrl::WindowProc(message, wParam, lParam);
}
