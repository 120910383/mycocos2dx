﻿#pragma once

enum test_type
{
	TEST_SPRITE = 0,
	TEST_LABEL,
	TESTS_COUNT,
};

const char* test_names[TESTS_COUNT] =
{
	"SpriteTest",
	"LabelTest",
};