#pragma once

#include "ccMacros.h"
#include "ccTypes.h"

NS_CC_BEGIN;
class CCZone;
class CCObject;

class CC_DLL CCCopying
{
public:
	virtual CCObject* copyWithZone(CCZone* pZone);
};

class CC_DLL CCObject : public CCCopying
{
protected:
	unsigned int		m_uReference;		//引用计数，构造中设置为1，每次release减1，每次retain加1，为0时delete
	bool				    m_bManaged;		//内存池管理标志，添加到内存池被管理则置为true，内存池释放后取消管理置为false
public:
	CCObject(void);
	virtual ~CCObject(void);

	void release(void);
	void retain(void);
	CCObject* autorelease(void);
	CCObject* copy(void);
	bool isSingleRefrence(void);
	unsigned int retainCount(void);
	virtual bool isEqual(const CCObject* pObject);

	virtual void update(ccTime dt) { CC_UNUSED_PARAM(dt); }

	friend class CCAutoreleasePool;
};

// 定义类成员函数指针类型
typedef void (CCObject::*SEL_SCHEDULE)(ccTime);
typedef void (CCObject::*SEL_MenuHandler)(CCObject*);

#define schedule_selector(_SELECTOR) (SEL_SCHEDULE)(&_SELECTOR)
#define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)

NS_CC_END;