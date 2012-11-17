#pragma once

#include "TestBasicScene.h"

//////////////////////////////////////////////////////////////////////////
class SpriteOpacityTest : public TestBasicLayer
{
public:
	CREATE_TEST(SpriteOpacityTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_cur_layer();
	virtual TestBasicLayer* get_next_layer();

protected:
	bool init();
};

//////////////////////////////////////////////////////////////////////////
class SpriteColorTest : public TestBasicLayer
{
public:
	CREATE_TEST(SpriteColorTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_cur_layer();
	virtual TestBasicLayer* get_next_layer();

protected:
	bool init();
};
