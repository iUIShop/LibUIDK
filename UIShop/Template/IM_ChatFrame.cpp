// ChatFrame.cpp : implementation of the CChatFrame class
//

#include "stdafx.h"
#include "$$root$$.h"
#include "ChatFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatFrame

BEGIN_MESSAGE_MAP(CChatFrame, CUIWnd)
	//{{AFX_MSG_MAP(CChatFrame)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_VIDEO, OnBtnVideo)
	ON_BN_CLICKED(IDC_BTN_AUDIO, OnBtnAudio)
	ON_BN_CLICKED(IDC_BTN_TRANS_FILE, OnBtnTransFile)
	ON_BN_CLICKED(IDC_BTN_CREATE_GROUP, OnBtnCreateGroup)
	ON_BN_CLICKED(IDC_BTN_REMOTE, OnBtnRemote)
	ON_BN_CLICKED(IDC_BTN_MORE, OnBtnMore)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BTN_SEND2, OnBtnSend2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatFrame construction/destruction

CChatFrame::CChatFrame()
{
	m_pBtnMin = NULL;
	m_pChkMax = NULL;
	m_pBtnExit = NULL;
	m_pBtnVideo = NULL;
	m_pBtnAudio = NULL;
	m_pBtnTransFile = NULL;
	m_pBtnCreateGroup = NULL;
	m_pBtnRemote = NULL;
	m_pBtnMore = NULL;
	m_pChkFont = NULL;
	m_pChkFace = NULL;
	m_pBtnClose = NULL;
	m_pBtnSend = NULL;
	m_pBtnSend2 = NULL;

	m_bMax = FALSE;
	m_bFont = FALSE;
	m_bFace = FALSE;
}

CChatFrame::~CChatFrame()
{
}

void CChatFrame::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CChatFrame)
	DDX_Check(pDX, IDC_CHK_MAX, m_bMax);
	DDX_Check(pDX, IDC_CHK_FONT, m_bFont);
	DDX_Check(pDX, IDC_CHK_FACE, m_bFace);
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CChatFrame::InitControls()
{
	m_pBtnMin = (CSkinButton *)GetDlgItem(IDC_BTN_MIN);
	ASSERT(m_pBtnMin->GetSafeHwnd() != NULL);
	m_pChkMax = (CSkinButton *)GetDlgItem(IDC_CHK_MAX);
	ASSERT(m_pChkMax->GetSafeHwnd() != NULL);
	m_pBtnExit = (CSkinButton *)GetDlgItem(IDC_BTN_EXIT);
	ASSERT(m_pBtnExit->GetSafeHwnd() != NULL);
	m_pBtnVideo = (CSkinButton *)GetDlgItem(IDC_BTN_VIDEO);
	ASSERT(m_pBtnVideo->GetSafeHwnd() != NULL);
	m_pBtnAudio = (CSkinButton *)GetDlgItem(IDC_BTN_AUDIO);
	ASSERT(m_pBtnAudio->GetSafeHwnd() != NULL);
	m_pBtnTransFile = (CSkinButton *)GetDlgItem(IDC_BTN_TRANS_FILE);
	ASSERT(m_pBtnTransFile->GetSafeHwnd() != NULL);
	m_pBtnCreateGroup = (CSkinButton *)GetDlgItem(IDC_BTN_CREATE_GROUP);
	ASSERT(m_pBtnCreateGroup->GetSafeHwnd() != NULL);
	m_pBtnRemote = (CSkinButton *)GetDlgItem(IDC_BTN_REMOTE);
	ASSERT(m_pBtnRemote->GetSafeHwnd() != NULL);
	m_pBtnMore = (CSkinButton *)GetDlgItem(IDC_BTN_MORE);
	ASSERT(m_pBtnMore->GetSafeHwnd() != NULL);
	m_pChkFont = (CSkinButton *)GetDlgItem(IDC_CHK_FONT);
	ASSERT(m_pChkFont->GetSafeHwnd() != NULL);
	m_pChkFace = (CSkinButton *)GetDlgItem(IDC_CHK_FACE);
	ASSERT(m_pChkFace->GetSafeHwnd() != NULL);
	m_pBtnClose = (CSkinButton *)GetDlgItem(IDC_BTN_CLOSE);
	ASSERT(m_pBtnClose->GetSafeHwnd() != NULL);
	m_pBtnSend = (CSkinButton *)GetDlgItem(IDC_BTN_SEND);
	ASSERT(m_pBtnSend->GetSafeHwnd() != NULL);
	m_pBtnSend2 = (CSkinButton *)GetDlgItem(IDC_BTN_SEND2);
	ASSERT(m_pBtnSend2->GetSafeHwnd() != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CChatFrame message handlers

int CChatFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;


	return 0;
}

void CChatFrame::OnBtnVideo()
{
	MessageBox(_T("IDC_BTN_VIDEO"));
}

void CChatFrame::OnBtnAudio()
{
	MessageBox(_T("IDC_BTN_AUDIO"));
}

void CChatFrame::OnBtnTransFile()
{
	MessageBox(_T("IDC_BTN_TRANS_FILE"));
}

void CChatFrame::OnBtnCreateGroup()
{
	MessageBox(_T("IDC_BTN_CREATE_GROUP"));
}

void CChatFrame::OnBtnRemote()
{
	MessageBox(_T("IDC_BTN_REMOTE"));
}

void CChatFrame::OnBtnMore()
{
	MessageBox(_T("IDC_BTN_MORE"));
}

void CChatFrame::OnBtnSend()
{
	MessageBox(_T("IDC_BTN_SEND"));
}

void CChatFrame::OnBtnSend2()
{
	MessageBox(_T("IDC_BTN_SEND2"));
}
