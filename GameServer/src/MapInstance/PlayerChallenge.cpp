/*
 * PlayerChallenge.cpp
 *
 *  Created on: 2016年5月6日
 *      Author: root
 */
#include "PlayerChallenge.h"
#include "../Object/Player/Player.h"
#include "ChallengeMgr.h"
#include "../Attribute/AttributeCreature.h"
#include "../Mail/MailMessage.h"
#include "../ServerEventDefine.h"

PlayerChallenge::PlayerChallenge(Player* player):m_player(player)
{
	ReInit();
}

PlayerChallenge::~PlayerChallenge()
{
	Release();
}

void PlayerChallenge::ReInit()
{
	m_layer = 1;
	m_maxlayer = 1;
	m_startTime = 0;
	m_totalTime = 0;

	if(!m_exitEvent.IsValid())
	{
		m_exitEvent = RegEvent(m_player,PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &PlayerChallenge::Exit);
	}
}

void PlayerChallenge::Release()
{
	if(m_sweepTimer.IsValid())
	{
		m_sweepTimer.SetNull();
	}

	if(m_exitEvent.IsValid())
	{
		m_exitEvent.SetNull();
	}

}

void PlayerChallenge::InitChallengeInfo(const PlayerInfo::ChallengeInstance& info)
{
	eDayResetType tempType = m_player->GetCounterService()->getResetDayType();
	m_layer = info.layer();
	m_maxlayer = info.maxlayer();

	if(m_layer == 0 || tempType != eDayResetToday)
	{
		m_layer = 1;
	}

	if(m_maxlayer <= 0)
	{
		m_maxlayer = 1;
	}

	m_player->SetInitPartDataFlag(eChallengeInfo);
}

void PlayerChallenge::GetChallengeInfo(PlayerInfo::ChallengeInstance* info)
{
	info->set_layer(m_layer);
	info->set_maxlayer(m_maxlayer);
}

void PlayerChallenge::GetData(MapInstanceProto::AckChallengeInfo *data)
{
	data->set_layer(m_layer);
	data->set_maxlayer(m_maxlayer);

	DWORD64 dx = CUtil::GetNowSecond()-m_startTime;
	dx = m_totalTime - dx > 0 ? m_totalTime - dx : 0;
	dx /= 1000;

	data->set_issweep( IsSweep() );
	data->set_sweeptime(dx);
	data->set_totaltime(m_totalTime/1000);

	int resettimes = m_player->GetCounterService()->GetNumById(CHALLEGE_INSTANCE_COUNTER);
	data->set_reset(resettimes);
}

bool PlayerChallenge::IsSweep()
{
	if(m_sweepTimer.IsValid())
	{
		return true;
	}
	return false;
}

int PlayerChallenge::StartSweep(int &totalTime)
{
	//是否正在扫荡
	if(IsSweep())
	{
		return eInInstanceSweep;
	}

	//能否扫荡
	if(m_layer > m_maxlayer)
	{
		return eNotPassInstance;
	}

	//计算剩余时间
	for(int i=m_layer; i<=m_maxlayer; ++i)
	{
		const ChallengeLayerInfo* pInfo = ChallengeMgr::GetInstance()->GetLayerInfo(i);
		if(pInfo == NULL)
			return eInstanceInfoError;

		totalTime += pInfo->sweeptime;
	}

	m_startTime = CUtil::GetNowSecond();
	m_totalTime = totalTime*1000;
	m_sweepTimer = TimerInterface::GetInstance()->RegTimePoint(&PlayerChallenge::OnSweepOver, this, 0, 0, 0, 0, m_totalTime);

	return 0;
}

int PlayerChallenge::EndSweep(MapInstanceProto::ACkEndSweep* data)
{
	//是否合法
	if(!m_sweepTimer.IsValid())
	{
		return eInstanceInfoError;
	}

	//计算扫荡到的层数
	DWORD64 curtime = CUtil::GetNowSecond();
	DWORD64 dx = curtime - m_startTime;

	DWORD64 temptime = 0;
	int tmeplayer = m_layer;
	bool isChange = false;

	for(int i=m_layer; i<=m_maxlayer; ++i)
	{
		const ChallengeLayerInfo * pInfo = ChallengeMgr::GetInstance()->GetLayerInfo(i);
		if(pInfo == NULL)
				continue;

		temptime += pInfo->sweeptime*1000;
		if(temptime > dx)
		{
			if(m_layer != i)
			{
				m_layer = i;
				isChange = true;
			}
			break;
		}
	}

	data->set_startlayer(tmeplayer);
	data->set_endlayer(m_layer);

	if(m_sweepTimer.IsValid())
	{
		m_sweepTimer.SetNull();
	}

	m_startTime = 0;
	m_totalTime = 0;

	//进度没有改变
	if(!isChange)
	{
		return 0;
	}

	//发送奖励
	std::vector<PropItem> items;
	Prize(tmeplayer, m_layer-1, items);

	for(size_t i=0; i<items.size(); ++i)
	{
		::MapInstanceProto::PrizeItem* pItem = data->add_prize();
		if(pItem != NULL)
		{
			pItem->set_id(items[i].id);
			pItem->set_num(items[i].num);
		}
	}

	return 0;
}

int PlayerChallenge::ExChange(int id, int num)
{
	const ChallengeShop* pInfo = ChallengeMgr::GetInstance()->GetShopInfo(id);
	if(pInfo == NULL)
		return eItemError;

	if(pInfo->layerLimit > m_maxlayer)
	{
		return eChallengeExchangeLimit;
	}

	if(num <= 0)
	{
		return eItemCountNull;
	}

	int64 money = m_player->getAttManage()->getValue<int64>(eChallengeMoney);
	if(money < pInfo->price*num)
	{
		return eChallengeNotEnough;
	}

	//判断背包是否已满
	vector<DWORD> itemVec;
	vector<DWORD> numVec;

	itemVec.push_back(id);
	numVec.push_back(num);

	int error = m_player->GetContainer(ePackageType)->IsBagEnought(id, num, pInfo->bindType);
	if(error != 0)
	{
		return ePackageIsFull;
	}

	vector<DWORD> strengthLvVec;
	vector<WORD> bindVec;
	vector<int> vecLock;

	strengthLvVec.resize(itemVec.size(), 0);
	bindVec.resize(itemVec.size(),pInfo->bindType);

	error = m_player->GetContainer(ePackageType)->AddItem(itemVec, numVec, vecLock, bindVec,strengthLvVec,true,shop_buy_type);
	if(error != 0)
	{
		return ePackageIsFull;
	}

	m_player->ChangeChallenge(pInfo->price*num, false);

	return 0;
}

void PlayerChallenge::Reset()
{
	m_layer = 1;
	m_player->SetDataFlag(eChallengeInfo);
}

int PlayerChallenge::CreateChecke(int id, int layer)
{
	if(id != CHALLEGEMAPFIRSTID)
	{
		return eInstanceInfoError;
	}

	if(IsSweep())
	{
		return eInInstanceSweep;
	}

	//层数是否正确
	if(layer != m_layer)
	{
		return eInstanceInfoError;
	}

	if(layer > ChallengeMgr::GetInstance()->GetMaxLayers())
	{
		return eInstanceInfoError;
	}

	return 0;
}

void PlayerChallenge::UpdateLayer(int layer)
{
	if(m_layer <= 0 || m_layer > ChallengeMgr::GetInstance()->GetMaxLayers())
	{
		return;
	}

	m_layer = layer;
	m_player->SetDataFlag(eChallengeInfo);
}

void PlayerChallenge::UpdateRecord(int layer)
{
	if(layer <= 0 || layer > ChallengeMgr::GetInstance()->GetMaxLayers())
	{
		return;
	}

	if(layer <= m_maxlayer)
	{
		return;
	}

	m_maxlayer = layer;
	m_player->SetDataFlag(eChallengeInfo);
}

void PlayerChallenge::OnSweepOver(void* args)
{
	m_startTime = 0;
	m_totalTime = 0;

	int temp = m_layer;
	m_layer = m_maxlayer+1;

	std::vector<PropItem> items;
	Prize(temp, m_maxlayer, items);

	//通知前端
	MapInstanceProto::ACkEndSweep msg;
	msg.set_startlayer(temp);
	msg.set_endlayer(m_layer);
	for(size_t i=0; i<items.size(); ++i)
	{
		::MapInstanceProto::PrizeItem* pItem = msg.add_prize();
		if(pItem != NULL)
		{
			pItem->set_id(items[i].id);
			pItem->set_num(items[i].num);
		}
	}

	m_player->SendSynchMessage(&msg, MSG_GS2C_SWEEP_OVER);

	if(m_sweepTimer.IsValid())
	{
		m_sweepTimer.SetNull();
	}
}

bool PlayerChallenge::Exit(const EventArgs & e)
{
	MapInstanceProto::ACkEndSweep data;
	EndSweep(&data);
	return true;
}



void PlayerChallenge::Prize(int startlayer, int endlayer, std::vector<PropItem>& items)
{
	//发送邮件
	std::map<int,int> prizeMap;

	for(int i=startlayer; i<=endlayer; ++i)
	{
		const ChallengeLayerInfo * pInfo = ChallengeMgr::GetInstance()->GetLayerInfo(i);
		if(pInfo == NULL)
			continue;

		for(size_t j=0; j<pInfo->prizes.size(); ++j)
		{
			std::map<int,int>::iterator it = prizeMap.find(pInfo->prizes[j].first);
			if(it != prizeMap.end())
			{
				it->second += pInfo->prizes[j].second;
			}
			else
			{
				prizeMap[pInfo->prizes[j].first] = pInfo->prizes[j].second;
			}
		}
	}

	std::map<int,int>::iterator itr = prizeMap.begin();
	for(; itr != prizeMap.end(); ++itr)
	{
		items.push_back(PropItem(itr->first, itr->second));
	}

	MailMessageMgr::GetInstance()->SendMail(m_player->GetID(), items);
}

