#include "CCTouchDispatcher.h"
#include "CCTouchHandler.h"
#include "CCTouchDelegateProtocol.h"
#include "CCTouch.h"

NS_CC_BEGIN;

static CCTouchDispatcher* pSharedDispatcher = NULL;
CCTouchDispatcher* CCTouchDispatcher::sharedDispatcher()
{
	if (pSharedDispatcher == NULL)
	{
		pSharedDispatcher = new CCTouchDispatcher();
		pSharedDispatcher->init();
	}
	return pSharedDispatcher;
}

CCTouchDispatcher::CCTouchDispatcher() 
	: m_pHandlers(NULL)
{

}

CCTouchDispatcher::~CCTouchDispatcher()
{
	CC_SAFE_DELETE(m_pHandlers);
}

bool CCTouchDispatcher::init()
{
	m_bDispatchEvents = true;
	m_pHandlers = new CCMutableArray<CCTouchHandler*>(8);

	return true;
}

bool CCTouchDispatcher::isDispatchEvents()
{
	return m_bDispatchEvents;
}

void CCTouchDispatcher::setDispatchEvents(bool bDispatchEvents)
{
	m_bDispatchEvents = bDispatchEvents;
}

void CCTouchDispatcher::addDelegate(CCTouchDelegate* pDelegate, int nPriority, bool bSwallowsTouches)
{
	CCTouchHandler* pHandler = CCTouchHandler::handlerWithDelegate(pDelegate, nPriority, bSwallowsTouches);
	
	unsigned int u = 0;
	CCMutableArray<CCTouchHandler*>::CCMutableArrayIterator iter;
	for (iter = m_pHandlers->begin(); iter != m_pHandlers->end(); ++iter)
	{
		CCTouchHandler* handler = *iter;
		if (NULL != handler)
		{
			if (handler->getPriority() < pHandler->getPriority())
			{
				++u;
			}

			if (handler->getDelegate() == pHandler->getDelegate())
			{
				CCAssert(false, "the delegate is already added");
				return;
			}
		}
	}

	m_pHandlers->insertObjectAtIndex(pHandler, u);
}

void CCTouchDispatcher::removeDelegate(CCTouchDelegate* pDelegate)
{
	if (NULL == pDelegate)
		return;

	CCTouchHandler* pHandler;
	CCMutableArray<CCTouchHandler*>::CCMutableArrayIterator iter;
	for (iter = m_pHandlers->begin(); iter != m_pHandlers->end(); ++iter)
	{
		pHandler = *iter;
		if (NULL != pHandler && pHandler->getDelegate() == pDelegate)
		{
			m_pHandlers->removeObject(pHandler);
			break;
		}
	}
}

void CCTouchDispatcher::removeAllDelegates()
{
	m_pHandlers->removeAllObjects();
}

void CCTouchDispatcher::touch(CCTouch* pTouch, CCEvent* pEvent, unsigned int uIndex)
{
	CCAssert(uIndex >= CCTOUCHBEGAN && uIndex < ccTouchMax, "");

	if (m_pHandlers->count() > 0)
	{
		CCTouchHandler* pHandler;
		CCMutableArray<CCTouchHandler*>::CCMutableArrayIterator arrayIter;
		for (arrayIter = m_pHandlers->begin(); arrayIter != m_pHandlers->end(); ++arrayIter)
		{
			pHandler = *arrayIter;
			if (NULL == pHandler)
			{
				break;
			}

			// mark:在响应节点的ccTouchBegan、ccTouchMoved、ccTouchEnded中不可调用addDelegate、removeDelegate等改变m_pHandlers元素的函数，否则会导致循环错乱
			// 有关这个特性将在以后遇到问题时修正，cocos2d-x使用的是缓存增加元素和缓存删除元素，即本次循环结束后统一增加或删除m_pHandlers元素。
			bool bClaimed = false;
			if (uIndex = CCTOUCHBEGAN)
			{
				bClaimed = pHandler->getDelegate()->ccTouchBegan(pTouch, pEvent);
				if (bClaimed)
				{
					pHandler->getClaimedTouches()->addObject(pTouch);
				}
			}
			else if (pHandler->getClaimedTouches()->containsObject(pTouch))
			{
				bClaimed = true;
				switch (uIndex)
				{
				case CCTOUCHMOVED:
					pHandler->getDelegate()->ccTouchMoved(pTouch, pEvent);
					break;
				case CCTOUCHENDED:
					pHandler->getDelegate()->ccTouchEnded(pTouch, pEvent);
					pHandler->getClaimedTouches()->removeObject(pTouch);
					break;
				default:
					CCAssert(false, "undefed type");
					pHandler->getClaimedTouches()->removeObject(pTouch);
					break;
				}
			}

			// 如果该消息被拦截(上层CCTouchBegan返回true)，则不再投递消息
			if (bClaimed)
			{
				break;
			}
		}
	}
}

void CCTouchDispatcher::toucheBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	if (m_bDispatchEvents)
	{
		this->touch(pTouch, pEvent, CCTOUCHBEGAN);
	}
}

void CCTouchDispatcher::toucheMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	if (m_bDispatchEvents)
	{
		this->touch(pTouch, pEvent, CCTOUCHMOVED);
	}
}

void CCTouchDispatcher::toucheEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	if (m_bDispatchEvents)
	{
		this->touch(pTouch, pEvent, CCTOUCHENDED);
	}
}

NS_CC_END;