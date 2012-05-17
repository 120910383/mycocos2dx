#pragma once

#include "CCObject.h"
#include "CCMutableArray.h"

NS_CC_BEGIN;
// 自动释放内存池
class CC_DLL CCAutoreleasePool : public CCObject
{
public:
	CCAutoreleasePool(void);
	~CCAutoreleasePool(void);

	void addObject(CCObject* pObject);
	void removeObject(CCObject* pObject);
	void clear();

private:
	CCMutableArray<CCObject*>* m_pManagedObjectArray;
};

// 自动释放内存池的管理类，由于在游戏运行中可以根据不同场景或条件使用多个内存池，因此增加一个管理类对内存池进行包装管理
class CC_DLL CCPoolManager
{
public:
	CCPoolManager(void);
	~CCPoolManager(void);
	void finalize();								// 全部清空
	void push();								// 新增一个内存池
	void pop();									// 销毁一个内存池

	void removeObject(CCObject* pObject);
	void addObject(CCObject* pObject);

	static CCPoolManager* getInstance();			// 提供一个静态方法，返回全局对象=>“内存池管理对象”

private:
	CCAutoreleasePool* getCurReleasePool();			//当前正在使用的内存池

private:
	CCMutableArray<CCAutoreleasePool*>*			m_pReleasePoolStack;
	CCAutoreleasePool*											m_pCurReleasePool;
};
NS_CC_END;