#pragma once

#include "CCLayer.h"

NS_CC_BEGIN;

typedef enum
{
	kCCMenuStateWaiting,
	kCCMenuStateTrackingTouch,
} tCCMenuState;

enum
{
	kCCMenuTouchPriority = -128,
};

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

	virtual void addChild(CCNode* child);
	virtual void addChild(CCNode* child, int zOrder);
	virtual void addChild(CCNode* child, int zOrder, int tag);
	virtual void registerWithTouchDispatcher();

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

	virtual void onExit();

protected:
	CCMenuItem* itemForTouch(CCTouch* touch);

protected:
	CCMenuItem* m_pSelectedItem;
	tCCMenuState m_eState;
};

NS_CC_END;