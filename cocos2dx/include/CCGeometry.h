#pragma once

#include "ccMacros.h"

NS_CC_BEGIN;
//////////////////////////////////////////////////////////////////////////
// declaration of CCPoint
class CC_DLL CCPoint
{
public:
	float x;
	float y;

public:
	CCPoint();
	CCPoint(float x, float y);

public:
	static bool CCPointEqualToPoint(const CCPoint& point1, const CCPoint& point2);
};

//////////////////////////////////////////////////////////////////////////
// declaration of CCSize
class CC_DLL CCSize
{
public:
	float width;
	float height;

public:
	CCSize();
	CCSize(float width, float height);

public:
	static bool CCSizeEqualToSize(const CCSize& size1, const CCSize& size2);
};

//////////////////////////////////////////////////////////////////////////
// declaration of CCRect
class CC_DLL CCRect
{
public:
	CCPoint origin;
	CCSize  size;

public:
	CCRect();	
	CCRect(float x, float y, float width, float height);

public:
	//! return the leftmost x-value of 'rect'
	static float CCRectGetMinX(const CCRect& rect);

	//! return the rightmost x-value of 'rect'
	static float CCRectGetMaxX(const CCRect& rect);

	//! return the midpoint x-value of 'rect'
	static float CCRectGetMidX(const CCRect& rect);

	//! Return the bottommost y-value of `rect'
	static float CCRectGetMinY(const CCRect& rect);

	//! Return the topmost y-value of `rect'
	static float CCRectGetMaxY(const CCRect& rect);

	//! Return the midpoint y-value of `rect'
	static float CCRectGetMidY(const CCRect& rect);

	static bool CCRectEqualToRect(const CCRect& rect1, const CCRect& rect2);

	static bool CCRectContainsPoint(const CCRect& rect, const CCPoint& point);

	static bool CCRectIntersectsRect(const CCRect& rectA, const CCRect& rectB);
};

//////////////////////////////////////////////////////////////////////////
// macro for geometry
#define CCPointMake(x, y) CCPoint((x), (y))
#define CCSizeMake(width, height) CCSize((width), (height))
#define CCRectMake(x, y, width, height) CCRect((x), (y), (width), (height))

const CCPoint CCPointZero = CCPointMake(0,0);
const CCSize CCSizeZero = CCSizeMake(0,0);
const CCRect CCRectZero = CCRectMake(0,0,0,0);

NS_CC_END;