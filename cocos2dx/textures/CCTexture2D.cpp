#include "CCTexture2D.h"
#include "CCImage.h"
#include "CCConfiguration.h"

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

static CCTexture2DPixelFormat g_defaultAlphaPixelFormat = kCCTexture2DPixelFormat_RGBA8888;
CCTexture2D::CCTexture2D()
	: m_uPixelsWide(0)
	, m_uPixelsHigh(0)
	, m_uName(0)
	, m_bHasPremultipliedAlpha(false)
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

	switch(pixelFormat)
	{
	case kCCTexture2DPixelFormat_RGBA8888:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		break;
	case kCCTexture2DPixelFormat_RGB888:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	default:
		CCAssert(false, "NSInternalInconsistencyException");
		break;
	}

	m_tContentSize = contentSize;
	m_uPixelsWide = pixelsWide;
	m_uPixelsHigh = pixelsHigh;

	m_bHasPremultipliedAlpha = false;

	return true;
}

bool CCTexture2D::initWithImage(CCImage* uiImage)
{
	unsigned int POTWide, POTHigh;

	if (NULL == uiImage)
	{
		this->release();
		return false;
	}

	CCConfiguration* conf = CCConfiguration::sharedConfiguration();

	// 根据是否支持NPOT，决定是否使用NPOT尺寸的纹理（目前暂不适用NPOT）
	if (false && NULL != conf && conf->isSupportsNPOT())
	{
		POTWide = uiImage->getWidth();
		POTHigh = uiImage->getHeight();
	}
	else
	{
		POTWide = ccNextPOT(uiImage->getWidth());
		POTHigh = ccNextPOT(uiImage->getHeight());
	}

	unsigned int maxTextureSize = conf->getMaxTextureSize();
	if (POTHigh > maxTextureSize || POTWide > maxTextureSize)
	{
		CCAssert(false, "Image is bigger than the supported size");
		this->release();
		return false;
	}

	return initPremultipliedATextureWithImage(uiImage, POTWide, POTHigh);
}

bool CCTexture2D::initWithString(const char* text, const char* fontName, float fontSize)
{
	return initWithString(text, CCSizeZero, CCTextAlignmentCenter, fontName, fontSize);
}

bool CCTexture2D::initWithString(const char* text, const CCSize& dimensions, CCTextAlignment alignment, const char* fontName, float fontSize)
{
	CCImage image;
	CCImage::ETextAlign eAlign = (CCTextAlignmentCenter == alignment) ? CCImage::kAlignCenter
		: (CCTextAlignmentLeft == alignment) ? CCImage::kAlignLeft : CCImage::kAlignRight;

	if (!image.initWithString(text, (int)dimensions.width, (int)dimensions.height, eAlign, fontName, (int)fontSize))
	{
		return false;
	}

	return initWithImage(&image);
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

bool CCTexture2D::getHasPremultipliedAlpha()
{
	return m_bHasPremultipliedAlpha;
}

bool CCTexture2D::initPremultipliedATextureWithImage(cocos2d::CCImage *image, unsigned int POTWide, unsigned int POTHigh)
{
	unsigned char* data = NULL;
	unsigned char* tempData =NULL;
	CCSize imageSize = CCSizeMake((float)(image->getWidth()), (float)(image->getHeight()));

	CCTexture2DPixelFormat pixelFormat = image->hasAlpha() ? g_defaultAlphaPixelFormat : kCCTexture2DPixelFormat_RGB888;
	// 处理image成为POT尺寸的纹理
	switch(pixelFormat)
	{
	case kCCTexture2DPixelFormat_RGBA8888:
		{
			tempData = (unsigned char*)(image->getData());
			CCAssert(tempData != NULL, "NULL image data.");

			data = new unsigned char[POTHigh * POTWide * 4];
			memset(data, 0, POTHigh * POTWide * 4);

			if(image->getWidth() == (short)POTWide && image->getHeight() == (short)POTHigh)
			{
				memcpy(data, tempData, POTHigh * POTWide * 4);
			}
			else
			{
				unsigned char* pPixelData = (unsigned char*)tempData;
				unsigned char* pTargetData = (unsigned char*)data;

				int imageHeight = image->getHeight();
				for(int y = 0; y < imageHeight; ++y)
				{
					memcpy(pTargetData+POTWide * 4 * y, pPixelData+(image->getWidth()) * 4 * y, (image->getWidth()) * 4);
				}
			}
		}
		break;
	case kCCTexture2DPixelFormat_RGB888:
		{
			tempData = (unsigned char*)(image->getData());
			CCAssert(tempData != NULL, "NULL image data.");

			data = new unsigned char[POTHigh * POTWide * 3];
			memset(data, 0, POTHigh * POTWide * 3);

			if(image->getWidth() == (short)POTWide && image->getHeight() == (short)POTHigh)
			{
				memcpy(data, tempData, POTHigh * POTWide * 3);
			}
			else
			{
				unsigned char* pPixelData = (unsigned char*)tempData;
				unsigned char* pTargetData = (unsigned char*)data;

				int imageHeight = image->getHeight();
				for(int y = 0; y < imageHeight; ++y)
				{
					memcpy(pTargetData+POTWide * 3 * y, pPixelData+(image->getWidth()) * 3 * y, (image->getWidth()) * 3);
				}
			}
		}
		break;
	default:
		CCAssert(false, "Invalid pixel format");
		break;
	}

	// 根据指定的颜色格式，重新编码
	// 对于kCCTexture2DPixelFormat_RGBA8888无需重新编码
	// 其他格式TODO...
	// RGB565: RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA ==>RRRRRGGGGGGBBBBB
	// RGBA4444: RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA ==> RRRRGGGGBBBBAAAA
	// RGB5A1: RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA ==> RRRRRGGGGGBBBBBA

	if (data)
	{
		this->initWithData(data, pixelFormat, POTWide, POTHigh, imageSize);

		// initWithData默认设置预乘变量为false，如果image的RGB数据已经预乘，则在init之后赋值
		m_bHasPremultipliedAlpha = image->isPremultipliedAlpha();

		delete []data;
	}
	return true;
}

NS_CC_END;