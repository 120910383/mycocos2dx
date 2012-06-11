#pragma once

#include "CCObject.h"
#include "CCGeometry.h"

NS_CC_BEGIN;

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

	virtual bool getIsRunning();
	virtual int getZOrder();

public:
	virtual void onEnter();
	virtual void onExit();

	virtual void draw(void);
	virtual void visit(void);

protected:
	int m_nZOrder;
	CCPoint m_tPosition;
	CCPoint m_tAnchorPoint;
	CCSize m_tContentSize;
	bool m_bIsRunning;
};

NS_CC_END;