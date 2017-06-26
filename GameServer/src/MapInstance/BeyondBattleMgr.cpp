/*
 * BeyondBattleMgr.cpp
 *
 *  Created on: 2014年12月10日
 *      Author: helh
 */

#include "BeyondBattleMgr.h"
#include "util.h"
#include "./CrossSvrCompetitionLoader/CrossSvrCompetitionLoader.h"
#include "MessageStruct/BeyondServerBattle/BeyondBattle.pb.h"
#include "../ServerManager.h"
#include "../MessageBuild.h"
#include "SvrConfig.h"
#include "Log/Logger.h"

BeyondBattleMgr::BeyondBattleMgr():m_onlyID(0)
{
	if(m_flushTime.IsValid())
	{
		m_flushTime.SetNull();
	}
	m_flushTime = TimerInterface::GetInstance()->RegRepeatedTime(&BeyondBattleMgr::FlushDeque, this, 0, 0, 0, 1, 0, 1000);
}

BeyondBattleMgr::~BeyondBattleMgr()
{
	if(m_flushTime.IsValid())
	{
		m_flushTime.SetNull();
	}
}

bool SortPlayer(const WaitMatch & p1, const WaitMatch &p2)
{
	if(p1.type < p2.type)
	{
		return true;
	}
	else if(p1.type == p2.type)
	{
		if((p1.member.size() == (size_t)p1.type && p2.member.size() == (size_t)p2.type) || (p1.member.size() < (size_t)p1.type && p2.member.size() < (size_t)p2.type))
		{
			if(p1.score < p2.score)
			{
				return true;
			}
			else if(p1.score == p2.score && p1.fight < p2.fight)
			{
				return true;
			}
		}
		else if(p1.member.size() < (size_t)p1.type && p2.member.size() == (size_t)p2.type)
		{
			return true;
		}
	}

	return false;
}

void BeyondBattleMgr::DeleteWaitDeque()
{
	int64 nowtime = CUtil::GetNowSecond();

	vector<WaitMatch>::iterator itVec = m_waitMatch.begin();
	for(; itVec!=m_waitMatch.end(); )
	{
		int tScore = 0;
		int fight = 0;
		int64 charid = 0;

		vector<SinglePlayerDeque>::iterator itSecVec = (*itVec).member.begin();
		for(; itSecVec!=(*itVec).member.end(); )
		{
			if(nowtime - (*itSecVec).time >= CCrossSvrCompeteLoader::GetInstance()->GetWaitMatchTime())
			{
				LOG_DEBUG(FILEINFO, "\n-------------等待匹配列表［%lld］ 组[%d]超时，删除－－－－－－－－－－－－－", (*itVec).charid, (*itVec).groupID);
				(*itVec).member.erase(itSecVec);
			}
			else
			{
				if((*itSecVec).score > tScore)
				{
					tScore = (*itSecVec).score;
					fight = (*itSecVec).fight;
					charid = (*itSecVec).charid;
				}

				++itSecVec;
			}
		}

		if((*itVec).member.size() <= 0)
		{
			LOG_DEBUG(FILEINFO, "\n-------------等待匹配列表删除组[%d]－－－－－－－－－－－－－",  (*itVec).groupID);
			itVec = m_waitMatch.erase(itVec);
		}
		else
		{
			if(tScore > 0)
			{
				(*itVec).score = tScore;
				(*itVec).fight = fight;
				(*itVec).charid = charid;
			}

			++itVec;
		}
	}
}

void BeyondBattleMgr::MatchPlayer()
{
	int64 nowtime = CUtil::GetNowSecond();
	vector<WaitMatch> tSortDeque;
	size_t i = 0;

	if(m_waitMatch.size() <= 0)
		return;

	//先排序,再临时保存一份,清空内存
	sort(m_waitMatch.begin(), m_waitMatch.end(), SortPlayer);
	tSortDeque = m_waitMatch;
	m_waitMatch.clear();

	//删除匹配到的，把不能匹配的继续加到内存中
	for(; i<tSortDeque.size()-1; )
	{
		int bScore = 0;
		int eScore = 0;

		if(nowtime - tSortDeque[i].time >= 20 * 1000
				&& tSortDeque[i].type == tSortDeque[i+1].type
				&& tSortDeque[i].member.size() == (size_t)tSortDeque[i].type
				&& tSortDeque[i+1].member.size() == (size_t)tSortDeque[i+1].type)
		{//规定，超过20秒没有匹配到的队伍，直接给匹配旁边的队伍
			LOG_DEBUG(FILEINFO, "\n-------------匹配队伍超时20秒 ，直接加入到匹配列表中－－－－－－－－－－－－－");
			MatchPlayer(tSortDeque[i], tSortDeque[i+1]);
			i += 2;

			continue;
		}
		else if(tSortDeque[i].type != tSortDeque[i+1].type)
		{//不想同的类型，直接跳过
			m_waitMatch.push_back(tSortDeque[i]);
			++i;

			continue;
		}

		CCrossSvrCompeteLoader::GetInstance()->GetCanPkScoreByMyScore(tSortDeque[i].score, bScore, eScore);
		if(tSortDeque[i].type == tSortDeque[i+1].type
				&& tSortDeque[i].member.size() == (size_t)tSortDeque[i].type
				&& tSortDeque[i+1].member.size() == (size_t)tSortDeque[i+1].type
				&& tSortDeque[i+1].score >= bScore && tSortDeque[i+1].score <= eScore)
		{//人数满了，而且类型相同,段位相单，就可以匹配
			LOG_DEBUG(FILEINFO, "\n-------------人数满了，而且类型相同,段位相单，直接加入到匹配列表中－－－－－－－－－－－－－");
			MatchPlayer(tSortDeque[i], tSortDeque[i+1]);
			i += 2;

			continue;
		}

		m_waitMatch.push_back(tSortDeque[i]);
		++i;
	}

	if(i == tSortDeque.size()-1)
	{
		m_waitMatch.push_back(tSortDeque[i]);
	}
}

void BeyondBattleMgr::DeleteMatchDeque()
{
	int64 nowtime = CUtil::GetNowSecond();

	//把匹配中的超时的删除
	map<int, MatchDeque>::iterator it = m_match.begin();
	for(; m_match.end()!=it; )
	{
		if((nowtime - it->second.matchTime >= CCrossSvrCompeteLoader::GetInstance()->GetInSceneDelay()))
		{
			LOG_DEBUG(FILEINFO, "\n-------------匹配列表 组［%d］超时，删除,并且重新加入到等待列表中－－－－－－－－－－－－－", it->first);
			AddWaitMatch(it->second);
			m_match.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void BeyondBattleMgr::DeleteFightDeque()
{
	int64 nowtime = CUtil::GetNowSecond();

	map<int64, MatchFight>::iterator itfight = m_fight.begin();
	for(; itfight!=m_fight.end(); )
	{
		if(nowtime - itfight->second.time >= CCrossSvrCompeteLoader::GetInstance()->GetCompeteSumTime())
		{
			LOG_DEBUG(FILEINFO, "\n-------------战斗列表超时   主动组［%d］   防守组［%d］，删除－－－－－－－－－－－－－", itfight->second.myself.groupID, itfight->second.target.groupID);
			m_fight.erase(itfight++);
		}
		else
		{
			++itfight;
		}
	}
}

void BeyondBattleMgr::FlushDeque(void *arg)
{
	//先把超时的去掉
	DeleteMatchDeque();
	DeleteWaitDeque();
	DeleteFightDeque();

	// 匹配
	MatchPlayer();
}

bool BeyondBattleMgr::IsFirstAttack(int64 mapid, int64 charid)
{
	map<int64, MatchFight>::iterator itMap = m_fight.find(mapid);
	if(itMap != m_fight.end())
	{
		vector<SinglePlayerDeque>::iterator itMy = itMap->second.myself.member.begin();
		for(; itMy!=itMap->second.myself.member.end(); ++itMy)
		{
			if(charid == (*itMy).charid)
			{
				return true;
			}
		}
	}

	return false;
}

bool BeyondBattleMgr::IsFightDeque(int64 mapid, int64 charid)
{
	map<int64, MatchFight>::iterator itMap = m_fight.find(mapid);
	if(itMap != m_fight.end())
	{
		vector<SinglePlayerDeque>::iterator itMy = itMap->second.myself.member.begin();
		for(; itMy!=itMap->second.myself.member.end(); ++itMy)
		{
			if(charid == (*itMy).charid)
			{
				return true;
			}
		}

		vector<SinglePlayerDeque>::iterator itTar = itMap->second.target.member.begin();
		for(; itTar!=itMap->second.target.member.end(); ++itTar)
		{
			if(charid == (*itTar).charid)
			{
				return true;
			}
		}
	}

	return false;
}

void BeyondBattleMgr::AddAttackValue(int64 mapid, int64 charid, int value)
{
	map<int64, MatchFight>::iterator itMap = m_fight.find(mapid);
	if(itMap != m_fight.end())
	{
		vector<SinglePlayerDeque>::iterator itMy = itMap->second.myself.member.begin();
		for(; itMy!=itMap->second.myself.member.end(); ++itMy)
		{
			if(charid == (*itMy).charid)
			{
				(*itMy).attack += value;

				break;
			}
		}

		vector<SinglePlayerDeque>::iterator itTar = itMap->second.target.member.begin();
		for(; itTar!=itMap->second.target.member.end(); ++itTar)
		{
			if(charid == (*itTar).charid)
			{
				(*itTar).attack += value;
			}
		}
	}
}

void BeyondBattleMgr::GetBattleGroup(int64 mapid, MatchDeque &first, MatchDeque & second)
{
	map<int64, MatchFight>::iterator itMap = m_fight.find(mapid);
	if(itMap != m_fight.end())
	{
		first = itMap->second.myself;
		second = itMap->second.target;
	}
}

void BeyondBattleMgr::GetPlayerIndexAndType(int64 mapid, int64 charid, int &index, int &type)
{
	int tIndex = 0;

	map<int64, MatchFight>::iterator itMap = m_fight.find(mapid);
	if(itMap != m_fight.end())
	{
		vector<SinglePlayerDeque>::iterator itMy = itMap->second.myself.member.begin();
		for(; itMy!=itMap->second.myself.member.end(); ++itMy)
		{
			++tIndex;
			if(charid == (*itMy).charid)
			{
				index = tIndex;
				type = itMap->second.myself.type;
				return;
			}
		}

		tIndex = itMap->second.myself.type;
		vector<SinglePlayerDeque>::iterator itTar = itMap->second.target.member.begin();
		for(; itTar!=itMap->second.target.member.end(); ++itTar)
		{
			++tIndex;
			if(charid == (*itTar).charid)
			{
				index = tIndex;
				type = itMap->second.target.type;
				return;
			}
		}
	}
}

void BeyondBattleMgr::AddWaitMatch(MatchDeque fGroup)
{
	WaitMatch tWait;

	tWait.charid = fGroup.charid;
	tWait.fight = fGroup.fight;
	tWait.groupID = fGroup.groupID;
	tWait.member = fGroup.member;
	tWait.score = fGroup.score;
	tWait.time = fGroup.time;
	tWait.type = fGroup.type;

	m_waitMatch.push_back(tWait);
	LOG_DEBUG(FILEINFO, "\n-------------加入到等待列表中    组［%d］－－－－－－－－－－－－－", tWait.groupID);
}

bool BeyondBattleMgr::MatchPlayer(WaitMatch fGroup, WaitMatch tGroup)
{
	MatchDeque fDeque;
	MatchDeque tDeque;
	int64 nowtime = CUtil::GetNowSecond();

	fDeque.groupID = fGroup.groupID;
	fDeque.targetID = tGroup.groupID;
	fDeque.matchTime = nowtime;
	fDeque.member = fGroup.member;
	fDeque.time = fGroup.time;
	fDeque.type = fGroup.type;
	fDeque.score = fGroup.score;
	fDeque.fight = fGroup.fight;
	fDeque.charid = fGroup.charid;

	tDeque.groupID = tGroup.groupID;
	tDeque.targetID = fGroup.groupID;
	tDeque.matchTime = nowtime;
	tDeque.member = tGroup.member;
	tDeque.time = tGroup.time;
	tDeque.type = tGroup.type;
	tDeque.score = tGroup.score;
	tDeque.fight = tGroup.fight;
	tDeque.charid = tGroup.charid;

	LOG_DEBUG(FILEINFO, "\n-------------加入到匹配队伍进攻组［%d］ 防守组［%d］ －－－－－－－－－－－－－", fDeque.groupID, tDeque.groupID);
	m_match[fDeque.groupID] = fDeque;
	m_match[tDeque.groupID] = tDeque;

	//通知所有人的world已经匹配到了
	vector<SinglePlayerDeque>::iterator fVec = fDeque.member.begin();
	for(; fVec!=fDeque.member.end(); ++fVec)
	{
		ServerBattle::ToWSMatchedPlayer toWS;
		Smart_Ptr<SvrItem> worldSvr;

		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID((*fVec).charid),worldSvr);
		if(worldSvr)
		{
			LOG_DEBUG(FILEINFO, "\n-------------加入到匹配队伍，告诉世界服匹配到了［%lld］ －－－－－－－－－－－－－", (*fVec).charid);
			toWS.set_charid((*fVec).charid);
			toWS.set_target(fDeque.targetID);
			toWS.set_id(GET_SERVER_CHARID(tGroup.charid));
			toWS.set_myself(fDeque.groupID);

			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_MATCHED_PLAYER, &toWS, worldSvr.Get(), SimpleMessage);
			Message_Facade::Send(messRet);
		}
	}

	vector<SinglePlayerDeque>::iterator tVec = tDeque.member.begin();
	for(; tVec!=tDeque.member.end(); ++tVec)
	{
		ServerBattle::ToWSMatchedPlayer toWS;
		Smart_Ptr<SvrItem> worldSvr;

		ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID((*tVec).charid),worldSvr);
		if(worldSvr)
		{
			LOG_DEBUG(FILEINFO, "\n-------------加入到匹配队伍，告诉世界服匹配到了［%lld］ －－－－－－－－－－－－－", (*tVec).charid);
			toWS.set_charid((*tVec).charid);
			toWS.set_target(tDeque.targetID);
			toWS.set_id(GET_SERVER_CHARID(fGroup.charid));
			toWS.set_myself(tGroup.groupID);

			Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_MATCHED_PLAYER, &toWS, worldSvr.Get(), SimpleMessage);
			Message_Facade::Send(messRet);
		}
	}

	return true;
}

bool BeyondBattleMgr::IsAddWaitMatch(int64 id)
{
	int64 nowtime = CUtil::GetNowSecond();

	vector<WaitMatch>::iterator itVec = m_waitMatch.begin();
	for(; itVec!=m_waitMatch.end(); )
	{
		vector<SinglePlayerDeque>::iterator itSecVec = (*itVec).member.begin();
		for(; itSecVec!=(*itVec).member.end(); ++itSecVec)
		{
			if((*itSecVec).charid == id)
			{
				LOG_DEBUG(FILEINFO, "\n-----------------判断是否能加入等待匹配列表，但是已经有了，先删除等待匹配列表［%lld］－－－－－－－－－－－－－", id);
				(*itVec).member.erase(itSecVec);
				break;
			}
		}

		if((*itVec).member.size() <= 0)
		{
			itVec = m_waitMatch.erase(itVec);
		}
		else
		{
			++itVec;
		}
	}

	//如果已经匹配到了，而且没有超时的情况下，就不允许加入
	map<int, MatchDeque>::iterator it = m_match.begin();
	for(; it!=m_match.end(); ++it)
	{
		vector<SinglePlayerDeque>::iterator itSecVec = it->second.member.begin();
		for(; itSecVec!=it->second.member.end(); ++itSecVec)
		{
			if(id == (*itSecVec).charid)
			{
				LOG_DEBUG(FILEINFO, "\n-----------------判断是否能加入等待匹配列表，但是匹配连表已经有了［%lld］－－－－－－－－－－－－－", id);
				return false;
//				if((nowtime - it->second.matchTime < CCrossSvrCompeteLoader::GetInstance()->GetInSceneDelay()))
//				{
//					LOG_DEBUG(FILEINFO, "\n-----------------判断是否能加入等待匹配列表，但是匹配连表已经有了，而且匹配没有超时，［%lld］－－－－－－－－－－－－－", id);
//				}
//				else
//				{
//					LOG_DEBUG(FILEINFO, "\n-----------------判断是否能加入等待匹配列表，但是匹配连表已经有了，而且匹配没有超时，［%lld］－－－－－－－－－－－－－", id);
//					isEnd = true;
//					break;
//				}
			}
		}
	}

	map<int64, MatchFight>::iterator itfight = m_fight.begin();
	for(; itfight!=m_fight.end(); ++itfight)
	{
		vector<SinglePlayerDeque>::iterator itMy = itfight->second.myself.member.begin();
		for(; itMy!=itfight->second.myself.member.end(); ++itMy)
		{
			if(id == (*itMy).charid && (nowtime - itfight->second.time) < CCrossSvrCompeteLoader::GetInstance()->GetCompeteSumTime())
			{
				LOG_DEBUG(FILEINFO, "\n-----------------判断是否能加入等待匹配列表，但是战斗连表已经有了， 而且没有超时［%lld］－－－－－－－－－－－－－", id);
				return false;
			}
			else
			{
				LOG_DEBUG(FILEINFO, "\n-----------------判断是否能加入等待匹配列表，但是战斗连表已经有了， 而且已经超时［%lld］－－－－－－－－－－－－－", id);
			}
		}

		vector<SinglePlayerDeque>::iterator itTar = itfight->second.target.member.begin();
		for(; itTar!=itfight->second.target.member.end(); ++itTar)
		{
			if(id == (*itTar).charid && (nowtime - itfight->second.time) < CCrossSvrCompeteLoader::GetInstance()->GetCompeteSumTime())
			{
				LOG_DEBUG(FILEINFO, "\n-----------------判断是否能加入等待匹配列表，但是战斗连表已经有了， 而且没有超时［%lld］－－－－－－－－－－－－－", id);
				return false;
			}
			else
			{
				LOG_DEBUG(FILEINFO, "\n-----------------判断是否能加入等待匹配列表，但是战斗连表已经有了， 而且已经超时［%lld］－－－－－－－－－－－－－", id);
			}
		}
	}

	return true;
}

void BeyondBattleMgr::AddWaitMatch(int64 id, int lv, int fight, int score, int type)
{
	SinglePlayerDeque vec;

	vec.charid = id;
	vec.time = CUtil::GetNowSecond();
	vec.lv = lv;
	vec.fight = fight;
	vec.score = score;

	if(type > eBeyondBattleOne)
	{
		//多人的都得找一个空位，如果没有就加入一个新的组
		vector<WaitMatch>::iterator it = m_waitMatch.begin();
		for(; it!=m_waitMatch.end(); ++it)
		{
			if((*it).type == type && (*it).member.size() < (size_t)type)
			{
				if((*it).score < score)
				{
					(*it).score = score;
					(*it).fight = fight;
					(*it).charid = id;
				}

				LOG_DEBUG(FILEINFO, "\n-----------------加入等待匹配列表［%lld］  组［%d］－－－－－－－－－－－－－", id, (*it).groupID);
				(*it).member.push_back(vec);

				return;
			}
		}
	}

	WaitMatch item;

	item.groupID = ++m_onlyID;
	item.type = type;
	item.score = score;
	item.fight = fight;
	item.charid = id;
	item.time = vec.time;
	item.member.push_back(vec);
	m_waitMatch.push_back(item);
	LOG_DEBUG(FILEINFO, "\n-----------------加入等待匹配列表［%lld］  组［%d］－－－－－－－－－－－－－", id, item.groupID);
}

void BeyondBattleMgr::DeletePlayerInfo(int64 id)
{
	bool isFind = false;

	vector<WaitMatch>::iterator it = m_waitMatch.begin();
	for(; it!=m_waitMatch.end(); )
	{
		int tScore = 0;
		int fight = 0;
		int64 charid = 0;

		vector<SinglePlayerDeque>::iterator itSecVec = (*it).member.begin();
		for(; itSecVec!=(*it).member.end(); )
		{
			if((*itSecVec).charid == id)
			{
				LOG_DEBUG(FILEINFO, "\n-----------------已经匹配到了。从等待匹配列表中删除［%lld］－－－－－－－－－－－－－", id);
				itSecVec = (*it).member.erase(itSecVec);
				isFind = true;
			}
			else
			{
				if((*itSecVec).score > tScore)
				{
					tScore = (*itSecVec).score;
					fight = (*itSecVec).fight;
					charid = (*itSecVec).charid;
				}

				++itSecVec;
			}
		}

		if((*it).member.size() <= 0)
		{
			it = m_waitMatch.erase(it);
		}
		else
		{
			if(tScore > 0)
			{
				(*it).score = tScore;
				(*it).fight = fight;
				(*it).charid = charid;
			}

			++it;
		}

		if(isFind)
		{
			break;
		}
	}

	isFind = false;
	map<int, MatchDeque>::iterator itMatch = m_match.begin();
	for(; itMatch!=m_match.end(); ++itMatch)
	{
		int tScore = 0;
		int fight = 0;
		int64 charid = 0;

		vector<SinglePlayerDeque>::iterator itSecVec = itMatch->second.member.begin();
		for(; itSecVec!=itMatch->second.member.end(); )
		{
			if((*itSecVec).charid == id)
			{
				LOG_DEBUG(FILEINFO, "\n-----------------已经匹配到了。从匹配列表中删除［%lld］－－－－－－－－－－－－－", id);
				itSecVec = itMatch->second.member.erase(itSecVec);
				isFind = true;
			}
			else
			{
				if((*itSecVec).score > tScore)
				{
					tScore = (*itSecVec).score;
					fight = (*itSecVec).fight;
					charid = (*itSecVec).charid;
				}

				++itSecVec;
			}
		}

		if(tScore > 0)
		{
			(*it).score = tScore;
			(*it).fight = fight;
			(*it).charid = charid;
		}

		++it;
		if(isFind)
		{
			break;
		}
	}

//	map<int64, MatchFight>::iterator itMap = m_fight.begin();
//	for(; itMap!=m_fight.end(); ++itMap)
//	{
//		if(id == itMap->second.myself || id == itMap->second.target)
//		{
//			printf("\n------------玩家［%lld］ 已经匹配到了，从pk匹配列表中删除－－－－－－－－－－－－－", id);
//			m_fight.erase(itMap);
//			break;
//		}
//	}
}

void BeyondBattleMgr::CancelMatch(int64 id)
{
	bool isFind = false;
	MatchDeque tDeque;

	vector<WaitMatch>::iterator it = m_waitMatch.begin();
	for(; it!=m_waitMatch.end(); )
	{
		int tScore = 0;
		int fight = 0;
		int64 charid = 0;

		vector<SinglePlayerDeque>::iterator itSecVec = (*it).member.begin();
		for(; itSecVec!=(*it).member.end(); )
		{
			if((*itSecVec).charid == id)
			{
				LOG_DEBUG(FILEINFO, "\n-----------------取消匹配。从等待匹配列表中删除［%lld］－－－－－－－－－－－－－", id);
				itSecVec = (*it).member.erase(itSecVec);
				isFind = true;
			}
			else
			{
				if((*itSecVec).score > tScore)
				{
					tScore = (*itSecVec).score;
					fight = (*itSecVec).fight;
					charid = (*itSecVec).charid;
				}

				++itSecVec;
			}
		}

		if((*it).member.size() <= 0)
		{
			it = m_waitMatch.erase(it);
		}
		else
		{
			if(tScore > 0)
			{
				(*it).score = tScore;
				(*it).fight = fight;
				(*it).charid = charid;
			}

			++it;
		}

		if(isFind)
		{
			break;
		}
	}

	isFind = false;
	map<int, MatchDeque>::iterator itMatch = m_match.begin();
	for(; itMatch!=m_match.end(); ++itMatch)
	{
		int tScore = 0;
		int fight = 0;
		int64 charid = 0;

		vector<SinglePlayerDeque>::iterator itSecVec = itMatch->second.member.begin();
		for(; itSecVec!=itMatch->second.member.end(); )
		{
			if((*itSecVec).charid == id)
			{
				LOG_DEBUG(FILEINFO, "\n-----------------取消匹配。从等待匹配列表中删除［%lld］－－－－－－－－－－－－－", id);
				itSecVec = itMatch->second.member.erase(itSecVec);
				isFind = true;
			}
			else
			{
				if((*itSecVec).score > tScore)
				{
					tScore = (*itSecVec).score;
					fight = (*itSecVec).fight;
					charid = (*itSecVec).charid;
				}

				++itSecVec;
			}
		}

		if(isFind)
		{
			tDeque = itMatch->second;
			m_match.erase(itMatch);
			if(tScore > 0)
			{
				tDeque.score = tScore;
				tDeque.fight = fight;
				tDeque.charid = charid;
			}

			break;
		}
	}

	if(isFind)
	{
		itMatch = m_match.find(tDeque.targetID);
		if(itMatch != m_match.end())
		{
			AddWaitMatch(itMatch->second);
		}

		if(tDeque.member.size() > 0)
		{
			AddWaitMatch(tDeque);
		}
	}
}

int BeyondBattleMgr::AddMapIDToMatch(int64 charid, int64 mapid, int myself, int target, int64 &targetMap)
{
	bool isEnd = true; //判断是否一个组里的成员都有了地图id

	map<int, MatchDeque>::iterator itMy = m_match.find(myself);
	if(itMy != m_match.end())
	{
		if(target != itMy->second.targetID)
			return -1;

		vector<SinglePlayerDeque>::iterator itSecVec = itMy->second.member.begin();
		for(; itSecVec!=itMy->second.member.end(); ++itSecVec)
		{
			if(charid == (*itSecVec).charid)
			{
				(*itSecVec).mapid = mapid;
			}
			else if((*itSecVec).mapid > 0)
			{
				targetMap = (*itSecVec).mapid;
				(*itSecVec).mapid = mapid;
			}

			if((*itSecVec).mapid <= 0)
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return -1;
	}

	map<int, MatchDeque>::iterator itTar = m_match.find(target);
	if(itTar != m_match.end())
	{
		if(myself != itTar->second.targetID)
			return -1;

		vector<SinglePlayerDeque>::iterator itSecVec = itTar->second.member.begin();
		for(; itSecVec!=itTar->second.member.end(); ++itSecVec)
		{
			if((*itSecVec).mapid > 0)
			{
				targetMap = (*itSecVec).mapid;
				(*itSecVec).mapid = mapid;
			}

			if((*itSecVec).mapid <= 0)
			{
				isEnd = false;
			}
		}
	}
	else
	{
		return -1;
	}

	if(!isEnd)
	{
		return 1;
	}

	return 0;
}

void BeyondBattleMgr::AddFight(int64 mapid, int myself, int target)
{
	MatchFight tFight;

	std::map<int, MatchDeque>::iterator itMy = m_match.find(myself);
	if(itMy != m_match.end())
	{
		tFight.myself = itMy->second;
		m_match.erase(itMy);
	}

	std::map<int, MatchDeque>::iterator itTar = m_match.find(target);
	if(itTar != m_match.end())
	{
		tFight.target = itTar->second;
		m_match.erase(itTar);
	}

	tFight.time = CUtil::GetNowSecond();
	m_fight[mapid] = tFight;
}

void BeyondBattleMgr::DeleteFight(int64 mapid)
{
	map<int64, MatchFight>::iterator itMap = m_fight.find(mapid);
	if(itMap != m_fight.end())
	{
		LOG_DEBUG(FILEINFO, "\n------------玩家组［%d］ 组[%d] 已经打完了，从pk匹配列表中删除－－－－－－－－－－－－－", itMap->second.myself.groupID, itMap->second.target.groupID);
		m_fight.erase(itMap);
	}
}


