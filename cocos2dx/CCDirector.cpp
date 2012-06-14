#include "CCDirector.h"
#include "CCAutoreleasePool.h"
#include "CCApplication.h"
#include "CCScene.h"
#include "CCTouchDispatcher.h"
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
	// 场景相关变量
	m_pRunningScene = NULL;
	m_pNextScene = NULL;

	// 初始化成员变量
	m_pobOpenGLView = NULL;

	// 创建内存池管理对象
	CCPoolManager::getInstance()->push();

	m_dOldAnimationInterval = m_dAnimationInterval = 1.0 / 60;
	m_obWinSizeInPoints = CCSizeZero;
	m_bInvalid = false;
	m_bPaused = false;
	m_bPurgeDirecotorInNextLoop = false;
	m_pobScenesStack = new CCMutableArray<CCScene*>();

	return true;
}

CCDirector::~CCDirector()
{
	CC_SAFE_RELEASE(m_pobScenesStack);
	// 释放管理的对象内存
	CCPoolManager::getInstance()->pop();
}

CCSize CCDirector::getWinSize()
{
	return m_obWinSizeInPoints;
}

void CCDirector::mainLoop()
{
	if (m_bPurgeDirecotorInNextLoop)
	{
		purgeDirector();
		m_bPurgeDirecotorInNextLoop = false;
	}
	else if (!m_bInvalid)
	{
		drawScene();
		// 释放管理的对象内存
		CCPoolManager::getInstance()->pop();
	}
}

void CCDirector::setOpenGLView(CCEGLView *pobOpenGLView)
{
	CCAssert(pobOpenGLView, "opengl view should not be null");
	if (m_pobOpenGLView != pobOpenGLView)
	{
		// 由于CCEGLView不是从CCObject继承，因此没有内存自动释放功能，需手动delete
		delete m_pobOpenGLView;
		m_pobOpenGLView = pobOpenGLView;

		m_obWinSizeInPoints = m_pobOpenGLView->getSize();
		setGLDefaultValues();

		CCTouchDispatcher* pTouchDispatcher = CCTouchDispatcher::sharedDispatcher();
		if (NULL != pTouchDispatcher)
		{
			m_pobOpenGLView->setTouchDelegate(pTouchDispatcher);
			pTouchDispatcher->setDispatchEvents(true);
		}
	}
}

void CCDirector::setGLDefaultValues()
{
	// 这个成员方法只能在CCEGLView初始化成功后被调用
	CCAssert(m_pobOpenGLView, "opengl view should not be null");

	// opengl es init todo...
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, (GLsizei)(m_obWinSizeInPoints.width), (GLsizei)(m_obWinSizeInPoints.height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, m_obWinSizeInPoints.width, 0, m_obWinSizeInPoints.height, -1024, 1024);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// 设置每次opengl es清除背景默认颜色为黑色
}

void CCDirector::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (NULL != m_pNextScene)
	{
		setNextScene();
	}

	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	if (NULL != m_pRunningScene)
	{
		m_pRunningScene->visit();
	}

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	if (m_pobOpenGLView)
	{
		m_pobOpenGLView->swapBuffers();
	}
}

void CCDirector::setAnimationInterval(double dValue)
{
	m_dAnimationInterval = dValue;
	if (!m_bInvalid)
	{
		stopAnimation();
		startAnimation();
	}
}

CCPoint CCDirector::convertToGL(const CCPoint& obPoint)
{
	CCPoint ret;
	ret.x = obPoint.x;
	ret.y = m_obWinSizeInPoints.height - obPoint.y;
	return ret;
}

CCPoint CCDirector::convertToUI(const CCPoint& obPoint)
{
	CCPoint ret;
	ret.x = obPoint.x;
	ret.y = m_obWinSizeInPoints.height - obPoint.y;
	return ret;
}

void CCDirector::runWithScene(CCScene* pScene)
{
	CCAssert(pScene != NULL, "the scene should not be null");
	CCAssert(m_pRunningScene == NULL, "m_pRunningScene should be null");

	pushScene(pScene);
	startAnimation();
}

void CCDirector::pushScene(CCScene* pScene)
{
	CCAssert(pScene != NULL, "the scene should not be null");
	m_pobScenesStack->addObject(pScene);
	m_pNextScene = pScene;
}

void CCDirector::popScene(CCScene* pScene)
{
	CCAssert(m_pRunningScene != NULL, "running scene should not be null");
	m_pobScenesStack->removeLastObject();
	unsigned int c = m_pobScenesStack->count();
	if (c == 0)
	{
		end();
	}
	else
	{
		m_pNextScene = m_pobScenesStack->getObjectAtIndex(c - 1);
	}
}

void CCDirector::replaceScene(CCScene* pScene)
{
	CCAssert(pScene != NULL, "the scene should not be null");
	unsigned int index = m_pobScenesStack->count();
	CCAssert(index != 0, "replaceScene must be used after runWithScene");
	m_pobScenesStack->replaceObjectAtIndex(index - 1, pScene);
	m_pNextScene = pScene;
}

void CCDirector::setNextScene()
{
	if (NULL != m_pRunningScene)
	{
		m_pRunningScene->onExit();
		m_pRunningScene->release();
	}
	m_pRunningScene = m_pNextScene;
	m_pNextScene->retain();
	m_pNextScene = NULL;

	if (NULL != m_pRunningScene)
	{
		m_pRunningScene->onEnter();
	}
}

void CCDirector::end()
{
	m_bPurgeDirecotorInNextLoop = true;
}

void CCDirector::pause()
{
	if (m_bPaused)
		return;

	m_dOldAnimationInterval = m_dAnimationInterval;

	setAnimationInterval(1 / 4.0f);
	m_bPaused = true;
}

void CCDirector::resume()
{
	if (!m_bPaused)
		return;

	setAnimationInterval(m_dOldAnimationInterval);
	m_bPaused = false;
}

void CCDirector::stopAnimation()
{
	m_bInvalid = true;
}

void CCDirector::startAnimation()
{
	m_bInvalid = false;
	CCApplication::sharedApplication().setAnimationInterval(m_dAnimationInterval);
}

void CCDirector::purgeDirector()
{
	// don't release the event handlers
	// They are needed in case the director is run again
	CCTouchDispatcher::sharedDispatcher()->removeAllDelegates();

	if (NULL != m_pRunningScene)
	{
		m_pRunningScene->onExit();
		m_pRunningScene->release();
	}

	m_pRunningScene = NULL;
	m_pNextScene = NULL;

	m_pobScenesStack->removeAllObjects();

	stopAnimation();

	m_pobOpenGLView->release();
	m_pobOpenGLView = NULL;
}

NS_CC_END;