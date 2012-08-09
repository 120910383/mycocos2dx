#include "CCFileUtils.h"
#include <windows.h>
#include "CCString.h"

NS_CC_BEGIN;

static bool s_bPopupNotify = true;
static char s_pszResourcePath[MAX_PATH] = {0};

namespace
{
	void _CheckPath()
	{
		if (0 == s_pszResourcePath[0])
		{
			WCHAR wszPath[MAX_PATH];
			// 路径问题 cocos2d-x => mine
			//int nNum = WideCharToMultiByte(CP_ACP, 0, wszPath, 
			//	GetCurrentDirectoryW(sizeof(wszPath), wszPath), 
			//	s_pszResourcePath, MAX_PATH, NULL, NULL);
			//s_pszResourcePath[nNum] = '\\';

			WideCharToMultiByte(CP_ACP, 0, wszPath, 
				GetModuleFileNameW(NULL, wszPath, sizeof(wszPath)), 
				s_pszResourcePath, MAX_PATH, NULL, NULL);
			char* pChar = strrchr(s_pszResourcePath, '\\');
			if (NULL != pChar)
				*pChar = '\0';
			strcat(s_pszResourcePath, "\\Resources\\");
		}
	}
}

unsigned char* CCFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long* pSize)
{
	unsigned char* pBuffer = NULL;
	do 
	{
		FILE* fp = fopen(pszFileName, pszMode);
		CC_BREAK_IF(NULL == fp);

		fseek(fp, 0, SEEK_END);
		*pSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		pBuffer = new unsigned char[*pSize];
		*pSize = fread(pBuffer, sizeof(unsigned char), *pSize, fp);
		fclose(fp);
	} while (0);

	if (NULL == pBuffer && getIsPopupNotify())
	{
		std::string title = "Notification";
		std::string msg = "Get data from file(";
		msg.append(pszFileName).append(") failed!");
		MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_OK);
	}
	return pBuffer;
}

void CCFileUtils::setResourcePath(const char *pszResourcePath)
{
	CCAssert(NULL != pszResourcePath, "[FileUtils setResourcePath] -- wrong resource path");
	CCAssert(strlen(pszResourcePath) <= MAX_PATH, "[FileUtils setResourcePath] -- resource path too long");

	strcpy(s_pszResourcePath, pszResourcePath);
}

const char* CCFileUtils::fullPathFromRelativePath(const char *pszRelativePath)
{
	_CheckPath();
	
	CCString* pRet = new CCString();
	pRet->autorelease();
	if ((strlen(pszRelativePath) > 1 && pszRelativePath[1] == ':'))
	{
		pRet->m_sString = pszRelativePath;
	}
	else if (strlen(pszRelativePath) > 0 && ('/' == pszRelativePath[0] || '\\' == pszRelativePath[0]))
	{
		char szDriver[3] = {s_pszResourcePath[0], s_pszResourcePath[1], 0};
		pRet->m_sString = szDriver;
		pRet->m_sString += pszRelativePath;
	}
	else
	{
		pRet->m_sString = s_pszResourcePath;
		pRet->m_sString += pszRelativePath;
	}

	return pRet->m_sString.c_str();
}

void CCFileUtils::setIsPopupNotify(bool bNotify)
{
	s_bPopupNotify = bNotify;
}

bool CCFileUtils::getIsPopupNotify()
{
	return s_bPopupNotify;
}

NS_CC_END;