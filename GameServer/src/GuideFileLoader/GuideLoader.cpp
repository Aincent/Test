/*
 * GuideLoader.cpp
 *
 *  Created on: 2016年1月8日
 *      Author: root
 */

#include "GuideLoader.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"

GuideLoader::GuideLoader():m_playerLv(1),m_needKillCount(0),m_bossId(0),m_DelayTime(0)
{
	m_pLua.OpenLua();

	LoadLua();
	Load();
}

GuideLoader::~GuideLoader()
{
	m_pLua.CloseLua();
}

void GuideLoader::Load()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return;
	}
	string strFile = strFileFullPath + "Data/Guide.txt";
	CSVLoader loader('	');

	if(!loader.OpenFromFile(strFile.c_str()))
	{
		return;
	}

	//职业	性别	神兵	时装	武器	坐骑	机器人名字	技能信息	攻击	防御	命中	闪避	暴击	抗暴	HP	MP
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		int count = 0;

		GuiderBaseInfo info;
		info.profession = loader.GetInt(i, count++);
		info.sex = loader.GetInt(i, count++);
		info.magic = loader.GetInt(i, count++);
		info.fashion = loader.GetInt(i, count++);
		info.weapon  = loader.GetInt(i, count++);
		info.hourse = loader.GetInt(i, count++);
		info.name = loader.GetString(i, count++);

		info.attack = loader.GetInt(i,count++);
		info.defene  = loader.GetInt(i, count++);
		info.hitRate = loader.GetInt(i, count++);
		info.avoidRate = loader.GetInt(i, count++);
		info.DownRate  = loader.GetInt(i, count++);
		info.AvoidCrackRate = loader.GetInt(i, count++);
		info.hp = loader.GetInt(i, count++);
		info.mp = loader.GetInt(i, count++);

		int index = info.profession<<16 | info.sex;
		m_guider_info[index] = info;
	}

}

void GuideLoader::LoadLua()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return;
	}

	string strFile = strFileFullPath + "Data/Guide.lua";
	if(m_pLua.LoadFile(strFile) != 0 || m_pLua.Call() != 0)
	{
		return;
	}

	//玩家等级
	m_pLua.SetLuaFunction("GetLv");
	if(m_pLua.Call(0, 1))
	{
		return;
	}
	m_playerLv = m_pLua.GetInt();

	//boss id
	m_pLua.SetLuaFunction("GetBossID");
	if(m_pLua.Call(0, 1))
	{
		return;
	}
	m_bossId = m_pLua.GetInt();

	//刷二波怪需要击杀的小怪
	m_pLua.SetLuaFunction("GetNeedCount");
	if(m_pLua.Call(0, 1))
	{
		return;
	}
	m_needKillCount = m_pLua.GetInt();

	//第二波怪物的延迟刷新时间
	m_pLua.SetLuaFunction("GetDelayTimes");
	if(m_pLua.Call(0, 1))
	{
		return;
	}
	m_DelayTime = m_pLua.GetInt();

	//刷出友方2个NPC怪物的位置
	m_pLua.SetLuaFunction("GetNpcPos");
	if(m_pLua.Call(0,1))
	{
		return;
	}

	int tableCount = m_pLua.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_pLua.GetFieldTable(i) != 0)
		{
			return;
		}

		int posx = m_pLua.GetFieldInt("posx");
		int posy = m_pLua.GetFieldInt("posy");
		m_positions.push_back(make_pair(posx,posy));
		m_pLua.CloseTable();
	}
	m_pLua.CloseTable();

	//第二波怪物刷出的id及位置坐标
	m_pLua.SetLuaFunction("GetSecondMonsters");
	if(m_pLua.Call(0, 1))
	{
		return;
	}

	tableCount = m_pLua.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_pLua.GetFieldTable(i) != 0)
		{
			return;
		}

		GuideMonsterData node;
		node.id = m_pLua.GetFieldInt("monsterid");
		node.posx = m_pLua.GetFieldInt("posx");
		node.posy = m_pLua.GetFieldInt("posy");
		m_secondMonsters.push_back(node);
		m_pLua.CloseTable();
	}
	m_pLua.CloseTable();

	//第三波怪物
	m_pLua.SetLuaFunction("GetThirdMonsters");
	if(m_pLua.Call(0, 1))
	{
		return;
	}

	tableCount = m_pLua.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_pLua.GetFieldTable(i) != 0)
		{
			return;
		}

		GuideMonsterData node;
		node.id = m_pLua.GetFieldInt("monsterid");
		node.posx = m_pLua.GetFieldInt("posx");
		node.posy = m_pLua.GetFieldInt("posy");
		m_thirdMonsters.push_back(node);
		m_pLua.CloseTable();
	}
	m_pLua.CloseTable();


	// 玩家增加的属性
	m_pLua.SetLuaFunction("GetAttrs");
	if(m_pLua.Call(0, 1))
	{
		return;
	}

	tableCount = m_pLua.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_pLua.GetFieldTable(i) != 0)
		{
			return;
		}

		int id = m_pLua.GetFieldInt("attrid");
		int value = m_pLua.GetFieldInt("value");
		m_add_attrs.push_back(make_pair(id,value));
		m_pLua.CloseTable();
	}
	m_pLua.CloseTable();

	//玩家职业及技能
	m_pLua.SetLuaFunction("GetSkill");
	if(m_pLua.Call(0, 1))
	{
		return;
	}

	char buffer[128] = {0};
	tableCount = m_pLua.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_pLua.GetFieldTable(i) != 0)
		{
			return;
		}

		memset(buffer, 0, 128);
		int profession = m_pLua.GetFieldInt("profession");
		m_pLua.GetFieldString(buffer, 128, "skillid");

		std::vector<int> skillInfo;
		string strSkill = buffer;
		StrArgPkg strPkg(":", strSkill);
		for(size_t n=0; n<strPkg.size(); ++n)
		{
			skillInfo.push_back( atoi(getArg(strPkg,n).c_str()) );
		}

		m_add_skills[profession] = skillInfo;
		m_pLua.CloseTable();
	}
	m_pLua.CloseTable();

}

const GuiderBaseInfo* GuideLoader::GetGuiderInfo(int profession, int sex)
{
	int index = profession<<16 | sex;
	std::map<int,GuiderBaseInfo>::const_iterator it = m_guider_info.find(index);
	if(it != m_guider_info.end())
	{
		return &it->second;
	}
	return NULL;
}




