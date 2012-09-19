#include "TestScene.h"
#include "GLES/gl.h"		//测试

CCScene* TestScene::scene()
{
	CCScene *pRet = new TestScene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		CCScene* pChild = new TestScene1();
		if (pChild && pChild->init())
		{
			pRet->addChild(pChild, 1, 0);
		}
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void TestScene::setPosition(const CCPoint& newPosition)
{
	CCDirector::sharedDirector()->replaceScene(TestScene1::scene());
}

void TestScene::setAnchorPoint(const CCPoint& point)
{
	if (CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->resume();
	else
		CCDirector::sharedDirector()->pause();
}

void TestScene::draw()
{
	CCScene::draw();
	
	unsigned char mIndices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
	unsigned char mIndices1[] = {0, 1, 3, 2, 6, 1, 5, 0, 4, 3, 7, 6, 4, 5};
	const GLfloat cube[] = 
	{
		100.0f, 100.0f, -100.0f,
		300.0f, 100.0f, -100.0f,
		300.0f, 300.0f, -100.0f,
		100.0f, 300.0f, -100.0f,
		100.0f, 100.0f, -300.0f,
		300.0f, 100.0f, -300.0f,
		300.0f, 300.0f, -300.0f,
		100.0f, 300.0f, -300.0f,
	};
	const GLfloat color[] = 
	{
		1.0f, 0.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 1.0f, 100.0f,
		0.0f, 1.0f, 1.0f, 100.0f,
		1.0f, 0.0f, 1.0f, 100.0f,
		1.0f, 1.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 0.0f, 100.0f,
		1.0f, 1.0f, 1.0f, 100.0f,
	};
	static GLfloat rotate = 0.4f;
	rotate += 0.4f;

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glColorPointer(4, GL_FLOAT, 0, color);
	glVertexPointer(3, GL_FLOAT, 0, &cube);
	glTranslatef(0.0f, -50.0f, 0.0f);
	glRotatef(rotate, 100.0f, 100.0f, -100.0f);
	glBlendFunc(GL_ONE, GL_ZERO);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_BYTE, mIndices1);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, mIndices);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

//////////////////////////////////////////////////////////////////////////
CCScene* TestScene1::scene()
{
	CCScene *pRet = new TestScene1();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void TestScene1::setPosition(const CCPoint& newPosition)
{
	CCDirector::sharedDirector()->replaceScene(TestScene2::scene());
}

void TestScene1::setAnchorPoint(const CCPoint& point)
{
	if (CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->resume();
	else
		CCDirector::sharedDirector()->pause();
}


void TestScene1::draw()
{
	CCScene::draw();

	unsigned char mIndices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
	const GLfloat cube[] = 
	{
		100.0f, 100.0f, -100.0f,
		300.0f, 100.0f, -100.0f,
		300.0f, 300.0f, -100.0f,
		100.0f, 300.0f, -100.0f,
		100.0f, 100.0f, -300.0f,
		300.0f, 100.0f, -300.0f,
		300.0f, 300.0f, -300.0f,
		100.0f, 300.0f, -300.0f,
	};
	static GLfloat rotate = 0.4f;
	rotate += 0.4f;

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &cube);
	glTranslatef(0.0f, -50.0f, 0.0f);
	glRotatef(rotate, 100.0f, 100.0f, -100.0f);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, mIndices);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

//////////////////////////////////////////////////////////////////////////
CCScene* TestScene2::scene()
{
	CCScene *pRet = new TestScene2();
	if (pRet && pRet->init())
	{
		pRet->autorelease();

		TestLayer2* layer = TestLayer2::node();
		if (NULL != layer)
			pRet->addChild(layer, 1, 0);

		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void TestScene2::setPosition(const CCPoint& newPosition)
{
	CCDirector::sharedDirector()->replaceScene(TestScene3::scene());
}

void TestScene2::setAnchorPoint(const CCPoint& point)
{
	if (CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->resume();
	else
		CCDirector::sharedDirector()->pause();
}


TestLayer2* TestLayer2::node()
{
	TestLayer2* layer = new TestLayer2();
	if (NULL != layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}
	else
	{
		CC_SAFE_DELETE(layer);
		return NULL;
	}
}

bool TestLayer2::init()
{
	m_rotate = 0;
	bool result = false;
	do 
	{
		CC_BREAK_IF( !CCLayer::init() );
		setIsTouchEnabled(true);

		//todo...

		result = true;
	} while (0);
	return result;
}

void TestLayer2::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addDelegate(this, -128, true);
}

bool TestLayer2::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	m_touch_pos = pTouch->locationInView(pTouch->view());
	m_touch_pos = CCDirector::sharedDirector()->convertToGL(m_touch_pos);
	return true;
}

void TestLayer2::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint move_pos = pTouch->locationInView(pTouch->view());
	move_pos = CCDirector::sharedDirector()->convertToGL(move_pos);
	m_perp = ccpPerp(ccpSub(move_pos, m_touch_pos));
	m_rotate = ccpDistance(move_pos, m_touch_pos) / 2.0f;
}

void TestLayer2::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	//m_perp = CCPointZero;
	//m_rotate = 0;
}

void TestLayer2::draw()
{
	CCLayer::draw();

	unsigned char mIndices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
	unsigned char mIndices1[] = {0, 1, 3, 2, 6, 1, 5, 0, 4, 3, 7, 6, 4, 5};
	const GLfloat cube[] = 
	{
		-100.0f, -100.0f, 100.0f,
		100.0f, -100.0f, 100.0f,
		100.0f, 100.0f, 100.0f,
		-100.0f, 100.0f, 100.0f,
		-100.0f, -100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,
		100.0f, 100.0f, -100.0f,
		-100.0f, 100.0f, -100.0f,
	};
	const GLfloat color[] = 
	{
		1.0f, 0.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 1.0f, 100.0f,
		0.0f, 1.0f, 1.0f, 100.0f,
		1.0f, 0.0f, 1.0f, 100.0f,
		1.0f, 1.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 0.0f, 100.0f,
		1.0f, 1.0f, 1.0f, 100.0f,
	};

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glColorPointer(4, GL_FLOAT, 0, color);
	glVertexPointer(3, GL_FLOAT, 0, &cube);
	glTranslatef(200.0f, 150.0f, 0.0f);
	glRotatef(m_rotate, m_perp.x, m_perp.y, 0.0f);
	glBlendFunc(GL_ONE, GL_ZERO);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_BYTE, mIndices1);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, mIndices);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

//////////////////////////////////////////////////////////////////////////
CCScene* TestScene3::scene()
{
	CCScene *pRet = new TestScene3();
	if (pRet && pRet->init())
	{
		pRet->autorelease();

		TestLayer3* layer = TestLayer3::node();
		if (NULL != layer)
			pRet->addChild(layer, 1, 0);

		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void TestScene3::setPosition(const CCPoint& newPosition)
{
	CCDirector::sharedDirector()->replaceScene(TestScene::scene());
}

void TestScene3::setAnchorPoint(const CCPoint& point)
{
	if (CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->resume();
	else
		CCDirector::sharedDirector()->pause();
}

TestLayer3* TestLayer3::node()
{
	TestLayer3* layer = new TestLayer3();
	if (NULL != layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}
	else
	{
		CC_SAFE_DELETE(layer);
		return NULL;
	}
}

TestLayer3::TestLayer3()
	: m_sprite(NULL)
{

}

bool TestLayer3::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !CCLayer::init() );
		setIsTouchEnabled(true);

		CCLabelTTF* label = CCLabelTTF::labelWithString("Hello World!", "none", 32);
		CC_BREAK_IF(NULL == label);
		label->setAnchorPoint(ccp(0.5, 1));
		label->setPosition(ccp(240, 320));
		label->setColor(ccc3(255, 0, 255));
		addChild(label, 2, 0);

		m_sprite = CCSprite::spriteWithFile("Images\\blocks.png", CCRectMake(16, 16, 96, 96));
		CC_BREAK_IF(NULL == m_sprite);
		m_sprite->setAnchorPoint(CCPointZero);
		m_sprite->setPosition(ccp(100, 100));
		m_sprite->setColor(ccc3(180, 180, 255));
		addChild(m_sprite, 1, 0);

		label = CCLabelTTF::labelWithString("Click Me!", "none", 26);
		CC_BREAK_IF(NULL == label);
		label->setColor(ccc3(255, 255, 0));

		CCMenuItemLabel* item_label = CCMenuItemLabel::itemWithLabel(label);
		CC_BREAK_IF(NULL == item_label);
		item_label->setAnchorPoint(ccp(1, 0));
		item_label->setPosition(ccp(480, 0));
		item_label->setTarget(this, menu_selector(TestLayer3::on_click_label_item));
		CCMenu* menu = CCMenu::menuWithItem(item_label);
		CC_BREAK_IF(NULL == menu);
		menu->setPosition(CCPointZero);
		addChild(menu, 2, 0);

		result = true;
	} while (0);
	return result;
}

bool TestLayer3::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint move_pos = convertTouchToNodeSpace(pTouch);
	m_sprite->setPosition(move_pos);
	return true;
}

void TestLayer3::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint move_pos = convertTouchToNodeSpace(pTouch);
	m_sprite->setPosition(move_pos);
}

void TestLayer3::on_click_label_item(CCObject* sender)
{
	CCMenuItemLabel* item_label = dynamic_cast<CCMenuItemLabel*>(sender);
	if (NULL != item_label)
		item_label->setString("Clicked again!");
	m_sprite->setPosition(CCPointZero);
}