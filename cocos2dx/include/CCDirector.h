#pragma once
#include "CCObject.h"
#include "CCGeometry.h"
#include "CCMutableArray.h"

NS_CC_BEGIN;

class CCEGLView;
class CCScene;
class CC_DLL CCDirector : public CCObject
{
public:
	static CCDirector* sharedDirector(void);
	virtual bool init();
	virtual ~CCDirector();
	virtual void mainLoop();

	CCSize getWinSize();
	inline CCEGLView* getOpenGLView(void) { return m_pobOpenGLView; }
	inline CCScene* getRunningScene(void) { return m_pRunningScene; }
	void setOpenGLView(CCEGLView *pobOpenGLView);
	void drawScene();
	void purgeCachedData();
	void setGLDefaultValues();
	inline bool isPaused(void) { return m_bPaused; }
	inline double getAnimationInterval(void) { return m_dAnimationInterval; }
	virtual void setAnimationInterval(double dValue);

	CCPoint convertToGL(const CCPoint& obPoint);
	CCPoint convertToUI(const CCPoint& obPoint);

public:
	void runWithScene(CCScene* pScene);
	void pushScene(CCScene* pScene);
	void popScene();
	void replaceScene(CCScene* pScene);

public:
	void end();
	void pause();
	void resume();
	void stopAnimation();
	void startAnimation();

protected:
	void purgeDirector();
	void setNextScene();

protected:
	CCEGLView* m_pobOpenGLView;
	CCSize m_obWinSizeInPoints;
	bool m_bPaused;
	bool m_bInvalid;
	double m_dAnimationInterval;
	double m_dOldAnimationInterval;
	bool m_bPurgeDirecotorInNextLoop;

	CCMutableArray<CCScene*> *m_pobScenesStack;
	CCScene* m_pRunningScene;
	CCScene* m_pNextScene;
};

NS_CC_END;