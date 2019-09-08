#ifndef _IDMGR_H_
#define _IDMGR_H_

class CIDMgr
{
public:
	CIDMgr();
	virtual ~CIDMgr();

	// manager the control's ID.
	static CString NextConditionFontChildID(FONTRESOURCEITEM *pResFont);
	static CString NextPnlStackID();
	static CString NextPnlUniformGridID();
	static CString NextPnlDockID();
	static CString NextTaskWndMgrID();
	static CString NextWLIMID();
	static CString NextWLRichEditID();
	static CString NextWLPicID();
	static CString NextWLSliderID();
	static CString NextWLBtnID();
	static CString NextWLChkID();
	static CString NextWLRadID();
	static CString NextWLTextID();
	static CString NextWLLineID();
	static CString NextWLSplitterID();
	static CString NextWLRectID();
	static CString NextVerScrollbarID();
	static CString NextHorScrollbarID();
	static CString NextHeaderID();
	static CString NextSplitterID();
	static CString NextMenuBarID();
	static CString NextMenuID();
	static CString NextPicCtrlID();
	static CString NextWaveCtrlID();
	static CString NextDateTimeCtrlID();
	static CString NextHtmlCtrlID();
	static CString NextRichEditID();
	static CString NextIMRichEditID();
	static CString NextTreeLstID();
	static CString NextThumbnailID();
	static CString NextScrollBarExID();
	static CString NextTreeCtrlID();
	static CString NextListCtrlID();
	static CString NextSliderExID();
	static CString NextIPAddressID();
	static CString NextHotKeyID();
	static CString NextWindowID();
	static CString NextSpinID();
	static CString NextSliderID();
	static CString NextProgressID();
	static CString NextStaticID();
	static CString NextComboBoxID();
	static CString NextEditID();
	static CString NextRadioButtonID();
	static CString NextCheckBoxID();
	static CString NextButtonID();

	// manager resource ID
	static CString NextFontResID();
	static CString NextColorResID();

	static int NextControlIDValue();

private:
	static int m_nWndStart;
	static int m_nBtnStart;
	static int m_nChkStart;
	static int m_nRadStart;
	static int m_nEdtStart;
	static int m_nCmbStart;
	static int m_nStaStart;
	static int m_nPrgStart;
	static int m_nSldStart;
	static int m_nSpnStart;
	static int m_nHotStart;
	static int m_nIPAStart;
	static int m_nSldExStart;
	static int m_nLstCtrlStart;
	static int m_nTreeCtrlStart;
	static int m_nScrollBarExStart;
	static int m_nThumbnailStart;
	static int m_nTreeLstStart;
	static int m_nRichEditStart;
	static int m_nIMRichEditStart;
	static int m_nHtmlCtrlStart;
	static int m_nDateTimeCtrlStart;
	static int m_nWaveCtrlStart;
	static int m_nPicCtrlStart;
	static int m_nMenuStart;
	static int m_nMenuBarStart;
	static int m_nSplitterStart;
	static int m_nHeaderStart;
	static int m_nHorScrollbarStart;
	static int m_nVerScrollbarStart;
	static int m_nWLRectStart;
	static int m_nWLSplitterStart;
	static int m_nWLLineStart;
	static int m_nWLTextStart;
	static int m_nWLBtnStart;
	static int m_nWLChkStart;
	static int m_nWLRadStart;
	static int m_nWLSldStart;
	static int m_nWLPicStart;
	static int m_nWLRichEditStart;
	static int m_nWLIMStart;
	static int m_nTaskWndMgrStart;
	static int m_nPnlDockStart;
	static int m_nPnlUniformGridStart;
	static int m_nPnlStackStart;

	static int m_nFontResStart;
	static int m_nColorResStart;

	static int m_nNextControlID;
};

#endif //_IDMGR_H_