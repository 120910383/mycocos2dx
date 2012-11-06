#include "CCApplication.h"
#include "CCEGLView.h"
#include "CCDirector.h"
#include "CCConfig.h"

#if !CC_USE_GLFW_WINDOW
/**
@brief	This function change the PVRFrame show/hide setting in register.
@param  bEnable If true show the PVRFrame window, otherwise hide.
*/
static void PVRFrameEnableControlWindow(bool bEnable);
#endif

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
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;

	QueryPerformanceFrequency(&nFreq);

#if !CC_USE_GLFW_WINDOW

	PVRFrameEnableControlWindow(true);

	// 初始化窗口和游戏引擎，由子类实现
	if (!initInstance() || !applicationDidFinishLaunching())
	{
		return 0;
	}

	CCEGLView& mainWnd = CCEGLView::sharedOpenGLView();
	mainWnd.centerWindow();
	ShowWindow(mainWnd.getHWnd(), SW_SHOW);

	MSG msg;
	QueryPerformanceCounter(&nLast);

	while(1)
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			QueryPerformanceCounter(&nNow);
			if (nNow.QuadPart - nLast.QuadPart > m_nAnimationInterval.QuadPart)
			{
				nLast.QuadPart = nNow.QuadPart;
				CCDirector::sharedDirector()->mainLoop();
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

#else

	// 初始化窗口和游戏引擎，由子类实现
	if (!initInstance() || !applicationDidFinishLaunching())
	{
		return 0;
	}

	while(1)
	{
		QueryPerformanceCounter(&nLast);
		CCDirector::sharedDirector()->mainLoop();
		QueryPerformanceCounter(&nNow);
		if (nNow.QuadPart - nLast.QuadPart < m_nAnimationInterval.QuadPart)
		{
			LONGLONG diff = (m_nAnimationInterval.QuadPart - nNow.QuadPart + nLast.QuadPart) * 1000 / nFreq.QuadPart;
			Sleep((DWORD)diff);
		}
	}

#endif
}

NS_CC_END;

#if !CC_USE_GLFW_WINDOW
//////////////////////////////////////////////////////////////////////////
// Local function
//////////////////////////////////////////////////////////////////////////
static void PVRFrameEnableControlWindow(bool bEnable)
{
	HKEY hKey = 0;

	// Open PVRFrame control key, if not exist create it.
	if(ERROR_SUCCESS != RegCreateKeyExW(HKEY_CURRENT_USER,
		L"Software\\Imagination Technologies\\PVRVFRame\\STARTUP\\",
		0,
		0,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		0,
		&hKey,
		NULL))
	{
		return;
	}

	const wchar_t * wszValue = L"hide_gui";
	const wchar_t * wszNewData = (bEnable) ? L"NO" : L"YES";
	wchar_t wszOldData[256] = {0};
	DWORD   dwSize = sizeof(wszOldData);
	LSTATUS status = RegQueryValueExW(hKey, wszValue, 0, NULL, (LPBYTE)wszOldData, &dwSize);
	if (ERROR_FILE_NOT_FOUND == status              // the key not exist
		|| (ERROR_SUCCESS == status                 // or the hide_gui value is exist
		&& 0 != wcscmp(wszNewData, wszOldData)))    // but new data and old data not equal
	{
		dwSize = sizeof(wchar_t) * (wcslen(wszNewData) + 1);
		RegSetValueEx(hKey, wszValue, 0, REG_SZ, (const BYTE *)wszNewData, dwSize);
	}

	RegCloseKey(hKey);
}
#endif
