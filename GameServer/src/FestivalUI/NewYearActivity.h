/*
 * NewYearActivity.h
 *
 *  Created on: Jan 18, 2016
 *      Author: root
 */

#ifndef NEWYEARACTIVITY_H_
#define NEWYEARACTIVITY_H_

#include "define.h"
#include "MessageStruct/CharLogin/ChristmasDB.pb.h"
#include "MessageStruct/ServerReturn4Int.pb.h"
#include "MessageCommonRet.h"
#include "BitArray.h"

using namespace std;
using namespace CommBaseOut;

class Player;

class NewYearActivity
{
public:
	NewYearActivity(Player& owner);
	~NewYearActivity();

	void ReInit();
	void Release();

public:
	void initFromDB(ChristmasDB::ChristmasAllData& dbData);

	void getToDB(ChristmasDB::ChristmasAllData& dbData);

	void playerLogin();

	void clientGetLoginData(ServerReturn::ServerFourInt& retrunData);

	ServerRet receiveLoginReward(int index);

	ServerRet buyHorseReward(int& skinID);

	bool getHorseData(){return m_HorseReceive;}

	void gmSetContinueDay(int day);

protected:
	void setBitData(int data,CBitArray& bitData);

	int getBitData(CBitArray& bitData);

	int changeLoginDay();

	int getMonthDays(int day);

	int getLoginDay();

	ServerRet loginReward();

	ServerRet continueReward();

private:
	Player&	  m_Owner;
	CBitArray m_LoginDay;
	CBitArray m_RewardData;
	bool	  m_HorseReceive;
	bool	  m_ContiReceive;
};


#endif /* NEWYEARACTIVITY_H_ */
