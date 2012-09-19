#pragma once

#include "CCLayer.h"

NS_CC_BEGIN;

class CCMenuItem;
class CC_DLL CCMenu : public CCLayer
{
public:
	static CCMenu* menuWithItem(CCMenuItem* item);

public:
	CCMenu();
	virtual ~CCMenu();
	bool init();
	bool initWithItem(CCMenuItem* item);

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
};

NS_CC_END;