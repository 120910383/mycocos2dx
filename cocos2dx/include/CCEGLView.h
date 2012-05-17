#pragma once

#include "ccMacros.h"
#include <Windows.h>

NS_CC_BEGIN;
class CC_DLL CCEGLView
{
public:
	static CCEGLView& sharedOpenGLView();
	CCEGLView();
	virtual ~CCEGLView();

	virtual bool Create(LPCTSTR pTitle, int w, int h);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	HWND getHWnd();
	void resize(int width, int height);
	void centerWindow();

private:
	HWND m_hWnd;
};
NS_CC_END;