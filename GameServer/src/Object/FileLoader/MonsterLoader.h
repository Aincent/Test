/*
 * MonsterLoader.h
 *
 *  Created on: 2013��10��22��
 *      Author: helh
 */

#ifndef MONSTERLOADER_H_
#define MONSTERLOADER_H_

#include <map>
#include "OtherDefine.h"
#include "define.h"


using namespace std;
using namespace CommBaseOut;

typedef struct sMonsterInfo
{
	int id; //����id
	string name; //��������
	WORD lv; //����ȼ�
	BYTE quality; //2为精英, 3为boss,4为采集
	int mapId;
	int born_x;
	int born_y;
	int flushTime; //ˢ��ʱ��
	int aliveTime; // ���ʱ��
	WORD camp; // ��Ӫ
	WORD idleSpeed; //�ƶ��ٶ�
	WORD moveSpeed; //׷���ٶ�
	WORD baseAI; //基本ai表对应的ID
	WORD betterAI; //�м�ai
	vector<int> fallID; //掉落表ID，采集怪直接为物品ID
	int exp; //��������
	int score; //怪物积分
	DWORD maxHP; //���Ѫ��
	DWORD maxMP; //�����
	int minAttack; // ��С����
	int maxAttack; // ��󹥻�
	int minMagAttack; // ��С��������
	int maxMagAttack; // ���������
	int phyDefence; //�������
	int magDefence; //��������
	int hitRate; //����
	int avoidRate; //����
	int lucky; //幸运值
	int crackRate; //����
	int avoidCrack; //抗暴
	DWORD gatherTime;	//采集时间
	DWORD gatherTaskId;	//对应的任务ID

	sMonsterInfo():id(-1),lv(0),quality(0),mapId(0),born_x(0),born_y(0),flushTime(0),aliveTime(0),camp(0),idleSpeed(0),moveSpeed(0),baseAI(0),betterAI(0),
			exp(0),score(0),maxHP(0),maxMP(0),minAttack(0),maxAttack(0),minMagAttack(0),maxMagAttack(0),phyDefence(0),
			magDefence(0),hitRate(0),avoidRate(0),lucky(0),crackRate(0),avoidCrack(0),gatherTime(0),gatherTaskId(0)
	{

	}
}MonsterInfo;

class MonsterLoader
{
public:
	~MonsterLoader();

	static MonsterLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new MonsterLoader();
		}

		return m_instance;
	}

	int InitMonster(string &file);

	void GetMonsterInfo(int id, MonsterInfo & info)
	{
		map<int, MonsterInfo>::iterator it = m_allMonster.find(id);
		if(it != m_allMonster.end())
		{
			info = it->second;
		}
	}

	void DestoryInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	bool IsDropItem(int id)
	{
		map<int, MonsterInfo>::iterator it = m_allMonster.find(id);
		if(it != m_allMonster.end())
		{
			if(it->second.fallID.size() > 0)
			{
				return true;
			}
		}

		return false;
	}

private:
	MonsterLoader();

private:
	static MonsterLoader * m_instance;

	map<int, MonsterInfo> m_allMonster; // monsterid -- monsterinfo
};


#endif /* MONSTERLOADER_H_ */
