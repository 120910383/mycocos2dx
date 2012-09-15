#pragma once

#include "CCObject.h"
#include "CCTexture2D.h"
#include "CCMutableDictionary.h"

NS_CC_BEGIN;

class CC_DLL CCTextureCache : public CCObject
{
public:
	CCTextureCache();
	virtual ~CCTextureCache();

	static CCTextureCache* sharedTextureCache();
	static void purgeSharedTextureCache();

	CCTextureCache* addImage(const char* path);

protected:
	CCMutableDictionary<std::string, CCTexture2D*>* m_pTextures;
};

NS_CC_END;