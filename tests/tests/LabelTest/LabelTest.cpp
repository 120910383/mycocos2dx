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

		// TODO...

		result = true;
	} while (0);

	return result;
}
