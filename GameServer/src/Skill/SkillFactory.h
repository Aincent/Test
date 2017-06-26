/*
 * SkillFactory.h
 *
 *  Created on: 2014年1月21日
 *      Author: helh
 */

#ifndef SKILLFACTORY_H_
#define SKILLFACTORY_H_

#include "SkillBase.h"
#include "../Object/Creature.h"
#include "Common.h"

using namespace std;
using namespace CommBaseOut;

class CSkillFactory
{
	enum
	{
		eSKILLINITMAXCOUNT = 5000,//	初始的技能个数
	};

public:
	// 帧更新
	static void Update();

	static CSkillBase* Create(int id, Smart_Ptr<CreatureObj> & caster,int key, int xpos, int ypos);

	static void FreeSkill(CSkillBase* pBase);

	static void InitObject();

	static CSkillBase* GetSkillObject(int type);

	static void DestroyAll();

private:
	static void CreateObject(int type, int size);

	static void Recycle(CSkillBase* pBase);

	static void DeleteSkill(CSkillBase* pBase);
	// 即将要删除的技能链表
	static std::vector<CSkillBase*> g_delSkillList;
	static DWORD m_onlyID;

	static std::queue<CSkillBase*> m_SkillBlink;
	static std::queue<CSkillBase*> m_SkillDelay;
	static std::queue<CSkillBase*> m_SkillNest;
	static std::queue<CSkillBase*> m_SkillIntonate;
};


#endif /* SKILLFACTORY_H_ */
