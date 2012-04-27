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

		// �����������
		unsigned int count(void);

		// �ж�ָ��Ԫ���Ƿ��ڸ�������
		bool containsObject(T pObject);

		// ��ȡ���һ��Ԫ��
		T getLastObject(void);

		// ��ȡָ������ֵ��Ԫ��
		T getObjectAtIndex(unsigned int uIndex);

		// ���һ��Ԫ��
		void addObject(T pObject);

		// �����е������У�������Ԫ��
		void addObjectsFromArray(CCMutableArray<T> *pArray);

		// ��Ԫ�ز��뵽����ָ��λ��
		void insertObjectAtIndex(T pObject, unsigned int uIndex);

		// ɾ�����һ��Ԫ��
		void removeLastObject(bool bDeleteObject = true);

		// ɾ��ָ��Ԫ��
		void removeObject(T pObject, bool bDeleteObject = true);

		// ɾ��������ָ��������Ԫ����ͬ��Ԫ��
		void removeObjectsInArray(CCMutableArray<T>* pDeleteArray);

		// ɾ��ָ��������Ԫ��
		void removeObjectAtIndex(unsigned int uIndex, bool bDeleteObject = true);

		// ɾ������Ԫ��
		void removeAllObjects(bool bDeleteObject = true);

		// �滻ָ��λ�õ�Ԫ��
		void replaceObjectAtIndex(unsigned int uIndex, T pObject, bool bDeleteObject = true);

		// ���Ƹ�����
		CCMutableArray<T>* copy(void);

	public:
		// ���ݲ���������Ԫ�ش�������
		static CCMutableArray<T>* arrayWithObjects(T pObject1, ...);

		// ����һ�����鴴�����飬��copy��ͬ
		static CCMutableArray<T>* arrayWithArray(CCMutableArray<T> *pSrcArray);

	private:
		std::vector<T> m_array;
	};
}