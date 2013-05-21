#pragma once

#include "CCObject.h"
#include "CCMutableArray.h"

NS_CC_BEGIN;

class CCUpdateHandle;
class CC_DLL CCScheduler : public CCObject
{
public:
	static CCScheduler* sharedScheduler();
	static void purgeSharedScheduler();

public:
	virtual ~CCScheduler();

	void tick();
	void scheduleUpdateForTarget(CCObject *pTarget, int nPriority);
	void unscheduleUpdateForTarget(const CCObject *pTarget);

private:
	CCScheduler();
	bool init();

protected:
	CCMutableArray<CCUpdateHandle*> *m_pHandlers;
};

NS_CC_END;