/*
 * EscortFileLoader.h
 *
 *  Created on: 2014年7月21日
 *      Author: root
 */

#ifndef ESCORTFILELOADER_H_
#define ESCORTFILELOADER_H_

//#include "define.h"
//#include <map>
//#include <vector>
//#include <string>
//#include "../Map/ConfigLoader/FieldInfo.h"
//#include "Smart_Ptr.h"
//#include "../Map/ConfigLoader/FieldMonster.h"
//#include "Singleton.h"
//
//using namespace CommBaseOut;
//using namespace std;
//
//struct sEscortEntry
//{
//	DWORD m_nEscortID;		//线路ID
//	DWORD m_nStartNpcID;
//	DWORD m_nEndNpcID;		//结束NPCID
//	vector<DWORD> m_nQualityExp;	//品质1宝箱经验
//	vector<DWORD> m_nQualityCost;	//选取品质1宝箱消耗元宝
//	vector<DWORD> m_nQuality1Probility;	//品质1宝箱概率,以1～10000为范围
//	vector<DWORD> m_nQuality2Probility;	//品质2宝箱概率
//	vector<DWORD> m_nQuality3Probility;	//品质3宝箱概率
//	vector<DWORD> m_nQuality4Probility;	//品质4宝箱概率
//	vector<DWORD> m_nQuality5Probility;	//品质5宝箱概率
//	DWORD m_nFreshCost;		//刷新需要的元宝
//	DWORD m_nFreeTimes;		//免费次数
//	DWORD m_nLimitTime;		//限制时间
//	DWORD m_nDayTimes;		//每天能作的次数
//	DWORD m_nImediaComplete;	//立即完成消耗元宝
//	DWORD m_nSpeedUpCost;	//加速消耗元宝
//	DWORD m_nSpeedUpTime;	//加速时间
//	vector<DWORD> m_nDoubleExpStartTime;//双倍经验开始时间
//	vector<DWORD> m_nDoubleExpEndTime;//双倍经验结束时间
//	vector<DWORD> m_nCarType;	//镖车ID
//	vector<DWORD> m_nByWayOfMap;	//途经地图
//	DWORD m_nSpeedUpRate;
//	DWORD m_expRate1;	//经验系数1   exp = (lv^exprate1 + lv * exprate2 + exprate3) * m_nQualityExp / 100 ,然后已5000四舍五入
//	DWORD m_expRate2;
//	DWORD m_expRate3;
//
//};
//
//const int No1EscortID = 1;  //护送黄冈路线ID  目前为止只有一条
//
//class EscortLoader : public Singleton<EscortLoader>
//{
//public:
//	EscortLoader();
//	~EscortLoader();
//public:
//	int InitEscort(string &file);
//
//	sEscortEntry *GetEscortEntry(DWORD id);
//
//private:
//	map<DWORD, sEscortEntry> m_EscortInfo;   // sEscortEntry - info
//};


#endif /* ESCORTFILELOADER_H_ */
