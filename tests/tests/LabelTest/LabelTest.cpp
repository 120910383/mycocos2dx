#include "LabelTest/LabelTest.h"

//////////////////////////////////////////////////////////////////////////
const char* LabelFontTest::title()
{
	return "Label Font";
}

TestBasicLayer* LabelFontTest::get_last_layer()
{
	return NULL;
}

TestBasicLayer* LabelFontTest::get_cur_layer()
{
	return LabelFontTest::create_test();
}

TestBasicLayer* LabelFontTest::get_next_layer()
{
	return NULL;
}

bool LabelFontTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !TestBasicLayer::init() );
		setIsTouchEnabled(true);

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();

		m_container_node = CCNode::node();
		CC_BREAK_IF( NULL == m_container_node );
		m_container_node->setPosition(ccp(win_size.width / 2, win_size.height));
		addChild(m_container_node);

		const float text_size = 28;
		const float line_space = 60;
		const char* font_names[] =
		{
			"fonts/Paint Boy.ttf",
			"Docum",
			"Tahoma",
			"Kartika",
			"David",
			"Candara",
			"Arial",
			"",
		};

		int SHOW_COUNT = sizeof(font_names) / sizeof(font_names[0]);
		for (int index = 0; index < SHOW_COUNT; ++index)
		{
			CCLabelTTF* label = CCLabelTTF::labelWithString("Hello World! Font Show!", font_names[index], 32);
			CC_BREAK_IF( NULL == label);
			label->setPosition(ccp(0,  - (index + 1) * line_space));
			m_container_node->addChild(label);
		}

		m_node_height = (SHOW_COUNT + 1) * line_space;

		result = true;
	} while (0);

	return result;
}

bool LabelFontTest::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	m_begin_pos = convertTouchToNodeSpace(pTouch);
	return true;
}

void LabelFontTest::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint touch_pos = convertTouchToNodeSpace(pTouch);
	float move_y = touch_pos.y - m_begin_pos.y;

	CCPoint cur_pos = m_container_node->getPosition();
	CCPoint next_pos = ccp(cur_pos.x, cur_pos.y + move_y);
	CCSize win_size = CCDirector::sharedDirector()->getWinSize();
	if (next_pos.y < win_size.height)
		next_pos.y = win_size.height;
	else if (next_pos.y > m_node_height)
		next_pos.y = m_node_height;

	m_container_node->setPosition(next_pos);
	m_begin_pos = touch_pos;
}