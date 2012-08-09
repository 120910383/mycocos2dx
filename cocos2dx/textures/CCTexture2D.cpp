#include "CCTexture2D.h"
#include "CCImage.h"

NS_CC_BEGIN;

/** returns the Next Power of Two value.

Examples:
- If "value" is 15, it will return 16.
- If "value" is 16, it will return 16.
- If "value" is 17, it will return 32.
*/
unsigned long ccNextPOT(unsigned long x)
{
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >>16);
	return x + 1;
}

static CCTexture2DPixelFormat g_defaultAlphaPixelFormat = kCCTexture2DPixelFormat_Default;
CCTexture2D::CCTexture2D()
: m_uPixelsWide(0)
, m_uPixelsHigh(0)
, m_uName(0)
{

}

CCTexture2D::~CCTexture2D()
{

}

bool CCTexture2D::initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(1, &m_uName);
	glBindTexture(GL_TEXTURE_2D, m_uName);

	this->setAntiAliasTexParameters();

	// Specify OpenGL texture image

	switch(pixelFormat)
	{
	case kCCTexture2DPixelFormat_RGBA8888:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		break;
	case kCCTexture2DPixelFormat_RGB888:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	case kCCTexture2DPixelFormat_RGBA4444:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
		break;
	case kCCTexture2DPixelFormat_RGB5A1:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
		break;
	case kCCTexture2DPixelFormat_RGB565:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
		break;
	case kCCTexture2DPixelFormat_AI88:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
		break;
	case kCCTexture2DPixelFormat_A8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
		break;
	default:
		CCAssert(0, "NSInternalInconsistencyException");

	}

	m_tContentSize = contentSize;
	m_uPixelsWide = pixelsWide;
	m_uPixelsHigh = pixelsHigh;
	//m_ePixelFormat = pixelFormat;
	//m_fMaxS = contentSize.width / (float)(pixelsWide);
	//m_fMaxT = contentSize.height / (float)(pixelsHigh);

	//m_bHasPremultipliedAlpha = false;

	return true;
}

bool CCTexture2D::initWithImage(CCImage* uiImage)
{
	if (NULL == uiImage)
	{
		this->release();
		return false;
	}

	//unsigned int POTWide = ccNextPOT(uiImage->getWidth());
	//unsigned int POTHigh = ccNextPOT(uiImage->getHeight());
	unsigned int imageWidth = uiImage->getWidth();
	unsigned int imageHeight = uiImage->getHeight();

	return initPremultipliedATextureWithImage(uiImage, imageWidth, imageHeight);
}

void CCTexture2D::setTexParameters(ccTexParams* texParams)
{
	CCAssert( m_uPixelsWide == ccNextPOT(m_uPixelsWide) && m_uPixelsHigh == ccNextPOT(m_uPixelsHigh)
		&& texParams->wrapS == GL_CLAMP_TO_EDGE && texParams->wrapT == GL_CLAMP_TO_EDGE,
		"GL_CLAMP_TO_EDGE should be used in NPOT textures");

	glBindTexture(GL_TEXTURE_2D, this->m_uName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParams->minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParams->magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParams->wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParams->wrapT);
}

void CCTexture2D::setAntiAliasTexParameters()
{
	ccTexParams texParams = { GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	this->setTexParameters(&texParams);
}

void CCTexture2D::setAliasTexParameters()
{
	ccTexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	this->setTexParameters(&texParams);
}

GLuint CCTexture2D::getName()
{
	return m_uName;
}

unsigned int CCTexture2D::getPixelsWide()
{
	return m_uPixelsWide;
}

unsigned int CCTexture2D::getPixelsHigh()
{
	return m_uPixelsHigh;
}

const CCSize& CCTexture2D::getContentSize()
{
	return m_tContentSize;
}

bool CCTexture2D::initPremultipliedATextureWithImage(cocos2d::CCImage *image, unsigned int POTWide, unsigned int POTHigh)
{
	unsigned char*			data = NULL;
	unsigned char*			tempData =NULL;
	unsigned int*			inPixel32 = NULL;
	unsigned short*			outPixel16 = NULL;
	bool					hasAlpha;
	CCSize					imageSize;
	CCTexture2DPixelFormat	pixelFormat;

	hasAlpha = image->hasAlpha();

	size_t bpp = image->getBitsPerComponent();

	// compute pixel format
	if(hasAlpha)
	{
		pixelFormat = g_defaultAlphaPixelFormat;
	}
	else
	{
		if (bpp >= 8)
		{
			pixelFormat = kCCTexture2DPixelFormat_RGB888;
		}
		else
		{
			//CCLOG("cocos2d: CCTexture2D: Using RGB565 texture since image has no alpha");
			pixelFormat = kCCTexture2DPixelFormat_RGB565;
		}
	}


	imageSize = CCSizeMake((float)(image->getWidth()), (float)(image->getHeight()));

	switch(pixelFormat) {          
		case kCCTexture2DPixelFormat_RGBA8888:
		case kCCTexture2DPixelFormat_RGBA4444:
		case kCCTexture2DPixelFormat_RGB5A1:
		case kCCTexture2DPixelFormat_RGB565:
		case kCCTexture2DPixelFormat_A8:
			tempData = (unsigned char*)(image->getData());
			CCAssert(tempData != NULL, "NULL image data.");

			if(image->getWidth() == (short)POTWide && image->getHeight() == (short)POTHigh)
			{
				data = new unsigned char[POTHigh * POTWide * 4];
				memcpy(data, tempData, POTHigh * POTWide * 4);
			}
			else
			{
				data = new unsigned char[POTHigh * POTWide * 4];
				memset(data, 0, POTHigh * POTWide * 4);

				unsigned char* pPixelData = (unsigned char*) tempData;
				unsigned char* pTargetData = (unsigned char*) data;

				int imageHeight = image->getHeight();
				for(int y = 0; y < imageHeight; ++y)
				{
					memcpy(pTargetData+POTWide*4*y, pPixelData+(image->getWidth())*4*y, (image->getWidth())*4);
				}
			}

			break;    
		case kCCTexture2DPixelFormat_RGB888:
			tempData = (unsigned char*)(image->getData());
			CCAssert(tempData != NULL, "NULL image data.");
			if(image->getWidth() == (short)POTWide && image->getHeight() == (short)POTHigh)
			{
				data = new unsigned char[POTHigh * POTWide * 3];
				memcpy(data, tempData, POTHigh * POTWide * 3);
			}
			else
			{
				data = new unsigned char[POTHigh * POTWide * 3];
				memset(data, 0, POTHigh * POTWide * 3);

				unsigned char* pPixelData = (unsigned char*) tempData;
				unsigned char* pTargetData = (unsigned char*) data;

				int imageHeight = image->getHeight();
				for(int y = 0; y < imageHeight; ++y)
				{
					memcpy(pTargetData+POTWide*3*y, pPixelData+(image->getWidth())*3*y, (image->getWidth())*3);
				}
			}
			break;   
		default:
			CCAssert(0, "Invalid pixel format");
	}

	// Repack the pixel data into the right format

	if(pixelFormat == kCCTexture2DPixelFormat_RGB565) {
		//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
		tempData = new unsigned char[POTHigh * POTWide * 2];
		inPixel32 = (unsigned int*)data;
		outPixel16 = (unsigned short*)tempData;

		unsigned int length = POTWide * POTHigh;
		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
		{
			*outPixel16++ = 
				((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) |  // R
				((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) |   // G
				((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);   // B
		}

		delete [] data;
		data = tempData;
	}
	else if (pixelFormat == kCCTexture2DPixelFormat_RGBA4444) {
		//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
		tempData = new unsigned char[POTHigh * POTWide * 2];
		inPixel32 = (unsigned int*)data;
		outPixel16 = (unsigned short*)tempData;

		unsigned int length = POTWide * POTHigh;
		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
		{
			*outPixel16++ = 
				((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
				((((*inPixel32 >> 8) & 0xFF) >> 4) << 8) | // G
				((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
				((((*inPixel32 >> 24) & 0xFF) >> 4) << 0); // A
		}

		delete [] data;
		data = tempData;
	}
	else if (pixelFormat == kCCTexture2DPixelFormat_RGB5A1) {
		//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
		tempData = new unsigned char[POTHigh * POTWide * 2];
		inPixel32 = (unsigned int*)data;
		outPixel16 = (unsigned short*)tempData;

		unsigned int length = POTWide * POTHigh;
		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
		{
			*outPixel16++ = 
				((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
				((((*inPixel32 >> 8) & 0xFF) >> 3) << 6) | // G
				((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
				((((*inPixel32 >> 24) & 0xFF) >> 7) << 0); // A
		}

		delete []data;
		data = tempData;
	}
	else if (pixelFormat == kCCTexture2DPixelFormat_A8)
	{
		// fix me, how to convert to A8
		pixelFormat = kCCTexture2DPixelFormat_RGBA8888;

		/*
		* The code can not work, how to convert to A8?
		*
		tempData = new unsigned char[POTHigh * POTWide];
		inPixel32 = (unsigned int*)data;
		outPixel8 = tempData;

		unsigned int length = POTWide * POTHigh;
		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
		{
		*outPixel8++ = (*inPixel32 >> 24) & 0xFF;
		}

		delete []data;
		data = tempData;
		*/
	}

	if (data)
	{
		this->initWithData(data, pixelFormat, POTWide, POTHigh, imageSize);

		// should be after calling super init
		//m_bHasPremultipliedAlpha = image->isPremultipliedAlpha();

		//CGContextRelease(context);
		delete [] data;
	}
	return true;
}

NS_CC_END;