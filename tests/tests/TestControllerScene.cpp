#include "TestControllerScene.h"
#include "TestDefinition.h"
#include "TestBasicScene.h"

const float LINE_SPACE = 40.0f;
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

bool TestControllerLayer::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		setIsTouchEnabled(true);

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();

		// 关闭按钮菜单
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

		// 测试项菜单
		CCMenu* tests_item_menu = CCMenu::menuWithItem(NULL);
		CC_BREAK_IF(NULL == tests_item_menu);
		tests_item_menu->setPosition(ccp(0, win_size.height));
		addChild(tests_item_menu);

		for (int index = 0; index < TESTS_COUNT; ++index)
		{
			CCLabelTTF* test_name_label = CCLabelTTF::labelWithString(test_names[index], "Arial", 24);
			if (NULL != test_name_label)
			{
				CCMenuItemLabel* test_item = CCMenuItemLabel::itemWithLabel(test_name_label);
				if (NULL != test_item)
				{
					test_item->setPosition(ccp(win_size.width / 2, -LINE_SPACE * (index + 1)));
					test_item->setTarget(this, menu_selector(TestControllerLayer::menu_call_back));
					test_item->setTag(index);
					tests_item_menu->addChild(test_item);
				}
			}
		}

		result = true;
	} while (0);

	return result;
}

void TestControllerLayer::close_call_back(CCObject* sender)
{
	CCDirector::sharedDirector()->end();
}

void TestControllerLayer::menu_call_back(CCObject* sender)
{
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(sender);
	if (NULL == item)
		return;

	TestBasicScene* test_scene = TestBasicScene::create_scene(NULL);
	if (NULL != test_scene)
	{
		CCDirector::sharedDirector()->replaceScene(test_scene);
	}
}

bool TestControllerLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	return true;
}