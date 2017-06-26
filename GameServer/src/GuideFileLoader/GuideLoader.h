/*
 * GuideLoader.h
 *
 *  Created on: 2016年1月8日
 *      Author: root
 */

#ifndef GUIDELOADER_H_
#define GUIDELOADER_H_

#include <vector>
#include <set>
#include "define.h"
#include "Singleton.h"
#include "util.h"
#include "Lua/LuaInterface.h"

using namespace std;
using namespace CommBaseOut;

struct GuiderBaseInfo
{
	GuiderBaseInfo()
	{
		profession = 0;
		sex = 0;
		magic = 0;
		fashion = 0;
		weapon = 0;
		hourse = 0;
		attack = 0;
		defene = 0;
		hitRate = 0;
		avoidRate = 0;
		DownRate = 0;
		AvoidCrackRate = 0;
		mp = 0;
		hp = 0;
	}

	int profession;//职业
	int sex;//性别
	int magic;//神兵
	int fashion;//时装
	int weapon;//武器
	int hourse;//坐骑

	int attack;//攻击
	int defene;//防御
	int hitRate;//命中率
	int avoidRate;//闪避率
	int DownRate;//暴击
	int AvoidCrackRate;//抗暴
	int mp;//mp
	int hp;//hp

	std::vector<int> skills;//技能信息

	std::string name;//名字
};

struct GuideMonsterData
{
	GuideMonsterData()
	{
		id = 0;
		posx = 0;
		posy = 0;
	}
	int id;
	int posx;
	int posy;
};

class GuideLoader : public Singleton<GuideLoader>
{
public:
	GuideLoader();
	~GuideLoader();

public:
	const GuiderBaseInfo* GetGuiderInfo(int profession, int sex);

	const std::vector<GuideMonsterData>& GetSecondMonsters(){ return m_secondMonsters; }

	const std::vector<GuideMonsterData>& GetThirdMonsters(){ return m_thirdMonsters; }

	const std::vector< pair<int,int> >& GetPosition(){ return m_positions; }

	const std::vector< pair<int,int> >& GetAddAttrs(){ return m_add_attrs; }

	const std::map<int, vector<int> >& GetAddSkills(){ return m_add_skills; }

	int GetNeedKillCount(){ return m_needKillCount; }

	int GetBossID(){ return m_bossId; }

	int GetDelayTime(){ return m_DelayTime; }

private:
	void Load();

	void LoadLua();

private:
	CLuaInterface m_pLua;

	int m_playerLv;																				//玩家等级
	int m_needKillCount;																	//杀死多少只小怪后刷第二波怪物
	int m_bossId;																					//最后通关需要击杀的boss ID
	int m_DelayTime;																			//npc刷出后第二波怪物刷新的延迟时间

	std::map<int,GuiderBaseInfo> m_guider_info;			//友方npc怪物信息
	std::vector< pair<int,int> >	m_positions;				//友方npc怪物刷出位置
	std::vector<GuideMonsterData> m_secondMonsters;	//第二波怪物刷出的位置及Id
	std::vector<GuideMonsterData> m_thirdMonsters;	//第三波怪物位置及id
	std::vector< pair<int,int> > m_add_attrs;				//玩家增加的属性
	std::map<int, vector<int> >  m_add_skills;				//玩家增加的技能
};



#endif /* GUIDELOADER_H_ */
