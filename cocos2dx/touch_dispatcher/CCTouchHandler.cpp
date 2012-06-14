#include "CCTouchHandler.h"
#include "CCTouchDelegateProtocol.h"
#include "CCTouch.h"

NS_CC_BEGIN;

CCTouchHandler::~CCTouchHandler(void)
{
	CC_SAFE_DELETE(m_pClaimedTouches);

	CCObject* old_delegate = dynamic_cast<CCObject*>(m_pDelegate);
	if (NULL != old_delegate)
	{
		old_delegate->release();
	}
}

int CCTouchHandler::getPriority()
{
	return m_nPriority;
}

CCTouchDelegate* CCTouchHandler::getDelegate()
{
	return m_pDelegate;
}

CCMutableArray<CCTouch*>* CCTouchHandler::getClaimedTouches()
{
	return m_pClaimedTouches;
}

CCTouchHandler* CCTouchHandler::handlerWithDelegate(CCTouchDelegate *pDelegate, int nPriority, bool bSwallow)
{
	CCTouchHandler* pHandler = new CCTouchHandler();
	if (NULL != pHandler)
	{
		if (pHandler->initWithDelegate(pDelegate, nPriority, bSwallow))
		{
			pHandler->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(pHandler);
		}
	}
	return pHandler;
}

bool CCTouchHandler::initWithDelegate(CCTouchDelegate* pDelegate, int nPriority, bool bSwallow)
{
	CCObject* new_delegate = dynamic_cast<CCObject*>(pDelegate);
	CCAssert(NULL != new_delegate, "touch delegate should be ccobject and not be null");

	m_pDelegate = pDelegate;
	new_delegate->retain();

	m_nPriority = nPriority;

	m_pClaimedTouches = new CCMutableArray<CCTouch*>;

	// 单点触控用不到,cocos2d-x-learn
	CC_UNUSED_PARAM(bSwallow);

	return true;
}

NS_CC_END;