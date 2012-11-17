#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class TestControllerScene : public CCScene
{
public:
	static TestControllerScene* create_scene();
};

class TestControllerLayer : public CCLayer
{
public:
	static TestControllerLayer* create_layer();
	bool init();

	void close_call_back(CCObject* sender);
	void menu_call_back(CCObject* sender);
};