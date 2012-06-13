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

	virtual bool getIsVisible();
	virtual void setIsVisible(bool var);

	virtual int getTag();
	virtual void setTag(int var);

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
	bool m_bIsRunning;
	bool m_bIsVisible;
	int m_nTag;
	CCNode* m_pParent;
	CCMutableArray<CCNode*>* m_pChildren;
};

NS_CC_END;