#pragma once

#include "cocos2d.h"
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
class TestScene : public CCScene
{
public:
	static CCScene* scene();
	virtual void draw();
	virtual void setPosition(const CCPoint& newPosition);
	virtual void setAnchorPoint(const CCPoint& point);
};

//////////////////////////////////////////////////////////////////////////
class TestScene1 : public CCScene
{
public:
	static CCScene* scene();
	virtual void draw();
	virtual void setPosition(const CCPoint& newPosition);
	virtual void setAnchorPoint(const CCPoint& point);
};

//////////////////////////////////////////////////////////////////////////
class TestScene2 : public CCScene
{
public:
	static CCScene* scene();
	virtual void setPosition(const CCPoint& newPosition);
	virtual void setAnchorPoint(const CCPoint& point);
};

class TestLayer2 : public CCLayer
{
public:
	static TestLayer2* node();
	virtual bool init();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void draw();

private:
	CCPoint m_touch_pos;
	CCPoint m_perp;
	float m_rotate;
};

//////////////////////////////////////////////////////////////////////////
class TestScene3 : public CCScene
{
public:
	static CCScene* scene();
	virtual void setPosition(const CCPoint& newPosition);
	virtual void setAnchorPoint(const CCPoint& point);
};

class TestLayer3 : public CCLayer
{
public:
	static TestLayer3* node();
	TestLayer3();
	virtual bool init();
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);

private:
	CCSprite* m_sprite;
};