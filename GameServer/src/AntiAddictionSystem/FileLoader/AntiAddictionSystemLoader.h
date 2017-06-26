/*
 * AntiAddictionSystemLoader.h
 *
 *  Created on: 2014年10月28日
 *      Author: root
 */

#ifndef ANTIADDICTIONSYSTEMLOADER_H_
#define ANTIADDICTIONSYSTEMLOADER_H_

#include <vector>
#include "Singleton.h"
#include <string>

using namespace CommBaseOut;
using namespace std;

class AntiAddictionSystemLoader : public Singleton<AntiAddictionSystemLoader>
{
public:
	AntiAddictionSystemLoader();
	~AntiAddictionSystemLoader();

	int InitAntiAddictionSystem(string &file);

	bool IsCityCodeExit(int cityCode);
private:
	//各城市在身份证上的代号
	std::vector<int> m_cityCodeList;
};

#endif /* ANTIADDICTIONSYSTEMLOADER_H_ */
