#pragma once

#include "CCObject.h"
#include "CCMutableArray.h"

NS_CC_BEGIN;

enum
{
	CCTOUCHBEGAN,
	CCTOUCHMOVED,
	CCTOUCHENDED,
	ccTouchMax,
};

class CCTouch;
class CCEvent;

class CC_DLL EGLTouchDelegate
{
public:
	virtual void toucheBegan(CCTouch* pTouch, CCEvent* pEvent) = 0;
	virtual void toucheMoved(CCTouch* pTouch, CCEvent* pEvent) = 0;
	virtual void toucheEnded(CCTouch* pTouch, CCEvent* pEvent) = 0;

	virtual ~EGLTouchDelegate() {}
};

class CC_DLL CCTouchDispatcher : public CCObject, public EGLTouchDelegate
{
public:
	CCTouchDispatcher();
	~CCTouchDispatcher();
	bool init();
	bool isDispatchEvents();
	void setDispatchEvents(bool bDispatchEvents);
	
public:
	void touch(CCTouch* pTouch, CCEvent* pEvent, unsigned int uIndex);		//暂时不涉及ios的多点触控功能，替代cocos2d中的touches
	virtual void toucheBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void toucheMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void toucheEnded(CCTouch* pTouch, CCEvent* pEvent);

public:
	static CCTouchDispatcher* sharedDispatcher();

protected:
	bool m_bDispatchEvents;
};

NS_CC_END;