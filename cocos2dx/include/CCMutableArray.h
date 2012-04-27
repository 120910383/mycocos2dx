#pragma once

#include "CCObject.h"
#include <vector>

namespace cocos2d
{
	template<class T = CCObject*>
	class CCMutableArray : public CCObject
	{
	public:
		CCMutableArray(unsigned int uSize = 0)
		{
			if (uSize != 0)
				m_array.reserve(uSize);
		}

		virtual ~CCMutableArray(void)
		{

		}

		// 返回数组个数
		unsigned int count(void);

		// 判断指定元素是否在该数组中
		bool containsObject(T pObject);

		// 获取最后一个元素
		T getLastObject(void);

		// 获取指定索引值的元素
		T getObjectAtIndex(unsigned int uIndex);

		// 添加一个元素
		void addObject(T pObject);

		// 从已有的数组中，逐个添加元素
		void addObjectsFromArray(CCMutableArray<T> *pArray);

		// 将元素插入到数组指定位置
		void insertObjectAtIndex(T pObject, unsigned int uIndex);

		// 删除最后一个元素
		void removeLastObject(bool bDeleteObject = true);

		// 删除指定元素
		void removeObject(T pObject, bool bDeleteObject = true);

		// 删除所有与指定数组中元素相同的元素
		void removeObjectsInArray(CCMutableArray<T>* pDeleteArray);

		// 删除指定索引的元素
		void removeObjectAtIndex(unsigned int uIndex, bool bDeleteObject = true);

		// 删除所有元素
		void removeAllObjects(bool bDeleteObject = true);

		// 替换指定位置的元素
		void replaceObjectAtIndex(unsigned int uIndex, T pObject, bool bDeleteObject = true);

		// 复制该数组
		CCMutableArray<T>* copy(void);

	public:
		// 根据不定个数的元素创建数组
		static CCMutableArray<T>* arrayWithObjects(T pObject1, ...);

		// 根据一个数组创建数组，与copy相同
		static CCMutableArray<T>* arrayWithArray(CCMutableArray<T> *pSrcArray);

	private:
		std::vector<T> m_array;
	};
}