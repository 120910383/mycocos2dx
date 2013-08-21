#include "CCNode.h"
#include "CCPointExtension.h"
#include "TransformUtils.h"
#include "CCDirector.h"
#include "CCTouch.h"
#include "CCStdC.h"
#include "CCScheduler.h"

NS_CC_BEGIN;

CCNode::CCNode()
	: m_nZOrder(0)
	, m_tPosition(CCPointZero)
	, m_tAnchorPointInPoints(CCPointZero)
	, m_tAnchorPoint(CCPointZero)
	, m_tContentSize(CCSizeZero)
	, m_fVertexZ(0.0f)
	, m_fRotation(0.0f)
	, m_fScaleX(1.0f)
	, m_fScaleY(1.0f)
	, m_fSkewX(0.0)
	, m_fSkewY(0.0)
	, m_bIsRelativeAnchorPoint(true)
	, m_bIsRunning(false)
	, m_bIsVisible(true)
	, m_nTag(kCCNodeTagInvalid)
	, m_pUserData(NULL)
	, m_pParent(NULL)
	, m_pChildren(NULL)
	, m_bIsTransformDirty(true)
	, m_bIsInverseDirty(true)
	, m_bIsTransformGLDirty(true)
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
	m_bIsTransformDirty = true;
	m_bIsInverseDirty = true;
	m_bIsTransformGLDirty = true;
}

const CCPoint& CCNode::getAnchorPointInPoints()
{
	return m_tAnchorPointInPoints;
}

const CCPoint& CCNode::getAnchorPoint()
{
	return m_tAnchorPoint;
}

void CCNode::setAnchorPoint(const CCPoint& point)
{
	if (!CCPoint::CCPointEqualToPoint(point, m_tAnchorPoint))
	{
		m_tAnchorPoint = point;
		m_tAnchorPointInPoints = ccp(m_tContentSize.width * m_tAnchorPoint.x, m_tContentSize.height * m_tAnchorPoint.y);
		m_bIsTransformDirty = true;
		m_bIsInverseDirty = true;
		m_bIsTransformGLDirty = true;
	}
}

const CCSize& CCNode::getContentSize()
{
	return m_tContentSize;
}

void CCNode::setContentSize(const CCSize& size)
{
	if (!CCSize::CCSizeEqualToSize(size, m_tContentSize))
	{
		m_tContentSize = size;
		m_tAnchorPointInPoints = ccp(m_tContentSize.width * m_tAnchorPoint.x, m_tContentSize.height * m_tAnchorPoint.y);
		m_bIsTransformDirty = true;
		m_bIsInverseDirty = true;
		m_bIsTransformGLDirty = true;
	}
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
	m_bIsTransformDirty = true;
	m_bIsInverseDirty = true;
	m_bIsTransformGLDirty = true;
}

float CCNode::getScale()
{
	CCAssert(m_fScaleX == m_fScaleY, "CCNode#scale. ScaleX != ScaleY. Don't know which one to return");
	return m_fScaleX;
}

void CCNode::setScale(float scale)
{
	m_fScaleX = scale;
	m_fScaleY = scale;
	m_bIsTransformDirty = true;
	m_bIsInverseDirty = true;
	m_bIsTransformGLDirty = true;
}

float CCNode::getScaleX()
{
	return m_fScaleX;
}

void CCNode::setScaleX(float var)
{
	m_fScaleX = var;
	m_bIsTransformDirty = true;
	m_bIsInverseDirty = true;
	m_bIsTransformGLDirty = true;
}

float CCNode::getScaleY()
{
	return m_fScaleY;
}

void CCNode::setScaleY(float var)
{
	m_fScaleY = var;
	m_bIsTransformDirty = true;
	m_bIsInverseDirty = true;
	m_bIsTransformGLDirty = true;
}

float CCNode::getSkewX()
{
	return m_fSkewX;
}

void CCNode::setSkewX(float var)
{
	m_fSkewX = var;
	m_bIsTransformDirty = true;
	m_bIsInverseDirty = true;
	m_bIsTransformGLDirty = true;
}

float CCNode::getSkewY()
{
	return m_fSkewY;
}

void CCNode::setSkewY(float var)
{
	m_fSkewY = var;
	m_bIsTransformDirty = true;
	m_bIsInverseDirty = true;
	m_bIsTransformGLDirty = true;
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
	m_bIsTransformDirty = true;
	m_bIsInverseDirty = true;
	m_bIsTransformGLDirty = true;
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
	resumeScheduler();
	m_bIsRunning = true;
	//TODO... 动作、日程和脚本处理
}

void CCNode::onExit()
{
	//TODO... 动作、日程和脚本处理
	pauseScheduler();
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

	this->transform();

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
	this->unscheduleAllSelectors();

	// 递归执行子节点
	arrayMakeObjectsPerformSelector(m_pChildren, &CCNode::cleanup);
}

CCAffineTransform CCNode::nodeToParentTransform()
{
	if (m_bIsTransformDirty)
	{
		m_tTransform = CCAffineTransformIdentity;
		// 根据position,rotation,skew,scale属性计算仿射矩阵
		if (!m_bIsRelativeAnchorPoint && !CCPoint::CCPointEqualToPoint(m_tAnchorPointInPoints, CCPointZero))
		{
			m_tTransform = CCAffineTransformTranslate(m_tTransform, m_tAnchorPointInPoints.x, m_tAnchorPointInPoints.y);
		}

		if (!CCPoint::CCPointEqualToPoint(m_tPosition, CCPointZero))
		{
			m_tTransform = CCAffineTransformTranslate(m_tTransform, m_tPosition.x, m_tPosition.y);
		}

		if (m_fRotation != 0)
		{
			m_tTransform = CCAffineTransformRotate(m_tTransform, -CC_DEGREES_TO_RADIANS(m_fRotation));
		}

		if (m_fSkewX != 0 || m_fSkewY != 0)
		{
			CCAffineTransform skew = CCAffineTransformMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(m_fSkewY)), tanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1.0f, 0.0f, 0.0f);
			m_tTransform = CCAffineTransformConcat(skew, m_tTransform);
		}

		if (!(m_fScaleX == 1 && m_fScaleY == 1))
		{
			m_tTransform = CCAffineTransformScale(m_tTransform, m_fScaleX, m_fScaleY);
		}

		if (!CCPoint::CCPointEqualToPoint(m_tAnchorPointInPoints, CCPointZero))
		{
			m_tTransform = CCAffineTransformTranslate(m_tTransform, -m_tAnchorPointInPoints.x, -m_tAnchorPointInPoints.y);
		}

		m_bIsTransformDirty = false;
	}

	return m_tTransform;
}

CCAffineTransform CCNode::parentToNodeTransform()
{
	if (m_bIsInverseDirty)
	{
		m_tInverse = CCAffineTransformInvert(this->nodeToParentTransform());
		m_bIsInverseDirty = false;
	}
	return m_tInverse;
}

CCAffineTransform CCNode::nodeToWorldTransform()
{
	CCAffineTransform t = this->nodeToParentTransform();
	for (CCNode* p = m_pParent; p != NULL; p = p->getParent())
	{
		t = CCAffineTransformConcat(t, p->nodeToParentTransform());
	}
	return t;
}

CCAffineTransform CCNode::worldToNodeTransform()
{
	return CCAffineTransformInvert(this->nodeToWorldTransform());
}

CCPoint CCNode::convertToNodeSpace(const CCPoint& worldPoint)
{
	return CCPointApplyAffineTransform(worldPoint, worldToNodeTransform());
}

CCPoint CCNode::convertToWorldSpace(const CCPoint& nodePoint)
{
	return CCPointApplyAffineTransform(nodePoint, nodeToWorldTransform());
}

CCPoint CCNode::convertTouchToNodeSpace(CCTouch* touch)
{
	CCPoint point = touch->locationInView(touch->view());
	point = CCDirector::sharedDirector()->convertToGL(point);
	return convertToNodeSpace(point);
}

void CCNode::transform()
{
	if (m_bIsTransformGLDirty)
	{
		CCAffineTransform t = this->nodeToParentTransform();
		CGAffineToGL(&t, m_pTransformGL);
		m_bIsTransformGLDirty = false;
	}

	glMultMatrixf(m_pTransformGL);
	if (m_fVertexZ)
	{
		glTranslatef(0, 0, m_fVertexZ);
	}

	//TODO...Camera转换，目前暂无Camera设置
}

void CCNode::transformAncestors()
{
	if (NULL != m_pParent)
	{
		m_pParent->transformAncestors();
		m_pParent->transform();
	}
}

CCRect CCNode::boundingBox()
{
	CCRect rect = CCRectMake(0, 0, m_tContentSize.width, m_tContentSize.height);
	return CCRectApplyAffineTransform(rect, nodeToParentTransform());
}

void CCNode::scheduleUpdate()
{
	scheduleUpdateWithPriority(0);
}

void CCNode::scheduleUpdateWithPriority(int priority)
{
	CCScheduler::sharedScheduler()->scheduleUpdateForTarget(this, priority, !m_bIsRunning);
}

void CCNode::unscheduleUpdate()
{
	CCScheduler::sharedScheduler()->unscheduleUpdateForTarget(this);
}

void CCNode::scheduled(SEL_SCHEDULE selector)
{
	this->scheduled(selector, 0);
}

void CCNode::scheduled(SEL_SCHEDULE selector, ccTime interval)
{
	CCAssert(NULL != selector, "Argument must be non-nil");
	CCAssert(interval >= 0, "Argument must be positive");
	CCScheduler::sharedScheduler()->scheduleSelector(selector, this, interval, !m_bIsRunning);
}

void CCNode::unschedule(SEL_SCHEDULE selector)
{
	if (NULL == selector)
		return;
	CCScheduler::sharedScheduler()->unscheduleSelector(selector, this);
}

void CCNode::unscheduleAllSelectors()
{
	CCScheduler::sharedScheduler()->unscheduleAllSelectorsForTarget(this);
}

void CCNode::resumeScheduler()
{
	CCScheduler::sharedScheduler()->resumeTarget(this);
}

void CCNode::pauseScheduler()
{
	CCScheduler::sharedScheduler()->pauseTarget(this);
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
