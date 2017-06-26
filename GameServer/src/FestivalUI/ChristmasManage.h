
#ifndef BREAKEGGMANAGE_H_
#define BREAKEGGMANAGE_H_

#include "define.h"
#include "MessageStruct/CharLogin/ChristmasDB.pb.h"
#include "MessageStruct/ChristmasActivity/ChristmasActivity.pb.h"
#include "BitArray.h"
#include "MessageCommonRet.h"
#include "Timer/TimerInterface.h"

using namespace std;
using namespace CommBaseOut;

class Player;

struct BreakEggData
{
	BreakEggData(){}
	CBitArray m_EggBreakInfo;
};

struct LoginData
{
	CBitArray m_LoginDay;
	CBitArray m_RewardData;
};

struct LimitSingleData
{
	LimitSingleData() : m_ID(0),m_LimitNum(0){}
	DWORD m_ID;
	WORD  m_LimitNum;
};

struct LimitBuyData
{
	LimitBuyData() : m_ReflushTime(0){}
	vector<LimitSingleData> m_DataList;
	DWORD64 				m_ReflushTime;
};

class ChristmasManage
{
public:
	ChristmasManage(Player&	player);
	~ChristmasManage();

	void ReInit();
	void Release();

public:
	void initFromDB(ChristmasDB::ChristmasAllData& dbData);

	void getDBdata(ChristmasDB::ChristmasAllData& dbData);

	ServerRet reflushEggTime();

	void breakEgg(int index,ChristmasActivity::BreakEggReturn& returnMsg);

	void clientGetEggData(ChristmasActivity::BreakEggMsg& returnMsg);

	void playerLogin();

	void clientGetLoginData(ChristmasActivity::ChristmasLoginMsg& returnMsg);

	ServerRet receiveLoginReward(int index);

	void clientGetLimitData(ChristmasActivity::ChristmasLimitBuyInfo& returnMsg);

	ServerRet buyLimitItem(DWORD id);

	void clientReflushLimit(ChristmasActivity::LimitBuyReturn& returnMsg);

	ServerRet clientExchangeItem(DWORD id);

	void gmSetLoginDay(DWORD day);

protected:
	void initEggData(ChristmasDB::BreakEggDB& dbData);

	void initLoginData(ChristmasDB::ChristmasLoginDB& dbData);

	void initLimitData(ChristmasDB::LimitDataAll& dbData);

	void setBitData(int data,CBitArray& bitData);

	int getBitData(CBitArray& bitData);

	short getNotBreakCounts();

	int getLoginDay();

	void reflushLimitData();

	LimitSingleData* getLimitDataByID(DWORD id);

private:
	BreakEggData m_EggData;
	LoginData	 m_LoginData;
	LimitBuyData m_LimitData;
	Player&		 m_Owner;
};


#endif /* BREAKEGGMANAGE_H_ */
