#include "CCAffineTransform.h"
#include "CCStdC.h"
using namespace std;

NS_CC_BEGIN;

CCAffineTransform __CCAffineTransformMake(float a, float b, float c, float d, float tx, float ty)
{
	CCAffineTransform t;
	t.a = a; t.b = b; t.c = c; t.d = d; t.tx = tx; t.ty = ty;
	return t;
}

CCPoint __CCPointApplyAffineTransform(const CCPoint& point, const CCAffineTransform& t)
{
	CCPoint p;
	p.x = t.a * point.x + t.c * point.y + t.tx;
	p.y = t.b * point.x + t.d * point.y + t.ty;
	return p;
}

CCSize __CCSizeApplyAffineTransform(const CCSize& size, const CCAffineTransform& t)
{
	CCSize s;
	s.width = t.a * size.width + t.c * size.height;
	s.height = t.b * size.width + t.d * size.height;
	return s;
}

CCAffineTransform CCAffineTransformMakeIdentity()
{
	return __CCAffineTransformMake(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

extern const CCAffineTransform CCAffineTransformIdentity = CCAffineTransformMakeIdentity();

CCRect CCRectApplyAffineTransform(const CCRect& rect, const CCAffineTransform& t)
{
	float top = CCRect::CCRectGetMinY(rect);
	float left = CCRect::CCRectGetMinX(rect);
	float right = CCRect::CCRectGetMaxX(rect);
	float bottom = CCRect::CCRectGetMaxY(rect);

	CCPoint topLeft = CCPointApplyAffineTransform(CCPointMake(left, top), t);
	CCPoint topRight = CCPointApplyAffineTransform(CCPointMake(right, top), t);
	CCPoint bottomLeft = CCPointApplyAffineTransform(CCPointMake(left, bottom), t);
	CCPoint bottomRight = CCPointApplyAffineTransform(CCPointMake(right, bottom), t);

	float minX = min(min(topLeft.x, topRight.x), min(bottomLeft.x, bottomRight.x));
	float maxX = max(max(topLeft.x, topRight.x), max(bottomLeft.x, bottomRight.x));
	float minY = min(min(topLeft.y, topRight.y), min(bottomLeft.y, bottomRight.y));
	float maxY = max(max(topLeft.y, topRight.y), max(bottomLeft.y, bottomRight.y));

	return CCRectMake(minX, minY, (maxX - minX), (maxY - minY));
}

CCAffineTransform CCAffineTransformTranslate(const CCAffineTransform& t, float tx, float ty)
{
	return __CCAffineTransformMake(t.a, t.b, t.c, t.d, t.tx + t.a * tx + t.c * ty, t.ty + t.b * tx + t.d * ty);
}

CCAffineTransform CCAffineTransformRotate(const CCAffineTransform& t, float angle)
{
	float fSin = sin(angle);
	float fCos = cos(angle);

	return __CCAffineTransformMake(t.a * fCos + t.c * fSin,
		t.b * fCos + t.d * fSin,
		t.c * fCos - t.a * fSin,
		t.d * fCos - t.b * fSin,
		t.tx,
		t.ty);
}

CCAffineTransform CCAffineTransformScale(const CCAffineTransform& t, float sx, float sy)
{
	return __CCAffineTransformMake(t.a * sx, t.b * sx, t.c * sy, t.d * sy, t.tx, t.ty);
}

CCAffineTransform CCAffineTransformConcat(const CCAffineTransform& t1, const CCAffineTransform& t2)
{
	return __CCAffineTransformMake(t1.a * t2.a + t1.b * t2.c, t1.a * t2.b + t1.b * t2.d, //a,b
		t1.c * t2.a + t1.d * t2.c, t1.c * t2.b + t1.d * t2.d, //c,d
		t1.tx * t2.a + t1.ty * t2.c + t2.tx, t1.tx * t2.b + t1.ty * t2.d + t2.ty //tx,ty
		);
}

CCAffineTransform CCAffineTransformInvert(const CCAffineTransform& t)
{
	float determinant = 1 / (t.a * t.d - t.b * t.c);

	return __CCAffineTransformMake(determinant * t.d, -determinant * t.b, //a,b
		-determinant * t.c, determinant * t.a, //c,d
		determinant * (t.c * t.ty - t.d * t.tx), determinant * (t.b * t.tx - t.a * t.ty) //tx,ty
		);
}

bool CCAffineTransformEqualToTransform(const CCAffineTransform& t1, const CCAffineTransform& t2)
{
	return (t1.a == t2.a && t1.b == t2.b && t1.c == t2.c && t1.d == t2.d && t1.tx == t2.tx && t1.ty == t2.ty);
}

NS_CC_END;