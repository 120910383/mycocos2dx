#pragma once

#define CC_PLATFORM_UNKNOWN 0
#define CC_PLATFORM_WIN32 1
#define CC_PLATFORM_LINUX 2

#define CC_TARGET_PLATFORM CC_PLATFORM_UNKNOWN

#if !CC_TARGET_PLATFORM && (defined(WIN32) && defined(_WINDOWS))
	#undef CC_TARGET_PLATFORM
	#define CC_TARGET_PLATFORM CC_PLATFORM_WIN32
#endif

#if !CC_TARGET_PLATFORM && defined(LINUX)
	#undef CC_TARGET_PLATFORM
	#define CC_TARGET_PLATFORM CC_PLATFORM_LINUX
#endif

#if !CC_TARGET_PLATFORM
	#error "Can not recognize the target platform, compling under a unsupported platform?"
#endif

//////////////////////////////////////////////////////////////////////////
// WIN32界面使用哪种窗口显示
//////////////////////////////////////////////////////////////////////////
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#ifndef CC_USE_GLFW_WINDOW
	#define CC_USE_GLFW_WINDOW 1		// 0使用win api显示窗口，1使用glfw库来显示
	#endif
#endif