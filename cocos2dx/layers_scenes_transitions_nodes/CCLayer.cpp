#include "CCLayer.h"
#include "CCPointExtension.h"
#include "CCTouchDispatcher.h"
#include "CCDirector.h"

NS_CC_BEGIN;

CCLayer::CCLayer()
	: m_bIsTouchEnabled(false)
{
	m_bIsRelativeAnchorPoint = false;
	setAnchorPoint(ccp(0.5f, 0.5f));
}

CCLayer::~CCLayer()
{

}

bool CCLayer::init()
{
	bool bRet = false;
	do 
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		CC_BREAK_IF(NULL == pDirector);
		this->setContentSize(pDirector->getWinSize());
		m_bIsTouchEnabled = false;
		bRet = true;
	} while (0);

	return bRet;
}

CCLayer* CCLayer::node()
{
	CCLayer* pRet = new CCLayer();
	if (NULL != pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void CCLayer::registerWithTouchDispatcher()
{
	// override me
	CCTouchDispatcher::sharedDispatcher()->addDelegate(this, -1, true);
}

bool CCLayer::getIsTouchEnabled()
{
	return m_bIsTouchEnabled;
}

void CCLayer::setIsTouchEnabled(bool enabled)
{
	if (m_bIsTouchEnabled != enabled)
	{
		m_bIsTouchEnabled = enabled;
		// 如果当前层正在运行，则根据enabled来决定注册或是取消CCTouchDispatcher的消息分发
		// 如果当前层未运行，则此处不做处理，只改变成员变量，待开始运行时在onEnter中来注册CCTouchDispatcher的消息分发
		if (m_bIsRunning)
		{
			if (enabled)
			{
				this->registerWithTouchDispatcher();
			}
			else
			{
				CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
			}
		}
	}
}

void CCLayer::onEnter()
{
	if (m_bIsTouchEnabled)
	{
		this->registerWithTouchDispatcher();
	}

	CCNode::onEnter();
}

void CCLayer::onExit()
{
	if (m_bIsTouchEnabled)
	{
		CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	}

	CCNode::onExit();
}

bool CCLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pTouch);
	CC_UNUSED_PARAM(pEvent);
	CCAssert(false, "ccTouchBegan override me");
	return true;
}

void CCLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pTouch);
	CC_UNUSED_PARAM(pEvent);
}

void CCLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pTouch);
	CC_UNUSED_PARAM(pEvent);
}

NS_CC_END;