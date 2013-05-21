#include "CCScheduler.h"

NS_CC_BEGIN;

//////////////////////////////////////////////////////////////////////////
class CCUpdateHandle : public CCObject
{
public:
	CCUpdateHandle() : m_pTarget(NULL), m_nPriority(0) {}
	virtual ~CCUpdateHandle() { CC_SAFE_RELEASE(m_pTarget); }

	int getPriority() { return m_nPriority; }
	CCObject* getTarget() { return m_pTarget; }

	virtual bool initWithTarget(CCObject* pTarget, int nPriority)
	{
		CCAssert(NULL != pTarget, "update target should not be null");
		CC_SAFE_RETAIN(pTarget);
		CC_SAFE_RELEASE(m_pTarget);
		m_pTarget = pTarget;
		m_nPriority = nPriority;
		return true;
	}

public:
	static CCUpdateHandle* handlerWithTarget(CCObject* pTarget, int nPriority)
	{
		CCUpdateHandle* pHandler = new CCUpdateHandle();
		if (NULL != pHandler)
		{
			if (pHandler->initWithTarget(pTarget, nPriority))
				pHandler->autorelease();
			else
				CC_SAFE_DELETE(pHandler);
		}
		return pHandler;
	}

protected:
	CCObject* m_pTarget;
	int m_nPriority;
};

//////////////////////////////////////////////////////////////////////////
static CCScheduler* pSharedScheduler = NULL;

CCScheduler::CCScheduler()
	: m_pHandlers(NULL)
{
	CCAssert(pSharedScheduler == NULL, "");
}

CCScheduler::~CCScheduler()
{
	m_pHandlers->removeAllObjects();
	CC_SAFE_DELETE(m_pHandlers);
	pSharedScheduler = NULL;
}

CCScheduler* CCScheduler::sharedScheduler()
{
	if (NULL == pSharedScheduler)
	{
		pSharedScheduler = new CCScheduler();
		pSharedScheduler->init();
	}

	return pSharedScheduler;
}

void CCScheduler::purgeSharedScheduler()
{
	CC_SAFE_RELEASE_NULL(pSharedScheduler);
}

bool CCScheduler::init()
{
	CCAssert(NULL == m_pHandlers, "handlers already init");
	m_pHandlers = new CCMutableArray<CCUpdateHandle*>(8);
	return true;
}

void CCScheduler::tick()
{
	CCUpdateHandle* pHandler;
	CCMutableArray<CCUpdateHandle*>::CCMutableArrayIterator iter;
	for (iter = m_pHandlers->begin(); iter != m_pHandlers->end(); ++iter)
	{
		pHandler = *iter;
		if (NULL != pHandler && NULL != pHandler->getTarget())
		{
			pHandler->getTarget()->update();
		}
	}
}

void CCScheduler::scheduleUpdateForTarget(CCObject *pTarget, int nPriority)
{
	CCUpdateHandle* pHandler = CCUpdateHandle::handlerWithTarget(pTarget, nPriority);
	if (NULL == pHandler)
		return;

	unsigned int u = 0;
	CCMutableArray<CCUpdateHandle*>::CCMutableArrayIterator iter;
	for (iter = m_pHandlers->begin(); iter != m_pHandlers->end(); ++iter)
	{
		CCUpdateHandle* handler = *iter;
		if (NULL != handler)
		{
			if (handler->getPriority() < pHandler->getPriority())
			{
				++u;
			}

			if (handler->getTarget() == pHandler->getTarget())
			{
				CCAssert(false, "the target is already added");
				return;
			}
		}
	}

	m_pHandlers->insertObjectAtIndex(pHandler, u);
}

void CCScheduler::unscheduleUpdateForTarget(const CCObject *pTarget)
{
	if (NULL == pTarget)
		return;

	CCUpdateHandle* pHandler;
	CCMutableArray<CCUpdateHandle*>::CCMutableArrayIterator iter;
	for (iter = m_pHandlers->begin(); iter != m_pHandlers->end(); ++iter)
	{
		pHandler = *iter;
		if (NULL != pHandler && pHandler->getTarget() == pTarget)
		{
			m_pHandlers->removeObject(pHandler);
			break;
		}
	}
}

NS_CC_END;