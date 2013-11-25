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
	return NULL;
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