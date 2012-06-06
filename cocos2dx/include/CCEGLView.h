#pragma once

#include <Windows.h>
#include "ccMacros.h"
#include "CCGeometry.h"

NS_CC_BEGIN;
class CCEGL;
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

private:
	CCEGL* m_pEGL;
	HWND m_hWnd;
	SIZE m_tSizeInPoints;
};
NS_CC_END;