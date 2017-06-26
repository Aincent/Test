/*
 * GsCountryWarLoader.h
 *
 *  Created on: 2015年4月20日
 *      Author: root
 */

#ifndef GSCOUNTRYWARLOADER_H_
#define GSCOUNTRYWARLOADER_H_
#include <map>
#include "define.h"
#include "Singleton.h"
#include "CharDefine.h"
#include "CountryDefine.h"
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;

class CountryWarLoader : public Singleton<CountryWarLoader>
{
public:
	CountryWarLoader();
	~CountryWarLoader();
public:
	const CountryWarTaskConfigInfo* getTaskConfig(int taskID);
	const CountryWarConfig& getWarConfig(){return m_warConfig;}
	int getScoreByMonster(int monID);
	void getTaskList(std::vector<int>& tasks);

	//经验区每隔多长时间加一次经验
	int getZonePrizeTime();
	int getZoneExp(int lv,int rate);
	int getZoneForce(int lv,int rate);

	//阵营奖励积分贡献度时间间隔
	int getPrizeTime();
	//根据人数奖励阵营积分
	int getScoreByPlayerNum(int num);
	//根据人数奖励个人贡献度
	int getContributeByNum(int num);
	//安全区奖励经验倍数
	int getSafeZonePrizeRate(){return m_warConfig.safeZonePrizeRate;}
	//非安全区奖励经验倍数
	int getNoSafeZonePrizeRate(){return m_warConfig.noSafeZonePrizeRate;}

	void GetRandomPos(int64& nXPos,int64& nYPos,int country);
	bool IsSafeZone(int64 nXPos,int64 nYPos,int country);
	void GetWarMapInfo(map<int,CountryWarMapInfo>& warMapInfo);


private:
	bool loadTaskConfig();
	bool loadWarConfig();
	bool loadWarMapInfo();
private:
	std::map<int,CountryWarTaskConfigInfo> m_taskInfo;
	CountryWarConfig m_warConfig;
	std::map<int,CountryWarMapInfo> m_warMapInfo;
	CLuaInterface m_pLua;
};


#endif /* GSCOUNTRYWARLOADER_H_ */
