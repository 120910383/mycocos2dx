﻿#include "SpriteTest/SpriteTest.h"

//////////////////////////////////////////////////////////////////////////
const char* SpriteOpacityTest::title()
{
	return "Sprite Opacity";
}

TestBasicLayer* SpriteOpacityTest::get_last_layer()
{
	return NULL;
}

TestBasicLayer* SpriteOpacityTest::get_cur_layer()
{
	return SpriteOpacityTest::create_test();
}

TestBasicLayer* SpriteOpacityTest::get_next_layer()
{
	return SpriteColorTest::create_test();
}

bool SpriteOpacityTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !TestBasicLayer::init() );

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();

		CCSprite* sprite1 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite1);
		sprite1->setPosition(ccp(win_size.width / 2 - 200, win_size.height / 2));
		sprite1->setOpacity(255);
		addChild(sprite1);

		CCSprite* sprite2 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite2);
		sprite2->setPosition(ccp(win_size.width / 2 - 100, win_size.height / 2));
		sprite2->setOpacity(200);
		addChild(sprite2);

		CCSprite* sprite3 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite3);
		sprite3->setPosition(ccp(win_size.width / 2, win_size.height / 2));
		sprite3->setOpacity(150);
		addChild(sprite3);

		CCSprite* sprite4 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite4);
		sprite4->setPosition(ccp(win_size.width / 2 + 100, win_size.height / 2));
		sprite4->setOpacity(100);
		addChild(sprite4);

		CCSprite* sprite5 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite5);
		sprite5->setPosition(ccp(win_size.width / 2 + 200, win_size.height / 2));
		sprite5->setOpacity(50);
		addChild(sprite5);

		scheduleUpdate();
		result = true;
	} while (0);

	return result;
}

void SpriteOpacityTest::update()
{
	static double angle = 0.02f;
	CCSize win_size = CCDirector::sharedDirector()->getWinSize();
	const int label_tag = 100;
	CCLabelTTF* test_label = dynamic_cast<CCLabelTTF*>(getChildByTag(label_tag));
	if (NULL == test_label)
	{
		test_label = CCLabelTTF::labelWithString("UpdateMoveTest", "", 32);
		if (NULL != test_label)
		{
			test_label->setPosition(ccp(win_size.width / 4, win_size.height / 4));
			addChild(test_label, 0, label_tag);
		}
	}

	if (NULL != test_label)
	{
		CCPoint pos = ccpSub(test_label->getPosition(), ccp(win_size.width / 2, win_size.height / 2));
		pos = ccp(ccpDot(pos, ccp(cos(angle), -sin(angle))), ccpDot(pos, ccp(sin(angle), cos(angle))));
		test_label->setPosition(ccpAdd(pos, ccp(win_size.width / 2, win_size.height / 2)));
	}
}

//////////////////////////////////////////////////////////////////////////
const char* SpriteColorTest::title()
{
	return "Sprite Color";
}

TestBasicLayer* SpriteColorTest::get_last_layer()
{
	return SpriteOpacityTest::create_test();;
}

TestBasicLayer* SpriteColorTest::get_cur_layer()
{
	return SpriteColorTest::create_test();
}

TestBasicLayer* SpriteColorTest::get_next_layer()
{
	return NULL;
}

bool SpriteColorTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !TestBasicLayer::init() );

		CCSize win_size = CCDirector::sharedDirector()->getWinSize();

		CCSprite* sprite1 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite1);
		sprite1->setPosition(ccp(win_size.width / 2 - 200, win_size.height / 2));
		sprite1->setColor(ccRED);
		addChild(sprite1);

		CCSprite* sprite2 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite2);
		sprite2->setPosition(ccp(win_size.width / 2 - 100, win_size.height / 2));
		sprite2->setColor(ccGREEN);
		addChild(sprite2);

		CCSprite* sprite3 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite3);
		sprite3->setPosition(ccp(win_size.width / 2, win_size.height / 2));
		sprite3->setColor(ccBLUE);
		addChild(sprite3);

		CCSprite* sprite4 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite4);
		sprite4->setPosition(ccp(win_size.width / 2 + 100, win_size.height / 2));
		sprite4->setColor(ccc3(255, 255, 0));
		addChild(sprite4);

		CCSprite* sprite5 = CCSprite::spriteWithFile("Images/man_ini.png");
		CC_BREAK_IF( NULL == sprite5);
		sprite5->setPosition(ccp(win_size.width / 2 + 200, win_size.height / 2));
		sprite5->setColor(ccc3(0, 255, 255));
		addChild(sprite5);

		result = true;
	} while (0);

	return result;
}