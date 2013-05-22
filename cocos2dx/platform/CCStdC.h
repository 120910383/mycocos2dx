#pragma once

#include "CCConfig.h"

// common include
#include <cstdio>					// use NULL macro
#include <cstdlib>					// use malloc free
#include <cassert>
#include <string>
#include <cstring>					// memset on linux need include string.h
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <ctime>

// win32 need include
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include <Windows.h>

	struct timezone
	{
		int tz_minuteswest;
		int tz_dsttime;
	};
	int gettimeofday(struct timeval*, struct timezone*);

#endif

// linux need include
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	// todo...
#endif