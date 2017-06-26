/*
 * PlayerMeditation.cpp
 *
 *  Created on: 2015年11月2日
 *      Author: root
 */
#include "PlayerMeditation.h"
#include "MessageStruct/CharBase/MeditationProto.pb.h"
#include "../Attribute/AttributeCreature.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "../Map/GameZone.h"
#include "MeditationMgr.h"
#include "../ServerEventDefine.h"
#include "../StatisticMgr/StatisticHandle.h"

PlayerMeditation::PlayerMeditation(Player* player):m_player(player)
{
	ReInit();
}

PlayerMeditation::~PlayerMeditation()
{
	Release();
}

void PlayerMeditation::ReInit()
{
	m_meditationForce = 0;
	m_targetID = 0;
}

void PlayerMeditation::Release()
{
	if(m_MeditationTimeID.IsValid())
	{
		m_MeditationTimeID.SetNull();
	}
}

void PlayerMeditation::BroadCastSingleMeditation(bool StartFlag, bool SendToSelf)
{
	MeditationProto::SingleMeditation msg;
	msg.set_charkey(m_player->GetKey());
	msg.set_flag(StartFlag);

	Smart_Ptr<GirlWar> girlwar = m_player->GetGirlWarManager()->GetGirlWar();
	if(girlwar.Get() != NULL)
	{
		msg.set_girlwarkey(girlwar->GetKey());
	}
	else
	{
		msg.set_girlwarkey(0);
	}
	m_player->SendToAround(&msg, MSG_SIM_GM2C_SINGLE_MEDITATION, SendToSelf);
}

Smart_Ptr<Player> PlayerMeditation::GetTargetInfo()
{
	if(m_targetID==0)
		return NULL;

	CGameZone * pZone = m_player->GetGameZone();
	if(pZone == NULL)
	{
		return NULL;
	}

	return pZone->GetPlayer(m_targetID);
}

int PlayerMeditation::StartMeditationTimer()
{
	//在战斗，或某些BUFF中不能打坐，行走是也不能打坐
	unsigned int state = ERS_Dead | ERS_Fight | ERS_Gather | ERS_Still | ERS_Ice | ERS_Stun | ERS_Distortion;
	if(m_player->IsInCreatureStateAny(state))
	{
		return eNotMeditationInSomeState;
	}
	//已在打坐中
	if (m_MeditationTimeID.IsValid())
	{
		return eIsInMeditation;
	}

	BEHAVIOR_MONI(m_player->GetMyself(), BehaviorType::Main::eZazen, BehaviorType::Minor::eZazen_Zazen, MONITOR);

	//通知取消采集
	Smart_Ptr<Player> object = m_player->GetMyself();
	OneParmaEventArgs args(object);
	FireGlobalEvent(PLAYER_MEDITATION_START, args);

	m_MeditationTimeID = TimerInterface::GetInstance()->RegRepeatedTime(&PlayerMeditation::CallBackMeditation, this, 0, 0, 0, 0, 30000, 30 * 1000);

	m_player->ClearPath();
	m_player->SetCreatureState(ERS_Meditation);

	m_player->SynCharAttribute(eSimpleState, 0, eMeditation);

	BroadCastSingleMeditation(true, true);

	

	return 0;
}

void PlayerMeditation::UpdateSelfDoubleState()
{
	if(m_MeditationTimeID.IsValid())
	{
		m_MeditationTimeID.SetNull();
	}

	m_player->ClearPath();
	m_player->SetCreatureState(ERS_Meditation);
	m_player->SynCharAttribute(eSimpleState, 0, eMeditation);
}

int PlayerMeditation::DeleteMeditationTimer(bool SendToSelf)
{
	BEHAVIOR_MONI(m_player->GetMyself(), BehaviorType::Main::eZazen, BehaviorType::Minor::eZazen_CancelZazen, MONITOR);

	if(m_MeditationTimeID.IsValid())
	{
		m_MeditationTimeID.SetNull();
	}

	m_player->UnsetCreatureState(ERS_Meditation);

	BroadCastSingleMeditation(false, SendToSelf);

	//通知对方
	NoticeTarget();

	//从邀请列表中移出
	MeditationMgr::GetInstance()->RemovInvits(m_player->GetID());

	if(m_meditationForce > 0)
	{
		//取消打坐的时候才发送一次可避免每30秒进行的真气统计
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,m_meditationForce);
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_player->GetID(),eStatic_Force,eStaMinor_Force_Meditation,&costAndGotInfo);
		m_meditationForce = 0;
	}

	

	return 0;
}

void PlayerMeditation::UpdateMeditation()
{
	if(m_player->IsInCreatureState(ERS_Meditation))
	{
		const MeditationConfig* pConfig = MeditationMgr::GetInstance()->GetConfig(eMeditationType_Single);
		if(pConfig != NULL)
		{
			Prize(pConfig->rate, 0);
		}
	}
	else
	{
		DeleteMeditationTimer();
	}

}

void PlayerMeditation::CallBackMeditation(void * obj)
{
	UpdateMeditation();
}

//取消定时器
void PlayerMeditation::CanccelTimer()
{
	if(m_MeditationTimeID.IsValid())
	{
		m_MeditationTimeID.SetNull();
	}
}

//重新开启定时器
void PlayerMeditation::ReStartTimer()
{
	if(m_MeditationTimeID.IsValid())
	{
		m_MeditationTimeID.SetNull();
	}

	m_MeditationTimeID = TimerInterface::GetInstance()->RegRepeatedTime(&PlayerMeditation::CallBackMeditation, this, 0, 0, 0, 0, 30000, 30000);

	BroadCastSingleMeditation(true);
}

void PlayerMeditation::NoticeTarget()
{
	if(GetTargetID() <= 0)
	{
		return;
	}

	//关闭双修定时器 释放内存
	MeditationMgr::GetInstance()->CanccelDoubleMeditation(m_player->GetID(), GetTargetID());

	int64 targetID = GetTargetID();
	SetTargetID(0);

	if(m_player->GetGameZone() == NULL)
	{
		return;
	}

	Smart_Ptr<Player> target = m_player->GetGameZone()->GetPlayer(targetID);
	if(target.Get() == NULL)
	{
		return;
	}

	//设置对方id
	target->GetMeditationMgr()->SetTargetID(0);

	//对方由双修变单修
	target->GetMeditationMgr()->ReStartTimer();
}

void PlayerMeditation::Prize(float rate, int closeValue)
{
	int tempLevel = m_player->getAttManage()->getValue<int>(eCharLv);
	DWORD addexp = tempLevel * 6 + 4;
	DWORD addforce = tempLevel * 4 + 3;

	// 是否是VIP 打坐加成
	double tRate = 0; //m_player->GetVipPart()->GetValueByRight(eVipMeditationForceAdd);

	//总的加成等于 vip加成、 修炼类型加成、区域加成
	double forceRate = m_player->getAttManage()->getValue<double>(eForceRate);
	double expRate = m_player->getAttManage()->getValue<double>(eMeditationExpRate);

	forceRate = forceRate > 0 ? forceRate : 0;
	expRate = expRate > 0 ? expRate : 0;
	rate = rate > 0 ? rate : 0;
	tRate = tRate > 0 ? tRate : 0;

	forceRate += tRate + rate;
	expRate += tRate + rate;

	addexp = (DWORD)((1.0f + expRate) * addexp);

	addforce = (DWORD)((1.0f + forceRate) * addforce);

	std::vector<int> vecType;
	m_player->AddExp(addexp);
	m_player->ChangeCurForce(addforce, true);
	m_meditationForce += addforce;

	vecType.push_back(eCharForce);
	m_player->SynCharAttribute(vecType);

	MeditationEventArgs args(m_player->GetMyself(), addexp, addforce);
	FireGlobalEvent(PLAYER_MEDITATION_INCOME, args);

	//推消息给玩家
	MeditationProto::simMeditationInfo toClient;
	toClient.set_exp(addexp);
	toClient.set_zhenqi(addforce);
	toClient.set_intimate(closeValue);
	toClient.set_rate((int)forceRate);
	QuickMessage::GetInstance()->AddSendMessage(m_player->GetID(), m_player->GetChannelID(),&toClient, MSG_SIM_GM2C_MEDITATIONINFO);
}
