#pragma once

#include "ccMacros.h"
#include "CCStdC.h"

NS_CC_BEGIN;

class CC_DLL CCFileUtils
{
public:
	static unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long* pSize);
	static void setResourcePath(const char *pszResourcePath);
	static const char* fullPathFromRelativePath(const char *pszRelativePath);
	static void setIsPopupNotify(bool bNotify);
	static bool getIsPopupNotify();
};

class CCFileData
{
public:
	CCFileData(const char* pszFileName, const char* pszMode)
	{
		m_pBuffer = CCFileUtils::getFileData(pszFileName, pszMode, &m_uSize);
	}

	~CCFileData()
	{
		CC_SAFE_DELETE_ARRAY(m_pBuffer);
	}

	bool reset(const char* pszFileName, const char* pszMode)
	{
		CC_SAFE_DELETE_ARRAY(m_pBuffer);
		m_uSize = 0;
		m_pBuffer = CCFileUtils::getFileData(pszFileName, pszMode, &m_uSize);
		return (m_pBuffer) ? true : false;
	}

	virtual unsigned char* getBuffer() const
	{
		return m_pBuffer;
	}

	virtual unsigned long getSize() const
	{
		return m_uSize;
	}

protected:
	unsigned char* m_pBuffer;
	unsigned long m_uSize;
};

NS_CC_END;