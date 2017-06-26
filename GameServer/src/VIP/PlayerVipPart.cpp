/*
 * PlayerVipPart.cpp
 *
 *  Created on: 2014年8月11日
 *      Author: root
 */

#include "VipLoader.h"
#include "PlayerVipPart.h"
#include "../Object/Player/Player.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "CounterDefine.h"
#include <math.h>
#include "MessageCommonRet.h"
#include "../QuickMessage.h"
#include "../Object/Creature.h"
#include <cstdlib>
#include "FightDefined.h"
#include "../Object/FileLoader/GirlWarLoader.h"
#include "../Mail/MailMessage.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "RedEnvelopeDefine.h"
#include "../CountryRedEnvelope/CountryRedEnvelopeLoader.h"
#include "./FileLoader/GameParaLoader.h"
#include <math.h>
#include "../Attribute/AttributeCreature.h"
#include "MessageStruct/StatisticInfo.pb.h"
#include "./FileLoader/InternationalLoader.h"
#include "../StatisticMgr/StatisticHandle.h"

PlayerVipPart::PlayerVipPart(Player* owner):m_nOwner(owner)
{
	ReInit();
}

PlayerVipPart::~PlayerVipPart()
{
	m_nOwner = 0;
	if(m_Timer.IsValid())
	{
		m_Timer.SetNull();
	}
	if(m_vipCardTimer.IsValid())
		m_vipCardTimer.SetNull();
}

void PlayerVipPart::ReInit()
{
	m_nGiftInfo = 0;
	m_nAddUpGold = 0;
	m_num = 0;
	m_maxNum = 0;
	m_lasetTime = 0;
	m_endtime = 0;
	m_vipCardBeginTime = 0;
	m_valideSecs = 0;
	m_RechargeAwardState.clear();
}

void PlayerVipPart::Release()
{
	if(m_Timer.IsValid())
	{
		m_Timer.SetNull();
	}

	if(m_vipCardTimer.IsValid())
	{
		m_vipCardTimer.SetNull();
	}

	m_RechargeAwardState.clear();

}

//存储到数据库
void PlayerVipPart::SaveToDB(PlayerInfo::PlayerVipInfo *info)
{
	info->set_giftinfo(m_nGiftInfo);
	info->set_vipaddupgold(m_nAddUpGold);
	info->set_day(m_num);
	info->set_maxday(m_maxNum);
	info->set_endtime(m_endtime);
	info->set_lasttime(m_lasetTime);
	int tempVipCardLv = m_nOwner->getAttManage()->getValue<int>(eVipCardLv);
	if(tempVipCardLv > 0 && (int64)CUtil::GetNowSecond() - m_vipCardBeginTime < m_valideSecs)
	{
		info->set_vipcardlv(tempVipCardLv);
		info->set_vipcardbegintime(m_vipCardBeginTime);
		info->set_validesecs(m_valideSecs);
	}

	map<int,int64>::iterator it = m_RechargeAwardState.begin();

	for(;it != m_RechargeAwardState.end() ; it++)
	{
		info->add_item(it->second);
	}

}

//VIP数据初始化
void PlayerVipPart::InitInfo(const PlayerInfo::PlayerVipInfo &info)
{
	m_nGiftInfo = info.giftinfo();
	m_nAddUpGold = info.vipaddupgold();
	m_num = info.day();
	m_maxNum = info.maxday();
	m_lasetTime = info.lasttime();

	if(info.endtime() == 0)
	{
		m_endtime = GetTime(CUtil::GetNowSecond(),30,true);
		VipLoader::GetInstance()->InitAwardState(m_RechargeAwardState);
	}
	else
	{
		m_endtime = info.endtime();
		for(int i = 0 ; i < info.item_size() ; i++)
		{
			m_RechargeAwardState[info.item(i)>>32] = info.item(i);
		}

	}
	int day = m_endtime/1000 - CUtil::GetNowSecond()/1000;
	//cout<<endl<<"daytime:"<<day<<endl;
	if(day > 0)
		m_Timer = TimerInterface::GetInstance()->RegTimePoint(&PlayerVipPart::end,this, 0, 0, 0,day,0);

	DWORD currentVipLv = m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv);

	int tempVipCardLv  = info.vipcardlv();
	m_vipCardBeginTime = info.vipcardbegintime();
	m_valideSecs       = info.validesecs();

	if(tempVipCardLv <= 0 || currentVipLv >= (DWORD)tempVipCardLv || (int64)CUtil::GetNowSecond() - m_vipCardBeginTime > m_valideSecs)
		ClearVipCard();
	else
	{
		m_nOwner->getAttManage()->setValue(eVipCardLv,tempVipCardLv);
		//剩下的VIP体验时间
		int64 valideSecs = GetVipCardRestTime();

		m_vipCardTimer = TimerInterface::GetInstance()->RegTimePoint(&PlayerVipPart::RemoveVipCard,this, 0, 0, 0,0,valideSecs);

		//同步玩家的VIP体验卡等级
//		vector<int> vec;
//		vec.push_back(eVipCardLv);
//		vec.push_back(eVipCardTime);
//		m_nOwner->SynCharAttribute(vec);
	}
}
//获得连续充值信息
void PlayerVipPart::GetRecharge(PlayerVipProto::ToClientRechargeInfo &info)
{
	info.set_day(m_num);
	info.set_maxday(m_maxNum);
	info.set_endtime(m_endtime);
	info.set_starttime(GetTime(m_endtime,30,false));

	map<int,int64>::iterator it = m_RechargeAwardState.begin();

	for(;it != m_RechargeAwardState.end() ; it++)
	{
		info.add_item(it->second);
		//cout<<endl<<"item:"<<it->second<<endl;
	}
}

//VIP权限初始化
void	PlayerVipPart::InitPlayerVipRight(DWORD lv)
{
	int tempVipCardLv = m_nOwner->getAttManage()->getValue<int>(eVipCardLv);
	if(tempVipCardLv > 0 && lv >= (DWORD)tempVipCardLv)
		ClearVipCard();
}

//int PlayerVipPart::GetVipTimes(eVipRight right)
//{
//	if(right <= eVipMin || right >= eVipMax)
//	{
//		return false;
//	}
//
//	sVipEntry *entry = VipLoader::GetInstance()->GetVipEntry(m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv));
//	if(!entry)
//	{
//		return false;
//	}
//
//	switch(right)
//	{
//		case eVipDepot:
//		{
//			return entry->m_nDepotOpenAdd;
//			break;
//		}
//		case eVipEscort:
//		{
//			return entry->m_nAddEscortFreshTimes;
//			break;
//		}
//		case eVipAddChalBoss:
//		{
//			return entry->m_nChalInstanceResetTimes;
//			break;
//		}
//		default:
//		{
//			return 0;
//		}
//	}
//}

////是否拥有某种权力
//bool PlayerVipPart::HasRight(eVipRight right)
//{
//	if(right <= eVipMin || right >= eVipMax)
//	{
//		return false;
//	}
//
//	sVipEntry *entry = VipLoader::GetInstance()->GetVipEntry(m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv));
//	if(!entry)
//	{
//		return eNull;
//	}
//
//	for(uint i = 0; i < entry->m_nFunctions.size(); i++)
//	{
//		if(right == (int)(entry->m_nFunctions[i]))
//		{
//			return true;
//		}
//	}
//
//	switch(right)
//	{
//		case eVipFlyShoes:
//		{
//			if(m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv) < 3)
//			{
//				return false;
//			}
//			break;
//		}
//		case eVipPhyDefence:
//		{
//			if(m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv) < 7)
//			{
//				return false;
//			}
//			break;
//		}
//		case eVipMagicDefence:
//		{
//			if(m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv) < 8)
//			{
//				return false;
//			}
//			break;
//		}
//		case eVipPhyAttMagicAtt:
//		{
//			if(m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv) < 9)
//			{
//				return false;
//			}
//			break;
//		}
//		case eVipMoveSpeedUp:
//		{
//			if(m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv) < 10)
//			{
//				return false;
//			}
//			break;
//		}
//		default:
//		{
//			return false;
//			break;
//		}
//	}
//
//	return true;
//}

//领取VIP礼包
int PlayerVipPart::GetVipGift(DWORD index, PlayerVipProto::RetGetVipGift &toClient)
{
	int ret = 0;
	if(index <= 0 || index > 10)
	{
		return eNull;
	}

	if(m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv) < index)
	{
		return eVipLevelNotEnough;
	}

	DWORD tempindex = 1 << index;
	DWORD isGot = tempindex & m_nGiftInfo;
	if(isGot)
	{
		return eVipGiftHadGot;
	}

	sVipEntry *entry = VipLoader::GetInstance()->GetVipEntry(index);
	if(!entry)
	{
		return eNull;
	}

	vector<DWORD> goodsIdList;
	vector<DWORD> numList;
	for(DWORD i = 0; i < entry->m_nAward.size();i++)
	{
		goodsIdList.push_back(entry->m_nAward[i]);
		numList.push_back(entry->m_nAwardNum[i]);
		toClient.add_list(entry->m_nAward[i]);
	}

	Smart_Ptr<ArticleBaseMgr> pPacket = m_nOwner->GetContainer(ePackageType);
	if(!pPacket)
	{
		return eNull;
	}

	vector<int> vecPos;

	//判断背包是否能领取
	vector<WORD> bindVec;
	bindVec.resize(goodsIdList.size(), 1);

	ret = pPacket->IsBagEnought(goodsIdList, numList,bindVec);
	if(ret)
	{
		return ret;
	}

	vector<DWORD> strengthLvVec;

	strengthLvVec.resize(goodsIdList.size(), 0);

	int retcode = pPacket->AddItem(goodsIdList, numList, vecPos, bindVec,strengthLvVec,true,npc_fly_type);

	if(retcode == 1)
	{
		return  ePackageFull;
	}

	m_nGiftInfo = m_nGiftInfo | tempindex;
	m_nOwner->SetDataFlag(eVipInfo);

//	StatisticInfo::CostAndGotInfo costAndGotInfo;
//	for(size_t i = 0;i < goodsIdList.size();++i)
//	{
//		StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotItemInfo,goodsIdList[i],numList[i]);
//	}

//	if(eAccountType_User == m_nOwner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_nOwner->GetID(),eStatic_VIP,eStaMinor_VIP_GetVipPrizeEvent,&costAndGotInfo);

	OneParmaEventArgs e(m_nOwner->GetMyself());
	m_nOwner->FireEvent(PLAYER_GET_VIP_GIFT,e);

	//统计VIP领取奖励次数
//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_nOwner->GetMyself(), eStatic_VIP,	eStaMinor_VIP_GetVipPrize);
	return 0;
}
//领取充值奖励
int PlayerVipPart::GetRechargeAward(int id)
{
	vector<DWORD> goodsIdList;
	vector<DWORD> numList;
	vector<int> vecPos;
	vector<DWORD> strengthLvVec;
	vector<WORD> bindVec;
	int state = 0;

	map<int,int64>::iterator it = m_RechargeAwardState.find(id);
	if(it == m_RechargeAwardState.end())
	{
		return eSystemError;
	}

	state = it->second&0xffffffff;

	if(state != eRechargeCanMayReceive)
	{
		return eSystemError;
	}

	sRecharge *recharge = VipLoader::GetInstance()->GetRecharge(id);
	if(!recharge)
	{
		return eNull;
	}

	for(DWORD i = 0; i < recharge->m_nAward.size();i++)
	{
		goodsIdList.push_back(recharge->m_nAward[i]);
		numList.push_back(recharge->m_nAwardNum[i]);
	}

	Smart_Ptr<ArticleBaseMgr> pPacket = m_nOwner->GetContainer(ePackageType);
	if(!pPacket)
	{
		return eNull;
	}

	//判断背包是否能领取
	bindVec.resize(goodsIdList.size(), 1);
	if(pPacket->IsBagEnought(goodsIdList, numList,bindVec))
	{
		return ePackageFull;
	}

	strengthLvVec.resize(goodsIdList.size(), 0);

	int retcode = pPacket->AddItem(goodsIdList, numList, vecPos, bindVec,strengthLvVec,true,npc_fly_type);

	if(retcode == 1)
	{
		return  ePackageFull;
	}

	int64 temp = id;
	it->second = (temp<<32)|eRechargeDrawed;
	m_nOwner->SetDataFlag(eVipInfo);

	return 0;
}

void PlayerVipPart::AddUpGold(DWORD value)
{
	if(value < 0)
		return;

	if(m_nAddUpGold == 0)
	{
		m_nOwner->SetRechargeTime(CUtil::GetNowSecond());
	}

	m_nAddUpGold += value;

	m_lasetTime = CUtil::GetNowSecond();

	DWORD lv = m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv); // 当前等级
	DWORD newLV = VipLoader::GetInstance()->GetVipLevel(m_nAddUpGold); // 新等级

	//同步到世界服
	if(lv!=newLV)
	{
		m_nOwner->SynchCharAttributeToWS(eVIPLv,newLV);
	}

	m_nOwner->getAttManage()->setValue(eAddUpGold,m_nAddUpGold);
	m_nOwner->getAttManage()->setValue(eVIPLv,newLV);

	m_nOwner->SynchCharAttributeToWS(eAddUpGold,m_nAddUpGold);

	m_nOwner->SetDataFlag(eVipInfo);
	m_nOwner->SetDataFlag(eBaseInfo);

	std::vector<int> vecType;
	vector<int> vecBroad;

	const DWORD Recharge_Add_Envelope= CountryRedEnvelopeLoader::GetInstance()->getRechargeAddEnvelope();
	if(value >= Recharge_Add_Envelope)
	{
		m_nOwner->setEnvelopCount(m_nOwner->getAttManage()->getValue<int>(eEnvelopeCount)+1);
		vecType.push_back(eEnvelopeCount);
	}

	vecBroad.push_back(eVIPLv);
	vecType.push_back(eVIPLv);
	vecType.push_back(eAddUpGold);
	m_nOwner->SynCharAttribute(vecType);
	m_nOwner->BroadcastCharAttribute(vecBroad, false);

	if(newLV > lv)
	{
		InitPlayerVipRight(newLV);

		PlayerAddPackSizeEventArgs tArgs(m_nOwner->GetMyself(),newLV);
		m_nOwner->FireEvent(PLAYER_UPGRADEVIPLV_EVENT_TYPE, tArgs);

		//vip升级公告
		string playerName = m_nOwner->GetName();
		char strLevel[256] = {0};
		sprintf(strLevel, "%d", newLV);
		string strName = strLevel;
		ChatMgr::GetInstance()->Marquee(eMarqueeVipLiveUp, playerName, strName, "");

		BEHAVIOR_HANDLE_DO(m_nOwner->GetMyself(), BehaviorHandType::UpVIP(newLV));
	}
}

//更新连续充值领取状态
void PlayerVipPart::UpDateRecharge()
{
	sRecharge * info =	VipLoader::GetInstance()->GetRechargeByDay(m_num);
	if((bool)info)
	{
		map<int,int64>::iterator it = m_RechargeAwardState.find(info->id);
		if(it != m_RechargeAwardState.end())
		{
			int state = it->second&0xffffffff;
			if(state == eRechargeCantMayReceive)
			{
				int64 temp = it->first;
				it->second = (temp<<32)|eRechargeCanMayReceive;
				m_nOwner->SetDataFlag(eVipInfo);
			}
		}
	}
}
//获得几天前后的时间点
int64 PlayerVipPart::GetTime(int64 start,BYTE num , bool bIs)
{
	int64 time = start;
	for(BYTE i = 0 ; i < num ; i++)
	{
		if(bIs)
		{
			time += 24*60*60*1000;
		}
		else
		{
			time -= 24*60*60*1000;
		}
	}
	return time;
}
//连续充值活动结束
void PlayerVipPart::end(void * arg)
{
	map<int,int64>::iterator it = m_RechargeAwardState.begin();
	//cout<<endl<<"时间到了！"<<endl;
	for(;it != m_RechargeAwardState.end() ; it++)
	{
		int state = it->second&0xffffffff;

		if(state == eRechargeCanMayReceive)
		{
			vector<int64> goodsF;
			vector<int64> goodsS;
			sRecharge *recharge = VipLoader::GetInstance()->GetRecharge(it->first);

			if(recharge)
			{
				for(DWORD i = 0; i < recharge->m_nAward.size();i++)
				{
					int64 num = recharge->m_nAwardNum[i];
					int64 item = recharge->m_nAward[i] | (num<<32);

					if(goodsF.size() < 5)
					{
						goodsF.push_back(item);
					}
					else
					{
						goodsS.push_back(item);
					}
				}
			}
			std::string strReward = InternationalLoader::GetInstance()->GetInterStr(20);
			MailMessageMgr::GetInstance()->SendSysMail(goodsF,m_nOwner->GetName(),m_nOwner->GetID(),strReward);
			MailMessageMgr::GetInstance()->SendSysMail(goodsS,m_nOwner->GetName(),m_nOwner->GetID(),strReward);
		}
	}
}
//过期
bool PlayerVipPart::IsOver()
{
	return (int64)CUtil::GetNowSecond() < m_endtime;
}

// 获取对应权限的特权的值
double PlayerVipPart::GetValueByRight(eVipRight right)
{
	//-2表示没有该功能
	if(right <= eVipMin || right >= eVipMax)
	{
		return VIP_NOT_HAVE_THIS_RIGHT;
	}

	DWORD tempVipCardLv = (DWORD)m_nOwner->getAttManage()->getValue<int>(eVipCardLv);
	DWORD vipLv = m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv);
	DWORD finalLv = vipLv > tempVipCardLv ? vipLv : tempVipCardLv;

	sVipEntry *entry = VipLoader::GetInstance()->GetVipEntry(finalLv);
	if(NULL == entry)
	{
		return VIP_NOT_HAVE_THIS_RIGHT;
	}

	std::map<int, double>::iterator it = entry->m_nFunctions.find(right);
	if (it != entry->m_nFunctions.end())
	{
		return it->second;
	}

	return VIP_NOT_HAVE_THIS_RIGHT;
}


void PlayerVipPart::SetVipCard(int64 valideSecs)
{
	int tempVipCardLv = GameParaLoader::GetInstance()->getVipCardLv();

	m_nOwner->getAttManage()->setValue(eVipCardLv,tempVipCardLv);

	if(0 == m_vipCardBeginTime)
	{
		m_vipCardBeginTime = (int64)CUtil::GetNowSecond();
	}

	m_valideSecs += valideSecs;

	if(m_vipCardTimer.IsValid())
	{
		m_vipCardTimer.SetNull();
	}

	//剩余VIP体验时间
	int restTime = this->GetVipCardRestTime();

	m_vipCardTimer = TimerInterface::GetInstance()->RegTimePoint(&PlayerVipPart::RemoveVipCard,this, 0, 0, 0,0,restTime);

	vector<int> vec;
	vec.push_back(eVipCardLv);
	vec.push_back(eVipCardTime);

	m_nOwner->SynCharAttribute(vec);
	m_nOwner->SetDataFlag(eVipInfo);

	if (m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv) < (DWORD)tempVipCardLv)
	{
		PlayerAddPackSizeEventArgs tArgs(m_nOwner->GetMyself(),tempVipCardLv);
		m_nOwner->FireEvent(PLAYER_UPGRADEVIPLV_EVENT_TYPE, tArgs);
	}

//	m_nOwner->UpdateUIHighLight(eUIHLHandle_ResetAndSyn, UIHLType(eUIHLBig_Null, eUIHLSmall_Null));

}

void PlayerVipPart::ClearVipCard(void)
{
	m_nOwner->getAttManage()->setValue(eVipCardLv,0);
	m_vipCardBeginTime = 0;
	m_valideSecs = 0;
	if(m_vipCardTimer.IsValid())
		m_vipCardTimer.SetNull();
	m_nOwner->SetDataFlag(eVipInfo);
}

void PlayerVipPart::RemoveVipCard(void * arg)
{
	DWORD beforeTempVip = (DWORD)m_nOwner->getAttManage()->getValue<int>(eVipCardLv);
	ClearVipCard();

	vector<int> vec;
	vec.push_back(eVipCardLv);
	m_nOwner->SynCharAttribute(vec);

	if (m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv) < beforeTempVip)
	{
		if (m_nOwner->GetMyself())
		{
			PlayerAddPackSizeEventArgs tArgs(m_nOwner->GetMyself(), m_nOwner->getAttManage()->getValue<DWORD>(eVIPLv));
			m_nOwner->FireEvent(PLAYER_UPGRADEVIPLV_EVENT_TYPE, tArgs);
		}

	}

//	m_nOwner->UpdateUIHighLight(eUIHLHandle_ResetAndSyn, UIHLType(eUIHLBig_Null, eUIHLSmall_Null));
}

int PlayerVipPart::CalBuyCostGolden(const Smart_Ptr<PlayerVipPart>& vip,  int cost)
{
	int ret = cost;

//	if ((bool)vip)
//	{
//		double discount = vip->GetValueByRight(eVipMallDiscount);
//		if (VIP_NOT_HAVE_THIS_RIGHT != discount)
//		{
//			ret = (int)(ceil(discount * cost));
//		}
//	}

	return ret;
}

int64 PlayerVipPart::GetVipCardRestTime(void)
{
	int64 valideSecs = m_valideSecs - ((int64)CUtil::GetNowSecond() - m_vipCardBeginTime);

	return valideSecs;
}

void PlayerVipPart::AddUpGoldAndSyn(DWORD value)
{
	m_nOwner->ChangeGolden(value);

	m_nOwner->SynchRechargeToWS(value);
	AddUpGold(value);
	//暂时把加元宝 设置充值次数
	m_nOwner->GetCounterService()->IncreasePlayerCounter(CHARGE_RMB_COUNT, 1);

	// 是否是首充
	if (!m_nOwner->GetSignIn()->IsGetFirstAward() &&
			1 == m_nOwner->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
	{
		m_nOwner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_FirstAward, eUIHLSmall_FirstAward_FirstRecharge), 1, true);
	}

	vector<int> vecType;
	vecType.push_back(eCharGolden);
	m_nOwner->SynCharAttribute(vecType);
}
