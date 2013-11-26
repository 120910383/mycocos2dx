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

//////////////////////////////////////////////////////////////////////////
/// CCScaleTo
CCScaleTo* CCScaleTo::actionWithDuration(ccTime duration, float s)
{
	CCScaleTo* pScaleTo = new CCScaleTo();
	pScaleTo->initWithDuration(duration, s);
	pScaleTo->autorelease();
	return pScaleTo;
}

CCScaleTo* CCScaleTo::actionWithDuration(ccTime duration, float sx, float sy)
{
	CCScaleTo* pScaleTo = new CCScaleTo();
	pScaleTo->initWithDuration(duration, sx, sy);
	pScaleTo->autorelease();
	return pScaleTo;
}

bool CCScaleTo::initWithDuration(ccTime duration, float s)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_fEndScaleX = s;
		m_fEndScaleY = s;
		return true;
	}
	return false;
}

bool CCScaleTo::initWithDuration(ccTime duration, float sx, float sy)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_fEndScaleX = sx;
		m_fEndScaleY = sy;
		return true;
	}
	return false;
}

void CCScaleTo::startWithTarget(CCNode* pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
	m_fStartScaleX = pTarget->getScaleX();
	m_fStartScaleY = pTarget->getScaleY();
	m_fDeltaX = m_fEndScaleX - m_fStartScaleX;
	m_fDeltaY = m_fEndScaleY - m_fStartScaleY;
}

void CCScaleTo::update(ccTime time)
{
	if (NULL != m_pTarget)
	{
		m_pTarget->setScaleX(m_fStartScaleX + m_fDeltaX * time);
		m_pTarget->setScaleY(m_fStartScaleY + m_fDeltaY * time);
	}
}

//////////////////////////////////////////////////////////////////////////
/// CCScaleBy
CCScaleBy* CCScaleBy::actionWithDuration(ccTime duration, float s)
{
	CCScaleBy* pScaleBy = new CCScaleBy();
	pScaleBy->initWithDuration(duration, s);
	pScaleBy->autorelease();
	return pScaleBy;
}

CCScaleBy* CCScaleBy::actionWithDuration(ccTime duration, float sx, float sy)
{
	CCScaleBy* pScaleBy = new CCScaleBy();
	pScaleBy->initWithDuration(duration, sx, sy);
	pScaleBy->autorelease();
	return pScaleBy;
}

void CCScaleBy::startWithTarget(CCNode* pTarget)
{
	CCScaleTo::startWithTarget(pTarget);
	m_fDeltaX = m_fStartScaleX * m_fEndScaleX - m_fStartScaleX;
	m_fDeltaY = m_fStartScaleY * m_fEndScaleY - m_fStartScaleY;
}

CCActionInterval* CCScaleBy::reverse()
{
	return CCScaleBy::actionWithDuration(m_fDuration, 1 / m_fEndScaleX, 1 / m_fEndScaleY);
}

//////////////////////////////////////////////////////////////////////////
/// CCRotateTo
CCRotateTo* CCRotateTo::actionWithDuration(ccTime duration, float fAngle)
{
	CCRotateTo* pRotateTo = new CCRotateTo();
	pRotateTo->initWithDuration(duration, fAngle);
	pRotateTo->autorelease();
	return pRotateTo;
}

bool CCRotateTo::initWithDuration(ccTime duration, float fAngle)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_fDstAngle = fAngle;
		return true;
	}
	return false;
}

void CCRotateTo::startWithTarget(CCNode* pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
	m_fStartAngle = pTarget->getRotation();
	if (m_fStartAngle > 0)
	{
		m_fStartAngle = fmodf(m_fStartAngle, 360.0f);
	}
	else
	{
		m_fStartAngle = fmodf(m_fStartAngle, -360.0f);
	}

	
	m_fDiffAngle = m_fDstAngle - m_fStartAngle;
	// 此处感觉原版计算有问题，如果传入360则不动，传入720则旋转360，有些奇怪
	// 这里进行修正(一种是最大选择范围-180-180，另一种是按实际角度旋转，这里暂时采用第一种方法)
	//if (m_fDiffAngle > 180)
	//{
	//	m_fDiffAngle -= 360;
	//}

	//if (m_fDiffAngle < -180)
	//{
	//	m_fDiffAngle += 360;
	//}

	if (m_fDiffAngle > 180)
	{
		while (m_fDiffAngle > 180)
		{
			m_fDiffAngle -= 360;
		}
	}
	else if (m_fDiffAngle < -180)
	{
		while (m_fDiffAngle < -180)
		{
			m_fDiffAngle += 360;
		}
	}
}

void CCRotateTo::update(ccTime time)
{
	if (NULL != m_pTarget)
	{
		m_pTarget->setRotation(m_fStartAngle + m_fDiffAngle * time);
	}
}

//////////////////////////////////////////////////////////////////////////
/// CCRotateBy
CCRotateBy* CCRotateBy::actionWithDuration(ccTime duration, float fDeltaAngle)
{
	CCRotateBy* pRotateBy = new CCRotateBy();
	pRotateBy->initWithDuration(duration, fDeltaAngle);
	pRotateBy->autorelease();
	return pRotateBy;
}

bool CCRotateBy::initWithDuration(ccTime duration, float fDeltaAngle)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_fDiffAngle = fDeltaAngle;
		return true;
	}
	return false;
}

void CCRotateBy::startWithTarget(CCNode* pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
	m_fStartAngle = pTarget->getRotation();
}

void CCRotateBy::update(ccTime time)
{
	if (NULL != m_pTarget)
	{
		m_pTarget->setRotation(m_fStartAngle + m_fDiffAngle * time);
	}
}

CCActionInterval* CCRotateBy::reverse()
{
	return CCRotateBy::actionWithDuration(m_fDuration, -m_fDiffAngle);
}

NS_CC_END;