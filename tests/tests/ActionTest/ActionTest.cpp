#include "ActionTest/ActionTest.h"

//////////////////////////////////////////////////////////////////////////
ActionMoveToTest::ActionMoveToTest()
	: m_move_label(NULL)
{
}

const char* ActionMoveToTest::title()
{
	return "Action CCMoveTo Test";
}

TestBasicLayer* ActionMoveToTest::get_last_layer()
{
	return NULL;
}

TestBasicLayer* ActionMoveToTest::get_cur_layer()
{
	return ActionMoveToTest::create_test();
}

TestBasicLayer* ActionMoveToTest::get_next_layer()
{
	return ActionMoveByTest::create_test();
}

bool ActionMoveToTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF(!TestBasicLayer::init());

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
		m_move_label = CCLabelTTF::labelWithString("MoveToTest", "", 32);
		if (NULL != m_move_label)
		{
			m_move_label->setPosition(ccp(win_size.width / 4, win_size.height / 4));
			CCMoveTo* move_to_action = CCMoveTo::actionWithDuration(2.0f, ccp(win_size.width * 3 / 4, win_size.height * 3 / 4));
			m_move_label->runAction(move_to_action);
			addChild(m_move_label);
		}

		result = true;
	} while (0);

	return result;
}

//////////////////////////////////////////////////////////////////////////
ActionMoveByTest::ActionMoveByTest()
	: m_move_label(NULL)
	, m_reverse_move_label(NULL)
{
}

const char* ActionMoveByTest::title()
{
	return "Action CCMoveBy Test";
}

TestBasicLayer* ActionMoveByTest::get_last_layer()
{
	return ActionMoveToTest::create_test();
}

TestBasicLayer* ActionMoveByTest::get_cur_layer()
{
	return ActionMoveByTest::create_test();
}

TestBasicLayer* ActionMoveByTest::get_next_layer()
{
	return ActionScaleToTest::create_test();
}

bool ActionMoveByTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF(!TestBasicLayer::init());

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
		CCActionInterval* move_by_action = CCMoveBy::actionWithDuration(2.0f, ccp(win_size.width / 4, win_size.height / 4));
		CCActionInterval* move_by_action_reverse = move_by_action->reverse();

		m_move_label = CCLabelTTF::labelWithString("MoveByTest", "", 32);
		if (NULL != m_move_label)
		{
			m_move_label->setPosition(ccp(win_size.width / 2, win_size.height / 2));
			m_move_label->runAction(move_by_action);
			addChild(m_move_label);
		}

		m_reverse_move_label = CCLabelTTF::labelWithString("MoveByReverseTest", "", 32);
		if (NULL != m_reverse_move_label)
		{
			m_reverse_move_label->setPosition(ccp(win_size.width / 2, win_size.height / 2));
			m_reverse_move_label->runAction(move_by_action_reverse);
			addChild(m_reverse_move_label);
		}

		result = true;
	} while (0);

	return result;
}

//////////////////////////////////////////////////////////////////////////
ActionScaleToTest::ActionScaleToTest()
	: m_scale_sprite(NULL)
	, m_scale_xy_sprite(NULL)
{
}

const char* ActionScaleToTest::title()
{
	return "Action CCScaleTo Test";
}

TestBasicLayer* ActionScaleToTest::get_last_layer()
{
	return ActionMoveByTest::create_test();
}

TestBasicLayer* ActionScaleToTest::get_cur_layer()
{
	return ActionScaleToTest::create_test();
}

TestBasicLayer* ActionScaleToTest::get_next_layer()
{
	return ActionScaleByTest::create_test();
}

bool ActionScaleToTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF(!TestBasicLayer::init());

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();

		m_scale_sprite = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF(NULL == m_scale_sprite);
		m_scale_sprite->setPosition(ccp(win_size.width / 3, win_size.height / 2));
		m_scale_sprite->runAction(CCScaleTo::actionWithDuration(2.0f, 0.2f));
		addChild(m_scale_sprite);

		m_scale_xy_sprite = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF(NULL == m_scale_xy_sprite);
		m_scale_xy_sprite->setPosition(ccp(win_size.width * 2 / 3, win_size.height / 2));
		m_scale_xy_sprite->runAction(CCScaleTo::actionWithDuration(2.0f, 0.5f, 2.0f));
		addChild(m_scale_xy_sprite);

		result = true;
	} while (0);

	return result;
}

//////////////////////////////////////////////////////////////////////////
ActionScaleByTest::ActionScaleByTest()
	: m_scale_sprite(NULL)
	, m_reverse_scale_sprite(NULL)
{
}

const char* ActionScaleByTest::title()
{
	return "Action CCScaleBy Test";
}

TestBasicLayer* ActionScaleByTest::get_last_layer()
{
	return ActionMoveToTest::create_test();
}

TestBasicLayer* ActionScaleByTest::get_cur_layer()
{
	return ActionScaleByTest::create_test();
}

TestBasicLayer* ActionScaleByTest::get_next_layer()
{
	return NULL;
}

bool ActionScaleByTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF(!TestBasicLayer::init());

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
		CCActionInterval* scale_by_action = CCScaleBy::actionWithDuration(2.0f, 3.0f);
		CC_BREAK_IF(NULL == scale_by_action);
		CCActionInterval* scale_by_action_reverse = scale_by_action->reverse();
		CC_BREAK_IF(NULL == scale_by_action_reverse);

		m_scale_sprite = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF(NULL == m_scale_sprite);
		m_scale_sprite->setPosition(ccp(win_size.width / 3, win_size.height / 2));
		m_scale_sprite->runAction(scale_by_action);
		addChild(m_scale_sprite);

		m_reverse_scale_sprite = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF(NULL == m_reverse_scale_sprite);
		m_reverse_scale_sprite->setPosition(ccp(win_size.width * 2 / 3, win_size.height / 2));
		m_reverse_scale_sprite->runAction(scale_by_action_reverse);
		addChild(m_reverse_scale_sprite);

		result = true;
	} while (0);

	return result;
}