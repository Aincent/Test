/*
 * ArticleStoreHouseMgr.cpp
 *
 *  Created on: 2014幄1�7�1�7旄1�7
 *      Author: helh
 */

#include "ArticleStoreHouseMgr.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "../QuickMessage.h"
#include "../VIP/VipLoader.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../Container/ContailerLoader/ContailerLoader.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "SvrConfig.h"

ArticleStorehouseMgr::ArticleStorehouseMgr(Player* player, size_t MaxContainerSize):ArticleBaseMgr(player, MaxContainerSize)
{
	ReInit();
}

ArticleStorehouseMgr::~ArticleStorehouseMgr()
{
	if(m_vipEvent.IsValid())
	{
		m_vipEvent.SetNull();
	}

	if(m_timer.IsValid())
	{
		m_timer.SetNull();
	}
}

void ArticleStorehouseMgr::ArticleStorehouseMgr::ReInit()
{
	ArticleBaseMgr::ReInit();
	m_timer = 0;

	if(!m_vipEvent.IsValid())
		m_vipEvent = RegEvent(m_player, PLAYER_UPGRADEVIPLV_EVENT_TYPE, this, &ArticleStorehouseMgr::HandleUpdateVipLv);
}

void ArticleStorehouseMgr::ArticleStorehouseMgr::Release()
{
	ArticleBaseMgr::Release();

	if(m_vipEvent.IsValid())
	{
		m_vipEvent.SetNull();
	}

	if(m_timer.IsValid())
	{
		m_timer.SetNull();
	}

}

void ArticleStorehouseMgr::SetArticleInfo(PlayerInfo::ContainerInfo *content)
{
	const PlayerInfo::StoreHouseInfo& info = content->storehouse();

	m_curContainerSize = info.size();
	for(size_t i = 0 ; i < m_curContainerSize; i++)
	{
		Smart_Ptr<PropBase> newProp = new PropBase(-1);
		m_gridList.push_back(newProp);
	}

	for(int i=0; i<info.storehouse_size(); ++i)
	{
		const PlayerInfo::ItemInfo& iInfo = info.storehouse(i);
		if(iInfo.id() > 0)
		{
			InitItem(&iInfo);
		}
	}

	if(GetSize()== (size_t)GetMaxContainerSize())
	{
		m_openTime = 0;
	}
	else if(info.time() > 0)
	{
		m_openTime = CUtil::GetNowSecond() + info.time();
	}
	else
	{
		OpenCountainer info;

		ContailerLoader::GetInstance()->GetOpenHouse(GetSize(),info);

		int64 intervalTime = info.nTime;

		SetOpenTimeByInter(intervalTime);
	}

	m_player->SetInitPartDataFlag(eStoreHouseInfo);

}

void ArticleStorehouseMgr::GetArticleInfo(google::protobuf::Message *info)
{
	PlayerInfo::StoreHouseInfo *tInfo = static_cast<PlayerInfo::StoreHouseInfo *>(info);

	for(size_t i = 0 ; i < m_gridList.size(); i++)
	{
		PlayerInfo::ItemInfo *itemInfo = tInfo->add_storehouse();
		m_gridList[i]->SetInfo(itemInfo);
		itemInfo->set_num(i);
	}

	tInfo->set_time(GetOpenTimeStamp());
	tInfo->set_size(m_curContainerSize);
}

void ArticleStorehouseMgr::AddBackPackSize(DWORD addNum)
{
	DWORD nowNum = GetSize()+addNum;
	if((int)nowNum> GetMaxContainerSize())
		nowNum = GetMaxContainerSize();

	m_curContainerSize = nowNum;
	m_openFlag  = false;

	for(DWORD i=GetSize(); i< nowNum; i++)
	{
		Smart_Ptr<PropBase> newProp = new PropBase(-1);
		m_gridList.push_back(newProp);
	}
	//推送  通知客户端 有增加的格子数量
	SynCharStorehouseAddPack(addNum);

	m_player->SetDataFlag(eStoreHouseInfo);
}

void ArticleStorehouseMgr::SynCharStorehouseAddPack(int addNum)
{
	ServerReturn::ServerRetInt toClient;
	toClient.set_ret(addNum);

	QuickMessage::GetInstance()->AddSendMessage(m_player->GetID(), m_player->GetChannelID(),&toClient,MSG_SIM_ADDBACKPACKSIZEBYUPVIP);
}

void ArticleStorehouseMgr::SendArticleInfo()
{
	CharLogin::ClientContainerInfo clientInfo;
	CharLogin::StoreHouseInfo *info = clientInfo.mutable_storehouse();

	clientInfo.set_type(eStoreHouseType);

	for(size_t i = 0 ; i < m_gridList.size(); i++)
	{
		if(m_gridList[i]->GetID() <= 0)
		{
			continue;
		}

		CharLogin::ClientItemInfo *itemInfo = info->add_storehouse();

		m_gridList[i]->SetClientInfo(itemInfo);
		itemInfo->set_num(i);

	}

	info->set_size(m_curContainerSize);
	info->set_dtime(m_openTime);

	QuickMessage::GetInstance()->AddSendMessage(m_player->GetID(), m_player->GetChannelID(),&clientInfo,MSG_SIM_GT2C_CONTAINERINFO);
}


void ArticleStorehouseMgr::StartTime()
{
	if(GetSize()== (size_t)GetMaxContainerSize())
		return;
	//注册定时器
	if(m_timer.IsValid())
	{
		m_timer.SetNull();
	}
	if(GetOpenTimeStamp()<=0)
	{
		ReStartTIme();
	}
	else
	{
		m_timer = TimerInterface::GetInstance()->RegTimePoint(&ArticleStorehouseMgr::OpenPpack,this, 0, 0, 0, 0, GetOpenTimeStamp());
	}

}
void ArticleStorehouseMgr::ReStartTIme()
{
	OpenCountainer info;
	//注册定时器
	if(m_timer.IsValid())
	{
		m_timer.SetNull();
	}

	ContailerLoader::GetInstance()->GetOpenHouse(GetSize(),info);

	int64 intervalTime = info.nTime;

	SetOpenTimeByInter(intervalTime); // 设置开启格子时间 intervalTime因VIP改变

	m_timer = TimerInterface::GetInstance()->RegTimePoint(&ArticleStorehouseMgr::OpenPpack,this, 0, 0, 0, 0, intervalTime);
}

// 针对仓库的 设置开启时间 接口 PARAM: IN OUT 间隔时间   OUT间隔时间因VIP而改变
void ArticleStorehouseMgr::SetOpenTimeByInter(int64& intervalTime)
{

	int64 openTime = 0;

	// 判断玩家是否VIP VIP开启速度加成
//	double value = m_player->GetVipPart()->GetValueByRight(eVipOpenDepotGridSpeedAdd);
//
//	if (0 < value)
//	{
//		intervalTime = (int64)(intervalTime / (1 + value));
//
//		intervalTime /= 60000;
//
//		intervalTime *= 60000;
//	}

	openTime = intervalTime + CUtil::GetNowSecond();
	SetOpenTime(openTime);
}

//定时开启一个格子
void ArticleStorehouseMgr::OpenPpack(void* arg)
{
	AddBackPackSize(1);

	if(GetSize()== (size_t)GetMaxContainerSize())
		return;

	OpenCountainer info;
	//注册定时器
	if(m_timer.IsValid())
	{
		m_timer.SetNull();
	}

	ContailerLoader::GetInstance()->GetOpenHouse(GetSize(),info);

	int64 intervalTime = info.nTime;

	SetOpenTimeByInter(intervalTime); //

	m_timer = TimerInterface::GetInstance()->RegTimePoint(&ArticleStorehouseMgr::OpenPpack,this, 0, 0, 0, 0, intervalTime);
}

// 操作VIP升级事件
bool ArticleStorehouseMgr::HandleUpdateVipLv(const EventArgs& e)
{
	int64 intervalTime = GetOpenTimeStamp();
	SetOpenTimeByInter(intervalTime);

	//注册定时器
	if(m_timer.IsValid())
	{
		m_timer.SetNull();
	}

	m_timer = TimerInterface::GetInstance()->RegTimePoint(&ArticleStorehouseMgr::OpenPpack,this, 0, 0, 0, 0, intervalTime);

	SendArticleInfo();

	return true;

}
