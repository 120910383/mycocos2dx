#pragma once
#include <cstdio>		// use NULL macro

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