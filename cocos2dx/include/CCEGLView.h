﻿#pragma once

#include "ccMacros.h"
#include <Windows.h>

NS_CC_BEGIN;
class CCEGLView
{
public:
	static CCEGLView& sharedOpenGLView();
	CCEGLView();
	virtual ~CCEGLView();

	virtual bool Create(LPCTSTR pTitle, int w, int h);
	HWND getHWnd();
	void resize(int width, int height);
	void centerWindow();

private:
	HWND m_hWnd;
};
NS_CC_END;