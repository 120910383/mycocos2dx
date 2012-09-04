#pragma once

#include "ccMacros.h"
#include "GLES/gl.h"

NS_CC_BEGIN;

struct CCAffineTransform;

// 仿射结构体和OpenGL矩阵数组间的相互转换
void CGAffineToGL(const CCAffineTransform* t, GLfloat* m);
void GLToCGAffine(const GLfloat* m, CCAffineTransform* t);

NS_CC_END;