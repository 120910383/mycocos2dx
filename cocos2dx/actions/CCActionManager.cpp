#include "CCActionManager.h"
#include "CCScheduler.h"

NS_CC_BEGIN;

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
	: m_pTargets(NULL)
{
	CCAssert(NULL == gSharedManager, "");
}

CCActionManager::~CCActionManager()
{
	removeAllActions();
	gSharedManager = NULL;
}

bool CCActionManager::init()
{
	CCScheduler::sharedScheduler()->scheduleUpdateForTarget(this, 0, false);
	m_pTargets = NULL;
	return true;
}

void CCActionManager::removeAllActions()
{
	// TODO...
}

void CCActionManager::update(ccTime dt)
{
	// TODO...
}

NS_CC_END;