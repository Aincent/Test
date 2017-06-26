/*
 * Ciconv.h
 *
 *  Created on: 2015年7月17日
 *      Author: root
 */

#ifndef CICONV_H_
#define CICONV_H_

#include "Common.h"
#include <iconv.h>
#include "define.h"

using namespace std;
using namespace CommBaseOut;

enum E_CONVER_TYPE
{
	eConverGbToUtf8,
	eConverUtf8ToGb,
	eConverUtf8ToCP1258,
};

class CIconv
{
public:
	static int Iconv(char* inBuf, size_t inBufLen, char* outBuf, size_t* outBufLen, E_CONVER_TYPE type);
	static int Iconv(std::string inStr, std::string& outStr, E_CONVER_TYPE type);

	static int GetUTF8Count(const char * str);
private:
	CIconv();
	~CIconv();
};

#define ICONV  CIconv::Iconv

#endif /* CICONV_H_ */
