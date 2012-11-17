#include "TestBasicScene.h"
#include "TestControllerScene.h"

TestBasicScene* TestBasicScene::create_scene(TestBasicLayer* init_layer)
{
	TestBasicScene* scene = new TestBasicScene();
	if (NULL != scene && scene->init(init_layer))
	{
		scene->autorelease();
		return scene;
	}

	CC_SAFE_DELETE(scene);
	return NULL;
}

TestBasicScene::TestBasicScene()
	: m_controller_menu(NULL)
	, m_title_label(NULL)
	, m_test_layer(NULL)
{
}

TestBasicScene::~TestBasicScene()
{
}

bool TestBasicScene::init(TestBasicLayer* init_layer)
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !CCScene::init() );

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();

		// 演示层
		CC_BREAK_IF( NULL == init_layer);
		m_test_layer = init_layer;
		m_test_layer->setPosition(CCPointZero);
		addChild(m_test_layer);

		// 控制层，包含一组按钮和标题说明
		CCLayer* controller_layer = CCLayer::node();
		CC_BREAK_IF( NULL == controller_layer );
		controller_layer->setPosition(CCPointZero);
		addChild(controller_layer, 99);

		// 测试标题
		m_title_label = CCLabelTTF::labelWithString(m_test_layer->title(), "Arial", 24);
		CC_BREAK_IF(NULL == m_title_label);
		m_title_label->setPosition(ccp(win_size.width / 2, win_size.height - 40));
		controller_layer->addChild(m_title_label);

		// 控制菜单
		m_controller_menu = CCMenu::menuWithItem(NULL);
		CC_BREAK_IF( NULL == m_controller_menu );
		m_controller_menu->setPosition(CCPointZero);
		controller_layer->addChild(m_controller_menu);

		// 回退按钮
		CCSprite* back_sprite_normal = CCSprite::spriteWithFile("Images/back_arrow_normal.png");
		CCSprite* back_sprite_selected = CCSprite::spriteWithFile("Images/back_arrow_selected.png");
		CCSprite* back_sprite_disabled = CCSprite::spriteWithFile("Images/back_arrow_disabled.png");
		CC_BREAK_IF( NULL == back_sprite_normal || NULL == back_sprite_selected);
		CCMenuItemSprite* back_item = CCMenuItemSprite::itemFromNormalSprite(back_sprite_normal, back_sprite_selected, back_sprite_disabled);
		CC_BREAK_IF( NULL == back_item);
		back_item->setPosition(ccp(win_size.width / 2 - 100, 30));
		back_item->setTarget(this, menu_selector(TestBasicScene::back_call_back));
		back_item->setIsEnabled(NULL != m_test_layer->get_last_layer());
		m_controller_menu->addChild(back_item);

		// 重启按钮
		CCSprite* restart_sprite_normal = CCSprite::spriteWithFile("Images/restart_normal.png");
		CCSprite* restart_sprite_selected = CCSprite::spriteWithFile("Images/restart_selected.png");
		CC_BREAK_IF( NULL == restart_sprite_normal || NULL == restart_sprite_selected);
		CCMenuItemSprite* restart_item = CCMenuItemSprite::itemFromNormalSprite(restart_sprite_normal, restart_sprite_selected);
		CC_BREAK_IF( NULL == restart_item);
		restart_item->setPosition(ccp(win_size.width / 2, 30));
		restart_item->setTarget(this, menu_selector(TestBasicScene::restart_call_back));
		m_controller_menu->addChild(restart_item);

		// 前进按钮
		CCSprite* next_sprite_normal = CCSprite::spriteWithFile("Images/next_arrow_normal.png");
		CCSprite* next_sprite_selected = CCSprite::spriteWithFile("Images/next_arrow_selected.png");
		CCSprite* next_sprite_disabled = CCSprite::spriteWithFile("Images/next_arrow_disabled.png");
		CC_BREAK_IF( NULL == next_sprite_normal || NULL == next_sprite_selected);
		CCMenuItemSprite* next_item = CCMenuItemSprite::itemFromNormalSprite(next_sprite_normal, next_sprite_selected, next_sprite_disabled);
		CC_BREAK_IF( NULL == next_item);
		next_item->setPosition(ccp(win_size.width / 2 + 100, 30));
		next_item->setTarget(this, menu_selector(TestBasicScene::next_call_back));
		next_item->setIsEnabled(NULL != m_test_layer->get_next_layer());
		m_controller_menu->addChild(next_item);

		// 返回主菜单按钮
		CCLabelTTF* return_label = CCLabelTTF::labelWithString("Main Menu", "Arial", 20);
		CC_BREAK_IF(NULL == return_label);
		CCMenuItemLabel* return_item = CCMenuItemLabel::itemWithLabel(return_label);
		CC_BREAK_IF( NULL == return_item);
		return_item->setAnchorPoint(ccp(1, 0));
		return_item->setPosition(ccp(win_size.width, 0));
		return_item->setTarget(this, menu_selector(TestBasicScene::return_call_back));
		m_controller_menu->addChild(return_item);

		// 隐藏按钮菜单
		CCMenu* hide_menu = CCMenu::menuWithItem(NULL);
		CC_BREAK_IF( NULL ==  hide_menu );
		hide_menu->setPosition(CCPointZero);
		controller_layer->addChild(hide_menu);

		CCSprite* hide_sprite_normal = CCSprite::spriteWithFile("Images/hide_normal.png");
		CCSprite* hide_sprite_selected = CCSprite::spriteWithFile("Images/hide_selected.png");
		CC_BREAK_IF( NULL == hide_sprite_normal || NULL == hide_sprite_selected);
		CCMenuItemSprite* hide_item = CCMenuItemSprite::itemFromNormalSprite(hide_sprite_normal, hide_sprite_selected);
		CC_BREAK_IF( NULL == hide_item);
		hide_item->setAnchorPoint(ccp(1, 1));
		hide_item->setPosition(ccp(win_size.width, win_size.height));
		hide_item->setTarget(this, menu_selector(TestBasicScene::hide_call_back));
		hide_menu->addChild(hide_item);

		// 前进和回退按钮互相绑定，便于更新使能状态
		back_item->setUserData((void*)next_item);
		next_item->setUserData((void*)back_item);

		result = true;
	} while (0);
	
	return result;
}

void TestBasicScene::hide_call_back(CCObject* sender)
{
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(sender);
	if (NULL == item)
		return;

	bool is_shown = m_controller_menu->getIsVisible() && m_title_label->getIsVisible();
	if (is_shown)
		item->selected();

	m_controller_menu->setIsVisible(!is_shown);
	m_title_label->setIsVisible(!is_shown);
}

void TestBasicScene::back_call_back(CCObject* sender)
{
	do 
	{
		CCAssert(NULL != m_test_layer->get_last_layer(), "back button should be disabled");
		TestBasicLayer* last_layer = m_test_layer->get_last_layer();
		CC_BREAK_IF(NULL == last_layer);
		last_layer->setPosition(CCPointZero);
		addChild(last_layer);

		m_test_layer->removeFromParentAndCleanup(true);
		m_test_layer = last_layer;

		CCMenuItem* back_item = dynamic_cast<CCMenuItem*>(sender);
		CC_BREAK_IF(NULL == back_item);
		CCMenuItem* next_item = (CCMenuItem*)(back_item->getUserData());
		CC_BREAK_IF(NULL == next_item);
		back_item->setIsEnabled(NULL != m_test_layer->get_last_layer());
		next_item->setIsEnabled(NULL != m_test_layer->get_next_layer());
		m_title_label->setString(m_test_layer->title());
	} while (0);
}

void TestBasicScene::restart_call_back(CCObject* sender)
{
	//nothing
}

void TestBasicScene::next_call_back(CCObject* sender)
{
	do 
	{
		CCAssert(NULL != m_test_layer->get_next_layer(), "next button should be disabled");
		TestBasicLayer* next_layer = m_test_layer->get_next_layer();
		CC_BREAK_IF(NULL == next_layer);
		next_layer->setPosition(CCPointZero);
		addChild(next_layer);

		m_test_layer->removeFromParentAndCleanup(true);
		m_test_layer = next_layer;

		CCMenuItem* next_item = dynamic_cast<CCMenuItem*>(sender);
		CC_BREAK_IF(NULL == next_item);
		CCMenuItem* back_item = (CCMenuItem*)(next_item->getUserData());
		CC_BREAK_IF(NULL == back_item);
		back_item->setIsEnabled(NULL != m_test_layer->get_last_layer());
		next_item->setIsEnabled(NULL != m_test_layer->get_next_layer());
		m_title_label->setString(m_test_layer->title());
	} while (0);
}

void TestBasicScene::return_call_back(CCObject* sender)
{
	CCDirector::sharedDirector()->replaceScene(TestControllerScene::create_scene());
}
