#include "CCActionInterval.h"

#include "CCNode.h"
#include "CCPointExtension.h"

NS_CC_BEGIN;

//////////////////////////////////////////////////////////////////////////
/// CCActionInterval
CCActionInterval* CCActionInterval::actionWithDuration(ccTime dt)
{
	CCActionInterval* pAction = new CCActionInterval();
	pAction->initWithDuration(dt);
	pAction->autorelease();
	return pAction;
}

bool CCActionInterval::initWithDuration(ccTime dt)
{
	m_fDuration = dt;

	// 防止被0除，设置为float类型的最小正数，便于step中统一处理
	if (m_fDuration == 0)
	{
		m_fDuration = FLT_EPSILON;
	}

	m_elapsed = 0;
	m_bFirstTick = true;
	return true;
}

bool CCActionInterval::isDone()
{
	return m_elapsed >= m_fDuration;
}

void CCActionInterval::step(ccTime dt)
{
	// 计时从第一次step开始算起
	if (m_bFirstTick)
	{
		m_bFirstTick = false;
		m_elapsed = 0;
	}
	else
	{
		m_elapsed += dt;
	}

	// 这里的update参数为[0,1]范围，0表示开始，1表示结束
	update(1 > m_elapsed / m_fDuration ? m_elapsed / m_fDuration : 1);
}

void CCActionInterval::startWithTarget(CCNode* pTarget)
{
	CCFiniteTimeAction::startWithTarget(pTarget);
	m_elapsed = 0;
	m_bFirstTick = true;
}

CCActionInterval* CCActionInterval::reverse()
{
	CCAssert(false, "not implement");
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
/// CCMoveTo
CCMoveTo* CCMoveTo::actionWithDuration(ccTime duration, const CCPoint& position)
{
	CCMoveTo* pMoveTo = new CCMoveTo();
	pMoveTo->initWithDuration(duration, position);
	pMoveTo->autorelease();
	return pMoveTo;
}

bool CCMoveTo::initWithDuration(ccTime duration, const CCPoint& position)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_endPosition = position;
		return true;
	}

	return false;
}

void CCMoveTo::startWithTarget(CCNode* pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
	m_startPosition = pTarget->getPosition();
	m_delta = ccpSub(m_endPosition, m_startPosition);
}

void CCMoveTo::update(ccTime time)
{
	if (NULL != m_pTarget)
	{
		m_pTarget->setPosition(ccp(m_startPosition.x + m_delta.x * time, m_startPosition.y + m_delta.y * time));
	}
}

//////////////////////////////////////////////////////////////////////////
/// CCMoveBy
CCMoveBy* CCMoveBy::actionWithDuration(ccTime duration, const CCPoint& position)
{
	CCMoveBy* pMoveBy = new CCMoveBy();
	pMoveBy->initWithDuration(duration, position);
	pMoveBy->autorelease();
	return pMoveBy;
}

bool CCMoveBy::initWithDuration(ccTime duration, const CCPoint& position)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_delta = position;
		return true;
	}

	return false;
}

void CCMoveBy::startWithTarget(CCNode* pTarget)
{
	CCPoint dTmp = m_delta;
	CCMoveTo::startWithTarget(pTarget);
	m_delta = dTmp;
}

CCActionInterval* CCMoveBy::reverse()
{
	return CCMoveBy::actionWithDuration(m_fDuration, ccp(-m_delta.x, -m_delta.y));
}

NS_CC_END;