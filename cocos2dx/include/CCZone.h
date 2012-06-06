#pragma once
#include "ccMacros.h"

NS_CC_BEGIN;
class CCObject;
class CCZone
{
public:
	CCZone(CCObject* pObject = NULL);

public:
	CCObject* m_pCopyObject;
};
NS_CC_END;