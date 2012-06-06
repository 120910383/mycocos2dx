#pragma once

#include "CCNode.h"

NS_CC_BEGIN;

class CC_DLL CCScene : public CCNode
{
public:
	static CCScene *node(void);
	CCScene();
	virtual ~CCScene();
	bool init();
};

NS_CC_END;