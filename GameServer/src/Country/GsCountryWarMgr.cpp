/*
 * GsCountryWarMgr.cpp
 *
 *  Created on: 2015年4月17日
 *      Author: root
 */
#include "GsCountryWarMgr.h"
#include "Network/Message_Facade.h"
#include "../MessageBuild.h"
#include "../ServerManager.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "CharDefine.h"
#include "../QuickMessage.h"
#include "../EventSet/EventDefine.h"
#include "../ServerEventDefine.h"
#include "stdlib.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"
#include "playerCampShopMgr.h"
#include "../ServerEventDefine.h"
#include "MessageStruct/Country/CountryWar.pb.h"
#include "GsCountryWarLoader.h"
#include "../Mail/MailMessage.h"
#include "../Map/GameZone.h"
#include "../Map/GameDuplicate.h"
#include "../MapCarbon/CountryWarCarbon.h"
#include "./FileLoader/InternationalLoader.h"
#include "FileLoader/ActivitiesTimerLoader.h"

GsCountryWarMgr::GsCountryWarMgr():m_mapID(0),m_time(0)
{
		DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_ELEMENTS_INFO);
		DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_WAR_DATA);
		DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_FITCH_TASK_REWARD);
		DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_WAR_HISTORY);
		DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_OCCUPY_ELEMENTS);
		DEF_MSG_QUICK_REQUEST_REG_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_WAR_MAP);

		DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GS_COUNTRY_WAR_MAP);
		DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GS_WAR_RECORD);
		DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_COUNTRYBATTLEOPEN);
}

GsCountryWarMgr::~GsCountryWarMgr()
{

}

int GsCountryWarMgr::getWinCount(int country)
{
	std::map<int,CountryWarRecord>::const_iterator it= m_historyRecord.find(country);
	if(it!=m_historyRecord.end())
		return it->second.Win;
	return 0;
}

void GsCountryWarMgr::noticeWarResult(short int WorlserverID,int win,int lose)
{
	//
	std::map<int,CountryWarRecord>::iterator itWin= m_historyRecord.find(win);
	if(itWin!=m_historyRecord.end())
		itWin->second.Win++;

	std::map<int,CountryWarRecord>::iterator itLose= m_historyRecord.find(lose);
	if(itLose!=m_historyRecord.end())
		itLose->second.Lose++;

	//通知世界服
	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(WorlserverID,worldSvr);

	if(worldSvr.Get()==NULL)
	{
			return;
	}

	CountryWar::CountryWarResult ret;
	ret.set_win(win);
	ret.set_lose(lose);
	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GS2WS_WAR_RESULT, &ret, worldSvr.Get(), SimpleMessage);
	Message_Facade::Send(messRet);
}

void GsCountryWarMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
			DEF_SWITCH_TRY_DISPATCH_BEGIN
			DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GS_COUNTRY_WAR_MAP);
			DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GS_WAR_RECORD);
			DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_COUNTRYBATTLEOPEN);
			DEF_SWITCH_TRY_DISPATCH_END
}

void GsCountryWarMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{

}

void GsCountryWarMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{

}

DEF_MSG_SIMPLE_DEFINE_FUN(GsCountryWarMgr,MSG_SIM_WS2GS_WAR_RECORD)
{
	CountryWar::HistoryRecord req;

	int len=0;
	char* con = message->GetBuffer(len);
	req.ParseFromArray(con,len);

	for(int i=0; i<req.item_size(); ++i)
	{
		const CountryWar::RecordItem& item = req.item(i);
		CountryWarRecord record;
		record.Win=item.win();
		record.Lose=item.lose();
		m_historyRecord[item.id()] = record;
	}
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_ELEMENTS_INFO)
{
		CGameZone *pZone = player->GetGameZone();
		if(pZone==NULL||pZone->GetSceneID()!=getMapID())
			return;

		//得到五行柱的占领状态信息
		std::map<int64,CountryWarElementInfo> info;
		((CCountryWarCarbon*)(pZone))->getElementInfo(info);

		int64 curtime=CUtil::GetNowSecond();//取当前时间
		CountryWar::AckReqElementInfo ret;
		std::map<int64,CountryWarElementInfo>::const_iterator it=info.begin();
		for(; it!=info.end(); ++it)
		{
				CountryWar::ElementInfo* pItem = ret.add_item();
				pItem->set_id(it->second.objectKey);
				pItem->set_ower(it->second.owner);
				pItem->set_monster(it->second.npcID);

				int64 dx=it->second.time;
				dx=curtime-it->second.time;

				if(it->second.time>0 && it->second.time<curtime)
					pItem->set_time(curtime-it->second.time);
				else
					pItem->set_time(0);
		}
		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_COUNTRY_ELEMENTS_INFO);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_WAR_DATA)
{
		CGameZone *pZone = player->GetGameZone();
		if(pZone==NULL||pZone->GetSceneID()!=getMapID())
		{
			return;
		}

		PersonCountryWarInfo info;
		CountryWarData score;
		((CCountryWarCarbon*)(pZone))->getWarInfo(player->GetID(),info,score);

		//时间
		int timeleft=0;
		DWORD64 timedx= CUtil::GetNowSecond()-getTime();
		InstanceMapEntry* pInstance = InstanceLoader::GetInstance()->GetEntry(GET_MAP_ID(pZone->GetSceneID()));
		if(pInstance!=NULL && (DWORD64)pInstance->m_nLimitTime > timedx)
		{
			timeleft = (int)(((DWORD64)pInstance->m_nLimitTime - timedx)*0.001);
		}

		CountryWar::AckClientReqWarData ret;
		ret.set_scoresui(score.score_sui);
		ret.set_scoretang(score.score_tang);
		ret.set_contribute(info.contribute);
		ret.set_sort(info.sort);
		ret.set_bekilled(info.beKilled);
		ret.set_time(timeleft);


		std::map<int,CountryTaskInfo>::const_iterator it=info.taskInfo.begin();
		for(; it!=info.taskInfo.end(); ++it)
		{
			CountryWar::taskinfo* pItem = ret.add_item();
			pItem->set_id(it->first);
			pItem->set_count(it->second.targetCount);
			pItem->set_flag(it->second.flag);
		}
		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_COUNTRY_WAR_DATA);
}

//请求领取任务奖励
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_FITCH_TASK_REWARD)
{
		CGameZone *pZone = player->GetGameZone();
		if(pZone==NULL||pZone->GetSceneID()!=getMapID())
				return;

		CountryWar::ClientReqFitchTaskReWard req;
		CountryWar::AckFitchTaskReWard ret;
		req.ParseFromString(content);

		ret.set_id(req.id());
		bool result = ((CCountryWarCarbon*)(pZone))->fitchTaskPrize(player->GetID(),req.id());
		if(result)
		{
			ret.set_success(0);
		}
		else
		{
			ret.set_success(1);
		}
		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_COUNTRY_FITCH_TASK_REWARD);
		//奖励物品 发邮件
		if(result)
		{
			const CountryWarTaskConfigInfo* pTask = CountryWarLoader::GetInstance()->getTaskConfig(req.id());
			if(pTask)
			{
				//个人贡献度奖励
				if(pTask->contribute>0)
				{
					player->ChangeCountryContribution(pTask->contribute,true);
				}

				//阵营积分奖励
				if(pTask->score>0)
				{
					((CCountryWarCarbon*)(pZone))->addScore(player->getAttManage()->getValue<DWORD>(eCharCountry),pTask->score);
				}

				//物品奖励
				if(pTask->propID>0 && pTask->propCount>0)
				{
					int64 num = pTask->propCount;
					int64 id = pTask->propID | (num<<32);
					std::vector<int64> vGoods;
					vGoods.push_back(id);
					string strReward = InternationalLoader::GetInstance()->GetInterStr(18);
					MailMessageMgr::GetInstance()->SendSysMail(vGoods,player->GetName(),player->GetID(),strReward);
				}
			}
		}
}

//请求历史战绩
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_WAR_HISTORY)
{
		CountryWar::AckReqHistoryRecord ret;
		std::map<int,CountryWarRecord>::iterator it=m_historyRecord.begin();
		for(; it!=m_historyRecord.end(); ++it)
		{
			if(it->first==eCountryID_sui)
			{
				ret.set_suiwin(it->second.Win);
				ret.set_suilose(it->second.Lose);
			}
			if(it->first==eCountryID_tang)
			{
				ret.set_tangwin(it->second.Win);
				ret.set_tanglose(it->second.Lose);
			}
		}
		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_COUNTRY_WAR_HISTORY);
}

//请求占领五行柱
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_OCCUPY_ELEMENTS)
{
			CGameZone *pZone = player->GetGameZone();
			if(pZone==NULL||pZone->GetSceneID()!=getMapID())
					return;

			CountryWar::ReqOccupyElement req;
			CountryWar::AckReqOccupyElement ret;
			req.ParseFromString(content);

			//能否占领
			bool isSuccess = ((CCountryWarCarbon*)(pZone))->occupyElement(player->GetID(),req.id());
			if(isSuccess)
				ret.set_success(0);
			else
				ret.set_success(1);

			ret.set_id(req.id());

			REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_COUNTRY_OCCUPY_ELEMENTS);
}

//请求阵营战副本
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(GsCountryWarMgr,MSG_REQ_COUNTRY_WAR_MAP)
{
		//CountryWar::ReqOccupyElement req;

}

//世界服通知阵营战副本
DEF_MSG_SIMPLE_DEFINE_FUN(GsCountryWarMgr,MSG_SIM_WS2GS_COUNTRY_WAR_MAP)
{
		CountryWar::AckReqCountryWarMapID req;
		int len=0;
		char* content = message->GetBuffer(len);
		req.ParseFromArray(content,len);
		setMapID(req.id());
		DWORD64 curtime = CUtil::GetNowSecond();
		setTime(curtime);
}

DEF_MSG_SIMPLE_DEFINE_FUN(GsCountryWarMgr, MSG_SIM_WS2GM_COUNTRYBATTLEOPEN)
{
	CountryWar::WSToGMCountryBattle req;
	int len = 0;
	char* content = message->GetBuffer(len);

	req.ParseFromArray(content, len);

	for (int i = 0; i < req.charid_size(); ++i)
	{
		Smart_Ptr<Player> player;
		PlayerMessage::GetInstance()->GetPlayerByCharId(player, req.charid(i));
		if ((bool)player)
		{
			switch (req.wartype())
			{
			case eCountDownTimeCountryBoss:
				{
					player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Activity, eUIHLSmall_CountryBossBattle_Open), 1, req.flag());
				}
				break;
			case eCountDownTimeCountryWar:
				{
					player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_Activity, eUIHLSmall_CountryFightBattle_Open), 1, req.flag());
				}
				break;
			}
		}
	}
}







