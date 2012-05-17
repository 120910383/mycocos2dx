#pragma once

#include <assert.h>
#define CC_ASSERT(cond)                assert(cond)

#ifndef CCAssert
#define CCAssert(cond, msg)         CC_ASSERT(cond)
#endif  // CCAssert
