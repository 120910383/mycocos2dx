#include "CCApplication.h"

NS_CC_BEGIN;
CCApplication* CCApplication::sm_pSharedApplication = NULL;
CCApplication& CCApplication::sharedApplication()
{
	CCAssert(NULL != sm_pSharedApplication, "");
	return *sm_pSharedApplication;
}

CCApplication::CCApplication()
{
	CCAssert(NULL == sm_pSharedApplication, "");
	sm_pSharedApplication = this;
}

CCApplication::~CCApplication()
{
	CCAssert(this == sm_pSharedApplication, "");
	sm_pSharedApplication = NULL;
}

int CCApplication::run()
{
	// 初始化窗口和游戏引擎，由子类实现
	if (!initInstance() || !applicationDidFinishLaunching())
	{
		return 0;
	}

	// 此时确保窗口已创建todo...

	// 消息循环todo...
	return 0;
}
NS_CC_END;