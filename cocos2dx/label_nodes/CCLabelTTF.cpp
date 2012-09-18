#include "CCLabelTTF.h"
#include "CCTexture2D.h"

NS_CC_BEGIN;

CCLabelTTF::CCLabelTTF()
	: m_eAlignment(CCTextAlignmentCenter)
	, m_pFontName(NULL)
	, m_fFontSize(0.0f)
	, m_pString(NULL)
{

}

CCLabelTTF::~CCLabelTTF()
{
	CC_SAFE_DELETE(m_pFontName);
	CC_SAFE_DELETE(m_pString);
}

CCLabelTTF* CCLabelTTF::labelWithString(const char* label, const char* fontName, float fontSize)
{
	CCLabelTTF* pRet = new CCLabelTTF();
	if (NULL != pRet && pRet->initWithString(label, fontName, fontSize))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

CCLabelTTF* CCLabelTTF::labelWithString(const char* label, const cocos2d::CCSize& dimensions, cocos2d::CCTextAlignment alignment, const char* fontName, float fontSize)
{
	CCLabelTTF* pRet = new CCLabelTTF();
	if (NULL != pRet && pRet->initWithString(label, dimensions, alignment, fontName, fontSize))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CCLabelTTF::initWithString(const char* label, const char* fontName, float fontSize)
{
	CCAssert(NULL != label, "");
	if (CCSprite::init())
	{
		m_tDimensions = CCSizeZero;

		CC_SAFE_DELETE(m_pFontName);
		m_pFontName = new std::string(fontName);

		m_fFontSize = fontSize;
		this->setString(label);
		return true;
	}
	return false;
}

bool CCLabelTTF::initWithString(const char* label, const cocos2d::CCSize& dimensions, cocos2d::CCTextAlignment alignment, const char* fontName, float fontSize)
{
	CCAssert(NULL != label, "");
	if (CCSprite::init())
	{
		m_tDimensions = dimensions;
		m_eAlignment = alignment;

		CC_SAFE_DELETE(m_pFontName);
		m_pFontName = new std::string(fontName);

		m_fFontSize = fontSize;
		this->setString(label);
		return true;
	}
	return false;
}

void CCLabelTTF::setString(const char* label)
{
	CC_SAFE_DELETE(m_pString);
	m_pString = new std::string(label);
	
	CCTexture2D* texture = NULL;
	if (CCSize::CCSizeEqualToSize(m_tDimensions, CCSizeZero))
	{
		texture = new CCTexture2D();
		texture->initWithString(label, m_pFontName->c_str(), m_fFontSize);
	}
	else
	{
		texture = new CCTexture2D();
		texture->initWithString(label, m_tDimensions, m_eAlignment, m_pFontName->c_str(), m_fFontSize);
	}
	this->setTexture(texture);
	texture->release();

	CCRect rect = CCRectZero;
	rect.size = m_pobTexture->getContentSize();
	this->setTextureRect(rect);
}

const char* CCLabelTTF::getString()
{
	return m_pString->c_str();
}

NS_CC_END;