#pragma once

#include "CCObject.h"

NS_CC_BEGIN;

class CC_DLL CCString : public CCObject
{
public:
	CCString()
		: m_sString("")
	{
	}

	CCString(const char* str)
	{
		m_sString = str;
	}

	virtual ~CCString()
	{
		m_sString.clear();
	}

	int toInt()
	{
		return atoi(m_sString.c_str());
	}

	unsigned int toUInt()
	{
		return (unsigned int)atoi(m_sString.c_str());
	}

	float toFloat()
	{
		return (float)atof(m_sString.c_str());
	}

	std::string toStdString()
	{
		return m_sString;
	}

	bool isEmpty()
	{
		return m_sString.empty();
	}

	virtual bool isEqual(const CCObject* pObject)
	{
		bool bRet = false;
		const CCString* pStr = dynamic_cast<const CCString*>(pObject);
		if (NULL != pStr)
		{
			if (0 == m_sString.compare(pStr->m_sString))
			{
				bRet = true;
			}
		}
		return bRet;
	}

public:
	std::string m_sString;
};

NS_CC_END;