#pragma once

#include "CCAction.h"
#include "CCGeometry.h"

NS_CC_BEGIN;

//////////////////////////////////////////////////////////////////////////
/// CCActionInterval
class CC_DLL CCActionInterval : public CCFiniteTimeAction
{
public:
	inline ccTime getElapsed() { return m_elapsed; }
	bool initWithDuration(ccTime dt);
	virtual bool isDone();
	virtual void step(ccTime dt);
	virtual void startWithTarget(CCNode* pTarget);
	virtual CCActionInterval* reverse();

public:
	static CCActionInterval* actionWithDuration(ccTime dt);

protected:
	ccTime m_elapsed;
	bool m_bFirstTick;
};

//////////////////////////////////////////////////////////////////////////
/// CCMoveTo
class CC_DLL CCMoveTo : public CCActionInterval
{
public:
	bool initWithDuration(ccTime duration, const CCPoint& position);
	virtual void startWithTarget(CCNode* pTarget);
	virtual void update(ccTime time);

public:
	static CCMoveTo* actionWithDuration(ccTime duration, const CCPoint& position);

protected:
	CCPoint m_endPosition;
	CCPoint m_startPosition;
	CCPoint m_delta;
};

//////////////////////////////////////////////////////////////////////////
/// CCMoveBy
class CC_DLL CCMoveBy : public CCMoveTo
{
public:
	bool initWithDuration(ccTime duration, const CCPoint& position);
	virtual void startWithTarget(CCNode* pTarget);
	virtual CCActionInterval* reverse();

public:
	static CCMoveBy* actionWithDuration(ccTime duration, const CCPoint& position);
};

//////////////////////////////////////////////////////////////////////////
/// CCScaleTo
class CC_DLL CCScaleTo : public CCActionInterval
{
public:
	bool initWithDuration(ccTime duration, float s);
	bool initWithDuration(ccTime duration, float sx, float sy);

	virtual void startWithTarget(CCNode* pTarget);
	virtual void update(ccTime time);

public:
	static CCScaleTo* actionWithDuration(ccTime duration, float s);
	static CCScaleTo* actionWithDuration(ccTime duration, float sx, float sy);

protected:
	float m_fStartScaleX;
	float m_fStartScaleY;
	float m_fEndScaleX;
	float m_fEndScaleY;
	float m_fDeltaX;
	float m_fDeltaY;
};

//////////////////////////////////////////////////////////////////////////
/// CCScaleBy
class CC_DLL CCScaleBy : public CCScaleTo
{
public:
	virtual void startWithTarget(CCNode* pTarget);
	virtual CCActionInterval* reverse();

public:
	static CCScaleBy* actionWithDuration(ccTime duration, float s);
	static CCScaleBy* actionWithDuration(ccTime duration, float sx, float sy);
};
NS_CC_END;