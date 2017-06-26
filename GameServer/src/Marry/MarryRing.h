/*
 * MarryRing.h
 *
 *  Created on: Sep 19, 2015
 *      Author: root
 */

#ifndef MARRYRING_H_
#define MARRYRING_H_

#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/Marry/MarryUI.pb.h"
#include "../Horse/HorseLoader/HorseLoader.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageCommonRet.h"
#include "BitArray.h"

using namespace CommBaseOut;
using namespace std;

class Player;
class MarryRingData;


enum eMarryVanceType
{
	eMarryVanceTypeNull   = 0,
	eMarryVanceTypeNormal = 1,
	eMarryVanceTypeAuto   = 2,
};

struct MarryExpStatistics
{
	MarryExpStatistics() : m_Counts(0),m_Exp(0){}
	int m_Counts;
	int m_Exp;
};

class MarryRing
{
public:
	MarryRing(Player& owner);

	~MarryRing();

	void ReInit();
	void Release();

public:
	void initFromDB(PlayerInfo::RingDBData& ringProto);

	void getDataToDB(PlayerInfo::RingDBData& ringProto);

	//上线计算属性
	void onlineCalculateAtt();

	//升阶
	void ascendeOrder(MarryUI::RequestAdvanceMarryRing& protoRequest,MarryUI::UpMarryStepReturn& protoReturn);

	//领取结婚奖励
	void receiveMarryReward(DWORD id,ServerReturn::ServerRetInt& protoReurn);

	//结婚
	void marrySuccess(int64 otherPlayerID,string otherName);

	//离婚
	void divorceSuccess();

	int getRewardData();

	//请求婚戒信息
	void requestRingData(MarryUI::RingInfo& protoToClient);

	//上线世界取数据返回设置
	void onlineSetData(int64 otherPlayerID,string otherName,DWORD64 marryTime);

protected:
	//计算一次战斗力
	int calculateFight();

	void getAttValue(int id,map<ECharAttrType,int64>& attList);

	//属性发生改变
	void attChange(DWORD lastID,DWORD currentID);

	//一次升阶加的经验
	int randExp(const MarryRingData& info,eCommonRateType& type);

	eCommonRateType getCritType(const MarryRingData& info);

	//一次升阶扣钱
	ServerRet oneAdvanceBuckle(DWORD currentID,map<DWORD,int>& itemList,int& golden,int& money,bool isAuto);

	//一次升阶结果
	ServerRet oneAdvanceExp(DWORD& currentID,DWORD& currentExp,DWORD& allExp,map<eCommonRateType,MarryExpStatistics>& resultExpList);

	//获得结婚天数
	DWORD getMarryDays();

private:
	Player& 		m_Owner;
	DWORD   		m_ID;
	DWORD   		m_Exp;			//经验大过当前星经验时就升星
	DWORD64			m_MarryTime;	//结婚日期
	CBitArray 		m_Reward;		//奖励领情况
};


#endif /* MARRYRING_H_ */
