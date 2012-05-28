#include "AppDelegate.h"
#include "cocos2d.h"

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
		cocos2d::CCEGLView* pMainWnd = new cocos2d::CCEGLView;
		if (!pMainWnd || !pMainWnd->Create(TEXT("mycocos2dx"), 480, 320))
			break;

		bRet = true;
	} while (0);

	return bRet;
}

bool AppDelegate::applicationDidFinishLaunching()
{
	cocos2d::CCDirector *pDirector = cocos2d::CCDirector::sharedDirector();
	pDirector->setOpenGLView(&cocos2d::CCEGLView::sharedOpenGLView());
	return true;
}

void AppDelegate::applicationDidEnterBackground()
{

}

void AppDelegate::applicationWillEnterForeground()
{

}