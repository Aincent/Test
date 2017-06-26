/*
 * MoneyInstanceMgr.cpp
 *
 *  Created on: 2015年9月11日
 *      Author: root
 */
#include "MoneyInstanceMgr.h"
#include "../MessageBuild.h"
#include "../Map/DuplicateMgr.h"
#include "../MsgCommonClass.h"
#include "MessageCommonRet.h"
#include "InstanceMapLoader.h"
#include "../ServerEventDefine.h"
#include "../EventSet/EventDefine.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "MessageStruct/MoneyInsatence/MoneyInsatence.pb.h"
#include "../Object/Player/PlayerMessage.h"
#include "../MapCarbon/MoneyCarbon.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../StatisticMgr/StatisticHandle.h"


MoneyInstanceMgr::MoneyInstanceMgr()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_BUY_ITEM);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_USESKILL);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_BUY_ATTR);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_FITCHPRIZE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_RANK);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_EXIT);
	DEF_MSG_QUICK_REQUEST_REG_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_BUY_COUNT);

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GS2WS_MONEYINSTANCE_RANK);

	m_pLua.OpenLua();
	loadFile();
}

MoneyInstanceMgr::~MoneyInstanceMgr()
{
	m_pLua.CloseLua();
}

void MoneyInstanceMgr::loadFile()
{
	loadTowerInfo();
	loadAttrBuyInfo();
	loadLimitInfo();
	loadLua();
}

int MoneyInstanceMgr::getMosterMoney(int type, int lv)
{
		m_pLua.SetLuaFunction("GetMonsterMoney");
		m_pLua.SetInt(lv);
		m_pLua.SetInt(type);
		if(m_pLua.Call(2,1))
		{
				return 0;
		}
		return m_pLua.GetInt();
}

int MoneyInstanceMgr::getMonsterAttack(int lv, int type, int node)
{
		m_pLua.SetLuaFunction("GetMonsterAttack");
		m_pLua.SetInt(lv);
		m_pLua.SetInt(type);
		m_pLua.SetInt(node);
		if(m_pLua.Call(3,1))
		{
				return 0;
		}
		return m_pLua.GetInt();
}

int MoneyInstanceMgr::getMonsterHP(int lv, int type, int node)
{
		m_pLua.SetLuaFunction("GetMonsterHP");
		m_pLua.SetInt(lv);
		m_pLua.SetInt(type);
		m_pLua.SetInt(node);
		if(m_pLua.Call(3,1))
		{
			return 0;
		}
		return m_pLua.GetInt();
}

int MoneyInstanceMgr::getTowerScore(int monType, int killType)
{
		m_pLua.SetLuaFunction("TowerGetScore");
		m_pLua.SetInt(monType);
		m_pLua.SetInt(killType);
		if(m_pLua.Call(2,1))
		{
			return 0;
		}
		return m_pLua.GetInt();
}

int MoneyInstanceMgr::getTowerCoolValue(int monType, int killType)
{
		m_pLua.SetLuaFunction("TowerGetCoolValue");
		m_pLua.SetInt(monType);
		m_pLua.SetInt(killType);
		if(m_pLua.Call(2,1))
		{
				return 0;
		}
  	return m_pLua.GetInt();
}

//根据玩家等级得到塔的攻击 防御 生命
int MoneyInstanceMgr::getTowerAttack(int lv, int param)
{
		m_pLua.SetLuaFunction("GetAttack");
		m_pLua.SetInt(lv);
		m_pLua.SetInt(param);
		if(m_pLua.Call(2,1))
		{
			return 0;
		}
	 return m_pLua.GetInt();
}

int MoneyInstanceMgr::getTowerDefene(int lv, int param)
{
		m_pLua.SetLuaFunction("GetDefense");
		m_pLua.SetInt(lv);
		m_pLua.SetInt(param);
		if(m_pLua.Call(2,1))
		{
			return 0;
		}
	 return m_pLua.GetInt();
}

int MoneyInstanceMgr::getTowerHP(int lv, int param)
{
		m_pLua.SetLuaFunction("GetHP");
		m_pLua.SetInt(lv);
		m_pLua.SetInt(param);
		if(m_pLua.Call(2,1))
		{
			return 0;
		}
		return m_pLua.GetInt();
}

void MoneyInstanceMgr::getTowerAttr(int playerLv, const TowerInfo* pInfo, int& attack, int& defene, int& hp)
{
		int param1 = pInfo->attack > 0 ? pInfo->attack : 1;
		int param2 = pInfo->defene > 0 ? pInfo->defene : 1;
		int param3 = pInfo->hp > 0 ? pInfo->hp : 1;

		attack = getTowerAttack( playerLv, param1 );
		defene = getTowerDefene( playerLv, param2 );
		hp = getTowerHP( playerLv, param3 );
}

int MoneyInstanceMgr::getTowerMaxLevel()
{
	return m_TowerInfo.size();
}

int MoneyInstanceMgr::getNodeType(int node)
{
	int type = eNoticeType_NormalMonster;
	std::map<int,FlushNodeInfo>::const_iterator it = m_NodeInfo.find(node);
	if(it != m_NodeInfo.end())
	{
		MonsterInfo info;
		MonsterLoader::GetInstance()->GetMonsterInfo(it->second.id, info);
		if(info.id > 0)
		{
			type = info.quality == eTowerBossMonster ? eNoticeType_BossMonster : eNoticeType_NormalMonster;
		}
	}
	return type;
}

const FlushNodeInfo* MoneyInstanceMgr::getMonsterNodeInfo(int node)const
{
	std::map<int,FlushNodeInfo>::const_iterator it = m_NodeInfo.find(node);
	if(it != m_NodeInfo.end())
		return &it->second;
	return NULL;
}

vector<Pos> MoneyInstanceMgr::GetPos(int index, int type)
{
		vector<Pos> pos;
		//小怪的路径点
		if(type == 0)
		{
			if(m_reflushPoint.size() <= 0)
				return pos;

			index = index % m_reflushPoint.size();
			return m_reflushPoint[index];
		}
		else//boss的路径点
		{
			if(m_BossPoints.size() <= 0)
					return pos;

			index = index % m_BossPoints.size();
			return m_BossPoints[index];
		}

}

const TowerInfo* MoneyInstanceMgr::getTowerInfo(int lv)const
{
	std::map<int,TowerInfo>::const_iterator it = m_TowerInfo.find(lv);
	if(it!=m_TowerInfo.end())
		return &it->second;
	return NULL;
}

const AttrBuyInfo* MoneyInstanceMgr::getAttrBuyInfo(int count)const
{
	std::map<int,AttrBuyInfo>::const_iterator it = m_BuyAtrrInfo.find(count);
	if(it!=m_BuyAtrrInfo.end())
		return &it->second;
	return NULL;
}

void MoneyInstanceMgr::loadTowerInfo()
{
		std::string strFileFullPath;
		if(CUtil::GetModulePath(strFileFullPath) != 0)
		{
				return;
		}
		string strFile = strFileFullPath + "Data/goldFubenTa.csv";
		CSVLoader loader;

		if(!loader.OpenFromFile(strFile.c_str()))
		{
				return;
		}

		//ID	塔等级	攻击	防御	生命	升级条件1	升级条件2	钢化符使用次数	钢化符价格	钢化符加益
		//强攻符使用次数	强攻符价格	强攻符加益	减冷却符使用次数	减冷却符价格	减冷却符加益	技能冷却值	结算奖励
		for(unsigned int i=0; i<loader.GetRowCount(); ++i)
		{
				int count = 0;
				loader.GetInt(i, count++);

				TowerInfo info;
				info.lv = loader.GetInt(i, count++);			//类型
				info.attack = loader.GetInt(i, count++);
				info.defene = loader.GetInt(i, count++);
				info.hp = loader.GetInt(i, count++);
				info.point = loader.GetInt(i, count++);
				loader.GetInt(i, count++);
				info.defeneUseNum = loader.GetInt(i, count++);
				info.defenePrice = loader.GetInt(i, count++);
				info.defeneValue = loader.GetInt(i, count++);
				info.moneyType1  = loader.GetInt(i, count++);

				info.attackUseNum = loader.GetInt(i, count++);
				info.attackPrice = loader.GetInt(i, count++);
				info.attackValue = loader.GetInt(i, count++);
				info.moneyType2  = loader.GetInt(i, count++);

				info.coolUseNum = loader.GetInt(i, count++);
				info.coolPrice = loader.GetInt(i, count++);
				info.coolValue = loader.GetInt(i, count++);
				info.maxCool = loader.GetInt(i,count++);
				info.moneyType3  = loader.GetInt(i, count++);

				std::string strProp = loader.GetString(i, count++);
				StrArgPkg strPkg(":", strProp);
				if(strPkg.size()!=2)
				{
						return;
				}
				info.prizeMoney = atoi(getArg(strPkg,1).c_str());

				m_TowerInfo[info.lv] = info;
		}

}

void MoneyInstanceMgr::loadAttrBuyInfo()
{
			std::string strFileFullPath;
			if(CUtil::GetModulePath(strFileFullPath) != 0)
			{
					return;
			}
			string strFile = strFileFullPath + "Data/goldFubenZ.csv";
			CSVLoader loader;

			if(!loader.OpenFromFile(strFile.c_str()))
			{
					return;
			}

			//ID	提高战力倍率	单次花费  属性id
			for(unsigned int i=0; i<loader.GetRowCount(); ++i)
			{
					int count = 0;
					AttrBuyInfo info;
					info.count = loader.GetInt(i, count++);
					info.rate = loader.GetFloat(i, count++);
					info.cost = loader.GetInt(i,count++);
					loader.GetInt(i,count++);
					info.moneyType = loader.GetInt(i,count++);
					std::string str = loader.GetString(i, count++);
					StrArgPkg strPkg(":", str);
					for(size_t j=0; j<strPkg.size(); ++j)
					{
						info.attrID.push_back( atoi(getArg(strPkg,j).c_str()) );
					}
					m_BuyAtrrInfo[info.count] = info;
			}
}

void MoneyInstanceMgr::loadLimitInfo()
{
		std::string strFileFullPath;
		if(CUtil::GetModulePath(strFileFullPath) != 0)
		{
				return;
		}

	 string strFile = strFileFullPath + "Data/goldFubenJ.csv";
		CSVLoader loader;

		if(!loader.OpenFromFile(strFile.c_str()))
		{
				return;
		}

		//ID	每日购买次数上限	单次购买花费  消耗金钱类型  自动退出时间
		for(unsigned int i=0; i<loader.GetRowCount(); ++i)
		{
			int count = 0;
			loader.GetInt(i, count++);
			m_config.maxCount = loader.GetInt(i, count++);
			m_config.cost = loader.GetInt(i, count++);
			m_config.moneyType = loader.GetInt(i, count++);
			m_config.exitTime = loader.GetInt(i, count++);

			return;
		}

}

void MoneyInstanceMgr::loadLua()
{
		std::string strFileFullPath;
		if(CUtil::GetModulePath(strFileFullPath) != 0)
		{
				return;
		}

		string strConfig = strFileFullPath + "Data/goldFuben.lua";
		if(m_pLua.LoadFile(strConfig) != 0 || m_pLua.Call() != 0)
		{
				return;
		}

		m_pLua.SetLuaFunction("GetCoolRecove");
		if(m_pLua.Call(0, 1))
		{
				return;
		}
		m_config.coolRecove = m_pLua.GetInt();

		m_pLua.SetLuaFunction("GetMailID");
		if(m_pLua.Call(0, 1))
		{
				return;
		}
		m_config.mailID = m_pLua.GetInt();

		//
		m_pLua.SetLuaFunction("GetFlushTime");
		if(m_pLua.Call(0, 4))
		{
				return;
		}

		m_config.flushTime = m_pLua.GetInt();
		m_config.noticeTime = m_pLua.GetInt();
		m_config.countDown = m_pLua.GetInt();
		m_config.mapID = m_pLua.GetInt();

		//
		m_pLua.SetLuaFunction("GetTowerID");
		if(m_pLua.Call(0, 1))
		{
				return;
		}
		m_config.towerID = m_pLua.GetInt();

		//刷新节点信息
		m_pLua.SetLuaFunction("GetAllNode");
		if(m_pLua.Call(0, 1))
		{
				return;
		}

		int tableCount = m_pLua.GetTableFieldCount();
		for(int i=1; i<=tableCount; ++i)
		{
			if(m_pLua.GetFieldTable(i) != 0)
			{
				return;
			}

			FlushNodeInfo node;
			node.node = m_pLua.GetFieldInt("node");
			node.id = m_pLua.GetFieldInt("id");
			node.num = m_pLua.GetFieldInt("num");
			m_NodeInfo[node.node] = node;
			m_pLua.CloseTable();
		}
		m_pLua.CloseTable();

		//小怪路径点信息
		m_pLua.SetLuaFunction("GetAllPath");
		if(m_pLua.Call(0, 1))
		{
				return;
		}

		tableCount = m_pLua.GetTableFieldCount();
		for(int i=1; i<=tableCount; ++i)
		{
			if(m_pLua.GetFieldTable(i) != 0)
			{
				return;
			}

			vector<Pos> tVec;
			int twCount = m_pLua.GetTableFieldCount();
			for(int j=1; j<=twCount; ++j)
			{
				if(m_pLua.GetFieldTable(j) != 0)
				{
					return;
				}

				Pos rand;
				rand.xpos = m_pLua.GetFieldInt(1);
				rand.ypos = m_pLua.GetFieldInt(2);

				tVec.push_back(rand);
				m_pLua.CloseTable();
			}
			m_reflushPoint.push_back(tVec);
			m_pLua.CloseTable();
		}
		m_pLua.CloseTable();

		//boss刷新点路径
		m_pLua.SetLuaFunction("GetBossPath");
		if(m_pLua.Call(0, 1))
		{
			return;
		}

		tableCount = m_pLua.GetTableFieldCount();
		for(int i=1; i<=tableCount; ++i)
		{
			if(m_pLua.GetFieldTable(i) != 0)
			{
				return;
			}

			vector<Pos> tVec;
			int twCount = m_pLua.GetTableFieldCount();
			for(int j=1; j<=twCount; ++j)
			{
				if(m_pLua.GetFieldTable(j) != 0)
				{
					return;
				}

				Pos rand;
				rand.xpos = m_pLua.GetFieldInt(1);
				rand.ypos = m_pLua.GetFieldInt(2);

				tVec.push_back(rand);
				m_pLua.CloseTable();
			}
			m_BossPoints.push_back(tVec);
			m_pLua.CloseTable();
		}
		m_pLua.CloseTable();

}


void MoneyInstanceMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GS2WS_MONEYINSTANCE_RANK);
	DEF_SWITCH_TRY_DISPATCH_END
}

void MoneyInstanceMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{

}

void MoneyInstanceMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{

}

DEF_MSG_ACK_DEFINE_FUN(MoneyInstanceMgr, MSG_REQ_GS2WS_MONEYINSTANCE_RANK)
{
	if(message->GetErrno() == eReqTimeOut)
	{
			LOG_DEBUG(FILEINFO, " MSG_REQ_GS2WS_MONEYINSTANCE_RANK ack timeout");
			return;
	}

	MoneyInstancePackage::AckMoneyInstanceRank toClient;
	MoneyInstancePackage::AckRank ack;

	int len = 0;
	char * content = message->GetBuffer(len);
	ack.ParseFromArray(content,len);
	for(int i=0; i<ack.ranklist_size(); ++i)
	{
		MoneyInstancePackage::RankData* pItem = toClient.add_ranklist();
		pItem->CopyFrom(ack.ranklist(i));
	}

	Smart_Ptr<Player> player;
	int64 charid = static_cast<FirstAct *>(act.Get())->mesDataf;
	PlayerMessage::GetInstance()->GetPlayerByCharid(charid,player);
	if(player.Get()==NULL)
		return;

	int maxCount = 0;
	int maxBuyCount = 0;
	int leftCount = 0;
	int buyCount = 0;
	player->GetInstanceMgr()->getMoneyInstanceCount(maxCount,maxBuyCount,leftCount,buyCount);

	toClient.set_maxcount(maxCount);
	toClient.set_maxbuycount(maxBuyCount);
	toClient.set_leftcount(leftCount);
	toClient.set_buycount(buyCount);

	REQ_MSG_QUICK_ACK_FUN(static_cast<FirstAct *>(act.Get())->mesReq, &toClient, MSG_REQ_C2GS_MONEYINSTANCE_RANK);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_BUY_ITEM)
{
		CGameZone *pZone = player->GetGameZone();
		if(pZone==NULL)
				return;

		if(GET_MAP_TYPE(player->GetMapID()) != ePVEMap )
		{
			return;
		}

		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
		if(entry==NULL || entry->m_eType != eMoneyInstance)
		{
				return;
		}

	MoneyInstancePackage::ReqBuyItem req;
	ServerReturn::ServerDoubleRetInt ret;
	req.ParseFromString(content);

	int code = ((MoneyCarbon*)pZone)->BuyItem(player, req.id());
	ret.set_retf(req.id());
	ret.set_rets(code);

	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_MONEYINSTANCE_BUY_ITEM);
}


DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_USESKILL)
{
		CGameZone *pZone = player->GetGameZone();
		if(pZone==NULL)
				return;

		if(GET_MAP_TYPE(player->GetMapID()) != ePVEMap )
		{
			return;
		}

		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
		if(entry==NULL || entry->m_eType != eMoneyInstance)
		{
				return;
		}

		//使用技能
		MoneyInstancePackage::AckUseSkill ack;
		int code = ((MoneyCarbon*)pZone)->UseSkill(player);
		ack.set_ret(code);
		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GS_MONEYINSTANCE_USESKILL);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_BUY_ATTR)
{
	CGameZone *pZone = player->GetGameZone();
	if(pZone==NULL)
			return;

	if(GET_MAP_TYPE(player->GetMapID()) != ePVEMap )
	{
		return;
	}

	InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
	if(entry==NULL || entry->m_eType != eMoneyInstance)
	{
			return;
	}

	MoneyInstancePackage::AckBuyAttr ret;
	int code = ((MoneyCarbon*)pZone)->BuyAttr(player);
	ret.set_ret(code);

	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_MONEYINSTANCE_BUY_ATTR);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_FITCHPRIZE)
{
		CGameZone *pZone = player->GetGameZone();
		if(pZone==NULL)
				return;

		if(GET_MAP_TYPE(player->GetMapID()) != ePVEMap )
		{
			return;
		}

		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
		if(entry==NULL || entry->m_eType != eMoneyInstance)
		{
				return;
		}

		MoneyInstancePackage::AckFitchMoney ack;
		int code = ((MoneyCarbon*)pZone)->Fitch(player);
		ack.set_ret(code);

		REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GS_MONEYINSTANCE_FITCHPRIZE);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_RANK)
{
	MoneyInstancePackage::ReqRank req;
	req.set_charid(player->GetID());

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
	if(worldSvr.Get()!=NULL)
	{
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GS2WS_MONEYINSTANCE_RANK, &req, worldSvr.Get(), Request);
		messRet->SetAct(new FirstAct(message,player->GetID()));
		Message_Facade::Send(messRet);
	}

}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_EXIT)
{
		CGameZone *pZone = player->GetGameZone();
		if(pZone==NULL)
				return;

		if(GET_MAP_TYPE(player->GetMapID()) != ePVEMap )
		{
			return;
		}

		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(player->GetMapID()));
		if(entry==NULL || entry->m_eType != eMoneyInstance)
		{
			return;
		}

		MoneyInstancePackage::AckBuyAttr ret;
		int code = ((MoneyCarbon*)pZone)->Exit(player);
		ret.set_ret(code);

		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_MONEYINSTANCE_EXIT);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(MoneyInstanceMgr, MSG_REQ_C2GS_MONEYINSTANCE_BUY_COUNT)
{
		const MoneyInstanceConfig& config = MoneyInstanceMgr::GetInstance()->getConfig();

		//本日金钱购买的次数
		int total = config.maxCount;
//		int times = (int)player->GetVipPart()->GetValueByRight(eVipMoneyInstAdd);
//		if(times != VIP_NOT_HAVE_THIS_RIGHT && times > 0)
//		{
//			total += times;
//		}

		ServerReturn::ServerResultInt ret;
		int code = 0;

		//次数限制
		int buyCount = player->GetInstanceMgr()->getMoneyInstanceBuyCount();
		if(buyCount >= total)
		{
			code = eMoneyInstanceBuyLimit;
		}
		else
		{
			//计算金钱是否足够
			switch(config.moneyType)
			{
				case eCharMoney:
				{
					int money = player->getAttManage()->getValue<int>((ECharAttrType)(config.moneyType));
					if(money < config.cost)
					{
						code = eNotEnoghMoney;
					}
					break;
				}
				case eCharGolden:
				{
					if(!player->IsEnoughGolden(config.cost))
					{
						code = eNotEnogGolden;
					}
					break;
				}
				case eCharBindGolden:
				{
					int money = player->getAttManage()->getValue<int>((ECharAttrType)(config.moneyType));
					if(money < config.cost)
					{
						code = eNotEnogBindGolden;
					}
					break;
				}
				default:
					code = eMoneyNotEnough;
			}
		}

		//更新购买次数
		if(code == 0)
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eInstance, BehaviorType::Minor::eInstance_BuyMoneyInstCnt, MONITOR);
//				StatisticInfo::CostAndGotInfo costAndGotInfo;
			std::vector<int> vecType;
			switch(config.moneyType)
			{
				case eCharMoney:
				{
//						StatisticInfo::CostItemInfo * costInfo = costAndGotInfo.add_costlist();
//						StatisticMgr::SetItemCostBefore(costInfo,eCharMoney,player);

					player->ChangeMoney(config.cost,false);
					vecType.push_back(eCharMoney);

//						StatisticMgr::SetItemCostAfter(costInfo,eCharMoney,config.cost,player);
					break;
				}
				case eCharGolden:
				{
					int costGolden = 0;
					int costbGolden = 0;

//						StatisticInfo::CostItemInfo * costGoldenInfo = costAndGotInfo.add_costlist();
//						StatisticMgr::SetItemCostBefore(costGoldenInfo,eCharGolden,player);

//						StatisticInfo::CostItemInfo * costBindGoldenInfo = costAndGotInfo.add_costlist();
//						StatisticMgr::SetItemCostBefore(costBindGoldenInfo,eCharBindGolden,player);

					player->DecreaseGoldenCost(config.cost, costGolden, costbGolden);
					if(costGolden > 0)
						vecType.push_back(eCharGolden);
					if(costbGolden > 0)
						vecType.push_back(eCharBindGolden);

//						StatisticMgr::SetItemCostAfter(costGoldenInfo,eCharGolden,costGolden,player);

//						StatisticMgr::SetItemCostAfter(costBindGoldenInfo,eCharBindGolden,costbGolden,player);

					break;
				}
				case eCharBindGolden:
				{
//						StatisticInfo::CostItemInfo * costBindGoldenInfo = costAndGotInfo.add_costlist();
//						StatisticMgr::SetItemCostBefore(costBindGoldenInfo,eCharBindGolden,player);

					player->ChangeBindGolden(config.cost,false);
					vecType.push_back(eCharBindGolden);

//						StatisticMgr::SetItemCostAfter(costBindGoldenInfo,eCharBindGolden,config.cost,player);
					break;
				}
			}

			player->SynCharAttribute(vecType);
			player->GetInstanceMgr()->addMoneyInstanceBuyCount();

			EventArgs e;
			player->FireEvent(PLAYER_MONEYINST_BUYCNT, e);

			

//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Instance,eStaMinor_Instance_AddChallengeCount,&costAndGotInfo);
		}

		ret.set_ret(code);
		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GS_MONEYINSTANCE_BUY_COUNT);
}


