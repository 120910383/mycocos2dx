#include "CCTextureCache.h"

NS_CC_BEGIN;

static CCTextureCache* g_sharedTextureCache;

CCTextureCache* CCTextureCache::sharedTextureCache()
{
	if (NULL == g_sharedTextureCache)
		g_sharedTextureCache = new CCTextureCache();

	return g_sharedTextureCache;
}

CCTextureCache::CCTextureCache()
{
	CCAssert(NULL == g_sharedTextureCache, "Attempted to allocate a second instance of a singleton.");

	m_pTextures = new CCMutableDictionary<std::string, CCTexture2D*>();
}

CCTextureCache::~CCTextureCache()
{
	CC_SAFE_RELEASE(m_pTextures);
}

void CCTextureCache::purgeSharedTextureCache()
{
	CC_SAFE_RELEASE_NULL(g_sharedTextureCache);
}

CCTextureCache* CCTextureCache::addImage(const char* path)
{
	CCAssert(NULL != path, "TextureCache: fileimage MUST not be NULL");

	return NULL;
}

NS_CC_END;