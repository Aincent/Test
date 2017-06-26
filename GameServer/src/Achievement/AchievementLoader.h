/*
 * AchievementLoader.h
 *
 *  Created on: 2014年6月19日
 *      Author: root
 */

#ifndef ACHIEVEMENTLOADER_H_
#define ACHIEVEMENTLOADER_H_

#include "define.h"
#include <map>
#include <vector>
#include <string>
#include "../Map/ConfigLoader/FieldInfo.h"
#include "Smart_Ptr.h"
#include "../Map/ConfigLoader/FieldMonster.h"
#include "MessageStruct/AchievementProto.pb.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "Singleton.h"

using namespace CommBaseOut;
using namespace std;

enum eAchievementType
{
	AchievementType_Level = 1,		//等级
	AchievementType_KillMonster = 2, //杀怪
	AchievementType_Quest	= 3,		//任务数
	AchievementType_Instance = 4,		//通关副本数
	AchievementType_ChannelUpdate = 5,//经脉升级
	AchievementTyp_KillBoss = 6,		//击杀BOSS
	AchievementType_Strength = 7,		//强化
	AchievementType_Xilian	= 8,		//洗练
	AchievementType_KillPlayers = 9,	//击杀玩家数量
	AchievementType_SinValue = 10,      //罪恶值
	AchievementType_FightPower = 11,//战斗力
	AchievementType_PassiveSkill = 12,//被动技能满级
	AchievementType_ActiveSKill = 13,//主动技能满级
	AchievementType_DailyQuest = 14,//日常任务
	AchievementType_ClanQuest = 15,//帮派任务
	AchievementType_SignIn = 16,//签到
	AchievementType_HorseUpgrade = 17,//坐骑升级
	AchievementType_ChallengeInstance = 18,//通关挑战
	AchievementType_RobEscort = 19,//劫取皇纲
	AchievementType_ClanPeopleCount = 20,//帮派人数
	AchievementType_ClanWarWinCount = 21,//帮派战胜利次数
	AchievementType_SetStoneCount = 22,//宝石镶嵌个数
	AchievementType_WearLegendEquip = 23,//传戴10件传说装备
	AchievementType_WearStrengthLegendEquip = 24,//传戴10件满强化传说装备
	AchievementType_MagicWeaponAdvance = 25,//神兵升阶段
	AchievementType_ReflushMagicWeaponSkill = 26,//刷新出橙色神兵技能
	AchievementType_CompleteQuest = 27,//完成指定的任务
	AchievementType_ClanUpgrade = 28,//帮派升级到10
};

enum AchievementUseType
{
	AchievementUseType_1,		// 还没有获取
	AchievementUseType_2,		// 获取了，但还没有领取
	AchievementUseType_3,		// 领取了

	AchievementUseType_s
};

struct sAchievementEntry
{
	int i_achID;				//成就ID
	string s_achname;			//成就名称
	eAchievementType i_target;		//成就目标
	int i_targetnumber;			//获得成就取得目标的数量
	string i_description;		//成就描述
	DWORD i_exp;				//经验
	DWORD i_money;				//金钱
	DWORD i_zhenqi;			//真气
	DWORD i_BGolden;			//绑定元宝
	DWORD i_prestige;			//声望数
	DWORD i_classification;	//所属分类
	DWORD i_Amount;			//此分类总的成就数
	DWORD i_titleID;			//对应的称号ID
};

struct sTitleEntry
{
	//
	DWORD i_titleID;		//称号ID
	DWORD i_resID;			//资源ID
	string i_titlename;	//称号名称
	DWORD i_prestige;		//声望
	DWORD	i_exp;			//经验
	DWORD i_money;			//金钱
	DWORD i_zhenqi;		//真气
	DWORD i_achID;			//对应的成就ID
	int phyack;					//物理攻击
	int magack;					//法攻
	int phydee;					//物理防御
	int magdee;					//法防
	int hp;
};

struct sAchGoodsItem
{
	int id;
	int goodsid;				//物品ID
	int money;						//所需铜钱
};

struct sAchShopItem
{
	int id;									//区间类型
	int count;							//购买次数
	map<int,sAchGoodsItem> goodsItem;
	void RandomGoods(vector<int> &vec)
	{
		vec.clear();

		vector<int> temp;
		map<int,sAchGoodsItem>::iterator it = goodsItem.begin();
		for(;it!=goodsItem.end();it++)
		{
			temp.push_back(it->first);
		}
		for(BYTE i = 0 ;i < 4;i++)
		{
			int index = rand()%(temp.size()-i);
			int item = temp[index];
			vec.push_back(item);
			temp[index] = temp[temp.size()-i-1];
			temp[temp.size()-i-1] = item;
		}
	}
};

struct sAchShopInfo
{
	int id;
	int count;
	vector<int> goodsid;
	sAchShopInfo():id(0),count(0)
	{
		goodsid.clear();
	}
};

//奖励是否领取
struct sPrestigeContent
{
	int achievementType;	//成就类型
	DWORD	   achievementId;				//ID
	DWORD		achievementPercent;		//进度
	bool		isComplete;					//是否完成
	int     UseType;	//领取状态
	sPrestigeContent():achievementType(0),achievementId(0),achievementPercent(0),isComplete(false),
			UseType(0)
	{

	}
	sPrestigeContent(PlayerInfo::PrestigeContent *prestigeInfo)
	{
		achievementType = prestigeInfo->prestigetype();
		achievementId = prestigeInfo->prestigeid();
		achievementPercent = prestigeInfo->num();
		isComplete = (bool)prestigeInfo->iscomplete();
		UseType = prestigeInfo->hadget();
	}
	void SetProto(PlayerInfo::PrestigeContent &prestigeInfo)
	{
		prestigeInfo.set_prestigetype(achievementType);
		prestigeInfo.set_prestigeid(achievementId);
		prestigeInfo.set_num(achievementPercent);
		prestigeInfo.set_iscomplete(isComplete);
		prestigeInfo.set_hadget(UseType);
	}
};

class AchievementLoader : public Singleton<AchievementLoader>
{
public:
	AchievementLoader();
	~AchievementLoader();
public:
	typedef std::map<int,sPrestigeContent> AchievementMap;
public:
	int InitAchievement(string &file);
	int InitTitle(string &file);
	int InitShop(string &file);
	int Init(string &file);

	sAchievementEntry *GetAchievementEntry(DWORD id);
	sTitleEntry *GetTitleEntry(DWORD id);

	void GetAllAchievementID(std::vector<DWORD> & ids);
	void GetAchievementTypeIDs(int type,std::vector<DWORD> & ids);
	void GetAllTitleID(std::vector<DWORD> & ids);
	map<DWORD, sTitleEntry> & GetTitleInfo(void)
	{
		return m_TitleInfo;
	}

	int GetNeedGolden()
	{
		return m_golden;
	}
	void UpdataShop(map<int,sAchShopInfo> &info);
	int GetAchByType(int type)
	{
		map<int,int>::iterator it = m_typeAndAch.find(type);
		if(it != m_typeAndAch.end())
		{
			return it->second;
		}
		return 999999;
	}
	void GetsAchShopItem(int type ,sAchShopItem &item)
	{
		map<int,sAchShopItem>::iterator it = m_AchShopInfo.find(type);
		if(it != m_AchShopInfo.end())
		{
			item = it->second;
		}
	}

	const AchievementMap& getAchievementIdMap() const
	{
		return mAchievementIDMap;
	}

private:
	// 所有成就  ID为KEY
	AchievementMap mAchievementIDMap;
	map<DWORD, sAchievementEntry> m_AchievementInfo;   // sAchievementEntry - info
	map<DWORD, sTitleEntry> m_TitleInfo;   // sTitleEntry - info
	map<int,sAchShopItem> m_AchShopInfo;
	map<int,int> m_typeAndAch;
	int m_golden;
};

#endif /* ACHIEVEMENTLOADER_H_ */
