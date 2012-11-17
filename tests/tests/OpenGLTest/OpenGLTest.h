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

//////////////////////////////////////////////////////////////////////////
class OpenGLDragCubeTest : public TestBasicLayer
{
public:
	CREATE_TEST(OpenGLDragCubeTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_next_layer();

protected:
	OpenGLDragCubeTest() : m_rotate(0) {}
	bool init();

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void draw();

private:
	CCPoint m_touch_pos;
	CCPoint m_perp;
	float m_rotate;
};