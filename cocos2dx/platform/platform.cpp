#include "platform.h"

#include "CCStdC.h"

NS_CC_BEGIN;

int CC_DLL CCTime::gettimeofdayCocos2d(struct cc_timeval* tp, void* tzp)
{
	CC_UNUSED_PARAM(tzp);
	if (tp)
	{
		gettimeofday((struct  timeval*)tp, 0);
	}
	return 0;
}

NS_CC_END;