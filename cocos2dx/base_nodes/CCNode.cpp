﻿#include "CCNode.h"
#include "gles/gl.h"

NS_CC_BEGIN;

CCNode::CCNode()
	: m_nZOrder(0)
	, m_fVertexZ(0.0f)
	, m_fRotation(0.0f)
	, m_fScaleX(1.0f)
	, m_fScaleY(1.0f)
	, m_fSkewX(0.0)
	, m_fSkewY(0.0)
	, m_tPosition(CCPointZero)
	, m_tAnchorPoint(CCPointZero)
	, m_tContentSize(CCSizeZero)
	, m_bIsRunning(false)
	, m_bIsVisible(true)
	, m_bIsRelativeAnchorPoint(true)
	, m_nTag(kCCNodeTagInvalid)
	, m_pUserData(NULL)
	, m_pParent(NULL)
	, m_pChildren(NULL)
{

}

CCNode::~CCNode()
{
	if (NULL != m_pChildren && m_pChildren->count() > 0)
	{
		CCMutableArray<CCNode*>::CCMutableArrayIterator iter;
		for (iter = m_pChildren->begin(); iter != m_pChildren->end(); ++iter)
		{
			CCNode* pChild = *iter;
			if (NULL != pChild)
			{
				pChild->m_pParent = NULL;
			}
		}
	}

	CC_SAFE_DELETE(m_pChildren);
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

void CCNode::setPosition(const CCPoint& newPosition)
{
	m_tPosition = newPosition;
}

const CCPoint& CCNode::getAnchorPoint()
{
	return m_tAnchorPoint;
}

void CCNode::setAnchorPoint(const CCPoint& point)
{
	m_tAnchorPoint = point;
}

const CCSize& CCNode::getContentSize()
{
	return m_tContentSize;
}

void CCNode::setContentSize(const CCSize& size)
{
	m_tContentSize = size;
}

float CCNode::getVertexZ()
{
	return m_fVertexZ;
}

void CCNode::setVertexZ(float var)
{
	m_fVertexZ = var;
}

float CCNode::getRotation()
{
	return m_fRotation;
}

void CCNode::setRotation(float var)
{
	m_fRotation = var;
}

float CCNode::getScale()
{
	CCAssert(m_fScaleX == m_fScaleY, "CCNode#scale. ScaleX != ScaleY. Don't know which one to return");
	return m_fScaleX;
}

void CCNode::setScale(float scale)
{
	m_fScaleX = m_fScaleY = scale;
}

float CCNode::getScaleX()
{
	return m_fScaleX;
}

void CCNode::setScaleX(float var)
{
	m_fScaleX = var;
}

float CCNode::getScaleY()
{
	return m_fScaleY;
}

void CCNode::setScaleY(float var)
{
	m_fScaleY = var;
}

float CCNode::getSkewX()
{
	return m_fSkewX;
}

void CCNode::setSkewX(float var)
{
	m_fSkewX = var;
}

float CCNode::getSkewY()
{
	return m_fSkewY;
}

void CCNode::setSkewY(float var)
{
	m_fSkewY = var;
}

bool CCNode::getIsVisible()
{
	return m_bIsVisible;
}

void CCNode::setIsVisible(bool var)
{
	m_bIsVisible = var;
}

bool CCNode::getIsRelativeAnchorPoint()
{
	return m_bIsRelativeAnchorPoint;
}

void CCNode::setIsRelativeAnchorPoint(bool var)
{
	m_bIsRelativeAnchorPoint = var;
}

int CCNode::getTag()
{
	return m_nTag;
}

void CCNode::setTag(int var)
{
	m_nTag = var;
}

void* CCNode::getUserData()
{
	return m_pUserData;
}

void CCNode::setUserData(void* var)
{
	m_pUserData = var;
}

CCNode* CCNode::getParent()
{
	return m_pParent;
}

void CCNode::setParent(CCNode* var)
{
	m_pParent = var;
}

bool CCNode::getIsRunning()
{
	return m_bIsRunning;
}

int CCNode::getZOrder()
{
	return m_nZOrder;
}

void CCNode::setZOrder(int z)
{
	m_nZOrder = z;
}

CCMutableArray<CCNode*>* CCNode::getChildren()
{
	return m_pChildren;
}

void CCNode::onEnter()
{
	arrayMakeObjectsPerformSelector(m_pChildren, &CCNode::onEnter);
	m_bIsRunning = true;
}

void CCNode::onExit()
{
	m_bIsRunning = false;
	arrayMakeObjectsPerformSelector(m_pChildren, &CCNode::onExit);
}

void CCNode::draw(void)
{
	// override me
}

void CCNode::visit(void)
{
	if (!m_bIsVisible)
	{
		return;
	}

	glPushMatrix();

	CCMutableArray<CCNode*>::CCMutableArrayIterator iter;
	// 绘制zOrder<0的child
	if (NULL != m_pChildren && m_pChildren->count() > 0)
	{
		iter = m_pChildren->begin();
		for ( ; iter != m_pChildren->end(); ++iter)
		{
			CCNode* pNode = *iter;
			if (NULL != pNode && pNode->m_nZOrder < 0)
			{
				pNode->visit();
			}
			else
			{
				break;
			}
		}
	}

	// 绘制自身
	this->draw();

	// 绘制zOrder>0的child
	if (NULL != m_pChildren && m_pChildren->count() > 0)
	{
		for ( ; iter != m_pChildren->end(); ++iter)
		{
			CCNode* pNode = *iter;
			if (NULL != pNode)
			{
				pNode->visit();
			}
		}
	}

	glPopMatrix();
}

void CCNode::addChild(CCNode* child)
{
	CCAssert(child != NULL, "Argument must be non-nil");
	this->addChild(child, child->m_nZOrder, child->m_nTag);
}

void CCNode::addChild(CCNode* child, int zOrder)
{
	CCAssert(child != NULL, "Argument must be non-nil");
	this->addChild(child, zOrder, child->m_nTag);
}

void CCNode::addChild(CCNode* child, int zOrder, int tag)
{
	CCAssert( child != NULL, "Argument must be non-nil");
	CCAssert( child->m_pParent == NULL, "child already added. It can't be added again");

	if (NULL == m_pChildren)
	{
		this->childrenAlloc();
	}

	this->insertChild(child, zOrder);
	child->m_nTag = tag;
	child->setParent(this);

	if (m_bIsRunning)
	{
		child->onEnter();
	}
}

void CCNode::reorderChild(CCNode* child, int zOrder)
{
	CCAssert(NULL != child, "Child must be non-nil");

	child->retain();
	m_pChildren->removeObject(child);
	insertChild(child, zOrder);
	child->release();
}

CCNode* CCNode::getChildByTag(int tag)
{
	CCAssert(tag != kCCNodeTagInvalid, "Invalid tag");

	if (NULL != m_pChildren && m_pChildren->count() > 0)
	{
		CCMutableArray<CCNode*>::CCMutableArrayIterator iter;
		for (iter = m_pChildren->begin(); iter != m_pChildren->end(); ++iter)
		{
			CCNode* pChild = *iter;
			if (NULL != pChild && pChild->m_nTag == tag)
				return pChild;
		}
	}
	return NULL;
}

unsigned int CCNode::getChildrenCount()
{
	return m_pChildren ? m_pChildren->count() : 0;
}

void CCNode::removeFromParentAndCleanup(bool cleanup)
{
	this->m_pParent->removeChild(this, cleanup);
}

void CCNode::removeChildByTag(int tag, bool cleanup)
{
	CCAssert(tag != kCCNodeTagInvalid, "Invalid tag");

	CCNode* child = this->getChildByTag(tag);
	if (NULL == child)
	{
		//CCLOG("cocos2d: removeChildByTag: child not found!");
	}
	else
	{
		this->removeChild(child, cleanup);
	}
}

void CCNode::removeChild(CCNode* child, bool cleanup)
{
	if (NULL != m_pChildren && m_pChildren->containsObject(child))
		this->detachChild(child, cleanup);
}

void CCNode::removeAllChildrenWithCleanup(bool cleanup)
{
	// 为了使用removeAllObjects代替removeObjects循环来提高效率，因此不使用detachChild
	if (NULL != m_pChildren && m_pChildren->count() > 0)
	{
		CCMutableArray<CCNode*>::CCMutableArrayIterator iter;
		for (iter = m_pChildren->begin(); iter != m_pChildren->end(); ++iter)
		{
			CCNode* pChild = *iter;
			if (NULL != pChild)
			{
				if (m_bIsRunning)
				{
					pChild->onExit();
				}

				if (cleanup)
				{
					pChild->cleanup();
				}

				pChild->setParent(NULL);
			}
		}
		m_pChildren->removeAllObjects();
	}
}

void CCNode::cleanup()
{
	// 停止所有本节点的动作和定时器
	// TODO...

	// 递归执行子节点
	arrayMakeObjectsPerformSelector(m_pChildren, &CCNode::cleanup);
}

void CCNode::childrenAlloc()
{
	m_pChildren = new CCMutableArray<CCNode*>(4);
}

void CCNode::insertChild(CCNode* child, int z)
{
	unsigned int index = 0;
	CCNode* a = m_pChildren->getLastObject();
	if (NULL == a || a->getZOrder() <= z)
	{
		m_pChildren->addObject(child);
	}
	else
	{
		CCMutableArray<CCNode*>::CCMutableArrayIterator iter;
		for (iter = m_pChildren->begin(); iter != m_pChildren->end(); ++iter)
		{
			CCNode* pNode = *iter;
			if (NULL != pNode && pNode->m_nZOrder > z)
			{
				m_pChildren->insertObjectAtIndex(child, index);
				break;
			}
			index++;
		}
	}

	child->setZOrder(z);
}

void CCNode::detachChild(CCNode* child, bool doCleanup)
{
	if (m_bIsRunning)
	{
		child->onExit();
	}

	if (doCleanup)
	{
		child->cleanup();
	}

	child->setParent(NULL);
	m_pChildren->removeObject(child);
}

void CCNode::arrayMakeObjectsPerformSelector(CCMutableArray<CCNode*>* pArray, callbackFunc func)
{
	if (NULL != pArray && pArray->count() > 0)
	{
		CCMutableArray<CCNode*>::CCMutableArrayIterator iter;
		for (iter = pArray->begin(); iter != pArray->end(); ++iter)
		{
			CCNode* pNode = *iter;
			if (NULL != pNode && NULL != func)
			{
				(pNode->*func)();
			}
		}
	}
}

NS_CC_END;