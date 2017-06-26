/*
 * BeyondCarbon.cpp
 *
 *  Created on: 2015年6月30日
 *      Author: root
 */

#include "BeyondCarbon.h"
#include "util.h"
#include "Log/Logger.h"
#include "../Object/Player/Player.h"
#include "../Object/ItemDrop/DropItemMgr.h"
#include "../Map/DuplicateMgr.h"
#include "CounterDefine.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/CharLogin/MapInstanceProto.pb.h"
#include "MessageStruct/ServerReturnBool.pb.h"
#include "../QuickMessage.h"
#include "../Object/Player/PlayerMessage.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "../MsgCommonClass.h"
#include "MessageCommonRet.h"
#include "../Object/FileLoader/MonsterLoader.h"
#include "../Object/ObjectFactory.h"
#include "../Object/FileLoader/AILoader.h"
#include "../Activity/ActivityMgr.h"
#include "../MapInstance/CrossSvrCompetitionLoader/CrossSvrCompetitionLoader.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/AI/AIBrain.h"
#include "../MapInstance/BeyondBattleMgr.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/ItemDrop/DropItemAlloc.h"
#include "../Chat/ChatMgr.h"
#include "../Container/ArticleEquipMgr.h"
#include "../Mail/MailMessage.h"
#include "./FileLoader/InternationalLoader.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../Object/GirlWar/GirlWarManager.h"

CBeyondCarbon::CBeyondCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params):CGameDuplicate(onlyid,charid,paramf,params)
{

}

CBeyondCarbon::~CBeyondCarbon()
{
	m_monsterDeadEvent.SetNull();
	m_playerDeadEvent.SetNull();
	m_leaveMapEvent.SetNull();
	m_playerAttackEvent.SetNull();

	if(m_countDownTime.IsValid())
	{
		m_countDownTime.SetNull();
	}
}

int CBeyondCarbon::Init()
{
	int ret = CGameDuplicate::Init();
	return ret;
}

bool CBeyondCarbon::Update()
{
	switch(m_process)
	{
	case eCreated:
		{
			if(CUtil::GetNowSecond() - m_createTime >= m_data->m_nWrdelTime)
			{//一直没有人就删除副本
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

			if(nowTime - m_createTime >= m_data->m_nLimitTime)
			{
				Over();
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
			int64 nowTime = CUtil::GetNowSecond();

			if(GetPlayerCount() <= 0)
			{
				CountEx();
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
				//到副本结束时间就强制传送玩家出副本
				//比需要在10秒内传送出去，不然就不管了
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

void CBeyondCarbon::CountDownStart(void * arg)
{
	m_countDownTime.SetNull();

	if(m_player.size() <= 0)
	{
		m_process = eOver;
	}
	else
	{
		int64 charid = m_player.begin()->first;
		bool fChar = BeyondBattleMgr::GetInstance()->IsFirstAttack(m_sceneID, charid);

		INT64PLAYERMAP::iterator it = m_player.begin();
		for(++it; it!=m_player.end(); ++it)
		{
			if(fChar != BeyondBattleMgr::GetInstance()->IsFirstAttack(m_sceneID, it->second->GetID()))
			{
				break;
			}
		}

		if(it == m_player.end())
		{
			if(fChar)
			{
				m_process = eWinFight;
			}
			else
			{
				m_process = eFailed;
			}
		}
	}
}

bool CBeyondCarbon::JoinHandle(Smart_Ptr<Object> & obj)
{
	if(m_process >= eDuplicateChangeMap)
		return false;

	vector<int> vec;
	int type = 0;
	int index = 0;

	MapInstanceProto::SimInstanceUpdate toClient;

	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;


		if(!BeyondBattleMgr::GetInstance()->IsFightDeque(m_sceneID, player->GetID()))
		{
			return false;
		}

		BeyondBattleMgr::GetInstance()->GetPlayerIndexAndType(m_sceneID, player->GetID(), index, type);

		//把血和法加满
		vector<int> vec;
		player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
		player->SetCurMP(player->getAttManage()->getValue<int>(eCharMPMax));
		vec.push_back(eCharHP);
		vec.push_back(eCharMP);
		player->SynCharAttribute(vec);

		if(m_process == eCreated)
		{
			if(!m_playerAttackEvent.IsValid())
			{
				m_playerAttackEvent = RegGlobalEventPtr(PLAYER_ATTACK_CREATURE, this, &CBeyondCarbon::HandlePlayerAttackEvent);
			}

			if(m_countDownTime.IsValid())
			{
				m_countDownTime.SetNull();
			}

			m_countDownTime = TimerInterface::GetInstance()->RegTimePoint(&CBeyondCarbon::CountDownStart, this, 0, 0, 0, 0, 5000);
		}

		if(!m_leaveMapEvent.IsValid())
		{
			m_leaveMapEvent = RegGlobalEventPtr(PlAYER_LOGINOUT_CLEAR_EVENT_TYPE, this, &CBeyondCarbon::HandleLeaveMapEvent);
		}

		Smart_Ptr<CounterService> & counter = player->GetCounterService();
		if(counter)
		{
			player->GetCounterService()->IncreasePlayerCounter(BEYOND_BATTLE,1);

			if(type == eBeyondBattleOne)
			{
				player->GetCounterService()->IncreasePlayerCounter(BEYOND_SINGLE_BATTLE,1);
			}

			if(type == eBeyondBattleThree)
			{
				player->GetCounterService()->IncreasePlayerCounter(BEYOND_GROUP_BATTLE,1);
			}
		}

		if(vec.size() > 0)
		{
			player->SynCharAttribute(vec);
		}

		if(!m_playerDeadEvent.IsValid())
		{
			m_playerDeadEvent = RegGlobalEventPtr(CREATURE_DEAD_EVENT_TYPE, this, &CBeyondCarbon::HandlePlayerDeadEvent);
		}

		PlayerEnterInstanceEventArgs tArgs(player, GetInstanceID());
		player->FireEvent(PLAYER_ENTERINSTANCE_EVENT_TYPE, tArgs);

		if(m_process == eCreated)
			m_process = ePlayerWork;

	}

	return true;
}

int CBeyondCarbon::Join(Smart_Ptr<Object> & obj)
{
	if(!JoinHandle(obj))
		return -1;

	CGameZone::Join(obj);
	return 0;
}

int CBeyondCarbon::Move(Smart_Ptr<Object> &obj)
{
	CGameZone::Move(obj);
	return 0;
}

void CBeyondCarbon::LeaveHandle(Smart_Ptr<Object> & obj)
{
	int64 mapid = 0;
	int xpos = 0;
	int ypos = 0;
	vector<int> vecType;
	if(obj->GetType() == ePlayer)
	{
		Smart_Ptr<Player> player = obj;

		if(player->GetGirlWarManager() && player->GetGirlWarManager()->GetGirlWar())
		{
				player->GetGirlWarManager()->GetGirlWar()->ReflushInstance();
		}

		if(player->getAttManage()->getValue<int>(eCharHP) <= 0)
		{
			CharBattle::RevivalData req;

			//增加地图id和坐标，重置死亡状态，解决客户端显示“使用复活单”的提示bug，是因为玩家回城复活，那么会发两次消息给客户端
			player->SetCurHP(player->getAttManage()->getValue<int>(eCharHPMax));
			player->UnsetCreatureState(ERS_Dead);
			player->SetDataFlag(eBaseInfo);

			//如果不传地图Id就表示原地复活
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
				if (sMapInfo == 0)
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

		if(!m_countDownTime.IsValid())
		{
			if(m_process >= ePlayerWork && m_process < eAward)
			{
				bool fChar = BeyondBattleMgr::GetInstance()->IsFirstAttack(m_sceneID, player->GetID());

				INT64PLAYERMAP::iterator it = m_player.begin();
				for(; it!=m_player.end(); ++it)
				{
					if(it->second.Get()==NULL)
						continue;

					if(player->GetID() == it->second->GetID())
						continue;

					if(fChar == BeyondBattleMgr::GetInstance()->IsFirstAttack(m_sceneID, it->second->GetID()))
					{
						break;
					}
				}

				//处理提前退出副本的玩家
				PersionalBeyondSvrBattleResult(player,fChar);

				if(it == m_player.end())
				{
					if(fChar)
					{
						m_process = eFailed;
					}
					else
					{
						m_process = eWinFight;
					}
				}
			}
		}
	}
}

void CBeyondCarbon::Leave(Smart_Ptr<Object> & obj, bool isDel, bool delFromMap)
{
	LeaveHandle(obj);
	CGameZone::Leave(obj, isDel, delFromMap);
	if(GetPlayerCount() <= 0)
	{
		m_monsterDeadEvent.SetNull();
		m_playerDeadEvent.SetNull();
		m_leaveTime = CUtil::GetNowSecond();
	}
}

void CBeyondCarbon::LeaveExame(Smart_Ptr<Player> & player)
{
	if(GetPlayerCount() <= 0)
	{
		m_process = eOver;
	}
}

bool CBeyondCarbon::IsCanEnter(Smart_Ptr<Player> & player)
{
	//传送阶段，就不允许进入了
	if(m_process >= eDuplicateChangeMap)
		return false;

	//玩家死了不入传入地图
	if(player->IsDead())
	{
		return false;
	}

	if(!BeyondBattleMgr::GetInstance()->IsFightDeque(m_sceneID, player->GetID()))
	{
		return false;
	}
	return true;
}

void CBeyondCarbon::Win()
{
	SynchBeyondSvrBattleResult(1);

	m_isSuccess = true;
	m_process = eAward;
}

void CBeyondCarbon::SynchWorldBeyondResult(int score, ServerBattle::BeyondBattleResult & toClient, vector<SinglePlayerDeque> & vec, bool isSeccess)
{
	ServerBattle::BroadCastPKResult toWS;

	vector<SinglePlayerDeque>::iterator itMy = vec.begin();
	for(; itMy!=vec.end(); ++itMy)
	{
		int tempScore = 0;
		Smart_Ptr<Player> playerTemp	= GetPlayer((*itMy).charid);

		if(!playerTemp)
			continue;

		if(isSeccess)
		{
			tempScore = (*itMy).score + score;
			int beforeGradeID = CCrossSvrCompeteLoader::GetInstance()->GetGradeIDByScore((*itMy).score);
			int curGradeID = CCrossSvrCompeteLoader::GetInstance()->GetGradeIDByScore(tempScore);
			if (curGradeID == CCrossSvrCompeteLoader::GetInstance()->GetMaxGradeID() &&  curGradeID > beforeGradeID)
			{
				//升级最强王者，需要发布跑马等
				ChatMgr::GetInstance()->Marquee(eMarqueeBeyondSvr,playerTemp->GetName(),"","");
			}
		}
		else
		{
			playerTemp->GetCDMgr()->StartCD(BEYOND_CHANNEL,CCrossSvrCompeteLoader::GetInstance()->GetChallengeCDTime());
			tempScore = (*itMy).score - score;
			if(tempScore < 0)
				tempScore = 0;
		}

		toWS.set_charid((*itMy).charid);
		toWS.set_charname(playerTemp->GetName());
		toWS.set_score(tempScore);
		toWS.set_profession(playerTemp->getAttManage()->getValue<BYTE>(eCharProfession));
		playerTemp->SetSvrScore(tempScore);
		playerTemp->SetDataFlag(true);
		SetContentToWS(playerTemp, toWS);

		//同步 最新的积分给所有的ws，用于排名
		ServerConHandler::GetInstance()->BraodcastMessageToWorld(SimpleMessage, MSG_SIM_GM2WS_PK_RESULT, &toWS);

		//通知客户端胜负
		QuickMessage::GetInstance()->AddSendMessage(playerTemp->GetID(), playerTemp->GetChannelID(), &toClient, MSG_SIM_GM2C_BEYOND_RESULT);
	}
}

void CBeyondCarbon::GetBeyondSvrBattleResult(int score, ServerBattle::BeyondBattleResult & toClient, vector<SinglePlayerDeque> & vec)
{
	vector<SinglePlayerDeque>::iterator itMy = vec.begin();
	for(; itMy!=vec.end(); ++itMy)
	{
		ServerBattle::BattlePlayerResult * info = toClient.add_res();
		Smart_Ptr<Player> playerTemp	= GetPlayer((*itMy).charid);

		if(!playerTemp)
			continue;

		info->set_charid((*itMy).charid);
		info->set_name(playerTemp->GetName());
		info->set_attack((*itMy).attack);
		info->set_nowscore((*itMy).score);
		info->set_score(score);
	}
}

void CBeyondCarbon::SynchBeyondSvrBattleResult(int isSuccess)
{
	ServerBattle::BeyondBattleResult toClientf;
	ServerBattle::BeyondBattleResult toClients;
	MatchDeque fDeque;
	MatchDeque tDeque;
	int findex = 0;
	int sindex = 0;
	int score = 0;

	BeyondBattleMgr::GetInstance()->GetBattleGroup(m_sceneID, fDeque, tDeque);
	findex = CCrossSvrCompeteLoader::GetInstance()->GetIDByScore(fDeque.score);
	sindex = CCrossSvrCompeteLoader::GetInstance()->GetIDByScore(tDeque.score);

	switch(isSuccess)
	{
	case 1://赢了
		{
			CCrossSvrCompeteLoader::GetInstance()->GetVictoryRewardScore(findex, sindex, score);

			toClientf.set_ret(1);
			GetBeyondSvrBattleResult(score, toClientf, fDeque.member);
			SynchWorldBeyondResult(score, toClientf, fDeque.member, true);

			toClients.set_ret(2);
			GetBeyondSvrBattleResult(score, toClients, tDeque.member);
			SynchWorldBeyondResult(score, toClients, tDeque.member);

			break;
		}
	case 2://输了
		{
			CCrossSvrCompeteLoader::GetInstance()->GetVictoryRewardScore(sindex, findex, score);

			toClientf.set_ret(2);
			GetBeyondSvrBattleResult(score, toClientf, fDeque.member);
			SynchWorldBeyondResult(score, toClientf, fDeque.member);

			toClients.set_ret(1);
			GetBeyondSvrBattleResult(score, toClients, tDeque.member);
			SynchWorldBeyondResult(score, toClients, tDeque.member, true);

			break;
		}
	case 3://平了
		{
			toClientf.set_ret(3);
			GetBeyondSvrBattleResult(0, toClientf, fDeque.member);
			SynchWorldBeyondResult(0, toClientf, fDeque.member);

			toClients.set_ret(3);
			GetBeyondSvrBattleResult(0, toClients, tDeque.member);
			SynchWorldBeyondResult(0, toClients, tDeque.member);

			break;
		}
	}

	BeyondBattleMgr::GetInstance()->DeleteFight(m_sceneID);
}

void CBeyondCarbon::SetContentToWS(Smart_Ptr<Player> &player, ServerBattle::BroadCastPKResult &toWS)
{
	ServerBattle::BeyondPlayerInfo * info = toWS.mutable_info();

	toWS.set_charname(player->GetName());
	toWS.set_profession(player->getAttManage()->getValue<BYTE>(eCharProfession));

	info->set_sex(player->GetSex());
	info->set_hp(player->getAttManage()->getValue<int>(eCharHP));
	info->set_hpmax(player->getAttManage()->getValue<int>(eCharHPMax));
	info->set_mp(player->getAttManage()->getValue<int>(eCharMP));
	info->set_mpmax(player->getAttManage()->getValue<int>(eCharMPMax));
	info->set_strength(player->getAttManage()->getValue<int>(eCharStrength));
	info->set_cleverpower(player->getAttManage()->getValue<int>(eCleverPower));
	info->set_cleverair(player->getAttManage()->getValue<int>(eCleverAir));
	info->set_physicalpower(player->getAttManage()->getValue<int>(eCharPhysicalPower));
	info->set_patience(player->getAttManage()->getValue<int>(eCharPatience));
	info->set_agility(player->getAttManage()->getValue<int>(eCharAgility));
	info->set_power(player->getAttManage()->getValue<int>(ePower));
	info->set_phyattackmax(player->getAttManage()->getValue<int>(eCharPhyAttackMax));
	info->set_phydefence(player->getAttManage()->getValue<int>(eCharPhyDefence));
	info->set_hitrate(player->getAttManage()->getValue<int>(eCharHitRate));
	info->set_avoidrate(player->getAttManage()->getValue<int>(eCharAvoidRate));
	info->set_crackdownrate(player->getAttManage()->getValue<int>(eCrackDownRate));
	info->set_baserte(player->getAttManage()->getValue<int>(eAvoidCrackRate));
	info->set_fight(player->getAttManage()->getValue<int>(eFightPower));
	info->set_powermax(player->getAttManage()->getValue<int>(ePowerMax));
	info->set_exp(player->getAttManage()->getValue<int64>(eCharExp));
	info->set_lv(player->getAttManage()->getValue<int>(eCharLv));
	info->set_guildname(player->GetClanName());
	info->set_guildjob(player->getAttManage()->getValue<BYTE>(eCharFaJob));
	info->set_titleid(player->getAttManage()->getValue<int>(eTitleID));
	info->set_pkcount(player->getAttManage()->getValue<int>(ePKCount));
	info->set_viplv(player->getAttManage()->getValue<int>(eVIPLv));
	info->set_country(player->getAttManage()->getValue<int>(eCharCountry));

	if(player->GetMagicWeapon())
	{
		int magicid = player->GetMagicWeapon()->GetMagicWeapon();
		info->set_magicequipid(magicid);
	}

	PlayerInfo::EquipQuickInfo eqInfo;
	Smart_Ptr<ArticleEquipMgr> mgr = player->GetContainer(eEquipQuickType);
	if(!mgr)
	{
		return;
	}

	mgr->GetArticleInfo(&eqInfo);
	for(int i = 0; i < eqInfo.equip_size(); i++)
	{
		ServerBattle::ItemInfo *itemInfo = info->add_equip();
		PlayerInfo::ItemInfo *dbitemInfo = eqInfo.mutable_equip(i);

		itemInfo->set_endtime(dbitemInfo->endtime());
		itemInfo->set_flag(dbitemInfo->flag());
		itemInfo->set_id(dbitemInfo->id());

		for(int j=0; j<dbitemInfo->newattr_size(); ++j)
		{
			itemInfo->add_newattr(dbitemInfo->newattr(j));
			itemInfo->add_refineattrlv(dbitemInfo->refineattrlv(j));
		}

		for(int k=0; k<dbitemInfo->stoneid_size(); ++k)
		{
			itemInfo->add_stoneid(dbitemInfo->stoneid(k));
			itemInfo->add_inlaypos(dbitemInfo->inlaypos(k));
		}
	}
}

void CBeyondCarbon::Over()
{
	SynchBeyondSvrBattleResult(3);
	m_process = eDuplicateChangeMap;
}

void CBeyondCarbon::CountEx()
{
	int64 nowTime = CUtil::GetNowSecond();

	if(nowTime - m_leaveTime >= m_data->m_nDelTime)
	{
		m_process = eOver;
	}
}

void CBeyondCarbon::Count()
{
	m_process = eOver;
}

void CBeyondCarbon::Failed()
{
	SynchBeyondSvrBattleResult(2);
	m_process = eAward;
}

void CBeyondCarbon::Award()
{
	m_process = eDuplicateChangeMap;
	m_awardTime = CUtil::GetNowSecond();
}

bool CBeyondCarbon::HandleLeaveMapEvent(const EventArgs & e)
{
	PlayerRemoveMemberEventArgs *pArgs = (PlayerRemoveMemberEventArgs *) (&e);

	if(pArgs->m_eMethod != GROUP_REMOVEMETHOD_OUT)
		return true;

	if(pArgs->m_pObj.Get()==NULL || pArgs->m_pObj->GetMapID() != m_sceneID)
		return true;

	if(!BeyondBattleMgr::GetInstance()->IsFightDeque(m_sceneID, pArgs->m_pObj->GetID()))
		return true;

	int64 mapid = 0;
	int xpos = 0;
	int ypos = 0;

	map<int64,OldMapInfo>::iterator itMap = m_oldMapPoint.find(pArgs->m_pObj->GetID());
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
		if (sMapInfo == 0)
			return false;

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

	pArgs->m_pObj->SetMapID(mapid);
	pArgs->m_pObj->SetXPos(xpos);
	pArgs->m_pObj->SetYPos(ypos);
	pArgs->m_pObj->SetXNewPos();
	pArgs->m_pObj->SetYNewPos();
//	pArgs->m_pObj->SetGroup(CSceneMgr::GetInstance()->GetSectorGroup(pArgs->m_pObj->GetMapID(), pArgs->m_pObj->GetXPos(), pArgs->m_pObj->GetYPos()));
	pArgs->m_pObj->SetDataFlag(eBaseInfo);

	return true;
}

bool CBeyondCarbon::HandlePlayerDeadEvent(const EventArgs & e)
{
	if(m_process != ePlayerWork)
		return true;

	PlayerKilledEventArgs *tArgs = (PlayerKilledEventArgs *)(&e);
	if(tArgs->m_dead.Get()==NULL || tArgs->m_killer.Get()==NULL)
		return true;

	if(GetSceneID() != tArgs->m_dead->GetMapID())
		return true;

	if(tArgs->m_dead->GetType() == ePlayer)
	{
		bool fChar = BeyondBattleMgr::GetInstance()->IsFirstAttack(m_sceneID, tArgs->m_dead->GetID());

		INT64PLAYERMAP::iterator it = m_player.begin();
		for(; it!=m_player.end(); ++it)
		{
			if(it->second.Get()==NULL)
				continue;

			if(tArgs->m_dead->GetID() == it->second->GetID())
				continue;

			if(fChar == BeyondBattleMgr::GetInstance()->IsFirstAttack(m_sceneID, it->second->GetID()))
			{
				if(!it->second->IsDead())
				{
					break;
				}
			}
		}

		if(it == m_player.end())
		{
			if(fChar)
			{
				m_process = eFailed;
			}
			else
			{
				m_process = eWinFight;
			}
		}
	}

	return true;
}

bool CBeyondCarbon::HandlePlayerAttackEvent(const EventArgs & e)
{
	PlayerAttackCreature *pArgs = (PlayerAttackCreature *) (&e);

	if(pArgs->m_player.Get()==NULL || pArgs->m_player->GetMapID() != m_sceneID)
	{
		return true;
	}

	BeyondBattleMgr::GetInstance()->AddAttackValue(m_sceneID, pArgs->m_player->GetID(), pArgs->m_value);

	return true;
}

bool CBeyondCarbon::IsAttacked(int64 fight, int64 attacked)
{
	bool fChar = BeyondBattleMgr::GetInstance()->IsFirstAttack(m_sceneID, fight);
	if(fChar == BeyondBattleMgr::GetInstance()->IsFirstAttack(m_sceneID, attacked))
		return false;
	return true;
}

void CBeyondCarbon::PersionalBeyondSvrBattleResult(Smart_Ptr<Player> & player,bool fChar)
{
	MatchDeque fDeque;
	MatchDeque tDeque;
	int findex = 0;
	int sindex = 0;
	int score = 0;
	ServerBattle::BroadCastPKResult toWS;
	vector<SinglePlayerDeque> vec;

	findex = CCrossSvrCompeteLoader::GetInstance()->GetIDByScore(fDeque.score);
	sindex = CCrossSvrCompeteLoader::GetInstance()->GetIDByScore(tDeque.score);
	BeyondBattleMgr::GetInstance()->GetBattleGroup(m_sceneID, fDeque, tDeque);

	if(fChar)
	{
		vec = fDeque.member;
		CCrossSvrCompeteLoader::GetInstance()->GetVictoryRewardScore(findex, sindex, score);
	}
	else
	{
		vec = tDeque.member;
		CCrossSvrCompeteLoader::GetInstance()->GetVictoryRewardScore(sindex,findex,score);
	}

	vector<SinglePlayerDeque>::iterator itMy	= vec.begin();
	for(; itMy!=vec.end(); ++itMy)
	{
		if((*itMy).charid == player->GetID())
		{
			int tempScore = 0;
			player->GetCDMgr()->StartCD(BEYOND_CHANNEL,CCrossSvrCompeteLoader::GetInstance()->GetChallengeCDTime());
			tempScore = (*itMy).score - score;
			if(tempScore < 0)
				tempScore = 0;

			toWS.set_charid((*itMy).charid);
			toWS.set_charname(player->GetName());
			toWS.set_score(tempScore);
			toWS.set_profession(player->getAttManage()->getValue<BYTE>(eCharProfession));
			player->SetSvrScore(tempScore);
			player->SetDataFlag(true);
			SetContentToWS(player, toWS);

			//同步 最新的积分给所有的ws，用于排名
			ServerConHandler::GetInstance()->BraodcastMessageToWorld(SimpleMessage, MSG_SIM_GM2WS_PK_RESULT, &toWS);
		}
	}
}
