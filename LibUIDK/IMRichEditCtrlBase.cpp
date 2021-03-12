#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IMRE_CALL_FUN(fun) \
	if (m_bWindowlessMode) \
	{\
		((CWLIMRichEditCtrl *)m_pIMReBind)->fun;\
	}\
	else\
	{\
		((CIMRichEditCtrl *)m_pIMReBind)->fun;\
	}\

#define IMRE_CALL_FUN_RETURN(nRet, fun) \
	if (m_bWindowlessMode) \
	{\
		(nRet) = ((CWLIMRichEditCtrl *)m_pIMReBind)->fun;\
	}\
	else\
	{\
		(nRet) = ((CIMRichEditCtrl *)m_pIMReBind)->fun;\
	}\


CIMMsg::CIMMsg()
{
	m_nVer = 1000;
	m_nMsgID = 1;
	m_bSelf = false;
	m_himgFace = NULL;
	m_nMsgSenderTitle = 1;
	m_pImreHost = NULL;
	m_nMsgParagraphCount = 1;

	m_bGroup = false;
}

CIMMsg::CIMMsg(const CIMMsg &rSrc)
{
	Copy(rSrc);
}

CIMMsg::~CIMMsg()
{
	int nRichMsgItemCount = (int)m_vpStrRichMsg.size();
	for (int i = nRichMsgItemCount - 1; i >= 0; --i)
	{
		CRichMsgItem *pMsgItem = m_vpStrRichMsg[i];
		SafeDelete(pMsgItem);
	}
}

CIMMsg &CIMMsg::operator=(const CIMMsg &rSrc)
{
	if (&rSrc == this)
	{
		return *this;
	}

	Copy(rSrc);
	return *this;
}

int CIMMsg::GetVer() const
{
	return m_nVer;
}

int CIMMsg::SetMsgID(int nID)
{
	m_nMsgID = nID;

	return 0;
}

int CIMMsg::GetMsgID() const
{
	return m_nMsgID;
}

int CIMMsg::SetSelf(BOOL bSelf)
{
	m_bSelf = bSelf;

	return 0;
}

BOOL CIMMsg::IsSelf() const
{
	return m_bSelf;
}

int CIMMsg::SetFace(HIUIIMAGE hImgFace)
{
	m_himgFace = hImgFace;

	return 0;
}

HIUIIMAGE CIMMsg::GetFace() const
{
	return m_himgFace;
}

int CIMMsg::SetMsgSenderTitle(int nTitle)
{
	m_nMsgSenderTitle = nTitle;

	return 0;
}

int CIMMsg::GetMsgSenderTitle() const
{
	return m_nMsgSenderTitle;
}

int CIMMsg::SetMsgSender(LPCTSTR lpszSender)
{
	m_strMsgSender = lpszSender;

	return 0;
}

CString CIMMsg::GetMsgSender() const
{
	return m_strMsgSender;
}

int CIMMsg::SetMsgStart(int nStart)
{
	m_nMsgStartCp = nStart;

	return 0;
}

int CIMMsg::GetMsgStart() const
{
	return m_nMsgStartCp;
}

int CIMMsg::SetMsgContentStart(int nStart)
{
	m_nMsgContentStartCp = nStart;

	return 0;
}

int CIMMsg::GetMsgContentStart() const
{
	return m_nMsgContentStartCp;
}

int CIMMsg::SetMsgEnd(int nEnd)
{
	m_nMsgEndCp = nEnd;

	return 0;
}

int CIMMsg::GetMsgEnd() const
{
	return m_nMsgEndCp;
}

int CIMMsg::SetMsgParagraphCount(int nCount)
{
	m_nMsgParagraphCount = nCount;

	return 0;
}

int CIMMsg::GetMsgParagraphCount() const
{
	return m_nMsgParagraphCount;
}

LPARAM CIMMsg::SetUserData(LPARAM lParam)
{
	LPARAM lOldParam = m_lParam;
	m_lParam = lParam;
	return lOldParam;
}

LPARAM CIMMsg::GetUserData() const
{
	return m_lParam;
}

int CIMMsg::AddMsgItem(const CRichMsgItem *pMsgItem)
{
	if (pMsgItem == NULL)
	{
		return -1;
	}

	CRichMsgItem *pNewMsgItem = new CRichMsgItem;
	*pNewMsgItem = *pMsgItem;

	// 普通消息，把换行符换成\r
	if (pNewMsgItem->m_strProtocol.IsEmpty())
	{
		// 把消息中的\r\n替换成\r。 然后把\n也替换为\r。 因为RichEdit只认\r
		pNewMsgItem->m_strMsgContent.Replace(_T("\r\n"), _T("\r"));
		pNewMsgItem->m_strMsgContent.Replace(_T("\n"), _T("\r"));

		// 记录消息中有多少个\r（段落）
		int nMsgLength = pNewMsgItem->m_strMsgContent.GetLength();
		for (int i = 0; i < nMsgLength; ++i)
		{
			if (pNewMsgItem->m_strMsgContent[i] == '\r')
			{
				int nOldCount = GetMsgParagraphCount();
				SetMsgParagraphCount(nOldCount + 1);
			}
		}
	}
	else
	{
		// 如果OLE不是图片，统统解析成一条新消息
		if (pNewMsgItem->m_strProtocol != _T("i/:"))
		{
			// 			CIMMsg *pMsgFile = new CIMMsg;
			// 			pMsgFile->SetMsgSenderTitle(GetMsgSenderTitle());
			// 			pMsgFile->SetMsgSender(GetMsgSender());
			// 			pMsgFile->SetFaceIndex(GetFaceIndex());
			// 			pMsgFile->SetSelf(IsSelf());
			// 			// f/:加上guid长度为41
			// 			pMsgFile->SetOrigRichMsg(m_strRichMsg.Mid(nFilePos, 41));
			//
			// 			CRichMsgItem *pMsgItem = new CRichMsgItem;
			// 			pMsgItem->m_strProtocol = _T("f/:");
			// 			pMsgItem->m_strMsg = m_strRichMsg.Mid(nFilePos + 3, 38);
			// 			pMsgFile->m_vpStrRichMsg.push_back(pMsgItem);
			//
			// 			// 置为解析过
			// 			pMsgFile->m_bParsedMsg = true;
			//
			// 			if (m_pImreHost != NULL)
			// 			{
			// 				m_pImreHost->InsertMsg(pMsgFile);
			// 			}
			// 			else
			// 			{
			// 				ASSERT(FALSE);
			// 			}
		}
	}

	m_vpStrRichMsg.push_back(pNewMsgItem);

	return 0;
}

void CIMMsg::Copy(const CIMMsg &rSrc)
{
	// 当培养类成员后，自动断言
	int nSize = sizeof(CIMMsg);
	ASSERT(nSize == 84);

	m_nVer = rSrc.m_nVer;
	m_nMsgID = rSrc.m_nMsgID;
	m_tMsg = rSrc.m_tMsg;
	m_bSelf = rSrc.m_bSelf;
	m_himgFace = rSrc.m_himgFace;
	m_himgFace->AddRef();
	m_nMsgSenderTitle = rSrc.m_nMsgSenderTitle;
	m_strMsgSender = rSrc.m_strMsgSender;
	m_strRichMsg = m_strRichMsg;
	m_lParam = rSrc.m_lParam;

	m_pImreHost = rSrc.m_pImreHost;
	m_bGroup = rSrc.m_bGroup;
	m_nMsgStartCp = rSrc.m_nMsgStartCp;
	m_nMsgContentStartCp = rSrc.m_nMsgContentStartCp;
	m_nMsgEndCp = rSrc.m_nMsgEndCp;
	m_nMsgParagraphCount = rSrc.m_nMsgParagraphCount;

	int nCount = rSrc.m_vpStrRichMsg.size();
	for (int i = 0; i < nCount ; i++)
	{
		const CRichMsgItem *p = rSrc.m_vpStrRichMsg[i];

		CRichMsgItem *pNew = new CRichMsgItem;
		*pNew = *p;
		m_vpStrRichMsg.push_back(pNew);
	}
}

//////////////////////////////////////////////////////////////////////////

CIMRichEditCtrlBase::CIMRichEditCtrlBase(BOOL bWindowlessMode, CWnd *pBindRe)
	: m_bQiPaoMode(TRUE)
	, m_nSenderLeftIndent(0)
	, m_nMsgLeftIndent(0)
	, m_nMsgRightIndent(0)
	, m_nMaxRelayoutWidth(1200)
	, m_bWindowlessMode(bWindowlessMode)
	, m_pIMReBind(pBindRe)
	, m_bShowMsgSenderTitle(TRUE)
	, m_bShowLeftFace(TRUE)
	, m_bShowRightFace(TRUE)
	, m_bCombineMsg(FALSE)
	, m_nCombineMsgTimeSpan(0)
{
	m_sizeFace.cx = m_sizeFace.cy = 32;
}

CIMRichEditCtrlBase::~CIMRichEditCtrlBase(void)
{
}

int CIMRichEditCtrlBase::ReleaseObject()
{
	// 释放从IOleCtrlBase派生的ole资源
	CComPtr<IRichEditOle> pRichEditOle = NULL;
	if (m_bWindowlessMode)
	{
		pRichEditOle = ((CWLRichEditCtrl *)m_pIMReBind)->GetIRichEditOle();
	}
	else
	{
		pRichEditOle = ((CSkinRichEditCtrl *)m_pIMReBind)->GetIRichEditOle();
	}

	LONG nOleCount = pRichEditOle->GetObjectCount();
	int i = 0;
	for (i = nOleCount - 1; i >= 0 ; --i)
	{
		REOBJECT ro;
		ro.cbStruct = sizeof(REOBJECT);
		HRESULT hr = pRichEditOle->GetObject(i, &ro, REO_GETOBJ_ALL_INTERFACES);
		if (FAILED(hr))
		{
			continue;
		}

		// 表示这个ole是从IOleCtrlBase派生的。
		if (ro.clsid == g_guidImageOle)
		{
			IOleCtrlBase *pOle = (IOleCtrlBase *)ro.poleobj;
			pOle->ReleaseObject();

			((CSkinRichEditCtrl *)m_pIMReBind)->SetSel(ro.cp, ro.cp + 1);
			((CSkinRichEditCtrl *)m_pIMReBind)->ReplaceSel(NULL);

			delete pOle;
		}
	}

	// 释放消息
	int nMsgCount = m_vMsg.size();
	for (i = 0; i < nMsgCount; ++i)
	{
		const CIMMsg *pMsg = m_vMsg[i];
		delete pMsg;
	}
	m_vMsg.clear();

	return 0;
}

int CIMRichEditCtrlBase::BindStyle(LPCTSTR lpszStyleID)
{
	if (lpszStyleID == NULL)
	{
		return -2;
	}

	IMREPROPERTIES *pCtrlProp = (IMREPROPERTIES *)CUIMgr::GetStyleItem(STYLET_IMRICHEDIT, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CIMRichEditCtrlBase::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_IMRICHEDIT
		&& ect != CT_STYLE_IMRICHEDIT
		&& ect != CT_WL_RICHEDIT_IM
		&& ect != CT_RICHEDIT_IM)
	{
		return -3;
	}

	IMREPROPERTIES *pRichEditProp = (IMREPROPERTIES *)pCtrlProp;

	// font and color
	CString strResFontID;
	CTRLPROPERTIES::IUIGetControlFont1(pCtrlProp, &strResFontID);
	HFONT hFont = CreateIUIFont(strResFontID);

	if (hFont == NULL)
	{
		CString strDefaultFont = CUIMgr::GetDefaultFont();
		hFont = CreateIUIFont(strDefaultFont);
		ReleaseIUIFont(strDefaultFont);
	}

	if (hFont != NULL)
	{
		// Text color
		COLORREF crN = RGB(0, 0, 0);
		CTRLPROPERTIES::IUIGetControlColor4(pCtrlProp, &crN, NULL, NULL, NULL);

		LOGFONT lf = {0};
		GetObject(hFont, sizeof(LOGFONT), &lf);

		CHARFORMAT2 cf;
		memset(&cf, 0, sizeof(CHARFORMAT2));
		cf.cbSize = sizeof(CHARFORMAT2);
		cf.dwMask = CFM_FACE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT
			| CFM_SIZE | CFM_COLOR;
		if (lf.lfWeight == FW_BOLD)
		{
			cf.dwEffects = CFE_BOLD;
		}
		if (lf.lfItalic)
		{
			cf.dwEffects |= CFE_ITALIC;
		}
		if (lf.lfUnderline)
		{
			cf.dwEffects |= CFE_UNDERLINE;
		}
		cf.crTextColor = crN;
		cf.yHeight = -GetYTwipsFromPixel(lf.lfHeight);
		TSTRCPY(cf.szFaceName, LF_FACESIZE, lf.lfFaceName);

		IMRE_CALL_FUN(SetDefaultCharFormat(cf));
	}

	// Set bitmaps for background
	bool bSpecifyBackgroundImages = pCtrlProp->m_bSpecifyBackgroundImages;
	if (bSpecifyBackgroundImages)
	{
		// Set resizing point
		CString strImageName[1 + COMBINEIMAGESIZE4];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetBackground4(pCtrlProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			if (m_bWindowlessMode)
			{
				CWLIMRichEditCtrl *pWLIMre = (CWLIMRichEditCtrl *)m_pIMReBind;
				pWLIMre->CRichEditCtrlBase::SetBkCombineImage(strImageName[0]);
			}
			else
			{
				((CIMRichEditCtrl *)m_pIMReBind)->SetBkCombineImage(strImageName[0]);
			}
		}
		else
		{
			IMRE_CALL_FUN(SetImages(CONTROL_STATE_UNCHECKED_ALL,
					strImageName[1], strImageName[2], strImageName[3], strImageName[4]));
		}
	}

	return 0;
}

// 为了性能考虑，调用InsertMsg前，不需要解析消息，因为并不是所有从服务器拉到的消息
// 都会插入到RichEdit。 所以在InsertMsg内部，要解析消息格式。
int CIMRichEditCtrlBase::InsertMsg(const CIMMsg *pMsg)
{
	if (pMsg == NULL)
	{
		return -1;
	}

	CIMMsg *pNewMsg = new CIMMsg;
	*pNewMsg = *pMsg;
	pNewMsg->m_pImreHost = this;

	pNewMsg->m_tMsg = GetCurrentTime();

	// 保存起来
	m_vMsg.push_back(pNewMsg);

	int nCount = (int)m_vMsg.size();

	DoInsertMsg(pNewMsg);

	// 返回索引
	return nCount - 1;
}

int CIMRichEditCtrlBase::DoInsertMsg(const CIMMsg *pMsg)
{
	if (pMsg == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	// 记录插入的开始位置
	IMRE_CALL_FUN(SetSel(-1, -1));
	int nLength = -1;
#if(_MSC_VER <= 1200) // VC6.0
	IMRE_CALL_FUN_RETURN(nLength, GetTextLength());
#else
	IMRE_CALL_FUN_RETURN(nLength, GetTextLengthEx(GTL_DEFAULT | GTL_NUMCHARS));
#endif
	((CIMMsg *)pMsg)->SetMsgStart(nLength);

	//
	// 1 插入【级别】+名字
	//
	BOOL bShowSender = TRUE; // 是否显示消息发送者名字
	if (m_bQiPaoMode)
	{
		if (pMsg->IsSelf())
		{
			bShowSender = FALSE;
		}

		if (!IsShowMsgSenderTitle())
		{
			bShowSender = FALSE;
		}
	}

	CComPtr<ITextDocument> pTextDocument = NULL;
	IMRE_CALL_FUN_RETURN(pTextDocument, GetITextDocument());

	if (bShowSender)
	{
		CString strName;
		strName = _T("【");
		strName += m_mapMsgSenderTitleAlias[pMsg->GetMsgSenderTitle()];
		strName += _T("】 ");
		strName += pMsg->GetMsgSender();
		strName += _T("\r");

		IMRE_CALL_FUN(ReplaceSel(strName));

		// 设置名字的段落格式。只有别人才显示发送者名字，自己是不显示的。
		// 名字永远显示在左侧，所以，只需要在插入消息时，设置名字的段落格式即可，之后都不需要设置。
		ITextRange *pTextRangeSender = NULL;
		int nNameLength = 0;
		IMRE_CALL_FUN_RETURN(nNameLength, LineLength(pMsg->GetMsgStart()));
		pTextDocument->Range(pMsg->GetMsgStart(), pMsg->GetMsgStart() + nNameLength, &pTextRangeSender);

#ifdef _DEBUG
		BSTR bstr;
		pTextRangeSender->GetText(&bstr);
#endif // _DEBUG

		CComPtr<ITextPara> pTextParaSender = NULL;
		pTextRangeSender->GetPara(&pTextParaSender);
		pTextParaSender->SetSpaceBefore(GetXFPPTSFromPixel(15));
		pTextParaSender->SetSpaceAfter(GetXFPPTSFromPixel(15));
		int nSenderIndent = GetSenderIndent();
		pTextParaSender->SetIndents(0, GetXFPPTSFromPixel(nSenderIndent), 0);
		pTextRangeSender->SetPara(pTextParaSender);
	}
	else
	{
		// 自己发送的消息，插入一个空行，否则在显示气泡时，会和前一条消息有重叠（气泡尺寸大于计算尺寸）
		IMRE_CALL_FUN(ReplaceSel(_T("\r")));
	}

#if(_MSC_VER <= 1200) // VC6.0
	IMRE_CALL_FUN_RETURN(nLength, GetTextLength());
#else
	IMRE_CALL_FUN_RETURN(nLength, GetTextLengthEx(GTL_DEFAULT | GTL_NUMCHARS));
#endif
	((CIMMsg *)pMsg)->SetMsgContentStart(nLength - pMsg->GetMsgStart());

	HWND hWnd = NULL;
	if (m_bWindowlessMode)
	{
		hWnd = ((CWLIMRichEditCtrl *)m_pIMReBind)->GetParent()->GetSafeHwnd();
	}
	else
	{
		hWnd = ((CIMRichEditCtrl *)m_pIMReBind)->GetSafeHwnd();
	}

	//
	// 2. 插入消息正文
	//
	int nRichCount = (int)pMsg->m_vpStrRichMsg.size();
	for (int i = 0; i < nRichCount; ++i)
	{
		CRichMsgItem *pRichMsgItem = pMsg->m_vpStrRichMsg[i];

		//
		// 1 表情
		//
		if (!pRichMsgItem->m_strProtocol.IsEmpty())
		{
			if (pRichMsgItem->m_strProtocol == _T("i/:"))
			{
				// 解析是哪个Gif
				TCHAR chB = pRichMsgItem->m_strMsgContent[0];
				TCHAR chX = pRichMsgItem->m_strMsgContent[1];
				TCHAR szX[2] = {chX, 0};
				int nX = IntFromString(szX);
				int nOrigMsgLength = pRichMsgItem->m_strMsgContent.GetLength();

				// 表情有正负号
				CString strIndex;
				int nIndex = 0;
				if (chB == '0')
				{
					if (nOrigMsgLength >= 3 + nX)
					{
						strIndex = pRichMsgItem->m_strMsgContent.Mid(2, nX + 1);
					}
					else
					{
						ASSERT(FALSE);
						break;
					}
				}
				else if (chB == '1')
				{
					if (nOrigMsgLength >= 2 + nX)
					{
						strIndex = pRichMsgItem->m_strMsgContent.Mid(2, nX);
					}
					else
					{
						ASSERT(FALSE);
						break;
					}
				}
				else
				{
					ASSERT(FALSE);
				}
				nIndex = IntFromString(strIndex);

				CImageOleCtrl *pImageCtrl = new CImageOleCtrl(hWnd);

				CString strImage;
				if (nIndex <= -1 && nIndex >= -4)
				{
					CString strGif;
					strGif.Format(_T("%d.gif"), -nIndex);
					strImage = CUIMgr::GetUIPathWithoutTitle() + strGif;
				}

				pImageCtrl->SetImage(strImage);

				int nDPIedWidth = GetDPIWidth(20);
				int nDPIedHeight = GetDPIHeight(20);
				IMRE_CALL_FUN(InsertOle(pImageCtrl, REO_CP_SELECTION, nDPIedWidth, nDPIedHeight));

				continue;
			}
			else
			{
				IOleCtrlBase *pCustomOleCtrl = new IOleCtrlBase(hWnd);
				pCustomOleCtrl->SetIDD(pRichMsgItem->m_uOleWinID);

				pCustomOleCtrl->Create(NULL, NULL, WS_VISIBLE, CWnd::FromHandle(hWnd), 1, NULL);

				int nDPIedWidth = GetDPIWidth(pRichMsgItem->m_size.cx);
				int nDPIedHeight = GetDPIHeight(pRichMsgItem->m_size.cy);
				IMRE_CALL_FUN(InsertOle(pCustomOleCtrl, REO_CP_SELECTION, nDPIedWidth, nDPIedHeight));

				// 通知RichEdit父窗口，插入了一个新的自定义OLE控件
				NMIMRE nmhdrIM;
				nmhdrIM.hdr.idFrom = GetDlgCtrlID(hWnd);
				nmhdrIM.hdr.hwndFrom = hWnd;
				nmhdrIM.hdr.code = EN_ADDED_CUSTOM_OLE;
				nmhdrIM.wParam = (WPARAM)this;
				nmhdrIM.lParam = (LPARAM)pCustomOleCtrl;
				::SendMessage(GetParent(hWnd), WM_NOTIFY, nmhdrIM.hdr.idFrom, (LPARAM)&nmhdrIM);

				continue;
			}
		}
		else
		{
			//
			// 普通文本
			//
			IMRE_CALL_FUN(ReplaceSel(pRichMsgItem->m_strMsgContent));
		}
	}

	// 记录插入的结束位置
#if(_MSC_VER <= 1200) // VC6.0
	IMRE_CALL_FUN_RETURN(nLength, GetTextLength());
#else
	IMRE_CALL_FUN_RETURN(nLength, GetTextLengthEx(GTL_DEFAULT | GTL_NUMCHARS));
#endif
	((CIMMsg *)pMsg)->SetMsgEnd(nLength - pMsg->GetMsgStart());

	//
	// 3. 设置消息体的段落格式。 别人发的消息，永远显示在左侧，只需要在插入消息时设置一次即可。
	//
	ITextRange *pTextRangeMsg = NULL;
	pTextDocument->Range(pMsg->GetMsgStart() + pMsg->GetMsgContentStart(),
		pMsg->GetMsgStart() + pMsg->GetMsgEnd(), &pTextRangeMsg);

#ifdef _DEBUG
	BSTR bstr;
	pTextRangeMsg->GetText(&bstr);
#endif // _DEBUG

	CComPtr<ITextPara> pTextParaMsg = NULL;
	pTextRangeMsg->GetPara(&pTextParaMsg);
	int nMsgLeftIndent = GetMsgLeftIndent();
	int nMsgRightIndent = GetMsgRightIndent();
	pTextParaMsg->SetIndents(0, GetXFPPTSFromPixel(nMsgLeftIndent), GetXFPPTSFromPixel(nMsgRightIndent));
	pTextRangeMsg->SetPara(pTextParaMsg);

	// 为了方便计算行高，每条消息结束，都插入一个空行。
	IMRE_CALL_FUN(ReplaceSel(_T("\r")));

	return 0;
}

// 消息显示在RichEdit的左侧还是右侧
BOOL CIMRichEditCtrlBase::IsMsgShowRight(const CIMMsg *pMsg) const
{
	// 只有在气泡模式下，自己发送的消息，在RichEdit宽度小于某个值时，消息在显示到RichEdit右侧
	BOOL bShowLeft = TRUE;
	if (m_bQiPaoMode && pMsg->IsSelf())
	{
		CRect rcClient;
		IMRE_CALL_FUN(GetClientRect(rcClient));

		if (rcClient.Width() < m_nMaxRelayoutWidth)
		{
			bShowLeft = FALSE;
		}
	}

	return !bShowLeft;
}

// 在控件缩放或其它需要更新的时候，设置所有内容的段落格式。
// 由于别人发送的消息，已经在插入的时候，设置过段落格式，且在运行中，不会发生变化
// 所以本接口，只需要重新布局自己发送的消息即可。
int CIMRichEditCtrlBase::DoSetParaFormat()
{
	int nMsgCount = (int)m_vMsg.size();
	if (nMsgCount <= 0)
	{
		return 1;
	}

	CRect rcClient;
	IMRE_CALL_FUN(GetClientRect(rcClient));

	// 在需要设置段落格式时（例如RichEdit的宽度有变化），要对RichEdit里所有消息设置，这是因为
	// 即使隐藏的消息，在RichEdit宽度变化后，也可能发生折行，这就会导致滚动范围的变化。
	// 例如，一个自己发的消息，现在是不可见状态，显示在窗口右侧，不换行，
	// 左缩进设置为200，文本长度为20个字符，20个字符占用的宽度为400（这时Richedit的宽度为600）。
	// 当RichEdit宽度调整到比200大，但无法容纳20个字符时，比如宽度调整到500，这20个字符就要换行。
	// 这将导致滚动范围变大。但实际上，对于500宽度的RichEdit，这20个字符是不需要换行的。
	// 所以，对于不可见的消息，我们只需要把它的左缩进设置为0即可。不需要计算气泡和头像的坐标，
	// 因为，在滚动到可见范围内时，头像和气泡的坐标，会实时获取的。

	CComPtr<ITextDocument> pTextDocument = NULL;
	IMRE_CALL_FUN_RETURN(pTextDocument, GetITextDocument());

	// 第一遍循环，把所有自己发送的消息的左缩进，设置为0(如果有固定缩进的话，设置为固定缩进)
	// 这样，在第二遍循环时，取得的可见范围消息才精确。
	int i = 0;
	for (i = 0; i < nMsgCount; ++i)
	{
		const CIMMsg *pMsg = m_vMsg[i];

		// 对于对方发送的消息，因为永远显示在左侧，所以在插入消息的时候，就已经为它设置了段落格式，
		// 在缩放RichEdit的时候，不需要重置。 所以，只需要重置自己发送的消息即可
		if (m_bQiPaoMode && pMsg->IsSelf())
		{
			//
			// 把自己这条消息之前的换行符，移到RichEdit右侧外面
			//
			CRect rcWnd;
			IMRE_CALL_FUN(GetWindowRect(rcWnd));

			ITextRange *pTextRangeEnter = NULL;
			pTextDocument->Range(pMsg->GetMsgStart(), pMsg->GetMsgStart() + pMsg->GetMsgContentStart(),
				&pTextRangeEnter);

#ifdef _DEBUG
			BSTR bstr;
			pTextRangeEnter->GetText(&bstr);
#endif // _DEBUG

			CComPtr<ITextPara> pTextParaEnter = NULL;
			pTextRangeEnter->GetPara(&pTextParaEnter);
			pTextParaEnter->SetIndents(0, GetXFPPTSFromPixel(rcWnd.Width() + 100), 0);
			pTextRangeEnter->SetPara(pTextParaEnter);

			//
			// 把消息左缩进重置
			//
			ITextRange *pTextRangeMsg = NULL;
			pTextDocument->Range(pMsg->GetMsgStart() + pMsg->GetMsgContentStart(),
				pMsg->GetMsgStart() + pMsg->GetMsgEnd(), &pTextRangeMsg);

#ifdef _DEBUG
			pTextRangeMsg->GetText(&bstr);
#endif // _DEBUG

			CComPtr<ITextPara> pTextParaMsg = NULL;
			pTextRangeMsg->GetPara(&pTextParaMsg);
			int nMsgLeftIndent = GetMsgLeftIndent();
			pTextParaMsg->SetIndents(0, GetXFPPTSFromPixel(nMsgLeftIndent),
				GetXFPPTSFromPixel(GetMsgRightIndent()));
			pTextRangeMsg->SetPara(pTextParaMsg);
		}
	}

	// 第二遍循环，布局需要显示到右侧的所有消息
	for (i = 0; i < nMsgCount; ++i)
	{
		const CIMMsg *pMsg = m_vMsg[i];

		// 布局消息
		if (IsMsgShowRight(pMsg))
		{
			DoLayoutRightMsg(pMsg);
		}
	}

	return 0;
}

int CIMRichEditCtrlBase::DoLayoutRightMsg(const CIMMsg *pMsg)
{
	ASSERT(m_bQiPaoMode);

	// 消息是否有换行，内部计算时，会把自己发送的消息（需要显示到右侧）的段落格式的左缩进，设置为0.
	int nStartLine = 0;
	int nEndLine = 0;
	BOOL bNewLine = IsMsgNewLine(pMsg, &nStartLine, &nEndLine);

	// 有折行的消息，不需要设置左缩进
	if (bNewLine)
	{
		return 1;
	}

	// 没有折行的消息，需要设置左缩进，让消息向右平移
	CRect rcQiPao;

	int nMinLeftLine = MAXINT; // 所有行，left的最小值
	int nMaxRightLine = 0; // 所有行，right的最大值

	for (int i = nStartLine; i <= nEndLine; ++i)
	{
		CPoint ptLine;
		int nLineWidth = 0;
		int nLineHeight = 0;

		if (i == nEndLine)
		{
			IMRE_CALL_FUN(GetLineRect(i, &ptLine, &nLineWidth, &nLineHeight));
		}
		else
		{
			IMRE_CALL_FUN(GetLineRect(i, &ptLine, &nLineWidth, NULL));
		}

		if (i == nStartLine)
		{
			rcQiPao.top = ptLine.y;
		}

		if (i == nEndLine)
		{
			rcQiPao.bottom = ptLine.y + nLineHeight;
		}

		nMinLeftLine = min(nMinLeftLine, ptLine.x);
		nMaxRightLine = max(nMaxRightLine, ptLine.x + nLineWidth);
	}

	rcQiPao.left = nMinLeftLine;
	rcQiPao.right = nMaxRightLine;

	// 当气泡需要显示到右侧、设置左缩进时，不能正好设置richedit宽度减
	// 气泡宽度，否则会导致原本可以单行显示的段落换了行。
	// 猜测是还要容纳行尾的回车换行的空间，所以，在没有办法得到这个空间宽度前，左缩进需要设置的比计算出来的
	// 值小一些（ RichEdit会自动把负偏移设置为0）。 nQiPaoLeftOffsetAdjust就是这个修正值
	const int nQiPaoLeftOffsetAdjust = 20; // 这个值，暂时用一个字符宽度比较合适，只会大不会小。

	CRect rcClient;
	IMRE_CALL_FUN(GetClientRect(rcClient));

	CRect rcInset;
	IMRE_CALL_FUN(GetInset(rcInset));

	rcClient.DeflateRect(rcInset);

	int nXOffset = rcClient.Width() - GetMsgRightIndent() - rcQiPao.Width() - nQiPaoLeftOffsetAdjust;
	if (nXOffset < 0)
	{
		ASSERT(FALSE);
		nXOffset = 0;
	}

	rcQiPao.OffsetRect(-rcQiPao.left, 0);
	rcQiPao.OffsetRect(nXOffset, 0);

	CComPtr<ITextDocument> pTextDocument = NULL;
	IMRE_CALL_FUN_RETURN(pTextDocument, GetITextDocument());

	ITextRange *pTextRange = NULL;
	pTextDocument->Range(pMsg->GetMsgStart() + pMsg->GetMsgContentStart(),
		pMsg->GetMsgStart() + pMsg->GetMsgEnd(), &pTextRange);

#ifdef _DEBUG
	BSTR bstr;
	pTextRange->GetText(&bstr);
#endif // _DEBUG

	CComPtr<ITextPara> pTextPara = NULL;
	pTextRange->GetPara(&pTextPara);
	int nMsgRightIndent = GetMsgRightIndent();
	pTextPara->SetIndents(0, GetXFPPTSFromPixel(nXOffset), GetXFPPTSFromPixel(nMsgRightIndent));
	pTextRange->SetPara(pTextPara);

	return 0;
}

int CIMRichEditCtrlBase::DoCalFaceRect(const CIMMsg *pMsg, __out LPRECT lprcFace)
{
	if (pMsg == NULL || lprcFace == NULL)
	{
		return -1;
	}

	CRect rcClient;
	IMRE_CALL_FUN(GetClientRect(rcClient));

	CRect rcInset;
	IMRE_CALL_FUN(GetInset(rcInset));

	// 计算头像坐标
	CRect rcFace(0, 0, 0, 0);

	// 对于有句柄的RichEdit，GetCharPos返回的是当前滚动位置相对RichEdit的坐标，
	// 所以，返回的坐标，不需要进行滚动位置修正。
	CPoint ptFace;
	IMRE_CALL_FUN_RETURN(ptFace, GetCharPos(pMsg->GetMsgStart()));

	if (IsMsgShowRight(pMsg))
	{
		rcFace.right = rcClient.right - rcInset.right - (GetMsgRightIndent() - m_sizeFace.cx) / 2;
		rcFace.left = rcFace.right - m_sizeFace.cx;
		rcFace.top = rcClient.top + ptFace.y;
		rcFace.bottom = rcFace.top + m_sizeFace.cy;
	}
	else
	{
		rcFace.left = rcClient.left + rcInset.left + (GetSenderIndent() - m_sizeFace.cx) / 2;
		rcFace.right = rcFace.left + m_sizeFace.cx;
		rcFace.top = rcClient.top + ptFace.y;
		rcFace.bottom = rcFace.top + m_sizeFace.cy;
	}

	*lprcFace = rcFace;

	return 0;
}

// 为了性能和解决显示方面的问题
// 在垂直滚动时，只计算气泡坐标，不设置段落格式，而是使用消息内缓存的折行标志
int CIMRichEditCtrlBase::DoCalQiPaoRect(const CIMMsg *pMsg, __out LPRECT lprcQiPao)
{
	if (lprcQiPao == NULL)
	{
		return -1;
	}

	// 消息是否有换行，内部计算时，会把自己发送的消息（需要显示到右侧）的段落格式的左缩进，设置为0.
	int nStartLine = 0;
	int nEndLine = 0;
	BOOL bNewLine = IsMsgNewLine(pMsg, &nStartLine, &nEndLine);

	// 下面的代码，会再次把段落的左缩进，设置为最终值。
	if (bNewLine)
	{
		DoCalNewLineQiPaoRect(pMsg, lprcQiPao);
	}
	else
	{
		DoCalSingleLineQiPaoRect(pMsg, nStartLine, nEndLine, lprcQiPao);
	}

	return 0;
}

// 计算换行(显示的行数是否大于CIMMsg::m_nMsgParagraphCount)消息的气泡坐标
int CIMRichEditCtrlBase::DoCalNewLineQiPaoRect(const CIMMsg *pMsg, __out LPRECT lprcQiPao)
{
	if (lprcQiPao == NULL)
	{
		return -1;
	}

	CRect rcClient;
	IMRE_CALL_FUN(GetClientRect(rcClient));

	CRect rcInset;
	IMRE_CALL_FUN(GetInset(rcInset));
	rcClient.DeflateRect(rcInset);

	// 得到消息第一个字符的上坐标
	CPoint ptFirstChar;
	IMRE_CALL_FUN_RETURN(ptFirstChar, GetCharPos(pMsg->GetMsgStart() + pMsg->GetMsgContentStart()));

	lprcQiPao->left = rcClient.left + GetMsgLeftIndent();
	lprcQiPao->right = rcClient.right - GetMsgRightIndent();
	lprcQiPao->top = ptFirstChar.y;

	// 本条消息最后一行最后一个字符的右下角坐标
	int nLine = -1;
	IMRE_CALL_FUN_RETURN(nLine, LineFromChar(pMsg->GetMsgStart() + pMsg->GetMsgEnd()));
	CPoint ptLastMsgLine;
	int nLineHeight = 0;
	IMRE_CALL_FUN(GetLineRect(nLine, &ptLastMsgLine, NULL, &nLineHeight));

	lprcQiPao->bottom = ptLastMsgLine.y + nLineHeight;

	return 0;
}

// 计算不换行(显示的行数是否大于CIMMsg::m_nMsgParagraphCount)消息显示时的气泡坐标
int CIMRichEditCtrlBase::DoCalSingleLineQiPaoRect(const CIMMsg *pMsg, int nStartLine, int nEndLine,
	__out LPRECT lprcQiPao)
{
	if (lprcQiPao == NULL)
	{
		return -1;
	}

	CRect rcQiPao;

	int nMinLeftLine = MAXINT; // 所有行，left的最小值
	int nMaxRightLine = 0; // 所有行，right的最大值

	for (int i = nStartLine; i <= nEndLine; ++i)
	{
		CPoint ptLine;
		int nLineWidth = 0;
		int nLineHeight = 0;

		if (i == nEndLine)
		{
			IMRE_CALL_FUN(GetLineRect(i, &ptLine, &nLineWidth, &nLineHeight));
		}
		else
		{
			IMRE_CALL_FUN(GetLineRect(i, &ptLine, &nLineWidth, NULL));
		}

		if (i == nStartLine)
		{
			rcQiPao.top = ptLine.y;
		}

		if (i == nEndLine)
		{
			rcQiPao.bottom = ptLine.y + nLineHeight;
		}

		nMinLeftLine = min(nMinLeftLine, ptLine.x);
		nMaxRightLine = max(nMaxRightLine, ptLine.x + nLineWidth);
	}

	rcQiPao.left = nMinLeftLine;
	rcQiPao.right = nMaxRightLine;

	*lprcQiPao = rcQiPao;

	return 0;
}

BOOL CIMRichEditCtrlBase::IsMsgNewLine(const CIMMsg *pMsg,
	__out int *pnStartLine, __out int *pnEndLine) const
{
	if (pMsg == NULL)
	{
		return FALSE;
	}

	// 在没有左缩进的情况下，计算是否需要换行显示
	int nStartLine = -1;
	IMRE_CALL_FUN_RETURN(nStartLine, LineFromChar(pMsg->GetMsgStart() + pMsg->GetMsgContentStart()));
	int nEndLine = -1;
	IMRE_CALL_FUN_RETURN(nEndLine, LineFromChar(pMsg->GetMsgStart() + pMsg->GetMsgEnd()));
	ASSERT(nEndLine - nStartLine + 1 >= pMsg->GetMsgParagraphCount());

	if (pnStartLine != NULL)
	{
		*pnStartLine = nStartLine;
	}

	if (pnEndLine != NULL)
	{
		*pnEndLine = nEndLine;
	}

	BOOL bNewLine = (nEndLine - nStartLine + 1 > pMsg->GetMsgParagraphCount());

	return bNewLine;
}

int CIMRichEditCtrlBase::SetSenderIndent(int nLeftIndent)
{
	m_nSenderLeftIndent = nLeftIndent;

	return 0;
}

int CIMRichEditCtrlBase::GetSenderIndent() const
{
	return m_nSenderLeftIndent;
}

int CIMRichEditCtrlBase::SetMsgLeftIndent(int nLeftIndent)
{
	m_nMsgLeftIndent = nLeftIndent;

	return 0;
}

int CIMRichEditCtrlBase::GetMsgLeftIndent() const
{
	return m_nMsgLeftIndent;
}

int CIMRichEditCtrlBase::SetMsgRightIndent(int nRightIndent)
{
	m_nMsgRightIndent = nRightIndent;

	return 0;
}

int CIMRichEditCtrlBase::GetMsgRightIndent() const
{
	return m_nMsgRightIndent;
}


int CIMRichEditCtrlBase::SetMaxRelayoutWidth(int nWidth)
{
	m_nMaxRelayoutWidth = nWidth;

	return 0;
}

int CIMRichEditCtrlBase::GetMaxRelayoutWidth() const
{
	return m_nMaxRelayoutWidth;
}

int CIMRichEditCtrlBase::SetMsgSenderTitleAlias(int nLevel, LPCTSTR lpszAlias)
{
	m_mapMsgSenderTitleAlias[nLevel] = lpszAlias;

	return 0;
}

int CIMRichEditCtrlBase::ShowMsgSenderTitle(BOOL bShow)
{
	m_bShowMsgSenderTitle = bShow;

	return 0;
}

BOOL CIMRichEditCtrlBase::IsShowMsgSenderTitle() const
{
	return m_bShowMsgSenderTitle;
}

int CIMRichEditCtrlBase::SetFaceSize(SIZE sizeFace)
{
	m_sizeFace = sizeFace;

	return 0;
}

SIZE CIMRichEditCtrlBase::GetFaceSize() const
{
	return m_sizeFace;
}

int CIMRichEditCtrlBase::ShowLeftFace(BOOL bShow)
{
	m_bShowLeftFace = bShow;
	return 0;
}

BOOL CIMRichEditCtrlBase::IsShowLeftFace() const
{
	return m_bShowLeftFace;
}

int CIMRichEditCtrlBase::ShowRightFace(BOOL bShow)
{
	m_bShowRightFace = bShow;
	return 0;
}

BOOL CIMRichEditCtrlBase::IsShowRightFace() const
{
	return m_bShowRightFace;
}

int CIMRichEditCtrlBase::GetFirstVisibleMsg() const
{
	int nRet = -1;

	int nFirstVisibleLine = -1;
	IMRE_CALL_FUN_RETURN(nFirstVisibleLine, GetFirstVisibleLine());
#ifdef _DEBUG
	int nFirstVisibleCp = -1;
	IMRE_CALL_FUN_RETURN(nFirstVisibleCp, LineIndex(nFirstVisibleLine));
#endif // _DEBUG

	// 遍历所有消息
	std::vector<const CIMMsg *>::const_iterator it = m_vMsg.begin();
	for (; it != m_vMsg.end(); ++it)
	{
		const CIMMsg *pMsg = *it;

		if (pMsg->GetMsgStart() + pMsg->GetMsgEnd() >= nFirstVisibleLine)
		{
			nRet = int(it - m_vMsg.begin());
			break;
		}
	}

	return nRet;
}

BOOL CIMRichEditCtrlBase::IsNeedUpdate() const
{
	BOOL bNeedUpdate = FALSE;

	if (m_bWindowlessMode)
	{
		bNeedUpdate = (BOOL)((CWLIMRichEditCtrl *)m_pIMReBind)->SendMessage(WM_ISNEEDUPDATA);
	}
	else
	{
		bNeedUpdate = (BOOL)((CIMRichEditCtrl *)m_pIMReBind)->SendMessage(WM_ISNEEDUPDATA);
	}

	return bNeedUpdate;
}

int CIMRichEditCtrlBase::NeedUpdate(BOOL bNeedUpdate)
{
	if (m_bWindowlessMode)
	{
		((CWLIMRichEditCtrl *)m_pIMReBind)->SendMessage(WM_SETNEEDUPDATE, 0, bNeedUpdate);
	}
	else
	{
		((CIMRichEditCtrl *)m_pIMReBind)->SendMessage(WM_SETNEEDUPDATE, 0, bNeedUpdate);
	}

	return 0;
}

// 绘制非Rich Text，包含头像和气泡等，只绘制可视频范围内的消息记录
// 在绘制前计算头像和气泡坐标，以提高性能和准确度。
// 如果在WM_VSCROLL时计算，会发现滚动过程中，气泡比内容滚动慢半拍（即使先调用基类的OnVScroll）。
int CIMRichEditCtrlBase::OnDrawNonRichText(CDC *pMemDCParent)
{
	// 如果第一可见行小于0
	int nFirstVisibleMsg = GetFirstVisibleMsg();
	if (nFirstVisibleMsg < 0)
	{
		return 1;
	}

	CRect rcWnd;
	IMRE_CALL_FUN(GetWindowRect(rcWnd));
	IMRE_CALL_FUN(GetParent()->ScreenToClient(rcWnd));

	CRect rcClient;
	IMRE_CALL_FUN(GetClientRect(rcClient));
	rcWnd.right = rcWnd.left + rcClient.Width();

	//
	// 1. 设置RichEdit区域为裁剪区域
	//
	HRGN hRgnControl = CreateRectRgnIndirect(rcWnd);

	// Get old region.
	HRGN hOldParentRgn = CreateRectRgn(0, 0, 0, 0);
	// If the function succeeds, the region is a handle to a copy of the current clipping region.
	int nRet2 = GetClipRgn(pMemDCParent->GetSafeHdc(), hOldParentRgn);
	if (nRet2 == 1)	// the function succeeds and there is a clipping region for the given device context.
	{
		hOldParentRgn = CreateRectRgn(0, 0, 0, 0);

		// combine region
		HRGN hNewRgn = CreateRectRgn(0, 0, 0, 0);

		CombineRgn(hNewRgn, hOldParentRgn, hRgnControl, RGN_AND);

		::SelectClipRgn(pMemDCParent->GetSafeHdc(), hNewRgn);
		DeleteObject(hNewRgn);
	}
	else
	{
		::DeleteObject(hOldParentRgn);
		hOldParentRgn = NULL;
		::SelectClipRgn(pMemDCParent->GetSafeHdc(), hRgnControl);
	}
	::DeleteObject(hRgnControl);

	//
	// 绘制
	//
	int nMsgCount = (int)m_vMsg.size();
	for (int i = nFirstVisibleMsg; i < nMsgCount; ++i)
	{
		const CIMMsg *pMsg = m_vMsg[i];

		// 下端超出显示范围后，停止绘制
		CPoint pt;
		IMRE_CALL_FUN_RETURN(pt, GetCharPos(pMsg->GetMsgStart()));
		if (pt.y >= rcClient.Height())
		{
			break;
		}

		// 得到头像坐标
		CRect rcFace;
		DoCalFaceRect(pMsg, rcFace);

		// 绘制头像
		OnDrawFace(pMemDCParent, i, rcFace);

		// 绘制气泡
		if (m_bQiPaoMode)
		{
			CRect rcQiPao;
			DoCalQiPaoRect(pMsg, rcQiPao);

			OnDrawQiPao(pMemDCParent, i, rcQiPao);
		}
	}

	// 恢复裁剪区域
	::SelectClipRgn(pMemDCParent->GetSafeHdc(), hOldParentRgn);
	::DeleteObject(hOldParentRgn);

	return 0;
}

#ifdef _DEBUG
// 测试代码。由外部调用，对RichEdit的某些功能进行测试。
int CIMRichEditCtrlBase::Test()
{

	return 0;
}
#endif // _DEBUG

// 画头像, lprcFace是相对于RichEdit控件的，如果头像画到父窗口上，要进行坐标转换
int CIMRichEditCtrlBase::OnDrawFace(CDC *pMemDCParent, int nMsgIndex, LPCRECT lprcFace)
{
	if (lprcFace == NULL)
	{
		return -1;
	}

	int nMsgCount = (int)m_vMsg.size();
	if (nMsgCount <= 0 || nMsgIndex >= nMsgCount)
	{
		return -2;
	}

	CRect rcWnd;
	IMRE_CALL_FUN(GetWindowRect(rcWnd));
	IMRE_CALL_FUN(GetParent()->ScreenToClient(rcWnd));

	CRect rcNew = *lprcFace;
	rcNew.OffsetRect(rcWnd.TopLeft());

	// 计算本消息在消息组中的位置（上中下，用DT_TOP，DT_CENTER和DT_BOTTOM表示）
	UINT uMsgPos = GetMsgPosInGroup(nMsgIndex);

	if (uMsgPos == DT_TOP)
	{
		const CIMMsg *pMsg = m_vMsg[nMsgIndex];
		rcNew.OffsetRect(0, 20);


		HIUIIMAGE hImage = pMsg->GetFace();
		if (hImage->GetSafeHBITMAP() != NULL)
		{
			Graphics g(pMemDCParent->GetSafeHdc());
			g.SetSmoothingMode(SmoothingModeAntiAlias);

			Bitmap image(hImage->GetSafeHBITMAP(), 0);

			TextureBrush tBrush(&image);
			tBrush.TranslateTransform((REAL)rcNew.left, (REAL)rcNew.top);

			GraphicsPath gp;
			//gp.AddEllipse(0, 0, 40, 40);
			gp.AddEllipse((int)rcNew.left, rcNew.top,
				rcNew.right - rcNew.left, rcNew.bottom - rcNew.top);

			g.FillPath(&tBrush, &gp);
		}
	}

	return 0;
}

// 当消息合并时，计算消息在组内的索引。（上中下，用DT_TOP，DT_CENTER和DT_BOTTOM表示）
int CIMRichEditCtrlBase::GetMsgPosInGroup(int nMsgIndex)
{
	int nMsgCount = (int)m_vMsg.size();
	if (nMsgCount <= 0 || nMsgIndex >= nMsgCount)
	{
		return -2;
	}

	const CIMMsg *pMsg = m_vMsg[nMsgIndex];

#ifdef _DEBUG
	int nc = pMsg->m_vpStrRichMsg.size();
	for (int j = 0; j < nc ; j++)
	{
		CRichMsgItem *pitem = pMsg->m_vpStrRichMsg[j];
		if (pitem->m_strMsgContent.Find(_T("不够再说")) >= 0)
		{
			int n = 0;
			break;
		}
	}
#endif // _DEBUG

	// 记录本消息消息的发送者
	CString strSender = pMsg->GetMsgSender();

	//
	// 计算本消息在消息组中的位置（上中下，用DT_TOP，DT_CENTER和DT_BOTTOM表示）
	//

	// 向上查找本气泡发送者发送的消息，直到找到非本气泡发送者，或者找到m_bGroup为true
	// 或者上一条消息离本条消息时间时隔过长。
	UINT uMsgPos = DT_TOP;
	for (int i = nMsgIndex - 1; i >= 0; i--)
	{
		const CIMMsg *pMsgWalk = m_vMsg[i];

		if (strSender != pMsgWalk->GetMsgSender())
		{
			if (nMsgIndex - i > 1)
			{
				uMsgPos = DT_CENTER;
			}
			else
			{
				uMsgPos = DT_TOP;
			}

			break;
		}

		if (pMsgWalk->m_bGroup)
		{
			uMsgPos = DT_CENTER;
			break;
		}

		// 超过5分种，就另起一组(插入一条时间消息)
		if ((pMsg->m_tMsg - pMsgWalk->m_tMsg).GetTotalMinutes() > 5)
		{
			((CIMMsg *)pMsg)->m_bGroup = true;
			uMsgPos = DT_TOP;
			break;
		}
	}

	// 如果本消息消息的坐标为DT_CENTER，并且本组没有下一条消息，则修正为DT_BOTTOM
	if (uMsgPos == DT_CENTER && nMsgIndex + 1 < nMsgCount)
	{
		const CIMMsg *pMsgNext = m_vMsg[nMsgIndex + 1];

		if (strSender != pMsgNext->GetMsgSender()
			|| pMsgNext->m_bGroup
			|| (pMsgNext->m_tMsg - pMsg->m_tMsg).GetTotalMinutes() > 5)
		{
			uMsgPos = DT_BOTTOM;
		}
	}

	return uMsgPos;
}

int CIMRichEditCtrlBase::SetCombineMsg(BOOL bCombine)
{
	BOOL bOld = m_bCombineMsg;
	m_bCombineMsg = bCombine;
	return bOld;
}

BOOL CIMRichEditCtrlBase::IsCombineMsg() const
{
	return m_bCombineMsg;
}

// 当使用合并消息模式时，设置多久之内的消息需要合并
int CIMRichEditCtrlBase::SetCombineMsgTimeSpan(int nSecond)
{
	m_nCombineMsgTimeSpan = nSecond;
	return 0;
}

int CIMRichEditCtrlBase::GetCombineMsgTimeSpan() const
{
	return m_nCombineMsgTimeSpan;
}

// 画气泡, lprcQiPao是相对于RichEdit控件的，如果气泡画到父窗口上，要进行坐标转换
int CIMRichEditCtrlBase::OnDrawQiPao(CDC *pMemDCParent, int nMsgIndex, LPCRECT lprcQiPao)
{
	if (lprcQiPao == NULL)
	{
		return -1;
	}

	int nMsgCount = (int)m_vMsg.size();
	if (nMsgCount <= 0 || nMsgIndex >= nMsgCount)
	{
		return -2;
	}

	CRect rcWnd;
	IMRE_CALL_FUN(GetWindowRect(rcWnd));
	IMRE_CALL_FUN(GetParent()->ScreenToClient(rcWnd));

	CRect rcNew = *lprcQiPao;
	rcNew.OffsetRect(rcWnd.TopLeft());

	const CIMMsg *pMsg = m_vMsg[nMsgIndex];

	// 计算本消息在消息组中的位置（上中下，用DT_TOP，DT_CENTER和DT_BOTTOM表示）
	UINT uMsgPos = DT_TOP;
	if (IsCombineMsg())
	{
		uMsgPos = GetMsgPosInGroup(nMsgIndex);
	}

	// 给父窗口一个自绘气泡的机会
	if (!m_bWindowlessMode)
	{
		CIMRichEditCtrl *pBindRichEdit = (CIMRichEditCtrl *)m_pIMReBind;
		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_IMQIPAO;
		dis.CtlID = pBindRichEdit->GetDlgCtrlID();
		dis.itemID = nMsgIndex;
		dis.itemAction = IsCombineMsg();
		dis.itemState = uMsgPos;
		dis.hwndItem = pBindRichEdit->GetSafeHwnd();
		dis.hDC = pMemDCParent->GetSafeHdc();
		dis.rcItem = rcNew;
		dis.itemData = (ULONG_PTR)pMsg;

		BOOL bParentDraw = (BOOL)pBindRichEdit->GetParent()->SendMessage(WM_DRAWITEMEX,
				dis.CtlID, LPARAM(&dis));
		if (bParentDraw)
		{
			return 0;
		}
	}

	if (IsCombineMsg())
	{
		rcNew.InflateRect(5, 10, 5, 10);

		HIUIIMAGE hImg = NULL;
		if (pMsg->m_bSelf)
		{
			if (uMsgPos == DT_TOP)
			{
				hImg = CreateIUIImage(_T("pop_right_up.png"));
			}
			else if (uMsgPos == DT_CENTER)
			{
				hImg = CreateIUIImage(_T("pop_right_mid.png"));
			}
			else if (uMsgPos == DT_BOTTOM)
			{
				hImg = CreateIUIImage(_T("pop_right_end.png"));
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			if (uMsgPos == DT_TOP)
			{
				hImg = CreateIUIImage(_T("pop_left_up.png"));
			}
			else if (uMsgPos == DT_CENTER)
			{
				hImg = CreateIUIImage(_T("pop_left_mid.png"));
			}
			else if (uMsgPos == DT_BOTTOM)
			{
				hImg = CreateIUIImage(_T("pop_left_end.png"));
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		POINT pt;
		pt.x = 16;
		pt.y = 15;
		IUINineGridBlt(pMemDCParent->GetSafeHdc(), rcNew, hImg, pt, 1, 1);
		ReleaseIUIImage(hImg);
	}

	return 0;
}
