/*
 * HeroInfo.cpp
 *
 *  Created on: 2016年5月18日
 *      Author: root
 */
#include "HeroInfo.h"
#include "../Object/Player/Player.h"
#include "HeroLoader.h"
#include "../Attribute/AttributeManage.h"
#include "../Fighting/Fighting.h"
#include "PVPDefine.h"
#include "FileLoader/InternationalLoader.h"
#include "../Mail/MailMessage.h"
#include "../Attribute/AttributeCreature.h"

HeroInfo::HeroInfo(Player* player):m_player(player)
{
	ReInit();
}

HeroInfo::~HeroInfo()
{
	Release();
}

void HeroInfo::ReInit()
{
	m_layer = 0;
}

void HeroInfo::Release()
{
	m_fitchs.clear();
}

void HeroInfo::InitHeroFromDB(const PlayerInfo::HeroInstanceData& info)
{
	m_fitchs.clear();

	m_layer = info.layer();
	if(m_layer <= 0)
	{
		m_layer = 1;
	}

	for(int i=0; i<info.fetchs_size(); ++i)
	{
		m_fitchs.push_back(info.fetchs(i));
	}

	m_player->SetInitPartDataFlag(eHeroInstanceInfo);
}

void HeroInfo::SetHeroInfo(PlayerInfo::HeroInstanceData* info)
{
	info->set_layer(m_layer);
	for(size_t i=0; i<m_fitchs.size(); ++i)
	{
		info->add_fetchs(m_fitchs[i]);
	}
}

void HeroInfo::onlineCalculateAtt()
{
	map<ECharAttrType,int64> tempAttList;

	GetDataAttList(tempAttList);

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttHero);

	m_player->calculateResultAddAtt(tempAttList,tempValueType);

	CalculateFight();
}

int HeroInfo::CalculateFight()
{
	map<ECharAttrType,int64> tempAttList;

	GetDataAttList(tempAttList);

	int tempResult = m_player->getFightingManage()->CalculateFighting(tempAttList);
	return m_player->getFightingManage()->setFighting(eFightHero,tempResult);
}


void HeroInfo::GetHeroData(HeroPackage::AckHeroInfo* data)
{
	data->set_layer(m_layer);
	for(size_t i=0; i<m_fitchs.size(); ++i)
	{
		data->add_fitchs(m_fitchs[i]);
	}
}

int HeroInfo::FitchBox(int layer)
{
	//是否打通
	if(layer >= m_layer)
	{
		return eNotPassInstance;
	}

	//是否领取
	std::vector<int>::iterator it = find(m_fitchs.begin(), m_fitchs.end(), layer);
	if(it != m_fitchs.end())
	{
		return eReceived;
	}

	//是否配置有奖励
	const HeroLayerInfo* pInfo = HeroLoader::GetInstance()->GetHeroLayerInfo(layer);
	if(pInfo==NULL || pInfo->exPrizes.size()==0)
	{
		return eInstanceInfoError;
	}

	//给玩家添加物品
	Smart_Ptr<ArticleBaseMgr> pPacket = m_player->GetContainer(ePackageType);
	if(!pPacket)
	{
		return ePlayerPackageFail;
	}

	vector<int> vec;
	vector<int64> goods;
	vector<DWORD> idList;
	vector<DWORD> numList;

	for(size_t i=0; i<pInfo->exPrizes.size(); ++i)
	{
		idList.push_back(pInfo->exPrizes[i].first);
		numList.push_back(pInfo->exPrizes[i].second);
	}

	vector<WORD> tempFlagList;
	vector<DWORD> tempStreghtList;
	tempFlagList.resize(idList.size(),1);
	tempStreghtList.resize(numList.size(),0);

	int res = pPacket->IsBagEnought(idList,numList,tempFlagList);
	if(res != 0)
	{
		return ePackageFull;
	}

	vector<int> tempPos;
	pPacket->AddItem(idList, numList,tempPos,tempFlagList,tempStreghtList);

	m_fitchs.push_back(layer);
	m_player->SetDataFlag(eHeroInstanceInfo);

	return 0;
}

int HeroInfo::CreateChecke(int mapid, int layer)
{
	InstanceMapEntry * entry = InstanceLoader::GetInstance()->GetEntry(mapid);
	if(!entry)
	{
		return eInstanceInfoError;
	}

	if(entry->m_eType != eHeroInstance)
	{
		return eInstanceInfoError;
	}

	if(layer > HeroLoader::GetInstance()->GetSize())
	{
		return eInstanceInfoError;
	}

	if(layer != m_layer)
	{
		return eInstanceInfoError;
	}

	const HeroLayerInfo* pInfo = HeroLoader::GetInstance()->GetHeroLayerInfo(layer);
	if(pInfo == NULL)
	{
		return eInstanceInfoError;
	}

	if(m_player->getAttManage()->getValue<int>(eCharLv) < pInfo->level)
	{
		return eLVNotMatch;
	}

	if(m_player->IsDead())
	{
		return eCreatureDead;
	}

	return 0;
}

void HeroInfo::PassLayer(int layer)
{
	const HeroLayerInfo* pInfo = HeroLoader::GetInstance()->GetHeroLayerInfo(layer);
	if(pInfo == NULL)
		return;

	m_layer = layer+1;
	m_player->SetDataFlag(eHeroInstanceInfo);

	std::map<ECharAttrType,int64> tempAttList;
	std::vector<int> vAttrs;
	vector<int> tempBroadcast;

	if(pInfo->attack > 0)	{
			tempAttList[eCharPhyAttackMax] = pInfo->attack;
		}
	if(pInfo->hp > 0){
			tempAttList[eCharHPMax] = pInfo->hp;
		}
	if(pInfo->defese > 0){
			tempAttList[eCharPhyDefence] = pInfo->defese;
		}
	if(pInfo->hit > 0)	{
			tempAttList[eCharHitRate] = pInfo->hit;
	}
	if(pInfo->avoid > 0)	{
			tempAttList[eCharAvoidRate] = pInfo->avoid;
	}
	if(pInfo->crackDown > 0)	{
			tempAttList[eCrackDownRate] = pInfo->crackDown;
	}
	if(pInfo->avoidCrack > 0)	{
			tempAttList[eAvoidCrackRate] = pInfo->avoidCrack;
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttHero);

	std::map<ECharAttrType,int64>::iterator tempIt = tempAttList.begin();
	for(;tempIt != tempAttList.end();++tempIt)
	{
		m_player->addAtt(tempIt->first,tempIt->second,true,tempValueType);
		vAttrs.push_back(tempIt->first);
		if(tempIt->first == eCharHPMax)
		{
			tempBroadcast.push_back(tempIt->first);
		}
	}

	if(tempBroadcast.size() > 0)
	{
		m_player->BroadcastCharAttribute(tempBroadcast);
	}

	if(vAttrs.size() > 0)
	{
		CalculateFight();
		vAttrs.push_back(eFightPower);
		m_player->SynCharAttribute(vAttrs);
	}

	int ret = Prize(layer);
	if(ret == ePackNoSpaceSendMail)
	{
		m_player->SendToPlayerResultCodeMessage(ePackNoSpaceSendMail);
	}

}

int HeroInfo::Prize(int layer)
{
	const HeroLayerInfo* pInfo = HeroLoader::GetInstance()->GetHeroLayerInfo(layer);
	if(pInfo == NULL)
			return -1;

	vector<int> vec;
	vector<int64> goods;
	vector<DWORD> idList;
	vector<DWORD> numList;

	for(size_t i=0; i<pInfo->prizes.size(); ++i)
	{
		idList.push_back(pInfo->prizes[i].first);
		numList.push_back(pInfo->prizes[i].second);
	}

	//给玩家添加物品
	Smart_Ptr<ArticleBaseMgr> pPacket = m_player->GetContainer(ePackageType);
	if(!pPacket)
	{
		return ePlayerPackageFail;
	}

	vector<WORD> tempFlagList;
	vector<DWORD> tempStreghtList;
	tempFlagList.resize(idList.size(),1);
	tempStreghtList.resize(numList.size(),0);

	int res = pPacket->IsBagEnought(idList,numList,tempFlagList);
	if(res == 0)
	{
		vector<int> tempPos;
		pPacket->AddItem(idList, numList,tempPos,tempFlagList,tempStreghtList);
	}
	else
	{
		std::vector<PropItem> vItems;
		for(size_t j = 0; j < idList.size(); j++)
		{
			vItems.push_back( PropItem(idList[j], numList[j]) );
		}

		string warning1 = InternationalLoader::GetInstance()->GetInterStr(8);
		MailMessageMgr::GetInstance()->SendMail(m_player->GetID(), vItems, warning1.c_str(), warning1.c_str());

		return ePackNoSpaceSendMail;
	}

	return 0;
}

void	HeroInfo::GetDataAttList(std::map<ECharAttrType,int64>& tempAttList)
{
	for(int i=1; i<m_layer; ++i)
	{
		const HeroLayerInfo *pInfo = HeroLoader::GetInstance()->GetHeroLayerInfo(i);
		if(pInfo==NULL)
			continue;

		if(pInfo->attack > 0)	{
			tempAttList[eCharPhyAttackMax] += pInfo->attack;
		}
		if(pInfo->hp > 0){
			tempAttList[eCharHPMax] += pInfo->hp;
		}
		if(pInfo->defese > 0){
			tempAttList[eCharPhyDefence] += pInfo->defese;
		}
		if(pInfo->hit > 0)	{
			tempAttList[eCharHitRate] += pInfo->hit;
		}
		if(pInfo->avoid > 0)	{
			tempAttList[eCharAvoidRate] += pInfo->avoid;
		}
		if(pInfo->crackDown > 0)	{
			tempAttList[eCrackDownRate] += pInfo->crackDown;
		}
		if(pInfo->avoidCrack > 0)	{
			tempAttList[eAvoidCrackRate] += pInfo->avoidCrack;
		}
	}
}


