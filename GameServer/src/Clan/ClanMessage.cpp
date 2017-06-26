/*
 * ClanMessage.cpp
 *
 *  Created on: 2014年9月30日
 *      Author: helh
 */

#include "ClanMessage.h"
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "MessageStruct/ServerReturn4Int.pb.h"
#include "MessageStruct/ServerReturn5Int.pb.h"
#include "../ServerManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "./ClanLoader/ClanLoader.h"
#include "MessageCommonRet.h"
#include "CharDefine.h"
#include "../QuickMessage.h"
#include "../Map/SceneManager.h"
#include "../EventSet/EventDefine.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/Clan/Clan.pb.h"
#include "FightDefined.h"
#include "../Fighting/Fighting.h"
#include "FileLoader/ActivitiesTimerLoader.h"
#include "../MapCarbon/RobberCarbon.h"
#include "../Attribute/AttributeManage.h"
#include "MessageStruct/Clan/ClanIntegral.pb.h"
#include "../Map/DuplicateMgr.h"
#include "../Map/GameDuplicate.h"
#include "FileLoader/ClanWelfareLoader.h"
#include "../MapCarbon/ClanCarbon.h"
#include "../Mail/MailMessage.h"
#include "FileLoader/GodTreeFile.h"
#include "../Object/FileLoader/NPCLoader.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "MessageStruct/Clan/GodTreepro.pb.h"
#include "../StatisticMgr/StatisticHandle.h"
#include "StatisticBase/StatisticDefine.h"

ClanMessage::ClanMessage()
{
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_TACK_OFF);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_CHANGE_ATTR);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_COST_CONTRIBUTION);
	DEF_MSG_QUICK_REQUEST_REG_FUN(ClanMessage, MSG_REQ_C2GM_SHOP_GOODS);//创建副本
	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2WS_SHOP_SUCCESS);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_CLANWARBEGIN);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_REQ_WS2GM_SYNWILLJOIN);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_INTEGRAL_SYSRESINGUP);//积分战补报扣元宝
	DEF_MSG_QUICK_REQUEST_REG_FUN(ClanMessage, MSG_REQ_C2GM_INTEGRAL_KILLRANK_INFO);  //查看击杀排名

	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_CLANWELFARECOST);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_WS2GM_TAKECLANWELFARE);

	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GS_DESTROY_CLAN_MAP);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GS_ACTIVE_START_END);
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_MAKE_GODTREE_ITEM);	
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_W2GM_BUCKLE_ITEM);
	DEF_MSG_QUICK_REQUEST_REG_FUN(ClanMessage, MSG_REQ_C2GS_OCCUPY_FLAG);//请求占领旗子

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GS2WS_FACTION_NAME);

}

ClanMessage::~ClanMessage()
{

}

void ClanMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_CLANWARBEGIN);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_REQ_WS2GM_SYNWILLJOIN);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GS_DESTROY_CLAN_MAP);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GS_ACTIVE_START_END);
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_MAKE_GODTREE_ITEM);

	DEF_SWITCH_TRY_DISPATCH_END
}

void ClanMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_TACK_OFF);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_CHANGE_ATTR);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_COST_CONTRIBUTION);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_INTEGRAL_SYSRESINGUP);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_CLANWELFARECOST);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_WS2GM_TAKECLANWELFARE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_W2GM_BUCKLE_ITEM);

	DEF_SWITCH_TRY_DISPATCH_END
}



void ClanMessage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2WS_SHOP_SUCCESS);
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GS2WS_FACTION_NAME);
	DEF_SWITCH_TRY_DISPATCH_END
}

void ClanMessage::DeleteAttr(const Smart_Ptr<Player> & player, int lv)
{
	if(lv <= 0)
		return;

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttClan);

	for(int i = 1;i <= lv;++i)
	{
		UpdateInfo * info = ClanLoader::GetInstance()->GetUpdateInfo(i);
		if(info == 0)
			continue;
		player->addAtt(eCharHPMax,info->hp, false,tempValueType);
		player->addAtt(eCharPhyAttackMax,info->pattack, false,tempValueType);
		player->addAtt(eCharPhyDefence,info->pdefence, false,tempValueType);
		player->addAtt(eCharHitRate,info->hrate, false,tempValueType);
		player->addAtt(eCharAvoidRate,info->avoid, false,tempValueType);
		player->addAtt(eCrackDownRate,info->crack, false,tempValueType);
		player->addAtt(eAvoidCrackRate,info->acrack, false,tempValueType);
	}

	calculateClanFight(0,player);

	return ;
}

void ClanMessage::AddAttr(const Smart_Ptr<Player> & player, int blv, int elv,bool isSendEvend)
{
	if(blv == elv)
	{
		return;
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttClan);

	//升级
	if(blv < elv)
	{
		for(int i=blv+1; i<=elv; ++i)
		{
			UpdateInfo * info = ClanLoader::GetInstance()->GetUpdateInfo(i);
			if(info == 0)
				continue;

			player->addAtt(eCharHPMax,info->hp, true,tempValueType);
			player->addAtt(eCharPhyAttackMax,info->pattack, true,tempValueType);
			player->addAtt(eCharPhyDefence,info->pdefence, true,tempValueType);
			player->addAtt(eCharHitRate,info->hrate, true,tempValueType);
			player->addAtt(eCharAvoidRate,info->avoid, true,tempValueType);
			player->addAtt(eCrackDownRate,info->crack, true,tempValueType);
			player->addAtt(eAvoidCrackRate,info->acrack, true,tempValueType);
		}
	}
	else//降级
	{
		for(int i=blv; i>elv; --i)
		{
			UpdateInfo * info = ClanLoader::GetInstance()->GetUpdateInfo(i);
			if(info == 0)
				continue;

			player->addAtt(eCharHPMax,info->hp, false,tempValueType);
			player->addAtt(eCharPhyAttackMax,info->pattack, false,tempValueType);
			player->addAtt(eCharPhyDefence,info->pdefence, false,tempValueType);
			player->addAtt(eCharHitRate,info->hrate, false,tempValueType);
			player->addAtt(eCharAvoidRate,info->avoid, false,tempValueType);
			player->addAtt(eCrackDownRate,info->crack, false,tempValueType);
			player->addAtt(eAvoidCrackRate,info->acrack, false,tempValueType);
		}
	}

	calculateClanFight(elv,player);

	if(!isSendEvend)
		return ;
}

void ClanMessage::onlineCalculateAtt(Smart_Ptr<Player>& player,BYTE currentLevel)
{
	map<ECharAttrType, int64> tempAttList;
	for(int i=1; i <= currentLevel;++i)
	{
		UpdateInfo * info = ClanLoader::GetInstance()->GetUpdateInfo(i);
		if(info == 0)
			continue;

		tempAttList[eCharHPMax] 	   += info->hp;
		tempAttList[eCharPhyAttackMax] += info->pattack;
		tempAttList[eCharPhyDefence]   += info->pdefence;
		tempAttList[eCharHitRate] 	   += info->hrate;
		tempAttList[eCharAvoidRate]    += info->avoid;
		tempAttList[eCrackDownRate]    += info->crack;
		tempAttList[eAvoidCrackRate]   += info->acrack;
	}

	eAttValueType tempValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttClan);

	player->calculateResultAddAtt(tempAttList,tempValueType);

	calculateClanFight(currentLevel,player);

	return ;
}

//计算等级加的战斗力
int ClanMessage::calculateClanFight(BYTE level,const Smart_Ptr<Player>& player)
{
	map<ECharAttrType, int64> tempAttList;
	for(int i=1; i<=level; ++i)
	{
		UpdateInfo * info = ClanLoader::GetInstance()->GetUpdateInfo(i);
		if(info == 0)
			continue;

		tempAttList[eCharHPMax] 	   += info->hp;
		tempAttList[eCharPhyAttackMax] += info->pattack;
		tempAttList[eCharPhyDefence]   += info->pdefence;
		tempAttList[eCharHitRate] 	   += info->hrate;
		tempAttList[eCharAvoidRate]    += info->avoid;
		tempAttList[eCrackDownRate]    += info->crack;
		tempAttList[eAvoidCrackRate]   += info->acrack;
	}

	int tempValue = player->getFightingManage()->CalculateFighting(tempAttList);
	return player->getFightingManage()->setFighting(eFightingClan,tempValue);
}

int ClanMessage::CreateClanByItem(const Smart_Ptr<Player> &player)
{
	//获取背包中是否有这个物品
	int pos = ClanLoader::GetInstance()->getCreateClanItemId();
	Smart_Ptr<PropBase>  stone = player->GetContainer(ePackageType)->GetItemDataPtrByID(pos);
	if(!stone)
		return eNotFindItem;

	player->GetContainer(ePackageType)->SubmitGrideItem(pos);  ///*************************
	return 0;
}

void ClanMessage::clanDataAlter(Smart_Ptr<Player>& player,ClanPackage::SynchAttrToGM& data,vector<int>& selfAtt,vector<int>& broadcastAtt,bool isSend)
{
	int tempJob = player->getAttManage()->getValue<BYTE>(eCharFaJob);
	if(data.id() != player->getAttManage()->getValue<DWORD>(eCharFaction))
	{
		player->getAttManage()->setValue(eCharFaction,data.id());

		selfAtt.push_back(eCharFaction);
		broadcastAtt.push_back(eCharFaction);

		tempJob = data.job();
		player->getAttManage()->setValue(eCharFaJob,tempJob);

		selfAtt.push_back(eCharFaJob);
		broadcastAtt.push_back(eCharFaJob);
	}
	else if(data.job() != player->getAttManage()->getValue<BYTE>(eCharFaJob))
	{
		tempJob = data.job();
		player->getAttManage()->setValue(eCharFaJob,tempJob);

		selfAtt.push_back(eCharFaJob);
		broadcastAtt.push_back(eCharFaJob);
	}

	if(player->GetClanName() != data.name())
	{
		player->SetClanName(data.name());
		if(isSend)
		{
			ClanPackage::ClanNameChange toClient;

			toClient.set_name(player->GetClanName());
			toClient.set_key(player->GetKey());

			CSceneMgr::GetInstance()->SendSynchMessage(&toClient, MSG_SIM_GM2C_SYN_CLANNAME, player->GetMapID(), player->GetKey(),
																					player->GetXNewPos(), player->GetYNewPos(), true);
		}
	}

	if(data.clanicon() != player->getAttManage()->getValue<BYTE>(eCharClanIcon))
	{
		player->getAttManage()->setValue(eCharClanIcon,data.clanicon());

		selfAtt.push_back(eCharClanIcon);
		broadcastAtt.push_back(eCharClanIcon);
	}

	if(data.clancontribution() != player->getAttManage()->getValue<int>(eFaContribution))
	{
		player->getAttManage()->setValue(eFaContribution,data.clancontribution());

		selfAtt.push_back(eFaContribution);
		broadcastAtt.push_back(eFaContribution);
	}

	return ;
}

void ClanMessage::clearClanData(Smart_Ptr<Player>& player,vector<int>& selfAtt,vector<int>& broadcastAtt,bool isSend)
{
	player->getAttManage()->setValue(eCharFaction,0);
	player->getAttManage()->setValue(eCharFaJob,0);
	player->SetClanName("");
	player->getAttManage()->setValue(eCharClanIcon,0);

	selfAtt.push_back(eCharFaction);
	broadcastAtt.push_back(eCharFaction);

	selfAtt.push_back(eCharFaJob);
	broadcastAtt.push_back(eCharFaJob);

	selfAtt.push_back(eCharClanIcon);
	broadcastAtt.push_back(eCharClanIcon);

	if(isSend)
	{
		ClanPackage::ClanNameChange toClient;

		toClient.set_name(player->GetClanName());
		toClient.set_key(player->GetKey());

		CSceneMgr::GetInstance()->SendSynchMessage(&toClient, MSG_SIM_GM2C_SYN_CLANNAME, player->GetMapID(), player->GetKey(),
																				player->GetXNewPos(), player->GetYNewPos(), true);
	}

	return ;
}

void ClanMessage::KickOutClanMap(Smart_Ptr<Player> &player)
{
	CGameZone * pZone = player->GetGameZone();
	if(pZone == NULL)
	{
		return;
	}

	if(GET_MAP_TYPE(pZone->GetSceneID()) != ePVEMap)
	{
		return;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(pZone->GetSceneID()));
	if(entry==NULL || (entry->m_eType != eClanRobber && entry->m_eType != eClanBattle))
	{
		return;
	}

	int64 newZone = 0;
	int xpos = 0;
	int ypos = 0;

	OldMapInfo *pOldMap = ((CGameDuplicate*)pZone)->GetOldMapPoint(player->GetID());
	if(pOldMap == NULL || (pOldMap != NULL && GET_MAP_TYPE(pOldMap->mapid) >= eCopyMap))
	{
		MapStaticInfo *sMapInfo = CMapConfig::GetInstance()->GetStaticInfo(GET_MAP_ID(pZone->GetSceneID()));
		if(sMapInfo==NULL)
				return;

		newZone = CREATE_MAP(eStaticMap, 1, 1, sMapInfo->reliveMap, 1);
		xpos = sMapInfo->reliveXPos;
		ypos = sMapInfo->reliveYPos;
	}
	else
	{
		newZone = pOldMap->mapid;
		xpos = pOldMap->xpos;
		ypos = pOldMap->ypos;
	}

	PlayerMessage::GetInstance()->ForceChangeMap(player, newZone, xpos, ypos);
}

DEF_MSG_REQUEST_DEFINE_FUN(ClanMessage, MSG_REQ_WS2GM_TACK_OFF)
{
	ClanPackage::WorldReqTakeOff meContent;
	ServerReturn::ServerRetInt toWs;
	char *con ;
	int len = 0;
	Smart_Ptr<Player> player;
	int ret = 0;
	vector<int> vec;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "player[%lld] in worldserver request take off by creating clan", GET_PLAYER_CHARID(meContent.id()));

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.id(), player);
	if(player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eClan, BehaviorType::Minor::eClan_CreateGMCostGolden, MONITOR);
		int account = ClanLoader::GetInstance()->GetCreateCost(false);
		int pos = ClanLoader::GetInstance()->getCreateClanItemId();

		if(player->IsOpenSystem(eStClan))
		{
			if(player->getAttManage()->getValue<int>(eCharMoney) < account)
				ret = eNotEnoghMoney;

			Smart_Ptr<PropBase> stone = player->GetContainer(ePackageType)->GetItemDataPtrByID(pos);
			if(!stone)
				ret = eNotFindItem;
		}
		else
		{
			ret = eNotOpen;
		}

		if(!ret)
		{
			player->ChangeMoney(account, false);
			vec.push_back(eCharMoney);

			player->GetContainer(ePackageType)->SubmitGrideItem(pos);  ///*************************
		}

		
	}
	else
	{
		return;
	}

	if(vec.size() > 0)
		player->SynCharAttribute(vec);

	toWs.set_ret(ret);

	Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_REQ_WS2GM_TACK_OFF, message, &toWs);
	Message_Facade::Send(clientRet);
}


DEF_MSG_REQUEST_DEFINE_FUN(ClanMessage, MSG_REQ_WS2GM_CHANGE_ATTR)
{
	ClanPackage::SynchAttrToGM meContent;
	ServerReturn::ServerFourInt toWs;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "worldserver request GM to Change player[%lld] attr", GET_PLAYER_CHARID(meContent.charid()));

	bool tempIsCache = false;
	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if(!player)
	{
		PlayerMessage::GetInstance()->GetPlayerCacheByCharid(meContent.charid(), player);
		if(player)
			tempIsCache = true;
	}

	if(!player)
		return ;

	if(!player->IsOpenSystem(eStClan))
		return;

	vector<int> broadCast;
	vector<int> vec;

	DWORD originalClanID = player->getAttManage()->getValue<DWORD>(eCharFaction);

	if(meContent.id() != 0)
	{
		clanDataAlter(player,meContent,vec,broadCast,!tempIsCache);

		//存在增加或减少属性的
		AddAttr(player, meContent.blv(), meContent.elv());

		if(meContent.elv() > meContent.blv())
		{
			if(!tempIsCache)
			{
				if (player->GetMyself())
				{
					ClanUpgradeArgs e(player->GetMyself(),meContent.elv());
					player->FireEvent(PLAYER_CLAN_UPGRADE,e);
				}
			}
		}

		broadCast.push_back(eCharHPMax);
		vec.push_back(eCharHPMax);
		vec.push_back(eCharPhyAttackMax);
		vec.push_back(eCharPhyDefence);
		vec.push_back(eCharHitRate);
		vec.push_back(eCharAvoidRate);
		vec.push_back(eCrackDownRate);
		vec.push_back(eAvoidCrackRate);
		vec.push_back(eFightPower);

		if(originalClanID == 0 && !tempIsCache)
		{
			//触发加入帮派事件
			OneParmaEventArgs args(player);

			FireGlobalEvent(PLAYER_CLAN_ENTERCLAN, args);
			player->FireEvent(PLAYER_CLAN_ENTERCLAN,args);
		}

		if(tempIsCache)
		{
			LOG_ERROR(FILEINFO, "join clan but player[%lld] is in cache", GET_PLAYER_CHARID(player->GetID()));
		}
	}
	else
	{
		/*退出帮派，或者帮派解散了*/
		DeleteAttr(player, meContent.blv());

		//传出帮派场景
		KickOutClanMap(player);

		clearClanData(player,vec,broadCast,!tempIsCache);

		if(!tempIsCache)
		{
			//触发离开帮派事件
			PlayerLeaveClanArgs args(player,originalClanID);

			FireGlobalEvent(PLAYER_CLAN_LEAVECLAN, args);
			player->FireEvent(PLAYER_CLAN_LEAVECLAN,args);
		}

		broadCast.push_back(eCharHPMax);
		vec.push_back(eCharHPMax);
		vec.push_back(eCharPhyAttackMax);
		vec.push_back(eCharPhyDefence);
		vec.push_back(eCharHitRate);
		vec.push_back(eCharAvoidRate);
		vec.push_back(eCrackDownRate);
		vec.push_back(eAvoidCrackRate);
		vec.push_back(eFightPower);
	}

	player->SynCharAttribute(vec);
	player->BroadcastCharAttribute(broadCast,false);

	player->SetDataFlag(eBaseInfo);

	toWs.set_rets(player->getAttManage()->getValue<int>(eFightPower));
	toWs.set_rett(meContent.elv());
	toWs.set_retf(player->GetID());
	toWs.set_retfo(0);

	Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_REQ_WS2GM_CHANGE_ATTR, message, &toWs);
	Message_Facade::Send(clientRet);
}

DEF_MSG_REQUEST_DEFINE_FUN(ClanMessage, MSG_REQ_WS2GM_COST_CONTRIBUTION)
{
	ClanPackage::WorldReqGiveCost meContent;
	ClanPackage::AckGiveCost toWs;
	char *con;
	int len = 0;
	Smart_Ptr<Player> player;
	int ret = 0;
	vector<int> vec;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "player[%lld] in worldserver request clan contribution[%d]", GET_PLAYER_CHARID(meContent.charid()), meContent.money());

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if(player)
	{
		if(player->IsOpenSystem(eStClan))
		{
			if(player->getAttManage()->getValue<int>(eCharMoney) < (int)meContent.money())
			{
				ret = eNotEnoghMoney;
			}
			else
			{
				player->ChangeMoney(meContent.money(), false);
				vec.push_back(eCharMoney);
			}
		}
		else
		{
			ret = eNotOpen;
		}
	}
	else
	{
		return;
	}

	if(vec.size() > 0)
		player->SynCharAttribute(vec);

	toWs.set_ret(ret);
	toWs.set_charid(meContent.charid());
	toWs.set_money(meContent.money());
//	toWs.set_golden(meContent.golden());

	Safe_Smart_Ptr<CommBaseOut::Message> clientRet  = build_message(MSG_REQ_WS2GM_COST_CONTRIBUTION, message, &toWs);
	Message_Facade::Send(clientRet);
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ClanMessage,MSG_REQ_C2GS_OCCUPY_FLAG)
{
	CGameZone *pZone = player->GetGameZone();
	if(pZone==NULL)
	{
		return;
	}

	if(GET_MAP_TYPE(player->GetMapID()) != ePVEMap )
	{
		return;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if(entry==NULL || entry->m_eType != eClanBattle)
	{
		return;
	}

	ClanPackage::AckOccupyFlag ret;
	ClanPackage::ReqOccupyFlag req;
	req.ParseFromString(content);

	//能否占领
	bool isSuccess = ((CClanCarbon*)(pZone))->OccupyFlag(player->GetID(),req.key());
	if(isSuccess)
		ret.set_ret(0);
	else
		ret.set_ret(1);

	ret.set_key( req.key() );
	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_OCCUPY_FLAG);
}

//帮贡兑换物品
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ClanMessage,MSG_REQ_C2GM_SHOP_GOODS)
{
	ClanPackage::ClanGoods req;
	ClanPackage::ClanShop toClient;
	ClanShop clanShop;
	vector<int> vecLock;;
	vector<DWORD> vec;

	int ret = 0;

	req.ParseFromString(content);

	ClanLoader::GetInstance()->GetClanShopById(req.itemid(),clanShop);

	if(player && player->IsOpenSystem(eStClan)==false)
	{
		ret = eNotOpen;
	}
	else if(player->getAttManage()->getValue<DWORD>(eCharFaction) <=  0)
	{
		ret = eClanIstExisted;
	}
	else if(clanShop.nItemId <= 0)
	{
		ret = eNotFindItem;
	}
	else if(player->getAttManage()->getValue<int>(eFaContribution) < clanShop.nPrice * req.limit())
	{
		ret = eNotEnoughFContri;
	}
	else
	{
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(req.itemid());
		if(ginfo == NULL)
		{
				ret = eItemError;
				toClient.set_ret(ret);
				REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_SHOP_GOODS);
				return;
		}

		if(player->GetContainer(ePackageType)->IsBagEnought(req.itemid(), req.limit(),ginfo->getFlag()) != 0)
		{
			ret = ePackageFull;
		}
		else
		{
			ServerReturn::ServerThreeInt reqGT;

			reqGT.set_retf(player->GetID());
			reqGT.set_rets(req.itemid());
			reqGT.set_rett(req.limit());

			Smart_Ptr<SvrItem> worldSvr;
			ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);

			if(worldSvr.Get()!=NULL)
			{
				Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2WS_SHOP_SUCCESS, &reqGT, worldSvr.Get(), Request);
				messRet->SetAct(new requestAct(message));
				Message_Facade::Send(messRet);
			}
		}
	}

	if(ret != 0)
	{
		toClient.set_ret(ret);
		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_SHOP_GOODS);
	}
}

DEF_MSG_ACK_DEFINE_FUN(ClanMessage, MSG_REQ_GM2WS_SHOP_SUCCESS)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "gameserver request wdserver get shop and ack timeout");
		return;
	}

	ServerReturn::ServerFiveInt ret;
	ClanPackage::ClanShop toClient;
	ClanShop clanShop;
	int len = 0;
	int retCode = 0;
	vector<int> vecType;
	Smart_Ptr<Player> player;
	vector<int> vecLock;

	char * content = message->GetBuffer(len);
	ret.ParseFromArray(content, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(ret.retf(), player);
	if(!(bool)player)
		return;

	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(ret.oldrets());

	retCode = ret.rets();

	if(ginfo==NULL)
	{
		retCode = eItemError;
	}

	if(retCode == 0)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eClan, BehaviorType::Minor::eClan_BuyGoods, MONITOR);;

		ClanLoader::GetInstance()->GetClanShopById(ret.oldrets(),clanShop);

		vector<DWORD> goodsIdList;
		vector<DWORD> numList;
		vector<DWORD> strengthLvVec;
		vector<WORD> flagVec;

		goodsIdList.push_back(ginfo->itemID);
		numList.resize(goodsIdList.size(), ret.oldxpos());

		strengthLvVec.resize(goodsIdList.size(), ginfo->equipStrengthLv);
		flagVec.resize(goodsIdList.size(), clanShop.nBindType);

		player->GetContainer(ePackageType)->AddItem(goodsIdList, numList, vecLock,flagVec,strengthLvVec);

//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		for(size_t i = 0;i < goodsIdList.size();++i)
//		{
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,goodsIdList[i],numList[i]);
//		}
//
//		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCostBefore(costItemInfo,eFaContribution,clanShop.nPrice * ret.oldxpos(),player);

		player->ChangeFContri(clanShop.nPrice * ret.oldxpos(),false);

//		StatisticMgr::SetItemCostAfter(costItemInfo,eFaContribution,player);

//		if(eAccountType_User == player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Clan,eStaMinor_Clan_MallBuyEvent,&costAndGotInfo);

		vecType.push_back(eFaContribution);

		player->SynCharAttribute(vecType);

		;
	}

	int tempCounts = ret.oldypos();
	toClient.set_ret(retCode);
	toClient.set_id(ret.oldrets());
	toClient.set_surpluscounts(tempCounts);

	REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &toClient, MSG_REQ_C2GM_SHOP_GOODS);
}

DEF_MSG_ACK_DEFINE_FUN(ClanMessage, MSG_REQ_GS2WS_FACTION_NAME)
{
	if(message->GetErrno() == eReqTimeOut)
	{
		LOG_WARNING(FILEINFO, "MSG_REQ_GS2WS_FACTION_NAME: gameserver request wdserver ack timeout");
		return;
	}

	int64 charid = static_cast<commonAct *>(act.Get())->mesDataf;
	int64 sence = static_cast<commonAct *>(act.Get())->mesDatas;

	ClanPackage::AckGS2WSGetFactionName ack;
	int len = 0;
	char * content = message->GetBuffer(len);
	ack.ParseFromArray(content, len);

	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharid(charid, player);
	if(!player || player->GetMapID() != sence)
	{
		return;
	}

	CGameZone *pZone = player->GetGameZone();
	if(pZone == NULL)
	{
		return;
	}

	ClanPackage::BaseBatterInfo info;
	bool isSuccesse = ((CClanCarbon*)pZone)->GetBaseInfos(player, info);
	if(!isSuccesse)
	{
		return;
	}

	info.set_targetclanname(ack.name());

	player->SendSynchMessage(&info, MSG_SIM_GS2C_CLAN_BATTLE_BASE);
}

DEF_MSG_SIMPLE_DEFINE_FUN(ClanMessage, MSG_SIM_WS2GS_DESTROY_CLAN_MAP)
{
	ClanPackage::Ws2GsDestroyClanMap msg;

	int len = 0;
	char* buffer = message->GetBuffer(len);
	msg.ParseFromArray(buffer, len);

	Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone( msg.mapid() );
	if(zone.Get() == NULL)
	{
		return;
	}

	if(GET_MAP_TYPE(msg.mapid()) != ePVEMap)
	{
		return;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(msg.mapid()));
	if(entry==NULL || entry->m_eType != eClanRobber)
	{
		return;
	}

	((RobberCarbon*)(zone.Get()))->Destroy( msg.clanid() );

}

DEF_MSG_SIMPLE_DEFINE_FUN(ClanMessage, MSG_SIM_WS2GS_ACTIVE_START_END)
{
	ClanPackage::Ws2GsRobberActive msg;

	int len = 0;
	char* buffer = message->GetBuffer(len);
	msg.ParseFromArray(buffer, len);

	for(int i=0; i<msg.info_size(); ++i)
	{
		const ::ClanPackage::ClanMapInfo& ref = msg.info(i);

		Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone( ref.mapid() );
		if(zone.Get() == NULL)
		{
			continue;
		}

		if(GET_MAP_TYPE(ref.mapid()) != ePVEMap)
		{
			continue;
		}

		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(ref.mapid()));
		if(entry==NULL || entry->m_eType != eClanRobber)
		{
			continue;
		}

		((RobberCarbon*)(zone.Get()))->Active( msg.flag(), ref.clanid(), ref.win(), ref.param(), ref.conwin(), ref.confail() );
	}

}

DEF_MSG_SIMPLE_DEFINE_FUN(ClanMessage, MSG_SIM_WS2GM_CLANWARBEGIN)
{

}

DEF_MSG_SIMPLE_DEFINE_FUN(ClanMessage, MSG_REQ_WS2GM_SYNWILLJOIN)
{
	ClanPackage::SynchWillJoinToGM req;
	char* content;
	int len = 0;

	content = message->GetBuffer(len);
	req.ParseFromArray(content, len);


	for (int i = 0; i < req.charid_size(); ++i)
	{
		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharId(player, req.charid(i));
		if ((bool)player)
		{
			player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Faction, eUIHLSmall_Faction_HaveUnHandleJoin), 1, req.flag());
		}
	}
}

DEF_MSG_REQUEST_DEFINE_FUN(ClanMessage, MSG_REQ_WS2GM_INTEGRAL_SYSRESINGUP)
{
	ClanPackage::SysReSingUp meContent;
	Smart_Ptr<Player> player;
	char *con;
	int len = 0;
	int ret = 0;
	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	PlayerMessage::GetInstance()->GetPlayerByCharid(meContent.charid(), player);
	if(player)
	{
		if(player->getAttManage()->getValue<int>(eCharGolden) < meContent.golden())
		{
			ret = eNotEnogGolden;
		}
		else
		{
			player->ChangeGolden(meContent.golden(),false);
		}
	}
	else
	{
		ret = eNotFindThisCharID;
	}

	meContent.set_ret(ret);

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_WS2GM_INTEGRAL_SYSRESINGUP, message, &meContent);
	Message_Facade::Send(messRet);
}

//查看击杀排名
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ClanMessage,MSG_REQ_C2GM_INTEGRAL_KILLRANK_INFO)
{
	ClanPackage::RetKillRankingInfo toClient;
	int ret = 0;
	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if (!entry)
	{
		LOG_DEBUG(FILEINFO,"InstanceMgr::GetAwardToPlayer: intanceId is invalid %d.",GET_MAP_ID(player->GetMapID()));
		return;
	}

	if(entry->m_eType == eIntegralInstance)
	{
		Smart_Ptr<CGameDuplicate> instance = DuplicateMgr::GetInstance()->GetDuplicate(player->GetMapID());
		if(!instance)
		{
			ret = eInstanceNotExsitence;
		}
		else
		{
			ret = instance->GetClanIntegralRanking(player,toClient);
		}
	}
	else
	{
		ret = eMapIdError;
	}

	toClient.set_ret(ret);

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_INTEGRAL_KILLRANK_INFO);
}

DEF_MSG_REQUEST_DEFINE_FUN(ClanMessage, MSG_REQ_WS2GM_CLANWELFARECOST)
{
	int64 charID = message->GetMessageTime();
	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player, charID);

	if (!player)
	{
		return;
	}

	int err = 0;
	ClanPackage::SendClanWelfare req;
	ServerReturn::ServerResultInt ret;
	int len = 0;
	char* buffer = message->GetBuffer(len);
	req.ParseFromArray(buffer, len);

	const ClanWelfareInfo* info = ClanWelfareLoader::GetInstance()->GetClanWelfareInfo(req.welfareinfoid());
	if (NULL == info)
	{
		err = eItemError;
	}
	else
	{
		if (player->getAttManage()->getValue<int>(eCharGolden) < info->price * req.num())
		{
			err = eNotEnoughGold;
		}
		else if (!player->GetContainer(ePackageType))
		{
			err = ePlayerPackageFail;
		}
		else
		{
			player->ChangeGolden(info->price * req.num(), false);
			err = 0;
		}
	}

	ret.set_ret(err);

	Safe_Smart_Ptr<Message> msg = build_message(message, &ret, Ack);
	msg->SetMessageTime(player->GetID());
	Message_Facade::Send(msg);
}

DEF_MSG_REQUEST_DEFINE_FUN(ClanMessage, MSG_REQ_WS2GM_TAKECLANWELFARE)
{
	Smart_Ptr<Player> player;
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharId(player, charID);
	if (!player)
	{
		return;
	}

	ServerReturn::ServerResultInt req;
	int len = 0;
	char* buffer = message->GetBuffer(len);
	req.ParseFromArray(buffer, len);

	int err = 0;

	const ClanWelfareInfo* info = ClanWelfareLoader::GetInstance()->GetClanWelfareInfo(req.ret());
	if (NULL == info)
	{
		err = eItemError;
	}
	else if (!player->GetContainer(ePackageType))
	{
		err = ePlayerPackageFail;
	}
	else
	{
		// 增加物品
		vector<int> pos;
		vector<DWORD> strength;
		strength.resize(info->items.itemID.size(), 0);

		int retErr = player->GetContainer(ePackageType)->IsBagEnought(info->items.itemID, info->items.itemNum, info->items.isBind);
		if (0 == retErr)
		{
			player->GetContainer(ePackageType)->AddItem(info->items.itemID, info->items.itemNum, pos, info->items.isBind, strength);
			err = 0;
		}
		else if (ePackageFull == retErr)
		{
			vector<int64> getAward;
			for(uint i = 0; i < info->items.itemID.size(); i++)
			{
				int64 good = info->items.itemNum[i];
				good = good << 32;	//物品数量
				good = good | info->items.itemID[i];
				getAward.push_back(good);
			}

			MailMessageMgr::GetInstance()->SendSysMail(getAward, player->GetName());
			err = 0;
		}
		else
		{
			err = retErr;
		}
	}

	req.set_ret(err);

	Safe_Smart_Ptr<Message> msg = build_message(message, &req, Ack);
	msg->SetMessageTime(player->GetID());
	Message_Facade::Send(msg);
}

DEF_MSG_SIMPLE_DEFINE_FUN(ClanMessage, MSG_SIM_MAKE_GODTREE_ITEM)
{
	ServerReturn::ServerDoubleRetInt meContent;
	char *con;
	int len = 0;
	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);
	Smart_Ptr<CGameZone> tZone = CSceneMgr::GetInstance()->GetGameZone(meContent.rets());
	if(!tZone)
	{
		return ;
	}
			
	const GodTreeData* tData =  GodTreeFile::GetInstance()->getData(meContent.retf());
	if(tData == NULL)
	{
		return ;
	}

	const NpcInfo* tNpcInfo = NPCInfo::GetInstance()->GetInfo(tData->m_NPCID);
	if(tNpcInfo == NULL)
	{
		return ;
	}

	for(size_t i = 0;i < tData->m_FallReward.size();++i)
	{
		int tEveryCounts = tData->m_FallReward[i].m_EveryCounts;
		int tAllCounts   = tData->m_FallReward[i].m_Item.m_Counts;
		int tCurCounts   = 0;

		if(tAllCounts == 0 || tEveryCounts == 0)
		{
			continue;
		}

		tAllCounts = tAllCounts - tEveryCounts;
		while(tAllCounts > 0)
		{
			int tRange = tData->m_FallReward[i].m_Range;
			if(tRange != 0)
				tRange = (rand() % tRange + 1) - tRange / 2;

			tCurCounts  = tEveryCounts + tRange;
			tCurCounts  = tAllCounts >= tCurCounts ? tCurCounts : tAllCounts;
			tAllCounts -= tCurCounts;
			
			vector<DWORD> tItems;
			vector<DWORD> tCount;
			vector<WORD> tFlag;

			tItems.push_back(tData->m_FallReward[i].m_Item.m_ItemID);
			tCount.push_back(tCurCounts);
			tFlag.push_back(tData->m_FallReward[i].m_Item.m_Bind);

			tRange = tData->m_Range;
			if(tRange != 0)
				tRange = (rand() % tRange + 1) - tRange / 2;

			int tPosX = tNpcInfo->xpos + tRange;
			int tPosY = tNpcInfo->ypos + tRange;

			DropItemsManager::GetInstance()->dropByparam(tZone,tItems,tCount,tFlag,tPosX,tPosY);
		}
	}

	ServerReturn::ServerRetInt tSendMsg;
	tZone->SendMapAllSynchMessage(&tSendMsg,MSG_SIM_GM2C_FALL_EFFECT,0,true);

	return ;
}

DEF_MSG_REQUEST_DEFINE_FUN(ClanMessage, MSG_REQ_W2GM_BUCKLE_ITEM)
{
	Smart_Ptr<Player> player;
	PlayerMessage::GetInstance()->GetPlayerByCharId(player, message->GetMessageTime());
	if (!player)
	{
		return;
	}
	GodTreepro::GmBuckleItem tMsg;
	char *con;
	int len = 0;
	con = message->GetBuffer(len);
	tMsg.ParseFromArray(con, len);

	map<int,int> tHaveItems;
	map<int,int> tCanBuckles;
	map<int,int> tResultBuckles;
	for(int i = 0;i < tMsg.items_size();++i)
	{
		tHaveItems[tMsg.items(i).id()] = tMsg.items(i).counts();
	}
	
	int tLevel = tMsg.level();;
	GodTreeFile::GetInstance()->getCanBuckle(tCanBuckles,tHaveItems,tLevel);

	tMsg.Clear();

	map<int,int>::iterator tIt = tCanBuckles.begin();
	
	for(;tIt != tCanBuckles.end();++tIt)
	{
		int tCounts = player->GetContainer(ePackageType)->FindPropCount(tIt->first);
		tCounts = tIt->second > tCounts ? tCounts : tIt->second; 	

		if(tCounts == 0)
		{
			continue;
		}
		tResultBuckles[tIt->first] += tCounts;
	}

	if(tResultBuckles.size() != 0)
	{
		vector<DWORD> tItems;
		vector<DWORD> tNums;
		vector<WORD> tFlags;
		vector<DWORD> tStrengs;
		vector<int> tPos;
		GodTreeFile::GetInstance()->getSubmitReward(tLevel,tResultBuckles,tItems,tNums,tFlags);
		tStrengs.resize(tItems.size(),0);
		if(player->GetContainer(ePackageType)->AddItem(tItems,tNums,tPos,tFlags,tStrengs))
		{
			tMsg.set_error(ePackageFull);
		}
		else
		{
			tIt = tResultBuckles.begin();
			for(;tIt != tResultBuckles.end();++tIt)
			{
				if(tIt->second == 0)
					continue;
				BaseArgs tempArgs;
				tempArgs.GoodsID = tIt->first;
				tempArgs.Num     = tIt->second;
				player->GetContainer(ePackageType)->DeleteGoods(&tempArgs,true);

				GodTreepro::OneItem* tOneItem = tMsg.add_items();
				tOneItem->set_id(tIt->first);
				tOneItem->set_counts(tIt->second);
			}

			tMsg.set_error(eRetSuccess);
		}
	}
	else
	{
		tMsg.set_error(eGodTreeNotItem);
	}

	Safe_Smart_Ptr<Message> msg = build_message(message, &tMsg,Ack);
	msg->SetMessageTime(player->GetID());
	Message_Facade::Send(msg);

	return ;
}

void ClanMessage::makeItemTest(Smart_Ptr<Player> player,int level)
{
	const GodTreeData* tData =  GodTreeFile::GetInstance()->getData(level);
	if(tData == NULL)
	{
		return ;
	}

	Smart_Ptr<CGameZone> tZone = CSceneMgr::GetInstance()->GetGameZone(player->GetGameZone()->GetSceneID());
	if(!tZone)
	{
		return ;
	}
	char tChar[2048] = {0};
	for(size_t i = 0;i < tData->m_FallReward.size();++i)
	{
		int tEveryCounts = tData->m_FallReward[i].m_EveryCounts;
		int tAllCounts   = tData->m_FallReward[i].m_Item.m_Counts;
		int tCurCounts   = 0;

		if(tAllCounts == 0 || tEveryCounts == 0)
		{
			continue;
		}

		tAllCounts = tAllCounts - tEveryCounts;
		while(tAllCounts > 0)
		{
			int tRange = tData->m_FallReward[i].m_Range;
			if(tRange != 0)
				tRange = (rand() % tRange + 1) - tRange / 2;

			tCurCounts  = tEveryCounts + tRange;
			tCurCounts  = tAllCounts >= tCurCounts ? tCurCounts : tAllCounts;
			tAllCounts -= tCurCounts;
			
			vector<DWORD> tItems;
			vector<DWORD> tCount;
			vector<WORD> tFlag;

			tItems.push_back(tData->m_FallReward[i].m_Item.m_ItemID);
			tCount.push_back(tCurCounts);
			tFlag.push_back(tData->m_FallReward[i].m_Item.m_Bind);

			tRange = tData->m_Range;
			if(tRange != 0)
				tRange = (rand() % tRange + 1) - tRange / 2;

			int tPosX = player->GetXNewPos() + tRange;
			int tPosY = player->GetYNewPos() + tRange;
			char tOneChar[16];
			sprintf(tOneChar,"x %d y %d,",tPosX,tPosY);
			strcat(tChar,tOneChar);

			DropItemsManager::GetInstance()->dropByparam(tZone,tItems,tCount,tFlag,tPosX,tPosY);
		}
	}

	LOG_DEBUG(FILEINFO,"%s",tChar);

	ServerReturn::ServerRetInt tSendMsg;
	tZone->SendMapAllSynchMessage(&tSendMsg,MSG_SIM_GM2C_FALL_EFFECT,0,true);
}
