#pragma once

#include "cocos2d.h"

using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////
// 测试场景
class TestBasicLayer;
class TestBasicScene : public CCScene
{
public:
	static TestBasicScene* create_scene(TestBasicLayer* init_layer);
	virtual ~TestBasicScene();

protected:
	TestBasicScene();
	bool init(TestBasicLayer* init_layer);

	void hide_call_back(CCObject* sender);
	void back_call_back(CCObject* sender);
	void restart_call_back(CCObject* sender);
	void next_call_back(CCObject* sender);
	void return_call_back(CCObject* sender);

protected:
	CCMenu* m_controller_menu;
	CCLabelTTF* m_title_label;
	TestBasicLayer* m_test_layer;
};

class TestBasicLayer : public CCLayer
{
public:
	virtual const char* title() = 0;
	virtual TestBasicLayer* get_last_layer() = 0;
	virtual TestBasicLayer* get_next_layer() = 0;
};

#define CREATE_TEST(test_layer) \
	static test_layer* create_test() \
{ \
	test_layer* pRet = new test_layer(); \
	if (NULL != pRet && pRet->init()) \
	{ \
		pRet->autorelease(); \
		return pRet; \
	} \
	CC_SAFE_DELETE(pRet); \
	return NULL; \
};