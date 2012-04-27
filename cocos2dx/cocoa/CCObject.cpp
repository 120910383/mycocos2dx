#include "CCObject.h"
#include "CCZone.h"
#include <cassert>

namespace cocos2d
{
	CCObject* CCCopying::copyWithZone(CCZone* pZone)
	{
		assert(false);			// 基类无实现copyWithZone
		return 0;
	}

	CCObject::CCObject(void)
	{
		m_uReference = 1;			// 当创建对象时，该对象的引用计数为1
	}

	CCObject::~CCObject(void)
	{

	}

	void CCObject::release(void)
	{
		assert(m_uReference > 0);			//引用计数应比0大
		--m_uReference;

		if (m_uReference == 0)
		{
			delete this;
		}
	}

	void CCObject::retain(void)
	{
		assert(m_uReference > 0);			//引用计数应比0大

		++m_uReference;
	}

	CCObject* CCObject::copy(void)
	{
		return copyWithZone(NULL);
	}

	bool CCObject::isSingleRefrence(void)
	{
		return m_uReference == 1;
	}

	unsigned int CCObject::retainCount(void)
	{
		return m_uReference;
	}

	bool CCObject::isEqual(const CCObject* pObject)
	{
		return this == pObject;
	}
}