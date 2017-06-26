/*
 * CJobLuckyMgr.cpp
 *
 *  Created on: 2014年8月15日
 *      Author: helh
 */

#include "CJobLuckyMgr.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../Chat/ChatMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../ServerEventDefine.h"
#include "../Attribute/AttributeCreature.h"
#include "../Fighting/Fighting.h"
#include "../Attribute/AttributeManage.h"
#include "../StatisticMgr/StatisticHandle.h"

#define FULL_LEVEL_JOB	10	//满级官运等级

CJobLuckyMgr::CJobLuckyMgr(Player * p):m_owner(p)
{
	ReInit();
}

CJobLuckyMgr::~CJobLuckyMgr()
{
	this->ReleaseCJobLucky();
}

void CJobLuckyMgr::ReInit()
{
	m_contri = 0;
	m_randIndex = eSevenSon;
	m_randJobCount = 0;

	m_package.clear();
	m_examPackage.clear();
	m_rolePackage.clear();

	m_package.resize(15);

	if(!m_lvEvent.IsValid())
			m_lvEvent = RegEvent(m_owner, PLAYER_LEVEL_UP_EVENT_TYPE, this, &CJobLuckyMgr::HandlePlayerAdvanceLevel);
}

void CJobLuckyMgr::ReleaseCJobLucky()
{
	m_package.clear();
	m_examPackage.clear();
	m_rolePackage.clear();

	if (m_lvEvent.IsValid())
	{
		m_lvEvent.SetNull();
	}
}

void CJobLuckyMgr::InitCJobLucky(const PlayerInfo::RandCJobLucky *info)
{
	m_owner->SetInitPartDataFlag(eCJobItem);

	if ((int)(m_package.size()) < info->package_size())
	{
		m_package.resize(info->package_size());
	}

	for (int i=0; i < info->package_size(); ++i)
	{
		CJobInfo cInfo;
		PlayerInfo::RandCJobLucky_CJobItem item = info->package(i);

		cInfo.id = item.id();
		cInfo.exp = item.exp();
		m_package[i] = cInfo;
	}

	for (int j=0; j < info->exampackage_size(); ++j)
	{
		m_examPackage.push_back(info->exampackage(j));
	}

	for (int k=0; k < info->rolepackage_size(); ++k)
	{
		PlayerInfo::RandCJobLucky_CreatureCJob creat = info->rolepackage(k);
		vector<CJobInfo> vec;

		for (int l = 0; l < creat.item_size(); ++l)
		{
			CJobInfo cInfo;
			PlayerInfo::RandCJobLucky_CJobItem item = creat.item(l);

			cInfo.id = item.id();
			cInfo.exp = item.exp();
			vec.push_back(cInfo);
		}

		if (creat.id() == 0)
			m_rolePackage[m_owner->GetID()] = vec;
		else
			m_rolePackage[creat.id()] = vec;
	}

	this->CompareJobLuckyPackage();

	if (info->rolepackage_size() == 0)
	{
		vector<CJobInfo> vec;

		m_rolePackage[m_owner->GetID()] = vec;
	}

	m_contri = info->jobexp();
	m_randJobCount = info->randnum();
}

//上线计算属性
void CJobLuckyMgr::onlineCalculateAtt()
{
	map<int64,vector<CJobInfo> >::iterator tempIt = m_rolePackage.find(m_owner->GetID());
	if (tempIt == m_rolePackage.end())
		return ;

	map<ECharAttrType,int64> tempAttList;

	for (size_t i = 0;i < tempIt->second.size();++i)
	{
		getAtt(tempAttList,tempIt->second[i].id);
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttOfficial);

	m_owner->calculateResultAddAtt(tempAttList,tempValueType);

	onlineCalculateFight();

	return ;
}

//计算战斗力
int CJobLuckyMgr::onlineCalculateFight()
{
	map<ECharAttrType,int64> tempAttList;

	map<int64,vector<CJobInfo> >::iterator tempIt = m_rolePackage.find(m_owner->GetID());
	if (tempIt != m_rolePackage.end())
	{
		for (size_t i = 0;i < tempIt->second.size();++i)
		{
			getAtt(tempAttList,tempIt->second[i].id);
		}
	}

	int tempResult = m_owner->getFightingManage()->CalculateFighting(tempAttList);
	return m_owner->getFightingManage()->setFighting(eFightingJobLucky,tempResult);
}

void CJobLuckyMgr::getAtt(map<ECharAttrType,int64>& attList,int id)
{
	OfficialItemInfo* tempInfo = CJobLuckyLoader::GetInstance()->GetOfficialInfo(id);
	if(tempInfo == NULL)
		return;

	map<ECharProf,map<ECharAttrType,int64> >::const_iterator tempProfessionIt = tempInfo->m_AttList.find(
			(ECharProf)m_owner->getAttManage()->getValue<BYTE>(eCharProfession));

	if(tempProfessionIt == tempInfo->m_AttList.end())
	{
		return ;
	}

	map<ECharAttrType,int64>::const_iterator tempAttIt = tempProfessionIt->second.begin();
	for(;tempAttIt != tempProfessionIt->second.end();++tempAttIt)
	{
		if(tempAttIt->second == 0)
		{
			continue ;
		}
		attList[tempAttIt->first] += tempAttIt->second;
	}

	return ;
}

void CJobLuckyMgr::CompareJobLuckyPackage()
{
	this->AddJobLuckyPackage();
}

void CJobLuckyMgr::SetCJobLucky(PlayerInfo::RandCJobLucky * info)
{
	info->set_randnum(m_randJobCount);
	info->set_jobexp(m_contri);

	for (uint i = 0; i < m_package.size(); ++i)
	{
		PlayerInfo::RandCJobLucky_CJobItem * item = info->add_package();

		item->set_id(m_package[i].id);
		item->set_exp(m_package[i].exp);
	}

	for (uint j = 0; j < m_examPackage.size(); ++j)
	{
		info->add_exampackage(m_examPackage[j]);
	}

	map<int64, vector<CJobInfo> >::iterator it = m_rolePackage.begin();
	for (; it != m_rolePackage.end(); ++it)
	{
		PlayerInfo::RandCJobLucky_CreatureCJob * creat = info->add_rolepackage();

		if (m_owner->GetID() == it->first)
			creat->set_id(0);
		else
			creat->set_id(it->first);

		for (uint k=0; k<it->second.size(); ++k)
		{
			PlayerInfo::RandCJobLucky_CJobItem * item = creat->add_item();

			item->set_id(it->second[k].id);
			item->set_exp(it->second[k].exp);
		}
	}
}

int64 CJobLuckyMgr::RandCJob(const int &index,int64& status)
{
	if (index != eKingWife)
	{
		if (index != this->GetRandIndex())
		{
			return eCJobIndexIstMatch;
		}
	}

	const DWORD maxExamPackageSize = GameParaLoader::GetInstance()->getJobLuckyPackageSize();
	const int firstJobLuckyID = GameParaLoader::GetInstance()->getFirstJobLuckyId();
	const int jobLuckyContriID = GameParaLoader::GetInstance()->getJobLuckyContriId();

//	StatisticInfo::CostAndGotInfo costAndGotInfo;

	int newIndex = 0;

	bool isCostGolden = false;

	if (index == eKingWife && index != this->GetRandIndex())
	{
		//直接到皇后，需要元宝
		int gold = CJobLuckyLoader::GetInstance()->GetGoldToKingWife();

		if (!m_owner->IsEnoughGolden(gold))
		{
			return eNotEnogGolden;
		}

		if (m_examPackage.size() >= maxExamPackageSize)
		{//科举格子最大20
			return eCJobPackageFull;
		}

//		StatisticInfo::CostItemInfo * costGoldenInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costGoldenInfo,eCharGolden,m_owner->GetMyself());

//		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_owner->GetMyself());
		//扣除金钱
		vector<int> vec;
		int costGolden = 0;
		int costbGolden = 0;

		m_owner->DecreaseGoldenCost(gold, costGolden, costbGolden);
		if(costGolden > 0)
			vec.push_back(eCharGolden);
		if(costbGolden > 0)
			vec.push_back(eCharBindGolden);

		m_owner->SynCharAttribute(vec);
		newIndex = eKingWife;

		isCostGolden = true;

//		StatisticMgr::SetItemCostAfter(costGoldenInfo,eCharGolden,costGolden,m_owner->GetMyself());
//		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,costbGolden,m_owner->GetMyself());

		// 统计元宝花费
//		StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(), eStatic_AboutGolden, eStaMinor_AboutGolden_JobLuckyRandJob,1, gold);

		//统计元宝探望
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_OfficeStrans, eStaMinor_OfficeStrans_CostGold, 1);
	}
	else
	{
		int money = CJobLuckyLoader::GetInstance()->GetRandCost(this->GetRandIndex());
		if (m_owner->getAttManage()->getValue<int>(eCharMoney) < money)
		{
			return eMoneyNotEnough;
		}

		if (m_examPackage.size() >= maxExamPackageSize)
		{//科举格子最大20
			return eCJobPackageFull;
		}

//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costItemInfo,eCharMoney,money,m_owner->GetMyself());
		//扣除金钱
		vector<int> vec;

		m_owner->ChangeMoney(money, false);
		vec.push_back(eCharMoney);
		m_owner->SynCharAttribute(vec);
		newIndex = this->GetRandIndex();

//		StatisticMgr::SetItemCostAfter(costItemInfo,eCharMoney,m_owner->GetMyself());
		//统计一天官运消耗的铜钱数
//		if(eAccountType_User == m_owner->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_OfficeStrans,eStaMinor_OfficeStrans_CostMoney,money);
			GLOBAL_STATISTIC_GM(m_owner->GetMyself(), GlobalType::Main::eJobLucky, GlobalType::Minor::eJobLucky_ExamCostMoney, StatistSaveType::Int64_Data(money));
//			GLOBAL_STATISTIC(Global::MainType::eJobLucky, Global::MinorType::eJobLucky_ExamCostMoney, StatistSaveType::Int64_Data(money), StatisticMgr::GetInstance());

	}

	int newid = 0;
	int newRandIndex = 0;

	CJobLuckyLoader::GetInstance()->GetRandCJob(newIndex, newid, newRandIndex);
	//元宝探望直接到太上皇
	if (isCostGolden)
		newRandIndex = eBeyondKing;

	if (newid <= 0 || newRandIndex < eSevenSon || newRandIndex > eBeyondKing)
	{
		return eNull;
	}

	if (m_randJobCount <=0
			&& this->GetRandIndex() == eSevenSon
			&& (newid ==10101 || newid ==10111 || newid ==10201))
	{
		/*第一次 官运 不能出现没用的 */
		newid = firstJobLuckyID;
	}

	//功勋写死
	if (newid == jobLuckyContriID)
	{
		m_contri += 1;
	}
	else
	{
		m_examPackage.push_back(newid);

		OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(newid);
		if(info == 0)
		{
			LOG_ERROR(FILEINFO, "official id is error");
			return eNull;
		}

//		StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotItemInfo,info->id,1);

		ExchangeJobluckyArgs e(m_owner->GetMyself(),info->quality,info->lv);
		m_owner->FireEvent(PLAYER_JOBLUCKY_EXCHANGE,e);
	}

	this->SetRandIndex(newRandIndex);

	m_owner->SetDataFlag(eCJobItem);

	m_randJobCount++;

	m_owner->GetVitalityMgr()->FinshVitality(eViCJobItem);

//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_OfficeStrans,eStaMinor_OfficeStrans_RandJobEvent,&costAndGotInfo);

	status =  (((int64)(newRandIndex) << 32) | newid);

	return 0;
}

void  CJobLuckyMgr::RandJob(const int &index, int& err,int64& status)
{
	BEHAVIOR_MONI(m_owner->GetMyself(), BehaviorType::Main::eJobLucky, BehaviorType::Minor::eJobLucky_Exam, MONITOR);

	err = RandCJob(index,status);

	
}

int CJobLuckyMgr::PickedUpToPackage(int index, int param)
{
	if (index < 0 || index > (int)(m_examPackage.size()))
	{
		return eCJobIndexIstMatch;
	}

	if(IsPackageFull())
		return ePackageIsFull;

	if (index == 0)
	{
		//一键拾取
		vector<int>::iterator it = m_examPackage.begin();
		for (; it < m_examPackage.end(); )
		{
			OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(*it);
			if(info == 0)
			{
				LOG_ERROR(FILEINFO, "official id is error");
				++it;
				continue;
			}

			if (info->quality > 0)
			{//品质为0不能拾取
				if(AddPackage(*it))
				{
					if (info->quality == GameParaLoader::GetInstance()->get0fficialItemQualityOrange())
					{
						ChatMgr::GetInstance()->Marquee(eMarqueeCJobItem,m_owner->GetName(),ChatMgr::GetInstance()->GetStringByInt(*it),"");
					}

					it = m_examPackage.erase(it);
					m_owner->SetDataFlag(eCJobItem);
				}
				else
				{
					//背包满了
					break;
				}
			}
			else
			{
				++it;
			}
		}
	}
	else
	{
		vector<int>::iterator it = m_examPackage.begin();
		for (int i = 0; it < m_examPackage.end(); ++it, ++i)
		{
			if (i == (index - 1))
			{
				OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(m_examPackage[i]);
				if (info == 0)
				{
					LOG_ERROR(FILEINFO, "official id is error");
					return eNull;
				}

				if (info->quality <= 0)
				{
					return eQualityNotSale;
				}

				if (AddPackage(*it))
				{
					it = m_examPackage.erase(it);
					m_owner->SetDataFlag(eCJobItem);

					//统计 获得 官运 物品品质
//					if (info->quality == GameParaLoader::GetInstance()->getOfficialItemQualityViolet()
//							|| info->quality == GameParaLoader::GetInstance()->get0fficialItemQualityOrange())
//					{
//						StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_OfficeStrans, eStaMinor_OfficeStrans_GetOrgAndPurple, 1);
//					}
					break;
				}
				else
				{
					return ePackageIsFull;
				}
			}
		}
	}

	return param;
}

bool CJobLuckyMgr::AddPackage(int id)
{
	uint i = 0;

	for (; i < m_package.size(); ++i)
	{
		if (m_package[i].id <= 0)
		{
			m_package[i].id = id;
			break;
		}
	}

	if(i >= m_package.size())
	{
		return false;
	}

	m_owner->SetDataFlag(eCJobItem);

	return true;
}

bool CJobLuckyMgr::IsPackageFull()
{
	uint i = 0;

	for (; i < m_package.size(); ++i)
	{
		if (m_package[i].id <= 0)
		{
			return false;
		}
	}

	return true;
}

int CJobLuckyMgr::SaleCJobItem(bool packageType, int index, int param)
{
	int money = 0;

	if (packageType)
	{
		//官印背包
		if (index <= 0 || index > (int)(m_package.size()))
		{
			return eCJobIndexIstMatch;
		}

		if (m_package[index-1].id <= 0)
		{
			return eCJobPackageNull;
		}

		OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(m_package[index-1].id);
		if (info == 0)
		{
			LOG_ERROR(FILEINFO, "official id is error");
			return eNull;
		}

		m_package[index-1].id = -1;
		m_package[index-1].exp = 0;
		m_owner->SetDataFlag(eCJobItem);
		money = info->sale;
	}
	else
	{
		if (index < 0 || index > (int)(m_examPackage.size()))
		{
			return eCJobIndexIstMatch;
		}

		if (index == 0)
		{
			vector<int>::iterator it = m_examPackage.begin();
			for (; it != m_examPackage.end();)
			{
				OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(*it);
				if(info == 0)
				{
					LOG_ERROR(FILEINFO, "official id is error");
					++it;
					return eNull;
				}

				if (info->quality == 0)
				{
					it = m_examPackage.erase(it);
					m_owner->SetDataFlag(eCJobItem);
					money += info->sale;
				}
				else
				{
					++it;
				}
			}
		}
		else
		{
			OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(m_examPackage[index-1]);
			if (info == 0)
			{
				LOG_ERROR(FILEINFO, "official id is error");
				return eNull;
			}

			vector<int>::iterator it = m_examPackage.begin();
			it += index - 1;
			it = m_examPackage.erase(it);
			m_owner->SetDataFlag(eCJobItem);
			money = info->sale;
		}
	}

	if (money > 0)
	{
		vector<int> vec;

		m_owner->ChangeMoney(money,true);
		vec.push_back(eCharMoney);
		m_owner->SynCharAttribute(vec);

//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,money);
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Money,eStaMinor_Money_SoldJobLucky,&costAndGotInfo);
	}

	return param;
}

int CJobLuckyMgr::HanlePackage(bool hType, bool packageType, int index, int param)
{
	if (hType)
	{
		//拾取
		if(!packageType)
		{
			BEHAVIOR_MONI(m_owner->GetMyself(), BehaviorType::Main::eJobLucky, BehaviorType::Minor::eJobLucky_Pick, MONITOR);

			int err = PickedUpToPackage(index, param);

			

			return err;
		}
	}
	else
	{
		BEHAVIOR_MONI(m_owner->GetMyself(), BehaviorType::Main::eJobLucky, BehaviorType::Minor::eJobLucky_Sale, MONITOR);

		//卖出
		int err = SaleCJobItem(packageType, index, param);

		

		return err;
	}

	return eNull;
}

int CJobLuckyMgr::ExchangeCJob(int id)
{
	int cost = CJobLuckyLoader::GetInstance()->ExchangeCJob(id);
	if (cost <= 0)
	{
		return eExchangedIDError;
	}

	if (m_contri < cost)
	{
		return eContriIsnEnough;
	}

	OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(id);
	if (info == 0)
	{
		LOG_ERROR(FILEINFO, "official id is error");
		return eNull;
	}

	if (!AddPackage(id))
	{
		return ePackageIsFull;
	}

//	StatisticInfo::CostAndGotInfo costAndGotInfo;
//	StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//	costItemInfo->set_beforenum(m_contri);
//	StatisticMgr::SetItemCost(costItemInfo,0,cost);

//	StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//	StatisticMgr::AddItemGotInfo(gotItemInfo,info->id,1);

	m_contri -= cost;
	m_owner->SetDataFlag(eCJobItem);

	ExchangeJobluckyArgs e(m_owner->GetMyself(),info->quality,info->lv);
	m_owner->FireEvent(PLAYER_JOBLUCKY_EXCHANGE,e);

//	costItemInfo->set_afternum(m_contri);

//	if(eAccountType_User == m_owner->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_OfficeStrans,eStaMinor_OfficeStrans_ContriExchangeEvent,&costAndGotInfo);
//	//统计兑换官运次数
//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(),eStatic_OfficeStrans,eStaMinor_OfficeStrans_Exchange, 1);

	return id;
}

void CJobLuckyMgr::gmAddJob(int id,int exp,int index)
{
	OfficialItemInfo* info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(id, exp);
	if (info == 0)
	{
		return;
	}

	if(index >= (int)m_package.size())
	{
		return ;
	}

	if (exp > info->addexp)
	{
		exp = info->addexp;
	}

	m_package[index].id  = info->id;
	m_package[index].exp = exp;

	return ;
}


//身上是否有满级官运
bool CJobLuckyMgr::isHaveFullLevelJob()
{
	OfficialItemInfo * tempInfo = NULL;

	for(size_t i = 0;i < m_package.size();++i)
	{
		tempInfo = CJobLuckyLoader::GetInstance()->GetOfficialInfo(m_package[i].id);
		if(tempInfo == NULL)
		{
			continue;
		}

		if(tempInfo->lv >= FULL_LEVEL_JOB)
		{
			return true;
		}
	}

	return false;
}

void CJobLuckyMgr::EatOtherCJob(int param, ServerReturn::ServerThreeInt &ret)
{
	if (param == 0)
	{
		//身上拥有满级官运
		if(isHaveFullLevelJob())
		{
			ret.set_rett(eHaveFullLevelJob);

			return ;
		}

		//一键吞噬
		int id = -1;
		int index = -1;
		int exp = 0;
		int allexp = 0;

		for (int i=0; i < (int)(m_package.size()); ++i)
		{
			if ((id < 0 || id == 40001) && m_package[i].id > 0)
			{
				id = m_package[i].id;
				index = i;
				exp = m_package[i].exp;
			}

			if (m_package[i].id <= 0)
				continue;

			OfficialItemInfo * binfo = CJobLuckyLoader::GetInstance()->GetOfficialInfo(m_package[i].id);
			OfficialItemInfo * einfo = CJobLuckyLoader::GetInstance()->GetOfficialInfo(id);
			if(binfo == 0 || einfo == 0)
			{
				LOG_ERROR(FILEINFO, "official id is error");
				continue;
			}

			if (binfo->id != 40001)
			{
				if (binfo->quality > einfo->quality )
				{
					id = m_package[i].id;
					index = i;
					exp = m_package[i].exp;
				}
				else if (binfo->quality == einfo->quality)
				{
					if (m_package[i].exp > exp)
					{
						index = i;
						id = m_package[i].id;
						exp = m_package[i].exp;
					}
				}
			}

			if(m_package[i].exp > 0)
				allexp += m_package[i].exp;
			else
				allexp += binfo->eatexp;

			m_package[i].id = -1;
			m_package[i].exp = 0;
		}

		if (id <= 0)
		{
			ret.set_rett(eCJobPackageNull);
			return;
		}

		OfficialItemInfo * info = NULL;

		if (id != 40001)
		{
			info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(id, allexp);
			if (info == 0)
			{
				LOG_ERROR(FILEINFO, "official advance id[%d] is error", id);

				ret.set_rett(eNull);
				return;
			}

			if (allexp > info->addexp)
				allexp = info->addexp;

			m_package[index].id = info->id;
			m_package[index].exp = allexp;
			m_owner->SetDataFlag(eCJobItem);
			ret.set_retf(((int64)allexp << 32) | info->id);
			ret.set_rets((index+1) << 16);

			ret.set_rett(eRetSuccess);

			if (info == 0)
			{
				LOG_ERROR(FILEINFO, "official id is error");
				return;
			}

			ExchangeJobluckyArgs e(m_owner->GetMyself(),info->quality,info->lv);
			m_owner->FireEvent(PLAYER_JOBLUCKY_EXCHANGE,e);
		}
		else
		{
			ret.set_retf(((int64)allexp << 32) | id);
			ret.set_rets((index+1) << 16);

			ret.set_rett(eRetSuccess);
		}
	}
	else
	{
		int bindex = (param & 0xff) - 1;
		int eindex = ((param >> 8) & 0xff) - 1;

		if (bindex < 0
				|| eindex < 0
				|| bindex >= (int)(m_package.size())
				|| eindex >= (int)(m_package.size()))
		{
			ret.set_rett(eCJobIndexIstMatch);
			return ;
		}

		if (m_package[bindex].id <= 0)
		{
			ret.set_rett(eBeginIsNull);
			return ;
		}

		if (m_package[eindex].id <= 0)
		{
			//挪动官印
			m_package[eindex].id = m_package[bindex].id;
			m_package[eindex].exp = m_package[bindex].exp;
			m_package[bindex].id = -1;
			m_package[bindex].exp = 0;
			m_owner->SetDataFlag(eCJobItem);
		}
		else
		{
			if (m_package[bindex].id == 40001)
			{
				ret.set_rett(eItemNotEat);

				return ;
			}

			OfficialItemInfo * binfo = CJobLuckyLoader::GetInstance()->GetOfficialInfo(m_package[bindex].id);
			OfficialItemInfo * einfo = CJobLuckyLoader::GetInstance()->GetOfficialInfo(m_package[eindex].id);

			if(binfo->lv >= FULL_LEVEL_JOB)
			{
				ret.set_retf(eFullJobCanNotEate);
				ret.set_rets(0);
			}


			if (binfo == 0 || einfo == 0)
			{
				LOG_ERROR(FILEINFO, "official id is error");
				ret.set_rett(eNull);
				return ;
			}

			int exp = m_package[eindex].exp + einfo->eatexp + m_package[bindex].exp;

			OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(m_package[bindex].id, exp);
			if (info == 0)
			{
				LOG_ERROR(FILEINFO, "official advance id[%d] is error", m_package[eindex].id);

				ret.set_rett(eNull);
				return ;
			}
			ExchangeJobluckyArgs e(m_owner->GetMyself(),info->quality,info->lv);
			m_owner->FireEvent(PLAYER_JOBLUCKY_EXCHANGE,e);

			if (exp > info->addexp)
				exp = info->addexp;

			m_package[eindex].id = info->id;
			m_package[eindex].exp = exp;
			m_package[bindex].id = -1;
			m_package[bindex].exp = 0;
			m_owner->SetDataFlag(eCJobItem);
		}

		ret.set_retf(((int64)m_package[eindex].exp << 32) | m_package[eindex].id);
		ret.set_rets(((eindex + 1) << 16) | param);

		ret.set_rett(eRetSuccess);
	}
}

int CJobLuckyMgr::WearCJob(const int64 &index)
{
	int bindex = (index & 0xff) - 1;
	int eindex = ((index >> 8) & 0xf) - 1;
	int64 id = index >> 32;

	if (bindex < 0
			|| eindex < 0
			|| bindex >= (int)(m_package.size())
			|| eindex >= 8)
		return eClientSendDataError;

	if (m_package[bindex].id <= 0)
		return eBeginIsNull;

	if (m_package[bindex].id == 40001)
		return eItemNotWear;

	if (id == 0)
		id = m_owner->GetID();

	int ret = AddPlayer(eindex, m_package[bindex].id, m_package[bindex].exp, id);
	if (ret != 0)
		return ret;

	m_package[bindex].id = -1;
	m_package[bindex].exp = 0;
	m_owner->SetDataFlag(eCJobItem);

	return index;
}

// 开启格子
void CJobLuckyMgr::OpenPackage(int newSectorSize, int& err)
{
	BEHAVIOR_MONI(m_owner->GetMyself(), BehaviorType::Main::eJobLucky, BehaviorType::Minor::eJobLucky_OpenPack, MONITOR);

	err = OpenPackage(newSectorSize);

	
}

int CJobLuckyMgr::AddPlayer(const int &index, const int &id, const int &exp, const int64 &player)
{
	OfficialItemInfo *infof = CJobLuckyLoader::GetInstance()->GetOfficialInfo(id);
	if (0 == infof)
	{
		LOG_ERROR(FILEINFO, "official advance id[%d] is error", id);
		return eItemError;
	}

	map<int64, vector<CJobInfo> >::iterator it = m_rolePackage.find(player);
	if (it != m_rolePackage.end())
	{
		if (index >= (int)(it->second.size()))
		{
			return eCJobIndexIstMatch;
		}

		if (it->second[index].id > 0)
			return eSectorIsnNull;

		for (BYTE i = 0; i < it->second.size() ; i++)
		{
			if (it->second[i].id > 0)
			{
				OfficialItemInfo *infos = CJobLuckyLoader::GetInstance()->GetOfficialInfo(it->second[i].id);
				if (infof->type == infos->type)
				{
					return eCJobEqual;
				}
			}
		}

		it->second[index].id = id;
		it->second[index].exp = exp;

		if (m_owner->GetMyself())
		{
			PlayerJobLuckly tempArgs(m_owner->GetMyself(),infof->quality);
			m_owner->FireEvent(PLAYER_WARE_JOB_LUCKLY,tempArgs);
		}
	}
	else
	{
		return eNull;
	}

	onlineCalculateFight();
	AddAttr(id);

	return 0;
}

void CJobLuckyMgr::AddAttr(const int &id)
{
	OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(id);
	if (info == 0)
	{
		LOG_ERROR(FILEINFO, "official advance id[%d] is error", id);
		return ;
	}

	vector<int> type;
	vector<int> typeBro;

	map<ECharProf,map<ECharAttrType,int64> >::iterator tempProfessionIt = info->m_AttList.find(
			(ECharProf)m_owner->getAttManage()->getValue<BYTE>(eCharProfession));

	if(tempProfessionIt == info->m_AttList.end())
	{
		return ;
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttOfficial);

	map<ECharAttrType,int64>::iterator tempAttIt = tempProfessionIt->second.begin();
	for(;tempAttIt != tempProfessionIt->second.end();++tempAttIt)
	{
		if(tempAttIt->second == 0)
		{
			continue ;
		}
		m_owner->addAtt(tempAttIt->first,tempAttIt->second,true,tempValueType);
		if(tempAttIt->first == eCharHPMax || eCharMPMax == tempAttIt->first)
		{
			typeBro.push_back(tempAttIt->first);
		}
		else
		{
			type.push_back(tempAttIt->first);
		}
	}

	type.push_back(eFightPower);

	if (typeBro.size() > 0)
	{
		m_owner->BroadcastCharAttribute(typeBro);
	}

	if (type.size() > 0)
	{
		m_owner->SynCharAttribute(type);
	}
}

void CJobLuckyMgr::DelAttr(const int &id)
{
	OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo(id);
	if (info == 0)
	{
		LOG_ERROR(FILEINFO, "official advance id[%d] is error", id);
		return ;
	}

	vector<int> type;
	vector<int> typeBro;

	map<ECharProf,map<ECharAttrType,int64> >::iterator tempProfessionIt = info->m_AttList.find(
			(ECharProf)m_owner->getAttManage()->getValue<BYTE>(eCharProfession));

	if(tempProfessionIt == info->m_AttList.end())
	{
		return ;
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttOfficial);

	map<ECharAttrType,int64>::iterator tempAttIt = tempProfessionIt->second.begin();
	for(;tempAttIt != tempProfessionIt->second.end();++tempAttIt)
	{
		if(tempAttIt->second == 0)
		{
			continue ;
		}
		m_owner->addAtt(tempAttIt->first,tempAttIt->second,false,tempValueType);
		if(tempAttIt->first == eCharHPMax)
		{
			typeBro.push_back(tempAttIt->first);
		}
		else
		{
			type.push_back(tempAttIt->first);
		}
	}

	type.push_back(eFightPower);

	if (typeBro.size() > 0)
	{
		m_owner->BroadcastCharAttribute(typeBro);
	}

	if (type.size() > 0)
	{
		m_owner->SynCharAttribute(type);
	}
}

int CJobLuckyMgr::OpenPackage(int newSectorSize)
{
	if (m_package.size() >= 30)
		return eOpenPackageFull;

	if (newSectorSize <= 0 || m_package.size() + newSectorSize > 30)
		return eClientSendDataError;

	if (!m_owner->IsEnoughGolden(30 * newSectorSize))
	{
		return eNotEnogGolden;
	}

	CJobInfo info;
	vector<int> vec;
	int costGolden = 0;
	int costbGolden = 0;

	int price = 30 * newSectorSize;
	for (int i = 0;i < newSectorSize;++i)
		m_package.push_back(info);

	m_owner->DecreaseGoldenCost(price, costGolden, costbGolden);
	if(costGolden > 0)
		vec.push_back(eCharGolden);
	if(costbGolden > 0)
		vec.push_back(eCharBindGolden);

	m_owner->SynCharAttribute(vec);
	m_owner->SetDataFlag(eCJobItem);

	// 统计元宝花费
//	StatisticMgr::GetInstance()->StatisticPlayerGetItems(m_owner->GetMyself(), eStatic_AboutGolden, eStaMinor_AboutGolden_JobLuckyOpenPackage,1, price);

	return 0;
}

int CJobLuckyMgr::ChangeCJobSector(int64 index)
{
	int bindex = (index & 0xf) - 1;
	int eindex = ((index >> 4) & 0xf) - 1;
	int64 id = index >> 32;

	if (id == 0)
		id = m_owner->GetID();

	if (bindex < 0
			|| eindex < 0
			|| bindex >= 8
			|| eindex >= 8)
		return eCJobIndexIstMatch;

	map<int64, vector<CJobInfo> >::iterator it = m_rolePackage.find(id);
	if (it != m_rolePackage.end())
	{
		if (bindex >= (int)(it->second.size()) || eindex >= (int)(it->second.size()))
			return eCJobIndexIstMatch;

		if (it->second[bindex].id <= 0)
			return eBeginIsNull;

		int cid = it->second[bindex].id;
		int exp = it->second[bindex].exp;

		it->second[bindex].id = it->second[eindex].id;
		it->second[bindex].exp = it->second[eindex].exp;
		it->second[eindex].id = cid;
		it->second[eindex].exp = exp;
	}

	m_owner->SetDataFlag(eCJobItem);

	return index;
}

int CJobLuckyMgr::UnWearCJob(int64 index)
{
	int bindex = (index & 0xf) - 1;
	int eindex = ((index >> 4) & 0xff) - 1;
	int64 id = index >> 32;

	if (id == 0)
		id = m_owner->GetID();

	if (bindex < 0
			|| eindex < 0
			|| eindex >= (int)(m_package.size())
			|| bindex >= 8)
		return eCJobIndexIstMatch;

	if (m_package[eindex].id > 0)
	{
		return eSectorIsnNull;
	}

	map<int64, vector<CJobInfo> >::iterator it = m_rolePackage.find(id);
	if (it != m_rolePackage.end())
	{
		if (bindex >= (int)(it->second.size()))
			return eCJobIndexIstMatch;

		if (it->second[bindex].id <= 0)
			return eBeginIsNull;

		m_package[eindex].id = it->second[bindex].id;
		m_package[eindex].exp = it->second[bindex].exp;
		it->second[bindex].id = -1;
		it->second[bindex].exp = 0;

		OneParmaEventArgs tempArgs(m_owner->GetMyself());
		m_owner->FireEvent(PLAYER_UNWARE_JOB_LUCKLY,tempArgs);

		onlineCalculateFight();
		DelAttr(m_package[eindex].id);
	}

	m_owner->SetDataFlag(eCJobItem);

	return index;
}

bool CJobLuckyMgr::HandlePlayerAdvanceLevel(const EventArgs & e)
{
	this->AddJobLuckyPackage();
	return true;
}

void CJobLuckyMgr::AddJobLuckyPackage()
{
	int lv = m_owner->getAttManage()->getValue<int>(eCharLv);

//	if (m_owner->IsOpenSystem(eStCJobItem) == false)
//		return ;

	map<int64, vector<CJobInfo> >::iterator it = m_rolePackage.find(m_owner->GetID());
	if (it != m_rolePackage.end())
	{
		if (it->second.size() >= 8)
			return ;

		int count = lv / 10 - 1;
		if(count > 8)
			count = 8;

		for (int i=it->second.size(); i<count; ++i )
		{
			CJobInfo info;

			it->second.push_back(info);
		}
	}
	else
	{
		vector<CJobInfo> vec;
		int count = lv / 10 - 1;
		if (count > 8)
			count = 8;

		for (int i = it->second.size(); i < count; ++i)
		{
			CJobInfo info;

			vec.push_back(info);
		}

		m_rolePackage[m_owner->GetID()] = vec;
	}

	m_owner->SetDataFlag(eCJobItem);
	m_owner->SetDataFlag(eBaseInfo);
}

DWORD CJobLuckyMgr::GetCjobCountByLv(DWORD lv)
{
	DWORD count = 0;

	map<int64, vector<CJobInfo> >::iterator it = m_rolePackage.find(m_owner->GetID());
	if (it != m_rolePackage.end())
	{
		vector<CJobInfo>::iterator itVec = it->second.begin();
		for(; itVec!=it->second.end(); ++itVec)
		{
			if((*itVec).id > 0)
			{
				OfficialItemInfo * info = CJobLuckyLoader::GetInstance()->GetOfficialInfo((*itVec).id);
				if(info)
				{
					if(info->lv >= lv)
						++count;
				}
			}
		}
	}

	return count;
}




