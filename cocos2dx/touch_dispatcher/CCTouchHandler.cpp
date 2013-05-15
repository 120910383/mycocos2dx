#include "CCTouchHandler.h"
#include "CCTouchDelegateProtocol.h"
#include "CCTouch.h"

NS_CC_BEGIN;

CCTouchHandler::CCTouchHandler()
	: m_pDelegate(NULL)
	, m_pClaimedTouches(NULL)
	, m_nPriority(0)
{
	m_pClaimedTouches = new CCMutableArray<CCTouch*>;
}

CCTouchHandler::~CCTouchHandler(void)
{
	CC_SAFE_DELETE(m_pClaimedTouches);
	CCObject* old_delegate = dynamic_cast<CCObject*>(m_pDelegate);
	CC_SAFE_RELEASE(old_delegate);
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
	CCAssert(NULL != new_delegate, "touch delegate should be CCObject and not be null");
	CC_SAFE_RETAIN(new_delegate);

	CCObject* old_delegate = dynamic_cast<CCObject*>(m_pDelegate);
	CC_SAFE_RELEASE(old_delegate);

	m_pDelegate = pDelegate;
	m_nPriority = nPriority;

	// 单点触控用不到,cocos2d-x-learn
	CC_UNUSED_PARAM(bSwallow);

	return true;
}

NS_CC_END;