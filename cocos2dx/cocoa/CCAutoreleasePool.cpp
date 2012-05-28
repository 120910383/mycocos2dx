#include "CCAutoreleasePool.h"

NS_CC_BEGIN;
CCAutoreleasePool::CCAutoreleasePool(void)
{
	m_pManagedObjectArray = new CCMutableArray<CCObject*>();
}

CCAutoreleasePool::~CCAutoreleasePool(void)
{
	CC_SAFE_DELETE(m_pManagedObjectArray);
}

void CCAutoreleasePool::addObject(CCObject* pObject)
{
	m_pManagedObjectArray->addObject(pObject);		// 引用计数加一，此时对象被内存池管理
	CCAssert(pObject->m_uReference > 1, "reference count should greager than 1");
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
		CCMutableArray<CCObject*>::CCMutableArrayRevIterator it;
		for (it = m_pManagedObjectArray->rbegin(); it != m_pManagedObjectArray->rend(); ++it)
		{
			CC_BREAK_IF(NULL == *it);
			(*it)->m_bManaged = false;
		}
		m_pManagedObjectArray->removeAllObjects();
	}
}

//--------------------------------------------------------------------
//
// CCPoolManager
//
//--------------------------------------------------------------------
CCPoolManager g_PoolManager;

CCPoolManager* CCPoolManager::getInstance()
{
	return &g_PoolManager;
}

CCPoolManager::CCPoolManager(void)
{
	m_pReleasePoolStack = new CCMutableArray<CCAutoreleasePool*>();
	m_pCurReleasePool = NULL;
}

CCPoolManager::~CCPoolManager(void)
{
	finalize();	
	// 由于pop函数每帧都会调用，而该类只保留最后一个内存池对象，因此这里也只处理唯一的一个内存池对象
	// 个人认为应该断言一下其count不大于1
	CCAssert(m_pReleasePoolStack->count() <= 1, "");
	m_pCurReleasePool = NULL;
	m_pReleasePoolStack->removeObjectAtIndex(0);
}

void CCPoolManager::finalize()
{
	// 清理所有内存池对象中的CCObject对象
	if (m_pReleasePoolStack->count() > 0)
	{
		CCMutableArray<CCAutoreleasePool*>::CCMutableArrayIterator it;
		for (it = m_pReleasePoolStack->begin(); it != m_pReleasePoolStack->end(); ++it)
		{
			CC_BREAK_IF(NULL == *it);
			(*it)->clear();
		}
	}
}

void CCPoolManager::push()
{
	// 新增一个内存池对象到内存池管理栈中
	CCAutoreleasePool* pPool = new CCAutoreleasePool();
	m_pCurReleasePool = pPool;
	m_pReleasePoolStack->addObject(pPool);
	pPool->release();
}

void CCPoolManager::pop()
{
	// 销毁一个内存池对象，并释放其所有元素，至少保留一个内存池对象（保留的内存池对象其所有元素仍销毁）
	if (NULL == m_pCurReleasePool)
	{
		return;
	}

	m_pCurReleasePool->clear();
	int nCount = m_pReleasePoolStack->count();
	if (nCount > 1)
	{
		m_pReleasePoolStack->removeObjectAtIndex(nCount - 1);
		m_pCurReleasePool = m_pReleasePoolStack->getObjectAtIndex(nCount - 2);
	}
}

void CCPoolManager::removeObject(CCObject* pObject)
{
	CCAssert(NULL != m_pCurReleasePool, "current auto release pool should not be null");
	m_pCurReleasePool->removeObject(pObject);
}

void CCPoolManager::addObject(CCObject* pObject)
{
	getCurReleasePool()->addObject(pObject);
}

CCAutoreleasePool* CCPoolManager::getCurReleasePool()
{
	if(!m_pCurReleasePool)
	{
		push();
	}

	CCAssert(NULL != m_pCurReleasePool, "current auto release pool should not be null");
	return m_pCurReleasePool;
}
NS_CC_END;