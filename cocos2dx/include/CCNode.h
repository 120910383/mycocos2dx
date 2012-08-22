#pragma once

#include "CCObject.h"
#include "CCGeometry.h"
#include "CCMutableArray.h"

NS_CC_BEGIN;

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

	virtual void addChild(CCNode* child, int zOrder, int tag);

private:
	void childrenAlloc();
	void insertChild(CCNode* child, int z);
	void setZOrder(int z);

	typedef void (CCNode::*callbackFunc)();
	void arrayMakeObjectsPerformSelector(CCMutableArray<CCNode*>* pArray, callbackFunc func);

protected:
	int m_nZOrder;
	CCPoint m_tPosition;
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
};

NS_CC_END;