/*
 * NewGroupMgr.cpp
 *
 *  Created on: 2015年1月20日
 *      Author: root
 */

#include "NewGroupMgr.h"
#include "../ServerManager.h"
#include "Log/Logger.h"

#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "../MessageBuild.h"
#include "MessageCommonRet.h"
#include <string.h>
#include "../StatisticMgr/StatisticMgr.h"

#include "StatisticBase/StatisticDefine.h"
#include "../Container/ArticleEquipMgr.h"
#include "../ServerEventDefine.h"

#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include <math.h>
#include "MessageStruct/ServerReturnIntChar.pb.h"

NewGroupMgr::NewGroupMgr()
{
	m_changeMapEvent = RegGlobalEventPtr(PLAYER_ENTER_MAP_EVENT_TYPE, this, &NewGroupMgr::ChangeMap);
}

NewGroupMgr::~NewGroupMgr()
{
	this->Release_NewGroupMgr();
}

void NewGroupMgr::Release_NewGroupMgr()
{
	INT64GROUPINFOMAP::iterator it = m_allGroup.begin();
	for(; it != m_allGroup.end(); ++it)
	{
		list<GroupMember *>::iterator itMem = it->second->memberList.begin();
		for(; itMem != it->second->memberList.end(); ++itMem)
		{
			if(*itMem==NULL)
				continue;
			GroupMember *ptr = *itMem;
			SafeDelete(ptr);
		}

		it->second->memberList.clear();
	}

	m_allGroup.clear();

	if(m_changeMapEvent.IsValid())
		m_changeMapEvent.SetNull();

	if(m_monsterKilledEvent.IsValid())
		m_monsterKilledEvent.SetNull();
}

int NewGroupMgr::CreateGroup(const Smart_Ptr<Player> &player, int64 *create_ID)
{
	if(player->IsHaveGroup())
		return eGroupIsExistence;

	//在副本中不能创建队伍
	if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
		return eInInstanceMap;

	int64 group_key = this->CreateGroup();
	if(group_key <0)
		return group_key;

	int ret = this->FillMemberInfo(group_key, player, true);
	if(ret <0)
		return ret ;

	*create_ID = group_key;

	return 0;
}

int64 NewGroupMgr::CreateGroup()
{
	int64 group_key = CreateGroupKeyMgr::GetInstance()->CreatKey();

	Smart_Ptr<GroupInfo> new_group = NULL;
	new_group = new GroupInfo(group_key);
	if(!new_group)
		return -1;

	m_allGroup[group_key] = new_group;

	return group_key;
}

int NewGroupMgr::FillMemberInfo(const int64 &groupID,
		const Smart_Ptr<Player>& player, bool isCaptain)
{
	INT64GROUPINFOMAP::iterator itGroup = m_allGroup.find(groupID);
	if(itGroup == m_allGroup.end())
	{
		return eGroupIsNotExistence;
	}

	if(isCaptain)
	{
		itGroup->second->mapID = GET_MAP_ID(player->GetMapID());
		itGroup->second->captainID = player->GetID();
	}

	GroupMember *memberInfo = NULL;
	memberInfo = new GroupMember();
	if(!memberInfo)
		return -1;

	this->FillMemberInfo(player, memberInfo, isCaptain);

	player->SetGroupID(groupID);

	itGroup->second->memberList.push_back(memberInfo);

	return 0;
}

int NewGroupMgr::FillMemberInfo(Smart_Ptr<GroupInfo>& group_ptr,
		const Smart_Ptr<Player>& player, bool isCaptain)
{
	GroupMember *memberInfo = NULL;
	memberInfo = new GroupMember();
	if(!memberInfo)
		return -1;

	this->FillMemberInfo(player, memberInfo, isCaptain);

	group_ptr->memberList.push_back(memberInfo);

	player->SetGroupID(group_ptr->groupID);


	return 0;
}

void NewGroupMgr::FillMemberInfo(const Smart_Ptr<Player> &player,
		GroupMember * ptr, bool isCaptain)
{
	if(!ptr || !player)
	{
		return ;
	}

	ptr->memberID = player->GetID();
	ptr->fightPower = player->getAttManage()->getValue<int>(eFightPower);
	ptr->lv = player->getAttManage()->getValue<int>(eCharLv);
	ptr->viplv = player->getAttManage()->getValue<int>(eVIPLv);
	ptr->name = player->GetName();
	ptr->profession = player->getAttManage()->getValue<BYTE>(eCharProfession);
	ptr->isCaptain = isCaptain;

	GroupEventArgs tArgs(player);
	player->FireEvent(PLAYER_ADDGROUP_EVENT_TYPE, tArgs);
}

void NewGroupMgr::BrocastNewGroup(const int64& groupID)
{
}

void NewGroupMgr::GetGroupPtr(const int64& groupID,
		Smart_Ptr<GroupInfo>& group_ptr)
{
	INT64GROUPINFOMAP::const_iterator itGroup = m_allGroup.find(groupID);
	if(itGroup != m_allGroup.end())
	{
		group_ptr = itGroup->second;
	}
}

  GroupMember * NewGroupMgr::GetGroupMemberPtr(const Smart_Ptr<GroupInfo>& group_ptr,
		const int64& charID)
 {
	list<GroupMember *>::iterator itMem = group_ptr->memberList.begin();
	for(; itMem != group_ptr->memberList.end(); ++itMem)
	{
		if(*itMem==NULL)
			continue;
		if((*itMem)->memberID == charID)
			return (*itMem);
	}

	return NULL;
}

GroupMember * NewGroupMgr::GetGroupCaptainPtr(const Smart_Ptr<GroupInfo>& group_ptr)
 {
	list<GroupMember *>::iterator itMem = group_ptr->memberList.begin();
	for(; itMem != group_ptr->memberList.end(); ++itMem)
	{
		if(*itMem==NULL)
			continue;

		if((*itMem)->isCaptain)
			return (*itMem);
	}

	return NULL;
 }

int NewGroupMgr::InviteToGroup(const Smart_Ptr<Player>& from_player,const Smart_Ptr<Player>& to_player)
{
	//邀请玩家是否屏蔽邀请
	if(to_player->GetSettings(eSettingsRanks))
		return  ePlayerNotInviteToTeam;

	if(to_player->IsHaveGroup() && from_player->IsHaveGroup())
		return eOtherAlreadyHaveGroup;

	Smart_Ptr<GroupInfo> group_ptr = NULL;
	this->GetGroupPtr(from_player->GetGroupID(), group_ptr);

	if(group_ptr && group_ptr->memberList.size() >= GameParaLoader::GetInstance()->getGroupMemberNum())
		return eGroupIsFull;

	group_ptr = NULL;
	this->GetGroupPtr(to_player->GetGroupID(), group_ptr);
	if(group_ptr && group_ptr->memberList.size() >= GameParaLoader::GetInstance()->getGroupMemberNum())
		return eGroupIsFull;

	if(to_player->IsHaveGroup() == 0)	
	{
		
		this->SendInviteMessage(from_player, to_player);
		return eGroupInviteSuccess;
	}

	GroupMessage::ToLeaderAcquire toLeader;

	toLeader.set_dstplayerid(from_player->GetID());
	toLeader.set_acquirename(from_player->GetName());

	QuickMessage::GetInstance()->AddSendMessage(to_player->GetID(), to_player->GetChannelID(),&toLeader,MSG_SIM_GM2C_APPTOLEADER);

	return eGroupApplySuccess;
}

void NewGroupMgr::SendInviteMessage(const Smart_Ptr<Player> &from_player,const Smart_Ptr<Player> &to_player)
{
	// 向被邀请者发消息
	GroupMessage::RetInviteToMember toMember;

	toMember.set_dwteammemid(from_player->GetID());
	toMember.set_sleadername(from_player->GetName());

	QuickMessage::GetInstance()->AddSendMessage(to_player->GetID(),to_player->GetChannelID(),&toMember,MSG_SIM_TOINVITOR);
}

int NewGroupMgr::ReplyInviteToGroup(Smart_Ptr<Player> fromPlayer, const int& agree,
		const Smart_Ptr<Player> &toPlayer, GroupMessage::RetInviteReply &toReply)
{
	if(!fromPlayer)
	{
		return eNotHaveOnline;
	}

	if(agree == e_group_noway)
	{
		//拒绝申请
		if(toPlayer->GetGroupID())
		{
			sendInviteResult(fromPlayer,eRefuseYouApplyGroup,toPlayer);

			return eRefuseOtherApplyGroup;
		}
		else
		{
			sendInviteResult(fromPlayer,eOtherRefuseInvite,toPlayer);

			return eRefuseOtherInviteGroup;
		}
	}

	if(toPlayer->GetGroupID() != 0 && fromPlayer->GetGroupID() != 0)
	{
		return eGroupIsExistence;
	}

	if(fromPlayer->GetGroupID() == 0 && toPlayer->GetGroupID() == 0)
	{
		ServerRet tError = eRetSuccess;
		tError = inviteCreateGroup(fromPlayer);
		if(tError != eRetSuccess)
		{
			sendInviteResult(fromPlayer,tError,toPlayer);

			return tError;
		}
	}

	Smart_Ptr<GroupInfo> group_ptr = NULL;
	this->GetGroupPtr(fromPlayer->GetGroupID(), group_ptr);

	if(group_ptr && group_ptr->memberList.size() >= GameParaLoader::GetInstance()->getGroupMemberNum())
	{
		return eGroupIsFull;
	}

	this->GetGroupPtr(toPlayer->GetGroupID(), group_ptr);

	if(group_ptr && group_ptr->memberList.size() >= GameParaLoader::GetInstance()->getGroupMemberNum())
	{
		return eGroupIsFull;
	}

	if(!group_ptr)
	{
		return eSystemError; 
	}
	
	//添加成员
	int ret = eRetSuccess;
  
	Smart_Ptr<Player> tJoinPlayer;	
	if(fromPlayer->GetGroupID() == 0)
	{
		tJoinPlayer  = fromPlayer;
		ret = this->FillMemberInfo(group_ptr,fromPlayer , false);
	}
	else
	{
		tJoinPlayer  = toPlayer;
		ret = this->FillMemberInfo(group_ptr, toPlayer, false);
	}

	if(ret <0)
		return ret;

	//获取 其他 成员 信息
	this->BrocastApplyer(group_ptr, tJoinPlayer);

	//发送 给 其他 成员
	this->SendReplyToMember(group_ptr, tJoinPlayer);

	this->ChangeAttrWhenMemberChange(group_ptr);

	//如果本次为申请,则不通过这里通知申请者
	if(fromPlayer.Get() != tJoinPlayer.Get())
	{
		sendInviteResult(fromPlayer,eGroupSuccess,toPlayer);
	}

	return 0;
}

ServerRet NewGroupMgr::inviteCreateGroup(Smart_Ptr<Player> player)
{
	ServerReturn::ServerDoubleRetInt toClient;

	int64 create_groupID = 0;
	ServerRet tError = eRetSuccess;

	if(player && player->IsOpenSystem(eStGroup))
	{
		tError = (ServerRet)CreateGroup(player,&create_groupID);
	}
	else
	{
		tError = eNotOpen;
	}

	if(tError != eRetSuccess)
	{
		return eRetSuccess;
	}

	toClient.set_rets(create_groupID);

	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_CREATETEAM);

	return tError;
}

void NewGroupMgr::sendInviteResult(Smart_Ptr<Player> invitePlayer,ServerRet error,Smart_Ptr<Player> beInvitePlayer)
{
	ServerReturn::ServerRetIntChar toClient;

	toClient.set_retf(error);
	toClient.set_rets(beInvitePlayer->GetName());

	QuickMessage::GetInstance()->AddSendMessage(invitePlayer->GetID(), invitePlayer->GetChannelID(),&toClient,MSG_REQ_GM2C_RESPONSES_INVITE_GROUP);

	return ;
}

void NewGroupMgr::DeleteGroup(const int64& groupID)
{
	INT64GROUPINFOMAP::iterator it = m_allGroup.find(groupID);
	if(it != m_allGroup.end())
	{
		m_allGroup.erase(it);
	}
}

int NewGroupMgr::SendReplyToMember(const Smart_Ptr<GroupInfo>& group_ptr,const Smart_Ptr<Player> &new_player)
{
	GroupMessage::RetPlayerStateToTeam toMember;

	this->FillReplyToMemberCmd(new_player, group_ptr->mapID, group_ptr->captainID,group_ptr->memberList.size() , &toMember);

	list<GroupMember *>::const_iterator itMem = group_ptr->memberList.begin();
	for(; itMem != group_ptr->memberList.end(); ++itMem)
	{
		if(*itMem==NULL)
			continue;

		if((*itMem)->memberID != new_player->GetID())
		{
			Smart_Ptr<Player> mem_player = NULL;

			PlayerMessage::GetInstance()->GetPlayerByCharid((*itMem)->memberID, mem_player);
			if(!mem_player)
			{
				Print_MemberInfo((*itMem));

				return eNotFindThisCharID;
			}

			QuickMessage::GetInstance()->AddSendMessage(mem_player->GetID(),mem_player->GetChannelID(),&toMember,MSG_SIM_BROADCASTPLAYERSINFO);
		}
	}

	return 0;
}

int NewGroupMgr::FillAnyMemberCmd(const Smart_Ptr<GroupInfo>& group_ptr,const Smart_Ptr<Player>& new_player,
		GroupMessage::RetInviteReply& toReply)
{
	toReply.set_dwteamid(group_ptr->groupID);

	list<GroupMember *>::const_iterator itMem = group_ptr->memberList.begin();
	for(; itMem != group_ptr->memberList.end(); ++itMem)
	{
		if(*itMem==NULL)
			continue;

		if((*itMem)->memberID != new_player->GetID())
		{
			Smart_Ptr<Player> mem_player = NULL;

			PlayerMessage::GetInstance()->GetPlayerByCharid((*itMem)->memberID, mem_player);
			if(!mem_player)
			{
				Print_MemberInfo((*itMem));

				return eNotFindThisCharID;
			}

			GroupMessage::RetPlayerStateToTeam* toMember = toReply.add_playerinfo();

			this->FillReplyToMemberCmd(mem_player, group_ptr->mapID, group_ptr->captainID, group_ptr->memberList.size(), toMember);
		}
	}
	return 0;
}

void NewGroupMgr::FillReplyToMemberCmd(const Smart_Ptr<Player>& player, const int &captainMapID,
		const int64 &captainID, const uint& member_count, GroupMessage::RetPlayerStateToTeam* toMember)
{
	//不知道主武器的位置，先根据客户端放的位置临时取的
	Smart_Ptr<EquipProp> propEquip = player->GetContainer(eEquipQuickType)->GetItemDataPtr(eItemPlaceWeapon-1);
	int equipID = -1;
	if(propEquip && propEquip->GetID() != -1)
	{
		equipID = propEquip->GetID();
	}

	int costumeID = -1;
	propEquip = player->GetContainer(eEquipQuickType)->GetItemDataPtr(eItemPlaceArmor-1);
	if(propEquip && propEquip->GetID() != -1)
	{
		costumeID = propEquip->GetID();
	}

	toMember->set_dwplayerid(player->GetID());		//ID
	toMember->set_splayername(player->GetName());

	toMember->set_dwhorseid(player->GetHorseModelID());//坐骑ID
	toMember->set_dwequipid(equipID);					//武器ID
	toMember->set_eclassex(player->getAttManage()->getValue<BYTE>(eCharProfession));								//职业
	toMember->set_bysex(player->GetSex());			//性别
	toMember->set_nlevel(player->getAttManage()->getValue<int>(eCharLv));			//level
	toMember->set_dwplayerpower(player->getAttManage()->getValue<int>(eFightPower));
	toMember->set_costume(costumeID);

	int mapid = GET_MAP_ID(player->GetMapID());

	toMember->set_mapid(mapid);

	if(mapid == captainMapID && member_count>= GameParaLoader::GetInstance()->getGroupMemberAddSomthigNum())
		toMember->set_dwoutofrange(true);
	else
		toMember->set_dwoutofrange(false);

	if(player->GetID() == captainID)
		toMember->set_iscaptain(true);
	else
		toMember->set_iscaptain(false);

	toMember->set_fashionid(player->GetFashionID()); // 时装ID
	toMember->set_viplv(player->getAttManage()->getValue<int>(eVIPLv));
}

void NewGroupMgr::DeleteMember(Smart_Ptr<GroupInfo>& group_ptr,
		const int64& memberID)
{
	list<GroupMember *>::iterator itMember = group_ptr->memberList.begin();
	for(; itMember != group_ptr->memberList.end(); ++itMember)
	{
		if(*itMember==NULL)
			continue;

		if((*itMember)->memberID == memberID)
		{
			ReversePKType(memberID);

			GroupMember * tmpptr = *itMember;
			SafeDelete(tmpptr);
			itMember = group_ptr->memberList.erase(itMember);
			return ;
		}
	}
}

int NewGroupMgr::KickMember(const Smart_Ptr<Player>& player,
		const int64& charID)
{
	if(player->GetID() == charID)
		return eLeaderNotKickMe;

	Smart_Ptr<GroupInfo> group_ptr = NULL;

	this->GetGroupPtr(player->GetGroupID(), group_ptr);
	if(!group_ptr)
		return eGroupIsNotExistence;

	const GroupMember *mem_ptr = GetGroupMemberPtr(group_ptr, player->GetID());
	if(!mem_ptr)
	{
//		Print_MemberInfo(mem_ptr);

		return eNotFindThisCharID;
	}

	if(!mem_ptr->isCaptain)
		return eNotLeaderNoCompetence;

	mem_ptr = GetGroupMemberPtr(group_ptr, charID);
	if(!mem_ptr)
		return eNotGroupMember;

	Smart_Ptr<Player> mem_player;

	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, mem_player);
	if(!mem_player)
	{
		return eNotFindThisCharID;
	}

	this->BrocastKickMember(group_ptr, player->GetID(), charID);

	mem_player->SetGroupID(0);

	this->DeleteMember(group_ptr, charID);

	return 0;
}

int NewGroupMgr::BrocastKickMember(const Smart_Ptr<GroupInfo> &group_ptr,
		const int64 & captainID, const int64 &leaveID)
{
	ServerReturn::ServerRetInt toMember;

	toMember.set_ret(leaveID);

	list<GroupMember *>::const_iterator itMember = group_ptr->memberList.begin();
	for(; itMember != group_ptr->memberList.end(); ++itMember)
	{
		if(*itMember==NULL)
			continue;

		Smart_Ptr<Player> mem_player;
		PlayerMessage::GetInstance()->GetPlayerByCharid((*itMember)->memberID,mem_player);
		if(!mem_player)
		{
			Print_MemberInfo((*itMember));

			return eNotFindThisCharID;
		}

		QuickMessage::GetInstance()->AddSendMessage(mem_player->GetID(),mem_player->GetChannelID(), &toMember,MSG_SIM_BROADCASTKICKOUT);
	}

	return 0;
}

int NewGroupMgr::ChangeGroupCaptain(const Smart_Ptr<Player>& player,
		const Smart_Ptr<Player>& willplayer)
{
	Smart_Ptr<GroupInfo> group_ptr;

	this->GetGroupPtr(player->GetGroupID(), group_ptr);
	if(!group_ptr)
		return eGroupIsNotExistence;

	GroupMember *captainmem_ptr = GetGroupMemberPtr(group_ptr, player->GetID());
	if(!captainmem_ptr)
	{
//		Print_MemberInfo(captainmem_ptr);

		return eNotFindThisCharID;
	}

	if(!captainmem_ptr->isCaptain)
		return eNotLeaderNoCompetence;

	GroupMember * mem_ptr = GetGroupMemberPtr(group_ptr, willplayer->GetID());
	if(!mem_ptr)
		return eNotGroupMember;

	captainmem_ptr->isCaptain = false;

	mem_ptr->isCaptain = true;

	group_ptr->mapID = GET_MAP_ID(willplayer->GetMapID());
	group_ptr->captainID = willplayer->GetID();

	this->BrocastChangeCaptain(group_ptr, willplayer->GetID());

	ChangeAttrWhenMemberChange(group_ptr);

	return 0;
}

void NewGroupMgr::BrocastChangeCaptain(const Smart_Ptr<GroupInfo> &group_ptr,
		const int64 &new_captainID)
{
	ServerReturn::ServerRetInt toMember;

	toMember.set_ret(new_captainID);

	list<GroupMember *>::const_iterator itMember = group_ptr->memberList.begin();
	for(; itMember != group_ptr->memberList.end(); ++itMember)
	{
		if(*itMember==NULL)
			continue;

		Smart_Ptr<Player> mem_player;
		PlayerMessage::GetInstance()->GetPlayerByCharid((*itMember)->memberID,mem_player);
		if(!mem_player)
		{
			Print_MemberInfo((*itMember));
			return ;
		}

		QuickMessage::GetInstance()->AddSendMessage(mem_player->GetID(),mem_player->GetChannelID(), &toMember,MSG_SIM_BROADCASTCHANGELEADER);
	}
}

int NewGroupMgr::ApplyJoinGroup(const Smart_Ptr<Player>& player,
		const int64& groupID)
{
	if(player->IsHaveGroup())
		return eGroupIsExistence;

	Smart_Ptr<GroupInfo> group_ptr;
	this->GetGroupPtr(groupID, group_ptr);
	if(!group_ptr)
		return eGroupIsNotExistence;

	if(group_ptr->memberList.size() >= GameParaLoader::GetInstance()->getGroupMemberNum())
		return eGroupIsFull;

	GroupMember * mem_ptr = GetGroupCaptainPtr(group_ptr);
	if(!mem_ptr)
	{
//		Print_MemberInfo(mem_ptr);

		return eNotFindThisCharID;
	}

		this->BrocastApplyIoinGroup(mem_ptr, player);

	//统计 申请入队次数
//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(player, eStatic_Team, eStaMinor_Team_AppToTeam, 1);

	return 0;
}

void NewGroupMgr::BrocastApplyIoinGroup(const GroupMember * mem_ptr, const Smart_Ptr<Player>& appPlayer)
{
	Smart_Ptr<Player> mem_player;

	PlayerMessage::GetInstance()->GetPlayerByCharid(mem_ptr->memberID, mem_player);
	if(!mem_player)
	{
		return ;
	}

	GroupMessage::ToLeaderAcquire toLeader;

	toLeader.set_dstplayerid(appPlayer->GetID());
	toLeader.set_acquirename(appPlayer->GetName());

	QuickMessage::GetInstance()->AddSendMessage(mem_player->GetID(), mem_player->GetChannelID(),&toLeader,MSG_SIM_GM2C_APPTOLEADER);
}

int NewGroupMgr::ReplyApplyJoinGroup(const Smart_Ptr<Player>& player,
		const Smart_Ptr<Player>& appPlayer)
{
	if(appPlayer->IsHaveGroup())
		return eGroupIsExistence;

	Smart_Ptr<GroupInfo> group_ptr;
	this->GetGroupPtr(player->GetGroupID(), group_ptr);
	if(!group_ptr)
		return eGroupIsNotExistence;

	if(group_ptr->memberList.size() >= GameParaLoader::GetInstance()->getGroupMemberNum())
		return eGroupIsFull;

	GroupMember * mem_ptr = GetGroupCaptainPtr(group_ptr);
	if(!mem_ptr)
	{
//		Print_MemberInfo(mem_ptr);

		return eNotFindThisCharID;
	}

	if(mem_ptr->memberID != player->GetID())
			return eNotLeaderNoCompetence;

	ChangeMemExpAdd(group_ptr, false);
	//添加成员
	int ret = this->FillMemberInfo(group_ptr, appPlayer, false);
	if(ret == 0)
	{
		//通知 申请这
		this->BrocastApplyer(group_ptr, appPlayer);

		//发送 给 其他 成员
		this->SendReplyToMember(group_ptr, appPlayer);

		this->ChangeAttrWhenMemberChange(group_ptr);
	}

	ChangeMemExpAdd(group_ptr, true);

	return ret;
}

void  NewGroupMgr::BrocastApplyer(const Smart_Ptr<GroupInfo> &group_ptr,
		const Smart_Ptr<Player>& appPlayer)
{
	GroupMessage::RetInviteReply info;

	info.set_ret(0);

	this->FillAnyMemberCmd(group_ptr, appPlayer, info);

	QuickMessage::GetInstance()->AddSendMessage(appPlayer->GetID(),appPlayer->GetChannelID(),&info,MSG_SIM_APPGETPLAYERSINFO);
}

int NewGroupMgr::GetNearlyGroupInfoCmd(const Smart_Ptr<Player>& player,
		GroupMessage::RetGetTeamID& info)
{
	int mapID = GET_MAP_ID(player->GetMapID());

	INT64GROUPINFOMAP::const_iterator itGroup = m_allGroup.begin();
	for(; itGroup != m_allGroup.end(); ++itGroup)
	{
		if(itGroup->second->mapID == mapID)
		{
			GroupMessage::TeamInfo *team = info.add_teaminfo();

			FillGroupInfoCmd(player, itGroup->second, team);
		}

		if(info.teaminfo_size() >= GameParaLoader::GetInstance()->getOutputPlayerInfoNum())
			return 0;
	}

	return 0;
}

void NewGroupMgr::FillGroupInfoCmd(const Smart_Ptr<Player>& player,
		const Smart_Ptr<GroupInfo>& group_ptr, GroupMessage::TeamInfo* info)
{
	int powfight = 0;
	int lv = 0;
	bool isJoin = false;
	string capname = "";

	if(player->GetGroupID() == group_ptr->groupID)
		isJoin = true;

	list<GroupMember *>::const_iterator itMem = group_ptr->memberList.begin();
	for(; itMem != group_ptr->memberList.end(); ++itMem)
	{
		if(*itMem==NULL)
			continue;

		if((*itMem)->isCaptain)
			capname = (*itMem)->name;

		powfight += (*itMem)->fightPower;
		lv += (*itMem)->lv;

		GroupMessage::MemberInfo* member_cmd = info->add_groupmember();

		FillGroupMemberCmd((*itMem), member_cmd);
	}

	info->set_dwteamid(group_ptr->groupID);
	info->set_dwteampower(powfight);
	int memberListSize = ((int)group_ptr->memberList.size());
	if(memberListSize > 0)
		info->set_dwteamlevel(lv/memberListSize);
	info->set_sleadername(capname);
	info->set_dwteammembers(memberListSize);
}

void NewGroupMgr::FillGroupMemberCmd(const GroupMember * ptr,
		GroupMessage::MemberInfo* info)
{
	info->set_charid(ptr->memberID);
	info->set_fight(ptr->fightPower);
	info->set_name(ptr->name);
	info->set_profession(ptr->profession);
	info->set_lv(ptr->lv);
	info->set_viplv(ptr->viplv);
}

int NewGroupMgr::GetNearlyPlayerInfoCmd(const Smart_Ptr<Player>& player,
		GroupMessage::RetGetNearbyPlayerInfo& info)
{
	Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(player->GetMapID());
	if(!zone)
	{
		return eRetZoneNotExist;
	}

	//获取 本场景中 玩家
	vector<Smart_Ptr<Player> > players;
	zone->GetWantPlayers(OutputPlayerInfo_Num, players);

	vector<Smart_Ptr<Player> >::const_iterator it = players.begin();
	for(; it != players.end(); ++it)
	{
		if((*it).Get()==NULL)
			continue;

		if((*it)->GetID() != player->GetID())
		{
			GroupMessage::NearbyPlayerInfo* nearly_info = info.add_playerinfos();

			FillNearlyPlayerInfoCmd(*it, nearly_info);
		}
	}
	return 0;
}

void NewGroupMgr::FillNearlyPlayerInfoCmd(const Smart_Ptr<Player>& player,
		GroupMessage::NearbyPlayerInfo* info)
{
	info->set_dwplayerid(player->GetID());
	info->set_splayername(player->GetName());
	info->set_nlevel(player->getAttManage()->getValue<int>(eCharLv));
	info->set_bhasteam(player->IsHaveGroup());
	info->set_eclassex(player->getAttManage()->getValue<BYTE>(eCharProfession));
	info->set_powernum(player->getAttManage()->getValue<int>(eFightPower));
	info->set_viplv(player->getAttManage()->getValue<int>(eVIPLv));
}

int64 NewGroupMgr::ChangeGroupCaptain(const Smart_Ptr<GroupInfo> &group_ptr)
{
	list<GroupMember *>::iterator it = group_ptr->memberList.begin();
	(*it)->isCaptain = true;

	return (*it)->memberID;
}

int NewGroupMgr::LeaveGroup(const Smart_Ptr<Player>& player)
{
	if(!player->IsHaveGroup())
		return eNotGroupMember;

	Smart_Ptr<GroupInfo> group_ptr;
	this->GetGroupPtr(player->GetGroupID(), group_ptr);
	if(!group_ptr)
	{
		return eGroupIsNotExistence;
	}

	ChangeMemExpAdd(group_ptr, false);
	int ret = this->LeaveGroup(group_ptr, player);
	ChangeMemExpAdd(group_ptr, true);
	return ret;
}

void NewGroupMgr::BrocastLeaveGroupCmd(const Smart_Ptr<GroupInfo>& group_ptr,
		const int64 &leaveID)
{
	list<GroupMember *>::iterator it = group_ptr->memberList.begin();
	for(; it != group_ptr->memberList.end(); ++it)
	{
		if(*it==NULL)
			continue;

		Smart_Ptr<Player> mem_player;
		PlayerMessage::GetInstance()->GetPlayerByCharid((*it)->memberID, mem_player);
		if(!mem_player)
		{
			Print_MemberInfo((*it));
			return ;
		}

		ServerReturn::ServerRetInt toMem;

		toMem.set_ret(leaveID);

		QuickMessage::GetInstance()->AddSendMessage(mem_player->GetID(),mem_player->GetChannelID(),&toMem, MSG_SIM_BROADCASTLEAVEPLAYER);
	}
}

int NewGroupMgr::GetFuzzyPlayerInfoCmd(const string &str,
		GroupMessage::RetGetNearbyPlayerInfo& info)
{
	list<Smart_Ptr<Player> > players;

	//模糊查询
	PlayerMessage::GetInstance()->FuzzySelectPlayerInfo(str, OutputPlayerInfo_Num, players);

	list<Smart_Ptr<Player> >::iterator it = players.begin();
	for(; it != players.end(); ++it)
	{
		if((*it).Get()==NULL)
			continue;

		GroupMessage::NearbyPlayerInfo* nearly_info = info.add_playerinfos();
		FillNearlyPlayerInfoCmd(*it, nearly_info);
	}

	return 0;
}

int NewGroupMgr::LeaveGroup(Smart_Ptr<GroupInfo>& group_ptr,
		const Smart_Ptr<Player>& player)
{
	GroupMember * mem_ptr = GetGroupMemberPtr(group_ptr, player->GetID());
	if(!mem_ptr)
	{
		return eNotGroupMember;
	}

	//通知 其他  队员 有退出
	BrocastLeaveGroupCmd(group_ptr, player->GetID());

	bool isCaptain = mem_ptr->isCaptain;
	DeleteMember(group_ptr, player->GetID());
	player->SetGroupID(0);

	if(group_ptr->memberList.size() <=0)
	{
		DeleteGroup(group_ptr->groupID);
	}
	else
	{
		int64 newLeaderID = 0;
		if(isCaptain)
		{//直接  下一个 为 队长
			newLeaderID = ChangeGroupCaptain(group_ptr);
			group_ptr->captainID = newLeaderID;
			BrocastChangeCaptain(group_ptr, newLeaderID);
		}

		//通知 其他  队员 有退出
		//BrocastLeaveGroupCmd(group_ptr, player->GetID());
		ChangeAttrMemberLeave(group_ptr);
	}

	player->ChangeAttrWhenGroupReduce(player->GetGroupAttr());

	return 0;
}

bool NewGroupMgr::ChangeMap(const EventArgs& e)
{
	PlayerMapEventArgs *arg = (PlayerMapEventArgs *)(&e);
	Smart_Ptr<Player> player = arg->m_obj;
	if(!player)
		return true;

	if(player->IsHaveGroup())
	{
		Smart_Ptr<GroupInfo> group_ptr;

		this->GetGroupPtr(player->GetGroupID(), group_ptr);
		if(!group_ptr)
		{
			return true;
		}

		GroupMember *captain_ptr = this->GetGroupCaptainPtr(group_ptr);
		if(captain_ptr && captain_ptr->memberID == player->GetID())
		{
			group_ptr->mapID = GET_MAP_ID(player->GetMapID());
		}

		//由于  队员 地图 变更 检测  队员 属性 变更
		ChangeAttrWhenMemberChangeMap(group_ptr, player->GetID());
	}

	return true;
}

void NewGroupMgr::ChangeAttrMemberLeave(const Smart_Ptr<GroupInfo> &group_ptr)
{
	list<GroupMember *>::const_iterator it = group_ptr->memberList.begin();
	for(; it != group_ptr->memberList.end(); ++it)
	{
		if(*it==NULL)
			continue;

		Smart_Ptr<Player> mem_player;
		PlayerMessage::GetInstance()->GetPlayerByCharid((*it)->memberID, mem_player);
		if(!mem_player)
		{
			Print_MemberInfo((*it));

			return ;
		}

		mem_player->ChangeAttrWhenGroupReduce(1);
	}
}

int NewGroupMgr::SynchGroupMemberCmd(const Smart_Ptr<Player>& player,
		GroupMessage::ToClientSysMemberMap& info)
{
	if(!player->IsHaveGroup())
		return eNotGroupMember;

	Smart_Ptr<GroupInfo> group_ptr;
	this->GetGroupPtr(player->GetGroupID(), group_ptr);
	if(!group_ptr)
	{
		return eGroupIsNotExistence;
	}

	list<GroupMember *>::const_iterator it = group_ptr->memberList.begin();
	for(; it != group_ptr->memberList.end(); ++it)
	{
		if(*it==NULL)
			continue;

		if((*it)->memberID != player->GetID())
		{
			Smart_Ptr<Player> mem_player;

			PlayerMessage::GetInstance()->GetPlayerByCharid((*it)->memberID, mem_player);
			if(!mem_player)
			{
				Print_MemberInfo((*it));

				return eNotFindThisCharID;
			}

			info.add_charid(mem_player->GetID());
			info.add_mapid(GET_MAP_ID(mem_player->GetMapID()));
		}
	}

	return 0;
}

int NewGroupMgr::FillGroupInfoCmd(const int64& groupID,
		GroupMessage::RetLookforTeamInfo& info)
{
	Smart_Ptr<GroupInfo> group_ptr;
	this->GetGroupPtr(groupID, group_ptr);
	if(!group_ptr)
		return eGroupIsNotExistence;

	list<GroupMember *>::const_iterator it = group_ptr->memberList.begin();
	for(; it != group_ptr->memberList.end(); ++it)
	{
		if(*it==NULL)
			continue;

		Smart_Ptr<Player> mem_player;
		PlayerMessage::GetInstance()->GetPlayerByCharid((*it)->memberID, mem_player);
		if(!mem_player)
		{
			Print_MemberInfo((*it));

			return eNotFindThisCharID;
		}

		GroupMessage::RetPlayerStateToTeam *toMember = info.add_playerinfo();

		this->FillReplyToMemberCmd(mem_player,group_ptr->mapID, group_ptr->captainID,group_ptr->memberList.size() , toMember);
	}

	return 0;
}

double NewGroupMgr::GetGroupAdditionWhenKillMonster(const uint memberNum)
{
	return 0.1f * (memberNum - 1);
}

bool NewGroupMgr::IsSameMapGroupMember(const int& mapID, const int64& charID,Smart_Ptr<Player>& player)
{
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if(!player)
	{
		Smart_Ptr<Player> tempPlayer;

		PlayerMessage::GetInstance()->GetPlayerCacheByCharid(charID,tempPlayer);

		if(tempPlayer)
		{
			string tempName = tempPlayer->GetName();

			LOG_WARNING(FILEINFO,"队伍在计算经验时，玩家：［%s]正在换图",tempName.c_str());
		}

		return false;
	}

	if(GET_MAP_ID(player->GetMapID()) == mapID)
	{
		return true;
	}
	return false;
}

void NewGroupMgr::ChangeAttrWhenMemberChange(const Smart_Ptr<GroupInfo>& group_ptr)
{
	list<GroupMember *>::const_iterator it = group_ptr->memberList.begin();
	for(; it != group_ptr->memberList.end(); ++it)
	{
		if(*it==NULL)
			continue;

		Smart_Ptr<Player> mem_player;
		PlayerMessage::GetInstance()->GetPlayerByCharid((*it)->memberID, mem_player);
		if(!mem_player)
		{
			Print_MemberInfo((*it));

			return;
		}

		mem_player->ChangeAttrWhenGroupMemberChangeMap(group_ptr);
	}
}

void NewGroupMgr::ChangeAttrWhenMemberChangeMap(const Smart_Ptr<GroupInfo> &group_ptr,const int64 &leaveMapCharID)
{
	list<GroupMember *>::const_iterator it = group_ptr->memberList.begin();
	for(; it != group_ptr->memberList.end(); ++it)
	{
		if(*it==NULL)
			continue;

		Smart_Ptr<Player> mem_player;
		if(leaveMapCharID == (*it)->memberID)
		{
			//切换地图的 人  要 playercache中 找
			PlayerMessage::GetInstance()->GetPlayerCacheByCharid((*it)->memberID, mem_player);
			if(!mem_player)
			{
				Print_MemberInfo((*it));

				return;
			}
		}
		else
		{
			/*leaveMapCharID该玩家换图时，(*it)->memberID也处于换图状态，
			GetPlayerByCharid是不能获取得到数据，可等(*it)->memberID换图成功会再调用ChangeAttrWhenMemberChangeMap*/
			PlayerMessage::GetInstance()->GetPlayerByCharid((*it)->memberID, mem_player);

			if(!mem_player)
			{
				Smart_Ptr<Player> tempPlayer;
				PlayerMessage::GetInstance()->GetPlayerCacheByCharid((*it)->memberID, tempPlayer);

				if(tempPlayer)
				{
					string tempName;

					tempName 	   = tempPlayer->GetName();
					int tempCharID = leaveMapCharID;

					LOG_WARNING(FILEINFO,"队伍在计算血加成时，玩家：［%d]正在换图时玩家[%s]也在换图",tempCharID,tempName.c_str());
				}

				return;
			}
		}

		mem_player->ChangeAttrWhenGroupMemberChangeMap(group_ptr);
	}
}

void NewGroupMgr::Print_MemberInfo(const GroupMember* ptr)
{
	if(ptr)
		printf("\n ---------- MemberId[%lld]  --------------\n", ptr->memberID);
}

void NewGroupMgr::ChangeMemExpAdd(const Smart_Ptr<GroupInfo>& group, bool flag)
{
	if ((bool)group)
	{
		double addition = GetGroupAdditionWhenKillMonster(group->memberList.size());
		Smart_Ptr<Player> player;

		if (0 >= addition)
		{
			return;
		}

		for (list<GroupMember *>::const_iterator it = group->memberList.begin(); it != group->memberList.end(); ++it)
		{
			PlayerMessage::GetInstance()->GetPlayerByCharId( player, (*it)->memberID);
			if ((bool)player)
			{
				if (flag)
				{
					player->getAttManage()->addValue(eKillMonsterExpRate,addition);
				}
				else
				{
					player->getAttManage()->addValue(eKillMonsterExpRate,-addition);
				}
			}
		}
	}
}

void NewGroupMgr::ReversePKType(int64 charid)
{
		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharid(charid, player);
		if(player.Get() == NULL)
		{
			return;
		}

		if( player->getAttManage()->getValue<int>(ePKType) == sTeam )
		{
			player->getAttManage()->setValue(ePKType, (int)sPeace);

			vector<int> vecType;
			vecType.push_back(ePKType);
			player->SynCharAttribute(vecType);

		}
}

