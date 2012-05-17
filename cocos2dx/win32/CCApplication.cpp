#include "CCApplication.h"
#include "CCEGLView.h"

NS_CC_BEGIN;
CCApplication* CCApplication::sm_pSharedApplication = NULL;
CCApplication& CCApplication::sharedApplication()
{
	CCAssert(NULL != sm_pSharedApplication, "");
	return *sm_pSharedApplication;
}

CCApplication::CCApplication()
{
	m_nAnimationInterval.QuadPart = 0;
	CCAssert(NULL == sm_pSharedApplication, "");
	sm_pSharedApplication = this;
}

CCApplication::~CCApplication()
{
	CCAssert(this == sm_pSharedApplication, "");
	sm_pSharedApplication = NULL;
}

void CCApplication::setAnimationInterval(double interval)
{
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	m_nAnimationInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);
}

int CCApplication::run()
{
	MSG msg;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nLast);

	// 初始化窗口和游戏引擎，由子类实现
	if (!initInstance() || !applicationDidFinishLaunching())
	{
		return 0;
	}

	CCEGLView& mainWnd = CCEGLView::sharedOpenGLView();
	mainWnd.centerWindow();
	ShowWindow(mainWnd.getHWnd(), SW_SHOW);

	while(1)
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			QueryPerformanceCounter(&nNow);
			if (nNow.QuadPart - nLast.QuadPart > m_nAnimationInterval.QuadPart)
			{
				nLast.QuadPart = nNow.QuadPart;
				// mainloop todo...
			}
			else
			{
				Sleep(0);
			}
			continue;
		}

		if (WM_QUIT == msg.message)
		{
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
NS_CC_END;