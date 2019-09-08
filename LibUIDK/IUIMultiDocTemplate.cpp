// IUIMultiDocTemplate.cpp: implementation of the CIUIMultiDocTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIUIMultiDocTemplate::CIUIMultiDocTemplate(UINT nIDResource, CRuntimeClass *pDocClass, CRuntimeClass *pFrameClass, CRuntimeClass *pViewClass)
	: CMultiDocTemplate(nIDResource, pDocClass, NULL, NULL)
{
	ASSERT(m_docList.IsEmpty());

	m_pFrameClass = pFrameClass;
	m_pViewClass = pViewClass;

	m_uViewID = 1;
}

CIUIMultiDocTemplate::~CIUIMultiDocTemplate()
{

}

//////////////////////////////////////////////////////////////////////////
// public

void CIUIMultiDocTemplate::InitialUpdateFrame(CIUIMDIChildWnd *pFame, CIUIDocument *pDoc, BOOL bMakeVisible/* = TRUE*/)
{
	pFame->InitialUpdateFrame(pDoc, bMakeVisible);
}

CIUIMDIChildWnd *CIUIMultiDocTemplate::CreateNewIUIFrame(CIUIDocument *pDoc)
{
	if (pDoc != NULL)
	{
		ASSERT_VALID(pDoc);
	}
	// create a frame wired to the specified document

	ASSERT(m_nIDResource != 0); // must have a resource ID to load from
	CCreateContext context;
	context.m_pCurrentFrame = NULL;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = m_pViewClass;
	context.m_pNewDocTemplate = this;

	if (m_pFrameClass == NULL)
	{
		TRACE0("[LibUIDK]===Error: you must override CDocTemplate::CreateNewFrame.\n");
		ASSERT(FALSE);
		return NULL;
	}
	CIUIMDIChildWnd *pFrame = (CIUIMDIChildWnd *)m_pFrameClass->CreateObject();
	if (pFrame == NULL)
	{
		TRACE1("[LibUIDK]===Warning: Dynamic create of frame %hs failed.\n",
			m_pFrameClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CIUIMDIChildWnd, pFrame);

	if (context.m_pNewViewClass == NULL)
	{
		TRACE0("[LibUIDK]===Warning: creating frame with no default view.\n");
	}

	// create new from resource
	if (!pFrame->LoadFrame(m_nIDResource,
			/*WS_OVERLAPPEDWINDOW | */FWS_ADDTOTITLE,   // default frame styles
			NULL, &context))
	{
		TRACE0("[LibUIDK]===Warning: CDocTemplate couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	// it worked !
	return pFrame;
}

CDocument *CIUIMultiDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	CIUIDocument *pDocument = (CIUIDocument *)CreateNewDocument();
	if (pDocument == NULL)
	{
		TRACE0("[LibUIDK]===CDocTemplate::CreateNewDocument returned NULL.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT_VALID(pDocument);
	ASSERT_KINDOF(CIUIDocument, pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CIUIMDIChildWnd *pFrame = CreateNewIUIFrame(pDocument);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		delete pDocument;       // explicit delete on error
		return NULL;
	}
	ASSERT_VALID(pFrame);

	if (lpszPathName == NULL)
	{
		// create a new document - with default document name
		SetDefaultTitle(pDocument);

		// avoid creating temporary compound file when starting up invisible
		if (!bMakeVisible)
		{
			pDocument->m_bEmbedded = TRUE;
		}

		if (!pDocument->OnNewDocument())
		{
			// user has be alerted to what failed in OnNewDocument
			TRACE0("[LibUIDK]===CDocument::OnNewDocument returned FALSE.\n");
			pFrame->DestroyWindow();
			return NULL;
		}

		// it worked, now bump untitled count
		m_nUntitledCount++;
	}
	else
	{
		// open an existing document
		CWaitCursor wait;
		if (!pDocument->OnOpenDocument(lpszPathName))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("[LibUIDK]===CDocument::OnOpenDocument returned FALSE.\n");
			pFrame->DestroyWindow();
			return NULL;
		}
		pDocument->SetPathName(lpszPathName);
	}

	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
	return pDocument;
}
