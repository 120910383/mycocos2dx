﻿#pragma once

#include "cocos2d.h"

using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////
// 测试场景
class TestBasicScene : public CCScene
{
public:
	static TestBasicScene* create_scene();
	virtual ~TestBasicScene();

protected:
	TestBasicScene();
	bool init();

	void hide_call_back(CCObject* sender);
	void back_call_back(CCObject* sender);
	void restart_call_back(CCObject* sender);
	void next_call_back(CCObject* sender);
	void return_call_back(CCObject* sender);

protected:
	CCMenu* m_controller_menu;
	CCLabelTTF* m_title_label;
};