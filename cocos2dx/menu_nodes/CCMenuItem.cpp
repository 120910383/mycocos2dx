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

NS_CC_END;