#include "CCMenu.h"
#include "CCMenuItem.h"
#include "CCDirector.h"
#include "CCPointExtension.h"

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
		this->m_bIsRelativeAnchorPoint = false;
		setAnchorPoint(ccp(0.5f, 0.5f));
		setContentSize(s);
		setPosition(ccp(s.width / 2, s.height / 2));

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

bool CCMenu::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pTouch);
	CC_UNUSED_PARAM(pEvent);
	// 如果menu不可见或者menu的递归parent有不可见，则不响应触摸事件
	for (CCNode* c = this; c != NULL; c = c->getParent())
	{
		if (!c->getIsVisible())
		{
			return false;
		}
	}

	// 处理item的响应功能TODO...
	return false;
}

void CCMenu::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pTouch);
	CC_UNUSED_PARAM(pEvent);
	// 处理item的响应功能TODO...
}

NS_CC_END;