/*
 * MoneyCarbon.cpp
 *
 *  Created on: 2015年9月11日
 *      Author: root
 */
#include "MoneyCarbon.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../Map/DuplicateMgr.h"
#include "../ServerEventDefine.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../ServerManager.h"
#include "../Map/GameZone.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageStruct/MoneyInsatence/MoneyInsatence.pb.h"
#include "../MapCarbon/MoneyCarbon.h"
#include "../MapInstance/MoneyInstanceMgr.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Object/AI/AIBrain.h"
#include "../Mail/MailMessage.h"
#include "MessageStruct/CharLogin/Mail.pb.h"
#include "./FileLoader/InternationalLoader.h"
#include "./FileLoader/GameParaLoader.h"
#include "../Fighting/Fighting.h"


MoneyCarbon::MoneyCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):
CGameDuplicate(onlyid,charid,paramf,params)
{
	m_IsCountDown = false;
	m_IsCalCool = true;
	m_IsEnd = false;
	m_fitchFlag = eFitchState_no;
	m_curNode = 0;
	m_buyAttrTimes = 0;

	for(int i=0; i<eMoneyInstanceItemType_cool; ++i)
	{
		m_TowerAddAttrs[i] = 0;
	}

}

MoneyCarbon::~MoneyCarbon()
{
	if(m_monsterDeadEvent.IsValid())
		m_monsterDeadEvent.SetNull();

	if(m_NoticeTimer.IsValid())
		m_NoticeTimer.SetNull();

	if(m_FlushTimer.IsValid())
		m_FlushTimer.SetNull();


	if(m_CoolTimer.IsValid())
		m_CoolTimer.SetNull();

	if(m_playerLiveUpEvent.IsValid())
		m_playerLiveUpEvent.SetNull();
}

void MoneyCarbon::InitTowerAttr(Smart_Ptr<Player>& player)
{
	const TowerInfo * pTower = MoneyInstanceMgr::GetInstance()->getTowerInfo(1);

	int lv = player->getAttManage()->getValue<int>(eCharLv);

	int country = player->getAttManage()->getValue<int>(eCharCountry);

	std::map<int,Smart_Ptr<Monster> >::iterator it = m_TowerObjects.begin();
	for( ; it != m_TowerObjects.end(); ++it)
	{
		if(it->second.Get() == NULL)
		{
			continue;
		}

		it->second->getAttManage()->setValue(eCharCountry,country);

		if(pTower != NULL)
		 {
			 int attack=0, defene=0, hp=0;
			 MoneyInstanceMgr::GetInstance()->getTowerAttr(lv, pTower, attack, defene, hp);
			 it->second->getAttManage()->setValue(eCharHPMax,hp);
			 it->second->getAttManage()->setValue(eCharHP,hp);
			 it->second->getAttManage()->setValue(eCharPhyAttackMax,attack);
			 it->second->getAttManage()->setValue(eCharPhyDefence,defene);
		 }

	}
}

int MoneyCarbon::BuyAttr(Smart_Ptr<Player>& player)
{
	if( IsEnd() )
	{
		return eActivityOver;
	}

	const AttrBuyInfo* pInfo = MoneyInstanceMgr::GetInstance()->getAttrBuyInfo(m_buyAttrTimes+1);
	if(pInfo==NULL)
	{
		return eMoneyInstanceBuyLimit;
	}

	int money = player->getAttManage()->getValue<int>((ECharAttrType)(pInfo->moneyType));
	if(money < pInfo->cost)
	{
		return eMoneyNotEnough;
	}

	//扣钱
	std::vector<int> vecType;
	switch(pInfo->moneyType)
	{
			case eCharMoney:
			{
				player->ChangeMoney(pInfo->cost,false);
				vecType.push_back(eCharMoney);
				break;
			}
			case eCharGolden:
			{
				player->ChangeGolden(pInfo->cost,false);
				vecType.push_back(eCharGolden);
				break;
			}
			case eCharBindGolden:
			{
				player->ChangeBindGolden(pInfo->cost,false);
				vecType.push_back(eCharBindGolden);
				break;
			}
	}

	//加属性
	for(size_t i=0; i<pInfo->attrID.size(); ++i)
	{
		int old = 0;
		std::map<ECharAttrType,int64>::iterator it = m_attrs_add.find((ECharAttrType)pInfo->attrID[i]);
		if(it != m_attrs_add.end())
		{
			old = it->second;
		}

		int temp = player->getAttManage()->getValue<int>((ECharAttrType)pInfo->attrID[i]);
		if(temp >= old)
			temp -= old;

		int add = (int)((double)(temp)*(double)(pInfo->rate)*0.01);

		if(it != m_attrs_add.end())
		{
			it->second += add;
		}
		else
		{
			m_attrs_add[(ECharAttrType)pInfo->attrID[i]] = add;
		}

		player->getAttManage()->addValue((ECharAttrType)pInfo->attrID[i], add);
		vecType.push_back( pInfo->attrID[i] );
	}

	int tempResult = player->getFightingManage()->CalculateFighting(m_attrs_add);
	player->getFightingManage()->setFighting(eFightMoneyInstance,tempResult);
	vecType.push_back(eFightPower);

	if(vecType.size() > 0)
	{
		player->SynCharAttribute(vecType);
	}

	//更新购买次数
	m_buyAttrTimes++;

	//同步当前倍率
	std::vector<pair<int,int64> > vAttr;
	vAttr.push_back( make_pair(eMoneyInstanceAttr_rate,m_buyAttrTimes+1) );
	SyntoClient(player, vAttr);

	return 0;
}

int MoneyCarbon::BuyItem(Smart_Ptr<Player>& player, int type)
{
		if( IsEnd() )
		{
			return eActivityOver;
		}

		if(isAllTowerDead())
		{
			return eMoneyInstanceTowerDead;
		}

		//塔的信息,是否可以购买使用
		const TowerInfo* pInfo = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
		if(pInfo==NULL)
		{
			return eMoneyInstanceTowerBuyLimit;
		}

		switch(type)
		{
			case eMoneyInstanceItemType_defene:
			{
				if(pInfo->defeneUseNum <= m_TowerData.defeneUseNum)
					return eMoneyInstanceTowerBuyLimit;

				int code = IsMoneyEnough(player,pInfo->moneyType1,pInfo->defenePrice);
				if(code != 0)
					return code;

				ReduceMoney(player, pInfo->moneyType1, pInfo->defenePrice);
				break;
			}
			case eMoneyInstanceItemType_impose:
			{
				if(pInfo->attackUseNum <= m_TowerData.attackUseNum)
					return eMoneyInstanceTowerBuyLimit;

				int code = IsMoneyEnough(player,pInfo->moneyType2,pInfo->attackPrice);
				if(code != 0)
					return code;

				ReduceMoney(player, pInfo->moneyType2, pInfo->attackPrice);
				break;
			}
			case eMoneyInstanceItemType_cool:
			{
				if(pInfo->coolUseNum <= m_TowerData.coolUseNum)
					return eMoneyInstanceTowerBuyLimit;

				int code = IsMoneyEnough(player,pInfo->moneyType3,pInfo->coolPrice);
				if(code != 0)
						return code;

				ReduceMoney(player, pInfo->moneyType3, pInfo->coolPrice);
				break;
			}
			default:
				return eMoneyInstanceTowerBuyLimit;
		}

	//增加塔的属性
	ChangeTowerAttr(player, pInfo, type);

	return 0;
}

int MoneyCarbon::UseSkill(Smart_Ptr<Player>& player)
{
	if( IsEnd() )
	{
		return eActivityOver;
	}

	if(isAllTowerDead())
	{
		return eMoneyInstanceTowerDead;
	}

	if(m_TowerData.cool > 0)
	{
		return eMoneyInstanceTowerCDLimit;
	}

	const TowerInfo* pInfo = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
	if(pInfo==NULL)
	{
		return eMoneyInstanceTowerCDLimit;
	}
	m_TowerData.cool = pInfo->maxCool;

	//设置怪物死亡计算冷却度标识
	setDeadCoolFlag(false);

	const INTOBJECTMAP &ref = GetAllObjects();
	INTOBJECTMAP::const_iterator it = ref.begin();
	for(; it != ref.end(); ++it)
	{
		if(it->second.Get()==NULL || it->second->GetType()!=eMonster)
			continue;

		Smart_Ptr<Monster> mon = it->second;
		if(mon->IsDead())
			continue;

		if( player->getAttManage()->getValue<int>(eCharCountry) == mon->getAttManage()->getValue<int>(eCharCountry) )
			continue;

		CharBase::SynchObjectAttr attr;
		CharBase::CharAttChange * change = attr.mutable_attr();

		attr.set_key(mon->GetKey());
		change->add_type(eSimpleState);
		change->add_value(0);
		change->add_state(eAttackDead);

		if(attr.has_attr())
		{
			CSceneMgr::GetInstance()->SendSynchAttrChange(attr, mon->GetMapID(), mon->GetXNewPos(), mon->GetYNewPos());
		}

		Smart_Ptr<CreatureObj> obj = player;
		mon->Dead(obj);
	}

	//恢复设置怪物死亡计算冷却度标识
	setDeadCoolFlag(true);

	std::vector< pair<int,int> > data;
	data.push_back( make_pair(eTowerAttr_cool,m_TowerData.cool) );
	TowerChange(player, data);

	return 0;
}

int MoneyCarbon::Exit(Smart_Ptr<Player>& player)
{
	m_process = eAward;

	//开启定时器
	//if(m_KickTimer.IsValid())
	//	m_KickTimer.SetNull();

	//m_fitchFlag = eFitchState_yes;
	//m_KickTimer = TimerInterface::GetInstance()->RegTimePoint(&MoneyCarbon::onExit, this, 0, 0, 0, 10, 0);
	return 0;
}

int MoneyCarbon::Fitch(Smart_Ptr<Player>& player)
{
	if(m_fitchFlag != eFitchState_yes)
	{
		if(m_fitchFlag == eFitchState_no)
			return eMoneyInstanceLimit;

		if(m_fitchFlag == eFitchState_success)
			return eMoneyInstanceHaveGet;
	}

	int totalMoney = m_PlayerData.money;
	const TowerInfo* pInfo = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
	if(pInfo != NULL)
	{
		totalMoney += pInfo->prizeMoney;
	}
	player->ChangeMoney(totalMoney, true);

	std::vector<int> vecType;
	vecType.push_back(eCharMoney);
	player->SynCharAttribute(vecType);

	m_fitchFlag = eFitchState_success;

	return 0;
}

void MoneyCarbon::SendMail(Smart_Ptr<Player>& player)
{
		int totalMoney = m_PlayerData.money;
		const TowerInfo* pInfo = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
		if(pInfo != NULL)
		{
			totalMoney += pInfo->prizeMoney;
		}

		int itemId = GameParaLoader::GetInstance()->getExchangeMoneyItemId();
		int num = totalMoney;

		MailInfo::SpecialMailItemList tempSpecial;
		MailInfo::SpecialItem* pItem = tempSpecial.add_itemlist();
		pItem->set_itemid(itemId);
		pItem->set_num(num);

		const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();
		std::string strContent=InternationalLoader::GetInstance()->GetInterStr(config.mailID);
		MailMessageMgr::GetInstance()->SendMailByCharId(player->GetID(), strContent,"系统邮件", &tempSpecial);

}


int MoneyCarbon::IsMoneyEnough(Smart_Ptr<Player>& player,int type, int value)
{
	int money = player->getAttManage()->getValue<int>((ECharAttrType)type);
	if(money >= value)
		return 0;

	int code = eNotEnoghMoney;
	if( type == eCharGolden )
	{
		code = eNotEnogGolden;
	}
	else if( type == eCharBindGolden)
	{
		code = eNotEnogBindGolden;
	}

	return code;
}

void MoneyCarbon::ReduceMoney(Smart_Ptr<Player>& player,int type, int value)
{
			std::vector<int> vecType;
			switch(type)
			{
			case eCharMoney:
			{
				player->ChangeMoney(value,false);
				vecType.push_back(eCharMoney);
				break;
			}
			case eCharGolden:
			{
				player->ChangeGolden(value,false);
				vecType.push_back(eCharGolden);
				break;
			}
			case eCharBindGolden:
			{
				player->ChangeBindGolden(value,false);
				vecType.push_back(eCharBindGolden);
				break;
			}
			}

			if(vecType.size() > 0)
			{
				player->SynCharAttribute(vecType);
			}
}

void MoneyCarbon::ChangeTowerAttr(Smart_Ptr<Player>& player,const TowerInfo* pInfo, int type)
{
	int attack = 0, defene = 0, hp = 0;
	int playerLv = player->getAttManage()->getValue<int>(eCharLv);
	MoneyInstanceMgr::GetInstance()->getTowerAttr(playerLv, pInfo, attack, defene, hp);

	//更新静态信息
	int attrID = 0;
	int dx = 0;
	std::vector< pair<int,int> > vAttr;
	switch(type)
	{
			case eMoneyInstanceItemType_defene:
			{
				attrID = eCharPhyDefence;
				dx = defene * pInfo->defeneValue/100;
				m_TowerData.defene += dx;
				m_TowerData.defeneUseNum++;
				m_TowerAddAttrs[eMoneyInstanceItemType_defene] += dx;

				vAttr.push_back( make_pair(eTowerAttr_defeneCount,m_TowerData.defeneUseNum) );
				vAttr.push_back( make_pair(eTowerAttr_defene,m_TowerData.defene) );
				break;
			}
			case eMoneyInstanceItemType_impose:
			{
				attrID = eCharPhyAttackMax;
				dx = attack * pInfo->attackValue/100;
				m_TowerData.attack += dx;
				m_TowerData.attackUseNum++;
				m_TowerAddAttrs[eMoneyInstanceItemType_impose] += dx;

				vAttr.push_back( make_pair(eTowerAttr_attackCount,m_TowerData.attackUseNum) );
				vAttr.push_back( make_pair(eTowerAttr_attack,m_TowerData.attack) );
				break;
			}
			case eMoneyInstanceItemType_cool:
			{
				m_TowerData.cool -= pInfo->coolValue;
				if(m_TowerData.cool < 0)
						m_TowerData.cool = 0;

				m_TowerData.coolUseNum++;
				vAttr.push_back( make_pair(eTowerAttr_coolCount,m_TowerData.coolUseNum) );
				vAttr.push_back( make_pair(eTowerAttr_cool,m_TowerData.cool) );
				break;
			}
			default:
				return;
	}

	//塔属性改变
	if(vAttr.size() > 0)
	{
		TowerChange(player, vAttr);
	}

	//更新所有修罗塔怪物属性
	if(attrID==0)
	{
		return;
	}

	std::map<int,Smart_Ptr<Monster> >::iterator it = m_TowerObjects.begin();
	for(; it!=m_TowerObjects.end(); ++it)
	{
		if(it->second.Get()==NULL || it->second->IsDead())
			continue;

		if(attrID == eCharPhyAttackMax)
		{
			it->second->ChangePhyAttackMax(dx,true);
		}
		else if(attrID == eCharPhyDefence)
		{
			it->second->ChangePDefence(dx,true);
		}
	}
}

void MoneyCarbon::Earning(Smart_Ptr<Monster>& mon, Smart_Ptr<Player>& player)
{
	int type=0;
	std::vector< pair<int,int64> > data;

	if(mon->GetQuality() == eTowerBossMonster)
	{
		type = 1;
		m_PlayerData.bossMonsterNum++;
		//data.push_back( make_pair(eMoneyInstanceAttr_boss,m_PlayerData.bossMonsterNum) );
	}
	else
	{
		m_PlayerData.normalMonsterNum++;
		//data.push_back( make_pair(eMoneyInstanceAttr_monster,m_PlayerData.normalMonsterNum) );
	}

	int lv = player->getAttManage()->getValue<int>(eCharLv);
	int money = MoneyInstanceMgr::GetInstance()->getMosterMoney(type, lv);
	m_PlayerData.money += money;

	data.push_back( make_pair(eMoneyInstanceAttr_money,m_PlayerData.money) );
	SyntoClient(player, data);

}

void MoneyCarbon::UpdateTowerInfoByMonDead(Smart_Ptr<Player>& player,Smart_Ptr<Monster>& mon, Smart_Ptr<CreatureObj>& other)
{
	//塔全部死亡了
	if(m_TowerObjects.size() == 0)
	{
		return;
	}

	const TowerInfo* pInfo = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
	if(pInfo==NULL)
			return;

	int monType=0;
	int killType=0;
	if(mon->GetQuality()==eTowerBossMonster)
	{
		monType = 1;
	}

	if(other->GetType()==eMonster)
	{
		killType = 1;
	}

	std::vector< std::pair<int,int> > data;

	//积分计算升级
	int score = MoneyInstanceMgr::GetInstance()->getTowerScore(monType, killType);
	int coolValue = MoneyInstanceMgr::GetInstance()->getTowerCoolValue(monType, killType);
	int maxLv = MoneyInstanceMgr::GetInstance()->getTowerMaxLevel();

	m_TowerData.score += score;

	bool isCoolChange = false;
	if(m_TowerData.cool > 0 && IsDeadCalCool() )
	{
		m_TowerData.cool -= coolValue;
		if(m_TowerData.cool < 0)
		{
			m_TowerData.cool = 0;
		}
		isCoolChange = true;
	}

	if(isCoolChange)
	{
		data.push_back( make_pair(eTowerAttr_cool,m_TowerData.cool) );
	}

	if(m_TowerData.score >= pInfo->point && m_TowerData.lv < maxLv)
	{
		m_TowerData.lv +=1;
		m_TowerData.score = 0;
		m_TowerData.coolUseNum = 0;
		m_TowerData.attackUseNum = 0;
		m_TowerData.defeneUseNum = 0;

		data.push_back( make_pair(eTowerAttr_lv,m_TowerData.lv) );
		data.push_back( make_pair(eTowerAttr_defeneCount,0) );
		data.push_back( make_pair(eTowerAttr_attackCount,0) );
		data.push_back( make_pair(eTowerAttr_coolCount,0) );
	}


	const TowerInfo* pInfoNext = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
	if(pInfoNext != NULL)
	{
		if(pInfo != pInfoNext)
		{
			int playerLv = player->getAttManage()->getValue<int>(eCharLv);

			int oldAttack = 0, oldDefene = 0, oldHP = 0;
			int newAttack = 0, newDefene = 0, newHP = 0;

			MoneyInstanceMgr::GetInstance()->getTowerAttr(playerLv, pInfo, oldAttack, oldDefene, oldHP);
			MoneyInstanceMgr::GetInstance()->getTowerAttr(playerLv, pInfoNext, newAttack, newDefene, newHP);

			m_TowerData.attack = newAttack + m_TowerAddAttrs[eMoneyInstanceItemType_impose];
			m_TowerData.defene = newDefene + m_TowerAddAttrs[eMoneyInstanceItemType_defene];
			m_TowerData.hp = newHP;

			//改变所有怪物塔的属性
			TowerLiveUp(pInfo, pInfoNext, newHP-oldHP);

			data.push_back( make_pair(eTowerAttr_attack,m_TowerData.attack) );
			data.push_back( make_pair(eTowerAttr_defene,m_TowerData.defene) );
			data.push_back( make_pair(eTowerAttr_hp,m_TowerData.hp) );
			data.push_back( make_pair(eTowerAttr_maxCool,pInfo->maxCool) );
		}
	}

	TowerChange(player, data);

	std::vector< std::pair<int,int64> > playerData;
	if(pInfoNext != pInfo && pInfoNext != NULL)
	{
		playerData.push_back( make_pair(eMoneyInstanceAttr_maxScore,pInfoNext->point) );
	}

	//积分不超过才下发
	if(pInfoNext != NULL && m_TowerData.score <= pInfoNext->point)
	{
		playerData.push_back( make_pair(eMoneyInstanceAttr_score,m_TowerData.score) );
	}

	SyntoClient(player,playerData);

}

void MoneyCarbon::RemoveAttr(Smart_Ptr<Player>& player)
{
	std::vector<int> vecType;
	std::map<ECharAttrType,int64>::iterator it = m_attrs_add.begin();
	for(; it != m_attrs_add.end(); ++it)
	{
		player->getAttManage()->addValue(it->first,-it->second);

		vecType.push_back(it->first);
	}

	player->getFightingManage()->setFighting(eFightMoneyInstance,0);
	vecType.push_back(eFightPower);

	player->SynCharAttribute(vecType);

}

bool MoneyCarbon::isAllTowerDead()
{
	return m_TowerObjects.size() == 0;
}

void MoneyCarbon::TowerLiveUp(const TowerInfo* pInfo, const TowerInfo* pInfoNext, int addHP)
{
	if(pInfo==NULL || pInfoNext==NULL)
	{
		return;
	}

	std::map<int,Smart_Ptr<Monster> >::iterator it = m_TowerObjects.begin();
	for(; it != m_TowerObjects.end(); ++it)
	{
		if(it->second.Get()==NULL || it->second->IsDead())
			continue;

		it->second->getAttManage()->setValue(eCharPhyAttackMax,m_TowerData.attack);
		it->second->getAttManage()->setValue(eCharPhyDefence,m_TowerData.defene);

		int cur = it->second->getAttManage()->getValue<int>(eCharHP);
		if(addHP > 0)
		{
			cur += addHP;
			it->second->getAttManage()->setValue(eCharHP,cur);
			it->second->getAttManage()->setValue(eCharHPMax,m_TowerData.hp);

			CharBase::SynchObjectAttr attr;
			CharBase::CharAttChange * change = attr.mutable_attr();

			attr.set_key(it->second->GetKey());
			change->add_type(eCharHP);
			change->add_value(cur);
			change->add_state(0);

			change->add_type(eCharHPMax);
			change->add_value(m_TowerData.hp);
			change->add_state(0);

			if(attr.has_attr())
			{
					CSceneMgr::GetInstance()->SendSynchAttrChange(attr, it->second->GetMapID(),
									it->second->GetXNewPos(), it->second->GetYNewPos());
			}
		}
	}

}

//倒计时通知
void MoneyCarbon::NoticeEnd()
{
	ServerReturn::ServerRetInt toClient;
	toClient.set_ret(0);

	std::vector<Smart_Ptr<Player> > players;
	GetAllPlayers(players);
	for(size_t i=0; i<players.size(); ++i)
	{
		if(players[i].Get()==NULL)
			continue;

		players[i]->SendSynchCommon(&toClient,MSG_REQ_GS2C_MONEYINSTANCE_PRIZE);
	}

	clearAll();

}

void MoneyCarbon::NoticeCoutDown()
{
		if(m_IsCountDown)
		{
			return;
		}
		m_IsCountDown = true;

		MoneyInstancePackage::FlushNotice toClient;
		toClient.set_type(eNoticeType_CountDown);
		toClient.set_queue(0);
		toClient.set_time(10);

		std::vector<Smart_Ptr<Player> > players;
		GetAllPlayers(players);
		for(size_t i=0; i<players.size(); ++i)
		{
			if(players[i].Get()==NULL)
				continue;

			players[i]->SendSynchCommon(&toClient,MSG_REQ_GS2C_MONEYINSTANCE_NOTICE);
		}

		if(m_NoticeTimer.IsValid())
		{
			m_NoticeTimer.SetNull();
		}

}

void MoneyCarbon::clearAll()
{
	//停止所有定时器和事件
	if(m_monsterDeadEvent.IsValid())
			m_monsterDeadEvent.SetNull();

	if(m_NoticeTimer.IsValid())
			m_NoticeTimer.SetNull();

	if(m_FlushTimer.IsValid())
			m_FlushTimer.SetNull();

	if(m_CoolTimer.IsValid())
			m_CoolTimer.SetNull();

	if(m_playerLiveUpEvent.IsValid())
			m_playerLiveUpEvent.SetNull();

	//杀死所有怪物
	const INTOBJECTMAP ref = GetAllObjects();
	INTOBJECTMAP::const_iterator it = ref.begin();
	for(; it != ref.end(); ++it)
	{
			if(it->second.Get()==NULL || it->second->GetType()!=eMonster)
				continue;

			Smart_Ptr<Monster> mon = it->second;
			if(mon->IsDead())
				continue;

			if(eTowerMonster == mon->GetQuality())
				continue;

			mon->SetDead(NULL);
	}

}

void MoneyCarbon::InitToClient(Smart_Ptr<Player>& player)
{
	//玩家收益属性
	std::vector< pair<int,int64> > data;
	const TowerInfo* pInfo = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
	if(pInfo!=NULL)
	{
			data.push_back( make_pair(eMoneyInstanceAttr_score,0));
			data.push_back( make_pair(eMoneyInstanceAttr_maxScore,pInfo->point));
	}

	data.push_back( make_pair(eMoneyInstanceAttr_money,0));
	data.push_back( make_pair(eMoneyInstanceAttr_rate,1));


	int64 left = 0;
	InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(GetSceneID()));
	if(pInstance!=NULL && pInstance->m_nLimitTime)
	{
		left = pInstance->m_nLimitTime;
	}

	data.push_back( make_pair(eMoneyInstanceAttr_time,left));

	SyntoClient(player, data);

	//修罗塔初始属性
	InitTowerData(player);

}

void MoneyCarbon::SyntoClient(Smart_Ptr<Player>& player, const std::vector< pair<int,int64> >& data)
{
	MoneyInstancePackage::DuplicateAttrAlter toClient;
	for(size_t i=0; i<data.size(); ++i)
	{
			MoneyInstancePackage::DuplicateAttr* pItem = toClient.add_attrlist();
			pItem->set_id(data[i].first);
			pItem->set_value(data[i].second);
	}
	player->SendSynchCommon(&toClient, MSG_REQ_GS2C_MONEYINSTANCE_ATTR);
}

void MoneyCarbon::InitTowerData(Smart_Ptr<Player>& player)
{
		m_TowerData.lv = 1;
		const TowerInfo* pInfo = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
		if(pInfo==NULL)
			return;

		int playerLv = player->getAttManage()->getValue<int>(eCharLv);
		int attack = 0, defene = 0, HP = 0;
		MoneyInstanceMgr::GetInstance()->getTowerAttr(playerLv, pInfo, attack, defene, HP);

		m_TowerData.cool = pInfo->maxCool;
		m_TowerData.attack = attack;
		m_TowerData.defene = defene;
		m_TowerData.hp = HP;

		std::vector< pair<int,int> > data;
		data.push_back( make_pair(eTowerAttr_lv,m_TowerData.lv) );
		data.push_back( make_pair(eTowerAttr_cool,m_TowerData.cool) );
		data.push_back( make_pair(eTowerAttr_attack,m_TowerData.attack) );
		data.push_back( make_pair(eTowerAttr_defene,m_TowerData.defene) );
		data.push_back( make_pair(eTowerAttr_hp,m_TowerData.hp) );
		data.push_back( make_pair(eTowerAttr_maxCool,m_TowerData.cool) );
		data.push_back( make_pair(eTowerAttr_defeneCount,m_TowerData.defeneUseNum) );
		data.push_back( make_pair(eTowerAttr_attackCount,m_TowerData.attackUseNum) );
		data.push_back( make_pair(eTowerAttr_coolCount,m_TowerData.coolUseNum) );

		TowerChange(player, data);

		//
		if(m_CoolTimer.IsValid())
			m_CoolTimer.SetNull();

		m_CoolTimer =  TimerInterface::GetInstance()->RegRepeatedTime(&MoneyCarbon::onRecoveCool, this, 0, 0, 0, 0, 5000, 5000);

}

void MoneyCarbon::TowerChange(Smart_Ptr<Player>& player, const std::vector< std::pair<int,int> > &data)
{
		MoneyInstancePackage::TowerAttrAlter toClient;
		for(size_t i=0; i<data.size(); ++i)
		{
			MoneyInstancePackage::TowerAttr* pItem = toClient.add_attr();
			pItem->set_id(data[i].first);
			pItem->set_value(data[i].second);
		}
		player->SendSynchCommon(&toClient, MSG_REQ_GS2C_MONEYINSTANCE_TOWER_ATTR);
}

int MoneyCarbon::Init()
{
		int ret = CGameDuplicate::Init();
		return ret;
}

bool MoneyCarbon::Update()
{
			switch(m_process)
			{
			case eCreated:
				{
					if(CUtil::GetNowSecond() - m_createTime >= m_data->m_nLimitTime)
					{
						m_process = eOver;
						return false;
					}

					break;
				}
			case ePlayerWork:
				{
					int64 nowTime = CUtil::GetNowSecond();

					if(GetPlayerCount() <= 0)
					{
						Count();
					}

					//副本时间到，可以进入领取状态
					if(nowTime - m_createTime >= m_data->m_nLimitTime)
					{
						m_process = eChangeMapWait;
						if(m_fitchFlag != eFitchState_success)
							m_fitchFlag = eFitchState_yes;

						NoticeEnd();

						Smart_Ptr<Player> player;
						PlayerMessage::GetInstance()->GetPlayerByCharid(m_owner,player);
						if(player)
						{
							OneParmaEventArgs tArgs(player);
							player->FireEvent(PLAYER_MONEYMAP_SUCCESS, tArgs);
						}
					}

					//10秒倒计时提前通知
					if(nowTime - m_createTime >= m_data->m_nLimitTime - 10*1000)
					{
						NoticeCoutDown();
					}

					break;
				}
			case eWinFight:
				{
					Win();
					break;
				}
			case eFailed:
				{
					Failed();
					break;
				}
			case eAward:
				{
					Award();
					break;
				}
			case eDuplicateChangeMap:
				{
					if(GetPlayerCount() <= 0)
					{
						CountEx();
						return true;
					}

					const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();
					int64 nowTime = CUtil::GetNowSecond();
					if(nowTime - m_awardTime >= config.exitTime)
					{
							ForceAllPlayerChangeMap();
							m_process = eOver;
							return true;
					}

					if(nowTime - m_createTime >= m_data->m_nLimitTime)
					{
							m_process = eChangeMapWait;
					}
					break;
				}
			case eChangeMapWait:
				{
					int64 nowTime = CUtil::GetNowSecond();
					if(nowTime - m_createTime >= m_data->m_nLimitTime + 10 * 1000)
					{
						ForceAllPlayerChangeMap();
						m_process = eOver;
					}

					if(GetPlayerCount() <= 0)
					{
						m_process = eOver;
					}
					break;
				}
			case eOver:
				{
					return false;
				}
			default :
				{
					break;
				}
			}

			return true;
}

int MoneyCarbon::Join(Smart_Ptr<Object> & obj)
{
		if(!JoinHandle(obj))
				return -1;

		int ret=CGameZone::Join(obj);
		if(ret!=0)
			return ret;

		if(obj->GetType()==eMonster)
		{
			 Smart_Ptr<Monster> mon = obj;
			 if(mon->GetQuality() == eTowerMonster)
			  {
				 m_TowerObjects[mon->GetKey()] = mon;
			  }
		}

		if(obj->GetType()==ePlayer)
		{
			Smart_Ptr<Player> player = obj;

			//扣除次数及金钱
			if(m_process == eCreated)
			{
				m_process = ePlayerWork;
				player->GetInstanceMgr()->EnterMoneyInstance();
			}

			//推送信息玩家副本信息
			InitToClient(player);

			const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();

			if(m_NoticeTimer.IsValid())
					m_NoticeTimer.SetNull();
			m_NoticeTimer = TimerInterface::GetInstance()->RegRepeatedTime(&MoneyCarbon::onNotice, this, 0, 0, 0, 0, config.countDown*1000, config.noticeTime*1000);

			if(!m_monsterDeadEvent.IsValid())
			{
				m_monsterDeadEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &MoneyCarbon::HandleMonsterDeadEvent);
			}

			if(!m_playerDeadEvent.IsValid())
			{
				m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &MoneyCarbon::HandlePlayerDeadEvent);
			}

			m_OwnerObj = player;
			m_playerLiveUpEvent = RegEvent(m_OwnerObj.Get(), PLAYER_LEVEL_UP_EVENT_TYPE, this, &MoneyCarbon::HandlePlayerLiveUp);

		}

		return ret;
}

bool MoneyCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
		if(obj->GetType() == ePlayer)
		{
			Smart_Ptr<Player> player = obj;

			//初始化塔的阵营
			InitTowerAttr(player);

			PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
			player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);
		}

		if(obj->GetType() == eMonster)
		{

		}

		return true;
}

int MoneyCarbon::Move(Smart_Ptr<Object> &obj)
{
	return CGameZone::Move(obj);
}

void MoneyCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
		LeaveHandle(obj);
		CGameZone::Leave(obj, isDel, delFromMap);
}

void MoneyCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
{
		if(obj->GetType() == ePlayer)
		{
				Smart_Ptr<Player> player = obj;
				if(player->getAttManage()->getValue<int>(eCharHP) <= 0)
				{
					CharBattle::RevivalData req;
					//增加地图id和坐标，重置死亡状态，解决客户端显示“使用复活单”的提示bug，是因为玩家回城复活，那么会发两次消息给客户端
					player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
					player->UnsetCreatureState(ERS_Dead);
					player->SetDataFlag(eBaseInfo);

					//如果不传地图Id就表示原地复活
					int64 mapid = 0;
					int xpos = 0;
					int ypos = 0;

					map<int64,OldMapInfo>::iterator itMap = m_oldMapPoint.find(player->GetID());
					if(itMap != m_oldMapPoint.end())
					{
							mapid = itMap->second.mapid;
							xpos = itMap->second.xpos;
							ypos = itMap->second.ypos;
					}
					else
					{
							MapStaticInfo *sMapInfo;
							int64 sceneid = 0;

							sMapInfo = CMapConfig::GetInstance()->GetFistMapInfo();
							if(sMapInfo == 0)
									return;

							sceneid = CSceneMgr::GetInstance()->GetSceneIDAboutPlayerIsLoginOn(sMapInfo->mapid);
							if(sceneid <= 0)
							{
								mapid = CREATE_MAP(eStaticMap, 1, 1, sMapInfo->mapid, 1);
							}
							else
							{
								mapid = sceneid;
							}

							xpos = sMapInfo->reliveXPos;
							ypos = sMapInfo->reliveYPos;
					}

					OldMapInfo * pInfo = GetOldMapPoint(player->GetID());
					if(!pInfo || CSceneMgr::GetInstance()->IsHaveScene(pInfo->mapid))
					{
						req.set_mapid(GET_MAP_ID(mapid));
						req.set_xpos(xpos);
						req.set_ypos(ypos);
					}

					req.set_key(player->GetKey());
					req.set_hp(player->getAttManage()->getValue<int>(eCharHP));
					req.set_mp(player->getAttManage()->getValue<int>(eCharMP));

					player->SendSynchMessage(&req, MSG_SIM_GT2C_REVIVAL);
				}

				//移出属性
				RemoveAttr(player);

				//如果没有领取奖励，发送邮件
				if(eFitchState_success != m_fitchFlag)
				{
					SendMail(player);
				}

				//发送排行数据到世界服
				Smart_Ptr<SvrItem> worldSvr;
				ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
				if(worldSvr.Get()!=NULL)
				{
					int totalMoney = m_PlayerData.money;
					const TowerInfo* pInfo = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
					if(pInfo != NULL)
					{
							totalMoney += pInfo->prizeMoney;
					}
					MoneyInstancePackage::MoneyInstanceResult msg;
					msg.set_id(player->GetID());
					msg.set_money(totalMoney);
					msg.set_name(player->GetName());
					Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GS2WS_MONEYINSTANCE_RESULT, &msg, worldSvr.Get(), SimpleMessage);
					Message_Facade::Send(messRet);
				}
		}

}

void MoneyCarbon::LeaveExame(Smart_Ptr<Player> & player)
{

}

bool MoneyCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	if(m_process >= eDuplicateChangeMap)
			return false;

	if(player->GetID() != m_owner)
			return false;

	return true;
}

void MoneyCarbon::Win()
{
		m_isSuccess = true;
		m_process = eAward;
}

void MoneyCarbon::Over()
{
		ForceAllPlayerChangeMap();
		m_process = eOver;
}

void MoneyCarbon::Count()
{
		int64 nowTime = CUtil::GetNowSecond();
		if(nowTime - m_leaveTime >= m_data->m_nDelTime)
		{
				m_process = eOver;
		}
}

void MoneyCarbon::CountEx()
{
		int64 nowTime = CUtil::GetNowSecond();
		if(nowTime - m_leaveTime >= m_data->m_nDelTime)
		{
			m_process = eOver;
		}
}

void MoneyCarbon::Failed()
{
		m_process = eAward;
}

void MoneyCarbon::Award()
{
		m_process = eDuplicateChangeMap;

		m_awardTime = CUtil::GetNowSecond();

		m_IsEnd = true;

		m_fitchFlag = eFitchState_yes;

		clearAll();
}

bool MoneyCarbon::HandleMonsterDeadEvent(const EventArgs & e)
{
		if(m_process != ePlayerWork)
				return true;

		if( IsEnd() )
			return true;

		TwoParmaEventArgs *tArgs = (TwoParmaEventArgs *)(&e);
		if(tArgs==NULL || tArgs->m_myself.Get()==NULL ||
				GetSceneID() != tArgs->m_myself->GetMapID()||tArgs->m_other.Get()==NULL)
				return true;

		Smart_Ptr<Monster> mon = tArgs->m_myself;
		//如果是修罗塔
		const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();
		if(mon->GetID() == config.towerID)
		{
			std::map<int,Smart_Ptr<Monster> >::iterator it = m_TowerObjects.find(mon->GetKey());
			if(it!=m_TowerObjects.end())
			{
				m_TowerObjects.erase(it);
			}
			return true;
		}

		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharid(m_owner,player);
		if(player.Get()==NULL)
			return true;

		//计算杀怪收益
		Earning(mon, player);

		//更新修罗塔信息(等级及技能冷却度)
		UpdateTowerInfoByMonDead(player, mon, tArgs->m_other);

	return true;
}

bool MoneyCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
		PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);
		if(tArgs == NULL)
			return true;

		if(tArgs->m_dead.Get()==NULL || tArgs->m_dead->GetType()!= ePlayer)
			return true;

		if(tArgs->m_dead->GetID() != m_owner)
			return true;

		//进入奖励领取状态
		m_process = eAward;

		return true;
}

bool MoneyCarbon::HandlePlayerLiveUp(const EventArgs & e)
{
	PlayerUpgradeEventArgs *tArgs = (PlayerUpgradeEventArgs *)(&e);
	if(tArgs==NULL)
	{
		return true;
	}

	if(m_OwnerObj.Get()==NULL)
	{
		return true;
	}

	if(isAllTowerDead())
	{
		return true;
	}

	int lv = m_OwnerObj->getAttManage()->getValue<int>(eCharLv);
	const TowerInfo * pTower = MoneyInstanceMgr::GetInstance()->getTowerInfo(m_TowerData.lv);
	if(pTower==NULL)
	{
		return true;
	}

	int oldAttack=0, oldDefene=0, oldHp=0;
	int newAttack=0, newDefene=0, newHp=0;
	MoneyInstanceMgr::GetInstance()->getTowerAttr(tArgs->m_nLevel, pTower, oldAttack, oldDefene, oldHp);
	MoneyInstanceMgr::GetInstance()->getTowerAttr(lv, pTower, newAttack, newDefene, newHp);

	//改变静态属性
	m_TowerData.attack = newAttack + m_TowerAddAttrs[eMoneyInstanceItemType_impose];
	m_TowerData.defene = newDefene + m_TowerAddAttrs[eMoneyInstanceItemType_defene];
	m_TowerData.hp = newHp;

	std::vector< pair<int,int> > data;
	data.push_back( make_pair(eTowerAttr_attack,m_TowerData.attack) );
	data.push_back( make_pair(eTowerAttr_defene,m_TowerData.defene) );
	data.push_back( make_pair(eTowerAttr_hp,m_TowerData.hp) );

	TowerChange(m_OwnerObj, data);

	//塔怪物属性更新
	std::map<int,Smart_Ptr<Monster> >::iterator it = m_TowerObjects.begin();
	for(; it != m_TowerObjects.end(); ++it)
	{
		if(it->second.Get()==NULL || it->second->IsDead())
				continue;

		it->second->getAttManage()->setValue(eCharPhyAttackMax,m_TowerData.attack);
		it->second->getAttManage()->setValue(eCharPhyDefence,m_TowerData.defene);

		int cur = it->second->getAttManage()->getValue<int>(eCharHP);
		int addHP = newHp - oldHp;
		if(addHP > 0)
		{
				cur += addHP;
				it->second->getAttManage()->setValue(eCharHP,cur);
				it->second->getAttManage()->setValue(eCharHPMax,m_TowerData.hp);
				CharBase::SynchObjectAttr attr;
				CharBase::CharAttChange * change = attr.mutable_attr();

				attr.set_key(it->second->GetKey());
				change->add_type(eCharHP);
				change->add_value(cur);
				change->add_state(0);

				change->add_type(eCharHPMax);
				change->add_value(m_TowerData.hp);
				change->add_state(0);

				if(attr.has_attr())
				{
						CSceneMgr::GetInstance()->SendSynchAttrChange(attr, it->second->GetMapID(),
											it->second->GetXNewPos(), it->second->GetYNewPos());
				}
		}
	}
	return true;
}

void MoneyCarbon::onNotice(void * arg)
{
	m_curNode++;

	//通知客户端刷新信息  提示类型  波次   时间
	const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();
	int type = MoneyInstanceMgr::GetInstance()->getNodeType(m_curNode);

	MoneyInstancePackage::FlushNotice toClient;
	toClient.set_type(type);
	toClient.set_queue(m_curNode);
	toClient.set_time(config.flushTime);
	std::vector<Smart_Ptr<Player> > players;
	GetAllPlayers(players);
	for(size_t i=0; i<players.size(); ++i)
	{
		if(players[i].Get()==NULL)
			continue;

		players[i]->SendSynchCommon(&toClient,MSG_REQ_GS2C_MONEYINSTANCE_NOTICE);
	}

	//开启刷新定时器
	if(m_FlushTimer.IsValid())
		m_FlushTimer.SetNull();

	m_FlushTimer = TimerInterface::GetInstance()->RegTimePoint(&MoneyCarbon::onFlushMonster, this, 0, 0, 0, config.flushTime, 0);
}

void MoneyCarbon::onFlushMonster(void * arg)
{
		const FlushNodeInfo* pNode = MoneyInstanceMgr::GetInstance()->getMonsterNodeInfo(m_curNode);
		if(pNode == NULL)
		{
			return;
		}

		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharid(m_owner,player);
		if(player.Get()==NULL)
			return ;

		int playerLv = player->getAttManage()->getValue<int>(eCharLv);

		MonsterInfo info;
		vector<Smart_Ptr<Monster> > vecMonster;

		MonsterLoader::GetInstance()->GetMonsterInfo(pNode->id, info);
		if(info.id == -1)
		{
				return;
		}

		int type = info.quality==eTowerBossMonster ? 1 : 0;
		int attack = MoneyInstanceMgr::GetInstance()->getMonsterAttack(playerLv,type,m_curNode);
		int hp = MoneyInstanceMgr::GetInstance()->getMonsterHP(playerLv,type,m_curNode);


		MoneyInstancePackage::FlushMonster toClient;
		toClient.set_queue(m_curNode);

		for(int i=0; i<pNode->num; ++i)
		{
				vector<Pos> vec = MoneyInstanceMgr::GetInstance()->GetPos(i,type);
				if(vec.size() < 2)
				{
					break;
				}

				Smart_Ptr<Object> obj = ObjectFactory::GetInstance()->AllocObject(eMonster);
				Smart_Ptr<Monster> mon = obj;

				mon->SetID(info.id);
				mon->SetName(info.name);
				mon->getAttManage()->setValue(eCharLv,playerLv);
				mon->SetQuality(info.quality);
				mon->SetFlushTime(info.flushTime);
				mon->SetAliveTime(info.aliveTime);
				mon->getAttManage()->setValue(eCharCountry,info.camp);
				mon->getAttManage()->setValue(eCharMoveSpeed,info.idleSpeed);
				mon->SetRunSpeed(info.moveSpeed);
//				mon->SetBaseAi(info.baseAI);
//				mon->SetBetterAi(info.betterAI);
				mon->SetFallID(info.fallID);
				mon->getAttManage()->setValue(eCharExp,info.exp);
				mon->getAttManage()->setValue(eCharPhyDefence,info.phyDefence);
				mon->getAttManage()->setValue(eCharHitRate,info.hitRate);
				mon->getAttManage()->setValue(eCharAvoidRate,info.avoidRate);
				mon->getAttManage()->setValue(eCrackDownRate,info.crackRate);
				mon->getAttManage()->setValue(eAvoidCrackRate,info.avoidCrack);
				mon->SetMPMax(info.maxMP);
				mon->SetCurMP(info.maxMP);

				mon->getAttManage()->setValue(eCharPhyAttackMax, attack);
				mon->SetHPMax(hp);
				mon->SetCurHP(hp);

				mon->SetGatherTime(info.gatherTime);	//采集时间
				mon->SetOwnerToBrain(mon);
				mon->SetMapID(m_sceneID);
				mon->SetBornTime();
				mon->SetOwnerName("");
				mon->GetBrain()->SetMonsterType(eSingleWayPos);
				mon->SetXPos(vec[0].xpos);
				mon->SetYPos(vec[0].ypos);
				mon->SetOriginalX(vec[1].xpos);
				mon->SetOriginalY(vec[1].ypos);

				Join(obj);

				vector<Sector *> moveVec;
				BaseAIInfo *aiInfo = 0;
				BetterAIInfo *betterAI = 0;

				aiInfo = AILoader::GetInstance()->GetBaseAIByid(info.baseAI);
				if(!aiInfo)
				{
					continue;
				}

				if(info.betterAI > 0)
				{
					betterAI = AILoader::GetInstance()->GetBetterAIByid(info.betterAI);
					if (!betterAI)
					{
						LOG_ERROR(FILEINFO, "monster[id=%d] better ai is error", info.id);

						continue;
					}
				}

				mon->SetStayTime(aiInfo->stayTime);
				mon->SetPhyAttackTime(aiInfo->atkTime);

				Sector * tSecf = 0;
				vector<Pos>::iterator itVec = vec.begin();
				for(; itVec!=vec.end(); ++itVec)
				{
					tSecf = GetSector((*itVec).xpos, (*itVec).ypos);
					if(tSecf)
					{
						moveVec.push_back(tSecf);
					}
				}

				mon->SetMoveSector(moveVec);

				Smart_Ptr<AIBrain> monBrain = mon->GetBrain();
				if(!monBrain)
					continue;

				monBrain->Init(aiInfo, betterAI);
				vecMonster.push_back(mon);
				if(info.quality == eWorldMonster)
				{
					CSceneMgr::GetInstance()->AddWorldBossToZone(mon->GetKey(), mon->GetMapID(), mon->GetID());
				}

				m_allObject[mon->GetKey()] = mon;

				MoneyInstancePackage::MonsterAttr* pItem = toClient.add_mons();
				pItem->set_key(mon->GetKey());
				pItem->set_id(info.id);
				pItem->set_hp(hp);
				pItem->set_level(playerLv);
		}

		INTVMPNSTERMAP::iterator itTemp = m_allMonster.find(info.id);
		if(itTemp != m_allMonster.end())
		{
			itTemp->second.insert( itTemp->second.end(), vecMonster.begin(), vecMonster.end() );
		}
		else
		{
			m_allMonster[info.id] = vecMonster;
		}

		player->SendSynchCommon(&toClient,MSG_REQ_GS2C_MONEYINSTANCE_FLUSH);
}

void MoneyCarbon::onExit(void* arg)
{
		m_process = eAward;
}

void MoneyCarbon::onRecoveCool(void* arg)
{
	if(m_TowerData.cool <= 0)
	{
		return;
	}

	//每5秒同步一次，所以乘以5
	const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();
	m_TowerData.cool -= config.coolRecove*5;
	if(m_TowerData.cool < 0)
	{
		m_TowerData.cool = 0;
	}

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharid(m_owner,player);
	if(player.Get()==NULL)
		return;

	std::vector< pair<int,int> > data;
	data.push_back( make_pair(eTowerAttr_cool,m_TowerData.cool) );
	TowerChange( player, data);
}







