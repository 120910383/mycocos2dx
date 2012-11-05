#pragma once

#include "CCObject.h"
#include "CCGeometry.h"
#include "CCMutableArray.h"
#include "CCAffineTransform.h"
#include "CCGL.h"

NS_CC_BEGIN;

class CCTouch;

enum
{
	kCCNodeTagInvalid = -1,
};

class CC_DLL CCNode : public CCObject
{
public:
	static CCNode* node();
	CCNode();
	virtual ~CCNode();

public:
	virtual const CCPoint& getPosition();
	virtual void setPosition(const CCPoint& newPosition);

	virtual const CCPoint& getAnchorPointInPoints();
	virtual const CCPoint& getAnchorPoint();
	virtual void setAnchorPoint(const CCPoint& point);

	virtual const CCSize& getContentSize();
	virtual void setContentSize(const CCSize& size);

	virtual float getVertexZ();
	virtual void setVertexZ(float var);

	virtual float getRotation();
	virtual void setRotation(float var);

	float getScale();
	void setScale(float scale);
	virtual float getScaleX();
	virtual void setScaleX(float var);
	virtual float getScaleY();
	virtual void setScaleY(float var);

	virtual float getSkewX();
	virtual void setSkewX(float var);
	virtual float getSkewY();
	virtual void setSkewY(float var);

	virtual bool getIsVisible();
	virtual void setIsVisible(bool var);

	// m_bIsRelativeAnchorPoint如果为true，则transform则会使用anchorpoint计算node的左下角位置，然后做参考点转换
	// 如果为false，则transform直接把position当作参考点进行转换，对于scene layer menu等全屏幕的节点，默认为false，
	// 即如果放置一个子节点在其上面的话，指定的位置是相对于anchorpoint位置的，对于sprite,label的带指定尺寸的
	// 节点，默认为true，即如果放置一个字节点在其上的话，指定的位置是相对于parent的左下角而言
	virtual bool getIsRelativeAnchorPoint();
	virtual void setIsRelativeAnchorPoint(bool var);

	virtual int getTag();
	virtual void setTag(int var);

	virtual void* getUserData();
	virtual void setUserData(void* var);

	virtual CCNode* getParent();
	virtual void setParent(CCNode* var);

	virtual bool getIsRunning();
	virtual int getZOrder();
	virtual CCMutableArray<CCNode*>* getChildren();

public:
	virtual void onEnter();
	virtual void onExit();

	virtual void draw(void);
	virtual void visit(void);

	virtual void addChild(CCNode* child);
	virtual void addChild(CCNode* child, int zOrder);
	virtual void addChild(CCNode* child, int zOrder, int tag);

	virtual void reorderChild(CCNode* child, int zOrder);
	CCNode* getChildByTag(int tag);
	unsigned int getChildrenCount();

	void removeFromParentAndCleanup(bool cleanup);
	void removeChildByTag(int tag, bool cleanup);
	virtual void removeChild(CCNode* child, bool cleanup);
	virtual void removeAllChildrenWithCleanup(bool cleanup);

	virtual void cleanup();

	CCAffineTransform nodeToParentTransform();
	CCAffineTransform parentToNodeTransform();
	CCAffineTransform nodeToWorldTransform();
	CCAffineTransform worldToNodeTransform();

	CCPoint convertToNodeSpace(const CCPoint& worldPoint);
	CCPoint convertToWorldSpace(const CCPoint& nodePoint);
	CCPoint convertTouchToNodeSpace(CCTouch* touch);

	void transform();
	void transformAncestors();
	CCRect boundingBox();

private:
	void childrenAlloc();
	void insertChild(CCNode* child, int z);
	void setZOrder(int z);
	void detachChild(CCNode* child, bool doCleanup);

	typedef void (CCNode::*callbackFunc)();
	void arrayMakeObjectsPerformSelector(CCMutableArray<CCNode*>* pArray, callbackFunc func);

protected:
	int m_nZOrder;
	CCPoint m_tPosition;
	CCPoint m_tAnchorPointInPoints;
	CCPoint m_tAnchorPoint;
	CCSize m_tContentSize;
	float m_fVertexZ;
	float m_fRotation;
	float m_fScaleX;
	float m_fScaleY;
	float m_fSkewX;
	float m_fSkewY;
	bool m_bIsRelativeAnchorPoint;
	bool m_bIsRunning;
	bool m_bIsVisible;
	int m_nTag;
	void* m_pUserData;
	CCNode* m_pParent;
	CCMutableArray<CCNode*>* m_pChildren;

	CCAffineTransform m_tTransform;
	CCAffineTransform m_tInverse;
	GLfloat m_pTransformGL[16];
	bool m_bIsTransformDirty;
	bool m_bIsInverseDirty;
	bool m_bIsTransformGLDirty;
};

NS_CC_END;