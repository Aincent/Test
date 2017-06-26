/*
 * Distortion.cpp
 *
 *  Created on: 2015年12月10日
 *      Author: root
 */
#include "Distortion.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/Monster/Monster.h"
#include "../ServerEventDefine.h"

PlayerDistortion::PlayerDistortion(Player* player):m_player(player)
{
	ReInit();
}

PlayerDistortion::~PlayerDistortion()
{
	Release();
}

void PlayerDistortion::ReInit()
{
	m_monsterID = 0;
}

void PlayerDistortion::Release()
{
	if(m_timeID.IsValid())
	{
		m_timeID.SetNull();
	}
}

void PlayerDistortion::Start(int monster)
{
	m_player->SetCreatureState(ERS_Distortion);

	m_monsterID = monster;

	if(m_timeID.IsValid())
	{
		m_timeID.SetNull();
	}

	m_timeID = TimerInterface::GetInstance()->RegRepeatedTime(&PlayerDistortion::OnTimer, this, 0, 0, 0, 0, 30000, 30000);

}

void PlayerDistortion::End()
{
	//移除玩家状态
	m_player->UnsetCreatureState(ERS_Distortion);

	m_monsterID = 0;

	if(m_timeID.IsValid())
	{
		m_timeID.SetNull();
	}

}

bool PlayerDistortion::IscanMove()
{
	if(!m_player->IsInCreatureState(ERS_Distortion))
	{
		return true;
	}

	MonsterInfo info;
	MonsterLoader::GetInstance()->GetMonsterInfo(m_monsterID,info);

	if(info.quality != eBoad)
	{
		return false;
	}

	return true;
}

void PlayerDistortion::OnTimer(void * args)
{
	int tempLevel = m_player->getAttManage()->getValue<int>(eCharLv);
	DWORD addexp = tempLevel * 6 + 4;
	//DWORD addforce = tempLevel * 4 + 3;

	// 是否是VIP 打坐加成
	double tRate = 0;//m_player->GetVipPart()->GetValueByRight(eVipMeditationForceAdd);

	//总的加成等于 vip加成、 修炼类型加成、区域加成
	//double forceRate = m_player->getAttManage()->getValue<double>(eForceRate);
	double expRate = m_player->getAttManage()->getValue<double>(eMeditationExpRate);

	//forceRate = forceRate > 0 ? forceRate : 0;
	expRate = expRate > 0 ? expRate : 0;
	tRate = tRate > 0 ? tRate : 0;

	//forceRate += tRate + rate;
	expRate += tRate;

	addexp = (DWORD)((1.0f + expRate) * addexp);

	//addforce = (DWORD)((1.0f + forceRate) * addforce);

	//std::vector<int> vecType;
	m_player->AddExp(addexp);
	//m_player->ChangeCurForce(addforce, true);

	//vecType.push_back(eCharForce);
	//m_player->SynCharAttribute(vecType);

	Smart_Ptr<Player> object = m_player->GetMyself();
	if (object)
	{
		MeditationEventArgs argsd(object, addexp, 0);
		FireGlobalEvent(PLAYER_MEDITATION_INCOME, argsd);
	}
}



