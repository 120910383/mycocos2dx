#include "OpenGLTest/OpenGLTest.h"

//////////////////////////////////////////////////////////////////////////
const char* OpenGLRotateCubeTest::title()
{
	return "OpenGL Rotate Cube";
}

TestBasicLayer* OpenGLRotateCubeTest::get_last_layer()
{
	return NULL;
}

TestBasicLayer* OpenGLRotateCubeTest::get_next_layer()
{
	return OpenGLDragCubeTest::create_test();
}

bool OpenGLRotateCubeTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !TestBasicLayer::init() );

		result = true;
	} while (0);

	return result;
}

void OpenGLRotateCubeTest::draw()
{
	TestBasicLayer::draw();

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
	
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_BYTE, mIndices1);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, mIndices);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

//////////////////////////////////////////////////////////////////////////
const char* OpenGLDragCubeTest::title()
{
	return "OpenGL Drag Cube(Drag Screen)";
}

TestBasicLayer* OpenGLDragCubeTest::get_last_layer()
{
	return OpenGLRotateCubeTest::create_test();
}

TestBasicLayer* OpenGLDragCubeTest::get_next_layer()
{
	return NULL;
}

bool OpenGLDragCubeTest::init()
{
	bool result = false;
	do 
	{
		CC_BREAK_IF( !TestBasicLayer::init() );
		setIsTouchEnabled(true);

		result = true;
	} while (0);

	return result;
}

bool OpenGLDragCubeTest::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	m_touch_pos = convertTouchToNodeSpace(pTouch);
	return true;
}

void OpenGLDragCubeTest::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint move_pos = convertTouchToNodeSpace(pTouch);
	m_perp = ccpPerp(ccpSub(move_pos, m_touch_pos));
	m_rotate = ccpDistance(move_pos, m_touch_pos) / 2.0f;
}

void OpenGLDragCubeTest::draw()
{
	TestBasicLayer::draw();

	unsigned char mIndices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
	unsigned char mIndices1[] = {0, 1, 3, 2, 6, 1, 5, 0, 4, 3, 7, 6, 4, 5};
	const GLfloat cube[] = 
	{
		-100.0f, -100.0f, 100.0f,
		100.0f, -100.0f, 100.0f,
		100.0f, 100.0f, 100.0f,
		-100.0f, 100.0f, 100.0f,
		-100.0f, -100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,
		100.0f, 100.0f, -100.0f,
		-100.0f, 100.0f, -100.0f,
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

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glColorPointer(4, GL_FLOAT, 0, color);
	glVertexPointer(3, GL_FLOAT, 0, &cube);
	glTranslatef(200.0f, 150.0f, 0.0f);
	glRotatef(m_rotate, m_perp.x, m_perp.y, 0.0f);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_BYTE, mIndices1);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, mIndices);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}