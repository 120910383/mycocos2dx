#pragma once

#include "GLES/gl.h"

NS_CC_BEGIN;

// RGB颜色结构体(字节表示0-255)
typedef struct _ccColor3B
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
} ccColor3B;

static inline ccColor3B ccc3(GLubyte r, GLubyte g, GLubyte b)
{
	ccColor3B c = {r, g, b};
	return c;
}

static const ccColor3B ccWHITE = {255, 255, 255};
static const ccColor3B ccYELLOW = {255, 255, 0};
static const ccColor3B ccBLUE = {0, 0, 255};
static const ccColor3B ccGREEN = {0, 255, 0};
static const ccColor3B ccRED = {255, 0, 0};
static const ccColor3B ccMAGENTA = {255, 0, 255};
static const ccColor3B ccBLACK = {0, 0, 0};
static const ccColor3B ccORANGE = {255, 127, 0};
static const ccColor3B ccGRAY = {166, 166, 166};

// RGBA颜色结构体(字节表示0-255)
typedef struct _ccColor4B
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
} ccColor4B;

static inline ccColor4B ccc4(GLubyte r, GLubyte g, GLubyte b, GLubyte o)
{
	ccColor4B c = {r, g, b, o};
	return c;
}


// 二维顶点坐标结构体(浮点数表示无限制)
typedef struct _ccVertex2F
{
	GLfloat x;
	GLfloat y;
} ccVertex2F;

static inline ccVertex2F vertex2(float x, float y)
{
	ccVertex2F c = {x, y};
	return c;
}

// 三维顶点坐标结构体(浮点数表示无限制)
typedef struct _ccVertex3F
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
} ccVertex3F;

static inline ccVertex3F vertex3(float x, float y, float z)
{
	ccVertex3F c = {x, y, z};
	return c;
}

// UV纹理贴图坐标(浮点数表示0.0-1.0)
typedef struct _ccTex2F
{
	GLfloat u;
	GLfloat v;
} ccTex2F;

static inline ccTex2F tex2(float u, float v)
{
	ccTex2F t = {u, v};
	return t;
}

// 带有顶点坐标，纹理坐标，颜色的点结构体
typedef struct _ccV3F_C4B_T2F
{
	ccVertex3F vertices;
	ccColor4B colors;
	ccTex2F texCoords;
} ccV3F_C4B_T2F;

// 由ccV3F_C4B_T2F点结构体描述矩形四顶点的结构体
typedef struct _ccV3F_C4B_T2F_Quad
{
	ccV3F_C4B_T2F tl;
	ccV3F_C4B_T2F bl;
	ccV3F_C4B_T2F tr;
	ccV3F_C4B_T2F br;
} ccV3F_C4B_T2F_Quad;

// 颜色混合结构体
typedef struct _ccBlendFunc
{
	GLenum src;
	GLenum dst;
} ccBlendFunc;

NS_CC_END;