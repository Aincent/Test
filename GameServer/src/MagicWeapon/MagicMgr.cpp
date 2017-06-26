/*
 * MagicMgr.cpp
 *
 *  Created on: 2014年8月8日
 *      Author: helh
 */

#include "MagicMgr.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "util.h"
#include "../QuickMessage.h"
#include "MessageCommonRet.h"
#include "../FileLoader/MallLoader.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "../Map/SceneManager.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../ServerEventDefine.h"
#include "../Chat/ChatMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Fighting/Fighting.h"
#include "../Attribute/AttributeManage.h"
#include "../StatisticMgr/StatisticHandle.h"


#define ONE_ADVANCE_MAX_COUNTS 999

MagicMgr::MagicMgr(Player * p):m_owner(p)
{
	ReInit();
}

MagicMgr::~MagicMgr()
{
	this->ReleaseMagic();
}

void MagicMgr::ReInit()
{
	m_id = -1;
	m_exp = 0;
	m_advanceTime = 0;
	m_beginTimer = 0;
	m_tempID = 0;
	m_attacktime = 0;
	m_needTm = 0;
	freeFlag = false;
	m_FirstReflush = true;

	m_skillID.clear();

	m_PassiveSkillList.clear();
}

void MagicMgr::ReleaseMagic()
{
	if(m_beginTimer.IsValid())
	{
		m_beginTimer.SetNull();
	}

	m_skillID.clear();

	m_PassiveSkillList.clear();
}

int MagicMgr::InitExp(const int &exp, const int &daynum)
{
	if(daynum <=0 || exp<=0)
		return 0;

	int retexp = exp;

	const int InterDay_Lose_Per = GameParaLoader::GetInstance()->getInterDayLosePer();

	const int WillBeZero_Per = GameParaLoader::GetInstance()->getWillBeZeroPer();

	if (0 == InterDay_Lose_Per || 0 == WillBeZero_Per)
	{
		return 0;
	}

	for(int i = 0; i < daynum; ++i)
	{
		int lose = retexp / InterDay_Lose_Per/100;
		retexp -= lose;
		if(retexp <= (exp / WillBeZero_Per/100))
		{
			return 0;
		}
	}

	m_owner->SetDataFlag(eMagicWeapon);

	return retexp;
}

void MagicMgr::InitMagicWeapon(const PlayerInfo::MagicWeapon & info)
{
	m_owner->SetInitPartDataFlag(eMagicWeapon);

	int64 nowTime = CUtil::GetNowSecond();

	m_id = info.id();
	if(m_id <= 0)
	{
		m_LevelEvent = RegEvent(m_owner,PLAYER_LEVEL_UP_EVENT_TYPE,this,&MagicMgr::onLevelEvent);//玩家升级

		return ;
	}

	m_exp = info.exp();
	m_advanceTime = info.advancetime();

	int inertday = (nowTime - m_advanceTime) / 24 * 60 * 60 * 1000;

	if(m_exp > 0 && inertday > 0)
	{
		const int Max_Inter_Day = GameParaLoader::GetInstance()->getMaxInterDay();
		if(inertday > Max_Inter_Day)
			inertday = Max_Inter_Day;

		m_exp = InitExp(m_exp, inertday);
	}

	for(int i=0; i<info.skillid_size(); ++i)
	{
		m_skillID.push_back(info.skillid(i));
	}

	for(int i = 0;i < info.passiveskilllist_size();++i)
	{
		const PlayerInfo::PassivePlayerSkill& tempInfo = info.passiveskilllist(i);
		MagicTempPassiveSkill tempSkillInfo;
		tempSkillInfo.m_Index   = tempInfo.index();
		tempSkillInfo.m_SkillID = tempInfo.skillid();

		m_PassiveSkillList.push_back(tempSkillInfo);
	}

	if(info.has_firstreflush())
		m_FirstReflush = info.firstreflush();

	MagicWeaponInfo* tempInfo = MagicLoader::GetInstance()->GetMagicWeapon(m_id);
	if(tempInfo != NULL && tempInfo->advance > 0)
	{
		m_skillID.resize(tempInfo->advance - 1);
	}

	if(m_PassiveSkillList.size() > m_skillID.size())
	{
		m_PassiveSkillList.clear();
	}

	vector<MagicTempPassiveSkill>::iterator tPassizeIt = m_PassiveSkillList.begin();
	for(;tPassizeIt != m_PassiveSkillList.end(); ++tPassizeIt)
	{
		if(tPassizeIt->m_Index >= m_skillID.size())
		{
			m_PassiveSkillList.clear();
			break;
		}
	}

	SetVitality();

	return ;
}

void MagicMgr::SetVitality()
{
	if(MagicLoader::GetInstance()->IsFinalMagic(m_id))
	{
		sVitality *vi = VitalityLoader::GetInstance()->GetVitalityInfoByID(eViMWUp);
		if(vi)
			m_owner->GetVitalityMgr()->FinshVitality(eViMWUp,vi->count);
	}
}

//上线计算属性
void MagicMgr::onlineCalculateAtt()
{
	map<ECharAttrType,int64> tempAttList;

	for(size_t i = 0;i < m_skillID.size();++i)
	{
		SkillInfo* tempInfo = SkillLoader::GetInstance()->GetSkillInfoByID(m_skillID[i]);
		if(tempInfo == NULL)
			continue ;
		m_owner->GetSkillMgr()->getDesignation(*tempInfo,tempAttList);
	}

	getAttValue(m_id,tempAttList);

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttMagic);

	m_owner->calculateResultAddAtt(tempAttList,tempValueType);

	calculateFight();

	return ;
}

//计算一次战斗力
int MagicMgr::calculateFight()
{
	map<ECharAttrType,int64> tempAttList;

	for(size_t i = 0;i < m_skillID.size();++i)
	{
		SkillInfo* tempInfo = SkillLoader::GetInstance()->GetSkillInfoByID(m_skillID[i]);
		if(tempInfo == NULL)
			continue ;
		m_owner->GetSkillMgr()->getDesignation(*tempInfo,tempAttList);
	}

	getAttValue(m_id,tempAttList);

	int tempResult = m_owner->getFightingManage()->CalculateFighting(tempAttList);
	return m_owner->getFightingManage()->setFighting(eFightingMagicWeapon,tempResult);
}

void MagicMgr::getAttValue(int id,map<ECharAttrType,int64>& attList)
{
	MagicWeaponInfo* tempData = MagicLoader::GetInstance()->GetMagicWeapon(id);
	if(tempData == NULL)
		return ;

	MagicAttrInfo& tempAtt = tempData->attr[m_owner->getAttManage()->getValue<BYTE>(eCharProfession) - 1];

	if(tempAtt.hp > 0)
	{
		attList[eCharHPMax] += tempAtt.hp;
	}

	if(tempAtt.attack)
	{
		attList[eCharPhyAttackMax] += tempAtt.attack;
	}

	if(tempAtt.pdefence)
	{
		attList[eCharPhyDefence] += tempAtt.pdefence;
	}

	if(tempAtt.hitrate)
	{
		attList[eCharHitRate] += tempAtt.hitrate;
	}

	if(tempAtt.avoidrate)
	{
		attList[eCharAvoidRate] += tempAtt.avoidrate;
	}

	if(tempAtt.crackrate)
	{
		attList[eCrackDownRate] += tempAtt.crackrate;
	}

	if(tempAtt.avoidcrack)
	{
		attList[eAvoidCrackRate] += tempAtt.avoidcrack;
	}

	return ;
}

void MagicMgr::SetMagicWeapon(PlayerInfo::MagicWeapon * info)
{
	info->set_id(m_id);
	info->set_exp(m_exp);
	info->set_advancetime(m_advanceTime);

	if(m_id <= 0)
	{
		info->set_tempid(m_tempID);

		if(m_needTm >0)
		{
			m_needTm  = m_needTm - (CUtil::GetNowSecond()-m_attacktime);

			info->set_needtime(m_needTm);
		}
	}

	for(uint i = 0; i < m_skillID.size(); ++i)
	{
		info->add_skillid(m_skillID[i]);
	}

	for(size_t i = 0;i < m_PassiveSkillList.size();++i)
	{
		PlayerInfo::PassivePlayerSkill* tempInfo = info->add_passiveskilllist();
		tempInfo->set_index(m_PassiveSkillList[i].m_Index);
		tempInfo->set_skillid(m_PassiveSkillList[i].m_SkillID);
	}

	info->set_firstreflush(m_FirstReflush);

	return ;

}

void MagicMgr::SetMagicWeapon(MagicWeapon::ClientMagicWeapon * info)
{
	eMagicMgrStateEnum state = (eMagicMgrStateEnum)CalNowState();

	SetMagicWeaponState(state, info);

	if(state == e_MagicMgr_Have)
	{
		if(MagicLoader::GetInstance()->IsFinalMagic(m_id))
			m_exp = 0;
		info->set_exp(m_exp);
		info->set_advancetime(m_advanceTime);

		for(uint i = 0; i < m_skillID.size(); ++i)
		{
			info->add_skillid(m_skillID[i]);
		}

		for(size_t i = 0;i < m_PassiveSkillList.size();++i)
		{
			MagicWeapon::PassiveTempSkill* tempSkill = info->add_passiveskilllist();

			tempSkill->set_index(m_PassiveSkillList[i].m_Index);
			tempSkill->set_skillid(m_PassiveSkillList[i].m_SkillID);
		}
	}

	return ;
}

void MagicMgr::SendToClientMagic()
{
	MagicWeapon::ClientMagicWeapon info;

	SetMagicWeapon(&info);

	QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(),m_owner->GetChannelID(), &info, MSG_SIM_GM2C_MAGICWEAPON_INFO);
}

void MagicMgr::AdvanceMagicWeapon(MagicWeapon::RetAdvanceMagic * toClient,MagicWeapon::RequestAdvanceMagic& requestMsg)
{
	BEHAVIOR_MONI(m_owner->GetMyself(), BehaviorType::Main::eMagicWeapon, BehaviorType::Minor::eMagicWeapon_Levelup, MONITOR);

	Advance(toClient, requestMsg);

	

	return ;
}

//一次升阶扣除
ServerRet MagicMgr::oneAdvanceExp(int& currentID,int& currentExp,vector<int>& passiveSkillList,map<int,short>& resultExpList,
		map<int,int>& itemList,int& money,int& golden,bool isAuto)
{
	MagicWeaponInfo* tempInfo = MagicLoader::GetInstance()->GetMagicWeapon(currentID);
	if(!tempInfo)
	{
		return eNull;
	}

	if(MagicLoader::GetInstance()->IsFinalMagic(currentID))
	{
		return eMagicWeaponMaxLv;
	}

	if(tempInfo->m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotEnough;
	}

	if(m_owner->getAttManage()->getValue<int>(eCharMoney) < money + tempInfo->m_NeedMoney)
	{
		return eMoneyNotEnough;
	}

	//获取背包中是否有这个物品
	int tempCount = m_owner->GetContainer(ePackageType)->FindPropCount(tempInfo->m_ItemID) - itemList[tempInfo->m_ItemID];

	if(tempCount < 0)
	{
		tempCount = 0;
	}

	if (tempInfo->upNeedNum > tempCount)
	{
		if(isAuto)
		{
			itemList[tempInfo->m_ItemID] += tempCount;

			int tempGolden = MallLoader::GetInstance()->GetDanLuStoneCost(tempInfo->m_ItemID) *
					(tempInfo->upNeedNum - tempCount);

			Smart_Ptr<PlayerVipPart> tempVip = m_owner->GetVipPart();
			if (tempVip)
			{
				tempGolden = PlayerVipPart::CalBuyCostGolden(tempVip, tempGolden);
			}

			if(!m_owner->IsEnoughGolden(golden + tempGolden))
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
	else
	{
		itemList[tempInfo->m_ItemID] += tempInfo->upNeedNum;
	}

	money += tempInfo->m_NeedMoney;

	//升阶部份,算经验
	int tempExp = tempInfo->m_MinExp;
	int tempGainExp = 0;

	ServerRet tempError = eRetSuccess;
	while(tempExp > 0)
	{
		if(currentExp + tempExp >= tempInfo->exp)
		{
			//生星了
			MagicWeaponInfo* tempNextInfo = MagicLoader::GetInstance()->GetMagicWeapon(currentID + 1);
			if(!tempNextInfo)
			{
				tempError = eNull;
				break;
			}

			if(tempNextInfo->m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv))
			{
				tempError = eLVNotEnough;
				break;
			}

			++currentID;

			if(tempInfo->advance < tempNextInfo->advance)
			{
				//升阶了
				int tempQuality = GameParaLoader::GetInstance()->getMagicFirstSkillQuality();
				int tempSkillID = tempNextInfo->RandSkill(tempQuality,tempQuality != INVALID_PASSIVE_SKILL_QUALITY);

				passiveSkillList.push_back(tempSkillID);
			}

			//该位置不能返回最大值,否则最大阶最后一星返回会出错
			if(MagicLoader::GetInstance()->IsFinalMagic(currentID))
			{
				tempGainExp += tempInfo->exp - currentExp;
				currentExp = 0;
				tempError = eRetSuccess;

				break;
			}
			else
			{
				tempExp	   = currentExp + tempExp - tempInfo->exp;
				tempGainExp += tempInfo->exp - currentExp;
				currentExp = 0;
			}

			tempInfo = tempNextInfo;
		}
		else
		{
			currentExp += tempExp;
			tempGainExp += tempExp;
			tempError = eRetSuccess;
			break ;
		}
	}

	resultExpList[tempGainExp] += 1;

	return tempError;
}

//增加锻炼度
void MagicMgr::AddWeaponExp(int exp)
{
	if(exp<0||exp>9999999)
		exp = 9999999;

	if(m_id <= 0)
	{
		return;
	}

	MagicWeaponInfo * mInfo = MagicLoader::GetInstance()->GetMagicWeapon(m_id);
	if(!mInfo)
	{
		return;
	}

	if(MagicLoader::GetInstance()->IsFinalMagic(m_id))
	{
		return;
	}

	while(exp + m_exp >= mInfo->exp)
	{
		MagicWeaponInfo * bInfo = MagicLoader::GetInstance()->GetMagicWeapon(m_id + 1);
		if(!bInfo)
		{
			break;
		}

		AttrChange(m_id,m_id + 1);

		m_id += 1;
		m_exp = exp + m_exp - mInfo->exp;
		exp = 0;
		if(mInfo->advance < bInfo->advance)
		{
			//升阶了
			int tempQuality = GameParaLoader::GetInstance()->getMagicFirstSkillQuality();
			int tempSkillID = bInfo->RandSkill(tempQuality,tempQuality != INVALID_PASSIVE_SKILL_QUALITY);

			m_skillID.push_back(tempSkillID);
			m_owner->GetSkillMgr()->AttackPassive(tempSkillID);

			if(mInfo->id > 0)
			{
				ChatMgr::GetInstance()->Marquee(eMarqueeMw,
																						m_owner->GetName(),
																						ChatMgr::GetInstance()->GetStringByInt(mInfo->id),
																						ChatMgr::GetInstance()->GetStringByInt(m_owner->getAttManage()->getValue<BYTE>(eCharProfession)));
			}
		}

		ServerReturn::ServerDoubleInt ret;
		ret.set_retf(m_id);
		ret.set_rets(m_owner->GetKey());
		CSceneMgr::GetInstance()->SendSynchMessage(&ret,
																										MSG_SIM_GM2C_SYNCH_MAGIC,
																										m_owner->GetMapID(),
																										m_owner->GetKey(),
																										m_owner->GetXNewPos(),
																										m_owner->GetYNewPos());

		mInfo = MagicLoader::GetInstance()->GetMagicWeapon(m_id);
		if(!mInfo)
		{
			break;
		}

		if(MagicLoader::GetInstance()->IsFinalMagic(m_id))
		{
			break;
		}
	}

	m_exp += exp;
	m_advanceTime = CUtil::GetNowSecond();
	m_owner->SetDataFlag(eMagicWeapon);
}

void MagicMgr::BeginTimer(void * arg)
{
	if(m_beginTimer.IsValid())
	{
		m_beginTimer.SetNull();
	}

	AddMagicWeapon(1, true);

	this->SetTempID(0);
}

void MagicMgr::AddMagicWeapon(int id, bool isbroad)
{
	if(m_id == id || m_id > 0)
	{
		return;
	}

	MagicWeaponInfo *mInfo = NULL;
	mInfo = MagicLoader::GetInstance()->GetMagicWeapon(id);
	if(!mInfo)
	{
		return;
	}

	int tempID = m_id;
	m_id = id;

	//上线不添加
	if(isbroad)
		AttrChange(tempID,id);

	if(isbroad)
	{
		SendToClientMagic();

		ServerReturn::ServerDoubleInt ret;

		ret.set_retf(m_id);
		ret.set_rets(m_owner->GetKey());

		CSceneMgr::GetInstance()->SendSynchMessage(&ret,
																									MSG_SIM_GM2C_SYNCH_MAGIC,
																									m_owner->GetMapID(),
																									m_owner->GetKey(),
																									m_owner->GetXNewPos(),
																									m_owner->GetYNewPos());
	}

	m_owner->SetDataFlag(eMagicWeapon);
}

bool MagicMgr::isMagicSkill(int skillID)
{
	MagicWeaponInfo *mInfo = MagicLoader::GetInstance()->GetMagicWeapon(m_id);
	if(mInfo == NULL)
	{
		return false;
	}

	return mInfo->skillid[m_owner->getAttManage()->getValue<BYTE>(eCharProfession) - 1] == skillID;
}

void MagicMgr::AttrChange(int beforeID,int currentID)
{
	map<ECharAttrType,int64> tempMinusAttList;
	map<ECharAttrType,int64> tempAddAttList;

	getAttValue(beforeID,tempMinusAttList);
	getAttValue(currentID,tempAddAttList);

	map<ECharAttrType,int64>::iterator tempIt;
	map<ECharAttrType,int64> tempSendAtt;

	tempIt = tempMinusAttList.begin();

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttMagic);

	for(;tempIt != tempMinusAttList.end();++tempIt)
	{
		m_owner->addAtt(tempIt->first,tempIt->second,false,tempValueType);
		tempSendAtt[tempIt->first] = 0;
	}

	tempIt = tempAddAttList.begin();
	for(;tempIt != tempAddAttList.end();++tempIt)
	{
		m_owner->addAtt(tempIt->first,tempIt->second,true,tempValueType);
		tempSendAtt[tempIt->first] = 0;
	}

	vector<int> tempMyselfVec;
	vector<int> tempBroadcast;
	tempIt = tempSendAtt.begin();
	for(;tempIt != tempSendAtt.end();++tempIt)
	{
		if(tempIt->first == eCharHPMax)
			tempBroadcast.push_back(tempIt->first);
		tempMyselfVec.push_back(tempIt->first);
	}

	if(tempBroadcast.size()>0)
	{
		m_owner->BroadcastCharAttribute(tempBroadcast);
	}


	if(tempMyselfVec.size() > 0)
	{
		calculateFight();
		tempMyselfVec.push_back(eFightPower);
		m_owner->SynCharAttribute(tempMyselfVec);
	}
}

int MagicMgr::FlushSkill(int index)
{
	int skillid = -1;
	bool isAuto = index >> 8;
	index = index & 0xff;

	if(index >= (int)m_skillID.size() || index < 0)
	{
		LOG_DEBUG(FILEINFO,"Player %lld magic FlushSkill %d",m_owner->GetID(),index);
		return eMagicSkillIndexBeyond;
	}

	Smart_Ptr<PropBase> stone;
	const int Magic_Flush_ItemID = GameParaLoader::GetInstance()->getMagicFlushItemId();
	int pos = Magic_Flush_ItemID; //物品id写死

	//获取背包中是否有这个物品
	stone = m_owner->GetContainer(ePackageType)->GetItemDataPtrByID(pos);
	if(!stone)
	{
		if(isAuto)
		{
			int buyPrice = MallLoader::GetInstance()->GetDanLuStoneCost(Magic_Flush_ItemID);

			if(!m_owner->IsEnoughGolden(buyPrice))
			{
				return eNotEnogGolden;
			}

			vector<int> vecType;
			//扣除元宝
			int tempGolden 	   = 0;
			int tempBindGolden = 0;

			m_owner->DecreaseGoldenCost(buyPrice,tempGolden,tempBindGolden);

			if(tempGolden > 0)
				vecType.push_back(eCharGolden);

			if(tempBindGolden > 0)
				vecType.push_back(eCharBindGolden);

			m_owner->SynCharAttribute(vecType);
		}
		else
		{
			return eItemCountNull;
		}
	}
	else
	{
		//扣除
		m_owner->GetContainer(ePackageType)->SubmitGrideItem(pos);
	}

	MagicWeaponInfo * mInfo = MagicLoader::GetInstance()->GetMagicWeapon(m_id);
	if(!mInfo)
	{
		return eNull;
	}

	int tempQuality = GameParaLoader::GetInstance()->getMagicSkillQuality();

	skillid = mInfo->RandSkill(tempQuality,tempQuality != INVALID_PASSIVE_SKILL_QUALITY ? m_FirstReflush : false);
	m_FirstReflush = false;

	//先移除之前的
	for(size_t i = 0;i < m_PassiveSkillList.size();++i)
	{
		if(m_PassiveSkillList[i].m_Index != index)
			continue;

		m_PassiveSkillList.erase(m_PassiveSkillList.begin() + i);
	}

	MagicTempPassiveSkill tempSkillInfo;
	tempSkillInfo.m_Index   = index;
	tempSkillInfo.m_SkillID = skillid;

	m_PassiveSkillList.push_back(tempSkillInfo);

	ReflushMagicWeaponSkillArgs e(m_owner->GetMyself(),skillid);
	m_owner->FireEvent(PLAYER_REFRESH_MAGICWEAPON_SKILL,e);

	m_owner->SetDataFlag(eMagicWeapon);

//	//统计 神兵升星次数
//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(),
//																																eStatic_MagicWeapon,
//																																eStaMinor_MagicWeapon_FlushSkill,
//																																1);
	m_owner->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_Role, eUIHLSmall_Role_MagicSkillCanRefresh),
			int(m_owner->GetMagicWeapon()->IsCanSkillRefresh()));

	return skillid;
}

int MagicMgr::ReplaceSkill(int index)
{
	int tempIndex = -1;
	for(size_t i = 0;i < m_PassiveSkillList.size();++i)
	{
		if(m_PassiveSkillList[i].m_Index != index)
			continue;

		tempIndex = i;
		break;
	}

	//没有找到
	if(tempIndex == -1)
	{
		LOG_DEBUG(FILEINFO,"Player %lld magic ReplaceSkill index %d",m_owner->GetID(),tempIndex);
		return eMagicSkillIndexBeyond;
	}

	int ret = m_owner->GetSkillMgr()->AttackPassive(m_skillID[m_PassiveSkillList[tempIndex].m_Index], true, true);
	if(ret < 0)
	{
		LOG_DEBUG(FILEINFO,"Player %lld magic error AttackPassive one %d index %d",m_owner->GetID(),ret,index);
		return ret;
	}

	m_skillID[m_PassiveSkillList[tempIndex].m_Index] = m_PassiveSkillList[tempIndex].m_SkillID;

	calculateFight();

	ret = m_owner->GetSkillMgr()->AttackPassive(m_skillID[m_PassiveSkillList[tempIndex].m_Index]);

	m_PassiveSkillList.erase(m_PassiveSkillList.begin() + tempIndex);

	if(ret < 0)
	{
		LOG_DEBUG(FILEINFO,"Player %lld magic error AttackPassive tow %d index %d",m_owner->GetID(),ret,index);
		return ret;
	}

	m_owner->SetDataFlag(eMagicWeapon);

	return 0;
}

void MagicMgr::CountDownMagicWeapon( int needtm)
{
	eMagicMgrStateEnum calenum = CalNowState();

	if(calenum  == e_MagicMgr_BeginTime || calenum == e_MagicMgr_InTime)
	{
		if(m_beginTimer.IsValid())
		{
			m_beginTimer.SetNull();
		}

		m_beginTimer = TimerInterface::GetInstance()->RegTimePoint(&MagicMgr::BeginTimer, this, 0, 0, 0, 0, needtm);

		if(!m_beginTimer.IsValid())
		{
			return ;
		}

		if(m_attacktime <=0)
		{
			m_attacktime = CUtil::GetNowSecond();
			m_needTm = needtm;
		}

		m_owner->SetDataFlag(eMagicWeapon);
	}
}

eMagicMgrStateEnum MagicMgr::CalNowState()
{
	if(m_id > 0)
		return e_MagicMgr_Have;

	else if(m_tempID > 0)
		return e_MagicMgr_Temp;

	if(m_tempID == -1)
	{
		if(m_attacktime <= 0)
			return e_MagicMgr_BeginTime;
		else
			return e_MagicMgr_InTime;
	}

	return e_MagicMgr_None;
}

void MagicMgr::SetMagicWeaponState(const eMagicMgrStateEnum &funEnum, MagicWeapon::ClientMagicWeapon* info)
{
	switch(funEnum)
	{
	case e_MagicMgr_Have:
	{
		info->set_id(m_id);
		break;
	}
	case e_MagicMgr_Temp:
	{
		info->set_tempid(m_tempID);
		break;
	}
	case e_MagicMgr_BeginTime:
	{
		info->set_begintime(GameParaLoader::GetInstance()->getMagicCountDownTime());
		break;
	}
	case e_MagicMgr_InTime:
	{
		info->set_begintime(m_needTm);
		break;
	}
	case e_MagicMgr_None:
	{
		info->set_tempid(m_tempID);
		info->set_id(m_id);
		info->set_begintime(0);
		break;
	}
	default:
		break;
	}
}

void MagicMgr::SetTempID(const int id)
{
	//	if(id != -1 && id != 1 && id != 0 && id != -2)
    //只 允许 -1  0   1 两个值 (1代表 启用  －1 开始到时)
	if(id == 0)
	{
		m_tempID = 0;
		return ;
	}
	else if(id == -1)
	{
		if(!freeFlag)
		{
			m_tempID = 0;

			this->SendToClientMagic();
		}

		m_tempID = -1;

		this->CountDownMagicWeapon(GameParaLoader::GetInstance()->getMagicCountDownTime());
	}
	else if(id == -2)
	{
		m_tempID = 0;
		freeFlag = true;
	}
	else if(id == 1)
	{
		MagicWeaponInfo *mInfo = NULL;

		mInfo = MagicLoader::GetInstance()->GetMagicWeapon(id);
		if(!mInfo)
		{
			return ;
		}
		else
		{
			m_tempID = mInfo->skillid[m_owner->getAttManage()->getValue<BYTE>(eCharProfession) - 1];
		}
	}
	this->SendToClientMagic();

	m_owner->SetDataFlag(eMagicWeapon);
	m_owner->SetDataFlag(eQuestInfo);
}

bool MagicMgr::CanUpdateSumitMagic()
{
	if((m_tempID>0 && !freeFlag)
			|| CalNowState() == e_MagicMgr_None )
		return true;

	return false;
}

//获得神兵阶数
BYTE MagicMgr::getMagicOrder()
{
	if(m_id <= 0)
		return 0;

	MagicWeaponInfo* tempInfo = MagicLoader::GetInstance()->GetMagicWeapon(m_id);
	if(tempInfo == NULL)
		return 0;

	return tempInfo->advance;
}

bool MagicMgr::IsMagicMaxByLv()
{
	if (MagicLoader::GetInstance()->getMaxID() <= m_id)
	{
		return true;
	}

	MagicWeaponInfo* info = MagicLoader::GetInstance()->GetMagicWeapon(m_id + 1);


	if (NULL == info)
	{
		return true;
	}

	return info->m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv);

}

bool MagicMgr::IsCanAdvance()
{
	return 0 == CheckAdvanceCond();
}

bool MagicMgr::IsCanSkillRefresh()
{
	return 0 == CheckSkillRefreshCond();
}

int MagicMgr::CheckAdvanceCond()
{
	if (!m_owner->IsOpenSystem(eStMagicWeapon))
	{
		return eNotOpen;
	}
	int curID = m_id;

	MagicWeaponInfo* tempInfo = MagicLoader::GetInstance()->GetMagicWeapon(curID);
	if(!tempInfo)
	{
		return eNull;
	}

	if(MagicLoader::GetInstance()->IsFinalMagic(curID))
	{
		return eMagicWeaponMaxLv;
	}

	if(tempInfo->m_LevelLimit > m_owner->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotEnough;
	}

	if(m_owner->getAttManage()->getValue<int>(eCharMoney) < tempInfo->m_NeedMoney)
	{
		return eMoneyNotEnough;
	}

	//获取背包中是否有这个物品
	int itemCnt = m_owner->GetContainer(ePackageType)->FindPropCount(tempInfo->m_ItemID);

	if(itemCnt < 0)
	{
		itemCnt = 0;
	}

	if (tempInfo->upNeedNum > itemCnt)
	{
			return eItemCountNull;
	}

	return 0;
}

int MagicMgr::CheckSkillRefreshCond()
{
	Smart_Ptr<PropBase> stone;
	const int Magic_Flush_ItemID = GameParaLoader::GetInstance()->getMagicFlushItemId();
	int pos = Magic_Flush_ItemID; //物品id写死

	//获取背包中是否有这个物品
	stone = m_owner->GetContainer(ePackageType)->GetItemDataPtrByID(pos);
	if(!stone)
	{
		return eItemCountNull;
	}

	MagicWeaponInfo * mInfo = MagicLoader::GetInstance()->GetMagicWeapon(m_id);
	if(!mInfo || m_skillID.size() <= 0)
	{
		return eNull;
	}

	return 0;
}

bool MagicMgr::onLevelEvent(const EventArgs& e)
{
	if(m_id > 0)
	{
		m_LevelEvent.SetNull();
		return true;
	}

	if(!m_owner->IsOpenSystem(eStMagicWeapon))
		return true;

	m_LevelEvent.SetNull();
	AddMagicWeapon(1, true);

	return true;
}

void MagicMgr::Advance(MagicWeapon::RetAdvanceMagic * toClient,MagicWeapon::RequestAdvanceMagic& requestMsg)
{
	if(m_id <= 0)
	{
		toClient->set_error(eNotHaveMagicWeapon);

		return;
	}

//	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
//	idAndCostAndGotInfo.set_beforeid(m_id);
//	StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();

	int beforeID 		 = m_id;
	int tempCurrentID    = m_id;
	int tempGolden       = 0;
	int tempExp 	     = m_exp;
	int tempMoney		 = 0;
	ServerRet tempCode   = eRetSuccess;

	vector<int> tempSKillList;
	map<int,short> tempResultExpList;
	map<int,int> tempItemList;
	int tempMaxID = MagicLoader::GetInstance()->getMaxID();

	bool tempIsFirst = true;

	ServerRet tempOneError = eRetSuccess;
	eMagicAdvanceType tempType = (eMagicAdvanceType)requestMsg.type();
	if(tempType == eMagicAdvanceNormal)
	{
		tempCode = oneAdvanceExp(tempCurrentID,tempExp,tempSKillList,tempResultExpList,tempItemList,tempMoney,tempGolden,
				requestMsg.isautobuy());

		if(tempCode == eRetSuccess)
		{
			//完成活跃次数
			m_owner->GetVitalityMgr()->FinshVitality(eViMWUp);
		}
	}
	else
	{
		int tempAdvanceID = requestMsg.id();
		while(tempAdvanceID <= tempMaxID && tempCurrentID < tempAdvanceID)
		{
			tempOneError = oneAdvanceExp(tempCurrentID,tempExp,tempSKillList,tempResultExpList,tempItemList,tempMoney,tempGolden,
					requestMsg.isautobuy());

			if(tempOneError != eRetSuccess)
			{
				if(tempIsFirst)
				{
					tempCode = tempOneError;
				}
				else
				{
					//完成活跃次数
					m_owner->GetVitalityMgr()->FinshVitality(eViMWUp);
				}
				break;
			}

			//完成活跃次数
			m_owner->GetVitalityMgr()->FinshVitality(eViMWUp);

			tempIsFirst = false;

		}
	}

	toClient->set_error(tempCode);
	toClient->set_stoperror(tempOneError);

	//不成功,升阶后神兵变为最大值，所以返回eMagicWeaponMaxLv
	if(tempCode != eRetSuccess)
	{
		return ;
	}

	vector<int> tempAtt;
	//扣除元宝
	int tempbuckleGolden 	   = 0;
	int tempbuckleBindGolden = 0;
	//扣钱
	if(tempGolden > 0)
	{
//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,m_owner->GetMyself());
//
//		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,m_owner->GetMyself());

		m_owner->DecreaseGoldenCost(tempGolden,tempbuckleGolden,tempbuckleBindGolden);

		if(tempbuckleGolden > 0)
			tempAtt.push_back(eCharGolden);

		if(tempbuckleBindGolden > 0)
			tempAtt.push_back(eCharBindGolden);

//		StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,tempbuckleGolden,m_owner->GetMyself());
//
//		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,tempbuckleBindGolden,m_owner->GetMyself());
	}

	if(tempMoney > 0)
	{
//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//		StatisticMgr::SetItemCostBefore(costItemInfo,eCharMoney,tempMoney,m_owner->GetMyself());

		m_owner->MoneyChange(tempMoney,false);

		tempAtt.push_back(eCharMoney);

//		StatisticMgr::SetItemCostAfter(costItemInfo,eCharMoney,m_owner->GetMyself());
	}

	m_owner->SynCharAttribute(tempAtt);

	//扣物品
	if(tempItemList.size() > 0)
	{
		map<int,int>::iterator tempItemIt = tempItemList.begin();
		for(;tempItemIt != tempItemList.end();++tempItemIt)
		{
			if(tempItemIt->second == 0)
			{
				continue;
			}
			BaseArgs tempArgs;
			tempArgs.GoodsID = tempItemIt->first;
			tempArgs.Num     = tempItemIt->second;
			m_owner->GetContainer(ePackageType)->DeleteGoods(&tempArgs,true);

//			StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo->add_costlist();
//			StatisticMgr::SetItemCost(costItemInfo,tempItemIt->first,tempItemIt->second);
		}
	}

	MagicWeaponInfo* tempCurrentInfo = MagicLoader::GetInstance()->GetMagicWeapon(m_id);
	MagicWeaponInfo* tempResultInfo = MagicLoader::GetInstance()->GetMagicWeapon(tempCurrentID);
	if(tempResultInfo != NULL && tempCurrentInfo != NULL && tempCurrentInfo->advance < tempResultInfo->advance)
	{
		//升阶了
		for(size_t i = 0;i < tempSKillList.size();++i)
		{
			m_skillID.push_back(tempSKillList[i]);
			m_owner->GetSkillMgr()->AttackPassive(tempSKillList[i]);
		}

		if(tempResultInfo->id > 0)
		{
			ChatMgr::GetInstance()->Marquee(eMarqueeMw,
											m_owner->GetName(),
											ChatMgr::GetInstance()->GetStringByInt(tempResultInfo->advance - 1),//升阶后的前一品阶
											ChatMgr::GetInstance()->GetStringByInt(m_owner->getAttManage()->getValue<BYTE>(eCharProfession)));//职业ID
		}

		m_owner->SynchCharAttributeToWS(eMagicweaponQuality,tempResultInfo->advance);

	}

	m_id		  = tempCurrentID;
	m_exp		  = tempExp;
	m_advanceTime = CUtil::GetNowSecond();

	if(beforeID != m_id)
		AttrChange(beforeID,m_id);

	if(tempSKillList.size() > 0)
	{
		for(size_t i = 0;i < tempSKillList.size();++i)
		{
			toClient->add_skillidlist(tempSKillList[i]);
		}
	}

	toClient->set_id(m_id);
	toClient->set_exp(m_exp);
	toClient->set_golden(tempbuckleGolden + tempbuckleBindGolden);
	toClient->set_money(tempMoney);
	toClient->set_type(tempType);

	map<int,int>::iterator tempIt = tempItemList.begin();
	for(;tempIt != tempItemList.end();++tempIt)
	{
		if(tempIt->second == 0)
		{
			continue;
		}
		MagicWeapon::MagicItemList* tempItemPtr = toClient->add_itemlist();

		tempItemPtr->set_itemid(tempIt->first);
		tempItemPtr->set_counts(tempIt->second);
	}

	int tempallExp = 0;
	if(tempResultExpList.size() > 0)
	{
		map<int,short>::iterator tempIt = tempResultExpList.begin();
		for(;tempIt != tempResultExpList.end();++tempIt)
		{
			MagicWeapon::MagicStatistics* tempMultip = toClient->add_multiplist();
			int tempFirst  = tempIt->first;
			int tempSecond = tempIt->second;
			tempMultip->set_multiple(tempFirst);
			tempMultip->set_counts(tempSecond);
			tempallExp += tempIt->first * tempIt->second;
		}

		toClient->set_addallexp(tempallExp);
	}

	ServerReturn::ServerDoubleInt tempCommon;
	tempCommon.set_retf(m_id);
	tempCommon.set_rets(m_owner->GetKey());

	CSceneMgr::GetInstance()->SendSynchMessage(&tempCommon,
												MSG_SIM_GM2C_SYNCH_MAGIC,
												m_owner->GetMapID(),
												m_owner->GetKey(),
												m_owner->GetXNewPos(),
												m_owner->GetYNewPos());

	m_owner->SetDataFlag(eMagicWeapon);

	if(m_id > beforeID)
	{
		OneParmaEventArgs e(m_owner->GetMyself());
		m_owner->FireEvent(PLAYER_WEAPON_MAGICWEAPON_ADVANCE,e);

		//统计 神兵升星次数
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(
//				m_owner->GetMyself(),eStatic_MagicWeapon,eStaMinor_MagicWeapon_LevelStar,m_id - beforeID);
//
//		idAndCostAndGotInfo.set_afterid(m_id);
//
//		if(eAccountType_User == m_owner->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(
//				m_owner->GetID(),eStatic_MagicWeapon,eStaMinor_MagicWeapon_LevelStarEvent,&idAndCostAndGotInfo);
	}

	// 神兵
	if (IsMagicMaxByLv())
	{
		m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Role, eUIHLSmall_Role_MagicCanAdvance), 1, false);
	}


	return ;
}





