/*
 * ClanLoader.h
 *
 *  Created on: 2014年9月30日
 *      Author: helh
 */

#ifndef CLANLOADER_H_
#define CLANLOADER_H_

#include "Singleton.h"
#include "define.h"
#include <vector>
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;

//升级信息
struct UpdateInfo
{
	BYTE lv;
	BYTE icon;
	DWORD money;
	DWORD golden;
	BYTE person;

	int zhp;
	int zpattack;
	int zmattack;
	int zpdefence;
	int zmdefence;
	int zhrate;
	int zavoid;
	int zcrack;
	int zacrack;

	int hp;
	int pattack;
	int mattack;
	int pdefence;
	int mdefence;
	int hrate;
	int avoid;
	int crack;
	int acrack;
};
//坐标点
struct pont
{
	int xpos;
	int ypos;
};

struct CityInfo
{
	int nId; //城市id
	WORD nLv;	//等级
	string sName; //城市名称
	int nCon; //帮贡奖励
	int nConCoefficient; //帮贡系数
	int nMoney; //铜钱奖励
	int nMoneyCoefficient; //铜钱系数
	int nForce; //真气奖励
	int nForceCoefficient; //真气系数
	int nExp; //经验奖励
	int nExpCoefficient; //经验系数
	int nInstance; //对应副本

	CityInfo():nId(0),nLv(0),nCon(0),nConCoefficient(0),nMoney(0),nMoneyCoefficient(0),nForce(0),nForceCoefficient(0),nExp(0),nExpCoefficient(0),nInstance(0)
	{
		sName.clear();
	}
};

struct ClanShop
{
	int nItemId; //道具ID
	int nLimitNum; //数量限制
	int nPrice;	//道具单价
	int nBindType;//绑定类型

	ClanShop():nItemId(0),nLimitNum(0),nPrice(0),nBindType(0)
	{

	}
};


struct ClanWarRewardItem//帮派战结束时的物品奖励
{
	std::vector<pair<int,int> > items;
};

struct BossInfo
{
	int monsterId;
	pont pos;

	BossInfo():monsterId(0)
	{

	}
};

struct IntegralRewards
{
	int nType;
	int nTarget;
	std::vector<DWORD> itemList;//奖励的物品列表
	std::vector<DWORD> num;//数量

	IntegralRewards():nType(0),nTarget(0)
	{
		itemList.clear();
		num.clear();
	}
};


struct IntegralMVPRewards
{
	int nLv;
	std::vector<DWORD> itemList;//奖励的物品列表
	std::vector<DWORD> num;//s_probability

	IntegralMVPRewards():nLv(0)
	{
		itemList.clear();
		num.clear();
	}
};

enum
{
	eCamp_Red = 1,		//帮派战中的红方
	eCamp_Blue = 2,	//帮派战中的蓝方
};

struct	BatterWarConfig
{
	BatterWarConfig(){
		occypytime = 0;
		protectime = 0;
		scoretime = 0;
		occupyscores = 0;
		killscores = 0;
		winscore = 0;
		redCampnpcId = 0;
		blueCampnpcId = 0;
		boxsnum = 0;
		flushtime = 0;
		mailid = 0;
	}
	int occypytime;		//占领时间
	int protectime;		//战旗保护时间
	int scoretime;			//占领战旗加积分间隔时间
	int occupyscores;	//战旗间隔时间加的积分
	int killscores; 		//杀人积分
	int winscore;				//胜利达到的积分
	int redCampnpcId;	//红方id
	int blueCampnpcId;//蓝方id
	int boxsnum;				//宝箱数量
	int flushtime;			//宝箱刷新时间
	int mailid;					//邮件
};

class ClanLoader : public Singleton<ClanLoader>
{
public:
	ClanLoader():m_openLv(0),m_createMoney(0),m_createGolden(0),m_createClanItemID(0),m_refreshTime(0)
	{
		m_lua.OpenLua();
	}
	~ClanLoader()
	{
		m_lua.CloseLua();
	}

	int Init(string &path);
	int InitUpdate(string & file);
	int InitLua(string & file);
	int InitManorLua(string & file);
	int InitGangShop(string& file);
	int InitGangRewardItem(string& file);
	int InitIntegralLua(string& file);
	int InitRewardsPRBang(string& file);
	int InitMvpRewardsPRBang(string& file);

	BYTE GetLv() { 	return m_openLv; }
	int GetCreateMoney() { 	return m_createMoney; }
	int GetCreateGolden() { 	return m_createGolden; }
	int GetCreateCost(bool type)
	{
		if(type)
			return m_createGolden;

		return m_createMoney;
	}

	UpdateInfo * GetUpdateInfo(BYTE lv)
	{
		if(lv > m_updateInfo.size())
			return 0;

		return &m_updateInfo[lv - 1];
	}

	//随即坐标点集合
	vector<pont> GetAllRandomPos(){return m_allRandomPos;}
	pont * GetRandomPos()
	{
		if(m_allRandomPos.size() <= 0)
			return 0;

		int randNum = rand() % m_allRandomPos.size();

		return &m_allRandomPos[randNum];
	}

	void GetClanShopById(int nItemId,ClanShop& pShop)
	{
		map<int,ClanShop>::iterator itr =  m_clanShop.find(nItemId);
		if(itr != m_clanShop.end())
		{
			pShop = itr->second;
		}
	}

	int getCreateClanItemId() const {
		return m_createClanItemID;
	}

	const ClanWarRewardItem * GetClanWarRewardItem(bool isWin);

	int GetRefreshTime()
	{
		return m_refreshTime;
	}
	//杀人积分
	int GetScorePeople(int nNum);
	//杀BOSS积分
	int GetScoreBoss(int nNum);
	//生存积分
	int GetScoreAlive(int nNum,int nTime);
	//帮贡奖励
	int GetCotriReward(int nNum);
	//铜钱奖励
	int GetMoneyReward(int nNum);
	//真气奖励
	int GetGasReward(int nNum);
	//帮派结算奖励
	int GetMoneyScore(int nNum);
	//胜利积分计算
	int GetVictoryScore(int scorePeople,int scoreBoss,int scoreAlive);
	//失败积分计算
	int GetFailScore(int scorePeople,int scoreBoss,int scoreAlive);
	//BOSS刷新后全属性增长倍率
	int GetBossRate(int nNum);
	//BOSS出生点
	const map<int,BossInfo>* GetBossInfo();
	//获取随机出生点
	void GetRandomPos(int64& nXPos,int64& nYPos,bool bFlag);
	//获取mvp奖励
	int GetMVPRewards(int nLv);
	//获取单杀连杀奖
	const IntegralRewards* GetIntegralRewards(int nType,int nTarget);

	//根据红蓝方得到出生点
	void GetBirthPos(int camp, int64& posx, int64& posy);

	const BatterWarConfig& GetBatterConfig(){ return m_config; }

	//获得宝箱的位置信息
	void GetClanWarBoxPos(vector<pont>& poss)const { poss = m_clanWarBoxsPos; }

private:

	BYTE m_openLv;
	int m_createMoney;
	int m_createGolden;
	int m_createClanItemID;

	vector<UpdateInfo> m_updateInfo;
	//领地变量
private:
	vector<pont> m_allRandomPos;				//随即坐标
	map<int,ClanShop> m_clanShop; 		//帮贡换购
	CLuaInterface m_lua;

	int m_refreshTime;
	vector<pont> m_attVec;
	vector<pont> m_defVec;
	map<int,BossInfo> m_bossInfo;
	map<int,int> m_bossRate;
	vector<IntegralRewards> m_integralRewards;
	vector<IntegralMVPRewards> m_integralMVPRewards;

	BatterWarConfig m_config;							//霸业战信息
	vector<pont> m_clanWarRedCampPos;	//霸业战红方出生点
	vector<pont> m_clanWarBlueCampPos;//霸业战蓝方出生点
	vector<pont> m_clanWarBoxsPos;			//霸业战宝箱的刷出位置
	map<int,ClanWarRewardItem> m_clanWarRewardItem;//帮派战胜利或失败后的物品奖励
 };






#endif /* CLANLOADER_H_ */
