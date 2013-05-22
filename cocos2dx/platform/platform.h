#pragma once

#include "CCPlatformMacros.h"

NS_CC_BEGIN;

struct CC_DLL cc_timeval
{
	long tv_sec;		// seconds
	long tv_usec;	// microseconds
};

class CC_DLL CCTime
{
public:
	static int gettimeofdayCocos2d(struct cc_timeval* tp, void* tzp);
};

NS_CC_END;