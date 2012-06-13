#include "CCEGLView.h"
#include "CCDirector.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "EGL/egl.h"
#include "CCScene.h"	// 测试场景转换的代码，随后删除

NS_CC_BEGIN;
//////////////////////////////////////////////////////////////////////////
// impliment CCEGL
//////////////////////////////////////////////////////////////////////////
class CCEGL
{
public:
	~CCEGL() 
	{
		if (EGL_NO_SURFACE != m_eglSurface)
		{
			eglDestroySurface(m_eglDisplay, m_eglSurface);
		}
		if (EGL_NO_CONTEXT != m_eglContext)
		{
			eglDestroyContext(m_eglDisplay, m_eglContext);
		}
		eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglTerminate(m_eglDisplay);
		if (m_eglNativeDisplay)
		{
			ReleaseDC(m_eglNativeWindow, m_eglNativeDisplay);
		}
	}

	static CCEGL * create(CCEGLView * pWindow)
	{
		CCEGL * pEGL = new CCEGL;
		BOOL bSuccess = FALSE;
		do 
		{
			CC_BREAK_IF(! pEGL);

			pEGL->m_eglNativeWindow = pWindow->getHWnd();

			pEGL->m_eglNativeDisplay = GetDC(pEGL->m_eglNativeWindow);

			EGLDisplay eglDisplay;
			CC_BREAK_IF(EGL_NO_DISPLAY == (eglDisplay = eglGetDisplay(pEGL->m_eglNativeDisplay)));

			EGLint nMajor, nMinor;
			CC_BREAK_IF(EGL_FALSE == eglInitialize(eglDisplay, &nMajor, &nMinor) || 1 != nMajor);

			const EGLint aConfigAttribs[] =
			{
				EGL_LEVEL,				0,
				EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
				EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
				EGL_NATIVE_RENDERABLE,	EGL_FALSE,
				EGL_DEPTH_SIZE,			16,
				EGL_NONE,
			};
			EGLint iConfigs;
			EGLConfig eglConfig;
			CC_BREAK_IF(EGL_FALSE == eglChooseConfig(eglDisplay, aConfigAttribs, &eglConfig, 1, &iConfigs) 
				|| (iConfigs != 1));

			EGLContext eglContext;
			eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
			CC_BREAK_IF(EGL_NO_CONTEXT == eglContext);

			EGLSurface eglSurface;
			eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, pEGL->m_eglNativeWindow, NULL);
			CC_BREAK_IF(EGL_NO_SURFACE == eglSurface);

			CC_BREAK_IF(EGL_FALSE == eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext));

			pEGL->m_eglDisplay = eglDisplay;
			pEGL->m_eglConfig  = eglConfig;
			pEGL->m_eglContext = eglContext;
			pEGL->m_eglSurface = eglSurface;
			bSuccess = TRUE;
		} while (0);

		if (! bSuccess)
		{
			CC_SAFE_DELETE(pEGL);  
		}

		return pEGL;
	}

	void swapBuffers()
	{
		if (EGL_NO_DISPLAY != m_eglDisplay)
		{
			eglSwapBuffers(m_eglDisplay, m_eglSurface);
		}
	}
private:
	CCEGL() 
		: m_eglNativeWindow(NULL)
		, m_eglNativeDisplay(EGL_DEFAULT_DISPLAY)
		, m_eglDisplay(EGL_NO_DISPLAY)
		, m_eglConfig(0)
		, m_eglSurface(EGL_NO_SURFACE)
		, m_eglContext(EGL_NO_CONTEXT)
	{}

	EGLNativeWindowType     m_eglNativeWindow;
	EGLNativeDisplayType    m_eglNativeDisplay;
	EGLDisplay              m_eglDisplay;
	EGLConfig               m_eglConfig;
	EGLSurface              m_eglSurface;
	EGLContext              m_eglContext;
};

//////////////////////////////////////////////////////////////////////////
// impliment CCEGL
//////////////////////////////////////////////////////////////////////////
static CCEGLView* s_pMainWindow;
static const WCHAR * kWindowClassName = L"Cocos2dxWin32";
static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (s_pMainWindow && s_pMainWindow->getHWnd() == hWnd)
	{
		return s_pMainWindow->WindowProc(uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

CCEGLView& CCEGLView::sharedOpenGLView()
{
	CCAssert(NULL != s_pMainWindow, "");
	return *s_pMainWindow;
}

CCEGLView::CCEGLView()
	: m_pEGL(NULL)
	, m_hWnd(NULL)
	, m_pDelegate(NULL)
	, m_bCaptured(false)
{
	m_pTouch = new CCTouch();
	m_tSizeInPoints.cx = m_tSizeInPoints.cy = 0;
}

CCEGLView::~CCEGLView()
{

}

CCSize CCEGLView::getSize()
{
	return CCSize((float)(m_tSizeInPoints.cx), (float)(m_tSizeInPoints.cy));
}

bool CCEGLView::Create(LPCTSTR pTitle, int w, int h)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(NULL != m_hWnd);

		HINSTANCE hInstance = GetModuleHandle(NULL);		//参数为NULL表示获取调用本模块的应用程序实例句柄
		WNDCLASS wc;

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = _WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = kWindowClassName;

		CC_BREAK_IF( !RegisterClass(&wc) && 1410 != GetLastError() );

		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			kWindowClassName,
			pTitle,
			WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX,
			0, 0,
			0,
			0,
			NULL,
			NULL,
			hInstance,
			NULL);

		CC_BREAK_IF(NULL == m_hWnd);

		m_tSizeInPoints.cx = w;
		m_tSizeInPoints.cy = h;
		resize(w, h);

		// init egl
		m_pEGL = CCEGL::create(this);

		if (! m_pEGL)
		{
			DestroyWindow(m_hWnd);
			m_hWnd = NULL;
			break;
		}

		s_pMainWindow = this;
		bRet = true;
	} while (0);

	return bRet;
}

LRESULT CCEGLView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (NULL != m_pDelegate && NULL != m_pTouch && MK_LBUTTON == wParam)
		{
			POINT pt = {(short)LOWORD(lParam), (short)HIWORD(lParam)};
			m_bCaptured = true;
			SetCapture(m_hWnd);
			m_pTouch->SetTouchInfo(0, (float)(pt.x), (float)(pt.y));
			m_pDelegate->toucheBegan(m_pTouch, NULL);
		}
		break;
	case WM_MOUSEMOVE:
		if (MK_LBUTTON == wParam && m_bCaptured)
		{
			m_pTouch->SetTouchInfo(0, (float)(LOWORD(lParam)), (float)(HIWORD(lParam)));
			m_pDelegate->toucheMoved(m_pTouch, NULL);
		}
		break;
	case WM_LBUTTONUP:
		if (m_bCaptured)
		{
			m_pTouch->SetTouchInfo(0, (float)(LOWORD(lParam)), (float)(HIWORD(lParam)));
			m_pDelegate->toucheEnded(m_pTouch, NULL);
			ReleaseCapture();
			m_bCaptured = false;
		}
		break;
	case WM_PAINT:
		BeginPaint(m_hWnd, &ps);
		EndPaint(m_hWnd, &ps);
		break;
	case WM_CHAR:
		if (VK_ESCAPE == wParam)
			CCDirector::sharedDirector()->end();
		else if (VK_RETURN == wParam)
			// 测试场景转换的代码，随后删除，功能表现为，在按回车时调用setPosition模拟函数，该函数为虚函数(这也是为什么要用这个函数测试的原因)
			// setPosition测试虚函数由当前运行的场景类实现，这里实现为切换场景操作，见TestScene.cpp
			CCDirector::sharedDirector()->getRunningScene()->setPosition(CCPointZero);
		else if (VK_SPACE == wParam)
			// 测试场景暂停恢复的代码，随后删除，功能表现为，在按空格时调用setAnchorPoint模拟函数，该函数为虚函数(这也是为什么要用这个函数测试的原因)
			// setAnchorPoint测试虚函数由当前运行的场景类实现，这里实现为暂停恢复场景操作，见TestScene.cpp
			CCDirector::sharedDirector()->getRunningScene()->setAnchorPoint(CCPointZero);
		break;
	case WM_CLOSE:
		CCDirector::sharedDirector()->end();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return 0;
}

bool CCEGLView::isOpenGLReady()
{
	return (NULL != m_pEGL);
}

HWND CCEGLView::getHWnd()
{
	return m_hWnd;
}

void CCEGLView::release()
{
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
	s_pMainWindow = NULL;
	UnregisterClass(kWindowClassName, GetModuleHandle(NULL));

	CC_SAFE_DELETE(m_pTouch);
	CC_SAFE_DELETE(m_pDelegate);
	CC_SAFE_DELETE(m_pEGL);
	delete this;
}

void CCEGLView::swapBuffers()
{
	if (m_pEGL)
	{
		m_pEGL->swapBuffers();
	}
}

void CCEGLView::resize(int width, int height)
{
	if (!m_hWnd)
	{
		return;
	}

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	if (rcClient.right - rcClient.left == width &&
		rcClient.bottom - rcClient.top == height)
	{
		return;
	}

	rcClient.right = rcClient.left + width;
	rcClient.bottom = rcClient.top + height;
	AdjustWindowRectEx(&rcClient, GetWindowLong(m_hWnd, GWL_STYLE), false, GetWindowLong(m_hWnd, GWL_EXSTYLE));
	SetWindowPos(m_hWnd, 0, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void CCEGLView::centerWindow()
{
	if (!m_hWnd)
	{
		return;
	}

	RECT rcDesktop, rcWindow;
	GetWindowRect(GetDesktopWindow(), &rcDesktop);

	HWND hTaskBar = FindWindow(TEXT("Shell_TrayWnd"), NULL);
	if (hTaskBar != NULL)
	{
		APPBARDATA abd;
		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd = hTaskBar;

		SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
		SubtractRect(&rcDesktop, &rcDesktop, &abd.rc);
	}
	GetWindowRect(m_hWnd, &rcWindow);

	int offsetX = (rcDesktop.right - rcDesktop.left - (rcWindow.right - rcWindow.left)) / 2;
	offsetX = (offsetX > 0) ? offsetX : rcDesktop.left;
	int offsetY = (rcDesktop.bottom - rcDesktop.top - (rcWindow.bottom - rcWindow.top)) / 2;
	offsetY = (offsetY > 0) ? offsetY : rcDesktop.top;

	SetWindowPos(m_hWnd, 0, offsetX, offsetY, 0, 0, SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void CCEGLView::setTouchDelegate(EGLTouchDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

NS_CC_END;