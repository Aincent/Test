/*
 * ChatMgr.cpp
 *
 *  Created on: 2014年5月20日
 *      Author: root
 */

#include "ChatMgr.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageCommonRet.h"
#include <sstream>
#include <string>
#include "../Order/OrderMgr.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "../ServerEventDefine.h"
#include "../Object/Monster/Monster.h"
#include "./FileLoader/InternationalLoader.h"
#include "../Group/NewGroupMgr.h"
#include "FileLoader/GameParaLoader.h"
#include "../Map/GameZone.h"
#include "../FileLoader/MallLoader.h"

ChatMgr::ChatMgr()
{
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_CHAT);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GT2WS_CHAT);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_WORLDCHATVITALITY);
	m_killMonsterEvent = RegGlobalEventPtr(MONSTER_KILLED_EVENT_TYPE, this, &ChatMgr::KillMonster);
}

ChatMgr::~ChatMgr()
{
	if(m_killMonsterEvent.IsValid())
	{
		m_killMonsterEvent.SetNull();
	}
}

bool ChatMgr::KillMonster(const EventArgs & e)
{
	TwoParmaEventArgs* data = (TwoParmaEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	if(data->m_other->GetType() != ePlayer && data->m_other->GetType() != ePet)
		return true;

	Smart_Ptr<Monster> mon = data->m_myself;
	if(mon.Get()==NULL || mon->GetQuality() != eGuardMonster)
	{
		return true;
	}

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

	if(player && mon)
	{
		{
			string name = player->GetName();
			char mapID[32];
			sprintf(mapID,"%lld",mon->GetMapID());
			string strMapID(mapID);
			ChatMgr::GetInstance()->Marquee(eMarqueeKillGuard,player->GetName(),strMapID,mon->GetName());
		}
	}

	return true;
}

bool ChatMgr::IsCDEnd(Smart_Ptr<Player> player, int chatType)
{
	switch (chatType)
	{
	case eChatOrdinary:
		{
			if(!player->GetCDMgr()->IsCDEnd(CD_ORDINARY))
			{
				return false;
			}
			player->GetCDMgr()->StartCD(CD_ORDINARY,GameParaLoader::GetInstance()->getChatGeneralCd());
		}
		break;
		case eChatHorn:
			{
				if(!player->GetCDMgr()->IsCDEnd(CD_HORN))
				{
					return false;
				}
				player->GetCDMgr()->StartCD(CD_HORN,GameParaLoader::GetInstance()->getChatHornCd());
			}
			break;
	case eChatRanks:
		{
			if(!player->GetCDMgr()->IsCDEnd(RANKS_CD))
			{
				return false;
			}
			player->GetCDMgr()->StartCD(RANKS_CD,GameParaLoader::GetInstance()->getChatTeamCd());
		}
		break;
	default:
		break;
	}

	return true;
}

void ChatMgr::OrdinaryChat(Smart_Ptr<Player>& player, ChatInfo::EndChatInfo &ret)
{
	if(!player->GetGameZone())
	{
		LOG_WARNING(FILEINFO, "chat but player gamezone is null");
		return;
	}

	Smart_Ptr<SvrItem> gtSvr;

	std::vector<Smart_Ptr<Player> > vecPlayer;
	player->GetGameZone()->GetAllPlayers(vecPlayer);

	 ServerConHandler::GetInstance()->GetGateServer(gtSvr);

	if((bool)gtSvr)
	{

		for (uint i = 0; i < vecPlayer.size(); ++i)
		{
			if ((bool)vecPlayer[i])
			{
//				ret.set_charid(vecPlayer[i]->GetID());
				QuickMessage::GetInstance()->AddSendMessage(vecPlayer[i]->GetID(), vecPlayer[i]->GetChannelID(),&ret,MSG_SIM_GT2C_CHAT);
//				Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_WS2GT_CHAT, &ret, gtSvr.Get(), SimpleMessage);
//				Message_Facade::Send(messRet);
			}
		}
	}
}
void ChatMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_WORLDCHATVITALITY);
	DEF_SWITCH_TRY_DISPATCH_END
}

void ChatMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_CHAT);
	DEF_SWITCH_TRY_DISPATCH_END
}

void ChatMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GT2WS_CHAT);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(ChatMgr, MSG_REQ_GT2GM_CHAT)
{
	ChatInfo::StartChatInfo reqGM;
	ServerReturn::ServerRetInt retGT;

	int len = 0;
	char * content = message->GetBuffer(len);
	reqGM.ParseFromArray(content, len);

	Smart_Ptr<Player> player;
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID,player);
	retGT.set_ret(0);
	if((bool)player)
	{
		//指令
		if(OrderMgr::GetInstance()->OrderPlayer(message, player,reqGM.str()))return;

		if(!player->IsCanTaling()||!player->IsOpenSystem(eStChatGeneralLv))
		{
			return;
		}

		if (!IsCDEnd(player, reqGM.channel()))
		{
			retGT.set_ret(eChatCDTime);
			Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_CHAT, message, &retGT);
			Message_Facade::Send(messRet);
			return;
		}

		ChatInfo::EndChatInfo end;

		end.set_channel(reqGM.channel());
		end.set_str(reqGM.str());
		for(int i = 0; i < reqGM.item_size() ; i++)
		{
			CharLogin::ClientItemInfo *item = end.add_item();
			item->CopyFrom(reqGM.item(i));
		}
		end.set_startcharid(charID);
		end.set_endname(reqGM.name());
		end.set_endcharid(PlayerMessage::GetInstance()->GetCharidByName(reqGM.name()));
		end.set_startname(player->GetName());
		end.set_viplv(player->getAttManage()->getValue<int>(eVIPLv));

		switch(reqGM.channel())
		{
			case eChatOrdinary:
			{
				OrdinaryChat(player, end);
			}
			break;

			case eChatHorn:
			{
				if(!reqGM.has_param())
				{
					retGT.set_ret(eItemError);
					break;
				}

				const ItemTypeData *pItem =	GoodsLoader::GetInstance()->GetItemDataByID(reqGM.param());
				if(pItem == NULL || pItem->effect != eMegaphoneProp)
				{
					retGT.set_ret(eItemError);
					break;
				}

				BaseArgs deleArg;
				deleArg.GoodsID = pItem->itemID;
				deleArg.Num = 1;
				deleArg.Pos = -1;

				bool isOk = true;
				int res = player->GetContainer(ePackageType)->DeleteGoods(&deleArg);
				if(res != 0)
				{
					isOk = ChatCostGolden(player, pItem->itemID);
				}

				if(res != 0 && !isOk)
				{
					retGT.set_ret(eNotEnogGolden);
				}
				else
				{
					Smart_Ptr<SvrItem> worldSvr = NULL;
					ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()), worldSvr);
					if ((bool)worldSvr)
					{
						Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GT2WS_CHAT, &reqGM, worldSvr.Get(), Request);
						messRet->SetMessageTime(charID);
						Message_Facade::Send(messRet);
					}
				}
				break;
			}
			case eChatRanks:
			{
				if(player->IsHaveGroup())
				{
					Smart_Ptr<GroupInfo> group_ptr = NULL;
					sNewGroupMgr->GetGroupPtr(player->GetGroupID(), group_ptr);

					if(group_ptr)
					{
						list<GroupMember *>::iterator it = group_ptr->memberList.begin();
						for(; it != group_ptr->memberList.end(); ++it)
						{
							if(*it==NULL)
								continue;
							Smart_Ptr<Player> mem_player = NULL;
							PlayerMessage::GetInstance()->GetPlayerByCharid((*it)->memberID, mem_player);
							if(!mem_player)
							{
								return;
							}

							QuickMessage::GetInstance()->AddSendMessage(mem_player->GetID(), mem_player->GetChannelID(),&end,MSG_SIM_GT2C_CHAT);
						}
					}
				}
				else
				{
					retGT.set_ret(eNotGroupMember);
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
	else
	{
		retGT.set_ret(eChatManNull);
	}
	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_CHAT, message, &retGT);
	Message_Facade::Send(messRet);
}

//跑马灯
void ChatMgr::Marquee(EMarqueeChannel type,string paramf, string params ,string  paramt)
{
	LOG_DEBUG(FILEINFO, "gameserver marquee type:%d name:%s target:%s param:%s !",type,paramf.c_str(),params.c_str(),paramt.c_str());
	ChatInfo::Marquee marquee;
	Smart_Ptr<SvrItem> wsSvr;
	string strName = paramf;
	string strTarget = params;
	string strParam = paramt;
	marquee.set_type(type);
	marquee.set_target(strTarget);
	marquee.set_name(strName);
	marquee.set_param(strParam);

	ServerConHandler::GetInstance()->GetWorldServerBySvrID(ServerConHandler::GetInstance()->GetServerId(),wsSvr);
	if((bool)wsSvr)
	{
		if(type == eMarqueeBoss || type == eMarqueeKillGuard )
		{
			const INT64PLAYERMAP playerMap = PlayerMessage::GetInstance()->getAllPlayer();
			INT64PLAYERMAP::const_iterator itr = playerMap.begin();
			for(; itr != playerMap.end(); ++itr)
			{
				if(itr->second.Get()==NULL)
					continue;
				QuickMessage::GetInstance()->AddSendMessage(itr->second->GetID(), itr->second->GetChannelID(),&marquee,MSG_SIM_GT2C_MARQUEE);
			}
		}
		else
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_MARQUEE, &marquee, wsSvr.Get(), SimpleMessage);
			Message_Facade::Send(messRet);
		}
	}
}

DEF_MSG_ACK_DEFINE_FUN(ChatMgr, MSG_REQ_GT2WS_CHAT)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request WS chat and ack timeout");
		return;
	}
}

//组队系统消息
void ChatMgr::chatRanksSystem(const string &str , const int64 &RanksId)
{
	ChatInfo::EndChatInfo ret;
	ret.set_str(str);
	string system = InternationalLoader::GetInstance()->GetInterStr(1);
	ret.set_startname(system.c_str());
	ret.set_channel(eChatRanks);
	ret.set_startcharid(-1);

	Smart_Ptr<GroupInfo> group_ptr = NULL;

	sNewGroupMgr->GetGroupPtr(RanksId, group_ptr);

	if(group_ptr)
	{
		list<GroupMember *>::iterator it = group_ptr->memberList.begin();
		for(; it != group_ptr->memberList.end(); ++it)
		{
			if(*it==NULL)
				continue;
			Smart_Ptr<Player> mem_player = NULL;
			PlayerMessage::GetInstance()->GetPlayerByCharid((*it)->memberID, mem_player);
			if(!mem_player)
			{
				return;
			}

			QuickMessage::GetInstance()->AddSendMessage(mem_player->GetID(), mem_player->GetChannelID(),&ret,MSG_SIM_GT2C_CHAT);
		}
	}
}

//int转化string
string ChatMgr::GetStringByInt(int value)
{
	std::stringstream ss;
	ss<<value;
	return ss.str();
}

DEF_MSG_SIMPLE_DEFINE_FUN(ChatMgr, MSG_SIM_WS2GM_WORLDCHATVITALITY)
{
	ServerReturn::ServerRetInt ret;
	int len = 0;
	char* content = NULL;
	len = message->GetLength();
	content = message->GetBuffer(len);

	ret.ParseFromArray(content, len);

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player, ret.ret());

	if ((bool)player)
	{
		player->GetVitalityMgr()->FinshVitality(eViChatWorld);
	}
}

void ChatMgr::WorldChat(int64 charid,ChatInfo::EndChatInfo &end)
{
	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player,charid);
	if(player)
	{
		QuickMessage::GetInstance()->AddSendMessage(player->GetID(), player->GetChannelID(),&end,MSG_SIM_GT2C_CHAT);
	}
}

bool ChatMgr::ChatCostGolden(Smart_Ptr<Player> player, int propID)
{
	MallGoodsInfo info;
	MallLoader::GetInstance()->GetItemTypeDataByID(propID,MallType_Common,info);

	if(info.id <= 0)
	{
		return false;
	}

	int price = info.price;

	Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
	if(vip)
	{
		price = PlayerVipPart::CalBuyCostGolden(vip, price);
	}

	if(price > player->getAttManage()->getValue<int>(eCharGolden))
	{
		return false;
	}

	player->ChangeGolden(price, false);

	return true;
}


