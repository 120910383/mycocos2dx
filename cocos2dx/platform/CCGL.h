#pragma once

#include "CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#if CC_USE_GLFW_WINDOW
		#include <Windows.h>
		#include "glfw.h"		// GL/gl.h 已经包含在内了
		#define ccglOrtho glOrtho
		#define ccglClearDepth glClearDepth
		#define ccglTranslate glTranslated
		#define GL_CLAMP_TO_EDGE GL_CLAMP
		#pragma comment(lib, "GLFW.lib")
		#pragma comment(lib, "Opengl32.lib")
	#else
		#include "GLES/gl.h"
		#include "EGL/egl.h"
		#define ccglOrtho glOrthof
		#define ccglClearDepth glClearDepthf
		#define ccglTranslate glTranslatef
		#pragma comment(lib, "libgles_cm.lib")
		#pragma comment(lib, "libEGL.lib")
	#endif
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	#include <GL/gl.h>
	#define ccglOrtho glOrtho
	#define ccglClearDepth glClearDepth
	#define ccglTranslate glTranslated
#endif
