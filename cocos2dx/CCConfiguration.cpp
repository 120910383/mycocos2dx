#include "CCConfiguration.h"

NS_CC_BEGIN;

CCConfiguration* CCConfiguration::sharedConfiguration()
{
	static CCConfiguration s_sharedConfiguration;
	if (!s_sharedConfiguration.m_bInited)
	{
		s_sharedConfiguration.init();
		s_sharedConfiguration.m_bInited = true;
	}

	return &s_sharedConfiguration;
}

CCConfiguration::CCConfiguration()
	: m_nMaxTextureSize(0)
	, m_bSupportsNPOT(false)
	, m_bSupportsBGRA8888(false)
	, m_bInited(false)
	, m_pGlExtensions(NULL)
{

}

bool CCConfiguration::init()
{
	m_pGlExtensions = (char*)glGetString(GL_EXTENSIONS);

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_nMaxTextureSize);

	m_bSupportsNPOT = checkForGLExtension("GL_APPLE_texture_2D_limited_npot");
	m_bSupportsBGRA8888 = checkForGLExtension("GL_IMG_texture_format_BGRA888");

	return true;
}

bool CCConfiguration::checkForGLExtension(const std::string& searchName)
{
	bool bRet = false;
	const char* kSearchName = searchName.c_str();

	if (NULL != m_pGlExtensions && NULL != strstr(m_pGlExtensions, kSearchName))
	{
		bRet = true;
	}

	return bRet;
}

NS_CC_END;