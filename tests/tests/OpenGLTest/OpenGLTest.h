#pragma once

#include "TestBasicScene.h"

//////////////////////////////////////////////////////////////////////////
class OpenGLRotateCubeTest : public TestBasicLayer
{
public:
	CREATE_TEST(OpenGLRotateCubeTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_next_layer();

protected:
	bool init();
	virtual void draw();
};