#pragma once

#include "CCPlatformMacros.h"
#include "CCStdC.h"

NS_CC_BEGIN;

class CC_DLL CCImage
{
public:
	CCImage();
	~CCImage();

	typedef enum
	{
		kFmtJpg = 0,
		kFmtPng,
		kFmtRawData,
		kFmtUnKnown
	}EImageFormat;

	typedef enum
	{
		// 前四位为垂直方向，后四位为水平方向，1=上/左；2=下/右；3=中/中
		kAlignCenter = 0x33,
		kAlignTop = 0x13,
		kAlignTopRight = 0x12,
		kAlignRight = 0x32,
		kAlignBottomRight = 0x22,
		kAlignBottom = 0x23,
		kAlignBottomLeft = 0x21,
		kAlignLeft = 0x31,
		kAlignTopLeft = 0x11,
	}ETextAlign;

	bool initWithImageData(void* pData, int nDataLen, EImageFormat eFmt = kFmtPng, int nWidth = 0, int nHeight = 0, int nBitsPerComponent = 8);

	bool initWithString(const char* pText, int nWidth = 0, int nHeight = 0, ETextAlign eAlignMask = kAlignCenter, const char* pFontName = NULL, int nSize = 0);

	unsigned char* getData() { return m_pData; }
	int getDataLen() { return m_nWidth * m_nHeight; }
	bool hasAlpha() { return m_bHasAlpha; }
	bool isPremultipliedAlpha() { return m_bPreMulti; }
	virtual short getWidth() const { return m_nWidth; }
	virtual short getHeight() const { return m_nHeight; }
	virtual int getBitsPerComponent() const { return m_nBitsPerComponent; }

protected:
	bool _initWithPngData(void* pData, int nDatalen);

protected:
	unsigned char* m_pData;
	short m_nWidth;
	short m_nHeight;
	int m_nBitsPerComponent;
	bool m_bHasAlpha;
	bool m_bPreMulti;
};

NS_CC_END;