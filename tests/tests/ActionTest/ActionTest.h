#pragma once

#include "TestBasicScene.h"

//////////////////////////////////////////////////////////////////////////
class ActionMoveToTest : public TestBasicLayer
{
public:
	ActionMoveToTest();
	CREATE_TEST(ActionMoveToTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_cur_layer();
	virtual TestBasicLayer* get_next_layer();
	
protected:
	bool init();

protected:
	CCLabelTTF* m_move_label;
};

//////////////////////////////////////////////////////////////////////////
class ActionMoveByTest : public TestBasicLayer
{
public:
	ActionMoveByTest();
	CREATE_TEST(ActionMoveByTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_cur_layer();
	virtual TestBasicLayer* get_next_layer();

protected:
	bool init();

protected:
	CCLabelTTF* m_move_label;
	CCLabelTTF* m_reverse_move_label;
};

//////////////////////////////////////////////////////////////////////////
class ActionScaleToTest : public TestBasicLayer
{
public:
	ActionScaleToTest();
	CREATE_TEST(ActionScaleToTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_cur_layer();
	virtual TestBasicLayer* get_next_layer();

protected:
	bool init();

protected:
	CCSprite* m_scale_sprite;
	CCSprite* m_scale_xy_sprite;
};

//////////////////////////////////////////////////////////////////////////
class ActionScaleByTest : public TestBasicLayer
{
public:
	ActionScaleByTest();
	CREATE_TEST(ActionScaleByTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_cur_layer();
	virtual TestBasicLayer* get_next_layer();

protected:
	bool init();

protected:
	CCSprite* m_scale_sprite;
	CCSprite* m_reverse_scale_sprite;
};