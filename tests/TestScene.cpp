#include "TestScene.h"
#include "gles/gl.h"		//测试

CCScene* TestScene::node()
{
	CCScene *pRet = new TestScene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void TestScene::setPosition(const CCPoint& newPosition)
{
	CCDirector::sharedDirector()->replaceScene(TestScene1::node());
}

void TestScene::setAnchorPoint(const CCPoint& point)
{
	if (CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->resume();
	else
		CCDirector::sharedDirector()->pause();
}

void TestScene::draw()
{
	CCScene::draw();
	
	unsigned char mIndices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
	unsigned char mIndices1[] = {0, 1, 3, 2, 6, 1, 5, 0, 4, 3, 7, 6, 4, 5};
	const GLfloat cube[] = 
	{
		100.0f, 100.0f, -100.0f,
		300.0f, 100.0f, -100.0f,
		300.0f, 300.0f, -100.0f,
		100.0f, 300.0f, -100.0f,
		100.0f, 100.0f, -300.0f,
		300.0f, 100.0f, -300.0f,
		300.0f, 300.0f, -300.0f,
		100.0f, 300.0f, -300.0f,
	};
	const GLfloat color[] = 
	{
		1.0f, 0.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 1.0f, 100.0f,
		0.0f, 1.0f, 1.0f, 100.0f,
		1.0f, 0.0f, 1.0f, 100.0f,
		1.0f, 1.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 0.0f, 100.0f,
		1.0f, 1.0f, 1.0f, 100.0f,
	};
	static GLfloat rotate = 0.4f;
	rotate += 0.4f;

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glColorPointer(4, GL_FLOAT, 0, color);
	glVertexPointer(3, GL_FLOAT, 0, &cube);
	glTranslatef(0.0f, -50.0f, 0.0f);
	glRotatef(rotate, 100.0f, 100.0f, -100.0f);
	glBlendFunc(GL_ONE, GL_ZERO);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_BYTE, mIndices1);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, mIndices);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

//////////////////////////////////////////////////////////////////////////
CCScene* TestScene1::node()
{
	CCScene *pRet = new TestScene1();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void TestScene1::setPosition(const CCPoint& newPosition)
{
	CCDirector::sharedDirector()->replaceScene(TestScene::node());
}

void TestScene1::setAnchorPoint(const CCPoint& point)
{
	if (CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->resume();
	else
		CCDirector::sharedDirector()->pause();
}


void TestScene1::draw()
{
	CCScene::draw();

	unsigned char mIndices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
	const GLfloat cube[] = 
	{
		100.0f, 100.0f, -100.0f,
		300.0f, 100.0f, -100.0f,
		300.0f, 300.0f, -100.0f,
		100.0f, 300.0f, -100.0f,
		100.0f, 100.0f, -300.0f,
		300.0f, 100.0f, -300.0f,
		300.0f, 300.0f, -300.0f,
		100.0f, 300.0f, -300.0f,
	};
	static GLfloat rotate = 0.4f;
	rotate += 0.4f;

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &cube);
	glTranslatef(0.0f, -50.0f, 0.0f);
	glRotatef(rotate, 100.0f, 100.0f, -100.0f);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, mIndices);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}