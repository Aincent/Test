/*
 * MicroRewardLoader.h
 *
 *  Created on: 2015年9月17日
 *      Author: root
 */

#ifndef MICROREWARDLOADER_H_
#define MICROREWARDLOADER_H_

#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

typedef struct sMicroRewardInfo
{
	double addExpRatio;
	vector<DWORD> itemID;
	vector<DWORD> itemNum;
	bool isBind; // 是否绑定
	sMicroRewardInfo() : addExpRatio(0), isBind(false)
	{
		itemID.clear();
		itemNum.clear();
	}
}MicroRewardInfo;

class MicroRewardLoader : public Singleton<MicroRewardLoader>
{
public:
	int Init(string& path);
	int InitFile(string& file);
	int InitLua(string& file);

	void GetMicroReward(MicroRewardInfo& rewardInfo);
public:
	MicroRewardLoader();
	~MicroRewardLoader();

private:
	MicroRewardInfo m_rewardInfo;
};

#endif /* MICROREWARDLOADER_H_ */
