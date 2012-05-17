#pragma once

#include "CCObject.h"
#include <vector>
#include <stdarg.h>

NS_CC_BEGIN;
// 从代码可以看出，本类型所接受的元素类型只能是CCObject及其派生类型，而不是任意类型，因为元素带有release retain等使用方法
template<class T = CCObject*>
class CCMutableArray : public CCObject
{
public:
	typedef std::vector<T>								CCObjectArray;
	typedef typename CCObjectArray::iterator			CCMutableArrayIterator;
	typedef typename CCObjectArray::reverse_iterator	CCMutableArrayRevIterator;

public:
	CCMutableArray(unsigned int uSize = 0)
	{
		if (uSize != 0)
			m_array.reserve(uSize);
	}

	virtual ~CCMutableArray(void)
	{
		removeAllObjects();
	}

	// 返回数组个数
	unsigned int count(void)
	{
		return (unsigned int)m_array.size();
	}

	// 判断指定元素是否在该数组中
	bool containsObject(T pObject)
	{
		if (m_array.empty() || (! pObject))
		{
			return false;
		}

		bool bRet = false;
		CCMutableArrayIterator iter;
		for (iter = m_array.begin(); iter != m_array.end(); ++iter)
		{
			if (*iter == pObject)
			{
				bRet = true;
				break;
			}
		}

		return bRet;
	}

	// 获取最后一个元素
	T getLastObject(void)
	{
		CCMutableArrayRevIterator iter = m_array.rbegin();

		if (iter != m_array.rend())
			return *iter;

		return 0;
	}

	// 获取指定索引值的元素
	T getObjectAtIndex(unsigned int uIndex)
	{
		CCAssert(uIndex < count(), "");

		if (uIndex >= count())
		{
			return 0;
		}

		return m_array[uIndex];
	}

	// 添加一个元素
	void addObject(T pObject)
	{
		if (pObject == NULL)
			return;

		pObject->retain();
		m_array.push_back(pObject);
	}

	// 从已有的数组中，逐个添加元素
	void addObjectsFromArray(CCMutableArray<T> *pArray)
	{
		if (pArray && pArray->count() > 0)
		{
			m_array.reserve(count() + pArray->count());
			CCMutableArrayIterator iter;
			for (iter = pArray->m_array.begin(); iter != pArray->m_array.end(); ++iter)
			{
				if (*iter)
					(*iter)->retain();
				m_array.push_back(*iter);
			}
		}
	}

	// 将元素插入到数组指定位置
	void insertObjectAtIndex(T pObject, unsigned int uIndex)
	{
		CCAssert(uIndex <= count(), "");
		if (pObject == 0)
			return;

		pObject->retain();
		if (uIndex >= m_array.capacity())
		{
			m_array.reserve(uIndex + 1);
			m_array.push_back(pObject);
		}
		else
			m_array.insert(m_array.begin() + uIndex, pObject);
	}

	// 删除最后一个元素
	void removeLastObject(bool bDeleteObject = true)
	{
		CCMutableArrayRevIterator it = m_array.rbegin();
		if (it != m_array.rend())
		{
			if (bDeleteObject)
				(*it)->release();
			m_array.pop_back();
		}
	}

	// 删除指定元素
	void removeObject(T pObject, bool bDeleteObject = true)
	{
		if (m_array.empty() || (! pObject))
			return;

		CCMutableArrayIterator iter;
		for (iter = m_array.begin(); iter != m_array.end(); ++iter)
		{
			if (*iter == pObject)
			{
				m_array.erase(iter);
				if (bDeleteObject)
					pObject->release();
				break;
			}
		}
	}

	// 删除所有与指定数组中元素相同的元素
	void removeObjectsInArray(CCMutableArray<T>* pDeleteArray)
	{
		if(pDeleteArray && pDeleteArray->count())
		{
			CCMutableArrayIterator it;
			for( it = pDeleteArray->m_array.begin(); it != pDeleteArray->m_array.end(); ++it)
			{
				removeObject(*it);
			}
		}
	}

	// 删除指定索引的元素
	void removeObjectAtIndex(unsigned int uIndex, bool bDeleteObject = true)
	{
		if (m_array.empty())
			return;

		if (bDeleteObject)
		{
			T pObject = m_array.at(uIndex);
			if (pObject)
				pObject->release();
		}

		m_array.erase(m_array.begin() + uIndex);
	}

	// 删除所有元素
	void removeAllObjects(bool bDeleteObject = true)
	{
		CCObjectArray tmp_array;
		m_array.swap(tmp_array);

		if (bDeleteObject)
		{
			CCMutableArrayIterator iter;
			for (iter = tmp_array.begin(); iter != tmp_array.end(); ++iter)
				(*iter)->release();
		}		
	}

	// 替换指定位置的元素
	void replaceObjectAtIndex(unsigned int uIndex, T pObject, bool bDeleteObject = true)
	{
		if (bDeleteObject && m_array[uIndex])
		{
			m_array[uIndex]->release();
		}

		m_array[uIndex] = pObject;

		if (pObject)
			pObject->retain();
	}

	// 一组适配函数，让外界可以像访问标准vector一样访问该类
	inline CCMutableArrayIterator begin(void)
	{
		return m_array.begin();
	}

	inline CCMutableArrayRevIterator rbegin(void)
	{
		return m_array.rbegin();
	}

	inline CCMutableArrayIterator end(void)
	{
		return m_array.end();
	}

	inline CCMutableArrayRevIterator rend(void)
	{
		return m_array.rend();
	}

	// 复制该数组
	CCMutableArray<T>* copy(void)
	{
		CCMutableArray* pArray = new CCMutableArray();

		pArray->m_array.assign(m_array.begin(), m_array.end());

		if(pArray->count() > 0)
		{
			CCMutableArrayIterator it;
			for(it = pArray->m_array.begin(); it != pArray->m_array.end(); ++it)
			{
				if(*it)
				{
					(*it)->retain();
				}
			}
		}

		return pArray;
	}

public:
	// 根据不定个数的元素创建数组
	static CCMutableArray<T>* arrayWithObjects(T pObject1, ...)
	{
		CCMutableArray<T> *pArray = new CCMutableArray<T>();
		pArray->autorelease();

		va_list params;
		va_start(params, pObject1);

		T pFirst = pObject1;
		while (pFirst)
		{
			pArray->addObject(pFirst);
			pFirst = va_arg(params, T);
		}

		va_end(params);

		return pArray;
	}

	// 根据一个数组创建数组，与copy相同
	static CCMutableArray<T>* arrayWithArray(CCMutableArray<T> *pSrcArray)
	{
		CCMutableArray<T> *pDestArray = 0;

		if (pSrcArray == 0)
			pDestArray = new CCMutableArray<T>();
		else
			pDestArray = pSrcArray->copy();

		pDestArray->autorelease();
		return pDestArray;
	}

private:
	CCObjectArray m_array;
};
NS_CC_END;