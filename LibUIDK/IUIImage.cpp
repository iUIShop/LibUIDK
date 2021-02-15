// IUIImage.cpp: implementation of the CIUIImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


#ifndef ATLASSERT
#define ATLASSERT(expr) _ASSERTE(expr)
#endif

template< typename N >
inline N WINAPI _IUIAtlAlignUp(N n, ULONG nAlign) throw()
{
	return (N((n + (nAlign - 1)) & ~(N(nAlign) - 1)));
}

#define CHECK_AND_LOAD(RETURN_VALUE) if (this == NULL)\
	{\
		return (RETURN_VALUE);\
	}\
	if (m_hBitmap == NULL)\
	{\
		((CIUIImage *)this)->SafeLoadSavedImage();\
	}\
	if (m_hBitmap == NULL)\
	{\
		return (RETURN_VALUE);\
	}\

CImageMgr::~CImageMgr()
{
	// Release image
	if (!m_mapImages.empty())
	{
		int nCount = m_mapImages.size();

		ATLASSERT(FALSE);

		ATLTRACE(_T("[LibUIDK]===Begin track unrelease images\r\n"));
		std::map<CString, HIUIIMAGE>::iterator it = m_mapImages.begin();
		for (; it != m_mapImages.end(); ++it)
		{
			ATLTRACE(_T("\t%s\r\n"), it->second->GetSafeImageName());

			ATLASSERT(it->second != NULL);

			int nRef = it->second->Destroy();

			if (nRef > 0)
			{
				ATLTRACE(_T("[LibUIDK]===Image '%s' need call ReleaseIUIImage to release\n"),
					it->second->GetSafeImageName());
			}

			it->second->Destroy();
			delete it->second;
		}
		ATLTRACE(_T("[LibUIDK]===End track unrelease images\r\n"));
		m_mapImages.clear();
	}
}

CImageMgr g_ImageMgr; // Using it's CImageMgr::~CImageMgr

HIUIIMAGE LibUIDK::CreateEmptyIUIImage(LPCTSTR lpszImageFile)
{
	if (lpszImageFile == NULL || CString(lpszImageFile).IsEmpty())
	{
		return NULL;
	}

#ifdef _DEBUG
	if (CString(lpszImageFile).Find(_T("1.gif")) >= 0)
	{
		int n = 0;
	}
#endif
	std::map<CString, HIUIIMAGE>::iterator it = g_ImageMgr.m_mapImages.find(lpszImageFile);

	// Not found the image, create the image, and add to map.
	if (it == g_ImageMgr.m_mapImages.end())
	{
		CIUIImage *pImage = new CIUIImage;

		if (pImage != NULL)
		{
			pImage->m_strImageName = lpszImageFile;
			pImage->AddRef();

			g_ImageMgr.m_mapImages[lpszImageFile] = pImage;

			return pImage;
		}
	}
	else
	{
		it->second->AddRef();
		return it->second;
	}

	return NULL;
}

int LibUIDK::ImageManager::IUISetControlImage(__inout HIUIIMAGE *phIUIImage, LPCTSTR lpszImage)
{
	if (phIUIImage == NULL)
	{
		return -1;
	}

	CString strNewImage = lpszImage;

	// 如果新旧Image不一样，释放旧Image
	if (*phIUIImage != NULL)
	{
		if (strNewImage != (*phIUIImage)->GetSafeImageName())
		{
			ReleaseIUIImage(*phIUIImage);
		}
	}

	// 输入ID有效
	if (!strNewImage.IsEmpty())
	{
		*phIUIImage = CreateEmptyIUIImage(lpszImage);
	}
	else
	{
		*phIUIImage = NULL;
	}

	return 0;
}

int LibUIDK::ImageManager::AddCustomImage(LPCTSTR lpszImageFile, HBITMAP hImage)
{
	if (lpszImageFile == NULL || hImage == NULL || CString(lpszImageFile).IsEmpty())
	{
		return -1;
	}

	// Find the file exist
	std::map<CString, HIUIIMAGE>::iterator it = g_ImageMgr.m_mapImages.find(lpszImageFile);

	// Not found the Image, create the Image, and add to map.
	if (it != g_ImageMgr.m_mapImages.end())
	{
		return -2; // Already exist
	}

	HIUIIMAGE hIUIImage = new CIUIImage();
	hIUIImage->Attach(hImage);
	hIUIImage->m_strImageName = lpszImageFile;
	hIUIImage->AddRef();

	g_ImageMgr.m_mapImages[lpszImageFile] = hIUIImage;

	return 0;
}

int LibUIDK::ImageManager::AddCustomImage(LPCTSTR lpszImageFile, HIUIIMAGE hImage)
{
	if (lpszImageFile == NULL || hImage->GetSafeHBITMAP() == NULL || CString(lpszImageFile).IsEmpty())
	{
		return -1;
	}

	// Find the file exist
	std::map<CString, HIUIIMAGE>::iterator it = g_ImageMgr.m_mapImages.find(lpszImageFile);

	// Not found the Image, create the Image, and add to map.
	if (it != g_ImageMgr.m_mapImages.end())
	{
		return -2; // Already exist
	}

	hImage->m_strImageName = lpszImageFile;
	hImage->AddRef();

	g_ImageMgr.m_mapImages[lpszImageFile] = hImage;

	return 0;
}

int LibUIDK::ImageManager::AddCustomImage(HBITMAP hImage, __out CString *pstrImageName)
{
	if (hImage == NULL || pstrImageName == NULL)
	{
		return -1;
	}

	GUID guid;
	CoCreateGuid(&guid);
	wchar_t wszName[64] = {0};
	StringFromGUID2(guid, wszName, sizeof(wszName));

	*pstrImageName = (LPCTSTR)_bstr_t(wszName);

	return AddCustomImage(*pstrImageName, hImage);
}

int LibUIDK::ImageManager::AddCustomImage(HIUIIMAGE hImage)
{
	if (hImage == NULL)
	{
		return -1;
	}

	GUID guid;
	CoCreateGuid(&guid);
	wchar_t wszName[64] = {0};
	StringFromGUID2(guid, wszName, sizeof(wszName));

	return AddCustomImage((LPCTSTR)_bstr_t(wszName), hImage);
}

HIUIIMAGE LibUIDK::ImageManager::CreateIUIImage(LPCTSTR lpszImageFile)
{
	HIUIIMAGE hRet = CreateEmptyIUIImage(lpszImageFile);
	if (hRet == NULL)
	{
		return NULL;
	}

	if (hRet->GetSafeHBITMAP() == NULL)
	{
		BOOL bIsAbsolutePath = FALSE;
		int nLen = (int)_tcslen(lpszImageFile);
		int i = 0;
		for (i = 0; i < nLen; ++i)
		{
			if (lpszImageFile[i] == ':')
			{
				bIsAbsolutePath = TRUE;
				break;
			}
		}

		if (bIsAbsolutePath)
		{
			hRet->Load(lpszImageFile);
		}
		else
		{
			hRet->AutoLoad(lpszImageFile);
		}
	}

	return hRet;
}

HIUIIMAGE LibUIDK::ImageManager::GetIUIImage(LPCTSTR lpszImageFile)
{
	if (CString(lpszImageFile).IsEmpty())
	{
		return NULL;
	}

	std::map<CString, HIUIIMAGE>::iterator it = g_ImageMgr.m_mapImages.find(lpszImageFile);

	// Not found the image
	if (it == g_ImageMgr.m_mapImages.end())
	{
		return NULL;
	}

	return it->second;
}

int LibUIDK::ImageManager::ReleaseIUIImage(HIUIIMAGE hIUIImage)
{
	if (hIUIImage == NULL)
	{
		return -1;
	}

	CString strFileName = hIUIImage->GetSafeImageName();

	std::map<CString, HIUIIMAGE>::iterator it = g_ImageMgr.m_mapImages.find(strFileName);

	// Not found the Image
	if (it == g_ImageMgr.m_mapImages.end())
	{
		return -2;
	}

	ATLASSERT(it->second != NULL);

	int nAddRef = it->second->Release();
	if (nAddRef == 0)
	{
		delete it->second;
		g_ImageMgr.m_mapImages.erase(it);
	}

	return nAddRef;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const DWORD CIUIImage::createAlphaChannel = 0x01;

CIUIImage::CIUIImage() throw() :
	m_nAddRef(0),
	m_hBitmap(NULL),
	m_pBits(NULL),
	m_nWidth(0),
	m_nHeight(0),
	m_nPitch(0),
	m_nBPP(0),
	m_iTransparentColor(-1),
	m_clrTransparentColor((COLORREF) - 1),
	m_bGif(false),
	m_nFrameCount(1),
	m_nCurFrame(0),
	m_pPropertyItem(NULL),
	m_nPropertyItemCount(0),
	m_bHasAlphaChannel(false),
	m_bIsDIBSection(false)
{
	m_rcShadow.SetRect(0, 0, 0, 0);
}

CIUIImage::~CIUIImage() throw()
{
	if (m_nAddRef == 0)
	{
		Destroy();
	}
}

int CIUIImage::AddRef()
{
	m_nAddRef++;

	return m_nAddRef;
}

int CIUIImage::Release()
{
	m_nAddRef--;

	if (m_nAddRef <= 0)
	{
		Destroy();
	}

	return m_nAddRef;
}

CString CIUIImage::GetSafeImageName() const throw()
{
	if (this == NULL)
	{
		return _T("");
	}

	return m_strImageName;
}

HBITMAP CIUIImage::GetSafeHBITMAP() const throw()
{
	if (this == NULL)
	{
		return NULL;
	}

	return m_hBitmap;
}

CIUIImage::operator HBITMAP() const throw()
{
	return m_hBitmap;
}

int CIUIImage::Load(__inout IStream *pStream) throw()
{
	Gdiplus::Bitmap bmSrc(pStream);
	if (bmSrc.GetLastStatus() != Gdiplus::Ok)
	{
		return E_FAIL;
	}

	return CreateFromGdiplusBitmap(bmSrc);
}

int CIUIImage::Load(LPCTSTR pszFileName) throw()
{
	CString strFileExt = PathFindExtension(pszFileName);

	if (strFileExt.CompareNoCase(_T(".bmp")) == 0)
	{
		m_hBitmap = LoadHBITMAP(pszFileName);
		if (m_hBitmap == NULL)
		{
			return E_FAIL;
		}

		SIZE size;
		int nRet = GetBitmapSizeG(m_hBitmap, &size);
		if (nRet != 0)
		{
			return E_FAIL;
		}

		m_nWidth = size.cx;
		m_nHeight = size.cy;

		return S_OK;
	}
	else
	{
		Gdiplus::Bitmap bmSrc((const wchar_t *)(_bstr_t)pszFileName);
		if (bmSrc.GetLastStatus() != Gdiplus::Ok)
		{
			return E_FAIL;
		}

		// 因为使用Gdiplus绘制性能太差，所以，如果加载到的是gif,需要把Gif每一帧从左至右排列
		// 成一个新的HBITMAP，然后用GDI绘制
		GUID guid;
		bmSrc.GetRawFormat(&guid);
		if (guid == ImageFormatGIF)
		{
			bool bGif = true;

			// 得到帧数
			UINT nCount = bmSrc.GetFrameDimensionsCount();
			GUID *pDimensionIDs = new GUID [nCount];
			bmSrc.GetFrameDimensionsList(pDimensionIDs, nCount);
			m_nFrameCount = bmSrc.GetFrameCount(&pDimensionIDs[0]);

			// 得到每一帧属性
			m_nPropertyItemCount = bmSrc.GetPropertyItemSize(PropertyTagFrameDelay);
			if (m_nPropertyItemCount > 0)
			{
				m_pPropertyItem = new PropertyItem [m_nPropertyItemCount];
				bmSrc.GetPropertyItem(PropertyTagFrameDelay, m_nPropertyItemCount, m_pPropertyItem);
			}
			delete []pDimensionIDs;
			pDimensionIDs = NULL;

			int nWidth = bmSrc.GetWidth();
			m_nHeight = bmSrc.GetHeight();

			// 把每一帧从右到右排列
			Bitmap *pImageAll = NULL;
			if (m_nFrameCount > 1)
			{
				pImageAll = ::new Bitmap(nWidth * m_nFrameCount, m_nHeight, PixelFormat32bppARGB);
				Graphics gra(pImageAll);

				for (int i = 0; i < (int)m_nFrameCount; ++i)
				{
					GUID activeGuid = FrameDimensionTime;
					bmSrc.SelectActiveFrame(&activeGuid, i);
					Rect rcDraw(i * nWidth, 0, nWidth, m_nHeight);
					gra.DrawImage(&bmSrc, rcDraw);
				}
			}

			if (pImageAll == NULL)
			{
				return -2;
			}

			int nRet = CreateFromGdiplusBitmap(*pImageAll);
			::delete pImageAll;
			pImageAll = NULL;

			// 由于CreateFromGdiplusBitmap会设置很多属性，所以需要重置这些属性
			m_bGif = bGif;
			m_nWidth = nWidth;

			return nRet;
		}
		else
		{
			return CreateFromGdiplusBitmap(bmSrc);
		}
	}
}

int CIUIImage::LoadFromResource(LPCTSTR lpszID, LPCTSTR lpszResType)
{
	// 由于这里的lpszID有可能是通过MAKEINTRESOURCE(ID)把数字强转成的ID，
	// 所以不要用PathFindExtension来得到扩展名，否则会引起崩溃。
	CString strExt = GetFileExt(lpszID, FALSE);
	if (strExt.CompareNoCase(_T("bmp")) == 0)
	{
		m_hBitmap = ::LoadBitmap(AfxFindResourceHandle(lpszID, RT_BITMAP), lpszID);
		if (m_hBitmap == NULL)
		{
			return E_FAIL;
		}

		SIZE size;
		int nRet = GetBitmapSizeG(m_hBitmap, &size);
		if (nRet != 0)
		{
			return E_FAIL;
		}

		m_nWidth = size.cx;
		m_nHeight = size.cy;
	}
	else
	{
		HRSRC hRsrc = FindResource(NULL, lpszID, lpszResType);
		if (NULL == hRsrc)
		{
			return E_FAIL;
		}

		DWORD dwSize = SizeofResource(NULL, hRsrc);
		if (0 == dwSize)
		{
			return E_FAIL;
		}

		HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
		if (NULL == hGlobal)
		{
			return E_FAIL;
		}

		LPVOID pBuffer = LockResource(hGlobal);
		if (NULL == pBuffer)
		{
			return E_FAIL;
		}

		HGLOBAL hData = GlobalAlloc(GPTR, dwSize);
		if (hData == NULL)
		{
			GlobalFree(hData);
			return E_FAIL;
		}

		char *pS = (char *)GlobalLock(hData);
		if (pS == NULL)
		{
			GlobalUnlock(hData);
			GlobalFree(hData);
			return E_FAIL;
		}

		memcpy(pS, (LPCTSTR)pBuffer, dwSize);

		IStream *pIStream = NULL;
		if (CreateStreamOnHGlobal(hData, FALSE, &pIStream) == S_OK)
		{
			int nRet = Load(pIStream);
			pIStream->Release();

			if (nRet != 0)
			{
				GlobalUnlock(hData);
				GlobalFree(hData);

				return -1;
			}
		}

		GlobalUnlock(hData);
		GlobalFree(hData);
	}

	return 0;
}

// Auto load image from file or rc by CUIMgr::m_bLoadFromFile.
int CIUIImage::AutoLoad(LPCTSTR lpszImageName) throw()
{
	if (CUIMgr::IsLoadFromFile())
	{
		return Load(CUIMgr::GetUIPathWithoutTitle() + lpszImageName);
	}
	else
	{
		CString strCode = GetFileNameHexCode(lpszImageName, FALSE);
		return LoadFromResource(strCode, CUIMgr::GetUIResourceType());
	}
}

int CIUIImage::SafeLoadSavedImage()
{
	if (this == NULL)
	{
		return -1;
	}

	if (m_hBitmap != NULL)
	{
		return 1;
	}

	int nRet = AutoLoad(GetSafeImageName());
	if (nRet != 0)
	{
		nRet = Load(GetSafeImageName());

		if (nRet)
		{
			return nRet;
		}
	}

	return 0;
}

int CIUIImage::Create(int nWidth, int nHeight, int nBPP, DWORD dwFlags/* = 0*/) throw()
{
	return CreateEx(nWidth, nHeight, nBPP, BI_RGB, NULL, dwFlags);
}

int CIUIImage::CreateEx(int nWidth, int nHeight, int nBPP, DWORD eCompression, const DWORD *pdwBitmasks/* = NULL*/, DWORD dwFlags/* = 0*/) throw()
{
	LPBITMAPINFO pbmi = NULL;

	if (dwFlags & createAlphaChannel)
	{
		ATLASSERT((nBPP == 32) && (eCompression == BI_RGB));
	}

	pbmi = (LPBITMAPINFO)new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
	if (pbmi == NULL)
	{
		return -1;
	}

	memset(&pbmi->bmiHeader, 0, sizeof(pbmi->bmiHeader));
	pbmi->bmiHeader.biSize = sizeof(pbmi->bmiHeader);
	pbmi->bmiHeader.biWidth = nWidth;
	pbmi->bmiHeader.biHeight = nHeight;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = USHORT(nBPP);
	pbmi->bmiHeader.biCompression = eCompression;
	if (nBPP <= 8)
	{
		ATLASSERT(eCompression == BI_RGB);
#pragma warning(push)
#pragma warning(disable:4068) //Disable unknown pragma warning that prefast pragma causes.
#pragma prefast(push)
#pragma prefast(disable:203, "no buffer overrun here, buffer was alocated properly")
		memset(pbmi->bmiColors, 0, 256 * sizeof(RGBQUAD));
#pragma prefast(pop)
#pragma warning(pop)
	}

	else
	{
		if (eCompression == BI_BITFIELDS)
		{
			ATLASSERT(pdwBitmasks != NULL);
			memcpy(pbmi->bmiColors, pdwBitmasks, 3 * sizeof(DWORD));
		}
	}

	// If you pass a pointer to the 4th param of CreateDIBSection, Must set to NULL after not use it,
	// otherwise, BoundsChecker report: 因重新分配发生内存泄漏：由 CreateDIBSection 分配的地址 0xXXXXXXXX (XX)。
	// If you want use the value of pBits, you can call:
	// DIBSECTION dibsection;
	// ::GetObject(m_hBitmap, sizeof(DIBSECTION), &dibsection);
	// the dibsection.dsBm.bmBits is you want
	HBITMAP hBitmap = ::CreateDIBSection(NULL, pbmi, DIB_RGB_COLORS, NULL, NULL, 0);

	delete [] pbmi;
	if (hBitmap == NULL)
	{
		return -2;
	}

	Attach(hBitmap, (nHeight < 0) ? DIBOR_TOPDOWN : DIBOR_BOTTOMUP);

	if (dwFlags & createAlphaChannel)
	{
		m_bHasAlphaChannel = true;
	}

	return 0;
}

int CIUIImage::Attach(HBITMAP hBitmap, DIBOrientation eOrientation/* = DIBOR_DEFAULT*/) throw()
{
	if (hBitmap == NULL)
	{
		return -1;
	}

	ATLASSERT(m_hBitmap == NULL);

	m_hBitmap = hBitmap;

	UpdateBitmapInfo(eOrientation);

	return 0;
}

int CIUIImage::Attach(HGPBITMAP hgpBitmap)
{
	if (hgpBitmap == NULL)
	{
		return -1;
	}

	return CreateFromGdiplusBitmap(*hgpBitmap);
}

// Move himgSrc to this, and clear himgSrc.
int CIUIImage::Attach(CIUIImage &himgSrc)
{
	m_hBitmap = himgSrc.m_hBitmap;
	m_pBits = himgSrc.m_pBits;
	m_nWidth = himgSrc.m_nWidth;
	m_nHeight = himgSrc.m_nHeight;
	m_nBPP = himgSrc.m_nBPP;
	m_nPitch = himgSrc.m_nPitch;
	m_iTransparentColor = himgSrc.m_iTransparentColor;
	m_clrTransparentColor = himgSrc.m_clrTransparentColor;
	m_bGif = himgSrc.m_bGif;
	m_nFrameCount = himgSrc.m_nFrameCount;
	m_nCurFrame = himgSrc.m_nCurFrame;
	if (himgSrc.m_pPropertyItem != NULL && m_nPropertyItemCount > 0)
	{
		memcpy(m_pPropertyItem, himgSrc.m_pPropertyItem, m_nPropertyItemCount);
		m_nPropertyItemCount = himgSrc.m_nPropertyItemCount;
	}
	m_bHasAlphaChannel = himgSrc.m_bHasAlphaChannel;
	m_bIsDIBSection = himgSrc.m_bIsDIBSection;
	m_rcShadow = himgSrc.m_rcShadow;

	himgSrc.Detach();

	return 0;
}

HBITMAP CIUIImage::Detach() throw()
{
	HBITMAP hBitmap;

	//	ATLASSERT(m_hBitmap != NULL);

	hBitmap = m_hBitmap;
	m_hBitmap = NULL;
	m_pBits = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBPP = 0;
	m_nPitch = 0;
	m_iTransparentColor = -1;
	m_clrTransparentColor = (COLORREF) - 1;
	m_bGif = false;
	m_nFrameCount = 1;
	m_nCurFrame = 0;
	m_pPropertyItem = NULL;
	m_nPropertyItemCount = 0;
	m_bHasAlphaChannel = false;
	m_bIsDIBSection = false;
	m_rcShadow.SetRect(0, 0, 0, 0);

	return hBitmap;
}

int CIUIImage::ConvertToDPIImage(int nCurDPIX, int nCurDPIY)
{
	if (this == NULL)
	{
		return -2;
	}

	if (m_hBitmap == NULL)
	{
		return -1;
	}

	int nDPIX = 96;
	int nDPIY = 96;

	// DPI not change
	if (nDPIX == nCurDPIX && nDPIY == nCurDPIY)
	{
		return 0;
	}
	else
	{
		int nNewWidth = MulDiv(nCurDPIX, GetWidth(), nDPIX);
		int nNewHeight = MulDiv(nCurDPIY, GetHeight(), nDPIY);

		//
		if (nDPIX == -1 || nDPIX == 0 || nDPIY == -1 || nDPIY == 0)
		{
			nNewWidth = GetWidth();
			nNewHeight = GetHeight();
		}

		HIUIIMAGE himgNew = NULL;
		int nRet = StretchBitmap(this, &himgNew, nNewWidth, nNewHeight);
		if (nRet != 0)
		{
			return nRet;
		}

		return Attach(himgNew->GetSafeHBITMAP());
	}
}

int CIUIImage::CreateFromGdiplusBitmap(__inout Gdiplus::Bitmap &bmSrc) throw()
{
	//
	// Adjust image
	//
	Gdiplus::PixelFormat eSrcPixelFormat = bmSrc.GetPixelFormat();
	UINT nBPP = 32;
	DWORD dwFlags = 0;
	Gdiplus::PixelFormat eDestPixelFormat = PixelFormat32bppRGB;
	if (eSrcPixelFormat & PixelFormatGDI)
	{
		nBPP = Gdiplus::GetPixelFormatSize(eSrcPixelFormat);
		eDestPixelFormat = eSrcPixelFormat;
	}

	if (Gdiplus::IsAlphaPixelFormat(eSrcPixelFormat))
	{
		nBPP = 32;
		dwFlags |= createAlphaChannel;
		eDestPixelFormat = PixelFormat32bppARGB;
	}

	//
	// Convert Gdiplus image to HBITMAP
	//
	{
		int nRet = Create(bmSrc.GetWidth(), bmSrc.GetHeight(), nBPP, dwFlags);
		if (nRet != 0)
		{
			return E_FAIL;
		}
	}

	//
	//
	//
	// Adjust
	Gdiplus::ColorPalette *pPalette = NULL;
	if (Gdiplus::IsIndexedPixelFormat(eSrcPixelFormat))
	{
		UINT nPaletteSize = bmSrc.GetPaletteSize();
		pPalette = (Gdiplus::ColorPalette *)new char[nPaletteSize];
		if (pPalette == NULL)
		{
			return E_OUTOFMEMORY;
		}

		bmSrc.GetPalette(pPalette, nPaletteSize);

		RGBQUAD argbPalette[256];
		// ATLENSURE_RETURN( (pPalette->Count > 0) && (pPalette->Count <= 256) );
		for (UINT iColor = 0; iColor < pPalette->Count; iColor++)
		{
			Gdiplus::ARGB color = pPalette->Entries[iColor];
			argbPalette[iColor].rgbRed = (BYTE)((color >> RED_SHIFT) & 0xff);
			argbPalette[iColor].rgbGreen = (BYTE)((color >> GREEN_SHIFT) & 0xff);
			argbPalette[iColor].rgbBlue = (BYTE)((color >> BLUE_SHIFT) & 0xff);
			argbPalette[iColor].rgbReserved = 0;
		}

		// SetColorTable(0, pPalette->Count, argbPalette);
	}

	if (eDestPixelFormat == eSrcPixelFormat)
	{
		// The pixel formats are identical, so just memcpy the rows.
		Gdiplus::BitmapData data;
		Gdiplus::Rect rect(0, 0, m_nWidth, m_nHeight);
		if (bmSrc.LockBits(&rect, Gdiplus::ImageLockModeRead, eSrcPixelFormat, &data) != Gdiplus::Ok)
		{
			return E_OUTOFMEMORY;
		}

		size_t nBytesPerRow = _IUIAtlAlignUp(nBPP * m_nWidth, 8) / 8;
		BYTE *pbDestRow = static_cast<BYTE *>(m_pBits);
		BYTE *pbSrcRow = static_cast<BYTE *>(data.Scan0);
		for (int y = 0; y < m_nHeight; y++)
		{
			memcpy(pbDestRow, pbSrcRow, nBytesPerRow);

			// the pbDestRow will out of range at last step of loop.
			if (y < m_nHeight - 1)
			{
				pbDestRow += m_nPitch;
			}

			pbSrcRow += data.Stride;
		}

		bmSrc.UnlockBits(&data);
	}
	else
	{
		// Let GDI+ work its magic
		Gdiplus::Bitmap bmDest(m_nWidth, m_nHeight, m_nPitch, eDestPixelFormat, static_cast<BYTE *>(m_pBits));
		Gdiplus::Graphics gDest(&bmDest);

		gDest.DrawImage(&bmSrc, 0, 0);
	}

	if (m_nBPP == 32) // More, Has alpha channel
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			for (int y = 0; y < m_nHeight; y++)
			{
				byte *pByte = (byte *)(m_pBits) + (y * m_nPitch) + ((x * m_nBPP) / 8);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}

	return S_OK;
}

int CIUIImage::UpdateBitmapInfo(DIBOrientation eOrientation)
{
	DIBSECTION dibsection;
	int nBytes;

	nBytes = ::GetObject(m_hBitmap, sizeof(DIBSECTION), &dibsection);
	if (nBytes == sizeof(DIBSECTION))
	{
		m_bIsDIBSection = true;
		m_nWidth = dibsection.dsBmih.biWidth;
		m_nHeight = abs(dibsection.dsBmih.biHeight);
		m_nBPP = dibsection.dsBmih.biBitCount;
		m_nPitch = ComputePitch(m_nWidth, m_nBPP);
		void *pOrigBits = dibsection.dsBm.bmBits;
		if (eOrientation == DIBOR_DEFAULT)
		{
			eOrientation = (dibsection.dsBmih.biHeight > 0) ? DIBOR_BOTTOMUP : DIBOR_TOPDOWN;
		}
		if (eOrientation == DIBOR_BOTTOMUP)
		{
			m_pBits = LPBYTE(pOrigBits) + ((m_nHeight - 1) * m_nPitch);
			m_nPitch = -m_nPitch;
		}

		pOrigBits = NULL;
	}
	else
	{
		// Non-DIBSection
		ATLASSERT(nBytes == sizeof(BITMAP));
		m_bIsDIBSection = false;
		m_nWidth = dibsection.dsBm.bmWidth;
		m_nHeight = dibsection.dsBm.bmHeight;
		m_nBPP = dibsection.dsBm.bmBitsPixel;
		m_nPitch = 0;
		m_pBits = 0;
	}

	m_iTransparentColor = -1;
	m_bHasAlphaChannel = false;

	return 0;
}

int CIUIImage::Destroy() throw()
{
	if (m_hBitmap != NULL)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	m_hBitmap = NULL;
	m_pBits = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBPP = 0;
	m_nPitch = 0;
	m_iTransparentColor = -1;
	m_clrTransparentColor = (COLORREF) - 1;
	m_bGif = false;
	m_nFrameCount = 1;
	m_nCurFrame = 0;
	delete [] m_pPropertyItem;
	m_pPropertyItem = NULL;
	m_nPropertyItemCount = 0;
	m_bHasAlphaChannel = false;
	m_bIsDIBSection = false;
	m_rcShadow.SetRect(0, 0, 0, 0);

	return 0;
}

int CIUIImage::GetBPP() const throw()
{
	ATLASSERT(m_hBitmap != NULL);

	return (m_nBPP);
}

int CIUIImage::GetHeight() const throw()
{
	CHECK_AND_LOAD(0);

	return m_nHeight;
}

int CIUIImage::GetWidth() const throw()
{
	CHECK_AND_LOAD(0);

	return m_nWidth;
}

bool CIUIImage::IsHasAlphaChannel() const throw()
{
	if (this == NULL)
	{
		return false;
	}

	CIUIImage *pThis = (CIUIImage *)this;
	pThis->SafeLoadSavedImage();

	return m_bHasAlphaChannel;
}

bool CIUIImage::IsGif() const throw()
{
	return m_bGif;
}

UINT CIUIImage::GetFrameCount() const throw()
{
	return m_nFrameCount;
}

int CIUIImage::GetCurFrame() const throw()
{
	return m_nCurFrame;
}

int CIUIImage::SetCurFrame(int nCurFrame) throw()
{
	if (nCurFrame >= (int)m_nFrameCount)
	{
		return -1;
	}

	m_nCurFrame = nCurFrame;

	return 0;
}

int CIUIImage::SetShadow(LPCRECT lprcShadow) throw()
{
	if (lprcShadow == NULL)
	{
		return -1;
	}

	m_rcShadow = *lprcShadow;

	return 0;
}

CRect CIUIImage::GetShowdow() const throw()
{
	return m_rcShadow;
}

//////////////////////////////////////////////////////////////
// static 函数。 对图片操作

//   _____________
//   | 0 | 1 | 2 |
//   |------------
//   | 3 | 4 | 5 |
//   |------------
//   | 6 | 7 | 8 |
//    ------------
int CIUIImage::NineGridBitmapG(HBITMAP hSrcBmp,
	POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	HBITMAP *phDstBmp, int nDestWidth, int nDestHeight)
{
	if (hSrcBmp == NULL)
	{
		return -1;
	}
	if (phDstBmp == NULL)
	{
		return -2;
	}

	ATLASSERT(phDstBmp != NULL);
	HDC hDC = ::GetDC(::GetDesktopWindow());

	// store the Font object before call DefWindowProc
	HFONT hFontOld = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);

	HDC memDst = ::CreateCompatibleDC(hDC);
	*phDstBmp = ::CreateCompatibleBitmap(hDC, nDestWidth, nDestHeight);
	ATLASSERT(*phDstBmp != NULL);
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(memDst, *phDstBmp);

	NineGridBltG(memDst, 0, 0, nDestWidth, nDestHeight,
		hSrcBmp, pt9GridResize, l9GridRepeatX, l9GridRepeatY);

	::SelectObject(memDst, hBmpOld);
	::DeleteDC(memDst);

	::SelectObject(hDC, hFontOld);
	::ReleaseDC(::GetDesktopWindow(), hDC);

	return 0;
}

int CIUIImage::PartNineGridBitmapG(HBITMAP hSrcBmp,
							   POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY, LPCRECT lprcSrcPart,
							   HBITMAP *phDstBmp, int nDestWidth, int nDestHeight)
{
	if (NULL != hSrcBmp)
	{
		return -1;
	}
	if (NULL != phDstBmp)
	{
		return -2;
	}
	if (NULL == lprcSrcPart)
	{
		return -3;
	}

	ATLASSERT(phDstBmp != NULL);
	HDC hDC = ::GetDC(::GetDesktopWindow());

	// store the Font object before call DefWindowProc
	HFONT hFontOld = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);

	HDC memDst = ::CreateCompatibleDC(hDC);
	*phDstBmp = ::CreateCompatibleBitmap(hDC, nDestWidth, nDestHeight);
	ATLASSERT(*phDstBmp != NULL);
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(memDst, *phDstBmp);

	PartNineGridBltG(memDst, CRect(0, 0, nDestWidth, nDestHeight),
		hSrcBmp, pt9GridResize, l9GridRepeatX, l9GridRepeatY, lprcSrcPart);

	::SelectObject(memDst, hBmpOld);
	::DeleteDC(memDst);

	::SelectObject(hDC, hFontOld);
	::ReleaseDC(::GetDesktopWindow(), hDC);

	return 0;
}

int CIUIImage::PartNineGridBitmapG(
	HBITMAP hSrcBmp, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPart, int nPartIndex,
	HBITMAP *phDstBmp, int nDestWidth, int nDestHeight)
{
	if (hSrcBmp == NULL)
	{
		return -1;
	}
	if (phDstBmp == NULL)
	{
		return -2;
	}
	if (nPart <= 0)
	{
		return -5;
	}
	if (nPartIndex < 0)
	{
		return -3;
	}
	if (nPartIndex >= nPart)
	{
		return -4;
	}

	ATLASSERT(phDstBmp != NULL);
	HDC hDC = ::GetDC(::GetDesktopWindow());

	// store the Font object before call DefWindowProc
	HFONT hFontOld = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);

	HDC memDst = ::CreateCompatibleDC(hDC);
	*phDstBmp = ::CreateCompatibleBitmap(hDC, nDestWidth, nDestHeight);
	ATLASSERT(*phDstBmp != NULL);
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(memDst, *phDstBmp);

	PartNineGridBltG(memDst, 0, 0, nDestWidth, nDestHeight,
		hSrcBmp, pt9GridResize, l9GridRepeatX, l9GridRepeatY, nPart, nPartIndex);

	::SelectObject(memDst, hBmpOld);
	::DeleteDC(memDst);

	::SelectObject(hDC, hFontOld);
	::ReleaseDC(::GetDesktopWindow(), hDC);

	return 0;
}

// Not use 9 Grid
int CIUIImage::StretchBitmapG(HBITMAP hSrcBmp, HBITMAP *phDstBmp, int nDestWidth, int nDestHeight)
{
	if (hSrcBmp == NULL)
	{
		return -1;
	}
	if (phDstBmp == NULL)
	{
		return -2;
	}

	ATLASSERT(phDstBmp != NULL);
	HDC hDC = ::GetDC(::GetDesktopWindow());

	// store the Font object before call DefWindowProc
	HFONT hFontOld = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);

	HDC memDst = ::CreateCompatibleDC(hDC);
	*phDstBmp = ::CreateCompatibleBitmap(hDC, nDestWidth, nDestHeight);
	ATLASSERT(*phDstBmp != NULL);
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(memDst, *phDstBmp);

	HDC memDC = ::CreateCompatibleDC(hDC);
	HBITMAP hSrcOld = (HBITMAP)::SelectObject(memDC, hSrcBmp);

	SIZE size;
	GetBitmapSizeG(hSrcBmp, &size);

	::SetStretchBltMode(memDst, COLORONCOLOR);
	::StretchBlt(memDst, 0, 0, nDestWidth, nDestHeight, memDC, 0, 0, size.cx, size.cy, SRCCOPY);
	::SelectObject(memDC, hSrcOld);

	::SelectObject(memDst, hBmpOld);
	::DeleteDC(memDst);

	::SelectObject(hDC, hFontOld);
	::ReleaseDC(::GetDesktopWindow(), hDC);

	return 0;
}

int CIUIImage::TileBitmapG(
	HBITMAP hSrcBmp,
	HBITMAP *phDstBmp,
	int nDestWidth,
	int nDestHeight,
	int nXOriginSrc/* = 0*/,
	int nYOriginSrc/* = 0*/,
	int nSrcWidth/* = -1*/,
	int nSrcHeight/* = -1*/)
{
	if (hSrcBmp == NULL)
	{
		return -1;
	}
	if (phDstBmp == NULL)
	{
		return -2;
	}

	ATLASSERT(phDstBmp != NULL);
	HDC hDC = ::GetDC(::GetDesktopWindow());

	// store the Font object before call DefWindowProc
	HFONT hFontOld = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);

	HDC memDst = ::CreateCompatibleDC(hDC);
	*phDstBmp = ::CreateCompatibleBitmap(hDC, nDestWidth, nDestHeight);
	ATLASSERT(*phDstBmp != NULL);
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(memDst, *phDstBmp);

	HDC memSrc = ::CreateCompatibleDC(hDC);
	HBITMAP hbmpOldSrc = (HBITMAP)::SelectObject(memSrc, hSrcBmp);

	TileBlt(memDst, 0, 0, nDestWidth, nDestHeight,
		memSrc, nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight, 255);

	::SelectObject(memSrc, hbmpOldSrc);
	::SelectObject(memDst, hBmpOld);
	::DeleteDC(memDst);

	::SelectObject(hDC, hFontOld);
	::ReleaseDC(::GetDesktopWindow(), hDC);

	return 0;
}

int CIUIImage::CopyBitmapG(HBITMAP hSrc, HBITMAP *phDest)
{
	if (phDest == NULL)
	{
		return -1;
	}

	CSize size;
	GetBitmapSizeG(hSrc, &size);

	HDC hDC = ::GetDC(::GetDesktopWindow());

	HDC hMemDC = ::CreateCompatibleDC(hDC);
	*phDest = ::CreateCompatibleBitmap(hDC, size.cx, size.cy);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, *phDest);

	HDC hMemDCSrc = ::CreateCompatibleDC(hDC);
	HBITMAP hOldSrc = (HBITMAP)::SelectObject(hMemDCSrc, hSrc);

	::BitBlt(hMemDC, 0, 0, size.cx, size.cy, hMemDCSrc, 0, 0, SRCCOPY);

	::SelectObject(hMemDCSrc, hOldSrc);
	::SelectObject(hMemDC, hOldBitmap);

	::DeleteDC(hMemDCSrc);
	::DeleteDC(hMemDC);
	::ReleaseDC(::GetDesktopWindow(), hDC);

	return 0;
}

int CIUIImage::SubBitmapG(HBITMAP hSrcBmp, HBITMAP *phDstBmp, const CRect &rcSub)
{
	if (hSrcBmp == NULL)
	{
		return -1;
	}
	if (phDstBmp == NULL)
	{
		return -2;
	}

	HDC hDC = ::GetDC(::GetDesktopWindow());

	HDC hMemDC = ::CreateCompatibleDC(hDC);
	HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(hMemDC, hSrcBmp);

	HDC hMemDestDC = ::CreateCompatibleDC(hDC);
	*phDstBmp = ::CreateCompatibleBitmap(hDC, rcSub.Width(), rcSub.Height());
	ATLASSERT(*phDstBmp != NULL);
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(hMemDestDC, *phDstBmp);

	::BitBlt(hMemDestDC, 0, 0, rcSub.Width(), rcSub.Height(), hMemDC, rcSub.left, rcSub.top, SRCCOPY);
	::SelectObject(hMemDC, pSrcBmpOld);
	::SelectObject(hMemDestDC, hBmpOld);

	::DeleteDC(hMemDestDC);
	::DeleteDC(hMemDC);
	::ReleaseDC(::GetDesktopWindow(), hDC);

	return 0;
}

int CIUIImage::SubBitmapG(HBITMAP hSrcBmp, HBITMAP *phDstBmp, int nX, int nY, int nWidth, int nHeight)
{
	return SubBitmapG(hSrcBmp, phDstBmp, CRect(nX, nY, nX + nWidth, nY + nHeight));
}

int CIUIImage::GetBitmapSizeG(HBITMAP obj, LPSIZE size)
{
	if (obj == NULL)
	{
		return -1;
	}

	BITMAP bitmap;
	int nCount = ::GetObject(obj, sizeof(BITMAP), &bitmap);
	if (nCount == 0)
	{
		return -2;
	}

	size->cx = bitmap.bmWidth;
	size->cy = bitmap.bmHeight;

	return 0;
}

int CIUIImage::ZoomImage(HIUIIMAGE himgSrc,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	HIUIIMAGE *phimgDest, int nDestWidth, int nDestHeight)
{
	if (himgSrc == NULL || phimgDest == NULL || nDestWidth <= 0 || nDestHeight <= 0)
	{
		return -1;
	}

	if (eIrm == IRM_9GRID)
	{
		NineGridBitmap(himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY,
			phimgDest, nDestWidth, nDestHeight);
	}
	else if (eIrm == IRM_TILE)
	{
		TileBitmap(himgSrc, phimgDest, nDestWidth, nDestHeight, 0, 0, -1, -1);
	}
	else if (eIrm == IRM_STRETCH || eIrm == IRM_STRETCH_HIGH_QUALITY)
	{
		StretchBitmap(himgSrc, phimgDest, nDestWidth, nDestHeight);
	}
	else
	{
	}

	return 0;
}

// 9 Grid
int CIUIImage::NineGridBitmap(HIUIIMAGE himgSrc,
	POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	HIUIIMAGE *phimgDest, int nDestWidth, int nDestHeight)
{
	if (himgSrc == NULL)
	{
		return -1;
	}
	if (phimgDest == NULL)
	{
		return -2;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (himgSrc->IsHasAlphaChannel())
	{
		HDC hDC = ::GetDC(::GetDesktopWindow());

		// store the Font object before call DefWindowProc
		HFONT hFontOld = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);

		HDC memDst = ::CreateCompatibleDC(hDC);

		*phimgDest = new CIUIImage;
		(*phimgDest)->Create(nDestWidth, nDestHeight, 32, CIUIImage::createAlphaChannel);

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));
		AddCustomImage((LPCTSTR)_bstr_t(wszName), (*phimgDest));

		HBITMAP hBmpOld = (HBITMAP)::SelectObject(memDst, (*phimgDest)->GetSafeHBITMAP());

		int nRet = IUINineGridBlt(memDst, 0, 0, nDestWidth, nDestHeight,
				himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY);

		::SelectObject(memDst, hBmpOld);
		::DeleteDC(memDst);

		::SelectObject(hDC, hFontOld);
		::ReleaseDC(::GetDesktopWindow(), hDC);

		return nRet;
	}
	else
	{
		HBITMAP hbmpDest = NULL;
		int nRet = NineGridBitmapG(himgSrc->GetSafeHBITMAP(),
				pt9GridResize, l9GridRepeatX, l9GridRepeatY,
				&hbmpDest, nDestWidth, nDestHeight);
		if (nRet != 0)
		{
			return nRet;
		}

		*phimgDest = new CIUIImage;
		(*phimgDest)->Attach(hbmpDest);

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));
		AddCustomImage((LPCTSTR)_bstr_t(wszName), (*phimgDest));

		return 0;
	}
}

int CIUIImage::PartNineGridBitmap(HIUIIMAGE himgSrc,
							  POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
							  LPCRECT lprcSrcPart,
							  HIUIIMAGE *phimgDest, int nDestWidth, int nDestHeight)
{
	himgSrc->SafeLoadSavedImage();

	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -1;
	}
	if (phimgDest == NULL)
	{
		return -2;
	}
	if (nDestWidth <= 0 || nDestHeight <= 0)
	{
		return -5;
	}

	if (himgSrc->IsHasAlphaChannel())
	{
		HDC hDC = ::GetDC(::GetDesktopWindow());

		// store the Font object before call DefWindowProc
		HFONT hFontOld = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);

		HDC memDst = ::CreateCompatibleDC(hDC);

		*phimgDest = new CIUIImage;
		(*phimgDest)->Create(nDestWidth, nDestHeight, 32, CIUIImage::createAlphaChannel);
		HBITMAP hBmpOld = (HBITMAP)::SelectObject(memDst, (*phimgDest)->GetSafeHBITMAP());
		ATLASSERT(hBmpOld != NULL);

		int nRet = IUIPartNineGridBlt(memDst, CRect(0, 0, nDestWidth, nDestHeight),
			himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, lprcSrcPart);

		::SelectObject(memDst, hBmpOld);
		::DeleteDC(memDst);

		::SelectObject(hDC, hFontOld);
		::ReleaseDC(::GetDesktopWindow(), hDC);

		AddCustomImage(*phimgDest);

		return nRet;
	}
	else
	{
		HBITMAP hbmpDest = NULL;
		int nRet = PartNineGridBitmapG(himgSrc->GetSafeHBITMAP(),
			pt9GridResize, l9GridRepeatX, l9GridRepeatY, lprcSrcPart,
			&hbmpDest, nDestWidth, nDestHeight);
		if (nRet != 0)
		{
			return nRet;
		}

		*phimgDest = new CIUIImage;
		(*phimgDest)->Attach(hbmpDest);
		AddCustomImage(*phimgDest);

		return 0;
	}
}

int CIUIImage::PartNineGridBitmap(
	HIUIIMAGE himgSrc,
	POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPart, int nPartIndex,
	HIUIIMAGE *phimgDest,
	int nDestWidth, int nDestHeight)
{
	himgSrc->SafeLoadSavedImage();

	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -1;
	}
	if (phimgDest == NULL)
	{
		return -2;
	}
	if (nPart <= 0)
	{
		return -5;
	}
	if (nPartIndex < 0)
	{
		return -3;
	}
	if (nPartIndex >= nPart)
	{
		return -4;
	}
	if (nDestWidth <= 0 || nDestHeight <= 0)
	{
		return -5;
	}

	if (himgSrc->IsHasAlphaChannel())
	{
		HDC hDC = ::GetDC(::GetDesktopWindow());

		// store the Font object before call DefWindowProc
		HFONT hFontOld = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);

		HDC memDst = ::CreateCompatibleDC(hDC);

		*phimgDest = new CIUIImage;
		(*phimgDest)->Create(nDestWidth, nDestHeight, 32, CIUIImage::createAlphaChannel);
		HBITMAP hBmpOld = (HBITMAP)::SelectObject(memDst, (*phimgDest)->GetSafeHBITMAP());
		ATLASSERT(hBmpOld != NULL);

		int nRet = IUIPartNineGridBlt(memDst, 0, 0, nDestWidth, nDestHeight,
				himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, nPart, nPartIndex);

		::SelectObject(memDst, hBmpOld);
		::DeleteDC(memDst);

		::SelectObject(hDC, hFontOld);
		::ReleaseDC(::GetDesktopWindow(), hDC);

		AddCustomImage(*phimgDest);

		return nRet;
	}
	else
	{
		HBITMAP hbmpDest = NULL;
		int nRet = PartNineGridBitmapG(himgSrc->GetSafeHBITMAP(),
				pt9GridResize, l9GridRepeatX, l9GridRepeatY, nPart, nPartIndex,
				&hbmpDest, nDestWidth, nDestHeight);
		if (nRet != 0)
		{
			return nRet;
		}

		*phimgDest = new CIUIImage;
		(*phimgDest)->Attach(hbmpDest);
		AddCustomImage(*phimgDest);

		return 0;
	}
}

int CIUIImage::StretchBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDest,
	int nDestWidth, int nDestHeight)
{
	if (himgSrc == NULL)
	{
		return -1;
	}
	if (phimgDest == NULL)
	{
		return -2;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (himgSrc->IsHasAlphaChannel())
	{
		HDC hDC = ::GetDC(::GetDesktopWindow());

		// store the Font object before call DefWindowProc
		HFONT hFontOld = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);

		HDC memDst = ::CreateCompatibleDC(hDC);
		(*phimgDest)->Create(nDestWidth, nDestHeight, 32, CIUIImage::createAlphaChannel);
		HBITMAP hBmpOld = (HBITMAP)::SelectObject(memDst, (HBITMAP)(*phimgDest));

		int nRet = IUIStretchBlt(memDst, 0, 0, nDestWidth, nDestHeight, himgSrc);

		::SelectObject(memDst, hBmpOld);
		::DeleteDC(memDst);

		::SelectObject(hDC, hFontOld);
		::ReleaseDC(::GetDesktopWindow(), hDC);

		return nRet;
	}
	else
	{
		HBITMAP hbmpDest = NULL;
		int nRet = StretchBitmapG(*himgSrc, &hbmpDest, nDestWidth, nDestHeight);

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));

		USES_CONVERSION;
		*phimgDest = CreateEmptyIUIImage(W2T(wszName));
		(*phimgDest)->Attach(hbmpDest);

		return nRet;
	}

	return -3;
}

int CIUIImage::TileBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDest,
	int nDestWidth, int nDestHeight, int nXOriginSrc/* = 0*/, int nYOriginSrc/* = 0*/,
	int nSrcWidth/* = -1*/, int nSrcHeight/* = -1*/)
{
	if (himgSrc == NULL)
	{
		return -1;
	}
	if (phimgDest == NULL)
	{
		return -2;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (nSrcWidth == -1)
	{
		nSrcWidth = himgSrc->GetWidth();
	}
	if (nSrcHeight == -1)
	{
		nSrcHeight = himgSrc->GetHeight();
	}

	HDC hDC = ::GetDC(::GetDesktopWindow());

	if (himgSrc->IsHasAlphaChannel())
	{
		//
		HDC dcMemDest = ::CreateCompatibleDC(hDC);
		BOOL bRet = (*phimgDest)->Create(nDestWidth, nDestHeight, 32,
				CIUIImage::createAlphaChannel);
		HBITMAP hBmpOld = (HBITMAP)::SelectObject(dcMemDest, (HBITMAP)(*phimgDest));

		//
		int nRet = IUITileBlt(dcMemDest, CRect(0, 0, nDestWidth, nDestHeight),
				himgSrc, nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight);

		::SelectObject(dcMemDest, hBmpOld);

		// release resource
		::DeleteDC(dcMemDest);
		::ReleaseDC(::GetDesktopWindow(), hDC);

		return nRet;
	}
	else
	{
		HBITMAP hbmpDest = NULL;
		int nRet = TileBitmapG(*himgSrc, &hbmpDest, nDestWidth, nDestHeight,
				nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight);
		(*phimgDest)->Attach(hbmpDest);

		return nRet;
	}

	return -3;
}

int CIUIImage::CopyBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDest)
{
	if (himgSrc == NULL)
	{
		return -1;
	}
	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (phimgDest == NULL)
	{
		return -2;
	}

	HDC hDC = GetDC(GetDesktopWindow());
	HDC hMemDC = CreateCompatibleDC(hDC);

	if (himgSrc->IsHasAlphaChannel())
	{
		(*phimgDest)->Create(himgSrc->GetWidth(), himgSrc->GetHeight(), 32,
			CIUIImage::createAlphaChannel);

		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, (*phimgDest)->GetSafeHBITMAP());

		IUIBitBlt(hMemDC, 0, 0, himgSrc->GetWidth(), himgSrc->GetHeight(),
			himgSrc, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOldBitmap);
	}
	else
	{
		HBITMAP hbmpDest = ::CreateCompatibleBitmap(hDC,
				himgSrc->GetWidth(), himgSrc->GetHeight());
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbmpDest);

		IUIBitBlt(hMemDC, 0, 0, himgSrc->GetWidth(), himgSrc->GetHeight(),
			himgSrc, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOldBitmap);

		(*phimgDest)->Attach(hbmpDest);
	}

	DeleteDC(hMemDC);
	ReleaseDC(GetDesktopWindow(), hDC);

	return 0;
}

int CIUIImage::SubBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDest, const CRect &rcSub)
{
	if (himgSrc == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (himgSrc->IsHasAlphaChannel())
	{
		HDC hDC = GetDC(GetDesktopWindow());

		//
		HDC hMemDestDC = CreateCompatibleDC(hDC);

		*phimgDest = new CIUIImage;
		int nRet = (*phimgDest)->Create(rcSub.Width(), rcSub.Height(), 32,
				CIUIImage::createAlphaChannel);

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));
		AddCustomImage((LPCTSTR)_bstr_t(wszName), (*phimgDest));

		HBITMAP hBmpOld = (HBITMAP)::SelectObject(hMemDestDC,
				(*phimgDest)->GetSafeHBITMAP());

		//
		HDC hMemSrcDC = CreateCompatibleDC(hDC);
		HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(hMemSrcDC, (HBITMAP)(*himgSrc));

		nRet = StretchAlphaBlend(hMemDestDC, 0, 0, rcSub.Width(), rcSub.Height(),
				hMemSrcDC, rcSub.left, rcSub.top, rcSub.Width(), rcSub.Height(), 255);

		::SelectObject(hMemSrcDC, pSrcBmpOld);
		::SelectObject(hMemDestDC, hBmpOld);

		DeleteDC(hMemSrcDC);
		DeleteDC(hMemDestDC);
		ReleaseDC(GetDesktopWindow(), hDC);

		return 0;
	}
	else
	{
		HBITMAP hbmpDest = NULL;
		int nRet = SubBitmapG(*himgSrc, &hbmpDest, rcSub);
		if (nRet != 0 || hbmpDest == NULL)
		{
			return nRet;
		}

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));

		*phimgDest = CreateEmptyIUIImage((LPCTSTR)_bstr_t(wszName));
		(*phimgDest)->Attach(hbmpDest);

		return 0;
	}
}

int CIUIImage::SubBitmap(HIUIIMAGE himgSrc, HIUIIMAGE *phimgDst,
	int nX, int nY, int nWidth, int nHeight)
{
	return SubBitmap(himgSrc, phimgDst, CRect(nX, nY, nX + nWidth, nY + nHeight));
}

HBITMAP CIUIImage::LoadHBITMAP(LPCTSTR lpszFile)
{
	return (HBITMAP)LoadImage(AfxGetInstanceHandle(),
			lpszFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

HBITMAP CIUIImage::LoadHBITMAPFromRes(UINT uID)
{
	return ::LoadBitmap(AfxFindResourceHandle(MAKEINTRESOURCE(uID), RT_BITMAP),
			MAKEINTRESOURCE(uID));
}

// Auto load image from file or rc use gdi mode by CUIMgr::m_bLoadFromFile.
HBITMAP CIUIImage::AutoLoadHBITMAP(LPCTSTR lpszFile)
{
	if (CUIMgr::IsLoadFromFile())
	{
		return LoadHBITMAP(CUIMgr::GetUIPathWithoutTitle() + lpszFile);
	}
	else
	{
		CString strFile = GetFileNameHexCode(lpszFile, FALSE);
		return ::LoadBitmap(AfxFindResourceHandle(strFile, RT_BITMAP), strFile);
	}
}

CString CIUIImage::GetFileNameHexCode(LPCTSTR lpszImageName, BOOL bEncodeExt)
{
	// Must utf-16.
	std::wstring wstrFileName = (LPCWSTR)_bstr_t(lpszImageName);
	// ABC.bmp and abc.bmp must be the same one hex code.
	std::transform(wstrFileName.begin(), wstrFileName.end(), wstrFileName.begin(), towlower);

	if (bEncodeExt)
	{
		return (LPCTSTR)_bstr_t(GetHexCodeW(wstrFileName.c_str()).c_str());
	}

	// Prefix, the hex code string may a number, So need a prefix.
	std::wstring wstrFileHexCode = L"I";

	std::wstring wstrTitle = GetFileTitleExW(wstrFileName.c_str());
	std::wstring wstrExt = GetFileExtW(wstrFileName.c_str(), TRUE);
	wstrFileHexCode += (LPCWSTR)_bstr_t(GetHexCodeW(wstrTitle.c_str()).c_str());
	wstrFileHexCode += wstrExt;

	CString strCode = (LPCTSTR)_bstr_t(wstrFileHexCode.c_str());
	return strCode;
}
