#pragma once

#include "CCObject.h"
#include "CCGeometry.h"
#include "GLES/gl.h"
#include "ccTypes.h"
#include <string>

NS_CC_BEGIN;

class CCImage;

typedef enum {
	kCCTexture2DPixelFormat_Automatic = 0,
	kCCTexture2DPixelFormat_RGBA8888,
	kCCTexture2DPixelFormat_RGB888,
} CCTexture2DPixelFormat;

typedef struct _ccTexParams 
{
	GLuint minFilter;
	GLuint magFilter;
	GLuint wrapS;
	GLuint wrapT;
} ccTexParams;

class CC_DLL CCTexture2D : public CCObject
{
public:
	CCTexture2D();
	virtual ~CCTexture2D();

	bool initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize);
	bool initWithImage(CCImage* uiImage);
	bool initWithString(const char* text, const char* fontName, float fontSize);
	bool initWithString(const char* text, const CCSize& dimensions, CCTextAlignment alignment, const char* fontName, float fontSize);

	void setTexParameters(ccTexParams* texParams);
	void setAntiAliasTexParameters();
	void setAliasTexParameters();

	virtual GLuint getName();
	virtual unsigned int getPixelsWide();
	virtual unsigned int getPixelsHigh();
	virtual const CCSize& getContentSize();
	virtual bool getHasPremultipliedAlpha();

protected:
	GLuint m_uName;
	unsigned int m_uPixelsWide;
	unsigned int m_uPixelsHigh;
	CCSize m_tContentSize;
	bool m_bHasPremultipliedAlpha;

private:
	bool initPremultipliedATextureWithImage(CCImage* image, unsigned int pixelsWide, unsigned int pixelsHigh);
};

NS_CC_END;