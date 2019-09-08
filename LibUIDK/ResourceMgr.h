#pragma once


namespace LibUIDK
{
	//////////////////////////////////////////////////////////////////////////
	// Image

	class CImageMgr
	{
	public:
		CImageMgr() {}
		~CImageMgr();

	public:
		std::map<CString, HIUIIMAGE> m_mapImages;
	};

	int GetImageFileName(HIUIIMAGE hIUIImage, CString *pstrFileName);
	HIUIIMAGE CreateEmptyIUIImage(LPCTSTR lpszImageFile);


	//////////////////////////////////////////////////////////////////////////
	// Font
	class CIUIFont
	{
	public:
		CIUIFont()
		{
			m_nAddRef = 0;
			m_hFont = NULL;
		}

		CIUIFont(HFONT hFont)
		{
			m_nAddRef = 0;
			m_hFont = hFont;
		}

		~CIUIFont()
		{
			if (m_nAddRef == 0)
			{
				DeleteObject(m_hFont);
			}
		}

	public:
		operator HFONT() const
		{
			return m_hFont;
		}

		int AddRef()
		{
			m_nAddRef++;

			return m_nAddRef;
		}

		int Release()
		{
			m_nAddRef--;

			if (m_nAddRef <= 0)
			{
				DeleteObject(m_hFont);
				m_hFont = NULL;
			}

			return m_nAddRef;
		}

		int Load(LPCTSTR lpszFontResID)
		{
			// Default font
			CString strCurrentFont = CUIMgr::GetDefaultFont();

			RESFONTPROPERTIES *pResDefaultFont = CUIMgr::GetDefaultFontResource(strCurrentFont);
			LOGFONT lfDefault = pResDefaultFont->m_logFont;
			GetDPILogFont(&lfDefault);

			FONTRESOURCEITEM *pResFont = CUIMgr::GetFontResourceItem(lpszFontResID);

			if (pResFont != NULL)
			{
				HFONT hFont = CreateControlFont(pResFont, &lfDefault, strCurrentFont);

				if (hFont != NULL)
				{
					m_hFont = hFont;
					m_strFontResID = lpszFontResID;

					return 0;
				}
			}
			else
			{
				m_hFont = CreateFontIndirect(&lfDefault);
				m_strFontResID = strCurrentFont;
			}

			return 0;
		}

		int SafeLoadSavedFont()
		{
			if (this == NULL)
			{
				return -1;
			}

			if (m_hFont != NULL)
			{
				return 1;
			}

			int nRet = Load(m_strFontResID);
			if (nRet != 0)
			{
				return nRet;
			}

			return 0;
		}

		HFONT GetSafeHFont()
		{
			if (this == NULL)
			{
				return NULL;
			}

			return m_hFont;
		}

	protected:
		int m_nAddRef;
		HFONT m_hFont;

	public:
		CString m_strFontResID;
	};

	typedef CIUIFont *HIUIFONT;

	class CFontMgr
	{
	public:
		CFontMgr() {}
		~CFontMgr();

	public:
		static std::map<CString, HIUIFONT> m_mapFonts;
	};


	HIUIFONT CreateEmptyIUIFontInternal(LPCTSTR lpszFontResID);
	HIUIFONT CreateIUIFontInternal(LPCTSTR lpszFontResID);
	int ReleaseIUIFontInternal(HIUIFONT hIUIFont);

	HFONT GetHFont(HIUIFONT hIUIFont);
	int GetFontResID(HIUIFONT hIUIFont, CString *pstrFontResID);

	//
	int IUISetControlFont(__inout HIUIFONT *phIUIFont, LPCTSTR lpszFontResID);

	int IUISetControlFont2(
		UINT uMask,
		__inout HIUIFONT *phIUIFont,
		LPCTSTR lpszFontResIDN,
		LPCTSTR lpszFontResIDD);

	int IUISetControlFont4(
		UINT uMask,
		__inout HIUIFONT *phIUIFont,
		LPCTSTR lpszFontResIDN,
		LPCTSTR lpszFontResIDH,
		LPCTSTR lpszFontResIDS,
		LPCTSTR lpszFontResIDD);

	int IUISetControlCheckedFont4(
		UINT uMask,
		__inout HIUIFONT *phIUIFont,
		LPCTSTR lpszFontResIDCN,
		LPCTSTR lpszFontResIDCH,
		LPCTSTR lpszFontResIDCS,
		LPCTSTR lpszFontResIDCD);
}
