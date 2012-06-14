#include "AppDelegate.h"

#include "cocos2d.h"
#include "TestScene.h"

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
		CCEGLView* pMainWnd = new CCEGLView;
		if (!pMainWnd || !pMainWnd->Create(TEXT("mycocos2dx"), 480, 320))
			break;

		bRet = true;
	} while (0);

	return bRet;
}

bool AppDelegate::applicationDidFinishLaunching()
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(&CCEGLView::sharedOpenGLView());

	CCScene* pScene = TestScene2::scene();
	pDirector->runWithScene(pScene);
	return true;
}

void AppDelegate::applicationDidEnterBackground()
{

}

void AppDelegate::applicationWillEnterForeground()
{

}