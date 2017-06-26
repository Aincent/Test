/*
 * AIStateArena.h
 *
 *  Created on: 2014年10月14日
 *      Author: root
 */

#ifndef AISTATEARENA_H_
#define AISTATEARENA_H_

#include "AIState.h"
#include <vector>
#include "../FileLoader/AILoader.h"

using namespace std;
using namespace CommBaseOut;

class AIStateArena : public AIState
{
public:
	AIStateArena(AIBrain * owner);
	~AIStateArena();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();

	//判断cd
	bool IsCanFire(int id)
	{
		if(id < 0 || id >= eSkill_Max)
			return false;

		int64 nowTime = CUtil::GetNowSecond();
		if(nowTime > m_cdTime[id])
			return true;

		return false;
	}
	//加入cd
	void StartCD(int id, int time)
	{
		if(id < 0 || id >= eSkill_Max || time <= 0)
			return;

		m_cdTime[id] = CUtil::GetNowSecond() + time;
	}

private:

	vector<int64> m_cdTime; //技能的cd频道
	int m_attackTime; //攻击频率
};



#endif /* AISTATEARENA_H_ */
