#include "SchedulerTest/SchedulerTest.h"

//////////////////////////////////////////////////////////////////////////
SchedulerUpdateTest::SchedulerUpdateTest()
	: m_move_label(NULL)
	, m_timer_label(NULL)
	, m_total_time(0)
{

}

const char* SchedulerUpdateTest::title()
{
	return "Scheduler Update Test";
}

TestBasicLayer* SchedulerUpdateTest::get_last_layer()
{
	return NULL;
}

TestBasicLayer* SchedulerUpdateTest::get_cur_layer()
{
	return SchedulerUpdateTest::create_test();
}

TestBasicLayer* SchedulerUpdateTest::get_next_layer()
{
	return SchedulerSelectorTest::create_test();
}

bool SchedulerUpdateTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !TestBasicLayer::init() );

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
		m_move_label = CCLabelTTF::labelWithString("UpdateMoveTest", "", 32);
		if (NULL != m_move_label)
		{
			m_move_label->setPosition(ccp(win_size.width / 4, win_size.height / 4));
			addChild(m_move_label);
		}

		m_timer_label = CCLabelTTF::labelWithString("", "", 32);
		if (NULL != m_timer_label)
		{
			m_timer_label->setAnchorPoint(ccp(0.5, 0.5));
			m_timer_label->setPosition(ccp(win_size.width / 2, win_size.height / 2));
			addChild(m_timer_label);
		}

		scheduleUpdate();
		result = true;
	} while (0);

	return result;
}

void SchedulerUpdateTest::update(ccTime dt)
{
	CCSize win_size = CCDirector::sharedDirector()->getWinSize();

	if (NULL != m_move_label)
	{
		const double angle = 0.02f;
		CCPoint pos = ccpSub(m_move_label->getPosition(), ccp(win_size.width / 2, win_size.height / 2));
		pos = ccp(ccpDot(pos, ccp(cos(angle), -sin(angle))), ccpDot(pos, ccp(sin(angle), cos(angle))));
		m_move_label->setPosition(ccpAdd(pos, ccp(win_size.width / 2, win_size.height / 2)));
	}

	if (NULL != m_timer_label)
	{
		m_total_time += dt;
		int hour = (int)m_total_time / 3600;
		int minute = (int)m_total_time % 3600 / 60;
		int second = (int)m_total_time % 60;
		char ret[100];
		sprintf(ret,"%02d:%02d:%02d", hour, minute, second);
		if (0 != strcmp(ret, m_timer_label->getString()))
			m_timer_label->setString(ret);
	}
}

//////////////////////////////////////////////////////////////////////////
SchedulerSelectorTest::SchedulerSelectorTest()
	: m_blind_label(NULL)
	, m_count_label(NULL)
	, m_count_num(0)
{

}

const char* SchedulerSelectorTest::title()
{
	return "Scheduler Selector Test";
}

TestBasicLayer* SchedulerSelectorTest::get_last_layer()
{
	return SchedulerUpdateTest::create_test();
}

TestBasicLayer* SchedulerSelectorTest::get_cur_layer()
{
	return SchedulerSelectorTest::create_test();
}

TestBasicLayer* SchedulerSelectorTest::get_next_layer()
{
	return NULL;
}

bool SchedulerSelectorTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !TestBasicLayer::init() );

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
		m_blind_label = CCLabelTTF::labelWithString("this blinks every two seconds", "", 32);
		if (NULL != m_blind_label)
		{
			m_blind_label->setAnchorPoint(ccp(0.5, 0.5));
			m_blind_label->setPosition(ccp(win_size.width / 2, win_size.height / 2));
			m_blind_label->setIsVisible(false);
			addChild(m_blind_label);
		}

		m_count_num = 0;
		m_count_label = CCLabelTTF::labelWithString("elapsed seconds : 0", "", 32);
		if (NULL != m_count_label)
		{
			m_count_label->setAnchorPoint(ccp(0.5, 0.5));
			m_count_label->setPosition(ccp(win_size.width / 2, win_size.height / 2 - 60));
			addChild(m_count_label);
		}

		scheduled(schedule_selector(SchedulerSelectorTest::update_custom_blink), 2);
		scheduled(schedule_selector(SchedulerSelectorTest::update_count_label), 1);
		result = true;
	} while (0);

	return result;
}

void SchedulerSelectorTest::update_custom_blink(ccTime dt)
{
	if (NULL != m_blind_label)
	{
		bool is_visible = m_blind_label->getIsVisible();
		m_blind_label->setIsVisible(!is_visible);
	}
}

void SchedulerSelectorTest::update_count_label(ccTime dt)
{
	if (NULL != m_count_label)
	{
		m_count_num += 1;
		char ret[100];
		sprintf(ret,"elapsed seconds : %d", m_count_num);
		m_count_label->setString(ret);
	}
}