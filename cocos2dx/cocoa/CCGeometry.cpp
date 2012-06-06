#include "CCGeometry.h"

NS_CC_BEGIN;
//////////////////////////////////////////////////////////////////////////
// implementation of CCPoint
CCPoint::CCPoint()
{
	x = 0;
	y = 0;
}

CCPoint::CCPoint(float x, float y)
{
	this->x = x;
	this->y = y;
}

bool CCPoint::CCPointEqualToPoint(const CCPoint& point1, const CCPoint& point2)
{
	return ((point1.x == point2.x) && (point1.y == point2.y));
}

//////////////////////////////////////////////////////////////////////////
// implementation of CCSize
CCSize::CCSize(void)
{
	width = 0;
	height = 0;
}

CCSize::CCSize(float width, float height)
{
	this->width = width;
	this->height = height;
}

bool CCSize::CCSizeEqualToSize(const CCSize& size1, const CCSize& size2)
{
	return ((size1.width == size2.width) && (size1.height == size2.height));
}

//////////////////////////////////////////////////////////////////////////
// implementation of CCRect
CCRect::CCRect(void)
{
	origin.x = 0;
	origin.y = 0;

	size.width = 0;
	size.height = 0;
}

CCRect::CCRect(float x, float y, float width, float height)
{
	CCAssert(width >= 0 && height >= 0, "only support that the width and height > 0");
	origin.x = x;
	origin.y = y;
	size.width = width;
	size.height = height;
}

bool CCRect::CCRectEqualToRect(const CCRect& rect1, const CCRect& rect2)
{
	return (CCPoint::CCPointEqualToPoint(rect1.origin, rect2.origin)
		&& CCSize::CCSizeEqualToSize(rect1.size, rect2.size));
}

float CCRect::CCRectGetMaxX(const CCRect& rect)
{
	return rect.origin.x + rect.size.width;
}

float CCRect::CCRectGetMidX(const CCRect& rect)
{
	return (float)(rect.origin.x + rect.size.width / 2.0);
}

float CCRect::CCRectGetMinX(const CCRect& rect)
{
	return rect.origin.x;
}

float CCRect::CCRectGetMaxY(const CCRect& rect)
{
	return rect.origin.y + rect.size.height;
}

float CCRect::CCRectGetMidY(const CCRect& rect)
{
	return (float)(rect.origin.y + rect.size.height / 2.0);
}

float CCRect::CCRectGetMinY(const CCRect& rect)
{
	return rect.origin.y;
}

bool CCRect::CCRectContainsPoint(const CCRect& rect, const CCPoint& point)
{
	bool bRet = false;

	if (point.x >= CCRectGetMinX(rect) && point.x <= CCRectGetMaxX(rect)
		&& point.y >= CCRectGetMinY(rect) && point.y <= CCRectGetMaxY(rect))
	{
		bRet = true;
	}

	return bRet;
}

bool CCRect::CCRectIntersectsRect(const CCRect& rectA, const CCRect& rectB)
{
	return !(CCRectGetMaxX(rectA) < CCRectGetMinX(rectB)||
		CCRectGetMaxX(rectB) < CCRectGetMinX(rectA)||
		CCRectGetMaxY(rectA) < CCRectGetMinY(rectB)||
		CCRectGetMaxY(rectB) < CCRectGetMinY(rectA));
}

NS_CC_END;