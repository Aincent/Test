/*
 * ChallengeMgr.cpp
 *
 *  Created on: 2016年5月6日
 *      Author: root
 */
#include "ChallengeMgr.h"

#include "../MessageBuild.h"
#include "../Map/DuplicateMgr.h"
#include "../MsgCommonClass.h"
#include "MessageCommonRet.h"
#include "InstanceMapLoader.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "../Object/Player/PlayerMessage.h"
#include "FileLoader/InternationalLoader.h"
#include "../Mail/MailMessage.h"
#include "PlayerChallenge.h"
#include "../Object/FileLoader/MonsterLoader.h"

ChallengeMgr::ChallengeMgr()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(ChallengeMgr, MSG_REQ_C2GS_CHALLENGEINFO);
	DEF_MSG_QUICK_REQUEST_REG_FUN(ChallengeMgr, MSG_REQ_SWEEP_CHALLENGE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(ChallengeMgr, MSG_REQ_END_CHALLENGE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(ChallengeMgr, MSG_REQ_EXCHANGE_CHALLENGE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(ChallengeMgr, MSG_C2GS_GATHER_BOX);

	m_pLua.OpenLua();
}

ChallengeMgr::~ChallengeMgr()
{
	m_pLua.CloseLua();
}

int ChallengeMgr::Init()
{
	if(InitLayerData() != 0 || InitShopData() != 0 || InitLua() != 0)
	{
		return -1;
	}
	return 0;
}

const ChallengeLayerInfo* ChallengeMgr::GetLayerInfo(int layer)
{
	std::map<int,ChallengeLayerInfo>::const_iterator it = m_layerConfig.find(layer);
	if(it != m_layerConfig.end())
	{
		return &it->second;
	}
	return NULL;
}

const ChallengeShop* ChallengeMgr::GetShopInfo(int id)
{
	std::map<int,ChallengeShop>::const_iterator it = m_ChallengeShop.find(id);
	if(it != m_ChallengeShop.end())
	{
		return &it->second;
	}
	return NULL;
}

void ChallengeMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{

}

void ChallengeMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{

}

void ChallengeMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ChallengeMgr, MSG_REQ_C2GS_CHALLENGEINFO)
{
	MapInstanceProto::AckChallengeInfo ack;

	player->GetChallenge()->GetData(&ack);

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GS_CHALLENGEINFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ChallengeMgr, MSG_REQ_SWEEP_CHALLENGE)
{
	MapInstanceProto::AckSweep ack;

	int elapsedTime = 0;
	int ret = player->GetChallenge()->StartSweep(elapsedTime);

	ack.set_code(ret);
	ack.set_elapsedtime(elapsedTime);
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_SWEEP_CHALLENGE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ChallengeMgr, MSG_REQ_END_CHALLENGE)
{
	MapInstanceProto::ACkEndSweep ack;

	int ret = player->GetChallenge()->EndSweep(&ack);

	ack.set_code(ret);
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_END_CHALLENGE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ChallengeMgr, MSG_REQ_EXCHANGE_CHALLENGE)
{
	MapInstanceProto::ReqExchange req;
	MapInstanceProto::AckExchange ack;
	req.ParseFromString(content);

	int ret = player->GetChallenge()->ExChange(req.id(), req.num());

	ack.set_id(req.id());
	ack.set_code(ret);

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_EXCHANGE_CHALLENGE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(ChallengeMgr, MSG_C2GS_GATHER_BOX)
{
	MapInstanceProto::GatherBox req;
	MapInstanceProto::AckGatherBox ack;

	req.ParseFromString(content);

	CGameZone * pZone = player->GetGameZone();
	if(pZone == NULL)
	{
		return;
	}

	Smart_Ptr<Object> obj = pZone->GetObjectByKey(req.key());

	if(obj.Get() == NULL || obj->GetType() != eMonster)
	{
		return;
	}

	Smart_Ptr<Monster> mon = obj;
	if(mon->GetQuality() != eChallengeBox)
	{
		return;
	}

	MonsterInfo info;
	MonsterLoader::GetInstance()->GetMonsterInfo(mon->GetID(), info);

	int error = 0;

	Smart_Ptr<ArticleBaseMgr> pack = player->GetContainer(ePackageType);
	if(info.fallID.size() > 0)
	{
		const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(info.fallID[0]);
		if(ginfo != NULL && pack->IsBagEnought(info.fallID[0],1))
		{
			error = ePackageFull;
		}
		else
		{
			vector<int> tempPosList;
			if(ginfo == NULL || 0 != pack->AddItem(info.fallID[0],1,tempPosList,ginfo->bindTyle))
			{
				error = ePackageFull;
			}
			else
			{
				//::MapInstanceProto::PrizeItem* pItem = ack.add_items();
				//pItem->set_id(info.fallID[0]);
				//pItem->set_num(1);
			}
		}
	}

	ack.set_key( req.key() );
	ack.set_code( error );
	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_C2GS_GATHER_BOX);

	if(error == 0)
	{
		mon->SetDead(player);
	}

}


int ChallengeMgr::InitLayerData()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return -1;
	}

	string strFile = strFileFullPath + "Data/tiaozhanfubenData.csv";
	CSVLoader loader;

	if(!loader.OpenFromFile(strFile.c_str()))
	{
		return -1;
	}

	//层数ID	显示模型模型	目标	扫荡时间	物品奖励	闯关时间	评分时间	1星系数	2星系数	3星系数	1星奖励	2星奖励	3星奖励
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		ChallengeLayerInfo info;
		int count = 0;

		info.layer = loader.GetInt(i, count++);

		count++;

		//击杀目标
		string target = loader.GetString(i, count++, "");
		StrArgPkg targetPkg("|", target);
		targetPkg.setArgs(target);

		for(uint itemSize=0; itemSize<targetPkg.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(targetPkg, itemSize).c_str());
			if(limitPkg.size() != 2)
					continue;

			int monster = atoi(getArg(limitPkg, 0).c_str());
			int count = atoi(getArg(limitPkg, 1).c_str());

			info.targets.push_back(make_pair(monster,count));
		}

		//扫荡时间
		info.sweeptime = loader.GetInt(i, count++);

		//奖励物品ID
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		funPkgItem.setArgs(funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			if(limitPkg.size() != 2)
				continue;

			int id = atoi(getArg(limitPkg, 0).c_str());
			int num = atoi(getArg(limitPkg, 1).c_str());

			info.prizes.push_back(make_pair(id,num));
		}

		//闯关时间
		info.passtime = loader.GetInt(i, count++);

		//评分时间
		string strtimes = loader.GetString(i, count++, "");
		StrArgPkg timesPkg("|", strtimes);
		timesPkg.setArgs(strtimes);
		if(timesPkg.size()==3)
		{
			info.oneStartTimes = atoi(getArg(timesPkg, 0).c_str());
			info.twoStartTimes = atoi(getArg(timesPkg, 1).c_str());
			info.threeStartTimes = atoi(getArg(timesPkg, 2).c_str());
		}

		//1星系数
		info.oneStartParam = loader.GetInt(i, count++);

		//2星系数
		info.twoStartParam = loader.GetInt(i, count++);

		//3星系数
		info.threeStartParam = loader.GetInt(i, count++);

		//1星奖励
		string oneStartPrizes = loader.GetString(i, count++, "");
		StrArgPkg oneStartPrizesPkg("|", oneStartPrizes);
		oneStartPrizesPkg.setArgs(oneStartPrizes);

		std::vector<pair<int,int> > vTmep;

		for(uint itemSize=0; itemSize<oneStartPrizesPkg.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(oneStartPrizesPkg, itemSize).c_str());
			if(limitPkg.size() != 2)
				continue;

			int id = atoi(getArg(limitPkg, 0).c_str());
			int num = atoi(getArg(limitPkg, 1).c_str());

			vTmep.push_back( make_pair(id, num) );
		}
		info.startsPrizes[1] =vTmep;

		//2星奖励
		string twoStartPrizes = loader.GetString(i, count++, "");
		StrArgPkg twoStartPrizesPkg("|", twoStartPrizes);
		twoStartPrizesPkg.setArgs(twoStartPrizes);

		vTmep.clear();
		for(uint itemSize=0; itemSize<twoStartPrizesPkg.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(twoStartPrizesPkg, itemSize).c_str());
			if(limitPkg.size() != 2)
					continue;

			int id = atoi(getArg(limitPkg, 0).c_str());
			int num = atoi(getArg(limitPkg, 1).c_str());

			vTmep.push_back( make_pair(id, num) );
		}
		info.startsPrizes[2] =vTmep;

		//3星奖励
		string threeStartPrizes = loader.GetString(i, count++, "");
		StrArgPkg threeStartPrizesPkg("|", threeStartPrizes);
		threeStartPrizesPkg.setArgs(threeStartPrizes);

		vTmep.clear();
		for(uint itemSize=0; itemSize<threeStartPrizesPkg.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(threeStartPrizesPkg, itemSize).c_str());
			if(limitPkg.size() != 2)
				continue;

			int id = atoi(getArg(limitPkg, 0).c_str());
			int num = atoi(getArg(limitPkg, 1).c_str());

			vTmep.push_back( make_pair(id, num) );
		}
		info.startsPrizes[3] =vTmep;

		m_layerConfig[info.layer] = info;
	}

	return 0;
}

int ChallengeMgr::InitShopData()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return -1;
	}

	string strFile = strFileFullPath + "Data/JingshiTypeData.csv";
	CSVLoader loader;

	if(!loader.OpenFromFile(strFile.c_str()))
	{
		return -1;
	}

	//排序	道具ID	货币类型	道具单价	购买限制	是否绑定
	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		ChallengeShop info;
		int count = 0;

		count++;

		info.id = loader.GetInt(i, count++);

		count++;

		info.price = loader.GetInt(i, count++);
		info.layerLimit = loader.GetInt(i, count++);
		info.bindType = loader.GetInt(i, count++);

		m_ChallengeShop[info.id] = info;
	}

	return 0;
}

int ChallengeMgr::InitLua()
{
	std::string strFileFullPath;
	if(CUtil::GetModulePath(strFileFullPath) != 0)
	{
		return -1;
	}

	string strConfig = strFileFullPath + "Data/Challenge.lua";
	if(m_pLua.LoadFile(strConfig) != 0 || m_pLua.Call() != 0)
	{
		return -1;
	}

	//层的数据
	m_pLua.SetLuaFunction("GetLayerNode");
	if(m_pLua.Call(0, 1))
	{
		return -1;
	}

	int tableCount = m_pLua.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_pLua.GetFieldTable(i) != 0)
		{
			return -1;
		}

		int layer = m_pLua.GetFieldInt("layer");

		std::vector<LayerMonsterInfo> vLayerMonsters;

		char buffer[512] = {0};
		m_pLua.GetFieldString(buffer, 512, "info");

		string strInfo = buffer;
		StrArgPkg strPkg("|", strInfo);
		for(size_t n=0; n<strPkg.size(); ++n)
		{
			LayerMonsterInfo info;

			string strTemp = getArg(strPkg,n);
			StrArgPkg strPkgTemp(":", strTemp);
			if(strPkgTemp.size()!=2)
				continue;

			info.monster = atoi(getArg(strPkgTemp,0).c_str());

			string strPos = getArg(strPkgTemp,1);
			StrArgPkg strPosPkg("#", strPos);
			for(size_t k=0; k<strPosPkg.size(); ++k)
			{
				info.vPos.push_back( atoi(getArg(strPosPkg,k).c_str()) );
			}

			if(info.monster > 0 && info.vPos.size() > 0)
			{
				vLayerMonsters.push_back(info);
			}
		}
		m_LayerMonsterInfo[layer] = vLayerMonsters;
		m_pLua.CloseTable();
	}
	m_pLua.CloseTable();

	//坐标数据
	m_pLua.SetLuaFunction("GetPos");
	if(m_pLua.Call(0, 1))
	{
		return -1;
	}

	tableCount = m_pLua.GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
		if(m_pLua.GetFieldTable(i) != 0)
		{
			return -1;
		}

		int layer = m_pLua.GetFieldInt("index");
		int posx = m_pLua.GetFieldInt("posx");
		int posy = m_pLua.GetFieldInt("posy");

		m_PosInfo[layer] = make_pair(posx,posy);
		m_pLua.CloseTable();
	}
	m_pLua.CloseTable();

	return 0;
}

void ChallengeMgr::GetMonsterPos(int layer, std::vector<ChallengeMonstersPos>& infos)
{
	std::map<int,std::vector<LayerMonsterInfo> >::iterator it = m_LayerMonsterInfo.find(layer);
	if(it != m_LayerMonsterInfo.end())
	{
		for(size_t i=0; i<it->second.size(); ++i)
		{
			ChallengeMonstersPos pos;
			pos.monster = it->second[i].monster;
			for(size_t j=0; j<it->second[i].vPos.size(); ++j)
			{
				std::map<int,pair<int,int> >::iterator itpos = m_PosInfo.find(it->second[i].vPos[j]);
				if(itpos != m_PosInfo.end())
				{
					pos.vPos.push_back( itpos->second );
				}
			}
			infos.push_back(pos);
		}
	}
}

void ChallengeMgr::SendAwardToPlayer(Smart_Ptr<Player> player, bool isSuccess, int layer, int star, DWORD64 epleseTime, int fight)
{
	MapInstanceProto::ToGTAwardInfo toGT;
	MapInstanceProto::SimInstanceEnd* toMemberAward = toGT.mutable_awardinfo();		//发送奖励

	toMemberAward->set_mapid(GET_MAP_ID(player->GetMapID()));
	toMemberAward->set_issuccess(isSuccess);
	toGT.set_charid(player->GetID());

	const ChallengeLayerInfo* entry = GetLayerInfo(layer);
	if(entry == NULL)
	{
		LOG_ERROR(FILEINFO,"SendAwardToPlayer is invalid %d.", layer);
		return;
	}

	if(!isSuccess)
	{
		//发送奖励物品
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2GT_ENDINSTANCE, &toGT, player->GetChannelID(), SimpleMessage);
		Message_Facade::Send(messRet);
		return;
	}

	MapInstanceProto::SimInstanceAward* awards = toMemberAward->mutable_awards();

	//关卡积分
	awards->set_barrierpoints(100);
	//消耗的时间
	awards->set_elapsedtime((int)(epleseTime/1000));
	awards->set_rewardstar(star);

	awards->set_personalpower(player->getAttManage()->getValue<int>(eFightPower));
	awards->set_grouppower(fight);

	for(size_t j = 0; j < entry->prizes.size(); j++)
	{
		Smart_Ptr<PropBase> newItem = new PropBase(entry->prizes[j].first,entry->prizes[j].second);
		if(!newItem)
		{
			continue;
		}

		CharLogin::ClientItemInfo* awardItme = awards->add_items();
		newItem->SetClientInfo(awardItme);
	}

	//发送奖励物品
	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GM2GT_ENDINSTANCE, &toGT, player->GetChannelID(), SimpleMessage);
	Message_Facade::Send(messRet);

	//通知世界服结果
	Pass(player, layer);

	return;
}

int64 ChallengeMgr::FitchReWard(Smart_Ptr<Player> player, int layer, int star)
{
	const ChallengeLayerInfo* entry = GetLayerInfo(layer);
	if(entry == NULL)
	{
		LOG_ERROR(FILEINFO,"FitchReWard is invalid %d.", layer);
		return eInstanceInfoError;
	}

	vector<int> vec;
	vector<int64> goods;
	vector<DWORD> idList;
	vector<DWORD> numList;
	int64 error = 0;

	for(size_t i=0; i<entry->prizes.size(); ++i)
	{
		idList.push_back(entry->prizes[i].first);
		numList.push_back(entry->prizes[i].second);
	}

	std::map<int, std::vector<pair<int,int> > >::const_iterator it = entry->startsPrizes.find(star);
	if(it != entry->startsPrizes.end())
	{
		for(size_t i=0; i<it->second.size(); ++i)
		{
			idList.push_back(it->second[i].first);
			numList.push_back(it->second[i].second);
		}
	}

	//给玩家添加物品
	Smart_Ptr<ArticleBaseMgr> pPacket = player->GetContainer(ePackageType);
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
		error = ePackNoSpaceSendMail;

		std::vector<PropItem> vItems;
		for(size_t j = 0; j < idList.size(); j++)
		{
			vItems.push_back( PropItem(idList[j], numList[j]) );
		}

		string warning1 = InternationalLoader::GetInstance()->GetInterStr(8);
		MailMessageMgr::GetInstance()->SendMail(player->GetID(), vItems, warning1.c_str(), warning1.c_str());
	}

	return error;
}

int ChallengeMgr::GetStart(int layer, DWORD64 esplese)
{
	const ChallengeLayerInfo* entry = GetLayerInfo(layer);
	if(entry == NULL)
		return 0;

	esplese /= 1000;

	if((int)esplese <= entry->oneStartTimes)
	{
		return 3;
	}

	if((int)esplese <= entry->twoStartTimes)
	{
		return 2;
	}

	if((int)esplese <= entry->threeStartTimes)
	{
		return 1;
	}

	return 0;
}

void ChallengeMgr::Pass(Smart_Ptr<Player> player, int layer)
{
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
	if(worldSvr)
	{
		::MapInstanceProto::PassChallenge toWorld;
		toWorld.set_layer( layer );
		toWorld.set_charid( player->GetID() );
		toWorld.set_name( player->GetName() );
		toWorld.set_level( player->getAttManage()->getValue<int>(eCharLv) );
		toWorld.set_fight( player->getAttManage()->getValue<int>(eFightPower) );
		toWorld.set_vip( player->getAttManage()->getValue<int>(eVIPLv) );

		Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_GS2WS_CHALLENGE_RESULT, &toWorld, worldSvr.Get(), SimpleMessage);
		Message_Facade::Send(messRet);
	}
}










