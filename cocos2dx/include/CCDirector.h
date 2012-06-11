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
	inline bool isPaused(void) { return m_bPaused; }
	inline double getAnimationInterval(void) { return m_dAnimationInterval; }
	virtual void setAnimationInterval(double dValue);

public:
	void end();
	void pause();
	void resume();
	void stopAnimation();
	void startAnimation();

protected:
	void purgeDirector();

protected:
	CCEGLView* m_pobOpenGLView;
	CCSize m_obWinSizeInPoints;
	bool m_bPaused;
	bool m_bInvalid;
	double m_dAnimationInterval;
	double m_dOldAnimationInterval;
	bool m_bPurgeDirecotorInNextLoop;
};

NS_CC_END;