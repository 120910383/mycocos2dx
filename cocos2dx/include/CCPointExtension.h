#pragma once

#include "CCGeometry.h"

NS_CC_BEGIN;

#define ccp(__X__,__Y__) cocos2d::CCPointMake((float)(__X__), (float)(__Y__))


static inline CCPoint ccpNeg(const CCPoint& v)
{
	return ccp(-v.x, -v.y);
}

static inline CCPoint ccpAdd(const CCPoint& v1, const CCPoint& v2)
{
	return ccp(v1.x + v2.x, v1.y + v2.y);
}

static inline CCPoint ccpSub(const CCPoint& v1, const CCPoint& v2)
{
	return ccp(v1.x - v2.x, v1.y - v2.y);
}

static inline CCPoint ccpMult(const CCPoint& v, const float s)
{
	return ccp(v.x*s, v.y*s);
}

NS_CC_END;