#pragma once

#include "CCNode.h"

NS_CC_BEGIN;

class CC_DLL CCMenuItem : public CCNode
{
public:
	static CCMenuItem* itemWithTarget(CCObject* rec, SEL_MenuHandler selector);

public:
	CCMenuItem();
	virtual ~CCMenuItem();
	bool initWithTarget(CCObject* rec, SEL_MenuHandler selector);

	virtual void activate();

	virtual void selected();
	virtual void unselected();
	virtual bool getIsSelected();

	void setTarget(CCObject* rec, SEL_MenuHandler selector);

protected:
	bool m_bIsSelected;
	CCObject* m_pListener;
	SEL_MenuHandler m_pfnSelector;
};

NS_CC_END;