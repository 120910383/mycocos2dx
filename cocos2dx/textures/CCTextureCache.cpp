#include "CCTextureCache.h"
#include "CCFileUtils.h"
#include "CCImage.h"

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

CCTexture2D* CCTextureCache::addImage(const char* path)
{
	CCAssert(NULL != path, "TextureCache: fileimage MUST not be NULL");

	CCTexture2D* texture = NULL;

	std::string pathKey = CCFileUtils::fullPathFromRelativePath(path);
	texture = m_pTextures->objectForKey(pathKey);

	std::string fullpath = pathKey;
	if (NULL == texture)
	{
		std::string lowerCase(path);
		for (unsigned int i = 0; i < lowerCase.length(); ++i)
		{
			lowerCase[i] = (char)tolower(lowerCase[i]);
		}

		do 
		{
			if (std::string::npos != lowerCase.find(".pvr"))
			{
				CCAssert(false, "");		//TODO...PVR文件解析
			}
			else if (std::string::npos != lowerCase.find(".jpg") || std::string::npos != lowerCase.find(".jpeg"))
			{
				CCAssert(false, "");		//TODO...JPG文件解析
			}
			else
			{
				CCImage image;
				CCFileData data(fullpath.c_str(), "rb");
				unsigned long nSize = data.getSize();
				unsigned char* pBuffer = data.getBuffer();
				CC_BREAK_IF(!image.initWithImageData((void*)pBuffer, nSize, CCImage::kFmtPng));

				texture = new CCTexture2D();

				if (NULL != texture && texture->initWithImage(&image))
				{
					m_pTextures->setObject(texture, pathKey);
					texture->autorelease();
				}
				else
				{
					// CCLOG信息输出TODO...
					CC_SAFE_DELETE(texture);
					CCAssert(false, "Couldn't add image!");
				}
			}
		} while (0);
	}

	return texture;
}

void CCTextureCache::removeUnusedTextures()
{
	std::vector<std::string> keys = m_pTextures->allKeys();
	std::vector<std::string>::iterator iter;
	for (iter = keys.begin(); iter != keys.end(); ++iter)
	{
		CCTexture2D* value = m_pTextures->objectForKey(*iter);
		if (value->retainCount() == 1)
		{
			m_pTextures->removeObjectForKey(*iter);
		}
	}
}

NS_CC_END;