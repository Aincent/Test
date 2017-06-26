/*
 * ArenaMgr.cpp
 *
 *  Created on: 2016年5月4日
 *      Author: root
 */

#include "ArenaMgr.h"
#include "../VIP/PlayerVipPart.h"
#include "../Object/Player/Player.h"
#include "../Counter/Counter.h"
#include "../ServerEventDefine.h"
#include "FileLoader/ArenaLoader.h"
#include "FileLoader/GameParaLoader.h"

ArenaMgr::ArenaMgr(Player& owner) : m_owner(owner), m_canChallCnt(0), m_lastResetTime(0)
{
	// TODO Auto-generated constructor stub

}

ArenaMgr::~ArenaMgr()
{
	// TODO Auto-generated destructor stub
}

void ArenaMgr::InitArenaData(const PlayerInfo::ArenaDataInfo& info)
{
	m_canChallCnt = info.canchallcnt();
	m_lastResetTime = info.lastresettime();
}

void ArenaMgr::SaveArenaData(PlayerInfo::ArenaDataInfo& info)
{
	info.set_canchallcnt(m_canChallCnt);
	info.set_lastresettime(m_lastResetTime);
}

bool ArenaMgr::IsAddChall()
{
	int baseCnt = GetBaseSumChallCnt();
	if (-1 == baseCnt)
	{
		return false;
	}

	if (baseCnt <= m_canChallCnt)
	{
		return false;
	}

	return true;
}

int ArenaMgr::AddChallCnt()
{
	int golden = 0;
	vector<int> vecBro;
	if(!IsAddChall())
	{
		return eCompeteInstanceCounterNotUse;
	}
	else if (-1 == ArenaLoader::GetInstance()->GetArenanumInfo(m_owner.GetCounterService()->GetNumById(ARENA_BUYCHALLENGE_CNT)+ 1,golden))
	{
		return -1;
	}
	else if (!m_owner.IsEnoughGolden(golden))
	{
		return eNotEnogGolden;
	}

	int costGolden = 0;
	int costbGolden = 0;

	m_owner.DecreaseGoldenCost(golden, costGolden, costbGolden);
	if(costGolden > 0)
		vecBro.push_back(eCharGolden);
	if(costbGolden > 0)
		vecBro.push_back(eCharBindGolden);

	AddChallCnt(1);

	m_owner.SynCharAttribute(vecBro);
//	m_owner.GetCounterService()->DecreasePlayerCounter(COMPETE_INSTANCE_COUNTER, addNum);
	m_owner.GetCounterService()->IncreasePlayerCounter(ARENA_BUYCHALLENGE_CNT, 1);

	ArenaAddChallArgs arg(m_owner.GetCreatureSelf(), 1);
	m_owner.FireEvent(PLAYER_ADD_ARENACHALL, arg);

	return 0;
}

void ArenaMgr::AddChallCnt(int cnt)
{
	m_canChallCnt += cnt;

	m_owner.SetDataFlag(eArenaInfo);
}

int ArenaMgr::RedChallCnt(int redCnt)
{
	int suplus = m_canChallCnt -= redCnt;

	m_owner.SetDataFlag(eArenaInfo);

	return suplus;
}
int ArenaMgr::GetCanChallCnt()
{
	return m_canChallCnt;
}

void ArenaMgr::Reset()
{
	if (CUtil::IsSameDay(m_lastResetTime))
	{
		return;
	}

	int cnt = GetBaseSumChallCnt();
	if (-1 == cnt)
	{
		return;
	}

	m_canChallCnt = cnt;

	m_lastResetTime = CUtil::GetNowSecond();

	m_owner.SetDataFlag(eArenaInfo);
}

int ArenaMgr::GetBaseSumChallCnt()
{
	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GameParaLoader::GetInstance()->getArenaMapId());
	if (NULL == entry)
	{
		return -1;
	}

	int vipTimes = 0, realReapt = 0;
	if (m_owner.GetVipPart() && 0 < m_owner.getAttManage()->getValue<DWORD>(eVIPLv))
	{
		double times = m_owner.GetVipPart()->GetValueByRight(eVipArenaCanChallenge);
		if (VIP_NOT_HAVE_THIS_RIGHT != times)
		{
			vipTimes = (int)times;
		}
	}

	if (vipTimes > 0)
	{
		realReapt = vipTimes;
	}
	else
	{
		realReapt = (int)entry->m_nReaptTimes;
	}

	return realReapt;
}



