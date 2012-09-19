#pragma once

#include "CCNode.h"

NS_CC_BEGIN;

class CC_DLL CCMenuItem : public CCNode
{
public:
	CCMenuItem();
	virtual ~CCMenuItem();

	virtual void selected();
	virtual void unselected();
	virtual bool getIsSelected();

protected:
	bool m_bIsSelected;
};

NS_CC_END;