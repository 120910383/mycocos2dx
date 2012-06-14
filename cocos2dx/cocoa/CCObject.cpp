#include "CCObject.h"
#include "CCAutoreleasePool.h"

NS_CC_BEGIN;
CCObject* CCCopying::copyWithZone(CCZone* pZone)
{
	CC_UNUSED_PARAM(pZone);
	CCAssert(0, "not implement");
	return 0;
}

CCObject::CCObject(void)
{
	m_uReference = 1;			// 当创建对象时，该对象的引用计数为1
	m_bManaged = false;
}

CCObject::~CCObject(void)
{
	// 如果本对象被内存池管理，则在析构时应从管理队列中移除，以免内存池指针成为野指针
	if (m_bManaged)
	{
		CCPoolManager::getInstance()->removeObject(this);
	}
}

void CCObject::release(void)
{
	CCAssert(m_uReference > 0, "reference count should greater than 0");
	--m_uReference;

	if (m_uReference == 0)
	{
		delete this;
	}
}

void CCObject::retain(void)
{
	CCAssert(m_uReference > 0, "reference count should greater than 0");

	++m_uReference;
}

CCObject* CCObject::autorelease(void)
{
	CCPoolManager::getInstance()->addObject(this);
	m_bManaged = true;
	return this;
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
NS_CC_END;