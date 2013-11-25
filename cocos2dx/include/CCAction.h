#pragma once

#include "CCObject.h"

NS_CC_BEGIN;

enum
{
	kCCActionTagInvalid = -1,
};

//////////////////////////////////////////////////////////////////////////
/// CCAction
class CCNode;
class CC_DLL CCAction : public CCObject
{
public:
	static CCAction* action();
	CCAction();
	virtual ~CCAction();

public:
	inline CCNode* getTarget() { return m_pTarget; }
	inline void setTarget(CCNode* pTarget) { m_pTarget = pTarget; }

	inline CCNode* getOriginalTarget() { return m_pOriginalTarget; }
	inline void setOriginalTarget(CCNode *pOriginalTarget) { m_pOriginalTarget = pOriginalTarget; }

	inline int getTag() { return m_nTag; }
	inline void setTag(int nTag) { m_nTag = nTag; }

public:
	virtual bool isDone();
	virtual void startWithTarget(CCNode* pTarget);
	virtual void stop();
	virtual void step(ccTime dt);
	virtual void update(ccTime dt);

protected:
	CCNode* m_pOriginalTarget;
	CCNode* m_pTarget;
	int m_nTag;
};

//////////////////////////////////////////////////////////////////////////
/// CCFiniteTimeAction
class CC_DLL CCFiniteTimeAction : public CCAction
{
public:
	CCFiniteTimeAction()
		: m_fDuration(0)
	{}
	virtual ~CCFiniteTimeAction() {}
	inline ccTime getDuration() { return m_fDuration; }
	inline void setDuration(ccTime duration) { m_fDuration = duration; }
	virtual CCFiniteTimeAction* reverse();

protected:
	ccTime m_fDuration;
};

NS_CC_END;