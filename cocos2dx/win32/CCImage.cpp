#include "CCImage.h"
#include "CCFileUtils.h"
#include "png.h"
#include <Windows.h>
#include <string>

NS_CC_BEGIN;

#define CC_RGB_PREMULTIPLY_APLHA(vr, vg, vb, va) \
	(unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
	((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
	((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
	((unsigned)(unsigned char)(va) << 24))

typedef struct 
{
	unsigned char* data;
	int size;
	int offset;
}tImageSource;

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);

	if((int)(isource->offset + length) <= isource->size)
	{
		memcpy(data, isource->data+isource->offset, length);
		isource->offset += length;
	}
	else
	{
		png_error(png_ptr, "pngReaderCallback failed");
	}
}

class BitmapDC
{
public:
	BitmapDC(HWND hWnd = NULL)
		: m_hDC(NULL)
		, m_hBmp(NULL)
		, m_hFont((HFONT)GetStockObject(DEFAULT_GUI_FONT))
		, m_hWnd(NULL)
	{
		m_hFont = NULL;		//bugfix: 第一次调用sharedBitmapDC时s_BmpDC中m_hFont和hDefFont相同，导致setFont无效
		m_hWnd = hWnd;
		HDC hdc = GetDC(hWnd);
		m_hDC = CreateCompatibleDC(hdc);
		ReleaseDC(hWnd, hdc);
	}

	~BitmapDC()
	{
		prepareBitmap(0, 0);
		if (m_hDC)
		{
			DeleteDC(m_hDC);
		}
		HFONT hDefFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if (hDefFont != m_hFont)
		{
			DeleteObject(m_hFont);
			m_hFont = hDefFont;
		}
		// release temp font resource	
		if (m_curFontPath.size() > 0)
		{
			wchar_t* pwszBuffer = utf8ToUtf16(m_curFontPath);
			if (NULL != pwszBuffer)
			{
				RemoveFontResource(pwszBuffer);
				SendMessage( m_hWnd, WM_FONTCHANGE, 0, 0);
				delete []pwszBuffer;
				pwszBuffer = NULL;
			}
		}
	}

	wchar_t* utf8ToUtf16(std::string nString)
	{
		wchar_t* pwszBuffer = NULL;
		do 
		{
			CC_BREAK_IF(nString.size() < 0);
			// utf-8 to utf-16
			int nLen = nString.size();
			int nBufLen = nLen + 1;
			pwszBuffer = new wchar_t[nBufLen];
			CC_BREAK_IF(NULL == pwszBuffer);
			memset(pwszBuffer, 0, nBufLen);
			nLen = MultiByteToWideChar(CP_UTF8, 0, nString.c_str(), nLen, pwszBuffer, nBufLen);
			pwszBuffer[nLen] = '\0';
		} while (0);

		return pwszBuffer;
	}

	bool setFont(const char* pFontName = NULL, int nSize = 0)
	{
		bool bRet = false;
		do 
		{
			std::string fontName = pFontName;
			std::string fontPath;
			HFONT hDefFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			LOGFONTA tNewFont = {0};
			LOGFONTA tOldFont = {0};
			GetObjectA(hDefFont, sizeof(tNewFont), &tNewFont);
			if (!fontName.empty())
			{
				// create font from ttf file
				int nFindttf = fontName.find(".ttf");
				int nFindTTF = fontName.find(".TTF");
				if (nFindttf >= 0 || nFindTTF >= 0)
				{
					fontPath = CCFileUtils::fullPathFromRelativePath(fontName.c_str());
					int nFindPos = fontName.rfind("/");
					fontName = &fontName[nFindPos+1];
					nFindPos = fontName.rfind(".");
					fontName = fontName.substr(0,nFindPos);
				}
				tNewFont.lfCharSet = DEFAULT_CHARSET;
				strcpy_s(tNewFont.lfFaceName, LF_FACESIZE, fontName.c_str());
			}
			if (nSize)
			{
				tNewFont.lfHeight = -nSize;
			}
			GetObjectA(m_hFont, sizeof(tOldFont), &tOldFont);

			if (tOldFont.lfHeight == tNewFont.lfHeight && !strcpy(tOldFont.lfFaceName, tNewFont.lfFaceName))
			{
				// already has the font 
				bRet = true;
				break;
			}

			// delete old font
			if (m_hFont != hDefFont)
			{
				DeleteObject(m_hFont);
				// release old font register
				if (m_curFontPath.size() > 0)
				{
					wchar_t* pwszBuffer = utf8ToUtf16(m_curFontPath);
					if (NULL != pwszBuffer)
					{
						if(RemoveFontResource(pwszBuffer))
						{
							SendMessage(m_hWnd, WM_FONTCHANGE, 0, 0);
						}
						delete []pwszBuffer;
						pwszBuffer = NULL;
					}
				}
				fontPath.size() > 0 ? (m_curFontPath = fontPath) : (m_curFontPath.clear());
				// register temp font
				if (m_curFontPath.size() > 0)
				{
					wchar_t* pwszBuffer = utf8ToUtf16(m_curFontPath);
					if (NULL != pwszBuffer)
					{
						if(AddFontResource(pwszBuffer))
						{
							SendMessage(m_hWnd, WM_FONTCHANGE, 0, 0);
						}
						delete []pwszBuffer;
						pwszBuffer = NULL;
					}
				}
			}
			m_hFont = NULL;

			// create new font
			m_hFont = CreateFontIndirectA(&tNewFont);
			if (!m_hFont)
			{
				// create failed, use default font
				m_hFont = hDefFont;
				break;
			}

			bRet = true;
		} while (0);

		return bRet;
	}

	SIZE sizeWithText(const char* pszText, int nLen, DWORD dwFmt, LONG nWidthLimit)
	{
		SIZE tRet = {0};
		do 
		{
			CC_BREAK_IF(NULL == pszText || nLen <= 0);

			RECT rc = {0, 0, 0, 0};
			DWORD dwCalcFmt = DT_CALCRECT;

			if (nWidthLimit > 0)
			{
				rc.right = nWidthLimit;
				dwCalcFmt |= DT_WORDBREAK
					| (dwFmt & DT_CENTER)
					| (dwFmt & DT_RIGHT);
			}
			// use current font to measure text extent
			HGDIOBJ hOld = SelectObject(m_hDC, m_hFont);

			// measure text size
			DrawTextA(m_hDC, pszText, nLen, &rc, dwCalcFmt);
			SelectObject(m_hDC, hOld);

			tRet.cx = rc.right;
			tRet.cy = rc.bottom;
		} while (0);

		return tRet;
	}

	bool prepareBitmap(int nWidth, int nHeight)
	{
		// release bitmap
		if (m_hBmp)
		{
			DeleteObject(m_hBmp);
			m_hBmp = NULL;
		}
		if (nWidth > 0 && nHeight > 0)
		{
			m_hBmp = CreateBitmap(nWidth, nHeight, 1, 32, NULL);
			if (! m_hBmp)
			{
				return false;
			}
		}
		return true;
	}

	int drawText(const char* pszText, SIZE& tSize, CCImage::ETextAlign eAlign)
	{
		int nRet = 0;
		wchar_t* pwszBuffer = 0;
		do 
		{
			CC_BREAK_IF(NULL == pszText);

			DWORD dwFmt = DT_WORDBREAK;
			DWORD dwHoriFlag = eAlign & 0x0f;
			DWORD dwVertFlag = (eAlign & 0xf0) >> 4;

			switch (dwHoriFlag)
			{
			case 1: // left
				dwFmt |= DT_LEFT;
				break;
			case 2: // right
				dwFmt |= DT_RIGHT;
				break;
			case 3: // center
				dwFmt |= DT_CENTER;
				break;
			}

			int nLen = strlen(pszText);
			SIZE newSize = sizeWithText(pszText, nLen, dwFmt, tSize.cx);

			RECT rcText = {0};
			// if content width is 0, use text size as content size
			if (tSize.cx <= 0)
			{
				tSize = newSize;
				rcText.right  = newSize.cx;
				rcText.bottom = newSize.cy;
			}
			else
			{
				LONG offsetX = 0;
				LONG offsetY = 0;
				rcText.right = newSize.cx; // store the text width to rectangle

				// calculate text horizontal offset
				if (1 != dwHoriFlag          // and text isn't align to left
					&& newSize.cx < tSize.cx)   // and text's width less then content width,
				{                               // then need adjust offset of X.
					offsetX = (2 == dwHoriFlag) ? tSize.cx - newSize.cx     // align to right
						: (tSize.cx - newSize.cx) / 2;                      // align to center
				}

				// if content height is 0, use text height as content height
				// else if content height less than text height, use content height to draw text
				if (tSize.cy <= 0)
				{
					tSize.cy = newSize.cy;
					dwFmt   |= DT_NOCLIP;
					rcText.bottom = newSize.cy; // store the text height to rectangle
				}
				else if (tSize.cy < newSize.cy)
				{
					// content height larger than text height need, clip text to rect
					rcText.bottom = tSize.cy;
				}
				else
				{
					rcText.bottom = newSize.cy; // store the text height to rectangle

					// content larger than text, need adjust vertical position
					dwFmt |= DT_NOCLIP;

					// calculate text vertical offset
					offsetY = (2 == dwVertFlag) ? tSize.cy - newSize.cy     // align to bottom
						: (3 == dwVertFlag) ? (tSize.cy - newSize.cy) / 2   // align to middle
						: 0;                                                // align to top
				}

				if (offsetX || offsetY)
				{
					OffsetRect(&rcText, offsetX, offsetY);
				}
			}

			CC_BREAK_IF(!prepareBitmap(tSize.cx, tSize.cy));

			// draw text
			HGDIOBJ hOldFont = SelectObject(m_hDC, m_hFont);
			HGDIOBJ hOldBmp = SelectObject(m_hDC, m_hBmp);

			SetBkMode(m_hDC, TRANSPARENT);
			SetTextColor(m_hDC, RGB(255, 255, 255)); // white color

			// utf-8 to utf-16
			int nBufLen = nLen + 1;
			pwszBuffer = new wchar_t[nBufLen];
			CC_BREAK_IF(NULL == pwszBuffer);
			nLen = MultiByteToWideChar(CP_UTF8, 0, pszText, nLen, pwszBuffer, nBufLen);

			// draw text
			nRet = DrawTextW(m_hDC, pwszBuffer, nLen, &rcText, dwFmt);
			//DrawTextA(m_hDC, pszText, nLen, &rcText, dwFmt);

			SelectObject(m_hDC, hOldBmp);
			SelectObject(m_hDC, hOldFont);
		} while (0);

		CC_SAFE_DELETE_ARRAY(pwszBuffer);
		return nRet;
	}

	virtual HDC getDC() const { return m_hDC; }
	virtual HBITMAP getBitmap() const { return m_hBmp; }

private:
	friend class CCImage;
	HFONT m_hFont;
	HWND m_hWnd;
	HDC m_hDC;
	HBITMAP m_hBmp;
	std::string m_curFontPath;
};

static BitmapDC& sharedBitmapDC()
{
	static BitmapDC s_BmpDC;
	return s_BmpDC;
}

CCImage::CCImage()
	: m_nWidth(0)
	, m_nHeight(0)
	, m_nBitsPerComponent(0)
	, m_pData(NULL)
	, m_bHasAlpha(false)
	, m_bPreMulti(false)
{

}

CCImage::~CCImage()
{
	CC_SAFE_DELETE_ARRAY(m_pData);
}

bool CCImage::initWithImageData(void * pData, int nDataLen, EImageFormat eFmt, int nWidth, int nHeight, int nBitsPerComponent)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(NULL == pData || nDataLen <= 0);

		if (kFmtPng == eFmt)
		{
			bRet = _initWithPngData(pData, nDataLen);
			break;
		}
		else if (kFmtJpg == eFmt)
		{
			// todo...
			break;
		}
		else if (kFmtRawData == eFmt)
		{
			// todo...
			CC_UNUSED_PARAM(nWidth);
			CC_UNUSED_PARAM(nHeight);
			CC_UNUSED_PARAM(nBitsPerComponent);
			break;
		}
	} while (0);
	return bRet;
}

bool CCImage::initWithString(const char* pText, int nWidth, int nHeight, ETextAlign eAlignMask, const char* pFontName, int nSize)
{
	bool bRet = false;
	unsigned char* pImageData = NULL;
	do 
	{
		CC_BREAK_IF(NULL == pText);

		BitmapDC& dc = sharedBitmapDC();

		if (!dc.setFont(pFontName, nSize))
		{
			//输出LOG信息TODO...
			//CCLog("Can't found font(%s), use system default", pFontName);
		}

		// draw text
		SIZE size = {nWidth, nHeight};
		CC_BREAK_IF(0 == dc.drawText(pText, size, eAlignMask));

		pImageData = new unsigned char[size.cx * size.cy * 4];
		CC_BREAK_IF(NULL == pImageData);

		struct
		{
			BITMAPINFOHEADER bmiHeader;
			int mask[4];
		} bi = {0};
		bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
		CC_BREAK_IF(0 == GetDIBits(dc.getDC(), dc.getBitmap(), 0, 0, NULL, (LPBITMAPINFO)&bi, DIB_RGB_COLORS));

		m_nWidth = (short)size.cx;
		m_nHeight = (short)size.cy;
		m_bHasAlpha = true;
		m_bPreMulti = false;
		m_pData = pImageData;
		pImageData = 0;
		m_nBitsPerComponent = 8;
		// copy pixed data
		bi.bmiHeader.biHeight = (bi.bmiHeader.biHeight > 0) ? -bi.bmiHeader.biHeight : bi.bmiHeader.biHeight;
		GetDIBits(dc.getDC(), dc.getBitmap(), 0, m_nHeight, m_pData, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);

		// change pixel's alpha value to 255, when it's RGB != 0
		COLORREF * pPixel = NULL;
		for (int y = 0; y < m_nHeight; ++y)
		{
			pPixel = (COLORREF*)m_pData + y * m_nWidth;
			for (int x = 0; x < m_nWidth; ++x)
			{
				COLORREF& clr = *pPixel;
				if (GetRValue(clr) || GetGValue(clr) || GetBValue(clr))
				{
					clr |= 0xff000000;
				}
				++pPixel;
			}
		}

		bRet = true;
	} while (0);

	return bRet;
}

bool CCImage::_initWithPngData(void* pData, int nDatalen)
{
	// length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8
	bool bRet = false;
	png_byte        header[PNGSIGSIZE]   = {0}; 
	png_structp     png_ptr     =   0;
	png_infop       info_ptr    = 0;

	do 
	{
		// png header len is 8 bytes
		CC_BREAK_IF(nDatalen < PNGSIGSIZE);

		// check the data is png or not
		memcpy(header, pData, PNGSIGSIZE);
		CC_BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));

		// init png_struct
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		CC_BREAK_IF(! png_ptr);

		// init png_info
		info_ptr = png_create_info_struct(png_ptr);
		CC_BREAK_IF(!info_ptr);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
		CC_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
#endif

		// set the read call back function
		tImageSource imageSource;
		imageSource.data    = (unsigned char*)pData;
		imageSource.size    = nDatalen;
		imageSource.offset  = 0;
		png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

		// read png header info

		// read png file info
		png_read_info(png_ptr, info_ptr);

		m_nWidth = (short)png_get_image_width(png_ptr, info_ptr);
		m_nHeight = (short)png_get_image_height(png_ptr, info_ptr);
		m_nBitsPerComponent = png_get_bit_depth(png_ptr, info_ptr);
		png_uint_32 channels = png_get_channels(png_ptr, info_ptr);
		png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

		//CCLOG("color type %u", color_type);
		// only support color type: PNG_COLOR_TYPE_RGB, PNG_COLOR_TYPE_RGB_ALPHA PNG_COLOR_TYPE_PALETTE
		// and expand bit depth to 8
		switch (color_type) {
			case PNG_COLOR_TYPE_RGB:
			case PNG_COLOR_TYPE_RGB_ALPHA:
				// do nothing

				break;
			case PNG_COLOR_TYPE_PALETTE:
				png_set_palette_to_rgb(png_ptr);
				channels = 3;

				break;
			case PNG_COLOR_TYPE_GRAY:
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				if (m_nBitsPerComponent < 8)
				{
					png_set_expand_gray_1_2_4_to_8(png_ptr);
				}
				png_set_gray_to_rgb(png_ptr);
				channels = 3;

				break;

			default:
				//CCLog("unsopprted color type %u", color_type);
				goto out;
		}
		if (m_nBitsPerComponent == 16)
		{
			png_set_strip_16(png_ptr);
			m_nBitsPerComponent = 8;
		} 

		m_bHasAlpha = (color_type & PNG_COLOR_MASK_ALPHA) ? true : false;
		if (m_bHasAlpha)
		{
			channels = 4;
		}

		// read png data
		// m_nBitsPerComponent will always be 8
		m_pData = new unsigned char[m_nWidth * m_nHeight * channels];
		png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*m_nHeight);
		if (row_pointers)
		{
			const unsigned int stride = m_nWidth * channels;
			for (unsigned short i = 0; i < m_nHeight; ++i)
			{
				png_uint_32 q = i * stride;
				row_pointers[i] = (png_bytep)m_pData + q;
			}
			png_read_image(png_ptr, row_pointers);

			if (m_bHasAlpha)
			{
				unsigned int *tmp = (unsigned int *)m_pData;
				for(unsigned short i = 0; i < m_nHeight; i++)
				{
					for(unsigned int j = 0; j < m_nWidth * channels; j += 4)
					{
						*tmp++ = CC_RGB_PREMULTIPLY_APLHA( row_pointers[i][j], row_pointers[i][j + 1], 
							row_pointers[i][j + 2], row_pointers[i][j + 3] );
					}
				}

				m_bPreMulti = true;
			}

			free(row_pointers);
			bRet = true;
		}
	} while (0);

out:
	if (png_ptr)
	{
		png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
	}
	return bRet;
}

NS_CC_END;