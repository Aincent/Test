/*
 * BuffFactory.h
 *
 *  Created on: 2014年2月12日
 *      Author: helh
 */

#ifndef BUFFFACTORY_H_
#define BUFFFACTORY_H_

#include <map>
#include <set>
#include "../Object/Creature.h"

using namespace std;


#include "BuffBase.h"

class BuffFactory
{
public:
	// 创建一个BUFF
	static BuffBase* Create(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, int id, int num = 1);
	static BuffBase* Create(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, int id, int type, int alltime, int timer, int num = 1);

	// 销毁一个BUFF
	static void Destory(BuffBase* pBuffer);

	//销毁所有的对象
	static void DestroyAll();

	// 更新
	static void Update();

	static void InitObject();
	static BuffBase* GetBufferObject(int type);

private:
	static void CreateBufferObject(int type, int size);
	static void Recycle(BuffBase* pBuffer);

private:
	enum
	{
		eBUFFERINITMAXCOUNT = 500,	//	初始的BUFFER个数
	};

	// 需要移除的链表
	static std::vector<BuffBase*> m_removeList;

	static DWORD m_onlyID;

	static std::queue<BuffBase*> m_BuffForever;
	static std::queue<BuffBase*> m_BuffCount;
	static std::queue<BuffBase*> m_BuffOnceTimer;
	static std::queue<BuffBase*> m_BuffCapInvlTime;

};



#endif /* BUFFFACTORY_H_ */
