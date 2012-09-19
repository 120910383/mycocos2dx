#pragma once

#include "CCNode.h"

NS_CC_BEGIN;

//////////////////////////////////////////////////////////////////////////
// 基类，自定义的item均可从次继承
class CC_DLL CCMenuItem : public CCNode
{
public:
	static CCMenuItem* itemWithTarget(CCObject* rec, SEL_MenuHandler selector);

public:
	CCMenuItem();
	virtual ~CCMenuItem();
	bool initWithTarget(CCObject* rec, SEL_MenuHandler selector);

	virtual void activate();

	virtual void selected();
	virtual void unselected();
	virtual bool getIsSelected();

	virtual void setIsEnabled(bool enabled);
	virtual bool getIsEnabled();

	void setTarget(CCObject* rec, SEL_MenuHandler selector);

protected:
	bool m_bIsSelected;
	bool m_bIsEnabled;
	CCObject* m_pListener;
	SEL_MenuHandler m_pfnSelector;
};


//////////////////////////////////////////////////////////////////////////
// 字符串菜单，具有点击执行自动放大缩小动画的特性
class CC_DLL CCMenuItemLabel : public CCMenuItem
{
public:
	static CCMenuItemLabel* itemWithLabel(CCNode* label);
	static CCMenuItemLabel* itemWithLabel(CCNode* label, CCObject* target, SEL_MenuHandler selector);

public:
	CCMenuItemLabel();
	virtual ~CCMenuItemLabel();
	bool initWithLabel(CCNode* label, CCObject* target, SEL_MenuHandler selector);

	void setString(const char* label);

	virtual void setLabel(CCNode* label);
	virtual CCNode* getLabel();

protected:
	CCNode* m_pLabel;
};


//////////////////////////////////////////////////////////////////////////
// 精灵菜单，可呈现三种状态精灵(正常，选中，非使能)
class CC_DLL CCMenuItemSprite : public CCMenuItem
{
public:
	static CCMenuItemSprite* itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite = NULL);
	static CCMenuItemSprite* itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector);
	static CCMenuItemSprite* itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);

public:
	CCMenuItemSprite();
	virtual ~CCMenuItemSprite();
	bool initFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);

	virtual void setNormalImage(CCNode* normalSprite);
	virtual CCNode* getNormalImage();

	virtual void setSelectedImage(CCNode* selectedSprite);
	virtual CCNode* getSelectedImage();

	virtual void setDisabledImage(CCNode* disabledSprite);
	virtual CCNode* getDisabledImage();

protected:
	CCNode* m_pNormalImage;
	CCNode* m_pSelectedImage;
	CCNode* m_pDisabledImage;
};

NS_CC_END;