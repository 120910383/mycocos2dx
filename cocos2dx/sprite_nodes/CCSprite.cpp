#include "CCSprite.h"
#include "CCPointExtension.h"
#include "CCTexture2D.h"
#include "CCFileUtils.h"
#include "CCImage.h"

NS_CC_BEGIN;

void CCSprite::draw()
{
	CCNode::draw();

	// 暂不实现颜色渲染
	glDisableClientState(GL_COLOR_ARRAY);

	if (NULL != m_pobTexture)
	{
		glBindTexture(GL_TEXTURE_2D, m_pobTexture->getName());
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	GLfloat vertices[] = 
	{
		0, m_tContentSize.height, 0,
		0, 0, 0,
		m_tContentSize.width, m_tContentSize.height, 0,
		m_tContentSize.width, 0, 0,
	};
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	GLfloat coordinates[] = 
	{
		0, 1,
		0, 0,
		1, 1,
		1, 0,
	};
	glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glEnableClientState(GL_COLOR_ARRAY);
}

CCSprite* CCSprite::spriteWithFile(const char *pszFileName)
{
	CCSprite* pobSprite = new CCSprite();
	if (NULL != pobSprite && pobSprite->initWithFile(pszFileName))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

CCSprite::CCSprite()
	: m_pobTexture(NULL)
{

}

CCSprite::~CCSprite()
{
	CC_SAFE_RELEASE(m_pobTexture);
}

bool CCSprite::init()
{
	setTexture(NULL);

	setAnchorPoint(ccp(0.5f, 0.5f));

	return true;
}

bool CCSprite::initWithFile(const char *pszFilename)
{
	CCAssert(NULL != pszFilename, "");
	CCTexture2D* pTexture = NULL;

	// 根据文件名创建纹理对象
	{
		std::string fullpath = CCFileUtils::fullPathFromRelativePath(pszFilename);
		std::string lowerCase(pszFilename);
		for (unsigned int i = 0; i < lowerCase.length(); ++i)
		{
			lowerCase[i] = (char)tolower(lowerCase[i]);
		}

		do 
		{
			CCImage image;
			CCFileData data(fullpath.c_str(), "rb");
			unsigned long nSize = data.getSize();
			unsigned char* pBuffer = data.getBuffer();
			CC_BREAK_IF( !image.initWithImageData((void*)pBuffer, nSize, CCImage::kFmtPng));

			pTexture = new CCTexture2D;
			if (NULL != pTexture && pTexture->initWithImage(&image))
			{
				pTexture->autorelease();
			}
			else
			{
				CC_SAFE_DELETE(pTexture);
			}
		} while (0);
	}

	if (NULL != pTexture)
	{
		CCRect rect = CCRectZero;
		rect.size = pTexture->getContentSize();
		return initWithTexture(pTexture, rect);
	}

	return false;
}

bool CCSprite::initWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
	CCAssert(NULL != pTexture, "");
	init();
	setTexture(pTexture);
	setTextureRect(rect);

	return true;
}

bool CCSprite::initWithTexture(CCTexture2D *pTexture)
{
	CCAssert(NULL != pTexture, "");

	CCRect rect = CCRectZero;
	rect.size = pTexture->getContentSize();

	return initWithTexture(pTexture, rect);
}

void CCSprite::setTexture(CCTexture2D *texture)
{
	CC_SAFE_RELEASE(m_pobTexture);
	m_pobTexture = texture;
	if (NULL != texture)
	{
		texture->retain();
	}
}

CCTexture2D* CCSprite::getTexture()
{
	return m_pobTexture;
}

void CCSprite::setTextureRect(const CCRect& rect)
{
	setContentSize(rect.size);
}

NS_CC_END;