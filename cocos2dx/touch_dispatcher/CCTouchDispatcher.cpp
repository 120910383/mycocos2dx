#include "CCTouchDispatcher.h"
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
{

}

CCTouchDispatcher::~CCTouchDispatcher()
{

}

bool CCTouchDispatcher::init()
{
	m_bDispatchEvents = true;

	// todo...

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

void CCTouchDispatcher::touch(CCTouch* pTouch, CCEvent* pEvent, unsigned int uIndex)
{
	CCAssert(uIndex >= CCTOUCHBEGAN && uIndex < ccTouchMax, "");
	// 事件分配
	// todo...
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