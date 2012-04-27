#pragma once

#include "CCObject.h"

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

	private:
		std::vector<T> m_array;
	};
}