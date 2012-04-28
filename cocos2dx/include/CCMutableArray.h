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

		// �����������
		unsigned int count(void)
		{
			return (unsigned int)m_array.size();
		}

		// �ж�ָ��Ԫ���Ƿ��ڸ�������
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

		// ��ȡ���һ��Ԫ��
		T getLastObject(void)
		{
			std::vector<T>::reverse_iterator iter = m_array.rbegin();

			if (iter != m_array.rend())
				return *iter;

			return 0;
		}

		// ��ȡָ������ֵ��Ԫ��
		T getObjectAtIndex(unsigned int uIndex)
		{
			assert(uIndex < count());

			if (uIndex >= count())
			{
				return 0;
			}

			return m_array[uIndex];
		}

		// ���һ��Ԫ��
		void addObject(T pObject)
		{
			if (pObject == NULL)
				return;

			pObject->retain();
			m_array.push_back(pObject);
		}

		// �����е������У�������Ԫ��
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

		// ��Ԫ�ز��뵽����ָ��λ��
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

		// ɾ�����һ��Ԫ��
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

		// ɾ��ָ��Ԫ��
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

		// ɾ��������ָ��������Ԫ����ͬ��Ԫ��
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

		// ɾ��ָ��������Ԫ��
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

		// ɾ������Ԫ��
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

		// �滻ָ��λ�õ�Ԫ��
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

		// ���Ƹ�����
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
		// ���ݲ���������Ԫ�ش�������
		static CCMutableArray<T>* arrayWithObjects(T pObject1, ...)
		{
			CCMutableArray<T> *pArray = new CCMutableArray<T>();		//�˴��ݲ���ʹ���ڴ���Զ��ͷŹ��ܣ�mark���ⲿ���ֶ�delete

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

		// ����һ�����鴴�����飬��copy��ͬ
		static CCMutableArray<T>* arrayWithArray(CCMutableArray<T> *pSrcArray)
		{
			CCMutableArray<T> *pDestArray = 0;

			if (pSrcArray == 0)
				pDestArray = new CCMutableArray<T>();		//�˴��ݲ���ʹ���ڴ���Զ��ͷŹ��ܣ�mark���ⲿ���ֶ�delete
			else
				pDestArray = pSrcArray->copy();

			return pDestArray;
		}

	private:
		std::vector<T> m_array;
	};
}