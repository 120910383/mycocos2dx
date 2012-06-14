#pragma once

#include "CCNode.h"
#include "CCTouchDelegateProtocol.h"

NS_CC_BEGIN;

class CC_DLL CCLayer : public CCNode, public CCTouchDelegate
{
public:
	CCLayer();
	virtual ~CCLayer();
	bool init();
	static CCLayer* node();

	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

	virtual void registerWithTouchDispatcher();

	virtual bool getIsTouchEnabled();
	virtual void setIsTouchEnabled(bool enabled);

protected:
	bool m_bIsTouchEnabled;

};

NS_CC_END;