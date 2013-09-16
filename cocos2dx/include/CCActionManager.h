#pragma once

#include "CCObject.h"
#include "CCMutableArray.h"

NS_CC_BEGIN;

class CCAction;
class CCNode;
class CCActionHandler;
class CC_DLL CCActionManager : public CCObject
{
public:
	static CCActionManager* sharedManager();
	CCActionManager();
	virtual ~CCActionManager();

public:
	bool init();

	void addAction(CCAction* pAction, CCNode* pTarget, bool paused);

	void removeAction(CCAction* pAction);
	void removeActionByTag(unsigned int tag, CCObject* pTarget);
	void removeAllActionsFromTarget(CCObject* pTarget);
	void removeAllActions();

	CCAction* getActionByTag(unsigned int tag, CCObject* pTarget);
	unsigned int numberOfRunningActionsInTarget(CCObject* pTarget);

	void pauseTarget(CCObject* pTarget);
	void resumeTarget(CCObject* pTarget);

	void purgeSharedManager();

protected:
	virtual void update(ccTime dt);

protected:
	CCMutableArray<CCActionHandler*>* m_pActionHandlers;
};

NS_CC_END;