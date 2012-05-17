#include "CCEGLView.h"

NS_CC_BEGIN;
static CCEGLView* s_pMainWindow;
static const WCHAR * kWindowClassName = L"Cocos2dxWin32";
CCEGLView& CCEGLView::sharedOpenGLView()
{
	CCAssert(NULL != s_pMainWindow, "");
	return *s_pMainWindow;
}

CCEGLView::CCEGLView()
	: m_hWnd(NULL)
{

}

CCEGLView::~CCEGLView()
{

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
		wc.lpfnWndProc = DefWindowProc;
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
		resize(w, h);
		// init egl todo...

		s_pMainWindow = this;
		bRet = true;
	} while (0);

	return bRet;
}

HWND CCEGLView::getHWnd()
{
	return m_hWnd;
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
NS_CC_END;