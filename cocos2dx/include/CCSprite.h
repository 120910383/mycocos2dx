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
	inline ccV3F_C4B_T2F_Quad getQuad() { return m_sQuad; }

	inline const CCRect& getTextureRect() { return m_obRect; }

	inline ccBlendFunc getBlendFunc() { return m_sBlendFunc; }
	inline void setBlendFunc(ccBlendFunc blendFunc) { m_sBlendFunc = blendFunc; }

public:
	static CCSprite* spriteWithFile(const char *pszFileName);
	static CCSprite* spriteWithFile(const char *pszFileName, const CCRect& rect);

public:
	CCSprite();
	virtual ~CCSprite();

	virtual bool init();

	bool initWithFile(const char *pszFilename);
	bool initWithFile(const char *pszFilename, const CCRect& rect);

	bool initWithTexture(CCTexture2D *pTexture);
	bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect);
	
	virtual void setTexture(CCTexture2D *texture);
	virtual CCTexture2D* getTexture();

	void setTextureRect(const CCRect& rect);

	void updateColor();

	void useSelfRender();

protected:
	void updateTextureCoords(const CCRect& rect);
	void updateBlendFunc();

protected:
	CCTexture2D* m_pobTexture;

	GLubyte m_nOpacity;
	ccColor3B m_sColor;
	ccColor3B m_sColorUnmodified;
	bool m_bOpacityModifyRGB;

	ccV3F_C4B_T2F_Quad m_sQuad;
	CCRect m_obRect;
	ccBlendFunc m_sBlendFunc;
};

NS_CC_END;