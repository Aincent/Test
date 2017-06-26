/*
 * PlayerTitle.cpp
 *
 *  Created on: 2014年6月26日
 *      Author: root
 */

#include <math.h>
#include "PlayerTitle.h"
#include "util.h"
#include "Timer/TimerInterface.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "Log/Logger.h"
#include "../Container/ContainerBase.h"
#include "AchievementLoader.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "MessageStruct/AchievementProto.pb.h"
#include "../QuickMessage.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../ServerEventDefine.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "FileLoader/GameParaLoader.h"
#include "../Fighting/Fighting.h"
#include "../Attribute/AttributeManage.h"

Title::Title(Player * player):m_owner(player)
{
}

Title::~Title()
{
	m_titleMap.clear();
}

void Title::ReInit()
{
	m_titleMap.clear();
}

void Title::Release()
{
	m_titleMap.clear();
}

void Title::SetTitleToDB(PlayerInfo::TitleInfoList *InfoList)
{
	PlayerInfo::TitleInfo *info = InfoList->mutable_info();
	if(!info)
	{
		return;
	}

	TitleMap::iterator itMap = m_titleMap.begin();

	for(; itMap != m_titleMap.end(); ++itMap)
	{
		PlayerInfo::TitleStatus * titleStatus = info->add_titleinfo();
		sTitleStatus tempTitle = itMap->second;
		tempTitle.SetsTitleStatus(titleStatus);
	}
}

void Title::InitTitleInfo(const PlayerInfo::TitleInfoList &infoList)
{
	m_titleMap.clear();
	PlayerInfo::TitleInfo info = infoList.info();
	//玩家创建时数据库还没有成就数据，用表格初始化
	map<DWORD, sTitleEntry> & titleInfo = AchievementLoader::GetInstance()->GetTitleInfo();
	map<DWORD, sTitleEntry>::iterator it = titleInfo.begin();
	for(; it!=titleInfo.end(); ++it)
	{
		sTitleStatus titleInfo;
		if(it->second.i_titleID <= 0)
		{
			LOG_ERROR(FILEINFO, "Get Player Title Form error");
			continue;
		}

		titleInfo.titleId = it->second.i_titleID;
		titleInfo.titleStatus = eTitleNotActive;
		titleInfo.titleActive = false;
		m_titleMap[it->second.i_titleID] = titleInfo;
	}

	for(int i = 0; i < info.titleinfo_size(); i++)
	{
		int titleId = info.titleinfo(i).titleid();
		sTitleEntry *entry = AchievementLoader::GetInstance()->GetTitleEntry(titleId);
		if(!entry)
		{
			LOG_ERROR(FILEINFO, "Get Player Title Form error");
			continue;
		}

		TitleMap::iterator it = m_titleMap.find(titleId);
		if(it!=m_titleMap.end())
		{
			int titlestatus = info.titleinfo(i).titlestatus();
			int titleactive = info.titleinfo(i).titleactive();
			it->second.titleStatus = titlestatus;
			it->second.titleActive = titleactive;
		}
	}

	m_owner->SetDataFlag(eTitleInfo);

}

//上线计算属性
void Title::onlineCalculateAtt()
{
	int tempTitleID = m_owner->getAttManage()->getValue<int>(eTitleID);
	if(tempTitleID == 0)
		return ;

	sTitleEntry * tempInfo = AchievementLoader::GetInstance()->GetTitleEntry(tempTitleID);
	if(tempInfo == NULL)
		return;

	map<ECharAttrType,int64> tempAttList;

	tempAttList[eCharPhyAttackMax] = int(tempInfo->phyack);
	tempAttList[eCharPhyDefence]   = int(tempInfo->phydee);
	tempAttList[eCharHPMax] = int(tempInfo->hp);

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttTitle);

	m_owner->calculateResultAddAtt(tempAttList,tempValueType);

	calculateFight();

	return ;
}

//计算战斗力
int Title::calculateFight()
{
	int tempTitleID = m_owner->getAttManage()->getValue<int>(eTitleID);
	map<ECharAttrType,int64> tempAttList;
	if(tempTitleID != 0)
	{
		sTitleEntry * tempInfo = AchievementLoader::GetInstance()->GetTitleEntry(tempTitleID);
		if(tempInfo != NULL)
		{
			tempAttList[eCharPhyAttackMax] = int(tempInfo->phyack);
			tempAttList[eCharPhyDefence]   = int(tempInfo->phydee);
			tempAttList[eCharHPMax] = int(tempInfo->hp);
		}
	}

	int tempResult = m_owner->getFightingManage()->CalculateFighting(tempAttList);
	return m_owner->getFightingManage()->setFighting(eFightingTitle,tempResult);
}

void Title::GetTitleWithPrestige()
{
	map<DWORD, sTitleEntry>  titleMap = AchievementLoader::GetInstance()->GetTitleInfo();

	map<DWORD, sTitleEntry>::iterator it = titleMap.begin();

	for(;it != titleMap.end();it++)
	{
		sTitleEntry entry = it->second;
		//说明为成就称号
		if(entry.i_prestige > 1 && entry.i_achID == 0)
		{
			//已经拥有了
			if(IsHaveTitle(entry.i_titleID))
				continue;

			int ownerPrestige = m_owner->getAttManage()->getValue<int>(ePrestige);
			if(ownerPrestige >= (int)entry.i_prestige)
			{
				int ret = addTitle(entry.i_titleID);
				if(ret!=0)
					return;
				//推送获得称号消息
				ServerReturn::ServerRetInt title;
				int64 i = 1;
				int64 vaule = i<<32 | entry.i_titleID;
				title.set_ret(vaule);

				QuickMessage::GetInstance()->AddSendMessage(m_owner->GetID(), m_owner->GetChannelID(),&title,MSG_SIM_PRESTIGECOMPLETE);
			}
		}
	}
}

int Title::addRankingTitle(int titleId)
{
	if(IsHaveTitle(titleId))
		return -1;

	DeleteRankingTitle();

	if(titleId >= GameParaLoader::GetInstance()->getRankingFightBegin() &&
			titleId <= GameParaLoader::GetInstance()->getRankingFightBegin() + 9)
	{
		addTitle(titleId);
	}

	return 0;
}

int Title::DeleteRankingTitle()
{
	TitleMap::iterator titleIt = m_titleMap.begin();

	for(;titleIt != m_titleMap.end();++titleIt)
	{
		if(titleIt->second.titleId >= GameParaLoader::GetInstance()->getRankingFightBegin() &&
				titleIt->second.titleId <= GameParaLoader::GetInstance()->getRankingFightBegin()+9 &&
				titleIt->second.titleStatus != eTitleNotActive)
		{
			titleIt->second.titleStatus = eTitleNotActive;
		}
	}

	return 0;
}

int Title::addTitle(int titleId)
{
	//已经拥有了
	if(IsHaveTitle(titleId))
		return eAlreadyHaveThisTitle;

	sTitleStatus title;
	title.titleId = titleId;
	title.titleActive = 0;
	title.titleStatus = eTitleNotGet;
	m_titleMap[titleId] = title;

	int error = 0;
	DrawTitlePrize(titleId,error);

	return error;
}

void Title::ActiveTitle(int &titleId, int &errorType)
{

	TitleMap::iterator LastActiveit = m_titleMap.find(m_owner->getAttManage()->getValue<int>(eTitleID));

	TitleMap::iterator it = m_titleMap.find(titleId);

	if(it != m_titleMap.end())
	{
		if(LastActiveit != m_titleMap.end())
		{
			LastActiveit->second.titleActive = false;
		}

		if(it->second.titleStatus == eTitleNotActive)
		{
			errorType = ePlayerTitleNotGet;
			return;
		}

		SetActiveTitleID(it->second.titleId);

//		this->StatisticTitle();
		return;
	}

	if(titleId == 0)
	{
		titleId = m_owner->getAttManage()->getValue<int>(eTitleID);
		SetActiveTitleID(0);

//		this->StatisticTitle();
		return;
	}
	errorType = ePlayerTitleIDERROR;
}
void Title::ActiveTitleRanking(int titleId, int &errorType)
{
	TitleMap::iterator LastActiveit = m_titleMap.find(m_owner->getAttManage()->getValue<int>(eTitleID));
	if(titleId != 0)
	{
		if(LastActiveit != m_titleMap.end())
		{
			LastActiveit->second.titleActive = false;
		}

		SetActiveTitleID(titleId);

		return;
	}
	errorType = ePlayerTitleNotGet;
}
void Title::GetTitleInfo(std::vector<ServerReturn::ServerThreeInt> &list)
{
	TitleMap::iterator it = m_titleMap.begin();
	for(; it != m_titleMap.end(); ++it)
	{
//		if(it->first > 0 && (it->second).titlestatus() > eTitleNotActive)
		{
			ServerReturn::ServerThreeInt ret;
			ret.set_retf(it->first);
			ret.set_rets((it->second).titleStatus);
			ret.set_rett(m_owner->getAttManage()->getValue<int>(ePrestige));
			list.push_back(ret);
		//	cout<<endl<<"称号："<<it->first<<"状态："<<it->second.titleStatus<<endl;
		}
	}
}


bool Title::DrawTitlePrize(uint titleId, int &errorType)
{
	TitleMap::iterator it = m_titleMap.find(titleId);

	vector<int> vec;
	if(it != m_titleMap.end())
	{
		if(it->second.titleStatus == eTitleNotGet)
		{
			it->second.titleStatus = eTitleHasGet;
			sTitleEntry *entry = AchievementLoader::GetInstance()->GetTitleEntry(titleId);
			if(entry)
			{
				m_owner->ChangeMoney(entry->i_money,true);
				vec.push_back(eCharMoney);

				m_owner->AddExp(entry->i_exp);
				m_owner->ChangeCurForce(entry->i_zhenqi,true);
				vec.push_back(eCharForce);
			}

			if(vec.size() > 0)
			{
				m_owner->SynCharAttribute(vec);
			}

			m_owner->SetDataFlag(eTitleInfo);
			errorType = 0;
			return true;
		}

		if(it->second.titleStatus == eTitleHasGet)
		{
			errorType = ePlayerTitleHadGet;
			return false;
		}
	}

	return false;
}

//void Title::StatisticTitle()
//{
//	if (m_owner->GetMyself())
//	{
//		//激活称号
//		StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), eStatic_Achievement, eStaMinor_Achievement_ActiTitle, 1);
//	}
//}
//设置称号
void Title::SetActiveTitleID(int value)
{
	ChangeAttrbuteBy(false,m_owner->getAttManage()->getValue<int>(eTitleID));
	m_owner->getAttManage()->setValue(eTitleID,value);
	ChangeAttrbuteBy(true,m_owner->getAttManage()->getValue<int>(eTitleID));

	calculateFight();

	if(value != 0)
	{
		if (m_owner->GetMyself())
		{
			PlayerUseTitle tempArgs(m_owner->GetMyself(),value);
			m_owner->FireEvent(PLAYER_USE_TITLE,tempArgs);
		}
	}

	vector<int> vecType;
	vecType.push_back(eTitleID);
	vecType.push_back(eFightPower);
	m_owner->SynCharAttribute(vecType);

	//还得同步给附近玩家
	vector<int> typeBro;
	typeBro.push_back(eTitleID);
	m_owner->BroadcastCharAttribute(typeBro);

	m_owner->SetDataFlag(eBaseInfo);
}
//熟悉变更
void Title::ChangeAttrbuteBy(bool isAdd,int titleID)
{
	vector<int> vec;

	sTitleEntry * info = AchievementLoader::GetInstance()->GetTitleEntry(titleID);

	if(!info)
		return;

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttTitle);

	m_owner->addAtt(eCharPhyAttackMax,info->phyack,isAdd,tempValueType);
	m_owner->addAtt(eCharPhyDefence,info->phydee,isAdd,tempValueType);
	m_owner->addAtt(eCharHPMax,info->hp,isAdd,tempValueType);

	vec.push_back(eCharPhyAttackMax);
	vec.push_back(eCharPhyDefence);
	vec.push_back(eCharHPMax);
	vec.push_back(eCharHP);

	m_owner->SynCharAttribute(vec);

}

bool Title::IsHaveTitle(int titleId)
{
	TitleMap::iterator titleIt = m_titleMap.find(titleId);

	if(titleIt != m_titleMap.end())
	{
		 if(titleIt->second.titleStatus != eTitleNotActive)
			 return true;
	}

	return false;
}

//更新
void Title::UpDate(ServerReturn::ServerDoubleIntArray &ret,bool isInit)
{
	//cout<<endl<<"更新排行榜称号"<<endl;
	TitleMap::iterator it =		m_titleMap.begin();
	for(;it != m_titleMap.end() ; it++)
	{
		int titleid = it->second.titleId;
		if(titleid >= GameParaLoader::GetInstance()->getRankingFightBegin()
			&& titleid <= GameParaLoader::GetInstance()->getRankingFightBegin()+9)
		{
			it->second.titleStatus = eTitleNotActive;
			it->second.titleActive = 0;
		}
	}

	for(int i = 0 ; i < ret.rets_size() ;i++)
	{
		int titleid = ret.rets(i);
		TitleMap::iterator its =		m_titleMap.find(titleid);
		if(its != m_titleMap.end())
		{
			its->second.titleStatus = eTitleHasGet;
			its->second.titleActive = 0;
		}
	}

	int activeTitleID = m_owner->getAttManage()->getValue<int>(eTitleID);
	TitleMap::iterator itt =		m_titleMap.find(activeTitleID);
	if(itt != m_titleMap.end())
	{
		if(itt->second.titleStatus == eTitleNotActive)
		{
			if(!isInit)
				SetActiveTitleID(0);
			else
				m_owner->getAttManage()->setValue(eTitleID,0);
		}
	}
	else
	{
		if(0 != activeTitleID)
		{
			if(!isInit)
				SetActiveTitleID(0);
			else
				m_owner->getAttManage()->setValue(eTitleID,0);
		}
	}

}
