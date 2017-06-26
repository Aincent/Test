/*
 * OnceReward.cpp
 *
 *  Created on: 2015年9月16日
 *      Author: root
 */

#include "OnceReward.h"
#include "MessageStruct/RewardHail/RewardHail.pb.h"
#include "FileLoader/MicroRewardLoader.h"
#include "../Object/Player/Player.h"
#include "../Counter/Counter.h"
#include "../QuickMessage.h"
#include "FileLoader/TotalLoginRewardLoader.h"
#include "../Container/ContainerBase.h"
#include "../Container/ParcelMgr.h"
#include "../Attribute/AttributeCreature.h"

OnceReward::OnceReward(Player* player) : m_owner(player)
{
	ReInit();
}

OnceReward::~OnceReward()
{

}

void OnceReward::ReInit()
{
	m_microRewardFlag = false;
	m_firstChargeRewardFlag = false;
}

void OnceReward::Release()
{

}

void OnceReward::SetInfoToDB(PlayerInfo::OnceRewardInfo& rewardInfo)
{
	rewardInfo.set_microrewardflag(m_microRewardFlag);
	rewardInfo.set_firstchargeoverflag(m_firstChargeRewardFlag);
}
void OnceReward::InitInfoByDB(const PlayerInfo::OnceRewardInfo& rewardInfo)
{
	// 由于前端需要改动 所以暂时首充不添加在新模块
//	if (!m_firstChargeRewardFlag)
//	{ //　首充是从别的模块移植过来 所以为了兼容旧的
//		m_firstChargeRewardFlag = rewardInfo.firstchargeoverflag();
//	}
	m_microRewardFlag = rewardInfo.microrewardflag();

}

void OnceReward::SynRewardToClient()
{
	if (NULL != m_owner)
	{
		RewardHail::ClientOnceReward toClient;
		toClient.set_firstchargereward(m_firstChargeRewardFlag);
		toClient.set_microreward(m_microRewardFlag);

		QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(), &toClient, MSG_SIM_GM2C_SYNONCEREWARD);
	}
}

bool OnceReward::IsFirstChargeRmb()
{
	if(m_owner->GetCounterService()->GetNumById(CHARGE_RMB_COUNT) >=1)
		return true;
	else
		return false;
}

void OnceReward::SetFirstChargeFlag(bool flag)
{
	m_firstChargeRewardFlag = flag;
	m_owner->SetDataFlag(eOnceRewardInfo);
}

int OnceReward::GetFirstChargeRmbReward(RewardHail::GetReward& reward,
		vector<DWORD>& goods, vector<DWORD>& num,
		vector<DWORD>& strengthLvVec)
{
	if(!IsFirstChargeRmb())
		return eNotGetReward;
	if(m_firstChargeRewardFlag)
		return eHaveGetReward;

	FirstChargeAw *info = TotalLoginRewardLoader::GetInstance()->GetFirstChargeAw(m_owner->getAttManage()->getValue<BYTE>(eCharProfession));
	if(!info)
		return eRewardHailConfigError;

	int res = 0;
	const GoodsInfo* ginfo=NULL;
	for(uint i=0; i<info->m_totalItem.size(); ++i)
	{
		ginfo = GoodsLoader::GetInstance()->GetItemDataByID(info->m_totalItem[i]);
		if(ginfo==NULL)
		{
			return eRewardHailConfigError;
		}
	}

	vector<WORD> tempFlagsList;
	tempFlagsList.resize(info->m_totalItem.size(),1);

	res = m_owner->GetContainer(ePackageType)->IsBagEnought(info->m_totalItem, info->m_totalNum,tempFlagsList);

	goods = info->m_totalItem;
	num = info->m_totalNum;
	strengthLvVec = info->m_totalLv;

	if(!res)
	{
		SetFirstChargeFlag(true);

		vector<int> vecType;

		if(info->bglod >0)
		{
			m_owner->ChangeBindGolden(info->bglod, true);
			vecType.push_back(eCharBindGolden);
			m_owner->SetDataFlag(eBaseInfo);
		}
		if(info->money >0)
		{
			m_owner->ChangeMoney(info->money, true);
			vecType.push_back(eCharMoney);
			m_owner->SetDataFlag(eBaseInfo);
		}
		if(info->zhenqi >0)
		{
			m_owner->ChangeCurForce(info->zhenqi, true);
			vecType.push_back(eCharForce);
			m_owner->SetDataFlag(eBattleInfo);
		}

		if(vecType.size() >0)
		{
			m_owner->SynCharAttribute(vecType);
		}

		SetRewardItemInfo(reward, goods);

		//统计 领取首充礼包
//		this->Statistic(eStatic_PackageGift, eStaMinor_Recharge_First);

		SynRewardToClient();
		return 0;
	}
	return res;
}

void OnceReward::SetRewardItemInfo(RewardHail::GetReward& info, vector<DWORD>& goods)
{
	for(uint i = 0; i < goods.size(); ++i)
	{
		info.add_list(goods[i]);
	}
}

// 获取微端奖励值
int OnceReward::GetMicroRewardState()
{
	if (m_microRewardFlag)
	{
		return 0;
	}
	if (!m_microRewardFlag && IsMicroLogin())
	{
		return 1;
	}

	return 0;
}

void OnceReward::SetMicroRewardFlag(bool flag)
{
	m_microRewardFlag = flag;
	m_owner->SetDataFlag(eOnceRewardInfo);
}

// 检查领取微端奖励条件
int OnceReward::CheckGetMicroReward()
{
	if (m_microRewardFlag)
	{
		return eHaveGetReward;
	}
	if (!IsMicroLogin())
	{ // 判断是否用微端登录过
		return eNotGetReward;
	}
	MicroRewardInfo info;
	MicroRewardLoader::GetInstance()->GetMicroReward(info);

	if (info.itemID.size() <= 0 || info.itemNum.size() != info.itemID.size())
	{ // 没有物品奖励
		return eItemError;
	}

	uint index = 0;
	// 检查物品信息是否正确
	DWORD tmpID = 0;
	DWORD tmpNum = 0;
	vector<WORD> bind;

	for ( ; index < info.itemID.size(); ++index)
	{
		tmpID = info.itemID[index];
		tmpNum = info.itemNum[index];
		const GoodsInfo* goodInfo = GoodsLoader::GetInstance()->GetItemDataByID(tmpID);
		if (NULL == goodInfo || tmpNum <= 0)
		{
			return eItemError;
		}
	}

	bind.resize(info.itemID.size(), info.isBind);

	Smart_Ptr<ArticleParcelMgr> container = m_owner->GetContainer(ePackageType);
	if (!container)
	{
		return ePlayerPackageFail;
	}

	int res = container->IsBagEnought(info.itemID, info.itemNum, bind);
	if (0 != res)
	{
		return res;
	}

	return 0;
}


int OnceReward::GetMicroReward(RewardHail::GetReward& reward, vector<DWORD>& goods, vector<DWORD>& num,
		vector<WORD>& bind)
{
	int res = CheckGetMicroReward();
	if (0 != res)
	{
		return res;
	}

	MicroRewardInfo info;
	vector<WORD> bindFlag;
	MicroRewardLoader::GetInstance()->GetMicroReward(info);

	bindFlag.resize(info.itemID.size(), info.isBind);

	vector<int> vecPos;
	vector<DWORD> strengthLvVec;

	goods = info.itemID;
	num = info.itemNum;
	bind = bindFlag;

	SetRewardItemInfo(reward, goods);

	strengthLvVec.resize(goods.size(), 0);
	m_owner->GetContainer(ePackageType)->AddItem(goods, num, vecPos, bind,strengthLvVec,true,npc_fly_type);


	SetMicroRewardFlag(true);

	SynRewardToClient();

	return 0;
}

//void OnceReward::Statistic(eStatisticMainType main_enum, eLogicRelevant fun_enum)
//{
//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), main_enum, fun_enum, 1);
//}

bool OnceReward::IsMicroLogin()
{
	Smart_Ptr<CounterService>& counter = m_owner->GetCounterService();
	if ((bool)counter)
	{
		int num = counter->GetNumById(LOGINMICRO_COUNT);
		if (0 < num)
		{
			return true;
		}
	}

	return false;
}
