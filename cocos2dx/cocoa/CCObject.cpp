#include "CCObject.h"
#include "CCZone.h"
#include <cassert>
#include "CCMutableArray.h"	//编译测试用

namespace cocos2d
{
	CCObject* CCCopying::copyWithZone(CCZone* pZone)
	{
		assert(false);			// 基类无实现copyWithZone
		return 0;
	}

	CCObject::CCObject(void)
	{
		m_uReference = 1;			// 当创建对象时，该对象的引用计数为1

		//////////////////////////////////////////////////////////////////////////测试CCMutableArray的各个方法编译错误,待测试工程建立后再考虑内存释放
		CCObject* obj1 = new CCObject;
		CCObject* obj2 = new CCObject;
		CCObject* obj3 = new CCObject;
		CCMutableArray<CCObject*>* test_array = CCMutableArray<CCObject*>::arrayWithObjects(obj1, obj2, NULL);
		CCMutableArray<CCObject*>* test_array1 = new CCMutableArray<CCObject*>();

		if (NULL == test_array || NULL == test_array1)
			return;

		test_array->addObject(obj1);
		unsigned int n = test_array->count();
		CCObject* last = test_array->getLastObject();
		CCObject* obj = test_array->getObjectAtIndex(0);
		test_array1->addObjectsFromArray(test_array);
		test_array->addObject(obj2);
		test_array->insertObjectAtIndex(obj3, 5);
		test_array->removeLastObject();
		test_array->removeObject(obj2);
		test_array->addObject(obj2);
		test_array->removeObjectsInArray(test_array1);
		test_array->removeObjectAtIndex(1);
		test_array1->removeAllObjects();
		test_array->replaceObjectAtIndex(0, obj3);
		CCMutableArray<CCObject*>* test_array2 = test_array->copy();
		CCMutableArray<CCObject*>* test_array3 = CCMutableArray<CCObject*>::arrayWithArray(test_array2);

		///////////////////////////////////////////////////////////////////////////测试结束
	}

	CCObject::~CCObject(void)
	{

	}

	void CCObject::release(void)
	{
		assert(m_uReference > 0);			//引用计数应比0大
		--m_uReference;

		if (m_uReference == 0)
		{
			delete this;
		}
	}

	void CCObject::retain(void)
	{
		assert(m_uReference > 0);			//引用计数应比0大

		++m_uReference;
	}

	CCObject* CCObject::copy(void)
	{
		return copyWithZone(NULL);
	}

	bool CCObject::isSingleRefrence(void)
	{
		return m_uReference == 1;
	}

	unsigned int CCObject::retainCount(void)
	{
		return m_uReference;
	}

	bool CCObject::isEqual(const CCObject* pObject)
	{
		return this == pObject;
	}
}