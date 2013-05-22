#pragma once

#include "CCPlatformMacros.h"
#include "CCGeometry.h"
#include "CCStdC.h"
#include "CCPlatformConfig.h"

NS_CC_BEGIN;

#if !CC_USE_GLFW_WINDOW

class CCEGL;
class CCTouch;
class EGLTouchDelegate;
class CC_DLL CCEGLView
{
public:
	static CCEGLView& sharedOpenGLView();
	CCEGLView();
	virtual ~CCEGLView();

	CCSize getSize();
	virtual bool Create(LPCTSTR pTitle, int w, int h);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool isOpenGLReady();
	HWND getHWnd();
	void release();	// 提供一个显式销毁接口，替代delete操作
	void swapBuffers();
	void resize(int width, int height);
	void centerWindow();

	void setTouchDelegate(EGLTouchDelegate* pDelegate);

private:
	CCEGL* m_pEGL;
	HWND m_hWnd;
	SIZE m_tSizeInPoints;
	bool m_bCaptured;
	CCTouch* m_pTouch;
	EGLTouchDelegate* m_pDelegate;
};

#else

class CCTouch;
class EGLTouchDelegate;
class CC_DLL CCEGLView
{
public:
	static CCEGLView& sharedOpenGLView();
	CCEGLView();
	virtual ~CCEGLView();

	friend void keyEventHandle(int,int);
	friend void mouseButtonEventHandle(int,int);
	friend void mousePosEventHandle(int,int);
	friend void charEventHandle(int,int);

	bool isOpenGLReady();
	CCSize getSize() { return m_size; }
	bool Create(LPCTSTR pTitle, int w, int h);
	void release();
	void swapBuffers();
	void centerWindow();
	void setTouchDelegate(EGLTouchDelegate* pDelegate);

private:
	bool m_is_init;
	CCTouch* m_pTouch;
	EGLTouchDelegate* m_pDelegate;
	CCPoint m_mousePoint;
	CCSize m_size;
};

#endif

NS_CC_END;