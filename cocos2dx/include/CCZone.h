#pragma once
#include <stdio.h>

namespace cocos2d
{
	class CCObject;
	class CCZone
	{
	public:
		CCZone(CCObject* pObject = NULL);

	public:
		CCObject* m_pCopyObject;
	};
}