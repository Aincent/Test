/*
 * TreasonKingReward.h
 *
 *  Created on: 2014年11月27日
 *      Author: root
 */

#ifndef TREASONKINGREWARD_H_
#define TREASONKINGREWARD_H_

#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "Smart_Ptr.h"
#include "define.h"
#include "BitArray.h"

using namespace std;
using namespace CommBaseOut;

class Player;

const int TotalLevels = 9;  //总共有层数

class TreasonKingReward
{
public:
	TreasonKingReward(Player * player);
	~TreasonKingReward();

	void ReInit();
	void Release();

	void InitTreasonKingReward(int reward);

	int GetRewardInfo();

	int GetLevelsReward(int nLevels);

	void ResetTreasonKingReward();

private:
	Player * m_owner;
	CBitArray m_Reward;
};


#endif /* TREASONKINGREWARD_H_ */
