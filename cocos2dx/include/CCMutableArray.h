#pragma once

#include "CCObject.h"
#include <vector>
#include <stdarg.h>

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
			std::vector<T>::iterator iter;
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
			std::vector<T>::reverse_iterator iter = m_array.rbegin();

			if (iter != m_array.rend())
				return *iter;

			return 0;
		}

		// 获取指定索引值的元素
		T getObjectAtIndex(unsigned int uIndex)
		{
			assert(uIndex < count());

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
				std::vector<T>::iterator iter;
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
			assert(uIndex <= count());
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
			std::vector<T>::reverse_iterator it = m_array.rbegin();
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

			std::vector<T>::iterator iter;
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
				std::vector<T>::iterator it;
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
			std::vector<T> tmp_array;
			m_array.swap(tmp_array);

			if (bDeleteObject)
			{
				std::vector<T>::iterator iter;
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

		// 复制该数组
		CCMutableArray<T>* copy(void)
		{
			CCMutableArray* pArray = new CCMutableArray();

			pArray->m_array.assign(m_array.begin(), m_array.end());

			if(pArray->count() > 0)
			{
				std::vector<T>::iterator it;
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
			CCMutableArray<T> *pArray = new CCMutableArray<T>();		//此处暂不可使用内存池自动释放功能，mark，外部需手动delete

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
				pDestArray = new CCMutableArray<T>();		//此处暂不可使用内存池自动释放功能，mark，外部需手动delete
			else
				pDestArray = pSrcArray->copy();

			return pDestArray;
		}

	private:
		std::vector<T> m_array;
	};
}