#include "CCImage.h"
#include "png.h"

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