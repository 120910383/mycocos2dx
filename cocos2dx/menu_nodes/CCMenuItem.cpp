#include "CCMenuItem.h"
#include "CCPointExtension.h"

NS_CC_BEGIN;

CCMenuItem* CCMenuItem::itemWithTarget(CCObject* rec, SEL_MenuHandler selector)
{
	CCMenuItem* item = new CCMenuItem();
	if (NULL != item && item->initWithTarget(rec, selector))
	{
		item->autorelease();
		return item;
	}

	CC_SAFE_DELETE(item);
	return NULL;
}

CCMenuItem::CCMenuItem()
	: m_bIsSelected(false)
	, m_bIsEnabled(true)
	, m_pListener(NULL)
	, m_pfnSelector(NULL)
{

}

CCMenuItem::~CCMenuItem()
{

}

bool CCMenuItem::initWithTarget(CCObject* rec, SEL_MenuHandler selector)
{
	setAnchorPoint(ccp(0.5f, 0.5f));
	setTarget(rec, selector);
	m_bIsSelected = false;
	m_bIsEnabled = true;
	return true;
}

void CCMenuItem::activate()
{
	if (m_bIsEnabled)
	{
		if (NULL != m_pListener && NULL != m_pfnSelector)
		{
			(m_pListener->*m_pfnSelector)(this);
		}
	}
}

void CCMenuItem::selected()
{
	m_bIsSelected = true;
}

void CCMenuItem::unselected()
{
	m_bIsSelected = false;
}

bool CCMenuItem::getIsSelected()
{
	return m_bIsSelected;
}

void CCMenuItem::setIsEnabled(bool enabled)
{
	m_bIsEnabled = enabled;
}

bool CCMenuItem::getIsEnabled()
{
	return m_bIsEnabled;
}

void CCMenuItem::setTarget(CCObject* rec, SEL_MenuHandler selector)
{
	m_pListener = rec;
	m_pfnSelector = selector;
}


//////////////////////////////////////////////////////////////////////////
// 字符串菜单，具有点击执行自动放大缩小动画的特性
// TODO...
CCMenuItemLabel* CCMenuItemLabel::itemWithLabel(CCNode* label)
{
	return itemWithLabel(label, NULL, NULL);
}
CCMenuItemLabel* CCMenuItemLabel::itemWithLabel(CCNode* label, CCObject* target, SEL_MenuHandler selector)
{
	//TODO...
	return NULL;
}

CCMenuItemLabel::CCMenuItemLabel()
	: m_pLabel(NULL)
{
	//TODO...
}

CCMenuItemLabel::~CCMenuItemLabel()
{
	//TODO...
}

bool CCMenuItemLabel::initWithLabel(CCNode* label, CCObject* target, SEL_MenuHandler selector)
{
	//TODO...
	return false;
}

void CCMenuItemLabel::setString(const char* label)
{
	//TODO...
}

void CCMenuItemLabel::setLabel(CCNode* label)
{
	//TODO...
}

CCNode* CCMenuItemLabel::getLabel()
{
	return m_pLabel;
}


//////////////////////////////////////////////////////////////////////////
// 精灵菜单，可呈现三种状态精灵(正常，选中，非使能)
// TODO...
CCMenuItemSprite* CCMenuItemSprite::itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite)
{
	return itemFromNormalSprite(normalSprite, selectedSprite, disabledSprite, NULL, NULL);
}

CCMenuItemSprite* CCMenuItemSprite::itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector)
{
	return itemFromNormalSprite(normalSprite, selectedSprite, NULL, target, selector);
}

CCMenuItemSprite* CCMenuItemSprite::itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector)
{
	//TODO...
	return NULL;
}

CCMenuItemSprite::CCMenuItemSprite()
	: m_pNormalImage(NULL)
	, m_pSelectedImage(NULL)
	, m_pDisabledImage(NULL)
{

}

CCMenuItemSprite::~CCMenuItemSprite()
{

}

bool CCMenuItemSprite::initFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector)
{
	//TODO...
	return false;
}

void CCMenuItemSprite::setNormalImage(CCNode* normalSprite)
{
	//TODO...
}

CCNode* CCMenuItemSprite::getNormalImage()
{
	return m_pNormalImage;
}

void CCMenuItemSprite::setSelectedImage(CCNode* selectedSprite)
{
	//TODO...
}

CCNode* CCMenuItemSprite::getSelectedImage()
{
	return m_pSelectedImage;
}

void CCMenuItemSprite::setDisabledImage(CCNode* disabledSprite)
{
	//TODO...
}

CCNode* CCMenuItemSprite::getDisabledImage()
{
	return m_pDisabledImage;
}

NS_CC_END;