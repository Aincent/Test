
/*
 * LucklyTurntableMgr.cpp
 *
 *  Created on: 2014年8月20日
 *      Author: root
 */

#include "LucklyTurntableMgr.h"
#include "CounterDefine.h"
#include "MessageCommonRet.h"
#include "../Container/ContainerBase.h"
#include "../Chat/ChatMgr.h"
#include "../EventSet/EventDefine.h"
#include "../ServerEventDefine.h"
#include "Probability/Probability.h"
#include "../ActiveUIHighLight/CActiveHighLightMgr.h"

/*************转盘**************/
LucklyTurntableMgr::LucklyTurntableMgr(Player* player):m_owner(player),enterTm(0)
{

}

LucklyTurntableMgr::~LucklyTurntableMgr()
{
	Release();
}

void LucklyTurntableMgr::ReInit()
{
	enterTm = 0;
	m_tmpAward.clear();
	m_RewardList.clear();
}

void LucklyTurntableMgr::Release()
{
	m_tmpAward.clear();
	m_RewardList.clear();
}

void LucklyTurntableMgr::InitFromDB(const PlayerInfo::LucklyTurntable& info)
{
	enterTm = info.tm();

	for(int i = 0;i < info.reawidlist_size();++i)
	{
		m_RewardList.push_back(info.reawidlist(i));
	}

	if(info.itemid_size() > 0)
	{
		//第一版老数据库
		for(int i = 0; i < info.itemid_size(); ++i)
		{
			int tempID = LucklyTurntableLoader::GetInstance()->itemIDToFileDataID(info.itemid(i),info.num(i));
			if(tempID == 0)
				continue;
			TurntableRward reaw;

			reaw.m_ID = tempID;
			m_tmpAward.push_back(reaw);
		}
	}
	//第一版新数据库
	else
	{
		//第二版老数据库
		if(info.bindtype_size() != info.id_size())
		{
			for(int i = 0;i < info.id_size();++i)
			{
				TurntableRward reaw;

				reaw.m_ID 		= info.id(i);
				m_tmpAward.push_back(reaw);
			}
		}
		//第二版新数据库
		else
		{
			for(int i = 0;i < info.id_size();++i)
			{
				TurntableRward reaw;

				reaw.m_ID 		= info.id(i);
				reaw.m_BindType = info.bindtype(i);
				m_tmpAward.push_back(reaw);
			}
		}
	}

	m_owner->SetInitPartDataFlag(eLucklyTurntable);
}

void LucklyTurntableMgr::SetLucklyTurntableInfo(PlayerInfo::LucklyTurntable* info)
{
	info->set_tm(enterTm);
	for(size_t i = 0;i < m_RewardList.size();++i)
	{
		info->add_reawidlist(m_RewardList[i]);
	}

	list<TurntableRward>::iterator it = m_tmpAward.begin();
	for(; it != m_tmpAward.end(); ++it)
	{
		info->add_id(it->m_ID);
		info->add_bindtype(it->m_BindType);
	}
}

bool LucklyTurntableMgr::IsCanEnter()
{
		return m_owner->IsOpenSystem(eStLucklyTurntable);
}

void LucklyTurntableMgr::EnterLucklyTurntable(LucklyTurntable::AckLucklyTurntable  &info)
{
	const ProLucklyTurntable * proPtr = LucklyTurntableLoader::GetInstance()->GetProLuckly();
	if(!proPtr)
		return ;

	if(m_owner->IsOpenSystem(eStLucklyTurntable)==false)
	{
		info.set_ret(eLVNotEnough);
		return ;
	}

	if(IsOverInterTm(proPtr->interTm))
	{
		RandReaw();
		m_owner->SetDataFlag(eLucklyTurntable);
	}

	int freeNum = m_owner->GetCounterService()->GetNumById(FREEROLL_LUCKLYTURNTABLE);
	info.set_entertm(CalNeedTm(proPtr->interTm));  //..........还剩多久时间
	info.set_freenum(freeNum);

	for(size_t i = 0;i < m_RewardList.size();++i)
	{
		info.add_reawidlist(m_RewardList[i]);
	}

	list<TurntableRward>::iterator it = m_tmpAward.begin();
	for(; it != m_tmpAward.end(); ++it)
	{
		const LucklyTurntableInfo* tempFileData = LucklyTurntableLoader::GetInstance()->getInfoByID(it->m_ID);
		if(tempFileData == NULL)
			continue;

		info.add_itemid(it->m_ID);
//		info.add_num(tempFileData->m_Counts);
	}

	return ;
}

bool LucklyTurntableMgr::IsOverInterTm(int intertm)
{
	if(enterTm >0 && (int)(CUtil::GetNowSecond() - enterTm) < intertm)
		return false;
	return true;
}

void LucklyTurntableMgr::RandReaw()
{
	const map<short,vector<LucklyTurntableInfo> >& tempFileDataList = LucklyTurntableLoader::GetInstance()->GetFileDataList();
	map<short,vector<LucklyTurntableInfo> >::const_iterator tempIt  = tempFileDataList.begin();

	vector<short> tempTypeList;
	short tempProbabilityType = 0;

	for(;tempIt != tempFileDataList.end();++tempIt)
	{
		tempTypeList.push_back(tempIt->first);
	}

	if(tempTypeList.size() == 0)
		return ;

	tempProbabilityType = tempTypeList[rand() % tempTypeList.size()];
	tempIt 				= tempFileDataList.find(tempProbabilityType);

	if(tempIt == tempFileDataList.end())
		return ;

	m_RewardList.clear();
	enterTm = CUtil::GetNowSecond();

	for(size_t i = 0;i < tempIt->second.size();++i)
	{
		m_RewardList.push_back(tempIt->second[i].m_ID);
	}

	randOneList(m_RewardList);

	return ;
}

void LucklyTurntableMgr::RollLucklyTurntable(int num,RewardHail::ToClientGetReward& info)
{
	const ProLucklyTurntable * proPtr = LucklyTurntableLoader::GetInstance()->GetProLuckly();
	if(!proPtr)
		return ;

	if(num  != 1 && num != 10)
		return ;

	if(m_owner->IsOpenSystem(eStLucklyTurntable)==false)
	{
		info.set_res(eLVNotEnough);
		return ;
	}

	if(m_RewardList.size() != LUCKLY_REWARD_MAX_COUNTS && m_RewardList.size() != 0)
	{
		info.set_res(eSystemError);
		return;
	}

	if((int)(m_tmpAward.size()) >=proPtr->countNum)
	{
		info.set_res(eContainerFull);
		return ;
	}

	bool oneRollFlag = true;  //转一次  表示为
	if(num == 10)
	{
		num = proPtr->countNum - (int)(m_tmpAward.size());  //计算真实能转的次数
		oneRollFlag = false;
	}

	int ret = CalRollCost(num, proPtr,oneRollFlag);
	if(ret !=0)
	{
		info.set_res(ret);
		return ;
	}

//	StatisticInfo::CostAndGotInfo costAndGotInfo;
//	StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//	StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo.add_costlist();

	bool flag = ChangeRollCost(num, proPtr,oneRollFlag);
	if(flag)
	{
		vector<int> goldenVec;

		goldenVec.push_back(eCharGolden);
		m_owner->SynCharAttribute(goldenVec);
	}

	list<int> vec;

	GetReawInfo(num,vec,flag);

	SetReawInfo(info, vec);

//	list<int>::iterator it = vec.begin();
//	for(; it != vec.end(); ++it)
//	{
//		StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotItemInfo,*it,1);
//	}

//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_TurnTable,eStaMinor_TurnTable_RollEvent,&costAndGotInfo);

	m_owner->SetDataFlag(eLucklyTurntable);

	if (m_owner->GetMyself() && flag)
	{
		TurnLuckyTableArgs e(m_owner->GetMyself(),num);
		m_owner->FireEvent(PLAYER_TURN_LUCKY_TABLE,e);
	}
//	m_owner->GetVitalityMgr()->FinshVitality(eViLucklyTurntable);

}

int LucklyTurntableMgr::CalRollCost(int num, const ProLucklyTurntable* proPtr, bool oneRollFlag)
{
	if(proPtr == NULL)
		return eNotEnoughGold;

	int ret = 0;
	if(oneRollFlag)
	{
		int freeNum = m_owner->GetCounterService()->GetNumById(FREEROLL_LUCKLYTURNTABLE);
		if(freeNum >= proPtr->freeRoll)
		{
			if(m_owner->getAttManage()->getValue<int>(eCharGolden) < proPtr->oneCost)
				ret = eNotEnoughGold;
		}
	}
	else
	{
		if (num < 0)
		{
			ret = eSystemError;
		}
		else
		{
			if(m_owner->getAttManage()->getValue<int>(eCharGolden) < proPtr->tenCost * num)
				ret = eNotEnoughGold;
		}
	}
	return ret;
}

bool LucklyTurntableMgr::ChangeRollCost(int num,const ProLucklyTurntable * proPtr, bool oneRollFlag)
{
	if(proPtr == NULL)
		return false;

	bool flag = false;

	int freeNum = m_owner->GetCounterService()->GetNumById(FREEROLL_LUCKLYTURNTABLE);
	if(oneRollFlag)
	{
		if(freeNum <proPtr->freeRoll)
		{
			m_owner->GetCounterService()->IncreasePlayerCounter(FREEROLL_LUCKLYTURNTABLE, 1);
			freeNum = m_owner->GetCounterService()->GetNumById(FREEROLL_LUCKLYTURNTABLE);

//			m_owner->UpdateAtiveHighLight(eHL_LucklyTable, 1, false, false);
			m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Turntable, eUIHLSmall_Turntable_FreeNumUnUse), 1, false);
//			m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EveryDay, eUIHLSmall_EveryDay_HaveUnComplete), 1, false);
			//统计免费转盘次数
//			StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_TurnTable,	eStaMinor_TurnTable_FreeLotterry);
//			if(freeNum >= proPtr->freeRoll)
//			{
//				m_owner->SynchActiveHighLight();//********************************
//			}
		}
		else
		{
//			StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,m_owner->GetMyself());

			m_owner->ChangeGolden(proPtr->oneCost,false);

//			StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,proPtr->oneCost,m_owner->GetMyself());

			// 统计元宝花费
//			StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(), eStatic_AboutGolden, eStaMinor_AboutGolden_RollTurnTable,1, proPtr->oneCost);
			//统计转盘花费元宝数量
//			StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_TurnTable,	eStaMinor_TurnTable_CostGold,proPtr->oneCost);
			flag = true;
		}
	}
	else
	{
//		StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,m_owner->GetMyself());

		m_owner->ChangeGolden(proPtr->tenCost * num,false);

//		StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,proPtr->tenCost * num,m_owner->GetMyself());

		// 统计元宝花费
//		StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(), eStatic_AboutGolden, eStaMinor_AboutGolden_RollTurnTable,num, proPtr->tenCost * num);
		//统计转盘花费元宝数量
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_TurnTable,	eStaMinor_TurnTable_CostGold, proPtr->tenCost * num);
		flag = true;
	}
	return flag;
}

void LucklyTurntableMgr::GetReawInfo(int num,list<int>& idList,bool isGolden)
{
	vector<LucklyTurntableInfo> tempProbabilityFileList;

	//取出之前算出的ID列表
	for(size_t i = 0;i < m_RewardList.size();++i)
	{
		const LucklyTurntableInfo* tempFileData = LucklyTurntableLoader::GetInstance()->getInfoByID(m_RewardList[i]);
		if(tempFileData == NULL)
			continue;

		tempProbabilityFileList.push_back(*tempFileData);
	}

	ProbabilityVector<LucklyTurntableInfo> tempTurnProbabilityHandle(tempProbabilityFileList);
	for(short i = 0;i < num;++i)
	{
		short tempTurnIndex = tempTurnProbabilityHandle.calculation();
		if(tempTurnIndex == INVALID_INDEX)
			continue;

		TurntableRward tempTurnData;
		//绑定方式扣元宝则根据配表，免费则为梆定
		tempTurnData.m_BindType = isGolden ? tempProbabilityFileList[tempTurnIndex].m_BindType : 1;
		tempTurnData.m_ID 		= tempProbabilityFileList[tempTurnIndex].m_ID;

		idList.push_back(tempProbabilityFileList[tempTurnIndex].m_ID);
		m_tmpAward.push_back(tempTurnData);

	}

	return ;
}

void LucklyTurntableMgr::RefulshLucklyTurntable(LucklyTurntable::AckRefulshOrGetLucklyTurntable& info)
{
	const ProLucklyTurntable * proPtr = LucklyTurntableLoader::GetInstance()->GetProLuckly();

	if((bool)proPtr)
	{
		if(m_owner->IsOpenSystem(eStLucklyTurntable)==false)
		{
			info.set_ret(eLVNotEnough);
			return ;
		}
		if(m_owner->getAttManage()->getValue<int>(eCharGolden) <proPtr->refuCost)
		{
			info.set_ret(eNotEnoughGold);
			return ;
		}

		bool flag  = IsOverInterTm(proPtr->interTm);  //正常到了  刷新时间

		RandReaw();
		DWORD64 needTm = CalNeedTm(proPtr->interTm);

		info.set_ret(0);
		info.set_entertm(needTm);

		for(size_t i = 0;i < m_RewardList.size();++i)
		{
			info.add_reawidlist(m_RewardList[i]);
		}

		if(!flag)
		{
			m_owner->ChangeGolden(proPtr->refuCost, false);

			ChangGoldenAttr(eCharGolden);

			// 统计元宝花费
//			StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(), eStatic_AboutGolden, eStaMinor_AboutGolden_TurnTableReflushReward,1, proPtr->refuCost);
		}

		m_owner->SetDataFlag(eLucklyTurntable);
	}
}

void LucklyTurntableMgr::GetLucklyTurntable(LucklyTurntable::AckGetLucklyTurntable& info)
{
	const ProLucklyTurntable * proPtr = LucklyTurntableLoader::GetInstance()->GetProLuckly();

	if((bool)proPtr)
	{
		if(m_owner->IsOpenSystem(eStLucklyTurntable)==false)
		{
			info.set_ret(eLVNotEnough);
			return ;
		}
	}

	const GoodsInfo* ginfo = NULL;
	vector<int> vecPos;
	vector<DWORD> vec;
	list<TurntableRward> brocastVec;
	int num =0;
	int ret = 0;
	map<int,int> tempAttList;

	list<TurntableRward>::iterator it = m_tmpAward.begin();
	for(; it != m_tmpAward.end();)
	{
		const LucklyTurntableInfo* tempFileData = LucklyTurntableLoader::GetInstance()->getInfoByID(it->m_ID);
		if(tempFileData == NULL)
		{
			++it;
			continue;
		}
		ginfo = GoodsLoader::GetInstance()->GetItemDataByID(tempFileData->m_ItemID);
		if(ginfo == NULL)
		{
			++it;
			continue;
		}

		int funType = GoodsLoader::GetInstance()->GetSpecliItemFunPlayerAttr(ginfo->itemID);

		if(funType == 0)
		{
			int retcode = m_owner->GetContainer(ePackageType)->AddItem(tempFileData->m_ItemID, tempFileData->m_Counts, vecPos,it->m_BindType,0,false);
			if(retcode != 0)
			{
				ret = retcode;
				break;
			}
			else
			{
				if((bool)ginfo->needToBroadCast)
					brocastVec.push_back(*it);
			}
		}
		else
		{
			//加属性
			switch(funType)
			{
			case eCharMoney:
				{
					m_owner->ChangeMoney(tempFileData->m_Counts, true);
					tempAttList[eCharMoney] = 0;
					break;
				}
			case eCharExp:
				{
					m_owner->AddExp(tempFileData->m_Counts);

					break;
				}
			case eCharForce:
				{
					m_owner->ChangeCurForce(tempFileData->m_Counts, true);
					tempAttList[eCharForce] = 0;

					break;
				}
			case eCharGolden:
				{
					m_owner->ChangeGolden(tempFileData->m_Counts, true);
					tempAttList[eCharGolden] = 0;

					break;
				}
			case eCharBindGolden:
				{
					m_owner->ChangeBindGolden(tempFileData->m_Counts, true);
					tempAttList[eCharBindGolden] = 0;

					break;
				}
			default:
				{
					break;
				}
			}
		}

		num++;
		m_tmpAward.erase(it++);
	}

	info.set_ret(ret);
	info.set_notadd(num);

	if(vecPos.size() > 0)
	{
		//背包 数据同步
		vector<int>::iterator itVec = vecPos.begin();
		for(; itVec !=vecPos.end(); ++itVec)
		{
			vec.push_back((ePackageType << 16) | (*itVec));
		}
		m_owner->SendSectorChange(vec);
	}

	//同步属性
	vector<int> tempAllVector;
	map<int,int>::iterator tempIt = tempAttList.begin();

	for(;tempIt != tempAttList.end();++tempIt)
	{
		tempAllVector.push_back(tempIt->first);
	}

	m_owner->SynCharAttribute(tempAllVector);

	//跑马灯
	BroadCastLucklyTurntable(brocastVec);

	m_owner->SetDataFlag(eLucklyTurntable);
}

void LucklyTurntableMgr::SetReawInfo(RewardHail::ToClientGetReward &info, list<int>&vec)
{
	/*获得 奖励的数据中  首元素 填 当前免费次数*/
	info.set_res(0);
	int freeNum = m_owner->GetCounterService()->GetNumById(FREEROLL_LUCKLYTURNTABLE);
	info.set_type(freeNum);

	list<int>::iterator it = vec.begin();
	for(; it != vec.end(); ++it)
	{
		info.add_list(*it);
	}
}

void LucklyTurntableMgr::BroadCastLucklyTurntable(list<TurntableRward>& brocastVec)
{
	if(brocastVec.size() <=0)
		return ;
	list<TurntableRward>::iterator it = brocastVec.begin();
	for(; it != brocastVec.end(); ++it)
	{
		const LucklyTurntableInfo* tempFileData = LucklyTurntableLoader::GetInstance()->getInfoByID(it->m_ID);
		if(tempFileData == NULL)
			continue;
		string strM = ChatMgr::GetInstance()->GetStringByInt(tempFileData->m_ID);
		string strE = ChatMgr::GetInstance()->GetStringByInt(tempFileData->m_Counts);

		ChatMgr::GetInstance()->Marquee(eMarqueeLucklyTurntable, m_owner->GetName(),strM, "");
	}
}

DWORD64 LucklyTurntableMgr::CalNeedTm(int intertm)
{
	DWORD64 nowtm = CUtil::GetNowSecond();
	int needTm = intertm - (int)(nowtm - enterTm);
	if(needTm <0)
		return  nowtm;
	return needTm + nowtm;
}

void LucklyTurntableMgr::ChangGoldenAttr(int type)
{
	vector<int> goldenVec;

	goldenVec.push_back(type);
	m_owner->SynCharAttribute(goldenVec);
}

int LucklyTurntableMgr::LucklyTurntable_IsHaveFreeNum()
{
	const ProLucklyTurntable * proPtr = LucklyTurntableLoader::GetInstance()->GetProLuckly();

	if((bool)proPtr)
	{
		if(m_owner->IsOpenSystem(eStLucklyTurntable)==false)
			return 0;
		int freeNum = m_owner->GetCounterService()->GetNumById(FREEROLL_LUCKLYTURNTABLE);
		return proPtr->freeRoll - freeNum;
	}
	return 0;
}

int LucklyTurntableMgr::ProduceRandNum(int randScop)
{
	int randNum = rand()%randScop +1;
	int randSec = rand()%randScop +1;
	int newRand = 0;

	for(int temp = randNum,k=10;temp/10 !=0;)
	{
		newRand += temp%10*k;
		temp /=10;
		k*=10;
	}
	return (newRand *randSec)%randScop +1;
}

//打乱一个列表
template<class T>
void LucklyTurntableMgr::randOneList(vector<T>& dataList)
{
	if(dataList.size() == 0)
		return ;

	short tempCounts = dataList.size();
	short tempIndex  = 0;
	T tempData;

	//取一个随机数，替换最后一个数，一直向前替换，直到第一个
	for(short i = tempCounts - 1;i > 1;--i)
	{
		tempIndex = rand() % i;

		if(i == tempIndex)
			continue;

		tempData 			 = dataList[tempIndex];
		dataList[tempIndex]  = dataList[i];
		dataList[i] 		 = tempData;
	}

	return ;
}












