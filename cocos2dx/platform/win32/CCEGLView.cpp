#include "CCEGLView.h"
#include "CCDirector.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "CCGL.h"

NS_CC_BEGIN;

#if !CC_USE_GLFW_WINDOW
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

#else

static CCEGLView* s_pMainWindow = NULL;

void keyEventHandle(int iKeyID,int iKeyState)
{
	if (iKeyState ==GLFW_RELEASE) {
		return;
	}

	if (GLFW_KEY_ESC == iKeyID)
		CCDirector::sharedDirector()->end();
}

void charEventHandle(int iCharID,int iCharState) {
	if (iCharState ==GLFW_RELEASE) {
		return;
	}

	// test
	int a = iCharID;
	a++;
}

void mouseButtonEventHandle(int iMouseID,int iMouseState)
{
	if (iMouseID == GLFW_MOUSE_BUTTON_LEFT) {
		//get current mouse pos
		int x,y;
		glfwGetMousePos(&x, &y);
		CCPoint oPoint((float)x,(float)y);

		s_pMainWindow->m_pTouch->SetTouchInfo(0, (float)(oPoint.x), (float)(oPoint.y));

		s_pMainWindow->m_mousePoint = oPoint;

		if (iMouseState == GLFW_PRESS) {
			s_pMainWindow->m_pDelegate->toucheBegan(s_pMainWindow->m_pTouch,NULL);

		} else if (iMouseState == GLFW_RELEASE) {
			s_pMainWindow->m_pDelegate->toucheEnded(s_pMainWindow->m_pTouch,NULL);
		}
	}
}

void mousePosEventHandle(int iPosX,int iPosY)
{
	int iButtonState = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);

	//to test move
	if (iButtonState == GLFW_PRESS) {
		if (iPosX!=(int)s_pMainWindow->m_mousePoint.x||iPosY!=(int)s_pMainWindow->m_mousePoint.y) {
			//it movies
			s_pMainWindow->m_pTouch->SetTouchInfo(0, (float)(iPosX), (float)(iPosY));
			s_pMainWindow->m_pDelegate->toucheMoved(s_pMainWindow->m_pTouch, NULL);
			//update new mouse pos
			s_pMainWindow->m_mousePoint.x = (float)iPosX;
			s_pMainWindow->m_mousePoint.y = (float)iPosY;
		}
	}
}

int closeEventHandle()
{
	CCDirector::sharedDirector()->end();
	return GL_TRUE;
}

CCEGLView& CCEGLView::sharedOpenGLView()
{
	CCAssert(NULL != s_pMainWindow, "");
	return *s_pMainWindow;
}

CCEGLView::CCEGLView()
	: m_pDelegate(NULL)
	, m_is_init(false)
{
	m_pTouch = new CCTouch();
}

CCEGLView::~CCEGLView()
{

}

bool CCEGLView::isOpenGLReady()
{
	return m_is_init;
}

bool CCEGLView::Create(LPCTSTR pTitle, int w, int h)
{
	bool result = false;

	CCAssert(0 != w && 0 != h, "invalid window's size equal 0");

	result = (GL_FALSE != glfwInit());
	if (!result)
	{
		CCAssert(false, "fail to init the glfw");
		return result;
	}

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

	// 创建窗口，窗口属性根据不同需要进行更改
	// glfwOpenWindow参数依次为窗口宽度、窗口高度、red位深、green位深、blue位深、Alpha位深、模板缓冲、全屏/窗口
	result = (GL_FALSE != glfwOpenWindow(w, h, 5, 6, 5, 0, 16, 0, GLFW_WINDOW));
	if (!result)
	{
		CCAssert(false, "fail to init the glfw window");
		return result;
	}

	char title_str[60] = {0};
	WideCharToMultiByte(CP_ACP, 0, pTitle, -1, title_str, 60, NULL, NULL);
	glfwSetWindowTitle(title_str);

	int real_width = 0;
	int real_height = 0;
	glfwGetWindowSize(&real_width, &real_height);
	m_size = CCSizeMake((float)real_width, (float)real_height);

	//register the glfw key event
	glfwSetKeyCallback(keyEventHandle);
	//register the glfw char event
	glfwSetCharCallback(charEventHandle);
	//register the glfw mouse event
	glfwSetMouseButtonCallback(mouseButtonEventHandle);
	//register the glfw mouse pos event
	glfwSetMousePosCallback(mousePosEventHandle);
	//register the glfw close event
	glfwSetWindowCloseCallback(closeEventHandle);

	m_is_init = true;
	s_pMainWindow = this;

	centerWindow();

	return result;
}

void CCEGLView::release()
{
	glfwTerminate();
	exit(0);
}

void CCEGLView::swapBuffers()
{
	if (m_is_init)
	{
		glfwSwapBuffers();
	}
}

void CCEGLView::centerWindow()
{
	// 获取桌面尺寸
	RECT rcDesktop;
	GetWindowRect(GetDesktopWindow(), &rcDesktop);
	CCSize desktop_size = CCSizeMake((float)(rcDesktop.right - rcDesktop.left), (float)(rcDesktop.bottom - rcDesktop.top));

	// 设置窗口位置为屏幕中心
	CCPoint window_pos = CCPointMake((desktop_size.width - m_size.width) / 2, (desktop_size.height - m_size.height) / 2);
	glfwSetWindowPos((int)window_pos.x, (int)window_pos.y);
}

void CCEGLView::setTouchDelegate(EGLTouchDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

#endif

NS_CC_END;