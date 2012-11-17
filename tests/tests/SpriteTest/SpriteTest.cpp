#include "SpriteTest/SpriteTest.h"

//////////////////////////////////////////////////////////////////////////
const char* SpriteOpacityTest::title()
{
	return "Sprite Opacity";
}

TestBasicLayer* SpriteOpacityTest::get_last_layer()
{
	return NULL;
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

		// TODO...

		result = true;
	} while (0);

	return result;
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

		// TODO...

		result = true;
	} while (0);

	return result;
}