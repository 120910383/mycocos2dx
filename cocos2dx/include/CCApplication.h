﻿#pragma once

#include "ccMacros.h"
#include <Windows.h>

NS_CC_BEGIN;
class CCApplication
{
public:
	static CCApplication& sharedApplication();
	CCApplication();
	virtual ~CCApplication();

	// 由子类实现，创建窗口，初始化Opengl，设置资源路径等
	virtual bool initInstance() = 0;
	// 由子类实现，初始化游戏引擎，如CCDirector,CCScene
	virtual bool applicationDidFinishLaunching() = 0;
	virtual void applicationDidEnterBackground() = 0;
	virtual void applicationWillEnterForeground() = 0;
	int run();

protected:
	static CCApplication* sm_pSharedApplication;
};
NS_CC_END;