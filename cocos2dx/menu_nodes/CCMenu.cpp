﻿#include "CCMenu.h"
#include "CCMenuItem.h"
#include "CCDirector.h"
#include "CCPointExtension.h"
#include "CCTouchDispatcher.h"

NS_CC_BEGIN;

CCMenu* CCMenu::menuWithItem(CCMenuItem* item)
{
	CCMenu* menu = new CCMenu();
	if (NULL != menu && menu->initWithItem(item))
	{
		menu->autorelease();
		return menu;
	}

	CC_SAFE_DELETE(menu);
	return NULL;
}

CCMenu::CCMenu()
	: m_pSelectedItem(NULL)
{

}

CCMenu::~CCMenu()
{

}

bool CCMenu::init()
{
	if (CCLayer::init())
	{
		setIsTouchEnabled(true);

		CCSize s = CCDirector::sharedDirector()->getWinSize();
		m_bIsRelativeAnchorPoint = false;
		setAnchorPoint(ccp(0.5f, 0.5f));
		setContentSize(s);
		setPosition(ccp(s.width / 2, s.height / 2));

		m_pSelectedItem = NULL;
		m_eState = kCCMenuStateWaiting;

		return true;
	}
	return false;
}

bool CCMenu::initWithItem(CCMenuItem* item)
{
	if (init())
	{
		int z = 0;
		if (NULL != item)
		{
			this->addChild(item, z);
		}
		
		return true;
	}

	return false;
}

void CCMenu::addChild(CCNode* child)
{
	// 由于重载了addChild(CCNode *child, int zOrder, int tag)导致一个参数的接口被子类屏蔽
	// 这里相当于将接口public出来，没有添加额外功能
	CCLayer::addChild(child);
}

void CCMenu::addChild(CCNode* child, int zOrder)
{
	// 由于重载了addChild(CCNode *child, int zOrder, int tag)导致两个参数的接口被子类屏蔽
	// 这里相当于将接口public出来，没有添加额外功能
	CCLayer::addChild(child, zOrder);
}

void CCMenu::addChild(CCNode *child, int zOrder, int tag)
{
	// CCMenu只能接受CCMenuItem作为其child来进行管理
	CCAssert(NULL != dynamic_cast<CCMenuItem*>(child), "Menu only supports MenuItem objects as children");
	CCLayer::addChild(child, zOrder, tag);
}

void CCMenu::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addDelegate(this, kCCMenuTouchPriority, true);
}

bool CCMenu::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pEvent);
	// 如果menu不可见或者menu的递归parent有不可见，则不响应触摸事件
	for (CCNode* c = this; c != NULL; c = c->getParent())
	{
		if (!c->getIsVisible())
		{
			return false;
		}
	}

	m_pSelectedItem = itemForTouch(pTouch);
	if (NULL != m_pSelectedItem)
	{
		m_eState = kCCMenuStateTrackingTouch;
		m_pSelectedItem->selected();
		return true;
	}

	return false;
}

void CCMenu::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pTouch);
	CC_UNUSED_PARAM(pEvent);
	CCAssert(kCCMenuStateTrackingTouch == m_eState, "[Menu ccTouchEnded] -- invalid state");	//内部状态错误
	if (NULL != m_pSelectedItem)
	{
		m_pSelectedItem->unselected();
		// 通知外界按钮激活事件TODO...
	}
	m_eState = kCCMenuStateWaiting;
}

void CCMenu::onExit()
{
	if (kCCMenuStateTrackingTouch == m_eState)
	{
		if (NULL != m_pSelectedItem)
			m_pSelectedItem->unselected();
		m_eState = kCCMenuStateWaiting;
		m_pSelectedItem = NULL;
	}

	CCLayer::onExit();
}

CCMenuItem* CCMenu::itemForTouch(CCTouch* touch)
{
	CC_UNUSED_PARAM(touch);
	// 根据触摸位置来判断当前哪个item被激活
	// TODO...
	return NULL;
}

NS_CC_END;