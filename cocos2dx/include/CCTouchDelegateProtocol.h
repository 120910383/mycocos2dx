#pragma once

#include "ccMacros.h"

NS_CC_BEGIN;

class CCTouch;
class CCEvent;

class CC_DLL CCTouchDelegate
{
public:
	CCTouchDelegate() {}
	virtual ~CCTouchDelegate() {}

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) { CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent); return false; }

	// optional
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent) { CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent); }
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent) { CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent); }
};

NS_CC_END;