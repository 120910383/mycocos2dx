#pragma once

#include "CCObject.h"
#include "CCMutableArray.h"

NS_CC_BEGIN;

class CCTouchDelegate;
class CCTouch;

class CC_DLL CCTouchHandler : public CCObject
{
public:
	CCTouchHandler();
	virtual ~CCTouchHandler();

	int getPriority();
	CCTouchDelegate* getDelegate();
	CCMutableArray<CCTouch*>* getClaimedTouches();

	virtual bool initWithDelegate(CCTouchDelegate* pDelegate, int nPriority, bool bSwallow);

public:
	static CCTouchHandler* handlerWithDelegate(CCTouchDelegate* pDelegate, int nPriority, bool bSwallow);

protected:
	CCTouchDelegate* m_pDelegate;
	int m_nPriority;
	CCMutableArray<CCTouch*>* m_pClaimedTouches;
};

NS_CC_END;