#include "CCActionManager.h"
#include "CCScheduler.h"
#include "CCNode.h"
#include "CCAction.h"

NS_CC_BEGIN;

//////////////////////////////////////////////////////////////////////////
class CCActionHandler : public CCObject
{
public:
	CCActionHandler() : m_pTarget(NULL), m_pActions(NULL), m_bPaused(false) {}
	virtual ~CCActionHandler()
	{
		CC_SAFE_RELEASE(m_pTarget);
		CC_SAFE_DELETE(m_pActions);
	}

	CCObject* getTarget() { return m_pTarget; }
	bool isPaused() { return m_bPaused; }
	void setPaused(bool bPaused) { m_bPaused = bPaused; }

	virtual bool initWithTarget(CCNode* pTarget)
	{
		CCAssert(NULL != pTarget, "update target should not be null");
		CC_SAFE_RETAIN(pTarget);
		CC_SAFE_RELEASE(m_pTarget);
		m_pTarget = pTarget;

		CC_SAFE_DELETE(m_pActions);
		m_pActions = new CCMutableArray<CCAction*>;
		return true;
	}

	virtual CCMutableArray<CCAction*>* getActions()
	{
		return m_pActions;
	}

public:
	static CCActionHandler* handlerWithTarget(CCNode* pTarget)
	{
		CCActionHandler* pHandler = new CCActionHandler();
		if (NULL != pHandler)
		{
			if (pHandler->initWithTarget(pTarget))
				pHandler->autorelease();
			else
				CC_SAFE_DELETE(pHandler);
		}
		return pHandler;
	}

protected:
	CCNode* m_pTarget;
	CCMutableArray<CCAction*>* m_pActions;
	bool m_bPaused;
};


//////////////////////////////////////////////////////////////////////////
static CCActionManager* gSharedManager = NULL;
CCActionManager* CCActionManager::sharedManager()
{
	if (NULL != gSharedManager)
		return gSharedManager;

	gSharedManager = new CCActionManager();
	if (NULL != gSharedManager && gSharedManager->init())
		return gSharedManager;
	
	CC_SAFE_DELETE(gSharedManager);
	return NULL;
}

void CCActionManager::purgeSharedManager()
{
	CCScheduler::sharedScheduler()->unscheduleUpdateForTarget(this);
	CC_SAFE_RELEASE(gSharedManager);
}

CCActionManager::CCActionManager()
	: m_pActionHandlers(NULL)
{
	CCAssert(NULL == gSharedManager, "");
}

CCActionManager::~CCActionManager()
{
	removeAllActions();
	CC_SAFE_DELETE(m_pActionHandlers);
	gSharedManager = NULL;
}

bool CCActionManager::init()
{
	CCScheduler::sharedScheduler()->scheduleUpdateForTarget(this, 0, false);
	CCAssert(NULL == m_pActionHandlers, "handlers already init");
	m_pActionHandlers = new CCMutableArray<CCActionHandler*>;
	return true;
}

void CCActionManager::addAction(CCAction* pAction, CCNode* pTarget, bool paused)
{
	CCAssert(NULL != pTarget && NULL != pAction, "");
	if (NULL == pTarget || NULL == pAction)
		return;

	CCAssert(NULL == pAction->getOriginalTarget(), "action has already target");
	if (NULL != pAction->getOriginalTarget())
		return;

	CCMutableArray<CCActionHandler*>::CCMutableArrayIterator iter;
	for (iter = m_pActionHandlers->begin(); iter != m_pActionHandlers->end(); ++iter)
	{
		CCActionHandler* handler = *iter;
		if (NULL != handler && NULL != handler->getActions() && handler->getTarget() == pTarget)
		{
			if (!handler->getActions()->containsObject(pAction))
			{
				handler->setPaused(paused);
				handler->getActions()->addObject(pAction);
			}
			else
			{
				CCAssert(false, "already have this action, need remove first then add again");
			}
			return;
		}
	}

	pAction->setOriginalTarget(pTarget);
	CCActionHandler* pHandler = CCActionHandler::handlerWithTarget(pTarget);
	if (NULL == pHandler || NULL == pHandler->getActions())
		return;

	pHandler->setPaused(paused);
	pHandler->getActions()->addObject(pAction);
	m_pActionHandlers->addObject(pHandler);
}

void CCActionManager::removeAllActionsFromTarget(CCObject* pTarget)
{
	CCMutableArray<CCActionHandler*>::CCMutableArrayIterator iter;
	for (iter = m_pActionHandlers->begin(); iter != m_pActionHandlers->end(); ++iter)
	{
		CCActionHandler* pHandler = *iter;
		if (NULL != pHandler && pHandler->getTarget() == pTarget)
		{
			m_pActionHandlers->removeObject(pHandler);
			break;
		}
	}
}

void CCActionManager::removeAllActions()
{
	CCMutableArray<CCActionHandler*>::CCMutableArrayIterator iter;
	for (iter = m_pActionHandlers->begin(); iter != m_pActionHandlers->end(); ++iter)
	{
		CCActionHandler* pHandler = *iter;
		if (NULL != pHandler)
			removeAllActionsFromTarget(pHandler->getTarget());
	}
}

void CCActionManager::update(ccTime dt)
{
	CCMutableArray<CCActionHandler*>::CCMutableArrayIterator iter;
	for (iter = m_pActionHandlers->begin(); iter != m_pActionHandlers->end();)
	{
		CCActionHandler* pHandler = *iter;
		if (NULL == pHandler || pHandler->isPaused() || NULL == pHandler->getActions())
			continue;

		CCMutableArray<CCAction*>::CCMutableArrayIterator iter_action;
		for (iter_action = pHandler->getActions()->begin(); iter_action != pHandler->getActions()->end();)
		{
			CCAction* pAction = *iter_action;
			if (NULL == pAction)
				continue;

			pAction->step(dt);

			++iter_action;
			if (pAction->isDone())
			{
				pAction->stop();
				pHandler->getActions()->removeObject(pAction);
			}
		}

		++iter;
		if (pHandler->getActions()->count() == 0)
		{
			m_pActionHandlers->removeObject(pHandler);
		}
	}
}

NS_CC_END;