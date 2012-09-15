#include "CCSprite.h"
#include "CCPointExtension.h"
#include "CCTexture2D.h"
#include "CCFileUtils.h"
#include "CCImage.h"

NS_CC_BEGIN;

void CCSprite::draw()
{
	CCNode::draw();

	bool newBlend = m_sBlendFunc.src != GL_ONE || m_sBlendFunc.dst != GL_ONE_MINUS_SRC_ALPHA;
	if (newBlend)
	{
		glBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
	}

	if (NULL != m_pobTexture)
	{
		glBindTexture(GL_TEXTURE_2D, m_pobTexture->getName());
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;

	// 顶点数组vertex间隔存储
	int diff = offsetof(ccV3F_C4B_T2F, vertices);
	glVertexPointer(3, GL_FLOAT, kQuadSize, (void*)(offset + diff));

	// 颜色数组colors间隔存储
	diff = offsetof(ccV3F_C4B_T2F, colors);
	glColorPointer(4, GL_UNSIGNED_BYTE, kQuadSize, (void*)(offset + diff));

	// 纹理数组texcoords间隔存储
	diff = offsetof(ccV3F_C4B_T2F, texCoords);
	glTexCoordPointer(2, GL_FLOAT, kQuadSize, (void*)(offset + diff));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (newBlend)
	{
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}
}

GLubyte CCSprite::getOpacity()
{
	return m_nOpacity;
}

void CCSprite::setOpacity(GLubyte opacity)
{
	m_nOpacity = opacity;

	if (m_bOpacityModifyRGB)
	{
		m_sColor.r = m_sColorUnmodified.r * m_nOpacity / 255;
		m_sColor.g = m_sColorUnmodified.g * m_nOpacity / 255;
		m_sColor.b = m_sColorUnmodified.b * m_nOpacity / 255;
	}

	updateColor();
}

const ccColor3B& CCSprite::getColor()
{
	if (m_bOpacityModifyRGB)
	{
		return m_sColorUnmodified;
	}

	return m_sColor;
}

void CCSprite::setColor(const ccColor3B& color3)
{
	m_sColor = m_sColorUnmodified = color3;

	if (m_bOpacityModifyRGB)
	{
		m_sColor.r = m_sColorUnmodified.r * m_nOpacity / 255;
		m_sColor.g = m_sColorUnmodified.g * m_nOpacity / 255;
		m_sColor.b = m_sColorUnmodified.b * m_nOpacity / 255;
	}

	updateColor();
}

bool CCSprite::getIsOpacityModifyRGB()
{
	return m_bOpacityModifyRGB;
}

void CCSprite::setIsOpacityModifyRGB(bool bValue)
{
	m_bOpacityModifyRGB = bValue;
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

CCSprite* CCSprite::spriteWithFile(const char *pszFileName, const CCRect& rect)
{
	CCSprite* pobSprite = new CCSprite();
	if (NULL != pobSprite && pobSprite->initWithFile(pszFileName, rect))
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
	useSelfRender();

	m_bOpacityModifyRGB = true;
	m_nOpacity = 255;
	m_sColor = m_sColorUnmodified = ccWHITE;

	m_sBlendFunc.src = GL_ONE;
	m_sBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;

	setTexture(NULL);

	memset(&m_sQuad, 0, sizeof(m_sQuad));

	setAnchorPoint(ccp(0.5f, 0.5f));

	updateColor();

	setTextureRect(CCRectZero);

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

bool CCSprite::initWithFile(const char *pszFilename, const CCRect& rect)
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
		return initWithTexture(pTexture, rect);
	}

	return false;
}

bool CCSprite::initWithTexture(CCTexture2D *pTexture)
{
	CCAssert(NULL != pTexture, "");

	CCRect rect = CCRectZero;
	rect.size = pTexture->getContentSize();

	return initWithTexture(pTexture, rect);
}

bool CCSprite::initWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
	CCAssert(NULL != pTexture, "");
	init();
	setTexture(pTexture);
	setTextureRect(rect);

	return true;
}

void CCSprite::setTexture(CCTexture2D *texture)
{
	CC_SAFE_RELEASE(m_pobTexture);
	m_pobTexture = texture;
	if (NULL != texture)
	{
		texture->retain();
	}

	updateBlendFunc();
}

CCTexture2D* CCSprite::getTexture()
{
	return m_pobTexture;
}

void CCSprite::setTextureRect(const CCRect& rect)
{
	m_obRect = rect;

	setContentSize(rect.size);
	updateTextureCoords(m_obRect);

	useSelfRender();
}

void CCSprite::updateColor()
{
	ccColor4B color4 = { m_sColor.r, m_sColor.g, m_sColor.b, m_nOpacity };

	m_sQuad.bl.colors = color4;
	m_sQuad.br.colors = color4;
	m_sQuad.tl.colors = color4;
	m_sQuad.tr.colors = color4;
}

void CCSprite::useSelfRender()
{
	float x1 = 0;
	float y1 = 0;
	float x2 = x1 + m_obRect.size.width;
	float y2 = y1 + m_obRect.size.height;

	m_sQuad.bl.vertices = vertex3(x1, y1, 0);
	m_sQuad.br.vertices = vertex3(x2, y1, 0);
	m_sQuad.tl.vertices = vertex3(x1, y2, 0);
	m_sQuad.tr.vertices = vertex3(x2, y2, 0);
}

void CCSprite::updateTextureCoords(const CCRect& rect)
{
	if (NULL == m_pobTexture)
	{
		return;
	}

	float tex_width = (float)m_pobTexture->getPixelsWide();
	float tex_height = (float)m_pobTexture->getPixelsHigh();

	//////////////////////////////////////////////////////////////////////////方案一：详细分析
	float left_o, right_o, top_o, bottom_o;		// 相对于图片rect，原点左上角
	float left_t, right_t, top_t, bottom_t;			// 相对于图片rect，原点左下角
	float left_s, right_s, top_s, bottom_s;		// 相对于纹理的比例，即uv值

	// 1.由于要计算rect的坐标转换，因此将rect用四个角坐标来表示，注意的是原点为左上角的坐标系
	left_o = rect.origin.x;
	right_o = rect.origin.x + rect.size.width;
	top_o = rect.origin.y;
	bottom_o = rect.origin.y + rect.size.height;

	// 2.由于Opengl ES在载入图片时，会让纹理和真实图片上下颠倒（原因不明，待查TODO...）
	left_t = left_o;
	right_t = right_o;
	top_t = tex_height - bottom_o;
	bottom_t = tex_height - top_o;

	// 3.由于Opengl ES的纹理坐标原点为左下角，这里进行转换
	left_t = left_t;
	right_t = right_t;
	top_t = tex_height - top_t;
	bottom_t = tex_height - bottom_t;

	// 4.计算纹理贴图坐标(UV坐标)，是一个相对于纹理尺寸的比例值
	left_s = left_t / tex_width;
	right_s = right_t / tex_width;
	top_s = top_t / tex_height;
	bottom_s = bottom_t / tex_height;

	// 5.由于纹理和真实图片颠倒，因此UV坐标的V坐标也应该交换
	m_sQuad.bl.texCoords.u = left_s;
	m_sQuad.bl.texCoords.v = top_s;
	m_sQuad.br.texCoords.u = right_s;
	m_sQuad.br.texCoords.v = top_s;
	m_sQuad.tl.texCoords.u = left_s;
	m_sQuad.tl.texCoords.v = bottom_s;
	m_sQuad.tr.texCoords.u = right_s;
	m_sQuad.tr.texCoords.v = bottom_s;

	//////////////////////////////////////////////////////////////////////////方案二：归纳整理
	//float left, right, top, bottom;
	//left = rect.origin.x / tex_width;
	//right = left + rect.size.width / tex_width;
	//top = rect.origin.y / tex_height;
	//bottom = top + rect.size.height / tex_height;

	//m_sQuad.bl.texCoords.u = left;
	//m_sQuad.bl.texCoords.v = bottom;
	//m_sQuad.br.texCoords.u = right;
	//m_sQuad.br.texCoords.v = bottom;
	//m_sQuad.tl.texCoords.u = left;
	//m_sQuad.tl.texCoords.v = top;
	//m_sQuad.tr.texCoords.u = right;
	//m_sQuad.tr.texCoords.v = top;
}

void CCSprite::updateBlendFunc()
{
	if (NULL == m_pobTexture/* || unpremultiplied_alpha*/)
	{
		m_sBlendFunc.src = GL_SRC_ALPHA;
		m_sBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
		setIsOpacityModifyRGB(false);
	}
	else
	{
		m_sBlendFunc.src = GL_ONE;
		m_sBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
		setIsOpacityModifyRGB(true);
	}
}

NS_CC_END;