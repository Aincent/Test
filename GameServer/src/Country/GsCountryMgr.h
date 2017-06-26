/*
 * GsCountryMgr.h
 *
 *  Created on: 2015年3月18日
 *      Author: root
 */

#ifndef GSCOUNTRYMGR_H_
#define GSCOUNTRYMGR_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include <vector>
#include "../QuickMessage.h"
#include "MessageStruct/Country/Country.pb.h"
#include "MessageStruct/Country/CountryBoss.pb.h"
#include "../MsgCommonClass.h"
#include "CountryDefine.h"
#include "Timer/TimerInterface.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class GsCountryMgr : public Singleton<GsCountryMgr>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
		GsCountryMgr();
		virtual ~GsCountryMgr();

public:
		//根据道具金钱获取贡献度
		int getContributeByProp(int propID);
		int getContributeByMoney(int money);
		int getContributeByGolden(int golden);

		const CampShopInfo* getShopInfo(int id);
		const CampGradeInfo* getCampInfo(int lv);
		const CampJobPrize* getCampJobInfo(BYTE job);
		void GetShopInfo(std::map<int, CampShopInfo> & info);

		int getCampGrade(int id);
		void getCampAttr(int id,std::vector<std::pair<int,int> >& attrs);

		int getMoneyID(){return m_MoneyPropID;}
		int getGoldenID(){return m_GoldenPropID;}
		int getGoldenRate(){return m_GoldenRate;}

		void GetRandomPos(int64& nXPos,int64& nYPos,DWORD country);

		void GetRandomPos(int mapId,vector<CountryBossPos>& vec);

		//是否在安全区
		bool IsSafeZone(int64 nXPos,int64 nYPos,DWORD country);

		int GetNextBossTime(int num);

		int GetNextBossBornTime(int num);

		void GetBossMap(map<int,BossMap>& bossMap);

		int GetFlushConTime();

		int GetFlushExp(int lv,int rate);

		int GetFlushCamp(int count);

		int GetFlushCon();

		int GetSystemScore();

		int GetPersionScore();

		int GetKillEnemyScore();

		int GetKillBossScore(int nId);

		int GetBossNum(int nId);

		int GetFlushNewsTime();

		int GetEndKillExp(int lv,int count);

		int GetEndKillCredit(int lv,int count);

		int GetEndKillCon(int lv,int count);

		int GetHurtRankingExp(int lv, int rank, int count);

		int GetHurtRankingCredit(int lv, int rank, int count);

		int GetHurtRankingCon(int lv, int rank, int count);

		void GetBossPower(CountryBossPackage::BossHarmSettle& info,int64 nBossId,int nRank);

		int GetCampAwardExp(int lv,int count);

		int GetCampAwardMoney(int lv,int count);

		int GetGoodsId()
		{
			return m_nGoosId;
		}

		int GetGoodsNum()
		{
			return m_nGoodsNum;
		}

		void GetCampGoods(map<int,int>*& vec)
		{
			vec = &m_CampGoods;
		}

		int GetCampWinAwardExp(int lv);

		int GetCampWinAwardMoney(int lv);

		int GetCampMoney();

		// 获取加入推荐阵营奖励
		const JoinRcndAward& GetJoinRcndAward() const;
		const int GetShowCntyText(eCountryID) const;
		const int GetShowCntyText(DWORD) const;

protected:
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_COUNTRY_JUANXIAN);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_COUNTRY_JOB_DAYPRIZE);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_COUNTRY_SHOPINFO);
		DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_COUNTRY_BUY);
		DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GS2WS_COUNTRY_JUANXIAN);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_WS2GS_COUNTRY_LIVEUP);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_WS2GS_COUNTRY_BASEINFO);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_WS2GS_COUNTRY_RANK_RESET);
		DEF_MSG_SIMPLE_DECLARE_FUN_H(MSG_WS2GS_COUNTRY_ATTR_ALTER);

		DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_WS2GM_GETJOINAWARD); // 获取加入推荐阵营奖励
		
		void loadConfig();

		//LUA配置
		void loadLuaConfig();

		//捐献表
		void loadContribute();
		//陈营信息表
		void loadCampInfo();
		//官职奖励表
		void loadJobInfo();
		//陈营商店表
		void loadShop();

		void loadBossInfo();

		//初始化定时器
		void initTimer();
		void onTime(void * obj);

		virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
		virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
		virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

private:
		CLuaInterface m_pLua;
		std::map<int,int> m_propContributeInfo;
		std::map<int,int> m_moneyContributeInfo;
		std::map<int,int> m_goldenContributeInfo;
		std::map<int,CampGradeInfo> m_CampGradeList;
		std::map<unsigned char,CampJobPrize> m_JobList;
		std::map<int,CampShopInfo> m_shopInfo;
		std::map<int,int> m_campbaseInfo;
		map<int,BossMap> m_campBoss;
		map<int,int> m_KillBossScore;
		map<int,int> m_BossNum; //boss出场顺序
		map<int,int> m_CampGoods;
		TimerConnection m_stime;
		int m_kingFightPower; //争夺霸主战力值
		int m_MoneyPropID;	//捐献铜钱道具id
		int m_GoldenPropID; //捐献元宝道具id
		int m_GoldenRate;//元宝捐献的资产增加倍率
		int m_nFirstTime;
		int m_nSecondTime;
		int m_nThirdTime;
		int m_nFlushConTime;
		int m_nGoosId;
		int m_nGoodsNum;

		// 加入推荐阵营奖励 rcnd:recommend 推荐
		JoinRcndAward m_joinRcndAward;
		//
		map<eCountryID, int> m_showCntyText;
};

#endif /* GSCOUNTRYMGR_H_ */
