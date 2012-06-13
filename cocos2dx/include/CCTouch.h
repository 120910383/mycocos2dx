#pragma once

#include "CCObject.h"
#include "CCGeometry.h"

NS_CC_BEGIN;

class CCTouch : public CCObject
{
public:
	CCTouch() {}
	CCTouch(int nViewId, float x, float y) : m_nViewId(nViewId), m_point(x, y), m_prevPoint(x, y) {}

	CCPoint locationInView(int nViewId) { CC_UNUSED_PARAM(nViewId); return m_point; }
	CCPoint previousLocationInView(int nViewId) { CC_UNUSED_PARAM(nViewId); return m_prevPoint; }
	int view() { return m_nViewId; }
	int id() { return m_iID; }

	void SetTouchInfo(int nViewId, float x, float y, int iID = 0)
	{
		m_nViewId = nViewId;
		m_prevPoint = m_point;
		m_point.x = x;
		m_point.y = y;
		m_iID = iID;
	}

private:
	int m_nViewId;
	CCPoint m_point;
	CCPoint m_prevPoint;
	int m_iID;
};

// 为了适配cocos2d定义了CCEvent, CCEvent在cocos2d-x中没有实现
class CCEvent : public CCObject
{

};

NS_CC_END;