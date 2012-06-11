#pragma once

#include "cocos2d.h"

class TestScene : public cocos2d::CCScene
{
public:
	static cocos2d::CCScene* node();
	virtual void draw();
	virtual void setPosition(const cocos2d::CCPoint& newPosition);
	virtual void setAnchorPoint(const cocos2d::CCPoint& point);
};

class TestScene1 : public cocos2d::CCScene
{
public:
	static cocos2d::CCScene* node();
	virtual void draw();
	virtual void setPosition(const cocos2d::CCPoint& newPosition);
	virtual void setAnchorPoint(const cocos2d::CCPoint& point);
};