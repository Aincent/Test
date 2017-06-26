/*
 * QuestCondition.cpp
 *
 *  Created on: 2014年10月20日
 *      Author: root
 */

#include "QuestCondition.h"
#include "Quest.h"
#include "../Object/Player/Player.h"
#include "../Object/Monster/Monster.h"
#include "Log/Logger.h"
#include "../ServerEventDefine.h"
#include "Timer/TimerInterface.h"
#include <math.h>
#include "../QuickMessage.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageCommonRet.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../LucklyTurntable/LucklyTurntableLoader.h"
#include "../MoneyShop/MoneyShopMgr.h"
#include "../EscortEx/EscortMgr.h"
#include "../Object//GirlWar/GirlWarManager.h"
#include "../FileLoader/MallLoader.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../Horse/HorseLoader/HorseLoader.h"
#include "../Horse/HorseManager.h"
#include "../DanLu/DanLuLoader.h"
#include "../DanLu/DanLuMgr.h"
#include "../CJobLucky/CJobLuckyMgr.h"
#include "../Container/ArticleEquipMgr.h"
#include "../MapInstance/HeroInfo.h"
#include "../MapInstance/InstanceMapLoader.h"


#define MONSTER_BELONG_LENGTH 20

QuestCondition::QuestCondition(Quest& quest, uint index)
	: mQuest(quest)
	, mIndex(index)
	, mValue(0)
	, mIsActive(false)
	, mCfg(quest.getEntry()->SubmitConds[index])
{
}

QuestCondition::~QuestCondition()
{
}

void QuestCondition::setValue(int val)
{
	if(mIsActive)
	{
		if(val != mValue)
		{
			mValue = val;

			mQuest.updateCondition();
		}
	}
}

void QuestCondition::setActive(bool setting)
{
	if(mIsActive != setting)
	{
		mIsActive = setting;
		if(mIsActive)
			onActive();
		else
			onDisactive();
	}
}


QuestCondition* QuestCondition::createCondition(Quest& quest, uint index)
{
	if(index >= MAX_CONDITION_NUM)
		return 0;

	const Entry_Quest* entry = quest.getEntry();
	const CondCfg& cfg = entry->SubmitConds[index];

	QuestCondition* qc = 0;

	switch (cfg.type)
	{
	case QuestCondType_Upgrade:
		qc = new QC_PlayerUpgrade(quest, index);
		break;
	case QuestCondType_KillMonster:
		qc = new QC_KillMonster(quest, index);
		break;
	case QuestCondType_RemoveItem:
		qc = new QC_RemoveItem(quest, index);
		break;
	case QuestCondType_Mining:
		qc = new QC_MiningItem(quest, index);
		break;
	case QuestCondType_DirectFinish:
		qc = new DirectFinish(quest, index);
		break;
	case QuestCondType_NpcFreMonster:
		qc = new QC_KillNpcMonster(quest,index);
		break;
	case QuestCondType_BuyMallItem:
		qc = new QC_BuyMallItem(quest,index);
		break;
	case QuestCondType_Channel:
		qc = new QC_Channel(quest,index);
		break;
	case QuestCondType_Strong:
		qc = new QC_StrongEquip(quest, index);
		break;
	case QuestCondType_Smelt:
		qc = new QC_SmeltEquip(quest, index);
		break;
	case QuestCondType_Slot:
			qc = new QC_Recoin(quest, index);
			break;
	case QuestCondType_Compose:
			qc = new QC_Compose(quest, index);
			break;
	case QuestCondType_House:
			qc = new QC_Horse(quest, index);
			break;
	case QuestCondType_MagicWeapon:
			qc = new QC_MagicWeapon(quest, index);
			break;
	case QuestCondType_Danlu:
			qc = new QC_DanLu(quest, index);
			break;
	case QuestCondType_Check:
			qc = new QC_Check(quest, index);
			break;
	case QuestCondType_EnterScene:
		qc = new QC_EnterScene(quest, index);
		break;
	case QuestCondType_OwnItem:
		qc = new QC_OwnItem(quest,index);
		break;
	case QuestCondType_SendMail:
		qc = new QC_SendMail(quest,index);
		break;
	case QuestCondType_UseItem:
		qc = new QC_UseItem(quest,index);
		break;
	case QuestCondType_DailySign:        		// 每日签到
		qc = new QC_DailySign(quest, index);
		break;
	case QuestCondType_SevenPackage:			// 七日礼包
		qc = new QC_SevenPackage(quest, index);
		break;
	case QuestCondType_Turntable:				// 转盘
		qc = new QC_Turntable(quest, index);
		break;
	case QuestCondType_OnLineGift:				// 在线礼包领取
		qc = new QC_OnLineGift(quest, index);
		break;
	case QuestCondType_LevelGift:				// 等级礼包领取
		qc = new QC_LevelGift(quest, index);
		break;
	case QuestCondType_UseTitle:				// 称号使用
		qc = new QC_UseTitle(quest, index);
		break;
	case QuestCondType_SkillUpgrade:			// 技能等级升级
		qc = new QC_SkillUpgrade(quest, index);
		break;
	case QuestCondType_EquipSHop:        		// 装备商店购买物品
		qc = new QC_EquipSHop(quest, index);
		break;
	case QuestCondType_EquipInherit:     		// 装备继承
		qc = new QC_EquipInherit(quest, index);
		break;
	case QuestCondType_Activity:				// 活跃度领取
		qc = new QC_Activity(quest, index);
		break;
	case QuestCondType_CampContribute:			// 阵营捐献
		qc = new QC_CampContribute(quest, index);
		break;
	case QuestCondType_MagicUpgrade:			// 神兵升级到指定等级
		qc = new QC_MagicUpgrade(quest, index);
		break;
	case QuestCondType_MagicRefresh:			// 神兵刷新技能
		qc = new QC_MagicRefresh(quest, index);
		break;
	case QuestCondType_CreateGroup:				// 队伍创建
		qc = new QC_CreateGroup(quest, index);
	break;
	case QuestCondType_EquipUpgrade:			// 锻造中升级装备
		qc = new QC_EquipUpgrade(quest, index);
		break;
	case QuestCondType_MoneyShop:				// 钱庄免费领取
		qc = new QC_MoneyShop(quest, index);
		break;
	case QuestCondType_HuangGang:				// 参加皇纲
		qc = new QC_Escort(quest, index);
		break;
	case QuestCondType_GirlWarLingering:		// 战姬缠绵
		qc = new QC_GirlWarLingering(quest, index);
		break;
	case QuestCondType_GirlWarUpgrade:			// 战姬升级
		qc = new QC_GirlWarUpgrade(quest, index);
		break;
	case QuestCondType_GirlWarFit:				// 战姬合体
		qc = new QC_GirlWarFit(quest, index);
		break;
	case QuestCondType_JobLucky:				// 官印装备
		qc = new QC_JobLucky(quest, index);
		break;
	case QuestCondType_WorldBoss:				// 世界BOSS
		qc = new QC_AttackWorldBoss(quest, index);
		break;
	case QuestCondType_ParametersArena:			// 竞技场
		qc = new QC_ParametersArena(quest, index);
		break;
	case QuestCondType_House_Update:
		qc = new QC_Horse_Update(quest, index);
		break;
	case QuestCondType_Danlu_Update:
		qc = new QC_Danlu_Update(quest, index);
		break;
	case QuestCondType_Strong_LV:
		qc = new QC_Strong_Lv(quest, index);
		break;
	case QuestCondType_Arena_Success:
		qc = new QC_Arena_Success(quest, index);
		break;
	case QuestCondType_Slot_Num:
		qc = new QC_Stone_Num(quest, index);
		break;
	case QuestCondType_Smelt_Num:
		qc = new QC_Smelt_Num(quest, index);
		break;
	case QuestCondType_EquipLv_Num:
		qc = new QC_EquipLv_Num(quest, index);
		break;
	case QuestCondType_JobLucky_Lv:
		qc = new QC_JobLucky_Num(quest, index);
		break;
	case QuestCondType_MoneyMap_Success:
		qc = new QC_MoneyMap_Success(quest, index);
		break;
	case QuestCondType_RoleTower:
		qc = new QC_Hero_Tower(quest, index);
		break;
	default:
		LOG_ERROR(FILEINFO, "No implement condition type: %d ", cfg.type);
		qc = new DirectFinish(quest, index); // TODO 未实现的任务条件，暂时用 直接完成 来替换
		break;
	}

	return qc;
}

//////////////////////////////////////////////////////////////////////////
QC_PlayerUpgrade::QC_PlayerUpgrade(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_PlayerUpgrade::~QC_PlayerUpgrade()
{
	onDisactive();
}

void QC_PlayerUpgrade::onActive()
{
	Player* player = mQuest.getOwner();
	if (player->getAttManage()->getValue<int>(eCharLv) >= mCfg.par2)			//等级任务取数量
	{
		setValue(1);
	}
	else
	{
		m_stopEvent = RegEvent(player, PLAYER_LEVEL_UP_EVENT_TYPE, this, &QC_PlayerUpgrade::onEvent);//玩家升级
	}
}

void QC_PlayerUpgrade::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_PlayerUpgrade::onEvent(const EventArgs & e)
{
		Player* player = mQuest.getOwner();
		if(player->getAttManage()->getValue<int>(eCharLv) >= mCfg.par2)
		{
			setValue(1);
		}

		return true;
}

//////////////////////////////////////////////////////////////////////////
QC_KillMonster::QC_KillMonster(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_KillMonster::~QC_KillMonster()
{
	onDisactive();
}


void QC_KillMonster::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(),MONSTER_KILLED_EVENT_BELONG, this, &QC_KillMonster::onEvent);
}

void QC_KillMonster::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_KillMonster::onEvent(const EventArgs & e)
{
	const MonsterBeKillBelongArgs& data = static_cast<const MonsterBeKillBelongArgs& >(e);

	if(data.m_myself->GetType() != eMonster)
	{
		return true;
	}

	if(data.m_myself->GetMapID() != mQuest.getOwner()->GetMapID())
	{
		return true;
	}

	int tempX = data.m_myself->GetXNewPos() - mQuest.getOwner()->GetXNewPos();
	int tempY = data.m_myself->GetYNewPos() - mQuest.getOwner()->GetYNewPos();

	if(tempX * tempX + tempY * tempY >= MONSTER_BELONG_LENGTH * MONSTER_BELONG_LENGTH)
	{
		return true;
	}

	const Entry_Quest *info = mQuest.getEntry();
	if(info->m_nCollItemID == eMonsterDeaBelongOnwer && data.m_Type != eMonsterDeaBelongOnwer)
	{
		return true;
	}

	Smart_Ptr<Monster> mon = data.m_myself;

	int leftrate = info->m_nDropItemRate - (rand()%100);
	if(leftrate > 0 && mon->GetID() == info->m_nTaskTarget)
	{
		setValue(getValue() + 1);
	}

	return true;
}

bool QC_KillMonster::isFinished() const
{
	//假如是帮派任务和根据星级算出
	const Entry_Quest *info = mQuest.getEntry();
	int targetNum = 0;
	if(info)
	{
		targetNum = mQuest.getAllNum();

		if(targetNum <= 0)
			targetNum = 1;
	}
	return (mValue >= targetNum);
}

void QC_KillMonster::setFinish()
{
	//假如是帮派任务和根据星级算出
	const Entry_Quest *info = mQuest.getEntry();
	int targetNum = 0;
	if(info)
	{
		targetNum = mQuest.getAllNum();

		if(targetNum <= 0)
			targetNum = 1;
	}
	setValue(targetNum);
}

//////////////////////////////////////////////////////////////////////////
QC_RemoveItem::QC_RemoveItem(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_RemoveItem::~QC_RemoveItem()
{
	onDisactive();
}

void QC_RemoveItem::onActive()
{
	// 订阅玩家
	m_stopEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &QC_RemoveItem::onEvent);
}

void QC_RemoveItem::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_RemoveItem::onEvent(const EventArgs & e)
{
	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	if(data->m_other->GetType() != ePlayer && data->m_other->GetType() !=  ePet)
		return true;

	Smart_Ptr<Player>  player;
	Smart_Ptr<GirlWar>  girlwar;

	if(data->m_other->GetType() == ePet)
	{
		girlwar = data->m_other;

		player  = girlwar->GetOwner();
	}
	else
	{
		player = data->m_other;
	}

	Player* owner = mQuest.getOwner();
	Smart_Ptr<Monster> mon = data->m_myself;
	const Entry_Quest *info = mQuest.getEntry();

	if(info && player && mon && owner &&  player->GetID() == owner->GetID())
	{
		if (mon->GetID() == info->m_nTaskTarget)
		{
			DWORD rate = rand()%100;
			int leftrate = info->m_nDropItemRate - rate;
			if(leftrate >= 0)
			{
				Smart_Ptr<ArticleBaseMgr> pack = player->GetContainer(ePackageType);
				if(pack)
				{
					//判断背包是否已满
					if(pack->IsBagEnought(info->m_nCollItemID,1))
					{
						/*通知  背包  已满  防止 一直  进行  此项操作*/
						owner->SendToPlayerResultCodeMessage(ePackageFull);
						return true;
					}

					vector<int> tempPosList;
					if(pack->AddItem(info->m_nCollItemID,1,tempPosList,1) != 0)
					{
						return true;
					}

					setValue(getValue() + 1);
				}
			}
		}
	}

	return true;
}

bool QC_RemoveItem::isFinished() const
{
	//假如是帮派任务和根据星级算出
	const Entry_Quest *info = mQuest.getEntry();
	int targetNum = 0;
	if(info)
	{
		targetNum = mQuest.getAllNum();

		if(targetNum <= 0)
			targetNum = 1;
	}
	return (mValue >= targetNum);
}

void QC_RemoveItem::setFinish()
{
	//假如是帮派任务和根据星级算出
	const Entry_Quest *info = mQuest.getEntry();
	int targetNum = 0;
	if(info)
	{
		targetNum = mQuest.getAllNum();

		if(targetNum <= 0)
			targetNum = 1;
	}
	setValue(targetNum);
}


//////////////////////////////////////////////////////////////////////////
QC_MiningItem::QC_MiningItem(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_MiningItem::~QC_MiningItem()
{
	onDisactive();
}

void QC_MiningItem::onActive()
{
	// 订阅玩家
	m_stopEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &QC_MiningItem::onEvent);
}

void QC_MiningItem::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_MiningItem::onEvent(const EventArgs & e)
{
	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	if(data->m_other->GetType() != ePlayer && data->m_other->GetType() !=  ePet)
		return true;

	Smart_Ptr<Player>  player;
	Smart_Ptr<GirlWar>  girlwar;

	if(data->m_other->GetType() == ePet)
	{
		girlwar = data->m_other;

		player  = girlwar->GetOwner();
	}
	else
	{
		player = data->m_other;
	}

	Player* owner = mQuest.getOwner();
	Smart_Ptr<Monster> mon = data->m_myself;
	const Entry_Quest *info = mQuest.getEntry();
	if(info && player && owner && player->GetID() == owner->GetID())
	{
		if (mon->GetID() == info->m_nTaskTarget)
		{
			DWORD rate = rand()%100;
			int leftrate = info->m_nDropItemRate - rate;
			if(leftrate >= 0)
			{
				Smart_Ptr<ArticleBaseMgr> pack = player->GetContainer(ePackageType);
				if(pack)
				{
					//判断背包是否已满
					if(pack->IsBagEnought(info->m_nCollItemID,1))
					{
						/*通知  背包  已满  防止 一直  进行  此项操作*/
						owner->SendToPlayerResultCodeMessage(ePackageFull);
						return true;
					}

					vector<int> tempPosList;
					if(pack->AddItem(info->m_nCollItemID,1,tempPosList,1) != 0)
					{
						return true;
					}

					if(getValue() < mCfg.par2)
						setValue(getValue() + 1);
				}
			}
		}
	}

	return true;
}

bool QC_MiningItem::isFinished() const
{
	//假如是帮派任务和根据星级算出
	const Entry_Quest *info = mQuest.getEntry();
	int targetNum = 0;
	if(info)
	{
		targetNum = mQuest.getAllNum();
		if(targetNum <= 0)
			targetNum = 1;
	}
	return (mValue >= targetNum);
}

void QC_MiningItem::setFinish()
{
	//假如是帮派任务和根据星级算出
	const Entry_Quest *info = mQuest.getEntry();
	int targetNum = 0;
	if(info)
	{
		targetNum = mQuest.getAllNum();

		if(targetNum <= 0)
			targetNum = 1;
	}
	setValue(targetNum);
}


//
////////////////////////////////////////////////////////////////////////////
QC_OwnItem::QC_OwnItem(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_OwnItem::~QC_OwnItem()
{
	onDisactive();
}

bool QC_OwnItem::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_OwnItem::setFinish()
{
	setValue(mCfg.par2);
}

void QC_OwnItem::onActive()
{
	Player *player = mQuest.getOwner();
	if(!player)
	{
		return;
	}

	// 订阅玩家获得物品
	m_stopEvent = RegEvent(player, PLAYER_BUYITEM_FORMSHOP, this, &QC_OwnItem::onEvent);

	return;
}

void QC_OwnItem::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_OwnItem::onEvent(const EventArgs & e)
{
	BuyItemEventArgs* data = (BuyItemEventArgs*)(&e);
	if(!data)
	{
		return true;
	}
	Smart_Ptr<Player>  player = data->m_pObj;
	Player* owner = mQuest.getOwner();
	const Entry_Quest *info = mQuest.getEntry();
	if(!info)
	{
		return true;
	}
	uint itemId = info->m_nCollItemID;

	if (player->GetID() == owner->GetID() && data->m_id == itemId)
	{
		Smart_Ptr<ArticleBaseMgr> pack = player->GetContainer(ePackageType);
		if(!pack)
		{
			return true;
		}

//		int num = pack->GetGoodsNum(data->m_id);

		setValue(getValue()+data->m_num);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
DirectFinish::DirectFinish(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

DirectFinish::~DirectFinish()
{

}

void DirectFinish::onActive()
{
	OnTimer();
}

void DirectFinish::OnTimer()
{
	mQuest.updateCondition();
}

////////////////////////////////////////////////////////////////////////////
QC_Compose::QC_Compose(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Compose::~QC_Compose()
{
	onDisactive();
}

void QC_Compose::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_SMELT_COMPOSE, this, &QC_Compose::onEvent);
}

void QC_Compose::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_Compose::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}


////////////////////////////////////////////////////////////////////////////
QC_StrongEquip::QC_StrongEquip(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_StrongEquip::~QC_StrongEquip()
{
	onDisactive();
}

void QC_StrongEquip::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_SMELT_STRONG, this, &QC_StrongEquip::onEvent);
}

void QC_StrongEquip::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_StrongEquip::isFinished() const
{
	return (mValue >= mCfg.par1);
}

void QC_StrongEquip::setFinish()
{
	setValue(mCfg.par2);
}

bool QC_StrongEquip::onEvent(const EventArgs & e)
{
	StrengthEquipArgs* data = (StrengthEquipArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	Player* owner = mQuest.getOwner();
	const Entry_Quest *info = mQuest.getEntry();
	if(info && player && player->GetID() == owner->GetID())
	{
		setValue(getValue()+1);
	}

	return true;
}

///镶嵌
////////////////////////////////////////////////////////////////////////////
QC_Recoin::QC_Recoin(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Recoin::~QC_Recoin()
{
	onDisactive();
}

void QC_Recoin::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_SMELT_INSERT, this, &QC_Recoin::onEvent);
}

void QC_Recoin::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_Recoin::onEvent(const EventArgs & e)
{
	UseItemEventArgs* data = (UseItemEventArgs*)(&e);
	if(!data)
		return true;

	Smart_Ptr<Player>  player = data->m_pObj;
	const Entry_Quest *info = mQuest.getEntry();

	if(info && player && player->GetID() == mQuest.getOwner()->GetID())
	{
		//镶嵌任意成功 任务完成
		setValue(1);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
QC_SmeltEquip::QC_SmeltEquip(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_SmeltEquip::~QC_SmeltEquip()
{
	onDisactive();
}

void QC_SmeltEquip::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_SMELT_SOPHISTICATED, this, &QC_SmeltEquip::onEvent);
}

void QC_SmeltEquip::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_SmeltEquip::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

QC_Channel::QC_Channel(Quest& quest, uint index) :
		QuestCondition(quest, index) {
}

QC_Channel::~QC_Channel()
{
	onDisactive();
}

void QC_Channel::onActive()
{
	Player* owner = mQuest.getOwner();
	if(owner)
	{
		if(owner->GetAcupoint() >= mCfg.par2)
		{
			setValue(mCfg.par2);
		}
		else
		{
			setValue(owner->GetAcupoint());
		}
	}
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_CHANNEL_UPDATE, this, &QC_Channel::onEvent);
}

void QC_Channel::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_Channel::onEvent(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
		if(!data)
		{
			return true;
		}

		Smart_Ptr<Player>  player = data->m_obj;
		Player* owner = mQuest.getOwner();
		const Entry_Quest *info = mQuest.getEntry();
		if(info && player && player->GetID() == owner->GetID())
		{
				setValue(getValue()+1);
		}

		return true;
}

bool QC_Channel::isFinished() const
{
	Player* owner = mQuest.getOwner();
	if(owner && owner->GetAcupoint() >= mCfg.par2)
	{
		return true;
	}
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////
QC_EnterScene::QC_EnterScene(Quest &quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_EnterScene::~QC_EnterScene()
{
	onDisactive();
}

void QC_EnterScene::onActive()
{
	if(mQuest.getOwner()->GetInstanceMgr()->GetLeftValidTimes(mCfg.par1) <= 0)
	{
		setValue(1);
		return ;
	}
//	m_stopEvent  = RegEvent(mQuest.getOwner(),PLAYER_ENTER_MAP_EVENT_TYPE, this, &QC_EnterScene::onEvent);//进入 场景
	m_stopEvent  = RegEvent(mQuest.getOwner(),PLAYER_PASS_INSTANCE, this, &QC_EnterScene::onEvent);//进入 场景 2016-12-16
	m_OtherEvent = RegEvent(mQuest.getOwner(),PLAYER_RAIDS_DUPLICATE, this, &QC_EnterScene::onOtherEvent);
}

void QC_EnterScene::onDisactive()
{
	if(m_stopEvent.IsValid())
		m_stopEvent.SetNull();

	if(m_OtherEvent.IsValid())
		m_OtherEvent.SetNull();

	return ;
}

bool QC_EnterScene::onEvent(const EventArgs & e)
{
	int mapid = GET_MAP_ID(mQuest.getOwner()->GetMapID());
	executeFun(mapid);

	return true;
}

bool QC_EnterScene::onOtherEvent(const EventArgs & e)
{
	const PlayerRaidsDuplicate& tempArgs = static_cast<const PlayerRaidsDuplicate& >(e);
	executeFun(tempArgs.m_MapID);

	return true;
}

void QC_EnterScene::executeFun(int mapid)
{
	if(mapid != mCfg.par1)
		return ;

	setValue(1);

	if(mQuest.getOwner()->GetMagicWeapon()->CalNowState() != e_MagicMgr_None)
		return ;

	return ;
}


//////////////////////////////////////////////////////////////////////////
QC_KillNpcMonster::QC_KillNpcMonster(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_KillNpcMonster::~QC_KillNpcMonster()
{
	onDisactive();
}


void QC_KillNpcMonster::onActive()
{
	// 订阅玩家
	m_stopEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &QC_KillNpcMonster::onEvent);
}

void QC_KillNpcMonster::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_KillNpcMonster::onEvent(const EventArgs & e)
{
	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
	if(!data)
		return true;

	if(data->m_other->GetType() != ePlayer &&
			data->m_other->GetType() !=  ePet)
	{
		return true;
	}

	Smart_Ptr<Player>  player = NULL;
	Smart_Ptr<GirlWar>  girlwar = NULL;

	if(data->m_other->GetType() == ePet)
	{
		girlwar = data->m_other;

		player  = girlwar->GetOwner();
	}
	else
	{
		player = data->m_other;
	}

	Player* owner = mQuest.getOwner();
	Smart_Ptr<Monster> mon = data->m_myself;
	const Entry_Quest *info = mQuest.getEntry();

	if(info && player && mon && owner && player->GetID() == owner->GetID())
	{
		if( mon->GetID() == info->m_nTaskTarget)
		{
			setValue(getValue() + 1);
		}
	}

	return true;
}

bool QC_KillNpcMonster::isFinished() const
{
	//假如是帮派任务和根据星级算出
	const Entry_Quest *info = mQuest.getEntry();
	int targetNum = 0;
	if(info)
	{
		targetNum = mQuest.getAllNum();

		if(targetNum <= 0)
			targetNum = 1;
	}

	return mValue >= targetNum;
}

void QC_KillNpcMonster::setFinish()
{
	const Entry_Quest *info = mQuest.getEntry();
	int targetNum = 0;
	if(info)
	{
		targetNum = mQuest.getAllNum();

		if(targetNum <= 0)
			targetNum = 1;
	}
	setValue(targetNum);
}


//////////////////////////////////////////////////////////////////////////
QC_BuyMallItem::QC_BuyMallItem(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_BuyMallItem::~QC_BuyMallItem()
{
	onDisactive();
}


void QC_BuyMallItem::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_BUYITEM_FROMMALL, this, &QC_BuyMallItem::onEvent);
}

void QC_BuyMallItem::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_BuyMallItem::onEvent(const EventArgs & e)
{
	BuyItemEventArgs* data = (BuyItemEventArgs*)(&e);
		if(!data)
		{
			return true;
		}

		Smart_Ptr<Player>  player = data->m_pObj;
		Player* owner = mQuest.getOwner();
		const Entry_Quest *info = mQuest.getEntry();
		if(info && player && owner && player->GetID() == owner->GetID())
		{
				if(data->m_id == info->m_nCollItemID)
				{
						setValue(getValue() + data->m_num);
				}
		}

		return true;
}

bool QC_BuyMallItem::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_BuyMallItem::setFinish()
{
	setValue(mCfg.par2);
}

//////////////////////////////////////////////////////////////////////////
QC_UseItem::QC_UseItem(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_UseItem::~QC_UseItem()
{
	onDisactive();
}


void QC_UseItem::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_ITEM_USEITEM, this, &QC_UseItem::onEvent);
}

void QC_UseItem::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_UseItem::onEvent(const EventArgs & e)
{
	UseItemEventArgs* data = (UseItemEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_pObj;
	Player* owner = mQuest.getOwner();
	const Entry_Quest *info = mQuest.getEntry();
	if(info && player && owner && player->GetID() == owner->GetID())
	{
		int xPos = (DWORD)player->GetXNewPos();
		int yPos = (DWORD)player->GetYNewPos();
		int itemX = info->m_nTaskXpos;
		int itemY = info->m_nTaskYpos;
		int diffx = abs(xPos-itemX);
		int diffy = abs(yPos-itemY);
		double diff = sqrt(diffx * diffx - diffy * diffy);
		if(GET_MAP_ID(player->GetMapID()) ==  info->m_nTaskMap && diff < GameParaLoader::GetInstance()->getCanFinishQuestDiff())
		{
			if(data->m_id == info->m_nTaskTarget)
			{
				setValue(getValue() + data->m_num);
			}
		}
	}

	return true;
}

bool QC_UseItem::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_UseItem::setFinish()
{
	setValue(mCfg.par2);
}

//////////////////////////////////////////////////////////////////////////
QC_Check::QC_Check(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Check::~QC_Check()
{
	onDisactive();
}


void QC_Check::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_POSITION_TOPOSITION, this, &QC_Check::onEvent);
}

void QC_Check::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_Check::onEvent(const EventArgs & e)
{
	UseItemEventArgs* data = (UseItemEventArgs*)(&e);
		if(!data)
		{
			return true;
		}

		Smart_Ptr<Player>  player = data->m_pObj;
		Player* owner = mQuest.getOwner();
		const Entry_Quest *info = mQuest.getEntry();
		if(info && player && owner && player->GetID() == owner->GetID())
		{
			int xPos = (DWORD)player->GetXNewPos();
			int yPos = (DWORD)player->GetYNewPos();
			int itemX = info->m_nTaskXpos;
			int itemY = info->m_nTaskYpos;
			int diffx = abs(xPos-itemX);
			int diffy = abs(yPos-itemY);
			double diff = sqrt(diffx * diffx - diffy * diffy);
			if(GET_MAP_ID(player->GetMapID()) ==  info->m_nTaskMap && diff < GameParaLoader::GetInstance()->getCanFinishQuestDiff())
			{
					setValue(1);
			}
		}

		return true;
}


////////////////////////////////////////////////////////////////////////////
QC_Horse::QC_Horse(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Horse::~QC_Horse()
{
	onDisactive();
}

void QC_Horse::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_HORSE_UPDATE, this, &QC_Horse::onEvent);
}

void QC_Horse::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_Horse::onEvent(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	Player* owner = mQuest.getOwner();
	const Entry_Quest *info = mQuest.getEntry();
	if(info && player && player->GetID() == owner->GetID())
	{
			setValue(1);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
QC_Horse_Update::QC_Horse_Update(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Horse_Update::~QC_Horse_Update()
{
	onDisactive();
}

void QC_Horse_Update::onActive()
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<HorseMgr> horse = owner->GetHorseManager();

	if(horse)
	{
		int id = horse->GetId();
		HorseStaticInfo * info = HorseLoader::GetInstance()->GetHorseInfoByID(id);
		if(info)
		{
			if(mCfg.par1 <= info->advancelv)
			{
				setValue(1);
				return;
			}
		}
	}

	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_HORSE_UPDATE, this, &QC_Horse_Update::onEvent);
}

void QC_Horse_Update::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_Horse_Update::onEvent(const EventArgs & e)
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<HorseMgr> horse = owner->GetHorseManager();
	if(horse)
	{
		int id = horse->GetId();
		HorseStaticInfo * info = HorseLoader::GetInstance()->GetHorseInfoByID(id);
		if(info)
		{
			if(mCfg.par1 <= info->advancelv)
			{
				setValue(1);
			}
		}
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////
QC_Danlu_Update::QC_Danlu_Update(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Danlu_Update::~QC_Danlu_Update()
{
	onDisactive();
}

void QC_Danlu_Update::onActive()
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<DanLuMgr> danlu = owner->GetDanLuManager();

	if(danlu)
	{
		int id = danlu->GetID();
		LianLuInfo * info = DanLuLoader::GetInstance()->GetLianLuPtr(id);
		if(info)
		{
			if(mCfg.par1 <= info->quality)
			{
				setValue(1);
				return;
			}
		}
	}

	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_MERIDIAN_UPDATE, this, &QC_Danlu_Update::onEvent);
}

void QC_Danlu_Update::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_Danlu_Update::onEvent(const EventArgs & e)
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<DanLuMgr> danlu = owner->GetDanLuManager();

	if(danlu)
	{
		int id = danlu->GetID();
		LianLuInfo * info = DanLuLoader::GetInstance()->GetLianLuPtr(id);
		if(info)
		{
			if(mCfg.par1 <= info->quality)
			{
				setValue(1);
			}
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
QC_Strong_Lv::QC_Strong_Lv(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Strong_Lv::~QC_Strong_Lv()
{
	onDisactive();
}

void QC_Strong_Lv::onActive()
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<ArticleEquipMgr> equipCntr = owner->GetContainer(eEquipQuickType);
	if ((bool)equipCntr)
	{
//		DWORD count = equipCntr->GetEquipCountStrength(mCfg.par1);
//		if(count >= (DWORD)mCfg.par2)
//		{
//			setValue(mCfg.par2);
//			return;
//		}
//		else
//		{
//			mValue = count;
//		}
	}

	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_EQUIP_STRONG, this, &QC_Strong_Lv::onEvent);
	m_changeEvent = RegEvent(mQuest.getOwner(), PLAYER_WEAR_EQUIP, this, &QC_Strong_Lv::onEvent);
	m_offEvent = RegEvent(mQuest.getOwner(), PLAYER_TAKE_OFF_EQUIP, this, &QC_Strong_Lv::onEvent);
	m_updateEvent = RegEvent(mQuest.getOwner(), PLAYER_EQUIP_UPGRADE, this, &QC_Strong_Lv::onEvent);//升级有可能会掉星，因此需要监听
	m_inheritEvent = RegEvent(mQuest.getOwner(), PLAYER_EQUIP_INHERIT, this, &QC_Strong_Lv::onEvent);
}

void QC_Strong_Lv::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	if(m_changeEvent.IsValid())
	{
		m_changeEvent.SetNull();
	}

	if(m_offEvent.IsValid())
	{
		m_offEvent.SetNull();
	}
}

bool QC_Strong_Lv::onEvent(const EventArgs & e)
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<ArticleEquipMgr> equipCntr = owner->GetContainer(eEquipQuickType);
	if ((bool)equipCntr)
	{
//		DWORD count = equipCntr->GetEquipCountStrength(mCfg.par1);
//		if(equipCntr->GetEquipCountStrength(mCfg.par1) >= (DWORD)mCfg.par2)
//		{
//			setValue(mCfg.par2);
//		}
//		else
//		{
//			setValue(count);
//		}
	}

	return true;
}

bool QC_Strong_Lv::isFinished() const
{
	return (mValue >= mCfg.par2);
}


////////////////////////////////////////////////////////////////////////////
QC_Arena_Success::QC_Arena_Success(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Arena_Success::~QC_Arena_Success()
{
	onDisactive();
}

void QC_Arena_Success::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_ARENA_OVER, this, &QC_Arena_Success::onEvent);
}

void QC_Arena_Success::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_Arena_Success::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_Arena_Success::setFinish()
{
	setValue(mCfg.par2);
}

bool QC_Arena_Success::onEvent(const EventArgs & e)
{
	ArenaOverArgs* data = (ArenaOverArgs*)(&e);
	if(!data)
	{
		return true;
	}

	if(data->m_result)
	{
		setValue(getValue() + 1);
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////
QC_Stone_Num::QC_Stone_Num(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Stone_Num::~QC_Stone_Num()
{
	onDisactive();
}

void QC_Stone_Num::onActive()
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<ArticleEquipMgr> equipCntr = owner->GetContainer(eEquipQuickType);
	if ((bool)equipCntr)
	{
//		DWORD count = equipCntr->GetStoneCount(mCfg.par1);
//		if(count >= (DWORD)mCfg.par2)
//		{
//			setValue(mCfg.par2);
//			return;
//		}
//		else
//		{
//			mValue = count;
//		}
	}

	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_SMELT_INSERT, this, &QC_Stone_Num::onEvent);
	m_updateEvent = RegEvent(mQuest.getOwner(), PLAYER_SMELT_COMPOSE, this, &QC_Stone_Num::onEvent);
	m_deleteEvent = RegEvent(mQuest.getOwner(), PLAYER_TAKE_OFF_STONE, this, &QC_Stone_Num::onEvent);
}

bool QC_Stone_Num::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_Stone_Num::setFinish()
{
	setValue(mCfg.par2);
}

void QC_Stone_Num::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	if(m_updateEvent.IsValid())
	{
		m_updateEvent.SetNull();
	}

	if(m_deleteEvent.IsValid())
	{
		m_deleteEvent.SetNull();
	}
}

bool QC_Stone_Num::onEvent(const EventArgs & e)
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<ArticleEquipMgr> equipCntr = owner->GetContainer(eEquipQuickType);
	if ((bool)equipCntr)
	{
//		DWORD count = equipCntr->GetStoneCount(mCfg.par1);
//		if(count >= (DWORD)mCfg.par2)
//		{
//			setValue(mCfg.par2);
//		}
//		else
//		{
//			setValue(count);
//		}
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////
QC_Smelt_Num::QC_Smelt_Num(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_Smelt_Num::~QC_Smelt_Num()
{
	onDisactive();
}

void QC_Smelt_Num::onActive()
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<ArticleEquipMgr> equipCntr = owner->GetContainer(eEquipQuickType);
	if ((bool)equipCntr)
	{
//		DWORD count = equipCntr->GetRefineCount(mCfg.par1);
//		if(count >= (DWORD)mCfg.par2)
//		{
//			setValue(mCfg.par2);
//			return;
//		}
//		else
//		{
//			mValue = count;
//		}
	}

	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_SMELT_SMELT, this, &QC_Smelt_Num::onEvent);
}

bool QC_Smelt_Num::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_Smelt_Num::setFinish()
{
	setValue(mCfg.par2);
}

void QC_Smelt_Num::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_Smelt_Num::onEvent(const EventArgs & e)
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<ArticleEquipMgr> equipCntr = owner->GetContainer(eEquipQuickType);
	if ((bool)equipCntr)
	{
//		DWORD count = equipCntr->GetRefineCount(mCfg.par1);
//		if(count >= (DWORD)mCfg.par2)
//		{
//			setValue(mCfg.par2);
//		}
//		else
//		{
//			setValue(count);
//		}
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////
QC_MagicWeapon::QC_MagicWeapon(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_MagicWeapon::~QC_MagicWeapon()
{
	onDisactive();
}

void QC_MagicWeapon::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_WEAPON_MAGICWEAPON, this, &QC_MagicWeapon::onEvent);
}

void QC_MagicWeapon::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_MagicWeapon::onEvent(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
		if(!data)
		{
			return true;
		}

		Smart_Ptr<Player>  player = data->m_obj;
		Player* owner = mQuest.getOwner();
		const Entry_Quest *info = mQuest.getEntry();
		if(info && player && player->GetID() == owner->GetID())
		{
				setValue(1);
		}

		return true;
}

////////////////////////////////////////////////////////////////////////////
QC_DanLu::QC_DanLu(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_DanLu::~QC_DanLu()
{
	onDisactive();
}

void QC_DanLu::onActive()
{
	// 订阅玩家
	m_stopEvent = RegEvent(mQuest.getOwner(), PLAYER_MERIDIAN_UPDATE, this, &QC_DanLu::onEvent);
}

void QC_DanLu::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_DanLu::onEvent(const EventArgs & e)
{
	OneParmaEventArgs* data = (OneParmaEventArgs*)(&e);
		if(!data)
		{
			return true;
		}

		Smart_Ptr<Player>  player = data->m_obj;
		Player* owner = mQuest.getOwner();
		const Entry_Quest *info = mQuest.getEntry();
		if(info && player && player->GetID() == owner->GetID())
		{
				setValue(1);
		}

		return true;
}


//
////////////////////////////////////////////////////////////////////////////
QC_SendMail::QC_SendMail(Quest& quest, uint index)
	: QuestCondition(quest, index)
{
}

QC_SendMail::~QC_SendMail()
{
	onDisactive();
}

bool QC_SendMail::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_SendMail::setFinish()
{
	setValue(mCfg.par2);
}

void QC_SendMail::onActive()
{
	// 检测背包中该物品的数量
	const Entry_Quest *info = mQuest.getEntry();
	if(!info)
	{
		return;
	}
	uint itemId = info->m_nTaskTarget;
	Player *player = mQuest.getOwner();
	Smart_Ptr<ArticleBaseMgr> pack = player->GetContainer(ePackageType);
	if(!pack)
	{
		return;
	}

	int num = pack->GetGoodsNum(itemId);

	setValue(num);


	// 订阅玩家获得物品
	m_stopEvent = RegEvent(player, PLAYER_PACKET_ADDITEM, this, &QC_SendMail::onEvent);

	if (isFinished())
		return;

}

void QC_SendMail::onDisactive()
{
	if(m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}
}

bool QC_SendMail::onEvent(const EventArgs & e)
{
	BuyItemEventArgs* data = (BuyItemEventArgs*)(&e);
	if(!data)
	{
		return true;
	}
	Smart_Ptr<Player>  player = data->m_pObj;
	Player* owner = mQuest.getOwner();
	const Entry_Quest *info = mQuest.getEntry();
	if(!info)
	{
		return true;
	}
	uint itemId = info->m_nTaskTarget;

	if (player->GetID() == owner->GetID() && data->m_id == itemId)
	{
		Smart_Ptr<ArticleBaseMgr> pack = player->GetContainer(ePackageType);
		if(!pack)
		{
			return true;
		}

		int num = pack->GetGoodsNum(data->m_id);

		setValue(num);
	}

	return true;
}


/*************签到*************/
QC_DailySign::QC_DailySign(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_DailySign::onActive()
{
	if(mQuest.getOwner()->GetSignIn()->isTodayHaveSignin())
	{
		setValue(1);
		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_SIGNIN_EVENT_TYPE,this,&QC_DailySign::onEvent);

	return ;
}

//初始化数据
void QC_DailySign::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_SIGNIN_EVENT_TYPE,this,&QC_DailySign::onEvent);
	return ;
}

void QC_DailySign::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_DailySign::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************七日礼包*************/
QC_SevenPackage::QC_SevenPackage(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_SevenPackage::onActive()
{
	const Entry_Quest* tempInfo = mQuest.getEntry();
	if(tempInfo->SubmitConds[mIndex].par1 == 0)
	{
		if(mQuest.getOwner()->GetSignIn()->isSevenPackageHaveReceive())
		{
			setValue(1);
			return ;
		}
	}
	else
	{
		if(mQuest.getOwner()->GetSignIn()->isSomeSevenPackageHaveReceive(tempInfo->SubmitConds[mIndex].par1))
		{
			setValue(1);
			return ;
		}
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_SEVEN_PACKAGE,this,&QC_SevenPackage::onEvent);

	return ;
}

//初始化数据
void QC_SevenPackage::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_SEVEN_PACKAGE,this,&QC_SevenPackage::onEvent);
	return ;
}

void QC_SevenPackage::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_SevenPackage::onEvent(const EventArgs & e)
{
	const Entry_Quest* tempInfo = mQuest.getEntry();

	if(tempInfo->SubmitConds[mIndex].par1 == 0)
	{
		setValue(1);

		return true;
	}
	else
	{
		const PlayerSevenPackage& tempArgs = static_cast<const PlayerSevenPackage&>(e);
		if(tempInfo->SubmitConds[mIndex].par1 != tempArgs.m_ID)
			return true;

		setValue(1);

		return true;
	}
}

/*************转盘*************/
QC_Turntable::QC_Turntable(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_Turntable::onActive()
{
	//免费次数用则直接完成
	const ProLucklyTurntable* tempTurnData = LucklyTurntableLoader::GetInstance()->GetProLuckly();
	if(mQuest.getOwner()->GetCounterService()->GetNumById(FREEROLL_LUCKLYTURNTABLE) >= tempTurnData->freeRoll)
	{
		setValue(1);

		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_TURN_LUCKY_TABLE,this,&QC_Turntable::onEvent);

	return ;
}

//初始化数据
void QC_Turntable::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_TURN_LUCKY_TABLE,this,&QC_Turntable::onEvent);
	return ;
}

void QC_Turntable::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_Turntable::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************在线礼包领取*************/
QC_OnLineGift::QC_OnLineGift(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_OnLineGift::onActive()
{
	if(mQuest.getOwner()->getOnlineReward()->isHaveReceiveOneReward())
	{
		setValue(1);
		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_ONLINE_GIFT,this,&QC_OnLineGift::onEvent);

	return ;
}

//初始化数据
void QC_OnLineGift::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_ONLINE_GIFT,this,&QC_OnLineGift::onEvent);
	return ;
}

void QC_OnLineGift::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_OnLineGift::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************等级礼包领取*************/
QC_LevelGift::QC_LevelGift(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_LevelGift::onActive()
{
	const Entry_Quest* tempInfo = mQuest.getEntry();
	if(tempInfo->SubmitConds[mIndex].par1 == 0)
	{
		if(mQuest.getOwner()->GetLevelReward()->isAnyLevelRwardReceive())
		{
			setValue(1);
			return ;
		}
	}
	else
	{
		if(mQuest.getOwner()->GetLevelReward()->isSomeLevelRewardReceive(tempInfo->SubmitConds[mIndex].par1))
		{
			setValue(1);
			return ;
		}
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_LEVEL_GIFT,this,&QC_LevelGift::onEvent);

	return ;
}

//初始化数据
void QC_LevelGift::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_LEVEL_GIFT,this,&QC_LevelGift::onEvent);
	return ;
}

void QC_LevelGift::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_LevelGift::onEvent(const EventArgs & e)
{
	const PlayerLevelGift& tempArgs = static_cast<const PlayerLevelGift& >(e);

	const Entry_Quest* tempInfo = mQuest.getEntry();

	if(tempInfo->SubmitConds[mIndex].par1 != 0 && tempInfo->SubmitConds[mIndex].par1 != tempArgs.m_ID)
		return true;

	setValue(1);

	return true;
}

/*************称号使用*************/
QC_UseTitle::QC_UseTitle(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_UseTitle::onActive()
{
	if(mQuest.getOwner()->getAttManage()->getValue<int>(eTitleID) != 0)
	{
		setValue(1);
		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_USE_TITLE,this,&QC_UseTitle::onEvent);

	return ;
}

//初始化数据
void QC_UseTitle::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_USE_TITLE,this,&QC_UseTitle::onEvent);
	return ;
}

void QC_UseTitle::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_UseTitle::onEvent(const EventArgs & e)
{
	const PlayerUseTitle& tempArgs = static_cast<const PlayerUseTitle& >(e);
	//取消称号不算
	if(tempArgs.m_ID == 0)
		return true;

	setValue(1);

	return true;
}

/*************技能升级*************/
QC_SkillUpgrade::QC_SkillUpgrade(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_SkillUpgrade::onActive()
{
	const Entry_Quest* tempInfo = mQuest.getEntry();

	if(tempInfo->SubmitConds[mIndex].par1 != 0)
	{
		if(mQuest.getOwner()->GetSkillMgr()->isHaveSkillLevelMoreThan(tempInfo->SubmitConds[mIndex].par1))
		{
			setValue(1);
			return ;
		}
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_SKILL_UPGRADE,this,&QC_SkillUpgrade::onEvent);

	return ;
}

//初始化数据
void QC_SkillUpgrade::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_SKILL_UPGRADE,this,&QC_SkillUpgrade::onEvent);
	return ;
}

void QC_SkillUpgrade::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_SkillUpgrade::onEvent(const EventArgs & e)
{
	const SkillUpgradeArgs& tempArgs = static_cast<const SkillUpgradeArgs&>(e);
	const Entry_Quest* tempInfo      = mQuest.getEntry();

	if(tempInfo->SubmitConds[mIndex].par1 != 0 && tempArgs.m_nLv < tempInfo->SubmitConds[mIndex].par1)
	{
		return true;
	}

	setValue(1);

	return true;
}

/*************装备商店购买物品*************/
QC_EquipSHop::QC_EquipSHop(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_EquipSHop::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_BUYITEM_FROMMALL,this,&QC_EquipSHop::onEvent);

	return ;
}

void QC_EquipSHop::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_EquipSHop::onEvent(const EventArgs & e)
{
	const BuyItemEventArgs& tempArgs = static_cast<const BuyItemEventArgs&>(e);
	const Entry_Quest* tempQuest     = mQuest.getEntry();

	if((MallType)tempArgs.m_type != MallType_Credit)
		return true;

	if(tempQuest->SubmitConds[mIndex].par1 != 0 && tempQuest->SubmitConds[mIndex].par1 != (int)tempArgs.m_id)
		return true;

	setValue(1);

	return true;
}

/*************装备继承*************/
QC_EquipInherit::QC_EquipInherit(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_EquipInherit::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_EQUIP_INHERIT,this,&QC_EquipInherit::onEvent);

	return ;
}

void QC_EquipInherit::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_EquipInherit::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************活跃度领取*************/
QC_Activity::QC_Activity(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_Activity::onActive()
{
	if(mQuest.getOwner()->GetVitalityMgr()->isTodayHaveReceiveOneReward())
	{
		setValue(1);
		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_RECEIVE_ACTIVITY,this,&QC_Activity::onEvent);

	return ;
}

//初始化数据
void QC_Activity::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_RECEIVE_ACTIVITY,this,&QC_Activity::onEvent);
	return ;
}

void QC_Activity::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_Activity::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************阵营捐献*************/
QC_CampContribute::QC_CampContribute(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_CampContribute::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_CAMP_CONTRIBUTE,this,&QC_CampContribute::onEvent);

	return ;
}

void QC_CampContribute::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_CampContribute::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************神兵升级到指定等级*************/
QC_MagicUpgrade::QC_MagicUpgrade(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_MagicUpgrade::onActive()
{
	const Entry_Quest* info = mQuest.getEntry();
	if(mQuest.getOwner()->GetMagicWeapon()->getMagicOrder() >= info->m_nTaskTarget)
	{
		setValue(1);

		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_WEAPON_MAGICWEAPON,this,&QC_MagicUpgrade::onEvent);

	return ;
}

//初始化数据
void QC_MagicUpgrade::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_WEAPON_MAGICWEAPON,this,&QC_MagicUpgrade::onEvent);
	return ;
}

void QC_MagicUpgrade::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_MagicUpgrade::onEvent(const EventArgs & e)
{
	const Entry_Quest* info = mQuest.getEntry();
	if(mQuest.getOwner()->GetMagicWeapon()->getMagicOrder() < info->m_nTaskTarget)
		return true;

	setValue(1);

	return true;
}

/*************神兵刷新技能*************/
QC_MagicRefresh::QC_MagicRefresh(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_MagicRefresh::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_REFRESH_MAGICWEAPON_SKILL,this,&QC_MagicRefresh::onEvent);

	return ;
}

void QC_MagicRefresh::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_MagicRefresh::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************加入队伍*************/
QC_CreateGroup::QC_CreateGroup(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_CreateGroup::onActive()
{
	if(mQuest.getOwner()->IsHaveGroup())
	{
		setValue(1);

		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_ADDGROUP_EVENT_TYPE,this,&QC_CreateGroup::onEvent);

	return ;
}

//初始化数据
void QC_CreateGroup::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_ADDGROUP_EVENT_TYPE,this,&QC_CreateGroup::onEvent);
	return ;
}

void QC_CreateGroup::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_CreateGroup::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************装备升级*************/
QC_EquipUpgrade::QC_EquipUpgrade(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_EquipUpgrade::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_EQUIP_UPGRADE,this,&QC_EquipUpgrade::onEvent);

	return ;
}

void QC_EquipUpgrade::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_EquipUpgrade::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************钱庄领钱*************/
QC_MoneyShop::QC_MoneyShop(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_MoneyShop::onActive()
{
	//免费次数用完，则任务完成
	if(MoneyShopMgr::GetInstance()->MoneyShop_IsHaveFreeNum(mQuest.getOwner()->GetMyself()) == 0)
	{
		setValue(1);

		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_RECEIVE_MONEY_SHOP,this,&QC_MoneyShop::onEvent);

	return ;
}

//初始化数据
void QC_MoneyShop::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_RECEIVE_MONEY_SHOP,this,&QC_MoneyShop::onEvent);
	return ;
}

void QC_MoneyShop::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_MoneyShop::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************皇纲*************/
QC_Escort::QC_Escort(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_Escort::onActive()
{
	//护送次数用完，则任务完成
	if(mQuest.getOwner()->GetEscortMgr()->getEsortSurplusTime() == 0)
	{
		setValue(1);

		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_START_ESCORT,this,&QC_Escort::onEvent);

	return ;
}

//初始化数据
void QC_Escort::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_START_ESCORT,this,&QC_Escort::onEvent);
	return ;
}

void QC_Escort::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_Escort::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************战姬缠绵*************/
QC_GirlWarLingering::QC_GirlWarLingering(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_GirlWarLingering::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_GIRLWAR_LINGERING,this,&QC_GirlWarLingering::onEvent);

	return ;
}

void QC_GirlWarLingering::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_GirlWarLingering::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************战姬升级*************/
QC_GirlWarUpgrade::QC_GirlWarUpgrade(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_GirlWarUpgrade::onActive()
{
	const Entry_Quest* tempInfo   = mQuest.getEntry();

	//等级
	if(tempInfo->SubmitConds[mIndex].par1 != 0 && mQuest.getOwner()->GetGirlWarManager()->isMoreThanLevel(tempInfo->SubmitConds[mIndex].par1))
	{
		setValue(1);
		return ;
	}

	m_Event = RegEvent(mQuest.getOwner(),PLAYER_GIRLWAR_UPGRADE,this,&QC_GirlWarUpgrade::onEvent);

	return ;
}

//初始化数据
void QC_GirlWarUpgrade::initCondition()
{
	mIsActive = true;
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_GIRLWAR_UPGRADE,this,&QC_GirlWarUpgrade::onEvent);
	return ;
}

void QC_GirlWarUpgrade::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_GirlWarUpgrade::onEvent(const EventArgs & e)
{
	const PlayerGirlwarUpgrade& tempArgs = static_cast<const PlayerGirlwarUpgrade& >(e);
	const Entry_Quest* tempInfo          = mQuest.getEntry();

	if(tempInfo->SubmitConds[mIndex].par1 != 0 && tempArgs.m_obj->getAttManage()->getValue<int>(eCharLv) < tempInfo->SubmitConds[mIndex].par1)
	{
		return true;
	}

	setValue(1);

	return true;
}

/*************战姬合体*************/
QC_GirlWarFit::QC_GirlWarFit(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_GirlWarFit::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_GIRLWAR_FIT,this,&QC_GirlWarFit::onEvent);

	return ;
}

void QC_GirlWarFit::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_GirlWarFit::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************穿戴官印*************/
QC_JobLucky::QC_JobLucky(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_JobLucky::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_WARE_JOB_LUCKLY,this,&QC_JobLucky::onEvent);

	return ;
}

void QC_JobLucky::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_JobLucky::onEvent(const EventArgs & e)
{
	const PlayerJobLuckly& tempArgs = static_cast<const PlayerJobLuckly&>(e);

	const Entry_Quest* tempInfo = mQuest.getEntry();
	if(tempInfo->m_nTaskTarget != 0)
	{
		if(tempInfo->m_nTaskTarget > tempArgs.m_Quality)
			return true;
	}
	setValue(1);

	return true;
}

/*************攻击世界BOSS*************/
QC_AttackWorldBoss::QC_AttackWorldBoss(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_AttackWorldBoss::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_ATTACK_WORLD_BOSS,this,&QC_AttackWorldBoss::onEvent);

	return ;
}

void QC_AttackWorldBoss::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_AttackWorldBoss::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/*************参加竞技场*************/
QC_ParametersArena::QC_ParametersArena(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

void QC_ParametersArena::onActive()
{
	m_Event = RegEvent(mQuest.getOwner(),PLAYER_ENTER_MAP_EVENT_TYPE,this,&QC_ParametersArena::onEvent);

	return ;
}

void QC_ParametersArena::onDisactive()
{
	if (m_Event.IsValid())
	{
		m_Event.SetNull();
	}

	return ;
}

bool QC_ParametersArena::onEvent(const EventArgs & e)
{
	EMapType tempMapType = (EMapType)GET_MAP_TYPE(mQuest.getOwner()->GetMapID());
	if(ePVEMap != tempMapType)
		return true;

	setValue(getValue() + 1);

	return true;
}

bool QC_ParametersArena::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_ParametersArena::setFinish()
{
	setValue(mCfg.par2);
}

/**********************************************************/
QC_EquipLv_Num::QC_EquipLv_Num(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

QC_EquipLv_Num::~QC_EquipLv_Num()
{
	onDisactive();
}

void QC_EquipLv_Num::onActive()
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<ArticleEquipMgr> equipCntr = owner->GetContainer(eEquipQuickType);
	if ((bool)equipCntr)
	{
//		DWORD count = equipCntr->GetEquipLvCount(mCfg.par1, 0);
//		if(count >= (DWORD)mCfg.par2)
//		{
//			setValue(mCfg.par2);
//			return;
//		}
//		else
//		{
//			mValue = count;
//		}
	}

	m_stopEvent = RegEvent(mQuest.getOwner(),PLAYER_WEAR_EQUIP,this,&QC_EquipLv_Num::onEvent);
	m_updateEvent = RegEvent(mQuest.getOwner(),PLAYER_EQUIP_UPGRADE,this,&QC_EquipLv_Num::onEvent);
	m_offEvent = RegEvent(mQuest.getOwner(),PLAYER_TAKE_OFF_EQUIP,this,&QC_EquipLv_Num::onEvent);

	return ;
}

bool QC_EquipLv_Num::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_EquipLv_Num::setFinish()
{
	setValue(mCfg.par2);
}

void QC_EquipLv_Num::onDisactive()
{
	if (m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	if (m_updateEvent.IsValid())
	{
		m_updateEvent.SetNull();
	}

	if (m_offEvent.IsValid())
	{
		m_offEvent.SetNull();
	}

	return ;
}

bool QC_EquipLv_Num::onEvent(const EventArgs & e)
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<ArticleEquipMgr> equipCntr = owner->GetContainer(eEquipQuickType);
	if ((bool)equipCntr)
	{
//		DWORD count = equipCntr->GetEquipLvCount(mCfg.par1, 0);
//		if(count >= (DWORD)mCfg.par2)
//		{
//			setValue(mCfg.par2);
//		}
//		else
//		{
//			setValue(count);
//		}
	}

	return true;
}


/**********************************************************/
QC_JobLucky_Num::QC_JobLucky_Num(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

QC_JobLucky_Num::~QC_JobLucky_Num()
{
	onDisactive();
}

void QC_JobLucky_Num::onActive()
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<CJobLuckyMgr> cjob = owner->GetCJobLucky();
	if ((bool)cjob)
	{
		DWORD count = cjob->GetCjobCountByLv(mCfg.par1);
		if(count >= (DWORD)mCfg.par2)
		{
			setValue(mCfg.par2);
			return;
		}
		else
		{
			mValue = count;
		}
	}

	m_stopEvent = RegEvent(mQuest.getOwner(),PLAYER_WARE_JOB_LUCKLY,this,&QC_JobLucky_Num::onEvent);
	m_unwearEvent = RegEvent(mQuest.getOwner(),PLAYER_UNWARE_JOB_LUCKLY,this,&QC_JobLucky_Num::onEvent);

	return ;
}

bool QC_JobLucky_Num::isFinished() const
{
	return (mValue >= mCfg.par2);
}

void QC_JobLucky_Num::setFinish()
{
	setValue(mCfg.par2);
}

void QC_JobLucky_Num::onDisactive()
{
	if (m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	if (m_unwearEvent.IsValid())
	{
		m_unwearEvent.SetNull();
	}

	return ;
}

bool QC_JobLucky_Num::onEvent(const EventArgs & e)
{
	Player* owner = mQuest.getOwner();
	Smart_Ptr<CJobLuckyMgr> cjob = owner->GetCJobLucky();
	if ((bool)cjob)
	{
		DWORD count = cjob->GetCjobCountByLv(mCfg.par1);
		if(count >= (DWORD)mCfg.par2)
		{
			setValue(mCfg.par2);
		}
		else
		{
			setValue(count);
		}
	}

	return true;
}


/**********************************************************/
QC_MoneyMap_Success::QC_MoneyMap_Success(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

QC_MoneyMap_Success::~QC_MoneyMap_Success()
{
	onDisactive();
}

void QC_MoneyMap_Success::onActive()
{
	m_stopEvent = RegEvent(mQuest.getOwner(),PLAYER_MONEYMAP_SUCCESS,this,&QC_MoneyMap_Success::onEvent);

	return ;
}

void QC_MoneyMap_Success::onDisactive()
{
	if (m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	return ;
}

bool QC_MoneyMap_Success::onEvent(const EventArgs & e)
{
	setValue(1);

	return true;
}

/**********************************************************/
QC_Hero_Tower::QC_Hero_Tower(Quest& quest, uint index) : QuestCondition(quest, index)
{

}

QC_Hero_Tower::~QC_Hero_Tower()
{
	onDisactive();
}

void QC_Hero_Tower::onActive()
{
	m_stopEvent = RegEvent(mQuest.getOwner(),PLAYER_PASS_INSTANCE,this,&QC_Hero_Tower::onEvent);
	int tNum = mQuest.getOwner()->GetHeroMgr()->getLayer() - 1;
	if(tNum >= mQuest.getAllNum())
	{
		setValue(mQuest.getAllNum());
	}

	mValue = tNum;

	return ;
}

void QC_Hero_Tower::onDisactive()
{
	if (m_stopEvent.IsValid())
	{
		m_stopEvent.SetNull();
	}

	return ;
}

bool QC_Hero_Tower::onEvent(const EventArgs & e)
{
	const PlayerInstancePassEventArgs &tArgs = static_cast<const PlayerInstancePassEventArgs &>(e);
	const InstanceMapEntry *tInfo = InstanceLoader::GetInstance()->GetEntry(tArgs.m_mapId);
	if(tInfo == NULL)
	{
		return true;
	}
	if(tInfo->m_eType != eHeroInstance)
	{
		return true;
	}

	setValue(getValue() + 1);

	return true;
}

void QC_Hero_Tower::initCondition()
{
	mIsActive = true;
	m_stopEvent = RegEvent(mQuest.getOwner(),PLAYER_PASS_INSTANCE,this,&QC_Hero_Tower::onEvent);

	return ;
}

bool QC_Hero_Tower::isFinished() const
{
	const Entry_Quest *info = mQuest.getEntry();
	int targetNum = 0;
	if(info)
	{
		targetNum = mQuest.getAllNum();

		if(targetNum <= 0)
			targetNum = 1;
	}
	return (mValue >= targetNum);
}
