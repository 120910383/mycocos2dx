#pragma once

#include "cocos2d.h"
USING_NS_CC;

class TestScene : public CCScene
{
public:
	static CCScene* node();
	virtual void draw();
	virtual void setPosition(const CCPoint& newPosition);
	virtual void setAnchorPoint(const CCPoint& point);
};

class TestScene1 : public CCScene
{
public:
	static CCScene* node();
	virtual void draw();
	virtual void setPosition(const CCPoint& newPosition);
	virtual void setAnchorPoint(const CCPoint& point);
};