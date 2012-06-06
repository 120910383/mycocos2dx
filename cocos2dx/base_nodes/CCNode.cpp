#include "CCNode.h"
#include "gles/gl.h"

NS_CC_BEGIN;

CCNode::CCNode()
	: m_nZOrder(0)
	, m_tPosition(CCPointZero)
	, m_tAnchorPoint(CCPointZero)
	, m_tContentSize(CCSizeZero)
	, m_bIsRunning(false)
{

}

CCNode::~CCNode()
{

}

CCNode* CCNode::node()
{
	CCNode* pRet = new CCNode();
	pRet->autorelease();
	return pRet;
}

const CCPoint& CCNode::getPosition()
{
	return m_tPosition;
}

void CCNode::setPosition(CCPoint& newPosition)
{
	m_tPosition = newPosition;
}

const CCPoint& CCNode::getAnchorPoint()
{
	return m_tAnchorPoint;
}

void CCNode::setAnchorPoint(CCPoint& point)
{
	m_tAnchorPoint = point;
}

const CCSize& CCNode::getContentSize()
{
	return m_tContentSize;
}

void CCNode::setContentSize(CCSize& size)
{
	m_tContentSize = size;
}

bool CCNode::getIsRunning()
{
	return m_bIsRunning;
}

int CCNode::getZOrder()
{
	return m_nZOrder;
}

void CCNode::onEnter()
{
	m_bIsRunning = true;
}

void CCNode::onExit()
{
	m_bIsRunning = false;
}

void CCNode::draw(void)
{
	// override me
}

void CCNode::visit(void)
{
	glPushMatrix();

	this->draw();

	glPopMatrix();
}

NS_CC_END;