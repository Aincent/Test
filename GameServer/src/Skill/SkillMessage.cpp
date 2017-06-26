/*
 * SkillMessage.cpp
 *
 *  Created on: 2014年1月14日
 *      Author: helh
 */

#include "SkillMessage.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "./MessageStruct/CharBattle/Battle.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/Clan/Clan.pb.h"
#include "../Map/SceneManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../ServerEventDefine.h"
#include "EventSet/EventSet.h"
#include "EventSet/EventArgs.h"
#include "FileLoader/GameParaLoader.h"
#include "Effect/Effect.h"
#include "../StatisticMgr/StatisticHandle.h"

SkillMessage * SkillMessage::m_instance = 0;


SkillMessage::SkillMessage()
{
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_SIM_GT2GM_ATTACK);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_UPGRADE_SKILL);
	DEF_MSG_QUICK_REQUEST_REG_FUN(SkillMessage, MSG_REQ_C2WS_UPGRADE_CLAN_SKILL);
	DEF_MSG_QUICK_REQUEST_REG_FUN(SkillMessage, MSG_REQ_C2WS_CLAN_SKILLS);
}

SkillMessage::~SkillMessage()
{

}

void SkillMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

void SkillMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_SIM_GT2GM_ATTACK);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_UPGRADE_SKILL);

	DEF_SWITCH_TRY_DISPATCH_END
}

void SkillMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{

}

DEF_MSG_REQUEST_DEFINE_FUN(SkillMessage, MSG_SIM_GT2GM_ATTACK)
{
	CharBattle::ClientReqAttack clientReq;
	ServerReturn::ServerRetInt ret;
	int len = 0;
	int res = -1;
	char *str = message->GetBuffer(len);
	Smart_Ptr<Player> player;

	clientReq.ParseFromArray(str, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(message->GetMessageTime(), player);
	if((bool)player)
	{
		vector<int> vec;
		vector<Smart_Ptr<CreatureObj> > objVec;

		res = player->GetSkillMgr()->IsFireSkill(clientReq.skillid());
		if(!res)
		{
			int targetkey = -1;
			int xpos = -1;
			int ypos = -1;

			if(clientReq.has_key())
			{
				targetkey = clientReq.key();
			}

			if(clientReq.xpos() > 0 || clientReq.xpos() > 0)
			{
				xpos = clientReq.xpos();
				ypos = clientReq.ypos();
			}

			res = player->GetSkillMgr()->Attack(xpos, ypos, targetkey, clientReq.skillid());
		}

		ret.set_ret(res);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_SIM_GT2GM_ATTACK, message, &ret);
		Message_Facade::Send(messRet);
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request attack but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(message->GetMessageTime()));
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(SkillMessage, MSG_REQ_GT2GM_UPGRADE_SKILL)
{
	CharBattle::ClientReqUpgradeSkill req;
	ServerReturn::ServerRetInt con;
	int len = 0;
	char *str = message->GetBuffer(len);
	Smart_Ptr<Player> player;

	req.ParseFromArray(str, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(message->GetMessageTime(), player);
	if((bool)player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eSkill, BehaviorType::Minor::eSkill_Levelup, MONITOR);

		int newID = player->GetSkillMgr()->SkillUpgrade(req.skillid(),(eUpSkillType)req.type());

		con.set_ret(newID);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_UPGRADE_SKILL, message, &con);
		Message_Facade::Send(messRet);

		
	}
	else
	{
		LOG_ERROR(FILEINFO, "Player request skill upgrade but player[gs charid=%lld] is not exist", GET_PLAYER_CHARID(message->GetMessageTime()));
	}
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(SkillMessage, MSG_REQ_C2WS_UPGRADE_CLAN_SKILL)
{
	ClanPackage::ReqUpGrageClanSkill req;
	req.ParseFromString(content);

	ClanPackage::AckUpGradeClanSkill ack;

	if(!player || !player->IsOpenSystem(eStClan))
	{
		ack.set_ret(-1);
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2WS_UPGRADE_CLAN_SKILL);
		return;
	}

	int newSkillID = 0;
	int ret = player->GetSkillMgr()->ClanSkillUpGrade(req.id(), newSkillID);

	ack.set_ret( ret );
	ack.set_oldskill( req.id() );
	ack.set_newskill( newSkillID );
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2WS_UPGRADE_CLAN_SKILL);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(SkillMessage,MSG_REQ_C2WS_CLAN_SKILLS)
{
	vector<int> skills;
	player->GetSkillMgr()->GetClanSkills(skills);

	ClanPackage::AckClanSkills ack;
	for(size_t i=0; i<skills.size(); ++i)
	{
		ack.add_id( skills[i] );
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2WS_CLAN_SKILLS);
}


//明于buff抵抗和免疫
bool SkillMessage::isCanAddBuff(const BuffInfo& info,Smart_Ptr<CreatureObj> owner,Smart_Ptr<CreatureObj> target)
{
	if(!owner)
	{
		return false;
	}

	if(!target)
	{
		return true;
	}

	if(owner.Get() == target.Get())
	{
		return true;
	}

	//处于免疫状态则不添加
	if((info.useType == eStiff && owner->IsInCreatureState(ERS_ImmunityStill)) ||
	   (info.useType == eIce   && owner->IsInCreatureState(ERS_ImmunityIce)) ||
	   (info.useType == eStun  && owner->IsInCreatureState(ERS_ImmunityStun)) ||
	   (info.useType == eAddState && (EAttackState)info.params == eSilentState &&  owner->IsInCreatureState(ERS_ImmunitySilent)) || 
	   (info.useType == eAddState && (EAttackState)info.params == eSleepState &&  owner->IsInCreatureState(ERS_ImmunitySleep)))
	{
		return false;
	}

	int randNum = rand() % 10000 + 1;
	EAttackState tState = eAttackMiss;
	if((tState = Effect::isAddStateControl(info)) != eAttackMiss)
	{
		randNum += owner->getAttManage()->getValue<int>(Effect::stateToStrengAtt(tState));
		if(target)
		{
			randNum -= target->getAttManage()->getValue<int>(Effect::stateToResistAtt(tState));
		}
	}
	
	if(randNum > info.fireRate * 100)
	{
		return false;
	}

	return true;
}



