/*
 * NPC.h
 *
 *  Created on: 2013年10月21日
 *      Author: helh
 */

#ifndef NPC_H_
#define NPC_H_

#include "../Object.h"

using namespace std;

class NPC : public Object
{
public:
	NPC(eObjType type);
	~NPC();

	void SetNpcID(int id) {	 m_npcID = id; }
	int GetNpcID() {	 return m_npcID; }

private:
	int m_npcID;
};

/*
 * npc 限制和功能类
 *
 */
class NPCFuntion
{
public:
	~NPCFuntion();

	static NPCFuntion * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new NPCFuntion();
		}

		return m_instance;
	}

	void DestoryInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	bool IsLimit();

private:
	NPCFuntion();

private:

	static NPCFuntion * m_instance;
};


#endif /* NPC_H_ */
