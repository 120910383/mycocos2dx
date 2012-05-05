#pragma once

#include "CCObject.h"
#include "CCMutableArray.h"

namespace cocos2d
{
	// 自动释放内存池
	class CCAutoreleasePool : public CCObject
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
}