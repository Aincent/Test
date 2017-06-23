/*
 * UrlEncode.h
 *
 *  Created on: 2015年12月11日
 *      Author: root
 */

#ifndef URLENCODE_H_
#define URLENCODE_H_
#include <string>
using namespace std;
class UrlEncode
{
public:
	static string Encode(const string& szToEncode);
	static string Decode(const string& szToDecode);
};


#endif /* URLENCODE_H_ */
