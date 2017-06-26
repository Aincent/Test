/*
 * DuplicateMgr.h
 *
 *  Created on: 2014年10月14日
 *      Author: helh
 */

#ifndef DUPLICATEMGR_H_
#define DUPLICATEMGR_H_

#include "define.h"
#include "GameZone.h"
#include "SceneManager.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "Singleton.h"
#include <map>
#include "GameDuplicate.h"
#include "../ServerOtherDefine.h"

using namespace std;
using namespace CommBaseOut;

class DuplicateMgr : public Singleton<DuplicateMgr>
{
public:
	DuplicateMgr();
	~DuplicateMgr();

	//判断是否可以创建副本
	bool IsCreateInstance(int64 charid);
	//删除将要创建副本列表
	void DeleteWillCreateInstance(int64 charid)
	{
		INT64VINT64MAP::iterator it = m_instanceWill.find(charid);
		if(it != m_instanceWill.end() && it->second.size() > 0)
		{
			it->second.erase(it->second.begin());
		}
	}
	//加入将创建副本列表
	void AddWillCreateInstance(int64 charid)
	{
		INT64VINT64MAP::iterator it = m_instanceWill.find(charid);
		if(it != m_instanceWill.end())
		{
			it->second.push_back(CUtil::GetNowSecond());
		}
		else
		{
			vector<int64> vec;
			vec.push_back(CUtil::GetNowSecond());
			m_instanceWill[charid] = vec;
		}
	}

	//是否处于创建副本中
	bool IsInWillCreateInstance(int64 charid)
	{
		INT64VINT64MAP::iterator it = m_instanceWill.find(charid);
		if(it != m_instanceWill.end())
		{
			if(it->second.size() > 0)
				return true;
		}

		return false;
	}

	//将副本加入管理器
	void AddMap(Smart_Ptr<CGameDuplicate> & map)
	{
		m_duplicate[map->GetSceneID()] = map;
	}

	//从管理器里面删除副本
	void DeleteMap(int64 instance);
	//从管理器中获取副本
	Smart_Ptr<CGameDuplicate> GetDuplicate(int64 instance)
	{
		INT64GAMEDUPLICATEMAP::iterator it = m_duplicate.find(instance);
		if(it != m_duplicate.end())
		{
			return it->second;
		}

		return 0;
	}

	void UpdateTimer(void * arg);
	void UpdateTimer();

private:

	INT64VINT64MAP m_instanceWill; //这些玩家将要创建副本（charid－time）
	INT64GAMEDUPLICATEMAP m_duplicate; //存在的副本
	TimerConnection m_updateTime; //更新所有副本的定时器
};



#endif /* DUPLICATEMGR_H_ */
