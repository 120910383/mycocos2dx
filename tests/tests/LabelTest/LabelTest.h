#pragma once

#include "TestBasicScene.h"

//////////////////////////////////////////////////////////////////////////
class LabelFontTest : public TestBasicLayer
{
public:
	CREATE_TEST(LabelFontTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_next_layer();

protected:
	bool init();
};