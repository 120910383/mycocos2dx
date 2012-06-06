#pragma once
#include "CCObject.h"
#include "CCGeometry.h"

NS_CC_BEGIN;

class CCEGLView;
class CC_DLL CCDirector : public CCObject
{
public:
	static CCDirector* sharedDirector(void);
	virtual bool init();
	virtual ~CCDirector();
	virtual void mainLoop();

	CCSize getWinSize();
	inline CCEGLView* getOpenGLView(void) { return m_pobOpenGLView; }
	void setOpenGLView(CCEGLView *pobOpenGLView);
	void drawScene();
	void setGLDefaultValues();

protected:
	CCEGLView* m_pobOpenGLView;
	CCSize m_obWinSizeInPoints;
};

NS_CC_END;