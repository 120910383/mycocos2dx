#pragma once

#include "CCGeometry.h"

NS_CC_BEGIN;

/*******************************************************************
// 仿射变换：几何中，一个向量空间进行一次线性变换并接上一个平移，变换为另一个向量空间，这个过程被称为仿射变换或者仿射映射。
   一个对向量x平移向量b，与旋转放大缩小矩阵A的仿射变换为 向量y = 矩阵A 乘以 向量x 加 平移向量b
// 二维仿射变换具体公式如下 t为结构体CCAffineTransform实例，xy是待变换点，x'y'是变换后的点
|  t.a  t.c  t.tx  |  | x |      | t.a * x + t.c * y + t.tx |     | x' |
|  t.b  t.d  t.ty  |  | y |  =  | t.b * x + t.d * y + t.ty |  = | y' |
|   0    0    1    |  | 1 |      |                1               |     | 1 |

// 平移变换矩阵
|  1     0     tx  |
|  0     1     ty  |
|  0     0     1   |

// 缩放变换矩阵
|  sx   0     0   |
|   0   sy    0   |
|   0    0     1   |

// 错切变换矩阵
|  1   shx   0   |
|shy   1     0   |
|  0     0    1   |

// 旋转变换矩阵
|  cos(theta)    -sin(theta)    x-x*cos(theta)+y*sin(theta)  |
|  sin(theta)    cos(theta)     y-x*sin(theta)+y*cos(theta)  |
|        0                0                              1                    |
*******************************************************************/
struct CCAffineTransform {
	float a, b, c, d;
	float tx, ty;
};

CC_DLL CCAffineTransform __CCAffineTransformMake(float a, float b, float c, float d, float tx, float ty);
#define CCAffineTransformMake __CCAffineTransformMake

CC_DLL CCPoint __CCPointApplyAffineTransform(const CCPoint& point, const CCAffineTransform& t);
#define CCPointApplyAffineTransform __CCPointApplyAffineTransform

CC_DLL CCSize __CCSizeApplyAffineTransform(const CCSize& size, const CCAffineTransform& t);
#define CCSizeApplyAffineTransform __CCSizeApplyAffineTransform

CC_DLL CCAffineTransform CCAffineTransformMakeIdentity();
CC_DLL CCRect CCRectApplyAffineTransform(const CCRect& rect, const CCAffineTransform& t);

CC_DLL CCAffineTransform CCAffineTransformTranslate(const CCAffineTransform& t, float tx, float ty);
CC_DLL CCAffineTransform CCAffineTransformRotate(const CCAffineTransform& t, float angle);
CC_DLL CCAffineTransform CCAffineTransformScale(const CCAffineTransform& t, float sx, float sy);
CC_DLL CCAffineTransform CCAffineTransformConcat(const CCAffineTransform& t1, const CCAffineTransform& t2);
CC_DLL CCAffineTransform CCAffineTransformInvert(const CCAffineTransform& t);

CC_DLL bool CCAffineTransformEqualToTransform(const CCAffineTransform& t1, const CCAffineTransform& t2);

extern const CCAffineTransform CCAffineTransformIdentity;
NS_CC_END;