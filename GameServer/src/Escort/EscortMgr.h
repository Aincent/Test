/*
 * EscortMgr.h
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#ifndef ESCORTMGR_H_
#define ESCORTMGR_H_

//#include "define.h"
//#include "Singleton.h"
//#include "../Object/Player/Player.h"
//#include <map>
//#include "./EventSet/EventArgs.h"
//#include "../Daily/Daily.h"
//#include "EscortFileLoader.h"
//
//using namespace std;
//using namespace CommBaseOut;
//
//#define ESCORTRANDOMRATE	10000	//宝箱随机基数
//
//enum eCarType
//{
//	eEscortBronze,		//青铜
//	eEscortSilver,			//白银
//	eEscortGold,				//黄金
//	eEscortPlatinum,			//白金
//	eEscortDiamond,			//钻石
//	eEscortMax
//};
//
//struct sEscortData
//{
//	DWORD 				line;					//护送线路ID
//	eCarType 			quality;				//宝箱品质
//	Smart_Ptr<Monster> carMon;				//运镖玩家
//	DWORD 				robTimes;				//被抢次数
//	bool 				isComplete;				//是否完成
//	DWORD				leftTimes;				//剩余时间
//	DWORD				leftSpeedUpTime;		//加速剩余时间
//
//	sEscortData()
//	{
//		memset(this,0,sizeof(sEscortData));
//	}
//};
//
//class EscortMgr : public Singleton<EscortMgr>
//{
//public:
//    EscortMgr();
//    ~EscortMgr();
//
//
//    typedef std::map<int64, sEscortData*> mapEscort;
//
//
//    //
//   void SetEscortToDB(Smart_Ptr<Player> player,PlayerInfo::EscortInfo *info);
//
//    void InitEscort(Smart_Ptr<Player> player, PlayerInfo::EscortInfo &info);
//    sEscortData *GetPlayerEscortData(int64 id);
//
//    //随机产生宝箱品质
//    int GenenateQuality(Smart_Ptr<Player> &player, DWORD escortId, DWORD &quality);
//
//    int SetEscortQuality(Smart_Ptr<Player> &player,eCarType quality);
//    void SetEscortMonster(int64 id,Smart_Ptr<Monster> mon);
//    void AddRobTimes(Smart_Ptr<Player> player);
//    Smart_Ptr<Monster> GetEscortMonster(Smart_Ptr<Player> player);
//    int GetLeftTime(Smart_Ptr<Player> player);
//    int GetEscortLine(Smart_Ptr<Player> player);
//    int GetEscortRobTimes(Smart_Ptr<Player> player);
//
//    //开始押送
//    int StartEscort(Smart_Ptr<Player> &player, DWORD settime = 0, bool needIncrease = true);
//    //押送完成
//    void EndEscort(Smart_Ptr<Player> &player);
//    //判断是否完成
//    bool IsComplete(Smart_Ptr<Player> player);
//    //立即完成
//    bool ImediaComplete(Smart_Ptr<Player> &player);
//    //加速
//    bool SpeedUpEscort(Smart_Ptr<Player> &player);
//    //放弃
//    void AbandonEscort(Smart_Ptr<Player> &player);
//    //任务结束
//    bool TaskIsComplete(Smart_Ptr<Player> player);
//    //设置任务失败
//    void SetEscortTaskComplete(Smart_Ptr<Player> player, bool complete = false);
//
//    //
//    bool UpdateEscortLeftTime(Smart_Ptr<Player> &player,DWORD tick);
//
//    //更新抢劫信息
//   void UpdateEscortInfo(Smart_Ptr<Player> player);
//   //发送抢劫信息
//   void SendRobInfo(Smart_Ptr<Player> beRober, Smart_Ptr<Player> rober);
////玩家下线清除数据指针
//   void ClearPlayerData(Smart_Ptr<Player> &player);
//   //更新加速剩余时间
//   void UpdateSpeedUpTime(Smart_Ptr<Player> &player, int value);
//   //压镖剩余次数
//   DWORD GetEscortLeftTime(Smart_Ptr<Player> &player);
//
//   //保存运镖数据
//   bool SaveEscortData(const EventArgs & e);
//
//   bool IsCanEnterEscort(const Smart_Ptr<Player> &player);
//   	   //找回 护送黄冈 数据 相关
//   int RecoverResource(const Smart_Ptr<Player> &player, int* num, sRecoverResource &tmpInfo);
//   int CalAddExp(const int &haveNum ,const int &lv , const sEscortEntry * entry);
//
//private:
//    mapEscort	m_mapPlayersEscort;
//    EventConnection m_logoutEvent;
//
//};
//
//#define sEscortMgr EscortMgr::GetInstance()


#endif /* ESCORTMGR_H_ */
