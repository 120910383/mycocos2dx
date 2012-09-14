#pragma once

#include "CCNode.h"
#include "ccTypes.h"

NS_CC_BEGIN;

class CCTexture2D;
class CC_DLL CCSprite : public CCNode
{
public:
	virtual void draw();

public:
	virtual GLubyte getOpacity();
	virtual void setOpacity(GLubyte opacity);

	virtual const ccColor3B& getColor();
	virtual void setColor(const ccColor3B& color3);

	virtual bool getIsOpacityModifyRGB();
	virtual void setIsOpacityModifyRGB(bool bVaule);

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
	GLubyte m_nOpacity;
	ccColor3B m_sColor;
	ccColor3B m_sColorUnmodified;
	bool m_bOpacityModifyRGB;
};

NS_CC_END;