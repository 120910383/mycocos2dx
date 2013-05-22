#pragma once

#include "CCPlatformMacros.h"
#include "CCStdC.h"

#define CC_ASSERT(cond)                assert(cond)

#ifndef CCAssert
	#define CCAssert(cond, msg)         CC_ASSERT(cond)
#endif  // CCAssert

#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define CC_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180
