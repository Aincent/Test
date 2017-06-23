/*
 * InternationalLoader.h
 *
 *  Created on: 2015年1月23日
 *      Author: root
 */

#ifndef INTERNATIONALLOADER_H_
#define INTERNATIONALLOADER_H_

#include <map>
#include <string>
#include "Singleton.h"

using namespace CommBaseOut;
using namespace std;

struct sInternationalEntry
{
	int id;
	//根据配置转化为指定国家的语言
	std::string internationalStr;
};

class InternationalLoader : public Singleton<InternationalLoader>
{
public:
	InternationalLoader();
	~InternationalLoader();
public:
	int InitInternational(string &file);
	sInternationalEntry * GetInternationalEntry(int id);
	string GetInterStr(int id);
private:
	map<int,sInternationalEntry> m_internationalMap;
};


#endif /* INTERNATIONALLOADER_H_ */
