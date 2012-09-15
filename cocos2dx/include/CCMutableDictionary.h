#pragma once

#include "CCObject.h"
#include <map>
#include <vector>
#include <string>

NS_CC_BEGIN;
class CCString;

// 从代码可以看出，本类型所接受的元素类型只能是CCObject及其派生类型，而不是任意类型，因为元素带有release retain等使用方法
template<class _KeyT, class _ValueT = CCObject*>
class CCMutableDictionary : public CCObject
{
public:
	typedef std::map<_KeyT, _ValueT>				CCObjectMap;
	typedef typename CCObjectMap::iterator		CCObjectMapIter;

public:
	CCMutableDictionary()
	{
		m_bBegin = false;
	}

	virtual ~CCMutableDictionary()
	{
		removeAllObjects();
	}

	// 返回map的元素个数
	unsigned int count()
	{
		return m_map.size();
	}

	// 返回map所有元素的键值key
	std::vector<_KeyT> allKeys()
	{
		std::vector<_KeyT> keys;
		if (m_map.size() > 0)
		{
			CCObjectMapIter iter;
			for (iter = m_map.begin(); iter != m_map.end(); ++iter)
			{
				keys.push_back(iter->first);
			}
		}

		return keys;
	}

	// 返回map中对应值object的所有键key
	// 注意：查找object时使用==来进行比较，因此自定义object需实现==
	// 不过从其他函数来看，已经有使用object->retain()的调用，因此本类
	// 默认的值只能是CCObject及派生类的指针，因此==完全可以满足要求
	std::vector<_KeyT> allKeysForObject(_ValueT object)
	{
		std::vector<_KeyT> keys;
		if (m_map.size() > 0)
		{
			CCObjectMapIter iter;
			for (iter = m_map.begin(); iter != m_map.end(); ++iter)
			{
				if (iter->second == object)
				{
					keys.push_back(iter->first);
				}
			}
		}

		return keys;
	}

	// 通过key获取value
	_ValueT objectForKey(const _KeyT& key)
	{
		CCObjectMapIter iter;

		iter = m_map.find(key);

		if (iter == m_map.end())
			return NULL;

		return iter->second;
	}

	// 添加键值对，不对已有元素修改，key已有则返回false
	bool setObject(_ValueT pObject, const _KeyT& key)
	{
		std::pair<CCObjectMapIter, bool>  pr;

		pr = m_map.insert(Int_Pair(key, pObject));

		if (pr.second)
		{
			pObject->retain();
			return true;
		}

		return false;
	}

	// 根据键值key删除对应的元素
	void removeObjectForKey(const _KeyT& key)
	{
		CCObjectMapIter iter;

		iter = m_map.find(key);

		if (iter == m_map.end())
			return;

		if (NULL != iter->second)
		{
			iter->second->release();
			m_map.erase(iter);
		}
	}

	// 一组适配函数，让外界可以像遍历标准map一样遍历该类元素(用法不同)
	bool begin()
	{
		if (m_map.size() == 0)
			return false;

		m_mapIter = m_map.begin();
		m_bBegin = true;

		return true;
	}

	_ValueT next(_KeyT* key = NULL)
	{
		if (!m_bBegin)
			return NULL;

		_ValueT pObject = m_mapIter->second;

		if (m_mapIter == m_map.end())
		{
			m_bBegin = false;
		}
		else
		{
			if (key)
			{
				*key = m_mapIter->first;
			}

			++m_mapIter;

			if (m_mapIter == m_map.end())
			{
				m_bBegin = false;
			}
		}

		return pObject;
	}

	void end()
	{
		m_bBegin = false;
	}

	// 移除所有元素
	void removeAllObjects()
	{
		if (m_map.size() > 0)
		{
			CCObjectMapIter iter;
			for (iter = m_map.begin(); iter != m_map.end(); ++iter)
			{
				if (NULL != iter->second)
				{
					iter->second->release();
				}
			}
		}

		m_map.clear();
	}
	
public:
	static CCMutableDictionary<_KeyT, _ValueT>* dictionaryWithDictionary(CCMutableDictionary<_KeyT, _ValueT>* srcDict)
	{
		CCMutableDictionary<_KeyT, _ValueT>* pNewDict = new CCMutableDictionary<_KeyT, _ValueT>();

		srcDict->begin();

		_KeyT key;
		_ValueT value;

		while ( NULL != (value = srcDict->next(&key)) )
		{
			pNewDict->setObject(value, key);
		}

		srcDict->end();

		return pNewDict;
	}

protected:
	typedef std::pair<_KeyT, _ValueT> Int_Pair;
	CCObjectMap m_map;
	bool m_bBegin;
	CCObjectMapIter m_mapIter;
};

#define CCDictionary CCMutableDictionary
typedef CCDictionary<std::string, CCString*> CCStringToStringDictionary;

NS_CC_END;