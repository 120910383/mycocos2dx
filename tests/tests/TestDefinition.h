#pragma once

#include "SpriteTest/SpriteTest.h"
#include "LabelTest/LabelTest.h"
#include "OpenGLTest/OpenGLTest.h"
#include "SchedulerTest/SchedulerTest.h"
#include "ActionTest/ActionTest.h"

enum test_type
{
	TEST_SPRITE = 0,
	TEST_LABEL,
	TEST_OPENGL,
	TEST_SCHEDULER,
	TEST_ACTION,
	TESTS_COUNT,
};

const char* test_names[TESTS_COUNT] =
{
	"SpriteTest",
	"LabelTest",
	"OpenGLTest",
	"SchedulerTest",
	"ActionTest",
};