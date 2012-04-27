#include "CCObject.h"
#include "CCZone.h"
#include <cassert>

namespace cocos2d
{
	CCObject* CCCopying::copyWithZone(CCZone* pZone)
	{
		assert(false);			// ������ʵ��copyWithZone
		return 0;
	}

	CCObject::CCObject(void)
	{
		m_uReference = 1;			// ����������ʱ���ö�������ü���Ϊ1
	}

	CCObject::~CCObject(void)
	{

	}

	void CCObject::release(void)
	{
		assert(m_uReference > 0);			//���ü���Ӧ��0��
		--m_uReference;

		if (m_uReference == 0)
		{
			delete this;
		}
	}

	void CCObject::retain(void)
	{
		assert(m_uReference > 0);			//���ü���Ӧ��0��

		++m_uReference;
	}

	CCObject* CCObject::copy(void)
	{
		return copyWithZone(NULL);
	}
}