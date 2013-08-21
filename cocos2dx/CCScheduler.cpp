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
class CCSelectorHandle : public CCObject
{
public:
	CCSelectorHandle() : m_pTarget(NULL), m_pfnSelector(NULL), m_fInterval(0), m_fElapsed(0), m_bPaused(false) {}
	virtual ~CCSelectorHandle() { CC_SAFE_RELEASE(m_pTarget); }

	CCObject* getTarget() { return m_pTarget; }
	SEL_SCHEDULE getSelector() { return m_pfnSelector; }
	bool isPaused() { return m_bPaused; }
	void setPaused(bool bPaused) { m_bPaused = bPaused; }
	ccTime getInterval() { return m_fInterval; }
	void setInterval(ccTime fSeconds) { m_fInterval = fSeconds; }

	virtual bool initWithTarget(CCObject* pTarget, SEL_SCHEDULE pfnSelector, ccTime fSeconds)
	{
		CCAssert(NULL != pTarget, "selector target should not be null");
		CC_SAFE_RETAIN(pTarget);
		CC_SAFE_RELEASE(m_pTarget);
		m_pTarget = pTarget;
		m_pfnSelector = pfnSelector;
		m_fElapsed = -1;
		m_fInterval = fSeconds;
		return true;
	}

	virtual void update(ccTime dt)
	{
		// 第一次(帧)调用时的dt不计算在总时间内
		if (-1 == m_fElapsed)
		{
			m_fElapsed = 0;
		}
		else
		{
			m_fElapsed += dt;
		}

		if (m_fElapsed >= m_fInterval)
		{
			if (NULL != m_pfnSelector)
			{
				(m_pTarget->*m_pfnSelector)(m_fElapsed);
			}
			m_fElapsed = 0;
		}
	}

public:
	static CCSelectorHandle* handlerWithTarget(CCObject* pTarget, SEL_SCHEDULE pfnSelector, ccTime fSeconds)
	{
		CCSelectorHandle* pHandler = new CCSelectorHandle();
		if (NULL != pHandler)
		{
			if (pHandler->initWithTarget(pTarget, pfnSelector, fSeconds))
				pHandler->autorelease();
			else
				CC_SAFE_DELETE(pHandler);
		}
		return pHandler;
	}

	static CCSelectorHandle* handlerWithTarget(CCObject* pTarget, SEL_SCHEDULE pfnSelector)
	{
		return handlerWithTarget(pTarget, pfnSelector, 0);
	}

protected:
	CCObject* m_pTarget;
	SEL_SCHEDULE m_pfnSelector;
	ccTime m_fInterval;
	ccTime m_fElapsed;
	bool m_bPaused;
};

//////////////////////////////////////////////////////////////////////////
static CCScheduler* pSharedScheduler = NULL;

CCScheduler::CCScheduler()
	: m_pUpdateHandlers(NULL)
	, m_pSelectorHandles(NULL)
{
	CCAssert(pSharedScheduler == NULL, "");
}

CCScheduler::~CCScheduler()
{
	unscheduleAllSelectors();
	CC_SAFE_DELETE(m_pSelectorHandles);
	CC_SAFE_DELETE(m_pUpdateHandlers);
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
	CCAssert(NULL == m_pUpdateHandlers, "handlers already init");
	m_pUpdateHandlers = new CCMutableArray<CCUpdateHandle*>(8);

	CCAssert(NULL == m_pSelectorHandles, "handlers already init");
	m_pSelectorHandles = new CCMutableArray<CCSelectorHandle*>(8);
	return true;
}

void CCScheduler::tick(ccTime dt)
{
	CCMutableArray<CCUpdateHandle*>::CCMutableArrayIterator update_iter;
	for (update_iter = m_pUpdateHandlers->begin(); update_iter != m_pUpdateHandlers->end(); ++update_iter)
	{
		CCUpdateHandle* pHandler = *update_iter;
		if (NULL == pHandler || pHandler->isPaused())
			continue;

		CCObject* pTarget = pHandler->getTarget();
		if (NULL != pTarget)
		{
			pTarget->update(dt);
		}
	}

	CCMutableArray<CCSelectorHandle*>::CCMutableArrayIterator selector_iter;
	for (selector_iter = m_pSelectorHandles->begin(); selector_iter != m_pSelectorHandles->end(); ++selector_iter)
	{
		CCSelectorHandle* pHandler = *selector_iter;
		if (NULL == pHandler || pHandler->isPaused())
			continue;

		pHandler->update(dt);
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
	for (iter = m_pUpdateHandlers->begin(); iter != m_pUpdateHandlers->end(); ++iter)
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

	m_pUpdateHandlers->insertObjectAtIndex(pHandler, u);
}

void CCScheduler::unscheduleUpdateForTarget(const CCObject *pTarget)
{
	if (NULL == pTarget)
		return;

	CCUpdateHandle* pHandler;
	CCMutableArray<CCUpdateHandle*>::CCMutableArrayIterator iter;
	for (iter = m_pUpdateHandlers->begin(); iter != m_pUpdateHandlers->end(); ++iter)
	{
		pHandler = *iter;
		if (NULL != pHandler && pHandler->getTarget() == pTarget)
		{
			m_pUpdateHandlers->removeObject(pHandler);
			break;
		}
	}
}

void CCScheduler::scheduleSelector(SEL_SCHEDULE pfnSelector, CCObject* pTarget, ccTime fInterval, bool bPaused)
{
	CCSelectorHandle* pHandler = CCSelectorHandle::handlerWithTarget(pTarget, pfnSelector, fInterval);
	if (NULL == pHandler)
		return;

	pHandler->setPaused(bPaused);

	CCMutableArray<CCSelectorHandle*>::CCMutableArrayIterator iter;
	for (iter = m_pSelectorHandles->begin(); iter != m_pSelectorHandles->end(); ++iter)
	{
		CCSelectorHandle* handler = *iter;
		if (NULL != handler)
		{
			if (handler->getTarget() == pHandler->getTarget() && handler->getSelector() == pHandler->getSelector())
			{
				handler->setInterval(fInterval);
				handler->setPaused(bPaused);
				return;
			}
		}
	}

	m_pSelectorHandles->addObject(pHandler);
}

void CCScheduler::unscheduleSelector(SEL_SCHEDULE pfnSelector, CCObject* pTarget)
{
	if (NULL == pTarget || NULL == pfnSelector)
	{
		return;
	}

	CCSelectorHandle* pHandler;
	CCMutableArray<CCSelectorHandle*>::CCMutableArrayIterator iter;
	for (iter = m_pSelectorHandles->begin(); iter != m_pSelectorHandles->end(); ++iter)
	{
		pHandler = *iter;
		if (NULL != pHandler && pHandler->getTarget() == pTarget && pHandler->getSelector() == pfnSelector)
		{
			m_pSelectorHandles->removeObject(pHandler);
			break;
		}
	}
}

void CCScheduler::pauseTarget(CCObject* pTarget)
{
	if (NULL == pTarget)
		return;

	CCUpdateHandle* pUpdateHandler;
	CCMutableArray<CCUpdateHandle*>::CCMutableArrayIterator update_iter;
	for (update_iter = m_pUpdateHandlers->begin(); update_iter != m_pUpdateHandlers->end(); ++update_iter)
	{
		pUpdateHandler = *update_iter;
		if (NULL != pUpdateHandler && pUpdateHandler->getTarget() == pTarget)
		{
			pUpdateHandler->setPaused(true);
			break;
		}
	}

	CCSelectorHandle* pSelectorHandler;
	CCMutableArray<CCSelectorHandle*>::CCMutableArrayIterator selector_iter;
	for (selector_iter = m_pSelectorHandles->begin(); selector_iter != m_pSelectorHandles->end(); ++selector_iter)
	{
		pSelectorHandler = *selector_iter;
		if (NULL != pSelectorHandler && pSelectorHandler->getTarget() == pTarget)
		{
			pSelectorHandler->setPaused(true);
		}
	}
}

void CCScheduler::resumeTarget(CCObject* pTarget)
{
	if (NULL == pTarget)
		return;

	CCUpdateHandle* pUpdateHandler;
	CCMutableArray<CCUpdateHandle*>::CCMutableArrayIterator update_iter;
	for (update_iter = m_pUpdateHandlers->begin(); update_iter != m_pUpdateHandlers->end(); ++update_iter)
	{
		pUpdateHandler = *update_iter;
		if (NULL != pUpdateHandler && pUpdateHandler->getTarget() == pTarget)
		{
			pUpdateHandler->setPaused(false);
			break;
		}
	}

	CCSelectorHandle* pSelectorHandler;
	CCMutableArray<CCSelectorHandle*>::CCMutableArrayIterator selector_iter;
	for (selector_iter = m_pSelectorHandles->begin(); selector_iter != m_pSelectorHandles->end(); ++selector_iter)
	{
		pSelectorHandler = *selector_iter;
		if (NULL != pSelectorHandler && pSelectorHandler->getTarget() == pTarget)
		{
			pSelectorHandler->setPaused(false);
		}
	}
}

void CCScheduler::unscheduleAllSelectorsForTarget(CCObject* pTarget)
{
	if (NULL == pTarget)
		return;

	CCMutableArray<CCSelectorHandle*> removed_handles;
	CCMutableArray<CCSelectorHandle*>::CCMutableArrayIterator selector_iter;
	for (selector_iter = m_pSelectorHandles->begin(); selector_iter != m_pSelectorHandles->end(); ++selector_iter)
	{
		CCSelectorHandle* pSelectorHandler = *selector_iter;
		if (NULL != pSelectorHandler && pSelectorHandler->getTarget() == pTarget)
		{
			removed_handles.addObject(pSelectorHandler);
		}
	}

	m_pSelectorHandles->removeObjectsInArray(&removed_handles);

	unscheduleUpdateForTarget(pTarget);
}

void CCScheduler::unscheduleAllSelectors()
{
	m_pUpdateHandlers->removeAllObjects();
	m_pSelectorHandles->removeAllObjects();
}

NS_CC_END;