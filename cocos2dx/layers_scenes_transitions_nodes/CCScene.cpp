#include "CCScene.h"
#include "CCPointExtension.h"
#include "CCDirector.h"

NS_CC_BEGIN;

CCScene::CCScene()
{
	m_bIsRelativeAnchorPoint = false;
	setAnchorPoint(ccp(0.5f, 0.5f));
}

CCScene::~CCScene()
{

}

CCScene* CCScene::node()
{
	CCScene *pRet = new CCScene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool CCScene::init()
{
	bool bRet = false;
	do 
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		CC_BREAK_IF(NULL == pDirector);
		this->setContentSize(pDirector->getWinSize());
		bRet = true;
	} while (0);
	return bRet;
}

NS_CC_END;