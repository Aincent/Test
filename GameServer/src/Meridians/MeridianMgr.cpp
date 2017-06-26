/*
 * MeridianMgr.cpp
 *
 *  Created on: 2014年6月5日
 *      Author: root
 */

#include <cstdlib>
#include "MeridianMgr.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "FileLoader/MeridianLoader.h"
#include "../Map/SceneManager.h"
#include "MessageCommonRet.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "Log/Logger.h"
#include "StatisticBase/StatisticDefine.h"
#include "../ServerEventDefine.h"
#include "EventSet/EventSet.h"
#include "EventSet/EventArgs.h"
#include "../LucklyTurntable/LucklyTurntableMgr.h"
#include "../ActiveUIHighLight/CActiveHighLightMgr.h"
#include "../Attribute/AttributeManage.h"
#include "../StatisticMgr/StatisticHandle.h"

MeridianMgr::MeridianMgr()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(MeridianMgr, MSG_REQ_C2GT_OPENMERIDIANS);		//请求打开经脉
}

MeridianMgr::~MeridianMgr()
{

}

int MeridianMgr::OpenMeridians(Smart_Ptr<Player> player, int acupoint, int isGolden, eMeridianType count,  Meridians::RetOpenMeridians& ret)
{
	//判断玩家登记是否满足
	if(player->IsOpenSystem(eStMeridians) == false)
	{
		return eLVNotEnough;
	}

	if(count < eMeridianTypeOne || count > eMeridianTypeGoldenAuto)
		return eClientSendDataError;

	//判断是否经脉已满
	int nowAcupoint = player->GetAcupoint();
	const int maxAcupoint = MeridianLoader::GetInstance()->GetMaxMeridian();
	if(nowAcupoint >= maxAcupoint)
	{
		return eAcupointFull;
	}

	MeridianStruct * meridianStruct = MeridianLoader::GetInstance()->GetMeridianStruct(acupoint);
	if(!meridianStruct)
		return eClientSendDataError;

	//判断是否满足下一个打开的穴位
	if(nowAcupoint + 1 != acupoint)
	{
		return eDontOpenPerAcupoint;
	}

	int needGolden = 0;
	int needMoney = 0;
	int needIng = 0;
	CBitArray bitArr;
	bitArr.SetSize(32);

	eMeridianType tempType = count;
	int tempRealCounts = 1;// 实际次数，冲穴都为1次

	if (maxAcupoint <= nowAcupoint + tempRealCounts)
	{
		tempRealCounts = maxAcupoint - nowAcupoint;
	}

	//等级判断
	int realCount   = 0;
	tempRealCounts += acupoint;
	for(int i = acupoint;i < tempRealCounts;++i)
	{
		MeridianStruct * tempInfo = MeridianLoader::GetInstance()->GetMeridianStruct(i);
		if(tempInfo == NULL)
		{
			return eSystemError;
		}

		if(tempInfo->m_LevelLimit > player->getAttManage()->getValue<int>(eCharLv))
		{
			//第一次等级不足,则不成功
			if(acupoint == i)
			{
				return eLVNotEnough;
			}
			else
			{
				break;
			}
		}

		++realCount;
	}

	// 特殊冲穴 折扣
	double specialDisc = MeridianLoader::GetInstance()->GetThroughAcpDisc();

	// 是否是使用元宝的
	if (isGolden)
	{
		needGolden = meridianStruct->i_upgolden * realCount;
		if (tempType == eMeridianTypeGoldenAuto)
		{
			// 100百分比
			needGolden = (int)(needGolden * specialDisc / 100);
		}

		if (!player->IsEnoughGolden(needGolden))
		{
			// 检查元宝是否满足
			return eNotEnogGolden;
		}
	}
	else
	{
		needMoney = meridianStruct->i_upmoney * realCount;
		needIng = meridianStruct->i_uping* realCount;
		if (tempType == eMeridianTypeNormalAuto)
		{
			needMoney = (int)(needMoney * specialDisc / 100);
			needIng = (int)(needIng * specialDisc / 100);
		}

		//检查玩家铜钱是否满足
		if(player->getAttManage()->getValue<int>(eCharMoney) < needMoney)
		{
			return eNotEnoghMoney;
		}

		//检查玩家真气是否满足
		if(player->getAttManage()->getValue<int>(eCharForce) < needIng)
		{
			return eNotEnoghForce;
		}
	}

//	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//	StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();
//	idAndCostAndGotInfo.set_beforeid(nowAcupoint);

	int failedNum = 0;
	int newAcupointID = nowAcupoint;
	int randVal = 0;
	vector<int> vecType;

	if (isGolden)
	{
//		StatisticInfo::CostItemInfo * costGoldenItem = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costGoldenItem,eCharGolden,player);
//
//		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,player);

		int costGolden = 0;
		int costbGolden = 0;

		player->DecreaseGoldenCost(needGolden, costGolden, costbGolden);
		if(costGolden > 0)
			vecType.push_back(eCharGolden);
		if(costbGolden > 0)
			vecType.push_back(eCharBindGolden);

//		StatisticMgr::SetItemCostAfter(costGoldenItem,eCharGolden,costGolden,player);
//
//		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,costbGolden,player);
	}
	else
	{
//		StatisticInfo::CostItemInfo * costMoneyItem = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costMoneyItem,eCharMoney,needMoney,player);
//
//		StatisticInfo::CostItemInfo * costForceItem = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costForceItem,eCharForce,needIng,player);

		//真气获得的行为日志
		player->ChangeCurForce(needIng, false);

		player->ChangeMoney(needMoney, false);
		vecType.push_back(eCharMoney);
		vecType.push_back(eCharForce);

//		StatisticMgr::SetItemCostAfter(costMoneyItem,eCharMoney,player);
//
//		StatisticMgr::SetItemCostAfter(costForceItem,eCharForce,player);

//		if(eAccountType_User == player->GetAccountType())
//		{
			//统计一天消耗的铜钱数
//			StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_Meridian,eStaMinor_Meridian_CostMoney,needMoney);
		GLOBAL_STATISTIC_GM(player, GlobalType::Main::eMeridian, GlobalType::Minor::eMeridian_UpgradeCostMoney, StatistSaveType::Int64_Data(needMoney));
		//统计一天消耗的总真气数
//			StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_Meridian,eStaMinor_Meridian_CostForce,needIng);
		GLOBAL_STATISTIC_GM(player, GlobalType::Main::eMeridian, GlobalType::Minor::eMeridian_UpgradeCostForce, StatistSaveType::Int64_Data(needIng));

//		}
	}
	//完成活跃次数
	player->GetVitalityMgr()->FinshVitality(eViAcupointUp);

	int statsticExp = 0;

	for (int  i = 1; i <= realCount; ++i)
	{
		//检查概率dalizi
		randVal = rand()%100 +1;
		if (isGolden)
		{
			randVal = 1;
		}

		player->AddExp(meridianStruct->i_exp);

		statsticExp += meridianStruct->i_exp;

		if(meridianStruct->i_pty >= randVal)
		{
			map<ECharProf,map<ECharAttrType,int64> >::iterator tempProfessionIt = meridianStruct->m_AttList.find(
					(ECharProf)player->getAttManage()->getValue<BYTE>(eCharProfession));

			if(tempProfessionIt == meridianStruct->m_AttList.end())
			{
				continue ;
			}
			eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttMeridian);
			map<ECharAttrType,int64>::iterator tempAttIt = tempProfessionIt->second.begin();
			for(;tempAttIt != tempProfessionIt->second.end();++tempAttIt)
			{
				if(tempAttIt->second == 0)
				{
					continue ;
				}
				player->addAtt(tempAttIt->first,tempAttIt->second,true,tempValueType);
				vecType.push_back(tempAttIt->first);
			}

			newAcupointID += 1;

			bitArr.SetBit(i-1, true);

			//通知全屏
			vector<Smart_Ptr<CreatureObj> > obj;
			Smart_Ptr<CreatureObj> owner = player->GetCreatureSelf();
			CSceneMgr::GetInstance()->GetGroupObject(owner, obj);
			for(size_t j = 0; j < obj.size(); j++)
			{
				if(obj[j] && obj[j]->GetType() == ePlayer)
				{
					Smart_Ptr<Player> otherPlayer = obj[j];
					if(otherPlayer->GetID() != player->GetID())
						otherPlayer->AddExp(meridianStruct->i_senexp);
				}
			}

			ServerMessage::ClientSingleMessage ack;
			Meridians::AskOpenMeridians toGate;
			toGate.set_charname(player->GetName());
			toGate.set_acupoint(newAcupointID);
			toGate.set_addexp(meridianStruct->i_senexp);
			string tStr = toGate.SerializePartialAsString();
			ack.set_content(tStr);
			ack.set_id(MSG_REQ_C2GT_BROCASTOPENMERIDIANS);
			CSceneMgr::GetInstance()->SendSynchMessage(&ack,
													MSG_REQ_GT2GM_QUICKMESSAGE,
													player->GetMapID(),
													player->GetKey(),
													player->GetXNewPos(),
													player->GetYNewPos(),
													false);

			//统计 玩家打通经脉成功次数
//			StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(player->GetMyself(),eStatic_Meridian,eStaMinor_Meridian_LevelUp,1);

			player->AddAcupoint(1);

			//判断是否经脉已满
			if(newAcupointID >= MeridianLoader::GetInstance()->GetMaxMeridian())
			{
				break;
			}

			meridianStruct = MeridianLoader::GetInstance()->GetMeridianStruct(acupoint+i);
		}
		else
		{
			++failedNum;
		}
	}

	if(vecType.size() > 0)
	{
		player->SetDataFlag(eBaseInfo);
		player->SetDataFlag(eBattleInfo);

		//从新计算一次经脉模块战斗力
		player->calculateMeridianFight();
		vecType.push_back(eFightPower);

		player->SynCharAttribute(vecType);
	}

	ret.set_newacupoint(newAcupointID);
	ret.set_type(tempType);

//	if(statsticExp > 0)
//	{
//		StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo->add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotItemInfo,eCharExp,statsticExp);
//	}

	if (IsAcupointMaxByLv(player))
	{
		player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Meridian, eUIHLSmall_Meridian_CanAdvance), 1, false);
	}

	//冲特殊次数，无论成功或失败，都不放送失败
	if (failedNum == realCount)
	{
		// 失败次数和冲穴次数一样
		if(tempType == eMeridianTypeOne || tempType == eMeridianTypeGoldOne)
			return eOpenAcupointFail;
	}

//	idAndCostAndGotInfo.set_afterid(player->GetAcupoint());
//	if(eAccountType_User == player->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Meridian,eStaMinor_Meridian_UpgradeEvent,&idAndCostAndGotInfo);

	return 0;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MeridianMgr,MSG_REQ_C2GT_OPENMERIDIANS)
{
	Meridians::OpenMeridians openMeridians;
	Meridians::RetOpenMeridians ret;

	vector<int> vecType;
	int res = 0;

	openMeridians.ParseFromString(content);
	if(!player)
	{
		return;
	}

	BEHAVIOR_MONI(player, BehaviorType::Main::eMeridian, BehaviorType::Minor::eMeridian_Levelup, MONITOR);
	if(player->IsOpenSystem(eStMeridians))
	{

		res = OpenMeridians(player, player->GetAcupoint()+1, openMeridians.isgolden(), (eMeridianType)openMeridians.count(), ret);
		ret.set_errnum(res);

		if(res == 0)
		{
			OneParmaEventArgs e(player);
			player->FireEvent(PLAYER_CHANNEL_UPDATE,e);
		}
	}
	else
	{
		ret.set_errnum(eNotOpen);
	}

	

	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GT_OPENMERIDIANS);
}

void MeridianMgr::getBforeIDAllAtt(map<ECharAttrType,int64>& attList,int id,ECharProf profession)
{
	const map<int, MeridianStruct>& tempData	= MeridianLoader::GetInstance()->getFileData();
	map<int, MeridianStruct>::const_iterator tempIt = tempData.begin();

	for(;tempIt != tempData.end();++tempIt)
	{
		if(tempIt->first > id)
		{
			break;
		}

		map<ECharProf,map<ECharAttrType,int64> >::const_iterator tempProfessionIt = tempIt->second.m_AttList.find(profession);
		if(tempProfessionIt == tempIt->second.m_AttList.end())
		{
			continue ;
		}

		map<ECharAttrType,int64>::const_iterator tempAttIt = tempProfessionIt->second.begin();
		for(;tempAttIt != tempProfessionIt->second.end();++tempAttIt)
		{
			attList[tempAttIt->first] += tempAttIt->second;
		}
	}

	return ;
}

bool MeridianMgr::IsAcupointMaxByLv(Smart_Ptr<Player> player)
{
	if (!player || MeridianLoader::GetInstance()->GetMaxMeridian() <= player->GetAcupoint())
	{
		return true;
	}

	MeridianStruct* meridianInfo = MeridianLoader::GetInstance()->GetMeridianStruct(player->GetAcupoint() + 1);
	if (NULL == meridianInfo)
	{
		return true;
	}

	return meridianInfo->m_LevelLimit > player->getAttManage()->getValue<int>(eCharLv);
}


bool MeridianMgr::IsCanAdvance(Smart_Ptr<Player> player)
{
	return 0 == CheckAdvanceCond(player);
}

int MeridianMgr::CheckAdvanceCond(Smart_Ptr<Player> player)
{
	if (!player->IsOpenSystem(eStMeridians))
	{
		return eLVNotEnough;
	}

	//判断是否经脉已满
	int nowAcupoint = player->GetAcupoint();

	const int maxAcupoint = MeridianLoader::GetInstance()->GetMaxMeridian();
	if(nowAcupoint >= maxAcupoint)
	{
		return eAcupointFull;
	}

	int needMoney = 0;
	int needIng = 0;

	//等级判断
	MeridianStruct * meridianStruct = MeridianLoader::GetInstance()->GetMeridianStruct(nowAcupoint + 1 );
	if (NULL == meridianStruct)
	{
		return eSystemError;
	}

	if(meridianStruct->m_LevelLimit > player->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotEnough;
	}

	needMoney = meridianStruct->i_upmoney;
	needIng = meridianStruct->i_uping;

	//检查玩家铜钱是否满足
	if(player->getAttManage()->getValue<int>(eCharMoney) < needMoney)
	{
		return eNotEnoghMoney;
	}

	//检查玩家真气是否满足
	if(player->getAttManage()->getValue<int>(eCharForce) < needIng)
	{
		return eNotEnoghForce;
	}

	return 0;
}






