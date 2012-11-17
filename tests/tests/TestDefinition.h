#pragma once

#include "SpriteTest/SpriteTest.h"
#include "LabelTest/LabelTest.h"
#include "OpenGLTest/OpenGLTest.h"

enum test_type
{
	TEST_SPRITE = 0,
	TEST_LABEL,
	TEST_OPENGL,
	TESTS_COUNT,
};

const char* test_names[TESTS_COUNT] =
{
	"SpriteTest",
	"LabelTest",
	"OpenGLTest",
};