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
		unsigned int		m_uReference;		//引用计数，构造中设置为1，每次release减1，每次retain加1，为0时delete

	public:
		CCObject(void);
		virtual ~CCObject(void);

		void release(void);
		void retain(void);
		CCObject* copy(void);
		bool isSingleRefrence(void);
		unsigned int retainCount(void);
		virtual bool isEqual(const CCObject* pObject);

		friend class CCAutoreleasePool;
	};
}