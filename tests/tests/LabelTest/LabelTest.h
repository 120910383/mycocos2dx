#pragma once

#include "TestBasicScene.h"

//////////////////////////////////////////////////////////////////////////
class LabelFontTest : public TestBasicLayer
{
public:
	CREATE_TEST(LabelFontTest);
	virtual const char* title();
	virtual TestBasicLayer* get_last_layer();
	virtual TestBasicLayer* get_cur_layer();
	virtual TestBasicLayer* get_next_layer();

protected:
	LabelFontTest() : m_container_node(NULL), m_node_height(0) {}
	bool init();

	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);

private:
	CCNode* m_container_node;
	CCPoint m_begin_pos;
	float m_node_height;
};