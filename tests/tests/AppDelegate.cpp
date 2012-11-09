#include "AppDelegate.h"

#include "cocos2d.h"
#include "TestControllerScene.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::initInstance()
{
	bool bRet = false;
	do 
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

		CCEGLView* pMainWnd = new CCEGLView;
		CC_BREAK_IF (NULL == pMainWnd || !pMainWnd->Create(TEXT("mycocos2dx"), 480, 320));

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

		// todo...

#endif

		bRet = true;
	} while (0);

	return bRet;
}

bool AppDelegate::applicationDidFinishLaunching()
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(&CCEGLView::sharedOpenGLView());

	CCScene* pScene = TestControllerScene::create_scene();
	pDirector->runWithScene(pScene);
	return true;
}

void AppDelegate::applicationDidEnterBackground()
{

}

void AppDelegate::applicationWillEnterForeground()
{

}