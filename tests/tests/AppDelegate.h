#pragma once

#include "CCApplication.h"

class AppDelegate : private cocos2d::CCApplication
{
public:
	AppDelegate();
	virtual ~AppDelegate();

	virtual bool initInstance();
	virtual bool applicationDidFinishLaunching();
	virtual void applicationDidEnterBackground();
	virtual void applicationWillEnterForeground();
};