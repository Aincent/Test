/*
 * GroupMessageMgr.cpp
 *
 *  Created on: 2014年5月22日
 *      Author: CZ
 */

#include "GroupMessage.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturnChar.pb.h"
#include "MessageStruct/Group/GroupMessage.pb.h"
#include "../Object/Player/PlayerMessage.h"
#include "NewGroupMgr.h"

GroupMessageMgr::GroupMessageMgr()
{
	DEF_MSG_QUICK_SIMPLE_REG_FUN(GroupMessageMgr, MSG_REQ_C2GM_CREATETEAM);		//请求创建队伍
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr, MSG_REQ_C2GM_INVITETOTEAM);		//邀请入队
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr, MSG_REQ_C2GM_REPLYINVITE);		//邀请回应
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr, MSG_REQ_C2GM_KICKMEMBER);		//踢出队友
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr, MSG_REQ_C2GM_CHANGELEADER);		//更换队长
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_C2GM_APPLICATETOTEAM);	//申请入队
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_C2GM_GETTEAMID);			//请求附近的队伍ID
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_C2GM_GETPLAYERS);			//请求附近玩家
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_C2GM_LEAVETEAM);			//离开队伍
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_C2GM_LEADERRETAPP);		//队长回复申请者
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_C2GM_LOOKFORTEAMINFO);	//查看队伍信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_C2GM_UPDATETEAMINFO);		//更新队伍信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_SYSTEAMMEMBERMAP);		//同步队员位置信息

	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_C2GM_FUZZYSELECTPLAYERINFO);  //模糊查询 在线玩家

	DEF_MSG_QUICK_REQUEST_REG_FUN(GroupMessageMgr,MSG_REQ_MEMBERAPPLYTOINSTANCE);		//队长创建副本，队员是否愿意进入询问

}

GroupMessageMgr::~GroupMessageMgr()
{

}


DEF_MSG_QUICK_SIMPLE_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_CREATETEAM)
{
	ServerReturn::ServerDoubleRetInt toClient;

	int64 create_groupID = 0;

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->CreateGroup(player, &create_groupID);
		toClient.set_retf(ret);
	}
	else
	{
		toClient.set_retf(eNotOpen);
	}

	toClient.set_rets(create_groupID);

	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GM_CREATETEAM);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_INVITETOTEAM)
{
	ServerReturn::ServerResultInt toClient;
	ServerReturn::ServerRetInt req;

	req.ParseFromString(content);

	Smart_Ptr<Player> dstPlayer;

	PlayerMessage::GetInstance()->GetPlayerByCharid(req.ret(),dstPlayer);
	if(!dstPlayer)
		return;

	if(!dstPlayer->IsOpenSystem(eStGroup))
	{
		toClient.set_ret(eOtherSideLvelNotEnough);
	}
	else if(GET_MAP_TYPE(dstPlayer->GetMapID()) >= eCopyMap)
	{
		toClient.set_ret(eOtherInInstanceMap);
	}
	else if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->InviteToGroup(player, dstPlayer);

		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_INVITETOTEAM);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_INVITETOTEAM);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_REPLYINVITE)
{
	ServerReturn::ServerDoubleRetInt req;
	GroupMessage::RetInviteReply toClient;

	req.ParseFromString(content);

	Smart_Ptr<Player> tFromPlayer;
	PlayerMessage::GetInstance()->GetPlayerByCharid(req.rets(),tFromPlayer);

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->ReplyInviteToGroup(tFromPlayer, req.retf(), player, toClient);
		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_REPLYINVITE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_KICKMEMBER)
{
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerResultInt toClient;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->KickMember(player, req.ret());
		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_KICKMEMBER);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_KICKMEMBER);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_CHANGELEADER)
{
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerResultInt toClient;

	req.ParseFromString(content);

	Smart_Ptr<Player> newleader;
	PlayerMessage::GetInstance()->GetPlayerByCharid(req.ret(), newleader);
	if(!newleader)
	{
		return;
	}

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->ChangeGroupCaptain(player, newleader);

		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_CHANGELEADER);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_CHANGELEADER);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_APPLICATETOTEAM)
{
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerResultInt toClient;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->ApplyJoinGroup(player, req.ret());
		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}


//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_APPLICATETOTEAM);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_APPLICATETOTEAM);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_LEADERRETAPP)
{
	ServerReturn::ServerDoubleRetInt req;
	ServerReturn::ServerResultInt toClient;

	req.ParseFromString(content);

	Smart_Ptr<Player> appPlayer;

	PlayerMessage::GetInstance()->GetPlayerByCharid(req.rets(),appPlayer);
	if(!appPlayer)
		return;

	if(req.retf() == e_group_noway)  //拒绝 不相应
	{
		return ;
	}

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->ReplyApplyJoinGroup(player, appPlayer);

		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_LEADERRETAPP);
	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_LEADERRETAPP);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_GETTEAMID)
{
	GroupMessage::RetGetTeamID toClient;

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->GetNearlyGroupInfoCmd(player, toClient);

		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GETTEAMID);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_GETTEAMID);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_GETPLAYERS)
{
	GroupMessage::RetGetNearbyPlayerInfo toClient;

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->GetNearlyPlayerInfoCmd(player, toClient);

		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_GETPLAYERS);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_GETPLAYERS);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_LEAVETEAM)
{
	ServerReturn::ServerResultInt toClient;

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->LeaveGroup(player);
		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_LEAVETEAM);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_LEAVETEAM);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_LOOKFORTEAMINFO)
{
	ServerReturn::ServerRetInt req;
	GroupMessage::RetLookforTeamInfo toClient;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->FillGroupInfoCmd(player->GetGroupID(), toClient);
		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_LOOKFORTEAMINFO);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &toClient, MSG_REQ_C2GT_LOOKFORTEAMINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_FUZZYSELECTPLAYERINFO)
{
	ServerReturn::ServerRetChar req;
	GroupMessage::RetGetNearbyPlayerInfo toClient;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->GetFuzzyPlayerInfoCmd(req.ret(), toClient);

		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_FUZZYSELECTPLAYERINFO);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(),player->GetChannelID(), &toClient, MSG_REQ_C2GM_FUZZYSELECTPLAYERINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_C2GM_UPDATETEAMINFO)
{
	GroupMessage::RetLookforTeamInfo toClient;

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->FillGroupInfoCmd(player->GetGroupID(), toClient);
		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_UPDATETEAMINFO);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_C2GT_UPDATETEAMINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_MEMBERAPPLYTOINSTANCE)
{

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GroupMessageMgr,MSG_REQ_SYSTEAMMEMBERMAP)
{
	GroupMessage::ToClientSysMemberMap toClient;

	if(player && player->IsOpenSystem(eStGroup))
	{
		int ret = sNewGroupMgr->SynchGroupMemberCmd(player, toClient);

		toClient.set_ret(ret);
	}
	else
	{
		toClient.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_SYSTEAMMEMBERMAP);
//	QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&toClient,MSG_REQ_SYSTEAMMEMBERMAP);
}


