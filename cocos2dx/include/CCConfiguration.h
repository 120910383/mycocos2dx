#pragma once

#include "CCObject.h"
#include "CCGL.h"
#include "CCStdC.h"

NS_CC_BEGIN;

class CC_DLL CCConfiguration : public CCObject
{
public:
	static CCConfiguration* sharedConfiguration();

public:
	// OpenGL所支持的单张纹理的最大尺寸
	inline int getMaxTextureSize()
	{
		return m_nMaxTextureSize;
	}

	// 当前GPU是否支持NPOT(Non Power Of Two)尺寸的纹理
	// NPOT尺寸纹理相对于POT尺寸纹理有以下两个限制
	// 1.不能使用MipMaps特性
	// 2.只能使用GL_CLAMP_TO_EDGE类型来和GL_TEXTURE_WRAP_{S,T}搭配使用
	inline bool isSupportsNPOT()
	{
		return m_bSupportsNPOT;
	}

	// OpenGL是否支持BGRA8888纹理格式
	inline bool isSupportsBGRA8888()
	{
		return m_bSupportsBGRA8888;
	}

	bool checkForGLExtension(const std::string& searchName);

private:
	CCConfiguration();
	bool init();

private:
	GLint m_nMaxTextureSize;
	bool m_bSupportsNPOT;
	bool m_bSupportsBGRA8888;
	bool m_bInited;
	char* m_pGlExtensions;
};

NS_CC_END;