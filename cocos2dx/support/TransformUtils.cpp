#include "TransformUtils.h"
#include "CCAffineTransform.h"

NS_CC_BEGIN;

void CGAffineToGL(const CCAffineTransform* t, GLfloat* m)
{
	m[2] = m[3] = m[6] = m[7] = m[8] = m[9] = m[11] = m[14] = 0.0f;
	m[10] = m[15] = 1.0f;
	m[0] = t->a; m[4] = t->c; m[12] = t->tx;
	m[1] = t->b; m[5] = t->d; m[13] = t->ty;
}

void GLToCGAffine(const GLfloat* m, CCAffineTransform* t)
{
	t->a = m[0]; t->c = m[4]; t->tx = m[12];
	t->b = m[1]; t->d = m[5]; t->ty = m[13];
}

NS_CC_END;