#pragma once

//
// all cocos2d include files
//
#include "CCDirector.h"
#include "CCNode.h"
#include "CCScene.h"
#include "CCLayer.h"
#include "CCTouchHandler.h"
#include "CCTouchDelegateProtocol.h"
#include "CCTouchDispatcher.h"
#include "CCTouch.h"
#include "CCSprite.h"
#include "CCTextureCache.h"
#include "CCLabelTTF.h"
#include "CCMenu.h"
#include "CCMenuItem.h"
#include "CCScheduler.h"

//
// cocoa includes
//
#include "CCMutableArray.h"
#include "CCMutableDictionary.h"
#include "CCObject.h"
#include "CCZone.h"
#include "CCGeometry.h"
#include "CCAffineTransform.h"
#include "CCPointExtension.h"

//
// platform specific
//
#include "CCApplication.h"
#include "CCEGLView.h"
#include "CCFileUtils.h"

//
// cocos2d macros
//
#include "ccTypes.h"
#include "ccMacros.h"

//
// 版本标识
//
NS_CC_BEGIN;
const char* cocos2dVersion();
NS_CC_END;
