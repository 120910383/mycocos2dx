#include "CCNode.h"
#include "gles/gl.h"

NS_CC_BEGIN;

CCNode::CCNode()
	: m_nZOrder(0)
	, m_tPosition(CCPointZero)
	, m_tAnchorPoint(CCPointZero)
	, m_tContentSize(CCSizeZero)
	, m_bIsRunning(false)
	, m_bIsVisible(true)
	, m_nTag(kCCNodeTagInvalid)
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

bool CCNode::getIsVisible()
{
	return m_bIsVisible;
}

void CCNode::setIsVisible(bool var)
{
	m_bIsVisible = var;
}

int CCNode::getTag()
{
	return m_nTag;
}

void CCNode::setTag(int var)
{
	m_nTag = var;
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