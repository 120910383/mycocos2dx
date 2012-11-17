#include "TestBasicScene.h"
#include "TestControllerScene.h"

TestBasicScene* TestBasicScene::create_scene()
{
	TestBasicScene* scene = new TestBasicScene();
	if (NULL != scene && scene->init())
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
{
}

TestBasicScene::~TestBasicScene()
{
}

bool TestBasicScene::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !CCScene::init() );

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();

		// 控制层，包含一组按钮和标题说明
		CCLayer* controller_layer = CCLayer::node();
		CC_BREAK_IF( NULL == controller_layer );
		controller_layer->setPosition(CCPointZero);
		addChild(controller_layer);

		// 测试标题
		m_title_label = CCLabelTTF::labelWithString("test title", "Arial", 24);
		CC_BREAK_IF(NULL == m_title_label);
		m_title_label->setPosition(ccp(win_size.width / 2, win_size.height - 40));
		controller_layer->addChild(m_title_label);

		// 控制菜单
		m_controller_menu = CCMenu::menuWithItem(NULL);
		CC_BREAK_IF( NULL == m_controller_menu );
		m_controller_menu->setPosition(CCPointZero);
		controller_layer->addChild(m_controller_menu);

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

void TestBasicScene::return_call_back(CCObject* sender)
{
	CCDirector::sharedDirector()->replaceScene(TestControllerScene::create_scene());
}