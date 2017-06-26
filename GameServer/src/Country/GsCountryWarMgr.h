/*
 * GsCountryWarMgr.h
 *
 *  Created on: 2015年4月17日
 *      Author: root
 */

#ifndef GSCOUNTRYWARMGR_H_
#define GSCOUNTRYWARMGR_H_
#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../QuickMessage.h"
#include "../MsgCommonClass.h"
#include "CountryDefine.h"
#include "Timer/TimerInterface.h"

using namespace std;
using namespace CommBaseOut;

class Player;
class GsCountryWarMgr : public Singleton<GsCountryWarMgr>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
		GsCountryWarMgr();
		virtual ~GsCountryWarMgr();
public:
		void setMapID(int64 map){m_mapID=map;}
		int64 getMapID(){return m_mapID;}

		void setTime(DWORD64 time){m_time=time;}
		DWORD64 getTime(){return m_time;}


		//得到胜利场次
		int getWinCount(int country);

		//通知阵营战结果
		void noticeWarResult(short int WorlserverID,int win,int lose);
protected:
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_COUNTRY_ELEMENTS_INFO);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_COUNTRY_WAR_DATA);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_COUNTRY_FITCH_TASK_REWARD);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_COUNTRY_WAR_HISTORY);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_COUNTRY_OCCUPY_ELEMENTS);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_COUNTRY_WAR_MAP);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GS_COUNTRY_WAR_MAP);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GS_WAR_RECORD);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_SIM_WS2GM_COUNTRYBATTLEOPEN);


		void loadConfig();

		//消息处理
		virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
		virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
		virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

private:
//		CLuaInterface m_pLua;
		TimerConnection m_stime;
		int64 m_mapID;//副本地图id
		DWORD64 m_time;//副本创建的时间
		std::map<int,CountryWarRecord> m_historyRecord;
};





















#endif /* GSCOUNTRYWARMGR_H_ */
