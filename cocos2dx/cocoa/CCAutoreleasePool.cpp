#include "CCAutoreleasePool.h"
#include <cassert>

namespace cocos2d
{
	CCAutoreleasePool::CCAutoreleasePool(void)
	{
		m_pManagedObjectArray = new CCMutableArray<CCObject*>();
	}

	CCAutoreleasePool::~CCAutoreleasePool(void)
	{
		if (NULL != m_pManagedObjectArray)
		{
			delete m_pManagedObjectArray;
			m_pManagedObjectArray = NULL;
		}
	}

	void CCAutoreleasePool::addObject(CCObject* pObject)
	{
		m_pManagedObjectArray->addObject(pObject);		// 引用计数加一，此时对象被内存池管理
		assert(pObject->m_uReference > 1);
		pObject->release();		// 为保持原有对象的引用计数，引用计数减一，也即内存池不应影响原有引用计数，而只是在恰当的时间使其减一（可能释放可能不释放）
	}

	void CCAutoreleasePool::removeObject(CCObject* pObject)
	{
		m_pManagedObjectArray->removeObject(pObject, false);
	}

	void CCAutoreleasePool::clear()
	{
		if (m_pManagedObjectArray->count() > 0)
		{
			m_pManagedObjectArray->removeAllObjects();
		}
	}
}