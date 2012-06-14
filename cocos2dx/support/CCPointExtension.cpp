#include "CCPointExtension.h"

NS_CC_BEGIN;

float CC_DLL ccpLength(const CCPoint& v)
{
	return sqrtf(ccpLengthSQ(v));
}

float CC_DLL ccpDistance(const CCPoint& v1, const CCPoint& v2)
{
	return ccpLength(ccpSub(v1, v2));
}

CCPoint CC_DLL ccpNormalize(const CCPoint& v)
{
	return ccpMult(v, 1.0f/ccpLength(v));
}

NS_CC_END;