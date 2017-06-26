/*
 * MarryRing.cpp
 *
 *  Created on: Sep 19, 2015
 *      Author: root
 */

#include "MarryRing.h"
#include "../Object/Player/Player.h"
#include "../Attribute/AttributeCreature.h"
#include "MarryLoader/MarryLoader.h"
#include "../Fighting/Fighting.h"
#include "Probability/Probability.h"
#include "../FileLoader/MallLoader.h"
#include "../Attribute/AttributeManage.h"
#include "StatisticBase/StatisticDefine.h"
#include "MessageStruct/StatisticInfo.pb.h"

#define MARRY_REWARD_INDEX_MAX_VALUE	32	//奖励索引对应的最大值，一个int bit的大小

MarryRing::MarryRing(Player& owner) : m_Owner(owner)
{
	ReInit();
}

MarryRing::~MarryRing()
{
	Release();
}

void MarryRing::ReInit()
{
	m_ID = 0;
	m_Exp = 0;
	m_MarryTime = 0;
	m_Reward.Init();
}

void MarryRing::Release()
{
	m_Reward.Release();
}

void MarryRing::initFromDB(PlayerInfo::RingDBData& ringProto)
{
	m_ID  = ringProto.id();
	m_Exp = ringProto.exp();

	int tempReward = ringProto.reward();
	unsigned char signinBuf[sizeof(int)] = {0};

	CUtil::SafeMemmove(signinBuf,sizeof(int),&tempReward,sizeof(int));
	m_Reward.SetData(signinBuf,sizeof(int));

	m_Owner.SetInitPartDataFlag(eMarryRingInfo);

	return ;
}

void MarryRing::getDataToDB(PlayerInfo::RingDBData& ringProto)
{
	ringProto.set_id(m_ID);
	ringProto.set_exp(m_Exp);

	int tempReward = 0;
	int tempSize   = 0;

	const unsigned char* tempChar = m_Reward.GetData(tempSize);
	CUtil::SafeMemmove(&tempReward,sizeof(int),tempChar,tempSize);

	ringProto.set_reward(tempReward);

	return ;
}


//上线计算属性
void MarryRing::onlineCalculateAtt()
{
	//没有结婚则不加属性，离过婚的所有数据都要保存
	if(m_Owner.getAttManage()->getValue<int64>(eCouple) == 0)
	{
		return ;
	}

	map<ECharAttrType,int64> tempAttList;

	getAttValue(m_ID,tempAttList);

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttMarryRing);

	m_Owner.calculateResultAddAtt(tempAttList,tempValueType);

	calculateFight();

	return ;
}

//升阶
void MarryRing::ascendeOrder(MarryUI::RequestAdvanceMarryRing& protoRequest,MarryUI::UpMarryStepReturn& protoReturn)
{
	if(m_Owner.getAttManage()->getValue<int64>(eCouple) == 0)
	{
		protoReturn.set_error(eSelfNotMarry);
		return ;
	}

	ServerRet tempError       = eRetSuccess;
	ServerRet tempStopError   = eRetSuccess;
	DWORD tempCurrentID  	  = m_ID;
	DWORD tempCurrentExp 	  = m_Exp;
	DWORD tempAllExp 	 	  = 0;
	int   tempMoney		 	  = 0;
	int   tempGolden 	 	  = 0;
	bool  tempFirst		 	  = true;
	map<DWORD,int> tempItemList;
	map<eCommonRateType,MarryExpStatistics> tempResultExpList;

	if(protoRequest.type() == eMarryVanceTypeNormal)
	{
		tempError = oneAdvanceBuckle(tempCurrentID,tempItemList,tempGolden,tempMoney,protoRequest.isautobuy());
		if(tempError == eRetSuccess)
		{
			tempError = oneAdvanceExp(tempCurrentID,tempCurrentExp,tempAllExp,tempResultExpList);
		}
	}
	else
	{
		DWORD tempMaxID = MarryLoader::GetInstance()->getMaxID();
		DWORD tempAscendeID = protoRequest.id();
		int count = 0;
		while(tempAscendeID <= tempMaxID && tempCurrentID < tempAscendeID)
		{
			tempStopError = oneAdvanceBuckle(tempCurrentID,tempItemList,tempGolden,tempMoney,protoRequest.isautobuy());
			if(tempStopError != eRetSuccess)
			{
				//第一次升不成功，则向客户端发送失败
				if(tempFirst)
				{
					tempError = tempStopError;
				}
				break;
			}

			tempStopError = oneAdvanceExp(tempCurrentID,tempCurrentExp,tempAllExp,tempResultExpList);

			if(tempStopError != eRetSuccess)
			{
				//第一次升不成功，则向客户端发送失败
				if(tempFirst)
				{
					tempError = tempStopError;
				}
				break;
			}

			tempFirst = false;
			printf("\n       count = %d    \n", ++count);
		}
	}

	//成功则扣
	if(tempError == eRetSuccess || !tempFirst)
	{
//		StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
		DWORD tempLastID = m_ID;

//		idAndCostAndGotInfo.set_beforeid(tempLastID);

		m_ID  = tempCurrentID;
		m_Exp = tempCurrentExp;

//		idAndCostAndGotInfo.set_afterid(tempCurrentID);
		//升星了，则修改属性和战斗力
		if(tempLastID != m_ID)
		{
			attChange(tempLastID,m_ID);
		}

		//大暴击和小暴击
		map<eCommonRateType,MarryExpStatistics>::iterator tempIt = tempResultExpList.begin();
		for(;tempIt != tempResultExpList.end();++tempIt)
		{
			MarryUI::MarryCritType* tempCrit = protoReturn.add_crittypelist();
			tempCrit->set_crittype(tempIt->first);
			tempCrit->set_counts(tempIt->second.m_Counts);
			tempCrit->set_exp(tempIt->second.m_Exp);
		}

//		StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();

		//扣除元宝
		int tempbuckleGolden 	 = 0;
		int tempbuckleBindGolden = 0;
		vector<int> tempAtt;
		if(tempGolden > 0)
		{
//			StatisticInfo::CostItemInfo * costInfo = costAndGotInfo->add_costlist();
//			StatisticMgr::SetItemCostBefore(costInfo,eCharGolden,m_Owner.GetMyself());
//
//			StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo->add_costlist();
//			StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_Owner.GetMyself());

			m_Owner.DecreaseGoldenCost(tempGolden,tempbuckleGolden,tempbuckleBindGolden);

			if(tempbuckleGolden > 0)
				tempAtt.push_back(eCharGolden);

			if(tempbuckleBindGolden > 0)
				tempAtt.push_back(eCharBindGolden);

//			StatisticMgr::SetItemCostAfter(costInfo,eCharGolden,tempbuckleGolden,m_Owner.GetMyself());
//
//			StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,tempbuckleBindGolden,m_Owner.GetMyself());
		}

		//扣钱
		if(tempMoney > 0)
		{
//			StatisticInfo::CostItemInfo * costInfo = costAndGotInfo->add_costlist();
//			StatisticMgr::SetItemCostBefore(costInfo,eCharMoney,tempMoney,m_Owner.GetMyself());

			m_Owner.MoneyChange(tempMoney,false);
			tempAtt.push_back(eCharMoney);

//			StatisticMgr::SetItemCostAfter(costInfo,eCharMoney,m_Owner.GetMyself());
		}

		if(tempAtt.size() > 0)
		{
			m_Owner.SynCharAttribute(tempAtt);
		}

		//扣物品
		map<DWORD,int>::iterator tempItemIt = tempItemList.begin();
		for(;tempItemIt != tempItemList.end();++tempItemIt)
		{
			if(tempItemIt->second <= 0)
			{
				continue;
			}

			BaseArgs tempArgs;
			tempArgs.GoodsID = tempItemIt->first;
			tempArgs.Num     = tempItemIt->second;
			m_Owner.GetContainer(ePackageType)->DeleteGoods(&tempArgs,true);

			MarryUI::MarryItemList* tempItemPtr = protoReturn.add_itemlist();
			tempItemPtr->set_itemid(tempItemIt->first);
			tempItemPtr->set_counts(tempItemIt->second);

//			StatisticInfo::CostItemInfo * costInfo = costAndGotInfo->add_costlist();
//			StatisticMgr::SetItemCost(costInfo,tempItemIt->first,tempItemIt->second);
		}

		protoReturn.set_id(m_ID);
		protoReturn.set_currentexp(m_Exp);
		protoReturn.set_addexp(tempAllExp);
		protoReturn.set_golden(tempbuckleGolden + tempbuckleBindGolden);
		protoReturn.set_money(tempMoney);

		m_Owner.SetDataFlag(eBaseInfo);
		m_Owner.SetDataFlag(eMarryRingInfo);

//		if(eAccountType_User == m_Owner.GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_Owner.GetID(),eStatic_Marry,eStaMinor_Marry_RingAdvance,&idAndCostAndGotInfo);
	}

	protoReturn.set_type(protoRequest.type());
	protoReturn.set_error(tempError);
	protoReturn.set_stoperror(tempStopError);

	return ;
}

//领取结婚奖励
void MarryRing::receiveMarryReward(DWORD id,ServerReturn::ServerRetInt& protoReurn)
{
	if(m_Owner.getAttManage()->getValue<int64>(eCouple) == 0)
	{
		protoReurn.set_ret(eSelfNotMarry);
		return ;
	}

	//找不到索引
	short tempIndex = MarryLoader::GetInstance()->idMappingToIndex(id);
	if(tempIndex == -1)
	{
		protoReurn.set_ret(eSystemError);
		return ;
	}

	//索引越界
	if(tempIndex >= MARRY_REWARD_INDEX_MAX_VALUE)
	{
		protoReurn.set_ret(eSystemError);
		return ;
	}

	//已经领过
	if(m_Reward.GetBit(tempIndex))
	{
		protoReurn.set_ret(eReceived);
		return ;
	}

	//ID对应数据
	const MarryRward* tempData = MarryLoader::GetInstance()->getMarryRwardData(id);
	if(tempData == NULL)
	{
		protoReurn.set_ret(eSystemError);
		return ;
	}

	if(tempData->m_Day > getMarryDays())
	{
		protoReurn.set_ret(eMarryDayNotReach);
		return ;
	}

	//给予物品
	vector<int>   tempPos;
	vector<DWORD> tempStrengthList;
	vector<DWORD> tempItemList      = tempData->m_ItemList;
	vector<DWORD> tempNumList		= tempData->m_NumList;
	vector<WORD>  tempFlagList		= tempData->m_FlagList;

	tempStrengthList.resize(tempData->m_ItemList.size(),0);

	ServerRet tempError = (ServerRet)m_Owner.GetContainer(ePackageType)->AddItem(tempItemList,tempNumList,tempPos,tempFlagList,tempStrengthList);

	if(tempError != eRetSuccess)
	{
		protoReurn.set_ret(tempError);
		return ;
	}

	DWORD64 tempRsult = (DWORD64)id << 32 | eRetSuccess;
	protoReurn.set_ret(tempRsult);

	//设置数据
	m_Reward.SetBit(tempIndex,true);

	m_Owner.SetDataFlag(eMarryRingInfo);

	return ;
}

//结婚
void MarryRing::marrySuccess(int64 otherPlayerID,string otherName)
{
	m_Owner.SetCoupleName(otherName);

	m_Owner.getAttManage()->setValue(eCouple,otherPlayerID);

	m_Owner.SetDataFlag(eBaseInfo);

	m_Owner.SetDataFlag(eMarryRingInfo);

	//结婚时世界服也设置了一次时间，玩家上线后，该数据从世界服取
	m_MarryTime = CUtil::GetNowSecond();

	//之前没有结过婚
	if(m_ID == 0)
	{
		m_ID = MarryLoader::GetInstance()->getFirstID();
	}

	//加属性
	attChange(0,m_ID);

	return ;
}

//离婚
void MarryRing::divorceSuccess()
{
	m_Owner.SetCoupleName("");

	m_Owner.getAttManage()->setValue(eCouple,0);

	m_MarryTime = 0;

	//减属性
	attChange(m_ID,0);

	m_Owner.SetDataFlag(eBaseInfo);

	m_Owner.SetDataFlag(eMarryRingInfo);

	return ;
}

int MarryRing::getRewardData()
{
	int tempReward = 0;
	int tempSize   = 0;

	const unsigned char* tempChar = m_Reward.GetData(tempSize);
	CUtil::SafeMemmove(&tempReward,sizeof(int),tempChar,tempSize);

	return tempReward;
}

//请求婚戒信息
void MarryRing::requestRingData(MarryUI::RingInfo& protoToClient)
{
	protoToClient.set_id(m_ID);

	protoToClient.set_currentexp(m_Exp);

	return ;
}

//上线世界取数据返回设置
void MarryRing::onlineSetData(int64 otherPlayerID,string otherName,DWORD64 marryTime)
{
	m_Owner.SetCoupleName(otherName);

	m_Owner.getAttManage()->setValue(eCouple,otherPlayerID);

	//玩家结婚成功世界设置数据后，会通知逻辑服，如果这个时候玩家下线有可能逻辑服找不到数据而无法设置该值
	if(otherPlayerID != 0 && m_ID == 0)
	{
		m_ID = MarryLoader::GetInstance()->getFirstID();
	}

	m_MarryTime = marryTime;

	return ;
}

//计算一次战斗力
int MarryRing::calculateFight()
{
	map<ECharAttrType,int64> tempAttList;

	//离婚或没有经婚
	if(m_Owner.getAttManage()->getValue<int64>(eCouple) != 0)
	{
		getAttValue(m_ID,tempAttList);
	}

	int tempResult = m_Owner.getFightingManage()->CalculateFighting(tempAttList);
	return m_Owner.getFightingManage()->setFighting(eFightingMarry,tempResult);
}

void MarryRing::getAttValue(int id,map<ECharAttrType,int64>& attList)
{
	const MarryRingData* tempData = MarryLoader::GetInstance()->getFileData(id);
	if(tempData == NULL)
	{
		return ;
	}

	ECharProf tempProf = (ECharProf)m_Owner.getAttManage()->getValue<BYTE>(eCharProfession);

	map<ECharProf,map<ECharAttrType,int64> >::const_iterator tempOutIt = tempData->m_AttList.find(tempProf);
	if(tempOutIt == tempData->m_AttList.end())
	{
		return ;
	}

	attList = tempOutIt->second;

	return ;
}

//属性发生改变
void MarryRing::attChange(DWORD lastID,DWORD currentID)
{
	map<ECharAttrType,int64> tempMinusAttList;
	map<ECharAttrType,int64> tempAddAttList;

	getAttValue(lastID,tempMinusAttList);
	getAttValue(currentID,tempAddAttList);

	map<ECharAttrType,int64>::iterator tempIt;
	map<ECharAttrType,int64> tempSendAtt;

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttMarryRing);

	//要减去的属性
	tempIt = tempMinusAttList.begin();
	for(;tempIt != tempMinusAttList.end();++tempIt)
	{
		m_Owner.addAtt(tempIt->first,tempIt->second,false,tempValueType);
		tempSendAtt[tempIt->first] = 0;
	}

	//要增加的属性
	tempIt = tempAddAttList.begin();
	for(;tempIt != tempAddAttList.end();++tempIt)
	{
		m_Owner.addAtt(tempIt->first,tempIt->second,true,tempValueType);
		tempSendAtt[tempIt->first] = 0;
	}

	//合并属性，防止相同的属性发两次
	vector<int> tempMyselfVec;
	vector<int> tempBroadcast;
	tempIt = tempSendAtt.begin();
	for(;tempIt != tempSendAtt.end();++tempIt)
	{
		if(tempIt->first == eCharHPMax)
		{
			tempBroadcast.push_back(tempIt->first);
		}

		tempMyselfVec.push_back(tempIt->first);
	}

	if(tempBroadcast.size()>0)
	{
		//不用广播给自己
		m_Owner.BroadcastCharAttribute(tempBroadcast,false);
	}

	if(tempMyselfVec.size() > 0)
	{
		//算一次战斗力
		calculateFight();
		tempMyselfVec.push_back(eFightPower);
		m_Owner.SynCharAttribute(tempMyselfVec);
	}

	return ;
}

//一次升阶加的经验
int MarryRing::randExp(const MarryRingData& info,eCommonRateType& type)
{
	int tempExp = info.m_BaseExp;
	switch(getCritType(info))
	{
	case eRateTypeBig:
		{
			tempExp = (int)(info.m_BaseExp * info.m_MaxCrit);

			type = eRateTypeBig;

			break;
		}
	case eRateTypeSmall:
		{
			tempExp = (int)(info.m_BaseExp * info.m_MinCrit);

			type = eRateTypeSmall;

			break;
		}
	default:
		{
			type = eRateTypeSimple;

			break;
		}
	}

	return tempExp;
}

eCommonRateType MarryRing::getCritType(const MarryRingData& info)
{
	float tempRate = (float)rand() / RAND_MAX;

	if (info.m_MaxCritRate >= tempRate)
		return eRateTypeBig;

	else if (info.m_MinCritRate >= tempRate)
		return eRateTypeSmall;

	return eRateTypeSimple;
}

//一次升阶扣钱部份
ServerRet MarryRing::oneAdvanceBuckle(DWORD currentID,map<DWORD,int>& itemList,int& golden,int& money,bool isAuto)
{
	const MarryRingData* tempInfo = MarryLoader::GetInstance()->getFileData(currentID);
	if(tempInfo == NULL)
	{
		return eNull;
	}

	if(tempInfo->m_LevelLimit > m_Owner.getAttManage()->getValue<int>(eCharLv))
	{
		return eRingLevelNotEnough;
	}

	if(m_Owner.getAttManage()->getValue<int>(eCharMoney) < money)
	{
		return eMoneyNotEnough;
	}

	//背包中是否乘余该物品
	int tempCount = m_Owner.GetContainer(ePackageType)->FindPropCount(tempInfo->m_BuckleItem) - itemList[tempInfo->m_BuckleItem];

	//足够一次
	if (tempCount >= tempInfo->m_ItemCounts)
	{
		itemList[tempInfo->m_BuckleItem] += tempInfo->m_ItemCounts;
	}
	else
	{
		//不足且，且自动购买
		if(isAuto)
		{
			itemList[tempInfo->m_BuckleItem] += tempCount;

			int tempGolden = MallLoader::GetInstance()->GetDanLuStoneCost(tempInfo->m_BuckleItem) *
					(tempInfo->m_ItemCounts - tempCount);

			tempGolden = PlayerVipPart::CalBuyCostGolden(m_Owner.GetVipPart(),tempGolden);

			if(!m_Owner.IsEnoughGolden(golden + tempGolden))
			{
				return eNotEnogGolden;
			}

			golden += tempGolden;
		}
		else
		{
			return eItemCountNull;
		}
	}

	money += tempInfo->m_ConsumeMoney;

	return eRetSuccess;
}

//一次升阶结果
ServerRet MarryRing::oneAdvanceExp(DWORD& currentID,DWORD& currentExp,DWORD& allExp,map<eCommonRateType,MarryExpStatistics>& resultExpList)
{
	//已是最大阶最大星
	if(MarryLoader::GetInstance()->isMaxRise(currentID))
	{
		return eMaxStart;
	}

	const map<DWORD,MarryRingData>& tempDataList    = MarryLoader::GetInstance()->getFileDataList();
	map<DWORD,MarryRingData>::const_iterator tempIt = tempDataList.find(currentID);

	if(tempIt == tempDataList.end())
	{
		return eNull;
	}

	eCommonRateType tempType = eRateTypeNUll;
	DWORD tempAddExp = randExp(tempIt->second,tempType);
	ServerRet tempError = eRetSuccess;
	int tempGainExp = 0;
	while(tempAddExp > 0)
	{
		//可以升到下一阶
		if(currentExp + tempAddExp >= tempIt->second.m_Exp)
		{
			//计算剩余经验
			DWORD tempExp = tempIt->second.m_Exp;

			++tempIt;
			if(tempIt == tempDataList.end())
			{
				tempError = eSystemError;
				break;
			}
			else if(tempIt->second.m_LevelLimit > m_Owner.getAttManage()->getValue<int>(eCharLv))
			{
				tempError = eMarryRingLevelNotEnough;
				break;
			}
			//达到最大阶，这里算成功
			else if(MarryLoader::GetInstance()->isMaxRise(tempIt->first))
			{
				currentID  = tempIt->first;
				allExp 	  += tempExp - currentExp;
				tempGainExp += tempExp - currentExp;
				currentExp = 0;

				tempError = eRetSuccess;
				break;
			}

			tempGainExp += tempExp - currentExp;
			tempAddExp = currentExp + tempAddExp - tempExp;

			//本次加的经验
			allExp	  += tempExp - currentExp;
			currentID  = tempIt->first;
			currentExp = 0;
		}
		else
		{
			tempGainExp += tempAddExp;
			currentExp += tempAddExp;
			allExp	   += tempAddExp;

			tempError = eRetSuccess;
			break;
		}
	}

	resultExpList[tempType].m_Counts += 1;
	resultExpList[tempType].m_Exp += tempGainExp;

	return tempError;
}

//获得结婚天数
DWORD MarryRing::getMarryDays()
{
	DWORD64 tempOfflineTime = CUtil::GetNowSecond() - m_MarryTime;

	DWORD tempOneDay = 3600 * 24 * 1000;

	//一年则粗略算为365天
	DWORD tempDays   = tempOfflineTime / tempOneDay + 1;

	return tempDays;
}






