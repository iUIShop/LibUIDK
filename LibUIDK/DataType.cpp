#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if(_MSC_VER <= 1200) // VC6
#pragma comment(lib, "comsupp.lib") // for ConvertBSTRToString
#else
#pragma comment(lib, "comsuppw.lib") // for ConvertBSTRToString
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// Constructors

void CopyVariantReservedValue(IUI_VARIANT *pThis, const _variant_t *pSrc)
{
	pThis->wReserved1 = pSrc->wReserved1;
	pThis->wReserved2 = pSrc->wReserved2;
	pThis->wReserved3 = pSrc->wReserved3;
}

IUI_VARIANT::IUI_VARIANT() throw() : _variant_t()
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(const _variant_t &varSrc) : _variant_t(varSrc)
{
	Init();
	CopyVariantReservedValue(this, &varSrc);
}

IUI_VARIANT::IUI_VARIANT(const _variant_t *pSrc) : _variant_t(pSrc)
{
	Init();
	CopyVariantReservedValue(this, pSrc);
}

IUI_VARIANT::IUI_VARIANT(const IUI_VARIANT &varSrc) : _variant_t(varSrc)
{
	Init();
	IUI_VARIANT_Copy(varSrc);
}

IUI_VARIANT::IUI_VARIANT(_variant_t &varSrc, bool fCopy) : _variant_t(varSrc, fCopy)
{
	Init();
	CopyVariantReservedValue(this, &varSrc);
}

IUI_VARIANT::IUI_VARIANT(short sSrc, VARTYPE vtSrc) : _variant_t(sSrc, vtSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(long lSrc, VARTYPE vtSrc) : _variant_t(lSrc, vtSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(float fltSrc) throw(): _variant_t(fltSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(double dblSrc, VARTYPE vtSrc) : _variant_t(dblSrc, vtSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(const CY &cySrc) throw(): _variant_t(cySrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(const _bstr_t &bstrSrc) : _variant_t(bstrSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(const wchar_t *pSrc) : _variant_t(pSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(const char *pSrc) : _variant_t(pSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(IDispatch *pSrc, bool fAddRef) throw() : _variant_t(pSrc, fAddRef)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(bool boolSrc) throw() : _variant_t(boolSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(IUnknown *pSrc, bool fAddRef) throw() : _variant_t(pSrc, fAddRef)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(const DECIMAL &decSrc) throw() : _variant_t(decSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(BYTE bSrc) throw() : _variant_t(bSrc)
{
	Init();
}

#if(_MSC_VER > 1200) // VC6.0
IUI_VARIANT::IUI_VARIANT(char cSrc) throw() : _variant_t(cSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(unsigned short usSrc) throw() : _variant_t(usSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(unsigned long ulSrc) throw() : _variant_t(ulSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(int iSrc) throw() : _variant_t(iSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(unsigned int uiSrc) throw() : _variant_t(uiSrc)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(__int64 i8Src) throw() : _variant_t(i8Src)
{
	Init();
}

IUI_VARIANT::IUI_VARIANT(unsigned __int64 ui8Src) throw() : _variant_t(ui8Src)
{
	Init();
}
#endif

IUI_VARIANT::IUI_VARIANT(BYTE *pbVal)
{
	Init();

	V_VT(this) = (VT_BYREF | VT_UI1);
	V_UI1REF(this) = pbVal;
}

IUI_VARIANT::IUI_VARIANT(RECT rcSrc) throw()
{
	Init();

	V_VT(this) = VT_ARRAY | VT_I4;
	parray = SafeArrayCreateVector(VT_I4, 0, 4);
	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	memcpy(pData, &rcSrc, sizeof(RECT));
	SafeArrayUnaccessData(parray);
}

IUI_VARIANT::IUI_VARIANT(POINT ptSrc) throw()
{
	Init();

	V_VT(this) = VT_ARRAY | VT_I4;
	parray = SafeArrayCreateVector(VT_I4, 0, 2);
	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	memcpy(pData, &ptSrc, sizeof(POINT));
	SafeArrayUnaccessData(parray);
}

IUI_VARIANT::IUI_VARIANT(LOGFONT lfSrc) throw()
{
	Init();

	V_VT(this) = VT_ARRAY | VT_UI1;
	parray = SafeArrayCreateVector(VT_UI1, 0, sizeof(LOGFONT));
	BYTE *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	memcpy(pData, &lfSrc, sizeof(LOGFONT));
	SafeArrayUnaccessData(parray);
}

IUI_VARIANT::IUI_VARIANT(CONTROL_TYPE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_CONTROL_TYPE;
}

IUI_VARIANT::IUI_VARIANT(IMAGE_RESIZE_MODE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_IMAGE_RESIZE_MODE;
}

IUI_VARIANT::IUI_VARIANT(CONTROL_LAYOUT_HOR eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_LAYOUT_HOR;
}

IUI_VARIANT::IUI_VARIANT(CONTROL_LAYOUT_VER eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_LAYOUT_VER;
}

IUI_VARIANT::IUI_VARIANT(TEXT_ALIGN_HOR eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_TEXT_ALIGN_HOR;
}

IUI_VARIANT::IUI_VARIANT(TEXT_ALIGN_VER eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_TEXT_ALIGN_VER;
}

IUI_VARIANT::IUI_VARIANT(FOREGROUND_ALIGN_HOR eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_FOREGROUND_ALIGN_HOR;
}

IUI_VARIANT::IUI_VARIANT(FOREGROUND_ALIGN_VER eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_FOREGROUND_ALIGN_VER;
}

IUI_VARIANT::IUI_VARIANT(BTN_TYPE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_BUTTON_TYPE;
}

IUI_VARIANT::IUI_VARIANT(CHK_TYPE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_CHECKBOX_TYPE;
}

IUI_VARIANT::IUI_VARIANT(WINDOW_TYPE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_WINDOW_TYPE;
}

IUI_VARIANT::IUI_VARIANT(CMB_STYLE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_COMBOBOX_STYLE;
}

IUI_VARIANT::IUI_VARIANT(SPN_ALIGN eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_SPN_ALIGN;
}

IUI_VARIANT::IUI_VARIANT(SLD_EXT_STYLE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_SLD_EXT_STYLE;
}

IUI_VARIANT::IUI_VARIANT(LST_STYLE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_LST_STYLE;
}

IUI_VARIANT::IUI_VARIANT(LST_ALIGN eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_LST_ALIGN;
}

IUI_VARIANT::IUI_VARIANT(LST_SORT eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_LST_SORT;
}

IUI_VARIANT::IUI_VARIANT(DATETIME_FORMAT eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_DATETIME_FORMAT;
}

IUI_VARIANT::IUI_VARIANT(BACKGROUND_TYPE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_BACKGROUND_TYPE;
}

IUI_VARIANT::IUI_VARIANT(PROGRESS_THUMB_MODE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_PROGRESS_THUMB_MODE;
}

IUI_VARIANT::IUI_VARIANT(PROGRESS_THUMB_POSITION eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_PROGRESS_THUMB_POSITION;
}

IUI_VARIANT::IUI_VARIANT(ALPHA_FORMAT eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_ALPHA_FORMAT;
}

IUI_VARIANT::IUI_VARIANT(MENU_BORDER_STYLE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_MENU_BORDER_STYLE;
}

IUI_VARIANT::IUI_VARIANT(SPLITTER_ORIENTATION eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_SPLITTER_ORIENTATION;
}

IUI_VARIANT::IUI_VARIANT(STACK_PANEL_ORIENTATION eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_STACK_PANEL_ORIENTATION;
}

IUI_VARIANT::IUI_VARIANT(LINE_PEN_STYLE eSrc) throw()
{
	Init();

	V_VT(this) = VT_INT;
	V_INT(this) = eSrc;
	vtEx = VT_EX_PEN_STYLE;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Extractors

#if(_MSC_VER <= 1200) // VC6.0

IUI_VARIANT::operator int() const throw()
{
	if (V_VT(this) == VT_I4)
	{
		return V_I4(this);
	}

	_variant_t varDest;

	varDest.ChangeType(VT_I4, this);

	return V_I4(&varDest);
}

#endif

IUI_VARIANT::operator BYTE *() const throw()
{
	if (V_VT(this) != (VT_BYREF | VT_UI1))
	{
		ASSERT(FALSE);
		return NULL;
	}

	return V_UI1REF(this);
}

IUI_VARIANT::operator _variant_t() const throw()
{
	return *(_variant_t *) this;
}

IUI_VARIANT::operator LPCWSTR() const throw()
{
	if (V_VT(this) == VT_BSTR)
	{
		return V_BSTR(this);
	}

	_variant_t varDest;
	varDest.ChangeType(VT_BSTR, this);

	return V_BSTR(&varDest);
}

IUI_VARIANT::operator LPCSTR() const throw()
{
	if (V_VT(this) == VT_BSTR)
	{
		if (strA != NULL)
		{
			delete[] strA;
		}
		strA = _com_util::ConvertBSTRToString(V_BSTR(this));
		return strA;
	}

	_variant_t varDest;
	varDest.ChangeType(VT_BSTR, this);

	if (strA != NULL)
	{
		delete[] strA;
	}
	strA = _com_util::ConvertBSTRToString(V_BSTR(this));
	return strA;
}

IUI_VARIANT::operator RECT() const throw()
{
	if (V_VT(this) == (VT_ARRAY | VT_I4) && (vtEx == VT_EX_RECT || vtEx == VT_EX_RECT_WH))
	{
		RECT rect;
		LONG *pData = NULL;
		SafeArrayAccessData(parray, (void **)&pData);
		memcpy(&rect, pData, sizeof(RECT));
		SafeArrayUnaccessData(parray);

		return rect;
	}

	RECT rcRet;
	rcRet.left = rcRet.right = rcRet.top = rcRet.bottom = -1;

	return rcRet;
}

IUI_VARIANT::operator CRect() const throw()
{
	return operator RECT();
}

IUI_VARIANT::operator POINT() const throw()
{
	if (V_VT(this) == (VT_ARRAY | VT_I4) && vtEx == VT_EX_POINT)
	{
		POINT pt;
		LONG *pData = NULL;
		SafeArrayAccessData(parray, (void **)&pData);
		memcpy(&pt, pData, sizeof(POINT));
		SafeArrayUnaccessData(parray);

		return pt;
	}

	POINT ptRet;
	ptRet.x = ptRet.y = -1;

	return ptRet;
}

IUI_VARIANT::operator COLORREF() const throw()
{
	if (vtEx != VT_EX_COLOR || V_VT(this) != VT_I4)
	{
		ASSERT(FALSE);
		return -1;
	}

	return V_I4(this);
}

IUI_VARIANT::operator LOGFONT() const throw()
{
	LOGFONT lf;
	if (V_VT(this) == (VT_ARRAY | VT_UI1) && vtEx == VT_EX_FONT)
	{
		BYTE *pData = NULL;
		SafeArrayAccessData(parray, (void **)&pData);
		memcpy(&lf, pData, sizeof(LOGFONT));
		SafeArrayUnaccessData(parray);
	}

	return lf;
}

IUI_VARIANT::operator CONTROL_TYPE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_CONTROL_TYPE)
	{
		return (CONTROL_TYPE)V_INT(this);
	}

	return CT_ARROW;
}

IUI_VARIANT::operator IMAGE_RESIZE_MODE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_IMAGE_RESIZE_MODE)
	{
		return (IMAGE_RESIZE_MODE)V_INT(this);
	}

	return IRM_UNKNOWN;
}

IUI_VARIANT::operator CONTROL_LAYOUT_HOR() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_LAYOUT_HOR)
	{
		return (CONTROL_LAYOUT_HOR)V_INT(this);
	}

	return CLH_UNKNOWN;
}

IUI_VARIANT::operator CONTROL_LAYOUT_VER() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_LAYOUT_VER)
	{
		return (CONTROL_LAYOUT_VER)V_INT(this);
	}

	return CLV_UNKNOWN;
}

IUI_VARIANT::operator TEXT_ALIGN_HOR() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_TEXT_ALIGN_HOR)
	{
		return (TEXT_ALIGN_HOR)V_INT(this);
	}

	return TAH_UNKNOWN;
}

IUI_VARIANT::operator TEXT_ALIGN_VER() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_TEXT_ALIGN_VER)
	{
		return (TEXT_ALIGN_VER)V_INT(this);
	}

	return TAV_UNKNOWN;
}

IUI_VARIANT::operator FOREGROUND_ALIGN_HOR() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_FOREGROUND_ALIGN_HOR)
	{
		return (FOREGROUND_ALIGN_HOR)V_INT(this);
	}

	return FAH_UNKNOWN;
}

IUI_VARIANT::operator FOREGROUND_ALIGN_VER() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_FOREGROUND_ALIGN_VER)
	{
		return (FOREGROUND_ALIGN_VER)V_INT(this);
	}

	return FAV_UNKNOWN;
}

IUI_VARIANT::operator BTN_TYPE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_BUTTON_TYPE)
	{
		return (BTN_TYPE)V_INT(this);
	}

	return BT_UNKNOWN;
}

IUI_VARIANT::operator CHK_TYPE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_CHECKBOX_TYPE)
	{
		return (CHK_TYPE)V_INT(this);
	}

	return CT_UNKNOWN;
}

IUI_VARIANT::operator WINDOW_TYPE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_WINDOW_TYPE)
	{
		return (WINDOW_TYPE)V_INT(this);
	}

	return WT_UNKNOWN;
}

IUI_VARIANT::operator CMB_STYLE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_COMBOBOX_STYLE)
	{
		return (CMB_STYLE)V_INT(this);
	}

	return CS_UNKNOWN;
}

IUI_VARIANT::operator SPN_ALIGN() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_SPN_ALIGN)
	{
		return (SPN_ALIGN)V_INT(this);
	}

	return SPNA_UNKNOWN;
}

IUI_VARIANT::operator SLD_EXT_STYLE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_SLD_EXT_STYLE)
	{
		return (SLD_EXT_STYLE)V_INT(this);
	}

	return SLDES_UNKNOWN;
}

IUI_VARIANT::operator LST_STYLE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_LST_STYLE)
	{
		return (LST_STYLE)V_INT(this);
	}

	return LSTS_UNKNOWN;
}

IUI_VARIANT::operator LST_ALIGN() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_LST_ALIGN)
	{
		return (LST_ALIGN)V_INT(this);
	}

	return LSTA_UNKNOWN;
}

IUI_VARIANT::operator LST_SORT() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_LST_SORT)
	{
		return (LST_SORT)V_INT(this);
	}

	return LSTST_UNKNOWN;
}

IUI_VARIANT::operator DATETIME_FORMAT() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_DATETIME_FORMAT)
	{
		return (DATETIME_FORMAT)V_INT(this);
	}

	return DTF_UNKNOWN;
}

IUI_VARIANT::operator BACKGROUND_TYPE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_BACKGROUND_TYPE)
	{
		return (BACKGROUND_TYPE)V_INT(this);
	}

	return BKT_UNKNOWN;
}

IUI_VARIANT::operator PROGRESS_THUMB_MODE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_PROGRESS_THUMB_MODE)
	{
		return (PROGRESS_THUMB_MODE)V_INT(this);
	}

	return PTM_UNKNOWN;
}

IUI_VARIANT::operator PROGRESS_THUMB_POSITION() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_PROGRESS_THUMB_POSITION)
	{
		return (PROGRESS_THUMB_POSITION)V_INT(this);
	}

	return PTP_UNKNOWN;
}

IUI_VARIANT::operator ALPHA_FORMAT() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_ALPHA_FORMAT)
	{
		return (ALPHA_FORMAT)V_INT(this);
	}

	return AF_UNKNOWN;
}

IUI_VARIANT::operator MENU_BORDER_STYLE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_MENU_BORDER_STYLE)
	{
		return (MENU_BORDER_STYLE)V_INT(this);
	}

	return MBS_UNKNOWN;
}

IUI_VARIANT::operator SPLITTER_ORIENTATION() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_SPLITTER_ORIENTATION)
	{
		return (SPLITTER_ORIENTATION)V_INT(this);
	}

	return SO_UNKNOWN;
}

IUI_VARIANT::operator STACK_PANEL_ORIENTATION() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_STACK_PANEL_ORIENTATION)
	{
		return (STACK_PANEL_ORIENTATION)V_INT(this);
	}

	return SPO_LEFT;
}

IUI_VARIANT::operator LINE_PEN_STYLE() const throw()
{
	if (V_VT(this) == VT_INT && vtEx == VT_EX_PEN_STYLE)
	{
		return (LINE_PEN_STYLE)V_INT(this);
	}

	return LPS_UNKNOWN;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Assignment operations

IUI_VARIANT &IUI_VARIANT::operator=(const _variant_t &varSrc)
{
	_variant_t::operator=(varSrc);
	CopyVariantReservedValue(this, &varSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const _variant_t *pSrc)
{
	_variant_t::operator=(pSrc);
	CopyVariantReservedValue(this, pSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const IUI_VARIANT &varSrc)
{
	_variant_t::operator=(varSrc);
	IUI_VARIANT_Copy(varSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(short sSrc)
{
	_variant_t::operator=(sSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(long lSrc)
{
	_variant_t::operator=(lSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(float fltSrc)
{
	_variant_t::operator=(fltSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(double dblSrc)
{
	_variant_t::operator=(dblSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const CY &cySrc)
{
	_variant_t::operator=(cySrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const _bstr_t &bstrSrc)
{
	_variant_t::operator=(bstrSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const wchar_t *pSrc)
{
	_variant_t::operator=(pSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const char *pSrc)
{
	_variant_t::operator=(pSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(IDispatch *pSrc)
{
	_variant_t::operator=(pSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(bool boolSrc)
{
	_variant_t::operator=(boolSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(IUnknown *pSrc)
{
	_variant_t::operator=(pSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const DECIMAL &decSrc)
{
	_variant_t::operator=(decSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(BYTE bSrc)
{
	_variant_t::operator=(bSrc);

	return *this;
}

#if(_MSC_VER > 1200) // VC6.0
IUI_VARIANT &IUI_VARIANT::operator=(char cSrc)
{
	_variant_t::operator=(cSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(unsigned short usSrc)
{
	_variant_t::operator=(usSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(unsigned long ulSrc)
{
	_variant_t::operator=(ulSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(int iSrc)
{
	_variant_t::operator=(iSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(unsigned int uiSrc)
{
	_variant_t::operator=(uiSrc);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(__int64 i8Src)
{
	_variant_t::operator=(i8Src);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(unsigned __int64 ui8Src)
{
	_variant_t::operator=(ui8Src);

	return *this;
}
#endif

IUI_VARIANT &IUI_VARIANT::operator=(BYTE *pbVal)
{
	if (V_VT(this) != (VT_BYREF | VT_UI1))
	{
		Clear();
		V_VT(this) = (VT_UI1 | VT_BYREF);
	}

	V_UI1REF(this) = pbVal;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const RECT &rcSrc)
{
	// Release the old data and memory
	IUI_VARIANT_Clear();

	V_VT(this) = VT_ARRAY | VT_I4;
	if (vtEx != VT_EX_RECT && vtEx != VT_EX_RECT_WH)
	{
		vtEx = VT_EX_RECT;
	}

	parray = SafeArrayCreateVector(VT_I4, 0, 4);
	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	memcpy(pData, &rcSrc, sizeof(RECT));
	SafeArrayUnaccessData(parray);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const POINT &ptSrc)
{
	// Release the old data and memory
	IUI_VARIANT_Clear();

	V_VT(this) = VT_ARRAY | VT_I4;
	vtEx = VT_EX_POINT;

	parray = SafeArrayCreateVector(VT_I4, 0, 2);
	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	memcpy(pData, &ptSrc, sizeof(POINT));
	SafeArrayUnaccessData(parray);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(const LOGFONT &lfSrc)
{
	// Release the old data and memory
	IUI_VARIANT_Clear();

	V_VT(this) = VT_ARRAY | VT_UI1;
	vtEx = VT_EX_FONT;

	parray = SafeArrayCreateVector(VT_UI1, 0, sizeof(LOGFONT));
	BYTE *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	memcpy(pData, &lfSrc, sizeof(LOGFONT));
	SafeArrayUnaccessData(parray);

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(CONTROL_TYPE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_CONTROL_TYPE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(IMAGE_RESIZE_MODE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_IMAGE_RESIZE_MODE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(CONTROL_LAYOUT_HOR eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_LAYOUT_HOR;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(CONTROL_LAYOUT_VER eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_LAYOUT_VER;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(TEXT_ALIGN_HOR eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_TEXT_ALIGN_HOR;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(TEXT_ALIGN_VER eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_TEXT_ALIGN_VER;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(FOREGROUND_ALIGN_HOR eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_FOREGROUND_ALIGN_HOR;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(FOREGROUND_ALIGN_VER eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_FOREGROUND_ALIGN_VER;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(BTN_TYPE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_BUTTON_TYPE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(CHK_TYPE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_CHECKBOX_TYPE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(WINDOW_TYPE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_WINDOW_TYPE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(CMB_STYLE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_COMBOBOX_STYLE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(SPN_ALIGN eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_SPN_ALIGN;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(SLD_EXT_STYLE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_SLD_EXT_STYLE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(LST_STYLE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_LST_STYLE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(LST_ALIGN eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_LST_ALIGN;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(LST_SORT eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_LST_SORT;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(DATETIME_FORMAT eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_DATETIME_FORMAT;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(BACKGROUND_TYPE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_BACKGROUND_TYPE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(PROGRESS_THUMB_MODE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_PROGRESS_THUMB_MODE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(PROGRESS_THUMB_POSITION eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_PROGRESS_THUMB_POSITION;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(ALPHA_FORMAT eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_ALPHA_FORMAT;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(MENU_BORDER_STYLE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_MENU_BORDER_STYLE;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(SPLITTER_ORIENTATION eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_SPLITTER_ORIENTATION;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(STACK_PANEL_ORIENTATION eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_STACK_PANEL_ORIENTATION;
	V_INT(this) = eSrc;

	return *this;
}

IUI_VARIANT &IUI_VARIANT::operator=(LINE_PEN_STYLE eSrc)
{
	if (V_VT(this) != VT_INT)
	{
		IUI_VARIANT_Clear();
	}

	V_VT(this) = VT_INT;
	vtEx = VT_EX_PEN_STYLE;
	V_INT(this) = eSrc;

	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Comparison operations

bool IUI_VARIANT::operator==(const _variant_t &varSrc) const throw()
{
	if (V_VT(this) == (VT_ARRAY | VT_UI1) && vtEx == VT_EX_FONT)
	{
		BYTE *pData1 = NULL;
		SafeArrayAccessData(parray, (void **)&pData1);

		BYTE *pData2 = NULL;
		SafeArrayAccessData(varSrc.parray, (void **)&pData2);

		if (memcmp(pData1, pData2, sizeof(LOGFONT)) != 0)
		{
			SafeArrayUnaccessData(parray);
			SafeArrayUnaccessData(varSrc.parray);
			return false;
		}

		SafeArrayUnaccessData(parray);
		SafeArrayUnaccessData(varSrc.parray);

		return true;
	}

	if (V_VT(this) == (VT_ARRAY | VT_I4))
	{
		if (vtEx == VT_EX_RECT || vtEx == VT_EX_RECT_WH)
		{
			LONG *pData1 = NULL;
			SafeArrayAccessData(parray, (void **)&pData1);

			LONG *pData2 = NULL;
			SafeArrayAccessData(varSrc.parray, (void **)&pData2);

			if (memcmp(pData1, pData2, sizeof(RECT)) != 0)
			{
				SafeArrayUnaccessData(parray);
				SafeArrayUnaccessData(varSrc.parray);
				return false;
			}

			SafeArrayUnaccessData(parray);
			SafeArrayUnaccessData(varSrc.parray);

			return true;
		}
		else if (vtEx == VT_EX_POINT)
		{
			LONG *pData1 = NULL;
			SafeArrayAccessData(parray, (void **)&pData1);

			LONG *pData2 = NULL;
			SafeArrayAccessData(varSrc.parray, (void **)&pData2);

			if (memcmp(pData1, pData2, sizeof(POINT)) != 0)
			{
				SafeArrayUnaccessData(parray);
				SafeArrayUnaccessData(varSrc.parray);
				return false;
			}

			SafeArrayUnaccessData(parray);
			SafeArrayUnaccessData(varSrc.parray);

			return true;
		}
	}

	return *this == &varSrc;
}

#pragma warning(push)
#pragma warning(disable: 4702) // unreachable code

bool IUI_VARIANT::operator==(const _variant_t *pSrc) const throw()
{
	return _variant_t::operator==(pSrc);
}

#pragma warning(pop)

bool IUI_VARIANT::operator!=(const _variant_t &varSrc) const throw()
{
	if (V_VT(this) == (VT_ARRAY | VT_UI1) && vtEx == VT_EX_FONT)
	{
		BYTE *pData1 = NULL;
		SafeArrayAccessData(parray, (void **)&pData1);

		BYTE *pData2 = NULL;
		SafeArrayAccessData(varSrc.parray, (void **)&pData2);

		if (memcmp(pData1, pData2, sizeof(LOGFONT)) != 0)
		{
			SafeArrayUnaccessData(parray);
			SafeArrayUnaccessData(varSrc.parray);
			return true;
		}

		SafeArrayUnaccessData(parray);
		SafeArrayUnaccessData(varSrc.parray);

		return false;
	}

	return !(*this == &varSrc);
}

bool IUI_VARIANT::operator!=(const _variant_t *pSrc) const throw()
{
	return !(*this == pSrc);
}

bool IUI_VARIANT::operator==(LPCTSTR lpszSrc) const throw()
{
	if (vt != VT_BSTR)
	{
		ASSERT(FALSE);
		return false;
	}

	CString str = (TCHAR *)_bstr_t(bstrVal);
	return (str == lpszSrc);
}

bool IUI_VARIANT::operator!=(LPCTSTR lpszSrc) const throw()
{
	if (vt != VT_BSTR)
	{
		ASSERT(FALSE);
		return true;
	}

	CString str = (TCHAR *)_bstr_t(bstrVal);
	return (str != lpszSrc);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Low-level operations

void IUI_VARIANT::Init()
{
	vtEx = VT_EMPTY;
	strA = NULL;
	bSaveToSkinFile = true;
	bNeedToConvert = false;
	bReadOnly = false;
	strXUIAttributeName = _T("Value");
	bShowInPropertiesWnd = true;
	bGroup = false;
	nMinValue = 0;
	nMaxValue = 0;

	bDisabledWhileBindIs = false;
	bDisabledWhileBindIsnot = false;
	pvarBind = NULL;
	pvarCompare = NULL;
	lBeBindCount = 0;

	wReserved1 = 0;
	wReserved2 = 0;
	wReserved3 = 0;
}

void IUI_VARIANT::IUI_VARIANT_Copy(const IUI_VARIANT &varSrc)
{
	if (&varSrc == this)
	{
		return;
	}

	vtEx = varSrc.vtEx;
	bSaveToSkinFile = varSrc.bSaveToSkinFile;
	bReadOnly = varSrc.bReadOnly;
	strVarName = varSrc.strVarName;
	strUIKeyName = varSrc.strUIKeyName;
	strXUIElementName = varSrc.strXUIElementName;
	strXUIAttributeName = varSrc.strXUIAttributeName;
	bShowInPropertiesWnd = varSrc.bShowInPropertiesWnd;
	bGroup = varSrc.bGroup;
	strGroupName = varSrc.strGroupName;
	strPropWndName = varSrc.strPropWndName;
	strDescription = varSrc.strDescription;
	POSITION pos = varSrc.lstStringOptions.GetHeadPosition();
	while (pos != NULL)
	{
		lstStringOptions.AddTail(varSrc.lstStringOptions.GetNext(pos));
	}
	nMinValue = varSrc.nMinValue;
	nMaxValue = varSrc.nMaxValue;

	CopyVariantReservedValue(this, &varSrc);
}

void IUI_VARIANT::IUI_VARIANT_Clear()
{
	Clear();
	if (V_VT(this) == (VT_ARRAY | VT_I4))
	{
		SafeArrayDestroy(parray);
	}
	if (strA != NULL)
	{
		delete []strA;
		strA = NULL;
	}
	if (pvarCompare != NULL)
	{
		delete pvarCompare;
		pvarCompare = NULL;
	}
}

_variant_t &IUI_VARIANT::GetVARIANT() throw()
{
	return *(_variant_t *) this;
}

_variant_t *IUI_VARIANT::GetAddress()
{
	Clear();
	return (_variant_t *) this;
}

void IUI_VARIANT::SetString(const char *pSrc)
{
	operator=(pSrc);
}

int IUI_VARIANT::SetPointX(LONG x)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 2);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	pData[0] = x;
	SafeArrayUnaccessData(parray);

	return 0;
}

int IUI_VARIANT::SetPointY(LONG y)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 2);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	pData[1] = y;
	SafeArrayUnaccessData(parray);

	return 0;
}

int IUI_VARIANT::SetRectLeftNoSize(LONG lLeft)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 4);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	LONG lWidth = pData[2] - pData[0];
	pData[0] = lLeft;
	pData[2] = pData[0] + lWidth;
	SafeArrayUnaccessData(parray);

	return 0;
}

int IUI_VARIANT::SetRectTopNoSize(LONG lTop)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 4);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	LONG lHeight = pData[3] - pData[1];
	pData[1] = lTop;
	pData[3] = pData[1] + lHeight;
	SafeArrayUnaccessData(parray);

	return 0;
}

int IUI_VARIANT::SetRectLeft(LONG lLeft)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 4);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	pData[0] = lLeft;
	SafeArrayUnaccessData(parray);

	return 0;
}

int IUI_VARIANT::SetRectTop(LONG lTop)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 4);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	pData[1] = lTop;
	SafeArrayUnaccessData(parray);

	return 0;
}

int IUI_VARIANT::SetRectRight(LONG lRight)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 4);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	pData[2] = lRight;
	SafeArrayUnaccessData(parray);

	return 0;
}

int IUI_VARIANT::SetRectBottom(LONG lBottom)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 4);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	pData[3] = lBottom;
	SafeArrayUnaccessData(parray);

	return 0;
}

int IUI_VARIANT::SetRectWidth(LONG lWidth)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 4);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	pData[2] = pData[0] + lWidth;
	SafeArrayUnaccessData(parray);

	return 0;
}

int IUI_VARIANT::SetRectHeight(LONG lHeight)
{
	if (V_VT(this) != (VT_ARRAY | VT_I4))
	{
		// Clear the VARIANT and create a VT_RECT
		//
		IUI_VARIANT_Clear();
		V_VT(this) = VT_ARRAY | VT_I4;
		parray = SafeArrayCreateVector(VT_I4, 0, 4);
	}

	LONG *pData = NULL;
	SafeArrayAccessData(parray, (void **)&pData);
	pData[3] = pData[1] + lHeight;
	SafeArrayUnaccessData(parray);

	return 0;
}

_bstr_t IUI_VARIANT::GetEnumString() const
{
	if (V_VT(this) == VT_INT)
	{
		_bstr_t str;
		if (vtEx == VT_EX_IMAGE_RESIZE_MODE)
		{
			switch (V_I4(this))
			{
			case IRM_9GRID:
				str = _T("9Grid");
				break;
			case IRM_STRETCH:
				str = _T("Stretch");
				break;
			case IRM_STRETCH_HIGH_QUALITY:
				str = _T("Stretch High Quality");
				break;
			case IRM_TILE:
				str = _T("Tile");
				break;
			case IRM_CENTER:
				str = _T("Center");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_LAYOUT_HOR)
		{
			switch (V_I4(this))
			{
			case CLH_DEFAULT:
				str = _T("Default");
				break;
			case CLH_LEFT:
				str = _T("Left");
				break;
			case CLH_RIGHT:
				str = _T("Right");
				break;
			case CLH_CENTER:
				str = _T("Center");
				break;
			case CLH_FILL:
				str = _T("Fill");
				break;
			case CLH_FULL_FILL:
				str = _T("Full Fill");
				break;
			case CLH_FREE:
				str = _T("Free");
				break;
			case CLH_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO:
				str = _T("Keep Children Orign Aspect Ratio");
				break;
			case CLH_KEEP_CHILDREN_ASPECT_RATIO:
				str = _T("Keep Children Aspect Ratio");
				break;
			case CLH_RIGHT_EVEN_ZOOM_OUT:
				str = _T("Right Even Zoom Out");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_LAYOUT_VER)
		{
			switch (V_I4(this))
			{
			case CLV_DEFAULT:
				str = _T("Default");
				break;
			case CLV_TOP:
				str = _T("Top");
				break;
			case CLV_BOTTOM:
				str = _T("Bottom");
				break;
			case CLV_CENTER:
				str = _T("Center");
				break;
			case CLV_FILL:
				str = _T("Fill");
				break;
			case CLV_FULL_FILL:
				str = _T("Full Fill");
				break;
			case CLV_FREE:
				str = _T("Free");
				break;
			case CLV_BOTTOM_EVEN_ZOOM_OUT:
				str = _T("Bottom Even Zoom Out");
				break;
			case CLV_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO:
				str = _T("Keep Children Origin Aspect Ratio");
				break;
			case CLV_KEEP_CHILDREN_ASPECT_RATIO:
				str = _T("Keep Children Aspect Ratio");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_TEXT_ALIGN_HOR)
		{
			switch (V_I4(this))
			{
			case TAH_DEFAULT:
				str = _T("Default");
				break;
			case TAH_LEFT:
				str = _T("Left");
				break;
			case TAH_RIGHT:
				str = _T("Right");
				break;
			case TAH_CENTER:
				str = _T("Center");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_TEXT_ALIGN_VER)
		{
			switch (V_I4(this))
			{
			case TAV_DEFAULT:
				str = _T("Default");
				break;
			case TAV_TOP:
				str = _T("Top");
				break;
			case TAV_BOTTOM:
				str = _T("Bottom");
				break;
			case TAV_CENTER:
				str = _T("Center");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_FOREGROUND_ALIGN_HOR)
		{
			switch (V_I4(this))
			{
			case FAH_LEFT:
				str = _T("Left");
				break;
			case FAH_RIGHT:
				str = _T("Right");
				break;
			case FAH_CENTER:
				str = _T("Center");
				break;
			case FAH_STRETCH:
				str = _T("Stretch");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_FOREGROUND_ALIGN_VER)
		{
			switch (V_I4(this))
			{
			case FAV_TOP:
				str = _T("Top");
				break;
			case FAV_BOTTOM:
				str = _T("Bottom");
				break;
			case FAV_CENTER:
				str = _T("Center");
				break;
			case FAV_STRETCH:
				str = _T("Stretch");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_BUTTON_TYPE)
		{
			switch (V_I4(this))
			{
			case BT_NORMAL:
				str = _T("Normal");
				break;
			case BT_MINIMIZEBOX:
				str = _T("MinimizeBox");
				break;
			case BT_CLOSEBOX:
				str = _T("CloseBox");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_CHECKBOX_TYPE)
		{
			switch (V_I4(this))
			{
			case CT_NORMAL:
				str = _T("Normal");
				break;
			case CT_MAXIMIZEBOX:
				str = _T("MaximizeBox");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_WINDOW_TYPE)
		{
			switch (V_I4(this))
			{
			case WT_NORMAL:
				str = _T("Normal");
				break;
			case WT_MDIFRAME:
				str = _T("MDI Frame");
				break;
			case WT_MDICLIENT:
				str = _T("MDI Client");
				break;
			case WT_MDICHILDFRAME:
				str = _T("MDI Child Frame");
				break;
			case WT_MDIVIEW:
				str = _T("MDI View");
				break;
			case WT_FORMWND:
				str = _T("Form Window");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_COMBOBOX_STYLE)
		{
			switch (V_I4(this))
			{
			case CS_SIMPLE:
				str = _T("Simple");
				break;
			case CS_DROPDOWN:
				str = _T("DropDown");
				break;
			case CS_DROPDOWNLIST:
				str = _T("DropDown List");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_SPN_ALIGN)
		{
			switch (V_I4(this))
			{
			case SPNA_UNATTACHED:
				str = _T("Unattached");
				break;
			case SPNA_LEFT:
				str = _T("Left");
				break;
			case SPNA_RIGHT:
				str = _T("Right");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_SLD_EXT_STYLE)
		{
			switch (V_I4(this))
			{
			case SLDES_NORMAL:
				str = _T("Normal");
				break;
			case SLDES_PROGRESS:
				str = _T("Progress");
				break;
			case SLDES_SPLITPROGRESS:
				str = _T("Split Progress");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_LST_STYLE)
		{
			switch (V_I4(this))
			{
			case LSTS_ICON:
				str = _T("Icon");
				break;
			case LSTS_SMALLICON:
				str = _T("Small Icon");
				break;
			case LSTS_LIST:
				str = _T("List");
				break;
			case LSTS_REPORT:
				str = _T("Report");
				break;
			case LSTS_THUMBNAIL:
				str = _T("Thumbnail");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_LST_ALIGN)
		{
			switch (V_I4(this))
			{
			case LSTA_TOP:
				str = _T("Top");
				break;
			case LSTA_LEFT:
				str = _T("Left");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_LST_SORT)
		{
			switch (V_I4(this))
			{
			case LSTST_UNSET:
				str = _T("Unset");
				break;
			case LSTST_ASCENDING:
				str = _T("Ascending");
				break;
			case LSTST_DESCENDING:
				str = _T("Descending");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_DATETIME_FORMAT)
		{
			switch (V_I4(this))
			{
			case DTF_SHORT_DATE:
				str = _T("Short Date");
				break;
			case DTF_LONG_DATE:
				str = _T("Long Date");
				break;
			case DTF_TIME:
				str = _T("Time");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_BACKGROUND_TYPE)
		{
			switch (V_I4(this))
			{
			case BKT_COLOR:
				str = _T("Color");
				break;
			case BKT_IMAGE:
				str = _T("Image");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_PROGRESS_THUMB_MODE)
		{
			switch (V_I4(this))
			{
			case PTM_STRETCH:
				str = _T("Stretch");
				break;
			case PTM_TILE:
				str = _T("Tile");
				break;
			case PTM_CLIP:
				str = _T("Clip");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_PROGRESS_THUMB_POSITION)
		{
			switch (V_I4(this))
			{
			case PTP_DEFAULT:
				str = _T("Default");
				break;
			case PTP_SPECIFIED:
				str = _T("Specified");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_ALPHA_FORMAT)
		{
			switch (V_I4(this))
			{
			case AF_NONE:
				str = _T("None");
				break;
			case AF_AC_SRC_ALPHA:
				str = _T("AC_SRC_ALPHA");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_MENU_BORDER_STYLE)
		{
			switch (V_I4(this))
			{
			case MBS_NORMAL:
				str = _T("Normal");
				break;
			case MBS_FLAT:
				str = _T("Flat");
				break;
			case MBS_NONE:
				str = _T("None");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_SPLITTER_ORIENTATION)
		{
			switch (V_I4(this))
			{
			case SO_WE:
				str = _T("WestEast");
				break;
			case SO_NS:
				str = _T("NorthSouth");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_STACK_PANEL_ORIENTATION)
		{
			switch (V_I4(this))
			{
			case SPO_LEFT:
				str = _T("Left");
				break;
			case SPO_TOP:
				str = _T("Top");
				break;
			case SPO_RIGHT:
				str = _T("Right");
				break;
			case SPO_BOTTOM:
				str = _T("Bottom");
				break;
			default:
				break;
			}
			return str;
		}
		else if (vtEx == VT_EX_PEN_STYLE)
		{
			switch (V_I4(this))
			{
			case LPS_SOLID:
				str = _T("PS_SOLID");
				break;
			case LPS_DASH:
				str = _T("PS_DASH");
				break;
			case LPS_DOT:
				str = _T("PS_DOT");
				break;
			case LPS_DASHDOT:
				str = _T("PS_DASHDOT");
				break;
			case LPS_DASHDOTDOT:
				str = _T("PS_DASHDOTDOT");
				break;
			case LPS_NULL:
				str = _T("PS_NULL");
				break;
			case LPS_INSIDEFRAME:
				str = _T("PS_INSIDEFRAME");
				break;
			case LPS_USERSTYLE:
				str = _T("PS_USERSTYLE");
				break;
			case LPS_ALTERNATE:
				str = _T("PS_ALTERNATE");
				break;
			default:
				break;
			}
			return str;
		}
	}

	return _variant_t::operator _bstr_t();
}

int IUI_VARIANT::InitByEnumString(const CString &str)
{
	if (V_VT(this) != VT_INT)
	{
		return -1;
	}

	if (vtEx == VT_EX_IMAGE_RESIZE_MODE)
	{
		if (str.CompareNoCase(IUI_VARIANT(IRM_9GRID).GetEnumString()) == 0)
		{
			V_I4(this) = IRM_9GRID;
		}
		else if (str.CompareNoCase(IUI_VARIANT(IRM_STRETCH).GetEnumString()) == 0)
		{
			V_I4(this) = IRM_STRETCH;
		}
		else if (str.CompareNoCase(IUI_VARIANT(IRM_STRETCH_HIGH_QUALITY).GetEnumString()) == 0)
		{
			V_I4(this) = IRM_STRETCH_HIGH_QUALITY;
		}
		else if (str.CompareNoCase(IUI_VARIANT(IRM_TILE).GetEnumString()) == 0)
		{
			V_I4(this) = IRM_TILE;
		}
		else if (str.CompareNoCase(IUI_VARIANT(IRM_CENTER).GetEnumString()) == 0)
		{
			V_I4(this) = IRM_CENTER;
		}
	}
	else if (vtEx == VT_EX_LAYOUT_HOR)
	{
		if (str.CompareNoCase(IUI_VARIANT(CLH_DEFAULT).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_DEFAULT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLH_LEFT).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_LEFT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLH_RIGHT).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_RIGHT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLH_CENTER).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_CENTER;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLH_FILL).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_FILL;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLH_FULL_FILL).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_FULL_FILL;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLH_FREE).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_FREE;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLH_RIGHT_EVEN_ZOOM_OUT).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_RIGHT_EVEN_ZOOM_OUT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLH_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLH_KEEP_CHILDREN_ASPECT_RATIO).GetEnumString()) == 0)
		{
			V_I4(this) = CLH_KEEP_CHILDREN_ASPECT_RATIO;
		}
	}
	else if (vtEx == VT_EX_LAYOUT_VER)
	{
		if (str.CompareNoCase(IUI_VARIANT(CLV_DEFAULT).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_DEFAULT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLV_TOP).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_TOP;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLV_BOTTOM).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_BOTTOM;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLV_CENTER).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_CENTER;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLV_FILL).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_FILL;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLV_FULL_FILL).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_FULL_FILL;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLV_FREE).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_FREE;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLV_BOTTOM_EVEN_ZOOM_OUT).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_BOTTOM_EVEN_ZOOM_OUT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLV_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO;
		}
		else if (str.CompareNoCase(IUI_VARIANT(CLV_KEEP_CHILDREN_ASPECT_RATIO).GetEnumString()) == 0)
		{
			V_I4(this) = CLV_KEEP_CHILDREN_ASPECT_RATIO;
		}
	}
	else if (vtEx == VT_EX_TEXT_ALIGN_HOR)
	{
		if (str.CompareNoCase(_T("Default")) == 0)
		{
			V_I4(this) = TAH_DEFAULT;
		}
		else if (str.CompareNoCase(_T("Left")) == 0)
		{
			V_I4(this) = TAH_LEFT;
		}
		else if (str.CompareNoCase(_T("Right")) == 0)
		{
			V_I4(this) = TAH_RIGHT;
		}
		else if (str.CompareNoCase(_T("Center")) == 0)
		{
			V_I4(this) = TAH_CENTER;
		}
	}
	else if (vtEx == VT_EX_TEXT_ALIGN_VER)
	{
		if (str.CompareNoCase(_T("Default")) == 0)
		{
			V_I4(this) = TAV_DEFAULT;
		}
		else if (str.CompareNoCase(_T("Top")) == 0)
		{
			V_I4(this) = TAV_TOP;
		}
		else if (str.CompareNoCase(_T("Bottom")) == 0)
		{
			V_I4(this) = TAV_BOTTOM;
		}
		else if (str.CompareNoCase(_T("Center")) == 0)
		{
			V_I4(this) = TAV_CENTER;
		}
	}
	else if (vtEx == VT_EX_FOREGROUND_ALIGN_HOR)
	{
		if (str.CompareNoCase(_T("Left")) == 0)
		{
			V_I4(this) = FAH_LEFT;
		}
		else if (str.CompareNoCase(_T("Right")) == 0)
		{
			V_I4(this) = FAH_RIGHT;
		}
		else if (str.CompareNoCase(_T("Center")) == 0)
		{
			V_I4(this) = FAH_CENTER;
		}
		else if (str.CompareNoCase(_T("Stretch")) == 0)
		{
			V_I4(this) = FAH_STRETCH;
		}
	}
	else if (vtEx == VT_EX_FOREGROUND_ALIGN_VER)
	{
		if (str.CompareNoCase(_T("Top")) == 0)
		{
			V_I4(this) = FAV_TOP;
		}
		else if (str.CompareNoCase(_T("Bottom")) == 0)
		{
			V_I4(this) = FAV_BOTTOM;
		}
		else if (str.CompareNoCase(_T("Center")) == 0)
		{
			V_I4(this) = FAV_CENTER;
		}
		else if (str.CompareNoCase(_T("Stretch")) == 0)
		{
			V_I4(this) = FAV_STRETCH;
		}
	}
	else if (vtEx == VT_EX_BUTTON_TYPE)
	{
		if (str.CompareNoCase(_T("Normal")) == 0)
		{
			V_I4(this) = BT_NORMAL;
		}
		else if (str.CompareNoCase(_T("MinimizeBox")) == 0)
		{
			V_I4(this) = BT_MINIMIZEBOX;
		}
		else if (str.CompareNoCase(_T("CloseBox")) == 0)
		{
			V_I4(this) = BT_CLOSEBOX;
		}
	}
	else if (vtEx == VT_EX_CHECKBOX_TYPE)
	{
		if (str.CompareNoCase(_T("Normal")) == 0)
		{
			V_I4(this) = CT_NORMAL;
		}
		else if (str.CompareNoCase(_T("MaximizeBox")) == 0)
		{
			V_I4(this) = CT_MAXIMIZEBOX;
		}
	}
	else if (vtEx == VT_EX_WINDOW_TYPE)
	{
		if (str.CompareNoCase(IUI_VARIANT(WT_NORMAL).GetEnumString()) == 0)
		{
			V_I4(this) = WT_NORMAL;
		}
		else if (str.CompareNoCase(IUI_VARIANT(WT_MDIFRAME).GetEnumString()) == 0)
		{
			V_I4(this) = WT_MDIFRAME;
		}
		else if (str.CompareNoCase(IUI_VARIANT(WT_MDICLIENT).GetEnumString()) == 0)
		{
			V_I4(this) = WT_MDICLIENT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(WT_MDICHILDFRAME).GetEnumString()) == 0)
		{
			V_I4(this) = WT_MDICHILDFRAME;
		}
		else if (str.CompareNoCase(IUI_VARIANT(WT_MDIVIEW).GetEnumString()) == 0)
		{
			V_I4(this) = WT_MDIVIEW;
		}
		else if (str.CompareNoCase(IUI_VARIANT(WT_FORMWND).GetEnumString()) == 0)
		{
			V_I4(this) = WT_FORMWND;
		}
	}
	else if (vtEx == VT_EX_COMBOBOX_STYLE)
	{
		if (str.CompareNoCase(_T("Simple")) == 0)
		{
			V_I4(this) = CS_SIMPLE;
		}
		else if (str.CompareNoCase(_T("DropDown")) == 0)
		{
			V_I4(this) = CS_DROPDOWN;
		}
		else if (str.CompareNoCase(_T("DropDown List")) == 0)
		{
			V_I4(this) = CS_DROPDOWNLIST;
		}
	}
	else if (vtEx == VT_EX_SPN_ALIGN)
	{
		if (str.CompareNoCase(_T("Unattached")) == 0)
		{
			V_I4(this) = SPNA_UNATTACHED;
		}
		else if (str.CompareNoCase(_T("Left")) == 0)
		{
			V_I4(this) = SPNA_LEFT;
		}
		else if (str.CompareNoCase(_T("Right")) == 0)
		{
			V_I4(this) = SPNA_RIGHT;
		}
	}
	else if (vtEx == VT_EX_SLD_EXT_STYLE)
	{
		if (str.CompareNoCase(_T("Normal")) == 0)
		{
			V_I4(this) = SLDES_NORMAL;
		}
		else if (str.CompareNoCase(_T("Progress")) == 0)
		{
			V_I4(this) = SLDES_PROGRESS;
		}
		else if (str.CompareNoCase(_T("Split Progress")) == 0)
		{
			V_I4(this) = SLDES_SPLITPROGRESS;
		}
	}
	else if (vtEx == VT_EX_LST_STYLE)
	{
		if (str.CompareNoCase(_T("Icon")) == 0)
		{
			V_I4(this) = LSTS_ICON;
		}
		else if (str.CompareNoCase(_T("Small Icon")) == 0)
		{
			V_I4(this) = LSTS_SMALLICON;
		}
		else if (str.CompareNoCase(_T("List")) == 0)
		{
			V_I4(this) = LSTS_LIST;
		}
		else if (str.CompareNoCase(_T("Report")) == 0)
		{
			V_I4(this) = LSTS_REPORT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(LSTS_THUMBNAIL).GetEnumString()) == 0)
		{
			V_I4(this) = LSTS_THUMBNAIL;
		}
	}
	else if (vtEx == VT_EX_LST_ALIGN)
	{
		if (str.CompareNoCase(_T("Top")) == 0)
		{
			V_I4(this) = LSTA_TOP;
		}
		else if (str.CompareNoCase(_T("Left")) == 0)
		{
			V_I4(this) = LSTA_LEFT;
		}
	}
	else if (vtEx == VT_EX_LST_SORT)
	{
		if (str.CompareNoCase(_T("Unset")) == 0)
		{
			V_I4(this) = LSTST_UNSET;
		}
		else if (str.CompareNoCase(_T("Ascending")) == 0)
		{
			V_I4(this) = LSTST_ASCENDING;
		}
		else if (str.CompareNoCase(_T("Descending")) == 0)
		{
			V_I4(this) = LSTST_DESCENDING;
		}
	}
	else if (vtEx == VT_EX_DATETIME_FORMAT)
	{
		if (str.CompareNoCase(IUI_VARIANT(DTF_SHORT_DATE).GetEnumString()) == 0)
		{
			V_I4(this) = DTF_SHORT_DATE;
		}
		else if (str.CompareNoCase(IUI_VARIANT(DTF_LONG_DATE).GetEnumString()) == 0)
		{
			V_I4(this) = DTF_LONG_DATE;
		}
		else if (str.CompareNoCase(IUI_VARIANT(DTF_TIME).GetEnumString()) == 0)
		{
			V_I4(this) = DTF_TIME;
		}
	}
	else if (vtEx == VT_EX_BACKGROUND_TYPE)
	{
		if (str.CompareNoCase(IUI_VARIANT(BKT_COLOR).GetEnumString()) == 0)
		{
			V_I4(this) = BKT_COLOR;
		}
		else if (str.CompareNoCase(IUI_VARIANT(BKT_IMAGE).GetEnumString()) == 0)
		{
			V_I4(this) = BKT_IMAGE;
		}
	}
	else if (vtEx == VT_EX_PROGRESS_THUMB_MODE)
	{
		if (str.CompareNoCase(IUI_VARIANT(PTM_STRETCH).GetEnumString()) == 0)
		{
			V_I4(this) = PTM_STRETCH;
		}
		else if (str.CompareNoCase(IUI_VARIANT(PTM_TILE).GetEnumString()) == 0)
		{
			V_I4(this) = PTM_TILE;
		}
		else if (str.CompareNoCase(IUI_VARIANT(PTM_CLIP).GetEnumString()) == 0)
		{
			V_I4(this) = PTM_CLIP;
		}
	}
	else if (vtEx == VT_EX_PROGRESS_THUMB_POSITION)
	{
		if (str.CompareNoCase(IUI_VARIANT(PTP_DEFAULT).GetEnumString()) == 0)
		{
			V_I4(this) = PTP_DEFAULT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(PTP_SPECIFIED).GetEnumString()) == 0)
		{
			V_I4(this) = PTP_SPECIFIED;
		}
	}
	else if (vtEx == VT_EX_ALPHA_FORMAT)
	{
		if (str.CompareNoCase(IUI_VARIANT(AF_NONE).GetEnumString()) == 0)
		{
			V_I4(this) = AF_NONE;
		}
		else if (str.CompareNoCase(IUI_VARIANT(AF_AC_SRC_ALPHA).GetEnumString()) == 0)
		{
			V_I4(this) = AF_AC_SRC_ALPHA;
		}
	}
	else if (vtEx == VT_EX_MENU_BORDER_STYLE)
	{
		if (str.CompareNoCase(IUI_VARIANT(MBS_NORMAL).GetEnumString()) == 0)
		{
			V_I4(this) = MBS_NORMAL;
		}
		else if (str.CompareNoCase(IUI_VARIANT(MBS_FLAT).GetEnumString()) == 0)
		{
			V_I4(this) = MBS_FLAT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(MBS_NONE).GetEnumString()) == 0)
		{
			V_I4(this) = MBS_NONE;
		}
	}
	else if (vtEx == VT_EX_SPLITTER_ORIENTATION)
	{
		if (str.CompareNoCase(IUI_VARIANT(SO_WE).GetEnumString()) == 0)
		{
			V_I4(this) = SO_WE;
		}
		else if (str.CompareNoCase(IUI_VARIANT(SO_NS).GetEnumString()) == 0)
		{
			V_I4(this) = SO_NS;
		}
	}
	else if (vtEx == VT_EX_STACK_PANEL_ORIENTATION)
	{
		if (str.CompareNoCase(IUI_VARIANT(SPO_LEFT).GetEnumString()) == 0)
		{
			V_I4(this) = SPO_LEFT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(SPO_TOP).GetEnumString()) == 0)
		{
			V_I4(this) = SPO_TOP;
		}
		else if (str.CompareNoCase(IUI_VARIANT(SPO_RIGHT).GetEnumString()) == 0)
		{
			V_I4(this) = SPO_RIGHT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(SPO_BOTTOM).GetEnumString()) == 0)
		{
			V_I4(this) = SPO_BOTTOM;
		}
	}
	else if (vtEx == VT_EX_PEN_STYLE)
	{
		if (str.CompareNoCase(IUI_VARIANT(LPS_SOLID).GetEnumString()) == 0)
		{
			V_I4(this) = LPS_SOLID;
		}
		else if (str.CompareNoCase(IUI_VARIANT(LPS_DASH).GetEnumString()) == 0)
		{
			V_I4(this) = LPS_DASH;
		}
		else if (str.CompareNoCase(IUI_VARIANT(LPS_DOT).GetEnumString()) == 0)
		{
			V_I4(this) = LPS_DOT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(LPS_DASHDOT).GetEnumString()) == 0)
		{
			V_I4(this) = LPS_DASHDOT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(LPS_DASHDOTDOT).GetEnumString()) == 0)
		{
			V_I4(this) = LPS_DASHDOTDOT;
		}
		else if (str.CompareNoCase(IUI_VARIANT(LPS_NULL).GetEnumString()) == 0)
		{
			V_I4(this) = LPS_NULL;
		}
		else if (str.CompareNoCase(IUI_VARIANT(LPS_INSIDEFRAME).GetEnumString()) == 0)
		{
			V_I4(this) = LPS_INSIDEFRAME;
		}
		else if (str.CompareNoCase(IUI_VARIANT(LPS_USERSTYLE).GetEnumString()) == 0)
		{
			V_I4(this) = LPS_USERSTYLE;
		}
		else if (str.CompareNoCase(IUI_VARIANT(LPS_ALTERNATE).GetEnumString()) == 0)
		{
			V_I4(this) = LPS_ALTERNATE;
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return 0;
}

int IUI_VARIANT::SetBindVariant(const IUI_VARIANT *pBind)
{
	ASSERT(pBind != NULL);
	if (pvarBind == pBind)
	{
		return 0;
	}

	if (pvarBind != NULL)
	{
		pvarBind->lBeBindCount--;
	}

	pvarBind = (IUI_VARIANT *)pBind;
	pvarBind->lBeBindCount++;

	return 0;
}

int IUI_VARIANT::RemoveBindVariant()
{
	if (pvarBind != NULL)
	{
		pvarBind->lBeBindCount--;
	}

	return 0;
}

const IUI_VARIANT *IUI_VARIANT::GetBindVariant() const
{
	return pvarBind;
}

int IUI_VARIANT::SetBindCompareValue(const IUI_VARIANT &var)
{
	if (pvarCompare != NULL)
	{
		delete pvarCompare;
		pvarCompare = NULL;
	}

	pvarCompare = new IUI_VARIANT;
	*pvarCompare = var;

	return 0;
}

const IUI_VARIANT IUI_VARIANT::GetBindCompareValue() const
{
	IUI_VARIANT var;
	if (pvarCompare == NULL)
	{
		return var;
	}

	var = *pvarCompare;

	return var;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Destructor

IUI_VARIANT::~IUI_VARIANT() throw()
{
	_variant_t::~_variant_t();

	IUI_VARIANT_Clear();
}

//////////////////////////////////////////////////////////////////////////
//

//////////////////////////////////////////////////////////////////////////
// CGridItem

CGridItem::CGridItem()
{
	m_uMask = 0;
	m_pBuffer = NULL;
	m_lParam = 0;
}

CGridItem::~CGridItem()
{
}

void CGridItem::SetText(const CString &strText)
{
	m_strText = strText;
}

CString CGridItem::GetText() const
{
	return m_strText;
}

void CGridItem::SetPointer(void *pBuffer)
{
	m_pBuffer = pBuffer;
}

void *CGridItem::GetPointer() const
{
	return m_pBuffer;
}

void CGridItem::SetParam(LPARAM lParam)
{
	m_lParam = lParam;
}

LPARAM CGridItem::GetParam() const
{
	return m_lParam;
}

//////////////////////////////////////////////////////////////////////////
// CGrid

CGrid::CGrid()
{

}

CGrid::~CGrid()
{

}

int CGrid::InsertColumn(int nCol, const CString &strColumnHeading/* = _T("")*/)
{
	if (nCol < 0)
	{
		return -1;
	}

	int nCount = (int)m_vHeader.size();
	if (nCol > nCount)
	{
		nCol = nCount;
	}

	// Insert header
	CGridItem item;
	item.SetText(strColumnHeading);
	m_vHeader.insert(m_vHeader.begin() + nCol, item);

	// Insert items of this column
	int nItems = (int)m_vvData.size();
	for (int i = 0; i < nItems; ++i)
	{
		std::vector<CGridItem> *pItem = &m_vvData[i];

		CGridItem iSubItem;
		pItem->insert(pItem->begin() + nCol, iSubItem);
	}

	return nCol;
}

BOOL CGrid::DeleteColumn(int nCol)
{
	if (!IsColumnValid(nCol))
	{
		return FALSE;
	}

	// remove header
	m_vHeader.erase(m_vHeader.begin() + nCol);

	// remove items of column
	size_t nItems = m_vvData.size();
	for (size_t i = 0; i < nItems; ++i)
	{
		std::vector<CGridItem> *pItem = &m_vvData[i];

		pItem->erase(pItem->begin() + nCol);
	}

	// If not column exist, clear the items
	size_t nCount = m_vHeader.size();
	if (nCount == 0)
	{
		m_vvData.clear();
	}

	return TRUE;
}

int CGrid::GetColumnCount() const
{
	int nCount = (int)m_vHeader.size();

	return nCount;
}

BOOL CGrid::SetColumn(int nCol, const CGridItem *pColumn)
{
	if (!IsColumnValid(nCol))
	{
		return FALSE;
	}

	CGridItem *pc = &m_vHeader[nCol];

	if ((pColumn->m_uMask & GIF_TEXT) == GIF_TEXT)
	{
		pc->SetText(pColumn->GetText());
	}
	if ((pColumn->m_uMask & GIF_POINTER) == GIF_POINTER)
	{
		pc->SetPointer(pColumn->GetPointer());
	}
	if ((pColumn->m_uMask & GIF_PARAM) == GIF_PARAM)
	{
		pc->SetParam(pColumn->GetParam());
	}

	return TRUE;
}

BOOL CGrid::GetColumn(int nCol, CGridItem *pColumn) const
{
	if (!IsColumnValid(nCol))
	{
		return FALSE;
	}

	const CGridItem *pc = &m_vHeader[nCol];

	if ((pColumn->m_uMask & GIF_TEXT) == GIF_TEXT)
	{
		pColumn->SetText(pc->GetText());
	}
	if ((pColumn->m_uMask & GIF_POINTER) == GIF_POINTER)
	{
		pColumn->SetPointer(pc->GetPointer());
	}
	if ((pColumn->m_uMask & GIF_PARAM) == GIF_PARAM)
	{
		pColumn->SetParam(pc->GetParam());
	}

	return TRUE;
}

BOOL CGrid::SetColumnText(int nCol, const CString &strColumn)
{
	CGridItem item;
	item.m_uMask = GIF_TEXT;
	item.SetText(strColumn);

	return SetColumn(nCol, &item);
}

BOOL CGrid::GetColumnText(int nCol, CString *pstrColumn) const
{
	CGridItem item;
	item.m_uMask = GIF_TEXT;
	BOOL bRet = GetColumn(nCol, &item);
	if (!bRet)
	{
		return FALSE;
	}

	*pstrColumn = item.GetText();
	return TRUE;
}

BOOL CGrid::SetColumnPointer(int nCol, void *pBuffer)
{
	CGridItem item;
	item.m_uMask = GIF_POINTER;
	item.SetPointer(pBuffer);

	return SetColumn(nCol, &item);
}

void *CGrid::GetColumnPointer(int nCol) const
{
	CGridItem item;
	item.m_uMask = GIF_POINTER;

	BOOL bRet = GetColumn(nCol, &item);
	if (!bRet)
	{
		return NULL;
	}

	return item.GetPointer();
}

BOOL CGrid::SetColumnData(int nCol, LPARAM lParam)
{
	CGridItem item;
	item.m_uMask = GIF_PARAM;
	item.SetParam(lParam);

	return SetColumn(nCol, &item);
}

LPARAM CGrid::GetColumnData(int nCol) const
{
	CGridItem item;
	item.m_uMask = GIF_PARAM;
	BOOL bRet = GetColumn(nCol, &item);
	if (!bRet)
	{
		return 0;
	}

	return item.GetParam();
}

BOOL CGrid::SwapColumn(int nFirst, int nSecond)
{
	if (nFirst == nSecond)
	{
		return TRUE;
	}

	if (!IsColumnValid(nFirst))
	{
		return FALSE;
	}

	if (!IsColumnValid(nSecond))
	{
		return FALSE;
	}

	// swap header
	std::swap(*(m_vHeader.begin() + nFirst), *(m_vHeader.begin() + nSecond));

	// swap items
	int nItems = (int)m_vvData.size();
	for (int i = 0; i < nItems; ++i)
	{
		std::vector<CGridItem> *pItem = &m_vvData[i];

		std::swap(*(pItem->begin() + nFirst), *(pItem->begin() + nSecond));
	}

	return TRUE;
}

int CGrid::InsertItem(int nItem, LPCTSTR lpszItem)
{
	if (nItem < 0 || lpszItem == NULL)
	{
		return -1;
	}

	int nColumns = (int)m_vHeader.size();
	if (nColumns <= 0)
	{
		return -1;
	}

	int nItems = (int)m_vvData.size();
	if (nItem >= nItems)
	{
		nItem = nItems;
	}

	std::vector<CGridItem> vItem;
	vItem.resize(nColumns);

	vItem[0].SetText(lpszItem);

	m_vvData.insert(m_vvData.begin() + nItem, vItem);

	return nItem;
}

BOOL CGrid::DeleteItem(int nItem)
{
	if (nItem < 0)
	{
		return FALSE;
	}

	int nItems = (int)m_vvData.size();
	if (nItem >= nItems)
	{
		return FALSE;
	}

	m_vvData.erase(m_vvData.begin() + nItem);

	return TRUE;
}

BOOL CGrid::DeleteAllItems()
{
	m_vvData.clear();

	return TRUE;
}

int CGrid::GetItemCount() const
{
	int nCount = (int)m_vvData.size();

	return nCount;
}

BOOL CGrid::SetItem(int nItem, int nSubItem, const CGridItem *pItem)
{
	if (!IsItemValid(nItem, nSubItem))
	{
		return FALSE;
	}

	if (pItem == NULL)
	{
		return FALSE;
	}

	std::vector<CGridItem> *pvItem = &m_vvData[nItem];
	CGridItem *pi = &(*pvItem)[nSubItem];

	*pi = *pItem;

	return TRUE;
}

BOOL CGrid::GetItem(int nItem, int nSubItem, CGridItem *pItem) const
{
	if (!IsItemValid(nItem, nSubItem))
	{
		return FALSE;
	}

	if (pItem == NULL)
	{
		return FALSE;
	}

	const std::vector<CGridItem> *pvItem = &m_vvData[nItem];
	const CGridItem *pi = &(*pvItem)[nSubItem];

	*pItem = *pi;

	return TRUE;
}

BOOL CGrid::SetItemText(int nItem, int nSubItem, const CString &strText)
{
	if (!IsItemValid(nItem, nSubItem))
	{
		return FALSE;
	}

	std::vector<CGridItem> *pvItem = &m_vvData[nItem];
	CGridItem *pItem = &(*pvItem)[nSubItem];

	pItem->SetText(strText);

	return TRUE;
}

CString CGrid::GetItemText(int nItem, int nSubItem) const
{
	CString strText;

	if (!IsItemValid(nItem, nSubItem))
	{
		return strText;
	}

	const std::vector<CGridItem> *pvItem = &m_vvData[nItem];
	const CGridItem *pItem = &(*pvItem)[nSubItem];

	strText = pItem->GetText();

	return strText;
}

BOOL CGrid::SetItemPointer(int nItem, int nSubItem, void *pBuffer)
{
	if (!IsItemValid(nItem, nSubItem))
	{
		return FALSE;
	}

	std::vector<CGridItem> *pvItem = &m_vvData[nItem];
	CGridItem *pItem = &(*pvItem)[nSubItem];

	pItem->SetPointer(pBuffer);

	return TRUE;
}

void *CGrid::GetItemPointer(int nItem, int nSubItem) const
{
	void *pBuffer = NULL;

	if (!IsItemValid(nItem, nSubItem))
	{
		return pBuffer;
	}

	const std::vector<CGridItem> *pvItem = &m_vvData[nItem];
	const CGridItem *pItem = &(*pvItem)[nSubItem];

	pBuffer = pItem->GetPointer();

	return pBuffer;
}

BOOL CGrid::SetItemData(int nItem, int nSubItem, LPARAM lParam)
{
	if (!IsItemValid(nItem, nSubItem))
	{
		return FALSE;
	}

	std::vector<CGridItem> *pvItem = &m_vvData[nItem];
	CGridItem *pItem = &(*pvItem)[nSubItem];

	pItem->SetParam(lParam);

	return TRUE;
}

LPARAM CGrid::GetItemData(int nItem, int nSubItem) const
{
	if (!IsItemValid(nItem, nSubItem))
	{
		return -1;
	}

	const std::vector<CGridItem> *pvItem = &m_vvData[nItem];
	const CGridItem *pItem = &(*pvItem)[nSubItem];

	LPARAM lParam = pItem->GetParam();

	return lParam;
}

BOOL CGrid::SortItems(PFNLVCOMPARE pfnCompare, DWORD dwData)
{
	return FALSE;
}

int CGrid::FindItem(LV_FINDINFO *pFindInfo, int nStart/* = -1*/) const
{
	int nItems = (int)m_vvData.size();
	if (nItems <= 0)
	{
		return -1;
	}

	if (nStart >= nItems)
	{
		return -1;
	}

	BOOL bFindText = FALSE;
	BOOL bFindParam = FALSE;
	if ((pFindInfo->flags & LVFI_PARAM) == LVFI_PARAM)
	{
		bFindParam = TRUE;
	}
	if ((pFindInfo->flags & LVFI_STRING) == LVFI_STRING)
	{
		bFindText = TRUE;
	}

	if (!bFindText && !bFindParam)
	{
		return 0;
	}

	BOOL bFind = FALSE;
	BOOL bTextFound = FALSE;

	int i = 0;
	for (i = 0; i < nItems; ++i)
	{
		const std::vector<CGridItem> *pvItem = &m_vvData[i];
		const CGridItem *pItem = &(*pvItem)[0];	// if has item, column mustn't be 0.

		if (bFindText && bFindParam)
		{
			if (pItem->GetText() == pFindInfo->psz && pItem->GetParam() == pFindInfo->lParam)
			{
				bFind = TRUE;
				break;
			}
		}
		else if (bFindText)
		{
			if (pItem->GetText() == pFindInfo->psz)
			{
				bFind = TRUE;
				break;
			}
		}
		else if (bFindParam)
		{
			if (pItem->GetParam() == pFindInfo->lParam)
			{
				bFind = TRUE;
				break;
			}
		}
	}

	if (bFind)
	{
		return i;
	}

	return -1;
}

BOOL CGrid::SwapItem(int nFirst, int nSecond)
{
	int nCount = (int)m_vvData.size();
	if (nFirst < 0 || nFirst >= nCount || nSecond < 0 || nSecond >= nCount)
	{
		return FALSE;
	}

	if (nFirst == nSecond)
	{
		return TRUE;
	}

	// swap item
	std::swap(*(m_vvData.begin() + nFirst), *(m_vvData.begin() + nSecond));

	return TRUE;
}

BOOL CGrid::IsColumnValid(int nCol) const
{
	int nCount = (int)m_vHeader.size();

	if (nCol < 0 || nCol >= nCount)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CGrid::IsItemValid(int nItem, int nSubItem) const
{
	if (nItem < 0 || nSubItem < 0)
	{
		return FALSE;
	}

	int nColumns = (int)m_vHeader.size();
	if (nSubItem >= nColumns)
	{
		return FALSE;
	}

	int nItems = (int)m_vvData.size();
	if (nItem >= nItems)
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

IUI_LOGFONT::IUI_LOGFONT()
{
	memset(this, 0, sizeof(IUI_LOGFONT));
}

IUI_LOGFONT::IUI_LOGFONT(const LOGFONT &lf)
{
	if (&lf == this)
	{
		return;
	}

	memcpy(this, &lf, sizeof(LOGFONT));
}

IUI_LOGFONT::IUI_LOGFONT(const IUI_LOGFONT &lf)
{
	if (&lf == this)
	{
		return;
	}

	memcpy(this, &lf, sizeof(IUI_LOGFONT));
}

IUI_LOGFONT::~IUI_LOGFONT()
{

}

IUI_LOGFONT &IUI_LOGFONT::operator=(const LOGFONT &lf)
{
	if (&lf == this)
	{
		return *this;
	}

	memcpy(this, &lf, sizeof(LOGFONT));

	return *this;
}

IUI_LOGFONT &IUI_LOGFONT::operator=(const IUI_LOGFONT &lf)
{
	if (&lf == this)
	{
		return *this;
	}

	memcpy(this, &lf, sizeof(IUI_LOGFONT));

	return *this;
}

//////////////////////////////////////////////////////////////////////////

HwndProcMap CWndProcList::m_mapHwndProc;

CWndProcList::CWndProcList()
{
}

CWndProcList::~CWndProcList()
{
}

int CWndProcList::AddToMap(HWND hWnd, WNDPROC procOld, WNDPROC procNew)
{
	if (hWnd == NULL || procOld == NULL || procNew == NULL)
	{
		return -1;
	}

	// If the hWnd not has a window proc list, set a list for it.
	HwndProcMap::iterator itFind = m_mapHwndProc.find(hWnd);
	if (itFind == m_mapHwndProc.end())
	{
		WndProcList mapWndProc;
		m_mapHwndProc[hWnd] = mapWndProc;
	}

	itFind = m_mapHwndProc.find(hWnd);
	if (itFind == m_mapHwndProc.end())
	{
		ASSERT(FALSE);
		return -2;
	}

	// If the procNew is already exist, return.
	WNDPROC wndProc = NULL;
	WndProcList::iterator itProc = itFind->second.find(procNew);
	if (itProc != itFind->second.end())
	{
		return -3;
	}

	itFind->second[procNew] = procOld;

	return 0;
}

WNDPROC CWndProcList::GetOldWndProc(HWND hWnd, WNDPROC wndprocCur)
{
	if (hWnd == NULL || wndprocCur == NULL)
	{
		return NULL;
	}

	HwndProcMap::iterator itFind = m_mapHwndProc.find(hWnd);
	if (itFind == m_mapHwndProc.end())
	{
		return NULL;
	}

	WNDPROC wndProc = NULL;
	WndProcList::iterator it = itFind->second.find(wndprocCur);
	if (it == itFind->second.end())
	{
		return NULL;
	}

	return it->second;
}

SplitterStruct::SplitterStruct()
{
	m_rcNew = m_rcOld = CRect(0, 0, 0, 0);
	m_eDragOrientation = SDO_UNKNOWN;
}

SplitterStruct::~SplitterStruct()
{

}
