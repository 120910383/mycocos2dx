#pragma once

/****************************************************************************************************************
	CCPoint相关扩展方法是基于开源物理引擎Chipmunk的cpVect文件创建的
	因此该文件中部分关于点的计算实际上是对原点为起点操作点为终点的向量
	的计算，诸如点积向量积等，因此使用时按向量理解即可
****************************************************************************************************************/

#include "CCGeometry.h"
#include <math.h>

NS_CC_BEGIN;

#define ccp(__X__,__Y__) cocos2d::CCPointMake((float)(__X__), (float)(__Y__))

// 返回关于原点的对称点(向量)
static inline CCPoint ccpNeg(const CCPoint& v)
{
	return ccp(-v.x, -v.y);
}

// 返回两点(向量)相加后的点(向量)
static inline CCPoint ccpAdd(const CCPoint& v1, const CCPoint& v2)
{
	return ccp(v1.x + v2.x, v1.y + v2.y);
}

// 返回两点(向量)相减后的点(向量)
static inline CCPoint ccpSub(const CCPoint& v1, const CCPoint& v2)
{
	return ccp(v1.x - v2.x, v1.y - v2.y);
}

// 返回指定点(向量)乘以给定因子后的点(向量)
static inline CCPoint ccpMult(const CCPoint& v, const float s)
{
	return ccp(v.x*s, v.y*s);
}

// 返回两点间中点坐标(向量之和的一半)
static inline CCPoint ccpMidpoint(const CCPoint& v1, const CCPoint& v2)
{
	return ccpMult(ccpAdd(v1, v2), 0.5f);
}

// 返回两点的点积(向量的数量积)
static inline float ccpDot(const CCPoint& v1, const CCPoint& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

// 返回两点的叉积(向量的向量积)
static inline float ccpCross(const CCPoint& v1, const CCPoint& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

// 返回指定向量逆时针旋转90度后的旋转向量(垂直向量)
static inline CCPoint ccpPerp(const CCPoint& v)
{
	return ccp(-v.y, v.x);
}

// 返回指定向量顺时针旋转90度后的旋转向量(垂直向量)
static inline CCPoint ccpRPerp(const CCPoint& v)
{
	return ccp(v.y, -v.x);
}

// 返回向量v1在向量v2上的投影向量
static inline CCPoint ccpProject(const CCPoint& v1, const CCPoint& v2)
{
	return ccpMult(v2, ccpDot(v1, v2)/ccpDot(v2, v2));
}

// 返回指定向量的平方长度
static inline float ccpLengthSQ(const CCPoint& v)
{
	return ccpDot(v, v);
}

// 返回指定向量的长度
float CC_DLL ccpLength(const CCPoint& v);

// 返回两点间距离(向量差的长度)
float CC_DLL ccpDistance(const CCPoint& v1, const CCPoint& v2);

// 返回指定向量的标准向量
CCPoint CC_DLL ccpNormalize(const CCPoint& v);

NS_CC_END;