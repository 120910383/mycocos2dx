#pragma once

#include "CCObject.h"
#include "CCMutableArray.h"

NS_CC_BEGIN;

class CCUpdateHandle;
class CCSelectorHandle;
class CC_DLL CCScheduler : public CCObject
{
public:
	static CCScheduler* sharedScheduler();
	static void purgeSharedScheduler();

public:
	virtual ~CCScheduler();

	void tick(ccTime dt);
	void scheduleUpdateForTarget(CCObject *pTarget, int nPriority, bool bPaused);
	void unscheduleUpdateForTarget(const CCObject *pTarget);

	void scheduleSelector(SEL_SCHEDULE pfnSelector, CCObject* pTarget, ccTime fInterval, bool bPaused);
	void unscheduleSelector(SEL_SCHEDULE pfnSelector, CCObject* pTarget);

	void pauseTarget(CCObject* pTarget);
	void resumeTarget(CCObject* pTarget);

	void unscheduleAllSelectorsForTarget(CCObject* pTarget);
	void unscheduleAllSelectors();

private:
	CCScheduler();
	bool init();

protected:
	CCMutableArray<CCUpdateHandle*> *m_pUpdateHandlers;
	CCMutableArray<CCSelectorHandle*> *m_pSelectorHandles;
};

NS_CC_END;