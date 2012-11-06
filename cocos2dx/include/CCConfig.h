#pragma once

#include "CCPlatformConfig.h"

// WIN32下是否使用GLFW来管理窗口
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#ifndef CC_USE_GLFW_WINDOW
		#define CC_USE_GLFW_WINDOW 1
	#endif
#endif