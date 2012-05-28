#include "CCDirector.h"
#include "CCAutoreleasePool.h"
#include "CCEGLView.h"
#include "GLES/gl.h"

NS_CC_BEGIN;
// 全局唯一的导演对象
static CCDirector s_sharedDirector;
static bool s_bFirstRun = true;
CCDirector* CCDirector::sharedDirector()
{
	if (s_bFirstRun)
	{
		s_sharedDirector.init();
		s_bFirstRun = false;
	}
	return &s_sharedDirector;
}

bool CCDirector::init()
{
	// 初始化成员变量
	m_pobOpenGLView = NULL;

	// 创建内存池管理对象
	CCPoolManager::getInstance()->push();

	return true;
}

CCDirector::~CCDirector()
{
	// 释放管理的对象内存
	CCPoolManager::getInstance()->pop();
}

void CCDirector::mainLoop()
{
	drawScene();
	// 释放管理的对象内存
	CCPoolManager::getInstance()->pop();
}

void CCDirector::setOpenGLView(CCEGLView *pobOpenGLView)
{
	CCAssert(pobOpenGLView, "opengl view should not be null");
	if (m_pobOpenGLView != pobOpenGLView)
	{
		// 由于CCEGLView不是从CCObject继承，因此没有内存自动释放功能，需手动delete
		delete m_pobOpenGLView;
		m_pobOpenGLView = pobOpenGLView;
		setGLDefaultValues();
	}
}

void CCDirector::setGLDefaultValues()
{
	// 这个成员方法只能在CCEGLView初始化成功后被调用
	CCAssert(m_pobOpenGLView, "opengl view should not be null");

	// opengl es init todo...
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// 设置每次opengl es清除背景默认颜色为黑色
}

void CCDirector::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// opengl es draw todo...

	if (m_pobOpenGLView)
	{
		m_pobOpenGLView->swapBuffers();
	}
}

NS_CC_END;