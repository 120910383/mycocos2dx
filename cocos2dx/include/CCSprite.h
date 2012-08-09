#pragma once

#include "CCNode.h"

NS_CC_BEGIN;

class CCTexture2D;
class CC_DLL CCSprite : public CCNode
{
public:
	virtual void draw();

public:
	static CCSprite* spriteWithFile(const char *pszFileName);

public:
	CCSprite();
	virtual ~CCSprite();
	virtual bool init();
	bool initWithFile(const char *pszFilename);
	bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect);
	bool initWithTexture(CCTexture2D *pTexture);
	virtual void setTexture(CCTexture2D *texture);
	virtual CCTexture2D* getTexture();
	void setTextureRect(const CCRect& rect);

protected:
	CCTexture2D* m_pobTexture;
};

NS_CC_END;