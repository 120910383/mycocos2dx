#pragma once

#include "CCStdC.h"

#define CC_ASSERT(cond)                assert(cond)

#ifndef CCAssert
	#define CCAssert(cond, msg)         CC_ASSERT(cond)
#endif  // CCAssert

// namespace cocos2d {}
#define NS_CC_BEGIN                     namespace cocos2d {
#define NS_CC_END                       }
#define USING_NS_CC                     using namespace cocos2d

#define CC_SAFE_DELETE(p)                    if(p) { delete (p); (p) = 0; }
#define CC_SAFE_DELETE_ARRAY(p)       if(p) { delete[] (p); (p) = 0; }
#define CC_SAFE_FREE(p)                        if(p) { free(p); (p) = 0; }
#define CC_SAFE_RELEASE(p)                 if(p) { (p)->release(); }
#define CC_SAFE_RELEASE_NULL(p)       if(p) { (p)->release(); (p) = 0; }
#define CC_SAFE_RETAIN(p)                     if(p) { (p)->retain(); }
#define CC_BREAK_IF(cond)                      if(cond) break;

#define CC_DLL

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#undef CC_DLL
	#if defined(_USRDLL)
		#define CC_DLL     __declspec(dllexport)
	#else
		#define CC_DLL     __declspec(dllimport)
	#endif
#endif

#define CC_UNUSED_PARAM(unusedparam)   (void)unusedparam

#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define CC_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

// close warning "Conditional expression is constant"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#pragma warning (disable:4127)
#endif