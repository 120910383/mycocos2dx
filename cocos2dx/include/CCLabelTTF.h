#pragma once

#include "CCSprite.h"
#include "ccTypes.h"

NS_CC_BEGIN;

class CC_DLL CCLabelTTF : public CCSprite
{
public:
	static CCLabelTTF* labelWithString(const char* label, const char* fontName, float fontSize);
	static CCLabelTTF* labelWithString(const char* label, const CCSize& dimensions, CCTextAlignment alignment, const char* fontName, float fontSize);

public:
	CCLabelTTF();
	virtual ~CCLabelTTF();

	bool initWithString(const char* label, const char* fontName, float fontSize);
	bool initWithString(const char* label, const CCSize& dimensions, CCTextAlignment alignment, const char* fontName, float fontSize);

	virtual void setString(const char* label);
	virtual const char* getString();

protected:
	CCSize m_tDimensions;
	CCTextAlignment m_eAlignment;
	std::string* m_pFontName;
	float m_fFontSize;
	std::string* m_pString;
};

NS_CC_END;