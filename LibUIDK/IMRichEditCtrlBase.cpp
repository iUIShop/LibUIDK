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

	// ��ͨ��Ϣ���ѻ��з�����\r
	if (pNewMsgItem->m_strProtocol.IsEmpty())
	{
		// ����Ϣ�е�\r\n�滻��\r�� Ȼ���\nҲ�滻Ϊ\r�� ��ΪRichEditֻ��\r
		pNewMsgItem->m_strMsgContent.Replace(_T("\r\n"), _T("\r"));
		pNewMsgItem->m_strMsgContent.Replace(_T("\n"), _T("\r"));

		// ��¼��Ϣ���ж��ٸ�\r�����䣩
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
		// ���OLE����ͼƬ��ͳͳ������һ������Ϣ
		if (pNewMsgItem->m_strProtocol != _T("i/:"))
		{
			// 			CIMMsg *pMsgFile = new CIMMsg;
			// 			pMsgFile->SetMsgSenderTitle(GetMsgSenderTitle());
			// 			pMsgFile->SetMsgSender(GetMsgSender());
			// 			pMsgFile->SetFaceIndex(GetFaceIndex());
			// 			pMsgFile->SetSelf(IsSelf());
			// 			// f/:����guid����Ϊ41
			// 			pMsgFile->SetOrigRichMsg(m_strRichMsg.Mid(nFilePos, 41));
			//
			// 			CRichMsgItem *pMsgItem = new CRichMsgItem;
			// 			pMsgItem->m_strProtocol = _T("f/:");
			// 			pMsgItem->m_strMsg = m_strRichMsg.Mid(nFilePos + 3, 38);
			// 			pMsgFile->m_vpStrRichMsg.push_back(pMsgItem);
			//
			// 			// ��Ϊ������
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
	// ���������Ա���Զ�����
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

	size_t nCount = rSrc.m_vpStrRichMsg.size();
	for (size_t i = 0; i < nCount ; i++)
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
	// �ͷŴ�IOleCtrlBase������ole��Դ
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

		// ��ʾ���ole�Ǵ�IOleCtrlBase�����ġ�
		if (ro.clsid == g_guidImageOle)
		{
			IOleCtrlBase *pOle = (IOleCtrlBase *)ro.poleobj;
			pOle->ReleaseObject();

			((CSkinRichEditCtrl *)m_pIMReBind)->SetSel(ro.cp, ro.cp + 1);
			((CSkinRichEditCtrl *)m_pIMReBind)->ReplaceSel(NULL);

			delete pOle;
		}
	}

	// �ͷ���Ϣ
	size_t nMsgCount = m_vMsg.size();
	for (size_t i = 0; i < nMsgCount; ++i)
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

// Ϊ�����ܿ��ǣ�����InsertMsgǰ������Ҫ������Ϣ����Ϊ���������дӷ�������������Ϣ
// ������뵽RichEdit�� ������InsertMsg�ڲ���Ҫ������Ϣ��ʽ��
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

	// ��������
	m_vMsg.push_back(pNewMsg);

	int nCount = (int)m_vMsg.size();

	DoInsertMsg(pNewMsg);

	// ��������
	return nCount - 1;
}

int CIMRichEditCtrlBase::DoInsertMsg(const CIMMsg *pMsg)
{
	if (pMsg == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	// ��¼����Ŀ�ʼλ��
	IMRE_CALL_FUN(SetSel(-1, -1));
	int nLength = -1;
#if(_MSC_VER <= 1200) // VC6.0
	IMRE_CALL_FUN_RETURN(nLength, GetTextLength());
#else
	IMRE_CALL_FUN_RETURN(nLength, GetTextLengthEx(GTL_DEFAULT | GTL_NUMCHARS));
#endif
	((CIMMsg *)pMsg)->SetMsgStart(nLength);

	//
	// 1 ���롾����+����
	//
	BOOL bShowSender = TRUE; // �Ƿ���ʾ��Ϣ����������
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
		strName = _T("��");
		strName += m_mapMsgSenderTitleAlias[pMsg->GetMsgSenderTitle()];
		strName += _T("�� ");
		strName += pMsg->GetMsgSender();
		strName += _T("\r");

		IMRE_CALL_FUN(ReplaceSel(strName));

		// �������ֵĶ����ʽ��ֻ�б��˲���ʾ���������֣��Լ��ǲ���ʾ�ġ�
		// ������Զ��ʾ����࣬���ԣ�ֻ��Ҫ�ڲ�����Ϣʱ���������ֵĶ����ʽ���ɣ�֮�󶼲���Ҫ���á�
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
		// �Լ����͵���Ϣ������һ�����У���������ʾ����ʱ�����ǰһ����Ϣ���ص������ݳߴ���ڼ���ߴ磩
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
	// 2. ������Ϣ����
	//
	int nRichCount = (int)pMsg->m_vpStrRichMsg.size();
	for (int i = 0; i < nRichCount; ++i)
	{
		CRichMsgItem *pRichMsgItem = pMsg->m_vpStrRichMsg[i];

		//
		// 1 ����
		//
		if (!pRichMsgItem->m_strProtocol.IsEmpty())
		{
			if (pRichMsgItem->m_strProtocol == _T("i/:"))
			{
				// �������ĸ�Gif
				TCHAR chB = pRichMsgItem->m_strMsgContent[0];
				TCHAR chX = pRichMsgItem->m_strMsgContent[1];
				TCHAR szX[2] = {chX, 0};
				int nX = IntFromString(szX);
				int nOrigMsgLength = pRichMsgItem->m_strMsgContent.GetLength();

				// ������������
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

				// ֪ͨRichEdit�����ڣ�������һ���µ��Զ���OLE�ؼ�
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
			// ��ͨ�ı�
			//
			IMRE_CALL_FUN(ReplaceSel(pRichMsgItem->m_strMsgContent));
		}
	}

	// ��¼����Ľ���λ��
#if(_MSC_VER <= 1200) // VC6.0
	IMRE_CALL_FUN_RETURN(nLength, GetTextLength());
#else
	IMRE_CALL_FUN_RETURN(nLength, GetTextLengthEx(GTL_DEFAULT | GTL_NUMCHARS));
#endif
	((CIMMsg *)pMsg)->SetMsgEnd(nLength - pMsg->GetMsgStart());

	//
	// 3. ������Ϣ��Ķ����ʽ�� ���˷�����Ϣ����Զ��ʾ����ֻ࣬��Ҫ�ڲ�����Ϣʱ����һ�μ��ɡ�
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

	// Ϊ�˷�������иߣ�ÿ����Ϣ������������һ�����С�
	IMRE_CALL_FUN(ReplaceSel(_T("\r")));

	return 0;
}

// ��Ϣ��ʾ��RichEdit����໹���Ҳ�
BOOL CIMRichEditCtrlBase::IsMsgShowRight(const CIMMsg *pMsg) const
{
	// ֻ��������ģʽ�£��Լ����͵���Ϣ����RichEdit���С��ĳ��ֵʱ����Ϣ����ʾ��RichEdit�Ҳ�
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

// �ڿؼ����Ż�������Ҫ���µ�ʱ�������������ݵĶ����ʽ��
// ���ڱ��˷��͵���Ϣ���Ѿ��ڲ����ʱ�����ù������ʽ�����������У����ᷢ���仯
// ���Ա��ӿڣ�ֻ��Ҫ���²����Լ����͵���Ϣ���ɡ�
int CIMRichEditCtrlBase::DoSetParaFormat()
{
	int nMsgCount = (int)m_vMsg.size();
	if (nMsgCount <= 0)
	{
		return 1;
	}

	CRect rcClient;
	IMRE_CALL_FUN(GetClientRect(rcClient));

	// ����Ҫ���ö����ʽʱ������RichEdit�Ŀ���б仯����Ҫ��RichEdit��������Ϣ���ã�������Ϊ
	// ��ʹ���ص���Ϣ����RichEdit��ȱ仯��Ҳ���ܷ������У���ͻᵼ�¹�����Χ�ı仯��
	// ���磬һ���Լ�������Ϣ�������ǲ��ɼ�״̬����ʾ�ڴ����Ҳ࣬�����У�
	// ����������Ϊ200���ı�����Ϊ20���ַ���20���ַ�ռ�õĿ��Ϊ400����ʱRichedit�Ŀ��Ϊ600����
	// ��RichEdit��ȵ�������200�󣬵��޷�����20���ַ�ʱ�������ȵ�����500����20���ַ���Ҫ���С�
	// �⽫���¹�����Χ��󡣵�ʵ���ϣ�����500��ȵ�RichEdit����20���ַ��ǲ���Ҫ���еġ�
	// ���ԣ����ڲ��ɼ�����Ϣ������ֻ��Ҫ����������������Ϊ0���ɡ�����Ҫ�������ݺ�ͷ������꣬
	// ��Ϊ���ڹ������ɼ���Χ��ʱ��ͷ������ݵ����꣬��ʵʱ��ȡ�ġ�

	CComPtr<ITextDocument> pTextDocument = NULL;
	IMRE_CALL_FUN_RETURN(pTextDocument, GetITextDocument());

	// ��һ��ѭ�����������Լ����͵���Ϣ��������������Ϊ0(����й̶������Ļ�������Ϊ�̶�����)
	// �������ڵڶ���ѭ��ʱ��ȡ�õĿɼ���Χ��Ϣ�ž�ȷ��
	int i = 0;
	for (i = 0; i < nMsgCount; ++i)
	{
		const CIMMsg *pMsg = m_vMsg[i];

		// ���ڶԷ����͵���Ϣ����Ϊ��Զ��ʾ����࣬�����ڲ�����Ϣ��ʱ�򣬾��Ѿ�Ϊ�������˶����ʽ��
		// ������RichEdit��ʱ�򣬲���Ҫ���á� ���ԣ�ֻ��Ҫ�����Լ����͵���Ϣ����
		if (m_bQiPaoMode && pMsg->IsSelf())
		{
			//
			// ���Լ�������Ϣ֮ǰ�Ļ��з����Ƶ�RichEdit�Ҳ�����
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
			// ����Ϣ����������
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

	// �ڶ���ѭ����������Ҫ��ʾ���Ҳ��������Ϣ
	for (i = 0; i < nMsgCount; ++i)
	{
		const CIMMsg *pMsg = m_vMsg[i];

		// ������Ϣ
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

	// ��Ϣ�Ƿ��л��У��ڲ�����ʱ������Լ����͵���Ϣ����Ҫ��ʾ���Ҳࣩ�Ķ����ʽ��������������Ϊ0.
	int nStartLine = 0;
	int nEndLine = 0;
	BOOL bNewLine = IsMsgNewLine(pMsg, &nStartLine, &nEndLine);

	// �����е���Ϣ������Ҫ����������
	if (bNewLine)
	{
		return 1;
	}

	// û�����е���Ϣ����Ҫ����������������Ϣ����ƽ��
	CRect rcQiPao;

	int nMinLeftLine = MAXINT; // �����У�left����Сֵ
	int nMaxRightLine = 0; // �����У�right�����ֵ

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

	// ��������Ҫ��ʾ���Ҳࡢ����������ʱ��������������richedit��ȼ�
	// ���ݿ�ȣ�����ᵼ��ԭ�����Ե�����ʾ�Ķ��任���С�
	// �²��ǻ�Ҫ������β�Ļس����еĿռ䣬���ԣ���û�а취�õ�����ռ���ǰ����������Ҫ���õıȼ��������
	// ֵСһЩ�� RichEdit���Զ��Ѹ�ƫ������Ϊ0���� nQiPaoLeftOffsetAdjust�����������ֵ
	const int nQiPaoLeftOffsetAdjust = 20; // ���ֵ����ʱ��һ���ַ���ȱȽϺ��ʣ�ֻ��󲻻�С��

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

	// ����ͷ������
	CRect rcFace(0, 0, 0, 0);

	// �����о����RichEdit��GetCharPos���ص��ǵ�ǰ����λ�����RichEdit�����꣬
	// ���ԣ����ص����꣬����Ҫ���й���λ��������
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

// Ϊ�����ܺͽ����ʾ���������
// �ڴ�ֱ����ʱ��ֻ�����������꣬�����ö����ʽ������ʹ����Ϣ�ڻ�������б�־
int CIMRichEditCtrlBase::DoCalQiPaoRect(const CIMMsg *pMsg, __out LPRECT lprcQiPao)
{
	if (lprcQiPao == NULL)
	{
		return -1;
	}

	// ��Ϣ�Ƿ��л��У��ڲ�����ʱ������Լ����͵���Ϣ����Ҫ��ʾ���Ҳࣩ�Ķ����ʽ��������������Ϊ0.
	int nStartLine = 0;
	int nEndLine = 0;
	BOOL bNewLine = IsMsgNewLine(pMsg, &nStartLine, &nEndLine);

	// ����Ĵ��룬���ٴΰѶ����������������Ϊ����ֵ��
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

// ���㻻��(��ʾ�������Ƿ����CIMMsg::m_nMsgParagraphCount)��Ϣ����������
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

	// �õ���Ϣ��һ���ַ���������
	CPoint ptFirstChar;
	IMRE_CALL_FUN_RETURN(ptFirstChar, GetCharPos(pMsg->GetMsgStart() + pMsg->GetMsgContentStart()));

	lprcQiPao->left = rcClient.left + GetMsgLeftIndent();
	lprcQiPao->right = rcClient.right - GetMsgRightIndent();
	lprcQiPao->top = ptFirstChar.y;

	// ������Ϣ���һ�����һ���ַ������½�����
	int nLine = -1;
	IMRE_CALL_FUN_RETURN(nLine, LineFromChar(pMsg->GetMsgStart() + pMsg->GetMsgEnd()));
	CPoint ptLastMsgLine;
	int nLineHeight = 0;
	IMRE_CALL_FUN(GetLineRect(nLine, &ptLastMsgLine, NULL, &nLineHeight));

	lprcQiPao->bottom = ptLastMsgLine.y + nLineHeight;

	return 0;
}

// ���㲻����(��ʾ�������Ƿ����CIMMsg::m_nMsgParagraphCount)��Ϣ��ʾʱ����������
int CIMRichEditCtrlBase::DoCalSingleLineQiPaoRect(const CIMMsg *pMsg, int nStartLine, int nEndLine,
	__out LPRECT lprcQiPao)
{
	if (lprcQiPao == NULL)
	{
		return -1;
	}

	CRect rcQiPao;

	int nMinLeftLine = MAXINT; // �����У�left����Сֵ
	int nMaxRightLine = 0; // �����У�right�����ֵ

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

	// ��û��������������£������Ƿ���Ҫ������ʾ
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

	// ����������Ϣ
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

// ���Ʒ�Rich Text������ͷ������ݵȣ�ֻ���ƿ���Ƶ��Χ�ڵ���Ϣ��¼
// �ڻ���ǰ����ͷ����������꣬��������ܺ�׼ȷ�ȡ�
// �����WM_VSCROLLʱ���㣬�ᷢ�ֹ��������У����ݱ����ݹ��������ģ���ʹ�ȵ��û����OnVScroll����
int CIMRichEditCtrlBase::OnDrawNonRichText(CDC *pMemDCParent)
{
	// �����һ�ɼ���С��0
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
	// 1. ����RichEdit����Ϊ�ü�����
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
	// ����
	//
	int nMsgCount = (int)m_vMsg.size();
	for (int i = nFirstVisibleMsg; i < nMsgCount; ++i)
	{
		const CIMMsg *pMsg = m_vMsg[i];

		// �¶˳�����ʾ��Χ��ֹͣ����
		CPoint pt;
		IMRE_CALL_FUN_RETURN(pt, GetCharPos(pMsg->GetMsgStart()));
		if (pt.y >= rcClient.Height())
		{
			break;
		}

		// �õ�ͷ������
		CRect rcFace;
		DoCalFaceRect(pMsg, rcFace);

		// ����ͷ��
		OnDrawFace(pMemDCParent, i, rcFace);

		// ��������
		if (m_bQiPaoMode)
		{
			CRect rcQiPao;
			DoCalQiPaoRect(pMsg, rcQiPao);

			OnDrawQiPao(pMemDCParent, i, rcQiPao);
		}
	}

	// �ָ��ü�����
	::SelectClipRgn(pMemDCParent->GetSafeHdc(), hOldParentRgn);
	::DeleteObject(hOldParentRgn);

	return 0;
}

#ifdef _DEBUG
// ���Դ��롣���ⲿ���ã���RichEdit��ĳЩ���ܽ��в��ԡ�
int CIMRichEditCtrlBase::Test()
{

	return 0;
}
#endif // _DEBUG

// ��ͷ��, lprcFace�������RichEdit�ؼ��ģ����ͷ�񻭵��������ϣ�Ҫ��������ת��
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

	// ���㱾��Ϣ����Ϣ���е�λ�ã������£���DT_TOP��DT_CENTER��DT_BOTTOM��ʾ��
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

// ����Ϣ�ϲ�ʱ��������Ϣ�����ڵ��������������£���DT_TOP��DT_CENTER��DT_BOTTOM��ʾ��
int CIMRichEditCtrlBase::GetMsgPosInGroup(int nMsgIndex)
{
	int nMsgCount = (int)m_vMsg.size();
	if (nMsgCount <= 0 || nMsgIndex >= nMsgCount)
	{
		return -2;
	}

	const CIMMsg *pMsg = m_vMsg[nMsgIndex];

#ifdef _DEBUG
	size_t nc = pMsg->m_vpStrRichMsg.size();
	for (size_t j = 0; j < nc ; j++)
	{
		CRichMsgItem *pitem = pMsg->m_vpStrRichMsg[j];
		if (pitem->m_strMsgContent.Find(_T("������˵")) >= 0)
		{
			int n = 0;
			break;
		}
	}
#endif // _DEBUG

	// ��¼����Ϣ��Ϣ�ķ�����
	CString strSender = pMsg->GetMsgSender();

	//
	// ���㱾��Ϣ����Ϣ���е�λ�ã������£���DT_TOP��DT_CENTER��DT_BOTTOM��ʾ��
	//

	// ���ϲ��ұ����ݷ����߷��͵���Ϣ��ֱ���ҵ��Ǳ����ݷ����ߣ������ҵ�m_bGroupΪtrue
	// ������һ����Ϣ�뱾����Ϣʱ��ʱ��������
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

		// ����5���֣�������һ��(����һ��ʱ����Ϣ)
		if ((pMsg->m_tMsg - pMsgWalk->m_tMsg).GetTotalMinutes() > 5)
		{
			((CIMMsg *)pMsg)->m_bGroup = true;
			uMsgPos = DT_TOP;
			break;
		}
	}

	// �������Ϣ��Ϣ������ΪDT_CENTER�����ұ���û����һ����Ϣ��������ΪDT_BOTTOM
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

// ��ʹ�úϲ���Ϣģʽʱ�����ö��֮�ڵ���Ϣ��Ҫ�ϲ�
int CIMRichEditCtrlBase::SetCombineMsgTimeSpan(int nSecond)
{
	m_nCombineMsgTimeSpan = nSecond;
	return 0;
}

int CIMRichEditCtrlBase::GetCombineMsgTimeSpan() const
{
	return m_nCombineMsgTimeSpan;
}

// ������, lprcQiPao�������RichEdit�ؼ��ģ�������ݻ����������ϣ�Ҫ��������ת��
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

	// ���㱾��Ϣ����Ϣ���е�λ�ã������£���DT_TOP��DT_CENTER��DT_BOTTOM��ʾ��
	UINT uMsgPos = DT_TOP;
	if (IsCombineMsg())
	{
		uMsgPos = GetMsgPosInGroup(nMsgIndex);
	}

	// ��������һ���Ի����ݵĻ���
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
