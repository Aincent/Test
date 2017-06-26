/*
 * PetStateAttack.h
 *
 *  Created on: 2014年12月8日
 *      Author: root
 */

#ifndef PETSTATEATTACK_H_
#define PETSTATEATTACK_H_

#include "PetState.h"

class Player;

class PetStateAttack : public PetState
{
public:
	PetStateAttack(PetBrain * owner);
	~PetStateAttack();

	virtual void Update(DWORD dwDeltaTick);
	virtual void BeginState();
	virtual void EndState();

	int GetAttackTime() { 	return m_attackTime; }

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
	int m_attackTime; //ս������ʱ��
	vector<int64> m_cdTime; //技能的cd频道
};


#endif /* PETSTATEATTACK_H_ */
