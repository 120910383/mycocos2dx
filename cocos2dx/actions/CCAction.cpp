#include "CCAction.h"

NS_CC_BEGIN;

//////////////////////////////////////////////////////////////////////////
/// CCAction
CCAction::CCAction()
	: m_pOriginalTarget(NULL)
	, m_pTarget(NULL)
	, m_nTag(kCCActionTagInvalid)
{

}

CCAction::~CCAction()
{

}

CCAction* CCAction::action()
{
	CCAction* pRet = new CCAction();
	pRet->autorelease();
	return pRet;
}

bool CCAction::isDone()
{
	return true;
}

void CCAction::startWithTarget(CCNode* pTarget)
{
	m_pOriginalTarget = pTarget;
	m_pTarget = pTarget;
}

void CCAction::stop()
{
	m_pTarget = NULL;
}

void CCAction::step(ccTime dt)
{
	CC_UNUSED_PARAM(dt);
	CCAssert(false, "[Action step]. override me");
}

void CCAction::update(ccTime dt)
{
	CC_UNUSED_PARAM(dt);
	CCAssert(false, "[Action update]. override me");
}

//////////////////////////////////////////////////////////////////////////
// CCFiniteTimeAction
CCFiniteTimeAction* CCFiniteTimeAction::reverse()
{
	CCAssert(false, "override me");
	return NULL;
}

NS_CC_END;