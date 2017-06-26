/*
 * ParcelMgr.cpp
 *
 *  Created on: 2014幄1�7�1�7旄1�7
 *      Author: helh
 */

#include "ParcelMgr.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../Container/ContailerLoader/ContailerLoader.h"
#include "../EventSet/EventDefine.h"
#include "ContainerBase.h"
#include "../ServerEventDefine.h"
#include "../QuickMessage.h"
#include "../StatisticMgr/StatisticHandle.h"

ArticleParcelMgr::ArticleParcelMgr(Player* player, size_t MaxContainerSize):ArticleBaseMgr(player, MaxContainerSize)
{
	ReInit();
}

ArticleParcelMgr::~ArticleParcelMgr()
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

void ArticleParcelMgr::ReInit()
{
	ArticleBaseMgr::ReInit();
	m_timer = 0;

	if(!m_vipEvent.IsValid())
		m_vipEvent = RegEvent(m_player, PLAYER_UPGRADEVIPLV_EVENT_TYPE, this, &ArticleParcelMgr::HandleUpdateVipLv);//是否可以 领取 vip奖励
}

void ArticleParcelMgr::Release()
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

void ArticleParcelMgr::OnLockGrid(BYTE pos, bool bLock)
{

}

void ArticleParcelMgr::SetArticleInfo(PlayerInfo::ContainerInfo *content)
{
	const PlayerInfo::PackageInfo& info = content->package();

	m_curContainerSize = info.size();

	for(size_t i = 0 ; i < m_curContainerSize; i++)
	{
		Smart_Ptr<PropBase> newProp = new PropBase(-1);
		m_gridList.push_back(newProp);
	}

	for(int i=0; i<info.package_size(); ++i)
	{
		const PlayerInfo::ItemInfo& iInfo = info.package(i);
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
		OpenCountainer infoOpen;

		ContailerLoader::GetInstance()->GetOpenPackage(GetSize(),infoOpen);

		int64 intervalTime = infoOpen.nTime;
		SetOpenTimeByInter(intervalTime);
	}


	if(m_player->GetMyself())
	{
		OneParmaEventArgs e(m_player->GetMyself());
		m_player->FireEvent(PLAYER_INIT_PACKAGEINFO,e);
	}

	m_player->SetInitPartDataFlag(ePackageInfo);

}

void ArticleParcelMgr::GetArticleInfo(google::protobuf::Message *info)
{
	PlayerInfo::PackageInfo *tInfo = static_cast<PlayerInfo::PackageInfo *>(info);

	for(size_t i = 0 ; i < m_gridList.size(); i++)
	{
		PlayerInfo::ItemInfo *itemInfo = tInfo->add_package();
		m_gridList[i]->SetInfo(itemInfo);
		itemInfo->set_num(i);
	}

	tInfo->set_size(m_curContainerSize);
	tInfo->set_time(GetOpenTimeStamp());
}

void ArticleParcelMgr::SendArticleInfo()
{
	CharLogin::ClientContainerInfo clientInfo;
	CharLogin::PackageInfo *info = clientInfo.mutable_pack();

	clientInfo.set_type(ePackageType);

	for(size_t i = 0 ; i < m_gridList.size(); i++)
	{
		if(m_gridList[i]->GetID() <= 0)
		{
			continue;
		}

		CharLogin::ClientItemInfo *itemInfo = info->add_pack();

		m_gridList[i]->SetClientInfo(itemInfo);
		itemInfo->set_num(i);
	}

	info->set_dtime(m_openTime);
	info->set_size(m_curContainerSize);


	QuickMessage::GetInstance()->AddSendMessage(m_player->GetID(), m_player->GetChannelID(),&clientInfo,MSG_SIM_GT2C_CONTAINERINFO);
}

void ArticleParcelMgr::StartTime()
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
		m_timer = TimerInterface::GetInstance()->RegTimePoint(&ArticleParcelMgr::OpenPpack,this, 0, 0, 0, 0, GetOpenTimeStamp());
	}
}
void ArticleParcelMgr::ReStartTIme()
{
	OpenCountainer info;
	//注册定时器
	if(m_timer.IsValid())
	{
		m_timer.SetNull();
	}

	ContailerLoader::GetInstance()->GetOpenPackage(GetSize(),info);
	int64 intervalTime = info.nTime;
	SetOpenTimeByInter(intervalTime); // 设置开启时间 并改变间隔时间

	m_timer = TimerInterface::GetInstance()->RegTimePoint(&ArticleParcelMgr::OpenPpack,this, 0, 0, 0, 0, intervalTime);
}

//定时开启一个格子
void ArticleParcelMgr::OpenPpack(void* arg)
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

	ReStartTIme();
}

// 针对背包的 设置开启时间 接口 PARAM: IN OUT 间隔时间   OUT间隔时间因VIP而改变
void ArticleParcelMgr::SetOpenTimeByInter(int64& intervalTime)
{

	int64 openTime = 0;

//	// 判断玩家是否VIP VIP开启速度加成
//	double value = m_player->GetVipPart()->GetValueByRight(eVipOpenPackGridSpeedAdd);
//
//	if (0 < value)
//	{
//		intervalTime = (int64)(intervalTime / (1 + value));
//	}

	openTime = intervalTime + CUtil::GetNowSecond();
	SetOpenTime(openTime);
}

void ArticleParcelMgr::AddPackSize(int nTime)
{

}
void ArticleParcelMgr::AddBackPackSize(DWORD addNum)
{
	if(addNum <= 0 )
	{
		return;
	}

	Goods::GTAckToCOpenGoodsRet ret;
	BYTE conNum = GetSize();

	//已经开满格子
	if((int)(GetSize() + 1) > GetMaxContainerSize())
	{
		return;

	}

	if(addNum + conNum > (size_t)GetMaxContainerSize())
	{
		addNum = GetMaxContainerSize() - conNum;
	}

	ret.set_status(0);
	ret.set_container(0);
	for(DWORD i = 0 ; i < addNum ; i++)
	{
		ret.add_pos(GetSize());

		Smart_Ptr<PropBase> newProp = new PropBase(-1);
		m_gridList.push_back(newProp);
	}

	m_curContainerSize = GetSize();
	m_openFlag = false;

	QuickMessage::GetInstance()->AddSendMessage(m_player->GetID(), m_player->GetChannelID(),&ret,MSG_REQ_C2CT_ADDBACKPACKSIZE);

	m_player->SetDataFlag(ePackageInfo);
}

// VIP升级 背包开启时间改变
bool ArticleParcelMgr::HandleUpdateVipLv(const EventArgs& e)
{
	int64 intervalTime = GetOpenTimeStamp();
	SetOpenTimeByInter(intervalTime);

	//注册定时器
	if(m_timer.IsValid())
	{
		m_timer.SetNull();
	}

	m_timer = TimerInterface::GetInstance()->RegTimePoint(&ArticleParcelMgr::OpenPpack,this, 0, 0, 0, 0, intervalTime);

	SendArticleInfo();

	return true;
}

void ArticleParcelMgr::getAllTaskItem(map<int,int>& mapList)
{
	const GoodsInfo* tempInfo = NULL;
	for(size_t i = 0;i < m_gridList.size();++i)
	{
		if(!m_gridList[i])
		{
			continue;
		}

		if(m_gridList[i]->GetID() <= 0)
		{
			continue;
		}

		tempInfo = GoodsLoader::GetInstance()->GetItemDataByID(m_gridList[i]->GetID());
		if(tempInfo == NULL)
		{
			continue;
		}

		if(tempInfo->type != eTask)
		{
			continue;
		}

		int tempCounts = m_gridList[i]->GetAmount();

		mapList[m_gridList[i]->GetID()] += tempCounts;
	}

}

void ArticleParcelMgr::GetAllIem(map<int, int>& itemList)
{
	for(size_t i = 0;i < m_gridList.size();++i)
	{
		const GoodsInfo* tempInfo = NULL;

		if(!m_gridList[i])
		{
			continue;
		}

		if(m_gridList[i]->GetID() <= 0)
		{
			continue;
		}

		tempInfo = GoodsLoader::GetInstance()->GetItemDataByID(m_gridList[i]->GetID());
		if(tempInfo == NULL)
		{
			continue;
		}

		int tempCounts = m_gridList[i]->GetAmount();

		itemList[m_gridList[i]->GetID()] += tempCounts;
	}
}

void ArticleParcelMgr::DiscardGoods(int container,  int gride, int& goodsID, int& num, int& err)
{
	BEHAVIOR_MONI(m_player->GetMyself(), BehaviorType::Main::ePackage, BehaviorType::Minor::ePackage_DropItem, MONITOR);

	err = DiscardGoods(container, gride, goodsID, num);

	
}

bool ArticleParcelMgr::IsGirdFull()
{
	return FindNullPos()==-1 ? true : false;
}

int ArticleParcelMgr::DiscardGoods(int container, int gride, int& goodsID, int& num)
{
	int goodsId = 0;

	if(!IsValidPos(gride))
	{
		return eSystemError;
	}
	else
	{
		if(container != ePackageType)
		{
			return eCantDiscard;
		}
		else
		{
			Smart_Ptr<PropBase> propBase = m_gridList[gride];
			goodsId = propBase->GetID();

			const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(propBase->GetID());
			if(ginfo == NULL)
			{
				return eItemError;
			}

			if(ginfo->type == eTask)
			{
				return eCantDiscardTaskGoods;
			}

			//格子号太大了
			if(!propBase)
			{
				return eItemError;
			}
			if(propBase->IsValid())
			{
				return eItemError;
			}
			else
			{
//				StatisticInfo::CostAndGotInfo costAndGotInfo;
//				StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//				StatisticMgr::SetItemCost(costItemInfo,propBase->GetID(),propBase->GetAmount());

//				if(eAccountType_User == m_player->GetAccountType())
//					StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_player->GetID(),eStatic_Package, eStaMinor_Package_DiscardItemEvent,&costAndGotInfo);

				goodsID = propBase->GetID();
				num = propBase->GetAmount();

				BEHAVIOR_HANDLE_DO(m_player->GetMyself(), BehaviorHandType::DelItem(propBase->GetID(),  propBase->GetAmount()));

				propBase->SetEmpty();

//				Goods::UpdateGoods *updateGoods = Ret.mutable_updategoods();
//				CharLogin::ClientItemInfo *info = updateGoods->mutable_iteminfo();
//				propBase->SetClientInfo(info);
//				updateGoods->set_container(container);
//				updateGoods->set_pos(gride);
				m_player->SetDataFlag(ePackageInfo);

				EventArgs args;
				m_player->FireEvent(PLYAER_PACKAGE_CHANGE, args);

				//统计丢弃 品质高的物品次数
//				if(ginfo->itemQuality > eGoodsQualityGreen)
//					StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_player->GetMyself(), eStatic_Package, eStaMinor_Package_DiscardItem, 1);

//				if (ePackageType == GetType())
//				{
//					Smart_Ptr<ArticleParcelMgr> parcelMgr = m_player->GetContainer(ePackageType);
//					m_player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Package, eUIHLSmall_Package_Full), parcelMgr->IsGirdFull());
//
//				}
			}
		}
	}
	return 0;
}


