/*
 * Ciconv.cpp
 *
 *  Created on: 2015年7月17日
 *      Author: root
 */

#include "Ciconv.h"
#include "Log/Logger.h"


CIconv::CIconv() {
	// TODO Auto-generated constructor stub
}

CIconv::~CIconv() {
	// TODO Auto-generated destructor stub
}

int CIconv::Iconv(char* inBuf, size_t inBufLen, char* outBuf, size_t* outBufLen, E_CONVER_TYPE type)
{
	iconv_t cd;
	switch (type)
	{
	case eConverGbToUtf8:
		{
			cd = iconv_open("UTF-8", "GB2312");
		}
		break;
	case eConverUtf8ToGb:
		{
			cd = iconv_open("GB2312", "UTF-8");
		}
		break;
	case eConverUtf8ToCP1258:
	{
		cd = iconv_open("cp1258", "UTF-8");
	}
		break;
	default:
		return -1;
	}
	if(0 == cd)
		return -1;
	char* in = inBuf, * out = outBuf;
	int ret = 0;

	ret = (int)(iconv(cd, &in, &inBufLen, &out, outBufLen));

	iconv_close(cd);
	return ret;
}

int CIconv::Iconv(std::string inStr, std::string& outStr, E_CONVER_TYPE type)
{
	iconv_t cd;
	switch (type)
	{
	case eConverGbToUtf8:
		{
			cd = iconv_open("UTF-8", "GB2312");
		}
		break;
	case eConverUtf8ToGb:
		{
			cd = iconv_open("GB2312", "UTF-8");
		}
		break;
	case eConverUtf8ToCP1258:
	{
		cd = iconv_open("cp1258", "UTF-8");
	}
		break;
	default:
		return -1;
	}

	char inBuf[1024] = {0};
	size_t inLen = inStr.length();
	size_t outLen = 1024;
	char outBuf[1024] = {0};
	int ret = 0;
	char* out = NULL;
	char* in = NULL;

	strcpy(inBuf, inStr.c_str());
	in = inBuf;
	out = outBuf;

	if (NULL == out || NULL == in)
	{
		ret = -1;
	}
	else
	{
		int tempRet = (int)(iconv(cd, &in, &inLen, &out, &outLen));
		if (-1 != tempRet)
		{
			outStr = outBuf;
		}
		else
		{
			LOG_ERROR(FILEINFO, "invoke iconv error:%s", strerror(errno));
			ret = -1;
		}
	}

	iconv_close(cd);
	return ret;
}

unsigned char g_utf8Table[]=
{
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,

4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

int CIconv::GetUTF8Count(const char * str)
{
	if(NULL == str)
		return 0;

	int clen = strlen(str);
	int len = 0;
	int realLen = 0;

	for(const char *ptr = str; *ptr!=0&&realLen<clen;ptr+=g_utf8Table[(unsigned char)*ptr])
	{
		realLen += (int)g_utf8Table[(unsigned char)*ptr];

		if((int)g_utf8Table[(unsigned char)*ptr] >= 3)
			len += 2;
		else
			len += 1;
	}

	return len;
}
