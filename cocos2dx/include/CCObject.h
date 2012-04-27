#pragma once

namespace cocos2d
{
	class CCZone;
	class CCObject;

	class CCCopying
	{
	public:
		virtual CCObject* copyWithZone(CCZone* pZone);
	};

	class CCObject : public CCCopying
	{
	protected:
		unsigned int		m_uReference;		//���ü���������������Ϊ1��ÿ��release��1��ÿ��retain��1��Ϊ0ʱdelete

	public:
		CCObject(void);
		virtual ~CCObject(void);

		void release(void);
		void retain(void);
		CCObject* copy(void);
	};
}