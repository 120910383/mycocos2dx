#include "CCMenuItem.h"

NS_CC_BEGIN;

CCMenuItem::CCMenuItem()
	: m_bIsSelected(false)
{

}

CCMenuItem::~CCMenuItem()
{

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

NS_CC_END;