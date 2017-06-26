/*
 * AILoader.h
 *
 *  Created on: 2013��10��23��
 *      Author: helh
 */

#ifndef AILOADER_H_
#define AILOADER_H_

#include <map>
#include "define.h"

using namespace std;
using namespace CommBaseOut;

typedef struct sBaseAIInfo
{
	int id;
	WORD atkDistance; //��������
	WORD viewDistance; //��Ұ����
	WORD moveDistance; // �ƶ�����
	WORD runDistance; // ׷������
	WORD stayTime; // ͣ��ʱ��
	BYTE atkType; // 1.主动攻击，2被动攻击
	WORD atkTime; // �������
	int skillID;
	vector<int> state;

	sBaseAIInfo():id(-1),atkDistance(0),viewDistance(0),moveDistance(0),runDistance(0),stayTime(0),atkType(0),atkTime(0),skillID(-1)
	{

	}
}BaseAIInfo;

typedef struct sBetterAIInfo
{
	int id;
	WORD cycleTime; // ��ѯʱ��
	BYTE firstSelect; //����ѡ��Ŀ�귽ʽ
	BYTE selectRate; //�л�Ŀ�꼸��
	BYTE firstType; // ��һ�ִ�������
	BYTE firstParam; //�������Ͳ���
	WORD firstAction; // ������Ķ���
	BYTE firstRate; // ��������
	BYTE secondType; // �ڶ��ִ�������
	BYTE secondParam; // ��������
	WORD secondAction; //����
	BYTE secondRate; //����
	bool callSkill; //�Ƿ����ٻ�����
	BYTE callParam; // �ٻ�����
	BYTE callRate; // �ٻ�����
	int callSkillID; //�ٻ�����id
	int callDelay; //�ٻ��ӳ�ʱ��
	int fSkillID; // ��һ����id
	int fSkillRate; // ��һ�����ͷż���
	int sSkillID; // �ڶ�����id
	int sSkillRate; // �ڶ������ͷż���
	int tSkillID; // ������
	int tSkillRate; // ��������ͷż���

	sBetterAIInfo():id(-1),cycleTime(0),firstSelect(0),selectRate(0),firstType(0),firstParam(0),firstAction(0),firstRate(0),secondType(0),
			secondParam(0),secondAction(0),secondRate(0),callSkill(0),callParam(0),callRate(0),callSkillID(0),callDelay(0),fSkillID(0),
			fSkillRate(0),sSkillID(0),sSkillRate(0),tSkillID(0),tSkillRate(0)
	{

	}
}BetterAIInfo;


enum EArenaSkil
{
	eSkill_A = 0,
	eSkill_B ,
	eSkill_C ,
	eSkill_D ,
	eSkill_E ,
	eSkill_F ,
	eSkill_G ,
	eSkill_H ,
	eSkill_I ,
	eSkill_Max,
};

typedef struct sArenaInfo
{
	int id;
	WORD nCycleTime;
	vector<int> nSkill;
	sArenaInfo():id(-1),nCycleTime(0)
	{
		nSkill.clear();
	}
}ArenaInfo;

class AILoader
{
public:
	~AILoader();

	int Init(string &file);

	//����ai
	int InitBaseAI(string &file);

	//�м�ai
	int InitBetterAI(string &file);

	int InitArena(string &file);

	BaseAIInfo * GetBaseAIByid(int id)
	{
		map<int, BaseAIInfo>::iterator it = m_baseAI.find(id);
		if(it != m_baseAI.end())
		{
			return &it->second;
		}

		return 0;
	}

	int GetAttackType(int id)
	{
		map<int, BaseAIInfo>::iterator it = m_baseAI.find(id);
		if(it == m_baseAI.end())
		{
			 return 0;
		}

		return it->second.atkType;
	}

	int GetViewDistance(int id)
	{
		map<int, BaseAIInfo>::iterator it = m_baseAI.find(id);
		if(it == m_baseAI.end())
		{
			 return 0;
		}

		return it->second.viewDistance;
	}

	int GetAttackDistance(int id)
	{
		map<int, BaseAIInfo>::iterator it = m_baseAI.find(id);
		if(it == m_baseAI.end())
		{
			 return 0;
		}

		return it->second.atkDistance;
	}

 int GetRunDistance(int id)
  {
	 map<int, BaseAIInfo>::iterator it = m_baseAI.find(id);
	 if(it == m_baseAI.end())
	  {
	 		return 0;
	  }

	 return it->second.runDistance;
  }


	int GetSkillID(int aiid)
	{
		map<int, BaseAIInfo>::iterator it = m_baseAI.find(aiid);
		if(it == m_baseAI.end())
		{
			 return 0;
		}

		return it->second.skillID;
	}

	BetterAIInfo *GetBetterAIByid(int id)
	{
		map<int, BetterAIInfo>::iterator it = m_betterAI.find(id);
		if(it != m_betterAI.end())
		{
			return &it->second;
		}

		return 0;
	}

	ArenaInfo *GetArenaAIByid(int id)
	{
		map<int, ArenaInfo>::iterator it = m_arenaAI.find(id);
		if(it != m_arenaAI.end())
		{
			return &it->second;
		}

		return 0;
	}

	static AILoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new AILoader();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

private:
	AILoader();

private:

	static AILoader * m_instance;

	map<int, BaseAIInfo> m_baseAI;   // ai id -   aiinfo
	map<int, BetterAIInfo> m_betterAI;  // aiid - aiinfo
	map<int, ArenaInfo> m_arenaAI;  // aiid - aiinfo
};


#endif /* AILOADER_H_ */
