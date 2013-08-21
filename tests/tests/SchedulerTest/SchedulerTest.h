#pragma once

#include "TestBasicScene.h"

//////////////////////////////////////////////////////////////////////////
class SchedulerUpdateTest : public TestBasicLayer
{
public:
	SchedulerUpdateTest();
	CREATE_TEST(SchedulerUpdateTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_cur_layer();
	virtual TestBasicLayer* get_next_layer();
	virtual void update(ccTime dt);

protected:
	bool init();

protected:
	CCLabelTTF* m_move_label;
	CCLabelTTF* m_timer_label;
	ccTime m_total_time;
};

//////////////////////////////////////////////////////////////////////////
class SchedulerSelectorTest : public TestBasicLayer
{
public:
	SchedulerSelectorTest();
	CREATE_TEST(SchedulerSelectorTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_cur_layer();
	virtual TestBasicLayer* get_next_layer();
	void update_custom_blink(ccTime dt);
	void update_count_label(ccTime dt);

protected:
	bool init();

protected:
	CCLabelTTF* m_blind_label;
	CCLabelTTF* m_count_label;
	unsigned int m_count_num;
};