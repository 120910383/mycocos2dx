#include "CCScheduler.h"

NS_CC_BEGIN;

//////////////////////////////////////////////////////////////////////////
class CCUpdateHandle : public CCObject
{
public:
	CCUpdateHandle() : m_pTarget(NULL), m_nPriority(0), m_bPaused(false) {}
	virtual ~CCUpdateHandle() { CC_SAFE_RELEASE(m_pTarget); }

	int getPriority() { return m_nPriority; }
	CCObject* getTarget() { return m_pTarget; }
	bool isPaused() { return m_bPaused; }
	void setPaused(bool bPaused) { m_bPaused = bPaused; }

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
	bool m_bPaused;
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

void CCScheduler::tick(ccTime dt)
{
	CCMutableArray<CCUpdateHandle*>::CCMutableArrayIterator iter;
	for (iter = m_pHandlers->begin(); iter != m_pHandlers->end(); ++iter)
	{
		CCUpdateHandle* pHandler = *iter;
		if (NULL == pHandler || pHandler->isPaused())
			continue;

		CCObject* pTarget = pHandler->getTarget();
		if (NULL != pTarget)
		{
			pTarget->update(dt);
		}
	}
}

void CCScheduler::scheduleUpdateForTarget(CCObject *pTarget, int nPriority, bool bPaused)
{
	CCUpdateHandle* pHandler = CCUpdateHandle::handlerWithTarget(pTarget, nPriority);
	if (NULL == pHandler)
		return;

	pHandler->setPaused(bPaused);

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

void CCScheduler::pauseTarget(CCObject* pTarget)
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
			pHandler->setPaused(true);
			break;
		}
	}
}

void CCScheduler::resumeTarget(CCObject* pTarget)
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
			pHandler->setPaused(false);
			break;
		}
	}
}

bool CCScheduler::isTargetPaused(CCObject* pTarget)
{
	CCAssert( pTarget != NULL, "target must be non nil" );
	if (NULL == pTarget)
		return false;

	CCUpdateHandle* pHandler;
	CCMutableArray<CCUpdateHandle*>::CCMutableArrayIterator iter;
	for (iter = m_pHandlers->begin(); iter != m_pHandlers->end(); ++iter)
	{
		pHandler = *iter;
		if (NULL != pHandler && pHandler->getTarget() == pTarget)
		{
			return pHandler->isPaused();
		}
	}

	return false;
}

NS_CC_END;