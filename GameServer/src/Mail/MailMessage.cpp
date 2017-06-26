/*
 * MailMessage.cpp
 *
 *  Created on: 2014年6月4日
 *      Author: root
 */

#include "MailMessage.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "Log/Logger.h"
#include "MessageStruct/CharLogin/Mail.pb.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "MessageStruct/ServerReturn4Int.pb.h"

#include "CharDefine.h"
#include "MessageCommonRet.h"
#include "FightDefined.h"
#include "Memory/MemAllocator.h"

#include "../ConfigLoader/MaskWordConfig.h"
#include "./FileLoader/InternationalLoader.h"
#include "../MsgCommonClass.h"

#include "FileLoader/GameParaLoader.h"
#include <set>
#include "../Attribute/AttributeCreature.h"
#include "../Dependence/Ciconv/Ciconv.h"
#include "../StatisticMgr/StatisticHandle.h"

MailMessageMgr::MailMessageMgr()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(MailMessageMgr, MSG_REQ_C2GM_MAILS);		//请求邮件基础信息列表
	DEF_MSG_QUICK_REQUEST_REG_FUN(MailMessageMgr, MSG_REQ_C2GM_MAILINFO);		//请求邮件详细信息
	DEF_MSG_QUICK_REQUEST_REG_FUN(MailMessageMgr, MSG_REQ_C2GM_DELETEMAIL);		//请求删除邮件
	DEF_MSG_QUICK_REQUEST_REG_FUN(MailMessageMgr, MSG_REQ_C2GM_GETMAILGOODS);		//请求提取邮件物品
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2DB_MAILS);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2DB_DELETEMAIL);
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2DB_GETMAILGOODS);
}

MailMessageMgr::~MailMessageMgr()
{

}

void MailMessageMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

void MailMessageMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}
void MailMessageMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_MAILS);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_MAILINFO);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_DELETEMAIL);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_GETMAILGOODS);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MailMessageMgr,MSG_REQ_C2GM_MAILS)
{
	LOG_DEBUG(FILEINFO, "client req gt get mails");
	ServerReturn::ServerRetInt req;

	req.set_ret(player->GetID());

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2DB_MAILS, &req, player->GetDBChannelID(), Request);
	messRet->SetAct(new requestAct(message));
	Message_Facade::Send(messRet);
}
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MailMessageMgr,MSG_REQ_C2GM_MAILINFO)
{
	LOG_DEBUG(FILEINFO, "client req gt get mailInfo");
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerDoubleInt reqDB;

	req.ParseFromString(content);
	reqDB.set_retf(player->GetID());
	reqDB.set_rets(req.ret());

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2DB_MAILINFO, &reqDB, player->GetDBChannelID(), Request);
	messRet->SetAct(new requestAct(message));
	Message_Facade::Send(messRet);
}
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MailMessageMgr,MSG_REQ_C2GM_DELETEMAIL)
{
	LOG_DEBUG(FILEINFO, "client req delete mail");
	ServerReturn::ServerRetInt req;
	ServerReturn::ServerDoubleInt reqDB;

	req.ParseFromString(content);
	reqDB.set_retf(player->GetID());
	reqDB.set_rets(req.ret());

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2DB_DELETEMAIL, &reqDB, player->GetDBChannelID(), Request);
	messRet->SetAct(new requestAct(message));
	Message_Facade::Send(messRet);
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MailMessageMgr,MSG_REQ_C2GM_GETMAILGOODS)
{
	LOG_DEBUG(FILEINFO, "client req gt get mail goods");
	MailInfo::ReqGoodsInfo req;
	ServerReturn::ServerFourInt reqDB;

	req.ParseFromString(content);
	reqDB.set_retf(player->GetID());
	reqDB.set_rets(player->GetContainer(ePackageType)->GetFreeSize());
	reqDB.set_rett(req.mailid());
	reqDB.set_retfo(req.type());

	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2DB_GETMAILGOODS, &reqDB, player->GetDBChannelID(), Request);
	messRet->SetAct(new requestAct(message));
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(MailMessageMgr, MSG_REQ_GM2DB_MAILS)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request dbserver get mails and ack timeout");
		return;
	}
	LOG_WARNING(FILEINFO, "gameserver request dbserver get mails and ack");
	MailInfo::MailItems ret;
	int len = 0;

	char * content = message->GetBuffer(len);
	ret.ParseFromArray(content, len);

	REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &ret, MSG_REQ_C2GM_MAILS);
}

DEF_MSG_ACK_DEFINE_FUN(MailMessageMgr, MSG_REQ_GM2DB_MAILINFO)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request dbserver get mailinfo and ack timeout");
		return;
	}

	LOG_DEBUG(FILEINFO, "gameserver ret gt send mail ack");
	ServerReturn::ServerDoubleInt info;
	int len = 0;
	char * content = message->GetBuffer(len);
	info.ParseFromArray(content, len);

	REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &info, MSG_REQ_C2GM_MAILINFO);
}

DEF_MSG_ACK_DEFINE_FUN(MailMessageMgr, MSG_REQ_GM2DB_DELETEMAIL)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request dbserver delete mail and ack timeout");
		return;
	}
	LOG_DEBUG(FILEINFO, "gameserver ret gt delete mail ack");
	ServerReturn::ServerThreeInt retGm;
	ServerReturn::ServerDoubleInt ret;
	int len = 0;

	char * content = message->GetBuffer(len);
	retGm.ParseFromArray(content, len);
	ret.set_retf(retGm.rett());
	ret.set_rets(retGm.rets());

	LOG_DEBUG(FILEINFO, "gameserver ret gt delete mail ack back ret = %lld mailId = %lld " , retGm.rett(),retGm.rets());

	REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &ret, MSG_REQ_C2GM_DELETEMAIL);
}

DEF_MSG_ACK_DEFINE_FUN(MailMessageMgr, MSG_REQ_GM2DB_GETMAILGOODS)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request dbserver get mail goods and ack timeout");
		return;
	}
	LOG_DEBUG(FILEINFO, "gameserver ret gt get mail goods ack");

	MailInfo::RetGoodsInfo retGm;
	MailInfo::ResGoodsInfo ret;
	int len = 0;
	vector<DWORD> tItemList;
	vector<DWORD> tItemNum;
	vector<WORD>  tBindTypeList;
	vector<DWORD> vecLock;
	Smart_Ptr<Player> player;

	char * content = message->GetBuffer(len);
	retGm.ParseFromArray(content, len);
	ret = retGm.ret();

	if(ret.ret()==0)
	{
		PlayerMessage::GetInstance()->GetPlayerByCharid(retGm.charid(), player);
		if(!player)
			return;

		BEHAVIOR_MONI(player, BehaviorType::Main::eMail, BehaviorType::Minor::eMail_GetItem, MONITOR);

		for(int i = 0; i < retGm.itemlist().itemlist_size();++i)
		{
			tItemList.push_back(retGm.itemlist().itemlist(i).itemid());
			tItemNum.push_back(retGm.itemlist().itemlist(i).num());
		}

		tBindTypeList.resize(tItemList.size(),1);

		vector<DWORD> strengthLvVec;
		strengthLvVec.resize(tItemList.size(), 0);

		if(!player->GetContainer(ePackageType)->IsBagEnought(tItemList,tItemNum,tBindTypeList))
		{
			vector<int>	  tempPosList;
			player->GetContainer(ePackageType)->AddItem(tItemList,tItemNum,tempPosList,tBindTypeList,strengthLvVec,true);
		}
	}

	REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &ret, MSG_REQ_C2GM_GETMAILGOODS);

	//统计提取邮件奖励次数
//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(player, eStatic_Mail,	eStaMinor_Mail_GetItem);
}
//系统邮件 goods低32位 物品ID 高32 数量
void MailMessageMgr::SendSysMail(vector<int64> &goods,string &name,int64 charID,string substance, string theme,	int money, int exp, int force, int bindGolden)
{
	int count = 0;
	MailInfo::SimDBSendMail reqDB;
	if(charID==0 && name.compare("")==0)
	{
			LOG_ERROR(FILEINFO, "sys mail send error,cannot find player!");
			return;
	}

	if(charID==0 && name.compare("")!=0)
	{
			charID = PlayerMessage::GetInstance()->GetCharidByName(name);
	}

	if(charID==0)
	{
			LOG_ERROR(FILEINFO, "sys mail send error,cannot find player,charid=0 ");
			return;
	}

	string system = InternationalLoader::GetInstance()->GetInterStr(1);
	reqDB.set_sendname(system.c_str());
	reqDB.set_charid(GET_PLAYER_CHARID(charID));
	if(0 == theme.compare("系统邮件"))
	{
		string warning = InternationalLoader::GetInstance()->GetInterStr(4);
		theme = warning;
	}
	reqDB.set_theme(theme);
	reqDB.set_substance(substance);
	reqDB.set_endname(name);

	MailInfo::SpecialMailItemList * specialItemList = reqDB.mutable_itemlist();

	map<int64, int> goodsNum;
	if(money > 0)
	{
		goodsNum[GameParaLoader::GetInstance()->getExchangeMoneyItemId()] += money;
	}

	if(exp > 0)
	{
		goodsNum[GameParaLoader::GetInstance()->getExchangeExpItemId()] += exp;
	}

	if(force > 0)
	{
		goodsNum[GameParaLoader::GetInstance()->getExchangeForceItemId()] += force;
	}

	if(bindGolden > 0)
	{
		goodsNum[GameParaLoader::GetInstance()->getExchangeBindGoldenItemId()] += bindGolden;
	}

	for (uint i = 0; i < goods.size(); ++i)
	{
		int itemID = goods[i] & 0xffffffff;

		int64 key = itemID;
		goodsNum[key] += (goods[i]>>32) & 0xffffffff;
	}

	for(map<int64, int>::iterator it = goodsNum.begin(); it != goodsNum.end() ; )
	{
		map<int64, int>::iterator tempIt = it;
		int itemID = tempIt->first & 0xFFFFFFFF;
		int num = tempIt->second;
		if (0 >= tempIt->second)
		{
			++it;
			continue;
		}

		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(itemID);
		if(ginfo == NULL)
		{
			return;
		}

		MailInfo::SpecialItem * specialItem = specialItemList->add_itemlist();
		specialItem->set_itemid(itemID);
		specialItem->set_num(num);

		++count;

		if (++it == goodsNum.end() || 4 == count)
		{
			Smart_Ptr<SvrItem> dbSvr;
			ServerConHandler::GetInstance()->GetDBServerBySvrID(GET_SERVER_CHARID(charID), dbSvr);
			if ((bool)dbSvr)
			{
				Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2DB_SENDMAIL, &reqDB, dbSvr.Get(),SimpleMessage);
				Message_Facade::Send(messRet);
			}
			else
			{
				LOG_ERROR(FILEINFO, "SendSysMail Send DBSvr Error, Not DBSvr ");
			}
			reqDB.mutable_itemlist()->clear_itemlist();
			count = 0;
		}
	}

	if(reqDB.itemlist().itemlist_size() > 0)
	{
		Smart_Ptr<SvrItem> dbSvr;
		ServerConHandler::GetInstance()->GetDBServerBySvrID(GET_SERVER_CHARID(charID), dbSvr);
		if ((bool)dbSvr)
		{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2DB_SENDMAIL, &reqDB, dbSvr.Get(),SimpleMessage);
			Message_Facade::Send(messRet);
		}
	}
}

void MailMessageMgr::SendMailByCharId(int64 charid,string substance, string theme,MailInfo::SpecialMailItemList * specialItemInfo)
{
	if(charid <= 0)
		return;

	if(specialItemInfo == NULL)
		return;

	string name="";
	MailInfo::SimDBSendMail reqDB;

	string system = InternationalLoader::GetInstance()->GetInterStr(1);

	if(theme.compare("系统邮件") == 0)
	{
		theme = InternationalLoader::GetInstance()->GetInterStr(4);
	}

	reqDB.set_sendname(system.c_str());
	reqDB.set_charid(GET_PLAYER_CHARID(charid));
	reqDB.set_theme(theme);
	reqDB.set_substance(substance);
	reqDB.set_endname(name);

	MailInfo::SpecialMailItemList * specialItemList = reqDB.mutable_itemlist();
	for(int i=0; i<specialItemInfo->itemlist_size(); ++i)
	{
		const ::MailInfo::SpecialItem& ref = specialItemInfo->itemlist(i);
		MailInfo::SpecialItem * specialItem = specialItemList->add_itemlist();
		specialItem->CopyFrom(ref);
	}

	Smart_Ptr<SvrItem> dbSvr;
	ServerConHandler::GetInstance()->GetDBServerBySvrID(GET_SERVER_CHARID(charid), dbSvr);
	if(dbSvr.Get()!=NULL)
	{
			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2DB_SENDMAIL, &reqDB, dbSvr.Get(),SimpleMessage);
			Message_Facade::Send(messRet);
	}
	return;
}


void MailMessageMgr::SendMail(int64 charid, const vector<PropItem>& items, string substance, string theme)
{
//		MailInfo::GoodsInfo tempGoods;
		MailInfo::SpecialMailItemList tempSpecial;
		int mailCount=0;

		for(size_t i = 0; i < items.size(); ++i)
		{
//			int tempType = GoodsLoader::GetInstance()->GetSpecliItemFunPlayerAttr(items[i].id);
//			//物品
//			if(tempType == 0)
//			{
//				const GoodsInfo* tempGoodsInfo = GoodsLoader::GetInstance()->GetItemDataByID(items[i].id);
//				if(tempGoodsInfo == NULL)
//					continue;
//
//				CharLogin::ClientItemInfo* pItem = tempGoods.add_item();
//				uint flag=(items[i].bind<<16)|((items[i].num) & 0xff);
//				pItem->set_id(items[i].id);
//				pItem->set_flag(flag);
//			}
//			//钱
//			else
			{
				MailInfo::SpecialItem* pItem = tempSpecial.add_itemlist();
				pItem->set_itemid(items[i].id);
				pItem->set_num(items[i].num);
			}

			mailCount++;
			if(mailCount>=4)
			{
				mailCount = 0;
				SendMailByCharId(charid, substance, theme, &tempSpecial);
//				tempGoods.Clear();
				tempSpecial.Clear();
			}
		}



		if(tempSpecial.itemlist_size()>0)
		{
			SendMailByCharId(charid, substance, theme, &tempSpecial);
		}
}
