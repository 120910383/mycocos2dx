#pragma once

#include "ccMacros.h"

NS_CC_BEGIN;

class CCImage
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

	bool initWithImageData(void* pData, 
		int nDataLen, 
		EImageFormat eFmt = kFmtPng,
		int nWidth = 0,
		int nHeight = 0,
		int nBitsPerComponent = 8);

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