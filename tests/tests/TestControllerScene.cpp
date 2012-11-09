#include "TestControllerScene.h"

//////////////////////////////////////////////////////////////////////////
// TestControllerScene 使用TestControllerLayer层创建测试列表场景
TestControllerScene* TestControllerScene::create_scene()
{
	TestControllerScene* scene = new TestControllerScene();
	if (NULL != scene && scene->init())
	{
		scene->autorelease();
		TestControllerLayer* layer = TestControllerLayer::create_layer();
		if (NULL != layer)
			scene->addChild(layer);
		return scene;
	}
	
	CC_SAFE_DELETE(scene);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// TestControllerLayer 测试列表层，包含所有的测试项目button和一个退出按钮
TestControllerLayer* TestControllerLayer::create_layer()
{
	TestControllerLayer* layer = new TestControllerLayer();
	if (NULL != layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

TestControllerLayer::TestControllerLayer()
{
}

TestControllerLayer::~TestControllerLayer()
{
}

bool TestControllerLayer::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		setIsTouchEnabled(true);

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();

		CCSprite* normal_sprite = CCSprite::spriteWithFile("Images/close.png");
		CC_BREAK_IF(NULL == normal_sprite);

		CCMenuItemSprite* close_item = CCMenuItemSprite::itemFromNormalSprite(normal_sprite, NULL, NULL);
		CC_BREAK_IF(NULL == close_item);
		close_item->setAnchorPoint(ccp(1, 1));
		close_item->setPosition(ccp(win_size.width, win_size.height));
		close_item->setTarget(this, menu_selector(TestControllerLayer::close_call_back));

		CCMenu* menu = CCMenu::menuWithItem(close_item);
		CC_BREAK_IF(NULL == menu);
		menu->setPosition(CCPointZero);
		addChild(menu);

		// TODO...
		// 初始化测试项菜单

		result = true;
	} while (0);

	return result;
}

void TestControllerLayer::close_call_back(CCObject* sender)
{
	CCDirector::sharedDirector()->end();
}

bool TestControllerLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	return true;
}