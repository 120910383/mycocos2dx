#include "CCMenuItem.h"
#include "CCPointExtension.h"
#include "CCLabelTTF.h"

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
	CCMenuItemLabel* item = new CCMenuItemLabel();
	if (NULL != item && item->initWithLabel(label, target, selector))
	{
		item->autorelease();
		return item;
	}

	CC_SAFE_DELETE(item);
	return NULL;
}

CCMenuItemLabel::CCMenuItemLabel()
	: m_pLabel(NULL)
{

}

CCMenuItemLabel::~CCMenuItemLabel()
{

}

bool CCMenuItemLabel::initWithLabel(CCNode* label, CCObject* target, SEL_MenuHandler selector)
{
	initWithTarget(target, selector);
	m_tColorBackup = ccWHITE;
	m_tDisabledColor = ccc3(126, 126, 126);
	setLabel(label);
	return true;
}

void CCMenuItemLabel::setString(const char* label)
{
	CCLabelTTF* label_ttf = dynamic_cast<CCLabelTTF*>(m_pLabel);
	if (NULL != label_ttf)
	{
		label_ttf->setString(label);
		setContentSize(label_ttf->getContentSize());
	}
	else
	{
		CCAssert(false, "m_pLabel is not a CCLabelTTF ?");
	}
}

void CCMenuItemLabel::setLabel(CCNode* label)
{
	if (NULL != label)
	{
		addChild(label);
		label->setAnchorPoint(ccp(0, 0));
		setContentSize(label->getContentSize());
	}

	if (NULL != m_pLabel)
	{
		removeChild(m_pLabel, true);
	}

	m_pLabel = label;
}

CCNode* CCMenuItemLabel::getLabel()
{
	return m_pLabel;
}

void CCMenuItemLabel::setDisabledColor(const ccColor3B& disabled_color)
{
	m_tDisabledColor = disabled_color;
}

const ccColor3B& CCMenuItemLabel::getDisabledColor()
{
	return m_tDisabledColor;
}

void CCMenuItemLabel::setIsEnabled(bool enabled)
{
	if (m_bIsEnabled != enabled)
	{
		if (!enabled)
		{
			CCLabelTTF* label_ttf = dynamic_cast<CCLabelTTF*>(m_pLabel);
			if (NULL != label_ttf)
			{
				m_tColorBackup = label_ttf->getColor();
				label_ttf->setColor(m_tDisabledColor);
			}
		}
		else
		{
			CCLabelTTF* label_ttf = dynamic_cast<CCLabelTTF*>(m_pLabel);
			if (NULL != label_ttf)
			{
				label_ttf->setColor(m_tColorBackup);
			}
		}
	}
	CCMenuItem::setIsEnabled(enabled);
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
	CCMenuItemSprite* pRet = new CCMenuItemSprite();
	if (NULL != pRet && pRet->initFromNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector))
	{
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
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
	CCAssert(NULL != normalSprite, "");
	if (NULL != normalSprite)
	{
		initWithTarget(target, selector);

		setNormalImage(normalSprite);
		setSelectedImage(selectedSprite);
		setDisabledImage(disabledSprite);

		setContentSize(normalSprite->getContentSize());

		return true;
	}

	return false;
}

void CCMenuItemSprite::setNormalImage(CCNode* normalSprite)
{
	if (NULL != normalSprite)
	{
		addChild(normalSprite);
		normalSprite->setAnchorPoint(ccp(0, 0));
		normalSprite->setIsVisible(true);
	}

	if (NULL != m_pNormalImage)
	{
		removeChild(m_pNormalImage, true);
	}

	m_pNormalImage = normalSprite;
}

CCNode* CCMenuItemSprite::getNormalImage()
{
	return m_pNormalImage;
}

void CCMenuItemSprite::setSelectedImage(CCNode* selectedSprite)
{
	if (NULL != selectedSprite)
	{
		addChild(selectedSprite);
		selectedSprite->setAnchorPoint(ccp(0, 0));
		selectedSprite->setIsVisible(false);
	}

	if (NULL != m_pSelectedImage)
	{
		removeChild(m_pSelectedImage, true);
	}

	m_pSelectedImage = selectedSprite;
}

CCNode* CCMenuItemSprite::getSelectedImage()
{
	return m_pSelectedImage;
}

void CCMenuItemSprite::setDisabledImage(CCNode* disabledSprite)
{
	if (NULL != disabledSprite)
	{
		addChild(disabledSprite);
		disabledSprite->setAnchorPoint(ccp(0, 0));
		disabledSprite->setIsVisible(false);
	}

	if (NULL != m_pDisabledImage)
	{
		removeChild(m_pDisabledImage, true);
	}

	m_pDisabledImage = disabledSprite;
}

CCNode* CCMenuItemSprite::getDisabledImage()
{
	return m_pDisabledImage;
}

NS_CC_END;